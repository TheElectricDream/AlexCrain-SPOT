# The Calibration Fits

The fits behind the [[Calibration tab|Calibration-Tab]] ([[calfit.py|Console-calfit]]). Both fit the positions PhaseSpace measures, not accelerations worked out from them: the positions are good to about 0.2 mm, but differentiating them twice amplifies that noise a lot. Over a stretch of samples starting at $t_0$,

$$x(t) = x(t_0) + \dot x(t_0)\,(t - t_0) + \int_{t_0}^{t}\!\!\int_{t_0}^{s} \ddot x(r)\,dr\,ds$$

and when the acceleration is linear in the unknowns once the measured path is put in, so is the position: every fit is linear least squares, one row per sample and axis, with each stretch's starting position and velocity as unknowns too. The double integrals are taken over the samples with trapezoids.

## The table's slope

The table top's height (with $u = x - x_c$, $v = y - y_c$ from the table's centre, 1.75 m and 1.2 m) is

$$h(x, y) = a_1 u + a_2 v + a_3 u^2 + a_4 u v + a_5 v^2$$

A platform floating on it accelerates downhill and is dragged by its air bearings:

$$\ddot x = -g\,(a_1 + 2 a_3 u + a_4 v) - c\,\dot x, \qquad \ddot y = -g\,(a_2 + a_4 u + 2 a_5 v) - c\,\dot y$$

For a drift, the position's row holds $-g\iint 1$, $-2g\iint u$, $-g\iint v$ for $a_1, a_3, a_4$ (and the matching ones for $y$), and $-\int (x - x(t_0))$ for the drag $c$, since $\iint \dot x = \int (x - x(t_0))$. The unknowns are $a_1 \dots a_5$, one $c$ per platform, and four per drift.

**Plane or surface.** The quadratic terms need drifts in different parts of the table: with fewer than three drifts, or drifts covering less than 40 % of the table in either direction, the fit is a plane ($a_3 = a_4 = a_5 = 0$).

**Drag.** Over a short drift from rest, the slope gives $x \approx \tfrac12 a t^2$ and the drag only changes it by $\tfrac16 a c\, t^3$, so the two are hard to tell apart; the fit can then trade them and find $c < 0$, which no air bearing has. Such a platform's drag is fixed at 0 and the slope fitted again (`damping_fixed`).

**What it reports.** The tilt $|\nabla h|$ at the centre and the steepest anywhere (in mrad: mm per m), the downhill direction $\operatorname{atan2}(-\partial h/\partial y, -\partial h/\partial x)$, and the RMS position residual, which should be close to the cameras' noise when the model fits. In the simulation, [[spot.sim.tableSlope|spot-sim-tableSlope]] adds the same acceleration to each floating platform.

## A thruster's thrust

During a burn of thruster $k$, a force fixed in the platform's body, $\mathbf F_b = [F_{bx}, F_{by}]$, pushes it, and a torque $T_z$ turns it. With its mass $m$, inertia $I$ and the attitude $\theta(t)$ PhaseSpace measures,

$$\ddot{\mathbf x} = \frac{1}{m} R(\theta)\,\mathbf F_b + \mathbf a_{\text{table}}, \qquad \ddot\theta = T_z / I$$

$R(\theta)$ is known at every sample, so the position is linear in $F_{bx}, F_{by}$: their rows hold $\tfrac1m \iint \cos\theta$, $-\tfrac1m \iint \sin\theta$ (and $\sin$, $\cos$ for $y$). The table's pull $\mathbf a_{\text{table}}$ (above) is known from the table calibration, and is taken out first. The attitude alone gives $T_z$ (θ is quadratic in time).

With the thruster's direction $\mathbf d$ in the body (1 and 2 push along −x, 3 and 4 along +y, 5 and 6 along +x, 7 and 8 along −y) and $\mathbf d_\perp$ it turned by +90°:

- its **thrust** is $\mathbf F_b \cdot \mathbf d$;
- the part across, $\mathbf F_b \cdot \mathbf d_\perp$, gives how far the thrust is **turned** from $\mathbf d$;
- the **torque ratio** $T_z / (\ell_k\, \mathbf F_b \cdot \mathbf d)$, with $\ell_k$ the mixer's moment arm (`thruster_dist2CG`), checks the assumed centre of gravity and inertia: far from 1, they are off.

The fit assumes $m$, $I$ and the CG are known: the thrust it finds is $m$ times the measured acceleration, so a mass 5 % off gives thrusts 5 % off.
