classdef ArmTorque < matlab.System & coder.ExternalDependency
    %ARMTORQUE The arm's Dynamixel servos driven by torque: torques in, encoders and torques out.
    %   The diagram's "Dynamixel Arm (Torque)" block, replacing the old Control Dynamixel
    %   Actuators and Read Joint Positions blocks (now in ../archive/old_arm_blocks). The servos
    %   (IDs 1-3 on /dev/ttyUSB0, 1 Mbps) stay in current control (operating mode 0) for the
    %   whole run; position and velocity control are controllers in the diagram
    %   (spot.arm.PositionControl, spot.arm.VelocityControl) whose output is torque.
    %
    %   Each step:
    %     1. Outputs (before the controllers): one sync read of each servo's Present Position,
    %        Velocity and Current. A servo that doesn't answer keeps its last values; Status
    %        says so.
    %     2. Update (end of the step, after the controllers): the torques become Goal Currents
    %        (torque / TorqueConstant, in 3.36 mA units, within CurrentLimit) in one sync write.
    %        A joint at or beyond Min/MaxPosition gets no torque pushing it further: in current
    %        control the servos have no position limits of their own. Because the input is used
    %        only here, the block can sit before the controllers that compute it.
    %
    %   The first step opens the port and sets the servos up: torque off, current control,
    %   Current Limit, Return Delay Time 0 (ZeroReturnDelay, an EEPROM change), Goal Current 0,
    %   torque on. At the end of the run: Goal Current 0, torque off. In simulation the block
    %   does nothing (spot.arm.Plant stands in for the arm).
    %
    %   Inputs
    %     Torque    3x1 joint torques [N m] (ARM_Controls)
    %
    %   Outputs
    %     Position  3x1 joint angles [rad], 0 straight out
    %     Velocity  3x1 joint speeds [rad/s]
    %     Torque    3x1 joint torques [N m] (current x TorqueConstant)
    %     Status    [servos that answered this step (0-3), read time [ms], USB adapter latency
    %               timer [ms] (-1 unknown; the block asks for 1 ms), reads missed since the
    %               start, 1 if a servo reports a hardware error (e.g. overload: the servo then
    %               turns its torque off)]
    %
    %   C code: +spot/+hardware/csrc/arm/src/arm_torque.cpp (protocol 2.0, no Dynamixel SDK).
    %
    %   See also spot.arm.Plant, spot.arm.withinLimits, spot.arm.servoFromDiagram.

    %#codegen

    properties (Nontunable)
        Port = '/dev/ttyUSB0'           % the U2D2's serial port
        CurrentLimit = 850              % [3.36 mA] written to the servos; goals kept within
        TorqueConstant = 6.0 / 4.1      % [N m/A] at the joint (MX-64(2.0) stall torque / current)
        ZeroReturnDelay (1,1) logical = false   % Return Delay Time 0 (from 500 us): faster reads
        MinPosition = 1024              % [pulses] at or beyond: no torque pushing further
        MaxPosition = 3072              % [pulses] (old blocks' limits: +-90 deg)
    end

    properties (Access = private)
        opened = false                  % port opened and servos set up
        pulses = 2048 * ones(3, 1)      % [pulses] last positions read
    end

    properties (Constant, Access = private)
        PULSES = 4096 / (2*pi)          % [pulses/rad] encoder
        VEL_UNIT = 0.229 * 2*pi/60      % [rad/s] per velocity unit
        CUR_UNIT = 3.36e-3              % [A] per current unit
        CENTER_PULSE = 2048             % [pulses] joint straight out
    end

    methods
        function obj = ArmTorque(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            obj.opened = false;
        end

        function [pos, vel, torque, status] = outputImpl(obj, ~)
            % Values in a simulation (straight out, still).
            pulses = obj.CENTER_PULSE * ones(3, 1);
            units = zeros(3, 1);
            current = zeros(3, 1);
            status = zeros(5, 1);
            coder.cinclude('arm_torque.h');

            % Generated code: set up on the first step, then read the servos.
            if ~isempty(coder.target)
                % Only on the platform that runs the arm (the block's subsystem is gated).
                if ~obj.opened
                    port = [obj.Port char(0)];
                    coder.ceval('arm_torque_open', coder.rref(port), obj.CurrentLimit, ...
                        double(obj.ZeroReturnDelay));
                    obj.opened = true;
                end
                coder.ceval('arm_torque_read', coder.wref(pulses), coder.wref(units), ...
                    coder.wref(current), coder.wref(status));
            end
            obj.pulses = pulses;

            % Servo units to joint units; angles wrapped to [-pi, pi).
            pos = mod((pulses - obj.CENTER_PULSE) / obj.PULSES + pi, 2*pi) - pi;
            vel = units * obj.VEL_UNIT;
            torque = current * obj.CUR_UNIT * obj.TorqueConstant;
        end

        function updateImpl(obj, tau)
            % Generated code: torques to goal currents [units], none pushing past a limit.
            coder.cinclude('arm_torque.h');
            if ~isempty(coder.target)
                if obj.opened
                    goal = spot.arm.withinLimits(tau(:), obj.pulses, obj.MinPosition, ...
                                                 obj.MaxPosition) ...
                           / obj.TorqueConstant / obj.CUR_UNIT;
                    coder.ceval('arm_torque_write', coder.rref(goal));
                end
            end
        end

        function releaseImpl(obj)
            % Generated code: goal current 0, torque off, port closed.
            coder.cinclude('arm_torque.h');
            if ~isempty(coder.target)
                if obj.opened
                    coder.ceval('arm_torque_close');
                end
            end
        end

        %% Simulink block interface
        function flag = isInputDirectFeedthroughImpl(~, ~)
            flag = false;   % the torque goes out in Update: the reads don't wait for it
        end

        function n = getNumInputsImpl(~)
            n = 1;
        end

        function n = getNumOutputsImpl(~)
            n = 4;
        end

        function a = getInputNamesImpl(~)
            a = 'Torque';
        end

        function [a, b, c, d] = getOutputNamesImpl(~)
            a = 'Position';
            b = 'Velocity';
            c = 'Torque';
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
            icon = sprintf('Dynamixel Arm\n(torque in;\nencoders, torque out)');
        end
    end

    methods (Static, Access = protected)
        function header = getHeaderImpl
            header = matlab.system.display.Header('spot.hardware.ArmTorque', ...
                'Title', 'Dynamixel Arm (Torque)', ...
                'Text', ['The arm''s three Dynamixel servos in current control. Outputs (read ' ...
                         'before the controllers): joint angles [rad], speeds [rad/s], torques ' ...
                         '[N m] and a status [servos read, read time ms, USB latency timer ms, ' ...
                         'reads missed, hardware error]. Input: the joint torques [N m] ' ...
                         '(ARM_Controls), sent at the end of the step. See help ' ...
                         'spot.hardware.ArmTorque.']);
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
            name = 'ArmTorque';
        end

        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end

        function updateBuildInfo(buildInfo, context)
            % Add the C++ source and header to the build.
            if context.isCodeGenTarget('rtw')
                here = fileparts(mfilename('fullpath'));
                addIncludePaths(buildInfo, fullfile(here, 'csrc', 'arm', 'include'));
                addIncludeFiles(buildInfo, 'arm_torque.h', ...
                    fullfile(here, 'csrc', 'arm', 'include'));
                addSourceFiles(buildInfo, 'arm_torque.cpp', fullfile(here, 'csrc', 'arm', 'src'));
            end
        end
    end
end
