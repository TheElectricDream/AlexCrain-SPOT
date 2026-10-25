function t = preview(x)
    %PREVIEW Short text for a value, like the Value column of MATLAB's Workspace browser.
    %   t = interface.workspace.preview(x)
    %
    %   Inputs
    %     x  any value
    %
    %   Outputs
    %     t  one line of text, e.g. '3', '[1 2 3]', '3x4 double'; '' when too large to show
    %
    %   See also interface.workspace.list.

    %% Constants
    MAX_ELEMENTS = 10;   % matrices up to this size are written out
    MAX_LENGTH = 80;     % characters

    t = '';
    sz = strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x');

    %% By kind
    if (isnumeric(x) || islogical(x)) && ismatrix(x)
        if isempty(x)
            t = '[]';
        elseif isscalar(x)
            t = num2str(x, 6);
        elseif numel(x) <= MAX_ELEMENTS
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

    %% One short line
    if strlength(t) > MAX_LENGTH
        t = [t(1:MAX_LENGTH - 3) '...'];
    end
end
