# Workspace Variables

The Simulink diagram reads its parameters from MATLAB's **base workspace**. [[spot.params.apply|spot-params-apply]] puts them there from the settings (every time a setting changes and before each simulation), then runs `SPOT_User_Parameters.m`, where you can add or override anything. When the diagram is built for the platforms, the same variables are compiled into the programs.

| Variable | From | Meaning |
|---|---|---|
| `platformSelection` | `platforms` | code 0–11 for which platforms take part ([[spot.params.platformSelection|spot-params-platformSelection]]) |
| `Phase0_Duration` … `Phase5_Duration`, `Phase3_SubPhase1_Duration` … `_4_Duration` | `timing` | each segment's length [s] (0 when switched off) |
| `Phase0_End` … `Phase5_End`, `Phase3_SubPhase1_End` … `_4_End` | `timing` | when each segment ends [s] from the start |
| `tsim` | `timing` | total simulation / experiment time [s] |
| `baseRate` | `sampleRate` | the diagram's step [s] (= 1/sampleRate) |
| `dataRate` | `dataRate` | the PhaseSpace period [s] (= 1/dataRate), the LQR design step |
| `PWMFreq` | `pwmFrequency` | the thrusters' PWM frequency [Hz] |
| `labConsoleIP` | `labConsoleAddress` | the lab console's IPv4 address as `[192 168 1 116]` (the UDP blocks can't take text) |
| `xLength`, `yLength` | constants | the table's size, 3.51155 × 2.41935 m |
| `mRED`, `mBLACK`, `mBLUE` | `mass` | each platform's mass [kg] |
| `IRED`, `IBLACK`, `IBLUE` | `mass` | each platform's inertia about z [kg m²] |
| `thruster_dist2CG_RED/BLACK/BLUE` | `mass` | 8×1 distances of each thruster's line of action from the CG [mm] |
| `Phi`, `b0`, `a1`, `b1`, `a2`, `b2`, `a3`, `b3` | `mass.arm` (moved by `mount`) | the arm's geometry [rad, m] ([[spot.arm.mountGeometry|spot-arm-mountGeometry]]) |
| `mPROX`, `mDIST`, `mEE`, `IPROX`, `IDIST`, `IEE` | `mass.arm` | the links' masses [kg] and inertias [kg m²] |
| `armShoulderOffset` | `mass.arm.mount` | the angle the shoulder's zero is turned by for its mounting face [rad] |
| `tableSlope` | `calibration.tableSlope` + `resources/spot_calibration.json` | 1×7 `[a1 a2 a3 a4 a5 xc yc]`: the table top's height surface, for the table's pull on the floating platforms ([[spot.sim.tableSlope|spot-sim-tableSlope]]); `[0 0 0 0 0 1.75 1.2]` (level) with the setting off or no table calibration |
| `tableDamping_RED/BLACK/BLUE` | `calibration.tableSlope` + `resources/spot_calibration.json` | each platform's air-bearing drag [1/s]; 0 by default |
| `init_states_RED/BLACK/BLUE` | `initial` | `[x y θ]` [m, m, rad] |
| `home_states_RED/BLACK/BLUE` | `initial` | `[x y θ]` [m, m, rad] |
| `init_states_ARM` | `initial.ARM` | `[q1 q2 q3]` [rad] |
| `armModel` | `mass.arm`, `calibration.arm` + the project's arm calibration | the simulated arm: servos, friction, motor, internal step, the table's slope under the Chaser ([[spot.arm.parameters|spot-arm-parameters]]); with the setting on, the joints' friction and armature from the project's arm calibration (the `arm` section of `resources/spot_calibration.json`, or `resources/arm_calibration.json`, whichever was measured last), else the defaults |
| `armMotor` | `armModel.motor` | the servos' motor constants on their own, for the arm controller blocks |
| `F_red_X_nominal`, `F_red_Y_nominal` (and `black`, `blue`) | `thrusterForce` | thrust of one x / one y thruster [N], always the setting's; the diagram no longer reads them (kept for your own code) |
| `F_thrusters_RED/BLACK/BLUE` | `thrusterForce`, `calibration.thrusters` + `resources/spot_calibration.json` | 8×1 thrust of each thruster [N], which the diagram's thruster mixer (and so the simulated plant) uses: the project's thruster calibration for that platform when the setting is on and there is one ([[spot.params.loadCalibration|spot-params-loadCalibration]]), else `thrusterForce` for all eight ([[spot.control.nominalForces|spot-control-nominalForces]]) |
| `Qx_RED`, `Qy_RED`, `Qtheta_RED`, `QxD_RED`, `QyD_RED`, `QthetaD_RED` (and BLACK, BLUE) | `control` | LQR state weights |
| `RFx_RED`, `RFy_RED`, `RTz_RED` (…) | `control` | LQR input weights |
| `A_d_RED`, `B_d_RED`, `C_d_RED`, `D_d_RED` (…) | computed | the discrete platform model at `dataRate` ([[spot.control.stateSpace|spot-control-stateSpace]]) |
| `K_RED`, `K_BLACK`, `K_BLUE` | computed | the 3×6 LQR gains ([[spot.control.lqrGains|spot-control-lqrGains]]) |
| `armControl` | `control.ARM` | the arm controllers' gains and limits, `.position.P/I/D` (1×3 each), `.position.profileVelocity/profileAcceleration/pwmLimit`, `.velocity.P/I` (1×3), `.velocity.profileAcceleration/pwmLimit`; read by *Default Position Control (ARM)* and *Default Velocity Control (ARM)* |
| `simMode` | [[spot.sim.run|spot-sim-run]] | 1 while simulating, 0 otherwise; the diagram simulates the plant only when it is 1 |
| `WhoAmI` | [[spot.sim.run|spot-sim-run]] | 0 in a simulation; on a platform the Platform Identification block sets it (1 RED, 2 BLACK, 3 BLUE) |
| `Kp`, `Kv` | `SPOT_User_Parameters.m` | the arm's transpose-Jacobian gains (an example of a user variable) |

After a simulation, [[spot.sim.run|spot-sim-run]] also leaves `simOut` (Simulink's output), `dataClass` (every logged signal as a timeseries, see [[Logged Signals|Logged-Signals]]) and `dataPacket` (the same side by side, as the old GUI had it).

The Workspace pane of the Simulation tab lists these live, with a preview on click ([[spot.workspace|spot-workspace]]).
