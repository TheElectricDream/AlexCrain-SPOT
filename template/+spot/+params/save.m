function save(s, file)
%SAVE Write settings to a JSON file (readable, and diff-friendly in version control).
%   spot.params.save(s, file)
text = jsonencode(s, 'PrettyPrint', true);
fid = fopen(file, 'w');
if fid < 0
    error('spot:params:save', 'Cannot write %s.', file);
end
cleanup = onCleanup(@() fclose(fid));
fwrite(fid, text, 'char');
end
