# Lab Console: the Hardware Tab

Checking the platforms before an experiment: pucks on and off, each thruster on its own, a thruster check that fires all eight in turn. One card per platform. The code is [hardware.js](file:hardware.js) in the page and [hardware.py](file:hardware.py) in the server; the commands reach each platform's Teensy through SSH and [spot_agent.sh](page:lab-admin).

## A platform's card

- **The pill**: **Checking…**, **Offline** (no SSH), **No Access** (the key isn't installed on the platform), **No Teensy** (the Jetson is up but the Teensy isn't plugged in), **In Use** (another program, e.g. a running experiment, has the Teensy's port), **Interactive** (flown by Interactive Mode), **Calibrating** (driven by a calibration from the [Calibration Tab](page:tab-calibration); during the arm calibration the Chaser is kept resting, with everything off), **Thruster Check**, **Live** (something is on) or **Ready**. The line below says why.
- **Address**: the platform's `user@host` (expand **Address** to change and **Save** it; the console reprobes).
- **Pucks**: the air bearings on or off (the body "floats" on the drawing when on). Switching anything on requires **Ready**; switching off always works.
- **The thruster map**: the eight thrusters on a top view of the platform (numbered as in [Mass Properties](page:math-mass-properties): +x up, +y left); click one to fire it at the **duty cycle** typed next to the map, click again to stop. A tooltip says why a thruster can't be fired.
- **Run Check**: fires each thruster at 100 % in turn for the chosen seconds, pucks off, with a progress bar; **Cancel** stops it. The result stays on the card. Use it to hear that every valve opens, and that it is the right one.
- **Last action**: who did what, when.

## Safety

- **Esc** on this tab switches **everything off on every platform** (as long as no dialog is open).
- The lab console re-sends the command every 0.2 s while anything is on (`Constants.TEENSY_SEND_PERIOD`); the Teensy shuts every valve after 2 s without a command (`TEENSY_WATCHDOG`), so a crashed console, a lost network or an unplugged cable ends with the platform off ([teensy_pwm_control.ino](file:teensy_pwm_control.ino)).
- If another program opens the Teensy's port while the console is driving it (an experiment starting), the console lets go rather than fight; the card says **In Use**.
- A platform that is **Interactive** or **Calibrating** can't be driven by hand; switch it off here to end its flight or the calibration (for the arm calibration, this also stops the script and switches the servos' torque off).
