"""Reads the project's source code for the Documentation tab.

Finds every function in the project (MATLAB, Python, JavaScript, C and
C++, shell scripts, and the MATLAB Function blocks of the Simulink
diagrams), reads its documentation (help text, docstring, JSDoc or the
comment above it) and splits its code into the blocks its comments
describe.

parse_file returns a file record and its entries:

    file   {"path", "lang", "lines", "entries": [ids], ...}
    entry  {"id", "name", "qual", "kind", "lang", "file", "line",
            "start", "body", "end", "sig", "doc", "parent", "chunks",
            "refs", ...}

Lines are numbered from 1. An entry's start includes the comments
above it, line is its declaration, body is the first line after its
signature and documentation, and end is its last line. Chunks are the
blocks of its body: {"title", "text", "start", "code": [first, last],
"depth"}, or {"child": id, "start", "end"} for a function defined
inside it (documented on its own).

cross_reference then links the entries: what each one calls, what
calls it, and the console's HTTP routes and MATLAB commands.

Part of the SPOT Console (webapp/app.py starts it).
"""

import ast
import bisect
import io
import re
import tokenize
import xml.etree.ElementTree as ElementTree
import zipfile
from pathlib import PurePosixPath


# Languages

# File extension -> language (the editor and the highlighter use the
# same names).
EXTENSIONS = {
    ".m": "matlab", ".py": "python", ".js": "javascript", ".mjs": "javascript",
    ".c": "c", ".h": "cpp", ".cc": "cpp", ".cpp": "cpp", ".hpp": "cpp", ".ino": "arduino",
    ".sh": "shell", ".service": "ini", ".md": "markdown", ".css": "css", ".html": "html",
    ".xml": "xml", ".json": "json", ".txt": "text", ".rc": "text",
}
FILE_NAMES = {"Makefile": "makefile"}

# Languages whose functions are found; the others are shown as files.
CODE_LANGUAGES = ("matlab", "python", "javascript", "c", "cpp", "arduino", "shell")

C_LIKE = ("javascript", "c", "cpp", "arduino")
LINE_COMMENT = {"matlab": "%", "python": "#", "shell": "#", "makefile": "#", "ini": "#",
                "javascript": "//", "c": "//", "cpp": "//", "arduino": "//"}

# Blocks longer than this are split at the comments inside them.
SPLIT_LINES = 30           # [lines]
MAX_DEPTH = 3              # nested splits, at most
TITLE_CHARS = 60           # a section title is at most this long
SUMMARY_CHARS = 240        # an entry's one-line summary, at most
SIGNATURE_CHARS = 300      # a signature, at most


def language_of(path):
    """Return the language of a file from its name, or None."""
    name = PurePosixPath(path).name
    if name in FILE_NAMES:
        return FILE_NAMES[name]
    return EXTENSIONS.get(PurePosixPath(path).suffix.lower())


def make_id(path, qual_local):
    """Return an entry's id: its file and its name within the file."""
    return "%s#%s" % (path, qual_local) if qual_local else path


def first_sentence(text):
    """Return the first sentence of some text, on one line."""
    flat = " ".join(text.split())
    match = re.match(r"(.+?[.!?])(\s|$)", flat)
    out = match.group(1) if match else flat
    if len(out) > SUMMARY_CHARS:
        out = out[:SUMMARY_CHARS - 1].rstrip() + "…"
    return out


def indent_of(line):
    """Return a line's indentation in columns (a tab is 4)."""
    width = 0
    for c in line:
        if c == " ":
            width += 1
        elif c == "\t":
            width += 4
        else:
            break
    return width


def new_entry(path, lang, name, qual, local, kind, line, end):
    """Return a new entry record with the common fields set."""
    return {"id": make_id(path, local), "name": name, "qual": qual, "local": local,
            "kind": kind, "lang": lang, "file": path, "line": line, "start": line,
            "body": line + 1, "end": end, "sig": "", "doc": None, "parent": None,
            "children": [], "chunks": [], "refs": [], "strings": []}


def signature_text(text):
    """Return a signature on one line, with its spaces tidied."""
    flat = " ".join(text.split())
    flat = re.sub(r"([(\[{]) ", r"\1", flat)
    flat = re.sub(r" ([)\]}])", r"\1", flat)
    if len(flat) > SIGNATURE_CHARS:
        flat = flat[:SIGNATURE_CHARS - 1] + "…"
    return flat


# Line kinds
#
# Every parser describes each line of its file for the chunker: "b"
# blank, "k" code, "c" a comment on a line of its own, "s" a MATLAB
# section line (%%). Index 0 is unused, so the lists are indexed by
# line number.


class LineInfo:
    """What each line of a file is, and its comment's text."""

    def __init__(self, lines):
        """Start with every line blank or code, and no comments."""
        self.lines = lines
        self.kind = ["b"] + ["b" if not ln.strip() else "k" for ln in lines]
        self.text = [""] * (len(lines) + 1)
        self.indent = [0] + [indent_of(ln) for ln in lines]

    def comment(self, number, text, kind="c"):
        """Mark a line as a comment with this text."""
        self.kind[number] = kind
        self.text[number] = text

    def count(self):
        """Return the number of lines."""
        return len(self.lines)


def comment_body(text):
    """Return a comment's text without the space after its marker."""
    return text[1:] if text.startswith(" ") else text


# Chunks


def title_text(text):
    """Return a comment's text without ruler characters (----)."""
    return text.strip().strip("-=*#~ ").strip()


def is_title(text):
    """Return whether a one-line comment reads as a section title."""
    ruler = re.match(r"^\s*[-=*#~]{4,}", text) is not None
    text = title_text(text)
    return (0 < len(text) <= TITLE_CHARS and not text.endswith((".", ":", ",", ";"))
            and (ruler or not text.startswith(("TODO", "NOTE", "eslint", "noqa"))))


def chunk_body(info, first, last, children, lang):
    """Split lines first..last into the blocks their comments describe.

    Args:
        info: The file's LineInfo.
        first: The body's first line.
        last: The body's last line.
        children: (start, end, id) of the functions defined inside, in
            order; each becomes a placeholder.
        lang: The language (MATLAB marks its sections with %%).

    Returns:
        A list of chunks and child placeholders, in line order.
    """
    items = []
    children = sorted(children)
    child_starts = {c[0]: c for c in children}

    # The body's own indentation: its least-indented line of code or
    # comment, outside the functions inside it.
    inside = set()
    for start, end, _ in children:
        inside.update(range(start, end + 1))
    levels = [info.indent[n] for n in range(first, last + 1)
              if info.kind[n] in ("k", "c") and n not in inside]
    base = min(levels) if levels else 0

    state = {"cur": None, "title": None}

    def close():
        """End the current chunk, trimming blank lines off its end."""
        cur = state["cur"]
        state["cur"] = None
        if cur is None:
            return
        end = cur["end"]
        while end >= cur["code"][0] and info.kind[end] == "b":
            end -= 1
        if end < cur["code"][0]:
            cur["code"] = None
            cur["end"] = cur["start"] + max(0, len(cur["text"].split("\n")) - 1)
        else:
            cur["code"][1] = end
            cur["end"] = end
        if cur["code"] is None and not cur["text"]:
            return
        items.append(cur)

    def open_chunk(start, text, code_start):
        """Start a chunk with this comment text."""
        close()
        state["cur"] = {"title": state["title"], "text": text, "start": start,
                        "code": [code_start, code_start - 1], "end": code_start - 1,
                        "depth": 0}
        state["title"] = None

    n = first
    while n <= last:
        # A function defined here: a placeholder for it.
        if n in child_starts:
            start, end, child_id = child_starts[n]
            close()
            items.append({"child": child_id, "start": start, "end": end})
            n = end + 1
            continue

        kind = info.kind[n]

        # A MATLAB section (%% Title): its title goes on the next chunk.
        if kind == "s":
            close()
            state["title"] = info.text[n].strip() or None
            n += 1
            continue

        # A comment of its own at the body's level starts a chunk; a
        # lone short one between blank lines is a section title.
        if kind == "c" and info.indent[n] <= base:
            run_end = n
            while (run_end + 1 <= last and info.kind[run_end + 1] == "c"
                   and info.indent[run_end + 1] == info.indent[n]
                   and run_end + 1 not in child_starts):
                run_end += 1
            text = "\n".join(info.text[k] for k in range(n, run_end + 1)).strip("\n")
            lone = (run_end == n and lang != "matlab"
                    and (n == first or info.kind[n - 1] == "b")
                    and (n + 1 > last or info.kind[n + 1] == "b"))
            if lone and is_title(text):
                close()
                state["title"] = title_text(text)
            else:
                open_chunk(n, text, run_end + 1)
            n = run_end + 1
            continue

        # Code, blank lines and deeper comments join the current chunk.
        if state["cur"] is None:
            if kind == "b":
                n += 1
                continue
            open_chunk(n, "", n)
        state["cur"]["end"] = n
        n += 1
    close()

    # Long chunks are split at the comments inside them.
    out = []
    for item in items:
        if "child" in item or not item["code"]:
            out.append(item)
        else:
            out.extend(split_chunk(info, item, 0))
    return out


def split_chunk(info, chunk, depth):
    """Split a long chunk at the comments of its nested blocks."""
    first, last = chunk["code"]
    if last - first + 1 <= SPLIT_LINES or depth >= MAX_DEPTH:
        return [chunk]

    # The chunk's own level, and the least-indented comment deeper than
    # it that starts a block (after a blank line or a shallower line).
    code_levels = [info.indent[n] for n in range(first, last + 1) if info.kind[n] == "k"]
    if not code_levels:
        return [chunk]
    own = min(code_levels)
    starts = []
    for n in range(first + 1, last + 1):
        if info.kind[n] == "c" and info.indent[n] > own and info.kind[n - 1] != "c":
            prev = info.kind[n - 1]
            if prev == "b" or info.indent[n - 1] < info.indent[n]:
                starts.append(n)
    if not starts:
        return [chunk]
    level = min(info.indent[n] for n in starts)
    starts = [n for n in starts if info.indent[n] == level]

    # The pieces: before the first nested comment, then one per nested
    # comment, and a piece for code back at the outer level.
    pieces = []
    cur = dict(chunk, code=[first, first - 1])
    n = first
    while n <= last:
        if n in starts:
            run_end = n
            while (run_end + 1 <= last and info.kind[run_end + 1] == "c"
                   and info.indent[run_end + 1] == level):
                run_end += 1
            pieces.append(cur)
            cur = {"title": None, "depth": depth + 1, "start": n, "end": run_end,
                   "text": "\n".join(info.text[k] for k in range(n, run_end + 1)).strip("\n"),
                   "code": [run_end + 1, run_end]}
            n = run_end + 1
            continue
        if (cur["depth"] > depth and info.kind[n] == "k" and info.indent[n] < level):
            pieces.append(cur)
            cur = {"title": None, "depth": depth, "start": n, "end": n - 1, "text": "",
                   "code": [n, n - 1], "cont": True}
        cur["code"][1] = n
        cur["end"] = n
        n += 1
    pieces.append(cur)

    # Trim blank lines; drop empty pieces; split again where needed.
    out = []
    for piece in pieces:
        a, b = piece["code"]
        while b >= a and info.kind[b] == "b":
            b -= 1
        while a <= b and info.kind[a] == "b":
            a += 1
        piece["code"] = [a, b] if a <= b else None
        if piece["code"] is None and not piece["text"]:
            continue
        if piece["code"] and piece["depth"] > depth:
            out.extend(split_chunk(info, piece, depth + 1))
        else:
            out.append(piece)
    return out


# Documentation text
#
# Every language's documentation becomes the same structure:
#     {"summary": "...", "blocks": [...]}
# with blocks
#     {"type": "para", "text"}               a paragraph
#     {"type": "pre", "text"}                lines kept as they are
#     {"type": "params", "title", "items": [{"name", "type", "text",
#                                            "sub": [...]}]}
#     {"type": "list", "title", "ordered", "items": [text]}
#     {"type": "see", "items": [names]}
# and an optional "title" on a para or pre block (its section).

PARAM_RE = re.compile(r"^(\S+?)\s{2,}(\S.*)$")
LIST_RE = re.compile(r"^(\d+[.)]|[-*•])\s+(.*)$")


def dedent_lines(lines):
    """Remove the common indentation of the non-blank lines."""
    widths = [indent_of(ln) for ln in lines if ln.strip()]
    cut = min(widths) if widths else 0
    return [ln.expandtabs(4)[cut:] if ln.strip() else "" for ln in lines]


def text_blocks(lines, title=None):
    """Split lines into paragraphs and preformatted blocks.

    Lines at the text's own indentation are joined into paragraphs;
    deeper or column-aligned lines (usage tables, commands) are kept as
    they are.
    """
    blocks = []
    para = []
    pre = []

    def flush():
        """Emit the paragraph or the preformatted lines collected."""
        if para:
            blocks.append({"type": "para", "text": " ".join(para)})
            del para[:]
        if pre:
            blocks.append({"type": "pre", "text": "\n".join(pre).rstrip()})
            del pre[:]

    lines = dedent_lines(lines)
    for ln in lines:
        if not ln.strip():
            flush()
            continue
        aligned = ln.startswith(" ") or re.search(r"\S\s{3,}\S", ln)
        if aligned:
            if para:
                flush()
            pre.append(ln)
        else:
            if pre:
                flush()
            para.append(ln.strip())
    flush()
    if title and blocks:
        blocks[0]["title"] = title
    elif title:
        blocks.append({"type": "para", "text": "", "title": title})
    return blocks


def item_blocks(lines, title):
    """Turn a section's lines into a parameter table or a list."""
    lines = dedent_lines(lines)
    while lines and not lines[-1].strip():
        lines.pop()
    top = [ln for ln in lines if ln.strip() and not ln.startswith(" ")]
    if not top:
        return text_blocks(lines, title)

    # A list: every top-level line starts with a number or a bullet.
    if all(LIST_RE.match(ln) for ln in top):
        items = []
        ordered = bool(re.match(r"^\d", top[0]))
        for ln in lines:
            match = LIST_RE.match(ln) if not ln.startswith(" ") else None
            if match:
                items.append(match.group(2).strip())
            elif ln.strip() and items:
                items[-1] += " " + ln.strip()
        return [{"type": "list", "title": title, "ordered": ordered, "items": items}]

    # A parameter table: "name   description" on each top-level line.
    if sum(1 for ln in top if PARAM_RE.match(ln)) >= max(1, len(top) - 1):
        items = []
        for ln in lines:
            if not ln.strip():
                continue
            match = PARAM_RE.match(ln) if not ln.startswith(" ") else None
            if match:
                items.append(new_item(match.group(1), match.group(2)))
                items[-1]["column"] = match.start(2)
            elif items:
                _add_detail(items[-1], ln)
            else:
                items.append(new_item("", ln.strip()))
        for item in items:
            _finish_item(item)
        return [{"type": "params", "title": title, "items": items}]
    return text_blocks(lines, title)


def new_item(name, text, type_text=""):
    """Return a parameter: its name, type, description and details."""
    return {"name": name, "type": type_text, "text": text, "more": ""}


def _add_detail(item, line):
    """Add a continuation line to a parameter.

    Prose continues the description. Once a line is laid out in columns
    (a nested table), it and the lines after it are kept as they are,
    in the item's "more".
    """
    stripped = line.strip()
    if item.get("rows") or re.search(r"\S\s{2,}\S", stripped):
        item.setdefault("rows", []).append(line.rstrip())
    else:
        item["text"] += " " + stripped


def _finish_item(item):
    """Turn an item's kept rows into its "more" text."""
    rows = item.pop("rows", [])
    item.pop("column", None)
    if rows:
        item["more"] = "\n".join(dedent_lines(rows))


def parse_matlab_help(help_lines, name):
    """Return the structured documentation of a MATLAB help block.

    Args:
        help_lines: The comment lines, without their % (H1 line first).
        name: The function's name (the H1 line starts with it).
    """
    if not help_lines:
        return None
    first = help_lines[0].strip()
    match = re.match(r"^([A-Za-z]\w*)(?:\s+(.*))?$", first)
    rest = help_lines[1:]
    if match and match.group(1).upper() == name.upper():
        summary = (match.group(2) or "").strip()
    else:
        summary = first

    # An H1 line with only the name (or nothing after it): the summary
    # is the first sentence of the text after it.
    if not summary or summary.upper() == name.upper():
        prose = []
        margin = ""
        for k, ln in enumerate(rest):
            if not ln.strip() or re.match(r"^\s*(Inputs?|Outputs?|Arguments?):?\s*$", ln):
                if prose:
                    rest = rest[k:]
                    break
                continue
            margin = margin or ln[:len(ln) - len(ln.lstrip())]
            prose.append(ln.strip())
        else:
            rest = []
        summary = first_sentence(" ".join(prose))
        if prose and summary != " ".join(prose):
            rest = [margin + " ".join(prose)[len(summary):].strip()] + rest

    # Sections: a short line at the text's own indentation with deeper
    # lines under it ("Inputs", "Outputs", "Behavior", ...).
    lines = dedent_lines(rest)
    blocks = []
    section_title = None
    section = []

    def flush():
        """Emit the section collected so far."""
        if section_title is None:
            blocks.extend(text_blocks(section))
        elif re.match(r"^examples?$", section_title, re.I):
            text = "\n".join(dedent_lines(section)).strip("\n")
            blocks.append({"type": "pre", "text": text, "title": section_title})
        else:
            blocks.extend(item_blocks(section, section_title))

    k = 0
    while k < len(lines):
        ln = lines[k]
        see = (re.match(r"^See also:?\s*(.*)$", ln.strip(), re.I)
               if not ln.startswith(" ") else None)
        if see:
            # See also, possibly continued on the next lines.
            text = see.group(1)
            while k + 1 < len(lines) and lines[k + 1].startswith(" ") and lines[k + 1].strip():
                k += 1
                text += " " + lines[k].strip()
            flush()
            section_title, section = None, []
            names = [s.strip(" .") for s in re.split(r"[,\s]+", text) if s.strip(" .")]
            blocks.append({"type": "see", "items": names})
            k += 1
            continue
        nxt = next((x for x in lines[k + 1:] if x.strip()), "")
        header = (ln.strip() and not ln.startswith(" ") and len(ln.strip()) <= 40
                  and re.match(r"^[A-Z][\w ()/-]*:?$", ln.strip())
                  and not ln.strip().endswith(".") and nxt.startswith(" "))
        if header:
            flush()
            section_title, section = ln.strip().rstrip(":"), []
        else:
            section.append(ln)
        k += 1
    flush()
    return {"summary": summary, "blocks": [b for b in blocks if b.get("text") != ""
                                           or b["type"] != "para" or b.get("title")]}


GOOGLE_SECTIONS = {
    "args": "params", "arguments": "params", "parameters": "params", "params": "params",
    "attributes": "params", "raises": "params", "keyword args": "params",
    "returns": "text", "return": "text", "yields": "text", "example": "pre",
    "examples": "pre", "note": "text", "notes": "text", "see also": "text",
    "warning": "text", "warnings": "text", "todo": "text",
}


def parse_docstring(text):
    """Return the structured documentation of a Google docstring."""
    if not text:
        return None
    lines = text.expandtabs(4).split("\n")
    summary_lines = []
    k = 0
    while k < len(lines) and lines[k].strip():
        summary_lines.append(lines[k].strip())
        k += 1
    summary = " ".join(summary_lines)

    blocks = []
    section_title = None
    section_kind = None
    section = []

    def flush():
        """Emit the section collected so far."""
        if section_kind == "params":
            items = []
            for ln in dedent_lines(section):
                if not ln.strip():
                    continue
                match = (re.match(r"^(\*{0,2}[\w.\[\]]+)\s*(?:\(([^)]*)\))?:\s*(.*)$", ln)
                         if not ln.startswith(" ") else None)
                if match:
                    items.append(new_item(match.group(1), match.group(3), match.group(2) or ""))
                elif items:
                    items[-1]["text"] += " " + ln.strip()
            if items:
                blocks.append({"type": "params", "title": section_title, "items": items})
        elif section_kind == "pre":
            blocks.append({"type": "pre", "title": section_title,
                           "text": "\n".join(dedent_lines(section)).strip("\n")})
        else:
            blocks.extend(text_blocks(section, section_title))

    for ln in lines[k:]:
        match = re.match(r"^\s*([A-Za-z][A-Za-z ]*):\s*$", ln)
        if match and match.group(1).lower() in GOOGLE_SECTIONS:
            flush()
            section_title = match.group(1)
            section_kind = GOOGLE_SECTIONS[match.group(1).lower()]
            section = []
        else:
            section.append(ln)
    flush()
    return {"summary": summary, "blocks": [b for b in blocks
                                           if b.get("text") or b["type"] != "para"]}


def parse_comment_doc(lines):
    """Return the structured documentation of plain comment lines."""
    lines = [ln for ln in lines]
    while lines and not lines[0].strip():
        lines.pop(0)
    if not lines:
        return None
    blocks = text_blocks(lines)
    summary = first_sentence(blocks[0]["text"]) if blocks else ""

    # The summary is shown on its own: take it off the first paragraph.
    if blocks and blocks[0]["type"] == "para":
        first = blocks[0]["text"]
        rest = first[len(summary):].strip() if first.startswith(summary) else first
        if rest:
            blocks[0]["text"] = rest
        else:
            blocks.pop(0)
    return {"summary": summary, "blocks": blocks}


def balanced_braces(text, start):
    """Return the index after the } matching the { at start."""
    depth = 0
    for k in range(start, len(text)):
        if text[k] == "{":
            depth += 1
        elif text[k] == "}":
            depth -= 1
            if depth == 0:
                return k + 1
    return len(text)


def parse_jsdoc(raw):
    """Return the structured documentation of a /** ... */ comment."""
    body = raw.strip()
    body = re.sub(r"^/\*\*?", "", body)
    body = re.sub(r"\*/$", "", body)
    lines = [re.sub(r"^\s*\* ?", "", ln) for ln in body.split("\n")]
    text = "\n".join(lines).strip()

    # The description, then the tags (each to the next tag).
    match = re.search(r"(^|\n)\s*@", text)
    description = text[:match.start()] if match else text
    tag_text = text[match.start():] if match else ""
    doc = parse_comment_doc(description.split("\n")) or {"summary": "", "blocks": []}

    params = []
    returns = []
    others = []
    for tag in re.split(r"\n\s*(?=@)", tag_text.strip()):
        tag = tag.strip()
        if not tag:
            continue
        name_match = re.match(r"^@(\w+)\s*", tag)
        if not name_match:
            continue
        tag_name = name_match.group(1)
        rest = tag[name_match.end():]
        type_text = ""
        if rest.startswith("{"):
            close = balanced_braces(rest, 0)
            type_text = " ".join(rest[1:close - 1].split())
            rest = rest[close:].strip()
        rest = " ".join(rest.split())
        if tag_name in ("param", "arg", "argument", "property", "prop"):
            pmatch = re.match(r"^(\[[^\]]*\]|[\w$.]+)\s*-?\s*(.*)$", rest)
            pname, ptext = (pmatch.group(1), pmatch.group(2)) if pmatch else (rest, "")
            params.append(new_item(pname, ptext, type_text))
        elif tag_name in ("return", "returns"):
            returns.append(new_item("", rest, type_text))
        else:
            others.append(new_item("@" + tag_name, rest, type_text))
    if params:
        doc["blocks"].append({"type": "params", "title": "Parameters", "items": params})
    if returns:
        doc["blocks"].append({"type": "params", "title": "Returns", "items": returns})
    if others:
        doc["blocks"].append({"type": "params", "title": "Tags", "items": others})
    if not doc["summary"] and not doc["blocks"]:
        return None
    return doc


# MATLAB

M_BLOCK_KEYWORDS = ("if", "for", "parfor", "while", "switch", "try", "spmd", "function",
                    "classdef")
M_CLASS_BLOCKS = ("properties", "methods", "events", "enumeration")
M_WORD_RE = re.compile(r"(\.\s*)?\b([A-Za-z_]\w*)\b|([(\[{])|([)\]}])")
M_FUNCTION_RE = re.compile(
    r"^function\s+(?:(?:\[[^\]]*\]|[\w.]+)\s*=\s*)?([\w.]+)\s*(\([^)]*\))?")
M_PROPERTY_RE = re.compile(r"^([A-Za-z]\w*)\b\s*(.*)$")


def matlab_continues(line):
    """Return whether a MATLAB line continues (ends in ...)."""
    return matlab_split(line, with_continuation=True)[2]


def matlab_split(line, with_continuation=False):
    """Split a MATLAB line into its code and its comment.

    Strings in the code are replaced by 0, so their contents can't be
    mistaken for keywords or brackets.

    Args:
        line: The line.
        with_continuation: Also return whether it ends in "...".

    Returns:
        (code, comment); comment is None if there is none. With
        with_continuation, (code, comment, continued).
    """
    code = []
    k = 0
    n = len(line)
    while k < n:
        c = line[k]
        if c == "%":
            out = ("".join(code), line[k + 1:])
            return out + (False,) if with_continuation else out
        if c == "." and line.startswith("...", k):
            out = ("".join(code), line[k + 3:])
            return out + (True,) if with_continuation else out
        if c == "'":
            # A transpose follows a name, a closing bracket, a dot or
            # another transpose directly; otherwise it starts a string.
            prev = code[-1] if code else ""
            if prev and (prev.isalnum() or prev in "_)]}.'"):
                code.append(c)
                k += 1
                continue
            k += 1
            while k < n:
                if line[k] == "'" and line.startswith("''", k):
                    k += 2
                elif line[k] == "'":
                    break
                else:
                    k += 1
            code.append("0")
            k += 1
            continue
        if c == '"':
            k += 1
            while k < n:
                if line.startswith('""', k):
                    k += 2
                elif line[k] == '"':
                    break
                else:
                    k += 1
            code.append("0")
            k += 1
            continue
        code.append(c)
        k += 1
    out = ("".join(code), None)
    return out + (False,) if with_continuation else out


def matlab_lines(lines):
    """Return the LineInfo of MATLAB code and each line's code part."""
    info = LineInfo(lines)
    codes = [""]
    in_block = 0
    for number, line in enumerate(lines, 1):
        stripped = line.strip()

        # Block comments: %{ and %} on lines of their own.
        if stripped == "%{":
            in_block += 1
            info.comment(number, "")
            codes.append("")
            continue
        if in_block:
            if stripped == "%}":
                in_block -= 1
                info.comment(number, "")
            else:
                info.comment(number, line.rstrip())
            codes.append("")
            continue

        code, comment = matlab_split(line)
        codes.append(code)
        # Pragmas (%#codegen, %#ok) are code, not explanations.
        if not code.strip() and comment is not None and not comment.startswith("#"):
            if stripped.startswith("%%"):
                info.comment(number, stripped[2:].strip(), "s")
            else:
                info.comment(number, comment_body(comment.rstrip()))
    return info, codes


def parse_matlab(path, text, qual_prefix, lang="matlab", file_lines=None):
    """Find the functions, classes and methods of a MATLAB file.

    Args:
        path: The file's path (for ids).
        text: Its contents.
        qual_prefix: The package prefix ("spot.sim."), or "".
        lang: The language recorded on the entries.
        file_lines: The lines, if already split.

    Returns:
        (LineInfo, the entries, the module entry).
    """
    lines = file_lines if file_lines is not None else text.split("\n")
    info, codes = matlab_lines(lines)
    stem = PurePosixPath(path).stem
    entries = []

    # The file's own entry: a script, a function file or a class.
    first_code = next((n for n in range(1, len(lines) + 1)
                       if info.kind[n] == "k" and codes[n].strip()), None)
    first_word = codes[first_code].split()[0] if first_code else ""
    module = new_entry(path, lang, stem, qual_prefix + stem, "", "file", 1, len(lines))
    module["kind"] = {"function": "file", "classdef": "class"}.get(first_word, "script")
    module["body"] = 1
    with_end = _matlab_functions_have_end(codes)

    # Walk the keywords, keeping a stack of open blocks.
    stack = []          # [kind, entry or None, line]
    class_entry = None
    method_attrs = ""
    open_functions = []
    for number in range(1, len(lines) + 1):
        code = codes[number]
        if not code.strip():
            continue
        depth = 0
        statement_start = True
        for match in M_WORD_RE.finditer(code):
            dot, word, opener, closer = match.groups()
            if opener:
                depth += 1
                statement_start = False
                continue
            if closer:
                depth = max(0, depth - 1)
                continue
            if dot or depth > 0:
                statement_start = False
                continue
            top = stack[-1][0] if stack else None
            if word == "end":
                if stack:
                    kind, entry, _ = stack.pop()
                    if entry is not None:
                        entry["end"] = number
                        if kind == "function":
                            open_functions.remove(entry)
                statement_start = True
                continue
            if word in M_BLOCK_KEYWORDS:
                entry = None
                if word == "function":
                    entry = _matlab_function(path, lang, lines, codes, info, number, stack,
                                             class_entry, method_attrs, qual_prefix, stem)
                    entries.append(entry)
                    open_functions.append(entry)
                    if not with_end:
                        break   # no "end" will close it
                elif word == "classdef":
                    class_entry = module
                    module["line"] = number
                    entry = module
                stack.append([word, entry, number])
                statement_start = False
                if word == "function":
                    break   # the rest of the line is the signature
                continue
            if word in M_CLASS_BLOCKS and top == "classdef":
                rest = code[match.end():].strip()
                if word == "methods":
                    method_attrs = rest.strip("() ")
                stack.append([word, None, number])
                if word == "properties" and class_entry is not None:
                    _matlab_properties(class_entry, lines, codes, number, rest)
                break
            if (word == "arguments" and top == "function" and statement_start
                    and code.strip().split("(")[0].strip() == "arguments"):
                stack.append(["arguments", None, number])
                break
            statement_start = False
        # A comma or semicolon starts a new statement on the same line.

    # Functions without "end" (allowed when none in the file has one):
    # each runs to the line before the next.
    if open_functions:
        starts = sorted(e["line"] for e in entries)
        for entry in open_functions:
            later = [s for s in starts if s > entry["line"]]
            end = (later[0] - 1) if later else len(lines)
            while end > entry["line"] and not lines[end - 1].strip():
                end -= 1
            entry["end"] = end

    # The file's documentation: the class's or script's help, or the
    # main function's.
    if module["kind"] == "class":
        class_sig, class_end = _matlab_signature(lines, codes, module["line"])
        help_lines, body = _matlab_help(lines, codes, info, class_end)
        module["doc"] = parse_matlab_help(help_lines, stem)
        module["help"] = "\n".join(help_lines)
        module["body"] = body
        module["sig"] = signature_text(class_sig)
    elif module["kind"] == "script":
        help_lines, body = _matlab_help(lines, codes, info, 0)
        module["doc"] = parse_matlab_help(help_lines, stem)
        module["help"] = "\n".join(help_lines)
        module["body"] = body
    elif entries:
        main = entries[0]
        module["doc"] = main["doc"]
        module["help"] = main.get("help", "")

    # Parents: the innermost function or class around each function.
    for entry in entries:
        if entry["parent"] is None:
            entry["parent"] = module["id"]
    _matlab_refs(entries + [module], lines, codes)
    return info, entries, module


def _matlab_functions_have_end(codes):
    """Return whether a file's functions end with "end".

    MATLAB allows either style, but not both in one file: count the
    ends against the blocks other than functions.
    """
    opens = 0
    ends = 0
    functions = 0
    for code in codes:
        depth = 0
        for match in M_WORD_RE.finditer(code):
            dot, word, opener, closer = match.groups()
            if opener:
                depth += 1
            elif closer:
                depth = max(0, depth - 1)
            elif dot or depth > 0:
                continue
            elif word == "end":
                ends += 1
            elif word == "function":
                functions += 1
                break
            elif word in M_BLOCK_KEYWORDS or word in M_CLASS_BLOCKS:
                opens += 1
    return functions == 0 or ends > opens


def _matlab_signature(lines, codes, number):
    """Return a function's signature and the line it ends on."""
    text = codes[number].strip()
    end = number
    while end < len(lines) and matlab_continues(lines[end - 1]):
        end += 1
        text += " " + codes[end].strip()
    return text, end


def _matlab_help(lines, codes, info, after):
    """Return the help after line `after`, and the body's start."""
    help_lines = []
    n = after + 1
    while n <= len(lines) and info.kind[n] == "c" and not codes[n].strip():
        raw = lines[n - 1]
        k = raw.find("%")
        help_lines.append(raw[k + 1:].rstrip() if k >= 0 else "")
        n += 1
    body = n
    while body <= len(lines) and info.kind[body] == "b":
        body += 1
    return help_lines, body


def _matlab_function(path, lang, lines, codes, info, number, stack, class_entry, attrs,
                     qual_prefix, stem):
    """Make the entry of the function declared on line `number`."""
    sig, sig_end = _matlab_signature(lines, codes, number)
    match = M_FUNCTION_RE.match(sig)
    name = match.group(1) if match else "?"

    # Where it sits: in a class, inside another function, or in the
    # file (the first is the main function, later ones are local).
    outer = [s for s in stack if s[0] == "function"]
    in_class = any(s[0] == "methods" for s in stack)
    if in_class and class_entry is not None:
        local = name
        qual = "%s%s.%s" % (qual_prefix, stem, name)
        kind = "method"
    elif outer:
        parent = outer[-1][1]
        local = "%s/%s" % (parent["local"], name)
        qual = "%s/%s" % (parent["qual"], name)
        kind = "nested"
    elif class_entry is not None or any(s[0] == "classdef" for s in stack):
        local = name
        qual = "%s%s>%s" % (qual_prefix, stem, name)
        kind = "local"
    else:
        first = not any(True for s in stack)
        is_main = first and number == next(
            (n for n in range(1, len(lines) + 1) if info.kind[n] == "k"), number)
        local = name
        if is_main:
            qual = qual_prefix + name
            kind = "function"
        else:
            qual = "%s%s>%s" % (qual_prefix, stem, name)
            kind = "local"
    entry = new_entry(path, lang, name, qual, local, kind, number, number)
    entry["sig"] = signature_text(sig)
    if kind == "method" and attrs:
        entry["attrs"] = attrs
    if outer:
        entry["parent"] = outer[-1][1]["id"]
    elif in_class and class_entry is not None:
        entry["parent"] = class_entry["id"]

    # Its help: the comments right after the signature.
    help_lines, body = _matlab_help(lines, codes, info, sig_end)
    entry["doc"] = parse_matlab_help(help_lines, name)
    entry["help"] = "\n".join(help_lines)
    entry["body"] = body
    return entry


def _matlab_properties(class_entry, lines, codes, number, attrs):
    """Read a properties block of a class into its documentation."""
    items = []
    n = number + 1
    while n <= len(lines):
        code = codes[n].strip()
        if code == "end" or code.startswith("end ") or code.startswith("end%"):
            break
        match = M_PROPERTY_RE.match(code) if code else None
        if match:
            raw = lines[n - 1]
            _, comment = matlab_split(raw)
            default = match.group(2).strip().lstrip("=").strip().rstrip(";")
            item = new_item(match.group(1), (comment or "").strip())
            if default:
                code_part = raw.split("%")[0]
                if "=" in code_part:
                    item["type"] = "= " + code_part.split("=", 1)[1].strip().rstrip(";")
                else:
                    item["type"] = default
            items.append(item)
        n += 1
    if not items:
        return
    doc = class_entry["doc"] or {"summary": "", "blocks": []}
    title = "Properties" + (" (%s)" % attrs.strip("() ") if attrs.strip("() ") else "")
    doc["blocks"].append({"type": "params", "title": title, "items": items})
    class_entry["doc"] = doc
    class_entry.setdefault("props_pending", []).append({"title": title, "items": items})


def _matlab_refs(entries, lines, codes):
    """Record the names each entry's code uses (for cross references).

    Also records the labels of its case statements (the MATLAB worker's
    commands are its dispatch method's cases).
    """
    for entry in entries:
        inner = [(c["line"], c["end"]) for c in entries
                 if c is not entry and c["line"] >= entry["line"] and c["end"] <= entry["end"]
                 and c["kind"] not in ("file", "class", "script")]
        for n in range(entry["body"], entry["end"] + 1):
            if any(a <= n <= b for a, b in inner):
                continue
            code = codes[n] if n < len(codes) else ""
            for match in re.finditer(r"(?<![\w.])((?:spot|interface)(?:\.\w+)+|[A-Za-z]\w*)",
                                     code):
                entry["refs"].append(("name", match.group(1), n))
            for match in re.finditer(r"\.\s*([A-Za-z]\w*)\s*\(", code):
                entry["refs"].append(("attr", "", match.group(1), n))
            if re.match(r"^\s*case\b", code):
                for label in re.findall(r"'([^']*)'", lines[n - 1].split("%")[0]):
                    entry["strings"].append((label, n))


# Python

PY_TOKEN_SKIP = (tokenize.NL, tokenize.NEWLINE, tokenize.INDENT, tokenize.DEDENT,
                 tokenize.ENDMARKER, tokenize.COMMENT)
PY_TOKEN_ENCODING = getattr(tokenize, "ENCODING", -1)


def python_tokens(text):
    """Return the tokens of Python code, or [] if it can't be read."""
    try:
        return list(tokenize.generate_tokens(io.StringIO(text).readline))
    except (tokenize.TokenError, IndentationError, SyntaxError):
        return []


def python_lines(lines, tokens):
    """Return the LineInfo of Python code."""
    info = LineInfo(lines)
    code_rows = set()
    for tok in tokens:
        if tok.type == tokenize.COMMENT:
            if tok.start[0] not in code_rows:
                info.comment(tok.start[0], comment_body(tok.string[1:].rstrip()))
        elif tok.type not in PY_TOKEN_SKIP and tok.type != PY_TOKEN_ENCODING:
            for row in range(tok.start[0], tok.end[0] + 1):
                code_rows.add(row)
                if info.kind[row] == "c":
                    info.kind[row] = "k"
    for row in code_rows:
        if row < len(info.kind):
            info.kind[row] = "k"
    return info


def python_logical_ends(tokens):
    """Return {row: the last row of the logical line containing it}."""
    ends = {}
    start = None
    for tok in tokens:
        if tok.type in (tokenize.NL, tokenize.COMMENT, PY_TOKEN_ENCODING, tokenize.INDENT,
                        tokenize.DEDENT):
            continue
        if start is None:
            start = tok.start[0]
        if tok.type in (tokenize.NEWLINE, tokenize.ENDMARKER):
            for row in range(start, tok.end[0] + 1):
                ends[row] = tok.start[0]
            start = None
    return ends


def python_string_rows(tokens):
    """Return {a string token's rows: (its first, its last row)}."""
    rows = {}
    for tok in tokens:
        if tok.type == tokenize.STRING:
            for row in range(tok.start[0], tok.end[0] + 1):
                rows[row] = (tok.start[0], tok.end[0])
    return rows


def _py_def_header_end(tokens, row, rows):
    """Return the row of the colon that ends the def or class on `row`.

    Args:
        tokens: The file's tokens.
        row: The line of the def or class.
        rows: Each token's first row, in order (to find `row` fast).
    """
    depth = 0
    seen = False
    for tok in tokens[bisect.bisect_left(rows, row):]:
        if tok.type == tokenize.NAME and tok.string in ("def", "class") and not seen:
            seen = True
            continue
        if not seen:
            continue
        if tok.type == tokenize.OP and tok.string in "([{":
            depth += 1
        elif tok.type == tokenize.OP and tok.string in ")]}":
            depth -= 1
        elif tok.type == tokenize.OP and tok.string == ":" and depth == 0:
            return tok.start[0]
    return row


def py_string(node):
    """Return the text of a string constant node, or None.

    Python 3.8 and newer use ast.Constant; 3.6 and 3.7 use ast.Str
    (gone in 3.12).
    """
    if isinstance(node, getattr(ast, "Constant", ())):
        return node.value if isinstance(node.value, str) else None
    if isinstance(node, getattr(ast, "Str", ())):
        return node.s
    return None


def _py_is_docstring(node):
    """Return whether a statement is a docstring (a lone string)."""
    return isinstance(node, ast.Expr) and py_string(node.value) is not None


def parse_python(path, text, module_qual, lines=None):
    """Find the classes, functions and methods of a Python file.

    Returns:
        (LineInfo, the entries, the module entry, an error message or
        None).
    """
    lines = lines if lines is not None else text.split("\n")
    tokens = python_tokens(text)
    info = python_lines(lines, tokens)
    stem = PurePosixPath(path).stem
    module = new_entry(path, "python", stem, module_qual, "", "module", 1, len(lines))
    try:
        tree = ast.parse(text)
    except (SyntaxError, ValueError) as e:
        module["error"] = "Python could not read this file: %s" % e
        return info, [], module, module["error"]

    logical_end = python_logical_ends(tokens)
    string_rows = python_string_rows(tokens)
    token_rows = [tok.start[0] for tok in tokens]
    entries = []

    def last_row(node):
        """Return a statement's last line (3.6 has no end_lineno)."""
        end = getattr(node, "end_lineno", None)
        if end:
            return end
        rows = [getattr(n, "lineno", 0) for n in ast.walk(node)]
        row = max(rows) if rows else node.lineno
        row = string_rows.get(row, (row, row))[1]
        return logical_end.get(row, row)

    def doc_range(node, header_end):
        """Return the first and last line of a def's docstring."""
        if not node.body or not _py_is_docstring(node.body[0]):
            return None
        row = header_end + 1
        while row <= len(lines) and row not in string_rows:
            row += 1
        return string_rows.get(row)

    def visit(node, parent, prefix, kind_inside):
        """Make entries for the definitions directly inside `node`."""
        for child in _py_direct_defs(node):
            is_class = isinstance(child, ast.ClassDef)
            name = child.name
            local = prefix + name
            if is_class:
                kind = "class"
            elif kind_inside == "class":
                kind = "method"
            elif kind_inside == "function":
                kind = "nested"
            else:
                kind = "function"
            start = min([child.lineno] + [d.lineno for d in child.decorator_list])
            entry = new_entry(path, "python", name, "%s.%s" % (module_qual, local), local,
                              kind, child.lineno, last_row(child))
            entry["start"] = start
            entry["parent"] = parent["id"]
            header_end = _py_def_header_end(tokens, child.lineno, token_rows)
            entry["sig"] = signature_text("\n".join(
                ln.strip() for ln in lines[child.lineno - 1:header_end]).rstrip(":"))
            docstring = ast.get_docstring(child)
            entry["doc"] = parse_docstring(docstring)
            entry["help"] = docstring or ""
            span = doc_range(child, header_end)
            entry["body"] = (span[1] if span else header_end) + 1
            entry["refs"] = _py_refs(child)
            entry["strings"] = _py_strings(child)
            entries.append(entry)
            visit(child, entry, local + ".", "class" if is_class else "function")

    module_doc = ast.get_docstring(tree)
    module["doc"] = parse_docstring(module_doc)
    module["help"] = module_doc or ""
    if tree.body and _py_is_docstring(tree.body[0]):
        span = string_rows.get(next((r for r in range(1, len(lines) + 1) if r in string_rows),
                                    0))
        module["body"] = (span[1] + 1) if span else 1
    else:
        module["body"] = 1
    module["refs"] = _py_refs(tree, top_level=True)
    module["strings"] = _py_strings(tree, top_level=True)
    module["imports"] = _py_imports(tree)
    visit(tree, module, "", "module")
    return info, entries, module, None


def _py_direct_defs(node):
    """Return the defs and classes directly inside a node."""
    found = []

    def walk(n):
        """Collect definitions, not entering them."""
        for child in ast.iter_child_nodes(n):
            if isinstance(child, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
                found.append(child)
            elif not isinstance(child, ast.Lambda):
                walk(child)

    walk(node)
    return sorted(found, key=lambda d: d.lineno)


def _py_own_nodes(node, top_level=False):
    """Yield the nodes of a definition's own code (not nested ones)."""
    stack = list(ast.iter_child_nodes(node))
    while stack:
        n = stack.pop()
        if isinstance(n, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
            # A nested definition's decorators and defaults belong here.
            stack.extend(n.decorator_list)
            if not isinstance(n, ast.ClassDef):
                stack.extend(n.args.defaults)
            continue
        yield n
        stack.extend(ast.iter_child_nodes(n))


def _py_refs(node, top_level=False):
    """Return the names a definition's code uses.

    Returns:
        [("name", name, line)] and [("attr", base, attr, line)].
    """
    refs = []
    for n in _py_own_nodes(node, top_level):
        if isinstance(n, ast.Name) and isinstance(n.ctx, ast.Load):
            refs.append(("name", n.id, n.lineno))
        elif isinstance(n, ast.Attribute) and isinstance(n.ctx, ast.Load):
            base = n.value.id if isinstance(n.value, ast.Name) else ""
            refs.append(("attr", base, n.attr, n.lineno))
    return refs


def _py_strings(node, top_level=False):
    """Return a definition's string constants: [(text, line)]."""
    out = []
    for n in _py_own_nodes(node, top_level):
        value = py_string(n)
        if value is not None and len(value) < 80:
            out.append((value, getattr(n, "lineno", 0)))
    return out


def _py_imports(tree):
    """Return {alias: (module, name or None, level)} for the imports."""
    out = {}
    for n in ast.walk(tree):
        if isinstance(n, ast.ImportFrom):
            for alias in n.names:
                out[alias.asname or alias.name] = (n.module or "", alias.name, n.level)
        elif isinstance(n, ast.Import):
            for alias in n.names:
                out[alias.asname or alias.name.split(".")[0]] = (alias.name, None, 0)
    return out


# JavaScript

JS_TOKEN_RE = re.compile(r"""
    (?P<ws>[ \t\r\f\v\u00a0\ufeff]+)
  | (?P<nl>\n)
  | (?P<lc>//[^\n]*)
  | (?P<bc>/\*.*?\*/)
  | (?P<str>'(?:\\.|[^'\\\n])*'|"(?:\\.|[^"\\\n])*")
  | (?P<tpl>`)
  | (?P<id>[A-Za-z_$\u00c0-\uffff][\w$\u00c0-\uffff]*)
  | (?P<num>\d[\w.]*|\.\d\w*)
  | (?P<punct>=>|\.\.\.|\?\.|\?\?=?|===|!==|\*\*=?|<<=?|>>>=?|>>=?|&&=?|\|\|=?|\+\+|--
              |[-+*/%&|^<>=!]=?|[{}()\[\];,.:?~@\#])
  | (?P<other>.)
""", re.S | re.X)

JS_REGEX_AFTER_WORDS = ("return", "typeof", "instanceof", "in", "of", "new", "delete", "void",
                        "throw", "case", "do", "else", "yield", "await")
JS_CONTROL = ("if", "for", "while", "switch", "catch", "with", "function", "return", "typeof")
JS_OBJECT_AFTER = ("=", "(", ",", ":", "[", "?", "||", "&&", "??", "return", "...", "!",
                   "=>", "+", "in", "of", "yield", "await", "default", "case")


def js_template_end(src, k):
    """Return the index after the template literal that starts at k."""
    n = len(src)
    k += 1
    while k < n:
        c = src[k]
        if c == "\\":
            k += 2
        elif c == "`":
            return k + 1
        elif c == "$" and src.startswith("${", k):
            k = js_expression_end(src, k + 2)
        else:
            k += 1
    return n


def js_expression_end(src, k):
    """Return the index after the } that closes a ${...} at depth 0."""
    n = len(src)
    depth = 0
    prev = ""   # the last character that isn't a space
    while k < n:
        c = src[k]
        # A regular expression where a value is expected (its // or
        # quotes are not comments or strings).
        value_expected = not prev or prev in "(,=:[!&|?{};+-*%<>~^"
        if c == "/" and not src.startswith("/*", k) and value_expected:
            end = js_regex_end(src, k)
            if end is not None:
                k = end
                prev = "x"
                continue
        if not c.isspace():
            prev = c
        if c in "'\"":
            k += 1
            while k < n and src[k] != c:
                k += 2 if src[k] == "\\" else 1
            k += 1
        elif c == "`":
            k = js_template_end(src, k)
        elif src.startswith("//", k):
            nl = src.find("\n", k)
            k = n if nl < 0 else nl
        elif src.startswith("/*", k):
            close = src.find("*/", k + 2)
            k = n if close < 0 else close + 2
        elif c == "{":
            depth += 1
            k += 1
        elif c == "}":
            if depth == 0:
                return k + 1
            depth -= 1
            k += 1
        else:
            k += 1
    return n


def js_regex_end(src, k):
    """Return the index after a regular expression at k, or None."""
    n = len(src)
    j = k + 1
    in_class = False
    while j < n:
        c = src[j]
        if c == "\\":
            j += 2
            continue
        if c == "\n":
            return None
        if c == "[":
            in_class = True
        elif c == "]":
            in_class = False
        elif c == "/" and not in_class:
            j += 1
            while j < n and (src[j].isalnum() or src[j] == "_"):
                j += 1
            return j
        j += 1
    return None


def js_tokens(src):
    """Split JavaScript into tokens and comments.

    Returns:
        (tokens, comments): tokens are [type, value, line, end line,
        column, position] with type id, str, tpl, num, punct, regex
        or other; comments are [line, end line, text, block?].
    """
    tokens = []
    comments = []
    line = 1
    line_start = 0
    k = 0
    n = len(src)
    prev = None
    while k < n:
        # A slash starts a regular expression where a value is expected.
        if src[k] == "/" and not src.startswith("//", k) and not src.startswith("/*", k):
            regex_ok = (prev is None
                        or (prev[0] == "punct" and prev[1] not in (")", "]", "}"))
                        or (prev[0] == "id" and prev[1] in JS_REGEX_AFTER_WORDS))
            if regex_ok:
                end = js_regex_end(src, k)
                if end is not None:
                    tok = ["regex", src[k:end], line, line, k - line_start, k]
                    tokens.append(tok)
                    prev = tok
                    k = end
                    continue
        match = JS_TOKEN_RE.match(src, k)
        kind = match.lastgroup
        value = match.group()
        if kind == "tpl":
            end = js_template_end(src, k)
            value = src[k:end]
        else:
            end = match.end()
        newlines = value.count("\n")
        if kind == "nl":
            line += 1
            line_start = end
        elif kind in ("lc", "bc"):
            comments.append([line, line + newlines, value, kind == "bc"])
        elif kind != "ws":
            tok = [kind, value, line, line + newlines, k - line_start, k]
            tokens.append(tok)
            prev = tok
        if newlines and kind != "nl":
            line += newlines
            line_start = k + value.rfind("\n") + 1
        k = end
    return tokens, comments


def js_lines(lines, tokens, comments):
    """Return the LineInfo of JavaScript or C-like code."""
    info = LineInfo(lines)
    code_rows = set()
    for tok in tokens:
        code_rows.update(range(tok[2], tok[3] + 1))
    for first, last, text, block in comments:
        if block:
            # Each line without its leading * (and /** on the first).
            for k, part in enumerate(text[2:-2].split("\n")):
                row = first + k
                if row not in code_rows:
                    clean = part.lstrip("*") if k == 0 else re.sub(r"^\s*\*?", "", part)
                    info.comment(row, comment_body(clean.rstrip()))
        elif first not in code_rows:
            info.comment(first, comment_body(text[2:].rstrip()))
    return info


class JsParser:
    """Finds the functions, methods and classes in JavaScript tokens."""

    def __init__(self, path, src, lines, tokens, comments):
        """Keep the file and prepare the structural pass."""
        self.path = path
        self.src = src
        self.lines = lines
        self.toks = tokens
        self.comments = comments
        self.entries = []
        self.module = None
        self.body_open = {}       # token index of a body's { -> (kind, entry)
        self.match = self._matching()
        self.closers = {end: start for start, end in self.match.items()}

    def _matching(self):
        """Return {index of an opening bracket: index of its closer}."""
        pairs = {}
        stack = []
        for k, tok in enumerate(self.toks):
            if tok[0] != "punct":
                continue
            if tok[1] in "([{":
                stack.append(k)
            elif tok[1] in ")]}" and stack:
                pairs[stack.pop()] = k
        return pairs

    def value(self, k):
        """Return the value of token k, or "" outside the file."""
        return self.toks[k][1] if 0 <= k < len(self.toks) else ""

    def kind(self, k):
        """Return the type of token k, or "" outside the file."""
        return self.toks[k][0] if 0 <= k < len(self.toks) else ""

    def opener_of(self, closer):
        """Return the index of the bracket that `closer` closes."""
        return self.closers.get(closer, closer)

    def parse(self):
        """Find every entry; return (entries, module entry)."""
        stem = PurePosixPath(self.path).name
        self.module = new_entry(self.path, "javascript", stem, stem, "", "module", 1,
                                len(self.lines))
        self.module["body"] = 1
        frames = [{"kind": "top", "entry": self.module, "obj": [], "pdepth": 0}]
        k = 0
        count = len(self.toks)
        while k < count:
            typ, val = self.toks[k][0], self.toks[k][1]
            top = frames[-1]

            if typ == "id" and val == "function" and self.value(k - 1) != ".":
                self._function_keyword(k, frames)
            elif typ == "punct" and val == "=>":
                self._arrow(k, frames)
            elif (typ == "id" and val == "class" and self.value(k - 1) != "."
                    and self.kind(k + 1) in ("id", "punct")):
                self._class(k, frames)
            elif (top["kind"] in ("class", "object") and top["pdepth"] == 0
                    and typ in ("id", "str", "num") and self.value(k + 1) == "("):
                self._method(k, frames)

            if typ == "punct":
                if val == "{":
                    frames.append(self._open_frame(k, frames))
                elif val == "}":
                    if len(frames) > 1:
                        frame = frames.pop()
                        entry = frame.get("owns")
                        if entry is not None:
                            entry["end"] = self.toks[k][3]
                            entry["tok_end"] = k
                elif val in "([":
                    top["pdepth"] += 1
                elif val in ")]":
                    top["pdepth"] = max(0, top["pdepth"] - 1)
            k += 1
        self._documentation()
        self._refs()
        return self.entries, self.module

    def _open_frame(self, k, frames):
        """Return the frame a { opens: a body, an object or a block."""
        top = frames[-1]
        if k in self.body_open:
            kind, entry = self.body_open[k]
            return {"kind": kind, "entry": entry if entry is not None else top["entry"],
                    "owns": entry, "obj": [], "pdepth": 0}
        prev = self.value(k - 1)
        obj = list(top["obj"])
        if prev == ":" and top["kind"] != "object" and top["pdepth"] == 0:
            kind = "block"     # a case or a label
        elif prev in JS_OBJECT_AFTER:
            kind = "object"
            if prev == "=" and self.kind(k - 2) == "id":
                obj.append(self.value(k - 2))
            elif prev == ":" and self.kind(k - 2) in ("id", "str"):
                obj.append(self.value(k - 2).strip("'\""))
        else:
            kind = "block"
        return {"kind": kind, "entry": top["entry"], "owns": None, "obj": obj, "pdepth": 0}

    def _statement_start(self, k):
        """Return the first token of what is assigned at k (=)."""
        j = k - 1
        while j >= 0:
            val = self.value(j)
            typ = self.kind(j)
            if val in (")", "]"):
                j = self.opener_of(j) - 1
                continue
            if typ == "id" or val in (".", "?."):
                j -= 1
                continue
            break
        return j + 1

    def _name_from_context(self, k, frames):
        """Name a function expression at token k from what precedes it.

        Returns:
            (name, kind, statement start token) or (None, None, k).
        """
        prev = self.value(k - 1)
        top = frames[-1]
        if prev == "=" and self.kind(k - 2) == "id" and self.value(k - 3) != ".":
            start = k - 2
            if self.value(k - 3) in ("const", "let", "var"):
                start = k - 3
                if self.value(k - 4) == "export":
                    start = k - 4
            return self.value(k - 2), "function", start
        if prev == "=" and self.value(k - 2) not in ("=", "!", "<", ">"):
            start = self._statement_start(k - 1)
            name = " ".join(t[1] for t in self.toks[start:k - 1])
            name = re.sub(r"\s*([.(),\[\]])\s*", r"\1", name)
            if start < k - 1 and len(name) <= TITLE_CHARS:
                return name, "handler", start
        if (prev == ":" and top["kind"] == "object" and top["pdepth"] == 0
                and self.kind(k - 2) in ("id", "str")):
            return self.value(k - 2).strip("'\""), "method", k - 2
        if prev == "default" and self.value(k - 2) == "export":
            return "default", "function", k - 2
        return None, None, k

    def _add(self, name, kind, start_tok, decl_tok, frames, body_tok=None, end_tok=None):
        """Make an entry for a function found in the tokens."""
        parent = frames[-1]["entry"]
        objs = frames[-1]["obj"]
        local_parts = ([parent["local"]] if parent["local"] else []) + objs + [name]
        local = ".".join(local_parts)
        if frames[-1]["kind"] == "class" and kind != "class":
            kind = "method"
        elif kind in ("function", "class") and parent is not self.module:
            kind = "nested" if kind == "function" else kind
        if kind == "method" and not objs and frames[-1]["kind"] != "class":
            kind = "nested"
        entry = new_entry(self.path, "javascript", name, local, local, kind,
                          self.toks[decl_tok][2], self.toks[decl_tok][3])
        entry["parent"] = parent["id"]
        entry["tok_start"] = start_tok
        entry["col"] = self.toks[start_tok][4]
        entry["line"] = self.toks[start_tok][2]
        sig_end = body_tok if body_tok is not None else (end_tok or decl_tok)
        entry["sig"] = signature_text(self.src[self.toks[start_tok][5]:self.toks[sig_end][5]]
                                      .rstrip("{ \n"))
        if body_tok is not None:
            entry["body"] = self.toks[body_tok][2] + 1
            self.body_open[body_tok] = ("class" if kind == "class" else "func", entry)
        if end_tok is not None:
            entry["end"] = self.toks[end_tok][3]
            entry["tok_end"] = end_tok
            entry["body"] = self.toks[start_tok][2]
        self.entries.append(entry)
        return entry

    def _function_keyword(self, k, frames):
        """Handle `function`: a declaration or a function expression."""
        j = k + 1
        if self.value(j) == "*":
            j += 1
        own_name = self.value(j) if self.kind(j) == "id" else None
        if own_name:
            j += 1
        if self.value(j) != "(" or j not in self.match:
            return
        body = self.match[j] + 1
        if self.value(body) != "{":
            return
        first = k - 1 if self.value(k - 1) == "async" else k
        if own_name and self.value(first - 1) not in ("=", ":", "(", ",", "return"):
            start = first - 1 if self.value(first - 1) == "export" else first
            if self.value(start - 1) == "export":
                start -= 1
            self._add(own_name, "function", start, k, frames, body_tok=body)
            return
        name, kind, start = self._name_from_context(first, frames)
        if name:
            self._add(name, kind, start, k, frames, body_tok=body)
        else:
            self.body_open[body] = ("func", None)

    def _arrow(self, k, frames):
        """Handle =>: name the arrow function from what it is set to."""
        prev = k - 1
        if self.value(prev) == ")":
            first = self.opener_of(prev)
        else:
            first = prev
        if self.value(first - 1) == "async":
            first -= 1
        name, kind, start = self._name_from_context(first, frames)
        body = k + 1
        if self.value(body) == "{":
            if name:
                self._add(name, kind, start, first, frames, body_tok=body)
            else:
                self.body_open[body] = ("func", None)
            return
        if not name or kind == "handler":
            return
        # An expression body: to the comma, semicolon or bracket that
        # ends it.
        j = body
        while j < len(self.toks):
            val = self.value(j)
            if val in ("(", "[", "{") and j in self.match:
                j = self.match[j] + 1
                continue
            if val in (",", ";", ")", "]", "}"):
                break
            j += 1
        self._add(name, kind, start, first, frames, end_tok=max(body, j - 1))

    def _class(self, k, frames):
        """Handle `class Name {`: the class and its body."""
        j = k + 1
        name = self.value(j) if self.kind(j) == "id" and self.value(j) != "extends" else None
        while j < len(self.toks) and self.value(j) != "{":
            j += 1
        if j >= len(self.toks):
            return
        start = k - 1 if self.value(k - 1) == "export" else k
        if self.value(start - 1) == "export":
            start -= 1
        if not name:
            name, _, start = self._name_from_context(k, frames)
        if name:
            self._add(name, "class", start, k, frames, body_tok=j)
        else:
            self.body_open[j] = ("class", None)

    def _method(self, k, frames):
        """Handle `name(...) {` in a class body or an object literal."""
        prev = self.value(k - 1)
        start = k
        while self.value(start - 1) in ("static", "async", "get", "set", "*"):
            start -= 1
        if (self.value(start - 1) not in ("{", ",", ";", "}") and start == k
                and prev not in ("{", ",", ";", "}")):
            return
        paren = k + 1
        if paren not in self.match:
            return
        body = self.match[paren] + 1
        if self.value(body) != "{":
            return
        if self.value(k) in JS_CONTROL:
            return
        self._add(self.value(k).strip("'\""), "method", start, k, frames, body_tok=body)

    def _documentation(self):
        """Attach the JSDoc or line comments above each entry."""
        by_end = {}
        for comment in self.comments:
            by_end.setdefault(comment[1], []).append(comment)
        for entry in self.entries:
            line = entry["line"]
            above = by_end.get(line - 1, [])
            jsdoc = [c for c in above if c[3] and c[2].startswith("/**")]
            if jsdoc:
                entry["doc"] = parse_jsdoc(jsdoc[-1][2])
                entry["help"] = jsdoc[-1][2]
                entry["start"] = jsdoc[-1][0]
                continue
            # Line comments directly above (a run of whole-line ones).
            run = []
            row = line - 1
            while row >= 1:
                found = [c for c in by_end.get(row, []) if not c[3] and c[0] == row
                         and self.lines[row - 1].strip().startswith("//")]
                if not found:
                    break
                run.insert(0, comment_body(found[0][2][2:].rstrip()))
                row -= 1
            if run:
                entry["doc"] = parse_comment_doc(run)
                entry["help"] = "\n".join(run)
                entry["start"] = row + 1

        # The module: the comment at the top of the file.
        top = [c for c in self.comments if c[0] <= 3]
        lines = []
        if top and top[0][3]:
            lines = [re.sub(r"^\s*\*\s?", "", ln) for ln in top[0][2][2:-2].split("\n")]
        else:
            row = 1
            for comment in self.comments:
                if comment[0] != row or comment[3]:
                    break
                lines.append(comment_body(comment[2][2:].rstrip()))
                row += 1
        if lines:
            self.module["doc"] = parse_comment_doc(lines)
            self.module["help"] = "\n".join(lines)
            self.module["body"] = (top[0][1] if top and top[0][3] else len(lines)) + 1

    def _refs(self):
        """Record the names and strings each entry's code uses."""
        spans = sorted((e for e in self.entries if "tok_end" in e),
                       key=lambda e: (e["tok_start"], -e["tok_end"]))
        stack = [self.module]
        order = list(spans)
        pos = 0
        for k, tok in enumerate(self.toks):
            while len(stack) > 1 and stack[-1]["tok_end"] < k:
                stack.pop()
            while pos < len(order) and order[pos]["tok_start"] <= k:
                stack.append(order[pos])
                pos += 1
                while len(stack) > 1 and stack[-1]["tok_end"] < k:
                    stack.pop()
            owner = stack[-1]
            typ, val = tok[0], tok[1]
            if typ == "id":
                if self.value(k - 1) in (".", "?."):
                    base = self.value(k - 2) if self.kind(k - 2) == "id" else ""
                    owner["refs"].append(("attr", base, val, tok[2]))
                else:
                    owner["refs"].append(("name", val, tok[2]))
            elif typ == "str":
                owner["strings"].append((val[1:-1], tok[2]))
            elif typ == "tpl":
                owner["strings"].append((val[1:].split("${")[0].rstrip("`"), tok[2]))

        # The imports: {local name: (module file, exported name)}.
        imports = {}
        for k, tok in enumerate(self.toks):
            if tok[1] != "import" or tok[0] != "id":
                continue
            j = k + 1
            names = []
            if self.value(j) == "{":
                end = self.match.get(j, j)
                parts = [t[1] for t in self.toks[j + 1:end]]
                text = " ".join(parts).replace(" , ", ",")
                for item in text.split(","):
                    bits = item.split()
                    if bits:
                        names.append((bits[0], bits[-1]))
                j = end + 1
            if self.value(j) == "from" and self.kind(j + 1) == "str":
                source = self.value(j + 1)[1:-1]
                for exported, local in names:
                    imports[local] = (source, exported)
        self.module["imports"] = imports


def parse_javascript(path, text, lines=None):
    """Find the functions of a JavaScript file.

    Returns:
        (LineInfo, the entries, the module entry).
    """
    lines = lines if lines is not None else text.split("\n")
    tokens, comments = js_tokens(text)
    info = js_lines(lines, tokens, comments)
    parser = JsParser(path, text, lines, tokens, comments)
    entries, module = parser.parse()
    for entry in entries:
        entry.pop("tok_start", None)
        entry.pop("tok_end", None)
    return info, entries, module


# C and C++

C_KEYWORDS = ("if", "for", "while", "switch", "catch", "return", "sizeof", "do", "else",
              "defined", "alignof", "decltype", "static_assert", "__attribute__")
C_MASK_RE = re.compile(r"""
    (?P<lc>//[^\n]*)
  | (?P<bc>/\*.*?\*/)
  | (?P<str>"(?:\\.|[^"\\\n])*"|'(?:\\.|[^'\\\n])*')
  | (?P<pp>^[ \t]*\#(?:[^\n\\]|\\.)*)
""", re.S | re.X | re.M)


def c_mask(text):
    """Return C code with comments, strings and directives blanked.

    Returns:
        (masked text, comments as [line, end line, text, block?],
        set of preprocessor lines).
    """
    out = []
    comments = []
    directives = set()
    pos = 0
    for match in C_MASK_RE.finditer(text):
        out.append(text[pos:match.start()])
        value = match.group()
        line = text.count("\n", 0, match.start()) + 1
        if match.lastgroup in ("lc", "bc"):
            comments.append([line, line + value.count("\n"), value, match.lastgroup == "bc"])
        elif match.lastgroup == "pp":
            directives.update(range(line, line + value.count("\n") + 1))
        out.append(re.sub(r"[^\n]", " ", value))
        pos = match.end()
    out.append(text[pos:])
    masked = _first_branches("".join(out), text, directives)
    return masked, comments, directives


def _first_branches(masked, text, directives):
    """Blank the #else and #elif branches of conditional code.

    Each branch may open a brace the other closes (an #ifdef _WIN32
    with an #else for Linux): keep only the first, as one build sees.
    """
    lines = masked.split("\n")
    raw = text.split("\n")
    depth = 0            # #if nesting
    skip_from = None     # the depth whose later branches are blanked
    for k, line in enumerate(raw):
        word = re.match(r"^\s*#\s*(\w+)", line)
        word = word.group(1) if word else ""
        if word in ("if", "ifdef", "ifndef"):
            depth += 1
        elif word in ("else", "elif") and (skip_from is None or skip_from == depth):
            skip_from = depth
        elif word == "endif":
            if skip_from == depth:
                skip_from = None
            depth = max(0, depth - 1)
        elif skip_from is not None and k + 1 not in directives:
            lines[k] = " " * len(lines[k])
            directives.add(k + 1)
    return "\n".join(lines)


def parse_c(path, text, lang, lines=None):
    """Find the functions of a C, C++ or Arduino file.

    Returns:
        (LineInfo, the entries, the module entry).
    """
    lines = lines if lines is not None else text.split("\n")
    masked, comments, directives = c_mask(text)
    info = LineInfo(lines)
    masked_lines = masked.split("\n")
    for number in range(1, len(lines) + 1):
        has_code = number - 1 < len(masked_lines) and masked_lines[number - 1].strip()
        info.kind[number] = "k" if has_code or number in directives else (
            "b" if not lines[number - 1].strip() else "k")
    for first, last, value, block in comments:
        if block:
            parts = value[2:-2].split("\n")
            for k, part in enumerate(parts):
                row = first + k
                if info.kind[row] != "k" or not masked_lines[row - 1].strip():
                    if row not in directives:
                        info.comment(row, comment_body(re.sub(r"^\s*\*", "", part).rstrip()))
        elif not masked_lines[first - 1].strip() and first not in directives:
            info.comment(first, comment_body(value[2:].rstrip()))

    name = PurePosixPath(path).name
    module = new_entry(path, lang, name, name, "", "module", 1, len(lines))
    module["body"] = 1
    entries = []
    frames = [("file", None, "")]
    offsets = [0]
    for ln in lines:
        offsets.append(offsets[-1] + len(ln) + 1)

    def line_of(pos):
        """Return the line number of a position in the text."""
        lo, hi = 0, len(offsets) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if offsets[mid] <= pos:
                lo = mid
            else:
                hi = mid - 1
        return lo + 1

    last_stop = 0
    for k, c in enumerate(masked):
        if c in ";":
            if frames[-1][0] in ("file", "ns", "type"):
                last_stop = k + 1
            continue
        if c == "{":
            header = " ".join(masked[last_stop:k].split())
            kind = frames[-1][0]
            frame = ("block", None, "")
            if kind in ("file", "ns", "type"):
                frame = _c_header(path, lang, header, frames, entries, line_of,
                                  last_stop, masked, k)
            frames.append(frame)
            last_stop = k + 1
        elif c == "}":
            if len(frames) > 1:
                frame = frames.pop()
                if frame[1] is not None:
                    frame[1]["end"] = line_of(k)
            if frames[-1][0] in ("file", "ns", "type"):
                last_stop = k + 1

    # Documentation: the comments right above each function.
    by_end = {}
    for comment in comments:
        by_end.setdefault(comment[1], []).append(comment)
    for entry in entries:
        above = by_end.get(entry["line"] - 1, [])
        if above and above[-1][3]:
            raw = above[-1][2]
            entry["doc"] = parse_jsdoc(raw) if raw.startswith("/**") else parse_comment_doc(
                [re.sub(r"^\s*\*\s?", "", ln) for ln in raw[2:-2].split("\n")])
            entry["help"] = raw
            entry["start"] = above[-1][0]
            continue
        run = []
        row = entry["line"] - 1
        while row >= 1 and info.kind[row] == "c" and lines[row - 1].strip().startswith("//"):
            run.insert(0, info.text[row])
            row -= 1
        if run:
            entry["doc"] = parse_comment_doc(run)
            entry["help"] = "\n".join(run)
            entry["start"] = row + 1

    # The file's comment at the top.
    head = []
    row = 1
    while row <= len(lines) and info.kind[row] in ("c", "b") and (head or info.kind[row] == "c"):
        if info.kind[row] == "b":
            break
        head.append(info.text[row])
        row += 1
    if head:
        module["doc"] = parse_comment_doc(head)
        module["help"] = "\n".join(head)
        module["body"] = row
    for entry in entries:
        entry["parent"] = entry["parent"] or module["id"]
    _c_refs(entries, masked_lines, info)
    return info, entries, module


def _c_header(path, lang, header, frames, entries, line_of, header_pos, masked, brace):
    """Classify what a { at file or class level opens."""
    if re.match(r"^(typedef\s+)?(struct|class|union|enum)\b[^()]*$", header):
        match = re.match(r"^(?:typedef\s+)?(?:struct|class|union|enum)\s+(\w+)", header)
        return ("type", None, match.group(1) if match else "")
    if re.match(r"^(namespace\b.*|extern\s*)$", header):
        return ("ns", None, "")
    # A function: ... name(params) [qualifiers] [: initialisers] {
    text = re.sub(r"\)\s*:(?!:).*$", ")", header)
    text = re.sub(r"\)\s*(const|noexcept|override|final|volatile|->\s*[\w:<>*& ]+|\s)*$", ")",
                  text)
    if not text.endswith(")"):
        return ("block", None, "")
    depth = 0
    open_at = None
    for k in range(len(text) - 1, -1, -1):
        if text[k] == ")":
            depth += 1
        elif text[k] == "(":
            depth -= 1
            if depth == 0:
                open_at = k
                break
    if open_at is None:
        return ("block", None, "")
    match = re.search(r"(~?[A-Za-z_]\w*(?:::~?[A-Za-z_]\w*)*|operator\s*\S+)\s*$",
                      text[:open_at])
    if not match or match.group(1) in C_KEYWORDS or "=" in text[:match.start()]:
        return ("block", None, "")
    name = match.group(1)
    owner = next((f[2] for f in reversed(frames) if f[0] == "type" and f[2]), "")
    local = "%s::%s" % (owner, name) if owner and "::" not in name else name
    # The header's first line: where its text starts.
    start_pos = header_pos
    while start_pos < brace and masked[start_pos].isspace():
        start_pos += 1
    line = line_of(start_pos)
    kind = "method" if owner or "::" in name else "function"
    entry = new_entry(path, lang, name.split("::")[-1], local, local, kind, line, line)
    entry["sig"] = signature_text(header)
    entry["body"] = line_of(brace) + 1
    entries.append(entry)
    return ("func", entry, "")


def _c_refs(entries, masked_lines, info):
    """Record the calls in each C function: names followed by (."""
    for entry in entries:
        for n in range(entry["body"], entry["end"] + 1):
            code = masked_lines[n - 1] if n - 1 < len(masked_lines) else ""
            for match in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", code):
                entry["refs"].append(("name", match.group(1), n))


# Shell

SH_FUNCTION_RE = re.compile(
    r"^\s*(?:function\s+([\w:.-]+)\s*(?:\(\))?|([\w:.-]+)\s*\(\))\s*\{?\s*(?:#.*)?$")
SH_HEREDOC_RE = re.compile(r"<<(-?)\s*['\"]?([A-Za-z_]\w*)['\"]?")


def parse_shell(path, text, lang="shell", lines=None):
    """Find the functions of a shell script (and Makefile comments).

    Returns:
        (LineInfo, the entries, the module entry).
    """
    lines = lines if lines is not None else text.split("\n")
    info = LineInfo(lines)
    heredoc = None
    code = [""]
    for number, line in enumerate(lines, 1):
        stripped = line.strip()
        if heredoc:
            code.append("")
            if (stripped if heredoc[0] else line) == heredoc[1]:
                heredoc = None
            continue
        if stripped.startswith("#") and not (number == 1 and stripped.startswith("#!")):
            info.comment(number, comment_body(stripped[1:].rstrip()))
            code.append("")
            continue
        bare = re.sub(r"'[^']*'|\"(?:\\.|[^\"\\])*\"", "''", line)
        bare = re.sub(r"\s#.*$", "", bare)
        code.append(bare)
        match = SH_HEREDOC_RE.search(bare)
        if match:
            heredoc = (bool(match.group(1)), match.group(2))

    name = PurePosixPath(path).name
    module = new_entry(path, lang, name, name, "", "module", 1, len(lines))
    module["body"] = 1
    entries = []
    if lang == "shell":
        number = 1
        while number <= len(lines):
            match = SH_FUNCTION_RE.match(code[number]) if code[number] else None
            if not match:
                number += 1
                continue
            fname = match.group(1) or match.group(2)

            # Its end: where the braces opened since its line balance.
            depth = 0
            opened = False
            end = number
            for row in range(number, len(lines) + 1):
                stripped_code = re.sub(r"\$\{[^}]*\}", "", code[row])
                opens = len(re.findall(r"(?:^|[\s;])\{(?=\s|$)", stripped_code))
                closes = len(re.findall(r"(?:^|[\s;])\}(?=[\s;)|&]|$)", stripped_code))
                depth += opens - closes
                opened = opened or opens > 0
                end = row
                if opened and depth <= 0:
                    break
            entry = new_entry(path, lang, fname, fname, fname, "function", number, end)
            entry["sig"] = signature_text(lines[number - 1])
            entry["parent"] = module["id"]
            run = []
            row = number - 1
            while row >= 1 and info.kind[row] == "c":
                run.insert(0, info.text[row])
                row -= 1
            if run:
                entry["doc"] = parse_comment_doc(run)
                entry["help"] = "\n".join(run)
                entry["start"] = row + 1
            entries.append(entry)
            number = end + 1

    # The header comment (after the #! line).
    head = []
    row = 2 if lines and lines[0].startswith("#!") else 1
    while row <= len(lines) and info.kind[row] == "c":
        head.append(info.text[row])
        row += 1
    if head:
        module["doc"] = parse_comment_doc(head)
        module["help"] = "\n".join(head)
        module["body"] = row
    names = {e["name"] for e in entries}
    for entry in entries + [module]:
        for row in range(entry["body"], entry["end"] + 1):
            for word in re.findall(r"[\w:.-]+", code[row] if row < len(code) else ""):
                if word in names:
                    entry["refs"].append(("name", word, row))
    return info, entries, module


# Simulink diagrams


def diagram_blocks(slx_bytes):
    """Return the MATLAB Function blocks of a Simulink diagram (.slx).

    Returns:
        [(block path, its exact path, script)], one per block, in the
        diagram's order. The first has its line breaks as spaces (to
        show); the exact one is what Simulink calls it.
    """
    out = []
    try:
        archive = zipfile.ZipFile(io.BytesIO(slx_bytes))
    except zipfile.BadZipFile:
        return out
    names = [n for n in archive.namelist() if n.startswith("simulink/stateflow/")
             and n.endswith(".xml") and "/_rels/" not in n]
    for name in sorted(names, key=_chart_order):
        try:
            root = ElementTree.fromstring(archive.read(name))
        except ElementTree.ParseError:
            continue
        charts = [root] if root.tag == "chart" else root.iter("chart")
        for chart in charts:
            raw = None
            for prop in chart.findall("P"):
                if prop.get("Name") == "name":
                    raw = prop.text or ""
            if not raw:
                continue
            block = " ".join(raw.split())
            for state in chart.iter("state"):
                for eml in state.findall("eml"):
                    for prop in eml.findall("P"):
                        if prop.get("Name") == "script" and prop.text:
                            out.append((block, raw, prop.text.replace("\r\n", "\n")))
    return out


def _chart_order(name):
    """Sort chart_12.xml before chart_100.xml."""
    match = re.search(r"(\d+)", name)
    return (int(match.group(1)) if match else 0, name)


# Any file


def module_qual(path):
    """Return the name a file is known by in its language.

    MATLAB: spot.sim.run (packages from +folders); Python: the
    module's dotted name; others: the file name.
    """
    parts = PurePosixPath(path).parts
    if path.endswith(".m"):
        packages = [p[1:] for p in parts[:-1] if p.startswith("+")]
        return ".".join(packages + [PurePosixPath(path).stem])
    if path.endswith(".py"):
        if "spot_console" in parts:
            k = parts.index("spot_console")
            stem = PurePosixPath(path).stem
            mods = list(parts[k:-1]) + ([] if stem == "__init__" else [stem])
            return ".".join(mods)
        return PurePosixPath(path).stem
    return PurePosixPath(path).name


def parse_file(path, text, lang):
    """Parse one file into its record, its entries and its chunks.

    Args:
        path: The file's path relative to the project ("+spot/...").
        text: Its contents (line ends already \\n).
        lang: Its language (language_of).

    Returns:
        (file record, entries), the module entry first.
    """
    text = text.lstrip("\ufeff")
    lines = text.split("\n")
    if lines and lines[-1] == "":
        lines = lines[:-1]
    record = {"path": path, "lang": lang, "lines": len(lines), "entries": []}
    if lang not in CODE_LANGUAGES:
        return record, []

    error = None
    if lang == "matlab":
        qual = module_qual(path)
        prefix = qual[:qual.rfind(".") + 1] if "." in qual else ""
        info, entries, module = parse_matlab(path, text, prefix, file_lines=lines)
    elif lang == "python":
        info, entries, module, error = parse_python(path, text, module_qual(path), lines)
    elif lang == "javascript":
        info, entries, module = parse_javascript(path, text, lines)
    elif lang in ("c", "cpp", "arduino"):
        info, entries, module = parse_c(path, text, lang, lines)
    else:
        info, entries, module = parse_shell(path, text, lang, lines)
    if error:
        record["error"] = error

    finish_entries(info, module, entries, lang)
    record["entries"] = [module["id"]] + [e["id"] for e in entries]
    record["summary"] = (module["doc"] or {}).get("summary", "")
    return record, [module] + entries


def finish_entries(info, module, entries, lang):
    """Link parents and children, make ids unique, chunk the bodies."""
    # Ids must be unique within a file (two functions of one name).
    seen = {module["id"]: module}
    for entry in entries:
        base = entry["id"]
        count = 2
        while entry["id"] in seen:
            entry["id"] = "%s~%d" % (base, count)
            count += 1
        seen[entry["id"]] = entry
    by_id = seen

    # Every entry's children, and placeholders for them in its body.
    for entry in entries:
        parent = by_id.get(entry["parent"]) or module
        entry["parent"] = parent["id"]
        parent["children"].append(entry["id"])
    for entry in [module] + entries:
        kids = [by_id[c] for c in entry["children"]]
        spans = []
        for kid in kids:
            line_text = info.lines[kid["line"] - 1] if kid["line"] - 1 < len(info.lines) else ""
            col = kid.get("col")
            alone = col is None or not line_text[:col].strip()
            if alone and kid["end"] >= kid["start"]:
                spans.append((kid["start"], kid["end"], kid["id"]))
        spans = _outermost(spans)
        first = entry["body"] if entry is not module else module["body"]
        last = entry["end"]
        if entry is not module and lang in ("matlab",) and entry["kind"] != "file":
            last = entry["end"] - 1   # the closing "end"
        if entry is not module and lang in C_LIKE and entry["end"] > entry["line"]:
            last = entry["end"] - 1   # the closing brace
        if entry is not module and lang == "shell":
            last = entry["end"] - 1
        if entry is module and module["kind"] == "class":
            last = module["end"] - 1
        if entry is not module and entry["end"] == entry["line"] and lang in C_LIKE:
            first = last = entry["line"]
        if last >= first:
            entry["chunks"] = merge_closers(chunk_body(info, first, last, spans, lang), info)
        entry.pop("col", None)
        entry.pop("props_pending", None)

    # Documentation the code doesn't give: standard methods, and
    # helpers that share the comment of their group.
    for entry in entries:
        if not entry["doc"]:
            entry["doc"] = standard_doc(entry, by_id)
    inherit_group_docs(entries, by_id, info)
    block_comment_docs(entries, info)
    for entry in entries:
        entry.pop("group_of", None)


# Methods whose job is set by MATLAB, Simulink or Python, documented
# once here rather than in each class: name -> (what calls it, what it
# does).
STANDARD_METHODS = {
    "matlab": {
        "setupImpl": ("Simulink, once before the first step",
                      "Sets up the block's internal state from its properties."),
        "resetImpl": ("Simulink, when the simulation starts (after setupImpl) and on a reset",
                      "Puts the block's state back to its initial values."),
        "stepImpl": ("Simulink, at every step of the block",
                     "Computes the block's outputs from its inputs and updates its state."),
        "releaseImpl": ("Simulink, when the simulation ends",
                        "Frees what setupImpl opened (files, ports, devices)."),
        "getNumInputsImpl": ("Simulink, when it sets up the block",
                             "Returns how many input ports the block has."),
        "getNumOutputsImpl": ("Simulink, when it sets up the block",
                              "Returns how many output ports the block has."),
        "getOutputSizeImpl": ("Simulink, when it sets up the block",
                              "Returns the size of each output."),
        "getOutputDataTypeImpl": ("Simulink, when it sets up the block",
                                  "Returns the data type of each output."),
        "isOutputComplexImpl": ("Simulink, when it sets up the block",
                                "Returns whether each output is complex (false: all are real)."),
        "isOutputFixedSizeImpl": ("Simulink, when it sets up the block",
                                  "Returns whether each output keeps the same size."),
        "isOutputSizeLockedImpl": ("Simulink, when it sets up the block",
                                   "Returns whether an output's size can change between runs."),
        "isInputFixedSizeImpl": ("Simulink, when it sets up the block",
                                 "Returns whether each input keeps the same size."),
        "isInputSizeMutableImpl": ("Simulink, when it sets up the block",
                                   "Returns whether an input's size may change while it runs."),
        "isInputComplexityMutableImpl": ("Simulink, when it sets up the block",
                                         "Returns whether an input may change between real "
                                         "and complex."),
        "isInputDirectFeedthroughImpl": ("Simulink, when it orders the blocks",
                                         "Returns whether each input affects the outputs in "
                                         "the same step (direct feedthrough)."),
        "getInputNamesImpl": ("Simulink, to label the block",
                              "Returns the names shown on the input ports."),
        "getOutputNamesImpl": ("Simulink, to label the block",
                               "Returns the names shown on the output ports."),
        "getIconImpl": ("Simulink, to draw the block",
                        "Returns the text shown on the block's icon."),
        "getHeaderImpl": ("Simulink, for the block's dialog",
                          "Returns the title and description shown at the top of the dialog."),
        "getPropertyGroupsImpl": ("Simulink, for the block's dialog",
                                  "Returns how the dialog groups the block's parameters."),
        "getSimulateUsingImpl": ("Simulink, for the block's dialog",
                                 "Returns how the block is simulated: as interpreted MATLAB "
                                 "or as generated code."),
        "showSimulateUsingImpl": ("Simulink, for the block's dialog",
                                  "Returns whether the dialog offers the Simulate Using "
                                  "choice."),
        "validatePropertiesImpl": ("Simulink, before the simulation starts",
                                   "Checks the block's parameters and stops with an error "
                                   "if one is not valid."),
        "validateInputsImpl": ("Simulink, before the simulation starts",
                               "Checks the inputs' sizes and types."),
        "isInactivePropertyImpl": ("Simulink, for the block's dialog",
                                   "Returns whether a parameter is greyed out (it doesn't "
                                   "apply with the other settings)."),
        "getDiscreteStateImpl": ("Simulink, when it saves or inspects the state",
                                 "Returns the block's discrete state."),
        "saveObjectImpl": ("MATLAB, when the object is saved",
                           "Returns the properties to save, including private state."),
        "loadObjectImpl": ("MATLAB, when the object is loaded",
                           "Restores the properties saved by saveObjectImpl."),
        "getSampleTimeImpl": ("Simulink, when it sets up the block",
                              "Returns the block's sample time."),
        "supportsMultipleInstanceImpl": ("Simulink, when it sets up the block",
                                         "Returns whether the block can be used in a For "
                                         "Each subsystem."),
        "getDescriptiveName": ("MATLAB Coder, when it builds code that uses the class",
                               "Returns the name of the external code library."),
        "isSupportedContext": ("MATLAB Coder, when it builds code that uses the class",
                               "Returns whether the external code can be used for this "
                               "build target."),
        "updateBuildInfo": ("MATLAB Coder, when it builds code that uses the class",
                            "Adds the C or C++ source files, include folders and link flags "
                            "to the build."),
        "delete": ("MATLAB, when the object is destroyed",
                   "Releases what the object holds (connections, timers)."),
    },
    "python": {
        "__init__": ("Python, when the object is created", "Sets up the object's attributes."),
        "__repr__": ("Python, to show the object", "Returns the object as text."),
        "__str__": ("Python, to show the object", "Returns the object as text."),
        "__enter__": ("Python, at the start of a with block",
                      "Starts using the object and returns it."),
        "__exit__": ("Python, at the end of a with block", "Stops using the object."),
        "__call__": ("Python, when the object is called like a function", ""),
        "__len__": ("Python, for len()", "Returns the number of items."),
        "__iter__": ("Python, when the object is looped over", "Returns an iterator."),
    },
    "javascript": {
        "constructor": ("JavaScript, when the object is created with new",
                        "Sets up the object's fields."),
    },
}


# Small helpers the page's modules each define the same way: name, a
# pattern its code must contain, and what it does.
JS_HELPERS = [
    ("$", r"querySelector\(", "Returns the first element that matches a CSS selector "
     "(document.querySelector), or null."),
    ("$$", r"querySelectorAll\(", "Returns every element that matches a CSS selector, as an "
     "array."),
    ("esc", r"&amp;", "Returns text made safe to put in HTML: &, <, > and \" replaced by their "
     "entities (null and undefined become '')."),
    ("list", r"Array\.isArray", "Returns its argument as an array: an array as it is, null or "
     "undefined as [], anything else as an array of one (MATLAB's JSON gives a one-element "
     "list as the element itself)."),
]


def standard_doc(entry, entries):
    """Return the documentation of a standard method, or None.

    Also covers a MATLAB class's constructor (the method named after
    its class), and the page's small shared helpers ($, $$, esc,
    list).
    """
    if entry["lang"] == "javascript" and entry["kind"] in ("function", "nested"):
        for name, pattern, text in JS_HELPERS:
            if entry["name"] == name and re.search(pattern, entry["sig"]):
                return {"summary": text, "blocks": [], "helper": True}
    if entry["kind"] != "method":
        return None
    known = STANDARD_METHODS.get(entry["lang"], {})
    caller, text = known.get(entry["name"], (None, None))
    parent = entries.get(entry["parent"])
    if caller is None and parent is not None and entry["name"] == parent["name"]:
        caller, text = ("MATLAB, when the object is created",
                        "Creates a %s object. Name-value arguments set its properties."
                        % entry["name"])
    if caller is None:
        return None
    blocks = [{"type": "para", "text": "Called by %s." % caller}]
    return {"summary": text or "Called by %s." % caller, "blocks": blocks, "standard": True}


def inherit_group_docs(entries, by_id, info):
    """Give undocumented functions the comment of the group they are in.

    In a run of short definitions under one comment (a comment, then
    several one-line helpers with no blank line between them), only
    the first has the comment above it; the others share it.
    """
    for entry in entries:
        if entry["doc"] or entry["kind"] in ("module", "file", "class"):
            continue
        parent = by_id.get(entry["parent"])
        if parent is None:
            continue
        siblings = [by_id[c] for c in parent["children"]]
        earlier = [s for s in siblings if s["end"] < entry["start"]]
        if not earlier:
            continue
        prev = max(earlier, key=lambda s: s["end"])
        gap = range(prev["end"] + 1, entry["start"])
        if any(info.kind[n] != "k" for n in gap) or len(gap) > 1:
            continue
        source = prev if prev["doc"] else prev.get("group_of")
        if isinstance(source, dict) and source.get("doc") and not source["doc"].get("standard"):
            entry["doc"] = dict(source["doc"], group=source["id"])
            entry["group_of"] = source


def block_comment_docs(entries, info):
    """Give undocumented functions the comment over their block.

    The coding standard puts a comment over every block of code, and a
    block runs to the next blank line: a function defined inside one
    (a handler in a loop, a method of an object) is described by that
    comment. Walk up from the function through its block's lines to
    the comment that starts it.
    """
    for entry in entries:
        if entry["doc"] or entry["kind"] in ("module", "file", "class", "script", "block"):
            continue
        row = entry["start"] - 1
        while row >= 1 and info.kind[row] == "k":
            row -= 1
        if row < 1 or info.kind[row] != "c":
            continue
        last = row
        while (row - 1 >= 1 and info.kind[row - 1] == "c"
               and info.indent[row - 1] == info.indent[last]):
            row -= 1
        doc = parse_comment_doc([info.text[n] for n in range(row, last + 1)])
        if doc:
            doc["block"] = row
            entry["doc"] = doc


def merge_closers(chunks, info):
    """Join chunks of only closing lines (end, }) to the one before."""
    out = []
    for chunk in chunks:
        code = chunk.get("code")
        closing = (code and not chunk.get("text") and not chunk.get("title")
                   and code[1] - code[0] < 3
                   and all(re.match(r"^\s*(end|[})\]]+[;,)]*|\);?)?\s*$", info.lines[n - 1])
                           for n in range(code[0], code[1] + 1)))
        prev = out[-1] if out else None
        if (closing and prev is not None and prev.get("code")
                and prev["code"][1] >= code[0] - 2):
            prev["code"][1] = code[1]
            prev["end"] = code[1]
            continue
        out.append(chunk)
    return out


def _outermost(spans):
    """Drop spans that lie inside another span."""
    out = []
    for span in sorted(spans, key=lambda s: (s[0], -s[1])):
        if out and span[0] >= out[-1][0] and span[1] <= out[-1][1]:
            continue
        out.append(span)
    return out


# Cross references


def cross_reference(files, entries):
    """Fill in each entry's calls and called_by, and collect the routes.

    Args:
        files: {path: file record}.
        entries: {id: entry} (every file's).

    Returns:
        {"routes": [...], "commands": [...]}: the console's HTTP routes
        and MATLAB commands, with their handlers and callers.
    """
    header_docs(files, entries)
    by_lang = {}
    for entry in entries.values():
        by_lang.setdefault(entry["lang"], []).append(entry)
        entry["calls"] = []
        entry["called_by"] = []

    _link_matlab(by_lang.get("matlab", []), entries)
    _link_python(by_lang.get("python", []), entries, files)
    _link_javascript(by_lang.get("javascript", []), entries)
    for lang in ("c", "cpp", "arduino", "shell"):
        _link_by_file(by_lang.get(lang, []), entries)

    # Each link both ways, without repeats.
    for entry in entries.values():
        seen = set()
        calls = []
        for target, line in entry["calls"]:
            if target != entry["id"] and target not in seen:
                seen.add(target)
                calls.append([target, line])
        entry["calls"] = calls
    for entry in entries.values():
        for target, line in entry["calls"]:
            entries[target]["called_by"].append([entry["id"], line])
    result = _routes(entries)
    for entry in entries.values():
        entry.pop("refs", None)
        entry.pop("strings", None)
        entry.pop("imports", None)
    return result


def header_docs(files, entries):
    """Document C and C++ functions from the comments in their headers.

    C code usually describes a function where it is declared, in a
    header (.h), not where it is defined. For a function the source
    file doesn't describe, look for its declaration in a header of the
    same name (next to it, or in an include folder beside it) and use
    the comment right above it.
    """
    headers = {}
    for path, record in files.items():
        if path.endswith((".h", ".hpp")) and record.get("text") is not None:
            headers.setdefault(PurePosixPath(path).stem, []).append(record)
    for entry in entries.values():
        if entry["doc"] or entry["lang"] not in ("c", "cpp") or entry["kind"] == "module":
            continue
        for record in headers.get(PurePosixPath(entry["file"]).stem, []):
            lines = record["text"].split("\n")
            declared = re.compile(r"\b%s\s*\(" % re.escape(entry["name"]))
            row = next((k for k, ln in enumerate(lines)
                        if declared.search(ln) and not ln.lstrip().startswith(("/*", "*", "//"))),
                       None)
            if row is None:
                continue
            comment = _comment_above(lines, row)
            if comment:
                doc = parse_comment_doc(comment)
                if doc:
                    doc["header"] = record["path"]
                    entry["doc"] = doc
                    break


def _comment_above(lines, row):
    """Return the comment lines right above line `row` (from 0)."""
    k = row - 1
    if k < 0 or not lines[k].strip():
        return []
    text = []
    if k >= 0 and lines[k].rstrip().endswith("*/"):
        while k >= 0:
            text.insert(0, lines[k])
            if "/*" in lines[k]:
                break
            k -= 1
        body = "\n".join(text)
        body = body[body.find("/*") + 2:body.rfind("*/")].lstrip("*")
        return [re.sub(r"^\s*\*?\s?", "", ln) for ln in body.split("\n")]
    while k >= 0 and lines[k].lstrip().startswith("//"):
        text.insert(0, comment_body(lines[k].lstrip()[2:].rstrip()))
        k -= 1
    return text


def _ancestors(entry, entries):
    """Return an entry and the entries around it, innermost first."""
    out = []
    while entry is not None:
        out.append(entry)
        entry = entries.get(entry["parent"]) if entry["parent"] else None
    return out


def _link_matlab(mentries, entries):
    """Link MATLAB entries: spot.* and interface.* names, local functions, methods."""
    by_qual = {}
    for entry in sorted(mentries, key=lambda e: e["kind"] in ("file", "script")):
        by_qual.setdefault(entry["qual"], entry)
    by_file = {}
    for entry in mentries:
        by_file.setdefault(entry["file"], {}).setdefault(entry["name"], entry)
    for entry in mentries:
        local = by_file.get(entry["file"], {})
        for ref in entry["refs"]:
            if ref[0] == "attr":
                # obj.method(...): a method of the same class.
                target = local.get(ref[2])
                if target is not None and target["kind"] == "method":
                    entry["calls"].append((target["id"], ref[3]))
                continue
            name, line = ref[1], ref[2]
            if name.startswith(("spot.", "interface.")):
                # The longest known prefix: spot.pkg.Class.method, a
                # class, or a function.
                parts = name.split(".")
                for k in range(len(parts), 1, -1):
                    target = by_qual.get(".".join(parts[:k]))
                    if target is not None:
                        entry["calls"].append((target["id"], line))
                        break
            elif name in local and local[name]["kind"] in ("local", "nested", "method"):
                entry["calls"].append((local[name]["id"], line))
            elif name in ("SPOT",) and name in by_qual:
                entry["calls"].append((by_qual[name]["id"], line))


def _link_python(pentries, entries, files):
    """Link Python entries through imports, classes and unique names."""
    modules = {e["file"]: e for e in pentries if e["kind"] == "module"}
    by_dotted = {e["qual"]: e for e in modules.values()}
    top_names = {}
    for entry in pentries:
        parent = entries.get(entry["parent"])
        if parent is not None and parent["kind"] == "module":
            top_names.setdefault(entry["file"], {})[entry["name"]] = entry
    method_names = {}
    for entry in pentries:
        if entry["kind"] == "method":
            method_names.setdefault(entry["name"], []).append(entry)

    def resolve_import(module, alias):
        """Return the entry an imported name refers to, or None."""
        spec = module.get("imports", {}).get(alias)
        if not spec:
            return None
        source, name, level = spec
        package = module["qual"].rsplit(".", 1)[0] if level else ""
        if module["file"].endswith("__init__.py"):
            package = module["qual"]
        dotted = ((package + "." + source) if source else package) if level else source
        target_module = by_dotted.get(dotted)
        if name is None:
            return target_module
        if target_module is not None:
            hit = top_names.get(target_module["file"], {}).get(name)
            if hit is not None:
                return hit
        # "from . import local": a module of the package.
        return by_dotted.get(dotted + "." + name)

    for entry in pentries:
        module = modules.get(entry["file"])
        if module is None:
            continue
        chain = _ancestors(entry, entries)
        cls = next((e for e in chain if e["kind"] == "class"), None)
        for ref in entry["refs"]:
            target = None
            if ref[0] == "name":
                name, line = ref[1], ref[2]
                for scope in chain:
                    kid = next((entries[c] for c in scope["children"]
                                if entries[c]["name"] == name), None)
                    if kid is not None and scope["kind"] != "class":
                        target = kid
                        break
                if target is None:
                    target = resolve_import(module, name)
                    if target is not None and target["kind"] == "module":
                        target = None
            else:
                base, attr, line = ref[1], ref[2], ref[3]
                if base in ("self", "cls") and cls is not None:
                    target = next((entries[c] for c in cls["children"]
                                   if entries[c]["name"] == attr), None)
                elif base:
                    imported = resolve_import(module, base)
                    if imported is not None and imported["kind"] == "module":
                        target = top_names.get(imported["file"], {}).get(attr)
                    elif imported is not None and imported["kind"] == "class":
                        target = next((entries[c] for c in imported["children"]
                                       if entries[c]["name"] == attr), None)
                if target is None and base not in ("self", "cls") and len(
                        method_names.get(attr, [])) == 1 and not attr.startswith("__"):
                    target = method_names[attr][0]
            if target is not None:
                entry["calls"].append((target["id"], line))


def _link_javascript(jentries, entries):
    """Link JavaScript entries through scopes, objects and imports."""
    modules = {e["file"]: e for e in jentries if e["kind"] == "module"}
    exported = {}
    for entry in jentries:
        parent = entries.get(entry["parent"])
        if parent is not None and parent["kind"] == "module":
            exported.setdefault(entry["file"], {})[entry["name"]] = entry
    for entry in jentries:
        module = modules.get(entry["file"])
        if module is None:
            continue
        chain = _ancestors(entry, entries)
        folder = str(PurePosixPath(entry["file"]).parent)
        for ref in entry["refs"]:
            target = None
            if ref[0] == "name":
                name, line = ref[1], ref[2]
                for scope in chain:
                    kid = next((entries[c] for c in scope["children"]
                                if entries[c]["name"] == name
                                and entries[c]["kind"] != "handler"), None)
                    if kid is not None:
                        target = kid
                        break
                if target is None and name in module.get("imports", {}):
                    source, exported_name = module["imports"][name]
                    path = str(PurePosixPath(folder) / source).replace("/./", "/")
                    path = _normalize(path)
                    target = exported.get(path, {}).get(exported_name)
            else:
                base, attr, line = ref[1], ref[2], ref[3]
                if base:
                    for scope in chain:
                        holder = next((entries[c] for c in scope["children"]
                                       if entries[c]["local"].endswith("." + base + "." + attr)
                                       or entries[c]["local"] == base + "." + attr), None)
                        if holder is not None:
                            target = holder
                            break
            if target is not None:
                entry["calls"].append((target["id"], line))


def _normalize(path):
    """Resolve ".." in a POSIX path."""
    parts = []
    for part in path.split("/"):
        if part == "..":
            if parts:
                parts.pop()
        elif part not in ("", "."):
            parts.append(part)
    return "/".join(parts)


def _link_by_file(fentries, entries):
    """Link functions by name within each file and its folder."""
    by_folder = {}
    for entry in fentries:
        if entry["kind"] != "module":
            folder = str(PurePosixPath(entry["file"]).parent)
            by_folder.setdefault(folder, {}).setdefault(entry["name"], entry)
    for entry in fentries:
        folder = str(PurePosixPath(entry["file"]).parent)
        names = by_folder.get(folder, {})
        for _, name, line in entry["refs"]:
            if name in names:
                entry["calls"].append((names[name]["id"], line))


def _routes(entries):
    """Collect the console's HTTP routes and MATLAB commands."""
    routes = {}
    commands = {}
    route_re = re.compile(r"^/api/[\w/.-]*[\w]$")

    # Handlers: the server's Python that compares the path with them.
    for entry in entries.values():
        if entry["lang"] != "python":
            continue
        for value, line in entry.get("strings", []):
            if route_re.match(value):
                item = routes.setdefault(value, {"route": value, "handlers": [], "callers": []})
                item["handlers"].append([entry["id"], line])
    # The MATLAB worker's commands: the cases of Worker.dispatch, and
    # where the server lists them (MATLAB_COMMANDS).
    for entry in entries.values():
        if entry["lang"] == "matlab" and entry["name"] == "dispatch":
            for value, line in entry.get("strings", []):
                if re.match(r"^[a-z]+\.[A-Za-z]+$", value) or value in ("ping", "info"):
                    item = commands.setdefault(value, {"command": value, "handlers": [],
                                                       "callers": [], "server": []})
                    item["handlers"].append([entry["id"], line])
    for entry in entries.values():
        if entry["lang"] == "python" and entry["kind"] == "module":
            for value, line in entry.get("strings", []):
                if value in commands:
                    commands[value]["server"].append([entry["id"], line])

    # Callers: the page's JavaScript that names them.
    for entry in entries.values():
        if entry["lang"] != "javascript":
            continue
        for value, line in entry.get("strings", []):
            route = value.split("?")[0]
            if route in routes:
                routes[route]["callers"].append([entry["id"], line])
            elif route_re.match(route.rstrip("/")) and route.rstrip("/") in routes:
                routes[route.rstrip("/")]["callers"].append([entry["id"], line])
            if value in commands:
                commands[value]["callers"].append([entry["id"], line])
    for entry in entries.values():
        if entry["lang"] == "javascript":
            for value, line in entry.get("strings", []):
                for route in routes:
                    if value != route and value.startswith(route + "?"):
                        routes[route]["callers"].append([entry["id"], line])
    return {"routes": sorted(routes.values(), key=lambda r: r["route"]),
            "commands": sorted(commands.values(), key=lambda c: c["command"])}
