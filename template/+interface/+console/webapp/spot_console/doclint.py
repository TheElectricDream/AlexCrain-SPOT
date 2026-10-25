"""Checks code edited on the Documentation tab (lint).

Each language gets its own checks, and the coding standard's rules
(+spot/CODE_TEMPLATE.md):

    Python       syntax; Python 3.6 (the lab console's version) syntax
                 and library; names (pyflakes when installed, else a
                 check of undefined, unused and shadowed names); PEP 8
                 widths, one statement per line, docstrings
    MATLAB       the Code Analyzer (through the MATLAB worker, when
                 MATLAB is connected and free); line width, one
                 statement per line, shadowed functions, help text,
                 Contents.m, the verbatim sections
    shell        bash -n, and ShellCheck when installed
    C, C++       the compiler's syntax check (local console, when g++
                 or gcc is installed)
    JSON, XML    whether they parse
    Markdown     links to pages, functions and images that don't exist

JavaScript is checked in the browser (ESLint), so the page can do it
as you type.

Every check returns diagnostics:
    {"line", "col", "end_line", "end_col", "severity", "message",
     "source", "code"}
with lines and columns from 1, end_col after the last character, and
severity "error", "warning" or "info".

Part of the SPOT Console (webapp/app.py starts it).
"""

import ast
import builtins
import io
import json
import os
import re
import shutil
import subprocess
import sys
import tokenize
import xml.etree.ElementTree as ElementTree
from pathlib import Path, PurePosixPath

from . import docparse
from .common import ApiError

MAX_LINT_BYTES = 1500000    # [bytes] larger texts aren't checked
TOOL_TIMEOUT = 20           # [s] for bash, ShellCheck and the compiler
MATLAB_TIMEOUT = 30         # [s] for the Code Analyzer

PY_CODE_WIDTH = 99          # [characters] PEP 8, as CODE_TEMPLATE.md
PY_TEXT_WIDTH = 72          # [characters] comments and docstrings
M_WIDTH = 100               # [characters] MATLAB lines
JS_WIDTH = 100              # [characters] (ESLint checks JavaScript)

# Names CODE_TEMPLATE.md says not to shadow.
PY_SHADOWED = ("list", "id", "type", "input", "max", "min", "sum", "format", "filter",
               "range", "dict", "str", "len", "open", "map", "next", "iter", "hash", "vars",
               "dir", "all", "any", "set", "bytes", "object", "file", "help", "round",
               "zip", "sorted", "print", "compile", "exec", "eval")
M_SHADOWED = ("path", "mode", "input", "min", "max", "sum", "size", "length", "angle",
              "error", "table", "line", "disp", "plot", "figure", "abs", "mean", "char",
              "double", "cell", "struct", "find", "sort", "diff", "title", "axis", "string",
              "class", "isempty", "numel", "zeros", "ones", "pi", "inv", "norm")

# Library features newer than Python 3.6, which the TX2 runs.
PY37_APIS = [
    (r"\bcapture_output\s*=", "subprocess's capture_output= is Python 3.7+; use "
     "stdout=subprocess.PIPE, stderr=subprocess.PIPE."),
    (r"\btext\s*=\s*True\b", "subprocess's text=True is Python 3.7+; use "
     "universal_newlines=True."),
    (r"^\s*(import|from)\s+dataclasses\b", "dataclasses is Python 3.7+."),
    (r"\basyncio\.run\(", "asyncio.run is Python 3.7+."),
    (r"\.removeprefix\(|\.removesuffix\(", "str.removeprefix/removesuffix are Python 3.9+."),
    (r"\bfromisoformat\(", "fromisoformat is Python 3.7+."),
    (r"\bnullcontext\b", "contextlib.nullcontext is Python 3.7+."),
    (r"\bmath\.(prod|isqrt|dist|comb|perm)\(", "This math function is Python 3.8+."),
    (r"\bcached_property\b", "functools.cached_property is Python 3.8+."),
    (r"^\s*(import|from)\s+(zoneinfo|graphlib|tomllib)\b", "This module is Python 3.9+."),
    (r"http\.server\b.*\bThreadingHTTPServer\b", "http.server.ThreadingHTTPServer is "
     "Python 3.7+."),
    (r"\bbreakpoint\(\)", "breakpoint() is Python 3.7+."),
]


def diag(line, col, message, severity="warning", source="", code="", end_line=None,
         end_col=None):
    """Return one diagnostic."""
    return {"line": max(1, int(line or 1)), "col": max(1, int(col or 1)),
            "end_line": int(end_line or line or 1),
            "end_col": int(end_col) if end_col else None,
            "severity": severity, "message": message, "source": source, "code": code}


def lint(path, lang, text, root=None, local=False, matlab=None, saved_text=None,
         known=None):
    """Check a file's text; return its diagnostics and what was checked.

    Args:
        path: The file's path (relative; for messages and rules).
        lang: Its language (docparse.language_of).
        text: The text to check.
        root: The project folder (Contents.m, include folders), or None.
        local: Whether this is the local console (runs compilers).
        matlab: The MATLAB bridge (local.matlab), or None.
        saved_text: The file as saved, to compare verbatim sections.
        known: {"pages": ids, "entries": quals, "images": paths} for
            Markdown links.

    Returns:
        {"diagnostics": [...], "checks": [names], "notes": [text]}.
    """
    if len(text.encode("utf-8")) > MAX_LINT_BYTES:
        return {"diagnostics": [], "checks": [], "notes": ["Too large to check."]}
    result = {"diagnostics": [], "checks": [], "notes": []}
    if lang == "python":
        lint_python(path, text, result)
    elif lang == "matlab":
        lint_matlab(path, text, result, root, matlab, saved_text)
    elif lang == "shell":
        lint_shell(text, result)
    elif lang in ("c", "cpp", "arduino"):
        lint_c(path, lang, text, result, root, local)
    elif lang == "json":
        lint_json(text, result)
    elif lang == "xml":
        lint_xml(text, result)
    elif lang == "markdown":
        lint_markdown(text, result, known or {})
    elif lang == "javascript":
        result["notes"].append("JavaScript is checked by ESLint in the page.")
    result["diagnostics"].sort(key=lambda d: (d["line"], d["col"]))
    return result


# Python


def lint_python(path, text, result):
    """Python: syntax, 3.6, names, and the coding standard."""
    result["checks"].append("Python syntax")
    try:
        tree = compile(text, path, "exec", ast.PyCF_ONLY_AST, dont_inherit=True)
    except SyntaxError as e:
        result["diagnostics"].append(diag(e.lineno, e.offset, "Syntax error: %s" % e.msg,
                                          "error", "python", "E999"))
        lint_python_lines(text, result, None)
        return
    except ValueError as e:
        result["diagnostics"].append(diag(1, 1, str(e), "error", "python"))
        return

    # Python 3.6, for the console's code (the lab console, the TX2,
    # runs it), where this Python can tell.
    if runs_on_tx2(path):
        lint_python36(text, result)
    else:
        result["notes"].append("Python 3.6 isn't required here (the TX2 doesn't run this "
                               "file).")

    # Names: pyflakes if it is installed, else our own check.
    try:
        from pyflakes import checker as pyflakes_checker
    except ImportError:
        pyflakes_checker = None
    if pyflakes_checker is not None:
        result["checks"].append("pyflakes")
        try:
            for message in pyflakes_checker.Checker(tree, path).messages:
                result["diagnostics"].append(diag(
                    message.lineno, getattr(message, "col", 0) + 1,
                    message.message % message.message_args,
                    "error" if "undefined name" in message.message else "warning",
                    "pyflakes", type(message).__name__))
        except Exception as e:   # pyflakes versions differ; don't fail the lint
            result["notes"].append("pyflakes failed: %s" % e)
    else:
        result["checks"].append("names")
        NameCheck(tree, result["diagnostics"]).run()

    # The coding standard.
    result["checks"].append("CODE_TEMPLATE.md (Python)")
    lint_python_tree(tree, result)
    lint_python_lines(text, result, tree)


def runs_on_tx2(path):
    """Return whether the lab console (TX2, Python 3.6) runs a file."""
    return "webapp/" in path or "calibration/" in path


def masked_python_lines(text):
    """Return Python's lines with strings and comments blanked."""
    lines = text.split("\n")
    for tok in docparse.python_tokens(text):
        if tok.type not in (tokenize.STRING, tokenize.COMMENT):
            continue
        for row in range(tok.start[0], tok.end[0] + 1):
            line = lines[row - 1]
            first = tok.start[1] if row == tok.start[0] else 0
            last = tok.end[1] if row == tok.end[0] else len(line)
            lines[row - 1] = line[:first] + " " * (last - first) + line[last:]
    return lines


def lint_python36(text, result):
    """Syntax and library features newer than Python 3.6."""
    if sys.version_info >= (3, 8):
        result["checks"].append("Python 3.6 syntax")
        for version in ((3, 6), (3, 7)):
            try:
                ast.parse(text, feature_version=version)
                break
            except ValueError:
                continue
            except SyntaxError as e:
                result["diagnostics"].append(diag(
                    e.lineno, e.offset, "Not valid in Python %d.%d, which the lab console "
                    "(TX2) runs: %s" % (version + (e.msg,)), "warning", "python3.6", "PY36"))
                break
    result["checks"].append("Python 3.6 library")
    for number, code in enumerate(masked_python_lines(text), 1):
        for pattern, message in PY37_APIS:
            match = re.search(pattern, code)
            if match:
                result["diagnostics"].append(diag(number, match.start() + 1, message,
                                                  "warning", "python3.6", "PY36",
                                                  end_col=match.end() + 1))
    for tok in docparse.python_tokens(text):
        if tok.type != tokenize.STRING:
            continue
        prefix = re.match(r"^[A-Za-z]*", tok.string).group().lower()
        if "f" in prefix and re.search(r"\{[^{}]*[^=!<>]=(![rsa])?(:[^{}]*)?\}", tok.string):
            result["diagnostics"].append(diag(tok.start[0], tok.start[1] + 1,
                                              "f\"{x=}\" is Python 3.8+.", "warning",
                                              "python3.6", "PY36"))


def lint_python_tree(tree, result):
    """Docstrings, one-line compound statements, shadowed built-ins."""
    out = result["diagnostics"]
    if ast.get_docstring(tree) is None:
        out.append(diag(1, 1, "The module has no docstring (PEP 257).", "info", "style",
                        "D100"))
    methods = set()
    for node in ast.walk(tree):
        if isinstance(node, ast.ClassDef):
            methods.update(id(n) for n in node.body
                           if isinstance(n, (ast.FunctionDef, ast.AsyncFunctionDef)))
    for node in ast.walk(tree):
        if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
            if ast.get_docstring(node) is None:
                what = "class" if isinstance(node, ast.ClassDef) else "function"
                out.append(diag(node.lineno, node.col_offset + 1,
                                "The %s %s has no docstring (PEP 257)." % (what, node.name),
                                "info", "style", "D101"))
            if not isinstance(node, ast.ClassDef):
                args = node.args
                names = [a.arg for a in args.args + args.kwonlyargs
                         + getattr(args, "posonlyargs", [])]
                names += [a.arg for a in (args.vararg, args.kwarg) if a is not None]
                for name in names:
                    if name in PY_SHADOWED:
                        out.append(diag(node.lineno, node.col_offset + 1,
                                        "The argument %s shadows the built-in %s." % (name,
                                                                                        name),
                                        "warning", "style", "A002"))
            if node.name in PY_SHADOWED and id(node) not in methods:
                out.append(diag(node.lineno, node.col_offset + 1,
                                "%s shadows the built-in %s." % (node.name, node.name),
                                "warning", "style", "A001"))

        # One statement per line: no "if x: y".
        if isinstance(node, (ast.If, ast.For, ast.While, ast.With, ast.FunctionDef,
                             ast.AsyncFunctionDef, ast.Try)):
            body = getattr(node, "body", [])
            if body and body[0].lineno == node.lineno:
                out.append(diag(node.lineno, node.col_offset + 1,
                                "Put the body on its own line (one statement per line).",
                                "warning", "style", "E701"))
        if (isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store)
                and node.id in PY_SHADOWED):
            out.append(diag(node.lineno, node.col_offset + 1,
                            "%s shadows the built-in %s." % (node.id, node.id), "warning",
                            "style", "A001"))


def lint_python_lines(text, result, tree):
    """Widths, tabs, trailing spaces, semicolons and backslashes."""
    out = result["diagnostics"]
    lines = text.split("\n")
    tokens = docparse.python_tokens(text)
    comment_rows = {t.start[0] for t in tokens if t.type == tokenize.COMMENT}
    code_rows = set()
    for tok in tokens:
        if tok.type not in docparse.PY_TOKEN_SKIP:
            code_rows.update(range(tok.start[0], tok.end[0] + 1))
    doc_rows = set()
    if tree is not None:
        for node in ast.walk(tree):
            definition = (ast.Module, ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)
            if (isinstance(node, definition) and node.body
                    and isinstance(node.body[0], ast.Expr)
                    and docparse.py_string(node.body[0].value) is not None):
                doc = docparse.py_string(node.body[0].value)
                end = getattr(node.body[0], "end_lineno", None) or node.body[0].lineno
                doc_rows.update(range(end - doc.count("\n"), end + 1))

    for number, line in enumerate(lines, 1):
        width = len(line)
        only_text = number in doc_rows or (number in comment_rows and number not in code_rows)
        limit = PY_TEXT_WIDTH if only_text else PY_CODE_WIDTH
        if width > limit:
            what = "comment or docstring" if only_text else "code"
            out.append(diag(number, limit + 1, "Line is %d characters; %s is at most %d "
                            "(PEP 8)." % (width, what, limit),
                            "info" if only_text else "warning", "style", "E501",
                            end_col=width + 1))
        if "\t" in line[:len(line) - len(line.lstrip())]:
            out.append(diag(number, 1, "Indent with spaces, not tabs.", "warning", "style",
                            "W191"))
        if line.rstrip() != line and line.strip():
            out.append(diag(number, len(line.rstrip()) + 1, "Trailing whitespace.", "info",
                            "style", "W291", end_col=width + 1))
    depth = 0
    for tok in tokens:
        if tok.type == tokenize.OP and tok.string in "([{":
            depth += 1
        elif tok.type == tokenize.OP and tok.string in ")]}":
            depth -= 1
        elif tok.type == tokenize.OP and tok.string == ";" and depth == 0:
            out.append(diag(tok.start[0], tok.start[1] + 1, "One statement per line (no ;).",
                            "warning", "style", "E702"))
    for number, line in enumerate(lines, 1):
        stripped = line.rstrip()
        if stripped.endswith("\\") and number in code_rows and not stripped.endswith("\\\\"):
            if not _in_string(tokens, number):
                out.append(diag(number, len(stripped), "Break the line inside brackets, not "
                                "with a backslash (CODE_TEMPLATE.md).", "info", "style",
                                "W605"))


def _in_string(tokens, row):
    """Return whether a line ends inside a string spanning lines."""
    for tok in tokens:
        if tok.type == tokenize.STRING and tok.start[0] <= row < tok.end[0]:
            return True
    return False


class NameCheck:
    """Undefined and unused names, when pyflakes isn't installed.

    A simple model of Python's scopes: the module, each function (with
    its arguments), class bodies (not visible inside their methods)
    and comprehensions.
    """

    BUILTINS = set(dir(builtins)) | {"__file__", "__name__", "__doc__", "__builtins__",
                                     "__spec__", "__loader__", "__package__", "__path__",
                                     "WindowsError", "__debug__"}

    def __init__(self, tree, out):
        """Keep the tree and where to add diagnostics."""
        self.tree = tree
        self.out = out
        self.star = any(isinstance(n, ast.ImportFrom) and any(a.name == "*" for a in n.names)
                        for n in ast.walk(tree))

    def run(self):
        """Check the module and every scope in it."""
        module_names = self._bound(self.tree)
        self._check_scope(self.tree, [module_names], module=True)

    def _bound(self, node):
        """Return {name: line} of the names a scope binds."""
        names = {}
        for child in self._own(node):
            if isinstance(child, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
                names.setdefault(child.name, child.lineno)
            elif isinstance(child, ast.Name) and isinstance(child.ctx, (ast.Store, ast.Del)):
                names.setdefault(child.id, child.lineno)
            elif isinstance(child, (ast.Import, ast.ImportFrom)):
                for alias in child.names:
                    name = alias.asname or alias.name.split(".")[0]
                    names.setdefault(name, child.lineno)
            elif isinstance(child, ast.ExceptHandler) and child.name:
                names.setdefault(child.name, child.lineno)
            elif isinstance(child, (ast.Global, ast.Nonlocal)):
                for name in child.names:
                    names.setdefault(name, child.lineno)
        if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.Lambda)):
            args = node.args
            for arg in args.args + args.kwonlyargs + getattr(args, "posonlyargs", []):
                names.setdefault(arg.arg, node.lineno)
            for arg in (args.vararg, args.kwarg):
                if arg is not None:
                    names.setdefault(arg.arg, node.lineno)
        return names

    def _own(self, node):
        """Yield the nodes of a scope, not entering nested scopes."""
        stack = list(ast.iter_child_nodes(node))
        if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.Lambda)):
            stack = list(node.body) if isinstance(node.body, list) else [node.body]
        while stack:
            child = stack.pop()
            yield child
            if isinstance(child, (ast.FunctionDef, ast.AsyncFunctionDef)):
                stack.extend(child.decorator_list)
                stack.extend(child.args.defaults + [d for d in child.args.kw_defaults if d])
                continue
            if isinstance(child, ast.ClassDef):
                stack.extend(child.decorator_list + child.bases)
                continue
            if isinstance(child, (ast.Lambda, ast.ListComp, ast.SetComp, ast.DictComp,
                                  ast.GeneratorExp)):
                continue
            stack.extend(ast.iter_child_nodes(child))

    def _check_scope(self, node, scopes, module=False, in_class=False):
        """Check a scope's names, then the scopes inside it."""
        mine = scopes[-1]
        used = set()
        for child in self._own(node):
            if isinstance(child, ast.Name) and isinstance(child.ctx, ast.Load):
                used.add(child.id)
                visible = any(child.id in s for s in scopes) or child.id in self.BUILTINS
                if not visible and not self.star:
                    self.out.append(diag(child.lineno, child.col_offset + 1,
                                         "Undefined name %s." % child.id, "error", "names",
                                         "F821"))
            elif isinstance(child, ast.Attribute):
                pass
            elif isinstance(child, (ast.FunctionDef, ast.AsyncFunctionDef, ast.Lambda)):
                inner = self._bound(child)
                outer = [s for s in scopes if "<class>" not in s]
                self._check_scope(child, outer + [inner])
            elif isinstance(child, ast.ClassDef):
                inner = dict(self._bound(child), **{"<class>": 0})
                self._check_scope(child, scopes + [inner], in_class=True)
            elif isinstance(child, (ast.ListComp, ast.SetComp, ast.DictComp, ast.GeneratorExp)):
                inner = {}
                for gen in child.generators:
                    for name in ast.walk(gen.target):
                        if isinstance(name, ast.Name):
                            inner[name.id] = child.lineno
                self._check_comprehension(child, scopes + [inner])
            elif isinstance(child, (ast.Global, ast.Nonlocal)):
                used.update(child.names)
        # Names a module exports by __all__ count as used.
        if module:
            for child in ast.walk(node):
                if isinstance(child, ast.Name) and isinstance(child.ctx, ast.Load):
                    used.add(child.id)
                elif isinstance(child, ast.Attribute):
                    used.add(getattr(child.value, "id", ""))

        # Imports never used; locals assigned and never read.
        for child in self._own(node):
            if isinstance(child, (ast.Import, ast.ImportFrom)) and not in_class:
                for alias in child.names:
                    name = alias.asname or alias.name.split(".")[0]
                    if name != "*" and name not in used and not module:
                        self.out.append(diag(child.lineno, child.col_offset + 1,
                                             "%s is imported but not used." % name,
                                             "warning", "names", "F401"))
                    elif (name != "*" and module and name not in used
                            and not self._exported(name)):
                        self.out.append(diag(child.lineno, child.col_offset + 1,
                                             "%s is imported but not used." % name,
                                             "warning", "names", "F401"))
        if not module and not in_class:
            assigned = {}
            for child in self._own(node):
                if (isinstance(child, ast.Assign) and len(child.targets) == 1
                        and isinstance(child.targets[0], ast.Name)):
                    assigned.setdefault(child.targets[0].id, child)
            for name, child in assigned.items():
                if name not in used and not name.startswith("_") and name in mine:
                    self.out.append(diag(child.lineno, child.col_offset + 1,
                                         "%s is assigned but never used." % name, "warning",
                                         "names", "F841"))

    def _check_comprehension(self, node, scopes):
        """Check the names used in a comprehension."""
        for child in ast.walk(node):
            if isinstance(child, ast.Name) and isinstance(child.ctx, ast.Load):
                visible = any(child.id in s for s in scopes) or child.id in self.BUILTINS
                if not visible and not self.star:
                    self.out.append(diag(child.lineno, child.col_offset + 1,
                                         "Undefined name %s." % child.id, "error", "names",
                                         "F821"))

    def _exported(self, name):
        """Return whether __all__ lists a name (a re-export)."""
        for child in self.tree.body:
            if isinstance(child, ast.Assign) and any(
                    isinstance(t, ast.Name) and t.id == "__all__" for t in child.targets):
                return name in [docparse.py_string(e) for e in
                                getattr(child.value, "elts", [])]
        return False


# MATLAB


def lint_matlab(path, text, result, root, matlab, saved_text):
    """MATLAB: the Code Analyzer, and the coding standard's rules."""
    out = result["diagnostics"]
    name = PurePosixPath(path).name
    lines = text.split("\n")

    # The Code Analyzer, in the user's MATLAB (the local console's
    # worker), unless it is busy running something.
    if matlab is not None and not path.endswith("Contents.m") and ".slx/" not in path:
        status = matlab.status()
        if not status["connected"]:
            result["notes"].append("MATLAB isn't connected, so the Code Analyzer didn't run "
                                   "(only the coding standard's rules).")
        elif status["busy"]:
            result["notes"].append("MATLAB is busy, so the Code Analyzer didn't run this "
                                   "time.")
        else:
            try:
                issues = matlab.request("code.lint", {"name": name, "text": text},
                                        timeout=MATLAB_TIMEOUT, quiet=True)
                result["checks"].append("MATLAB Code Analyzer")
                if isinstance(issues, dict):
                    issues = [issues]   # one message, sent as an object
                for issue in issues or []:
                    out.append(diag(issue.get("line"), issue.get("col"), issue.get("message"),
                                    issue.get("severity") or "warning", "MATLAB",
                                    issue.get("id") or "", issue.get("endLine"),
                                    issue["endCol"] + 1 if issue.get("endCol") else None))
            except ApiError as e:
                result["notes"].append("The Code Analyzer didn't run: %s" % e)
    elif matlab is None and ".slx/" not in path:
        result["notes"].append("The MATLAB Code Analyzer runs on the local console (with "
                               "MATLAB connected).")

    # The coding standard.
    result["checks"].append("CODE_TEMPLATE.md (MATLAB)")
    contents = name == "Contents.m"
    depth = 0   # brackets open from earlier lines (a list over several lines)
    for number, line in enumerate(lines, 1):
        code, comment = docparse.matlab_split(line)
        if len(line) > M_WIDTH and not contents:
            out.append(diag(number, M_WIDTH + 1, "Line is %d characters; at most %d."
                            % (len(line), M_WIDTH), "warning", "style", "LEN",
                            end_col=len(line) + 1))
        if "\t" in line:
            out.append(diag(number, line.index("\t") + 1, "Indent with spaces, not tabs.",
                            "warning", "style", "TAB"))
        if line.rstrip() != line and line.strip():
            out.append(diag(number, len(line.rstrip()) + 1, "Trailing whitespace.", "info",
                            "style", "WS", end_col=len(line) + 1))
        stripped = code.strip()
        if not stripped:
            continue

        # One statement per line: a ; or , that ends a statement and
        # more code after it.
        reported = False
        for k, c in enumerate(code):
            if c in "([{":
                depth += 1
            elif c in ")]}":
                depth = max(0, depth - 1)
            elif c in ";," and depth == 0 and code[k + 1:].strip() and not reported:
                out.append(diag(number, k + 1, "One statement per line (CODE_TEMPLATE.md "
                                "section 2).", "warning", "style", "ONE"))
                reported = True

        # Variables named like MATLAB functions.
        match = re.match(r"^\s*([A-Za-z]\w*)\s*=(?!=)", code)
        if match and match.group(1) in M_SHADOWED:
            out.append(diag(number, match.start(1) + 1, "%s shadows the MATLAB function %s "
                            "(CODE_TEMPLATE.md section 4)." % (match.group(1),
                                                                 match.group(1)),
                            "warning", "style", "SHADOW", end_col=match.end(1) + 1))

    _matlab_help_checks(path, lines, result, root)
    if saved_text is not None:
        _verbatim_check(lines, saved_text.split("\n"), result)


def _matlab_help_checks(path, lines, result, root):
    """The H1 line, and that it matches the package's Contents.m."""
    out = result["diagnostics"]
    name = PurePosixPath(path).stem
    first = next((k for k, ln in enumerate(lines) if ln.strip()), None)
    if first is None or not re.match(r"^\s*(function|classdef)\b", lines[first]):
        return
    h1 = lines[first + 1].strip() if first + 1 < len(lines) else ""
    while first + 1 < len(lines) and docparse.matlab_continues(lines[first]):
        first += 1
        h1 = lines[first + 1].strip() if first + 1 < len(lines) else ""
    match = re.match(r"^%\s*([A-Za-z]\w*)\s+(.*)$", h1)
    if not match or match.group(1).upper() != name.upper():
        out.append(diag(first + 2, 1, "The help should start with an H1 line: %%%s and a "
                        "one-line summary (CODE_TEMPLATE.md section 5)." % name.upper(),
                        "info", "style", "H1"))
        return
    if root is None or ".slx/" in path:
        return
    contents = Path(root) / PurePosixPath(path).parent / "Contents.m"
    try:
        listing = contents.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return
    entry = re.search(r"^%\s+" + re.escape(name) + r"\s+-\s+(.*)$", listing, re.M)
    if entry and entry.group(1).strip() != match.group(2).strip():
        out.append(diag(first + 2, 1, "The H1 line differs from this function's line in "
                        "Contents.m (\"%s\"); keep them the same." % entry.group(1).strip(),
                        "info", "style", "CONTENTS"))


def _verbatim_blocks(lines):
    """Return {name: [lines]} of a file's verbatim sections."""
    blocks = {}
    current = None
    for line in lines:
        start = re.match(r"^\s*% ---- verbatim:\s*(.+?)\s*$", line)
        if start:
            current = start.group(1)
            blocks[current] = []
            continue
        if re.match(r"^\s*% ---- end verbatim", line):
            current = None
            continue
        if current is not None:
            blocks[current].append(line)
    return blocks


def _verbatim_check(lines, saved, result):
    """Warn about a change inside a verbatim section."""
    new_blocks = _verbatim_blocks(lines)
    old_blocks = _verbatim_blocks(saved)
    for name, old in old_blocks.items():
        if new_blocks.get(name) != old:
            row = next((k + 1 for k, ln in enumerate(lines)
                        if re.match(r"^\s*% ---- verbatim:\s*" + re.escape(name), ln)), 1)
            result["diagnostics"].append(diag(
                row, 1, "The verbatim section \"%s\" has changed. It must match the diagram's "
                "block exactly: spot.control.verifyConsoleController compares them, and "
                "Interactive Mode runs a Python port of it (CODE_TEMPLATE.md section 7)."
                % name, "warning", "verbatim", "VERBATIM"))


# Shell, C and C++, data files


def run_tool(cmd, text, cwd=None):
    """Run a checker on the text; return (code, stdout, stderr)."""
    try:
        proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, cwd=cwd)
        out, err = proc.communicate(text.encode("utf-8"), timeout=TOOL_TIMEOUT)
    except subprocess.TimeoutExpired:
        proc.kill()
        return None, "", "timed out"
    except OSError as e:
        return None, "", str(e)
    return (proc.returncode, out.decode("utf-8", errors="replace"),
            err.decode("utf-8", errors="replace"))


def lint_shell(text, result):
    """Shell scripts: bash -n, then ShellCheck if it is installed."""
    out = result["diagnostics"]
    if shutil.which("bash"):
        result["checks"].append("bash -n")
        code, _, err = run_tool(["bash", "-n"], text)
        for match in re.finditer(r"line (\d+): (.*)", err):
            out.append(diag(int(match.group(1)), 1, match.group(2), "error", "bash"))
    else:
        result["notes"].append("bash isn't installed here, so the syntax wasn't checked.")
    if shutil.which("shellcheck"):
        result["checks"].append("ShellCheck")
        _, stdout, _ = run_tool(["shellcheck", "-f", "json", "-s", "bash", "-"], text)
        try:
            items = json.loads(stdout or "[]")
        except ValueError:
            items = []
        levels = {"error": "error", "warning": "warning"}
        for item in items:
            out.append(diag(item.get("line"), item.get("column"), item.get("message"),
                            levels.get(item.get("level"), "info"), "ShellCheck",
                            "SC%s" % item.get("code"), item.get("endLine"),
                            item.get("endColumn")))
    else:
        result["notes"].append("ShellCheck isn't installed (only the syntax was checked).")


def lint_c(path, lang, text, result, root, local):
    """C and C++: the compiler's syntax check, on the local console."""
    if lang == "arduino":
        result["notes"].append("Arduino sketches need the Teensy's libraries to compile: "
                               "build it with the Arduino IDE (Teensyduino).")
        return
    if not local:
        result["notes"].append("The compiler check runs on the local console only.")
        return
    compiler = shutil.which("g++" if lang == "cpp" else "gcc") or shutil.which("clang")
    if not compiler:
        result["notes"].append("No C/C++ compiler is installed here, so the code wasn't "
                               "compiled.")
        return
    folder = Path(root) / PurePosixPath(path).parent if root else None
    includes = []
    if folder is not None and folder.is_dir():
        includes = [folder, folder / "include", folder.parent / "include"]
        includes += [p for p in folder.iterdir() if p.is_dir()]
    cmd = [compiler, "-fsyntax-only", "-Wall", "-Wextra", "-x",
           "c++" if lang == "cpp" else "c", "-std=c++17" if lang == "cpp" else "-std=gnu11"]
    cmd += ["-I" + str(p) for p in includes if p.is_dir()]
    cmd.append("-")
    result["checks"].append(os.path.basename(compiler) + " -fsyntax-only")
    _, _, err = run_tool(cmd, text, cwd=str(folder) if folder and folder.is_dir() else None)
    for match in re.finditer(r"^<stdin>:(\d+):(\d+): (fatal error|error|warning|note): (.*)$",
                             err, re.M):
        severity = {"fatal error": "error", "note": "info"}.get(match.group(3),
                                                                 match.group(3))
        out_text = match.group(4)
        result["diagnostics"].append(diag(int(match.group(1)), int(match.group(2)), out_text,
                                          severity, os.path.basename(compiler)))


def lint_json(text, result):
    """JSON: whether it parses."""
    result["checks"].append("JSON")
    try:
        json.loads(text)
    except ValueError as e:
        match = re.search(r"line (\d+) column (\d+)", str(e))
        line, col = (int(match.group(1)), int(match.group(2))) if match else (1, 1)
        result["diagnostics"].append(diag(line, col, str(e).split(":")[0], "error", "json"))


def lint_xml(text, result):
    """XML: whether it parses."""
    result["checks"].append("XML")
    try:
        ElementTree.fromstring(text)
    except ElementTree.ParseError as e:
        line, col = getattr(e, "position", (1, 0))
        result["diagnostics"].append(diag(line, col + 1, str(e), "error", "xml"))


def lint_markdown(text, result, known):
    """Documentation pages: links to pages, functions and images."""
    result["checks"].append("documentation links")
    pages = set(known.get("pages", ())) | {"routes", "az"}
    entries = set(known.get("entries", ()))
    images = set(known.get("images", ()))
    fenced = False
    for number, line in enumerate(text.split("\n"), 1):
        # Examples in code blocks and `code` aren't links.
        if line.lstrip().startswith("```"):
            fenced = not fenced
            continue
        if fenced:
            continue
        line = re.sub(r"`[^`]*`", lambda m: " " * len(m.group()), line)
        for match in re.finditer(r"\]\((page|ref|file):([^)\s#]+)(#[^)\s]*)?\)", line):
            kind, target = match.group(1), match.group(2)
            missing = ((kind == "page" and target not in pages)
                       or (kind == "ref" and entries and target not in entries))
            if missing:
                result["diagnostics"].append(diag(
                    number, match.start() + 2, "No %s named %s." % (
                        {"page": "page", "ref": "function"}[kind], target), "warning", "links",
                    end_col=match.end() + 1))
        for match in re.finditer(r"!\[[^\]]*\]\(([^)\s]+)\)", line):
            target = match.group(1)
            if (not re.match(r"^(https?:|data:)", target) and images
                    and target.lstrip("./") not in images):
                result["diagnostics"].append(diag(number, match.start() + 1,
                                                  "No image %s in docs/." % target,
                                                  "warning", "links",
                                                  end_col=match.end() + 1))


def known_targets(index):
    """Return the pages, functions and images Markdown may link to."""
    data = json.loads(index) if isinstance(index, (bytes, str)) else index
    from .docs import DOCS_DIR
    images = []
    folder = DOCS_DIR / "images"
    if folder.is_dir():
        images = ["images/" + p.relative_to(folder).as_posix() for p in folder.rglob("*")
                  if p.is_file()]
    return {"pages": list(data["guides"]["pages"]),
            "entries": [e["qual"] for e in data["entries"].values()]
            + [e["id"] for e in data["entries"].values()],
            "images": images}


def read_saved(path):
    """Return the saved text of a file, or None."""
    try:
        return io.open(str(path), encoding="utf-8", errors="replace").read().replace("\r\n",
                                                                                        "\n")
    except OSError:
        return None
