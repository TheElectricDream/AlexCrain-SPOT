# spot.control.lqrGains

**File:** `+spot/+control/lqrGains.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/lqrGains.m -->

## What it is for

The 3×6 gain `K` of the discrete LQR, $u = -Kx$, from the model and the diagonal weights on the Control pane. Maths: [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]].

## How to call it

```matlab
K = spot.control.lqrGains(sys, [1 1 0.05 10 10 1], [6 6 6]);
```

## Line by line

**Line 6.** `dlqr` solves the discrete Riccati equation for `A`, `B` with `Q = diag(Q)` and `R = diag(R)` and returns the gain. A zero or negative weight makes it fail; [[spot.params.apply|spot-params-apply]] catches that and shows it as a warning.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+control/lqrCommand.m`, `+spot/+control/verifyConsoleController.m`, `+spot/+params/apply.m`
- **Uses:** [[spot.control.stateSpace|spot-control-stateSpace]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/lqrGains.m</b> (7 lines; click to show)</summary>

```matlab
   1  function K = lqrGains(sys, Q, R)
   2  %LQRGAINS Discrete LQR gain matrix for a platform.
   3  %   K = spot.control.lqrGains(sys, Q, R) with sys from spot.control.stateSpace, the state weights
   4  %   Q = [x y theta xdot ydot thetadot] and input weights R = [Fx Fy Tz] (diagonals). Returns the
   5  %   3x6 gain of u = -K*x. Needs the Control System Toolbox.
   6  K = dlqr(sys.A, sys.B, diag(Q), diag(R));
   7  end
```

</details>
<!-- /listing -->
