function signals = flatten(x, prefix)
%FLATTEN Logged data as one flat struct of timeseries.
%   signals = spot.data.flatten(x) accepts what the diagrams and the old GUI saved:
%     - a struct of timeseries (dataClass, dataClass_rt)
%     - nested structs of timeseries (the old GUI's merged files: RED.Px_m -> RED_Px_m)
%     - a Simulink.SimulationData.Dataset (logsout), whose elements may hold either
%     - a timeseries
%   and returns a struct whose fields are all timeseries, named parent_child for nested ones.
%   Fields that aren't logged signals are left out.
if nargin < 2
    prefix = '';
end
signals = struct();
if isa(x, 'timeseries')
    signals.(name(prefix, 'signal')) = x;
elseif isa(x, 'Simulink.SimulationData.Dataset')
    for k = 1:x.numElements
        e = x.getElement(k);
        n = e.Name;
        if isempty(n)
            n = sprintf('signal%d', k);
        end
        signals = merge(signals, spot.data.flatten(e.Values, name(prefix, n)));
    end
elseif isstruct(x) && isscalar(x)
    for f = fieldnames(x).'
        v = x.(f{1});
        if isa(v, 'timeseries')
            signals.(name(prefix, f{1})) = v;
        elseif (isstruct(v) && isscalar(v)) || isa(v, 'Simulink.SimulationData.Dataset')
            signals = merge(signals, spot.data.flatten(v, name(prefix, f{1})));
        end
    end
end
end

function n = name(prefix, field)
if isempty(prefix)
    n = matlab.lang.makeValidName(field);
else
    n = matlab.lang.makeValidName([prefix '_' field]);
end
n = n(1:min(end, namelengthmax));
end

function a = merge(a, b)
for f = fieldnames(b).'
    a.(f{1}) = b.(f{1});
end
end
