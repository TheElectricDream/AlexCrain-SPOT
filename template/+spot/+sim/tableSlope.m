function a = tableSlope(position, velocity, slope, damping)
%TABLESLOPE The table's pull on a floating platform: its slope and the air bearings' drag.
%   a = spot.sim.tableSlope(position, velocity, tableSlope, damping) is the acceleration [ax; ay]
%   [m/s^2] in the table frame of a platform floating at POSITION [x y] [m] with VELOCITY
%   [xdot ydot] [m/s], on the table top whose height is (the table slope calibration, Calibration
%   tab; spot.params.loadCalibration)
%       h = a1*u + a2*v + a3*u^2 + a4*u*v + a5*v^2,   u = x - xc, v = y - yc
%   with tableSlope = [a1 a2 a3 a4 a5 xc yc]: it slides downhill at -g * grad(h), and its air
%   bearings drag it with -DAMPING * velocity (DAMPING [1/s]). All zeros: a level table, no drag.
%   The diagram's plant blocks (BLACK_dynamics, BLUE_dynamics, RED_dynamics) and spot.arm.Plant add
%   it to the platforms' motion.

%#codegen
g = 9.80665;
u = position(1) - slope(6);
v = position(2) - slope(7);
a = [-g * (slope(1) + 2 * slope(3) * u + slope(4) * v) - damping * velocity(1);
     -g * (slope(2) + slope(4) * u + 2 * slope(5) * v) - damping * velocity(2)];
end
