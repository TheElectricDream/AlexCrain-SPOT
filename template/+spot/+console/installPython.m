function [ok, message] = installPython(dryRun)
%INSTALLPYTHON Try to install Python 3 for the SPOT Console (Windows or Linux).
%   [ok, message] = spot.console.installPython() installs Python with what the computer has:
%     Windows  winget (Windows 10 and 11), for this user only; without winget, Python's own
%              installer, downloaded from python.org and run silently for this user. Neither
%              needs administrator rights.
%     Linux    the distribution's package manager (apt, dnf, yum, zypper or pacman), asking for
%              the administrator password in a system dialog (pkexec). Without one, message is
%              the command to run in a terminal.
%   ok is true when the install ran without error; message says what happened, or what to do.
%   spot.console.findPython finds the new Python afterwards, without restarting MATLAB.
%   [~, command] = spot.console.installPython(true) only returns the command it would run.
if nargin < 1
    dryRun = false;
end
VERSION = '3.12.10';   % python.org's installer, when there is no winget

if ispc
    [noWinget, ~] = system('winget --version');
    if ~noWinget
        command = ['winget install --id Python.Python.3.12 --exact --scope user --silent ' ...
                   '--accept-package-agreements --accept-source-agreements'];
    else
        installer = fullfile(tempdir, sprintf('python-%s-amd64.exe', VERSION));
        url = sprintf('https://www.python.org/ftp/python/%s/python-%s-amd64.exe', VERSION, VERSION);
        command = sprintf('"%s" /quiet InstallAllUsers=0 PrependPath=1 Include_launcher=1 Include_test=0', installer);
        if ~dryRun
            try
                websave(installer, url);
            catch err
                ok = false;
                message = sprintf('Could not download Python''s installer (%s): %s', url, err.message);
                return
            end
        end
    end
elseif isunix && ~ismac
    managers = {'apt-get', 'apt-get update && apt-get install -y python3'
                'dnf',     'dnf install -y python3'
                'yum',     'yum install -y python3'
                'zypper',  'zypper --non-interactive install python3'
                'pacman',  'pacman -S --noconfirm --needed python'};
    clean = 'env -u LD_LIBRARY_PATH -u LD_PRELOAD ';   % MATLAB's libraries can break other programs
    k = find(cellfun(@(m) system([clean 'sh -c "command -v ' m ' >/dev/null"']) == 0, managers(:, 1)), 1);
    if isempty(k)
        ok = false;
        message = 'No known package manager was found: install Python 3 with your distribution''s tools.';
        return
    end
    [~, uid] = system('id -u');
    if strcmp(strtrim(uid), '0')
        command = [clean 'sh -c "' managers{k, 2} '"'];
    elseif system([clean 'sh -c "command -v pkexec >/dev/null"']) == 0 && ...
            (~isempty(getenv('DISPLAY')) || ~isempty(getenv('WAYLAND_DISPLAY')))
        command = [clean 'pkexec sh -c "' managers{k, 2} '"'];
    else
        ok = false;
        message = sprintf('Run this in a terminal (it asks for your password), then SPOT again:\n  sudo sh -c "%s"', managers{k, 2});
        return
    end
else
    ok = false;
    message = 'Install Python 3 from https://www.python.org/downloads/ (installing it automatically is only done on Windows and Linux).';
    return
end

if dryRun
    ok = true;
    message = command;
    return
end
[status, out] = system(command);
ok = status == 0;
if ok
    message = 'Python was installed.';
else
    lines = splitlines(strtrim(out));
    message = sprintf('The install did not finish (exit code %d): %s\nCommand: %s', status, ...
        strjoin(lines(max(1, end - 4):end), ' '), command);
end
end
