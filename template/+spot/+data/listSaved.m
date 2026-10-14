function runs = listSaved(root)
%LISTSAVED What is in the project's "Saved Data" folder, for the console's Results page.
%   runs = spot.data.listSaved(projectFolder) returns one entry per run, newest first, with
%     name      folder name
%     kind      'experiment'  <run>/<PLATFORM>/<PLATFORM>_<n>.mat (collected by the console)
%               'legacy'      ExperimentData_<PLATFORM>_<stamp>/..._<n>.mat (the old GUI), grouped
%                             by stamp across platforms
%               'simulation'  <run>/simulation.mat (saved from the console)
%               'files'       any other .mat files
%     modified  POSIX time of the newest file
%     items     what can be loaded: label, name (for the loaded dataset; '' = the file's own),
%               and either files (PLATFORM -> path) or file (path);
%               paths are relative to the project folder. An experiment whose ExpLog files
%               weren't converted yet has an item with convert = true and no files.
%     label     a short name for it, from its time stamp ("10-07 15:02")
%     settings  path of the settings saved with the run, or ''
%     folder    the run's folder (full path; '' for the old GUI's runs)
folder = fullfile(root, 'Saved Data');
runs = struct('name', {}, 'kind', {}, 'modified', {}, 'items', {}, 'settings', {}, 'folder', {});
if ~isfolder(folder)
    return
end
legacy = containers.Map();
entries = dir(folder);
for e = entries(:).'
    if e.name(1) == '.'
        continue
    end
    path = fullfile(folder, e.name);
    if ~e.isdir
        continue
    end
    tok = regexp(e.name, '^ExperimentData_(RED|BLACK|BLUE)_(.+)$', 'tokens', 'once');
    if ~isempty(tok)
        % The old GUI: one folder per platform; gather the platforms of one run (same stamp)
        if ~isKey(legacy, tok{2})
            legacy(tok{2}) = struct();
        end
        g = legacy(tok{2});
        g.(tok{1}) = e.name;
        legacy(tok{2}) = g;
        continue
    end
    run = struct('name', e.name, 'kind', 'files', 'modified', posix(e.datenum), 'items', [], 'settings', '', 'folder', path);
    items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
    platforms = {'RED', 'BLACK', 'BLUE'};
    hasPlatform = cellfun(@(p) isfolder(fullfile(path, p)), platforms);
    if any(hasPlatform)
        run.kind = 'experiment';
        found = struct();
        undecoded = false;
        for p = platforms(hasPlatform)
            mats = dir(fullfile(path, p{1}, [p{1} '_*.mat']));
            for m = mats(:).'
                n = sscanf(m.name, [p{1} '_%d.mat']);
                if isempty(n)
                    continue
                end
                key = sprintf('n%d', n);
                if ~isfield(found, key)
                    found.(key) = struct();
                end
                found.(key).(p{1}) = rel(root, fullfile(m.folder, m.name));
                run.modified = max(run.modified, posix(m.datenum));
            end
            undecoded = undecoded || (isempty(mats) && ~isempty(dir(fullfile(path, p{1}, 'ExpLog*'))));
        end
        keys = fieldnames(found);
        [~, order] = sort(cellfun(@(k) sscanf(k, 'n%d'), keys));
        for k = keys(order).'
            items(end + 1) = struct('label', sprintf('Run %d', sscanf(k{1}, 'n%d')), ...
                'files', found.(k{1}), 'file', '', 'convert', false); %#ok<AGROW>
        end
        if undecoded
            items(end + 1) = struct('label', 'ExpLog Files Not Converted Yet', 'files', struct(), ...
                'file', '', 'convert', true); %#ok<AGROW>
        end
    else
        mats = dir(fullfile(path, '*.mat'));
        if isfile(fullfile(path, 'simulation.mat'))
            run.kind = 'simulation';
        end
        for m = mats(:).'
            items(end + 1) = struct('label', m.name, 'files', struct(), ...
                'file', rel(root, fullfile(m.folder, m.name)), 'convert', false); %#ok<AGROW>
            run.modified = max(run.modified, posix(m.datenum));
        end
    end
    if isfile(fullfile(path, 'spot_settings.json'))
        run.settings = rel(root, fullfile(path, 'spot_settings.json'));
    end
    if ~isempty(items)
        run.items = items;
        runs(end + 1) = run; %#ok<AGROW>
    end
end

for stamp = legacy.keys()
    g = legacy(stamp{1});
    run = struct('name', ['ExperimentData_' stamp{1}], 'kind', 'legacy', 'modified', 0, 'items', [], 'settings', '', 'folder', '');
    found = struct();
    for p = fieldnames(g).'
        mats = dir(fullfile(folder, g.(p{1}), '*.mat'));
        for m = mats(:).'
            n = regexp(m.name, '_(\d+)\.mat$', 'tokens', 'once');
            if isempty(n)
                continue
            end
            key = ['n' n{1}];
            if ~isfield(found, key)
                found.(key) = struct();
            end
            found.(key).(p{1}) = rel(root, fullfile(m.folder, m.name));
            run.modified = max(run.modified, posix(m.datenum));
        end
    end
    items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
    for k = sort(fieldnames(found)).'
        items(end + 1) = struct('label', sprintf('Run %s', k{1}(2:end)), 'files', found.(k{1}), ...
            'file', '', 'convert', false); %#ok<AGROW>
    end
    if ~isempty(items)
        run.items = items;
        runs(end + 1) = run; %#ok<AGROW>
    end
end

% Loose .mat files directly in Saved Data
mats = dir(fullfile(folder, '*.mat'));
if ~isempty(mats)
    items = struct('label', {mats.name}, 'files', struct(), 'file', '', 'convert', false);
    for k = 1:numel(mats)
        items(k).file = rel(root, fullfile(mats(k).folder, mats(k).name));
    end
    runs(end + 1) = struct('name', 'Saved Data', 'kind', 'files', 'modified', posix(max([mats.datenum])), ...
        'items', items, 'settings', '', 'folder', folder);
end

[~, order] = sort([runs.modified], 'descend');
runs = runs(order);
% Short names for the console: "10-07 15:02" for a run, "10-07 15:02 · Run 2" for its datasets
for r = 1:numel(runs)
    runs(r).label = runLabel(runs(r).name);
    for k = 1:numel(runs(r).items)
        it = runs(r).items(k);
        if strcmp(runs(r).kind, 'simulation')
            runs(r).items(k).name = runs(r).label;
        elseif ~isempty(it.file)
            runs(r).items(k).name = '';   % a file keeps its own name (spot.data.load)
        else
            runs(r).items(k).name = sprintf('%s · %s', runs(r).label, it.label);
        end
    end
end
end

function label = runLabel(name)
% Template_..._20261007_150241 -> "10-07 15:02"; ExperimentData_2026_10_7_11_29_45_3901 -> "10-07 11:29"
label = name;
t = regexp(name, '(\d{4})(\d{2})(\d{2})_(\d{2})(\d{2})\d{2}$', 'tokens', 'once');
if isempty(t)
    t = regexp(name, '(\d{4})_(\d{1,2})_(\d{1,2})_(\d{1,2})_(\d{1,2})_\d{1,2}(_\d+)?$', 'tokens', 'once');
end
if ~isempty(t)
    n = str2double(t(1:5));
    label = sprintf('%02d-%02d %02d:%02d', n(2), n(3), n(4), n(5));
end
end

function p = rel(root, path)
p = strrep(path(numel(root) + 2:end), '\', '/');
end

function t = posix(datenum)
t = posixtime(datetime(datenum, 'ConvertFrom', 'datenum', 'TimeZone', 'local'));
end
