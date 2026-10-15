# control.py

**File:** `+spot/+console/webapp/spot_console/control.py` · **Part of:** [[the console's server|Console-Server]] · **MATLAB originals:** [[spot.control|spot-control]]

<!-- source: +spot/+console/webapp/spot_console/control.py -->

## What it is for

The template diagram's default controller in plain Python (standard library only), for Interactive Mode, which has no MATLAB. Each function is a line-by-line port of the MATLAB in `+spot/+control`, which holds verbatim copies of the diagram's blocks; [[spot.control.verifyConsoleController|spot-control-verifyConsoleController]] runs this file as a script and checks that it gives the same gains, forces and duty cycles, and that `DEFAULTS` are SPOT's defaults. Maths: [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]], [[The Thruster Mixer|Math-Thruster-Mixer]].

## Function by function

**Lines 32–65.** `DEFAULTS`: the sample rate (20 Hz), data rate (10 Hz), the mixer's mask values, and per platform the mass, inertia, thruster offsets, thrust and LQR weights, as SPOT's default settings give them. The thrust is one number for all eight thrusters; a `params` given to `Controller` may instead have a list of 8 (each thruster's, as the lab's thruster calibration gives them).

**Lines 71–122.** Small matrix helpers on lists of rows: zeros, identity, diagonal, transpose, multiply, matrix-vector, add, `solve` (Gauss-Jordan with partial pivoting), 2-norm.

**Lines 127–139.** `state_space`: the discrete double integrator, written out directly (`A = [I TsI; 0 I]`, `B = [Ts²/2 M⁻¹; Ts M⁻¹]`, which is exactly what `c2d` gives here).

**Lines 142–167.** `lqr_gains`: the discrete Riccati equation solved by the **structure-preserving doubling algorithm** (each iteration doubles the horizon, so it converges in a few dozen steps): from $A_0 = A$, $G_0 = B R^{-1} B^T$, $H_0 = Q$, with $W = I + G_k H_k$: $A_{k+1} = A_k W^{-1} A_k$, $G_{k+1} = G_k + A_k W^{-1} G_k A_k^T$, $H_{k+1} = H_k + A_k^T H_k W^{-1} A_k$, and $H_k \to P$; then $K = (R + B^T P B)^{-1} B^T P A$. (MATLAB uses a Schur method; the two agree to 1e-9 in the verifier.)

**Lines 170–173.** `platform_gains`: `K` at the PhaseSpace period, as `spot.params.apply` makes it.

**Lines 178–198.** `matlab_mod` (MATLAB's `mod`, result with the sign of the divisor), `smooth_angular_difference`, `lqr_command` (the error vector and $-Ke$).

**Lines 202–329.** The mixer, function by function as the MATLAB: the direction vectors, `rotate_frame`, `make_h`, `make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve)` (each thruster's thrust times the decay, halved or full), `pinv_full_row_rank` ($H^T (H H^T)^{-1}$ for the 3×8 full-row-rank H), `custom_constrained_least_squares` (the same steps and constants), `check_thrust_decay`, `optimize_duty_cycle_with_decay(u, x0, dist, F_thrusters, max_iters, tol)`, `thrusts(F)` (the eight thrusts from a list of 8 or one number), and `duty_cycles(u, theta, dist, F, max_iters, tol)` wiring the three steps with `thrusts(F)`.

**Lines 334–360.** `Controller`: one platform's parameters (`params`, as `DEFAULTS`; `None` uses `DEFAULTS`; `params["thrust"]` may be one number or a list of 8, the lab's calibrated thrust) and gain; `step(measured, desired)` takes the desired pose's derivative over the last step (starting from the first desired pose rather than from 0, so engaging doesn't kick the platform), runs the LQR law and the mixer, and returns the forces, the duty cycles and the body-frame command.

**Lines 363–377.** `serve`: run a `Controller` (with the `params` it is given) in a process of its own, answering `(measured, desired)` messages on a pipe until `None` or the pipe closes (used by [[interactive.py|Console-interactive]]).

**Lines 382–400.** `main`: for the MATLAB verifier: cases as JSON on stdin, results as JSON on stdout (the mixer cases give `F`, the eight thrusts).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/control.py</b> (404 lines; click to show)</summary>

```python
   1  """The template diagram's default controller, in plain Python, for the lab console's Interactive
   2  mode (interactive.py), which has no MATLAB to run it with.
   3  
   4  Each function is a line-by-line port of MATLAB code in +spot/+control, which in turn holds
   5  verbatim copies of the diagram's blocks:
   6  
   7      lqr_gains     spot.control.stateSpace + spot.control.lqrGains (MATLAB's dlqr)
   8      lqr_command   spot.control.lqrCommand          "4 - Control/Default LQR Control (<PLATFORM>)"
   9      duty_cycles   spot.control.thrusterDutyCycles  "Thruster & Float Control Code/Change <PLATFORM> Behavior"
  10  
  11  The MATLAB names are kept in the comments so the two can be read side by side.
  12  spot.control.verifyConsoleController (in MATLAB) checks that this file gives the same gains, forces
  13  and duty cycles as MATLAB, and that DEFAULTS are SPOT's default settings; it runs this file as a
  14  script (see main() at the end).
  15  
  16  Python 3.6 standard library only, and nothing else from the console, so it can be read and run on
  17  its own.
  18  """
  19  
  20  import json
  21  import math
  22  import sys
  23  from operator import mul
  24  
  25  
  26  # ---------------------------------------------------------------------------- parameters
  27  
  28  # SPOT's default settings (spot.params.defaults), what spot.massprops.compute makes of their mass
  29  # section, and the mixer block's mask values in the diagram. The lab console has no settings file
  30  # of the project's, so Interactive Mode uses these; spot.control.verifyConsoleController checks
  31  # them against MATLAB.
  32  DEFAULTS = {
  33      # The diagram runs every baseRate = 1/sample_rate seconds, its LQR and its thruster code
  34      # included; its LQR gains are designed for the PhaseSpace period, dataRate = 1/data_rate
  35      # (spot.params.apply)
  36      "sample_rate": 20.0,                  # [Hz] s.sampleRate
  37      "data_rate": 10.0,                    # [Hz] s.dataRate
  38      # "Calculate Duty Cycle/MATLAB Function5" mask: Maximum Number of Iterations, Convergence Tolerance
  39      "max_iters": 20,
  40      "tol": 1e-6,
  41      "platforms": {
  42          # mass [kg], inertia [kg m^2] and thruster_dist2CG [mm] from the default scale readings and
  43          # bifilar test; thrust [N] of one thruster (s.thrusterForce; a list of 8: each thruster's, as
  44          # the lab's thruster calibration gives them, calibration.py); LQR weights
  45          # Q = [x y theta xdot ydot thetadot], R = [Fx Fy Tz] (s.control.<PLATFORM>.lqr)
  46          "RED": {
  47              "mass": 13.75, "inertia": 0.23543476058293455, "thrust": 0.2825,
  48              "thruster_dist2CG": [84.381818181818176, -53.618181818181817, 54.381818181818176, -83.61818181818181,
  49                                   53.618181818181817, -84.381818181818176, 83.61818181818181, -54.381818181818176],
  50              "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
  51          },
  52          "BLACK": {
  53              "mass": 13.219999999999999, "inertia": 0.24326386911004549, "thrust": 0.2825,
  54              "thruster_dist2CG": [79.665658093797262, -58.334341906202717, 58.334341906202717, -79.665658093797262,
  55                                   58.334341906202717, -79.665658093797262, 79.665658093797262, -58.334341906202717],
  56              "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
  57          },
  58          "BLUE": {
  59              "mass": 12.77, "inertia": 0.22157979686198798, "thrust": 0.2825,
  60              "thruster_dist2CG": [86.267032106499599, -51.732967893500387, 57.723570869224737, -80.276429130775242,
  61                                   51.732967893500387, -86.267032106499599, 80.276429130775242, -57.723570869224737],
  62              "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
  63          },
  64      },
  65  }
  66  
  67  
  68  # ---------------------------------------------------------------------------- small matrices
  69  # Lists of rows; vectors are plain lists.
  70  
  71  def zeros(rows, cols):
  72      return [[0.0] * cols for _ in range(rows)]
  73  
  74  
  75  def eye(n):
  76      return [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
  77  
  78  
  79  def diag(values):
  80      m = zeros(len(values), len(values))
  81      for i, v in enumerate(values):
  82          m[i][i] = float(v)
  83      return m
  84  
  85  
  86  def transpose(A):
  87      return [list(col) for col in zip(*A)]
  88  
  89  
  90  def matmul(A, B):
  91      Bt = transpose(B)
  92      return [[sum(map(mul, row, col)) for col in Bt] for row in A]
  93  
  94  
  95  def matvec(A, x):
  96      return [sum(map(mul, row, x)) for row in A]
  97  
  98  
  99  def add(A, B):
 100      return [[a + b for a, b in zip(ra, rb)] for ra, rb in zip(A, B)]
 101  
 102  
 103  def solve(A, B):
 104      """A \\ B for a square, invertible A (Gauss-Jordan elimination with partial pivoting)."""
 105      n = len(A)
 106      M = [list(map(float, A[i])) + list(map(float, B[i])) for i in range(n)]
 107      for c in range(n):
 108          p = max(range(c, n), key=lambda r: abs(M[r][c]))
 109          if M[p][c] == 0.0:
 110              raise ValueError("singular matrix")
 111          M[c], M[p] = M[p], M[c]
 112          pivot = M[c][c]
 113          M[c] = [v / pivot for v in M[c]]
 114          for r in range(n):
 115              if r != c and M[r][c] != 0.0:
 116                  f = M[r][c]
 117                  M[r] = [v - f * w for v, w in zip(M[r], M[c])]
 118      return [row[n:] for row in M]
 119  
 120  
 121  def norm2(v):
 122      return math.sqrt(sum(map(mul, v, v)))
 123  
 124  
 125  # ---------------------------------------------------------------------------- LQR gains
 126  
 127  def state_space(mass, inertia, Ts):
 128      """spot.control.stateSpace: the zero-order-hold discretisation (sample time Ts [s]) of the
 129      double integrator with states [x y theta xdot ydot thetadot] and inputs [Fx Fy Tz].
 130      (MATLAB's c2d computes it with a matrix exponential; for a double integrator that is exactly
 131      A = [I Ts*I; 0 I], B = [Ts^2/2 * Minv; Ts * Minv].)"""
 132      Minv = [1.0 / mass, 1.0 / mass, 1.0 / inertia]
 133      A = eye(6)
 134      B = zeros(6, 3)
 135      for i in range(3):
 136          A[i][i + 3] = Ts
 137          B[i][i] = Ts * Ts / 2 * Minv[i]
 138          B[i + 3][i] = Ts * Minv[i]
 139      return A, B
 140  
 141  
 142  def lqr_gains(A, B, Q, R, max_iterations=100):
 143      """spot.control.lqrGains: K = dlqr(A, B, diag(Q), diag(R)), the 3x6 gain of u = -K*x that
 144      minimises sum(x'Qx + u'Ru). P solves the discrete algebraic Riccati equation
 145          P = Q + A'PA - A'PB (R + B'PB)^-1 B'PA,
 146      and K = (R + B'PB)^-1 B'PA. MATLAB finds P with a Schur method; here it is found with the
 147      structure-preserving doubling algorithm (each step doubles the horizon, so it reaches machine
 148      precision in a few dozen steps): from A0 = A, G0 = B R^-1 B', H0 = Q, with W = I + Gk Hk,
 149          Ak+1 = Ak W^-1 Ak,   Gk+1 = Gk + Ak W^-1 Gk Ak',   Hk+1 = Hk + Ak' Hk W^-1 Ak,
 150      and Hk -> P."""
 151      n = len(A)
 152      Ak, Hk = A, diag(Q)
 153      Gk = matmul(B, solve(diag(R), transpose(B)))
 154      for _ in range(max_iterations):
 155          W = add(eye(n), matmul(Gk, Hk))
 156          W_Ak, W_GkAkt = solve(W, Ak), solve(W, matmul(Gk, transpose(Ak)))
 157          H_next = add(Hk, matmul(matmul(transpose(Ak), Hk), W_Ak))
 158          Gk = add(Gk, matmul(Ak, W_GkAkt))
 159          Ak = matmul(Ak, W_Ak)
 160          change = max(abs(a - b) for ra, rb in zip(H_next, Hk) for a, b in zip(ra, rb))
 161          Hk = H_next
 162          if change <= 1e-15 * max(abs(v) for row in Hk for v in row):
 163              break
 164      else:
 165          raise ValueError("the Riccati equation has no solution here (are Q and R positive?)")
 166      P, Rm, Bt = Hk, diag(R), transpose(B)
 167      return solve(add(Rm, matmul(Bt, matmul(P, B))), matmul(Bt, matmul(P, A)))
 168  
 169  
 170  def platform_gains(params, data_rate):
 171      """K_<PLATFORM> as spot.params.apply makes it: the model at the period dataRate = 1/data_rate."""
 172      A, B = state_space(params["mass"], params["inertia"], 1.0 / data_rate)
 173      return lqr_gains(A, B, params["Q"], params["R"])
 174  
 175  
 176  # ---------------------------------------------------------------------------- the control law
 177  
 178  def matlab_mod(a, m):
 179      """MATLAB's mod(a, m) for m > 0: the result has the sign of m."""
 180      return a - math.floor(a / m) * m
 181  
 182  
 183  def smooth_angular_difference(Rz_path, Rz_measured):
 184      """SmoothAngularDifference ("Calculate Error"): the attitude error wrapped to [-pi, pi)."""
 185      return matlab_mod(Rz_measured - Rz_path + math.pi, 2 * math.pi) - math.pi
 186  
 187  
 188  def lqr_command(K, measured, desired, desired_rate):
 189      """spot.control.lqrCommand: u = -K*e = [Fx, Fy, Tz] in the table frame, with
 190      measured = [x y theta xdot ydot thetadot], desired = [x y theta] (*_Path) and desired_rate its
 191      derivative (the block's Discrete Derivative)."""
 192      e = [measured[0] - desired[0],
 193           measured[1] - desired[1],
 194           smooth_angular_difference(desired[2], measured[2]),
 195           measured[3] - desired_rate[0],
 196           measured[4] - desired_rate[1],
 197           measured[5] - desired_rate[2]]
 198      return [-v for v in matvec(K, e)]
 199  
 200  
 201  # ---------------------------------------------------------------------------- the thruster mixer
 202  # Thruster directions: 1, 2 push along -x, 5, 6 along +x, 3, 4 along +y, 7, 8 along -y (body frame)
 203  VEC1 = [-1, -1, 0, 0, 1, 1, 0, 0]  # X
 204  VEC2 = [0, 0, 1, 1, 0, 0, -1, -1]  # Y
 205  
 206  
 207  def rotate_frame(Rz):
 208      """RotateFrame ("Rotate Forces to Body/Create Rotation Matrix"): C_bI."""
 209      return [[math.cos(Rz), math.sin(Rz)],
 210              [-math.sin(Rz), math.cos(Rz)]]
 211  
 212  
 213  def make_h(thruster_dist2CG, F_thrusters):
 214      """MakeH ("Generate Initial Guess/MATLAB Function"): [Vec1, Vec2, dist/1000]' * diag(F/2)."""
 215      Mat1 = [VEC1, VEC2, [d / 1000 for d in thruster_dist2CG]]
 216      return matmul(Mat1, diag([f / 2 for f in F_thrusters]))
 217  
 218  
 219  def make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=True):
 220      """MakeHWithDecay (halve=True: diag(F/2)) and MakeHWithDecayRecalculateForce (halve=False:
 221      diag(F)), with every thruster's thrust (F_thrusters, 8) scaled by the decay factor."""
 222      adjusted_dist = [d / 1000 for d in thruster_dist2CG]
 223      Mat1 = [VEC1, VEC2, adjusted_dist]
 224      F_decayed = [f * thrust_decay_factor for f in F_thrusters]
 225      Mat2 = diag([f / 2 for f in F_decayed] if halve else F_decayed)
 226      return matmul(Mat1, Mat2)
 227  
 228  
 229  def pinv_full_row_rank(H):
 230      """pinv(H) for a 3x8 H of rank 3 (the DSP Pseudoinverse block): H' (H H')^-1."""
 231      Ht = transpose(H)
 232      return transpose(solve(matmul(H, Ht), H))   # (H H')^-1 H, transposed; H H' is symmetric
 233  
 234  
 235  def custom_constrained_least_squares(H, b, x0):
 236      """custom_constrained_least_squares: min ||H*x - b||^2 with 0 <= x <= 1, by projected gradient
 237      descent with a backtracking line search (the same steps and constants as the block)."""
 238      max_iterations = 100
 239      tolerance = 1e-6
 240      alpha = 0.01  # initial step size
 241      beta = 0.5    # backtracking factor
 242  
 243      x = list(x0)
 244      x_prev = [v + 2 * tolerance for v in x0]
 245  
 246      HtH = matmul(transpose(H), H)
 247      Htb = matvec(transpose(H), b)
 248  
 249      def objective(v):  # norm(H*v - b)^2
 250          r = [sum(map(mul, row, v)) - bv for row, bv in zip(H, b)]
 251          return math.sqrt(sum(map(mul, r, r))) ** 2
 252  
 253      for iteration in range(1, max_iterations + 1):
 254          gradient = [2 * (sum(map(mul, row, x)) - c) for row, c in zip(HtH, Htb)]
 255          current_obj = objective(x)
 256  
 257          step_size = alpha
 258          for _ in range(20):
 259              x_new = [0.0 if v < 0.0 else 1.0 if v > 1.0 else v
 260                       for v in [xv - step_size * gv for xv, gv in zip(x, gradient)]]   # max(0, min(1, .))
 261              new_obj = objective(x_new)
 262              if new_obj < current_obj:
 263                  break
 264              step_size = step_size * beta
 265  
 266          x = x_new
 267  
 268          if norm2([a - p for a, p in zip(x, x_prev)]) < tolerance:
 269              break
 270  
 271          if iteration > 1 and new_obj < current_obj:
 272              alpha = min(alpha * 1.1, 0.1)
 273          else:
 274              alpha = alpha * 0.9
 275  
 276          x_prev = x
 277      return x
 278  
 279  
 280  def check_thrust_decay(PWMs):
 281      """check_thrust_decay: the thrust decay factor for these duty cycles, and how many are on."""
 282      if max(PWMs) > 1:
 283          top = max(PWMs)
 284          PWMs = [p / top for p in PWMs]
 285      count = sum(1 for p in PWMs if p > 0)
 286      avg_duty = sum(max(p, 0) for p in PWMs) / max(count, 1)
 287      if avg_duty < 0.3 or count == 0:
 288          thrust_decay_factor = 1
 289      else:
 290          thrust_decay_factor = max(0.6 - 2 * avg_duty + 1, 0.5)
 291      return thrust_decay_factor, count
 292  
 293  
 294  def optimize_duty_cycle_with_decay(u_desired, x0, thruster_dist2CG, F_thrusters, max_iters, tol):
 295      """optimize_duty_cycle_with_decay ("MATLAB Function5"): returns (H_final, duty_cycles)."""
 296      duty_cycles = list(x0)
 297      thrust_decay_factor = 1
 298      prev_duty_cycles = [d + 2 * tol for d in duty_cycles]
 299  
 300      for _ in range(max_iters):
 301          H = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor)
 302          duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles)
 303          thrust_decay_factor, _count = check_thrust_decay(duty_cycles)
 304          if max(abs(a - b) for a, b in zip(duty_cycles, prev_duty_cycles)) < tol:
 305              break
 306          prev_duty_cycles = duty_cycles
 307      H_final = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=False)
 308      return H_final, duty_cycles
 309  
 310  
 311  def thrusts(F):
 312      """The 8 thrusters' thrust from F: a list of 8, or one number for all eight."""
 313      return [float(f) for f in F] if isinstance(F, (list, tuple)) else [float(F)] * 8
 314  
 315  
 316  def duty_cycles(u, theta, thruster_dist2CG, F, max_iters=20, tol=1e-6):
 317      """spot.control.thrusterDutyCycles: (duty, Fb, H) for the table-frame command u = [Fx Fy Tz]
 318      of a platform at attitude theta [rad] whose thrusters give F [N] (8, or one for all). duty: the
 319      8 duty cycles in [0, 1]; Fb: the command in the body frame; H: the 3x8 map from duty cycles to
 320      body forces (with thrust decay)."""
 321      F_thrusters = thrusts(F)
 322      # 1. Rotate Forces to Body
 323      C_bI = rotate_frame(theta)
 324      Fb = matvec(C_bI, u[:2]) + [u[2]]
 325      # 2. Generate Initial Guess
 326      x0 = matvec(pinv_full_row_rank(make_h(thruster_dist2CG, F_thrusters)), Fb)
 327      # 3. MATLAB Function5
 328      H, duty = optimize_duty_cycle_with_decay(Fb, x0, thruster_dist2CG, F_thrusters, max_iters, tol)
 329      return duty, Fb, H
 330  
 331  
 332  # ---------------------------------------------------------------------------- one platform
 333  
 334  class Controller:
 335      """The diagram's default controller for one platform, stepped every baseRate (1/sample_rate),
 336      as the diagram runs its LQR law and its thruster code: the LQR law (the desired pose's
 337      derivative taken over the last step, as the block's Discrete Derivative does), then the mixer."""
 338  
 339      def __init__(self, platform, params=None):
 340          self.platform = platform
 341          d = params or DEFAULTS
 342          self.params = dict(d["platforms"][platform])
 343          self.sample_rate = float(d["sample_rate"])
 344          self.max_iters, self.tol = int(d["max_iters"]), float(d["tol"])
 345          self.period = 1.0 / self.sample_rate   # [s] between steps: baseRate
 346          self.K = platform_gains(self.params, float(d["data_rate"]))
 347          self._previous = None              # the desired pose one step earlier
 348  
 349      def step(self, measured, desired):
 350          """measured = [x y theta xdot ydot thetadot] (theta wrapped), desired = [x y theta].
 351          Returns (u in the table frame, duty cycles in [0, 1], the command in the body frame)."""
 352          # The diagram's Discrete Derivative starts from 0 (a jump at its first step); here it
 353          # starts from the first desired pose instead, so engaging doesn't kick the platform
 354          previous = self._previous or list(desired)
 355          rate = [(d - p) / self.period for d, p in zip(desired, previous)]
 356          self._previous = list(desired)
 357          u = lqr_command(self.K, measured, desired, rate)
 358          p = self.params
 359          duty, Fb, _H = duty_cycles(u, measured[2], p["thruster_dist2CG"], p["thrust"], self.max_iters, self.tol)
 360          return u, duty, Fb
 361  
 362  
 363  def serve(conn, platform, params=None):
 364      """Run one platform's Controller (with PARAMS, as Controller takes them; None: DEFAULTS) in a
 365      process of its own (interactive.py starts one per platform flown, so their mixers run side by
 366      side on separate CPU cores): answer each (measured, desired) received on the connection CONN
 367      with Controller.step's result, until None arrives or the other end closes."""
 368      controller = Controller(platform, params)
 369      conn.send("ready")
 370      try:
 371          while True:
 372              msg = conn.recv()
 373              if msg is None:
 374                  break
 375              conn.send(controller.step(*msg))
 376      except (EOFError, OSError):
 377          pass   # the console has gone
 378  
 379  
 380  # ---------------------------------------------------------------------------- for the MATLAB check
 381  
 382  def main():
 383      """Read cases (JSON) on standard input and write the results (JSON) on standard output:
 384         {"gains": [{"mass", "inertia", "Ts", "Q", "R"}, ...],
 385          "lqr": [{"K", "measured", "desired", "rate"}, ...],
 386          "mixer": [{"u", "theta", "dist", "F" (8), "max_iters", "tol"}, ...]}
 387      -> {"gains": [K, ...], "lqr": [u, ...], "mixer": [{"duty", "Fb", "H"}, ...], "defaults": DEFAULTS,
 388          "default_gains": {PLATFORM: K}}"""
 389      cases = json.load(sys.stdin)
 390      out = {"gains": [], "lqr": [], "mixer": [], "defaults": DEFAULTS,
 391             "default_gains": {n: platform_gains(p, DEFAULTS["data_rate"]) for n, p in DEFAULTS["platforms"].items()}}
 392      for c in cases.get("gains", []):
 393          A, B = state_space(c["mass"], c["inertia"], c["Ts"])
 394          out["gains"].append(lqr_gains(A, B, c["Q"], c["R"]))
 395      for c in cases.get("lqr", []):
 396          out["lqr"].append(lqr_command(c["K"], c["measured"], c["desired"], c["rate"]))
 397      for c in cases.get("mixer", []):
 398          duty, Fb, H = duty_cycles(c["u"], c["theta"], c["dist"], c["F"], c["max_iters"], c["tol"])
 399          out["mixer"].append({"duty": duty, "Fb": Fb, "H": H})
 400      json.dump(out, sys.stdout)
 401  
 402  
 403  if __name__ == "__main__":
 404      main()
```

</details>
<!-- /listing -->
