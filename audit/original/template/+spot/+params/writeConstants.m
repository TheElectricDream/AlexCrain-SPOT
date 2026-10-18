function changed = writeConstants(values, file)
    %WRITECONSTANTS Put new values into Constants.m, keeping everything else as it is.
    %   changed = spot.params.writeConstants(values) rewrites, in the project's Constants.m, the
    %   line of every constant in values whose value differs from the class's (Constants.all):
    %   the value between "NAME =" and ";" is replaced, the trailing comment and the indentation
    %   stay, a long vector wraps with "..." as the file does. Then the class is cleared, so the
    %   next Constants.NAME reads the file again. This is how the console changes a setting,
    %   the Designer's choices or the lab calibration the project took: Constants.m is the only
    %   place they live.
    %   changed = spot.params.writeConstants(values, file) writes another Constants.m (not
    %   cleared from MATLAB).
    %
    %   Inputs
    %     values   struct, constant name -> new value: a number, a row vector or matrix (NaN and
    %              [] allowed), a logical (scalar or vector) or a char
    %     file     the file to edit (default Constants.file())
    %
    %   Outputs
    %     changed  cell array of the names written ({} if every value was the same)
    %
    %   Example
    %     spot.params.writeConstants(struct('SAMPLE_RATE', 25, 'USE_RED', true));
    %
    %   See also Constants, spot.params.fromConstants, spot.params.toConstants.

    %% Constants
    COMMENT_COLUMN = 57;    % the trailing comments start here (1-based) when the value is short
    MAX_LINE = 100;         % a longer value wraps with ...
    TOLERANCE = 1e-12;      % relative: a number this close is the same value

    %% What differs
    own = nargin < 2 || isempty(file);
    if own
        file = Constants.file();
        current = Constants.all();
    else
        current = struct();
    end
    names = fieldnames(values);
    changed = {};
    for k = 1:numel(names)
        if ~isfield(current, names{k}) ...
                || ~Constants.sameValue(current.(names{k}), values.(names{k}), TOLERANCE)
            changed{end + 1} = names{k}; %#ok<AGROW>
        end
    end
    if isempty(changed)
        return
    end

    %% Edit the lines
    text = fileread(file);
    lineBreak = newline;
    if contains(text, sprintf('\r\n'))
        lineBreak = sprintf('\r\n');
    end
    lines = strsplit(text, lineBreak, 'CollapseDelimiters', false);
    for k = 1:numel(changed)
        name = changed{k};
        first = find(~cellfun(@isempty, regexp(lines, ['^\s*' name '\s*='], 'once')), 1);
        if isempty(first)
            error('spot:params:writeConstants', '%s has no constant %s.', file, name);
        end

        % The statement's last line: the first whose code (not comment) doesn't end in "...".
        last = first;
        while continues(lines{last})
            last = last + 1;
            if last > numel(lines)
                error('spot:params:writeConstants', '%s: %s is unfinished.', file, name);
            end
        end

        % Its indentation and comment (from whichever line has one).
        indent = regexp(lines{first}, '^\s*', 'match', 'once');
        comment = '';
        for j = first:last
            [~, c] = splitComment(lines{j});
            if ~isempty(strtrim(c))
                comment = strtrim(c);
                break
            end
        end

        % The new statement, wrapped if long, the comment on its first line.
        statement = format(indent, name, values.(name), comment, COMMENT_COLUMN, MAX_LINE);
        lines = [lines(1:first - 1), statement, lines(last + 1:end)];
    end

    %% Write, and forget the old class
    fid = fopen(file, 'w');
    if fid < 0
        error('spot:params:writeConstants', 'Cannot write %s.', file);
    end
    fwrite(fid, strjoin(lines, lineBreak), 'char');
    fclose(fid);
    if own
        clear('Constants');
    end
end

function yes = continues(line)
    % Whether a line's code ends with MATLAB's continuation.
    code = splitComment(line);
    yes = endsWith(strtrim(code), '...');
end

function [code, comment] = splitComment(line)
    % A line's code and its comment: the first % outside quotes starts the comment.
    quoted = false;
    for k = 1:numel(line)
        if line(k) == ''''
            quoted = ~quoted;
        elseif line(k) == '%' && ~quoted
            code = line(1:k - 1);
            comment = line(k + 1:end);
            return
        end
    end
    code = line;
    comment = '';
end

function out = format(indent, name, value, comment, commentColumn, maxLine)
    % The lines of "NAME = value;   % comment", the value wrapped when the line would be long.
    head = sprintf('%s%s = ', indent, name);
    parts = pieces(value);
    if isscalar(parts)
        body = parts{1};
    else
        body = ['[' strjoin(parts, ' ') ']'];
    end
    line = [head body ';'];
    if numel(line) <= maxLine || isscalar(parts)
        out = {withComment(line, comment, commentColumn)};
        return
    end

    % Wrap the vector's elements, continuation lines aligned under the first element.
    pad = repmat(' ', 1, numel(head) + 1);
    out = {};
    row = [head '['];
    for k = 1:numel(parts)
        piece = parts{k};
        if k < numel(parts) && numel(row) + 1 + numel(piece) + 4 > maxLine
            out{end + 1} = [row ' ...']; %#ok<AGROW>
            row = [pad piece];
        elseif k == 1
            row = [row piece]; %#ok<AGROW>
        else
            row = [row ' ' piece]; %#ok<AGROW>
        end
    end
    out{end + 1} = [row '];'];
    out{1} = withComment(out{1}, comment, commentColumn);
end

function line = withComment(line, comment, column)
    % The line with its comment at the comment column (or after one space).
    if isempty(comment)
        return
    end
    line = [line repmat(' ', 1, max(1, column - 1 - numel(line))) '% ' comment];
end

function parts = pieces(value)
    % The text of a value: one piece for a scalar, string, matrix or empty; the elements of a
    % row vector as pieces (so the caller can wrap them).
    if ischar(value) || isstring(value)
        parts = {['''' strrep(char(value), '''', '''''') '''']};
    elseif isempty(value)
        parts = {'[]'};
    elseif isscalar(value)
        parts = {scalarText(value)};
    elseif isvector(value)
        parts = arrayfun(@scalarText, reshape(value, 1, []), 'UniformOutput', false);
    else
        rows = cell(1, size(value, 1));
        for r = 1:size(value, 1)
            rows{r} = strjoin(arrayfun(@scalarText, value(r, :), 'UniformOutput', false), ' ');
        end
        parts = {['[' strjoin(rows, '; ') ']']};
    end
end

function s = scalarText(x)
    % The shortest text that reads back as x.
    if islogical(x)
        if x
            s = 'true';
        else
            s = 'false';
        end
        return
    end
    x = double(x);
    if isnan(x)
        s = 'NaN';
        return
    end
    if isinf(x)
        if x > 0
            s = 'Inf';
        else
            s = '-Inf';
        end
        return
    end
    if x == round(x) && abs(x) < 1e15
        s = sprintf('%d', x);
        return
    end
    for digits = 1:17
        s = sprintf('%.*g', digits, x);

        % Positional notation where it reads naturally (10, not 1e+01): more digits give it.
        if contains(s, 'e') && abs(x) >= 1e-4 && abs(x) < 1e15 && digits < 17
            continue
        end
        if str2double(s) == x
            return
        end
    end
end
