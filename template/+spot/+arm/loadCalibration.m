function a = loadCalibration(file, a)
    %LOADCALIBRATION Apply the arm calibration (from calibrate_arm.py) to a model.
    %   a = spot.arm.loadCalibration(file, a) replaces each calibrated joint's friction and,
    %   when measured, its armature. A value the file lacks (or has as null) keeps the model's.
    %
    %   Inputs
    %     file  the calibration script's JSON file, the lab's calibration (the project's
    %           spot_calibration.json, its arm section), or the calibration as a struct
    %     a     arm model (spot.arm.defaults or spot.arm.parameters)
    %
    %   Outputs
    %     a     the model with the calibration; a.calibration names the source
    %
    %   Example
    %     armModel = spot.arm.loadCalibration('arm_calibration.json', armModel);
    %
    %   See also spot.arm.parameters, spot.arm.compareSteps.

    %% Read the calibration
    if isstruct(file)
        c = file;
        file = 'the arm calibration';
    else
        c = jsondecode(fileread(file));

        % The lab's calibration: use its arm section.
        if isfield(c, 'format') && strcmp(c.format, 'spot-calibration')
            if ~isfield(c, 'arm') || ~isstruct(c.arm)
                error('spot:arm:loadCalibration', '%s has no arm calibration.', file);
            end
            c = c.arm;
            file = [file ' (arm)'];
        end
    end
    if ~isfield(c, 'joints')
        error('spot:arm:loadCalibration', ...
            '%s has no "joints" (not an arm calibration file?).', file);
    end

    % Joints as a cell array, whether jsondecode gave a struct array or one struct.
    joints = c.joints;
    if isstruct(joints) && ~isscalar(joints)
        joints = num2cell(joints);
    elseif isstruct(joints)
        joints = {joints};
    end

    %% Apply each joint
    fields = {'static', 'coulomb', 'stribeck', 'viscous'};
    for k = 1:numel(joints)
        jt = joints{k};
        j = jt.index;   % 1 shoulder, 2 elbow, 3 wrist
        if ~(isnumeric(j) && isscalar(j) && any(j == 1:3))
            error('spot:arm:loadCalibration', '%s: joint index must be 1, 2 or 3.', file);
        end

        % Friction: each finite scalar value.
        for f = fields
            if isfield(jt, f{1}) && isnumeric(jt.(f{1})) && isscalar(jt.(f{1})) ...
                    && isfinite(jt.(f{1}))
                a.friction.(f{1})(j) = jt.(f{1});
            end
        end

        % Armature: given directly, or the measured inertia about the joint (base fixed, other
        % joints held) less the links' share at that pose.
        if isfield(jt, 'armature') && isnumeric(jt.armature) && isscalar(jt.armature) ...
                && jt.armature > 0
            a.armature(j) = jt.armature;
        elseif isfield(jt, 'inertia') && isnumeric(jt.inertia) && isscalar(jt.inertia) ...
                && isfield(jt, 'pose') && numel(jt.pose) == 3 && isfield(a, 'IEE')
            M = spot.arm.inertiaMatrix(a, [0; 0; 0; jt.pose(:); zeros(6, 1)]);
            rotor = jt.inertia - M(3 + j, 3 + j);
            if rotor > 0
                a.armature(j) = rotor;
            else
                warning('spot:arm:loadCalibration', ...
                    ['%s: joint %d''s measured inertia (%.4g kg m^2) is below its links'' ' ...
                     '(%.4g); its armature is left at %.4g.'], ...
                    file, j, jt.inertia, M(3 + j, 3 + j), a.armature(j));
            end
        end
    end
    a.calibration = file;
end
