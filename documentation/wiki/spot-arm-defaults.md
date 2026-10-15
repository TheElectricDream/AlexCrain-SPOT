# spot.arm.defaults

**File:** `+spot/+arm/defaults.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/defaults.m -->

## What it is for

The parts of the arm model that are *not* in the settings file: the plant's inner step, the servos' motor constants, the reflected rotor inertia, the joint friction, and the servos' registers. [[parameters|spot-arm-parameters]] adds the settings' geometry and masses and the calibration on top. Joints are [shoulder elbow wrist], Dynamixel IDs 1, 2, 3.

## Line by line

**Lines 24–25.** The inner step, 1 ms: the servos' current loop and the friction are integrated at this rate.

**Lines 27–33.** The MX-64(2.0) at 12 V: stall torque 6.0 N m, stall current 4.1 A, no-load speed 63 rpm (converted to rad/s), supply 12 V.

**Lines 35–37.** The rotor and gear train inertia seen at each joint, 3.5 mN m s² (an estimate for a 200:1 gearbox and a 17 mm coreless motor, until calibrated).

**Lines 39–45.** Friction placeholders until calibrated: breakaway 0.30 N m, Coulomb 0.20 N m, Stribeck velocity 0.05 rad/s, viscous 0.10 N m s/rad.

**Lines 47–62.** The servos' registers in Dynamixel units: the position gains 400/0/200, feedforwards 0, profile velocity 20, acceleration 0, position limits 1024–3072, velocity limit 1023, current limit 850, PWM limit 885, and the servo's assumed control period 1 ms. `currentLimit`, `minPosition`, `maxPosition` and `velocityLimit` are overwritten from the diagram's blocks by [[servoFromDiagram|spot-arm-servoFromDiagram]] before a simulation; the position-mode settings are used by [[compareSteps|spot-arm-compareSteps]] to replay the calibration's steps.

<!-- xref -->
- **Used by:** `+spot/+arm/PositionControl.m`, `+spot/+arm/VelocityControl.m`, `+spot/+arm/inertiaMatrix.m`, `+spot/+arm/loadCalibration.m`, `+spot/+arm/parameters.m`, `SPOT_User_Parameters.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]], [[spot.arm.checkCommands|spot-arm-checkCommands]], [[spot.arm.compareSteps|spot-arm-compareSteps]], [[spot.arm.parameters|spot-arm-parameters]], [[spot.arm.servoFromDiagram|spot-arm-servoFromDiagram]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/defaults.m</b> (63 lines; click to show)</summary>

```matlab
   1  function a = defaults()
   2  %DEFAULTS The simulated arm's servos, their friction and the plant's internal step.
   3  %   a = spot.arm.defaults() returns the parts of the arm model that are not in the settings: the
   4  %   Dynamixel servos (motor, limits), the joint friction, the reflected rotor inertia, and the
   5  %   internal step at which spot.arm.Plant integrates (the diagram runs at baseRate; the plant
   6  %   takes baseRate/innerStep steps each time). The servos run in current control; the position
   7  %   and velocity control laws are the diagram's controllers (spot.arm.PositionControl,
   8  %   spot.arm.VelocityControl, with their own gains). spot.arm.parameters adds the
   9  %   masses, the link geometry and the initial state, and a calibration file if there is one.
  10  %
  11  %   Joints are [shoulder elbow wrist] (Dynamixel IDs 1, 2, 3).
  12  %
  13  %   Friction (per joint, at the joint, in N m): a joint at rest stays at rest until the torque
  14  %   on it exceeds the breakaway torque "static"; moving, the friction is
  15  %       coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous |v|
  16  %   against the motion. The values here are placeholders until the arm is calibrated
  17  %   (resources/arm_calibration/calibrate_arm.py writes resources/arm_calibration.json).
  18  %
  19  %   Servo units (Dynamixel MX series, protocol 2.0): position 4096 pulses/rev, velocity 0.229
  20  %   rev/min, current 3.36 mA, profile acceleration 214.577 rev/min^2, PWM 885 = 100 %.
  21  
  22  a = struct();
  23  
  24  % The plant's internal step [s]: the servos' current control and the friction run at this rate
  25  a.innerStep = 1e-3;
  26  
  27  % Motor and gearbox of each joint, from the MX-64(2.0) datasheet at 12 V: stall torque [N m],
  28  % stall current [A], no-load speed [rad/s] (63 rev/min), supply voltage [V]
  29  a.motor = struct( ...
  30      'stallTorque', [6.0 6.0 6.0], ...
  31      'stallCurrent', [4.1 4.1 4.1], ...
  32      'noLoadSpeed', [63 63 63] * 2*pi/60, ...
  33      'voltage', 12);
  34  
  35  % Rotor and gear train inertia seen at each joint (rotor inertia x gear ratio^2) [kg m^2]: an
  36  % estimate for a 200:1 MX-64 with a coreless 17 mm motor (about 0.9 g cm^2), until calibrated
  37  a.armature = [3.5e-3 3.5e-3 3.5e-3];
  38  
  39  % Joint friction (placeholders until calibrated): breakaway [N m], Coulomb [N m], Stribeck
  40  % velocity [rad/s], viscous [N m s/rad]
  41  a.friction = struct( ...
  42      'static', [0.30 0.30 0.30], ...
  43      'coulomb', [0.20 0.20 0.20], ...
  44      'stribeck', [0.05 0.05 0.05], ...
  45      'viscous', [0.10 0.10 0.10]);
  46  
  47  % The servos, in Dynamixel units. The arm runs in current control (spot.hardware.ArmTorque,
  48  % spot.arm.Plant): currentLimit is the Current Limit the plant holds the goal currents within,
  49  % minPosition/maxPosition and velocityLimit are what spot.arm.checkCommands checks the controllers'
  50  % goals against; spot.sim.run copies all three from the diagram's blocks (spot.arm.servoFromDiagram).
  51  % The rest are the servos' own position-mode settings, as the calibration script runs them for its
  52  % position steps: spot.arm.compareSteps replays those through spot.arm.PositionControl at the
  53  % servo's period, with back-EMF, which is the servo's own control law.
  54  a.servo = struct( ...
  55      'positionP', 400, 'positionI', 0, 'positionD', 200, ...   % Position P/I/D Gain
  56      'feedforward1', 0, 'feedforward2', 0, ...                  % Feedforward 1st/2nd Gain
  57      'profileVelocity', 20, 'profileAcceleration', 0, ...       % position mode profile (0 = none)
  58      'minPosition', 1024, 'maxPosition', 3072, ...              % Min/Max Position Limit [pulses]
  59      'velocityLimit', 1023, ...                                 % Velocity Limit [0.229 rev/min]
  60      'currentLimit', 850, ...                                   % Current Limit [3.36 mA]
  61      'pwmLimit', 885, ...                                       % PWM Limit
  62      'period', 1e-3);                                           % the servo's control period [s] (assumed)
  63  end
```

</details>
<!-- /listing -->
