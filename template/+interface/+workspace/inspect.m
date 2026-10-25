function r = inspect(expr)
    %INSPECT Details of a workspace variable (or a field of one) for the Workspace panel.
    %   r = interface.workspace.inspect('dataClass.RED_Px_m')
    %
    %   Inputs
    %     expr  a variable name, with optional .field and (index) parts
    %
    %   Outputs
    %     r     name, class, size and one of:
    %             values  numeric/logical matrices (up to 50 x 12 shown)
    %             fields  structs: name, class, size and preview of each field
    %             series  timeseries: time and data, resampled to at most 600 points
    %             text    anything else, as MATLAB displays it (shortened)
    %
    %   See also interface.workspace.list, interface.workspace.preview.

    %% Constants
    MAX_ROWS = 50;          % matrix values shown
    MAX_COLS = 12;
    MAX_SERIES = 6;         % timeseries columns
    MAX_POINTS = 600;       % timeseries samples
    MAX_TEXT = 4000;        % characters of displayed text

    %% The value
    % Only names, fields and indices: nothing else is evaluated.
    if isempty(regexp(expr, '^[A-Za-z]\w*(\.[A-Za-z]\w*|\(\d+\))*$', 'once'))
        error('spot:workspace:inspect', '%s is not a variable name.', expr);
    end
    x = evalin('base', expr);
    r = struct('name', expr, 'class', class(x), ...
               'size', strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x'));

    %% Details by kind
    if (isnumeric(x) || islogical(x)) && ismatrix(x)

        % The top-left corner; real parts of complex values.
        r.values = double(x(1:min(end, MAX_ROWS), 1:min(end, MAX_COLS)));
        r.truncated = any(size(x) > [MAX_ROWS MAX_COLS]);
        if ~isreal(x)
            r.values = real(r.values);
            r.note = 'complex: real parts shown';
        end

    elseif isstruct(x) && isscalar(x)

        % Each field's class, size and preview.
        f = fieldnames(x);
        r.fields = struct('name', f, 'class', '', 'size', '', 'value', '');
        for k = 1:numel(f)
            value = x.(f{k});
            r.fields(k).class = class(value);
            r.fields(k).size = strjoin(arrayfun(@num2str, size(value), 'UniformOutput', false), ...
                'x');
            r.fields(k).value = interface.workspace.preview(value);
        end

    elseif isa(x, 'timeseries')

        % The first columns, at evenly spaced samples.
        t = x.Time(:);
        data = double(x.Data);
        if ndims(data) == 3
            data = reshape(data, [], size(data, 3)).';
        end
        data = data(:, 1:min(end, MAX_SERIES));
        n = numel(t);
        pick = unique(round(linspace(1, n, min(n, MAX_POINTS))));
        r.series = struct('t', t(pick).', 'data', data(pick, :).', ...
                          'units', char(x.DataInfo.Units));

    else

        % As MATLAB displays it (x is used by name inside the evalc string).
        shown = evalc('disp(x)');
        if strlength(shown) > MAX_TEXT
            shown = [shown(1:MAX_TEXT) newline '...'];
        end
        r.text = shown;
    end
end
