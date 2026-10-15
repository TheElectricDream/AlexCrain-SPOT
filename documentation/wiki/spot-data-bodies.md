# spot.data.bodies

**File:** `+spot/+data/bodies.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/bodies.m -->

## What it is for

Which platforms, and whether the arm, a dataset can draw on the table. A platform needs `<NAME>_Px_m`, `_Py_m` and `_Rz_rad`. A simulation's settings say which platforms were selected; for an experiment or a file (where every platform logs every signal), a platform counts if its position isn't all zeros.

## How to call it

```matlab
b = spot.data.bodies(ds);   % b.crafts = {'RED', 'BLACK'}, b.arm = true
```

## Line by line

**Lines 7–11.** The signals; the selected platforms from the settings when it is a simulation with settings.

**Lines 12–26.** For each platform: skip it without the three position signals; otherwise it is on if selected (simulation) or if x or y has any non-zero value (measured data).

**Lines 27–34.** The arm needs RED and the three joint angles; selected in a simulation, or any non-zero angle otherwise.

<!-- xref -->
- **Used by:** `+spot/+data/summary.m`, `+spot/+data/toFigure.m`, `+spot/+data/trajectory.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/bodies.m</b> (35 lines; click to show)</summary>

```matlab
   1  function b = bodies(ds)
   2  %BODIES Which platforms (and the arm) a dataset can show on the table.
   3  %   b = spot.data.bodies(dataset) returns crafts (cell of RED, BLACK, BLUE) and arm (true/false).
   4  %   A platform needs <NAME>_Px_m, _Py_m and _Rz_rad. For a simulation, the platforms selected in
   5  %   its settings; for an experiment or a file, those whose position isn't all zero (every
   6  %   platform logs every signal, but only the ones in the run move).
   7  sig = ds.signals;
   8  selected = [];
   9  if strcmp(ds.kind, 'simulation') && isstruct(ds.settings) && isfield(ds.settings, 'platforms')
  10      selected = ds.settings.platforms;
  11  end
  12  b = struct('crafts', {{}}, 'arm', false);
  13  for n = {'RED', 'BLACK', 'BLUE'}
  14      f = strcat(n{1}, {'_Px_m', '_Py_m', '_Rz_rad'});
  15      if ~all(isfield(sig, f))
  16          continue
  17      end
  18      if isempty(selected)
  19          on = any(sig.(f{1}).Data(:)) || any(sig.(f{2}).Data(:));
  20      else
  21          on = selected.(n{1});
  22      end
  23      if on
  24          b.crafts{end + 1} = n{1};
  25      end
  26  end
  27  joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
  28  if ismember('RED', b.crafts) && all(isfield(sig, joints))
  29      if isempty(selected)
  30          b.arm = any(cellfun(@(j) any(sig.(j).Data(:)), joints));
  31      else
  32          b.arm = selected.ARM;
  33      end
  34  end
  35  end
```

</details>
<!-- /listing -->
