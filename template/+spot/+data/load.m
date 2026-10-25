function datasets = load(spec, root, settings)
    %LOAD Read logged data for the console's Results page and 3D playback.
    %   datasets = spot.data.load(spec, root, settings)
    %
    %   Inputs
    %     spec      what to read; paths may be relative to the project folder:
    %                 source = 'simulation'  the last simulation (dataClass in the base
    %                                        workspace), with the current settings
    %                 files.<PLATFORM>       one experiment run, a .mat file per platform
    %                                        (dataClass_rt), merged as below
    %                 file                   any .mat file; each variable holding logged data is
    %                                        a dataset (and each run of the old GUI's merged
    %                                        files)
    %                 settings               optional settings file saved with the run (without
    %                                        it a file or experiment has no phase times and the
    %                                        default arm geometry)
    %                 name                   optional dataset name
    %     root      project folder
    %     settings  current settings, for a simulation (default [])
    %
    %   Outputs
    %     datasets  cell array of structs: name, kind ('simulation', 'experiment' or 'file'),
    %               source, signals (flat struct of timeseries, spot.data.flatten) and settings
    %               ([] when unknown)
    %
    %   Merging an experiment: every platform logs every signal, but only its own are measured,
    %   so <PLATFORM>_* come from that platform's file and ARM_* from RED's. Other signals
    %   (Time_s, Float_State, ...) are kept from each file as <PLATFORM>_<signal>. Samples whose
    %   time doesn't move forward (the zeros before an experiment's clock starts) are dropped.
    %
    %   See also interface.results.listSaved, spot.data.flatten, interface.results.summary.

    %% Defaults
    if nargin < 3
        settings = [];
    end
    spec = defaults(spec);

    % For a file or an experiment: only the settings saved with it.
    runSettings = [];
    if ~isempty(spec.settings)
        runSettings = spot.params.load(resolve(spec.settings, root));
    end

    if strcmp(spec.source, 'simulation')

        %% The last simulation
        dataClass = [];
        try
            dataClass = evalin('base', 'dataClass');
        catch
        end
        signals = spot.data.flatten(dataClass);
        if isempty(fieldnames(signals))
            error('spot:data:load', ...
                'There is no simulation to load: run one first (dataClass in the workspace).');
        end
        name = spec.name;
        if isempty(name)
            name = sprintf('Simulation %s', char(datetime('now', 'Format', 'HH:mm:ss')));
        end
        datasets = {make(name, 'simulation', 'MATLAB workspace (dataClass)', signals, settings)};

    elseif ~isempty(fieldnames(spec.files))

        %% One experiment run, a file per platform
        platforms = fieldnames(spec.files);
        parts = struct();
        for p = platforms.'
            file = resolve(spec.files.(p{1}), root);
            parts.(p{1}) = experimentFile(file);
        end
        signals = mergeExperiment(parts);

        % Default name: the platforms, e.g. "Chaser+Target".
        name = spec.name;
        if isempty(name)
            name = strjoin(cellfun(@spot.plotting.platformStyle, platforms, ...
                'UniformOutput', false), '+');
        end
        datasets = {make(name, 'experiment', strjoin(struct2cell(spec.files).', ', '), ...
            signals, runSettings)};

    elseif ~isempty(spec.file)

        %% Any .mat file: each variable (and each old-GUI run in it) is a dataset
        file = resolve(spec.file, root);
        contents = builtin('load', file);
        [~, base, ext] = fileparts(file);
        vars = fieldnames(contents);
        datasets = {};
        for varName = vars.'
            value = contents.(varName{1});
            subsets = runsIn(value);
            for k = 1:numel(subsets)
                signals = cleanAll(spot.data.flatten(subsets{k}.value));
                if isempty(fieldnames(signals))
                    continue
                end

                % Label: file, variable (if several) and run (if any), unless named.
                label = [base ext];
                if numel(vars) > 1
                    label = [label ' · ' varName{1}]; %#ok<AGROW>
                end
                if ~isempty(subsets{k}.name)
                    label = [label ' · ' subsets{k}.name]; %#ok<AGROW>
                end
                if ~isempty(spec.name)
                    label = spec.name;
                end

                % Kind, from the variable's name.
                kind = 'file';
                if strcmp(varName{1}, 'dataClass_rt')
                    kind = 'experiment';
                elseif strcmp(varName{1}, 'dataClass') && isstruct(runSettings)
                    kind = 'simulation';   % saved by spot.data.saveSimulation
                end
                datasets{end + 1} = make(label, kind, file, signals, runSettings); %#ok<AGROW>
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
    % Fill missing or empty fields of the spec.
    d = struct('source', '', 'files', struct(), 'file', '', 'settings', '', 'name', '');
    for f = fieldnames(d).'
        if ~isfield(spec, f{1}) || isempty(spec.(f{1}))
            spec.(f{1}) = d.(f{1});
        end
    end
end

function d = make(name, kind, source, signals, settings)
    % A dataset struct.
    d = struct('name', name, 'kind', kind, 'source', source, 'signals', signals, ...
               'settings', settings);
end

function f = resolve(file, root)
    % A path as given, else relative to the project folder; error if neither exists.
    f = char(file);
    if ~isfile(f)
        f = fullfile(root, f);
    end
    if ~isfile(f)
        error('spot:data:load', 'There is no file %s.', file);
    end
end

function signals = experimentFile(f)
    % An experiment file's signals: dataClass_rt, else its first variable.
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
    % One flat struct from the platforms' files (see the help above).
    platforms = fieldnames(parts);
    out = struct();

    % The platform a signal belongs to, from its prefix.
    owned = @(f) regexp(f, '^(RED|BLACK|BLUE|ARM)_', 'tokens', 'once');

    for p = platforms.'
        partSignals = parts.(p{1});
        for f = fieldnames(partSignals).'
            prefix = owned(f{1});

            % No platform prefix: keep each file's copy, named after its platform.
            if isempty(prefix)
                if isscalar(platforms)
                    out.(f{1}) = partSignals.(f{1});
                else
                    out.([p{1} '_' f{1}]) = partSignals.(f{1});
                end
                continue
            end

            % The arm's signals belong to RED.
            owner = prefix{1};
            if strcmp(owner, 'ARM')
                owner = 'RED';
            end

            % The owner's copy wins; without the owner's file, the first copy with data.
            if ~isfield(out, f{1}) || strcmp(owner, p{1}) || ...
                    (~isfield(parts, owner) && ~any(out.(f{1}).Data(:)) ...
                     && any(partSignals.(f{1}).Data(:)))
                out.(f{1}) = partSignals.(f{1});
            end
        end
    end
end

function signals = cleanAll(signals)
    % clean() on every signal.
    for f = fieldnames(signals).'
        signals.(f{1}) = clean(signals.(f{1}));
    end
end

function ts = clean(ts)
    % Drop samples whose time doesn't move forward (the zeros before an experiment's clock
    % starts).
    t = ts.Time(:);
    if numel(t) < 2 || size(ts.Data, 1) ~= numel(t)
        return
    end
    keep = [true; t(2:end) > cummax(t(1:end - 1))];
    if all(keep)
        return
    end

    % A new timeseries with the kept samples, its name and units.
    data = ts.Data(keep, :);
    name = ts.Name;
    units = ts.DataInfo.Units;
    ts = timeseries(data, t(keep));
    ts.Name = name;
    ts.DataInfo.Units = units;
end

function subsets = runsIn(x)
    % The old GUI's merged files hold one struct per run (mergedData.<stamp>.<signal>);
    % anything else is one dataset.
    subsets = {struct('name', '', 'value', x)};
    if ~isstruct(x) || ~isscalar(x)
        return
    end

    % Platform-level fields: not a merged file.
    f = fieldnames(x);
    if isempty(f) || any(ismember(f, {'RED', 'BLACK', 'BLUE', 'ARM'}))
        return
    end

    % Every field a struct holding timeseries: one dataset per run.
    isRun = cellfun(@(k) isstruct(x.(k)) && isscalar(x.(k)) && ...
        any(structfun(@(v) isa(v, 'timeseries'), x.(k))), f);
    if all(isRun)
        subsets = cellfun(@(k) struct('name', k, 'value', x.(k)), f, 'UniformOutput', false).';
    end
end
