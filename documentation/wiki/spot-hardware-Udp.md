# spot.hardware.Udp

**File:** `+spot/+hardware/Udp.m` · **Package:** [[spot.hardware|spot-hardware]] · **C:** [[spot_udp.c|Hardware-spot-udp]]

<!-- source: +spot/+hardware/Udp.m -->

## What it is for

The glue between the UDP functions ([[udpReceive|spot-hardware-udpReceive]], [[udpSend|spot-hardware-udpSend]], [[phasespaceLatest|spot-hardware-phasespaceLatest]], [[experimentTime|spot-hardware-experimentTime]]) and the C code in `csrc/udp/spot_udp.c`. When a diagram using them is built, Simulink calls `updateBuildInfo` (as ordinary MATLAB) to add the C file; the other methods are the `coder.ceval` calls into it. Use the named functions, not these.

## Line by line

**Lines 10–16.** The dependency's name; supported everywhere.

**Lines 18–23.** `updateBuildInfo`: add `csrc/udp` as an include path, `spot_udp.c` as a source, and `-lpthread` (the receive threads).

**Lines 25–31.** `rxOpen(port, senderIP, minBytes, maxBytes)`: the address as text ([[ipText|spot-hardware-ipText]]) with a NUL terminator; `spot_udp_rx_open` returns a handle.

**Lines 33–41.** `rxRead(h, n, type)`: preallocate `n` values of the class; `spot_udp_rx_read` fills them with the newest packet, returns how many packets arrived since the last read, and writes the newest packet's age.

**Lines 43–49.** `txOpen(ip, port)`: a sending socket for a destination.

**Lines 51–56.** `txSend(h, data)`: the array's bytes in memory order as one packet.

**Lines 58–63.** `now()`: a steady clock in seconds.

**Lines 65–77.** `bytesOf(type)`: 8, 4, 2 or 1 bytes per value.

<!-- xref -->
- **Used by:** `+spot/+hardware/experimentTime.m`, `+spot/+hardware/phasespaceLatest.m`, `+spot/+hardware/udpReceive.m`, `+spot/+hardware/udpSend.m`
- **Uses:** [[spot.hardware.ipText|spot-hardware-ipText]], [[spot.hardware.udpReceive|spot-hardware-udpReceive]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/Udp.m</b> (79 lines; click to show)</summary>

```matlab
   1  classdef Udp < coder.ExternalDependency
   2  %UDP  Connects csrc/udp/spot_udp.c to the generated code (used by the UDP functions here).
   3  %   Used by spot.hardware.udpReceive, udpSend, phasespaceLatest and experimentTime. When a
   4  %   diagram that uses them is built, Simulink calls updateBuildInfo below (as ordinary MATLAB),
   5  %   which adds spot_udp.c and .h to the build. The other methods are the C calls; use the
   6  %   functions named above rather than these.
   7  %#codegen
   8  
   9      methods (Static)
  10          function name = getDescriptiveName(~)
  11              name = 'spot_udp';
  12          end
  13  
  14          function supported = isSupportedContext(~)
  15              supported = true;
  16          end
  17  
  18          function updateBuildInfo(buildInfo, ~)
  19              here = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'udp');
  20              buildInfo.addIncludePaths(here);
  21              buildInfo.addSourceFiles('spot_udp.c', here);
  22              buildInfo.addLinkFlags('-lpthread');  % the receive threads (Linux targets such as the Jetson)
  23          end
  24  
  25          function h = rxOpen(port, senderIP, minBytes, maxBytes)
  26              % Listen on a port, keeping the newest packet (see spot_udp.h)
  27              coder.cinclude('spot_udp.h');
  28              ip = [spot.hardware.ipText(senderIP) char(0)];   % text, or four numbers
  29              h = int32(-1);
  30              h = coder.ceval('spot_udp_rx_open', int32(port), coder.rref(ip), int32(minBytes), int32(maxBytes));
  31          end
  32  
  33          function [data, newPackets, age] = rxRead(h, n, type)
  34              % Newest packet as n values of class type, packets since the last read, its age in s
  35              coder.cinclude('spot_udp.h');
  36              data = zeros(n, 1, type);
  37              age = -1;
  38              newPackets = int32(0);
  39              newPackets = coder.ceval('spot_udp_rx_read', int32(h), coder.wref(data), ...
  40                  int32(n * spot.hardware.Udp.bytesOf(type)), coder.wref(age));
  41          end
  42  
  43          function h = txOpen(ip, port)
  44              % A socket that sends to ip:port
  45              coder.cinclude('spot_udp.h');
  46              dest = [spot.hardware.ipText(ip) char(0)];        % text, or four numbers
  47              h = int32(-1);
  48              h = coder.ceval('spot_udp_tx_open', coder.rref(dest), int32(port));
  49          end
  50  
  51          function txSend(h, data)
  52              % data as one packet, in memory order (little-endian), without waiting
  53              coder.cinclude('spot_udp.h');
  54              coder.ceval('spot_udp_tx_send', int32(h), coder.rref(data), ...
  55                  int32(numel(data) * spot.hardware.Udp.bytesOf(class(data))));
  56          end
  57  
  58          function t = now()
  59              % A steady clock in seconds (for intervals)
  60              coder.cinclude('spot_udp.h');
  61              t = 0;
  62              t = coder.ceval('spot_udp_now');
  63          end
  64  
  65          function b = bytesOf(type)
  66              % Bytes per value of a numeric class
  67              switch type
  68                  case {'double', 'int64', 'uint64'}
  69                      b = 8;
  70                  case {'single', 'int32', 'uint32'}
  71                      b = 4;
  72                  case {'int16', 'uint16'}
  73                      b = 2;
  74                  otherwise   % int8, uint8, logical, char
  75                      b = 1;
  76              end
  77          end
  78      end
  79  end
```

</details>
<!-- /listing -->
