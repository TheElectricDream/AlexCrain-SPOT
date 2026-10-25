# Robotic Arm Code

**Priority 21.** On RED, during an experiment: send the joint PWM to the three Dynamixel servos and read back what they measure. In a simulation this subsystem does nothing; `spot.arm.Plant` inside [Simulate Plant Dynamics](page:diagram-plant) plays the servos.

![Robotic Arm Code](images/sub_Robotic_Arm_Code.png)

**Input:** *PWM* (3, 885 = 100 % of the supply), wired from [Arm PWM (After Control)](page:diagram-arm-torques), i.e. the `ARM_Controls` written by [4 - Control](page:diagram-control) in the **previous** step. **Writes:** `ARM_Measured_States`, `ARM_Measured_PWM`, `ARM_Servo_Status`. **Reads:** `WhoAmI`, `isSim`.

- **Data Store Read1** (`WhoAmI`), **Data Store Read2** (`isSim`) → **If** `u1 == 1 & u2 == 0`: RED, and an experiment.
- **Change ARM Behavior** (the action):

![Change ARM Behavior](images/sub_Robotic_Arm_Code_Change_ARM_Behavior.png)

- **Dynamixel Arm (PWM)**: the MATLAB System block `spot.hardware.ArmPWM`. Each step it sends the three commands as Goal PWM (whole units within the PWM Limit, none pushing a joint past its position limits; see [Dynamixel Units](page:math-dynamixel)) to the servos over the USB bus (a group sync write), and reads back the present PWM, velocities and positions (a group sync read). Its four outputs:
  1. the joint angles (rad, 2π/4096 per encoder count),
  2. the joint rates (rad/s, 0.229 rpm per unit),
  3. the PWM each servo applies (its Present PWM, 885 = 100 %),
  4. the status 5-vector: servos read this step, the read's duration (ms), the USB latency timer (ms), reads missed, and an alert flag.
- **Mux** joins angles and rates into the 6-vector → **Data Store Write** `ARM_Measured_States`; the applied PWM → `ARM_Measured_PWM`; the status → `ARM_Servo_Status`. All three are logged ([Logged Signals](page:logged-signals)).

Why the block replaced the old Position/Velocity Dynamixel blocks, and how the read rate went from about 2 Hz to the bus's limit, is told on `spot.hardware.ArmPWM` and [arm_pwm.cpp](file:arm_pwm.cpp): the servos run in **PWM (voltage) control** for the whole experiment, and position or velocity control are controllers in [4 - Control](page:diagram-control) that output PWM. At rest a PWM acts as a torque ([The Arm](page:math-arm)); the servos' current readings, too coarse at the arm's torques, are not used.
