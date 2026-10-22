function [duty] = allocateThrusters(controls, theta, thrusterDist, thrust, decayPerValve)
    %ALLOCATETHRUSTERS Duty cycles for the 8 thrusters that best produce a force command.
    %   duty = diagram.control.allocateThrusters(controls, theta, thrusterDist, thrust, decayPerValve)
    %
    %   Finds the duty cycles d, each between 0 and 1, that solve
    %
    %       minimize  ||H*d - target||^2 * FORCE_PRIORITY + ||d||^2
    %
    %   where target is the commanded force and torque in the body frame. The first
    %   term matches the command as closely as the thrusters allow. The second,
    %   much smaller term picks the solution that uses the least thrust when several
    %   would match it equally well, so opposing thrusters never fire together.
    %
    %   The problem is solved exactly with an active-set method (Harkegard, 2002,
    %   "Efficient Active Set Algorithms for Solving Constrained Least Squares
    %   Problems in Aircraft Control Allocation", LiTH-ISY-R-2426). Each iteration
    %   either frees one thruster from its limit or pins one to its limit, so it
    %   finishes in a handful of iterations.
    %
    %   Thrusters open at the same time share the air supply and each gives less thrust
    %   (diagram.control.thrustShare), which depends on the duty cycles being solved for.
    %   So, as in the v5 template, the solve is repeated with each thruster's thrust
    %   reduced by its share from the previous solve until the shares stop changing.
    %
    %   As in v5, H uses half of each thruster's thrust: a thruster at 100 % is assumed
    %   to give half its thrust on average. The applied force (thrusterForces) uses the
    %   full thrust, so the platform gets about twice the force commanded.
    %
    %   Inputs
    %     controls        Commanded force and torque in the inertial frame [N; N; N.m]
    %     theta           Spacecraft attitude [rad]
    %     thrusterDist    Moment arm of each thruster about the CG [mm]
    %     thrust          Thrust of each thruster [N]
    %     decayPerValve   Thrust lost per extra valve open (thrust_decay_<PLATFORM>)
    %
    %   Outputs
    %     duty            Duty cycle of each thruster, 0 to 1
    %
    %   See also diagram.control.thrusterMatrix, diagram.control.thrustShare.

    %#codegen

    arguments (Input)
            controls        (3,1) double
            theta           (1,1) double
            thrusterDist    (8,1) double
            thrust          (8,1) double {mustBePositive}
            decayPerValve   (1,1) double {mustBeNonnegative}
    end

    % Solver settings
    FORCE_PRIORITY  = 1e6;   % weight of matching the command vs. saving thrust
    MAX_ITERATIONS  = 50;    % a safety cap; each solve normally takes under 10
    MAX_DECAY_SOLVES = 20;   % re-solves for the thrust loss (v5: max_iters)
    DECAY_TOLERANCE = 1e-6;  % stop when no share changes more than this (v5: tol)

    % Rotate the commanded force into the body frame; torque is unchanged
    C_bI = [ cos(theta)  sin(theta);
            -sin(theta)  cos(theta)];
    target = [C_bI * controls(1:2); controls(3)];          % [N; N; N.m]

    % Solve, then re-solve with the thrust each thruster really gives with the others
    % open, until that stops changing (at once when decayPerValve is 0)
    share = ones(8, 1);
    duty  = zeros(8, 1);
    for solve = 1:MAX_DECAY_SOLVES
        H    = diagram.control.thrusterMatrix(thrusterDist, thrust .* share / 2);
        duty = solveDuty(H, target, FORCE_PRIORITY, MAX_ITERATIONS);

        newShare = diagram.control.thrustShare(duty, decayPerValve);
        if max(abs(newShare - share)) < DECAY_TOLERANCE
            break;
        end
        share = newShare;
    end
end

function duty = solveDuty(H, target, forcePriority, maxIterations)
    % Duty cycles in [0, 1] minimizing ||H*d - target||^2 * forcePriority + ||d||^2

    % Write both terms of the objective as one least-squares problem ||A*d - b||^2
    A = [sqrt(forcePriority) * H; eye(8)];
    b = [sqrt(forcePriority) * target; zeros(8, 1)];
    AtA = A.' * A;

    % Start with every thruster off and pinned at its lower limit
    %   bound: -1 pinned at 0, +1 pinned at 1, 0 free to move
    duty     = zeros(8, 1);
    bound    = -ones(8, 1);
    residual = b - A * duty;
    for iteration = 1:maxIterations

        % Best step for the free thrusters, with the pinned ones held still.
        % Pinned rows and columns are replaced by the identity, so their step is 0
        free = (bound == 0);
        M = AtA;
        M(~free, :) = 0;
        M(:, ~free) = 0;
        M = M + diag(double(~free));
        r = A.' * residual;
        r(~free) = 0;
        step = M \ r;
        candidate = duty + step;

        % Does the full step push any free thruster past its limits?
        violates = free & (candidate < 0 | candidate > 1);
        if ~any(violates)

            % No: take the full step
            duty     = candidate;
            residual = residual - A * step;

            % Would the fit improve by releasing a pinned thruster? A negative
            % multiplier means the limit is holding that thruster back

            multiplier = bound .* (A.' * residual);
            if all(multiplier >= -1e-10)
                break;              % no: this is the optimum
            end
            [~, release] = min(multiplier);
            bound(release) = 0;
        else

            % Yes: step only as far as the first limit reached, and pin that thruster
            fraction = inf(8, 1);
            up   = violates & step > 0;
            down = violates & step < 0;
            fraction(up)   = (1 - duty(up))  ./ step(up);
            fraction(down) = (0 - duty(down)) ./ step(down);
            [alpha, hit] = min(fraction);
            duty     = duty + alpha * step;
            residual = residual - alpha * (A * step);
            bound(hit) = sign(step(hit));
        end
    end

    % Remove any round-off just outside [0, 1]
    duty = min(max(duty, 0), 1);
end
