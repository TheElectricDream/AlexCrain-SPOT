function a = defaults()
    %DEFAULTS The simulated arm's servos, their friction and the plant's internal step.
    %   a = spot.arm.defaults() returns the parts of the arm model not in the settings: the
    %   Dynamixel servos, joint friction, reflected rotor inertia and the plant's internal step.
    %   Every value comes from Constants (the Dynamixel and arm sections); this function
    %   only arranges them. spot.arm.parameters adds masses, geometry, initial state and any
    %   calibration.
    %
    %   Joints are [shoulder elbow wrist] (Dynamixel IDs 1, 2, 3).
    %
    %   The servos run in PWM control (operating mode MODE_PWM): each command is the share of the
    %   supply voltage put across the motor, in servo units (PWM_FULL = 100 %). At the joint the
    %   motor then gives
    %       torquePerVolt (V - backEMF w),   V = PWM / PWM_FULL x voltage
    %   so at rest PWM acts as torque (the stall torque at PWM_FULL: TORQUE_PER_VOLT_MX64 x the
    %   supply), and turning, the back-EMF brakes the joint like viscous friction
    %   (torquePerVolt x backEMF, about 0.74 N m s/rad).
    %
    %   Friction per joint [N m]: a joint at rest stays at rest until the torque on it exceeds
    %   the breakaway torque "static"; moving, the friction against the motion is
    %       coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous |v|
    %   The friction, backEMF and armature are one MX-64(2.0)'s, measured unloaded on the bench
    %   with calibrate_arm.py on ARM_BENCH_DATE (every joint uses this servo); the arm
    %   calibration replaces them with each joint's own, on the arm.
    %
    %   Servo units (Dynamixel MX, protocol 2.0): position PULSES_PER_REV pulses/rev, velocity
    %   VEL_UNIT_RPM rev/min, PWM PWM_FULL = 100 %, profile acceleration ACC_UNIT_RPM2
    %   rev/min^2.
    %
    %   See also Constants, spot.arm.parameters, spot.arm.Plant.

    a = struct();

    %% Internal step
    % Plant's internal step [s]: the motors and friction run at this rate.
    a.innerStep = Constants.ARM_PLANT_INNER_STEP;

    %% Motors
    % Per joint, at the joint (after the gearbox): torque per volt across the motor at rest
    % (stall torque / supply, the datasheet's: nothing measures it), and the back-EMF. The
    % bench servo's speed coefficient (the back-EMF and viscous friction together, which is
    % all the plant uses) lay below the datasheet's BACK_EMF_MX64_DATASHEET: it is taken as
    % all back-EMF, as calibrate_arm.py does.
    a.motor = struct( ...
        'voltage', Constants.MX64_SUPPLY, ...                      % [V] supply
        'torquePerVolt', Constants.TORQUE_PER_VOLT_MX64 * [1 1 1], ... % [N m/V]
        'backEMF', Constants.ARM_BACK_EMF * [1 1 1]);               % [V s/rad]

    % Rotor and gear train inertia at each joint [kg m^2]: the bench servo's inertia from its
    % PWM steps, with no link attached.
    a.armature = Constants.ARM_ARMATURE * [1 1 1];

    %% Friction
    % The bench servo's: breakaway from six PWM ramps (ARM_STATIC_POS turning +,
    % ARM_STATIC_NEG turning -), Coulomb from the speed sweep. Its steady speeds all lay above
    % the Stribeck knee, so the Stribeck velocity is the script's convention (half the slowest
    % steady speed), not a measurement; no viscous friction beyond the back-EMF.
    a.friction = struct( ...
        'static', Constants.ARM_STATIC * [1 1 1], ...       % breakaway [N m]
        'coulomb', Constants.ARM_COULOMB * [1 1 1], ...     % [N m]
        'stribeck', Constants.ARM_STRIBECK * [1 1 1], ...   % Stribeck velocity [rad/s]
        'viscous', Constants.ARM_VISCOUS * [1 1 1]);        % [N m s/rad] besides the back-EMF

    %% Servo settings
    % In Dynamixel units. pwmLimit, minPosition/maxPosition and velocityLimit are overwritten
    % from the diagram's blocks (spot.arm.servoFromDiagram). The position-mode gains are what
    % calibrate_arm.py uses for its steps (spot.arm.compareSteps).
    a.servo = struct( ...
        'positionP', Constants.SERVO_POSITION_P, ...               % Position P/I/D Gain
        'positionI', Constants.SERVO_POSITION_I, ...
        'positionD', Constants.SERVO_POSITION_D, ...
        'feedforward1', Constants.SERVO_FEEDFORWARD_1, ...         % Feedforward 1st/2nd Gain
        'feedforward2', Constants.SERVO_FEEDFORWARD_2, ...
        'profileVelocity', Constants.SERVO_PROFILE_VELOCITY, ...   % profile (0 = none)
        'profileAcceleration', Constants.SERVO_PROFILE_ACCELERATION, ...
        'minPosition', Constants.MIN_POSITION, ...                 % Min/Max Position [pulses]
        'maxPosition', Constants.MAX_POSITION, ...
        'velocityLimit', Constants.VELOCITY_LIMIT, ...             % [VEL_UNIT_RPM rev/min]
        'pwmLimit', Constants.PWM_FULL, ...                        % PWM Limit
        'period', Constants.SERVO_PERIOD);                         % period [s] (assumed)
end
