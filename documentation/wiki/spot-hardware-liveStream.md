# spot.hardware.liveStream

**File:** `+spot/+hardware/liveStream.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/liveStream.m -->

## What it is for

The *Live Stream* block's function: connect any signal to such a block (in the *Live Data Streams* subsystem) and it appears in the console's Live Plot, named after what is connected ([[streamName|spot-hardware-streamName]]).

- **Experiment**: one UDP packet per step to the lab console (55011 RED, 55012 BLACK, 55013 BLUE): `uint8 1` (version), `uint8` name length, `uint16` value count, `double t`, the name's characters, the values as doubles, all little-endian.
- **Simulation**: each step recorded in [[spot.sim.liveData|spot-sim-liveData]].

## Line by line

**Lines 28–33.** The lab console's address (parameter or default); `spot.sim.liveData` extrinsic.

**Line 35.** The signal as a row of doubles.

**Lines 36–49.** On a platform, not simulating: build the packet and send it to the port for this platform.

**Lines 50–52.** In MATLAB: record it.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/experiment.py`, `+spot/+console/webapp/static/liveplot.js`, `+spot/+sim/Contents.m`, `+spot/+sim/liveData.m`, `+spot/+sim/run.m`
- **Uses:** [[spot.hardware.streamName|spot-hardware-streamName]], [[spot.hardware.udpSend|spot-hardware-udpSend]], [[spot.sim.liveData|spot-sim-liveData]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/liveStream.m</b> (53 lines; click to show)</summary>

```matlab
   1  function liveStream(label, t, u, whoAmI, isSim, labConsoleIP)
   2  %LIVESTREAM  Send a signal to the SPOT Console's Live Plot, every step (the Live Stream block).
   3  %
   4  %   The diagram's "Live Data Streams" subsystem holds Live Stream blocks: connect any signal
   5  %   (e.g. a Data Store Read) to one and it appears in the console's Live Plot, named after what
   6  %   is connected (spot.hardware.streamName). Inside the block:
   7  %
   8  %       function streamData(u, t, whoAmI, isSim, streamLabel, labConsoleIP)
   9  %       spot.hardware.liveStream(streamLabel, t, u, whoAmI, isSim, labConsoleIP);
  10  %       end
  11  %
  12  %   Experiment (generated code on a platform): one packet per step to the lab console
  13  %   (labConsoleIP, the settings' labConsoleAddress; UDP 55011 RED, 55012 BLACK, 55013 BLUE):
  14  %       uint8 1 (version), uint8 name length L, uint16 value count n,
  15  %       double t, L name characters, n doubles (all little-endian)
  16  %   Simulation: each step is recorded (spot.sim.liveData); the console plots it while the
  17  %   simulation runs and with its replay.
  18  %
  19  %   label    the name as uint8 character codes (from the block's mask)
  20  %   t        experiment time [s] (Univ_Time)
  21  %   u        the signal: any numeric or logical array, sent as doubles
  22  %   whoAmI   1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
  23  %   isSim    1 in a simulation, 0 in an experiment
  24  %   labConsoleIP  the lab console's address, four numbers (the block parameter labConsoleIP);
  25  %            blocks made before it was a setting leave it out and send to 192.168.1.116
  26  %#codegen
  27  
  28  if nargin < 6
  29      LAB_CONSOLE = '192.168.1.116';
  30  else
  31      LAB_CONSOLE = labConsoleIP;
  32  end
  33  coder.extrinsic('spot.sim.liveData');   % simulations only (not in the generated code)
  34  
  35  v = reshape(double(u), 1, []);
  36  if coder.target('Rtw')   % generated code running on a platform
  37      if isSim == 0
  38          packet = [uint8(1), uint8(numel(label)), typecast(uint16(numel(v)), 'uint8'), ...
  39                    typecast(double(t), 'uint8'), reshape(uint8(label), 1, []), typecast(v, 'uint8')];
  40          switch whoAmI
  41              case 1
  42                  spot.hardware.udpSend(LAB_CONSOLE, 55011, packet);
  43              case 2
  44                  spot.hardware.udpSend(LAB_CONSOLE, 55012, packet);
  45              case 3
  46                  spot.hardware.udpSend(LAB_CONSOLE, 55013, packet);
  47              otherwise
  48          end
  49      end
  50  else                     % a simulation: record it for the console
  51      spot.sim.liveData(label, t, v);
  52  end
  53  end
```

</details>
<!-- /listing -->
