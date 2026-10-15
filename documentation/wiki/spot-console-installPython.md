# spot.console.installPython

**File:** `+spot/+console/installPython.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/installPython.m -->

## What it is for

The console's server is a Python program, and a test user may not have Python or know how to install it. When [[SPOT|spot-console-start]] can't find one, [[offerPython|spot-console-offerPython]] offers to install it, and this does the installing, with whatever the computer has:

| Computer | How | Rights needed |
|---|---|---|
| Windows 10/11 with `winget` | `winget install --id Python.Python.3.12 --exact --scope user --silent` | none (installed for this user) |
| Windows without `winget` | downloads python.org's 3.12.10 installer and runs it with `/quiet InstallAllUsers=0 PrependPath=1 Include_launcher=1` | none (installed for this user) |
| Linux | the distribution's package manager (`apt-get`, `dnf`, `yum`, `zypper` or `pacman`) through `pkexec`, which asks for the administrator password in a system dialog | the administrator password |
| Linux without `pkexec` or a display | nothing; returns the `sudo` command to run in a terminal | |
| macOS | nothing; returns python.org's address | |

## How to call it

```matlab
[ok, message] = spot.console.installPython();         % install
[ok, command] = spot.console.installPython(true);     % only say what it would run (ok false: it can't)
```

Afterwards [[findPython|spot-console-findPython]] finds the new Python without restarting MATLAB (on Windows it also looks where the installers put Python, since MATLAB's `PATH` doesn't change while it runs).

## Line by line

**Lines 1–12.** Signature and help. `dryRun` is optional.

**Lines 13–16.** `dryRun` defaults to false. `VERSION` is the python.org installer used when there is no `winget`.

**Lines 18–22.** Windows: `system('winget --version')` returns 0 when `winget` exists (it comes with Windows 10 and 11 through the App Installer). Then the command installs the Python 3.12 package for this user only (`--scope user`), silently, accepting the store's and the package's agreements so it doesn't stop to ask.

**Lines 23–36.** No `winget`: the installer's path in the temporary folder and its python.org address; the command runs it quietly for this user, adding Python to the user's `PATH` and installing the `py` launcher (the first thing [[findPython|spot-console-findPython]] tries on Windows). Unless it's a dry run, download it now (`websave`); a failed download (no internet, a proxy) returns its reason.

**Lines 37–42.** Linux: the package managers to look for and the command for each. `apt-get update` first, since a fresh system's package lists may be empty.

**Lines 43–49.** `clean` strips MATLAB's `LD_LIBRARY_PATH` and `LD_PRELOAD` from the commands (MATLAB's own libraries break system programs). Find the first manager on the system (`command -v` succeeds); none → say so.

**Lines 50–60.** Installing packages needs root. Already root (`id -u` is 0, e.g. in a container): run the command directly. Otherwise, with `pkexec` and a display, run it through `pkexec`, which shows the desktop's password dialog. Otherwise (a remote terminal, no polkit), return the `sudo` command for the user to run themselves.

**Lines 61–65.** macOS (or anything else): no automatic install; point at python.org.

**Lines 67–71.** A dry run stops here and returns the command.

**Lines 72–80.** Run it. Success is exit code 0. On failure, the message has the exit code, the last five lines of the output and the command, so the user can see why and run it by hand (`winget` also returns non-zero when the package is already installed; [[offerPython|spot-console-offerPython]] then just finds it).

<!-- xref -->
- **Used by:** `+spot/+console/findPython.m`, `+spot/+console/offerPython.m`
- **Uses:** [[spot.console.findPython|spot-console-findPython]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/installPython.m</b> (81 lines; click to show)</summary>

```matlab
   1  function [ok, message] = installPython(dryRun)
   2  %INSTALLPYTHON Try to install Python 3 for the SPOT Console (Windows or Linux).
   3  %   [ok, message] = spot.console.installPython() installs Python with what the computer has:
   4  %     Windows  winget (Windows 10 and 11), for this user only; without winget, Python's own
   5  %              installer, downloaded from python.org and run silently for this user. Neither
   6  %              needs administrator rights.
   7  %     Linux    the distribution's package manager (apt, dnf, yum, zypper or pacman), asking for
   8  %              the administrator password in a system dialog (pkexec). Without one, message is
   9  %              the command to run in a terminal.
  10  %   ok is true when the install ran without error; message says what happened, or what to do.
  11  %   spot.console.findPython finds the new Python afterwards, without restarting MATLAB.
  12  %   [~, command] = spot.console.installPython(true) only returns the command it would run.
  13  if nargin < 1
  14      dryRun = false;
  15  end
  16  VERSION = '3.12.10';   % python.org's installer, when there is no winget
  17  
  18  if ispc
  19      [noWinget, ~] = system('winget --version');
  20      if ~noWinget
  21          command = ['winget install --id Python.Python.3.12 --exact --scope user --silent ' ...
  22                     '--accept-package-agreements --accept-source-agreements'];
  23      else
  24          installer = fullfile(tempdir, sprintf('python-%s-amd64.exe', VERSION));
  25          url = sprintf('https://www.python.org/ftp/python/%s/python-%s-amd64.exe', VERSION, VERSION);
  26          command = sprintf('"%s" /quiet InstallAllUsers=0 PrependPath=1 Include_launcher=1 Include_test=0', installer);
  27          if ~dryRun
  28              try
  29                  websave(installer, url);
  30              catch err
  31                  ok = false;
  32                  message = sprintf('Could not download Python''s installer (%s): %s', url, err.message);
  33                  return
  34              end
  35          end
  36      end
  37  elseif isunix && ~ismac
  38      managers = {'apt-get', 'apt-get update && apt-get install -y python3'
  39                  'dnf',     'dnf install -y python3'
  40                  'yum',     'yum install -y python3'
  41                  'zypper',  'zypper --non-interactive install python3'
  42                  'pacman',  'pacman -S --noconfirm --needed python'};
  43      clean = 'env -u LD_LIBRARY_PATH -u LD_PRELOAD ';   % MATLAB's libraries can break other programs
  44      k = find(cellfun(@(m) system([clean 'sh -c "command -v ' m ' >/dev/null"']) == 0, managers(:, 1)), 1);
  45      if isempty(k)
  46          ok = false;
  47          message = 'No known package manager was found: install Python 3 with your distribution''s tools.';
  48          return
  49      end
  50      [~, uid] = system('id -u');
  51      if strcmp(strtrim(uid), '0')
  52          command = [clean 'sh -c "' managers{k, 2} '"'];
  53      elseif system([clean 'sh -c "command -v pkexec >/dev/null"']) == 0 && ...
  54              (~isempty(getenv('DISPLAY')) || ~isempty(getenv('WAYLAND_DISPLAY')))
  55          command = [clean 'pkexec sh -c "' managers{k, 2} '"'];
  56      else
  57          ok = false;
  58          message = sprintf('Run this in a terminal (it asks for your password), then SPOT again:\n  sudo sh -c "%s"', managers{k, 2});
  59          return
  60      end
  61  else
  62      ok = false;
  63      message = 'Install Python 3 from https://www.python.org/downloads/ (installing it automatically is only done on Windows and Linux).';
  64      return
  65  end
  66  
  67  if dryRun
  68      ok = true;
  69      message = command;
  70      return
  71  end
  72  [status, out] = system(command);
  73  ok = status == 0;
  74  if ok
  75      message = 'Python was installed.';
  76  else
  77      lines = splitlines(strtrim(out));
  78      message = sprintf('The install did not finish (exit code %d): %s\nCommand: %s', status, ...
  79          strjoin(lines(max(1, end - 4):end), ' '), command);
  80  end
  81  end
```

</details>
<!-- /listing -->
