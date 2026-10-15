# spot.control.lqrCommand

**File:** `+spot/+control/lqrCommand.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/lqrCommand.m -->

## What it is for

The diagram's default control law as a function you can call: what the block *4 - Control / Default LQR Control (PLATFORM)* computes each step. The attitude-error wrap is the block's own `Calculate Error` code, copied verbatim (between the `% ---- verbatim` markers; [[verifyConsoleController|spot-control-verifyConsoleController]] checks it still matches the diagram). Maths: [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]].

## How to call it

```matlab
u = spot.control.lqrCommand(K, measured, desired, desiredRate)
%  K            3x6 gain (K_RED)
%  measured     [x y theta xdot ydot thetadot]   (*_Measured_States(1:6))
%  desired      [x y theta]                      (*_Path)
%  desiredRate  (desired - previous desired) / baseRate
%  u            [Fx; Fy; Tz] in the table frame  (*_Controls)
```

## Line by line

**Lines 20–22.** Make the three inputs columns whatever shape they came in.

**Lines 23–30.** The error vector, as the block wires it: two Demux blocks split the measured state and the path; five Subtract blocks take measured minus desired for x, y and the three rates (the desired rates come from the block's Discrete Derivative of the path); the attitude goes through `SmoothAngularDifference` so the error is in [−π, π).

**Line 31.** The matrix multiply by the constant −K.

**Lines 34–41.** The verbatim block code: $e_\theta = ((\theta_m - \theta_p + \pi) \bmod 2\pi) - \pi$.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+console/webapp/static/interactive.js`, `+spot/+control/thrusterDutyCycles.m`, `+spot/+control/verifyConsoleController.m`
- **Uses:** [[spot.control.lqrGains|spot-control-lqrGains]], [[spot.control.thrusterDutyCycles|spot-control-thrusterDutyCycles]], [[spot.control.verifyConsoleController|spot-control-verifyConsoleController]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/lqrCommand.m</b> (41 lines; click to show)</summary>

```matlab
   1  function u = lqrCommand(K, measured, desired, desiredRate)
   2  %LQRCOMMAND The diagram's default LQR control law: forces and torque for one platform, one step.
   3  %   u = spot.control.lqrCommand(K, measured, desired, desiredRate) computes what the diagram's
   4  %   block "4 - Control/Default LQR Control (<PLATFORM>)" computes each step:
   5  %     K            3x6 gain K_<PLATFORM> (spot.control.lqrGains)
   6  %     measured     [x y theta xdot ydot thetadot] [m, rad, m/s, rad/s]: *_Measured_States(1:6)
   7  %     desired      [x y theta] the platform is sent to [m, rad]: *_Path
   8  %     desiredRate  d/dt of desired: the block's Discrete Derivative of *_Path, i.e.
   9  %                  (desired - desired one step earlier) / baseRate
  10  %   and returns u = -K*e = [Fx; Fy; Tz] [N, N, N m] in the table frame (*_Controls), where the
  11  %   error is e = measured - desired, with the attitude error wrapped to [-pi, pi) by the block's
  12  %   "Calculate Error" function (copied verbatim below).
  13  %
  14  %   The lab console's Interactive Mode runs this law in Python (lqr_command in
  15  %   +spot/+console/webapp/spot_console/control.py). spot.control.verifyConsoleController checks
  16  %   that the copy below is still the diagram's code, that this function reproduces the forces
  17  %   the diagram logs in a simulation, and that the Python port agrees with it.
  18  %
  19  %   See also spot.control.thrusterDutyCycles, spot.control.lqrGains.
  20  measured = measured(:);
  21  desired = desired(:);
  22  desiredRate = desiredRate(:);
  23  % The block's wiring: Demux of *_Measured_States and of *_Path, five Subtract blocks (measured
  24  % minus desired), Calculate Error for the attitude, then Matrix Multiply by the constant -K
  25  e = [measured(1) - desired(1)
  26       measured(2) - desired(2)
  27       SmoothAngularDifference(desired(3), measured(3))
  28       measured(4) - desiredRate(1)
  29       measured(5) - desiredRate(2)
  30       measured(6) - desiredRate(3)];
  31  u = -K * e;
  32  end
  33  
  34  % ---- verbatim: 4 - Control/Default LQR Control (RED)/Calculate Error/MATLAB Function
  35  function Rz_error = SmoothAngularDifference(Rz_path, Rz_measured)
  36  
  37      Rz_error = mod(Rz_measured-Rz_path+pi, 2*pi)-pi;
  38  
  39  
  40  end
  41  % ---- end verbatim
```

</details>
<!-- /listing -->
