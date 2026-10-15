# spot.data.samples

**File:** `+spot/+data/samples.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/samples.m -->

## What it is for

A timeseries' time and data as plain double columns, whatever way Simulink stored them: N×m, or a×b×N for some blocks. Repeated time stamps are removed (and their rows), so the result can be interpolated.

## How to call it

```matlab
[t, y] = spot.data.samples(ds.signals.RED_Px_m);   % t N x 1, y N x m
```

## Line by line

**Lines 5–6.** Time as a column, data as doubles.

**Lines 7–9.** If the data is 3-D, or its first dimension isn't the sample count while its last is, reshape it so samples run down the rows.

**Lines 10–11.** Unique, increasing times, and the matching rows.

<!-- xref -->
- **Used by:** `+spot/+data/series.m`, `+spot/+data/summary.m`, `+spot/+data/toFigure.m`, `+spot/+data/trajectory.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/samples.m</b> (12 lines; click to show)</summary>

```matlab
   1  function [t, y] = samples(ts)
   2  %SAMPLES Time and data of a logged signal as plain columns.
   3  %   [t, y] = spot.data.samples(ts) returns the time stamps (N x 1, each once, increasing) and the
   4  %   data as an N x m double matrix, whether Simulink logged it as N x m or as a x b x N.
   5  t = double(ts.Time(:));
   6  y = double(ts.Data);
   7  if ndims(y) == 3 || (size(y, 1) ~= numel(t) && size(y, ndims(y)) == numel(t))
   8      y = reshape(y, [], numel(t)).';
   9  end
  10  [t, keep] = unique(t);
  11  y = y(keep, :);
  12  end
```

</details>
<!-- /listing -->
