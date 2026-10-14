function url = start(root)
%START Start the SPOT Console for a project and connect this MATLAB session to it.
%   url = spot.console.start(projectFolder) starts the console's local web server (Python,
%   +spot/+console/webapp/app.py --local, reachable from this computer only), connects the
%   MATLAB worker that runs its commands in this session, and opens the console in the browser.
%   If the console is already running for this project, it is reused. Stop it with
%   spot.console.stop. SPOT.m in the project folder calls this.
spot.console.removeOldLibraryPaths();   % (the old block library; its code is spot.hardware now)
cache = fullfile(root, '.spot_cache');
if ~isfolder(cache)
    mkdir(cache);
end
readyFile = fullfile(cache, 'console.json');

ready = spot.console.status(root);
if isempty(ready)
    % Start the web server; it picks two free ports and makes the session token (os.urandom),
    % and writes them to the ready file. The token isn't made here: MATLAB's randi starts from
    % the same state in every fresh session, and would advance the user's random stream.
    if isfile(readyFile)
        delete(readyFile);
    end
    app = fullfile(fileparts(mfilename('fullpath')), 'webapp', 'app.py');   % next to this file
    try
        python = spot.console.findPython();
    catch err
        if ~strcmp(err.identifier, 'spot:console:noPython')
            rethrow(err);
        end
        python = spot.console.offerPython();   % explain, and offer to install it
        if isempty(python)
            fprintf(2, 'The SPOT Console was not started: it needs Python 3.6 or newer. Run SPOT again once it is installed.\n');
            url = '';
            return
        end
    end
    logFile = fullfile(cache, 'console.log');
    cmd = sprintf('%s "%s" --local --port 0 --worker-port 0 --no-browser --root "%s" --ready-file "%s"', ...
        python, app, root, readyFile);
    if ispc
        system(sprintf('start "SPOT Console" /b cmd /c "%s > "%s" 2>&1"', cmd, logFile));
    else
        system(sprintf('%s > "%s" 2>&1 &', cmd, logFile));
    end
    t0 = tic;
    while isempty(ready) && toc(t0) < 20
        pause(0.25);
        ready = spot.console.status(root);
    end
    if isempty(ready)
        error('spot:console:start', 'The SPOT Console did not start. Its log: %s', logFile);
    end
end

spot.worker.start(ready.worker_port, ready.token, root);
url = spot.console.open(root);
end
