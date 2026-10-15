function url = start(root)
    %START Start the SPOT Console for a project and connect this MATLAB session to it.
    %   url = spot.console.start(root) starts the console's local web server (app.py --local,
    %   reachable from this computer only), connects the MATLAB worker that runs its commands in
    %   this session, and opens the console in the browser. A console already running for this
    %   project is reused. SPOT.m calls this; spot.console.stop stops it.
    %
    %   Inputs
    %     root  project folder
    %
    %   Outputs
    %     url   the console's address, or '' if Python is missing
    %
    %   See also spot.console.stop, spot.console.open, spot.worker.start.

    %% Constants
    START_TIMEOUT = 20;     % [s] wait for the server to answer
    POLL_PERIOD = 0.25;     % [s]

    %% Preparation
    % The old block library would shadow the blocks' classes (now spot.hardware).
    spot.console.removeOldLibraryPaths();

    % Cache folder for the ready file and log.
    cache = fullfile(root, '.spot_cache');
    if ~isfolder(cache)
        mkdir(cache);
    end
    readyFile = fullfile(cache, 'console.json');

    %% Start the server if it isn't running
    ready = spot.console.status(root);
    if isempty(ready)

        % The server picks two free ports and makes the session token, and writes them to the
        % ready file. (MATLAB's randi would give the same token in every fresh session.)
        if isfile(readyFile)
            delete(readyFile);
        end
        app = fullfile(fileparts(mfilename('fullpath')), 'webapp', 'app.py');

        % Python, offering to install it if missing.
        try
            python = spot.console.findPython();
        catch err
            if ~strcmp(err.identifier, 'spot:console:noPython')
                rethrow(err);
            end
            python = spot.console.offerPython();
            if isempty(python)
                fprintf(2, ['The SPOT Console was not started: it needs Python 3.6 or newer. ' ...
                            'Run SPOT again once it is installed.\n']);
                url = '';
                return
            end
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

        % Wait for it to answer.
        t0 = tic;
        while isempty(ready) && toc(t0) < START_TIMEOUT
            pause(POLL_PERIOD);
            ready = spot.console.status(root);
        end
        if isempty(ready)
            error('spot:console:start', 'The SPOT Console did not start. Its log: %s', logFile);
        end
    end

    %% Connect and open
    spot.worker.start(ready.worker_port, ready.token, root);
    url = spot.console.open(root);
end
