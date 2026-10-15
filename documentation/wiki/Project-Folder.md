# The Project Folder

What is in a SPOT project (and in this template), and what each part is for. Paths are relative to the folder that holds `SPOT.m`.

```
<Project>/
├── SPOT.m                       start/open/stop the console (type SPOT in MATLAB)
├── SPOT_User_Parameters.m       your own workspace variables (runs after the settings)
├── <Name>.slx                   the Simulink diagram (the template's is Template_v5_0_0_2026a_Jetson_PSClock.slx)
├── spot_settings.json           the console's settings (created on first start)
├── spot_design.json             the Spacecraft Designer's choices
├── +spot/                       all the MATLAB code, as packages (see below)
├── resources/                   images the pages refer to, the arm and lab calibrations, the PhaseSpace marker file
├── Saved Data/                  your saved simulations and collected experiments
├── SPOT_Tutorial.pdf            the user guide
└── .spot_cache/                 the console's per-computer state (safe to delete)
```

The repository is the folder above the template: `New_SPOT_Project.m`, `template/` and an empty `projects/`, with one `.gitignore` that leaves out `archive/`, `designs/`, the projects, the template's `documentation/` and `STATUS.md`, caches, builds and `Saved Data/`. The template also has `STATUS.md` (the developers' working log) and `documentation/` (the tutorial's sources and this wiki), both git-ignored. Next to the template, `New_SPOT_Project.m` makes projects from it into `projects/` ([[New_SPOT_Project|Tool-New-SPOT-Project]]).

## The `+spot` packages

| Package | What it holds | Overview page |
|---|---|---|
| `+spot/+params` | settings: defaults, load/save, phase timing, putting them in the workspace | [[spot.params|spot-params]] |
| `+spot/+massprops` | mass, centre of gravity, inertia, LED positions | [[spot.massprops|spot-massprops]] |
| `+spot/+control` | the platform model, LQR gains, thruster forces; the diagram's controller as plain functions | [[spot.control|spot-control]] |
| `+spot/+sim` | loading diagrams, running simulations, errors, results for the 3D view | [[spot.sim|spot-sim]] |
| `+spot/+experiment` | checking, connecting to the TX2, building for the platforms, decoding their logs | [[spot.experiment|spot-experiment]] |
| `+spot/+data` | saved runs, loading `.mat` files, series for the plots, trajectories, figures | [[spot.data|spot-data]] |
| `+spot/+plotting` | the arm's joint positions, spacecraft outlines, signal names and units | [[spot.plotting|spot-plotting]] |
| `+spot/+workspace` | the console's Workspace panel and command window | [[spot.workspace|spot-workspace]] |
| `+spot/+worker` | the MATLAB side of the console: runs its commands in your session | [[spot.worker|spot-worker]] |
| `+spot/+console` | starting the console; the web app (`webapp/`), the PhaseSpace stream program (`phasespace/`), the deploy scripts (`deploy/`) | [[spot.console|spot-console]] |
| `+spot/+hardware` | the blocks that talk to the hardware and their C/C++ (`csrc/`), the Teensy firmware (`teensy/`), the arm's serial setup (`arm/`) | [[spot.hardware|spot-hardware]] |
| `+spot/+arm` | the simulated arm: plant, friction, servo models, the position/velocity controllers, calibration | [[spot.arm|spot-arm]] |

Each package has a `Contents.m` so that `help spot.<package>` lists it in MATLAB.

## `resources/`

| File | Used by |
|---|---|
| `ArmConfig.png`, `COMConfig.png`, `ThrusterConfig.png` | the tutorial and this wiki: the arm's geometry, the scale layout for the centre of gravity, the thruster numbering |
| `SRL_logo.png` | the console's page |
| `tracker_positions_phasespace.txt` | the PhaseSpace marker calibration (each LED's offset from its platform's CG), written by [[spot.massprops.writeTrackerPositions|spot-massprops-writeTrackerPositions]] and sent to the lab console before the cameras start |
| `spot_calibration.json` (optional) | the project's copy of the lab's calibration (the table's slope, each thruster's thrust and the arm's joint friction and inertia, measured on the lab console's [[Calibration Tab|Calibration-Tab]]), written by the tab's Update on the local console; [[spot.params.loadCalibration|spot-params-loadCalibration]] reads it for simulations and builds. Part of the project; without it the simulated table is level, every thruster has the nominal thrust and the arm has its default friction (unless there is an `arm_calibration.json`) |
| `arm_calibration/` | `calibrate_arm.py` ([[page|Tool-calibrate-arm]]) and its `README.md`: measure the arm's friction in the lab (the Calibration tab runs it on the Chaser and puts the result in the lab's calibration, so in `spot_calibration.json`); run by hand, it writes `arm_calibration.json`, to copy into `resources/`, which still works: [[spot.arm.parameters|spot-arm-parameters]] uses the newer of the two |

## `documentation/`

- `SPOT_Tutorial.pdf` / `.tex`: the long-form tutorial. `update_tutorial_reference.py` regenerates its appendices (settings, workspace variables, file list) from the code.
- `demo_execution_order.m`: a small script that prints the order Simulink runs the diagram's subsystems in ([[page|Tool-demo-execution-order]]).
- `wiki/`: these pages, `build_wiki.py` and `images/`.

## What is *not* here

Everything that is not needed to run simulations and experiments was moved out of the project, to `../archive/` next to the template: the old App Designer GUI and its library, earlier diagrams, the tool that builds the 3D models from the CAD, manuals, old saved data, the camera code and the earlier Dynamixel blocks. The lab's CAD itself is in `../designs/`. See [[Archive|Archive]].
