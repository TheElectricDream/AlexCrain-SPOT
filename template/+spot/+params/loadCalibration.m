function c = loadCalibration(root)
%LOADCALIBRATION The project's calibration of the lab (resources/spot_calibration.json).
%   c = spot.params.loadCalibration(projectFolder) reads the calibration the lab console measured
%   (its Calibration tab), as the local console copied it into the project. Returns a struct:
%     file      the file ('' if the project has none)
%     updated   when its newest part was measured (ISO 8601, UTC; '' if none)
%     table     the table slope calibration, or [] if none:
%                 slope    [a1 a2 a3 a4 a5 xc yc], the table top's height surface (spot.sim.tableSlope)
%                 damping  struct RED, BLACK, BLUE: the air bearings' drag [1/s] (0 if not measured)
%                 model, updated, tilt_centre_mrad, tilt_max_mrad, downhill_deg
%     thrusters struct RED, BLACK, BLUE: each platform's thruster calibration, or [] if none:
%                 thrust   8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>)
%                 updated, angle (8x1 [deg], how far each thrust is turned), torqueRatio (8x1)
%     arm       the arm calibration (calibrate_arm.py's, from the Calibration tab), or [] if none: its
%               joints' friction and inertia, created, updated, simulated (spot.arm.parameters applies it)
%     error     why the file couldn't be used ('' if it could)
%   spot.params.apply uses it (Scenario: Table Slope); a file that can't be read is reported and
%   ignored, so simulations still run on a level table.

c = struct('file', '', 'updated', '', 'table', [], 'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], 'error', '');
file = fullfile(root, 'resources', 'spot_calibration.json');
if ~isfile(file)
    return
end
c.file = file;
try
    x = jsondecode(fileread(file));
    if ~isfield(x, 'format') || ~strcmp(x.format, 'spot-calibration')
        error('spot:params:loadCalibration', 'not a SPOT calibration file');
    end
    if isfield(x, 'updated') && ischar(x.updated)
        c.updated = x.updated;
    end
    if isfield(x, 'table') && isstruct(x.table)
        t = x.table;
        h = double(t.height(:).');
        centre = [1.75 1.2];
        if isfield(t, 'centre')
            centre = double(t.centre(:).');
        end
        if numel(h) ~= 5 || numel(centre) ~= 2 || ~all(isfinite([h centre]))
            error('spot:params:loadCalibration', 'table.height must be 5 numbers and table.centre 2');
        end
        damping = struct('RED', 0, 'BLACK', 0, 'BLUE', 0);
        if isfield(t, 'damping') && isstruct(t.damping)
            for n = {'RED', 'BLACK', 'BLUE'}
                if isfield(t.damping, n{1}) && isfinite(t.damping.(n{1}))
                    damping.(n{1}) = double(t.damping.(n{1}));
                end
            end
        end
        c.table = struct('slope', [h centre], 'damping', damping, 'model', field(t, 'model', ''), ...
            'updated', field(t, 'updated', ''), 'tilt_centre_mrad', field(t, 'tilt_centre_mrad', NaN), ...
            'tilt_max_mrad', field(t, 'tilt_max_mrad', NaN), 'downhill_deg', field(t, 'downhill_deg', NaN));
    end
    if isfield(x, 'thrusters') && isstruct(x.thrusters)
        for n = {'RED', 'BLACK', 'BLUE'}
            if ~isfield(x.thrusters, n{1})
                continue
            end
            p = x.thrusters.(n{1});
            F = double(p.thrust(:));
            if numel(F) ~= 8 || ~all(isfinite(F)) || any(F < 0.01 | F > 2)
                error('spot:params:loadCalibration', 'thrusters.%s.thrust must be 8 thrusts of 0.01 to 2 N', n{1});
            end
            c.thrusters.(n{1}) = struct('thrust', F, 'updated', field(p, 'updated', ''), ...
                'angle', numbers(field(p, 'angle', [])), 'torqueRatio', numbers(field(p, 'torque_ratio', [])));
        end
    end
    if isfield(x, 'arm') && isstruct(x.arm) && isfield(x.arm, 'format') && strcmp(x.arm.format, 'spot-arm-calibration')
        c.arm = struct('created', field(x.arm, 'created', ''), 'updated', field(x.arm, 'updated', ''), ...
            'joints', {field(x.arm, 'joints', [])});
    end
catch err
    c = struct('file', file, 'updated', '', 'table', [], 'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
        'error', sprintf('%s could not be used (%s); the simulation ignores it.', file, err.message));
end
end

function v = numbers(x)
% 8x1 numbers from a JSON list (null, a thruster without a result, and a missing list: NaN)
v = nan(8, 1);
if iscell(x)
    x = cellfun(@(e) ifEmpty(e), x);
end
if isnumeric(x) && numel(x) == 8
    v = double(x(:));
end
end

function e = ifEmpty(e)
if isempty(e)
    e = NaN;
end
end

function v = field(s, name, default)
if isfield(s, name)
    v = s.(name);
else
    v = default;
end
end
