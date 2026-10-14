function report = verifyConsoleController(varargin)
%VERIFYCONSOLECONTROLLER Check the lab console's Interactive-mode controller against the diagram.
%   spot.control.verifyConsoleController() runs four checks and prints what each found:
%     1. Code      the verbatim copies of the diagram's block code in spot.control.lqrCommand and
%                  spot.control.thrusterDutyCycles are still the code of the diagram's blocks (for
%                  RED, BLACK and BLUE), and each platform's mixer reads its own parameters
%     2. Diagram   in a short simulation of the diagram (RED, BLACK and BLUE), the two functions
%                  give, step by step, the forces (*_Controls) and duty cycles (*_Duty_Cycle) the
%                  diagram itself logged (the duty cycles at the steps where its "Thruster & Float
%                  Control Code" runs, which hold them in between)
%                  This check needs a diagram whose step order is fixed (atomic subsystems with
%                  priorities, as in the template's diagram; SPOT_Tutorial section 6). In the earlier
%                  template diagram (archive/Legacy/original_diagram) the controller read the previous step's
%                  measurements, so it fails there although the code is the same.
%     3. Python    control.py, which the lab console runs (+spot/+console/webapp/spot_console), gives
%                  the same LQR gains, forces and duty cycles as MATLAB, for random cases
%     4. Defaults  control.py's parameters are SPOT's default settings (spot.params.defaults,
%                  spot.massprops.compute), the mixer block's mask values and the thruster code's
%                  sample time (it must be baseRate, the console's loop's step)
%   report = spot.control.verifyConsoleController(...) also returns them: report.<check> has ok
%   (true, false, or [] when skipped), text (one line per finding) and the numbers behind them.
%
%   Options (name, value):
%     'diagram'   the .slx in the project folder to check (default: the one in spot_settings.json,
%                 else the first in the folder)
%     'simulate'  false to skip check 2 (it simulates about 70 s of the diagram)
%     'cases'     random cases for check 3 (default 300)
%
%   Needs Simulink and the Control System Toolbox (as simulations do) and Python 3.6+ (as the
%   console does). Nothing is saved: the diagram is loaded but not changed.
%
%   See also spot.control.lqrCommand, spot.control.thrusterDutyCycles.
opt = struct('diagram', '', 'simulate', true, 'cases', 300);
for k = 1:2:numel(varargin)
    opt.(varargin{k}) = varargin{k + 1};
end
root = fileparts(fileparts(fileparts(mfilename('fullpath'))));   % the project folder, above +spot
if isempty(opt.diagram)
    opt.diagram = defaultDiagram(root);
end
model = spot.sim.loadDiagram(opt.diagram, root);
platforms = {'RED', 'BLACK', 'BLUE'};

report = struct();
[report.code, wiring] = checkCode(model, platforms);
if opt.simulate
    report.diagram = checkDiagram(root, opt.diagram, model, platforms, wiring);
else
    report.diagram = struct('ok', [], 'text', {{'skipped (''simulate'', false)'}});
end
[report.python, py] = checkPython(root, opt.cases);
report.defaults = checkDefaults(model, py);

fprintf('\nInteractive Mode''s controller, checked against %s\n', opt.diagram);
titles = struct('code', '1. Code', 'diagram', '2. Diagram', 'python', '3. Python', 'defaults', '4. Defaults');
for f = fieldnames(titles).'
    r = report.(f{1});
    if isempty(r.ok)
        mark = '-';
    elseif r.ok
        mark = 'OK';
    else
        mark = 'FAILED';
    end
    fprintf('\n%s: %s\n', titles.(f{1}), mark);
    fprintf('   %s\n', r.text{:});
end
fprintf('\n');
if nargout == 0
    clear report
end
end

% ============================================================================ 1. code

function [r, wiring] = checkCode(model, platforms)
% The verbatim sections of the two functions against the blocks' MATLAB Function code
here = fileparts(mfilename('fullpath'));
copies = [verbatim(fullfile(here, 'lqrCommand.m')); verbatim(fullfile(here, 'thrusterDutyCycles.m'))];
charts = find(sfroot, '-isa', 'Stateflow.EMChart');
paths = strrep(get(charts, 'Path'), newline, ' ');
if ischar(paths)
    paths = {paths};
end
r = struct('ok', true, 'text', {{}});
wiring = struct();
for k = 1:size(copies, 1)
    for p = platforms
        block = strrep(copies{k, 1}, '(RED)', ['(' p{1} ')']);
        block = strrep(block, 'Change RED Behavior', ['Change ' p{1} ' Behavior']);
        hit = strcmp(paths, [model '/' block]);
        if ~any(hit)
            r.ok = false;
            r.text{end + 1} = sprintf('%s: block not found in the diagram', block);
            continue
        end
        code = normalise(charts(hit).Script);
        % BLACK's and BLUE's copies name their own parameters (thruster_dist2CG_BLACK,
        % F_thrusters_BLACK, ...)
        code = strrep(strrep(code, ['_' p{1}], '_RED'), ['_' lower(p{1}) '_'], '_red_');
        if strcmp(code, normalise(copies{k, 2}))
            r.text{end + 1} = sprintf('same code: %s', block);
        else
            r.ok = false;
            r.text{end + 1} = sprintf('DIFFERENT code: %s (update the copy in spot.control)', block);
        end
        % The mixer blocks must read their own platform's workspace variables (until 2026-10-09
        % BLACK's and BLUE's optimisers read RED's thruster offsets and thrust)
        if endsWith(block, 'MATLAB Function5') || endsWith(block, 'Generate Initial Guess/MATLAB Function')
            data = charts(hit).find('-isa', 'Stateflow.Data', 'Scope', 'Parameter');
            names = get(data, 'Name');
            if ischar(names)
                names = {names};
            end
            if endsWith(block, 'MATLAB Function5')
                wiring.(p{1}) = names;
            end
            others = setdiff(platforms, p);
            foreign = names(contains(names, others) | contains(names, strcat('_', lower(others), '_')));
            if ~isempty(foreign)
                r.ok = false;
                r.text{end + 1} = sprintf('WRONG parameters: %s reads %s, another platform''s', block, ...
                    strjoin(sort(foreign(:)).', ', '));
            end
        end
    end
end
end

function c = verbatim(file)
% {block path, code} for each "% ---- verbatim: <block>" ... "% ---- end verbatim" section
t = regexp(fileread(file), '% ---- verbatim: ([^\n]*)\n(.*?)\n% ---- end verbatim', 'tokens');
c = vertcat(t{:});
end

function s = normalise(s)
s = strtrim(regexprep(strrep(char(s), sprintf('\r\n'), newline), '[ \t]+(\n|$)', '$1'));
end

% ============================================================================ 2. diagram

function r = checkDiagram(root, diagram, model, platforms, wiring)
% Simulate the diagram for about 70 s, then recompute each step with the two functions
s = spot.params.defaults();
s.diagram = diagram;
s.platforms = struct('RED', true, 'BLACK', true, 'BLUE', true, 'ARM', false);
s.timing.phase0 = 5; s.timing.phase1 = 3; s.timing.phase2 = 20;
s.timing.subPhases = [5 5 5 5]; s.timing.phase4 = 10; s.timing.phase5 = 5;
r = struct('ok', true, 'text', {{}});
out = spot.sim.run(s, root);
sig = spot.data.flatten(evalin('base', 'dataClass'));
Ts = evalin('base', 'baseRate');
period = thrusterPeriod(model, Ts);
r.text{end + 1} = sprintf(['simulated %g s in %.0f s (%d steps of %g s, defaults with the timeline ' ...
    'shortened); the thruster code runs every %g s'], out.tsim, out.seconds, out.samples, Ts, period);
r.lqr = struct(); r.mixer = struct();
for p = platforms
    n = p{1};
    get1 = @(name) sig.([n '_' name]).Data;
    measured = [get1('Px_m'), get1('Py_m'), get1('Rz_rad'), get1('Vx_mpers'), get1('Vy_mpers'), get1('RzD_radpers')];
    path = [get1('Px_Path_m'), get1('Py_Path_m'), get1('Rz_Path_rad')];
    u = [get1('Fx_N'), get1('Fy_N'), get1('Tz_Nm')];
    duty = squeeze(sig.(dutyName(sig, n)).Data);
    if size(duty, 1) == 8 && size(duty, 2) ~= 8
        duty = duty.';
    end
    enabled = get1('Control_Law_Enabler') == 1;
    K = evalin('base', ['K_' n]);

    % The LQR, at steps where it ran this step and the previous one (its Discrete Derivative
    % then holds the previous step's path)
    steps = find(enabled(2:end) & enabled(1:end-1)) + 1;
    err = zeros(numel(steps), 1);
    for i = 1:numel(steps)
        k = steps(i);
        mine = spot.control.lqrCommand(K, measured(k, :), path(k, :), (path(k, :) - path(k - 1, :)) / Ts);
        err(i) = max(abs(mine(:) - u(k, :).'));
    end
    r.lqr.(n) = max([err; 0]);
    okL = ~isempty(steps) && r.lqr.(n) <= 1e-9 * max(1, max(abs(u(:))));

    % The mixer, at the thruster code's steps, with the workspace values the block reads (see
    % check 1); in between, the logged duty cycles must hold
    names = wiring.(n);
    dist = evalin('base', names{startsWith(names, 'thruster_dist2CG')});
    F = evalin('base', names{startsWith(names, 'F_thrusters')});   % each thruster's (calibrated or nominal)
    t = sig.([n '_Px_m']).Time;
    runs = find(abs(t / period - round(t / period)) < 1e-9);
    errD = zeros(numel(runs), 1);
    for i = 1:numel(runs)
        k = runs(i);
        mine = spot.control.thrusterDutyCycles(u(k, :), measured(k, 3), dist, F);
        errD(i) = max(abs(mine(:) - duty(k, :).'));
    end
    between = setdiff(2:size(duty, 1), runs);
    held = all(all(duty(between, :) == duty(between - 1, :)));
    r.mixer.(n) = max([errD; 0]);
    okD = ~isempty(runs) && r.mixer.(n) <= 1e-6 && held;
    r.text{end + 1} = sprintf(['%s: LQR forces at %d steps, largest difference %.2g N; ' ...
        'duty cycles at %d steps, largest difference %.2g%s'], n, numel(steps), r.lqr.(n), numel(runs), ...
        r.mixer.(n), ternary(held, '', ' (and they DON''T hold in between)'));
    r.ok = r.ok && okL && okD;
end
end

function name = dutyName(sig, platform)
f = fieldnames(sig);
f = f(startsWith(f, [platform '_Duty_Cycle']));
if isempty(f)
    error('spot:control:verifyConsoleController', 'The diagram logs no %s_Duty_Cycle.', platform);
end
name = f{1};
end

% ============================================================================ 3. python

function [r, py] = checkPython(root, n)
rng(20261009, 'twister');
in = struct('gains', [], 'lqr', [], 'mixer', []);
s = spot.params.defaults();
mp = spot.massprops.compute(s.mass);
names = {'RED', 'BLACK', 'BLUE'};

g = cell(1, 30);
Kref = cell(1, 30);
for k = 1:30
    c = struct('mass', 5 + 15 * rand, 'inertia', 0.1 + 0.4 * rand, 'Ts', 0.02 * randi(5), ...
               'Q', 10 .^ (4 * rand(1, 6) - 2), 'R', 10 .^ (3 * rand(1, 3) - 1));
    g{k} = c;
    Kref{k} = spot.control.lqrGains(spot.control.stateSpace(c.mass, c.inertia, c.Ts), c.Q, c.R);
end
in.gains = [g{:}];

l = cell(1, n);
uref = zeros(3, n);
for k = 1:n
    K = Kref{randi(30)} .* (0.5 + rand(3, 6));
    c = struct('K', K, 'measured', [3.5 * rand, 2.4 * rand, 8 * rand - 4, randn(1, 3) * 0.1], ...
               'desired', [3.5 * rand, 2.4 * rand, 8 * rand - 4], 'rate', randn(1, 3) * 0.2);
    l{k} = c;
    uref(:, k) = spot.control.lqrCommand(c.K, c.measured, c.desired, c.rate);
end
in.lqr = [l{:}];

m = cell(1, n);
dref = zeros(8, n);
for k = 1:n
    p = mp.(names{randi(3)});
    F = 0.2 + 0.2 * rand(1, 8);   % each thruster its own thrust, as a thruster calibration gives them
    c = struct('u', [0.8 * (2 * rand - 1), 0.8 * (2 * rand - 1), 0.15 * (2 * rand - 1)] .* (rand > 0.2), ...
               'theta', 4 * pi * rand - 2 * pi, 'dist', p.thrusters(:).' + randn(1, 8), ...
               'F', F, 'max_iters', 20, 'tol', 1e-6);
    m{k} = c;
    dref(:, k) = spot.control.thrusterDutyCycles(c.u, c.theta, c.dist, c.F, c.max_iters, c.tol);
end
in.mixer = [m{:}];

py = runPython(root, in);
r = struct('ok', true, 'text', {{}});
G = py.gains;   % 30 x 3 x 6 from JSON
G = arrayfun(@(k) reshape(G(k, :, :), 3, 6), (1:size(G, 1)).', 'UniformOutput', false);
eK = max(cellfun(@(a, b) max(abs(a(:) - b(:))) / max(abs(b(:))), G, Kref(:)));
uPy = py.lqr.';   % n x 3 from JSON
eU = max(abs(uPy(:) - uref(:)) ./ max(1, abs(uref(:))));
dPy = [py.mixer.duty];
eD = abs(dPy - dref);
r.gains = eK; r.lqr = eU; r.mixer = max(eD(:));
r.text{end + 1} = sprintf('LQR gains (dlqr), 30 random models and weights: largest relative difference %.2g', eK);
r.text{end + 1} = sprintf('LQR forces, %d random states: largest difference %.2g', n, eU);
r.text{end + 1} = sprintf(['duty cycles, %d random commands, attitudes and thrusts: largest difference %.2g ' ...
    '(%d of %d cases above 1e-6)'], n, r.mixer, nnz(max(eD, [], 1) > 1e-6), n);
r.ok = eK <= 1e-9 && eU <= 1e-12 && r.mixer <= 1e-4;
if r.mixer > 1e-6 && r.ok
    r.text{end + 1} = ['   (the mixer iterates and compares numbers; where MATLAB and Python round a ' ...
        'sum differently it can take another branch and settle a little differently)'];
end
end

function out = runPython(root, in)
script = fullfile(root, '+spot', '+console', 'webapp', 'spot_console', 'control.py');
inFile = [tempname '.json'];
outFile = [tempname '.json'];
cleanup = onCleanup(@() cellfun(@(f) deleteIfThere(f), {inFile, outFile}));
fid = fopen(inFile, 'w');
fwrite(fid, jsonencode(in), 'char');
fclose(fid);
py = spot.console.findPython();
[status, msg] = system(sprintf('%s "%s" < "%s" > "%s"', py, script, inFile, outFile));
if status ~= 0
    error('spot:control:verifyConsoleController', 'control.py failed: %s', msg);
end
out = jsondecode(fileread(outFile));
end

function deleteIfThere(f)
if isfile(f)
    delete(f);
end
end

% ============================================================================ 4. defaults

function r = checkDefaults(model, py)
s = spot.params.defaults();
mp = spot.massprops.compute(s.mass);
d = py.defaults;
r = struct('ok', true, 'text', {{}});
same = @(a, b) isequal(size(a(:)), size(b(:))) && all(abs(a(:) - b(:)) <= 1e-12 * max(1, abs(b(:))));
checks = {};
checks(end + 1, :) = {same(d.sample_rate, s.sampleRate), sprintf('sample rate %g Hz', s.sampleRate)};
checks(end + 1, :) = {same(d.data_rate, s.dataRate), sprintf('PhaseSpace data rate %g Hz', s.dataRate)};
base = 1 / s.sampleRate;
period = thrusterPeriod(model, base);
checks(end + 1, :) = {same(period, base) && same(1 / d.sample_rate, base), sprintf(['the diagram''s thruster ' ...
    'code runs every %g s and the console''s loop every %g s; both should be baseRate, %g s'], period, ...
    1 / d.sample_rate, base)};
mixer = [model '/Thruster & Float' newline 'Control Code/Change RED Behavior/Calculate Duty Cycle/MATLAB Function5'];
maxIters = str2double(get_param(mixer, 'max_iters'));
tol = str2double(get_param(mixer, 'tol'));
checks(end + 1, :) = {same(d.max_iters, maxIters) && same(d.tol, tol), ...
    sprintf('mixer mask: %g iterations, tolerance %g', maxIters, tol)};
for p = {'RED', 'BLACK', 'BLUE'}
    n = p{1};
    q = d.platforms.(n);
    ok = same(q.mass, mp.(n).mass) && same(q.inertia, mp.(n).inertia) && same(q.thrust, s.thrusterForce.(n)) ...
        && same(q.thruster_dist2CG, mp.(n).thrusters) && same(q.Q, s.control.(n).lqr.Q) && same(q.R, s.control.(n).lqr.R);
    K = spot.control.lqrGains(spot.control.stateSpace(mp.(n).mass, mp.(n).inertia, 1 / s.dataRate), ...
                              s.control.(n).lqr.Q, s.control.(n).lqr.R);
    Kpy = py.default_gains.(n);
    eK = max(abs(Kpy(:) - K(:))) / max(abs(K(:)));
    checks(end + 1, :) = {ok && eK <= 1e-9, sprintf(['%s: mass %.4g kg, inertia %.4g kg m^2, thrust %g N, ' ...
        'thruster offsets, Q and R; its gain K_%s differs by %.2g (relative)'], n, q.mass, q.inertia, q.thrust, n, eK)};
end
for k = 1:size(checks, 1)
    [ok, text] = checks{k, :};
    if ok
        r.text{end + 1} = ['same: ' text];
    else
        r.text{end + 1} = ['DIFFERENT: ' text ' (update DEFAULTS in control.py)'];
        r.ok = false;
    end
end
end

% ============================================================================ helpers

function p = thrusterPeriod(model, baseRate)
% How often the diagram's "Thruster & Float Control Code" runs [s]: its own sample time if it is
% atomic (Simulink ignores a virtual subsystem's), else the diagram's step, baseRate
blk = [model '/Thruster & Float' newline 'Control Code'];
value = strtrim(get_param(blk, 'SystemSampleTime'));
if strcmp(get_param(blk, 'TreatAsAtomicUnit'), 'off') || any(strcmp(value, {'-1', 'baseRate'}))
    p = baseRate;
else
    p = str2double(value);
    if isnan(p)
        p = evalin('base', value);   % another workspace variable, e.g. dataRate
    end
end
end

function v = ternary(c, a, b)
if c
    v = a;
else
    v = b;
end
end

function name = defaultDiagram(root)
file = fullfile(root, 'spot_settings.json');
name = '';
if isfile(file)
    s = spot.params.load(file);
    name = s.diagram;
end
if isempty(name) || ~isfile(fullfile(root, name))
    names = spot.sim.listDiagrams(root);
    if isempty(names)
        error('spot:control:verifyConsoleController', 'There is no diagram (.slx) in %s.', root);
    end
    name = names{1};
end
end
