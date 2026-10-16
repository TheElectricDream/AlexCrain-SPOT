function udpSend(ip, port, data)
    %UDPSEND  Send a signal as one UDP packet every model step (replaces the UDP Send block).
    %   spot.hardware.udpSend(ip, port, data). Each destination gets its own socket and a send
    %   never waits, so a computer off the network can't slow the model or hold up other
    %   destinations (csrc/udp/spot_udp.c). Runs in the generated code on the spacecraft
    %   computer; in a desktop simulation it does nothing. ip and port must be constants.
    %
    %   Inputs
    %     ip    destination, e.g. '192.168.1.116' or [192 168 1 116] (the block parameter
    %           labConsoleIP). A broadcast address works, but on Wi-Fi broadcasts can be held
    %           back ~0.1 s or more: send to one address when timing matters.
    %     port  destination UDP port
    %     data  any numeric array, sent in memory order (little-endian), e.g. a double is 8
    %           bytes
    %
    %   Example (in a MATLAB Function block)
    %     function fcn(data)
    %     spot.hardware.udpSend('192.168.1.116', 48720, data);
    %
    %   See also spot.hardware.udpReceive, spot.hardware.Udp.

    %#codegen

    % Only in generated code running on the hardware.
    if coder.target('Rtw')
        tx = spot.hardware.Udp.txOpen(ip, port);
        spot.hardware.Udp.txSend(tx, data);
    end
end
