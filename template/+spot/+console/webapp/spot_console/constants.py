"""SPOT's constants: Constants (Constants.m, next to SPOT.m).

Every parameter of SPOT, and every choice a project makes, is a
constant property of the MATLAB class Constants. This module gives
them to the Python and, through the state the server sends, to the
page. Nothing in the console types a parameter itself.

Two ways to read them:

  - The local console (started by SPOT in MATLAB) reads the user's
    MATLAB session through the MATLAB Engine for Python: SPOT shares
    the session (matlab.engine.shareEngine) and names it in the
    environment variable SPOT_MATLAB_ENGINE; this module connects to it
    (matlab.engine.connect_matlab) and evaluates
    jsonencode(Constants.all()). C.reload() reads it again after the
    MATLAB worker has changed the file.
  - The lab console (the TX2) has no MATLAB: it parses the copy of
    Constants.m that deploy_tx2.sh leaves (docs_source/). The reader
    understands what Constants.m promises to use: in a properties
    block, lines of the form

        NAME = value;   % comment

    where value is a number, a 'string', true or false, NaN, a row
    vector or matrix in brackets ([] for none), pi, an earlier
    constant as Constants.NAME, or arithmetic on these with + - * / ^
    and parentheses (a scalar times a vector or matrix included); a
    long value may run on over lines with MATLAB's ... continuation.
    Anything else in the file (the help text, the methods) is skipped.
    Constants.verify in MATLAB checks this reader against the class.

    from .constants import C
    C.TABLE_X                     # a value, by its MATLAB name
    C.platform("MASS", "RED")     # MASS_RED
    C.as_dict()                   # everything, for the page

    python3 constants.py --json [Constants.m]   # the reader, as JSON

Python 3.6 (the lab console).
"""

import json
import math
import os
import re
import sys
from pathlib import Path

if __package__:
    from .common import PROJECT_DIR
else:   # run as a script (--json): the project folder is three above this file
    PROJECT_DIR = Path(__file__).resolve().parent.parent.parent

# Where Constants.m is: the project folder (two above +spot/+console)
# here; the copy deploy_tx2.sh makes for the Documentation tab on the TX2.
CONSTANTS_FILES = (PROJECT_DIR.parent.parent / "Constants.m",
                   PROJECT_DIR / "docs_source" / "Constants.m")

CLASS_PREFIX = "Constants."

# A constant's line: name, the value up to the ; and the trailing comment.
LINE_RE = re.compile(r"^\s*([A-Z][A-Z0-9_]*)\s*=\s*(.+?);\s*(?:%(.*))?$")

TOKEN_RE = re.compile(r"\s*(?:(\d+\.?\d*(?:[eE][-+]?\d+)?|\.\d+(?:[eE][-+]?\d+)?)"
                      r"|('(?:[^']|'')*')"
                      r"|(Constants\.[A-Za-z_]\w*|pi|true|false|NaN|Inf)"
                      r"|(.))")


class ConstantsError(Exception):
    """Constants.m can't be read, or a line isn't in the reader's grammar."""


# Parsing one value


def _arith(op, a, b):
    """Apply + - * / ^ to numbers, or a scalar and a vector/matrix elementwise.

    A vector/matrix on both sides is refused: the file promises not to
    need it (and MATLAB's * would be a matrix product).
    """
    scalar = lambda v: isinstance(v, (int, float, bool))
    if scalar(a) and scalar(b):
        if op == "+":
            return a + b
        if op == "-":
            return a - b
        if op == "*":
            return a * b
        if op == "/":
            return a / b
        return a ** b
    if isinstance(a, list) and isinstance(b, list):
        raise ConstantsError("Constants.m: arithmetic between two vectors isn't supported")
    if isinstance(a, list):
        return [_arith(op, x, b) for x in a]
    return [_arith(op, a, x) for x in b]


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
            value = _arith(op, value, self.term())
        return value

    def term(self):
        """Products and quotients."""
        value = self.power()
        while self.peek() in (("op", "*"), ("op", "/")):
            op = self.take()[1]
            value = _arith(op, value, self.power())
        return value

    def power(self):
        """Exponents (right-associative)."""
        base = self.unary()
        if self.peek() == ("op", "^"):
            self.take()
            return _arith("^", base, self.power())
        return base

    def unary(self):
        """A sign in front of a value."""
        if self.peek() == ("op", "-"):
            self.take()
            return _arith("-", 0, self.unary())
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
            if value in ("true", "false"):
                return value == "true"
            if value == "NaN":
                return float("nan")
            if value == "Inf":
                return float("inf")
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
            return rows[0]          # a row vector ([] when empty)
        return rows

    def element(self):
        """One element of a matrix: a signed value, then products only."""
        value = self.unary()
        while self.peek() in (("op", "*"), ("op", "/"), ("op", "^")):
            op = self.take()[1]
            value = _arith(op, value, self.unary())
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


def json_safe(value):
    """Return value with NaN and infinities as None (JSON has neither)."""
    if isinstance(value, float) and not math.isfinite(value):
        return None
    if isinstance(value, list):
        return [json_safe(v) for v in value]
    return value


class Constants:
    """The constants, as attributes: C.TABLE_X, C.LED_IDS_RED, ...

    Read from the shared MATLAB session named in SPOT_MATLAB_ENGINE
    (the local console), else from Constants.m (the lab console).
    """

    PLATFORMS = ("RED", "BLACK", "BLUE")
    ENGINE_VARIABLE = "SPOT_MATLAB_ENGINE"

    def __init__(self, path=None, engine=None):
        """Read the constants.

        Args:
            path: A Constants.m to parse (default: the one found).
            engine: The shared MATLAB session's name (default: the
                environment's SPOT_MATLAB_ENGINE; none: parse the file).
        """
        self.engine_name = engine or os.environ.get(self.ENGINE_VARIABLE) or None
        self._engine = None
        self.path = None
        self._values = {}
        if self.engine_name and not path:
            self.reload()
        else:
            self.path = Path(path) if path else find_file()
            self._values, _ = read_constants(self.path)
        self.source = self.engine_name and ("MATLAB (%s)" % self.engine_name) or str(self.path)

    def _connect(self):
        """Connect to the shared MATLAB session (once)."""
        if self._engine is not None:
            return self._engine
        try:
            import matlab.engine
        except ImportError:
            raise ConstantsError(
                "The local console reads Constants from MATLAB with the MATLAB Engine for "
                "Python, which this Python doesn't have. In MATLAB, run SPOT again (it offers "
                "to install it), or: python -m pip install \"<matlabroot>/extern/engines/python\"")
        try:
            self._engine = matlab.engine.connect_matlab(self.engine_name)
        except Exception as e:   # matlab.engine.EngineError and friends
            raise ConstantsError("Could not connect to the MATLAB session %r (%s). Start the "
                                 "console with SPOT in MATLAB." % (self.engine_name, e))
        return self._engine

    def reload(self):
        """Read the constants from MATLAB again (after the worker changed the file).

        MATLAB answers when it is idle, so call this between its
        commands, not during a simulation.
        """
        eng = self._connect()
        try:
            text = eng.eval("jsonencode(Constants.all())", nargout=1)
        except Exception as e:
            raise ConstantsError("MATLAB could not give its Constants (%s)." % e)
        self._values = json.loads(text, parse_constant=lambda c: float("nan"))
        # jsonencode writes NaN as null: keep NaN, as the file reader does.
        for name, value in self._values.items():
            self._values[name] = _nan_for_none(value)
        return self

    def evaluate(self, expression):
        """Evaluate a MATLAB expression in the shared session; its value (text).

        The local console only (the lab console has no MATLAB). Like
        reload(), call it while MATLAB is idle.

        Raises:
            ConstantsError: without a shared session, or if MATLAB fails.
        """
        if not self.engine_name:
            raise ConstantsError("No MATLAB session here (the lab console reads the file).")
        try:
            return self._connect().eval(expression, nargout=1)
        except ConstantsError:
            raise
        except Exception as e:
            raise ConstantsError("MATLAB could not evaluate %s (%s)." % (expression, e))

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

    def as_dict(self):
        """Every constant, for the page (JSON-ready: NaN becomes null)."""
        return dict((k, json_safe(v)) for k, v in self._values.items())

    def index(self, name):
        """A 1-based packet index as the 0-based index Python uses."""
        return int(self._values[name]) - 1

    def int(self, name):
        """A constant as an int (a port, a count, an id)."""
        return int(self._values[name])


def _nan_for_none(value):
    """Return a value read from MATLAB's JSON with null (NaN) as NaN."""
    if value is None:
        return float("nan")
    if isinstance(value, list):
        return [_nan_for_none(v) for v in value]
    return value


C = Constants()


if __name__ == "__main__":
    # python3 constants.py [Constants.m]: every constant, as read from the
    # file; --json prints them as one JSON object (Constants.verify in
    # MATLAB compares it with the class).
    args = [a for a in sys.argv[1:] if a != "--json"]
    values, _ = read_constants(args[0] if args else find_file())
    if "--json" in sys.argv:
        print(json.dumps(dict((k, json_safe(v)) for k, v in values.items())))
    else:
        for key, value in values.items():
            print("%-32s %r" % (key, value))
