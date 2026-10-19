function liveStream(label, t, u, whoAmI, isSim, labConsoleIP)
    %LIVESTREAM  Send a signal to the SPOT Console's Live Plot, every step (the Live Stream block).
    %   spot.hardware.liveStream(label, t, u, whoAmI, isSim, labConsoleIP). The diagram's "Live
    %   Data Streams" subsystem holds Live Stream blocks: connect any signal to one and it
    %   appears in the console's Live Plot, named after what is connected
    %   (spot.hardware.streamName).
    %     Experiment  one packet per step to the lab console (UDP 55011 RED, 55012 BLACK,
    %                 55013 BLUE): uint8 1 (version), uint8 name length L, uint16 value count n,
    %                 double t, L name characters, n doubles (all little-endian)
    %     Simulation  each step is recorded (spot.sim.liveData); the console plots it while
    %                 the simulation runs and with its replay
    %
    %   Inputs
    %     label         the name as uint8 character codes (from the block's mask)
    %     t             experiment time [s] (Univ_Time)
    %     u             the signal: any numeric or logical array, sent as doubles
    %     whoAmI        1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
    %     isSim         1 in a simulation, 0 in an experiment
    %     labConsoleIP  the lab console's address, four numbers (block parameter labConsoleIP);
    %                   blocks made before it was a setting leave it out (192.168.1.116)
    %
    %   Example (inside the block)
    %     function streamData(u, t, whoAmI, isSim, streamLabel, labConsoleIP)
    %     spot.hardware.liveStream(streamLabel, t, u, whoAmI, isSim, labConsoleIP);
    %     end
    %
    %   See also spot.hardware.streamName, spot.sim.liveData.

    %#codegen

    %% Constants
    PACKET_VERSION = 1;
    PORT_RED = 55011;
    PORT_BLACK = 55012;
    PORT_BLUE = 55013;

    % The lab console's address; older blocks don't pass it.
    if nargin < 6
        LAB_CONSOLE = '192.168.1.116';
    else
        LAB_CONSOLE = labConsoleIP;
    end
    coder.extrinsic('spot.sim.liveData');   % simulations only (not in the generated code)

    values = reshape(double(u), 1, []);
    if coder.target('Rtw')

        % Generated code on a platform, in an experiment: one packet to the lab console.
        if isSim == 0
            packet = [uint8(PACKET_VERSION), uint8(numel(label)), ...
                      typecast(uint16(numel(values)), 'uint8'), ...
                      typecast(double(t), 'uint8'), reshape(uint8(label), 1, []), ...
                      typecast(values, 'uint8')];
            switch whoAmI
                case 1
                    spot.hardware.udpSend(LAB_CONSOLE, PORT_RED, packet);
                case 2
                    spot.hardware.udpSend(LAB_CONSOLE, PORT_BLACK, packet);
                case 3
                    spot.hardware.udpSend(LAB_CONSOLE, PORT_BLUE, packet);
                otherwise
            end
        end
    else
        % A simulation: record it for the console.
        spot.sim.liveData(label, t, values);
    end
end
