function a = parameters(v, root)
%PARAMETERS The arm model spot.arm.Plant simulates (the workspace variable armModel).
%   a = spot.arm.parameters(v) completes spot.arm.defaults with the values spot.params.apply
%   puts in the workspace (v: mRED, IRED, the arm's geometry and masses, init_states_RED,
%   init_states_ARM, baseRate).
%   a = spot.arm.parameters(v, projectFolder) also applies the project's arm calibration
%   (spot.arm.loadCalibration): the lab's (resources/spot_calibration.json's arm section, from the
%   Calibration tab) or resources/arm_calibration.json (calibrate_arm.py's own file, copied by hand),
%   whichever was measured last; a.calibration names it. The table's slope and the air
%   bearings' drag under the Chaser come from v.tableSlope and v.tableDamping_RED when there
%   (spot.params.apply; spot.sim.tableSlope), else the table is level.
%
%   Change it in SPOT_User_Parameters.m, e.g. armModel.friction.static = [0.4 0.3 0.2];
a = spot.arm.defaults();
for f = {'mRED', 'IRED', 'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', 'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
    a.(f{1}) = v.(f{1});
end
a.q1Offset = 0;                     % the arm turned with its mount face (spot.arm.mountGeometry)
if isfield(v, 'armShoulderOffset')
    a.q1Offset = v.armShoulderOffset;
end
a.x0 = [v.init_states_RED(:); v.init_states_ARM(:)];
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
if nargin > 1 && ~isempty(root)
    % The arm calibrations the project has, and when each was measured ('created': the script's time)
    files = {fullfile(root, 'resources', 'spot_calibration.json'), fullfile(root, 'resources', 'arm_calibration.json')};
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
    if ~isempty(created{1}) && string(created{1}) >= string(created{2})   % (ISO times: as text, in order)
        a = spot.arm.loadCalibration(files{1}, a);
    elseif ~isempty(created{2})
        a = spot.arm.loadCalibration(files{2}, a);
    end
end
end
