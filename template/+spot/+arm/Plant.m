classdef Plant < matlab.System
%PLANT The Chaser (RED) with its arm and Dynamixel servos, simulated at the plant's own step.
%   The diagram's "RED + ARM Plant" block. The servos are driven by torque only, as the hardware
%   is (spot.hardware.ArmTorque: each servo in current control mode); position and velocity
%   control are controllers in the diagram (spot.arm.PositionControl, spot.arm.VelocityControl),
%   not modes of the plant. Each time the diagram steps (baseRate), the plant takes
%   baseRate/innerStep internal steps (1 ms by default) of:
%     - the servos in current control (operating mode 0): the joint torques the diagram sends
%       (ARM_Controls, [N m]) become goal currents as the hardware block converts them (torque /
%       torque constant, in 3.36 mA units, within the Current Limit; none that pushes a joint
%       further beyond its Min/Max Position, armModel.servo); each servo holds its current
%       as far as the supply voltage allows (the motor's back-EMF), and the motor's torque goes
%       through the gearbox (stall torque, stall current and no-load speed from the datasheet)
%     - the dynamics of the base (x, y, theta; thruster forces F_RED, and the table's slope and
%       the air bearings' drag, spot.sim.tableSlope, on the whole platform's mass) and the three
%       joints, coupled (spot.arm.inertiaMatrix, spot.arm.coriolisMatrix), with each joint's
%       reflected rotor inertia
%     - joint friction with stiction: each step solves for the friction torques that keep a
%       joint at rest if that takes less than its breakaway torque, and otherwise applies the
%       sliding friction (Coulomb + Stribeck + viscous) against the motion, all at the end of the
%       step (implicit), so the joints stick and slip without chattering
%   Positions are then updated with the new velocities (semi-implicit Euler). The third output is
%   the torque each servo reports (its Present Current, in 3.36 mA units, times the torque
%   constant), as the hardware block's.
%
%   Parameters: Model, the workspace variable armModel (spot.arm.parameters). With FixedBase the
%   base doesn't move (the arm on a fixed stand, e.g. to compare with lab calibration data). With
%   FloatInput the block has a third input, the float state (Float_State: 1 while the air bearings
%   are on), and the table's pull acts only then (a platform resting on its pucks doesn't slide);
%   without it, the table's pull always acts.
%
%   Outside Simulink:
%     p = spot.arm.Plant('Model', armModel);
%     [pos, vel, torque] = p(F_RED, jointTorques);   % one diagram step
%     (with FloatInput: p(F_RED, jointTorques, floatState))
%   p.MotorTorque, p.FrictionTorque, p.Current and p.Stuck describe the last internal step.

    %#codegen

    properties (Nontunable)
        Model = struct()          % armModel (spot.arm.parameters)
        FixedBase (1,1) logical = false
        FloatInput (1,1) logical = false   % a third input: the float state (the diagram's Float_State)
    end

    properties (SetAccess = private)
        MotorTorque = zeros(3, 1)     % [N m] at each joint
        FrictionTorque = zeros(3, 1)  % [N m] at each joint (dry + viscous), signed
        Current = zeros(3, 1)         % [A] each motor's current
        Stuck = false(3, 1)           % the joint is held by its friction
    end

    properties (Access = private)
        q = zeros(6, 1)               % [x y theta q1 q2 q3]
        v = zeros(6, 1)
        iGoal = zeros(3, 1)           % goal current [A]
        nSub = 1
        h = 1e-3
        Kt = zeros(3, 1)              % [N m/A] at the joint
        R = zeros(3, 1)               % [ohm]
        Ke = zeros(3, 1)              % [V s/rad] at the joint
        floating = 1                  % the air bearings are on (the table's pull acts)
    end

    properties (Constant, Access = private)
        CUR_UNIT = 3.36e-3            % [A] per current unit
    end

    methods
        function obj = Plant(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            m = obj.Model;
            obj.nSub = max(1, round(m.outerStep / m.innerStep));
            obj.h = m.outerStep / obj.nSub;
            mo = m.motor;
            obj.Kt = mo.stallTorque(:) ./ mo.stallCurrent(:);
            obj.R = mo.voltage ./ mo.stallCurrent(:);
            obj.Ke = mo.voltage ./ mo.noLoadSpeed(:);
        end

        function resetImpl(obj)
            obj.q = obj.Model.x0(:);
            obj.v = zeros(6, 1);
            obj.iGoal = zeros(3, 1);
            obj.MotorTorque = zeros(3, 1);
            obj.FrictionTorque = zeros(3, 1);
            obj.Current = zeros(3, 1);
            obj.Stuck = false(3, 1);
            obj.floating = 1;
        end

        function [pos, vel, torque] = stepImpl(obj, F, tau, varargin)
            if obj.FloatInput
                obj.floating = double(varargin{1}(1) ~= 0);
            end
            % The goal currents, as spot.hardware.ArmTorque sends them: no torque pushing a joint
            % further beyond its position limits, within the Current Limit
            s = obj.Model.servo;
            pulses = round(obj.q(4:6) * 4096 / (2*pi)) + 2048;
            tau = spot.arm.withinLimits(tau(:), pulses, s.minPosition, s.maxPosition);
            lim = s.currentLimit;
            units = min(max(round(tau ./ obj.Kt / obj.CUR_UNIT), -lim), lim);
            obj.iGoal = units * obj.CUR_UNIT;
            for k = 1:obj.nSub
                innerStep(obj, F(:));
            end
            pos = obj.q;
            vel = obj.v;
            torque = round(obj.Current / obj.CUR_UNIT) * obj.CUR_UNIT .* obj.Kt;
        end

        % ------------------------------------------------------------ Simulink block interface
        function n = getNumInputsImpl(obj), n = 2 + double(obj.FloatInput); end
        function n = getNumOutputsImpl(~), n = 3; end


        function [a, b, c] = getOutputSizeImpl(~), a = [6 1]; b = [6 1]; c = [3 1]; end
        function [a, b, c] = getOutputDataTypeImpl(~), a = 'double'; b = 'double'; c = 'double'; end
        function [a, b, c] = isOutputComplexImpl(~), a = false; b = false; c = false; end
        function [a, b, c] = isOutputFixedSizeImpl(~), a = true; b = true; c = true; end

    end

    methods (Static, Access = protected)

    end

    methods (Access = private)
        function innerStep(obj, F)
            m = obj.Model;
            h = obj.h;
            fr = m.friction;
            q = obj.q;
            v = obj.v;
            % Each servo holds its goal current, as far as the supply voltage allows
            V = m.motor.voltage;
            w = v(4:6);
            i = min(max(obj.iGoal, (-V - obj.Ke .* w) ./ obj.R), (V - obj.Ke .* w) ./ obj.R);
            tau0 = obj.Kt .* i;
            x = [q; v];
            x(4) = x(4) + m.q1Offset;                       % the shoulder's angle from its face -> the model's
            M = spot.arm.inertiaMatrix(m, x);
            M(4:6, 4:6) = M(4:6, 4:6) + diag(m.armature(:));
            Cv = spot.arm.coriolisMatrix(m, x) * v;
            b = fr.viscous(:);                              % velocity-proportional torques, taken implicitly
            Ts = fr.static(:); Tc = fr.coulomb(:);
            mu = Tc + (Ts - Tc) .* exp(-(w ./ fr.stribeck(:)).^2);   % dry friction bound at this speed
            if obj.FixedBase
                A = M(4:6, 4:6) + h * diag(b);
                rhs = M(4:6, 4:6) * w + h * (tau0 - Cv(4:6));
                [vj, p] = obj.dryFriction(A, rhs, [1 2 3], h * mu);
                vNew = [0; 0; 0; vj];
            else
                A = M + h * diag([0; 0; 0; b]);
                % the table's pull, on the whole platform (M(1,1): the base and the arm's links)
                Fs = obj.floating * M(1, 1) * spot.sim.tableSlope(q(1:2), v(1:2), m.tableSlope, m.tableDamping);
                rhs = M * v + h * ([F + [Fs; 0]; tau0] - Cv);
                [vNew, p] = obj.dryFriction(A, rhs, [4 5 6], h * mu);
            end
            obj.v = vNew;
            obj.q = q + h * vNew;
            obj.Current = i;
            obj.MotorTorque = tau0;
            obj.FrictionTorque = p / h - fr.viscous(:) .* vNew(4:6);
            obj.Stuck = abs(vNew(4:6)) < 1e-9 & abs(p) < h * mu * (1 - 1e-9);
        end
    end

    methods (Static, Access = private)
        function [vNew, p] = dryFriction(A, rhs, J, bound)
            % Velocities at the end of the step, A vNew = rhs + (friction impulses p at joints J),
            % with each |p(j)| <= bound(j): p stops joint j if it can, else it is at its bound
            % against the motion (a box-constrained problem, solved by projected Gauss-Seidel)
            Ainv = A \ eye(size(A, 1));
            vFree = Ainv * rhs;
            W = Ainv(J, J);
            p = zeros(3, 1);
            for it = 1:60
                change = 0;
                for j = 1:3
                    vj = vFree(J(j)) + W(j, :) * p;
                    pj = min(max(p(j) - vj / W(j, j), -bound(j)), bound(j));
                    change = max(change, abs(pj - p(j)));
                    p(j) = pj;
                end
                if change <= 1e-14 * max(1, max(abs(bound)))
                    break
                end
            end
            vNew = vFree + Ainv(:, J) * p;
        end
    end
end
