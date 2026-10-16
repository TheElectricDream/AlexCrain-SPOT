function K = lqrGains(sys, Q, R)
    %LQRGAINS Discrete LQR gain matrix for a platform.
    %   K = spot.control.lqrGains(sys, Q, R) returns the gain of u = -K*x. Needs the Control
    %   System Toolbox.
    %
    %   Inputs
    %     sys  discrete model (spot.control.stateSpace)
    %     Q    state weights [x y theta xdot ydot thetadot] (the diagonal)
    %     R    input weights [Fx Fy Tz] (the diagonal)
    %
    %   Outputs
    %     K    3x6 gain matrix
    %
    %   See also spot.control.stateSpace, spot.control.lqrCommand.

    K = dlqr(sys.A, sys.B, diag(Q), diag(R));
end
