# spot.params.ipv4

**File:** `+spot/+params/ipv4.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/ipv4.m -->

## What it is for

The diagram's UDP blocks take the lab console's address as a parameter, and a block parameter can't be text, so the address is a vector of four numbers. This turns `'192.168.1.116'` into `[192 168 1 116]`; [[spot.hardware.ipText|spot-hardware-ipText]] goes the other way.

## How to call it

```matlab
ip = spot.params.ipv4(s.labConsoleAddress);   % [192 168 1 116]
```

## Line by line

**Line 5.** Trim spaces, make sure it is a char array, and match it against the pattern "one to three digits, a dot, …, four times" anchored at both ends. `tokens` with `once` returns the four digit groups as a cell array, or empty when the text isn't shaped like an address.

**Lines 6–8.** No match: an error that quotes what was given.

**Line 9.** Convert each group to a number.

**Lines 10–12.** Three digits can spell 999, so check that no number is above 255.

<!-- xref -->
- **Used by:** `+spot/+params/apply.m`
- **Uses:** [[spot.hardware.ipText|spot-hardware-ipText]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/ipv4.m</b> (13 lines; click to show)</summary>

```matlab
   1  function ip = ipv4(address)
   2  %IPV4 An IPv4 address as four numbers: [192 168 1 116] from '192.168.1.116'.
   3  %   ip = spot.params.ipv4(address). The diagram's UDP blocks take the lab console's address this
   4  %   way (labConsoleIP: a block parameter can't be text); spot.hardware.ipText turns it back.
   5  parts = regexp(strtrim(char(address)), '^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$', 'tokens', 'once');
   6  if isempty(parts)
   7      error('spot:params:ipv4', 'The lab console address must be an IPv4 address such as 192.168.1.116 (it is "%s").', char(address));
   8  end
   9  ip = cellfun(@str2double, parts);
  10  if any(ip > 255)
  11      error('spot:params:ipv4', 'The lab console address %s has a number above 255.', char(address));
  12  end
  13  end
```

</details>
<!-- /listing -->
