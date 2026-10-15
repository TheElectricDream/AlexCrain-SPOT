# demo_execution_order.m

**File:** `documentation/demo_execution_order.m`

<!-- source: documentation/demo_execution_order.m -->

## What it is for

A small experiment that shows **why** SPOT's diagram makes its top-level subsystems atomic and gives them priorities ([[Diagram Overview|Diagram-Overview]]). It builds a tiny diagram in memory: a *1 - Experiment Logic* subsystem writes a phase into a data store at t = 0.5 s and a *3 - Control* subsystem reads it, nothing else connecting them, just as SPOT's subsystems only talk through data stores. It simulates three versions and prints which subsystem ran first, Simulink's data-store warnings, and when Control saw the new phase:

| Case | Setup | Control sees the phase |
|---|---|---|
| A | virtual subsystems (Simulink picks the order) | depends on luck |
| B | atomic, priorities the wrong way round (Control 30, Logic 50) | one step (0.1 s) late, with a "read before write" warning |
| C | atomic, priorities in step order (Logic 30, Control 50) | in the same step |

```matlab
cd documentation; demo_execution_order            % prints the three cases
results = demo_execution_order('demo.pdf');       % and saves a figure of what Control saw
```

## Line by line

**Lines 20–27.** A fresh model (closed on exit), fixed-step discrete solver at 0.1 s, 1 s long, outputs returned to the workspace.

**Lines 29–32.** The data store `Phase`, initial 0, with Simulink's read-before-write and write-after-read checks set to warn (as SPOT's stores are).

**Lines 34–45.** *3 - Control*: a subsystem (its default ports removed) with a Data Store Read of `Phase` at 0.1 s into a To Workspace block `seen`. It is added first, like any block Simulink might happen to put first.

**Lines 47–57.** *1 - Experiment Logic*: a Step from 0 to 1 at 0.5 s into a Data Store Write of `Phase`.

**Lines 59–65.** The three cases (atomic on/off and the two priorities) and the result struct.

**Lines 66–97.** For each case: set the subsystems' `TreatAsAtomicUnit` and `Priority`; compile the model and compare the two blocks' positions in the sorted order (`orderOf`, 123–131: for an atomic subsystem the subsystem's own place counts); simulate with the warnings captured (`evalc`, backtrace off) and extracted by a regular expression; find when `seen` first reached 1; print it all.

**Lines 99–120.** Optionally a three-panel figure (what Logic wrote vs what Control saw) in Times New Roman, saved as PDF or PNG.

**Lines 133–141.** `shorten` (a warning without hyperlinks and the model's name) and `ternary`.

## The code
<!-- listing -->
<details>
<summary><b>documentation/demo_execution_order.m</b> (141 lines; click to show)</summary>

```matlab
   1  function results = demo_execution_order(figureFile)
   2  %DEMO_EXECUTION_ORDER Why SPOT's diagram makes its top-level subsystems atomic and gives them priorities.
   3  %   demo_execution_order builds a tiny diagram in memory (nothing is saved) that works like the
   4  %   SPOT diagram: a "1 - Experiment Logic" subsystem writes the phase into a data store, and a
   5  %   "3 - Control" subsystem reads it. Nothing else connects them, just as SPOT's top-level
   6  %   subsystems only talk through data stores. The phase changes from 0 to 1 at t = 0.5 s; the
   7  %   model runs every 0.1 s. It simulates three versions and prints, for each, which subsystem ran
   8  %   first, Simulink's data store warnings, and when Control saw the new phase:
   9  %     A  virtual subsystems (how they were): Simulink picks the order itself
  10  %     B  atomic subsystems with priorities the wrong way round (Control 30, Logic 50)
  11  %     C  atomic subsystems with priorities in the order of a step (Logic 30, Control 50)
  12  %   In the right order Control sees the phase in the step it changes; in the wrong order one
  13  %   step (0.1 s) later, and Simulink warns that a data store was read before it was written.
  14  %
  15  %   results = demo_execution_order returns the three cases as a struct array.
  16  %   demo_execution_order(file) also saves a figure of what Control saw (e.g. 'demo.pdf').
  17  %
  18  %   See documentation/SPOT_Tutorial.pdf, "Inside the diagram: the order of a step".
  19  
  20  model = 'spotExecutionOrderDemo';
  21  if bdIsLoaded(model)
  22      close_system(model, 0);
  23  end
  24  new_system(model);
  25  cleanup = onCleanup(@() close_system(model, 0));
  26  set_param(model, 'SolverType', 'Fixed-step', 'Solver', 'FixedStepDiscrete', 'FixedStep', '0.1', ...
  27      'StopTime', '1', 'ReturnWorkspaceOutputs', 'on');
  28  
  29  % The data store, with Simulink's checks on (as for most of SPOT's stores)
  30  add_block('simulink/Signal Routing/Data Store Memory', [model '/Phase store'], 'DataStoreName', 'Phase', ...
  31      'InitialValue', '0', 'ReadBeforeWriteMsg', 'warning', 'WriteAfterReadMsg', 'warning', ...
  32      'Position', [40 30 120 70]);
  33  
  34  % "3 - Control": reads the phase and logs what it saw (added first, like any block Simulink may
  35  % happen to put first)
  36  control = [model '/3 - Control'];
  37  add_block('simulink/Ports & Subsystems/Subsystem', control, 'Position', [300 120 420 180]);
  38  delete_line(control, 'In1/1', 'Out1/1');
  39  delete_block([control '/In1']);
  40  delete_block([control '/Out1']);
  41  add_block('simulink/Signal Routing/Data Store Read', [control '/Read phase'], 'DataStoreName', 'Phase', ...
  42      'SampleTime', '0.1', 'Position', [40 40 140 70]);
  43  add_block('simulink/Sinks/To Workspace', [control '/Seen'], 'VariableName', 'seen', ...
  44      'SaveFormat', 'Structure With Time', 'Position', [200 40 280 70]);
  45  add_line(control, 'Read phase/1', 'Seen/1');
  46  
  47  % "1 - Experiment Logic": the phase becomes 1 at t = 0.5 s
  48  logic = [model '/1 - Experiment Logic'];
  49  add_block('simulink/Ports & Subsystems/Subsystem', logic, 'Position', [100 120 220 180]);
  50  delete_line(logic, 'In1/1', 'Out1/1');
  51  delete_block([logic '/In1']);
  52  delete_block([logic '/Out1']);
  53  add_block('simulink/Sources/Step', [logic '/Phase changes'], 'Time', '0.5', 'Before', '0', 'After', '1', ...
  54      'SampleTime', '0.1', 'Position', [40 40 80 70]);
  55  add_block('simulink/Signal Routing/Data Store Write', [logic '/Write phase'], 'DataStoreName', 'Phase', ...
  56      'Position', [140 40 240 70]);
  57  add_line(logic, 'Phase changes/1', 'Write phase/1');
  58  
  59  cases = {
  60      'A', 'virtual subsystems (Simulink picks the order)', 'off', '', ''
  61      'B', 'atomic, priorities wrong way round (Control 30, Logic 50)', 'on', '50', '30'
  62      'C', 'atomic, priorities in step order (Logic 30, Control 50)', 'on', '30', '50'
  63      };
  64  results = struct('case', {}, 'setup', {}, 'first', {}, 'warnings', {}, 'seenAt', {}, 't', {}, 'seen', {});
  65  fprintf('\nThe phase becomes 1 at t = 0.50 s. Each case: which subsystem ran first, the warnings, and\nwhen 3 - Control saw phase 1.\n\n');
  66  for k = 1:size(cases, 1)
  67      set_param(logic, 'TreatAsAtomicUnit', cases{k, 3}, 'Priority', cases{k, 4});
  68      set_param(control, 'TreatAsAtomicUnit', cases{k, 3}, 'Priority', cases{k, 5});
  69      % Which ran first: the blocks' execution order while the model is compiled
  70      feval(model, [], [], [], 'compile'); %#ok<FVAL> (the model's name is in a variable)
  71      try
  72          first = ternary(orderOf([logic '/Write phase'], logic) < orderOf([control '/Read phase'], control), ...
  73              '1 - Experiment Logic', '3 - Control');
  74      catch err
  75          feval(model, [], [], [], 'term'); %#ok<FVAL>
  76          rethrow(err);
  77      end
  78      feval(model, [], [], [], 'term'); %#ok<FVAL>
  79      % Simulate, keeping Simulink's warnings
  80      warnState = warning('off', 'backtrace');
  81      restore = onCleanup(@() warning(warnState));
  82      log = evalc('out = sim(model);');
  83      clear restore
  84      warnings = regexp(log, '(?<=Warning: )[^\n]*(\n(?!Warning)[^\n]+)*', 'match');
  85      warnings = regexprep(strtrim(warnings), '\s+', ' ');
  86      t = out.seen.time;
  87      seen = out.seen.signals.values(:);
  88      seenAt = t(find(seen >= 1, 1));
  89      results(k) = struct('case', cases{k, 1}, 'setup', cases{k, 2}, 'first', first, 'warnings', {warnings}, ...
  90          'seenAt', seenAt, 't', t, 'seen', seen);
  91      fprintf('%s  %s\n   ran first: %s\n   warnings:  %d\n', cases{k, 1}, cases{k, 2}, first, numel(warnings));
  92      for w = warnings
  93          fprintf('              - %s\n', shorten(w{1}, model));
  94      end
  95      fprintf('   3 - Control saw phase 1 at t = %.2f s (%s)\n\n', seenAt, ...
  96          ternary(abs(seenAt - 0.5) < 1e-9, 'in the same step', sprintf('%d step late', round((seenAt - 0.5) / 0.1))));
  97  end
  98  
  99  if nargin > 0
 100      fig = figure('Visible', 'off', 'Units', 'inches', 'Position', [0 0 6.5 2.4], 'Color', 'w');
 101      cleanFig = onCleanup(@() close(fig));
 102      tl = tiledlayout(fig, 1, 3, 'TileSpacing', 'compact', 'Padding', 'compact');
 103      for k = 1:numel(results)
 104          ax = nexttile(tl);
 105          hold(ax, 'on'); grid(ax, 'on'); box(ax, 'on');
 106          stairs(ax, results(k).t, double(results(k).t >= 0.5 - 1e-9), 'k-', 'LineWidth', 1);
 107          stairs(ax, results(k).t, results(k).seen, 'r--', 'LineWidth', 1.2);
 108          ylim(ax, [-0.2 1.3]); xlim(ax, [0 1]);
 109          title(ax, sprintf('%s: %s first', results(k).case, results(k).first), 'FontWeight', 'normal', 'FontSize', 9);
 110          xlabel(ax, 'Time [s]');
 111          if k == 1
 112              ylabel(ax, 'Phase');
 113              legend(ax, {'written by Logic', 'seen by Control'}, 'Location', 'northwest', 'FontSize', 7);
 114          end
 115          set(ax, 'FontName', 'Times New Roman', 'FontSize', 9);
 116      end
 117      set(fig, 'PaperUnits', 'inches', 'PaperSize', [6.5 2.4], 'PaperPosition', [0 0 6.5 2.4]);
 118      print(fig, figureFile, ['-d' ternary(endsWith(figureFile, '.png'), 'png', 'pdf')], '-vector', '-r300');
 119      fprintf('Figure saved: %s\n', figureFile);
 120  end
 121  end
 122  
 123  function n = orderOf(block, subsystem)
 124  % A block's place in the step at the top level of the diagram (compiled model). A virtual
 125  % subsystem is only a drawing, so its blocks are placed one by one among all the others; an
 126  % atomic subsystem runs as one unit, so its own place is what counts.
 127  if strcmp(get_param(subsystem, 'TreatAsAtomicUnit'), 'on')
 128      block = subsystem;
 129  end
 130  n = get_param(block, 'SortedOrder').BlockIndex;
 131  end
 132  
 133  function s = shorten(text, model)
 134  s = regexprep(text, '<a [^>]*>|</a>', '');   % (Simulink's hyperlinks)
 135  s = strrep(s, [model '/'], '');
 136  s = regexprep(s, ' For performance reasons.*', '');
 137  end
 138  
 139  function v = ternary(c, a, b)
 140  if c, v = a; else, v = b; end
 141  end
```

</details>
<!-- /listing -->
