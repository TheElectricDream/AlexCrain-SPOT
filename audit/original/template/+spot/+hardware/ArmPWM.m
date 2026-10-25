classdef ArmPWM < matlab.System & coder.ExternalDependency
    %ARMPWM The arm's Dynamixel servos driven by PWM: joint PWM in, encoders and applied PWM out.
    %   The diagram's "Dynamixel Arm (PWM)" block, replacing the old Control Dynamixel Actuators
    %   and Read Joint Positions blocks (now in ../archive/old_arm_blocks). The servos (IDs 1-3
    %   on /dev/ttyUSB0, 1 Mbps) stay in PWM control (operating mode 16, voltage control) for
    %   the whole run: each command is the share of the supply voltage put across the motor
    %   [885 = 100 %], which at rest acts as a torque (spot.arm.defaults). Position and velocity
    %   control are controllers in the diagram (spot.control.ArmPositionControl,
    %   spot.control.ArmVelocityControl) whose output is PWM. The servos' current reading is not
    %   used: it is too coarse and noisy at the arm's small torques.
    %
    %   Each step:
    %     1. Outputs (before the controllers): one sync read of each servo's Present PWM,
    %        Velocity and Position. A servo that doesn't answer keeps its last values; Status
    %        says so.
    %     2. Update (end of the step, after the controllers): the commands become Goal PWM
    %        (whole units within PWMLimit) in one sync write. A joint at or beyond
    %        Min/MaxPosition gets no PWM pushing it further: in PWM control the servos have no
    %        position limits of their own. Because the input is used only here, the block can
    %        sit before the controllers that compute it.
    %
    %   The first step opens the port and sets the servos up: torque off, PWM control, PWM
    %   Limit, Return Delay Time 0 (ZeroReturnDelay, an EEPROM change), Goal PWM 0, torque on.
    %   At the end of the run: Goal PWM 0, torque off. In simulation the block does nothing
    %   (spot.arm.Plant stands in for the arm).
    %
    %   Inputs
    %     PWM       3x1 joint commands [PWM, 885 = 100 %] (ARM_Controls)
    %
    %   Outputs
    %     Position  3x1 joint angles [rad], 0 straight out
    %     Velocity  3x1 joint speeds [rad/s]
    %     PWM       3x1 PWM each servo applies (its Present PWM) [885 = 100 %]
    %     Status    [servos that answered this step (0-3), read time [ms], USB adapter latency
    %               timer [ms] (-1 unknown; the block asks for 1 ms), reads missed since the
    %               start, 1 if a servo reports a hardware error (e.g. overload: the servo then
    %               turns its torque off)]
    %
    %   C code: +spot/+hardware/csrc/arm/src/arm_pwm.cpp (protocol 2.0, no Dynamixel SDK).
    %
    %   See also spot.arm.Plant, spot.arm.withinLimits, spot.arm.servoFromDiagram.

    %#codegen

    properties (Nontunable)
        Port = '/dev/ttyUSB0'           % the U2D2's serial port
        PWMLimit = 885                  % PWM Limit written to the servos; commands kept within
        ZeroReturnDelay (1,1) logical = false   % Return Delay Time 0 (from 500 us): faster reads
        MinPosition = 1024              % [pulses] at or beyond: no PWM pushing further
        MaxPosition = 3072              % [pulses] (old blocks' limits: +-90 deg)
    end

    properties (Access = private)
        opened = false                  % port opened and servos set up
        pulses = 2048 * ones(3, 1)      % [pulses] last positions read
    end

    properties (Constant, Access = private)
        PULSES = 4096 / (2*pi)          % [pulses/rad] encoder
        VEL_UNIT = 0.229 * 2*pi/60      % [rad/s] per velocity unit
        CENTER_PULSE = 2048             % [pulses] joint straight out
    end

    methods
        function obj = ArmPWM(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            obj.opened = false;
        end

        function [pos, vel, applied, status] = outputImpl(obj, ~)
            % Values in a simulation (straight out, still).
            pulses = obj.CENTER_PULSE * ones(3, 1);
            units = zeros(3, 1);
            applied = zeros(3, 1);
            status = zeros(5, 1);
            coder.cinclude('arm_pwm.h');

            % Generated code: set up on the first step, then read the servos.
            if ~isempty(coder.target)
                % Only on the platform that runs the arm (the block's subsystem is gated).
                if ~obj.opened
                    port = [obj.Port char(0)];
                    coder.ceval('arm_pwm_open', coder.rref(port), obj.PWMLimit, ...
                        double(obj.ZeroReturnDelay));
                    obj.opened = true;
                end
                coder.ceval('arm_pwm_read', coder.wref(pulses), coder.wref(units), ...
                    coder.wref(applied), coder.wref(status));
            end
            obj.pulses = pulses;

            % Servo units to joint units; angles wrapped to [-pi, pi).
            pos = mod((pulses - obj.CENTER_PULSE) / obj.PULSES + pi, 2*pi) - pi;
            vel = units * obj.VEL_UNIT;
        end

        function updateImpl(obj, pwm)
            % Generated code: the commands as Goal PWM, none pushing past a limit (the C code
            % rounds them and keeps them within the PWM Limit).
            coder.cinclude('arm_pwm.h');
            if ~isempty(coder.target)
                if obj.opened
                    goal = spot.arm.withinLimits(pwm(:), obj.pulses, obj.MinPosition, ...
                                                 obj.MaxPosition);
                    coder.ceval('arm_pwm_write', coder.rref(goal));
                end
            end
        end

        function releaseImpl(obj)
            % Generated code: Goal PWM 0, torque off, port closed.
            coder.cinclude('arm_pwm.h');
            if ~isempty(coder.target)
                if obj.opened
                    coder.ceval('arm_pwm_close');
                end
            end
        end

        %% Simulink block interface
        function flag = isInputDirectFeedthroughImpl(~, ~)
            flag = false;   % the PWM goes out in Update: the reads don't wait for it
        end

        function n = getNumInputsImpl(~)
            n = 1;
        end

        function n = getNumOutputsImpl(~)
            n = 4;
        end

        function a = getInputNamesImpl(~)
            a = 'PWM';
        end

        function [a, b, c, d] = getOutputNamesImpl(~)
            a = 'Position';
            b = 'Velocity';
            c = 'PWM';
            d = 'Status';
        end

        function [a, b, c, d] = getOutputSizeImpl(~)
            a = [3 1];
            b = [3 1];
            c = [3 1];
            d = [5 1];
        end

        function [a, b, c, d] = getOutputDataTypeImpl(~)
            a = 'double';
            b = 'double';
            c = 'double';
            d = 'double';
        end

        function [a, b, c, d] = isOutputComplexImpl(~)
            a = false;
            b = false;
            c = false;
            d = false;
        end

        function [a, b, c, d] = isOutputFixedSizeImpl(~)
            a = true;
            b = true;
            c = true;
            d = true;
        end

        function icon = getIconImpl(~)
            icon = sprintf('Dynamixel Arm\n(PWM in;\nencoders, PWM out)');
        end
    end

    methods (Static, Access = protected)
        function header = getHeaderImpl
            header = matlab.system.display.Header('spot.hardware.ArmPWM', ...
                'Title', 'Dynamixel Arm (PWM)', ...
                'Text', ['The arm''s three Dynamixel servos in PWM (voltage) control. Outputs ' ...
                         '(read before the controllers): joint angles [rad], speeds [rad/s], ' ...
                         'the PWM each servo applies [885 = 100 %] and a status [servos read, ' ...
                         'read time ms, USB latency timer ms, reads missed, hardware error]. ' ...
                         'Input: the joint commands [PWM] (ARM_Controls), sent at the end of ' ...
                         'the step. See help spot.hardware.ArmPWM.']);
        end

        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end

        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
    end

    methods (Static)
        %% coder.ExternalDependency interface
        function name = getDescriptiveName(~)
            name = 'ArmPWM';
        end

        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end

        function updateBuildInfo(buildInfo, context)
            % Add the C++ source and header to the build.
            if context.isCodeGenTarget('rtw')
                here = fileparts(mfilename('fullpath'));
                addIncludePaths(buildInfo, fullfile(here, 'csrc', 'arm', 'include'));
                addIncludeFiles(buildInfo, 'arm_pwm.h', ...
                    fullfile(here, 'csrc', 'arm', 'include'));
                addSourceFiles(buildInfo, 'arm_pwm.cpp', fullfile(here, 'csrc', 'arm', 'src'));
            end
        end
    end
end
