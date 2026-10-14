function file = writeTrackerPositions(props, root)
%WRITETRACKERPOSITIONS Write resources/tracker_positions_phasespace.txt for the PhaseSpace server.
%   file = spot.massprops.writeTrackerPositions(spot.massprops.compute(s.mass), projectFolder)
file = fullfile(root, 'resources', 'tracker_positions_phasespace.txt');
fid = fopen(file, 'w');
if fid < 0
    error('spot:massprops:writeTrackerPositions', 'Cannot write %s.', file);
end
cleanup = onCleanup(@() fclose(fid));
fwrite(fid, spot.massprops.trackerPositions(props), 'char');
end
