# Units, Frames and Names

## Units

- **Inside the code and the diagram:** SI units throughout: metres, seconds, kilograms, newtons, newton-metres, and **radians**.
- **In the settings and the consoles:** angles in **degrees** (the initial and home poses, the arm's initial pose), rates in Hz. `spot.params.apply` converts them when it puts them in the workspace.
- **Signal names carry their unit:** `RED_Px_m`, `RED_Rz_rad`, `RED_Vx_mpers` (m/s), `RED_Ax_mpers2` (m/s²), `RED_RzD_radpers`, `RED_Fx_N`, `RED_Tz_Nm` ([Logged Signals](page:logged-signals)).
- **In the code's comments,** units are in brackets: `[N m]`, `[rad/s]`, `[pulses]` ([The Coding Standard](page:dev-code-standard)).
- **The arm's servos** have units of their own (encoder pulses, 0.229 rev/min, 3.36 mA; `Constants.VEL_UNIT_RPM`, `CURRENT_UNIT`): [Dynamixel Units](page:math-dynamixel).
- **Every such number** (the table, the units, the addresses, the defaults) is a property of `Constants` ([Constants](page:dev-constants)); the guides name it in brackets.

## Frames

**The table frame** (the inertial frame): the origin at the corner of the table top where the 3D view draws its axes, +x along the 3.51155 m side, +y along the 2.41935 m side (about 3.5 × 2.4 m; `TABLE_X`, `TABLE_Y`), +z up. PhaseSpace reports poses in this frame, and the controllers' states, desired states and commanded forces `[Fx Fy Tz]` are all in it. A platform's attitude θ is measured from +x, counter-clockwise seen from above, wrapped to (−π, π].

**A platform's body frame:** the origin at its centre of gravity, +x out of its front face (the face with the Chaser's ZED2 camera; the control panel is on −x), +y to its left, +z up. The thrusters push along the body's ±x and ±y, so the thruster mixer rotates the commanded force into this frame before choosing the valves ([The Thruster Mixer](page:math-mixer)). Docking parts and solar panels are placed on the +x, +y and −y faces ([The Designer](page:tab-designer)).

**The arm's joints:** each angle is 0 with the link straight out (the shoulder: straight out from its mounting face), up to ±90° either side ([The Arm](page:math-arm)).

## Names

| In the code | On screen | Number (`WhoAmI`) | Address |
|---|---|---|---|
| `RED` | Chaser | 1 (`WHO_AM_I_RED`) | `192.168.1.110` (`IP_RED`) |
| `BLACK` | Target | 2 (`WHO_AM_I_BLACK`) | `192.168.1.111` (`IP_BLACK`) |
| `BLUE` | Obstacle | 3 (`WHO_AM_I_BLUE`) | `192.168.1.112` (`IP_BLUE`) |
| `ARM` | Arm (on the Chaser) | | |

`WhoAmI` is 0 in a simulation, where all three platforms run in one diagram.

## Phases

An experiment runs through phases 0 to 5; phase 3, the experiment itself, has four sub-phases:

| Phase | Name |
|---|---|
| 0 | Synchronization |
| 1 | Start Floating |
| 2 | Initial Position |
| 3.1–3.4 | Experiment (Sub-Phases 1–4) |
| 4 | Return Home |
| 5 | Hold Home |

[1 - Experiment Logic](page:diagram-experiment-logic) says what each does; the Simulation tab's Timeline sets their durations and which ones run.

## Time

`Univ_Time` is the time every phase decision is made on: Simulink's clock in a simulation, and in an experiment the **experiment clock** that StreamData sends in every PhaseSpace packet, so every platform agrees on it. It stays at 0 until the lab console starts the experiment, which is why phase 0 lasts as long as it needs to.
