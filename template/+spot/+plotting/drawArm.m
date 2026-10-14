function links = drawArm(joints)
%DRAWARM Outlines of the arm's three links, for publication figures.
%   links = spot.plotting.drawArm(P) takes the joint positions from spot.plotting.armPosition
%   (4 x 2: shoulder, elbow, wrist, end effector) and returns a 1 x 3 cell of outlines (proximal,
%   distal and end-effector links) for patch.
%   From the lab's GetStarted/PlottingFunctions/DrawARM.m (C. Savytska, 2024).
w = 0.015;   % half width of a link [m]
links = cell(1, 3);
for k = 1:3
    a = joints(k, :);
    d = joints(k + 1, :) - a;
    L = norm(d);
    if k < 3
        shape = [L/2 -w; L/2 w; -L/2 w; -L/2 -w; L/2 -w];
    else   % the end effector, with its gripper
        shape = [L/2 -w; L/2 w; -0.01 0.045; -0.01 w; -L/2 w; -L/2 -w; -0.01 -w; -0.01 -0.045; L/2 -w];
    end
    th = atan2(d(2), d(1));
    R = [cos(th), -sin(th); sin(th), cos(th)];
    links{k} = (R * shape.').' + (a + d / 2);
end
end
