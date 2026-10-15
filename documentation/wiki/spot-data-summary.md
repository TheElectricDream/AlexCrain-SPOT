# spot.data.summary

**File:** `+spot/+data/summary.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/summary.m -->

## What it is for

What the Results tab shows about a loaded dataset without receiving its data: its id, name, kind, time span, sample count, and for every signal its name, number of columns, unit, readable label, platform and whether it is all zero (such signals are greyed in the list). Also which platforms it can show on the table and whether its phase times are known.

## How to call it

```matlab
s = spot.data.summary(ds, id);
s.signals(1)     % name, cols, units, zero, unit, quantity, platform, label
```

## Line by line

**Lines 7–8.** The signal names; a struct array with one row per signal, defaults filled.

**Lines 9–27.** For each signal: its columns, the units logged with it, whether all its data is zero, and the readable parts from [[spot.plotting.signalLabel|spot-plotting-signalLabel]]; units logged with the signal win over the unit guessed from the name. Track the earliest start, latest end and longest length.

**Lines 28–31.** The bodies ([[bodies|spot-data-bodies]]) and the result; `phases` is true when the dataset has settings to compute the phase times from.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/results.js`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.data.bodies|spot-data-bodies]], [[spot.data.samples|spot-data-samples]], [[spot.plotting.signalLabel|spot-plotting-signalLabel]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/summary.m</b> (32 lines; click to show)</summary>

```matlab
   1  function s = summary(ds, id)
   2  %SUMMARY What the console shows about a loaded dataset (no data).
   3  %   s = spot.data.summary(dataset, id) returns id, name, kind, source, t0 and t1 [s], samples
   4  %   (of the longest signal), signals (name, columns, units, zero = all zero, and unit, quantity,
   5  %   platform and label from spot.plotting.signalLabel), crafts and arm
   6  %   (see spot.data.bodies) and phases (whether its settings give the phase times).
   7  f = fieldnames(ds.signals);
   8  sig = struct('name', f, 'cols', 1, 'units', '', 'zero', false, 'unit', '', 'quantity', '', 'platform', '', 'label', '');
   9  t0 = inf; t1 = -inf; n = 0;
  10  for k = 1:numel(f)
  11      ts = ds.signals.(f{k});
  12      [t, y] = spot.data.samples(ts);
  13      sig(k).cols = size(y, 2);
  14      sig(k).units = char(ts.DataInfo.Units);
  15      sig(k).zero = ~any(y(:));
  16      L = spot.plotting.signalLabel(f{k});
  17      if ~isempty(sig(k).units)   % units logged with the signal win over the name's suffix
  18          L.unit = sig(k).units;
  19          L.label = sprintf('%s [%s]', L.quantity, L.unit);
  20      end
  21      sig(k).unit = L.unit; sig(k).quantity = L.quantity; sig(k).platform = L.platform; sig(k).label = L.label;
  22      if ~isempty(t)
  23          t0 = min(t0, t(1));
  24          t1 = max(t1, t(end));
  25          n = max(n, numel(t));
  26      end
  27  end
  28  b = spot.data.bodies(ds);
  29  s = struct('id', id, 'name', ds.name, 'kind', ds.kind, 'source', ds.source, ...
  30             't0', t0, 't1', t1, 'samples', n, 'signals', sig, ...
  31             'crafts', {b.crafts}, 'arm', b.arm, 'phases', isstruct(ds.settings));
  32  end
```

</details>
<!-- /listing -->
