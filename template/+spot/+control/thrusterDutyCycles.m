function [duty, Fb, H] = thrusterDutyCycles(u, theta, thrusterDist, F, maxIters, tol, decay)
    %THRUSTERDUTYCYCLES The diagram's thruster mixer: 8 thrusters' duty cycles for a force command.
    %   [duty, Fb, H] = spot.control.thrusterDutyCycles(u, theta, thrusterDist, F) computes
    %   what the diagram's "Thruster & Float Control Code/Change <PLATFORM> Behavior" computes
    %   each step.
    %
    %   Inputs
    %     u             [Fx; Fy; Tz] in the table frame [N, N, N m] (*_Controls), e.g. from
    %                   spot.control.lqrCommand
    %     theta         measured attitude [rad] (*_Measured_States(3))
    %     thrusterDist  8x1 distance of each thruster's line of action from the CG [mm]
    %                   (thruster_dist2CG_<PLATFORM>, spot.massprops.centerOfGravity)
    %     F             8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>); one number:
    %                   all eight alike
    %     maxIters      mixer's Maximum Number of Iterations (default 20, as in the diagram)
    %     tol           mixer's Convergence Tolerance (default 1e-6, as in the diagram)
    %     decay         thrust lost per extra valve open at once: with n open, each gives
    %                   1 - decay*(n - 1) of its thrust alone (thrust_decay_<PLATFORM>);
    %                   default from spot.params.defaults
    %
    %   Outputs
    %     duty          8x1 duty cycles in [0, 1] (*_Duty_Cycle; x100 is the percentage sent to
    %                   the Teensy)
    %     Fb            command in the body frame [Fx_b; Fy_b; Tz]
    %     H             3x8 map from duty cycles to [Fx_b; Fy_b; Tz] including thrust decay
    %                   (the block's H_final; the simulated plant's forces are H * duty)
    %
    %   Steps (the diagram's blocks, in order):
    %     1. "Rotate Forces to Body": Fb = C_bI(theta) * [Fx; Fy] (RotateFrame)
    %     2. "Generate Initial Guess": x0 = pinv(MakeH(...)) * Fb
    %     3. "MATLAB Function5": least squares with 0 <= duty <= 1 (projected gradient),
    %        repeated while each thruster's thrust decay changes
    %        (optimize_duty_cycle_with_decay). All valves open at the start of each PWM period
    %        and close after their duty cycle, so how many are open at once follows from the
    %        duty cycles (check_thrust_decay).
    %   The local functions are verbatim copies of the RED blocks' code (BLACK's and BLUE's are
    %   the same). Only this first function is new.
    %
    %   The lab console's Interactive Mode runs this mixer in Python (duty_cycles in
    %   spot_console/control.py); spot.control.verifyConsoleController checks the copies, this
    %   function and the Python port against the diagram.
    %
    %   Example (the Chaser at 30 deg, pushed along the table's +x with 0.2 N)
    %     s = spot.params.defaults();
    %     mp = spot.massprops.compute(s.mass);
    %     duty = spot.control.thrusterDutyCycles([0.2; 0; 0], pi/6, mp.RED.thrusters, ...
    %                                            s.thrusterForce.RED)
    %
    %   See also spot.control.lqrCommand, spot.control.nominalForces.

    %% Defaults (the diagram's mask values)
    if nargin < 5
        maxIters = 20;
    end
    if nargin < 6
        tol = 1e-6;
    end
    if nargin < 7
        defaults = spot.params.defaults();
        decay = defaults.thrusterDecay.RED;
    end

    %% Inputs as columns
    u = u(:);
    thrusterDist = thrusterDist(:);
    F_thrusters = F(:) .* ones(8, 1);   % one number: all eight alike

    %% Mixer
    % 1. Rotate Forces to Body: C_bI * [Fx; Fy], with Tz unchanged.
    Fb = [RotateFrame(theta) * u(1:2); u(3)];

    % 2. Generate Initial Guess from the thrusts without decay.
    x0 = pinv(MakeH(thrusterDist, F_thrusters)) * Fb;

    % 3. MATLAB Function5: constrained fit with thrust decay.
    [H, duty] = optimize_duty_cycle_with_decay(Fb, x0, thrusterDist, F_thrusters, ...
        maxIters, tol, decay);
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
    u_desired, x0, thruster_dist2CG_RED, F_thrusters_RED, max_iters, tol, thrust_decay_RED)
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
    %       thrust_decay_RED    - Thrust lost per extra valve open at once: with n
    %                             open, each gives 1 - thrust_decay_RED*(n - 1) of
    %                             its thrust alone (thruster calibration, Together)
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
    thrust_decay_factor = ones(8, 1);            % initial decay (each thruster's share)
    prev_duty_cycles   = duty_cycles + 2*tol;    % force first iteration

    % ---------------------------
    % Iterative optimization
    % ---------------------------
    for iter = 1:max_iters
        % Build system matrix H with current decay factor
        H = MakeHWithDecay(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);

        % Solve constrained least squares (box constraints: 0 ≤ x ≤ 1)
        duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles);

        % Update each thruster's decay from the current duty cycles
        [thrust_decay_factor, ~] = check_thrust_decay(duty_cycles, thrust_decay_RED);

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
    %   applying each thruster's thrust decay (8x1, or one for all).
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
    F_decayed = F_thrusters(:) .* thrust_decay_factor(:);

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

    F_decayed = F_thrusters(:) .* thrust_decay_factor(:);

    Mat2 = diag(F_decayed);

    H = Mat1 * Mat2;
end


% ========================================================================
% Thrust Decay Update Rule
% ========================================================================
function [thrust_decay_factor, count] = check_thrust_decay(PWMs, decay_per_valve)
    %CHECK_THRUST_DECAY
    %   Each thruster's share of its thrust alone over one PWM period. The
    %   valves share one regulated supply: with n open at once, each gives
    %   max(0, 1 - decay_per_valve*(n - 1)) of its thrust alone (linear, as
    %   the thruster calibration's Together stage measures it). The Teensy
    %   opens every valve at the start of each PWM period and closes each
    %   after its duty cycle, so between two closings the number open is
    %   known, and each thruster's share is the average over its own opening.
    %
    %   Inputs:
    %       PWMs            - Duty cycle vector (8x1)
    %       decay_per_valve - Thrust lost per extra valve open (scalar)
    %
    %   Outputs:
    %       thrust_decay_factor - Each thruster's share of its thrust (8x1)
    %       count               - Number of active thrusters

    % Normalize if needed
    if max(PWMs) > 1
        PWMs = PWMs / max(PWMs);
    end
    duty = max(PWMs(:), 0);

    % Active thrusters
    count = sum(duty > 0);

    % The instants the valves close, in order
    closing = sort(duty);

    thrust_decay_factor = ones(8, 1);
    for i = 1:8
        if duty(i) > 0
            share = 0;
            t0 = 0;
            for k = 1:8
                t1 = min(closing(k), duty(i));
                if t1 > t0
                    n_open = sum(duty >= t1);   % valves open from t0 to t1
                    share = share + (t1 - t0) * max(0, 1 - decay_per_valve*(n_open - 1));
                    t0 = t1;
                end
            end
            thrust_decay_factor(i) = share / duty(i);
        end
    end
end
% ---- end verbatim
