function s = summary(ds, id)
%SUMMARY What the console shows about a loaded dataset (no data).
%   s = spot.data.summary(dataset, id) returns id, name, kind, source, t0 and t1 [s], samples
%   (of the longest signal), signals (name, columns, units, zero = all zero, and unit, quantity,
%   platform and label from spot.plotting.signalLabel), crafts and arm
%   (see spot.data.bodies) and phases (whether its settings give the phase times).
f = fieldnames(ds.signals);
sig = struct('name', f, 'cols', 1, 'units', '', 'zero', false, 'unit', '', 'quantity', '', 'platform', '', 'label', '');
t0 = inf; t1 = -inf; n = 0;
for k = 1:numel(f)
    ts = ds.signals.(f{k});
    [t, y] = spot.data.samples(ts);
    sig(k).cols = size(y, 2);
    sig(k).units = char(ts.DataInfo.Units);
    sig(k).zero = ~any(y(:));
    L = spot.plotting.signalLabel(f{k});
    if ~isempty(sig(k).units)   % units logged with the signal win over the name's suffix
        L.unit = sig(k).units;
        L.label = sprintf('%s [%s]', L.quantity, L.unit);
    end
    sig(k).unit = L.unit; sig(k).quantity = L.quantity; sig(k).platform = L.platform; sig(k).label = L.label;
    if ~isempty(t)
        t0 = min(t0, t(1));
        t1 = max(t1, t(end));
        n = max(n, numel(t));
    end
end
b = spot.data.bodies(ds);
s = struct('id', id, 'name', ds.name, 'kind', ds.kind, 'source', ds.source, ...
           't0', t0, 't1', t1, 'samples', n, 'signals', sig, ...
           'crafts', {b.crafts}, 'arm', b.arm, 'phases', isstruct(ds.settings));
end
