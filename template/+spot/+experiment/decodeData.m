function files = decodeData(folder, s, root)
%DECODEDATA Turn the experiment data files (ExpLog*) into MATLAB timeseries.
%   files = spot.experiment.decodeData(folder, s, projectFolder) reads every ExpLog* file in
%   folder/<PLATFORM>/ (as downloaded by the console into "Saved Data"), splits the columns into
%   the signals of the diagram's "Data Logger Subsystem/Data Logging Bus" (column 1 is time), and
%   saves each file as <PLATFORM>_<n>.mat holding dataClass_rt (a struct of timeseries) next to it.
%   The diagram is compiled (model update, not code generation) to learn the signal widths.
model = spot.sim.loadDiagram(s.diagram, root);
feval(model, [], [], [], 'compile');
terminate = onCleanup(@() feval(model, [], [], [], 'term'));
ph = get_param([model '/Data Logger Subsystem/Data Logging Bus'], 'PortHandles');
sh = get_param(ph.Inport, 'SignalHierarchy');
if ~iscell(sh)
    sh = {sh};
end
widths = zeros(1, numel(sh));
names = cell(1, numel(sh));
for k = 1:numel(sh)
    d = get_param(ph.Inport(k), 'CompiledPortDimensions');
    widths(k) = d(2);
    names{k} = sh{k}.SignalName;
    if isempty(names{k})
        names{k} = sprintf('UnnamedSignal_%d', k);
    end
end

files = {};
for p = {'RED', 'BLACK', 'BLUE'}
    logs = dir(fullfile(folder, p{1}, 'ExpLog*'));
    for k = 1:numel(logs)
        data = importdata(fullfile(logs(k).folder, logs(k).name));
        data(isnan(data)) = 0;
        t = data(:, 1);
        dataClass_rt = struct();
        col = 1;
        for j = 1:numel(names)
            ts = timeseries(data(:, col:col + widths(j) - 1), t);
            ts.Name = names{j};
            dataClass_rt.(matlab.lang.makeValidName(names{j})) = ts;
            col = col + widths(j);
        end
        out = fullfile(logs(k).folder, sprintf('%s_%d.mat', p{1}, k));
        save(out, 'dataClass_rt', '-mat');
        files{end + 1} = out; %#ok<AGROW>
    end
end
end
