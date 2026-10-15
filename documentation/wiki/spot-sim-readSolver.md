# spot.sim.readSolver

**File:** `+spot/+sim/readSolver.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/readSolver.m -->

## What it is for

A diagram carries its own solver configuration. When you select a diagram (or reset the settings), the console copies that configuration into the settings' `solver` section so the Solver pane starts from what the diagram was saved with.

## How to call it

```matlab
s.solver = spot.sim.readSolver(s.diagram, projectFolder, s.solver);
```

## Line by line

**Line 4.** Load the diagram.

**Lines 5–11.** The solver type (`Fixed-step` / `Variable-step`) and the solver's name, stored in the field for that type (so the other type's choice is remembered).

**Lines 12–16.** The variable-step parameters: maximum, minimum and initial step, relative and absolute tolerance, as text (`'auto'` is a valid value).

<!-- xref -->
- **Used by:** `+spot/+params/reset.m`, `+spot/+params/update.m`
- **Uses:** [[spot.sim.loadDiagram|spot-sim-loadDiagram]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/readSolver.m</b> (17 lines; click to show)</summary>

```matlab
   1  function solver = readSolver(diagram, root, solver)
   2  %READSOLVER Copy a diagram's own solver settings into the settings' solver section.
   3  %   s.solver = spot.sim.readSolver(s.diagram, projectFolder, s.solver)
   4  model = spot.sim.loadDiagram(diagram, root);
   5  solver.type = get_param(model, 'SolverType');
   6  name = get_param(model, 'SolverName');
   7  if strcmp(solver.type, 'Variable-step')
   8      solver.variableStepName = name;
   9  else
  10      solver.fixedStepName = name;
  11  end
  12  solver.maxStep = get_param(model, 'MaxStep');
  13  solver.minStep = get_param(model, 'MinStep');
  14  solver.initialStep = get_param(model, 'InitialStep');
  15  solver.relTol = get_param(model, 'RelTol');
  16  solver.absTol = get_param(model, 'AbsTol');
  17  end
```

</details>
<!-- /listing -->
