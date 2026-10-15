# spot.hardware.ipText

**File:** `+spot/+hardware/ipText.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/ipText.m -->

## What it is for

The C code wants an address as text, but the diagram carries the lab console's address as four numbers (`labConsoleIP`, because a block parameter can't be text). This writes `[192 168 1 116]` out as `'192.168.1.116'`, in a fixed 16-character buffer padded with NULs so the generated code has a fixed-size string. Text is returned unchanged. The reverse is [[spot.params.ipv4|spot-params-ipv4]].

## Line by line

**Lines 7–10.** Text goes straight through.

**Lines 11–12.** A 16-character buffer of NULs and a write position.

**Lines 13–25.** For each of the four numbers: its hundreds, tens and units digits; skip the leading zeros (start at digit 2 if below 100, 3 if below 10); write the digits as characters; a dot after all but the last.

<!-- xref -->
- **Used by:** `+spot/+hardware/Udp.m`, `+spot/+params/ipv4.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/ipText.m</b> (26 lines; click to show)</summary>

```matlab
   1  function s = ipText(ip)
   2  %IPTEXT An IPv4 address as text for the C code: '192.168.1.116' from [192 168 1 116].
   3  %   s = spot.hardware.ipText(ip): ip as text is returned as it is; as four numbers (e.g. the
   4  %   workspace's labConsoleIP, a block parameter) it is written out as 15 characters at most,
   5  %   padded with NUL characters, so the generated code has a fixed-size string.
   6  %#codegen
   7  if ischar(ip)
   8      s = ip;
   9      return
  10  end
  11  s = char(zeros(1, 16));
  12  k = 0;
  13  for i = 1:4
  14      v = round(double(ip(i)));
  15      digits = [floor(v / 100), mod(floor(v / 10), 10), mod(v, 10)];
  16      first = 1 + (v < 100) + (v < 10);
  17      for j = first:3
  18          k = k + 1;
  19          s(k) = char(48 + digits(j));
  20      end
  21      if i < 4
  22          k = k + 1;
  23          s(k) = '.';
  24      end
  25  end
  26  end
```

</details>
<!-- /listing -->
