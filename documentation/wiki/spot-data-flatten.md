# spot.data.flatten

**File:** `+spot/+data/flatten.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/flatten.m -->

## What it is for

Logged data comes in several shapes: `dataClass` (a struct of timeseries), the old GUI's nested structs (`RED.Px_m`), Simulink's `logsout` Datasets, or a single timeseries. This turns any of them into one flat struct whose every field is a timeseries, nested names joined with `_` (`RED_Px_m`). Anything that isn't a logged signal is left out.

## How to call it

```matlab
signals = spot.data.flatten(dataClass);
```

## Line by line

**Lines 10–13.** An optional prefix (used when recursing); start empty.

**Lines 14–15.** A timeseries on its own becomes a field named by the prefix (or `signal`).

**Lines 16–24.** A Simulink Dataset: each element's values are flattened with the element's name (or `signalN`) added to the prefix, and merged in.

**Lines 25–34.** A scalar struct: timeseries fields are taken; struct or Dataset fields are flattened recursively with the field name added to the prefix.

**Lines 37–44.** `name`: joins prefix and field with `_`, makes it a valid MATLAB name, and caps it at the maximum name length.

**Lines 46–50.** `merge`: copy `b`'s fields into `a`.

<!-- xref -->
- **Used by:** `+spot/+control/verifyConsoleController.m`, `+spot/+data/load.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/flatten.m</b> (50 lines; click to show)</summary>

```matlab
   1  function signals = flatten(x, prefix)
   2  %FLATTEN Logged data as one flat struct of timeseries.
   3  %   signals = spot.data.flatten(x) accepts what the diagrams and the old GUI saved:
   4  %     - a struct of timeseries (dataClass, dataClass_rt)
   5  %     - nested structs of timeseries (the old GUI's merged files: RED.Px_m -> RED_Px_m)
   6  %     - a Simulink.SimulationData.Dataset (logsout), whose elements may hold either
   7  %     - a timeseries
   8  %   and returns a struct whose fields are all timeseries, named parent_child for nested ones.
   9  %   Fields that aren't logged signals are left out.
  10  if nargin < 2
  11      prefix = '';
  12  end
  13  signals = struct();
  14  if isa(x, 'timeseries')
  15      signals.(name(prefix, 'signal')) = x;
  16  elseif isa(x, 'Simulink.SimulationData.Dataset')
  17      for k = 1:x.numElements
  18          e = x.getElement(k);
  19          n = e.Name;
  20          if isempty(n)
  21              n = sprintf('signal%d', k);
  22          end
  23          signals = merge(signals, spot.data.flatten(e.Values, name(prefix, n)));
  24      end
  25  elseif isstruct(x) && isscalar(x)
  26      for f = fieldnames(x).'
  27          v = x.(f{1});
  28          if isa(v, 'timeseries')
  29              signals.(name(prefix, f{1})) = v;
  30          elseif (isstruct(v) && isscalar(v)) || isa(v, 'Simulink.SimulationData.Dataset')
  31              signals = merge(signals, spot.data.flatten(v, name(prefix, f{1})));
  32          end
  33      end
  34  end
  35  end
  36  
  37  function n = name(prefix, field)
  38  if isempty(prefix)
  39      n = matlab.lang.makeValidName(field);
  40  else
  41      n = matlab.lang.makeValidName([prefix '_' field]);
  42  end
  43  n = n(1:min(end, namelengthmax));
  44  end
  45  
  46  function a = merge(a, b)
  47  for f = fieldnames(b).'
  48      a.(f{1}) = b.(f{1});
  49  end
  50  end
```

</details>
<!-- /listing -->
