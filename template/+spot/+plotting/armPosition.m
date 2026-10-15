function P = armPosition(base, q, arm)
    %ARMPOSITION Joint positions of the robotic arm on RED, in the table frame.
    %   P = spot.plotting.armPosition([x y theta], [q1 q2 q3], arm). Also accepts N poses and
    %   angles as rows; P is then 4x2xN.
    %
    %   Inputs
    %     base  RED's [x y theta] [m, m, rad], or N x 3
    %     q     shoulder, elbow and wrist angles [rad], or N x 3
    %     arm   arm geometry (s.mass.arm: Phi, b0, a1, b1, a2, b2, a3, b3). For an arm mounted
    %           elsewhere, the geometry from spot.arm.mountGeometry (its q1Offset turns the arm
    %           with its mount face).
    %
    %   Outputs
    %     P     4x2 [x y] of the shoulder, elbow, wrist and end effector [m]
    %
    %   See also spot.plotting.drawArm, spot.arm.mountGeometry.

    x = base(:, 1);
    y = base(:, 2);
    th = base(:, 3);

    %% Link directions [rad]
    b1 = th + arm.Phi;                  % shoulder mount direction
    offset = 0;
    if isfield(arm, 'q1Offset')
        offset = arm.q1Offset;
    end
    b2 = th + q(:, 1) + pi/2 + offset;  % proximal link
    b3 = b2 + q(:, 2);                  % distal link
    b4 = b3 + q(:, 3);                  % end effector

    %% Joint positions: shoulder (s), elbow (e), wrist (w), tip (t)
    sx = x + arm.b0 * cos(b1);
    sy = y + arm.b0 * sin(b1);
    ex = sx + (arm.a1 + arm.b1) * cos(b2);
    ey = sy + (arm.a1 + arm.b1) * sin(b2);
    wx = ex + (arm.a2 + arm.b2) * cos(b3);
    wy = ey + (arm.a2 + arm.b2) * sin(b3);
    tx = wx + (arm.a3 + arm.b3) * cos(b4);
    ty = wy + (arm.a3 + arm.b3) * sin(b4);

    %% As 4 x 2 x N
    n = numel(x);
    P = zeros(4, 2, n);
    P(:, 1, :) = reshape([sx ex wx tx].', 4, 1, n);
    P(:, 2, :) = reshape([sy ey wy ty].', 4, 1, n);
end
