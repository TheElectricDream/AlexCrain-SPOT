# spot.console.offerPython

**File:** `+spot/+console/offerPython.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/offerPython.m -->

## What it is for

What a user sees when they type `SPOT` on a computer without Python: [[spot.console.start|spot-console-start]] calls this when [[findPython|spot-console-findPython]] finds nothing. In the MATLAB desktop it is a dialog:

> **SPOT Console: Python Needed**
> The SPOT Console (the page you run simulations and experiments from) needs Python 3.6 or newer, and none was found on this computer.
> Install Python will try to install it for you (it can take a few minutes; on Linux it asks for your password). Or download it from python.org yourself (on Windows, tick "Add python.exe to PATH" in the installer).
>
> [Install Python] [Open python.org] [Cancel]

**Install Python** runs [[installPython|spot-console-installPython]] and, if Python can be found afterwards, the console starts as if it had been there all along. **Open python.org** opens the download page in the browser. Without a desktop (`matlab -batch`, `-nodesktop`) the same explanation is printed with the command `installPython` would run.

## How to call it

```matlab
exe = spot.console.offerPython();   % the Python command to use, or '' (cancelled, or still none)
```

## Line by line

**Lines 1–6.** Signature and help.

**Lines 7–10.** The dialog's title and first paragraph; nothing found yet.

**Lines 11–20.** No desktop: ask [[installPython|spot-console-installPython]] what it *would* do (a dry run). If it can install, print the explanation and the command `spot.console.installPython` runs; if not (no `pkexec`, macOS), print its explanation instead (e.g. the `sudo` command for a terminal). Either way, add python.org, and return `''`.

**Lines 22–25.** The dialog (`questdlg`) with the three buttons, *Install Python* the default.

**Lines 27–32.** *Install Python*: a message box while it works (the dialog would otherwise look frozen for the minute an install takes), the install, and the box removed.

**Lines 33–43.** Look for Python again. Not found: tell the user why in an error dialog (the installer's message; or, if the install reported success, that MATLAB needs restarting, the rare case where neither the `PATH` nor the usual folders show the new Python yet).

**Lines 44–45.** *Open python.org*: the download page in the system browser. `exe` stays `''` and [[start|spot-console-start]] says to run `SPOT` again once it is installed. *Cancel* (or closing the dialog) does nothing.

<!-- xref -->
- **Used by:** `+spot/+console/findPython.m`, `+spot/+console/start.m`
- **Uses:** [[spot.console.findPython|spot-console-findPython]], [[spot.console.installPython|spot-console-installPython]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/offerPython.m</b> (47 lines; click to show)</summary>

```matlab
   1  function exe = offerPython()
   2  %OFFERPYTHON Ask to install Python when the SPOT Console can't find it; '' if it still can't.
   3  %   exe = spot.console.offerPython() explains that the console needs Python 3 and offers to
   4  %   install it (spot.console.installPython), to open python.org's download page, or to cancel.
   5  %   In the MATLAB desktop this is a dialog; without one (matlab -batch, -nodesktop) it prints the
   6  %   same. After an install it looks for Python again (spot.console.findPython) and returns it.
   7  TITLE = 'SPOT Console: Python Needed';
   8  INTRO = ['The SPOT Console (the page you run simulations and experiments from) needs Python 3.6 ' ...
   9           'or newer, and none was found on this computer.'];
  10  exe = '';
  11  if ~usejava('desktop')
  12      [canInstall, how] = spot.console.installPython(true);   % what it would do, without doing it
  13      if canInstall
  14          fprintf(2, '%s\nTo install it, run spot.console.installPython (it runs: %s),\n', INTRO, how);
  15      else
  16          fprintf(2, '%s\n%s\n', INTRO, how);
  17      end
  18      fprintf(2, 'or install it from https://www.python.org/downloads/, then SPOT again.\n');
  19      return
  20  end
  21  
  22  choice = questdlg(sprintf(['%s\n\nInstall Python will try to install it for you (it can take a few ' ...
  23      'minutes; on Linux it asks for your password). Or download it from python.org yourself (on ' ...
  24      'Windows, tick "Add python.exe to PATH" in the installer).'], INTRO), ...
  25      TITLE, 'Install Python', 'Open python.org', 'Cancel', 'Install Python');
  26  switch choice
  27      case 'Install Python'
  28          wait = msgbox('Installing Python. This can take a few minutes...', TITLE);
  29          [ok, message] = spot.console.installPython();
  30          if isvalid(wait)
  31              delete(wait);
  32          end
  33          try
  34              exe = spot.console.findPython();
  35          catch
  36              exe = '';
  37          end
  38          if isempty(exe)
  39              if ok
  40                  message = [message ' But it was not found afterwards: restart MATLAB, then run SPOT again.'];
  41              end
  42              uiwait(errordlg(message, TITLE, 'modal'));
  43          end
  44      case 'Open python.org'
  45          web('https://www.python.org/downloads/', '-browser');
  46  end
  47  end
```

</details>
<!-- /listing -->
