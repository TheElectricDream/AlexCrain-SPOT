function issues = checkCode(fileName, codeText)
    %CHECKCODE The Code Analyzer's messages for code being edited in the console.
    %   issues = interface.worker.checkCode(fileName, codeText) runs MATLAB's Code Analyzer on code
    %   that hasn't been saved yet: the Documentation tab's editor sends it as you type (the
    %   worker's code.lint command).
    %
    %   Inputs
    %     fileName   the file's name, e.g. 'run.m' (the Analyzer compares a function's name
    %                with its file's)
    %     codeText   the code (char)
    %
    %   Outputs
    %     issues     1xN cell array of structs, one per message, with
    %                  line, col        where it starts (from 1)
    %                  endLine, endCol  where it ends
    %                  message          the Analyzer's text
    %                  id               its check, e.g. 'NOPRT'
    %                  severity         'error', 'warning' or 'info'
    %                {} if there are none. A cell array, so it is a JSON list even with one.
    %
    %   Behavior
    %     1. Write the code, as UTF-8, to a new temporary folder under its own file name.
    %     2. Run codeIssues (R2022b and newer); if that fails, checkcode.
    %     3. Delete the folder.
    %
    %   See also codeIssues, checkcode, interface.worker.Worker.

    %% Temporary file
    % In a folder of its own, deleted however this function ends.
    folder = tempname;
    mkdir(folder);
    removeFolder = onCleanup(@() rmdir(folder, 's'));
    [~, base, ext] = fileparts(fileName);
    if isempty(ext)
        ext = '.m';
    end
    file = fullfile(folder, [base ext]);
    fid = fopen(file, 'w', 'n', 'UTF-8');
    fprintf(fid, '%s', codeText);
    fclose(fid);

    %% Code Analyzer
    % codeIssues gives each message's extent and severity; checkcode is the older interface.
    try
        issues = fromCodeIssues(codeIssues(file));
    catch
        issues = fromCheckcode(checkcode(file, '-struct', '-id'));
    end
    clear removeFolder
end

function issues = fromCodeIssues(result)
    %FROMCODEISSUES The messages of codeIssues' table, as structs.
    found = result.Issues;
    issues = cell(1, height(found));
    for k = 1:height(found)
        issues{k} = struct( ...
            'line', double(found.LineStart(k)), ...
            'col', double(found.ColumnStart(k)), ...
            'endLine', double(found.LineEnd(k)), ...
            'endCol', double(found.ColumnEnd(k)), ...
            'message', char(found.Description(k)), ...
            'id', char(found.CheckID(k)), ...
            'severity', lower(char(found.Severity(k))));
    end
end

function issues = fromCheckcode(messages)
    %FROMCHECKCODE The messages of checkcode's struct array, as structs.
    issues = cell(1, numel(messages));
    for k = 1:numel(messages)
        msg = messages(k);

        % Parse errors stop the code from running; the rest are warnings.
        severity = 'warning';
        if contains(msg.message, 'Parse error') || contains(msg.message, 'Invalid')
            severity = 'error';
        end
        issues{k} = struct( ...
            'line', msg.line, ...
            'col', msg.column(1), ...
            'endLine', msg.line, ...
            'endCol', msg.column(end), ...
            'message', msg.message, ...
            'id', msg.id, ...
            'severity', severity);
    end
end
