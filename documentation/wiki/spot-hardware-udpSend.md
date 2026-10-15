# spot.hardware.udpSend

**File:** `+spot/+hardware/udpSend.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/udpSend.m -->

## What it is for

A replacement for the *UDP Send* block: send an array as one packet, every step, without ever waiting. Each destination gets its own socket, so a computer that is off the network can't slow the model down or hold up the other destinations. Used by *Send Data to Orin*, the debug messages and the live streams.

```matlab
function fcn(data)
spot.hardware.udpSend('192.168.1.116', 48720, data);     % or [192 168 1 116] (labConsoleIP)
```

The values go in memory order, little-endian (a double is 8 bytes). Broadcast addresses work but Wi-Fi can hold broadcasts back by 0.1 s or more, so send to one address when timing matters. Nothing happens in a desktop simulation; `ip` and `port` must be constants.

## Line by line

**Lines 23–26.** In generated code: open (or reuse) the socket for this destination and send the bytes.

<!-- xref -->
- **Used by:** `+spot/+hardware/csrc/udp/spot_udp.h`, `+spot/+hardware/debugMessages.m`, `+spot/+hardware/liveStream.m`
- **Uses:** [[spot.hardware.Udp|spot-hardware-Udp]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/udpSend.m</b> (27 lines; click to show)</summary>

```matlab
   1  function udpSend(ip, port, data)
   2  %UDPSEND  Send a signal as one UDP packet every model step (replaces the UDP Send block).
   3  %
   4  %   Each destination gets its own socket, and a send never waits, so a computer that is off the
   5  %   network can't slow the model down or hold up the other destinations (csrc/udp/spot_udp.c).
   6  %   Use it in a MATLAB Function block:
   7  %
   8  %       function fcn(data)
   9  %       spot.hardware.udpSend('192.168.1.116', 48720, data);
  10  %
  11  %   ip      destination, e.g. '192.168.1.116' or [192 168 1 116] (the lab console's: the block
  12  %           parameter labConsoleIP); a broadcast address ('255.255.255.255') works,
  13  %           but on Wi-Fi broadcasts can be held back ~0.1 s or more, so send to one address
  14  %           when timing matters
  15  %   port    destination UDP port
  16  %   data    any numeric array; its values are sent in memory order as they are in the model
  17  %           (little-endian), e.g. a double is 8 bytes
  18  %
  19  %   It runs in the generated code on the spacecraft computer; in a desktop simulation it does
  20  %   nothing. ip and port must be constants.
  21  %#codegen
  22  
  23  if coder.target('Rtw')   % generated code running on the hardware
  24      h = spot.hardware.Udp.txOpen(ip, port);
  25      spot.hardware.Udp.txSend(h, data);
  26  end
  27  end
```

</details>
<!-- /listing -->
