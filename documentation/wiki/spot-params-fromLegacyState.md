# spot.params.fromLegacyState

**File:** `+spot/+params/fromLegacyState.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/fromLegacyState.m -->

## What it is for

The old App Designer GUI saved its state (*Save GUI State*) as a `.mat` file holding three structs whose fields are the names of its edit boxes and check boxes (`REDCheckBox`, `DurPhase2EditField`, `ARedScaleMeasurement`, …). This reads such a file and fills a settings struct from it, so old work can be loaded into the console. Anything the file lacks keeps its default.

## How to call it

```matlab
s = spot.params.fromLegacyState('gui_state.mat');    % or spot.params.load, which calls this for .mat
```

## Line by line

**Lines 6–9.** Load the `.mat`; without an `appState` struct it isn't a GUI state file.

**Lines 10–13.** `a` = the main window's state, `m` = the mass-properties window's, `ic` = the initial-conditions window's (the latter two may be missing: `getfield_or`, lines 103–105, returns an empty struct then). Start from the defaults.

**Lines 15–23.** Platforms from the four check boxes (the helper `num`, lines 107–115, returns the field as a double if it is a numeric or logical scalar, else the default; `~= 0` makes it logical). The diagram from the drop-down, kept only if it ends in `.slx`.

**Lines 25–36.** Sample rate and the phase durations, field by field, into a copy of `s.timing`.

**Lines 38–50.** The solver: type and the two solver names from drop-downs (`txt`, lines 117–127, returns text, converting a number if needed); the step sizes and tolerances only when the box wasn't empty.

**Lines 52–65.** The LQR weights per platform: the GUI named them `RedQXEditField`, `BlackRFYEditField`, … so a prefix table maps `RED` → `Red`, and the six Q and three R names are read in order.

**Lines 67–76.** Initial conditions. The GUI's "Start" fields are the initial pose (its "Initial" fields were the drop pose the diagrams no longer have), its "Home" fields the home pose, and `ARMInitialQ1..3` the arm's angles.

**Lines 78–100.** Mass properties: the override check box; per platform the three scale readings (`A<NAME>ScaleMeasurement` …), the bifilar `T`, `L` (whose field name is irregular, hence the `lname` table) and `D`, and the typed mass and inertia (`MassRedEditField` …). The arm's fourteen parameters through a two-column name table.

**Lines 103–127.** The three helpers described above.

<!-- xref -->
- **Used by:** `+spot/+params/load.m`
- **Uses:** [[spot.params.defaults|spot-params-defaults]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/fromLegacyState.m</b> (127 lines; click to show)</summary>

```matlab
   1  function s = fromLegacyState(file)
   2  %FROMLEGACYSTATE Convert a GUI state file saved by the old SPOT GUI (Save GUI State) to settings.
   3  %   s = spot.params.fromLegacyState(file) reads the appState, subAppState (mass properties) and
   4  %   subAppStateInit (initial conditions) structs, whose fields are App Designer component names,
   5  %   and returns a settings struct. Anything not in the file keeps its default.
   6  L = load(file);
   7  if ~isfield(L, 'appState')
   8      error('spot:params:fromLegacyState', '%s is not a SPOT GUI state file (no appState).', file);
   9  end
  10  a = L.appState;
  11  m = getfield_or(L, 'subAppState', struct());
  12  ic = getfield_or(L, 'subAppStateInit', struct());
  13  s = spot.params.defaults();
  14  
  15  % Platforms and diagram
  16  s.platforms.RED = num(a, 'REDCheckBox', s.platforms.RED) ~= 0;
  17  s.platforms.BLACK = num(a, 'BLACKCheckBox', s.platforms.BLACK) ~= 0;
  18  s.platforms.BLUE = num(a, 'BLUECheckBox', s.platforms.BLUE) ~= 0;
  19  s.platforms.ARM = num(a, 'ARMCheckBox', s.platforms.ARM) ~= 0;
  20  diagram = txt(a, 'AvailableDiagramsDropDown', '');
  21  if endsWith(diagram, '.slx')
  22      s.diagram = diagram;
  23  end
  24  
  25  % Timing and rate
  26  s.sampleRate = num(a, 'SampleRateEditField', s.sampleRate);
  27  tm = s.timing;
  28  tm.phase0 = num(a, 'DurPhase0EditField', tm.phase0);
  29  tm.phase1 = num(a, 'DurPhase1EditField', tm.phase1);
  30  tm.phase2 = num(a, 'DurPhase2EditField', tm.phase2);
  31  for k = 1:4
  32      tm.subPhases(k) = num(a, sprintf('SubPhase%dEditField', k), tm.subPhases(k));
  33  end
  34  tm.phase4 = num(a, 'DurPhase4EditField', tm.phase4);
  35  tm.phase5 = num(a, 'DurPhase5EditField', tm.phase5);
  36  s.timing = tm;
  37  
  38  % Solver
  39  s.solver.type = txt(a, 'SolverTypeDropDown', s.solver.type);
  40  s.solver.fixedStepName = txt(a, 'SolverNameDropDown', s.solver.fixedStepName);
  41  s.solver.variableStepName = txt(a, 'SolverNameVarStepDropDown', s.solver.variableStepName);
  42  pairs = {'maxStep', 'VarStepMaxStepSizeEditField'; 'minStep', 'VarStepMinStepSizeEditField';
  43           'initialStep', 'VarStepInitStepSizeEditField'; 'relTol', 'VarStepRelTolEditField';
  44           'absTol', 'VarStepAbsTolEditField'};
  45  for k = 1:size(pairs, 1)
  46      value = txt(a, pairs{k, 2}, '');
  47      if ~isempty(value)
  48          s.solver.(pairs{k, 1}) = value;
  49      end
  50  end
  51  
  52  % Controllers
  53  prefix = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
  54  for name = {'RED', 'BLACK', 'BLUE'}
  55      n = name{1}; p = prefix.(n); c = s.control.(n);
  56      q = {'QX', 'QY', 'QTheta', 'QXD', 'QYD', 'QThetaD'};
  57      for k = 1:6
  58          c.lqr.Q(k) = num(a, [p q{k} 'EditField'], c.lqr.Q(k));
  59      end
  60      r = {'RFX', 'RFY', 'RTz'};
  61      for k = 1:3
  62          c.lqr.R(k) = num(a, [p r{k} 'EditField'], c.lqr.R(k));
  63      end
  64      s.control.(n) = c;
  65  end
  66  
  67  % Initial conditions: the GUI's "Start" fields are the initial pose (its "Initial" fields were
  68  % the drop pose, which the diagrams no longer have); its arm angles are the arm's initial pose
  69  for name = {'RED', 'BLACK', 'BLUE'}
  70      n = name{1}; c = s.initial.(n);
  71      c.initial = [num(ic, [n 'StartX'], c.initial(1)), num(ic, [n 'StartY'], c.initial(2)), num(ic, [n 'StartTh'], c.initial(3))];
  72      c.home = [num(ic, [n 'HomeX'], c.home(1)), num(ic, [n 'HomeY'], c.home(2)), num(ic, [n 'HomeTh'], c.home(3))];
  73      s.initial.(n) = c;
  74  end
  75  s.initial.ARM.initial = [num(ic, 'ARMInitialQ1', s.initial.ARM.initial(1)), ...
  76                           num(ic, 'ARMInitialQ2', s.initial.ARM.initial(2)), num(ic, 'ARMInitialQ3', s.initial.ARM.initial(3))];
  77  
  78  % Mass properties
  79  s.mass.override = num(m, 'OverridePropertiesCheckBox', s.mass.override) ~= 0;
  80  field = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
  81  lname = struct('RED', 'LREDScaleMeasurement', 'BLACK', 'LBLACKMeasurement', 'BLUE', 'LBLUEMeasurement');
  82  for name = {'RED', 'BLACK', 'BLUE'}
  83      n = name{1}; c = s.mass.(n);
  84      c.scales = [num(m, ['A' n 'ScaleMeasurement'], c.scales(1)), num(m, ['B' n 'ScaleMeasurement'], c.scales(2)), ...
  85                  num(m, ['C' n 'ScaleMeasurement'], c.scales(3))];
  86      c.bifilar.T = num(m, ['T' n 'Measurement'], c.bifilar.T);
  87      c.bifilar.L = num(m, lname.(n), c.bifilar.L);
  88      c.bifilar.D = num(m, ['D' n 'Measurement'], c.bifilar.D);
  89      c.mass = num(m, ['Mass' field.(n) 'EditField'], c.mass);
  90      c.inertia = num(m, ['Inertia' field.(n) 'EditField'], c.inertia);
  91      s.mass.(n) = c;
  92  end
  93  armFields = {'Phi', 'ArmPhiEditField'; 'b0', 'ArmB0EditField'; 'a1', 'ArmA1EditField'; 'b1', 'ArmB1EditField';
  94               'a2', 'ArmA2EditField'; 'b2', 'ArmB2EditField'; 'a3', 'ArmA3EditField'; 'b3', 'ArmB3EditField';
  95               'mPROX', 'MassProximalLinkEditField'; 'mDIST', 'MassDistalLinkEditField';
  96               'mEE', 'MassEndEffectorLinkEditField'; 'IPROX', 'InertiaProximalLinkEditField';
  97               'IDIST', 'InertiaDistalLinkEditField'; 'IEE', 'InertiaEndEffectorLinkEditField'};
  98  for k = 1:size(armFields, 1)
  99      s.mass.arm.(armFields{k, 1}) = num(m, armFields{k, 2}, s.mass.arm.(armFields{k, 1}));
 100  end
 101  end
 102  
 103  function v = getfield_or(st, name, default)
 104  if isfield(st, name), v = st.(name); else, v = default; end
 105  end
 106  
 107  function v = num(st, name, default)
 108  v = default;
 109  if isfield(st, name)
 110      x = st.(name);
 111      if (isnumeric(x) || islogical(x)) && isscalar(x)
 112          v = double(x);
 113      end
 114  end
 115  end
 116  
 117  function v = txt(st, name, default)
 118  v = default;
 119  if isfield(st, name)
 120      x = st.(name);
 121      if ischar(x) || (isstring(x) && isscalar(x))
 122          v = char(x);
 123      elseif isnumeric(x) && isscalar(x)
 124          v = num2str(x);
 125      end
 126  end
 127  end
```

</details>
<!-- /listing -->
