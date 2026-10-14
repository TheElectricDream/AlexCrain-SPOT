function t = preview(x)
%PREVIEW Short text for a value, like the Value column of MATLAB's Workspace browser.
%   t = spot.workspace.preview(x) returns '' when the value is too large to show in a line.
t = '';
sz = strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x');
if (isnumeric(x) || islogical(x)) && ismatrix(x)
    if isempty(x)
        t = '[]';
    elseif isscalar(x)
        t = num2str(x, 6);
    elseif numel(x) <= 10
        t = mat2str(x, 5);
    else
        t = sprintf('%s %s', sz, class(x));
    end
elseif ischar(x) && (isrow(x) || isempty(x))
    t = ['''' x ''''];
elseif isstring(x) && isscalar(x)
    t = ['"' char(x) '"'];
elseif isstruct(x)
    t = sprintf('%s struct (%d fields)', sz, numel(fieldnames(x)));
elseif isa(x, 'timeseries')
    t = sprintf('timeseries, %d samples', x.Length);
elseif iscell(x)
    t = sprintf('%s cell', sz);
elseif isa(x, 'function_handle')
    t = func2str(x);
else
    t = sprintf('%s %s', sz, class(x));
end
if strlength(t) > 80
    t = [t(1:77) '...'];
end
end
