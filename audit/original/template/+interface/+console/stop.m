function stop(root)
    %STOP Disconnect the MATLAB worker and stop the project's SPOT Console web server.
    %   interface.console.stop(root)
    %
    %   Inputs
    %     root  project folder
    %
    %   See also interface.console.start, interface.worker.stop.

    %% Constants
    SHUTDOWN_TIMEOUT = 5;   % [s]

    interface.worker.stop();

    % Nothing more to do if the server isn't running.
    ready = interface.console.status(root);
    if isempty(ready)
        return
    end

    % Ask the server to shut down, with the session token.
    try
        opts = weboptions('MediaType', 'application/json', 'Timeout', SHUTDOWN_TIMEOUT, ...
            'HeaderFields', {'X-SPOT-Token', ready.token});
        webwrite(sprintf('http://127.0.0.1:%d/api/shutdown', ready.web_port), struct(), opts);
    catch
    end
end
