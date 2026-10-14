function d = errorDetails(err, root)
%ERRORDETAILS An error's root cause, the blocks involved and the full report, as plain text.
%   d = spot.sim.errorDetails(err) takes any MATLAB error (an MException, Simulink's
%   MSLException, a spot.sim.SimulationFailed or an MSLDiagnostic) and returns a struct with
%     summary  one line: the root cause, and the block it is in (if any)
%     chain    the messages from the outermost to the root cause, each with its identifier,
%              depth and the blocks it names (struct array: message, identifier, depth, blocks)
%     blocks   every block named, as full paths (for spot.sim.showBlock)
%     where    'file.m, line N' when the error comes from the project's own code (e.g.
%              SPOT_User_Parameters.m), otherwise ''
%     output   what Simulink printed before the error (spot.sim.SimulationFailed only)
%     report   MATLAB's full report (getReport), without hyperlinks
%   d = spot.sim.errorDetails(err, projectFolder) also looks for 'where' in that folder.
%   Simulink's messages carry hyperlinks; they are removed everywhere.
if nargin < 2
    root = '';
end
output = '';
report = '';
if isa(err, 'spot.sim.SimulationFailed')
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
report = plain(report);   % (Simulink's messages carry their hyperlinks into the report)
if isempty(report)
    report = plain(err.message);
end

chain = struct('message', {}, 'identifier', {}, 'depth', {}, 'blocks', {});
chain = walk(err, 0, chain);
blocks = unique([chain.blocks], 'stable');

% The root cause: the deepest message, with the deepest block named on the way to it
summary = 'Unknown error.';
if ~isempty(chain)
    [~, k] = max([chain.depth]);
    summary = chain(k).message;
    named = find(~cellfun(@isempty, {chain(1:k).blocks}), 1, 'last');
    if ~isempty(named)
        summary = sprintf('%s (in %s)', summary, shortPath(chain(named).blocks{1}));
    end
end

d = struct('summary', summary, 'chain', chain, 'blocks', {blocks}, 'where', where(stack, root), ...
           'output', output, 'report', strtrim(report));
end

function chain = walk(e, depth, chain)
% Depth first through the causes, skipping messages already listed
msg = plain(e.message);
if ~isempty(msg) && ~any(strcmp({chain.message}, msg))
    chain(end + 1) = struct('message', msg, 'identifier', char(e.identifier), 'depth', depth, ...
                            'blocks', {blockPaths(e)});
end
causes = {};
try
    causes = e.cause;
catch
end
if ~iscell(causes)
    causes = num2cell(causes);
end
for k = 1:numel(causes)
    chain = walk(causes{k}, depth + 1, chain);
end
end

function paths = blockPaths(e)
% Blocks named in Simulink's hyperlinks, and the handles/paths the error carries
paths = {};
tok = regexp(e.message, 'open_and_hilite_hyperlink\s*\(\s*''((?:[^'']|'''')*)''', 'tokens');
for k = 1:numel(tok)
    paths{end + 1} = strrep(tok{k}{1}, '''''', ''''); %#ok<AGROW>
end
for name = {'handles', 'paths'}
    try
        h = e.(name{1});
    catch
        continue
    end
    if ~iscell(h)
        h = num2cell(h);
    end
    for k = 1:numel(h)
        try
            if ischar(h{k}) || isstring(h{k})
                p = cellstr(h{k});
            else
                p = cellstr(getfullname(h{k}));   % (several handles give several names)
            end
            p = p(~cellfun(@isempty, p));
            paths = [paths, reshape(p, 1, [])]; %#ok<AGROW>
        catch
        end
    end
end
% Blocks only (a path inside the model), not the model itself; block names may contain line
% breaks, which are kept as they are
paths = unique(paths(contains(paths, '/')), 'stable');
end

function text = plain(text)
% Without Simulink's hyperlinks
text = char(text);
text = regexprep(text, '<a\s[^>]*>|</a>', '');
text = strtrim(text);
end

function p = shortPath(p)
% A block path without the model's name in front
slash = find(p == '/', 1);
if ~isempty(slash)
    p = p(slash + 1:end);
end
end

function stack = stackOf(e)
stack = struct('file', {}, 'name', {}, 'line', {});
try
    stack = e.stack;
catch
end
end

function w = where(stack, root)
% The first frame in the project's own code (not the console's +spot package)
w = '';
if isempty(root)
    return
end
pkg = [filesep '+spot' filesep];
for k = 1:numel(stack)
    f = stack(k).file;
    if startsWith(f, [root filesep]) && ~contains(f, pkg)
        w = sprintf('%s, line %d', f(numel(root) + 2:end), stack(k).line);
        return
    end
end
end
