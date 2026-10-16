function arm = mountGeometry(arm)
    %MOUNTGEOMETRY The arm's geometry for where it is mounted on the Chaser.
    %   arm = spot.arm.mountGeometry(arm) moves the shoulder from the usual mount (+y face, left
    %   side) to arm.mount, the same way the 3D view moves attachments, so the drawing and the
    %   simulation agree.
    %
    %   Inputs
    %     arm   the settings' arm (s.mass.arm): Phi and b0 place the shoulder on the usual
    %           mount. Optional arm.mount: face '+x', '+y' or '-y', side 'left' or 'right'.
    %
    %   Outputs
    %     arm   with Phi and b0 for the shoulder on arm.mount, and q1Offset [rad], the angle
    %           the whole arm is turned by. Without arm.mount, or on the usual mount, Phi and
    %           b0 are unchanged and q1Offset is 0.
    %
    %   See also spot.arm.parameters.

    %% Constants
    FACE = 0.15;          % [m] face distance from the centre (CAD: info.mount.face)
    LATERAL = 0.082735;   % [m] place offset from the face's centre line (CAD: info.mount.lateral)

    %% Mount
    % Each face's angle, keyed by face name with +/- spelled p/m (valid field names).
    angles = struct('px', 0, 'py', pi/2, 'my', -pi/2);
    key = @(f) strrep(strrep(f, '+', 'p'), '-', 'm');

    % The requested mount, or the usual one.
    place = struct('face', '+y', 'side', 'left');
    if isfield(arm, 'mount') && isstruct(arm.mount)
        place = arm.mount;
    end

    % Validate it.
    if ~(ischar(place.face) || isstring(place.face)) || ~isfield(angles, key(char(place.face)))
        error('spot:arm:mountGeometry', ...
            'The arm''s mount face must be +x, +y or -y (the back, -x, has the control panel).');
    end
    if ~any(strcmp(place.side, {'left', 'right'}))
        error('spot:arm:mountGeometry', 'The arm''s mount side must be left or right.');
    end

    %% Shoulder position
    % A mount's origin on the platform, and a 2D rotation.
    origin = @(a, side) [FACE * cos(a) - lat(side) * sin(a), FACE * sin(a) + lat(side) * cos(a)];
    rot = @(a) [cos(a) -sin(a); sin(a) cos(a)];

    % The shoulder in the usual mount's frame.
    a0 = pi/2;                                               % usual mount: +y, left
    s0 = arm.b0 * [cos(arm.Phi), sin(arm.Phi)];              % shoulder there
    inMount = rot(-a0) * (s0 - origin(a0, 'left')).';        % ... in the mount's frame

    % The same point in the requested mount's frame, back on the platform.
    a = angles.(key(char(place.face)));
    s = origin(a, char(place.side)).' + rot(a) * inMount;
    arm.Phi = atan2(s(2), s(1));
    arm.b0 = hypot(s(1), s(2));
    arm.q1Offset = a - a0;
    arm.mount = struct('face', char(place.face), 'side', char(place.side));

    function l = lat(side)
        % Lateral offset of a side [m]: negative on the left.
        l = LATERAL;
        if strcmp(side, 'left')
            l = -LATERAL;
        end
    end
end
