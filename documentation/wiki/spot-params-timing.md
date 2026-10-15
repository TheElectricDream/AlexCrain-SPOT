# spot.params.timing

**File:** `+spot/+params/timing.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/timing.m -->

## What it is for

Turn the durations typed on the Scenario pane into the numbers the diagram keeps time with: how long each phase lasts, when each one ends, and the total. Segments switched off are given zero length so the diagram skips them without any other change: the diagram picks the current phase with half-open windows, $\text{Phase}_{k-1}\_\text{End} \le t < \text{Phase}_k\_\text{End}$, and a window of zero width is never entered.

Phase 3's own clock (time since `Phase2_End`) starts with the first sub-phase that runs.

## How to call it

```matlab
t = spot.params.timing(s);
t.tsim                       % total time [s]
t.Phase3_SubPhase2_End       % when sub-phase 3.2 ends [s]
t.phases                     % the segments that run: names and end times (for the playback)
t.segments                   % all nine, with duration (as typed), run (on/off), start, end
```

## The arithmetic

With durations $d_0 \dots d_5$ (where $d_3 = \sum_i s_i$ over the sub-phases that are on, and $d_2$, $d_4$ are 0 when off):

$$\text{Phase}_k\_\text{End} = \sum_{j=0}^{k} d_j, \qquad t_{sim} = \text{Phase}_5\_\text{End}, \qquad \text{SubPhase}_i\_\text{End} = \text{Phase}_2\_\text{End} + \sum_{j=1}^{i} s_j .$$

## Line by line

**Lines 16–17.** Take the `timing` section and the four sub-phase durations as a row.

**Lines 18–23.** Two checks: exactly four sub-phases, and no negative duration.

**Line 24.** The on/off switches through the local `switches` (lines 51–61), which supplies "all on" for files without them.

**Lines 25–26.** Zero out the sub-phases that are off (`sub .* on.subPhases`), then build the six phase durations: phases 0 and 1 as typed, phase 2 times its switch, phase 3 as the sum of the (switched) sub-phases, phase 4 times its switch, phase 5 as typed.

**Line 27.** `cumsum` gives each phase's end time.

**Lines 28–32.** Store `Phase0_Duration … Phase5_Duration` and `Phase0_End … Phase5_End` with the names the diagram expects.

**Lines 33–37.** The sub-phases' end times are phase 2's end plus the running sum of the sub-phase durations; stored as `Phase3_SubPhase1_Duration/_End` etc.

**Line 38.** The total is the last end time.

**Lines 40–44.** `t.phases`: the names and ends of the segments that actually run (duration > 0), which the 3D playback uses to mark phases. The nine segments are phases 0, 1, 2, the four sub-phases, 4 and 5.

**Lines 45–48.** `t.segments`: all nine segments for the Scenario pane's timeline bar, each with its typed duration, whether it runs, and its start (`end − duration`) and end as they will run.

**Lines 51–61.** `switches` reads `tm.on` defensively: everything on by default; `phase2` and `phase4` taken if present; `subPhases` only if it has exactly four entries, made logical and a row.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/sim.js`, `+spot/+data/trajectory.m`, `+spot/+experiment/plan.m`, `+spot/+params/apply.m`, `+spot/+params/defaults.m`, `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/timing.m</b> (61 lines; click to show)</summary>

```matlab
   1  function t = timing(s)
   2  %TIMING Phase durations, end times and total time from the settings.
   3  %   t = spot.params.timing(s) returns a struct with Phase0_Duration ... Phase5_Duration,
   4  %   Phase3_SubPhase1_Duration ... _4, the matching *_End times and tsim, named as the
   5  %   Simulink diagrams expect them in the workspace.
   6  %
   7  %   Segments switched off in s.timing.on (phase 2, the four phase 3 sub-phases, phase 4) count as
   8  %   zero seconds, so they are skipped: the diagrams pick the phase with half-open windows
   9  %   (PhaseK-1_End <= t < PhaseK_End), and a window of zero length is never entered. The durations
  10  %   typed in the settings are kept. Phases 0, 1 and 5 always run (they start and stop the
  11  %   platforms). Phase 3's clock (time since Phase2_End) starts with the first sub-phase that runs.
  12  %
  13  %   t.phases lists the segments that run: names and end times, for the playback's markers.
  14  %   t.segments lists all nine segments for the console's timeline: name, duration (as typed),
  15  %   run (on or skipped), start and end [s] as they will run.
  16  tm = s.timing;
  17  sub = tm.subPhases(:).';
  18  if numel(sub) ~= 4
  19      error('spot:params:timing', 'Phase 3 needs exactly four sub-phase durations.');
  20  end
  21  if any([tm.phase0, tm.phase1, tm.phase2, tm.phase4, tm.phase5] < 0) || any(sub < 0)
  22      error('spot:params:timing', 'Phase durations cannot be negative.');
  23  end
  24  on = switches(tm);
  25  sub = sub .* on.subPhases;
  26  d = [tm.phase0, tm.phase1, tm.phase2 * on.phase2, sum(sub), tm.phase4 * on.phase4, tm.phase5];
  27  ends = cumsum(d);
  28  t = struct();
  29  for k = 0:5
  30      t.(sprintf('Phase%d_Duration', k)) = d(k + 1);
  31      t.(sprintf('Phase%d_End', k)) = ends(k + 1);
  32  end
  33  subEnds = ends(3) + cumsum(sub);
  34  for k = 1:4
  35      t.(sprintf('Phase3_SubPhase%d_Duration', k)) = sub(k);
  36      t.(sprintf('Phase3_SubPhase%d_End', k)) = subEnds(k);
  37  end
  38  t.tsim = ends(end);
  39  
  40  names = {'Phase 0', 'Phase 1', 'Phase 2', 'Phase 3.1', 'Phase 3.2', 'Phase 3.3', 'Phase 3.4', 'Phase 4', 'Phase 5'};
  41  segEnds = [ends(1:3), subEnds, ends(5:6)];
  42  segDur = [d(1:3), sub, d(5:6)];
  43  keep = segDur > 0;
  44  t.phases = struct('names', {names(keep)}, 'ends', segEnds(keep));
  45  typed = [tm.phase0, tm.phase1, tm.phase2, tm.subPhases(:).', tm.phase4, tm.phase5];
  46  runs = [true, true, on.phase2, on.subPhases, on.phase4, true];
  47  t.segments = struct('name', names, 'duration', num2cell(typed), 'run', num2cell(runs), ...
  48                      'start', num2cell(segEnds - segDur), 'end', num2cell(segEnds));
  49  end
  50  
  51  function on = switches(tm)
  52  % The on/off switches, all on when the settings don't have them (older settings files)
  53  on = struct('phase2', true, 'subPhases', true(1, 4), 'phase4', true);
  54  if isfield(tm, 'on') && isstruct(tm.on)
  55      if isfield(tm.on, 'phase2'), on.phase2 = logical(tm.on.phase2); end
  56      if isfield(tm.on, 'phase4'), on.phase4 = logical(tm.on.phase4); end
  57      if isfield(tm.on, 'subPhases') && numel(tm.on.subPhases) == 4
  58          on.subPhases = logical(tm.on.subPhases(:).');
  59      end
  60  end
  61  end
```

</details>
<!-- /listing -->
