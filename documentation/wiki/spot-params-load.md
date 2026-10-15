# spot.params.load

**File:** `+spot/+params/load.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/load.m -->

## What it is for

Read a settings file. Anything the file doesn't have gets its default, so a file written by an older version keeps working, and two older formats are converted on the way.

## How to call it

```matlab
s = spot.params.load('spot_settings.json');
s = spot.params.load('old_gui_state.mat');    % a state saved by the old App Designer GUI
```

## Line by line

**Lines 5–9.** Split the file name; if its extension is `.mat` it is an old GUI state, which [[fromLegacyState|spot-params-fromLegacyState]] converts, and we are done.

**Line 10.** Otherwise read the whole file as text (`fileread`) and decode the JSON into a struct `x`. JSON arrays come back as column vectors, which [[merge|spot-params-merge]] turns back into rows.

**Lines 11–18.** First conversion: files written when the arm still had a "drop" pose. If `initial.ARM` has `drop` but no `initial`, the drop pose becomes the initial pose. It is inside `try` because an incomplete file may not have `initial.ARM` at all; then there is nothing to convert.

**Line 19.** Overlay the file on the defaults: every field present in the file replaces the default, every field absent keeps it, and fields the defaults don't know are dropped.

**Lines 20–23.** Second conversion: files written before the PhaseSpace data rate was a setting. If the file has a numeric scalar `sampleRate` but no `dataRate`, the data rate is set to half the sample rate (what the diagram used to assume), rounded and kept within 1–960 Hz.

<!-- xref -->
- **Used by:** `+spot/+control/verifyConsoleController.m`, `+spot/+data/load.m`, `+spot/+experiment/buildBoard.m`, `+spot/+params/apply.m`, `+spot/+worker/Worker.m`, `SPOT.m`
- **Uses:** [[spot.params.defaults|spot-params-defaults]], [[spot.params.fromLegacyState|spot-params-fromLegacyState]], [[spot.params.merge|spot-params-merge]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/load.m</b> (24 lines; click to show)</summary>

```matlab
   1  function s = load(file)
   2  %LOAD Read settings from a JSON file, filling anything missing with the defaults.
   3  %   s = spot.params.load(file). Old GUI state files (.mat) are converted with
   4  %   spot.params.fromLegacyState.
   5  [~, ~, ext] = fileparts(file);
   6  if strcmpi(ext, '.mat')
   7      s = spot.params.fromLegacyState(file);
   8      return
   9  end
  10  x = jsondecode(fileread(file));
  11  % Settings saved before the drop pose was removed: the arm's angles were its drop pose, and are
  12  % now its initial pose (the platforms' drop poses are simply dropped)
  13  try
  14      if isfield(x.initial.ARM, 'drop') && ~isfield(x.initial.ARM, 'initial')
  15          x.initial.ARM.initial = x.initial.ARM.drop;
  16      end
  17  catch
  18  end
  19  s = spot.params.merge(spot.params.defaults(), x);
  20  % Settings saved before the PhaseSpace data rate was a setting: it was half the sample rate
  21  if ~isfield(x, 'dataRate') && isfield(x, 'sampleRate') && isnumeric(x.sampleRate) && isscalar(x.sampleRate)
  22      s.dataRate = min(max(round(x.sampleRate / 2), 1), 960);
  23  end
  24  end
```

</details>
<!-- /listing -->
