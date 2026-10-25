function r = evaluate(command)
    %EVALUATE Run a command in the base workspace and capture what it prints.
    %   r = interface.workspace.evaluate('x = magic(3)'), like typing the command in MATLAB's
    %   Command Window. Commands that wait for keyboard input (input, keyboard, pause) would block
    %   MATLAB.
    %
    %   Inputs
    %     command  MATLAB command text
    %
    %   Outputs
    %     r        output (the printed text) and error (the message, '' if none)
    %
    %   See also interface.workspace.list.

    r = struct('output', '', 'error', '');
    try
        % command is used by name inside the evalc string.
        r.output = evalc('evalin(''base'', command);');
    catch err
        % The message, and where it happened unless that was here.
        r.error = err.message;
        if ~isempty(err.stack) && ~strcmp(err.stack(1).name, 'evaluate')
            r.error = sprintf('%s\n(in %s, line %d)', err.message, err.stack(1).name, ...
                err.stack(1).line);
        end
    end
end
