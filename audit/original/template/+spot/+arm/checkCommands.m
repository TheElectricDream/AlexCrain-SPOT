function msgs = checkCommands(dataClass, armModel)
    %CHECKCOMMANDS Arm goals and PWM commands the servos would ignore or clip, from a run's log.
    %   msgs = spot.arm.checkCommands(dataClass, armModel) checks a run's logged arm signals for
    %   commands the Dynamixel servos would ignore or clip. spot.sim.run adds the messages to the
    %   simulation's warnings.
    %
    %   Inputs
    %     dataClass  logged signals (simOut.dataPacket), as timeseries. Required:
    %                  ARM_Control_Mode      0 off, 1 position, 2 custom (PWM), 3 velocity
    %                  ARM_<Joint>_Command   PWM sent [885 = 100 %]
    %                  ARM_<Joint>_Path      goal: angle [rad] (position) or speed [rad/s]
    %                                        (velocity)
    %                Optional (needed for check 3):
    %                  ARM_Rz_<Joint>_rad    measured joint angle [rad]
    %                <Joint> is Shoulder, Elbow or Wrist.
    %     armModel   arm model (spot.arm.parameters): its .servo limits
    %
    %   Outputs
    %     msgs       1xN cell array of messages, one per joint and problem; {} if none. Each
    %                gives the first offending sample and the number of steps affected.
    %
    %   Checks
    %     0. Control mode not 0-3.
    %     For each joint:
    %     1. Position goal beyond Min/Max Position: ignored, the joint holds its last goal.
    %     2. Velocity goal beyond the Velocity Limit: ignored, the joint keeps its last goal.
    %     3. PWM pushing a joint further past its position limit: replaced by zero
    %        (spot.arm.withinLimits).
    %     4. PWM beyond the PWM Limit: the servo gets the limit.
    %
    %   See also spot.sim.run, spot.arm.withinLimits, spot.control.ArmPositionControl,
    %   spot.control.ArmVelocityControl, spot.arm.Plant, spot.hardware.ArmPWM.

    %% Constants
    % Dynamixel MX unit conversions, as used by the controllers and blocks being checked.
    % Goals use 651.74 pulses/rad (old hardware code); the encoder uses 4096 pulses/rev.
    CENTER_PULSE = 2048;                    % [pulses] joint straight out
    PULSES_PER_REV = 4096;                  % [pulses/rev] encoder resolution
    CMD_PULSES_PER_RAD = 651.74;            % [pulses/rad] goal conversion (PositionControl)
    RADPS_TO_RPM = 9.549296585513702;       % [rev/min per rad/s] = 60/(2*pi)
    RPM_TO_VEL_UNITS = 4.366756307;         % [units per rev/min] = 1/0.229 (VelocityControl)
    VEL_UNIT_RPM = 0.229;                   % [rev/min] per velocity unit
    PWM_ROUNDING = 0.5;                     % [PWM] commands are sent in whole units

    % ARM_Control_Mode values
    MODE_OFF = 0;
    MODE_POSITION = 1;
    MODE_CUSTOM = 2;
    MODE_VELOCITY = 3;

    JOINT_NAMES = {'shoulder', 'elbow', 'wrist'};

    msgs = {};

    %% Logged signals
    % Nothing to check without the mode, commands and goals.
    requiredSignals = {'ARM_Control_Mode', ...
                       'ARM_Shoulder_Command', 'ARM_Elbow_Command', 'ARM_Wrist_Command', ...
                       'ARM_Shoulder_Path', 'ARM_Elbow_Path', 'ARM_Wrist_Path'};
    if ~all(isfield(dataClass, requiredSignals))
        return
    end

    % Mode (logged as double), and commands and goals with one column per joint.
    sampleTime = dataClass.ARM_Control_Mode.Time;
    ctrlMode = round(dataClass.ARM_Control_Mode.Data(:));
    pwmCmd = [dataClass.ARM_Shoulder_Command.Data(:), ...
              dataClass.ARM_Elbow_Command.Data(:), ...
              dataClass.ARM_Wrist_Command.Data(:)];
    goal = [dataClass.ARM_Shoulder_Path.Data(:), ...
            dataClass.ARM_Elbow_Path.Data(:), ...
            dataClass.ARM_Wrist_Path.Data(:)];
    nSamples = min([numel(ctrlMode), size(pwmCmd, 1), size(goal, 1)]);

    % Measured joint angles, if logged. Left as NaN otherwise, so check 3 never fires.
    jointAngle = nan(nSamples, 3);
    if all(isfield(dataClass, {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'}))
        jointAngle = [dataClass.ARM_Rz_Shoulder_rad.Data(:), ...
                      dataClass.ARM_Rz_Elbow_rad.Data(:), ...
                      dataClass.ARM_Rz_Wrist_rad.Data(:)];
        nSamples = min(nSamples, size(jointAngle, 1));
        jointAngle = jointAngle(1:nSamples, :);
    end

    % Trim all signals to a common length.
    ctrlMode = ctrlMode(1:nSamples);
    pwmCmd = pwmCmd(1:nSamples, :);
    goal = goal(1:nSamples, :);
    sampleTime = sampleTime(1:nSamples);

    %% Servo limits
    servo = armModel.servo;

    % Position limits [deg] and Velocity Limit [rad/s], for the messages.
    minAngleDeg = (servo.minPosition - CENTER_PULSE) / CMD_PULSES_PER_RAD * 180/pi;
    maxAngleDeg = (servo.maxPosition - CENTER_PULSE) / CMD_PULSES_PER_RAD * 180/pi;
    maxSpeed = servo.velocityLimit * VEL_UNIT_RPM * 2*pi/60;

    %% Check 0: unknown control mode
    badMode = ~ismember(ctrlMode, [MODE_OFF, MODE_POSITION, MODE_CUSTOM, MODE_VELOCITY]);
    if any(badMode)
        k = find(badMode, 1);
        msgs{end + 1} = sprintf( ...
            ['ARM_Control_Mode was %g at %.1f s (%d steps): SPOT''s arm controllers write 0 ' ...
             '(off), 1 (position), 2 (custom, PWM) or 3 (velocity).'], ...
            ctrlMode(k), sampleTime(k), nnz(badMode));
    end

    %% Checks 1-4, per joint
    for j = 1:3

        % Check 1: position goal beyond Min/Max Position (position control).
        % Pulses as spot.control.ArmPositionControl computes them.
        goalPulses = round(CMD_PULSES_PER_RAD * goal(:, j) + CENTER_PULSE);
        goalOutOfRange = ctrlMode == MODE_POSITION ...
                       & (goalPulses < servo.minPosition | goalPulses > servo.maxPosition);
        if any(goalOutOfRange)
            k = find(goalOutOfRange, 1);
            msgs{end + 1} = sprintf( ...
                ['The arm''s %s goal of %.1f deg (%d pulses) at %.1f s (%d steps) is beyond ' ...
                 'its servo''s position limits, %.0f to %.0f deg (Min/Max Position %d-%d on ' ...
                 'the Default Position Control (ARM) block). The controller ignores it, as ' ...
                 'the servo would, so the joint holds its last goal. Joint angles are 0 ' ...
                 'straight out (the shoulder: out from its face) and +-90 deg to either side.'], ...
                JOINT_NAMES{j}, goal(k, j) * 180/pi, goalPulses(k), sampleTime(k), ...
                nnz(goalOutOfRange), minAngleDeg, maxAngleDeg, ...
                servo.minPosition, servo.maxPosition);
        end

        % Check 2: velocity goal beyond the Velocity Limit (velocity control).
        % Velocity units as spot.control.ArmVelocityControl computes them.
        goalUnits = round(goal(:, j) * RADPS_TO_RPM * RPM_TO_VEL_UNITS);
        goalTooFast = ctrlMode == MODE_VELOCITY & abs(goalUnits) > servo.velocityLimit;
        if any(goalTooFast)
            k = find(goalTooFast, 1);
            msgs{end + 1} = sprintf( ...
                ['The arm''s %s goal speed of %.3f rad/s at %.1f s (%d steps) is beyond its ' ...
                 'Velocity Limit (%d x 0.229 rev/min = %.3f rad/s, Default Velocity Control ' ...
                 '(ARM)): the controller ignores it, so the joint keeps its last goal.'], ...
                JOINT_NAMES{j}, goal(k, j), sampleTime(k), nnz(goalTooFast), ...
                servo.velocityLimit, maxSpeed);
        end

        % Check 3: PWM pushing a joint further past its position limit (any mode).
        anglePulses = round(jointAngle(:, j) * PULSES_PER_REV / (2*pi)) + CENTER_PULSE;
        pushedPastLimit = (anglePulses >= servo.maxPosition & pwmCmd(:, j) > 0) ...
                        | (anglePulses <= servo.minPosition & pwmCmd(:, j) < 0);
        if any(pushedPastLimit)
            k = find(pushedPastLimit, 1);
            msgs{end + 1} = sprintf( ...
                ['The arm''s %s was at its position limit (%.0f deg) with a PWM of %.0f ' ...
                 'pushing it further at %.1f s (%d steps): it got none (Min/Max Position on ' ...
                 'the Dynamixel Arm (PWM) block).'], ...
                JOINT_NAMES{j}, jointAngle(k, j) * 180/pi, pwmCmd(k, j), sampleTime(k), ...
                nnz(pushedPastLimit));
        end

        % Check 4: PWM beyond the PWM Limit (any mode but off). Commands are sent in whole
        % units, so allow half a unit over. Samples zeroed by check 3 are skipped.
        overLimit = ctrlMode ~= MODE_OFF & ~pushedPastLimit ...
                  & abs(pwmCmd(:, j)) > servo.pwmLimit + PWM_ROUNDING;
        if any(overLimit)
            k = find(overLimit, 1);
            msgs{end + 1} = sprintf( ...
                ['The arm''s %s PWM of %.0f at %.1f s (%d steps) is beyond its servo''s PWM ' ...
                 'Limit (%d of 885, on the Dynamixel Arm (PWM) block): the servo gets the ' ...
                 'limit.'], ...
                JOINT_NAMES{j}, pwmCmd(k, j), sampleTime(k), nnz(overLimit), servo.pwmLimit);
        end
    end
end
