# spot.experiment.plan

**File:** `+spot/+experiment/plan.m` · **Package:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+experiment/plan.m -->

## What it is for

The Experiment tab's *Check*: everything [[build|spot-experiment-build]] would do, said in advance without compiling anything, plus warnings about things that will go wrong (no platform selected, no plant block to comment out, continuous-time blocks, the Jetson support package missing).

## How to call it

```matlab
p = spot.experiment.plan(s, projectFolder);
p.program, p.platforms, p.changes, p.warnings, p.segments, p.tsim, p.hasJetsonSupport
```

## Line by line

**Lines 6–7.** Load the diagram; the build board ([[buildBoard|spot-experiment-buildBoard]]).

**Lines 9–15.** The model name, the board, the program's path on the TX2 (`<workspaceDir><model>.elf`), the selected platforms, and the list of changes the build makes.

**Lines 17–20.** The phase segments that will run and the total time (switched-off segments are built in as zero seconds, like in a simulation).

**Lines 21–23.** No platform → warning.

**Lines 24–29.** The block that must be commented out, `<model>/Simulate Plant Dynamics`, must exist.

**Lines 30–36.** Count continuous-time `Derivative` and `Integrator` blocks anywhere in the model: compiled code on the platforms runs at a fixed step and such blocks may not behave as expected.

**Lines 37–40.** Is `jetson` on the path (the support package installed)?

<!-- xref -->
- **Used by:** `+spot/+experiment/build.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.experiment.build|spot-experiment-build]], [[spot.experiment.buildBoard|spot-experiment-buildBoard]], [[spot.params.timing|spot-params-timing]], [[spot.sim.loadDiagram|spot-sim-loadDiagram]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+experiment/plan.m</b> (41 lines; click to show)</summary>

```matlab
   1  function p = plan(s, root)
   2  %PLAN What spot.experiment.build would do, without building anything.
   3  %   p = spot.experiment.plan(s, projectFolder) loads the diagram (no compiling) and returns the
   4  %   model, the build board and folders, the platforms, the changes made for the build (and undone
   5  %   afterwards), and warnings such as continuous-time blocks. Use it to check a setup.
   6  model = spot.sim.loadDiagram(s.diagram, root);
   7  b = spot.experiment.buildBoard(root, s);
   8  names = {'RED', 'BLACK', 'BLUE'};
   9  p = struct();
  10  p.model = model;
  11  p.board = b;
  12  p.program = [b.workspaceDir model '.elf'];
  13  p.platforms = names(cellfun(@(n) logical(s.platforms.(n)), names));
  14  p.changes = {sprintf('comment out %s/Simulate Plant Dynamics', model), ...
  15               'solver: Fixed-step ode4', 'hardware board: NVIDIA Jetson', 'simMode = 0 while building'};
  16  p.warnings = {};
  17  % Segments switched off on the Scenario tab are built in as zero seconds (see spot.params.timing)
  18  tm = spot.params.timing(s);
  19  p.segments = tm.phases.names;
  20  p.tsim = tm.tsim;
  21  if isempty(p.platforms)
  22      p.warnings{end + 1} = 'No platform is selected (Scenario): nothing would be deployed.';
  23  end
  24  plant = [model '/Simulate Plant Dynamics'];
  25  try
  26      get_param(plant, 'Handle');
  27  catch
  28      p.warnings{end + 1} = sprintf('The diagram has no "%s" block to comment out.', plant);
  29  end
  30  blocks = find_system(model, 'FindAll', 'on', 'Type', 'Block');
  31  types = string(get_param(blocks, 'BlockType'));
  32  nd = nnz(types == "Derivative"); ni = nnz(types == "Integrator");
  33  if nd || ni
  34      p.warnings{end + 1} = sprintf(['%d continuous-time derivative and %d integrator block(s): compiled code ' ...
  35          'may not behave as expected. Use discrete-time blocks.'], nd, ni);
  36  end
  37  p.hasJetsonSupport = ~isempty(which('jetson'));
  38  if ~p.hasJetsonSupport
  39      p.warnings{end + 1} = 'The MATLAB Coder Support Package for NVIDIA Jetson is not installed.';
  40  end
  41  end
```

</details>
<!-- /listing -->
