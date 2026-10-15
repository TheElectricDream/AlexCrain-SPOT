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
    %     root  project folder (optional). The newer of resources/spot_calibration.json (arm
    %           section, from the Calibration tab) and resources/arm_calibration.json
    %           (calibrate_arm.py's file) is applied with spot.arm.loadCalibration.
    %
    %   Outputs
    %     a     the arm model; a.calibration names the calibration applied, '' if none
    %
    %   Change it in SPOT_User_Parameters.m, e.g. armModel.friction.static = [0.4 0.3 0.2];
    %
    %   See also spot.arm.defaults, spot.arm.loadCalibration, spot.params.apply.

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

    % Table slope and air-bearing drag under the Chaser (spot.sim.tableSlope); level by default.
    a.tableSlope = [0 0 0 0 0 1.75 1.2];
    a.tableDamping = 0;
    if isfield(v, 'tableSlope')
        a.tableSlope = v.tableSlope;
    end
    if isfield(v, 'tableDamping_RED')
        a.tableDamping = v.tableDamping_RED;
    end

    a.outerStep = v.baseRate;
    a.calibration = '';

    %% Calibration
    if nargin > 1 && ~isempty(root)

        % When each calibration was measured ('' if missing or not an arm calibration).
        files = {fullfile(root, 'resources', 'spot_calibration.json'), ...
                 fullfile(root, 'resources', 'arm_calibration.json')};
        created = {'', ''};
        for k = 1:2
            try
                c = jsondecode(fileread(files{k}));
                if k == 1
                    c = c.arm;
                end
                if strcmp(c.format, 'spot-arm-calibration')
                    created{k} = char(c.created);
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
    end
end
