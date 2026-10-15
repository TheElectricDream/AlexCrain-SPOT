# spot.control.stateSpace

**File:** `+spot/+control/stateSpace.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/stateSpace.m -->

## What it is for

The discrete model of one platform the LQR is designed on: a planar rigid body (three double integrators) sampled with a zero-order hold. Maths: [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]].

## How to call it

```matlab
sys = spot.control.stateSpace(13.75, 0.2354, 0.1);   % mass [kg], inertia [kg m^2], Ts [s]
sys.A, sys.B, sys.C, sys.D                          % 6x6, 6x3, 6x6, 6x3
```

Needs the Control System Toolbox (`ss`, `c2d`).

## Line by line

**Line 6.** The continuous `A`: the top-right identity says velocity is the derivative of position; everything else is zero (no forces without input).

**Line 7.** `B`: the inputs act on the velocities, scaled by 1/m, 1/m, 1/I.

**Lines 8–9.** Full-state output, no feed-through.

**Line 10.** Discretise with a zero-order hold at `Ts`.

**Line 11.** Return the four matrices as a plain struct (so the rest of the code doesn't need the toolbox's `ss` objects).

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+control/lqrGains.m`, `+spot/+control/verifyConsoleController.m`, `+spot/+params/apply.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/stateSpace.m</b> (12 lines; click to show)</summary>

```matlab
   1  function sys = stateSpace(mass, inertia, Ts)
   2  %STATESPACE Discrete planar rigid-body model of one platform.
   3  %   sys = spot.control.stateSpace(mass, inertia, Ts) returns the zero-order-hold discretisation
   4  %   (sample time Ts [s]) of the double integrator with states [x y theta xdot ydot thetadot] and
   5  %   inputs [Fx Fy Tz]. sys has fields A, B, C, D. Needs the Control System Toolbox.
   6  A = [zeros(3) eye(3); zeros(3, 6)];
   7  B = [zeros(3); diag([1/mass, 1/mass, 1/inertia])];
   8  C = eye(6);
   9  D = zeros(6, 3);
  10  d = c2d(ss(A, B, C, D), Ts, 'zoh');
  11  sys = struct('A', d.A, 'B', d.B, 'C', d.C, 'D', d.D);
  12  end
```

</details>
<!-- /listing -->
