# Mass Properties

How a platform's mass, centre of gravity, inertia and thruster offsets are found from measurements. Code: `spot.massprops`. The body's dimensions and the default readings are the platforms section of `Constants` ([Constants](page:dev-constants)), named in brackets.

## Mass and centre of gravity from three scales

The platform (a 0.30 m square body: `BODY_HALF_SIDE` = 0.15 m) is set on three scales: **A** under the middle of the left edge, **B** under the right-back corner, **C** under the right-front corner. In the body frame (origin at the body's centre, +x forward):

| Scale | Position (x, y) [m] |
|---|---|
| A | (0, +0.15) (`SCALE_A`) |
| B | (−0.15, −0.15) (`SCALE_B`) |
| C | (+0.15, −0.15) (`SCALE_C`) |

![COMConfig.png](images/COMConfig.png)

The total mass is the sum of the readings (the defaults `SCALES_RED`, `SCALES_BLACK`, `SCALES_BLUE`, in kg), and the centre of gravity is the reading-weighted average of the positions:

$$M = A + B + C, \qquad X_{cg} = \frac{-0.15\,B + 0.15\,C}{M}, \qquad Y_{cg} = \frac{0.15\,A - 0.15\,B - 0.15\,C}{M}.$$

(`spot.massprops.centerOfGravity`.)

## Thruster offsets

The eight thrusters sit 81 mm in from the body's edges (`THRUSTER_INSET`), in pairs on each face (numbered as in the picture below). What the controller needs is the perpendicular distance from the CG to each thruster's line of action, with a sign that says which way its force turns the platform. With $e = 0.15 - 0.081 = 0.069$ m (`THRUSTER_ARM`):

$$d = \begin{bmatrix} e - Y \\ -(e + Y) \\ e - X \\ -(e + X) \\ e + Y \\ -(e - Y) \\ e + X \\ -(e - X) \end{bmatrix} \times 1000 \ \text{mm}$$

Thrusters 1, 2, 5, 6 push along ±x (so their lever arm is a y distance), 3, 4, 7, 8 along ±y. These are `thruster_dist2CG_<PLATFORM>` in the workspace and the third row of the mixer's H matrix ([The Thruster Mixer](page:math-mixer)).

![ThrusterConfig.png](images/ThrusterConfig.png)

## Inertia from a bifilar pendulum

The platform hangs from two parallel strings of length $L$, spaced $D$ apart, and is set swinging about the vertical axis; the period $T$ gives the moment of inertia about that axis:

$$I_{zz} = \frac{T^2\, M g\, D^2}{16\,\pi^2\, L}, \qquad g = 9.80665\ \text{m/s}^2 .$$

(`spot.massprops.bifilarInertia`; $g$ is `Constants.G`.) With the default readings (`BIFILAR_PERIOD_RED` 3.00 s, `BIFILAR_LENGTH` 2.56 m, `BIFILAR_SPACING` 0.28 m) this gives about 0.235 kg m² for the Chaser (`INERTIA_RED`).

## LED positions

The four PhaseSpace LEDs are near the body's corners; the PhaseSpace server wants them **relative to the CG** (the point it will report as the platform's position). Where each LED is on the body, $(x_k, y_k)$ from the body centre, is kept in the project's `Constants.m` (`LED_XY_RED`, `_BLACK`, `_BLUE`) and edited in the Simulation tab's **Thruster and LED Layout** window; by default they sit 13 mm in from each edge (`LED_INSET`), at $(\pm 0.137, \pm 0.137)$ m (`LED_DEFAULT_OFFSET`; `spot.massprops.markerLayout`). From the CG:

$$\text{LED}_k = (x_k - X_{cg},\ y_k - Y_{cg}) .$$

`spot.massprops.trackerPositions` turns them into the lab console's marker file, in millimetres, one line per LED id. `spot.params.apply` hands it to the console, and the Experiment tab's **Start Cameras** sends it to the lab console, which writes `tracker_positions_phasespace.txt` next to StreamData before it starts. (This replaces the Platforms pane's old **Write PhaseSpace Marker File** button and `spot.massprops.writeTrackerPositions`, which wrote the file into the project's `resources/` folder for the Experiment tab to copy across; the project keeps no copy of the file now.)

## Override

When `mass.override` is true, the typed `mass` and `inertia` are used in the simulation instead of the measured ones; the CG, thruster offsets and LED positions still come from the scale readings (`spot.massprops.compute`).
