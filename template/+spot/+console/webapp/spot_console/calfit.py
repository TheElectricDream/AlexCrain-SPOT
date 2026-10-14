"""The fits behind the Calibration tab (calibration.py): the table's slope from platforms drifting on
their air bearings, and each thruster's thrust from short burns.

Both fit the measured positions themselves rather than accelerations worked out from them (PhaseSpace's
positions are good to a fraction of a millimetre, but differentiating them twice amplifies that noise a
lot). Over a stretch of samples starting at t0, a platform's position is

    x(t) = x(t0) + vx(t0) (t - t0) + double integral of its acceleration from t0 to t

and the accelerations here are linear in the unknowns once the measured path is put in, so every fit
is linear least squares: each sample gives one row, the unknowns are the model's parameters and each
stretch's starting position and velocity.

The table. Its top is a smooth surface of height h(x, y) [m]; a floating platform accelerates downhill
at -g grad h, and its air bearings drag it with -c v (c [1/s] per platform). With u = x - xc and
v = y - yc (the table's centre, CENTRE),

    h = a1 u + a2 v + a3 u^2 + a4 u v + a5 v^2       (the "quadratic" model; "plane": a3 = a4 = a5 = 0)
    ax = -g (a1 + 2 a3 u + a4 v) - c vx,   ay = -g (a2 + a4 u + 2 a5 v) - c vy

A thruster. During a burn the platform (mass m, inertia I, known) is pushed by a force fixed in its
body, F_b = [Fbx, Fby], and turned by a torque Tz:

    a = R(theta) F_b / m + (the table's pull, known from its calibration),   alpha = Tz / I

R(theta) uses the measured attitude, so the burn's own turning is accounted for. The thrust along the
thruster's nominal direction d, F_b . d, is its calibrated thrust; the part across it gives how far
the thrust is turned from d; Tz against the torque the mixer expects from it (dist2CG * thrust) checks
the assumed CG and inertia.

Part of the SPOT Console (webapp/app.py starts it). Python 3.6 standard library only, and nothing else
from the console, so the fits can be checked on their own.
"""

import math


G = 9.80665                 # [m/s^2]
CENTRE = (1.75, 1.2)        # [m] the table's centre (the slope's u, v are measured from here)
TABLE = (3.5, 2.4)          # [m] the table top (as the 3D view draws it)

# Thruster directions in the body frame (control.py's VEC1, VEC2): 1, 2 push along -x, 5, 6 along +x,
# 3, 4 along +y, 7, 8 along -y
THRUSTER_DIRECTIONS = [(-1, 0), (-1, 0), (0, 1), (0, 1), (1, 0), (1, 0), (0, -1), (0, -1)]


# ---------------------------------------------------------------------------- least squares

class LeastSquares:
    """min ||A p - b||^2, built one row at a time (normal equations, with the columns scaled so that
    unknowns of very different sizes, metres and micro-radians, are solved for equally well)."""

    def __init__(self, n):
        self.n = n
        self.AtA = [[0.0] * n for _ in range(n)]
        self.Atb = [0.0] * n
        self.btb = 0.0
        self.rows = 0

    def add(self, row, b):
        """ROW: {column: coefficient} (the other columns are 0); B: the measured value."""
        items = list(row.items())
        for i, ai in items:
            self.Atb[i] += ai * b
            r = self.AtA[i]
            for j, aj in items:
                r[j] += ai * aj
        self.btb += b * b
        self.rows += 1

    def solve(self):
        """(p, rms residual, standard deviations of p). Columns never used come out as 0."""
        n = self.n
        scale = [math.sqrt(self.AtA[i][i]) or 1.0 for i in range(n)]
        M = [[self.AtA[i][j] / (scale[i] * scale[j]) for j in range(n)] for i in range(n)]
        rhs = [self.Atb[i] / scale[i] for i in range(n)]
        inv = invert(M)
        q = [sum(inv[i][j] * rhs[j] for j in range(n)) for i in range(n)]
        p = [q[i] / scale[i] for i in range(n)]
        # residual: |b|^2 - 2 p.A'b + p'A'A p
        ssr = self.btb - 2 * sum(p[i] * self.Atb[i] for i in range(n)) + \
            sum(p[i] * sum(self.AtA[i][j] * p[j] for j in range(n)) for i in range(n))
        ssr = max(ssr, 0.0)
        dof = max(self.rows - n, 1)
        sigma2 = ssr / dof
        sd = [math.sqrt(max(inv[i][i], 0.0) * sigma2) / scale[i] for i in range(n)]
        return p, math.sqrt(ssr / max(self.rows, 1)), sd


def invert(M):
    """The inverse of a symmetric positive (semi-)definite matrix; a zero row and column (an unused
    unknown) stays zero rather than making it singular."""
    n = len(M)
    A = [list(M[i]) + [1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
    for c in range(n):
        p = max(range(c, n), key=lambda r: abs(A[r][c]))
        if abs(A[p][c]) < 1e-14:
            A[c][c] = 1.0   # unused: leave it at 0
            A[c][n + c] = 0.0
            continue
        A[c], A[p] = A[p], A[c]
        pivot = A[c][c]
        A[c] = [v / pivot for v in A[c]]
        for r in range(n):
            if r != c and A[r][c] != 0.0:
                f = A[r][c]
                A[r] = [v - f * w for v, w in zip(A[r], A[c])]
    return [row[n:] for row in A]


# ---------------------------------------------------------------------------- integrals of samples

def cumulative(t, f):
    """The running integral of the samples F over the times T (trapezoids), starting at 0."""
    out = [0.0]
    for k in range(1, len(t)):
        out.append(out[-1] + 0.5 * (f[k] + f[k - 1]) * (t[k] - t[k - 1]))
    return out


def double_integral(t, f):
    return cumulative(t, cumulative(t, f))


def unwrap(angles):
    out = []
    for a in angles:
        if out:
            a = out[-1] + math.atan2(math.sin(a - out[-1]), math.cos(a - out[-1]))
        out.append(a)
    return out


# ---------------------------------------------------------------------------- the table

def slope_acceleration(coeffs, x, y, centre=CENTRE):
    """The table's pull [ax, ay] [m/s^2] at (x, y) for height coefficients [a1 .. a5]."""
    a1, a2, a3, a4, a5 = coeffs
    u, v = x - centre[0], y - centre[1]
    return -G * (a1 + 2 * a3 * u + a4 * v), -G * (a2 + a4 * u + 2 * a5 * v)


def height(coeffs, x, y, centre=CENTRE):
    a1, a2, a3, a4, a5 = coeffs
    u, v = x - centre[0], y - centre[1]
    return a1 * u + a2 * v + a3 * u * u + a4 * u * v + a5 * v * v


def fit_table(drifts, model=None, min_samples=20):
    """Fit the table's slope to DRIFTS: [{"platform", "t", "x", "y"}] (each a list, s and m), each
    the record of one platform floating with its thrusters off. MODEL: "quadratic", "plane", or None
    to choose (quadratic only with at least 3 drifts that between them cover most of the table).
    Returns the fit (see the module's help), with "model", "height" [a1 .. a5], "sigma" (their
    standard deviations), "damping" {platform: c [1/s]}, "damping_fixed" (the platforms whose drag
    the drifts couldn't tell from the slope: short drifts make the two alike, and the fit then finds
    a drag below 0, which no air bearing has; theirs is fixed at 0 and the slope fitted again),
    "rms_mm" (position residual), the tilt ("tilt_centre_mrad", "tilt_max_mrad", "downhill_deg":
    the downhill direction at the centre, from +x towards +y), "coverage" and "drifts" (how many)."""
    drifts = [d for d in drifts if len(d["t"]) >= min_samples]
    if not drifts:
        raise ValueError("no drift long enough to fit (each needs at least %d samples)" % min_samples)
    xs = [v for d in drifts for v in d["x"]]
    ys = [v for d in drifts for v in d["y"]]
    coverage = {"x": [min(xs), max(xs)], "y": [min(ys), max(ys)]}
    if model is None:
        wide = coverage["x"][1] - coverage["x"][0] >= 0.4 * TABLE[0] and coverage["y"][1] - coverage["y"][0] >= 0.4 * TABLE[1]
        model = "quadratic" if len(drifts) >= 3 and wide else "plane"
    if model not in ("quadratic", "plane"):
        raise ValueError("model must be quadratic or plane")
    nslope = 5 if model == "quadratic" else 2
    platforms = sorted(set(d["platform"] for d in drifts))
    fixed = []
    while True:
        p, rms, sd = _fit_table(drifts, nslope, platforms, fixed)
        damping = dict((n, p[nslope + i]) for i, n in enumerate(platforms))
        negative = [n for n in platforms if n not in fixed and damping[n] < 0]
        if not negative:
            break
        fixed += negative
    coeffs = list(p[:nslope]) + [0.0] * (5 - nslope)
    sigma = list(sd[:nslope]) + [0.0] * (5 - nslope)
    return dict(model=model, height=coeffs, sigma=sigma, damping=damping, damping_fixed=fixed, rms_mm=1000 * rms,
                coverage=coverage, drifts=len(drifts), centre=list(CENTRE), g=G, **tilt(coeffs))


def _fit_table(drifts, nslope, platforms, fixed):
    """The least squares of fit_table; the drag of the platforms in FIXED is 0 (its column unused)."""
    ncols = nslope + len(platforms) + 4 * len(drifts)
    ls = LeastSquares(ncols)
    for k, d in enumerate(drifts):
        t, x, y = d["t"], d["x"], d["y"]
        u = [v - CENTRE[0] for v in x]
        w = [v - CENTRE[1] for v in y]
        J1 = [0.5 * (tk - t[0]) ** 2 for tk in t]
        Ju, Jw = double_integral(t, u), double_integral(t, w)
        Dx = cumulative(t, [v - x[0] for v in x])
        Dy = cumulative(t, [v - y[0] for v in y])
        c = nslope + platforms.index(d["platform"])
        free = d["platform"] not in fixed
        base = nslope + len(platforms) + 4 * k   # x0, vx0, y0, vy0 of this drift
        for i in range(len(t)):
            tau = t[i] - t[0]
            rx = {0: -G * J1[i], base: 1.0, base + 1: tau}
            ry = {1: -G * J1[i], base + 2: 1.0, base + 3: tau}
            if free:
                rx[c] = -Dx[i]
                ry[c] = -Dy[i]
            if nslope == 5:
                rx.update({2: -G * 2 * Ju[i], 3: -G * Jw[i]})
                ry.update({3: -G * Ju[i], 4: -G * 2 * Jw[i]})
            ls.add(rx, x[i])
            ls.add(ry, y[i])
    return ls.solve()


def tilt(coeffs, n=36):
    """The slope's size: at the table's centre and the largest over the table [mrad], and the downhill
    direction at the centre [deg, from +x towards +y]."""
    def grad(x, y):
        ax, ay = slope_acceleration(coeffs, x, y)
        return -ax / G, -ay / G
    gx, gy = grad(*CENTRE)
    worst = 0.0
    for i in range(n + 1):
        for j in range(n + 1):
            hx, hy = grad(TABLE[0] * i / n, TABLE[1] * j / n)
            worst = max(worst, math.hypot(hx, hy))
    return {"tilt_centre_mrad": 1000 * math.hypot(gx, gy), "tilt_max_mrad": 1000 * worst,
            "downhill_deg": math.degrees(math.atan2(-gy, -gx))}


# ---------------------------------------------------------------------------- a thruster

def fit_burn(t, x, y, theta, mass, inertia, thruster, dist2CG, slope=None, damping=0.0):
    """Fit one burn: samples T, X, Y, THETA (s, m, rad) while thruster THRUSTER (1-8) fires, of a
    platform of MASS [kg] and INERTIA [kg m^2] whose thruster has the moment arm DIST2CG [mm] (the
    mixer's thruster_dist2CG). SLOPE: the table's height coefficients (its pull is taken out), or None
    for a level table; DAMPING: the air bearings' c [1/s].
    Returns {"thrust" [N] along the nominal direction, "across" [N], "angle" [deg] (how far the thrust is
    turned from nominal, towards the body's +z rotation), "torque" [N m] (measured), "torque_expected"
    (dist2CG * thrust), "torque_ratio", "rms_mm", "samples"}."""
    if len(t) < 8:
        raise ValueError("too few samples in the burn (%d)" % len(t))
    th = unwrap(theta)
    C = double_integral(t, [math.cos(a) for a in th])
    S = double_integral(t, [math.sin(a) for a in th])
    known_x = [0.0] * len(t)
    known_y = [0.0] * len(t)
    if slope is not None:
        pulls = [slope_acceleration(slope, xi, yi) for xi, yi in zip(x, y)]
        known_x = double_integral(t, [p[0] for p in pulls])
        known_y = double_integral(t, [p[1] for p in pulls])
    if damping:
        dx = cumulative(t, [v - x[0] for v in x])
        dy = cumulative(t, [v - y[0] for v in y])
        known_x = [k - damping * d for k, d in zip(known_x, dx)]
        known_y = [k - damping * d for k, d in zip(known_y, dy)]
    # x = x0 + vx0 tau + (C Fbx - S Fby)/m + known;  y = y0 + vy0 tau + (S Fbx + C Fby)/m + known
    ls = LeastSquares(6)   # Fbx, Fby, x0, vx0, y0, vy0
    for i in range(len(t)):
        tau = t[i] - t[0]
        ls.add({0: C[i] / mass, 1: -S[i] / mass, 2: 1.0, 3: tau}, x[i] - known_x[i])
        ls.add({0: S[i] / mass, 1: C[i] / mass, 4: 1.0, 5: tau}, y[i] - known_y[i])
    p, rms, sd = ls.solve()
    Fbx, Fby = p[0], p[1]
    # theta = theta0 + w0 tau + (Tz / I) tau^2 / 2
    la = LeastSquares(3)
    for i in range(len(t)):
        tau = t[i] - t[0]
        la.add({0: 1.0, 1: tau, 2: 0.5 * tau * tau / inertia}, th[i])
    q, _, qsd = la.solve()
    dxn, dyn = THRUSTER_DIRECTIONS[thruster - 1]
    along = Fbx * dxn + Fby * dyn
    across = -Fbx * dyn + Fby * dxn          # along d rotated +90 deg
    expected = dist2CG / 1000.0 * along
    return {"thrust": along, "across": across, "angle": math.degrees(math.atan2(across, along)),
            "torque": q[2], "torque_expected": expected,
            "torque_ratio": q[2] / expected if abs(expected) > 1e-9 else None,
            "thrust_sd": math.hypot(sd[0] * dxn, sd[1] * dyn), "rms_mm": 1000 * rms, "samples": len(t)}


def summarise(values):
    """(mean, standard deviation) of a list (sd 0 for one value)."""
    if not values:
        return None, None
    m = sum(values) / len(values)
    if len(values) < 2:
        return m, 0.0
    return m, math.sqrt(sum((v - m) ** 2 for v in values) / (len(values) - 1))
