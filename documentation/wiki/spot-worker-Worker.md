# spot.worker.Worker

**File:** `+spot/+worker/Worker.m` · **Package:** [[spot.worker|spot-worker]]

<!-- source: +spot/+worker/Worker.m -->

## What it is for

The object that runs the console's commands in your MATLAB session. It holds the connection to the console's server, the current settings (and saves them after every change), and the datasets loaded for the Results tab (by id, so the page never receives raw data). Its `dispatch` method is the complete list of what the page can ask MATLAB to do.

## How to use it

Normally you don't: `SPOT` creates it. From the Command Window you can reach it and try commands:

```matlab
w = spot.worker.Worker.instance();
w.dispatch('workspace.list', struct())
w.Settings
```

## The commands

| `cmd` | What it does | Returns |
|---|---|---|
| `ping`, `info` | liveness; MATLAB release, toolboxes, project folder, pid | |
| `settings.get` / `settings.set` / `settings.reset` / `settings.open` / `settings.saveAs` | the Simulation tab's settings: current; replace ([[spot.params.update|spot-params-update]]); defaults with backup; load through a file dialog; save-as dialog | the settings, the derived values (or the error), the diagram list, the file |
| `diagram.open`, `diagram.showBlock` | open the diagram in Simulink; open it at a block ([[spot.sim.showBlock|spot-sim-showBlock]]) | |
| `sim.run`, `sim.stop` | simulate ([[spot.sim.run|spot-sim-run]]) with progress events; stop it | the run's summary |
| `experiment.plan` / `connect` / `build` / `decode` | the Experiment tab's Check, Connect, Build, and converting collected logs | |
| `data.list` / `load` / `unload` / `series` / `trajectory` / `saveSimulation` / `figure` | the Results tab and the 3D playback ([[spot.data|spot-data]]) | summaries, series, trajectories |
| `massprops.writeTracker` | write the PhaseSpace marker file | |
| `workspace.list` / `inspect` / `eval` | the Workspace pane and the command window ([[spot.workspace|spot-workspace]]) | |

## Line by line

**Lines 12–24.** Properties: the `tcpclient`, the project folder, the settings and their file, the cache folder; privately, whether a stop was asked for, the loaded datasets (a map by id) and the next id.

**Lines 27–30.** `disposeTimer`: stop and delete a timer (used by `onCleanup`).

**Lines 32–42.** `instance`: a persistent handle to the running worker (set with an argument, read without), cleared if the object was deleted.

**Lines 46–62.** Constructor: paths; an empty dataset map; the first dataset id from the clock (so ids differ between MATLAB sessions and a page can't mix up datasets from a restarted MATLAB; not `rand`, to leave the user's random stream alone); the settings from the file or the defaults; with no diagram chosen, the first in the folder.

**Lines 64–69.** `connect`: open the socket to the server's worker port, lines end with LF, announce itself with the token and MATLAB's info, and run `onMessage` whenever a full line arrives.

**Lines 71–79.** `delete`: switch the callback off and drop the socket.

**Lines 81–265.** `dispatch`, one `case` per command:
- `settings.set` (91–93) runs [[spot.params.update|spot-params-update]] and answers with `settingsReply`.
- `settings.reset` (94–98) also returns the backup's path.
- `settings.open` / `settings.saveAs` (99–117) use MATLAB's file dialogs; a cancelled dialog returns `cancelled`.
- `sim.run` (125–144): deletes any stale stop-request file; computes `tsim`; starts a 0.25 s timer that calls `progress`; runs [[spot.sim.run|spot-sim-run]] with the result file `.spot_cache/sim_latest.json`; removes the messages from the reply (the page reads the file) and adds whether it was stopped. Simulink runs timers while simulating but not the socket's callback, so a stop request comes as a file written by the server (`stop_request`), which the timer checks.
- `sim.stop` (145–153): if the model is simulating, set the flag and send Simulink the stop command.
- `experiment.build` (162–166) cuts the returned log to its last 20 000 characters (the page already saw it live).
- `experiment.decode` (167–170) converts and also saves the settings beside the data.
- `data.list` (171–173): the saved runs, the loaded datasets' summaries and whether there is a last simulation.
- `data.load` (174–191): with `dialog`, a file dialog; load; give each dataset an id and return its summary.
- `data.series` (197–210): several signals in one request, each a result or an error.
- `data.figure` (216–252): the page's panels as a MATLAB figure; with `save`, printed to a file chosen in a dialog (default `Saved Data/Figures/SPOT_plot_<stamp>.pdf`) or given, and closed afterwards.

**Lines 269–304.** `onMessage`: read a line (a closed connection just returns), decode it, run `dispatch` and reply with `ok: true` and the result, or `ok: false` with the root cause and the details from [[spot.sim.errorDetails|spot-sim-errorDetails]]; the report's frames inside the worker and the socket library are stripped so the user sees their own code's frames.

**Lines 306–331.** `progress` (the timer): act on a stop-request file; send the simulation time and status; send the new debug messages of the selected platforms ([[spot.sim.debugLog|spot-sim-debugLog]]) and the new live-stream samples ([[spot.sim.liveData|spot-sim-liveData]]).

**Lines 333–344.** `dataset` (a loaded dataset or a clear error when MATLAB was restarted) and `loadedList` (summaries in id order).

**Lines 346–354.** `send`: JSON with every non-ASCII character as a `\uXXXX` escape, so names with `·` or `°` survive any socket encoding.

**Lines 356–367.** `settingsReply`: save the settings, then the reply with the settings, the derived values from [[spot.params.apply|spot-params-apply]] (or the error message when they can't be applied: the page shows it at the top and disables Run), the diagram list and the file path.

**Lines 369–376.** `info`: MATLAB's release and version, the project folder, whether Simulink and the Control System Toolbox are available, whether the desktop is running, the process id.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/data.js`, `+spot/+console/webapp/static/sim.js`, `+spot/+data/toFigure.m`, `+spot/+worker/start.m`, `+spot/+worker/stop.m`
- **Uses:** [[spot.data.exportFigure|spot-data-exportFigure]], [[spot.data.listSaved|spot-data-listSaved]], [[spot.data.load|spot-data-load]], [[spot.data.saveSimulation|spot-data-saveSimulation]], [[spot.data.series|spot-data-series]], [[spot.data.summary|spot-data-summary]], [[spot.data.toFigure|spot-data-toFigure]], [[spot.data.trajectory|spot-data-trajectory]], [[spot.experiment.build|spot-experiment-build]], [[spot.experiment.connect|spot-experiment-connect]], [[spot.experiment.decodeData|spot-experiment-decodeData]], [[spot.experiment.plan|spot-experiment-plan]], [[spot.massprops.compute|spot-massprops-compute]], [[spot.massprops.writeTrackerPositions|spot-massprops-writeTrackerPositions]], [[spot.params.apply|spot-params-apply]], [[spot.params.defaults|spot-params-defaults]], [[spot.params.load|spot-params-load]], [[spot.params.reset|spot-params-reset]], [[spot.params.save|spot-params-save]], [[spot.params.timing|spot-params-timing]], [[spot.params.update|spot-params-update]], [[spot.sim.debugLog|spot-sim-debugLog]], [[spot.sim.errorDetails|spot-sim-errorDetails]], [[spot.sim.listDiagrams|spot-sim-listDiagrams]], [[spot.sim.liveData|spot-sim-liveData]], [[spot.sim.loadDiagram|spot-sim-loadDiagram]], [[spot.sim.modelName|spot-sim-modelName]], [[spot.sim.run|spot-sim-run]], [[spot.sim.showBlock|spot-sim-showBlock]], [[spot.worker.start|spot-worker-start]], [[spot.worker.stop|spot-worker-stop]], [[spot.workspace.evaluate|spot-workspace-evaluate]], [[spot.workspace.inspect|spot-workspace-inspect]], [[spot.workspace.list|spot-workspace-list]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+worker/Worker.m</b> (378 lines; click to show)</summary>

```matlab
   1  classdef Worker < handle
   2  %WORKER Runs the SPOT Console's MATLAB commands in this MATLAB session.
   3  %   The console's web server (+spot/+console/webapp/app.py --local) listens on a local port;
   4  %   the worker connects to it and receives one JSON command per line:
   5  %       {"id": 7, "cmd": "sim.run", "args": {...}}
   6  %   and answers each with {"id": 7, "ok": true, "result": ...} or {"id": 7, "ok": false,
   7  %   "error": "...", "details": {...}} (the root cause, and spot.sim.errorDetails of the error). Commands run in MATLAB's main thread from tcpclient's callback, so the
   8  %   Command Window stays usable while the worker is idle.
   9  %
  10  %   Start and stop it with spot.worker.start / spot.worker.stop (SPOT.m does this).
  11  
  12      properties (SetAccess = private)
  13          Client          % tcpclient connected to the console
  14          Root            % project folder
  15          Settings        % current settings (see spot.params.defaults)
  16          SettingsFile    % where they are saved: <Root>/spot_settings.json
  17          CacheFolder     % <Root>/.spot_cache (simulation results for the browser)
  18      end
  19  
  20      properties (Access = private)
  21          StopRequested = false
  22          Datasets        % loaded results (spot.data.load), by id: the Results page and 3D playback
  23          NextDataset = 1
  24      end
  25  
  26      methods (Static)
  27          function disposeTimer(t)
  28              stop(t);
  29              delete(t);
  30          end
  31  
  32          function w = instance(w)
  33              % The running worker, if any (spot.worker.Worker.instance()).
  34              persistent current
  35              if nargin
  36                  current = w;
  37              end
  38              if ~isempty(current) && ~isvalid(current)
  39                  current = [];
  40              end
  41              w = current;
  42          end
  43      end
  44  
  45      methods
  46          function obj = Worker(root)
  47              obj.Root = root;
  48              obj.SettingsFile = fullfile(root, 'spot_settings.json');
  49              obj.CacheFolder = fullfile(root, '.spot_cache');
  50              obj.Datasets = containers.Map('KeyType', 'char', 'ValueType', 'any');
  51              % Ids differ from one MATLAB session to the next, so a page never mixes them up
  52              obj.NextDataset = 100 * floor(mod(now * 86400, 1e5)) + 1;   % (not rand: keep the user's random stream)
  53              if isfile(obj.SettingsFile)
  54                  obj.Settings = spot.params.load(obj.SettingsFile);
  55              else
  56                  obj.Settings = spot.params.defaults();
  57              end
  58              diagrams = spot.sim.listDiagrams(root);
  59              if isempty(obj.Settings.diagram) && ~isempty(diagrams)
  60                  obj.Settings.diagram = diagrams{1};
  61              end
  62          end
  63  
  64          function connect(obj, port, token)
  65              obj.Client = tcpclient('127.0.0.1', port, 'Timeout', 10);
  66              configureTerminator(obj.Client, 'LF');
  67              obj.send(struct('hello', 'spot-worker', 'token', token, 'info', obj.info()));
  68              configureCallback(obj.Client, 'terminator', @(~, ~) obj.onMessage());
  69          end
  70  
  71          function delete(obj)
  72              if ~isempty(obj.Client)
  73                  try
  74                      configureCallback(obj.Client, 'off');
  75                  catch
  76                  end
  77                  obj.Client = [];
  78              end
  79          end
  80  
  81          function result = dispatch(obj, cmd, args)
  82              % Run one command. Public so it can be tried from the Command Window:
  83              %   w = spot.worker.Worker.instance(); w.dispatch('workspace.list', struct())
  84              switch cmd
  85                  case 'ping'
  86                      result = 'pong';
  87                  case 'info'
  88                      result = obj.info();
  89                  case 'settings.get'
  90                      result = obj.settingsReply();
  91                  case 'settings.set'
  92                      obj.Settings = spot.params.update(obj.Settings, args.settings, obj.Root);
  93                      result = obj.settingsReply();
  94                  case 'settings.reset'
  95                      % The template's defaults, keeping the diagram; the old file is backed up
  96                      [obj.Settings, backup] = spot.params.reset(obj.Settings, obj.Root, obj.SettingsFile, obj.CacheFolder);
  97                      result = obj.settingsReply();
  98                      result.backup = backup;
  99                  case 'settings.open'
 100                      % File dialog on this computer; .json settings or an old GUI state .mat
 101                      [file, folder] = uigetfile({'*.json;*.mat', 'SPOT settings (*.json, *.mat)'}, ...
 102                          'Load SPOT settings', obj.Root);
 103                      if isequal(file, 0)
 104                          result = struct('cancelled', true);
 105                          return
 106                      end
 107                      obj.Settings = spot.params.load(fullfile(folder, file));
 108                      result = obj.settingsReply();
 109                      result.loaded = fullfile(folder, file);
 110                  case 'settings.saveAs'
 111                      [file, folder] = uiputfile('*.json', 'Save SPOT settings as', fullfile(obj.Root, 'spot_settings.json'));
 112                      if isequal(file, 0)
 113                          result = struct('cancelled', true);
 114                          return
 115                      end
 116                      spot.params.save(obj.Settings, fullfile(folder, file));
 117                      result = struct('saved', fullfile(folder, file));
 118                  case 'diagram.open'
 119                      model = spot.sim.loadDiagram(obj.Settings.diagram, obj.Root);
 120                      open_system(model);
 121                      result = struct('opened', model);
 122                  case 'diagram.showBlock'
 123                      % Open the diagram at a block and highlight it (the block an error is in)
 124                      result = struct('block', spot.sim.showBlock(obj.Settings.diagram, obj.Root, args.block));
 125                  case 'sim.run'
 126                      % Report the simulation time, the debug messages and the live streams about
 127                      % four times a second, and stop when the console asks. Simulink runs timers now and then while it simulates, but not the
 128                      % connection's callback, so a stop request arrives as a file
 129                      % (<CacheFolder>/stop_request, written by the console) that the timer checks.
 130                      model = spot.sim.modelName(obj.Settings.diagram);
 131                      stopFile = fullfile(obj.CacheFolder, 'stop_request');
 132                      if isfile(stopFile)
 133                          delete(stopFile);
 134                      end
 135                      tsim = spot.params.timing(obj.Settings).tsim;
 136                      tmr = timer('Period', 0.25, 'ExecutionMode', 'fixedSpacing', 'BusyMode', 'drop', ...
 137                          'TimerFcn', @(~, ~) obj.progress(model, tsim, stopFile));
 138                      start(tmr);
 139                      cleanup = onCleanup(@() spot.worker.Worker.disposeTimer(tmr));
 140                      file = fullfile(obj.CacheFolder, 'sim_latest.json');
 141                      result = spot.sim.run(obj.Settings, obj.Root, file);
 142                      result = rmfield(result, 'messages');   % they are in the result file
 143                      result.stopped = obj.StopRequested;
 144                      obj.StopRequested = false;
 145                  case 'sim.stop'
 146                      model = spot.sim.modelName(obj.Settings.diagram);
 147                      if bdIsLoaded(model) && ~strcmp(get_param(model, 'SimulationStatus'), 'stopped')
 148                          obj.StopRequested = true;
 149                          set_param(model, 'SimulationCommand', 'stop');
 150                          result = struct('stopping', true);
 151                      else
 152                          result = struct('stopping', false);
 153                      end
 154                  case 'experiment.plan'
 155                      result = spot.experiment.plan(obj.Settings, obj.Root);
 156                  case 'experiment.connect'
 157                      password = '';
 158                      if isfield(args, 'password')
 159                          password = args.password;
 160                      end
 161                      result = spot.experiment.connect(obj.Root, password);
 162                  case 'experiment.build'
 163                      result = spot.experiment.build(obj.Settings, obj.Root);
 164                      if strlength(result.log) > 20000   % the console showed it live already
 165                          result.log = ['...' result.log(end - 20000:end)];
 166                      end
 167                  case 'experiment.decode'
 168                      result = struct('files', {spot.experiment.decodeData(args.folder, obj.Settings, obj.Root)});
 169                      % The settings it ran with, for the Results page (phase times, arm geometry)
 170                      spot.params.save(obj.Settings, fullfile(args.folder, 'spot_settings.json'));
 171                  case 'data.list'
 172                      result = struct('saved', spot.data.listSaved(obj.Root), 'loaded', {obj.loadedList()}, ...
 173                                      'simulation', evalin('base', 'exist(''dataClass'', ''var'') == 1'));
 174                  case 'data.load'
 175                      if isfield(args, 'dialog') && args.dialog
 176                          [file, folder] = uigetfile({'*.mat', 'Logged data (*.mat)'}, 'Load Results', ...
 177                              fullfile(obj.Root, 'Saved Data', filesep));
 178                          if isequal(file, 0)
 179                              result = struct('cancelled', true);
 180                              return
 181                          end
 182                          args = struct('file', fullfile(folder, file));
 183                      end
 184                      loaded = spot.data.load(args, obj.Root, obj.Settings);
 185                      result = struct('datasets', {cell(1, numel(loaded))});
 186                      for k = 1:numel(loaded)
 187                          id = sprintf('d%d', obj.NextDataset);
 188                          obj.NextDataset = obj.NextDataset + 1;
 189                          obj.Datasets(id) = loaded{k};
 190                          result.datasets{k} = spot.data.summary(loaded{k}, id);
 191                      end
 192                  case 'data.unload'
 193                      if isKey(obj.Datasets, args.id)
 194                          remove(obj.Datasets, args.id);
 195                      end
 196                      result = struct('unloaded', args.id);
 197                  case 'data.series'
 198                      % One request for several signals: {id, signal, col, ...} each (see spot.data.series)
 199                      items = args.items;
 200                      if isstruct(items)
 201                          items = num2cell(items);
 202                      end
 203                      result = cell(1, numel(items));
 204                      for k = 1:numel(items)
 205                          try
 206                              result{k} = spot.data.series(obj.dataset(items{k}.id), items{k});
 207                          catch err
 208                              result{k} = struct('error', err.message);
 209                          end
 210                      end
 211                  case 'data.trajectory'
 212                      result = spot.data.trajectory(obj.dataset(args.id));
 213                  case 'data.saveSimulation'
 214                      folder = spot.data.saveSimulation(obj.Settings, obj.Root);
 215                      result = struct('folder', folder);
 216                  case 'data.figure'
 217                      % The Results page's plots in the lab's style (spot.data.toFigure): shown in
 218                      % MATLAB, or with save = true printed to a file (spot.data.exportFigure),
 219                      % chosen in a dialog unless args.file is given
 220                      opts = struct();
 221                      if isfield(args, 'options') && isstruct(args.options)
 222                          opts = args.options;
 223                      end
 224                      toFile = isfield(args, 'save') && args.save;
 225                      file = '';
 226                      if toFile
 227                          if isfield(args, 'file') && ~isempty(args.file)
 228                              file = args.file;
 229                          else
 230                              folder = fullfile(obj.Root, 'Saved Data', 'Figures');
 231                              if ~isfolder(folder)
 232                                  mkdir(folder);
 233                              end
 234                              name = sprintf('SPOT_plot_%s.pdf', char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
 235                              [f, folder] = uiputfile({'*.pdf', 'PDF (*.pdf)'; '*.eps', 'EPS (*.eps)'; ...
 236                                  '*.svg', 'SVG (*.svg)'; '*.png', 'PNG, 700 dpi (*.png)'}, 'Export plots', fullfile(folder, name));
 237                              if isequal(f, 0)
 238                                  result = struct('cancelled', true);
 239                                  return
 240                              end
 241                              file = fullfile(folder, f);
 242                          end
 243                          opts.visible = false;
 244                      end
 245                      fig = spot.data.toFigure(args.panels, @(id) obj.dataset(id), opts);
 246                      if toFile
 247                          closeFig = onCleanup(@() close(fig));
 248                          result = struct('file', spot.data.exportFigure(fig, file, opts));
 249                      else
 250                          figure(fig);
 251                          result = struct('figure', fig.Number);
 252                      end
 253                  case 'massprops.writeTracker'
 254                      props = spot.massprops.compute(obj.Settings.mass);
 255                      result = struct('file', spot.massprops.writeTrackerPositions(props, obj.Root));
 256                  case 'workspace.list'
 257                      result = spot.workspace.list();
 258                  case 'workspace.inspect'
 259                      result = spot.workspace.inspect(args.name);
 260                  case 'workspace.eval'
 261                      result = spot.workspace.evaluate(args.command);
 262                  otherwise
 263                      error('spot:worker:unknown', 'Unknown command %s.', cmd);
 264              end
 265          end
 266      end
 267  
 268      methods (Access = private)
 269          function onMessage(obj)
 270              try
 271                  line = readline(obj.Client);
 272              catch
 273                  return  % connection closed
 274              end
 275              if isempty(line) || strlength(line) == 0
 276                  return
 277              end
 278              msg = jsondecode(char(line));
 279              args = struct();
 280              if isfield(msg, 'args') && isstruct(msg.args)
 281                  args = msg.args;
 282              end
 283              reply = struct('id', msg.id, 'ok', true, 'result', []);
 284              try
 285                  reply.result = obj.dispatch(msg.cmd, args);
 286              catch err
 287                  % The root cause in one line, and the details (causes, blocks, Simulink's output,
 288                  % the full report) for the console to show on request
 289                  reply.ok = false;
 290                  reply.error = err.message;
 291                  try
 292                      reply.details = spot.sim.errorDetails(err, obj.Root);
 293                      reply.error = reply.details.summary;
 294                      % The report's call stacks end in the worker and its connection: drop those
 295                      % frames ("Error in spot.worker..." / "matlabshared...") with their code line
 296                      % and ^^^ marker only, as a nested "Caused by" error can follow right after
 297                      reply.details.report = regexprep(reply.details.report, ...
 298                          '^[ \t]*Error in (spot\.worker\.|matlabshared\.)[^\n]*(\n[^\n]*\n[ \t]*\^+[ \t]*)?\n?', '', 'lineanchors');
 299                      reply.details.report = strtrim(regexprep(reply.details.report, '\n[ \t]*\n(\s*\n)+', '\n\n'));
 300                  catch
 301                  end
 302              end
 303              obj.send(reply);
 304          end
 305  
 306          function progress(obj, model, tsim, stopFile)
 307              try
 308                  if isfile(stopFile)
 309                      delete(stopFile);
 310                      obj.StopRequested = true;
 311                      set_param(model, 'SimulationCommand', 'stop');
 312                  end
 313                  obj.send(struct('event', 'sim_progress', 't', get_param(model, 'SimulationTime'), ...
 314                      'tsim', tsim, 'status', get_param(model, 'SimulationStatus')));
 315                  % The platforms' debug messages so far, for the Messages panel (selected platforms)
 316                  new = spot.sim.debugLog('new');
 317                  if ~isempty(new)
 318                      p = obj.Settings.platforms;
 319                      new = new(arrayfun(@(m) isfield(p, m.platform) && p.(m.platform), new));
 320                  end
 321                  if ~isempty(new)
 322                      obj.send(struct('event', 'sim_debug', 'messages', new));
 323                  end
 324                  % The Live Stream blocks' new samples, for the Live Plot
 325                  live = spot.sim.liveData('new');
 326                  if ~isempty(live)
 327                      obj.send(struct('event', 'sim_live', 'streams', live));
 328                  end
 329              catch
 330              end
 331          end
 332  
 333          function ds = dataset(obj, id)
 334              if ~isKey(obj.Datasets, id)
 335                  error('spot:worker:data', 'That dataset is no longer loaded in MATLAB (it was restarted?). Load it again.');
 336              end
 337              ds = obj.Datasets(id);
 338          end
 339  
 340          function list = loadedList(obj)
 341              ids = keys(obj.Datasets);
 342              [~, order] = sort(cellfun(@(k) sscanf(k, 'd%d'), ids));
 343              list = cellfun(@(k) spot.data.summary(obj.Datasets(k), k), ids(order), 'UniformOutput', false);
 344          end
 345  
 346          function send(obj, s)
 347              % Non-ASCII characters (names with "·" or "°", output of commands) go as \uXXXX
 348              % escapes, so the line arrives intact whatever encoding the connection uses
 349              j = jsonencode(s);
 350              for c = unique(j(j > 127))
 351                  j = strrep(j, c, sprintf('\\u%04x', double(c)));
 352              end
 353              writeline(obj.Client, j);
 354          end
 355  
 356          function r = settingsReply(obj)
 357              % Settings, the values derived from them (or why they can't be applied), and the
 358              % diagrams to choose from. Applying also refreshes the base workspace.
 359              spot.params.save(obj.Settings, obj.SettingsFile);
 360              r = struct('settings', obj.Settings, 'derived', [], 'error', '', ...
 361                         'diagrams', {spot.sim.listDiagrams(obj.Root)}, 'file', obj.SettingsFile);
 362              try
 363                  r.derived = spot.params.apply(obj.Settings, obj.Root);
 364              catch err
 365                  r.error = err.message;
 366              end
 367          end
 368  
 369          function s = info(obj)
 370              v = ver('MATLAB');
 371              s = struct('release', strtrim(strrep(strrep(v.Release, '(', ''), ')', '')), ...
 372                         'version', version, 'root', obj.Root, 'settingsFile', obj.SettingsFile, ...
 373                         'simulink', license('test', 'Simulink') && ~isempty(which('sim')), ...
 374                         'controlToolbox', license('test', 'Control_Toolbox') && ~isempty(which('dlqr')), ...
 375                         'desktop', usejava('desktop'), 'pid', feature('getpid'));
 376          end
 377      end
 378  end
```

</details>
<!-- /listing -->
