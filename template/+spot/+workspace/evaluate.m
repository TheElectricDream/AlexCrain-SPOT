function r = evaluate(command)
%EVALUATE Run a command in the base workspace and capture what it prints.
%   r = spot.workspace.evaluate('x = magic(3)') returns output (the printed text) and error
%   (the error message, '' if none), like typing the command in MATLAB's Command Window.
%   Commands that wait for keyboard input (input, keyboard, pause) would block MATLAB.
r = struct('output', '', 'error', '');
try
    r.output = evalc('evalin(''base'', command);');
catch err
    r.error = err.message;
    if ~isempty(err.stack) && ~strcmp(err.stack(1).name, 'evaluate')
        r.error = sprintf('%s\n(in %s, line %d)', err.message, err.stack(1).name, err.stack(1).line);
    end
end
end
