# spot.console.ensureShell

**File:** `+spot/+console/ensureShell.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/ensureShell.m -->

## What it is for

MATLAB runs `system()` commands through `$SHELL`. Some launchers set `SHELL=bash` (a name, not a path), and then every command fails with status 127 and no output. This points MATLAB at `/bin/sh` through `MATLAB_SHELL` when `$SHELL` isn't a full path to an existing file.

## Line by line

**Lines 6–8.** Nothing to do on Windows, or if `MATLAB_SHELL` is already set.

**Lines 9–12.** If `$SHELL` is empty, not absolute, or not a file: set `MATLAB_SHELL`.

<!-- xref -->
- **Used by:** `+spot/+console/findPython.m`, `+spot/+console/openBrowser.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/ensureShell.m</b> (13 lines; click to show)</summary>

```matlab
   1  function ensureShell()
   2  %ENSURESHELL Make sure MATLAB's system() has a working shell.
   3  %   MATLAB runs system() commands with $SHELL. If that isn't a full path to a shell (some
   4  %   container and desktop launchers set SHELL=bash), every command fails with status 127 and no
   5  %   output. spot.console.ensureShell() then points MATLAB at /bin/sh (MATLAB_SHELL).
   6  if ispc || ~isempty(getenv('MATLAB_SHELL'))
   7      return
   8  end
   9  sh = getenv('SHELL');
  10  if isempty(sh) || ~startsWith(sh, '/') || ~isfile(sh)
  11      setenv('MATLAB_SHELL', '/bin/sh');
  12  end
  13  end
```

</details>
<!-- /listing -->
