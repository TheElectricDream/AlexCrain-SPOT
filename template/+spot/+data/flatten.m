function signals = flatten(x, prefix)
    %FLATTEN Logged data as one flat struct of timeseries.
    %   signals = spot.data.flatten(x) flattens what the diagrams and the old GUI saved.
    %
    %   Inputs
    %     x        a struct of timeseries (dataClass, dataClass_rt); nested structs of
    %              timeseries (the old GUI's merged files: RED.Px_m); a
    %              Simulink.SimulationData.Dataset (logsout), whose elements may hold either;
    %              or a timeseries
    %     prefix   name prefix, used when recursing (default '')
    %
    %   Outputs
    %     signals  struct of timeseries, nested ones named parent_child (RED_Px_m). Fields that
    %              aren't logged signals are left out.
    %
    %   See also spot.data.load, spot.data.samples.

    %% Defaults
    if nargin < 2
        prefix = '';
    end

    signals = struct();
    if isa(x, 'timeseries')

        % A single timeseries.
        signals.(name(prefix, 'signal')) = x;

    elseif isa(x, 'Simulink.SimulationData.Dataset')

        % Each element, named after the element (or its index).
        for k = 1:x.numElements
            element = x.getElement(k);
            elemName = element.Name;
            if isempty(elemName)
                elemName = sprintf('signal%d', k);
            end
            signals = merge(signals, spot.data.flatten(element.Values, name(prefix, elemName)));
        end

    elseif isstruct(x) && isscalar(x)

        % Each field: timeseries kept, structs and datasets flattened.
        for f = fieldnames(x).'
            value = x.(f{1});
            if isa(value, 'timeseries')
                signals.(name(prefix, f{1})) = value;
            elseif (isstruct(value) && isscalar(value)) ...
                    || isa(value, 'Simulink.SimulationData.Dataset')
                signals = merge(signals, spot.data.flatten(value, name(prefix, f{1})));
            end
        end
    end
end

function n = name(prefix, field)
    % prefix_field as a valid field name, within MATLAB's name length.
    if isempty(prefix)
        n = matlab.lang.makeValidName(field);
    else
        n = matlab.lang.makeValidName([prefix '_' field]);
    end
    n = n(1:min(end, namelengthmax));
end

function a = merge(a, b)
    % b's fields copied into a.
    for f = fieldnames(b).'
        a.(f{1}) = b.(f{1});
    end
end
