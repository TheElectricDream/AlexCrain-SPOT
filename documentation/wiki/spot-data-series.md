# spot.data.series

**File:** `+spot/+data/series.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/series.m -->

## What it is for

One signal of a dataset, ready for the page to draw: a chosen column, optionally smoothed, cut to a time range, and **thinned** to at most a few thousand points in a way that keeps peaks (each slice of time contributes its minimum and maximum). With `x`, the signal is returned against another signal instead of time (an x–y plot); a platform's `Py` against any `Px` is paired with its own `Px`, which draws the table from above.

## How to call it

```matlab
r = spot.data.series(ds, struct('signal', 'RED_Px_m'));                       % r.t, r.y
r = spot.data.series(ds, struct('signal', 'RED_Vx_mpers', 'col', 1, 'smooth', 0.5, 'range', [10 60], 'points', 2000));
r = spot.data.series(ds, struct('signal', 'RED_Py_m', 'x', struct('signal', 'RED_Px_m')));   % r.x, r.y
```

## Line by line

**Lines 16–22.** Fill defaults (`defaults`, 78–88: column 1, no x, no smoothing, no range, 4000 points, clamped to 10–20000); get the timeseries (`signal`, 90–95, with a clear error for a wrong name); its samples; check the column; take it.

**Lines 23–31.** With an `x` signal: if this is `<P>_Py_m` and `x` is any `<Q>_Px_m`, use `<P>_Px_m` instead, so each platform's path is against its own x.

**Lines 32–44.** Interpolate the x signal at this signal's times. For measured data, drop samples where both are exactly 0 (PhaseSpace hadn't seen the platform yet).

**Lines 45–53.** Smoothing: the window in seconds becomes a number of samples from the median sample spacing; Gaussian `smoothdata` on y (and x).

**Lines 54–64.** The range: the indices from one sample before the first time ≥ t0 to one after the last ≤ t1, so lines reach the edges of the plot; an empty range gives empty output.

**Lines 65–75.** The result with the sample count before thinning and the units. With x: evenly spaced points. Without: `thin` (97–115) keeps, when there are more samples than wanted, the first, the last, and the minimum and maximum of each of `points/2` equal slices, sorted and unique.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/results.js`, `+spot/+data/toFigure.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.data.samples|spot-data-samples]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/series.m</b> (115 lines; click to show)</summary>

```matlab
   1  function r = series(ds, a)
   2  %SERIES One signal of a dataset, ready to plot in the console.
   3  %   r = spot.data.series(dataset, args) with args fields
   4  %     signal    signal name (field of dataset.signals)
   5  %     col       column, 1-based (default 1)
   6  %     x         (optional) struct with signal and col: plot against that signal instead of time
   7  %               (it is interpolated at this signal's time stamps); for an experiment, samples
   8  %               where both are 0 are left out (the platform wasn't seen yet). A platform's Py
   9  %               against any platform's Px uses its own Px (the table seen from above).
  10  %     smooth    (optional) smoothing window [s], Gaussian (0: off)
  11  %     range     (optional) [t0 t1]: only that time span
  12  %     points    (optional) at most this many points (default 4000)
  13  %   returns t (time [s]) and y, and x when asked for; n is the number of samples before
  14  %   thinning. Long signals are thinned to the minimum and maximum of each slice of time, so
  15  %   peaks stay visible.
  16  a = defaults(a);
  17  ts = signal(ds, a.signal);
  18  [t, y] = spot.data.samples(ts);
  19  if a.col > size(y, 2)
  20      error('spot:data:series', '%s has %d column(s).', a.signal, size(y, 2));
  21  end
  22  y = y(:, a.col);
  23  hasX = isstruct(a.x) && isfield(a.x, 'signal') && ~isempty(a.x.signal);
  24  if hasX
  25      % The table seen from above: <P>_Py_m against any <Q>_Px_m uses P's own Px
  26      own = regexp(a.signal, '^(RED|BLACK|BLUE)_Py_m$', 'tokens', 'once');
  27      if ~isempty(own) && ~isempty(regexp(a.x.signal, '^(RED|BLACK|BLUE)_Px_m$', 'once'))
  28          a.x.signal = [own{1} '_Px_m'];
  29          a.x.col = 1;
  30      end
  31  end
  32  if hasX
  33      xcol = 1;
  34      if isfield(a.x, 'col') && ~isempty(a.x.col)
  35          xcol = a.x.col;
  36      end
  37      [tx, x] = spot.data.samples(signal(ds, a.x.signal));
  38      x = interp1(tx, x(:, min(xcol, end)), t, 'linear', NaN);
  39      if ~strcmp(ds.kind, 'simulation')
  40          % (0, 0): a platform PhaseSpace hadn't seen yet (experiment logs start with these)
  41          keep = ~(x == 0 & y == 0);
  42          t = t(keep); x = x(keep); y = y(keep);
  43      end
  44  end
  45  if a.smooth > 0 && numel(t) > 2
  46      w = max(1, round(a.smooth / median(diff(t))));
  47      if w > 1
  48          y = smoothdata(y, 'gaussian', w);
  49          if hasX
  50              x = smoothdata(x, 'gaussian', w);
  51          end
  52      end
  53  end
  54  if numel(a.range) == 2 && all(isfinite(a.range))
  55      i0 = max(1, find(t >= a.range(1), 1) - 1);
  56      i1 = min(numel(t), find(t <= a.range(2), 1, 'last') + 1);
  57      if isempty(i0) || isempty(i1) || i1 < i0
  58          i0 = 1; i1 = 0;
  59      end
  60      t = t(i0:i1); y = y(i0:i1);
  61      if hasX
  62          x = x(i0:i1);
  63      end
  64  end
  65  r = struct('signal', a.signal, 'col', a.col, 'n', numel(t), 'units', char(ts.DataInfo.Units));
  66  if hasX
  67      pick = unique(round(linspace(1, numel(t), min(numel(t), a.points))));
  68      r.t = t(pick).';
  69      r.x = x(pick).';
  70      r.y = y(pick).';
  71      return
  72  end
  73  pick = thin(y, a.points);
  74  r.t = t(pick).';
  75  r.y = y(pick).';
  76  end
  77  
  78  function a = defaults(a)
  79  d = struct('signal', '', 'col', 1, 'x', [], 'smooth', 0, 'range', [], 'points', 4000);
  80  for f = fieldnames(d).'
  81      if ~isfield(a, f{1}) || isempty(a.(f{1}))
  82          a.(f{1}) = d.(f{1});
  83      end
  84  end
  85  a.col = max(1, round(double(a.col)));
  86  a.points = max(10, min(20000, round(double(a.points))));
  87  a.range = double(a.range(:)).';
  88  end
  89  
  90  function ts = signal(ds, name)
  91  if ~isfield(ds.signals, name)
  92      error('spot:data:series', '%s has no signal %s.', ds.name, name);
  93  end
  94  ts = ds.signals.(name);
  95  end
  96  
  97  function pick = thin(y, points)
  98  % Indices to keep: all of them when there are few; otherwise the first, the last, and the
  99  % minimum and maximum of each of points/2 slices
 100  n = numel(y);
 101  if n <= points
 102      pick = 1:n;
 103      return
 104  end
 105  slices = floor(points / 2);
 106  edges = round(linspace(1, n + 1, slices + 1));
 107  pick = zeros(1, 2 * slices);
 108  for k = 1:slices
 109      i = edges(k):edges(k + 1) - 1;
 110      [~, lo] = min(y(i));
 111      [~, hi] = max(y(i));
 112      pick(2 * k - 1:2 * k) = sort(i([lo hi]));
 113  end
 114  pick = unique([1 pick n]);
 115  end
```

</details>
<!-- /listing -->
