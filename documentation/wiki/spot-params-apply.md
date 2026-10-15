# spot.params.apply

**File:** `+spot/+params/apply.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/apply.m -->

## What it is for

The bridge between the settings and the Simulink diagram. The diagram reads named variables from MATLAB's base workspace ([[Workspace Variables|Workspace-Variables]]); this function computes every one of them from `s` and assigns them, then runs your `SPOT_User_Parameters.m` so you can add to or override them. It also returns the derived values the page displays (total time, centre of gravity, LQR gains, the calibration used, warnings).

It runs on every settings change (so the Workspace pane and the Control pane's gains are always current) and at the start of every simulation.

## How to call it

```matlab
d = spot.params.apply(s);          % variables only
d = spot.params.apply(s, pwd);     % and use the project's calibration, run <pwd>/SPOT_User_Parameters.m afterwards
d.tsim, d.mass.RED.cg, d.lqr.RED, d.warnings
```

## Line by line

**Lines 18–19.** `d` collects what is returned; `v` collects name → value pairs, all assigned to the base workspace in one go at the end (line 194), so a failure half-way leaves the workspace untouched.

**Lines 21–23.** The platforms as the diagram's single code ([[platformSelection|spot-params-platformSelection]]); an unsupported combination errors here.

**Lines 25–33.** The phase timing ([[timing|spot-params-timing]]): every field except the two lists meant for the console (`phases`, `segments`) becomes a variable (`Phase0_End`, `tsim`, …).

**Lines 35–46.** Rates. The sample rate must be positive; the data rate a whole number from 1 to 960 (the PhaseSpace server's limits). The workspace gets `baseRate = 1/sampleRate` and `dataRate = 1/dataRate` (both in seconds), and `d.cameraRate` keeps the Hz value for the Experiment tab.

**Lines 48–59.** The PWM frequency must be positive and becomes `PWMFreq`. If it isn't 5 Hz a warning text is prepared (the Teensy firmware's constant is 5 Hz; the two must agree or the duty cycles don't mean what the diagram thinks). The lab console's address becomes the four-number `labConsoleIP` ([[ipv4|spot-params-ipv4]]).

**Lines 61–63.** The table's size, constants the diagram uses for wall checks.

**Lines 65–73.** Mass properties ([[spot.massprops.compute|spot-massprops-compute]]): per platform, mass `mRED`, inertia `IRED` and the eight thruster offsets `thruster_dist2CG_RED`.

**Lines 74–81.** The arm's geometry for where it is mounted ([[spot.arm.mountGeometry|spot-arm-mountGeometry]]): the fourteen parameters `Phi … IEE`, plus `armShoulderOffset`, the angle the shoulder's zero is turned by for its face.

**Lines 83–110.** The lab's calibration ([[Calibration Tab|Calibration-Tab]]). By default the table is level: `tableSlope = [0 0 0 0 0 1.75 1.2]` (the height surface's five coefficients, all zero, and the table's centre [m]; see [[spot.sim.tableSlope|spot-sim-tableSlope]]) and `tableDamping_RED`, `tableDamping_BLACK`, `tableDamping_BLUE` (the air bearings' drag [1/s]) are 0. With the project folder, [[spot.params.loadCalibration|spot-params-loadCalibration]] reads `resources/spot_calibration.json`; `d.calibration` (`file`, `updated`, `table`, `tableUsed`, `thrusters`, `thrustersUsed`, `arm`, `armUsed`, `error`) is what the Scenario's Table Slope, Calibrated Thrust and Calibrated Arm switches show; `thrusters` has the file's thruster calibration per platform (empty for a platform without one) and `thrustersUsed` is filled in under Thruster forces below; `arm` is the file's arm calibration (empty without one) and `armUsed` is filled in under `armModel` below. A file that can't be used becomes the first warning (kept in `d.warnings0` until line 157). If the file has a table slope calibration and `s.calibration.tableSlope` is on, its `slope` and per-platform `damping` replace the level values and `tableUsed` is true.

**Lines 112–120.** Initial conditions: the local `pose` (lines 237–240) converts `[x y θ°]` to `[x y θ rad]` for `init_states_*` and `home_states_*`; the arm's three angles to radians for `init_states_ARM`. `d.armInitial` is the arm's joint positions on the table at that pose, for the 3D view's marker.

**Lines 122–130.** `armModel`, the simulated arm ([[spot.arm.parameters|spot-arm-parameters]]): with the project folder and `s.calibration.arm` on, it also applies the project's arm calibration (the `arm` section of `resources/spot_calibration.json` or `resources/arm_calibration.json`, whichever was measured last); otherwise the defaults. `d.calibration.armUsed` is the file it came from (`armModel.calibration`), or `''`. It also takes the table's slope and RED's drag from `v`.

**Lines 132–133.** The arm controllers' gains and limits, checked by the local `armGains` (lines 213–235), as the struct `armControl`, which the diagram's *Default Position Control (ARM)* and *Default Velocity Control (ARM)* blocks read (`armControl.position.P`, …).

**Lines 135–150.** Thruster forces: the 8×1 `F_thrusters_RED`, each thruster's thrust, which the diagram's mixer (and, through it, the simulated plant) uses. When `s.calibration.thrusters` is on and the project's calibration has a thruster calibration for that platform, it is that calibration's eight thrusts ([[spot.params.loadCalibration|spot-params-loadCalibration]]) and the platform's name is added to `d.calibration.thrustersUsed`; otherwise it is [[spot.control.nominalForces|spot-control-nominalForces]] of `s.thrusterForce.RED`, the same thrust for all eight. `F_red_X_nominal` and `F_red_Y_nominal` (lower-case colour as the diagram's blocks were written) are always the setting's x and y thrust; the diagram no longer reads them, they are kept for users' own code.

**Lines 152–190.** The controller. `d.warnings` starts with the calibration's warning, if any, and the PWM warning (lines 156–163). The six Q and three R weights become scalars `Qx_RED … RTz_RED`. If the Control System Toolbox is there (`license` and `dlqr` found), the discrete model at `dataRate` ([[spot.control.stateSpace|spot-control-stateSpace]]) gives `A_d_RED … D_d_RED`, and the gain `K_RED` comes from [[spot.control.lqrGains|spot-control-lqrGains]]; a failure (e.g. a zero weight that makes the problem singular) becomes a warning rather than an error, so the page still works. Without the toolbox, one warning says the gains were not computed.

**Lines 192–196.** Assign every collected variable to the base workspace; `d.variables` lists their names.

**Lines 198–205.** With a project folder: if `SPOT_User_Parameters.m` exists, run it **in the base workspace** (`evalin('base', 'run(...)')`), so whatever it defines lands next to the variables above and can override them. Quotes in the path are doubled for the string.

**Lines 207–210.** `armMotor`: the arm's controller blocks need the motor constants but can't take `armModel` as a whole (its field `friction.static` would become a C++ identifier `static` in generated code), so the motor sub-struct is copied out, after the user script in case it changed `armModel`.

**Lines 213–235, `armGains`.** Each gain (position P, I, D; velocity P, I) must be numeric, one value or three, finite and within the servos' register range 0–16383, and is stored as three values `[shoulder elbow wrist]` (a single value applies to all three). The profile velocity and accelerations (0–32767) and the PWM limits (0–885, full voltage) must be single values in range. Anything else is an error naming the setting, which the console shows under the Control pane.

**Lines 237–240.** `pose`: degrees to radians for the third element.

<!-- xref -->
- **Used by:** `+spot/+arm/parameters.m`, `+spot/+console/webapp/spot_console/calibration.py`, `+spot/+console/webapp/spot_console/control.py`, `+spot/+experiment/build.m`, `+spot/+params/defaults.m`, `+spot/+params/loadCalibration.m`, `+spot/+sim/results.m`, `+spot/+sim/run.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.arm.Plant|spot-arm-Plant]], [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.arm.parameters|spot-arm-parameters]], [[spot.control.lqrGains|spot-control-lqrGains]], [[spot.control.nominalForces|spot-control-nominalForces]], [[spot.control.stateSpace|spot-control-stateSpace]], [[spot.massprops.compute|spot-massprops-compute]], [[spot.params.ipv4|spot-params-ipv4]], [[spot.params.load|spot-params-load]], [[spot.params.loadCalibration|spot-params-loadCalibration]], [[spot.params.platformSelection|spot-params-platformSelection]], [[spot.params.timing|spot-params-timing]], [[spot.plotting.armPosition|spot-plotting-armPosition]], [[spot.sim.run|spot-sim-run]], [[spot.sim.tableSlope|spot-sim-tableSlope]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/apply.m</b> (240 lines; click to show)</summary>

```matlab
   1  function d = apply(s, root)
   2  %APPLY Put the settings into the base workspace, as the Simulink diagrams expect them.
   3  %   d = spot.params.apply(s) assigns every workspace variable the old GUI assigned (phase times,
   4  %   rates, gains, LQR matrices, mass properties, initial conditions, thruster forces, ...), and
   5  %   armModel (spot.arm.parameters, for the simulated arm), and
   6  %   returns the derived values in d (for display, e.g. tsim, the CG and the LQR gains).
   7  %   d = spot.params.apply(s, projectFolder) also uses the project's calibration of the lab
   8  %   (resources/spot_calibration.json, spot.params.loadCalibration: the table's slope, with
   9  %   s.calibration.tableSlope, each thruster's thrust, F_thrusters_<PLATFORM>, with
  10  %   s.calibration.thrusters, and the arm's friction and inertia, with s.calibration.arm), and runs projectFolder/SPOT_User_Parameters.m afterwards, where users
  11  %   define their own variables or override these.
  12  %
  13  %   Example (no GUI needed):
  14  %     s = spot.params.load('spot_settings.json');
  15  %     s.platforms.RED = true;
  16  %     spot.params.apply(s, pwd);
  17  %     out = spot.sim.run(s, pwd);
  18  d = struct();
  19  v = struct();   % name -> value, assigned to the base workspace at the end
  20  
  21  % Platforms
  22  d.platformSelection = spot.params.platformSelection(s.platforms);
  23  v.platformSelection = d.platformSelection;
  24  
  25  % Phase timing
  26  t = spot.params.timing(s);
  27  for f = fieldnames(t).'
  28      if ~any(strcmp(f{1}, {'phases', 'segments'}))   % (lists for the console, not diagram variables)
  29          v.(f{1}) = t.(f{1});
  30      end
  31  end
  32  d.timing = t;
  33  d.tsim = t.tsim;
  34  
  35  % Rates
  36  if ~(s.sampleRate > 0)
  37      error('spot:params:apply', 'The sample rate must be above 0 Hz.');
  38  end
  39  if ~(isscalar(s.dataRate) && s.dataRate >= 1 && s.dataRate <= 960 && s.dataRate == round(s.dataRate))
  40      error('spot:params:apply', 'The PhaseSpace data rate must be a whole number of Hz, 1 to 960.');
  41  end
  42  v.baseRate = 1 / s.sampleRate;
  43  v.dataRate = 1 / s.dataRate;            % [s] the setting is in Hz
  44  d.baseRate = v.baseRate;
  45  d.dataRate = v.dataRate;
  46  d.cameraRate = s.dataRate;              % PhaseSpace stream rate the Experiment tab starts [Hz]
  47  
  48  % Thrusters' PWM frequency, and the lab console's address (for the diagram's UDP blocks)
  49  if ~(isscalar(s.pwmFrequency) && s.pwmFrequency > 0)
  50      error('spot:params:apply', 'The thrusters'' PWM frequency must be above 0 Hz.');
  51  end
  52  v.PWMFreq = s.pwmFrequency;
  53  pwmWarning = '';
  54  if s.pwmFrequency ~= 5
  55      pwmWarning = sprintf(['The thrusters'' PWM frequency is %g Hz, but the Teensy firmware runs its PWM at ' ...
  56          '5 Hz (PWM_FREQUENCY in +spot/+hardware/teensy/teensy_pwm_control.ino): flash it with the same value, or ' ...
  57          'the duty cycles won''t mean what the diagram assumes.'], s.pwmFrequency);
  58  end
  59  v.labConsoleIP = spot.params.ipv4(s.labConsoleAddress);
  60  
  61  % Table size, and the simulation flag (spot.sim.run sets simMode = 1 while simulating)
  62  v.xLength = 3.51155;   % [m]
  63  v.yLength = 2.41935;   % [m]
  64  
  65  % Mass properties
  66  mp = spot.massprops.compute(s.mass);
  67  d.mass = mp;
  68  for name = {'RED', 'BLACK', 'BLUE'}
  69      n = name{1};
  70      v.(['m' n]) = mp.(n).mass;
  71      v.(['I' n]) = mp.(n).inertia;
  72      v.(['thruster_dist2CG_' n]) = mp.(n).thrusters;
  73  end
  74  % The arm, where it is mounted (s.mass.arm.mount): Phi and b0 for that place, and the angle it is
  75  % turned by (armShoulderOffset: the shoulder's 0 is straight out from its face)
  76  arm = spot.arm.mountGeometry(s.mass.arm);
  77  for f = {'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', 'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
  78      v.(f{1}) = arm.(f{1});
  79  end
  80  v.armShoulderOffset = arm.q1Offset;
  81  d.armGeometry = arm;
  82  
  83  % The lab's calibration (the Calibration tab; resources/spot_calibration.json): the table's slope
  84  % pulls the floating platforms, and their air bearings drag them (tableSlope = [a1 .. a5 xc yc],
  85  % spot.sim.tableSlope; zeros: a level table). Without the project folder, or with the setting off,
  86  % the table is level.
  87  v.tableSlope = [0 0 0 0 0 1.75 1.2];
  88  for name = {'RED', 'BLACK', 'BLUE'}
  89      v.(['tableDamping_' name{1}]) = 0;
  90  end
  91  none = struct('RED', [], 'BLACK', [], 'BLUE', []);
  92  cal = struct('file', '', 'updated', '', 'table', [], 'thrusters', none, 'error', '');
  93  d.calibration = struct('file', '', 'updated', '', 'table', [], 'tableUsed', false, 'thrusters', none, ...
  94      'thrustersUsed', {{}}, 'arm', [], 'armUsed', '', 'error', '');
  95  if nargin > 1
  96      cal = spot.params.loadCalibration(root);
  97      d.calibration = struct('file', cal.file, 'updated', cal.updated, 'table', cal.table, ...
  98          'tableUsed', false, 'thrusters', cal.thrusters, 'thrustersUsed', {{}}, 'arm', cal.arm, 'armUsed', '', ...
  99          'error', cal.error);
 100      if ~isempty(cal.error)
 101          d.warnings0 = {cal.error};
 102      end
 103      if ~isempty(cal.table) && s.calibration.tableSlope
 104          v.tableSlope = cal.table.slope;
 105          for name = {'RED', 'BLACK', 'BLUE'}
 106              v.(['tableDamping_' name{1}]) = cal.table.damping.(name{1});
 107          end
 108          d.calibration.tableUsed = true;
 109      end
 110  end
 111  
 112  % Initial conditions (degrees in the settings, radians in the workspace)
 113  ic = s.initial;
 114  for name = {'RED', 'BLACK', 'BLUE'}
 115      n = name{1};
 116      v.(['init_states_' n]) = pose(ic.(n).initial);
 117      v.(['home_states_' n]) = pose(ic.(n).home);
 118  end
 119  v.init_states_ARM = ic.ARM.initial(:).' * pi/180;
 120  d.armInitial = spot.plotting.armPosition(v.init_states_RED, v.init_states_ARM, arm);
 121  
 122  % The arm model the diagram's RED + ARM plant simulates (servos, friction, its own 1 ms step;
 123  % spot.arm.Plant), with the project's arm calibration (the lab's, or resources/arm_calibration.json,
 124  % the newer) when s.calibration.arm is on
 125  if nargin > 1 && s.calibration.arm
 126      v.armModel = spot.arm.parameters(v, root);
 127  else
 128      v.armModel = spot.arm.parameters(v);
 129  end
 130  d.calibration.armUsed = v.armModel.calibration;   % (the file it came from, or '')
 131  
 132  % The arm's controllers' gains and limits (the diagram's Default Position/Velocity Control (ARM))
 133  v.armControl = armGains(s.control.ARM);
 134  
 135  % Thruster forces: each thruster's thrust, F_thrusters_<PLATFORM> (8x1), which the diagram's mixer and,
 136  % through it, the simulated plant use: the thruster calibration's (Calibration tab) when the project has
 137  % one for that platform and s.calibration.thrusters is on, else thrusterForce for all eight.
 138  % (F_<platform>_X_nominal and _Y_nominal: the setting's, for code of your own)
 139  short = struct('RED', 'red', 'BLACK', 'black', 'BLUE', 'blue');
 140  for name = {'RED', 'BLACK', 'BLUE'}
 141      n = name{1};
 142      F = spot.control.nominalForces(s.thrusterForce.(n));
 143      v.(['F_' short.(n) '_X_nominal']) = F(1);
 144      v.(['F_' short.(n) '_Y_nominal']) = F(3);
 145      if s.calibration.thrusters && ~isempty(cal.thrusters.(n))
 146          F = cal.thrusters.(n).thrust(:);
 147          d.calibration.thrustersUsed{end + 1} = n;
 148      end
 149      v.(['F_thrusters_' n]) = F;
 150  end
 151  
 152  % LQR weights, discrete models and gains
 153  qNames = {'Qx', 'Qy', 'Qtheta', 'QxD', 'QyD', 'QthetaD'};
 154  rNames = {'RFx', 'RFy', 'RTz'};
 155  d.lqr = struct();
 156  d.warnings = {};
 157  if isfield(d, 'warnings0')
 158      d.warnings = d.warnings0;
 159      d = rmfield(d, 'warnings0');
 160  end
 161  if ~isempty(pwmWarning)
 162      d.warnings{end + 1} = pwmWarning;
 163  end
 164  haveControl = license('test', 'Control_Toolbox') && ~isempty(which('dlqr'));
 165  for name = {'RED', 'BLACK', 'BLUE'}
 166      n = name{1};
 167      lq = s.control.(n).lqr;
 168      for k = 1:6
 169          v.([qNames{k} '_' n]) = lq.Q(k);
 170      end
 171      for k = 1:3
 172          v.([rNames{k} '_' n]) = lq.R(k);
 173      end
 174      if haveControl
 175          sys = spot.control.stateSpace(mp.(n).mass, mp.(n).inertia, v.dataRate);
 176          v.(['A_d_' n]) = sys.A;
 177          v.(['B_d_' n]) = sys.B;
 178          v.(['C_d_' n]) = sys.C;
 179          v.(['D_d_' n]) = sys.D;
 180          try
 181              v.(['K_' n]) = spot.control.lqrGains(sys, lq.Q, lq.R);
 182              d.lqr.(n) = v.(['K_' n]);
 183          catch err
 184              d.warnings{end + 1} = sprintf('%s LQR gains: %s', n, err.message);
 185          end
 186      end
 187  end
 188  if ~haveControl
 189      d.warnings{end + 1} = 'The Control System Toolbox is not available, so the LQR gains were not calculated.';
 190  end
 191  
 192  % Into the base workspace
 193  for f = fieldnames(v).'
 194      assignin('base', f{1}, v.(f{1}));
 195  end
 196  d.variables = fieldnames(v);
 197  
 198  % The user's own variables (they may override any of the above)
 199  if nargin > 1
 200      script = fullfile(root, 'SPOT_User_Parameters.m');
 201      if isfile(script)
 202          evalin('base', sprintf('run(''%s'');', strrep(script, '''', '''''')));
 203          d.userScript = script;
 204      end
 205  end
 206  
 207  % The arm controllers' motor, after the user's changes to armModel (the controller blocks take it
 208  % on its own: blocks sharing armModel itself would make it a tunable parameter in the generated
 209  % code, which its field friction.static, a C++ keyword, prevents)
 210  evalin('base', 'if exist(''armModel'', ''var''), armMotor = armModel.motor; end');
 211  end
 212  
 213  function g = armGains(arm)
 214  % The arm controllers' settings, checked: per-joint gains (one value for all three, or three), the
 215  % profile and the PWM limit within the servos' register ranges
 216  g = arm;
 217  checks = {'position', 'P'; 'position', 'I'; 'position', 'D'; 'velocity', 'P'; 'velocity', 'I'};
 218  for k = 1:size(checks, 1)
 219      x = arm.(checks{k, 1}).(checks{k, 2});
 220      if ~(isnumeric(x) && any(numel(x) == [1 3]) && all(isfinite(x)) && all(x >= 0) && all(x <= 16383))
 221          error('spot:params:apply', 'The arm''s %s control %s gain must be 0 to 16383 (one value, or one per joint).', ...
 222              checks{k, 1}, checks{k, 2});
 223      end
 224      g.(checks{k, 1}).(checks{k, 2}) = double(x(:)).' .* ones(1, 3);   % always [shoulder elbow wrist]
 225  end
 226  limits = {'position', 'profileVelocity', 32767; 'position', 'profileAcceleration', 32767; ...
 227            'position', 'pwmLimit', 885; 'velocity', 'profileAcceleration', 32767; 'velocity', 'pwmLimit', 885};
 228  for k = 1:size(limits, 1)
 229      x = arm.(limits{k, 1}).(limits{k, 2});
 230      if ~(isnumeric(x) && isscalar(x) && isfinite(x) && x >= 0 && x <= limits{k, 3})
 231          error('spot:params:apply', 'The arm''s %s control %s must be 0 to %d.', limits{k, 1}, limits{k, 2}, limits{k, 3});
 232      end
 233      g.(limits{k, 1}).(limits{k, 2}) = double(x);
 234  end
 235  end
 236  
 237  function p = pose(xyDeg)
 238  % [x y theta_deg] -> [x y theta_rad]
 239  p = [xyDeg(1), xyDeg(2), xyDeg(3) * pi/180];
 240  end
```

</details>
<!-- /listing -->
