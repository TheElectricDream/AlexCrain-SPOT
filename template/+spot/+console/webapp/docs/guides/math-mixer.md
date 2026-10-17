# The Thruster Mixer

How a force and torque command becomes eight valve duty cycles. Code: `spot.control.thrusterDutyCycles` (verbatim copies of the diagram's blocks in *Thruster & Float Control Code / Change PLATFORM Behavior*, [page](page:diagram-thrusters)).

## 1. Into the body frame

The controller's command $u = [F_x\ F_y\ T_z]$ is in the table frame; the thrusters are fixed to the body. With the platform's attitude $\theta$:

$$\begin{bmatrix} F_{x,b} \\ F_{y,b} \end{bmatrix} = C_{bI}(\theta) \begin{bmatrix} F_x \\ F_y \end{bmatrix}, \qquad C_{bI}(\theta) = \begin{bmatrix} \cos\theta & \sin\theta \\ -\sin\theta & \cos\theta \end{bmatrix}, \qquad F_b = [F_{x,b}\ F_{y,b}\ T_z]^T .$$

(block *Rotate Forces to Body / Create Rotation Matrix*, `RotateFrame`.)

## 2. The H matrix

Thruster $i$ open with duty cycle $d_i \in [0, 1]$ gives, on average over a PWM period, the force $d_i f_i$ along its direction, and the torque $d_i f_i r_i$ with $r_i$ its signed lever arm about the CG ([Mass Properties](page:math-mass-properties)). Stacked:

$$F_b = H\, d, \qquad H = \underbrace{\begin{bmatrix} -1 & -1 & 0 & 0 & 1 & 1 & 0 & 0 \\ 0 & 0 & 1 & 1 & 0 & 0 & -1 & -1 \\ r_1 & r_2 & r_3 & r_4 & r_5 & r_6 & r_7 & r_8 \end{bmatrix}}_{\text{directions and lever arms [m]}} \ \mathrm{diag}\!\left(\frac{f_1}{2}, \dots, \frac{f_8}{2}\right)$$

Thrusters 1, 2, 5, 6 push along x (1, 2 backwards, 5, 6 forwards) and 3, 4, 7, 8 along y. Each $f_i$ is that thruster's own thrust, the 8×1 `F_thrusters_<PLATFORM>` ([Workspace Variables](page:workspace-variables)): the eight thrusts the lab's thruster calibration measured ([Calibration Tab](page:tab-calibration)) when the project has one and it is switched on, else the nominal `thrusterForce` for all eight. (The diagonal used to be built from one thrust for the x thrusters and one for the y thrusters, `F_*_X_nominal` and `F_*_Y_nominal`; with all eight equal the two are the same.) The $f_i/2$ is the block's convention (`MakeH`): the duty cycle it solves for is relative to half of each thruster's thrust, which keeps a 1:1 command from saturating; the final `H_final` uses the full $f_i$ when the forces are recomputed for the simulated plant.

## 3. Initial guess

$$d_0 = H^{+} F_b$$

with $H^+$ the pseudo-inverse (block *Generate Initial Guess*, the DSP *Pseudoinverse* block). It is the minimum-norm solution, but it can be negative or above 1: a valve can't push backwards or be more than fully open.

## 4. Constrained least squares with thrust decay

The block *MATLAB Function5* (`optimize_duty_cycle_with_decay`) solves

$$\min_{d} \| H(\kappa)\, d - F_b \|^2 \quad \text{subject to} \quad 0 \le d_i \le 1$$

by **projected gradient descent** with a backtracking line search (`custom_constrained_least_squares`):

$$d \leftarrow \operatorname{clip}_{[0,1]}\!\big( d - \alpha\, 2 H^T (H d - F_b) \big),$$

halving $\alpha$ until the residual decreases (up to 20 tries, `Constants.MIXER_CLS_LINE_SEARCH`), growing $\alpha$ by 10 % after a successful step and shrinking it by 10 % otherwise (`MIXER_CLS_STEP_GROWTH`, `MIXER_CLS_STEP_SHRINK`), for at most 100 iterations (`MIXER_CLS_MAX_ITERATIONS`) or until $d$ stops changing by more than $10^{-6}$ (`MIXER_CLS_TOLERANCE`).

**Thrust decay.** The eight thrusters share one regulated air supply, so each gives less when several are open at once. With $n$ valves open, each gives

$$\kappa(n) = \max\bigl(0,\ 1 - m\,(n - 1)\bigr)$$

of its thrust alone, where $m$ is the platform's **thrust decay**, the fraction lost per extra valve open (`thrust_decay_<PLATFORM>`: the Together calibration's when it is used, else the Platforms pane's Thrust Decay, 0.112 by default, `THRUST_DECAY_DEFAULT`). The Together stage of the thruster calibration measures $m$ ([The Calibration Tab](page:tab-calibration)).

How many valves are open at a given moment follows from the duty cycles, because the Teensy opens every valve at the start of each PWM period and closes each one after its duty cycle. Sort the duty cycles: between two closings the number open is known, so each thruster's effective share is the average of $\kappa(n)$ over its own opening (`check_thrust_decay`):

$$\kappa_i = \frac{1}{d_i} \int_0^{d_i} \kappa\bigl(n(t)\bigr)\,dt, \qquad n(t) = \#\{\,j : d_j \ge t\,\}.$$

$H(\kappa)$ then has $\kappa_i f_i$ in place of $f_i$ (`MakeHWithDecay`). Since the $\kappa_i$ depend on $d$ and $d$ on $H(\kappa)$, the two are iterated (the outer loop, at most `max_iters` = 20 times, until $d$ changes by less than `tol` = $10^{-6}$: the block mask's values, `MIXER_MAX_ITERATIONS` and `MIXER_TOLERANCE`): build $H(\kappa)$, solve for $d$, update the $\kappa_i$. With a single valve open there is no decay; with all eight equal thrusts and $m = 0$ the result is the plain constrained fit.

The outputs are the eight duty cycles (`*_Duty_Cycle`, ×100 as the percentage sent to the Teensy) and `H_final`, the full-thrust map with the final decay, which the simulated plant uses to compute the force actually produced.

## Timing

The mixer runs at `baseRate` (20 Hz, `SAMPLE_RATE`) and the Teensy runs its PWM at `PWMFreq` (5 Hz, `TEENSY_PWM_FREQUENCY`), so a duty cycle is held for the whole valve period. The `verifyConsoleController` check 4 makes sure the block's sample time is still `baseRate`.
