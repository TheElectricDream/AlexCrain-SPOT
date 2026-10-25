function folder = saveSimulation(s, root)
    %SAVESIMULATION Keep the last simulation in "Saved Data", next to the experiments.
    %   folder = spot.data.saveSimulation(settings, root) saves dataClass (from the base
    %   workspace) as Saved Data/Simulation_<diagram>_<yyyyMMdd_HHmmss>/simulation.mat, with the
    %   settings it ran with as spot_settings.json beside it. The Results page lists it.
    %
    %   Inputs
    %     s       settings the simulation ran with
    %     root    project folder
    %
    %   Outputs
    %     folder  the new folder
    %
    %   See also interface.results.listSaved, spot.data.load.

    % The last simulation's log.
    try
        dataClass = evalin('base', 'dataClass');
    catch
        error('spot:data:saveSimulation', ...
            'There is no simulation to save: run one first (dataClass in the workspace).');
    end

    % A new folder named after the diagram and the time.
    name = sprintf('Simulation_%s_%s', spot.sim.modelName(s.diagram), ...
        char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
    folder = fullfile(root, 'Saved Data', name);
    mkdir(folder);

    % The log and its settings.
    save(fullfile(folder, 'simulation.mat'), 'dataClass');
    spot.params.save(s, fullfile(folder, 'spot_settings.json'));
end
