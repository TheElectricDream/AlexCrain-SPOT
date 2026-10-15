# spot.console.stop

**File:** `+spot/+console/stop.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/stop.m -->

## What it is for

`SPOT stop`: disconnect the worker and ask the server to shut down.

## Line by line

**Line 4.** Stop the worker.

**Lines 5–8.** If no server answers there is nothing more to do.

**Lines 9–14.** POST to `/api/shutdown` with the token in the `X-SPOT-Token` header (5 s timeout); a failure is ignored (the server may already be gone).

<!-- xref -->
- **Used by:** `+spot/+console/start.m`, `SPOT.m`
- **Uses:** [[spot.console.status|spot-console-status]], [[spot.worker.stop|spot-worker-stop]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/stop.m</b> (15 lines; click to show)</summary>

```matlab
   1  function stop(root)
   2  %STOP Disconnect the MATLAB worker and stop the project's SPOT Console web server.
   3  %   spot.console.stop(projectFolder)
   4  spot.worker.stop();
   5  ready = spot.console.status(root);
   6  if isempty(ready)
   7      return
   8  end
   9  try
  10      opts = weboptions('MediaType', 'application/json', 'Timeout', 5, ...
  11          'HeaderFields', {'X-SPOT-Token', ready.token});
  12      webwrite(sprintf('http://127.0.0.1:%d/api/shutdown', ready.web_port), struct(), opts);
  13  catch
  14  end
  15  end
```

</details>
<!-- /listing -->
