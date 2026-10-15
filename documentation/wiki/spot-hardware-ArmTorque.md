# spot.hardware.ArmTorque

**File:** `+spot/+hardware/ArmTorque.m` · **Package:** [[spot.hardware|spot-hardware]] · **C++:** [[arm_torque.cpp|Hardware-arm-torque]]

<!-- source: +spot/+hardware/ArmTorque.m -->

## What it is for

The diagram's *Dynamixel Arm (Torque)* block: the arm's three MX-64 servos driven in **current (torque) control** for the whole run. Position and velocity control are no longer done inside the servos; they are controllers in the diagram ([[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]]) whose output is a torque, exactly as in the simulation ([[spot.arm.Plant|spot-arm-Plant]]), so the same controller code runs in both.

Each step:
- **Outputs**, where the block sits (before the controllers): one sync read of each servo's present position, velocity and current → `Position` [rad] (0 = straight out, wrapped to [−π, π)), `Velocity` [rad/s], `Torque` [N m] (current × torque constant), and `Status`.
- **Update**, at the end of the step (after the controllers): the `Torque` input becomes each servo's goal current in one sync write, within `CurrentLimit`; a joint at or beyond `MinPosition`/`MaxPosition` gets no torque pushing it further ([[spot.arm.withinLimits|spot-arm-withinLimits]]), because servos in current control have no position limits of their own.

Because the input is only used in Update, the block can sit *before* the controllers that compute it: it is not direct feed-through. The diagram feeds it `ARM_Controls` from the previous step's *Arm Torques (After Control)*.

`Status` = [servos that answered (0–3), the read's time in ms, the USB adapter's latency timer in ms (−1 unknown; the block asks for 1 ms), reads missed since the start, 1 if a servo reports a hardware error (overload: the servo then switches its own torque off)].

The first step opens the port and sets the servos up (torque off, current control, current limit, optionally Return Delay Time 0, goal 0, torque on); at the end of the run, goal 0 and torque off. The old blocks (position/velocity control in the servos, read at ~2 Hz) are in `archive/old_arm_blocks`.

## Block parameters

| Parameter | Default | |
|---|---|---|
| `Port` | `/dev/ttyUSB0` | the U2D2 adapter |
| `CurrentLimit` | 850 (×3.36 mA ≈ 2.9 A) | written to the servos; goals are clipped to it |
| `TorqueConstant` | 6.0/4.1 N m/A | stall torque / stall current of the MX-64(2.0), at the joint |
| `ZeroReturnDelay` | off | write Return Delay Time 0 (an EEPROM change) for faster replies |
| `MinPosition`, `MaxPosition` | 1024, 3072 pulses (±90°) | the software position limits |

## Line by line

**Lines 1–29.** The class derives from `matlab.System` and `coder.ExternalDependency`; the help explains the above.

**Lines 33–51.** The parameters; private state (`opened`, the last positions read); the unit constants: 4096 pulses per turn, the velocity unit 0.229 rpm, the current unit 3.36 mA.

**Lines 60–62.** Setup: not opened yet.

**Lines 64–81.** `outputImpl`: defaults (2048 pulses = straight out, zero speed and current, zero status); in generated code only, open the port on the first call (`arm_torque_open` with the port name as a NUL-terminated string, the current limit and the zero-delay flag), then `arm_torque_read` fills the four arrays. Convert: pulses to radians about 2048 and wrap; velocity units to rad/s; current units to amps to newton-metres.

**Lines 83–92.** `updateImpl`: in generated code, if the port is open, the torque input is first clipped by the position limits ([[spot.arm.withinLimits|spot-arm-withinLimits]] with the last read pulses), then divided by the torque constant and the current unit to become goal-current units, and written (`arm_torque_write`).

**Lines 94–101.** `releaseImpl`: close the port (goal 0, torque off) at the end.

**Lines 104–117.** The block's interface: no direct feed-through; one input `Torque`, four outputs with their sizes (3, 3, 3, 5), all double, real, fixed-size; the icon text.

**Lines 121–135.** The block's help header; always *Interpreted execution* in simulation (the option is hidden).

**Lines 139–154.** For code generation: the name, that it is supported for `rtw` targets, and `updateBuildInfo`, which adds `csrc/arm/include` and `arm_torque.cpp` to the build.

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/PositionControl.m`, `+spot/+arm/VelocityControl.m`, `+spot/+arm/defaults.m`, `+spot/+arm/servoFromDiagram.m`, `+spot/+arm/withinLimits.m`, `+spot/+hardware/arm/setup_arm_serial.sh`, `+spot/+hardware/csrc/arm/include/arm_torque.h`, `+spot/+hardware/csrc/arm/src/arm_torque.cpp`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]], [[spot.arm.withinLimits|spot-arm-withinLimits]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/ArmTorque.m</b> (156 lines; click to show)</summary>

```matlab
   1  classdef ArmTorque < matlab.System & coder.ExternalDependency
   2  %ARMTORQUE The arm's three Dynamixel servos driven by torque: joint torques in, encoders and torques out.
   3  %   The diagram's "Dynamixel Arm (Torque)" block, in place of the old Control Dynamixel Actuators
   4  %   and Read Joint Positions blocks (Dynamixel_Controller, ReadArm_Position_Rates, now in
   5  %   ../archive/old_arm_blocks). The servos (IDs 1-3 on /dev/ttyUSB0, 1 Mbps) stay in current control
   6  %   (operating mode 0) for the whole run: position and velocity control are controllers in the
   7  %   diagram (spot.arm.PositionControl, spot.arm.VelocityControl) whose output is torque.
   8  %
   9  %   Each step:
  10  %     - Outputs (where the block sits in the step: before the controllers): one sync read of each
  11  %       servo's Present Position, Velocity and Current, as Position [rad] (0 straight out, as the
  12  %       old block), Velocity [rad/s] and Torque [N m] (the current times TorqueConstant). A servo
  13  %       that doesn't answer keeps its last values; Status says so.
  14  %     - Update (at the end of the step, after the controllers): the Torque input [N m] becomes each
  15  %       servo's Goal Current (torque / TorqueConstant, in 3.36 mA units, within CurrentLimit), in
  16  %       one sync write. A joint at or beyond MinPosition/MaxPosition (pulses; +-90 deg, as the old
  17  %       blocks' position limits) gets no torque that pushes it further: in current control the
  18  %       servos have no position limits of their own. Because the input is used only there, the block can sit before the
  19  %       controllers that compute it: the diagram feeds it ARM_Controls from "Robotic Arm Code".
  20  %   Status: [servos that answered this step (0-3), the read's time [ms], the USB adapter's latency
  21  %   timer [ms] (-1 unknown; the block asks for 1 ms), servo reads missed since the start, 1 if a
  22  %   servo reports a hardware error (e.g. overload; the servo then turns its torque off)].
  23  %
  24  %   The first step opens the port and sets the servos up: torque off, current control, Current
  25  %   Limit, Return Delay Time 0 (ZeroReturnDelay, an EEPROM change), Goal Current 0, torque on.
  26  %   At the end of the run: Goal Current 0, torque off. In simulation the block does nothing (the
  27  %   plant, spot.arm.Plant, stands in for the arm).
  28  %
  29  %   C code: +spot/+hardware/csrc/arm/src/arm_torque.cpp (protocol 2.0, no Dynamixel SDK).
  30  
  31      %#codegen
  32  
  33      properties (Nontunable)
  34          Port = '/dev/ttyUSB0'            % the U2D2's serial port
  35          CurrentLimit = 850               % Current Limit [3.36 mA]: written to the servos, and goals are kept within it
  36          TorqueConstant = 6.0 / 4.1       % [N m/A] at the joint (MX-64(2.0) stall torque / stall current, as armModel.motor)
  37          ZeroReturnDelay (1,1) logical = false   % set each servo's Return Delay Time to 0 (from 500 us): faster reads
  38          MinPosition = 1024               % [pulses] a joint at or beyond it gets no torque pushing it further
  39          MaxPosition = 3072               % [pulses] (the old blocks' Min/Max Position Limit: +-90 deg)
  40      end
  41  
  42      properties (Access = private)
  43          opened = false
  44          pulses = 2048 * ones(3, 1)       % the last positions read
  45      end
  46  
  47      properties (Constant, Access = private)
  48          PULSES = 4096 / (2*pi)           % encoder pulses per rad
  49          VEL_UNIT = 0.229 * 2*pi/60       % [rad/s] per velocity unit
  50          CUR_UNIT = 3.36e-3               % [A] per current unit
  51      end
  52  
  53      methods
  54          function obj = ArmTorque(varargin)
  55              setProperties(obj, nargin, varargin{:});
  56          end
  57      end
  58  
  59      methods (Access = protected)
  60          function setupImpl(obj)
  61              obj.opened = false;
  62          end
  63  
  64          function [pos, vel, torque, status] = outputImpl(obj, ~)
  65              pulses = 2048 * ones(3, 1); units = zeros(3, 1); current = zeros(3, 1);
  66              status = zeros(5, 1);
  67              coder.cinclude('arm_torque.h');
  68              if ~isempty(coder.target)
  69                  if ~obj.opened   % only on the platform that runs the arm (the block's subsystem is gated)
  70                      port = [obj.Port char(0)];
  71                      coder.ceval('arm_torque_open', coder.rref(port), obj.CurrentLimit, double(obj.ZeroReturnDelay));
  72                      obj.opened = true;
  73                  end
  74                  coder.ceval('arm_torque_read', coder.wref(pulses), coder.wref(units), coder.wref(current), coder.wref(status));
  75              end
  76              obj.pulses = pulses;
  77              % 2048 = straight out; wrapped to [-pi, pi)
  78              pos = mod((pulses - 2048) / obj.PULSES + pi, 2*pi) - pi;
  79              vel = units * obj.VEL_UNIT;
  80              torque = current * obj.CUR_UNIT * obj.TorqueConstant;
  81          end
  82  
  83          function updateImpl(obj, tau)
  84              coder.cinclude('arm_torque.h');
  85              if ~isempty(coder.target)
  86                  if obj.opened
  87                      goal = spot.arm.withinLimits(tau(:), obj.pulses, obj.MinPosition, obj.MaxPosition) ...
  88                          / obj.TorqueConstant / obj.CUR_UNIT;
  89                      coder.ceval('arm_torque_write', coder.rref(goal));
  90                  end
  91              end
  92          end
  93  
  94          function releaseImpl(obj)
  95              coder.cinclude('arm_torque.h');
  96              if ~isempty(coder.target)
  97                  if obj.opened
  98                      coder.ceval('arm_torque_close');
  99                  end
 100              end
 101          end
 102  
 103          % ------------------------------------------------------------ Simulink block interface
 104          function flag = isInputDirectFeedthroughImpl(~, ~)
 105              flag = false;   % the torque goes out in Update: the reads don't wait for it
 106          end
 107          function n = getNumInputsImpl(~), n = 1; end
 108          function n = getNumOutputsImpl(~), n = 4; end
 109          function a = getInputNamesImpl(~), a = 'Torque'; end
 110          function [a, b, c, d] = getOutputNamesImpl(~), a = 'Position'; b = 'Velocity'; c = 'Torque'; d = 'Status'; end
 111          function [a, b, c, d] = getOutputSizeImpl(~), a = [3 1]; b = [3 1]; c = [3 1]; d = [5 1]; end
 112          function [a, b, c, d] = getOutputDataTypeImpl(~), a = 'double'; b = 'double'; c = 'double'; d = 'double'; end
 113          function [a, b, c, d] = isOutputComplexImpl(~), a = false; b = false; c = false; d = false; end
 114          function [a, b, c, d] = isOutputFixedSizeImpl(~), a = true; b = true; c = true; d = true; end
 115          function icon = getIconImpl(~)
 116              icon = sprintf('Dynamixel Arm\n(torque in;\nencoders, torque out)');
 117          end
 118      end
 119  
 120      methods (Static, Access = protected)
 121          function header = getHeaderImpl
 122              header = matlab.system.display.Header('spot.hardware.ArmTorque', 'Title', 'Dynamixel Arm (Torque)', 'Text', ...
 123                  ['The arm''s three Dynamixel servos in current control. Outputs (read before the controllers): ' ...
 124                   'joint angles [rad], speeds [rad/s], torques [N m] and a status [servos read, read time ms, ' ...
 125                   'USB latency timer ms, reads missed, hardware error]. Input: the joint torques [N m] ' ...
 126                   '(ARM_Controls), sent at the end of the step. See help spot.hardware.ArmTorque.']);
 127          end
 128  
 129          function simMode = getSimulateUsingImpl(~)
 130              simMode = 'Interpreted execution';
 131          end
 132  
 133          function isVisible = showSimulateUsingImpl
 134              isVisible = false;
 135          end
 136      end
 137  
 138      methods (Static)
 139          function name = getDescriptiveName(~)
 140              name = 'ArmTorque';
 141          end
 142  
 143          function b = isSupportedContext(context)
 144              b = context.isCodeGenTarget('rtw');
 145          end
 146  
 147          function updateBuildInfo(buildInfo, context)
 148              if context.isCodeGenTarget('rtw')
 149                  here = fileparts(mfilename('fullpath'));
 150                  addIncludePaths(buildInfo, fullfile(here, 'csrc', 'arm', 'include'));
 151                  addIncludeFiles(buildInfo, 'arm_torque.h', fullfile(here, 'csrc', 'arm', 'include'));
 152                  addSourceFiles(buildInfo, 'arm_torque.cpp', fullfile(here, 'csrc', 'arm', 'src'));
 153              end
 154          end
 155      end
 156  end
```

</details>
<!-- /listing -->
