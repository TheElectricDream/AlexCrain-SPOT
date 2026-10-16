# Arm PWM (After Control)

**Priority 61.** Two blocks: a **Data Store Read** of `ARM_Controls` (each joint's PWM) and an **Outport** *PWM*. (Until October 2026, when the servos ran in current control, it was *Arm Torques (After Control)*; the picture still shows that name.)

![Arm PWM (After Control)](images/sub_Arm_Torques_After_Control.png)

It exists for the step order. [Robotic Arm Code](page:diagram-arm-code) (priority 21) must run early, with the measurements, so the arm's states are fresh for the controller. But the PWM it sends is computed by [4 - Control](page:diagram-control) (priority 50), later in the step. If Robotic Arm Code read `ARM_Controls` itself, Simulink would flag a *read before write* and the value would be last step's anyway. Reading the store here, after Control has written it, and wiring the result into Robotic Arm Code's inport makes the timing explicit: the PWM computed at step k is written to the servos at the start of step k + 1, one step of delay, the same as the simulated plant's reading of `ARM_Controls` inside [Simulate Plant Dynamics](page:diagram-plant). (The hardware block has a **ZeroReturnDelay** option that reads the servos after writing, for the case where that delay should be removed on both sides.)

This is the only wire between top-level subsystems; everything else goes through [data stores](page:data-stores).
