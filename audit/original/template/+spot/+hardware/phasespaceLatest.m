function [packet, newPackets, age] = phasespaceLatest(port, senderIP)
    %PHASESPACELATEST  Newest PhaseSpace packet every model step (replaces the UDP Receive block).
    %   [packet, newPackets, age] = spot.hardware.phasespaceLatest(port, senderIP). The UDP
    %   Receive block hands the model the OLDEST queued packet each step, so when PhaseSpace
    %   streams faster than the model runs the model reads data many steps old (about 0.2 s at
    %   960 Hz stream, 50 Hz model). This keeps only the newest packet (0.6 ms data age in the
    %   same test), using the same receiver as spot.hardware.udpReceive.
    %
    %   Only packets of exactly 460 bytes (115 float32) from senderIP are accepted, so an old
    %   StreamData.exe sending the 19-double format can't mix in. Runs in the generated code on
    %   the spacecraft computer; in a desktop simulation the outputs stay at zero. Slot 113
    %   carries the experiment clock while an experiment runs (spot.hardware.experimentTime).
    %
    %   Inputs
    %     port        UDP port StreamData sends to (31534)
    %     senderIP    only accept packets from this computer (the TX2: labConsoleIP), as text
    %                 or four numbers, or '0.0.0.0' for any sender
    %
    %   Outputs
    %     packet      115x1 single: the values the UDP Receive block gave (layout in
    %                 StreamData.cc)
    %     newPackets  int32, packets since the previous step (0 = nothing new; packet still
    %                 holds the last one)
    %     age         seconds since the newest packet arrived (-1 = nothing yet); e.g. treat
    %                 age > 0.5 as "PhaseSpace data lost"
    %
    %   Example (in a MATLAB Function block, in place of the UDP Receive block)
    %     function [packet, newPackets, age] = fcn()
    %     [packet, newPackets, age] = spot.hardware.phasespaceLatest(31534, '192.168.1.116');
    %
    %   See also spot.hardware.experimentTime, spot.hardware.udpReceive.

    %#codegen

    %% Constants
    PACKET_LENGTH = Constants.PACKET_LENGTH;             % rigid-body values + LEDs x (x, y, z)
    BYTES_PER_VALUE = Constants.PACKET_BYTES_PER_VALUE;  % float32 on the wire

    % Outputs in a desktop simulation.
    packet = zeros(PACKET_LENGTH, 1, 'single');
    newPackets = int32(0);
    age = -1;

    % Generated code on the hardware: the newest packet of exactly the right size.
    if coder.target('Rtw')
        rx = spot.hardware.Udp.rxOpen(port, senderIP, BYTES_PER_VALUE * PACKET_LENGTH, ...
            BYTES_PER_VALUE * PACKET_LENGTH);
        [packet, newPackets, age] = spot.hardware.Udp.rxRead(rx, PACKET_LENGTH, 'single');
    end
end
