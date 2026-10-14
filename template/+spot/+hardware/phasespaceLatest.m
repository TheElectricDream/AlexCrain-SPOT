function [packet, newPackets, age] = phasespaceLatest(port, senderIP)
%PHASESPACELATEST  Newest PhaseSpace packet every model step (replaces the UDP Receive block).
%
%   The UDP Receive block hands the model the OLDEST queued packet each step, so when PhaseSpace
%   streams faster than the model runs, the queue fills and the model reads data many steps old
%   (about 0.2 s in testing: 960 Hz stream, 50 Hz model). This instead keeps only the newest
%   packet (a background thread in csrc/udp/spot_udp.c reads each packet as it arrives), so the
%   stream can run at any rate faster than the model: tested at 0.6 ms data age in the same case.
%
%   Use it in a MATLAB Function block in place of the UDP Receive block:
%
%       function [packet, newPackets, age] = fcn()
%       [packet, newPackets, age] = spot.hardware.phasespaceLatest(31534, '192.168.1.116');
%
%   port        UDP port StreamData sends to (31534)
%   senderIP    only accept packets from this computer (the TX2: labConsoleIP), as text or four
%               numbers, or '0.0.0.0' for any sender
%
%   packet      115x1 single: the same values the UDP Receive block gave (layout in StreamData.cc)
%   newPackets  int32, packets that arrived since the previous step (0 = nothing new, and packet
%               still holds the last one received)
%   age         seconds since the newest packet arrived (-1 = nothing received yet); e.g. treat
%               age > 0.5 as "PhaseSpace data lost"
%
%   Only packets of exactly 460 bytes (115 float32) from senderIP are accepted, so an old
%   StreamData.exe sending the 19-double format can't mix in. It runs in the generated code on
%   the spacecraft computer; in a normal desktop simulation the outputs stay at zero, like the
%   UDP Receive block. It uses the same receiver as spot.hardware.udpReceive (csrc/udp/spot_udp.c,
%   added to the build by spot.hardware.Udp). Slot 113 carries the experiment clock while an
%   experiment runs (see spot.hardware.experimentTime).
%#codegen

PACKET_LENGTH = 115;   % 19 rigid-body values + 32 LEDs x (x, y, z), float32 on the wire

packet = zeros(PACKET_LENGTH, 1, 'single');
newPackets = int32(0);
age = -1;

if coder.target('Rtw')   % generated code running on the hardware
    h = spot.hardware.Udp.rxOpen(port, senderIP, 4 * PACKET_LENGTH, 4 * PACKET_LENGTH);
    [packet, newPackets, age] = spot.hardware.Udp.rxRead(h, PACKET_LENGTH, 'single');
end
end
