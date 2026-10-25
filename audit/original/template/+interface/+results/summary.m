function s = summary(ds, id)
    %SUMMARY What the console shows about a loaded dataset (no data).
    %   s = interface.results.summary(dataset, id)
    %
    %   Inputs
    %     ds  dataset (spot.data.load)
    %     id  the console's id for it
    %
    %   Outputs
    %     s   id, name, kind, source; t0 and t1 [s]; samples (of the longest signal); signals
    %         (name, cols, units, zero = all zero, and unit, quantity, platform and label from
    %         spot.plotting.signalLabel); crafts and arm (interface.results.bodies); phases
    %         (whether its settings give the phase times)
    %
    %   See also spot.data.load, interface.results.bodies.

    %% Signals
    f = fieldnames(ds.signals);
    sig = struct('name', f, 'cols', 1, 'units', '', 'zero', false, 'unit', '', 'quantity', '', ...
                 'platform', '', 'label', '');
    t0 = inf;
    t1 = -inf;
    nSamples = 0;
    for k = 1:numel(f)

        % Columns, logged units, and whether it is all zero.
        ts = ds.signals.(f{k});
        [t, y] = spot.data.samples(ts);
        sig(k).cols = size(y, 2);
        sig(k).units = char(ts.DataInfo.Units);
        sig(k).zero = ~any(y(:));

        % Label from the name; units logged with the signal win over the name's suffix.
        info = spot.plotting.signalLabel(f{k});
        if ~isempty(sig(k).units)
            info.unit = sig(k).units;
            info.label = sprintf('%s [%s]', info.quantity, info.unit);
        end
        sig(k).unit = info.unit;
        sig(k).quantity = info.quantity;
        sig(k).platform = info.platform;
        sig(k).label = info.label;

        % Time span and longest signal.
        if ~isempty(t)
            t0 = min(t0, t(1));
            t1 = max(t1, t(end));
            nSamples = max(nSamples, numel(t));
        end
    end

    %% Summary
    found = interface.results.bodies(ds);
    s = struct('id', id, 'name', ds.name, 'kind', ds.kind, 'source', ds.source, ...
               't0', t0, 't1', t1, 'samples', nSamples, 'signals', sig, ...
               'crafts', {found.crafts}, 'arm', found.arm, 'phases', isstruct(ds.settings));
end
