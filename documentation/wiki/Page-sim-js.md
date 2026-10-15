# sim.js

**File:** `+spot/+console/webapp/static/sim.js` · **Tab:** [[Simulation Tab|Simulation-Tab]] · **MATLAB side:** [[spot.worker.Worker|spot-worker-Worker]], [[spot.params|spot-params]], [[spot.sim|spot-sim]], [[spot.workspace|spot-workspace]]

<!-- source: +spot/+console/webapp/static/sim.js -->

## What it is for

The Simulation tab: the settings the diagram needs (built as forms from the settings struct), simulations run in the user's MATLAB and replayed in the 3D view, the Compare overlays, the MATLAB command window and the Workspace pane. **The settings live in MATLAB**: every change is sent back whole (`settings.set`, debounced), MATLAB saves them, puts them in the base workspace and answers with what follows from them (total time, CG, LQR gains, the calibration used, warnings) or why they can't be applied.

## Function by function

**Lines 15–35.** Constants: the two pose kinds (initial, home), the solver lists, the LQR state and input names, the nine timeline segments with their settings paths (phases 0, 1 and 5 have no switch), the padlock icon.

**Lines 37–87.** Helpers: `getPath`/`setPath` on dotted paths; small form builders (`num`, `rateField` and `pwmField` with their padlocks and their checks `validRate` (1–960 whole Hz) and `validPwm` (above 0, at most 100 Hz), `text`, `cells`).

**Lines 89–104.** `initSimulation`: the state `S` (settings, derived values, the error, the diagrams, the file, MATLAB's status, the run, the result, the sub-tab, the chosen platform, the poses shown, the playback clock, the edit/push counters, the command history, the workspace).

**Lines 107–174.** Settings round trip: `applyReply` takes MATLAB's reply (ignoring the settings part if the user edited meanwhile, i.e. an older sequence number), rebuilds the panes once, fills the fields, redraws the setup markers and the Run line, refreshes the workspace and tells the Designer. When the project's calibration changes (the `spot-calibration` event from [[calibration.js|Page-calibration-js]], after Update from Lab), the settings are loaded again from MATLAB, so the Calibrated Thrust, Calibrated Arm and Table Slope lines say what the next simulation uses. `loadSettings` (`settings.get`). `edited` bumps the sequence and schedules a push; `push` sends the whole struct (`settings.set`) unless one is in flight or a simulation runs.

**Lines 176–436.** The panes, rendered as HTML from the settings:
- `scenarioPane` (191–229): the Model card (the diagram list with *Open in Simulink*, the four platform chips, Sample Rate / Data Rate / Thruster PWM (the last two locked), the Calibrated Thrust switch, `calibration.thrusters`, the Calibrated Arm switch, `calibration.arm`, and the Table Slope switch, `calibration.tableSlope`, each with its status line) and the Timeline card (`timelineRows`, 231–244: a row per segment with its switch or lock, colour, name, when it runs, duration).
- `timeline` (249–257): each segment's typed duration, whether it runs, and MATLAB's start/end.
- `platformsPane` (259–265): the platform picker and either `platformCards` (267–298: Initial Conditions table with the Initial/Home row buttons; Mass Properties with the three scales, the bifilar test, the override fields, the thrust with a note under it when the project has a thruster calibration for that platform, and the computed offsets with *Write PhaseSpace Marker File*) or `armCards` (307–327: the arm's initial pose, where it is mounted, its geometry, link masses and inertias).
- `controlPane` (333–337): the LQR cards of the selected platforms and, with the Arm on, the arm's cards.
- `armCard` (342–376): two cards for `control.ARM`: *Position · Mode 1* (a P/I/D row per joint, then Profile Velocity, Profile Accel. and PWM Limit) and *Velocity · Mode 3* (P/I per joint, Profile Accel., PWM Limit). Each input carries its range (`data-min`/`data-max`: gains 0–16383, profile 0–32767, PWM limit 0–885), which `commit` checks; MATLAB checks again ([[spot.params.apply|spot-params-apply]]).
- `lqrCards` (378–398): per selected platform the LQR table (a row per axis: the error weight, the rate weight, the effort weight) and the computed gain K.
- `solverPane` (400–425): fixed/variable step, the solver name, the variable-step parameters, and the Workspace note.
- `showTab` (427–436): the sub-tab; the Workspace pane replaces the panes.

**Lines 438–539.** `fill` writes the settings into every field (not the one being edited), the toggles, the pose rows, the two locks' state (Data Rate and Thruster PWM fields disabled while locked) and the override fields; `renderDerived` shows what MATLAB computed: the total time and the timeline bar (segments proportional to their typed length, skipped ones hatched and clickable), each segment's times, the pose hint, the chosen platform's mass/CG/inertia, thruster offsets and LED positions, each platform's K, the Table Slope line (from `derived.calibration`: "Used" or "Not used" with the tilt at the centre [mrad] and the date it was measured, "Level: this project has no table calibration", or the file's error), the Calibrated Arm line (from `derived.calibration.armUsed` and `arm`: "Used: spot_calibration.json (arm), measured <date>." (or "Used: arm_calibration.json." for a file copied by hand), "Not used (measured <date>)." with the switch off, or, with no calibration in use, "Defaults: this project has no arm calibration." (switch on) or "Not used: defaults." (switch off)), the Calibrated Thrust line ("Used for the Chaser." naming the platforms the project has a thruster calibration for, "Not used for …" with the switch off, or "Nominal: this project has no thruster calibration."), the note under the chosen platform's Thrust ("Calibrated thrust used: 0.250–0.310 N, measured <date> (Calibration tab).", or "Calibrated thrust (not used)" with the switch off; empty without a calibration for it), the variable count, the file, and the notice with the error and warnings.

**Lines 541–579.** The setup markers on the table: `renderSetup` gives the viewer the selected platforms' poses of the chosen kind (and the arm's initial joints), draggable while the tab is shown; `setPose` (the toolbar's Off/Initial/Home); the viewer's drag callback writes the pose back into the settings (rounded to mm and 0.1°), switches the Platforms pane to that platform, and pushes when the drag ends.

**Lines 581–643.** Input handling: Real Time; typing in a field commits it (validated: numbers finite, the data rate 1–960) with a 500 ms debounce; a change commits and pushes at once (a new diagram rebuilds the panes); clicks on the padlock, the platform chips and timeline segments (toggle), the solver type, the platform picker, the pose row buttons, *Open in Simulink*, *Write PhaseSpace Marker File*.

**Lines 645–688.** Load… (`settings.open`, a MATLAB dialog), Save As… (`settings.saveAs`), Save Results, and the Reset… dialog: settings (`settings.reset`, with the backup's path in the toast), loaded results (unload all), page and view (clear this page's stored preferences except the command history, then reload).

**Lines 690–759.** Run / Stop: a push of pending edits, then `/api/sim/run`; Stop is `/api/sim/stop`. `renderRun` shows the button and the status line: progress while running (preparing, initialising, simulating t of tsim, finishing), or afterwards the result (simulated N s in M s, samples), a failure with *Details* and *Show Block*, or why it can't run (MATLAB not connected, the token missing, settings errors).

**Lines 761–782.** `loadResult`: fetch the replay JSON (`/api/sim/result`), print the summary and Simulink's output into the command window, give the messages panel, the live plot and the viewer their parts, and start playback.

**Lines 784–876.** Playback: the tracks (the last simulation and the overlays) on one clock, each with an offset; `span`, `phases`; `renderPlayback` (the bar, the phase ticks); `seek` (the viewer, the messages, the live plot, the scrubber, the phase name); `play`/`tick` at the chosen speed; Space toggles play; the Paths switch; `layers`.

**Lines 878–965.** Compare: the pop-over listing the last simulation and every loaded dataset with positions, each with a switch and a time offset, plus *Load Results…* and *Keep This Simulation*; `syncOverlays` fetches each switched-on dataset's trajectory from MATLAB and hands the overlays to the viewer.

**Lines 967–1107.** The command window: `print`, `printDetails` (a collapsible entry), `printError` (an error entry: where in your code, each block with *Show in Simulink*, the chain of causes, Simulink's output, MATLAB's report), `showBlock` (`diagram.showBlock`), the status line's buttons, `runCommand` (`workspace.eval`, with a 200-entry history, ↑/↓, `clc`), the input box, Clear, collapse.

**Lines 1109–1188.** The Workspace pane: `refreshWorkspace` (`workspace.list`, debounced), the filtered table, `inspect` (`workspace.inspect`: a matrix table, a struct's fields to click into, a timeseries as a sparkline, or text), the ↑ and ✕ buttons.

**Lines 1190–1205.** The MATLAB badge (Not Connected / release · Idle / release · what it is doing · for N s), updated every second while busy or running.

**Lines 1207–1282.** The interface the app uses: `printError`; `onState`/`onMatlab` (load the settings when MATLAB connects; load a result that appeared); `onSim` (a finished run: load its result and refresh the workspace, or show the failure); `onMatlabEvent` (progress, debug messages, live samples); `show`/`hide`; `hasResult`; `getState` (for the Experiment tab and the Designer); `setPlatform` (a chip from elsewhere; the arm goes with RED); `setArm` (the Designer's choice, with its mount); `setSetting` (the Experiment tab's address and data rate).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/sim.js</b> (1283 lines; click to show)</summary>

```javascript
   1  // Simulation tab (local console, started from MATLAB by SPOT.m): the settings the Simulink
   2  // diagrams need, simulations run in the user's MATLAB and replayed in the shared 3D view, and
   3  // MATLAB's command window and workspace.
   4  //
   5  // The settings live in MATLAB (spot.worker.Worker keeps them in spot_settings.json). Every
   6  // change is sent back whole ("settings.set"); MATLAB puts them into the base workspace and
   7  // answers with what follows from them (total time, CG, LQR gains, ...) or why they don't work.
   8  
   9  import { PLATFORM_KEYS as PLATFORMS, COLORS as SWATCH, platformName, swatch } from './platforms.js';
  10  import { handleLockClick, isUnlocked, lockButton, onLockChange } from './ratelock.js';
  11  
  12  const $ = (sel, el = document) => el.querySelector(sel);
  13  const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
  14  // Target poses (there is no "drop" pose any more: the platforms start at their initial pose)
  15  const POSE_KINDS = [['initial', 'Initial', 'Where the platform starts: the simulation starts here, and phase 2 drives the platform here'],
  16                      ['home', 'Home', 'Where the platform goes back to at the end (phases 4 and 5)']];
  17  const POSE_LABELS = { initial: 'Initial', home: 'Home' };
  18  const FIXED_SOLVERS = ['FixedStepAuto', 'FixedStepDiscrete', 'ode8', 'ode5', 'ode4', 'ode3', 'ode2', 'ode1', 'ode14x', 'ode1be'];
  19  const VARIABLE_SOLVERS = ['VariableStepAuto', 'VariableStepDiscrete', 'ode45', 'ode23', 'ode113', 'ode15s', 'ode23s', 'ode23t', 'ode23tb', 'odeN', 'daessc'];
  20  // LQR states and inputs. Derivatives are written d·/dt: a dot over a letter is hard to see.
  21  const STATES = ['x', 'y', 'θ', 'dx/dt', 'dy/dt', 'dθ/dt'];
  22  const INPUTS = ['Fx', 'Fy', 'Tz'];
  23  const PHASE_COLORS = ['#5c7cfa', '#4c6ef5', '#4263eb', '#f59f00', '#f08c00', '#e8590c', '#d9480f', '#12b886', '#0ca678'];
  24  // The timeline's segments, in order (names as in the diagrams' phase subsystems). Those with an
  25  // "on" switch can be skipped (zero seconds, see spot.params.timing); phases 0, 1 and 5 always
  26  // run, since they start and stop the platforms.
  27  const SEGMENTS = [
  28    { title: 'Phase 0', name: 'Synchronization', dur: 'timing.phase0' },
  29    { title: 'Phase 1', name: 'Start Floating', dur: 'timing.phase1' },
  30    { title: 'Phase 2', name: 'Initial Position', dur: 'timing.phase2', on: 'timing.on.phase2' },
  31    ...[0, 1, 2, 3].map(k => ({ title: `3.${k + 1}`, name: `Sub-Phase ${k + 1}`, dur: `timing.subPhases.${k}`, on: `timing.on.subPhases.${k}`, sub: true })),
  32    { title: 'Phase 4', name: 'Return Home', dur: 'timing.phase4', on: 'timing.on.phase4' },
  33    { title: 'Phase 5', name: 'Hold Home', dur: 'timing.phase5' },
  34  ];
  35  const LOCK = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 10 0v3h1a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1zm2 0h6V7a3 3 0 0 0-6 0z"/></svg>';
  36  
  37  const getPath = (obj, path) => path.split('.').reduce((o, k) => (o == null ? o : o[k]), obj);
  38  function setPath(obj, path, value) {
  39    const keys = path.split('.');
  40    const last = keys.pop();
  41    keys.reduce((o, k) => o[k], obj)[last] = value;
  42  }
  43  const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
  44  const kindTag = k => ({ simulation: 'SIM', experiment: 'EXP' }[k] || 'MAT');
  45  const esc = s => String(s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  46  const fmt = (v, d = 4) => (v == null || !isFinite(v) ? '—' : Number(v).toPrecision(d).replace(/\.?0+(e|$)/, '$1'));
  47  
  48  // ---------------------------------------------------------------- small form builders
  49  
  50  function num(path, label, { unit = '', title = '', step = 'any' } = {}) {
  51    return `<label class="sfield"${title ? ` title="${esc(title)}"` : ''}>
  52      <span class="sfield-label">${label}</span>
  53      <span class="sfield-input${unit.length > 2 ? ' long-unit' : ''}"><input class="input" type="number" step="${step}" data-path="${path}">${unit ? `<span class="unit">${unit}</span>` : ''}</span>
  54    </label>`;
  55  }
  56  // The PhaseSpace data rate (dataRate): locked unless unlocked with its padlock (ratelock.js)
  57  function rateField() {
  58    const title = 'The PhaseSpace data rate: how often the cameras send the platforms\' positions in an experiment, '
  59      + 'and the rate the LQR gains are designed at. The Experiment tab\'s Data Rate is the same setting. Locked: '
  60      + 'click the padlock to change it.';
  61    return `<div class="sfield" title="${esc(title)}">
  62      <span class="sfield-label lock-label"><span>Data Rate</span>${lockButton()}</span>
  63      <span class="sfield-input"><input class="input" type="number" min="1" max="960" step="1" data-path="dataRate" data-rate aria-label="Data rate"><span class="unit">Hz</span></span>
  64    </div>`;
  65  }
  66  const validRate = v => Number.isInteger(v) && v >= 1 && v <= 960;
  67  // The thrusters' PWM frequency (pwmFrequency): locked like the data rate, since it must match the
  68  // Teensy firmware's
  69  function pwmField() {
  70    const title = 'The thrusters\' PWM frequency (PWMFreq): the duty cycles assume a valve period of 1/frequency. '
  71      + 'The Teensy firmware runs its PWM at 5 Hz (PWM_FREQUENCY in teensy_pwm_control.ino), so they must be equal. '
  72      + 'Locked: click the padlock to change it.';
  73    return `<div class="sfield" title="${esc(title)}">
  74      <span class="sfield-label lock-label"><span>Thruster PWM</span>${lockButton('pwm')}</span>
  75      <span class="sfield-input"><input class="input" type="number" min="0.1" max="100" step="any" data-path="pwmFrequency" data-pwm aria-label="Thruster PWM frequency"><span class="unit">Hz</span></span>
  76    </div>`;
  77  }
  78  const validPwm = v => isFinite(v) && v > 0 && v <= 100;
  79  function text(path, label, title = '') {
  80    return `<label class="sfield"${title ? ` title="${esc(title)}"` : ''}>
  81      <span class="sfield-label">${label}</span>
  82      <span class="sfield-input"><input class="input mono" data-path="${path}" data-type="text" spellcheck="false"></span>
  83    </label>`;
  84  }
  85  function cells(path, n) {
  86    return Array.from({ length: n }, (_, k) => `<td><input class="input" type="number" step="any" data-path="${path}.${k}"></td>`).join('');
  87  }
  88  
  89  export function initSimulation({ api, act, toast, storage, viewer, store, messages, livePlot }) {
  90    const S = {
  91      settings: null, derived: null, error: '', diagrams: [], file: '',
  92      matlab: { connected: false }, sim: {}, progress: null, result: null, resultId: 0,
  93      tab: storage.get('spot.sim.tab', 'scenario'),
  94      platform: storage.get('spot.sim.platform', 'RED'),
  95      // Poses shown on the table (the view's toolbar, on this and the Experiment tab): off, initial or home
  96      pose: ['off', 'initial', 'home'].includes(storage.get('spot.pose', 'initial')) ? storage.get('spot.pose', 'initial') : 'initial',
  97      t: 0, playing: false, lastTick: 0, shown: false,
  98      editSeq: 0, pushTimer: null, pushing: false,
  99      history: storage.get('spot.cmdHistory', []), histPos: -1,
 100      workspace: [], inspected: null,
 101    };
 102    const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
 103    const panes = $('#simPanes');
 104  
 105    // ---------------------------------------------------------------- settings from MATLAB
 106  
 107    function applyReply(r, seq) {
 108      if (!r || r.cancelled) return;
 109      // A reply to an older version of the settings: keep what the user typed since
 110      if (seq === undefined || seq === S.editSeq) S.settings = r.settings;
 111      S.derived = r.derived;
 112      S.error = r.error || '';
 113      S.diagrams = r.diagrams || [];
 114      S.file = r.file || S.file;
 115      if (!S.panesBuilt) renderPanes();
 116      fill();
 117      renderSetup();
 118      renderRun();
 119      if (seq !== undefined && seq !== S.editSeq) schedulePush();
 120      refreshWorkspaceSoon();
 121      // (the Designer follows the arm: S.settings, with any edit not sent yet)
 122      document.dispatchEvent(new CustomEvent('spot-settings', { detail: S.settings }));
 123    }
 124  
 125    onLockChange(() => fill());
 126  
 127    // The project's calibration changed (the Calibration tab's Update from Lab): MATLAB applies the
 128    // settings again, so the Scenario tab says what the next simulation uses
 129    let calUpdated;
 130    document.addEventListener('spot-calibration', ev => {
 131      const u = ev.detail && ev.detail.updated;
 132      if (calUpdated !== undefined && u !== calUpdated && S.settings && S.matlab && S.matlab.connected) {
 133        loadSettings().catch(() => {});
 134      }
 135      calUpdated = u;
 136    });
 137  
 138    async function loadSettings() {
 139      try {
 140        applyReply(await matlab('settings.get'));
 141        S.authError = '';
 142      } catch (e) {
 143        S.authError = e.message;   // e.g. opened without the session token
 144        renderRun();
 145        throw e;
 146      }
 147    }
 148  
 149    function edited(rebuild = false) {
 150      S.editSeq++;
 151      if (rebuild) renderPanes();
 152      fill();
 153      renderSetup();
 154      schedulePush();
 155    }
 156  
 157    function schedulePush(delay = 300) {
 158      clearTimeout(S.pushTimer);
 159      S.pushTimer = setTimeout(push, delay);
 160    }
 161  
 162    async function push() {
 163      if (S.pushing || S.sim.running) return schedulePush(600);
 164      S.pushing = true;
 165      const seq = S.editSeq;
 166      try {
 167        applyReply(await matlab('settings.set', { settings: S.settings }), seq);
 168        S.pushedSeq = seq;
 169      } catch (e) {
 170        toast(e.message, 'error', 8000);
 171      } finally {
 172        S.pushing = false;
 173      }
 174    }
 175  
 176    // ---------------------------------------------------------------- panes
 177  
 178    function renderPanes() {
 179      if (!S.settings) return;
 180      S.panesBuilt = true;
 181      const s = S.settings;
 182      const on = s.platforms;
 183      panes.innerHTML = `
 184        <div data-simpane="scenario">${scenarioPane(s)}</div>
 185        <div data-simpane="platforms">${platformsPane(s, on)}</div>
 186        <div data-simpane="control">${controlPane(on)}</div>
 187        <div data-simpane="solver">${solverPane(s)}</div>`;
 188      showTab(S.tab);
 189    }
 190  
 191    function scenarioPane(s) {
 192      const options = (S.diagrams.length ? S.diagrams : [s.diagram]).filter(Boolean)
 193        .map(d => `<option value="${esc(d)}">${esc(d)}</option>`).join('') || '<option value="">No .slx in the Project Folder</option>';
 194      const chips = ['RED', 'BLACK', 'BLUE', 'ARM'].map(n =>
 195        `<button type="button" class="plat-chip" data-toggle="platforms.${n}" style="--c:${SWATCH[n]}">${platformName(n)}</button>`).join('');
 196      return `
 197        <section class="card">
 198          <div class="card-head"><h2>Model</h2>
 199            <button type="button" class="btn btn-sm" data-action="openDiagram" title="Open the diagram in Simulink (in MATLAB)"><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" d="M14 4h6v6M20 4l-9 9M18 14v5a1 1 0 0 1-1 1H5a1 1 0 0 1-1-1V7a1 1 0 0 1 1-1h5"/></svg><span>Open in Simulink</span></button></div>
 200          <label class="field-label">Diagram</label>
 201          <select class="input" data-path="diagram" data-type="text">${options}</select>
 202          <label class="field-label sp-top">Platforms</label>
 203          <div class="plat-chips">${chips}</div>
 204          <div class="fgrid cols-3 sp-top">
 205            ${num('sampleRate', 'Sample Rate', { unit: 'Hz', title: 'Controller rate: the diagram runs every 1/rate s (baseRate, its fixed step).' })}
 206            ${rateField()}
 207            ${pwmField()}
 208          </div>
 209          <div class="sp-top cal-scenario">
 210            <label class="toggle" title="Use each thruster's measured thrust (the lab console's thruster calibration, Calibration tab; this project's resources/spot_calibration.json) in the mixer and so in the simulated plant, as the experiments' controller does. Off: the Thrust on the Platforms tab for all eight."><input type="checkbox" data-path="calibration.thrusters"><span></span>Calibrated Thrust</label>
 211            <span class="hint" data-derived="calthrusters"></span>
 212          </div>
 213          <div class="cal-scenario">
 214            <label class="toggle" title="Use the arm's measured friction and inertia (the lab console's arm calibration, Calibration tab, or a resources/arm_calibration.json copied by hand: the newer) in the simulated arm. Off: the arm model's defaults."><input type="checkbox" data-path="calibration.arm"><span></span>Calibrated Arm</label>
 215            <span class="hint" data-derived="calarm"></span>
 216          </div>
 217          <div class="cal-scenario">
 218            <label class="toggle" title="Simulate the table's measured slope: the floating platforms slide downhill, and their air bearings drag them, as the lab console's table slope calibration found (Calibration tab; this project's resources/spot_calibration.json). Off: a level table."><input type="checkbox" data-path="calibration.tableSlope"><span></span>Table Slope</label>
 219            <span class="hint" data-derived="calslope"></span>
 220          </div>
 221        </section>
 222        <section class="card">
 223          <div class="card-head"><h2>Timeline</h2><span class="pill" data-derived="tsim"></span></div>
 224          <div class="timeline-bar" data-derived="timeline"></div>
 225          <div class="tl-rows">${timelineRows()}</div>
 226          <p class="hint">Switch segments off to skip them, in simulation and in experiments (build again after a change).
 227            ${LOCK.replace('<svg', '<svg class="tl-lock-inline"')} Phases 0, 1 and 5 always run: they start and stop the platforms.</p>
 228        </section>`;
 229    }
 230  
 231    function timelineRows() {
 232      const row = (g, k) => `<div class="tl-row${g.sub ? ' sub' : ''}" data-seg="${k}">
 233          ${g.on ? `<label class="toggle tl-switch" title="Run ${g.title}: switch off to skip it"><input type="checkbox" data-path="${g.on}"><span></span></label>`
 234                 : `<span class="tl-lock" title="${g.title} always runs">${LOCK}</span>`}
 235          <i class="tl-swatch" style="background:${PHASE_COLORS[k]}"></i>
 236          <span class="tl-name" title="${g.title}: ${g.name}"><b>${g.title}</b><span>${g.name}</span></span>
 237          <span class="tl-when" data-derived="when.${k}"></span>
 238          <span class="sfield-input tl-dur"><input class="input" type="number" step="any" min="0" data-path="${g.dur}" aria-label="${g.title} duration"><span class="unit">s</span></span>
 239        </div>`;
 240      const head = `<div class="tl-row tl-head"><span></span><i class="tl-swatch" style="background:linear-gradient(90deg, ${PHASE_COLORS.slice(3, 7).join(', ')})"></i>
 241          <span class="tl-name"><b>Phase 3</b><span>Experiment</span></span><span class="tl-when" data-derived="when.p3"></span>
 242          <span class="tl-sum" data-derived="p3total"></span></div>`;
 243      return SEGMENTS.map((g, k) => (k === 3 ? head : '') + row(g, k)).join('');
 244    }
 245  
 246    // Start and end [s] of each segment as it will run (skipped segments take no time)
 247    // The segments as MATLAB computed them (spot.params.timing: when each runs, skipped ones
 248    // taking no time); the switches and typed durations are shown as set, at once
 249    function timeline(s) {
 250      const derived = list(((S.derived || {}).timing || {}).segments);
 251      return SEGMENTS.map((g, k) => {
 252        const m = derived[k] || {};
 253        const d = Math.max(Number(getPath(s, g.dur)) || 0, 0);
 254        const run = !g.on || getPath(s, g.on) !== false;
 255        return { ...g, d, run, start: m.start, end: m.end };   // g.on stays the switch's settings path
 256      });
 257    }
 258  
 259    function platformsPane(s, on) {
 260      const picker = ['RED', 'BLACK', 'BLUE', 'ARM'].map(n =>
 261        `<button type="button" data-pick="${n}" class="${S.platform === n ? 'active' : ''}${on[n] ? '' : ' off'}">${swatch(n)}${platformName(n)}</button>`).join('');
 262      const n = S.platform;
 263      const off = on[n] ? '' : `<p class="hint warn-text">The ${platformName(n)} is not in this scenario; turn it on under Scenario to use it.</p>`;
 264      return `<div class="segmented plat-picker">${picker}</div>${off}` + (n === 'ARM' ? armCards() : platformCards(n));
 265    }
 266  
 267    function platformCards(n) {
 268      const rows = POSE_KINDS.map(([k, label, title]) =>
 269        `<tr data-poserow="${k}" title="${esc(title)}"><th><button type="button" class="pose-row-btn" data-setpose="${k}" title="Show the ${label.toLowerCase()} poses on the table">${label}</button></th>${cells(`initial.${n}.${k}`, 3)}</tr>`).join('');
 270      const sc = ['A · left middle edge', 'B · right back corner', 'C · right front corner'];
 271      return `
 272        <section class="card">
 273          <div class="card-head"><h2>Initial Conditions</h2></div>
 274          <table class="ic-table"><thead><tr><th></th><th>x [m]</th><th>y [m]</th><th>θ [°]</th></tr></thead><tbody>${rows}</tbody></table>
 275          <p class="hint" data-derived="posehint"></p>
 276        </section>
 277        <section class="card">
 278          <div class="card-head"><h2>Mass Properties</h2>
 279            <label class="toggle" title="Use the mass and inertia typed below instead of the measured ones (all platforms)"><input type="checkbox" data-path="mass.override"><span></span>Override</label></div>
 280          <span class="field-label tip" title="Scale readings with the platform on three scales (resources/COMConfig.png)">Scales</span>
 281          <div class="fgrid cols-3">${[0, 1, 2].map(k => num(`mass.${n}.scales.${k}`, 'ABC'[k], { unit: 'kg', title: sc[k] })).join('')}</div>
 282          <p class="derived-row" data-derived="cg"></p>
 283          <span class="field-label tip sp-top" title="Bifilar pendulum: period T, string length L, string spacing D">Bifilar Pendulum</span>
 284          <div class="fgrid cols-3">${num(`mass.${n}.bifilar.T`, 'T', { unit: 's' })}${num(`mass.${n}.bifilar.L`, 'L', { unit: 'm' })}${num(`mass.${n}.bifilar.D`, 'D', { unit: 'm' })}</div>
 285          <p class="derived-row" data-derived="inertia"></p>
 286          <div class="fgrid cols-3 sp-top override-fields">
 287            ${num(`mass.${n}.mass`, 'Mass', { unit: 'kg', title: 'Used when Override is on' })}
 288            ${num(`mass.${n}.inertia`, 'Inertia', { unit: 'kg·m²', title: 'Used when Override is on' })}
 289            ${num(`thrusterForce.${n}`, 'Thrust', { unit: 'N', title: 'Force of one thruster (all eight; the thruster calibration replaces it when used)' })}
 290          </div>
 291          <p class="hint" data-derived="calthrust"></p>
 292          <details class="advanced"><summary>Thruster Offsets and LED Positions</summary>
 293            <div data-derived="offsets"></div>
 294            <button type="button" class="btn btn-sm sp-top" data-action="writeTracker">Write PhaseSpace Marker File</button>
 295            <p class="hint">Writes resources/tracker_positions_phasespace.txt (LED positions of all three platforms from their CG), which the PhaseSpace switch uploads to the lab console.</p>
 296          </details>
 297        </section>`;
 298    }
 299  
 300    function mountText() {
 301      const m = S.settings && S.settings.mass && S.settings.mass.arm && S.settings.mass.arm.mount;
 302      if (!m) return '';
 303      const face = { '+x': '+X', '+y': '+Y', '-y': '−Y' }[m.face] || m.face;
 304      return ` at ${face} Face · ${m.side === 'left' ? 'Left' : 'Right'} (choose its place in the Designer)`;
 305    }
 306  
 307    function armCards() {
 308      const g = [['Phi', 'Φ', 'rad'], ['b0', 'b₀', 'm'], ['a1', 'a₁', 'm'], ['b1', 'b₁', 'm'], ['a2', 'a₂', 'm'],
 309                 ['b2', 'b₂', 'm'], ['a3', 'a₃', 'm'], ['b3', 'b₃', 'm']];
 310      const links = [['mPROX', 'Proximal', 'kg'], ['mDIST', 'Distal', 'kg'], ['mEE', 'End Effector', 'kg'],
 311                     ['IPROX', 'Proximal', 'kg·m²'], ['IDIST', 'Distal', 'kg·m²'], ['IEE', 'End Effector', 'kg·m²']];
 312      return `
 313        <section class="card">
 314          <div class="card-head"><h2>Initial Pose</h2></div>
 315          <div class="fgrid cols-3">${['Shoulder q₁', 'Elbow q₂', 'Wrist q₃'].map((l, k) => num(`initial.ARM.initial.${k}`, l, { unit: '°' })).join('')}</div>
 316          <p class="hint">Mounted on the Chaser${mountText()}; shown on the table with the Chaser's initial pose (Initial Conditions: Initial, above the view). Each angle is 0 straight out (the shoulder: out from its face), ±90° to either side.</p>
 317        </section>
 318        <section class="card">
 319          <div class="card-head"><h2>Arm Geometry</h2></div>
 320          <div class="fgrid cols-4">${g.map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
 321          <span class="field-label sp-top">Link Masses</span>
 322          <div class="fgrid cols-3">${links.slice(0, 3).map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
 323          <span class="field-label sp-top">Link Inertias</span>
 324          <div class="fgrid cols-3">${links.slice(3).map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
 325          <p class="hint">Definitions: resources/ArmConfig.png.</p>
 326        </section>`;
 327    }
 328  
 329    // LQR (the only platform controller): state weights Q, control weights R, and the gain K
 330    // MATLAB computes from them (STATES, INPUTS at the top)
 331    // One compact table per platform: a row per axis (x, y, θ) with its three weights, the error
 332    // (Q), its rate (Q) and the effort on that axis (R). Each box is tagged with what it weighs.
 333    function controlPane(on) {
 334      const names = PLATFORMS.filter(n => on[n]);
 335      if (!names.length && !on.ARM) return '<p class="hint">Turn on a platform under Scenario to set its controller.</p>';
 336      return (names.length ? lqrCards(names) : '') + (on.ARM ? armCard() : '');
 337    }
 338  
 339    // The arm's two default controllers (the servos' own position and velocity laws, run by the
 340    // diagram; spot.arm.PositionControl / VelocityControl): per-joint gains in the servos' units, and
 341    // the profile and PWM limit (control.ARM in the settings, armControl in MATLAB)
 342    function armCard() {
 343      const JOINTS = ['Shoulder', 'Elbow', 'Wrist'];
 344      const gain = (path, title) => `<td><span class="lqr-in" title="${esc(title)}"><input class="input" type="number" step="1" min="0" max="16383"
 345          data-min="0" data-max="16383" data-path="${path}" aria-label="${esc(title)}"></span></td>`;
 346      const rows = (mode, gains) => JOINTS.map((j, i) => `<tr><th scope="row" class="arm-joint">${j}</th>
 347          ${gains.map(g => gain(`control.ARM.${mode}.${g}.${i}`, `${j}: ${mode} ${g} gain`)).join('')}</tr>`).join('');
 348      const limit = (path, label, max, unit, title) => `<label class="sfield" title="${esc(title)}">
 349          <span class="sfield-label">${label}</span>
 350          <span class="sfield-input${unit.length > 2 ? ' long-unit' : ''}"><input class="input" type="number" step="1" min="0" max="${max}"
 351            data-min="0" data-max="${max}" data-path="${path}">${unit ? `<span class="unit">${unit}</span>` : ''}</span></label>`;
 352      const head = cols => `<thead><tr><th></th>${cols.map(c => `<th>${c}</th>`).join('')}</tr></thead>`;
 353      return `
 354        <section class="card lqr-card arm-card">
 355          <div class="card-head"><h2 class="hw-name">${swatch('ARM')}${platformName('ARM')}</h2><span class="pill">Position · Mode 1</span></div>
 356          <p class="hint">The servo's position law, run every step: a PID on the joint angle error, giving the torque the servo
 357            would give. Gains in the servo's units (P/128, I/65536, D/16 per the Dynamixel manual).</p>
 358          <table class="lqr-table arm-table">${head(['P', 'I', 'D'])}<tbody>${rows('position', ['P', 'I', 'D'])}</tbody></table>
 359          <div class="fgrid cols-3 sp-top">
 360            ${limit('control.ARM.position.profileVelocity', 'Profile Velocity', 32767, '', 'How fast the desired angle moves to the goal, in units of 0.229 rev/min (20 = 0.48 rad/s; 0 = at once)')}
 361            ${limit('control.ARM.position.profileAcceleration', 'Profile Accel.', 32767, '', 'The profile\'s acceleration, in units of 214.577 rev/min² (0 = none)')}
 362            ${limit('control.ARM.position.pwmLimit', 'PWM Limit', 885, '', 'The most the controller asks of the motor, out of 885 (= the full supply voltage)')}
 363          </div>
 364        </section>
 365        <section class="card lqr-card arm-card">
 366          <div class="card-head"><h2 class="hw-name">${swatch('ARM')}${platformName('ARM')}</h2><span class="pill">Velocity · Mode 3</span></div>
 367          <p class="hint">The servo's velocity law: a PI on the joint speed error (P/128, I/65536).</p>
 368          <table class="lqr-table arm-table">${head(['P', 'I'])}<tbody>${rows('velocity', ['P', 'I'])}</tbody></table>
 369          <div class="fgrid cols-3 sp-top">
 370            ${limit('control.ARM.velocity.profileAcceleration', 'Profile Accel.', 32767, '', 'How fast the desired speed moves to the goal, in units of 214.577 rev/min² (0 = at once)')}
 371            ${limit('control.ARM.velocity.pwmLimit', 'PWM Limit', 885, '', 'The most the controller asks of the motor, out of 885 (= the full supply voltage)')}
 372          </div>
 373          <p class="hint">Which controller runs is set by each phase (<span class="mono">ARM_Control_Law_Enabler</span> in
 374            1 - Experiment Logic: 1 position, 3 velocity, 2 your torque controller, 0 off). Defaults are tuned for 20 Hz.</p>
 375        </section>`;
 376    }
 377  
 378    function lqrCards(names) {
 379      const box = (path, tag, title) => `<span class="lqr-in" title="${esc(title)}">
 380          <input class="input" type="number" step="any" min="0" data-path="${path}" aria-label="${esc(title)}"><span class="lqr-tag">${tag}</span></span>`;
 381      const rows = n => [0, 1, 2].map(i => `<tr><th scope="row">${STATES[i]}</th>
 382          <td>${box(`control.${n}.lqr.Q.${i}`, STATES[i], `Q: weight on the ${STATES[i]} error`)}</td>
 383          <td>${box(`control.${n}.lqr.Q.${i + 3}`, STATES[i + 3], `Q: weight on the ${STATES[i + 3]} error`)}</td>
 384          <td>${box(`control.${n}.lqr.R.${i}`, INPUTS[i], `R: weight on ${INPUTS[i]}`)}</td></tr>`).join('');
 385      return `<p class="hint lqr-help">Larger <b>Q</b> corrects that error harder; larger <b>R</b> uses the thrusters more gently.</p>` +
 386        names.map(n => `
 387        <section class="card lqr-card">
 388          <div class="card-head"><h2 class="hw-name">${swatch(n)}${platformName(n)}</h2><span class="pill">LQR</span></div>
 389          <table class="lqr-table">
 390            <thead><tr><th></th>
 391              <th title="Q: weights on the position and attitude errors">Error <i>Q</i></th>
 392              <th title="Q: weights on the velocity and angular-rate errors">Rate <i>Q</i></th>
 393              <th title="R: weights on the forces and torque">Effort <i>R</i></th></tr></thead>
 394            <tbody>${rows(n)}</tbody>
 395          </table>
 396          <details class="advanced"><summary>Gain K (Computed)</summary><div data-derived="K.${n}"></div></details>
 397        </section>`).join('');
 398    }
 399  
 400    function solverPane(s) {
 401      const variable = s.solver.type === 'Variable-step';
 402      const list = variable ? VARIABLE_SOLVERS : FIXED_SOLVERS;
 403      const path = variable ? 'solver.variableStepName' : 'solver.fixedStepName';
 404      return `
 405        <section class="card">
 406          <div class="card-head"><h2>Solver</h2></div>
 407          <div class="segmented" data-solvertype>
 408            <button type="button" data-set="solver.type" data-value="Fixed-step">Fixed-Step</button>
 409            <button type="button" data-set="solver.type" data-value="Variable-step">Variable-Step</button>
 410          </div>
 411          <label class="field-label sp-top">Solver</label>
 412          <select class="input" data-path="${path}" data-type="text">${list.map(v => `<option>${v}</option>`).join('')}</select>
 413          ${variable ? `<div class="fgrid cols-2 sp-top">
 414            ${text('solver.maxStep', 'Max Step')}${text('solver.minStep', 'Min Step')}
 415            ${text('solver.initialStep', 'Initial Step')}${text('solver.relTol', 'Relative Tolerance')}
 416            ${text('solver.absTol', 'Absolute Tolerance')}</div>`
 417            : '<p class="hint">Fixed step = baseRate (1 / sample rate, under Scenario).</p>'}
 418          <p class="hint">Read from the diagram when you choose it, and applied to each simulation without changing the diagram file.</p>
 419        </section>
 420        <section class="card">
 421          <div class="card-head"><h2>Workspace</h2></div>
 422          <p class="hint" data-derived="variables"></p>
 423          <p class="hint">Settings File: <span class="mono" data-derived="file"></span></p>
 424        </section>`;
 425    }
 426  
 427    function showTab(tab) {
 428      S.tab = tab;
 429      storage.set('spot.sim.tab', tab);
 430      for (const b of $$('[data-simtab]')) b.classList.toggle('active', b.dataset.simtab === tab);
 431      for (const p of $$('[data-simpane]', panes)) p.hidden = p.dataset.simpane !== tab;
 432      // The workspace has a pane of its own, the full height of the sidebar
 433      panes.hidden = tab === 'workspace';
 434      $('#wsPane').hidden = tab !== 'workspace';
 435      if (tab === 'workspace') refreshWorkspace();
 436    }
 437  
 438    // ---------------------------------------------------------------- filling in values
 439  
 440    function fill() {
 441      const s = S.settings;
 442      if (!s) return;
 443      messages?.setSelected(s.platforms);
 444      $('#simRealTime').checked = Boolean(s.realTime);
 445      $('#simRealTimeHint').textContent = s.realTime && S.derived ? `· takes about ${fmt(S.derived.tsim, 4)} s` : '';
 446      for (const el of $$('[data-path]', panes)) {
 447        if (el === document.activeElement) continue;
 448        const v = getPath(s, el.dataset.path);
 449        if (el.type === 'checkbox') el.checked = Boolean(v);
 450        else el.value = v == null ? '' : v;
 451      }
 452      for (const b of $$('[data-toggle]', panes)) b.classList.toggle('on', Boolean(getPath(s, b.dataset.toggle)));
 453      for (const b of $$('[data-set]', panes)) b.classList.toggle('active', getPath(s, b.dataset.set) === b.dataset.value);
 454      for (const r of $$('[data-poserow]', panes)) r.classList.toggle('shown', r.dataset.poserow === S.pose);
 455      for (const el of $$('[data-rate]', panes)) el.disabled = !isUnlocked('rate');
 456      for (const el of $$('[data-pwm]', panes)) el.disabled = !isUnlocked('pwm');
 457      for (const el of $$('.override-fields input', panes)) {
 458        el.disabled = !s.mass.override && !el.dataset.path.startsWith('thrusterForce');
 459      }
 460      renderDerived();
 461    }
 462  
 463    function renderDerived() {
 464      const s = S.settings, d = S.derived || {};
 465      const set = (key, html) => { for (const el of $$(`[data-derived="${key}"]`, panes)) el.innerHTML = html; };
 466      // Timeline: the bar shows every segment at its typed length (skipped ones hatched, so they
 467      // can be clicked back on); the times are as it will run
 468      const segs = timeline(s);
 469      const tm = d.timing || {};
 470      const skipped = segs.filter(g => !g.run);
 471      set('tsim', `${fmt(d.tsim, 6)} s Total${skipped.length ? ` · ${skipped.length} Skipped` : ''}`);
 472      const typed = segs.reduce((a, g) => a + g.d, 0) || 1;
 473      set('timeline', segs.map((g, k) => {
 474        const short = g.title.replace('Phase ', '');
 475        const label = g.d / typed > 0.06 ? short : '';
 476        const tip = `${g.title} · ${g.name}: ${g.run ? `${fmt(g.start, 5)}–${fmt(g.end, 5)} s` : `skipped (${fmt(g.d, 5)} s)`}`;
 477        if (!g.on) {   // always runs
 478          return g.d ? `<span class="tl-seg locked on" style="flex:${g.d};--c:${PHASE_COLORS[k]}" title="${esc(tip)} (always runs)">${label}</span>` : '';
 479        }
 480        return `<button type="button" class="tl-seg${g.run ? ' on' : ''}" data-toggle="${g.on}" style="flex:${Math.max(g.d, typed * 0.012)};--c:${PHASE_COLORS[k]}" title="${esc(tip)}: click to ${g.run ? 'skip' : 'run'} it">${label}</button>`;
 481      }).join(''));
 482      segs.forEach((g, k) => {
 483        set(`when.${k}`, g.run ? `${fmt(g.start, 5)}–${fmt(g.end, 5)} s` : 'skipped');
 484        const row = $(`[data-seg="${k}"]`, panes);
 485        if (row) row.classList.toggle('off', !g.run);
 486      });
 487      set('when.p3', tm.Phase3_Duration > 0 ? `${fmt(tm.Phase2_End, 5)}–${fmt(tm.Phase3_End, 5)} s` : 'Skipped');
 488      set('p3total', `${fmt(tm.Phase3_Duration, 5)} s`);
 489      // The table slope calibration this project has (and whether the simulation uses it)
 490      const cal = d.calibration || {}, t = cal.table && !Array.isArray(cal.table) ? cal.table : null;   // (MATLAB's [] for none)
 491      set('calslope', cal.error ? `<span class="bad">${esc(cal.error)}</span>`
 492        : !t ? 'Level: this project has no table calibration (Calibration tab: Update from Lab).'
 493        : `${s.calibration && s.calibration.tableSlope ? 'Used' : 'Not used'}: ${fmt(t.tilt_centre_mrad, 3)} mrad at the centre, measured ${esc(String(t.updated).slice(0, 10))}.`);
 494      // The arm calibration the simulation uses (the file it came from), or the one the project has
 495      const armFrom = String(cal.armUsed || ''), armCal = cal.arm && !Array.isArray(cal.arm) ? cal.arm : null;
 496      set('calarm', armFrom ? `Used: ${esc(armFrom.split(/[\\/]/).pop())}${armCal && armFrom.includes('spot_calibration') ? `, measured ${esc(String(armCal.created).slice(0, 10))}` : ''}.`
 497        : armCal ? `Not used (measured ${esc(String(armCal.created).slice(0, 10))}).`
 498        : s.calibration && s.calibration.arm ? 'Defaults: this project has no arm calibration.' : 'Not used: defaults.');
 499      // The thruster calibrations this project has (and which the simulation uses)
 500      const th = cal.thrusters && !Array.isArray(cal.thrusters) ? cal.thrusters : {};
 501      const have = ['RED', 'BLACK', 'BLUE'].filter(k => th[k] && !Array.isArray(th[k]));
 502      const using = s.calibration && s.calibration.thrusters;
 503      set('calthrusters', !have.length ? 'Nominal: this project has no thruster calibration.'
 504        : `${using ? 'Used' : 'Not used'} for the ${have.map(k => platformName(k)).join(', ')}.`);
 505      const mine = th[S.platform] && !Array.isArray(th[S.platform]) ? th[S.platform] : null, F = mine ? [].concat(mine.thrust) : null;
 506      set('calthrust', !mine ? '' : `${using ? 'Calibrated thrust used' : 'Calibrated thrust (not used)'}: ${Math.min(...F).toFixed(3)}–${Math.max(...F).toFixed(3)} N, measured ${esc(String(mine.updated).slice(0, 10))} (Calibration tab).`);
 507      set('posehint', S.pose === 'off'
 508        ? 'Choose Initial Conditions: Initial or Home above the view (or click Initial or Home here) to see them on the table and drag them.'
 509        : `The ${S.pose} poses are on the table: drag ${S.platform} to move its pose; Shift+drag rotates it.`);
 510      const n = S.platform;
 511      const mp = d.mass && d.mass[n];
 512      if (mp) {
 513        set('cg', `Mass <b>${fmt(mp.measuredMass)} kg</b> · CG <b>${fmt(mp.cg[0] * 100, 3)}, ${fmt(mp.cg[1] * 100, 3)} cm</b> from the body centre`);
 514        set('inertia', `Inertia <b>${fmt(mp.measuredInertia)} kg·m²</b>` +
 515          (s.mass.override ? ` · <span class="warn-text">overridden: ${fmt(mp.mass)} kg, ${fmt(mp.inertia)} kg·m²</span>` : ''));
 516        const thrRow = ks => `<tr>${ks.map(k => `<th>T${k}</th>`).join('')}</tr><tr>${ks.map(k => `<td>${fmt(mp.thrusters[k - 1], 4)}</td>`).join('')}</tr>`;
 517        const corners = ['+x +y', '+x −y', '−x −y', '−x +y'];
 518        const leds = mp.leds.map((p, k) => `<tr><th>${corners[k]}</th><td>${fmt(p[0] * 1000, 4)}</td><td>${fmt(p[1] * 1000, 4)}</td></tr>`).join('');
 519        set('offsets', `<span class="field-label">Thruster Lines from the CG (mm)</span>
 520          <table class="gain-table thr-table"><tbody>${thrRow([1, 2, 3, 4])}${thrRow([5, 6, 7, 8])}</tbody></table>
 521          <span class="field-label sp-top">LEDs from the CG (mm)</span>
 522          <table class="gain-table"><thead><tr><th>Corner</th><th>x</th><th>y</th></tr></thead><tbody>${leds}</tbody></table>`);
 523      } else {
 524        set('cg', ''); set('inertia', ''); set('offsets', '');
 525      }
 526      for (const p of PLATFORMS) {
 527        const K = d.lqr && d.lqr[p];
 528        set(`K.${p}`, K ? `<table class="gain-table k-table"><thead><tr><th></th>${STATES.map(h => `<th>${h}</th>`).join('')}</tr></thead>
 529            <tbody>${K.map((row, r) => `<tr><th>${INPUTS[r] || ''}</th>${row.map(v => `<td>${fmt(Math.abs(v) < 1e-9 ? 0 : v, 3)}</td>`).join('')}</tr>`).join('')}</tbody></table>
 530            <p class="hint">Rows: the forces and torque; columns: the state errors they respond to.</p>`
 531                        : '<p class="hint">Not calculated (see the warnings above).</p>');
 532      }
 533      set('variables', d.variables ? `${d.variables.length} variables are put into MATLAB's base workspace whenever a setting changes; SPOT_User_Parameters.m then runs, for your own.` : '');
 534      set('file', esc(S.file || ''));
 535      const notice = $('#simNotice');
 536      const warnings = [S.error, ...((d && d.warnings) || [])].filter(Boolean);
 537      notice.hidden = !warnings.length;
 538      notice.textContent = warnings.join(' ');
 539    }
 540  
 541    // ---------------------------------------------------------------- the table (setup poses)
 542  
 543    // The poses chosen in the view's toolbar (also shown, not draggable, on the Experiment tab)
 544    function renderSetup() {
 545      for (const b of $$('[data-pose]')) b.classList.toggle('active', b.dataset.pose === S.pose);
 546      if (!S.settings) return;
 547      const s = S.settings;
 548      const kind = S.pose === 'home' ? 'home' : 'initial';
 549      const poses = {};
 550      for (const n of PLATFORMS) if (s.platforms[n]) poses[n] = s.initial[n][kind];
 551      const arm = s.platforms.ARM && kind === 'initial' && S.derived && S.derived.armInitial ? S.derived.armInitial : null;
 552      viewer?.setSetup({ poses, kind: POSE_LABELS[kind], arm, editable: S.shown && S.pose !== 'off' });
 553    }
 554  
 555    function setPose(kind) {
 556      S.pose = kind;
 557      storage.set('spot.pose', kind);
 558      fill();
 559      renderSetup();
 560      if (S.shown) layers();
 561      document.dispatchEvent(new CustomEvent('spot-pose', { detail: kind }));
 562    }
 563    for (const b of $$('[data-pose]')) b.onclick = () => setPose(b.dataset.pose);
 564  
 565    if (viewer) {
 566      viewer.setup.onDrag = (name, x, y, deg, done) => {
 567        if (!S.settings || !S.settings.initial[name]) return;
 568        const r = (v, k) => Math.round(v * 10 ** k) / 10 ** k;
 569        const th = ((deg % 360) + 360) % 360;
 570        S.settings.initial[name][S.pose === 'home' ? 'home' : 'initial'] = [r(x, 3), r(y, 3), r(th > 180 ? th - 360 : th, 1)];
 571        if (S.platform !== name && PLATFORMS.includes(name) && done) {
 572          S.platform = name;
 573          storage.set('spot.sim.platform', name);
 574          renderPanes();
 575        }
 576        fill();
 577        if (done) edited();
 578      };
 579    }
 580  
 581    // ---------------------------------------------------------------- input events
 582  
 583    $('#simRealTime').onchange = ev => {
 584      if (!S.settings) return;
 585      setPath(S.settings, 'realTime', ev.target.checked);
 586      edited();
 587    };
 588  
 589    panes.addEventListener('input', ev => {
 590      const el = ev.target;
 591      if (!el.dataset.path || el.type === 'checkbox' || el.tagName === 'SELECT') return;
 592      if (!commit(el)) return;
 593      S.editSeq++;
 594      renderDerived();
 595      renderSetup();
 596      schedulePush(500);
 597    });
 598    panes.addEventListener('change', ev => {
 599      const el = ev.target;
 600      if (!el.dataset.path) return;
 601      if (commit(el)) edited(el.dataset.path === 'diagram');
 602    });
 603    function commit(el) {
 604      let v;
 605      if (el.type === 'checkbox') v = el.checked;
 606      else if (el.dataset.type === 'text') v = el.value;
 607      else {
 608        v = el.value === '' ? NaN : Number(el.value);
 609        const ok = el.dataset.rate !== undefined ? validRate(v) : el.dataset.pwm !== undefined ? validPwm(v)
 610          : el.dataset.min !== undefined ? isFinite(v) && v >= Number(el.dataset.min) && v <= Number(el.dataset.max)
 611          : isFinite(v);
 612        el.classList.toggle('invalid', !ok);
 613        if (!ok) return false;
 614      }
 615      setPath(S.settings, el.dataset.path, v);
 616      return true;
 617    }
 618    panes.addEventListener('click', ev => {
 619      const b = ev.target.closest('button');
 620      if (b && b.dataset.ratelock !== undefined) return handleLockClick(b.dataset.ratelock);
 621      if (!b || !S.settings) return;
 622      if (b.dataset.toggle) {
 623        setPath(S.settings, b.dataset.toggle, !getPath(S.settings, b.dataset.toggle));
 624        edited(true);
 625      } else if (b.dataset.set) {
 626        setPath(S.settings, b.dataset.set, b.dataset.value);
 627        edited(true);
 628      } else if (b.dataset.pick) {
 629        S.platform = b.dataset.pick;
 630        storage.set('spot.sim.platform', S.platform);
 631        renderPanes();
 632        fill();
 633      } else if (b.dataset.setpose) {
 634        setPose(b.dataset.setpose);
 635        fill();
 636        renderSetup();
 637      } else if (b.dataset.action === 'openDiagram') {
 638        act(() => matlab('diagram.open'));
 639      } else if (b.dataset.action === 'writeTracker') {
 640        act(async () => toast(`Wrote ${(await matlab('massprops.writeTracker')).file}`, 'success', 6000));
 641      }
 642    });
 643    for (const b of $$('[data-simtab]')) b.onclick = () => showTab(b.dataset.simtab);
 644  
 645    $('#simOpen').onclick = () => act(async () => {
 646      toast('Choose the file in MATLAB\'s dialog', 'info', 4000);
 647      const r = await matlab('settings.open');
 648      if (r && !r.cancelled) { S.panesBuilt = false; applyReply(r); toast(`Loaded ${r.loaded}`, 'success', 5000); }
 649    });
 650    $('#simSaveAs').onclick = () => act(async () => {
 651      toast('Choose where to save in MATLAB\'s dialog', 'info', 4000);
 652      const r = await matlab('settings.saveAs');
 653      if (r && r.saved) toast(`Saved ${r.saved}`, 'success', 5000);
 654    });
 655    $('#simSaveResult').onclick = () => act(() => store.saveSimulation());
 656    // Reset: a clean slate, in parts (settings in MATLAB, this page's preferences, loaded results)
 657    const resetDialog = $('#resetDialog');
 658    $('#simReset').onclick = () => {
 659      for (const c of $$('[data-reset]', resetDialog)) c.checked = true;
 660      resetDialog.showModal();
 661    };
 662    for (const c of $$('[data-reset]', resetDialog)) {
 663      c.onchange = () => { $('#resetGo').disabled = !$$('[data-reset]', resetDialog).some(x => x.checked); };
 664    }
 665    $('#resetGo').onclick = () => act(async () => {
 666      const what = Object.fromEntries($$('[data-reset]', resetDialog).map(c => [c.dataset.reset, c.checked]));
 667      resetDialog.close();
 668      toast('Resetting to the defaults…', 'info', 4000);
 669      if (what.settings) {
 670        clearTimeout(S.pushTimer);
 671        S.panesBuilt = false;
 672        const r = await matlab('settings.reset');
 673        applyReply(r);
 674        S.pushedSeq = S.editSeq;
 675        toast(r.backup ? `Settings reset to the defaults. The previous ones are in ${r.backup} (Load… to bring them back).`
 676                       : 'Settings reset to the defaults.', 'success', 9000);
 677      }
 678      if (what.results) {
 679        for (const d of [...store.loaded]) await store.unload(d.id);
 680      }
 681      if (what.view) {
 682        // Every preference this page keeps (spot.*) except the command history; then start afresh
 683        try {
 684          for (const k of Object.keys(localStorage)) if (k.startsWith('spot.') && k !== 'spot.cmdHistory') localStorage.removeItem(k);
 685        } catch { /* storage blocked: nothing kept anyway */ }
 686        setTimeout(() => location.reload(), what.settings ? 1500 : 300);
 687      }
 688    });
 689  
 690    // ---------------------------------------------------------------- running a simulation
 691  
 692    $('#simRun').onclick = () => act(async () => {
 693      if (S.sim.running) {
 694        await api('/api/sim/stop', {});
 695        toast('Stopping… (MATLAB checks between steps, so it can take a moment)', 'info', 5000);
 696        return;
 697      }
 698      clearTimeout(S.pushTimer);
 699      if (S.editSeq !== S.pushedSeq) await push();
 700      S.progress = null;
 701      S.sim = await api('/api/sim/run', {});
 702      messages?.simStarted();
 703      livePlot?.simStarted();
 704      renderRun();
 705    });
 706  
 707    function renderRun() {
 708      const btn = $('#simRun'), status = $('#simStatus');
 709      const sim = S.sim || {};
 710      const ready = S.matlab.connected && S.settings && S.settings.diagram && !S.error;
 711      if (sim.running) {
 712        btn.className = 'btn btn-stop';
 713        btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="6" y="6" width="12" height="12" rx="2" fill="currentColor"/></svg><span>Stop</span>';
 714        btn.disabled = false;
 715        const elapsed = Math.round(Date.now() / 1000 - sim.started);
 716        const p = S.progress;
 717        const phase = !p || p.status === 'stopped' || p.status === 'compiling' ? 'Preparing the diagram'
 718          : p.status === 'initializing' ? 'Initialising' : p.status === 'terminating' ? 'Finishing'
 719          : `Simulating: ${fmt(p.t, 4)} of ${fmt(p.tsim, 4)} s`;
 720        status.className = 'hint';
 721        status.innerHTML = `<span class="spinner dark"></span> ${phase} · ${elapsed} s`;
 722      } else {
 723        btn.className = 'btn btn-go';
 724        btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg><span>Run Simulation</span>';
 725        btn.disabled = !ready || Boolean(S.matlab.busy);
 726        if (!S.matlab.connected) {
 727          status.className = 'hint warn-text';
 728          status.textContent = 'MATLAB is not connected. Run SPOT in MATLAB, in this project\'s folder.';
 729        } else if (S.authError && !S.settings) {
 730          status.className = 'hint bad';
 731          status.textContent = S.authError;
 732        } else if (S.error) {
 733          status.className = 'hint bad';
 734          status.textContent = 'Fix the settings first (see the message at the top).';
 735        } else if (sim.finished && sim.ok === false) {
 736          status.className = 'hint bad';
 737          const det = sim.error_details;
 738          status.innerHTML = `✕ ${esc(sim.error)}` + (det ? ' <span class="err-actions">' +
 739            '<button type="button" class="btn btn-sm" data-err="details" title="Show the causes, the blocks and MATLAB\'s full report in the command window below">Details</button>' +
 740            (det.blocks && det.blocks.length ? `<button type="button" class="btn btn-sm" data-err="block" title="Open ${esc(det.blocks[0])} in Simulink, highlighted">Show Block</button>` : '') +
 741            '</span>' : '');
 742        } else if (sim.finished && sim.summary) {
 743          const sm = sim.summary;
 744          status.className = 'hint ok';
 745          status.textContent = `${sm.stopped ? '■ Stopped early' : '✓ Simulated'} ${fmt(S.result ? S.result.t[S.result.t.length - 1] : sm.tsim, 4)} s in ${fmt(sm.seconds, 3)} s · ${sm.samples} samples logged (dataClass in the workspace)`;
 746        } else {
 747          status.className = 'hint';
 748          status.textContent = S.settings ? `Ready: ${S.settings.diagram} · ${fmt(S.derived ? S.derived.tsim : 0, 6)} s` : 'Loading settings…';
 749        }
 750      }
 751      for (const b of ['#simOpen', '#simSaveAs', '#simReset']) $(b).disabled = !S.matlab.connected || Boolean(sim.running);
 752      // Save results: next to Run, once there is a simulation to save
 753      $('#simSaveResult').hidden = !S.result || Boolean(sim.running);
 754      $('#simSaveResult').disabled = !S.matlab.connected;
 755      const file = $('#settingsFile');
 756      file.textContent = (S.file || 'spot_settings.json').split(/[\\/]/).pop();
 757      file.title = S.file ? `${S.file} (saved automatically)` : '';
 758      $('#settingsBar .sb-label').title = `Settings: ${S.file || 'spot_settings.json'}, saved automatically. They are what the Simulation tab shows (scenario, timeline, initial conditions, platforms, control, solver), used for simulations and for building experiments.`;
 759    }
 760  
 761    async function loadResult(announce = false) {
 762      const res = await api('/api/sim/result');
 763      S.result = res;
 764      if (announce) {
 765        // What Simulink printed while simulating (kept out of MATLAB's own Command Window)
 766        const sm = S.sim.summary || {};
 767        const n = sm.warnings || 0;
 768        const head = `${sm.stopped ? 'Simulation stopped' : 'Simulation finished'} in ${fmt(sm.seconds, 3)} s · ` +
 769          (n ? `${n} warning${n === 1 ? '' : 's'} from Simulink (click to show)` : 'no warnings');
 770        if (res.messages) printDetails(head, res.messages, n ? 'c-warn' : 'c-info');
 771        else print(head, 'c-info');
 772      }
 773      S.resultId = S.sim.result_id;
 774      messages?.simResult(res.debug);
 775      livePlot?.simResult(res.live);
 776      viewer?.setSimulation(res);
 777      if (S.shown) layers();
 778      renderPlayback();
 779      seek(span()[0]);
 780      if (S.shown) play(true);
 781      renderRun();
 782    }
 783  
 784    // ---------------------------------------------------------------- playback
 785    // The last simulation, and any loaded results laid over it (Compare), on one clock: each
 786    // overlay can be shifted by an offset [s] to line it up.
 787  
 788    const scrub = $('#pbScrub');
 789    S.primaryShown = true;   // not remembered: a new page always shows the last simulation
 790    S.overlays = [];   // [{id, result, label, offset, phases}] as given to the viewer
 791  
 792    function tracks() {
 793      const out = [];
 794      if (S.result && S.primaryShown) out.push({ result: S.result, offset: 0 });
 795      return out.concat(S.overlays);
 796    }
 797    const hasPlayback = () => tracks().length > 0;
 798    function span() {
 799      let t0 = Infinity, t1 = -Infinity;
 800      for (const k of tracks()) {
 801        const t = k.result.t;
 802        t0 = Math.min(t0, t[0] + k.offset);
 803        t1 = Math.max(t1, t[t.length - 1] + k.offset);
 804      }
 805      return isFinite(t0) ? [t0, t1] : [0, 0];
 806    }
 807    function phases() {
 808      const k = tracks().find(x => x.result.phases);
 809      if (!k) return { names: [], ends: [] };
 810      return { names: list(k.result.phases.names), ends: list(k.result.phases.ends).map(e => e + k.offset) };
 811    }
 812    function renderPlayback() {
 813      const pb = $('#playback');
 814      const any = hasPlayback();
 815      pb.hidden = !any && !S.result && !S.overlays.length && !store.loaded.some(d => d.crafts.length);
 816      $('#simEmpty').classList.toggle('hidden', any || Boolean(S.result));
 817      $('#pbPlay').disabled = !any;
 818      scrub.disabled = !any;
 819      renderCompare();
 820      if (!any) { $('#pbPhases').innerHTML = ''; $('#pbTime').textContent = '—'; $('#pbPhase').textContent = ''; return; }
 821      const [t0, t1] = span();
 822      $('#pbPhases').innerHTML = phases().ends.slice(0, -1).filter(e => e > t0 && e < t1)
 823        .map(e => `<i style="left:${(e - t0) / (t1 - t0) * 100}%" title="${fmt(e, 5)} s"></i>`).join('');
 824    }
 825    function seek(t) {
 826      if (!hasPlayback()) return;
 827      const [t0, t1] = span();
 828      S.t = Math.min(Math.max(t, t0), t1);
 829      viewer?.setSimTime(S.t);
 830      messages?.simTime(S.t);
 831      livePlot?.simTime(S.t);
 832      if (document.activeElement !== scrub) scrub.value = Math.round((S.t - t0) / Math.max(t1 - t0, 1e-9) * 1000);
 833      $('#pbTime').textContent = `${S.t.toFixed(1)} / ${t1.toFixed(1)} s`;
 834      const ph = phases();
 835      const k = ph.ends.findIndex(e => S.t <= e + 1e-9);
 836      $('#pbPhase').textContent = k >= 0 ? ph.names[k] : '';
 837    }
 838    function play(on) {
 839      if (!hasPlayback()) on = false;
 840      else if (on && S.t >= span()[1] - 1e-6) seek(span()[0]);
 841      S.playing = on;
 842      $('#pbPlay').textContent = on ? '❚❚' : '▶';
 843      S.lastTick = performance.now();
 844      if (on) requestAnimationFrame(tick);
 845    }
 846    function tick(now) {
 847      if (!S.playing) return;
 848      const dt = (now - S.lastTick) / 1000;
 849      S.lastTick = now;
 850      seek(S.t + dt * Number($('#pbSpeed').value));
 851      if (S.t >= span()[1] - 1e-9) return play(false);
 852      requestAnimationFrame(tick);
 853    }
 854    $('#pbPlay').onclick = () => play(!S.playing);
 855    scrub.addEventListener('input', () => {
 856      if (!hasPlayback()) return;
 857      const [t0, t1] = span();
 858      play(false);
 859      seek(t0 + (t1 - t0) * scrub.value / 1000);
 860    });
 861    const traces = $('#pbTraces');
 862    traces.checked = storage.get('spot.sim.traces', true);
 863    traces.onchange = () => { storage.set('spot.sim.traces', traces.checked); viewer?.setTraces(traces.checked); };
 864    $('#pbSpeed').value = String(storage.get('spot.sim.speed', 5));
 865    $('#pbSpeed').onchange = () => storage.set('spot.sim.speed', Number($('#pbSpeed').value));
 866    document.addEventListener('keydown', ev => {
 867      if (!S.shown || ev.key !== ' ' || ev.target.closest('input, textarea, select, button')) return;
 868      ev.preventDefault();
 869      play(!S.playing);
 870    });
 871  
 872    function layers() {
 873      viewer?.setLayers({ live: false, sim: hasPlayback(), setup: S.pose !== 'off' });
 874      viewer?.setPrimaryShown(S.primaryShown);
 875      viewer?.setTraces(traces.checked);
 876    }
 877  
 878    // ---------------------------------------------------------------- compare: results over the simulation
 879  
 880    const pop = $('#pbOverlays');
 881    $('#pbCompare').onclick = ev => {
 882      ev.stopPropagation();
 883      pop.hidden = !pop.hidden;
 884      renderCompare();
 885      if (!pop.hidden) act(() => store.refresh());
 886    };
 887    document.addEventListener('pointerdown', ev => {
 888      if (!pop.hidden && !ev.target.closest('#pbOverlays, #pbCompare')) { pop.hidden = true; renderCompare(); }
 889    });
 890  
 891    function renderCompare() {
 892      const sets = store.loaded.filter(d => d.crafts.length);
 893      const n = S.overlays.length;
 894      const count = $('#pbCompare .pb-compare-count');
 895      count.hidden = !n;
 896      count.textContent = n;
 897      $('#pbCompare').classList.toggle('on', n > 0);
 898      $('#pbCompare').setAttribute('aria-expanded', String(!pop.hidden));
 899      if (pop.hidden) return;
 900      const row = d => {
 901        const o = store.overlay(d.id);
 902        const waiting = o.on && !S.overlays.some(x => x.id === d.id);
 903        return `<div class="pb-ov-row"><label class="toggle"><input type="checkbox" data-ov="${d.id}"${o.on ? ' checked' : ''}><span></span>
 904            <span class="pill">${kindTag(d.kind)}</span>${esc(d.name)}${waiting ? ' <span class="spinner dark"></span>' : ''}</label>
 905          <label class="pb-ov-offset" title="Shift this result in time [s] to line it up with the others">±<input class="input" type="number" step="0.1" data-offset="${d.id}" value="${o.offset || 0}">s</label></div>`;
 906      };
 907      pop.innerHTML = `
 908        <div class="pb-ov-head">Replay on the Table</div>
 909        ${S.result ? `<div class="pb-ov-row"><label class="toggle"><input type="checkbox" data-primary${S.primaryShown ? ' checked' : ''}><span></span>
 910          <span class="pill">SIM</span>Last Simulation</label></div>` : ''}
 911        ${sets.map(row).join('') || '<p class="hint">No results loaded with platform positions. Load an experiment run or a saved simulation to replay it over this one.</p>'}
 912        <p class="hint">Overlays: outlined platforms and dashed paths, named after the result.</p>
 913        <div class="pb-ov-foot">
 914          <button type="button" class="btn btn-sm" data-pick>Load Results…</button>
 915          <button type="button" class="btn btn-sm" data-keep ${S.result && S.matlab.connected ? '' : 'disabled'} title="Keep a copy of this simulation to compare with the next ones">Keep This Simulation</button>
 916        </div>`;
 917    }
 918    pop.addEventListener('change', ev => {
 919      const el = ev.target;
 920      if (el.dataset.primary !== undefined) {
 921        S.primaryShown = el.checked;
 922        layers(); renderPlayback(); seek(S.t);
 923      } else if (el.dataset.ov) {
 924        store.setOverlay(el.dataset.ov, { on: el.checked });
 925      } else if (el.dataset.offset) {
 926        const v = Number(el.value);
 927        if (isFinite(v)) store.setOverlay(el.dataset.offset, { offset: v });
 928      }
 929    });
 930    pop.addEventListener('click', ev => {
 931      if (ev.target.closest('[data-pick]')) act(async () => showOverlays(await store.pick()));
 932      if (ev.target.closest('[data-keep]')) act(async () => showOverlays(await store.keepSimulation()));
 933    });
 934    $('#simEmptyLoad').onclick = () => act(async () => showOverlays(await store.pick()));
 935    function showOverlays(got) {
 936      for (const d of got || []) if (d.crafts.length) store.setOverlay(d.id, { on: true });
 937      if ((got || []).length && !got.some(d => d.crafts.length)) toast('That result has no platform positions to replay; plot it on the Results page.', 'info', 6000);
 938    }
 939  
 940    // Follow the store: fetch the motion of each result turned on, then hand them to the viewer
 941    let syncing = 0;
 942    async function syncOverlays() {
 943      const run = ++syncing;
 944      const want = store.loaded.filter(d => d.crafts.length && store.overlay(d.id).on);
 945      renderPlayback();
 946      const out = [];
 947      for (const d of want) {
 948        try {
 949          const result = await store.trajectory(d.id);
 950          out.push({ id: d.id, result, label: d.name, offset: store.overlay(d.id).offset || 0 });
 951        } catch (e) {
 952          toast(`${d.name}: ${e.message}`, 'error', 8000);
 953          store.setOverlay(d.id, { on: false });
 954          return;
 955        }
 956      }
 957      if (run !== syncing) return;   // changed again meanwhile
 958      const before = hasPlayback();
 959      S.overlays = out;
 960      viewer?.setOverlays(out);
 961      if (S.shown) layers();
 962      renderPlayback();
 963      if (!before && hasPlayback()) seek(span()[0]); else seek(S.t);
 964    }
 965    store.on(syncOverlays);
 966  
 967    // ---------------------------------------------------------------- command window
 968  
 969    const out = $('#cmdOut'), input = $('#cmdIn');
 970    function print(textOut, cls) {
 971      const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
 972      const div = document.createElement('div');
 973      if (cls) div.className = cls;
 974      div.textContent = textOut;
 975      out.append(div);
 976      while (out.childElementCount > 2000) out.firstElementChild.remove();
 977      if (atBottom) out.scrollTop = out.scrollHeight;
 978    }
 979    function printDetails(summary, textOut, cls) {
 980      const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
 981      const d = document.createElement('details');
 982      if (cls) d.className = cls;
 983      const sm = document.createElement('summary');
 984      sm.textContent = summary;
 985      const body = document.createElement('div');
 986      body.className = 'c-details';
 987      body.textContent = textOut;
 988      d.append(sm, body);
 989      out.append(d);
 990      if (atBottom) out.scrollTop = out.scrollHeight;
 991    }
 992  
 993    // An error from MATLAB as an entry in the command window: the root cause, with (on click)
 994    // where it is (your code, the blocks, each with a button to show it in Simulink), the chain
 995    // of causes, what Simulink printed before it and MATLAB's full report
 996    function printError(title, det) {
 997      const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
 998      const d = document.createElement('details');
 999      d.className = 'c-err err-entry';
1000      const sm = document.createElement('summary');
1001      sm.textContent = `${title}: ${det.summary} (click for details)`;
1002      const body = document.createElement('div');
1003      body.className = 'c-details';
1004      const line = (text, cls) => {
1005        const el = document.createElement('div');
1006        if (cls) el.className = cls;
1007        el.textContent = text;
1008        body.append(el);
1009        return el;
1010      };
1011      if (det.where) line(`In your code: ${det.where}`, 'err-where');
1012      for (const b of list(det.blocks)) {
1013        const el = line(`Block: ${b} `, 'err-block');
1014        const btn = document.createElement('button');
1015        btn.type = 'button';
1016        btn.className = 'btn btn-sm';
1017        btn.textContent = 'Show in Simulink';
1018        btn.onclick = () => showBlock(b);
1019        el.append(btn);
1020      }
1021      const chain = list(det.chain);
1022      if (chain.length > 1) {
1023        line('Causes, from the first reported to the root cause:', 'err-head');
1024        for (const c of chain) line(`${'  '.repeat(c.depth)}${c.depth ? '↳ ' : ''}${c.message}`, 'err-cause');
1025      }
1026      const more = (label, text) => {
1027        if (!text) return;
1028        const m = document.createElement('details');
1029        const s = document.createElement('summary');
1030        s.textContent = label;
1031        const pre = document.createElement('div');
1032        pre.className = 'err-text';
1033        pre.textContent = text;
1034        m.append(s, pre);
1035        body.append(m);
1036      };
1037      more('What Simulink Printed Before the Error', det.output);
1038      more("MATLAB's Full Report", det.report);
1039      d.append(sm, body);
1040      out.append(d);
1041      if (atBottom) out.scrollTop = out.scrollHeight;
1042      return d;
1043    }
1044    async function showBlock(block) {
1045      toast('Opening the block in Simulink…', 'info', 3000);   // (the first time takes a while)
1046      try {
1047        await matlab('diagram.showBlock', { block });
1048        toast(`Opened in Simulink: ${block.split('/').slice(1).join('/')}`, 'success', 4000);
1049      } catch (e) {
1050        toast(e.message, 'error', 8000);
1051      }
1052    }
1053  
1054    // The status line's buttons after a failed simulation
1055    $('#simStatus').addEventListener('click', ev => {
1056      const b = ev.target.closest('[data-err]');
1057      const det = S.sim && S.sim.error_details;
1058      if (!b || !det) return;
1059      if (b.dataset.err === 'block') { showBlock(list(det.blocks)[0]); return; }
1060      if (!S.errorEntry || !S.errorEntry.isConnected) S.errorEntry = printError('Simulation failed', det);
1061      S.errorEntry.open = true;
1062      S.errorEntry.scrollIntoView({ block: 'nearest' });
1063    });
1064  
1065    async function runCommand(cmd) {
1066      if (!cmd.trim()) return;
1067      S.history = [...S.history.filter(h => h !== cmd), cmd].slice(-200);
1068      storage.set('spot.cmdHistory', S.history);
1069      S.histPos = -1;
1070      if (cmd.trim() === 'clc') { out.textContent = ''; return; }
1071      print(`>> ${cmd}`, 'c-cmd');
1072      try {
1073        const r = await matlab('workspace.eval', { command: cmd });
1074        if (r.output) print(r.output.replace(/^\n+|\n+$/g, ''), '');
1075        if (r.error) print(r.error, 'c-err');
1076      } catch (e) {
1077        print(e.message, 'c-err');
1078      }
1079      refreshWorkspace();
1080    }
1081    input.addEventListener('keydown', ev => {
1082      if (ev.key === 'Enter' && !ev.shiftKey) {
1083        ev.preventDefault();
1084        const cmd = input.value;
1085        input.value = '';
1086        autosize();
1087        runCommand(cmd);
1088      } else if ((ev.key === 'ArrowUp' || ev.key === 'ArrowDown') && !input.value.includes('\n') && S.history.length) {
1089        ev.preventDefault();
1090        const n = S.history.length;
1091        S.histPos = ev.key === 'ArrowUp' ? (S.histPos < 0 ? n - 1 : Math.max(0, S.histPos - 1))
1092                                         : (S.histPos < 0 ? -1 : S.histPos + 1);
1093        if (S.histPos >= n) S.histPos = -1;
1094        input.value = S.histPos < 0 ? '' : S.history[S.histPos];
1095        autosize();
1096      }
1097    });
1098    function autosize() { input.style.height = 'auto'; input.style.height = Math.min(input.scrollHeight, 140) + 'px'; }
1099    input.addEventListener('input', autosize);
1100    $('#cmdClear').onclick = () => { if ($('#matlabConsole').dataset.tab !== 'plot') out.textContent = ''; };
1101    $('#toggleMatlab').onclick = () => {
1102      const c = $('#matlabConsole');
1103      c.classList.toggle('collapsed');
1104      $('#toggleMatlab').textContent = c.classList.contains('collapsed') ? '▴' : '▾';
1105      storage.set('spot.sim.consoleCollapsed', c.classList.contains('collapsed'));
1106    };
1107    if (storage.get('spot.sim.consoleCollapsed', false)) $('#toggleMatlab').click();
1108  
1109    // ---------------------------------------------------------------- workspace
1110  
1111    let wsTimer = null;
1112    function refreshWorkspaceSoon() { clearTimeout(wsTimer); wsTimer = setTimeout(refreshWorkspace, 800); }
1113    async function refreshWorkspace() {
1114      if (!S.matlab.connected || S.sim.running) return;
1115      try {
1116        S.workspace = await matlab('workspace.list') || [];
1117        if (!Array.isArray(S.workspace)) S.workspace = [S.workspace];
1118      } catch (e) {
1119        return;  // busy: the next refresh catches up
1120      }
1121      renderWorkspace();
1122      if (S.inspected) inspect(S.inspected, true);
1123    }
1124    function renderWorkspace() {
1125      const f = $('#wsFilter').value.trim().toLowerCase();
1126      const rows = S.workspace.filter(v => !f || v.name.toLowerCase().includes(f));
1127      $('#wsCount').textContent = S.workspace.length ? `${S.workspace.length}` : '';
1128      $('#wsBody').innerHTML = rows.map(v => `<tr data-name="${esc(v.name)}" class="${v.name === S.inspected ? 'sel' : ''}">
1129        <td class="mono">${esc(v.name)}</td><td class="mono" title="${esc(v.value)}">${esc(v.value)}</td>
1130        <td class="mono">${esc(v.size)}</td><td>${esc(v.class)}</td></tr>`).join('');
1131    }
1132    $('#wsFilter').addEventListener('input', renderWorkspace);
1133    $('#wsRefresh').onclick = () => refreshWorkspace();
1134    $('#wsBody').addEventListener('click', ev => {
1135      const tr = ev.target.closest('tr[data-name]');
1136      if (tr) inspect(tr.dataset.name);
1137    });
1138    async function inspect(name, quiet) {
1139      let r;
1140      try {
1141        r = await matlab('workspace.inspect', { name });
1142      } catch (e) {
1143        if (!quiet) toast(e.message, 'error');
1144        if (quiet) { S.inspected = null; $('#wsInspect').hidden = true; }
1145        return;
1146      }
1147      S.inspected = name;
1148      renderWorkspace();
1149      const box = $('#wsInspect');
1150      box.hidden = false;
1151      let body = '';
1152      if (r.values !== undefined) {
1153        const rows = Array.isArray(r.values) ? (Array.isArray(r.values[0]) ? r.values : [r.values]) : [[r.values]];
1154        body = `<table class="gain-table k-table"><tbody>${rows.map(row => `<tr>${(Array.isArray(row) ? row : [row]).map(v => `<td>${fmt(v, 5)}</td>`).join('')}</tr>`).join('')}</tbody></table>` +
1155               (r.truncated ? '<p class="hint">First 50 rows and 12 columns.</p>' : '') + (r.note ? `<p class="hint">${esc(r.note)}</p>` : '');
1156      } else if (r.fields !== undefined) {
1157        const fields = Array.isArray(r.fields) ? r.fields : [r.fields];
1158        body = `<table class="ws-table"><tbody>${fields.map(f => `<tr data-field="${esc(name + '.' + f.name)}"><td class="mono">${esc(f.name)}</td><td class="mono">${esc(f.value)}</td><td class="mono">${esc(f.size)}</td><td>${esc(f.class)}</td></tr>`).join('')}</tbody></table>`;
1159      } else if (r.series) {
1160        body = sparkline(r.series);
1161      } else {
1162        body = `<pre class="mono">${esc(r.text || '')}</pre>`;
1163      }
1164      const up = name.includes('.') ? `<button type="button" class="btn btn-ghost btn-sm" data-up="${esc(name.replace(/\.[^.]*$/, ''))}">↑</button>` : '';
1165      box.innerHTML = `<div class="ws-inspect-head">${up}<span class="mono"><b>${esc(name)}</b> · ${esc(r.size)} ${esc(r.class)}</span>
1166        <button type="button" class="btn btn-ghost btn-sm" data-close>✕</button></div><div class="ws-inspect-body">${body}</div>`;
1167    }
1168    $('#wsInspect').addEventListener('click', ev => {
1169      const field = ev.target.closest('[data-field]');
1170      if (field) return inspect(field.dataset.field);
1171      const up = ev.target.closest('[data-up]');
1172      if (up) return inspect(up.dataset.up);
1173      if (ev.target.closest('[data-close]')) { S.inspected = null; $('#wsInspect').hidden = true; renderWorkspace(); }
1174    });
1175    function sparkline(series) {
1176      const t = Array.isArray(series.t) ? series.t : [series.t];
1177      let data = series.data;
1178      if (!Array.isArray(data[0])) data = [data];
1179      const W = 600, H = 120, colors = ['#4c8dff', '#3ccf7e', '#f5a524', '#ff6b6b', '#be4bdb', '#22b8cf'];
1180      const t0 = t[0], t1 = t[t.length - 1] || 1;
1181      const lines = data.map((row, k) => {
1182        const lo = Math.min(...row), hi = Math.max(...row), r = hi - lo || 1;
1183        const pts = row.map((v, i) => `${((t[i] - t0) / (t1 - t0 || 1) * W).toFixed(1)},${(H - 4 - (v - lo) / r * (H - 8)).toFixed(1)}`).join(' ');
1184        return { pts, color: colors[k % colors.length], lo, hi };
1185      });
1186      return `<svg class="spark" viewBox="0 0 ${W} ${H}" preserveAspectRatio="none">${lines.map(l => `<polyline points="${l.pts}" fill="none" stroke="${l.color}" stroke-width="1.5" vector-effect="non-scaling-stroke"/>`).join('')}</svg>
1187        <p class="hint">${fmt(t0, 4)} to ${fmt(t1, 4)} s${series.units ? ` · ${esc(series.units)}` : ''} · ${lines.map(l => `<span style="color:${l.color}">${fmt(l.lo, 4)} … ${fmt(l.hi, 4)}</span>`).join(' · ')}</p>`;
1188    }
1189  
1190    // ---------------------------------------------------------------- status from the server
1191  
1192    function renderBadge() {
1193      const b = $('#matlabBadge'), m = S.matlab;
1194      if (!m.connected) { b.className = 'pill err'; b.textContent = 'Not Connected'; return; }
1195      const rel = (m.info && m.info.release) || 'MATLAB';
1196      if (m.busy) {
1197        b.className = 'pill warn';
1198        b.textContent = `${rel} · ${m.busy.label || m.busy.cmd} · ${Math.round(Date.now() / 1000 - m.busy.since)} s`;
1199      } else {
1200        b.className = 'pill ok';
1201        b.textContent = `${rel} · Idle`;
1202      }
1203    }
1204  
1205    setInterval(() => { if (S.sim.running) renderRun(); if (S.matlab.busy) renderBadge(); }, 1000);
1206  
1207    return {
1208      printError,
1209      onState(st) {
1210        const was = S.matlab.connected;
1211        S.matlab = st.matlab || { connected: false };
1212        S.sim = st.sim || {};
1213        renderBadge();
1214        renderRun();
1215        if (S.matlab.connected && !was) act(loadSettings).then(refreshWorkspace);
1216        if (S.sim.ok && S.sim.result_id && S.sim.result_id !== S.resultId) act(loadResult);
1217      },
1218      onMatlab(m) {
1219        const was = S.matlab.connected;
1220        S.matlab = m;
1221        renderBadge();
1222        renderRun();
1223        if (m.connected && !was) act(loadSettings).then(refreshWorkspace);
1224      },
1225      onSim(sim) {
1226        const finished = S.sim.running && !sim.running;
1227        S.sim = sim;
1228        renderRun();
1229        if (finished) {
1230          if (sim.ok) {
1231            act(() => loadResult(true));
1232            refreshWorkspace();
1233            toast(sim.summary && sim.summary.stopped ? 'Simulation stopped' : 'Simulation finished', 'success', 3000);
1234          } else {
1235            toast(`Simulation failed: ${sim.error}`, 'error', 12000);
1236            if (sim.error_details) S.errorEntry = printError('Simulation failed', sim.error_details);
1237            else print(`Simulation failed: ${sim.error}`, 'c-err');
1238          }
1239        }
1240      },
1241      onMatlabEvent(ev) {
1242        if (ev.event === 'sim_progress') { S.progress = ev; renderRun(); }
1243        if (ev.event === 'sim_debug') messages?.simMessages(ev.messages);
1244        if (ev.event === 'sim_live') livePlot?.simData(ev.streams);
1245      },
1246      show() {
1247        S.shown = true;
1248        layers();
1249        renderSetup();
1250        if (hasPlayback()) viewer?.setSimTime(S.t);
1251      },
1252      hide() {
1253        S.shown = false;
1254        play(false);
1255        renderSetup();   // the poses stay visible elsewhere, but can't be dragged there
1256      },
1257      hasResult: () => Boolean(S.result),
1258      getState: () => ({ settings: S.settings, derived: S.derived, result: S.result, pose: S.pose }),
1259      setPlatform(name, on) {
1260        // Switch a platform on or off, as its chip on Scenario does (the arm goes with RED)
1261        if (!S.settings) return;
1262        setPath(S.settings, `platforms.${name}`, on);
1263        if (name === 'RED' && !on) setPath(S.settings, 'platforms.ARM', false);
1264        edited(true);
1265      },
1266      setArm(on, mount) {
1267        // The arm on the Chaser (the Designer's Arm), at mount {face, side}
1268        if (!S.settings) return;
1269        setPath(S.settings, 'platforms.ARM', on);
1270        if (on) setPath(S.settings, 'platforms.RED', true);
1271        if (on && mount) setPath(S.settings, 'mass.arm.mount', { face: mount.face, side: mount.side });
1272        edited(true);
1273      },
1274      setSetting(path, value) {
1275        // A setting edited on another page (e.g. dataRate, the Experiment tab's Frequency)
1276        if (!S.settings) return false;
1277        setPath(S.settings, path, value);
1278        edited();
1279        return true;
1280      },
1281      renderSetup,
1282    };
1283  }
```

</details>
<!-- /listing -->
