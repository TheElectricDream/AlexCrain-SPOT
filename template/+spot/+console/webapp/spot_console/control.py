"""The template diagram's default controller, in plain Python.

For the lab console's Interactive Mode (interactive.py), which has no
MATLAB to run it with. Each function is a line-by-line port of MATLAB
code in +spot/+control, which in turn holds verbatim copies of the
diagram's blocks:

    lqr_gains    spot.control.stateSpace + spot.control.lqrGains
                 (MATLAB's dlqr)
    lqr_command  spot.control.lqrCommand
                 "4 - Control/Default LQR Control (<PLATFORM>)"
    duty_cycles  spot.control.thrusterDutyCycles
                 "Thruster & Float Control Code/Change <PLATFORM>
                 Behavior"

The MATLAB names are kept (in the code and the comments) so the two
can be read side by side. spot.control.verifyConsoleController (in
MATLAB) checks that this file gives the same gains, forces and duty
cycles as MATLAB, and that DEFAULTS are SPOT's default settings; it
runs this file as a script (see main() at the end).

Python 3.6 standard library only, and nothing else from the console
but SPOT's constants (constants.py reads them from Constants.m),
so it can be read and run on its own.
"""

import json
import math
import os
import sys
from operator import mul

# SPOT's constants, through the package; run as a script (by
# spot.control.verifyConsoleController) the package is the folder this
# file is in.
if __package__:
    from .constants import C
else:
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    from spot_console.constants import C


# Parameters

NUM_THRUSTERS = C.int("NUM_THRUSTERS")
MM_PER_M = 1000   # [mm/m] thruster_dist2CG is in mm, the mixer works in m


def center_of_gravity(scales):
    """Port of spot.massprops.centerOfGravity: mass, CG and moment arms.

    The same operations in the same order as the MATLAB, so the
    numbers are identical to its.

    Args:
        scales: The three scale readings [A B C] [kg] (SCALES_* in
            Constants.m; A left-middle, B right-back, C right-front,
            at SCALE_A, SCALE_B, SCALE_C from the body centre).

    Returns:
        (mass [kg], [X, Y] of the CG from the body centre [m], the 8
        thruster_dist2CG [mm]: each thruster's line of action from the
        CG).
    """
    a, b, c = scales
    M = a + b + c

    # Readings weighted by each scale's position.
    X = (a * C.SCALE_A[0] + b * C.SCALE_B[0] + c * C.SCALE_C[0]) / M
    Y = (a * C.SCALE_A[1] + b * C.SCALE_B[1] + c * C.SCALE_C[1]) / M

    # Thruster offsets from the CG (1-2 push along -x, 3-4 +y, 5-6 +x,
    # 7-8 -y).
    e = C.BODY_HALF_SIDE - C.THRUSTER_INSET
    thrusters = [v * MM_PER_M for v in (e - Y, -(e + Y), e - X, -(e + X),
                                        e + Y, -(e - Y), e + X, -(e - X))]
    return M, [X, Y], thrusters


def platform_defaults(platform):
    """Return a platform's entry of DEFAULTS["platforms"].

    Its mass (the scales' sum, as spot.massprops.compute takes it) and
    thruster_dist2CG from its default scale readings, its inertia from
    its bifilar test (INERTIA_*), the nominal thrust and thrust decay,
    and the LQR weights.
    """
    mass, _cg, dist2CG = center_of_gravity(C.platform("SCALES", platform))
    return {"mass": mass, "inertia": C.platform("INERTIA", platform),
            "thrust": C.THRUST_NOMINAL, "thrust_decay": C.THRUST_DECAY_DEFAULT,
            "thruster_dist2CG": dist2CG, "Q": list(C.LQR_Q), "R": list(C.LQR_R)}


# SPOT's default settings (spot.params.fromConstants), what
# spot.massprops.compute makes of their mass section, and the mixer
# block's mask values in the diagram, all from Constants.m. The
# lab console has no settings file of the project's, so Interactive
# Mode uses these; spot.control.verifyConsoleController checks them
# against MATLAB.
#
# Per platform: mass [kg], inertia [kg m^2] and thruster_dist2CG [mm]
# from the default scale readings and bifilar test; thrust [N] of one
# thruster (s.thrusterForce; a list of 8: each thruster's, as the lab's
# thruster calibration gives them, calibration.py); thrust_decay, the
# thrust lost per extra valve open (s.thrusterDecay: the Obstacle's,
# measured 2026-10-14, until each platform's Together calibration
# replaces it); LQR weights Q = [x y theta xdot ydot thetadot],
# R = [Fx Fy Tz] (s.control.<PLATFORM>.lqr).
DEFAULTS = {
    # The diagram runs every baseRate = 1/sample_rate seconds, its LQR
    # and its thruster code included; its LQR gains are designed for
    # the PhaseSpace period, dataRate = 1/data_rate (spot.params.apply).
    "sample_rate": float(C.SAMPLE_RATE),   # [Hz] s.sampleRate
    "data_rate": float(C.DATA_RATE),       # [Hz] s.dataRate
    # "Calculate Duty Cycle/MATLAB Function5" mask: Maximum Number of
    # Iterations, Convergence Tolerance.
    "max_iters": C.int("MIXER_MAX_ITERATIONS"),
    "tol": C.MIXER_TOLERANCE,
    "platforms": dict((p, platform_defaults(p)) for p in C.PLATFORMS),
}


# Small matrices: lists of rows; vectors are plain lists


def zeros(rows, cols):
    """Return a rows x cols matrix of zeros."""
    return [[0.0] * cols for _ in range(rows)]


def eye(n):
    """Return the n x n identity matrix."""
    return [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]


def diag(values):
    """Return a square matrix with values on its diagonal."""
    m = zeros(len(values), len(values))
    for i, v in enumerate(values):
        m[i][i] = float(v)
    return m


def transpose(A):
    """Return A'."""
    return [list(col) for col in zip(*A)]


def matmul(A, B):
    """Return A * B."""
    Bt = transpose(B)
    return [[sum(map(mul, row, col)) for col in Bt] for row in A]


def matvec(A, x):
    """Return A * x for a vector x."""
    return [sum(map(mul, row, x)) for row in A]


def add(A, B):
    """Return A + B."""
    return [[a + b for a, b in zip(ra, rb)] for ra, rb in zip(A, B)]


def solve(A, B):
    """Return A \\ B for a square, invertible A.

    Gauss-Jordan elimination with partial pivoting.

    Raises:
        ValueError: if A is singular.
    """
    n = len(A)
    M = [list(map(float, A[i])) + list(map(float, B[i])) for i in range(n)]
    for c in range(n):
        # Pivot: the largest entry in the column.
        p = max(range(c, n), key=lambda r: abs(M[r][c]))
        if M[p][c] == 0.0:
            raise ValueError("singular matrix")

        # Normalise the pivot row and clear the column elsewhere.
        M[c], M[p] = M[p], M[c]
        pivot = M[c][c]
        M[c] = [v / pivot for v in M[c]]
        for r in range(n):
            if r != c and M[r][c] != 0.0:
                f = M[r][c]
                M[r] = [v - f * w for v, w in zip(M[r], M[c])]
    return [row[n:] for row in M]


def norm2(v):
    """Return the Euclidean norm of a vector."""
    return math.sqrt(sum(map(mul, v, v)))


# LQR gains


def state_space(mass, inertia, Ts):
    """Port of spot.control.stateSpace: the discrete double integrator.

    The zero-order-hold discretisation (sample time Ts [s]) of the
    double integrator with states [x y theta xdot ydot thetadot] and
    inputs [Fx Fy Tz]. MATLAB's c2d computes it with a matrix
    exponential; for a double integrator that is exactly
    A = [I Ts*I; 0 I], B = [Ts^2/2 * Minv; Ts * Minv].

    Returns:
        (A, B), 6x6 and 6x3.
    """
    Minv = [1.0 / mass, 1.0 / mass, 1.0 / inertia]
    A = eye(6)
    B = zeros(6, 3)
    for i in range(3):
        A[i][i + 3] = Ts
        B[i][i] = Ts * Ts / 2 * Minv[i]
        B[i + 3][i] = Ts * Minv[i]
    return A, B


def lqr_gains(A, B, Q, R, max_iterations=100):
    """Port of spot.control.lqrGains: K = dlqr(A, B, diag(Q), diag(R)).

    K is the 3x6 gain of u = -K*x that minimises sum(x'Qx + u'Ru). P
    solves the discrete algebraic Riccati equation

        P = Q + A'PA - A'PB (R + B'PB)^-1 B'PA

    and K = (R + B'PB)^-1 B'PA. MATLAB finds P with a Schur method;
    here it is found with the structure-preserving doubling algorithm
    (each step doubles the horizon, so it reaches machine precision in
    a few dozen steps): from A0 = A, G0 = B R^-1 B', H0 = Q, with
    W = I + Gk Hk,

        Ak+1 = Ak W^-1 Ak
        Gk+1 = Gk + Ak W^-1 Gk Ak'
        Hk+1 = Hk + Ak' Hk W^-1 Ak

    and Hk -> P.

    Raises:
        ValueError: if it doesn't converge (Q and R not positive).
    """
    n = len(A)
    Ak, Hk = A, diag(Q)
    Gk = matmul(B, solve(diag(R), transpose(B)))

    # Doubling steps until H stops changing (relative to its size).
    for _ in range(max_iterations):
        W = add(eye(n), matmul(Gk, Hk))
        W_Ak, W_GkAkt = solve(W, Ak), solve(W, matmul(Gk, transpose(Ak)))
        H_next = add(Hk, matmul(matmul(transpose(Ak), Hk), W_Ak))
        Gk = add(Gk, matmul(Ak, W_GkAkt))
        Ak = matmul(Ak, W_Ak)
        change = max(abs(a - b) for ra, rb in zip(H_next, Hk) for a, b in zip(ra, rb))
        Hk = H_next
        if change <= 1e-15 * max(abs(v) for row in Hk for v in row):
            break
    else:
        raise ValueError("the Riccati equation has no solution here (are Q and R positive?)")

    # K = (R + B'PB)^-1 B'PA.
    P, Rm, Bt = Hk, diag(R), transpose(B)
    return solve(add(Rm, matmul(Bt, matmul(P, B))), matmul(Bt, matmul(P, A)))


def platform_gains(params, data_rate):
    """Return K_<PLATFORM> as spot.params.apply makes it.

    The model is discretised at the period dataRate = 1/data_rate.
    """
    A, B = state_space(params["mass"], params["inertia"], 1.0 / data_rate)
    return lqr_gains(A, B, params["Q"], params["R"])


# The control law


def matlab_mod(a, m):
    """Return MATLAB's mod(a, m) for m > 0: the sign of m."""
    return a - math.floor(a / m) * m


def smooth_angular_difference(Rz_path, Rz_measured):
    """Port of SmoothAngularDifference ("Calculate Error").

    Returns the attitude error wrapped to [-pi, pi).
    """
    return matlab_mod(Rz_measured - Rz_path + math.pi, 2 * math.pi) - math.pi


def lqr_command(K, measured, desired, desired_rate):
    """Port of spot.control.lqrCommand: u = -K*e.

    Args:
        K: The 3x6 gain.
        measured: [x y theta xdot ydot thetadot].
        desired: [x y theta] (*_Path).
        desired_rate: Its derivative (the block's Discrete
            Derivative).

    Returns:
        [Fx, Fy, Tz] in the table frame.
    """
    e = [measured[0] - desired[0],
         measured[1] - desired[1],
         smooth_angular_difference(desired[2], measured[2]),
         measured[3] - desired_rate[0],
         measured[4] - desired_rate[1],
         measured[5] - desired_rate[2]]
    return [-v for v in matvec(K, e)]


# The thruster mixer

# Thruster directions in the body frame: 1, 2 push along -x, 5, 6
# along +x, 3, 4 along +y, 7, 8 along -y.
VEC1 = list(C.THRUSTER_DIRECTION_X)  # X
VEC2 = list(C.THRUSTER_DIRECTION_Y)  # Y

# custom_constrained_least_squares's constants, as the block has them:
# iterations and convergence tolerance, the first step, the backtracking
# factor and the most backtracking steps, and how the step grows (to a
# cap) after an improving iteration or shrinks after another.
CLS_MAX_ITERATIONS = C.int("MIXER_CLS_MAX_ITERATIONS")
CLS_TOLERANCE = C.MIXER_CLS_TOLERANCE
CLS_ALPHA = C.MIXER_CLS_ALPHA
CLS_BETA = C.MIXER_CLS_BETA
CLS_LINE_SEARCH = C.int("MIXER_CLS_LINE_SEARCH")
CLS_STEP_GROWTH = C.MIXER_CLS_STEP_GROWTH
CLS_STEP_CAP = C.MIXER_CLS_STEP_CAP
CLS_STEP_SHRINK = C.MIXER_CLS_STEP_SHRINK


def rotate_frame(Rz):
    """Port of RotateFrame: C_bI.

    Block: "Rotate Forces to Body/Create Rotation Matrix".
    """
    return [[math.cos(Rz), math.sin(Rz)],
            [-math.sin(Rz), math.cos(Rz)]]


def make_h(thruster_dist2CG, F_thrusters):
    """Port of MakeH: [Vec1, Vec2, dist/1000]' * diag(F/2).

    Block: "Generate Initial Guess/MATLAB Function".
    """
    Mat1 = [VEC1, VEC2, [d / MM_PER_M for d in thruster_dist2CG]]
    return matmul(Mat1, diag([f / 2 for f in F_thrusters]))


def make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=True):
    """Port of MakeHWithDecay and MakeHWithDecayRecalculateForce.

    Args:
        thruster_dist2CG: The 8 moment arms [mm].
        F_thrusters: The 8 thrusts [N].
        thrust_decay_factor: Each thruster's decay factor (8, or one
            for all).
        halve: True for MakeHWithDecay (diag(F/2)), False for
            MakeHWithDecayRecalculateForce (diag(F)).
    """
    adjusted_dist = [d / MM_PER_M for d in thruster_dist2CG]
    Mat1 = [VEC1, VEC2, adjusted_dist]
    if isinstance(thrust_decay_factor, (list, tuple)):
        factors = thrust_decay_factor
    else:
        factors = [thrust_decay_factor] * NUM_THRUSTERS
    F_decayed = [f * e for f, e in zip(F_thrusters, factors)]
    Mat2 = diag([f / 2 for f in F_decayed] if halve else F_decayed)
    return matmul(Mat1, Mat2)


def pinv_full_row_rank(H):
    """Return pinv(H) for a 3x8 H of rank 3: H' (H H')^-1.

    As the DSP Pseudoinverse block. Computed as (H H')^-1 H,
    transposed, since H H' is symmetric.
    """
    Ht = transpose(H)
    return transpose(solve(matmul(H, Ht), H))


def custom_constrained_least_squares(H, b, x0):
    """Port of custom_constrained_least_squares.

    min ||H*x - b||^2 with 0 <= x <= 1, by projected gradient descent
    with a backtracking line search (the same steps and constants as
    the block).
    """
    max_iterations = CLS_MAX_ITERATIONS
    tolerance = CLS_TOLERANCE
    alpha = CLS_ALPHA   # initial step size
    beta = CLS_BETA     # backtracking factor

    x = list(x0)
    x_prev = [v + 2 * tolerance for v in x0]

    HtH = matmul(transpose(H), H)
    Htb = matvec(transpose(H), b)

    def objective(v):
        """Return norm(H*v - b)^2, computed as the block does."""
        r = [sum(map(mul, row, v)) - bv for row, bv in zip(H, b)]
        return math.sqrt(sum(map(mul, r, r))) ** 2

    for iteration in range(1, max_iterations + 1):
        gradient = [2 * (sum(map(mul, row, x)) - c) for row, c in zip(HtH, Htb)]
        current_obj = objective(x)

        # Backtracking: the projected step, halved until it improves.
        step_size = alpha
        for _ in range(CLS_LINE_SEARCH):
            # max(0, min(1, x - step * gradient))
            x_new = [0.0 if v < 0.0 else 1.0 if v > 1.0 else v
                     for v in [xv - step_size * gv for xv, gv in zip(x, gradient)]]
            new_obj = objective(x_new)
            if new_obj < current_obj:
                break
            step_size = step_size * beta

        x = x_new

        if norm2([a - p for a, p in zip(x, x_prev)]) < tolerance:
            break

        # Grow the step after an improving one, else shrink it.
        if iteration > 1 and new_obj < current_obj:
            alpha = min(alpha * CLS_STEP_GROWTH, CLS_STEP_CAP)
        else:
            alpha = alpha * CLS_STEP_SHRINK

        x_prev = x
    return x


def check_thrust_decay(PWMs, decay_per_valve):
    """Port of check_thrust_decay.

    The Teensy opens every valve at the start of the PWM period and
    closes each after its duty cycle. With n valves open at once each
    gives max(0, 1 - decay_per_valve (n - 1)) of its thrust alone.

    Args:
        PWMs: The 8 duty cycles.
        decay_per_valve: The thrust lost per extra valve open.

    Returns:
        (each thruster's share of its thrust alone over one period,
        8; how many are on).
    """
    # Duty cycles in [0, 1].
    if max(PWMs) > 1:
        top = max(PWMs)
        PWMs = [p / top for p in PWMs]
    duty = [max(p, 0.0) for p in PWMs]
    count = sum(1 for p in duty if p > 0)

    # Each open thruster: its thrust share over the intervals between
    # valve closings, each with the number of valves open then.
    closing = sorted(duty)
    thrust_decay_factor = [1.0] * NUM_THRUSTERS
    for i in range(NUM_THRUSTERS):
        if duty[i] > 0:
            share, t0 = 0.0, 0.0
            for k in range(NUM_THRUSTERS):
                t1 = min(closing[k], duty[i])
                if t1 > t0:
                    n_open = sum(1 for d in duty if d >= t1)   # open from t0 to t1
                    share += (t1 - t0) * max(0.0, 1 - decay_per_valve * (n_open - 1))
                    t0 = t1
            thrust_decay_factor[i] = share / duty[i]
    return thrust_decay_factor, count


def optimize_duty_cycle_with_decay(u_desired, x0, thruster_dist2CG, F_thrusters, max_iters,
                                   tol, thrust_decay):
    """Port of optimize_duty_cycle_with_decay ("MATLAB Function5").

    Fits the duty cycles, recomputes each thruster's decay from them,
    and repeats until they settle.

    Returns:
        (H_final, duty_cycles).
    """
    duty_cycles = list(x0)
    thrust_decay_factor = [1.0] * NUM_THRUSTERS
    prev_duty_cycles = [d + 2 * tol for d in duty_cycles]

    for _ in range(max_iters):
        H = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor)
        duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles)
        thrust_decay_factor, _count = check_thrust_decay(duty_cycles, thrust_decay)
        if max(abs(a - b) for a, b in zip(duty_cycles, prev_duty_cycles)) < tol:
            break
        prev_duty_cycles = duty_cycles
    H_final = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=False)
    return H_final, duty_cycles


def thrusts(F):
    """Return the 8 thrusts from F: a list of 8, or one for all."""
    return [float(f) for f in F] if isinstance(F, (list, tuple)) else [float(F)] * NUM_THRUSTERS


def duty_cycles(u, theta, thruster_dist2CG, F, max_iters=DEFAULTS["max_iters"],
                tol=DEFAULTS["tol"], thrust_decay=C.THRUST_DECAY_DEFAULT):
    """Port of spot.control.thrusterDutyCycles.

    Args:
        u: The table-frame command [Fx Fy Tz].
        theta: The platform's attitude [rad].
        thruster_dist2CG: The 8 moment arms [mm].
        F: The thrusters' thrust [N] (8, or one for all).
        max_iters, tol: The mixer's mask values (the diagram's by
            default).
        thrust_decay: Thrust lost per extra valve open (the default
            until a platform's Together calibration).

    Returns:
        (duty, Fb, H): the 8 duty cycles in [0, 1], the command in the
        body frame, and the 3x8 map from duty cycles to body forces
        (with thrust decay).
    """
    F_thrusters = thrusts(F)

    # 1. Rotate Forces to Body.
    C_bI = rotate_frame(theta)
    Fb = matvec(C_bI, u[:2]) + [u[2]]

    # 2. Generate Initial Guess.
    x0 = matvec(pinv_full_row_rank(make_h(thruster_dist2CG, F_thrusters)), Fb)

    # 3. MATLAB Function5.
    H, duty = optimize_duty_cycle_with_decay(Fb, x0, thruster_dist2CG, F_thrusters, max_iters,
                                             tol, thrust_decay)
    return duty, Fb, H


# One platform


class Controller:
    """The diagram's default controller for one platform.

    Stepped every baseRate (1/sample_rate), as the diagram runs its LQR
    law and its thruster code: the LQR law (the desired pose's
    derivative taken over the last step, as the block's Discrete
    Derivative does), then the mixer.
    """

    def __init__(self, platform, params=None):
        """Set up a platform's controller.

        Args:
            platform: "RED", "BLACK" or "BLUE".
            params: Parameters shaped like DEFAULTS; None for DEFAULTS.
        """
        self.platform = platform
        d = params or DEFAULTS
        self.params = dict(d["platforms"][platform])
        self.sample_rate = float(d["sample_rate"])
        self.max_iters, self.tol = int(d["max_iters"]), float(d["tol"])
        self.period = 1.0 / self.sample_rate   # [s] between steps: baseRate
        self.K = platform_gains(self.params, float(d["data_rate"]))
        self._previous = None              # the desired pose one step earlier

    def step(self, measured, desired):
        """Run one step.

        Args:
            measured: [x y theta xdot ydot thetadot] (theta wrapped).
            desired: [x y theta].

        Returns:
            (u in the table frame, the duty cycles in [0, 1], the
            command in the body frame).
        """
        # The diagram's Discrete Derivative starts from 0 (a jump at
        # its first step); here it starts from the first desired pose,
        # so engaging doesn't kick the platform.
        previous = self._previous or list(desired)
        rate = [(d - p) / self.period for d, p in zip(desired, previous)]
        self._previous = list(desired)

        # LQR law, then the mixer.
        u = lqr_command(self.K, measured, desired, rate)
        p = self.params
        duty, Fb, _H = duty_cycles(u, measured[2], p["thruster_dist2CG"], p["thrust"],
                                   self.max_iters, self.tol, p["thrust_decay"])
        return u, duty, Fb


def serve(conn, platform, params=None):
    """Run one platform's Controller in a process of its own.

    interactive.py starts one per platform flown, so their mixers run
    side by side on separate CPU cores. Answers each (measured,
    desired) received on the connection with Controller.step's result,
    until None arrives or the other end closes.

    Args:
        conn: A multiprocessing connection.
        platform: "RED", "BLACK" or "BLUE".
        params: As Controller takes them; None for DEFAULTS.
    """
    controller = Controller(platform, params)
    conn.send("ready")
    try:
        while True:
            msg = conn.recv()
            if msg is None:
                break
            conn.send(controller.step(*msg))
    except (EOFError, OSError):
        pass   # the console has gone


# For the MATLAB check


def main():
    """Answer spot.control.verifyConsoleController's cases.

    Reads cases (JSON) on standard input:

        {"gains": [{"mass", "inertia", "Ts", "Q", "R"}, ...],
         "lqr": [{"K", "measured", "desired", "rate"}, ...],
         "mixer": [{"u", "theta", "dist", "F" (8), "max_iters",
                    "tol", "decay"}, ...]}

    and writes the results (JSON) on standard output:

        {"gains": [K, ...], "lqr": [u, ...],
         "mixer": [{"duty", "Fb", "H"}, ...], "defaults": DEFAULTS,
         "default_gains": {PLATFORM: K}}
    """
    cases = json.load(sys.stdin)
    out = {"gains": [], "lqr": [], "mixer": [], "defaults": DEFAULTS,
           "default_gains": {n: platform_gains(p, DEFAULTS["data_rate"])
                             for n, p in DEFAULTS["platforms"].items()}}

    # Each kind of case in turn.
    for c in cases.get("gains", []):
        A, B = state_space(c["mass"], c["inertia"], c["Ts"])
        out["gains"].append(lqr_gains(A, B, c["Q"], c["R"]))
    for c in cases.get("lqr", []):
        out["lqr"].append(lqr_command(c["K"], c["measured"], c["desired"], c["rate"]))
    for c in cases.get("mixer", []):
        duty, Fb, H = duty_cycles(c["u"], c["theta"], c["dist"], c["F"], c["max_iters"],
                                  c["tol"], c["decay"])
        out["mixer"].append({"duty": duty, "Fb": Fb, "H": H})
    json.dump(out, sys.stdout)


if __name__ == "__main__":
    main()
