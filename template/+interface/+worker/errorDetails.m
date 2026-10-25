function d = errorDetails(err, root)
    %ERRORDETAILS An error's root cause, the blocks involved and the full report, as plain text.
    %   d = interface.worker.errorDetails(err) explains any MATLAB error. Simulink's messages carry
    %   hyperlinks; they are removed everywhere.
    %   d = interface.worker.errorDetails(err, root) also looks for 'where' in the project folder.
    %
    %   Inputs
    %     err   an MException, Simulink's MSLException, a spot.sim.SimulationFailed or an
    %           MSLDiagnostic
    %     root  project folder (optional)
    %
    %   Outputs
    %     d     struct with:
    %             summary  one line: the root cause, and the block it is in (if any)
    %             chain    the messages from the outermost to the root cause (struct array:
    %                      message, identifier, depth, blocks)
    %             blocks   every block named, as full paths (for interface.worker.showBlock)
    %             where    'file.m, line N' when the error comes from the project's own code
    %                      (e.g. SPOT_User_Parameters.m), otherwise ''
    %             output   what Simulink printed before the error (SimulationFailed only)
    %             report   MATLAB's full report (getReport), without hyperlinks
    %
    %   See also spot.sim.SimulationFailed, interface.worker.showBlock.

    %% Defaults
    if nargin < 2
        root = '';
    end

    %% Report, printed output and stack
    output = '';
    report = '';
    if isa(err, 'spot.sim.SimulationFailed')
        % Simulink's own error inside it, and what Simulink printed.
        output = err.Output;
        try
            report = getReport(err.Original, 'extended', 'hyperlinks', 'off');
        catch
        end
        stack = stackOf(err.Original);
        err = err.Original;
    else
        try
            report = getReport(err, 'extended', 'hyperlinks', 'off');
        catch
        end
        stack = stackOf(err);
    end

    % Simulink's messages carry their hyperlinks into the report.
    report = plain(report);
    if isempty(report)
        report = plain(err.message);
    end

    %% Messages and blocks
    chain = struct('message', {}, 'identifier', {}, 'depth', {}, 'blocks', {});
    chain = walk(err, 0, chain);
    blocks = unique([chain.blocks], 'stable');

    %% Summary
    % The root cause: the deepest message, with the deepest block named on the way to it.
    summary = 'Unknown error.';
    if ~isempty(chain)
        [~, k] = max([chain.depth]);
        summary = chain(k).message;
        named = find(~cellfun(@isempty, {chain(1:k).blocks}), 1, 'last');
        if ~isempty(named)
            summary = sprintf('%s (in %s)', summary, shortPath(chain(named).blocks{1}));
        end
    end

    d = struct('summary', summary, 'chain', chain, 'blocks', {blocks}, ...
               'where', where(stack, root), 'output', output, 'report', strtrim(report));
end

function chain = walk(err, depth, chain)
    % Depth first through the causes, skipping messages already listed.
    msg = plain(err.message);
    if ~isempty(msg) && ~any(strcmp({chain.message}, msg))
        chain(end + 1) = struct('message', msg, 'identifier', char(err.identifier), ...
                                'depth', depth, 'blocks', {blockPaths(err)});
    end

    % Its causes (a cell array, or an array of diagnostics).
    causes = {};
    try
        causes = err.cause;
    catch
    end
    if ~iscell(causes)
        causes = num2cell(causes);
    end
    for k = 1:numel(causes)
        chain = walk(causes{k}, depth + 1, chain);
    end
end

function paths = blockPaths(err)
    % Blocks named in Simulink's hyperlinks, and the handles/paths the error carries.
    paths = {};

    % From the hyperlinks: open_and_hilite_hyperlink('<path>', ...), quotes doubled.
    tok = regexp(err.message, 'open_and_hilite_hyperlink\s*\(\s*''((?:[^'']|'''')*)''', ...
        'tokens');
    for k = 1:numel(tok)
        paths{end + 1} = strrep(tok{k}{1}, '''''', ''''); %#ok<AGROW>
    end

    % From the error's handles and paths properties, where it has them.
    for name = {'handles', 'paths'}
        try
            refs = err.(name{1});
        catch
            continue
        end
        if ~iscell(refs)
            refs = num2cell(refs);
        end
        for k = 1:numel(refs)
            try
                if ischar(refs{k}) || isstring(refs{k})
                    names = cellstr(refs{k});
                else
                    names = cellstr(getfullname(refs{k}));   % several handles: several names
                end
                names = names(~cellfun(@isempty, names));
                paths = [paths, reshape(names, 1, [])]; %#ok<AGROW>
            catch
            end
        end
    end

    % Blocks only (a path inside the model), not the model itself; block names may contain
    % line breaks, which are kept as they are.
    paths = unique(paths(contains(paths, '/')), 'stable');
end

function str = plain(str)
    % Without Simulink's hyperlinks.
    str = char(str);
    str = regexprep(str, '<a\s[^>]*>|</a>', '');
    str = strtrim(str);
end

function p = shortPath(p)
    % A block path without the model's name in front.
    slash = find(p == '/', 1);
    if ~isempty(slash)
        p = p(slash + 1:end);
    end
end

function stack = stackOf(err)
    % The error's stack, or an empty one (diagnostics have none).
    stack = struct('file', {}, 'name', {}, 'line', {});
    try
        stack = err.stack;
    catch
    end
end

function location = where(stack, root)
    % The first frame in the project's own code (not SPOT's +spot and +interface packages).
    location = '';
    if isempty(root)
        return
    end
    packages = {[filesep '+spot' filesep], [filesep '+interface' filesep]};
    for k = 1:numel(stack)
        file = stack(k).file;
        if startsWith(file, [root filesep]) && ~contains(file, packages)
            location = sprintf('%s, line %d', file(numel(root) + 2:end), stack(k).line);
            return
        end
    end
end
