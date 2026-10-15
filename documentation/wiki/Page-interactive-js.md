# interactive.js

**File:** `+spot/+console/webapp/static/interactive.js` · **Page:** [[Interactive Mode|Interactive-Mode]] · **Server side:** [[interactive.py|Console-interactive]]

<!-- source: +spot/+console/webapp/static/interactive.js -->

## What it is for

The Interactive Mode card on the lab console's PhaseSpace tab: switch platforms on, drag their goals on the table, *Fly to Goals*; the lab console closes the loops. This page only picks the platforms, moves the goals, keeps the flight alive with a heartbeat and shows what the loops are doing.

## Function by function

**Lines 14–32.** Constants (heartbeat every 0.5 s; goal updates while dragging at most every 100 ms), formatting, and `CLIENT`, this tab's random id for the lab console (kept in `sessionStorage`, so a reload keeps flying what it flew).

**Lines 34–47.** State and helpers: whose flight it is (`mine`/`other`), which platforms are flying, the PhaseSpace frame's pose of a platform, which switched-on platforms are not flying yet (`pending`).

**Lines 49–61.** `blocker(n)`: why a platform can't start (no stream, not seen, checking, offline, no Teensy, in use, a check running, no goal) as a short and a long text.

**Lines 63–85.** `separation`: the first pair of goals (or goal and resting platform) closer than the minimum, as a sentence; the lab console checks the same.

**Lines 89–112.** `renderController`: the Controller details once: the period, each platform's mass, inertia, thrust (one number: "0.2825 N per thruster"; the lab's calibrated thrusts, a list of eight: their range, e.g. "thrust 0.250–0.310 N (calibrated)"), Q, R and gain matrix K, with a note that each platform flies straight at its goal.

**Lines 114–143.** `renderRow`: a platform's row: the switch (disabled when another page flies or the platform isn't seen), the pill (Flying / Lost / Ready / the blocker), the Here button, the goal, the error (mm and degrees), the eight duty-cycle bars.

**Lines 145–184.** `render`: the rows, the Fly button's text and enabled state (nothing pending, a blocker, or a separation problem), Stop All, the hint line (another page's flight, a goal error, separation, lost platforms, a blocker, or how to use it), the step statistics, the controller and the view.

**Lines 186–197.** `renderView`: the thruster plumes on the flying platforms and the goal markers (draggable when this page flies; read-only when another does).

**Lines 203–215.** `enable(n, on)`: switch a platform on (its goal starts where it is) or off (stopping it if flying).

**Lines 217–240.** `sendGoal`: a goal to the lab console, throttled while dragging; a rejected final goal (e.g. too close) is toasted and the marker snaps back to where the platform is really going; while dragging the error is shown in the hint.

**Lines 242–263.** `fly` (`/api/interactive/start` with the pending platforms' goals), `stopAll`, `heartbeat` (every 0.5 s while this page flies).

**Lines 267–309.** `onStatus`: adopt the lab console's status (after a reload, take over the goals of a flight this tab started), start or stop the heartbeat, and toast the platforms that stopped since the page opened, grouped by reason. `setState` also takes the controller info.

**Lines 311–316.** `onFrame`: keep the newest PhaseSpace frame; give switched-on platforms without a goal one where they are.

**Lines 320–350.** Wiring: the rows' switches and Here buttons; the card's main toggle (switching it off stops this page's flight); Fly and Stop; the viewer's goal picking and dragging callbacks; **Esc** on the PhaseSpace tab stops every platform.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/interactive.js</b> (361 lines; click to show)</summary>

```javascript
   1  // Interactive Mode (lab console, PhaseSpace tab): fly one or more platforms, each to a goal pose
   2  // dragged on the table. Each platform has a switch: switched on, its goal appears on the table;
   3  // Fly to goals sends every switched-on platform to its goal at once. The lab console closes each
   4  // platform's loop with the template diagram's own controller (spot_console/control.py, a port of
   5  // spot.control.lqrCommand and spot.control.thrusterDutyCycles, see spot_console/interactive.py);
   6  // this page only picks the platforms, moves the goals, keeps the flight alive with a heartbeat,
   7  // and shows what the loops are doing.
   8  
   9  import { COLORS, platformName } from './platforms.js';
  10  
  11  const $ = (sel, el = document) => el.querySelector(sel);
  12  const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
  13  
  14  const NAMES = ['RED', 'BLACK', 'BLUE'];
  15  const HEARTBEAT_MS = 500;     // the lab console stops the flight after 3 s without one
  16  const GOAL_SEND_MS = 100;     // goal updates while dragging, at most this often (per platform)
  17  const wrapDeg = rad => { const d = rad * 180 / Math.PI; return ((d + 180) % 360 + 360) % 360 - 180; };
  18  const fmt = (v, digits) => (v == null || !Number.isFinite(v) ? '—' : v.toFixed(digits));
  19  const listNames = names => {
  20    const n = names.map(platformName);
  21    return n.length < 2 ? `the ${n[0] || ''}` : `the ${n.slice(0, -1).join(', the ')} and the ${n[n.length - 1]}`;
  22  };
  23  
  24  // This page's id for the lab console, kept for the tab (a reload keeps flying what it flew)
  25  const CLIENT = (() => {
  26    const make = () => Array.from(crypto.getRandomValues(new Uint8Array(12)), b => b.toString(16).padStart(2, '0')).join('');
  27    try {
  28      let id = sessionStorage.getItem('spot.ixClient');
  29      if (!id) { id = make(); sessionStorage.setItem('spot.ixClient', id); }
  30      return id;
  31    } catch { return make(); }
  32  })();
  33  
  34  export function initInteractive({ api, act, toast, viewer, isShown }) {
  35    const S = { on: false, enabled: {}, goals: {}, status: { active: false }, info: null, frame: null,
  36                stream: {}, hardware: {}, send: {}, dragging: {}, goalError: {}, beat: null,
  37                endedSeen: null, endQueue: new Map(), endTimer: null, renderedAt: 0 };
  38    for (const n of NAMES) S.send[n] = { timer: null, at: 0 };
  39  
  40    const st = () => S.status;
  41    const mine = () => Boolean(st().active && st().client === CLIENT);
  42    const other = () => Boolean(st().active && st().client !== CLIENT);
  43    const flying = n => Boolean(st().active && st().platforms && st().platforms[n]);
  44    const craft = n => S.frame && S.frame.crafts && S.frame.crafts[n];
  45    const tracked = n => { const c = craft(n); return Boolean(c && c.tracked && c.x != null && c.y != null); };
  46    const poseOf = n => { const c = craft(n); return [c.x, c.y, wrapDeg(c.att || 0)]; };
  47    const pending = () => NAMES.filter(n => S.enabled[n] && !flying(n));
  48  
  49    function blocker(n) {
  50      // [short, long]: why platform N can't start flying now, or null
  51      const hw = S.hardware[n], pn = platformName(n);
  52      if (!S.stream.running) return ['No Stream', 'Press Start Streaming first: the platforms are flown from their PhaseSpace poses.'];
  53      if (!tracked(n)) return ['Not Seen', `PhaseSpace doesn't see the ${pn} (on the table, LEDs on?).`];
  54      if (!hw || hw.probe.state === 'checking') return ['Checking', `Checking the ${pn}…`];
  55      if (hw.probe.state !== 'ok') return ['Offline', `The ${pn} can't be reached (see the Hardware tab).`];
  56      if (!hw.probe.teensy) return ['No Teensy', `The ${pn} has no Teensy (see the Hardware tab).`];
  57      if (hw.probe.others.length) return ['In Use', `The ${pn}'s Teensy is in use by ${hw.probe.others.join(', ')}.`];
  58      if (hw.check) return ['Check Running', `A thruster check is running on the ${pn}.`];
  59      if (!S.goals[n]) return ['No Goal', `The ${pn} has no goal yet.`];
  60      return null;
  61    }
  62  
  63    function separation() {
  64      // The first pair of goals (or a goal and a platform resting on the table) too close together,
  65      // as a sentence, or null. The lab console checks the same.
  66      const min = S.info ? S.info.min_separation : 0.45;
  67      const pts = [];
  68      for (const n of NAMES) {
  69        const g = flying(n) && !mine() ? st().platforms[n].goal : (S.enabled[n] || flying(n)) ? S.goals[n] : null;
  70        if (g) pts.push([n, g[0], g[1], 'goal']);
  71        else if (tracked(n)) { const p = poseOf(n); pts.push([n, p[0], p[1], 'resting place']); }
  72      }
  73      for (let i = 0; i < pts.length; i++) {
  74        for (let j = i + 1; j < pts.length; j++) {
  75          const [a, xa, ya, wa] = pts[i], [b, xb, yb, wb] = pts[j];
  76          if (wa !== 'goal' && wb !== 'goal') continue;
  77          const d = Math.hypot(xa - xb, ya - yb);
  78          if (d < min) {
  79            const [g, o, w] = wa === 'goal' ? [a, b, wb] : [b, a, wa];
  80            return `The ${platformName(g)}'s goal is ${d.toFixed(2)} m from the ${platformName(o)}'s ${w}: keep them at least ${min} m apart.`;
  81          }
  82        }
  83      }
  84      return null;
  85    }
  86  
  87    // ---------------------------------------------------------------- the card
  88  
  89    function renderController() {
  90      const info = S.info;
  91      const box = $('#ixController');
  92      if (!info || box.dataset.done) return;
  93      box.dataset.done = '1';
  94      const k3 = v => (Math.abs(v) < 1e-9 ? '0' : Number(v.toPrecision(3)).toString());
  95      const rows = Object.entries(info.platforms).map(([name, p]) => `
  96        <div class="ix-ctl">
  97          <b>${platformName(name)}</b>
  98          <span>mass ${p.mass.toFixed(2)} kg · inertia ${p.inertia.toFixed(4)} kg·m² · thrust ${Array.isArray(p.thrust) ? `${Math.min(...p.thrust).toFixed(3)}–${Math.max(...p.thrust).toFixed(3)} N (calibrated)` : `${p.thrust} N per thruster`}</span>
  99          <span>Q [x y θ ẋ ẏ θ̇] = [${p.Q.join(' ')}] · R [Fx Fy Tz] = [${p.R.join(' ')}]</span>
 100          <table class="ix-k" title="u = −K·(measured − goal)"><tbody>${p.K.map(r => `<tr>${r.map(v => `<td>${k3(v)}</td>`).join('')}</tr>`).join('')}</tbody></table>
 101        </div>`).join('');
 102      box.innerHTML = `
 103        <p class="hint">The template diagram's default controller, run by this console for each platform every ${info.period} s
 104          (the diagram's baseRate, as often as its controller and thruster code run): its LQR law (<span class="mono">spot.control.lqrCommand</span>)
 105          and thruster mixer (<span class="mono">spot.control.thrusterDutyCycles</span>), ported line by line to
 106          <span class="mono">spot_console/control.py</span> and checked against the diagram by
 107          <span class="mono">spot.control.verifyConsoleController</span> in MATLAB. SPOT's default settings
 108          (gains for a ${info.sample_rate} Hz diagram; mixer: ${info.max_iters} iterations, tolerance ${info.tol}).
 109          Each platform flies straight at its goal and knows nothing of the others: goals are kept
 110          ${info.min_separation} m apart, but keep their paths clear of each other too.</p>
 111        ${rows}`;
 112    }
 113  
 114    function renderRow(n) {
 115      const row = $(`[data-ixrow=${n}]`), s = st(), f = flying(n) ? s.platforms[n] : null, oth = other();
 116      const on = Boolean(S.enabled[n] || f);
 117      row.classList.toggle('on', on);
 118      const sw = $('[data-ixon]', row);
 119      sw.checked = on;
 120      sw.disabled = oth || (!on && !tracked(n));
 121      sw.parentElement.title = oth ? `Interactive Mode is flying for ${s.by}`
 122        : !on && !tracked(n) ? `PhaseSpace doesn't see the ${platformName(n)}`
 123        : `Give the ${platformName(n)} a goal on the table; Fly to Goals sends it there. Switching it off while it flies stops it (everything off).`;
 124      const pill = $('[data-role=state]', row);
 125      const why = on && !f ? blocker(n) : null;
 126      const [text, kind] = f ? (f.lost ? ['Lost', 'warn'] : ['Flying', 'live'])
 127        : !on ? ['', ''] : why ? [why[0], 'warn'] : ['Ready', 'ok'];
 128      pill.textContent = text;
 129      pill.className = `pill ${kind}`;
 130      pill.hidden = !text;
 131      pill.title = why ? why[1] : f && f.late ? `${f.late} late steps` : '';
 132      $('[data-role=here]', row).hidden = !on || oth || !tracked(n);
 133      const g = f && !mine() ? f.goal : S.goals[n];
 134      $('[data-role=goal]', row).textContent = g ? `goal ${fmt(g[0], 3)}, ${fmt(g[1], 3)} m · ${fmt(g[2], 1)}°` : 'goal —';
 135      const e = f && f.error;
 136      $('[data-role=error]', row).textContent = e ? `error ${fmt(Math.hypot(e[0], e[1]) * 1000, 0)} mm · ${fmt(e[2], 1)}°`
 137        : f ? 'error —' : 'not flying';
 138      $$('[data-role=bars] span', row).forEach((bar, k) => {
 139        const d = f ? f.duty[k] : 0;
 140        bar.firstElementChild.style.height = `${Math.max(0, Math.min(100, d))}%`;
 141        bar.title = `T${k + 1}: ${fmt(d, 0)} %`;
 142      });
 143    }
 144  
 145    function render() {
 146      S.renderedAt = performance.now();
 147      $('#ixBody').hidden = !S.on;
 148      $('#toggleInteractive').checked = S.on;
 149      const s = st(), own = mine(), oth = other();
 150      for (const n of NAMES) renderRow(n);
 151  
 152      const todo = oth ? [] : pending();
 153      const blocked = todo.map(n => [n, blocker(n)]).filter(([, b]) => b);
 154      const sep = oth ? null : separation();
 155      const fly = $('#ixFly');
 156      fly.disabled = !todo.length || blocked.length > 0 || Boolean(sep);
 157      fly.textContent = !todo.length ? 'Fly to Goals' : own ? `Fly ${listNames(todo)} Too` : `Fly ${listNames(todo)}`;
 158      fly.hidden = oth;
 159      $('#ixStop').hidden = !s.active;
 160  
 161      const flyingNames = s.active ? Object.keys(s.platforms) : [];
 162      const lost = flyingNames.filter(n => s.platforms[n].lost);
 163      const goalErr = NAMES.map(n => S.goalError[n]).find(Boolean);
 164      let hint, warn = false;
 165      if (oth) hint = `Flying ${listNames(flyingNames)} for ${s.by}. Anyone can stop it.`;
 166      else if (goalErr) { hint = goalErr; warn = true; }
 167      else if (sep) { hint = sep; warn = true; }
 168      else if (lost.length) { hint = `PhaseSpace has lost ${listNames(lost)}: thrusters off until it sees it again.`; warn = true; }
 169      else if (blocked.length) { hint = blocked[0][1][1]; warn = true; }
 170      else if (own) hint = 'Drag a goal to move its platform; Shift+drag turns it. A platform\'s switch stops it; Esc stops all.'
 171        + (todo.length ? ` Fly to Goals adds ${listNames(todo)}.` : '');
 172      else if (!todo.length) hint = 'Switch on the platforms to fly (or click them on the table), then drag their goals.';
 173      else hint = 'Place the goals (drag; Shift+drag turns), then Fly to Goals: the pucks come on and each platform flies straight to its goal. Keep their paths clear of each other.';
 174      const h = $('#ixHint');
 175      h.textContent = hint;
 176      h.className = `hint ${warn ? 'warn-text' : ''}`;
 177  
 178      const end = s.ended && s.ended[s.ended.length - 1];
 179      $('#ixStats').textContent = s.active
 180        ? `step ${flyingNames.map(n => `${platformName(n)} ${fmt(s.platforms[n].step_ms, 1)} ms${s.platforms[n].late ? ` (${s.platforms[n].late} late)` : ''}`).join(' · ')} · every ${s.period} s`
 181        : end ? `Last: the ${platformName(end.platform)} ended: ${end.reason}` : '';
 182      renderController();
 183      renderView();
 184    }
 185  
 186    function renderView() {
 187      if (!viewer) return;
 188      const s = st(), oth = other();
 189      for (const n of NAMES) viewer.setThrust(n, S.on && flying(n) ? s.platforms[n].duty : null);
 190      if (!S.on) { viewer.setGoals({}); return; }
 191      const goals = {};
 192      for (const n of NAMES) {
 193        if (oth) { if (flying(n)) goals[n] = { pose: s.platforms[n].goal, editable: false }; }
 194        else if ((S.enabled[n] || flying(n)) && S.goals[n]) goals[n] = { pose: S.goals[n], editable: true };
 195      }
 196      viewer.setGoals({ goals, pickable: !oth, margin: S.info ? S.info.margin : null });
 197    }
 198  
 199    const renderSoon = () => { if (performance.now() - S.renderedAt > 200) render(); };
 200  
 201    // ---------------------------------------------------------------- actions
 202  
 203    async function enable(n, on) {
 204      if (other()) return;
 205      if (on) {
 206        S.enabled[n] = true;
 207        if (!S.goals[n] && tracked(n)) S.goals[n] = poseOf(n);
 208      } else {
 209        S.enabled[n] = false;
 210        delete S.goals[n];
 211        delete S.goalError[n];
 212        if (flying(n) && mine()) onStatus(await api('/api/interactive/stop', { platform: n }));
 213      }
 214      render();
 215    }
 216  
 217    function sendGoal(n, final) {
 218      // While flying from this page: platform N's goal to the lab console (throttled while dragging)
 219      const q = S.send[n];
 220      clearTimeout(q.timer);
 221      q.timer = null;
 222      if (!mine() || !flying(n) || !S.goals[n]) return;
 223      const wait = GOAL_SEND_MS - (performance.now() - q.at);
 224      if (!final && wait > 0) { q.timer = setTimeout(() => sendGoal(n, false), wait); return; }
 225      q.at = performance.now();
 226      const [x, y, att] = S.goals[n];
 227      api('/api/interactive/goal', { client: CLIENT, platform: n, x, y, att, final })
 228        .then(() => { if (S.goalError[n]) { delete S.goalError[n]; render(); } })
 229        .catch(e => {
 230          // e.g. too close to another goal: the lab console keeps flying to the last good one
 231          if (final) {
 232            toast(e.message, 'error');
 233            delete S.goalError[n];
 234            if (flying(n)) S.goals[n] = st().platforms[n].goal;   // back to where it is really going
 235          } else {
 236            S.goalError[n] = e.message;   // shown while dragging
 237          }
 238          render();
 239        });
 240    }
 241  
 242    async function fly() {
 243      const todo = pending();
 244      if (!todo.length) return;
 245      const platforms = {};
 246      for (const n of todo) { const [x, y, att] = S.goals[n]; platforms[n] = { x, y, att }; }
 247      onStatus(await api('/api/interactive/start', { platforms, client: CLIENT }));
 248    }
 249  
 250    async function stopAll() {
 251      onStatus(await api('/api/interactive/stop', {}));
 252    }
 253  
 254    function heartbeat(on) {
 255      if (on && !S.beat) {
 256        S.beat = setInterval(() => {
 257          api('/api/interactive/heartbeat', { client: CLIENT }).catch(() => { /* the lab console stops it */ });
 258        }, HEARTBEAT_MS);
 259      } else if (!on && S.beat) {
 260        clearInterval(S.beat);
 261        S.beat = null;
 262      }
 263    }
 264  
 265    // ---------------------------------------------------------------- from the lab console
 266  
 267    function onStatus(status) {
 268      const wasMine = mine();
 269      S.status = status || { active: false };
 270      const s = st(), own = mine();
 271      if (own) {
 272        for (const n of Object.keys(s.platforms)) {
 273          const q = S.send[n];
 274          if (!wasMine || !S.goals[n]) {
 275            // Flying from this page (also after a reload): show the controls, take the goals
 276            S.on = true;
 277            S.enabled[n] = true;
 278            S.goals[n] = s.platforms[n].goal;
 279          } else if (!S.dragging[n] && !q.timer && performance.now() - q.at > 500 && !S.goalError[n]) {
 280            S.goals[n] = s.platforms[n].goal;   // as the lab console has it (e.g. kept inside the margin)
 281          }
 282        }
 283      }
 284      heartbeat(own);
 285      // Tell about platforms that stopped (not those already seen when the page opened)
 286      const ended = s.ended || [];
 287      if (S.endedSeen == null) S.endedSeen = ended.length ? ended[ended.length - 1].time : 0;
 288      for (const e of ended) {
 289        if (e.time <= S.endedSeen) continue;
 290        S.endedSeen = e.time;
 291        if (S.on || wasMine) {
 292          // Platforms that stop together arrive one event each: one toast per reason, a moment later
 293          S.endQueue.set(e.reason, [...(S.endQueue.get(e.reason) || []), e.platform]);
 294          clearTimeout(S.endTimer);
 295          S.endTimer = setTimeout(() => {
 296            for (const [reason, names] of S.endQueue) {
 297              toast(`Interactive Mode stopped ${listNames(names)}: ${reason}`, /^stopped by/.test(reason) ? 'info' : 'error', 9000);
 298            }
 299            S.endQueue.clear();
 300          }, 300);
 301        }
 302      }
 303      render();
 304    }
 305  
 306    function setState(status, info) {
 307      if (info) {   // (new parameters, e.g. a new thruster calibration: the Controller text is drawn again)
 308        S.info = info;
 309        delete $('#ixController').dataset.done;
 310      }
 311      onStatus(status);
 312    }
 313  
 314    function onFrame(frame) {
 315      S.frame = frame;
 316      if (!S.on) return;
 317      for (const n of NAMES) if (S.enabled[n] && !S.goals[n] && tracked(n)) S.goals[n] = poseOf(n);
 318      renderSoon();
 319    }
 320  
 321    // ---------------------------------------------------------------- wiring
 322  
 323    for (const n of NAMES) {
 324      const row = $(`[data-ixrow=${n}]`);
 325      $('.swatch', row).style.background = COLORS[n];
 326      $('[data-ixon]', row).onchange = e => act(() => enable(n, e.target.checked));
 327      $('[data-role=here]', row).onclick = () => {
 328        if (!tracked(n)) return;
 329        S.goals[n] = poseOf(n);
 330        sendGoal(n, true);
 331        render();
 332      };
 333    }
 334    $('#toggleInteractive').onchange = () => act(async () => {
 335      S.on = $('#toggleInteractive').checked;
 336      if (!S.on && mine()) await stopAll();
 337      render();
 338    });
 339    $('#ixFly').onclick = () => act(fly);
 340    $('#ixStop').onclick = () => act(stopAll);
 341    if (viewer) {
 342      viewer.goal.onPick = n => { if (S.on && !S.enabled[n]) act(() => enable(n, true)); };
 343      viewer.goal.onDrag = (n, x, y, att, done) => {
 344        S.goals[n] = [x, y, att];
 345        S.dragging[n] = !done;
 346        sendGoal(n, done);
 347        if (done) render(); else renderSoon();
 348      };
 349    }
 350    // Esc on the PhaseSpace tab stops every platform (whoever started them)
 351    document.addEventListener('keydown', e => {
 352      if (e.key === 'Escape' && st().active && isShown() && !document.querySelector('dialog[open]')) act(stopAll);
 353    });
 354  
 355    return {
 356      setState, onStatus, onFrame,
 357      onStream(s) { S.stream = s || {}; if (S.on) render(); },
 358      onHardware(hw) { for (const p of hw.platforms) S.hardware[p.name] = p; if (S.on) render(); },
 359      onHardwarePlatform(p) { S.hardware[p.name] = p; if (S.on) renderSoon(); },
 360    };
 361  }
```

</details>
<!-- /listing -->
