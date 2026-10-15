# spot.experiment.build

**File:** `+spot/+experiment/build.m` · **Package:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+experiment/build.m -->

## What it is for

The Experiment tab's *Build*: compile the diagram into the program the platforms run. The settings go into the workspace (they are compiled into the program), the diagram is temporarily changed for the build, `slbuild` generates the code and compiles it on the TX2, and every change is undone without saving the diagram. The build's output is written live to `.spot_cache/build.log` so the page can show it as it happens.

## How to call it

```matlab
spot.experiment.connect(projectFolder, password);
out = spot.experiment.build(s, projectFolder);
out.program    % '/home/spot-tx2/ExperimentSoftware/MATLAB_ws/R2026a/.../Template.elf'
out.seconds, out.log, out.platforms
```

## Line by line

**Lines 15–21.** The plan; refuse without the Jetson support package or without a platform.

**Lines 22–24.** The model and board; apply the settings (the generated code takes the workspace's values).

**Lines 26–31.** The support package's preferences: build on the TX2, as its user, in the build and workspace folders.

**Lines 33–40.** Remember whether the diagram was already modified, the active configuration set, and the current values of what will be changed (the plant block's commented state, solver type and name, hardware board, build action). `onCleanup` undoes all of it when the function ends, however it ends (`undo`, lines 85–96: restore the values, `simMode` back to 1, and clear the diagram's dirty flag if it was clean, so it looks untouched).

**Lines 41–43.** Comment out *Simulate Plant Dynamics* (on the platforms the real world is the plant), set the fixed-step `ode4` solver and the NVIDIA Jetson board.

**Lines 44–49.** The build action must be *Build*: *Build and run* would start the program on the TX2 as root. Verify it took.

**Line 50.** `simMode = 0` while building, so the compiled program knows it is an experiment.

**Lines 52–63.** Make `.spot_cache/`, delete the old `build.log`, start `diary` into it (stopped on exit), print a header line.

**Lines 65–68.** Time `slbuild(model)`; stop the diary.

**Lines 69–73.** Read the log back without hyperlinks; return the model, the program path, the time, the log and the platforms.

**Lines 76–83.** `buildAction`: the current build action, or `''` when the board isn't a Jetson target (then there is none to restore).

<!-- xref -->
- **Used by:** `+spot/+experiment/Contents.m`, `+spot/+experiment/buildBoard.m`, `+spot/+experiment/plan.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.experiment.connect|spot-experiment-connect]], [[spot.experiment.plan|spot-experiment-plan]], [[spot.params.apply|spot-params-apply]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+experiment/build.m</b> (96 lines; click to show)</summary>

```matlab
   1  function out = build(s, root)
   2  %BUILD Build the selected diagram for the platforms, on the lab console (TX2).
   3  %   out = spot.experiment.build(s, projectFolder) puts the settings into the workspace, then, for the
   4  %   build only, comments out the diagram's "Simulate Plant Dynamics" block, sets the solver to
   5  %   Fixed-step ode4, the hardware board to NVIDIA Jetson and its build action to "Build" (never
   6  %   "Build and run": the program must not run on the TX2), and runs slbuild with the TX2 as the
   7  %   board (connect first with spot.experiment.connect). Afterwards every change to the diagram is
   8  %   undone and the diagram is NOT saved (the old GUI saved it with the plant commented out).
   9  %   The lab console then deploys the program to the platforms.
  10  %
  11  %   What slbuild prints goes to <projectFolder>/.spot_cache/build.log as it happens (the SPOT
  12  %   Console shows it live) as well as to the Command Window.
  13  %
  14  %   out: model, program (path on the TX2), seconds, log (what the build printed).
  15  p = spot.experiment.plan(s, root);
  16  if ~p.hasJetsonSupport
  17      error('spot:experiment:build', 'The MATLAB Coder Support Package for NVIDIA Jetson is not installed.');
  18  end
  19  if isempty(p.platforms)
  20      error('spot:experiment:build', 'Select at least one platform (Scenario) first.');
  21  end
  22  model = p.model;
  23  b = p.board;
  24  spot.params.apply(s, root);
  25  
  26  % Build on the TX2, in this project's folder there
  27  group = 'Hardware_Connectivity_Installer_NVIDIAJetson';
  28  setpref(group, 'DefaultIpAddress', b.address);
  29  setpref(group, 'DefaultUserName', b.user);
  30  setpref(group, 'DefaultBuildDirPref', b.buildDir);
  31  setpref(group, 'DefaulttargetwsdirPref', b.workspaceDir);
  32  
  33  % Changes for the build, all undone afterwards
  34  wasDirty = strcmp(get_param(model, 'Dirty'), 'on');
  35  cs = getActiveConfigSet(model);
  36  plant = [model '/Simulate Plant Dynamics'];
  37  old = struct('commented', get_param(plant, 'Commented'), 'solverType', get_param(model, 'SolverType'), ...
  38               'solver', get_param(model, 'SolverName'), 'board', get_param(cs, 'HardwareBoard'), ...
  39               'action', buildAction(cs));
  40  restore = onCleanup(@() undo(model, cs, plant, old, wasDirty));
  41  set_param(plant, 'Commented', 'on');
  42  set_param(model, 'SolverType', 'Fixed-step', 'SolverName', 'ode4');
  43  set_param(cs, 'HardwareBoard', 'NVIDIA Jetson');
  44  % Build only: with "Build and run" MATLAB would also start the program on the TX2 (as root)
  45  codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', 'Build');
  46  if ~strcmp(codertarget.data.getParameterValue(cs, 'Runtime.BuildAction'), 'Build')
  47      error('spot:experiment:build', ['Could not set the build action to "Build". Set it in the diagram: ' ...
  48          'Hardware Implementation > Target hardware resources > Build options.']);
  49  end
  50  assignin('base', 'simMode', 0);
  51  
  52  % The build's output, as it happens, for the console
  53  cache = fullfile(root, '.spot_cache');
  54  if ~isfolder(cache)
  55      mkdir(cache);
  56  end
  57  logFile = fullfile(cache, 'build.log');
  58  if isfile(logFile)
  59      delete(logFile);
  60  end
  61  diary(logFile);
  62  stopDiary = onCleanup(@() diary('off'));
  63  fprintf('### SPOT: building %s on %s@%s (build action: Build)\n', model, b.user, b.address);
  64  
  65  tic;
  66  slbuild(model);
  67  seconds = toc;
  68  diary('off');
  69  log = '';
  70  if isfile(logFile)
  71      log = strtrim(regexprep(fileread(logFile), '<a [^>]*>|</a>', ''));
  72  end
  73  out = struct('model', model, 'program', p.program, 'seconds', seconds, 'log', log, 'platforms', {p.platforms});
  74  end
  75  
  76  function a = buildAction(cs)
  77  % The diagram's build action ('' if its board isn't a Jetson target, so there is none)
  78  try
  79      a = codertarget.data.getParameterValue(cs, 'Runtime.BuildAction');
  80  catch
  81      a = '';
  82  end
  83  end
  84  
  85  function undo(model, cs, plant, old, wasDirty)
  86  assignin('base', 'simMode', 1);
  87  set_param(plant, 'Commented', old.commented);
  88  set_param(model, 'SolverType', old.solverType, 'SolverName', old.solver);
  89  set_param(cs, 'HardwareBoard', old.board);
  90  if ~isempty(old.action) && ~isempty(buildAction(cs))
  91      codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', old.action);
  92  end
  93  if ~wasDirty
  94      set_param(model, 'Dirty', 'off');   % back as it was on disk
  95  end
  96  end
```

</details>
<!-- /listing -->
