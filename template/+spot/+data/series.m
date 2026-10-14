function r = series(ds, a)
%SERIES One signal of a dataset, ready to plot in the console.
%   r = spot.data.series(dataset, args) with args fields
%     signal    signal name (field of dataset.signals)
%     col       column, 1-based (default 1)
%     x         (optional) struct with signal and col: plot against that signal instead of time
%               (it is interpolated at this signal's time stamps); for an experiment, samples
%               where both are 0 are left out (the platform wasn't seen yet). A platform's Py
%               against any platform's Px uses its own Px (the table seen from above).
%     smooth    (optional) smoothing window [s], Gaussian (0: off)
%     range     (optional) [t0 t1]: only that time span
%     points    (optional) at most this many points (default 4000)
%   returns t (time [s]) and y, and x when asked for; n is the number of samples before
%   thinning. Long signals are thinned to the minimum and maximum of each slice of time, so
%   peaks stay visible.
a = defaults(a);
ts = signal(ds, a.signal);
[t, y] = spot.data.samples(ts);
if a.col > size(y, 2)
    error('spot:data:series', '%s has %d column(s).', a.signal, size(y, 2));
end
y = y(:, a.col);
hasX = isstruct(a.x) && isfield(a.x, 'signal') && ~isempty(a.x.signal);
if hasX
    % The table seen from above: <P>_Py_m against any <Q>_Px_m uses P's own Px
    own = regexp(a.signal, '^(RED|BLACK|BLUE)_Py_m$', 'tokens', 'once');
    if ~isempty(own) && ~isempty(regexp(a.x.signal, '^(RED|BLACK|BLUE)_Px_m$', 'once'))
        a.x.signal = [own{1} '_Px_m'];
        a.x.col = 1;
    end
end
if hasX
    xcol = 1;
    if isfield(a.x, 'col') && ~isempty(a.x.col)
        xcol = a.x.col;
    end
    [tx, x] = spot.data.samples(signal(ds, a.x.signal));
    x = interp1(tx, x(:, min(xcol, end)), t, 'linear', NaN);
    if ~strcmp(ds.kind, 'simulation')
        % (0, 0): a platform PhaseSpace hadn't seen yet (experiment logs start with these)
        keep = ~(x == 0 & y == 0);
        t = t(keep); x = x(keep); y = y(keep);
    end
end
if a.smooth > 0 && numel(t) > 2
    w = max(1, round(a.smooth / median(diff(t))));
    if w > 1
        y = smoothdata(y, 'gaussian', w);
        if hasX
            x = smoothdata(x, 'gaussian', w);
        end
    end
end
if numel(a.range) == 2 && all(isfinite(a.range))
    i0 = max(1, find(t >= a.range(1), 1) - 1);
    i1 = min(numel(t), find(t <= a.range(2), 1, 'last') + 1);
    if isempty(i0) || isempty(i1) || i1 < i0
        i0 = 1; i1 = 0;
    end
    t = t(i0:i1); y = y(i0:i1);
    if hasX
        x = x(i0:i1);
    end
end
r = struct('signal', a.signal, 'col', a.col, 'n', numel(t), 'units', char(ts.DataInfo.Units));
if hasX
    pick = unique(round(linspace(1, numel(t), min(numel(t), a.points))));
    r.t = t(pick).';
    r.x = x(pick).';
    r.y = y(pick).';
    return
end
pick = thin(y, a.points);
r.t = t(pick).';
r.y = y(pick).';
end

function a = defaults(a)
d = struct('signal', '', 'col', 1, 'x', [], 'smooth', 0, 'range', [], 'points', 4000);
for f = fieldnames(d).'
    if ~isfield(a, f{1}) || isempty(a.(f{1}))
        a.(f{1}) = d.(f{1});
    end
end
a.col = max(1, round(double(a.col)));
a.points = max(10, min(20000, round(double(a.points))));
a.range = double(a.range(:)).';
end

function ts = signal(ds, name)
if ~isfield(ds.signals, name)
    error('spot:data:series', '%s has no signal %s.', ds.name, name);
end
ts = ds.signals.(name);
end

function pick = thin(y, points)
% Indices to keep: all of them when there are few; otherwise the first, the last, and the
% minimum and maximum of each of points/2 slices
n = numel(y);
if n <= points
    pick = 1:n;
    return
end
slices = floor(points / 2);
edges = round(linspace(1, n + 1, slices + 1));
pick = zeros(1, 2 * slices);
for k = 1:slices
    i = edges(k):edges(k + 1) - 1;
    [~, lo] = min(y(i));
    [~, hi] = max(y(i));
    pick(2 * k - 1:2 * k) = sort(i([lo hi]));
end
pick = unique([1 pick n]);
end
