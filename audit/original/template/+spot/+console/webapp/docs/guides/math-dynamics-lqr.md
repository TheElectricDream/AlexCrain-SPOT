# Platform Dynamics and LQR

The model behind the default controller, and how its gains are found. Code: `spot.control.stateSpace`, `spot.control.lqrGains`, `spot.control.lqrCommand`.

## The plant

A platform floating on the table is a rigid body with three degrees of freedom, (x, y, θ), and no friction. With the state $x = [x\ y\ \theta\ \dot x\ \dot y\ \dot\theta]^T$ and the input $u = [F_x\ F_y\ T_z]^T$ (forces in the **table frame**, torque about z):

$$\dot x = A x + B u, \qquad A = \begin{bmatrix} 0_{3\times3} & I_3 \\ 0_{3\times3} & 0_{3\times3} \end{bmatrix}, \qquad B = \begin{bmatrix} 0_{3\times3} \\ \mathrm{diag}\!\left(\tfrac1m, \tfrac1m, \tfrac1{I_{zz}}\right) \end{bmatrix}.$$

That is three independent double integrators: $m\ddot x = F_x$, $m\ddot y = F_y$, $I_{zz}\ddot\theta = T_z$. The output is the full state ($C = I_6$, $D = 0$).

The controller runs in discrete time, so the model is discretised with a zero-order hold at the step $T_s$ (`c2d(..., 'zoh')`):

$$x_{k+1} = A_d x_k + B_d u_k, \qquad A_d = \begin{bmatrix} I & T_s I \\ 0 & I \end{bmatrix}, \quad B_d = \begin{bmatrix} \tfrac{T_s^2}{2} M^{-1} \\ T_s M^{-1} \end{bmatrix}, \quad M = \mathrm{diag}(m, m, I_{zz}).$$

$T_s$ is the **PhaseSpace period** `dataRate` (1/10 s by default, `Constants.DATA_RATE`), not the controller's step: the measurements only change at the camera rate, so that is the rate the loop effectively runs at. The matrices are `A_d_RED`, `B_d_RED`, … in the workspace.

## The LQR gain

The discrete linear-quadratic regulator minimises

$$J = \sum_{k=0}^{\infty} \left( x_k^T Q x_k + u_k^T R u_k \right)$$

with $Q = \mathrm{diag}(Q_x, Q_y, Q_\theta, Q_{\dot x}, Q_{\dot y}, Q_{\dot\theta})$ and $R = \mathrm{diag}(R_{F_x}, R_{F_y}, R_{T_z})$, the nine numbers on the Control pane. The solution is the state feedback $u_k = -K x_k$ where

$$K = (R + B_d^T P B_d)^{-1} B_d^T P A_d$$

and $P$ solves the discrete algebraic Riccati equation

$$P = A_d^T P A_d - A_d^T P B_d (R + B_d^T P B_d)^{-1} B_d^T P A_d + Q .$$

MATLAB's `dlqr` does this (`K = dlqr(A_d, B_d, diag(Q), diag(R))`), giving the 3×6 gain `K_<PLATFORM>`. Bigger $Q$ entries make the controller care more about that state (faster, stronger), bigger $R$ entries make it thriftier with that input. The defaults, $Q = [1\ 1\ 0.05\ 10\ 10\ 1]$ and $R = [6\ 6\ 6]$ (`LQR_Q`, `LQR_R`), favour smooth motion over speed.

## The control law in the diagram

Each step, the block *Default LQR Control (PLATFORM)* forms the error between the measured state and the commanded path, and multiplies by $-K$:

$$e = \begin{bmatrix} x - x_d \\ y - y_d \\ \operatorname{wrap}(\theta - \theta_d) \\ \dot x - \dot x_d \\ \dot y - \dot y_d \\ \dot\theta - \dot\theta_d \end{bmatrix}, \qquad u = -K e, \qquad \operatorname{wrap}(a) = \big((a + \pi) \bmod 2\pi\big) - \pi .$$

The desired rates $\dot x_d, \dot y_d, \dot\theta_d$ are the path's discrete derivative, $(p_k - p_{k-1})/\text{baseRate}$ (the block's *Discrete Derivative*). Wrapping the attitude error keeps the platform from spinning the long way round through ±π. The result $u = [F_x\ F_y\ T_z]$ is in the table frame and goes to the [thruster mixer](page:math-mixer), which turns it into eight duty cycles.

`spot.control.lqrCommand` is this law as a function (the wrap is the block's own `SmoothAngularDifference`, copied verbatim).
