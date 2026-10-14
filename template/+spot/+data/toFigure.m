function fig = toFigure(panels, lookup, opts)
%TOFIGURE The Results page's plots as a MATLAB figure in the lab's publication style.
%   fig = spot.data.toFigure(panels, lookup) draws one tile per panel, following the lab's
%   GetStarted templates (github.com/Carleton-SRL/GetStarted): Times New Roman, the platforms'
%   colours (spot.plotting.platformStyle: the Chaser and the arm grey-silver, the Target gold, the
%   Obstacle blue, other signals black), 1 pt lines, grid and box, simulations dashed when shown
%   with experiments. Every sample is drawn.
%   Each panel has
%     traces    list of id, signal, col: a column of a signal of a loaded dataset
%     x         (optional) signal and col to plot against instead of time
%     smooth    (optional) smoothing window [s]
%     xlim, ylim (optional) axis limits
%   A panel of <PLATFORM>_Py_m against a <PLATFORM>_Px_m is drawn as the table seen from above:
%   each platform's path (against its own Px), its outline at the start (faint) and the end
%   (spot.plotting.drawSpacecraft, and the arm), 0-3.5 x 0-2.4 m, equal axes.
%   lookup(id) returns the dataset with that id (see spot.worker.Worker).
%   fig = spot.data.toFigure(..., opts) with opts fields width and height [in] (default 7 x 4,
%   taller for several panels), fontSize [pt] (default 10) and visible (default true).
%   Save it with spot.data.exportFigure.
if nargin < 3
    opts = struct();
end
panels = asCell(panels);
n = numel(panels);
w = field(opts, 'width', 7);
h = field(opts, 'height', max(4, min(9, 2.6 * n + 0.4)));
fs = field(opts, 'fontSize', 10);
visible = 'on';
if ~field(opts, 'visible', true)
    visible = 'off';
end
fig = figure('Name', 'SPOT results', 'Color', 'w', 'Visible', visible, 'Units', 'inches');
pos = get(fig, 'Position');
set(fig, 'Position', [pos(1) pos(2) w h]);
layout = tiledlayout(fig, n, 1, 'TileSpacing', 'compact', 'Padding', 'compact');
for k = 1:n
    ax = nexttile(layout);
    hold(ax, 'on'); grid(ax, 'on'); box(ax, 'on');
    p = panels{k};
    traces = asCell(field(p, 'traces', {}));
    if isTable(p, traces)
        [lines, labels] = tablePlot(ax, p, traces, lookup);
    else
        [lines, labels] = signalPlot(ax, p, traces, lookup);
    end
    ax.FontName = 'Times New Roman';
    ax.FontSize = fs;
    if numel(lines) > 1
        legend(ax, lines, labels, 'Location', 'best', 'Interpreter', 'none', ...
            'FontName', 'Times New Roman', 'FontSize', fs);
    end
end
% Labels and legends too (where this computer has the font; otherwise MATLAB's substitute)
if any(strcmpi(listfonts, 'Times New Roman'))
    fontname(fig, 'Times New Roman');
end
end

% ---------------------------------------------------------------- signals against time (or x)

function [lines, labels] = signalPlot(ax, p, traces, lookup)
xs = field(p, 'x', []);
[colors, styles, names] = styling(traces, lookup);
lines = gobjects(0);
labels = {};
quantities = {}; units = {};
for j = order(traces, lookup)
    tr = traces{j};
    a = struct('signal', tr.signal, 'col', field(tr, 'col', 1), 'x', xs, ...
               'smooth', field(p, 'smooth', 0), 'points', 20000);
    r = spot.data.series(lookup(tr.id), a);
    if isfield(r, 'x')
        xv = r.x;
    else
        xv = r.t;
    end
    lines(end + 1) = plot(ax, xv, r.y, 'Color', colors{j}, 'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
    labels{end + 1} = names{j}; %#ok<AGROW>
    L = spot.plotting.signalLabel(tr.signal);
    quantities{end + 1} = L.label; %#ok<AGROW>
    units{end + 1} = L.unit; %#ok<AGROW>
end
if isstruct(xs) && ~isempty(field(xs, 'signal', ''))
    xlabel(ax, spot.plotting.signalLabel(xs.signal).label);
else
    xlabel(ax, 'Time [s]');
end
quantities = unique(quantities);
units = unique(units);
if isscalar(quantities)
    ylabel(ax, quantities{1});
elseif isscalar(units) && ~isempty(units{1})
    ylabel(ax, sprintf('%s [%s]', unitQuantity(units{1}), units{1}));
end
if numel(field(p, 'xlim', [])) == 2
    xlim(ax, p.xlim);
elseif ~isempty(lines)
    xlim(ax, 'tight');
end
if numel(field(p, 'ylim', [])) == 2
    ylim(ax, p.ylim);
end
end

% ---------------------------------------------------------------- the table from above

function yes = isTable(p, traces)
xs = field(p, 'x', []);
yes = isstruct(xs) && ~isempty(regexp(field(xs, 'signal', ''), '^(RED|BLACK|BLUE)_Px_m$', 'once')) && ...
      ~isempty(traces) && all(cellfun(@(t) ~isempty(regexp(t.signal, '^(RED|BLACK|BLUE)_Py_m$', 'once')), traces));
end

function [lines, labels] = tablePlot(ax, p, traces, lookup)
[colors, styles, names] = styling(traces, lookup);
lines = gobjects(0);
labels = {};
paths = {};
for j = order(traces, lookup)
    tr = traces{j};
    ds = lookup(tr.id);
    P = tr.signal(1:end - 5);   % RED_Py_m -> RED
    [t, x] = spot.data.samples(ds.signals.([P '_Px_m']));
    [ty, y] = spot.data.samples(ds.signals.([P '_Py_m']));
    [ta, th] = spot.data.samples(ds.signals.([P '_Rz_rad']));
    x = x(:, 1);
    y = interp1(ty, y(:, 1), t, 'linear', NaN);
    th = interp1(ta, th(:, 1), t, 'linear', NaN);
    valid = isfinite(x) & isfinite(y);
    if ~strcmp(ds.kind, 'simulation')
        valid = valid & ~(x == 0 & y == 0);   % not seen by PhaseSpace yet
    end
    idx = find(valid);
    if isempty(idx)
        continue
    end
    % Outlines first, so the paths are drawn over them: faint at the start, solid at the end
    b = spot.data.bodies(ds);
    shapes = struct('RED', 1, 'BLACK', 4, 'BLUE', 3);
    id = shapes.(P);
    arm = strcmp(P, 'RED') && b.arm;
    if arm
        id = 5;
    end
    snaps = [idx(1), idx(end)];
    alphas = [0.2, 1];
    for s = 1:2
        i = snaps(s);
        pts = spot.plotting.drawSpacecraft(x(i), y(i), th(i), id);
        patch(ax, pts(:, 1), pts(:, 2), 'w', 'FaceAlpha', 0.5, 'EdgeColor', colors{j}, ...
            'EdgeAlpha', alphas(s), 'LineWidth', 0.5);
        if arm
            q = zeros(1, 3);
            jn = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
            for m = 1:3
                [tj, v] = spot.data.samples(ds.signals.(jn{m}));
                q(m) = interp1(tj, v(:, 1), t(i), 'linear', 0);
            end
            geometry = spot.params.defaults().mass.arm;
            if isstruct(ds.settings)
                geometry = ds.settings.mass.arm;
            end
            for link = spot.plotting.drawArm(spot.plotting.armPosition([x(i) y(i) th(i)], q, spot.arm.mountGeometry(geometry)))
                patch(ax, link{1}(:, 1), link{1}(:, 2), 'w', 'FaceAlpha', alphas(s), 'EdgeColor', colors{j}, ...
                    'EdgeAlpha', alphas(s), 'LineWidth', 0.5);
            end
        end
    end
    paths{end + 1} = {x(valid), y(valid), j}; %#ok<AGROW>
end
for k = 1:numel(paths)
    j = paths{k}{3};
    lines(end + 1) = plot(ax, paths{k}{1}, paths{k}{2}, 'Color', colors{j}, 'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
    labels{end + 1} = names{j}; %#ok<AGROW>
end
xlabel(ax, 'X-Position [m]');
ylabel(ax, 'Y-Position [m]');
xl = field(p, 'xlim', [0 3.5]);
yl = field(p, 'ylim', [0 2.4]);
xlim(ax, xl); ylim(ax, yl);
daspect(ax, [1 1 1]);
end

% ---------------------------------------------------------------- colours, line styles, names

function [colors, styles, names] = styling(traces, lookup)
% Colour by platform; within a colour, solid first, then dashed, dotted, dash-dot, with the
% experiments (and files) before the simulations, so a simulation over a trial is dashed
n = numel(traces);
colors = cell(1, n); styles = cell(1, n); names = cell(1, n);
lineStyles = {'-', '--', ':', '-.'};
used = containers.Map();
labels = cellfun(@(t) spot.plotting.signalLabel(t.signal), traces, 'UniformOutput', false);
labels = [labels{:}];
sets = unique(cellfun(@(t) t.id, traces, 'UniformOutput', false));
sameQuantity = isscalar(unique({labels.quantity}));
for j = order(traces, lookup)
    L = labels(j);
    key = L.platform;
    if isempty(key)
        key = 'none';
    end
    [shown, colors{j}] = spot.plotting.platformStyle(key);
    c = 0;
    if isKey(used, key)
        c = used(key);
    end
    used(key) = c + 1;
    styles{j} = lineStyles{mod(c, numel(lineStyles)) + 1};
    parts = {};
    if ~isempty(L.platform)
        parts{end + 1} = shown; %#ok<AGROW>
    end
    if ~sameQuantity || isempty(L.platform)
        parts{end + 1} = L.quantity; %#ok<AGROW>
    end
    col = field(traces{j}, 'col', 1);
    if col > 1 || size(lookup(traces{j}.id).signals.(traces{j}.signal).Data, 2) > 1
        parts{end + 1} = sprintf('%d', col); %#ok<AGROW>
    end
    name = strjoin(parts, ' ');
    if numel(sets) > 1
        name = sprintf('%s (%s)', name, lookup(traces{j}.id).name);
    end
    names{j} = name;
end
end

function idx = order(traces, lookup)
sim = cellfun(@(t) strcmp(lookup(t.id).kind, 'simulation'), traces);
idx = [find(~sim), find(sim)];
idx = idx(:).';
end

function q = unitQuantity(unit)
m = containers.Map({'m', 'rad', 'm/s', 'rad/s', 'm/s^2', 'rad/s^2', 'N', 'N m', 's'}, ...
    {'Position', 'Angle', 'Velocity', 'Angular Rate', 'Acceleration', 'Angular Acceleration', 'Force', 'Torque', 'Time'});
q = 'Value';
if isKey(m, unit)
    q = m(unit);
end
end

% ---------------------------------------------------------------- helpers

function v = field(s, name, default)
if isstruct(s) && isfield(s, name) && ~isempty(s.(name))
    v = s.(name);
else
    v = default;
end
end

function c = asCell(x)
if iscell(x)
    c = x;
elseif isstruct(x)
    c = num2cell(x);
else
    c = {};
end
c = c(:).';
end
