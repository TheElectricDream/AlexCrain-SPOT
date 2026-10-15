# spot.params.loadCalibration

**File:** `+spot/+params/loadCalibration.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/loadCalibration.m -->

## What it is for

Reads the project's copy of the lab's calibration, `resources/spot_calibration.json`, for the simulation. The lab console measures the lab on its [[Calibration tab|Calibration-Tab]] and keeps the lab's calibration; the local console copies it into the project (the tab's **Update from Lab**, or the prompt when the lab's is newer). [[spot.params.apply|spot-params-apply]] calls this and, with the Scenario's **Table Slope** on (`s.calibration.tableSlope`), puts the table's slope into the workspace (`tableSlope`, `tableDamping_<PLATFORM>`), which [[spot.sim.tableSlope|spot-sim-tableSlope]] turns into the platforms' downhill pull, and, with **Calibrated Thrust** on (`s.calibration.thrusters`), each platform's measured thrusts (`F_thrusters_<PLATFORM>`), which the mixer and so the simulated plant use. The file's format is on [[calibration.py|Console-calibration]].

## How to call it

```matlab
c = spot.params.loadCalibration(projectFolder)
c.table.slope       % [a1 a2 a3 a4 a5 xc yc], or c.table = [] without a table calibration
c.table.damping.RED % the Chaser's air-bearing drag [1/s] (0 if it wasn't measured)
c.thrusters.RED.thrust   % the Chaser's 8 thrusts [N], or c.thrusters.RED = [] without its thruster calibration
c.arm.created            % when the arm calibration was measured, or c.arm = [] without one
```

A project without the file gets `c.file = ''` and no table; a file that can't be used gets `c.error` (the reason) and no table, so simulations still run, on a level table, and `apply` reports the reason as a warning.

## Line by line

**Lines 18–22.** The result's fields, empty; no file: return them.

**Lines 24–28.** Read the JSON; it must say it is a SPOT calibration (`"format": "spot-calibration"`).

**Lines 29–31.** `updated`: when its newest part was measured (ISO 8601, UTC).

**Lines 32–53.** The table section: the height surface's five coefficients and its centre (the table's centre, 1.75 m, 1.2 m, if the file has none), all finite numbers, or an error; the air-bearing drag of each platform it measured (the others 0); then the table calibration for `apply` and the Scenario tab: `slope = [a1 .. a5 xc yc]`, the drag, the model, when, and the tilt it found.

**Lines 54–69.** The thruster section: for each platform it has, eight thrusts of 0.01 to 2 N, or an error; with when, how far each push is turned and its torque ratio (for the page).

**Lines 70–73.** The arm section (calibrate_arm.py's fit, from the Calibration tab): when it was measured and its joints, for the Scenario tab ([[spot.arm.parameters|spot-arm-parameters]] applies it).

**Lines 74–77.** Anything wrong: the reason, and no calibration.

**Lines 80–95.** `numbers`: eight numbers from a JSON list, where `null` (a thruster without a result, which `jsondecode` makes a cell) and a missing list give NaN.

**Lines 97–103.** `field`: a field of the JSON, or a default if it is missing.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/calibration.py`, `+spot/+params/apply.m`, `+spot/+sim/tableSlope.m`
- **Uses:** [[spot.arm.parameters|spot-arm-parameters]], [[spot.params.apply|spot-params-apply]], [[spot.sim.tableSlope|spot-sim-tableSlope]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/loadCalibration.m</b> (103 lines; click to show)</summary>

```matlab
   1  function c = loadCalibration(root)
   2  %LOADCALIBRATION The project's calibration of the lab (resources/spot_calibration.json).
   3  %   c = spot.params.loadCalibration(projectFolder) reads the calibration the lab console measured
   4  %   (its Calibration tab), as the local console copied it into the project. Returns a struct:
   5  %     file      the file ('' if the project has none)
   6  %     updated   when its newest part was measured (ISO 8601, UTC; '' if none)
   7  %     table     the table slope calibration, or [] if none:
   8  %                 slope    [a1 a2 a3 a4 a5 xc yc], the table top's height surface (spot.sim.tableSlope)
   9  %                 damping  struct RED, BLACK, BLUE: the air bearings' drag [1/s] (0 if not measured)
  10  %                 model, updated, tilt_centre_mrad, tilt_max_mrad, downhill_deg
  11  %     thrusters struct RED, BLACK, BLUE: each platform's thruster calibration, or [] if none:
  12  %                 thrust   8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>)
  13  %                 updated, angle (8x1 [deg], how far each thrust is turned), torqueRatio (8x1)
  14  %     arm       the arm calibration (calibrate_arm.py's, from the Calibration tab), or [] if none: its
  15  %               joints' friction and inertia, created, updated, simulated (spot.arm.parameters applies it)
  16  %     error     why the file couldn't be used ('' if it could)
  17  %   spot.params.apply uses it (Scenario: Table Slope); a file that can't be read is reported and
  18  %   ignored, so simulations still run on a level table.
  19  
  20  c = struct('file', '', 'updated', '', 'table', [], 'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], 'error', '');
  21  file = fullfile(root, 'resources', 'spot_calibration.json');
  22  if ~isfile(file)
  23      return
  24  end
  25  c.file = file;
  26  try
  27      x = jsondecode(fileread(file));
  28      if ~isfield(x, 'format') || ~strcmp(x.format, 'spot-calibration')
  29          error('spot:params:loadCalibration', 'not a SPOT calibration file');
  30      end
  31      if isfield(x, 'updated') && ischar(x.updated)
  32          c.updated = x.updated;
  33      end
  34      if isfield(x, 'table') && isstruct(x.table)
  35          t = x.table;
  36          h = double(t.height(:).');
  37          centre = [1.75 1.2];
  38          if isfield(t, 'centre')
  39              centre = double(t.centre(:).');
  40          end
  41          if numel(h) ~= 5 || numel(centre) ~= 2 || ~all(isfinite([h centre]))
  42              error('spot:params:loadCalibration', 'table.height must be 5 numbers and table.centre 2');
  43          end
  44          damping = struct('RED', 0, 'BLACK', 0, 'BLUE', 0);
  45          if isfield(t, 'damping') && isstruct(t.damping)
  46              for n = {'RED', 'BLACK', 'BLUE'}
  47                  if isfield(t.damping, n{1}) && isfinite(t.damping.(n{1}))
  48                      damping.(n{1}) = double(t.damping.(n{1}));
  49                  end
  50              end
  51          end
  52          c.table = struct('slope', [h centre], 'damping', damping, 'model', field(t, 'model', ''), ...
  53              'updated', field(t, 'updated', ''), 'tilt_centre_mrad', field(t, 'tilt_centre_mrad', NaN), ...
  54              'tilt_max_mrad', field(t, 'tilt_max_mrad', NaN), 'downhill_deg', field(t, 'downhill_deg', NaN));
  55      end
  56      if isfield(x, 'thrusters') && isstruct(x.thrusters)
  57          for n = {'RED', 'BLACK', 'BLUE'}
  58              if ~isfield(x.thrusters, n{1})
  59                  continue
  60              end
  61              p = x.thrusters.(n{1});
  62              F = double(p.thrust(:));
  63              if numel(F) ~= 8 || ~all(isfinite(F)) || any(F < 0.01 | F > 2)
  64                  error('spot:params:loadCalibration', 'thrusters.%s.thrust must be 8 thrusts of 0.01 to 2 N', n{1});
  65              end
  66              c.thrusters.(n{1}) = struct('thrust', F, 'updated', field(p, 'updated', ''), ...
  67                  'angle', numbers(field(p, 'angle', [])), 'torqueRatio', numbers(field(p, 'torque_ratio', [])));
  68          end
  69      end
  70      if isfield(x, 'arm') && isstruct(x.arm) && isfield(x.arm, 'format') && strcmp(x.arm.format, 'spot-arm-calibration')
  71          c.arm = struct('created', field(x.arm, 'created', ''), 'updated', field(x.arm, 'updated', ''), ...
  72              'joints', {field(x.arm, 'joints', [])});
  73      end
  74  catch err
  75      c = struct('file', file, 'updated', '', 'table', [], 'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
  76          'error', sprintf('%s could not be used (%s); the simulation ignores it.', file, err.message));
  77  end
  78  end
  79  
  80  function v = numbers(x)
  81  % 8x1 numbers from a JSON list (null, a thruster without a result, and a missing list: NaN)
  82  v = nan(8, 1);
  83  if iscell(x)
  84      x = cellfun(@(e) ifEmpty(e), x);
  85  end
  86  if isnumeric(x) && numel(x) == 8
  87      v = double(x(:));
  88  end
  89  end
  90  
  91  function e = ifEmpty(e)
  92  if isempty(e)
  93      e = NaN;
  94  end
  95  end
  96  
  97  function v = field(s, name, default)
  98  if isfield(s, name)
  99      v = s.(name);
 100  else
 101      v = default;
 102  end
 103  end
```

</details>
<!-- /listing -->
