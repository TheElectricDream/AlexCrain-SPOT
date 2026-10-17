# Safety and Emergencies

## The E-stop

The **E-stop** button is on the ground station table. It is a system of its own, separate from the software it protects, so it works whatever the software does: a radio transmitter in the button, and a receiver on each platform that cuts the power to its thruster valves.

- **Press it** to stop: every platform's thrusters are disabled at once (the platforms can still float; the air bearings stay on).
- **Twist it clockwise** until it clicks up to release it.

Each platform's E-stop light says what it receives:

| Light | Meaning |
|---|---|
| solid blue | the "go" signal: the thrusters can fire |
| solid red | the E-stop is pressed: the thrusters are off |
| flashing red | no signal for 5 s, or a fault in the E-stop circuit: the thrusters are off |

In an emergency, first make sure the platforms won't leave the table (get hold of them if you must), then have someone press the E-stop, then stop the experiment (**Stop** on the Experiment tab, or Esc on the lab console's Hardware tab).

## Software stops

The consoles stop a platform in several ways, but the E-stop is the one that does not depend on them:

- **Stop** on the Experiment tab stops the programs and sends every Teensy an all-off.
- **Esc** on the lab console's Hardware tab switches everything off on every platform; **Esc** on the PhaseSpace tab ends Interactive Mode; **Stop** (or Esc) on the Calibration tab ends a calibration with everything off.
- Interactive Mode and the calibrations stop a platform PhaseSpace loses, or one that leaves the table.
- Each **Teensy** shuts every valve when no command has arrived for 2 s (`Constants.TEENSY_WATCHDOG`): a crashed program, a lost network or an unplugged cable ends with the thrusters off.

## Looking after the air bearings

The pucks are expensive and easily damaged (in 2017 all of the Chaser's and Target's had to be replaced).

- Wear gloves before touching their faces.
- Clean them before putting a platform on the table ([Preparing the Lab](page:lab-procedures)).
- Never set a platform on the table without floating it.
- Don't let a platform slide off the table during an experiment.

## Leaks

Small leaks are common. When you hear one, judge how bad it is: a small one that doesn't affect the experiment can wait. If a leak makes a platform move by itself, stop the experiments until it is fixed.

## The arm

During the arm calibration, each joint moves up to 25° either way: the Chaser must rest on the table (not float), the arm must be clear, and someone must have the arm's power switch at hand. **Stop** (or Esc) on the Calibration tab switches the servos' torque off at once.
