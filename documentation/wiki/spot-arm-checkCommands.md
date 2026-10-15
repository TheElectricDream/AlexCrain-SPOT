# spot.arm.checkCommands

**File:** `+spot/+arm/checkCommands.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/checkCommands.m -->

## What it is for

After a simulation, look through the logged arm signals for commands that silently didn't do what the user meant, and say so as warnings (which [[spot.sim.run|spot-sim-run]] appends to the run's messages):

- a goal angle beyond the servo's position limits in position mode, or a goal speed beyond the Velocity Limit in velocity mode: the controller ignores it (as the servo would) and the joint keeps its last goal;
- a torque pushing a joint further past its position limit: none is sent ([[withinLimits|spot-arm-withinLimits]]);
- a torque beyond what the Current Limit allows: the servo gets the limit.

It reads `ARM_Control_Mode` (0 off, 1 position, 2 custom/torque, 3 velocity), `ARM_*_Path`, `ARM_*_Command` and the joint angles.

## Line by line

**Lines 14–19.** Nothing to say if the signals aren't logged.

**Lines 20–31.** The mode, the three commands and the three goals as columns, cut to a common length; the joint angles too when logged.

**Lines 32–35.** The servo limits and the motor's $K_t$; the largest torque the Current Limit allows, per joint.

**Lines 37–42.** A mode outside 0–3 is reported once with its first time and how many steps.

**Lines 43–45.** The position limits in degrees (through the 651.74 goal conversion) and the velocity limit in rad/s, for the messages.

**Lines 46–80.** Per joint: (47–56) goals in position mode whose pulses fall outside the limits; (57–64) goal speeds in velocity mode beyond the limit; (65–72) steps where the joint sat at a limit with a torque pushing further; (73–79) torques beyond the current limit (with a half-unit tolerance for rounding), not counting those already cut by the limits. Each kind is reported once with the first occurrence and the count.

<!-- xref -->
- **Used by:** `+spot/+arm/defaults.m`, `+spot/+arm/servoFromDiagram.m`, `+spot/+sim/run.m`
- **Uses:** [[spot.arm.withinLimits|spot-arm-withinLimits]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/checkCommands.m</b> (81 lines; click to show)</summary>

```matlab
   1  function msgs = checkCommands(dataClass, armModel)
   2  %CHECKCOMMANDS Arm goals the controllers ignore and torques the servos can't give, from a run's log.
   3  %   msgs = spot.arm.checkCommands(dataClass, armModel) looks at the logged ARM_Control_Mode (the
   4  %   arm controller in use: 0 off, 1 position, 2 custom/torque, 3 velocity), the path
   5  %   (ARM_Shoulder/Elbow/Wrist_Path) and the torques sent (ARM_Shoulder/Elbow/Wrist_Command), and
   6  %   returns one message per joint and kind of problem:
   7  %     - a goal angle beyond the servo's position limits (position control), or a goal speed beyond
   8  %       its Velocity Limit (velocity control): the controller ignores it, as the servo would, so
   9  %       the joint keeps its last goal
  10  %     - a torque pushing a joint further beyond its position limits (from the logged angles,
  11  %       ARM_Rz_*): the hardware block and the plant send none (spot.arm.withinLimits)
  12  %     - a torque beyond what the Current Limit allows: the servo gets the limit
  13  %   spot.sim.run adds them to the simulation's warnings.
  14  msgs = {};
  15  need = {'ARM_Control_Mode', 'ARM_Shoulder_Command', 'ARM_Elbow_Command', 'ARM_Wrist_Command', ...
  16          'ARM_Shoulder_Path', 'ARM_Elbow_Path', 'ARM_Wrist_Path'};
  17  if ~all(isfield(dataClass, need))
  18      return
  19  end
  20  t = dataClass.ARM_Control_Mode.Time;
  21  md = round(dataClass.ARM_Control_Mode.Data(:));
  22  cmd = [dataClass.ARM_Shoulder_Command.Data(:), dataClass.ARM_Elbow_Command.Data(:), dataClass.ARM_Wrist_Command.Data(:)];
  23  path = [dataClass.ARM_Shoulder_Path.Data(:), dataClass.ARM_Elbow_Path.Data(:), dataClass.ARM_Wrist_Path.Data(:)];
  24  n = min([numel(md), size(cmd, 1), size(path, 1)]);
  25  q = nan(n, 3);
  26  if all(isfield(dataClass, {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'}))
  27      q = [dataClass.ARM_Rz_Shoulder_rad.Data(:), dataClass.ARM_Rz_Elbow_rad.Data(:), dataClass.ARM_Rz_Wrist_rad.Data(:)];
  28      n = min(n, size(q, 1));
  29      q = q(1:n, :);
  30  end
  31  md = md(1:n); cmd = cmd(1:n, :); path = path(1:n, :); t = t(1:n);
  32  s = armModel.servo;
  33  mo = armModel.motor;
  34  Kt = mo.stallTorque(:).' ./ mo.stallCurrent(:).';
  35  tauMax = s.currentLimit * 3.36e-3 * Kt;
  36  names = {'shoulder', 'elbow', 'wrist'};
  37  bad = ~ismember(md, 0:3);
  38  if any(bad)
  39      k = find(bad, 1);
  40      msgs{end + 1} = sprintf(['ARM_Control_Mode was %g at %.1f s (%d steps): SPOT''s arm controllers write 0 (off), ' ...
  41          '1 (position), 2 (custom, torque) or 3 (velocity).'], md(k), t(k), nnz(bad));
  42  end
  43  lo = (s.minPosition - 2048) / 651.74 * 180/pi;
  44  hi = (s.maxPosition - 2048) / 651.74 * 180/pi;
  45  vmax = s.velocityLimit * 0.229 * 2*pi/60;
  46  for j = 1:3
  47      pulses = round(651.74 * path(:, j) + 2048);
  48      out = md == 1 & (pulses < s.minPosition | pulses > s.maxPosition);
  49      if any(out)
  50          k = find(out, 1);
  51          msgs{end + 1} = sprintf(['The arm''s %s goal of %.1f deg (%d pulses) at %.1f s (%d steps) is beyond its servo''s ' ...
  52              'position limits, %.0f to %.0f deg (Min/Max Position %d-%d on the Default Position Control (ARM) block). ' ...
  53              'The controller ignores it, as the servo would, so the joint holds its last goal. Joint angles are 0 ' ...
  54              'straight out (the shoulder: out from its face) and +-90 deg to either side.'], ...
  55              names{j}, path(k, j) * 180/pi, pulses(k), t(k), nnz(out), lo, hi, s.minPosition, s.maxPosition);
  56      end
  57      units = round(path(:, j) * 9.549296585513702 * 4.366756307);
  58      out = md == 3 & abs(units) > s.velocityLimit;
  59      if any(out)
  60          k = find(out, 1);
  61          msgs{end + 1} = sprintf(['The arm''s %s goal speed of %.3f rad/s at %.1f s (%d steps) is beyond its Velocity ' ...
  62              'Limit (%d x 0.229 rev/min = %.3f rad/s, Default Velocity Control (ARM)): the controller ignores it, so ' ...
  63              'the joint keeps its last goal.'], names{j}, path(k, j), t(k), nnz(out), s.velocityLimit, vmax);
  64      end
  65      pulses = round(q(:, j) * 4096 / (2*pi)) + 2048;
  66      cut = (pulses >= s.maxPosition & cmd(:, j) > 0) | (pulses <= s.minPosition & cmd(:, j) < 0);
  67      if any(cut)
  68          k = find(cut, 1);
  69          msgs{end + 1} = sprintf(['The arm''s %s was at its position limit (%.0f deg) with a torque of %.2f N m pushing it ' ...
  70              'further at %.1f s (%d steps): it got none (Min/Max Position on the Dynamixel Arm (Torque) block).'], ...
  71              names{j}, q(k, j) * 180/pi, cmd(k, j), t(k), nnz(cut));
  72      end
  73      out = md ~= 0 & ~cut & abs(cmd(:, j)) > tauMax(j) * (1 + 1e-9) + 3.36e-3 * Kt(j) / 2;
  74      if any(out)
  75          k = find(out, 1);
  76          msgs{end + 1} = sprintf(['The arm''s %s torque of %.2f N m at %.1f s (%d steps) is beyond what its servo''s ' ...
  77              'Current Limit allows (%d x 3.36 mA = %.2f N m, on the Dynamixel Arm (Torque) block): the servo gets ' ...
  78              'the limit.'], names{j}, cmd(k, j), t(k), nnz(out), s.currentLimit, tauMax(j));
  79      end
  80  end
  81  end
```

</details>
<!-- /listing -->
