# spot.massprops.centerOfGravity

**File:** `+spot/+massprops/centerOfGravity.m` · **Package:** [[spot.massprops|spot-massprops]]

<!-- source: +spot/+massprops/centerOfGravity.m -->

## What it is for

From the three scale readings: the mass, where the centre of gravity is, how far each thruster's line of action is from it, and where the LEDs are relative to it. The geometry is on [[Mass Properties|Math-Mass-Properties]].

## How to call it

```matlab
r = spot.massprops.centerOfGravity([6.17 3.12 4.46]);   % kg on scales A, B, C
r.mass        % 13.75
r.cg          % [x y] from the body centre [m]
r.thrusters   % 8x1 [mm]
r.leds        % 4x2 [m]
```

## Line by line

**Lines 15–19.** Name the readings; the mass is their sum; a non-positive sum is an error.

**Lines 20–21.** The CG as the reading-weighted average of the scale positions: A at (0, 0.15), B at (−0.15, −0.15), C at (0.15, −0.15).

**Lines 23–32.** The thrusters are 81 mm in from the edge, so their lines of action are at $e = 0.15 - 0.081$ m from the body centre. Each row is the perpendicular distance from the CG to one thruster's line, signed by the direction its force turns the platform, converted to mm: thrusters 1 and 2 (on the −x face, pushing +x... see `ThrusterConfig.png`) use the y distance, 3 and 4 the x distance, and so on round the body.

**Lines 34–38.** The LEDs are 13 mm in from the edges; their positions relative to the CG, in the corner order (+x,+y), (+x,−y), (−x,−y), (−x,+y).

**Line 40.** Pack the four results.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/calibration.py`, `+spot/+control/thrusterDutyCycles.m`, `+spot/+massprops/compute.m`, `+spot/+massprops/trackerPositions.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+massprops/centerOfGravity.m</b> (41 lines; click to show)</summary>

```matlab
   1  function r = centerOfGravity(scales)
   2  %CENTEROFGRAVITY Mass, centre of gravity, thruster offsets and LED positions from scale readings.
   3  %   r = spot.massprops.centerOfGravity([A B C]) with the three scale readings in kg:
   4  %     A  left-middle edge   at body (x, y) = (0, 0.15) m
   5  %     B  right-back corner  at (-0.15, -0.15) m
   6  %     C  right-front corner at ( 0.15, -0.15) m
   7  %   (scale layout of 2025-08-29, relative to the centre of the 0.3 m body; see
   8  %   resources/COMConfig.png). Returns:
   9  %     mass        total mass [kg]
  10  %     cg          [x y] of the centre of gravity from the body centre [m]
  11  %     thrusters   8x1 distance of each thruster's line of action from the CG [mm]
  12  %                 (thruster_dist2CG_* in the diagrams; layout in resources/ThrusterConfig.png)
  13  %     leds        4x2 LED positions from the CG [m], corners in the order
  14  %                 (+x,+y), (+x,-y), (-x,-y), (-x,+y), each 13 mm in from the edges
  15  A = scales(1); B = scales(2); C = scales(3);
  16  M = A + B + C;
  17  if M <= 0
  18      error('spot:massprops:centerOfGravity', 'The scale readings must add up to more than 0 kg.');
  19  end
  20  X = (A*0 + B*(-0.15) + C*0.15) / M;
  21  Y = (A*0.15 + B*(-0.15) + C*(-0.15)) / M;
  22  
  23  % Thrusters sit 81 mm in from the body edge (measured 2025-08-29)
  24  e = 0.15 - 0.081;
  25  thr = [ e - Y
  26         -(e + Y)
  27          e - X
  28         -(e + X)
  29          e + Y
  30         -(e - Y)
  31          e + X
  32         -(e - X)] * 1000;
  33  
  34  edge = 0.15 - 0.013;
  35  leds = [ edge - X,  edge - Y
  36           edge - X, -edge - Y
  37          -edge - X, -edge - Y
  38          -edge - X,  edge - Y];
  39  
  40  r = struct('mass', M, 'cg', [X Y], 'thrusters', thr, 'leds', leds);
  41  end
```

</details>
<!-- /listing -->
