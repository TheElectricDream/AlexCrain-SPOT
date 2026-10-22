# The Project Folder

What is in a SPOT project, and what each part is for. Paths are relative to the folder that holds `SPOT.m`.

```text
<Project>/
├── SPOT.m                     start, reopen or stop the console (type SPOT in MATLAB)
├── SPOT_User_Parameters.m     your own workspace variables (runs after the settings)
├── <Project>.slx              the Simulink diagram, named after the project
├── spot_settings.json         the console's settings (created on the first start)
├── spot_design.json           the Spacecraft Designer's choices (created on the first change)
├── spot_calibration.json      this project's copy of the lab's calibration, your overrides, the LEDs' positions
├── +spot/                     all of SPOT's code, as MATLAB packages (below)
├── Saved Data/                saved simulations and collected experiments (one folder per run)
└── .spot_cache/               the console's state on this computer (safe to delete)
```

A project is made by [`New_SPOT_Project`](page:new-project), which copies the repository's `template/` folder and renames the diagram (the one you choose, when the template has several). The template's own settings, saved data, caches and documentation sources are not copied, so a new project starts from the default settings.

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
| `+spot/+console/webapp/docs/` | this documentation: the guides, the notes on functions, their pictures (`docs/images/`: among them `ArmConfig.png`, `COMConfig.png` and `ThrusterConfig.png`, the arm's geometry, where the scales go and the thruster numbering) |
| `+spot/+console/webapp/static/vendor/` | third-party libraries the page uses (three.js, CodeMirror, ESLint, marked, KaTeX) |

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

The lab measures the table's slope, each thruster's thrust and thrust decay, and the arm's friction on the lab console's [Calibration Tab](page:tab-calibration). The local console offers to copy a newer calibration into this file when it can see the lab console, and the Simulation tab's Calibration pane shows what simulations will use. Any value you override there is kept in the same file (`overrides`), so updating from the lab doesn't lose it.

The file also holds where each platform's PhaseSpace LEDs are (`markers`: each LED's id and its x and y in millimetres from the body centre), set in the Platforms pane's **Thruster and LED Layout** window; until then, the default layout (13 mm in from each corner). Updating from the lab keeps them too. MATLAB works out each LED's position from the platform's centre of gravity, and the Experiment tab's **Start Cameras** sends that marker file to the lab console, which writes it next to StreamData.

`spot.params.loadCalibration` reads the file. A project made before the file moved to the top of the project kept it in a `resources/` folder; the console moves it across the first time it starts.

## What is not here

The old App Designer GUI, earlier diagrams, the CAD and old data are kept outside the repository. The repository itself holds `New_SPOT_Project.m`, `template/` and an empty `projects/`.
