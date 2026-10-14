function a = defaults()
%DEFAULTS The simulated arm's servos, their friction and the plant's internal step.
%   a = spot.arm.defaults() returns the parts of the arm model that are not in the settings: the
%   Dynamixel servos (motor, limits), the joint friction, the reflected rotor inertia, and the
%   internal step at which spot.arm.Plant integrates (the diagram runs at baseRate; the plant
%   takes baseRate/innerStep steps each time). The servos run in current control; the position
%   and velocity control laws are the diagram's controllers (spot.arm.PositionControl,
%   spot.arm.VelocityControl, with their own gains). spot.arm.parameters adds the
%   masses, the link geometry and the initial state, and a calibration file if there is one.
%
%   Joints are [shoulder elbow wrist] (Dynamixel IDs 1, 2, 3).
%
%   Friction (per joint, at the joint, in N m): a joint at rest stays at rest until the torque
%   on it exceeds the breakaway torque "static"; moving, the friction is
%       coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous |v|
%   against the motion. The values here are placeholders until the arm is calibrated
%   (resources/arm_calibration/calibrate_arm.py writes resources/arm_calibration.json).
%
%   Servo units (Dynamixel MX series, protocol 2.0): position 4096 pulses/rev, velocity 0.229
%   rev/min, current 3.36 mA, profile acceleration 214.577 rev/min^2, PWM 885 = 100 %.

a = struct();

% The plant's internal step [s]: the servos' current control and the friction run at this rate
a.innerStep = 1e-3;

% Motor and gearbox of each joint, from the MX-64(2.0) datasheet at 12 V: stall torque [N m],
% stall current [A], no-load speed [rad/s] (63 rev/min), supply voltage [V]
a.motor = struct( ...
    'stallTorque', [6.0 6.0 6.0], ...
    'stallCurrent', [4.1 4.1 4.1], ...
    'noLoadSpeed', [63 63 63] * 2*pi/60, ...
    'voltage', 12);

% Rotor and gear train inertia seen at each joint (rotor inertia x gear ratio^2) [kg m^2]: an
% estimate for a 200:1 MX-64 with a coreless 17 mm motor (about 0.9 g cm^2), until calibrated
a.armature = [3.5e-3 3.5e-3 3.5e-3];

% Joint friction (placeholders until calibrated): breakaway [N m], Coulomb [N m], Stribeck
% velocity [rad/s], viscous [N m s/rad]
a.friction = struct( ...
    'static', [0.30 0.30 0.30], ...
    'coulomb', [0.20 0.20 0.20], ...
    'stribeck', [0.05 0.05 0.05], ...
    'viscous', [0.10 0.10 0.10]);

% The servos, in Dynamixel units. The arm runs in current control (spot.hardware.ArmTorque,
% spot.arm.Plant): currentLimit is the Current Limit the plant holds the goal currents within,
% minPosition/maxPosition and velocityLimit are what spot.arm.checkCommands checks the controllers'
% goals against; spot.sim.run copies all three from the diagram's blocks (spot.arm.servoFromDiagram).
% The rest are the servos' own position-mode settings, as the calibration script runs them for its
% position steps: spot.arm.compareSteps replays those through spot.arm.PositionControl at the
% servo's period, with back-EMF, which is the servo's own control law.
a.servo = struct( ...
    'positionP', 400, 'positionI', 0, 'positionD', 200, ...   % Position P/I/D Gain
    'feedforward1', 0, 'feedforward2', 0, ...                  % Feedforward 1st/2nd Gain
    'profileVelocity', 20, 'profileAcceleration', 0, ...       % position mode profile (0 = none)
    'minPosition', 1024, 'maxPosition', 3072, ...              % Min/Max Position Limit [pulses]
    'velocityLimit', 1023, ...                                 % Velocity Limit [0.229 rev/min]
    'currentLimit', 850, ...                                   % Current Limit [3.36 mA]
    'pwmLimit', 885, ...                                       % PWM Limit
    'period', 1e-3);                                           % the servo's control period [s] (assumed)
end
