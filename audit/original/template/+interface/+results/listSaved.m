function runs = listSaved(root)
    %LISTSAVED What is in the project's "Saved Data" folder, for the console's Results page.
    %   runs = interface.results.listSaved(root) returns one entry per run, newest first.
    %
    %   Inputs
    %     root  project folder
    %
    %   Outputs
    %     runs  struct array:
    %             name      folder name
    %             kind      'experiment'  <run>/<PLATFORM>/<PLATFORM>_<n>.mat (from the console)
    %                       'legacy'      ExperimentData_<PLATFORM>_<stamp>/..._<n>.mat (old
    %                                     GUI), grouped by stamp across platforms
    %                       'simulation'  <run>/simulation.mat (saved from the console)
    %                       'files'       any other .mat files
    %             modified  POSIX time of the newest file
    %             items     what can be loaded: label, name (for the loaded dataset; '' = the
    %                       file's own), and files (PLATFORM -> path) or file (path), relative
    %                       to the project folder. An experiment whose ExpLog files weren't
    %                       converted yet has an item with convert = true and no files.
    %             label     short name from its time stamp ("10-07 15:02")
    %             settings  path of the settings saved with the run, or ''
    %             folder    the run's full path ('' for the old GUI's runs)
    %
    %   See also spot.data.load, spot.data.saveSimulation.

    folder = fullfile(root, 'Saved Data');
    runs = struct('name', {}, 'kind', {}, 'modified', {}, 'items', {}, 'settings', {}, ...
                  'folder', {});
    if ~isfolder(folder)
        return
    end

    %% Run folders
    % The old GUI's folders, grouped by stamp: stamp -> struct(PLATFORM -> folder name).
    legacy = containers.Map();
    entries = dir(folder);
    for entry = entries(:).'
        if entry.name(1) == '.'
            continue
        end
        runFolder = fullfile(folder, entry.name);
        if ~entry.isdir
            continue
        end

        % The old GUI: one folder per platform; gather the platforms of one run (same stamp).
        tok = regexp(entry.name, '^ExperimentData_(RED|BLACK|BLUE)_(.+)$', 'tokens', 'once');
        if ~isempty(tok)
            if ~isKey(legacy, tok{2})
                legacy(tok{2}) = struct();
            end
            group = legacy(tok{2});
            group.(tok{1}) = entry.name;
            legacy(tok{2}) = group;
            continue
        end

        run = struct('name', entry.name, 'kind', 'files', 'modified', posix(entry.datenum), ...
                     'items', [], 'settings', '', 'folder', runFolder);
        items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
        platforms = {'RED', 'BLACK', 'BLUE'};
        hasPlatform = cellfun(@(p) isfolder(fullfile(runFolder, p)), platforms);

        if any(hasPlatform)

            % An experiment: <PLATFORM>_<n>.mat per platform, grouped by run number n.
            run.kind = 'experiment';
            found = struct();
            undecoded = false;
            for p = platforms(hasPlatform)
                mats = dir(fullfile(runFolder, p{1}, [p{1} '_*.mat']));
                for mat = mats(:).'
                    runNumber = sscanf(mat.name, [p{1} '_%d.mat']);
                    if isempty(runNumber)
                        continue
                    end
                    key = sprintf('n%d', runNumber);
                    if ~isfield(found, key)
                        found.(key) = struct();
                    end
                    found.(key).(p{1}) = rel(root, fullfile(mat.folder, mat.name));
                    run.modified = max(run.modified, posix(mat.datenum));
                end

                % ExpLog files without .mat files: not converted yet.
                undecoded = undecoded || (isempty(mats) ...
                                          && ~isempty(dir(fullfile(runFolder, p{1}, 'ExpLog*'))));
            end

            % One item per run number, in order.
            runKeys = fieldnames(found);
            [~, order] = sort(cellfun(@(k) sscanf(k, 'n%d'), runKeys));
            for k = runKeys(order).'
                items(end + 1) = struct('label', sprintf('Run %d', sscanf(k{1}, 'n%d')), ...
                    'files', found.(k{1}), 'file', '', 'convert', false); %#ok<AGROW>
            end
            if undecoded
                items(end + 1) = struct('label', 'ExpLog Files Not Converted Yet', ...
                    'files', struct(), 'file', '', 'convert', true); %#ok<AGROW>
            end
        else

            % A simulation, or loose .mat files: one item per file.
            mats = dir(fullfile(runFolder, '*.mat'));
            if isfile(fullfile(runFolder, 'simulation.mat'))
                run.kind = 'simulation';
            end
            for mat = mats(:).'
                items(end + 1) = struct('label', mat.name, 'files', struct(), ...
                    'file', rel(root, fullfile(mat.folder, mat.name)), ...
                    'convert', false); %#ok<AGROW>
                run.modified = max(run.modified, posix(mat.datenum));
            end
        end

        % Settings saved with the run.
        if isfile(fullfile(runFolder, 'spot_settings.json'))
            run.settings = rel(root, fullfile(runFolder, 'spot_settings.json'));
        end
        if ~isempty(items)
            run.items = items;
            runs(end + 1) = run; %#ok<AGROW>
        end
    end

    %% The old GUI's runs
    for stamp = legacy.keys()
        group = legacy(stamp{1});
        run = struct('name', ['ExperimentData_' stamp{1}], 'kind', 'legacy', 'modified', 0, ...
                     'items', [], 'settings', '', 'folder', '');

        % Files ..._<n>.mat per platform, grouped by run number n.
        found = struct();
        for p = fieldnames(group).'
            mats = dir(fullfile(folder, group.(p{1}), '*.mat'));
            for mat = mats(:).'
                runNumber = regexp(mat.name, '_(\d+)\.mat$', 'tokens', 'once');
                if isempty(runNumber)
                    continue
                end
                key = ['n' runNumber{1}];
                if ~isfield(found, key)
                    found.(key) = struct();
                end
                found.(key).(p{1}) = rel(root, fullfile(mat.folder, mat.name));
                run.modified = max(run.modified, posix(mat.datenum));
            end
        end

        % One item per run number.
        items = struct('label', {}, 'files', {}, 'file', {}, 'convert', {});
        for k = sort(fieldnames(found)).'
            items(end + 1) = struct('label', sprintf('Run %s', k{1}(2:end)), ...
                'files', found.(k{1}), 'file', '', 'convert', false); %#ok<AGROW>
        end
        if ~isempty(items)
            run.items = items;
            runs(end + 1) = run; %#ok<AGROW>
        end
    end

    %% Loose .mat files directly in Saved Data
    mats = dir(fullfile(folder, '*.mat'));
    if ~isempty(mats)
        items = struct('label', {mats.name}, 'files', struct(), 'file', '', 'convert', false);
        for k = 1:numel(mats)
            items(k).file = rel(root, fullfile(mats(k).folder, mats(k).name));
        end
        runs(end + 1) = struct('name', 'Saved Data', 'kind', 'files', ...
            'modified', posix(max([mats.datenum])), ...
            'items', items, 'settings', '', 'folder', folder);
    end

    %% Newest first, with short names
    [~, order] = sort([runs.modified], 'descend');
    runs = runs(order);

    % "10-07 15:02" for a run, "10-07 15:02 · Run 2" for its datasets.
    for r = 1:numel(runs)
        runs(r).label = runLabel(runs(r).name);
        for k = 1:numel(runs(r).items)
            item = runs(r).items(k);
            if strcmp(runs(r).kind, 'simulation')
                runs(r).items(k).name = runs(r).label;
            elseif ~isempty(item.file)
                runs(r).items(k).name = '';   % a file keeps its own name (spot.data.load)
            else
                runs(r).items(k).name = sprintf('%s · %s', runs(r).label, item.label);
            end
        end
    end
end

function label = runLabel(name)
    % Short label from a run's time stamp:
    %   Template_..._20261007_150241            -> "10-07 15:02"
    %   ExperimentData_2026_10_7_11_29_45_3901  -> "10-07 11:29"
    label = name;
    t = regexp(name, '(\d{4})(\d{2})(\d{2})_(\d{2})(\d{2})\d{2}$', 'tokens', 'once');
    if isempty(t)
        t = regexp(name, '(\d{4})_(\d{1,2})_(\d{1,2})_(\d{1,2})_(\d{1,2})_\d{1,2}(_\d+)?$', ...
            'tokens', 'once');
    end
    if ~isempty(t)
        n = str2double(t(1:5));
        label = sprintf('%02d-%02d %02d:%02d', n(2), n(3), n(4), n(5));
    end
end

function p = rel(root, file)
    % A path relative to the project folder, with forward slashes.
    p = strrep(file(numel(root) + 2:end), '\', '/');
end

function t = posix(serialDate)
    % POSIX time of a datenum in local time.
    t = posixtime(datetime(serialDate, 'ConvertFrom', 'datenum', 'TimeZone', 'local'));
end
