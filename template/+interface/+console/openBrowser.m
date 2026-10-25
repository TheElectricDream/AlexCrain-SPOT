function ok = openBrowser(url)
    %OPENBROWSER Open a URL in the computer's default web browser.
    %   ok = interface.console.openBrowser(url) opens url, falling back to MATLAB's web(). On Linux
    %   it uses xdg-open, which also works when MATLAB runs in a container (distrobox forwards
    %   it to the host), where web(url, '-browser') fails.
    %
    %   Outputs
    %     ok  false if no browser could be started
    %
    %   See also interface.console.open.

    ok = false;
    interface.console.ensureShell();

    %% Platform's own opener
    if isunix && ~ismac

        % xdg-open, without MATLAB's LD_LIBRARY_PATH (it can break the browser).
        [status, ~] = system('command -v xdg-open');
        if status == 0
            system(sprintf( ...
                'env -u LD_LIBRARY_PATH -u LD_PRELOAD xdg-open "%s" > /dev/null 2>&1 &', url));
            ok = true;
            return
        end
    elseif ismac
        ok = system(sprintf('open "%s"', url)) == 0;
        if ok
            return
        end
    end

    %% Fallback: MATLAB's web(), with its warnings silenced
    % With '-browser', web() returns only a status: 0 opened, 1 no browser, 2 browser failed.
    try
        w = warning('off', 'all');
        restore = onCleanup(@() warning(w));
        status = web(url, '-browser');
        ok = status == 0;
    catch
    end
end
