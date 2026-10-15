# spot.massprops.writeTrackerPositions

**File:** `+spot/+massprops/writeTrackerPositions.m` · **Package:** [[spot.massprops|spot-massprops]]

<!-- source: +spot/+massprops/writeTrackerPositions.m -->

## What it is for

Write the marker calibration ([[trackerPositions|spot-massprops-trackerPositions]]) to `resources/tracker_positions_phasespace.txt` in the project folder.

## How to call it

```matlab
file = spot.massprops.writeTrackerPositions(spot.massprops.compute(s.mass), projectFolder);
```

## Line by line

**Line 4.** The path in `resources/`.

**Lines 5–8.** Open for writing; error if that fails.

**Lines 9–10.** Close on exit; write the text.

<!-- xref -->
- **Used by:** `+spot/+massprops/trackerPositions.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.massprops.compute|spot-massprops-compute]], [[spot.massprops.trackerPositions|spot-massprops-trackerPositions]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+massprops/writeTrackerPositions.m</b> (11 lines; click to show)</summary>

```matlab
   1  function file = writeTrackerPositions(props, root)
   2  %WRITETRACKERPOSITIONS Write resources/tracker_positions_phasespace.txt for the PhaseSpace server.
   3  %   file = spot.massprops.writeTrackerPositions(spot.massprops.compute(s.mass), projectFolder)
   4  file = fullfile(root, 'resources', 'tracker_positions_phasespace.txt');
   5  fid = fopen(file, 'w');
   6  if fid < 0
   7      error('spot:massprops:writeTrackerPositions', 'Cannot write %s.', file);
   8  end
   9  cleanup = onCleanup(@() fclose(fid));
  10  fwrite(fid, spot.massprops.trackerPositions(props), 'char');
  11  end
```

</details>
<!-- /listing -->
