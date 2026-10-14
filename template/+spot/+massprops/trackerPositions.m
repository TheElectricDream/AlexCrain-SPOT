function text = trackerPositions(props)
%TRACKERPOSITIONS PhaseSpace marker calibration text from the computed mass properties.
%   text = spot.massprops.trackerPositions(spot.massprops.compute(s.mass)) returns the contents
%   of resources/tracker_positions_phasespace.txt: each LED's position from its platform's CG
%   in mm. StreamData reads this file (the SPOT GUI uploads it to the PhaseSpace server).
%   Write it with spot.massprops.writeTrackerPositions.
% LED tracker ids per platform, in the corner order of spot.massprops.centerOfGravity
ids = struct('RED', [5 3 7 4], 'BLACK', [13 11 9 15], 'BLUE', [8 14 12 10]);
lines = {};
for name = {'RED', 'BLACK', 'BLUE'}
    leds = props.(name{1}).leds * 1000;
    for k = 1:4
        lines{end + 1} = sprintf('tracker_id_%s_%d_pos_string,%f,%f,0', ...
            name{1}, ids.(name{1})(k), leds(k, 1), leds(k, 2)); %#ok<AGROW>
    end
end
text = [strjoin(lines, newline) newline];
end
