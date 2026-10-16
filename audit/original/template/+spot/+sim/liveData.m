function out = liveData(name, t, v)
    %LIVEDATA The Live Stream blocks' signals recorded during a simulation.
    %   spot.sim.liveData('reset')          start a new record (spot.sim.run does this)
    %   spot.sim.liveData(codes, t, v)      stream named by codes (uint8 characters) has values v
    %                                       at time t
    %   all = spot.sim.liveData('get')      every stream: struct array with name, t (column) and
    %                                       v (one row per sample)
    %   new = spot.sim.liveData('new')      samples added since the previous 'new', same form,
    %                                       for the Live Plot while the simulation runs
    %   names = spot.sim.liveData('names')  the streams' names
    %
    %   See also spot.hardware.liveStream, spot.sim.run.

    %% Constants
    INITIAL_CAPACITY = 1024;   % samples per stream; doubled when full

    %% Record, between calls
    % Per stream: name, times, values (one row per sample), sample count, samples delivered.
    persistent names times values counts delivered
    if isempty(counts)
        [names, times, values, counts, delivered] = empty();
    end

    %% Commands
    if ischar(name) || isstring(name)
        switch char(name)
            case 'reset'
                [names, times, values, counts, delivered] = empty();
                out = [];
            case 'get'
                out = collect(1:numel(names), ones(1, numel(names)));
            case 'new'
                % New samples only, and only streams that have some.
                out = collect(1:numel(names), delivered + 1);
                delivered = counts;
                out = out(arrayfun(@(s) ~isempty(s.t), out));
            case 'names'
                out = names;
            otherwise
                error('spot:sim:liveData', 'Use ''reset'', ''get'', ''new'' or ''names''.');
        end
        return
    end

    %% A new sample
    % The stream, added on its first sample.
    label = char(reshape(name, 1, []));
    k = find(strcmp(names, label), 1);
    v = reshape(double(v), 1, []);
    if isempty(k)
        names{end + 1} = label;
        k = numel(names);
        times{k} = zeros(INITIAL_CAPACITY, 1);
        values{k} = zeros(INITIAL_CAPACITY, numel(v));
        counts(k) = 0;
        delivered(k) = 0;
    end

    % Grow by doubling when full.
    if counts(k) == numel(times{k})
        times{k}(end + 1:2 * end, 1) = 0;
        values{k}(end + 1:2 * end, :) = 0;
    end

    % Store it; values of another width are dropped (the time is kept).
    counts(k) = counts(k) + 1;
    times{k}(counts(k)) = double(t);
    if size(values{k}, 2) == numel(v)
        values{k}(counts(k), :) = v;
    end

    function s = collect(ks, from)
        % Streams ks, from sample from(j) of each to the last.
        s = struct('name', {}, 't', {}, 'v', {});
        for j = 1:numel(ks)
            idx = ks(j);
            s(j).name = names{idx};
            s(j).t = times{idx}(from(j):counts(idx));
            s(j).v = values{idx}(from(j):counts(idx), :);
        end
    end
end

function [names, times, values, counts, delivered] = empty()
    % An empty record.
    names = {};
    times = {};
    values = {};
    counts = zeros(1, 0);
    delivered = zeros(1, 0);
end
