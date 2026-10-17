classdef PositionControl < matlab.System
    %POSITIONCONTROL The Dynamixel servos' position control, as a controller giving joint PWM.
    %   The diagram's "Default Position Control (ARM)" block. It runs the MX-64(2.0) position
    %   control law (operating mode 3) with the servo's settings and units, but in the diagram
    %   every StepTime, and outputs the PWM the servo would set: the servos stay in PWM control
    %   (spot.hardware.ArmPWM), so they apply it as the servo's own law would.
    %
    %   Each step, per joint:
    %     1. Goal: [rad] to pulses as the old hardware code did (CMD_PULSES_PER_RAD pulses/rad
    %        + CENTER_PULSE). A goal beyond Min/Max Position is ignored, so the joint keeps its
    %        last goal.
    %     2. Profile: the desired position moves to the goal at Profile Velocity with Profile
    %        Acceleration (0 = none: the goal at once).
    %     3. PID + feedforward in the servo's units, per ServoPeriod (integral and derivative
    %        scaled to StepTime), within PWM Limit [885 = 100 %], in whole units:
    %          PWM = P/128 e + I/65536 sum(e) + D/16 de + FF1/4 vd + FF2/4 ad,  e in pulses
    %
    %   At StepTime = ServoPeriod this is the servo's own position control. At the diagram's
    %   SAMPLE_RATE the servo's gains (SERVO_POSITION_P/I/D) are unstable, so the defaults
    %   (ARM_POSITION_P/I/D) are retuned for it: about 1 N m/rad stiffness at rest
    %   (0.0068 N m per PWM unit), with the I gain removing the last few degrees against
    %   stiction (slowly, within ~3 deg). The motors' back-EMF damps the joints on top of the D
    %   gain. Every default is Constants'.
    %   In the diagram: armControl.position (Control pane).
    %
    %   Each gain is one value for all three servos, or [shoulder elbow wrist].
    %
    %   Inputs
    %     goalRad   3x1 goal joint angles [rad] (ARM_Path)
    %     meas      6x1 [q1 q2 q3 w1 w2 w3] [rad, rad/s] (ARM_Measured_States)
    %
    %   Outputs
    %     pwm       3x1 joint commands [PWM, 885 = 100 %] (ARM_Controls)
    %
    %   The first step (and the first after a reset) holds where the arm is, as a servo does
    %   when its torque is switched on.
    %
    %   Example
    %     c = spot.arm.PositionControl('StepTime', 0.05);
    %     pwm = c(goal, [q; w]);
    %
    %   See also spot.arm.VelocityControl, spot.arm.Plant, spot.arm.compareSteps.

    %#codegen

    properties (Nontunable)
        StepTime = 1 / Constants.SAMPLE_RATE           % [s] controller period (baseRate)
        ServoPeriod = Constants.SERVO_PERIOD           % [s] the servo's; its gains are per it
        PositionP = Constants.ARM_POSITION_P           % Position P Gain [shoulder elbow wrist]
        PositionI = Constants.ARM_POSITION_I           % Position I Gain
        PositionD = Constants.ARM_POSITION_D           % Position D Gain
        Feedforward1 = Constants.SERVO_FEEDFORWARD_1   % Feedforward 1st Gain
        Feedforward2 = Constants.SERVO_FEEDFORWARD_2   % Feedforward 2nd Gain
        ProfileVelocity = Constants.ARM_POSITION_PROFILE_VELOCITY           % 0 = no profile
        ProfileAcceleration = Constants.ARM_POSITION_PROFILE_ACCELERATION   % 0 = none
        MinPosition = Constants.MIN_POSITION           % Min Position Limit [pulses]
        MaxPosition = Constants.MAX_POSITION           % Max Position Limit [pulses]
        PWMLimit = Constants.PWM_FULL                  % PWM Limit
    end

    properties (Access = private)
        started = false                 % holding position has been initialised
        goal = zeros(3, 1)              % [pulses, relative to CENTER_PULSE] goal position
        pd = zeros(3, 1)                % [pulses] profile's desired position
        vd = zeros(3, 1)                % [pulses/s] profile's desired velocity
        ePrev = zeros(3, 1)             % [pulses] position error last step
        eInt = zeros(3, 1)              % [pulses * servo periods] position error integral
    end

    properties (Constant, Access = private)
        PULSES = Constants.PULSES_PER_REV / (2*pi)       % [pulses/rad] encoder
        CMD_PULSES = Constants.CMD_PULSES_PER_RAD        % [pulses/rad] goals (old hw code)
        CENTER_PULSE = Constants.CENTER_PULSE            % [pulses] joint straight out
        VEL_UNIT = Constants.VEL_UNIT_RPM * 2*pi/60      % [rad/s] per velocity unit
        ACC_UNIT = Constants.ACC_UNIT_RPM2 * 2*pi/3600   % [rad/s^2] per profile accel. unit
        P_SCALE = Constants.GAIN_SCALE_P                 % gain scalings: PWM per gain unit
        I_SCALE = Constants.GAIN_SCALE_I
        D_SCALE = Constants.GAIN_SCALE_D
        FF_SCALE = Constants.GAIN_SCALE_FF
    end

    methods
        function obj = PositionControl(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function resetImpl(obj)
            obj.started = false;
        end

        function pwm = stepImpl(obj, goalRad, meas)
            h = obj.StepTime;

            % Present position as the servo sees it (encoder pulses).
            presPulses = round(meas(1:3) * obj.PULSES);

            % First step: torque on, hold where the arm is.
            if ~obj.started
                obj.goal = presPulses(:);
                obj.pd = presPulses(:);
                obj.vd = zeros(3, 1);
                obj.ePrev = zeros(3, 1);
                obj.eInt = zeros(3, 1);
                obj.started = true;
            end

            % New goals; a goal beyond a position limit is ignored.
            for j = 1:3
                goalPulses = round(obj.CMD_PULSES * goalRad(j) + obj.CENTER_PULSE);
                if goalPulses >= obj.MinPosition && goalPulses <= obj.MaxPosition
                    obj.goal(j) = goalPulses - obj.CENTER_PULSE;
                end
            end

            % Profile limits in pulses (0 = none).
            profileVel = obj.ProfileVelocity * obj.VEL_UNIT * obj.PULSES;      % [pulses/s]
            profileAcc = obj.ProfileAcceleration * obj.ACC_UNIT * obj.PULSES;  % [pulses/s^2]

            pwm = zeros(3, 1);
            for j = 1:3

                % Profile: move the desired position toward the goal.
                vdPrev = obj.vd(j);
                dist = obj.goal(j) - obj.pd(j);
                if profileVel <= 0
                    obj.pd(j) = obj.goal(j);
                    obj.vd(j) = 0;
                else
                    % Target speed: Profile Velocity, slowed to stop at the goal.
                    vTarget = profileVel;
                    if profileAcc > 0
                        vTarget = min(vTarget, sqrt(2 * profileAcc * abs(dist)));
                    end
                    vTarget = sign(dist) * vTarget;

                    % Desired velocity, within Profile Acceleration.
                    if profileAcc > 0
                        obj.vd(j) = obj.vd(j) ...
                                  + min(max(vTarget - obj.vd(j), -profileAcc * h), profileAcc * h);
                    else
                        obj.vd(j) = vTarget;
                    end

                    % Desired position; snap to the goal when this step would reach it.
                    if abs(obj.vd(j) * h) >= abs(dist)
                        obj.pd(j) = obj.goal(j);
                        obj.vd(j) = 0;
                    else
                        obj.pd(j) = obj.pd(j) + obj.vd(j) * h;
                    end
                end
                accDesired = (obj.vd(j) - vdPrev) / h;

                % Error, its derivative and integral, per servo period.
                posErr = obj.pd(j) - presPulses(j);
                errRate = (posErr - obj.ePrev(j)) * obj.ServoPeriod / h;
                obj.ePrev(j) = posErr;
                errIntNew = obj.eInt(j) + posErr * h / obj.ServoPeriod;

                % PID + feedforward, in PWM.
                pwmJ = gain(obj.PositionP, j) / obj.P_SCALE * posErr ...
                     + gain(obj.PositionI, j) / obj.I_SCALE * errIntNew ...
                     + gain(obj.PositionD, j) / obj.D_SCALE * errRate ...
                     + gain(obj.Feedforward1, j) / obj.FF_SCALE * obj.vd(j) ...
                       / (obj.VEL_UNIT * obj.PULSES) ...
                     + gain(obj.Feedforward2, j) / obj.FF_SCALE * accDesired ...
                       / (obj.ACC_UNIT * obj.PULSES);

                % Anti-windup: hold the integral while saturated in the error's direction.
                if abs(pwmJ) < obj.PWMLimit || sign(pwmJ) ~= sign(posErr)
                    obj.eInt(j) = errIntNew;
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
    end

    methods (Static, Access = protected)
    end
end

function g = gain(value, j)
    % Gain for joint j: one value for all three, or one per joint.
    g = value(min(j, numel(value)));
end
