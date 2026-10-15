// The Calibration tab (both consoles). The lab console runs the calibrations
// (spot_console/calibration.py) and keeps the lab's calibration file; the local console shows its
// project's copy (resources/spot_calibration.json, which the simulation uses) next to the lab's,
// and offers the lab's whenever it is newer (the sync dialog: Update, Not Now, or Never Ask
// Again).
//
// Sections: Table Slope, Thrusters and Arm.

import { platformName, COLORS } from './platforms.js';
import {
  drawTable, heightGrid, colorBar, deviationColor, thrustBars, burnPaths, decayPlot, decayFormula,
  frictionPlot, stepsPlot, THRUSTERS,
} from './calviz.js';
import { THRUSTERS as THRUSTER_MAP } from './hardware.js';

const $ = (s, el = document) => el.querySelector(s);
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const NAMES = ['RED', 'BLACK', 'BLUE'];
const fmt = (v, d = 2) => (v == null || !Number.isFinite(v) ? '—' : v.toFixed(d));

// A date and time ('Never' for none).
const when = iso => {
  if (!iso) {
    return 'Never';
  }
  const d = new Date(iso);
  return d.toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' }) + ' ' +
    d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
};

// The four steps of the table calibration, as the stepper shows them (and the run's steps they
// cover): [title, text, icon].
const TABLE_STEPS = [
  ['Rest', 'PhaseSpace records the platform resting on the table: the cameras\' noise.',
   '<path d="M4 17h16M7 17v-3h10v3" /><circle cx="12" cy="9" r="2.2"/>'],
  ['Float', 'The air bearings come on with the thrusters off: it drifts from where it is.',
   '<path d="M4 18h16"/><rect x="7" y="9" width="10" height="5" rx="1"/>' +
   '<path d="M8 16.5h1M11.5 16.5h1M15 16.5h1"/>'],
  ['Drift Map', 'It flies to each point of the grid, settles, rests with the pucks off, then ' +
   'floats and drifts again with the thrusters off.',
   '<circle cx="6" cy="7" r="1.6"/><circle cx="18" cy="7" r="1.6"/><circle cx="6" cy="17" ' +
   'r="1.6"/><circle cx="18" cy="17" r="1.6"/><path d="M7.5 8.2 16.5 15.8" ' +
   'stroke-dasharray="2 2"/>'],
  ['Fit', 'The table\'s height surface and the air bearings\' drag are fitted to every drift.',
   '<path d="M4 18c3-8 6-10 8-10s5 2 8 10"/><path d="M4 18h16"/>'],
];

// The thruster calibration's three steps, for each thruster in turn (1 to 8, and each repeat).
const THRUSTER_STEPS = [
  ['Centre', 'The diagram\'s controller flies it to the table\'s centre, where every burn starts.',
   '<path d="M19 12a7 7 0 1 1-2-4.9"/><path d="M17 3v4h-4"/>'],
  ['Rest', 'Pucks off for 5 s: it stops on the table.',
   '<path d="M4 17h16M7 17v-3h10v3"/>'],
  ['Fire', 'Pucks on and one thruster at full thrust at the same moment, for the burn time; ' +
   'PhaseSpace records the push.',
   '<rect x="6" y="8" width="10" height="8" rx="1"/><path d="M16 12h5M18 9.5l3 2.5-3 2.5"/>'],
];

// The second stage, thrusters together: 1, then 1-2, ... 1-8, each firing as one burn above.
const DECAY_STEPS = [
  ['Centre', 'The diagram\'s controller flies it to the table\'s centre.',
   '<path d="M19 12a7 7 0 1 1-2-4.9"/><path d="M17 3v4h-4"/>'],
  ['Rest', 'Pucks off for 5 s: it stops on the table.', '<path d="M4 17h16M7 17v-3h10v3"/>'],
  ['Fire Together', 'Pucks on and thrusters 1 to N at full thrust at once (N = 1, 2, … 8); ' +
   'PhaseSpace records the push.',
   '<rect x="6" y="8" width="10" height="8" rx="1"/>' +
   '<path d="M16 10h5M16 14h5M18 8l3 2-3 2M18 12l3 2-3 2"/>'],
  ['Fit', 'Each firing\'s push against what its thrusters give alone (the first stage); how the ' +
   'output falls with N open.',
   '<path d="M4 6c4 0 6 12 16 12"/><path d="M4 18h16"/>'],
];

// The section a run of KIND shows in.
const sectionOf = kind => (kind === 'decay' ? 'thrusters' : kind);

// The thrust decay a platform takes until its own is measured (control.py's DEFAULTS,
// spot.params.defaults: the Obstacle's, 2026-10-14); the lab console says it with each
// platform's mass properties (S.assumed).
const DEFAULT_DECAY = 0.112;

// The arm calibration's four tests (calibrate_arm.py), each on one joint while the others hold:
// [key, title, text].
const ARM_TESTS = [
  ['breakaway', 'Breakaway', 'The motor current creeps up from zero until the joint moves, ' +
   'three times each way: the static friction (breakaway torque).'],
  ['sweep', 'Speed Sweep', 'The joint turns back and forth at nine speeds; the current it needs ' +
   'at each is the friction at that speed (Coulomb, Stribeck, viscous).'],
  ['inertia', 'Inertia', 'A torque step from rest, each way: how fast it accelerates gives the ' +
   'joint\'s inertia (link and rotor).'],
  ['steps', 'Position Steps', 'Goals of 10, 0, -10, 20, 0, -20, 5, 0° with SPOT\'s servo ' +
   'settings, for spot.arm.compareSteps to check the model.'],
];
const JOINTS = { 1: 'Shoulder', 2: 'Elbow', 3: 'Wrist' };
const ARM_MINUTES = { breakaway: 1.5, sweep: 2.5, inertia: 0.5, steps: 0.5 };   // per joint, about

/**
 * Sets up the Calibration tab.
 * @param {{api: function(string, !Object=): !Promise, act: function(function(): !Promise),
 *     toast: function(string, string, number),
 *     storage: {get: function(string, *): *, set: function(string, *)}, viewer: ?Object,
 *     getStream: function(): ?Object}} deps The page's API call, its action wrapper (reports
 *     errors), its toast and settings storage, the 3D view, and PhaseSpace's streaming state.
 * @return {!Object} Handlers for the page's events: onState, onCalibration, onRun, onLab,
 *     onLabCalibration, onStream, show, hide, hasTable.
 */
export function initCalibration({ api, act, toast, storage, viewer, getStream = () => null }) {
  const S = {
    local: false, shown: false, section: storage.get('spot.calibration.section', 'table'),
    cal: null, path: null,                 // this console's calibration
    run: null, active: false,              // the lab console's run (lab console only)
    lab: null, labCal: null,               // local console: the lab console and its calibration
    platform: storage.get('spot.calibration.platform', 'RED'),
    form: storage.get('spot.calibration.table',
      { nx: 3, ny: 2, drift: 12, margin: 0.35, model: 'auto' }),
    thrForm: storage.get('spot.calibration.thrusters', { repeats: 2, burn: 2, margin: 0.35 }),
    // The Thrusters Run card: one at a time, or together; and together's settings.
    thrStage: storage.get('spot.calibration.thrStage', 'single'),
    decayForm: storage.get('spot.calibration.decay', { repeats: 1, burn: 2, margin: 0.35 }),
    armForm: storage.get('spot.calibration.arm', {
      joints: [1, 2, 3], tests: ['breakaway', 'sweep', 'inertia', 'steps'], simulate: false,
    }),
    armCheck: null, armChecking: false, armStepsJoint: 1,
    // Lab console: what it takes each platform's mass properties to be.
    assumed: null,
    asked: null,                           // the lab calibration already offered this session
    // Lab console: each kind's saved runs (null: not fetched yet), and the one chosen.
    savedRuns: { table: null, thrusters: null, decay: null },
    runPick: { table: null, thrusters: null, decay: null },
  };
  // (The sync card is in the body's left column.)
  const body = $('#calBody');
  const head = body;

  // Rendering

  // The section chosen.
  function render() {
    for (const b of document.querySelectorAll('[data-calsec]')) {
      b.classList.toggle('active', b.dataset.calsec === S.section);
    }
    if (S.section === 'table') {
      renderTable();
    } else {
      parkView();
      body.innerHTML = S.section === 'thrusters' ? thrustersSection() : armSection();
      body.dataset.layout = S.section;
    }
    draw();
  }

  // The 3D view (Table Slope)
  // The page's one 3D view (viewer.js) sits in the Table Slope card, the table coloured by its
  // slope (always: the view's own Table Slope switch is for the other pages). The card's frame is
  // built once and only its text is refreshed, so the view isn't taken out of the page (and a
  // drag on it cut short) every time the run's progress comes in.
  const viewEl = document.getElementById('viewer');

  // Back to its usual place (the PhaseSpace page), out of the way of a re-render.
  function parkView() {
    if (viewEl && body.contains(viewEl)) {
      document.querySelector('[data-slot="phasespace"]')?.prepend(viewEl);
    }
  }

  // Into the card, showing the live platforms and the slope (and, while this session's run
  // goes on, its drifts live).
  function attachView() {
    const slot = body.querySelector('[data-calview]');
    if (!viewEl || !slot || !S.shown) {
      return;
    }
    if (viewEl.parentElement !== slot) {
      slot.append(viewEl);
    }
    viewEl.dataset.showing = 'calibration';
    const run = S.run && S.run.kind === 'table' ? S.run : null;
    viewer?.setLayers({ live: true, sim: false, setup: false });
    viewer?.setTableSlopeShown(true);
    viewer?.setLivePaths(Boolean(run));
  }

  // The Table Slope section: its frame once, then only the parts that changed.
  function renderTable() {
    const parts = tableSection();
    if (body.dataset.layout !== 'table' || !body.querySelector('[data-calview]')) {
      parkView();
      body.innerHTML = `<div class="cal-col" data-part="left"></div>
        <div class="cal-col cal-wide"><section class="card cal-map-card">
          <div class="card-head" data-part="head"></div>
          <div class="cal-view" data-calview></div>
          <div data-part="below"></div>
        </section></div>`;
      body.dataset.layout = 'table';
    }
    for (const [k, html] of Object.entries(parts)) {
      const el = body.querySelector(`[data-part="${k}"]`);
      if (el.innerHTML !== html) {
        el.innerHTML = html;
      }
    }
    attachView();
  }

  // Local console: this project's calibration against the lab's, with Update from Lab.
  function syncCard() {
    const mine = S.cal || {};
    const lab = S.labCal;
    const reach = S.lab && S.lab.reachable;
    let pill = ['No Calibration Yet', ''];
    if (!reach) {
      pill = ['Lab Console Not Reachable', ''];
    } else if (!lab) {
      pill = ['Lab Console Needs Update', 'warn'];
    } else if (!lab.updated) {
      pill = ['The Lab Has No Calibration', ''];
    } else if (!mine.updated || lab.updated > mine.updated) {
      pill = ['The Lab Has a Newer One', 'warn'];
    } else {
      pill = ['Up to Date', 'ok'];
    }
    const ask = !mine.sync || mine.sync.ask !== false;
    const canTake = reach && lab && lab.updated && (!mine.updated || lab.updated !== mine.updated);
    return `<section class="card cal-sync">
      <div class="card-head"><h2>This Project and the Lab</h2><span class="pill ${pill[1]}">${pill[0]}</span></div>
      <div class="cal-sync-row">
        <div title="resources/spot_calibration.json"><span class="field-label">This Project</span><b>${when(mine.updated)}</b></div>
        <div title="Calibrations run on the lab console (its Calibration tab)"><span class="field-label">Lab Console</span><b>${reach && lab ? when(lab.updated) : '—'}</b></div>
        <div class="cal-sync-actions">
          <button type="button" class="btn btn-sm btn-primary" data-cal="take" ${canTake ? '' : 'disabled'}
            title="Copy the lab console's calibration into this project; the next simulation uses it">Update from Lab</button>
          <label class="toggle" title="When this computer sees the lab console and its calibration is newer than this project's, offer it"><input type="checkbox" data-cal="ask" ${ask ? 'checked' : ''}><span></span>Ask When the Lab's Is Newer</label>
        </div>
      </div>
    </section>`;
  }

  // The Table Slope section's parts (renderTable): the left column, and the 3D view card's head
  // and what is under the view.
  function tableSection() {
    const t = S.cal && S.cal.table;
    const run = S.run && S.run.kind === 'table' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    return {
      left: `${S.local ? syncCard() : ''}
        ${S.local ? labOnlyCard('table') : runCard(run, running) + savedRunsCard('table', running)}
        ${run ? progressCard(run) : ''}`,
      head: '<h2 class="tip" title="The table coloured by its fitted height (blue low, red high)' +
        `${running ? ', with the platform and its drifts live' : ''}. Drag to orbit, ` +
        'right-drag to pan, scroll to zoom.">Table Slope</h2>' +
        `${running ? `<span class="pill live">Measuring · ${platformName(run.platform)}</span>`
          : t ? '<span class="pill ok">' +
            `${t.model === 'quadratic' ? 'Surface' : 'Plane'} · ${when(t.updated)}</span>`
          : '<span class="pill">Not Calibrated</span>'}`,
      below: running ? '<p class="hint">The fitted slope appears when the run is done.</p>'
        : t ? `${colorBar(heightGrid(t).range)}${tableStats(t)}`
        : '<p class="hint">No table calibration yet: a level table.</p>',
    };
  }

  // More Details (How It Works)

  // A calibration's steps, each in the state STATE(k) gives ('now', 'done' or '').
  const stepList = (steps, state = () => '') => '<ol class="cal-steps">' +
    `${steps.map(([title, text, icon], k) => `<li class="cal-step ${state(k)}">` +
      `<svg viewBox="0 0 24 24" aria-hidden="true">${icon}</svg>` +
      `<div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`).join('')}</ol>`;
  const ARM_ICONS = [
    '<path d="M4 18h16M12 18V8M8 11l4-4 4 4"/>',
    '<path d="M4 12c3-6 5-6 8 0s5 6 8 0"/>',
    '<path d="M5 18 19 6M13 6h6v6"/>',
    '<path d="M4 16h4v-6h4v8h4v-4h4"/>',
  ];

  // The dialog that explains the section's calibration, step by step, with the run's step lit.
  function showDetails() {
    const run = S.run && sectionOf(S.run.kind) === S.section ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    let title;
    let html;
    if (S.section === 'table') {
      // Which step the run is in.
      let stage = -1;
      if (run && run.step != null) {
        const name = run.steps[run.step].name;
        stage = name === 'Still' ? 0 : name === 'Float Here' ? 1 : name === 'Fit' ? 3 : 2;
        if (!running) {
          stage = run.state === 'done' ? 4 : stage;
        }
      }
      title = 'Table Slope Calibration';
      html = stepList(TABLE_STEPS, k => (running && k === stage ? 'now'
        : stage > k || (run && run.state === 'done') ? 'done' : '')) +
        `<p class="hint">A drift ends after its time, or at once (pucks off) when the platform nears an edge or another platform.
          The platform needs PhaseSpace to see it the whole time, and the table clear around the grid.</p>`;
    } else if (S.section === 'thrusters' && (run ? run.kind === 'decay' : S.thrStage === 'decay')) {
      const msg = run ? run.message : '';
      const stage = !running ? -1
        : /flying to the centre/.test(msg) ? 0
        : /resting/.test(msg) ? 1
        : /firing/.test(msg) ? 2
        : run.steps[run.step] && run.steps[run.step].name === 'Fit' ? 3
        : -1;
      title = 'Thrusters Together';
      html = stepList(DECAY_STEPS,
        k => (k === stage ? 'now' : run && run.state === 'done' ? 'done' : '')) +
        `<p class="hint">Opened together, each thruster gives less than it does alone: the valves share one regulated supply. The
          platform's push with thrusters 1 to N open is compared with the sum of what each gives alone (the one-at-a-time
          calibration, so run that first), and the straight line r(N) = s (1 − m (N − 1)) is fitted: each extra valve open
          takes m of every thruster's thrust alone, and s is the thrust alone that day against the calibration (near 1).</p>
        <p class="hint">The mixer (the diagram's, so the simulated plant's and the platforms', and this console's) uses m: the Teensy
          opens every valve at the start of each PWM period and closes each after its duty cycle, so how many are open, and so
          each thruster's thrust, follows from the duty cycles. A platform not measured yet takes the Obstacle's.</p>
        <p class="hint">Opposite thrusters cancel: with 1 to 5, 6 or 7 open the net push is two, two or one thrusters' worth, and with
          all eight it is none, so that firing is only a check (the platform should stay still) and the fitted curve covers it.</p>`;
    } else if (S.section === 'thrusters') {
      const msg = run ? run.message : '';
      const stage = !running ? -1
        : /flying to the centre/.test(msg) ? 0
        : /resting/.test(msg) ? 1
        : /firing/.test(msg) ? 2
        : -1;
      title = 'Thruster Calibration';
      html = stepList(THRUSTER_STEPS,
        k => (k === stage ? 'now' : run && run.state === 'done' ? 'done' : '')) +
        `<p class="hint">Each burn's thrust comes from the push it gave: the platform's mass times its acceleration, with the
          table's slope (its calibration) taken out. So the platform's mass, inertia and centre of gravity must be right: the
          console asks before it starts. A second stage (Together) then measures how the thrust falls with several open.</p>`;
    } else {
      const now = running && run.step != null ? run.steps[run.step] : null;
      title = 'Arm Calibration';
      html = stepList(ARM_TESTS.map(([, t, text], k) => [t, text, ARM_ICONS[k]]),
        k => (now && now.test === ARM_TESTS[k][0] ? 'now' : '')) +
        `<p class="hint">For each joint in turn, the other joints hold their position. The script
          (resources/arm_calibration/calibrate_arm.py) runs on the Chaser, which has the arm's U2D2; it fits each joint's friction
          and inertia, which SPOT's arm model then uses (spot.arm.parameters). The Chaser rests on the table throughout.</p>`;
    }
    $('#calDetailsTitle').textContent = title;
    $('#calDetailsBody').innerHTML = html;
    $('#calDetailsDialog').showModal();
  }
  const detailsButton = '<button type="button" class="btn btn-sm btn-ghost" data-cal="details" ' +
    'title="How this calibration works, step by step">More Details</button>';

  // A run card's status: Running, Another Calibration Runs, Ready, or (no PhaseSpace yet) a
  // button that starts it.
  function runPill(running, streaming) {
    return running ? '<span class="pill live">Running</span>'
      : busyOther() ? '<span class="pill warn">Another Calibration Runs</span>'
      : streaming ? '<span class="pill ok">Ready</span>'
      : '<button type="button" class="btn btn-sm btn-go" data-cal="start-stream" ' +
        'title="Start PhaseSpace streaming (at the PhaseSpace tab\'s frequency): the calibration ' +
        'measures the platform\'s motion with it">Start PhaseSpace</button>';
  }

  // The table calibration's numbers: tilt, steepest slope, downhill, residual, and its drifts.
  function tableStats(t) {
    const fixed = t.damping_fixed || [];
    const drag = Object.entries(t.damping || {}).map(([n, c]) => (fixed.includes(n)
      ? `${platformName(n)} not resolved` : `${platformName(n)} ${fmt(c, 3)} /s`)).join(', ');
    const tile = (label, value, unit, title) => `<div class="cal-tile" title="${esc(title)}">` +
      `<span>${label}</span><b>${value}</b><i>${unit}</i></div>`;
    const dir = t.downhill_deg;
    return `<div class="cal-tiles">
        ${tile('Tilt at Centre', fmt(t.tilt_centre_mrad, 3), 'mrad', 'The slope at the table\'s centre: 1 mrad is a 1 mm drop per metre, and pulls a floating platform at 0.0098 m/s²')}
        ${tile('Steepest', fmt(t.tilt_max_mrad, 3), 'mrad', 'The steepest slope anywhere on the table')}
        ${tile('Downhill', `<svg class="cal-dir" viewBox="-10 -10 20 20" aria-hidden="true"><g transform="rotate(${-dir})"><path d="M-7 0H6M2-4l4 4-4 4" /></g></svg>${fmt(dir, 0)}°`, 'from +x', 'Which way is downhill at the centre, from +x towards +y')}
        ${tile('Fit Residual', fmt(t.rms_mm, 2), 'mm', 'How far the drifts are from the fitted model (the cameras\' own noise is ' + fmt(t.noise_mm, 2) + ' mm)')}
      </div>
      ${t.warning ? `<p class="hint warn-text">${esc(t.warning)}</p>` : ''}
      <p class="hint" title="${t.model === 'plane' ? 'A plane: one tilt everywhere.' : 'A smooth surface (quadratic).'}${fixed.length ? ' A drag not resolved: longer drifts tell it from the slope; 0 is used.' : ''}">${t.drifts} drift${t.drifts === 1 ? '' : 's'} of the ${platformName(t.platform)} · by ${esc(t.by || '?')}${t.loaded && t.run
          ? ` · from the run of ${esc(`${t.run.slice(0, 4)}-${t.run.slice(4, 6)}-${t.run.slice(6, 8)} ${t.run.slice(9, 11)}:${t.run.slice(11, 13)}`)}` : ''}
        · drag ${drag || '—'}</p>
      ${S.local ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-table" title="Remove the table calibration from the lab console\'s file">Remove</button></div>'}`;
  }

  // The table's Run card: the platform, the grid and drift settings, and Start or Stop (while a
  // run goes on, its own platform and settings).
  function runCard(run, running) {
    const f = running ? run.params : S.form;
    const st = getStream();
    const streaming = st && st.running;
    const chosen = running ? run.platform : S.platform;
    const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
        <input type="number" class="input" data-calform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
    return `<section class="card">
        <div class="card-head"><h2>Run</h2>${runPill(running, streaming)}</div>
        <div class="segmented cal-plat" role="group" aria-label="Platform">${plat}</div>
        <div class="cal-form">
          ${field('nx', 'Grid Columns', 1, 5, 1, '', 'How many start points along x')}
          ${field('ny', 'Grid Rows', 1, 4, 1, '', 'How many start points along y')}
          ${field('drift', 'Drift Time', 4, 40, 1, 's', 'How long each drift is recorded (at most)')}
          ${field('margin', 'Edge Margin', 0.25, 0.8, 0.05, 'm', 'A drift ends (pucks off) when the platform\'s centre comes this close to an edge')}
          <label class="field" title="Quadratic: a smooth surface (needs 3+ drifts over most of the table). Plane: one tilt everywhere. Auto: chooses."><span class="field-label">Model</span>
            <select class="input" data-calform="model" ${running ? 'disabled' : ''}>
              ${['auto', 'quadratic', 'plane'].map(m => `<option value="${m}" ${f.model === m ? 'selected' : ''}>${{ auto: 'Auto', quadratic: 'Surface', plane: 'Plane' }[m]}</option>`).join('')}
            </select></label>
        </div>
        <p class="hint" title="Clear the table around the grid; other platforms may stay where they are (points next to them are skipped).">${f.nx * f.ny + 1} drifts · about ${Math.round((f.nx * f.ny + 1) * (Number(f.drift) + 35) / 60)} min</p>
        <div class="cal-actions">
          ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
                    : `<button type="button" class="btn btn-go" data-cal="start-table" ${!streaming ? 'disabled title="Start PhaseSpace first"' : busyOther() ? 'disabled title="Another calibration is running"' : ''}>Start Table Calibration</button>`}
          ${detailsButton}
        </div>
      </section>`;
  }

  // A saved run fitted again instead of a new run: the table's drifts (with the Run card's
  // Model), or a platform's burns (with the run's mass properties and the lab's table
  // calibration now).
  const RUN_PART = { table: 'drift', thrusters: 'burn', decay: 'firing' };
  const RUNS_TIP = {
    table: 'Or skip the flying: fit the table again to a run saved on the lab console, with the ' +
      'Run card\'s Model, and make that the lab\'s table calibration. The run\'s own data isn\'t ' +
      'changed.',
    thrusters: 'Or skip the flying: fit each burn of a run saved on the lab console again (with ' +
      'that run\'s mass properties, and the lab\'s table calibration now), and make that the ' +
      'lab\'s thruster calibration for its platform. The run\'s own data isn\'t changed.',
    decay: 'Or skip the flying: fit each firing of a Together run saved on the lab console again ' +
      '(the straight line), and make that the platform\'s thrust decay. A run marked stopped ' +
      'ended early (out of air, say): its pressure fell, so its decay is not the valves\'.',
  };

  // The Previous Runs card: a saved run to load.
  function savedRunsCard(kind, running) {
    const runs = S.savedRuns[kind];
    let inner;
    if (!runs) {
      inner = '<p class="hint">Looking for saved runs…</p>';
    } else if (!runs.length) {
      inner = `<p class="hint">No saved run has a ${RUN_PART[kind]} to fit yet.</p>`;
    } else {
      const pick = runs.find(r => r.id === S.runPick[kind]) || runs[0];
      const label = r => `${r.started} · ${platformName(r.platform)} · ` +
        `${r.count} ${RUN_PART[kind]}${r.count === 1 ? '' : 's'}` +
        `${r.stopped && kind === 'decay' ? ' · stopped' : ''}${r.current ? ' · in use' : ''}`;
      inner = `<label class="field"><span class="field-label">Saved Run</span>
          <select class="input" data-calrun="${kind}" ${running ? 'disabled' : ''}>${runs.map(r =>
            `<option value="${esc(r.id)}" ${r === pick ? 'selected' : ''}>${esc(label(r))}</option>`).join('')}</select></label>
        <div class="cal-actions"><button type="button" class="btn" data-cal="load-run" data-kind="${kind}" ${S.active ? 'disabled title="A calibration is running"' : ''}>Load Run</button></div>`;
    }
    return `<section class="card"><div class="card-head"><h2 class="tip" title="${RUNS_TIP[kind]}">Previous Runs</h2></div>
        ${inner}</section>`;
  }

  // Reads a kind's saved runs from the lab console.
  async function fetchRuns(kind) {
    S.savedRuns[kind] = (await api(`/api/calibration/${kind}/runs`)).runs;
    if (S.shown && S.section === sectionOf(kind)) {
      render();
    }
  }
  const shownRunsKind = () =>
    (S.section === 'thrusters' && S.thrStage === 'decay' ? 'decay' : S.section);
  const fetchShownRuns = () => {
    if (!S.local && (S.section === 'table' || S.section === 'thrusters')) {
      act(() => fetchRuns(shownRunsKind()));
    }
  };

  // Local console: the calibration runs on the lab console (with a link to it).
  function labOnlyCard(what) {
    return `<section class="card"><div class="card-head"><h2 class="tip" title="The ${what === 'table' ? 'table slope' : what} calibration drives a platform, so it runs on the lab console. Its results come here with Update from Lab.">Run</h2><span class="pill">On the Lab Console</span></div>
        ${S.lab && S.lab.url ? `<p class="hint"><a href="${esc(S.lab.url)}#calibration" target="_blank" rel="noopener">${esc(S.lab.url)}</a></p>` : ''}
        <div class="cal-actions">${detailsButton}</div></section>`;
  }

  // A run's progress: its state, message, steps and log.
  function progressCard(run) {
    const icon = { waiting: '○', running: '◉', done: '✓', skipped: '–', failed: '✕' };
    const steps = run.steps.map((s, k) => `<li class="cal-prog ${s.state}${k === run.step ? ' current' : ''}"><span>${icon[s.state] || '○'}</span>
        <b>${esc(s.name)}</b><i>${esc(s.note || '')}</i></li>`).join('');
    const kind = {
      running: 'live', stopping: 'warn', done: 'ok', failed: 'err', stopped: 'warn',
    }[run.state] || '';
    const label = {
      running: 'Running', stopping: 'Stopping', done: 'Done', failed: 'Failed', stopped: 'Stopped',
    }[run.state] || run.state;
    return `<section class="card">
        <div class="card-head"><h2>Progress · ${platformName(run.platform)}</h2><span class="pill ${kind}">${label}</span></div>
        <p class="cal-msg ${run.state === 'failed' ? 'bad' : ''}">${esc(run.message)}</p>
        <ol class="cal-progress">${steps}</ol>
        <details class="advanced"><summary>Log</summary><pre class="cal-log">${esc(run.log.join('\n'))}</pre></details>
      </section>`;
  }

  // The thrusters

  // The Thrusters section: the run (or the lab-only card), its progress, and the map while it
  // runs, else the platform's results.
  function thrustersSection() {
    const run = S.run && sectionOf(S.run.kind) === 'thrusters' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    const step = run && run.step != null ? run.steps[run.step].name : '';

    // The thruster(s) firing now: "Thruster 3" (one at a time) or "Thrusters 1-3" (together).
    const m = running && /firing/.test(run.message)
      ? /^Thrusters? (\d+)(?:-(\d+))?$/.exec(step)
      : null;
    const firing = m
      ? (m[2] ? Array.from({ length: m[2] - m[1] + 1 }, (_, i) => Number(m[1]) + i)
        : [Number(m[1])])
      : [];
    const viewing = running ? run.platform : S.platform;
    const res = (S.cal && S.cal.thrusters || {})[viewing];
    return `<div class="cal-col">
        ${S.local ? syncCard() : ''}
        ${S.local ? labOnlyCard('thrusters') : thrustRunCard(run, running) + savedRunsCard(running ? run.kind : S.thrStage === 'decay' ? 'decay' : 'thrusters', running)}
        ${run ? progressCard(run) : ''}
      </div>
      <div class="cal-col cal-wide">
        ${running ? `<section class="card cal-map-card"><div class="card-head"><h2>Thrusters</h2><span class="pill live">${firing.length
            ? `Firing T${firing[0]}${firing.length > 1 ? `–T${firing[firing.length - 1]}` : ''}` : 'Measuring'} · ${platformName(run.platform)}</span></div>
            <div class="cal-thr-grid">
              ${thrusterMap({ firing })}
              <div class="cal-map"><canvas id="calMap" aria-label="The table seen from above: the platform and its burns"></canvas></div>
            </div>
            <p class="hint">The platform, its burns so far (yellow), and the centre each burn starts from.</p></section>`
          : thrustResults(viewing, res || placeholderThrust(viewing)) + decayCard(viewing, res)}
      </div>`;
  }

  // The Thrusters Run card: the stage, the platform, the settings, and Start or Stop.
  function thrustRunCard(run, running) {
    const f = running ? run.params : thrForm();
    const st = getStream();
    const streaming = st && st.running;
    const chosen = running ? run.platform : S.platform;
    const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
        <input type="number" class="input" data-thrform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
    // Each burn: a flight to the centre, 5 s rest, the burn.
    const minutes = Math.round(8 * f.repeats * (Number(f.burn) + 20) / 60);
    const table = S.cal && S.cal.table;
    const stage = running ? run.kind : S.thrStage;
    const single = (S.cal && S.cal.thrusters || {})[chosen];
    const canTogether = Boolean(single && (single.valid || []).some(Boolean));
    const stageBtn = (v, label, title) => `<button type="button" data-thrstage="${v}" ` +
      `class="${stage === v ? 'active' : ''}" title="${esc(title)}" ${running ? 'disabled' : ''}>` +
      `${label}</button>`;

    // Start: Together needs the one-at-a-time calibration first.
    const go = stage === 'decay'
      ? `<button type="button" class="btn btn-go" data-cal="start-thrusters" ${streaming && !busyOther() && canTogether ? '' : 'disabled'}
           title="${canTogether ? '' : `Calibrate the ${platformName(chosen)}'s thrusters one at a time first`}">Start Together Calibration…</button>`
      : '<button type="button" class="btn btn-go" data-cal="start-thrusters" ' +
        `${streaming && !busyOther() ? '' : 'disabled'}>Start Thruster Calibration…</button>`;
    return `<section class="card">
        <div class="card-head"><h2>Run</h2>${runPill(running, streaming)}</div>
        <div class="segmented cal-stage" role="group" aria-label="Stage">
          ${stageBtn('single', 'One at a Time', 'The first stage: each thruster alone, its thrust and direction')}
          ${stageBtn('decay', 'Together', 'The second stage: thrusters 1, 1-2, … 1-8 at once, how the thrust falls with more open (after the first stage)')}
        </div>
        <div class="segmented cal-plat" role="group" aria-label="Platform">${plat}</div>
        <div class="cal-form">
          ${field('repeats', 'Repeats', 1, 3, 1, '', stage === 'decay' ? 'How many times each firing is made (their ratios are fitted together)' : 'How many burns of each thruster (their thrusts are averaged)')}
          ${field('burn', 'Burn Time', 0.5, 3, 0.5, 's', 'How long each thruster fires (longer: more motion to measure, but more room needed)')}
          ${field('margin', 'Edge Margin', 0.25, 0.8, 0.05, 'm', 'A burn ends early when the platform comes this close to an edge')}
        </div>
        <p class="hint" title="${table ? `The table's slope (calibrated ${esc(String(table.updated).slice(0, 10))}) is taken out of each burn.` : 'Calibrate the table first for the best thrusts.'}">${8 * f.repeats} ${stage === 'decay' ? 'firings' : 'burns'} · about ${minutes} min${table ? '' : ' · <span class="warn-text">no table calibration</span>'}${stage === 'decay' && !canTogether ? ' · <span class="warn-text">one at a time first</span>' : ''}</p>
        <div class="cal-actions">
          ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>' : go}
          ${detailsButton}
        </div>
      </section>`;
  }

  // Another section's calibration is running.
  function busyOther() {
    return S.active && S.run && sectionOf(S.run.kind) !== S.section;
  }

  // A platform's thruster results: the map and bars, the table, the burns, and what they assumed.
  function thrustResults(name, res) {
    const picker = NAMES.map(n => `<button type="button" data-calview="${n}" class="${n === name ? 'active' : ''}">
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const head = `<div class="card-head"><h2>Thrusters</h2>${res.placeholder ? `<span class="pill tip" title="No thruster calibration for the ${platformName(name)} yet${S.local ? ' (the lab console measures it; Update from Lab brings it here)' : ''}: the nominal thrust for all eight, as simulations use until it is calibrated, and the burns it would give">Placeholder · Nominal</span>` : ''}
        <div class="segmented cal-plat" role="group" aria-label="Platform">${picker}</div></div>`;
    const nominal = res.nominal || 0.2825;
    const f = res.thrust;
    const row = k => `<tr><td>T${k + 1}${res.valid[k] ? '' : ' <span class="warn-text" title="No good burn: the nominal thrust is used">·</span>'}</td>
        <td>${fmt(f[k], 4)}</td><td>${res.thrust_sd[k] != null ? fmt(res.thrust_sd[k], 4) : '—'}</td>
        <td style="color:${f[k] / nominal < 0.97 || f[k] / nominal > 1.03 ? 'var(--warn)' : 'inherit'}">${res.valid[k] ? `${f[k] >= nominal ? '+' : ''}${fmt(100 * (f[k] / nominal - 1), 1)} %` : '—'}</td>
        <td>${res.angle[k] != null ? `${fmt(res.angle[k], 1)}°` : '—'}</td>
        <td style="color:${res.torque_ratio[k] != null && Math.abs(res.torque_ratio[k] - 1) > 0.1 ? 'var(--warn)' : 'inherit'}">${res.torque_ratio[k] != null ? fmt(res.torque_ratio[k], 2) : '—'}</td></tr>`;
    const bad = res.valid.filter(v => !v).length;
    const a = res.params && res.params.assumed;
    return `<section class="card">${head}
        <div class="cal-thr-grid">
          ${thrusterMap({ thrust: f, nominal })}
          <div>${thrustBars({ thrust: f, sd: res.thrust_sd, nominal, valid: res.valid })}</div>
        </div>
        <table class="cal-thr-table"><thead><tr><th>Thruster</th><th>Thrust [N]</th><th>± [N]</th><th>vs Nominal</th>
          <th title="How far the push is turned from the thruster's direction (towards the body's +z rotation)">Turned</th>
          <th title="The torque it gave over the torque the mixer expects from it (moment arm × thrust): far from 1, the CG or the inertia is off">Torque Ratio</th></tr></thead>
          <tbody>${f.map((_, k) => row(k)).join('')}</tbody></table>
        ${bad ? `<p class="hint warn-text">${bad} thruster${bad > 1 ? 's' : ''} had no good burn and keep${bad > 1 ? '' : 's'} the nominal thrust.</p>` : ''}
        ${burnPaths(res.burns || [])}
        ${res.placeholder ? `<p class="hint">Not measured yet: ${fmt(nominal, 4)} N for each thruster.</p>` : `<p class="hint">Measured ${when(res.updated)} by ${esc(res.by || '?')}, assuming ${a ? `${fmt(a.mass, 2)} kg, ${fmt(a.inertia, 4)} kg m², CG (${fmt(a.cg_mm[0], 1)}, ${fmt(a.cg_mm[1], 1)}) mm${a.changed ? ' (changed from the defaults)' : ''}` : '—'};
          ${res.table_used ? 'the table\'s slope taken out' : 'no table calibration then'}. Simulations and the lab console's controller use these thrusts (Scenario: Calibrated Thrust).</p>`}
        ${S.local || res.placeholder ? '' : `<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-thrusters" title="Remove the ${platformName(name)}'s thruster calibration from the lab console's file">Remove</button></div>`}
      </section>`;
  }

  // Thrusters together (the second stage): each one's share of its thrust alone against how many
  // are open, and the fitted decay; a placeholder (no decay) until it is measured.
  function decayCard(name, single) {
    const dec = single && single.decay;

    // Not measured: the decay simulations and the controller use until it is (the setting's
    // default, the Obstacle's).
    const m = Number(((S.assumed || {})[name] || {}).thrust_decay) || DEFAULT_DECAY;
    const d = dec || {
      model: 'linear', slope: m, k: 0, scale: 1, firings: [], placeholder: true,
      drop_pct: Array.from({ length: 8 }, (_, i) => 100 * Math.min(1, m * i)),
    };

    // Fitted before the straight line was settled on.
    const unused = !d.placeholder && d.model !== 'linear';
    const pill = d.placeholder
      ? '<span class="pill tip" title="Not measured yet for the ' +
        `${platformName(name)}: the decay simulations and the controller use until it is, the ` +
        'Obstacle\'s (measured 2026-10-14). Run the Together stage after the one-at-a-time ' +
        'one.">Placeholder · Obstacle\'s</span>'
      : unused ? '<span class="pill warn tip" title="Fitted with the earlier curve, which ' +
        'simulations and the controller don\'t use (they take ' +
        `${fmt(100 * m, 1)} % per valve): load its run again under Previous Runs (Together) to ` +
        'fit the straight line.">Not Used · Refit</span>'
      : `<span class="pill ok">${when(d.updated)}</span>`;

    // Each N's measured share (mean of its firings, over the scale).
    const used = n => (d.firings || []).filter(f => f.n === n);
    const cell = n => {
      const fs = used(n).filter(f => f.ratio != null);
      if (!fs.length) {
        return '—';
      }
      const v = fs.reduce((a, f) => a + f.ratio, 0) / fs.length / (d.scale || 1);
      return `${fmt(100 * v, 1)} %${fs.some(f => f.used) ? ''
        : ' <span class="muted" title="Left out: their pushes cancel">(out)</span>'}`;
    };
    const rows = Array.from({ length: 8 }, (_, i) => i + 1).map(n => `<tr><td>1${n > 1 ? `–${n}` : ''}</td><td>${cell(n)}</td>
        <td>${fmt(100 - d.drop_pct[n - 1], 1)} %</td><td>${n === 1 ? '—' : `−${fmt(d.drop_pct[n - 1], 1)} %`}</td></tr>`).join('');
    return `<section class="card">
        <div class="card-head"><h2 class="tip" title="Each thruster's output with N open at once, as a share of its thrust alone: with N open each gives ${decayFormula(d)} of it. The mixer, and so the simulation and the experiments, use m.">Thrusters Together</h2>${pill}</div>
        <div class="cal-thr-grid cal-decay-grid">
          <div>${decayPlot(d)}</div>
          <table class="cal-thr-table"><thead><tr><th>Open</th><th title="The firing's push against its thrusters' alone (over s)">Measured</th>
            <th title="${decayFormula(d)}">Fitted</th><th>Drop</th></tr></thead><tbody>${rows}</tbody></table>
        </div>
        <p class="hint" ${d.placeholder || d.rms_linear == null ? '' : `title="Residual of the straight line ${fmt(100 * d.rms_linear, 1)} %, of 1 / (1 + k (N − 1)) ${fmt(100 * d.rms_rational, 1)} %"`}>${d.placeholder ? `Not measured yet: the Obstacle's ${fmt(100 * m, 1)} % per extra valve.`
          : `${d.model === 'rational' ? `k = ${fmt(d.k, 4)}` : `m = ${fmt(100 * d.slope, 1)} % per thruster added`} · s = ${fmt(d.scale, 3)} (thrust alone against the calibration) · residual ${fmt(100 * d.rms, 1)} %`}</p>
      </section>`;
  }

  // The thrusters on the platform seen from above, as on the Hardware tab: each one's thrust
  // (coloured by how far it is from nominal), or the one firing lit.
  function thrusterMap({ thrust = null, nominal = 0.2825, firing = null } = {}) {
    const tags = Object.entries(THRUSTER_MAP).map(([n, [side, x, y]]) => {
      const f = thrust ? thrust[n - 1] : null;
      const c = f != null ? deviationColor(f / nominal) : null;
      return `<span class="thruster cal-thr-tag${[].concat(firing || []).includes(Number(n)) ? ' on' : ''}" data-side="${side}" style="left:${x}%;top:${y}%${c ? `;border-color:${c};--c:${c}` : ''}"
        ${f != null ? `title="T${n}: ${f.toFixed(4)} N (${f >= nominal ? '+' : ''}${(100 * (f / nominal - 1)).toFixed(1)} % from nominal)"` : ''}>T${n}${f != null ? `<b>${f.toFixed(3)}</b>` : ''}</span>`;
    }).join('');
    return '<div class="thruster-map cal-thruster-map"><div class="hw-body">' +
      `<span class="hw-axis">+x ↑</span></div>${tags}</div>`;
  }

  // No calibration yet: what one looks like, with the nominal thrust (the controller's) for all
  // eight, each burn the push that thrust would give the platform (its assumed mass) over the
  // burn time.
  function placeholderThrust(name) {
    const a = (S.assumed || {})[name] || {};
    const F = Number([].concat(a.thrust || [])[0]) || 0.2825;
    const mass = Number(a.mass) || 13.5;
    const T = Number(S.thrForm.burn) || 2;
    const burns = THRUSTERS.map(([, push], k) => ({
      thruster: k + 1, thrust: F,
      points: Array.from({ length: 16 }, (_, i) => {
        const t = T * i / 15;
        const d = 0.5 * F / mass * t * t;
        return [push[0] * d, push[1] * d];
      }),
    }));
    const eight = v => Array(8).fill(v);
    return {
      placeholder: true, nominal: F, thrust: eight(F), thrust_sd: eight(null), valid: eight(true),
      angle: eight(0), torque_ratio: eight(1), burns,
    };
  }

  // The assumptions dialog: the platform's mass properties, which the person starting it
  // confirms or changes. Together takes the mass properties the first stage assumed, so the two
  // compare.
  function askThrusters() {
    const n = S.platform;
    const single = (S.cal && S.cal.thrusters || {})[n];
    const a = (S.thrStage === 'decay' && single && single.params && single.params.assumed) ||
      (S.assumed || {})[n];
    if (!a) {
      toast('The lab console didn\'t say what it takes the platform\'s mass properties to be; ' +
        'reload the page.', 'error');
      return;
    }
    const dlg = $('#calThrustDialog');
    $('#calThrustName').textContent = platformName(n);

    // Shown rounded; a value left as shown is sent exactly as the console has it (data-exact).
    const set = (k, v, digits) => {
      const el = dlg.querySelector(`[data-assume="${k}"]`);
      el.value = el.dataset.shown = String(Number(Number(v).toFixed(digits)));
      el.dataset.exact = v;
    };
    set('mass', a.mass, 3);
    set('inertia', a.inertia, 5);
    set('cgx', a.cg_mm[0], 2);
    set('cgy', a.cg_mm[1], 2);
    const t = S.cal && S.cal.table;
    $('#calThrustTable').innerHTML = t
      ? `The table's slope, calibrated ${esc(String(t.updated).slice(0, 10))}, ` +
        'is taken out of each burn.'
      : '<span class="warn-text">There is no table calibration, so the table\'s slope is not ' +
        'taken out: calibrate the table first for the best thrusts.</span>';
    $('#calThrustAck').checked = false;
    $('#calThrustGo').disabled = true;
    dlg.showModal();
  }

  // The dialog's acknowledgement enables Start, which sends the mass properties confirmed.
  $('#calThrustAck').onchange = ev => {
    $('#calThrustGo').disabled = !ev.target.checked;
  };
  $('#calThrustGo').onclick = () => act(async () => {
    const dlg = $('#calThrustDialog');
    const v = k => {
      const el = dlg.querySelector(`[data-assume="${k}"]`);
      return Number(el.value === el.dataset.shown ? el.dataset.exact : el.value);
    };
    readThrForm();
    dlg.close();
    onRun(await api(S.thrStage === 'decay' ? '/api/calibration/decay/start'
      : '/api/calibration/thrusters/start', {
      platform: S.platform, acknowledged: true, ...thrForm(),
      assumed: { mass: v('mass'), inertia: v('inertia'), cg_mm: [v('cgx'), v('cgy')] },
    }));
  });

  // Reads the Thrusters Run card's settings, remembered per stage.
  function readThrForm() {
    const f = thrForm();
    for (const el of body.querySelectorAll('[data-thrform]')) {
      f[el.dataset.thrform] = Number(el.value);
    }
    storage.set(S.thrStage === 'decay' ? 'spot.calibration.decay' : 'spot.calibration.thrusters',
      f);
  }

  // The Run card's settings for the stage chosen (one at a time, together).
  function thrForm() {
    return S.thrStage === 'decay' ? S.decayForm : S.thrForm;
  }

  // The arm

  // The Arm section: the Chaser's setup and the run (or the lab-only card), its progress, and the
  // results.
  function armSection() {
    const run = S.run && S.run.kind === 'arm' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    const res = armResult(run) || placeholderArm();
    return `<div class="cal-col">
        ${S.local ? syncCard() : ''}
        ${S.local ? labOnlyCard('arm') : armChecksCard() + armRunCard(run, running)}
        ${run ? armProgressCard(run) : ''}
      </div>
      <div class="cal-col cal-wide">${armResults(res)}</div>`;
  }

  // The Chaser's setup for calibrate_arm.py: each check, with its fix if it fails.
  function armChecksCard() {
    const c = S.armCheck;
    let body;
    if (S.armChecking) {
      body = '<p class="hint">Asking the Chaser…</p>';
    } else if (!c) {
      body = '<p class="hint">Not checked yet.</p>';
    } else if (!c.reachable) {
      body = `<p class="hint bad">The Chaser doesn't answer: ${esc(c.error)}</p>`;
    } else {
      body = `<ul class="cal-checks">${c.items.map(i => {
        const mark = !i.ok ? '<span class="n">✕</span>'
          : i.warn ? '<span class="w">!</span>'
          : '<span class="y">✓</span>';
        return `<li>${mark}<b>${esc(i.what)}</b><span class="mono">${esc(i.value)}</span>` +
          `${!i.ok || i.warn ? `<span class="fix">${esc(i.fix)}</span>` : ''}</li>`;
      }).join('')}</ul>${c.running
        ? '<p class="hint warn-text">A calibration is already running on the Chaser.</p>'
        : ''}`;
    }
    const pill = !c ? ''
      : c.ok ? '<span class="pill ok">Ready</span>'
      : '<span class="pill warn">Not Ready</span>';
    return `<section class="card"><div class="card-head"><h2 class="tip" title="What calibrate_arm.py needs on the Chaser. A rehearsal (simulated servos) needs none of it.">The Chaser's Setup</h2>${pill}</div>${body}
        <div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="arm-check" ${S.armChecking ? 'disabled' : ''}>Check Again</button></div></section>`;
  }

  // The Arm Run card: joints, tests, Rehearse, and Start or Stop.
  function armRunCard(run, running) {
    const f = running ? run.params : S.armForm;
    const chip = (kind, value, label, on) =>
      `<button type="button" class="cal-chip${on ? ' on' : ''}" data-arm${kind}="${value}" ` +
      `${running ? 'disabled' : ''}>${label}</button>`;
    const minutes =
      Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
    const ready = S.armCheck && S.armCheck.ok;
    return `<section class="card">
        <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : busyOther() ? '<span class="pill warn">Another Calibration Runs</span>' : ''}</div>
        <span class="field-label">Joints</span><div class="cal-chips">${[1, 2, 3].map(j => chip('joint', j, JOINTS[j], f.joints.includes(j))).join('')}</div>
        <span class="field-label">Tests</span><div class="cal-chips">${ARM_TESTS.map(([k, t]) => chip('test', k, t, f.tests.includes(k))).join('')}</div>
        <label class="toggle" title="Run the script with its simulated servos instead: it needs no hardware, moves nothing, and its result is shown but never becomes the lab's"><input type="checkbox" data-armsim ${f.simulate ? 'checked' : ''} ${running ? 'disabled' : ''}><span></span>Rehearse (Simulated Servos)</label>
        <p class="hint">${f.simulate ? 'A rehearsal takes under a minute.' : `About ${minutes} min with the real servos.`}</p>
        <div class="cal-actions">
          ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
                    : `<button type="button" class="btn btn-go" data-cal="start-arm" ${!f.joints.length || !f.tests.length || busyOther() || (!f.simulate && !ready) ? 'disabled' : ''}
                        title="${!f.simulate && !ready ? 'The Chaser isn\'t ready (see its setup)' : ''}">${f.simulate ? 'Start Rehearsal' : 'Start Arm Calibration…'}</button>`}
          ${detailsButton}
        </div></section>`;
  }

  // The arm run's progress, with a progress bar over its tests, and calibrate_arm.py's own log
  // (without its PROGRESS lines) in place of the run's.
  function armProgressCard(run) {
    const tests = run.steps.filter(s => s.test);
    const done = tests.filter(s => s.state === 'done').length;
    const pct = tests.length ? Math.round(100 * done / tests.length) : 0;
    return progressCard(run)
      .replace('<ol class="cal-progress">', '<div class="cal-bar-progress">' +
        `<i style="width:${pct}%"></i></div><ol class="cal-progress">`)
      .replace(/<pre class="cal-log">[\s\S]*?<\/pre>/, `<pre class="cal-log">${esc(
        (run.armlog || []).filter(l => !l.startsWith('PROGRESS ')).join('\n')
        || run.log.join('\n'))}</pre>`);
  }

  // What to show: a rehearsal just run (it isn't saved), else the lab's arm calibration.
  function armResult(run) {
    if (run && run.result && run.result.format === 'spot-arm-calibration' && run.result.simulated) {
      return run.result;
    }
    return S.cal && S.cal.arm;
  }

  // No arm calibration yet: what one looks like, with SPOT's arm model defaults
  // (spot.arm.defaults: its placeholder friction, and the rotor and gear inertia), and the
  // position-steps test's goals (10, 0, -10, 20, 0, -20, 5, 0°, 2 s each) with a first-order
  // response (0.25 s) for the measured angle.
  function placeholderArm() {
    const joints = [1, 2, 3].map(index => ({
      index, name: JOINTS[index], static: 0.30, static_pos: 0.30, static_neg: 0.30,
      coulomb: 0.20, stribeck: 0.05, viscous: 0.10, inertia: 3.5e-3,
    }));
    const goals = [10, 0, -10, 20, 0, -20, 5, 0].map(d => d * Math.PI / 180);
    const rows = [];
    let q = 0;
    for (let i = 0; i <= 320; i++) {
      const t = i * 0.05;
      const g = goals[Math.min(goals.length - 1, Math.floor(t / 2))];
      q += (g - q) * (1 - Math.exp(-0.05 / 0.25));
      rows.push([t, g, q]);
    }
    return {
      placeholder: true, joints,
      plots: Object.fromEntries([1, 2, 3].map(j => [String(j), { steps: rows }])),
    };
  }

  // The arm results: each joint's friction and inertia, warnings, the friction and steps plots,
  // and where they came from.
  function armResults(res) {
    const joints = [].concat(res.joints || []);
    const plots = res.plots || {};
    const row = j => `<tr><td>${esc(JOINTS[j.index] || j.name)}</td><td>${fmt(j.static, 3)}</td>
        <td>${j.static_pos != null ? `+${fmt(j.static_pos, 3)} / −${fmt(j.static_neg, 3)}` : '—'}</td>
        <td>${fmt(j.coulomb, 3)}</td><td>${fmt(j.stribeck, 3)}</td><td>${fmt(j.viscous, 3)}</td>
        <td>${j.inertia != null ? j.inertia.toExponential(2) : '—'}</td></tr>`;
    const sj = joints.some(j => j.index === S.armStepsJoint)
      ? S.armStepsJoint
      : (joints[0] || {}).index;
    const tabs = joints.map(j =>
      `<button type="button" class="cal-chip${j.index === sj ? ' on' : ''}" ` +
      `data-armsteps="${j.index}">${JOINTS[j.index]}</button>`).join('');
    const label = res.placeholder
      ? '<span class="pill tip" title="No arm calibration yet' +
        `${S.local ? ' (the lab console measures it; Update from Lab brings it here)' : ''}: ` +
        'SPOT\'s arm model defaults (spot.arm.defaults), which simulations use until it is ' +
        'calibrated. The steps show the test\'s goals with an assumed response.">' +
        'Placeholder · Defaults</span>'
      : res.simulated ? '<span class="pill warn">Rehearsal · Not Saved</span>'
      : `<span class="pill ok">${when(res.updated)}</span>`;
    return `<section class="card"><div class="card-head"><h2>Arm</h2>${label}</div>
        <table class="cal-thr-table"><thead><tr><th>Joint</th><th title="Breakaway torque">Static [N m]</th><th>+ / − [N m]</th>
          <th title="Sliding friction">Coulomb [N m]</th><th title="How quickly friction falls from static to Coulomb with speed">Stribeck [rad/s]</th>
          <th>Viscous [N m s/rad]</th><th title="About the joint, link and rotor">Inertia [kg m²]</th></tr></thead>
          <tbody>${joints.map(row).join('')}</tbody></table>
        ${joints.some(j => j.warnings && j.warnings.length) ? `<ul class="cal-arm-warn">${joints.flatMap(j => (j.warnings || [])
          .map(w => `<li class="warn-text"><b>${esc(JOINTS[j.index] || j.name)}</b>: ${esc(w)}</li>`)).join('')}</ul>` : ''}
        <div class="cal-arm-plots">
          <div><h3 class="cal-sub">Friction against Speed</h3>${frictionPlot(joints, plots)}</div>
          <div><h3 class="cal-sub">Position Steps</h3><div class="cal-tabs-mini">${tabs}</div>${stepsPlot((plots[String(sj)] || {}).steps)}</div>
        </div>
        <p class="hint">${res.placeholder ? 'Not measured yet: SPOT\'s arm model defaults.' : res.simulated ? 'A rehearsal with the script\'s simulated servos: the lab\'s arm calibration is unchanged.'
          : `Measured ${when(res.updated)} by ${esc(res.by || '?')} (servos: ${esc(Object.values(res.servos || {}).join(', ') || '?')}, supply ${fmt(res.supply_voltage, 1)} V).
             Simulations use it with the Scenario's Calibrated Arm; the raw data stays on the lab console (${esc(res.lab_folder || '')}).`}</p>
        ${S.local || res.simulated || res.placeholder ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-arm" title="Remove the arm calibration from the lab console\'s file">Remove</button></div>'}
      </section>`;
  }

  // Checks the Chaser's setup.
  async function checkArm() {
    S.armChecking = true;
    if (S.shown && S.section === 'arm') {
      render();
    }
    try {
      S.armCheck = await api('/api/calibration/arm/check');
    } finally {
      S.armChecking = false;
      if (S.shown && S.section === 'arm') {
        render();
      }
    }
  }

  // Starts the arm calibration: a rehearsal at once, else after its dialog.
  function startArm() {
    const f = S.armForm;
    if (f.simulate) {
      return act(async () => onRun(await api('/api/calibration/arm/start', { ...f })));
    }
    const minutes =
      Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
    $('#calArmTime').textContent =
      `${f.joints.map(j => JOINTS[j]).join(', ')}: ${f.tests.join(', ')}; about ${minutes} min.`;
    $('#calArmAck').checked = false;
    $('#calArmGo').disabled = true;
    $('#calArmDialog').showModal();
  }
  $('#calArmAck').onchange = ev => {
    $('#calArmGo').disabled = !ev.target.checked;
  };
  $('#calArmGo').onclick = () => act(async () => {
    $('#calArmDialog').close();
    onRun(await api('/api/calibration/arm/start', { ...S.armForm, acknowledged: true }));
  });

  // A thruster run's map (the table seen from above: the platform, its burns, the centre),
  // redrawn when anything on it changes.
  function draw() {
    const canvas = $('#calMap');
    const r = S.run;
    if (!canvas || S.section !== 'thrusters' || !r || sectionOf(r.kind) !== 'thrusters') {
      return;
    }
    fitMap(canvas.parentElement);
    drawTable(canvas, {
      cal: S.cal && S.cal.table, paths: r.records || [],
      live: { pose: r.pose, path: r.path, goal: r.goal }, color: COLORS[r.platform],
    });
  }

  // The map as large as the window allows with its whole card in view: its height is what is
  // left below its top, less what the card has under it (the colour bar, the numbers); its width
  // follows the table's proportions. (A narrow window lets the page scroll: full width.)
  function fitMap(map) {
    const scroll = $('.cal-scroll');
    const card = map.closest('.cal-map-card');
    map.style.width = '';
    if (!scroll || !card || window.innerWidth <= 900) {
      return;
    }
    const m = map.getBoundingClientRect();
    const below = card.getBoundingClientRect().bottom - m.bottom;
    const top = m.top - scroll.getBoundingClientRect().top + scroll.scrollTop;
    const height = Math.max(240, scroll.clientHeight - top - below - 24);
    if (height < m.height) {
      map.style.width = `${Math.floor(height * 3.9 / 2.9)}px`;
    }
  }
  new ResizeObserver(() => draw()).observe(body);
  window.addEventListener('resize', () => draw());

  // The sync prompt (local console)

  // Offers the lab's calibration when it is newer, unless asked not to (always, or for that
  // one), once a session.
  function maybeAsk() {
    if (!S.local || !S.lab || !S.lab.reachable || !S.labCal || !S.labCal.updated || !S.cal) {
      return;
    }
    const mine = S.cal;
    const lab = S.labCal;
    if (mine.updated && lab.updated <= mine.updated) {
      return;
    }
    if (mine.sync && (mine.sync.ask === false || mine.sync.declined === lab.updated)) {
      return;
    }
    if (S.asked === lab.updated) {
      return;
    }
    S.asked = lab.updated;
    const dialog = $('#calSyncDialog');
    $('#calSyncList').innerHTML = newerSections(lab, mine)
      .map(([what, t]) => `<li><b>${what}</b><span>${when(t)}</span></li>`).join('');
    $('#calSyncNever').checked = false;
    dialog.showModal();
  }

  // The lab's sections newer than this project's: [name, time] (a thruster section's time
  // includes its decay's).
  function newerSections(lab, mine) {
    const time = c =>
      [c && c.updated, c && c.decay && c.decay.updated].filter(Boolean).sort().pop() || '';
    const out = [];
    const newer = (a, b) => a && (!b || time(a) > time(b));
    if (newer(lab.table, mine.table)) {
      out.push(['Table Slope', lab.table.updated]);
    }
    for (const n of NAMES) {
      const a = (lab.thrusters || {})[n];
      const b = (mine.thrusters || {})[n];
      if (newer(a, b)) {
        out.push([`${platformName(n)} Thrusters`, time(a)]);
      }
    }
    if (newer(lab.arm, mine.arm)) {
      out.push(['Arm', lab.arm.updated]);
    }
    return out;
  }

  // Update (and maybe never ask again), or Not Now (this one declined, or never ask again).
  $('#calSyncUpdate').onclick = () => act(async () => {
    const never = $('#calSyncNever').checked;
    $('#calSyncDialog').close();
    onCalibration(await api('/api/calibration/take', {}));
    if (never) {
      onCalibration(await api('/api/calibration/sync', { ask: false }));
    }
    toast('This project now has the lab\'s calibration; the next simulation uses it.', 'success',
      5000);
  });
  $('#calSyncLater').onclick = () => act(async () => {
    const never = $('#calSyncNever').checked;
    const lab = S.labCal;
    $('#calSyncDialog').close();
    onCalibration(await api('/api/calibration/sync',
      never ? { ask: false } : { declined: lab.updated }));
    toast(never
      ? 'The lab\'s calibration won\'t be offered again (Calibration tab: Ask When the Lab\'s Is ' +
        'Newer turns it back on).'
      : 'Kept this project\'s calibration. The Calibration tab can update it later.', 'info', 6000);
  });

  // Input

  // The section tabs (the arm's setup is checked the first time).
  document.addEventListener('click', ev => {
    const b = ev.target.closest('[data-calsec]');
    if (b) {
      S.section = b.dataset.calsec;
      storage.set('spot.calibration.section', S.section);
      render();
      fetchShownRuns();
      if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) {
        act(checkArm);
      }
    }
  });

  // The section's buttons.
  body.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (!b || b.disabled) {
      return;
    }
    if (b.dataset.calplat) {
      S.platform = b.dataset.calplat;
      storage.set('spot.calibration.platform', S.platform);
      render();
    } else if (b.dataset.calview) {
      S.platform = b.dataset.calview;
      storage.set('spot.calibration.platform', S.platform);
      render();
    } else if (b.dataset.thrstage) {
      S.thrStage = b.dataset.thrstage;
      storage.set('spot.calibration.thrStage', S.thrStage);
      render();
      fetchShownRuns();
    } else if (b.dataset.cal === 'start-thrusters') {
      readThrForm();
      askThrusters();
    } else if (b.dataset.cal === 'clear-thrusters') {
      if (!confirm(`Remove the ${platformName(S.platform)}'s thruster calibration from the lab ` +
          'console? Projects keep their own copies.')) {
        return;
      }
      onCalibration(await api('/api/calibration/clear',
        { section: 'thrusters', platform: S.platform }));
    } else if (b.dataset.armjoint || b.dataset.armtest) {
      // A joint or test chip: in or out (tests kept in their order).
      const f = S.armForm;
      const key = b.dataset.armjoint ? 'joints' : 'tests';
      const v = b.dataset.armjoint ? Number(b.dataset.armjoint) : b.dataset.armtest;
      f[key] = f[key].includes(v) ? f[key].filter(x => x !== v) : [...f[key], v];
      if (key === 'joints') {
        f.joints.sort();
      } else {
        f.tests = ARM_TESTS.map(t => t[0]).filter(t => f.tests.includes(t));
      }
      storage.set('spot.calibration.arm', f);
      render();
    } else if (b.dataset.armsteps) {
      S.armStepsJoint = Number(b.dataset.armsteps);
      render();
    } else if (b.dataset.cal === 'arm-check') {
      await checkArm();
    } else if (b.dataset.cal === 'start-arm') {
      startArm();
    } else if (b.dataset.cal === 'clear-arm') {
      if (!confirm('Remove the arm calibration from the lab console? ' +
          'Projects keep their own copies.')) {
        return;
      }
      onCalibration(await api('/api/calibration/clear', { section: 'arm' }));
    } else if (b.dataset.cal === 'start-table') {
      readForm();
      const r = await api('/api/calibration/table/start', { platform: S.platform, ...S.form });
      onRun(r);
    } else if (b.dataset.cal === 'details') {
      showDetails();
    } else if (b.dataset.cal === 'start-stream') {
      // At the PhaseSpace tab's frequency.
      await api('/api/stream/start', {});
      toast('PhaseSpace is streaming.', 'success', 3000);
    } else if (b.dataset.cal === 'load-run') {
      await loadRun(b.dataset.kind);
    } else if (b.dataset.cal === 'stop') {
      onRun(await api('/api/calibration/stop', {}));
    } else if (b.dataset.cal === 'clear-table') {
      if (!confirm('Remove the table calibration from the lab console? ' +
          'Projects keep their own copies.')) {
        return;
      }
      onCalibration(await api('/api/calibration/clear', { section: 'table' }));
    }
  }));

  // Load Run: fit the chosen saved run again (after confirming a replacement), and show the
  // platform loaded.
  async function loadRun(kind) {
    const runs = S.savedRuns[kind] || [];
    const r = runs.find(x => x.id === S.runPick[kind]) || runs[0];
    if (!r) {
      return;
    }
    if (kind === 'table') {
      readForm();
      if (S.cal && S.cal.table && !confirm('Replace the lab\'s table calibration with a fit of ' +
          `the ${platformName(r.platform)}'s run of ${r.started}?`)) {
        return;
      }
      onCalibration(await api('/api/calibration/table/load', { run: r.id, model: S.form.model }));
      toast(`The table calibration is now fitted to the run of ${r.started}.`, 'success', 5000);
    } else if (kind === 'decay') {
      const mine = ((S.cal && S.cal.thrusters || {})[r.platform] || {}).decay;
      if (r.stopped && !confirm(`The run of ${r.started} was stopped early (out of air, say): ` +
          'its decay may be the pressure falling, not the valves. Load it anyway?')) {
        return;
      }
      if (mine && !r.stopped && !confirm(`Replace the ${platformName(r.platform)}'s thrust decay ` +
          `with a fit of its Together run of ${r.started}?`)) {
        return;
      }
      const out = await api('/api/calibration/decay/load', { run: r.id });
      S.platform = r.platform;
      storage.set('spot.calibration.platform', S.platform);
      onCalibration(out);
      const d = (out.calibration.thrusters || {})[r.platform].decay;
      toast(`The ${platformName(r.platform)}'s thrust decay is now ${fmt(100 * d.slope, 1)} % ` +
        `per extra valve (its run of ${r.started}).`, 'success', 6000);
    } else {
      const mine = (S.cal && S.cal.thrusters || {})[r.platform];
      if (mine && !confirm(`Replace the ${platformName(r.platform)}'s thruster calibration with ` +
          `a fit of its run of ${r.started}?`)) {
        return;
      }
      const out = await api('/api/calibration/thrusters/load', { run: r.id });
      S.platform = r.platform;
      storage.set('spot.calibration.platform', S.platform);
      onCalibration(out);
      const res = (out.calibration.thrusters || {})[r.platform];
      const bad = res ? res.valid.filter(v => !v).length : 0;
      toast(`The ${platformName(r.platform)}'s thrusters are now fitted to its run of ` +
        `${r.started}${bad ? ` (${bad} keep the nominal thrust)` : ''}.`, 'success', 6000);
    }
    await fetchRuns(kind);
  }

  // The section's settings: the table's form, the saved run chosen, the thrusters' form, and
  // Rehearse.
  body.addEventListener('change', ev => {
    if (ev.target.dataset.calform) {
      readForm();
    }
    if (ev.target.matches('[data-calrun]')) {
      S.runPick[ev.target.dataset.calrun] = ev.target.value;
      render();
    }
    if (ev.target.dataset.thrform) {
      readThrForm();
    }
    if (ev.target.matches('[data-armsim]')) {
      S.armForm.simulate = ev.target.checked;
      storage.set('spot.calibration.arm', S.armForm);
      render();
    }
  });

  // The sync card: Update from Lab, and Ask When the Lab's Is Newer.
  head.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (b && b.dataset.cal === 'take' && !b.disabled) {
      onCalibration(await api('/api/calibration/take', {}));
      toast('This project now has the lab\'s calibration; the next simulation uses it.',
        'success', 5000);
    }
  }));
  head.addEventListener('change', ev => act(async () => {
    if (ev.target.dataset.cal === 'ask') {
      onCalibration(await api('/api/calibration/sync', { ask: ev.target.checked }));
    }
  }));

  // Esc on the tab (lab console) stops the run.
  document.addEventListener('keydown', ev => {
    if (ev.key === 'Escape' && S.shown && S.active && !S.local) {
      act(async () => onRun(await api('/api/calibration/stop', {})));
    }
  });

  // Reads the table's Run card settings, remembered.
  function readForm() {
    for (const el of body.querySelectorAll('[data-calform]')) {
      S.form[el.dataset.calform] = el.tagName === 'SELECT' ? el.value : Number(el.value);
    }
    storage.set('spot.calibration.table', S.form);
  }

  // From the server

  // This console's calibration: the 3D view's slope and the other pages follow it.
  function onCalibration(d) {
    if (!d || !d.calibration) {
      return;
    }
    S.cal = d.calibration;
    S.path = d.path;
    viewer?.setTableSlope(S.cal.table || null);
    document.dispatchEvent(new CustomEvent('spot-calibration', { detail: S.cal }));
    if (S.shown) {
      render();
    }
    maybeAsk();
  }

  // The lab console's run.
  let lastRender = 0;
  function onRun(d) {
    if (!d || !('active' in d)) {
      return;
    }
    const before = S.run;
    S.run = d.run;
    S.active = d.active;
    const isRunning = r => r && ['running', 'stopping'].includes(r.state);

    // A new table run: its own drifts only.
    if (S.run && S.run.kind === 'table' && isRunning(S.run) &&
        !(before && before.started === S.run.started && isRunning(before))) {
      viewer?.clearLivePaths();
    }

    // A run just ended: it saved its data, so its kind's saved runs change.
    if (!S.local && before && S.savedRuns[before.kind] !== undefined && isRunning(before) &&
        !isRunning(S.run)) {
      act(() => fetchRuns(before.kind));
    }

    // The page re-renders when the run's steps change (at least every second), and redraws the
    // map for the platform's moves.
    const restructure = !before || !S.run || before.state !== S.run.state ||
      before.step !== S.run.step || before.message !== S.run.message ||
      (before.records || []).length !== (S.run.records || []).length;
    if (!S.shown) {
      return;
    }
    const now = performance.now();
    if (restructure || now - lastRender > 1000) {
      lastRender = now;
      render();
    } else {
      draw();
    }
  }

  return {
    // The page's state: local console?, the lab's calibration (local) or the run and assumed
    // mass properties (lab), and this console's calibration.
    onState(st) {
      S.local = st.features && st.features.mode === 'local';
      if (S.local) {
        S.lab = st.lab || null;
        const snap = S.lab && S.lab.snapshot && S.lab.snapshot.calibration;
        S.labCal = snap ? snap.calibration : null;
      } else {
        S.assumed = st.calibration_assumed || null;
        onRun(st.calibration_run);
      }
      onCalibration(st.calibration);
    },
    onCalibration,
    onRun,
    // The lab link changed.
    onLab(l) {
      S.lab = l;
      if (l && !l.reachable) {
        S.labCal = null;
      } else if (l && l.snapshot && l.snapshot.calibration) {
        S.labCal = l.snapshot.calibration.calibration;
      }
      if (S.shown) {
        render();
      }
      maybeAsk();
    },
    // The lab's calibration changed.
    onLabCalibration(d) {
      S.labCal = d && d.calibration;
      if (S.shown) {
        render();
      }
      maybeAsk();
    },
    // PhaseSpace started or stopped (the Run cards' state).
    onStream() {
      if (S.shown && !S.local) {
        render();
      }
    },
    show() {
      S.shown = true;
      render();
      fetchShownRuns();
      if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) {
        act(checkArm);
      }
    },
    // The tab is left: the other pages' view shows the slope as their Table Slope switch has
    // it, and no drifts.
    hide() {
      S.shown = false;
      parkView();
      viewer?.setTableSlopeShown(Boolean(document.getElementById('toggleSlope')?.checked));
      viewer?.setLivePaths(false);
    },
    hasTable: () => Boolean(S.cal && S.cal.table),
  };
}
