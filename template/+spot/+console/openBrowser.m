function ok = openBrowser(url)
%OPENBROWSER Open a URL in the computer's default web browser.
%   ok = spot.console.openBrowser(url). On Linux this uses xdg-open, which also works when
%   MATLAB runs in a container (distrobox forwards it to the host's browser), where MATLAB's own
%   web(url, '-browser') fails. Falls back to web(). Returns false if nothing could be started.
ok = false;
spot.console.ensureShell();
if isunix && ~ismac
    % MATLAB points LD_LIBRARY_PATH at its own libraries, which can break the browser
    [status, ~] = system('command -v xdg-open');
    if status == 0
        system(sprintf('env -u LD_LIBRARY_PATH -u LD_PRELOAD xdg-open "%s" > /dev/null 2>&1 &', url));
        ok = true;
        return
    end
elseif ismac
    ok = system(sprintf('open "%s"', url)) == 0;
    if ok
        return
    end
end
try
    w = warning('off', 'all');
    restore = onCleanup(@() warning(w));
    [status, ~] = web(url, '-browser');
    ok = status == 0;
catch
end
end
