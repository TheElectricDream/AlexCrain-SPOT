function debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
    %DEBUGMESSAGES  Each platform's debug message, for the SPOT Console's Messages panel.
    %   spot.hardware.debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP). The
    %   diagram writes a short text to RED_Logger, BLACK_Logger and BLUE_Logger (e.g. "Chaser:
    %   Running Phase 1"); the Debugger subsystem turns them into character codes and calls
    %   this every step, in simulations and experiments alike.
    %     Experiment  the platform's own message goes to the lab console (UDP
    %                 Constants.PORT_DEBUG_RED, _BLACK, _BLUE) as one packet: experiment
    %                 time (double) then the text. Sent when it changes and every
    %                 DEBUG_REPEAT_PERIOD while unchanged, so a lost packet only delays it.
    %     Simulation  every change of each platform's message is recorded with its time
    %                 (spot.sim.debugLog); the console shows them live and with the replay
    %
    %   Inputs
    %     whoAmI            1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
    %     isSim             1 in a simulation, 0 in an experiment
    %     t                 experiment time [s] (Univ_Time)
    %     red, black, blue  messages as uint8 character codes; zeros after the text are ignored
    %     labConsoleIP      the lab console's address, four numbers (block parameter
    %                       labConsoleIP); without it, Constants.IP_LAB_CONSOLE
    %
    %   Example (inside the block)
    %     function sendDebugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
    %     spot.hardware.debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP);
    %     end
    %
    %   See also spot.sim.debugLog, spot.hardware.udpSend.

    %#codegen

    %% Constants
    % The lab console's address; older blocks don't pass it.
    if nargin < 7
        LAB_CONSOLE = Constants.IP_LAB_CONSOLE;
    else
        LAB_CONSOLE = labConsoleIP;
    end
    REPEAT = Constants.DEBUG_REPEAT_PERIOD;    % [s] resend an unchanged message this often
    PORT_RED = Constants.PORT_DEBUG_RED;
    PORT_BLACK = Constants.PORT_DEBUG_BLACK;
    PORT_BLUE = Constants.PORT_DEBUG_BLUE;
    coder.extrinsic('spot.sim.debugLog');   % simulations only (not in the generated code)

    %% Last messages and send time, between steps
    persistent lastRed lastBlack lastBlue lastSent
    if isempty(lastSent)
        lastRed = zeros(size(red), 'uint8');
        lastBlack = zeros(size(black), 'uint8');
        lastBlue = zeros(size(blue), 'uint8');
        lastSent = -inf;
    end

    if coder.target('Rtw')

        %% Experiment: send this platform's message
        if isSim == 0
            switch whoAmI
                case 1
                    [lastRed, lastSent] = sendOwn(LAB_CONSOLE, PORT_RED, t, uint8(red), ...
                        lastRed, lastSent, REPEAT);
                case 2
                    [lastBlack, lastSent] = sendOwn(LAB_CONSOLE, PORT_BLACK, t, uint8(black), ...
                        lastBlack, lastSent, REPEAT);
                case 3
                    [lastBlue, lastSent] = sendOwn(LAB_CONSOLE, PORT_BLUE, t, uint8(blue), ...
                        lastBlue, lastSent, REPEAT);
                otherwise
            end
        end
    else

        %% Simulation: record each change for the console
        if ~isequal(uint8(red), lastRed)
            lastRed = uint8(red);
            spot.sim.debugLog(1, t, lastRed);
        end
        if ~isequal(uint8(black), lastBlack)
            lastBlack = uint8(black);
            spot.sim.debugLog(2, t, lastBlack);
        end
        if ~isequal(uint8(blue), lastBlue)
            lastBlue = uint8(blue);
            spot.sim.debugLog(3, t, lastBlue);
        end
    end
end

function [last, lastSent] = sendOwn(ip, port, t, msg, last, lastSent, repeat)
    % Send [time, text] when the message changed, was last sent repeat s ago, or the time went
    % backwards (e.g. the experiment clock started).
    if ~isequal(msg, last) || t - lastSent >= repeat || t < lastSent
        packet = [typecast(double(t), 'uint8'), reshape(msg, 1, [])];
        spot.hardware.udpSend(ip, port, packet);
        last = msg;
        lastSent = t;
    end
end
