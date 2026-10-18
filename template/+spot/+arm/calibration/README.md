# Arm friction calibration

`calibrate_arm.py` measures the friction, back-EMF and inertia of the arm's three Dynamixel joints, for
SPOT's simulated arm (`spot.arm.Plant`; the Documentation tab's "The Arm" page). It drives the servos
the way SPOT does, in PWM (voltage) control, and measures only with PWM and the encoders: the servos'
current readings are too coarse and noisy at the arm's small torques to be used.

The easiest way to run it is from the lab console's **Calibration tab** (Arm): it checks what the Chaser
needs, sends this script, runs it (`--yes`), shows its progress and keeps the result in the lab's
calibration, which projects get with **Update from Lab** (the `LAB_ARM_*` constants in the project's
`Constants.m`). Below is how to run it by hand.

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
   smaller range), `--max-pwm 200` (a lower cap), `--port /dev/ttyUSB1`, `--help` for the rest.
   `--simulate` rehearses without hardware; `--yes` starts without asking for Enter. The script runs on its own
   on the Chaser, so it keeps copies of the servo units and SPOT's servo settings; `Constants.verify`
   checks them against `Constants.m`, where SPOT keeps every such number.
5. To use it in a project, let the lab console keep it (the Calibration tab's Arm run does this) and
   take it with **Update from Lab**; on the bench, `spot.arm.loadCalibration('runs/<date_time>/arm_calibration.json', armModel)`
   applies it to a model and `spot.arm.compareSteps` checks it. Keep the whole `runs/<date_time>` folder
   with the raw data.

## Afterwards, in MATLAB

```matlab
armModel = evalin('base', 'armModel');               % after any simulation (it includes the calibration)
spot.arm.compareSteps('runs/20261016_101500', armModel)  % simulated vs measured position steps
```

`python3 calibrate_arm.py --fit runs/<date_time>` fits saved data again (e.g. with another
`--back-emf`).

## What it measures

At the joint, a servo in PWM control gives `torque_per_volt (V - back_emf w)`, with V = PWM / 885 x the
supply. At rest a voltage is a torque; turning, the back-EMF brakes the joint like viscous friction.

| Test | Mode | Gives |
| --- | --- | --- |
| breakaway | PWM ramps from zero, 3 each way | breakaway torque (`static`): the PWM where the joint first moves |
| sweep | fixed PWM levels about the breakaway, back and forth, each after a short kick | the steady speed at each: `coulomb`, `stribeck`, and the speed coefficient (`back_emf` and `viscous`) |
| inertia | a PWM step from rest, each way | the joint's inertia (its step simulated with the fitted friction); SPOT subtracts the links' part (`armature`) |
| steps | position steps with SPOT's servo settings | data for `spot.arm.compareSteps` |

Torques come from the datasheet's torque per volt (MX-64(2.0): 6.0 N m / 12 V = 0.5 N m/V). The
back-EMF and viscous friction both brake in proportion to speed, so PWM control measures only their sum
(the speed coefficient); the datasheet's back-EMF (MX-64(2.0): 12 V at 63 rev/min, 1.82 V s/rad) is
taken as the back-EMF, the rest as viscous friction; a speed coefficient below the datasheet's (an
MX-64(2.0) on the bench gave 1.49 V s/rad) is taken as all back-EMF. Below the breakaway a joint
stick-slips at the slowest speeds; those levels are left out of the fit. The wrist settles within a few milliseconds of a
PWM step, close to the time between reads, so its inertia is only roughly known (the result says so).

Calibrations from before (version 1) measured the motor current under current control; SPOT no longer
uses them, and simulations warn until the arm is calibrated again.
