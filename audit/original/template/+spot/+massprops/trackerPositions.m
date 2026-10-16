function contents = trackerPositions(props)
    %TRACKERPOSITIONS PhaseSpace marker calibration text from the computed mass properties.
    %   contents = spot.massprops.trackerPositions(spot.massprops.compute(s.mass, markers))
    %   returns what the lab console keeps in tracker_positions_phasespace.txt, next to
    %   StreamData: each LED's position from its platform's CG [mm]. spot.params.apply puts it
    %   in the console's derived values (trackerPositions), and the Experiment tab's Start
    %   Cameras sends it to the lab console, which writes the file before StreamData starts.
    %
    %   Inputs
    %     props     mass properties (spot.massprops.compute)
    %
    %   Outputs
    %     contents  the file's text, one line per LED
    %
    %   See also spot.massprops.compute, spot.massprops.markerLayout.

    %% Constants
    MM_PER_M = 1000;

    %% One line per LED
    rows = {};
    for name = {'RED', 'BLACK', 'BLUE'}
        leds = props.(name{1}).leds * MM_PER_M;
        ids = props.(name{1}).ledIds;
        for k = 1:4
            rows{end + 1} = sprintf('tracker_id_%s_%d_pos_string,%f,%f,0', ...
                name{1}, ids(k), leds(k, 1), leds(k, 2)); %#ok<AGROW>
        end
    end
    contents = [strjoin(rows, newline) newline];
end
