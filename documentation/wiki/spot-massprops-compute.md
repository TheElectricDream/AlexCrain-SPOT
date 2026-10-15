# spot.massprops.compute

**File:** `+spot/+massprops/compute.m` · **Package:** [[spot.massprops|spot-massprops]]

<!-- source: +spot/+massprops/compute.m -->

## What it is for

The mass properties of all three platforms from the settings' `mass` section, with the `override` switch applied. [[spot.params.apply|spot-params-apply]] calls it to fill `mRED`, `IRED`, `thruster_dist2CG_RED` and so on.

## How to call it

```matlab
mp = spot.massprops.compute(s.mass);
mp.RED      % mass, inertia (used), measuredMass, measuredInertia, cg, thrusters, leds
```

## Line by line

**Lines 7–8.** An empty result; loop over the three platforms.

**Lines 9–10.** That platform's section, and the centre-of-gravity calculation from its scale readings ([[centerOfGravity|spot-massprops-centerOfGravity]]): mass, cg, thrusters, leds.

**Lines 11–12.** Keep the measured mass under its own name, and the measured inertia from the bifilar test ([[bifilarInertia|spot-massprops-bifilarInertia]]) with the measured mass.

**Lines 13–18.** With `override`, the mass and inertia typed in replace the measured ones; without it, the inertia used is the measured one (the mass already is).

**Line 19.** Store under the platform's name.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+control/thrusterDutyCycles.m`, `+spot/+control/verifyConsoleController.m`, `+spot/+massprops/trackerPositions.m`, `+spot/+massprops/writeTrackerPositions.m`, `+spot/+params/apply.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.massprops.bifilarInertia|spot-massprops-bifilarInertia]], [[spot.massprops.centerOfGravity|spot-massprops-centerOfGravity]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+massprops/compute.m</b> (21 lines; click to show)</summary>

```matlab
   1  function r = compute(mass)
   2  %COMPUTE Mass properties of RED, BLACK and BLUE from the settings' mass section.
   3  %   r = spot.massprops.compute(s.mass) returns r.RED, r.BLACK, r.BLUE, each with
   4  %     mass, inertia     the values used in the simulation (typed in, if s.mass.override)
   5  %     measuredMass, measuredInertia   from the scales and the bifilar test
   6  %     cg, thrusters, leds             see spot.massprops.centerOfGravity
   7  r = struct();
   8  for name = {'RED', 'BLACK', 'BLUE'}
   9      p = mass.(name{1});
  10      c = spot.massprops.centerOfGravity(p.scales);
  11      c.measuredMass = c.mass;
  12      c.measuredInertia = spot.massprops.bifilarInertia(c.mass, p.bifilar.T, p.bifilar.L, p.bifilar.D);
  13      if mass.override
  14          c.mass = p.mass;
  15          c.inertia = p.inertia;
  16      else
  17          c.inertia = c.measuredInertia;
  18      end
  19      r.(name{1}) = c;
  20  end
  21  end
```

</details>
<!-- /listing -->
