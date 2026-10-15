# spot.massprops.trackerPositions

**File:** `+spot/+massprops/trackerPositions.m` · **Package:** [[spot.massprops|spot-massprops]]

<!-- source: +spot/+massprops/trackerPositions.m -->

## What it is for

The text of `resources/tracker_positions_phasespace.txt`: for each platform, each of its four LEDs' position relative to the CG, in millimetres, in the format the PhaseSpace stream program (StreamData) reads. The Experiment tab sends this file to the lab console before starting the cameras, so PhaseSpace reports each platform's CG.

## How to call it

```matlab
text = spot.massprops.trackerPositions(spot.massprops.compute(s.mass));
```

A line looks like `tracker_id_RED_5_pos_string,137.000000,-12.345678,0`.

## Line by line

**Line 8.** Which LED id sits at which corner, per platform, in the corner order of [[centerOfGravity|spot-massprops-centerOfGravity]]: RED 5, 3, 7, 4; BLACK 13, 11, 9, 15; BLUE 8, 14, 12, 10.

**Lines 9–16.** For each platform: its LED positions in mm, then one line per LED with the platform name, the id, x, y and a z of 0.

**Line 17.** Join the lines with newlines, ending with one.

<!-- xref -->
- **Used by:** `+spot/+massprops/writeTrackerPositions.m`
- **Uses:** [[spot.massprops.centerOfGravity|spot-massprops-centerOfGravity]], [[spot.massprops.compute|spot-massprops-compute]], [[spot.massprops.writeTrackerPositions|spot-massprops-writeTrackerPositions]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+massprops/trackerPositions.m</b> (18 lines; click to show)</summary>

```matlab
   1  function text = trackerPositions(props)
   2  %TRACKERPOSITIONS PhaseSpace marker calibration text from the computed mass properties.
   3  %   text = spot.massprops.trackerPositions(spot.massprops.compute(s.mass)) returns the contents
   4  %   of resources/tracker_positions_phasespace.txt: each LED's position from its platform's CG
   5  %   in mm. StreamData reads this file (the SPOT GUI uploads it to the PhaseSpace server).
   6  %   Write it with spot.massprops.writeTrackerPositions.
   7  % LED tracker ids per platform, in the corner order of spot.massprops.centerOfGravity
   8  ids = struct('RED', [5 3 7 4], 'BLACK', [13 11 9 15], 'BLUE', [8 14 12 10]);
   9  lines = {};
  10  for name = {'RED', 'BLACK', 'BLUE'}
  11      leds = props.(name{1}).leds * 1000;
  12      for k = 1:4
  13          lines{end + 1} = sprintf('tracker_id_%s_%d_pos_string,%f,%f,0', ...
  14              name{1}, ids.(name{1})(k), leds(k, 1), leds(k, 2)); %#ok<AGROW>
  15      end
  16  end
  17  text = [strjoin(lines, newline) newline];
  18  end
```

</details>
<!-- /listing -->
