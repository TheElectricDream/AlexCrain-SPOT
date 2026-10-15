# spot.sim.modelName

**File:** `+spot/+sim/modelName.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/modelName.m -->

## What it is for

Simulink refers to a loaded diagram by its model name, the file name without `.slx`.

## How to call it

```matlab
name = spot.sim.modelName('Template.slx')    % 'Template'
```

## Line by line

**Line 3.** `fileparts` splits the path; the middle output is the name without extension.

**Lines 4–6.** An empty name means no diagram was selected: say so.

<!-- xref -->
- **Used by:** `+spot/+data/saveSimulation.m`, `+spot/+sim/loadDiagram.m`, `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/modelName.m</b> (7 lines; click to show)</summary>

```matlab
   1  function name = modelName(diagram)
   2  %MODELNAME Simulink model name of a diagram file ('Template.slx' -> 'Template').
   3  [~, name] = fileparts(diagram);
   4  if isempty(name)
   5      error('spot:sim:modelName', 'Select a Simulink diagram first.');
   6  end
   7  end
```

</details>
<!-- /listing -->
