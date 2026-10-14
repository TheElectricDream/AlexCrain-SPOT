function folder = saveSimulation(s, root)
%SAVESIMULATION Keep the last simulation in "Saved Data", next to the experiments.
%   folder = spot.data.saveSimulation(settings, projectFolder) saves dataClass (from the base
%   workspace) as Saved Data/Simulation_<diagram>_<yyyyMMdd_HHmmss>/simulation.mat, with the
%   settings it ran with as spot_settings.json beside it. The Results page lists it.
try
    dataClass = evalin('base', 'dataClass');
catch
    error('spot:data:saveSimulation', 'There is no simulation to save: run one first (dataClass in the workspace).');
end
name = sprintf('Simulation_%s_%s', spot.sim.modelName(s.diagram), char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
folder = fullfile(root, 'Saved Data', name);
mkdir(folder);
save(fullfile(folder, 'simulation.mat'), 'dataClass');
spot.params.save(s, fullfile(folder, 'spot_settings.json'));
end
