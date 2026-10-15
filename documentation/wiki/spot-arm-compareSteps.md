# spot.arm.compareSteps

**File:** `+spot/+arm/compareSteps.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/compareSteps.m -->

## What it is for

Check the simulated servos against the real ones. The calibration script records position-mode goal steps for each joint on the real arm (`steps_j<N>.csv`, base fixed, the servos' own gains). This replays the same goals through [[PositionControl|spot-arm-PositionControl]] *at the servo's period with its gains and back-EMF* (which is the servo's own law) driving [[Plant|spot-arm-Plant]] with a fixed base, and compares the simulated joint motion with the recorded one. A match validates the plant (motor, friction, inertia) and the controller replica together.

## How to call it

```matlab
r = spot.arm.compareSteps('resources/arm_calibration/run_2026-10-20', armModel, true);   % plots too
r(1).rms, r(1).maxError   % [rad]
```

## Line by line

**Lines 13–19.** Plot if asked (or if nothing is returned); the step files.

**Line 20.** The goals changed every 10 ms in the script; the logs are faster and uneven, so everything is put on a 10 ms grid.

**Lines 22–29.** For each file: the table, the joint it stepped, the joint angle columns, time from zero, the grid, and the goal held between changes (`'previous'` interpolation).

**Lines 30–40.** A copy of the model with the servo's period as the outer step, the recorded first pose as the initial state, and the current limit raised to the maximum (position mode isn't held to the Current Limit, only the PWM limit); a fixed-base plant; a position controller at the servo's period with the servo's own settings and back-EMF on.

**Lines 41–54.** The script sends true pulses while the controller converts goals with the old 651.74 factor, so a scale compensates; the other joints are held at their first angle; per grid point, the stepped joint's goal is set and the plant/controller pair is run `per` inner periods; the simulated angle is recorded.

**Lines 55–59.** The measured angle on the grid, the error, the rms and maximum, printed in degrees.

**Lines 61–71.** Optionally a figure: goal (dotted), measured (blue), simulated (red dashed) per joint.

<!-- xref -->
- **Used by:** `+spot/+arm/defaults.m`, `+spot/+console/webapp/static/calibration.js`, `+spot/+console/webapp/static/calviz.js`, `resources/arm_calibration/calibrate_arm.py`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.loadCalibration|spot-arm-loadCalibration]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/compareSteps.m</b> (72 lines; click to show)</summary>

```matlab
   1  function r = compareSteps(folder, armModel, show)
   2  %COMPARESTEPS The simulated servos against the real ones, on the calibration's position steps.
   3  %   r = spot.arm.compareSteps(folder, armModel) replays each steps_j<N>.csv in folder (a run of
   4  %   resources/arm_calibration/calibrate_arm.py: position-mode goal steps with SPOT's servo
   5  %   settings, base fixed) with the same goals through the servo's own position control law
   6  %   (spot.arm.PositionControl at the servo's period, armModel.servo's gains, back-EMF) driving
   7  %   spot.arm.Plant, and compares the joint's motion. A match checks both the plant (motor,
   8  %   friction, inertia) and the controller replica that the diagram's Default Position Control
   9  %   (ARM) runs, retuned, at 20 Hz. armModel is the model to try (e.g. evalin('base', 'armModel') after a simulation, or
  10  %   spot.arm.loadCalibration(fullfile(folder, 'arm_calibration.json'), armModel)).
  11  %   r(k) has joint, t, goal, measured, simulated [rad] and rms, maxError [rad].
  12  %   r = spot.arm.compareSteps(folder, armModel, true) also plots them.
  13  if nargin < 3
  14      show = nargout == 0;
  15  end
  16  files = dir(fullfile(folder, 'steps_j*.csv'));
  17  if isempty(files)
  18      error('spot:arm:compareSteps', 'No steps_j*.csv in %s.', folder);
  19  end
  20  dt = 0.01;   % the goals change every 10 ms (the logs are faster and uneven)
  21  r = struct('joint', {}, 't', {}, 'goal', {}, 'measured', {}, 'simulated', {}, 'rms', {}, 'maxError', {});
  22  for f = files.'
  23      T = readtable(fullfile(f.folder, f.name));
  24      j = T.joint(1);
  25      qCols = startsWith(T.Properties.VariableNames, 'q');
  26      Q = T{:, qCols};
  27      t = T.t - T.t(1);
  28      tt = (0:dt:t(end)).';
  29      goal = interp1(t, T.goal, tt, 'previous', 'extrap');
  30      a = armModel;
  31      sv = a.servo;
  32      a.outerStep = sv.period;
  33      a.x0 = [0; 0; 0; Q(1, :).'];
  34      a.servo.currentLimit = 1941;        % position mode isn't held to the Current Limit (only to the PWM Limit)
  35      p = spot.arm.Plant('Model', a, 'FixedBase', true);
  36      c = spot.arm.PositionControl('Motor', a.motor, 'StepTime', sv.period, 'ServoPeriod', sv.period, ...
  37          'PositionP', sv.positionP, 'PositionI', sv.positionI, 'PositionD', sv.positionD, ...
  38          'Feedforward1', sv.feedforward1, 'Feedforward2', sv.feedforward2, 'ProfileVelocity', sv.profileVelocity, ...
  39          'ProfileAcceleration', sv.profileAcceleration, 'MinPosition', sv.minPosition, 'MaxPosition', sv.maxPosition, ...
  40          'PWMLimit', sv.pwmLimit, 'BackEMF', true);
  41      scale = (4096 / (2*pi)) / 651.74;   % the script sends true pulses; the controller converts as SPOT's code did
  42      held = Q(1, :).' * scale;
  43      sim = zeros(numel(tt), 1);
  44      per = round(dt / sv.period);
  45      meas = [Q(1, :).'; 0; 0; 0];
  46      for k = 1:numel(tt)
  47          cmd = held;
  48          cmd(j) = goal(k) * scale;
  49          for i = 1:per
  50              [pos, vel] = p([0; 0; 0], c(cmd, meas));
  51              meas = [pos(4:6); vel(4:6)];
  52          end
  53          sim(k) = pos(3 + j);
  54      end
  55      meas = interp1(t, Q(:, j), tt);
  56      err = sim - meas;
  57      r(end + 1) = struct('joint', j, 't', tt, 'goal', goal, 'measured', meas, 'simulated', sim, ...
  58                          'rms', sqrt(mean(err.^2, 'omitnan')), 'maxError', max(abs(err)));
  59      fprintf('joint %d: simulated vs measured %.2f deg rms, %.2f deg at most\n', j, r(end).rms * 180/pi, r(end).maxError * 180/pi);
  60  end
  61  if show
  62      figure('Name', 'Arm servos: simulated vs measured');
  63      for k = 1:numel(r)
  64          subplot(numel(r), 1, k);
  65          plot(r(k).t, r(k).goal * 180/pi, 'k:', r(k).t, r(k).measured * 180/pi, 'b', r(k).t, r(k).simulated * 180/pi, 'r--');
  66          ylabel(sprintf('Joint %d [deg]', r(k).joint));
  67          grid on
  68          if k == 1, legend('Goal', 'Measured', 'Simulated', 'Location', 'best'); end
  69      end
  70      xlabel('Time [s]');
  71  end
  72  end
```

</details>
<!-- /listing -->
