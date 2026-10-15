# spot.params.platformSelection

**File:** `+spot/+params/platformSelection.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/platformSelection.m -->

## What it is for

The diagram doesn't read four switches; it reads one number, `platformSelection`, whose value says which platforms take part. This function turns the settings' four logicals into that number, and refuses combinations the diagram has no branch for.

| Code | Platforms | Code | Platforms |
|---|---|---|---|
| 0 | none | 6 | BLUE |
| 1 | RED | 7 | RED + BLUE |
| 2 | BLACK | 8 | RED + BLACK + BLUE |
| 3 | RED + BLACK | 9 | BLACK + BLUE |
| 4 | RED + BLACK + ARM | 10 | RED + BLUE + ARM |
| 5 | RED + ARM | 11 | RED + BLACK + BLUE + ARM |

The arm is only ever with the Chaser (it is mounted on it), so e.g. BLACK + ARM has no code.

## How to call it

```matlab
n = spot.params.platformSelection(s.platforms);
```

## Line by line

**Line 10.** Pack the four switches into a 1×4 logical row in the fixed order RED, BLACK, BLUE, ARM.

**Lines 11–22.** The table above as a matrix: four columns of switches and a fifth with the code.

**Line 23.** Find the first row whose four switch columns all equal `key`.

**Lines 24–28.** No row means the diagram can't run that combination: the error names the platforms that were on, in words ("Chaser (RED) + Arm").

**Line 29.** Return that row's code.

<!-- xref -->
- **Used by:** `+spot/+params/apply.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/platformSelection.m</b> (30 lines; click to show)</summary>

```matlab
   1  function n = platformSelection(p)
   2  %PLATFORMSELECTION The diagrams' platformSelection code for a set of platforms.
   3  %   n = spot.params.platformSelection(s.platforms), where the struct has logical fields
   4  %   RED, BLACK, BLUE and ARM. Errors for combinations the diagrams don't implement.
   5  %
   6  %   0 none            4 RED+BLACK+ARM   8 RED+BLACK+BLUE
   7  %   1 RED             5 RED+ARM         9 BLACK+BLUE
   8  %   2 BLACK           6 BLUE           10 RED+BLUE+ARM
   9  %   3 RED+BLACK       7 RED+BLUE       11 RED+BLACK+BLUE+ARM
  10  key = [logical(p.RED), logical(p.BLACK), logical(p.BLUE), logical(p.ARM)];
  11  codes = [0 0 0 0  0
  12           1 0 0 0  1
  13           0 1 0 0  2
  14           1 1 0 0  3
  15           1 1 0 1  4
  16           1 0 0 1  5
  17           0 0 1 0  6
  18           1 0 1 0  7
  19           1 1 1 0  8
  20           0 1 1 0  9
  21           1 0 1 1 10
  22           1 1 1 1 11];
  23  row = find(all(codes(:, 1:4) == key, 2), 1);
  24  if isempty(row)
  25      names = {'Chaser (RED)', 'Target (BLACK)', 'Obstacle (BLUE)', 'Arm'};
  26      error('spot:params:platformSelection', ...
  27          'The combination %s has not been implemented in the diagrams.', strjoin(names(key), ' + '));
  28  end
  29  n = codes(row, 5);
  30  end
```

</details>
<!-- /listing -->
