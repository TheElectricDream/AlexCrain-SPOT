# The Simulation Tab

The local console's main tab: the scenario and its settings on the left, the 3D table in the middle, the MATLAB command window and the Live Plot under it, and each platform's messages on the right. Everything you type is saved to `spot_settings.json` straight away and put into MATLAB's base workspace, so the diagram, the console and your own scripts all see the same values. The page's code is [sim.js](file:sim.js); in MATLAB it is `spot.params` and `spot.sim`.

## The top bar

From left to right: the console's name (**SPOT Local Simulations** here, **SPOT Hardware Interfaces** on the lab console) with this computer's name; the tabs; the **Settings** group with the settings file's name and **Load…**, **Save As…** and **Reset…**; **Lab Setup** ([Lab Setup](page:lab-setup)); **Lab Console ↗**, which opens the lab console's page; and the connection badge. On the local console the badge says whether the **lab console** can be reached, since that decides whether experiments can be run.

- **Load…** reads settings from another file through MATLAB's file dialog: a `.json` saved with Save As…, or a state saved by the old SPOT GUI (`.mat`, converted by `spot.params.fromLegacyState`).
- **Save As…** writes a copy of the current settings to a file you choose.
- **Reset…** offers three separate resets: the **Simulation Settings** go back to the defaults (the old file is backed up first, and its path is shown), the **Loaded Results** are unloaded, and the **Page and View** (this browser's remembered tabs, switches and zooms) are forgotten.

## The Scenario pane

**Model**

- **Diagram**: which `.slx` in the project folder to simulate and build. **Open in Simulink** opens it in MATLAB. A new project has one diagram, named after the project.
- **Platforms**: the Chaser, Target, Obstacle and Arm that take part. The Arm is mounted on the Chaser; turning it on here also picks it on the [Designer](page:tab-designer), and the other way round. The combination becomes `platformSelection`, which tells the diagram which plants to simulate and the build which programs to make.
- **Sample Rate** (Hz): the diagram's step, `baseRate = 1/rate`. 20 Hz is the default, and what the arm controllers are tuned for.
- **Data Rate** (Hz): the PhaseSpace stream's rate. The simulation holds its measurements at this rate so the controller sees what it will see in the lab, the LQR gains are designed at it, and the Experiment tab starts the cameras at it. It is **locked**: its padlock opens a dialog that explains the consequences before it unlocks.
- **Thruster PWM** (Hz): the thrusters' PWM frequency, `PWMFreq`. It must match the Teensy firmware's fixed 5 Hz, so it is locked too.

**Timeline**

One row per phase ([1 - Experiment Logic](page:diagram-experiment-logic)): a switch (phases 2, 3.1 to 3.4 and 4 can be skipped; 0, 1 and 5 always run, so they show a lock), the phase's name, when it runs, and its duration in seconds. The bar above draws the whole run to scale, with the total time `tsim`. A skipped segment takes no time, in simulations and in the experiments built afterwards (build again after a change).

**Solver**

The solver the simulation uses: fixed step (`ode4` at `baseRate`, the default, and what the platforms run) or variable step with its tolerances. It is read from the diagram when you choose it (`spot.sim.readSolver`) and applied to each simulation without changing the diagram file. It is locked behind a padlock like the rates.

## The Platforms pane

Pick a platform at the top, then:

- **Initial Conditions**: the **Initial** pose (where the simulation starts, and where phase 2 drives the platform) and the **Home** pose (where phase 4 returns it), as x and y in metres and θ in degrees in the table frame. Click **Initial** or **Home** to show those poses on the table, where you can **drag** them (Shift+drag turns them). Viewing Options › Initial Conditions does the same.
- **Mass Properties**: the platform's mass, centre of gravity and inertia, computed from the three scale readings and the bifilar pendulum test (`spot.massprops.compute`, see [Mass Properties](page:math-mass-properties)), or typed in directly with **Override**. The card shows each thruster's lever arm about the centre of gravity, which the [thruster mixer](page:math-mixer) uses, and the LED positions.
- **Thrust** (N): the nominal thrust of one thruster, used when the project has no thruster calibration for this platform (or the Thrusters calibration card's **Use** switch is off).
- **Thrust Decay** (/valve): the thrust lost per extra valve open at once. With *n* valves open each thruster gives $1 - \text{decay}\,(n-1)$ of its thrust alone. The default, 0.112, is the Obstacle's, measured on 2026-10-14; the thruster calibration's Together stage replaces it when used.
- **Write PhaseSpace Marker File** writes `resources/tracker_positions_phasespace.txt`, each LED's offset from the platform's centre of gravity. PhaseSpace needs it so the poses it reports are those of the centre of gravity; the Experiment tab sends it to the lab console when it starts the cameras.

For the **Arm**: its **Initial Pose** (three joint angles in degrees, 0 straight out, ±90° to either side), and its **Arm Geometry**: link lengths, masses and inertias (`spot.arm.defaults`; the definitions are in `resources/ArmConfig.png`). Where it is mounted is chosen on the Designer.

## The Calibration pane

What the next simulation takes from the lab's calibration. At the top, **This Project and the Lab** compares this project's copy (`spot_calibration.json`) with the lab console's, with **Update from Lab** when the lab has a newer one. Then a card per calibration:

- **Table Slope**: the table's measured slope and each platform's air-bearing drag. While they float, the simulated platforms slide downhill and are slowed by their air bearings, as on the real table.
- **Thrusters**: each platform's eight measured thrusts and its measured thrust decay, which the diagram's mixer uses, and so the simulated plant and the experiments built from this project.
- **Arm**: each joint's measured friction and inertia, for the simulated arm.

Each card has a **Use** switch (the settings' `calibration.tableSlope`, `calibration.thrusters` and `calibration.arm`, all on by default) and shows the values the next simulation will use, read back from MATLAB after `SPOT_User_Parameters.m` has run. With the switch off, the card shows what is used instead: a level table, the Platforms pane's thrust and decay, or the arm model's defaults.

Any calibrated value can be **overridden**: type a new value in its cell. Overridden values are marked, with **↺** to go back to the measured value, and **Reset Overrides** clears a card's. They are kept in `spot_calibration.json`, so **Update from Lab** keeps them. While this pane is open, the main area shows the calibrations themselves (maps, plots and tables, as on the lab console's [Calibration Tab](page:tab-calibration)) in place of the 3D view.

## The Control pane

Per platform taking part, its **LQR** weights: `Q` on the position, attitude, velocity and rate errors, and `R` on the forces and torque. Larger Q corrects that error harder; larger R uses the thrusters more gently. The gain matrix **K** under them is recomputed in MATLAB as you type (`spot.control.lqrGains`, see [Platform Dynamics and LQR](page:math-dynamics-lqr)) and put in the workspace as `K_RED`, `K_BLACK` and `K_BLUE`, where the diagram's **Default LQR Control** reads it. A warning appears if the gains can't be computed.

With the **Arm** taking part, two more cards set its controllers (`control.ARM` in the settings, `armControl` in the workspace; see [4 - Control](page:diagram-control) and [The Arm](page:math-arm)):

- **Position · Mode 1**: the servo's position law, a PID on the joint angle error, with a P, I and D gain per joint; the **Profile Velocity** (how fast the goal moves, in 0.229 rev/min; 0 = at once), the **Profile Acceleration** (in 214.577 rev/min²; 0 = none) and the **PWM Limit** (the most it asks of the motor, out of 885).
- **Velocity · Mode 3**: the servo's velocity law, a PI on the joint speed error, with its profile acceleration and PWM limit.

The gains are in the servos' own units (P/128, I/65536, D/16 in the Dynamixel manual), so values from a servo's control table mean the same here. The defaults are retuned for the diagram's 20 Hz. Which controller runs in each phase is set by `ARM_Control_Law_Enabler` in [1 - Experiment Logic](page:diagram-experiment-logic): 1 position, 3 velocity, 2 your own torque controller, 0 off.

## The Workspace pane

MATLAB's base workspace: every variable, with a filter box; click one to inspect it (a matrix as a table, a struct's fields to click into, a timeseries as a small plot). After a run, `dataClass`, `dataPacket` and `simOut` are here ([Workspace Variables](page:workspace-variables)).

## Running

**Run Simulation** sends any edits not yet saved, then runs the diagram in your MATLAB (`spot.sim.run`). The status line shows the stages (preparing, simulating *t* of *tsim*, finishing); **Stop** ends it early. **Real Time** runs it at the clock's pace, one second of simulation per second, so the 3D view, the Live Plot and the messages show it as an experiment would.

When it finishes, the status says how much was simulated, how long it took and how many samples were logged; the command window prints Simulink's warnings; the 3D view replays the run; the Messages panel follows the replay; and the Results tab has the data. **Save Results** keeps the run in `Saved Data/` with the settings it ran with (`spot.data.saveSimulation`). A failed run shows the root cause under the Run button, with **Details** (the chain of errors, the blocks involved) and **Show Block**, which opens the diagram at the block (`spot.sim.errorDetails`, `spot.sim.showBlock`).

## Playback and Compare

The bar under the 3D view: play and pause (Space), the time and a scrubber marked with the phases, the phase being shown, and the speed. **Compare** lists the results loaded on the Results tab: switch one on to draw it over this run, as outlined platforms with dashed paths, with a time offset to line them up. **Load Results…** loads a saved run, and **Keep This Simulation** keeps the last run as a dataset when you run again. **Replay Saved Results…** (on the empty table) replays an experiment or saved simulation on its own.

## The 3D view

**3D**, **Top** and **Side** cameras; drag to orbit, right-drag to pan, scroll to zoom. **Viewing Options** holds the switches: **Initial Conditions** (Off, Initial, Home), **Labels**, **Axes**, **Use CAD** (the platforms drawn from the lab's CAD, with the Designer's docking cones and solar panels; about 1 MB, loaded the first time), **ZED2 Viewpoint** (a window showing what the Chaser's camera sees, from either lens or both), **Table Slope** (the table coloured by its height, from the table slope calibration) and **Paths**. The table is 3.5 m × 2.4 m; the origin is the corner marked with the axes ([viewer.js](file:viewer.js)).

## The MATLAB console and the Live Plot

The **MATLAB** tab is a command window into your MATLAB session: Enter runs a command, Shift+Enter starts a new line, ↑ recalls earlier ones. The console also prints what it did there, and any error. The badge shows whether MATLAB is connected, idle, or busy and with what. The **Live Plot** tab plots the signals the diagram's [Live Stream](page:diagram-live-streams) blocks send, in simulations (best with Real Time) and in experiments: tick a stream, choose the time window, **Pause** to look closer ([liveplot.js](file:liveplot.js)).

## The Messages panel

Each platform's debug message, the text the diagram writes to `RED_Logger`, `BLACK_Logger` and `BLUE_Logger`, listed each time it changes, with its time. During a replay the current one is highlighted. In an experiment the same panel shows the messages as the platforms send them ([messages.js](file:messages.js)). The **›** button folds it to a narrow strip.

## Where the values go

Every field is a key of `spot_settings.json` ([The Settings File](page:settings-file)). `spot.params.apply` turns the file into the workspace variables the diagram reads ([Workspace Variables](page:workspace-variables)), then runs `SPOT_User_Parameters.m`, where your own variables go and where you can override any of SPOT's ([Your Own Parameters](page:user-parameters)).
