function r = trajectory(ds, maxRate)
%TRAJECTORY A dataset's motion for the console's 3D view.
%   r = spot.data.trajectory(dataset) returns, resampled to at most 30 Hz:
%     t               time [s]
%     crafts.<NAME>   x, y [m] and att [rad, unwrapped] of each platform (see spot.data.bodies);
%                     NaN where that platform has no position (before its log starts, or a
%                     sample of all zeros in an experiment: PhaseSpace didn't see it yet)
%     arm             4 x 2 x N joint positions [m], when the arm is in it
%     phases          names and end times of the phases, when the dataset's settings are known
%   r = spot.data.trajectory(dataset, maxRate) uses another maximum rate [Hz].
if nargin < 2
    maxRate = 30;
end
sig = ds.signals;
b = spot.data.bodies(ds);
r = struct('t', [], 'crafts', struct());
if isempty(b.crafts)
    error('spot:data:trajectory', '%s has no platform positions (<NAME>_Px_m, _Py_m, _Rz_rad) to show.', ds.name);
end
measured = ~strcmp(ds.kind, 'simulation');

% One time grid for all: from the first to the last position of any platform
raw = struct();
t0 = inf; t1 = -inf; n = 0;
for k = 1:numel(b.crafts)
    c = b.crafts{k};
    [t, x] = spot.data.samples(sig.([c '_Px_m']));
    [~, y] = spot.data.samples(sig.([c '_Py_m']));
    [ta, a] = spot.data.samples(sig.([c '_Rz_rad']));
    x = x(:, 1); y = y(:, 1);
    valid = true(size(t));
    if measured
        valid = ~(x == 0 & y == 0);
    end
    raw.(c) = struct('t', t(valid), 'x', x(valid), 'y', y(valid), 'ta', ta, 'a', unwrap(a(:, 1)));
    if any(valid)
        t0 = min(t0, raw.(c).t(1));
        t1 = max(t1, raw.(c).t(end));
        n = max(n, nnz(valid));
    end
end
if ~isfinite(t0)
    error('spot:data:trajectory', '%s has no position other than zero.', ds.name);
end
if measured
    t0 = min(t0, 0);   % keep the experiment's own clock: 0 is when the program started
end
m = max(2, min(n, ceil((t1 - t0) * maxRate) + 1));
grid = linspace(t0, t1, m).';
r.t = grid.';
for k = 1:numel(b.crafts)
    c = b.crafts{k};
    q = raw.(c);
    if numel(q.t) < 2
        continue
    end
    r.crafts.(c) = struct('x', interp1(q.t, q.x, grid, 'linear', NaN).', ...
                          'y', interp1(q.t, q.y, grid, 'linear', NaN).', ...
                          'att', interp1(q.ta, q.a, grid, 'linear', NaN).');
    r.crafts.(c).att(isnan(r.crafts.(c).x)) = NaN;
end

if b.arm && isfield(r.crafts, 'RED')
    joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
    q = zeros(m, 3);
    for j = 1:3
        [tj, v] = spot.data.samples(sig.(joints{j}));
        q(:, j) = interp1(tj, v(:, 1), grid, 'linear', NaN);
    end
    arm = spot.params.defaults().mass.arm;
    if isstruct(ds.settings)
        arm = ds.settings.mass.arm;
    end
    base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
    r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(arm));
end

if isstruct(ds.settings)
    try
        r.phases = spot.params.timing(ds.settings).phases;
    catch
    end
end
end
