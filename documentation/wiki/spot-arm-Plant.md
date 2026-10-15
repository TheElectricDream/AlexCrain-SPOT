# spot.arm.Plant

**File:** `+spot/+arm/Plant.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/Plant.m -->

## What it is for

The diagram's *RED + ARM Plant* block: the Chaser with its arm, simulated. Inputs: the thruster force/torque on the base (`F_RED`), the three joint torques the diagram commands (`ARM_Controls`) and, with `FloatInput`, the float state (`Float_State`). Outputs: the six positions and velocities, and the torque each servo would report. Each diagram step (`baseRate`, 50 ms) the plant takes `baseRate/innerStep` internal steps (1 ms) of: the servos in current control (goal current from the torque, held as far as the voltage allows), the coupled base + arm dynamics with the rotor inertias and the table's slope ([[spot.sim.tableSlope|spot-sim-tableSlope]]), and the joint friction with stiction solved implicitly. The maths is on [[The Arm|Math-Arm]].

With `FixedBase` the base doesn't move (the arm on a stand, to compare with calibration data). With `FloatInput` the block has a third input, the float state (1 while the air bearings are on), and the table's pull acts only while the platform floats; without it the pull always acts. The diagram's block has `FloatInput` on, fed by a Data Store Read of `Float_State`.

## How to use it

In the diagram it is a MATLAB System block with `Model = armModel`. Outside Simulink:

```matlab
p = spot.arm.Plant('Model', armModel);
[pos, vel, torque] = p(F_RED, jointTorques);     % one diagram step
% with FloatInput: p(F_RED, jointTorques, floatState)
p.MotorTorque, p.FrictionTorque, p.Current, p.Stuck   % the last inner step
```

## Line by line

**Lines 40–67.** Parameters (`Model`, `FixedBase`, `FloatInput`); the last inner step's motor torques, friction torques, currents and "stuck" flags; private state (positions `q`, velocities `v`, goal currents, the number and size of inner steps, the motor constants $K_t$, $R$, $K_e$, and `floating`, whether the table's pull acts); the current unit.

**Lines 76–84.** `setupImpl`: the number of inner steps and their size from the model's outer and inner step; the motor constants from stall torque, stall current, no-load speed and voltage.

**Lines 86–95.** `resetImpl`: start at the model's initial state `x0` with zero velocities, goals and torques, floating.

**Lines 97–115.** `stepImpl`, once per diagram step: with `FloatInput`, `floating` is 1 if the third input is non-zero, else 0; the joint angles as encoder pulses; the torques with [[withinLimits|spot-arm-withinLimits]] (no push past the position limits); goal current units = torque / $K_t$ / 3.36 mA, rounded and clipped to the Current Limit (that is exactly what [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]] sends); then `nSub` inner steps with the base force; outputs: positions, velocities, and the reported torque (the current quantised to units, times $K_t$).

**Lines 117–125.** The block interface: two inputs (three with `FloatInput`), three outputs (6, 6, 3).

**Lines 134–171.** `innerStep`, the physics of one millisecond:
- 140–144: the current each servo can actually drive, the goal clipped to what the supply voltage leaves after the back-EMF $K_e \omega$; the motor torque $\tau_0 = K_t i$.
- 145–146: the state with the shoulder angle shifted by `q1Offset` (the model's shoulder zero is at the body's +y mount; the diagram's is "straight out from its face").
- 147–149: the mass matrix with the rotor inertias added on the joint diagonal; the Coriolis term $Cv$.
- 150–152: the viscous coefficients (taken implicitly) and the dry-friction bound at the current speed, $\mu = T_c + (T_s - T_c) e^{-(\omega/v_s)^2}$.
- 153–164: the implicit velocity update. With a fixed base only the three joint rows are used (base velocities stay 0); otherwise all six. `A = M + h·diag(b)`, `rhs = M v + h (forces − Cv)`, and `dryFriction` returns the new velocities and the friction impulses `p`. The base's forces include the table's pull `Fs` (lines 160–161): the acceleration from [[spot.sim.tableSlope|spot-sim-tableSlope]] (the slope and the air bearings' drag, `m.tableSlope`, `m.tableDamping`) times `M(1,1)`, the whole platform's mass with the arm's links, and times `floating`.
- 165–170: store velocities, advance positions (semi-implicit Euler), and the diagnostic outputs: the current, the motor torque, the friction torque (impulse / h minus the viscous part, which is what the dry part was), and whether each joint is stuck (zero velocity with an impulse strictly inside its bound).

**Lines 175–196.** `dryFriction`: solve `A v = rhs + Σ e_j p_j` with `|p_j| ≤ bound_j`. Invert `A` once; `vFree` is the velocity without friction; `W` is the 3×3 block of `A⁻¹` at the joints (how an impulse at one joint changes the velocity at another). Projected Gauss-Seidel: for each joint, the velocity it would have with the current impulses, the impulse that would zero it (`p_j − v_j / W_jj`), clipped to the bound; sweep until the impulses stop changing (60 sweeps at most). The new velocities follow.

<!-- xref -->
- **Used by:** `+spot/+arm/Contents.m`, `+spot/+arm/PositionControl.m`, `+spot/+arm/VelocityControl.m`, `+spot/+arm/compareSteps.m`, `+spot/+arm/defaults.m`, `+spot/+arm/parameters.m`, `+spot/+arm/withinLimits.m`, `+spot/+hardware/ArmTorque.m`, `+spot/+params/apply.m`, `+spot/+sim/tableSlope.m`
- **Uses:** [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]], [[spot.arm.coriolisMatrix|spot-arm-coriolisMatrix]], [[spot.arm.inertiaMatrix|spot-arm-inertiaMatrix]], [[spot.arm.parameters|spot-arm-parameters]], [[spot.arm.withinLimits|spot-arm-withinLimits]], [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]], [[spot.sim.tableSlope|spot-sim-tableSlope]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/Plant.m</b> (198 lines; click to show)</summary>

```matlab
   1  classdef Plant < matlab.System
   2  %PLANT The Chaser (RED) with its arm and Dynamixel servos, simulated at the plant's own step.
   3  %   The diagram's "RED + ARM Plant" block. The servos are driven by torque only, as the hardware
   4  %   is (spot.hardware.ArmTorque: each servo in current control mode); position and velocity
   5  %   control are controllers in the diagram (spot.arm.PositionControl, spot.arm.VelocityControl),
   6  %   not modes of the plant. Each time the diagram steps (baseRate), the plant takes
   7  %   baseRate/innerStep internal steps (1 ms by default) of:
   8  %     - the servos in current control (operating mode 0): the joint torques the diagram sends
   9  %       (ARM_Controls, [N m]) become goal currents as the hardware block converts them (torque /
  10  %       torque constant, in 3.36 mA units, within the Current Limit; none that pushes a joint
  11  %       further beyond its Min/Max Position, armModel.servo); each servo holds its current
  12  %       as far as the supply voltage allows (the motor's back-EMF), and the motor's torque goes
  13  %       through the gearbox (stall torque, stall current and no-load speed from the datasheet)
  14  %     - the dynamics of the base (x, y, theta; thruster forces F_RED, and the table's slope and
  15  %       the air bearings' drag, spot.sim.tableSlope, on the whole platform's mass) and the three
  16  %       joints, coupled (spot.arm.inertiaMatrix, spot.arm.coriolisMatrix), with each joint's
  17  %       reflected rotor inertia
  18  %     - joint friction with stiction: each step solves for the friction torques that keep a
  19  %       joint at rest if that takes less than its breakaway torque, and otherwise applies the
  20  %       sliding friction (Coulomb + Stribeck + viscous) against the motion, all at the end of the
  21  %       step (implicit), so the joints stick and slip without chattering
  22  %   Positions are then updated with the new velocities (semi-implicit Euler). The third output is
  23  %   the torque each servo reports (its Present Current, in 3.36 mA units, times the torque
  24  %   constant), as the hardware block's.
  25  %
  26  %   Parameters: Model, the workspace variable armModel (spot.arm.parameters). With FixedBase the
  27  %   base doesn't move (the arm on a fixed stand, e.g. to compare with lab calibration data). With
  28  %   FloatInput the block has a third input, the float state (Float_State: 1 while the air bearings
  29  %   are on), and the table's pull acts only then (a platform resting on its pucks doesn't slide);
  30  %   without it, the table's pull always acts.
  31  %
  32  %   Outside Simulink:
  33  %     p = spot.arm.Plant('Model', armModel);
  34  %     [pos, vel, torque] = p(F_RED, jointTorques);   % one diagram step
  35  %     (with FloatInput: p(F_RED, jointTorques, floatState))
  36  %   p.MotorTorque, p.FrictionTorque, p.Current and p.Stuck describe the last internal step.
  37  
  38      %#codegen
  39  
  40      properties (Nontunable)
  41          Model = struct()          % armModel (spot.arm.parameters)
  42          FixedBase (1,1) logical = false
  43          FloatInput (1,1) logical = false   % a third input: the float state (the diagram's Float_State)
  44      end
  45  
  46      properties (SetAccess = private)
  47          MotorTorque = zeros(3, 1)     % [N m] at each joint
  48          FrictionTorque = zeros(3, 1)  % [N m] at each joint (dry + viscous), signed
  49          Current = zeros(3, 1)         % [A] each motor's current
  50          Stuck = false(3, 1)           % the joint is held by its friction
  51      end
  52  
  53      properties (Access = private)
  54          q = zeros(6, 1)               % [x y theta q1 q2 q3]
  55          v = zeros(6, 1)
  56          iGoal = zeros(3, 1)           % goal current [A]
  57          nSub = 1
  58          h = 1e-3
  59          Kt = zeros(3, 1)              % [N m/A] at the joint
  60          R = zeros(3, 1)               % [ohm]
  61          Ke = zeros(3, 1)              % [V s/rad] at the joint
  62          floating = 1                  % the air bearings are on (the table's pull acts)
  63      end
  64  
  65      properties (Constant, Access = private)
  66          CUR_UNIT = 3.36e-3            % [A] per current unit
  67      end
  68  
  69      methods
  70          function obj = Plant(varargin)
  71              setProperties(obj, nargin, varargin{:});
  72          end
  73      end
  74  
  75      methods (Access = protected)
  76          function setupImpl(obj)
  77              m = obj.Model;
  78              obj.nSub = max(1, round(m.outerStep / m.innerStep));
  79              obj.h = m.outerStep / obj.nSub;
  80              mo = m.motor;
  81              obj.Kt = mo.stallTorque(:) ./ mo.stallCurrent(:);
  82              obj.R = mo.voltage ./ mo.stallCurrent(:);
  83              obj.Ke = mo.voltage ./ mo.noLoadSpeed(:);
  84          end
  85  
  86          function resetImpl(obj)
  87              obj.q = obj.Model.x0(:);
  88              obj.v = zeros(6, 1);
  89              obj.iGoal = zeros(3, 1);
  90              obj.MotorTorque = zeros(3, 1);
  91              obj.FrictionTorque = zeros(3, 1);
  92              obj.Current = zeros(3, 1);
  93              obj.Stuck = false(3, 1);
  94              obj.floating = 1;
  95          end
  96  
  97          function [pos, vel, torque] = stepImpl(obj, F, tau, varargin)
  98              if obj.FloatInput
  99                  obj.floating = double(varargin{1}(1) ~= 0);
 100              end
 101              % The goal currents, as spot.hardware.ArmTorque sends them: no torque pushing a joint
 102              % further beyond its position limits, within the Current Limit
 103              s = obj.Model.servo;
 104              pulses = round(obj.q(4:6) * 4096 / (2*pi)) + 2048;
 105              tau = spot.arm.withinLimits(tau(:), pulses, s.minPosition, s.maxPosition);
 106              lim = s.currentLimit;
 107              units = min(max(round(tau ./ obj.Kt / obj.CUR_UNIT), -lim), lim);
 108              obj.iGoal = units * obj.CUR_UNIT;
 109              for k = 1:obj.nSub
 110                  innerStep(obj, F(:));
 111              end
 112              pos = obj.q;
 113              vel = obj.v;
 114              torque = round(obj.Current / obj.CUR_UNIT) * obj.CUR_UNIT .* obj.Kt;
 115          end
 116  
 117          % ------------------------------------------------------------ Simulink block interface
 118          function n = getNumInputsImpl(obj), n = 2 + double(obj.FloatInput); end
 119          function n = getNumOutputsImpl(~), n = 3; end
 120  
 121  
 122          function [a, b, c] = getOutputSizeImpl(~), a = [6 1]; b = [6 1]; c = [3 1]; end
 123          function [a, b, c] = getOutputDataTypeImpl(~), a = 'double'; b = 'double'; c = 'double'; end
 124          function [a, b, c] = isOutputComplexImpl(~), a = false; b = false; c = false; end
 125          function [a, b, c] = isOutputFixedSizeImpl(~), a = true; b = true; c = true; end
 126  
 127      end
 128  
 129      methods (Static, Access = protected)
 130  
 131      end
 132  
 133      methods (Access = private)
 134          function innerStep(obj, F)
 135              m = obj.Model;
 136              h = obj.h;
 137              fr = m.friction;
 138              q = obj.q;
 139              v = obj.v;
 140              % Each servo holds its goal current, as far as the supply voltage allows
 141              V = m.motor.voltage;
 142              w = v(4:6);
 143              i = min(max(obj.iGoal, (-V - obj.Ke .* w) ./ obj.R), (V - obj.Ke .* w) ./ obj.R);
 144              tau0 = obj.Kt .* i;
 145              x = [q; v];
 146              x(4) = x(4) + m.q1Offset;                       % the shoulder's angle from its face -> the model's
 147              M = spot.arm.inertiaMatrix(m, x);
 148              M(4:6, 4:6) = M(4:6, 4:6) + diag(m.armature(:));
 149              Cv = spot.arm.coriolisMatrix(m, x) * v;
 150              b = fr.viscous(:);                              % velocity-proportional torques, taken implicitly
 151              Ts = fr.static(:); Tc = fr.coulomb(:);
 152              mu = Tc + (Ts - Tc) .* exp(-(w ./ fr.stribeck(:)).^2);   % dry friction bound at this speed
 153              if obj.FixedBase
 154                  A = M(4:6, 4:6) + h * diag(b);
 155                  rhs = M(4:6, 4:6) * w + h * (tau0 - Cv(4:6));
 156                  [vj, p] = obj.dryFriction(A, rhs, [1 2 3], h * mu);
 157                  vNew = [0; 0; 0; vj];
 158              else
 159                  A = M + h * diag([0; 0; 0; b]);
 160                  % the table's pull, on the whole platform (M(1,1): the base and the arm's links)
 161                  Fs = obj.floating * M(1, 1) * spot.sim.tableSlope(q(1:2), v(1:2), m.tableSlope, m.tableDamping);
 162                  rhs = M * v + h * ([F + [Fs; 0]; tau0] - Cv);
 163                  [vNew, p] = obj.dryFriction(A, rhs, [4 5 6], h * mu);
 164              end
 165              obj.v = vNew;
 166              obj.q = q + h * vNew;
 167              obj.Current = i;
 168              obj.MotorTorque = tau0;
 169              obj.FrictionTorque = p / h - fr.viscous(:) .* vNew(4:6);
 170              obj.Stuck = abs(vNew(4:6)) < 1e-9 & abs(p) < h * mu * (1 - 1e-9);
 171          end
 172      end
 173  
 174      methods (Static, Access = private)
 175          function [vNew, p] = dryFriction(A, rhs, J, bound)
 176              % Velocities at the end of the step, A vNew = rhs + (friction impulses p at joints J),
 177              % with each |p(j)| <= bound(j): p stops joint j if it can, else it is at its bound
 178              % against the motion (a box-constrained problem, solved by projected Gauss-Seidel)
 179              Ainv = A \ eye(size(A, 1));
 180              vFree = Ainv * rhs;
 181              W = Ainv(J, J);
 182              p = zeros(3, 1);
 183              for it = 1:60
 184                  change = 0;
 185                  for j = 1:3
 186                      vj = vFree(J(j)) + W(j, :) * p;
 187                      pj = min(max(p(j) - vj / W(j, j), -bound(j)), bound(j));
 188                      change = max(change, abs(pj - p(j)));
 189                      p(j) = pj;
 190                  end
 191                  if change <= 1e-14 * max(1, max(abs(bound)))
 192                      break
 193                  end
 194              end
 195              vNew = vFree + Ainv(:, J) * p;
 196          end
 197      end
 198  end
```

</details>
<!-- /listing -->
