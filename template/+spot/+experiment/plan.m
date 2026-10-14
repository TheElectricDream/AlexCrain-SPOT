function p = plan(s, root)
%PLAN What spot.experiment.build would do, without building anything.
%   p = spot.experiment.plan(s, projectFolder) loads the diagram (no compiling) and returns the
%   model, the build board and folders, the platforms, the changes made for the build (and undone
%   afterwards), and warnings such as continuous-time blocks. Use it to check a setup.
model = spot.sim.loadDiagram(s.diagram, root);
b = spot.experiment.buildBoard(root, s);
names = {'RED', 'BLACK', 'BLUE'};
p = struct();
p.model = model;
p.board = b;
p.program = [b.workspaceDir model '.elf'];
p.platforms = names(cellfun(@(n) logical(s.platforms.(n)), names));
p.changes = {sprintf('comment out %s/Simulate Plant Dynamics', model), ...
             'solver: Fixed-step ode4', 'hardware board: NVIDIA Jetson', 'simMode = 0 while building'};
p.warnings = {};
% Segments switched off on the Scenario tab are built in as zero seconds (see spot.params.timing)
tm = spot.params.timing(s);
p.segments = tm.phases.names;
p.tsim = tm.tsim;
if isempty(p.platforms)
    p.warnings{end + 1} = 'No platform is selected (Scenario): nothing would be deployed.';
end
plant = [model '/Simulate Plant Dynamics'];
try
    get_param(plant, 'Handle');
catch
    p.warnings{end + 1} = sprintf('The diagram has no "%s" block to comment out.', plant);
end
blocks = find_system(model, 'FindAll', 'on', 'Type', 'Block');
types = string(get_param(blocks, 'BlockType'));
nd = nnz(types == "Derivative"); ni = nnz(types == "Integrator");
if nd || ni
    p.warnings{end + 1} = sprintf(['%d continuous-time derivative and %d integrator block(s): compiled code ' ...
        'may not behave as expected. Use discrete-time blocks.'], nd, ni);
end
p.hasJetsonSupport = ~isempty(which('jetson'));
if ~p.hasJetsonSupport
    p.warnings{end + 1} = 'The MATLAB Coder Support Package for NVIDIA Jetson is not installed.';
end
end
