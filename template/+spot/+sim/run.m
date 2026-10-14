function out = run(s, root, resultFile)
%RUN Simulate the selected diagram with the given settings.
%   out = spot.sim.run(s, projectFolder) puts the settings into the base workspace
%   (spot.params.apply), simulates s.diagram, and leaves simOut, dataClass and dataPacket in the
%   base workspace as the old GUI did. The diagram file is not modified: the simulation mode,
%   hardware board and solver are passed as temporary overrides.
%   out = spot.sim.run(s, projectFolder, resultFile) also writes the trajectory for the SPOT
%   Console's 3D view to resultFile (JSON, see spot.sim.results).
%
%   What Simulink prints while simulating (its warnings and diagnostics) is captured instead of
%   filling the Command Window: out.messages has the text and out.warnings how many warnings.
%   The result file includes the text too.
%
%   When Simulink stops with an error, run throws a spot.sim.SimulationFailed: its message is
%   the root cause (and the block it is in), and spot.sim.errorDetails(err) gives the whole
%   chain of causes, the blocks, and what Simulink printed before failing.
%
%   out has fields seconds (wall-clock), tsim, samples, warnings, messages, debugMessages (how
%   many debug messages the platforms showed, see spot.sim.debugLog), streams (the Live Stream
%   blocks' names, see spot.sim.liveData) and, with resultFile, file.
%   With s.realTime true the simulation runs at the clock's pace (Simulink's simulation pacing),
%   e.g. to watch the Live Plot or the console as an experiment would show them.
%   The result file also has the debug messages (debug: RED/BLACK/BLUE with t and text).
model = spot.sim.loadDiagram(s.diagram, root);
d = spot.params.apply(s, root);
% The simulated servos are set up as the real ones: their limits from the diagram's arm blocks
% (the arm's plant integrates at its own 1 ms step, whatever the sample rate)
if evalin('base', 'exist(''armModel'', ''var'')')
    armModel = evalin('base', 'armModel');
    armModel.servo = spot.arm.servoFromDiagram(model, armModel.servo);
    assignin('base', 'armModel', armModel);
end

% simMode tells the diagram it is being simulated; WhoAmI = 0 is the simulation's platform id
assignin('base', 'simMode', 1);
assignin('base', 'WhoAmI', 0);
restore = onCleanup(@() assignin('base', 'simMode', 0));

in = Simulink.SimulationInput(model);
in = in.setModelParameter('SimulationMode', 'normal', 'HardwareBoard', 'None');
if isfield(s, 'realTime') && s.realTime
    % Real time: Simulink paces the simulation to the clock (1 s simulated per second)
    in = in.setModelParameter('EnablePacing', 'on', 'PacingRate', '1');
end
sv = s.solver;
if strcmp(sv.type, 'Variable-step')
    in = in.setModelParameter('SolverType', 'Variable-step', 'Solver', sv.variableStepName, ...
        'MaxStep', sv.maxStep, 'MinStep', sv.minStep, 'InitialStep', sv.initialStep, ...
        'RelTol', sv.relTol, 'AbsTol', sv.absTol); %#ok<NASGU> (used in the evalc below)
else
    in = in.setModelParameter('SolverType', 'Fixed-step', 'Solver', sv.fixedStepName); %#ok<NASGU>
end

% Capture Simulink's output, without the long call stacks it adds to every warning
backtrace = warning('query', 'backtrace');
warning('off', 'backtrace');
restoreBacktrace = onCleanup(@() warning(backtrace));
spot.sim.debugLog('reset');   % the platforms' debug messages (spot.hardware.debugMessages)
spot.sim.liveData('reset');   % the Live Stream blocks' signals (spot.hardware.liveStream)
tic;
% Errors are caught inside the capture, so what Simulink printed before failing is kept with them
simOut = [];
simErr = [];
messages = evalc('try, simOut = sim(in); catch simErr, end');
messages = cleanMessages(messages);
if isempty(simErr) && ~isempty(simOut.ErrorMessage)
    % An error Simulink recorded instead of throwing: its diagnostic has the causes and blocks
    simErr = MException('spot:sim:run', '%s', simOut.ErrorMessage);
    try
        simErr = simOut.SimulationMetadata.ExecutionInfo.ErrorDiagnostic.Diagnostic;
    catch
    end
end
if ~isempty(simErr)
    throw(spot.sim.SimulationFailed(simErr, messages));
end
% Arm goals the controllers ignore and torques the servos can't give (beyond their limits): say so
if s.platforms.ARM && evalin('base', 'exist(''armModel'', ''var'')')
    for msg = spot.arm.checkCommands(simOut.dataPacket, evalin('base', 'armModel'))
        if ~isempty(messages) && messages(end) ~= newline
            messages(end + 1) = newline; %#ok<AGROW>
        end
        messages = sprintf('%sWarning: %s\n', messages, msg{1});
    end
end
out = struct('seconds', toc, 'tsim', d.tsim, 'warnings', numel(regexp(messages, '(^|\n)Warning: ', 'match')), ...
             'messages', messages);
assignin('base', 'simOut', simOut);

dataClass = simOut.dataPacket;
assignin('base', 'dataClass', dataClass);
try
    assignin('base', 'dataPacket', packAll(dataClass));
catch
    % signals of different lengths can't be put side by side; dataClass has everything anyway
end

r = spot.sim.results(dataClass, s, d);
r.messages = messages;
r.debug = selectedDebug(spot.sim.debugLog('get'), s.platforms);
out.debugMessages = sum(structfun(@(p) numel(p.t), r.debug));
r.live = thinned(spot.sim.liveData('get'), 4000);
out.streams = {r.live.name};
out.samples = r.samples;
if nargin > 2
    folder = fileparts(resultFile);
    if ~isfolder(folder)
        mkdir(folder);
    end
    fid = fopen(resultFile, 'w');
    if fid < 0
        error('spot:sim:run', 'Cannot write %s.', resultFile);
    end
    fwrite(fid, jsonencode(r), 'char');
    fclose(fid);
    out.file = resultFile;
end
end

function live = thinned(live, maxSamples)
% The streams for the console's Live Plot after the run: at most maxSamples samples each (the
% full record stays available as spot.sim.liveData('get'))
for k = 1:numel(live)
    n = numel(live(k).t);
    if n > maxSamples
        keep = unique(round(linspace(1, n, maxSamples)));
        live(k).t = live(k).t(keep);
        live(k).v = live(k).v(keep, :);
    end
end
end

function log = selectedDebug(log, platforms)
% The platforms' debug messages, only for the platforms taking part (the diagram writes all three)
for name = {'RED', 'BLACK', 'BLUE'}
    if ~platforms.(name{1})
        log.(name{1}) = struct('t', zeros(0, 1), 'text', {cell(0, 1)});
    end
end
end

function packet = packAll(dataClass)
% All logged signals side by side, as the old GUI's dataPacket variable
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

function text = cleanMessages(text)
% Simulink's captured output without its hyperlink markup, the hidden "[<backspace>" and
% "]<backspace>" MATLAB wraps warnings in, and blank-line padding
text = regexprep(text, '<a [^>]*>|</a>', '');
text = regexprep(text, ['[\[\]]?' char(8)], '');
text = regexprep(text, '\n\s*\n(\s*\n)+', '\n\n');
text = strtrim(text);
if strlength(text) > 200000
    text = [text(1:200000) newline '... (shortened)'];
end
end
