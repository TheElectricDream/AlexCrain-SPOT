# spot.sim.liveData

**File:** `+spot/+sim/liveData.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/liveData.m -->

## What it is for

During a simulation the diagram's Live Stream blocks ([[spot.hardware.liveStream|spot-hardware-liveStream]]) don't send UDP; they call this function with their stream's name, the time and the values. It keeps everything in persistent variables so that the console's Live Plot can fetch what is new while the run is going (`'new'`) and the whole record afterwards (`'get'`).

## How to call it

```matlab
spot.sim.liveData('reset')          % start a new record (run does this)
spot.sim.liveData(codes, t, v)      % from a block: stream named by codes (uint8), values v at t
all = spot.sim.liveData('get')      % struct array: name, t (column), v (one row per sample)
new = spot.sim.liveData('new')      % only what came since the last 'new'
names = spot.sim.liveData('names')
```

## Line by line

**Lines 10–13.** The record lives in persistent variables: `names` (cell of stream names), `T` and `V` (one preallocated array per stream), `N` (how many samples each has) and `delivered` (how many each had at the last `'new'`). `empty` (lines 66–72) initialises them.

**Lines 15–32.** When the first argument is text, it is a command: `reset` empties everything; `get` collects every stream from its first sample; `new` collects from the sample after the last delivered one, remembers the new counts and drops streams with nothing new; `names` returns the names. Anything else is an error.

**Lines 34–35.** Otherwise it is a block call: the name arrives as character codes (a MATLAB Function block can't pass a string to an extrinsic function), so convert to a char row and look it up.

**Lines 36–44.** The values as one row. A new stream gets a slot: 1024 rows preallocated for time and values, counters at zero.

**Lines 45–48.** When the preallocated rows are used up, double them (amortised constant time per sample).

**Lines 49–53.** Store the sample. Values are only stored if their count matches the stream's width (a stream can't change size mid-run).

**Lines 55–63.** `collect` (a nested function, so it sees the persistent variables): for each requested stream index, its name and the slice from `from(j)` to `N(i)`.

<!-- xref -->
- **Used by:** `+spot/+hardware/liveStream.m`, `+spot/+sim/run.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.hardware.liveStream|spot-hardware-liveStream]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/liveData.m</b> (72 lines; click to show)</summary>

```matlab
   1  function out = liveData(name, t, v)
   2  %LIVEDATA The Live Stream blocks' signals recorded during a simulation (by spot.hardware.liveStream).
   3  %   spot.sim.liveData('reset')            start a new record (spot.sim.run does this)
   4  %   spot.sim.liveData(codes, t, v)        stream named by codes (uint8 characters) has values v at t
   5  %   all = spot.sim.liveData('get')        every stream: struct array with name, t (column) and v
   6  %                                         (one row per sample)
   7  %   new = spot.sim.liveData('new')        the samples added since the previous 'new', same form,
   8  %                                         for the console's Live Plot while the simulation runs
   9  %   names = spot.sim.liveData('names')    the streams' names
  10  persistent names T V N delivered
  11  if isempty(N)
  12      [names, T, V, N, delivered] = empty();
  13  end
  14  
  15  if ischar(name) || isstring(name)
  16      switch char(name)
  17          case 'reset'
  18              [names, T, V, N, delivered] = empty();
  19              out = [];
  20          case 'get'
  21              out = collect(1:numel(names), ones(1, numel(names)));
  22          case 'new'
  23              out = collect(1:numel(names), delivered + 1);
  24              delivered = N;
  25              out = out(arrayfun(@(s) ~isempty(s.t), out));
  26          case 'names'
  27              out = names;
  28          otherwise
  29              error('spot:sim:liveData', 'Use ''reset'', ''get'', ''new'' or ''names''.');
  30      end
  31      return
  32  end
  33  
  34  label = char(reshape(name, 1, []));
  35  k = find(strcmp(names, label), 1);
  36  v = reshape(double(v), 1, []);
  37  if isempty(k)
  38      names{end + 1} = label;
  39      k = numel(names);
  40      T{k} = zeros(1024, 1);
  41      V{k} = zeros(1024, numel(v));
  42      N(k) = 0;
  43      delivered(k) = 0;
  44  end
  45  if N(k) == numel(T{k})   % grow by doubling
  46      T{k}(end + 1:2 * end, 1) = 0;
  47      V{k}(end + 1:2 * end, :) = 0;
  48  end
  49  N(k) = N(k) + 1;
  50  T{k}(N(k)) = double(t);
  51  if size(V{k}, 2) == numel(v)
  52      V{k}(N(k), :) = v;
  53  end
  54  
  55      function s = collect(ks, from)
  56          s = struct('name', {}, 't', {}, 'v', {});
  57          for j = 1:numel(ks)
  58              i = ks(j);
  59              s(j).name = names{i};
  60              s(j).t = T{i}(from(j):N(i));
  61              s(j).v = V{i}(from(j):N(i), :);
  62          end
  63      end
  64  end
  65  
  66  function [names, T, V, N, delivered] = empty()
  67  names = {};
  68  T = {};
  69  V = {};
  70  N = zeros(1, 0);
  71  delivered = zeros(1, 0);
  72  end
```

</details>
<!-- /listing -->
