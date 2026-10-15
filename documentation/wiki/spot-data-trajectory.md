# spot.data.trajectory

**File:** `+spot/+data/trajectory.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/trajectory.m -->

## What it is for

A loaded dataset's motion for the 3D view, the counterpart of [[spot.sim.results|spot-sim-results]] for saved runs: at most 30 samples per second of each platform's x, y and unwrapped attitude (NaN where the platform has no position yet), the arm's joint points when the arm is in it, and the phase boundaries when the settings are known.

## How to call it

```matlab
r = spot.data.trajectory(ds);        % r.t, r.crafts.RED.x/y/att, r.arm, r.phases
r = spot.data.trajectory(ds, 60);
```

## Line by line

**Lines 11–20.** Default 30 Hz; the signals; the bodies ([[bodies|spot-data-bodies]]); nothing to show is an error; `measured` is true for anything but a simulation.

**Lines 22–44.** One time grid for all platforms. For each: its x, y (first column) and attitude samples; in measured data, samples at exactly (0, 0) are invalid (not seen yet); keep the valid times, x, y, and the unwrapped attitude on its own time base; track the earliest start, latest end and most samples. No valid position anywhere is an error.

**Lines 45–47.** For an experiment the grid starts at 0 (the program's start), so the playback clock matches the experiment's.

**Lines 48–61.** `m` grid points (enough for the rate, at least 2, no more than samples); interpolate x, y and attitude onto it, NaN outside each platform's own span; where x is NaN the attitude is too.

**Lines 63–76.** The arm: the three joint angles on the grid, the geometry from the dataset's settings (or the defaults), and the joint points from [[spot.plotting.armPosition|spot-plotting-armPosition]] with RED's poses as the base, at the mount from [[spot.arm.mountGeometry|spot-arm-mountGeometry]].

**Lines 78–83.** The phases that ran, from the settings' timing, if it computes.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.data.bodies|spot-data-bodies]], [[spot.data.samples|spot-data-samples]], [[spot.params.defaults|spot-params-defaults]], [[spot.params.timing|spot-params-timing]], [[spot.plotting.armPosition|spot-plotting-armPosition]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/trajectory.m</b> (84 lines; click to show)</summary>

```matlab
   1  function r = trajectory(ds, maxRate)
   2  %TRAJECTORY A dataset's motion for the console's 3D view.
   3  %   r = spot.data.trajectory(dataset) returns, resampled to at most 30 Hz:
   4  %     t               time [s]
   5  %     crafts.<NAME>   x, y [m] and att [rad, unwrapped] of each platform (see spot.data.bodies);
   6  %                     NaN where that platform has no position (before its log starts, or a
   7  %                     sample of all zeros in an experiment: PhaseSpace didn't see it yet)
   8  %     arm             4 x 2 x N joint positions [m], when the arm is in it
   9  %     phases          names and end times of the phases, when the dataset's settings are known
  10  %   r = spot.data.trajectory(dataset, maxRate) uses another maximum rate [Hz].
  11  if nargin < 2
  12      maxRate = 30;
  13  end
  14  sig = ds.signals;
  15  b = spot.data.bodies(ds);
  16  r = struct('t', [], 'crafts', struct());
  17  if isempty(b.crafts)
  18      error('spot:data:trajectory', '%s has no platform positions (<NAME>_Px_m, _Py_m, _Rz_rad) to show.', ds.name);
  19  end
  20  measured = ~strcmp(ds.kind, 'simulation');
  21  
  22  % One time grid for all: from the first to the last position of any platform
  23  raw = struct();
  24  t0 = inf; t1 = -inf; n = 0;
  25  for k = 1:numel(b.crafts)
  26      c = b.crafts{k};
  27      [t, x] = spot.data.samples(sig.([c '_Px_m']));
  28      [~, y] = spot.data.samples(sig.([c '_Py_m']));
  29      [ta, a] = spot.data.samples(sig.([c '_Rz_rad']));
  30      x = x(:, 1); y = y(:, 1);
  31      valid = true(size(t));
  32      if measured
  33          valid = ~(x == 0 & y == 0);
  34      end
  35      raw.(c) = struct('t', t(valid), 'x', x(valid), 'y', y(valid), 'ta', ta, 'a', unwrap(a(:, 1)));
  36      if any(valid)
  37          t0 = min(t0, raw.(c).t(1));
  38          t1 = max(t1, raw.(c).t(end));
  39          n = max(n, nnz(valid));
  40      end
  41  end
  42  if ~isfinite(t0)
  43      error('spot:data:trajectory', '%s has no position other than zero.', ds.name);
  44  end
  45  if measured
  46      t0 = min(t0, 0);   % keep the experiment's own clock: 0 is when the program started
  47  end
  48  m = max(2, min(n, ceil((t1 - t0) * maxRate) + 1));
  49  grid = linspace(t0, t1, m).';
  50  r.t = grid.';
  51  for k = 1:numel(b.crafts)
  52      c = b.crafts{k};
  53      q = raw.(c);
  54      if numel(q.t) < 2
  55          continue
  56      end
  57      r.crafts.(c) = struct('x', interp1(q.t, q.x, grid, 'linear', NaN).', ...
  58                            'y', interp1(q.t, q.y, grid, 'linear', NaN).', ...
  59                            'att', interp1(q.ta, q.a, grid, 'linear', NaN).');
  60      r.crafts.(c).att(isnan(r.crafts.(c).x)) = NaN;
  61  end
  62  
  63  if b.arm && isfield(r.crafts, 'RED')
  64      joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
  65      q = zeros(m, 3);
  66      for j = 1:3
  67          [tj, v] = spot.data.samples(sig.(joints{j}));
  68          q(:, j) = interp1(tj, v(:, 1), grid, 'linear', NaN);
  69      end
  70      arm = spot.params.defaults().mass.arm;
  71      if isstruct(ds.settings)
  72          arm = ds.settings.mass.arm;
  73      end
  74      base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
  75      r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(arm));
  76  end
  77  
  78  if isstruct(ds.settings)
  79      try
  80          r.phases = spot.params.timing(ds.settings).phases;
  81      catch
  82      end
  83  end
  84  end
```

</details>
<!-- /listing -->
