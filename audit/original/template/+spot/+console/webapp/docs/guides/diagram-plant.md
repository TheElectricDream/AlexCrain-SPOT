# Simulate Plant Dynamics

**Runs only in a simulation** (`isSim == 1`), from the If block at the top level that sits between the hardware inputs (priority 25) and [1 - Experiment Logic](page:diagram-experiment-logic) (30). It does what the laboratory does for an experiment: moves the platforms under the thruster forces and measures them, at the camera's rate, with noise.

![Simulate Plant Dynamics](images/sub_Simulate_Plant_Dynamics.png)

**Reads:** `RED/BLACK/BLUE_Saturated_Controls`, `ARM_Controls`, `Univ_Time`, `Float_State` (last step's: [1 - Experiment Logic](page:diagram-experiment-logic) writes it later in the step, so this store's read-before-write and write-after-read checks are off; [Data Stores](page:data-stores)). **Writes:** `X_True_States`, `X_Measured_States` for the three platforms, and `ARM_True_States`, `ARM_Measured_States`, `ARM_Measured_PWM`. The maths is on [Platform Dynamics and LQR](page:math-dynamics-lqr) and [The Arm](page:math-arm).

Three subsystems, one per platform; **BLACK Dynamics** and **BLUE Dynamics** are identical apart from their names, and **RED and ARM Dynamics** chooses between RED alone and RED with the arm.

## BLACK Dynamics

![BLACK Dynamics](images/sub_Simulate_Plant_Dynamics_BLACK_Dynamics.png)

1. **Saturated Control Commands for BLACK** reads `BLACK_Saturated_Controls`, the force and torque the thrusters can actually give (what [Thruster & Float Control Code](page:diagram-thrusters) wrote last step, in the table frame).
2. **Dynamics Model for BLACK**: the MATLAB Function `BLACK_dynamics(control_inputs, position, velocity, floating, mBLACK, IBLACK, tableSlope, tableDamping_BLACK)` computes the accelerations of a free body in the plane,

$$\ddot x = \frac{F_x}{m}, \qquad \ddot y = \frac{F_y}{m}, \qquad \ddot\theta = \frac{T_z}{I},$$

   and, while the platform floats (`floating`, from a Data Store Read **Float State** of `Float_State`, input 4, is not 0), adds the table's pull to $\ddot x, \ddot y$: `spot.sim.tableSlope` of the position and velocity, with `tableSlope` (the table slope calibration; all zeros on a level table) and `tableDamping_BLACK` (the air bearings' drag [1/s]) from [Workspace Variables](page:workspace-variables). Then two **Discrete-Time Integrators** (Backward Euler): *Acceleration to Velocity* (initial condition 0) and *Velocity to Position* (initial condition `init_states_BLACK`, the Initial pose from the settings, in m and rad). The output is the true `[x y θ]`. The position and velocity go back into the function (inputs 2 and 3) through two **Unit Delays**, *Previous Position* (initial condition `init_states_BLACK(:)`) and *Previous Velocity* (initial condition `[0; 0; 0]`): Backward Euler integrators pass this step's input straight through, so without the delays the loop would be algebraic. With the Table Slope calibration not used (its card's switch off on the Calibration pane), or no calibration in the project, the table is level and there is no friction and no drag, which an air-bearing platform on granite is very close to.
3. **Calculated & Store True States**: the true 9-state vector for the record. **Demux** splits `[x y θ]`; θ goes through **Wrap** (`atan2(sin θ, cos θ)`); **Discrete Derivative** and **Discrete Derivative1** (Simulink's `(u_k − u_{k−1}) / T_s` blocks) give the velocities and accelerations; **Mux2** = `[x y θ_wrapped ẋ ẏ θ̇ ẍ ÿ θ̈]` → `BLACK_True_States`.
4. **Add Noise** (a Random Number block) + **Sum**: measurement noise added to the true pose, as the camera's jitter would be.
5. **Data @ Measurement Rate**: the camera's **sample-and-hold**. The noisy pose only changes every `dataRate` seconds, however fast the diagram steps:
   - **Data Store Read** (`Univ_Time`) − the time of the last held sample (**Enabled Subsystem**, which latches the time) = **Subtract**, the age of the held sample; **Delay** → **Relational Operator1** `≥ dataRate` re-enables the latch when the sample is old enough.
   - **Relational Operator2** `== 0`: on the step where a new time was latched, the age is zero, so **Enabled Subsystem1** passes the new *Data Input* through; on every other step it holds its output (*Data Held*).
   - **Calculate True Time Step**: the same block as in [PhaseSpace Camera Code](page:diagram-phasespace): the interval between the held samples (`CheckforInvalidTime` makes it `dataRate` when it would be ≤ 0) → *Time Step*.
6. **Calculated & Store Measured States**: the held pose → **Demux**, x and y straight into **Mux1**, θ through **Wrap**; **Subsystem2** and **Subsystem1** are the EMA derivatives (τ = 0.1 s, `Constants.LOWPASS_TIME_CONSTANT`, running only when the input changed, with the true time step) for the velocities and, from them, the accelerations; **Mux2** = `[pose; velocities; accelerations]` → `BLACK_Measured_States`.

So a controller in a simulation sees exactly what it sees in the lab: positions at the camera's rate, with noise, velocities and accelerations estimated from them with the same filter and the same lag.

## RED and ARM Dynamics

![RED and ARM Dynamics](images/sub_Simulate_Plant_Dynamics_RED_and_ARM_Dynamics.png)

- **Constant** `platformSelection` → **If** `u1 == 4 | u1 == 5 | u1 == 10 | u1 == 11`: the codes in which the Arm is ticked together with RED (`spot.params.platformSelection`) run **RED + ARM**; anything else runs **RED Only**.
- **RED Only** is BLACK Dynamics with RED's names (`RED_dynamics(control_inputs, position, velocity, floating, mRED, IRED, tableSlope, tableDamping_RED)`, `init_states_RED`).

### RED + ARM

![RED + ARM](images/sub_Simulate_Plant_Dynamics_RED_and_ARM_Dynamics_RED_ARM.png)

1. **RED + ARM Plant**: the MATLAB System block `spot.arm.Plant` (with `FloatInput` on), with three inputs, `RED_Saturated_Controls` (the thruster force and torque), `ARM_Controls` (the three joint torques) and, from a Data Store Read **Float State**, `Float_State` (port *Float*: the table's pull acts only while the platform floats), and three outputs: the generalised positions `[x y θ θ1 θ2 θ3]`, their rates, and the torques the servos would report. Inside, every diagram step is divided into 1 ms sub-steps (`Constants.ARM_PLANT_INNER_STEP`) of the coupled base+arm dynamics (the inertia and Coriolis matrices of `spot.arm.inertiaMatrix` and `spot.arm.coriolisMatrix`), the servos' current control with back-EMF, the stick–slip friction ([The Arm: Dynamics, Friction, Servos](page:math-arm)), and the table's slope and the air bearings' drag on the whole platform's mass (`armModel.tableSlope`, `armModel.tableDamping`).
2. **Mux** `[positions; rates]` → **Demux** `[1, 1, 10]`: x and y go to **Exceeded Table Limits**, which is `x ≥ xLength | x < 0 | y ≥ yLength | y < 0` (four Relational Operators and three ORs) → **Stop Simulation**: a simulation whose Chaser leaves the 3.51155 m × 2.41935 m table (about 3.5 × 2.4; `xLength`, `yLength` from `Constants.TABLE_X`, `TABLE_Y`) stops there, as the real one would hit the barrier. (The other platforms are not checked.)
3. **Joint Angles** and **Joint Rates** (Selectors `[4 5 6]`) take the arm's part; **Encoder Pulses** (a Quantizer, 2π/4096 rad; `PULSES_PER_REV`) and **Velocity Units** (a Quantizer, 0.229 rpm = 0.229·2π/60 rad/s; `VEL_UNIT_RPM`) round them to what the real servos report ([Dynamixel Units](page:math-dynamixel)); **Mux Encoders** → `ARM_Measured_States`. The plant's third output, the PWM each servo applies → `ARM_Measured_PWM`. These two stores are exactly what [Robotic Arm Code](page:diagram-arm-code) writes in an experiment, so the arm controllers see the same thing in both.
4. The base's true pose goes through the same **Add Noise → Data @ Measurement Rate → Calculated & Store Measured States** chain as BLACK's (writing `RED_Measured_States`; the arm part of the measured vector is terminated, "stored by the encoder path"), and **Calculated & Store True States** writes `RED_True_States` and, from the joint part, `ARM_True_States`.
