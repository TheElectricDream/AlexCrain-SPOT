function files = decodeData(folder, s, root)
    %DECODEDATA Turn the experiment data files (ExpLog*) into MATLAB timeseries.
    %   files = spot.experiment.decodeData(folder, s, root) reads every ExpLog* file in
    %   folder/<PLATFORM>/ (as the console downloads them into "Saved Data"), splits the columns
    %   into the signals of the diagram's "Data Logger Subsystem/Data Logging Bus" (column 1 is
    %   time), and saves each as <PLATFORM>_<n>.mat holding dataClass_rt (a struct of
    %   timeseries) next to it. The diagram is compiled (model update, no code generation) to
    %   learn the signal widths.
    %
    %   Inputs
    %     folder  the run's folder
    %     s       settings (for the diagram)
    %     root    project folder
    %
    %   Outputs
    %     files   cell array of the .mat files written
    %
    %   See also spot.data.listSaved, spot.data.load.

    %% Signal names and widths
    % Compile the diagram (terminated on return).
    model = spot.sim.loadDiagram(s.diagram, root);
    feval(model, [], [], [], 'compile');
    terminate = onCleanup(@() feval(model, [], [], [], 'term'));

    % The logging bus's inputs.
    portHandles = get_param([model '/Data Logger Subsystem/Data Logging Bus'], 'PortHandles');
    hierarchy = get_param(portHandles.Inport, 'SignalHierarchy');
    if ~iscell(hierarchy)
        hierarchy = {hierarchy};
    end

    % Each input's width and name (unnamed ones numbered).
    widths = zeros(1, numel(hierarchy));
    names = cell(1, numel(hierarchy));
    for k = 1:numel(hierarchy)
        dims = get_param(portHandles.Inport(k), 'CompiledPortDimensions');
        widths(k) = dims(2);
        names{k} = hierarchy{k}.SignalName;
        if isempty(names{k})
            names{k} = sprintf('UnnamedSignal_%d', k);
        end
    end

    %% Each platform's logs
    files = {};
    for p = {'RED', 'BLACK', 'BLUE'}
        logs = dir(fullfile(folder, p{1}, 'ExpLog*'));
        for k = 1:numel(logs)

            % Columns: time, then each signal's width; NaN as 0.
            data = importdata(fullfile(logs(k).folder, logs(k).name));
            data(isnan(data)) = 0;
            t = data(:, 1);

            % One timeseries per signal.
            dataClass_rt = struct();
            col = 1;
            for j = 1:numel(names)
                ts = timeseries(data(:, col:col + widths(j) - 1), t);
                ts.Name = names{j};
                dataClass_rt.(matlab.lang.makeValidName(names{j})) = ts;
                col = col + widths(j);
            end

            % Saved next to the log.
            matFile = fullfile(logs(k).folder, sprintf('%s_%d.mat', p{1}, k));
            save(matFile, 'dataClass_rt', '-mat');
            files{end + 1} = matFile; %#ok<AGROW>
        end
    end
end
