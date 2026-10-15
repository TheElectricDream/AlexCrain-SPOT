function [s, backup] = reset(current, root, settingsFile, backupFolder)
    %RESET Back to the template's default settings, keeping the diagram (a clean slate).
    %   [s, backup] = spot.params.reset(current, root, settingsFile, backupFolder)
    %
    %   Inputs
    %     current       the current settings
    %     root          project folder
    %     settingsFile  the settings file, backed up first
    %     backupFolder  where the backup goes
    %
    %   Outputs
    %     s             spot.params.defaults() with current's diagram still selected, and that
    %                   diagram's own solver settings
    %     backup        <backupFolder>/settings_before_reset_<yyyyMMdd_HHmmss>.json, so it can
    %                   be loaded back ('' when there was no file yet)
    %
    %   See also spot.params.defaults, spot.params.load.

    %% Back up the settings file
    backup = '';
    if isfile(settingsFile)
        if ~isfolder(backupFolder)
            mkdir(backupFolder);
        end
        backup = fullfile(backupFolder, sprintf('settings_before_reset_%s.json', ...
            char(datetime('now', 'Format', 'yyyyMMdd_HHmmss'))));
        copyfile(settingsFile, backup);
    end

    %% Defaults, with the same diagram and its solver
    s = spot.params.defaults();
    s.diagram = current.diagram;
    if ~isempty(s.diagram)
        s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
    end
end
