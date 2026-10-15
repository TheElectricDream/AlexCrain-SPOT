# spot.experiment.decodeData

**File:** `+spot/+experiment/decodeData.m` · **Package:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+experiment/decodeData.m -->

## What it is for

The platforms log their data as plain text (`ExpLog*` files written by the [[Data Logger block|Hardware-small-blocks]]: one row per step, time first, then every signal of the *Data Logging Bus* side by side). This turns each file into a `.mat` with `dataClass_rt`, a struct of timeseries named like the simulation's `dataClass`, so the Results tab treats experiments and simulations alike. To know how many columns each signal takes, the diagram is compiled (a model update, no code generation) and the bus's port dimensions read.

## How to call it

```matlab
files = spot.experiment.decodeData('Saved Data/<run>', s, projectFolder);   % {'.../RED/RED_1.mat', ...}
```

## Line by line

**Lines 8–10.** Load the diagram and put it in the compiled state (`feval(model, [], [], [], 'compile')`), to be terminated on exit.

**Lines 11–25.** The *Data Logging Bus* block's input ports: for each, the compiled width (second dimension) and the signal's name from its hierarchy (or `UnnamedSignal_k`).

**Lines 27–46.** For each platform folder, each `ExpLog*` file: read it as a numeric matrix, zero out NaNs, take column 1 as time, then walk the columns giving each signal its slice of `width` columns as a timeseries under a valid field name; save as `<PLATFORM>_<k>.mat` beside the log and collect the path.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.sim.loadDiagram|spot-sim-loadDiagram]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+experiment/decodeData.m</b> (47 lines; click to show)</summary>

```matlab
   1  function files = decodeData(folder, s, root)
   2  %DECODEDATA Turn the experiment data files (ExpLog*) into MATLAB timeseries.
   3  %   files = spot.experiment.decodeData(folder, s, projectFolder) reads every ExpLog* file in
   4  %   folder/<PLATFORM>/ (as downloaded by the console into "Saved Data"), splits the columns into
   5  %   the signals of the diagram's "Data Logger Subsystem/Data Logging Bus" (column 1 is time), and
   6  %   saves each file as <PLATFORM>_<n>.mat holding dataClass_rt (a struct of timeseries) next to it.
   7  %   The diagram is compiled (model update, not code generation) to learn the signal widths.
   8  model = spot.sim.loadDiagram(s.diagram, root);
   9  feval(model, [], [], [], 'compile');
  10  terminate = onCleanup(@() feval(model, [], [], [], 'term'));
  11  ph = get_param([model '/Data Logger Subsystem/Data Logging Bus'], 'PortHandles');
  12  sh = get_param(ph.Inport, 'SignalHierarchy');
  13  if ~iscell(sh)
  14      sh = {sh};
  15  end
  16  widths = zeros(1, numel(sh));
  17  names = cell(1, numel(sh));
  18  for k = 1:numel(sh)
  19      d = get_param(ph.Inport(k), 'CompiledPortDimensions');
  20      widths(k) = d(2);
  21      names{k} = sh{k}.SignalName;
  22      if isempty(names{k})
  23          names{k} = sprintf('UnnamedSignal_%d', k);
  24      end
  25  end
  26  
  27  files = {};
  28  for p = {'RED', 'BLACK', 'BLUE'}
  29      logs = dir(fullfile(folder, p{1}, 'ExpLog*'));
  30      for k = 1:numel(logs)
  31          data = importdata(fullfile(logs(k).folder, logs(k).name));
  32          data(isnan(data)) = 0;
  33          t = data(:, 1);
  34          dataClass_rt = struct();
  35          col = 1;
  36          for j = 1:numel(names)
  37              ts = timeseries(data(:, col:col + widths(j) - 1), t);
  38              ts.Name = names{j};
  39              dataClass_rt.(matlab.lang.makeValidName(names{j})) = ts;
  40              col = col + widths(j);
  41          end
  42          out = fullfile(logs(k).folder, sprintf('%s_%d.mat', p{1}, k));
  43          save(out, 'dataClass_rt', '-mat');
  44          files{end + 1} = out; %#ok<AGROW>
  45      end
  46  end
  47  end
```

</details>
<!-- /listing -->
