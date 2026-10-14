function a = loadCalibration(file, a)
%LOADCALIBRATION Apply the arm calibration (resources/arm_calibration/calibrate_arm.py) to a model.
%   a = spot.arm.loadCalibration(file, a) reads the JSON file the calibration script writes and
%   replaces, for each joint it has, the friction (static, coulomb, stribeck, viscous) and, when
%   measured, the armature, in the arm model a (spot.arm.defaults / spot.arm.parameters). A
%   value the file doesn't have (or has as null) keeps the model's. FILE may also be the lab's
%   calibration (resources/spot_calibration.json: its arm section, which the Calibration tab's arm
%   calibration writes), or the calibration itself as a struct.
%
%   Example:
%     armModel = spot.arm.loadCalibration('resources/arm_calibration.json', armModel);
if isstruct(file)
    c = file;
    file = 'the arm calibration';
else
    c = jsondecode(fileread(file));
    if isfield(c, 'format') && strcmp(c.format, 'spot-calibration')   % the lab's calibration: its arm section
        if ~isfield(c, 'arm') || ~isstruct(c.arm)
            error('spot:arm:loadCalibration', '%s has no arm calibration.', file);
        end
        c = c.arm;
        file = [file ' (arm)'];
    end
end
if ~isfield(c, 'joints')
    error('spot:arm:loadCalibration', '%s has no "joints" (not an arm calibration file?).', file);
end
joints = c.joints;
if isstruct(joints) && ~isscalar(joints)
    joints = num2cell(joints);
elseif isstruct(joints)
    joints = {joints};
end
fields = {'static', 'coulomb', 'stribeck', 'viscous'};
for k = 1:numel(joints)
    jt = joints{k};
    j = jt.index;   % 1 shoulder, 2 elbow, 3 wrist
    if ~(isnumeric(j) && isscalar(j) && any(j == 1:3))
        error('spot:arm:loadCalibration', '%s: joint index must be 1, 2 or 3.', file);
    end
    for f = fields
        if isfield(jt, f{1}) && isnumeric(jt.(f{1})) && isscalar(jt.(f{1})) && isfinite(jt.(f{1}))
            a.friction.(f{1})(j) = jt.(f{1});
        end
    end
    if isfield(jt, 'armature') && isnumeric(jt.armature) && isscalar(jt.armature) && jt.armature > 0
        a.armature(j) = jt.armature;
    elseif isfield(jt, 'inertia') && isnumeric(jt.inertia) && isscalar(jt.inertia) && isfield(jt, 'pose') ...
            && numel(jt.pose) == 3 && isfield(a, 'IEE')
        % the inertia measured about the joint (base fixed, the other joints held) less the links'
        % part at that pose is the rotor's
        M = spot.arm.inertiaMatrix(a, [0; 0; 0; jt.pose(:); zeros(6, 1)]);
        rotor = jt.inertia - M(3 + j, 3 + j);
        if rotor > 0
            a.armature(j) = rotor;
        else
            warning('spot:arm:loadCalibration', ['%s: joint %d''s measured inertia (%.4g kg m^2) is below its links'' ' ...
                '(%.4g); its armature is left at %.4g.'], file, j, jt.inertia, M(3 + j, 3 + j), a.armature(j));
        end
    end
end
a.calibration = file;
end
