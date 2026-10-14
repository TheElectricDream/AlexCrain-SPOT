function pts = drawSpacecraft(x, y, theta, id)
%DRAWSPACECRAFT Outline of a SPOT platform on the table, for publication figures.
%   pts = spot.plotting.drawSpacecraft(x, y, theta, id) returns the N x 2 outline of a platform
%   at position (x, y) [m] and attitude theta [rad], for patch(pts(:,1), pts(:,2), ...). The line
%   from the centre to the +x face marks the body +x axis. id picks the shape:
%     1  chaser (docking grapple, +y)          5  chaser (arm joint only)
%     2  target (docking cone, +y)             6  chaser (docking grapple, +x)
%     3  generic platform                      7  generic platform with solar panels
%     4  target (docking cone and solar panels)
%   From the lab's GetStarted/PlottingFunctions/DrawSpacecraft.m (C. Savytska, 2024).
sc = shape(id);
R = [cos(theta), -sin(theta); sin(theta), cos(theta)];
pts = (R * sc.').' + [x, y];
end

function sc = shape(id)
% The outline around (0, 0), before rotation
cm = 1 / 100;
r = 0.15;                       % half side [m]
excess = 2.477 * cm; shrink = 1 * cm; bracket = 6.5 * cm;
cyl = 8.07 * cm; cone = 5.5 * cm; coneAngle = 30 * pi / 180;
grapple = 5.5 * cm; grappleAngle = 20 * pi / 180;
panelL = 0.45; panelW = 0.02;
g = r - (excess + shrink + bracket);
switch id
    case 1
        sc = [0 0; r 0; r r; g r; g + grapple * sin(grappleAngle), r + grapple; ...
              g + bracket - grapple * sin(grappleAngle), r + grapple; g + bracket, r; -r r; -r -r; r -r; r 0];
    case 2
        sc = [0 0; r 0; r r; g r; g, r + cyl; g - cone * sin(coneAngle), r + cyl + cone; ...
              g + bracket + cone * sin(coneAngle), r + cyl + cone; g + bracket, r + cyl; g + bracket, r; ...
              -r r; -r -r; r -r; r 0];
    case 4
        sc = [0 0; r 0; r r; g r; g, r + cyl; g - cone * sin(coneAngle), r + cyl + cone; ...
              g + bracket + cone * sin(coneAngle), r + cyl + cone; g + bracket, r + cyl; g + bracket, r; ...
              -r r; -r, -panelW / 2; -r - panelL, -panelW / 2; -r - panelL, panelW / 2; -r, panelW / 2; ...
              -r -r; r -r; r, panelW / 2; r + panelL, panelW / 2; r + panelL, -panelW / 2; r, -panelW / 2; r 0];
    case 5
        sc = [0 0; r 0; r r; r - 0.08, r; r - 0.08, r + 0.08; r - 0.02, r + 0.08; r - 0.02, r; ...
              -r r; -r -r; r -r; r 0];
    case 6
        sc = [0 0; r 0; r r; -r r; -r -r; r -r; r -g; r + grapple, -g - grapple * sin(grappleAngle); ...
              r + grapple, -g - bracket + grapple * sin(grappleAngle); r, -g - bracket; r 0];
    case 7
        sc = [0 0; r 0; r r; -r r; -r, -panelW / 2; -r - panelL, -panelW / 2; -r - panelL, panelW / 2; ...
              -r, panelW / 2; -r -r; r -r; r, panelW / 2; r + panelL, panelW / 2; r + panelL, -panelW / 2; ...
              r, -panelW / 2; r 0];
    otherwise   % 3: generic platform
        sc = [0 0; r 0; r r; -r r; -r -r; r -r; r 0];
end
end
