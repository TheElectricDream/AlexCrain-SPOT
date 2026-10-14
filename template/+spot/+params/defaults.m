function s = defaults()
%DEFAULTS Default SPOT settings.
%   s = spot.params.defaults() returns the settings struct the SPOT Console starts from (the
%   values the old GUI's "Reset GUI Parameters to Default" loaded). Settings are saved as JSON (spot.params.save) and turned into
%   workspace variables for the Simulink diagrams by spot.params.apply.
%
%   Units: lengths in m, angles in degrees (as typed in the GUI), times in s, masses in kg,
%   inertias in kg*m^2, forces in N.

s = struct();
s.version = 1;

% Simulink diagram (a .slx file in the project folder) and the platforms taking part
s.diagram = '';
s.platforms = struct('RED', false, 'BLACK', false, 'BLUE', false, 'ARM', false);

% Controller sample rate: baseRate = 1/sampleRate
s.sampleRate = 20;                                          % [Hz]
% PhaseSpace data rate: the rate the Experiment tab starts the cameras at, a whole number of Hz
% (1 to 960, as the lab console's Frequency). The workspace's dataRate is its period, 1/dataRate
% s, at which the LQR gains are designed. (Settings saved before it existed get sampleRate/2.)
s.dataRate = 10;                                            % [Hz]
% Simulations at the clock's pace (Simulink's simulation pacing), e.g. to watch the Live Plot
s.realTime = false;
% Thrusters' PWM frequency [Hz] (the workspace's PWMFreq): the diagram's duty cycles assume a valve
% period of 1/pwmFrequency. The Teensy firmware has its own (PWM_FREQUENCY, 5 Hz): keep them equal.
s.pwmFrequency = 5;                                         % [Hz]
% The lab console: the Jetson TX2 that streams PhaseSpace to the platforms, builds and deploys the
% experiments, and receives the platforms' clocks, debug messages and live streams. Its IPv4
% address (the workspace's labConsoleIP, as four numbers, for the diagram's UDP blocks).
s.labConsoleAddress = '192.168.1.116';

% Phase durations. Phase 3 (the experiment) is the sum of its four sub-phases.
% on: which segments run (a segment switched off is skipped, see spot.params.timing); phases 0,
% 1 and 5 always run.
s.timing = struct( ...
    'phase0', 10, 'phase1', 5, 'phase2', 40, ...            % startup phases [s]
    'subPhases', [10 10 30 120], ...                         % phase 3 sub-phases [s]
    'phase4', 30, 'phase5', 20, ...                          % termination phases [s]
    'on', struct('phase2', true, 'subPhases', true(1, 4), 'phase4', true));

% Solver. Selecting a diagram copies its own solver settings in here (spot.sim.selectDiagram).
s.solver = struct( ...
    'type', 'Fixed-step', ...
    'fixedStepName', 'ode4', ...
    'variableStepName', 'VariableStepAuto', ...
    'maxStep', 'auto', 'minStep', 'auto', 'initialStep', 'auto', ...
    'relTol', '1e-3', 'absTol', 'auto');

% Initial conditions: [x y theta] in m, m, deg. "initial" is where the platform starts (the
% simulation starts there, and phase 2 drives it there), "home" where it returns to. The arm's
% initial pose is its three joint angles [q1 q2 q3] in deg. (There is no "drop" pose any more.)
s.initial = struct( ...
    'RED',   struct('initial', [2.61 1.21 0],   'home', [2.61 1.21 0]), ...
    'BLACK', struct('initial', [1.33 1.95 120], 'home', [1.33 1.95 120]), ...
    'BLUE',  struct('initial', [1.33 0.47 240], 'home', [1.33 0.47 240]), ...
    'ARM',   struct('initial', [90 90 0]));

% Mass properties. Mass and centre of gravity come from the three scale readings [A B C] (kg),
% the inertia from a bifilar pendulum test (period T [s], string length L [m], string spacing
% D [m]). With override = true, the mass and inertia typed in are used instead.
s.mass = struct();
s.mass.override = false;
s.mass.RED   = platformMass([6.17 3.12 4.46], 3.00, 2.56, 0.28, 13.75, 0.23543476058293455);
s.mass.BLACK = platformMass([6.14 3.07 4.01], 3.11, 2.56, 0.28, 13.22, 0.2432638691100455);
s.mass.BLUE  = platformMass([5.65 3.08 4.04], 3.02, 2.56, 0.28, 12.77, 0.22157979686198798);
s.mass.arm = struct( ...
    'Phi', 1.2337865258868412, ...                           % [rad] shoulder mounting angle (CAD:
    'b0', 0.2502067178238507, ...                            % [m]  motor flush, centred on the coupler)
    'a1', 0.196822, 'b1', 0.107678, ...                      % link geometry [m]
    'a2', 0.198152, 'b2', 0.106348, 'a3', 0.062097, 'b3', 0.025153, ...
    'mPROX', 0.345, 'mDIST', 0.335, 'mEE', 0.111, ...        % link masses [kg]
    'IPROX', 0.003704, 'IDIST', 0.003506, 'IEE', 0.000106, ...  % link inertias [kg*m^2]
    'mount', struct('face', '+y', 'side', 'left'));          % where it is (Spacecraft Designer): Phi and b0
                                                             % are for this place, spot.arm.mountGeometry
                                                             % moves them with it

% The lab's calibration (Calibration tab; the project's resources/spot_calibration.json): whether
% simulations use the table's measured slope (the floating platforms slide downhill; off: a level
% table), each thruster's measured thrust (the mixer and the simulated plant; off: thrusterForce
% below for all eight), and the arm's measured friction and inertia (spot.arm.parameters; off: its
% defaults)
s.calibration = struct('tableSlope', true, 'thrusters', true, 'arm', true);

% Thrust of one thruster [N] (the GUI used 0.2825 N for every thruster)
s.thrusterForce = struct('RED', 0.2825, 'BLACK', 0.2825, 'BLUE', 0.2825);

% Controller: LQR weights Q = [x y theta dx/dt dy/dt dtheta/dt], R = [Fx Fy Tz]
% (the diagrams have no PD controller any more; settings saved with PD gains still load, and
% the gains are dropped)
lqr = struct('Q', [1 1 0.05 10 10 1], 'R', [6 6 6]);
% The arm's controllers (the servos' own position and velocity laws, run by the diagram at
% baseRate; spot.arm.PositionControl and spot.arm.VelocityControl): gains in the servos' units
% [shoulder elbow wrist], retuned for 20 Hz; profile velocity [0.229 rev/min], profile
% acceleration [214.577 rev/min^2] (0 = none) and PWM limit (0-885), as the servos' registers
armPosition = struct('P', [25 56 21], 'I', [10 10 10], 'D', [1545 230 236], ...
    'profileVelocity', 20, 'profileAcceleration', 0, 'pwmLimit', 80);
armVelocity = struct('P', [160 70 30], 'I', [220 1500 300], 'profileAcceleration', 0, 'pwmLimit', 885);
s.control = struct( ...
    'RED', struct('lqr', lqr), ...
    'BLACK', struct('lqr', lqr), ...
    'BLUE', struct('lqr', lqr), ...
    'ARM', struct('position', armPosition, 'velocity', armVelocity));
end

function m = platformMass(scales, T, L, D, mass, inertia)
m = struct('scales', scales, 'bifilar', struct('T', T, 'L', L, 'D', D), ...
           'mass', mass, 'inertia', inertia);
end
