function r = centerOfGravity(scales)
%CENTEROFGRAVITY Mass, centre of gravity, thruster offsets and LED positions from scale readings.
%   r = spot.massprops.centerOfGravity([A B C]) with the three scale readings in kg:
%     A  left-middle edge   at body (x, y) = (0, 0.15) m
%     B  right-back corner  at (-0.15, -0.15) m
%     C  right-front corner at ( 0.15, -0.15) m
%   (scale layout of 2025-08-29, relative to the centre of the 0.3 m body; see
%   resources/COMConfig.png). Returns:
%     mass        total mass [kg]
%     cg          [x y] of the centre of gravity from the body centre [m]
%     thrusters   8x1 distance of each thruster's line of action from the CG [mm]
%                 (thruster_dist2CG_* in the diagrams; layout in resources/ThrusterConfig.png)
%     leds        4x2 LED positions from the CG [m], corners in the order
%                 (+x,+y), (+x,-y), (-x,-y), (-x,+y), each 13 mm in from the edges
A = scales(1); B = scales(2); C = scales(3);
M = A + B + C;
if M <= 0
    error('spot:massprops:centerOfGravity', 'The scale readings must add up to more than 0 kg.');
end
X = (A*0 + B*(-0.15) + C*0.15) / M;
Y = (A*0.15 + B*(-0.15) + C*(-0.15)) / M;

% Thrusters sit 81 mm in from the body edge (measured 2025-08-29)
e = 0.15 - 0.081;
thr = [ e - Y
       -(e + Y)
        e - X
       -(e + X)
        e + Y
       -(e - Y)
        e + X
       -(e - X)] * 1000;

edge = 0.15 - 0.013;
leds = [ edge - X,  edge - Y
         edge - X, -edge - Y
        -edge - X, -edge - Y
        -edge - X,  edge - Y];

r = struct('mass', M, 'cg', [X Y], 'thrusters', thr, 'leds', leds);
end
