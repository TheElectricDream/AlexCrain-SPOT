# The Thruster Mixer

How a force and torque command becomes eight valve duty cycles. Code: [[spot.control.thrusterDutyCycles|spot-control-thrusterDutyCycles]] (verbatim copies of the diagram's blocks in *Thruster & Float Control Code / Change PLATFORM Behavior*, [[page|Diagram-Thruster-and-Float-Control-Code]]).

## 1. Into the body frame

The controller's command $u = [F_x\ F_y\ T_z]$ is in the table frame; the thrusters are fixed to the body. With the platform's attitude $\theta$:

$$\begin{bmatrix} F_{x,b} \\ F_{y,b} \end{bmatrix} = C_{bI}(\theta) \begin{bmatrix} F_x \\ F_y \end{bmatrix}, \qquad C_{bI}(\theta) = \begin{bmatrix} \cos\theta & \sin\theta \\ -\sin\theta & \cos\theta \end{bmatrix}, \qquad F_b = [F_{x,b}\ F_{y,b}\ T_z]^T .$$

(block *Rotate Forces to Body / Create Rotation Matrix*, `RotateFrame`.)

## 2. The H matrix

Thruster $i$ open with duty cycle $d_i \in [0, 1]$ gives, on average over a PWM period, the force $d_i f_i$ along its direction, and the torque $d_i f_i r_i$ with $r_i$ its signed lever arm about the CG ([[Mass Properties|Math-Mass-Properties]]). Stacked:

$$F_b = H\, d, \qquad H = \underbrace{\begin{bmatrix} -1 & -1 & 0 & 0 & 1 & 1 & 0 & 0 \\ 0 & 0 & 1 & 1 & 0 & 0 & -1 & -1 \\ r_1 & r_2 & r_3 & r_4 & r_5 & r_6 & r_7 & r_8 \end{bmatrix}}_{\text{directions and lever arms [m]}} \ \mathrm{diag}\!\left(\frac{f_1}{2}, \dots, \frac{f_8}{2}\right)$$

Thrusters 1, 2, 5, 6 push along x (1, 2 backwards, 5, 6 forwards) and 3, 4, 7, 8 along y. Each $f_i$ is that thruster's own thrust, the 8×1 `F_thrusters_<PLATFORM>` ([[Workspace Variables|Workspace-Variables]]): the eight thrusts the lab's thruster calibration measured ([[Calibration Tab|Calibration-Tab]]) when the project has one and it is switched on, else the nominal `thrusterForce` for all eight. (The diagonal used to be built from one thrust for the x thrusters and one for the y thrusters, `F_*_X_nominal` and `F_*_Y_nominal`; with all eight equal the two are the same.) The $f_i/2$ is the block's convention (`MakeH`): the duty cycle it solves for is relative to half of each thruster's thrust, which keeps a 1:1 command from saturating; the final `H_final` uses the full $f_i$ when the forces are recomputed for the simulated plant.

## 3. Initial guess

$$d_0 = H^{+} F_b$$

with $H^+$ the pseudo-inverse (block *Generate Initial Guess*, the DSP *Pseudoinverse* block). It is the minimum-norm solution, but it can be negative or above 1: a valve can't push backwards or be more than fully open.

## 4. Constrained least squares with thrust decay

The block *MATLAB Function5* (`optimize_duty_cycle_with_decay`) solves

$$\min_{d} \| H(\kappa)\, d - F_b \|^2 \quad \text{subject to} \quad 0 \le d_i \le 1$$

by **projected gradient descent** with a backtracking line search (`custom_constrained_least_squares`):

$$d \leftarrow \operatorname{clip}_{[0,1]}\!\big( d - \alpha\, 2 H^T (H d - F_b) \big),$$

halving $\alpha$ until the residual decreases (up to 20 tries), growing $\alpha$ by 10 % after a successful step and shrinking it by 10 % otherwise, for at most 100 iterations or until $d$ stops changing by more than $10^{-6}$.

**Thrust decay** $\kappa$: when many valves are open at once the supply pressure drops and each thruster gives less than nominal. The block models this with one factor on all eight thrusts ($H(\kappa)$ has $\kappa f_i$ in place of $f_i$), from the mean duty cycle $\bar d$ of the valves that are open (`check_thrust_decay`):

$$\kappa = \begin{cases} 1 & \bar d < 0.3 \text{ or none open} \\ \max(1.6 - 2\bar d,\ 0.5) & \text{otherwise} \end{cases}$$

Since $\kappa$ depends on $d$ and $d$ on $H(\kappa)$, the two are iterated (outer loop, at most `max_iters` = 20 times, until $d$ changes by less than `tol` = $10^{-6}$ in the ∞-norm): build $H(\kappa)$, solve for $d$, update $\kappa$.

The outputs are the eight duty cycles (`*_Duty_Cycle`, ×100 as the percentage sent to the Teensy) and `H_final`, the full-thrust map with the final decay, which the simulated plant uses to compute the force actually produced.

## Timing

The mixer runs at `baseRate` (20 Hz) and the Teensy runs its PWM at `PWMFreq` (5 Hz), so a duty cycle is held for the whole valve period. The `verifyConsoleController` check 4 makes sure the block's sample time is still `baseRate`.
