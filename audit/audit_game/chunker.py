"""Splits a file into the chunks the audit shows: [comment] -> [code].

A chunk is a run of lines of the original file. Joined in order, a
file's chunks give the file back byte for byte, so a rewritten chunk
can replace its lines without touching the rest. Every language gets
its own line classes (comment, code, definition, section) and the
same assembly rules:

    1. A comment that follows code or a blank line starts a chunk,
       and so does a definition (function, def, class) that has no
       comment over it, and a section (%% in MATLAB, a heading in
       Markdown).
    2. Tiny code-only chunks (a function line, an end) are merged into
       a neighbour, so every chunk has something to say.
    3. Long chunks are split at blank lines into parts, so one chunk
       fits on a screen.

Part of the SPOT audit game (audit/app.py starts it).
"""

import hashlib
import os
import re

# Extension -> (editor language, line-class family)
LANGUAGES = {
    ".m": ("matlab", "matlab"),
    ".py": ("python", "python"),
    ".js": ("javascript", "slash"),
    ".mjs": ("javascript", "slash"),
    ".cpp": ("cpp", "slash"),
    ".cc": ("cpp", "slash"),
    ".c": ("c", "slash"),
    ".h": ("cpp", "slash"),
    ".hpp": ("cpp", "slash"),
    ".ino": ("arduino", "slash"),
    ".css": ("css", "slash"),
    ".sh": ("shell", "hash"),
    ".service": ("ini", "hash"),
    ".html": ("html", "html"),
    ".md": ("markdown", "markdown"),
    ".tex": ("latex", "latex"),
    ".json": ("json", "json"),
    ".txt": ("text", "text"),
}
SPECIAL_NAMES = {"Makefile": ("shell", "hash")}

# Families whose chunks are prose rather than code (no code check).
PROSE_FAMILIES = {"markdown", "latex", "text", "html", "json"}

MAX_CODE_LINES = 45       # a code chunk longer than this is split
MAX_PROSE_CHARS = 2200    # a prose chunk bigger than this is split
MIN_PART_LINES = 6        # a split never leaves a part shorter than this
TINY_CHUNK_LINES = 3      # code-only chunks this short merge into a neighbour

CLOSER_RE = re.compile(
    r"^\s*(end;?|\}\)?;?|\);?|\];?|\}\);?|</\w+>|\\end\{\w+\})\s*$")
VERBATIM_END_RE = re.compile(r"end verbatim")
TOKEN_RE = re.compile(
    r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|`(?:\\.|[^`\\])*`'
    r"|[A-Za-z_]\w*|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?|\S")


def language_of(path):
    """Return (editor language, family) for a file path, or None."""
    name = os.path.basename(path)
    if name in SPECIAL_NAMES:
        return SPECIAL_NAMES[name]
    ext = os.path.splitext(name)[1].lower()
    return LANGUAGES.get(ext)


# Line classes


def classify(lines, family):
    """Return one class per line: blank, comment, code, def or section.

    A comment is a line that is only a comment (a trailing comment on
    code counts as code). A docstring, a %{ %} block and a /* */ block
    are comments line for line, blank lines inside them included, so
    they are never split.
    """
    kinds = []
    state = {"block": False, "fence": False, "delim": None}
    for raw in lines:
        s = raw.strip()
        kind = "code"
        if family == "matlab":
            kind = _matlab_kind(s, state)
        elif family == "python":
            kind = _python_kind(s, state)
        elif family == "hash":
            kind = _hash_kind(s)
        elif family == "slash":
            kind = _slash_kind(s, state)
        elif family == "markdown":
            kind = _markdown_kind(s, state)
        elif family == "latex":
            kind = _latex_kind(s)
        elif family == "html":
            kind = _html_kind(s, state)
        elif family == "json":
            kind = _json_kind(raw, s)
        else:
            kind = "blank" if not s else "code"
        kinds.append(kind)
    return kinds


def _matlab_kind(s, state):
    if state["block"]:
        if s == "%}":
            state["block"] = False
        return "comment"
    if not s:
        return "blank"
    if s == "%{":
        state["block"] = True
        return "comment"
    if s.startswith("%%"):
        return "section"
    if s.startswith("%"):
        return "comment"
    if re.match(r"(function|classdef|case|otherwise)\b", s):
        return "def"
    return "code"


def _python_kind(s, state):
    delim = state["delim"]
    if delim:
        if delim in s:
            state["delim"] = None
        return "comment"
    if not s:
        return "blank"
    m = re.match(r"(?:[rRuUbB]{0,2})(\"\"\"|''')", s)
    if m:
        d = m.group(1)
        rest = s[m.end():]
        if d not in rest:
            state["delim"] = d
        return "comment"
    if s.startswith("#"):
        return "comment"
    if re.match(r"(async\s+def|def|class)\b|@", s):
        return "def"
    return "code"


def _hash_kind(s):
    if not s:
        return "blank"
    if s.startswith("#"):
        return "comment"
    if re.match(r"(function\s+)?[A-Za-z_][\w-]*\s*\(\)\s*\{?\s*$", s):
        return "def"
    return "code"


def _slash_kind(s, state):
    if state["block"]:
        if "*/" in s:
            state["block"] = False
        return "comment"
    if not s:
        return "blank"
    if s.startswith("/*"):
        if "*/" not in s[2:]:
            state["block"] = True
        return "comment"
    if s.startswith("//"):
        return "comment"
    if re.match(r"(export\s+)?(default\s+)?(async\s+)?(function\b|class\b)", s):
        return "def"
    # A C function definition at the outer level: a type, a name, (args) and no ;
    if (re.match(r"[A-Za-z_][\w\s\*&:<>,]*\b[A-Za-z_]\w*\s*\([^;]*\)\s*(const\s*)?\{?\s*$", s)
            and not s.startswith(("if", "for", "while", "switch", "return", "else"))
            and ";" not in s):
        return "def"
    return "code"


def _markdown_kind(s, state):
    if s.startswith(("```", "~~~")):
        state["fence"] = not state["fence"]
        return "code"
    if state["fence"]:
        return "code"
    if not s:
        return "blank"
    if re.match(r"#{1,6}\s", s):
        return "section"
    if s.startswith("<!--"):
        return "comment"
    return "code"


def _latex_kind(s):
    if not s:
        return "blank"
    if s.startswith("%"):
        return "comment"
    if re.match(r"\\(part|chapter|section|subsection|subsubsection)\*?\{", s):
        return "section"
    return "code"


def _html_kind(s, state):
    if state["block"]:
        if "-->" in s:
            state["block"] = False
        return "comment"
    if not s:
        return "blank"
    if s.startswith("<!--"):
        if "-->" not in s:
            state["block"] = True
        return "comment"
    if re.match(r"<(section|header|aside|footer|nav|dialog|template|script|style|main)\b", s):
        return "def"
    return "code"


def _json_kind(raw, s):
    if not s:
        return "blank"
    if re.match(r" {0,2}\"", raw):
        return "def"
    return "code"


# Assembly


def chunk_file(rel_path, text):
    """Return the chunks of one file, in order.

    Args:
        rel_path: The file's path (used in the chunk keys).
        text: The file's whole text.

    Returns:
        A list of dicts: key, index, start and end (1-based, inclusive),
        text, title, kind (code or prose), lines, part ([k, n] or None),
        has_comment, has_code.
    """
    lang = language_of(rel_path)
    family = lang[1] if lang else "text"
    lines = text.splitlines(keepends=True)
    if not lines:
        return []
    kinds = classify(lines, family)
    ranges = _initial_ranges(kinds, family)
    ranges = _merge_tiny(lines, kinds, ranges, family)
    ranges = _coalesce_small(lines, kinds, ranges)
    parts = _split_long(lines, kinds, ranges, family)

    chunks = []
    seen = {}
    for index, (lo, hi, part) in enumerate(parts):
        body = "".join(lines[lo:hi])
        digest = hashlib.sha1(("%s\n%s" % (rel_path, body)).encode("utf-8")).hexdigest()[:16]
        n = seen.get(digest, 0) + 1
        seen[digest] = n
        key = digest if n == 1 else "%s~%d" % (digest, n)
        ks = kinds[lo:hi]
        chunks.append({
            "key": key,
            "index": index,
            "start": lo + 1,
            "end": hi,
            "text": body,
            "title": _title(lines[lo:hi], ks, family),
            "kind": "prose" if family in PROSE_FAMILIES else "code",
            "lines": hi - lo,
            "part": part,
            "has_comment": any(k in ("comment", "section") for k in ks),
            "has_code": any(k in ("code", "def") for k in ks),
        })
    return chunks


def _initial_ranges(kinds, family):
    """Chunk boundaries from the line classes (rule 1)."""
    starts = [0]
    prev_nonblank = None
    for i in range(1, len(kinds)):
        kind = kinds[i]
        before = kinds[i - 1]
        begins = False
        if kind == "section":
            begins = True
        elif kind == "comment":
            if before == "comment":
                begins = False
            elif before == "def" and family in ("matlab", "python"):
                # The help block or docstring belongs with its definition.
                begins = False
            else:
                begins = True
        elif kind == "def":
            begins = prev_nonblank not in ("comment", "def") and before != "def"
        if begins:
            starts.append(i)
        if kind != "blank":
            prev_nonblank = kind
    return [(starts[k], starts[k + 1] if k + 1 < len(starts) else len(kinds))
            for k in range(len(starts))]


def _merge_tiny(lines, kinds, ranges, family):
    """Merge chunks too small to stand alone into a neighbour (rule 2)."""
    out = []
    pending_next = None   # a tiny chunk waiting to join the next one
    for lo, hi in ranges:
        if pending_next is not None:
            lo = pending_next
            pending_next = None
        ks = kinds[lo:hi]
        has_comment = any(k in ("comment", "section") for k in ks)
        has_code = any(k in ("code", "def") for k in ks)
        nonblank = [lines[lo + j] for j, k in enumerate(ks) if k != "blank"]
        body = "".join(nonblank)
        if not has_comment and len(nonblank) <= TINY_CHUNK_LINES:
            closer = all(CLOSER_RE.match(l) for l in nonblank)
            if closer and out:
                out[-1] = (out[-1][0], hi)
            elif closer or not nonblank:
                if out:
                    out[-1] = (out[-1][0], hi)
                else:
                    pending_next = lo
            else:
                pending_next = lo
            continue
        if not has_code and len(nonblank) <= 2:
            if VERBATIM_END_RE.search(body) and out:
                out[-1] = (out[-1][0], hi)
            else:
                pending_next = lo
            continue
        out.append((lo, hi))
    if pending_next is not None:
        if out:
            out[-1] = (out[-1][0], len(lines))
        else:
            out.append((pending_next, len(lines)))
    return out


def _coalesce_small(lines, kinds, ranges):
    """Join runs of very small chunks (a comment and a line each) into one."""
    out = []
    for lo, hi in ranges:
        if out:
            plo, phi = out[-1]
            prev_small = sum(1 for k in kinds[plo:phi] if k != "blank") <= 3
            cur_small = sum(1 for k in kinds[lo:hi] if k != "blank") <= 3
            if prev_small and cur_small and hi - plo <= 14:
                out[-1] = (plo, hi)
                continue
        out.append((lo, hi))
    return out


def _split_long(lines, kinds, ranges, family):
    """Split chunks that are too long at blank lines (rule 3)."""
    prose = family in PROSE_FAMILIES
    out = []
    for lo, hi in ranges:
        parts = []
        s = lo
        while _too_long(lines, s, hi, prose):
            cut = None
            for c in range(s + MIN_PART_LINES, hi - MIN_PART_LINES + 1):
                if _too_long(lines, s, c, prose):
                    break
                if _can_cut(lines, kinds, c, family):
                    cut = c
            if cut is None:
                # No blank line to cut at: cut where the limit falls, unless
                # that leaves a sliver, when the chunk stays a little long.
                cut = s + MAX_CODE_LINES if not prose else _char_cut(lines, s, hi)
                cut = max(s + 1, min(cut, hi - 1))
                if cut <= s or cut >= hi or hi - cut < MIN_PART_LINES:
                    break
            parts.append((s, cut))
            s = cut
        parts.append((s, hi))
        n = len(parts)
        for k, (a, b) in enumerate(parts):
            out.append((a, b, [k + 1, n] if n > 1 else None))
    return out


def _too_long(lines, lo, hi, prose):
    if prose:
        return sum(len(l) for l in lines[lo:hi]) > MAX_PROSE_CHARS and hi - lo > 1
    return hi - lo > MAX_CODE_LINES


def _char_cut(lines, lo, hi):
    total = 0
    for c in range(lo, hi):
        total += len(lines[c])
        if total > MAX_PROSE_CHARS:
            return max(lo + 1, c)
    return hi - 1


def _can_cut(lines, kinds, c, family):
    """Whether a part may start at line c."""
    if kinds[c - 1] == "blank" and kinds[c] != "blank":
        return True
    s = lines[c].lstrip()
    if family == "latex" and s.startswith("\\item"):
        return True
    if family == "markdown" and kinds[c] == "code" and re.match(r"([-*+]|\d+\.)\s", s):
        return kinds[c - 1] == "code" and re.match(r"\s*([-*+]|\d+\.)\s", lines[c - 1]) is not None
    if family == "json" and re.match(r" {0,4}\"", lines[c]):
        return True
    return False


def _title(lines, kinds, family):
    """A short label for a chunk: its first comment, heading or code line."""
    for line, kind in zip(lines, kinds):
        if kind in ("section", "comment") and not line.startswith("#!"):
            t = _strip_markers(line.strip())
            if t:
                return _short(t)
    for line, kind in zip(lines, kinds):
        if kind in ("def", "code"):
            t = line.strip()
            if t:
                return _short(t)
    return "(blank)"


def _strip_markers(s):
    s = re.sub(r"^(%%+|%\{|%|#+|//+|/\*+|\*+/?|<!--|-->)\s?", "", s)
    s = re.sub(r"(\*/|-->)\s*$", "", s)
    s = re.sub(r"^[-=]{3,}\s*", "", s)
    s = re.sub(r"^(\"\"\"|''')", "", s)
    s = re.sub(r"(\"\"\"|''')$", "", s)
    s = re.sub(r"^\\(sub)*section\*?\{(.*)\}\s*$", r"\2", s)
    s = re.sub(r"\\textcolor\{\w+\}\{(.*)\}", r"\1", s)
    return s.strip()


def _short(s, n=90):
    return s if len(s) <= n else s[:n - 1].rstrip() + "…"


# The code check: did a rewrite change more than the comments?


def code_tokens(text, family):
    """Return the code's tokens with comments and whitespace removed."""
    if family in PROSE_FAMILIES:
        return None
    return TOKEN_RE.findall(strip_comments(text, family))


def compare_code(original, rewrite, family):
    """Compare two versions of a chunk's code.

    Returns:
        A dict: changed (bool, None for prose), and for a change
        where (token index), was and now (a few tokens either side).
    """
    a = code_tokens(original, family)
    b = code_tokens(rewrite, family)
    if a is None:
        return {"changed": None}
    if a == b:
        return {"changed": False, "tokens": len(a)}
    i = 0
    while i < len(a) and i < len(b) and a[i] == b[i]:
        i += 1
    lo = max(0, i - 4)
    return {"changed": True, "where": i, "tokens": len(a),
            "was": " ".join(a[lo:i + 6]), "now": " ".join(b[lo:i + 6])}


def strip_comments(text, family):
    """Return the text without its comments (docstrings included)."""
    if family == "matlab":
        return _strip_matlab(text)
    if family == "python":
        return _strip_python(text)
    if family == "hash":
        return _strip_hash(text)
    if family == "slash":
        return _strip_slash(text)
    return text


def _strip_matlab(text):
    out = []
    block = 0
    for line in text.splitlines():
        s = line.strip()
        if s == "%{":
            block += 1
            continue
        if block:
            if s == "%}":
                block -= 1
            continue
        res = []
        i = 0
        n = len(line)
        in_str = None
        prev = ""
        while i < n:
            ch = line[i]
            if in_str:
                res.append(ch)
                if ch == in_str:
                    if in_str == "'" and i + 1 < n and line[i + 1] == "'":
                        res.append("'")
                        i += 2
                        continue
                    in_str = None
                i += 1
                continue
            if ch == '"':
                in_str = '"'
            elif ch == "'" and not (prev.isalnum() or prev in "_)]}'.\""):
                in_str = "'"
            elif ch == "%":
                break
            elif line.startswith("...", i):
                break
            res.append(ch)
            if not ch.isspace():
                prev = ch
            i += 1
        out.append("".join(res))
    return "\n".join(out)


def _strip_python(text):
    out = []
    i = 0
    n = len(text)
    at_stmt_start = True
    while i < n:
        ch = text[i]
        if ch == "#":
            j = text.find("\n", i)
            i = n if j < 0 else j
            continue
        if text.startswith(('"""', "'''"), i):
            d = text[i:i + 3]
            j = text.find(d, i + 3)
            j = n if j < 0 else j + 3
            if not at_stmt_start:
                out.append(text[i:j])
            i = j
            at_stmt_start = False
            continue
        if ch in "\"'":
            j = i + 1
            while j < n and text[j] != ch:
                if text[j] == "\\":
                    j += 1
                if text[j:j + 1] == "\n":
                    break
                j += 1
            out.append(text[i:j + 1])
            i = j + 1
            at_stmt_start = False
            continue
        out.append(ch)
        if ch == "\n":
            at_stmt_start = True
        elif not ch.isspace() and ch not in "rRbBuU":
            at_stmt_start = False
        elif ch in "rRbBuU" and at_stmt_start:
            # A prefix letter keeps the statement start only if quotes follow.
            if not text.startswith(('"""', "'''"), i + 1):
                at_stmt_start = False
        i += 1
    return "".join(out)


def _strip_hash(text):
    out = []
    for line in text.splitlines():
        res = []
        in_str = None
        i = 0
        while i < len(line):
            ch = line[i]
            if in_str:
                res.append(ch)
                if ch == "\\" and in_str == '"':
                    res.append(line[i + 1:i + 2])
                    i += 2
                    continue
                if ch == in_str:
                    in_str = None
            elif ch in "\"'":
                in_str = ch
                res.append(ch)
            elif ch == "#" and (i == 0 or line[i - 1].isspace() or line[i - 1] in "(;&|"):
                break
            else:
                res.append(ch)
            i += 1
        out.append("".join(res))
    return "\n".join(out)


def _strip_slash(text):
    out = []
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            i = n if j < 0 else j
            continue
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            i = n if j < 0 else j + 2
            continue
        if ch in "\"'`":
            j = i + 1
            while j < n and text[j] != ch:
                if text[j] == "\\":
                    j += 1
                elif ch != "`" and text[j] == "\n":
                    break
                j += 1
            out.append(text[i:j + 1])
            i = j + 1
            continue
        out.append(ch)
        i += 1
    return "".join(out)
