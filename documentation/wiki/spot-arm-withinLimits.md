# spot.arm.withinLimits

**File:** `+spot/+arm/withinLimits.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/withinLimits.m -->

## What it is for

In current control the servos have no position limits of their own (position mode had Min/Max Position). So before a torque is sent, the hardware block ([[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]) and the plant ([[Plant|spot-arm-Plant]]) zero any torque that would push a joint further past its limit: at or beyond `maxPosition` and positive, or at or beyond `minPosition` and negative. A torque pulling the joint back is allowed.

## Line by line

**Lines 10–14.** For each of the three joints, the test above on its encoder pulses (2048 = straight out) and its torque.

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/checkCommands.m`, `+spot/+hardware/ArmTorque.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/withinLimits.m</b> (15 lines; click to show)</summary>

```matlab
   1  function tau = withinLimits(tau, pulses, minPosition, maxPosition)
   2  %WITHINLIMITS No joint torque that pushes a joint further beyond its position limits.
   3  %   tau = spot.arm.withinLimits(tau, pulses, minPosition, maxPosition) zeroes each torque [N m]
   4  %   whose joint is at or beyond maxPosition and pushes it up, or at or beyond minPosition and
   5  %   pushes it down (positions in encoder pulses, 2048 straight out). In current control the
   6  %   servos have no position limits of their own (position control had Min/Max Position), so the
   7  %   hardware block (spot.hardware.ArmTorque) applies this before sending the torques, and the
   8  %   plant (spot.arm.Plant) does the same.
   9  %#codegen
  10  for j = 1:3
  11      if (pulses(j) >= maxPosition && tau(j) > 0) || (pulses(j) <= minPosition && tau(j) < 0)
  12          tau(j) = 0;
  13      end
  14  end
  15  end
```

</details>
<!-- /listing -->
