# spot.sim.loadDiagram

**File:** `+spot/+sim/loadDiagram.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/loadDiagram.m -->

## What it is for

Make sure a diagram is loaded in Simulink (in memory, no window) and return its model name. Every function that touches the diagram starts with this.

## How to call it

```matlab
model = spot.sim.loadDiagram('Template.slx', projectFolder)    % 'Template'
```

## Line by line

**Line 4.** The model name from the file name ([[modelName|spot-sim-modelName]]).

**Lines 5–11.** If a model of that name isn't loaded yet: the file must exist in the project folder (a clear error otherwise), then `load_system` loads it without opening it. If it is already loaded (e.g. you have it open in the editor), that one is used.

<!-- xref -->
- **Used by:** `+spot/+control/verifyConsoleController.m`, `+spot/+experiment/decodeData.m`, `+spot/+experiment/plan.m`, `+spot/+sim/readSolver.m`, `+spot/+sim/run.m`, `+spot/+sim/showBlock.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.sim.modelName|spot-sim-modelName]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/loadDiagram.m</b> (12 lines; click to show)</summary>

```matlab
   1  function model = loadDiagram(diagram, root)
   2  %LOADDIAGRAM Load a diagram from the project folder (without opening its window).
   3  %   model = spot.sim.loadDiagram('Template.slx', projectFolder)
   4  model = spot.sim.modelName(diagram);
   5  if ~bdIsLoaded(model)
   6      file = fullfile(root, diagram);
   7      if ~isfile(file)
   8          error('spot:sim:loadDiagram', 'There is no %s in %s.', diagram, root);
   9      end
  10      load_system(file);
  11  end
  12  end
```

</details>
<!-- /listing -->
