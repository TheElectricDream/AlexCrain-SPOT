# Logged Signals

Every step, the [Data Logger Subsystem](page:diagram-data-logger) records the **108 signals** below (118 columns: two are vectors). In a simulation they end up in the workspace as `dataClass` (a struct with one `timeseries` per signal, see [Workspace Variables](page:workspace-variables)) and in `Saved Data/<run>/` when you **Save Results**; in an experiment each platform writes them to its `ExpLog` file, which **Collect** turns into the same `dataClass` (`spot.experiment.decodeData`). The [Results tab](page:tab-results) plots any of them, grouped by platform; `spot.data.load` reads them in MATLAB.

**Names** are `<PLATFORM>_<quantity>_<unit>`: `Px`/`Py` position, `Rz` attitude about z, `V`/`RzD` first derivatives, `A`/`RzDD` second, `F`/`T` force and torque, `Sat` saturated, `Desired` the phase's goal, `Path` the guidance output; units `m`, `rad`, `mpers` (m/s), `mpers2`, `radpers`, `radpers2`, `N`, `Nm`. `spot.plotting.signalLabel` turns them into axis labels. The table is in the bus's order, which is also the column order of an `ExpLog` file (time first).

| # | Signal | Width | Unit | What it is |
|---|---|---|---|---|
| 1 | `Time_s` | 1 | s | `Univ_Time`: the experiment time (Simulinks clock in a simulation; the shared PhaseSpace clock in the lab) |
| 2 | `RED_Fx_N` | 1 | N | `RED_Controls(1)`: the force along table x the controller asked for |
| 3 | `RED_Fy_N` | 1 | N | `RED_Controls(2)`: along table y |
| 4 | `RED_Tz_Nm` | 1 | N·m | `RED_Controls(3)`: the torque |
| 5 | `RED_Fx_Sat_N` | 1 | N | `RED_Saturated_Controls(1)`: the force the thrusters can actually give |
| 6 | `RED_Fy_Sat_N` | 1 | N | `RED_Saturated_Controls(2)` |
| 7 | `RED_Tz_Sat_Nm` | 1 | N·m | `RED_Saturated_Controls(3)` |
| 8 | `RED_Px_m` | 1 | m | `RED_Measured_States(1)`: measured x (Chaser) |
| 9 | `RED_Py_m` | 1 | m | `RED_Measured_States(2)`: measured y |
| 10 | `RED_Rz_rad` | 1 | rad | `RED_Measured_States(3)`: measured attitude, wrapped to (−π, π] |
| 11 | `RED_Vx_mpers` | 1 | m/s | `RED_Measured_States(4)`: velocity (StreamDatas, or the plants held sample) |
| 12 | `RED_Vy_mpers` | 1 | m/s | `RED_Measured_States(5)` |
| 13 | `RED_RzD_radpers` | 1 | rad/s | `RED_Measured_States(6)`: attitude rate |
| 14 | `RED_Ax_mpers2` | 1 | m/s² | `RED_Measured_States(7)`: acceleration, the EMA derivative of the velocity |
| 15 | `RED_Ay_mpers2` | 1 | m/s² | `RED_Measured_States(8)` |
| 16 | `RED_RzDD_radpers2` | 1 | rad/s² | `RED_Measured_States(9)` |
| 17 | `RED_Px_Desired_m` | 1 | m | `RED_Desired_States(1)`: the phases goal |
| 18 | `RED_Py_Desired_m` | 1 | m | `RED_Desired_States(2)` |
| 19 | `RED_Rz_Desired_rad` | 1 | rad | `RED_Desired_States(3)` |
| 20 | `RED_IMU_Ax_mpers2` | 1 | m/s² | `RED_IMU_Measurements(1)`: zero unless you add an IMU |
| 21 | `RED_IMU_Ay_mpers2` | 1 | m/s² | `RED_IMU_Measurements(2)` |
| 22 | `RED_IMU_Gz_radpers` | 1 | rad/s | `RED_IMU_Measurements(3)` |
| 23 | `RED_Duty_Cycle` | 8 | 0–1 | `RED_Duty_Cycle(s)`: **8 columns**, the thrusters duty cycles |
| 24 | `RED_Control_Law_Enabler` | 1 | – | `RED_Control_Law_Enabler`: 0 off, 1 LQR, 2/3 custom |
| 25 | `RED_Px_Path_m` | 1 | m | `RED_Path(1)`: what the controller tracks |
| 26 | `RED_Py_Path_m` | 1 | m | `RED_Path(2)` |
| 27 | `RED_Rz_Path_rad` | 1 | rad | `RED_Path(3)` |
| 28 | `ARM_Control_Mode` | 1 | – | `ARM_Control_Mode`: the arm controller that ran (0 off, 1 position, 2 custom, 3 velocity) |
| 29 | `ARM_Shoulder_Command` | 1 | PWM (885 = 100 %, `Constants.PWM_FULL`) | `ARM_Controls(1)`: the shoulder's PWM command |
| 30 | `ARM_Elbow_Command` | 1 | N·m | `ARM_Controls(2)` |
| 31 | `ARM_Wrist_Command` | 1 | N·m | `ARM_Controls(3)` |
| 32 | `ARM_Rz_Shoulder_rad` | 1 | rad | `ARM_Measured_States(1)`: the shoulder angle (0 = straight out; 2π/4096 steps, `PULSES_PER_REV`) |
| 33 | `ARM_Rz_Elbow_rad` | 1 | rad | `ARM_Measured_States(2)` |
| 34 | `ARM_Rz_Wrist_rad` | 1 | rad | `ARM_Measured_States(3)` |
| 35 | `ARM_RzD_Shoulder_radpers` | 1 | rad/s | `ARM_Measured_States(4)`: the shoulder rate (0.229 rpm steps, `VEL_UNIT_RPM`) |
| 36 | `ARM_RzD_Elbow_radpers` | 1 | rad/s | `ARM_Measured_States(5)` |
| 37 | `ARM_RzD_Wrist_radpers` | 1 | rad/s | `ARM_Measured_States(6)` |
| 38 | `ARM_Shoulder_Path` | 1 | rad, rad/s or PWM | `ARM_Path(1)`: the arm controllers input (angle, speed or PWM by mode) |
| 39 | `ARM_Elbow_Path` | 1 | … | `ARM_Path(2)` |
| 40 | `ARM_Wrist_Path` | 1 | … | `ARM_Path(3)` |
| 41 | `ARM_Shoulder_Desired` | 1 | rad | `ARM_Desired_States(1)`: the phases goal |
| 42 | `ARM_Elbow_Desired` | 1 | rad | `ARM_Desired_States(2)` |
| 43 | `ARM_Wrist_Desired` | 1 | rad | `ARM_Desired_States(3)` |
| 44 | `BLACK_Fx_N` | 1 | N | `BLACK_Controls(1)`: the force along table x the controller asked for |
| 45 | `BLACK_Fy_N` | 1 | N | `BLACK_Controls(2)`: along table y |
| 46 | `BLACK_Tz_Nm` | 1 | N·m | `BLACK_Controls(3)`: the torque |
| 47 | `BLACK_Fx_Sat_N` | 1 | N | `BLACK_Saturated_Controls(1)`: the force the thrusters can actually give |
| 48 | `BLACK_Fy_Sat_N` | 1 | N | `BLACK_Saturated_Controls(2)` |
| 49 | `BLACK_Tz_Sat_Nm` | 1 | N·m | `BLACK_Saturated_Controls(3)` |
| 50 | `BLACK_Px_m` | 1 | m | `BLACK_Measured_States(1)`: measured x (Target) |
| 51 | `BLACK_Py_m` | 1 | m | `BLACK_Measured_States(2)`: measured y |
| 52 | `BLACK_Rz_rad` | 1 | rad | `BLACK_Measured_States(3)`: measured attitude, wrapped to (−π, π] |
| 53 | `BLACK_Vx_mpers` | 1 | m/s | `BLACK_Measured_States(4)`: velocity (StreamDatas, or the plants held sample) |
| 54 | `BLACK_Vy_mpers` | 1 | m/s | `BLACK_Measured_States(5)` |
| 55 | `BLACK_RzD_radpers` | 1 | rad/s | `BLACK_Measured_States(6)`: attitude rate |
| 56 | `BLACK_Ax_mpers2` | 1 | m/s² | `BLACK_Measured_States(7)`: acceleration, the EMA derivative of the velocity |
| 57 | `BLACK_Ay_mpers2` | 1 | m/s² | `BLACK_Measured_States(8)` |
| 58 | `BLACK_RzDD_radpers2` | 1 | rad/s² | `BLACK_Measured_States(9)` |
| 59 | `BLACK_Px_Desired_m` | 1 | m | `BLACK_Desired_States(1)`: the phases goal |
| 60 | `BLACK_Py_Desired_m` | 1 | m | `BLACK_Desired_States(2)` |
| 61 | `BLACK_Rz_Desired_rad` | 1 | rad | `BLACK_Desired_States(3)` |
| 62 | `BLACK_IMU_Ax_mpers2` | 1 | m/s² | `BLACK_IMU_Measurements(1)`: zero unless you add an IMU |
| 63 | `BLACK_IMU_Ay_mpers2` | 1 | m/s² | `BLACK_IMU_Measurements(2)` |
| 64 | `BLACK_IMU_Gz_radpers` | 1 | rad/s | `BLACK_IMU_Measurements(3)` |
| 65 | `BLACK_Duty_Cycle` | 8 | 0–1 | `BLACK_Duty_Cycle(s)`: **8 columns**, the thrusters duty cycles |
| 66 | `BLACK_Control_Law_Enabler` | 1 | – | `BLACK_Control_Law_Enabler`: 0 off, 1 LQR, 2/3 custom |
| 67 | `BLACK_Px_Path_m` | 1 | m | `BLACK_Path(1)`: what the controller tracks |
| 68 | `BLACK_Py_Path_m` | 1 | m | `BLACK_Path(2)` |
| 69 | `BLACK_Rz_Path_rad` | 1 | rad | `BLACK_Path(3)` |
| 70 | `BLUE_Fx_N` | 1 | N | `BLUE_Controls(1)`: the force along table x the controller asked for |
| 71 | `BLUE_Fy_N` | 1 | N | `BLUE_Controls(2)`: along table y |
| 72 | `BLUE_Tz_Nm` | 1 | N·m | `BLUE_Controls(3)`: the torque |
| 73 | `BLUE_Fx_Sat_N` | 1 | N | `BLUE_Saturated_Controls(1)`: the force the thrusters can actually give |
| 74 | `BLUE_Fy_Sat_N` | 1 | N | `BLUE_Saturated_Controls(2)` |
| 75 | `BLUE_Tz_Sat_Nm` | 1 | N·m | `BLUE_Saturated_Controls(3)` |
| 76 | `BLUE_Px_m` | 1 | m | `BLUE_Measured_States(1)`: measured x (Obstacle) |
| 77 | `BLUE_Py_m` | 1 | m | `BLUE_Measured_States(2)`: measured y |
| 78 | `BLUE_Rz_rad` | 1 | rad | `BLUE_Measured_States(3)`: measured attitude, wrapped to (−π, π] |
| 79 | `BLUE_Vx_mpers` | 1 | m/s | `BLUE_Measured_States(4)`: velocity (StreamDatas, or the plants held sample) |
| 80 | `BLUE_Vy_mpers` | 1 | m/s | `BLUE_Measured_States(5)` |
| 81 | `BLUE_RzD_radpers` | 1 | rad/s | `BLUE_Measured_States(6)`: attitude rate |
| 82 | `BLUE_Ax_mpers2` | 1 | m/s² | `BLUE_Measured_States(7)`: acceleration, the EMA derivative of the velocity |
| 83 | `BLUE_Ay_mpers2` | 1 | m/s² | `BLUE_Measured_States(8)` |
| 84 | `BLUE_RzDD_radpers2` | 1 | rad/s² | `BLUE_Measured_States(9)` |
| 85 | `BLUE_Px_Desired_m` | 1 | m | `BLUE_Desired_States(1)`: the phases goal |
| 86 | `BLUE_Py_Desired_m` | 1 | m | `BLUE_Desired_States(2)` |
| 87 | `BLUE_Rz_Desired_rad` | 1 | rad | `BLUE_Desired_States(3)` |
| 88 | `BLUE_IMU_Ax_mpers2` | 1 | m/s² | `BLUE_IMU_Measurements(1)`: zero unless you add an IMU |
| 89 | `BLUE_IMU_Ay_mpers2` | 1 | m/s² | `BLUE_IMU_Measurements(2)` |
| 90 | `BLUE_IMU_Gz_radpers` | 1 | rad/s | `BLUE_IMU_Measurements(3)` |
| 91 | `BLUE_Duty_Cycle` | 8 | 0–1 | `BLUE_Duty_Cycle(s)`: **8 columns**, the thrusters duty cycles |
| 92 | `BLUE_Control_Law_Enabler` | 1 | – | `BLUE_Control_Law_Enabler`: 0 off, 1 LQR, 2/3 custom |
| 93 | `BLUE_Px_Path_m` | 1 | m | `BLUE_Path(1)`: what the controller tracks |
| 94 | `BLUE_Py_Path_m` | 1 | m | `BLUE_Path(2)` |
| 95 | `BLUE_Rz_Path_rad` | 1 | rad | `BLUE_Path(3)` |
| 96 | `Float_State` | 1 | 0/1 | `Float_State`: 1 = pucks on |
| 97 | `Platform_Identification` | 1 | – | `WhoAmI`: 1 RED, 2 BLACK, 3 BLUE, 0 simulation |
| 98 | `Simulation_Flag` | 1 | 0/1 | `isSim` |
| 99 | `VIS_Measured_States` | 3 | m, m, rad | `VIS_Measured_States`: **3 columns**, the Orins pose estimate |
| 100 | `VIS_Confidence` | 1 | – | `VIS_Confidence` |
| 101 | `ARM_Shoulder_PWM` | 1 | PWM (885 = 100 %) | `ARM_Measured_PWM(1)`: the PWM the servo applies (its Present PWM) |
| 102 | `ARM_Elbow_PWM` | 1 | PWM | `ARM_Measured_PWM(2)` |
| 103 | `ARM_Wrist_PWM` | 1 | PWM | `ARM_Measured_PWM(3)` |
| 104 | `ARM_Servos_Read` | 1 | count | `ARM_Servo_Status(1)`: servos answered this step (3 = all) |
| 105 | `ARM_Servo_Read_ms` | 1 | ms | `ARM_Servo_Status(2)`: how long the bus read took |
| 106 | `ARM_USB_Latency_ms` | 1 | ms | `ARM_Servo_Status(3)`: the USB adapters latency timer (1 after setup) |
| 107 | `ARM_Servo_Reads_Missed` | 1 | count | `ARM_Servo_Status(4)`: reads that failed so far |
| 108 | `ARM_Servo_Alert` | 1 | 0/1 | `ARM_Servo_Status(5)`: a servo raised a hardware error alert |

## Notes

- **Measured, not true.** The platform states logged are the *measured* ones (camera, or the simulated camera: held at the data rate, with noise). The simulation's exact states (`X_True_States`) are not logged; read the store yourself if you need them (a Data Store Read into a Live Stream block, or another To Workspace).
- **Zeros** are normal in `X_IMU_*` (no IMU in the template), `VIS_*` (no Orin running), and every signal of a platform that is not part of the scenario; the Results tab greys those out.
- **The arm's `Path`** changes meaning with the controller: joint angles for position control, joint speeds for velocity control, PWM for the custom (PWM) controller; `ARM_Control_Mode` says which.
- Adding a signal is described on the [Data Logger](page:diagram-data-logger) page. This list is the template diagram's; a diagram you have changed may log more.
