function out = liveData(name, t, v)
%LIVEDATA The Live Stream blocks' signals recorded during a simulation (by spot.hardware.liveStream).
%   spot.sim.liveData('reset')            start a new record (spot.sim.run does this)
%   spot.sim.liveData(codes, t, v)        stream named by codes (uint8 characters) has values v at t
%   all = spot.sim.liveData('get')        every stream: struct array with name, t (column) and v
%                                         (one row per sample)
%   new = spot.sim.liveData('new')        the samples added since the previous 'new', same form,
%                                         for the console's Live Plot while the simulation runs
%   names = spot.sim.liveData('names')    the streams' names
persistent names T V N delivered
if isempty(N)
    [names, T, V, N, delivered] = empty();
end

if ischar(name) || isstring(name)
    switch char(name)
        case 'reset'
            [names, T, V, N, delivered] = empty();
            out = [];
        case 'get'
            out = collect(1:numel(names), ones(1, numel(names)));
        case 'new'
            out = collect(1:numel(names), delivered + 1);
            delivered = N;
            out = out(arrayfun(@(s) ~isempty(s.t), out));
        case 'names'
            out = names;
        otherwise
            error('spot:sim:liveData', 'Use ''reset'', ''get'', ''new'' or ''names''.');
    end
    return
end

label = char(reshape(name, 1, []));
k = find(strcmp(names, label), 1);
v = reshape(double(v), 1, []);
if isempty(k)
    names{end + 1} = label;
    k = numel(names);
    T{k} = zeros(1024, 1);
    V{k} = zeros(1024, numel(v));
    N(k) = 0;
    delivered(k) = 0;
end
if N(k) == numel(T{k})   % grow by doubling
    T{k}(end + 1:2 * end, 1) = 0;
    V{k}(end + 1:2 * end, :) = 0;
end
N(k) = N(k) + 1;
T{k}(N(k)) = double(t);
if size(V{k}, 2) == numel(v)
    V{k}(N(k), :) = v;
end

    function s = collect(ks, from)
        s = struct('name', {}, 't', {}, 'v', {});
        for j = 1:numel(ks)
            i = ks(j);
            s(j).name = names{i};
            s(j).t = T{i}(from(j):N(i));
            s(j).v = V{i}(from(j):N(i), :);
        end
    end
end

function [names, T, V, N, delivered] = empty()
names = {};
T = {};
V = {};
N = zeros(1, 0);
delivered = zeros(1, 0);
end
