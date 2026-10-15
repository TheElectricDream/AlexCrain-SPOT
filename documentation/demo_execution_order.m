function results = demo_execution_order(figureFile)
%DEMO_EXECUTION_ORDER Why SPOT's diagram makes its top-level subsystems atomic and gives them priorities.
%   demo_execution_order builds a tiny diagram in memory (nothing is saved) that works like the
%   SPOT diagram: a "1 - Experiment Logic" subsystem writes the phase into a data store, and a
%   "3 - Control" subsystem reads it. Nothing else connects them, just as SPOT's top-level
%   subsystems only talk through data stores. The phase changes from 0 to 1 at t = 0.5 s; the
%   model runs every 0.1 s. It simulates three versions and prints, for each, which subsystem ran
%   first, Simulink's data store warnings, and when Control saw the new phase:
%     A  virtual subsystems (how they were): Simulink picks the order itself
%     B  atomic subsystems with priorities the wrong way round (Control 30, Logic 50)
%     C  atomic subsystems with priorities in the order of a step (Logic 30, Control 50)
%   In the right order Control sees the phase in the step it changes; in the wrong order one
%   step (0.1 s) later, and Simulink warns that a data store was read before it was written.
%
%   results = demo_execution_order returns the three cases as a struct array.
%   demo_execution_order(file) also saves a figure of what Control saw (e.g. 'demo.pdf').
%
%   See documentation/SPOT_Tutorial.pdf, "Inside the diagram: the order of a step".

model = 'spotExecutionOrderDemo';
if bdIsLoaded(model)
    close_system(model, 0);
end
new_system(model);
cleanup = onCleanup(@() close_system(model, 0));
set_param(model, 'SolverType', 'Fixed-step', 'Solver', 'FixedStepDiscrete', 'FixedStep', '0.1', ...
    'StopTime', '1', 'ReturnWorkspaceOutputs', 'on');

% The data store, with Simulink's checks on (as for most of SPOT's stores)
add_block('simulink/Signal Routing/Data Store Memory', [model '/Phase store'], 'DataStoreName', 'Phase', ...
    'InitialValue', '0', 'ReadBeforeWriteMsg', 'warning', 'WriteAfterReadMsg', 'warning', ...
    'Position', [40 30 120 70]);

% "3 - Control": reads the phase and logs what it saw (added first, like any block Simulink may
% happen to put first)
control = [model '/3 - Control'];
add_block('simulink/Ports & Subsystems/Subsystem', control, 'Position', [300 120 420 180]);
delete_line(control, 'In1/1', 'Out1/1');
delete_block([control '/In1']);
delete_block([control '/Out1']);
add_block('simulink/Signal Routing/Data Store Read', [control '/Read phase'], 'DataStoreName', 'Phase', ...
    'SampleTime', '0.1', 'Position', [40 40 140 70]);
add_block('simulink/Sinks/To Workspace', [control '/Seen'], 'VariableName', 'seen', ...
    'SaveFormat', 'Structure With Time', 'Position', [200 40 280 70]);
add_line(control, 'Read phase/1', 'Seen/1');

% "1 - Experiment Logic": the phase becomes 1 at t = 0.5 s
logic = [model '/1 - Experiment Logic'];
add_block('simulink/Ports & Subsystems/Subsystem', logic, 'Position', [100 120 220 180]);
delete_line(logic, 'In1/1', 'Out1/1');
delete_block([logic '/In1']);
delete_block([logic '/Out1']);
add_block('simulink/Sources/Step', [logic '/Phase changes'], 'Time', '0.5', 'Before', '0', 'After', '1', ...
    'SampleTime', '0.1', 'Position', [40 40 80 70]);
add_block('simulink/Signal Routing/Data Store Write', [logic '/Write phase'], 'DataStoreName', 'Phase', ...
    'Position', [140 40 240 70]);
add_line(logic, 'Phase changes/1', 'Write phase/1');

cases = {
    'A', 'virtual subsystems (Simulink picks the order)', 'off', '', ''
    'B', 'atomic, priorities wrong way round (Control 30, Logic 50)', 'on', '50', '30'
    'C', 'atomic, priorities in step order (Logic 30, Control 50)', 'on', '30', '50'
    };
results = struct('case', {}, 'setup', {}, 'first', {}, 'warnings', {}, 'seenAt', {}, 't', {}, 'seen', {});
fprintf('\nThe phase becomes 1 at t = 0.50 s. Each case: which subsystem ran first, the warnings, and\nwhen 3 - Control saw phase 1.\n\n');
for k = 1:size(cases, 1)
    set_param(logic, 'TreatAsAtomicUnit', cases{k, 3}, 'Priority', cases{k, 4});
    set_param(control, 'TreatAsAtomicUnit', cases{k, 3}, 'Priority', cases{k, 5});
    % Which ran first: the blocks' execution order while the model is compiled
    feval(model, [], [], [], 'compile'); %#ok<FVAL> (the model's name is in a variable)
    try
        first = ternary(orderOf([logic '/Write phase'], logic) < orderOf([control '/Read phase'], control), ...
            '1 - Experiment Logic', '3 - Control');
    catch err
        feval(model, [], [], [], 'term'); %#ok<FVAL>
        rethrow(err);
    end
    feval(model, [], [], [], 'term'); %#ok<FVAL>
    % Simulate, keeping Simulink's warnings
    warnState = warning('off', 'backtrace');
    restore = onCleanup(@() warning(warnState));
    log = evalc('out = sim(model);');
    clear restore
    warnings = regexp(log, '(?<=Warning: )[^\n]*(\n(?!Warning)[^\n]+)*', 'match');
    warnings = regexprep(strtrim(warnings), '\s+', ' ');
    t = out.seen.time;
    seen = out.seen.signals.values(:);
    seenAt = t(find(seen >= 1, 1));
    results(k) = struct('case', cases{k, 1}, 'setup', cases{k, 2}, 'first', first, 'warnings', {warnings}, ...
        'seenAt', seenAt, 't', t, 'seen', seen);
    fprintf('%s  %s\n   ran first: %s\n   warnings:  %d\n', cases{k, 1}, cases{k, 2}, first, numel(warnings));
    for w = warnings
        fprintf('              - %s\n', shorten(w{1}, model));
    end
    fprintf('   3 - Control saw phase 1 at t = %.2f s (%s)\n\n', seenAt, ...
        ternary(abs(seenAt - 0.5) < 1e-9, 'in the same step', sprintf('%d step late', round((seenAt - 0.5) / 0.1))));
end

if nargin > 0
    fig = figure('Visible', 'off', 'Units', 'inches', 'Position', [0 0 6.5 2.4], 'Color', 'w');
    cleanFig = onCleanup(@() close(fig));
    tl = tiledlayout(fig, 1, 3, 'TileSpacing', 'compact', 'Padding', 'compact');
    for k = 1:numel(results)
        ax = nexttile(tl);
        hold(ax, 'on'); grid(ax, 'on'); box(ax, 'on');
        stairs(ax, results(k).t, double(results(k).t >= 0.5 - 1e-9), 'k-', 'LineWidth', 1);
        stairs(ax, results(k).t, results(k).seen, 'r--', 'LineWidth', 1.2);
        ylim(ax, [-0.2 1.3]); xlim(ax, [0 1]);
        title(ax, sprintf('%s: %s first', results(k).case, results(k).first), 'FontWeight', 'normal', 'FontSize', 9);
        xlabel(ax, 'Time [s]');
        if k == 1
            ylabel(ax, 'Phase');
            legend(ax, {'written by Logic', 'seen by Control'}, 'Location', 'northwest', 'FontSize', 7);
        end
        set(ax, 'FontName', 'Times New Roman', 'FontSize', 9);
    end
    set(fig, 'PaperUnits', 'inches', 'PaperSize', [6.5 2.4], 'PaperPosition', [0 0 6.5 2.4]);
    print(fig, figureFile, ['-d' ternary(endsWith(figureFile, '.png'), 'png', 'pdf')], '-vector', '-r300');
    fprintf('Figure saved: %s\n', figureFile);
end
end

function n = orderOf(block, subsystem)
% A block's place in the step at the top level of the diagram (compiled model). A virtual
% subsystem is only a drawing, so its blocks are placed one by one among all the others; an
% atomic subsystem runs as one unit, so its own place is what counts.
if strcmp(get_param(subsystem, 'TreatAsAtomicUnit'), 'on')
    block = subsystem;
end
n = get_param(block, 'SortedOrder').BlockIndex;
end

function s = shorten(text, model)
s = regexprep(text, '<a [^>]*>|</a>', '');   % (Simulink's hyperlinks)
s = strrep(s, [model '/'], '');
s = regexprep(s, ' For performance reasons.*', '');
end

function v = ternary(c, a, b)
if c, v = a; else, v = b; end
end
