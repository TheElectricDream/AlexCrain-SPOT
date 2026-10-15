function u = lqrCommand(K, measured, desired, desiredRate)
    %LQRCOMMAND The diagram's default LQR control law: forces and torque for one platform, one step.
    %   u = spot.control.lqrCommand(K, measured, desired, desiredRate) computes what the
    %   diagram's "4 - Control/Default LQR Control (<PLATFORM>)" block computes each step.
    %
    %   Inputs
    %     K            3x6 gain K_<PLATFORM> (spot.control.lqrGains)
    %     measured     [x y theta xdot ydot thetadot] [m, rad, m/s, rad/s] (*_Measured_States)
    %     desired      [x y theta] target [m, rad] (*_Path)
    %     desiredRate  d/dt of desired: (desired - desired one step earlier) / baseRate
    %
    %   Outputs
    %     u            [Fx; Fy; Tz] [N, N, N m] in the table frame (*_Controls): u = -K*e, with
    %                  e = measured - desired and the attitude error wrapped to [-pi, pi)
    %
    %   The attitude error uses the block's "Calculate Error" code, copied verbatim below. The
    %   lab console's Interactive Mode runs this law in Python (lqr_command in
    %   spot_console/control.py); spot.control.verifyConsoleController checks the copy, this
    %   function and the Python port against the diagram.
    %
    %   See also spot.control.thrusterDutyCycles, spot.control.lqrGains.

    measured = measured(:);
    desired = desired(:);
    desiredRate = desiredRate(:);

    % Error, as the block wires it: measured minus desired, attitude wrapped.
    e = [measured(1) - desired(1)
         measured(2) - desired(2)
         SmoothAngularDifference(desired(3), measured(3))
         measured(4) - desiredRate(1)
         measured(5) - desiredRate(2)
         measured(6) - desiredRate(3)];

    % Control law.
    u = -K * e;
end

% ---- verbatim: 4 - Control/Default LQR Control (RED)/Calculate Error/MATLAB Function
function Rz_error = SmoothAngularDifference(Rz_path, Rz_measured)

    Rz_error = mod(Rz_measured-Rz_path+pi, 2*pi)-pi;


end
% ---- end verbatim
