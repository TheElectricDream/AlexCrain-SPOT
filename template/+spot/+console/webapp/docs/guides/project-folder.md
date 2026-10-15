# The Project Folder

What is in a SPOT project, and what each part is for. Paths are relative to the folder that holds `SPOT.m`.

```text
<Project>/
├── SPOT.m                     start, reopen or stop the console (type SPOT in MATLAB)
├── SPOT_User_Parameters.m     your own workspace variables (runs after the settings)
├── <Project>.slx              the Simulink diagram, named after the project
├── spot_settings.json         the console's settings (created on the first start)
├── spot_design.json           the Spacecraft Designer's choices (created on the first change)
├── spot_calibration.json      this project's copy of the lab's calibration, and your overrides
├── +spot/                     all of SPOT's code, as MATLAB packages (below)
├── resources/                 pictures the guides use, and the PhaseSpace marker file
├── Saved Data/                saved simulations and collected experiments (one folder per run)
└── .spot_cache/               the console's state on this computer (safe to delete)
```

A project is made by [`New_SPOT_Project`](page:new-project), which copies the repository's `template/` folder and renames the diagram. The template's own settings, saved data, caches and documentation sources are not copied, so a new project starts from the default settings.

## The `+spot` packages

Everything SPOT does is a function in one of these packages. The console only calls them, so all of it also works from the MATLAB Command Window or a script ([Running Without the Console](page:matlab-without-console)). `help spot.<package>` lists a package in MATLAB; the Reference lists every function, with its code.

| Package | What it holds |
|---|---|
| `spot.params` | settings: defaults, loading and saving, phase timing, putting them into the workspace |
| `spot.massprops` | mass, centre of gravity, inertia, the LEDs' positions for PhaseSpace |
| `spot.control` | the platform model, LQR gains, thruster forces; the diagram's controller and mixer as plain functions |
| `spot.sim` | finding and loading diagrams, running simulations, errors, results for the 3D view |
| `spot.experiment` | checking, connecting to the lab console, building for the platforms, decoding their logs |
| `spot.data` | saved runs, loading `.mat` files, series for the plots, trajectories, figures |
| `spot.plotting` | the arm's joint positions, spacecraft outlines, signal names and units |
| `spot.workspace` | the console's Workspace pane and command window |
| `spot.worker` | the MATLAB side of the console: runs its commands in your session |
| `spot.console` | starting, reopening and stopping the console; the web app (`webapp/`), the PhaseSpace program (`phasespace/`), the deployment scripts (`deploy/`), SPOT Vision (`vision/`) |
| `spot.hardware` | the diagram's hardware blocks and their C/C++ (`csrc/`), the Teensy firmware (`teensy/`), the arm's serial setup (`arm/`) |
| `spot.arm` | the simulated arm (plant, friction, servo models), its position and velocity controllers, the calibration script (`calibration/`) |

`+spot/CODE_TEMPLATE.md` is the coding standard every file follows ([The Coding Standard](page:dev-code-standard)).

## The console's own files

| Path | What |
|---|---|
| `+spot/+console/webapp/app.py` | starts the console's web server |
| `+spot/+console/webapp/spot_console/` | the server, one Python module per area |
| `+spot/+console/webapp/static/` | the page: HTML, CSS and one JavaScript module per tab |
| `+spot/+console/webapp/docs/` | this documentation: the guides, the notes on functions, their pictures |
| `+spot/+console/webapp/static/vendor/` | third-party libraries the page uses (three.js, CodeMirror, ESLint, marked, KaTeX) |

## `resources/`

| File | Used for |
|---|---|
| `ArmConfig.png`, `COMConfig.png`, `ThrusterConfig.png` | the arm's geometry, where the scales go for the centre of gravity, and the thruster numbering ([Mass Properties](page:math-mass-properties)) |
| `SRL_logo.png` | the lab's logo |
| `tracker_positions_phasespace.txt` | the PhaseSpace marker file: each LED's offset from its platform's centre of gravity. The Platforms pane's **Write PhaseSpace Marker File** writes it (`spot.massprops.writeTrackerPositions`); the Experiment tab sends it to the lab console before the cameras start |

## `.spot_cache/`

Per-computer state the console keeps; nothing in it needs to be kept or committed.

| File | What |
|---|---|
| `console.json` | the running console's ports, token and process, so `SPOT open` and `SPOT stop` find it |
| `sim_latest.json` | the last simulation's replay for the 3D view |
| `console.log`, `build.log` | the server's output, the last experiment build's log |
| `stop_request` | written by Stop while a simulation runs; the worker checks for it |
| `docs_backups/` | the previous version of every file saved from the Documentation tab's editor (the last 40 saves) |

## `spot_calibration.json`

The lab measures the table's slope, each thruster's thrust and thrust decay, and the arm's friction on the lab console's [Calibration Tab](page:tab-calibration). The local console offers to copy a newer calibration into this file when it can see the lab console, and the Simulation tab's Calibration pane shows what simulations will use. Any value you override there is kept in the same file (`overrides`), so updating from the lab doesn't lose it. `spot.params.loadCalibration` reads it. A project made before this file moved to the top of the project kept it in `resources/`; the console moves it across the first time it starts.

## What is not here

The old App Designer GUI, earlier diagrams, the CAD and old data are kept outside the repository. The repository itself holds `New_SPOT_Project.m`, `template/` and an empty `projects/`.
