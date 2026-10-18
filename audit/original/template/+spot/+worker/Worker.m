classdef Worker < handle
    %WORKER Runs the SPOT Console's MATLAB commands in this MATLAB session.
    %   The console's web server (+spot/+console/webapp/app.py --local) listens on a local port;
    %   the worker connects to it and receives one JSON command per line:
    %       {"id": 7, "cmd": "sim.run", "args": {...}}
    %   and answers each with
    %       {"id": 7, "ok": true, "result": ...}
    %       {"id": 7, "ok": false, "error": "...", "details": {...}}
    %   (the root cause, and spot.sim.errorDetails of the error). Commands run in MATLAB's main
    %   thread from tcpclient's callback, so the Command Window stays usable while the worker is
    %   idle.
    %
    %   The project's settings live in Constants.m (spot.params.fromConstants): the worker is
    %   the only writer of that file. The page sends the settings it changed (settings.set), the
    %   Designer and the calibration send constants (constants.set, calibration.take), and the
    %   worker rewrites those lines (spot.params.writeConstants) and applies the result. The
    %   console's Python reads the constants from this session through the MATLAB Engine, after
    %   each such command.
    %
    %   Start and stop it with spot.worker.start / spot.worker.stop (SPOT.m does this).
    %
    %   See also spot.worker.start, spot.worker.stop, spot.console.start.

    properties (SetAccess = private)
        Client          % tcpclient connected to the console
        Root            % project folder
        Settings        % current settings (spot.params.fromConstants)
        ConstantsFile   % where they live: <Root>/Constants.m
        CacheFolder     % <Root>/.spot_cache (simulation results for the browser)
    end

    properties (Access = private)
        ConstantsRead = 0       % Constants.m's modification time when the settings were read
        StopRequested = false   % the console asked to stop the running simulation
        Datasets        % loaded results (spot.data.load), by id: Results page and 3D playback
        NextDataset = 1 % next dataset id number
    end

    properties (Constant, Access = private)
        CONNECT_TIMEOUT = 10        % [s]
        PROGRESS_PERIOD = 0.25      % [s] simulation progress reports
        MAX_BUILD_LOG = 20000       % characters of the build log returned
    end

    methods (Static)
        function disposeTimer(tmr)
            % Stop and delete a timer.
            stop(tmr);
            delete(tmr);
        end

        function openInEditor(tmr, file)
            % Open a file in MATLAB's editor (from a timer), then drop the timer.
            try
                if usejava('desktop')
                    matlab.desktop.editor.openDocument(file);
                else
                    edit(file);
                end
            catch err
                fprintf(2, 'Could not open %s in the editor: %s\n', file, err.message);
            end
            stop(tmr);
            delete(tmr);
        end

        function w = instance(w)
            % The running worker, if any (spot.worker.Worker.instance()); set with an argument.
            persistent current
            if nargin
                current = w;
            end
            if ~isempty(current) && ~isvalid(current)
                current = [];
            end
            w = current;
        end
    end

    methods
        function obj = Worker(root)
            obj.Root = root;
            obj.ConstantsFile = fullfile(root, 'Constants.m');
            obj.CacheFolder = fullfile(root, '.spot_cache');
            obj.Datasets = containers.Map('KeyType', 'char', 'ValueType', 'any');

            % Ids differ from one MATLAB session to the next, so a page never mixes them up.
            % (Not rand: keep the user's random stream.)
            obj.NextDataset = 100 * floor(mod(now * 86400, 1e5)) + 1;

            % The project's settings, from its Constants.m.
            obj.readSettings();
        end

        function connect(obj, port, token)
            % Connect, say hello with the session token, and handle each line as a command.
            obj.Client = tcpclient('127.0.0.1', port, 'Timeout', obj.CONNECT_TIMEOUT);
            configureTerminator(obj.Client, 'LF');
            obj.send(struct('hello', 'spot-worker', 'token', token, 'info', obj.info()));
            configureCallback(obj.Client, 'terminator', @(~, ~) obj.onMessage());
        end

        function delete(obj)
            % Stop handling commands.
            if ~isempty(obj.Client)
                try
                    configureCallback(obj.Client, 'off');
                catch
                end
                obj.Client = [];
            end
        end

        function result = dispatch(obj, cmd, args)
            % Run one command. Public so it can be tried from the Command Window:
            %   w = spot.worker.Worker.instance(); w.dispatch('workspace.list', struct())
            switch cmd

                %% Connection
                case 'ping'
                    result = 'pong';

                case 'info'
                    result = obj.info();

                %% Settings
                case 'settings.get'
                    obj.readSettings();
                    result = obj.settingsReply();

                case 'settings.set'
                    % The page's changes: a list of {path, value} (spot.params.setPath), applied
                    % to the settings as Constants.m has them now, then written back.
                    obj.readSettings();
                    s = obj.Settings;
                    changes = {};
                    if isfield(args, 'changes')
                        changes = args.changes;
                    end
                    if isstruct(changes)
                        changes = num2cell(changes);
                    end
                    for k = 1:numel(changes)
                        s = spot.params.setPath(s, changes{k}.path, changes{k}.value);
                    end

                    % A new diagram brings its own solver settings.
                    if ~strcmp(obj.Settings.diagram, s.diagram) && ~isempty(s.diagram)
                        s.solver = spot.sim.readSolver(s.diagram, obj.Root, s.solver);
                    end
                    obj.writeSettings(s);
                    result = obj.settingsReply();

                case 'settings.reset'
                    % The template's values for every setting (the diagram stays), with a
                    % backup of Constants.m first.
                    [values, templateFile] = spot.params.templateValues(obj.Root);
                    if isempty(values)
                        error('spot:worker:reset', ...
                              ['No template to reset to: the project must sit in the ' ...
                               'repository''s projects folder, next to template/Constants.m.']);
                    end
                    backup = obj.backupConstants('reset');
                    values.DIAGRAM = obj.Settings.diagram;
                    spot.params.writeConstants(values);
                    obj.readSettings();
                    if ~isempty(obj.Settings.diagram)
                        s = obj.Settings;
                        s.solver = spot.sim.readSolver(s.diagram, obj.Root, s.solver);
                        obj.writeSettings(s);
                    end
                    result = obj.settingsReply();
                    result.backup = backup;
                    result.template = templateFile;

                %% Constants
                case 'constants.get'
                    result = Constants.all();

                case 'constants.set'
                    % Values for named constants (the Designer, the calibration's overrides,
                    % sync and LEDs), written and applied.
                    obj.readSettings();
                    result = struct('changed', {spot.params.writeConstants(args.values)});
                    obj.readSettings();
                    reply = obj.settingsReply();
                    result.settings = reply.settings;
                    result.derived = reply.derived;
                    result.error = reply.error;

                case 'constants.catalog'
                    % Every constant in its sections, for the console's Constants window.
                    obj.readSettings();
                    result = spot.params.constantsCatalog();

                case 'constants.setText'
                    % Values typed in the Constants window ({name, text} each), read as the
                    % type of the constant they replace (nothing is evaluated), written and
                    % applied.
                    obj.readSettings();
                    edits = args.edits;
                    if isstruct(edits)
                        edits = num2cell(edits);
                    end
                    current = Constants.all();
                    values = struct();
                    for k = 1:numel(edits)
                        name = char(edits{k}.name);
                        if ~isfield(current, name)
                            error('spot:worker:constants', 'Constants has no %s.', name);
                        end
                        values.(name) = spot.params.parseConstant(edits{k}.text, current.(name));
                    end
                    result = struct('changed', {spot.params.writeConstants(values)});
                    obj.readSettings();
                    reply = obj.settingsReply();
                    result.settings = reply.settings;
                    result.derived = reply.derived;
                    result.error = reply.error;
                    result.diagrams = reply.diagrams;
                    result.file = reply.file;

                case 'constants.edit'
                    % Constants.m in MATLAB's editor. The command runs inside the connection's
                    % callback, where the desktop's editor can't be opened: a timer opens it a
                    % moment later, from MATLAB's idle loop.
                    file = obj.ConstantsFile;
                    tmr = timer('StartDelay', 0.1, 'TimerFcn', ...
                        @(t, ~) spot.worker.Worker.openInEditor(t, file));
                    start(tmr);
                    result = struct('file', file);

                case 'calibration.take'
                    % The lab console's calibration: its numbers into Constants.LAB_*, its full
                    % record into Saved Data (spot.params.calibrationRecord). The
                    % arm's joints go through spot.arm.loadCalibration, which turns a measured
                    % inertia into the rotor's armature with the project's arm geometry.
                    obj.readSettings();
                    values = spot.worker.labConstants(args.calibration, obj.Settings);
                    result = struct('changed', {spot.params.writeConstants(values)});

                    % The lab's full record (drift paths, burns, sweeps), for the Calibration
                    % pane to draw: measurement data, kept in Saved Data.
                    if isfield(args, 'record') && ischar(args.record) && ~isempty(args.record)
                        spot.params.calibrationRecord(args.record);
                    end
                    obj.readSettings();
                    reply = obj.settingsReply();
                    result.settings = reply.settings;
                    result.derived = reply.derived;
                    result.error = reply.error;

                %% Diagram
                case 'diagram.open'
                    model = spot.sim.loadDiagram(obj.Settings.diagram, obj.Root);
                    open_system(model);
                    result = struct('opened', model);

                case 'diagram.showBlock'
                    % Open the diagram at a block and highlight it (the block an error is in).
                    result = struct('block', ...
                        spot.sim.showBlock(obj.Settings.diagram, obj.Root, args.block));

                %% Simulation
                case 'sim.run'
                    % Report progress, debug messages and live streams four times a second.
                    % Simulink runs timers while it simulates, but not the connection's
                    % callback, so a stop request arrives as a file (<CacheFolder>/stop_request,
                    % written by the console) that the timer checks.
                    model = spot.sim.modelName(obj.Settings.diagram);
                    stopFile = fullfile(obj.CacheFolder, 'stop_request');
                    if isfile(stopFile)
                        delete(stopFile);
                    end
                    tsim = spot.params.timing(obj.Settings).tsim;
                    progressTimer = timer('Period', obj.PROGRESS_PERIOD, ...
                        'ExecutionMode', 'fixedSpacing', 'BusyMode', 'drop', ...
                        'TimerFcn', @(~, ~) obj.progress(model, tsim, stopFile));
                    start(progressTimer);
                    cleanup = onCleanup(@() spot.worker.Worker.disposeTimer(progressTimer));

                    % Run; the messages are in the result file.
                    file = fullfile(obj.CacheFolder, 'sim_latest.json');
                    result = spot.sim.run(obj.Settings, obj.Root, file);
                    result = rmfield(result, 'messages');
                    result.stopped = obj.StopRequested;
                    obj.StopRequested = false;

                case 'sim.stop'
                    model = spot.sim.modelName(obj.Settings.diagram);
                    if bdIsLoaded(model) ...
                            && ~strcmp(get_param(model, 'SimulationStatus'), 'stopped')
                        obj.StopRequested = true;
                        set_param(model, 'SimulationCommand', 'stop');
                        result = struct('stopping', true);
                    else
                        result = struct('stopping', false);
                    end

                %% Experiment
                case 'experiment.plan'
                    result = spot.experiment.plan(obj.Settings, obj.Root);

                case 'experiment.connect'
                    password = '';
                    if isfield(args, 'password')
                        password = args.password;
                    end
                    result = spot.experiment.connect(obj.Root, password);

                case 'experiment.build'
                    % The console showed the log live already: return only its end.
                    result = spot.experiment.build(obj.Settings, obj.Root);
                    if strlength(result.log) > obj.MAX_BUILD_LOG
                        result.log = ['...' result.log(end - obj.MAX_BUILD_LOG:end)];
                    end

                case 'experiment.decode'
                    result = struct('files', ...
                        {spot.experiment.decodeData(args.folder, obj.Settings, obj.Root)});

                    % The settings it ran with, for the Results page (phase times, arm geometry).
                    settings = obj.Settings;   % saved by name
                    save(fullfile(args.folder, 'settings.mat'), 'settings');

                %% Results
                case 'data.list'
                    result = struct('saved', spot.data.listSaved(obj.Root), ...
                        'loaded', {obj.loadedList()}, ...
                        'simulation', evalin('base', 'exist(''dataClass'', ''var'') == 1'));

                case 'data.load'
                    % A file chosen in a dialog, if asked.
                    if isfield(args, 'dialog') && args.dialog
                        [file, folder] = uigetfile({'*.mat', 'Logged data (*.mat)'}, ...
                            'Load Results', fullfile(obj.Root, 'Saved Data', filesep));
                        if isequal(file, 0)
                            result = struct('cancelled', true);
                            return
                        end
                        args = struct('file', fullfile(folder, file));
                    end

                    % Each dataset kept under a new id.
                    loaded = spot.data.load(args, obj.Root, obj.Settings);
                    result = struct('datasets', {cell(1, numel(loaded))});
                    for k = 1:numel(loaded)
                        id = sprintf('d%d', obj.NextDataset);
                        obj.NextDataset = obj.NextDataset + 1;
                        obj.Datasets(id) = loaded{k};
                        result.datasets{k} = spot.data.summary(loaded{k}, id);
                    end

                case 'data.unload'
                    if isKey(obj.Datasets, args.id)
                        remove(obj.Datasets, args.id);
                    end
                    result = struct('unloaded', args.id);

                case 'data.series'
                    % Several signals in one request: {id, signal, col, ...} each
                    % (spot.data.series); an error for one doesn't fail the others.
                    items = args.items;
                    if isstruct(items)
                        items = num2cell(items);
                    end
                    result = cell(1, numel(items));
                    for k = 1:numel(items)
                        try
                            result{k} = spot.data.series(obj.dataset(items{k}.id), items{k});
                        catch err
                            result{k} = struct('error', err.message);
                        end
                    end

                case 'data.trajectory'
                    result = spot.data.trajectory(obj.dataset(args.id));

                case 'data.saveSimulation'
                    folder = spot.data.saveSimulation(obj.Settings, obj.Root);
                    result = struct('folder', folder);

                case 'data.figure'
                    % The Results page's plots in the lab's style (spot.data.toFigure): shown
                    % in MATLAB, or with save = true printed to a file (spot.data.exportFigure),
                    % chosen in a dialog unless args.file is given.
                    opts = struct();
                    if isfield(args, 'options') && isstruct(args.options)
                        opts = args.options;
                    end
                    toFile = isfield(args, 'save') && args.save;

                    % The file to print to.
                    file = '';
                    if toFile
                        if isfield(args, 'file') && ~isempty(args.file)
                            file = args.file;
                        else
                            folder = fullfile(obj.Root, 'Saved Data', 'Figures');
                            if ~isfolder(folder)
                                mkdir(folder);
                            end
                            name = sprintf('SPOT_plot_%s.pdf', ...
                                char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
                            [chosen, folder] = uiputfile({'*.pdf', 'PDF (*.pdf)'
                                                          '*.eps', 'EPS (*.eps)'
                                                          '*.svg', 'SVG (*.svg)'
                                                          '*.png', 'PNG, 700 dpi (*.png)'}, ...
                                'Export plots', fullfile(folder, name));
                            if isequal(chosen, 0)
                                result = struct('cancelled', true);
                                return
                            end
                            file = fullfile(folder, chosen);
                        end
                        opts.visible = false;
                    end

                    % Draw, then print and close, or show.
                    fig = spot.data.toFigure(args.panels, @(id) obj.dataset(id), opts);
                    if toFile
                        closeFig = onCleanup(@() close(fig));
                        result = struct('file', spot.data.exportFigure(fig, file, opts));
                    else
                        figure(fig);
                        result = struct('figure', fig.Number);
                    end

                %% Workspace panel and Command Window
                case 'workspace.list'
                    result = spot.workspace.list();

                case 'workspace.inspect'
                    result = spot.workspace.inspect(args.name);

                case 'workspace.eval'
                    result = spot.workspace.evaluate(args.command);

                %% Documentation tab
                case 'code.lint'
                    % The Code Analyzer's messages for code in the editor (not saved yet).
                    result = spot.worker.checkCode(args.name, args.text);

                case 'code.rehash'
                    % A file was saved from the page: use its new version from the next call.
                    rehash;
                    result = true;

                otherwise
                    error('spot:worker:unknown', 'Unknown command %s.', cmd);
            end
        end
    end

    methods (Access = private)
        function onMessage(obj)
            % Read one command line, run it, and send the reply.
            try
                rawLine = readline(obj.Client);
            catch
                return  % connection closed
            end
            if isempty(rawLine) || strlength(rawLine) == 0
                return
            end

            % The command and its arguments.
            msg = jsondecode(char(rawLine));
            args = struct();
            if isfield(msg, 'args') && isstruct(msg.args)
                args = msg.args;
            end

            % Run it; on error, the root cause in one line and the details (causes, blocks,
            % Simulink's output, the full report) for the console to show on request.
            reply = struct('id', msg.id, 'ok', true, 'result', []);
            try
                reply.result = obj.dispatch(msg.cmd, args);
            catch err
                reply.ok = false;
                reply.error = err.message;
                try
                    reply.details = spot.sim.errorDetails(err, obj.Root);
                    reply.error = reply.details.summary;

                    % The report's call stacks end in the worker and its connection: drop those
                    % frames ("Error in spot.worker..." / "matlabshared...") with their code line
                    % and ^^^ marker only, as a nested "Caused by" error can follow right after.
                    reply.details.report = regexprep(reply.details.report, ...
                        ['^[ \t]*Error in (spot\.worker\.|matlabshared\.)[^\n]*' ...
                         '(\n[^\n]*\n[ \t]*\^+[ \t]*)?\n?'], '', 'lineanchors');
                    reply.details.report = strtrim(regexprep(reply.details.report, ...
                        '\n[ \t]*\n(\s*\n)+', '\n\n'));
                catch
                end
            end
            obj.send(reply);
        end

        function progress(obj, model, tsim, stopFile)
            % The timer's report during a simulation; also handles a stop request.
            try
                % Stop, if the console asked.
                if isfile(stopFile)
                    delete(stopFile);
                    obj.StopRequested = true;
                    set_param(model, 'SimulationCommand', 'stop');
                end

                % Simulation time and status.
                obj.send(struct('event', 'sim_progress', ...
                    't', get_param(model, 'SimulationTime'), ...
                    'tsim', tsim, 'status', get_param(model, 'SimulationStatus')));

                % New debug messages, for the selected platforms only (Messages panel).
                new = spot.sim.debugLog('new');
                if ~isempty(new)
                    selected = obj.Settings.platforms;
                    new = new(arrayfun(@(m) isfield(selected, m.platform) ...
                                            && selected.(m.platform), new));
                end
                if ~isempty(new)
                    obj.send(struct('event', 'sim_debug', 'messages', new));
                end

                % New Live Stream samples (Live Plot).
                live = spot.sim.liveData('new');
                if ~isempty(live)
                    obj.send(struct('event', 'sim_live', 'streams', live));
                end
            catch
            end
        end

        function ds = dataset(obj, id)
            % A loaded dataset by id, or an error.
            if ~isKey(obj.Datasets, id)
                error('spot:worker:data', ['That dataset is no longer loaded in MATLAB (it was ' ...
                                           'restarted?). Load it again.']);
            end
            ds = obj.Datasets(id);
        end

        function list = loadedList(obj)
            % Summaries of the loaded datasets, in the order they were loaded.
            ids = keys(obj.Datasets);
            [~, order] = sort(cellfun(@(k) sscanf(k, 'd%d'), ids));
            list = cellfun(@(k) spot.data.summary(obj.Datasets(k), k), ids(order), ...
                'UniformOutput', false);
        end

        function send(obj, s)
            % One JSON line. Non-ASCII characters (names with "·" or "°", command output) go
            % as \uXXXX escapes, so the line arrives intact whatever the connection's encoding.
            json = jsonencode(s);
            for ch = unique(json(json > 127))
                json = strrep(json, ch, sprintf('\\u%04x', double(ch)));
            end
            writeline(obj.Client, json);
        end

        function readSettings(obj)
            % The settings from Constants.m, read again when the file changed since (an edit in
            % the editor, say): the class is cleared so MATLAB parses the file anew.
            info = dir(obj.ConstantsFile);
            if isempty(info)
                error('spot:worker:constants', 'The project has no Constants.m (%s).', ...
                      obj.ConstantsFile);
            end
            if info.datenum ~= obj.ConstantsRead
                clear('Constants');
                obj.ConstantsRead = info.datenum;
            end
            obj.Settings = spot.params.fromConstants();

            % No diagram chosen: the project's (the first).
            diagrams = spot.sim.listDiagrams(obj.Root);
            if isempty(obj.Settings.diagram) && ~isempty(diagrams)
                obj.Settings.diagram = diagrams{1};
            end
        end

        function writeSettings(obj, s)
            % New settings into Constants.m (only the constants that changed), then read back.
            spot.params.writeConstants(spot.params.toConstants(s));
            obj.readSettings();
        end

        function backup = backupConstants(obj, why)
            % A copy of Constants.m in the cache folder before it is replaced wholesale.
            if ~isfolder(obj.CacheFolder)
                mkdir(obj.CacheFolder);
            end
            backup = fullfile(obj.CacheFolder, sprintf('Constants_before_%s_%s.m', why, ...
                char(datetime('now', 'Format', 'yyyyMMdd_HHmmss'))));
            copyfile(obj.ConstantsFile, backup);
        end

        function reply = settingsReply(obj)
            % Settings, the values derived from them (or why they can't be applied), the
            % diagrams to choose from, and the file they live in. Applying refreshes the
            % workspace.
            reply = struct('settings', obj.Settings, 'derived', [], 'error', '', ...
                           'diagrams', {spot.sim.listDiagrams(obj.Root)}, ...
                           'file', obj.ConstantsFile);
            try
                reply.derived = spot.params.apply(obj.Settings, obj.Root);
            catch err
                reply.error = err.message;
            end
        end

        function details = info(obj)
            % MATLAB release and version, the project, the toolboxes the console needs, and
            % this process.
            matlabVer = ver('MATLAB');
            details = struct( ...
                'release', strtrim(strrep(strrep(matlabVer.Release, '(', ''), ')', '')), ...
                'version', version, 'root', obj.Root, 'constantsFile', obj.ConstantsFile, ...
                'simulink', license('test', 'Simulink') && ~isempty(which('sim')), ...
                'controlToolbox', license('test', 'Control_Toolbox') && ~isempty(which('dlqr')), ...
                'desktop', usejava('desktop'), 'pid', feature('getpid'));
        end
    end
end
