# Getting Started

This page takes you from nothing to a simulation on your computer and an experiment in the lab.

## What you need

| | Simulations (anywhere) | Experiments (in the lab) |
|---|---|---|
| MATLAB | R2026a or newer, with **Simulink** and the **Control System Toolbox** (for the LQR gains) | the same, plus the Simulink Coder / Embedded Coder and the NVIDIA Jetson support package to build for the platforms |
| Python | 3.6 or newer on your computer (the console's server is plain Python, no packages needed); if it is missing, `SPOT` offers to install it | the lab console (Jetson TX2) runs its own copy |
| A browser | Any current browser (Chrome, Edge, Firefox, Safari) | the same |
| Network | none | the SRCL Wi-Fi or a cable: the lab console is at `192.168.1.116`, the platforms at `192.168.1.110/111/112` |

## 1. Make a project from the template

The `template/` folder is not meant to be worked in directly; make a copy with your own name. In MATLAB, in the folder that holds `template/`:

```matlab
New_SPOT_Project('MyProject')     % or just New_SPOT_Project to be asked for the name
```

You get `projects/MyProject/` with `SPOT.m`, `SPOT_User_Parameters.m`, `+spot/`, `resources/`, the tutorial, an empty `Saved Data/` and the diagram **renamed to `MyProject.slx`**. The name is also the Simulink model's name, so it must be a valid MATLAB name (letters, digits and `_`, starting with a letter, not a MATLAB keyword or an existing function). A second argument puts the project somewhere else. Details: [[New_SPOT_Project|Tool-New-SPOT-Project]].

## 2. Start the console

In MATLAB, go to the project folder and run

```
SPOT
```

MATLAB starts the console's server (a Python process), opens the page in your browser and connects itself to it: the page is the **local console** ("SPOT Local Simulations" in the top left). The link it opens carries a token; keep using that link (or `SPOT open` prints it again). `SPOT stop` shuts it down. See [[spot.console.start|spot-console-start]].

The first time, the console creates `spot_settings.json` from the defaults and picks the first diagram in the folder.

## 3. Run a simulation

On the [[Simulation tab|Simulation-Tab]]:

1. **Scenario**: tick the platforms that take part (Chaser, Target, Obstacle, Arm), set the sample rate and the phase timeline.
2. **Platforms**: the initial and home poses (also draggable on the 3D table: Initial Conditions → Initial, then drag a marker; Shift+drag turns it) and the mass properties.
3. **Control**: the LQR weights; the gains are computed as you type.
4. **Run Simulation**. The 3D view replays the run, the Messages panel shows what each platform was doing, the command window shows Simulink's warnings, and the [[Results tab|Results-Tab]] plots any logged signal. **Save Results** keeps the run in `Saved Data/`.

Every change is saved to `spot_settings.json` straight away, and put into MATLAB's base workspace ([[Workspace Variables|Workspace-Variables]]) so the diagram can read it. Your own variables go in `SPOT_User_Parameters.m`, which runs after the settings.

## 4. Run an experiment

On the [[Experiment tab|Experiment-Tab]], connected to the lab network:

1. **Lab**: the lab console's address (default `192.168.1.116`); the pill says *Connected* when the local server reaches it. **Start Cameras** starts the PhaseSpace stream at the data rate (locked to the Simulation tab's Data Rate).
2. **Platforms**: the ones in the scenario; each will get its own program.
3. **Build**: *Check* the setup, *Connect* MATLAB to the TX2, then *Build* compiles the diagram for each platform (on your computer) and sends the programs to the lab console.
4. **Deploy & Run**: *Deploy* copies the programs to the platforms, *Start* runs them (the lab console counts them in so they start together), *Stop* ends the experiment.
5. **Data**: *Collect* fetches each platform's log and converts it to `.mat`, into `Saved Data/`.

The lab console's own page (`http://192.168.1.116:8080`, "SPOT Hardware Interfaces") has the [[PhaseSpace tab|Lab-Console-PhaseSpace-Tab]] (stream, compile StreamData, Interactive Mode) and the [[Hardware tab|Lab-Console-Hardware-Tab]] (pucks, thruster checks).

## 5. Where things end up

| | |
|---|---|
| `spot_settings.json` | your settings (the console writes it) |
| `spot_design.json` | what is mounted on each platform (Designer) |
| `Saved Data/<run>/` | simulations you saved and experiments you collected (`.mat` files, settings, figures) |
| `.spot_cache/` | the console's token and ports, the last simulation's replay (`sim_latest.json`); safe to delete |
| MATLAB's base workspace | `dataClass` (every logged signal of the last simulation), `simOut`, and the [[workspace variables|Workspace-Variables]] |

## If something goes wrong

- *"MATLAB is not connected"* on the page: run `SPOT` in MATLAB, in the project folder.
- *The page can't talk to MATLAB*: open the page with the link SPOT printed (it carries the session token), not a bookmark without it.
- A failed simulation shows its root cause under the Run button; **Details** prints the whole chain and **Show Block** opens the diagram at the block ([[spot.sim.errorDetails|spot-sim-errorDetails]]).
- *Lab Console Not Reachable*: you are not on the lab network, or the address is wrong (Experiment tab › Lab).
