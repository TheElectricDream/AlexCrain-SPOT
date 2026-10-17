"""SPOT's constants, read from Constants.m (next to SPOT.m).

The lab console (the TX2) and the page have no MATLAB, so this module
reads the classdef that holds every parameter of SPOT
(Constants) and gives its values to the Python and, through the
state the server sends, to the page. Nothing in the console types a
parameter itself.

The reader understands what Constants.m promises to use: in a
properties block, lines of the form

    NAME = value;   % comment

where value is a number, a 'string', a row vector or matrix in
brackets, pi, an earlier constant as Constants.NAME, or
arithmetic on these with + - * / ^ and parentheses; a long value may
run on over lines with MATLAB's ... continuation. Anything else in the
file (the help text, the methods) is skipped.

    from .constants import C
    C.TABLE_X                     # a value, by its MATLAB name
    C.platform("MASS", "RED")     # MASS_RED
    C.as_dict()                   # everything, for the page

Python 3.6 (the lab console).
"""

import math
import re
from pathlib import Path

from .common import PROJECT_DIR

# Where Constants.m is: the project folder (two above +spot/+console)
# here; the copy deploy_tx2.sh makes for the Documentation tab on the TX2.
CONSTANTS_FILES = (PROJECT_DIR.parent.parent / "Constants.m",
                   PROJECT_DIR / "docs_source" / "Constants.m")

CLASS_PREFIX = "Constants."

# A constant's line: name, the value up to the ; and the trailing comment.
LINE_RE = re.compile(r"^\s*([A-Z][A-Z0-9_]*)\s*=\s*(.+?);\s*(?:%(.*))?$")

TOKEN_RE = re.compile(r"\s*(?:(\d+\.?\d*(?:[eE][-+]?\d+)?|\.\d+(?:[eE][-+]?\d+)?)"
                      r"|('(?:[^']|'')*')"
                      r"|(Constants\.[A-Za-z_]\w*|pi)"
                      r"|(.))")


class ConstantsError(Exception):
    """Constants.m can't be read, or a line isn't in the reader's grammar."""


# Parsing one value


class _Parser:
    """A recursive-descent parser for one value expression."""

    def __init__(self, text, values, line):
        """Tokenize text; values holds the constants read so far."""
        self.tokens = []
        self.values = values
        self.line = line
        for m in TOKEN_RE.finditer(text):
            number, string, name, other = m.groups()
            if number is not None:
                whole = re.fullmatch(r"\d+", number) is not None
                self.tokens.append(("num", int(number) if whole else float(number)))
            elif string is not None:
                self.tokens.append(("str", string[1:-1].replace("''", "'")))
            elif name is not None:
                self.tokens.append(("name", name))
            elif other is not None and not other.isspace():
                self.tokens.append(("op", other))
        self.pos = 0

    def peek(self):
        """Return the next token, or (None, None) at the end."""
        return self.tokens[self.pos] if self.pos < len(self.tokens) else (None, None)

    def take(self, kind=None, value=None):
        """Consume the next token (checking its kind or value)."""
        tok = self.peek()
        if tok[0] is None or (kind and tok[0] != kind) or (value is not None and tok[1] != value):
            raise ConstantsError("Constants.m line %d: unexpected %r" % (self.line, tok[1]))
        self.pos += 1
        return tok

    def parse(self):
        """Return the whole expression's value."""
        value = self.expr()
        if self.peek()[0] is not None:
            raise ConstantsError("Constants.m line %d: trailing %r" % (self.line, self.peek()[1]))
        return value

    def expr(self):
        """Sums and differences."""
        value = self.term()
        while self.peek() in (("op", "+"), ("op", "-")):
            op = self.take()[1]
            rhs = self.term()
            value = value + rhs if op == "+" else value - rhs
        return value

    def term(self):
        """Products and quotients."""
        value = self.power()
        while self.peek() in (("op", "*"), ("op", "/")):
            op = self.take()[1]
            rhs = self.power()
            value = value * rhs if op == "*" else value / rhs
        return value

    def power(self):
        """Exponents (right-associative)."""
        base = self.unary()
        if self.peek() == ("op", "^"):
            self.take()
            return base ** self.power()
        return base

    def unary(self):
        """A sign in front of a value."""
        if self.peek() == ("op", "-"):
            self.take()
            return -self.unary()
        if self.peek() == ("op", "+"):
            self.take()
            return self.unary()
        return self.atom()

    def atom(self):
        """A number, string, name, parenthesis or bracket."""
        kind, value = self.peek()
        if kind == "num":
            self.take()
            return value
        if kind == "str":
            self.take()
            return value
        if kind == "name":
            self.take()
            if value == "pi":
                return math.pi
            name = value[len(CLASS_PREFIX):]
            if name not in self.values:
                raise ConstantsError("Constants.m line %d: %s is not defined above"
                                     % (self.line, name))
            return self.values[name]
        if (kind, value) == ("op", "("):
            self.take()
            inner = self.expr()
            self.take("op", ")")
            return inner
        if (kind, value) == ("op", "["):
            return self.matrix()
        raise ConstantsError("Constants.m line %d: unexpected %r" % (self.line, value))

    def matrix(self):
        """A row vector [a b c] or [a, b, c], rows split by ;."""
        self.take("op", "[")
        rows, row = [], []
        while True:
            kind, value = self.peek()
            if (kind, value) == ("op", "]"):
                self.take()
                break
            if (kind, value) == ("op", ";"):
                self.take()
                rows.append(row)
                row = []
                continue
            if (kind, value) == ("op", ","):
                self.take()
                continue
            # In brackets a sign right before a number (no space after
            # it) belongs to the number: "[1 -1]" is two elements.
            row.append(self.element())
        rows.append(row)
        if len(rows) == 1:
            return rows[0]
        return rows

    def element(self):
        """One element of a matrix: a signed value, then products only."""
        value = self.unary()
        while self.peek() in (("op", "*"), ("op", "/"), ("op", "^")):
            op = self.take()[1]
            rhs = self.unary()
            value = value * rhs if op == "*" else value / rhs if op == "/" else value ** rhs
        return value


# The file


def split_comment(line):
    """Return (code, comment) of a MATLAB line: the first % outside quotes starts the comment."""
    quoted = False
    for k, ch in enumerate(line):
        if ch == "'":
            quoted = not quoted
        elif ch == "%" and not quoted:
            return line[:k], line[k + 1:]
    return line, ""


def read_constants(path):
    """Return {name: value} from a Constants.m file.

    Args:
        path: The classdef file.

    Returns:
        An ordered dict of the constants, in the file's order, with
        the comment of each under "_comments".

    Raises:
        ConstantsError: for a line the grammar doesn't cover.
    """
    values = {}
    comments = {}
    in_properties = False
    pending = ""      # a statement continued with ... over several lines
    first = 0
    with open(str(path), encoding="utf-8") as f:
        for number, raw in enumerate(f, 1):
            line = raw.rstrip("\n")
            stripped = line.strip()
            if not pending:
                if stripped.startswith("properties"):
                    in_properties = True
                    continue
                if in_properties and stripped == "end":
                    in_properties = False
                    continue
                if not in_properties or not stripped or stripped.startswith("%"):
                    continue
                first = number

            # MATLAB's continuation: ... in the code (not in a comment)
            # joins the next line; the rest of its line is a comment.
            code, _ = split_comment(stripped)
            if code.rstrip().endswith("..."):
                pending += code.rstrip()[:-3] + " "
                continue
            line = pending + stripped
            pending = ""
            m = LINE_RE.match(line)
            if not m:
                raise ConstantsError("Constants.m line %d: not 'NAME = value;'" % first)
            name, text, comment = m.groups()
            values[name] = _Parser(text, values, first).parse()
            comments[name] = (comment or "").strip()
    if pending:
        raise ConstantsError("Constants.m line %d: unfinished continuation" % first)
    return values, comments


def find_file():
    """Return the Constants.m to read (the first that exists)."""
    for path in CONSTANTS_FILES:
        if path.is_file():
            return path
    raise ConstantsError("Constants.m is missing: looked in %s"
                         % ", ".join(str(p) for p in CONSTANTS_FILES))


class Constants:
    """The constants, as attributes: C.TABLE_X, C.LED_IDS_RED, ..."""

    PLATFORMS = ("RED", "BLACK", "BLUE")

    def __init__(self, path=None):
        """Read the file (found if not given)."""
        self.path = Path(path) if path else find_file()
        self._values, self._comments = read_constants(self.path)

    def __getattr__(self, name):
        """A constant by its MATLAB name."""
        try:
            return self.__dict__["_values"][name]
        except KeyError:
            raise AttributeError("Constants has no %s" % name)

    def get(self, name, default=None):
        """A constant by name, or default."""
        return self._values.get(name, default)

    def platform(self, name, platform):
        """A per-platform constant: platform("MASS", "RED") is MASS_RED."""
        return self._values[name + "_" + platform]

    def per_platform(self, name):
        """{platform: NAME_<platform>} for RED, BLACK and BLUE."""
        return dict((p, self._values[name + "_" + p]) for p in self.PLATFORMS)

    def comment(self, name):
        """The trailing comment of a constant ('' if none)."""
        return self._comments.get(name, "")

    def as_dict(self):
        """Every constant, for the page (JSON-ready)."""
        return dict(self._values)

    def index(self, name):
        """A 1-based packet index as the 0-based index Python uses."""
        return int(self._values[name]) - 1

    def int(self, name):
        """A constant as an int (a port, a count, an id)."""
        return int(self._values[name])


C = Constants()


if __name__ == "__main__":
    # python3 -m spot_console.constants: every constant, as read.
    for key, value in C.as_dict().items():
        print("%-32s %r" % (key, value))
