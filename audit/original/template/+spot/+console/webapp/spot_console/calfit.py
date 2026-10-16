"""The fits behind the Calibration tab (calibration.py).

The table's slope, from platforms drifting on their air bearings, and
each thruster's thrust, from short burns.

Both fit the measured positions themselves rather than accelerations
worked out from them (PhaseSpace's positions are good to a fraction of
a millimetre, but differentiating them twice amplifies that noise a
lot). Over a stretch of samples starting at t0, a platform's position
is

    x(t) = x(t0) + vx(t0) (t - t0)
           + double integral of its acceleration from t0 to t

and the accelerations here are linear in the unknowns once the
measured path is put in, so every fit is linear least squares: each
sample gives one row, and the unknowns are the model's parameters and
each stretch's starting position and velocity.

The table. Its top is a smooth surface of height h(x, y) [m]; a
floating platform accelerates downhill at -g grad h, and its air
bearings drag it with -c v (c [1/s] per platform). With u = x - xc and
v = y - yc (the table's centre, CENTRE):

    h = a1 u + a2 v + a3 u^2 + a4 u v + a5 v^2
        ("quadratic" model; "plane": a3 = a4 = a5 = 0)
    ax = -g (a1 + 2 a3 u + a4 v) - c vx
    ay = -g (a2 + a4 u + 2 a5 v) - c vy

A thruster. During a burn the platform (mass m, inertia I, known) is
pushed by a force fixed in its body, F_b = [Fbx, Fby], and turned by a
torque Tz:

    a = R(theta) F_b / m + (the table's pull, from its calibration)
    alpha = Tz / I

R(theta) uses the measured attitude, so the burn's own turning is
accounted for. The thrust along the thruster's nominal direction d,
F_b . d, is its calibrated thrust; the part across it gives how far
the thrust is turned from d; Tz against the torque the mixer expects
from it (dist2CG * thrust) checks the assumed CG and inertia.

Thrusters together (the second stage). With N thrusters open at once,
each may give less than it does alone (their valves share one supply).
A firing of thrusters 1..N is fitted for F_b as above (fit_force) and
compared with what they give alone, P = the sum of each one's
calibrated thrust along its direction: the ratio
r = (F_b . P) / (P . P). Opposite thrusters cancel (all eight together
give no net force or torque), so a firing whose P is small says little
and is left out. fit_decay fits the ratios with

    r(N) = s (1 - m (N - 1))

(each valve added takes m of every one's thrust alone), s being the
single thrust that day against the calibration (near 1). The regulator
holds the supply, so the loss is the valves sharing it: the lab's
first run (Obstacle, 2026-10-14) fell in a straight line, 11.2 % per
valve, to 1.6 % (the rational s / (1 + k (N - 1)) only to 7 %; it is
still reported, as rms_rational). The mixer
(spot.control.thrusterDutyCycles) uses m: with n valves open at once
each gives 1 - m (n - 1).

Matrices and vectors keep their single capital letters (A, M, C, S,
J1, ...) so the code reads like the equations above.

Part of the SPOT Console (webapp/app.py starts it). Python 3.6
standard library only, and nothing else from the console, so the fits
can be checked on their own.
"""

import math


G = 9.80665                 # [m/s^2]
CENTRE = (1.75, 1.2)        # [m] the table's centre (u, v measured from here)
TABLE = (3.5, 2.4)          # [m] the table top (as the 3D view draws it)

# Thruster directions in the body frame (control.py's VEC1, VEC2): 1, 2
# push along -x, 5, 6 along +x, 3, 4 along +y, 7, 8 along -y.
THRUSTER_DIRECTIONS = [(-1, 0), (-1, 0), (0, 1), (0, 1), (1, 0), (1, 0), (0, -1), (0, -1)]

SINGULAR = 1e-14            # pivot below which an unknown counts as unused
MIN_BURN_SAMPLES = 8
MIN_TORQUE = 1e-9           # [N m] expected torque below which no ratio is given
MIN_PUSH = 1e-12            # [N^2] P . P below which a firing says nothing


# Least squares


class LeastSquares:
    """Least squares min ||A p - b||^2, built one row at a time.

    Uses the normal equations, with the columns scaled so that unknowns
    of very different sizes (metres and micro-radians) are solved for
    equally well.
    """

    def __init__(self, n):
        """Start an empty problem with n unknowns."""
        self.n = n
        self.AtA = [[0.0] * n for _ in range(n)]
        self.Atb = [0.0] * n
        self.btb = 0.0
        self.rows = 0

    def add(self, row, b):
        """Add one row.

        Args:
            row: {column: coefficient}; the other columns are 0.
            b: The measured value.
        """
        items = list(row.items())
        for i, ai in items:
            self.Atb[i] += ai * b
            AtA_i = self.AtA[i]
            for j, aj in items:
                AtA_i[j] += ai * aj
        self.btb += b * b
        self.rows += 1

    def solve(self):
        """Solve the problem.

        Returns:
            (p, rms residual, standard deviations of p). Columns never
            used come out as 0.
        """
        n = self.n

        # Scale the columns to unit diagonal, solve, and scale back.
        scale = [math.sqrt(self.AtA[i][i]) or 1.0 for i in range(n)]
        M = [[self.AtA[i][j] / (scale[i] * scale[j]) for j in range(n)] for i in range(n)]
        rhs = [self.Atb[i] / scale[i] for i in range(n)]
        inv = invert(M)
        q = [sum(inv[i][j] * rhs[j] for j in range(n)) for i in range(n)]
        p = [q[i] / scale[i] for i in range(n)]

        # Residual: |b|^2 - 2 p.A'b + p'A'A p.
        ssr = (self.btb - 2 * sum(p[i] * self.Atb[i] for i in range(n))
               + sum(p[i] * sum(self.AtA[i][j] * p[j] for j in range(n)) for i in range(n)))
        ssr = max(ssr, 0.0)

        # Standard deviations from the residual variance.
        dof = max(self.rows - n, 1)
        sigma2 = ssr / dof
        sd = [math.sqrt(max(inv[i][i], 0.0) * sigma2) / scale[i] for i in range(n)]
        return p, math.sqrt(ssr / max(self.rows, 1)), sd


def invert(M):
    """Invert a symmetric positive (semi-)definite matrix.

    A zero row and column (an unused unknown) stays zero rather than
    making the matrix singular. Gauss-Jordan with partial pivoting.
    """
    n = len(M)
    A = [list(M[i]) + [1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
    for c in range(n):
        # Pivot: the largest entry in the column; none: an unused
        # unknown, left at 0.
        p = max(range(c, n), key=lambda r: abs(A[r][c]))
        if abs(A[p][c]) < SINGULAR:
            A[c][c] = 1.0
            A[c][n + c] = 0.0
            continue

        # Normalise the pivot row and clear the column elsewhere.
        A[c], A[p] = A[p], A[c]
        pivot = A[c][c]
        A[c] = [v / pivot for v in A[c]]
        for r in range(n):
            if r != c and A[r][c] != 0.0:
                f = A[r][c]
                A[r] = [v - f * w for v, w in zip(A[r], A[c])]
    return [row[n:] for row in A]


# Integrals of samples


def cumulative(t, f):
    """Return the running integral of samples f over times t.

    Trapezoids, starting at 0.
    """
    out = [0.0]
    for k in range(1, len(t)):
        out.append(out[-1] + 0.5 * (f[k] + f[k - 1]) * (t[k] - t[k - 1]))
    return out


def double_integral(t, f):
    """Return the running double integral of samples f over times t."""
    return cumulative(t, cumulative(t, f))


def unwrap(angles):
    """Return the angles [rad] without jumps of 2 pi."""
    out = []
    for a in angles:
        if out:
            a = out[-1] + math.atan2(math.sin(a - out[-1]), math.cos(a - out[-1]))
        out.append(a)
    return out


# The table


def slope_acceleration(coeffs, x, y, centre=CENTRE):
    """Return the table's pull (ax, ay) [m/s^2] at (x, y).

    Args:
        coeffs: The height coefficients [a1 .. a5].
        x, y: The position [m].
        centre: The height surface's centre [m].
    """
    a1, a2, a3, a4, a5 = coeffs
    u, v = x - centre[0], y - centre[1]
    return -G * (a1 + 2 * a3 * u + a4 * v), -G * (a2 + a4 * u + 2 * a5 * v)


def height(coeffs, x, y, centre=CENTRE):
    """Return the table top's height [m] at (x, y).

    See slope_acceleration.
    """
    a1, a2, a3, a4, a5 = coeffs
    u, v = x - centre[0], y - centre[1]
    return a1 * u + a2 * v + a3 * u * u + a4 * u * v + a5 * v * v


def fit_table(drifts, model=None, min_samples=20):
    """Fit the table's slope to platforms drifting on it.

    Args:
        drifts: [{"platform", "t", "x", "y"}] (each a list, s and m),
            each the record of one platform floating with its
            thrusters off.
        model: "quadratic", "plane", or None to choose (quadratic only
            with at least 3 drifts that between them cover most of the
            table).
        min_samples: Drifts shorter than this are left out.

    Returns:
        A dict with model, height [a1 .. a5], sigma (their standard
        deviations), damping {platform: c [1/s]}, damping_fixed (the
        platforms whose drag the drifts couldn't tell from the slope:
        short drifts make the two alike, and the fit then finds a drag
        below 0, which no air bearing has; theirs is fixed at 0 and
        the slope fitted again), rms_mm (position residual), the tilt
        (tilt_centre_mrad, tilt_max_mrad, downhill_deg: the downhill
        direction at the centre, from +x towards +y), coverage,
        drifts (how many), centre and g.

    Raises:
        ValueError: if no drift is long enough, or model is unknown.
    """
    # Long enough drifts, and the area they cover.
    drifts = [d for d in drifts if len(d["t"]) >= min_samples]
    if not drifts:
        raise ValueError("no drift long enough to fit (each needs at least %d samples)"
                         % min_samples)
    xs = [v for d in drifts for v in d["x"]]
    ys = [v for d in drifts for v in d["y"]]
    coverage = {"x": [min(xs), max(xs)], "y": [min(ys), max(ys)]}

    # The model: quadratic only with 3+ drifts covering 40 % of each
    # side.
    if model is None:
        wide = (coverage["x"][1] - coverage["x"][0] >= 0.4 * TABLE[0]
                and coverage["y"][1] - coverage["y"][0] >= 0.4 * TABLE[1])
        model = "quadratic" if len(drifts) >= 3 and wide else "plane"
    if model not in ("quadratic", "plane"):
        raise ValueError("model must be quadratic or plane")
    nslope = 5 if model == "quadratic" else 2

    # Fit; fix any negative drag at 0 and fit again, until none is.
    platforms = sorted(set(d["platform"] for d in drifts))
    fixed = []
    while True:
        p, rms, sd = _fit_table(drifts, nslope, platforms, fixed)
        damping = dict((n, p[nslope + i]) for i, n in enumerate(platforms))
        negative = [n for n in platforms if n not in fixed and damping[n] < 0]
        if not negative:
            break
        fixed += negative

    # All five coefficients (the plane's quadratic ones 0).
    coeffs = list(p[:nslope]) + [0.0] * (5 - nslope)
    sigma = list(sd[:nslope]) + [0.0] * (5 - nslope)
    return dict(model=model, height=coeffs, sigma=sigma, damping=damping, damping_fixed=fixed,
                rms_mm=1000 * rms, coverage=coverage, drifts=len(drifts), centre=list(CENTRE),
                g=G, **tilt(coeffs))


def _fit_table(drifts, nslope, platforms, fixed):
    """Solve fit_table's least squares.

    Columns: the nslope height coefficients, each platform's drag (0
    for the platforms in fixed: its column unused), then x0, vx0, y0,
    vy0 of each drift.
    """
    ncols = nslope + len(platforms) + 4 * len(drifts)
    ls = LeastSquares(ncols)
    for k, d in enumerate(drifts):
        t, x, y = d["t"], d["x"], d["y"]

        # Integrals of the known parts: 1, u and w (twice), and the
        # displacement (once, for the drag).
        u = [v - CENTRE[0] for v in x]
        w = [v - CENTRE[1] for v in y]
        J1 = [0.5 * (tk - t[0]) ** 2 for tk in t]
        Ju, Jw = double_integral(t, u), double_integral(t, w)
        Dx = cumulative(t, [v - x[0] for v in x])
        Dy = cumulative(t, [v - y[0] for v in y])

        # This drift's columns.
        c = nslope + platforms.index(d["platform"])
        free = d["platform"] not in fixed
        base = nslope + len(platforms) + 4 * k   # x0, vx0, y0, vy0 of this drift

        # One row per sample and axis.
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
    """Return the slope's size and direction.

    Args:
        coeffs: The height coefficients [a1 .. a5].
        n: Grid divisions per side for the largest tilt.

    Returns:
        A dict with tilt_centre_mrad and tilt_max_mrad (at the centre,
        and the largest over the table) and downhill_deg (the downhill
        direction at the centre, from +x towards +y).
    """
    def grad(x, y):
        """Return the height's gradient (dh/dx, dh/dy) at (x, y)."""
        ax, ay = slope_acceleration(coeffs, x, y)
        return -ax / G, -ay / G

    # At the centre, and the largest on an n x n grid.
    gx, gy = grad(*CENTRE)
    worst = 0.0
    for i in range(n + 1):
        for j in range(n + 1):
            hx, hy = grad(TABLE[0] * i / n, TABLE[1] * j / n)
            worst = max(worst, math.hypot(hx, hy))
    return {"tilt_centre_mrad": 1000 * math.hypot(gx, gy), "tilt_max_mrad": 1000 * worst,
            "downhill_deg": math.degrees(math.atan2(-gy, -gx))}


# A thruster


def fit_force(t, x, y, theta, mass, slope=None, damping=0.0):
    """Fit the constant body-frame force a platform felt.

    Args:
        t, x, y, theta: The samples [s, m, m, rad].
        mass: The platform's mass [kg].
        slope, damping: The table's pull taken out, as in fit_burn.

    Returns:
        A dict with Fb ([Fbx, Fby] [N]), sd (their standard
        deviations), rms_mm and theta (unwrapped).

    Raises:
        ValueError: if there are too few samples.
    """
    if len(t) < MIN_BURN_SAMPLES:
        raise ValueError("too few samples in the burn (%d)" % len(t))

    # Double integrals of the body frame's rotation.
    th = unwrap(theta)
    C = double_integral(t, [math.cos(a) for a in th])
    S = double_integral(t, [math.sin(a) for a in th])

    # The known displacements: the table's pull and the drag.
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

    # x = x0 + vx0 tau + (C Fbx - S Fby)/m + known
    # y = y0 + vy0 tau + (S Fbx + C Fby)/m + known
    ls = LeastSquares(6)   # Fbx, Fby, x0, vx0, y0, vy0
    for i in range(len(t)):
        tau = t[i] - t[0]
        ls.add({0: C[i] / mass, 1: -S[i] / mass, 2: 1.0, 3: tau}, x[i] - known_x[i])
        ls.add({0: S[i] / mass, 1: C[i] / mass, 4: 1.0, 5: tau}, y[i] - known_y[i])
    p, rms, sd = ls.solve()
    return {"Fb": [p[0], p[1]], "sd": [sd[0], sd[1]], "rms_mm": 1000 * rms, "theta": th}


def fit_burn(t, x, y, theta, mass, inertia, thruster, dist2CG, slope=None, damping=0.0):
    """Fit one burn of one thruster.

    Args:
        t, x, y, theta: The samples [s, m, m, rad] while it fires.
        mass: The platform's mass [kg].
        inertia: The platform's inertia [kg m^2].
        thruster: The thruster's number, 1-8.
        dist2CG: Its moment arm [mm] (the mixer's thruster_dist2CG).
        slope: The table's height coefficients (its pull is taken
            out), or None for a level table.
        damping: The air bearings' c [1/s].

    Returns:
        A dict with thrust [N] along the nominal direction, across
        [N], angle [deg] (how far the thrust is turned from nominal,
        towards the body's +z rotation), torque [N m] (measured),
        torque_expected (dist2CG * thrust), torque_ratio, thrust_sd,
        rms_mm and samples.
    """
    # The force.
    f = fit_force(t, x, y, theta, mass, slope, damping)
    (Fbx, Fby), sd, rms, th = f["Fb"], f["sd"], f["rms_mm"] / 1000, f["theta"]

    # The torque: theta = theta0 + w0 tau + (Tz / I) tau^2 / 2.
    la = LeastSquares(3)
    for i in range(len(t)):
        tau = t[i] - t[0]
        la.add({0: 1.0, 1: tau, 2: 0.5 * tau * tau / inertia}, th[i])
    q, _, qsd = la.solve()

    # Along and across the nominal direction d (across: d turned +90
    # deg).
    dxn, dyn = THRUSTER_DIRECTIONS[thruster - 1]
    along = Fbx * dxn + Fby * dyn
    across = -Fbx * dyn + Fby * dxn
    expected = dist2CG / 1000.0 * along
    return {"thrust": along, "across": across, "angle": math.degrees(math.atan2(across, along)),
            "torque": q[2], "torque_expected": expected,
            "torque_ratio": q[2] / expected if abs(expected) > MIN_TORQUE else None,
            "thrust_sd": math.hypot(sd[0] * dxn, sd[1] * dyn), "rms_mm": 1000 * rms,
            "samples": len(t)}


def summarise(values):
    """Return (mean, standard deviation); sd 0 for one value."""
    if not values:
        return None, None
    m = sum(values) / len(values)
    if len(values) < 2:
        return m, 0.0
    return m, math.sqrt(sum((v - m) ** 2 for v in values) / (len(values) - 1))


# Thrusters together


def combined_ratio(Fb, sd, thrusts):
    """Compare a firing of several thrusters with their thrusts alone.

    Args:
        Fb: The body-frame force the firing pushed with [N].
        sd: Its standard deviations [N].
        thrusts: {thruster number: calibrated thrust alone [N]}.

    Returns:
        (r, r_sd, P), with P their summed push alone [N]; r and r_sd
        are None if P is 0.
    """
    Px = sum(f * THRUSTER_DIRECTIONS[k - 1][0] for k, f in thrusts.items())
    Py = sum(f * THRUSTER_DIRECTIONS[k - 1][1] for k, f in thrusts.items())
    PP = Px * Px + Py * Py
    if PP < MIN_PUSH:
        return None, None, [Px, Py]
    return (Fb[0] * Px + Fb[1] * Py) / PP, math.hypot(sd[0] * Px, sd[1] * Py) / PP, [Px, Py]


# A firing's ratio is good to about 1 % at best (the fit's own sd is far
# smaller, and too uneven to weight by: one or two firings would decide
# the curve).
DECAY_SD_FLOOR = 0.01


def fit_decay(rows, n_max=8):
    """Fit the firings' ratios with the linear decay.

    Args:
        rows: [(N, r, r_sd)]; weighted by 1/r_sd^2, r_sd at least
            DECAY_SD_FLOOR.
        n_max: The most thrusters open at once.

    Returns:
        A dict with model ("linear"), scale (s), slope (m), rms,
        rms_linear, rms_rational and k (the rational curve's, for
        comparison), fraction (each thruster's share of its thrust
        alone with N = 1 .. n_max open) and drop_pct (100 (1 -
        fraction)).

    Raises:
        ValueError: with fewer than two firings.
    """
    rows = [(n, r, max(sd or 0.0, DECAY_SD_FLOOR)) for n, r, sd in rows]
    if len(rows) < 2:
        raise ValueError("at least two firings are needed to fit the decay (got %d)" % len(rows))
    lin, rat = _fit_decay_linear(rows), _fit_decay_rational(rows)
    frac = [max(0.0, 1 - lin["slope"] * (n - 1)) for n in range(1, n_max + 1)]
    return dict(lin, rms_linear=lin["rms"], rms_rational=rat["rms"], k=rat["k"],
                fraction=frac, drop_pct=[100 * (1 - f) for f in frac])


def _fit_decay_linear(rows):
    """Fit r = a + b (N - 1), weighted; s = a, m = -b / a."""
    w = [1.0 / sd ** 2 for _, _, sd in rows]
    x = [n - 1 for n, _, _ in rows]
    y = [r for _, r, _ in rows]

    # Weighted means and the regression line.
    W = sum(w)
    mx = sum(wi * xi for wi, xi in zip(w, x)) / W
    my = sum(wi * yi for wi, yi in zip(w, y)) / W
    sxx = sum(wi * (xi - mx) ** 2 for wi, xi in zip(w, x))
    b = sum(wi * (xi - mx) * (yi - my) for wi, xi, yi in zip(w, x, y)) / sxx if sxx else 0.0
    a = my - b * mx

    res = [yi - (a + b * xi) for xi, yi in zip(x, y)]
    return {"model": "linear", "scale": a, "slope": -b / a if a else 0.0,
            "rms": math.sqrt(sum(e * e for e in res) / len(res))}


def _fit_decay_rational(rows):
    """Fit r = s / (1 + k (N - 1)), weighted.

    s is linear once k is fixed, so k is found by a scan, then refined
    by golden-section search.
    """

    def solve(k):
        """Return (weighted chi^2, best s) for this k."""
        g = [1.0 / (1.0 + k * (n - 1)) for n, _, _ in rows]
        w = [1.0 / sd ** 2 for _, _, sd in rows]
        s_ = (sum(wi * gi * r for wi, gi, (_, r, _) in zip(w, g, rows))
              / sum(wi * gi * gi for wi, gi in zip(w, g)))
        chi = sum(wi * (r - s_ * gi) ** 2 for wi, gi, (_, r, _) in zip(w, g, rows))
        return chi, s_

    # Scan k (k > -1/(n_max - 1) keeps 1 + k (N - 1) positive).
    lo, hi = -0.12, 3.0
    ks = [lo + (hi - lo) * i / 3000 for i in range(3001)]
    k = min(ks, key=lambda v: solve(v)[0])

    # Refine around the best by golden-section search.
    step = (hi - lo) / 3000
    a, b = max(lo, k - step), min(hi, k + step)
    for _ in range(60):
        c, d = b - (b - a) * 0.618, a + (b - a) * 0.618
        if solve(c)[0] < solve(d)[0]:
            b = d
        else:
            a = c
    k = (a + b) / 2

    _, scale = solve(k)
    res = [r - scale / (1 + k * (n - 1)) for n, r, _ in rows]
    return {"model": "rational", "scale": scale, "k": k,
            "rms": math.sqrt(sum(e * e for e in res) / len(res))}
