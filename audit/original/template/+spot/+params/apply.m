function d = apply(s, ~)
    %APPLY Put the settings into the base workspace, as the Simulink diagrams expect them.
    %   d = spot.params.apply(s) assigns every workspace variable the old GUI assigned (phase
    %   times, rates, gains, LQR matrices, mass properties, initial conditions, thruster forces,
    %   ...) and armModel (spot.arm.parameters, for the simulated arm), with the lab's
    %   calibration as the project took it (spot.params.labCalibration, Constants.LAB_*), each
    %   part when its s.calibration switch is on: the table's slope, each thruster's thrust and
    %   decay, and the arm's friction and inertia, with the values the project overrides; and
    %   where the project's LEDs are.
    %   d = spot.params.apply(s, root) is the same: the project folder is accepted for the
    %   callers that pass it, and not needed (the calibration lives in Constants).
    %
    %   Inputs
    %     s     settings
    %     root  project folder (optional, unused)
    %
    %   Outputs
    %     d     derived values for display: tsim, timing, rates, mass properties, the PhaseSpace
    %           marker file (trackerPositions), arm geometry, calibration used
    %           (d.calibration.inUse: the calibrated values in the workspace), LQR gains,
    %           warnings, the variables assigned
    %
    %   Example (no GUI needed)
    %     s = spot.params.fromConstants();
    %     s.platforms.RED = true;
    %     spot.params.apply(s, pwd);
    %     out = spot.sim.run(s, pwd);
    %
    %   See also spot.params.fromConstants, spot.params.timing, spot.sim.run.

    %% Constants
    MAX_DATA_RATE = Constants.MAX_DATA_RATE;                   % [Hz] PhaseSpace
    TEENSY_PWM_FREQUENCY = Constants.TEENSY_PWM_FREQUENCY;     % [Hz] the firmware's
    TABLE_X = Constants.TABLE_X;                               % [m] table size
    TABLE_Y = Constants.TABLE_Y;                               % [m]
    LEVEL_TABLE = Constants.LEVEL_TABLE;                       % tableSlope of a level table

    d = struct();
    ws = struct();   % name -> value, assigned to the base workspace at the end

    %% Platforms
    d.platformSelection = spot.params.platformSelection(s.platforms);
    ws.platformSelection = d.platformSelection;

    %% Phase timing
    % All but the console's lists (phases, segments) are diagram variables.
    times = spot.params.timing(s);
    for f = fieldnames(times).'
        if ~any(strcmp(f{1}, {'phases', 'segments'}))
            ws.(f{1}) = times.(f{1});
        end
    end
    d.timing = times;
    d.tsim = times.tsim;

    %% Rates
    if ~(s.sampleRate > 0)
        error('spot:params:apply', 'The sample rate must be above 0 Hz.');
    end
    if ~(isscalar(s.dataRate) && s.dataRate >= 1 && s.dataRate <= MAX_DATA_RATE ...
         && s.dataRate == round(s.dataRate))
        error('spot:params:apply', ...
            'The PhaseSpace data rate must be a whole number of Hz, 1 to %d.', MAX_DATA_RATE);
    end
    ws.baseRate = 1 / s.sampleRate;
    ws.dataRate = 1 / s.dataRate;            % [s] the setting is in Hz
    d.baseRate = ws.baseRate;
    d.dataRate = ws.dataRate;
    d.cameraRate = s.dataRate;               % [Hz] PhaseSpace rate the Experiment tab starts

    %% Thrusters' PWM frequency and the lab console's address
    if ~(isscalar(s.pwmFrequency) && s.pwmFrequency > 0)
        error('spot:params:apply', 'The thrusters'' PWM frequency must be above 0 Hz.');
    end
    ws.PWMFreq = s.pwmFrequency;

    % The Teensy firmware has its own PWM frequency: warn if they differ.
    pwmWarning = '';
    if s.pwmFrequency ~= TEENSY_PWM_FREQUENCY
        pwmWarning = sprintf( ...
            ['The thrusters'' PWM frequency is %g Hz, but the Teensy firmware runs its PWM at ' ...
             '%g Hz (PWM_FREQUENCY in +spot/+hardware/teensy/teensy_pwm_control.ino): ' ...
             'flash it with the same value, or the duty cycles won''t mean what the diagram ' ...
             'assumes.'], ...
            s.pwmFrequency, TEENSY_PWM_FREQUENCY);
    end

    % Four numbers, for the diagram's UDP blocks.
    ws.labConsoleIP = spot.params.ipv4(s.labConsoleAddress);

    %% Table size
    % (spot.sim.run sets simMode = 1 while simulating.)
    ws.xLength = TABLE_X;
    ws.yLength = TABLE_Y;

    %% The project's calibration
    % The lab's calibration as the project took it (Constants.LAB_*), with the project's own
    % values: the values it overrides and where its LEDs are.
    cal = spot.params.labCalibration();

    %% Mass properties
    % The LEDs from each platform's CG are the lab console's marker file (trackerPositions),
    % which the Experiment tab sends it before the cameras start.
    massProps = spot.massprops.compute(s.mass, cal.markers);
    d.mass = massProps;
    d.trackerPositions = spot.massprops.trackerPositions(massProps);
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        ws.(['m' key]) = massProps.(key).mass;
        ws.(['I' key]) = massProps.(key).inertia;
        ws.(['thruster_dist2CG_' key]) = massProps.(key).thrusters;
    end

    % The arm where it is mounted: Phi and b0 for that place, and the angle it is turned by
    % (armShoulderOffset: the shoulder's 0 is straight out from its face).
    arm = spot.arm.mountGeometry(s.mass.arm);
    for f = {'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', ...
             'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
        ws.(f{1}) = arm.(f{1});
    end
    ws.armShoulderOffset = arm.q1Offset;
    d.armGeometry = arm;

    %% The lab's calibration: table slope
    % The table's slope pulls the floating platforms and their air bearings drag them
    % (spot.sim.tableSlope). Without the project folder, or with the setting off, it is level.
    ws.tableSlope = LEVEL_TABLE;
    for name = {'RED', 'BLACK', 'BLUE'}
        ws.(['tableDamping_' name{1}]) = 0;
    end

    ov = cal.overrides;
    d.calibration = struct('file', cal.file, 'updated', cal.updated, 'table', cal.table, ...
        'tableUsed', false, 'thrusters', cal.thrusters, 'thrustersUsed', {{}}, ...
        'decayUsed', {{}}, 'arm', cal.arm, 'armUsed', '', 'overrides', ov, ...
        'error', cal.error);
    if ~isempty(cal.error)
        d.warnings0 = {cal.error};
    end

    % The measured slope and drag (a level table where not measured), then the values the
    % project overrides.
    damping = ov.table.damping;
    overridden = any(isfinite(ov.table.height)) ...
        || any(isfinite([damping.RED damping.BLACK damping.BLUE]));
    if s.calibration.tableSlope && (~isempty(cal.table) || overridden)
        if ~isempty(cal.table)
            ws.tableSlope = cal.table.slope;
            for name = {'RED', 'BLACK', 'BLUE'}
                ws.(['tableDamping_' name{1}]) = cal.table.damping.(name{1});
            end
        end
        given = isfinite(ov.table.height);
        ws.tableSlope(given) = ov.table.height(given);
        for name = {'RED', 'BLACK', 'BLUE'}
            if isfinite(damping.(name{1}))
                ws.(['tableDamping_' name{1}]) = damping.(name{1});
            end
        end
        d.calibration.tableUsed = true;
    end

    %% Initial conditions (degrees in the settings, radians in the workspace)
    initial = s.initial;
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        ws.(['init_states_' key]) = pose(initial.(key).initial);
        ws.(['home_states_' key]) = pose(initial.(key).home);
    end
    ws.init_states_ARM = initial.ARM.initial(:).' * pi/180;
    d.armInitial = spot.plotting.armPosition(ws.init_states_RED, ws.init_states_ARM, arm);

    %% Arm model and controllers
    % The model spot.arm.Plant simulates, with the arm calibration the project took
    % (Constants.LAB_ARM_*) and the project's overrides when s.calibration.arm is on.
    if s.calibration.arm
        ws.armModel = spot.arm.parameters(ws, cal);
    else
        ws.armModel = spot.arm.parameters(ws);
    end
    d.calibration.armUsed = ws.armModel.calibration;   % where it came from, or ''
    armWarning = '';

    % Gains and limits of the diagram's Default Position/Velocity Control (ARM).
    ws.armControl = armGains(s.control.ARM);

    %% Thruster forces and thrust decay
    % F_thrusters_<PLATFORM> (8x1, used by the mixer and so the simulated plant): the thruster
    % calibration's when there is one and s.calibration.thrusters is on, else thrusterForce for
    % all eight. thrust_decay_<PLATFORM> likewise, else thrusterDecay. With the switch on, the
    % values the project overrides replace either.
    % F_<platform>_X_nominal and _Y_nominal: the setting's, for code of your own.
    short = struct('RED', 'red', 'BLACK', 'black', 'BLUE', 'blue');
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        F = spot.control.nominalForces(s.thrusterForce.(key));
        ws.(['F_' short.(key) '_X_nominal']) = F(1);
        ws.(['F_' short.(key) '_Y_nominal']) = F(3);
        decay = s.thrusterDecay.(key);
        if s.calibration.thrusters && ~isempty(cal.thrusters.(key))
            F = cal.thrusters.(key).thrust(:);
            d.calibration.thrustersUsed{end + 1} = key;
            if ~isempty(cal.thrusters.(key).decay)
                decay = cal.thrusters.(key).decay;
                d.calibration.decayUsed{end + 1} = key;
            end
        end
        if s.calibration.thrusters
            o = ov.thrusters.(key);
            given = isfinite(o.thrust);
            F(given) = o.thrust(given);
            if isfinite(o.decay)
                decay = o.decay;
            end
        end
        ws.(['F_thrusters_' key]) = F;
        ws.(['thrust_decay_' key]) = decay;
    end

    %% LQR weights, discrete models and gains
    qNames = {'Qx', 'Qy', 'Qtheta', 'QxD', 'QyD', 'QthetaD'};
    rNames = {'RFx', 'RFy', 'RTz'};
    d.lqr = struct();

    % Warnings so far: the calibration's, the PWM frequency's, then the arm calibration's.
    d.warnings = {};
    if isfield(d, 'warnings0')
        d.warnings = d.warnings0;
        d = rmfield(d, 'warnings0');
    end
    if ~isempty(pwmWarning)
        d.warnings{end + 1} = pwmWarning;
    end
    if ~isempty(armWarning)
        d.warnings{end + 1} = armWarning;
    end

    % Weights for each platform; models and gains only with the Control System Toolbox.
    haveControl = license('test', 'Control_Toolbox') && ~isempty(which('dlqr'));
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        weights = s.control.(key).lqr;
        for k = 1:6
            ws.([qNames{k} '_' key]) = weights.Q(k);
        end
        for k = 1:3
            ws.([rNames{k} '_' key]) = weights.R(k);
        end
        if haveControl
            sys = spot.control.stateSpace(massProps.(key).mass, massProps.(key).inertia, ...
                ws.dataRate);
            ws.(['A_d_' key]) = sys.A;
            ws.(['B_d_' key]) = sys.B;
            ws.(['C_d_' key]) = sys.C;
            ws.(['D_d_' key]) = sys.D;
            try
                ws.(['K_' key]) = spot.control.lqrGains(sys, weights.Q, weights.R);
                d.lqr.(key) = ws.(['K_' key]);
            catch err
                d.warnings{end + 1} = sprintf('%s LQR gains: %s', key, err.message);
            end
        end
    end
    if ~haveControl
        d.warnings{end + 1} = ['The Control System Toolbox is not available, so the LQR gains ' ...
                               'were not calculated.'];
    end

    %% Into the base workspace
    for f = fieldnames(ws).'
        assignin('base', f{1}, ws.(f{1}));
    end
    d.variables = fieldnames(ws);

    % The calibrated values the simulation will use.
    d.calibration.inUse = inUse(ws);
end

function u = inUse(ws)
    % The calibrated values put in the base workspace, for the Simulation tab's cards.
    u = struct('tableSlope', [], 'tableDamping', struct(), 'thrust', struct(), ...
               'decay', struct(), 'arm', []);
    values = ws;
    u.tableSlope = values.tableSlope;
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        u.tableDamping.(key) = values.(['tableDamping_' key]);
        u.thrust.(key) = values.(['F_thrusters_' key]);
        u.decay.(key) = values.(['thrust_decay_' key]);
    end
    m = values.armModel;
    u.arm = struct('static', m.friction.static, 'coulomb', m.friction.coulomb, ...
                   'stribeck', m.friction.stribeck, 'viscous', m.friction.viscous, ...
                   'armature', m.armature, 'back_emf', m.motor.backEMF);
end

function gains = armGains(arm)
    % The arm controllers' settings, checked against the servos' register ranges; per-joint
    % gains (one value for all three, or three) expanded to [shoulder elbow wrist].
    MAX_GAIN = Constants.MAX_GAIN;         % the gain registers' range
    MAX_PROFILE = Constants.MAX_PROFILE;   % the profile registers' range
    MAX_PWM = Constants.PWM_FULL;          % PWM at 100 % of the supply

    gains = arm;

    % Gains.
    checks = {'position', 'P'
              'position', 'I'
              'position', 'D'
              'velocity', 'P'
              'velocity', 'I'};
    for k = 1:size(checks, 1)
        value = arm.(checks{k, 1}).(checks{k, 2});
        if ~(isnumeric(value) && any(numel(value) == [1 3]) && all(isfinite(value)) ...
             && all(value >= 0) && all(value <= MAX_GAIN))
            error('spot:params:apply', ...
                ['The arm''s %s control %s gain must be 0 to %d (one value, or one per ' ...
                 'joint).'], checks{k, 1}, checks{k, 2}, MAX_GAIN);
        end
        gains.(checks{k, 1}).(checks{k, 2}) = double(value(:)).' .* ones(1, 3);
    end

    % Profile and PWM limit: {controller, setting, maximum}.
    limits = {'position', 'profileVelocity', MAX_PROFILE
              'position', 'profileAcceleration', MAX_PROFILE
              'position', 'pwmLimit', MAX_PWM
              'velocity', 'profileAcceleration', MAX_PROFILE
              'velocity', 'pwmLimit', MAX_PWM};
    for k = 1:size(limits, 1)
        value = arm.(limits{k, 1}).(limits{k, 2});
        if ~(isnumeric(value) && isscalar(value) && isfinite(value) && value >= 0 ...
             && value <= limits{k, 3})
            error('spot:params:apply', 'The arm''s %s control %s must be 0 to %d.', ...
                limits{k, 1}, limits{k, 2}, limits{k, 3});
        end
        gains.(limits{k, 1}).(limits{k, 2}) = double(value);
    end
end

function p = pose(xyDeg)
    % [x y theta_deg] -> [x y theta_rad].
    p = [xyDeg(1), xyDeg(2), xyDeg(3) * pi/180];
end
