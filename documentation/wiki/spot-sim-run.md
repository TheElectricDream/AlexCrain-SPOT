# spot.sim.run

**File:** `+spot/+sim/run.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/run.m -->

## What it is for

Run one simulation of the selected diagram with the settings `s`, the way the Run button does. It puts the settings into the workspace, tells the diagram it is being simulated, runs Simulink with the output captured (so warnings don't flood the command window), turns a failure into a readable error, and gathers what the console shows afterwards: the replay, the messages, the live streams, the logged data.

The diagram file is **not** changed: simulation mode, hardware board, solver and pacing are passed as temporary overrides in a `Simulink.SimulationInput`.

## How to call it

```matlab
out = spot.sim.run(s, projectFolder)
out = spot.sim.run(s, projectFolder, resultFile)    % also writes the replay JSON
```

| `out.` | |
|---|---|
| `seconds` | wall-clock time of the run |
| `tsim` | simulated time |
| `samples` | number of logged time steps |
| `warnings` | how many `Warning:` lines Simulink printed |
| `messages` | that text (what the command window shows) |
| `debugMessages` | how many debug messages the platforms showed |
| `streams` | the Live Stream blocks' names |
| `file` | the result file, if asked for |

In the base workspace afterwards: `simOut`, `dataClass` (every logged signal), `dataPacket` (the signals side by side). On failure it throws a [[spot.sim.SimulationFailed|spot-sim-SimulationFailed]].

## Line by line

**Line 24.** Load the diagram (nothing opens on screen); `model` is its name.

**Line 25.** Apply the settings: every workspace variable, and `d` with the derived values (timing etc.).

**Lines 26–32.** The simulated servos must have the same limits as the real ones. If `armModel` exists, its `servo` section is replaced by the limits read from the diagram's own arm blocks (current limit, position limits, velocity limit: [[spot.arm.servoFromDiagram|spot-arm-servoFromDiagram]]) and written back.

**Lines 34–37.** `simMode = 1` tells the diagram to simulate the plant dynamics (its top-level If block); `WhoAmI = 0` is the simulation's platform id (on hardware, each platform finds its own). `onCleanup` sets `simMode` back to 0 when the function ends, however it ends.

**Lines 39–40.** A `SimulationInput` for the model, with normal mode and no hardware board (the diagram is saved for the Jetson; building for it is [[spot.experiment.build|spot-experiment-build]]'s job).

**Lines 41–44.** Real time: Simulink's pacing holds the simulation to 1 simulated second per second.

**Lines 45–52.** The solver from the settings: a variable-step solver with its steps and tolerances, or the fixed-step solver by name. (The fixed step itself is the diagram's `baseRate`.)

**Lines 54–57.** Switch off `warning backtrace` for the duration (otherwise every Simulink warning carries a call stack), restoring it on exit.

**Lines 58–59.** Reset the two recorders that the diagram's blocks write to while it runs: the debug messages ([[debugLog|spot-sim-debugLog]]) and the live streams ([[liveData|spot-sim-liveData]]).

**Lines 60–65.** Start the clock and run `sim(in)` inside `evalc`, which captures everything printed. The `try/catch` is *inside* the captured text so that, if Simulink fails, what it printed before failing is kept together with the error. `cleanMessages` (lines 159–169) strips hyperlink markup, the backspace characters MATLAB hides warning brackets with, and blank-line padding, and caps the text at 200 000 characters.

**Lines 66–73.** Some errors Simulink records instead of throwing (`simOut.ErrorMessage`). Make an exception from the message, and prefer the diagnostic object in `SimulationMetadata` when it exists, because it names the blocks.

**Lines 74–76.** Any error becomes a [[SimulationFailed|spot-sim-SimulationFailed]] carrying the captured output.

**Lines 77–85.** With the arm simulated, [[spot.arm.checkCommands|spot-arm-checkCommands]] looks at the logged data for goals the controllers ignored and torques beyond the servos' limits; each finding is appended to the messages as a `Warning:` line (on its own line).

**Lines 86–88.** Build `out` (counting the `Warning:` lines) and put `simOut` in the workspace.

**Lines 90–96.** `dataClass` is the diagram's logged signals (`simOut.dataPacket`, a struct of timeseries). `packAll` (lines 142–157) puts every signal's data side by side into one matrix, the old GUI's `dataPacket`; if the signals have different lengths this fails and is skipped.

**Lines 98–104.** The replay for the 3D view ([[results|spot-sim-results]]), with the messages, the debug messages of the platforms that took part (`selectedDebug`, lines 133–140, blanks the others), and the live streams thinned to at most 4000 samples each (`thinned`, lines 120–131, keeps evenly spaced samples; the full record stays in `liveData('get')`).

**Lines 105–117.** If a result file was asked for: make its folder, write the JSON, and record the path.

**Lines 120–169.** The helpers described above.

<!-- xref -->
- **Used by:** `+spot/+arm/checkCommands.m`, `+spot/+arm/defaults.m`, `+spot/+control/verifyConsoleController.m`, `+spot/+params/apply.m`, `+spot/+sim/Contents.m`, `+spot/+sim/SimulationFailed.m`, `+spot/+sim/debugLog.m`, `+spot/+sim/liveData.m`, `+spot/+worker/Worker.m`, `SPOT.m`
- **Uses:** [[spot.arm.checkCommands|spot-arm-checkCommands]], [[spot.arm.servoFromDiagram|spot-arm-servoFromDiagram]], [[spot.hardware.debugMessages|spot-hardware-debugMessages]], [[spot.hardware.liveStream|spot-hardware-liveStream]], [[spot.params.apply|spot-params-apply]], [[spot.sim.SimulationFailed|spot-sim-SimulationFailed]], [[spot.sim.debugLog|spot-sim-debugLog]], [[spot.sim.errorDetails|spot-sim-errorDetails]], [[spot.sim.liveData|spot-sim-liveData]], [[spot.sim.loadDiagram|spot-sim-loadDiagram]], [[spot.sim.results|spot-sim-results]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/run.m</b> (169 lines; click to show)</summary>

```matlab
   1  function out = run(s, root, resultFile)
   2  %RUN Simulate the selected diagram with the given settings.
   3  %   out = spot.sim.run(s, projectFolder) puts the settings into the base workspace
   4  %   (spot.params.apply), simulates s.diagram, and leaves simOut, dataClass and dataPacket in the
   5  %   base workspace as the old GUI did. The diagram file is not modified: the simulation mode,
   6  %   hardware board and solver are passed as temporary overrides.
   7  %   out = spot.sim.run(s, projectFolder, resultFile) also writes the trajectory for the SPOT
   8  %   Console's 3D view to resultFile (JSON, see spot.sim.results).
   9  %
  10  %   What Simulink prints while simulating (its warnings and diagnostics) is captured instead of
  11  %   filling the Command Window: out.messages has the text and out.warnings how many warnings.
  12  %   The result file includes the text too.
  13  %
  14  %   When Simulink stops with an error, run throws a spot.sim.SimulationFailed: its message is
  15  %   the root cause (and the block it is in), and spot.sim.errorDetails(err) gives the whole
  16  %   chain of causes, the blocks, and what Simulink printed before failing.
  17  %
  18  %   out has fields seconds (wall-clock), tsim, samples, warnings, messages, debugMessages (how
  19  %   many debug messages the platforms showed, see spot.sim.debugLog), streams (the Live Stream
  20  %   blocks' names, see spot.sim.liveData) and, with resultFile, file.
  21  %   With s.realTime true the simulation runs at the clock's pace (Simulink's simulation pacing),
  22  %   e.g. to watch the Live Plot or the console as an experiment would show them.
  23  %   The result file also has the debug messages (debug: RED/BLACK/BLUE with t and text).
  24  model = spot.sim.loadDiagram(s.diagram, root);
  25  d = spot.params.apply(s, root);
  26  % The simulated servos are set up as the real ones: their limits from the diagram's arm blocks
  27  % (the arm's plant integrates at its own 1 ms step, whatever the sample rate)
  28  if evalin('base', 'exist(''armModel'', ''var'')')
  29      armModel = evalin('base', 'armModel');
  30      armModel.servo = spot.arm.servoFromDiagram(model, armModel.servo);
  31      assignin('base', 'armModel', armModel);
  32  end
  33  
  34  % simMode tells the diagram it is being simulated; WhoAmI = 0 is the simulation's platform id
  35  assignin('base', 'simMode', 1);
  36  assignin('base', 'WhoAmI', 0);
  37  restore = onCleanup(@() assignin('base', 'simMode', 0));
  38  
  39  in = Simulink.SimulationInput(model);
  40  in = in.setModelParameter('SimulationMode', 'normal', 'HardwareBoard', 'None');
  41  if isfield(s, 'realTime') && s.realTime
  42      % Real time: Simulink paces the simulation to the clock (1 s simulated per second)
  43      in = in.setModelParameter('EnablePacing', 'on', 'PacingRate', '1');
  44  end
  45  sv = s.solver;
  46  if strcmp(sv.type, 'Variable-step')
  47      in = in.setModelParameter('SolverType', 'Variable-step', 'Solver', sv.variableStepName, ...
  48          'MaxStep', sv.maxStep, 'MinStep', sv.minStep, 'InitialStep', sv.initialStep, ...
  49          'RelTol', sv.relTol, 'AbsTol', sv.absTol); %#ok<NASGU> (used in the evalc below)
  50  else
  51      in = in.setModelParameter('SolverType', 'Fixed-step', 'Solver', sv.fixedStepName); %#ok<NASGU>
  52  end
  53  
  54  % Capture Simulink's output, without the long call stacks it adds to every warning
  55  backtrace = warning('query', 'backtrace');
  56  warning('off', 'backtrace');
  57  restoreBacktrace = onCleanup(@() warning(backtrace));
  58  spot.sim.debugLog('reset');   % the platforms' debug messages (spot.hardware.debugMessages)
  59  spot.sim.liveData('reset');   % the Live Stream blocks' signals (spot.hardware.liveStream)
  60  tic;
  61  % Errors are caught inside the capture, so what Simulink printed before failing is kept with them
  62  simOut = [];
  63  simErr = [];
  64  messages = evalc('try, simOut = sim(in); catch simErr, end');
  65  messages = cleanMessages(messages);
  66  if isempty(simErr) && ~isempty(simOut.ErrorMessage)
  67      % An error Simulink recorded instead of throwing: its diagnostic has the causes and blocks
  68      simErr = MException('spot:sim:run', '%s', simOut.ErrorMessage);
  69      try
  70          simErr = simOut.SimulationMetadata.ExecutionInfo.ErrorDiagnostic.Diagnostic;
  71      catch
  72      end
  73  end
  74  if ~isempty(simErr)
  75      throw(spot.sim.SimulationFailed(simErr, messages));
  76  end
  77  % Arm goals the controllers ignore and torques the servos can't give (beyond their limits): say so
  78  if s.platforms.ARM && evalin('base', 'exist(''armModel'', ''var'')')
  79      for msg = spot.arm.checkCommands(simOut.dataPacket, evalin('base', 'armModel'))
  80          if ~isempty(messages) && messages(end) ~= newline
  81              messages(end + 1) = newline; %#ok<AGROW>
  82          end
  83          messages = sprintf('%sWarning: %s\n', messages, msg{1});
  84      end
  85  end
  86  out = struct('seconds', toc, 'tsim', d.tsim, 'warnings', numel(regexp(messages, '(^|\n)Warning: ', 'match')), ...
  87               'messages', messages);
  88  assignin('base', 'simOut', simOut);
  89  
  90  dataClass = simOut.dataPacket;
  91  assignin('base', 'dataClass', dataClass);
  92  try
  93      assignin('base', 'dataPacket', packAll(dataClass));
  94  catch
  95      % signals of different lengths can't be put side by side; dataClass has everything anyway
  96  end
  97  
  98  r = spot.sim.results(dataClass, s, d);
  99  r.messages = messages;
 100  r.debug = selectedDebug(spot.sim.debugLog('get'), s.platforms);
 101  out.debugMessages = sum(structfun(@(p) numel(p.t), r.debug));
 102  r.live = thinned(spot.sim.liveData('get'), 4000);
 103  out.streams = {r.live.name};
 104  out.samples = r.samples;
 105  if nargin > 2
 106      folder = fileparts(resultFile);
 107      if ~isfolder(folder)
 108          mkdir(folder);
 109      end
 110      fid = fopen(resultFile, 'w');
 111      if fid < 0
 112          error('spot:sim:run', 'Cannot write %s.', resultFile);
 113      end
 114      fwrite(fid, jsonencode(r), 'char');
 115      fclose(fid);
 116      out.file = resultFile;
 117  end
 118  end
 119  
 120  function live = thinned(live, maxSamples)
 121  % The streams for the console's Live Plot after the run: at most maxSamples samples each (the
 122  % full record stays available as spot.sim.liveData('get'))
 123  for k = 1:numel(live)
 124      n = numel(live(k).t);
 125      if n > maxSamples
 126          keep = unique(round(linspace(1, n, maxSamples)));
 127          live(k).t = live(k).t(keep);
 128          live(k).v = live(k).v(keep, :);
 129      end
 130  end
 131  end
 132  
 133  function log = selectedDebug(log, platforms)
 134  % The platforms' debug messages, only for the platforms taking part (the diagram writes all three)
 135  for name = {'RED', 'BLACK', 'BLUE'}
 136      if ~platforms.(name{1})
 137          log.(name{1}) = struct('t', zeros(0, 1), 'text', {cell(0, 1)});
 138      end
 139  end
 140  end
 141  
 142  function packet = packAll(dataClass)
 143  % All logged signals side by side, as the old GUI's dataPacket variable
 144  packet = [];
 145  for f = fieldnames(dataClass).'
 146      x = dataClass.(f{1});
 147      if isa(x, 'timeseries')
 148          packet = [packet, x.Data]; %#ok<AGROW>
 149      elseif isstruct(x)
 150          for g = fieldnames(x).'
 151              if isa(x.(g{1}), 'timeseries')
 152                  packet = [packet, x.(g{1}).Data]; %#ok<AGROW>
 153              end
 154          end
 155      end
 156  end
 157  end
 158  
 159  function text = cleanMessages(text)
 160  % Simulink's captured output without its hyperlink markup, the hidden "[<backspace>" and
 161  % "]<backspace>" MATLAB wraps warnings in, and blank-line padding
 162  text = regexprep(text, '<a [^>]*>|</a>', '');
 163  text = regexprep(text, ['[\[\]]?' char(8)], '');
 164  text = regexprep(text, '\n\s*\n(\s*\n)+', '\n\n');
 165  text = strtrim(text);
 166  if strlength(text) > 200000
 167      text = [text(1:200000) newline '... (shortened)'];
 168  end
 169  end
```

</details>
<!-- /listing -->
