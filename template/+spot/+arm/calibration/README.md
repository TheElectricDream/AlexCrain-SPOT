# Arm friction calibration

`calibrate_arm.py` measures the friction (and inertia) of the arm's three Dynamixel joints, for SPOT's
simulated arm (`spot.arm.Plant`; see the tutorial, "The Arm: Servos, Friction and Calibration").

The easiest way to run it is from the lab console's **Calibration tab** (Arm): it checks what the Chaser
needs, sends this script, runs it (`--yes`), shows its progress and keeps the result in the lab's
calibration, which projects get with **Update from Lab** (`spot_calibration.json`, in the project
folder). Below is how to run it by hand.

## In the lab

1. On RED's computer (or any computer with the arm's USB adapter): `pip install dynamixel-sdk`.
2. Make sure no SPOT program is running (it holds the serial port, `/dev/ttyUSB0`).
3. The platform rests on the table (not floating). Each joint will move up to 25 deg from where it is,
   so start with the arm clear and away from its limits. Keep the power switch at hand; Ctrl+C stops at
   once with every servo's torque off.
4. Run it (about 15 minutes for all three joints):

   ```
   python3 calibrate_arm.py
   ```

   Other options: `--joints 3` (one joint), `--tests breakaway sweep` (some tests), `--window 15` (a
   smaller range), `--port /dev/ttyUSB1`, `--help` for the rest. `--simulate` rehearses without hardware; `--yes` starts
   without asking for Enter.
5. Copy `runs/<date_time>/arm_calibration.json` to the SPOT project's folder (next to
   `spot_settings.json`). The next simulation uses it (the newer of it and the Calibration tab's). Keep the whole `runs/<date_time>` folder with the raw data.

## Afterwards, in MATLAB

```matlab
armModel = evalin('base', 'armModel');               % after any simulation (it includes the calibration)
spot.arm.compareSteps('runs/20261011_101500', armModel)  % simulated vs measured position steps
```

`python3 calibrate_arm.py --fit runs/<date_time>` fits saved data again (e.g. with another `--kt`).

## What it measures

| Test | Mode | Gives |
| --- | --- | --- |
| breakaway | current ramps from zero, 3 each way | breakaway torque (`static`) |
| sweep | constant speeds back and forth | `coulomb`, `stribeck`, `viscous` |
| inertia | a torque step from rest | the joint's inertia; SPOT subtracts the links' part (`armature`) |
| steps | position steps with SPOT's servo settings | data for `spot.arm.compareSteps` |

Torque is the measured motor current times the torque constant from the datasheet (MX-64(2.0):
6.0 N m / 4.1 A = 1.46 N m/A; MX-106(2.0): 8.4 / 5.2). MX-28 servos don't measure current and can't
be calibrated this way. Speeds below about 0.07 rad/s often stick-slip; those are left out of the fit.
