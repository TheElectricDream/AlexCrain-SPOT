// The Calibration tab (both consoles). The lab console runs the calibrations (spot_console/
// calibration.py) and keeps the lab's calibration file; the local console shows its project's copy
// (resources/spot_calibration.json, which the simulation uses) next to the lab's, and offers the
// lab's whenever it is newer (the sync dialog: Update, Not Now, or Never Ask Again).
//
// Sections: Table Slope, Thrusters and Arm.

import { platformName, COLORS } from './platforms.js';
import { drawTable, heightGrid, colorBar, thrusterDiagram, thrustBars, burnPaths, armFigure, frictionPlot, stepsPlot } from './calviz.js';

const $ = (s, el = document) => el.querySelector(s);
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const NAMES = ['RED', 'BLACK', 'BLUE'];
const fmt = (v, d = 2) => (v == null || !Number.isFinite(v) ? '—' : v.toFixed(d));
const when = iso => {
  if (!iso) return 'Never';
  const d = new Date(iso);
  return d.toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' }) + ' ' +
    d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
};

// The four steps of the table calibration, as the stepper shows them (and the run's steps they cover)
const TABLE_STEPS = [
  ['Rest', 'PhaseSpace records the platform resting on the table: the cameras\' noise.',
   '<path d="M4 17h16M7 17v-3h10v3" /><circle cx="12" cy="9" r="2.2"/>'],
  ['Float', 'The air bearings come on with the thrusters off: it drifts from where it is.',
   '<path d="M4 18h16"/><rect x="7" y="9" width="10" height="5" rx="1"/><path d="M8 16.5h1M11.5 16.5h1M15 16.5h1"/>'],
  ['Drift Map', 'It flies to each point of the grid, settles, and drifts again with the thrusters off.',
   '<circle cx="6" cy="7" r="1.6"/><circle cx="18" cy="7" r="1.6"/><circle cx="6" cy="17" r="1.6"/><circle cx="18" cy="17" r="1.6"/><path d="M7.5 8.2 16.5 15.8" stroke-dasharray="2 2"/>'],
  ['Fit', 'The table\'s height surface and the air bearings\' drag are fitted to every drift.',
   '<path d="M4 18c3-8 6-10 8-10s5 2 8 10"/><path d="M4 18h16"/>'],
];

// The thruster calibration's four steps, for each thruster in turn
const THRUSTER_STEPS = [
  ['Float', 'The air bearings come on where the platform rests.',
   '<path d="M4 18h16"/><rect x="7" y="9" width="10" height="5" rx="1"/><path d="M8 16.5h1M11.5 16.5h1M15 16.5h1"/>'],
  ['Fire', 'One thruster fires at full thrust for the burn time; PhaseSpace records the push.',
   '<rect x="6" y="8" width="10" height="8" rx="1"/><path d="M16 12h5M18 9.5l3 2.5-3 2.5"/>'],
  ['Return', 'The diagram\'s controller flies it back to where it started.',
   '<path d="M19 12a7 7 0 1 1-2-4.9"/><path d="M17 3v4h-4"/>'],
  ['Rest', 'Pucks off, a moment\'s rest, then the next thruster (1 to 8, and each repeat).',
   '<path d="M4 17h16M7 17v-3h10v3"/>'],
];

// The arm calibration's four tests (calibrate_arm.py), each on one joint while the others hold
const ARM_TESTS = [
  ['breakaway', 'Breakaway', 'The motor current creeps up from zero until the joint moves, three times each way: the static friction (breakaway torque).'],
  ['sweep', 'Speed Sweep', 'The joint turns back and forth at nine speeds; the current it needs at each is the friction at that speed (Coulomb, Stribeck, viscous).'],
  ['inertia', 'Inertia', 'A torque step from rest, each way: how fast it accelerates gives the joint\'s inertia (link and rotor).'],
  ['steps', 'Position Steps', 'Goals of 10, 0, -10, 20, 0, -20, 5, 0° with SPOT\'s servo settings, for spot.arm.compareSteps to check the model.'],
];
const JOINTS = { 1: 'Shoulder', 2: 'Elbow', 3: 'Wrist' };
const ARM_MINUTES = { breakaway: 1.5, sweep: 2.5, inertia: 0.5, steps: 0.5 };   // per joint, about

export function initCalibration({ api, act, toast, storage, viewer, getStream = () => null }) {
  const S = {
    local: false, shown: false, section: storage.get('spot.calibration.section', 'table'),
    cal: null, path: null,                 // this console's calibration
    run: null, active: false,              // the lab console's run (lab console only)
    lab: null, labCal: null,               // local console: the lab console and its calibration
    platform: storage.get('spot.calibration.platform', 'RED'),
    form: storage.get('spot.calibration.table', { nx: 3, ny: 2, drift: 12, margin: 0.35, model: 'auto' }),
    thrForm: storage.get('spot.calibration.thrusters', { repeats: 2, burn: 2, margin: 0.35 }),
    armForm: storage.get('spot.calibration.arm', { joints: [1, 2, 3], tests: ['breakaway', 'sweep', 'inertia', 'steps'], simulate: false }),
    armCheck: null, armChecking: false, armDemo: { test: 'breakaway', joint: 1 }, armStepsJoint: 1,
    assumed: null,                         // lab console: what it takes each platform's mass properties to be
    asked: null,                           // the lab calibration already offered this session
  };
  const body = $('#calBody'), head = body;   // (the sync card is in the body's left column)

  // ---------------------------------------------------------------- rendering

  function render() {
    for (const b of document.querySelectorAll('[data-calsec]')) b.classList.toggle('active', b.dataset.calsec === S.section);
    if (S.section === 'table') body.innerHTML = tableSection();
    else if (S.section === 'thrusters') body.innerHTML = thrustersSection();
    else body.innerHTML = armSection();
    animateArm();
    draw();
  }

  function syncCard() {
    const mine = S.cal || {}, lab = S.labCal, reach = S.lab && S.lab.reachable;
    let pill = ['No Calibration Yet', ''];
    if (!reach) pill = ['Lab Console Not Reachable', ''];
    else if (!lab) pill = ['Lab Console Needs Update', 'warn'];
    else if (!lab.updated) pill = ['The Lab Has No Calibration', ''];
    else if (!mine.updated || lab.updated > mine.updated) pill = ['The Lab Has a Newer One', 'warn'];
    else pill = ['Up to Date', 'ok'];
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
      <p class="hint">Simulations use this project's copy (resources/spot_calibration.json): Scenario, Table Slope and Calibrated Thrust.</p>
    </section>`;
  }

  function tableSection() {
    const t = S.cal && S.cal.table;
    const run = S.run && S.run.kind === 'table' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    // which of the four stepper steps the run is in
    let stage = -1;
    if (run && run.step != null) {
      const name = run.steps[run.step].name;
      stage = name === 'Still' ? 0 : name === 'Float Here' ? 1 : name === 'Fit' ? 3 : 2;
      if (!running) stage = run.state === 'done' ? 4 : stage;
    }
    const stepper = TABLE_STEPS.map(([title, text, icon], k) => {
      const cls = running && k === stage ? 'now' : stage > k || (run && run.state === 'done') ? 'done' : '';
      return `<li class="cal-step ${cls}"><svg viewBox="0 0 24 24" aria-hidden="true">${icon}</svg>
        <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`;
    }).join('');
    const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2></div>
        <ol class="cal-steps">${stepper}</ol>
        <p class="hint">A drift ends after its time, or at once (pucks off) when the platform nears an edge or another
          platform. The platform needs PhaseSpace to see it the whole time, and the table clear around the grid.</p></section>`;
    return `<div class="cal-col">
        ${S.local ? syncCard() : ''}
        ${howCard}
        ${S.local ? labOnlyCard('table') : runCard(run, running)}
        ${run ? progressCard(run) : ''}
      </div>
      <div class="cal-col cal-wide">
        <section class="card cal-map-card">
          <div class="card-head"><h2>Table Slope</h2>${running ? `<span class="pill live">Measuring · ${platformName(run.platform)}</span>`
            : t ? `<span class="pill ok">${t.model === 'quadratic' ? 'Surface' : 'Plane'} · ${when(t.updated)}</span>` : '<span class="pill">Not Calibrated</span>'}</div>
          <div class="cal-map"><canvas id="calMap" aria-label="The table seen from above: the fitted height in colour, downhill arrows, the drifts"></canvas></div>
          ${running ? '<p class="hint">The run under way: the grid of start points (the current one in yellow), the drifts so far, and the platform with its goal. The table\'s map appears when the fit is done.</p>'
            : t ? `${colorBar(heightGrid(t).range)}${tableStats(t)}` : '<p class="hint">No table calibration yet. Run one on the lab console: the map shows the table\'s height (blue low, red high), its contour lines, which way is downhill, and the drifts it was fitted to.</p>'}
        </section>
      </div>`;
  }

  function tableStats(t) {
    const fixed = t.damping_fixed || [];
    const drag = Object.entries(t.damping || {}).map(([n, c]) => (fixed.includes(n)
      ? `${platformName(n)} not resolved (longer drifts tell it from the slope; 0 used)` : `${platformName(n)} ${fmt(c, 3)} /s`)).join(', ');
    const tile = (label, value, unit, title) => `<div class="cal-tile" title="${esc(title)}"><span>${label}</span><b>${value}</b><i>${unit}</i></div>`;
    const dir = t.downhill_deg;
    return `<div class="cal-tiles">
        ${tile('Tilt at Centre', fmt(t.tilt_centre_mrad, 3), 'mrad', 'The slope at the table\'s centre: 1 mrad is a 1 mm drop per metre, and pulls a floating platform at 0.0098 m/s²')}
        ${tile('Steepest', fmt(t.tilt_max_mrad, 3), 'mrad', 'The steepest slope anywhere on the table')}
        ${tile('Downhill', `<svg class="cal-dir" viewBox="-10 -10 20 20" aria-hidden="true"><g transform="rotate(${-dir})"><path d="M-7 0H6M2-4l4 4-4 4" /></g></svg>${fmt(dir, 0)}°`, 'from +x', 'Which way is downhill at the centre, from +x towards +y')}
        ${tile('Fit Residual', fmt(t.rms_mm, 2), 'mm', 'How far the drifts are from the fitted model (the cameras\' own noise is ' + fmt(t.noise_mm, 2) + ' mm)')}
      </div>
      ${t.warning ? `<p class="hint warn-text">${esc(t.warning)}</p>` : ''}
      <p class="hint">Fitted to ${t.drifts} drift${t.drifts === 1 ? '' : 's'} of the ${platformName(t.platform)} by ${esc(t.by || '?')}.
        Air-bearing drag: ${drag || '—'}. ${t.model === 'plane' ? 'A plane (one tilt everywhere): the drifts didn\'t cover enough of the table for a surface.' : 'A smooth surface (quadratic).'}</p>
      ${S.local ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-table" title="Remove the table calibration from the lab console\'s file">Remove</button></div>'}`;
  }

  function runCard(run, running) {
    // (while a run goes on, its own platform and settings)
    const f = running ? run.params : S.form, st = getStream(), streaming = st && st.running;
    const chosen = running ? run.platform : S.platform;
    const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
        <input type="number" class="input" data-calform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
    return `<section class="card">
        <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : streaming ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Start PhaseSpace First</span>'}</div>
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
        <p class="hint">${f.nx * f.ny + 1} drifts of up to ${f.drift} s, plus the flights between them: about ${Math.round((f.nx * f.ny + 1) * (Number(f.drift) + 25) / 60)} min. Clear the table around the grid; other platforms may stay where they are (points next to them are skipped).</p>
        <div class="cal-actions">
          ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
                    : `<button type="button" class="btn btn-go" data-cal="start-table" ${!streaming ? 'disabled title="Start PhaseSpace streaming first (PhaseSpace tab)"' : busyOther() ? 'disabled title="Another calibration is running"' : ''}>Start Table Calibration</button>`}
        </div>
      </section>`;
  }

  function labOnlyCard(what) {
    return `<section class="card"><div class="card-head"><h2>Run</h2><span class="pill">On the Lab Console</span></div>
        <p class="hint">The ${what === 'table' ? 'table slope' : what} calibration drives a platform, so it runs on the lab console
          (its Calibration tab${S.lab && S.lab.url ? `, <a href="${esc(S.lab.url)}#calibration" target="_blank" rel="noopener">${esc(S.lab.url)}</a>` : ''}).
          Its results come here with Update from Lab.</p></section>`;
  }

  function progressCard(run) {
    const icon = { waiting: '○', running: '◉', done: '✓', skipped: '–', failed: '✕' };
    const steps = run.steps.map((s, k) => `<li class="cal-prog ${s.state}${k === run.step ? ' current' : ''}"><span>${icon[s.state] || '○'}</span>
        <b>${esc(s.name)}</b><i>${esc(s.note || '')}</i></li>`).join('');
    const kind = { running: 'live', stopping: 'warn', done: 'ok', failed: 'err', stopped: 'warn' }[run.state] || '';
    const label = { running: 'Running', stopping: 'Stopping', done: 'Done', failed: 'Failed', stopped: 'Stopped' }[run.state] || run.state;
    return `<section class="card">
        <div class="card-head"><h2>Progress · ${platformName(run.platform)}</h2><span class="pill ${kind}">${label}</span></div>
        <p class="cal-msg ${run.state === 'failed' ? 'bad' : ''}">${esc(run.message)}</p>
        <ol class="cal-progress">${steps}</ol>
        <details class="advanced"><summary>Log</summary><pre class="cal-log">${esc(run.log.join('\n'))}</pre></details>
      </section>`;
  }

  // ---------------------------------------------------------------- the thrusters

  function thrustersSection() {
    const run = S.run && S.run.kind === 'thrusters' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    const step = run && run.step != null ? run.steps[run.step].name : '';
    const firing = running && step.startsWith('Thruster ') ? Number(step.slice(9)) : null;
    const msg = run ? run.message : '';
    const stage = !running || !firing ? -1 : /firing/.test(msg) ? 1 : /flying back/.test(msg) ? 2 : /floating/.test(msg) ? 0 : 3;
    const stepper = THRUSTER_STEPS.map(([title, text, icon], k) =>
      `<li class="cal-step ${k === stage ? 'now' : run && run.state === 'done' ? 'done' : ''}"><svg viewBox="0 0 24 24" aria-hidden="true">${icon}</svg>
        <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`).join('');
    const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2>${firing ? `<span class="pill live">Thruster ${firing}</span>` : ''}</div>
        <ol class="cal-steps">${stepper}</ol>
        <div class="cal-thr-live">${thrusterDiagram({ firing: stage === 1 ? firing : null })}</div>
        <p class="hint">Each burn's thrust comes from the push it gave: the platform's mass times its acceleration, with the
          table's slope (its calibration) taken out. So the platform's mass, inertia and centre of gravity must be right:
          the console asks before it starts.</p></section>`;
    const viewing = running ? run.platform : S.platform;
    const res = (S.cal && S.cal.thrusters || {})[viewing];
    return `<div class="cal-col">
        ${S.local ? syncCard() : ''}
        ${howCard}
        ${S.local ? labOnlyCard('thrusters') : thrustRunCard(run, running)}
        ${run ? progressCard(run) : ''}
      </div>
      <div class="cal-col cal-wide">
        ${running ? `<section class="card cal-map-card"><div class="card-head"><h2>Thrusters</h2><span class="pill live">Measuring · ${platformName(run.platform)}</span></div>
            <div class="cal-map"><canvas id="calMap" aria-label="The table seen from above: the platform and its burns"></canvas></div>
            <p class="hint">The platform, its burns so far (yellow), and where it flies back to.</p></section>`
          : thrustResults(viewing, res)}
      </div>`;
  }

  function thrustRunCard(run, running) {
    const f = running ? run.params : S.thrForm, st = getStream(), streaming = st && st.running;
    const chosen = running ? run.platform : S.platform;
    const plat = NAMES.map(n => `<button type="button" data-calplat="${n}" class="${n === chosen ? 'active' : ''}" ${running ? 'disabled' : ''}>
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const field = (key, label, min, max, step, unit, title) => `<label class="field" title="${esc(title)}"><span class="field-label">${label}${unit ? ` [${unit}]` : ''}</span>
        <input type="number" class="input" data-thrform="${key}" min="${min}" max="${max}" step="${step}" value="${f[key]}" ${running ? 'disabled' : ''}></label>`;
    const minutes = Math.round(8 * f.repeats * (Number(f.burn) + 12) / 60);
    const table = S.cal && S.cal.table;
    return `<section class="card">
        <div class="card-head"><h2>Run</h2>${running ? '<span class="pill live">Running</span>' : busyOther() ? '<span class="pill warn">Another Calibration Runs</span>'
          : streaming ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Start PhaseSpace First</span>'}</div>
        <div class="segmented cal-plat" role="group" aria-label="Platform">${plat}</div>
        <div class="cal-form">
          ${field('repeats', 'Repeats', 1, 3, 1, '', 'How many burns of each thruster (their thrusts are averaged)')}
          ${field('burn', 'Burn Time', 0.5, 3, 0.5, 's', 'How long each thruster fires (longer: more motion to measure, but more room needed)')}
          ${field('margin', 'Edge Margin', 0.25, 0.8, 0.05, 'm', 'A burn ends early when the platform comes this close to an edge')}
        </div>
        <p class="hint">${8 * f.repeats} burns of ${f.burn} s, each with a flight back: about ${minutes} min.
          ${table ? `The table's slope (calibrated ${esc(String(table.updated).slice(0, 10))}) is taken out.` : '<span class="warn-text">No table calibration: the table\'s slope is not taken out (calibrate the table first for the best thrusts).</span>'}</p>
        <div class="cal-actions">
          ${running ? '<button type="button" class="btn btn-stop" data-cal="stop">Stop (Esc)</button>'
                    : `<button type="button" class="btn btn-go" data-cal="start-thrusters" ${streaming && !busyOther() ? '' : 'disabled'}>Start Thruster Calibration…</button>`}
        </div>
      </section>`;
  }

  function busyOther() { return S.active && S.run && S.run.kind !== S.section; }

  function thrustResults(name, res) {
    const picker = NAMES.map(n => `<button type="button" data-calview="${n}" class="${n === name ? 'active' : ''}">
        <i class="swatch" style="background:${COLORS[n]}"></i>${platformName(n)}</button>`).join('');
    const head = `<div class="card-head"><h2>Thrusters</h2><div class="segmented cal-plat" role="group" aria-label="Platform">${picker}</div></div>`;
    if (!res) {
      return `<section class="card">${head}<p class="hint">No thruster calibration for the ${platformName(name)} yet.${S.local ? ' The lab console measures it; Update from Lab brings it here.' : ''}
        The result shows each thruster's thrust against nominal, which way it really pushes, and each burn.</p>
        ${thrusterDiagram({})}</section>`;
    }
    const nominal = res.nominal || 0.2825, f = res.thrust;
    const row = k => `<tr><td>T${k + 1}${res.valid[k] ? '' : ' <span class="warn-text" title="No good burn: the nominal thrust is used">·</span>'}</td>
        <td>${fmt(f[k], 4)}</td><td>${res.thrust_sd[k] != null ? fmt(res.thrust_sd[k], 4) : '—'}</td>
        <td style="color:${f[k] / nominal < 0.97 || f[k] / nominal > 1.03 ? 'var(--warn)' : 'inherit'}">${res.valid[k] ? `${f[k] >= nominal ? '+' : ''}${fmt(100 * (f[k] / nominal - 1), 1)} %` : '—'}</td>
        <td>${res.angle[k] != null ? `${fmt(res.angle[k], 1)}°` : '—'}</td>
        <td style="color:${res.torque_ratio[k] != null && Math.abs(res.torque_ratio[k] - 1) > 0.1 ? 'var(--warn)' : 'inherit'}">${res.torque_ratio[k] != null ? fmt(res.torque_ratio[k], 2) : '—'}</td></tr>`;
    const bad = res.valid.filter(v => !v).length, a = res.params && res.params.assumed;
    return `<section class="card">${head}
        <div class="cal-thr-grid">
          ${thrusterDiagram({ thrust: f, nominal, angle: res.angle })}
          <div>${thrustBars({ thrust: f, sd: res.thrust_sd, nominal, valid: res.valid })}</div>
        </div>
        <table class="cal-thr-table"><thead><tr><th>Thruster</th><th>Thrust [N]</th><th>± [N]</th><th>vs Nominal</th>
          <th title="How far the push is turned from the thruster's direction (towards the body's +z rotation)">Turned</th>
          <th title="The torque it gave over the torque the mixer expects from it (moment arm × thrust): far from 1, the CG or the inertia is off">Torque Ratio</th></tr></thead>
          <tbody>${f.map((_, k) => row(k)).join('')}</tbody></table>
        ${bad ? `<p class="hint warn-text">${bad} thruster${bad > 1 ? 's' : ''} had no good burn and keep${bad > 1 ? '' : 's'} the nominal thrust.</p>` : ''}
        ${burnPaths(res.burns || [])}
        <p class="hint">Measured ${when(res.updated)} by ${esc(res.by || '?')}, assuming ${a ? `${fmt(a.mass, 2)} kg, ${fmt(a.inertia, 4)} kg m², CG (${fmt(a.cg_mm[0], 1)}, ${fmt(a.cg_mm[1], 1)}) mm${a.changed ? ' (changed from the defaults)' : ''}` : '—'};
          ${res.table_used ? 'the table\'s slope taken out' : 'no table calibration then'}. Simulations and the lab console's controller use these thrusts (Scenario: Calibrated Thrust).</p>
        ${S.local ? '' : `<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-thrusters" title="Remove the ${platformName(name)}'s thruster calibration from the lab console's file">Remove</button></div>`}
      </section>`;
  }

  // The assumptions dialog: the platform's mass properties, which the person starting it confirms or changes
  function askThrusters() {
    const n = S.platform, a = (S.assumed || {})[n];
    if (!a) { toast('The lab console didn\'t say what it takes the platform\'s mass properties to be; reload the page.', 'error'); return; }
    const dlg = $('#calThrustDialog');
    $('#calThrustName').textContent = platformName(n);
    const set = (k, v) => { dlg.querySelector(`[data-assume="${k}"]`).value = v; };
    set('mass', a.mass); set('inertia', Number(a.inertia).toFixed(5)); set('cgx', a.cg_mm[0]); set('cgy', a.cg_mm[1]);
    const t = S.cal && S.cal.table;
    $('#calThrustTable').innerHTML = t ? `The table's slope, calibrated ${esc(String(t.updated).slice(0, 10))}, is taken out of each burn.`
      : '<span class="warn-text">There is no table calibration, so the table\'s slope is not taken out: calibrate the table first for the best thrusts.</span>';
    $('#calThrustAck').checked = false;
    $('#calThrustGo').disabled = true;
    dlg.showModal();
  }
  $('#calThrustAck').onchange = ev => { $('#calThrustGo').disabled = !ev.target.checked; };
  $('#calThrustGo').onclick = () => act(async () => {
    const dlg = $('#calThrustDialog'), v = k => Number(dlg.querySelector(`[data-assume="${k}"]`).value);
    readThrForm();
    dlg.close();
    onRun(await api('/api/calibration/thrusters/start', {
      platform: S.platform, acknowledged: true, ...S.thrForm,
      assumed: { mass: v('mass'), inertia: v('inertia'), cg_mm: [v('cgx'), v('cgy')] } }));
  });

  function readThrForm() {
    for (const el of body.querySelectorAll('[data-thrform]')) S.thrForm[el.dataset.thrform] = Number(el.value);
    storage.set('spot.calibration.thrusters', S.thrForm);
  }

  // ---------------------------------------------------------------- the arm

  function armSection() {
    const run = S.run && S.run.kind === 'arm' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    const now = running && run.step != null ? run.steps[run.step] : null;
    if (now && now.test) S.armDemo = { test: now.test, joint: now.joint };
    const stepper = ARM_TESTS.map(([key, title, text], k) => {
      const cls = S.armDemo.test === key ? 'now' : '';
      return `<li class="cal-step ${cls}" data-armdemo="${key}" style="cursor:pointer" title="Show this test on the arm">
        <svg viewBox="0 0 24 24" aria-hidden="true">${['<path d="M4 18h16M12 18V8M8 11l4-4 4 4"/>', '<path d="M4 12c3-6 5-6 8 0s5 6 8 0"/>', '<path d="M5 18 19 6M13 6h6v6"/>', '<path d="M4 16h4v-6h4v8h4v-4h4"/>'][k]}</svg>
        <div><b>${k + 1}. ${title}</b><span>${text}</span></div></li>`;
    }).join('');
    const howCard = `<section class="card"><div class="card-head"><h2>How It Works</h2>${now && now.test ? `<span class="pill live">${JOINTS[now.joint]} · ${now.test}</span>` : ''}</div>
        <div id="calArmFig">${armFigure(S.armDemo.test, S.armDemo.joint, 0)}</div>
        <ol class="cal-steps">${stepper}</ol>
        <p class="hint">For each joint in turn, the other joints hold their position. The script (resources/arm_calibration/calibrate_arm.py)
          runs on the Chaser, which has the arm's U2D2; it fits each joint's friction and inertia, which SPOT's arm model then uses
          (spot.arm.parameters). The Chaser rests on the table throughout.</p></section>`;
    const res = armResult(run);
    return `<div class="cal-col">
        ${S.local ? syncCard() : ''}
        ${howCard}
        ${S.local ? labOnlyCard('arm') : armChecksCard() + armRunCard(run, running)}
        ${run ? armProgressCard(run) : ''}
      </div>
      <div class="cal-col cal-wide">${armResults(res)}</div>`;
  }

  function armChecksCard() {
    const c = S.armCheck;
    let body;
    if (S.armChecking) body = '<p class="hint">Asking the Chaser…</p>';
    else if (!c) body = '<p class="hint">Not checked yet.</p>';
    else if (!c.reachable) body = `<p class="hint bad">The Chaser doesn't answer: ${esc(c.error)}</p>`;
    else {
      body = `<ul class="cal-checks">${c.items.map(i => {
        const mark = !i.ok ? '<span class="n">✕</span>' : i.warn ? '<span class="w">!</span>' : '<span class="y">✓</span>';
        return `<li>${mark}<b>${esc(i.what)}</b><span class="mono">${esc(i.value)}</span>${!i.ok || i.warn ? `<span class="fix">${esc(i.fix)}</span>` : ''}</li>`;
      }).join('')}</ul>${c.running ? '<p class="hint warn-text">A calibration is already running on the Chaser.</p>' : ''}`;
    }
    const pill = !c ? '' : c.ok ? '<span class="pill ok">Ready</span>' : '<span class="pill warn">Not Ready</span>';
    return `<section class="card"><div class="card-head"><h2>The Chaser's Setup</h2>${pill}</div>${body}
        <div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="arm-check" ${S.armChecking ? 'disabled' : ''}>Check Again</button></div>
        <p class="hint">What calibrate_arm.py needs on the Chaser. A rehearsal (simulated servos) needs none of it.</p></section>`;
  }

  function armRunCard(run, running) {
    const f = running ? run.params : S.armForm;
    const chip = (kind, value, label, on) => `<button type="button" class="cal-chip${on ? ' on' : ''}" data-arm${kind}="${value}" ${running ? 'disabled' : ''}>${label}</button>`;
    const minutes = Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
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
        </div></section>`;
  }

  function armProgressCard(run) {
    const tests = run.steps.filter(s => s.test), done = tests.filter(s => s.state === 'done').length;
    const pct = tests.length ? Math.round(100 * done / tests.length) : 0;
    return progressCard(run).replace('<ol class="cal-progress">', `<div class="cal-bar-progress"><i style="width:${pct}%"></i></div><ol class="cal-progress">`)
      .replace(/<pre class="cal-log">[\s\S]*?<\/pre>/, `<pre class="cal-log">${esc((run.armlog || []).filter(l => !l.startsWith('PROGRESS ')).join('\n') || run.log.join('\n'))}</pre>`);
  }

  // What to show: a rehearsal just run (it isn't saved), else the lab's arm calibration
  function armResult(run) {
    if (run && run.result && run.result.format === 'spot-arm-calibration' && run.result.simulated) return run.result;
    return S.cal && S.cal.arm;
  }

  function armResults(res) {
    if (!res) {
      return `<section class="card"><div class="card-head"><h2>Arm</h2><span class="pill">Not Calibrated</span></div>
        <p class="hint">No arm calibration yet.${S.local ? ' The lab console measures it; Update from Lab brings it here.' : ''}
          The result shows each joint's friction (its breakaway torque, and the friction against speed with the fitted curve), its
          inertia, and the position steps against the goals. Until then SPOT's arm model uses placeholder friction.</p></section>`;
    }
    const joints = [].concat(res.joints || []), plots = res.plots || {};
    const row = j => `<tr><td>${esc(JOINTS[j.index] || j.name)}</td><td>${fmt(j.static, 3)}</td>
        <td>${j.static_pos != null ? `+${fmt(j.static_pos, 3)} / −${fmt(j.static_neg, 3)}` : '—'}</td>
        <td>${fmt(j.coulomb, 3)}</td><td>${fmt(j.stribeck, 3)}</td><td>${fmt(j.viscous, 3)}</td>
        <td>${j.inertia != null ? j.inertia.toExponential(2) : '—'}</td></tr>`;
    const sj = joints.some(j => j.index === S.armStepsJoint) ? S.armStepsJoint : (joints[0] || {}).index;
    const tabs = joints.map(j => `<button type="button" class="cal-chip${j.index === sj ? ' on' : ''}" data-armsteps="${j.index}">${JOINTS[j.index]}</button>`).join('');
    const label = res.simulated ? '<span class="pill warn">Rehearsal · Not Saved</span>' : `<span class="pill ok">${when(res.updated)}</span>`;
    return `<section class="card"><div class="card-head"><h2>Arm</h2>${label}</div>
        <table class="cal-thr-table"><thead><tr><th>Joint</th><th title="Breakaway torque">Static [N m]</th><th>+ / − [N m]</th>
          <th title="Sliding friction">Coulomb [N m]</th><th title="How quickly friction falls from static to Coulomb with speed">Stribeck [rad/s]</th>
          <th>Viscous [N m s/rad]</th><th title="About the joint, link and rotor">Inertia [kg m²]</th></tr></thead>
          <tbody>${joints.map(row).join('')}</tbody></table>
        <div class="cal-arm-plots">
          <div><h3 class="cal-sub">Friction against Speed</h3>${frictionPlot(joints, plots)}</div>
          <div><h3 class="cal-sub">Position Steps</h3><div class="cal-tabs-mini">${tabs}</div>${stepsPlot((plots[String(sj)] || {}).steps)}</div>
        </div>
        <p class="hint">${res.simulated ? 'A rehearsal with the script\'s simulated servos: the lab\'s arm calibration is unchanged.'
          : `Measured ${when(res.updated)} by ${esc(res.by || '?')} (servos: ${esc(Object.values(res.servos || {}).join(', ') || '?')}, supply ${fmt(res.supply_voltage, 1)} V).
             Simulations use it with the Scenario's Calibrated Arm; the raw data stays on the lab console (${esc(res.lab_folder || '')}).`}</p>
        ${S.local || res.simulated ? '' : '<div class="cal-actions"><button type="button" class="btn btn-sm" data-cal="clear-arm" title="Remove the arm calibration from the lab console\'s file">Remove</button></div>'}
      </section>`;
  }

  // The arm figure, animated with the test shown (the stepper's, or the run's)
  let armRaf = 0, armT0 = 0;
  function animateArm() {
    cancelAnimationFrame(armRaf);
    if (!S.shown || S.section !== 'arm') return;
    armT0 = performance.now();
    const frame = now => {
      const el = $('#calArmFig');
      if (!el || !S.shown || S.section !== 'arm') return;
      el.innerHTML = armFigure(S.armDemo.test, S.armDemo.joint, (now - armT0) / 1000);
      armRaf = requestAnimationFrame(frame);
    };
    armRaf = requestAnimationFrame(frame);
  }

  async function checkArm() {
    S.armChecking = true;
    if (S.shown && S.section === 'arm') render();
    try { S.armCheck = await api('/api/calibration/arm/check'); }
    finally { S.armChecking = false; if (S.shown && S.section === 'arm') render(); }
  }

  function startArm() {
    const f = S.armForm;
    if (f.simulate) return act(async () => onRun(await api('/api/calibration/arm/start', { ...f })));
    const minutes = Math.max(1, Math.round(f.joints.length * f.tests.reduce((m, t) => m + ARM_MINUTES[t], 0)));
    $('#calArmTime').textContent = `${f.joints.map(j => JOINTS[j]).join(', ')}: ${f.tests.join(', ')}; about ${minutes} min.`;
    $('#calArmAck').checked = false;
    $('#calArmGo').disabled = true;
    $('#calArmDialog').showModal();
  }
  $('#calArmAck').onchange = ev => { $('#calArmGo').disabled = !ev.target.checked; };
  $('#calArmGo').onclick = () => act(async () => {
    $('#calArmDialog').close();
    onRun(await api('/api/calibration/arm/start', { ...S.armForm, acknowledged: true }));
  });

  // The map (the table seen from above), redrawn when anything on it changes
  function draw() {
    const canvas = $('#calMap');
    if (!canvas) return;
    if (S.section === 'thrusters') {   // a thruster run under way: the platform, its burns, where it flies back to
      const r = S.run;
      if (!r) return;
      fitMap(canvas.parentElement);
      drawTable(canvas, { cal: S.cal && S.cal.table, paths: r.records || [], live: { pose: r.pose, path: r.path, goal: r.goal },
                          color: COLORS[r.platform] });
      return;
    }
    const t = S.cal && S.cal.table;
    const run = S.run && S.run.kind === 'table' ? S.run : null;
    const running = run && ['running', 'stopping'].includes(run.state);
    let paths = t ? (t.paths || []) : [];
    let live = null, points = null, current = null;
    if (running) {   // the run under way: its drifts so far, the grid, the platform
      paths = run.records || [];
      live = { pose: run.pose, path: run.path, goal: run.goal };
      points = run.points;
      const name = run.step != null ? run.steps[run.step].name : '';
      current = name.startsWith('Point ') ? Number(name.slice(6)) - 1 : null;
    }
    fitMap(canvas.parentElement);
    drawTable(canvas, { cal: running ? null : t, paths, live, points, current, color: COLORS[run ? run.platform : S.platform] });
  }

  // The map as large as the window allows with its whole card in view: its height is what is left below its
  // top, less what the card has under it (the colour bar, the numbers); its width follows the table's
  // proportions. (A narrow window lets the page scroll: full width.)
  function fitMap(map) {
    const scroll = $('.cal-scroll'), card = map.closest('.cal-map-card');
    map.style.width = '';
    if (!scroll || !card || window.innerWidth <= 900) return;
    const m = map.getBoundingClientRect();
    const below = card.getBoundingClientRect().bottom - m.bottom;
    const top = m.top - scroll.getBoundingClientRect().top + scroll.scrollTop;
    const height = Math.max(240, scroll.clientHeight - top - below - 24);
    if (height < m.height) map.style.width = `${Math.floor(height * 3.9 / 2.9)}px`;
  }
  new ResizeObserver(() => draw()).observe(body);
  window.addEventListener('resize', () => draw());

  // ---------------------------------------------------------------- the sync prompt (local console)

  function maybeAsk() {
    if (!S.local || !S.lab || !S.lab.reachable || !S.labCal || !S.labCal.updated || !S.cal) return;
    const mine = S.cal, lab = S.labCal;
    if (mine.updated && lab.updated <= mine.updated) return;
    if (mine.sync && (mine.sync.ask === false || mine.sync.declined === lab.updated)) return;
    if (S.asked === lab.updated) return;
    S.asked = lab.updated;
    const dialog = $('#calSyncDialog');
    $('#calSyncList').innerHTML = newerSections(lab, mine).map(([what, t]) => `<li><b>${what}</b><span>${when(t)}</span></li>`).join('');
    $('#calSyncNever').checked = false;
    dialog.showModal();
  }

  function newerSections(lab, mine) {
    const out = [], newer = (a, b) => a && (!b || (a.updated || '') > (b.updated || ''));
    if (newer(lab.table, mine.table)) out.push(['Table Slope', lab.table.updated]);
    for (const n of NAMES) {
      const a = (lab.thrusters || {})[n], b = (mine.thrusters || {})[n];
      if (newer(a, b)) out.push([`${platformName(n)} Thrusters`, a.updated]);
    }
    if (newer(lab.arm, mine.arm)) out.push(['Arm', lab.arm.updated]);
    return out;
  }

  $('#calSyncUpdate').onclick = () => act(async () => {
    const never = $('#calSyncNever').checked;
    $('#calSyncDialog').close();
    onCalibration(await api('/api/calibration/take', {}));
    if (never) onCalibration(await api('/api/calibration/sync', { ask: false }));
    toast('This project now has the lab\'s calibration; the next simulation uses it.', 'success', 5000);
  });
  $('#calSyncLater').onclick = () => act(async () => {
    const never = $('#calSyncNever').checked, lab = S.labCal;
    $('#calSyncDialog').close();
    onCalibration(await api('/api/calibration/sync', never ? { ask: false } : { declined: lab.updated }));
    toast(never ? 'The lab\'s calibration won\'t be offered again (Calibration tab: Ask When the Lab\'s Is Newer turns it back on).'
                : 'Kept this project\'s calibration. The Calibration tab can update it later.', 'info', 6000);
  });

  // ---------------------------------------------------------------- input

  document.addEventListener('click', ev => {
    const b = ev.target.closest('[data-calsec]');
    if (b) {
      S.section = b.dataset.calsec;
      storage.set('spot.calibration.section', S.section);
      render();
      if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) act(checkArm);
    }
  });
  body.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (!b || b.disabled) return;
    if (b.dataset.calplat) {
      S.platform = b.dataset.calplat;
      storage.set('spot.calibration.platform', S.platform);
      render();
    } else if (b.dataset.calview) {
      S.platform = b.dataset.calview;
      storage.set('spot.calibration.platform', S.platform);
      render();
    } else if (b.dataset.cal === 'start-thrusters') {
      readThrForm();
      askThrusters();
    } else if (b.dataset.cal === 'clear-thrusters') {
      if (!confirm(`Remove the ${platformName(S.platform)}'s thruster calibration from the lab console? Projects keep their own copies.`)) return;
      onCalibration(await api('/api/calibration/clear', { section: 'thrusters', platform: S.platform }));
    } else if (b.dataset.armjoint || b.dataset.armtest) {
      const f = S.armForm, key = b.dataset.armjoint ? 'joints' : 'tests';
      const v = b.dataset.armjoint ? Number(b.dataset.armjoint) : b.dataset.armtest;
      f[key] = f[key].includes(v) ? f[key].filter(x => x !== v) : [...f[key], v];
      if (key === 'joints') f.joints.sort(); else f.tests = ARM_TESTS.map(t => t[0]).filter(t => f.tests.includes(t));
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
      if (!confirm('Remove the arm calibration from the lab console? Projects keep their own copies.')) return;
      onCalibration(await api('/api/calibration/clear', { section: 'arm' }));
    } else if (b.dataset.cal === 'start-table') {
      readForm();
      const r = await api('/api/calibration/table/start', { platform: S.platform, ...S.form });
      onRun(r);
    } else if (b.dataset.cal === 'stop') {
      onRun(await api('/api/calibration/stop', {}));
    } else if (b.dataset.cal === 'clear-table') {
      if (!confirm('Remove the table calibration from the lab console? Projects keep their own copies.')) return;
      onCalibration(await api('/api/calibration/clear', { section: 'table' }));
    }
  }));
  body.addEventListener('change', ev => {
    if (ev.target.dataset.calform) readForm();
    if (ev.target.dataset.thrform) readThrForm();
    if (ev.target.matches('[data-armsim]')) {
      S.armForm.simulate = ev.target.checked;
      storage.set('spot.calibration.arm', S.armForm);
      render();
    }
  });
  body.addEventListener('click', ev => {   // the stepper: show that test on the arm
    const li = ev.target.closest('[data-armdemo]');
    if (li && !(S.run && S.run.kind === 'arm' && S.active)) { S.armDemo = { test: li.dataset.armdemo, joint: S.armDemo.joint }; render(); }
  });
  head.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (b && b.dataset.cal === 'take' && !b.disabled) {
      onCalibration(await api('/api/calibration/take', {}));
      toast('This project now has the lab\'s calibration; the next simulation uses it.', 'success', 5000);
    }
  }));
  head.addEventListener('change', ev => act(async () => {
    if (ev.target.dataset.cal === 'ask') onCalibration(await api('/api/calibration/sync', { ask: ev.target.checked }));
  }));
  document.addEventListener('keydown', ev => {
    if (ev.key === 'Escape' && S.shown && S.active && !S.local) act(async () => onRun(await api('/api/calibration/stop', {})));
  });

  function readForm() {
    for (const el of body.querySelectorAll('[data-calform]')) {
      S.form[el.dataset.calform] = el.tagName === 'SELECT' ? el.value : Number(el.value);
    }
    storage.set('spot.calibration.table', S.form);
  }

  // ---------------------------------------------------------------- from the server

  function onCalibration(d) {
    if (!d || !d.calibration) return;
    S.cal = d.calibration;
    S.path = d.path;
    viewer?.setTableSlope(S.cal.table || null);
    document.dispatchEvent(new CustomEvent('spot-calibration', { detail: S.cal }));
    if (S.shown) render();
    maybeAsk();
  }

  let lastRender = 0;
  function onRun(d) {
    if (!d || !('active' in d)) return;
    const before = S.run;
    S.run = d.run;
    S.active = d.active;
    // the page re-renders when the run's steps change, and redraws the map for the platform's moves
    const restructure = !before || !S.run || before.state !== S.run.state || before.step !== S.run.step ||
      before.message !== S.run.message || (before.records || []).length !== (S.run.records || []).length;
    if (!S.shown) return;
    const now = performance.now();
    if (restructure || now - lastRender > 1000) { lastRender = now; render(); } else draw();
  }

  return {
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
    onLab(l) {
      S.lab = l;
      if (l && !l.reachable) S.labCal = null;
      else if (l && l.snapshot && l.snapshot.calibration) S.labCal = l.snapshot.calibration.calibration;
      if (S.shown) render();
      maybeAsk();
    },
    onLabCalibration(d) { S.labCal = d && d.calibration; if (S.shown) render(); maybeAsk(); },
    onStream() { if (S.shown && !S.local) render(); },
    show() {
      S.shown = true;
      render();
      if (!S.local && S.section === 'arm' && !S.armCheck && !S.armChecking) act(checkArm);
    },
    hide() { S.shown = false; },
    hasTable: () => Boolean(S.cal && S.cal.table),
  };
}
