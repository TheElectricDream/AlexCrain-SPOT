function w = start(port, token, root)
    %START Connect a MATLAB worker to the SPOT Console's web server.
    %   w = interface.worker.start(port, token, root) replaces any worker already running. Normally
    %   called by interface.console.start.
    %
    %   Inputs
    %     port   the console's worker port
    %     token  the session token
    %     root   project folder
    %
    %   Outputs
    %     w      the worker (interface.worker.Worker)
    %
    %   See also interface.worker.stop, interface.console.start.

    interface.worker.stop();

    % A new worker, connected and remembered as the running one.
    w = interface.worker.Worker(root);
    w.connect(port, token);
    interface.worker.Worker.instance(w);
end
