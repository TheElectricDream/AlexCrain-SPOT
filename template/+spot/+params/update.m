function s = update(old, changes, root)
%UPDATE New settings from the console: the defaults, overlaid with what the page sent.
%   s = spot.params.update(old, changes, projectFolder) merges CHANGES (a whole settings struct, as
%   the page sends it) onto the defaults (spot.params.merge). When the diagram changed, its own
%   solver settings are read from it (spot.sim.readSolver).
s = spot.params.merge(spot.params.defaults(), changes);
if ~strcmp(old.diagram, s.diagram) && ~isempty(s.diagram)
    s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
end
end
