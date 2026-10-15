# Mass Properties

How a platform's mass, centre of gravity, inertia and thruster offsets are found from measurements. Code: `spot.massprops`.

## Mass and centre of gravity from three scales

The platform (a 0.30 m square body) is set on three scales: **A** under the middle of the left edge, **B** under the right-back corner, **C** under the right-front corner. In the body frame (origin at the body's centre, +x forward):

| Scale | Position (x, y) [m] |
|---|---|
| A | (0, +0.15) |
| B | (−0.15, −0.15) |
| C | (+0.15, −0.15) |

![resources/COMConfig.png](images/COMConfig.png)

The total mass is the sum of the readings, and the centre of gravity is the reading-weighted average of the positions:

$$M = A + B + C, \qquad X_{cg} = \frac{-0.15\,B + 0.15\,C}{M}, \qquad Y_{cg} = \frac{0.15\,A - 0.15\,B - 0.15\,C}{M}.$$

(`spot.massprops.centerOfGravity`.)

## Thruster offsets

The eight thrusters sit 81 mm in from the body's edges, in pairs on each face (numbering in `resources/ThrusterConfig.png`). What the controller needs is the perpendicular distance from the CG to each thruster's line of action, with a sign that says which way its force turns the platform. With $e = 0.15 - 0.081 = 0.069$ m:

$$d = \begin{bmatrix} e - Y \\ -(e + Y) \\ e - X \\ -(e + X) \\ e + Y \\ -(e - Y) \\ e + X \\ -(e - X) \end{bmatrix} \times 1000 \ \text{mm}$$

Thrusters 1, 2, 5, 6 push along ±x (so their lever arm is a y distance), 3, 4, 7, 8 along ±y. These are `thruster_dist2CG_<PLATFORM>` in the workspace and the third row of the mixer's H matrix ([The Thruster Mixer](page:math-mixer)).

![resources/ThrusterConfig.png](images/ThrusterConfig.png)

## Inertia from a bifilar pendulum

The platform hangs from two parallel strings of length $L$, spaced $D$ apart, and is set swinging about the vertical axis; the period $T$ gives the moment of inertia about that axis:

$$I_{zz} = \frac{T^2\, M g\, D^2}{16\,\pi^2\, L}, \qquad g = 9.81\ \text{m/s}^2 .$$

(`spot.massprops.bifilarInertia`.) With the default readings this gives about 0.235 kg m² for the Chaser.

## LED positions

The four PhaseSpace LEDs are at the body's corners, 13 mm in from each edge; the PhaseSpace server wants them **relative to the CG** (the point it will report as the platform's position). With $c = 0.15 - 0.013 = 0.137$ m, in the order (+x,+y), (+x,−y), (−x,−y), (−x,+y):

$$\text{LED}_k = (\pm c - X_{cg},\ \pm c - Y_{cg}) .$$

`spot.massprops.trackerPositions` writes them in millimetres, one line per LED id, into `resources/tracker_positions_phasespace.txt`, which the Experiment tab sends to the lab console before the cameras start.

## Override

When `mass.override` is true, the typed `mass` and `inertia` are used in the simulation instead of the measured ones; the CG, thruster offsets and LED positions still come from the scale readings (`spot.massprops.compute`).
