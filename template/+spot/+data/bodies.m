function b = bodies(ds)
%BODIES Which platforms (and the arm) a dataset can show on the table.
%   b = spot.data.bodies(dataset) returns crafts (cell of RED, BLACK, BLUE) and arm (true/false).
%   A platform needs <NAME>_Px_m, _Py_m and _Rz_rad. For a simulation, the platforms selected in
%   its settings; for an experiment or a file, those whose position isn't all zero (every
%   platform logs every signal, but only the ones in the run move).
sig = ds.signals;
selected = [];
if strcmp(ds.kind, 'simulation') && isstruct(ds.settings) && isfield(ds.settings, 'platforms')
    selected = ds.settings.platforms;
end
b = struct('crafts', {{}}, 'arm', false);
for n = {'RED', 'BLACK', 'BLUE'}
    f = strcat(n{1}, {'_Px_m', '_Py_m', '_Rz_rad'});
    if ~all(isfield(sig, f))
        continue
    end
    if isempty(selected)
        on = any(sig.(f{1}).Data(:)) || any(sig.(f{2}).Data(:));
    else
        on = selected.(n{1});
    end
    if on
        b.crafts{end + 1} = n{1};
    end
end
joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
if ismember('RED', b.crafts) && all(isfield(sig, joints))
    if isempty(selected)
        b.arm = any(cellfun(@(j) any(sig.(j).Data(:)), joints));
    else
        b.arm = selected.ARM;
    end
end
end
