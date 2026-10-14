function debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
%DEBUGMESSAGES  Each platform's debug message, for the SPOT Console's Messages panel.
%
%   The diagram writes a short text to RED_Logger, BLACK_Logger and BLUE_Logger (e.g. "Chaser:
%   Running Phase 1"). The Debugger subsystem turns them into character codes (String to ASCII)
%   and calls this every step, in simulations and experiments alike, so what a simulation shows
%   is what the experiment will show:
%
%       function sendDebugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
%       spot.hardware.debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP);
%       end
%
%   Experiment (generated code on a platform): the platform's own message goes to the lab
%   console (labConsoleIP; UDP 55001 RED, 55002 BLACK, 55003 BLUE) as one packet: the
%   experiment time (double, 8 bytes) followed by the text. It is sent when it changes and every
%   0.5 s while it stays the same, so a lost packet only delays it.
%   Simulation: every change of each platform's message is recorded with its time
%   (spot.sim.debugLog); the console shows them while the simulation runs and with its replay.
%
%   whoAmI            1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
%   isSim             1 in a simulation, 0 in an experiment
%   t                 experiment time [s] (Univ_Time)
%   red, black, blue  the messages as uint8 character codes; zeros after the text are ignored
%   labConsoleIP      the lab console's address, four numbers (the block parameter labConsoleIP,
%                     the settings' labConsoleAddress); without it, 192.168.1.116
%#codegen

if nargin < 7
    LAB_CONSOLE = '192.168.1.116';
else
    LAB_CONSOLE = labConsoleIP;
end
REPEAT = 0.5;   % [s] resend an unchanged message this often
coder.extrinsic('spot.sim.debugLog');   % simulations only (not in the generated code)

persistent lastRed lastBlack lastBlue lastSent
if isempty(lastSent)
    lastRed = zeros(size(red), 'uint8');
    lastBlack = zeros(size(black), 'uint8');
    lastBlue = zeros(size(blue), 'uint8');
    lastSent = -inf;
end

if coder.target('Rtw')   % generated code running on a platform
    if isSim == 0
        switch whoAmI
            case 1
                [lastRed, lastSent] = sendOwn(LAB_CONSOLE, 55001, t, uint8(red), lastRed, lastSent, REPEAT);
            case 2
                [lastBlack, lastSent] = sendOwn(LAB_CONSOLE, 55002, t, uint8(black), lastBlack, lastSent, REPEAT);
            case 3
                [lastBlue, lastSent] = sendOwn(LAB_CONSOLE, 55003, t, uint8(blue), lastBlue, lastSent, REPEAT);
            otherwise
        end
    end
else                     % a simulation: record each change for the console
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
% Send [time, text] when the message changed, or it was last sent repeat s ago (or the time
% went backwards, e.g. the experiment clock started)
if ~isequal(msg, last) || t - lastSent >= repeat || t < lastSent
    packet = [typecast(double(t), 'uint8'), reshape(msg, 1, [])];
    spot.hardware.udpSend(ip, port, packet);
    last = msg;
    lastSent = t;
end
end
