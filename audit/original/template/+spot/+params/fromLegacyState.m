function s = fromLegacyState(file)
    %FROMLEGACYSTATE Convert the old SPOT GUI's saved state (Save GUI State) to settings.
    %   s = spot.params.fromLegacyState(file) reads the appState, subAppState (mass properties)
    %   and subAppStateInit (initial conditions) structs, whose fields are App Designer component
    %   names. Anything not in the file keeps its default.
    %
    %   Inputs
    %     file  the old GUI's state file (.mat)
    %
    %   Outputs
    %     s     settings
    %
    %   See also spot.params.load, spot.params.defaults.

    %% The state file
    stored = load(file);
    if ~isfield(stored, 'appState')
        error('spot:params:fromLegacyState', ...
            '%s is not a SPOT GUI state file (no appState).', file);
    end
    app = stored.appState;
    massState = getfield_or(stored, 'subAppState', struct());
    initState = getfield_or(stored, 'subAppStateInit', struct());
    s = spot.params.defaults();

    %% Platforms and diagram
    s.platforms.RED = num(app, 'REDCheckBox', s.platforms.RED) ~= 0;
    s.platforms.BLACK = num(app, 'BLACKCheckBox', s.platforms.BLACK) ~= 0;
    s.platforms.BLUE = num(app, 'BLUECheckBox', s.platforms.BLUE) ~= 0;
    s.platforms.ARM = num(app, 'ARMCheckBox', s.platforms.ARM) ~= 0;
    diagram = txt(app, 'AvailableDiagramsDropDown', '');
    if endsWith(diagram, '.slx')
        s.diagram = diagram;
    end

    %% Timing and rate
    s.sampleRate = num(app, 'SampleRateEditField', s.sampleRate);
    cfg = s.timing;
    cfg.phase0 = num(app, 'DurPhase0EditField', cfg.phase0);
    cfg.phase1 = num(app, 'DurPhase1EditField', cfg.phase1);
    cfg.phase2 = num(app, 'DurPhase2EditField', cfg.phase2);
    for k = 1:4
        cfg.subPhases(k) = num(app, sprintf('SubPhase%dEditField', k), cfg.subPhases(k));
    end
    cfg.phase4 = num(app, 'DurPhase4EditField', cfg.phase4);
    cfg.phase5 = num(app, 'DurPhase5EditField', cfg.phase5);
    s.timing = cfg;

    %% Solver
    s.solver.type = txt(app, 'SolverTypeDropDown', s.solver.type);
    s.solver.fixedStepName = txt(app, 'SolverNameDropDown', s.solver.fixedStepName);
    s.solver.variableStepName = txt(app, 'SolverNameVarStepDropDown', s.solver.variableStepName);

    % Variable-step options: {setting, GUI field}; empty fields keep the default.
    pairs = {'maxStep', 'VarStepMaxStepSizeEditField'
             'minStep', 'VarStepMinStepSizeEditField'
             'initialStep', 'VarStepInitStepSizeEditField'
             'relTol', 'VarStepRelTolEditField'
             'absTol', 'VarStepAbsTolEditField'};
    for k = 1:size(pairs, 1)
        value = txt(app, pairs{k, 2}, '');
        if ~isempty(value)
            s.solver.(pairs{k, 1}) = value;
        end
    end

    %% Controllers: LQR weights, in fields named <Red|Black|Blue><weight>EditField
    prefix = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        guiPrefix = prefix.(key);
        ctrl = s.control.(key);
        qFields = {'QX', 'QY', 'QTheta', 'QXD', 'QYD', 'QThetaD'};
        for k = 1:6
            ctrl.lqr.Q(k) = num(app, [guiPrefix qFields{k} 'EditField'], ctrl.lqr.Q(k));
        end
        rFields = {'RFX', 'RFY', 'RTz'};
        for k = 1:3
            ctrl.lqr.R(k) = num(app, [guiPrefix rFields{k} 'EditField'], ctrl.lqr.R(k));
        end
        s.control.(key) = ctrl;
    end

    %% Initial conditions
    % The GUI's "Start" fields are the initial pose (its "Initial" fields were the drop pose,
    % which the diagrams no longer have); its arm angles are the arm's initial pose.
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        pose = s.initial.(key);
        pose.initial = [num(initState, [key 'StartX'], pose.initial(1)), ...
                        num(initState, [key 'StartY'], pose.initial(2)), ...
                        num(initState, [key 'StartTh'], pose.initial(3))];
        pose.home = [num(initState, [key 'HomeX'], pose.home(1)), ...
                     num(initState, [key 'HomeY'], pose.home(2)), ...
                     num(initState, [key 'HomeTh'], pose.home(3))];
        s.initial.(key) = pose;
    end
    s.initial.ARM.initial = [num(initState, 'ARMInitialQ1', s.initial.ARM.initial(1)), ...
                             num(initState, 'ARMInitialQ2', s.initial.ARM.initial(2)), ...
                             num(initState, 'ARMInitialQ3', s.initial.ARM.initial(3))];

    %% Mass properties
    s.mass.override = num(massState, 'OverridePropertiesCheckBox', s.mass.override) ~= 0;

    % Each platform's scales, bifilar test and typed-in values (the GUI's field names vary).
    guiName = struct('RED', 'Red', 'BLACK', 'Black', 'BLUE', 'Blue');
    lname = struct('RED', 'LREDScaleMeasurement', 'BLACK', 'LBLACKMeasurement', ...
                   'BLUE', 'LBLUEMeasurement');
    for name = {'RED', 'BLACK', 'BLUE'}
        key = name{1};
        props = s.mass.(key);
        props.scales = [num(massState, ['A' key 'ScaleMeasurement'], props.scales(1)), ...
                        num(massState, ['B' key 'ScaleMeasurement'], props.scales(2)), ...
                        num(massState, ['C' key 'ScaleMeasurement'], props.scales(3))];
        props.bifilar.T = num(massState, ['T' key 'Measurement'], props.bifilar.T);
        props.bifilar.L = num(massState, lname.(key), props.bifilar.L);
        props.bifilar.D = num(massState, ['D' key 'Measurement'], props.bifilar.D);
        props.mass = num(massState, ['Mass' guiName.(key) 'EditField'], props.mass);
        props.inertia = num(massState, ['Inertia' guiName.(key) 'EditField'], props.inertia);
        s.mass.(key) = props;
    end

    % The arm: {setting, GUI field}.
    armFields = {'Phi', 'ArmPhiEditField'
                 'b0', 'ArmB0EditField'
                 'a1', 'ArmA1EditField'
                 'b1', 'ArmB1EditField'
                 'a2', 'ArmA2EditField'
                 'b2', 'ArmB2EditField'
                 'a3', 'ArmA3EditField'
                 'b3', 'ArmB3EditField'
                 'mPROX', 'MassProximalLinkEditField'
                 'mDIST', 'MassDistalLinkEditField'
                 'mEE', 'MassEndEffectorLinkEditField'
                 'IPROX', 'InertiaProximalLinkEditField'
                 'IDIST', 'InertiaDistalLinkEditField'
                 'IEE', 'InertiaEndEffectorLinkEditField'};
    for k = 1:size(armFields, 1)
        s.mass.arm.(armFields{k, 1}) = num(massState, armFields{k, 2}, ...
            s.mass.arm.(armFields{k, 1}));
    end
end

function v = getfield_or(st, name, default)
    % st.(name) if present, else default.
    if isfield(st, name)
        v = st.(name);
    else
        v = default;
    end
end

function v = num(st, name, default)
    % A numeric or logical scalar field as a double, else default.
    v = default;
    if isfield(st, name)
        x = st.(name);
        if (isnumeric(x) || islogical(x)) && isscalar(x)
            v = double(x);
        end
    end
end

function v = txt(st, name, default)
    % A text field (or a number, as text), else default.
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
