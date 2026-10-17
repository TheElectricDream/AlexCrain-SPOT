function m = markerLayout()
    %MARKERLAYOUT Each platform's PhaseSpace LEDs: their ids and default positions.
    %   m = spot.massprops.markerLayout() returns, for RED, BLACK and BLUE, the ids of the four
    %   LEDs on the platform's top and where they sit when not measured: LED_INSET in from the
    %   corners of the body (Constants). The project's calibration file holds the measured
    %   positions (spot_calibration.json, "markers"; spot.params.loadCalibration), which the
    %   Simulation tab's Thruster and LED Layout window edits. StreamData knows the LEDs by
    %   these ids, so they are fixed.
    %
    %   Outputs
    %     m  struct with RED, BLACK, BLUE, each with:
    %          ids   1x4 LED ids, in the corner order (+x,+y), (+x,-y), (-x,-y), (-x,+y)
    %          leds  4x2 [x y] of each LED from the body centre [m]
    %
    %   See also spot.params.loadCalibration, spot.massprops.centerOfGravity.

    %% Constants
    EDGE = Constants.LED_DEFAULT_OFFSET;   % [m] LEDs from the body centre, each way

    %% The layout
    % The four corners, in the order (+x,+y), (+x,-y), (-x,-y), (-x,+y), and each platform's ids.
    corners = EDGE * Constants.LED_CORNERS;
    m = struct('RED', struct('ids', Constants.LED_IDS_RED, 'leds', corners), ...
               'BLACK', struct('ids', Constants.LED_IDS_BLACK, 'leds', corners), ...
               'BLUE', struct('ids', Constants.LED_IDS_BLUE, 'leds', corners));
end
