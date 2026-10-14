function u = lqrCommand(K, measured, desired, desiredRate)
%LQRCOMMAND The diagram's default LQR control law: forces and torque for one platform, one step.
%   u = spot.control.lqrCommand(K, measured, desired, desiredRate) computes what the diagram's
%   block "4 - Control/Default LQR Control (<PLATFORM>)" computes each step:
%     K            3x6 gain K_<PLATFORM> (spot.control.lqrGains)
%     measured     [x y theta xdot ydot thetadot] [m, rad, m/s, rad/s]: *_Measured_States(1:6)
%     desired      [x y theta] the platform is sent to [m, rad]: *_Path
%     desiredRate  d/dt of desired: the block's Discrete Derivative of *_Path, i.e.
%                  (desired - desired one step earlier) / baseRate
%   and returns u = -K*e = [Fx; Fy; Tz] [N, N, N m] in the table frame (*_Controls), where the
%   error is e = measured - desired, with the attitude error wrapped to [-pi, pi) by the block's
%   "Calculate Error" function (copied verbatim below).
%
%   The lab console's Interactive Mode runs this law in Python (lqr_command in
%   +spot/+console/webapp/spot_console/control.py). spot.control.verifyConsoleController checks
%   that the copy below is still the diagram's code, that this function reproduces the forces
%   the diagram logs in a simulation, and that the Python port agrees with it.
%
%   See also spot.control.thrusterDutyCycles, spot.control.lqrGains.
measured = measured(:);
desired = desired(:);
desiredRate = desiredRate(:);
% The block's wiring: Demux of *_Measured_States and of *_Path, five Subtract blocks (measured
% minus desired), Calculate Error for the attitude, then Matrix Multiply by the constant -K
e = [measured(1) - desired(1)
     measured(2) - desired(2)
     SmoothAngularDifference(desired(3), measured(3))
     measured(4) - desiredRate(1)
     measured(5) - desiredRate(2)
     measured(6) - desiredRate(3)];
u = -K * e;
end

% ---- verbatim: 4 - Control/Default LQR Control (RED)/Calculate Error/MATLAB Function
function Rz_error = SmoothAngularDifference(Rz_path, Rz_measured)

    Rz_error = mod(Rz_measured-Rz_path+pi, 2*pi)-pi;


end
% ---- end verbatim
