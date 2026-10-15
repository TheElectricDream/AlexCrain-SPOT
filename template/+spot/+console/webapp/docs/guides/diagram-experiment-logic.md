# 1 - Experiment Logic

**Priority 30.** The experiment's **timeline**: which phase we are in, and in each phase, what every platform should be doing: where it should go, whether its controller and path planner are on, whether it floats, and a text for the console's Messages panel. **This is the subsystem you edit** to define an experiment (with [3 - Guidance](page:diagram-guidance) and [4 - Control](page:diagram-control) for *how* it goes there).

![1 - Experiment Logic](images/sub_1_Experiment_Logic.png)

**Reads:** `Univ_Time`. **Writes:** for RED, BLACK, BLUE and ARM: `X_Desired_States`, `X_Control_Law_Enabler`, `X_Path_Planner_Selection`; and `Float_State`, `RED_Logger`, `BLACK_Logger`, `BLUE_Logger`.

## The phases

| Phase | Name | Ends at (workspace) | Float | Controllers | Desired states |
|---|---|---|---|---|---|
| 0 | Synchronization | `Phase0_End` | off | all off (0) | unchanged |
| 1 | Start Floating | `Phase1_End` | **on** | all off | unchanged |
| 2 | Initial Position | `Phase2_End` | on | platforms on (1), arm off | `init_states_X`; arm `[π/2 0 π/2]` |
| 3 | Experiment | `Phase3_End` | on | on; arm on in sub-phase 1 | sub-phases 1–4 (below) |
| 4 | Return Home | `Phase4_End` | on | platforms on, arm off | `home_states_X`; arm `[π/2 0 π/2]` |
| 5 | Hold Home | `Phase5_End` | on | platforms on, arm off | `home_states_X` |
| 6 | Clean Shutdown | from `Phase5_End` (= `tsim`, so the run's last step) | **off** | all off | unchanged |

The phase durations are the Scenario pane's **Timeline** ([Settings File](page:settings-file) `timing`); `spot.params.timing` turns them into the cumulative end times `Phase0_End … Phase5_End`, `Phase3_SubPhase1_End … Phase3_SubPhase4_End` and `tsim`. A phase switched off on the Timeline has zero length, so it is skipped.

## The top level

- **Universal_Time25** reads `Univ_Time` (Simulink's clock in a simulation, the shared experiment clock in the lab).
- Six **Constants** hold the end times.
- **Separate Phases** is an **If** block with seven branches: `u1 < u2` (phase 0), `u1 < u3 & u1 ≥ u2` (phase 1), … , `u1 ≥ u7` (phase 6). Exactly one of the seven **Phase #n** action subsystems runs each step.

## Phase #0: Synchronization and Phase #6: Clean Shutdown

Twenty-five blocks that are all constants and Data Store Writes: `0` into the four `X_Control_Law_Enabler`s (controllers off: [4 - Control](page:diagram-control) then runs its *Disable* branches), `0` into the four `X_Path_Planner_Selection`s, `0` (**Puck State**) into `Float_State` (the air bearings off, so the platform sits on the table), and a **String Constant** into each `X_Logger` (the message the console shows for this phase). In the lab, phase 0 is the time the programs spend waiting for every platform to start ([Experiment Tab](page:tab-experiment)); the shared clock stays at 0 until then, so phase 0 lasts as long as it needs to.

## Phase #1: Start Floating

The same, with **Puck State** = 1: the pucks are switched on and the platform lifts off the granite, controllers still off. The default 5 s gives the air bearings time to come up before anything pushes.

## Phase #2: Initial Position

![Phase #2](images/sub_1_Experiment_Logic_Phase_2_Initial_Position.png)

Controllers on: `1` into `RED/BLACK/BLUE_Control_Law_Enabler` (the default LQR), `0` into `ARM_Control_Law_Enabler` (arm off), path planners `0` (the desired state is used directly), `Float_State` 1, loggers. The desired states:
- **Desired Position RED/BLACK/BLUE**: a Constant `init_states_X` (the Platforms pane's **Initial** pose, converted to m and rad by `spot.params.apply`) → `X_Desired_States`.
- **Desired Joint Angles RED**: `[π/2, 0, π/2]` muxed → `ARM_Desired_States` (the folded pose; the arm controller is off in this phase, so it is just a value for the record).

Phase 2 is when the platforms fly from wherever they were put on the table to the experiment's starting poses. 40 s by default.

## Phase #3: Experiment

![Phase #3](images/sub_1_Experiment_Logic_Phase_3_Experiment.png)

**Universal_Time** and four **Constants** (`Phase3_SubPhase1_End` …) feed another **If**, **Experiment Sub-Phases**, with four branches, one **Sub-Phase #n** each. All four sub-phases in the template contain the **same** example experiment; they are there so you can put a different step of your experiment in each.

### Sub-Phase #1 (and 2–4)

![Sub-Phase #1](images/sub_1_Experiment_Logic_Phase_3_Experiment_Sub_Phase_1.png)

Controllers on for the three platforms **and the arm** (`ARM_Control_Law_Enabler` = 1 in Sub-Phase #1: the default position controller; 0 in 2–4), path planners 0, floating, loggers. The desired positions are a **circle**: each platform goes round the table's centre.

**Desired Position BLACK**:
- **Calculate Time since Phase #3 Start**: `Univ_Time − Phase2_End` = the time `t` into the experiment.
- **Desired Rate** 0.0349 rad/s (2° per second) × `t` (**Product**) = the angle travelled, `ωt`.
- **Constant** = the platform's **phase on the circle**: 0 for RED, 2π/3 for BLACK, 4π/3 for BLUE, so the three are 120° apart.
- **Calculate Circular Tracking** (inputs *Angular Frequency* = ωt and *Phase* = φ):

$$x_d = \frac{x_{\text{Length}}}{2} + 0.85\cos(\omega t + \varphi), \qquad y_d = \frac{y_{\text{Length}}}{2} + 0.85\sin(\omega t + \varphi)$$

  (Sum, Cos/Sin, Gain 0.85 = the circle's radius in metres, Sum1/Sum2 adding the table's centre.)
- **Sum** → **Wrap**: the desired heading `θ_d = ωt + φ`, wrapped to (−π, π]: each platform points along the radius, outwards.
- **Mux** `[x_d y_d θ_d]` → `BLACK_Desired_States`.

**Desired Joint Angles RED** in Sub-Phase #1: `[π/2, 0, 0]`: the position controller unfolds the arm's elbow and wrist (shoulder stays at 90°). In Sub-Phases 2–4 the arm's desired angles return to `[π/2, 0, π/2]`, with its controller off.

## Phase #4: Return Home and Phase #5: Hold Home

Like phase 2 with `home_states_X` (the Platforms pane's **Home** pose) and the arm's folded pose: the platforms fly back to a parking place and hold it, so that nothing drifts while the pucks are switched off in phase 6.

## Making it yours

The phases' *structure* (synchronise, float, go to the start, experiment, go home, hold, shut down) is what the lab procedure needs; keep it. What you change:
- the sub-phases' desired states (replace the circle with your trajectory, or set `X_Path_Planner_Selection` to 1 or 2 to hand it to a [Guidance](page:diagram-guidance) path planner you write),
- the `X_Control_Law_Enabler` values (2 or 3 select your custom controllers in [4 - Control](page:diagram-control); for the arm 1 = position, 2 = custom/torque, 3 = velocity, 0 = off),
- the String Constants (what the Messages panel says in each phase),
- the durations, on the Simulation tab's Timeline.
