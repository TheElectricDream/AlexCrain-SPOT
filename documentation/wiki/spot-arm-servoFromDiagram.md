# spot.arm.servoFromDiagram

**File:** `+spot/+arm/servoFromDiagram.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/servoFromDiagram.m -->

## What it is for

The simulated servos must have the limits the diagram's blocks set, so that a simulation warns about the same things an experiment would hit. Before each run, [[spot.sim.run|spot-sim-run]] calls this to copy into `armModel.servo`: the position controller's Min/Max Position, the velocity controller's Velocity Limit, and the hardware block's Current Limit and Min/Max Position (the last ones win, since they are what the servos actually get). Blocks that aren't in the diagram, or values that can't be evaluated, leave the model's own.

## Line by line

**Lines 10–12.** The three System object classes and, for each, the block parameter → servo field pairs.

**Lines 13–18.** Find the MATLAB System blocks of each class anywhere in the model (under masks, through links, in all variants); skip missing ones.

**Lines 19–29.** For each parameter, resolve its expression in the block's context (`slResolve`: it may be a workspace variable) and take it if it is a finite scalar.

<!-- xref -->
- **Used by:** `+spot/+arm/defaults.m`, `+spot/+sim/run.m`
- **Uses:** [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]], [[spot.arm.checkCommands|spot-arm-checkCommands]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/servoFromDiagram.m</b> (30 lines; click to show)</summary>

```matlab
   1  function servo = servoFromDiagram(model, servo)
   2  %SERVOFROMDIAGRAM The simulated servos' limits, from the diagram's arm blocks.
   3  %   servo = spot.arm.servoFromDiagram(model, servo) copies into servo (armModel.servo) the limits
   4  %   the diagram's blocks set: the Velocity Limit of the velocity controller
   5  %   (spot.arm.VelocityControl) and the Min/Max Position of the position controller
   6  %   (spot.arm.PositionControl), which spot.arm.checkCommands warns about, then the Current Limit
   7  %   and Min/Max Position of "Dynamixel Arm (Torque)" (spot.hardware.ArmTorque: what the real
   8  %   servos get, and what the plant holds the torques within). A block that isn't there, or a
   9  %   value that can't be evaluated, leaves servo's own.
  10  maps = {'spot.arm.PositionControl', {'MinPosition', 'minPosition'; 'MaxPosition', 'maxPosition'};
  11          'spot.arm.VelocityControl', {'VelocityLimit', 'velocityLimit'};
  12          'spot.hardware.ArmTorque', {'CurrentLimit', 'currentLimit'; 'MinPosition', 'minPosition'; 'MaxPosition', 'maxPosition'}};
  13  for b = 1:size(maps, 1)
  14      blocks = find_system(model, 'LookUnderMasks', 'all', 'FollowLinks', 'on', 'MatchFilter', @Simulink.match.allVariants, ...
  15          'BlockType', 'MATLABSystem', 'System', maps{b, 1});
  16      if isempty(blocks)
  17          continue
  18      end
  19      map = maps{b, 2};
  20      for k = 1:size(map, 1)
  21          try
  22              value = slResolve(get_param(blocks{1}, map{k, 1}), blocks{1});
  23              if isnumeric(value) && isscalar(value) && isfinite(value)
  24                  servo.(map{k, 2}) = double(value);
  25              end
  26          catch
  27          end
  28      end
  29  end
  30  end
```

</details>
<!-- /listing -->
