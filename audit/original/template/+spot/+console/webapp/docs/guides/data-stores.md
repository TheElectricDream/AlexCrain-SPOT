# Data Stores

The diagram's subsystems share their results through **data stores**: 49 named global variables, each declared by a *Data Store Memory* block at the top level (the small blocks along the bottom of [the top level](page:diagram-overview)) and read or written anywhere with *Data Store Read* / *Data Store Write* blocks. This page lists them: what each holds, who writes it and who reads it. `X` stands for `RED`, `BLACK` or `BLUE`; the arm's stores are separate.

Units are SI: metres, radians, seconds, newtons, newton-metres. States are in the **table frame** (origin at a table corner, x along 3.5 m, y along 2.4 m, θ counter-clockwise from +x), controls `[Fx Fy Tz]` too; only inside [Thruster & Float Control Code](page:diagram-thrusters) are forces rotated into the body frame.

## The flags and the clock

| Store | Size | Initial | Written by | Read by | Meaning |
|---|---|---|---|---|---|
| `isSim` | 1 | 0 | [Simulation Status](page:diagram-status) | nearly everything | 1 in a simulation, 0 on a platform (`simMode`) |
| `WhoAmI` | 1 | 0 | [Platform Identification](page:diagram-status) | the hardware branches, Debugger, Live Streams, Data Logger | 1 RED, 2 BLACK, 3 BLUE; 0 in a simulation |
| `Univ_Time` | 1 | 0 | [PhaseSpace Camera Code](page:diagram-phasespace) (Simulink's clock, or the experiment clock from the stream) | Experiment Logic, the sample-and-holds, Debugger, Live Streams, Send Data to Orin, Data Logger | the time [s] every phase decision is made on; the same on every platform |
| `Float_State` | 1 | 0 | [1 - Experiment Logic](page:diagram-experiment-logic) | [Simulate Plant Dynamics](page:diagram-plant) (the table's slope acts only while floating; last step's value), Thruster & Float Control Code, Data Logger | 1 = air bearings on (the platform floats) |

## Per platform (RED, BLACK, BLUE)

| Store | Size | Initial | Written by | Read by | Meaning |
|---|---|---|---|---|---|
| `X_Measured_States` | 9 | zeros | PhaseSpace Camera Code (experiment) or [Simulate Plant Dynamics](page:diagram-plant) (simulation) | 4 - Control, Thruster & Float Control Code (θ), Live Data Streams, Send Data to Orin, Data Logger | `[x y θ ẋ ẏ θ̇ ẍ ÿ θ̈]` as measured: camera positions, StreamData's velocities, EMA-filtered accelerations |
| `X_True_States` | 9 | zeros | Simulate Plant Dynamics | nobody (not logged; for your own analysis) | the simulated truth, without noise or the camera's hold |
| `X_Desired_States` | 3 | zeros | 1 - Experiment Logic | 3 - Guidance, Data Logger | `[x_d y_d θ_d]`: where the current phase wants the platform |
| `X_Path` | 3 | zeros | [3 - Guidance](page:diagram-guidance) | 4 - Control, Data Logger | the state to track now (= desired, unless a path planner is on) |
| `X_Controls` | 3 | zeros | [4 - Control](page:diagram-control) | Thruster & Float Control Code, Data Logger | `[Fx Fy Tz]` the controller asks for, table frame |
| `X_Saturated_Controls` | 3 | zeros | Thruster & Float Control Code | Simulate Plant Dynamics, Data Logger | `[Fx Fy Tz]` the thrusters can give (after the mixer, the saturation and the PWM hold) |
| `RED_Duty_Cycle`, `BLACK_Duty_Cycles`, `BLUE_Duty_Cycles` | 8 | zeros | Thruster & Float Control Code | Data Logger | the eight thrusters' duty cycles, 0–1 |
| `X_Control_Law_Enabler` | 1 | 0 | 1 - Experiment Logic | 4 - Control, Data Logger | which controller: 0 off, 1 default LQR, 2 Custom #1, 3 Custom #2 (RED) |
| `X_Path_Planner_Selection` | 1 | 0 | 1 - Experiment Logic | 3 - Guidance | which path planner: 0 none (pass-through), 1 / 2 custom |
| `X_Logger` | string | | 1 - Experiment Logic | [Debugger](page:diagram-debugger) | the platform's message for the console's Messages panel |
| `X_IMU_Measurements` | 3 | zeros | nobody in the template | Data Logger | `[a_x a_y ω_z]` from an IMU, if you add one |

## The arm

| Store | Size | Initial | Written by | Read by | Meaning |
|---|---|---|---|---|---|
| `ARM_Measured_States` | 6 | zeros | [Robotic Arm Code](page:diagram-arm-code) (experiment) or Simulate Plant Dynamics (simulation) | 4 - Control, Data Logger | `[θ1 θ2 θ3 θ̇1 θ̇2 θ̇3]` as the servos report them (encoder counts, 0.229 rpm units) |
| `ARM_True_States` | 6 | zeros | Simulate Plant Dynamics | nobody | the simulated truth |
| `ARM_Measured_PWM` | 3 | zeros | Robotic Arm Code / the plant | Data Logger | the PWM each servo applies (its Present PWM) [885 = 100 %] |
| `ARM_Servo_Status` | 5 | zeros | Robotic Arm Code | Data Logger | servos read this step, read duration [ms], USB latency [ms], reads missed, alert |
| `ARM_Desired_States` | 3 | zeros | 1 - Experiment Logic | 3 - Guidance, Data Logger | the phase's goal: joint angles [rad] (or speeds / PWM, for those controllers) |
| `ARM_Path` | 3 | zeros | 3 - Guidance | 4 - Control, Data Logger | what the arm controller tracks now |
| `ARM_Controls` | 3 | zeros | 4 - Control | [Arm PWM (After Control)](page:diagram-arm-torques), Simulate Plant Dynamics, Data Logger | the three joints' PWM [885 = 100 % of the supply] |
| `ARM_Control_Law_Enabler` | 1 | 0 | 1 - Experiment Logic | 4 - Control | 0 off, 1 position, 2 custom (PWM), 3 velocity |
| `ARM_Path_Planner_Selection` | 1 | 0 | 1 - Experiment Logic | 3 - Guidance | 0 none, 2 custom |
| `ARM_Control_Mode` | 1 | 0 | 4 - Control | Data Logger | the controller that actually ran this step |

## The vision computer

| Store | Size | Initial | Written by | Read by | Meaning |
|---|---|---|---|---|---|
| `VIS_Measured_States` | 3 | zeros | [Receive Data from Orin](page:diagram-orin-receive) | Data Logger | the Orin's pose estimate `[x y θ]` |
| `VIS_Confidence` | 1 | 0 | Receive Data from Orin | Data Logger | its confidence |

## Rules of thumb

- A store is a **shared variable**, not a signal: a Data Store Read gives the latest value written *so far in this step* (or last step's, if the writer runs later). The [step order](page:diagram-overview) is arranged so that each store is written before its readers run; keep new blocks inside the subsystem whose priority fits.
- Simulink warns about a read before a write or a write after a read within a step (*Diagnostics → Data Validity → Data Store Memory block*). The template compiles with none. Some stores are read before they are written on purpose and have these checks off, with the reason in their Data Store Memory block's description; for example `X_Saturated_Controls` and `ARM_Controls` (the simulated plant uses last step's command), `Float_State` (Simulate Plant Dynamics, priority 25, reads it a step before 1 - Experiment Logic, 30, writes it), and, for read-before-write only, the stores written only in some phases or only in experiments (`X_Desired_States`, `WhoAmI`, `VIS_*`, …). If you add a store, set its block's diagnostics the same way and check the warnings after a run (they appear in the console's command window).
- To add a store: a *Data Store Memory* block at the top level with its name, initial value and (for a vector) its length; a *Data Store Write* where the value is made; *Data Store Read*s where it is used; and, if you want it recorded, a column in the [Data Logger](page:diagram-data-logger).
