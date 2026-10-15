# spot.data.toFigure

**File:** `+spot/+data/toFigure.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/toFigure.m -->

## What it is for

The Results tab's *Export › MATLAB Figure / Save Figure*: redraw the page's plots as a MATLAB figure in the lab's publication style (the GetStarted templates: Times New Roman, the platforms' colours, 1 pt lines, grid and box, simulations dashed when shown with experiments), with every sample drawn. One tile per panel. A panel of `<P>_Py_m` against `<P>_Px_m` is drawn as **the table from above**: each platform's path, its outline at the start (faint) and the end, the arm when there is one, 0–3.5 × 0–2.4 m with equal axes.

## How to call it

```matlab
panels = {struct('traces', {{struct('id', 1, 'signal', 'RED_Px_m', 'col', 1)}}, 'smooth', 0, 'xlim', [0 100])};
fig = spot.data.toFigure(panels, @(id) ds{1}, struct('width', 7, 'height', 4, 'fontSize', 10, 'visible', true));
spot.data.exportFigure(fig, 'figure.pdf');
```

`lookup(id)` must return the dataset with that id (the worker keeps them by id).

## Line by line

**Lines 20–31.** Options with defaults: 7 in wide, 4 in tall (taller for several panels, up to 9), 10 pt, visible.

**Lines 32–35.** A white figure sized in inches, with a one-column tiled layout.

**Lines 36–52.** For each panel: an axes with hold, grid and box; its traces; `isTable` (107–111) decides between the table view (`tablePlot`) and signals against time or x (`signalPlot`); set the font; a legend when there is more than one line.

**Lines 54–56.** Apply Times New Roman to everything if the computer has it.

**Lines 61–103.** `signalPlot`: colours, line styles and legend names from `styling`; for each trace (experiments first, then simulations: `order`, 228–232) fetch the series with up to 20 000 points through [[series|spot-data-series]] and plot it against its x or its time; collect each signal's quantity and unit for the axis labels. The x label is the x signal's label or "Time [s]"; the y label is the common quantity, or "Quantity [unit]" when the quantities differ but the unit is shared (`unitQuantity`, 234–241). Axis limits from the panel, else tight in x.

**Lines 113–181.** `tablePlot`: for each trace (a platform's `Py`), take that platform's x, y and θ samples, interpolate y and θ onto x's times, drop non-finite and (for measured data) (0, 0) samples. Draw the outline ([[spot.plotting.drawSpacecraft|spot-plotting-drawSpacecraft]], shape 1 for the Chaser, 4 for the Target with cone and panels, 3 for the Obstacle; 5 for the Chaser with the arm joint) at the first and last valid sample, faint then solid, and the arm's links ([[spot.plotting.drawArm|spot-plotting-drawArm]]) at those instants with the joint angles interpolated and the geometry from the dataset's settings. Then the paths over the outlines, axis labels, the table's extent and equal aspect.

**Lines 185–226.** `styling`: colour by platform ([[spot.plotting.platformStyle|spot-plotting-platformStyle]]); within one colour the styles cycle solid, dashed, dotted, dash-dot, with experiments before simulations so a simulation over a trial comes out dashed. Legend names: the platform's name, the quantity when the traces differ in quantity, the column when the signal has several, and the dataset's name when several datasets are shown.

**Lines 245–262.** Helpers: a field with a default, and anything (cell, struct array) as a row cell.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/results.js`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.data.bodies|spot-data-bodies]], [[spot.data.exportFigure|spot-data-exportFigure]], [[spot.data.samples|spot-data-samples]], [[spot.data.series|spot-data-series]], [[spot.params.defaults|spot-params-defaults]], [[spot.plotting.armPosition|spot-plotting-armPosition]], [[spot.plotting.drawArm|spot-plotting-drawArm]], [[spot.plotting.drawSpacecraft|spot-plotting-drawSpacecraft]], [[spot.plotting.platformStyle|spot-plotting-platformStyle]], [[spot.plotting.signalLabel|spot-plotting-signalLabel]], [[spot.worker.Worker|spot-worker-Worker]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/toFigure.m</b> (262 lines; click to show)</summary>

```matlab
   1  function fig = toFigure(panels, lookup, opts)
   2  %TOFIGURE The Results page's plots as a MATLAB figure in the lab's publication style.
   3  %   fig = spot.data.toFigure(panels, lookup) draws one tile per panel, following the lab's
   4  %   GetStarted templates (github.com/Carleton-SRL/GetStarted): Times New Roman, the platforms'
   5  %   colours (spot.plotting.platformStyle: the Chaser and the arm grey-silver, the Target gold, the
   6  %   Obstacle blue, other signals black), 1 pt lines, grid and box, simulations dashed when shown
   7  %   with experiments. Every sample is drawn.
   8  %   Each panel has
   9  %     traces    list of id, signal, col: a column of a signal of a loaded dataset
  10  %     x         (optional) signal and col to plot against instead of time
  11  %     smooth    (optional) smoothing window [s]
  12  %     xlim, ylim (optional) axis limits
  13  %   A panel of <PLATFORM>_Py_m against a <PLATFORM>_Px_m is drawn as the table seen from above:
  14  %   each platform's path (against its own Px), its outline at the start (faint) and the end
  15  %   (spot.plotting.drawSpacecraft, and the arm), 0-3.5 x 0-2.4 m, equal axes.
  16  %   lookup(id) returns the dataset with that id (see spot.worker.Worker).
  17  %   fig = spot.data.toFigure(..., opts) with opts fields width and height [in] (default 7 x 4,
  18  %   taller for several panels), fontSize [pt] (default 10) and visible (default true).
  19  %   Save it with spot.data.exportFigure.
  20  if nargin < 3
  21      opts = struct();
  22  end
  23  panels = asCell(panels);
  24  n = numel(panels);
  25  w = field(opts, 'width', 7);
  26  h = field(opts, 'height', max(4, min(9, 2.6 * n + 0.4)));
  27  fs = field(opts, 'fontSize', 10);
  28  visible = 'on';
  29  if ~field(opts, 'visible', true)
  30      visible = 'off';
  31  end
  32  fig = figure('Name', 'SPOT results', 'Color', 'w', 'Visible', visible, 'Units', 'inches');
  33  pos = get(fig, 'Position');
  34  set(fig, 'Position', [pos(1) pos(2) w h]);
  35  layout = tiledlayout(fig, n, 1, 'TileSpacing', 'compact', 'Padding', 'compact');
  36  for k = 1:n
  37      ax = nexttile(layout);
  38      hold(ax, 'on'); grid(ax, 'on'); box(ax, 'on');
  39      p = panels{k};
  40      traces = asCell(field(p, 'traces', {}));
  41      if isTable(p, traces)
  42          [lines, labels] = tablePlot(ax, p, traces, lookup);
  43      else
  44          [lines, labels] = signalPlot(ax, p, traces, lookup);
  45      end
  46      ax.FontName = 'Times New Roman';
  47      ax.FontSize = fs;
  48      if numel(lines) > 1
  49          legend(ax, lines, labels, 'Location', 'best', 'Interpreter', 'none', ...
  50              'FontName', 'Times New Roman', 'FontSize', fs);
  51      end
  52  end
  53  % Labels and legends too (where this computer has the font; otherwise MATLAB's substitute)
  54  if any(strcmpi(listfonts, 'Times New Roman'))
  55      fontname(fig, 'Times New Roman');
  56  end
  57  end
  58  
  59  % ---------------------------------------------------------------- signals against time (or x)
  60  
  61  function [lines, labels] = signalPlot(ax, p, traces, lookup)
  62  xs = field(p, 'x', []);
  63  [colors, styles, names] = styling(traces, lookup);
  64  lines = gobjects(0);
  65  labels = {};
  66  quantities = {}; units = {};
  67  for j = order(traces, lookup)
  68      tr = traces{j};
  69      a = struct('signal', tr.signal, 'col', field(tr, 'col', 1), 'x', xs, ...
  70                 'smooth', field(p, 'smooth', 0), 'points', 20000);
  71      r = spot.data.series(lookup(tr.id), a);
  72      if isfield(r, 'x')
  73          xv = r.x;
  74      else
  75          xv = r.t;
  76      end
  77      lines(end + 1) = plot(ax, xv, r.y, 'Color', colors{j}, 'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
  78      labels{end + 1} = names{j}; %#ok<AGROW>
  79      L = spot.plotting.signalLabel(tr.signal);
  80      quantities{end + 1} = L.label; %#ok<AGROW>
  81      units{end + 1} = L.unit; %#ok<AGROW>
  82  end
  83  if isstruct(xs) && ~isempty(field(xs, 'signal', ''))
  84      xlabel(ax, spot.plotting.signalLabel(xs.signal).label);
  85  else
  86      xlabel(ax, 'Time [s]');
  87  end
  88  quantities = unique(quantities);
  89  units = unique(units);
  90  if isscalar(quantities)
  91      ylabel(ax, quantities{1});
  92  elseif isscalar(units) && ~isempty(units{1})
  93      ylabel(ax, sprintf('%s [%s]', unitQuantity(units{1}), units{1}));
  94  end
  95  if numel(field(p, 'xlim', [])) == 2
  96      xlim(ax, p.xlim);
  97  elseif ~isempty(lines)
  98      xlim(ax, 'tight');
  99  end
 100  if numel(field(p, 'ylim', [])) == 2
 101      ylim(ax, p.ylim);
 102  end
 103  end
 104  
 105  % ---------------------------------------------------------------- the table from above
 106  
 107  function yes = isTable(p, traces)
 108  xs = field(p, 'x', []);
 109  yes = isstruct(xs) && ~isempty(regexp(field(xs, 'signal', ''), '^(RED|BLACK|BLUE)_Px_m$', 'once')) && ...
 110        ~isempty(traces) && all(cellfun(@(t) ~isempty(regexp(t.signal, '^(RED|BLACK|BLUE)_Py_m$', 'once')), traces));
 111  end
 112  
 113  function [lines, labels] = tablePlot(ax, p, traces, lookup)
 114  [colors, styles, names] = styling(traces, lookup);
 115  lines = gobjects(0);
 116  labels = {};
 117  paths = {};
 118  for j = order(traces, lookup)
 119      tr = traces{j};
 120      ds = lookup(tr.id);
 121      P = tr.signal(1:end - 5);   % RED_Py_m -> RED
 122      [t, x] = spot.data.samples(ds.signals.([P '_Px_m']));
 123      [ty, y] = spot.data.samples(ds.signals.([P '_Py_m']));
 124      [ta, th] = spot.data.samples(ds.signals.([P '_Rz_rad']));
 125      x = x(:, 1);
 126      y = interp1(ty, y(:, 1), t, 'linear', NaN);
 127      th = interp1(ta, th(:, 1), t, 'linear', NaN);
 128      valid = isfinite(x) & isfinite(y);
 129      if ~strcmp(ds.kind, 'simulation')
 130          valid = valid & ~(x == 0 & y == 0);   % not seen by PhaseSpace yet
 131      end
 132      idx = find(valid);
 133      if isempty(idx)
 134          continue
 135      end
 136      % Outlines first, so the paths are drawn over them: faint at the start, solid at the end
 137      b = spot.data.bodies(ds);
 138      shapes = struct('RED', 1, 'BLACK', 4, 'BLUE', 3);
 139      id = shapes.(P);
 140      arm = strcmp(P, 'RED') && b.arm;
 141      if arm
 142          id = 5;
 143      end
 144      snaps = [idx(1), idx(end)];
 145      alphas = [0.2, 1];
 146      for s = 1:2
 147          i = snaps(s);
 148          pts = spot.plotting.drawSpacecraft(x(i), y(i), th(i), id);
 149          patch(ax, pts(:, 1), pts(:, 2), 'w', 'FaceAlpha', 0.5, 'EdgeColor', colors{j}, ...
 150              'EdgeAlpha', alphas(s), 'LineWidth', 0.5);
 151          if arm
 152              q = zeros(1, 3);
 153              jn = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
 154              for m = 1:3
 155                  [tj, v] = spot.data.samples(ds.signals.(jn{m}));
 156                  q(m) = interp1(tj, v(:, 1), t(i), 'linear', 0);
 157              end
 158              geometry = spot.params.defaults().mass.arm;
 159              if isstruct(ds.settings)
 160                  geometry = ds.settings.mass.arm;
 161              end
 162              for link = spot.plotting.drawArm(spot.plotting.armPosition([x(i) y(i) th(i)], q, spot.arm.mountGeometry(geometry)))
 163                  patch(ax, link{1}(:, 1), link{1}(:, 2), 'w', 'FaceAlpha', alphas(s), 'EdgeColor', colors{j}, ...
 164                      'EdgeAlpha', alphas(s), 'LineWidth', 0.5);
 165              end
 166          end
 167      end
 168      paths{end + 1} = {x(valid), y(valid), j}; %#ok<AGROW>
 169  end
 170  for k = 1:numel(paths)
 171      j = paths{k}{3};
 172      lines(end + 1) = plot(ax, paths{k}{1}, paths{k}{2}, 'Color', colors{j}, 'LineStyle', styles{j}, 'LineWidth', 1); %#ok<AGROW>
 173      labels{end + 1} = names{j}; %#ok<AGROW>
 174  end
 175  xlabel(ax, 'X-Position [m]');
 176  ylabel(ax, 'Y-Position [m]');
 177  xl = field(p, 'xlim', [0 3.5]);
 178  yl = field(p, 'ylim', [0 2.4]);
 179  xlim(ax, xl); ylim(ax, yl);
 180  daspect(ax, [1 1 1]);
 181  end
 182  
 183  % ---------------------------------------------------------------- colours, line styles, names
 184  
 185  function [colors, styles, names] = styling(traces, lookup)
 186  % Colour by platform; within a colour, solid first, then dashed, dotted, dash-dot, with the
 187  % experiments (and files) before the simulations, so a simulation over a trial is dashed
 188  n = numel(traces);
 189  colors = cell(1, n); styles = cell(1, n); names = cell(1, n);
 190  lineStyles = {'-', '--', ':', '-.'};
 191  used = containers.Map();
 192  labels = cellfun(@(t) spot.plotting.signalLabel(t.signal), traces, 'UniformOutput', false);
 193  labels = [labels{:}];
 194  sets = unique(cellfun(@(t) t.id, traces, 'UniformOutput', false));
 195  sameQuantity = isscalar(unique({labels.quantity}));
 196  for j = order(traces, lookup)
 197      L = labels(j);
 198      key = L.platform;
 199      if isempty(key)
 200          key = 'none';
 201      end
 202      [shown, colors{j}] = spot.plotting.platformStyle(key);
 203      c = 0;
 204      if isKey(used, key)
 205          c = used(key);
 206      end
 207      used(key) = c + 1;
 208      styles{j} = lineStyles{mod(c, numel(lineStyles)) + 1};
 209      parts = {};
 210      if ~isempty(L.platform)
 211          parts{end + 1} = shown; %#ok<AGROW>
 212      end
 213      if ~sameQuantity || isempty(L.platform)
 214          parts{end + 1} = L.quantity; %#ok<AGROW>
 215      end
 216      col = field(traces{j}, 'col', 1);
 217      if col > 1 || size(lookup(traces{j}.id).signals.(traces{j}.signal).Data, 2) > 1
 218          parts{end + 1} = sprintf('%d', col); %#ok<AGROW>
 219      end
 220      name = strjoin(parts, ' ');
 221      if numel(sets) > 1
 222          name = sprintf('%s (%s)', name, lookup(traces{j}.id).name);
 223      end
 224      names{j} = name;
 225  end
 226  end
 227  
 228  function idx = order(traces, lookup)
 229  sim = cellfun(@(t) strcmp(lookup(t.id).kind, 'simulation'), traces);
 230  idx = [find(~sim), find(sim)];
 231  idx = idx(:).';
 232  end
 233  
 234  function q = unitQuantity(unit)
 235  m = containers.Map({'m', 'rad', 'm/s', 'rad/s', 'm/s^2', 'rad/s^2', 'N', 'N m', 's'}, ...
 236      {'Position', 'Angle', 'Velocity', 'Angular Rate', 'Acceleration', 'Angular Acceleration', 'Force', 'Torque', 'Time'});
 237  q = 'Value';
 238  if isKey(m, unit)
 239      q = m(unit);
 240  end
 241  end
 242  
 243  % ---------------------------------------------------------------- helpers
 244  
 245  function v = field(s, name, default)
 246  if isstruct(s) && isfield(s, name) && ~isempty(s.(name))
 247      v = s.(name);
 248  else
 249      v = default;
 250  end
 251  end
 252  
 253  function c = asCell(x)
 254  if iscell(x)
 255      c = x;
 256  elseif isstruct(x)
 257      c = num2cell(x);
 258  else
 259      c = {};
 260  end
 261  c = c(:).';
 262  end
```

</details>
<!-- /listing -->
