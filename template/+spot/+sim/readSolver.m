function solver = readSolver(diagram, root, solver)
%READSOLVER Copy a diagram's own solver settings into the settings' solver section.
%   s.solver = spot.sim.readSolver(s.diagram, projectFolder, s.solver)
model = spot.sim.loadDiagram(diagram, root);
solver.type = get_param(model, 'SolverType');
name = get_param(model, 'SolverName');
if strcmp(solver.type, 'Variable-step')
    solver.variableStepName = name;
else
    solver.fixedStepName = name;
end
solver.maxStep = get_param(model, 'MaxStep');
solver.minStep = get_param(model, 'MinStep');
solver.initialStep = get_param(model, 'InitialStep');
solver.relTol = get_param(model, 'RelTol');
solver.absTol = get_param(model, 'AbsTol');
end
