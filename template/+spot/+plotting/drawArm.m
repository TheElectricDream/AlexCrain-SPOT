function links = drawArm(joints)
    %DRAWARM Outlines of the arm's three links, for publication figures.
    %   links = spot.plotting.drawArm(P)
    %
    %   Inputs
    %     joints  4 x 2 joint positions [m] (spot.plotting.armPosition): shoulder, elbow, wrist,
    %             end effector
    %
    %   Outputs
    %     links   1 x 3 cell of outlines (proximal, distal and end-effector links) for patch
    %
    %   From the lab's GetStarted/PlottingFunctions/DrawARM.m (C. Savytska, 2024).
    %
    %   See also spot.plotting.armPosition, spot.plotting.drawSpacecraft.

    %% Constants
    HALF_WIDTH = 0.015;      % [m] half width of a link
    GRIP_BASE = 0.01;        % [m] gripper jaws' base, behind the link's centre
    GRIP_HALF_SPAN = 0.045;  % [m] gripper jaws' half span

    links = cell(1, 3);
    for k = 1:3
        % The link from joint k to joint k + 1.
        a = joints(k, :);
        d = joints(k + 1, :) - a;
        len = norm(d);

        % Its outline along x, centred at 0; the end effector has its gripper.
        if k < 3
            shape = [len/2 -HALF_WIDTH; len/2 HALF_WIDTH; -len/2 HALF_WIDTH; -len/2 -HALF_WIDTH; ...
                     len/2 -HALF_WIDTH];
        else
            shape = [len/2 -HALF_WIDTH; len/2 HALF_WIDTH; -GRIP_BASE GRIP_HALF_SPAN; ...
                     -GRIP_BASE HALF_WIDTH; -len/2 HALF_WIDTH; -len/2 -HALF_WIDTH; ...
                     -GRIP_BASE -HALF_WIDTH; -GRIP_BASE -GRIP_HALF_SPAN; len/2 -HALF_WIDTH];
        end

        % Rotated along the link and moved to its middle.
        th = atan2(d(2), d(1));
        R = [cos(th), -sin(th); sin(th), cos(th)];
        links{k} = (R * shape.').' + (a + d / 2);
    end
end
