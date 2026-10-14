function vars = list()
%LIST The base workspace's variables, as MATLAB's Workspace browser shows them.
%   vars = spot.workspace.list() returns a struct array with name, size (e.g. '3x1'), class,
%   bytes and value (a short preview, or '' when too large to show).
w = evalin('base', 'whos');
vars = struct('name', {}, 'size', {}, 'class', {}, 'bytes', {}, 'value', {});
for k = 1:numel(w)
    preview = '';
    try
        preview = spot.workspace.preview(evalin('base', w(k).name));
    catch
    end
    vars(end + 1) = struct('name', w(k).name, 'size', sizeText(w(k).size), ...
        'class', w(k).class, 'bytes', w(k).bytes, 'value', preview); %#ok<AGROW>
end
end

function t = sizeText(sz)
t = strjoin(arrayfun(@num2str, sz, 'UniformOutput', false), 'x');
end
