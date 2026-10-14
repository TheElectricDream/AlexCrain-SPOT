function exe = offerPython()
%OFFERPYTHON Ask to install Python when the SPOT Console can't find it; '' if it still can't.
%   exe = spot.console.offerPython() explains that the console needs Python 3 and offers to
%   install it (spot.console.installPython), to open python.org's download page, or to cancel.
%   In the MATLAB desktop this is a dialog; without one (matlab -batch, -nodesktop) it prints the
%   same. After an install it looks for Python again (spot.console.findPython) and returns it.
TITLE = 'SPOT Console: Python Needed';
INTRO = ['The SPOT Console (the page you run simulations and experiments from) needs Python 3.6 ' ...
         'or newer, and none was found on this computer.'];
exe = '';
if ~usejava('desktop')
    [canInstall, how] = spot.console.installPython(true);   % what it would do, without doing it
    if canInstall
        fprintf(2, '%s\nTo install it, run spot.console.installPython (it runs: %s),\n', INTRO, how);
    else
        fprintf(2, '%s\n%s\n', INTRO, how);
    end
    fprintf(2, 'or install it from https://www.python.org/downloads/, then SPOT again.\n');
    return
end

choice = questdlg(sprintf(['%s\n\nInstall Python will try to install it for you (it can take a few ' ...
    'minutes; on Linux it asks for your password). Or download it from python.org yourself (on ' ...
    'Windows, tick "Add python.exe to PATH" in the installer).'], INTRO), ...
    TITLE, 'Install Python', 'Open python.org', 'Cancel', 'Install Python');
switch choice
    case 'Install Python'
        wait = msgbox('Installing Python. This can take a few minutes...', TITLE);
        [ok, message] = spot.console.installPython();
        if isvalid(wait)
            delete(wait);
        end
        try
            exe = spot.console.findPython();
        catch
            exe = '';
        end
        if isempty(exe)
            if ok
                message = [message ' But it was not found afterwards: restart MATLAB, then run SPOT again.'];
            end
            uiwait(errordlg(message, TITLE, 'modal'));
        end
    case 'Open python.org'
        web('https://www.python.org/downloads/', '-browser');
end
end
