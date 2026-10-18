# SPOT code template

How every MATLAB file in `+spot` is laid out, formatted and commented (§1-§7, §9), and how the
same principles apply to the console's Python and JavaScript (§8).
Reference implementation: `spot.arm.checkCommands` (`+arm/checkCommands.m`).

---

## 1. File layout

| # | Part | Notes |
|---|------|-------|
| 1 | `function` line | Descriptive input and output names. |
| 2 | H1 line | `%NAME One-line summary.` Must match the file's line in the package's `Contents.m`. |
| 3 | Help block | Inputs, Outputs, Behavior, Example (optional), See also (§5). |
| 4 | Blank line | Ends the text `help` prints. |
| 5 | `%#codegen` | Only if the file is used in generated code. |
| 6 | `%% Defaults` | `if nargin < k` blocks for optional inputs. |
| 7 | `%% Constants` | Program-internal constants with units; parameters come from `Constants` (§4). |
| 8 | `%% <Section>` | The body, as commented blocks (§3). |
| 9 | `end` | Closes the function. |
| 10 | Local functions | Same layout, after the main function. |

Class files (`classdef`, `matlab.System`) follow the same rules: help under `classdef`,
properties before methods, a units comment on every property.

---

## 2. Formatting

**Everything inside a function is indented one level** (four spaces, which the MATLAB
editor's Tab inserts), including the help block. Only `function` and its closing `end` sit at
the outer level. Nested blocks (`if`, `for`, `while`, `switch`, `try`) add one level each.

```matlab
function y = scale(x)
    %SCALE Twice the input.

    %% Result
    y = 2 * x;
end
```

**One statement per line.** A `;` or `,` that ends a statement ends the line. Not allowed:

```matlab
md = md(1:n); cmd = cmd(1:n, :);                 % two statements
function n = getNumInputsImpl(~), n = 1; end     % one-line function
if isempty(x), return; end                       % one-line if
```

**Line length** is at most 100 characters. Break after a comma, or before a binary operator
(`&`, `|`, `+`) with the operator starting the continuation line:

```matlab
pushedPastLimit = (anglePulses >= servo.maxPosition & torqueCmd(:, j) > 0) ...
                | (anglePulses <= servo.minPosition & torqueCmd(:, j) < 0);
```

Inside `[ ]` or `{ }` the operator goes at the **end** of the line instead. There, whitespace
separates elements, so `a ...` followed by `+b` would become two elements, `a` and `+b`.

**Long lists** (concatenations, `struct(...)` pairs, argument lists) put one item per line:

```matlab
torqueCmd = [dataClass.ARM_Shoulder_Command.Data(:), ...
             dataClass.ARM_Elbow_Command.Data(:), ...
             dataClass.ARM_Wrist_Command.Data(:)];
```

**Long messages** split the format string at word boundaries, one piece per line, with the
arguments on their own lines after it:

```matlab
msgs{end + 1} = sprintf( ...
    ['The arm''s %s torque of %.2f N m at %.1f s (%d steps) is beyond what its ' ...
     'servo''s Current Limit allows.'], ...
    JOINT_NAMES{j}, torqueCmd(k, j), sampleTime(k), nnz(overCurrent));
```

**Spacing.** One space around `=`, comparison and logical operators, and after commas. `2*pi`
and `180/pi` may stay tight. One blank line between blocks, never two.

---

## 3. Comments

**Sections** start with `%%` and a short title. They split a function into its main parts
(and give the MATLAB editor its code sections). In a long `switch`, a `%%` line at `case`
indentation can group related cases:

```matlab
%% Check 4: torque beyond the Current Limit
```

**Every block that does one thing gets a comment above it**: what it does, with units, and
the reason only when it isn't obvious. Keep it short and plain, one or two lines.

```matlab
% Torque is rounded to whole current units before clamping, so allow half a unit over.
overCurrent = ...
```

**Trailing comments** give units or a short fact on a constant or property:
`CURRENT_UNIT_A = 3.36e-3;   % [A] per current unit`.

Do not restate the code (`k = k + 1;   % increment k`), and do not leave commented-out code.
Write units in brackets: `[N m]`, `[rad/s]`, `[pulses]`.

---

## 4. Names and constants

**Variables** use descriptive camelCase (`torqueCmd`, `goalPulses`, `nSamples`). Short names
only for loop indices (`j` joint, `k` sample, `p` platform) and standard physics symbols
defined where first used (`q`, `tau`, `Kt`, `Fb`).

**Do not shadow MATLAB functions** (`path`, `mode`, `input`, `min`, `max`, `sum`, `size`,
`length`, `angle`, `error`, `table`, `line`, ...). Use `goal`, `ctrlMode`, etc.

**Constants.** No unexplained numeric literals in the body. Name them in UPPER_SNAKE_CASE
under `%% Constants` (or in `properties (Constant)` in a class), with units:

```matlab
CENTER_PULSE = 2048;            % [pulses] joint straight out
CMD_PULSES_PER_RAD = 651.74;    % [pulses/rad] goal conversion (spot.arm.PositionControl)
```

`0`, `1`, `2*pi`, `180/pi` and indices need no names. Mode codes do: `ctrlMode == MODE_POSITION`,
not `ctrlMode == 1`.

**Parameters live in `Constants`** (`Constants.m`), never typed in a function. A
*parameter* is a number that describes something outside the program: a physical constant, a lab
fact (the table's size, an address, a port), a hardware fact (a servo's units, the Teensy's
protocol), a nominal or placeholder value, a tuning value, a limit, or a default a user sees. Read
it as `Constants.NAME`:

```matlab
pull = Constants.MRAD_PULL * tilt;             % [m/s^2]
```

A local `%% Constants` section holds only *program-internal* values (timeouts, buffer sizes,
iteration caps, drawing sizes), and may alias a parameter for a shorter name:
`PWM_FULL = Constants.PWM_FULL;`. One property serves every language:

| Where | How it reads a parameter |
|---|---|
| MATLAB | `Constants.NAME` |
| The Simulink diagram | `Constants.NAME` in a block's parameter (a Constant, a Gain, a mask value) |
| Python, the local console | `from .constants import C`, then `C.NAME`: `constants.py` reads `jsonencode(Constants.all())` from the user's MATLAB session through the MATLAB Engine for Python (`SPOT` shares the session; `SPOT_MATLAB_ENGINE` names it), and again after the worker writes the file |
| Python, the lab console (TX2) | the same `C.NAME`: no MATLAB there, so `constants.py` parses the copy of `Constants.m` that `deploy_tx2.sh` leaves; `Constants.verify` checks that reader against the class |
| JavaScript (the page) | `constant('NAME')` or `constantPlatform('MASS', 'RED')` from `static/constants.js`, filled from the `state.constants` the server sends (and the `constants` event after a change) |
| Shell scripts | `grep` the line out of `Constants.m` |
| Firmware and C (`teensy_pwm_control.ino`, `csrc/`, `StreamData.cc`), `calibrate_arm.py` | can't read MATLAB: each keeps its copy, commented with the property it mirrors, and `Constants.verify` compares them with the file |

`Constants.m` keeps to the grammar the lab console's reader parses: `NAME = value;   % [unit]
comment` in a `properties (Constant)` block, where the value is a number, a `'string'`, `true` or
`false`, `NaN`, a row vector `[a b c]` (a matrix with `;`, `[]` for none), `pi`, an earlier
constant as `Constants.NAME`, or arithmetic on these (`+ - * / ^`, parentheses; a scalar times a
vector or matrix). No structs, cells or function calls. A per-platform value carries the
platform's suffix (`MASS_RED`).

**Settings are constants too.** A value a user changes on the Simulation tab, in the Designer or
by Update from Lab is a constant like any other (`USE_RED`, `SAMPLE_RATE`, `DOCK_RED`,
`LAB_THRUST_BLUE`): `spot.params.fromConstants` builds the settings struct from them, and the
MATLAB worker writes a change back with `spot.params.writeConstants` (the value between `NAME =`
and `;` is replaced; the comment stays). The worker is the only writer of the file; nothing else
in SPOT keeps a settings, design or calibration file. A value the lab measures lands in a `LAB_*`
constant; its placeholder (`THRUST_NOMINAL`, the `ARM_*` bench values) is what runs without one.

---

## 5. Help block

```matlab
function out = doThing(in1, in2)
    %DOTHING One-line summary (same as Contents.m).
    %   out = spot.pkg.doThing(in1, in2) what it is for and who calls it.
    %
    %   Inputs
    %     in1   meaning [units], shape
    %     in2   meaning [units], default if optional
    %
    %   Outputs
    %     out   meaning [units], shape, value when there is nothing to return
    %
    %   Behavior
    %     1. First step.
    %     2. Next step.
    %
    %   See also spot.pkg.other.
```

---

## 6. Function skeleton

```matlab
function out = doThing(in1, in2)
    %DOTHING ...help block (§5)...

    %% Defaults
    if nargin < 2
        in2 = 0;
    end

    %% Constants
    SOME_SCALE = 3.36e-3;    % [A] per unit

    %% <First part>
    % <What this block does.>
    a = ...;
    b = ...;

    %% <Second part>
    ...
end

function y = helper(x)
    %HELPER One-line summary.
    ...
end
```

---

## 7. Refactoring rules

1. **Same results, bit for bit.** Keep arithmetic in the same order when naming constants
   (`x * 9.549 * 4.366` becomes `x * RADPS_TO_RPM * RPM_TO_VEL_UNITS`, not a single merged
   constant).
2. **Same messages, character for character.** Re-split strings only at word boundaries.
3. **Same interface.** Function names, argument order and property names don't change.
4. **Leave `% ---- verbatim: <block>` ... `% ---- end verbatim` sections untouched**, including
   their indentation (`spot.control.lqrCommand`, `spot.control.thrusterDutyCycles`).
   `spot.control.verifyConsoleController` compares them as text with the diagram's blocks.
5. **Report bugs found while refactoring; don't fix them in the same change.**
6. **Generated code** (Symbolic Math Toolbox output in `spot.arm.inertiaMatrix` and
   `spot.arm.coriolisMatrix`) gets indentation and line wrapping only. It has no block
   comments, and its names stay as generated.
7. **Variables named inside strings keep their names.** `evalc('... sim(in) ...')` and
   `evalc('disp(x)')` refer to `in` and `x` by name, so renaming them breaks the call.
8. **`Contents.m` files** keep one line per function, so the 100-character limit doesn't
   apply to them. When an H1 line changes, update its `Contents.m` line to match.

---

## 8. Python and JavaScript (`+console`)

The same principles as the MATLAB code: one statement per line, a short comment over every block
that does one thing, a documented interface for every function, named constants with units, and
no names that shadow built-ins. Each language follows its own standard for the details: **PEP 8
and PEP 257** for Python, the **Google JavaScript Style Guide** for JavaScript. Third-party code
(`webapp/static/vendor/`) is left as published.

### 8.1 Python

| Rule | Python |
|------|--------|
| Indentation | 4 spaces |
| Line length | Code at most 99 characters; comments and docstrings at most 72 (PEP 8) |
| Statements | One per line: no `;`, no `if x: return` on one line |
| Line breaks | Inside brackets (wrap in parentheses if needed), never with a backslash. Break **before** a binary operator (`and`, `or`, `+`), which starts the continuation line (PEP 8). |
| Interface docs | A docstring for every module, class and function (PEP 257): a one-line summary ending in a period; then, if needed, a blank line, details, and `Args:`, `Returns:`, `Raises:` sections (Google docstring style) |
| Block comments | `# ` and full sentences, above the block, at the block's indentation |
| Inline comments | Sparingly, two spaces before `#`; mainly units: `RATE = 20.0  # [Hz]` |
| Sections | A one-line block comment naming the part, with the blank lines PEP 8 puts between top-level definitions. No ruler lines (`# ----`). |
| Names | `snake_case` functions and variables, `CapWords` classes, `UPPER_SNAKE` module constants. Don't shadow built-ins (`list`, `id`, `type`, `input`, `max`, `min`, `sum`, `format`, `filter`, `range`, ...). |
| Version | The lab console (TX2) runs Python 3.6: no syntax newer than 3.6 (no `:=`, no `f"{x=}"`, no positional-only parameters). |

```python
"""Small matrix helpers for the controller port.

Lists of rows; vectors are plain lists. Python 3.6 standard library only.
"""

import math

GRAVITY = 9.80665  # [m/s^2]


# Matrices

def zeros(rows, cols):
    """Return a rows x cols matrix of zeros."""
    return [[0.0] * cols for _ in range(rows)]


def solve(a, b):
    """Solve a x = b by Gaussian elimination with partial pivoting.

    Args:
        a: n x n matrix, as a list of rows.
        b: right-hand side, a list of n numbers.

    Returns:
        x, a list of n numbers.

    Raises:
        ValueError: if a is singular.
    """
    n = len(b)

    # Augmented matrix, so row swaps carry b along.
    m = [row[:] + [b[i]] for i, row in enumerate(a)]
    ...
```

### 8.2 JavaScript

| Rule | JavaScript |
|------|------------|
| Indentation | 2 spaces (+4 for a continuation line) |
| Line length | At most 100 characters. A template literal that spans lines is exempt: its line breaks and indentation are part of the string. A long one-line string or template is split into `+`-joined pieces. |
| Statements | One per line, each ending in `;`. Never break a line right after `return`, `throw`, `break` or `continue` (automatic semicolon insertion would end the statement there). |
| Line breaks | Break **after** a binary operator (`+`, `&&`, `\|\|`), which ends the line (Google style); a line breaks before `.` in a method chain. A chain of ternaries puts `?` and `:` at the start of each continuation line. |
| Braces | On every `if`, `else`, `for`, `while` and `do`, even around one statement. `} else {` on one line. |
| Interface docs | A JSDoc comment (`/** ... */`) on every top-level and exported function and class: a summary sentence, then `@param {type} name description` and `@return {type} description` |
| Block comments | `// ` and full sentences, above the block, at the block's indentation |
| Sections | A one-line `//` comment naming the part, after a blank line. No ruler lines (`// ----`). |
| Declarations | `const` by default, `let` when reassigned, never `var`; one variable per declaration |
| Names | `camelCase` functions and variables, `PascalCase` classes, `UPPER_SNAKE` module constants |
| Strings | Single quotes; template literals for interpolation and HTML |

```javascript
/**
 * Draws the table and the platforms for one replay frame.
 * @param {CanvasRenderingContext2D} ctx The canvas to draw on.
 * @param {!Object} frame Positions [m] and attitudes [rad] by platform.
 * @return {number} How many platforms were drawn.
 */
export function drawFrame(ctx, frame) {
  // Background: the table (Constants.TABLE_X by TABLE_Y, from S.constants).
  ctx.fillStyle = TABLE_COLOR;
  ctx.fillRect(0, 0, TABLE_X, TABLE_Y);

  // Each platform at its pose; skip ones not seen yet.
  let drawn = 0;
  for (const [name, pose] of Object.entries(frame)) {
    if (!pose) {
      continue;
    }
    drawPlatform(ctx, name, pose);
    drawn++;
  }
  return drawn;
}
```

### 8.3 Refactoring rules

The §7 rules apply here too. The checks for each language:
- **Python:** the syntax tree (comments and docstrings ignored) must be unchanged apart from the
  renames, and the file must parse as Python 3.6.
- **JavaScript:** the token stream (comments and layout ignored, braces added around one-statement
  bodies) must be unchanged apart from the renames.
- **No public renames:** don't rename exported functions, module-level names other modules
  import, object keys or JSON fields. The browser, the server and MATLAB exchange them by name.

---

## 9. Review checklist

- [ ] H1 line matches `Contents.m`.
- [ ] Help block has Inputs, Outputs, Behavior, See also.
- [ ] Function body (help included) indented one level; four spaces per level.
- [ ] One statement per line; no one-line `if`, `for` or `function`; lines at most 100 characters.
- [ ] Sections marked with `%%`; every block has a short comment.
- [ ] No unexplained numeric literals; constants have units.
- [ ] Parameters read from `Constants`, not typed; a local `%% Constants` holds program-internal
      values only; a firmware or script copy names its property and passes `Constants.verify`.
- [ ] No shadowed MATLAB functions.
- [ ] Verbatim sections untouched.
- [ ] Results and messages unchanged.
- [ ] Python: PEP 8 widths (99 code, 72 text), docstrings everywhere, Python 3.6 syntax;
      lines break before an operator, never with a backslash.
- [ ] JavaScript: braces on every control statement, JSDoc on top-level functions, 100 columns;
      lines break after an operator (ternaries: `?` and `:` lead).
