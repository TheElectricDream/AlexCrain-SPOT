# spot.params.reset

**File:** `+spot/+params/reset.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/reset.m -->

## What it is for

The *Reset…* button's "settings" option: back to the template's defaults, keeping the diagram you had selected (and its solver settings), after copying the old file aside so that nothing is lost.

## How to call it

```matlab
[s, backup] = spot.params.reset(current, projectFolder, settingsFile, backupFolder);
```

`backup` is the path of the copy, or `''` when there was no file to back up.

## Line by line

**Lines 8–16.** If the settings file exists: make the backup folder if needed, build a name `settings_before_reset_<yyyyMMdd_HHmmss>.json` from the current time, and copy the file there. The console uses `.spot_cache/` as the backup folder.

**Lines 17–18.** Fresh defaults, with the diagram name carried over from `current`.

**Lines 19–21.** When a diagram is selected, take its own solver settings ([[spot.sim.readSolver|spot-sim-readSolver]]), as selecting it would have.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.params.defaults|spot-params-defaults]], [[spot.sim.readSolver|spot-sim-readSolver]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/reset.m</b> (22 lines; click to show)</summary>

```matlab
   1  function [s, backup] = reset(current, root, settingsFile, backupFolder)
   2  %RESET Back to the template's default settings, keeping the diagram (a clean slate).
   3  %   [s, backup] = spot.params.reset(current, projectFolder, settingsFile, backupFolder) returns
   4  %   spot.params.defaults() with the diagram of CURRENT still selected and that diagram's own solver
   5  %   settings. The settings file as it was is copied first to
   6  %   <backupFolder>/settings_before_reset_<yyyyMMdd_HHmmss>.json, so it can be loaded back; BACKUP is
   7  %   that path ('' when there was no file yet).
   8  backup = '';
   9  if isfile(settingsFile)
  10      if ~isfolder(backupFolder)
  11          mkdir(backupFolder);
  12      end
  13      backup = fullfile(backupFolder, sprintf('settings_before_reset_%s.json', ...
  14          char(datetime('now', 'Format', 'yyyyMMdd_HHmmss'))));
  15      copyfile(settingsFile, backup);
  16  end
  17  s = spot.params.defaults();
  18  s.diagram = current.diagram;
  19  if ~isempty(s.diagram)
  20      s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
  21  end
  22  end
```

</details>
<!-- /listing -->
