function sys = stateSpace(mass, inertia, Ts)
%STATESPACE Discrete planar rigid-body model of one platform.
%   sys = spot.control.stateSpace(mass, inertia, Ts) returns the zero-order-hold discretisation
%   (sample time Ts [s]) of the double integrator with states [x y theta xdot ydot thetadot] and
%   inputs [Fx Fy Tz]. sys has fields A, B, C, D. Needs the Control System Toolbox.
A = [zeros(3) eye(3); zeros(3, 6)];
B = [zeros(3); diag([1/mass, 1/mass, 1/inertia])];
C = eye(6);
D = zeros(6, 3);
d = c2d(ss(A, B, C, D), Ts, 'zoh');
sys = struct('A', d.A, 'B', d.B, 'C', d.C, 'D', d.D);
end
