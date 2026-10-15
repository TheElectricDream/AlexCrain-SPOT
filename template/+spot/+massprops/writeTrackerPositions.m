function file = writeTrackerPositions(props, root)
    %WRITETRACKERPOSITIONS Write the PhaseSpace server's tracker_positions_phasespace.txt.
    %   file = spot.massprops.writeTrackerPositions(spot.massprops.compute(s.mass), root)
    %   writes resources/tracker_positions_phasespace.txt.
    %
    %   Inputs
    %     props  mass properties (spot.massprops.compute)
    %     root   project folder
    %
    %   Outputs
    %     file   the file written
    %
    %   See also spot.massprops.trackerPositions.

    file = fullfile(root, 'resources', 'tracker_positions_phasespace.txt');

    % Open for writing; closed on return.
    fid = fopen(file, 'w');
    if fid < 0
        error('spot:massprops:writeTrackerPositions', 'Cannot write %s.', file);
    end
    cleanup = onCleanup(@() fclose(fid));

    fwrite(fid, spot.massprops.trackerPositions(props), 'char');
end
