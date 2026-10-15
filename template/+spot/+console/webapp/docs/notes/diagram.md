<!-- Notes on the Simulink diagrams' MATLAB Function blocks. Each "## heading" is a block's path inside the diagram (without the diagram's name), so the notes apply to every project's copy of the template diagram. A local function's note is "## <block path> › <function>". -->

## Thruster & Float Control Code/Change BLUE Behavior/Calculate Duty Cycle/MATLAB Function1

`ThrusterSat` takes the mixer's eight duty cycles (0–1) and makes them ones the valves can give. If any is above 1, all eight are scaled down together so the largest is 1, which keeps the direction of the push. Then any duty cycle shorter than the valve's opening time (7 ms) within a PWM period (1/`PWMFreq`, 0.2 s at 5 Hz), so below 0.035, is set to 0: the valve couldn't open that briefly. The same code is in each platform's block.

## Data Logger Subsystem/Data Logger/MATLAB Function

`getSizeInput` returns how many values the Data Logger's input holds (the length of the bus vector), which the logger block needs as its second input.

## 4 - Control/Default LQR Control (BLACK)/Calculate Error/MATLAB Function

`SmoothAngularDifference` returns the attitude error, measured minus desired, wrapped to [−π, π): the shortest way round, so a platform at +170° asked for −170° turns 20°, not 340°. The same code is in each platform's controller, and in `spot.control.lqrCommand`.

## Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/Data @ Measurement Rate/Calculate True Time Step/MATLAB Function

`CheckforInvalidTime` returns the time since the last measurement, or the data rate's period (`dataRate`) when that difference is zero or negative (the first step, or two samples at the same time), so a velocity computed from it never divides by zero.

## 4 - Control/Custom Control (ARM)/Calculate Torque

`PlaceholderControl` passes its input straight through: the arm's custom controller (mode 2) in the template sends the path (`ARM_Path`) to the servos as torques. Replace it with your own torque controller ([4 - Control](page:diagram-control)).

## Simulate Plant Dynamics/BLACK Dynamics/Dynamics Model for BLACK/MATLAB Function

`BLACK_dynamics` is the Target's equations of motion in a simulation: its accelerations from the forces and torque the thrusters give, $\ddot x = F_x/m$, $\ddot y = F_y/m$, $\ddot\theta = T_z/I$, plus, while it floats, the table's pull and the air bearings' drag (`spot.sim.tableSlope`). The diagram integrates them ([Simulate Plant Dynamics](page:diagram-plant)). The Obstacle's block is the same.

## Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/Dynamics Model for RED/MATLAB Function

`RED_dynamics` is the Chaser's equations of motion when it flies without the arm: as the Target's (`BLACK_dynamics`), with the Chaser's mass and inertia. With the arm, the `RED + ARM Plant` block (`spot.arm.Plant`) simulates the Chaser instead.

## Thruster & Float Control Code/MATLAB Function2

`pack_command` builds the 37-byte packet the platform's Teensy reads every step: a sync byte (0xAA), the eight thrusters' duty cycles (0–100) as 4-byte floats, and the air bearings' state (0 or 1) as a 4-byte float. It is the layout the Teensy's firmware reads ([teensy_pwm_control.ino](file:teensy_pwm_control.ino)); `spot.hardware.packCommand` and the lab console's Hardware tab build the same packet.

## Thruster & Float Control Code/Change BLACK Behavior/Calculate Duty Cycle/Generate Initial Guess/MATLAB Function

`MakeH` builds the 3×8 matrix that maps the eight duty cycles to the force along the body's x and y and the torque: each thruster's direction (thrusters 1, 2 push −x, 5, 6 push +x, 3, 4 push +y, 7, 8 push −y), its lever arm about the centre of gravity (in mm, converted to m), and half its thrust ([The Thruster Mixer](page:math-mixer)). Its pseudo-inverse gives the mixer's first guess. The same code is in each platform's block, and in `spot.control.thrusterDutyCycles`.

## Thruster & Float Control Code/Change BLUE Behavior/Calculate Duty Cycle/Generate Initial Guess/MATLAB Function

`MakeH` for the Obstacle: the same as the Target's (see that block).

## Thruster & Float Control Code/Change RED Behavior/Calculate Duty Cycle/Generate Initial Guess/MATLAB Function

`MakeH` for the Chaser: the same as the Target's (see that block).

## Thruster & Float Control Code/Change BLACK Behavior/Calculate Duty Cycle/Rotate Forces to Inertial/Create Rotation Matrix

`RotateFrame` (this block's) returns the rotation from the body frame to the table frame, the inverse of $C_{bI}(\theta)$, to turn the force the thrusters actually give back into the table frame for the simulated plant.

## Thruster & Float Control Code/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix

`RotateFrame` returns the rotation $C_{bI}(\theta)$ from the table frame to the body frame, for a platform at attitude θ: the controller's force, in the table frame, times it gives the force the thrusters must give in the body's frame ([The Thruster Mixer](page:math-mixer)).
