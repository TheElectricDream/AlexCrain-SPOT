function w = start(port, token, root)
%START Connect a MATLAB worker to the SPOT Console's web server.
%   w = spot.worker.start(workerPort, token, projectFolder). Replaces any worker already running.
%   Normally called by spot.console.start.
spot.worker.stop();
w = spot.worker.Worker(root);
w.connect(port, token);
spot.worker.Worker.instance(w);
end
