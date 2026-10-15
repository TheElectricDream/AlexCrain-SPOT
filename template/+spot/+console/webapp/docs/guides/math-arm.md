# The Arm: Dynamics, Friction, Servos

The model inside `spot.arm.Plant`, and the servo control laws in `spot.arm.PositionControl` / `spot.arm.VelocityControl`.

![resources/ArmConfig.png](images/ArmConfig.png)

## Geometry

The shoulder sits on the Chaser's body at distance $b_0$ and angle $\Phi$ from the body centre (for the default mount on the +y face, left; `spot.arm.mountGeometry` moves them with the mount). Link $i$ has its joint-to-centre-of-mass length $a_i$ and centre-of-mass-to-next-joint length $b_i$, mass $m_i$ (`mPROX`, `mDIST`, `mEE`) and inertia $I_i$ about its centre of mass (`IPROX`, `IDIST`, `IEE`). The joint angles $q_1, q_2, q_3$ are 0 with the link straight out (the shoulder: straight out from its face), positive counter-clockwise. The forward kinematics is on `spot.plotting.armPosition`.

## Equations of motion

The Chaser with its arm is a free-floating planar system with generalised coordinates $\mathbf{q} = [x\ y\ \theta\ q_1\ q_2\ q_3]^T$:

$$M(\mathbf{q})\,\ddot{\mathbf{q}} + C(\mathbf{q}, \dot{\mathbf{q}})\,\dot{\mathbf{q}} = \begin{bmatrix} F_x \\ F_y \\ T_z \\ \tau_1 - f_1 \\ \tau_2 - f_2 \\ \tau_3 - f_3 \end{bmatrix}$$

where $F$ is the thruster force/torque on the base, $\tau_i$ the motor torque at joint $i$ and $f_i$ its friction. $M$ (`spot.arm.inertiaMatrix`) and $C$ (`spot.arm.coriolisMatrix`) were derived symbolically in 2017 (Lagrange's equations for the four bodies) and generated as code; each joint's reflected rotor inertia $J_{a,i}$ (`armature`, rotor inertia × gear ratio²) is added to $M_{3+i,3+i}$. Because the base floats, moving the arm moves the base: $M$ couples all six coordinates.

## The servos

Each MX-64 is a DC motor through a 200:1 gearbox, in **current control**: the servo's own loop holds the commanded current $i^*$. The model keeps three constants from the datasheet (at 12 V): stall torque $\tau_s = 6.0$ N m, stall current $i_s = 4.1$ A, no-load speed $\omega_0 = 63$ rpm, giving at the joint

$$K_t = \frac{\tau_s}{i_s}\ [\text{N m/A}], \qquad R = \frac{V}{i_s}\ [\Omega], \qquad K_e = \frac{V}{\omega_0}\ [\text{V s/rad}].$$

The commanded torque $\tau^*$ becomes a goal current $i^* = \tau^*/K_t$, quantised to the servo's 3.36 mA unit and clipped to the Current Limit (850 units ≈ 2.86 A), with no torque that pushes a joint further past its position limits (`spot.arm.withinLimits`). The servo can only hold that current as far as its supply voltage allows against the motor's back-EMF:

$$i = \operatorname{clip}\!\left(i^*,\ \frac{-V - K_e\omega}{R},\ \frac{V - K_e\omega}{R}\right), \qquad \tau = K_t\, i .$$

So a joint spinning fast can't be pushed harder in the same direction; that is the motor's torque-speed curve.

## Friction with stiction

Per joint, four parameters (`spot.arm.defaults`, or the measured ones: the lab's arm calibration from the [Calibration Tab](page:tab-calibration), or a `resources/arm_calibration.json` from [calibrate_arm.py](file:calibrate_arm.py), whichever is newer; `spot.arm.parameters`): breakaway torque $T_s$, Coulomb torque $T_c$, Stribeck velocity $v_s$, viscous coefficient $b$. Moving at speed $\omega$, the friction is

$$f(\omega) = \left[T_c + (T_s - T_c)\, e^{-(\omega/v_s)^2}\right] \operatorname{sign}(\omega) + b\,\omega ,$$

and at rest the joint stays at rest as long as the torque on it is below $T_s$. Simulating that with an explicit integrator chatters (the sign flips every step). The plant solves it **implicitly** each inner step $h$: the velocity update is written as

$$A\, v^{+} = r + \sum_j e_j\, p_j, \qquad A = M + h\,\mathrm{diag}(0,0,0,b), \quad r = M v + h\,([F; \tau] - C v),$$

where $p_j$ is the friction *impulse* at joint $j$ over the step, bounded by $|p_j| \le h\,\mu_j(\omega)$ with $\mu_j = T_c + (T_s - T_c)e^{-(\omega/v_s)^2}$. Each $p_j$ is chosen to bring joint $j$ to rest if that is within its bound, otherwise it sits at the bound against the motion. That is a small box-constrained linear complementarity problem, solved by projected Gauss-Seidel (`dryFriction`, up to 60 sweeps); positions then advance with the new velocities (semi-implicit Euler, $q^+ = q + h v^+$). The result sticks and slips without chattering, at a 1 ms step, however stiff the friction.

## The position control law (MX-64 mode 3, replicated)

In the servo's units (pulses, 0.229 rpm velocity units, PWM 885 = 100 %), per control period $T_p$ (1 ms in the servo; the controller scales to its own step $h$):

1. **Goal**: $g = \operatorname{round}(651.74\, q^* + 2048)$ pulses; a goal outside [Min, Max Position] is ignored (the joint keeps its last goal), as the servo does.
2. **Profile**: the desired position $p_d$ moves towards $g$ at Profile Velocity $V_p$ (and, if set, Profile Acceleration $A_p$ with the usual $\sqrt{2 A_p\, d}$ slow-down); 0 means "jump to the goal".
3. **PID + feedforward**:
$$u = \frac{P}{128}\, e + \frac{I}{65536} \sum e\,\frac{h}{T_p} + \frac{D}{16}\, \Delta e\,\frac{T_p}{h} + \frac{FF_1}{4} v_d + \frac{FF_2}{4} a_d, \qquad e = p_d - p \ [\text{pulses}]$$
clipped to ±PWM Limit, with anti-windup (the integral holds while the output is saturated in the direction of the error).
4. **PWM to torque**: $\tau = \dfrac{K_t}{R}\left(\dfrac{u}{885} V - [K_e\,\omega]\right)$, the back-EMF term optional (on for the servo's own 1 ms loop, off at 20 Hz).

Run at 1 ms with the servo's gains (400 / 0 / 200) and back-EMF, this *is* the servo's position mode. At the diagram's 20 Hz those gains are far too stiff and the arm goes unstable, so the block's defaults are retuned for 20 Hz: P = [25 56 21], I = 10, D = [1545 230 236] — about 1 N m/rad of stiffness, a twentieth of the servo's own, with the integral doing the last few degrees against stiction.

## The velocity control law (mode 1, replicated)

Goal in 0.229 rpm units (ignored beyond Velocity Limit), an optional acceleration profile, then PI: $u = \frac{P}{128} e + \frac{I}{65536} \sum e\, \frac{h}{T_p}$ with $e$ in velocity units, clipped and anti-wound-up as above, and the same PWM-to-torque. Retuned for 20 Hz: P = [160 70 30], I = [220 1500 300].

## Units cheat-sheet

See [Dynamixel Units](page:math-dynamixel).
