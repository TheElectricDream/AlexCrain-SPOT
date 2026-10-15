# spot.hardware.packCommand

**File:** `+spot/+hardware/packCommand.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/packCommand.m -->

## What it is for

The command packet for a platform's Teensy ([[firmware|Hardware-Teensy-Firmware]]): 37 bytes, `0xAA` then nine little-endian `single` values: the eight thruster duty cycles in percent (0–100) and the pucks (air bearings on if ≥ 0.5). The Teensy switches everything off when no packet has arrived for 2 s. The lab console's Hardware tab builds the same packet in Python ([[hardware.py|Console-hardware]]).

## How to call it

```matlab
packet = spot.hardware.packCommand(duty, pucks);   % 37x1 uint8
```

## Line by line

**Lines 7–9.** Exactly eight duty cycles.

**Lines 10–12.** The nine values as `single`, reinterpreted as bytes (MATLAB is little-endian on every platform), with the `0xAA` (170) header in front.

<!-- xref -->
- **Used by:** nothing in the code (called by you, from the command window, or by the diagram)
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/packCommand.m</b> (13 lines; click to show)</summary>

```matlab
   1  function packet = packCommand(duty, pucks)
   2  %PACKCOMMAND Command packet for a platform's Teensy (firmware: +spot/+hardware/teensy/teensy_pwm_control.ino).
   3  %   packet = spot.hardware.packCommand(duty, pucks) with DUTY the 8 thruster duty cycles in
   4  %   percent (0-100) and PUCKS the air bearings (on if >= 0.5). Returns the 37-byte packet as a
   5  %   uint8 column: 0xAA, then the 9 values as little-endian single.
   6  %   The Teensy switches everything off when no packet has arrived for 2 s.
   7  if numel(duty) ~= 8
   8      error('spot:hardware:packCommand', 'Give 8 thruster duty cycles.');
   9  end
  10  values = single([duty(:); pucks]);
  11  bytes = typecast(values, 'uint8');           % MATLAB runs little-endian on all its platforms
  12  packet = [uint8(170); bytes(:)];
  13  end
```

</details>
<!-- /listing -->
