# spot.arm.parameters

**File:** `+spot/+arm/parameters.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/parameters.m -->

## What it is for

Build `armModel`, the workspace variable the *RED + ARM Plant* block takes: the [[defaults|spot-arm-defaults]] completed with what [[spot.params.apply|spot-params-apply]] computed (the Chaser's mass and inertia, the arm's geometry and masses for its mount, the initial state, the table's slope, the diagram's step) and, when the project has one, the arm calibration: the lab's (the `arm` section of `resources/spot_calibration.json`, from the [[Calibration tab|Calibration-Tab]]) or `resources/arm_calibration.json` ([[calibrate_arm.py|Tool-calibrate-arm]]'s own file, copied by hand), whichever was measured last. Change it in `SPOT_User_Parameters.m`, e.g. `armModel.friction.static = [0.4 0.3 0.2];`.

## Line by line

**Line 14.** Start from the defaults.

**Lines 15–17.** Copy the sixteen dynamic parameters from the workspace values `v`.

**Lines 18–21.** The shoulder's angular offset for its mount face (0 on the default mount).

**Line 22.** The initial state: the Chaser's `[x y θ]` and the arm's three angles.

**Lines 23–30.** The table under the Chaser: `a.tableSlope` (`[a1 … a5 xc yc]`, [[spot.sim.tableSlope|spot-sim-tableSlope]]) and `a.tableDamping` (the air bearings' drag [1/s]) from `v.tableSlope` and `v.tableDamping_RED` when [[apply|spot-params-apply]] set them; otherwise level (zeros, centre [1.75 1.2] m) and no drag.

**Line 31.** The outer step is the diagram's `baseRate`.

**Lines 32–54.** No calibration by default (`a.calibration` empty). With a project folder: read when each arm calibration was measured (`created`, the script's time), from `resources/spot_calibration.json`'s `arm` section and from `resources/arm_calibration.json` (a file that is missing, unreadable or not a `spot-arm-calibration` counts as none); apply the newer with [[loadCalibration|spot-arm-loadCalibration]] (the lab's on a tie; ISO times compare in order as text), which sets `a.calibration` to its file (`…/spot_calibration.json (arm)` for the lab's).

<!-- xref -->
- **Used by:** `+spot/+arm/Plant.m`, `+spot/+arm/defaults.m`, `+spot/+arm/loadCalibration.m`, `+spot/+console/webapp/static/calibration.js`, `+spot/+params/apply.m`, `+spot/+params/defaults.m`, `+spot/+params/loadCalibration.m`, `resources/arm_calibration/calibrate_arm.py`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.defaults|spot-arm-defaults]], [[spot.arm.loadCalibration|spot-arm-loadCalibration]], [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.params.apply|spot-params-apply]], [[spot.sim.tableSlope|spot-sim-tableSlope]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/parameters.m</b> (55 lines; click to show)</summary>

```matlab
   1  function a = parameters(v, root)
   2  %PARAMETERS The arm model spot.arm.Plant simulates (the workspace variable armModel).
   3  %   a = spot.arm.parameters(v) completes spot.arm.defaults with the values spot.params.apply
   4  %   puts in the workspace (v: mRED, IRED, the arm's geometry and masses, init_states_RED,
   5  %   init_states_ARM, baseRate).
   6  %   a = spot.arm.parameters(v, projectFolder) also applies the project's arm calibration
   7  %   (spot.arm.loadCalibration): the lab's (resources/spot_calibration.json's arm section, from the
   8  %   Calibration tab) or resources/arm_calibration.json (calibrate_arm.py's own file, copied by hand),
   9  %   whichever was measured last; a.calibration names it. The table's slope and the air
  10  %   bearings' drag under the Chaser come from v.tableSlope and v.tableDamping_RED when there
  11  %   (spot.params.apply; spot.sim.tableSlope), else the table is level.
  12  %
  13  %   Change it in SPOT_User_Parameters.m, e.g. armModel.friction.static = [0.4 0.3 0.2];
  14  a = spot.arm.defaults();
  15  for f = {'mRED', 'IRED', 'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', 'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
  16      a.(f{1}) = v.(f{1});
  17  end
  18  a.q1Offset = 0;                     % the arm turned with its mount face (spot.arm.mountGeometry)
  19  if isfield(v, 'armShoulderOffset')
  20      a.q1Offset = v.armShoulderOffset;
  21  end
  22  a.x0 = [v.init_states_RED(:); v.init_states_ARM(:)];
  23  a.tableSlope = [0 0 0 0 0 1.75 1.2];
  24  a.tableDamping = 0;
  25  if isfield(v, 'tableSlope')
  26      a.tableSlope = v.tableSlope;
  27  end
  28  if isfield(v, 'tableDamping_RED')
  29      a.tableDamping = v.tableDamping_RED;
  30  end
  31  a.outerStep = v.baseRate;
  32  a.calibration = '';
  33  if nargin > 1 && ~isempty(root)
  34      % The arm calibrations the project has, and when each was measured ('created': the script's time)
  35      files = {fullfile(root, 'resources', 'spot_calibration.json'), fullfile(root, 'resources', 'arm_calibration.json')};
  36      created = {'', ''};
  37      for k = 1:2
  38          try
  39              c = jsondecode(fileread(files{k}));
  40              if k == 1
  41                  c = c.arm;
  42              end
  43              if strcmp(c.format, 'spot-arm-calibration')
  44                  created{k} = char(c.created);
  45              end
  46          catch
  47          end
  48      end
  49      if ~isempty(created{1}) && string(created{1}) >= string(created{2})   % (ISO times: as text, in order)
  50          a = spot.arm.loadCalibration(files{1}, a);
  51      elseif ~isempty(created{2})
  52          a = spot.arm.loadCalibration(files{2}, a);
  53      end
  54  end
  55  end
```

</details>
<!-- /listing -->
