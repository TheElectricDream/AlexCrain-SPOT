function a = parameters(v, cal)
    %PARAMETERS The arm model spot.arm.Plant simulates (the workspace variable armModel).
    %   a = spot.arm.parameters(v) completes spot.arm.defaults with the workspace values from
    %   spot.params.apply.
    %   a = spot.arm.parameters(v, cal) also applies the arm calibration the project took from
    %   the lab (cal = spot.params.labCalibration(): its arm section, Constants.LAB_ARM_*, with
    %   spot.arm.loadCalibration), then the arm values the project overrides (the Simulation
    %   tab's Calibration pane, cal.overrides.arm).
    %
    %   Inputs
    %     v     workspace values: mRED, IRED, the arm's geometry and masses, init_states_RED,
    %           init_states_ARM, baseRate; optional armShoulderOffset, tableSlope,
    %           tableDamping_RED (else the table is level)
    %     cal   the project's calibration (optional)
    %
    %   Outputs
    %     a     the arm model; a.calibration names the calibration applied, '' if none
    %
    %   Change the project's Constants.m to change it (ARM_STATIC, ARM_COULOMB, ...).
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
    a.tableSlope = Constants.LEVEL_TABLE;
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
    if nargin > 1 && ~isempty(cal)

        % The lab's arm calibration, as the project took it.
        if ~isempty(cal.arm)
            a = spot.arm.loadCalibration(cal.arm, a);
            a.calibration = 'Constants.m (LAB_ARM_*)';
        end

        % Then the values the project overrides (NaN: not overridden).
        ov = cal.overrides.arm;
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
