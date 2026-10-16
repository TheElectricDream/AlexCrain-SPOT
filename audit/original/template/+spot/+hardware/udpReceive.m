function [data, newPackets, age] = udpReceive(port, senderIP, n, type)
    %UDPRECEIVE  Newest UDP packet every model step (replaces the UDP Receive block).
    %   [data, newPackets, age] = spot.hardware.udpReceive(port, senderIP, n, type). The UDP
    %   Receive block hands the model the OLDEST queued packet each step, so once packets queue
    %   up the model reads old data for the rest of the run. This keeps only the newest: a
    %   background thread (csrc/udp/spot_udp.c) reads each packet as it arrives. Runs in the
    %   generated code on the spacecraft computer; in a desktop simulation the outputs stay at
    %   zeros and -1. All arguments must be constants.
    %
    %   Inputs
    %     port        UDP port to listen on
    %     senderIP    only accept packets from this computer, or '0.0.0.0' for any sender
    %     n, type     the first n values of class type ('single', 'double', 'int32', 'uint8',
    %                 ...); shorter packets are ignored, longer ones give their first n values
    %
    %   Outputs
    %     data        n x 1 of class type: the newest packet (zeros until the first arrives)
    %     newPackets  int32, packets since the previous step (0 = nothing new; data still holds
    %                 the last one)
    %     age         seconds since the newest packet arrived (-1 = nothing yet); e.g. treat
    %                 age > 0.5 as "link lost"
    %
    %   Example (in a MATLAB Function block)
    %     function [data, newPackets, age] = fcn()
    %     [data, newPackets, age] = spot.hardware.udpReceive(36845, '0.0.0.0', 4, 'single');
    %
    %   See also spot.hardware.udpSend, spot.hardware.phasespaceLatest, spot.hardware.Udp.

    %#codegen

    % Outputs in a desktop simulation.
    data = zeros(n, 1, type);
    newPackets = int32(0);
    age = -1;

    % Generated code on the hardware: the newest packet.
    if coder.target('Rtw')
        rx = spot.hardware.Udp.rxOpen(port, senderIP, n * spot.hardware.Udp.bytesOf(type), 0);
        [data, newPackets, age] = spot.hardware.Udp.rxRead(rx, n, type);
    end
end
