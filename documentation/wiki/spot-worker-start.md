# spot.worker.start

**File:** `+spot/+worker/start.m` · **Package:** [[spot.worker|spot-worker]]

<!-- source: +spot/+worker/start.m -->

## What it is for

Make a [[Worker|spot-worker-Worker]] for the project and connect it to the console's server; any worker already running is replaced. [[spot.console.start|spot-console-start]] calls it with the port and token the server wrote to `.spot_cache/console.json`.

## Line by line

**Line 5.** Stop the old one.

**Lines 6–8.** New worker, connect, remember it as the instance.

<!-- xref -->
- **Used by:** `+spot/+console/start.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.console.start|spot-console-start]], [[spot.worker.Worker|spot-worker-Worker]], [[spot.worker.stop|spot-worker-stop]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+worker/start.m</b> (9 lines; click to show)</summary>

```matlab
   1  function w = start(port, token, root)
   2  %START Connect a MATLAB worker to the SPOT Console's web server.
   3  %   w = spot.worker.start(workerPort, token, projectFolder). Replaces any worker already running.
   4  %   Normally called by spot.console.start.
   5  spot.worker.stop();
   6  w = spot.worker.Worker(root);
   7  w.connect(port, token);
   8  spot.worker.Worker.instance(w);
   9  end
```

</details>
<!-- /listing -->
