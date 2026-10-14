function r = inspect(expr)
%INSPECT Details of a workspace variable (or a field of one) for the Workspace panel.
%   r = spot.workspace.inspect('dataClass.RED_Px_m') returns name, class, size and one of:
%     values        numeric/logical matrices (up to 50 x 12 shown)
%     fields        structs: name, class, size and preview of each field
%     series        timeseries: time and data, resampled to at most 600 points
%     text          anything else, as MATLAB displays it (shortened)
if isempty(regexp(expr, '^[A-Za-z]\w*(\.[A-Za-z]\w*|\(\d+\))*$', 'once'))
    error('spot:workspace:inspect', '%s is not a variable name.', expr);
end
x = evalin('base', expr);
r = struct('name', expr, 'class', class(x), ...
           'size', strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x'));
if (isnumeric(x) || islogical(x)) && ismatrix(x)
    r.values = double(x(1:min(end, 50), 1:min(end, 12)));
    r.truncated = any(size(x) > [50 12]);
    if ~isreal(x)
        r.values = real(r.values);
        r.note = 'complex: real parts shown';
    end
elseif isstruct(x) && isscalar(x)
    f = fieldnames(x);
    r.fields = struct('name', f, 'class', '', 'size', '', 'value', '');
    for k = 1:numel(f)
        v = x.(f{k});
        r.fields(k).class = class(v);
        r.fields(k).size = strjoin(arrayfun(@num2str, size(v), 'UniformOutput', false), 'x');
        r.fields(k).value = spot.workspace.preview(v);
    end
elseif isa(x, 'timeseries')
    t = x.Time(:);
    d = double(x.Data);
    if ndims(d) == 3
        d = reshape(d, [], size(d, 3)).';
    end
    d = d(:, 1:min(end, 6));
    n = numel(t);
    pick = unique(round(linspace(1, n, min(n, 600))));
    r.series = struct('t', t(pick).', 'data', d(pick, :).', 'units', char(x.DataInfo.Units));
else
    text = evalc('disp(x)');
    if strlength(text) > 4000
        text = [text(1:4000) newline '...'];
    end
    r.text = text;
end
end
