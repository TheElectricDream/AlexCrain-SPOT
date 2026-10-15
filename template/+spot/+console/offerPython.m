function exe = offerPython()
    %OFFERPYTHON Ask to install Python when the SPOT Console can't find it; '' if it still can't.
    %   exe = spot.console.offerPython() explains that the console needs Python 3 and offers to
    %   install it (spot.console.installPython), open python.org's download page, or cancel. In
    %   the MATLAB desktop this is a dialog; without one (-batch, -nodesktop) it prints the same.
    %
    %   Outputs
    %     exe  the Python found after an install (spot.console.findPython), or ''
    %
    %   See also spot.console.findPython, spot.console.installPython.

    %% Constants
    TITLE = 'SPOT Console: Python Needed';
    INTRO = ['The SPOT Console (the page you run simulations and experiments from) needs ' ...
             'Python 3.6 or newer, and none was found on this computer.'];

    exe = '';

    %% Without a desktop: print what to do
    if ~usejava('desktop')
        [canInstall, how] = spot.console.installPython(true);   % the command, not run
        if canInstall
            fprintf(2, '%s\nTo install it, run spot.console.installPython (it runs: %s),\n', ...
                INTRO, how);
        else
            fprintf(2, '%s\n%s\n', INTRO, how);
        end
        fprintf(2, 'or install it from https://www.python.org/downloads/, then SPOT again.\n');
        return
    end

    %% Dialog
    choice = questdlg(sprintf(['%s\n\nInstall Python will try to install it for you (it can ' ...
                               'take a few minutes; on Linux it asks for your password). Or ' ...
                               'download it from python.org yourself (on Windows, tick "Add ' ...
                               'python.exe to PATH" in the installer).'], INTRO), ...
        TITLE, 'Install Python', 'Open python.org', 'Cancel', 'Install Python');

    switch choice
        case 'Install Python'

            % Install, with a wait message.
            wait = msgbox('Installing Python. This can take a few minutes...', TITLE);
            [ok, message] = spot.console.installPython();
            if isvalid(wait)
                delete(wait);
            end

            % Look for it again; explain if it is still missing.
            try
                exe = spot.console.findPython();
            catch
                exe = '';
            end
            if isempty(exe)
                if ok
                    message = [message ' But it was not found afterwards: restart MATLAB, ' ...
                               'then run SPOT again.'];
                end
                uiwait(errordlg(message, TITLE, 'modal'));
            end

        case 'Open python.org'
            web('https://www.python.org/downloads/', '-browser');
    end
end
