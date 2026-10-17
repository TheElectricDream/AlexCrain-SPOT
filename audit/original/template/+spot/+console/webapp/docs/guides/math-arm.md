# The Arm: Dynamics, Friction, Servos

The model inside `spot.arm.Plant`, the servos in PWM control, and the servo control laws in `spot.arm.PositionControl` / `spot.arm.VelocityControl`.

![ArmConfig.png](images/ArmConfig.png)

## Geometry

The shoulder sits on the Chaser's body at distance $b_0$ and angle $\Phi$ from the body centre (for the default mount on the +y face, left; `spot.arm.mountGeometry` moves them with the mount). Link $i$ has its joint-to-centre-of-mass length $a_i$ and centre-of-mass-to-next-joint length $b_i$, mass $m_i$ (`mPROX`, `mDIST`, `mEE`) and inertia $I_i$ about its centre of mass (`IPROX`, `IDIST`, `IEE`). The joint angles $q_1, q_2, q_3$ are 0 with the link straight out (the shoulder: straight out from its face), positive counter-clockwise. The forward kinematics is on `spot.plotting.armPosition`.

## Equations of motion

The Chaser with its arm is a free-floating planar system with generalised coordinates $\mathbf{q} = [x\ y\ \theta\ q_1\ q_2\ q_3]^T$:

$$M(\mathbf{q})\,\ddot{\mathbf{q}} + C(\mathbf{q}, \dot{\mathbf{q}})\,\dot{\mathbf{q}} = \begin{bmatrix} F_x \\ F_y \\ T_z \\ \tau_1 - f_1 \\ \tau_2 - f_2 \\ \tau_3 - f_3 \end{bmatrix}$$

where $F$ is the thruster force/torque on the base, $\tau_i$ the motor torque at joint $i$ (back-EMF included) and $f_i$ its friction. $M$ (`spot.arm.inertiaMatrix`) and $C$ (`spot.arm.coriolisMatrix`) were derived symbolically in 2017 (Lagrange's equations for the four bodies) and generated as code; each joint's reflected rotor inertia $J_{a,i}$ (`armature`, rotor inertia × gear ratio²) is added to $M_{3+i,3+i}$. Because the base floats, moving the arm moves the base: $M$ couples all six coordinates.

## The servos

Each MX-64 is a DC motor through a 200:1 gearbox (`Constants.MX64_GEAR_RATIO`; the servo's and the motor's numbers below are the Dynamixel and arm sections of [`Constants`](page:dev-constants)), in **PWM control** (operating mode 16, `MODE_PWM`, also called voltage control): the command $u$ (`ARM_Controls`, in the servo's units, 885 = 100 %, `PWM_FULL`) is the share of the supply voltage $V_s$ put across the motor, $V = \frac{u}{885} V_s$. With the winding's inductance neglected, the motor then gives at the joint

$$\tau = g\,(V - K_e\,\omega), \qquad g = \frac{\tau_s}{V_0}\ [\text{N m/V}], \qquad K_e = \frac{V_0}{\omega_0}\ [\text{V s/rad}],$$

from the datasheet at $V_0 = 12$ V (`MX64_SUPPLY`): stall torque $\tau_s = 6.0$ N m (`MX64_STALL_TORQUE`) and no-load speed $\omega_0 = 63$ rpm (`MX64_NO_LOAD_RPM`), so $g = 0.5$ N m/V (`TORQUE_PER_VOLT_MX64`) and $K_e \approx 1.82$ V s/rad (`BACK_EMF_MX64_DATASHEET`) (`armModel.motor.torquePerVolt`, `.backEMF`). The arm calibration measures the back-EMF (with the viscous friction, which brakes alike): an MX-64(2.0) on the bench gave $K_e = 1.485$ V s/rad, 18 % below the datasheet, and that is the default (`ARM_BACK_EMF`, which `spot.arm.defaults` reads); $g$ stays the datasheet's, since nothing measures it. At rest PWM *is* a torque, about 0.0068 N m per unit at 12 V, which is why it stands in for torque control. Turning, the back-EMF brakes the joint like a viscous friction of $g K_e \approx 0.74$ N m s/rad, far more than the joints' own; it is what makes the arm well damped. The plant takes it implicitly with the viscous friction below, so it is stable at any step.

The command is sent in whole units, within the PWM Limit, with none pushing a joint further past its position limits (`spot.arm.withinLimits`), as `spot.hardware.ArmPWM` sends it; the plant's third output is that applied PWM (`ARM_Measured_PWM`).

Why PWM rather than current (torque) control: the servos measure their current in 3.36 mA units (`CURRENT_UNIT`), and at the arm's small torques (a few tenths of a N m, a few dozen mA) that reading is too coarse and noisy to control with, or to calibrate from; one wrist servo even read differently each way. PWM needs no measurement at all: it is what the servo applies.

## Friction with stiction

Per joint, four parameters (the bench values `ARM_STATIC`, `ARM_COULOMB`, `ARM_STRIBECK` and `ARM_VISCOUS` through `spot.arm.defaults`, or the ones measured in PWM control: the lab's arm calibration from the [Calibration Tab](page:tab-calibration), or an `arm_calibration.json` in the project folder from [calibrate_arm.py](file:calibrate_arm.py), whichever is newer; `spot.arm.parameters`): breakaway torque $T_s$, Coulomb torque $T_c$, Stribeck velocity $v_s$, viscous coefficient $b$. Moving at speed $\omega$, the friction is

$$f(\omega) = \left[T_c + (T_s - T_c)\, e^{-(\omega/v_s)^2}\right] \operatorname{sign}(\omega) + b\,\omega ,$$

and at rest the joint stays at rest as long as the torque on it is below $T_s$. Simulating that with an explicit integrator chatters (the sign flips every step). The plant solves it **implicitly** each inner step $h$: the velocity update is written as

$$A\, v^{+} = r + \sum_j e_j\, p_j, \qquad A = M + h\,\mathrm{diag}(0,0,0,b + g K_e), \quad r = M v + h\,([F; g V] - C v),$$

where $p_j$ is the friction *impulse* at joint $j$ over the step, bounded by $|p_j| \le h\,\mu_j(\omega)$ with $\mu_j = T_c + (T_s - T_c)e^{-(\omega/v_s)^2}$. Each $p_j$ is chosen to bring joint $j$ to rest if that is within its bound, otherwise it sits at the bound against the motion. That is a small box-constrained linear complementarity problem, solved by projected Gauss-Seidel (`dryFriction`, up to 60 sweeps); positions then advance with the new velocities (semi-implicit Euler, $q^+ = q + h v^+$). The result sticks and slips without chattering, at a 1 ms step, however stiff the friction.

## The position control law (MX-64 mode 3, replicated)

In the servo's units (pulses, 0.229 rpm velocity units (`VEL_UNIT_RPM`), PWM 885 = 100 %), per control period $T_p$ (1 ms in the servo, `SERVO_PERIOD`; the controller scales to its own step $h$):

1. **Goal**: $g = \operatorname{round}(651.74\, q^* + 2048)$ pulses (`CMD_PULSES_PER_RAD`, `CENTER_PULSE`); a goal outside [Min, Max Position] is ignored (the joint keeps its last goal), as the servo does.
2. **Profile**: the desired position $p_d$ moves towards $g$ at Profile Velocity $V_p$ (and, if set, Profile Acceleration $A_p$ with the usual $\sqrt{2 A_p\, d}$ slow-down); 0 means "jump to the goal".
3. **PID + feedforward**:
$$u = \frac{P}{128}\, e + \frac{I}{65536} \sum e\,\frac{h}{T_p} + \frac{D}{16}\, \Delta e\,\frac{T_p}{h} + \frac{FF_1}{4} v_d + \frac{FF_2}{4} a_d, \qquad e = p_d - p \ [\text{pulses}]$$
clipped to ±PWM Limit, with anti-windup (the integral holds while the output is saturated in the direction of the error), in whole units.

The output is that PWM ($u$), which the servos in PWM control apply as they would their own position law's: run at 1 ms with the servo's gains (400 / 0 / 200, `SERVO_POSITION_P/I/D`), this *is* the servo's position mode (`spot.arm.compareSteps` checks the model so). At the diagram's 20 Hz (`SAMPLE_RATE`) those gains are far too stiff and the arm goes unstable, so the block's defaults are retuned for 20 Hz: P = [25 56 21], I = 10, D = [1545 230 236] (`ARM_POSITION_P`, `ARM_POSITION_I`, `ARM_POSITION_D`) — about 1 N m/rad of stiffness at rest, a twentieth of the servo's own, with the integral doing the last few degrees against stiction. The motors' back-EMF adds its damping to the D gain's (the elbow then takes about 3 s to reach a 20° step, against under a second without it).

## The velocity control law (mode 1, replicated)

Goal in 0.229 rpm units (ignored beyond Velocity Limit), an optional acceleration profile, then PI: $u = \frac{P}{128} e + \frac{I}{65536} \sum e\, \frac{h}{T_p}$ with $e$ in velocity units, clipped and anti-wound-up as above; the output is the PWM $u$. Retuned for 20 Hz: P = [160 70 30], I = [220 1500 300] (`ARM_VELOCITY_P`, `ARM_VELOCITY_I`). In PWM control a joint's speed follows its voltage closely ($\omega \approx (V - f/g)/K_e$ once moving), which helps this loop.

## Units cheat-sheet

See [Dynamixel Units](page:math-dynamixel).
