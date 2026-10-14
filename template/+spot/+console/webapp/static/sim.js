// Simulation tab (local console, started from MATLAB by SPOT.m): the settings the Simulink
// diagrams need, simulations run in the user's MATLAB and replayed in the shared 3D view, and
// MATLAB's command window and workspace.
//
// The settings live in MATLAB (spot.worker.Worker keeps them in spot_settings.json). Every
// change is sent back whole ("settings.set"); MATLAB puts them into the base workspace and
// answers with what follows from them (total time, CG, LQR gains, ...) or why they don't work.

import { PLATFORM_KEYS as PLATFORMS, COLORS as SWATCH, platformName, swatch } from './platforms.js';
import { handleLockClick, isUnlocked, lockButton, onLockChange } from './ratelock.js';

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
// Target poses (there is no "drop" pose any more: the platforms start at their initial pose)
const POSE_KINDS = [['initial', 'Initial', 'Where the platform starts: the simulation starts here, and phase 2 drives the platform here'],
                    ['home', 'Home', 'Where the platform goes back to at the end (phases 4 and 5)']];
const POSE_LABELS = { initial: 'Initial', home: 'Home' };
const FIXED_SOLVERS = ['FixedStepAuto', 'FixedStepDiscrete', 'ode8', 'ode5', 'ode4', 'ode3', 'ode2', 'ode1', 'ode14x', 'ode1be'];
const VARIABLE_SOLVERS = ['VariableStepAuto', 'VariableStepDiscrete', 'ode45', 'ode23', 'ode113', 'ode15s', 'ode23s', 'ode23t', 'ode23tb', 'odeN', 'daessc'];
// LQR states and inputs. Derivatives are written d·/dt: a dot over a letter is hard to see.
const STATES = ['x', 'y', 'θ', 'dx/dt', 'dy/dt', 'dθ/dt'];
const INPUTS = ['Fx', 'Fy', 'Tz'];
const PHASE_COLORS = ['#5c7cfa', '#4c6ef5', '#4263eb', '#f59f00', '#f08c00', '#e8590c', '#d9480f', '#12b886', '#0ca678'];
// The timeline's segments, in order (names as in the diagrams' phase subsystems). Those with an
// "on" switch can be skipped (zero seconds, see spot.params.timing); phases 0, 1 and 5 always
// run, since they start and stop the platforms.
const SEGMENTS = [
  { title: 'Phase 0', name: 'Synchronization', dur: 'timing.phase0' },
  { title: 'Phase 1', name: 'Start Floating', dur: 'timing.phase1' },
  { title: 'Phase 2', name: 'Initial Position', dur: 'timing.phase2', on: 'timing.on.phase2' },
  ...[0, 1, 2, 3].map(k => ({ title: `3.${k + 1}`, name: `Sub-Phase ${k + 1}`, dur: `timing.subPhases.${k}`, on: `timing.on.subPhases.${k}`, sub: true })),
  { title: 'Phase 4', name: 'Return Home', dur: 'timing.phase4', on: 'timing.on.phase4' },
  { title: 'Phase 5', name: 'Hold Home', dur: 'timing.phase5' },
];
const LOCK = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 10 0v3h1a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1zm2 0h6V7a3 3 0 0 0-6 0z"/></svg>';

const getPath = (obj, path) => path.split('.').reduce((o, k) => (o == null ? o : o[k]), obj);
function setPath(obj, path, value) {
  const keys = path.split('.');
  const last = keys.pop();
  keys.reduce((o, k) => o[k], obj)[last] = value;
}
const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
const kindTag = k => ({ simulation: 'SIM', experiment: 'EXP' }[k] || 'MAT');
const esc = s => String(s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const fmt = (v, d = 4) => (v == null || !isFinite(v) ? '—' : Number(v).toPrecision(d).replace(/\.?0+(e|$)/, '$1'));

// ---------------------------------------------------------------- small form builders

function num(path, label, { unit = '', title = '', step = 'any' } = {}) {
  return `<label class="sfield"${title ? ` title="${esc(title)}"` : ''}>
    <span class="sfield-label">${label}</span>
    <span class="sfield-input${unit.length > 2 ? ' long-unit' : ''}"><input class="input" type="number" step="${step}" data-path="${path}">${unit ? `<span class="unit">${unit}</span>` : ''}</span>
  </label>`;
}
// The PhaseSpace data rate (dataRate): locked unless unlocked with its padlock (ratelock.js)
function rateField() {
  const title = 'The PhaseSpace data rate: how often the cameras send the platforms\' positions in an experiment, '
    + 'and the rate the LQR gains are designed at. The Experiment tab\'s Data Rate is the same setting. Locked: '
    + 'click the padlock to change it.';
  return `<div class="sfield" title="${esc(title)}">
    <span class="sfield-label lock-label"><span>Data Rate</span>${lockButton()}</span>
    <span class="sfield-input"><input class="input" type="number" min="1" max="960" step="1" data-path="dataRate" data-rate aria-label="Data rate"><span class="unit">Hz</span></span>
  </div>`;
}
const validRate = v => Number.isInteger(v) && v >= 1 && v <= 960;
// The thrusters' PWM frequency (pwmFrequency): locked like the data rate, since it must match the
// Teensy firmware's
function pwmField() {
  const title = 'The thrusters\' PWM frequency (PWMFreq): the duty cycles assume a valve period of 1/frequency. '
    + 'The Teensy firmware runs its PWM at 5 Hz (PWM_FREQUENCY in teensy_pwm_control.ino), so they must be equal. '
    + 'Locked: click the padlock to change it.';
  return `<div class="sfield" title="${esc(title)}">
    <span class="sfield-label lock-label"><span>Thruster PWM</span>${lockButton('pwm')}</span>
    <span class="sfield-input"><input class="input" type="number" min="0.1" max="100" step="any" data-path="pwmFrequency" data-pwm aria-label="Thruster PWM frequency"><span class="unit">Hz</span></span>
  </div>`;
}
const validPwm = v => isFinite(v) && v > 0 && v <= 100;
function text(path, label, title = '') {
  return `<label class="sfield"${title ? ` title="${esc(title)}"` : ''}>
    <span class="sfield-label">${label}</span>
    <span class="sfield-input"><input class="input mono" data-path="${path}" data-type="text" spellcheck="false"></span>
  </label>`;
}
function cells(path, n) {
  return Array.from({ length: n }, (_, k) => `<td><input class="input" type="number" step="any" data-path="${path}.${k}"></td>`).join('');
}

export function initSimulation({ api, act, toast, storage, viewer, store, messages, livePlot }) {
  const S = {
    settings: null, derived: null, error: '', diagrams: [], file: '',
    matlab: { connected: false }, sim: {}, progress: null, result: null, resultId: 0,
    tab: storage.get('spot.sim.tab', 'scenario'),
    platform: storage.get('spot.sim.platform', 'RED'),
    // Poses shown on the table (the view's toolbar, on this and the Experiment tab): off, initial or home
    pose: ['off', 'initial', 'home'].includes(storage.get('spot.pose', 'initial')) ? storage.get('spot.pose', 'initial') : 'initial',
    t: 0, playing: false, lastTick: 0, shown: false,
    editSeq: 0, pushTimer: null, pushing: false,
    history: storage.get('spot.cmdHistory', []), histPos: -1,
    workspace: [], inspected: null,
  };
  const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
  const panes = $('#simPanes');

  // ---------------------------------------------------------------- settings from MATLAB

  function applyReply(r, seq) {
    if (!r || r.cancelled) return;
    // A reply to an older version of the settings: keep what the user typed since
    if (seq === undefined || seq === S.editSeq) S.settings = r.settings;
    S.derived = r.derived;
    S.error = r.error || '';
    S.diagrams = r.diagrams || [];
    S.file = r.file || S.file;
    if (!S.panesBuilt) renderPanes();
    fill();
    renderSetup();
    renderRun();
    if (seq !== undefined && seq !== S.editSeq) schedulePush();
    refreshWorkspaceSoon();
    // (the Designer follows the arm: S.settings, with any edit not sent yet)
    document.dispatchEvent(new CustomEvent('spot-settings', { detail: S.settings }));
  }

  onLockChange(() => fill());

  // The project's calibration changed (the Calibration tab's Update from Lab): MATLAB applies the
  // settings again, so the Scenario tab says what the next simulation uses
  let calUpdated;
  document.addEventListener('spot-calibration', ev => {
    const u = ev.detail && ev.detail.updated;
    if (calUpdated !== undefined && u !== calUpdated && S.settings && S.matlab && S.matlab.connected) {
      loadSettings().catch(() => {});
    }
    calUpdated = u;
  });

  async function loadSettings() {
    try {
      applyReply(await matlab('settings.get'));
      S.authError = '';
    } catch (e) {
      S.authError = e.message;   // e.g. opened without the session token
      renderRun();
      throw e;
    }
  }

  function edited(rebuild = false) {
    S.editSeq++;
    if (rebuild) renderPanes();
    fill();
    renderSetup();
    schedulePush();
  }

  function schedulePush(delay = 300) {
    clearTimeout(S.pushTimer);
    S.pushTimer = setTimeout(push, delay);
  }

  async function push() {
    if (S.pushing || S.sim.running) return schedulePush(600);
    S.pushing = true;
    const seq = S.editSeq;
    try {
      applyReply(await matlab('settings.set', { settings: S.settings }), seq);
      S.pushedSeq = seq;
    } catch (e) {
      toast(e.message, 'error', 8000);
    } finally {
      S.pushing = false;
    }
  }

  // ---------------------------------------------------------------- panes

  function renderPanes() {
    if (!S.settings) return;
    S.panesBuilt = true;
    const s = S.settings;
    const on = s.platforms;
    panes.innerHTML = `
      <div data-simpane="scenario">${scenarioPane(s)}</div>
      <div data-simpane="platforms">${platformsPane(s, on)}</div>
      <div data-simpane="control">${controlPane(on)}</div>
      <div data-simpane="solver">${solverPane(s)}</div>`;
    showTab(S.tab);
  }

  function scenarioPane(s) {
    const options = (S.diagrams.length ? S.diagrams : [s.diagram]).filter(Boolean)
      .map(d => `<option value="${esc(d)}">${esc(d)}</option>`).join('') || '<option value="">No .slx in the Project Folder</option>';
    const chips = ['RED', 'BLACK', 'BLUE', 'ARM'].map(n =>
      `<button type="button" class="plat-chip" data-toggle="platforms.${n}" style="--c:${SWATCH[n]}">${platformName(n)}</button>`).join('');
    return `
      <section class="card">
        <div class="card-head"><h2>Model</h2>
          <button type="button" class="btn btn-sm" data-action="openDiagram" title="Open the diagram in Simulink (in MATLAB)"><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" d="M14 4h6v6M20 4l-9 9M18 14v5a1 1 0 0 1-1 1H5a1 1 0 0 1-1-1V7a1 1 0 0 1 1-1h5"/></svg><span>Open in Simulink</span></button></div>
        <label class="field-label">Diagram</label>
        <select class="input" data-path="diagram" data-type="text">${options}</select>
        <label class="field-label sp-top">Platforms</label>
        <div class="plat-chips">${chips}</div>
        <div class="fgrid cols-3 sp-top">
          ${num('sampleRate', 'Sample Rate', { unit: 'Hz', title: 'Controller rate: the diagram runs every 1/rate s (baseRate, its fixed step).' })}
          ${rateField()}
          ${pwmField()}
        </div>
        <div class="sp-top cal-scenario">
          <label class="toggle" title="Use each thruster's measured thrust (the lab console's thruster calibration, Calibration tab; this project's resources/spot_calibration.json) in the mixer and so in the simulated plant, as the experiments' controller does. Off: the Thrust on the Platforms tab for all eight."><input type="checkbox" data-path="calibration.thrusters"><span></span>Calibrated Thrust</label>
          <span class="hint" data-derived="calthrusters"></span>
        </div>
        <div class="cal-scenario">
          <label class="toggle" title="Use the arm's measured friction and inertia (the lab console's arm calibration, Calibration tab, or a resources/arm_calibration.json copied by hand: the newer) in the simulated arm. Off: the arm model's defaults."><input type="checkbox" data-path="calibration.arm"><span></span>Calibrated Arm</label>
          <span class="hint" data-derived="calarm"></span>
        </div>
        <div class="cal-scenario">
          <label class="toggle" title="Simulate the table's measured slope: the floating platforms slide downhill, and their air bearings drag them, as the lab console's table slope calibration found (Calibration tab; this project's resources/spot_calibration.json). Off: a level table."><input type="checkbox" data-path="calibration.tableSlope"><span></span>Table Slope</label>
          <span class="hint" data-derived="calslope"></span>
        </div>
      </section>
      <section class="card">
        <div class="card-head"><h2>Timeline</h2><span class="pill" data-derived="tsim"></span></div>
        <div class="timeline-bar" data-derived="timeline"></div>
        <div class="tl-rows">${timelineRows()}</div>
        <p class="hint">Switch segments off to skip them, in simulation and in experiments (build again after a change).
          ${LOCK.replace('<svg', '<svg class="tl-lock-inline"')} Phases 0, 1 and 5 always run: they start and stop the platforms.</p>
      </section>`;
  }

  function timelineRows() {
    const row = (g, k) => `<div class="tl-row${g.sub ? ' sub' : ''}" data-seg="${k}">
        ${g.on ? `<label class="toggle tl-switch" title="Run ${g.title}: switch off to skip it"><input type="checkbox" data-path="${g.on}"><span></span></label>`
               : `<span class="tl-lock" title="${g.title} always runs">${LOCK}</span>`}
        <i class="tl-swatch" style="background:${PHASE_COLORS[k]}"></i>
        <span class="tl-name" title="${g.title}: ${g.name}"><b>${g.title}</b><span>${g.name}</span></span>
        <span class="tl-when" data-derived="when.${k}"></span>
        <span class="sfield-input tl-dur"><input class="input" type="number" step="any" min="0" data-path="${g.dur}" aria-label="${g.title} duration"><span class="unit">s</span></span>
      </div>`;
    const head = `<div class="tl-row tl-head"><span></span><i class="tl-swatch" style="background:linear-gradient(90deg, ${PHASE_COLORS.slice(3, 7).join(', ')})"></i>
        <span class="tl-name"><b>Phase 3</b><span>Experiment</span></span><span class="tl-when" data-derived="when.p3"></span>
        <span class="tl-sum" data-derived="p3total"></span></div>`;
    return SEGMENTS.map((g, k) => (k === 3 ? head : '') + row(g, k)).join('');
  }

  // Start and end [s] of each segment as it will run (skipped segments take no time)
  // The segments as MATLAB computed them (spot.params.timing: when each runs, skipped ones
  // taking no time); the switches and typed durations are shown as set, at once
  function timeline(s) {
    const derived = list(((S.derived || {}).timing || {}).segments);
    return SEGMENTS.map((g, k) => {
      const m = derived[k] || {};
      const d = Math.max(Number(getPath(s, g.dur)) || 0, 0);
      const run = !g.on || getPath(s, g.on) !== false;
      return { ...g, d, run, start: m.start, end: m.end };   // g.on stays the switch's settings path
    });
  }

  function platformsPane(s, on) {
    const picker = ['RED', 'BLACK', 'BLUE', 'ARM'].map(n =>
      `<button type="button" data-pick="${n}" class="${S.platform === n ? 'active' : ''}${on[n] ? '' : ' off'}">${swatch(n)}${platformName(n)}</button>`).join('');
    const n = S.platform;
    const off = on[n] ? '' : `<p class="hint warn-text">The ${platformName(n)} is not in this scenario; turn it on under Scenario to use it.</p>`;
    return `<div class="segmented plat-picker">${picker}</div>${off}` + (n === 'ARM' ? armCards() : platformCards(n));
  }

  function platformCards(n) {
    const rows = POSE_KINDS.map(([k, label, title]) =>
      `<tr data-poserow="${k}" title="${esc(title)}"><th><button type="button" class="pose-row-btn" data-setpose="${k}" title="Show the ${label.toLowerCase()} poses on the table">${label}</button></th>${cells(`initial.${n}.${k}`, 3)}</tr>`).join('');
    const sc = ['A · left middle edge', 'B · right back corner', 'C · right front corner'];
    return `
      <section class="card">
        <div class="card-head"><h2>Initial Conditions</h2></div>
        <table class="ic-table"><thead><tr><th></th><th>x [m]</th><th>y [m]</th><th>θ [°]</th></tr></thead><tbody>${rows}</tbody></table>
        <p class="hint" data-derived="posehint"></p>
      </section>
      <section class="card">
        <div class="card-head"><h2>Mass Properties</h2>
          <label class="toggle" title="Use the mass and inertia typed below instead of the measured ones (all platforms)"><input type="checkbox" data-path="mass.override"><span></span>Override</label></div>
        <span class="field-label tip" title="Scale readings with the platform on three scales (resources/COMConfig.png)">Scales</span>
        <div class="fgrid cols-3">${[0, 1, 2].map(k => num(`mass.${n}.scales.${k}`, 'ABC'[k], { unit: 'kg', title: sc[k] })).join('')}</div>
        <p class="derived-row" data-derived="cg"></p>
        <span class="field-label tip sp-top" title="Bifilar pendulum: period T, string length L, string spacing D">Bifilar Pendulum</span>
        <div class="fgrid cols-3">${num(`mass.${n}.bifilar.T`, 'T', { unit: 's' })}${num(`mass.${n}.bifilar.L`, 'L', { unit: 'm' })}${num(`mass.${n}.bifilar.D`, 'D', { unit: 'm' })}</div>
        <p class="derived-row" data-derived="inertia"></p>
        <div class="fgrid cols-3 sp-top override-fields">
          ${num(`mass.${n}.mass`, 'Mass', { unit: 'kg', title: 'Used when Override is on' })}
          ${num(`mass.${n}.inertia`, 'Inertia', { unit: 'kg·m²', title: 'Used when Override is on' })}
          ${num(`thrusterForce.${n}`, 'Thrust', { unit: 'N', title: 'Force of one thruster (all eight; the thruster calibration replaces it when used)' })}
        </div>
        <p class="hint" data-derived="calthrust"></p>
        <details class="advanced"><summary>Thruster Offsets and LED Positions</summary>
          <div data-derived="offsets"></div>
          <button type="button" class="btn btn-sm sp-top" data-action="writeTracker">Write PhaseSpace Marker File</button>
          <p class="hint">Writes resources/tracker_positions_phasespace.txt (LED positions of all three platforms from their CG), which the PhaseSpace switch uploads to the lab console.</p>
        </details>
      </section>`;
  }

  function mountText() {
    const m = S.settings && S.settings.mass && S.settings.mass.arm && S.settings.mass.arm.mount;
    if (!m) return '';
    const face = { '+x': '+X', '+y': '+Y', '-y': '−Y' }[m.face] || m.face;
    return ` at ${face} Face · ${m.side === 'left' ? 'Left' : 'Right'} (choose its place in the Designer)`;
  }

  function armCards() {
    const g = [['Phi', 'Φ', 'rad'], ['b0', 'b₀', 'm'], ['a1', 'a₁', 'm'], ['b1', 'b₁', 'm'], ['a2', 'a₂', 'm'],
               ['b2', 'b₂', 'm'], ['a3', 'a₃', 'm'], ['b3', 'b₃', 'm']];
    const links = [['mPROX', 'Proximal', 'kg'], ['mDIST', 'Distal', 'kg'], ['mEE', 'End Effector', 'kg'],
                   ['IPROX', 'Proximal', 'kg·m²'], ['IDIST', 'Distal', 'kg·m²'], ['IEE', 'End Effector', 'kg·m²']];
    return `
      <section class="card">
        <div class="card-head"><h2>Initial Pose</h2></div>
        <div class="fgrid cols-3">${['Shoulder q₁', 'Elbow q₂', 'Wrist q₃'].map((l, k) => num(`initial.ARM.initial.${k}`, l, { unit: '°' })).join('')}</div>
        <p class="hint">Mounted on the Chaser${mountText()}; shown on the table with the Chaser's initial pose (Initial Conditions: Initial, above the view). Each angle is 0 straight out (the shoulder: out from its face), ±90° to either side.</p>
      </section>
      <section class="card">
        <div class="card-head"><h2>Arm Geometry</h2></div>
        <div class="fgrid cols-4">${g.map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
        <span class="field-label sp-top">Link Masses</span>
        <div class="fgrid cols-3">${links.slice(0, 3).map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
        <span class="field-label sp-top">Link Inertias</span>
        <div class="fgrid cols-3">${links.slice(3).map(([k, l, u]) => num(`mass.arm.${k}`, l, { unit: u })).join('')}</div>
        <p class="hint">Definitions: resources/ArmConfig.png.</p>
      </section>`;
  }

  // LQR (the only platform controller): state weights Q, control weights R, and the gain K
  // MATLAB computes from them (STATES, INPUTS at the top)
  // One compact table per platform: a row per axis (x, y, θ) with its three weights, the error
  // (Q), its rate (Q) and the effort on that axis (R). Each box is tagged with what it weighs.
  function controlPane(on) {
    const names = PLATFORMS.filter(n => on[n]);
    if (!names.length && !on.ARM) return '<p class="hint">Turn on a platform under Scenario to set its controller.</p>';
    return (names.length ? lqrCards(names) : '') + (on.ARM ? armCard() : '');
  }

  // The arm's two default controllers (the servos' own position and velocity laws, run by the
  // diagram; spot.arm.PositionControl / VelocityControl): per-joint gains in the servos' units, and
  // the profile and PWM limit (control.ARM in the settings, armControl in MATLAB)
  function armCard() {
    const JOINTS = ['Shoulder', 'Elbow', 'Wrist'];
    const gain = (path, title) => `<td><span class="lqr-in" title="${esc(title)}"><input class="input" type="number" step="1" min="0" max="16383"
        data-min="0" data-max="16383" data-path="${path}" aria-label="${esc(title)}"></span></td>`;
    const rows = (mode, gains) => JOINTS.map((j, i) => `<tr><th scope="row" class="arm-joint">${j}</th>
        ${gains.map(g => gain(`control.ARM.${mode}.${g}.${i}`, `${j}: ${mode} ${g} gain`)).join('')}</tr>`).join('');
    const limit = (path, label, max, unit, title) => `<label class="sfield" title="${esc(title)}">
        <span class="sfield-label">${label}</span>
        <span class="sfield-input${unit.length > 2 ? ' long-unit' : ''}"><input class="input" type="number" step="1" min="0" max="${max}"
          data-min="0" data-max="${max}" data-path="${path}">${unit ? `<span class="unit">${unit}</span>` : ''}</span></label>`;
    const head = cols => `<thead><tr><th></th>${cols.map(c => `<th>${c}</th>`).join('')}</tr></thead>`;
    return `
      <section class="card lqr-card arm-card">
        <div class="card-head"><h2 class="hw-name">${swatch('ARM')}${platformName('ARM')}</h2><span class="pill">Position · Mode 1</span></div>
        <p class="hint">The servo's position law, run every step: a PID on the joint angle error, giving the torque the servo
          would give. Gains in the servo's units (P/128, I/65536, D/16 per the Dynamixel manual).</p>
        <table class="lqr-table arm-table">${head(['P', 'I', 'D'])}<tbody>${rows('position', ['P', 'I', 'D'])}</tbody></table>
        <div class="fgrid cols-3 sp-top">
          ${limit('control.ARM.position.profileVelocity', 'Profile Velocity', 32767, '', 'How fast the desired angle moves to the goal, in units of 0.229 rev/min (20 = 0.48 rad/s; 0 = at once)')}
          ${limit('control.ARM.position.profileAcceleration', 'Profile Accel.', 32767, '', 'The profile\'s acceleration, in units of 214.577 rev/min² (0 = none)')}
          ${limit('control.ARM.position.pwmLimit', 'PWM Limit', 885, '', 'The most the controller asks of the motor, out of 885 (= the full supply voltage)')}
        </div>
      </section>
      <section class="card lqr-card arm-card">
        <div class="card-head"><h2 class="hw-name">${swatch('ARM')}${platformName('ARM')}</h2><span class="pill">Velocity · Mode 3</span></div>
        <p class="hint">The servo's velocity law: a PI on the joint speed error (P/128, I/65536).</p>
        <table class="lqr-table arm-table">${head(['P', 'I'])}<tbody>${rows('velocity', ['P', 'I'])}</tbody></table>
        <div class="fgrid cols-3 sp-top">
          ${limit('control.ARM.velocity.profileAcceleration', 'Profile Accel.', 32767, '', 'How fast the desired speed moves to the goal, in units of 214.577 rev/min² (0 = at once)')}
          ${limit('control.ARM.velocity.pwmLimit', 'PWM Limit', 885, '', 'The most the controller asks of the motor, out of 885 (= the full supply voltage)')}
        </div>
        <p class="hint">Which controller runs is set by each phase (<span class="mono">ARM_Control_Law_Enabler</span> in
          1 - Experiment Logic: 1 position, 3 velocity, 2 your torque controller, 0 off). Defaults are tuned for 20 Hz.</p>
      </section>`;
  }

  function lqrCards(names) {
    const box = (path, tag, title) => `<span class="lqr-in" title="${esc(title)}">
        <input class="input" type="number" step="any" min="0" data-path="${path}" aria-label="${esc(title)}"><span class="lqr-tag">${tag}</span></span>`;
    const rows = n => [0, 1, 2].map(i => `<tr><th scope="row">${STATES[i]}</th>
        <td>${box(`control.${n}.lqr.Q.${i}`, STATES[i], `Q: weight on the ${STATES[i]} error`)}</td>
        <td>${box(`control.${n}.lqr.Q.${i + 3}`, STATES[i + 3], `Q: weight on the ${STATES[i + 3]} error`)}</td>
        <td>${box(`control.${n}.lqr.R.${i}`, INPUTS[i], `R: weight on ${INPUTS[i]}`)}</td></tr>`).join('');
    return `<p class="hint lqr-help">Larger <b>Q</b> corrects that error harder; larger <b>R</b> uses the thrusters more gently.</p>` +
      names.map(n => `
      <section class="card lqr-card">
        <div class="card-head"><h2 class="hw-name">${swatch(n)}${platformName(n)}</h2><span class="pill">LQR</span></div>
        <table class="lqr-table">
          <thead><tr><th></th>
            <th title="Q: weights on the position and attitude errors">Error <i>Q</i></th>
            <th title="Q: weights on the velocity and angular-rate errors">Rate <i>Q</i></th>
            <th title="R: weights on the forces and torque">Effort <i>R</i></th></tr></thead>
          <tbody>${rows(n)}</tbody>
        </table>
        <details class="advanced"><summary>Gain K (Computed)</summary><div data-derived="K.${n}"></div></details>
      </section>`).join('');
  }

  function solverPane(s) {
    const variable = s.solver.type === 'Variable-step';
    const list = variable ? VARIABLE_SOLVERS : FIXED_SOLVERS;
    const path = variable ? 'solver.variableStepName' : 'solver.fixedStepName';
    return `
      <section class="card">
        <div class="card-head"><h2>Solver</h2></div>
        <div class="segmented" data-solvertype>
          <button type="button" data-set="solver.type" data-value="Fixed-step">Fixed-Step</button>
          <button type="button" data-set="solver.type" data-value="Variable-step">Variable-Step</button>
        </div>
        <label class="field-label sp-top">Solver</label>
        <select class="input" data-path="${path}" data-type="text">${list.map(v => `<option>${v}</option>`).join('')}</select>
        ${variable ? `<div class="fgrid cols-2 sp-top">
          ${text('solver.maxStep', 'Max Step')}${text('solver.minStep', 'Min Step')}
          ${text('solver.initialStep', 'Initial Step')}${text('solver.relTol', 'Relative Tolerance')}
          ${text('solver.absTol', 'Absolute Tolerance')}</div>`
          : '<p class="hint">Fixed step = baseRate (1 / sample rate, under Scenario).</p>'}
        <p class="hint">Read from the diagram when you choose it, and applied to each simulation without changing the diagram file.</p>
      </section>
      <section class="card">
        <div class="card-head"><h2>Workspace</h2></div>
        <p class="hint" data-derived="variables"></p>
        <p class="hint">Settings File: <span class="mono" data-derived="file"></span></p>
      </section>`;
  }

  function showTab(tab) {
    S.tab = tab;
    storage.set('spot.sim.tab', tab);
    for (const b of $$('[data-simtab]')) b.classList.toggle('active', b.dataset.simtab === tab);
    for (const p of $$('[data-simpane]', panes)) p.hidden = p.dataset.simpane !== tab;
    // The workspace has a pane of its own, the full height of the sidebar
    panes.hidden = tab === 'workspace';
    $('#wsPane').hidden = tab !== 'workspace';
    if (tab === 'workspace') refreshWorkspace();
  }

  // ---------------------------------------------------------------- filling in values

  function fill() {
    const s = S.settings;
    if (!s) return;
    messages?.setSelected(s.platforms);
    $('#simRealTime').checked = Boolean(s.realTime);
    $('#simRealTimeHint').textContent = s.realTime && S.derived ? `· takes about ${fmt(S.derived.tsim, 4)} s` : '';
    for (const el of $$('[data-path]', panes)) {
      if (el === document.activeElement) continue;
      const v = getPath(s, el.dataset.path);
      if (el.type === 'checkbox') el.checked = Boolean(v);
      else el.value = v == null ? '' : v;
    }
    for (const b of $$('[data-toggle]', panes)) b.classList.toggle('on', Boolean(getPath(s, b.dataset.toggle)));
    for (const b of $$('[data-set]', panes)) b.classList.toggle('active', getPath(s, b.dataset.set) === b.dataset.value);
    for (const r of $$('[data-poserow]', panes)) r.classList.toggle('shown', r.dataset.poserow === S.pose);
    for (const el of $$('[data-rate]', panes)) el.disabled = !isUnlocked('rate');
    for (const el of $$('[data-pwm]', panes)) el.disabled = !isUnlocked('pwm');
    for (const el of $$('.override-fields input', panes)) {
      el.disabled = !s.mass.override && !el.dataset.path.startsWith('thrusterForce');
    }
    renderDerived();
  }

  function renderDerived() {
    const s = S.settings, d = S.derived || {};
    const set = (key, html) => { for (const el of $$(`[data-derived="${key}"]`, panes)) el.innerHTML = html; };
    // Timeline: the bar shows every segment at its typed length (skipped ones hatched, so they
    // can be clicked back on); the times are as it will run
    const segs = timeline(s);
    const tm = d.timing || {};
    const skipped = segs.filter(g => !g.run);
    set('tsim', `${fmt(d.tsim, 6)} s Total${skipped.length ? ` · ${skipped.length} Skipped` : ''}`);
    const typed = segs.reduce((a, g) => a + g.d, 0) || 1;
    set('timeline', segs.map((g, k) => {
      const short = g.title.replace('Phase ', '');
      const label = g.d / typed > 0.06 ? short : '';
      const tip = `${g.title} · ${g.name}: ${g.run ? `${fmt(g.start, 5)}–${fmt(g.end, 5)} s` : `skipped (${fmt(g.d, 5)} s)`}`;
      if (!g.on) {   // always runs
        return g.d ? `<span class="tl-seg locked on" style="flex:${g.d};--c:${PHASE_COLORS[k]}" title="${esc(tip)} (always runs)">${label}</span>` : '';
      }
      return `<button type="button" class="tl-seg${g.run ? ' on' : ''}" data-toggle="${g.on}" style="flex:${Math.max(g.d, typed * 0.012)};--c:${PHASE_COLORS[k]}" title="${esc(tip)}: click to ${g.run ? 'skip' : 'run'} it">${label}</button>`;
    }).join(''));
    segs.forEach((g, k) => {
      set(`when.${k}`, g.run ? `${fmt(g.start, 5)}–${fmt(g.end, 5)} s` : 'skipped');
      const row = $(`[data-seg="${k}"]`, panes);
      if (row) row.classList.toggle('off', !g.run);
    });
    set('when.p3', tm.Phase3_Duration > 0 ? `${fmt(tm.Phase2_End, 5)}–${fmt(tm.Phase3_End, 5)} s` : 'Skipped');
    set('p3total', `${fmt(tm.Phase3_Duration, 5)} s`);
    // The table slope calibration this project has (and whether the simulation uses it)
    const cal = d.calibration || {}, t = cal.table && !Array.isArray(cal.table) ? cal.table : null;   // (MATLAB's [] for none)
    set('calslope', cal.error ? `<span class="bad">${esc(cal.error)}</span>`
      : !t ? 'Level: this project has no table calibration (Calibration tab: Update from Lab).'
      : `${s.calibration && s.calibration.tableSlope ? 'Used' : 'Not used'}: ${fmt(t.tilt_centre_mrad, 3)} mrad at the centre, measured ${esc(String(t.updated).slice(0, 10))}.`);
    // The arm calibration the simulation uses (the file it came from), or the one the project has
    const armFrom = String(cal.armUsed || ''), armCal = cal.arm && !Array.isArray(cal.arm) ? cal.arm : null;
    set('calarm', armFrom ? `Used: ${esc(armFrom.split(/[\\/]/).pop())}${armCal && armFrom.includes('spot_calibration') ? `, measured ${esc(String(armCal.created).slice(0, 10))}` : ''}.`
      : armCal ? `Not used (measured ${esc(String(armCal.created).slice(0, 10))}).`
      : s.calibration && s.calibration.arm ? 'Defaults: this project has no arm calibration.' : 'Not used: defaults.');
    // The thruster calibrations this project has (and which the simulation uses)
    const th = cal.thrusters && !Array.isArray(cal.thrusters) ? cal.thrusters : {};
    const have = ['RED', 'BLACK', 'BLUE'].filter(k => th[k] && !Array.isArray(th[k]));
    const using = s.calibration && s.calibration.thrusters;
    set('calthrusters', !have.length ? 'Nominal: this project has no thruster calibration.'
      : `${using ? 'Used' : 'Not used'} for the ${have.map(k => platformName(k)).join(', ')}.`);
    const mine = th[S.platform] && !Array.isArray(th[S.platform]) ? th[S.platform] : null, F = mine ? [].concat(mine.thrust) : null;
    set('calthrust', !mine ? '' : `${using ? 'Calibrated thrust used' : 'Calibrated thrust (not used)'}: ${Math.min(...F).toFixed(3)}–${Math.max(...F).toFixed(3)} N, measured ${esc(String(mine.updated).slice(0, 10))} (Calibration tab).`);
    set('posehint', S.pose === 'off'
      ? 'Choose Initial Conditions: Initial or Home above the view (or click Initial or Home here) to see them on the table and drag them.'
      : `The ${S.pose} poses are on the table: drag ${S.platform} to move its pose; Shift+drag rotates it.`);
    const n = S.platform;
    const mp = d.mass && d.mass[n];
    if (mp) {
      set('cg', `Mass <b>${fmt(mp.measuredMass)} kg</b> · CG <b>${fmt(mp.cg[0] * 100, 3)}, ${fmt(mp.cg[1] * 100, 3)} cm</b> from the body centre`);
      set('inertia', `Inertia <b>${fmt(mp.measuredInertia)} kg·m²</b>` +
        (s.mass.override ? ` · <span class="warn-text">overridden: ${fmt(mp.mass)} kg, ${fmt(mp.inertia)} kg·m²</span>` : ''));
      const thrRow = ks => `<tr>${ks.map(k => `<th>T${k}</th>`).join('')}</tr><tr>${ks.map(k => `<td>${fmt(mp.thrusters[k - 1], 4)}</td>`).join('')}</tr>`;
      const corners = ['+x +y', '+x −y', '−x −y', '−x +y'];
      const leds = mp.leds.map((p, k) => `<tr><th>${corners[k]}</th><td>${fmt(p[0] * 1000, 4)}</td><td>${fmt(p[1] * 1000, 4)}</td></tr>`).join('');
      set('offsets', `<span class="field-label">Thruster Lines from the CG (mm)</span>
        <table class="gain-table thr-table"><tbody>${thrRow([1, 2, 3, 4])}${thrRow([5, 6, 7, 8])}</tbody></table>
        <span class="field-label sp-top">LEDs from the CG (mm)</span>
        <table class="gain-table"><thead><tr><th>Corner</th><th>x</th><th>y</th></tr></thead><tbody>${leds}</tbody></table>`);
    } else {
      set('cg', ''); set('inertia', ''); set('offsets', '');
    }
    for (const p of PLATFORMS) {
      const K = d.lqr && d.lqr[p];
      set(`K.${p}`, K ? `<table class="gain-table k-table"><thead><tr><th></th>${STATES.map(h => `<th>${h}</th>`).join('')}</tr></thead>
          <tbody>${K.map((row, r) => `<tr><th>${INPUTS[r] || ''}</th>${row.map(v => `<td>${fmt(Math.abs(v) < 1e-9 ? 0 : v, 3)}</td>`).join('')}</tr>`).join('')}</tbody></table>
          <p class="hint">Rows: the forces and torque; columns: the state errors they respond to.</p>`
                      : '<p class="hint">Not calculated (see the warnings above).</p>');
    }
    set('variables', d.variables ? `${d.variables.length} variables are put into MATLAB's base workspace whenever a setting changes; SPOT_User_Parameters.m then runs, for your own.` : '');
    set('file', esc(S.file || ''));
    const notice = $('#simNotice');
    const warnings = [S.error, ...((d && d.warnings) || [])].filter(Boolean);
    notice.hidden = !warnings.length;
    notice.textContent = warnings.join(' ');
  }

  // ---------------------------------------------------------------- the table (setup poses)

  // The poses chosen in the view's toolbar (also shown, not draggable, on the Experiment tab)
  function renderSetup() {
    for (const b of $$('[data-pose]')) b.classList.toggle('active', b.dataset.pose === S.pose);
    if (!S.settings) return;
    const s = S.settings;
    const kind = S.pose === 'home' ? 'home' : 'initial';
    const poses = {};
    for (const n of PLATFORMS) if (s.platforms[n]) poses[n] = s.initial[n][kind];
    const arm = s.platforms.ARM && kind === 'initial' && S.derived && S.derived.armInitial ? S.derived.armInitial : null;
    viewer?.setSetup({ poses, kind: POSE_LABELS[kind], arm, editable: S.shown && S.pose !== 'off' });
  }

  function setPose(kind) {
    S.pose = kind;
    storage.set('spot.pose', kind);
    fill();
    renderSetup();
    if (S.shown) layers();
    document.dispatchEvent(new CustomEvent('spot-pose', { detail: kind }));
  }
  for (const b of $$('[data-pose]')) b.onclick = () => setPose(b.dataset.pose);

  if (viewer) {
    viewer.setup.onDrag = (name, x, y, deg, done) => {
      if (!S.settings || !S.settings.initial[name]) return;
      const r = (v, k) => Math.round(v * 10 ** k) / 10 ** k;
      const th = ((deg % 360) + 360) % 360;
      S.settings.initial[name][S.pose === 'home' ? 'home' : 'initial'] = [r(x, 3), r(y, 3), r(th > 180 ? th - 360 : th, 1)];
      if (S.platform !== name && PLATFORMS.includes(name) && done) {
        S.platform = name;
        storage.set('spot.sim.platform', name);
        renderPanes();
      }
      fill();
      if (done) edited();
    };
  }

  // ---------------------------------------------------------------- input events

  $('#simRealTime').onchange = ev => {
    if (!S.settings) return;
    setPath(S.settings, 'realTime', ev.target.checked);
    edited();
  };

  panes.addEventListener('input', ev => {
    const el = ev.target;
    if (!el.dataset.path || el.type === 'checkbox' || el.tagName === 'SELECT') return;
    if (!commit(el)) return;
    S.editSeq++;
    renderDerived();
    renderSetup();
    schedulePush(500);
  });
  panes.addEventListener('change', ev => {
    const el = ev.target;
    if (!el.dataset.path) return;
    if (commit(el)) edited(el.dataset.path === 'diagram');
  });
  function commit(el) {
    let v;
    if (el.type === 'checkbox') v = el.checked;
    else if (el.dataset.type === 'text') v = el.value;
    else {
      v = el.value === '' ? NaN : Number(el.value);
      const ok = el.dataset.rate !== undefined ? validRate(v) : el.dataset.pwm !== undefined ? validPwm(v)
        : el.dataset.min !== undefined ? isFinite(v) && v >= Number(el.dataset.min) && v <= Number(el.dataset.max)
        : isFinite(v);
      el.classList.toggle('invalid', !ok);
      if (!ok) return false;
    }
    setPath(S.settings, el.dataset.path, v);
    return true;
  }
  panes.addEventListener('click', ev => {
    const b = ev.target.closest('button');
    if (b && b.dataset.ratelock !== undefined) return handleLockClick(b.dataset.ratelock);
    if (!b || !S.settings) return;
    if (b.dataset.toggle) {
      setPath(S.settings, b.dataset.toggle, !getPath(S.settings, b.dataset.toggle));
      edited(true);
    } else if (b.dataset.set) {
      setPath(S.settings, b.dataset.set, b.dataset.value);
      edited(true);
    } else if (b.dataset.pick) {
      S.platform = b.dataset.pick;
      storage.set('spot.sim.platform', S.platform);
      renderPanes();
      fill();
    } else if (b.dataset.setpose) {
      setPose(b.dataset.setpose);
      fill();
      renderSetup();
    } else if (b.dataset.action === 'openDiagram') {
      act(() => matlab('diagram.open'));
    } else if (b.dataset.action === 'writeTracker') {
      act(async () => toast(`Wrote ${(await matlab('massprops.writeTracker')).file}`, 'success', 6000));
    }
  });
  for (const b of $$('[data-simtab]')) b.onclick = () => showTab(b.dataset.simtab);

  $('#simOpen').onclick = () => act(async () => {
    toast('Choose the file in MATLAB\'s dialog', 'info', 4000);
    const r = await matlab('settings.open');
    if (r && !r.cancelled) { S.panesBuilt = false; applyReply(r); toast(`Loaded ${r.loaded}`, 'success', 5000); }
  });
  $('#simSaveAs').onclick = () => act(async () => {
    toast('Choose where to save in MATLAB\'s dialog', 'info', 4000);
    const r = await matlab('settings.saveAs');
    if (r && r.saved) toast(`Saved ${r.saved}`, 'success', 5000);
  });
  $('#simSaveResult').onclick = () => act(() => store.saveSimulation());
  // Reset: a clean slate, in parts (settings in MATLAB, this page's preferences, loaded results)
  const resetDialog = $('#resetDialog');
  $('#simReset').onclick = () => {
    for (const c of $$('[data-reset]', resetDialog)) c.checked = true;
    resetDialog.showModal();
  };
  for (const c of $$('[data-reset]', resetDialog)) {
    c.onchange = () => { $('#resetGo').disabled = !$$('[data-reset]', resetDialog).some(x => x.checked); };
  }
  $('#resetGo').onclick = () => act(async () => {
    const what = Object.fromEntries($$('[data-reset]', resetDialog).map(c => [c.dataset.reset, c.checked]));
    resetDialog.close();
    toast('Resetting to the defaults…', 'info', 4000);
    if (what.settings) {
      clearTimeout(S.pushTimer);
      S.panesBuilt = false;
      const r = await matlab('settings.reset');
      applyReply(r);
      S.pushedSeq = S.editSeq;
      toast(r.backup ? `Settings reset to the defaults. The previous ones are in ${r.backup} (Load… to bring them back).`
                     : 'Settings reset to the defaults.', 'success', 9000);
    }
    if (what.results) {
      for (const d of [...store.loaded]) await store.unload(d.id);
    }
    if (what.view) {
      // Every preference this page keeps (spot.*) except the command history; then start afresh
      try {
        for (const k of Object.keys(localStorage)) if (k.startsWith('spot.') && k !== 'spot.cmdHistory') localStorage.removeItem(k);
      } catch { /* storage blocked: nothing kept anyway */ }
      setTimeout(() => location.reload(), what.settings ? 1500 : 300);
    }
  });

  // ---------------------------------------------------------------- running a simulation

  $('#simRun').onclick = () => act(async () => {
    if (S.sim.running) {
      await api('/api/sim/stop', {});
      toast('Stopping… (MATLAB checks between steps, so it can take a moment)', 'info', 5000);
      return;
    }
    clearTimeout(S.pushTimer);
    if (S.editSeq !== S.pushedSeq) await push();
    S.progress = null;
    S.sim = await api('/api/sim/run', {});
    messages?.simStarted();
    livePlot?.simStarted();
    renderRun();
  });

  function renderRun() {
    const btn = $('#simRun'), status = $('#simStatus');
    const sim = S.sim || {};
    const ready = S.matlab.connected && S.settings && S.settings.diagram && !S.error;
    if (sim.running) {
      btn.className = 'btn btn-stop';
      btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="6" y="6" width="12" height="12" rx="2" fill="currentColor"/></svg><span>Stop</span>';
      btn.disabled = false;
      const elapsed = Math.round(Date.now() / 1000 - sim.started);
      const p = S.progress;
      const phase = !p || p.status === 'stopped' || p.status === 'compiling' ? 'Preparing the diagram'
        : p.status === 'initializing' ? 'Initialising' : p.status === 'terminating' ? 'Finishing'
        : `Simulating: ${fmt(p.t, 4)} of ${fmt(p.tsim, 4)} s`;
      status.className = 'hint';
      status.innerHTML = `<span class="spinner dark"></span> ${phase} · ${elapsed} s`;
    } else {
      btn.className = 'btn btn-go';
      btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg><span>Run Simulation</span>';
      btn.disabled = !ready || Boolean(S.matlab.busy);
      if (!S.matlab.connected) {
        status.className = 'hint warn-text';
        status.textContent = 'MATLAB is not connected. Run SPOT in MATLAB, in this project\'s folder.';
      } else if (S.authError && !S.settings) {
        status.className = 'hint bad';
        status.textContent = S.authError;
      } else if (S.error) {
        status.className = 'hint bad';
        status.textContent = 'Fix the settings first (see the message at the top).';
      } else if (sim.finished && sim.ok === false) {
        status.className = 'hint bad';
        const det = sim.error_details;
        status.innerHTML = `✕ ${esc(sim.error)}` + (det ? ' <span class="err-actions">' +
          '<button type="button" class="btn btn-sm" data-err="details" title="Show the causes, the blocks and MATLAB\'s full report in the command window below">Details</button>' +
          (det.blocks && det.blocks.length ? `<button type="button" class="btn btn-sm" data-err="block" title="Open ${esc(det.blocks[0])} in Simulink, highlighted">Show Block</button>` : '') +
          '</span>' : '');
      } else if (sim.finished && sim.summary) {
        const sm = sim.summary;
        status.className = 'hint ok';
        status.textContent = `${sm.stopped ? '■ Stopped early' : '✓ Simulated'} ${fmt(S.result ? S.result.t[S.result.t.length - 1] : sm.tsim, 4)} s in ${fmt(sm.seconds, 3)} s · ${sm.samples} samples logged (dataClass in the workspace)`;
      } else {
        status.className = 'hint';
        status.textContent = S.settings ? `Ready: ${S.settings.diagram} · ${fmt(S.derived ? S.derived.tsim : 0, 6)} s` : 'Loading settings…';
      }
    }
    for (const b of ['#simOpen', '#simSaveAs', '#simReset']) $(b).disabled = !S.matlab.connected || Boolean(sim.running);
    // Save results: next to Run, once there is a simulation to save
    $('#simSaveResult').hidden = !S.result || Boolean(sim.running);
    $('#simSaveResult').disabled = !S.matlab.connected;
    const file = $('#settingsFile');
    file.textContent = (S.file || 'spot_settings.json').split(/[\\/]/).pop();
    file.title = S.file ? `${S.file} (saved automatically)` : '';
    $('#settingsBar .sb-label').title = `Settings: ${S.file || 'spot_settings.json'}, saved automatically. They are what the Simulation tab shows (scenario, timeline, initial conditions, platforms, control, solver), used for simulations and for building experiments.`;
  }

  async function loadResult(announce = false) {
    const res = await api('/api/sim/result');
    S.result = res;
    if (announce) {
      // What Simulink printed while simulating (kept out of MATLAB's own Command Window)
      const sm = S.sim.summary || {};
      const n = sm.warnings || 0;
      const head = `${sm.stopped ? 'Simulation stopped' : 'Simulation finished'} in ${fmt(sm.seconds, 3)} s · ` +
        (n ? `${n} warning${n === 1 ? '' : 's'} from Simulink (click to show)` : 'no warnings');
      if (res.messages) printDetails(head, res.messages, n ? 'c-warn' : 'c-info');
      else print(head, 'c-info');
    }
    S.resultId = S.sim.result_id;
    messages?.simResult(res.debug);
    livePlot?.simResult(res.live);
    viewer?.setSimulation(res);
    if (S.shown) layers();
    renderPlayback();
    seek(span()[0]);
    if (S.shown) play(true);
    renderRun();
  }

  // ---------------------------------------------------------------- playback
  // The last simulation, and any loaded results laid over it (Compare), on one clock: each
  // overlay can be shifted by an offset [s] to line it up.

  const scrub = $('#pbScrub');
  S.primaryShown = true;   // not remembered: a new page always shows the last simulation
  S.overlays = [];   // [{id, result, label, offset, phases}] as given to the viewer

  function tracks() {
    const out = [];
    if (S.result && S.primaryShown) out.push({ result: S.result, offset: 0 });
    return out.concat(S.overlays);
  }
  const hasPlayback = () => tracks().length > 0;
  function span() {
    let t0 = Infinity, t1 = -Infinity;
    for (const k of tracks()) {
      const t = k.result.t;
      t0 = Math.min(t0, t[0] + k.offset);
      t1 = Math.max(t1, t[t.length - 1] + k.offset);
    }
    return isFinite(t0) ? [t0, t1] : [0, 0];
  }
  function phases() {
    const k = tracks().find(x => x.result.phases);
    if (!k) return { names: [], ends: [] };
    return { names: list(k.result.phases.names), ends: list(k.result.phases.ends).map(e => e + k.offset) };
  }
  function renderPlayback() {
    const pb = $('#playback');
    const any = hasPlayback();
    pb.hidden = !any && !S.result && !S.overlays.length && !store.loaded.some(d => d.crafts.length);
    $('#simEmpty').classList.toggle('hidden', any || Boolean(S.result));
    $('#pbPlay').disabled = !any;
    scrub.disabled = !any;
    renderCompare();
    if (!any) { $('#pbPhases').innerHTML = ''; $('#pbTime').textContent = '—'; $('#pbPhase').textContent = ''; return; }
    const [t0, t1] = span();
    $('#pbPhases').innerHTML = phases().ends.slice(0, -1).filter(e => e > t0 && e < t1)
      .map(e => `<i style="left:${(e - t0) / (t1 - t0) * 100}%" title="${fmt(e, 5)} s"></i>`).join('');
  }
  function seek(t) {
    if (!hasPlayback()) return;
    const [t0, t1] = span();
    S.t = Math.min(Math.max(t, t0), t1);
    viewer?.setSimTime(S.t);
    messages?.simTime(S.t);
    livePlot?.simTime(S.t);
    if (document.activeElement !== scrub) scrub.value = Math.round((S.t - t0) / Math.max(t1 - t0, 1e-9) * 1000);
    $('#pbTime').textContent = `${S.t.toFixed(1)} / ${t1.toFixed(1)} s`;
    const ph = phases();
    const k = ph.ends.findIndex(e => S.t <= e + 1e-9);
    $('#pbPhase').textContent = k >= 0 ? ph.names[k] : '';
  }
  function play(on) {
    if (!hasPlayback()) on = false;
    else if (on && S.t >= span()[1] - 1e-6) seek(span()[0]);
    S.playing = on;
    $('#pbPlay').textContent = on ? '❚❚' : '▶';
    S.lastTick = performance.now();
    if (on) requestAnimationFrame(tick);
  }
  function tick(now) {
    if (!S.playing) return;
    const dt = (now - S.lastTick) / 1000;
    S.lastTick = now;
    seek(S.t + dt * Number($('#pbSpeed').value));
    if (S.t >= span()[1] - 1e-9) return play(false);
    requestAnimationFrame(tick);
  }
  $('#pbPlay').onclick = () => play(!S.playing);
  scrub.addEventListener('input', () => {
    if (!hasPlayback()) return;
    const [t0, t1] = span();
    play(false);
    seek(t0 + (t1 - t0) * scrub.value / 1000);
  });
  const traces = $('#pbTraces');
  traces.checked = storage.get('spot.sim.traces', true);
  traces.onchange = () => { storage.set('spot.sim.traces', traces.checked); viewer?.setTraces(traces.checked); };
  $('#pbSpeed').value = String(storage.get('spot.sim.speed', 5));
  $('#pbSpeed').onchange = () => storage.set('spot.sim.speed', Number($('#pbSpeed').value));
  document.addEventListener('keydown', ev => {
    if (!S.shown || ev.key !== ' ' || ev.target.closest('input, textarea, select, button')) return;
    ev.preventDefault();
    play(!S.playing);
  });

  function layers() {
    viewer?.setLayers({ live: false, sim: hasPlayback(), setup: S.pose !== 'off' });
    viewer?.setPrimaryShown(S.primaryShown);
    viewer?.setTraces(traces.checked);
  }

  // ---------------------------------------------------------------- compare: results over the simulation

  const pop = $('#pbOverlays');
  $('#pbCompare').onclick = ev => {
    ev.stopPropagation();
    pop.hidden = !pop.hidden;
    renderCompare();
    if (!pop.hidden) act(() => store.refresh());
  };
  document.addEventListener('pointerdown', ev => {
    if (!pop.hidden && !ev.target.closest('#pbOverlays, #pbCompare')) { pop.hidden = true; renderCompare(); }
  });

  function renderCompare() {
    const sets = store.loaded.filter(d => d.crafts.length);
    const n = S.overlays.length;
    const count = $('#pbCompare .pb-compare-count');
    count.hidden = !n;
    count.textContent = n;
    $('#pbCompare').classList.toggle('on', n > 0);
    $('#pbCompare').setAttribute('aria-expanded', String(!pop.hidden));
    if (pop.hidden) return;
    const row = d => {
      const o = store.overlay(d.id);
      const waiting = o.on && !S.overlays.some(x => x.id === d.id);
      return `<div class="pb-ov-row"><label class="toggle"><input type="checkbox" data-ov="${d.id}"${o.on ? ' checked' : ''}><span></span>
          <span class="pill">${kindTag(d.kind)}</span>${esc(d.name)}${waiting ? ' <span class="spinner dark"></span>' : ''}</label>
        <label class="pb-ov-offset" title="Shift this result in time [s] to line it up with the others">±<input class="input" type="number" step="0.1" data-offset="${d.id}" value="${o.offset || 0}">s</label></div>`;
    };
    pop.innerHTML = `
      <div class="pb-ov-head">Replay on the Table</div>
      ${S.result ? `<div class="pb-ov-row"><label class="toggle"><input type="checkbox" data-primary${S.primaryShown ? ' checked' : ''}><span></span>
        <span class="pill">SIM</span>Last Simulation</label></div>` : ''}
      ${sets.map(row).join('') || '<p class="hint">No results loaded with platform positions. Load an experiment run or a saved simulation to replay it over this one.</p>'}
      <p class="hint">Overlays: outlined platforms and dashed paths, named after the result.</p>
      <div class="pb-ov-foot">
        <button type="button" class="btn btn-sm" data-pick>Load Results…</button>
        <button type="button" class="btn btn-sm" data-keep ${S.result && S.matlab.connected ? '' : 'disabled'} title="Keep a copy of this simulation to compare with the next ones">Keep This Simulation</button>
      </div>`;
  }
  pop.addEventListener('change', ev => {
    const el = ev.target;
    if (el.dataset.primary !== undefined) {
      S.primaryShown = el.checked;
      layers(); renderPlayback(); seek(S.t);
    } else if (el.dataset.ov) {
      store.setOverlay(el.dataset.ov, { on: el.checked });
    } else if (el.dataset.offset) {
      const v = Number(el.value);
      if (isFinite(v)) store.setOverlay(el.dataset.offset, { offset: v });
    }
  });
  pop.addEventListener('click', ev => {
    if (ev.target.closest('[data-pick]')) act(async () => showOverlays(await store.pick()));
    if (ev.target.closest('[data-keep]')) act(async () => showOverlays(await store.keepSimulation()));
  });
  $('#simEmptyLoad').onclick = () => act(async () => showOverlays(await store.pick()));
  function showOverlays(got) {
    for (const d of got || []) if (d.crafts.length) store.setOverlay(d.id, { on: true });
    if ((got || []).length && !got.some(d => d.crafts.length)) toast('That result has no platform positions to replay; plot it on the Results page.', 'info', 6000);
  }

  // Follow the store: fetch the motion of each result turned on, then hand them to the viewer
  let syncing = 0;
  async function syncOverlays() {
    const run = ++syncing;
    const want = store.loaded.filter(d => d.crafts.length && store.overlay(d.id).on);
    renderPlayback();
    const out = [];
    for (const d of want) {
      try {
        const result = await store.trajectory(d.id);
        out.push({ id: d.id, result, label: d.name, offset: store.overlay(d.id).offset || 0 });
      } catch (e) {
        toast(`${d.name}: ${e.message}`, 'error', 8000);
        store.setOverlay(d.id, { on: false });
        return;
      }
    }
    if (run !== syncing) return;   // changed again meanwhile
    const before = hasPlayback();
    S.overlays = out;
    viewer?.setOverlays(out);
    if (S.shown) layers();
    renderPlayback();
    if (!before && hasPlayback()) seek(span()[0]); else seek(S.t);
  }
  store.on(syncOverlays);

  // ---------------------------------------------------------------- command window

  const out = $('#cmdOut'), input = $('#cmdIn');
  function print(textOut, cls) {
    const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
    const div = document.createElement('div');
    if (cls) div.className = cls;
    div.textContent = textOut;
    out.append(div);
    while (out.childElementCount > 2000) out.firstElementChild.remove();
    if (atBottom) out.scrollTop = out.scrollHeight;
  }
  function printDetails(summary, textOut, cls) {
    const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
    const d = document.createElement('details');
    if (cls) d.className = cls;
    const sm = document.createElement('summary');
    sm.textContent = summary;
    const body = document.createElement('div');
    body.className = 'c-details';
    body.textContent = textOut;
    d.append(sm, body);
    out.append(d);
    if (atBottom) out.scrollTop = out.scrollHeight;
  }

  // An error from MATLAB as an entry in the command window: the root cause, with (on click)
  // where it is (your code, the blocks, each with a button to show it in Simulink), the chain
  // of causes, what Simulink printed before it and MATLAB's full report
  function printError(title, det) {
    const atBottom = out.scrollHeight - out.scrollTop - out.clientHeight < 40;
    const d = document.createElement('details');
    d.className = 'c-err err-entry';
    const sm = document.createElement('summary');
    sm.textContent = `${title}: ${det.summary} (click for details)`;
    const body = document.createElement('div');
    body.className = 'c-details';
    const line = (text, cls) => {
      const el = document.createElement('div');
      if (cls) el.className = cls;
      el.textContent = text;
      body.append(el);
      return el;
    };
    if (det.where) line(`In your code: ${det.where}`, 'err-where');
    for (const b of list(det.blocks)) {
      const el = line(`Block: ${b} `, 'err-block');
      const btn = document.createElement('button');
      btn.type = 'button';
      btn.className = 'btn btn-sm';
      btn.textContent = 'Show in Simulink';
      btn.onclick = () => showBlock(b);
      el.append(btn);
    }
    const chain = list(det.chain);
    if (chain.length > 1) {
      line('Causes, from the first reported to the root cause:', 'err-head');
      for (const c of chain) line(`${'  '.repeat(c.depth)}${c.depth ? '↳ ' : ''}${c.message}`, 'err-cause');
    }
    const more = (label, text) => {
      if (!text) return;
      const m = document.createElement('details');
      const s = document.createElement('summary');
      s.textContent = label;
      const pre = document.createElement('div');
      pre.className = 'err-text';
      pre.textContent = text;
      m.append(s, pre);
      body.append(m);
    };
    more('What Simulink Printed Before the Error', det.output);
    more("MATLAB's Full Report", det.report);
    d.append(sm, body);
    out.append(d);
    if (atBottom) out.scrollTop = out.scrollHeight;
    return d;
  }
  async function showBlock(block) {
    toast('Opening the block in Simulink…', 'info', 3000);   // (the first time takes a while)
    try {
      await matlab('diagram.showBlock', { block });
      toast(`Opened in Simulink: ${block.split('/').slice(1).join('/')}`, 'success', 4000);
    } catch (e) {
      toast(e.message, 'error', 8000);
    }
  }

  // The status line's buttons after a failed simulation
  $('#simStatus').addEventListener('click', ev => {
    const b = ev.target.closest('[data-err]');
    const det = S.sim && S.sim.error_details;
    if (!b || !det) return;
    if (b.dataset.err === 'block') { showBlock(list(det.blocks)[0]); return; }
    if (!S.errorEntry || !S.errorEntry.isConnected) S.errorEntry = printError('Simulation failed', det);
    S.errorEntry.open = true;
    S.errorEntry.scrollIntoView({ block: 'nearest' });
  });

  async function runCommand(cmd) {
    if (!cmd.trim()) return;
    S.history = [...S.history.filter(h => h !== cmd), cmd].slice(-200);
    storage.set('spot.cmdHistory', S.history);
    S.histPos = -1;
    if (cmd.trim() === 'clc') { out.textContent = ''; return; }
    print(`>> ${cmd}`, 'c-cmd');
    try {
      const r = await matlab('workspace.eval', { command: cmd });
      if (r.output) print(r.output.replace(/^\n+|\n+$/g, ''), '');
      if (r.error) print(r.error, 'c-err');
    } catch (e) {
      print(e.message, 'c-err');
    }
    refreshWorkspace();
  }
  input.addEventListener('keydown', ev => {
    if (ev.key === 'Enter' && !ev.shiftKey) {
      ev.preventDefault();
      const cmd = input.value;
      input.value = '';
      autosize();
      runCommand(cmd);
    } else if ((ev.key === 'ArrowUp' || ev.key === 'ArrowDown') && !input.value.includes('\n') && S.history.length) {
      ev.preventDefault();
      const n = S.history.length;
      S.histPos = ev.key === 'ArrowUp' ? (S.histPos < 0 ? n - 1 : Math.max(0, S.histPos - 1))
                                       : (S.histPos < 0 ? -1 : S.histPos + 1);
      if (S.histPos >= n) S.histPos = -1;
      input.value = S.histPos < 0 ? '' : S.history[S.histPos];
      autosize();
    }
  });
  function autosize() { input.style.height = 'auto'; input.style.height = Math.min(input.scrollHeight, 140) + 'px'; }
  input.addEventListener('input', autosize);
  $('#cmdClear').onclick = () => { if ($('#matlabConsole').dataset.tab !== 'plot') out.textContent = ''; };
  $('#toggleMatlab').onclick = () => {
    const c = $('#matlabConsole');
    c.classList.toggle('collapsed');
    $('#toggleMatlab').textContent = c.classList.contains('collapsed') ? '▴' : '▾';
    storage.set('spot.sim.consoleCollapsed', c.classList.contains('collapsed'));
  };
  if (storage.get('spot.sim.consoleCollapsed', false)) $('#toggleMatlab').click();

  // ---------------------------------------------------------------- workspace

  let wsTimer = null;
  function refreshWorkspaceSoon() { clearTimeout(wsTimer); wsTimer = setTimeout(refreshWorkspace, 800); }
  async function refreshWorkspace() {
    if (!S.matlab.connected || S.sim.running) return;
    try {
      S.workspace = await matlab('workspace.list') || [];
      if (!Array.isArray(S.workspace)) S.workspace = [S.workspace];
    } catch (e) {
      return;  // busy: the next refresh catches up
    }
    renderWorkspace();
    if (S.inspected) inspect(S.inspected, true);
  }
  function renderWorkspace() {
    const f = $('#wsFilter').value.trim().toLowerCase();
    const rows = S.workspace.filter(v => !f || v.name.toLowerCase().includes(f));
    $('#wsCount').textContent = S.workspace.length ? `${S.workspace.length}` : '';
    $('#wsBody').innerHTML = rows.map(v => `<tr data-name="${esc(v.name)}" class="${v.name === S.inspected ? 'sel' : ''}">
      <td class="mono">${esc(v.name)}</td><td class="mono" title="${esc(v.value)}">${esc(v.value)}</td>
      <td class="mono">${esc(v.size)}</td><td>${esc(v.class)}</td></tr>`).join('');
  }
  $('#wsFilter').addEventListener('input', renderWorkspace);
  $('#wsRefresh').onclick = () => refreshWorkspace();
  $('#wsBody').addEventListener('click', ev => {
    const tr = ev.target.closest('tr[data-name]');
    if (tr) inspect(tr.dataset.name);
  });
  async function inspect(name, quiet) {
    let r;
    try {
      r = await matlab('workspace.inspect', { name });
    } catch (e) {
      if (!quiet) toast(e.message, 'error');
      if (quiet) { S.inspected = null; $('#wsInspect').hidden = true; }
      return;
    }
    S.inspected = name;
    renderWorkspace();
    const box = $('#wsInspect');
    box.hidden = false;
    let body = '';
    if (r.values !== undefined) {
      const rows = Array.isArray(r.values) ? (Array.isArray(r.values[0]) ? r.values : [r.values]) : [[r.values]];
      body = `<table class="gain-table k-table"><tbody>${rows.map(row => `<tr>${(Array.isArray(row) ? row : [row]).map(v => `<td>${fmt(v, 5)}</td>`).join('')}</tr>`).join('')}</tbody></table>` +
             (r.truncated ? '<p class="hint">First 50 rows and 12 columns.</p>' : '') + (r.note ? `<p class="hint">${esc(r.note)}</p>` : '');
    } else if (r.fields !== undefined) {
      const fields = Array.isArray(r.fields) ? r.fields : [r.fields];
      body = `<table class="ws-table"><tbody>${fields.map(f => `<tr data-field="${esc(name + '.' + f.name)}"><td class="mono">${esc(f.name)}</td><td class="mono">${esc(f.value)}</td><td class="mono">${esc(f.size)}</td><td>${esc(f.class)}</td></tr>`).join('')}</tbody></table>`;
    } else if (r.series) {
      body = sparkline(r.series);
    } else {
      body = `<pre class="mono">${esc(r.text || '')}</pre>`;
    }
    const up = name.includes('.') ? `<button type="button" class="btn btn-ghost btn-sm" data-up="${esc(name.replace(/\.[^.]*$/, ''))}">↑</button>` : '';
    box.innerHTML = `<div class="ws-inspect-head">${up}<span class="mono"><b>${esc(name)}</b> · ${esc(r.size)} ${esc(r.class)}</span>
      <button type="button" class="btn btn-ghost btn-sm" data-close>✕</button></div><div class="ws-inspect-body">${body}</div>`;
  }
  $('#wsInspect').addEventListener('click', ev => {
    const field = ev.target.closest('[data-field]');
    if (field) return inspect(field.dataset.field);
    const up = ev.target.closest('[data-up]');
    if (up) return inspect(up.dataset.up);
    if (ev.target.closest('[data-close]')) { S.inspected = null; $('#wsInspect').hidden = true; renderWorkspace(); }
  });
  function sparkline(series) {
    const t = Array.isArray(series.t) ? series.t : [series.t];
    let data = series.data;
    if (!Array.isArray(data[0])) data = [data];
    const W = 600, H = 120, colors = ['#4c8dff', '#3ccf7e', '#f5a524', '#ff6b6b', '#be4bdb', '#22b8cf'];
    const t0 = t[0], t1 = t[t.length - 1] || 1;
    const lines = data.map((row, k) => {
      const lo = Math.min(...row), hi = Math.max(...row), r = hi - lo || 1;
      const pts = row.map((v, i) => `${((t[i] - t0) / (t1 - t0 || 1) * W).toFixed(1)},${(H - 4 - (v - lo) / r * (H - 8)).toFixed(1)}`).join(' ');
      return { pts, color: colors[k % colors.length], lo, hi };
    });
    return `<svg class="spark" viewBox="0 0 ${W} ${H}" preserveAspectRatio="none">${lines.map(l => `<polyline points="${l.pts}" fill="none" stroke="${l.color}" stroke-width="1.5" vector-effect="non-scaling-stroke"/>`).join('')}</svg>
      <p class="hint">${fmt(t0, 4)} to ${fmt(t1, 4)} s${series.units ? ` · ${esc(series.units)}` : ''} · ${lines.map(l => `<span style="color:${l.color}">${fmt(l.lo, 4)} … ${fmt(l.hi, 4)}</span>`).join(' · ')}</p>`;
  }

  // ---------------------------------------------------------------- status from the server

  function renderBadge() {
    const b = $('#matlabBadge'), m = S.matlab;
    if (!m.connected) { b.className = 'pill err'; b.textContent = 'Not Connected'; return; }
    const rel = (m.info && m.info.release) || 'MATLAB';
    if (m.busy) {
      b.className = 'pill warn';
      b.textContent = `${rel} · ${m.busy.label || m.busy.cmd} · ${Math.round(Date.now() / 1000 - m.busy.since)} s`;
    } else {
      b.className = 'pill ok';
      b.textContent = `${rel} · Idle`;
    }
  }

  setInterval(() => { if (S.sim.running) renderRun(); if (S.matlab.busy) renderBadge(); }, 1000);

  return {
    printError,
    onState(st) {
      const was = S.matlab.connected;
      S.matlab = st.matlab || { connected: false };
      S.sim = st.sim || {};
      renderBadge();
      renderRun();
      if (S.matlab.connected && !was) act(loadSettings).then(refreshWorkspace);
      if (S.sim.ok && S.sim.result_id && S.sim.result_id !== S.resultId) act(loadResult);
    },
    onMatlab(m) {
      const was = S.matlab.connected;
      S.matlab = m;
      renderBadge();
      renderRun();
      if (m.connected && !was) act(loadSettings).then(refreshWorkspace);
    },
    onSim(sim) {
      const finished = S.sim.running && !sim.running;
      S.sim = sim;
      renderRun();
      if (finished) {
        if (sim.ok) {
          act(() => loadResult(true));
          refreshWorkspace();
          toast(sim.summary && sim.summary.stopped ? 'Simulation stopped' : 'Simulation finished', 'success', 3000);
        } else {
          toast(`Simulation failed: ${sim.error}`, 'error', 12000);
          if (sim.error_details) S.errorEntry = printError('Simulation failed', sim.error_details);
          else print(`Simulation failed: ${sim.error}`, 'c-err');
        }
      }
    },
    onMatlabEvent(ev) {
      if (ev.event === 'sim_progress') { S.progress = ev; renderRun(); }
      if (ev.event === 'sim_debug') messages?.simMessages(ev.messages);
      if (ev.event === 'sim_live') livePlot?.simData(ev.streams);
    },
    show() {
      S.shown = true;
      layers();
      renderSetup();
      if (hasPlayback()) viewer?.setSimTime(S.t);
    },
    hide() {
      S.shown = false;
      play(false);
      renderSetup();   // the poses stay visible elsewhere, but can't be dragged there
    },
    hasResult: () => Boolean(S.result),
    getState: () => ({ settings: S.settings, derived: S.derived, result: S.result, pose: S.pose }),
    setPlatform(name, on) {
      // Switch a platform on or off, as its chip on Scenario does (the arm goes with RED)
      if (!S.settings) return;
      setPath(S.settings, `platforms.${name}`, on);
      if (name === 'RED' && !on) setPath(S.settings, 'platforms.ARM', false);
      edited(true);
    },
    setArm(on, mount) {
      // The arm on the Chaser (the Designer's Arm), at mount {face, side}
      if (!S.settings) return;
      setPath(S.settings, 'platforms.ARM', on);
      if (on) setPath(S.settings, 'platforms.RED', true);
      if (on && mount) setPath(S.settings, 'mass.arm.mount', { face: mount.face, side: mount.side });
      edited(true);
    },
    setSetting(path, value) {
      // A setting edited on another page (e.g. dataRate, the Experiment tab's Frequency)
      if (!S.settings) return false;
      setPath(S.settings, path, value);
      edited();
      return true;
    },
    renderSetup,
  };
}
