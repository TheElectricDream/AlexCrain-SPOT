classdef PositionControl < matlab.System
%POSITIONCONTROL The Dynamixel servos' own position control, as a controller giving joint torques.
%   The diagram's "Default Position Control (ARM)" block. It runs the control law the MX-64(2.0)
%   runs in position control mode (operating mode 3), with the same settings in the same units,
%   but in the diagram, every StepTime, and its output is torque: the arm's servos stay in current
%   control (spot.hardware.ArmTorque on the hardware, spot.arm.Plant in simulation).
%     - Goal Position: each goal [rad] becomes pulses as the old hardware code converted it
%       (651.74 pulses/rad + 2048); a goal beyond Min/Max Position is ignored, as the servo
%       ignores it, so the joint keeps its last goal.
%     - Profile: the desired position moves to the goal at Profile Velocity [0.229 rev/min] with
%       Profile Acceleration [214.577 rev/min^2] (0 = none: the goal at once).
%     - PID + feedforward on the error, in the servo's units: PWM = P/128 e + I/65536 sum(e)
%       + D/16 de + FF1/4 vd + FF2/4 ad, e in pulses, per ServoPeriod (the gains are the servo's
%       per-period gains; the controller scales the integral and the derivative to its StepTime),
%       within PWM Limit [885 = 100 %]. Each gain is one value for the three servos, as on the
%       servos, or [shoulder elbow wrist].
%     - PWM to torque: the motor's torque at that PWM, Kt (PWM/885 V - BackEMF Ke w) / R, with Kt,
%       R and Ke from Motor (armModel.motor: the datasheet's stall torque, stall current and no-load speed).
%       The servos' Current Limit then caps it (hardware block and plant alike).
%   Run at the servo's own period (StepTime = ServoPeriod = 1 ms) with BackEMF on, it gives the
%   servo's own position control (spot.arm.Plant's mode 1 before 2026-10-11, to within its
%   rounding). At the diagram's 20 Hz it can't: the servo's gains (400/0/200) and the back-EMF
%   term are far too stiff for a 50 ms step and the arm goes unstable. The defaults here are
%   retuned for 20 Hz, for the three joints together (they are coupled), over the arm's poses,
%   with the base fixed or floating, rotor inertia +-50 % and friction x0.3 to x2: about 1 N m/rad
%   of stiffness, a twentieth of the servo's own, so the I gain does the last few degrees against
%   the joints' stiction (slowly, within ~3 deg). In the diagram: armControl.position (Control pane).
%
%   Inputs: Goal, the goal joint angles [rad] (ARM_Path); Measured, [q1 q2 q3 w1 w2 w3] [rad,
%   rad/s] (ARM_Measured_States). Output: Torque [N m] for each joint (ARM_Controls).
%   The first step (and the first after the controller is switched back on, with the block's
%   Action Port set to reset its states) starts from where the arm is, as a servo whose torque
%   is switched on holds where it is.
%
%   Outside Simulink:
%     c = spot.arm.PositionControl('Motor', armModel.motor, 'StepTime', 0.05);
%     tau = c(goal, [q; w]);

    %#codegen

    properties (Nontunable)
        Motor = struct('stallTorque', [6 6 6], 'stallCurrent', [4.1 4.1 4.1], 'noLoadSpeed', [63 63 63] * 2*pi/60, ...
            'voltage', 12)                % armModel.motor: the servos' motor (spot.arm.defaults)
        StepTime = 0.05                  % [s] how often the controller runs (the diagram's baseRate)
        ServoPeriod = 1e-3               % [s] the servo's control period, which its gains are per
        PositionP = [25 56 21]           % Position P Gain [shoulder elbow wrist] (SPOT's servos had 400)
        PositionI = 10                   % Position I Gain (SPOT's servos had 0)
        PositionD = [1545 230 236]       % Position D Gain (SPOT's servos had 200)
        Feedforward1 = 0                 % Feedforward 1st Gain
        Feedforward2 = 0                 % Feedforward 2nd Gain
        ProfileVelocity = 20             % [0.229 rev/min], 0 = no profile
        ProfileAcceleration = 0          % [214.577 rev/min^2], 0 = none
        MinPosition = 1024               % Min Position Limit [pulses]
        MaxPosition = 3072               % Max Position Limit [pulses]
        PWMLimit = 885                   % PWM Limit
        BackEMF (1,1) logical = false    % include the motor's back-EMF in the PWM's torque
    end

    properties (SetAccess = private)
        PWM = zeros(3, 1)                % the last step's PWM, as the servo would set it
    end

    properties (Access = private)
        started = false
        goal = zeros(3, 1)               % goal position [pulses, relative to 2048]
        pd = zeros(3, 1)                 % profile: desired position [pulses] and velocity [pulses/s]
        vd = zeros(3, 1)
        ePrev = zeros(3, 1)              % position error last step [pulses]
        eInt = zeros(3, 1)               % position error integral [pulses * servo periods]
        Kt = zeros(3, 1)
        R = zeros(3, 1)
        Ke = zeros(3, 1)
        V = 12
    end

    properties (Constant, Access = private)
        PULSES = 4096 / (2*pi)           % encoder pulses per rad
        CMD_PULSES = 651.74              % pulses per rad of a goal (the old hardware code's conversion)
        VEL_UNIT = 0.229 * 2*pi/60       % [rad/s] per velocity unit
        ACC_UNIT = 214.577 * 2*pi/3600   % [rad/s^2] per profile acceleration unit
        PWM_FULL = 885
    end

    methods
        function obj = PositionControl(varargin)
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
            obj.started = false;
            obj.PWM = zeros(3, 1);
        end

        function tau = stepImpl(obj, goalRad, meas)
            h = obj.StepTime;
            pres = round(meas(1:3) * obj.PULSES);          % the servo sees its encoder's pulses
            w = meas(4:6);
            if ~obj.started                                % torque on: hold where it is
                obj.goal = pres(:);
                obj.pd = pres(:);
                obj.vd = zeros(3, 1);
                obj.ePrev = zeros(3, 1);
                obj.eInt = zeros(3, 1);
                obj.started = true;
            end
            for j = 1:3                                    % goals beyond a limit are ignored
                g = round(obj.CMD_PULSES * goalRad(j) + 2048);
                if g >= obj.MinPosition && g <= obj.MaxPosition
                    obj.goal(j) = g - 2048;
                end
            end
            Vp = obj.ProfileVelocity * obj.VEL_UNIT * obj.PULSES;      % [pulses/s], 0 = none
            Ap = obj.ProfileAcceleration * obj.ACC_UNIT * obj.PULSES;  % [pulses/s^2], 0 = none
            u = zeros(3, 1);
            for j = 1:3
                vdPrev = obj.vd(j);
                dist = obj.goal(j) - obj.pd(j);
                if Vp <= 0
                    obj.pd(j) = obj.goal(j);
                    obj.vd(j) = 0;
                else
                    vt = Vp;
                    if Ap > 0, vt = min(vt, sqrt(2 * Ap * abs(dist))); end
                    vt = sign(dist) * vt;
                    if Ap > 0
                        obj.vd(j) = obj.vd(j) + min(max(vt - obj.vd(j), -Ap * h), Ap * h);
                    else
                        obj.vd(j) = vt;
                    end
                    if abs(obj.vd(j) * h) >= abs(dist)
                        obj.pd(j) = obj.goal(j);
                        obj.vd(j) = 0;
                    else
                        obj.pd(j) = obj.pd(j) + obj.vd(j) * h;
                    end
                end
                ad = (obj.vd(j) - vdPrev) / h;
                e = obj.pd(j) - pres(j);
                de = (e - obj.ePrev(j)) * obj.ServoPeriod / h;
                obj.ePrev(j) = e;
                iNew = obj.eInt(j) + e * h / obj.ServoPeriod;
                uj = gain(obj.PositionP, j) / 128 * e + gain(obj.PositionI, j) / 65536 * iNew ...
                    + gain(obj.PositionD, j) / 16 * de ...
                    + gain(obj.Feedforward1, j) / 4 * obj.vd(j) / (obj.VEL_UNIT * obj.PULSES) ...
                    + gain(obj.Feedforward2, j) / 4 * ad / (obj.ACC_UNIT * obj.PULSES);
                if abs(uj) < obj.PWMLimit || sign(uj) ~= sign(e)   % anti-windup: hold the integral when saturated
                    obj.eInt(j) = iNew;
                end
                u(j) = min(max(uj, -obj.PWMLimit), obj.PWMLimit);
            end
            obj.PWM = u;
            tau = spot.arm.PositionControl.pwmTorque(u, w, obj.Kt, obj.R, obj.Ke, obj.V, obj.BackEMF);
        end

        % ------------------------------------------------------------ Simulink block interface
        function n = getNumInputsImpl(~), n = 2; end
        function n = getNumOutputsImpl(~), n = 1; end


        function a = getOutputSizeImpl(~), a = [3 1]; end
        function a = getOutputDataTypeImpl(~), a = 'double'; end
        function a = isOutputComplexImpl(~), a = false; end
        function a = isOutputFixedSizeImpl(~), a = true; end

    end

    methods (Static)
        function tau = pwmTorque(u, w, Kt, R, Ke, V, backEMF)
            % The motor's torque at the joint for PWM u [885 = 100 %] at joint speed w [rad/s]
            volts = u / 885 * V;
            if backEMF
                volts = volts - Ke .* w;
            end
            tau = Kt ./ R .* volts;
        end
    end

    methods (Static, Access = protected)

    end
end

function g = gain(value, j)
% A gain for joint j: one value for all three, or one per joint
g = value(min(j, numel(value)));
end
