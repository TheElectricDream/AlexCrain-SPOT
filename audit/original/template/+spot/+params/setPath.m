function s = setPath(s, path, value)
    %SETPATH Set one value of a settings struct by its dotted path (as the page names it).
    %   s = spot.params.setPath(s, 'mass.RED.scales.0', 6.2) follows the path through struct
    %   fields; a numeric segment is a 0-based index into a vector, as in the page's JavaScript
    %   (timing.subPhases.2 is the third sub-phase). A whole vector is set as a row.
    %
    %   Inputs
    %     s      settings struct
    %     path   dotted path (fields and 0-based indices)
    %     value  the new value
    %
    %   Outputs
    %     s      the settings with the value set
    %
    %   See also spot.params.fromConstants, spot.worker.Worker.

    keys = strsplit(char(path), '.');
    if (isnumeric(value) || islogical(value)) && ~isscalar(value)
        value = reshape(value, 1, []);
    end
    s = assign(s, keys, value);
end

function s = assign(s, keys, value)
    % Recursive assignment along keys.
    key = keys{1};
    index = str2double(key);
    if isempty(keys(2:end))
        if ~isnan(index)
            if ~(isnumeric(s) || islogical(s)) || index + 1 > numel(s)
                error('spot:params:setPath', 'No element %s to set.', key);
            end
            s(index + 1) = value;
        else
            if ~isstruct(s) || ~isfield(s, key)
                error('spot:params:setPath', 'No setting %s.', key);
            end
            s.(key) = value;
        end
        return
    end
    if ~isnan(index)
        error('spot:params:setPath', 'A path can only index into a vector at its end.');
    end
    if ~isstruct(s) || ~isfield(s, key)
        error('spot:params:setPath', 'No setting %s.', key);
    end
    s.(key) = assign(s.(key), keys(2:end), value);
end
