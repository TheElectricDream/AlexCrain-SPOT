function a = defaults()
    %DEFAULTS The simulated arm's servos, their friction and the plant's internal step.
    %   a = spot.arm.defaults() returns the parts of the arm model not in the settings: the
    %   Dynamixel servos, joint friction, reflected rotor inertia and the plant's internal step.
    %   spot.arm.parameters adds masses, geometry, initial state and any calibration.
    %
    %   Joints are [shoulder elbow wrist] (Dynamixel IDs 1, 2, 3).
    %
    %   Friction per joint [N m]: a joint at rest stays at rest until the torque on it exceeds
    %   the breakaway torque "static"; moving, the friction against the motion is
    %       coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous |v|
    %   The values are placeholders until calibrated (calibrate_arm.py).
    %
    %   Servo units (Dynamixel MX, protocol 2.0): position 4096 pulses/rev, velocity
    %   0.229 rev/min, current 3.36 mA, profile acceleration 214.577 rev/min^2, PWM 885 = 100 %.
    %
    %   See also spot.arm.parameters, spot.arm.Plant.

    a = struct();

    % Plant's internal step [s]: servo current control and friction run at this rate.
    a.innerStep = 1e-3;

    % Motor and gearbox per joint (MX-64(2.0) datasheet at 12 V).
    a.motor = struct( ...
        'stallTorque', [6.0 6.0 6.0], ...           % [N m]
        'stallCurrent', [4.1 4.1 4.1], ...          % [A]
        'noLoadSpeed', [63 63 63] * 2*pi/60, ...    % [rad/s] (63 rev/min)
        'voltage', 12);                             % [V]

    % Rotor and gear train inertia at each joint [kg m^2]: estimate for a 200:1 MX-64 with a
    % 17 mm coreless motor (about 0.9 g cm^2), until calibrated.
    a.armature = [3.5e-3 3.5e-3 3.5e-3];

    % Joint friction (placeholders until calibrated).
    a.friction = struct( ...
        'static', [0.30 0.30 0.30], ...             % breakaway [N m]
        'coulomb', [0.20 0.20 0.20], ...            % [N m]
        'stribeck', [0.05 0.05 0.05], ...           % Stribeck velocity [rad/s]
        'viscous', [0.10 0.10 0.10]);               % [N m s/rad]

    % Servo settings, in Dynamixel units. currentLimit, minPosition/maxPosition and
    % velocityLimit are overwritten from the diagram's blocks (spot.arm.servoFromDiagram). The
    % position-mode gains are what calibrate_arm.py uses for its steps (spot.arm.compareSteps).
    a.servo = struct( ...
        'positionP', 400, 'positionI', 0, 'positionD', 200, ...   % Position P/I/D Gain
        'feedforward1', 0, 'feedforward2', 0, ...                  % Feedforward 1st/2nd Gain
        'profileVelocity', 20, 'profileAcceleration', 0, ...       % position profile (0 = none)
        'minPosition', 1024, 'maxPosition', 3072, ...              % Min/Max Position [pulses]
        'velocityLimit', 1023, ...                                 % [0.229 rev/min]
        'currentLimit', 850, ...                                   % [3.36 mA]
        'pwmLimit', 885, ...                                       % PWM Limit
        'period', 1e-3);                                           % control period [s] (assumed)
end
