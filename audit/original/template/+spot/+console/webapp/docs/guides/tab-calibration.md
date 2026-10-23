# The Calibration Tab

Measurements of the lab that make simulations behave as the experiments do. The **lab console** (the TX2) runs them on its Calibration tab and keeps the lab's calibration; each **project** keeps a copy, `spot_calibration.json`, which its simulations use. On the local console the same views are on the Simulation tab's **Calibration** pane ([The Simulation Tab](page:tab-simulation)). Code: [calibration.py](file:calibration.py) and [calfit.py](file:calfit.py) (server), [calibration.js and calviz.js](file:calibration.js) (page); the maths is on [The Calibration Fits](page:math-calibration).

Three calibrations, chosen at the top right:

| | What it measures | Used by the simulation |
|---|---|---|
| **Table Slope** | The table top's height surface (it isn't perfectly level) and each platform's air-bearing drag | The floating platforms slide downhill (`spot.sim.tableSlope`) |
| **Thrusters** | Each thruster's real thrust, which way it pushes, and a check of the platform's assumed CG and inertia; then, together, the thrust decay | The mixer and the simulated plant use each thruster's thrust and the decay; so does the lab console's controller (Interactive Mode) |
| **Arm** | Each joint's friction (breakaway, against speed) and inertia, with the real servos ([calibrate_arm.py](file:calibrate_arm.py), run on the Chaser) | The simulated arm's friction and inertia |

## Table Slope

**How It Works.** With PhaseSpace streaming and the platform resting on the table:

1. **Rest.** PhaseSpace records the platform resting for 2 s: the cameras' noise.
2. **Float.** The air bearings come on with the thrusters off: it drifts from where it is.
3. **Drift Map.** It flies (the diagram's own controller, as [Interactive Mode](page:interactive-mode)) to each point of a grid over the table, settles, and drifts again with the thrusters off.
4. **Fit.** The table's height surface and the air bearings' drag are fitted to every drift, and saved in the lab's calibration.

A drift ends after its time, or **at once (pucks off)** when the platform's centre comes within the edge margin of an edge, within 0.45 m of another platform, or goes faster than 0.15 m/s; the platform then stops on its pucks. Grid points next to other platforms (or whose path passes them) are skipped, so the other platforms may stay where they are.

**Run** (lab console): the platform, the grid (columns × rows), the drift time, the edge margin and the model (**Auto**, **Surface**, **Plane**), then **Start Table Calibration** (only while PhaseSpace streams). The default 3 × 2 grid with 12 s drifts takes about 4 minutes. **Stop** (or Esc on this tab) ends it at once with everything off; so does Esc or switching the platform off on the [Hardware tab](page:tab-hardware), where the platform shows as *Calibrating* while it runs.

While it runs, the map shows the grid (the point under way in yellow), the drifts so far, the platform and its goal, and the steps tick off on the left.

**The result.** The map shows the table's height in colour (blue low, red high; the colour bar gives the range in µm, the lowest point at 0), its contour lines, which way is downhill (the arrows, longer where it is steeper) and the drifts it was fitted to (a red cross where a drift was cut short). Below it:

- **Tilt at Centre** and **Steepest** [mrad]: 1 mrad is a 1 mm drop per metre, and pulls a floating platform at 0.0098 m/s².
- **Downhill**: which way at the centre, from +x towards +y.
- **Fit Residual** [mm]: how far the drifts are from the fitted model; close to the cameras' noise means the model fits.
- The air bearings' drag per platform [1/s]. Short drifts can't tell drag from slope; the drag then shows as *not resolved* and 0 is used. Longer drifts (12 s or more) resolve it.

**When a run isn't saved.** A run replaces the lab's calibration only if its data can be trusted: one where PhaseSpace's positions jumped or went back in time (mis-tracking, or a second stream), or where the drifts don't follow the model at all, fails with the reason and leaves the lab's calibration as it was. A fit that follows the drifts less well than the cameras' noise suggests is kept, with a warning under the map.

**Surface or plane.** A smooth surface (quadratic: tilt plus a bow in each direction) needs at least three drifts that between them cover at least 40 % of the table each way; with fewer, Auto fits a plane (one tilt everywhere).

## Thrusters

**How It Works.** With PhaseSpace streaming and the platform resting on the table, for each thruster 1 to 8 (and each repeat):

1. **Centre.** The pucks come on and the diagram's own controller flies the platform to the table's centre.
2. **Rest.** Pucks off: the platform sets down and comes to rest for 5 s.
3. **Fire.** The pucks come on with the thruster firing at full thrust for the burn time (2 s by default); PhaseSpace records every frame of the push.

Clear the table around the centre before starting.

Each burn's thrust is worked out from the push it gave: the platform's mass times its acceleration, with the table's slope (the table calibration) taken out, and the turn it gave against its inertia. So the result is only as good as the platform's **mass, inertia and centre of gravity**: before it starts, the tab asks you to confirm them (SPOT's defaults, as the lab console's controller uses them, which you can change for this run) and tick **I understand**.

**Run** (lab console): the platform, the repeats (2 by default: their thrusts are averaged, and their spread shown), the burn time and the edge margin, then **Start Thruster Calibration…**. Eight thrusters with two repeats take about 4 minutes. If there is no table calibration yet, the tab says so: calibrate the table first for the best thrusts. A burn ends early near an edge; **Stop** (Esc), the Hardware tab and losing PhaseSpace stop it as for the table.

**The result**, for the platform picked at the top right:

- **The platform from above**, with each thruster's push as an arrow onto its nozzle: its length is the thrust, its direction the way it really pushes, and its colour how it compares with nominal (green within 3 %, amber or red weaker, blue stronger).
- **The bars**: each thruster's thrust with its spread, against nominal (dashed).
- **The table**: thrust, ±, against nominal, **Turned** (how far the push is turned from the thruster's direction) and **Torque Ratio** (the torque the burn gave over the torque the mixer expects from that thrust: far from 1, the CG or the inertia is off).
- **Each burn's path** from where the platform started, at one scale.

A thruster without a good burn (its frames jumped, its motion didn't follow a constant thrust, or the thrust was implausible) keeps the nominal thrust and is marked.

**Where the thrust is used.** The lab console's controller (Interactive Mode, and the calibrations' own flights) uses the lab's calibrated thrust at once. A project uses its copy (after **Update from Lab**) when the Thrusters card's **Use** switch on its Calibration pane is on (the default): the diagram's mixer and, through it, the simulated plant get each thruster's thrust (`F_thrusters_<PLATFORM>`), as do experiments built from it.

## Thrusters Together: the Thrust Decay

The thrusters share one regulated air supply, so each gives less thrust when several are open at once. The second stage, **Start Together Calibration…**, measures it for a platform that already has a thruster calibration. Each firing opens thrusters 1, then 1–2, 1–3, up to 1–8 at once (from the table's centre, after a rest, as for a single burn), and its push is compared with the sum of what those thrusters give alone.

The result is the straight line

$$r(N) = s\,\bigl(1 - m\,(N - 1)\bigr),$$

where $r$ is the push with $N$ valves open over the sum of their single pushes and $m$ is the **thrust decay**, the fraction lost per extra valve open. All eight together give no net push (opposite thrusters cancel), so that firing is only a check, and firings whose pushes cancel are left out. The decay is kept with the platform's thruster calibration and used by Interactive Mode, the mixer and the simulation ([The Thruster Mixer](page:math-mixer)). Until a platform's decay is measured, the Obstacle's (11.2 % per extra valve, measured 2026-10-14) is used.

## Previous Runs

Every run's raw data is kept on the lab console, so a table, thruster or Together run can be **fitted again** without flying the platform: **Previous Runs** lists them (date, platform, how many drifts or burns), with **Load Run**. A table run can be refitted with another model (Auto, Quadratic or Plane). Runs stopped early (out of air, say) are flagged, and the fit uses what they recorded. Loading a run makes its result the lab's calibration, as a new run would. A Together run fitted with an earlier curve shows a note: load it again to fit the straight line.

## Arm

The arm calibration runs [calibrate_arm.py](file:calibrate_arm.py) on the Chaser, which has the arm's U2D2, from this tab. It drives the servos as SPOT does, in **PWM (voltage) control**, and measures with PWM and the encoders only: the servos' current readings are too coarse and noisy at the arm's small torques ([The Arm](page:math-arm)). Torques come from the PWM through the datasheet's torque per volt (0.5 N m/V for an MX-64). Each joint in turn, while the others hold:

1. **Breakaway.** The PWM creeps up from zero until the joint moves, three times each way: the static friction (breakaway torque).
2. **Speed Sweep.** The joint turns back and forth at fixed PWM levels about its breakaway, each started with a short kick; the speed it settles at shows the friction at that speed (Coulomb, Stribeck) and the speed coefficient: the back-EMF and viscous friction, which brake it alike and so can't be told apart; the datasheet's back-EMF splits them.
3. **Inertia.** A PWM step from rest, each way: how fast it gets up to speed gives the joint's inertia (link and rotor), from a simulation of the step with the friction just fitted for that direction (a joint can be harder to turn one way; the sweep gives each way's Coulomb friction too). The wrist gets up to speed within a few milliseconds, close to the time between reads, so its inertia is only roughly known (the result says so).
4. **Position Steps.** Goals of 10, 0, −10, 20, 0, −20, 5, 0° with SPOT's servo settings, for `spot.arm.compareSteps` to check the model against.

The figure at the top of How It Works shows what the test picked (click one) does to a joint; during a run, the test under way.

**The Chaser's Setup.** Opening the section asks the Chaser what the script needs (**Check Again** asks again): Python 3, the Dynamixel SDK, the U2D2 at `/dev/ttyUSB0`, access to it, and nothing else holding it; each missing item says what to do (`pip3 install --user dynamixel-sdk`; `setup_arm_serial.sh` for the access and the 1 ms latency timer). A real run can't start until they are all there. If the Chaser's agent is older than the console, the check says to run `setup_hardware_link.sh`.

**Run** (lab console): the joints and tests (all by default), then **Start Arm Calibration…**; a dialog asks you to confirm that the Chaser rests on the table, the arm is clear (each joint moves up to 25° either way) and someone has the power switch at hand. Before anything moves, the script checks each joint against its servo's position limits (±90°): in PWM control the servos don't keep to them, so an experiment can leave a joint past one, where position mode can't hold it. A joint past its limits, or one whose tests would take it past them, stops the run with which joint is where; move it back with the torque off. All three joints and all four tests take about 15 minutes. While it runs the console keeps the Chaser resting (the Hardware tab shows it as *Calibrating*, and nothing can float it); **Stop** (or Esc) switches the servos' torque off at once and restores their settings, and so does switching the Chaser off on the Hardware tab.

**Rehearse (Simulated Servos)** runs the same script with its simulated servos instead: it needs none of the hardware, moves nothing, takes under a minute, and its result is shown (marked *Rehearsal · Not Saved*) but never becomes the lab's.

**The result.** Each joint's static friction (and each way), Coulomb friction, Stribeck speed, viscous friction, back-EMF and inertia; the friction against speed for every joint (the measured points, what the motor gave at each steady speed, and the fitted curve); and each joint's position steps (goal and measured). The script warns about unreliable speed sweeps and one-sided inertia results, and its setup check scans the bus for the servos. The run's raw data stays on the lab console (`~/SPOTCalibration/<stamp>_arm_RED/`). Projects take it with **Update from Lab**, and the simulated arm uses it when the Arm card's **Use** switch is on (the newer of it and an `arm_calibration.json` copied by hand into the project folder).

> [!NOTE]
> Until October 2026 the servos ran in current (torque) control, and the calibration measured the friction from their current readings. Those readings proved unusable at the arm's torques (the wrist servo, ID 3, even read differently in each direction), so SPOT moved to PWM control. A calibration from before (version 1) is marked *Current Control · Not Used*: simulations ignore it (and warn), and projects aren't offered it. Until the arm is calibrated again, every joint uses one MX-64(2.0)'s friction and inertia measured unloaded on the bench with this script (2026-10-17; `spot.arm.defaults`).

## The 3D view

Once the console has a table calibration, the 3D view's **Viewing Options** has a **Table Slope** switch, which colours the table top by its height (with contour lines), on every tab with the 3D view.

## This project and the lab (local console)

On the local console the Simulation tab's **Calibration** pane shows the project's calibration, with **This Project and the Lab**: when each was measured, **Update from Lab** (copies the lab's into the project), and **Ask When the Lab's Is Newer**. Each calibration has its card, with its **Use** switch and the values the next simulation uses, any of which you can override ([The Simulation Tab](page:tab-simulation)).

Whenever the local console sees the lab console (on the lab network) and the lab's calibration is newer than the project's, it asks once: **Update** copies it in (the next simulation uses it), **Not Now** keeps the project's and doesn't ask again until the lab's changes, and **Never Ask Again** stops asking for this project (the switch above turns it back on).

In the simulation, the table's slope acts only while the platforms float (`Float_State`), as on the real table. With the Table Slope card's switch off, or without a table calibration, the table is level.

## Where things are

| What | Where |
|---|---|
| The lab's calibration | `~/.spot_calibration.json` on the lab console |
| Each run's raw data | `~/SPOTCalibration/<date>_<time>_<table or thrusters>_<PLATFORM>/run.json` (every PhaseSpace frame of every drift or burn), `…_arm_RED/` (the arm script's CSV and JSON files) on the lab console |
| A project's copy | `spot_calibration.json`, next to `spot_settings.json` (`spot.params.loadCalibration`) |
| The API | `GET /api/calibration` (with `assumed`, the mass properties the lab console assumes); lab console: `POST /api/calibration/table/start {platform, nx, ny, drift, margin, max_speed, model}`, `/thrusters/start {platform, acknowledged: true, repeats, burn, margin, assumed: {mass, inertia, cg_mm}}`, `/arm/start {joints, tests, simulate, acknowledged}`, `/decay/start`, `/table/load`, `/thrusters/load`, `/decay/load` (Previous Runs), `GET /api/calibration/<table|thrusters|decay>/runs`, `GET /api/calibration/arm/check`, `/stop`, `/clear {section, platform}`; local console: `/api/calibration/take`, `/sync {ask | declined}`, `/override {path, value}` or `{clear}`. All are on [API Routes and MATLAB Commands](page:routes). |
