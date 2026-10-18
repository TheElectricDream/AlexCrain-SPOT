function catalog = constantsCatalog()
    %CONSTANTSCATALOG Every constant of Constants.m, in its sections, for the console's window.
    %   catalog = spot.params.constantsCatalog() reads the project's Constants.m and returns its
    %   sections in the file's order, each with its title, its description (the comment under
    %   the %% line) and its constants: the name, the text of its value as the file writes it,
    %   the trailing comment (unit and meaning), a note (a comment line just above it), and
    %   whether the value is a formula on other constants (derived). The values themselves come
    %   from the class (Constants.all), so the console shows what MATLAB uses.
    %
    %   Outputs
    %     catalog  struct with:
    %                file      the file read
    %                sections  1xN struct array: title, description, entries (1xM struct
    %                          array: name, expression, comment, note, derived)
    %                values    Constants.all()
    %
    %   See also Constants, spot.params.parseConstant, spot.params.writeConstants.

    %% Read the file
    file = Constants.file();
    lines = strsplit(fileread(file), newline, 'CollapseDelimiters', false);
    lines = regexprep(lines, '\r$', '');

    %% Walk it: %% headers, their description, the properties blocks' lines
    sections = struct('title', {}, 'description', {}, 'entries', {});
    entry = struct('name', {}, 'expression', {}, 'comment', {}, 'note', {}, 'derived', {});
    current = [];
    inProperties = false;
    note = '';
    k = 1;
    while k <= numel(lines)
        line = strtrim(lines{k});
        k = k + 1;
        if startsWith(line, '%%')
            % A new section; the comment lines under it are its description.
            current = numel(sections) + 1;
            sections(current).title = strtrim(line(3:end));
            sections(current).entries = entry;
            description = {};
            while k <= numel(lines) && startsWith(strtrim(lines{k}), '%') ...
                    && ~startsWith(strtrim(lines{k}), '%%')
                comment = strtrim(lines{k});
                description{end + 1} = strtrim(comment(2:end)); %#ok<AGROW>
                k = k + 1;
            end
            sections(current).description = strjoin(description, ' ');
            continue
        end
        if startsWith(line, 'properties')
            inProperties = true;
            note = '';
            continue
        end
        if ~inProperties || isempty(current)
            continue
        end
        if strcmp(line, 'end')
            inProperties = false;
            continue
        end
        if startsWith(line, '%')
            note = strtrim(line(2:end));   % a comment line above the next constant
            continue
        end

        % A constant: NAME = value; % comment, over several lines with ...
        statement = line;
        while endsWith(strtrim(codeOf(statement)), '...') && k <= numel(lines)
            [code, comment] = splitComment(statement);
            statement = [strtrim(code(1:end - 3)) ' ' strtrim(lines{k})];
            if ~isempty(strtrim(comment))
                statement = [statement ' %' comment]; %#ok<AGROW>
            end
            k = k + 1;
        end
        [code, comment] = splitComment(statement);
        tok = regexp(code, '^\s*([A-Z][A-Z0-9_]*)\s*=\s*(.*?);\s*$', 'tokens', 'once');
        if isempty(tok)
            continue
        end
        expression = regexprep(strtrim(tok{2}), '\s+', ' ');
        sections(current).entries(end + 1) = struct('name', tok{1}, ...
            'expression', expression, 'comment', strtrim(comment), 'note', note, ...
            'derived', contains(expression, 'Constants.') || ~isempty(regexp(expression, ...
                '(^|[^A-Za-z_])pi([^A-Za-z_]|$)', 'once')));
        note = '';
    end

    %% Only the sections that hold constants (not the methods' help)
    keep = arrayfun(@(s) ~isempty(s.entries), sections);
    catalog = struct('file', file, 'sections', sections(keep), 'values', Constants.all());
end

function code = codeOf(line)
    % A line's code, without its comment.
    code = splitComment(line);
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
