function url = open(root, show)
    %OPEN Open (or just show the address of) the project's running SPOT Console.
    %   url = spot.console.open(root) opens the console in the browser and prints its address.
    %   url = spot.console.open(root, false) only prints it.
    %
    %   Inputs
    %     root  project folder
    %     show  open the browser (default true)
    %
    %   Outputs
    %     url   the console's address, with this session's token (the console only accepts
    %           requests from pages opened with it)
    %
    %   See also spot.console.start, spot.console.status.

    %% Defaults
    if nargin < 2
        show = true;
    end

    %% Address
    ready = spot.console.status(root);
    if isempty(ready)
        error('spot:console:open', 'The SPOT Console is not running. Start it with SPOT.');
    end
    url = sprintf('http://127.0.0.1:%d/?token=%s#simulation', ready.web_port, ready.token);

    %% Print and open
    fprintf(['SPOT Console: <a href="matlab:spot.console.openBrowser(''%s'');">open in the ' ...
             'browser</a>  (or paste: %s )\n'], url, url);
    if show && ~spot.console.openBrowser(url)
        fprintf('Could not start a browser: paste the address above into one.\n');
    end
end
