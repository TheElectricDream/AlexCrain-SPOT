function a = tableSlope(position, velocity, slope, damping)
    %TABLESLOPE The table's pull on a floating platform: its slope and the air bearings' drag.
    %   a = spot.sim.tableSlope(position, velocity, tableSlope, damping). The table top's height
    %   (the table slope calibration, spot.params.loadCalibration) is
    %       h = a1*u + a2*v + a3*u^2 + a4*u*v + a5*v^2,   u = x - xc, v = y - yc
    %   The platform slides downhill at -g * grad(h), and its air bearings drag it with
    %   -damping * velocity. The diagram's plant blocks (RED_, BLACK_, BLUE_dynamics) and
    %   spot.arm.Plant add it to the platforms' motion.
    %
    %   Inputs
    %     position  [x y] [m]
    %     velocity  [xdot ydot] [m/s]
    %     slope     [a1 a2 a3 a4 a5 xc yc]; all zeros: a level table
    %     damping   air-bearing drag [1/s]; 0: no drag
    %
    %   Outputs
    %     a         [ax; ay] [m/s^2] in the table frame
    %
    %   See also spot.params.loadCalibration, spot.arm.Plant.

    %#codegen

    %% Constants
    G = 9.80665;   % [m/s^2]

    % Position relative to the height surface's centre.
    u = position(1) - slope(6);
    v = position(2) - slope(7);

    % Downhill pull (-g grad h) and drag.
    a = [-G * (slope(1) + 2 * slope(3) * u + slope(4) * v) - damping * velocity(1);
         -G * (slope(2) + slope(4) * u + 2 * slope(5) * v) - damping * velocity(2)];
end
