classdef ArmVelocityControl < matlab.System
    %ARMVELOCITYCONTROL The Dynamixel servos' velocity control, as a controller giving joint PWM.
    %   The diagram's "Default Velocity Control (ARM)" block. It runs the MX-64(2.0) velocity
    %   control law (operating mode 1) with the servo's settings and units, but in the diagram
    %   every StepTime, and outputs the PWM the servo would set: the servos stay in PWM control
    %   (spot.hardware.ArmPWM), so they apply it as the servo's own law would.
    %
    %   Each step, per joint:
    %     1. Goal: [rad/s] to velocity units [0.229 rev/min] as the old hardware code did. A goal
    %        beyond Velocity Limit is ignored, as the servo ignores it.
    %     2. Profile: the desired velocity moves to the goal at Profile Acceleration (0 = none).
    %     3. PI in the servo's units, per ServoPeriod (integral scaled to StepTime), within PWM
    %        Limit [885 = 100 %], in whole units:
    %          PWM = P/128 e + I/65536 sum(e),  e in velocity units
    %
    %   At StepTime = ServoPeriod = 1 ms this is the servo's own velocity control. The
    %   defaults are retuned for the diagram's 20 Hz; a joint with stiction well above the
    %   default friction can take a second or more to break away.
    %
    %   Each gain is one value for all three servos, or [shoulder elbow wrist].
    %
    %   Inputs
    %     goal      3x1 goal joint speeds [rad/s]
    %     meas      6x1 [q1 q2 q3 w1 w2 w3] [rad, rad/s] (ARM_Measured_States)
    %
    %   Outputs
    %     pwm       3x1 joint commands [PWM, 885 = 100 %] (ARM_Controls)
    %
    %   Example
    %     c = spot.control.ArmVelocityControl('StepTime', 0.05);
    %     pwm = c(goalSpeeds, [q; w]);
    %
    %   See also spot.control.ArmPositionControl, spot.arm.Plant, spot.arm.checkCommands.

    %#codegen

    properties (Nontunable)
        StepTime = 0.05                 % [s] controller period (the diagram's baseRate)
        ServoPeriod = 1e-3              % [s] servo's control period, which its gains are per
        VelocityP = [160 70 30]         % Velocity P Gain [shoulder elbow wrist] (servo had 100)
        VelocityI = [220 1500 300]      % Velocity I Gain (servo had 1920)
        ProfileAcceleration = 0         % [214.577 rev/min^2], 0 = none
        VelocityLimit = 1023            % Velocity Limit [0.229 rev/min]
        PWMLimit = 885                  % PWM Limit
    end

    properties (Access = private)
        vGoal = zeros(3, 1)             % [rad/s] goal velocity
        vDes = zeros(3, 1)              % [rad/s] profile's desired velocity
        vInt = zeros(3, 1)              % [units * servo periods] velocity error integral
    end

    properties (Constant, Access = private)
        VEL_UNIT = 0.229 * 2*pi/60      % [rad/s] per velocity unit
        ACC_UNIT = 214.577 * 2*pi/3600  % [rad/s^2] per profile acceleration unit
        RADPS_TO_RPM = 9.549296585513702    % [rev/min per rad/s] = 60/(2*pi)
        RPM_TO_VEL_UNITS = 4.366756307      % [units per rev/min] = 1/0.229
        P_SCALE = 128                   % Dynamixel gain scalings: PWM per gain unit
        I_SCALE = 65536
    end

    methods
        function obj = ArmVelocityControl(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function resetImpl(obj)
            obj.vGoal = zeros(3, 1);
            obj.vDes = zeros(3, 1);
            obj.vInt = zeros(3, 1);
        end

        function pwm = stepImpl(obj, goal, meas)
            h = obj.StepTime;

            % New goals, in whole velocity units; a goal beyond the limit is ignored.
            for j = 1:3
                goalUnits = round(goal(j) * obj.RADPS_TO_RPM * obj.RPM_TO_VEL_UNITS);
                if abs(goalUnits) <= obj.VelocityLimit
                    obj.vGoal(j) = goalUnits * obj.VEL_UNIT;
                end
            end

            % Present velocity as the servo sees it (whole velocity units).
            presUnits = round(meas(4:6) / obj.VEL_UNIT);

            % Profile acceleration [rad/s^2], 0 = none.
            profileAcc = obj.ProfileAcceleration * obj.ACC_UNIT;

            pwm = zeros(3, 1);
            for j = 1:3

                % Profile: move the desired velocity toward the goal.
                if profileAcc > 0
                    obj.vDes(j) = obj.vDes(j) ...
                                + min(max(obj.vGoal(j) - obj.vDes(j), -profileAcc * h), ...
                                      profileAcc * h);
                else
                    obj.vDes(j) = obj.vGoal(j);
                end

                % PI on the error in velocity units, integral per servo period.
                velErr = obj.vDes(j) / obj.VEL_UNIT - presUnits(j);
                errIntNew = obj.vInt(j) + velErr * h / obj.ServoPeriod;
                pwmJ = gain(obj.VelocityP, j) / obj.P_SCALE * velErr ...
                     + gain(obj.VelocityI, j) / obj.I_SCALE * errIntNew;

                % Anti-windup: hold the integral while saturated in the error's direction.
                if abs(pwmJ) < obj.PWMLimit || sign(pwmJ) ~= sign(velErr)
                    obj.vInt(j) = errIntNew;
                end
                pwm(j) = round(min(max(pwmJ, -obj.PWMLimit), obj.PWMLimit));
            end
        end

        %% Simulink block interface
        function n = getNumInputsImpl(~)
            n = 2;
        end

        function n = getNumOutputsImpl(~)
            n = 1;
        end

        function [a, b] = getInputNamesImpl(~)
            a = 'Goal';
            b = 'Measured';
        end

        function a = getOutputNamesImpl(~)
            a = 'PWM';
        end

        function a = getOutputSizeImpl(~)
            a = [3 1];
        end

        function a = getOutputDataTypeImpl(~)
            a = 'double';
        end

        function a = isOutputComplexImpl(~)
            a = false;
        end

        function a = isOutputFixedSizeImpl(~)
            a = true;
        end

        function icon = getIconImpl(~)
            icon = sprintf('Dynamixel velocity\ncontrol -> PWM');
        end
    end

    methods (Static, Access = protected)
        function header = getHeaderImpl
            header = matlab.system.display.Header('spot.control.ArmVelocityControl', ...
                'Title', 'Dynamixel Velocity Control', ...
                'Text', ['The MX-64(2.0)''s velocity control law (acceleration profile, PI), ' ...
                         'in its own units, run in the diagram: its output is the PWM the ' ...
                         'servo would set [885 = 100 %], for the servos in PWM control. ' ...
                         'Inputs: goal joint speeds [rad/s] and ARM_Measured_States. The gains ' ...
                         'are retuned for the diagram''s 20 Hz. See help ' ...
                         'spot.control.ArmVelocityControl.']);
        end
    end
end

function g = gain(value, j)
    % Gain for joint j: one value for all three, or one per joint.
    g = value(min(j, numel(value)));
end
