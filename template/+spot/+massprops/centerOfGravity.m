function r = centerOfGravity(scales)
    %CENTEROFGRAVITY Mass, centre of gravity, thruster offsets and LED positions from scales.
    %   r = spot.massprops.centerOfGravity([A B C])
    %
    %   Inputs
    %     scales  the three scale readings [kg] (layout of 2025-08-29, relative to the centre of
    %             the 0.3 m body; resources/COMConfig.png):
    %               A  left-middle edge   at body (x, y) = (0, 0.15) m
    %               B  right-back corner  at (-0.15, -0.15) m
    %               C  right-front corner at ( 0.15, -0.15) m
    %
    %   Outputs
    %     r       struct with:
    %               mass       total mass [kg]
    %               cg         [x y] of the centre of gravity from the body centre [m]
    %               thrusters  8x1 distance of each thruster's line of action from the CG [mm]
    %                          (thruster_dist2CG_*; layout in resources/ThrusterConfig.png)
    %               leds       4x2 LED positions from the CG [m], corners in the order
    %                          (+x,+y), (+x,-y), (-x,-y), (-x,+y)
    %
    %   See also spot.massprops.compute.

    %% Constants
    HALF_SIDE = 0.15;         % [m] body centre to edge
    THRUSTER_INSET = 0.081;   % [m] thrusters in from the edge (measured 2025-08-29)
    LED_INSET = 0.013;        % [m] LEDs in from the edges
    MM_PER_M = 1000;

    %% Mass and centre of gravity
    A = scales(1);
    B = scales(2);
    C = scales(3);
    M = A + B + C;
    if M <= 0
        error('spot:massprops:centerOfGravity', ...
            'The scale readings must add up to more than 0 kg.');
    end

    % Readings weighted by each scale's position.
    X = (A*0 + B*(-HALF_SIDE) + C*HALF_SIDE) / M;
    Y = (A*HALF_SIDE + B*(-HALF_SIDE) + C*(-HALF_SIDE)) / M;

    %% Thruster offsets from the CG [mm]
    e = HALF_SIDE - THRUSTER_INSET;
    thrusters = [ e - Y
                 -(e + Y)
                  e - X
                 -(e + X)
                  e + Y
                 -(e - Y)
                  e + X
                 -(e - X)] * MM_PER_M;

    %% LED positions from the CG [m]
    edge = HALF_SIDE - LED_INSET;
    leds = [ edge - X,  edge - Y
             edge - X, -edge - Y
            -edge - X, -edge - Y
            -edge - X,  edge - Y];

    r = struct('mass', M, 'cg', [X Y], 'thrusters', thrusters, 'leds', leds);
end
