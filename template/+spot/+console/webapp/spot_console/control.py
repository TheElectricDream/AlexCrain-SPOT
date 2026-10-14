"""The template diagram's default controller, in plain Python, for the lab console's Interactive
mode (interactive.py), which has no MATLAB to run it with.

Each function is a line-by-line port of MATLAB code in +spot/+control, which in turn holds
verbatim copies of the diagram's blocks:

    lqr_gains     spot.control.stateSpace + spot.control.lqrGains (MATLAB's dlqr)
    lqr_command   spot.control.lqrCommand          "4 - Control/Default LQR Control (<PLATFORM>)"
    duty_cycles   spot.control.thrusterDutyCycles  "Thruster & Float Control Code/Change <PLATFORM> Behavior"

The MATLAB names are kept in the comments so the two can be read side by side.
spot.control.verifyConsoleController (in MATLAB) checks that this file gives the same gains, forces
and duty cycles as MATLAB, and that DEFAULTS are SPOT's default settings; it runs this file as a
script (see main() at the end).

Python 3.6 standard library only, and nothing else from the console, so it can be read and run on
its own.
"""

import json
import math
import sys
from operator import mul


# ---------------------------------------------------------------------------- parameters

# SPOT's default settings (spot.params.defaults), what spot.massprops.compute makes of their mass
# section, and the mixer block's mask values in the diagram. The lab console has no settings file
# of the project's, so Interactive Mode uses these; spot.control.verifyConsoleController checks
# them against MATLAB.
DEFAULTS = {
    # The diagram runs every baseRate = 1/sample_rate seconds, its LQR and its thruster code
    # included; its LQR gains are designed for the PhaseSpace period, dataRate = 1/data_rate
    # (spot.params.apply)
    "sample_rate": 20.0,                  # [Hz] s.sampleRate
    "data_rate": 10.0,                    # [Hz] s.dataRate
    # "Calculate Duty Cycle/MATLAB Function5" mask: Maximum Number of Iterations, Convergence Tolerance
    "max_iters": 20,
    "tol": 1e-6,
    "platforms": {
        # mass [kg], inertia [kg m^2] and thruster_dist2CG [mm] from the default scale readings and
        # bifilar test; thrust [N] of one thruster (s.thrusterForce; a list of 8: each thruster's, as
        # the lab's thruster calibration gives them, calibration.py); LQR weights
        # Q = [x y theta xdot ydot thetadot], R = [Fx Fy Tz] (s.control.<PLATFORM>.lqr)
        "RED": {
            "mass": 13.75, "inertia": 0.23543476058293455, "thrust": 0.2825,
            "thruster_dist2CG": [84.381818181818176, -53.618181818181817, 54.381818181818176, -83.61818181818181,
                                 53.618181818181817, -84.381818181818176, 83.61818181818181, -54.381818181818176],
            "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
        },
        "BLACK": {
            "mass": 13.219999999999999, "inertia": 0.24326386911004549, "thrust": 0.2825,
            "thruster_dist2CG": [79.665658093797262, -58.334341906202717, 58.334341906202717, -79.665658093797262,
                                 58.334341906202717, -79.665658093797262, 79.665658093797262, -58.334341906202717],
            "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
        },
        "BLUE": {
            "mass": 12.77, "inertia": 0.22157979686198798, "thrust": 0.2825,
            "thruster_dist2CG": [86.267032106499599, -51.732967893500387, 57.723570869224737, -80.276429130775242,
                                 51.732967893500387, -86.267032106499599, 80.276429130775242, -57.723570869224737],
            "Q": [1, 1, 0.05, 10, 10, 1], "R": [6, 6, 6],
        },
    },
}


# ---------------------------------------------------------------------------- small matrices
# Lists of rows; vectors are plain lists.

def zeros(rows, cols):
    return [[0.0] * cols for _ in range(rows)]


def eye(n):
    return [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]


def diag(values):
    m = zeros(len(values), len(values))
    for i, v in enumerate(values):
        m[i][i] = float(v)
    return m


def transpose(A):
    return [list(col) for col in zip(*A)]


def matmul(A, B):
    Bt = transpose(B)
    return [[sum(map(mul, row, col)) for col in Bt] for row in A]


def matvec(A, x):
    return [sum(map(mul, row, x)) for row in A]


def add(A, B):
    return [[a + b for a, b in zip(ra, rb)] for ra, rb in zip(A, B)]


def solve(A, B):
    """A \\ B for a square, invertible A (Gauss-Jordan elimination with partial pivoting)."""
    n = len(A)
    M = [list(map(float, A[i])) + list(map(float, B[i])) for i in range(n)]
    for c in range(n):
        p = max(range(c, n), key=lambda r: abs(M[r][c]))
        if M[p][c] == 0.0:
            raise ValueError("singular matrix")
        M[c], M[p] = M[p], M[c]
        pivot = M[c][c]
        M[c] = [v / pivot for v in M[c]]
        for r in range(n):
            if r != c and M[r][c] != 0.0:
                f = M[r][c]
                M[r] = [v - f * w for v, w in zip(M[r], M[c])]
    return [row[n:] for row in M]


def norm2(v):
    return math.sqrt(sum(map(mul, v, v)))


# ---------------------------------------------------------------------------- LQR gains

def state_space(mass, inertia, Ts):
    """spot.control.stateSpace: the zero-order-hold discretisation (sample time Ts [s]) of the
    double integrator with states [x y theta xdot ydot thetadot] and inputs [Fx Fy Tz].
    (MATLAB's c2d computes it with a matrix exponential; for a double integrator that is exactly
    A = [I Ts*I; 0 I], B = [Ts^2/2 * Minv; Ts * Minv].)"""
    Minv = [1.0 / mass, 1.0 / mass, 1.0 / inertia]
    A = eye(6)
    B = zeros(6, 3)
    for i in range(3):
        A[i][i + 3] = Ts
        B[i][i] = Ts * Ts / 2 * Minv[i]
        B[i + 3][i] = Ts * Minv[i]
    return A, B


def lqr_gains(A, B, Q, R, max_iterations=100):
    """spot.control.lqrGains: K = dlqr(A, B, diag(Q), diag(R)), the 3x6 gain of u = -K*x that
    minimises sum(x'Qx + u'Ru). P solves the discrete algebraic Riccati equation
        P = Q + A'PA - A'PB (R + B'PB)^-1 B'PA,
    and K = (R + B'PB)^-1 B'PA. MATLAB finds P with a Schur method; here it is found with the
    structure-preserving doubling algorithm (each step doubles the horizon, so it reaches machine
    precision in a few dozen steps): from A0 = A, G0 = B R^-1 B', H0 = Q, with W = I + Gk Hk,
        Ak+1 = Ak W^-1 Ak,   Gk+1 = Gk + Ak W^-1 Gk Ak',   Hk+1 = Hk + Ak' Hk W^-1 Ak,
    and Hk -> P."""
    n = len(A)
    Ak, Hk = A, diag(Q)
    Gk = matmul(B, solve(diag(R), transpose(B)))
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
    P, Rm, Bt = Hk, diag(R), transpose(B)
    return solve(add(Rm, matmul(Bt, matmul(P, B))), matmul(Bt, matmul(P, A)))


def platform_gains(params, data_rate):
    """K_<PLATFORM> as spot.params.apply makes it: the model at the period dataRate = 1/data_rate."""
    A, B = state_space(params["mass"], params["inertia"], 1.0 / data_rate)
    return lqr_gains(A, B, params["Q"], params["R"])


# ---------------------------------------------------------------------------- the control law

def matlab_mod(a, m):
    """MATLAB's mod(a, m) for m > 0: the result has the sign of m."""
    return a - math.floor(a / m) * m


def smooth_angular_difference(Rz_path, Rz_measured):
    """SmoothAngularDifference ("Calculate Error"): the attitude error wrapped to [-pi, pi)."""
    return matlab_mod(Rz_measured - Rz_path + math.pi, 2 * math.pi) - math.pi


def lqr_command(K, measured, desired, desired_rate):
    """spot.control.lqrCommand: u = -K*e = [Fx, Fy, Tz] in the table frame, with
    measured = [x y theta xdot ydot thetadot], desired = [x y theta] (*_Path) and desired_rate its
    derivative (the block's Discrete Derivative)."""
    e = [measured[0] - desired[0],
         measured[1] - desired[1],
         smooth_angular_difference(desired[2], measured[2]),
         measured[3] - desired_rate[0],
         measured[4] - desired_rate[1],
         measured[5] - desired_rate[2]]
    return [-v for v in matvec(K, e)]


# ---------------------------------------------------------------------------- the thruster mixer
# Thruster directions: 1, 2 push along -x, 5, 6 along +x, 3, 4 along +y, 7, 8 along -y (body frame)
VEC1 = [-1, -1, 0, 0, 1, 1, 0, 0]  # X
VEC2 = [0, 0, 1, 1, 0, 0, -1, -1]  # Y


def rotate_frame(Rz):
    """RotateFrame ("Rotate Forces to Body/Create Rotation Matrix"): C_bI."""
    return [[math.cos(Rz), math.sin(Rz)],
            [-math.sin(Rz), math.cos(Rz)]]


def make_h(thruster_dist2CG, F_thrusters):
    """MakeH ("Generate Initial Guess/MATLAB Function"): [Vec1, Vec2, dist/1000]' * diag(F/2)."""
    Mat1 = [VEC1, VEC2, [d / 1000 for d in thruster_dist2CG]]
    return matmul(Mat1, diag([f / 2 for f in F_thrusters]))


def make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=True):
    """MakeHWithDecay (halve=True: diag(F/2)) and MakeHWithDecayRecalculateForce (halve=False:
    diag(F)), with every thruster's thrust (F_thrusters, 8) scaled by the decay factor."""
    adjusted_dist = [d / 1000 for d in thruster_dist2CG]
    Mat1 = [VEC1, VEC2, adjusted_dist]
    F_decayed = [f * thrust_decay_factor for f in F_thrusters]
    Mat2 = diag([f / 2 for f in F_decayed] if halve else F_decayed)
    return matmul(Mat1, Mat2)


def pinv_full_row_rank(H):
    """pinv(H) for a 3x8 H of rank 3 (the DSP Pseudoinverse block): H' (H H')^-1."""
    Ht = transpose(H)
    return transpose(solve(matmul(H, Ht), H))   # (H H')^-1 H, transposed; H H' is symmetric


def custom_constrained_least_squares(H, b, x0):
    """custom_constrained_least_squares: min ||H*x - b||^2 with 0 <= x <= 1, by projected gradient
    descent with a backtracking line search (the same steps and constants as the block)."""
    max_iterations = 100
    tolerance = 1e-6
    alpha = 0.01  # initial step size
    beta = 0.5    # backtracking factor

    x = list(x0)
    x_prev = [v + 2 * tolerance for v in x0]

    HtH = matmul(transpose(H), H)
    Htb = matvec(transpose(H), b)

    def objective(v):  # norm(H*v - b)^2
        r = [sum(map(mul, row, v)) - bv for row, bv in zip(H, b)]
        return math.sqrt(sum(map(mul, r, r))) ** 2

    for iteration in range(1, max_iterations + 1):
        gradient = [2 * (sum(map(mul, row, x)) - c) for row, c in zip(HtH, Htb)]
        current_obj = objective(x)

        step_size = alpha
        for _ in range(20):
            x_new = [0.0 if v < 0.0 else 1.0 if v > 1.0 else v
                     for v in [xv - step_size * gv for xv, gv in zip(x, gradient)]]   # max(0, min(1, .))
            new_obj = objective(x_new)
            if new_obj < current_obj:
                break
            step_size = step_size * beta

        x = x_new

        if norm2([a - p for a, p in zip(x, x_prev)]) < tolerance:
            break

        if iteration > 1 and new_obj < current_obj:
            alpha = min(alpha * 1.1, 0.1)
        else:
            alpha = alpha * 0.9

        x_prev = x
    return x


def check_thrust_decay(PWMs):
    """check_thrust_decay: the thrust decay factor for these duty cycles, and how many are on."""
    if max(PWMs) > 1:
        top = max(PWMs)
        PWMs = [p / top for p in PWMs]
    count = sum(1 for p in PWMs if p > 0)
    avg_duty = sum(max(p, 0) for p in PWMs) / max(count, 1)
    if avg_duty < 0.3 or count == 0:
        thrust_decay_factor = 1
    else:
        thrust_decay_factor = max(0.6 - 2 * avg_duty + 1, 0.5)
    return thrust_decay_factor, count


def optimize_duty_cycle_with_decay(u_desired, x0, thruster_dist2CG, F_thrusters, max_iters, tol):
    """optimize_duty_cycle_with_decay ("MATLAB Function5"): returns (H_final, duty_cycles)."""
    duty_cycles = list(x0)
    thrust_decay_factor = 1
    prev_duty_cycles = [d + 2 * tol for d in duty_cycles]

    for _ in range(max_iters):
        H = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor)
        duty_cycles = custom_constrained_least_squares(H, u_desired, duty_cycles)
        thrust_decay_factor, _count = check_thrust_decay(duty_cycles)
        if max(abs(a - b) for a, b in zip(duty_cycles, prev_duty_cycles)) < tol:
            break
        prev_duty_cycles = duty_cycles
    H_final = make_h_with_decay(thruster_dist2CG, F_thrusters, thrust_decay_factor, halve=False)
    return H_final, duty_cycles


def thrusts(F):
    """The 8 thrusters' thrust from F: a list of 8, or one number for all eight."""
    return [float(f) for f in F] if isinstance(F, (list, tuple)) else [float(F)] * 8


def duty_cycles(u, theta, thruster_dist2CG, F, max_iters=20, tol=1e-6):
    """spot.control.thrusterDutyCycles: (duty, Fb, H) for the table-frame command u = [Fx Fy Tz]
    of a platform at attitude theta [rad] whose thrusters give F [N] (8, or one for all). duty: the
    8 duty cycles in [0, 1]; Fb: the command in the body frame; H: the 3x8 map from duty cycles to
    body forces (with thrust decay)."""
    F_thrusters = thrusts(F)
    # 1. Rotate Forces to Body
    C_bI = rotate_frame(theta)
    Fb = matvec(C_bI, u[:2]) + [u[2]]
    # 2. Generate Initial Guess
    x0 = matvec(pinv_full_row_rank(make_h(thruster_dist2CG, F_thrusters)), Fb)
    # 3. MATLAB Function5
    H, duty = optimize_duty_cycle_with_decay(Fb, x0, thruster_dist2CG, F_thrusters, max_iters, tol)
    return duty, Fb, H


# ---------------------------------------------------------------------------- one platform

class Controller:
    """The diagram's default controller for one platform, stepped every baseRate (1/sample_rate),
    as the diagram runs its LQR law and its thruster code: the LQR law (the desired pose's
    derivative taken over the last step, as the block's Discrete Derivative does), then the mixer."""

    def __init__(self, platform, params=None):
        self.platform = platform
        d = params or DEFAULTS
        self.params = dict(d["platforms"][platform])
        self.sample_rate = float(d["sample_rate"])
        self.max_iters, self.tol = int(d["max_iters"]), float(d["tol"])
        self.period = 1.0 / self.sample_rate   # [s] between steps: baseRate
        self.K = platform_gains(self.params, float(d["data_rate"]))
        self._previous = None              # the desired pose one step earlier

    def step(self, measured, desired):
        """measured = [x y theta xdot ydot thetadot] (theta wrapped), desired = [x y theta].
        Returns (u in the table frame, duty cycles in [0, 1], the command in the body frame)."""
        # The diagram's Discrete Derivative starts from 0 (a jump at its first step); here it
        # starts from the first desired pose instead, so engaging doesn't kick the platform
        previous = self._previous or list(desired)
        rate = [(d - p) / self.period for d, p in zip(desired, previous)]
        self._previous = list(desired)
        u = lqr_command(self.K, measured, desired, rate)
        p = self.params
        duty, Fb, _H = duty_cycles(u, measured[2], p["thruster_dist2CG"], p["thrust"], self.max_iters, self.tol)
        return u, duty, Fb


def serve(conn, platform, params=None):
    """Run one platform's Controller (with PARAMS, as Controller takes them; None: DEFAULTS) in a
    process of its own (interactive.py starts one per platform flown, so their mixers run side by
    side on separate CPU cores): answer each (measured, desired) received on the connection CONN
    with Controller.step's result, until None arrives or the other end closes."""
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


# ---------------------------------------------------------------------------- for the MATLAB check

def main():
    """Read cases (JSON) on standard input and write the results (JSON) on standard output:
       {"gains": [{"mass", "inertia", "Ts", "Q", "R"}, ...],
        "lqr": [{"K", "measured", "desired", "rate"}, ...],
        "mixer": [{"u", "theta", "dist", "F" (8), "max_iters", "tol"}, ...]}
    -> {"gains": [K, ...], "lqr": [u, ...], "mixer": [{"duty", "Fb", "H"}, ...], "defaults": DEFAULTS,
        "default_gains": {PLATFORM: K}}"""
    cases = json.load(sys.stdin)
    out = {"gains": [], "lqr": [], "mixer": [], "defaults": DEFAULTS,
           "default_gains": {n: platform_gains(p, DEFAULTS["data_rate"]) for n, p in DEFAULTS["platforms"].items()}}
    for c in cases.get("gains", []):
        A, B = state_space(c["mass"], c["inertia"], c["Ts"])
        out["gains"].append(lqr_gains(A, B, c["Q"], c["R"]))
    for c in cases.get("lqr", []):
        out["lqr"].append(lqr_command(c["K"], c["measured"], c["desired"], c["rate"]))
    for c in cases.get("mixer", []):
        duty, Fb, H = duty_cycles(c["u"], c["theta"], c["dist"], c["F"], c["max_iters"], c["tol"])
        out["mixer"].append({"duty": duty, "Fb": Fb, "H": H})
    json.dump(out, sys.stdout)


if __name__ == "__main__":
    main()
