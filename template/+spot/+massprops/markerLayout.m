function m = markerLayout()
    %MARKERLAYOUT Each platform's PhaseSpace LEDs: their ids and default positions.
    %   m = spot.massprops.markerLayout() returns, for RED, BLACK and BLUE, the ids of the four
    %   LEDs on the platform's top and where they sit when not measured: 13 mm in from the
    %   corners of the 0.3 m body. The project's calibration file holds the measured positions
    %   (spot_calibration.json, "markers"; spot.params.loadCalibration), which the Simulation
    %   tab's Thruster and LED Layout window edits. StreamData knows the LEDs by these ids, so
    %   they are fixed.
    %
    %   Outputs
    %     m  struct with RED, BLACK, BLUE, each with:
    %          ids   1x4 LED ids, in the corner order (+x,+y), (+x,-y), (-x,-y), (-x,+y)
    %          leds  4x2 [x y] of each LED from the body centre [m]
    %
    %   See also spot.params.loadCalibration, spot.massprops.centerOfGravity.

    %% Constants
    HALF_SIDE = 0.15;    % [m] body centre to edge
    LED_INSET = 0.013;   % [m] LEDs in from the edges

    %% The layout
    edge = HALF_SIDE - LED_INSET;
    corners = edge * [ 1  1
                       1 -1
                      -1 -1
                      -1  1];
    m = struct('RED', struct('ids', [5 3 7 4], 'leds', corners), ...
               'BLACK', struct('ids', [13 11 9 15], 'leds', corners), ...
               'BLUE', struct('ids', [8 14 12 10], 'leds', corners));
end
