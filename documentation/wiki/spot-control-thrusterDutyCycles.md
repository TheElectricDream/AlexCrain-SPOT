# spot.control.thrusterDutyCycles

**File:** `+spot/+control/thrusterDutyCycles.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/thrusterDutyCycles.m -->

## What it is for

The diagram's thruster mixer as a function: what *Thruster & Float Control Code / Change PLATFORM Behavior* computes each step, from the controller's force command to the eight valves' duty cycles. The first function (lines 43–61) is new and only wires things together; everything after `% ---- verbatim` is the diagram's own block code, copied unchanged ([[verifyConsoleController|spot-control-verifyConsoleController]] checks that). Maths: [[The Thruster Mixer|Math-Thruster-Mixer]].

## How to call it

```matlab
[duty, Fb, H] = spot.control.thrusterDutyCycles(u, theta, thrusterDist, F)
[duty, Fb, H] = spot.control.thrusterDutyCycles(u, theta, thrusterDist, F, maxIters, tol)
%  u             [Fx; Fy; Tz] table frame (*_Controls)
%  theta         measured attitude [rad]
%  thrusterDist  8x1 [mm] (thruster_dist2CG_<PLATFORM>)
%  F             8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>), or one number for all eight
%  duty          8x1 in [0, 1];  Fb body-frame command;  H the 3x8 map with the final decay
```

Example: the Chaser at 30°, pushed along the table's +x with 0.2 N:

```matlab
s = spot.params.defaults(); mp = spot.massprops.compute(s.mass);
duty = spot.control.thrusterDutyCycles([0.2; 0; 0], pi/6, mp.RED.thrusters, s.thrusterForce.RED)
```

## Line by line

**Lines 43–51.** Defaults for the mask values (20 iterations, 1e-6), inputs as columns, and `F` as eight thrusts (one number is repeated for all eight).

**Line 54.** Step 1, *Rotate Forces to Body*: the x, y part of `u` through the rotation matrix, the torque unchanged.

**Line 57.** Step 2, *Generate Initial Guess*: the H matrix from `MakeH` with the eight thrusts, and the pseudo-inverse times the body command.

**Line 60.** Step 3, *MATLAB Function5*: the constrained optimisation with thrust decay, with the same thrusts; returns the final H and the duty cycles.

**Lines 64–69.** `RotateFrame`: $C_{bI}(\theta)$.

**Lines 73–101.** `MakeH`: the direction columns (x for 1, 2, 5, 6 with signs −, −, +, +; y for 3, 4, 7, 8 with +, +, −, −), the lever arms in metres, and a diagonal of half of each thruster's thrust; H is 3×8.

**Lines 105–157.** `optimize_duty_cycle_with_decay`: takes the eight thrusts `F_thrusters_RED` (as `MakeH` does); start from `x0` with decay 1 and a previous solution far enough to force a first pass; each outer iteration builds H with the current decay, solves the box-constrained least squares, recomputes the decay from the new duty cycles, and stops when the duty cycles moved less than `tol` (∞-norm), returning `H_final` built with full thrusts and the last decay. If `max_iters` passes without converging, the last values are returned.

**Lines 163–231.** `custom_constrained_least_squares`: projected gradient descent on $\|Hx - b\|^2$. The gradient is $2(H^TH x - H^Tb)$ (precomputed products). Each iteration tries a step, projects into [0, 1], and halves the step up to 20 times until the objective drops; the step then adapts (×1.1 up to 0.1 after progress, ×0.9 otherwise); stop when $x$ moves less than 1e-6, or after 100 iterations.

**Lines 237–262.** `MakeHWithDecay`: as `MakeH` but each thruster's thrust is scaled by the decay factor (`F_decayed`, still halved).

**Lines 268–285.** `MakeHWithDecayRecalculateForce`: the same with the full thrusts (no halving), for `H_final`.

**Lines 291–319.** `check_thrust_decay`: normalise if anything exceeds 1, count the open valves, average their duty cycles; below 0.3 (or none open) the decay is 1, otherwise $\max(0.6 - 2\bar d + 1,\ 0.5)$.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+console/webapp/static/interactive.js`, `+spot/+console/webapp/static/viewer.js`, `+spot/+control/lqrCommand.m`, `+spot/+control/verifyConsoleController.m`
- **Uses:** [[spot.control.lqrCommand|spot-control-lqrCommand]], [[spot.control.nominalForces|spot-control-nominalForces]], [[spot.control.verifyConsoleController|spot-control-verifyConsoleController]], [[spot.massprops.centerOfGravity|spot-massprops-centerOfGravity]], [[spot.massprops.compute|spot-massprops-compute]], [[spot.params.defaults|spot-params-defaults]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/thrusterDutyCycles.m</b> (320 lines; click to show)</summary>

```matlab
   1  function [duty, Fb, H] = thrusterDutyCycles(u, theta, thrusterDist, F, maxIters, tol)
   2  %THRUSTERDUTYCYCLES The diagram's thruster mixer: the 8 thrusters' duty cycles for a force command.
   3  %   [duty, Fb, H] = spot.control.thrusterDutyCycles(u, theta, thrusterDist, F) computes
   4  %   what the diagram's "Thruster & Float Control Code/Change <PLATFORM> Behavior" computes each
   5  %   step:
   6  %     u             [Fx; Fy; Tz] in the table frame [N, N, N m]: *_Controls, from the controller
   7  %                   (spot.control.lqrCommand for the default LQR)
   8  %     theta         the platform's measured attitude [rad]: *_Measured_States(3)
   9  %     thrusterDist  8x1 distance of each thruster's line of action from the CG [mm]:
  10  %                   thruster_dist2CG_<PLATFORM> (spot.massprops.centerOfGravity)
  11  %     F             8x1 thrust of each thruster [N]: F_thrusters_<PLATFORM> (the calibrated thrust,
  12  %                   Calibration tab, or spot.control.nominalForces); one number: all eight alike
  13  %     maxIters, tol the mixer block's mask values (Maximum Number of Iterations, Convergence
  14  %                   Tolerance); 20 and 1e-6 as in the diagram if left out
  15  %   Returns
  16  %     duty          8x1 duty cycles in [0, 1] (*_Duty_Cycle; times 100 is the percentage sent
  17  %                   to the Teensy)
  18  %     Fb            the command in the body frame [Fx_b; Fy_b; Tz]
  19  %     H             3x8 map from duty cycles to [Fx_b; Fy_b; Tz] with the thrust decay of these
  20  %                   duty cycles (the block's H_final)
  21  %
  22  %   The steps are the diagram's blocks, in its order:
  23  %     1. "Rotate Forces to Body": Fb = C_bI(theta) * [Fx; Fy] (RotateFrame below)
  24  %     2. "Calculate Duty Cycle/Generate Initial Guess": x0 = pinv(MakeH(...)) * Fb, with MakeH
  25  %        below and the DSP Pseudoinverse block (pinv)
  26  %     3. "Calculate Duty Cycle/MATLAB Function5": optimize_duty_cycle_with_decay below, a
  27  %        least-squares fit with 0 <= duty <= 1 (projected gradient), repeated while the thrust
  28  %        decay factor (fewer newtons when many valves are open) changes
  29  %   The functions below are verbatim copies of those blocks' code (the RED blocks; BLACK's and
  30  %   BLUE's are the same code). Only this first function, which wires them, is new.
  31  %   (The block's other outputs, the saturated forces for the simulated plant, are not needed.)
  32  %
  33  %   The lab console's Interactive Mode runs this mixer in Python (duty_cycles in
  34  %   +spot/+console/webapp/spot_console/control.py). spot.control.verifyConsoleController checks
  35  %   that the copies below are still the diagram's code, that this function reproduces the duty
  36  %   cycles the diagram logs in a simulation, and that the Python port agrees with it.
  37  %
  38  %   Example (the Chaser at 30 degrees, pushed along the table's +x with 0.2 N):
  39  %     s = spot.params.defaults();  mp = spot.massprops.compute(s.mass);
  40  %     duty = spot.control.thrusterDutyCycles([0.2; 0; 0], pi/6, mp.RED.thrusters, s.thrusterForce.RED)
  41  %
  42  %   See also spot.control.lqrCommand, spot.control.nominalForces.
  43  if nargin < 5
  44      maxIters = 20;
  45  end
  46  if nargin < 6
  47      tol = 1e-6;
  48  end
  49  u = u(:);
  50  thrusterDist = thrusterDist(:);
  51  F_thrusters = F(:) .* ones(8, 1);   % (one number: all eight alike)
  52  
  53  % 1. Rotate Forces to Body: Demux [2,1] of *_Controls, C_bI * F_I, Mux with Tz
  54  Fb = [RotateFrame(theta) * u(1:2); u(3)];
  55  
  56  % 2. Generate Initial Guess: MakeH gets thruster_dist2CG_<PLATFORM> and F_thrusters_<PLATFORM>
  57  x0 = pinv(MakeH(thrusterDist, F_thrusters)) * Fb;
  58  
  59  % 3. MATLAB Function5: the same thrusts
  60  [H, duty] = optimize_duty_cycle_with_decay(Fb, x0, thrusterDist, F_thrusters, maxIters, tol);
  61  end
  62  
  63  % ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Rotate Forces to Body/Create Rotation Matrix
  64  function C_bI = RotateFrame(Rz)
  65  
  66  C_bI = [  cos(Rz) sin(Rz)
  67           -sin(Rz) cos(Rz) ];
  68  
  69  end
  70  % ---- end verbatim
  71  
  72  % ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Calculate Duty Cycle/Generate Initial Guess/MATLAB Function
  73  function H = MakeH(thruster_dist2CG_RED,F_thrusters_RED)
  74  
  75      Vec1 = [ -1
  76               -1
  77                0 
  78                0
  79                1
  80                1
  81                0
  82                0 ];
  83            
  84      Vec2 = [  0
  85                0
  86                1
  87                1
  88                0
  89                0
  90               -1
  91               -1 ];
  92           
  93      Vec3 = thruster_dist2CG_RED./1000;
  94      
  95      Mat1 = [Vec1, Vec2, Vec3]';
  96      
  97      Mat2 = diag((F_thrusters_RED./2));
  98      
  99      H    = Mat1*Mat2;
 100  
 101  end
 102  % ---- end verbatim
 103  
 104  % ---- verbatim: Thruster & Float Control Code/Change RED Behavior/Calculate Duty Cycle/MATLAB Function5
 105  function [H_final, duty_cycles] = optimize_duty_cycle_with_decay( ...
 106      u_desired, x0, thruster_dist2CG_RED, F_thrusters_RED, max_iters, tol)
 107      %OPTIMIZE_DUTY_CYCLE_WITH_DECAY 
 108      %   Iteratively optimizes thruster duty cycles while updating thrust decay.
 109      %
 110      %   Inputs:
 111      %       u_desired           - Desired force/torque vector [Fx; Fy; M] (3x1)
 112      %       x0                  - Initial guess for duty cycles (8x1)
 113      %       thruster_dist2CG_RED- Distances of thrusters from CG (vector, mm)
 114      %       F_thrusters_RED     - Thrust of each thruster (8x1, N): calibrated
 115      %                             (Calibration tab) or nominal (all eight alike)
 116      %       max_iters           - Maximum optimization iterations
 117      %       tol                 - Convergence tolerance (on duty cycles)
 118      %
 119      %   Outputs:
 120      %       H_final     - Final H-matrix including thrust decay (3x8)
 121      %       duty_cycles - Optimized duty cycles (8x1)
 122      %
 123  
 124      % ---------------------------
 125      % Initialization
 126      % ---------------------------
 127      num_thrusters      = length(x0); %#ok<NASGU> % reserved if needed later
 128      duty_cycles        = x0;                     % current solution
 129      thrust_decay_factor = 1;                     % initial decay
 130      prev_duty_cycles   = duty_cycles + 2*tol;    % force first iteration
 131  
 132      % ---------------------------
 133      % Iterative optimization
 134      % ---------------------------
 135      for iter = 1:max_iters
 136          % Build system matrix H with current decay factor
 137          H = MakeHWithDecay(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);
 138  
 139          % Solve constrained least squares (box constraints: 0 ≤ x ≤ 1)
 140          duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles);
 141  
 142          % Update decay factor based on the current duty cycles
 143          [thrust_decay_factor, ~] = check_thrust_decay(duty_cycles);
 144  
 145          % Check convergence (∞-norm difference)
 146          if norm(duty_cycles - prev_duty_cycles, Inf) < tol
 147              H_final = MakeHWithDecayRecalculateForce(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);
 148              return;
 149          end
 150  
 151          % Update reference for next iteration
 152          prev_duty_cycles = duty_cycles;
 153      end
 154  
 155      % If not converged within max_iters, return final H with last decay
 156      H_final = MakeHWithDecayRecalculateForce(thruster_dist2CG_RED, F_thrusters_RED, thrust_decay_factor);
 157  end
 158  
 159  
 160  % ========================================================================
 161  % Custom Constrained Least Squares Solver
 162  % ========================================================================
 163  function x_opt = custom_constrained_least_squares(H, b, x0)
 164      %CUSTOM_CONSTRAINED_LEAST_SQUARES
 165      %   Solves min_x ||H*x - b||^2 subject to 0 <= x <= 1
 166      %   using projected gradient descent with backtracking line search.
 167      %
 168      %   Inputs:
 169      %       H   - System matrix (3x8)
 170      %       b   - Desired vector (3x1)
 171      %       x0  - Initial guess (8x1)
 172      %
 173      %   Output:
 174      %       x_opt - Optimal duty cycle vector (8x1)
 175  
 176      % Parameters
 177      max_iterations = 100;
 178      tolerance      = 1e-6;
 179      alpha          = 0.01;  % initial step size
 180      beta           = 0.5;   % backtracking factor
 181  
 182      % Initialization
 183      x      = x0;
 184      x_prev = x0 + 2*tolerance;
 185  
 186      % Precompute matrices
 187      HtH = H' * H;
 188      Htb = H' * b;
 189  
 190      for iter = 1:max_iterations
 191          % Gradient of ||H*x - b||^2
 192          gradient = 2 * (HtH*x - Htb);
 193  
 194          % Store objective for comparison
 195          current_obj = norm(H*x - b)^2;
 196  
 197          % Backtracking line search
 198          step_size = alpha;
 199          for ls_iter = 1:20
 200              x_new = x - step_size * gradient;
 201              x_new = max(0, min(1, x_new)); % project into [0,1]
 202  
 203              new_obj = norm(H*x_new - b)^2;
 204              if new_obj < current_obj
 205                  break; % accept step
 206              else
 207                  step_size = step_size * beta;
 208              end
 209          end
 210  
 211          % Update
 212          x = x_new;
 213  
 214          % Convergence check
 215          if norm(x - x_prev) < tolerance
 216              break;
 217          end
 218  
 219          % Adaptive step size tuning
 220          if iter > 1 && new_obj < current_obj
 221              alpha = min(alpha * 1.1, 0.1); % speed up if progress
 222          else
 223              alpha = alpha * 0.9;           % slow down otherwise
 224          end
 225  
 226          % Store for next iteration
 227          x_prev = x;
 228      end
 229  
 230      x_opt = x;
 231  end
 232  
 233  
 234  % ========================================================================
 235  % Construct H-matrix (with decay factor)
 236  % ========================================================================
 237  function H = MakeHWithDecay(thruster_dist2CG, F_thrusters, thrust_decay_factor)
 238      %MAKEHWITHDECAY
 239      %   Builds the H matrix that maps thruster duty cycles to [Fx; Fy; M],
 240      %   applying thrust decay.
 241      %
 242      %   Outputs H as (3x8)
 243  
 244      % Convert thruster arm distances to meters
 245      adjusted_dist = thruster_dist2CG / 1000;
 246  
 247      % Thruster directions (X, Y, and moment arm)
 248      Vec1 = [-1; -1;  0;  0;  1;  1;  0;  0]; % X
 249      Vec2 = [ 0;  0;  1;  1;  0;  0; -1; -1]; % Y
 250      Vec3 = adjusted_dist;                    % moment (Z)
 251  
 252      Mat1 = [Vec1, Vec2, Vec3]';
 253  
 254      % Thruster forces (scaled by decay)
 255      F_decayed = F_thrusters(:) * thrust_decay_factor;
 256  
 257      % Build diagonal scaling
 258      Mat2 = diag(F_decayed / 2);
 259  
 260      % Final mapping
 261      H = Mat1 * Mat2;
 262  end
 263  
 264  
 265  % ========================================================================
 266  % Construct H-matrix (recalculate full force with decay)
 267  % ========================================================================
 268  function H = MakeHWithDecayRecalculateForce(thruster_dist2CG, F_thrusters, thrust_decay_factor)
 269      %MAKEHWITHDECAYRECALCULATEFORCE
 270      %   Same as MakeHWithDecay but applies full nominal force scaling.
 271  
 272      adjusted_dist = thruster_dist2CG / 1000;
 273  
 274      Vec1 = [-1; -1;  0;  0;  1;  1;  0;  0];
 275      Vec2 = [ 0;  0;  1;  1;  0;  0; -1; -1];
 276      Vec3 = adjusted_dist;
 277  
 278      Mat1 = [Vec1, Vec2, Vec3]';
 279  
 280      F_decayed = F_thrusters(:) * thrust_decay_factor;
 281  
 282      Mat2 = diag(F_decayed);
 283  
 284      H = Mat1 * Mat2;
 285  end
 286  
 287  
 288  % ========================================================================
 289  % Thrust Decay Update Rule
 290  % ========================================================================
 291  function [thrust_decay_factor, count] = check_thrust_decay(PWMs)
 292      %CHECK_THRUST_DECAY
 293      %   Updates thrust decay factor based on average duty cycle.
 294      %
 295      %   Inputs:
 296      %       PWMs - Duty cycle vector (8x1)
 297      %
 298      %   Outputs:
 299      %       thrust_decay_factor - Adjusted decay factor (scalar)
 300      %       count               - Number of active thrusters
 301  
 302      % Normalize if needed
 303      if max(PWMs) > 1
 304          PWMs = PWMs / max(PWMs);
 305      end
 306      
 307      % Active thrusters
 308      count = sum(PWMs > 0);
 309      
 310      % Mean duty cycle (only active thrusters)
 311      avg_duty = sum(max(PWMs, 0)) / max(count, 1);
 312      
 313      % Update rule (piecewise)
 314      if avg_duty < 0.3 || count == 0
 315          thrust_decay_factor = 1;
 316      else
 317          thrust_decay_factor = max(0.6 - 2*avg_duty + 1, 0.5);
 318      end
 319  end
 320  % ---- end verbatim
```

</details>
<!-- /listing -->
