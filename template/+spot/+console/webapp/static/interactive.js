// Interactive Mode (lab console, PhaseSpace tab): fly one or more platforms, each to a goal pose
// dragged on the table. Each platform has a switch: switched on, its goal appears on the table;
// Fly to goals sends every switched-on platform to its goal at once. The lab console closes each
// platform's loop with the template diagram's own controller (spot_console/control.py, a port of
// spot.control.lqrCommand and spot.control.thrusterDutyCycles, see spot_console/interactive.py);
// this page only picks the platforms, moves the goals, keeps the flight alive with a heartbeat,
// and shows what the loops are doing.

import { COLORS, platformName } from './platforms.js';

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];

const NAMES = ['RED', 'BLACK', 'BLUE'];
const HEARTBEAT_MS = 500;     // the lab console stops the flight after 3 s without one
const GOAL_SEND_MS = 100;     // goal updates while dragging, at most this often (per platform)
const wrapDeg = rad => { const d = rad * 180 / Math.PI; return ((d + 180) % 360 + 360) % 360 - 180; };
const fmt = (v, digits) => (v == null || !Number.isFinite(v) ? '—' : v.toFixed(digits));
const listNames = names => {
  const n = names.map(platformName);
  return n.length < 2 ? `the ${n[0] || ''}` : `the ${n.slice(0, -1).join(', the ')} and the ${n[n.length - 1]}`;
};

// This page's id for the lab console, kept for the tab (a reload keeps flying what it flew)
const CLIENT = (() => {
  const make = () => Array.from(crypto.getRandomValues(new Uint8Array(12)), b => b.toString(16).padStart(2, '0')).join('');
  try {
    let id = sessionStorage.getItem('spot.ixClient');
    if (!id) { id = make(); sessionStorage.setItem('spot.ixClient', id); }
    return id;
  } catch { return make(); }
})();

export function initInteractive({ api, act, toast, viewer, isShown }) {
  const S = { on: false, enabled: {}, goals: {}, status: { active: false }, info: null, frame: null,
              stream: {}, hardware: {}, send: {}, dragging: {}, goalError: {}, beat: null,
              endedSeen: null, endQueue: new Map(), endTimer: null, renderedAt: 0 };
  for (const n of NAMES) S.send[n] = { timer: null, at: 0 };

  const st = () => S.status;
  const mine = () => Boolean(st().active && st().client === CLIENT);
  const other = () => Boolean(st().active && st().client !== CLIENT);
  const flying = n => Boolean(st().active && st().platforms && st().platforms[n]);
  const craft = n => S.frame && S.frame.crafts && S.frame.crafts[n];
  const tracked = n => { const c = craft(n); return Boolean(c && c.tracked && c.x != null && c.y != null); };
  const poseOf = n => { const c = craft(n); return [c.x, c.y, wrapDeg(c.att || 0)]; };
  const pending = () => NAMES.filter(n => S.enabled[n] && !flying(n));

  function blocker(n) {
    // [short, long]: why platform N can't start flying now, or null
    const hw = S.hardware[n], pn = platformName(n);
    if (!S.stream.running) return ['No Stream', 'Press Start Streaming first: the platforms are flown from their PhaseSpace poses.'];
    if (!tracked(n)) return ['Not Seen', `PhaseSpace doesn't see the ${pn} (on the table, LEDs on?).`];
    if (!hw || hw.probe.state === 'checking') return ['Checking', `Checking the ${pn}…`];
    if (hw.probe.state !== 'ok') return ['Offline', `The ${pn} can't be reached (see the Hardware tab).`];
    if (!hw.probe.teensy) return ['No Teensy', `The ${pn} has no Teensy (see the Hardware tab).`];
    if (hw.probe.others.length) return ['In Use', `The ${pn}'s Teensy is in use by ${hw.probe.others.join(', ')}.`];
    if (hw.check) return ['Check Running', `A thruster check is running on the ${pn}.`];
    if (!S.goals[n]) return ['No Goal', `The ${pn} has no goal yet.`];
    return null;
  }

  function separation() {
    // The first pair of goals (or a goal and a platform resting on the table) too close together,
    // as a sentence, or null. The lab console checks the same.
    const min = S.info ? S.info.min_separation : 0.45;
    const pts = [];
    for (const n of NAMES) {
      const g = flying(n) && !mine() ? st().platforms[n].goal : (S.enabled[n] || flying(n)) ? S.goals[n] : null;
      if (g) pts.push([n, g[0], g[1], 'goal']);
      else if (tracked(n)) { const p = poseOf(n); pts.push([n, p[0], p[1], 'resting place']); }
    }
    for (let i = 0; i < pts.length; i++) {
      for (let j = i + 1; j < pts.length; j++) {
        const [a, xa, ya, wa] = pts[i], [b, xb, yb, wb] = pts[j];
        if (wa !== 'goal' && wb !== 'goal') continue;
        const d = Math.hypot(xa - xb, ya - yb);
        if (d < min) {
          const [g, o, w] = wa === 'goal' ? [a, b, wb] : [b, a, wa];
          return `The ${platformName(g)}'s goal is ${d.toFixed(2)} m from the ${platformName(o)}'s ${w}: keep them at least ${min} m apart.`;
        }
      }
    }
    return null;
  }

  // ---------------------------------------------------------------- the card

  function renderController() {
    const info = S.info;
    const box = $('#ixController');
    if (!info || box.dataset.done) return;
    box.dataset.done = '1';
    const k3 = v => (Math.abs(v) < 1e-9 ? '0' : Number(v.toPrecision(3)).toString());
    const rows = Object.entries(info.platforms).map(([name, p]) => `
      <div class="ix-ctl">
        <b>${platformName(name)}</b>
        <span>mass ${p.mass.toFixed(2)} kg · inertia ${p.inertia.toFixed(4)} kg·m² · thrust ${Array.isArray(p.thrust) ? `${Math.min(...p.thrust).toFixed(3)}–${Math.max(...p.thrust).toFixed(3)} N (calibrated)` : `${p.thrust} N per thruster`}</span>
        <span>Q [x y θ ẋ ẏ θ̇] = [${p.Q.join(' ')}] · R [Fx Fy Tz] = [${p.R.join(' ')}]</span>
        <table class="ix-k" title="u = −K·(measured − goal)"><tbody>${p.K.map(r => `<tr>${r.map(v => `<td>${k3(v)}</td>`).join('')}</tr>`).join('')}</tbody></table>
      </div>`).join('');
    box.innerHTML = `
      <p class="hint">The template diagram's default controller, run by this console for each platform every ${info.period} s
        (the diagram's baseRate, as often as its controller and thruster code run): its LQR law (<span class="mono">spot.control.lqrCommand</span>)
        and thruster mixer (<span class="mono">spot.control.thrusterDutyCycles</span>), ported line by line to
        <span class="mono">spot_console/control.py</span> and checked against the diagram by
        <span class="mono">spot.control.verifyConsoleController</span> in MATLAB. SPOT's default settings
        (gains for a ${info.sample_rate} Hz diagram; mixer: ${info.max_iters} iterations, tolerance ${info.tol}).
        Each platform flies straight at its goal and knows nothing of the others: goals are kept
        ${info.min_separation} m apart, but keep their paths clear of each other too.</p>
      ${rows}`;
  }

  function renderRow(n) {
    const row = $(`[data-ixrow=${n}]`), s = st(), f = flying(n) ? s.platforms[n] : null, oth = other();
    const on = Boolean(S.enabled[n] || f);
    row.classList.toggle('on', on);
    const sw = $('[data-ixon]', row);
    sw.checked = on;
    sw.disabled = oth || (!on && !tracked(n));
    sw.parentElement.title = oth ? `Interactive Mode is flying for ${s.by}`
      : !on && !tracked(n) ? `PhaseSpace doesn't see the ${platformName(n)}`
      : `Give the ${platformName(n)} a goal on the table; Fly to Goals sends it there. Switching it off while it flies stops it (everything off).`;
    const pill = $('[data-role=state]', row);
    const why = on && !f ? blocker(n) : null;
    const [text, kind] = f ? (f.lost ? ['Lost', 'warn'] : ['Flying', 'live'])
      : !on ? ['', ''] : why ? [why[0], 'warn'] : ['Ready', 'ok'];
    pill.textContent = text;
    pill.className = `pill ${kind}`;
    pill.hidden = !text;
    pill.title = why ? why[1] : f && f.late ? `${f.late} late steps` : '';
    $('[data-role=here]', row).hidden = !on || oth || !tracked(n);
    const g = f && !mine() ? f.goal : S.goals[n];
    $('[data-role=goal]', row).textContent = g ? `goal ${fmt(g[0], 3)}, ${fmt(g[1], 3)} m · ${fmt(g[2], 1)}°` : 'goal —';
    const e = f && f.error;
    $('[data-role=error]', row).textContent = e ? `error ${fmt(Math.hypot(e[0], e[1]) * 1000, 0)} mm · ${fmt(e[2], 1)}°`
      : f ? 'error —' : 'not flying';
    $$('[data-role=bars] span', row).forEach((bar, k) => {
      const d = f ? f.duty[k] : 0;
      bar.firstElementChild.style.height = `${Math.max(0, Math.min(100, d))}%`;
      bar.title = `T${k + 1}: ${fmt(d, 0)} %`;
    });
  }

  function render() {
    S.renderedAt = performance.now();
    $('#ixBody').hidden = !S.on;
    $('#toggleInteractive').checked = S.on;
    const s = st(), own = mine(), oth = other();
    for (const n of NAMES) renderRow(n);

    const todo = oth ? [] : pending();
    const blocked = todo.map(n => [n, blocker(n)]).filter(([, b]) => b);
    const sep = oth ? null : separation();
    const fly = $('#ixFly');
    fly.disabled = !todo.length || blocked.length > 0 || Boolean(sep);
    fly.textContent = !todo.length ? 'Fly to Goals' : own ? `Fly ${listNames(todo)} Too` : `Fly ${listNames(todo)}`;
    fly.hidden = oth;
    $('#ixStop').hidden = !s.active;

    const flyingNames = s.active ? Object.keys(s.platforms) : [];
    const lost = flyingNames.filter(n => s.platforms[n].lost);
    const goalErr = NAMES.map(n => S.goalError[n]).find(Boolean);
    let hint, warn = false;
    if (oth) hint = `Flying ${listNames(flyingNames)} for ${s.by}. Anyone can stop it.`;
    else if (goalErr) { hint = goalErr; warn = true; }
    else if (sep) { hint = sep; warn = true; }
    else if (lost.length) { hint = `PhaseSpace has lost ${listNames(lost)}: thrusters off until it sees it again.`; warn = true; }
    else if (blocked.length) { hint = blocked[0][1][1]; warn = true; }
    else if (own) hint = 'Drag a goal to move its platform; Shift+drag turns it. A platform\'s switch stops it; Esc stops all.'
      + (todo.length ? ` Fly to Goals adds ${listNames(todo)}.` : '');
    else if (!todo.length) hint = 'Switch on the platforms to fly (or click them on the table), then drag their goals.';
    else hint = 'Place the goals (drag; Shift+drag turns), then Fly to Goals: the pucks come on and each platform flies straight to its goal. Keep their paths clear of each other.';
    const h = $('#ixHint');
    h.textContent = hint;
    h.className = `hint ${warn ? 'warn-text' : ''}`;

    const end = s.ended && s.ended[s.ended.length - 1];
    $('#ixStats').textContent = s.active
      ? `step ${flyingNames.map(n => `${platformName(n)} ${fmt(s.platforms[n].step_ms, 1)} ms${s.platforms[n].late ? ` (${s.platforms[n].late} late)` : ''}`).join(' · ')} · every ${s.period} s`
      : end ? `Last: the ${platformName(end.platform)} ended: ${end.reason}` : '';
    renderController();
    renderView();
  }

  function renderView() {
    if (!viewer) return;
    const s = st(), oth = other();
    for (const n of NAMES) viewer.setThrust(n, S.on && flying(n) ? s.platforms[n].duty : null);
    if (!S.on) { viewer.setGoals({}); return; }
    const goals = {};
    for (const n of NAMES) {
      if (oth) { if (flying(n)) goals[n] = { pose: s.platforms[n].goal, editable: false }; }
      else if ((S.enabled[n] || flying(n)) && S.goals[n]) goals[n] = { pose: S.goals[n], editable: true };
    }
    viewer.setGoals({ goals, pickable: !oth, margin: S.info ? S.info.margin : null });
  }

  const renderSoon = () => { if (performance.now() - S.renderedAt > 200) render(); };

  // ---------------------------------------------------------------- actions

  async function enable(n, on) {
    if (other()) return;
    if (on) {
      S.enabled[n] = true;
      if (!S.goals[n] && tracked(n)) S.goals[n] = poseOf(n);
    } else {
      S.enabled[n] = false;
      delete S.goals[n];
      delete S.goalError[n];
      if (flying(n) && mine()) onStatus(await api('/api/interactive/stop', { platform: n }));
    }
    render();
  }

  function sendGoal(n, final) {
    // While flying from this page: platform N's goal to the lab console (throttled while dragging)
    const q = S.send[n];
    clearTimeout(q.timer);
    q.timer = null;
    if (!mine() || !flying(n) || !S.goals[n]) return;
    const wait = GOAL_SEND_MS - (performance.now() - q.at);
    if (!final && wait > 0) { q.timer = setTimeout(() => sendGoal(n, false), wait); return; }
    q.at = performance.now();
    const [x, y, att] = S.goals[n];
    api('/api/interactive/goal', { client: CLIENT, platform: n, x, y, att, final })
      .then(() => { if (S.goalError[n]) { delete S.goalError[n]; render(); } })
      .catch(e => {
        // e.g. too close to another goal: the lab console keeps flying to the last good one
        if (final) {
          toast(e.message, 'error');
          delete S.goalError[n];
          if (flying(n)) S.goals[n] = st().platforms[n].goal;   // back to where it is really going
        } else {
          S.goalError[n] = e.message;   // shown while dragging
        }
        render();
      });
  }

  async function fly() {
    const todo = pending();
    if (!todo.length) return;
    const platforms = {};
    for (const n of todo) { const [x, y, att] = S.goals[n]; platforms[n] = { x, y, att }; }
    onStatus(await api('/api/interactive/start', { platforms, client: CLIENT }));
  }

  async function stopAll() {
    onStatus(await api('/api/interactive/stop', {}));
  }

  function heartbeat(on) {
    if (on && !S.beat) {
      S.beat = setInterval(() => {
        api('/api/interactive/heartbeat', { client: CLIENT }).catch(() => { /* the lab console stops it */ });
      }, HEARTBEAT_MS);
    } else if (!on && S.beat) {
      clearInterval(S.beat);
      S.beat = null;
    }
  }

  // ---------------------------------------------------------------- from the lab console

  function onStatus(status) {
    const wasMine = mine();
    S.status = status || { active: false };
    const s = st(), own = mine();
    if (own) {
      for (const n of Object.keys(s.platforms)) {
        const q = S.send[n];
        if (!wasMine || !S.goals[n]) {
          // Flying from this page (also after a reload): show the controls, take the goals
          S.on = true;
          S.enabled[n] = true;
          S.goals[n] = s.platforms[n].goal;
        } else if (!S.dragging[n] && !q.timer && performance.now() - q.at > 500 && !S.goalError[n]) {
          S.goals[n] = s.platforms[n].goal;   // as the lab console has it (e.g. kept inside the margin)
        }
      }
    }
    heartbeat(own);
    // Tell about platforms that stopped (not those already seen when the page opened)
    const ended = s.ended || [];
    if (S.endedSeen == null) S.endedSeen = ended.length ? ended[ended.length - 1].time : 0;
    for (const e of ended) {
      if (e.time <= S.endedSeen) continue;
      S.endedSeen = e.time;
      if (S.on || wasMine) {
        // Platforms that stop together arrive one event each: one toast per reason, a moment later
        S.endQueue.set(e.reason, [...(S.endQueue.get(e.reason) || []), e.platform]);
        clearTimeout(S.endTimer);
        S.endTimer = setTimeout(() => {
          for (const [reason, names] of S.endQueue) {
            toast(`Interactive Mode stopped ${listNames(names)}: ${reason}`, /^stopped by/.test(reason) ? 'info' : 'error', 9000);
          }
          S.endQueue.clear();
        }, 300);
      }
    }
    render();
  }

  function setState(status, info) {
    if (info) {   // (new parameters, e.g. a new thruster calibration: the Controller text is drawn again)
      S.info = info;
      delete $('#ixController').dataset.done;
    }
    onStatus(status);
  }

  function onFrame(frame) {
    S.frame = frame;
    if (!S.on) return;
    for (const n of NAMES) if (S.enabled[n] && !S.goals[n] && tracked(n)) S.goals[n] = poseOf(n);
    renderSoon();
  }

  // ---------------------------------------------------------------- wiring

  for (const n of NAMES) {
    const row = $(`[data-ixrow=${n}]`);
    $('.swatch', row).style.background = COLORS[n];
    $('[data-ixon]', row).onchange = e => act(() => enable(n, e.target.checked));
    $('[data-role=here]', row).onclick = () => {
      if (!tracked(n)) return;
      S.goals[n] = poseOf(n);
      sendGoal(n, true);
      render();
    };
  }
  $('#toggleInteractive').onchange = () => act(async () => {
    S.on = $('#toggleInteractive').checked;
    if (!S.on && mine()) await stopAll();
    render();
  });
  $('#ixFly').onclick = () => act(fly);
  $('#ixStop').onclick = () => act(stopAll);
  if (viewer) {
    viewer.goal.onPick = n => { if (S.on && !S.enabled[n]) act(() => enable(n, true)); };
    viewer.goal.onDrag = (n, x, y, att, done) => {
      S.goals[n] = [x, y, att];
      S.dragging[n] = !done;
      sendGoal(n, done);
      if (done) render(); else renderSoon();
    };
  }
  // Esc on the PhaseSpace tab stops every platform (whoever started them)
  document.addEventListener('keydown', e => {
    if (e.key === 'Escape' && st().active && isShown() && !document.querySelector('dialog[open]')) act(stopAll);
  });

  return {
    setState, onStatus, onFrame,
    onStream(s) { S.stream = s || {}; if (S.on) render(); },
    onHardware(hw) { for (const p of hw.platforms) S.hardware[p.name] = p; if (S.on) render(); },
    onHardwarePlatform(p) { S.hardware[p.name] = p; if (S.on) renderSoon(); },
  };
}
