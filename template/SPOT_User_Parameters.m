% SPOT_User_Parameters: your own workspace variables for the Simulink diagrams.
%
% The SPOT Console runs this script after putting its settings into the base workspace (every
% time a setting changes, and before each simulation), so anything defined here is available to
% the diagrams and can override the console's values. It replaces the custom-variable section of
% Run_Initializer.m.

%% Robotic arm: the simulated arm (armModel, see help spot.arm.defaults)
% Its servos, friction and internal step. The servos run in PWM (voltage) control: ARM_Controls
% is each joint's PWM [885 = 100 % of the supply]. Their limits come from the diagram's
% "Dynamixel Arm (PWM)" block, and the position and velocity control laws (with their gains)
% are the diagram's Default Position/Velocity Control (ARM) blocks. The friction and back-EMF
% come from the arm calibration when there is one (the Simulation tab's Calibration pane;
% +spot/+arm/calibration/calibrate_arm.py). Change any of it here, e.g.:
% armModel.friction.static  = [0.163 0.163 0.163]; % breakaway torque [N m] (shoulder, elbow, wrist)
% armModel.friction.coulomb = [0.123 0.123 0.123]; % sliding friction [N m]
% armModel.armature = [0.0115 0.0115 0.0115];      % rotor inertia at the joint [kg m^2]
% armModel.motor.backEMF = [1.485 1.485 1.485];    % back-EMF at the joint [V s/rad]
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
