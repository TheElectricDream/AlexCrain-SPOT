# spot.plotting.armPosition

**File:** `+spot/+plotting/armPosition.m` · **Package:** [[spot.plotting|spot-plotting]]

<!-- source: +spot/+plotting/armPosition.m -->

## What it is for

Forward kinematics of the arm in the table frame: given the Chaser's pose and the three joint angles, where the shoulder, elbow, wrist and end-effector tip are. The 3D view, the table-view figure and the initial-pose marker all use it. It works on one pose or on N poses at once.

## The geometry

With the Chaser at $(x, y, \theta)$ and the arm's geometry ($\Phi$, $b_0$, $a_i$, $b_i$, see [[The Arm|Math-Arm]]), the shoulder sits at

$$\beta_1 = \theta + \Phi, \qquad s = (x + b_0\cos\beta_1,\ y + b_0\sin\beta_1),$$

and each link points along the running sum of angles:

$$\beta_2 = \theta + q_1 + \tfrac{\pi}{2} + \delta, \quad \beta_3 = \beta_2 + q_2, \quad \beta_4 = \beta_3 + q_3,$$

$$e = s + (a_1 + b_1)(\cos\beta_2, \sin\beta_2), \quad w = e + (a_2 + b_2)(\cos\beta_3, \sin\beta_3), \quad t = w + (a_3 + b_3)(\cos\beta_4, \sin\beta_4),$$

where $\delta$ is `q1Offset`, the turn of the shoulder's zero for the face it is mounted on (0 for the default +y face: the $\pi/2$ already points the arm out of that face).

## How to call it

```matlab
P = spot.plotting.armPosition([x y theta], [q1 q2 q3], s.mass.arm);          % 4 x 2: shoulder, elbow, wrist, tip
P = spot.plotting.armPosition(poses, angles, spot.arm.mountGeometry(s.mass.arm));   % N poses: 4 x 2 x N
```

## Line by line

**Line 10.** Split the poses into columns.

**Lines 11–16.** The angles above: the shoulder mount direction, then each link's direction (the offset only if the geometry carries one).

**Lines 17–20.** Walk out from the body centre: shoulder, elbow, wrist, tip, each the previous point plus that link's length along its direction.

**Lines 21–24.** Pack as 4×2×N (x in column 1, y in column 2).

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/cad.js`, `+spot/+data/toFigure.m`, `+spot/+data/trajectory.m`, `+spot/+params/apply.m`, `+spot/+plotting/drawArm.m`, `+spot/+sim/results.m`
- **Uses:** [[spot.arm.mountGeometry|spot-arm-mountGeometry]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+plotting/armPosition.m</b> (25 lines; click to show)</summary>

```matlab
   1  function P = armPosition(base, q, arm)
   2  %ARMPOSITION Joint positions of the robotic arm on RED, in the table frame.
   3  %   P = spot.plotting.armPosition([x y theta], [q1 q2 q3], arm) with RED's position [m] and
   4  %   attitude [rad], the shoulder, elbow and wrist angles [rad] and the arm geometry
   5  %   (s.mass.arm: Phi, b0, a1, b1, a2, b2, a3, b3). Returns a 4x2 matrix of [x y] for the
   6  %   shoulder, elbow, wrist and end effector.
   7  %   Accepts column vectors of poses and angles (N samples) too; P is then 4x2xN.
   8  %   For an arm mounted elsewhere, give the geometry from spot.arm.mountGeometry (its q1Offset turns
   9  %   the arm with its mount face).
  10  x = base(:, 1); y = base(:, 2); th = base(:, 3);
  11  b1 = th + arm.Phi;                  % shoulder mount direction
  12  offset = 0;
  13  if isfield(arm, 'q1Offset'), offset = arm.q1Offset; end
  14  b2 = th + q(:, 1) + pi/2 + offset;  % proximal link
  15  b3 = b2 + q(:, 2);                  % distal link
  16  b4 = b3 + q(:, 3);                  % end effector
  17  sx = x + arm.b0 * cos(b1);              sy = y + arm.b0 * sin(b1);
  18  ex = sx + (arm.a1 + arm.b1) * cos(b2);  ey = sy + (arm.a1 + arm.b1) * sin(b2);
  19  wx = ex + (arm.a2 + arm.b2) * cos(b3);  wy = ey + (arm.a2 + arm.b2) * sin(b3);
  20  tx = wx + (arm.a3 + arm.b3) * cos(b4);  ty = wy + (arm.a3 + arm.b3) * sin(b4);
  21  n = numel(x);
  22  P = zeros(4, 2, n);
  23  P(:, 1, :) = reshape([sx ex wx tx].', 4, 1, n);
  24  P(:, 2, :) = reshape([sy ey wy ty].', 4, 1, n);
  25  end
```

</details>
<!-- /listing -->
