function ok = ensureEngine(python, ask)
    %ENSUREENGINE Make sure a Python has the MATLAB Engine for Python, installing it if asked.
    %   ok = spot.console.ensureEngine(python) returns true when "import matlab.engine" works
    %   in that Python. Otherwise it offers to install the package MATLAB ships
    %   (<matlabroot>/extern/engines/python) for this user only, with that Python's pip, or
    %   without pip (a distribution's python3 often has none) by copying the package into the
    %   user's site-packages (install_engine.py, which does what pip's install does): a
    %   dialog in the MATLAB desktop, a question in the Command Window without one.
    %   ok = spot.console.ensureEngine(python, false) installs without asking.
    %
    %   The local console reads the project's Constants from this MATLAB session through the
    %   engine (spot_console/constants.py), so it can't start without it. The engine package
    %   supports the Python versions of its MATLAB release (R2026a: 3.9 to 3.13);
    %   spot.console.findPython prefers a Python that has it or can take it.
    %
    %   Inputs
    %     python  the Python command (spot.console.findPython)
    %     ask     whether to ask before installing (default true)
    %
    %   Outputs
    %     ok      whether the engine imports now
    %
    %   See also spot.console.findPython, spot.console.start.

    %% Constants
    TITLE = 'SPOT Console: MATLAB Engine for Python Needed';
    SOURCE = fullfile(matlabroot, 'extern', 'engines', 'python');
    COPIER = fullfile(fileparts(mfilename('fullpath')), 'install_engine.py');

    if nargin < 2
        ask = true;
    end
    ok = spot.console.hasEngine(python);
    if ok
        return
    end

    %% Ask
    command = sprintf('%s -m pip install --user "%s"', python, SOURCE);
    message = sprintf(['The SPOT Console reads the project''s Constants from this MATLAB ' ...
                       'session, through the MATLAB Engine for Python, which the Python it ' ...
                       'found (%s) doesn''t have yet.\n\nInstall it now? It runs:\n  %s'], ...
                      python, command);
    if ask
        if usejava('desktop')
            choice = questdlg(message, TITLE, 'Install', 'Cancel', 'Install');
            install = strcmp(choice, 'Install');
        else
            answer = input([message '  [y/n] '], 's');
            install = strncmpi(strtrim(answer), 'y', 1);
        end
        if ~install
            fprintf(2, 'Install it yourself with:\n  %s\n', command);
            return
        end
    end

    %% Install: with pip, else by copying the package
    fprintf('Installing the MATLAB Engine for Python: %s\n', command);
    [status, out] = system(command);
    if status ~= 0 && contains(out, 'No module named pip')
        command = sprintf('%s "%s" "%s"', python, COPIER, matlabroot);
        fprintf('This Python has no pip: copying the package instead: %s\n', command);
        [status, out] = system(command);
    end
    if status ~= 0
        fprintf(2, '%s\n', out);
        fprintf(2, ['The install failed. The engine supports Python 3.9 to 3.13 for this ' ...
                    'MATLAB; point MATLAB at such a Python with pyenv(''Version'', ...) or ' ...
                    'install one, then SPOT again.\n']);
        return
    end
    fprintf('%s', out);
    ok = spot.console.hasEngine(python);
    if ~ok
        fprintf(2, 'The engine installed but does not import: %s\n', out);
    end
end
