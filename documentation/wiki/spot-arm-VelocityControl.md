# spot.arm.VelocityControl

**File:** `+spot/+arm/VelocityControl.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/VelocityControl.m -->

## What it is for

The diagram's *Default Velocity Control (ARM)* block: the MX-64's velocity-mode law (operating mode 1), in its units, run in the diagram and giving a torque. Goal speeds beyond the Velocity Limit are ignored, as the servo ignores them. Retuned for 20 Hz (P = [160 70 30], I = [220 1500 300]; the servos' own are 100 / 1920); a joint whose stiction is well above the model's default can take a second or more to break away. Inputs: `Goal` [rad/s], `Measured` (`ARM_Measured_States`); output: `Torque` [N m].

## Line by line

**Lines 29–59.** Parameters (motor, step, servo period, P and I gains, profile acceleration, velocity limit, PWM limit, back-EMF), the last PWM, private state (goal, desired velocity, integral, motor constants), unit constants.

**Lines 68–74.** `setupImpl`: the motor constants.

**Lines 76–81.** `resetImpl`: zero everything.

**Lines 83–111.** `stepImpl`:
- 85–90: each goal in rad/s becomes velocity units with the old code's constants (rad/s → rpm × 9.5493, rpm → units × 4.3668); accepted only within the limit.
- 91–93: the measured speeds in units; the profile acceleration in rad/s².
- 95–108: per joint, the desired velocity approaches the goal at most $A_v h$ per step (or jumps); the error in units; the integral scaled by $h/T_p$; $u = P/128\, e + I/65536\, \Sigma e$; anti-windup; clip.
- 109–110: store the PWM; torque through [[PositionControl|spot-arm-PositionControl]]'s `pwmTorque`.

**Lines 114–124.** The block interface and icon; **128–134** the help header; **138–141** `gain`.

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/defaults.m`, `+spot/+arm/servoFromDiagram.m`, `+spot/+hardware/ArmTorque.m`, `+spot/+params/defaults.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.defaults|spot-arm-defaults]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/VelocityControl.m</b> (141 lines; click to show)</summary>

```matlab
   1  classdef VelocityControl < matlab.System
   2  %VELOCITYCONTROL The Dynamixel servos' own velocity control, as a controller giving joint torques.
   3  %   The diagram's "Default Velocity Control (ARM)" block. It runs the control law the MX-64(2.0)
   4  %   runs in velocity control mode (operating mode 1), with the same settings in the same units,
   5  %   but in the diagram, every StepTime, and its output is torque: the arm's servos stay in current
   6  %   control (spot.hardware.ArmTorque on the hardware, spot.arm.Plant in simulation).
   7  %     - Goal Velocity: each goal [rad/s] becomes velocity units [0.229 rev/min] as the old hardware
   8  %       code converted it; a goal beyond Velocity Limit is ignored, as the servo ignores it.
   9  %     - Profile: the desired velocity moves to the goal at Profile Acceleration
  10  %       [214.577 rev/min^2] (0 = none: the goal at once).
  11  %     - PI on the error, in the servo's units: PWM = P/128 e + I/65536 sum(e), e in velocity
  12  %       units, per ServoPeriod (the controller scales the integral to its StepTime), within PWM
  13  %       Limit [885 = 100 %]. Each gain is one value for the three servos or [shoulder elbow wrist].
  14  %     - PWM to torque as spot.arm.PositionControl (Motor, armModel.motor; BackEMF).
  15  %   Run at the servo's own period (StepTime = ServoPeriod = 1 ms) with BackEMF on, it gives the
  16  %   servo's own velocity control (spot.arm.Plant's mode 3 before 2026-10-11). The defaults are
  17  %   retuned for the diagram's 20 Hz (as spot.arm.PositionControl's); a joint whose stiction is
  18  %   well above the default friction can take a second or more to break away.
  19  %
  20  %   Inputs: Goal, the goal joint speeds [rad/s]; Measured, [q1 q2 q3 w1 w2 w3] [rad, rad/s]
  21  %   (ARM_Measured_States). Output: Torque [N m] for each joint (ARM_Controls).
  22  %
  23  %   Outside Simulink:
  24  %     c = spot.arm.VelocityControl('Motor', armModel.motor, 'StepTime', 0.05);
  25  %     tau = c(goalSpeeds, [q; w]);
  26  
  27      %#codegen
  28  
  29      properties (Nontunable)
  30          Motor = struct('stallTorque', [6 6 6], 'stallCurrent', [4.1 4.1 4.1], 'noLoadSpeed', [63 63 63] * 2*pi/60, ...
  31              'voltage', 12)                % armModel.motor: the servos' motor (spot.arm.defaults)
  32          StepTime = 0.05                  % [s] how often the controller runs (the diagram's baseRate)
  33          ServoPeriod = 1e-3               % [s] the servo's control period, which its gains are per
  34          VelocityP = [160 70 30]          % Velocity P Gain [shoulder elbow wrist] (SPOT's servos had 100)
  35          VelocityI = [220 1500 300]       % Velocity I Gain (SPOT's servos had 1920)
  36          ProfileAcceleration = 0          % [214.577 rev/min^2], 0 = none
  37          VelocityLimit = 1023             % Velocity Limit [0.229 rev/min]
  38          PWMLimit = 885                   % PWM Limit
  39          BackEMF (1,1) logical = false    % include the motor's back-EMF in the PWM's torque
  40      end
  41  
  42      properties (SetAccess = private)
  43          PWM = zeros(3, 1)                % the last step's PWM, as the servo would set it
  44      end
  45  
  46      properties (Access = private)
  47          vGoal = zeros(3, 1)              % goal velocity [rad/s]
  48          vDes = zeros(3, 1)               % the profile's desired velocity [rad/s]
  49          vInt = zeros(3, 1)               % velocity error integral [units * servo periods]
  50          Kt = zeros(3, 1)
  51          R = zeros(3, 1)
  52          Ke = zeros(3, 1)
  53          V = 12
  54      end
  55  
  56      properties (Constant, Access = private)
  57          VEL_UNIT = 0.229 * 2*pi/60       % [rad/s] per velocity unit
  58          ACC_UNIT = 214.577 * 2*pi/3600   % [rad/s^2] per profile acceleration unit
  59      end
  60  
  61      methods
  62          function obj = VelocityControl(varargin)
  63              setProperties(obj, nargin, varargin{:});
  64          end
  65      end
  66  
  67      methods (Access = protected)
  68          function setupImpl(obj)
  69              mo = obj.Motor;
  70              obj.Kt = mo.stallTorque(:) ./ mo.stallCurrent(:);
  71              obj.R = mo.voltage ./ mo.stallCurrent(:);
  72              obj.Ke = mo.voltage ./ mo.noLoadSpeed(:);
  73              obj.V = mo.voltage;
  74          end
  75  
  76          function resetImpl(obj)
  77              obj.vGoal = zeros(3, 1);
  78              obj.vDes = zeros(3, 1);
  79              obj.vInt = zeros(3, 1);
  80              obj.PWM = zeros(3, 1);
  81          end
  82  
  83          function tau = stepImpl(obj, goal, meas)
  84              h = obj.StepTime;
  85              for j = 1:3                                    % goals beyond the limit are ignored
  86                  units = round(goal(j) * 9.549296585513702 * 4.366756307);
  87                  if abs(units) <= obj.VelocityLimit
  88                      obj.vGoal(j) = units * obj.VEL_UNIT;
  89                  end
  90              end
  91              w = meas(4:6);
  92              presUnits = round(w(:) / obj.VEL_UNIT);        % the servo sees its own velocity units
  93              Av = obj.ProfileAcceleration * obj.ACC_UNIT;   % [rad/s^2], 0 = none
  94              u = zeros(3, 1);
  95              for j = 1:3
  96                  if Av > 0
  97                      obj.vDes(j) = obj.vDes(j) + min(max(obj.vGoal(j) - obj.vDes(j), -Av * h), Av * h);
  98                  else
  99                      obj.vDes(j) = obj.vGoal(j);
 100                  end
 101                  e = obj.vDes(j) / obj.VEL_UNIT - presUnits(j);
 102                  iNew = obj.vInt(j) + e * h / obj.ServoPeriod;
 103                  uj = gain(obj.VelocityP, j) / 128 * e + gain(obj.VelocityI, j) / 65536 * iNew;
 104                  if abs(uj) < obj.PWMLimit || sign(uj) ~= sign(e)
 105                      obj.vInt(j) = iNew;
 106                  end
 107                  u(j) = min(max(uj, -obj.PWMLimit), obj.PWMLimit);
 108              end
 109              obj.PWM = u;
 110              tau = spot.arm.PositionControl.pwmTorque(u, w(:), obj.Kt, obj.R, obj.Ke, obj.V, obj.BackEMF);
 111          end
 112  
 113          % ------------------------------------------------------------ Simulink block interface
 114          function n = getNumInputsImpl(~), n = 2; end
 115          function n = getNumOutputsImpl(~), n = 1; end
 116          function [a, b] = getInputNamesImpl(~), a = 'Goal'; b = 'Measured'; end
 117          function a = getOutputNamesImpl(~), a = 'Torque'; end
 118          function a = getOutputSizeImpl(~), a = [3 1]; end
 119          function a = getOutputDataTypeImpl(~), a = 'double'; end
 120          function a = isOutputComplexImpl(~), a = false; end
 121          function a = isOutputFixedSizeImpl(~), a = true; end
 122          function icon = getIconImpl(~)
 123              icon = sprintf('Dynamixel velocity\ncontrol -> torque');
 124          end
 125      end
 126  
 127      methods (Static, Access = protected)
 128          function header = getHeaderImpl
 129              header = matlab.system.display.Header('spot.arm.VelocityControl', 'Title', 'Dynamixel Velocity Control', ...
 130                  'Text', ['The MX-64(2.0)''s velocity control law (acceleration profile, PI), in its own units, run in ' ...
 131                  'the diagram: its output is the joint torque [N m] the servo''s PWM would give, for the servos in ' ...
 132                  'current control. Inputs: goal joint speeds [rad/s] and ARM_Measured_States. The gains are retuned ' ...
 133                  'for the diagram''s 20 Hz. See help spot.arm.VelocityControl.']);
 134          end
 135      end
 136  end
 137  
 138  function g = gain(value, j)
 139  % A gain for joint j: one value for all three, or one per joint
 140  g = value(min(j, numel(value)));
 141  end
```

</details>
<!-- /listing -->
