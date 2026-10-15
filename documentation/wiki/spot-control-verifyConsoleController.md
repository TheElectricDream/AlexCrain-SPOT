# spot.control.verifyConsoleController

**File:** `+spot/+control/verifyConsoleController.m` · **Package:** [[spot.control|spot-control]]

<!-- source: +spot/+control/verifyConsoleController.m -->

## What it is for

The same controller exists three times: in the diagram's blocks, as the MATLAB copies [[lqrCommand|spot-control-lqrCommand]] / [[thrusterDutyCycles|spot-control-thrusterDutyCycles]], and in Python for the lab console's Interactive Mode ([[control.py|Console-control]]). This runs four checks that they agree, and prints a report:

1. **Code**: the verbatim sections in the two `.m` files are still the diagram's block code, for RED, BLACK and BLUE, and each platform's mixer reads its *own* parameters (until 2026-10-09 BLACK's and BLUE's read RED's).
2. **Diagram**: a short simulation; step by step, the two functions reproduce the forces and duty cycles the diagram logged.
3. **Python**: `control.py` gives the same gains, forces and duty cycles as MATLAB for hundreds of random cases (300 random commands, attitudes and thrusts for the mixer by default).
4. **Defaults**: `control.py`'s built-in parameters equal SPOT's defaults, the mixer block's mask values and the thruster code's sample time (`baseRate`).

## How to call it

```matlab
spot.control.verifyConsoleController()                    % prints the report
r = spot.control.verifyConsoleController('simulate', false, 'cases', 100);
r.code.ok, r.diagram.ok, r.python.ok, r.defaults.ok       % true / false / [] (skipped)
```

Needs Simulink, the Control System Toolbox and Python 3.6+.

## Line by line

**Lines 33–42.** Options from name/value pairs; the project folder is three levels above this file; the diagram is the settings' or the first in the folder (`defaultDiagram`, lines 370–384); load it.

**Lines 44–52.** Run the four checks (check 2 only if `simulate`), check 3 also returns the Python results for check 4.

**Lines 54–71.** Print each check's title, OK / FAILED / `-`, and its findings; return nothing if not asked for.

**Lines 76–128, check 1.** `verbatim` (130–134) pulls each `% ---- verbatim: <block>` … `% ---- end verbatim` section (block path + code) from the two files. All MATLAB Function charts of the loaded model are listed with their paths (block names may contain line breaks, replaced by spaces). For each copy and each platform, the block path is adapted (`(RED)` → `(BLACK)`, `Change RED Behavior` → …), the chart is found (missing → fail), and its script, with that platform's parameter names turned into RED's, is compared after `normalise` (136–138: line endings and trailing spaces). For the two mixer blocks, the chart's *parameters* (Stateflow data with scope Parameter) are listed: the `MATLAB Function5` names are kept in `wiring` for check 2, and any name mentioning another platform is a failure.

**Lines 142–204, check 2.** Defaults with a short timeline (5+3+20+4×5+10+5 = 63 s) and the three platforms; run it; flatten the logged data; `Ts = baseRate`; the thruster code's period (`thrusterPeriod`, 347–360). Per platform: the measured state, path, forces and duty cycles from the log, and when the control law was enabled. The LQR is checked at steps where it ran this step and the last (so its derivative had a previous path): recompute with `lqrCommand` and keep the largest difference (tolerance 1e-9 relative). The mixer is checked at the steps where the thruster code runs (multiples of its period), with the workspace values the block actually reads (from `wiring`: `thruster_dist2CG_<PLATFORM>` and `F_thrusters_<PLATFORM>`, the calibrated or nominal thrusts) and `thrusterDutyCycles(u, theta, dist, F)`, tolerance 1e-6, and in between the logged duty cycles must hold their value.

**Lines 217–277, check 3.** Seeded random cases: 30 models/weights for the gains, `n` states for the LQR, `n` commands/attitudes/offsets/thrusts for the mixer (each thruster its own thrust, 0.2–0.4 N, in the `F` field), with MATLAB's answers. `runPython` (279–293) writes them to a JSON file, runs `control.py` with it on stdin and reads its JSON answer. Differences: gains relative (≤1e-9), forces (≤1e-12), duty cycles (≤1e-4, with a note that the iterative mixer can take a different branch when MATLAB and Python round a sum differently).

**Lines 303–343, check 4.** Compare `control.py`'s `defaults` section with SPOT's: sample rate, data rate, that both the diagram's thruster code and the console's loop run at `baseRate`, the mixer block's mask (`max_iters`, `tol` read from the block), and per platform the mass, inertia, thrust, thruster offsets, Q, R and the resulting gain.

**Lines 347–360.** `thrusterPeriod`: the thruster subsystem's own sample time if it is atomic and not inherited, else `baseRate`.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/control.py`, `+spot/+console/webapp/static/interactive.js`, `+spot/+control/lqrCommand.m`, `+spot/+control/thrusterDutyCycles.m`
- **Uses:** [[spot.console.findPython|spot-console-findPython]], [[spot.control.lqrCommand|spot-control-lqrCommand]], [[spot.control.lqrGains|spot-control-lqrGains]], [[spot.control.stateSpace|spot-control-stateSpace]], [[spot.control.thrusterDutyCycles|spot-control-thrusterDutyCycles]], [[spot.data.flatten|spot-data-flatten]], [[spot.massprops.compute|spot-massprops-compute]], [[spot.params.defaults|spot-params-defaults]], [[spot.params.load|spot-params-load]], [[spot.sim.listDiagrams|spot-sim-listDiagrams]], [[spot.sim.loadDiagram|spot-sim-loadDiagram]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+control/verifyConsoleController.m</b> (384 lines; click to show)</summary>

```matlab
   1  function report = verifyConsoleController(varargin)
   2  %VERIFYCONSOLECONTROLLER Check the lab console's Interactive-mode controller against the diagram.
   3  %   spot.control.verifyConsoleController() runs four checks and prints what each found:
   4  %     1. Code      the verbatim copies of the diagram's block code in spot.control.lqrCommand and
   5  %                  spot.control.thrusterDutyCycles are still the code of the diagram's blocks (for
   6  %                  RED, BLACK and BLUE), and each platform's mixer reads its own parameters
   7  %     2. Diagram   in a short simulation of the diagram (RED, BLACK and BLUE), the two functions
   8  %                  give, step by step, the forces (*_Controls) and duty cycles (*_Duty_Cycle) the
   9  %                  diagram itself logged (the duty cycles at the steps where its "Thruster & Float
  10  %                  Control Code" runs, which hold them in between)
  11  %                  This check needs a diagram whose step order is fixed (atomic subsystems with
  12  %                  priorities, as in the template's diagram; SPOT_Tutorial section 6). In the earlier
  13  %                  template diagram (archive/Legacy/original_diagram) the controller read the previous step's
  14  %                  measurements, so it fails there although the code is the same.
  15  %     3. Python    control.py, which the lab console runs (+spot/+console/webapp/spot_console), gives
  16  %                  the same LQR gains, forces and duty cycles as MATLAB, for random cases
  17  %     4. Defaults  control.py's parameters are SPOT's default settings (spot.params.defaults,
  18  %                  spot.massprops.compute), the mixer block's mask values and the thruster code's
  19  %                  sample time (it must be baseRate, the console's loop's step)
  20  %   report = spot.control.verifyConsoleController(...) also returns them: report.<check> has ok
  21  %   (true, false, or [] when skipped), text (one line per finding) and the numbers behind them.
  22  %
  23  %   Options (name, value):
  24  %     'diagram'   the .slx in the project folder to check (default: the one in spot_settings.json,
  25  %                 else the first in the folder)
  26  %     'simulate'  false to skip check 2 (it simulates about 70 s of the diagram)
  27  %     'cases'     random cases for check 3 (default 300)
  28  %
  29  %   Needs Simulink and the Control System Toolbox (as simulations do) and Python 3.6+ (as the
  30  %   console does). Nothing is saved: the diagram is loaded but not changed.
  31  %
  32  %   See also spot.control.lqrCommand, spot.control.thrusterDutyCycles.
  33  opt = struct('diagram', '', 'simulate', true, 'cases', 300);
  34  for k = 1:2:numel(varargin)
  35      opt.(varargin{k}) = varargin{k + 1};
  36  end
  37  root = fileparts(fileparts(fileparts(mfilename('fullpath'))));   % the project folder, above +spot
  38  if isempty(opt.diagram)
  39      opt.diagram = defaultDiagram(root);
  40  end
  41  model = spot.sim.loadDiagram(opt.diagram, root);
  42  platforms = {'RED', 'BLACK', 'BLUE'};
  43  
  44  report = struct();
  45  [report.code, wiring] = checkCode(model, platforms);
  46  if opt.simulate
  47      report.diagram = checkDiagram(root, opt.diagram, model, platforms, wiring);
  48  else
  49      report.diagram = struct('ok', [], 'text', {{'skipped (''simulate'', false)'}});
  50  end
  51  [report.python, py] = checkPython(root, opt.cases);
  52  report.defaults = checkDefaults(model, py);
  53  
  54  fprintf('\nInteractive Mode''s controller, checked against %s\n', opt.diagram);
  55  titles = struct('code', '1. Code', 'diagram', '2. Diagram', 'python', '3. Python', 'defaults', '4. Defaults');
  56  for f = fieldnames(titles).'
  57      r = report.(f{1});
  58      if isempty(r.ok)
  59          mark = '-';
  60      elseif r.ok
  61          mark = 'OK';
  62      else
  63          mark = 'FAILED';
  64      end
  65      fprintf('\n%s: %s\n', titles.(f{1}), mark);
  66      fprintf('   %s\n', r.text{:});
  67  end
  68  fprintf('\n');
  69  if nargout == 0
  70      clear report
  71  end
  72  end
  73  
  74  % ============================================================================ 1. code
  75  
  76  function [r, wiring] = checkCode(model, platforms)
  77  % The verbatim sections of the two functions against the blocks' MATLAB Function code
  78  here = fileparts(mfilename('fullpath'));
  79  copies = [verbatim(fullfile(here, 'lqrCommand.m')); verbatim(fullfile(here, 'thrusterDutyCycles.m'))];
  80  charts = find(sfroot, '-isa', 'Stateflow.EMChart');
  81  paths = strrep(get(charts, 'Path'), newline, ' ');
  82  if ischar(paths)
  83      paths = {paths};
  84  end
  85  r = struct('ok', true, 'text', {{}});
  86  wiring = struct();
  87  for k = 1:size(copies, 1)
  88      for p = platforms
  89          block = strrep(copies{k, 1}, '(RED)', ['(' p{1} ')']);
  90          block = strrep(block, 'Change RED Behavior', ['Change ' p{1} ' Behavior']);
  91          hit = strcmp(paths, [model '/' block]);
  92          if ~any(hit)
  93              r.ok = false;
  94              r.text{end + 1} = sprintf('%s: block not found in the diagram', block);
  95              continue
  96          end
  97          code = normalise(charts(hit).Script);
  98          % BLACK's and BLUE's copies name their own parameters (thruster_dist2CG_BLACK,
  99          % F_thrusters_BLACK, ...)
 100          code = strrep(strrep(code, ['_' p{1}], '_RED'), ['_' lower(p{1}) '_'], '_red_');
 101          if strcmp(code, normalise(copies{k, 2}))
 102              r.text{end + 1} = sprintf('same code: %s', block);
 103          else
 104              r.ok = false;
 105              r.text{end + 1} = sprintf('DIFFERENT code: %s (update the copy in spot.control)', block);
 106          end
 107          % The mixer blocks must read their own platform's workspace variables (until 2026-10-09
 108          % BLACK's and BLUE's optimisers read RED's thruster offsets and thrust)
 109          if endsWith(block, 'MATLAB Function5') || endsWith(block, 'Generate Initial Guess/MATLAB Function')
 110              data = charts(hit).find('-isa', 'Stateflow.Data', 'Scope', 'Parameter');
 111              names = get(data, 'Name');
 112              if ischar(names)
 113                  names = {names};
 114              end
 115              if endsWith(block, 'MATLAB Function5')
 116                  wiring.(p{1}) = names;
 117              end
 118              others = setdiff(platforms, p);
 119              foreign = names(contains(names, others) | contains(names, strcat('_', lower(others), '_')));
 120              if ~isempty(foreign)
 121                  r.ok = false;
 122                  r.text{end + 1} = sprintf('WRONG parameters: %s reads %s, another platform''s', block, ...
 123                      strjoin(sort(foreign(:)).', ', '));
 124              end
 125          end
 126      end
 127  end
 128  end
 129  
 130  function c = verbatim(file)
 131  % {block path, code} for each "% ---- verbatim: <block>" ... "% ---- end verbatim" section
 132  t = regexp(fileread(file), '% ---- verbatim: ([^\n]*)\n(.*?)\n% ---- end verbatim', 'tokens');
 133  c = vertcat(t{:});
 134  end
 135  
 136  function s = normalise(s)
 137  s = strtrim(regexprep(strrep(char(s), sprintf('\r\n'), newline), '[ \t]+(\n|$)', '$1'));
 138  end
 139  
 140  % ============================================================================ 2. diagram
 141  
 142  function r = checkDiagram(root, diagram, model, platforms, wiring)
 143  % Simulate the diagram for about 70 s, then recompute each step with the two functions
 144  s = spot.params.defaults();
 145  s.diagram = diagram;
 146  s.platforms = struct('RED', true, 'BLACK', true, 'BLUE', true, 'ARM', false);
 147  s.timing.phase0 = 5; s.timing.phase1 = 3; s.timing.phase2 = 20;
 148  s.timing.subPhases = [5 5 5 5]; s.timing.phase4 = 10; s.timing.phase5 = 5;
 149  r = struct('ok', true, 'text', {{}});
 150  out = spot.sim.run(s, root);
 151  sig = spot.data.flatten(evalin('base', 'dataClass'));
 152  Ts = evalin('base', 'baseRate');
 153  period = thrusterPeriod(model, Ts);
 154  r.text{end + 1} = sprintf(['simulated %g s in %.0f s (%d steps of %g s, defaults with the timeline ' ...
 155      'shortened); the thruster code runs every %g s'], out.tsim, out.seconds, out.samples, Ts, period);
 156  r.lqr = struct(); r.mixer = struct();
 157  for p = platforms
 158      n = p{1};
 159      get1 = @(name) sig.([n '_' name]).Data;
 160      measured = [get1('Px_m'), get1('Py_m'), get1('Rz_rad'), get1('Vx_mpers'), get1('Vy_mpers'), get1('RzD_radpers')];
 161      path = [get1('Px_Path_m'), get1('Py_Path_m'), get1('Rz_Path_rad')];
 162      u = [get1('Fx_N'), get1('Fy_N'), get1('Tz_Nm')];
 163      duty = squeeze(sig.(dutyName(sig, n)).Data);
 164      if size(duty, 1) == 8 && size(duty, 2) ~= 8
 165          duty = duty.';
 166      end
 167      enabled = get1('Control_Law_Enabler') == 1;
 168      K = evalin('base', ['K_' n]);
 169  
 170      % The LQR, at steps where it ran this step and the previous one (its Discrete Derivative
 171      % then holds the previous step's path)
 172      steps = find(enabled(2:end) & enabled(1:end-1)) + 1;
 173      err = zeros(numel(steps), 1);
 174      for i = 1:numel(steps)
 175          k = steps(i);
 176          mine = spot.control.lqrCommand(K, measured(k, :), path(k, :), (path(k, :) - path(k - 1, :)) / Ts);
 177          err(i) = max(abs(mine(:) - u(k, :).'));
 178      end
 179      r.lqr.(n) = max([err; 0]);
 180      okL = ~isempty(steps) && r.lqr.(n) <= 1e-9 * max(1, max(abs(u(:))));
 181  
 182      % The mixer, at the thruster code's steps, with the workspace values the block reads (see
 183      % check 1); in between, the logged duty cycles must hold
 184      names = wiring.(n);
 185      dist = evalin('base', names{startsWith(names, 'thruster_dist2CG')});
 186      F = evalin('base', names{startsWith(names, 'F_thrusters')});   % each thruster's (calibrated or nominal)
 187      t = sig.([n '_Px_m']).Time;
 188      runs = find(abs(t / period - round(t / period)) < 1e-9);
 189      errD = zeros(numel(runs), 1);
 190      for i = 1:numel(runs)
 191          k = runs(i);
 192          mine = spot.control.thrusterDutyCycles(u(k, :), measured(k, 3), dist, F);
 193          errD(i) = max(abs(mine(:) - duty(k, :).'));
 194      end
 195      between = setdiff(2:size(duty, 1), runs);
 196      held = all(all(duty(between, :) == duty(between - 1, :)));
 197      r.mixer.(n) = max([errD; 0]);
 198      okD = ~isempty(runs) && r.mixer.(n) <= 1e-6 && held;
 199      r.text{end + 1} = sprintf(['%s: LQR forces at %d steps, largest difference %.2g N; ' ...
 200          'duty cycles at %d steps, largest difference %.2g%s'], n, numel(steps), r.lqr.(n), numel(runs), ...
 201          r.mixer.(n), ternary(held, '', ' (and they DON''T hold in between)'));
 202      r.ok = r.ok && okL && okD;
 203  end
 204  end
 205  
 206  function name = dutyName(sig, platform)
 207  f = fieldnames(sig);
 208  f = f(startsWith(f, [platform '_Duty_Cycle']));
 209  if isempty(f)
 210      error('spot:control:verifyConsoleController', 'The diagram logs no %s_Duty_Cycle.', platform);
 211  end
 212  name = f{1};
 213  end
 214  
 215  % ============================================================================ 3. python
 216  
 217  function [r, py] = checkPython(root, n)
 218  rng(20261009, 'twister');
 219  in = struct('gains', [], 'lqr', [], 'mixer', []);
 220  s = spot.params.defaults();
 221  mp = spot.massprops.compute(s.mass);
 222  names = {'RED', 'BLACK', 'BLUE'};
 223  
 224  g = cell(1, 30);
 225  Kref = cell(1, 30);
 226  for k = 1:30
 227      c = struct('mass', 5 + 15 * rand, 'inertia', 0.1 + 0.4 * rand, 'Ts', 0.02 * randi(5), ...
 228                 'Q', 10 .^ (4 * rand(1, 6) - 2), 'R', 10 .^ (3 * rand(1, 3) - 1));
 229      g{k} = c;
 230      Kref{k} = spot.control.lqrGains(spot.control.stateSpace(c.mass, c.inertia, c.Ts), c.Q, c.R);
 231  end
 232  in.gains = [g{:}];
 233  
 234  l = cell(1, n);
 235  uref = zeros(3, n);
 236  for k = 1:n
 237      K = Kref{randi(30)} .* (0.5 + rand(3, 6));
 238      c = struct('K', K, 'measured', [3.5 * rand, 2.4 * rand, 8 * rand - 4, randn(1, 3) * 0.1], ...
 239                 'desired', [3.5 * rand, 2.4 * rand, 8 * rand - 4], 'rate', randn(1, 3) * 0.2);
 240      l{k} = c;
 241      uref(:, k) = spot.control.lqrCommand(c.K, c.measured, c.desired, c.rate);
 242  end
 243  in.lqr = [l{:}];
 244  
 245  m = cell(1, n);
 246  dref = zeros(8, n);
 247  for k = 1:n
 248      p = mp.(names{randi(3)});
 249      F = 0.2 + 0.2 * rand(1, 8);   % each thruster its own thrust, as a thruster calibration gives them
 250      c = struct('u', [0.8 * (2 * rand - 1), 0.8 * (2 * rand - 1), 0.15 * (2 * rand - 1)] .* (rand > 0.2), ...
 251                 'theta', 4 * pi * rand - 2 * pi, 'dist', p.thrusters(:).' + randn(1, 8), ...
 252                 'F', F, 'max_iters', 20, 'tol', 1e-6);
 253      m{k} = c;
 254      dref(:, k) = spot.control.thrusterDutyCycles(c.u, c.theta, c.dist, c.F, c.max_iters, c.tol);
 255  end
 256  in.mixer = [m{:}];
 257  
 258  py = runPython(root, in);
 259  r = struct('ok', true, 'text', {{}});
 260  G = py.gains;   % 30 x 3 x 6 from JSON
 261  G = arrayfun(@(k) reshape(G(k, :, :), 3, 6), (1:size(G, 1)).', 'UniformOutput', false);
 262  eK = max(cellfun(@(a, b) max(abs(a(:) - b(:))) / max(abs(b(:))), G, Kref(:)));
 263  uPy = py.lqr.';   % n x 3 from JSON
 264  eU = max(abs(uPy(:) - uref(:)) ./ max(1, abs(uref(:))));
 265  dPy = [py.mixer.duty];
 266  eD = abs(dPy - dref);
 267  r.gains = eK; r.lqr = eU; r.mixer = max(eD(:));
 268  r.text{end + 1} = sprintf('LQR gains (dlqr), 30 random models and weights: largest relative difference %.2g', eK);
 269  r.text{end + 1} = sprintf('LQR forces, %d random states: largest difference %.2g', n, eU);
 270  r.text{end + 1} = sprintf(['duty cycles, %d random commands, attitudes and thrusts: largest difference %.2g ' ...
 271      '(%d of %d cases above 1e-6)'], n, r.mixer, nnz(max(eD, [], 1) > 1e-6), n);
 272  r.ok = eK <= 1e-9 && eU <= 1e-12 && r.mixer <= 1e-4;
 273  if r.mixer > 1e-6 && r.ok
 274      r.text{end + 1} = ['   (the mixer iterates and compares numbers; where MATLAB and Python round a ' ...
 275          'sum differently it can take another branch and settle a little differently)'];
 276  end
 277  end
 278  
 279  function out = runPython(root, in)
 280  script = fullfile(root, '+spot', '+console', 'webapp', 'spot_console', 'control.py');
 281  inFile = [tempname '.json'];
 282  outFile = [tempname '.json'];
 283  cleanup = onCleanup(@() cellfun(@(f) deleteIfThere(f), {inFile, outFile}));
 284  fid = fopen(inFile, 'w');
 285  fwrite(fid, jsonencode(in), 'char');
 286  fclose(fid);
 287  py = spot.console.findPython();
 288  [status, msg] = system(sprintf('%s "%s" < "%s" > "%s"', py, script, inFile, outFile));
 289  if status ~= 0
 290      error('spot:control:verifyConsoleController', 'control.py failed: %s', msg);
 291  end
 292  out = jsondecode(fileread(outFile));
 293  end
 294  
 295  function deleteIfThere(f)
 296  if isfile(f)
 297      delete(f);
 298  end
 299  end
 300  
 301  % ============================================================================ 4. defaults
 302  
 303  function r = checkDefaults(model, py)
 304  s = spot.params.defaults();
 305  mp = spot.massprops.compute(s.mass);
 306  d = py.defaults;
 307  r = struct('ok', true, 'text', {{}});
 308  same = @(a, b) isequal(size(a(:)), size(b(:))) && all(abs(a(:) - b(:)) <= 1e-12 * max(1, abs(b(:))));
 309  checks = {};
 310  checks(end + 1, :) = {same(d.sample_rate, s.sampleRate), sprintf('sample rate %g Hz', s.sampleRate)};
 311  checks(end + 1, :) = {same(d.data_rate, s.dataRate), sprintf('PhaseSpace data rate %g Hz', s.dataRate)};
 312  base = 1 / s.sampleRate;
 313  period = thrusterPeriod(model, base);
 314  checks(end + 1, :) = {same(period, base) && same(1 / d.sample_rate, base), sprintf(['the diagram''s thruster ' ...
 315      'code runs every %g s and the console''s loop every %g s; both should be baseRate, %g s'], period, ...
 316      1 / d.sample_rate, base)};
 317  mixer = [model '/Thruster & Float' newline 'Control Code/Change RED Behavior/Calculate Duty Cycle/MATLAB Function5'];
 318  maxIters = str2double(get_param(mixer, 'max_iters'));
 319  tol = str2double(get_param(mixer, 'tol'));
 320  checks(end + 1, :) = {same(d.max_iters, maxIters) && same(d.tol, tol), ...
 321      sprintf('mixer mask: %g iterations, tolerance %g', maxIters, tol)};
 322  for p = {'RED', 'BLACK', 'BLUE'}
 323      n = p{1};
 324      q = d.platforms.(n);
 325      ok = same(q.mass, mp.(n).mass) && same(q.inertia, mp.(n).inertia) && same(q.thrust, s.thrusterForce.(n)) ...
 326          && same(q.thruster_dist2CG, mp.(n).thrusters) && same(q.Q, s.control.(n).lqr.Q) && same(q.R, s.control.(n).lqr.R);
 327      K = spot.control.lqrGains(spot.control.stateSpace(mp.(n).mass, mp.(n).inertia, 1 / s.dataRate), ...
 328                                s.control.(n).lqr.Q, s.control.(n).lqr.R);
 329      Kpy = py.default_gains.(n);
 330      eK = max(abs(Kpy(:) - K(:))) / max(abs(K(:)));
 331      checks(end + 1, :) = {ok && eK <= 1e-9, sprintf(['%s: mass %.4g kg, inertia %.4g kg m^2, thrust %g N, ' ...
 332          'thruster offsets, Q and R; its gain K_%s differs by %.2g (relative)'], n, q.mass, q.inertia, q.thrust, n, eK)};
 333  end
 334  for k = 1:size(checks, 1)
 335      [ok, text] = checks{k, :};
 336      if ok
 337          r.text{end + 1} = ['same: ' text];
 338      else
 339          r.text{end + 1} = ['DIFFERENT: ' text ' (update DEFAULTS in control.py)'];
 340          r.ok = false;
 341      end
 342  end
 343  end
 344  
 345  % ============================================================================ helpers
 346  
 347  function p = thrusterPeriod(model, baseRate)
 348  % How often the diagram's "Thruster & Float Control Code" runs [s]: its own sample time if it is
 349  % atomic (Simulink ignores a virtual subsystem's), else the diagram's step, baseRate
 350  blk = [model '/Thruster & Float' newline 'Control Code'];
 351  value = strtrim(get_param(blk, 'SystemSampleTime'));
 352  if strcmp(get_param(blk, 'TreatAsAtomicUnit'), 'off') || any(strcmp(value, {'-1', 'baseRate'}))
 353      p = baseRate;
 354  else
 355      p = str2double(value);
 356      if isnan(p)
 357          p = evalin('base', value);   % another workspace variable, e.g. dataRate
 358      end
 359  end
 360  end
 361  
 362  function v = ternary(c, a, b)
 363  if c
 364      v = a;
 365  else
 366      v = b;
 367  end
 368  end
 369  
 370  function name = defaultDiagram(root)
 371  file = fullfile(root, 'spot_settings.json');
 372  name = '';
 373  if isfile(file)
 374      s = spot.params.load(file);
 375      name = s.diagram;
 376  end
 377  if isempty(name) || ~isfile(fullfile(root, name))
 378      names = spot.sim.listDiagrams(root);
 379      if isempty(names)
 380          error('spot:control:verifyConsoleController', 'There is no diagram (.slx) in %s.', root);
 381      end
 382      name = names{1};
 383  end
 384  end
```

</details>
<!-- /listing -->
