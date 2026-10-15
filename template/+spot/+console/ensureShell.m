function ensureShell()
    %ENSURESHELL Make sure MATLAB's system() has a working shell.
    %   spot.console.ensureShell() points MATLAB at /bin/sh (MATLAB_SHELL) when $SHELL is not a
    %   full path to a shell. Some container and desktop launchers set SHELL=bash, and then every
    %   system() command fails with status 127 and no output.
    %
    %   See also spot.console.findPython, spot.console.openBrowser.

    % Windows, or a shell already chosen: nothing to do.
    if ispc || ~isempty(getenv('MATLAB_SHELL'))
        return
    end

    % $SHELL must be an existing absolute path.
    sh = getenv('SHELL');
    if isempty(sh) || ~startsWith(sh, '/') || ~isfile(sh)
        setenv('MATLAB_SHELL', '/bin/sh');
    end
end
