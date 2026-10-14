function K = lqrGains(sys, Q, R)
%LQRGAINS Discrete LQR gain matrix for a platform.
%   K = spot.control.lqrGains(sys, Q, R) with sys from spot.control.stateSpace, the state weights
%   Q = [x y theta xdot ydot thetadot] and input weights R = [Fx Fy Tz] (diagonals). Returns the
%   3x6 gain of u = -K*x. Needs the Control System Toolbox.
K = dlqr(sys.A, sys.B, diag(Q), diag(R));
end
