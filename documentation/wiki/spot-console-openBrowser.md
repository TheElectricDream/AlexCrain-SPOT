# spot.console.openBrowser

**File:** `+spot/+console/openBrowser.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/openBrowser.m -->

## What it is for

Open a URL in the computer's default browser. MATLAB's own `web(url, '-browser')` fails when MATLAB runs in a container (distrobox), where `xdg-open` still works because it is forwarded to the host, so on Linux that is tried first.

## Line by line

**Lines 6–7.** Not opened yet; make sure `system()` has a shell ([[ensureShell|spot-console-ensureShell]]).

**Lines 8–15.** Linux: if `xdg-open` exists, run it in the background with MATLAB's `LD_LIBRARY_PATH` and `LD_PRELOAD` removed (MATLAB points them at its own libraries, which can break the browser).

**Lines 16–20.** macOS: `open`.

**Lines 22–28.** Otherwise (Windows, or the above failed) MATLAB's `web`, with warnings silenced.

<!-- xref -->
- **Used by:** `+spot/+console/open.m`
- **Uses:** [[spot.console.ensureShell|spot-console-ensureShell]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/openBrowser.m</b> (29 lines; click to show)</summary>

```matlab
   1  function ok = openBrowser(url)
   2  %OPENBROWSER Open a URL in the computer's default web browser.
   3  %   ok = spot.console.openBrowser(url). On Linux this uses xdg-open, which also works when
   4  %   MATLAB runs in a container (distrobox forwards it to the host's browser), where MATLAB's own
   5  %   web(url, '-browser') fails. Falls back to web(). Returns false if nothing could be started.
   6  ok = false;
   7  spot.console.ensureShell();
   8  if isunix && ~ismac
   9      % MATLAB points LD_LIBRARY_PATH at its own libraries, which can break the browser
  10      [status, ~] = system('command -v xdg-open');
  11      if status == 0
  12          system(sprintf('env -u LD_LIBRARY_PATH -u LD_PRELOAD xdg-open "%s" > /dev/null 2>&1 &', url));
  13          ok = true;
  14          return
  15      end
  16  elseif ismac
  17      ok = system(sprintf('open "%s"', url)) == 0;
  18      if ok
  19          return
  20      end
  21  end
  22  try
  23      w = warning('off', 'all');
  24      restore = onCleanup(@() warning(w));
  25      [status, ~] = web(url, '-browser');
  26      ok = status == 0;
  27  catch
  28  end
  29  end
```

</details>
<!-- /listing -->
