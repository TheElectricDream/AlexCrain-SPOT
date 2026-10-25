function s = updateSettings(old, changes, root)
    %UPDATESETTINGS New settings from the console: the defaults, overlaid with what the page sent.
    %   s = interface.worker.updateSettings(old, changes, root)
    %
    %   Inputs
    %     old      the current settings
    %     changes  a whole settings struct, as the page sends it
    %     root     project folder
    %
    %   Outputs
    %     s        the defaults with changes merged on (spot.params.merge); when the diagram
    %              changed, its own solver settings (spot.sim.readSolver)
    %
    %   See also spot.params.merge, spot.params.defaults.

    s = spot.params.merge(spot.params.defaults(), changes);

    % A new diagram brings its own solver settings.
    if ~strcmp(old.diagram, s.diagram) && ~isempty(s.diagram)
        s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
    end
end
