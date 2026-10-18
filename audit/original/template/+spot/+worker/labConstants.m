function values = labConstants(cal, s)
    %LABCONSTANTS The LAB_* constants a lab console calibration stands for (Update from Lab).
    %   values = spot.worker.labConstants(cal, s) turns the lab console's calibration (its
    %   /api/calibration JSON, decoded) into the constants spot.params.labCalibration reads:
    %   the table's height surface, centre, drag and tilt figures; each platform's eight
    %   thrusts, decay, angles and torque ratios; each joint's friction, back-EMF, torque per
    %   volt and armature, worked out by spot.arm.loadCalibration on the project's arm model
    %   (a measured inertia less the links' share). A part the lab hasn't measured, or an arm
    %   calibration measured with the motor current (which the PWM-driven model can't use),
    %   leaves its constants as they are.
    %
    %   Inputs
    %     cal     the lab's calibration struct (table, thrusters.<PLATFORM>, arm)
    %     s       the project's settings (for the arm's geometry)
    %
    %   Outputs
    %     values  struct, constant name -> value, for spot.params.writeConstants
    %
    %   See also spot.params.labCalibration, spot.arm.loadCalibration, spot.worker.Worker.

    %% Constants
    PWM_FORMAT = Constants.ARM_CALIBRATION_FORMAT;   % first calibration version in PWM control
    PLATFORMS = {'RED', 'BLACK', 'BLUE'};
    JOINT_FIELDS = {'static', 'coulomb', 'stribeck', 'viscous'};

    values = struct();

    %% Table
    if isfield(cal, 'table') && isstruct(cal.table) && isfield(cal.table, 'height')
        tb = cal.table;
        values.LAB_TABLE_UPDATED = char(field(tb, 'updated', ''));
        values.LAB_TABLE_MODEL = char(field(tb, 'model', ''));
        values.LAB_TABLE_HEIGHT = double(tb.height(:).');
        values.LAB_TABLE_CENTRE = double(field(tb, 'centre', ...
            [Constants.TABLE_CENTRE_X Constants.TABLE_CENTRE_Y]));
        values.LAB_TABLE_CENTRE = values.LAB_TABLE_CENTRE(:).';
        damping = field(tb, 'damping', struct());
        for name = PLATFORMS
            values.(['LAB_TABLE_DAMPING_' name{1}]) = number(field(damping, name{1}, NaN));
        end
        values.LAB_TABLE_TILT_CENTRE_MRAD = number(field(tb, 'tilt_centre_mrad', NaN));
        values.LAB_TABLE_TILT_MAX_MRAD = number(field(tb, 'tilt_max_mrad', NaN));
        values.LAB_TABLE_DOWNHILL_DEG = number(field(tb, 'downhill_deg', NaN));
    end

    %% Thrusters
    if isfield(cal, 'thrusters') && isstruct(cal.thrusters)
        for name = PLATFORMS
            key = name{1};
            if ~isfield(cal.thrusters, key) || ~isstruct(cal.thrusters.(key))
                continue
            end
            th = cal.thrusters.(key);
            values.(['LAB_THRUST_UPDATED_' key]) = char(field(th, 'updated', ''));
            values.(['LAB_THRUST_' key]) = numbers(th.thrust, 8);
            decay = NaN;
            if isfield(th, 'decay') && isstruct(th.decay) && isfield(th.decay, 'slope') ...
                    && strcmp(field(th.decay, 'model', 'linear'), 'linear')
                decay = number(th.decay.slope);
            end
            values.(['LAB_THRUST_DECAY_' key]) = decay;
            values.(['LAB_THRUST_ANGLE_' key]) = numbers(field(th, 'angle', []), 8);
            values.(['LAB_TORQUE_RATIO_' key]) = numbers(field(th, 'torque_ratio', []), 8);
        end
    end

    %% Arm
    if isfield(cal, 'arm') && isstruct(cal.arm) && isfield(cal.arm, 'joints') ...
            && isfield(cal.arm, 'version') && cal.arm.version >= PWM_FORMAT ...
            && isfield(cal.arm, 'control') && strcmp(cal.arm.control, 'pwm')

        % The project's arm model, with its geometry, so a measured inertia becomes the
        % rotor's armature; then each joint's values as the model holds them.
        base = spot.arm.defaults();
        geometry = spot.arm.mountGeometry(s.mass.arm);
        for f = {'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', ...
                 'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
            base.(f{1}) = geometry.(f{1});
        end
        model = spot.arm.loadCalibration(cal.arm, base);
        measured = false(1, 3);
        joints = cal.arm.joints;
        if isstruct(joints)
            joints = num2cell(joints);
        end
        for k = 1:numel(joints)
            j = joints{k}.index;
            if isnumeric(j) && isscalar(j) && any(j == 1:3)
                measured(j) = true;
            end
        end
        nanWhereNot = @(v) v .* (measured ./ measured);   % NaN where not measured
        for f = JOINT_FIELDS
            values.(['LAB_ARM_' upper(f{1})]) = nanWhereNot(reshape(model.friction.(f{1}), 1, 3));
        end
        values.LAB_ARM_BACK_EMF = nanWhereNot(reshape(model.motor.backEMF, 1, 3));
        values.LAB_ARM_TORQUE_PER_VOLT = nanWhereNot(reshape(model.motor.torquePerVolt, 1, 3));
        values.LAB_ARM_ARMATURE = nanWhereNot(reshape(model.armature, 1, 3));
        values.LAB_ARM_UPDATED = char(field(cal.arm, 'updated', field(cal.arm, 'created', '')));
    end
end

function v = field(s, name, default)
    % s.(name) if present, else default.
    if isstruct(s) && isfield(s, name)
        v = s.(name);
    else
        v = default;
    end
end

function x = number(x)
    % A finite number, else NaN.
    if ~(isnumeric(x) && isscalar(x) && isfinite(x))
        x = NaN;
    else
        x = double(x);
    end
end

function v = numbers(x, n)
    % A 1xn row of numbers (NaN where missing or null); [] when there are none.
    if iscell(x)
        x = cellfun(@(e) ifEmpty(e), x);
    end
    if ~isnumeric(x) || numel(x) ~= n
        v = [];
        return
    end
    v = double(reshape(x, 1, n));
end

function e = ifEmpty(e)
    % NaN for an empty value.
    if isempty(e)
        e = NaN;
    end
end
