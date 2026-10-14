function ensureShell()
%ENSURESHELL Make sure MATLAB's system() has a working shell.
%   MATLAB runs system() commands with $SHELL. If that isn't a full path to a shell (some
%   container and desktop launchers set SHELL=bash), every command fails with status 127 and no
%   output. spot.console.ensureShell() then points MATLAB at /bin/sh (MATLAB_SHELL).
if ispc || ~isempty(getenv('MATLAB_SHELL'))
    return
end
sh = getenv('SHELL');
if isempty(sh) || ~startsWith(sh, '/') || ~isfile(sh)
    setenv('MATLAB_SHELL', '/bin/sh');
end
end
