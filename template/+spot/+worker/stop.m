function stop()
%STOP Disconnect the MATLAB worker from the SPOT Console (if one is running).
w = spot.worker.Worker.instance();
if ~isempty(w)
    delete(w);
end
spot.worker.Worker.instance([]);
end
