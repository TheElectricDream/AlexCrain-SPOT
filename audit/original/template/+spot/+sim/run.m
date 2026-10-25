function out = run(s, root, resultFile)
    %RUN Simulate the selected diagram with the given settings.
    %   out = spot.sim.run(s, root) puts the settings into the base workspace
    %   (spot.params.apply), simulates s.diagram, and leaves simOut, dataClass and dataPacket in
    %   the base workspace as the old GUI did. The diagram file is not modified: the simulation
    %   mode, hardware board and solver are passed as temporary overrides.
    %   out = spot.sim.run(s, root, resultFile) also writes the trajectory for the console's 3D
    %   view to resultFile (JSON, interface.results.simulation), with the captured messages, the
    %   debug messages (debug: RED/BLACK/BLUE with t and text) and the Live Stream signals.
    %
    %   What Simulink prints while simulating is captured instead of filling the Command Window.
    %   When Simulink stops with an error, run throws a spot.sim.SimulationFailed: its message
    %   is the root cause (and its block), and interface.worker.errorDetails(err) gives the whole
    %   chain of causes, the blocks, and what Simulink printed before failing.
    %
    %   With s.realTime the simulation runs at the clock's pace, e.g. to watch the Live Plot.
    %
    %   Inputs
    %     s           settings
    %     root        project folder
    %     resultFile  JSON file for the console (optional)
    %
    %   Outputs
    %     out         seconds (wall-clock), tsim, samples, warnings (count), messages (text),
    %                 debugMessages (count, spot.sim.debugLog), streams (Live Stream names,
    %                 spot.sim.liveData) and, with resultFile, file
    %
    %   See also spot.params.apply, interface.results.simulation, interface.worker.errorDetails.

    %% Constants
    LIVE_PLOT_SAMPLES = 4000;   % per stream in the result file

    %% Settings into the workspace
    model = spot.sim.loadDiagram(s.diagram, root);
    applied = spot.params.apply(s, root);

    % The simulated servos get the real ones' limits from the diagram's arm blocks.
    if evalin('base', 'exist(''armModel'', ''var'')')
        armModel = evalin('base', 'armModel');
        armModel.servo = spot.arm.servoFromDiagram(model, armModel.servo);
        assignin('base', 'armModel', armModel);
    end

    % simMode tells the diagram it is being simulated; WhoAmI = 0 is the simulation's id.
    assignin('base', 'simMode', 1);
    assignin('base', 'WhoAmI', 0);
    restore = onCleanup(@() assignin('base', 'simMode', 0));

    %% Simulation input: temporary overrides
    in = Simulink.SimulationInput(model);
    in = in.setModelParameter('SimulationMode', 'normal', 'HardwareBoard', 'None');

    % Real time: 1 s simulated per second.
    if isfield(s, 'realTime') && s.realTime
        in = in.setModelParameter('EnablePacing', 'on', 'PacingRate', '1');
    end

    % The settings' solver (in is used inside the evalc below).
    solver = s.solver;
    if strcmp(solver.type, 'Variable-step')
        in = in.setModelParameter('SolverType', 'Variable-step', ...
            'Solver', solver.variableStepName, 'MaxStep', solver.maxStep, ...
            'MinStep', solver.minStep, 'InitialStep', solver.initialStep, ...
            'RelTol', solver.relTol, 'AbsTol', solver.absTol); %#ok<NASGU>
    else
        in = in.setModelParameter('SolverType', 'Fixed-step', ...
            'Solver', solver.fixedStepName); %#ok<NASGU>
    end

    %% Simulate, capturing what Simulink prints
    % Without the long call stacks Simulink adds to every warning.
    backtrace = warning('query', 'backtrace');
    warning('off', 'backtrace');
    restoreBacktrace = onCleanup(@() warning(backtrace));

    % Fresh records of the debug messages and Live Stream signals.
    spot.sim.debugLog('reset');
    spot.sim.liveData('reset');

    % Errors are caught inside the capture, so what Simulink printed before failing is kept.
    tic;
    simOut = [];
    simErr = [];
    messages = evalc('try, simOut = sim(in); catch simErr, end');
    messages = cleanMessages(messages);

    % An error Simulink recorded instead of throwing: its diagnostic has the causes and blocks.
    if isempty(simErr) && ~isempty(simOut.ErrorMessage)
        simErr = MException('spot:sim:run', '%s', simOut.ErrorMessage);
        try
            simErr = simOut.SimulationMetadata.ExecutionInfo.ErrorDiagnostic.Diagnostic;
        catch
        end
    end
    if ~isempty(simErr)
        throw(spot.sim.SimulationFailed(simErr, messages));
    end

    %% Arm commands the servos would ignore or clip, as warnings
    if s.platforms.ARM && evalin('base', 'exist(''armModel'', ''var'')')
        for msg = spot.arm.checkCommands(simOut.dataPacket, evalin('base', 'armModel'))
            if ~isempty(messages) && messages(end) ~= newline
                messages(end + 1) = newline; %#ok<AGROW>
            end
            messages = sprintf('%sWarning: %s\n', messages, msg{1});
        end
    end
    out = struct('seconds', toc, 'tsim', applied.tsim, ...
                 'warnings', numel(regexp(messages, '(^|\n)Warning: ', 'match')), ...
                 'messages', messages);

    %% Results into the workspace
    assignin('base', 'simOut', simOut);
    dataClass = simOut.dataPacket;
    assignin('base', 'dataClass', dataClass);
    try
        assignin('base', 'dataPacket', packAll(dataClass));
    catch
        % signals of different lengths can't be put side by side; dataClass has everything
    end

    %% Results for the console
    result = interface.results.simulation(dataClass, s, applied);
    result.messages = messages;
    result.debug = selectedDebug(spot.sim.debugLog('get'), s.platforms);
    out.debugMessages = sum(structfun(@(p) numel(p.t), result.debug));
    result.live = thinned(spot.sim.liveData('get'), LIVE_PLOT_SAMPLES);
    out.streams = {result.live.name};
    out.samples = result.samples;

    % Written to the result file, if asked for.
    if nargin > 2
        folder = fileparts(resultFile);
        if ~isfolder(folder)
            mkdir(folder);
        end
        fid = fopen(resultFile, 'w');
        if fid < 0
            error('spot:sim:run', 'Cannot write %s.', resultFile);
        end
        fwrite(fid, jsonencode(result), 'char');
        fclose(fid);
        out.file = resultFile;
    end
end

function live = thinned(live, maxSamples)
    % The streams for the Live Plot after the run: at most maxSamples samples each (the full
    % record stays available as spot.sim.liveData('get')).
    for k = 1:numel(live)
        n = numel(live(k).t);
        if n > maxSamples
            keep = unique(round(linspace(1, n, maxSamples)));
            live(k).t = live(k).t(keep);
            live(k).v = live(k).v(keep, :);
        end
    end
end

function debug = selectedDebug(debug, platforms)
    % Debug messages only for the platforms taking part (the diagram writes all three).
    for name = {'RED', 'BLACK', 'BLUE'}
        if ~platforms.(name{1})
            debug.(name{1}) = struct('t', zeros(0, 1), 'text', {cell(0, 1)});
        end
    end
end

function packet = packAll(dataClass)
    % All logged signals side by side, as the old GUI's dataPacket variable.
    packet = [];
    for f = fieldnames(dataClass).'
        x = dataClass.(f{1});
        if isa(x, 'timeseries')
            packet = [packet, x.Data]; %#ok<AGROW>
        elseif isstruct(x)
            for g = fieldnames(x).'
                if isa(x.(g{1}), 'timeseries')
                    packet = [packet, x.(g{1}).Data]; %#ok<AGROW>
                end
            end
        end
    end
end

function str = cleanMessages(str)
    % Simulink's captured output without its hyperlink markup, the hidden "[<backspace>" and
    % "]<backspace>" MATLAB wraps warnings in, and blank-line padding; shortened if huge.
    MAX_LENGTH = 200000;   % characters

    str = regexprep(str, '<a [^>]*>|</a>', '');
    str = regexprep(str, ['[\[\]]?' char(8)], '');
    str = regexprep(str, '\n\s*\n(\s*\n)+', '\n\n');
    str = strtrim(str);
    if strlength(str) > MAX_LENGTH
        str = [str(1:MAX_LENGTH) newline '... (shortened)'];
    end
end
