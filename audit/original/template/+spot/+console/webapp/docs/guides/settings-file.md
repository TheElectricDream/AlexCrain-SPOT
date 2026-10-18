# The Settings in `Constants.m`

Everything the Simulation tab shows is a constant in the project's `Constants.m` ([Constants](page:dev-constants)), next to every other parameter of SPOT. There is no settings file: the console writes the constant's line on every change (the top bar's **Constants…** window shows every constant with its unit and meaning and edits any of them; a hand edit in MATLAB's editor is picked up when you save), and MATLAB builds the same struct `s` that every `spot.*` function takes with `spot.params.fromConstants`. `spot.params.toConstants` maps the struct back to the constants' names, and `spot.params.writeConstants` writes them.

The sections below name each field of `s` and the constant behind it, in brackets. Where a setting has a lab-wide default, its line derives from that default until the setting is changed (`THRUST_RED = Constants.THRUST_NOMINAL;`); the console then writes the value in its place.

## Top level

| Field of `s` | Constant | Meaning |
|---|---|---|
| `diagram` | `DIAGRAM` (`''`: the project's one `.slx`) | the Simulink diagram to simulate or build |
| `platforms.RED/BLACK/BLUE/ARM` | `USE_RED`, `USE_BLACK`, `USE_BLUE`, `USE_ARM` (`false`) | which platforms take part; the diagram gets them as one code, `platformSelection` (`spot.params.platformSelection`) |
| `sampleRate` | `SAMPLE_RATE` (20 Hz) | the controllers' rate; the workspace gets `baseRate = 1/sampleRate` |
| `dataRate` | `DATA_RATE` (10 Hz) | the PhaseSpace camera rate the Experiment tab starts the stream at (1–960, whole Hz; `MAX_DATA_RATE`); the workspace gets `dataRate = 1/dataRate` s and the LQR gains are designed at it. Locked in the page behind a padlock, because changing it changes the gains |
| `realTime` | `REAL_TIME` (`false`) | simulate at the clock's pace (Simulink pacing), so you can watch the Live Plot as an experiment |
| `pwmFrequency` | `PWM_FREQUENCY` (= `TEENSY_PWM_FREQUENCY`, 5 Hz) | the thrusters' PWM frequency the diagram's duty cycles assume (`PWMFreq`). The Teensy firmware has its own copy of the constant (checked by `Constants.verify`); a different value here gives a warning. Locked in the console: its padlock explains why before it unlocks |
| `labConsoleAddress` | `IP_LAB_CONSOLE` (`'192.168.1.116'`) | the lab console (TX2): the diagram's UDP blocks send the platforms' clocks, messages and live streams there (`labConsoleIP`, as four numbers) |

## `timing`: the phases

`phase0`, `phase1`, `phase2`, `subPhases` (four), `phase4`, `phase5` are `PHASE0_DURATION` … `PHASE5_DURATION` and `SUB_PHASE_DURATIONS`; `on.phase2`, `on.subPhases` and `on.phase4` are `PHASE2_ON`, `SUB_PHASES_ON` and `PHASE4_ON`.

| Phase | Name | What happens | Can be switched off? |
|---|---|---|---|
| 0 | Synchronization | the platforms wait for the lab console's start, pucks off | no |
| 1 | Start Floating | the pucks (air bearings) come on, controllers off | no |
| 2 | Initial Position | fly to the initial poses | yes |
| 3.1–3.4 | Experiment | the experiment proper, four sub-phases you program | each one |
| 4 | Return Home | fly to the home poses | yes |
| 5 | Hold Home | hold the home poses | no |

When phase 5 ends (at `tsim`) the diagram's last branch, *Clean Shutdown*, switches the pucks and the controllers off ([1 - Experiment Logic](page:diagram-experiment-logic)).

A segment that is off counts as zero seconds, so the timeline simply skips it. `spot.params.timing` turns these into `Phase0_End … Phase5_End`, `Phase3_SubPhase1_End …` and `tsim` for the diagram.

## `solver`

The solver the simulation uses: `type` (`SOLVER_TYPE`: `Fixed-step` or `Variable-step`), `fixedStepName` (`SOLVER_FIXED_STEP`, `ode4`), `variableStepName` (`SOLVER_VARIABLE_STEP`), `maxStep`, `minStep`, `initialStep` (`SOLVER_MAX_STEP`, `SOLVER_MIN_STEP`, `SOLVER_INITIAL_STEP`), `relTol`, `absTol` (`SOLVER_REL_TOL`, `SOLVER_ABS_TOL`). Selecting a diagram copies its own solver settings into these (`spot.sim.readSolver`). The fixed step is always `baseRate`. Experiments always use the fixed-step solver.

## `initial`: poses

`[x y θ]` in **metres and degrees** in the table frame: *initial* is where the platform starts (and where phase 2 drives it), *home* where phase 4 returns it (`INITIAL_POSE_RED`, `HOME_POSE_RED` and so on). The arm's pose is its three joint angles in degrees (`INITIAL_POSE_ARM`). In the workspace they become `init_states_RED` etc. in radians.

## `mass`: mass properties

For each platform: `scales` (the three scale readings A, B, C in kg, see [Mass Properties](page:math-mass-properties); `SCALES_RED` …), `bifilar` (`T` period, `BIFILAR_PERIOD_RED` …; `L` string length and `D` string spacing of the pendulum test are the lab's one rig, `BIFILAR_LENGTH` and `BIFILAR_SPACING`, so they are the same for every platform), and `mass`/`inertia` (`MASS_RED`, `INERTIA_RED` …), which are used instead of the measured ones when `override` (`MASS_OVERRIDE`) is `true`. `arm` holds the arm's geometry and link masses/inertias (`Phi`, `b0`, `a1…b3`, `mPROX…IEE`; `ARM_PHI`, `ARM_B0`, `ARM_A1` …, `ARM_MASS_PROX` …, `ARM_INERTIA_PROX` …) and `mount` (`ARM_MOUNT_FACE`, `ARM_MOUNT_SIDE`: which face and side of the Chaser it sits on; the Designer sets it).

## `calibration`

Three switches, all `true` by default (`CAL_USE_TABLE`, `CAL_USE_THRUSTERS`, `CAL_USE_ARM`), that decide which parts of the lab's calibration the project took (the `LAB_*` constants, see [The Calibration Tab](page:tab-calibration)) simulations use. They are the switches on the cards of the Simulation tab's Calibration pane.

`calibration.tableSlope`: whether simulations use the table's measured slope (`LAB_TABLE_HEIGHT`, `LAB_TABLE_CENTRE`, `LAB_TABLE_DAMPING_*`), measured by the lab console's table slope calibration. The floating platforms then slide downhill and their air bearings drag them (`tableSlope`, `tableDamping_*` in the workspace, `spot.params.apply`). With `false`, or without a table calibration, the table is level.

`calibration.thrusters`: whether the diagram's thruster mixer, in simulations and in the experiments built from it, uses each thruster's measured thrust and the platform's measured thrust decay (`LAB_THRUST_*`, `LAB_THRUST_DECAY_*`; `F_thrusters_*` and `thrust_decay_*` in the workspace). With `false`, or for a platform without a thruster calibration, `thrusterForce` is used for all eight and `thrusterDecay` for the decay.

`calibration.arm`: whether the simulated arm (`armModel`, `spot.arm.parameters`) uses its joints' measured friction and inertia (`LAB_ARM_STATIC`, `LAB_ARM_COULOMB`, `LAB_ARM_STRIBECK`, `LAB_ARM_VISCOUS`, `LAB_ARM_BACK_EMF`, `LAB_ARM_TORQUE_PER_VOLT`, `LAB_ARM_ARMATURE`, one value per joint). With `false`, or without an arm calibration, the arm model's defaults (the `ARM_*` bench values).

Values you override on the Calibration pane are the `OVERRIDE_*` constants (`NaN`: not overridden), and apply while the part's switch is on. Update from Lab rewrites the `LAB_*` constants (and the record behind them in `Saved Data/Calibration`, which only the Calibration pane draws) and leaves the overrides alone.

## `thrusterForce`

The thrust of one thruster per platform (`THRUST_RED`, `THRUST_BLACK`, `THRUST_BLUE`; 0.2825 N, `THRUST_NOMINAL`, until changed). `spot.control.nominalForces` expands it to the eight thrusters. A thruster calibration replaces it when `calibration.thrusters` is on and the project has one for that platform.

## `thrusterDecay`

Per platform (`THRUST_DECAY_RED` …), the thrust lost per extra valve open at once: with *n* valves open, each thruster gives $1 - \text{decay}\,(n-1)$ of its thrust alone, because they share one regulated air supply. The default, `0.112` (11.2 %) for all three, is the Obstacle's, measured on 2026-10-14 (`THRUST_DECAY_DEFAULT`). The thruster calibration's Together stage measures it per platform, and replaces this value when `calibration.thrusters` is on. The Platforms pane's Thrust Decay field. See [The Thruster Mixer](page:math-mixer).

## `control`

Per platform, the LQR weights under `lqr`: `Q` = `[x y θ ẋ ẏ θ̇]` (`LQR_Q_RED` …, from `LQR_Q` = `[1 1 0.05 10 10 1]`), `R` = `[Fx Fy Tz]` (`LQR_R_RED` …, from `LQR_R` = `[6 6 6]`). The gain `K_<PLATFORM>` is computed from them at `dataRate` (`spot.control.lqrGains`).

`control.ARM`: the arm's two controllers, in the servos' units, `[shoulder elbow wrist]` where there are three values: `position.P/I/D` (`ARM_POSITION_P/I/D`), `position.profileVelocity`, `profileAcceleration`, `pwmLimit` (`ARM_POSITION_PROFILE_VELOCITY`, `ARM_POSITION_PROFILE_ACCELERATION`, `ARM_POSITION_PWM_LIMIT`); `velocity.P/I`, `profileAcceleration`, `pwmLimit` (`ARM_VELOCITY_P/I`, `ARM_VELOCITY_PROFILE_ACCELERATION`, `ARM_VELOCITY_PWM_LIMIT`). Gains 0–16383 (`MAX_GAIN`; one value applies to all three joints), profile 0–32767 (`MAX_PROFILE`), PWM limit 0–885 (`PWM_FULL`). They become `armControl` in the workspace (`spot.params.apply`).

## Old settings files

The settings files of the earlier pre-releases (`spot_settings.json`) and the old GUI's saved states (`.mat`) are not read: type the values you need into the page, or into `Constants.m`. (`spot.params.fromLegacyState` still converts an old GUI state to a settings struct in MATLAB, for scripts.)

## The Designer and the calibration, in the same file

What is mounted on each platform (the Spacecraft Designer) is `DOCK_RED`, `DOCK_FACE_RED`, `DOCK_SIDE_RED`, `SOLAR_RED`, `SOLAR_AXIS_RED` and the same for BLACK and BLUE ([design.py](file:design.py) checks them; the Chaser's arm choice and `ARM_MOUNT_FACE`/`ARM_MOUNT_SIDE` follow each other). Where each platform's PhaseSpace LEDs are is `LED_XY_RED` … (4×2 m from the body centre). The lab calibration the project took is the `LAB_*` section at the end of the file, written by Update from Lab.
