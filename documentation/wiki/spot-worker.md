# spot.worker: the MATLAB worker

`+spot/+worker/` is how the console reaches MATLAB. When you run `SPOT`, the console's Python server starts and a **worker** object in your MATLAB session connects to it over a local TCP socket. The page sends commands to the server, the server forwards them to the worker as one JSON line each, the worker runs the matching `spot.*` function **in your MATLAB session** and answers with a JSON line. That is why simulations run in your MATLAB, `dataClass` appears in your workspace, and the command window on the page is your command window.

```
{"id": 7, "cmd": "sim.run", "args": {}}                           → worker
{"id": 7, "ok": true, "result": {"seconds": 48.1, ...}}            ← worker
{"event": "sim_progress", "t": 12.5, "tsim": 275, "status": "running"}   ← worker (while simulating)
```

| | |
|---|---|
| [[Worker|spot-worker-Worker]] | the class: the connection, the settings, the loaded datasets, and `dispatch`, one case per command |
| [[start|spot-worker-start]] | make a worker and connect it (called by `spot.console.start`) |
| [[stop|spot-worker-stop]] | disconnect it |

Commands run in MATLAB's main thread from the socket's callback, so the MATLAB Command Window stays usable while the worker is idle, and a long command (a simulation) blocks it as a simulation would.

The server side is [[local.py|Console-local]].
