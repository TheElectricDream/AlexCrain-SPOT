# calfit.py

**File:** `+spot/+console/webapp/spot_console/calfit.py` · **Part of:** [[the console's server|Console-Server]] · **Used by:** [[calibration.py|Console-calibration]] · **Maths:** [[The Calibration Fits|Math-Calibration]]

<!-- source: +spot/+console/webapp/spot_console/calfit.py -->

## What it is for

The fits behind the [[Calibration tab|Calibration-Tab]]: the table's slope from platforms drifting on their air bearings, and each thruster's thrust from short burns. Both fit the measured positions themselves, not accelerations worked out from them: PhaseSpace's positions are good to about 0.2 mm, but differentiating them twice amplifies that noise a lot. Every fit is linear least squares (the [[maths page|Math-Calibration]] derives them). Python 3.6 standard library only, and nothing else from the console, so the fits can be checked on their own.

## Function by function

**Lines 38–44.** Constants: g, the table's centre (where the height surface's `u`, `v` are measured from), the table's size, and the thrusters' directions in the body frame (as [[control.py|Console-control]]'s `VEC1`, `VEC2`).

**Lines 49–87.** `LeastSquares`: `min |A p − b|²`, built one row at a time (`add` takes the row's non-zero entries), solved from the normal equations with every column scaled to unit size first (the unknowns range from metres to micro-radians). `solve` returns the parameters, the RMS residual and the parameters' standard deviations (from the residual and the inverse of `A'A`; optimistic, since the samples are correlated).

**Lines 90–110.** `invert`: Gauss–Jordan with partial pivoting; a column that no row uses (an unknown not in this fit) stays 0 instead of making it singular.

**Lines 113–133.** `cumulative` (trapezoids), `double_integral`, `unwrap` (attitudes made continuous).

**Lines 136–146.** `slope_acceleration` and `height`: the table's pull at a point, and its height, for coefficients `[a1 .. a5]`. [[spot.sim.tableSlope|spot-sim-tableSlope]] is the same in MATLAB.

**Lines 149–183.** `fit_table`: drifts of at least 20 samples; the model (quadratic only with three drifts or more that between them cover at least 40 % of the table each way; else a plane); then the fit, again with the drag of any platform whose drag comes out below 0 fixed at 0 (`damping_fixed`): short drifts can't tell drag from slope, and an air bearing never pushes. Returns the coefficients and their standard deviations, the drag per platform, the residual, the coverage and the tilt.

**Lines 186–213.** `_fit_table`: one row per sample and axis. The unknowns are the slope's coefficients, each platform's drag, and each drift's starting position and velocity; the rows hold the double integrals of the measured path that multiply them.

**Lines 216–231.** `tilt`: the slope at the centre and the steepest anywhere (on a 37 × 37 grid) [mrad], and the downhill direction at the centre [°, from +x towards +y].

**Lines 234–279.** `fit_burn`: one burn of thruster k. The force is fixed in the body, so with the measured attitude the position is linear in the body-frame force `[Fbx, Fby]` (six unknowns with the starting position and velocity); the table's pull and the drag, known from the table calibration, are taken out first. The attitude alone gives the torque (θ quadratic in time). Returns the thrust along the thruster's direction, the part across it and the angle between them, the measured torque, the torque the mixer expects from that thrust (`dist2CG × thrust`) and their ratio, which checks the assumed CG and inertia.

**Lines 282–289.** `summarise`: the mean and standard deviation of repeated burns.

## Checks

Synthetic drifts from a known quadratic surface and drag, with 0.2 mm noise, give the coefficients back to 0.1 % and the drag to 0.0001 /s; synthetic burns (thrust 0.25–0.30 N, turned by up to 5°, one with a torque 20 % below expected) give the thrust within 2 % per 1.5 s burn, the angle within 1°, and the torque ratio within 0.03.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/calfit.py</b> (289 lines; click to show)</summary>

```python
   1  """The fits behind the Calibration tab (calibration.py): the table's slope from platforms drifting on
   2  their air bearings, and each thruster's thrust from short burns.
   3  
   4  Both fit the measured positions themselves rather than accelerations worked out from them (PhaseSpace's
   5  positions are good to a fraction of a millimetre, but differentiating them twice amplifies that noise a
   6  lot). Over a stretch of samples starting at t0, a platform's position is
   7  
   8      x(t) = x(t0) + vx(t0) (t - t0) + double integral of its acceleration from t0 to t
   9  
  10  and the accelerations here are linear in the unknowns once the measured path is put in, so every fit
  11  is linear least squares: each sample gives one row, the unknowns are the model's parameters and each
  12  stretch's starting position and velocity.
  13  
  14  The table. Its top is a smooth surface of height h(x, y) [m]; a floating platform accelerates downhill
  15  at -g grad h, and its air bearings drag it with -c v (c [1/s] per platform). With u = x - xc and
  16  v = y - yc (the table's centre, CENTRE),
  17  
  18      h = a1 u + a2 v + a3 u^2 + a4 u v + a5 v^2       (the "quadratic" model; "plane": a3 = a4 = a5 = 0)
  19      ax = -g (a1 + 2 a3 u + a4 v) - c vx,   ay = -g (a2 + a4 u + 2 a5 v) - c vy
  20  
  21  A thruster. During a burn the platform (mass m, inertia I, known) is pushed by a force fixed in its
  22  body, F_b = [Fbx, Fby], and turned by a torque Tz:
  23  
  24      a = R(theta) F_b / m + (the table's pull, known from its calibration),   alpha = Tz / I
  25  
  26  R(theta) uses the measured attitude, so the burn's own turning is accounted for. The thrust along the
  27  thruster's nominal direction d, F_b . d, is its calibrated thrust; the part across it gives how far
  28  the thrust is turned from d; Tz against the torque the mixer expects from it (dist2CG * thrust) checks
  29  the assumed CG and inertia.
  30  
  31  Part of the SPOT Console (webapp/app.py starts it). Python 3.6 standard library only, and nothing else
  32  from the console, so the fits can be checked on their own.
  33  """
  34  
  35  import math
  36  
  37  
  38  G = 9.80665                 # [m/s^2]
  39  CENTRE = (1.75, 1.2)        # [m] the table's centre (the slope's u, v are measured from here)
  40  TABLE = (3.5, 2.4)          # [m] the table top (as the 3D view draws it)
  41  
  42  # Thruster directions in the body frame (control.py's VEC1, VEC2): 1, 2 push along -x, 5, 6 along +x,
  43  # 3, 4 along +y, 7, 8 along -y
  44  THRUSTER_DIRECTIONS = [(-1, 0), (-1, 0), (0, 1), (0, 1), (1, 0), (1, 0), (0, -1), (0, -1)]
  45  
  46  
  47  # ---------------------------------------------------------------------------- least squares
  48  
  49  class LeastSquares:
  50      """min ||A p - b||^2, built one row at a time (normal equations, with the columns scaled so that
  51      unknowns of very different sizes, metres and micro-radians, are solved for equally well)."""
  52  
  53      def __init__(self, n):
  54          self.n = n
  55          self.AtA = [[0.0] * n for _ in range(n)]
  56          self.Atb = [0.0] * n
  57          self.btb = 0.0
  58          self.rows = 0
  59  
  60      def add(self, row, b):
  61          """ROW: {column: coefficient} (the other columns are 0); B: the measured value."""
  62          items = list(row.items())
  63          for i, ai in items:
  64              self.Atb[i] += ai * b
  65              r = self.AtA[i]
  66              for j, aj in items:
  67                  r[j] += ai * aj
  68          self.btb += b * b
  69          self.rows += 1
  70  
  71      def solve(self):
  72          """(p, rms residual, standard deviations of p). Columns never used come out as 0."""
  73          n = self.n
  74          scale = [math.sqrt(self.AtA[i][i]) or 1.0 for i in range(n)]
  75          M = [[self.AtA[i][j] / (scale[i] * scale[j]) for j in range(n)] for i in range(n)]
  76          rhs = [self.Atb[i] / scale[i] for i in range(n)]
  77          inv = invert(M)
  78          q = [sum(inv[i][j] * rhs[j] for j in range(n)) for i in range(n)]
  79          p = [q[i] / scale[i] for i in range(n)]
  80          # residual: |b|^2 - 2 p.A'b + p'A'A p
  81          ssr = self.btb - 2 * sum(p[i] * self.Atb[i] for i in range(n)) + \
  82              sum(p[i] * sum(self.AtA[i][j] * p[j] for j in range(n)) for i in range(n))
  83          ssr = max(ssr, 0.0)
  84          dof = max(self.rows - n, 1)
  85          sigma2 = ssr / dof
  86          sd = [math.sqrt(max(inv[i][i], 0.0) * sigma2) / scale[i] for i in range(n)]
  87          return p, math.sqrt(ssr / max(self.rows, 1)), sd
  88  
  89  
  90  def invert(M):
  91      """The inverse of a symmetric positive (semi-)definite matrix; a zero row and column (an unused
  92      unknown) stays zero rather than making it singular."""
  93      n = len(M)
  94      A = [list(M[i]) + [1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
  95      for c in range(n):
  96          p = max(range(c, n), key=lambda r: abs(A[r][c]))
  97          if abs(A[p][c]) < 1e-14:
  98              A[c][c] = 1.0   # unused: leave it at 0
  99              A[c][n + c] = 0.0
 100              continue
 101          A[c], A[p] = A[p], A[c]
 102          pivot = A[c][c]
 103          A[c] = [v / pivot for v in A[c]]
 104          for r in range(n):
 105              if r != c and A[r][c] != 0.0:
 106                  f = A[r][c]
 107                  A[r] = [v - f * w for v, w in zip(A[r], A[c])]
 108      return [row[n:] for row in A]
 109  
 110  
 111  # ---------------------------------------------------------------------------- integrals of samples
 112  
 113  def cumulative(t, f):
 114      """The running integral of the samples F over the times T (trapezoids), starting at 0."""
 115      out = [0.0]
 116      for k in range(1, len(t)):
 117          out.append(out[-1] + 0.5 * (f[k] + f[k - 1]) * (t[k] - t[k - 1]))
 118      return out
 119  
 120  
 121  def double_integral(t, f):
 122      return cumulative(t, cumulative(t, f))
 123  
 124  
 125  def unwrap(angles):
 126      out = []
 127      for a in angles:
 128          if out:
 129              a = out[-1] + math.atan2(math.sin(a - out[-1]), math.cos(a - out[-1]))
 130          out.append(a)
 131      return out
 132  
 133  
 134  # ---------------------------------------------------------------------------- the table
 135  
 136  def slope_acceleration(coeffs, x, y, centre=CENTRE):
 137      """The table's pull [ax, ay] [m/s^2] at (x, y) for height coefficients [a1 .. a5]."""
 138      a1, a2, a3, a4, a5 = coeffs
 139      u, v = x - centre[0], y - centre[1]
 140      return -G * (a1 + 2 * a3 * u + a4 * v), -G * (a2 + a4 * u + 2 * a5 * v)
 141  
 142  
 143  def height(coeffs, x, y, centre=CENTRE):
 144      a1, a2, a3, a4, a5 = coeffs
 145      u, v = x - centre[0], y - centre[1]
 146      return a1 * u + a2 * v + a3 * u * u + a4 * u * v + a5 * v * v
 147  
 148  
 149  def fit_table(drifts, model=None, min_samples=20):
 150      """Fit the table's slope to DRIFTS: [{"platform", "t", "x", "y"}] (each a list, s and m), each
 151      the record of one platform floating with its thrusters off. MODEL: "quadratic", "plane", or None
 152      to choose (quadratic only with at least 3 drifts that between them cover most of the table).
 153      Returns the fit (see the module's help), with "model", "height" [a1 .. a5], "sigma" (their
 154      standard deviations), "damping" {platform: c [1/s]}, "damping_fixed" (the platforms whose drag
 155      the drifts couldn't tell from the slope: short drifts make the two alike, and the fit then finds
 156      a drag below 0, which no air bearing has; theirs is fixed at 0 and the slope fitted again),
 157      "rms_mm" (position residual), the tilt ("tilt_centre_mrad", "tilt_max_mrad", "downhill_deg":
 158      the downhill direction at the centre, from +x towards +y), "coverage" and "drifts" (how many)."""
 159      drifts = [d for d in drifts if len(d["t"]) >= min_samples]
 160      if not drifts:
 161          raise ValueError("no drift long enough to fit (each needs at least %d samples)" % min_samples)
 162      xs = [v for d in drifts for v in d["x"]]
 163      ys = [v for d in drifts for v in d["y"]]
 164      coverage = {"x": [min(xs), max(xs)], "y": [min(ys), max(ys)]}
 165      if model is None:
 166          wide = coverage["x"][1] - coverage["x"][0] >= 0.4 * TABLE[0] and coverage["y"][1] - coverage["y"][0] >= 0.4 * TABLE[1]
 167          model = "quadratic" if len(drifts) >= 3 and wide else "plane"
 168      if model not in ("quadratic", "plane"):
 169          raise ValueError("model must be quadratic or plane")
 170      nslope = 5 if model == "quadratic" else 2
 171      platforms = sorted(set(d["platform"] for d in drifts))
 172      fixed = []
 173      while True:
 174          p, rms, sd = _fit_table(drifts, nslope, platforms, fixed)
 175          damping = dict((n, p[nslope + i]) for i, n in enumerate(platforms))
 176          negative = [n for n in platforms if n not in fixed and damping[n] < 0]
 177          if not negative:
 178              break
 179          fixed += negative
 180      coeffs = list(p[:nslope]) + [0.0] * (5 - nslope)
 181      sigma = list(sd[:nslope]) + [0.0] * (5 - nslope)
 182      return dict(model=model, height=coeffs, sigma=sigma, damping=damping, damping_fixed=fixed, rms_mm=1000 * rms,
 183                  coverage=coverage, drifts=len(drifts), centre=list(CENTRE), g=G, **tilt(coeffs))
 184  
 185  
 186  def _fit_table(drifts, nslope, platforms, fixed):
 187      """The least squares of fit_table; the drag of the platforms in FIXED is 0 (its column unused)."""
 188      ncols = nslope + len(platforms) + 4 * len(drifts)
 189      ls = LeastSquares(ncols)
 190      for k, d in enumerate(drifts):
 191          t, x, y = d["t"], d["x"], d["y"]
 192          u = [v - CENTRE[0] for v in x]
 193          w = [v - CENTRE[1] for v in y]
 194          J1 = [0.5 * (tk - t[0]) ** 2 for tk in t]
 195          Ju, Jw = double_integral(t, u), double_integral(t, w)
 196          Dx = cumulative(t, [v - x[0] for v in x])
 197          Dy = cumulative(t, [v - y[0] for v in y])
 198          c = nslope + platforms.index(d["platform"])
 199          free = d["platform"] not in fixed
 200          base = nslope + len(platforms) + 4 * k   # x0, vx0, y0, vy0 of this drift
 201          for i in range(len(t)):
 202              tau = t[i] - t[0]
 203              rx = {0: -G * J1[i], base: 1.0, base + 1: tau}
 204              ry = {1: -G * J1[i], base + 2: 1.0, base + 3: tau}
 205              if free:
 206                  rx[c] = -Dx[i]
 207                  ry[c] = -Dy[i]
 208              if nslope == 5:
 209                  rx.update({2: -G * 2 * Ju[i], 3: -G * Jw[i]})
 210                  ry.update({3: -G * Ju[i], 4: -G * 2 * Jw[i]})
 211              ls.add(rx, x[i])
 212              ls.add(ry, y[i])
 213      return ls.solve()
 214  
 215  
 216  def tilt(coeffs, n=36):
 217      """The slope's size: at the table's centre and the largest over the table [mrad], and the downhill
 218      direction at the centre [deg, from +x towards +y]."""
 219      def grad(x, y):
 220          ax, ay = slope_acceleration(coeffs, x, y)
 221          return -ax / G, -ay / G
 222      gx, gy = grad(*CENTRE)
 223      worst = 0.0
 224      for i in range(n + 1):
 225          for j in range(n + 1):
 226              hx, hy = grad(TABLE[0] * i / n, TABLE[1] * j / n)
 227              worst = max(worst, math.hypot(hx, hy))
 228      return {"tilt_centre_mrad": 1000 * math.hypot(gx, gy), "tilt_max_mrad": 1000 * worst,
 229              "downhill_deg": math.degrees(math.atan2(-gy, -gx))}
 230  
 231  
 232  # ---------------------------------------------------------------------------- a thruster
 233  
 234  def fit_burn(t, x, y, theta, mass, inertia, thruster, dist2CG, slope=None, damping=0.0):
 235      """Fit one burn: samples T, X, Y, THETA (s, m, rad) while thruster THRUSTER (1-8) fires, of a
 236      platform of MASS [kg] and INERTIA [kg m^2] whose thruster has the moment arm DIST2CG [mm] (the
 237      mixer's thruster_dist2CG). SLOPE: the table's height coefficients (its pull is taken out), or None
 238      for a level table; DAMPING: the air bearings' c [1/s].
 239      Returns {"thrust" [N] along the nominal direction, "across" [N], "angle" [deg] (how far the thrust is
 240      turned from nominal, towards the body's +z rotation), "torque" [N m] (measured), "torque_expected"
 241      (dist2CG * thrust), "torque_ratio", "rms_mm", "samples"}."""
 242      if len(t) < 8:
 243          raise ValueError("too few samples in the burn (%d)" % len(t))
 244      th = unwrap(theta)
 245      C = double_integral(t, [math.cos(a) for a in th])
 246      S = double_integral(t, [math.sin(a) for a in th])
 247      known_x = [0.0] * len(t)
 248      known_y = [0.0] * len(t)
 249      if slope is not None:
 250          pulls = [slope_acceleration(slope, xi, yi) for xi, yi in zip(x, y)]
 251          known_x = double_integral(t, [p[0] for p in pulls])
 252          known_y = double_integral(t, [p[1] for p in pulls])
 253      if damping:
 254          dx = cumulative(t, [v - x[0] for v in x])
 255          dy = cumulative(t, [v - y[0] for v in y])
 256          known_x = [k - damping * d for k, d in zip(known_x, dx)]
 257          known_y = [k - damping * d for k, d in zip(known_y, dy)]
 258      # x = x0 + vx0 tau + (C Fbx - S Fby)/m + known;  y = y0 + vy0 tau + (S Fbx + C Fby)/m + known
 259      ls = LeastSquares(6)   # Fbx, Fby, x0, vx0, y0, vy0
 260      for i in range(len(t)):
 261          tau = t[i] - t[0]
 262          ls.add({0: C[i] / mass, 1: -S[i] / mass, 2: 1.0, 3: tau}, x[i] - known_x[i])
 263          ls.add({0: S[i] / mass, 1: C[i] / mass, 4: 1.0, 5: tau}, y[i] - known_y[i])
 264      p, rms, sd = ls.solve()
 265      Fbx, Fby = p[0], p[1]
 266      # theta = theta0 + w0 tau + (Tz / I) tau^2 / 2
 267      la = LeastSquares(3)
 268      for i in range(len(t)):
 269          tau = t[i] - t[0]
 270          la.add({0: 1.0, 1: tau, 2: 0.5 * tau * tau / inertia}, th[i])
 271      q, _, qsd = la.solve()
 272      dxn, dyn = THRUSTER_DIRECTIONS[thruster - 1]
 273      along = Fbx * dxn + Fby * dyn
 274      across = -Fbx * dyn + Fby * dxn          # along d rotated +90 deg
 275      expected = dist2CG / 1000.0 * along
 276      return {"thrust": along, "across": across, "angle": math.degrees(math.atan2(across, along)),
 277              "torque": q[2], "torque_expected": expected,
 278              "torque_ratio": q[2] / expected if abs(expected) > 1e-9 else None,
 279              "thrust_sd": math.hypot(sd[0] * dxn, sd[1] * dyn), "rms_mm": 1000 * rms, "samples": len(t)}
 280  
 281  
 282  def summarise(values):
 283      """(mean, standard deviation) of a list (sd 0 for one value)."""
 284      if not values:
 285          return None, None
 286      m = sum(values) / len(values)
 287      if len(values) < 2:
 288          return m, 0.0
 289      return m, math.sqrt(sum((v - m) ** 2 for v in values) / (len(values) - 1))
```

</details>
<!-- /listing -->
