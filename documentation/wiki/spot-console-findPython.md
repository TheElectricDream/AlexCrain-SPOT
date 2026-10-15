# spot.console.findPython

**File:** `+spot/+console/findPython.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/findPython.m -->

## What it is for

The console's server is a Python 3.6+ program using only the standard library. This finds a Python to run it with: the one MATLAB is configured for (`pyenv`), then `py -3`, `python`, `python3` on Windows, or `python3`, `python` elsewhere; the first that reports version ≥ 3.6 wins. On Windows it then looks where Python's installers put it, so a Python installed while MATLAB is open (by [[installPython|spot-console-installPython]]) is found without restarting MATLAB, whose `PATH` doesn't change while it runs. Nothing found: [[start|spot-console-start]] offers to install it ([[offerPython|spot-console-offerPython]]).

## Line by line

**Lines 1–7.** Signature and help.

**Lines 8–16.** Ensure a shell; start with `pyenv`'s executable if it is set and exists (quoted, it may contain spaces).

**Lines 17–21.** The usual commands per platform; on Windows also the installed copies (`installed`, below).

**Lines 22–25.** On Linux/macOS, prefix each with `env -u LD_LIBRARY_PATH -u LD_PRELOAD` (MATLAB's library path breaks other programs).

**Lines 26–33.** Try each: run a one-liner that prints `major*100 + minor`; accept the first with status 0 and ≥ 306. (The Microsoft Store's `python` placeholder fails this test: it exits with an error instead of running.)

**Lines 34–36.** None: an error with the identifier `spot:console:noPython` (which [[start|spot-console-start]] catches) saying how to install one or point MATLAB at one.

**Lines 39–53, `installed`.** The `python.exe` files under `%LOCALAPPDATA%\Programs\Python\Python3*` (a per-user install, what `winget --scope user` and python.org's installer for "just me" make) and `%ProgramFiles%\Python3*` (for all users), newest version first (the folder names sort by version), each quoted; then the `py` launcher in either of its two places, with `-3`.

<!-- xref -->
- **Used by:** `+spot/+console/installPython.m`, `+spot/+console/offerPython.m`, `+spot/+console/start.m`, `+spot/+control/verifyConsoleController.m`
- **Uses:** [[spot.console.ensureShell|spot-console-ensureShell]], [[spot.console.installPython|spot-console-installPython]], [[spot.console.offerPython|spot-console-offerPython]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/findPython.m</b> (53 lines; click to show)</summary>

```matlab
   1  function exe = findPython()
   2  %FINDPYTHON A Python 3.6+ to run the SPOT Console's web server with.
   3  %   exe = spot.console.findPython() tries the Python MATLAB is set up to use (pyenv), then the
   4  %   usual commands (py -3 and python on Windows, python3 elsewhere), then, on Windows, where
   5  %   Python's installer puts it (so a Python installed while MATLAB runs is found without a
   6  %   restart). Errors (spot:console:noPython) if none works; spot.console.offerPython offers to
   7  %   install one.
   8  spot.console.ensureShell();
   9  candidates = {};
  10  try
  11      pe = pyenv;
  12      if strlength(pe.Executable) > 0 && isfile(pe.Executable)
  13          candidates{end + 1} = ['"' char(pe.Executable) '"'];
  14      end
  15  catch
  16  end
  17  if ispc
  18      candidates = [candidates, {'py -3', 'python', 'python3'}, installed()];
  19  else
  20      candidates = [candidates, {'python3', 'python'}];
  21  end
  22  if ~ispc
  23      % MATLAB points LD_LIBRARY_PATH at its own libraries, which can break other programs
  24      candidates = cellfun(@(c) ['env -u LD_LIBRARY_PATH -u LD_PRELOAD ' c], candidates, 'UniformOutput', false);
  25  end
  26  for k = 1:numel(candidates)
  27      [status, out] = system([candidates{k} ' -c "import sys; print(sys.version_info[0]*100+sys.version_info[1])"']);
  28      v = str2double(strtrim(out));
  29      if status == 0 && v >= 306
  30          exe = candidates{k};
  31          return
  32      end
  33  end
  34  error('spot:console:noPython', ['The SPOT Console needs Python 3.6 or newer (only its standard ' ...
  35      'library). Install it with spot.console.installPython or from python.org, or tell MATLAB where it ' ...
  36      'is with pyenv(''Version'', ...).']);
  37  end
  38  
  39  function c = installed()
  40  % Where Python's Windows installer (and winget) put it, newest first: for this user, for all
  41  % users, and the py launcher
  42  local = getenv('LOCALAPPDATA');
  43  found = [dir(fullfile(local, 'Programs', 'Python', 'Python3*', 'python.exe')); ...
  44           dir(fullfile(getenv('ProgramFiles'), 'Python3*', 'python.exe'))];
  45  [~, order] = sort({found.folder});
  46  found = found(flip(order));
  47  c = cellfun(@(f) ['"' fullfile(f, 'python.exe') '"'], {found.folder}, 'UniformOutput', false);
  48  for launcher = {fullfile(local, 'Programs', 'Python', 'Launcher', 'py.exe'), fullfile(getenv('SystemRoot'), 'py.exe')}
  49      if isfile(launcher{1})
  50          c{end + 1} = ['"' launcher{1} '" -3']; %#ok<AGROW>
  51      end
  52  end
  53  end
```

</details>
<!-- /listing -->
