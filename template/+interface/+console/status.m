function ready = status(root)
    %STATUS The project's running SPOT Console (ports and token), or [] if none answers.
    %   ready = interface.console.status(root) reads .spot_cache/console.json, written by the
    %   console's web server when it starts, and checks that the server still answers.
    %
    %   Inputs
    %     root   project folder
    %
    %   Outputs
    %     ready  struct with web_port, worker_port and token, or [] if not running
    %
    %   See also interface.console.start, interface.console.stop.

    %% Constants
    PING_TIMEOUT = 2;   % [s]

    ready = [];

    % The ready file the server writes.
    file = fullfile(root, '.spot_cache', 'console.json');
    if ~isfile(file)
        return
    end

    % Only report it if the server answers a ping.
    try
        info = jsondecode(fileread(file));
        webread(sprintf('http://127.0.0.1:%d/api/ping', info.web_port), ...
            weboptions('Timeout', PING_TIMEOUT));
        ready = info;
    catch
    end
end
