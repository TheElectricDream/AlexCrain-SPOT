function r = results(dataClass, s, d, maxRate)
%RESULTS Trajectory of a simulation for the SPOT Console's 3D view.
%   r = spot.sim.results(dataClass, s, d) takes the logged data (simOut.dataPacket), the settings
%   and the values from spot.params.apply, and returns, resampled to at most 30 Hz:
%     t                 time [s]
%     crafts.<NAME>     x, y [m] and att [rad, unwrapped] of each selected platform
%     arm               4 x 2 x N joint positions [m] (shoulder, elbow, wrist, end effector)
%     phases            end time of each phase and sub-phase [s]
%     signals           name and size of every logged signal
%   r = spot.sim.results(..., maxRate) uses another maximum rate [Hz].
if nargin < 4
    maxRate = 30;
end
time = unique(timeOf(dataClass));
n = max(2, min(numel(time), ceil((time(end) - time(1)) * maxRate) + 1));
t = linspace(time(1), time(end), n).';

r = struct();
r.t = t.';
r.samples = numel(time);
r.crafts = struct();
names = {'RED', 'BLACK', 'BLUE'};
for k = 1:3
    n = names{k};
    if ~s.platforms.(n) || ~isfield(dataClass, [n '_Px_m'])
        continue
    end
    x = resample1(dataClass.([n '_Px_m']), t);
    y = resample1(dataClass.([n '_Py_m']), t);
    a = resample1(dataClass.([n '_Rz_rad']), t, true);
    r.crafts.(n) = struct('x', x.', 'y', y.', 'att', a.');
end
if s.platforms.ARM && isfield(r.crafts, 'RED') && isfield(dataClass, 'ARM_Rz_Shoulder_rad')
    q = [resample1(dataClass.ARM_Rz_Shoulder_rad, t), ...
         resample1(dataClass.ARM_Rz_Elbow_rad, t), ...
         resample1(dataClass.ARM_Rz_Wrist_rad, t)];
    base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
    r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(s.mass.arm));
end

r.phases = d.timing.phases;   % the segments that ran (skipped ones left out)
r.signals = signalList(dataClass);
end

function time = timeOf(dataClass)
if isfield(dataClass, 'Time_s') && isa(dataClass.Time_s, 'timeseries')
    time = dataClass.Time_s.Time;
else
    f = fieldnames(dataClass);
    for k = 1:numel(f)
        if isa(dataClass.(f{k}), 'timeseries')
            time = dataClass.(f{k}).Time;
            return
        end
    end
    error('spot:sim:results', 'The logged data has no timeseries.');
end
end

function y = resample1(ts, t, isAngle)
% A signal (first column) at times t, on its own time stamps (repeated ones dropped)
[time, keep] = unique(ts.Time(:));
data = firstColumn(ts.Data);
data = data(keep);
if nargin > 2 && isAngle
    data = unwrap(data);
end
y = interp1(time, data, t, 'linear', 'extrap');
end

function list = signalList(dataClass)
list = struct('name', {}, 'size', {});
for f = fieldnames(dataClass).'
    x = dataClass.(f{1});
    if isa(x, 'timeseries')
        list(end + 1) = struct('name', f{1}, 'size', size(x.Data)); %#ok<AGROW>
    elseif isstruct(x)
        for g = fieldnames(x).'
            if isa(x.(g{1}), 'timeseries')
                list(end + 1) = struct('name', [f{1} '.' g{1}], 'size', size(x.(g{1}).Data)); %#ok<AGROW>
            end
        end
    end
end
end

function c = firstColumn(data)
% First signal of logged data stored as N x m, or as 1 x 1 x N (Simulink's 3-D format)
data = double(data);
if ndims(data) == 3
    data = reshape(data, [], size(data, 3)).';
end
c = data(:, 1);
end
