function udpSend(ip, port, data)
%UDPSEND  Send a signal as one UDP packet every model step (replaces the UDP Send block).
%
%   Each destination gets its own socket, and a send never waits, so a computer that is off the
%   network can't slow the model down or hold up the other destinations (csrc/udp/spot_udp.c).
%   Use it in a MATLAB Function block:
%
%       function fcn(data)
%       spot.hardware.udpSend('192.168.1.116', 48720, data);
%
%   ip      destination, e.g. '192.168.1.116' or [192 168 1 116] (the lab console's: the block
%           parameter labConsoleIP); a broadcast address ('255.255.255.255') works,
%           but on Wi-Fi broadcasts can be held back ~0.1 s or more, so send to one address
%           when timing matters
%   port    destination UDP port
%   data    any numeric array; its values are sent in memory order as they are in the model
%           (little-endian), e.g. a double is 8 bytes
%
%   It runs in the generated code on the spacecraft computer; in a desktop simulation it does
%   nothing. ip and port must be constants.
%#codegen

if coder.target('Rtw')   % generated code running on the hardware
    h = spot.hardware.Udp.txOpen(ip, port);
    spot.hardware.Udp.txSend(h, data);
end
end
