# spot.sim.results

**File:** `+spot/+sim/results.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/results.m -->

## What it is for

The 3D view doesn't need every sample of every signal; it needs where each platform was, at a rate the eye can follow. This takes the logged data and returns, resampled to at most 30 Hz: time, each selected platform's `x`, `y` and unwrapped attitude, the arm's four joint points, the phase boundaries, and a list of every logged signal with its size (for the Results tab's signal list).

## How to call it

```matlab
r = spot.sim.results(dataClass, s, d)        % d from spot.params.apply
r = spot.sim.results(dataClass, s, d, 60)    % another maximum rate [Hz]
```

## Line by line

**Lines 11–13.** Default maximum rate 30 Hz.

**Lines 14–16.** The logged time stamps (`timeOf`, lines 45–58: `Time_s` if the diagram logs it, else the first timeseries found), made unique. The output has `n` evenly spaced times from first to last: enough for 30 per second, at least 2, and never more than the samples logged.

**Lines 18–21.** `t` as a row, and how many samples were logged.

**Lines 22–32.** For each of RED, BLACK, BLUE that is on *and* was logged (`<NAME>_Px_m` exists): x and y interpolated to `t`, and the attitude `Rz` interpolated after `unwrap` so a turn through ±π doesn't produce a spin in the replay. `resample1` (lines 60–69) drops repeated time stamps, takes the first column, optionally unwraps, and uses linear interpolation (extrapolating at the ends).

**Lines 33–39.** If the arm is on and RED is there and the three joint angles were logged: resample them, build the base poses from RED's x, y, θ, and get the four joint points per sample from [[spot.plotting.armPosition|spot-plotting-armPosition]] with the arm geometry for its mount.

**Lines 41–42.** The phases that ran (names and end times), and the signal list (`signalList`, lines 71–85: every timeseries at the top level or one level down, with the size of its data).

**Lines 87–94.** `firstColumn`: logged data can be N×m or, for some blocks, 1×1×N; both are reduced to one column of doubles.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/messages.js`, `+spot/+console/webapp/static/viewer.js`, `+spot/+sim/run.m`
- **Uses:** [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.params.apply|spot-params-apply]], [[spot.plotting.armPosition|spot-plotting-armPosition]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/results.m</b> (94 lines; click to show)</summary>

```matlab
   1  function r = results(dataClass, s, d, maxRate)
   2  %RESULTS Trajectory of a simulation for the SPOT Console's 3D view.
   3  %   r = spot.sim.results(dataClass, s, d) takes the logged data (simOut.dataPacket), the settings
   4  %   and the values from spot.params.apply, and returns, resampled to at most 30 Hz:
   5  %     t                 time [s]
   6  %     crafts.<NAME>     x, y [m] and att [rad, unwrapped] of each selected platform
   7  %     arm               4 x 2 x N joint positions [m] (shoulder, elbow, wrist, end effector)
   8  %     phases            end time of each phase and sub-phase [s]
   9  %     signals           name and size of every logged signal
  10  %   r = spot.sim.results(..., maxRate) uses another maximum rate [Hz].
  11  if nargin < 4
  12      maxRate = 30;
  13  end
  14  time = unique(timeOf(dataClass));
  15  n = max(2, min(numel(time), ceil((time(end) - time(1)) * maxRate) + 1));
  16  t = linspace(time(1), time(end), n).';
  17  
  18  r = struct();
  19  r.t = t.';
  20  r.samples = numel(time);
  21  r.crafts = struct();
  22  names = {'RED', 'BLACK', 'BLUE'};
  23  for k = 1:3
  24      n = names{k};
  25      if ~s.platforms.(n) || ~isfield(dataClass, [n '_Px_m'])
  26          continue
  27      end
  28      x = resample1(dataClass.([n '_Px_m']), t);
  29      y = resample1(dataClass.([n '_Py_m']), t);
  30      a = resample1(dataClass.([n '_Rz_rad']), t, true);
  31      r.crafts.(n) = struct('x', x.', 'y', y.', 'att', a.');
  32  end
  33  if s.platforms.ARM && isfield(r.crafts, 'RED') && isfield(dataClass, 'ARM_Rz_Shoulder_rad')
  34      q = [resample1(dataClass.ARM_Rz_Shoulder_rad, t), ...
  35           resample1(dataClass.ARM_Rz_Elbow_rad, t), ...
  36           resample1(dataClass.ARM_Rz_Wrist_rad, t)];
  37      base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
  38      r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(s.mass.arm));
  39  end
  40  
  41  r.phases = d.timing.phases;   % the segments that ran (skipped ones left out)
  42  r.signals = signalList(dataClass);
  43  end
  44  
  45  function time = timeOf(dataClass)
  46  if isfield(dataClass, 'Time_s') && isa(dataClass.Time_s, 'timeseries')
  47      time = dataClass.Time_s.Time;
  48  else
  49      f = fieldnames(dataClass);
  50      for k = 1:numel(f)
  51          if isa(dataClass.(f{k}), 'timeseries')
  52              time = dataClass.(f{k}).Time;
  53              return
  54          end
  55      end
  56      error('spot:sim:results', 'The logged data has no timeseries.');
  57  end
  58  end
  59  
  60  function y = resample1(ts, t, isAngle)
  61  % A signal (first column) at times t, on its own time stamps (repeated ones dropped)
  62  [time, keep] = unique(ts.Time(:));
  63  data = firstColumn(ts.Data);
  64  data = data(keep);
  65  if nargin > 2 && isAngle
  66      data = unwrap(data);
  67  end
  68  y = interp1(time, data, t, 'linear', 'extrap');
  69  end
  70  
  71  function list = signalList(dataClass)
  72  list = struct('name', {}, 'size', {});
  73  for f = fieldnames(dataClass).'
  74      x = dataClass.(f{1});
  75      if isa(x, 'timeseries')
  76          list(end + 1) = struct('name', f{1}, 'size', size(x.Data)); %#ok<AGROW>
  77      elseif isstruct(x)
  78          for g = fieldnames(x).'
  79              if isa(x.(g{1}), 'timeseries')
  80                  list(end + 1) = struct('name', [f{1} '.' g{1}], 'size', size(x.(g{1}).Data)); %#ok<AGROW>
  81              end
  82          end
  83      end
  84  end
  85  end
  86  
  87  function c = firstColumn(data)
  88  % First signal of logged data stored as N x m, or as 1 x 1 x N (Simulink's 3-D format)
  89  data = double(data);
  90  if ndims(data) == 3
  91      data = reshape(data, [], size(data, 3)).';
  92  end
  93  c = data(:, 1);
  94  end
```

</details>
<!-- /listing -->
