function [values, count] = logVector(packet)
    %LOGVECTOR The logged signals as one row of numbers, for the platform's data logger.
    %   [values, count] = diagram.hardware.logVector(packet)
    %
    %   On a platform, spot.hardware.CustomDataLogger writes one line of numbers per step
    %   to the ExpLog file. This lays the LogPacket's fields end to end in the order
    %   diagram.buses.define lists them (Time_s first, which the files need), so that
    %   spot.experiment.decodeData can split the columns back into the same signals,
    %   with the same names, as a simulation logs.
    %
    %   Inputs
    %     packet  The logged signals (Bus: LogPacket, diagram.buses.logPacket)
    %
    %   Outputs
    %     values  Every field's values, in order, as one column
    %     count   How many values that is (the logger's second input)
    %
    %   See also diagram.buses.logPacket, spot.hardware.CustomDataLogger,
    %   spot.experiment.decodeData.

    %#codegen

    arguments (Input)
        packet  (1,1) struct
    end

    names = fieldnames(packet);

    % Total length. Every field has a fixed size, so this is known when the code is
    % generated and values has a fixed size
    count = 0;
    for k = coder.unroll(1:numel(names))
        count = count + numel(packet.(names{k}));
    end

    % Each field's values, one after the other
    values = zeros(count, 1);
    first  = 1;
    for k = coder.unroll(1:numel(names))
        field = double(packet.(names{k}));
        values(first:first + numel(field) - 1) = field(:);
        first = first + numel(field);
    end
end
