# spot.control.nominalForces

**File:** `+spot/+control/nominalForces.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/nominalForces.m -->

## What it is for

The thrust of each of the eight thrusters as the 8×1 vector `F_thrusters_<PLATFORM>`. The settings give one number per platform; this lays it out per thruster, with the x thrusters (1, 2, 5, 6) and y thrusters (3, 4, 7, 8) kept separate so that a platform with different x and y thrusters could be described by changing two lines. [[spot.params.apply|spot-params-apply]] uses it when the project has no thruster calibration for the platform, or `calibration.thrusters` is off; otherwise `F_thrusters_<PLATFORM>` is the calibration's eight measured thrusts.

## How to call it

```matlab
F = spot.control.nominalForces(0.2825);    % [0.2825; 0.2825; ...] (8x1)
```

## Line by line

**Line 7.** The same value for x and y.

**Line 8.** In thruster order: x, x, y, y, x, x, y, y.

<!-- xref -->
- **Used by:** `+spot/+control/thrusterDutyCycles.m`, `+spot/+params/apply.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/nominalForces.m</b> (9 lines; click to show)</summary>

```matlab
   1  function F = nominalForces(force)
   2  %NOMINALFORCES Thrust of each of the 8 thrusters of a platform.
   3  %   F = spot.control.nominalForces(force) returns the 8x1 vector F_thrusters_<platform> for a
   4  %   platform whose thrusters all give FORCE newtons. Thrusters 1, 2, 5, 6 act along x and 3, 4,
   5  %   7, 8 along y (resources/ThrusterConfig.png); they are listed separately so that a platform
   6  %   with unequal x and y thrusters can be described.
   7  Fx = force; Fy = force;
   8  F = [Fx; Fx; Fy; Fy; Fx; Fx; Fy; Fy];
   9  end
```

</details>
<!-- /listing -->
