# The Coding Standard

Every file in `+spot` follows one written standard, [`+spot/CODE_TEMPLATE.md`](file:CODE_TEMPLATE.md): how each MATLAB file is laid out, formatted and commented, and how the same principles apply to the console's Python and JavaScript. It is what makes this documentation possible. Every function has a help text or docstring, and a short comment over every block of code, so the Reference can explain each function block by block from the code itself.

## The principles

- **A documented interface for every function**: MATLAB's help block (an H1 line that matches the package's `Contents.m`, then Inputs, Outputs, Behavior, See also), a PEP 257 docstring in Python (Google style: Args, Returns, Raises), a JSDoc comment in JavaScript.
- **A short comment over every block that does one thing**: what it does, with units, and the reason only when it isn't obvious. Sections (`%%` in MATLAB, a one-line title comment in Python and JavaScript) split a function into its main parts; the Reference shows them as headings.
- **One statement per line**, at most 100 characters (Python: 99 for code, 72 for comments and docstrings).
- **Named constants with their units**, no unexplained numbers: `CURRENT_UNIT_A = 3.36e-3;   % [A] per current unit`.
- **No names that shadow built-in functions** (`path`, `input`, `max`, `table`, ... in MATLAB; `list`, `type`, `id`, ... in Python).
- **Python 3.6** for everything the lab console (TX2) runs.

## Refactoring rules

Changes for style must not change behaviour: the same results bit for bit, the same messages character for character, the same interfaces. The `% ---- verbatim:` sections of `spot.control.lqrCommand` and `spot.control.thrusterDutyCycles` are copies of the diagram's blocks and stay untouched: `spot.control.verifyConsoleController` compares them with the diagram as text. Bugs found while refactoring are reported, not fixed in the same change.

## Checking it

The Documentation tab's editor checks the standard as you type ([Using the Documentation Tab](page:tab-documentation)): the MATLAB rules (width, one statement per line, shadowed functions, the H1 line against `Contents.m`, changes inside a verbatim section), the Python rules (widths, docstrings, one statement per line, Python 3.6), and the JavaScript rules through ESLint (Google style: braces, `const`, semicolons, single quotes, operators at line ends, JSDoc). The full checklist is section 9 of `CODE_TEMPLATE.md`.
