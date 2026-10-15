This script measures the arm's joint friction and inertia on the real servos. The lab console's Calibration tab sends it to the Chaser and runs it there through the Chaser's agent (`spot_agent.sh`: `arm-cal-receive`, `arm-cal-start`, ...), with `--yes`, and reads its `PROGRESS` lines. It can also be run by hand on any computer with the arm's U2D2 adapter. It writes `arm_calibration.json`, which SPOT's arm model reads (`spot.arm.loadCalibration`), and the raw data as CSV and JSON files in a timestamped folder under `runs/`.

The script is older than the coding standard and hasn't been brought up to it yet: its functions have few docstrings, so these notes describe them. The [Arm Calibration](page:tab-calibration#arm) guide covers the procedure, and [The Arm](page:math-arm) the friction model it fits.

## signed

Turns a register value read as an unsigned number of `nbytes` bytes into a signed one (two's complement). The servos report goals, currents, velocities and positions as signed numbers.

## Abort

The exception that stops a test cleanly: a joint leaving its window, a servo too hot. `main` catches it, switches the torque off and restores the servos' settings.

## DxlBus.__init__

Opens the U2D2 at `device` with the Dynamixel SDK (protocol 2.0) at `baud`, and prepares one group read of the 16 bytes of state (tick, moving, PWM, current, velocity, position) for every servo in `ids`. Exits with a message if the SDK isn't installed or the port can't be opened (another program holding it, or the adapter unplugged).

## DxlBus.now

The time [s], from a monotonic clock (so a change of the computer's clock doesn't upset the timing).

## DxlBus.sleep

Waits `dt` seconds of real time.

## DxlBus._check

Raises `IOError` with the SDK's description when a transaction failed (no reply, a bad packet) or the servo reported an error.

## DxlBus.ping

Pings one servo and returns its model number (311 for an MX-64 with protocol 2.0).

## DxlBus.read

Reads one register of one servo by name (its address and size are in `ADDR`). Goals and present values are returned signed.

## DxlBus.write

Writes one register of one servo by name, as an unsigned number of the register's size (so a negative goal is sent in two's complement).

## DxlBus.close

Closes the serial port.

## SimBus.__init__

Builds three simulated joints (used with `--simulate`, and by the Calibration tab's **Rehearse**): each with an inertia, a stick-slip friction (static and Coulomb torques, a Stribeck speed, viscous friction) drawn at random around plausible values, and a Dynamixel-like controller with the real servos' default registers. The "true" values are kept in `truth`, and printed, so the fit can be checked against them.

## SimBus.now

The simulated time [s]. The simulation runs on a virtual clock, not real time, so a rehearsal takes seconds.

## SimBus.sleep

Advances the simulated time by `dt` instead of waiting.

## SimBus._advance

Advances every joint by `dt`, in steps of at most 0.25 ms (`h`).

## SimBus._step

One time step of one joint. The servo's output torque comes from its mode: in current mode the goal current, limited by what the supply voltage can drive against the back-EMF; in velocity or position mode the servo's own PI or PID law (with its profile in position mode) gives a PWM, turned into a torque and a back-EMF damping. Then the joint's speed is updated implicitly (viscous friction and back-EMF), and the stick-slip friction is applied as a torque that can stop the joint but never reverse it.

## SimBus.ping

Answers as an MX-64 (model 311), taking 1 ms of simulated time.

## SimBus.read

Returns a simulated register: the model, whether the torque is on, a 12 V supply, 35 °C, or the register's stored value.

## SimBus.write

Applies a register write to the simulated servo: torque on or off (which resets its controller), the operating mode, a goal current, velocity or position, or any other register.

## SimBus.state

Returns every joint's state as the real bus does (`{id: (tick, pwm, current, velocity, position)}`), rounded to the servos' units and with a little noise on the current, as the real sensors give.

## SimBus.close

Nothing to close.

## Arm

The arm being calibrated: the bus (real or simulated), each joint's starting position (`center`) and the window it may move in, and the registers saved before the tests (restored at the end).

## Arm.__init__

Keeps the bus and the arguments, and starts the clock the logged times are measured from.

## Arm.setup

Pings every servo (returns their models) and saves the registers the tests will change (`SAVED_REGISTERS`), so `restore` can put them back.

## Arm.restore

Switches every servo's torque off and writes back the registers saved by `setup`. A servo that fails is reported, and the others still get their torque off.

## Arm.mode

Puts one servo in an operating mode (current, velocity or position) with the given registers. The torque has to be off to change the mode, so it switches it off, writes, and switches it on again.

## Arm.goto

Moves one joint to an angle in position mode with SPOT's servo settings, and waits (up to `timeout`) until it is within 1.5° and nearly still.

## Arm.guard

Stops the test (`Abort`) if a joint has moved more than 5° beyond its window from where it started.

## Arm.t

The time since the calibration started [s], for the logs.

## write_csv

Writes rows of numbers to a CSV file with a header line.

## read_csv

Reads a CSV file written by `write_csv` back as a list of dicts of numbers.

## median

The median of a list (the middle value, or the mean of the two middle ones). Used rather than the mean so a single bad ramp or point doesn't move the result.

## fit_joint

Fits one joint's results from its data files:

- **Static friction**: from the breakaway ramps, the current [A] × torque constant at the moment each ramp moved the joint; the median each way and overall, with their spread. A ramp that reached the current cap without moving doesn't count.
- **Friction against speed**: the sweep's steady points, fitted with $\tau = \tau_c + (\tau_s - \tau_c)\,e^{-(v/v_s)^2} + b\,v$ (Coulomb, static, Stribeck speed, viscous), searching the Stribeck speed on a logarithmic grid and the rest by least squares. Points where the current opposes the motion can't be friction and are left out (and counted). When the fitted Stribeck speed is below the slowest steady speed measured, it is set to half that speed, since the data can't tell.
- **Inertia**: each torque step's acceleration (a parabola fitted to its positions) against the torque left after friction, each way; the median of the two. A large difference between the two directions is reported (something loads the joint one way).

Warnings are added for unreliable sweeps and one-sided inertia results.

## fit_all

Fits every joint (`fit_joint`), prints a summary line per joint, and writes `arm_calibration.json` with the results, the torque constants, the servos and the supply voltage.

## fit_all.f

Formats a number for the summary line, or "-" when the fit has no value.

## main

Reads the options (`--help` lists them), then either fits a saved run again (`--fit`), or runs the tests: opens the bus (real or simulated), saves the servos' settings, checks each joint's model has a known torque constant (an MX-28 can't measure current), holds every joint where it is, and runs the chosen tests on each joint in turn, printing a `PROGRESS` line before each. Between joints it returns the joint to where it started and stops if a servo is above 65 °C. Ctrl+C, SIGTERM (the Calibration tab's Stop) or an `Abort` switch the torque off and restore the servos; the exit code is 0 only when `arm_calibration.json` was written.

## main.on_signal

SIGTERM handler: marks the run as stopped and raises `KeyboardInterrupt`, so the same clean-up as Ctrl+C runs (torque off, settings restored).

## main.announce

Prints the `PROGRESS {json}` line the Calibration tab reads to show which joint and test are under way.
