# Constants: Every Parameter in One Place

Every number that describes SPOT rather than its programs lives in one file, [`Constants.m`](file:Constants.m): the table and gravity, the lab's addresses and ports, the PhaseSpace packet, the platforms' bodies and measured mass properties, the thrusters and the Teensy, the mixer, the rates and phases, the poses, the LQR weights, the Dynamixel servos, the arm's bench calibration, geometry and controllers, the calibrations' limits and defaults, Interactive Mode, the experiment's streams. Nothing else types these numbers. Change one there and MATLAB, the diagram, both consoles' Python and the page all follow.

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
| MATLAB (`+spot`) | `Constants.NAME`: `pull = Constants.MRAD_PULL * tilt;` |
| The Simulink diagram | the same expression in a block's parameter (a Constant, a Gain, a mask value), so the diagram has no numbers of its own |
| The consoles' Python | [constants.py](file:constants.py) parses the file at startup: `from .constants import C`, then `C.TABLE_X`, `C.platform("MASS", "RED")`, `C.int("PORT_LAB_CONSOLE")`, `C.index("PACKET_EXP_CLOCK")` (a 1-based index as Python's 0-based). On the TX2, the copy `deploy_tx2.sh` leaves for the Documentation tab |
| The page | the server sends `C.as_dict()` as `state.constants`; [constants.js](file:constants.js) keeps it: `constant('TABLE_X')`, `constantPlatform('MASS', 'RED')` |
| Shell scripts | `grep` the line out of `Constants.m` |
| The Teensy firmware, the platforms' C, StreamData, `calibrate_arm.py` | can't read MATLAB, so they keep a copy (below) |

Neither console needs MATLAB for this, which is why the lab console can check a calibration file against `CAL_MAX_THRUST` or know the Chaser's address without it.

## Parameter or local constant?

A **parameter** belongs in `Constants`: a physical constant (`G`), a lab fact (`TABLE_X`, `IP_PHASESPACE_SERVER`), a hardware or protocol fact (`PULSES_PER_REV`, `TEENSY_SYNC_BYTE`), a nominal or placeholder value (`THRUST_NOMINAL`, `ARM_STATIC`), a tuning value (`LQR_Q`, `ARM_POSITION_P`), a limit (`CAL_MAX_THRUST`, `MAX_GAIN`) or a default a user sees (`CAL_DRIFT_TIME`, `PHASE2_DURATION`). If a reader of the documentation could ask "where does that number come from?", it is a parameter.

A **local constant** stays in the function's `%% Constants` section (a module constant in Python or JavaScript): a value the program needs for its own sake, which no one outside it would tune. Timeouts on a socket, buffer sizes, iteration caps of a solver, the size of a drawn marker, a log's length. A local section may alias a parameter for a shorter name (`PWM_FULL = Constants.PWM_FULL;`), but never repeats its value.

A value a **user changes** is a setting, not a constant: its default lives here, and `spot.params.defaults` builds `spot_settings.json` from it (`THRUST_NOMINAL` becomes `thrusterForce`, `INITIAL_POSE_RED` becomes `initial.RED.initial`). A value the **lab measures** is a calibration: `spot_calibration.json` holds the measurement, and its placeholder lives here (`THRUST_NOMINAL`, the arm's `ARM_*` bench values, `LED_DEFAULT_OFFSET`). The calibration and settings files hold state; this file holds what the state starts from.

## Adding one

1. Pick the section it belongs to, and a name with the unit's meaning in it where that helps (`CAL_BURN_TIME`, `MIXER_TOLERANCE`, `SERVO_LATENCY_TIMER_MS`).
2. Write the line as `NAME = value;   % [unit] what it is`, with the unit in brackets. If it comes from a measurement, say when and of what (`THRUST_DECAY_DEFAULT`'s comment names the Obstacle and the date).
3. Keep to the grammar the Python reader accepts: a number, a `'string'`, a row vector `[a b c]` or matrix (`;` between rows), `pi`, an earlier constant as `Constants.NAME`, and arithmetic on these with `+ - * / ^` and parentheses. No structs, cells, function calls or forward references: `constants.py` raises `ConstantsError` with the line number on anything else, and the console won't start until it is fixed.
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
- Python: `python3 -c "from spot_console.constants import C; print(C.NAME)"` from `webapp/`.
- The page: the Simulation tab's values, and the lab console's limits, come from the state, so a reload shows the change.
- `Constants.verify` for anything with a firmware copy.

The Coding Standard's rule and checklist item are in [`CODE_TEMPLATE.md`](file:CODE_TEMPLATE.md), section 4 ([The Coding Standard](page:dev-code-standard)).
