# spot.experiment.buildBoard

**File:** `+spot/+experiment/buildBoard.m` · **Package:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+experiment/buildBoard.m -->

## What it is for

Where the build happens: the lab console's address (the settings' `labConsoleAddress`), its user (`spot-tx2`), the build folder on it, and the per-project workspace folder MATLAB compiles in (`/home/spot-tx2/ExperimentSoftware/MATLAB_ws/R2026a/<project path>/`), named after the project's path as the old GUI did, so two projects never overwrite each other's build.

## How to call it

```matlab
b = spot.experiment.buildBoard(projectFolder, s);     % or without s: the project's spot_settings.json
b.address, b.user, b.buildDir, b.workspaceDir
```

## Line by line

**Line 7.** The MATLAB release as `R2026a`.

**Lines 9–16.** Without settings, load the project's file (or the defaults).

**Lines 17–19.** The address from the settings; the fixed user and build folder.

**Lines 20–24.** The project's path with `:` removed, spaces and parentheses replaced by `_`, backslashes by slashes and leading slashes dropped, under `MATLAB_ws/<release>/`.

<!-- xref -->
- **Used by:** `+spot/+experiment/connect.m`, `+spot/+experiment/plan.m`
- **Uses:** [[spot.experiment.build|spot-experiment-build]], [[spot.params.defaults|spot-params-defaults]], [[spot.params.load|spot-params-load]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+experiment/buildBoard.m</b> (25 lines; click to show)</summary>

```matlab
   1  function b = buildBoard(root, s)
   2  %BUILDBOARD Where experiments are built: the lab console (Jetson TX2).
   3  %   b = spot.experiment.buildBoard(projectFolder, s) returns address, user, buildDir and
   4  %   workspaceDir: MATLAB copies the generated code to the TX2 and compiles it in workspaceDir; the
   5  %   lab console then copies the program to the platforms (see spot.experiment.build). The address
   6  %   is the settings' labConsoleAddress (s, or the project's spot_settings.json without it).
   7  release = ['R' version('-release')];
   8  b = struct();
   9  if nargin < 2
  10      file = fullfile(root, 'spot_settings.json');
  11      if isfile(file)
  12          s = spot.params.load(file);
  13      else
  14          s = spot.params.defaults();
  15      end
  16  end
  17  b.address = strtrim(char(s.labConsoleAddress));
  18  b.user = 'spot-tx2';
  19  b.buildDir = ['/home/' b.user '/ExperimentSoftware/'];
  20  % One folder per project on the TX2, named after the project's path as the old GUI did
  21  compileDir = regexprep(root, '[:]', '');
  22  compileDir = regexprep(compileDir, '[ ()]', '_');
  23  compileDir = strrep(compileDir, '\', '/');
  24  b.workspaceDir = [b.buildDir 'MATLAB_ws/' release '/' regexprep(compileDir, '^/+', '') '/'];
  25  end
```

</details>
<!-- /listing -->
