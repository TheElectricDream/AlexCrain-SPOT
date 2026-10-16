function label = streamName(block, name)
    %STREAMNAME The name a Live Stream block shows in the SPOT Console's Live Plot.
    %   label = spot.hardware.streamName(block, name) is called by the Live Stream block's mask
    %   (in MATLAB, when the diagram is updated or built).
    %
    %   Inputs
    %     block  the Live Stream block
    %     name   the name typed into the block, or ''
    %
    %   Outputs
    %     label  the typed name; otherwise what is connected to the input: a Data Store Read's
    %            data store (e.g. RED_Measured_States), else the signal's name, else the source
    %            block's name. At most 40 printable characters.
    %
    %   See also spot.hardware.liveStream.

    %% Constants
    MAX_LENGTH = 40;

    %% Name
    label = strtrim(char(name));
    if isempty(label)
        label = 'stream';
        try
            % The line into the block's input, and its source block.
            portHandles = get_param(block, 'PortHandles');
            lineHandle = get_param(portHandles.Inport(1), 'Line');
            src = get_param(lineHandle, 'SrcBlockHandle');

            % Data store name, signal name, or block name.
            if strcmp(get_param(src, 'BlockType'), 'DataStoreRead')
                label = get_param(src, 'DataStoreName');
            elseif ~isempty(strtrim(get_param(lineHandle, 'Name')))
                label = get_param(lineHandle, 'Name');
            else
                label = get_param(src, 'Name');
            end
        catch
            % nothing connected yet
        end
    end

    %% Printable, single-spaced, limited length
    label = regexprep(label, '\s+', ' ');
    label = label(label >= 32 & label < 127);
    label = label(1:min(end, MAX_LENGTH));
end
