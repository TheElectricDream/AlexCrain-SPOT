function out = debugLog(platform, t, codes)
    %DEBUGLOG The platforms' debug messages recorded during a simulation.
    %   spot.sim.debugLog('reset')       start a new record (spot.sim.run does this)
    %   spot.sim.debugLog(k, t, codes)   platform k (1 RED, 2 BLACK, 3 BLUE) changed its message
    %                                    to the text in codes (uint8 character codes) at time t
    %   log = spot.sim.debugLog('get')   the record: RED, BLACK and BLUE, each with t (column,
    %                                    [s]) and text (cell array), in the order they came
    %   new = spot.sim.debugLog('new')   messages added since the previous 'new' (struct array
    %                                    with platform, t and text), for the console while it
    %                                    runs
    %   An empty message (all zeros) is recorded as "" only after a non-empty one, i.e. when a
    %   platform stops showing a message.
    %
    %   See also spot.hardware.debugMessages, spot.sim.run.

    %% Record, between calls
    persistent entries delivered
    if isempty(entries)
        entries = struct('platform', {}, 't', {}, 'text', {});
        delivered = 0;
    end
    names = {'RED', 'BLACK', 'BLUE'};

    %% Commands
    if ischar(platform) || isstring(platform)
        switch char(platform)
            case 'reset'
                entries = struct('platform', {}, 't', {}, 'text', {});
                delivered = 0;
                out = [];
            case 'get'
                % Each platform's messages, as columns.
                out = struct();
                for k = 1:3
                    mine = entries(strcmp({entries.platform}, names{k}));
                    out.(names{k}) = struct('t', reshape([mine.t], [], 1), ...
                                            'text', {reshape({mine.text}, [], 1)});
                end
            case 'new'
                out = entries(delivered + 1:end);
                delivered = numel(entries);
            otherwise
                error('spot:sim:debugLog', 'Use ''reset'', ''get'' or ''new''.');
        end
        return
    end

    %% A new message
    % The text, without the trailing zeros.
    codes = double(codes(:)');
    message = strtrim(char(codes(codes > 0)));
    name = names{platform};

    % Nothing to show yet: an empty message before any non-empty one.
    previous = entries(strcmp({entries.platform}, name));
    if isempty(message) && (isempty(previous) || isempty(previous(end).text))
        return
    end
    entries(end + 1) = struct('platform', name, 't', double(t), 'text', message);
end
