function datasets = load(spec, root, settings)
%LOAD Read logged data for the console's Results page and 3D playback.
%   datasets = spot.data.load(spec, projectFolder, settings) returns a cell array of datasets,
%   each a struct with name, kind ('simulation', 'experiment' or 'file'), source, signals (a flat
%   struct of timeseries, see spot.data.flatten) and settings (the SPOT settings that go with it,
%   or [] when unknown). spec says what to read:
%     source = 'simulation'   the last simulation: dataClass in the base workspace, with the
%                             current settings (a copy: later simulations don't change it)
%     files.<PLATFORM>        one experiment run, a .mat file per platform (dataClass_rt); the
%                             signals are merged as described below
%     file                    any .mat file; each variable holding logged data is a dataset (and
%                             each run of the old GUI's merged files)
%     settings                (optional) settings file saved with the run (without it, a file or
%                             an experiment has no settings: no phase times, default arm geometry)
%     name                    (optional) name to give the dataset
%   Paths may be relative to the project folder.
%
%   Merging an experiment: every platform logs every signal, but only its own are measured, so
%   <PLATFORM>_* signals come from that platform's file and ARM_* from RED's. Other signals
%   (Time_s, Float_State, ...) are kept from each file as <PLATFORM>_<signal>.
%   Experiment logs start with a few samples at time 0 before the clock runs; samples whose time
%   doesn't move forward are dropped.
if nargin < 3
    settings = [];
end
spec = defaults(spec);
runSettings = [];   % for a file or an experiment: only the settings saved with it
if ~isempty(spec.settings)
    runSettings = spot.params.load(resolve(spec.settings, root));
end

if strcmp(spec.source, 'simulation')
    dataClass = [];
    try
        dataClass = evalin('base', 'dataClass');
    catch
    end
    signals = spot.data.flatten(dataClass);
    if isempty(fieldnames(signals))
        error('spot:data:load', 'There is no simulation to load: run one first (dataClass in the workspace).');
    end
    name = spec.name;
    if isempty(name)
        name = sprintf('Simulation %s', char(datetime('now', 'Format', 'HH:mm:ss')));
    end
    datasets = {make(name, 'simulation', 'MATLAB workspace (dataClass)', signals, settings)};

elseif ~isempty(fieldnames(spec.files))
    platforms = fieldnames(spec.files);
    parts = struct();
    for p = platforms.'
        f = resolve(spec.files.(p{1}), root);
        parts.(p{1}) = experimentFile(f);
    end
    signals = mergeExperiment(parts);
    name = spec.name;
    if isempty(name)
        name = strjoin(cellfun(@spot.plotting.platformStyle, platforms, 'UniformOutput', false), '+');
    end
    datasets = {make(name, 'experiment', strjoin(struct2cell(spec.files).', ', '), signals, runSettings)};

elseif ~isempty(spec.file)
    f = resolve(spec.file, root);
    contents = builtin('load', f);
    [~, base, ext] = fileparts(f);
    vars = fieldnames(contents);
    datasets = {};
    for v = vars.'
        x = contents.(v{1});
        subsets = runsIn(x);
        for k = 1:numel(subsets)
            signals = cleanAll(spot.data.flatten(subsets{k}.value));
            if isempty(fieldnames(signals))
                continue
            end
            label = [base ext];
            if numel(vars) > 1
                label = [label ' · ' v{1}]; %#ok<AGROW>
            end
            if ~isempty(subsets{k}.name)
                label = [label ' · ' subsets{k}.name]; %#ok<AGROW>
            end
            if ~isempty(spec.name)
                label = spec.name;
            end
            kind = 'file';
            if strcmp(v{1}, 'dataClass_rt')
                kind = 'experiment';
            elseif strcmp(v{1}, 'dataClass') && isstruct(runSettings)
                kind = 'simulation';   % saved by spot.data.saveSimulation
            end
            datasets{end + 1} = make(label, kind, f, signals, runSettings); %#ok<AGROW>
        end
    end
    if isempty(datasets)
        error('spot:data:load', '%s holds no logged signals (timeseries).', [base ext]);
    end
else
    error('spot:data:load', 'Say what to load: source, files or file.');
end
end

function spec = defaults(spec)
d = struct('source', '', 'files', struct(), 'file', '', 'settings', '', 'name', '');
for f = fieldnames(d).'
    if ~isfield(spec, f{1}) || isempty(spec.(f{1}))
        spec.(f{1}) = d.(f{1});
    end
end
end

function d = make(name, kind, source, signals, settings)
d = struct('name', name, 'kind', kind, 'source', source, 'signals', signals, 'settings', settings);
end

function f = resolve(path, root)
f = char(path);
if ~isfile(f)
    f = fullfile(root, f);
end
if ~isfile(f)
    error('spot:data:load', 'There is no file %s.', path);
end
end

function signals = experimentFile(f)
contents = builtin('load', f);
if isfield(contents, 'dataClass_rt')
    x = contents.dataClass_rt;
else
    vars = fieldnames(contents);
    x = contents.(vars{1});
end
signals = cleanAll(spot.data.flatten(x));
end

function out = mergeExperiment(parts)
platforms = fieldnames(parts);
out = struct();
owned = @(f) regexp(f, '^(RED|BLACK|BLUE|ARM)_', 'tokens', 'once');
for p = platforms.'
    s = parts.(p{1});
    for f = fieldnames(s).'
        o = owned(f{1});
        if isempty(o)
            if isscalar(platforms)
                out.(f{1}) = s.(f{1});
            else
                out.([p{1} '_' f{1}]) = s.(f{1});
            end
            continue
        end
        owner = o{1};
        if strcmp(owner, 'ARM')
            owner = 'RED';
        end
        % The owner's copy wins; without the owner's file, the first copy with data
        if ~isfield(out, f{1}) || strcmp(owner, p{1}) || ...
                (~isfield(parts, owner) && ~any(out.(f{1}).Data(:)) && any(s.(f{1}).Data(:)))
            out.(f{1}) = s.(f{1});
        end
    end
end
end

function signals = cleanAll(signals)
for f = fieldnames(signals).'
    signals.(f{1}) = clean(signals.(f{1}));
end
end

function ts = clean(ts)
% Drop samples whose time doesn't move forward (the zeros before an experiment's clock starts)
t = ts.Time(:);
if numel(t) < 2 || size(ts.Data, 1) ~= numel(t)
    return
end
keep = [true; t(2:end) > cummax(t(1:end - 1))];
if all(keep)
    return
end
d = ts.Data(keep, :);
name = ts.Name;
units = ts.DataInfo.Units;
ts = timeseries(d, t(keep));
ts.Name = name;
ts.DataInfo.Units = units;
end

function subsets = runsIn(x)
% The old GUI's merged files hold one struct per run (mergedData.<stamp>.<signal>); anything
% else is one dataset
subsets = {struct('name', '', 'value', x)};
if ~isstruct(x) || ~isscalar(x)
    return
end
f = fieldnames(x);
if isempty(f) || any(ismember(f, {'RED', 'BLACK', 'BLUE', 'ARM'}))
    return
end
isRun = cellfun(@(k) isstruct(x.(k)) && isscalar(x.(k)) && ...
    any(structfun(@(v) isa(v, 'timeseries'), x.(k))), f);
if all(isRun)
    subsets = cellfun(@(k) struct('name', k, 'value', x.(k)), f, 'UniformOutput', false).';
end
end
