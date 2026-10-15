# spot.plotting.drawSpacecraft

**File:** `+spot/+plotting/drawSpacecraft.m` · **Package:** [[spot.plotting|spot-plotting]]

<!-- source: +spot/+plotting/drawSpacecraft.m -->

## What it is for

A platform's outline on the table for publication figures: the 0.30 m square body with, depending on the shape id, a docking grapple, a docking cone (with its cylinder), solar panels or the arm's joint. A line from the centre to the +x face marks the body's x axis. From the lab's GetStarted `DrawSpacecraft.m`.

| id | Shape |
|---|---|
| 1 | chaser with docking grapple on +y |
| 2 | target with docking cone on +y |
| 3 | plain platform |
| 4 | target with cone and solar panels |
| 5 | chaser with the arm's shoulder joint |
| 6 | chaser with grapple on +x |
| 7 | plain platform with solar panels |

## How to call it

```matlab
pts = spot.plotting.drawSpacecraft(x, y, theta, 4);   % N x 2
patch(pts(:, 1), pts(:, 2), 'w');
```

## Line by line

**Lines 11–13.** The shape around the origin, rotated by θ and moved to (x, y).

**Lines 16–51.** `shape`: the dimensions (half side 0.15 m; the cone's cylinder 8.07 cm and cone 5.5 cm at 30°; the grapple 5.5 cm at 20°; panels 0.45 × 0.02 m; the bracket's position `g` from the body's edge allowing for the excess, shrink and bracket widths), then the point list for each id, always starting at (0, 0) → (r, 0) (the axis mark) and going round the body counter-clockwise with the attachment's points inserted on its face.

<!-- xref -->
- **Used by:** `+spot/+data/toFigure.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+plotting/drawSpacecraft.m</b> (51 lines; click to show)</summary>

```matlab
   1  function pts = drawSpacecraft(x, y, theta, id)
   2  %DRAWSPACECRAFT Outline of a SPOT platform on the table, for publication figures.
   3  %   pts = spot.plotting.drawSpacecraft(x, y, theta, id) returns the N x 2 outline of a platform
   4  %   at position (x, y) [m] and attitude theta [rad], for patch(pts(:,1), pts(:,2), ...). The line
   5  %   from the centre to the +x face marks the body +x axis. id picks the shape:
   6  %     1  chaser (docking grapple, +y)          5  chaser (arm joint only)
   7  %     2  target (docking cone, +y)             6  chaser (docking grapple, +x)
   8  %     3  generic platform                      7  generic platform with solar panels
   9  %     4  target (docking cone and solar panels)
  10  %   From the lab's GetStarted/PlottingFunctions/DrawSpacecraft.m (C. Savytska, 2024).
  11  sc = shape(id);
  12  R = [cos(theta), -sin(theta); sin(theta), cos(theta)];
  13  pts = (R * sc.').' + [x, y];
  14  end
  15  
  16  function sc = shape(id)
  17  % The outline around (0, 0), before rotation
  18  cm = 1 / 100;
  19  r = 0.15;                       % half side [m]
  20  excess = 2.477 * cm; shrink = 1 * cm; bracket = 6.5 * cm;
  21  cyl = 8.07 * cm; cone = 5.5 * cm; coneAngle = 30 * pi / 180;
  22  grapple = 5.5 * cm; grappleAngle = 20 * pi / 180;
  23  panelL = 0.45; panelW = 0.02;
  24  g = r - (excess + shrink + bracket);
  25  switch id
  26      case 1
  27          sc = [0 0; r 0; r r; g r; g + grapple * sin(grappleAngle), r + grapple; ...
  28                g + bracket - grapple * sin(grappleAngle), r + grapple; g + bracket, r; -r r; -r -r; r -r; r 0];
  29      case 2
  30          sc = [0 0; r 0; r r; g r; g, r + cyl; g - cone * sin(coneAngle), r + cyl + cone; ...
  31                g + bracket + cone * sin(coneAngle), r + cyl + cone; g + bracket, r + cyl; g + bracket, r; ...
  32                -r r; -r -r; r -r; r 0];
  33      case 4
  34          sc = [0 0; r 0; r r; g r; g, r + cyl; g - cone * sin(coneAngle), r + cyl + cone; ...
  35                g + bracket + cone * sin(coneAngle), r + cyl + cone; g + bracket, r + cyl; g + bracket, r; ...
  36                -r r; -r, -panelW / 2; -r - panelL, -panelW / 2; -r - panelL, panelW / 2; -r, panelW / 2; ...
  37                -r -r; r -r; r, panelW / 2; r + panelL, panelW / 2; r + panelL, -panelW / 2; r, -panelW / 2; r 0];
  38      case 5
  39          sc = [0 0; r 0; r r; r - 0.08, r; r - 0.08, r + 0.08; r - 0.02, r + 0.08; r - 0.02, r; ...
  40                -r r; -r -r; r -r; r 0];
  41      case 6
  42          sc = [0 0; r 0; r r; -r r; -r -r; r -r; r -g; r + grapple, -g - grapple * sin(grappleAngle); ...
  43                r + grapple, -g - bracket + grapple * sin(grappleAngle); r, -g - bracket; r 0];
  44      case 7
  45          sc = [0 0; r 0; r r; -r r; -r, -panelW / 2; -r - panelL, -panelW / 2; -r - panelL, panelW / 2; ...
  46                -r, panelW / 2; -r -r; r -r; r, panelW / 2; r + panelL, panelW / 2; r + panelL, -panelW / 2; ...
  47                r, -panelW / 2; r 0];
  48      otherwise   % 3: generic platform
  49          sc = [0 0; r 0; r r; -r r; -r -r; r -r; r 0];
  50  end
  51  end
```

</details>
<!-- /listing -->
