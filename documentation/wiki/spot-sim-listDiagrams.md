# spot.sim.listDiagrams

**File:** `+spot/+sim/listDiagrams.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/listDiagrams.m -->

## What it is for

The Diagram drop-down of the Scenario pane: the `.slx` files at the top of the project folder.

## How to call it

```matlab
names = spot.sim.listDiagrams(projectFolder)    % {'Template_v5_0_0_2026a_Jetson_PSClock.slx'}
```

## Line by line

**Line 4.** `dir` with a wildcard lists the `.slx` files (not in subfolders).

**Line 5.** Their names, sorted.

<!-- xref -->
- **Used by:** `+spot/+control/verifyConsoleController.m`, `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/listDiagrams.m</b> (6 lines; click to show)</summary>

```matlab
   1  function names = listDiagrams(root)
   2  %LISTDIAGRAMS Simulink diagrams (.slx) in the project folder.
   3  %   names = spot.sim.listDiagrams(projectFolder) returns a cell array of file names.
   4  files = dir(fullfile(root, '*.slx'));
   5  names = sort({files.name});
   6  end
```

</details>
<!-- /listing -->
