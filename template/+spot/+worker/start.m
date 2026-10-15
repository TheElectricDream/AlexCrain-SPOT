function w = start(port, token, root)
    %START Connect a MATLAB worker to the SPOT Console's web server.
    %   w = spot.worker.start(port, token, root) replaces any worker already running. Normally
    %   called by spot.console.start.
    %
    %   Inputs
    %     port   the console's worker port
    %     token  the session token
    %     root   project folder
    %
    %   Outputs
    %     w      the worker (spot.worker.Worker)
    %
    %   See also spot.worker.stop, spot.console.start.

    spot.worker.stop();

    % A new worker, connected and remembered as the running one.
    w = spot.worker.Worker(root);
    w.connect(port, token);
    spot.worker.Worker.instance(w);
end
