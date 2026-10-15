function s = merge(base, changes)
    %MERGE Copy the fields of CHANGES over BASE, recursing into structs.
    %   s = spot.params.merge(base, changes). Fields only in base keep their value, so settings
    %   saved by an older version gain any new fields with their defaults. Fields not in base
    %   are ignored.
    %
    %   Inputs
    %     base     struct, e.g. the defaults
    %     changes  struct to copy over it (anything else leaves base unchanged)
    %
    %   Outputs
    %     s        the merged struct
    %
    %   See also spot.params.load, spot.params.update.

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
            % A nested struct: merge it too.
            s.(name) = spot.params.merge(base.(name), changes.(name));
        else
            % A value. JSON arrays come back as columns; keep vectors as rows, like the
            % defaults.
            value = changes.(name);
            if (isnumeric(value) || islogical(value)) && iscolumn(value) && ~isscalar(value)
                value = value.';
            end
            s.(name) = value;
        end
    end
end
