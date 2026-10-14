function [s, backup] = reset(current, root, settingsFile, backupFolder)
%RESET Back to the template's default settings, keeping the diagram (a clean slate).
%   [s, backup] = spot.params.reset(current, projectFolder, settingsFile, backupFolder) returns
%   spot.params.defaults() with the diagram of CURRENT still selected and that diagram's own solver
%   settings. The settings file as it was is copied first to
%   <backupFolder>/settings_before_reset_<yyyyMMdd_HHmmss>.json, so it can be loaded back; BACKUP is
%   that path ('' when there was no file yet).
backup = '';
if isfile(settingsFile)
    if ~isfolder(backupFolder)
        mkdir(backupFolder);
    end
    backup = fullfile(backupFolder, sprintf('settings_before_reset_%s.json', ...
        char(datetime('now', 'Format', 'yyyyMMdd_HHmmss'))));
    copyfile(settingsFile, backup);
end
s = spot.params.defaults();
s.diagram = current.diagram;
if ~isempty(s.diagram)
    s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
end
end
