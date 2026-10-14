function liveStream(label, t, u, whoAmI, isSim, labConsoleIP)
%LIVESTREAM  Send a signal to the SPOT Console's Live Plot, every step (the Live Stream block).
%
%   The diagram's "Live Data Streams" subsystem holds Live Stream blocks: connect any signal
%   (e.g. a Data Store Read) to one and it appears in the console's Live Plot, named after what
%   is connected (spot.hardware.streamName). Inside the block:
%
%       function streamData(u, t, whoAmI, isSim, streamLabel, labConsoleIP)
%       spot.hardware.liveStream(streamLabel, t, u, whoAmI, isSim, labConsoleIP);
%       end
%
%   Experiment (generated code on a platform): one packet per step to the lab console
%   (labConsoleIP, the settings' labConsoleAddress; UDP 55011 RED, 55012 BLACK, 55013 BLUE):
%       uint8 1 (version), uint8 name length L, uint16 value count n,
%       double t, L name characters, n doubles (all little-endian)
%   Simulation: each step is recorded (spot.sim.liveData); the console plots it while the
%   simulation runs and with its replay.
%
%   label    the name as uint8 character codes (from the block's mask)
%   t        experiment time [s] (Univ_Time)
%   u        the signal: any numeric or logical array, sent as doubles
%   whoAmI   1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
%   isSim    1 in a simulation, 0 in an experiment
%   labConsoleIP  the lab console's address, four numbers (the block parameter labConsoleIP);
%            blocks made before it was a setting leave it out and send to 192.168.1.116
%#codegen

if nargin < 6
    LAB_CONSOLE = '192.168.1.116';
else
    LAB_CONSOLE = labConsoleIP;
end
coder.extrinsic('spot.sim.liveData');   % simulations only (not in the generated code)

v = reshape(double(u), 1, []);
if coder.target('Rtw')   % generated code running on a platform
    if isSim == 0
        packet = [uint8(1), uint8(numel(label)), typecast(uint16(numel(v)), 'uint8'), ...
                  typecast(double(t), 'uint8'), reshape(uint8(label), 1, []), typecast(v, 'uint8')];
        switch whoAmI
            case 1
                spot.hardware.udpSend(LAB_CONSOLE, 55011, packet);
            case 2
                spot.hardware.udpSend(LAB_CONSOLE, 55012, packet);
            case 3
                spot.hardware.udpSend(LAB_CONSOLE, 55013, packet);
            otherwise
        end
    end
else                     % a simulation: record it for the console
    spot.sim.liveData(label, t, v);
end
end
