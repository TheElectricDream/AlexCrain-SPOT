# spot.hardware.experimentTime

**File:** `+spot/+hardware/experimentTime.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/experimentTime.m -->

## What it is for

The *Experiment Clock*: the one time that every platform agrees on. All platforms receive the same PhaseSpace packets from the lab console. When the lab console starts an experiment it waits until every platform's program is running, then tells StreamData to start the experiment a few seconds later; from then on every packet carries the experiment clock in slot 113 (LED 31's x, which PhaseSpace never uses):

| slot 113 | meaning |
|---|---|
| NaN | no experiment armed |
| negative | counting down to the agreed start |
| ≥ 0 | seconds since the start |

So every platform starts at the same instant, whenever its own program was launched, and the phases switch together. Between packets the time is advanced by the packet's age, so it moves smoothly at any stream rate.

| `state` | |
|---|---|
| 0 | waiting: no experiment armed yet (`t = 0`) |
| 1 | counting down (`t = 0`) |
| 2 | running on the shared clock |
| 3 | running on this computer's clock: the shared clock was lost or jumped back after the start; `t` carries on from where it was |

A program started after the countdown (it never saw the clock negative) stays at `t = 0` rather than jumping into the middle of an experiment.

## Line by line

**Lines 28–29.** The slot; how far back the shared clock may jump before it counts as lost (0.5 s).

**Lines 31–37.** Persistent state: armed, started, the last time, the last local clock.

**Lines 39–47.** The local steady clock (in generated code; [[Udp.now|spot-hardware-Udp]]); the clock value from the packet; valid if a packet has arrived and the value isn't NaN; add the packet's age.

**Lines 49–59.** Not started: a negative clock arms; a valid clock after arming starts (`t = c`, state 2).

**Lines 60–68.** Started: a valid clock not too far behind gives `t = max(c, lastT)` (never backwards), state 2; otherwise the local clock carries on, state 3.

**Lines 69–72.** Remember the time and the local clock once started.

<!-- xref -->
- **Used by:** `+spot/+console/phasespace/StreamData.cc`, `+spot/+console/webapp/spot_console/experiment.py`, `+spot/+console/webapp/static/experiment.js`, `+spot/+hardware/phasespaceLatest.m`
- **Uses:** [[spot.hardware.Udp|spot-hardware-Udp]], [[spot.hardware.phasespaceLatest|spot-hardware-phasespaceLatest]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/experimentTime.m</b> (73 lines; click to show)</summary>

```matlab
   1  function [t, state] = experimentTime(packet, age)
   2  %EXPERIMENTTIME  The experiment's time, the same on every platform, from the PhaseSpace stream.
   3  %
   4  %   All platforms receive the same PhaseSpace packets from the lab console (TX2). When the lab
   5  %   console starts an experiment, it waits until every platform's program is running, then
   6  %   tells StreamData to start the experiment a few seconds later. From then on every packet
   7  %   carries the experiment clock in slot 113 (LED 31's x, which PhaseSpace never uses):
   8  %     NaN        no experiment armed
   9  %     negative   counting down to the agreed start
  10  %     >= 0       seconds since the start
  11  %   So every platform starts at the same moment, whenever its own program was started, and the
  12  %   phases switch together. Use it in a MATLAB Function block, on the packet and age from
  13  %   spot.hardware.phasespaceLatest:
  14  %
  15  %       function [t, state] = fcn(packet, age)
  16  %       [t, state] = spot.hardware.experimentTime(packet, age);
  17  %
  18  %   t       experiment time [s]: 0 until the start, then the shared clock. Between packets
  19  %           it is advanced by the packet's age, so it moves smoothly at any stream rate.
  20  %   state   0 waiting (no experiment armed yet), 1 counting down, 2 running on the shared
  21  %           clock, 3 running on this computer's clock (the shared clock was lost or jumped
  22  %           back after the start: t carries on from where it was instead)
  23  %
  24  %   Safety: a program that was started after the countdown (it never saw the clock negative)
  25  %   stays at t = 0, rather than jumping into the middle of an experiment.
  26  %#codegen
  27  
  28  EXP_CLOCK = 113;     % 1-based slot of the experiment clock in the PhaseSpace packet
  29  MAX_BACKWARDS = 0.5; % [s] a shared clock this far behind t counts as lost
  30  
  31  persistent armed started lastT lastNow
  32  if isempty(armed)
  33      armed = false;
  34      started = false;
  35      lastT = 0;
  36      lastNow = 0;
  37  end
  38  
  39  now = 0;
  40  if coder.target('Rtw')
  41      now = spot.hardware.Udp.now();
  42  end
  43  c = double(packet(EXP_CLOCK));
  44  valid = age >= 0 && ~isnan(c);
  45  if valid
  46      c = c + age;   % the clock when the newest packet left, plus the time since it arrived
  47  end
  48  
  49  t = 0;
  50  state = 0;
  51  if ~started
  52      if valid && c < 0
  53          armed = true;
  54          state = 1;
  55      elseif valid && armed
  56          started = true;
  57          t = c;
  58          state = 2;
  59      end
  60  else
  61      if valid && c > lastT - MAX_BACKWARDS
  62          t = max(c, lastT);   % never backwards
  63          state = 2;
  64      else
  65          t = lastT + (now - lastNow);
  66          state = 3;
  67      end
  68  end
  69  if started
  70      lastT = t;
  71      lastNow = now;
  72  end
  73  end
```

</details>
<!-- /listing -->
