# spot.console.start

**File:** `+spot/+console/start.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/start.m -->

## What it is for

What `SPOT` does: start the console's web server for this project (a Python process, local only), connect the MATLAB worker to it, and open the page in the browser. If the console is already running for this project it is reused.

## How to call it

```matlab
url = spot.console.start(projectFolder);
```

## Line by line

**Line 8.** Take the old block library off the path, in case an old project put it there (its class names would clash with the diagram's blocks).

**Lines 9–13.** Make `.spot_cache/`; the "ready file" the server writes when it is up.

**Lines 15–53.** If no console answers ([[status|spot-console-status]]): delete a stale ready file; find Python ([[findPython|spot-console-findPython]]). If there is none (lines 26–36, the error `spot:console:noPython`; any other error is passed on), [[offerPython|spot-console-offerPython]] explains it and offers to install it; if there is still none, say so and return `''` without starting anything. Then the command line for `app.py` with `--local`, port 0 (pick free ports), no browser, the project folder and the ready file; start it in the background with its output going to `.spot_cache/console.log` (`start /b` on Windows, `&` elsewhere); then wait up to 20 s for the ready file, polling every 0.25 s; give up with a pointer to the log. The server makes the session token (from `os.urandom`) rather than MATLAB, because MATLAB's random generator starts from the same state in every fresh session and using it would also disturb the user's random stream.

**Lines 55–56.** Connect the worker with the port and token from the ready file ([[spot.worker.start|spot-worker-start]]), and open the browser ([[open|spot-console-open]]).

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/local.py`, `+spot/+worker/start.m`, `SPOT.m`
- **Uses:** [[spot.console.findPython|spot-console-findPython]], [[spot.console.offerPython|spot-console-offerPython]], [[spot.console.open|spot-console-open]], [[spot.console.removeOldLibraryPaths|spot-console-removeOldLibraryPaths]], [[spot.console.status|spot-console-status]], [[spot.console.stop|spot-console-stop]], [[spot.worker.start|spot-worker-start]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/start.m</b> (57 lines; click to show)</summary>

```matlab
   1  function url = start(root)
   2  %START Start the SPOT Console for a project and connect this MATLAB session to it.
   3  %   url = spot.console.start(projectFolder) starts the console's local web server (Python,
   4  %   +spot/+console/webapp/app.py --local, reachable from this computer only), connects the
   5  %   MATLAB worker that runs its commands in this session, and opens the console in the browser.
   6  %   If the console is already running for this project, it is reused. Stop it with
   7  %   spot.console.stop. SPOT.m in the project folder calls this.
   8  spot.console.removeOldLibraryPaths();   % (the old block library; its code is spot.hardware now)
   9  cache = fullfile(root, '.spot_cache');
  10  if ~isfolder(cache)
  11      mkdir(cache);
  12  end
  13  readyFile = fullfile(cache, 'console.json');
  14  
  15  ready = spot.console.status(root);
  16  if isempty(ready)
  17      % Start the web server; it picks two free ports and makes the session token (os.urandom),
  18      % and writes them to the ready file. The token isn't made here: MATLAB's randi starts from
  19      % the same state in every fresh session, and would advance the user's random stream.
  20      if isfile(readyFile)
  21          delete(readyFile);
  22      end
  23      app = fullfile(fileparts(mfilename('fullpath')), 'webapp', 'app.py');   % next to this file
  24      try
  25          python = spot.console.findPython();
  26      catch err
  27          if ~strcmp(err.identifier, 'spot:console:noPython')
  28              rethrow(err);
  29          end
  30          python = spot.console.offerPython();   % explain, and offer to install it
  31          if isempty(python)
  32              fprintf(2, 'The SPOT Console was not started: it needs Python 3.6 or newer. Run SPOT again once it is installed.\n');
  33              url = '';
  34              return
  35          end
  36      end
  37      logFile = fullfile(cache, 'console.log');
  38      cmd = sprintf('%s "%s" --local --port 0 --worker-port 0 --no-browser --root "%s" --ready-file "%s"', ...
  39          python, app, root, readyFile);
  40      if ispc
  41          system(sprintf('start "SPOT Console" /b cmd /c "%s > "%s" 2>&1"', cmd, logFile));
  42      else
  43          system(sprintf('%s > "%s" 2>&1 &', cmd, logFile));
  44      end
  45      t0 = tic;
  46      while isempty(ready) && toc(t0) < 20
  47          pause(0.25);
  48          ready = spot.console.status(root);
  49      end
  50      if isempty(ready)
  51          error('spot:console:start', 'The SPOT Console did not start. Its log: %s', logFile);
  52      end
  53  end
  54  
  55  spot.worker.start(ready.worker_port, ready.token, root);
  56  url = spot.console.open(root);
  57  end
```

</details>
<!-- /listing -->
