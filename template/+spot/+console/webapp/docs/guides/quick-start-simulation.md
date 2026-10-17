# Quick Start: Your First Simulation

From nothing to a simulated run on your computer, in about ten minutes. You need MATLAB R2026a with Simulink, the DSP System Toolbox and the Control System Toolbox ([Setting Up Your Computer](page:installing)).

## 1. Make a project

In MATLAB, in the folder that holds `New_SPOT_Project.m` and `template/`:

```matlab
New_SPOT_Project('MyProject')
```

This makes `projects/MyProject/`, a copy of the template with the diagram renamed `MyProject.slx` ([Making a New Project](page:new-project)).

## 2. Start the console

```matlab
cd projects/MyProject
SPOT
```

The page *SPOT Local Simulations* opens in your browser on the **Simulation** tab. The first time, the console creates `spot_settings.json` from the defaults and picks the project's diagram.

## 3. Set up the scenario

On the Simulation tab's **Scenario** pane ([The Simulation Tab](page:tab-simulation)):

1. Under **Platforms**, turn on the Chaser (and any others you want).
2. Check the **Timeline**: phase 0 waits, phase 1 starts floating, phase 2 goes to the initial position, phase 3 (four sub-phases) is your experiment, phase 4 returns home and phase 5 holds there; then everything switches off. Switch off the segments you don't need; the bar shows the total time.
3. On the **Platforms** pane, set each platform's **Initial** and **Home** pose, or drag them on the 3D table.

Everything is saved as you type, and put into MATLAB's workspace for the diagram.

## 4. Run

Press **Run Simulation**. The status line follows the run; a full 275 s run of the template (the default phases, `Constants.PHASE0_DURATION` … `PHASE5_DURATION`) typically takes under a minute. When it finishes:

- the 3D view replays it (Space plays and pauses; drag the scrubber);
- the **Messages** panel on the right shows what each platform was doing;
- the MATLAB command window shows Simulink's warnings (the template gives none);
- the **Results** tab can plot any of the 108 logged signals ([Logged Signals](page:logged-signals)).

**Save Results** keeps the run in `Saved Data/` to replay or plot later.

## 5. Change something

- On the **Control** pane, change the LQR weights; the gains under them update as you type ([Platform Dynamics and LQR](page:math-dynamics-lqr)). Run again, and use **Compare** on the playback bar to lay the earlier run over the new one.
- Open the diagram (**Open in Simulink** on the Scenario pane) and look at [1 - Experiment Logic](page:diagram-experiment-logic): each phase sets where each platform should be and which controller flies it. Your own guidance and control go in the numbered subsystems ([Writing Your Own Guidance and Control](page:diagram-your-code)).
- Your own values go in `Constants.m`, which the diagram reads as `Constants.NAME` ([Constants](page:dev-constants)).

## Next

- [Quick Start: An Experiment in the Lab](page:quick-start-experiment)
- [Running Without the Console](page:matlab-without-console): the same from scripts, for sweeps and batches.
