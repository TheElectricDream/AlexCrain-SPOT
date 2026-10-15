function fig = toFigure(panels, lookup, opts)
    %TOFIGURE The Results page's plots as a MATLAB figure in the lab's publication style.
    %   fig = spot.data.toFigure(panels, lookup) draws one tile per panel in the style of the
    %   lab's GetStarted templates (github.com/Carleton-SRL/GetStarted): Times New Roman, the
    %   platforms' colours (spot.plotting.platformStyle), 1 pt lines, grid and box, simulations
    %   dashed when shown with experiments. Every sample is drawn.
    %   fig = spot.data.toFigure(panels, lookup, opts) sets the size, font and visibility.
    %
    %   A panel of <PLATFORM>_Py_m against <PLATFORM>_Px_m is drawn as the table from above:
    %   each platform's path (against its own Px), its outline at the start (faint) and the end
    %   (with the arm), 0-3.5 x 0-2.4 m, equal axes.
    %
    %   Inputs
    %     panels  struct array or cell array, each with:
    %               traces      list of id, signal, col: a column of a loaded dataset's signal
    %               x           optional signal and col to plot against instead of time
    %               smooth      optional smoothing window [s]
    %               xlim, ylim  optional axis limits
    %     lookup  function: lookup(id) returns the dataset with that id (spot.worker.Worker)
    %     opts    optional width and height [in] (default 7 x 4, taller for several panels),
    %             fontSize [pt] (default 10), visible (default true)
    %
    %   Outputs
    %     fig     the figure; save it with spot.data.exportFigure
    %
    %   See also spot.data.exportFigure, spot.data.series, spot.plotting.platformStyle.

    %% Defaults
    if nargin < 3
        opts = struct();
    end

    %% Constants
    FONT = 'Times New Roman';

    %% Figure
    panels = asCell(panels);
    nPanels = numel(panels);

    % Size: 7 x 4 in, taller for several panels (within 4-9 in).
    width = field(opts, 'width', 7);
    height = field(opts, 'height', max(4, min(9, 2.6 * nPanels + 0.4)));
    fontSize = field(opts, 'fontSize', 10);
    visible = 'on';
    if ~field(opts, 'visible', true)
        visible = 'off';
    end

    fig = figure('Name', 'SPOT results', 'Color', 'w', 'Visible', visible, 'Units', 'inches');
    pos = get(fig, 'Position');
    set(fig, 'Position', [pos(1) pos(2) width height]);
    layout = tiledlayout(fig, nPanels, 1, 'TileSpacing', 'compact', 'Padding', 'compact');

    %% Panels
    for k = 1:nPanels
        ax = nexttile(layout);
        hold(ax, 'on');
        grid(ax, 'on');
        box(ax, 'on');

        % The table from above, or signals against time (or x).
        panel = panels{k};
        traces = asCell(field(panel, 'traces', {}));
        if isTable(panel, traces)
            [handles, labels] = tablePlot(ax, panel, traces, lookup);
        else
            [handles, labels] = signalPlot(ax, panel, traces, lookup);
        end

        % Font, and a legend when there are several lines.
        ax.FontName = FONT;
        ax.FontSize = fontSize;
        if numel(handles) > 1
            legend(ax, handles, labels, 'Location', 'best', 'Interpreter', 'none', ...
                'FontName', FONT, 'FontSize', fontSize);
        end
    end

    % Labels and legends too, where this computer has the font (else MATLAB's substitute).
    if any(strcmpi(listfonts, FONT))
        fontname(fig, FONT);
    end
end

%% Signals against time (or x)

function [handles, labels] = signalPlot(ax, panel, traces, lookup)
    % One line per trace; axis labels from the signals' quantities and units.
    MAX_POINTS = 20000;     % every sample, in practice

    xs = field(panel, 'x', []);
    [colors, styles, names] = styling(traces, lookup);
    handles = gobjects(0);
    labels = {};
    quantities = {};
    units = {};

    % Lines, experiments first.
    for j = order(traces, lookup)
        trace = traces{j};
        args = struct('signal', trace.signal, 'col', field(trace, 'col', 1), 'x', xs, ...
                      'smooth', field(panel, 'smooth', 0), 'points', MAX_POINTS);
        result = spot.data.series(lookup(trace.id), args);
        if isfield(result, 'x')
            xValues = result.x;
        else
            xValues = result.t;
        end
        handles(end + 1) = plot(ax, xValues, result.y, 'Color', colors{j}, ...
            'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
        labels{end + 1} = names{j}; %#ok<AGROW>
        info = spot.plotting.signalLabel(trace.signal);
        quantities{end + 1} = info.label; %#ok<AGROW>
        units{end + 1} = info.unit; %#ok<AGROW>
    end

    % X label: the x signal's, or time.
    if isstruct(xs) && ~isempty(field(xs, 'signal', ''))
        xlabel(ax, spot.plotting.signalLabel(xs.signal).label);
    else
        xlabel(ax, 'Time [s]');
    end

    % Y label: the quantity if all lines share it, else the shared unit's quantity.
    quantities = unique(quantities);
    units = unique(units);
    if isscalar(quantities)
        ylabel(ax, quantities{1});
    elseif isscalar(units) && ~isempty(units{1})
        ylabel(ax, sprintf('%s [%s]', unitQuantity(units{1}), units{1}));
    end

    % Limits: as given, else tight in x.
    if numel(field(panel, 'xlim', [])) == 2
        xlim(ax, panel.xlim);
    elseif ~isempty(handles)
        xlim(ax, 'tight');
    end
    if numel(field(panel, 'ylim', [])) == 2
        ylim(ax, panel.ylim);
    end
end

%% The table from above

function yes = isTable(panel, traces)
    % Every trace a <PLATFORM>_Py_m, against a <PLATFORM>_Px_m.
    xs = field(panel, 'x', []);
    yes = isstruct(xs) ...
          && ~isempty(regexp(field(xs, 'signal', ''), '^(RED|BLACK|BLUE)_Px_m$', 'once')) ...
          && ~isempty(traces) ...
          && all(cellfun(@(t) ~isempty(regexp(t.signal, '^(RED|BLACK|BLUE)_Py_m$', 'once')), ...
                         traces));
end

function [handles, labels] = tablePlot(ax, panel, traces, lookup)
    % Each platform's path, with its outline (and arm) at the start and the end.
    TABLE_X = [0 3.5];      % [m]
    TABLE_Y = [0 2.4];      % [m]
    START_ALPHA = 0.2;      % start outline faint, end solid
    END_ALPHA = 1;

    [colors, styles, names] = styling(traces, lookup);
    handles = gobjects(0);
    labels = {};
    tracks = {};
    for j = order(traces, lookup)
        trace = traces{j};
        ds = lookup(trace.id);
        platform = trace.signal(1:end - 5);   % RED_Py_m -> RED

        % Position and attitude at the Px samples.
        [t, x] = spot.data.samples(ds.signals.([platform '_Px_m']));
        [ty, y] = spot.data.samples(ds.signals.([platform '_Py_m']));
        [ta, th] = spot.data.samples(ds.signals.([platform '_Rz_rad']));
        x = x(:, 1);
        y = interp1(ty, y(:, 1), t, 'linear', NaN);
        th = interp1(ta, th(:, 1), t, 'linear', NaN);

        % Valid samples: finite, and in an experiment not (0, 0) (not seen by PhaseSpace yet).
        valid = isfinite(x) & isfinite(y);
        if ~strcmp(ds.kind, 'simulation')
            valid = valid & ~(x == 0 & y == 0);
        end
        idx = find(valid);
        if isempty(idx)
            continue
        end

        % Outline shape (spot.plotting.drawSpacecraft); the Chaser with its arm if shown.
        found = spot.data.bodies(ds);
        shapes = struct('RED', 1, 'BLACK', 4, 'BLUE', 3);
        shapeId = shapes.(platform);
        arm = strcmp(platform, 'RED') && found.arm;
        if arm
            shapeId = 5;
        end

        % Outlines first, so the paths are drawn over them.
        snaps = [idx(1), idx(end)];
        alphas = [START_ALPHA, END_ALPHA];
        for snap = 1:2
            sample = snaps(snap);
            pts = spot.plotting.drawSpacecraft(x(sample), y(sample), th(sample), shapeId);
            patch(ax, pts(:, 1), pts(:, 2), 'w', 'FaceAlpha', 0.5, 'EdgeColor', colors{j}, ...
                'EdgeAlpha', alphas(snap), 'LineWidth', 0.5);
            if arm

                % Joint angles at this sample.
                q = zeros(1, 3);
                joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
                for m = 1:3
                    [tj, v] = spot.data.samples(ds.signals.(joints{m}));
                    q(m) = interp1(tj, v(:, 1), t(sample), 'linear', 0);
                end

                % The arm's geometry: the dataset's settings, else the defaults.
                geometry = spot.params.defaults().mass.arm;
                if isstruct(ds.settings)
                    geometry = ds.settings.mass.arm;
                end

                % Its links.
                position = spot.plotting.armPosition([x(sample) y(sample) th(sample)], q, ...
                    spot.arm.mountGeometry(geometry));
                for link = spot.plotting.drawArm(position)
                    patch(ax, link{1}(:, 1), link{1}(:, 2), 'w', 'FaceAlpha', alphas(snap), ...
                        'EdgeColor', colors{j}, 'EdgeAlpha', alphas(snap), 'LineWidth', 0.5);
                end
            end
        end
        tracks{end + 1} = {x(valid), y(valid), j}; %#ok<AGROW>
    end

    % Paths.
    for k = 1:numel(tracks)
        j = tracks{k}{3};
        handles(end + 1) = plot(ax, tracks{k}{1}, tracks{k}{2}, 'Color', colors{j}, ...
            'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
        labels{end + 1} = names{j}; %#ok<AGROW>
    end

    % Axes: the table, equal scale.
    xlabel(ax, 'X-Position [m]');
    ylabel(ax, 'Y-Position [m]');
    xl = field(panel, 'xlim', TABLE_X);
    yl = field(panel, 'ylim', TABLE_Y);
    xlim(ax, xl);
    ylim(ax, yl);
    daspect(ax, [1 1 1]);
end

%% Colours, line styles, names

function [colors, styles, names] = styling(traces, lookup)
    % Colour by platform; within a colour, solid first, then dashed, dotted, dash-dot, with the
    % experiments (and files) before the simulations, so a simulation over a trial is dashed.
    n = numel(traces);
    colors = cell(1, n);
    styles = cell(1, n);
    names = cell(1, n);
    lineStyles = {'-', '--', ':', '-.'};
    used = containers.Map();   % platform -> lines so far

    % Labels, datasets, and whether all traces show one quantity.
    labels = cellfun(@(t) spot.plotting.signalLabel(t.signal), traces, 'UniformOutput', false);
    labels = [labels{:}];
    sets = unique(cellfun(@(t) t.id, traces, 'UniformOutput', false));
    sameQuantity = isscalar(unique({labels.quantity}));

    for j = order(traces, lookup)
        info = labels(j);

        % Colour by platform, the next line style within it.
        key = info.platform;
        if isempty(key)
            key = 'none';
        end
        [shown, colors{j}] = spot.plotting.platformStyle(key);
        count = 0;
        if isKey(used, key)
            count = used(key);
        end
        used(key) = count + 1;
        styles{j} = lineStyles{mod(count, numel(lineStyles)) + 1};

        % Name: platform, quantity (if they differ or no platform), column (if several).
        parts = {};
        if ~isempty(info.platform)
            parts{end + 1} = shown; %#ok<AGROW>
        end
        if ~sameQuantity || isempty(info.platform)
            parts{end + 1} = info.quantity; %#ok<AGROW>
        end
        col = field(traces{j}, 'col', 1);
        if col > 1 || size(lookup(traces{j}.id).signals.(traces{j}.signal).Data, 2) > 1
            parts{end + 1} = sprintf('%d', col); %#ok<AGROW>
        end
        name = strjoin(parts, ' ');

        % And the dataset, when there are several.
        if numel(sets) > 1
            name = sprintf('%s (%s)', name, lookup(traces{j}.id).name);
        end
        names{j} = name;
    end
end

function idx = order(traces, lookup)
    % Trace indices: experiments and files first, then simulations.
    isSimulation = cellfun(@(t) strcmp(lookup(t.id).kind, 'simulation'), traces);
    idx = [find(~isSimulation), find(isSimulation)];
    idx = idx(:).';
end

function q = unitQuantity(unit)
    % The quantity a unit measures, for an axis label.
    quantityOf = containers.Map( ...
        {'m', 'rad', 'm/s', 'rad/s', 'm/s^2', 'rad/s^2', 'N', 'N m', 's'}, ...
        {'Position', 'Angle', 'Velocity', 'Angular Rate', 'Acceleration', ...
         'Angular Acceleration', 'Force', 'Torque', 'Time'});
    q = 'Value';
    if isKey(quantityOf, unit)
        q = quantityOf(unit);
    end
end

%% Helpers

function v = field(s, name, default)
    % s.(name) if present and not empty, else default.
    if isstruct(s) && isfield(s, name) && ~isempty(s.(name))
        v = s.(name);
    else
        v = default;
    end
end

function c = asCell(x)
    % A struct array or cell array as a 1 x n cell array; anything else as {}.
    if iscell(x)
        c = x;
    elseif isstruct(x)
        c = num2cell(x);
    else
        c = {};
    end
    c = c(:).';
end
