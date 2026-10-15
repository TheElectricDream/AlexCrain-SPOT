# spot.data.listSaved

**File:** `+spot/+data/listSaved.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/listSaved.m -->

## What it is for

The *Saved Data…* dialog's list: every run in the project's `Saved Data/` folder, newest first, and for each run the items that can be loaded. Four layouts are recognised:

| `kind` | Layout | Made by |
|---|---|---|
| `experiment` | `<run>/RED/RED_1.mat`, `<run>/BLACK/BLACK_1.mat`, … (one `.mat` per platform per run number) | the Experiment tab's *Collect* |
| `simulation` | `<run>/simulation.mat` | *Save Results* ([[saveSimulation|spot-data-saveSimulation]]) |
| `legacy` | `ExperimentData_RED_<stamp>/…_1.mat` (one folder per platform, grouped by stamp) | the old GUI |
| `files` | any other folder with `.mat` files, and loose `.mat` files in `Saved Data/` itself | you |

## How to call it

```matlab
runs = spot.data.listSaved(projectFolder);
runs(1).name, runs(1).kind, runs(1).label, runs(1).items(1).label, runs(1).items(1).files, runs(1).settings
```

## Line by line

**Lines 18–22.** The folder; an empty struct array with the right fields if it doesn't exist.

**Lines 23–32.** A map to group the old GUI's per-platform folders by stamp; walk the folder's entries, skipping dot files and plain files.

**Lines 33–43.** A folder named `ExperimentData_<PLATFORM>_<stamp>` goes into the legacy map under its stamp (platform → folder name) and is handled later.

**Lines 44–47.** Otherwise start a `run` record (kind `files` until proven otherwise, modified time from the folder) and look for `RED/`, `BLACK/`, `BLUE/` subfolders.

**Lines 48–77.** With platform subfolders it is an experiment: in each, every `<P>_<n>.mat` is filed under run number `n` (platform → relative path; `rel`, lines 170–172, strips the project folder and uses forward slashes); the run's modified time follows the newest file. A platform folder with `ExpLog*` files but no `.mat` means the logs weren't converted yet. Items are made per run number in order, plus a *convert* item when needed.

**Lines 78–88.** Without platform folders: every `.mat` in the folder is an item on its own; a `simulation.mat` marks the run as a simulation.

**Lines 89–95.** A `spot_settings.json` beside the data is recorded; runs with no items are dropped.

**Lines 98–126.** The legacy groups: for each stamp, every platform folder's `.mat` files are filed by their trailing `_<n>`, and items made per run number.

**Lines 128–137.** Loose `.mat` files directly in `Saved Data/` become one run called "Saved Data".

**Lines 139–140.** Newest first.

**Lines 141–154.** Short labels: `runLabel` (157–168) reads the time stamp at the end of a folder name (either `yyyyMMdd_HHmmss` or the old `yyyy_M_d_H_m_s_ms`) into "MM-dd HH:mm". A simulation's dataset takes the run's label; a file keeps its own name (empty here, [[load|spot-data-load]] names it); an experiment run's datasets get "label · Run n".

**Lines 174–176.** `posix`: MATLAB datenum → POSIX seconds, local time zone, for the page's clock.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/data.js`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.data.load|spot-data-load]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/listSaved.m</b> (176 lines; click to show)</summary>

```matlab
   1  function runs = listSaved(root)
   2  %LISTSAVED What is in the project's "Saved Data" folder, for the console's Results page.
   3  %   runs = spot.data.listSaved(projectFolder) returns one entry per run, newest first, with
   4  %     name      folder name
   5  %     kind      'experiment'  <run>/<PLATFORM>/<PLATFORM>_<n>.mat (collected by the console)
   6  %               'legacy'      ExperimentData_<PLATFORM>_<stamp>/..._<n>.mat (the old GUI), grouped
   7  %                             by stamp across platforms
   8  %               'simulation'  <run>/simulation.mat (saved from the console)
   9  %               'files'       any other .mat files
  10  %     modified  POSIX time of the newest file
  11  %     items     what can be loaded: label, name (for the loaded dataset; '' = the file's own),
  12  %               and either files (PLATFORM -> path) or file (path);
  13  %               paths are relative to the project folder. An experiment whose ExpLog files
  14  %               weren't converted yet has an item with convert = true and no files.
  15  %     label     a short name for it, from its time stamp ("10-07 15:02")
  16  %     settings  path of the settings saved with the run, or ''
  17  %     folder    the run's folder (full path; '' for the old GUI's runs)
  18  folder = fullfile(root, 'Saved Data');
  19  runs = struct('name', {}, 'kind', {}, 'modified', {}, 'items', {}, 'settings', {}, 'folder', {});
  20  if ~isfolder(folder)
  21      return
  22  end
  23  legacy = containers.Map();
  24  entries = dir(folder);
  25  for e = entries(:).'
  26      if e.name(1) == '.'
  27          continue
  28      end
  29      path = fullfile(folder, e.name);
  30      if ~e.isdir
  31          continue
  32      end
  33      tok = regexp(e.name, '^ExperimentData_(RED|BLACK|BLUE)_(.+)$', 'tokens', 'once');
  34      if ~isempty(tok)
  35          % The old GUI: one folder per platform; gather the platforms of one run (same stamp)
  36          if ~isKey(legacy, tok{2})
  37              legacy(tok{2}) = struct();
  38          end
  39          g = legacy(tok{2});
  40          g.(tok{1}) = e.name;
  41          legacy(tok{2}) = g;
  42          continue
  43      end
  44      run = struct('name', e.name, 'kind', 'files', 'modified', posix(e.datenum), 'items', [], 'settings', '', 'folder', path);
  45      items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
  46      platforms = {'RED', 'BLACK', 'BLUE'};
  47      hasPlatform = cellfun(@(p) isfolder(fullfile(path, p)), platforms);
  48      if any(hasPlatform)
  49          run.kind = 'experiment';
  50          found = struct();
  51          undecoded = false;
  52          for p = platforms(hasPlatform)
  53              mats = dir(fullfile(path, p{1}, [p{1} '_*.mat']));
  54              for m = mats(:).'
  55                  n = sscanf(m.name, [p{1} '_%d.mat']);
  56                  if isempty(n)
  57                      continue
  58                  end
  59                  key = sprintf('n%d', n);
  60                  if ~isfield(found, key)
  61                      found.(key) = struct();
  62                  end
  63                  found.(key).(p{1}) = rel(root, fullfile(m.folder, m.name));
  64                  run.modified = max(run.modified, posix(m.datenum));
  65              end
  66              undecoded = undecoded || (isempty(mats) && ~isempty(dir(fullfile(path, p{1}, 'ExpLog*'))));
  67          end
  68          keys = fieldnames(found);
  69          [~, order] = sort(cellfun(@(k) sscanf(k, 'n%d'), keys));
  70          for k = keys(order).'
  71              items(end + 1) = struct('label', sprintf('Run %d', sscanf(k{1}, 'n%d')), ...
  72                  'files', found.(k{1}), 'file', '', 'convert', false); %#ok<AGROW>
  73          end
  74          if undecoded
  75              items(end + 1) = struct('label', 'ExpLog Files Not Converted Yet', 'files', struct(), ...
  76                  'file', '', 'convert', true); %#ok<AGROW>
  77          end
  78      else
  79          mats = dir(fullfile(path, '*.mat'));
  80          if isfile(fullfile(path, 'simulation.mat'))
  81              run.kind = 'simulation';
  82          end
  83          for m = mats(:).'
  84              items(end + 1) = struct('label', m.name, 'files', struct(), ...
  85                  'file', rel(root, fullfile(m.folder, m.name)), 'convert', false); %#ok<AGROW>
  86              run.modified = max(run.modified, posix(m.datenum));
  87          end
  88      end
  89      if isfile(fullfile(path, 'spot_settings.json'))
  90          run.settings = rel(root, fullfile(path, 'spot_settings.json'));
  91      end
  92      if ~isempty(items)
  93          run.items = items;
  94          runs(end + 1) = run; %#ok<AGROW>
  95      end
  96  end
  97  
  98  for stamp = legacy.keys()
  99      g = legacy(stamp{1});
 100      run = struct('name', ['ExperimentData_' stamp{1}], 'kind', 'legacy', 'modified', 0, 'items', [], 'settings', '', 'folder', '');
 101      found = struct();
 102      for p = fieldnames(g).'
 103          mats = dir(fullfile(folder, g.(p{1}), '*.mat'));
 104          for m = mats(:).'
 105              n = regexp(m.name, '_(\d+)\.mat$', 'tokens', 'once');
 106              if isempty(n)
 107                  continue
 108              end
 109              key = ['n' n{1}];
 110              if ~isfield(found, key)
 111                  found.(key) = struct();
 112              end
 113              found.(key).(p{1}) = rel(root, fullfile(m.folder, m.name));
 114              run.modified = max(run.modified, posix(m.datenum));
 115          end
 116      end
 117      items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
 118      for k = sort(fieldnames(found)).'
 119          items(end + 1) = struct('label', sprintf('Run %s', k{1}(2:end)), 'files', found.(k{1}), ...
 120              'file', '', 'convert', false); %#ok<AGROW>
 121      end
 122      if ~isempty(items)
 123          run.items = items;
 124          runs(end + 1) = run; %#ok<AGROW>
 125      end
 126  end
 127  
 128  % Loose .mat files directly in Saved Data
 129  mats = dir(fullfile(folder, '*.mat'));
 130  if ~isempty(mats)
 131      items = struct('label', {mats.name}, 'files', struct(), 'file', '', 'convert', false);
 132      for k = 1:numel(mats)
 133          items(k).file = rel(root, fullfile(mats(k).folder, mats(k).name));
 134      end
 135      runs(end + 1) = struct('name', 'Saved Data', 'kind', 'files', 'modified', posix(max([mats.datenum])), ...
 136          'items', items, 'settings', '', 'folder', folder);
 137  end
 138  
 139  [~, order] = sort([runs.modified], 'descend');
 140  runs = runs(order);
 141  % Short names for the console: "10-07 15:02" for a run, "10-07 15:02 · Run 2" for its datasets
 142  for r = 1:numel(runs)
 143      runs(r).label = runLabel(runs(r).name);
 144      for k = 1:numel(runs(r).items)
 145          it = runs(r).items(k);
 146          if strcmp(runs(r).kind, 'simulation')
 147              runs(r).items(k).name = runs(r).label;
 148          elseif ~isempty(it.file)
 149              runs(r).items(k).name = '';   % a file keeps its own name (spot.data.load)
 150          else
 151              runs(r).items(k).name = sprintf('%s · %s', runs(r).label, it.label);
 152          end
 153      end
 154  end
 155  end
 156  
 157  function label = runLabel(name)
 158  % Template_..._20261007_150241 -> "10-07 15:02"; ExperimentData_2026_10_7_11_29_45_3901 -> "10-07 11:29"
 159  label = name;
 160  t = regexp(name, '(\d{4})(\d{2})(\d{2})_(\d{2})(\d{2})\d{2}$', 'tokens', 'once');
 161  if isempty(t)
 162      t = regexp(name, '(\d{4})_(\d{1,2})_(\d{1,2})_(\d{1,2})_(\d{1,2})_\d{1,2}(_\d+)?$', 'tokens', 'once');
 163  end
 164  if ~isempty(t)
 165      n = str2double(t(1:5));
 166      label = sprintf('%02d-%02d %02d:%02d', n(2), n(3), n(4), n(5));
 167  end
 168  end
 169  
 170  function p = rel(root, path)
 171  p = strrep(path(numel(root) + 2:end), '\', '/');
 172  end
 173  
 174  function t = posix(datenum)
 175  t = posixtime(datetime(datenum, 'ConvertFrom', 'datenum', 'TimeZone', 'local'));
 176  end
```

</details>
<!-- /listing -->
