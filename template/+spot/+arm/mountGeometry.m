function arm = mountGeometry(arm)
%MOUNTGEOMETRY The arm's geometry for where it is mounted on the Chaser.
%   arm = spot.arm.mountGeometry(arm) takes the settings' arm (s.mass.arm), whose Phi and b0 place
%   the shoulder for the arm on its usual mount (the arm coupler on the +y face, left as seen from
%   outside), and returns it for arm.mount (face '+x', '+y' or '-y'; side 'left' or 'right'; the
%   Spacecraft Designer's places): Phi and b0 for the shoulder there, and q1Offset, the angle the
%   whole arm is turned by (the shoulder's 0 is straight out from its face, wherever it is).
%   The mount moves as the 3D view moves attachments (the place's frame on the face, the shoulder
%   at the same spot in it), so the drawing and the simulation agree.
%   Without arm.mount, or on the usual mount, Phi and b0 are unchanged and q1Offset is 0.
FACE = 0.15;          % [m] the faces' distance from the centre (CAD: info.mount.face)
LATERAL = 0.082735;   % [m] a place's distance from the face's centre line (CAD: info.mount.lateral)
angles = struct('px', 0, 'py', pi/2, 'my', -pi/2);
key = @(f) strrep(strrep(f, '+', 'p'), '-', 'm');
place = struct('face', '+y', 'side', 'left');
if isfield(arm, 'mount') && isstruct(arm.mount)
    place = arm.mount;
end
if ~(ischar(place.face) || isstring(place.face)) || ~isfield(angles, key(char(place.face)))
    error('spot:arm:mountGeometry', 'The arm''s mount face must be +x, +y or -y (the back, -x, has the control panel).');
end
if ~any(strcmp(place.side, {'left', 'right'}))
    error('spot:arm:mountGeometry', 'The arm''s mount side must be left or right.');
end
origin = @(a, side) [FACE * cos(a) - lat(side) * sin(a), FACE * sin(a) + lat(side) * cos(a)];
rot = @(a) [cos(a) -sin(a); sin(a) cos(a)];
a0 = pi/2;                                               % the usual mount: +y, left
s0 = arm.b0 * [cos(arm.Phi), sin(arm.Phi)];              % the shoulder there
inMount = rot(-a0) * (s0 - origin(a0, 'left')).';        % ... in the mount's frame
a = angles.(key(char(place.face)));
s = origin(a, char(place.side)).' + rot(a) * inMount;
arm.Phi = atan2(s(2), s(1));
arm.b0 = hypot(s(1), s(2));
arm.q1Offset = a - a0;
arm.mount = struct('face', char(place.face), 'side', char(place.side));

    function l = lat(side)
        l = LATERAL;
        if strcmp(side, 'left'), l = -LATERAL; end
    end
end
