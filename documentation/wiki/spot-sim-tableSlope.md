# spot.sim.tableSlope

**File:** `+spot/+sim/tableSlope.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/tableSlope.m -->

## What it is for

The granite table isn't perfectly level, so a floating platform slides downhill, and its air bearings drag it a little. The [[Calibration tab|Calibration-Tab]] measures both (the table slope calibration); this is the acceleration they give a platform, which the simulated plant adds to the thrusters' while the platform floats: the diagram's `BLACK_dynamics`, `BLUE_dynamics` and `RED_dynamics` ([[Simulate Plant Dynamics|Diagram-Simulate-Plant-Dynamics]]) and [[spot.arm.Plant|spot-arm-Plant]] for the Chaser with its arm. The maths is on [[The Calibration Fits|Math-Calibration]].

## How to call it

```matlab
a = spot.sim.tableSlope(position, velocity, tableSlope, damping)   % [ax; ay] [m/s^2], table frame
```

`position` is `[x y]` (or the platform's whole `[x y theta]`) [m], `velocity` `[xdot ydot]` [m/s]; `tableSlope` is the workspace variable of that name, `[a1 a2 a3 a4 a5 xc yc]`, and `damping` the platform's `tableDamping_<PLATFORM>` [1/s], both set by [[spot.params.apply|spot-params-apply]] from the project's calibration ([[spot.params.loadCalibration|spot-params-loadCalibration]]). With `tableSlope = [0 0 0 0 0 1.75 1.2]` and no drag (the defaults) it is zero: a level table. It is code-generation compatible (`%#codegen`), so the diagram's MATLAB Function blocks call it directly.

## Line by line

**Line 14.** Standard gravity [m/s²], as `calfit.py` (`G`) uses for the fit.

**Lines 15–16.** The position from the height surface's centre (`xc`, `yc`: the table's centre, 1.75 m, 1.2 m): `u`, `v`.

**Lines 17–18.** The table top's height is `h = a1 u + a2 v + a3 u² + a4 u v + a5 v²`; a floating platform accelerates at `−g ∂h/∂x` and `−g ∂h/∂y`, i.e. `−g (a1 + 2 a3 u + a4 v)` and `−g (a2 + a4 u + 2 a5 v)`, and the air bearings take `damping × velocity` off each. (A plane, the fit with few drifts, has `a3 = a4 = a5 = 0`: the same pull everywhere.)

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/parameters.m`, `+spot/+params/apply.m`, `+spot/+params/loadCalibration.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.params.loadCalibration|spot-params-loadCalibration]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/tableSlope.m</b> (19 lines; click to show)</summary>

```matlab
   1  function a = tableSlope(position, velocity, slope, damping)
   2  %TABLESLOPE The table's pull on a floating platform: its slope and the air bearings' drag.
   3  %   a = spot.sim.tableSlope(position, velocity, tableSlope, damping) is the acceleration [ax; ay]
   4  %   [m/s^2] in the table frame of a platform floating at POSITION [x y] [m] with VELOCITY
   5  %   [xdot ydot] [m/s], on the table top whose height is (the table slope calibration, Calibration
   6  %   tab; spot.params.loadCalibration)
   7  %       h = a1*u + a2*v + a3*u^2 + a4*u*v + a5*v^2,   u = x - xc, v = y - yc
   8  %   with tableSlope = [a1 a2 a3 a4 a5 xc yc]: it slides downhill at -g * grad(h), and its air
   9  %   bearings drag it with -DAMPING * velocity (DAMPING [1/s]). All zeros: a level table, no drag.
  10  %   The diagram's plant blocks (BLACK_dynamics, BLUE_dynamics, RED_dynamics) and spot.arm.Plant add
  11  %   it to the platforms' motion.
  12  
  13  %#codegen
  14  g = 9.80665;
  15  u = position(1) - slope(6);
  16  v = position(2) - slope(7);
  17  a = [-g * (slope(1) + 2 * slope(3) * u + slope(4) * v) - damping * velocity(1);
  18       -g * (slope(2) + slope(4) * u + 2 * slope(5) * v) - damping * velocity(2)];
  19  end
```

</details>
<!-- /listing -->
