# 4 - Control

**Priority 50.** The controllers: for each platform, pick a control law by `X_Control_Law_Enabler`, compute the force and torque (or the arm's joint torques) that drive the measured state to the path, and write it to `X_Controls`.

![4 - Control](images/sub_4_Control.png)

**Reads:** `X_Control_Law_Enabler`, `X_Path`, `X_Measured_States`. **Writes:** `RED_Controls`, `BLACK_Controls`, `BLUE_Controls` (`[Fx Fy Tz]` in the **table frame**), `ARM_Controls` (three torques), `ARM_Control_Mode`.

## The selectors

| Platform | If block | 1 | 2 | 3 | else |
|---|---|---|---|---|---|
| RED | `If` (4 branches) | Default LQR Control (RED) | Custom Control #1 (RED) | Custom Control #2 (RED) | Disable Thrusters (RED) |
| BLACK | `If1` (3) | Default LQR Control (BLACK) | Custom Control #1 (BLACK) | | Disable Thrusters (BLACK) |
| BLUE | `If2` (3) | Default LQR Control (BLUE) | Custom Control #1 (BLUE) | | Disable Thrusters (BLUE) |
| ARM | `If3` (4) | Default Position Control (ARM) | Custom Control (ARM) | Default Velocity Control (ARM) | Disable Control (ARM) |

A **Merge** after each set passes whichever ran to the **Data Store Write**.

## Default LQR Control (RED, BLACK, BLUE)

![Default LQR Control (RED)](images/sub_4_Control_Default_LQR_Control_RED.png)

The control law is $u = -K\,(x - x_d)$ with the six-state error and the 3 × 6 gain $K$ computed by [[spot.control.lqrGains|spot-control-lqrGains]] from the Control pane's weights (the derivation is on [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]]).

- **Data Store Read13** `X_Measured_States` → **Demux** `[1 1 1 1 1 1 3]`: x, y, θ, ẋ, ẏ, θ̇ (the accelerations are terminated).
- **Data Store Read1** `X_Path` → **Demux1**: x_d, y_d, θ_d.
- Position errors: **Subtract5** `x − x_d`, **Subtract4** `y − y_d`; the attitude error through **Calculate Error**, the MATLAB Function `SmoothAngularDifference`:

$$e_\theta = \operatorname{mod}(\theta - \theta_d + \pi,\ 2\pi) - \pi$$

  the shortest way round, so a platform at +170° asked for −170° turns 20°, not 340°. **Mux1** = `[e_x e_y e_θ]`.
- Velocity errors: the path's velocity is taken as the **Discrete Derivative** of the path (three of them, `(u_k − u_{k−1})/T_s`), and **Subtract7/6/8** give `ẋ − ẋ_d`, `ẏ − ẏ_d`, `θ̇ − θ̇_d`. For a constant target the derivative is 0 and the controller damps the velocity to zero; for a moving target (the circle) it feeds the target's velocity forward. **Mux3**.
- **Reshape1**, **Reshape3**, **Mux2**: the 6 × 1 error vector `[e_x e_y e_θ ė_x ė_y ė_θ]`.
- **Constant2** = `-K_RED` (the workspace gain, negated) → **Matrix Multiply** with the error → **Reshape4** → the output *u* = `[Fx Fy Tz]` in the table frame (N, N, N·m).

Rotating the force into the body frame and turning it into eight duty cycles happens in [[Thruster & Float Control Code|Diagram-Thruster-and-Float-Control-Code]]; the controller only asks for a force.

## Custom Control #1 / #2 (RED, BLACK, BLUE)

![Custom Control #1 (RED)](images/sub_4_Control_Custom_Control_1_RED.png)

Placeholders: a **Mux** of three **Zero** constants, i.e. `[0 0 0]`. Replace the inside with your controller: read `X_Measured_States` and `X_Path` (and anything else), output `[Fx Fy Tz]` in the table frame, and have the phase that should use it write 2 (or 3 for RED) to `X_Control_Law_Enabler`. [[spot.control.verifyConsoleController|spot-control-verifyConsoleController]] is an example of checking a MATLAB re-implementation of the default one against the diagram.

## Disable Thrusters (RED, BLACK, BLUE)

`[0 0 0]`: no force. The thruster code then opens no valve. This is what runs in phases 0, 1 and 6.

## The arm's controllers

The servos run in current (torque) control throughout ([[Robotic Arm Code|Diagram-Robotic-Arm-Code]]); position and velocity control are controllers **here** that output torques, replicas of the servos' own control laws retuned for the diagram's 20 Hz step ([[The Arm|Math-Arm]]). Each branch also writes its number to `ARM_Control_Mode` (logged), so the record says which law produced each torque.

### Default Position Control (ARM): mode 1

![Default Position Control (ARM)](images/sub_4_Control_Default_Position_Control_ARM.png)

**Dynamixel Position Control**, the MATLAB System block [[spot.arm.PositionControl|spot-arm-PositionControl]], with inputs `ARM_Measured_States` (Data Store Read1) and `ARM_Path` (Data Store Read2; the goal joint angles in rad). Inside: the servo's profile (the desired position moves towards the goal at Profile Velocity), a PID on the position error with the servo's structure (P/I/D 25 56 21 / 10 / 1545 230 236 for the shoulder/elbow/wrist at 20 Hz), the PWM limit, and the conversion of the PWM to the torque the motor would give. Its gains, profile and PWM limit come from the settings (the Control pane's arm cards, `armControl.position.*` on the block); the step time is `baseRate`. Output: three torques.

### Default Velocity Control (ARM): mode 3

![Default Velocity Control (ARM)](images/sub_4_Control_Default_Velocity_Control_ARM.png)

**Dynamixel Velocity Control**, [[spot.arm.VelocityControl|spot-arm-VelocityControl]], with `ARM_Path` as goal joint **speeds** (rad/s) and `ARM_Measured_States`: a PI on the speed error (P 160 70 30, I 220 1500 300), to torques. Its gains, profile acceleration and PWM limit are `armControl.velocity.*` (the Control pane).

### Custom Control (ARM): mode 2

![Custom Control (ARM)](images/sub_4_Control_Custom_Control_ARM.png)

**Torque control**, an example: **Calculate Torque** is the one-line MATLAB Function `y = u`: the three values in `ARM_Path` are sent straight to the servos as joint torques in N·m. Put your own control law here (a computed-torque controller using [[spot.arm.inertiaMatrix|spot-arm-inertiaMatrix]], an impedance controller, …), reading `ARM_Measured_States`, `ARM_Measured_Torque` and the base's states as you need. Torques beyond what the servos' Current Limit allows are clipped by the hardware block and the plant, and listed after a simulation by [[spot.arm.checkCommands|spot-arm-checkCommands]].

### Disable Control (ARM): mode 0

`[0 0 0]` N·m: the joints are free, held only by their friction.
