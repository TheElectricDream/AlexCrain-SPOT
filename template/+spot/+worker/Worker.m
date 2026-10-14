classdef Worker < handle
%WORKER Runs the SPOT Console's MATLAB commands in this MATLAB session.
%   The console's web server (+spot/+console/webapp/app.py --local) listens on a local port;
%   the worker connects to it and receives one JSON command per line:
%       {"id": 7, "cmd": "sim.run", "args": {...}}
%   and answers each with {"id": 7, "ok": true, "result": ...} or {"id": 7, "ok": false,
%   "error": "...", "details": {...}} (the root cause, and spot.sim.errorDetails of the error). Commands run in MATLAB's main thread from tcpclient's callback, so the
%   Command Window stays usable while the worker is idle.
%
%   Start and stop it with spot.worker.start / spot.worker.stop (SPOT.m does this).

    properties (SetAccess = private)
        Client          % tcpclient connected to the console
        Root            % project folder
        Settings        % current settings (see spot.params.defaults)
        SettingsFile    % where they are saved: <Root>/spot_settings.json
        CacheFolder     % <Root>/.spot_cache (simulation results for the browser)
    end

    properties (Access = private)
        StopRequested = false
        Datasets        % loaded results (spot.data.load), by id: the Results page and 3D playback
        NextDataset = 1
    end

    methods (Static)
        function disposeTimer(t)
            stop(t);
            delete(t);
        end

        function w = instance(w)
            % The running worker, if any (spot.worker.Worker.instance()).
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
            obj.SettingsFile = fullfile(root, 'spot_settings.json');
            obj.CacheFolder = fullfile(root, '.spot_cache');
            obj.Datasets = containers.Map('KeyType', 'char', 'ValueType', 'any');
            % Ids differ from one MATLAB session to the next, so a page never mixes them up
            obj.NextDataset = 100 * floor(mod(now * 86400, 1e5)) + 1;   % (not rand: keep the user's random stream)
            if isfile(obj.SettingsFile)
                obj.Settings = spot.params.load(obj.SettingsFile);
            else
                obj.Settings = spot.params.defaults();
            end
            diagrams = spot.sim.listDiagrams(root);
            if isempty(obj.Settings.diagram) && ~isempty(diagrams)
                obj.Settings.diagram = diagrams{1};
            end
        end

        function connect(obj, port, token)
            obj.Client = tcpclient('127.0.0.1', port, 'Timeout', 10);
            configureTerminator(obj.Client, 'LF');
            obj.send(struct('hello', 'spot-worker', 'token', token, 'info', obj.info()));
            configureCallback(obj.Client, 'terminator', @(~, ~) obj.onMessage());
        end

        function delete(obj)
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
                case 'ping'
                    result = 'pong';
                case 'info'
                    result = obj.info();
                case 'settings.get'
                    result = obj.settingsReply();
                case 'settings.set'
                    obj.Settings = spot.params.update(obj.Settings, args.settings, obj.Root);
                    result = obj.settingsReply();
                case 'settings.reset'
                    % The template's defaults, keeping the diagram; the old file is backed up
                    [obj.Settings, backup] = spot.params.reset(obj.Settings, obj.Root, obj.SettingsFile, obj.CacheFolder);
                    result = obj.settingsReply();
                    result.backup = backup;
                case 'settings.open'
                    % File dialog on this computer; .json settings or an old GUI state .mat
                    [file, folder] = uigetfile({'*.json;*.mat', 'SPOT settings (*.json, *.mat)'}, ...
                        'Load SPOT settings', obj.Root);
                    if isequal(file, 0)
                        result = struct('cancelled', true);
                        return
                    end
                    obj.Settings = spot.params.load(fullfile(folder, file));
                    result = obj.settingsReply();
                    result.loaded = fullfile(folder, file);
                case 'settings.saveAs'
                    [file, folder] = uiputfile('*.json', 'Save SPOT settings as', fullfile(obj.Root, 'spot_settings.json'));
                    if isequal(file, 0)
                        result = struct('cancelled', true);
                        return
                    end
                    spot.params.save(obj.Settings, fullfile(folder, file));
                    result = struct('saved', fullfile(folder, file));
                case 'diagram.open'
                    model = spot.sim.loadDiagram(obj.Settings.diagram, obj.Root);
                    open_system(model);
                    result = struct('opened', model);
                case 'diagram.showBlock'
                    % Open the diagram at a block and highlight it (the block an error is in)
                    result = struct('block', spot.sim.showBlock(obj.Settings.diagram, obj.Root, args.block));
                case 'sim.run'
                    % Report the simulation time, the debug messages and the live streams about
                    % four times a second, and stop when the console asks. Simulink runs timers now and then while it simulates, but not the
                    % connection's callback, so a stop request arrives as a file
                    % (<CacheFolder>/stop_request, written by the console) that the timer checks.
                    model = spot.sim.modelName(obj.Settings.diagram);
                    stopFile = fullfile(obj.CacheFolder, 'stop_request');
                    if isfile(stopFile)
                        delete(stopFile);
                    end
                    tsim = spot.params.timing(obj.Settings).tsim;
                    tmr = timer('Period', 0.25, 'ExecutionMode', 'fixedSpacing', 'BusyMode', 'drop', ...
                        'TimerFcn', @(~, ~) obj.progress(model, tsim, stopFile));
                    start(tmr);
                    cleanup = onCleanup(@() spot.worker.Worker.disposeTimer(tmr));
                    file = fullfile(obj.CacheFolder, 'sim_latest.json');
                    result = spot.sim.run(obj.Settings, obj.Root, file);
                    result = rmfield(result, 'messages');   % they are in the result file
                    result.stopped = obj.StopRequested;
                    obj.StopRequested = false;
                case 'sim.stop'
                    model = spot.sim.modelName(obj.Settings.diagram);
                    if bdIsLoaded(model) && ~strcmp(get_param(model, 'SimulationStatus'), 'stopped')
                        obj.StopRequested = true;
                        set_param(model, 'SimulationCommand', 'stop');
                        result = struct('stopping', true);
                    else
                        result = struct('stopping', false);
                    end
                case 'experiment.plan'
                    result = spot.experiment.plan(obj.Settings, obj.Root);
                case 'experiment.connect'
                    password = '';
                    if isfield(args, 'password')
                        password = args.password;
                    end
                    result = spot.experiment.connect(obj.Root, password);
                case 'experiment.build'
                    result = spot.experiment.build(obj.Settings, obj.Root);
                    if strlength(result.log) > 20000   % the console showed it live already
                        result.log = ['...' result.log(end - 20000:end)];
                    end
                case 'experiment.decode'
                    result = struct('files', {spot.experiment.decodeData(args.folder, obj.Settings, obj.Root)});
                    % The settings it ran with, for the Results page (phase times, arm geometry)
                    spot.params.save(obj.Settings, fullfile(args.folder, 'spot_settings.json'));
                case 'data.list'
                    result = struct('saved', spot.data.listSaved(obj.Root), 'loaded', {obj.loadedList()}, ...
                                    'simulation', evalin('base', 'exist(''dataClass'', ''var'') == 1'));
                case 'data.load'
                    if isfield(args, 'dialog') && args.dialog
                        [file, folder] = uigetfile({'*.mat', 'Logged data (*.mat)'}, 'Load Results', ...
                            fullfile(obj.Root, 'Saved Data', filesep));
                        if isequal(file, 0)
                            result = struct('cancelled', true);
                            return
                        end
                        args = struct('file', fullfile(folder, file));
                    end
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
                    % One request for several signals: {id, signal, col, ...} each (see spot.data.series)
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
                    % The Results page's plots in the lab's style (spot.data.toFigure): shown in
                    % MATLAB, or with save = true printed to a file (spot.data.exportFigure),
                    % chosen in a dialog unless args.file is given
                    opts = struct();
                    if isfield(args, 'options') && isstruct(args.options)
                        opts = args.options;
                    end
                    toFile = isfield(args, 'save') && args.save;
                    file = '';
                    if toFile
                        if isfield(args, 'file') && ~isempty(args.file)
                            file = args.file;
                        else
                            folder = fullfile(obj.Root, 'Saved Data', 'Figures');
                            if ~isfolder(folder)
                                mkdir(folder);
                            end
                            name = sprintf('SPOT_plot_%s.pdf', char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
                            [f, folder] = uiputfile({'*.pdf', 'PDF (*.pdf)'; '*.eps', 'EPS (*.eps)'; ...
                                '*.svg', 'SVG (*.svg)'; '*.png', 'PNG, 700 dpi (*.png)'}, 'Export plots', fullfile(folder, name));
                            if isequal(f, 0)
                                result = struct('cancelled', true);
                                return
                            end
                            file = fullfile(folder, f);
                        end
                        opts.visible = false;
                    end
                    fig = spot.data.toFigure(args.panels, @(id) obj.dataset(id), opts);
                    if toFile
                        closeFig = onCleanup(@() close(fig));
                        result = struct('file', spot.data.exportFigure(fig, file, opts));
                    else
                        figure(fig);
                        result = struct('figure', fig.Number);
                    end
                case 'massprops.writeTracker'
                    props = spot.massprops.compute(obj.Settings.mass);
                    result = struct('file', spot.massprops.writeTrackerPositions(props, obj.Root));
                case 'workspace.list'
                    result = spot.workspace.list();
                case 'workspace.inspect'
                    result = spot.workspace.inspect(args.name);
                case 'workspace.eval'
                    result = spot.workspace.evaluate(args.command);
                otherwise
                    error('spot:worker:unknown', 'Unknown command %s.', cmd);
            end
        end
    end

    methods (Access = private)
        function onMessage(obj)
            try
                line = readline(obj.Client);
            catch
                return  % connection closed
            end
            if isempty(line) || strlength(line) == 0
                return
            end
            msg = jsondecode(char(line));
            args = struct();
            if isfield(msg, 'args') && isstruct(msg.args)
                args = msg.args;
            end
            reply = struct('id', msg.id, 'ok', true, 'result', []);
            try
                reply.result = obj.dispatch(msg.cmd, args);
            catch err
                % The root cause in one line, and the details (causes, blocks, Simulink's output,
                % the full report) for the console to show on request
                reply.ok = false;
                reply.error = err.message;
                try
                    reply.details = spot.sim.errorDetails(err, obj.Root);
                    reply.error = reply.details.summary;
                    % The report's call stacks end in the worker and its connection: drop those
                    % frames ("Error in spot.worker..." / "matlabshared...") with their code line
                    % and ^^^ marker only, as a nested "Caused by" error can follow right after
                    reply.details.report = regexprep(reply.details.report, ...
                        '^[ \t]*Error in (spot\.worker\.|matlabshared\.)[^\n]*(\n[^\n]*\n[ \t]*\^+[ \t]*)?\n?', '', 'lineanchors');
                    reply.details.report = strtrim(regexprep(reply.details.report, '\n[ \t]*\n(\s*\n)+', '\n\n'));
                catch
                end
            end
            obj.send(reply);
        end

        function progress(obj, model, tsim, stopFile)
            try
                if isfile(stopFile)
                    delete(stopFile);
                    obj.StopRequested = true;
                    set_param(model, 'SimulationCommand', 'stop');
                end
                obj.send(struct('event', 'sim_progress', 't', get_param(model, 'SimulationTime'), ...
                    'tsim', tsim, 'status', get_param(model, 'SimulationStatus')));
                % The platforms' debug messages so far, for the Messages panel (selected platforms)
                new = spot.sim.debugLog('new');
                if ~isempty(new)
                    p = obj.Settings.platforms;
                    new = new(arrayfun(@(m) isfield(p, m.platform) && p.(m.platform), new));
                end
                if ~isempty(new)
                    obj.send(struct('event', 'sim_debug', 'messages', new));
                end
                % The Live Stream blocks' new samples, for the Live Plot
                live = spot.sim.liveData('new');
                if ~isempty(live)
                    obj.send(struct('event', 'sim_live', 'streams', live));
                end
            catch
            end
        end

        function ds = dataset(obj, id)
            if ~isKey(obj.Datasets, id)
                error('spot:worker:data', 'That dataset is no longer loaded in MATLAB (it was restarted?). Load it again.');
            end
            ds = obj.Datasets(id);
        end

        function list = loadedList(obj)
            ids = keys(obj.Datasets);
            [~, order] = sort(cellfun(@(k) sscanf(k, 'd%d'), ids));
            list = cellfun(@(k) spot.data.summary(obj.Datasets(k), k), ids(order), 'UniformOutput', false);
        end

        function send(obj, s)
            % Non-ASCII characters (names with "·" or "°", output of commands) go as \uXXXX
            % escapes, so the line arrives intact whatever encoding the connection uses
            j = jsonencode(s);
            for c = unique(j(j > 127))
                j = strrep(j, c, sprintf('\\u%04x', double(c)));
            end
            writeline(obj.Client, j);
        end

        function r = settingsReply(obj)
            % Settings, the values derived from them (or why they can't be applied), and the
            % diagrams to choose from. Applying also refreshes the base workspace.
            spot.params.save(obj.Settings, obj.SettingsFile);
            r = struct('settings', obj.Settings, 'derived', [], 'error', '', ...
                       'diagrams', {spot.sim.listDiagrams(obj.Root)}, 'file', obj.SettingsFile);
            try
                r.derived = spot.params.apply(obj.Settings, obj.Root);
            catch err
                r.error = err.message;
            end
        end

        function s = info(obj)
            v = ver('MATLAB');
            s = struct('release', strtrim(strrep(strrep(v.Release, '(', ''), ')', '')), ...
                       'version', version, 'root', obj.Root, 'settingsFile', obj.SettingsFile, ...
                       'simulink', license('test', 'Simulink') && ~isempty(which('sim')), ...
                       'controlToolbox', license('test', 'Control_Toolbox') && ~isempty(which('dlqr')), ...
                       'desktop', usejava('desktop'), 'pid', feature('getpid'));
        end
    end
end
