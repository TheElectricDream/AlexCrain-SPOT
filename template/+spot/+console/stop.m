function stop(root)
%STOP Disconnect the MATLAB worker and stop the project's SPOT Console web server.
%   spot.console.stop(projectFolder)
spot.worker.stop();
ready = spot.console.status(root);
if isempty(ready)
    return
end
try
    opts = weboptions('MediaType', 'application/json', 'Timeout', 5, ...
        'HeaderFields', {'X-SPOT-Token', ready.token});
    webwrite(sprintf('http://127.0.0.1:%d/api/shutdown', ready.web_port), struct(), opts);
catch
end
end
