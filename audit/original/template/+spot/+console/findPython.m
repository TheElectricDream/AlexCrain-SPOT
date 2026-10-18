function exe = findPython()
    %FINDPYTHON A Python to run the SPOT Console's web server with.
    %   exe = spot.console.findPython() returns the command for a Python the local console can
    %   use: one that already has the MATLAB Engine for Python (the console reads Constants
    %   from this session through it), else the first one new enough for the engine
    %   (MIN_VERSION), which spot.console.ensureEngine then installs it into. It tries, in
    %   order:
    %     1. the Python MATLAB is set up to use (pyenv)
    %     2. the usual commands: py -3 and python on Windows, python3 elsewhere
    %     3. on Windows, where Python's installer puts it, so a Python installed while MATLAB
    %        runs is found without a restart
    %   Errors with spot:console:noPython if none works; spot.console.offerPython offers to
    %   install one.
    %
    %   See also spot.console.ensureEngine, spot.console.offerPython,
    %   spot.console.installPython.

    %% Constants
    MIN_VERSION = 309;   % 100 * major + minor: Python 3.9, the oldest the engine supports

    %% Candidates
    spot.console.ensureShell();
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

    %% The first with the engine, else the first new enough
    usable = {};
    for k = 1:numel(candidates)
        [status, out] = system([candidates{k} ...
            ' -c "import sys; print(sys.version_info[0]*100+sys.version_info[1])"']);
        v = str2double(strtrim(out));
        if status == 0 && v >= MIN_VERSION
            if spot.console.hasEngine(candidates{k})
                exe = candidates{k};
                return
            end
            usable{end + 1} = candidates{k}; %#ok<AGROW>
        end
    end
    if ~isempty(usable)
        exe = usable{1};
        return
    end

    error('spot:console:noPython', ...
        ['The SPOT Console needs Python 3.9 or newer with the MATLAB Engine for Python. ' ...
         'Install Python with spot.console.installPython or from python.org, or tell MATLAB ' ...
         'where it is with pyenv(''Version'', ...); SPOT then offers to install the engine.']);
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
