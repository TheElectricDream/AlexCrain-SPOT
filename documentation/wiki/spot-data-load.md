# spot.data.load

**File:** `+spot/+data/load.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/load.m -->

## What it is for

Read logged data into datasets the Results tab and the 3D playback can use. Three sources:

- `source = 'simulation'`: the last simulation, `dataClass` in the base workspace, with the current settings (a copy, so later runs don't change it).
- `files.<PLATFORM>`: one experiment run, a `.mat` per platform, **merged** into one dataset. Every platform logs every signal, but only its own are measured, so `RED_*` comes from RED's file, `ARM_*` from RED's, `BLACK_*` from BLACK's, and the shared signals (`Time_s`, `Float_State`, …) are kept per platform as `<PLATFORM>_<signal>`.
- `file`: any `.mat`; each variable holding logged data becomes a dataset (and each run of the old GUI's merged files).

Experiment logs start with some samples at time 0 before the clock runs; those are dropped.

## How to call it

```matlab
ds = spot.data.load(struct('source', 'simulation'), root, s);
ds = spot.data.load(struct('files', struct('RED', 'Saved Data/x/RED/RED_1.mat', 'BLACK', '...'), 'settings', 'Saved Data/x/spot_settings.json'), root);
ds = spot.data.load(struct('file', 'something.mat', 'name', 'My run'), root);
ds{1}.signals.RED_Px_m     % a timeseries
```

## Line by line

**Lines 23–30.** Default settings `[]`; fill the spec's missing fields (`defaults`, 103–110); if the spec names a settings file, load it (`resolve`, 116–124, accepts paths relative to the project).

**Lines 32–46.** The simulation: fetch `dataClass` from the base workspace (empty if there isn't one), flatten it, error if nothing is there; name it "Simulation HH:mm:ss" unless a name was given; one dataset of kind `simulation` with the given settings.

**Lines 48–60.** An experiment: for each platform, read its file (`experimentFile`, 126–135: the `dataClass_rt` variable, or the first variable; flattened and cleaned), merge (`mergeExperiment`, 137–164), name it "Chaser+Target" unless told otherwise; one dataset of kind `experiment` with the run's own settings (none if none were saved: then no phase markers and the default arm geometry).

**Lines 62–97.** A file: load it; for each variable, split it into runs (`runsIn`, 190–206: the old GUI's merged files hold one struct per run), flatten and clean each, skip empties, and label it with the file name, the variable name when there are several, and the run name when there is one. The kind is `experiment` for a `dataClass_rt` variable, `simulation` for a `dataClass` with settings beside it, else `file`. No datasets at all is an error.

**Line 99.** None of the three given: error.

**Lines 137–164.** `mergeExperiment`: for every signal of every platform's file, decide its owner from its prefix (`ARM_*` belongs to RED). Unowned signals go in as they are (one platform) or prefixed with the platform (several). An owned signal is taken from its owner; if the owner's file isn't in the set, the first copy with any non-zero data wins.

**Lines 166–188.** `cleanAll` / `clean`: drop samples whose time doesn't move past the running maximum (the zeros before the clock starts), keeping the name and units.

<!-- xref -->
- **Used by:** `+spot/+data/listSaved.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.data.flatten|spot-data-flatten]], [[spot.data.saveSimulation|spot-data-saveSimulation]], [[spot.params.load|spot-params-load]], [[spot.plotting.platformStyle|spot-plotting-platformStyle]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/load.m</b> (206 lines; click to show)</summary>

```matlab
   1  function datasets = load(spec, root, settings)
   2  %LOAD Read logged data for the console's Results page and 3D playback.
   3  %   datasets = spot.data.load(spec, projectFolder, settings) returns a cell array of datasets,
   4  %   each a struct with name, kind ('simulation', 'experiment' or 'file'), source, signals (a flat
   5  %   struct of timeseries, see spot.data.flatten) and settings (the SPOT settings that go with it,
   6  %   or [] when unknown). spec says what to read:
   7  %     source = 'simulation'   the last simulation: dataClass in the base workspace, with the
   8  %                             current settings (a copy: later simulations don't change it)
   9  %     files.<PLATFORM>        one experiment run, a .mat file per platform (dataClass_rt); the
  10  %                             signals are merged as described below
  11  %     file                    any .mat file; each variable holding logged data is a dataset (and
  12  %                             each run of the old GUI's merged files)
  13  %     settings                (optional) settings file saved with the run (without it, a file or
  14  %                             an experiment has no settings: no phase times, default arm geometry)
  15  %     name                    (optional) name to give the dataset
  16  %   Paths may be relative to the project folder.
  17  %
  18  %   Merging an experiment: every platform logs every signal, but only its own are measured, so
  19  %   <PLATFORM>_* signals come from that platform's file and ARM_* from RED's. Other signals
  20  %   (Time_s, Float_State, ...) are kept from each file as <PLATFORM>_<signal>.
  21  %   Experiment logs start with a few samples at time 0 before the clock runs; samples whose time
  22  %   doesn't move forward are dropped.
  23  if nargin < 3
  24      settings = [];
  25  end
  26  spec = defaults(spec);
  27  runSettings = [];   % for a file or an experiment: only the settings saved with it
  28  if ~isempty(spec.settings)
  29      runSettings = spot.params.load(resolve(spec.settings, root));
  30  end
  31  
  32  if strcmp(spec.source, 'simulation')
  33      dataClass = [];
  34      try
  35          dataClass = evalin('base', 'dataClass');
  36      catch
  37      end
  38      signals = spot.data.flatten(dataClass);
  39      if isempty(fieldnames(signals))
  40          error('spot:data:load', 'There is no simulation to load: run one first (dataClass in the workspace).');
  41      end
  42      name = spec.name;
  43      if isempty(name)
  44          name = sprintf('Simulation %s', char(datetime('now', 'Format', 'HH:mm:ss')));
  45      end
  46      datasets = {make(name, 'simulation', 'MATLAB workspace (dataClass)', signals, settings)};
  47  
  48  elseif ~isempty(fieldnames(spec.files))
  49      platforms = fieldnames(spec.files);
  50      parts = struct();
  51      for p = platforms.'
  52          f = resolve(spec.files.(p{1}), root);
  53          parts.(p{1}) = experimentFile(f);
  54      end
  55      signals = mergeExperiment(parts);
  56      name = spec.name;
  57      if isempty(name)
  58          name = strjoin(cellfun(@spot.plotting.platformStyle, platforms, 'UniformOutput', false), '+');
  59      end
  60      datasets = {make(name, 'experiment', strjoin(struct2cell(spec.files).', ', '), signals, runSettings)};
  61  
  62  elseif ~isempty(spec.file)
  63      f = resolve(spec.file, root);
  64      contents = builtin('load', f);
  65      [~, base, ext] = fileparts(f);
  66      vars = fieldnames(contents);
  67      datasets = {};
  68      for v = vars.'
  69          x = contents.(v{1});
  70          subsets = runsIn(x);
  71          for k = 1:numel(subsets)
  72              signals = cleanAll(spot.data.flatten(subsets{k}.value));
  73              if isempty(fieldnames(signals))
  74                  continue
  75              end
  76              label = [base ext];
  77              if numel(vars) > 1
  78                  label = [label ' · ' v{1}]; %#ok<AGROW>
  79              end
  80              if ~isempty(subsets{k}.name)
  81                  label = [label ' · ' subsets{k}.name]; %#ok<AGROW>
  82              end
  83              if ~isempty(spec.name)
  84                  label = spec.name;
  85              end
  86              kind = 'file';
  87              if strcmp(v{1}, 'dataClass_rt')
  88                  kind = 'experiment';
  89              elseif strcmp(v{1}, 'dataClass') && isstruct(runSettings)
  90                  kind = 'simulation';   % saved by spot.data.saveSimulation
  91              end
  92              datasets{end + 1} = make(label, kind, f, signals, runSettings); %#ok<AGROW>
  93          end
  94      end
  95      if isempty(datasets)
  96          error('spot:data:load', '%s holds no logged signals (timeseries).', [base ext]);
  97      end
  98  else
  99      error('spot:data:load', 'Say what to load: source, files or file.');
 100  end
 101  end
 102  
 103  function spec = defaults(spec)
 104  d = struct('source', '', 'files', struct(), 'file', '', 'settings', '', 'name', '');
 105  for f = fieldnames(d).'
 106      if ~isfield(spec, f{1}) || isempty(spec.(f{1}))
 107          spec.(f{1}) = d.(f{1});
 108      end
 109  end
 110  end
 111  
 112  function d = make(name, kind, source, signals, settings)
 113  d = struct('name', name, 'kind', kind, 'source', source, 'signals', signals, 'settings', settings);
 114  end
 115  
 116  function f = resolve(path, root)
 117  f = char(path);
 118  if ~isfile(f)
 119      f = fullfile(root, f);
 120  end
 121  if ~isfile(f)
 122      error('spot:data:load', 'There is no file %s.', path);
 123  end
 124  end
 125  
 126  function signals = experimentFile(f)
 127  contents = builtin('load', f);
 128  if isfield(contents, 'dataClass_rt')
 129      x = contents.dataClass_rt;
 130  else
 131      vars = fieldnames(contents);
 132      x = contents.(vars{1});
 133  end
 134  signals = cleanAll(spot.data.flatten(x));
 135  end
 136  
 137  function out = mergeExperiment(parts)
 138  platforms = fieldnames(parts);
 139  out = struct();
 140  owned = @(f) regexp(f, '^(RED|BLACK|BLUE|ARM)_', 'tokens', 'once');
 141  for p = platforms.'
 142      s = parts.(p{1});
 143      for f = fieldnames(s).'
 144          o = owned(f{1});
 145          if isempty(o)
 146              if isscalar(platforms)
 147                  out.(f{1}) = s.(f{1});
 148              else
 149                  out.([p{1} '_' f{1}]) = s.(f{1});
 150              end
 151              continue
 152          end
 153          owner = o{1};
 154          if strcmp(owner, 'ARM')
 155              owner = 'RED';
 156          end
 157          % The owner's copy wins; without the owner's file, the first copy with data
 158          if ~isfield(out, f{1}) || strcmp(owner, p{1}) || ...
 159                  (~isfield(parts, owner) && ~any(out.(f{1}).Data(:)) && any(s.(f{1}).Data(:)))
 160              out.(f{1}) = s.(f{1});
 161          end
 162      end
 163  end
 164  end
 165  
 166  function signals = cleanAll(signals)
 167  for f = fieldnames(signals).'
 168      signals.(f{1}) = clean(signals.(f{1}));
 169  end
 170  end
 171  
 172  function ts = clean(ts)
 173  % Drop samples whose time doesn't move forward (the zeros before an experiment's clock starts)
 174  t = ts.Time(:);
 175  if numel(t) < 2 || size(ts.Data, 1) ~= numel(t)
 176      return
 177  end
 178  keep = [true; t(2:end) > cummax(t(1:end - 1))];
 179  if all(keep)
 180      return
 181  end
 182  d = ts.Data(keep, :);
 183  name = ts.Name;
 184  units = ts.DataInfo.Units;
 185  ts = timeseries(d, t(keep));
 186  ts.Name = name;
 187  ts.DataInfo.Units = units;
 188  end
 189  
 190  function subsets = runsIn(x)
 191  % The old GUI's merged files hold one struct per run (mergedData.<stamp>.<signal>); anything
 192  % else is one dataset
 193  subsets = {struct('name', '', 'value', x)};
 194  if ~isstruct(x) || ~isscalar(x)
 195      return
 196  end
 197  f = fieldnames(x);
 198  if isempty(f) || any(ismember(f, {'RED', 'BLACK', 'BLUE', 'ARM'}))
 199      return
 200  end
 201  isRun = cellfun(@(k) isstruct(x.(k)) && isscalar(x.(k)) && ...
 202      any(structfun(@(v) isa(v, 'timeseries'), x.(k))), f);
 203  if all(isRun)
 204      subsets = cellfun(@(k) struct('name', k, 'value', x.(k)), f, 'UniformOutput', false).';
 205  end
 206  end
```

</details>
<!-- /listing -->
