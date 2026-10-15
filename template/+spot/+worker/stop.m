function stop()
    %STOP Disconnect the MATLAB worker from the SPOT Console (if one is running).
    %   spot.worker.stop()
    %
    %   See also spot.worker.start, spot.console.stop.

    % Delete the running worker, if any, and forget it.
    w = spot.worker.Worker.instance();
    if ~isempty(w)
        delete(w);
    end
    spot.worker.Worker.instance([]);
end
