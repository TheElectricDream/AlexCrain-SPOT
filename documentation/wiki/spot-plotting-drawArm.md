# spot.plotting.drawArm

**File:** `+spot/+plotting/drawArm.m` · **Package:** [[spot.plotting|spot-plotting]]

<!-- source: +spot/+plotting/drawArm.m -->

## What it is for

Outlines of the three links, as rectangles 3 cm wide between consecutive joint points (the last one with a gripper shape), for `patch` in a figure. From the lab's GetStarted `DrawARM.m`.

## How to call it

```matlab
links = spot.plotting.drawArm(spot.plotting.armPosition(pose, q, arm));   % 1 x 3 cell of N x 2 outlines
for k = 1:3, patch(links{k}(:, 1), links{k}(:, 2), 'w'); end
```

## Line by line

**Line 7.** Half width 15 mm.

**Lines 9–12.** For each link: its start, its vector to the next joint, its length.

**Lines 13–17.** A rectangle centred on the link's middle for the first two; the end effector gets a notch and two jaws (the points at ±0.045).

**Lines 18–20.** Rotate the shape by the link's direction and move it to the link's middle.

<!-- xref -->
- **Used by:** `+spot/+data/toFigure.m`
- **Uses:** [[spot.plotting.armPosition|spot-plotting-armPosition]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+plotting/drawArm.m</b> (22 lines; click to show)</summary>

```matlab
   1  function links = drawArm(joints)
   2  %DRAWARM Outlines of the arm's three links, for publication figures.
   3  %   links = spot.plotting.drawArm(P) takes the joint positions from spot.plotting.armPosition
   4  %   (4 x 2: shoulder, elbow, wrist, end effector) and returns a 1 x 3 cell of outlines (proximal,
   5  %   distal and end-effector links) for patch.
   6  %   From the lab's GetStarted/PlottingFunctions/DrawARM.m (C. Savytska, 2024).
   7  w = 0.015;   % half width of a link [m]
   8  links = cell(1, 3);
   9  for k = 1:3
  10      a = joints(k, :);
  11      d = joints(k + 1, :) - a;
  12      L = norm(d);
  13      if k < 3
  14          shape = [L/2 -w; L/2 w; -L/2 w; -L/2 -w; L/2 -w];
  15      else   % the end effector, with its gripper
  16          shape = [L/2 -w; L/2 w; -0.01 0.045; -0.01 w; -L/2 w; -L/2 -w; -0.01 -w; -0.01 -0.045; L/2 -w];
  17      end
  18      th = atan2(d(2), d(1));
  19      R = [cos(th), -sin(th); sin(th), cos(th)];
  20      links{k} = (R * shape.').' + (a + d / 2);
  21  end
  22  end
```

</details>
<!-- /listing -->
