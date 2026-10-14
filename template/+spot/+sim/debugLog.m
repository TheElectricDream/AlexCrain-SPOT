function out = debugLog(platform, t, codes)
%DEBUGLOG The platforms' debug messages recorded during a simulation (by spot.hardware.debugMessages).
%   spot.sim.debugLog('reset')          start a new record (spot.sim.run does this)
%   spot.sim.debugLog(k, t, codes)      platform k (1 RED, 2 BLACK, 3 BLUE) changed its message to
%                                       the text in codes (uint8 character codes) at time t
%   log = spot.sim.debugLog('get')      the record: struct with RED, BLACK and BLUE, each with t
%                                       (column, s) and text (cell array), in the order they came
%   new = spot.sim.debugLog('new')      the messages added since the previous 'new' (struct array
%                                       with platform, t and text), for the console while it runs
%   An empty message (all zeros) is recorded as "" only after a non-empty one, i.e. when a
%   platform stops showing a message.
persistent entries delivered
if isempty(entries)
    entries = struct('platform', {}, 't', {}, 'text', {});
    delivered = 0;
end
names = {'RED', 'BLACK', 'BLUE'};

if ischar(platform) || isstring(platform)
    switch char(platform)
        case 'reset'
            entries = struct('platform', {}, 't', {}, 'text', {});
            delivered = 0;
            out = [];
        case 'get'
            out = struct();
            for k = 1:3
                mine = entries(strcmp({entries.platform}, names{k}));
                out.(names{k}) = struct('t', reshape([mine.t], [], 1), 'text', {reshape({mine.text}, [], 1)});
            end
        case 'new'
            out = entries(delivered + 1:end);
            delivered = numel(entries);
        otherwise
            error('spot:sim:debugLog', 'Use ''reset'', ''get'' or ''new''.');
    end
    return
end

codes = double(codes(:)');
text = strtrim(char(codes(codes > 0)));
name = names{platform};
previous = entries(strcmp({entries.platform}, name));
if isempty(text) && (isempty(previous) || isempty(previous(end).text))
    return   % nothing to show yet
end
entries(end + 1) = struct('platform', name, 't', double(t), 'text', text);
end
