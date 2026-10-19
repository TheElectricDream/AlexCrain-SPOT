function a = defaults()
    %DEFAULTS The simulated arm's servos, their friction and the plant's internal step.
    %   a = spot.arm.defaults() returns the parts of the arm model not in the settings: the
    %   Dynamixel servos, joint friction, reflected rotor inertia and the plant's internal step.
    %   spot.arm.parameters adds masses, geometry, initial state and any calibration.
    %
    %   Joints are [shoulder elbow wrist] (Dynamixel IDs 1, 2, 3).
    %
    %   The servos run in PWM control (operating mode 16): each command is the share of the
    %   supply voltage put across the motor, in servo units (885 = 100 %). At the joint the
    %   motor then gives
    %       torquePerVolt (V - backEMF w),   V = PWM / 885 x voltage
    %   so at rest PWM acts as torque (6 N m at 885 for an MX-64(2.0) at 12 V), and turning,
    %   the back-EMF brakes the joint like viscous friction (torquePerVolt x backEMF, about
    %   0.74 N m s/rad).
    %
    %   Friction per joint [N m]: a joint at rest stays at rest until the torque on it exceeds
    %   the breakaway torque "static"; moving, the friction against the motion is
    %       coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous |v|
    %   The friction, backEMF and armature are one MX-64(2.0)'s, measured unloaded on the bench
    %   with calibrate_arm.py on 2026-10-17 (every joint uses this servo); the arm calibration
    %   replaces them with each joint's own, on the arm.
    %
    %   Servo units (Dynamixel MX, protocol 2.0): position 4096 pulses/rev, velocity
    %   0.229 rev/min, PWM 885 = 100 %, profile acceleration 214.577 rev/min^2.
    %
    %   See also spot.arm.parameters, spot.arm.Plant.

    %% Constants
    % MX-64(2.0) datasheet at 12 V.
    STALL_TORQUE = 6.0;                     % [N m]
    NO_LOAD_SPEED = 63 * 2*pi/60;           % [rad/s] (63 rev/min; back-EMF 1.82 V s/rad)
    SUPPLY = 12;                            % [V]

    % One MX-64(2.0) (ID 1, firmware 45) measured unloaded on the bench with calibrate_arm.py,
    % 2026-10-17, at 11.8 V: the model replayed its position steps to 0.15 deg rms.
    BENCH_STATIC = 0.163;                   % [N m] breakaway (24 to 27 PWM)
    BENCH_COULOMB = 0.123;                  % [N m]
    BENCH_STRIBECK = 0.021;                 % [rad/s] (a convention: see Friction below)
    BENCH_BACK_EMF = 1.485;                 % [V s/rad] the measured speed coefficient
    BENCH_ARMATURE = 0.0115;                % [kg m^2] inertia about the joint, no link

    a = struct();

    %% Internal step
    % Plant's internal step [s]: the motors and friction run at this rate.
    a.innerStep = 1e-3;

    %% Motors
    % Per joint, at the joint (after the 200:1 gearbox): torque per volt across the motor at
    % rest (stall torque / supply, the datasheet's: nothing measures it), and the back-EMF.
    % The bench servo's speed coefficient (the back-EMF and viscous friction together, which
    % is all the plant uses) was 1.485 V s/rad, below the datasheet's SUPPLY / NO_LOAD_SPEED
    % (1.82): it is taken as all back-EMF, as calibrate_arm.py does.
    a.motor = struct( ...
        'voltage', SUPPLY, ...                              % [V] supply
        'torquePerVolt', STALL_TORQUE / SUPPLY * [1 1 1], ...   % [N m/V]
        'backEMF', BENCH_BACK_EMF * [1 1 1]);               % [V s/rad]

    % Rotor and gear train inertia at each joint [kg m^2]: the bench servo's inertia from its
    % PWM steps (0.0118 turning +, 0.0113 turning -), with no link attached.
    a.armature = BENCH_ARMATURE * [1 1 1];

    %% Friction
    % The bench servo's: breakaway from six PWM ramps (0.167 turning +, 0.160 turning -),
    % Coulomb from the speed sweep. Its steady speeds all lay above the Stribeck knee (it
    % stick-slips below 0.04 rad/s), so the Stribeck velocity is the script's convention (half
    % the slowest steady speed), not a measurement; no viscous friction beyond the back-EMF.
    a.friction = struct( ...
        'static', BENCH_STATIC * [1 1 1], ...       % breakaway [N m]
        'coulomb', BENCH_COULOMB * [1 1 1], ...     % [N m]
        'stribeck', BENCH_STRIBECK * [1 1 1], ...   % Stribeck velocity [rad/s]
        'viscous', [0 0 0]);                        % [N m s/rad] (besides the back-EMF)

    %% Servo settings
    % In Dynamixel units. pwmLimit, minPosition/maxPosition and velocityLimit are overwritten
    % from the diagram's blocks (spot.arm.servoFromDiagram). The position-mode gains are what
    % calibrate_arm.py uses for its steps (spot.arm.compareSteps).
    a.servo = struct( ...
        'positionP', 400, 'positionI', 0, 'positionD', 200, ...   % Position P/I/D Gain
        'feedforward1', 0, 'feedforward2', 0, ...                  % Feedforward 1st/2nd Gain
        'profileVelocity', 20, 'profileAcceleration', 0, ...       % position profile (0 = none)
        'minPosition', 1024, 'maxPosition', 3072, ...              % Min/Max Position [pulses]
        'velocityLimit', 1023, ...                                 % [0.229 rev/min]
        'pwmLimit', 885, ...                                       % PWM Limit
        'period', 1e-3);                                           % control period [s] (assumed)
end
