function d = apply(s, root)
%APPLY Put the settings into the base workspace, as the Simulink diagrams expect them.
%   d = spot.params.apply(s) assigns every workspace variable the old GUI assigned (phase times,
%   rates, gains, LQR matrices, mass properties, initial conditions, thruster forces, ...), and
%   armModel (spot.arm.parameters, for the simulated arm), and
%   returns the derived values in d (for display, e.g. tsim, the CG and the LQR gains).
%   d = spot.params.apply(s, projectFolder) also uses the project's calibration of the lab
%   (resources/spot_calibration.json, spot.params.loadCalibration: the table's slope, with
%   s.calibration.tableSlope, each thruster's thrust, F_thrusters_<PLATFORM>, with
%   s.calibration.thrusters, and the arm's friction and inertia, with s.calibration.arm), and runs projectFolder/SPOT_User_Parameters.m afterwards, where users
%   define their own variables or override these.
%
%   Example (no GUI needed):
%     s = spot.params.load('spot_settings.json');
%     s.platforms.RED = true;
%     spot.params.apply(s, pwd);
%     out = spot.sim.run(s, pwd);
d = struct();
v = struct();   % name -> value, assigned to the base workspace at the end

% Platforms
d.platformSelection = spot.params.platformSelection(s.platforms);
v.platformSelection = d.platformSelection;

% Phase timing
t = spot.params.timing(s);
for f = fieldnames(t).'
    if ~any(strcmp(f{1}, {'phases', 'segments'}))   % (lists for the console, not diagram variables)
        v.(f{1}) = t.(f{1});
    end
end
d.timing = t;
d.tsim = t.tsim;

% Rates
if ~(s.sampleRate > 0)
    error('spot:params:apply', 'The sample rate must be above 0 Hz.');
end
if ~(isscalar(s.dataRate) && s.dataRate >= 1 && s.dataRate <= 960 && s.dataRate == round(s.dataRate))
    error('spot:params:apply', 'The PhaseSpace data rate must be a whole number of Hz, 1 to 960.');
end
v.baseRate = 1 / s.sampleRate;
v.dataRate = 1 / s.dataRate;            % [s] the setting is in Hz
d.baseRate = v.baseRate;
d.dataRate = v.dataRate;
d.cameraRate = s.dataRate;              % PhaseSpace stream rate the Experiment tab starts [Hz]

% Thrusters' PWM frequency, and the lab console's address (for the diagram's UDP blocks)
if ~(isscalar(s.pwmFrequency) && s.pwmFrequency > 0)
    error('spot:params:apply', 'The thrusters'' PWM frequency must be above 0 Hz.');
end
v.PWMFreq = s.pwmFrequency;
pwmWarning = '';
if s.pwmFrequency ~= 5
    pwmWarning = sprintf(['The thrusters'' PWM frequency is %g Hz, but the Teensy firmware runs its PWM at ' ...
        '5 Hz (PWM_FREQUENCY in +spot/+hardware/teensy/teensy_pwm_control.ino): flash it with the same value, or ' ...
        'the duty cycles won''t mean what the diagram assumes.'], s.pwmFrequency);
end
v.labConsoleIP = spot.params.ipv4(s.labConsoleAddress);

% Table size, and the simulation flag (spot.sim.run sets simMode = 1 while simulating)
v.xLength = 3.51155;   % [m]
v.yLength = 2.41935;   % [m]

% Mass properties
mp = spot.massprops.compute(s.mass);
d.mass = mp;
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1};
    v.(['m' n]) = mp.(n).mass;
    v.(['I' n]) = mp.(n).inertia;
    v.(['thruster_dist2CG_' n]) = mp.(n).thrusters;
end
% The arm, where it is mounted (s.mass.arm.mount): Phi and b0 for that place, and the angle it is
% turned by (armShoulderOffset: the shoulder's 0 is straight out from its face)
arm = spot.arm.mountGeometry(s.mass.arm);
for f = {'Phi', 'b0', 'a1', 'b1', 'a2', 'b2', 'a3', 'b3', 'mPROX', 'mDIST', 'mEE', 'IPROX', 'IDIST', 'IEE'}
    v.(f{1}) = arm.(f{1});
end
v.armShoulderOffset = arm.q1Offset;
d.armGeometry = arm;

% The lab's calibration (the Calibration tab; resources/spot_calibration.json): the table's slope
% pulls the floating platforms, and their air bearings drag them (tableSlope = [a1 .. a5 xc yc],
% spot.sim.tableSlope; zeros: a level table). Without the project folder, or with the setting off,
% the table is level.
v.tableSlope = [0 0 0 0 0 1.75 1.2];
for name = {'RED', 'BLACK', 'BLUE'}
    v.(['tableDamping_' name{1}]) = 0;
end
none = struct('RED', [], 'BLACK', [], 'BLUE', []);
cal = struct('file', '', 'updated', '', 'table', [], 'thrusters', none, 'error', '');
d.calibration = struct('file', '', 'updated', '', 'table', [], 'tableUsed', false, 'thrusters', none, ...
    'thrustersUsed', {{}}, 'arm', [], 'armUsed', '', 'error', '');
if nargin > 1
    cal = spot.params.loadCalibration(root);
    d.calibration = struct('file', cal.file, 'updated', cal.updated, 'table', cal.table, ...
        'tableUsed', false, 'thrusters', cal.thrusters, 'thrustersUsed', {{}}, 'arm', cal.arm, 'armUsed', '', ...
        'error', cal.error);
    if ~isempty(cal.error)
        d.warnings0 = {cal.error};
    end
    if ~isempty(cal.table) && s.calibration.tableSlope
        v.tableSlope = cal.table.slope;
        for name = {'RED', 'BLACK', 'BLUE'}
            v.(['tableDamping_' name{1}]) = cal.table.damping.(name{1});
        end
        d.calibration.tableUsed = true;
    end
end

% Initial conditions (degrees in the settings, radians in the workspace)
ic = s.initial;
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1};
    v.(['init_states_' n]) = pose(ic.(n).initial);
    v.(['home_states_' n]) = pose(ic.(n).home);
end
v.init_states_ARM = ic.ARM.initial(:).' * pi/180;
d.armInitial = spot.plotting.armPosition(v.init_states_RED, v.init_states_ARM, arm);

% The arm model the diagram's RED + ARM plant simulates (servos, friction, its own 1 ms step;
% spot.arm.Plant), with the project's arm calibration (the lab's, or resources/arm_calibration.json,
% the newer) when s.calibration.arm is on
if nargin > 1 && s.calibration.arm
    v.armModel = spot.arm.parameters(v, root);
else
    v.armModel = spot.arm.parameters(v);
end
d.calibration.armUsed = v.armModel.calibration;   % (the file it came from, or '')

% The arm's controllers' gains and limits (the diagram's Default Position/Velocity Control (ARM))
v.armControl = armGains(s.control.ARM);

% Thruster forces: each thruster's thrust, F_thrusters_<PLATFORM> (8x1), which the diagram's mixer and,
% through it, the simulated plant use: the thruster calibration's (Calibration tab) when the project has
% one for that platform and s.calibration.thrusters is on, else thrusterForce for all eight.
% (F_<platform>_X_nominal and _Y_nominal: the setting's, for code of your own)
short = struct('RED', 'red', 'BLACK', 'black', 'BLUE', 'blue');
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1};
    F = spot.control.nominalForces(s.thrusterForce.(n));
    v.(['F_' short.(n) '_X_nominal']) = F(1);
    v.(['F_' short.(n) '_Y_nominal']) = F(3);
    if s.calibration.thrusters && ~isempty(cal.thrusters.(n))
        F = cal.thrusters.(n).thrust(:);
        d.calibration.thrustersUsed{end + 1} = n;
    end
    v.(['F_thrusters_' n]) = F;
end

% LQR weights, discrete models and gains
qNames = {'Qx', 'Qy', 'Qtheta', 'QxD', 'QyD', 'QthetaD'};
rNames = {'RFx', 'RFy', 'RTz'};
d.lqr = struct();
d.warnings = {};
if isfield(d, 'warnings0')
    d.warnings = d.warnings0;
    d = rmfield(d, 'warnings0');
end
if ~isempty(pwmWarning)
    d.warnings{end + 1} = pwmWarning;
end
haveControl = license('test', 'Control_Toolbox') && ~isempty(which('dlqr'));
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1};
    lq = s.control.(n).lqr;
    for k = 1:6
        v.([qNames{k} '_' n]) = lq.Q(k);
    end
    for k = 1:3
        v.([rNames{k} '_' n]) = lq.R(k);
    end
    if haveControl
        sys = spot.control.stateSpace(mp.(n).mass, mp.(n).inertia, v.dataRate);
        v.(['A_d_' n]) = sys.A;
        v.(['B_d_' n]) = sys.B;
        v.(['C_d_' n]) = sys.C;
        v.(['D_d_' n]) = sys.D;
        try
            v.(['K_' n]) = spot.control.lqrGains(sys, lq.Q, lq.R);
            d.lqr.(n) = v.(['K_' n]);
        catch err
            d.warnings{end + 1} = sprintf('%s LQR gains: %s', n, err.message);
        end
    end
end
if ~haveControl
    d.warnings{end + 1} = 'The Control System Toolbox is not available, so the LQR gains were not calculated.';
end

% Into the base workspace
for f = fieldnames(v).'
    assignin('base', f{1}, v.(f{1}));
end
d.variables = fieldnames(v);

% The user's own variables (they may override any of the above)
if nargin > 1
    script = fullfile(root, 'SPOT_User_Parameters.m');
    if isfile(script)
        evalin('base', sprintf('run(''%s'');', strrep(script, '''', '''''')));
        d.userScript = script;
    end
end

% The arm controllers' motor, after the user's changes to armModel (the controller blocks take it
% on its own: blocks sharing armModel itself would make it a tunable parameter in the generated
% code, which its field friction.static, a C++ keyword, prevents)
evalin('base', 'if exist(''armModel'', ''var''), armMotor = armModel.motor; end');
end

function g = armGains(arm)
% The arm controllers' settings, checked: per-joint gains (one value for all three, or three), the
% profile and the PWM limit within the servos' register ranges
g = arm;
checks = {'position', 'P'; 'position', 'I'; 'position', 'D'; 'velocity', 'P'; 'velocity', 'I'};
for k = 1:size(checks, 1)
    x = arm.(checks{k, 1}).(checks{k, 2});
    if ~(isnumeric(x) && any(numel(x) == [1 3]) && all(isfinite(x)) && all(x >= 0) && all(x <= 16383))
        error('spot:params:apply', 'The arm''s %s control %s gain must be 0 to 16383 (one value, or one per joint).', ...
            checks{k, 1}, checks{k, 2});
    end
    g.(checks{k, 1}).(checks{k, 2}) = double(x(:)).' .* ones(1, 3);   % always [shoulder elbow wrist]
end
limits = {'position', 'profileVelocity', 32767; 'position', 'profileAcceleration', 32767; ...
          'position', 'pwmLimit', 885; 'velocity', 'profileAcceleration', 32767; 'velocity', 'pwmLimit', 885};
for k = 1:size(limits, 1)
    x = arm.(limits{k, 1}).(limits{k, 2});
    if ~(isnumeric(x) && isscalar(x) && isfinite(x) && x >= 0 && x <= limits{k, 3})
        error('spot:params:apply', 'The arm''s %s control %s must be 0 to %d.', limits{k, 1}, limits{k, 2}, limits{k, 3});
    end
    g.(limits{k, 1}).(limits{k, 2}) = double(x);
end
end

function p = pose(xyDeg)
% [x y theta_deg] -> [x y theta_rad]
p = [xyDeg(1), xyDeg(2), xyDeg(3) * pi/180];
end
