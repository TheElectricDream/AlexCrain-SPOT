function [duty, Fb, H] = thrusterDutyCycles(u, theta, thrusterDist, F, maxIters, tol)
%THRUSTERDUTYCYCLES The diagram's thruster mixer: the 8 thrusters' duty cycles for a force command.
%   [duty, Fb, H] = spot.control.thrusterDutyCycles(u, theta, thrusterDist, F) computes
%   what the diagram's "Thruster & Float Control Code/Change <PLATFORM> Behavior" computes each
%   step:
%     u             [Fx; Fy; Tz] in the table frame [N, N, N m]: *_Controls, from the controller
%                   (spot.control.lqrCommand for the default LQR)
%     theta         the platform's measured attitude [rad]: *_Measured_States(3)
%     thrusterDist  8x1 distance of each thruster's line of action from the CG [mm]:
%                   thruster_dist2CG_<PLATFORM> (spot.massprops.centerOfGravity)
%     F             8x1 thrust of each thruster [N]: F_thrusters_<PLATFORM> (the calibrated thrust,
%                   Calibration tab, or spot.control.nominalForces); one number: all eight alike
%     maxIters, tol the mixer block's mask values (Maximum Number of Iterations, Convergence
%                   Tolerance); 20 and 1e-6 as in the diagram if left out
%   Returns
%     duty          8x1 duty cycles in [0, 1] (*_Duty_Cycle; times 100 is the percentage sent
%                   to the Teensy)
%     Fb            the command in the body frame [Fx_b; Fy_b; Tz]
%     H             3x8 map from duty cycles to [Fx_b; Fy_b; Tz] with the thrust decay of these
%                   duty cycles (the block's H_final)
%
%   The steps are the diagram's blocks, in its order:
%     1. "Rotate Forces to Body": Fb = C_bI(theta) * [Fx; Fy] (RotateFrame below)
%     2. "Calculate Duty Cycle/Generate Initial Guess": x0 = pinv(MakeH(...)) * Fb, with MakeH
%        below and the DSP Pseudoinverse block (pinv)
%     3. "Calculate Duty Cycle/MATLAB Function5": optimize_duty_cycle_with_decay below, a
%        least-squares fit with 0 <= duty <= 1 (projected gradient), repeated while the thrust
%        decay factor (fewer newtons when many valves are open) changes
%   The functions below are verbatim copies of those blocks' code (the RED blocks; BLACK's and
%   BLUE's are the same code). Only this first function, which wires them, is new.
%   (The block's other outputs, the saturated forces for the simulated plant, are not needed.)
%
%   The lab console's Interactive Mode runs this mixer in Python (duty_cycles in
%   +spot/+console/webapp/spot_console/control.py). spot.control.verifyConsoleController checks
%   that the copies below are still the diagram's code, that this function reproduces the duty
%   cycles the diagram logs in a simulation, and that the Python port agrees with it.
%
%   Example (the Chaser at 30 degrees, pushed along the table's +x with 0.2 N):
%     s = spot.params.defaults();  mp = spot.massprops.compute(s.mass);
%     duty = spot.control.thrusterDutyCycles([0.2; 0; 0], pi/6, mp.RED.thrusters, s.thrusterForce.RED)
%
%   See also spot.control.lqrCommand, spot.control.nominalForces.
if nargin < 5
    maxIters = 20;
end
if nargin < 6
    tol = 1e-6;
end
u = u(:);
thrusterDist = thrusterDist(:);
F_thrusters = F(:) .* ones(8, 1);   % (one number: all eight alike)

% 1. Rotate Forces to Body: Demux [2,1] of *_Controls, C_bI * F_I, Mux with Tz
Fb = [RotateFrame(theta) * u(1:2); u(3)];

% 2. Generate Initial Guess: MakeH gets thruster_dist2CG_<PLATFORM> and F_thrusters_<PLATFORM>
x0 = pinv(MakeH(thrusterDist, F_thrusters)) * Fb;

% 3. MATLAB Function5: the same thrusts
[H, duty] = optimize_duty_cycle_with_decay(Fb, x0, thrusterDist, F_thrusters, maxIters, tol);
end

% ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Rotate Forces to Body/Create Rotation Matrix
function C_bI = RotateFrame(Rz)

C_bI = [  cos(Rz) sin(Rz)
         -sin(Rz) cos(Rz) ];

end
% ---- end verbatim

% ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Calculate Duty Cycle/Generate Initial Guess/MATLAB Function
function H = MakeH(thruster_dist2CG_RED,F_thrusters_RED)

    Vec1 = [ -1
             -1
              0 
              0
              1
              1
              0
              0 ];
          
    Vec2 = [  0
              0
              1
              1
              0
              0
             -1
             -1 ];
         
    Vec3 = thruster_dist2CG_RED./1000;
    
    Mat1 = [Vec1, Vec2, Vec3]';
    
    Mat2 = diag((F_thrusters_RED./2));
    
    H    = Mat1*Mat2;

end
% ---- end verbatim

% ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Calculate Duty Cycle/MATLAB Function5
function [H_final, duty_cycles] = optimize_duty_cycle_with_decay( ...
    u_desired, x0, thruster_dist2CG_RED, F_thrusters_RED, max_iters, tol)
    %OPTIMIZE_DUTY_CYCLE_WITH_DECAY 
    %   Iteratively optimizes thruster duty cycles while updating thrust decay.
    %
    %   Inputs:
    %       u_desired           - Desired force/torque vector [Fx; Fy; M] (3x1)
    %       x0                  - Initial guess for duty cycles (8x1)
    %       thruster_dist2CG_RED- Distances of thrusters from CG (vector, mm)
    %       F_thrusters_RED     - Thrust of each thruster (8x1, N): calibrated
    %                             (Calibration tab) or nominal (all eight alike)
    %       max_iters           - Maximum optimization iterations
    %       tol                 - Convergence tolerance (on duty cycles)
    %
    %   Outputs:
    %       H_final     - Final H-matrix including thrust decay (3x8)
    %       duty_cycles - Optimized duty cycles (8x1)
    %

    % ---------------------------
    % Initialization
    % ---------------------------
    num_thrusters      = length(x0); %#ok<NASGU> % reserved if needed later
    duty_cycles        = x0;                     % current solution
    thrust_decay_factor = 1;                     % initial decay
    prev_duty_cycles   = duty_cycles + 2*tol;    % force first iteration

    % ---------------------------
    % Iterative optimization
    % ---------------------------
    for iter = 1:max_iters
        % Build system matrix H with current decay factor
        H = MakeHWithDecay(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);

        % Solve constrained least squares (box constraints: 0 ≤ x ≤ 1)
        duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles);

        % Update decay factor based on the current duty cycles
        [thrust_decay_factor, ~] = check_thrust_decay(duty_cycles);

        % Check convergence (∞-norm difference)
        if norm(duty_cycles - prev_duty_cycles, Inf) < tol
            H_final = MakeHWithDecayRecalculateForce(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);
            return;
        end

        % Update reference for next iteration
        prev_duty_cycles = duty_cycles;
    end

    % If not converged within max_iters, return final H with last decay
    H_final = MakeHWithDecayRecalculateForce(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);
end


% ========================================================================
% Custom Constrained Least Squares Solver
% ========================================================================
function x_opt = custom_constrained_least_squares(H, b, x0)
    %CUSTOM_CONSTRAINED_LEAST_SQUARES
    %   Solves min_x ||H*x - b||^2 subject to 0 <= x <= 1
    %   using projected gradient descent with backtracking line search.
    %
    %   Inputs:
    %       H   - System matrix (3x8)
    %       b   - Desired vector (3x1)
    %       x0  - Initial guess (8x1)
    %
    %   Output:
    %       x_opt - Optimal duty cycle vector (8x1)

    % Parameters
    max_iterations = 100;
    tolerance      = 1e-6;
    alpha          = 0.01;  % initial step size
    beta           = 0.5;   % backtracking factor

    % Initialization
    x      = x0;
    x_prev = x0 + 2*tolerance;

    % Precompute matrices
    HtH = H' * H;
    Htb = H' * b;

    for iter = 1:max_iterations
        % Gradient of ||H*x - b||^2
        gradient = 2 * (HtH*x - Htb);

        % Store objective for comparison
        current_obj = norm(H*x - b)^2;

        % Backtracking line search
        step_size = alpha;
        for ls_iter = 1:20
            x_new = x - step_size * gradient;
            x_new = max(0, min(1, x_new)); % project into [0,1]

            new_obj = norm(H*x_new - b)^2;
            if new_obj < current_obj
                break; % accept step
            else
                step_size = step_size * beta;
            end
        end

        % Update
        x = x_new;

        % Convergence check
        if norm(x - x_prev) < tolerance
            break;
        end

        % Adaptive step size tuning
        if iter > 1 && new_obj < current_obj
            alpha = min(alpha * 1.1, 0.1); % speed up if progress
        else
            alpha = alpha * 0.9;           % slow down otherwise
        end

        % Store for next iteration
        x_prev = x;
    end

    x_opt = x;
end


% ========================================================================
% Construct H-matrix (with decay factor)
% ========================================================================
function H = MakeHWithDecay(thruster_dist2CG, F_thrusters, thrust_decay_factor)
    %MAKEHWITHDECAY
    %   Builds the H matrix that maps thruster duty cycles to [Fx; Fy; M],
    %   applying thrust decay.
    %
    %   Outputs H as (3x8)

    % Convert thruster arm distances to meters
    adjusted_dist = thruster_dist2CG / 1000;

    % Thruster directions (X, Y, and moment arm)
    Vec1 = [-1; -1;  0;  0;  1;  1;  0;  0]; % X
    Vec2 = [ 0;  0;  1;  1;  0;  0; -1; -1]; % Y
    Vec3 = adjusted_dist;                    % moment (Z)

    Mat1 = [Vec1, Vec2, Vec3]';

    % Thruster forces (scaled by decay)
    F_decayed = F_thrusters(:) * thrust_decay_factor;

    % Build diagonal scaling
    Mat2 = diag(F_decayed / 2);

    % Final mapping
    H = Mat1 * Mat2;
end


% ========================================================================
% Construct H-matrix (recalculate full force with decay)
% ========================================================================
function H = MakeHWithDecayRecalculateForce(thruster_dist2CG, F_thrusters, thrust_decay_factor)
    %MAKEHWITHDECAYRECALCULATEFORCE
    %   Same as MakeHWithDecay but applies full nominal force scaling.

    adjusted_dist = thruster_dist2CG / 1000;

    Vec1 = [-1; -1;  0;  0;  1;  1;  0;  0];
    Vec2 = [ 0;  0;  1;  1;  0;  0; -1; -1];
    Vec3 = adjusted_dist;

    Mat1 = [Vec1, Vec2, Vec3]';

    F_decayed = F_thrusters(:) * thrust_decay_factor;

    Mat2 = diag(F_decayed);

    H = Mat1 * Mat2;
end


% ========================================================================
% Thrust Decay Update Rule
% ========================================================================
function [thrust_decay_factor, count] = check_thrust_decay(PWMs)
    %CHECK_THRUST_DECAY
    %   Updates thrust decay factor based on average duty cycle.
    %
    %   Inputs:
    %       PWMs - Duty cycle vector (8x1)
    %
    %   Outputs:
    %       thrust_decay_factor - Adjusted decay factor (scalar)
    %       count               - Number of active thrusters

    % Normalize if needed
    if max(PWMs) > 1
        PWMs = PWMs / max(PWMs);
    end
    
    % Active thrusters
    count = sum(PWMs > 0);
    
    % Mean duty cycle (only active thrusters)
    avg_duty = sum(max(PWMs, 0)) / max(count, 1);
    
    % Update rule (piecewise)
    if avg_duty < 0.3 || count == 0
        thrust_decay_factor = 1;
    else
        thrust_decay_factor = max(0.6 - 2*avg_duty + 1, 0.5);
    end
end
% ---- end verbatim
