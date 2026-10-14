function [data, newPackets, age] = udpReceive(port, senderIP, n, type)
%UDPRECEIVE  Newest UDP packet every model step (replaces the UDP Receive block).
%
%   The UDP Receive block hands the model the OLDEST queued packet each step, so once packets
%   queue up (a faster sender, or one slow step) the model reads old data for the rest of the
%   run. This keeps only the newest packet: a background thread (csrc/udp/spot_udp.c) reads
%   each one as it arrives. Use it in a MATLAB Function block:
%
%       function [data, newPackets, age] = fcn()
%       [data, newPackets, age] = spot.hardware.udpReceive(36845, '0.0.0.0', 4, 'single');
%
%   port        UDP port to listen on
%   senderIP    only accept packets from this computer, or '0.0.0.0' for any sender
%   n, type     the first n values of class type ('single', 'double', 'int32', 'uint8', ...);
%               shorter packets are ignored, longer ones give their first n values
%
%   data        n x 1 of class type: the newest packet (zeros until the first one arrives)
%   newPackets  int32, packets that arrived since the previous step (0 = nothing new, and data
%               still holds the last one received)
%   age         seconds since the newest packet arrived (-1 = nothing received yet); e.g. treat
%               age > 0.5 as "link lost"
%
%   It runs in the generated code on the spacecraft computer; in a desktop simulation the
%   outputs stay at zeros and -1. All arguments must be constants.
%#codegen

data = zeros(n, 1, type);
newPackets = int32(0);
age = -1;

if coder.target('Rtw')   % generated code running on the hardware
    h = spot.hardware.Udp.rxOpen(port, senderIP, n * spot.hardware.Udp.bytesOf(type), 0);
    [data, newPackets, age] = spot.hardware.Udp.rxRead(h, n, type);
end
end
