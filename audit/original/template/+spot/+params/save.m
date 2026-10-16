function save(s, file)
    %SAVE Write settings to a JSON file (readable, and diff-friendly in version control).
    %   spot.params.save(s, file)
    %
    %   Inputs
    %     s     settings
    %     file  JSON file to write
    %
    %   See also spot.params.load.

    json = jsonencode(s, 'PrettyPrint', true);

    % Open for writing; closed on return.
    fid = fopen(file, 'w');
    if fid < 0
        error('spot:params:save', 'Cannot write %s.', file);
    end
    cleanup = onCleanup(@() fclose(fid));

    fwrite(fid, json, 'char');
end
