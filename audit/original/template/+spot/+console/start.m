function url = start(root)
    %START Start the SPOT Console for a project and connect this MATLAB session to it.
    %   url = spot.console.start(root) shares this MATLAB session (the MATLAB Engine for Python,
    %   matlab.engine.shareEngine), starts the console's local web server (app.py --local,
    %   reachable from this computer only), and returns at once: a timer connects the MATLAB
    %   worker that runs the console's commands in this session and opens the console in the
    %   browser as soon as the server is ready. A console already running for this project is
    %   reused. SPOT.m calls this; spot.console.stop stops it.
    %
    %   Why a timer: the server reads the project's Constants from this session through the
    %   engine when it starts, and MATLAB only answers the engine while it is idle at the prompt
    %   (not inside a function waiting in pause), so this function must not wait for the server.
    %
    %   Inputs
    %     root  project folder
    %
    %   Outputs
    %     url   the console's address when it was already running, '' otherwise (printed when
    %           it is ready)
    %
    %   See also spot.console.stop, spot.console.open, spot.worker.start,
    %   spot.console.ensureEngine.

    %% Constants
    START_TIMEOUT = 90;     % [s] wait for the server to answer (the engine's first connection
                            %     can take a while on a slow computer)
    POLL_PERIOD = 0.5;      % [s]
    TIMER_TAG = 'spot-console-start';

    %% Preparation
    % The old block library would shadow the blocks' classes (now spot.hardware).
    spot.console.removeOldLibraryPaths();

    % Cache folder for the ready file and log.
    cache = fullfile(root, '.spot_cache');
    if ~isfolder(cache)
        mkdir(cache);
    end
    readyFile = fullfile(cache, 'console.json');

    %% Already running: connect and open
    ready = spot.console.status(root);
    if ~isempty(ready)
        spot.worker.start(ready.worker_port, ready.token, root);
        url = spot.console.open(root);
        return
    end
    url = '';

    %% Share this session
    % The console's Python reads Constants from here: matlab.engine.connect_matlab(name).
    if matlab.engine.isEngineShared
        engineName = matlab.engine.engineName;
    else
        engineName = sprintf('SPOT_%d', matlabProcessID);
        matlab.engine.shareEngine(engineName);
    end
    setenv('SPOT_MATLAB_ENGINE', engineName);

    %% Start the server
    % The server picks two free ports and makes the session token, and writes them to the
    % ready file. (MATLAB's randi would give the same token in every fresh session.)
    if isfile(readyFile)
        delete(readyFile);
    end
    app = fullfile(fileparts(mfilename('fullpath')), 'webapp', 'app.py');

    % Python with the MATLAB Engine for Python, offering to install either if missing.
    try
        python = spot.console.findPython();
    catch err
        if ~strcmp(err.identifier, 'spot:console:noPython')
            rethrow(err);
        end
        python = spot.console.offerPython();
        if isempty(python)
            fprintf(2, ['The SPOT Console was not started: it needs Python 3.9 or newer. ' ...
                        'Run SPOT again once it is installed.\n']);
            return
        end
    end
    if ~spot.console.ensureEngine(python)
        fprintf(2, ['The SPOT Console was not started: its Python needs the MATLAB Engine ' ...
                    'for Python. Run SPOT again once it is installed.\n']);
        return
    end

    % Run it in the background, logging to the cache folder.
    logFile = fullfile(cache, 'console.log');
    cmd = sprintf(['%s "%s" --local --port 0 --worker-port 0 --no-browser --root "%s" ' ...
                   '--ready-file "%s"'], python, app, root, readyFile);
    if ispc
        system(sprintf('start "SPOT Console" /b cmd /c "%s > "%s" 2>&1"', cmd, logFile));
    else
        system(sprintf('%s > "%s" 2>&1 &', cmd, logFile));
    end
    fprintf('Starting the SPOT Console (its log: %s)...\n', logFile);

    %% Connect and open when it is ready, from a timer
    % Any earlier start still waiting is dropped.
    for old = timerfind('Tag', TIMER_TAG)
        stop(old);
        delete(old);
    end
    t = timer('Tag', TIMER_TAG, 'Period', POLL_PERIOD, 'ExecutionMode', 'fixedSpacing', ...
              'TasksToExecute', ceil(START_TIMEOUT / POLL_PERIOD), 'BusyMode', 'drop');
    t.TimerFcn = @(tmr, ~) whenReady(tmr, root, logFile);
    t.StopFcn = @(tmr, ~) finish(tmr, root, logFile);
    start(t);
end

function whenReady(tmr, root, ~)
    % One poll: the server answers, so connect the worker and open the page.
    ready = spot.console.status(root);
    if isempty(ready)
        return
    end
    tmr.UserData = 'connected';
    stop(tmr);
    spot.worker.start(ready.worker_port, ready.token, root);
    spot.console.open(root);
end

function finish(tmr, ~, logFile)
    % The timer is over: say so if the server never answered, and clean up.
    if ~strcmp(tmr.UserData, 'connected')
        fprintf(2, 'The SPOT Console did not start. Its log: %s\n', logFile);
    end
    delete(tmr);
end
