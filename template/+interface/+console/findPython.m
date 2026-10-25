function exe = findPython()
    %FINDPYTHON A Python 3.6+ to run the SPOT Console's web server with.
    %   exe = interface.console.findPython() returns the command for the first Python 3.6+ found.
    %   It tries, in order:
    %     1. the Python MATLAB is set up to use (pyenv)
    %     2. the usual commands: py -3 and python on Windows, python3 elsewhere
    %     3. on Windows, where Python's installer puts it, so a Python installed while MATLAB
    %        runs is found without a restart
    %   Errors with spot:console:noPython if none works; interface.console.offerPython offers to
    %   install one.
    %
    %   See also interface.console.offerPython, interface.console.installPython.

    %% Constants
    MIN_VERSION = 306;   % 100 * major + minor: Python 3.6

    %% Candidates
    interface.console.ensureShell();
    candidates = {};

    % MATLAB's own Python setting.
    try
        pe = pyenv;
        if strlength(pe.Executable) > 0 && isfile(pe.Executable)
            candidates{end + 1} = ['"' char(pe.Executable) '"'];
        end
    catch
    end

    % The usual commands, and on Windows the installer's folders.
    if ispc
        candidates = [candidates, {'py -3', 'python', 'python3'}, installed()];
    else
        candidates = [candidates, {'python3', 'python'}];
    end

    % MATLAB points LD_LIBRARY_PATH at its own libraries, which can break other programs.
    if ~ispc
        candidates = cellfun(@(c) ['env -u LD_LIBRARY_PATH -u LD_PRELOAD ' c], candidates, ...
            'UniformOutput', false);
    end

    %% First that runs and is new enough
    for k = 1:numel(candidates)
        [status, out] = system([candidates{k} ...
            ' -c "import sys; print(sys.version_info[0]*100+sys.version_info[1])"']);
        v = str2double(strtrim(out));
        if status == 0 && v >= MIN_VERSION
            exe = candidates{k};
            return
        end
    end

    error('spot:console:noPython', ...
        ['The SPOT Console needs Python 3.6 or newer (only its standard library). Install it ' ...
         'with interface.console.installPython or from python.org, or tell MATLAB where it is ' ...
         'with pyenv(''Version'', ...).']);
end

function c = installed()
    % Where Python's Windows installer (and winget) put it, newest first: for this user, for
    % all users, then the py launcher.
    local = getenv('LOCALAPPDATA');

    % Installed Pythons, newest version first.
    found = [dir(fullfile(local, 'Programs', 'Python', 'Python3*', 'python.exe')); ...
             dir(fullfile(getenv('ProgramFiles'), 'Python3*', 'python.exe'))];
    [~, order] = sort({found.folder});
    found = found(flip(order));
    c = cellfun(@(f) ['"' fullfile(f, 'python.exe') '"'], {found.folder}, ...
        'UniformOutput', false);

    % The py launcher, per user or system-wide.
    for launcher = {fullfile(local, 'Programs', 'Python', 'Launcher', 'py.exe'), ...
                    fullfile(getenv('SystemRoot'), 'py.exe')}
        if isfile(launcher{1})
            c{end + 1} = ['"' launcher{1} '" -3']; %#ok<AGROW>
        end
    end
end
