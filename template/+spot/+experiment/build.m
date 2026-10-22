function out = build(s, root)
    %BUILD Build the selected diagram for the platforms, on the lab console (TX2).
    %   out = spot.experiment.build(s, root) puts the settings into the workspace and builds the
    %   diagram with slbuild on the TX2 (connect first with spot.experiment.connect). For the
    %   build only, it:
    %     - comments out "Simulate Plant Dynamics", when the diagram has it (a diagram that
    %       switches to its hardware itself with simMode, e.g. through a Variant Subsystem, has
    %       nothing to comment out)
    %     - sets the solver to Fixed-step ode4 and the board to NVIDIA Jetson
    %     - sets the build action to "Build", never "Build and run" (the program must not run on
    %       the TX2)
    %   Every change is undone afterwards and the diagram is NOT saved. The lab console then
    %   deploys the program to the platforms.
    %
    %   slbuild's output goes to <root>/.spot_cache/build.log as it happens (the console shows it
    %   live) as well as to the Command Window.
    %
    %   Inputs
    %     s     settings
    %     root  project folder
    %
    %   Outputs
    %     out   model, program (path on the TX2), seconds, log (what the build printed),
    %           platforms
    %
    %   See also spot.experiment.plan, spot.experiment.connect, spot.experiment.buildBoard.

    %% Check the plan
    planned = spot.experiment.plan(s, root);
    if ~planned.hasJetsonSupport
        error('spot:experiment:build', ...
            'The MATLAB Coder Support Package for NVIDIA Jetson is not installed.');
    end
    if isempty(planned.platforms)
        error('spot:experiment:build', 'Select at least one platform (Scenario) first.');
    end
    model = planned.model;
    board = planned.board;
    spot.params.apply(s, root);

    %% Build board
    % Build on the TX2, in this project's folder there.
    group = 'Hardware_Connectivity_Installer_NVIDIAJetson';
    setpref(group, 'DefaultIpAddress', board.address);
    setpref(group, 'DefaultUserName', board.user);
    setpref(group, 'DefaultBuildDirPref', board.buildDir);
    setpref(group, 'DefaulttargetwsdirPref', board.workspaceDir);

    %% Changes for the build, all undone afterwards
    % What to restore.
    wasDirty = strcmp(get_param(model, 'Dirty'), 'on');
    cs = getActiveConfigSet(model);
    plant = [model '/Simulate Plant Dynamics'];
    if getSimulinkBlockHandle(plant) == -1
        plant = '';     % none: the diagram switches to its hardware itself (simMode)
    end
    commented = '';
    if ~isempty(plant)
        commented = get_param(plant, 'Commented');
    end
    old = struct('commented', commented, ...
                 'solverType', get_param(model, 'SolverType'), ...
                 'solver', get_param(model, 'SolverName'), ...
                 'board', get_param(cs, 'HardwareBoard'), ...
                 'action', buildAction(cs));
    restore = onCleanup(@() undo(model, cs, plant, old, wasDirty));

    % No plant, fixed-step solver, Jetson board.
    if ~isempty(plant)
        set_param(plant, 'Commented', 'on');
    end
    set_param(model, 'SolverType', 'Fixed-step', 'SolverName', 'ode4');
    set_param(cs, 'HardwareBoard', 'NVIDIA Jetson');

    % Build only: with "Build and run" MATLAB would also start the program on the TX2 (as root).
    codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', 'Build');
    if ~strcmp(codertarget.data.getParameterValue(cs, 'Runtime.BuildAction'), 'Build')
        error('spot:experiment:build', ...
            ['Could not set the build action to "Build". Set it in the diagram: Hardware ' ...
             'Implementation > Target hardware resources > Build options.']);
    end
    assignin('base', 'simMode', 0);

    %% Build log, for the console
    cache = fullfile(root, '.spot_cache');
    if ~isfolder(cache)
        mkdir(cache);
    end
    logFile = fullfile(cache, 'build.log');
    if isfile(logFile)
        delete(logFile);
    end
    diary(logFile);
    stopDiary = onCleanup(@() diary('off'));
    fprintf('### SPOT: building %s on %s@%s (build action: Build)\n', model, board.user, ...
        board.address);

    %% Build
    tic;
    slbuild(model);
    seconds = toc;
    diary('off');

    % What it printed, without hyperlinks.
    buildLog = '';
    if isfile(logFile)
        buildLog = strtrim(regexprep(fileread(logFile), '<a [^>]*>|</a>', ''));
    end
    out = struct('model', model, 'program', planned.program, 'seconds', seconds, ...
                 'log', buildLog, 'platforms', {planned.platforms});
end

function action = buildAction(cs)
    % The diagram's build action ('' if its board isn't a Jetson target, so there is none).
    try
        action = codertarget.data.getParameterValue(cs, 'Runtime.BuildAction');
    catch
        action = '';
    end
end

function undo(model, cs, plant, old, wasDirty)
    % Undo the build's changes to the diagram.
    assignin('base', 'simMode', 1);
    if ~isempty(plant)
        set_param(plant, 'Commented', old.commented);
    end
    set_param(model, 'SolverType', old.solverType, 'SolverName', old.solver);
    set_param(cs, 'HardwareBoard', old.board);
    if ~isempty(old.action) && ~isempty(buildAction(cs))
        codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', old.action);
    end

    % Back as it was on disk.
    if ~wasDirty
        set_param(model, 'Dirty', 'off');
    end
end
