function stop()
    %STOP Disconnect the MATLAB worker from the SPOT Console (if one is running).
    %   interface.worker.stop()
    %
    %   See also interface.worker.start, interface.console.stop.

    % Delete the running worker, if any, and forget it.
    w = interface.worker.Worker.instance();
    if ~isempty(w)
        delete(w);
    end
    interface.worker.Worker.instance([]);
end
