# spot.sim.showBlock

**File:** `+spot/+sim/showBlock.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/showBlock.m -->

## What it is for

The *Show Block* button after a failed simulation: open the diagram at the subsystem that holds the block and highlight the block in red.

## How to call it

```matlab
spot.sim.showBlock('Template.slx', projectFolder, 'Template/4 - Control/Default LQR Control (RED)/Matrix Multiply')
```

## Line by line

**Lines 6–7.** Load the diagram (if it isn't already) and make sure the path is a char array.

**Lines 8–10.** The path must start with the model's name and a slash, i.e. be inside this diagram.

**Lines 11–15.** Ask Simulink for the block's parent; if that fails, the block no longer exists (the diagram was edited since the error).

**Lines 16–17.** Open the parent subsystem's window and highlight the block with the `error` style (red).

<!-- xref -->
- **Used by:** `+spot/+sim/errorDetails.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.sim.errorDetails|spot-sim-errorDetails]], [[spot.sim.loadDiagram|spot-sim-loadDiagram]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/showBlock.m</b> (18 lines; click to show)</summary>

```matlab
   1  function block = showBlock(diagram, root, block)
   2  %SHOWBLOCK Open a diagram at a block and highlight it (e.g. the block an error is in).
   3  %   spot.sim.showBlock(diagramFile, projectFolder, blockPath) loads the diagram from the project
   4  %   folder, opens the subsystem holding the block and highlights the block in red. blockPath is
   5  %   a full path, as in spot.sim.errorDetails(err).blocks; it must be in that diagram.
   6  model = spot.sim.loadDiagram(diagram, root);
   7  block = char(block);
   8  if ~startsWith(block, [model '/'])
   9      error('spot:sim:showBlock', 'The block %s is not in %s.', block, model);
  10  end
  11  try
  12      parent = get_param(block, 'Parent');
  13  catch
  14      error('spot:sim:showBlock', 'The block %s no longer exists in %s.', block, model);
  15  end
  16  open_system(parent);
  17  hilite_system(block, 'error');
  18  end
```

</details>
<!-- /listing -->
