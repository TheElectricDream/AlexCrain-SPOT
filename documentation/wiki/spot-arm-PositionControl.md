# spot.arm.PositionControl

**File:** `+spot/+arm/PositionControl.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/PositionControl.m -->

## What it is for

The diagram's *Default Position Control (ARM)* block: the control law an MX-64(2.0) runs in position mode (operating mode 3), with the same settings in the same units, but running in the diagram every `StepTime` and giving a **torque** (the servos stay in current control). Run at the servo's own 1 ms period with its gains and back-EMF on, it reproduces the servo's own control; the block's defaults are retuned for the diagram's 20 Hz. The equations are on [[The Arm|Math-Arm]]; the units on [[Dynamixel Units|Math-Dynamixel-Units]].

Inputs: `Goal`, the goal joint angles [rad] (`ARM_Path`); `Measured`, `[q1 q2 q3 w1 w2 w3]` (`ARM_Measured_States`). Output: `Torque` [N m] (`ARM_Controls`). The first step (and the first after the block is re-enabled with its states reset) starts from where the arm is, as a servo whose torque is switched on holds its position.

## Block parameters

| | Default (20 Hz) | Servo's own |
|---|---|---|
| `PositionP` / `PositionI` / `PositionD` | [25 56 21] / 10 / [1545 230 236] | 400 / 0 / 200 |
| `Feedforward1`, `Feedforward2` | 0 | 0 |
| `ProfileVelocity` [0.229 rpm], `ProfileAcceleration` | 20, 0 | |
| `MinPosition`, `MaxPosition` [pulses] | 1024, 3072 | |
| `PWMLimit` | 885 | |
| `StepTime`, `ServoPeriod` [s] | 0.05, 0.001 | |
| `BackEMF` | off | on in the servo |
| `Motor` | `armMotor` | |

A gain can be one value for all three joints or `[shoulder elbow wrist]`.

## Line by line

**Lines 41–82.** Parameters as above; the last PWM; private state (started, the goal and the profile's desired position and velocity, the previous error, the integral, the motor constants); the unit constants.

**Lines 91–97.** `setupImpl`: $K_t$, $R$, $K_e$, $V$ from the motor.

**Lines 99–102.** `resetImpl`: not started, zero PWM.

**Lines 104–163.** `stepImpl`:
- 105–107: the step; the measured angles as the pulses the servo would see; the speeds.
- 108–115: first step: goal and desired position = the present position, nothing accumulated.
- 116–121: each goal in radians becomes pulses with the old code's 651.74 factor; only goals within the limits are accepted (the servo ignores others).
- 122–123: profile velocity and acceleration in pulses/s and pulses/s².
- 125–147: per joint, the profile: without a profile velocity the desired position jumps to the goal; otherwise the target speed is $V_p$ (reduced to $\sqrt{2 A_p d}$ near the goal when there is an acceleration), signed towards the goal, approached at most $A_p h$ per step when there is an acceleration; if the next step would overshoot, land on the goal; the desired acceleration is the change of desired velocity.
- 148–151: the error in pulses, its change scaled to per servo period, the new integral scaled by $h/T_p$.
- 152–155: the servo's formula: P/128, I/65536, D/16, FF1/4, FF2/4 (the feedforwards in the servo's velocity/acceleration units).
- 156–159: anti-windup (keep the integral only if not saturated against the error), clip to the PWM limit.
- 161–162: store the PWM and convert to torque.

**Lines 166–173.** Two inputs, one 3×1 double output.

**Lines 178–185.** `pwmTorque` (shared with [[VelocityControl|spot-arm-VelocityControl]]): the PWM's voltage, less the back-EMF if enabled, times $K_t/R$.

**Lines 193–196.** `gain`: the joint's gain from a scalar or a 3-vector.

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/VelocityControl.m`, `+spot/+arm/compareSteps.m`, `+spot/+arm/defaults.m`, `+spot/+arm/servoFromDiagram.m`, `+spot/+console/webapp/static/sim.js`, `+spot/+hardware/ArmTorque.m`, `+spot/+params/defaults.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.defaults|spot-arm-defaults]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/PositionControl.m</b> (196 lines; click to show)</summary>

```matlab
   1  classdef PositionControl < matlab.System
   2  %POSITIONCONTROL The Dynamixel servos' own position control, as a controller giving joint torques.
   3  %   The diagram's "Default Position Control (ARM)" block. It runs the control law the MX-64(2.0)
   4  %   runs in position control mode (operating mode 3), with the same settings in the same units,
   5  %   but in the diagram, every StepTime, and its output is torque: the arm's servos stay in current
   6  %   control (spot.hardware.ArmTorque on the hardware, spot.arm.Plant in simulation).
   7  %     - Goal Position: each goal [rad] becomes pulses as the old hardware code converted it
   8  %       (651.74 pulses/rad + 2048); a goal beyond Min/Max Position is ignored, as the servo
   9  %       ignores it, so the joint keeps its last goal.
  10  %     - Profile: the desired position moves to the goal at Profile Velocity [0.229 rev/min] with
  11  %       Profile Acceleration [214.577 rev/min^2] (0 = none: the goal at once).
  12  %     - PID + feedforward on the error, in the servo's units: PWM = P/128 e + I/65536 sum(e)
  13  %       + D/16 de + FF1/4 vd + FF2/4 ad, e in pulses, per ServoPeriod (the gains are the servo's
  14  %       per-period gains; the controller scales the integral and the derivative to its StepTime),
  15  %       within PWM Limit [885 = 100 %]. Each gain is one value for the three servos, as on the
  16  %       servos, or [shoulder elbow wrist].
  17  %     - PWM to torque: the motor's torque at that PWM, Kt (PWM/885 V - BackEMF Ke w) / R, with Kt,
  18  %       R and Ke from Motor (armModel.motor: the datasheet's stall torque, stall current and no-load speed).
  19  %       The servos' Current Limit then caps it (hardware block and plant alike).
  20  %   Run at the servo's own period (StepTime = ServoPeriod = 1 ms) with BackEMF on, it gives the
  21  %   servo's own position control (spot.arm.Plant's mode 1 before 2026-10-11, to within its
  22  %   rounding). At the diagram's 20 Hz it can't: the servo's gains (400/0/200) and the back-EMF
  23  %   term are far too stiff for a 50 ms step and the arm goes unstable. The defaults here are
  24  %   retuned for 20 Hz, for the three joints together (they are coupled), over the arm's poses,
  25  %   with the base fixed or floating, rotor inertia +-50 % and friction x0.3 to x2: about 1 N m/rad
  26  %   of stiffness, a twentieth of the servo's own, so the I gain does the last few degrees against
  27  %   the joints' stiction (slowly, within ~3 deg). In the diagram: armControl.position (Control pane).
  28  %
  29  %   Inputs: Goal, the goal joint angles [rad] (ARM_Path); Measured, [q1 q2 q3 w1 w2 w3] [rad,
  30  %   rad/s] (ARM_Measured_States). Output: Torque [N m] for each joint (ARM_Controls).
  31  %   The first step (and the first after the controller is switched back on, with the block's
  32  %   Action Port set to reset its states) starts from where the arm is, as a servo whose torque
  33  %   is switched on holds where it is.
  34  %
  35  %   Outside Simulink:
  36  %     c = spot.arm.PositionControl('Motor', armModel.motor, 'StepTime', 0.05);
  37  %     tau = c(goal, [q; w]);
  38  
  39      %#codegen
  40  
  41      properties (Nontunable)
  42          Motor = struct('stallTorque', [6 6 6], 'stallCurrent', [4.1 4.1 4.1], 'noLoadSpeed', [63 63 63] * 2*pi/60, ...
  43              'voltage', 12)                % armModel.motor: the servos' motor (spot.arm.defaults)
  44          StepTime = 0.05                  % [s] how often the controller runs (the diagram's baseRate)
  45          ServoPeriod = 1e-3               % [s] the servo's control period, which its gains are per
  46          PositionP = [25 56 21]           % Position P Gain [shoulder elbow wrist] (SPOT's servos had 400)
  47          PositionI = 10                   % Position I Gain (SPOT's servos had 0)
  48          PositionD = [1545 230 236]       % Position D Gain (SPOT's servos had 200)
  49          Feedforward1 = 0                 % Feedforward 1st Gain
  50          Feedforward2 = 0                 % Feedforward 2nd Gain
  51          ProfileVelocity = 20             % [0.229 rev/min], 0 = no profile
  52          ProfileAcceleration = 0          % [214.577 rev/min^2], 0 = none
  53          MinPosition = 1024               % Min Position Limit [pulses]
  54          MaxPosition = 3072               % Max Position Limit [pulses]
  55          PWMLimit = 885                   % PWM Limit
  56          BackEMF (1,1) logical = false    % include the motor's back-EMF in the PWM's torque
  57      end
  58  
  59      properties (SetAccess = private)
  60          PWM = zeros(3, 1)                % the last step's PWM, as the servo would set it
  61      end
  62  
  63      properties (Access = private)
  64          started = false
  65          goal = zeros(3, 1)               % goal position [pulses, relative to 2048]
  66          pd = zeros(3, 1)                 % profile: desired position [pulses] and velocity [pulses/s]
  67          vd = zeros(3, 1)
  68          ePrev = zeros(3, 1)              % position error last step [pulses]
  69          eInt = zeros(3, 1)               % position error integral [pulses * servo periods]
  70          Kt = zeros(3, 1)
  71          R = zeros(3, 1)
  72          Ke = zeros(3, 1)
  73          V = 12
  74      end
  75  
  76      properties (Constant, Access = private)
  77          PULSES = 4096 / (2*pi)           % encoder pulses per rad
  78          CMD_PULSES = 651.74              % pulses per rad of a goal (the old hardware code's conversion)
  79          VEL_UNIT = 0.229 * 2*pi/60       % [rad/s] per velocity unit
  80          ACC_UNIT = 214.577 * 2*pi/3600   % [rad/s^2] per profile acceleration unit
  81          PWM_FULL = 885
  82      end
  83  
  84      methods
  85          function obj = PositionControl(varargin)
  86              setProperties(obj, nargin, varargin{:});
  87          end
  88      end
  89  
  90      methods (Access = protected)
  91          function setupImpl(obj)
  92              mo = obj.Motor;
  93              obj.Kt = mo.stallTorque(:) ./ mo.stallCurrent(:);
  94              obj.R = mo.voltage ./ mo.stallCurrent(:);
  95              obj.Ke = mo.voltage ./ mo.noLoadSpeed(:);
  96              obj.V = mo.voltage;
  97          end
  98  
  99          function resetImpl(obj)
 100              obj.started = false;
 101              obj.PWM = zeros(3, 1);
 102          end
 103  
 104          function tau = stepImpl(obj, goalRad, meas)
 105              h = obj.StepTime;
 106              pres = round(meas(1:3) * obj.PULSES);          % the servo sees its encoder's pulses
 107              w = meas(4:6);
 108              if ~obj.started                                % torque on: hold where it is
 109                  obj.goal = pres(:);
 110                  obj.pd = pres(:);
 111                  obj.vd = zeros(3, 1);
 112                  obj.ePrev = zeros(3, 1);
 113                  obj.eInt = zeros(3, 1);
 114                  obj.started = true;
 115              end
 116              for j = 1:3                                    % goals beyond a limit are ignored
 117                  g = round(obj.CMD_PULSES * goalRad(j) + 2048);
 118                  if g >= obj.MinPosition && g <= obj.MaxPosition
 119                      obj.goal(j) = g - 2048;
 120                  end
 121              end
 122              Vp = obj.ProfileVelocity * obj.VEL_UNIT * obj.PULSES;      % [pulses/s], 0 = none
 123              Ap = obj.ProfileAcceleration * obj.ACC_UNIT * obj.PULSES;  % [pulses/s^2], 0 = none
 124              u = zeros(3, 1);
 125              for j = 1:3
 126                  vdPrev = obj.vd(j);
 127                  dist = obj.goal(j) - obj.pd(j);
 128                  if Vp <= 0
 129                      obj.pd(j) = obj.goal(j);
 130                      obj.vd(j) = 0;
 131                  else
 132                      vt = Vp;
 133                      if Ap > 0, vt = min(vt, sqrt(2 * Ap * abs(dist))); end
 134                      vt = sign(dist) * vt;
 135                      if Ap > 0
 136                          obj.vd(j) = obj.vd(j) + min(max(vt - obj.vd(j), -Ap * h), Ap * h);
 137                      else
 138                          obj.vd(j) = vt;
 139                      end
 140                      if abs(obj.vd(j) * h) >= abs(dist)
 141                          obj.pd(j) = obj.goal(j);
 142                          obj.vd(j) = 0;
 143                      else
 144                          obj.pd(j) = obj.pd(j) + obj.vd(j) * h;
 145                      end
 146                  end
 147                  ad = (obj.vd(j) - vdPrev) / h;
 148                  e = obj.pd(j) - pres(j);
 149                  de = (e - obj.ePrev(j)) * obj.ServoPeriod / h;
 150                  obj.ePrev(j) = e;
 151                  iNew = obj.eInt(j) + e * h / obj.ServoPeriod;
 152                  uj = gain(obj.PositionP, j) / 128 * e + gain(obj.PositionI, j) / 65536 * iNew ...
 153                      + gain(obj.PositionD, j) / 16 * de ...
 154                      + gain(obj.Feedforward1, j) / 4 * obj.vd(j) / (obj.VEL_UNIT * obj.PULSES) ...
 155                      + gain(obj.Feedforward2, j) / 4 * ad / (obj.ACC_UNIT * obj.PULSES);
 156                  if abs(uj) < obj.PWMLimit || sign(uj) ~= sign(e)   % anti-windup: hold the integral when saturated
 157                      obj.eInt(j) = iNew;
 158                  end
 159                  u(j) = min(max(uj, -obj.PWMLimit), obj.PWMLimit);
 160              end
 161              obj.PWM = u;
 162              tau = spot.arm.PositionControl.pwmTorque(u, w, obj.Kt, obj.R, obj.Ke, obj.V, obj.BackEMF);
 163          end
 164  
 165          % ------------------------------------------------------------ Simulink block interface
 166          function n = getNumInputsImpl(~), n = 2; end
 167          function n = getNumOutputsImpl(~), n = 1; end
 168  
 169  
 170          function a = getOutputSizeImpl(~), a = [3 1]; end
 171          function a = getOutputDataTypeImpl(~), a = 'double'; end
 172          function a = isOutputComplexImpl(~), a = false; end
 173          function a = isOutputFixedSizeImpl(~), a = true; end
 174  
 175      end
 176  
 177      methods (Static)
 178          function tau = pwmTorque(u, w, Kt, R, Ke, V, backEMF)
 179              % The motor's torque at the joint for PWM u [885 = 100 %] at joint speed w [rad/s]
 180              volts = u / 885 * V;
 181              if backEMF
 182                  volts = volts - Ke .* w;
 183              end
 184              tau = Kt ./ R .* volts;
 185          end
 186      end
 187  
 188      methods (Static, Access = protected)
 189  
 190      end
 191  end
 192  
 193  function g = gain(value, j)
 194  % A gain for joint j: one value for all three, or one per joint
 195  g = value(min(j, numel(value)));
 196  end
```

</details>
<!-- /listing -->
