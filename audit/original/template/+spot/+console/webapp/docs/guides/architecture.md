# How It Fits Together

SPOT is five kinds of computer and the programs that connect them. This page is the map.

![The computers of SPOT and what connects them](images/architecture.svg)

## The pieces

**MATLAB** holds the truth: the settings struct ([Settings File](page:settings-file)), the diagram, the simulations, the saved data. Everything the console shows is computed by a `spot.*` function ([the packages](page:project-folder)), so it all works without the console too ([Running Without the Console](page:matlab-without-console)). `SPOT` starts the console: `spot.console.start` launches the Python server and a [worker](ref:spot.worker.Worker) that connects MATLAB to it over a local socket and answers the page's commands, one JSON line each, from a `tcpclient` callback, so the Command Window stays usable while it is idle (`settings.set`, `sim.run`, `data.series`, …; the list is in [local.py](file:local.py)).

**The local console server** (`+spot/+console/webapp/app.py`, [server.py](file:server.py)) is a plain-Python HTTP server (no packages to install). It serves the page, forwards its requests to MATLAB, pushes events to the page (MATLAB's status, a simulation's progress, live samples), runs long jobs in the background ([local.py](file:local.py)), and keeps a **link to the lab console** when the lab network is reachable: it polls `/api/state`, relays the lab console's event stream to the page, and forwards the Experiment tab's actions. Its address is a random port on `127.0.0.1` with a session **token**, so only the browser tab MATLAB opened can drive your MATLAB.

**The page** ([index.html and app.js](file:app.js)): one HTML page, ES modules, three.js for the 3D table ([viewer.js](file:viewer.js)). Which tabs it shows depends on which server it is talking to ([server.py](file:server.py)'s `features`): the local one gives Simulation (with its Calibration pane), Experiment, Results, Designer and Documentation, plus the **Lab Setup** button; the lab one gives PhaseSpace, Hardware, Designer, Calibration, Vision and Documentation.

**The lab console** is the same `app.py` running on the Jetson TX2 in **lab mode** ([server.py](file:server.py), [the deployment scripts](page:lab-admin)). It compiles and runs [StreamData](file:StreamData.cc), which turns PhaseSpace's LED tracking into the pose packets the platforms consume ([phasespace.py](file:phasespace.py)); it drives the platforms' Teensies for the Hardware tab and [Interactive Mode](page:interactive-mode) ([hardware.py](file:hardware.py), [interactive.py](file:interactive.py), [control.py](file:control.py)); and it runs experiments: deploys the programs MATLAB built, starts them together, collects their data ([experiment.py](file:experiment.py)); it runs the lab's calibrations (the table's slope, the thrusters one at a time and together, the arm's friction on the Chaser) and keeps their results, which each project copies for its simulations ([calibration.py](file:calibration.py), [calfit.py](file:calfit.py)); and its Vision tab shows the Orin's cameras ([vision.py](file:vision.py)). It never needs MATLAB. Its page is `http://192.168.1.116:8080`.

**The platforms** each run a Jetson (Ubuntu) with the compiled diagram, which reads the PhaseSpace packets (`spot.hardware.phasespaceLatest`), computes the control, and writes the thruster packet to a **Teensy** over USB serial ([teensy_pwm_control.ino](file:teensy_pwm_control.ino)); the Chaser's Jetson also drives the arm's **Dynamixel** servos in PWM control (`spot.hardware.ArmPWM`). A small shell agent ([spot_agent.sh](file:spot_agent.sh)) lets the lab console manage them over SSH with a restricted key.

**The Jetson Orin** (`192.168.1.115`) runs **SPOT Vision** ([spot_vision.py](file:spot_vision.py)), a small web service on port 8090 that streams the cameras plugged into it (the ZED 2i, the RealSense L515, the DVXplorer, USB cameras) to the lab console's Vision tab. A camera is only opened while it streams, so experiments can use it the rest of the time. The Chaser's program can also exchange UDP packets with the Orin (its states out, a vision estimate in).

## One diagram, two runs

The Simulink diagram ([The Simulink Diagram: Overview and Step Order](page:diagram-overview)) is the same file in both cases. In a **simulation** MATLAB runs it with `simMode = 1`: the plant is integrated in the diagram, the clock is Simulink's, the hardware blocks are inert, and the results land in the workspace. For an **experiment**, `spot.experiment.build` generates C code for each selected platform with `simMode = 0` (and `WhoAmI` found at run time), compiles it on the TX2 and leaves the `.elf` there; the lab console deploys it. The control code you write runs unchanged in both.

## Where the data flows during an experiment

1. **StreamData** on the TX2 sends every platform a packet at the data rate (positions, velocities, LEDs, and the experiment clock slot).
2. Each platform's program sends its own clock back (UDP 48720–48722). When the lab console has heard all of them, it arms the clock in the stream (`exp start 3`); three seconds later (`Constants.SYNC_COUNTDOWN`) every program leaves phase 0 at the same instant ([PhaseSpace Camera Code](page:diagram-phasespace)).
3. Each program sends its debug message (55001–55003) and its live streams (55011–55013) to the lab console, which forwards them to the Experiment tab's Messages panel and Live Plot.
4. Each program logs its 108 signals to its `ExpLog` file. After **Stop**, **Collect** fetches them to the lab console, then to your `Saved Data/` folder, where MATLAB decodes them ([Logged Signals](page:logged-signals)).

## Ports and addresses

| Who | Address / port | What |
|---|---|---|
| Lab console page and API | `192.168.1.116:8080` (`IP_LAB_CONSOLE`, `PORT_LAB_CONSOLE`) | HTTP; `/api/events` is the event stream |
| PhaseSpace server | `192.168.1.109` (`IP_PHASESPACE_SERVER`) | the OWL API (StreamData connects to it) |
| StreamData → platforms | UDP 31535 → 31534 (`PORT_STREAMDATA`, `PORT_PHASESPACE_CLIENT`) | 115 floats (`PACKET_LENGTH`): poses, velocities, LEDs, clock |
| StreamData → lab console | UDP (the viewer port) | the same packet, for the 3D view |
| Platforms → lab console | UDP 48720 / 48721 / 48722 (`PORT_EXP_CLOCK_*`) | experiment clocks (RED / BLACK / BLUE) |
| | UDP 55001 / 55002 / 55003 (`PORT_DEBUG_*`) | debug messages |
| | UDP 55011 / 55012 / 55013 (`PORT_LIVE_*`) | live streams |
| Platforms → network | UDP 33001–33003 broadcast (`PORT_HEARTBEAT_*`) | heartbeats ([Check Connection](page:diagram-outgoing)) |
| RED ↔ Orin | UDP 46875 / 36845 (`PORT_ORIN_STATES`, `PORT_ORIN_COMMANDS`) | states out, vision estimate in |

The addresses and ports are the network section of `Constants` ([Constants](page:dev-constants)); both consoles read them from that file.
| Lab console → platforms | SSH 22 (`spot_agent.sh`) | deploy, start, stop, status, data, Teensy stream |
| Local console | `127.0.0.1:<random>` + token | the page; the worker's socket is another random port |

All addresses are settings or constants in one place each: the lab console's in the settings (`labConsoleAddress`, compiled into the programs as `labConsoleIP`), the platforms' in `spot_console/platforms` configuration on the lab console, the fixed ones in `spot.hardware`'s blocks and [experiment.py](file:experiment.py).

## The files a project keeps

| File / folder | What | Written by |
|---|---|---|
| `spot_settings.json` | every setting of the Simulation tab | the console, on every change ([Settings File](page:settings-file)) |
| `spot_design.json` | what is mounted on each platform | the Designer |
| `Constants.m` | every parameter of SPOT, and your own values | SPOT, you |
| `spot_calibration.json` (optional) | the project's copy of the lab's calibration (the table's slope, each thruster's thrust and the thrust decay, the arm's joint friction and inertia) and any values you override, and where each platform's PhaseSpace LEDs are; an `arm_calibration.json` from `calibrate_arm.py` run by hand still works, and the newer of the two arm calibrations is used | Update from Lab, the overrides on the Simulation tab's Calibration pane ([Calibration Tab](page:tab-calibration)), the Platforms pane's Thruster and LED Layout |
| `Saved Data/<run>/` | saved simulations and collected experiments | Save Results, Collect |
| `.spot_cache/` | the console's port and token, the last simulation's replay, logs | the console (not to be committed) |
| `~/ExperimentSoftware/MATLAB_ws`, `~/ExperimentData` (on the TX2) | the compiled programs; collected runs | the build, Collect |
| `~/.spot_calibration.json` (on the TX2) | the lab's calibration | the Calibration tab's runs |
