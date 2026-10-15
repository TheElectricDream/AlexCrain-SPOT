# spot.data.saveSimulation

**File:** `+spot/+data/saveSimulation.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/saveSimulation.m -->

## What it is for

*Save Results* on the Simulation tab: keep the last simulation's `dataClass` in `Saved Data/Simulation_<diagram>_<yyyyMMdd_HHmmss>/simulation.mat`, with the settings it ran with beside it as `spot_settings.json`, so the Results tab (and [[listSaved|spot-data-listSaved]]) can find it later with its phase times and arm geometry.

## How to call it

```matlab
folder = spot.data.saveSimulation(s, projectFolder);
```

## Line by line

**Lines 6–10.** Fetch `dataClass` from the base workspace; no simulation yet is an error.

**Lines 11–13.** The folder name from the diagram's model name and the current time; make it.

**Lines 14–15.** Save the variable, and the settings next to it.

<!-- xref -->
- **Used by:** `+spot/+data/load.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.params.save|spot-params-save]], [[spot.sim.modelName|spot-sim-modelName]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/saveSimulation.m</b> (16 lines; click to show)</summary>

```matlab
   1  function folder = saveSimulation(s, root)
   2  %SAVESIMULATION Keep the last simulation in "Saved Data", next to the experiments.
   3  %   folder = spot.data.saveSimulation(settings, projectFolder) saves dataClass (from the base
   4  %   workspace) as Saved Data/Simulation_<diagram>_<yyyyMMdd_HHmmss>/simulation.mat, with the
   5  %   settings it ran with as spot_settings.json beside it. The Results page lists it.
   6  try
   7      dataClass = evalin('base', 'dataClass');
   8  catch
   9      error('spot:data:saveSimulation', 'There is no simulation to save: run one first (dataClass in the workspace).');
  10  end
  11  name = sprintf('Simulation_%s_%s', spot.sim.modelName(s.diagram), char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
  12  folder = fullfile(root, 'Saved Data', name);
  13  mkdir(folder);
  14  save(fullfile(folder, 'simulation.mat'), 'dataClass');
  15  spot.params.save(s, fullfile(folder, 'spot_settings.json'));
  16  end
```

</details>
<!-- /listing -->
