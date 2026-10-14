function P = armPosition(base, q, arm)
%ARMPOSITION Joint positions of the robotic arm on RED, in the table frame.
%   P = spot.plotting.armPosition([x y theta], [q1 q2 q3], arm) with RED's position [m] and
%   attitude [rad], the shoulder, elbow and wrist angles [rad] and the arm geometry
%   (s.mass.arm: Phi, b0, a1, b1, a2, b2, a3, b3). Returns a 4x2 matrix of [x y] for the
%   shoulder, elbow, wrist and end effector.
%   Accepts column vectors of poses and angles (N samples) too; P is then 4x2xN.
%   For an arm mounted elsewhere, give the geometry from spot.arm.mountGeometry (its q1Offset turns
%   the arm with its mount face).
x = base(:, 1); y = base(:, 2); th = base(:, 3);
b1 = th + arm.Phi;                  % shoulder mount direction
offset = 0;
if isfield(arm, 'q1Offset'), offset = arm.q1Offset; end
b2 = th + q(:, 1) + pi/2 + offset;  % proximal link
b3 = b2 + q(:, 2);                  % distal link
b4 = b3 + q(:, 3);                  % end effector
sx = x + arm.b0 * cos(b1);              sy = y + arm.b0 * sin(b1);
ex = sx + (arm.a1 + arm.b1) * cos(b2);  ey = sy + (arm.a1 + arm.b1) * sin(b2);
wx = ex + (arm.a2 + arm.b2) * cos(b3);  wy = ey + (arm.a2 + arm.b2) * sin(b3);
tx = wx + (arm.a3 + arm.b3) * cos(b4);  ty = wy + (arm.a3 + arm.b3) * sin(b4);
n = numel(x);
P = zeros(4, 2, n);
P(:, 1, :) = reshape([sx ex wx tx].', 4, 1, n);
P(:, 2, :) = reshape([sy ey wy ty].', 4, 1, n);
end
