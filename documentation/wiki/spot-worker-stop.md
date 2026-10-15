# spot.worker.stop

**File:** `+spot/+worker/stop.m` · **Package:** [[spot.worker|spot-worker]]

<!-- source: +spot/+worker/stop.m -->

## What it is for

Disconnect the worker, if there is one.

## Line by line

**Lines 3–7.** Fetch the instance, delete it (which closes the socket), and clear the instance.

<!-- xref -->
- **Used by:** `+spot/+console/stop.m`, `+spot/+worker/Worker.m`, `+spot/+worker/start.m`
- **Uses:** [[spot.worker.Worker|spot-worker-Worker]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+worker/stop.m</b> (8 lines; click to show)</summary>

```matlab
   1  function stop()
   2  %STOP Disconnect the MATLAB worker from the SPOT Console (if one is running).
   3  w = spot.worker.Worker.instance();
   4  if ~isempty(w)
   5      delete(w);
   6  end
   7  spot.worker.Worker.instance([]);
   8  end
```

</details>
<!-- /listing -->
