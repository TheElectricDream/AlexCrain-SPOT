# The Settings File: `spot_settings.json`

Everything the Simulation tab shows is one JSON file in the project folder, `spot_settings.json`. The console writes it after every change; you can also edit it by hand (the top bar's **Load…** reads it again) or build the same struct in MATLAB with `spot.params.defaults` and `spot.params.load`.

The file is a snapshot of the MATLAB struct `s` that every `spot.*` function takes. Missing keys get their defaults when it is loaded (`spot.params.merge`), so an old file still works after new settings are added. Every default comes from `Constants` ([Constants](page:dev-constants)): `spot.params.defaults` builds the struct from it, and the property names in brackets below say which.

## Top level

| Key | Default | Meaning |
|---|---|---|
| `version` | `1` | file format version |
| `diagram` | `""` (the console picks the first `.slx` in the folder) | the Simulink diagram to simulate or build |
| `platforms.RED/BLACK/BLUE/ARM` | `false` | which platforms take part; the diagram gets them as one code, `platformSelection` (`spot.params.platformSelection`) |
| `sampleRate` | `20` Hz (`SAMPLE_RATE`) | the controllers' rate; the workspace gets `baseRate = 1/sampleRate` |
| `dataRate` | `10` Hz (`DATA_RATE`) | the PhaseSpace camera rate the Experiment tab starts the stream at (1–960, whole Hz; `MAX_DATA_RATE`); the workspace gets `dataRate = 1/dataRate` s and the LQR gains are designed at it. Locked in the page behind a padlock, because changing it changes the gains |
| `realTime` | `false` | simulate at the clock's pace (Simulink pacing), so you can watch the Live Plot as an experiment |
| `pwmFrequency` | `5` Hz (`TEENSY_PWM_FREQUENCY`) | the thrusters' PWM frequency the diagram's duty cycles assume (`PWMFreq`). The Teensy firmware has its own copy of the constant (5 Hz, checked by `Constants.verify`); a different value here gives a warning. Locked in the console: its padlock explains why before it unlocks |
| `labConsoleAddress` | `"192.168.1.116"` (`IP_LAB_CONSOLE`) | the lab console (TX2): the diagram's UDP blocks send the platforms' clocks, messages and live streams there (`labConsoleIP`, as four numbers) |

## `timing`: the phases

```json
"timing": { "phase0": 10, "phase1": 5, "phase2": 40, "subPhases": [10, 10, 30, 120], "phase4": 30, "phase5": 20,
            "on": { "phase2": true, "subPhases": [true, true, true, true], "phase4": true } }
```

The durations are `PHASE0_DURATION` … `PHASE5_DURATION` and `SUB_PHASE_DURATIONS`.

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

The solver the simulation uses (the diagram's own settings are copied here when a diagram is selected, `spot.sim.readSolver`; the defaults are `SOLVER_TYPE`, `SOLVER_FIXED_STEP`, `SOLVER_VARIABLE_STEP` and `SOLVER_REL_TOL`): `type` (`Fixed-step` or `Variable-step`), `fixedStepName` (`ode4`), `variableStepName`, `maxStep`, `minStep`, `initialStep`, `relTol`, `absTol`. The fixed step is always `baseRate`. Experiments always use the fixed-step solver.

## `initial`: poses

```json
"RED":   { "initial": [2.61, 1.21, 0], "home": [2.61, 1.21, 0] },
"ARM":   { "initial": [90, 90, 0] }
```

`[x y θ]` in **metres and degrees** in the table frame: *initial* is where the platform starts (and where phase 2 drives it), *home* where phase 4 returns it (`INITIAL_POSE_RED`, `HOME_POSE_RED` and so on). The arm's pose is its three joint angles in degrees (`INITIAL_POSE_ARM`). In the workspace they become `init_states_RED` etc. in radians.

## `mass`: mass properties

For each platform: `scales` (the three scale readings A, B, C in kg, see [Mass Properties](page:math-mass-properties); `SCALES_RED` …), `bifilar` (`T` period, `L` string length, `D` string spacing of the pendulum test; `BIFILAR_PERIOD_RED` …, `BIFILAR_LENGTH`, `BIFILAR_SPACING`), and `mass`/`inertia` which are used instead of the measured ones when `override` is `true`. `arm` holds the arm's geometry and link masses/inertias (`Phi`, `b0`, `a1…b3`, `mPROX…IEE`; `ARM_PHI`, `ARM_B0`, `ARM_A1` …, `ARM_MASS_PROX` …, `ARM_INERTIA_PROX` …) and `mount` (`ARM_MOUNT_FACE`, `ARM_MOUNT_SIDE`) (which face and side of the Chaser it sits on; the Designer sets it).

## `calibration`

Three switches, all `true` by default, that decide which parts of the project's calibration (`spot_calibration.json`, see [The Calibration Tab](page:tab-calibration)) simulations use. They are the switches on the cards of the Simulation tab's Calibration pane.

`calibration.tableSlope`: whether simulations use the table's measured slope, measured by the lab console's table slope calibration. The floating platforms then slide downhill and their air bearings drag them (`tableSlope`, `tableDamping_*` in the workspace, `spot.params.apply`). With `false`, or without a table calibration in the project, the table is level.

`calibration.thrusters`: whether the diagram's thruster mixer, in simulations and in the experiments built from it, uses each thruster's measured thrust and the platform's measured thrust decay (`F_thrusters_*` and `thrust_decay_*` in the workspace). With `false`, or for a platform the project has no thruster calibration for, `thrusterForce` is used for all eight and `thrusterDecay` for the decay.

`calibration.arm`: whether the simulated arm (`armModel`, `spot.arm.parameters`) uses its joints' measured friction and inertia, from the arm calibration in the same file or from an `arm_calibration.json` copied by hand into the project folder from [calibrate_arm.py](file:calibrate_arm.py), whichever was measured last. With `false`, or without an arm calibration in the project, the arm model's defaults.

Values you override on the Calibration pane are not settings: they are kept in `spot_calibration.json` itself (its `overrides`), and apply while the part's switch is on.

## `thrusterForce`

The thrust of one thruster per platform, 0.2825 N by default (`THRUST_NOMINAL`). `spot.control.nominalForces` expands it to the eight thrusters. A thruster calibration replaces it when `calibration.thrusters` is on and the project has one for that platform.

## `thrusterDecay`

Per platform, the thrust lost per extra valve open at once: with *n* valves open, each thruster gives $1 - \text{decay}\,(n-1)$ of its thrust alone, because they share one regulated air supply. The default, `0.112` (11.2 %) for all three, is the Obstacle's, measured on 2026-10-14 (`THRUST_DECAY_DEFAULT`). The thruster calibration's Together stage measures it per platform, and replaces this value when `calibration.thrusters` is on. The Platforms pane's Thrust Decay field. See [The Thruster Mixer](page:math-mixer).

## `control`

Per platform, the LQR weights under `lqr`: `Q` = `[x y θ ẋ ẏ θ̇]` (default `[1 1 0.05 10 10 1]`, `LQR_Q`), `R` = `[Fx Fy Tz]` (default `[6 6 6]`, `LQR_R`):

```json
"RED": { "lqr": { "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6] } }
```

The gain `K_<PLATFORM>` is computed from them at `dataRate` (`spot.control.lqrGains`). Settings saved with the old PD gains still load; the PD gains are dropped.

`control.ARM`: the arm's two controllers, in the servos' units, `[shoulder elbow wrist]` where there are three values:

```json
"ARM": { "position": { "P": [25, 56, 21], "I": [10, 10, 10], "D": [1545, 230, 236],
                       "profileVelocity": 20, "profileAcceleration": 0, "pwmLimit": 80 },
         "velocity": { "P": [160, 70, 30], "I": [220, 1500, 300], "profileAcceleration": 0, "pwmLimit": 885 } }
```

Gains 0–16383 (`MAX_GAIN`; one value applies to all three joints), profile 0–32767 (`MAX_PROFILE`), PWM limit 0–885 (`PWM_FULL`). The defaults are `ARM_POSITION_P/I/D`, `ARM_POSITION_PROFILE_VELOCITY`, `ARM_POSITION_PROFILE_ACCELERATION`, `ARM_POSITION_PWM_LIMIT`, `ARM_VELOCITY_P/I`, `ARM_VELOCITY_PROFILE_ACCELERATION` and `ARM_VELOCITY_PWM_LIMIT`. They become `armControl` in the workspace (`spot.params.apply`).

## Older files

- Files without `dataRate` get `sampleRate/2`.
- Files without `thrusterDecay` get the default.
- Files with an arm `drop` pose (before it was removed) use it as the initial pose.
- Saved GUI states of the old App Designer GUI (`.mat`) are converted on load by `spot.params.fromLegacyState`.

## The sibling file: `spot_design.json`

What is mounted on each platform (the Spacecraft Designer): a docking cone (`none`, `male`, `female`, or `arm` on the Chaser), its `face` and `side`, and the solar panels. It belongs to the console's server, not to MATLAB ([design.py](file:design.py)), but the Chaser's arm choice and the settings' `mass.arm.mount` follow each other.
