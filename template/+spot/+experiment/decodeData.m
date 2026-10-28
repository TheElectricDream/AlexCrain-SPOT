function files = decodeData(folder, s, root)
    %DECODEDATA Turn the experiment data files (ExpLog*) into MATLAB timeseries.
    %   files = spot.experiment.decodeData(folder, s, root) reads every ExpLog* file in
    %   folder/<PLATFORM>/ (as the console downloads them into "Saved Data"), splits the columns
    %   into the logged signals (column 1 is time), and saves each as <PLATFORM>_<n>.mat holding
    %   dataClass_rt (a struct of timeseries) next to it. An empty file (a start that stopped
    %   before logging anything) is skipped, and n counts the files with data. The diagram is
    %   compiled (model update, no code generation) to learn the signals. They are the inputs
    %   of the diagram's
    %   "Data Logger Subsystem/Data Logging Bus" (the v5 template), or, in a diagram without
    %   that block, the fields of the LogPacket bus type (spot.buses.define), in the order
    %   spot.hardware.logVector writes them.
    %
    %   Inputs
    %     folder  the run's folder
    %     s       settings (for the diagram)
    %     root    project folder
    %
    %   Outputs
    %     files   cell array of the .mat files written
    %
    %   See also interface.results.listSaved, spot.data.load.

    %% Signal names and widths
    % Compile the diagram (terminated on return).
    model = spot.sim.loadDiagram(s.diagram, root);
    feval(model, [], [], [], 'compile');
    terminate = onCleanup(@() feval(model, [], [], [], 'term'));

    % The logging bus's inputs, else the LogPacket bus type's fields.
    loggingBus = [model '/Data Logger Subsystem/Data Logging Bus'];
    if getSimulinkBlockHandle(loggingBus) ~= -1
        [names, widths] = busInputs(loggingBus);
    else
        [names, widths] = logPacketFields(model);
    end

    %% Each platform's logs
    files = {};
    for p = {'RED', 'BLACK', 'BLUE'}
        logs = dir(fullfile(folder, p{1}, 'ExpLog*'));
        n = 0;
        for k = 1:numel(logs)

            % Columns: time, then each signal's width; NaN as 0. An empty file gives no rows.
            data = importdata(fullfile(logs(k).folder, logs(k).name));
            if ~isnumeric(data) || isempty(data)
                continue
            end
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
            n = n + 1;
            matFile = fullfile(logs(k).folder, sprintf('%s_%d.mat', p{1}, n));
            save(matFile, 'dataClass_rt', '-mat');
            files{end + 1} = matFile; %#ok<AGROW>
        end
    end
end

function [names, widths] = busInputs(block)
    % Name and width of each input of a Bus Creator in the compiled diagram (unnamed ones
    % numbered).
    portHandles = get_param(block, 'PortHandles');
    hierarchy = get_param(portHandles.Inport, 'SignalHierarchy');
    if ~iscell(hierarchy)
        hierarchy = {hierarchy};
    end
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
end

function [names, widths] = logPacketFields(model)
    % Name and width of each field of the LogPacket bus type, which compiling the diagram put
    % in the base workspace (its InitFcn runs spot.buses.define).
    if ~evalin('base', 'exist(''LogPacket'', ''var'')')
        error('spot:experiment:decodeData', ...
            ['%s has neither a "Data Logger Subsystem/Data Logging Bus" block nor a LogPacket ' ...
             'bus type, so the columns of its data files are unknown.'], model);
    end
    logPacket = evalin('base', 'LogPacket');
    elements = logPacket.Elements;
    names = {elements.Name};
    widths = arrayfun(@(e) prod(e.Dimensions), elements);
end
