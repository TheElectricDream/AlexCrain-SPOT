# The Experiment Tab

Running the diagram **on the platforms**, from your MATLAB, through the lab console. The sidebar is a checklist from top to bottom: Lab, Platforms, Build, Recording, Deploy & Run, Data. It only works on the lab network; off it, everything locks. Everything that touches hardware is done by the lab console (the Jetson TX2); this page only asks it to. The code is [experiment.js](file:experiment.js) in the page, `spot.experiment` in MATLAB and [experiment.py](file:experiment.py) on the lab console.

## Lab

- **Lab Console (TX2) Address**: where the lab console is (`192.168.1.116` by default). It is a setting (`labConsoleAddress`): the diagram compiles it in as `labConsoleIP`, the address every platform sends its clock, messages and streams to, so **after changing it, build again**. The pill says **Connected**, **Needs Update** (the lab console runs an older console than yours; update it with the deploy script) or **Not Reachable** with the reason; the local server keeps retrying every 3 s.

### PhaseSpace

In the same card:

- **Start Cameras** / **Stop Cameras**: starts [StreamData](file:StreamData.cc) on the lab console at the **Data Rate**, after sending it this project's marker file (`tracker_positions`: each LED's position from its platform's centre of gravity, from the LED positions in `spot_calibration.json` and the scale readings; see the Simulation tab's Thruster and LED Layout). The lab console writes it next to StreamData. The line shows whether it is streaming and the packet rate the lab console sees.
- **Data Rate**: the same value as the Simulation tab's (they are one setting, `dataRate`). Locked, and fixed while the cameras stream.

## Platforms

The diagram the experiment is built from, and a row per platform in the scenario (turn a platform off on the Simulation tab's Scenario pane to leave it out), with its hardware state as the lab console sees it (for example **Ready**, **In Use** or **Offline**: the [Hardware tab](page:tab-hardware) explains each).

## Build

1. **Check** (`spot.experiment.plan`): what a build would do, without building: the platforms and their programs, the phases that will run, the changes since the last build, and warnings (a platform not selected, the arm without the Chaser, …).
2. **Connect** (`spot.experiment.connect`): MATLAB's connection to the TX2 for remote building. The TX2's password is only needed the first time; the connection is remembered.
3. **Build** (`spot.experiment.build`): generates C code from the diagram for each selected platform (with `simMode = 0`, and *Simulate Plant Dynamics* commented out when the diagram has that block; a diagram that switches to its hardware itself with `simMode`, e.g. through a Variant Subsystem, has nothing to comment out), compiles it on the TX2 and leaves the `.elf` in `~/ExperimentSoftware/MATLAB_ws`. The log streams into the card as it goes; a build takes several minutes. **Details** on a failure opens the error in the command window.

The **Deploy & Run** card says whether a program for this diagram already exists on the TX2, when it was built and how big it is.

## Recording

The cameras plugged into the Jetson Orin, each with a switch to record it with the experiment (the ZED 2i, the RealSense L515, the DVXplorer, the thermal camera, USB cameras: whatever SPOT Vision finds). Your choice is remembered in this browser. Each camera shows its kind and its recording rate: **estimate** until it has recorded once with its current settings, **measured** after. A camera another program holds shows as in use.

Under the cameras, the **estimate**: their total size for this experiment (its phases from the settings' `timing`, plus 30 s for the start and the stop) against the Orin's free disk. It turns amber above half of the free space and red if it won't fit. **Start Experiment**'s confirmation repeats it, and asks again if it won't fit. A recording stops itself when 1 GB is left on the Orin.

With cameras switched on, **Start Experiment** first has SPOT Vision start recording all of them. If one can't (unplugged, in use, no library), nothing records and the experiment doesn't start, with the reason. Recording starts before the platforms' programs, so it includes the start, the countdown and phase 0, and it stops with the experiment. Every frame is timed on the experiment's clock, as the platforms log `Time_s`: [Camera Recordings](page:camera-recordings) explains the files and the timing.

While it records, each camera shows its frames and size so far, and the switches are locked. A recorded camera is held by SPOT Vision until the experiment stops, so your own code on the Orin can't open it (the Vision tab still streams it). The recording stays on the Orin's disk; nothing crosses the network until **Collect**. If the lab console or SPOT Vision runs an older version without recording, the card says which to update.

## Deploy & Run

- **Deploy**: the lab console copies the program (and the shared libraries it needs) to each platform over SSH.
- **Start Experiment** (enabled once this diagram is deployed): the lab console checks that the cameras stream and every Teensy is free, starts each program, and waits until all of them report their clock; then it arms the clock in the PhaseSpace stream and **three seconds later all platforms leave phase 0 together**. The status line tells the story (waiting for which platforms, counting down, running since). Use the lab's **E-stop** if anything looks wrong; the Teensies also shut their valves after 2 s without a packet.
- **Stop**: stops every program and sends the Teensies an all-off.

Under the 3D view, each platform's experiment clock is shown, with how far apart the clocks are (network delays included).

While it runs: the 3D view shows the platforms live (the **Simulation** overlay replays the last simulation in step with the experiment's clock, so you can see how reality departs from the model; its arm only while the Designer's Chaser carries it, see [Runs flown with the arm](page:tab-designer#runs-flown-with-the-arm)), the Messages panel shows each platform's message, the Live Plot each platform's streams, and **Clear Paths** wipes the trails.

## Data

**Collect** (enabled once stopped, with MATLAB connected): the lab console fetches each platform's `ExpLog` files and debug messages, your console downloads them into `Saved Data/<diagram>_<stamp>/<PLATFORM>/`, and MATLAB decodes them into `.mat` (`spot.experiment.decodeData`). The run then appears in the [Results tab](page:tab-results)'s **Saved Data…** list next to your simulations, with the same signal names ([Logged Signals](page:logged-signals)).

If cameras were recorded, Collect also copies their recording from the Orin into `Saved Data/<run>/cameras/`, straight from the Orin to your computer and in the background (a line under Collect's shows its progress). Every file's size is checked, and then the recording is deleted from the Orin. If the copy fails, the recording stays on the Orin, and Collecting again finishes it, in the same run's folder.

## A typical session

1. Simulate the experiment first; check the trajectory and the thruster duty cycles.
2. In the lab: the table and the platforms prepared ([Preparing the Lab](page:lab-procedures)), platforms powered and on the table; the lab console's [Hardware tab](page:tab-hardware) shows them **Ready**.
3. **Start Cameras**; check on the lab console's [PhaseSpace tab](page:tab-phasespace) that every platform is tracked.
4. **Check → Connect → Build** (once per change of the diagram or settings).
5. **Recording**: switch on the Orin's cameras to record, and check the estimate against the Orin's free space.
6. **Deploy → Start Experiment**. Watch, with a hand near the E-stop. **Stop** when the messages say *End of Experiment Reached* (after phase 5), or earlier.
7. **Collect** (the platforms' data and the recordings), then compare in the Results tab.
