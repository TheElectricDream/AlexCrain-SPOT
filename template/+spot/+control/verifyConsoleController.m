function report = verifyConsoleController(varargin)
    %VERIFYCONSOLECONTROLLER Check the lab console's interactive controller against the diagram.
    %   spot.control.verifyConsoleController() runs four checks and prints what each found:
    %     1. Code      the verbatim copies in spot.control.lqrCommand and
    %                  spot.control.thrusterDutyCycles still match the diagram's blocks (RED,
    %                  BLACK, BLUE), and each platform's mixer reads its own parameters
    %     2. Diagram   in a short simulation, the two functions reproduce, step by step, the
    %                  forces (*_Controls) and duty cycles (*_Duty_Cycle) the diagram logged.
    %                  Needs a diagram with a fixed step order (atomic subsystems with
    %                  priorities, as in the template); the earlier template diagram fails here
    %                  although the code is the same.
    %     3. Python    control.py (the lab console's port) gives the same LQR gains, forces
    %                  and duty cycles as MATLAB, for random cases
    %     4. Defaults  control.py's parameters match SPOT's defaults, the mixer block's mask
    %                  values and the thruster code's sample time (must be baseRate)
    %   report = spot.control.verifyConsoleController(...) also returns them.
    %
    %   Options (name, value)
    %     'diagram'   .slx in the project folder (default: spot_settings.json's, else the first)
    %     'simulate'  false to skip check 2 (it simulates about 70 s)
    %     'cases'     random cases for check 3 (default 300)
    %
    %   Outputs
    %     report      report.<check> has ok (true, false, or [] when skipped), text (one line
    %                 per finding) and the numbers behind them
    %
    %   Needs Simulink, the Control System Toolbox and Python 3.6+. The diagram is loaded but
    %   not changed.
    %
    %   See also spot.control.lqrCommand, spot.control.thrusterDutyCycles,
    %   spot.control.updateDiagramMixer.

    %% Options
    opt = struct('diagram', '', 'simulate', true, 'cases', 300);
    for k = 1:2:numel(varargin)
        opt.(varargin{k}) = varargin{k + 1};
    end

    %% Diagram
    % Project folder, above +spot.
    root = fileparts(fileparts(fileparts(mfilename('fullpath'))));
    if isempty(opt.diagram)
        opt.diagram = defaultDiagram(root);
    end
    model = spot.sim.loadDiagram(opt.diagram, root);
    platforms = {'RED', 'BLACK', 'BLUE'};

    %% Run the checks
    report = struct();
    [report.code, wiring] = checkCode(model, platforms);
    if opt.simulate
        report.diagram = checkDiagram(root, opt.diagram, model, platforms, wiring);
    else
        report.diagram = struct('ok', [], 'text', {{'skipped (''simulate'', false)'}});
    end
    [report.python, py] = checkPython(root, opt.cases);
    report.defaults = checkDefaults(model, py);

    %% Print
    fprintf('\nInteractive Mode''s controller, checked against %s\n', opt.diagram);
    titles = struct('code', '1. Code', 'diagram', '2. Diagram', 'python', '3. Python', ...
                    'defaults', '4. Defaults');
    for f = fieldnames(titles).'
        result = report.(f{1});
        if isempty(result.ok)
            mark = '-';
        elseif result.ok
            mark = 'OK';
        else
            mark = 'FAILED';
        end
        fprintf('\n%s: %s\n', titles.(f{1}), mark);
        fprintf('   %s\n', result.text{:});
    end
    fprintf('\n');

    % No output requested: don't echo the report.
    if nargout == 0
        clear report
    end
end

%% 1. Code

function [result, wiring] = checkCode(model, platforms)
    % The verbatim sections of the two functions against the blocks' MATLAB Function code.
    % wiring.<PLATFORM> lists the parameters each mixer block reads.

    % The copies, and every MATLAB Function block in the diagram.
    here = fileparts(mfilename('fullpath'));
    copies = [verbatim(fullfile(here, 'lqrCommand.m'));
              verbatim(fullfile(here, 'thrusterDutyCycles.m'))];
    charts = find(sfroot, '-isa', 'Stateflow.EMChart');
    paths = strrep(get(charts, 'Path'), newline, ' ');
    if ischar(paths)
        paths = {paths};
    end

    result = struct('ok', true, 'text', {{}});
    wiring = struct();
    for k = 1:size(copies, 1)
        for p = platforms

            % This platform's block.
            block = strrep(copies{k, 1}, '(RED)', ['(' p{1} ')']);
            block = strrep(block, 'Change RED Behavior', ['Change ' p{1} ' Behavior']);
            hit = strcmp(paths, [model '/' block]);
            if ~any(hit)
                result.ok = false;
                result.text{end + 1} = sprintf('%s: block not found in the diagram', block);
                continue
            end

            % Compare its code with the copy, with BLACK's and BLUE's parameter names mapped to
            % RED's (thruster_dist2CG_BLACK, F_thrusters_BLACK, ...).
            code = normalise(charts(hit).Script);
            code = strrep(strrep(code, ['_' p{1}], '_RED'), ['_' lower(p{1}) '_'], '_red_');
            if strcmp(code, normalise(copies{k, 2}))
                result.text{end + 1} = sprintf('same code: %s', block);
            else
                result.ok = false;
                result.text{end + 1} = sprintf( ...
                    'DIFFERENT code: %s (update the copy in spot.control)', block);
            end

            % The mixer blocks must read their own platform's workspace variables (until
            % 2026-10-09 BLACK's and BLUE's read RED's thruster offsets and thrust).
            if endsWith(block, 'MATLAB Function5') ...
                    || endsWith(block, 'Generate Initial Guess/MATLAB Function')
                data = charts(hit).find('-isa', 'Stateflow.Data', 'Scope', 'Parameter');
                names = get(data, 'Name');
                if ischar(names)
                    names = {names};
                end
                if endsWith(block, 'MATLAB Function5')
                    wiring.(p{1}) = names;
                end
                others = setdiff(platforms, p);
                foreign = names(contains(names, others) ...
                                | contains(names, strcat('_', lower(others), '_')));
                if ~isempty(foreign)
                    result.ok = false;
                    result.text{end + 1} = sprintf( ...
                        'WRONG parameters: %s reads %s, another platform''s', block, ...
                        strjoin(sort(foreign(:)).', ', '));
                end
            end
        end
    end
end

function c = verbatim(file)
    % {block path, code} for each "% ---- verbatim: <block>" ... "% ---- end verbatim" section.
    t = regexp(fileread(file), '% ---- verbatim: ([^\n]*)\n(.*?)\n% ---- end verbatim', 'tokens');
    c = vertcat(t{:});
end

function s = normalise(s)
    % Code compared without trailing whitespace or Windows line ends.
    s = strtrim(regexprep(strrep(char(s), sprintf('\r\n'), newline), '[ \t]+(\n|$)', '$1'));
end

%% 2. Diagram

function result = checkDiagram(root, diagram, model, platforms, wiring)
    % Simulate the diagram for about 70 s, then recompute each step with the two functions.
    LQR_REL_TOL = 1e-9;      % relative to the largest force
    DUTY_TOL = 1e-6;
    RUN_TIME_TOL = 1e-9;     % [periods] a step on the thruster code's period
    MAX_ITERS = 20;          % mixer mask values
    MIXER_TOL = 1e-6;

    %% Simulate with the defaults and a shortened timeline
    settings = spot.params.defaults();
    settings.diagram = diagram;
    settings.platforms = struct('RED', true, 'BLACK', true, 'BLUE', true, 'ARM', false);
    settings.timing.phase0 = 5;
    settings.timing.phase1 = 3;
    settings.timing.phase2 = 20;
    settings.timing.subPhases = [5 5 5 5];
    settings.timing.phase4 = 10;
    settings.timing.phase5 = 5;
    result = struct('ok', true, 'text', {{}});
    out = spot.sim.run(settings, root);

    % Logged signals, step and the thruster code's period.
    sig = spot.data.flatten(evalin('base', 'dataClass'));
    Ts = evalin('base', 'baseRate');
    period = thrusterPeriod(model, Ts);
    result.text{end + 1} = sprintf( ...
        ['simulated %g s in %.0f s (%d steps of %g s, defaults with the timeline shortened); ' ...
         'the thruster code runs every %g s'], ...
        out.tsim, out.seconds, out.samples, Ts, period);

    %% Each platform
    result.lqr = struct();
    result.mixer = struct();
    for p = platforms
        platform = p{1};

        % This platform's logged states, path, forces and duty cycles.
        signal = @(name) sig.([platform '_' name]).Data;
        measured = [signal('Px_m'), signal('Py_m'), signal('Rz_rad'), ...
                    signal('Vx_mpers'), signal('Vy_mpers'), signal('RzD_radpers')];
        desired = [signal('Px_Path_m'), signal('Py_Path_m'), signal('Rz_Path_rad')];
        u = [signal('Fx_N'), signal('Fy_N'), signal('Tz_Nm')];
        duty = squeeze(sig.(dutyName(sig, platform)).Data);
        if size(duty, 1) == 8 && size(duty, 2) ~= 8
            duty = duty.';
        end
        enabled = signal('Control_Law_Enabler') == 1;
        K = evalin('base', ['K_' platform]);

        % LQR, at steps where it ran this step and the previous one (its Discrete Derivative
        % then holds the previous step's path).
        steps = find(enabled(2:end) & enabled(1:end-1)) + 1;
        err = zeros(numel(steps), 1);
        for i = 1:numel(steps)
            k = steps(i);
            mine = spot.control.lqrCommand(K, measured(k, :), desired(k, :), ...
                (desired(k, :) - desired(k - 1, :)) / Ts);
            err(i) = max(abs(mine(:) - u(k, :).'));
        end
        result.lqr.(platform) = max([err; 0]);
        okL = ~isempty(steps) && result.lqr.(platform) <= LQR_REL_TOL * max(1, max(abs(u(:))));

        % Mixer, at the thruster code's steps, with the workspace values the block reads
        % (check 1). In between, the logged duty cycles must hold.
        names = wiring.(platform);
        dist = evalin('base', names{startsWith(names, 'thruster_dist2CG')});
        F = evalin('base', names{startsWith(names, 'F_thrusters')});
        decay = evalin('base', names{startsWith(names, 'thrust_decay')});
        t = sig.([platform '_Px_m']).Time;
        runs = find(abs(t / period - round(t / period)) < RUN_TIME_TOL);
        errD = zeros(numel(runs), 1);
        for i = 1:numel(runs)
            k = runs(i);
            mine = spot.control.thrusterDutyCycles(u(k, :), measured(k, 3), dist, F, ...
                MAX_ITERS, MIXER_TOL, decay);
            errD(i) = max(abs(mine(:) - duty(k, :).'));
        end
        between = setdiff(2:size(duty, 1), runs);
        held = all(all(duty(between, :) == duty(between - 1, :)));
        result.mixer.(platform) = max([errD; 0]);
        okD = ~isempty(runs) && result.mixer.(platform) <= DUTY_TOL && held;

        % Report.
        result.text{end + 1} = sprintf( ...
            ['%s: LQR forces at %d steps, largest difference %.2g N; duty cycles at %d ' ...
             'steps, largest difference %.2g%s'], ...
            platform, numel(steps), result.lqr.(platform), numel(runs), ...
            result.mixer.(platform), ternary(held, '', ' (and they DON''T hold in between)'));
        result.ok = result.ok && okL && okD;
    end
end

function name = dutyName(sig, platform)
    % The logged duty cycle signal's name for a platform.
    f = fieldnames(sig);
    f = f(startsWith(f, [platform '_Duty_Cycle']));
    if isempty(f)
        error('spot:control:verifyConsoleController', ...
            'The diagram logs no %s_Duty_Cycle.', platform);
    end
    name = f{1};
end

%% 3. Python

function [result, py] = checkPython(root, n)
    % Random gains, LQR and mixer cases through MATLAB and control.py.
    SEED = 20261009;
    N_MODELS = 30;           % random models for the gains
    GAIN_REL_TOL = 1e-9;
    LQR_TOL = 1e-12;
    MIXER_TOL = 1e-4;
    MIXER_WARN = 1e-6;       % above this, explain the mixer's rounding
    TABLE_X = Constants.TABLE_X;                       % [m] random poses on the table
    TABLE_Y = Constants.TABLE_Y;                       % [m]
    MAX_ITERATIONS = Constants.MIXER_MAX_ITERATIONS;   % the mixer mask's defaults
    TOLERANCE = Constants.MIXER_TOLERANCE;

    rng(SEED, 'twister');
    inputs = struct('gains', [], 'lqr', [], 'mixer', []);
    settings = spot.params.defaults();
    mp = spot.massprops.compute(settings.mass);
    names = {'RED', 'BLACK', 'BLUE'};

    %% Cases
    % Gains: random mass, inertia, step and weights.
    gainCases = cell(1, N_MODELS);
    Kref = cell(1, N_MODELS);
    for k = 1:N_MODELS
        testCase = struct('mass', 5 + 15 * rand, 'inertia', 0.1 + 0.4 * rand, ...
                          'Ts', 0.02 * randi(5), ...
                          'Q', 10 .^ (4 * rand(1, 6) - 2), 'R', 10 .^ (3 * rand(1, 3) - 1));
        gainCases{k} = testCase;
        Kref{k} = spot.control.lqrGains( ...
            spot.control.stateSpace(testCase.mass, testCase.inertia, testCase.Ts), ...
            testCase.Q, testCase.R);
    end
    inputs.gains = [gainCases{:}];

    % LQR: scaled gains from those, random states and targets.
    lqrCases = cell(1, n);
    uref = zeros(3, n);
    for k = 1:n
        K = Kref{randi(N_MODELS)} .* (0.5 + rand(3, 6));
        testCase = struct('K', K, ...
                          'measured', [TABLE_X * rand, TABLE_Y * rand, 8 * rand - 4, ...
                                       randn(1, 3) * 0.1], ...
                          'desired', [TABLE_X * rand, TABLE_Y * rand, 8 * rand - 4], ...
                          'rate', randn(1, 3) * 0.2);
        lqrCases{k} = testCase;
        uref(:, k) = spot.control.lqrCommand(testCase.K, testCase.measured, ...
            testCase.desired, testCase.rate);
    end
    inputs.lqr = [lqrCases{:}];

    % Mixer: random commands (some zero), attitudes, offsets, per-thruster thrusts and decay.
    mixerCases = cell(1, n);
    dref = zeros(8, n);
    for k = 1:n
        props = mp.(names{randi(3)});
        F = 0.2 + 0.2 * rand(1, 8);
        testCase = struct( ...
            'u', [0.8 * (2 * rand - 1), 0.8 * (2 * rand - 1), 0.15 * (2 * rand - 1)] ...
                 .* (rand > 0.2), ...
            'theta', 4 * pi * rand - 2 * pi, 'dist', props.thrusters(:).' + randn(1, 8), ...
            'F', F, 'max_iters', MAX_ITERATIONS, 'tol', TOLERANCE, 'decay', 0.2 * rand);
        mixerCases{k} = testCase;
        dref(:, k) = spot.control.thrusterDutyCycles(testCase.u, testCase.theta, testCase.dist, ...
            testCase.F, testCase.max_iters, testCase.tol, testCase.decay);
    end
    inputs.mixer = [mixerCases{:}];

    %% Python's answers
    py = runPython(root, inputs);
    result = struct('ok', true, 'text', {{}});

    % Gains: N_MODELS x 3 x 6 from JSON, as 3x6 matrices.
    G = py.gains;
    G = arrayfun(@(k) reshape(G(k, :, :), 3, 6), (1:size(G, 1)).', 'UniformOutput', false);
    eK = max(cellfun(@(a, b) max(abs(a(:) - b(:))) / max(abs(b(:))), G, Kref(:)));

    % Forces: n x 3 from JSON.
    uPy = py.lqr.';
    eU = max(abs(uPy(:) - uref(:)) ./ max(1, abs(uref(:))));

    % Duty cycles.
    dPy = [py.mixer.duty];
    eD = abs(dPy - dref);

    %% Report
    result.gains = eK;
    result.lqr = eU;
    result.mixer = max(eD(:));
    result.text{end + 1} = sprintf(['LQR gains (dlqr), 30 random models and weights: largest ' ...
                                    'relative difference %.2g'], eK);
    result.text{end + 1} = sprintf('LQR forces, %d random states: largest difference %.2g', ...
        n, eU);
    result.text{end + 1} = sprintf(['duty cycles, %d random commands, attitudes and thrusts: ' ...
                                    'largest difference %.2g (%d of %d cases above 1e-6)'], ...
        n, result.mixer, nnz(max(eD, [], 1) > MIXER_WARN), n);
    result.ok = eK <= GAIN_REL_TOL && eU <= LQR_TOL && result.mixer <= MIXER_TOL;
    if result.mixer > MIXER_WARN && result.ok
        result.text{end + 1} = ['   (the mixer iterates and compares numbers; where MATLAB and ' ...
                                'Python round a sum differently it can take another branch and ' ...
                                'settle a little differently)'];
    end
end

function out = runPython(root, inputs)
    % Run control.py on the cases (JSON in, JSON out).
    script = fullfile(root, '+spot', '+console', 'webapp', 'spot_console', 'control.py');

    % Temporary files, deleted on return.
    inFile = [tempname '.json'];
    outFile = [tempname '.json'];
    cleanup = onCleanup(@() cellfun(@(f) deleteIfThere(f), {inFile, outFile}));

    % Write the cases.
    fid = fopen(inFile, 'w');
    fwrite(fid, jsonencode(inputs), 'char');
    fclose(fid);

    % Run and read the answers.
    py = spot.console.findPython();
    [status, msg] = system(sprintf('%s "%s" < "%s" > "%s"', py, script, inFile, outFile));
    if status ~= 0
        error('spot:control:verifyConsoleController', 'control.py failed: %s', msg);
    end
    out = jsondecode(fileread(outFile));
end

function deleteIfThere(f)
    % Delete a file if it exists.
    if isfile(f)
        delete(f);
    end
end

%% 4. Defaults

function result = checkDefaults(model, py)
    % control.py's defaults against SPOT's settings and the diagram.
    REL_TOL = 1e-12;
    GAIN_REL_TOL = 1e-9;

    settings = spot.params.defaults();
    mp = spot.massprops.compute(settings.mass);
    d = py.defaults;
    result = struct('ok', true, 'text', {{}});
    same = @(a, b) isequal(size(a(:)), size(b(:))) ...
                   && all(abs(a(:) - b(:)) <= REL_TOL * max(1, abs(b(:))));

    %% Rates and the thruster code's period
    checks = {};
    checks(end + 1, :) = {same(d.sample_rate, settings.sampleRate), ...
                          sprintf('sample rate %g Hz', settings.sampleRate)};
    checks(end + 1, :) = {same(d.data_rate, settings.dataRate), ...
                          sprintf('PhaseSpace data rate %g Hz', settings.dataRate)};
    base = 1 / settings.sampleRate;
    period = thrusterPeriod(model, base);
    checks(end + 1, :) = {same(period, base) && same(1 / d.sample_rate, base), ...
                          sprintf(['the diagram''s thruster code runs every %g s and the ' ...
                                   'console''s loop every %g s; both should be baseRate, %g s'], ...
                                  period, 1 / d.sample_rate, base)};

    %% Mixer mask
    % The mask's values are numbers or expressions (Constants.MIXER_MAX_ITERATIONS, ...).
    mixer = [model '/Thruster & Float' newline ...
             'Control Code/Change RED Behavior/Calculate Duty Cycle/MATLAB Function5'];
    maxIters = maskValue(mixer, 'max_iters');
    tol = maskValue(mixer, 'tol');
    checks(end + 1, :) = {same(d.max_iters, maxIters) && same(d.tol, tol), ...
                          sprintf('mixer mask: %g iterations, tolerance %g', maxIters, tol)};

    %% Each platform's mass properties, thrust, weights and gain
    for p = {'RED', 'BLACK', 'BLUE'}
        platform = p{1};
        q = d.platforms.(platform);
        ok = same(q.mass, mp.(platform).mass) ...
             && same(q.inertia, mp.(platform).inertia) ...
             && same(q.thrust, settings.thrusterForce.(platform)) ...
             && same(q.thrust_decay, settings.thrusterDecay.(platform)) ...
             && same(q.thruster_dist2CG, mp.(platform).thrusters) ...
             && same(q.Q, settings.control.(platform).lqr.Q) ...
             && same(q.R, settings.control.(platform).lqr.R);
        K = spot.control.lqrGains( ...
            spot.control.stateSpace(mp.(platform).mass, mp.(platform).inertia, ...
                                    1 / settings.dataRate), ...
            settings.control.(platform).lqr.Q, settings.control.(platform).lqr.R);
        Kpy = py.default_gains.(platform);
        eK = max(abs(Kpy(:) - K(:))) / max(abs(K(:)));
        checks(end + 1, :) = {ok && eK <= GAIN_REL_TOL, ...
                              sprintf(['%s: mass %.4g kg, inertia %.4g kg m^2, thrust %g N, ' ...
                                       'thrust decay %g per valve, thruster offsets, Q and R; ' ...
                                       'its gain K_%s differs by %.2g (relative)'], ...
                                      platform, q.mass, q.inertia, q.thrust, q.thrust_decay, ...
                                      platform, eK)};
    end

    %% Report
    for k = 1:size(checks, 1)
        [ok, text] = checks{k, :};
        if ok
            result.text{end + 1} = ['same: ' text];
        else
            result.text{end + 1} = ['DIFFERENT: ' text ' (update DEFAULTS in control.py)'];
            result.ok = false;
        end
    end
end

%% Helpers

function period = thrusterPeriod(model, baseRate)
    % How often the diagram's "Thruster & Float Control Code" runs [s]: its own sample time if
    % it is atomic (Simulink ignores a virtual subsystem's), else baseRate.
    blk = [model '/Thruster & Float' newline 'Control Code'];
    value = strtrim(get_param(blk, 'SystemSampleTime'));
    if strcmp(get_param(blk, 'TreatAsAtomicUnit'), 'off') || any(strcmp(value, {'-1', 'baseRate'}))
        period = baseRate;
    else
        period = str2double(value);

        % Another workspace variable, e.g. dataRate.
        if isnan(period)
            period = evalin('base', value);
        end
    end
end

function v = maskValue(block, name)
    % A mask parameter's value [number]: the text as a number, else evaluated where the block
    % can see it (an expression such as Constants.MIXER_TOLERANCE, or a workspace variable).
    text = strtrim(get_param(block, name));
    v = str2double(text);
    if isnan(v)
        try
            v = slResolve(text, block);
        catch
            v = evalin('base', text);
        end
    end
end

function v = ternary(c, a, b)
    % a if c, else b.
    if c
        v = a;
    else
        v = b;
    end
end

function name = defaultDiagram(root)
    % The settings' diagram, else the first .slx in the project.
    file = fullfile(root, 'spot_settings.json');
    name = '';
    if isfile(file)
        settings = spot.params.load(file);
        name = settings.diagram;
    end
    if isempty(name) || ~isfile(fullfile(root, name))
        names = spot.sim.listDiagrams(root);
        if isempty(names)
            error('spot:control:verifyConsoleController', ...
                'There is no diagram (.slx) in %s.', root);
        end
        name = names{1};
    end
end
