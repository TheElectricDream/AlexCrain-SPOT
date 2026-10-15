function contents = trackerPositions(props)
    %TRACKERPOSITIONS PhaseSpace marker calibration text from the computed mass properties.
    %   contents = spot.massprops.trackerPositions(spot.massprops.compute(s.mass)) returns the
    %   contents of resources/tracker_positions_phasespace.txt: each LED's position from its
    %   platform's CG [mm]. StreamData reads this file (uploaded to the PhaseSpace server).
    %   Write it with spot.massprops.writeTrackerPositions.
    %
    %   Inputs
    %     props     mass properties (spot.massprops.compute)
    %
    %   Outputs
    %     contents  the file's text, one line per LED
    %
    %   See also spot.massprops.writeTrackerPositions.

    %% Constants
    MM_PER_M = 1000;

    % LED tracker ids per platform, in the corner order of spot.massprops.centerOfGravity.
    ids = struct('RED', [5 3 7 4], 'BLACK', [13 11 9 15], 'BLUE', [8 14 12 10]);

    %% One line per LED
    rows = {};
    for name = {'RED', 'BLACK', 'BLUE'}
        leds = props.(name{1}).leds * MM_PER_M;
        for k = 1:4
            rows{end + 1} = sprintf('tracker_id_%s_%d_pos_string,%f,%f,0', ...
                name{1}, ids.(name{1})(k), leds(k, 1), leds(k, 2)); %#ok<AGROW>
        end
    end
    contents = [strjoin(rows, newline) newline];
end
