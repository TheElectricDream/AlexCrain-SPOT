# spot.params.defaults

**File:** `+spot/+params/defaults.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/defaults.m -->

## What it is for

The one place where every setting has its starting value. The console starts from it when there is no `spot_settings.json` yet, *Reset…* goes back to it, and [[spot.params.load|spot-params-load]] fills anything missing in a file from it. The values are the ones the old GUI's "Reset GUI Parameters to Default" loaded, so old and new behave the same out of the box.

## How to call it

```matlab
s = spot.params.defaults();
```

Returns the settings struct described on [[The Settings File|Settings-File]]. Units: metres, degrees (as typed in the GUI), seconds, kilograms, kg m², newtons.

## Line by line

**Lines 1–8.** The function takes nothing and returns `s`. The help text states the units.

**Lines 10–11.** Start with an empty struct and stamp the file-format version, `1`. Nothing checks it yet; it is there so a future version can convert old files.

**Lines 13–15.** No diagram is chosen (`''`); the worker picks the first `.slx` in the folder on first start ([[spot.worker.Worker|spot-worker-Worker]]). All four platforms start switched **off**.

**Lines 17–22.** `sampleRate` is the controllers' rate, 20 Hz; the diagram's step is `baseRate = 1/sampleRate`. `dataRate` is the PhaseSpace camera rate the Experiment tab starts the stream at, 10 Hz; the LQR gains are designed at its period. The comment records that files saved before `dataRate` existed get half the sample rate (done in [[load|spot-params-load]]).

**Lines 23–24.** `realTime` off: simulations run as fast as they can.

**Lines 25–27.** The thrusters' PWM frequency, 5 Hz. The diagram turns a duty cycle into "valve open for *duty* × (1/PWMFreq) seconds", so the value must match the Teensy firmware's constant (also 5 Hz); [[apply|spot-params-apply]] warns when it doesn't.

**Lines 28–31.** The lab console's address, `192.168.1.116`. It becomes `labConsoleIP = [192 168 1 116]` in the workspace for the UDP blocks.

**Lines 33–40.** The phase timeline: phases 0, 1, 2 (10, 5, 40 s), the four sub-phases of phase 3 (10, 10, 30, 120 s), phases 4 and 5 (30, 20 s), and `on`, the switches that let you skip phase 2, any sub-phase, or phase 4. Phases 0, 1 and 5 can't be skipped: they start and stop the platforms. The total is 275 s.

**Lines 42–48.** The solver: fixed-step `ode4` by default; the variable-step alternative (`VariableStepAuto`) and its tolerances are kept so the Solver pane can switch. Selecting a diagram copies the diagram's own solver settings over these ([[spot.sim.readSolver|spot-sim-readSolver]]).

**Lines 50–57.** Initial conditions, `[x y θ]` in m, m, degrees. `initial` is where the platform starts and where phase 2 drives it; `home` is where phase 4 returns it. The arm's initial pose is its three joint angles, `[90 90 0]` degrees. (The old "drop" pose no longer exists.)

**Lines 59–66.** Mass properties, one entry per platform through the local helper `platformMass` (lines 106–109): the three scale readings `[A B C]`, the bifilar pendulum test (`T` period, `L` string length, `D` string spacing) and the mass and inertia that are used instead when `override` is true. The numbers are the lab's 2025 measurements.

**Lines 67–76.** The arm: `Phi` (the shoulder's mounting angle on the body, rad) and `b0` (its distance from the body centre, m) are for the default mounting place; `a1, b1, a2, b2, a3, b3` are the link lengths to and from each joint, `mPROX, mDIST, mEE` the link masses and `IPROX, IDIST, IEE` their inertias. `mount` says which face and side of the Chaser the arm is on (`+y`, left); [[spot.arm.mountGeometry|spot-arm-mountGeometry]] moves `Phi` and `b0` when the Designer moves the arm.

**Lines 78–83.** The lab's calibration (`resources/spot_calibration.json`, from the [[Calibration Tab|Calibration-Tab]]; [[apply|spot-params-apply]]): `calibration.tableSlope` on, so simulations use the table's measured slope when the project has a table calibration (off, or with no calibration, the table is level), and `calibration.thrusters` on, so the mixer and the simulated plant use each thruster's measured thrust when the project has a thruster calibration for that platform (off, or with none, `thrusterForce` for all eight), and `calibration.arm` on, so the simulated arm uses its measured friction and inertia when the project has an arm calibration ([[spot.arm.parameters|spot-arm-parameters]]; off, or with none, its defaults).

**Lines 85–86.** 0.2825 N for every thruster of every platform.

**Lines 88–91.** The LQR weights, the same for every platform: `Q = [1 1 0.05 10 10 1]` on `[x y θ ẋ ẏ θ̇]` and `R = [6 6 6]` on `[Fx Fy Tz]`. There is no PD controller any more; old files with PD gains still load and the gains are ignored.

**Lines 92–98.** The arm's two controllers, the servos' own control laws run by the diagram ([[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]]), in the servos' own units so the numbers mean what they mean in the Dynamixel manual:
- position: P, I, D per joint `[shoulder elbow wrist]` (`[25 56 21]`, `[10 10 10]`, `[1545 230 236]`, retuned for the diagram's 20 Hz), the profile velocity (20 × 0.229 rev/min = 0.48 rad/s) and acceleration (0 = none), and the PWM limit (80 of 885);
- velocity: P and I per joint (`[160 70 30]`, `[220 1500 300]`), the profile acceleration and the PWM limit (885, full).

**Lines 99–103.** The `control` struct: the LQR weights for each platform and the arm's two controllers.

**Lines 106–109.** `platformMass` just packs its six arguments into the nested struct used above.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+console/webapp/static/cad.js`, `+spot/+control/thrusterDutyCycles.m`, `+spot/+control/verifyConsoleController.m`, `+spot/+data/toFigure.m`, `+spot/+data/trajectory.m`, `+spot/+experiment/buildBoard.m`, `+spot/+params/fromLegacyState.m`, `+spot/+params/load.m`, `+spot/+params/reset.m`, `+spot/+params/update.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.arm.PositionControl|spot-arm-PositionControl]], [[spot.arm.VelocityControl|spot-arm-VelocityControl]], [[spot.arm.mountGeometry|spot-arm-mountGeometry]], [[spot.arm.parameters|spot-arm-parameters]], [[spot.params.apply|spot-params-apply]], [[spot.params.save|spot-params-save]], [[spot.params.timing|spot-params-timing]], `spot.sim.selectDiagram`
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/defaults.m</b> (109 lines; click to show)</summary>

```matlab
   1  function s = defaults()
   2  %DEFAULTS Default SPOT settings.
   3  %   s = spot.params.defaults() returns the settings struct the SPOT Console starts from (the
   4  %   values the old GUI's "Reset GUI Parameters to Default" loaded). Settings are saved as JSON (spot.params.save) and turned into
   5  %   workspace variables for the Simulink diagrams by spot.params.apply.
   6  %
   7  %   Units: lengths in m, angles in degrees (as typed in the GUI), times in s, masses in kg,
   8  %   inertias in kg*m^2, forces in N.
   9  
  10  s = struct();
  11  s.version = 1;
  12  
  13  % Simulink diagram (a .slx file in the project folder) and the platforms taking part
  14  s.diagram = '';
  15  s.platforms = struct('RED', false, 'BLACK', false, 'BLUE', false, 'ARM', false);
  16  
  17  % Controller sample rate: baseRate = 1/sampleRate
  18  s.sampleRate = 20;                                          % [Hz]
  19  % PhaseSpace data rate: the rate the Experiment tab starts the cameras at, a whole number of Hz
  20  % (1 to 960, as the lab console's Frequency). The workspace's dataRate is its period, 1/dataRate
  21  % s, at which the LQR gains are designed. (Settings saved before it existed get sampleRate/2.)
  22  s.dataRate = 10;                                            % [Hz]
  23  % Simulations at the clock's pace (Simulink's simulation pacing), e.g. to watch the Live Plot
  24  s.realTime = false;
  25  % Thrusters' PWM frequency [Hz] (the workspace's PWMFreq): the diagram's duty cycles assume a valve
  26  % period of 1/pwmFrequency. The Teensy firmware has its own (PWM_FREQUENCY, 5 Hz): keep them equal.
  27  s.pwmFrequency = 5;                                         % [Hz]
  28  % The lab console: the Jetson TX2 that streams PhaseSpace to the platforms, builds and deploys the
  29  % experiments, and receives the platforms' clocks, debug messages and live streams. Its IPv4
  30  % address (the workspace's labConsoleIP, as four numbers, for the diagram's UDP blocks).
  31  s.labConsoleAddress = '192.168.1.116';
  32  
  33  % Phase durations. Phase 3 (the experiment) is the sum of its four sub-phases.
  34  % on: which segments run (a segment switched off is skipped, see spot.params.timing); phases 0,
  35  % 1 and 5 always run.
  36  s.timing = struct( ...
  37      'phase0', 10, 'phase1', 5, 'phase2', 40, ...            % startup phases [s]
  38      'subPhases', [10 10 30 120], ...                         % phase 3 sub-phases [s]
  39      'phase4', 30, 'phase5', 20, ...                          % termination phases [s]
  40      'on', struct('phase2', true, 'subPhases', true(1, 4), 'phase4', true));
  41  
  42  % Solver. Selecting a diagram copies its own solver settings in here (spot.sim.selectDiagram).
  43  s.solver = struct( ...
  44      'type', 'Fixed-step', ...
  45      'fixedStepName', 'ode4', ...
  46      'variableStepName', 'VariableStepAuto', ...
  47      'maxStep', 'auto', 'minStep', 'auto', 'initialStep', 'auto', ...
  48      'relTol', '1e-3', 'absTol', 'auto');
  49  
  50  % Initial conditions: [x y theta] in m, m, deg. "initial" is where the platform starts (the
  51  % simulation starts there, and phase 2 drives it there), "home" where it returns to. The arm's
  52  % initial pose is its three joint angles [q1 q2 q3] in deg. (There is no "drop" pose any more.)
  53  s.initial = struct( ...
  54      'RED',   struct('initial', [2.61 1.21 0],   'home', [2.61 1.21 0]), ...
  55      'BLACK', struct('initial', [1.33 1.95 120], 'home', [1.33 1.95 120]), ...
  56      'BLUE',  struct('initial', [1.33 0.47 240], 'home', [1.33 0.47 240]), ...
  57      'ARM',   struct('initial', [90 90 0]));
  58  
  59  % Mass properties. Mass and centre of gravity come from the three scale readings [A B C] (kg),
  60  % the inertia from a bifilar pendulum test (period T [s], string length L [m], string spacing
  61  % D [m]). With override = true, the mass and inertia typed in are used instead.
  62  s.mass = struct();
  63  s.mass.override = false;
  64  s.mass.RED   = platformMass([6.17 3.12 4.46], 3.00, 2.56, 0.28, 13.75, 0.23543476058293455);
  65  s.mass.BLACK = platformMass([6.14 3.07 4.01], 3.11, 2.56, 0.28, 13.22, 0.2432638691100455);
  66  s.mass.BLUE  = platformMass([5.65 3.08 4.04], 3.02, 2.56, 0.28, 12.77, 0.22157979686198798);
  67  s.mass.arm = struct( ...
  68      'Phi', 1.2337865258868412, ...                           % [rad] shoulder mounting angle (CAD:
  69      'b0', 0.2502067178238507, ...                            % [m]  motor flush, centred on the coupler)
  70      'a1', 0.196822, 'b1', 0.107678, ...                      % link geometry [m]
  71      'a2', 0.198152, 'b2', 0.106348, 'a3', 0.062097, 'b3', 0.025153, ...
  72      'mPROX', 0.345, 'mDIST', 0.335, 'mEE', 0.111, ...        % link masses [kg]
  73      'IPROX', 0.003704, 'IDIST', 0.003506, 'IEE', 0.000106, ...  % link inertias [kg*m^2]
  74      'mount', struct('face', '+y', 'side', 'left'));          % where it is (Spacecraft Designer): Phi and b0
  75                                                               % are for this place, spot.arm.mountGeometry
  76                                                               % moves them with it
  77  
  78  % The lab's calibration (Calibration tab; the project's resources/spot_calibration.json): whether
  79  % simulations use the table's measured slope (the floating platforms slide downhill; off: a level
  80  % table), each thruster's measured thrust (the mixer and the simulated plant; off: thrusterForce
  81  % below for all eight), and the arm's measured friction and inertia (spot.arm.parameters; off: its
  82  % defaults)
  83  s.calibration = struct('tableSlope', true, 'thrusters', true, 'arm', true);
  84  
  85  % Thrust of one thruster [N] (the GUI used 0.2825 N for every thruster)
  86  s.thrusterForce = struct('RED', 0.2825, 'BLACK', 0.2825, 'BLUE', 0.2825);
  87  
  88  % Controller: LQR weights Q = [x y theta dx/dt dy/dt dtheta/dt], R = [Fx Fy Tz]
  89  % (the diagrams have no PD controller any more; settings saved with PD gains still load, and
  90  % the gains are dropped)
  91  lqr = struct('Q', [1 1 0.05 10 10 1], 'R', [6 6 6]);
  92  % The arm's controllers (the servos' own position and velocity laws, run by the diagram at
  93  % baseRate; spot.arm.PositionControl and spot.arm.VelocityControl): gains in the servos' units
  94  % [shoulder elbow wrist], retuned for 20 Hz; profile velocity [0.229 rev/min], profile
  95  % acceleration [214.577 rev/min^2] (0 = none) and PWM limit (0-885), as the servos' registers
  96  armPosition = struct('P', [25 56 21], 'I', [10 10 10], 'D', [1545 230 236], ...
  97      'profileVelocity', 20, 'profileAcceleration', 0, 'pwmLimit', 80);
  98  armVelocity = struct('P', [160 70 30], 'I', [220 1500 300], 'profileAcceleration', 0, 'pwmLimit', 885);
  99  s.control = struct( ...
 100      'RED', struct('lqr', lqr), ...
 101      'BLACK', struct('lqr', lqr), ...
 102      'BLUE', struct('lqr', lqr), ...
 103      'ARM', struct('position', armPosition, 'velocity', armVelocity));
 104  end
 105  
 106  function m = platformMass(scales, T, L, D, mass, inertia)
 107  m = struct('scales', scales, 'bifilar', struct('T', T, 'L', L, 'D', D), ...
 108             'mass', mass, 'inertia', inertia);
 109  end
```

</details>
<!-- /listing -->
