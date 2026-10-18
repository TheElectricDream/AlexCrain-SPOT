# Constants: Every Parameter in One Place

Every number that describes SPOT rather than its programs lives in one file, [`Constants.m`](file:Constants.m): the table and gravity, the lab's addresses and ports, the PhaseSpace packet, the platforms' bodies and measured mass properties, the thrusters and the Teensy, the mixer, the rates and phases, the poses, the LQR weights, the Dynamixel servos, the arm's bench calibration, geometry and controllers, the calibrations' limits and defaults, Interactive Mode, the experiment's streams. So does every choice a project makes: the Simulation tab's settings (`USE_RED`, `SAMPLE_RATE`, `INITIAL_POSE_RED`, …), the Spacecraft Designer's design (`DOCK_RED`, `SOLAR_RED`, …), the LEDs' positions (`LED_XY_RED`) and the lab calibration the project took (`LAB_*`, with the `CAL_USE_*` switches and `OVERRIDE_*` values). It is the project's only configuration file: there is no settings, design or calibration file beside it. Nothing else types these numbers. Change one there and MATLAB, the diagram, both consoles' Python and the page all follow.

## What it is

A MATLAB class with only constant properties, `Constants`, in sections (`%%`) that match the list above. Each line is a name, a value and a unit:

```matlab
TABLE_X = 3.51155;                              % [m] along +x
TABLE_CENTRE_X = Constants.TABLE_X / 2;    % [m] centre of the height surface
IP_RED = '192.168.1.110';                       % the Chaser
LED_IDS_RED = [5 3 7 4];
```

Names are `UPPER_SNAKE_CASE`; a per-platform value carries the platform's suffix (`MASS_RED`, `MASS_BLACK`, `MASS_BLUE`, `_ARM`). A derived value is written as arithmetic on earlier constants (`TABLE_CENTRE_X`, `THRUSTER_ARM`, `BACK_EMF_MX64_DATASHEET`), so the relation is in the file, not in someone's head. `help Constants` lists the sections.

## Who reads it, and how

| Where | How |
|---|---|
| MATLAB (`+spot`) | `Constants.NAME`: `pull = Constants.MRAD_PULL * tilt;`; `Constants.all()` is every constant as a struct |
| The Simulink diagram | the same expression in a block's parameter (a Constant, a Gain, a mask value), so the diagram has no numbers of its own |
| The local console's Python | from your MATLAB session, through the **MATLAB Engine for Python**: `SPOT` shares the session (`matlab.engine.shareEngine`) and names it in `SPOT_MATLAB_ENGINE`; [constants.py](file:constants.py) connects (`matlab.engine.connect_matlab`) and evaluates `jsonencode(Constants.all())` when the server starts, and again after each change the worker writes (`C.reload()`). Then `from .constants import C`, `C.TABLE_X`, `C.platform("MASS", "RED")`, `C.int("PORT_LAB_CONSOLE")`, `C.index("PACKET_EXP_CLOCK")` (a 1-based index as Python's 0-based) |
| The lab console's Python (the TX2) | it has no MATLAB, so `constants.py` parses the copy of `Constants.m` that `deploy_tx2.sh` leaves for the Documentation tab (the same `C`). `Constants.verify` runs that reader on the file and compares every value with the class, and `deploy_tx2.sh` runs it before copying, so a line the reader can't parse never reaches the TX2 |
| The page | the server sends `C.as_dict()` as `state.constants` (and a `constants` event after a change); [constants.js](file:constants.js) keeps it: `constant('TABLE_X')`, `constantPlatform('MASS', 'RED')` |
| Shell scripts | `grep` the line out of `Constants.m` |
| The Teensy firmware, the platforms' C, StreamData, `calibrate_arm.py` | can't read MATLAB, so they keep a copy (below) |

The MATLAB Engine for Python is the package MATLAB ships in `<matlabroot>/extern/engines/python`. `SPOT` checks that its Python has it (`spot.console.hasEngine`) and offers to install it (`spot.console.ensureEngine`: `python -m pip install --user "<matlabroot>/extern/engines/python"`, or without pip a copy of the package into the user's site-packages, `install_engine.py`); `spot.console.findPython` prefers a Python that has it, else one new enough for it (R2026a: Python 3.9 to 3.13). MATLAB answers the engine only while it is idle at the prompt, which is why `spot.console.start` returns at once and connects the worker from a timer, and why the console reads the constants again right after a worker command, never during a simulation.

## Who writes it

The MATLAB worker ([Worker.m](file:Worker.m)), and no one else. A value changed on the Simulation tab arrives as its path (`settings.set` with `{path, value}` changes), the Designer's and the Calibration pane's changes as constants (`constants.set`), Update from Lab as the lab's calibration (`calibration.take`, mapped by `spot.worker.labConstants`); the worker rewrites those lines with [`spot.params.writeConstants`](file:writeConstants.m) (the value between `NAME =` and `;` replaced, the comment kept, a long vector wrapped with `...`), clears the class and applies the settings again. The Simulation tab's **Constants…** window shows the whole file the same way: `spot.params.constantsCatalog` (MATLAB reading its own file) gives the sections, comments and formulas, `Constants.all()` the values; an edit there is sent as text (`constants.setText`), read as the type of the constant it replaces by `spot.params.parseConstant` (numbers, `NaN`, `true`/`false`, vectors and matrices, or text for a string: nothing is evaluated), and written the same way. You can edit the file by hand as well: the worker notices the file's time and reads it anew, and the page follows when it comes back to the front.

## Parameter or local constant?

A **parameter** belongs in `Constants`: a physical constant (`G`), a lab fact (`TABLE_X`, `IP_PHASESPACE_SERVER`), a hardware or protocol fact (`PULSES_PER_REV`, `TEENSY_SYNC_BYTE`), a nominal or placeholder value (`THRUST_NOMINAL`, `ARM_STATIC`), a tuning value (`LQR_Q`, `ARM_POSITION_P`), a limit (`CAL_MAX_THRUST`, `MAX_GAIN`) or a default a user sees (`CAL_DRIFT_TIME`, `PHASE2_DURATION`). If a reader of the documentation could ask "where does that number come from?", it is a parameter.

A **local constant** stays in the function's `%% Constants` section (a module constant in Python or JavaScript): a value the program needs for its own sake, which no one outside it would tune. Timeouts on a socket, buffer sizes, iteration caps of a solver, the size of a drawn marker, a log's length. A local section may alias a parameter for a shorter name (`PWM_FULL = Constants.PWM_FULL;`), but never repeats its value.

A value a **user changes** is a constant too: `spot.params.fromConstants` builds the settings struct from them (`THRUST_RED` is `thrusterForce.RED`, `INITIAL_POSE_RED` is `initial.RED.initial`), and the worker writes a change back into the same line. Where a setting has a lab-wide default, the setting's line derives from it until it is changed (`THRUST_RED = Constants.THRUST_NOMINAL;`). A value the **lab measures** lands in a `LAB_*` constant when the project takes it (Update from Lab); its placeholder lives here too (`THRUST_NOMINAL`, the arm's `ARM_*` bench values, `LED_DEFAULT_OFFSET`) and runs until then.

## Adding one

1. Pick the section it belongs to, and a name with the unit's meaning in it where that helps (`CAL_BURN_TIME`, `MIXER_TOLERANCE`, `SERVO_LATENCY_TIMER_MS`).
2. Write the line as `NAME = value;   % [unit] what it is`, with the unit in brackets. If it comes from a measurement, say when and of what (`THRUST_DECAY_DEFAULT`'s comment names the Obstacle and the date).
3. Keep to the grammar the lab console's reader accepts: a number, a `'string'`, `true`/`false`, `NaN`, a row vector `[a b c]` or matrix (`;` between rows; `[]` for none), `pi`, an earlier constant as `Constants.NAME`, and arithmetic on these with `+ - * / ^` and parentheses (a scalar times a vector or matrix). No structs, cells, function calls or forward references: `constants.py` raises `ConstantsError` with the line number on anything else, `Constants.verify` reports it, and `deploy_tx2.sh` refuses to copy the file until it is fixed.
4. Read it where it is used, in each language as above. Don't copy the value into a guide's text without naming the constant: the guides quote values as "0.2825 N (`Constants.THRUST_NOMINAL`)" so a reader can find the source.

## The copies the firmware keeps

[teensy_pwm_control.ino](file:teensy_pwm_control.ino), [arm_pwm.cpp](file:arm_pwm.cpp), the Identify Platform block's [resource_functions.cpp](file:resource_functions.cpp), [StreamData.cc](file:StreamData.cc) and [calibrate_arm.py](file:calibrate_arm.py) (which runs on its own on the Chaser) can't read a MATLAB class. Each keeps its own constants, with a comment naming the `Constants` property it mirrors (`PWM_FREQUENCY` is `TEENSY_PWM_FREQUENCY`, `NUM_SERVER_MARKERS` is `PACKET_SERVER_MARKERS`, `DATASHEET_VOLTS` is `MX64_SUPPLY`).

`Constants.verify` reads those files with a regular expression per mirrored value and reports each as `same` or `DIFFERENT` against this file, and errors if any differ. Run it after changing either side, and before deploying firmware:

```matlab
Constants.verify            % prints one line per mirrored value
```

The setup scripts (`setup_teensy_serial.sh`, `setup_arm_serial.sh`, `deploy_tx2.sh`) read `Constants.m` with `grep`, so they need no mirror.

## Checking a change

- MATLAB: `Constants.NAME` at the prompt, then `spot.control.verifyConsoleController` if it is anything the LQR or the mixer uses, and a simulation.
- Python: `python3 spot_console/constants.py` from `webapp/` prints every constant as the lab console's reader sees it (`--json` for one JSON object, which `Constants.verify` compares with the class).
- The page: the Simulation tab's values, and the lab console's limits, come from the state, so a reload shows the change.
- `Constants.verify` for anything with a firmware copy.

The Coding Standard's rule and checklist item are in [`CODE_TEMPLATE.md`](file:CODE_TEMPLATE.md), section 4 ([The Coding Standard](page:dev-code-standard)).
