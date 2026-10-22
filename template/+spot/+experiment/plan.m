function p = plan(s, root)
    %PLAN What spot.experiment.build would do, without building anything.
    %   p = spot.experiment.plan(s, root) loads the diagram (no compiling). Use it to check a
    %   setup.
    %
    %   Inputs
    %     s     settings
    %     root  project folder
    %
    %   Outputs
    %     p     model, board (spot.experiment.buildBoard), program (path on the TX2), platforms,
    %           changes (made for the build and undone afterwards), segments and tsim (the
    %           timeline), warnings (e.g. continuous-time blocks), hasJetsonSupport
    %
    %   See also spot.experiment.build.

    %% Build
    model = spot.sim.loadDiagram(s.diagram, root);
    board = spot.experiment.buildBoard(root, s);
    names = {'RED', 'BLACK', 'BLUE'};
    p = struct();
    p.model = model;
    p.board = board;
    p.program = [board.workspaceDir model '.elf'];
    p.platforms = names(cellfun(@(n) logical(s.platforms.(n)), names));
    p.changes = {'solver: Fixed-step ode4', 'hardware board: NVIDIA Jetson', ...
                 'simMode = 0 while building'};
    p.warnings = {};

    % Timeline; segments switched off on the Scenario tab are built in as zero seconds.
    times = spot.params.timing(s);
    p.segments = times.phases.names;
    p.tsim = times.tsim;

    %% Warnings
    if isempty(p.platforms)
        p.warnings{end + 1} = 'No platform is selected (Scenario): nothing would be deployed.';
    end

    % The plant block the build comments out. A diagram without one should switch to its
    % hardware itself with simMode, e.g. through a Variant Subsystem.
    plant = [model '/Simulate Plant Dynamics'];
    if getSimulinkBlockHandle(plant) ~= -1
        p.changes = [{sprintf('comment out %s', plant)}, p.changes];
    elseif isempty(find_system(model, 'LookUnderMasks', 'all', 'Variant', 'on'))
        p.warnings{end + 1} = sprintf( ...
            ['The diagram has no "%s" block to comment out and no Variant Subsystem: the ' ...
             'simulated plant would be built into the program.'], plant);
    end

    % Continuous-time blocks.
    blocks = find_system(model, 'FindAll', 'on', 'Type', 'Block');
    types = string(get_param(blocks, 'BlockType'));
    nDerivative = nnz(types == "Derivative");
    nIntegrator = nnz(types == "Integrator");
    if nDerivative || nIntegrator
        p.warnings{end + 1} = sprintf( ...
            ['%d continuous-time derivative and %d integrator block(s): compiled code may not ' ...
             'behave as expected. Use discrete-time blocks.'], nDerivative, nIntegrator);
    end

    % The Jetson support package.
    p.hasJetsonSupport = ~isempty(which('jetson'));
    if ~p.hasJetsonSupport
        p.warnings{end + 1} = ['The MATLAB Coder Support Package for NVIDIA Jetson is not ' ...
                               'installed.'];
    end
end
