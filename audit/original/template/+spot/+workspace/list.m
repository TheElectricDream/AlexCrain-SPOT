function vars = list()
    %LIST The base workspace's variables, as MATLAB's Workspace browser shows them.
    %   vars = spot.workspace.list()
    %
    %   Outputs
    %     vars  struct array with name, size (e.g. '3x1'), class, bytes and value (a short
    %           preview, or '' when too large to show)
    %
    %   See also spot.workspace.preview, spot.workspace.inspect.

    info = evalin('base', 'whos');
    vars = struct('name', {}, 'size', {}, 'class', {}, 'bytes', {}, 'value', {});
    for k = 1:numel(info)

        % A short preview; '' if the value can't be read.
        preview = '';
        try
            preview = spot.workspace.preview(evalin('base', info(k).name));
        catch
        end

        vars(end + 1) = struct('name', info(k).name, 'size', sizeText(info(k).size), ...
            'class', info(k).class, 'bytes', info(k).bytes, 'value', preview); %#ok<AGROW>
    end
end

function t = sizeText(sz)
    % Size as text, e.g. '3x1'.
    t = strjoin(arrayfun(@num2str, sz, 'UniformOutput', false), 'x');
end
