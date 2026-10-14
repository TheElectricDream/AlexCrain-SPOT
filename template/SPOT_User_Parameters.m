% SPOT_User_Parameters: your own workspace variables for the Simulink diagrams.
%
% The SPOT Console runs this script after putting its settings into the base workspace (every
% time a setting changes, and before each simulation), so anything defined here is available to
% the diagrams and can override the console's values. It replaces the custom-variable section of
% Run_Initializer.m.

%% Robotic arm: the simulated arm (armModel, see help spot.arm.defaults)
% Its servos, friction and internal step. The servos run in current control; their limits come
% from the diagram's "Dynamixel Arm (Torque)" block, and the position and velocity control laws
% (with their gains) are the diagram's Default Position/Velocity Control (ARM) blocks. The
% friction comes from resources/arm_calibration.json when there is one
% (resources/arm_calibration/calibrate_arm.py). Change any of it here, e.g.:
% armModel.friction.static  = [0.30 0.30 0.30];   % breakaway torque [N m] (shoulder, elbow, wrist)
% armModel.friction.coulomb = [0.20 0.20 0.20];   % sliding friction [N m]
% armModel.armature = [3.5e-3 3.5e-3 3.5e-3];     % rotor inertia at the joint [kg m^2]
% armModel.motor.voltage = 12;                    % servo supply [V]

%% Robotic arm: transpose-Jacobian controller gains
Kp = [0.08 0    0
      0    0.08 0
      0    0    0.002];
Kv = [0.05 0    0
      0    0.05 0
      0    0    0.005];

%% Set in the SPOT Console now (don't define them here: this script runs after the settings)
% PWMFreq:      the thrusters' PWM frequency, Simulation tab > Scenario > Thruster PWM
% labConsoleIP: the lab console's (TX2's) address, Experiment tab > Lab
