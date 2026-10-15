# calibration.js and calviz.js

**Files:** `+spot/+console/webapp/static/calibration.js`, `static/calviz.js` · **Page:** [[Calibration Tab|Calibration-Tab]] · **Server:** [[calibration.py|Console-calibration]]

<!-- source: +spot/+console/webapp/static/calibration.js -->
<!-- source: +spot/+console/webapp/static/calviz.js -->

## What they are for

`calibration.js` is the Calibration tab on both consoles (Table Slope, Thrusters, Arm): on the lab console it starts and stops the calibrations and follows them live; on the local console it shows the project's calibration next to the lab's, updates it, and asks (the sync dialog) when the lab's is newer. `calviz.js` draws: the jet colour map, the table's height surface, the top-down table map, and the canvas the 3D view lays on the table top ([[viewer.js|Page-viewer-js]], `setTableSlope`).

## calibration.js, function by function

**Lines 23–54.** The steps of the calibrations for the steppers: the table's (Rest, Float, Drift Map, Fit), the thrusters' (Float, Fire, Return, Rest) and the arm's four tests (Breakaway, Speed Sweep, Inertia, Position Steps), each with its sentence; the joints' names and the minutes each arm test takes per joint.

**Lines 56–72.** `initCalibration`: the state (this console's calibration; the lab console's run and the mass properties it assumes; on the local console, the lab console and its calibration; the section, the platform, the three forms, the Chaser's arm setup, the test the arm figure shows).

**Lines 74–81.** `render`: the section switcher, the section, and the arm figure's animation.

**Lines 83–106.** `syncCard` (local console, at the top of the left column): this project's date and the lab's, a pill, **Update from Lab**, and **Ask When the Lab's Is Newer**.

**Lines 108–161.** `tableSection` and `tableStats`.

**Lines 163–213.** `runCard` (the table run), `labOnlyCard`, `progressCard`.

**Lines 215–245.** `thrustersSection`: How It Works, Run, progress, and the run under way or the results.

**Lines 247–274.** `thrustRunCard` and `busyOther`.

**Lines 276–308.** `thrustResults`: the platform with each push, the bars, the table, each burn's path, what it assumed.

**Lines 310–339.** The thrusters' assumptions dialog (`#calThrustDialog`) and `readThrForm`.

**Lines 341–366.** `armSection`: How It Works (the arm figure, animated with the test picked in the stepper, or the run's; the four tests), the Chaser's setup, Run, progress, and the results.

**Lines 368–384.** `armChecksCard`: the Chaser's `arm-cal-check`, item by item (✓, ✕ with what to do, ! for the latency timer), **Check Again**.

**Lines 386–402.** `armRunCard`: the joints and tests (chips), **Rehearse (Simulated Servos)**, how long it takes, **Start Arm Calibration…** (only when the Chaser is ready) or **Start Rehearsal**, or **Stop**.

**Lines 404–415.** `armProgressCard` (a progress bar over the tests, and the script's own log) and `armResult` (a rehearsal just run, else the lab's arm calibration).

**Lines 417–447.** `armResults`: each joint's friction and inertia in a table, the friction against speed (measured and fitted, all joints) and the position steps (goal and measured, a joint at a time), and whether it is a rehearsal (not saved).

**Lines 449–484.** The arm figure's animation (`animateArm`), `checkArm`, and `startArm`: a rehearsal starts at once; a real run first shows `#calArmDialog` (the Chaser rests, the joints move up to 25°, the power switch at hand) and starts only once its box is ticked.

**Lines 486–524.** `draw` (the table map: a thruster run's platform and burns, or the table's height and drifts, or a run under way) and `fitMap` (as large as the window allows with its whole card in view).

**Lines 531–570.** The sync prompt (local console): `maybeAsk` and `newerSections` (Table Slope, a platform's Thrusters, Arm); **Update**, **Not Now**, **Never Ask Again**.

**Lines 572–660.** Input: the section switcher (opening Arm checks the Chaser), the platform, Start, Stop, Remove, the arm's chips and Rehearse, the stepper, Update from Lab, the Ask switch, Esc, the forms.

**Lines 662–684.** From the server: `onCalibration` and `onRun`.

**Lines 686–718.** The module's interface for [[app.js|Page-app-js]] (the Arm section checks the Chaser when first shown).

## calviz.js, function by function

**Lines 11–16.** `jet`: t in 0–1 to a colour, dark blue through cyan and yellow to dark red.

**Lines 19–29.** `height` and `gradient` of the height surface (as [[calfit.py|Console-calfit]] and [[spot.sim.tableSlope|spot-sim-tableSlope]]).

**Lines 31–64.** `heightGrid` and `heightCanvas` (the map alone, for the 3D view's overlay).

**Lines 69–101.** `contours`: eight contour levels by marching squares.

**Lines 105–249.** `drawTable` (the table from above: the height map, contours, downhill arrows, the grid and ticks, the drifts or burns, the start points, the path, the goal, the platform), `platform`, `colorBar`.

**Lines 253–270.** `THRUSTERS` and `deviationColor`.

**Lines 273–337.** `thrusterDiagram` (each measured push as an arrow onto its nozzle, the firing thruster's plume), `thrustBars` (thrust ± spread against nominal, and the key), `burnPaths` (each burn's path from its start, at one scale).

**Lines 345–366.** `ARM_TEST_MOTION`: what each arm test does to a joint over a few seconds, for the animation (breakaway: still while the current rises, then a jerk each way; sweep: passes back and forth, faster each time; inertia: a torque step; steps: the goals 10, 0, −10, 20, 0, −20, 5, 0°).

**Lines 368–386.** `armFigure`: the arm on the Chaser's side, three links, the tested joint and its link lit, moving as its test does, and a word for the moment.

**Lines 389–415.** `frictionPlot`: each joint's friction torque against speed, the sweep's points and the fitted curve, coulomb + (static − coulomb) exp(−(v/stribeck)²) + viscous v, for both directions.

**Lines 417–434.** `stepsPlot`: a joint's position steps, the goal (dashed) and the measured angle against time.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/calibration.js</b> (718 lines; click to show)</summary>

```javascript
   1  // The Calibration tab (both consoles). The lab console runs the calibrations (spot_console/
   2  // calibration.py) and keeps the lab's calibration file; the local console shows its project's copy
   3  // (resources/spot_calibration.json, which the simulation uses) next to the lab's, and offers the
   4  // lab's whenever it is newer (the sync dialog: Update, Not Now, or Never Ask Again).
   5  //
   6  // Sections: Table Slope, Thrusters and Arm.
   7  
   8  import { platformName, COLORS } from './platforms.js';
   9  import { drawTable, heightGrid, colorBar, thrusterDiagram, thrustBars, burnPaths, armFigure, frictionPlot, stepsPlot } from './calviz.js';
  10  
  11  const $ = (s, el = document) => el.querySelector(s);
  12  const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  13  const NAMES = ['RED', 'BLACK', 'BLUE'];
  14  const fmt = (v, d = 2) => (v == null || !Number.isFinite(v) ? '—' : v.toFixed(d));
  15  const when = iso => {
  16    if (!iso) return 'Never';
  17    const d = new Date(iso);
  18    return d.toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' }) + ' ' +
  19      d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  20  };
  21  
  22  // The four steps of the table calibration, as the stepper shows them (and the run's steps they cover)
  23  const TABLE_STEPS = [
  24    ['Rest', 'PhaseSpace records the platform resting on the table: the cameras\' noise.',
  25     '<path d="M4 17h16M7 17v-3h10v3" /><circle cx="12" cy="9" r="2.2"/>'],
  26    ['Float', 'The air bearings come on with the thrusters off: it drifts from where it is.',
  27     '<path d="M4 18h16"/><rect x="7" y="9" width="10" height="5" rx="1"/><path d="M8 16.5h1M11.5 16.5h1M15 16.5h1"/>'],
  28    ['Drift Map', 'It flies to each point of the grid, settles, and drifts again with the thrusters off.',
  29     '<circle cx="6" cy="7" r="1.6"/><circle cx="18" cy="7" r="1.6"/><circle cx="6" cy="17" r="1.6"/><circle cx="18" cy="17" r="1.6"/><path d="M7.5 8.2 16.5 15.8" stroke-dasharray="2 2"/>'],
  30    ['Fit', 'The table\'s height surface and the air bearings\' drag are fitted to every drift.',
  31     '<path d="M4 18c3-8 6-10 8-10s5 2 8 10"/><path d="M4 18h16"/>'],
  32  ];
  33  
  34  // The thruster calibration's four steps, for each thruster in turn
  35  const THRUSTER_STEPS = [
  36    ['Float', 'The air bearings come on where the platform rests.',
  37     '<path d="M4 18h16"/><rect x="7" y="9" width="10" height="5" rx="1"/><path d="M8 16.5h1M11.5 16.5h1M15 16.5h1"/>'],
  38    ['Fire', 'One thruster fires at full thrust for the burn time; PhaseSpace records the push.',
  39     '<rect x="6" y="8" width="10" height="8" rx="1"/><path d="M16 12h5M18 9.5l3 2.5-3 2.5"/>'],
  40    ['Return', 'The diagram\'s controller flies it back to where it started.',
  41     '<path d="M19 12a7 7 0 1 1-2-4.9"/><path d="M17 3v4h-4"/>'],
  42    ['Rest', 'Pucks off, a moment\'s rest, then the next thruster (1 to 8, and each repeat).',
  43     '<path d="M4 17h16M7 17v-3h10v3"/>'],
  44  ];
  45  
  46  // The arm calibration's four tests (calibrate_arm.py), each on one joint while the others hold
  47  const ARM_TESTS = [
  48    ['breakaway', 'Breakaway', 'The motor current creeps up from zero until the joint moves, three times each way: the static friction (breakaway torque).'],
  49    ['sweep', 'Speed Sweep', 'The joint turns back and forth at nine speeds; the current it needs at each is the friction at that speed (Coulomb, Stribeck, viscous).'],
  50    ['inertia', 'Inertia', 'A torque step from rest, each way: how fast it accelerates gives the joint\'s inertia (link and rotor).'],
  51    ['steps', 'Position Steps', 'Goals of 10, 0, -10, 20, 0, -20, 5, 0° with SPOT\'s servo settings, for spot.arm.compareSteps to check the model.'],
  52  ];
  53  const JOINTS = { 1: 'Shoulder', 2: 'Elbow', 3: 'Wrist' };
  54  const ARM_MINUTES = { breakaway: 1.5, sweep: 2.5, inertia: 0.5, steps: 0.5 };   // per joint, about
  55  
  56  export function initCalibration({ api, act, toast, storage, viewer, getStream = () => null }) {
  57    const S = {
  58      local: false, shown: false, section: storage.get('spot.calibration.section', 'table'),
  59      cal: null, path: null,                 // this console's calibration
  60      run: null, active: false,              // the lab console's run (lab console only)
  61      lab: null, labCal: null,               // local console: the lab console and its calibration
  62      platform: storage.get('spot.calibration.platform', 'RED'),
  63      form: storage.get('spot.calibration.table', { nx: 3, ny: 2, drift: 12, margin: 0.35, model: 'auto' }),
  64      thrForm: storage.get('spot.calibration.thrusters', { repeats: 2, burn: 2, margin: 0.35 }),
  65      armForm: storage.get('spot.calibration.arm', { joints: [1, 2, 3], tests: ['breakaway', 'sweep', 'inertia', 'steps'], simulate: false }),
  66      armCheck: null, armChecking: false, armDemo: { test: 'breakaway', joint: 1 }, armStepsJoint: 1,
  67      assumed: null,                         // lab console: what it takes each platform's mass properties to be
  68      asked: null,                           // the lab calibration already offered this session
  69    };
  70    const body = $('#calBody'), head = body;   // (the sync card is in the body's left column)
  71  
  72    // ---------------------------------------------------------------- rendering
  73  
  74    function render() {
  75      for (const b of document.querySelectorAll('[data-calsec]')) b.classList.toggle('active', b.dataset.calsec === S.section);
  76      if (S.section === 'table') body.innerHTML = tableSection();
  77      else if (S.section === 'thrusters') body.innerHTML = thrustersSection();
  78      else body.innerHTML = armSection();
  79      animateArm();
  80      draw();
  81    }
  82  
  83    function syncCard() {
  84      const mine = S.cal || {}, lab = S.labCal, reach = S.lab && S.lab.reachable;
  85      let pill = ['No Calibration Yet', ''];
  86      if (!reach) pill = ['Lab Console Not Reachable', ''];
  87      else if (!lab) pill = ['Lab Console Needs Update', 'warn'];
  88      else if (!lab.updated) pill = ['The Lab Has No Calibration', ''];
  89      else if (!mine.updated || lab.updated > mine.updated) pill = ['The Lab Has a Newer One', 'warn'];
  90      else pill = ['Up to Date', 'ok'];
  91      const ask = !mine.sync || mine.sync.ask !== false;
  92      const canTake = reach && lab && lab.updated && (!mine.updated || lab.updated !== mine.updated);
  93      return `<section class="card cal-sync">
  94        <div class="card-head"><h2>This Project and the Lab</h2><span class="pill ${pill[1]}">${pill[0]}</span></div>
  95        <div class="cal-sync-row">
  96          <div title="resources/spot_calibration.json"><span class="field-label">This Project</span><b>${when(mine.updated)}</b></div>
  97          <div title="Calibrations run on the lab console (its Calibration tab)"><span class="field-label">Lab Console</span><b>${reach && lab ? when(lab.updated) : '—'}</b></div>
  98          <div class="cal-sync-actions">
  99            <button type="button" class="btn btn-sm btn-primary" data-cal="take" ${canTake ? '' : 'disabled'}
 100              title="Copy the lab console's calibration into this project; the next simulation uses it">Update from Lab</button>
 101            <label class="toggle" title="When this computer sees the lab console and its calibration is newer than this project's, offer it"><input type="checkbox" data-cal="ask" ${ask ? 'checked' : ''}><span></span>Ask When the Lab's Is Newer</label>
 102          </div>
 103        </div>
 104        <p class="hint">Simulations use this project's copy (resources/spot_calibration.json): Scenario, Table Slope and Calibrated Thrust.</p>
 105      </section>`;
 106    }
 107  
 108    function tableSection() {
 109      const t = S.cal && S.cal.table;
 110      const run = S.run && S.run.kind === 'table' ? S.run : null;
 111      const running = run && ['running', 'stopping'].includes(run.state);
 112      // which of the four stepper steps the run is in
 113      let stage = -1;
 114      if (run && run.step != null) {
 115        const name = run.steps[run.step].name;
 116        stage = name === 'Still' ? 0 : name === 'Float Here' ? 1 : name === 'Fit' ? 3 : 2;
 117        if (!running) stage = run.state === 'done' ? 4 : stage;
 118      }
 119      const stepper = TABLE_STEPS.map(([title, text, icon], k) => {
 120        const cls = running && k === stage ? 'now' : stage > k || (run && run.state === 'done') ? 'done' : '';
 121        return `<li class="cal-step ${cls}"><svg viewBox="0 0 24 24" aria-hidden="true">${icon}</svg>
 122          <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`;
 123      }).join('');
 124      const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2></div>
 125          <ol class="cal-steps">${stepper}</ol>
 126          <p class="hint">A drift ends after its time, or at once (pucks off) when the platform nears an edge or another
 127            platform. The platform needs PhaseSpace to see it the whole time, and the table clear around the grid.</p></section>`;
 128      return `<div class="cal-col">
 129          ${S.local ? syncCard() : ''}
 130          ${howCard}
 131          ${S.local ? labOnlyCard('table') : runCard(run, running)}
 132          ${run ? progressCard(run) : ''}
 133        </div>
 134        <div class="cal-col cal-wide">
 135          <section class="card cal-map-card">
 136            <div class="card-head"><h2>Table Slope</h2>${running ? `<span class="pill live">Measuring · ${platformName(run.platform)}</span>`
 137              : t ? `<span class="pill ok">${t.model === 'quadratic' ? 'Surface' : 'Plane'} · ${when(t.updated)}</span>` : '<span class="pill">Not Calibrated</span>'}</div>
 138            <div class="cal-map"><canvas id="calMap" aria-label="The table seen from above: the fitted height in colour, downhill arrows, the drifts"></canvas></div>
 139            ${running ? '<p class="hint">The run under way: the grid of start points (the current one in yellow), the drifts so far, and the platform with its goal. The table\'s map appears when the fit is done.</p>'
 140              : t ? `${colorBar(heightGrid(t).range)}${tableStats(t)}` : '<p class="hint">No table calibration yet. Run one on the lab console: the map shows the table\'s height (blue low, red high), its contour lines, which way is downhill, and the drifts it was fitted to.</p>'}
 141          </section>
 142        </div>`;
 143    }
 144  
 145    function tableStats(t) {
 146      const fixed = t.damping_fixed || [];
 147      const drag = Object.entries(t.damping || {}).map(([n, c]) => (fixed.includes(n)
 148        ? `${platformName(n)} not resolved (longer drifts tell it from the slope; 0 used)` : `${platformName(n)} ${fmt(c, 3)} /s`)).join(', ');
 149      const tile = (label, value, unit, title) => `<div class="cal-tile" title="${esc(title)}"><span>${label}</span><b>${value}</b><i>${unit}</i></div>`;
 150      const dir = t.downhill_deg;
 151      return `<div class="cal-tiles">
 152          ${tile('Tilt at Centre', fmt(t.tilt_centre_mrad, 3), 'mrad', 'The slope at the table\'s centre: 1 mrad is a 1 mm drop per metre, and pulls a floating platform at 0.0098 m/s²')}
 153          ${tile('Steepest', fmt(t.tilt_max_mrad, 3), 'mrad', 'The steepest slope anywhere on the table')}
 154          ${tile('Downhill', `<svg class="cal-dir" viewBox="-10 -10 20 20" aria-hidden="true"><g transform="rotate(${-dir})"><path d="M-7 0H6M2-4l4 4-4 4" /></g></svg>${fmt(dir, 0)}°`, 'from +x', 'Which way is downhill at the centre, from +x towards +y')}
 155          ${tile('Fit Residual', fmt(t.rms_mm, 2), 'mm', 'How far the drifts are from the fitted model (the cameras\' own noise is ' + fmt(t.noise_mm, 2) + ' mm)')}
 156        </div>
 157        ${t.warning ? `<p class="hint warn-text">${esc(t.warning)}</p>` : ''}
 158        <p class="hint">Fitted to ${t.drifts} drift${t.drifts === 1 ? '' : 's'} of the ${platformName(t.platform)} by ${esc(t.by || '?')}.
 159          Air-bearing drag: ${drag || '—'}. ${t.model === 'plane' ? 'A plane (one tilt everywhere): the drifts didn\'t cover enough of the table for a surface.' : 'A smooth surface (quadratic).'}</p>
 160        ${S.local ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-table" title="Remove the table calibration from the lab console\'s file">Remove</button></div>'}`;
 161    }
 162  
 163    function runCard(run, running) {
 164      // (while a run goes on, its own platform and settings)
 165      const f = running ? run.params : S.form, st = getStream(), streaming = st && st.running;
 166      const chosen = running ? run.platform : S.platform;
 167      const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
 168          <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
 169      const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
 170          <input type="number" class="input" data-calform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
 171      return `<section class="card">
 172          <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : streaming ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Start PhaseSpace First</span>'}</div>
 173          <div class="segmented cal-plat" role="group" aria-label="Platform">${plat}</div>
 174          <div class="cal-form">
 175            ${field('nx', 'Grid Columns', 1, 5, 1, '', 'How many start points along x')}
 176            ${field('ny', 'Grid Rows', 1, 4, 1, '', 'How many start points along y')}
 177            ${field('drift', 'Drift Time', 4, 40, 1, 's', 'How long each drift is recorded (at most)')}
 178            ${field('margin', 'Edge Margin', 0.25, 0.8, 0.05, 'm', 'A drift ends (pucks off) when the platform\'s centre comes this close to an edge')}
 179            <label class="field" title="Quadratic: a smooth surface (needs 3+ drifts over most of the table). Plane: one tilt everywhere. Auto: chooses."><span class="field-label">Model</span>
 180              <select class="input" data-calform="model" ${running ? 'disabled' : ''}>
 181                ${['auto', 'quadratic', 'plane'].map(m => `<option value="${m}" ${f.model === m ? 'selected' : ''}>${{ auto: 'Auto', quadratic: 'Surface', plane: 'Plane' }[m]}</option>`).join('')}
 182              </select></label>
 183          </div>
 184          <p class="hint">${f.nx * f.ny + 1} drifts of up to ${f.drift} s, plus the flights between them: about ${Math.round((f.nx * f.ny + 1) * (Number(f.drift) + 25) / 60)} min. Clear the table around the grid; other platforms may stay where they are (points next to them are skipped).</p>
 185          <div class="cal-actions">
 186            ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
 187                      : `<button type="button" class="btn btn-go" data-cal="start-table" ${!streaming ? 'disabled title="Start PhaseSpace streaming first (PhaseSpace tab)"' : busyOther() ? 'disabled title="Another calibration is running"' : ''}>Start Table Calibration</button>`}
 188          </div>
 189        </section>`;
 190    }
 191  
 192    function labOnlyCard(what) {
 193      return `<section class="card"><div class="card-head"><h2>Run</h2><span class="pill">On the Lab Console</span></div>
 194          <p class="hint">The ${what === 'table' ? 'table slope' : what} calibration drives a platform, so it runs on the lab console
 195            (its Calibration tab${S.lab && S.lab.url ? `, <a href="${esc(S.lab.url)}#calibration" target="_blank" rel="noopener">${esc(S.lab.url)}</a>` : ''}).
 196            Its results come here with Update from Lab.</p></section>`;
 197    }
 198  
 199    function progressCard(run) {
 200      const icon = { waiting: '○', running: '◉', done: '✓', skipped: '–', failed: '✕' };
 201      const steps = run.steps.map((s, k) => `<li class="cal-prog ${s.state}${k === run.step ? ' current' : ''}"><span>${icon[s.state] || '○'}</span>
 202          <b>${esc(s.name)}</b><i>${esc(s.note || '')}</i></li>`).join('');
 203      const kind = { running: 'live', stopping: 'warn', done: 'ok', failed: 'err', stopped: 'warn' }[run.state] || '';
 204      const label = { running: 'Running', stopping: 'Stopping', done: 'Done', failed: 'Failed', stopped: 'Stopped' }[run.state] || run.state;
 205      return `<section class="card">
 206          <div class="card-head"><h2>Progress · ${platformName(run.platform)}</h2><span class="pill ${kind}">${label}</span></div>
 207          <p class="cal-msg ${run.state === 'failed' ? 'bad' : ''}">${esc(run.message)}</p>
 208          <ol class="cal-progress">${steps}</ol>
 209          <details class="advanced"><summary>Log</summary><pre class="cal-log">${esc(run.log.join('\n'))}</pre></details>
 210        </section>`;
 211    }
 212  
 213    // ---------------------------------------------------------------- the thrusters
 214  
 215    function thrustersSection() {
 216      const run = S.run && S.run.kind === 'thrusters' ? S.run : null;
 217      const running = run && ['running', 'stopping'].includes(run.state);
 218      const step = run && run.step != null ? run.steps[run.step].name : '';
 219      const firing = running && step.startsWith('Thruster ') ? Number(step.slice(9)) : null;
 220      const msg = run ? run.message : '';
 221      const stage = !running || !firing ? -1 : /firing/.test(msg) ? 1 : /flying back/.test(msg) ? 2 : /floating/.test(msg) ? 0 : 3;
 222      const stepper = THRUSTER_STEPS.map(([title, text, icon], k) =>
 223        `<li class="cal-step ${k === stage ? 'now' : run && run.state === 'done' ? 'done' : ''}"><svg viewBox="0 0 24 24" aria-hidden="true">${icon}</svg>
 224          <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`).join('');
 225      const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2>${firing ? `<span class="pill live">Thruster ${firing}</span>` : ''}</div>
 226          <ol class="cal-steps">${stepper}</ol>
 227          <div class="cal-thr-live">${thrusterDiagram({ firing: stage === 1 ? firing : null })}</div>
 228          <p class="hint">Each burn's thrust comes from the push it gave: the platform's mass times its acceleration, with the
 229            table's slope (its calibration) taken out. So the platform's mass, inertia and centre of gravity must be right:
 230            the console asks before it starts.</p></section>`;
 231      const viewing = running ? run.platform : S.platform;
 232      const res = (S.cal && S.cal.thrusters || {})[viewing];
 233      return `<div class="cal-col">
 234          ${S.local ? syncCard() : ''}
 235          ${howCard}
 236          ${S.local ? labOnlyCard('thrusters') : thrustRunCard(run, running)}
 237          ${run ? progressCard(run) : ''}
 238        </div>
 239        <div class="cal-col cal-wide">
 240          ${running ? `<section class="card cal-map-card"><div class="card-head"><h2>Thrusters</h2><span class="pill live">Measuring · ${platformName(run.platform)}</span></div>
 241              <div class="cal-map"><canvas id="calMap" aria-label="The table seen from above: the platform and its burns"></canvas></div>
 242              <p class="hint">The platform, its burns so far (yellow), and where it flies back to.</p></section>`
 243            : thrustResults(viewing, res)}
 244        </div>`;
 245    }
 246  
 247    function thrustRunCard(run, running) {
 248      const f = running ? run.params : S.thrForm, st = getStream(), streaming = st && st.running;
 249      const chosen = running ? run.platform : S.platform;
 250      const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
 251          <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
 252      const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
 253          <input type="number" class="input" data-thrform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
 254      const minutes = Math.round(8 * f.repeats * (Number(f.burn) + 12) / 60);
 255      const table = S.cal && S.cal.table;
 256      return `<section class="card">
 257          <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : busyOther() ? '<span class="pill warn">Another Calibration Runs</span>'
 258            : streaming ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Start PhaseSpace First</span>'}</div>
 259          <div class="segmented cal-plat" role="group" aria-label="Platform">${plat}</div>
 260          <div class="cal-form">
 261            ${field('repeats', 'Repeats', 1, 3, 1, '', 'How many burns of each thruster (their thrusts are averaged)')}
 262            ${field('burn', 'Burn Time', 0.5, 3, 0.5, 's', 'How long each thruster fires (longer: more motion to measure, but more room needed)')}
 263            ${field('margin', 'Edge Margin', 0.25, 0.8, 0.05, 'm', 'A burn ends early when the platform comes this close to an edge')}
 264          </div>
 265          <p class="hint">${8 * f.repeats} burns of ${f.burn} s, each with a flight back: about ${minutes} min.
 266            ${table ? `The table's slope (calibrated ${esc(String(table.updated).slice(0, 10))}) is taken out.` : '<span class="warn-text">No table calibration: the table\'s slope is not taken out (calibrate the table first for the best thrusts).</span>'}</p>
 267          <div class="cal-actions">
 268            ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
 269                      : `<button type="button" class="btn btn-go" data-cal="start-thrusters" ${streaming && !busyOther() ? '' : 'disabled'}>Start Thruster Calibration…</button>`}
 270          </div>
 271        </section>`;
 272    }
 273  
 274    function busyOther() { return S.active && S.run && S.run.kind !== S.section; }
 275  
 276    function thrustResults(name, res) {
 277      const picker = NAMES.map(n => `<button type="button" data-calview="${n}" class="${n === name ? 'active' : ''}">
 278          <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
 279      const head = `<div class="card-head"><h2>Thrusters</h2><div class="segmented cal-plat" role="group" aria-label="Platform">${picker}</div></div>`;
 280      if (!res) {
 281        return `<section class="card">${head}<p class="hint">No thruster calibration for the ${platformName(name)} yet.${S.local ? ' The lab console measures it; Update from Lab brings it here.' : ''}
 282          The result shows each thruster's thrust against nominal, which way it really pushes, and each burn.</p>
 283          ${thrusterDiagram({})}</section>`;
 284      }
 285      const nominal = res.nominal || 0.2825, f = res.thrust;
 286      const row = k => `<tr><td>T${k + 1}${res.valid[k] ? '' : ' <span class="warn-text" title="No good burn: the nominal thrust is used">·</span>'}</td>
 287          <td>${fmt(f[k], 4)}</td><td>${res.thrust_sd[k] != null ? fmt(res.thrust_sd[k], 4) : '—'}</td>
 288          <td style="color:${f[k] / nominal < 0.97 || f[k] / nominal > 1.03 ? 'var(--warn)' : 'inherit'}">${res.valid[k] ? `${f[k] >= nominal ? '+' : ''}${fmt(100 * (f[k] / nominal - 1), 1)} %` : '—'}</td>
 289          <td>${res.angle[k] != null ? `${fmt(res.angle[k], 1)}°` : '—'}</td>
 290          <td style="color:${res.torque_ratio[k] != null && Math.abs(res.torque_ratio[k] - 1) > 0.1 ? 'var(--warn)' : 'inherit'}">${res.torque_ratio[k] != null ? fmt(res.torque_ratio[k], 2) : '—'}</td></tr>`;
 291      const bad = res.valid.filter(v => !v).length, a = res.params && res.params.assumed;
 292      return `<section class="card">${head}
 293          <div class="cal-thr-grid">
 294            ${thrusterDiagram({ thrust: f, nominal, angle: res.angle })}
 295            <div>${thrustBars({ thrust: f, sd: res.thrust_sd, nominal, valid: res.valid })}</div>
 296          </div>
 297          <table class="cal-thr-table"><thead><tr><th>Thruster</th><th>Thrust [N]</th><th>± [N]</th><th>vs Nominal</th>
 298            <th title="How far the push is turned from the thruster's direction (towards the body's +z rotation)">Turned</th>
 299            <th title="The torque it gave over the torque the mixer expects from it (moment arm × thrust): far from 1, the CG or the inertia is off">Torque Ratio</th></tr></thead>
 300            <tbody>${f.map((_, k) => row(k)).join('')}</tbody></table>
 301          ${bad ? `<p class="hint warn-text">${bad} thruster${bad > 1 ? 's' : ''} had no good burn and keep${bad > 1 ? '' : 's'} the nominal thrust.</p>` : ''}
 302          ${burnPaths(res.burns || [])}
 303          <p class="hint">Measured ${when(res.updated)} by ${esc(res.by || '?')}, assuming ${a ? `${fmt(a.mass, 2)} kg, ${fmt(a.inertia, 4)} kg m², CG (${fmt(a.cg_mm[0], 1)}, ${fmt(a.cg_mm[1], 1)}) mm${a.changed ? ' (changed from the defaults)' : ''}` : '—'};
 304            ${res.table_used ? 'the table\'s slope taken out' : 'no table calibration then'}. Simulations and the lab console's controller use these thrusts (Scenario: Calibrated Thrust).</p>
 305          ${S.local ? '' : `<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-thrusters" title="Remove the ${platformName(name)}'s thruster calibration from the lab console's file">Remove</button></div>`}
 306        </section>`;
 307    }
 308  
 309    // The assumptions dialog: the platform's mass properties, which the person starting it confirms or changes
 310    function askThrusters() {
 311      const n = S.platform, a = (S.assumed || {})[n];
 312      if (!a) { toast('The lab console didn\'t say what it takes the platform\'s mass properties to be; reload the page.', 'error'); return; }
 313      const dlg = $('#calThrustDialog');
 314      $('#calThrustName').textContent = platformName(n);
 315      const set = (k, v) => { dlg.querySelector(`[data-assume="${k}"]`).value = v; };
 316      set('mass', a.mass); set('inertia', Number(a.inertia).toFixed(5)); set('cgx', a.cg_mm[0]); set('cgy', a.cg_mm[1]);
 317      const t = S.cal && S.cal.table;
 318      $('#calThrustTable').innerHTML = t ? `The table's slope, calibrated ${esc(String(t.updated).slice(0, 10))}, is taken out of each burn.`
 319        : '<span class="warn-text">There is no table calibration, so the table\'s slope is not taken out: calibrate the table first for the best thrusts.</span>';
 320      $('#calThrustAck').checked = false;
 321      $('#calThrustGo').disabled = true;
 322      dlg.showModal();
 323    }
 324    $('#calThrustAck').onchange = ev => { $('#calThrustGo').disabled = !ev.target.checked; };
 325    $('#calThrustGo').onclick = () => act(async () => {
 326      const dlg = $('#calThrustDialog'), v = k => Number(dlg.querySelector(`[data-assume="${k}"]`).value);
 327      readThrForm();
 328      dlg.close();
 329      onRun(await api('/api/calibration/thrusters/start', {
 330        platform: S.platform, acknowledged: true, ...S.thrForm,
 331        assumed: { mass: v('mass'), inertia: v('inertia'), cg_mm: [v('cgx'), v('cgy')] } }));
 332    });
 333  
 334    function readThrForm() {
 335      for (const el of body.querySelectorAll('[data-thrform]')) S.thrForm[el.dataset.thrform] = Number(el.value);
 336      storage.set('spot.calibration.thrusters', S.thrForm);
 337    }
 338  
 339    // ---------------------------------------------------------------- the arm
 340  
 341    function armSection() {
 342      const run = S.run && S.run.kind === 'arm' ? S.run : null;
 343      const running = run && ['running', 'stopping'].includes(run.state);
 344      const now = running && run.step != null ? run.steps[run.step] : null;
 345      if (now && now.test) S.armDemo = { test: now.test, joint: now.joint };
 346      const stepper = ARM_TESTS.map(([key, title, text], k) => {
 347        const cls = S.armDemo.test === key ? 'now' : '';
 348        return `<li class="cal-step ${cls}" data-armdemo="${key}" style="cursor:pointer" title="Show this test on the arm">
 349          <svg viewBox="0 0 24 24" aria-hidden="true">${['<path d="M4 18h16M12 18V8M8 11l4-4 4 4"/>', '<path d="M4 12c3-6 5-6 8 0s5 6 8 0"/>', '<path d="M5 18 19 6M13 6h6v6"/>', '<path d="M4 16h4v-6h4v8h4v-4h4"/>'][k]}</svg>
 350          <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`;
 351      }).join('');
 352      const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2>${now && now.test ? `<span class="pill live">${JOINTS[now.joint]} · ${now.test}</span>` : ''}</div>
 353          <div id="calArmFig">${armFigure(S.armDemo.test, S.armDemo.joint, 0)}</div>
 354          <ol class="cal-steps">${stepper}</ol>
 355          <p class="hint">For each joint in turn, the other joints hold their position. The script (resources/arm_calibration/calibrate_arm.py)
 356            runs on the Chaser, which has the arm's U2D2; it fits each joint's friction and inertia, which SPOT's arm model then uses
 357            (spot.arm.parameters). The Chaser rests on the table throughout.</p></section>`;
 358      const res = armResult(run);
 359      return `<div class="cal-col">
 360          ${S.local ? syncCard() : ''}
 361          ${howCard}
 362          ${S.local ? labOnlyCard('arm') : armChecksCard() + armRunCard(run, running)}
 363          ${run ? armProgressCard(run) : ''}
 364        </div>
 365        <div class="cal-col cal-wide">${armResults(res)}</div>`;
 366    }
 367  
 368    function armChecksCard() {
 369      const c = S.armCheck;
 370      let body;
 371      if (S.armChecking) body = '<p class="hint">Asking the Chaser…</p>';
 372      else if (!c) body = '<p class="hint">Not checked yet.</p>';
 373      else if (!c.reachable) body = `<p class="hint bad">The Chaser doesn't answer: ${esc(c.error)}</p>`;
 374      else {
 375        body = `<ul class="cal-checks">${c.items.map(i => {
 376          const mark = !i.ok ? '<span class="n">✕</span>' : i.warn ? '<span class="w">!</span>' : '<span class="y">✓</span>';
 377          return `<li>${mark}<b>${esc(i.what)}</b><span class="mono">${esc(i.value)}</span>${!i.ok || i.warn ? `<span class="fix">${esc(i.fix)}</span>` : ''}</li>`;
 378        }).join('')}</ul>${c.running ? '<p class="hint warn-text">A calibration is already running on the Chaser.</p>' : ''}`;
 379      }
 380      const pill = !c ? '' : c.ok ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Not Ready</span>';
 381      return `<section class="card"><div class="card-head"><h2>The Chaser's Setup</h2>${pill}</div>${body}
 382          <div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="arm-check" ${S.armChecking ? 'disabled' : ''}>Check Again</button></div>
 383          <p class="hint">What calibrate_arm.py needs on the Chaser. A rehearsal (simulated servos) needs none of it.</p></section>`;
 384    }
 385  
 386    function armRunCard(run, running) {
 387      const f = running ? run.params : S.armForm;
 388      const chip = (kind, value, label, on) => `<button type="button" class="cal-chip${on ? ' on' : ''}" data-arm${kind}="${value}" ${running ? 'disabled' : ''}>${label}</button>`;
 389      const minutes = Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
 390      const ready = S.armCheck && S.armCheck.ok;
 391      return `<section class="card">
 392          <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : busyOther() ? '<span class="pill warn">Another Calibration Runs</span>' : ''}</div>
 393          <span class="field-label">Joints</span><div class="cal-chips">${[1, 2, 3].map(j => chip('joint', j, JOINTS[j], f.joints.includes(j))).join('')}</div>
 394          <span class="field-label">Tests</span><div class="cal-chips">${ARM_TESTS.map(([k, t]) => chip('test', k, t, f.tests.includes(k))).join('')}</div>
 395          <label class="toggle" title="Run the script with its simulated servos instead: it needs no hardware, moves nothing, and its result is shown but never becomes the lab's"><input type="checkbox" data-armsim ${f.simulate ? 'checked' : ''} ${running ? 'disabled' : ''}><span></span>Rehearse (Simulated Servos)</label>
 396          <p class="hint">${f.simulate ? 'A rehearsal takes under a minute.' : `About ${minutes} min with the real servos.`}</p>
 397          <div class="cal-actions">
 398            ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
 399                      : `<button type="button" class="btn btn-go" data-cal="start-arm" ${!f.joints.length || !f.tests.length || busyOther() || (!f.simulate && !ready) ? 'disabled' : ''}
 400                          title="${!f.simulate && !ready ? 'The Chaser isn\'t ready (see its setup)' : ''}">${f.simulate ? 'Start Rehearsal' : 'Start Arm Calibration…'}</button>`}
 401          </div></section>`;
 402    }
 403  
 404    function armProgressCard(run) {
 405      const tests = run.steps.filter(s => s.test), done = tests.filter(s => s.state === 'done').length;
 406      const pct = tests.length ? Math.round(100 * done / tests.length) : 0;
 407      return progressCard(run).replace('<ol class="cal-progress">', `<div class="cal-bar-progress"><i style="width:${pct}%"></i></div><ol class="cal-progress">`)
 408        .replace(/<pre class="cal-log">[\s\S]*?<\/pre>/, `<pre class="cal-log">${esc((run.armlog || []).filter(l => !l.startsWith('PROGRESS ')).join('\n') || run.log.join('\n'))}</pre>`);
 409    }
 410  
 411    // What to show: a rehearsal just run (it isn't saved), else the lab's arm calibration
 412    function armResult(run) {
 413      if (run && run.result && run.result.format === 'spot-arm-calibration' && run.result.simulated) return run.result;
 414      return S.cal && S.cal.arm;
 415    }
 416  
 417    function armResults(res) {
 418      if (!res) {
 419        return `<section class="card"><div class="card-head"><h2>Arm</h2><span class="pill">Not Calibrated</span></div>
 420          <p class="hint">No arm calibration yet.${S.local ? ' The lab console measures it; Update from Lab brings it here.' : ''}
 421            The result shows each joint's friction (its breakaway torque, and the friction against speed with the fitted curve), its
 422            inertia, and the position steps against the goals. Until then SPOT's arm model uses placeholder friction.</p></section>`;
 423      }
 424      const joints = [].concat(res.joints || []), plots = res.plots || {};
 425      const row = j => `<tr><td>${esc(JOINTS[j.index] || j.name)}</td><td>${fmt(j.static, 3)}</td>
 426          <td>${j.static_pos != null ? `+${fmt(j.static_pos, 3)} / −${fmt(j.static_neg, 3)}` : '—'}</td>
 427          <td>${fmt(j.coulomb, 3)}</td><td>${fmt(j.stribeck, 3)}</td><td>${fmt(j.viscous, 3)}</td>
 428          <td>${j.inertia != null ? j.inertia.toExponential(2) : '—'}</td></tr>`;
 429      const sj = joints.some(j => j.index === S.armStepsJoint) ? S.armStepsJoint : (joints[0] || {}).index;
 430      const tabs = joints.map(j => `<button type="button" class="cal-chip${j.index === sj ? ' on' : ''}" data-armsteps="${j.index}">${JOINTS[j.index]}</button>`).join('');
 431      const label = res.simulated ? '<span class="pill warn">Rehearsal · Not Saved</span>' : `<span class="pill ok">${when(res.updated)}</span>`;
 432      return `<section class="card"><div class="card-head"><h2>Arm</h2>${label}</div>
 433          <table class="cal-thr-table"><thead><tr><th>Joint</th><th title="Breakaway torque">Static [N m]</th><th>+ / − [N m]</th>
 434            <th title="Sliding friction">Coulomb [N m]</th><th title="How quickly friction falls from static to Coulomb with speed">Stribeck [rad/s]</th>
 435            <th>Viscous [N m s/rad]</th><th title="About the joint, link and rotor">Inertia [kg m²]</th></tr></thead>
 436            <tbody>${joints.map(row).join('')}</tbody></table>
 437          <div class="cal-arm-plots">
 438            <div><h3 class="cal-sub">Friction against Speed</h3>${frictionPlot(joints, plots)}</div>
 439            <div><h3 class="cal-sub">Position Steps</h3><div class="cal-tabs-mini">${tabs}</div>${stepsPlot((plots[String(sj)] || {}).steps)}</div>
 440          </div>
 441          <p class="hint">${res.simulated ? 'A rehearsal with the script\'s simulated servos: the lab\'s arm calibration is unchanged.'
 442            : `Measured ${when(res.updated)} by ${esc(res.by || '?')} (servos: ${esc(Object.values(res.servos || {}).join(', ') || '?')}, supply ${fmt(res.supply_voltage, 1)} V).
 443               Simulations use it with the Scenario's Calibrated Arm; the raw data stays on the lab console (${esc(res.lab_folder || '')}).`}</p>
 444          ${S.local || res.simulated ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-arm" title="Remove the arm calibration from the lab console\'s file">Remove</button></div>'}
 445        </section>`;
 446    }
 447  
 448    // The arm figure, animated with the test shown (the stepper's, or the run's)
 449    let armRaf = 0, armT0 = 0;
 450    function animateArm() {
 451      cancelAnimationFrame(armRaf);
 452      if (!S.shown || S.section !== 'arm') return;
 453      armT0 = performance.now();
 454      const frame = now => {
 455        const el = $('#calArmFig');
 456        if (!el || !S.shown || S.section !== 'arm') return;
 457        el.innerHTML = armFigure(S.armDemo.test, S.armDemo.joint, (now - armT0) / 1000);
 458        armRaf = requestAnimationFrame(frame);
 459      };
 460      armRaf = requestAnimationFrame(frame);
 461    }
 462  
 463    async function checkArm() {
 464      S.armChecking = true;
 465      if (S.shown && S.section === 'arm') render();
 466      try { S.armCheck = await api('/api/calibration/arm/check'); }
 467      finally { S.armChecking = false; if (S.shown && S.section === 'arm') render(); }
 468    }
 469  
 470    function startArm() {
 471      const f = S.armForm;
 472      if (f.simulate) return act(async () => onRun(await api('/api/calibration/arm/start', { ...f })));
 473      const minutes = Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
 474      $('#calArmTime').textContent = `${f.joints.map(j => JOINTS[j]).join(', ')}: ${f.tests.join(', ')}; about ${minutes} min.`;
 475      $('#calArmAck').checked = false;
 476      $('#calArmGo').disabled = true;
 477      $('#calArmDialog').showModal();
 478    }
 479    $('#calArmAck').onchange = ev => { $('#calArmGo').disabled = !ev.target.checked; };
 480    $('#calArmGo').onclick = () => act(async () => {
 481      $('#calArmDialog').close();
 482      onRun(await api('/api/calibration/arm/start', { ...S.armForm, acknowledged: true }));
 483    });
 484  
 485    // The map (the table seen from above), redrawn when anything on it changes
 486    function draw() {
 487      const canvas = $('#calMap');
 488      if (!canvas) return;
 489      if (S.section === 'thrusters') {   // a thruster run under way: the platform, its burns, where it flies back to
 490        const r = S.run;
 491        if (!r) return;
 492        fitMap(canvas.parentElement);
 493        drawTable(canvas, { cal: S.cal && S.cal.table, paths: r.records || [], live: { pose: r.pose, path: r.path, goal: r.goal },
 494                            color: COLORS[r.platform] });
 495        return;
 496      }
 497      const t = S.cal && S.cal.table;
 498      const run = S.run && S.run.kind === 'table' ? S.run : null;
 499      const running = run && ['running', 'stopping'].includes(run.state);
 500      let paths = t ? (t.paths || []) : [];
 501      let live = null, points = null, current = null;
 502      if (running) {   // the run under way: its drifts so far, the grid, the platform
 503        paths = run.records || [];
 504        live = { pose: run.pose, path: run.path, goal: run.goal };
 505        points = run.points;
 506        const name = run.step != null ? run.steps[run.step].name : '';
 507        current = name.startsWith('Point ') ? Number(name.slice(6)) - 1 : null;
 508      }
 509      fitMap(canvas.parentElement);
 510      drawTable(canvas, { cal: running ? null : t, paths, live, points, current, color: COLORS[run ? run.platform : S.platform] });
 511    }
 512  
 513    // The map as large as the window allows with its whole card in view: its height is what is left below its
 514    // top, less what the card has under it (the colour bar, the numbers); its width follows the table's
 515    // proportions. (A narrow window lets the page scroll: full width.)
 516    function fitMap(map) {
 517      const scroll = $('.cal-scroll'), card = map.closest('.cal-map-card');
 518      map.style.width = '';
 519      if (!scroll || !card || window.innerWidth <= 900) return;
 520      const m = map.getBoundingClientRect();
 521      const below = card.getBoundingClientRect().bottom - m.bottom;
 522      const top = m.top - scroll.getBoundingClientRect().top + scroll.scrollTop;
 523      const height = Math.max(240, scroll.clientHeight - top - below - 24);
 524      if (height < m.height) map.style.width = `${Math.floor(height * 3.9 / 2.9)}px`;
 525    }
 526    new ResizeObserver(() => draw()).observe(body);
 527    window.addEventListener('resize', () => draw());
 528  
 529    // ---------------------------------------------------------------- the sync prompt (local console)
 530  
 531    function maybeAsk() {
 532      if (!S.local || !S.lab || !S.lab.reachable || !S.labCal || !S.labCal.updated || !S.cal) return;
 533      const mine = S.cal, lab = S.labCal;
 534      if (mine.updated && lab.updated <= mine.updated) return;
 535      if (mine.sync && (mine.sync.ask === false || mine.sync.declined === lab.updated)) return;
 536      if (S.asked === lab.updated) return;
 537      S.asked = lab.updated;
 538      const dialog = $('#calSyncDialog');
 539      $('#calSyncList').innerHTML = newerSections(lab, mine).map(([what, t]) => `<li><b>${what}</b><span>${when(t)}</span></li>`).join('');
 540      $('#calSyncNever').checked = false;
 541      dialog.showModal();
 542    }
 543  
 544    function newerSections(lab, mine) {
 545      const out = [], newer = (a, b) => a && (!b || (a.updated || '') > (b.updated || ''));
 546      if (newer(lab.table, mine.table)) out.push(['Table Slope', lab.table.updated]);
 547      for (const n of NAMES) {
 548        const a = (lab.thrusters || {})[n], b = (mine.thrusters || {})[n];
 549        if (newer(a, b)) out.push([`${platformName(n)} Thrusters`, a.updated]);
 550      }
 551      if (newer(lab.arm, mine.arm)) out.push(['Arm', lab.arm.updated]);
 552      return out;
 553    }
 554  
 555    $('#calSyncUpdate').onclick = () => act(async () => {
 556      const never = $('#calSyncNever').checked;
 557      $('#calSyncDialog').close();
 558      onCalibration(await api('/api/calibration/take', {}));
 559      if (never) onCalibration(await api('/api/calibration/sync', { ask: false }));
 560      toast('This project now has the lab\'s calibration; the next simulation uses it.', 'success', 5000);
 561    });
 562    $('#calSyncLater').onclick = () => act(async () => {
 563      const never = $('#calSyncNever').checked, lab = S.labCal;
 564      $('#calSyncDialog').close();
 565      onCalibration(await api('/api/calibration/sync', never ? { ask: false } : { declined: lab.updated }));
 566      toast(never ? 'The lab\'s calibration won\'t be offered again (Calibration tab: Ask When the Lab\'s Is Newer turns it back on).'
 567                  : 'Kept this project\'s calibration. The Calibration tab can update it later.', 'info', 6000);
 568    });
 569  
 570    // ---------------------------------------------------------------- input
 571  
 572    document.addEventListener('click', ev => {
 573      const b = ev.target.closest('[data-calsec]');
 574      if (b) {
 575        S.section = b.dataset.calsec;
 576        storage.set('spot.calibration.section', S.section);
 577        render();
 578        if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) act(checkArm);
 579      }
 580    });
 581    body.addEventListener('click', ev => act(async () => {
 582      const b = ev.target.closest('button');
 583      if (!b || b.disabled) return;
 584      if (b.dataset.calplat) {
 585        S.platform = b.dataset.calplat;
 586        storage.set('spot.calibration.platform', S.platform);
 587        render();
 588      } else if (b.dataset.calview) {
 589        S.platform = b.dataset.calview;
 590        storage.set('spot.calibration.platform', S.platform);
 591        render();
 592      } else if (b.dataset.cal === 'start-thrusters') {
 593        readThrForm();
 594        askThrusters();
 595      } else if (b.dataset.cal === 'clear-thrusters') {
 596        if (!confirm(`Remove the ${platformName(S.platform)}'s thruster calibration from the lab console? Projects keep their own copies.`)) return;
 597        onCalibration(await api('/api/calibration/clear', { section: 'thrusters', platform: S.platform }));
 598      } else if (b.dataset.armjoint || b.dataset.armtest) {
 599        const f = S.armForm, key = b.dataset.armjoint ? 'joints' : 'tests';
 600        const v = b.dataset.armjoint ? Number(b.dataset.armjoint) : b.dataset.armtest;
 601        f[key] = f[key].includes(v) ? f[key].filter(x => x !== v) : [...f[key], v];
 602        if (key === 'joints') f.joints.sort(); else f.tests = ARM_TESTS.map(t => t[0]).filter(t => f.tests.includes(t));
 603        storage.set('spot.calibration.arm', f);
 604        render();
 605      } else if (b.dataset.armsteps) {
 606        S.armStepsJoint = Number(b.dataset.armsteps);
 607        render();
 608      } else if (b.dataset.cal === 'arm-check') {
 609        await checkArm();
 610      } else if (b.dataset.cal === 'start-arm') {
 611        startArm();
 612      } else if (b.dataset.cal === 'clear-arm') {
 613        if (!confirm('Remove the arm calibration from the lab console? Projects keep their own copies.')) return;
 614        onCalibration(await api('/api/calibration/clear', { section: 'arm' }));
 615      } else if (b.dataset.cal === 'start-table') {
 616        readForm();
 617        const r = await api('/api/calibration/table/start', { platform: S.platform, ...S.form });
 618        onRun(r);
 619      } else if (b.dataset.cal === 'stop') {
 620        onRun(await api('/api/calibration/stop', {}));
 621      } else if (b.dataset.cal === 'clear-table') {
 622        if (!confirm('Remove the table calibration from the lab console? Projects keep their own copies.')) return;
 623        onCalibration(await api('/api/calibration/clear', { section: 'table' }));
 624      }
 625    }));
 626    body.addEventListener('change', ev => {
 627      if (ev.target.dataset.calform) readForm();
 628      if (ev.target.dataset.thrform) readThrForm();
 629      if (ev.target.matches('[data-armsim]')) {
 630        S.armForm.simulate = ev.target.checked;
 631        storage.set('spot.calibration.arm', S.armForm);
 632        render();
 633      }
 634    });
 635    body.addEventListener('click', ev => {   // the stepper: show that test on the arm
 636      const li = ev.target.closest('[data-armdemo]');
 637      if (li && !(S.run && S.run.kind === 'arm' && S.active)) { S.armDemo = { test: li.dataset.armdemo, joint: S.armDemo.joint }; render(); }
 638    });
 639    head.addEventListener('click', ev => act(async () => {
 640      const b = ev.target.closest('button');
 641      if (b && b.dataset.cal === 'take' && !b.disabled) {
 642        onCalibration(await api('/api/calibration/take', {}));
 643        toast('This project now has the lab\'s calibration; the next simulation uses it.', 'success', 5000);
 644      }
 645    }));
 646    head.addEventListener('change', ev => act(async () => {
 647      if (ev.target.dataset.cal === 'ask') onCalibration(await api('/api/calibration/sync', { ask: ev.target.checked }));
 648    }));
 649    document.addEventListener('keydown', ev => {
 650      if (ev.key === 'Escape' && S.shown && S.active && !S.local) act(async () => onRun(await api('/api/calibration/stop', {})));
 651    });
 652  
 653    function readForm() {
 654      for (const el of body.querySelectorAll('[data-calform]')) {
 655        S.form[el.dataset.calform] = el.tagName === 'SELECT' ? el.value : Number(el.value);
 656      }
 657      storage.set('spot.calibration.table', S.form);
 658    }
 659  
 660    // ---------------------------------------------------------------- from the server
 661  
 662    function onCalibration(d) {
 663      if (!d || !d.calibration) return;
 664      S.cal = d.calibration;
 665      S.path = d.path;
 666      viewer?.setTableSlope(S.cal.table || null);
 667      document.dispatchEvent(new CustomEvent('spot-calibration', { detail: S.cal }));
 668      if (S.shown) render();
 669      maybeAsk();
 670    }
 671  
 672    let lastRender = 0;
 673    function onRun(d) {
 674      if (!d || !('active' in d)) return;
 675      const before = S.run;
 676      S.run = d.run;
 677      S.active = d.active;
 678      // the page re-renders when the run's steps change, and redraws the map for the platform's moves
 679      const restructure = !before || !S.run || before.state !== S.run.state || before.step !== S.run.step ||
 680        before.message !== S.run.message || (before.records || []).length !== (S.run.records || []).length;
 681      if (!S.shown) return;
 682      const now = performance.now();
 683      if (restructure || now - lastRender > 1000) { lastRender = now; render(); } else draw();
 684    }
 685  
 686    return {
 687      onState(st) {
 688        S.local = st.features && st.features.mode === 'local';
 689        if (S.local) {
 690          S.lab = st.lab || null;
 691          const snap = S.lab && S.lab.snapshot && S.lab.snapshot.calibration;
 692          S.labCal = snap ? snap.calibration : null;
 693        } else {
 694          S.assumed = st.calibration_assumed || null;
 695          onRun(st.calibration_run);
 696        }
 697        onCalibration(st.calibration);
 698      },
 699      onCalibration,
 700      onRun,
 701      onLab(l) {
 702        S.lab = l;
 703        if (l && !l.reachable) S.labCal = null;
 704        else if (l && l.snapshot && l.snapshot.calibration) S.labCal = l.snapshot.calibration.calibration;
 705        if (S.shown) render();
 706        maybeAsk();
 707      },
 708      onLabCalibration(d) { S.labCal = d && d.calibration; if (S.shown) render(); maybeAsk(); },
 709      onStream() { if (S.shown && !S.local) render(); },
 710      show() {
 711        S.shown = true;
 712        render();
 713        if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) act(checkArm);
 714      },
 715      hide() { S.shown = false; },
 716      hasTable: () => Boolean(S.cal && S.cal.table),
 717    };
 718  }
```

</details>

<details>
<summary><b>+spot/+console/webapp/static/calviz.js</b> (434 lines; click to show)</summary>

```javascript
   1  // The Calibration tab's pictures (calibration.js), and the table slope overlay on the 3D view
   2  // (viewer.js): the jet colour map, the table's height surface (spot_console/calfit.py), and a
   3  // top-down drawing of the table with the height map, its contour lines, downhill arrows, the drift
   4  // paths and the platform.
   5  
   6  export const TABLE = { x: 3.5, y: 2.4 };
   7  export const G = 9.80665;
   8  const DEFAULT_CENTRE = [1.75, 1.2];
   9  
  10  // The jet colour map: t in [0, 1] -> [r, g, b] (0-255), dark blue through cyan, yellow to dark red
  11  export function jet(t) {
  12    t = Math.min(1, Math.max(0, t));
  13    const c = x => Math.round(255 * Math.min(1, Math.max(0, x)));
  14    return [c(1.5 - Math.abs(4 * t - 3)), c(1.5 - Math.abs(4 * t - 2)), c(1.5 - Math.abs(4 * t - 1))];
  15  }
  16  export const jetCss = t => `rgb(${jet(t).join(',')})`;
  17  
  18  // Height [m] of the table top at (x, y) for height coefficients a = [a1 .. a5] (calfit.height)
  19  export function height(a, x, y, centre = DEFAULT_CENTRE) {
  20    const u = x - centre[0], v = y - centre[1];
  21    return a[0] * u + a[1] * v + a[2] * u * u + a[3] * u * v + a[4] * v * v;
  22  }
  23  
  24  // The table's slope there: [dh/dx, dh/dy] (the floating platform accelerates at -g times it)
  25  export function gradient(a, x, y, centre = DEFAULT_CENTRE) {
  26    const u = x - centre[0], v = y - centre[1];
  27    return [a[0] + 2 * a[2] * u + a[3] * v, a[1] + a[3] * u + 2 * a[4] * v];
  28  }
  29  
  30  // Heights on an NX x NY grid over the table [um], with the lowest at 0, and the range
  31  export function heightGrid(cal, nx = 140, ny = 96) {
  32    const a = cal.height, centre = cal.centre || DEFAULT_CENTRE;
  33    const z = new Float64Array(nx * ny);
  34    let lo = Infinity, hi = -Infinity;
  35    for (let j = 0; j < ny; j++) {
  36      for (let i = 0; i < nx; i++) {
  37        const h = 1e6 * height(a, (i + 0.5) * TABLE.x / nx, (j + 0.5) * TABLE.y / ny, centre);
  38        z[j * nx + i] = h;
  39        if (h < lo) lo = h;
  40        if (h > hi) hi = h;
  41      }
  42    }
  43    for (let k = 0; k < z.length; k++) z[k] -= lo;
  44    return { z, nx, ny, range: Math.max(hi - lo, 1e-9) };
  45  }
  46  
  47  // A canvas of the height map alone (jet, low = blue), for the 3D view's overlay: the canvas's x runs
  48  // along the table's x, its rows from y = 2.4 (top) to 0
  49  export function heightCanvas(cal, nx = 350, ny = 240) {
  50    const g = heightGrid(cal, nx, ny);
  51    const cv = document.createElement('canvas');
  52    cv.width = nx; cv.height = ny;
  53    const ctx = cv.getContext('2d');
  54    const img = ctx.createImageData(nx, ny);
  55    for (let j = 0; j < ny; j++) {
  56      for (let i = 0; i < nx; i++) {
  57        const [r, gg, b] = jet(g.z[j * nx + i] / g.range);
  58        const k = ((ny - 1 - j) * nx + i) * 4;
  59        img.data[k] = r; img.data[k + 1] = gg; img.data[k + 2] = b; img.data[k + 3] = 255;
  60      }
  61    }
  62    ctx.putImageData(img, 0, 0);
  63    contours(ctx, g, (i, j) => [i * nx / g.nx, ny - j * ny / g.ny], 'rgba(255,255,255,0.45)', 1);
  64    return cv;
  65  }
  66  
  67  // Contour lines of a height grid (marching squares), at 8 levels; MAP(i, j) puts grid point (i, j)
  68  // (cell centres at i + 0.5) on the canvas
  69  function contours(ctx, g, map, color, width) {
  70    const { z, nx, ny, range } = g;
  71    ctx.strokeStyle = color;
  72    ctx.lineWidth = width;
  73    ctx.beginPath();
  74    for (let l = 1; l < 8; l++) {
  75      const level = range * l / 8;
  76      for (let j = 0; j < ny - 1; j++) {
  77        for (let i = 0; i < nx - 1; i++) {
  78          const v = [z[j * nx + i], z[j * nx + i + 1], z[(j + 1) * nx + i + 1], z[(j + 1) * nx + i]];
  79          const corners = [[i, j], [i + 1, j], [i + 1, j + 1], [i, j + 1]];
  80          const pts = [];
  81          for (let e = 0; e < 4; e++) {
  82            const a = v[e], b = v[(e + 1) % 4];
  83            if ((a < level) !== (b < level)) {
  84              const t = (level - a) / (b - a);
  85              const [ia, ja] = corners[e], [ib, jb] = corners[(e + 1) % 4];
  86              pts.push(map(ia + t * (ib - ia) + 0.5, ja + t * (jb - ja) + 0.5));
  87            }
  88          }
  89          for (let p = 0; p + 1 < pts.length; p += 2) {
  90            ctx.moveTo(pts[p][0], pts[p][1]);
  91            ctx.lineTo(pts[p + 1][0], pts[p + 1][1]);
  92          }
  93        }
  94      }
  95    }
  96    ctx.stroke();
  97  }
  98  
  99  // Draw the table seen from above into CANVAS (sized to its box, for the screen's pixel ratio):
 100  //   cal       the table calibration ({height, centre, ...}) for the height map, or null
 101  //   paths     [{points: [[x, y], ...], cut}] finished drifts
 102  //   live      {pose: [x, y, thetaDeg], path: [[x, y]...], goal: [x, y, thetaDeg]} while a run goes on
 103  //   points    [[x, y], ...] a grid of start points; current: the index of the one under way
 104  //   color     the platform's colour
 105  export function drawTable(canvas, { cal = null, paths = [], live = null, points = null, current = null, color = '#aeb5bd' } = {}) {
 106    const dpr = window.devicePixelRatio || 1;
 107    const w = canvas.clientWidth, h = canvas.clientHeight;
 108    if (!w || !h) return null;
 109    if (canvas.width !== Math.round(w * dpr) || canvas.height !== Math.round(h * dpr)) {
 110      canvas.width = Math.round(w * dpr); canvas.height = Math.round(h * dpr);
 111    }
 112    const ctx = canvas.getContext('2d');
 113    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
 114    ctx.clearRect(0, 0, w, h);
 115    const pad = { l: 34, r: 26, t: 12, b: 28 };
 116    const s = Math.min((w - pad.l - pad.r) / TABLE.x, (h - pad.t - pad.b) / TABLE.y);
 117    const ox = pad.l + ((w - pad.l - pad.r) - s * TABLE.x) / 2, oy = pad.t + ((h - pad.t - pad.b) - s * TABLE.y) / 2;
 118    const X = x => ox + x * s, Y = y => oy + (TABLE.y - y) * s;
 119  
 120    // the table: the height map, or plain granite grey
 121    if (cal && cal.height) {
 122      const g = heightGrid(cal, 175, 120);
 123      const img = ctx.createImageData(g.nx, g.ny);
 124      for (let j = 0; j < g.ny; j++) {
 125        for (let i = 0; i < g.nx; i++) {
 126          const [r, gg, b] = jet(g.z[j * g.nx + i] / g.range);
 127          const k = ((g.ny - 1 - j) * g.nx + i) * 4;
 128          img.data[k] = r; img.data[k + 1] = gg; img.data[k + 2] = b; img.data[k + 3] = 255;
 129        }
 130      }
 131      const off = document.createElement('canvas');
 132      off.width = g.nx; off.height = g.ny;
 133      off.getContext('2d').putImageData(img, 0, 0);
 134      ctx.imageSmoothingEnabled = true;
 135      ctx.globalAlpha = 0.92;
 136      ctx.drawImage(off, X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
 137      ctx.globalAlpha = 1;
 138      contours(ctx, g, (i, j) => [X(i * TABLE.x / g.nx), Y(j * TABLE.y / g.ny)], 'rgba(255,255,255,0.5)', 1);
 139      // downhill arrows, their length growing with the slope
 140      const centre = cal.centre || DEFAULT_CENTRE;
 141      let max = 1e-12;
 142      const grid = [];
 143      for (let i = 0; i < 7; i++) {
 144        for (let j = 0; j < 5; j++) {
 145          const x = (i + 0.5) * TABLE.x / 7, y = (j + 0.5) * TABLE.y / 5;
 146          const gr = gradient(cal.height, x, y, centre);
 147          grid.push([x, y, gr]);
 148          max = Math.max(max, Math.hypot(gr[0], gr[1]));
 149        }
 150      }
 151      ctx.strokeStyle = 'rgba(10,12,16,0.85)'; ctx.fillStyle = 'rgba(10,12,16,0.85)'; ctx.lineWidth = 1.6;
 152      for (const [x, y, gr] of grid) {
 153        const m = Math.hypot(gr[0], gr[1]);
 154        if (m < 1e-12) continue;
 155        const len = 0.16 * s * (0.35 + 0.65 * m / max), ux = -gr[0] / m, uy = -gr[1] / m;
 156        const x0 = X(x) - ux * len / 2, y0 = Y(y) + uy * len / 2, x1 = X(x) + ux * len / 2, y1 = Y(y) - uy * len / 2;
 157        ctx.beginPath(); ctx.moveTo(x0, y0); ctx.lineTo(x1, y1); ctx.stroke();
 158        const a = Math.atan2(y1 - y0, x1 - x0);
 159        ctx.beginPath(); ctx.moveTo(x1, y1);
 160        ctx.lineTo(x1 - 7 * Math.cos(a - 0.45), y1 - 7 * Math.sin(a - 0.45));
 161        ctx.lineTo(x1 - 7 * Math.cos(a + 0.45), y1 - 7 * Math.sin(a + 0.45));
 162        ctx.closePath(); ctx.fill();
 163      }
 164    } else {
 165      ctx.fillStyle = '#2a2d33';
 166      ctx.fillRect(X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
 167    }
 168    // 0.5 m grid, outline and ticks
 169    ctx.strokeStyle = 'rgba(255,255,255,0.12)'; ctx.lineWidth = 1;
 170    ctx.beginPath();
 171    for (let x = 0.5; x < TABLE.x; x += 0.5) { ctx.moveTo(X(x), Y(0)); ctx.lineTo(X(x), Y(TABLE.y)); }
 172    for (let y = 0.5; y < TABLE.y; y += 0.5) { ctx.moveTo(X(0), Y(y)); ctx.lineTo(X(TABLE.x), Y(y)); }
 173    ctx.stroke();
 174    ctx.strokeStyle = 'rgba(255,255,255,0.5)';
 175    ctx.strokeRect(X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
 176    ctx.fillStyle = '#8a93a3'; ctx.font = '11px system-ui, sans-serif';
 177    ctx.textAlign = 'center'; ctx.textBaseline = 'top';
 178    for (let x = 0; x <= TABLE.x + 1e-6; x += 0.5) ctx.fillText(x >= TABLE.x - 1e-6 ? `${x.toFixed(1)} m` : x.toFixed(1), X(x), Y(0) + 5);
 179    ctx.textAlign = 'right'; ctx.textBaseline = 'middle';
 180    for (let y = 0; y <= TABLE.y + 1e-6; y += 0.5) ctx.fillText(y.toFixed(1), X(0) - 5, Y(y));
 181  
 182    // the drifts
 183    const line = (pts, style, width, dash = []) => {
 184      if (!pts || pts.length < 2) return;
 185      ctx.setLineDash(dash);
 186      ctx.strokeStyle = 'rgba(0,0,0,0.6)'; ctx.lineWidth = width + 2.5;
 187      ctx.beginPath(); pts.forEach(([x, y], k) => (k ? ctx.lineTo(X(x), Y(y)) : ctx.moveTo(X(x), Y(y)))); ctx.stroke();
 188      ctx.strokeStyle = style; ctx.lineWidth = width;
 189      ctx.beginPath(); pts.forEach(([x, y], k) => (k ? ctx.lineTo(X(x), Y(y)) : ctx.moveTo(X(x), Y(y)))); ctx.stroke();
 190      ctx.setLineDash([]);
 191    };
 192    for (const p of paths) {
 193      line(p.points, '#ffffff', 2);
 194      const [x, y] = p.points[0];
 195      ctx.fillStyle = '#ffffff';
 196      ctx.beginPath(); ctx.arc(X(x), Y(y), 3.5, 0, 2 * Math.PI); ctx.fill();
 197      if (p.cut) {   // where the pucks went off
 198        const [xe, ye] = p.points[p.points.length - 1];
 199        ctx.strokeStyle = '#ff6b6b'; ctx.lineWidth = 2;
 200        ctx.beginPath(); ctx.moveTo(X(xe) - 5, Y(ye) - 5); ctx.lineTo(X(xe) + 5, Y(ye) + 5);
 201        ctx.moveTo(X(xe) + 5, Y(ye) - 5); ctx.lineTo(X(xe) - 5, Y(ye) + 5); ctx.stroke();
 202      }
 203    }
 204    // the grid of start points
 205    if (points) {
 206      points.forEach(([x, y], k) => {
 207        const now = k === current;
 208        ctx.strokeStyle = now ? '#ffd43b' : 'rgba(255,255,255,0.7)'; ctx.lineWidth = now ? 2.5 : 1.5;
 209        ctx.setLineDash(now ? [] : [4, 3]);
 210        ctx.beginPath(); ctx.arc(X(x), Y(y), 9, 0, 2 * Math.PI); ctx.stroke();
 211        ctx.setLineDash([]);
 212        ctx.fillStyle = now ? '#ffd43b' : 'rgba(255,255,255,0.85)';
 213        ctx.font = '600 11px system-ui, sans-serif'; ctx.textAlign = 'center'; ctx.textBaseline = 'middle';
 214        ctx.fillText(String(k + 1), X(x), Y(y) - 18);
 215      });
 216    }
 217    // the run under way: the path so far, the goal and the platform
 218    if (live) {
 219      line(live.path, '#ffd43b', 2.5);
 220      if (live.goal) {
 221        ctx.strokeStyle = '#ffd43b'; ctx.lineWidth = 1.5; ctx.setLineDash([5, 4]);
 222        ctx.beginPath(); ctx.arc(X(live.goal[0]), Y(live.goal[1]), 0.15 * s, 0, 2 * Math.PI); ctx.stroke();
 223        if (live.pose) { ctx.beginPath(); ctx.moveTo(X(live.pose[0]), Y(live.pose[1])); ctx.lineTo(X(live.goal[0]), Y(live.goal[1])); ctx.stroke(); }
 224        ctx.setLineDash([]);
 225      }
 226      if (live.pose) platform(ctx, X(live.pose[0]), Y(live.pose[1]), live.pose[2] * Math.PI / 180, 0.3 * s, color);
 227    }
 228    return { X, Y, s };
 229  }
 230  
 231  // A platform seen from above: a square with a tick to its front (+x)
 232  function platform(ctx, cx, cy, theta, size, color) {
 233    ctx.save();
 234    ctx.translate(cx, cy);
 235    ctx.rotate(-theta);
 236    ctx.fillStyle = color + 'cc'; ctx.strokeStyle = '#0b0d11'; ctx.lineWidth = 2;
 237    ctx.fillRect(-size / 2, -size / 2, size, size);
 238    ctx.strokeRect(-size / 2, -size / 2, size, size);
 239    ctx.strokeStyle = '#0b0d11'; ctx.lineWidth = 2.5;
 240    ctx.beginPath(); ctx.moveTo(0, 0); ctx.lineTo(size / 2, 0); ctx.stroke();
 241    ctx.restore();
 242  }
 243  
 244  // A colour bar for the height map: low (blue) to high (red), labelled in um
 245  export function colorBar(range) {
 246    const stops = Array.from({ length: 11 }, (_, k) => `${jetCss(k / 10)} ${k * 10}%`).join(', ');
 247    return `<div class="cal-bar"><span>0 µm</span><i style="background:linear-gradient(90deg, ${stops})"></i><span>${range < 10 ? range.toFixed(1) : Math.round(range)} µm</span></div>`;
 248  }
 249  
 250  // ---------------------------------------------------------------- the thrusters
 251  
 252  // The 8 thrusters in the body frame (as viewer.js): the nozzle [x, y] [m] and the way each pushes the platform
 253  export const THRUSTERS = [
 254    [[0.1547, 0.069], [-1, 0]], [[0.1547, -0.069], [-1, 0]],     // T1, T2 push along -x
 255    [[0.069, -0.1547], [0, 1]], [[-0.069, -0.1547], [0, 1]],     // T3, T4 along +y
 256    [[-0.1547, -0.069], [1, 0]], [[-0.1547, 0.069], [1, 0]],     // T5, T6 along +x
 257    [[-0.069, 0.1547], [0, -1]], [[0.069, 0.1547], [0, -1]],     // T7, T8 along -y
 258  ];
 259  
 260  // The colour of a thrust against the nominal one: within 3 % neutral, weaker amber to red, stronger blue
 261  export function deviationColor(ratio) {
 262    if (ratio == null || !Number.isFinite(ratio)) return '#8a93a3';
 263    const d = ratio - 1;
 264    if (Math.abs(d) < 0.03) return '#3ccf7e';
 265    if (d < 0) return d < -0.1 ? '#ff6b6b' : '#f5a524';
 266    return d > 0.1 ? '#4c8dff' : '#74c0fc';
 267  }
 268  
 269  // The platform from above (+x up, +y left, as the Hardware tab draws it), with each thruster's measured
 270  // push drawn outside the body as an arrow onto its nozzle: length by thrust, turned by its measured angle,
 271  // coloured against nominal, its thrust beyond it. FIRING: a thruster to light up (a run under way: its
 272  // plume, the gas leaving the nozzle).
 273  export function thrusterDiagram({ thrust = null, nominal = 0.2825, angle = null, firing = null } = {}) {
 274    const S = 520;   // px per m
 275    const P = ([x, y]) => [150 - y * S, 150 - x * S];   // body (x up, y left) -> svg
 276    const parts = [`<rect x="${150 - 0.15 * S}" y="${150 - 0.15 * S}" width="${0.3 * S}" height="${0.3 * S}" rx="10" class="cal-body"/>`,
 277      `<path d="M150 170V122M143 131l7-9 7 9" class="cal-front"/><text x="150" y="114" class="cal-front-t">+x</text>`];
 278    THRUSTERS.forEach(([nozzle, push], k) => {
 279      const [nx, ny] = P(nozzle);
 280      if (thrust) {
 281        const a = (angle && Number.isFinite(angle[k]) ? angle[k] : 0) * Math.PI / 180;
 282        const px = push[0] * Math.cos(a) - push[1] * Math.sin(a), py = push[0] * Math.sin(a) + push[1] * Math.cos(a);
 283        const len = 60 * thrust[k] / 0.3, color = deviationColor(thrust[k] / nominal);
 284        const [tx, ty] = P([nozzle[0] - px * (len + 14) / S, nozzle[1] - py * (len + 14) / S]);
 285        const [hx, hy] = P([nozzle[0] - px * 14 / S, nozzle[1] - py * 14 / S]);
 286        const [lx, ly] = P([nozzle[0] - push[0] * (len + 34) / S, nozzle[1] - push[1] * (len + 34) / S]);
 287        parts.push(`<defs><marker id="calArrow${k}" viewBox="0 0 10 10" refX="7" refY="5" markerWidth="5" markerHeight="5" orient="auto"><path d="M0 0L10 5L0 10z" fill="${color}"/></marker></defs>`,
 288          `<line x1="${tx}" y1="${ty}" x2="${hx}" y2="${hy}" stroke="${color}" stroke-width="3.5" marker-end="url(#calArrow${k})"/>`,
 289          `<text x="${lx}" y="${ly + 4}" class="cal-thrust-t" fill="${color}">${thrust[k].toFixed(3)}</text>`);
 290      }
 291      if (firing === k + 1) {   // the plume: the gas leaves the nozzle, away from the push
 292        const [fx, fy] = P([nozzle[0] - push[0] * 0.09, nozzle[1] - push[1] * 0.09]);
 293        parts.push(`<line x1="${nx}" y1="${ny}" x2="${fx}" y2="${fy}" class="cal-plume"/>`);
 294      }
 295      parts.push(`<circle cx="${nx}" cy="${ny}" r="9" class="cal-nozzle${firing === k + 1 ? ' lit' : ''}"/>`,
 296        `<text x="${nx}" y="${ny + 3.5}" class="cal-nozzle-t">${k + 1}</text>`);
 297    });
 298    return `<svg viewBox="-70 -70 440 440" class="cal-thr-svg" role="img" aria-label="The platform from above, with each thruster's measured push">${parts.join('')}</svg>`;
 299  }
 300  
 301  // Bars: each thruster's thrust with its spread (error bar), and the nominal thrust (dashed)
 302  export function thrustBars({ thrust, sd = [], nominal, valid = [] }) {
 303    const W = 560, H = 230, L = 46, B = 30, T = 14, top = Math.max(nominal * 1.25, ...thrust.map((f, k) => f + (sd[k] || 0))) * 1.05;
 304    const Y = v => H - B - (H - B - T) * v / top, bw = (W - L - 10) / 8;
 305    const parts = [];
 306    for (const v of [0, 0.1, 0.2, 0.3, 0.4, 0.5].filter(v => v <= top)) {
 307      parts.push(`<line x1="${L}" x2="${W - 6}" y1="${Y(v)}" y2="${Y(v)}" class="cal-grid"/><text x="${L - 6}" y="${Y(v) + 4}" class="cal-ax-t" style="text-anchor:end">${v.toFixed(1)}</text>`);
 308    }
 309    thrust.forEach((f, k) => {
 310      const x = L + k * bw + bw * 0.18, w = bw * 0.64, color = valid[k] === false ? '#4a515e' : deviationColor(f / nominal);
 311      parts.push(`<rect x="${x}" y="${Y(f)}" width="${w}" height="${Y(0) - Y(f)}" rx="3" fill="${color}" opacity="0.85"><title>T${k + 1}: ${f.toFixed(4)} N${sd[k] ? ` ± ${sd[k].toFixed(4)}` : ''}${valid[k] === false ? ' (no good burn: nominal)' : ''}</title></rect>`);
 312      if (sd[k]) {
 313        const cx = x + w / 2;
 314        parts.push(`<path d="M${cx} ${Y(f + sd[k])}V${Y(f - sd[k])}M${cx - 6} ${Y(f + sd[k])}h12M${cx - 6} ${Y(f - sd[k])}h12" class="cal-err"/>`);
 315      }
 316      parts.push(`<text x="${x + w / 2}" y="${H - 10}" class="cal-ax-t" text-anchor="middle">T${k + 1}</text>`,
 317        `<text x="${x + w / 2}" y="${Y(f) - 6}" class="cal-bar-t" text-anchor="middle">${f.toFixed(3)}</text>`);
 318    });
 319    parts.push(`<line x1="${L}" x2="${W - 6}" y1="${Y(nominal)}" y2="${Y(nominal)}" class="cal-nominal"><title>nominal ${nominal} N</title></line>`,
 320      `<text x="12" y="${T + (H - B - T) / 2}" class="cal-ax-t" text-anchor="middle" transform="rotate(-90 12 ${T + (H - B - T) / 2})">thrust [N]</text>`);
 321    return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Each thruster's measured thrust against nominal">${parts.join('')}</svg>
 322      <p class="cal-key"><i class="dash"></i>nominal ${nominal} N <i style="background:#3ccf7e"></i>within 3 % <i style="background:#f5a524"></i>weaker
 323        <i style="background:#ff6b6b"></i>over 10 % weaker <i style="background:#74c0fc"></i>stronger <i style="background:#4c8dff"></i>over 10 % stronger</p>`;
 324  }
 325  
 326  // Small multiples: each thruster's burns, the platform's path from where it started (table frame, mm),
 327  // all at one scale
 328  export function burnPaths(burns) {
 329    const by = Array.from({ length: 8 }, () => []);
 330    for (const b of burns) if (b.points && b.points.length > 1) by[b.thruster - 1].push(b);
 331    const reach = Math.max(0.01, ...burns.flatMap(b => (b.points || []).map(([x, y]) => Math.max(Math.abs(x), Math.abs(y)))));
 332    const R = 40, s = (R - 6) / reach;
 333    return `<div class="cal-burns">${by.map((list, k) => {
 334      const lines = list.map(b => `<polyline points="${b.points.map(([x, y]) => `${(R + x * s).toFixed(1)},${(R - y * s).toFixed(1)}`).join(' ')}" class="cal-burn${b.error ? ' bad' : ''}"><title>${b.error ? esc(b.error) : `${b.thrust.toFixed(4)} N`}</title></polyline>`).join('');
 335      return `<figure><svg viewBox="0 0 ${2 * R} ${2 * R}"><path d="M${R} 4V${2 * R - 4}M4 ${R}H${2 * R - 4}" class="cal-grid"/>${lines}<circle cx="${R}" cy="${R}" r="2.5" fill="#fff"/></svg><figcaption>T${k + 1}</figcaption></figure>`;
 336    }).join('')}<p class="hint">Each burn's path from where the platform started, on the table (all at one scale: ${Math.round(reach * 1000)} mm to the edge of a box).</p></div>`;
 337  }
 338  
 339  const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
 340  
 341  // ---------------------------------------------------------------- the arm
 342  
 343  // What each test does to a joint over time (for the animation): its angle from where it started [rad]
 344  // at time t [s] of the test's loop (about 6 s), and a word for the moment
 345  const D = Math.PI / 180;
 346  const STEP_DEG = [10, 0, -10, 20, 0, -20, 5, 0];
 347  export const ARM_TEST_MOTION = {
 348    breakaway: t => {   // the current creeps up until the joint breaks away, then it is held and ramped the other way
 349      const u = t % 6, s = u < 3 ? 1 : -1, v = u % 3;
 350      return [v < 2.2 ? 0 : s * Math.min(8, (v - 2.2) * 30) * D, v < 2.2 ? 'current rising' : 'breakaway'];
 351    },
 352    sweep: t => {        // constant speeds back and forth, faster each pass
 353      const u = t % 6, pass = Math.floor(u / 1.5), f = (u % 1.5) / 1.5, dir = pass % 2 ? -1 : 1;
 354      return [dir * (-25 + 50 * f) * D, `speed ${pass + 1}`];
 355    },
 356    inertia: t => {      // a torque step from rest: it accelerates, then it is caught
 357      const u = t % 3;
 358      return [u < 1 ? 0 : Math.min(15, 15 * ((u - 1) / 0.6) ** 2) * D, u < 1 ? 'at rest' : 'torque step'];
 359    },
 360    steps: t => {        // position goals 10, 0, -10, 20, 0, -20, 5, 0 deg, as the servo follows them
 361      const k = Math.floor(t / 0.75) % STEP_DEG.length, prev = STEP_DEG[(k + STEP_DEG.length - 1) % STEP_DEG.length];
 362      const f = Math.min(1, (t % 0.75) / 0.35);
 363      return [(prev + (STEP_DEG[k] - prev) * (1 - (1 - f) ** 3)) * D, `goal ${STEP_DEG[k]}°`];
 364    },
 365  };
 366  
 367  // The arm from above on the Chaser's side, its three links, JOINT (1-3) moving as TEST does at time T
 368  export function armFigure(test, joint, t) {
 369    const L = [58, 54, 30];
 370    const base = [52, 148], q = [0, 0, 0];
 371    const [angle, word] = (ARM_TEST_MOTION[test] || (() => [0, '']))(t);
 372    q[joint - 1] = angle;
 373    const pts = [base];
 374    let th = -1.0, [x, y] = base;   // (the pose shown: up and out from the Chaser, the elbow and wrist bent outward)
 375    for (let k = 0; k < 3; k++) {
 376      th += q[k] + (k ? 0.55 : 0);
 377      x += L[k] * Math.cos(th); y += L[k] * Math.sin(th);
 378      pts.push([x, y]);
 379    }
 380    const links = pts.slice(1).map((p, k) => `<line x1="${pts[k][0]}" y1="${pts[k][1]}" x2="${p[0]}" y2="${p[1]}" class="cal-link${k === joint - 1 ? ' on' : ''}"/>`).join('');
 381    const joints = pts.slice(0, 3).map(([px, py], k) => `<circle cx="${px}" cy="${py}" r="${k === joint - 1 ? 8 : 6}" class="cal-joint${k === joint - 1 ? ' on' : ''}"/>`).join('');
 382    return `<svg viewBox="0 0 260 180" class="cal-arm-svg" role="img" aria-label="The arm, its ${['shoulder', 'elbow', 'wrist'][joint - 1]} moving as in the ${test} test">
 383      <rect x="8" y="148" width="110" height="24" rx="4" class="cal-body"/><text x="63" y="164" class="cal-ax-t">Chaser</text>
 384      ${links}${joints}<text x="252" y="18" class="cal-ax-t" style="text-anchor:end">${word}</text></svg>`;
 385  }
 386  
 387  // Friction against speed for each joint: the sweep's points (torque [N m] at each steady speed [rad/s])
 388  // and the fitted curve, coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous v
 389  export function frictionPlot(joints, plots) {
 390    const W = 520, H = 250, L = 52, B = 34, T = 12, R = 10;
 391    const colors = { 1: '#4c8dff', 2: '#f5a524', 3: '#3ccf7e' };
 392    const pts = joints.flatMap(j => ((plots[String(j.index)] || {}).sweep || []));
 393    const vmax = Math.max(0.2, ...pts.map(p => Math.abs(p[0]))) * 1.08;
 394    const tmax = Math.max(0.1, ...pts.map(p => Math.abs(p[1])), ...joints.map(j => (j.static || 0) * 1.05)) * 1.1;
 395    const X = v => L + (W - L - R) * (v + vmax) / (2 * vmax), Y = tq => T + (H - B - T) * (tmax - tq) / (2 * tmax);
 396    const parts = [`<line x1="${L}" x2="${W - R}" y1="${Y(0)}" y2="${Y(0)}" class="cal-grid"/>`, `<line x1="${X(0)}" x2="${X(0)}" y1="${T}" y2="${H - B}" class="cal-grid"/>`];
 397    for (const v of [-vmax * 0.9, -vmax / 2, vmax / 2, vmax * 0.9]) parts.push(`<text x="${X(v)}" y="${H - B + 14}" class="cal-ax-t">${(v).toFixed(2)}</text>`);
 398    for (const tq of [-tmax * 0.8, tmax * 0.8]) parts.push(`<text x="${L - 6}" y="${Y(tq) + 4}" class="cal-ax-t" style="text-anchor:end">${tq.toFixed(2)}</text>`);
 399    for (const j of joints) {
 400      const c = colors[j.index], f = v => Math.sign(v) * ((j.coulomb || 0) + ((j.static || 0) - (j.coulomb || 0)) * Math.exp(-((v / (j.stribeck || 0.05)) ** 2))) + (j.viscous || 0) * v;
 401      if (j.coulomb != null) {
 402        for (const side of [-1, 1]) {
 403          const d = Array.from({ length: 60 }, (_, k) => { const v = side * (0.002 + vmax * k / 59); return `${X(v).toFixed(1)},${Y(f(v)).toFixed(1)}`; }).join(' ');
 404          parts.push(`<polyline points="${d}" fill="none" stroke="${c}" stroke-width="2"/>`);
 405        }
 406      }
 407      for (const [v, tq] of ((plots[String(j.index)] || {}).sweep || [])) parts.push(`<circle cx="${X(v)}" cy="${Y(tq)}" r="3.2" fill="${c}" opacity="0.85"/>`);
 408    }
 409    parts.push(`<text x="${(L + W) / 2}" y="${H - 4}" class="cal-ax-t">joint speed [rad/s]</text>`,
 410      `<text x="14" y="${(T + H - B) / 2}" class="cal-ax-t" transform="rotate(-90 14 ${(T + H - B) / 2})">friction torque [N m]</text>`);
 411    const key = joints.map(j => `<i style="background:${colors[j.index]}"></i>${esc(j.name || `joint ${j.index}`)}`).join(' ');
 412    return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Friction torque against speed, measured and fitted">${parts.join('')}</svg>
 413      <p class="cal-key">${key} <span>dots: measured · lines: the fitted model</span></p>`;
 414  }
 415  
 416  // A joint's position steps: the goal (dashed) and the measured angle [deg] against time [s]
 417  export function stepsPlot(rows) {
 418    if (!rows || rows.length < 2) return '<p class="hint">No position steps in this run.</p>';
 419    const W = 520, H = 200, L = 44, B = 30, T = 10, R = 10;
 420    const t0 = rows[0][0], t1 = rows[rows.length - 1][0];
 421    const all = rows.flatMap(r => [r[1], r[2]]).map(v => v / D);
 422    const lo = Math.min(...all) - 2, hi = Math.max(...all) + 2;
 423    const X = t => L + (W - L - R) * (t - t0) / Math.max(t1 - t0, 1e-6), Y = v => T + (H - B - T) * (hi - v) / (hi - lo);
 424    const line = k => rows.map(r => `${X(r[0]).toFixed(1)},${Y(r[k] / D).toFixed(1)}`).join(' ');
 425    const parts = [];
 426    for (let v = Math.ceil(lo / 10) * 10; v <= hi; v += 10) parts.push(`<line x1="${L}" x2="${W - R}" y1="${Y(v)}" y2="${Y(v)}" class="cal-grid"/><text x="${L - 6}" y="${Y(v) + 4}" class="cal-ax-t" style="text-anchor:end">${v}</text>`);
 427    parts.push(`<polyline points="${line(1)}" fill="none" stroke="#c3c9d4" stroke-width="1.5" stroke-dasharray="5 4"/>`,
 428      `<polyline points="${line(2)}" fill="none" stroke="#ffd43b" stroke-width="2"/>`,
 429      `<text x="${(L + W) / 2}" y="${H - 4}" class="cal-ax-t">time [s] (${(t1 - t0).toFixed(0)} s)</text>`,
 430      `<text x="12" y="${(T + H - B) / 2}" class="cal-ax-t" transform="rotate(-90 12 ${(T + H - B) / 2})">angle [deg]</text>`);
 431    return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Position steps: goal and measured">${parts.join('')}</svg>
 432      <p class="cal-key"><i class="dash"></i>goal <i style="background:#ffd43b"></i>measured</p>
 433      <p class="hint">With SPOT's servo settings; spot.arm.compareSteps replays them in the model.</p>`;
 434  }
```

</details>
<!-- /listing -->
