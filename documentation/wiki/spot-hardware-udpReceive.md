# spot.hardware.udpReceive

**File:** `+spot/+hardware/udpReceive.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/udpReceive.m -->

## What it is for

A replacement for Simulink's *UDP Receive* block, which hands the model the **oldest** queued packet each step: once packets queue up (a faster sender, or one slow step) the model reads stale data for the rest of the run. This keeps only the **newest** packet: a background thread in `spot_udp.c` reads each one as it arrives. Used for the link from the Orin (*Receive Data from Orin*).

```matlab
function [data, newPackets, age] = fcn()
[data, newPackets, age] = spot.hardware.udpReceive(36845, '0.0.0.0', 4, 'single');
```

| | |
|---|---|
| `port`, `senderIP` | listen on the port; accept only that sender (`'0.0.0.0'` = anyone) |
| `n`, `type` | the first `n` values of that class; shorter packets are ignored |
| `data` | `n×1` of the class: the newest packet (zeros until one arrives) |
| `newPackets` | packets since the previous step (0 = nothing new; `data` still holds the last) |
| `age` | seconds since the newest packet (−1 = none yet); e.g. `age > 0.5` means the link is lost |

In a desktop simulation the outputs stay at zeros and −1. All arguments must be constants (the generated code opens the socket once).

## Line by line

**Lines 27–29.** The default outputs.

**Lines 31–34.** In generated code: open (or reuse) the receiver with a minimum packet size of `n` values and no maximum, and read the newest packet.

<!-- xref -->
- **Used by:** `+spot/+hardware/Udp.m`, `+spot/+hardware/csrc/udp/spot_udp.h`, `+spot/+hardware/phasespaceLatest.m`
- **Uses:** [[spot.hardware.Udp|spot-hardware-Udp]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/udpReceive.m</b> (35 lines; click to show)</summary>

```matlab
   1  function [data, newPackets, age] = udpReceive(port, senderIP, n, type)
   2  %UDPRECEIVE  Newest UDP packet every model step (replaces the UDP Receive block).
   3  %
   4  %   The UDP Receive block hands the model the OLDEST queued packet each step, so once packets
   5  %   queue up (a faster sender, or one slow step) the model reads old data for the rest of the
   6  %   run. This keeps only the newest packet: a background thread (csrc/udp/spot_udp.c) reads
   7  %   each one as it arrives. Use it in a MATLAB Function block:
   8  %
   9  %       function [data, newPackets, age] = fcn()
  10  %       [data, newPackets, age] = spot.hardware.udpReceive(36845, '0.0.0.0', 4, 'single');
  11  %
  12  %   port        UDP port to listen on
  13  %   senderIP    only accept packets from this computer, or '0.0.0.0' for any sender
  14  %   n, type     the first n values of class type ('single', 'double', 'int32', 'uint8', ...);
  15  %               shorter packets are ignored, longer ones give their first n values
  16  %
  17  %   data        n x 1 of class type: the newest packet (zeros until the first one arrives)
  18  %   newPackets  int32, packets that arrived since the previous step (0 = nothing new, and data
  19  %               still holds the last one received)
  20  %   age         seconds since the newest packet arrived (-1 = nothing received yet); e.g. treat
  21  %               age > 0.5 as "link lost"
  22  %
  23  %   It runs in the generated code on the spacecraft computer; in a desktop simulation the
  24  %   outputs stay at zeros and -1. All arguments must be constants.
  25  %#codegen
  26  
  27  data = zeros(n, 1, type);
  28  newPackets = int32(0);
  29  age = -1;
  30  
  31  if coder.target('Rtw')   % generated code running on the hardware
  32      h = spot.hardware.Udp.rxOpen(port, senderIP, n * spot.hardware.Udp.bytesOf(type), 0);
  33      [data, newPackets, age] = spot.hardware.Udp.rxRead(h, n, type);
  34  end
  35  end
```

</details>
<!-- /listing -->
