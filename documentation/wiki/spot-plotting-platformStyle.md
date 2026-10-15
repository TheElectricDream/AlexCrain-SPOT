# spot.plotting.platformStyle

**File:** `+spot/+plotting/platformStyle.m` · **Package:** [[spot.plotting|spot-plotting]]

<!-- source: +spot/+plotting/platformStyle.m -->

## What it is for

The code, the diagram and the data keep the keys `RED`, `BLACK`, `BLUE` (and `ARM`); people see *Chaser*, *Target*, *Obstacle*. This maps a key to its name and its figure colour: the Chaser a mid grey (its silver MLI would vanish on white paper), the Target gold, the Obstacle blue, the arm the Chaser's grey; anything else as given, in black. The page has the same table in `platforms.js`.

## How to call it

```matlab
[name, color] = spot.plotting.platformStyle('BLACK');   % 'Target', [0.80 0.58 0.05]
```

## Line by line

**Lines 7–18.** A `switch` on the upper-cased key with the five cases above.

<!-- xref -->
- **Used by:** `+spot/+data/load.m`, `+spot/+data/toFigure.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+plotting/platformStyle.m</b> (19 lines; click to show)</summary>

```matlab
   1  function [name, color] = platformStyle(key)
   2  %PLATFORMSTYLE What a platform is called and its colour in figures.
   3  %   [name, color] = spot.plotting.platformStyle('RED') returns 'Chaser' and its RGB colour. The
   4  %   keys stay RED, BLACK and BLUE (and ARM) in the code, the diagram and the data; the names are
   5  %   the Chaser (silver, drawn mid grey so it shows on white), the Target (gold) and the Obstacle
   6  %   (blue). The arm is drawn in the Chaser's colour. Other keys are returned as they are, in black.
   7  switch upper(key)
   8      case 'RED'
   9          name = 'Chaser';   color = [0.45 0.48 0.52];
  10      case 'BLACK'
  11          name = 'Target';   color = [0.80 0.58 0.05];
  12      case 'BLUE'
  13          name = 'Obstacle'; color = [0 0 1];
  14      case 'ARM'
  15          name = 'Arm';      color = [0.45 0.48 0.52];
  16      otherwise
  17          name = key;        color = [0 0 0];
  18  end
  19  end
```

</details>
<!-- /listing -->
