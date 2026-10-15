# Troubleshooting

Start with what the consoles say: most problems show up as a status with its reason, a toast, or a message in a log. The tables below go from what you see to what to do.

## Starting the local console

| What you see | What to do |
|---|---|
| "needs Python 3.6 or newer" | **Install Python** in the dialog, or `spot.console.installPython`, or point MATLAB at a Python with `pyenv('Version', ...)` ([Setting Up Your Computer](page:installing)) |
| the page says it can't talk to MATLAB | open the page with the link MATLAB printed, or run `SPOT open`. The page only takes commands from a tab opened with that link |
| **MATLAB Not Connected** on the Simulation tab | MATLAB was closed or the worker stopped: run `SPOT` again in the project folder |
| MATLAB is busy | a simulation, a build, a file dialog or a command is still running in MATLAB; wait, or **Stop** the simulation |
| the console doesn't start at all | read `.spot_cache/console.log` in the project folder |
| **Server Offline** in the top right | the console's server stopped (`SPOT stop`, or MATLAB closed): run `SPOT` |

## Simulations

| What you see | What to do |
|---|---|
| the run fails with a red message under Run | it is the error's root cause. **Details** prints the chain of errors and the blocks involved in the command window; **Show Block** opens the diagram at the block |
| "Select a Simulink diagram first" | choose the diagram on the Scenario pane (a `.slx` in the project folder) |
| warnings in the command window | Simulink's; the template gives none. Read each: a new data store read before it is written is the usual one ([Data Stores](page:data-stores)) |
| arm warnings ("goal beyond Min/Max Position", "PWM beyond its servo's PWM Limit") | your arm commands ask for something the real servos would ignore or clip (`spot.arm.checkCommands`) |
| the LQR gains can't be computed | a weight is zero or negative on the Control pane |

## The lab network and the lab console

| What you see | What to do |
|---|---|
| **Lab Disconnected** (local console) | you are not on the lab network, or the lab console's address is wrong (Experiment tab, Lab card). Its tooltip gives the reason |
| **Needs Update** (Experiment tab) | the lab console runs an older console than your project: **Lab Setup › Update the Lab Console** ([Lab Setup](page:lab-setup)) |
| the lab console's page doesn't load | the TX2 is off or its service stopped. On the TX2: `systemctl --user status spot-phasespace-console`; `journalctl --user -u spot-phasespace-console -f` shows its log |

## Platforms (the lab console's Hardware tab)

| Status | Meaning, and what to do |
|---|---|
| **Offline** | the platform doesn't answer: switched off, still booting, or not on the network. Check its power and Wi-Fi; wait a minute after power-on. A platform that drops off now and then may have Wi-Fi power saving on: run `setup_hardware_link.sh` in a terminal for it ([Lab Administration](page:lab-admin)) |
| **No Access** | the platform doesn't accept the lab console's key (a reinstalled platform, or a new one): **Lab Setup › Link the Platforms** |
| host key changed | the platform was reinstalled; on the TX2 run the `ssh-keygen -R <address>` the message gives |
| **No Teensy** | the Jetson is up, but there is no Teensy at `/dev/ttyACM0`: check its USB cable, and power-cycle the platform |
| **In Use** | another program has the Teensy's port (usually an experiment still running): stop it on the Experiment tab |
| pucks or a thruster don't respond | check that the E-stop is released (blue light), that the tank valve is open and the gauges read pressure, and the connections to the valves on the bottom level; power-cycle the platform |
| a thruster stays open | switch everything off (Esc on the Hardware tab), then toggle the pucks and fire each thruster once; if it persists, power-cycle the platform. The Teensy shuts every valve 2 s after its commands stop, so a stuck valve after that is electrical: press the E-stop and look at the valve's driver |

## PhaseSpace

| What you see | What to do |
|---|---|
| **Waiting for Packets** on the 3D view | streaming hasn't started, or the packets don't reach this computer. Its line gives the reason |
| a platform isn't tracked | its LEDs are off (check the LED driver's connections) or hidden from the cameras; the Telemetry card lists the LEDs seen |
| poses jump or a platform flickers | LEDs seen by too few cameras, or the PhaseSpace calibration has drifted: recalibrate PhaseSpace (the lab's PhaseSpace calibration procedure) |
| UDP selected under WSL | WSL 2's NAT networking can't receive the frames: use TCP, or switch WSL to mirrored networking |

## Experiments

| What you see | What to do |
|---|---|
| the build fails | **Details** shows the cause in the command window; the full log is `.spot_cache/build.log` |
| Start waits for a platform | that platform's program hasn't started or can't reach the lab console: see its row on the Experiment tab, and its status on the Hardware tab |
| the platforms oscillate or are unstable | the network delays the packets (reset the router), PhaseSpace's calibration has drifted (recalibrate it), or a platform's mass properties have changed (check them on the Platforms pane). Check the Live Plot and the clocks' spread under the 3D view |
| Collect finds no data | the programs didn't write their `ExpLog` files (they didn't run long enough, or stopped at once): read each platform's log on the Experiment tab |

## Calibrations

| What you see | What to do |
|---|---|
| a table or thruster run isn't saved | the reason is shown: PhaseSpace's positions jumped or went back in time (mis-tracking, or a second stream), or the motion didn't fit the model. Fix the cause and run again |
| a run stopped early | out of air, a platform too close to an edge or another platform, or PhaseSpace lost it. Previous Runs can still fit what it recorded |
| the arm calibration can't start | the setup check lists what the Chaser is missing and how to fix each item ([The Calibration Tab](page:tab-calibration)) |

If a problem isn't here, report it to the lab's administrators with what the console said (a screenshot of the status, and the log).
