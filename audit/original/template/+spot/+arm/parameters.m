function a = parameters(v, root)
    %PARAMETERS The arm model spot.arm.Plant simulates (the workspace variable armModel).
    %   a = spot.arm.parameters(v) completes spot.arm.defaults with the workspace values from
    %   spot.params.apply.
    %   a = spot.arm.parameters(v, root) also applies the project's arm calibration.
    %
    %   Inputs
    %     v     workspace values: mRED, IRED, the arm's geometry and masses, init_states_RED,
    %           init_states_ARM, baseRate; optional armShoulderOffset, tableSlope,
    %           tableDamping_RED (else the table is level)
    %     root  project folder (optional). The newer of spot_calibration.json (arm section,
    %           from the lab console's Calibration tab) and arm_calibration.json
    %           (calibrate_arm.py's file), both in the project folder, is applied with
    %           spot.arm.loadCalibration; then the arm values the project overrides (the
    %           Simulation tab's Calibration pane, spot.params.loadCalibration).
    %
    %   Outputs
    %     a     the arm model; a.calibration names the calibration applied, '' if none;
    %           a.calibrationIgnored names one left out because it was measured with the
    %           motor current (calibration version 1), '' if none
    %
    %   Change it in SPOT_User_Parameters.m, e.g. armModel.friction.static = [0.4 0.3 0.2];
    %
    %   See also spot.arm.defaults, spot.arm.loadCalibration, spot.params.apply.

    %% Constants
    PWM_FORMAT = 2;   % first arm calibration version measured in PWM control

    %% Model from the workspace
    a = spot.arm.defaults();

    % Masses, inertias and link geometry.
    for f = {'mRED', 'IRED', 'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', ...
             'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
        a.(f{1}) = v.(f{1});
    end

    % Shoulder angle offset from the mount face (spot.arm.mountGeometry).
    a.q1Offset = 0;
    if isfield(v, 'armShoulderOffset')
        a.q1Offset = v.armShoulderOffset;
    end

    % Initial state: base, then joints.
    a.x0 = [v.init_states_RED(:); v.init_states_ARM(:)];

    % Table slope and air-bearing drag under the Chaser (spot.dynamics.tableSlope); level by
    % default.
    a.tableSlope = [0 0 0 0 0 1.755775 1.209675];
    a.tableDamping = 0;
    if isfield(v, 'tableSlope')
        a.tableSlope = v.tableSlope;
    end
    if isfield(v, 'tableDamping_RED')
        a.tableDamping = v.tableDamping_RED;
    end

    a.outerStep = v.baseRate;
    a.calibration = '';
    a.calibrationIgnored = '';

    %% Calibration
    if nargin > 1 && ~isempty(root)

        % The two files: the project's calibration (where spot.params.loadCalibration found
        % it), and calibrate_arm.py's (a project made before the files moved has it in
        % resources/).
        lab = spot.params.loadCalibration(root);
        script = fullfile(root, 'arm_calibration.json');
        if ~isfile(script)
            script = fullfile(root, 'resources', 'arm_calibration.json');
        end
        files = {lab.file, script};

        % When each calibration was measured ('' if missing, not an arm calibration, or
        % measured with the motor current, which the PWM-driven model can't use).
        created = {'', ''};
        for k = 1:2
            try
                c = jsondecode(fileread(files{k}));
                if k == 1
                    c = c.arm;
                end
                if strcmp(c.format, 'spot-arm-calibration')
                    if isfield(c, 'version') && c.version >= PWM_FORMAT ...
                            && isfield(c, 'control') && strcmp(c.control, 'pwm')
                        created{k} = char(c.created);
                    else
                        a.calibrationIgnored = files{k};
                    end
                end
            catch
            end
        end

        % Apply the newer one. ISO times sort correctly as text.
        if ~isempty(created{1}) && string(created{1}) >= string(created{2})
            a = spot.arm.loadCalibration(files{1}, a);
        elseif ~isempty(created{2})
            a = spot.arm.loadCalibration(files{2}, a);
        end

        % Then the values the project overrides (NaN: not overridden).
        ov = lab.overrides.arm;
        for f = {'static', 'coulomb', 'stribeck', 'viscous'}
            given = isfinite(ov.(f{1}));
            a.friction.(f{1})(given) = ov.(f{1})(given);
        end
        given = isfinite(ov.armature);
        a.armature(given) = ov.armature(given);
        given = isfinite(ov.back_emf);
        a.motor.backEMF(given) = ov.back_emf(given);
    end
end
