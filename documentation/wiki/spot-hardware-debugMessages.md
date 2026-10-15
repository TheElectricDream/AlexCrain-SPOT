# spot.hardware.debugMessages

**File:** `+spot/+hardware/debugMessages.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/debugMessages.m -->

## What it is for

The Messages panel's source. The diagram writes a short text to `RED_Logger`, `BLACK_Logger`, `BLUE_Logger` ("Chaser: Running Phase 1"); the *Debugger* subsystem turns them into character codes and calls this every step, in simulations and experiments alike:

- **Experiment** (generated code): the platform's *own* message goes to the lab console as a UDP packet (port 55001 RED, 55002 BLACK, 55003 BLUE): the experiment time as a double, then the text. Sent when it changes and every 0.5 s while unchanged, so a lost packet only delays it.
- **Simulation**: every change of every platform's message is recorded with its time in [[spot.sim.debugLog|spot-sim-debugLog]].

The MATLAB Function block around it is one line: `spot.hardware.debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)`.

## Line by line

**Lines 28–34.** The lab console's address (the block parameter, or the lab's default for old blocks); the repeat interval; `spot.sim.debugLog` is extrinsic (MATLAB only, left out of the generated code).

**Lines 36–42.** Persistent memory of the last text per platform and the last send time, initialised to empty messages.

**Lines 44–55.** On a platform (`coder.target('Rtw')`), not simulating: send this platform's message with `sendOwn` to its port.

**Lines 56–69.** In MATLAB: for each platform, if the text changed, remember it and record it.

**Lines 72–81.** `sendOwn`: send `[time as 8 bytes, text]` ([[udpSend|spot-hardware-udpSend]]) when the text changed, the repeat interval passed, or the clock went backwards (the experiment clock just started); remember the text and the time.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/experiment.py`, `+spot/+console/webapp/static/messages.js`, `+spot/+sim/Contents.m`, `+spot/+sim/debugLog.m`, `+spot/+sim/run.m`
- **Uses:** [[spot.hardware.udpSend|spot-hardware-udpSend]], [[spot.sim.debugLog|spot-sim-debugLog]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/debugMessages.m</b> (81 lines; click to show)</summary>

```matlab
   1  function debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
   2  %DEBUGMESSAGES  Each platform's debug message, for the SPOT Console's Messages panel.
   3  %
   4  %   The diagram writes a short text to RED_Logger, BLACK_Logger and BLUE_Logger (e.g. "Chaser:
   5  %   Running Phase 1"). The Debugger subsystem turns them into character codes (String to ASCII)
   6  %   and calls this every step, in simulations and experiments alike, so what a simulation shows
   7  %   is what the experiment will show:
   8  %
   9  %       function sendDebugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP)
  10  %       spot.hardware.debugMessages(whoAmI, isSim, t, red, black, blue, labConsoleIP);
  11  %       end
  12  %
  13  %   Experiment (generated code on a platform): the platform's own message goes to the lab
  14  %   console (labConsoleIP; UDP 55001 RED, 55002 BLACK, 55003 BLUE) as one packet: the
  15  %   experiment time (double, 8 bytes) followed by the text. It is sent when it changes and every
  16  %   0.5 s while it stays the same, so a lost packet only delays it.
  17  %   Simulation: every change of each platform's message is recorded with its time
  18  %   (spot.sim.debugLog); the console shows them while the simulation runs and with its replay.
  19  %
  20  %   whoAmI            1 RED, 2 BLACK, 3 BLUE on a platform (0 in a simulation)
  21  %   isSim             1 in a simulation, 0 in an experiment
  22  %   t                 experiment time [s] (Univ_Time)
  23  %   red, black, blue  the messages as uint8 character codes; zeros after the text are ignored
  24  %   labConsoleIP      the lab console's address, four numbers (the block parameter labConsoleIP,
  25  %                     the settings' labConsoleAddress); without it, 192.168.1.116
  26  %#codegen
  27  
  28  if nargin < 7
  29      LAB_CONSOLE = '192.168.1.116';
  30  else
  31      LAB_CONSOLE = labConsoleIP;
  32  end
  33  REPEAT = 0.5;   % [s] resend an unchanged message this often
  34  coder.extrinsic('spot.sim.debugLog');   % simulations only (not in the generated code)
  35  
  36  persistent lastRed lastBlack lastBlue lastSent
  37  if isempty(lastSent)
  38      lastRed = zeros(size(red), 'uint8');
  39      lastBlack = zeros(size(black), 'uint8');
  40      lastBlue = zeros(size(blue), 'uint8');
  41      lastSent = -inf;
  42  end
  43  
  44  if coder.target('Rtw')   % generated code running on a platform
  45      if isSim == 0
  46          switch whoAmI
  47              case 1
  48                  [lastRed, lastSent] = sendOwn(LAB_CONSOLE, 55001, t, uint8(red), lastRed, lastSent, REPEAT);
  49              case 2
  50                  [lastBlack, lastSent] = sendOwn(LAB_CONSOLE, 55002, t, uint8(black), lastBlack, lastSent, REPEAT);
  51              case 3
  52                  [lastBlue, lastSent] = sendOwn(LAB_CONSOLE, 55003, t, uint8(blue), lastBlue, lastSent, REPEAT);
  53              otherwise
  54          end
  55      end
  56  else                     % a simulation: record each change for the console
  57      if ~isequal(uint8(red), lastRed)
  58          lastRed = uint8(red);
  59          spot.sim.debugLog(1, t, lastRed);
  60      end
  61      if ~isequal(uint8(black), lastBlack)
  62          lastBlack = uint8(black);
  63          spot.sim.debugLog(2, t, lastBlack);
  64      end
  65      if ~isequal(uint8(blue), lastBlue)
  66          lastBlue = uint8(blue);
  67          spot.sim.debugLog(3, t, lastBlue);
  68      end
  69  end
  70  end
  71  
  72  function [last, lastSent] = sendOwn(ip, port, t, msg, last, lastSent, repeat)
  73  % Send [time, text] when the message changed, or it was last sent repeat s ago (or the time
  74  % went backwards, e.g. the experiment clock started)
  75  if ~isequal(msg, last) || t - lastSent >= repeat || t < lastSent
  76      packet = [typecast(double(t), 'uint8'), reshape(msg, 1, [])];
  77      spot.hardware.udpSend(ip, port, packet);
  78      last = msg;
  79      lastSent = t;
  80  end
  81  end
```

</details>
<!-- /listing -->
