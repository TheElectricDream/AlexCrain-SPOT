function s = fromLegacyState(file)
%FROMLEGACYSTATE Convert a GUI state file saved by the old SPOT GUI (Save GUI State) to settings.
%   s = spot.params.fromLegacyState(file) reads the appState, subAppState (mass properties) and
%   subAppStateInit (initial conditions) structs, whose fields are App Designer component names,
%   and returns a settings struct. Anything not in the file keeps its default.
L = load(file);
if ~isfield(L, 'appState')
    error('spot:params:fromLegacyState', '%s is not a SPOT GUI state file (no appState).', file);
end
a = L.appState;
m = getfield_or(L, 'subAppState', struct());
ic = getfield_or(L, 'subAppStateInit', struct());
s = spot.params.defaults();

% Platforms and diagram
s.platforms.RED = num(a, 'REDCheckBox', s.platforms.RED) ~= 0;
s.platforms.BLACK = num(a, 'BLACKCheckBox', s.platforms.BLACK) ~= 0;
s.platforms.BLUE = num(a, 'BLUECheckBox', s.platforms.BLUE) ~= 0;
s.platforms.ARM = num(a, 'ARMCheckBox', s.platforms.ARM) ~= 0;
diagram = txt(a, 'AvailableDiagramsDropDown', '');
if endsWith(diagram, '.slx')
    s.diagram = diagram;
end

% Timing and rate
s.sampleRate = num(a, 'SampleRateEditField', s.sampleRate);
tm = s.timing;
tm.phase0 = num(a, 'DurPhase0EditField', tm.phase0);
tm.phase1 = num(a, 'DurPhase1EditField', tm.phase1);
tm.phase2 = num(a, 'DurPhase2EditField', tm.phase2);
for k = 1:4
    tm.subPhases(k) = num(a, sprintf('SubPhase%dEditField', k), tm.subPhases(k));
end
tm.phase4 = num(a, 'DurPhase4EditField', tm.phase4);
tm.phase5 = num(a, 'DurPhase5EditField', tm.phase5);
s.timing = tm;

% Solver
s.solver.type = txt(a, 'SolverTypeDropDown', s.solver.type);
s.solver.fixedStepName = txt(a, 'SolverNameDropDown', s.solver.fixedStepName);
s.solver.variableStepName = txt(a, 'SolverNameVarStepDropDown', s.solver.variableStepName);
pairs = {'maxStep', 'VarStepMaxStepSizeEditField'; 'minStep', 'VarStepMinStepSizeEditField';
         'initialStep', 'VarStepInitStepSizeEditField'; 'relTol', 'VarStepRelTolEditField';
         'absTol', 'VarStepAbsTolEditField'};
for k = 1:size(pairs, 1)
    value = txt(a, pairs{k, 2}, '');
    if ~isempty(value)
        s.solver.(pairs{k, 1}) = value;
    end
end

% Controllers
prefix = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1}; p = prefix.(n); c = s.control.(n);
    q = {'QX', 'QY', 'QTheta', 'QXD', 'QYD', 'QThetaD'};
    for k = 1:6
        c.lqr.Q(k) = num(a, [p q{k} 'EditField'], c.lqr.Q(k));
    end
    r = {'RFX', 'RFY', 'RTz'};
    for k = 1:3
        c.lqr.R(k) = num(a, [p r{k} 'EditField'], c.lqr.R(k));
    end
    s.control.(n) = c;
end

% Initial conditions: the GUI's "Start" fields are the initial pose (its "Initial" fields were
% the drop pose, which the diagrams no longer have); its arm angles are the arm's initial pose
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1}; c = s.initial.(n);
    c.initial = [num(ic, [n 'StartX'], c.initial(1)), num(ic, [n 'StartY'], c.initial(2)), num(ic, [n 'StartTh'], c.initial(3))];
    c.home = [num(ic, [n 'HomeX'], c.home(1)), num(ic, [n 'HomeY'], c.home(2)), num(ic, [n 'HomeTh'], c.home(3))];
    s.initial.(n) = c;
end
s.initial.ARM.initial = [num(ic, 'ARMInitialQ1', s.initial.ARM.initial(1)), ...
                         num(ic, 'ARMInitialQ2', s.initial.ARM.initial(2)), num(ic, 'ARMInitialQ3', s.initial.ARM.initial(3))];

% Mass properties
s.mass.override = num(m, 'OverridePropertiesCheckBox', s.mass.override) ~= 0;
field = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
lname = struct('RED', 'LREDScaleMeasurement', 'BLACK', 'LBLACKMeasurement', 'BLUE', 'LBLUEMeasurement');
for name = {'RED', 'BLACK', 'BLUE'}
    n = name{1}; c = s.mass.(n);
    c.scales = [num(m, ['A' n 'ScaleMeasurement'], c.scales(1)), num(m, ['B' n 'ScaleMeasurement'], c.scales(2)), ...
                num(m, ['C' n 'ScaleMeasurement'], c.scales(3))];
    c.bifilar.T = num(m, ['T' n 'Measurement'], c.bifilar.T);
    c.bifilar.L = num(m, lname.(n), c.bifilar.L);
    c.bifilar.D = num(m, ['D' n 'Measurement'], c.bifilar.D);
    c.mass = num(m, ['Mass' field.(n) 'EditField'], c.mass);
    c.inertia = num(m, ['Inertia' field.(n) 'EditField'], c.inertia);
    s.mass.(n) = c;
end
armFields = {'Phi', 'ArmPhiEditField'; 'b0', 'ArmB0EditField'; 'a1', 'ArmA1EditField'; 'b1', 'ArmB1EditField';
             'a2', 'ArmA2EditField'; 'b2', 'ArmB2EditField'; 'a3', 'ArmA3EditField'; 'b3', 'ArmB3EditField';
             'mPROX', 'MassProximalLinkEditField'; 'mDIST', 'MassDistalLinkEditField';
             'mEE', 'MassEndEffectorLinkEditField'; 'IPROX', 'InertiaProximalLinkEditField';
             'IDIST', 'InertiaDistalLinkEditField'; 'IEE', 'InertiaEndEffectorLinkEditField'};
for k = 1:size(armFields, 1)
    s.mass.arm.(armFields{k, 1}) = num(m, armFields{k, 2}, s.mass.arm.(armFields{k, 1}));
end
end

function v = getfield_or(st, name, default)
if isfield(st, name), v = st.(name); else, v = default; end
end

function v = num(st, name, default)
v = default;
if isfield(st, name)
    x = st.(name);
    if (isnumeric(x) || islogical(x)) && isscalar(x)
        v = double(x);
    end
end
end

function v = txt(st, name, default)
v = default;
if isfield(st, name)
    x = st.(name);
    if ischar(x) || (isstring(x) && isscalar(x))
        v = char(x);
    elseif isnumeric(x) && isscalar(x)
        v = num2str(x);
    end
end
end
