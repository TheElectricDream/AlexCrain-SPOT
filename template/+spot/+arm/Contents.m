% ARM  The simulated arm: its Dynamixel servos, joint friction and 1 ms plant; its controllers; calibration from lab data
%
%   Plant             - The Chaser (RED) with its arm and Dynamixel servos, simulated at the plant's own step.
%   PositionControl   - The Dynamixel servos' own position control, as a controller giving joint torques.
%   VelocityControl   - The Dynamixel servos' own velocity control, as a controller giving joint torques.
%   withinLimits      - No joint torque that pushes a joint further beyond its position limits.
%   defaults          - The simulated arm's servos, their friction and the plant's internal step.
%   parameters        - The arm model spot.arm.Plant simulates (the workspace variable armModel).
%   loadCalibration   - Apply the arm calibration (resources/arm_calibration/calibrate_arm.py) to a model.
%   compareSteps      - The simulated servos against the real ones, on the calibration's position steps.
%   servoFromDiagram  - The simulated servos' limits, from the diagram's arm blocks.
%   mountGeometry     - The arm's geometry for where it is mounted on the Chaser.
%   checkCommands     - Arm goals the controllers ignore and torques the servos can't give, from a run's log.
%   inertiaMatrix     - Mass matrix of the Chaser (RED) with its three-link arm, free floating.
%   coriolisMatrix    - Coriolis and centripetal matrix of the Chaser (RED) with its three-link arm.
%
%   The lab calibration script is resources/arm_calibration/calibrate_arm.py (Python, Dynamixel SDK).
