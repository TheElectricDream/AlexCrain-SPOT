# spot.params.update

**File:** `+spot/+params/update.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/update.m -->

## What it is for

What the MATLAB worker does when the page sends new settings (the `settings.set` command): rebuild the struct from the defaults plus what was sent, and, when the diagram changed, take the solver settings from the new diagram.

## How to call it

```matlab
s = spot.params.update(old, changes, projectFolder);
```

`old` is the current settings, `changes` the whole struct the page sent.

## Line by line

**Line 6.** Merge the page's struct onto a fresh set of defaults (not onto `old`): the page always sends everything it knows, so building from the defaults guarantees that nothing stale survives and that new fields appear with their defaults.

**Lines 7–9.** If the diagram name differs from before and isn't empty, read the solver type, name, steps and tolerances from the diagram itself ([[spot.sim.readSolver|spot-sim-readSolver]]) into `s.solver`, so the Solver pane shows what the diagram was saved with.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.params.defaults|spot-params-defaults]], [[spot.params.merge|spot-params-merge]], [[spot.sim.readSolver|spot-sim-readSolver]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/update.m</b> (10 lines; click to show)</summary>

```matlab
   1  function s = update(old, changes, root)
   2  %UPDATE New settings from the console: the defaults, overlaid with what the page sent.
   3  %   s = spot.params.update(old, changes, projectFolder) merges CHANGES (a whole settings struct, as
   4  %   the page sends it) onto the defaults (spot.params.merge). When the diagram changed, its own
   5  %   solver settings are read from it (spot.sim.readSolver).
   6  s = spot.params.merge(spot.params.defaults(), changes);
   7  if ~strcmp(old.diagram, s.diagram) && ~isempty(s.diagram)
   8      s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
   9  end
  10  end
```

</details>
<!-- /listing -->
