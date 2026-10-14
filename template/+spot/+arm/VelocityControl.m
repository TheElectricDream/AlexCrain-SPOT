classdef VelocityControl < matlab.System
%VELOCITYCONTROL The Dynamixel servos' own velocity control, as a controller giving joint torques.
%   The diagram's "Default Velocity Control (ARM)" block. It runs the control law the MX-64(2.0)
%   runs in velocity control mode (operating mode 1), with the same settings in the same units,
%   but in the diagram, every StepTime, and its output is torque: the arm's servos stay in current
%   control (spot.hardware.ArmTorque on the hardware, spot.arm.Plant in simulation).
%     - Goal Velocity: each goal [rad/s] becomes velocity units [0.229 rev/min] as the old hardware
%       code converted it; a goal beyond Velocity Limit is ignored, as the servo ignores it.
%     - Profile: the desired velocity moves to the goal at Profile Acceleration
%       [214.577 rev/min^2] (0 = none: the goal at once).
%     - PI on the error, in the servo's units: PWM = P/128 e + I/65536 sum(e), e in velocity
%       units, per ServoPeriod (the controller scales the integral to its StepTime), within PWM
%       Limit [885 = 100 %]. Each gain is one value for the three servos or [shoulder elbow wrist].
%     - PWM to torque as spot.arm.PositionControl (Motor, armModel.motor; BackEMF).
%   Run at the servo's own period (StepTime = ServoPeriod = 1 ms) with BackEMF on, it gives the
%   servo's own velocity control (spot.arm.Plant's mode 3 before 2026-10-11). The defaults are
%   retuned for the diagram's 20 Hz (as spot.arm.PositionControl's); a joint whose stiction is
%   well above the default friction can take a second or more to break away.
%
%   Inputs: Goal, the goal joint speeds [rad/s]; Measured, [q1 q2 q3 w1 w2 w3] [rad, rad/s]
%   (ARM_Measured_States). Output: Torque [N m] for each joint (ARM_Controls).
%
%   Outside Simulink:
%     c = spot.arm.VelocityControl('Motor', armModel.motor, 'StepTime', 0.05);
%     tau = c(goalSpeeds, [q; w]);

    %#codegen

    properties (Nontunable)
        Motor = struct('stallTorque', [6 6 6], 'stallCurrent', [4.1 4.1 4.1], 'noLoadSpeed', [63 63 63] * 2*pi/60, ...
            'voltage', 12)                % armModel.motor: the servos' motor (spot.arm.defaults)
        StepTime = 0.05                  % [s] how often the controller runs (the diagram's baseRate)
        ServoPeriod = 1e-3               % [s] the servo's control period, which its gains are per
        VelocityP = [160 70 30]          % Velocity P Gain [shoulder elbow wrist] (SPOT's servos had 100)
        VelocityI = [220 1500 300]       % Velocity I Gain (SPOT's servos had 1920)
        ProfileAcceleration = 0          % [214.577 rev/min^2], 0 = none
        VelocityLimit = 1023             % Velocity Limit [0.229 rev/min]
        PWMLimit = 885                   % PWM Limit
        BackEMF (1,1) logical = false    % include the motor's back-EMF in the PWM's torque
    end

    properties (SetAccess = private)
        PWM = zeros(3, 1)                % the last step's PWM, as the servo would set it
    end

    properties (Access = private)
        vGoal = zeros(3, 1)              % goal velocity [rad/s]
        vDes = zeros(3, 1)               % the profile's desired velocity [rad/s]
        vInt = zeros(3, 1)               % velocity error integral [units * servo periods]
        Kt = zeros(3, 1)
        R = zeros(3, 1)
        Ke = zeros(3, 1)
        V = 12
    end

    properties (Constant, Access = private)
        VEL_UNIT = 0.229 * 2*pi/60       % [rad/s] per velocity unit
        ACC_UNIT = 214.577 * 2*pi/3600   % [rad/s^2] per profile acceleration unit
    end

    methods
        function obj = VelocityControl(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            mo = obj.Motor;
            obj.Kt = mo.stallTorque(:) ./ mo.stallCurrent(:);
            obj.R = mo.voltage ./ mo.stallCurrent(:);
            obj.Ke = mo.voltage ./ mo.noLoadSpeed(:);
            obj.V = mo.voltage;
        end

        function resetImpl(obj)
            obj.vGoal = zeros(3, 1);
            obj.vDes = zeros(3, 1);
            obj.vInt = zeros(3, 1);
            obj.PWM = zeros(3, 1);
        end

        function tau = stepImpl(obj, goal, meas)
            h = obj.StepTime;
            for j = 1:3                                    % goals beyond the limit are ignored
                units = round(goal(j) * 9.549296585513702 * 4.366756307);
                if abs(units) <= obj.VelocityLimit
                    obj.vGoal(j) = units * obj.VEL_UNIT;
                end
            end
            w = meas(4:6);
            presUnits = round(w(:) / obj.VEL_UNIT);        % the servo sees its own velocity units
            Av = obj.ProfileAcceleration * obj.ACC_UNIT;   % [rad/s^2], 0 = none
            u = zeros(3, 1);
            for j = 1:3
                if Av > 0
                    obj.vDes(j) = obj.vDes(j) + min(max(obj.vGoal(j) - obj.vDes(j), -Av * h), Av * h);
                else
                    obj.vDes(j) = obj.vGoal(j);
                end
                e = obj.vDes(j) / obj.VEL_UNIT - presUnits(j);
                iNew = obj.vInt(j) + e * h / obj.ServoPeriod;
                uj = gain(obj.VelocityP, j) / 128 * e + gain(obj.VelocityI, j) / 65536 * iNew;
                if abs(uj) < obj.PWMLimit || sign(uj) ~= sign(e)
                    obj.vInt(j) = iNew;
                end
                u(j) = min(max(uj, -obj.PWMLimit), obj.PWMLimit);
            end
            obj.PWM = u;
            tau = spot.arm.PositionControl.pwmTorque(u, w(:), obj.Kt, obj.R, obj.Ke, obj.V, obj.BackEMF);
        end

        % ------------------------------------------------------------ Simulink block interface
        function n = getNumInputsImpl(~), n = 2; end
        function n = getNumOutputsImpl(~), n = 1; end
        function [a, b] = getInputNamesImpl(~), a = 'Goal'; b = 'Measured'; end
        function a = getOutputNamesImpl(~), a = 'Torque'; end
        function a = getOutputSizeImpl(~), a = [3 1]; end
        function a = getOutputDataTypeImpl(~), a = 'double'; end
        function a = isOutputComplexImpl(~), a = false; end
        function a = isOutputFixedSizeImpl(~), a = true; end
        function icon = getIconImpl(~)
            icon = sprintf('Dynamixel velocity\ncontrol -> torque');
        end
    end

    methods (Static, Access = protected)
        function header = getHeaderImpl
            header = matlab.system.display.Header('spot.arm.VelocityControl', 'Title', 'Dynamixel Velocity Control', ...
                'Text', ['The MX-64(2.0)''s velocity control law (acceleration profile, PI), in its own units, run in ' ...
                'the diagram: its output is the joint torque [N m] the servo''s PWM would give, for the servos in ' ...
                'current control. Inputs: goal joint speeds [rad/s] and ARM_Measured_States. The gains are retuned ' ...
                'for the diagram''s 20 Hz. See help spot.arm.VelocityControl.']);
        end
    end
end

function g = gain(value, j)
% A gain for joint j: one value for all three, or one per joint
g = value(min(j, numel(value)));
end
