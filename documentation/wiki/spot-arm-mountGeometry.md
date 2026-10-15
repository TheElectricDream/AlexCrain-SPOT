# spot.arm.mountGeometry

**File:** `+spot/+arm/mountGeometry.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/mountGeometry.m -->

## What it is for

The settings' `Phi` and `b0` place the shoulder for the arm on its usual mount: the arm coupler's plate on the Chaser's +y face, left of centre as seen from outside. The Spacecraft Designer can put the arm on any of six places (the +x, +y, −y faces × left/right; not the back, where the control panel is). This moves `Phi` and `b0` to the chosen place and gives `q1Offset`, the angle the whole arm is turned by, so the 3D drawing, the simulation and the figures all agree.

The idea: find the shoulder's position *in the mount's own frame* (the frame on the face at the usual place), then put that same frame at the new place and read the shoulder off it.

## Line by line

**Lines 11–14.** The faces are 0.15 m from the centre; a place is 0.082735 m to the side of its face's centre line (both from the CAD); the three faces' outward angles; a helper turning `'+y'` into the field name `py`.

**Lines 15–24.** The place: the arm's `mount` or the default; the face and side are validated.

**Lines 25–26.** `origin(a, side)`: the place's position for a face at angle `a`, i.e. the face's centre moved sideways by ±lateral; `rot(a)` a rotation matrix.

**Lines 27–29.** The usual mount (+y, left): the shoulder's position from `Phi`, `b0`, expressed in that mount's frame.

**Lines 30–35.** The new face's angle; the shoulder at the new place with the same in-frame offset; back to polar (`Phi`, `b0`); the offset is the difference of face angles; record the mount.

**Lines 37–40.** `lat`: the lateral offset, negative for the left.

<!-- xref -->
- **Used by:** `+spot/+arm/parameters.m`, `+spot/+console/webapp/static/cad.js`, `+spot/+data/toFigure.m`, `+spot/+data/trajectory.m`, `+spot/+params/apply.m`, `+spot/+params/defaults.m`, `+spot/+plotting/armPosition.m`, `+spot/+sim/results.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/mountGeometry.m</b> (41 lines; click to show)</summary>

```matlab
   1  function arm = mountGeometry(arm)
   2  %MOUNTGEOMETRY The arm's geometry for where it is mounted on the Chaser.
   3  %   arm = spot.arm.mountGeometry(arm) takes the settings' arm (s.mass.arm), whose Phi and b0 place
   4  %   the shoulder for the arm on its usual mount (the arm coupler on the +y face, left as seen from
   5  %   outside), and returns it for arm.mount (face '+x', '+y' or '-y'; side 'left' or 'right'; the
   6  %   Spacecraft Designer's places): Phi and b0 for the shoulder there, and q1Offset, the angle the
   7  %   whole arm is turned by (the shoulder's 0 is straight out from its face, wherever it is).
   8  %   The mount moves as the 3D view moves attachments (the place's frame on the face, the shoulder
   9  %   at the same spot in it), so the drawing and the simulation agree.
  10  %   Without arm.mount, or on the usual mount, Phi and b0 are unchanged and q1Offset is 0.
  11  FACE = 0.15;          % [m] the faces' distance from the centre (CAD: info.mount.face)
  12  LATERAL = 0.082735;   % [m] a place's distance from the face's centre line (CAD: info.mount.lateral)
  13  angles = struct('px', 0, 'py', pi/2, 'my', -pi/2);
  14  key = @(f) strrep(strrep(f, '+', 'p'), '-', 'm');
  15  place = struct('face', '+y', 'side', 'left');
  16  if isfield(arm, 'mount') && isstruct(arm.mount)
  17      place = arm.mount;
  18  end
  19  if ~(ischar(place.face) || isstring(place.face)) || ~isfield(angles, key(char(place.face)))
  20      error('spot:arm:mountGeometry', 'The arm''s mount face must be +x, +y or -y (the back, -x, has the control panel).');
  21  end
  22  if ~any(strcmp(place.side, {'left', 'right'}))
  23      error('spot:arm:mountGeometry', 'The arm''s mount side must be left or right.');
  24  end
  25  origin = @(a, side) [FACE * cos(a) - lat(side) * sin(a), FACE * sin(a) + lat(side) * cos(a)];
  26  rot = @(a) [cos(a) -sin(a); sin(a) cos(a)];
  27  a0 = pi/2;                                               % the usual mount: +y, left
  28  s0 = arm.b0 * [cos(arm.Phi), sin(arm.Phi)];              % the shoulder there
  29  inMount = rot(-a0) * (s0 - origin(a0, 'left')).';        % ... in the mount's frame
  30  a = angles.(key(char(place.face)));
  31  s = origin(a, char(place.side)).' + rot(a) * inMount;
  32  arm.Phi = atan2(s(2), s(1));
  33  arm.b0 = hypot(s(1), s(2));
  34  arm.q1Offset = a - a0;
  35  arm.mount = struct('face', char(place.face), 'side', char(place.side));
  36  
  37      function l = lat(side)
  38          l = LATERAL;
  39          if strcmp(side, 'left'), l = -LATERAL; end
  40      end
  41  end
```

</details>
<!-- /listing -->
