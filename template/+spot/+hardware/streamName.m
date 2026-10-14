function label = streamName(block, name)
%STREAMNAME The name a Live Stream block shows in the SPOT Console's Live Plot.
%   label = spot.hardware.streamName(block, name) is called by the Live Stream block's mask (in
%   MATLAB, when the diagram is updated or built). With a name typed into the block, that name;
%   otherwise the name of what is connected to its input: a Data Store Read's data store (e.g.
%   RED_Measured_States), else the signal's name, else the source block's name.
%   At most 40 printable characters.
label = strtrim(char(name));
if isempty(label)
    label = 'stream';
    try
        ph = get_param(block, 'PortHandles');
        line = get_param(ph.Inport(1), 'Line');
        src = get_param(line, 'SrcBlockHandle');
        if strcmp(get_param(src, 'BlockType'), 'DataStoreRead')
            label = get_param(src, 'DataStoreName');
        elseif ~isempty(strtrim(get_param(line, 'Name')))
            label = get_param(line, 'Name');
        else
            label = get_param(src, 'Name');
        end
    catch
        % nothing connected yet
    end
end
label = regexprep(label, '\s+', ' ');
label = label(label >= 32 & label < 127);
label = label(1:min(end, 40));
end
