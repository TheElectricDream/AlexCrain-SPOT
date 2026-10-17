classdef Plant < matlab.System
    %PLANT The Chaser (RED) with its arm and Dynamixel servos, simulated at the plant's own step.
    %   The diagram's "RED + ARM Plant" block. The servos are driven by PWM only (PWM control,
    %   as on the hardware: a share of the supply voltage across each motor); position and
    %   velocity control are controllers in the diagram (spot.arm.PositionControl,
    %   spot.arm.VelocityControl) whose output is PWM.
    %
    %   Each diagram step (baseRate), the commands are taken as spot.hardware.ArmPWM sends them
    %   (whole PWM units within the PWM Limit, none pushing a joint past Min/Max Position), and
    %   the plant takes baseRate/innerStep internal steps (ARM_PLANT_INNER_STEP by default) of:
    %     1. Motors: each gives torquePerVolt (V - backEMF w) at its joint, V = PWM / PWM_FULL x
    %        the supply voltage (spot.arm.defaults). The back-EMF part brakes the joint in
    %        proportion to its speed, and is taken implicitly with the viscous friction.
    %     2. Dynamics: base (x, y, theta; thruster forces, table slope and air-bearing drag) and
    %        the three joints, coupled (spot.arm.inertiaMatrix, spot.arm.coriolisMatrix), with
    %        each joint's reflected rotor inertia.
    %     3. Joint friction with stiction, solved implicitly at the end of the step so the
    %        joints stick and slip without chattering.
    %   Positions are then updated with the new velocities (semi-implicit Euler).
    %
    %   Parameters
    %     Model       armModel (spot.arm.parameters)
    %     FixedBase   base does not move (arm on a stand, e.g. to compare with lab data)
    %     FloatInput  third input, the float state (Float_State: 1 while the air bearings are
    %                 on); the table's pull acts only then. Without it, it always acts.
    %
    %   Inputs
    %     F           3x1 thruster forces on the base [N, N, N m] (F_RED)
    %     pwm         3x1 joint commands [PWM, 885 = 100 %] (ARM_Controls)
    %     floatState  air bearings on (with FloatInput)
    %
    %   Outputs
    %     pos         6x1 [x y theta q1 q2 q3]
    %     vel         6x1 velocities
    %     applied     3x1 PWM each servo applies (its Present PWM): the command within the
    %                 limits
    %
    %   Example
    %     p = spot.arm.Plant('Model', armModel);
    %     [pos, vel, applied] = p(F_RED, pwm);   % one diagram step
    %   p.MotorTorque, p.FrictionTorque, p.Voltage and p.Stuck describe the last internal step.
    %
    %   See also spot.arm.parameters, spot.hardware.ArmPWM, spot.arm.withinLimits.

    %#codegen

    properties (Nontunable)
        Model = struct()                    % armModel (spot.arm.parameters)
        FixedBase (1,1) logical = false     % base does not move
        FloatInput (1,1) logical = false    % third input: the float state (Float_State)
    end

    properties (SetAccess = private)
        MotorTorque = zeros(3, 1)           % [N m] at each joint (back-EMF included)
        FrictionTorque = zeros(3, 1)        % [N m] at each joint (dry + viscous), signed
        Voltage = zeros(3, 1)               % [V] across each motor
        Stuck = false(3, 1)                 % joint held by its friction
    end

    properties (Access = private)
        q = zeros(6, 1)                     % [x y theta q1 q2 q3]
        v = zeros(6, 1)                     % velocities
        volts = zeros(3, 1)                 % [V] across each motor this diagram step
        nSub = 1                            % internal steps per diagram step
        h = Constants.ARM_PLANT_INNER_STEP   % [s] internal step
        g = zeros(3, 1)                     % [N m/V] torque per volt at the joint
        Ke = zeros(3, 1)                    % [V s/rad] back-EMF at the joint
        floating = 1                        % air bearings on (the table's pull acts)
    end

    properties (Constant, Access = private)
        PWM_FULL = Constants.PWM_FULL                  % PWM at 100 % of the supply
        PULSES_PER_REV = Constants.PULSES_PER_REV      % [pulses/rev] encoder resolution
        CENTER_PULSE = Constants.CENTER_PULSE          % [pulses] joint straight out
    end

    methods
        function obj = Plant(varargin)
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            m = obj.Model;

            % Internal step: a whole number of steps per diagram step.
            obj.nSub = max(1, round(m.outerStep / m.innerStep));
            obj.h = m.outerStep / obj.nSub;

            % Motor constants at the joints.
            obj.g = m.motor.torquePerVolt(:);
            obj.Ke = m.motor.backEMF(:);
        end

        function resetImpl(obj)
            obj.q = obj.Model.x0(:);
            obj.v = zeros(6, 1);
            obj.volts = zeros(3, 1);
            obj.MotorTorque = zeros(3, 1);
            obj.FrictionTorque = zeros(3, 1);
            obj.Voltage = zeros(3, 1);
            obj.Stuck = false(3, 1);
            obj.floating = 1;
        end

        function [pos, vel, applied] = stepImpl(obj, F, pwm, varargin)
            if obj.FloatInput
                obj.floating = double(varargin{1}(1) ~= 0);
            end

            % Goal PWM, as spot.hardware.ArmPWM sends it: none pushing a joint past its position
            % limits, in whole units within the PWM Limit.
            servo = obj.Model.servo;
            pulses = round(obj.q(4:6) * obj.PULSES_PER_REV / (2*pi)) + obj.CENTER_PULSE;
            pwm = spot.arm.withinLimits(pwm(:), pulses, servo.minPosition, servo.maxPosition);
            lim = servo.pwmLimit;
            applied = min(max(round(pwm), -lim), lim);
            obj.volts = applied / obj.PWM_FULL * obj.Model.motor.voltage;

            % Internal steps.
            for k = 1:obj.nSub
                innerStep(obj, F(:));
            end

            % Outputs.
            pos = obj.q;
            vel = obj.v;
        end

        %% Simulink block interface
        function n = getNumInputsImpl(obj)
            n = 2 + double(obj.FloatInput);
        end

        function n = getNumOutputsImpl(~)
            n = 3;
        end

        function [a, b, c] = getOutputSizeImpl(~)
            a = [6 1];
            b = [6 1];
            c = [3 1];
        end

        function [a, b, c] = getOutputDataTypeImpl(~)
            a = 'double';
            b = 'double';
            c = 'double';
        end

        function [a, b, c] = isOutputComplexImpl(~)
            a = false;
            b = false;
            c = false;
        end

        function [a, b, c] = isOutputFixedSizeImpl(~)
            a = true;
            b = true;
            c = true;
        end
    end

    methods (Static, Access = protected)
    end

    methods (Access = private)
        function innerStep(obj, F)
            % One internal step: motors, dynamics and friction.
            STUCK_SPEED = 1e-9;             % [rad/s] below this a joint counts as still
            BOUND_REL_TOL = 1e-9;           % friction strictly inside its bound: stuck

            m = obj.Model;
            h = obj.h;
            fr = m.friction;
            q = obj.q;
            v = obj.v;

            % Each motor's torque at rest for its voltage; its back-EMF brakes the joint in
            % proportion to its speed, taken implicitly with the viscous friction below.
            jointVel = v(4:6);
            tau0 = obj.g .* obj.volts;
            backEMF = obj.g .* obj.Ke;              % [N m s/rad]

            % Mass matrix (with rotor inertia) and Coriolis torques. The shoulder angle is
            % measured from its face; the model's from the base frame.
            x = [q; v];
            x(4) = x(4) + m.q1Offset;
            M = spot.arm.inertiaMatrix(m, x);
            M(4:6, 4:6) = M(4:6, 4:6) + diag(m.armature(:));
            Cv = spot.arm.coriolisMatrix(m, x) * v;

            % Friction: viscous (and the back-EMF) taken implicitly; dry friction bounded by the
            % Stribeck curve at this speed.
            b = fr.viscous(:) + backEMF;
            Ts = fr.static(:);
            Tc = fr.coulomb(:);
            dryLimit = Tc + (Ts - Tc) .* exp(-(jointVel ./ fr.stribeck(:)).^2);

            % Velocities at the end of the step, with friction impulses at the joints.
            if obj.FixedBase
                A = M(4:6, 4:6) + h * diag(b);
                rhs = M(4:6, 4:6) * jointVel + h * (tau0 - Cv(4:6));
                [vj, impulse] = obj.dryFriction(A, rhs, [1 2 3], h * dryLimit);
                vNew = [0; 0; 0; vj];
            else
                A = M + h * diag([0; 0; 0; b]);

                % Table's pull on the whole platform (M(1,1): base and arm links).
                Fs = obj.floating * M(1, 1) ...
                   * spot.sim.tableSlope(q(1:2), v(1:2), m.tableSlope, m.tableDamping);
                rhs = M * v + h * ([F + [Fs; 0]; tau0] - Cv);
                [vNew, impulse] = obj.dryFriction(A, rhs, [4 5 6], h * dryLimit);
            end

            % Semi-implicit Euler, and the step's diagnostics.
            obj.v = vNew;
            obj.q = q + h * vNew;
            obj.Voltage = obj.volts;
            obj.MotorTorque = tau0 - backEMF .* vNew(4:6);
            obj.FrictionTorque = impulse / h - fr.viscous(:) .* vNew(4:6);
            obj.Stuck = abs(vNew(4:6)) < STUCK_SPEED ...
                      & abs(impulse) < h * dryLimit * (1 - BOUND_REL_TOL);
        end
    end

    methods (Static, Access = private)
        function [vNew, impulse] = dryFriction(A, rhs, J, bound)
            % Velocities at the end of the step: A vNew = rhs + friction impulses at joints J,
            % each |impulse(j)| <= bound(j). An impulse stops its joint if it can, else it sits
            % at its bound against the motion. Solved by projected Gauss-Seidel.
            MAX_ITERATIONS = 60;
            TOLERANCE = 1e-14;              % relative to the largest bound

            % Free velocities, and the joints' response to impulses.
            Ainv = A \ eye(size(A, 1));
            vFree = Ainv * rhs;
            W = Ainv(J, J);

            % Projected Gauss-Seidel on the impulses.
            impulse = zeros(3, 1);
            for it = 1:MAX_ITERATIONS
                change = 0;
                for j = 1:3
                    vj = vFree(J(j)) + W(j, :) * impulse;
                    pj = min(max(impulse(j) - vj / W(j, j), -bound(j)), bound(j));
                    change = max(change, abs(pj - impulse(j)));
                    impulse(j) = pj;
                end
                if change <= TOLERANCE * max(1, max(abs(bound)))
                    break
                end
            end

            vNew = vFree + Ainv(:, J) * impulse;
        end
    end
end
