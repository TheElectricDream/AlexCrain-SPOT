# The Settings File: `spot_settings.json`

Everything the Simulation tab shows is one JSON file in the project folder, `spot_settings.json`. The console writes it after every change; you can also edit it by hand (the console reloads it with *Load…*) or build the same struct in MATLAB with [[spot.params.defaults|spot-params-defaults]] and [[spot.params.load|spot-params-load]].

The file is a snapshot of the MATLAB struct `s` that every `spot.*` function takes. Missing keys get their defaults when it is loaded ([[spot.params.merge|spot-params-merge]]), so an old file still works after new settings are added.

## Top level

| Key | Default | Meaning |
|---|---|---|
| `version` | `1` | file format version |
| `diagram` | `""` (the console picks the first `.slx` in the folder) | the Simulink diagram to simulate or build |
| `platforms.RED/BLACK/BLUE/ARM` | `false` | which platforms take part; the diagram gets them as one code, `platformSelection` ([[spot.params.platformSelection|spot-params-platformSelection]]) |
| `sampleRate` | `20` Hz | the controllers' rate; the workspace gets `baseRate = 1/sampleRate` |
| `dataRate` | `10` Hz | the PhaseSpace camera rate the Experiment tab starts the stream at (1–960, whole Hz); the workspace gets `dataRate = 1/dataRate` s and the LQR gains are designed at it. Locked in the page behind a padlock, because changing it changes the gains |
| `realTime` | `false` | simulate at the clock's pace (Simulink pacing), so you can watch the Live Plot as an experiment |
| `pwmFrequency` | `5` Hz | the thrusters' PWM frequency the diagram's duty cycles assume (`PWMFreq`). The Teensy firmware has its own constant (5 Hz); a different value here gives a warning. Locked in the console: its padlock explains why before it unlocks |
| `labConsoleAddress` | `"192.168.1.116"` | the lab console (TX2): the diagram's UDP blocks send the platforms' clocks, messages and live streams there (`labConsoleIP`, as four numbers) |

## `timing`: the phases

```json
"timing": { "phase0": 10, "phase1": 5, "phase2": 40, "subPhases": [10, 10, 30, 120], "phase4": 30, "phase5": 20,
            "on": { "phase2": true, "subPhases": [true, true, true, true], "phase4": true } }
```

| Phase | What happens | Can be switched off? |
|---|---|---|
| 0 | synchronisation: the platforms wait for the lab console's start | no |
| 1 | start floating: the pucks (air bearings) come on | no |
| 2 | go to the initial position | yes |
| 3.1–3.4 | the experiment proper, four sub-phases you program | each one |
| 4 | return home | yes |
| 5 | stop: pucks off, thrusters off | no |

A segment that is off counts as zero seconds, so the timeline simply skips it. [[spot.params.timing|spot-params-timing]] turns these into `Phase0_End … Phase5_End`, `Phase3_SubPhase1_End …` and `tsim` for the diagram.

## `solver`

The solver the simulation uses (the diagram's own settings are copied here when a diagram is selected, [[spot.sim.readSolver|spot-sim-readSolver]]): `type` (`Fixed-step` or `Variable-step`), `fixedStepName` (`ode4`), `variableStepName`, `maxStep`, `minStep`, `initialStep`, `relTol`, `absTol`. The fixed step is always `baseRate`. Experiments always use the fixed-step solver.

## `initial`: poses

```json
"RED":   { "initial": [2.894, 0.736, 93.9], "home": [2.963, 1.604, 156.9] },
"ARM":   { "initial": [60, 90, 0] }
```

`[x y θ]` in **metres and degrees** in the table frame: *initial* is where the platform starts (and where phase 2 drives it), *home* where phase 4 returns it. The arm's pose is its three joint angles in degrees. In the workspace they become `init_states_RED` etc. in radians.

## `mass`: mass properties

For each platform: `scales` (the three scale readings A, B, C in kg, see [[Mass Properties|Math-Mass-Properties]]), `bifilar` (`T` period, `L` string length, `D` string spacing of the pendulum test), and `mass`/`inertia` which are used instead of the measured ones when `override` is `true`. `arm` holds the arm's geometry and link masses/inertias (`Phi`, `b0`, `a1…b3`, `mPROX…IEE`) and `mount` (which face and side of the Chaser it sits on; the Designer sets it).

## `calibration`

`calibration.tableSlope` (`true` by default): whether simulations use the table's measured slope, the table slope calibration the lab console's [[Calibration Tab|Calibration-Tab]] measured, copied into the project as `resources/spot_calibration.json`. The floating platforms then slide downhill and their air bearings drag them (`tableSlope`, `tableDamping_*` in the workspace, [[spot.params.apply|spot-params-apply]]). With `false`, or without a table calibration in the project, the table is level. The Scenario pane's Table Slope switch.

`calibration.thrusters` (`true` by default): whether the diagram's thruster mixer, in simulations and in the experiments built from it, uses each thruster's measured thrust, from the thruster calibration in the same file (`F_thrusters_*` in the workspace, [[spot.params.apply|spot-params-apply]]). With `false`, or for a platform the project has no thruster calibration for, `thrusterForce` is used for all eight. The Scenario pane's Calibrated Thrust switch.

`calibration.arm` (`true` by default): whether the simulated arm (`armModel`, [[spot.arm.parameters|spot-arm-parameters]]) uses its joints' measured friction and inertia, from the arm calibration in the same file (the Calibration tab's arm calibration) or from a `resources/arm_calibration.json` copied by hand from [[calibrate_arm.py|Tool-calibrate-arm]], whichever was measured last. With `false`, or without an arm calibration in the project, the arm model's defaults. The Scenario pane's Calibrated Arm switch.

## `thrusterForce`

The thrust of one thruster per platform, 0.2825 N by default. [[spot.control.nominalForces|spot-control-nominalForces]] expands it to the eight thrusters. A thruster calibration replaces it when `calibration.thrusters` is on and the project has one for that platform.

## `control`

Per platform, the LQR weights: `Q` = `[x y θ ẋ ẏ θ̇]`, `R` = `[Fx Fy Tz]`. The gain `K_<PLATFORM>` is computed from them at `dataRate` ([[spot.control.lqrGains|spot-control-lqrGains]]).

`control.ARM`: the arm's two controllers, in the servos' units, `[shoulder elbow wrist]` where there are three values:

```json
"ARM": { "position": { "P": [25, 56, 21], "I": [10, 10, 10], "D": [1545, 230, 236],
                       "profileVelocity": 20, "profileAcceleration": 0, "pwmLimit": 80 },
         "velocity": { "P": [160, 70, 30], "I": [220, 1500, 300], "profileAcceleration": 0, "pwmLimit": 885 } }
```

Gains 0–16383 (one value applies to all three joints), profile 0–32767, PWM limit 0–885. They become `armControl` in the workspace ([[spot.params.apply|spot-params-apply]]).

## Older files

- Files without `dataRate` get `sampleRate/2`.
- Files with an arm `drop` pose (before it was removed) use it as the initial pose.
- Saved GUI states of the old App Designer GUI (`.mat`) are converted on load by [[spot.params.fromLegacyState|spot-params-fromLegacyState]].

## The sibling file: `spot_design.json`

What is mounted on each platform (the Spacecraft Designer): a docking cone (`none`, `male`, `female`, or `arm` on the Chaser), its `face` and `side`, and the solar panels. It belongs to the console's server, not to MATLAB ([[design.py|Console-design]]), but the Chaser's arm choice and the settings' `mass.arm.mount` follow each other.
