function s = merge(base, changes)
%MERGE Copy the fields of CHANGES over BASE, recursing into structs.
%   s = spot.params.merge(base, changes). Fields only in BASE keep their value, so settings saved
%   by an older version gain any new fields with their defaults. Fields not in BASE are ignored.
s = base;
if ~isstruct(changes)
    return
end
for f = fieldnames(changes).'
    name = f{1};
    if ~isfield(base, name)
        continue
    end
    if isstruct(base.(name)) && isscalar(base.(name)) && isstruct(changes.(name))
        s.(name) = spot.params.merge(base.(name), changes.(name));
    else
        value = changes.(name);
        % JSON arrays come back as columns; keep vectors as rows, like the defaults
        if (isnumeric(value) || islogical(value)) && iscolumn(value) && ~isscalar(value)
            value = value.';
        end
        s.(name) = value;
    end
end
end
