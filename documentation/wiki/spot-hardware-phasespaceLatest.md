# spot.hardware.phasespaceLatest

**File:** `+spot/+hardware/phasespaceLatest.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/phasespaceLatest.m -->

## What it is for

*Receive PhaseSpace Data*: the newest PhaseSpace packet every step. With Simulink's *UDP Receive* block, a 960 Hz stream into a 50 Hz model filled the queue and the model read data about 0.2 s old; keeping only the newest packet (a background thread in `spot_udp.c`) gave 0.6 ms data age in the same test, so the stream can run at any rate faster than the model.

```matlab
function [packet, newPackets, age] = fcn()
[packet, newPackets, age] = spot.hardware.phasespaceLatest(31534, labConsoleIP);
```

| | |
|---|---|
| `packet` | 115×1 single: 19 rigid-body values then 32 LEDs × (x, y, z), the layout StreamData sends ([[StreamData.cc|Hardware-StreamData]]) |
| `newPackets` | packets since the previous step |
| `age` | seconds since the newest packet (−1: none yet); `age > 0.5` means PhaseSpace is lost |

Only packets of exactly 460 bytes (115 float32) from the sender are accepted, so an old StreamData sending the 19-double format can't mix in. Slot 113 carries the experiment clock ([[experimentTime|spot-hardware-experimentTime]]). In a desktop simulation the outputs stay at zero, like the block it replaces.

## Line by line

**Line 33.** The packet length.

**Lines 35–37.** Defaults.

**Lines 39–42.** In generated code: open the receiver with the minimum and maximum size both 460 bytes, and read the newest packet as singles.

<!-- xref -->
- **Used by:** `+spot/+hardware/csrc/udp/spot_udp.h`, `+spot/+hardware/experimentTime.m`
- **Uses:** [[spot.hardware.Udp|spot-hardware-Udp]], [[spot.hardware.experimentTime|spot-hardware-experimentTime]], [[spot.hardware.udpReceive|spot-hardware-udpReceive]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/phasespaceLatest.m</b> (43 lines; click to show)</summary>

```matlab
   1  function [packet, newPackets, age] = phasespaceLatest(port, senderIP)
   2  %PHASESPACELATEST  Newest PhaseSpace packet every model step (replaces the UDP Receive block).
   3  %
   4  %   The UDP Receive block hands the model the OLDEST queued packet each step, so when PhaseSpace
   5  %   streams faster than the model runs, the queue fills and the model reads data many steps old
   6  %   (about 0.2 s in testing: 960 Hz stream, 50 Hz model). This instead keeps only the newest
   7  %   packet (a background thread in csrc/udp/spot_udp.c reads each packet as it arrives), so the
   8  %   stream can run at any rate faster than the model: tested at 0.6 ms data age in the same case.
   9  %
  10  %   Use it in a MATLAB Function block in place of the UDP Receive block:
  11  %
  12  %       function [packet, newPackets, age] = fcn()
  13  %       [packet, newPackets, age] = spot.hardware.phasespaceLatest(31534, '192.168.1.116');
  14  %
  15  %   port        UDP port StreamData sends to (31534)
  16  %   senderIP    only accept packets from this computer (the TX2: labConsoleIP), as text or four
  17  %               numbers, or '0.0.0.0' for any sender
  18  %
  19  %   packet      115x1 single: the same values the UDP Receive block gave (layout in StreamData.cc)
  20  %   newPackets  int32, packets that arrived since the previous step (0 = nothing new, and packet
  21  %               still holds the last one received)
  22  %   age         seconds since the newest packet arrived (-1 = nothing received yet); e.g. treat
  23  %               age > 0.5 as "PhaseSpace data lost"
  24  %
  25  %   Only packets of exactly 460 bytes (115 float32) from senderIP are accepted, so an old
  26  %   StreamData.exe sending the 19-double format can't mix in. It runs in the generated code on
  27  %   the spacecraft computer; in a normal desktop simulation the outputs stay at zero, like the
  28  %   UDP Receive block. It uses the same receiver as spot.hardware.udpReceive (csrc/udp/spot_udp.c,
  29  %   added to the build by spot.hardware.Udp). Slot 113 carries the experiment clock while an
  30  %   experiment runs (see spot.hardware.experimentTime).
  31  %#codegen
  32  
  33  PACKET_LENGTH = 115;   % 19 rigid-body values + 32 LEDs x (x, y, z), float32 on the wire
  34  
  35  packet = zeros(PACKET_LENGTH, 1, 'single');
  36  newPackets = int32(0);
  37  age = -1;
  38  
  39  if coder.target('Rtw')   % generated code running on the hardware
  40      h = spot.hardware.Udp.rxOpen(port, senderIP, 4 * PACKET_LENGTH, 4 * PACKET_LENGTH);
  41      [packet, newPackets, age] = spot.hardware.Udp.rxRead(h, PACKET_LENGTH, 'single');
  42  end
  43  end
```

</details>
<!-- /listing -->
