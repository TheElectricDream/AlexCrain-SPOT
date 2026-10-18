function value = parseConstant(text, current)
    %PARSECONSTANT A value typed in the console, as the type of the constant it replaces.
    %   value = spot.params.parseConstant(text, current) reads text as a value of the same kind
    %   as current (the constant's value now): true/false for a logical (a vector of them in
    %   brackets), a number for a number (NaN and Inf allowed), a row vector "[1 2 3]" or
    %   "1, 2, 3", a matrix "[1 2; 3 4]", "[]" for none, or the text itself for a string.
    %   Nothing is evaluated: only numbers, true and false are accepted, so no MATLAB code
    %   runs.
    %
    %   Inputs
    %     text     what was typed
    %     current  the constant's current value (its type decides the reading)
    %
    %   Outputs
    %     value    the new value
    %
    %   Errors with spot:params:parseConstant when text isn't a value of that kind.
    %
    %   See also spot.params.constantsCatalog, spot.params.writeConstants.

    text = strtrim(char(text));

    %% Text
    if ischar(current) || isstring(current)
        value = text;
        return
    end

    %% Numbers and logicals: [rows; separated] of elements
    inner = regexprep(text, '^\[(.*)\]$', '$1');
    if isempty(strtrim(inner))
        value = zeros(0, 0);
        if islogical(current)
            value = false(0, 0);
        end
        return
    end
    rows = strsplit(inner, ';');
    parsed = cell(numel(rows), 1);
    for r = 1:numel(rows)
        items = regexp(strtrim(rows{r}), '[\s,]+', 'split');
        items = items(~cellfun(@isempty, items));
        values = zeros(1, numel(items));
        for c = 1:numel(items)
            values(c) = element(items{c}, islogical(current), text);
        end
        parsed{r} = values;
    end
    widths = cellfun(@numel, parsed);
    if any(widths ~= widths(1))
        error('spot:params:parseConstant', ...
              '"%s": every row of a matrix needs the same number of values.', text);
    end
    value = vertcat(parsed{:});
    if islogical(current)
        value = logical(value);
    end
end

function v = element(item, isLogical, text)
    % One element: true/false for a logical, a number otherwise.
    if isLogical
        switch lower(item)
            case {'true', '1'}
                v = 1;
            case {'false', '0'}
                v = 0;
            otherwise
                error('spot:params:parseConstant', '"%s": use true or false.', text);
        end
        return
    end
    v = str2double(item);
    if isnan(v) && ~strcmpi(item, 'nan')
        error('spot:params:parseConstant', '"%s": %s is not a number.', text, item);
    end
end
