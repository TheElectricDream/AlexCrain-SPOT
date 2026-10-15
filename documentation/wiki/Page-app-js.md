# The Page: index.html and app.js

**Files:** `+spot/+console/webapp/static/index.html`, `app.js`, `app.css`

<!-- source: +spot/+console/webapp/static/app.js -->
<!-- source: +spot/+console/webapp/static/index.html -->

## What it is

One HTML page serves both consoles. `app.js` is the entry module: it talks to the server (JSON requests and the server-sent event stream), owns the top bar, the page switching, the shared 3D view and the lab console's own cards (Compile, Stream, Telemetry, the logs), and starts the modules for the other tabs:

| Module | Tab |
|---|---|
| [[sim.js|Page-sim-js]] | Simulation |
| [[experiment.js|Page-experiment-js]] | Experiment |
| [[results.js and data.js|Page-results-js]] | Results |
| [[designer.js|Page-designer-js]] | Designer |
| [[calibration.js and calviz.js|Page-calibration-js]] | Calibration (both consoles) |
| [[hardware.js|Page-hardware-js]] | Hardware (lab console) |
| [[interactive.js|Page-interactive-js]] | Interactive Mode (lab console, PhaseSpace tab) |
| [[viewer.js|Page-viewer-js]], [[cad.js|Page-cad-js]] | the 3D view |
| [[liveplot.js|Page-liveplot-js]], [[messages.js|Page-messages-js]] | the Live Plot and Messages panels |
| [[platforms.js, ratelock.js|Page-small-modules]] | names/colours, the data-rate lock |

No framework and no build step: ES modules loaded by the browser, `three.js` from `vendor/` through an import map (`index.html` line 9).

## `index.html`

The skeleton, in order: the **top bar** (brand and host line, the page tabs, the local console's settings bar with Load… / Save As… / Reset…, the lab-console link, the packet rate and the connection badge); the **PhaseSpace page** (Compile, Stream, Telemetry and Interactive Mode cards; the 3D view with its toolbar; the Build/Stream logs); the **Simulation page** (the Scenario/Platforms/Control/Solver/Workspace sub-tabs and panes, Real Time, Run, the 3D view slot with the playback bar, the MATLAB console with its Live Plot tab, the Messages panel); the **Experiment page** (Lab, Platforms, Build, Deploy & Run, Data cards; the view with clocks and overlays); the **Results page** (Datasets and Signals cards; the plot toolbar, plots and Export); the **Designer page**; the **Hardware page** (cards built by `hardware.js`, the hardware log); the **Calibration page** (`#pageCalibration`: the Table Slope / Thrusters / Arm section switcher, `[data-calsec]`, the `#calSync` line and `#calBody`, filled by `calibration.js`); and the dialogs (choose folder, reset, unlock the data rate, `#calSyncDialog` "Newer Calibration on the Lab Console" with Update, Not Now and Never Ask Again, `#calThrustDialog` "Before the Thruster Calibration" with the platform's assumed Mass, Inertia, CG x and CG y inputs and an I Understand checkbox that enables Start, Saved Data, `#calArmDialog` "Before the Arm Calibration": the Chaser rests on the table and the console keeps it resting, each tested joint moves up to 25° either way, the power switch at hand and Stop (or Esc) switching the servos' torque off, the chosen joints and tests with the time it takes (`#calArmTime`), and a confirmation checkbox (`#calArmAck`: the Chaser rests and the arm is clear to move) that enables Start) and the toast area. The 3D view's toolbar has a Table Slope toggle (`#toggleSlope`) next to the others.

The 3D view (`#viewer`), the MATLAB console (`#matlabConsole`) and the Messages panel (`#msgPanel`) exist **once**; `showPage` moves them into the shown page's slot (`data-slot`, `data-console-slot`, `data-msg-slot`).

## `app.js`, function by function

**Lines 18–22.** The page's state (`S`) and the viewer.

**Lines 26–40.** `?capture` keeps the WebGL image for screenshots. `TOKEN`: the local console opens the page with `?token=…`; it is kept in `sessionStorage` for the tab, sent with every request, and removed from the address bar.

**Lines 42–55.** `api(path, body)`: GET (no body) or POST as JSON with the token header and `X-SPOT-Client: webapp`; a non-2xx reply becomes an error with the server's message and, for MATLAB errors, the details.

**Lines 57–80.** `toast` (a message for a few seconds), `act` (run an action, toast its error), time/duration/angle formatting, and `storage` (localStorage with JSON and a fallback).

**Lines 84–104.** `renderHost` (the host line) and `renderSettings` (the lab console's output folder, frequency, viewer port, transport, with a hint when UDP can't work under WSL's NAT).

**Lines 106–141.** `renderTargets`: the Compile card: whether a compiler was found, the last build's result, the Compile and Re-Check buttons.

**Lines 143–171.** `renderStream`: the Stream card: the pill (Streaming · time · rate / Stopped / Exited), the Start/Stop button (disabled until StreamData is built), who started or stopped it, the frequency and transport locked while streaming.

**Lines 173–196.** `renderMonitor` (toast a monitor error once) and `updateStale` (the "Waiting for Packets" overlay when no frame arrived for 2 s, with the reason; the Live / Last Packet N s Ago text).

**Lines 198–222.** `renderTelemetry`: the table of each platform's x, y, θ and rates from a frame, and the visible LED chips coloured by owner.

**Lines 226–287.** The logs: lines queued per channel and flushed once per animation frame, classified (command, header, ok, error, warning), capped at 3000 lines, auto-scrolled; `resetLogs` loads the history on connect; `showTab` switches Build/Stream.

**Lines 291–323.** The extra destinations' chips (remove with ×), the "+ This Computer" shortcut (the client's own IP, when it isn't local and not already sent to), `addDestination`.

**Lines 325–362.** `loadState`: `GET /api/state`, then everything that depends on it. In **local** mode: host, logs, the Simulation/Experiment/Results/Designer/Calibration modules' `onState`, the lab link, the connection badge. In **lab** mode: settings, toolchains, build, stream, monitor, LED owners, logs, the hardware cards, Interactive Mode's state, the Designer and the Calibration tab.

**Lines 364–396.** The connection badge: on the lab console whether this page reaches it; on the local console (which is always local) whether the **lab console** is reachable, since that decides whether experiments work; the Lab Console link is enabled accordingly.

**Lines 398–456.** `connect`: the `EventSource` on `/api/events` and a handler per event: `log`, `toolchains`, `build_status` (switch to the Build log, toast when done), `stream_status` (toast when someone else starts or stops it; Interactive Mode and the Calibration tab are told), `settings`, `monitor`, `hardware_platform`, `interactive`, `matlab`, `build`, `build_log`, `lab` (the badge, the Experiment, Designer and Calibration modules), `design`/`lab_design`, `calibration`, `calibration_run` and `lab_calibration` (the Calibration tab), the relayed `lab_*` events for the Experiment tab, `sim` and `matlab_event` for the Simulation tab, and `frame` (the 3D view, Interactive Mode, the telemetry at ≤10 Hz).

**Lines 458–521.** The folder dialog: browse, up, go, new folder, use this folder (saves the output folder).

**Lines 523–586.** The lab console's controls: Compile, Re-Check, Start/Stop Streaming (with the typed frequency), frequency and transport changes, destinations, the viewer port, log tabs, Clear, collapse.

**Lines 588–645.** Pages (`PAGES` includes `calibration`): `pageAvailable` (by the server's features), `showPage` (tab highlight, hide the others, remember it in the hash and storage, move the 3D view, the MATLAB console and the Messages panel into the page, tell the Live Plot which mode, call each module's `show`/`hide`, set the view's layers for pages without a module), `applyFeatures` (which tabs exist, the title "SPOT Local Simulations" / "SPOT Hardware Interfaces", the first page from the hash or the last shown).

**Lines 647–698.** The Hardware tab module; the `Viewer` (an error message if WebGL isn't available); the view buttons (3D/Top/Side); the toggles (LED IDs, Labels, Axes, LED Height Lines) remembered in storage; *Use CAD* (loads the models on first use; off again if they can't be loaded); the *ZED2 Viewpoint* window and its lens choice.

**Lines 700–725.** The other modules, wired together: the data store, messages, live plot, simulation (which owns the settings), experiment (reading the simulation's state and settings through callbacks), results, interactive, designer (which can switch the simulation's Arm platform on and off), calibration (`initCalibration`, reading the stream status); the Table Slope toggle (remembered in storage, off by default; hidden until a `spot-calibration` event says there is a table calibration; it calls the viewer's `setTableSlopeShown`), the Simulation overlay toggle.

**Lines 729–733.** Start: empty telemetry, the targets, load the state, a 1 s timer for the stream pill and staleness, connect the event stream.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/app.js</b> (737 lines; click to show)</summary>

```javascript
   1  // UI logic for the SPOT Console. Talks to app.py over JSON + server-sent events.
   2  import { Viewer, CRAFT_COLORS, LED_COLORS } from './viewer.js';
   3  import { platformName } from './platforms.js';
   4  import { initHardware } from './hardware.js';
   5  import { initSimulation } from './sim.js';
   6  import { initExperiment } from './experiment.js';
   7  import { initResults } from './results.js';
   8  import { createDataStore } from './data.js';
   9  import { initMessages } from './messages.js';
  10  import { initLivePlot } from './liveplot.js';
  11  import { initInteractive } from './interactive.js';
  12  import { initDesigner } from './designer.js';
  13  import { initCalibration } from './calibration.js';
  14  
  15  const $ = (sel, el = document) => el.querySelector(sel);
  16  const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
  17  
  18  const S = {
  19    host: {}, settings: {}, toolchains: { checking: true }, build: {}, stream: {}, monitor: {},
  20    ledOwner: {}, lastFrameAt: 0, lastTelemetryAt: 0, historyUntil: 0, monitorErrorShown: null,
  21  };
  22  let viewer = null;
  23  
  24  // ---------------------------------------------------------------- helpers
  25  
  26  // ?capture keeps the 3D view's image so it can be saved or screenshotted
  27  const CAPTURE = new URLSearchParams(location.search).has('capture');
  28  
  29  // The local console (started by SPOT in MATLAB) opens this page with ?token=...; it is kept for this
  30  // tab and sent with every request, and taken out of the address bar
  31  const TOKEN = (() => {
  32    const params = new URLSearchParams(location.search);
  33    let t = params.get('token');
  34    try {
  35      if (t) sessionStorage.setItem('spot.token', t);
  36      else t = sessionStorage.getItem('spot.token');
  37    } catch { /* storage blocked: keep it in memory */ }
  38    if (params.has('token')) history.replaceState(null, '', location.pathname + (CAPTURE ? '?capture' : '') + location.hash);
  39    return t || '';
  40  })();
  41  
  42  async function api(path, body) {
  43    const headers = TOKEN ? { 'X-SPOT-Token': TOKEN } : {};
  44    const opts = body === undefined ? { headers } :
  45      { method: 'POST', headers: { ...headers, 'Content-Type': 'application/json', 'X-SPOT-Client': 'webapp' }, body: JSON.stringify(body) };
  46    const res = await fetch(path, opts);
  47    let data = {};
  48    try { data = await res.json(); } catch { /* not JSON */ }
  49    if (!res.ok) {
  50      const err = new Error(data.error || `Request failed (${res.status})`);
  51      err.details = data.details || null;   // MATLAB's errors: causes, blocks, report (spot.sim.errorDetails)
  52      throw err;
  53    }
  54    return data;
  55  }
  56  
  57  function toast(message, kind = 'info', ms = 6000) {
  58    const el = document.createElement('div');
  59    el.className = `toast ${kind}`;
  60    el.textContent = message;
  61    $('#toasts').append(el);
  62    setTimeout(() => el.remove(), ms);
  63  }
  64  
  65  async function act(fn) {
  66    try { return await fn(); } catch (e) { toast(e.message, 'error', 9000); }
  67  }
  68  
  69  const fmtTime = t => new Date(t * 1000).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  70  const fmtDur = sec => {
  71    sec = Math.max(0, Math.floor(sec));
  72    const h = Math.floor(sec / 3600), m = Math.floor(sec / 60) % 60, s = sec % 60;
  73    return (h ? `${h}:${String(m).padStart(2, '0')}` : `${m}`) + `:${String(s).padStart(2, '0')}`;
  74  };
  75  const wrapDeg = rad => { let d = rad * 180 / Math.PI; d = ((d + 180) % 360 + 360) % 360 - 180; return d; };
  76  const num = (v, digits) => (v == null ? '—' : v.toFixed(digits));
  77  const storage = {
  78    get(k, dflt) { try { const v = localStorage.getItem(k); return v === null ? dflt : JSON.parse(v); } catch { return dflt; } },
  79    set(k, v) { try { localStorage.setItem(k, JSON.stringify(v)); } catch { /* private mode */ } },
  80  };
  81  
  82  // ---------------------------------------------------------------- rendering
  83  
  84  function renderHost() {
  85    const h = S.host;
  86    const os = h.wsl ? `Linux in WSL ${h.wsl.version} (${h.wsl.networking} networking)` : h.os;
  87    $('#hostLine').textContent = h.name ? `${h.name} · ${os}` : 'Connecting…';
  88    $('#hostLine').title = S.projectDir ? `Project folder: ${S.projectDir}` : '';
  89  }
  90  
  91  function renderSettings() {
  92    const out = $('#outputDir');
  93    out.value = S.settings.output_dir || '';
  94    out.title = S.settings.output_dir || '';
  95    if (document.activeElement !== $('#frequency') && S.settings.frequency) $('#frequency').value = S.settings.frequency;
  96    if (document.activeElement !== $('#monitorPort') && S.settings.monitor_port) $('#monitorPort').value = S.settings.monitor_port;
  97    const transport = S.settings.transport || 'tcp';
  98    for (const b of $$('[data-transport]')) b.classList.toggle('active', b.dataset.transport === transport);
  99    // In WSL 2's default NAT networking, PhaseSpace's UDP frames can't reach StreamData
 100    const natUdp = transport === 'udp' && S.host.wsl && S.host.wsl.networking === 'nat';
 101    const hint = $('#transportHint');
 102    hint.hidden = !natUdp;
 103    hint.textContent = "UDP won't arrive through WSL's NAT networking: use TCP, or switch WSL to mirrored networking.";
 104  }
 105  
 106  function renderTargets() {
 107    const tcs = S.toolchains || {};
 108    const tc = tcs.linux;
 109    const tile = $('.target');
 110    const status = $('[data-role=status]', tile);
 111    const last = $('[data-role=last]', tile);
 112    const btn = $('[data-role=build]', tile);
 113  
 114    if (!tc) {
 115      status.className = 'target-status';
 116      status.textContent = 'Checking for a compiler…';
 117    } else if (tc.available) {
 118      status.className = 'target-status ok';
 119      status.textContent = 'Compiler Ready';
 120      status.title = tc.via;
 121    } else {
 122      status.className = 'target-status bad';
 123      status.textContent = tc.reason;
 124      status.title = tc.reason;
 125    }
 126  
 127    const lb = S.build.last;
 128    if (S.build.running) {
 129      last.className = 'target-last'; last.textContent = 'Compiling…';
 130    } else if (lb) {
 131      last.className = `target-last ${lb.ok ? 'ok' : 'bad'}`;
 132      last.textContent = lb.ok ? `✓ Built ${fmtTime(lb.finished)} (${lb.seconds} s)` : `✕ Failed ${fmtTime(lb.finished)}, see Build Log`;
 133    } else {
 134      last.className = 'target-last'; last.textContent = '';
 135    }
 136  
 137    btn.disabled = Boolean(S.build.running) || !tc || !tc.available;
 138    btn.innerHTML = S.build.running ? '<span class="spinner"></span><span>Compiling…</span>' : '<span>Compile</span>';
 139    $('#recheckBtn').disabled = Boolean(tcs.checking);
 140    $('#recheckBtn').textContent = tcs.checking ? 'Checking…' : 'Re-Check';
 141  }
 142  
 143  function renderStream() {
 144    const st = S.stream;
 145    const pill = $('#streamPill');
 146    const btn = $('#streamBtn');
 147    if (st.running) {
 148      pill.className = 'pill live';
 149      pill.textContent = `Streaming · ${fmtDur(Date.now() / 1000 - st.started)} · ${st.frequency} Hz`;
 150      btn.className = 'btn btn-stop';
 151      btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="6" y="6" width="12" height="12" rx="2" fill="currentColor"/></svg><span>Stop Streaming</span>';
 152      btn.disabled = false;
 153    } else {
 154      const failed = st.last_exit != null && st.last_exit !== 0;
 155      pill.className = failed ? 'pill err' : 'pill';
 156      pill.textContent = failed ? `Exited (Code ${st.last_exit})` : 'Stopped';
 157      btn.className = 'btn btn-go';
 158      btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg><span>Start Streaming</span>';
 159      btn.disabled = !st.exe_exists;
 160    }
 161    if (st.running && st.started_by) pill.title = `Started ${fmtTime(st.started)} by ${st.started_by}`;
 162    else if (!st.running && st.stopped_by) pill.title = `Stopped ${fmtTime(st.stopped)} by ${st.stopped_by}`;
 163    else pill.title = '';
 164    btn.title = st.exe ? `Runs ${st.exe}` : '';
 165    const exe = $('#exeLine');
 166    exe.hidden = !st.exe || st.exe_exists;
 167    exe.textContent = 'Not built yet. Click Compile first.';
 168    exe.title = st.exe || '';
 169    $('#frequency').disabled = Boolean(st.running);
 170    for (const b of $$('[data-transport]')) b.disabled = Boolean(st.running);
 171  }
 172  
 173  function renderMonitor() {
 174    const m = S.monitor;
 175    if (m.error && S.monitorErrorShown !== m.error) {
 176      S.monitorErrorShown = m.error;
 177      toast(m.error, 'error', 12000);
 178    }
 179    updateStale();
 180  }
 181  
 182  function updateStale() {
 183    const age = (performance.now() - S.lastFrameAt) / 1000;
 184    const stale = !S.lastFrameAt || age > 2;
 185    const empty = $('#viewerEmpty');
 186    empty.classList.toggle('hidden', !stale);
 187    if (stale) {
 188      const m = S.monitor;
 189      $('#viewerEmptyDetail').textContent = m.error ? m.error
 190        : S.lastFrameAt ? `No packets for ${Math.round(age)} s (listening on 127.0.0.1:${m.port}).`
 191        : S.stream.running ? `Streaming started; waiting for the first packet on 127.0.0.1:${m.port}…`
 192        : 'Press Start Streaming to see the spacecraft.';
 193      $('#statRate').textContent = '0 Hz';
 194    }
 195    $('#lastPacket').textContent = S.lastFrameAt ? (stale ? `Last Packet ${Math.round(age)} s Ago` : 'Live') : 'No Packets Yet';
 196  }
 197  
 198  function renderTelemetry(frame) {
 199    const rows = Object.keys(CRAFT_COLORS).map(name => {
 200      const c = frame.crafts[name] || {};
 201      const t = c.tracked;
 202      return `<tr class="${t ? '' : 'untracked'}"><td><span class="craft"><i style="background:${CRAFT_COLORS[name]}"></i>${platformName(name)}</span></td>` +
 203        `<td>${t ? num(c.x, 3) : '—'}</td><td>${t ? num(c.y, 3) : '—'}</td>` +
 204        `<td>${t && c.att != null ? wrapDeg(c.att).toFixed(1) : '—'}</td>` +
 205        `<td>${t ? num(c.vx, 3) : '—'}</td><td>${t ? num(c.vy, 3) : '—'}</td>` +
 206        `<td>${t && c.w != null ? (c.w * 180 / Math.PI).toFixed(1) : '—'}</td></tr>`;
 207    });
 208    $('#telemetryBody').innerHTML = rows.join('');
 209    const leds = [...frame.leds].sort((a, b) => a[0] - b[0]);
 210    $('#ledCount').textContent = `${leds.length} Visible`;
 211    $('#ledChips').innerHTML = leds.map(([k, x, y, z]) => {
 212      const owner = S.ledOwner[k];
 213      const color = LED_COLORS[owner] || LED_COLORS.none;
 214      const title = `LED ${k}${owner ? ` (${platformName(owner)})` : ' (not on a body)'}: x ${x.toFixed(3)}  y ${y.toFixed(3)}  z ${z.toFixed(3)} m`;
 215      return `<span class="chip" title="${title}" style="color:${color};border-color:${color}55;background:${color}14">${k}</span>`;
 216    }).join('');
 217    $('#statRate').textContent = `${frame.rate} Hz`;
 218  }
 219  
 220  function renderTelemetryEmpty() {
 221    renderTelemetry({ crafts: {}, leds: [], rate: 0 });
 222  }
 223  
 224  // ---------------------------------------------------------------- logs
 225  
 226  const logEls = { build: $('#logBuild'), stream: $('#logStream'), hardware: $('#logHardware') };
 227  const logCounts = { build: 0, stream: 0, hardware: 0 };
 228  const pending = { build: [], stream: [], hardware: [] };
 229  let flushQueued = false;
 230  const MAX_LOG_LINES = 3000;
 231  
 232  function classify(line) {
 233    if (line.startsWith('$ ')) return 'l-cmd';
 234    if (line.startsWith('===')) return /FAILED|exited \(code [^0]/.test(line) ? 'l-err' : /succeeded/.test(line) ? 'l-ok' : 'l-head';
 235    if (/\berror\b|fatal|could not|cannot|failed|link to the Teensy lost|not answering|doesn't accept/i.test(line)) return 'l-err';
 236    if (/warning|let go of it|cancelled/i.test(line)) return 'l-warn';
 237    return '';
 238  }
 239  
 240  function queueLog(entry) {
 241    if (!pending[entry.channel]) return;  // a channel this page doesn't show (e.g. matlab)
 242    pending[entry.channel].push(entry.line);
 243    if (!flushQueued) { flushQueued = true; requestAnimationFrame(flushLogs); }
 244  }
 245  
 246  function flushLogs() {
 247    flushQueued = false;
 248    for (const ch of Object.keys(pending)) {
 249      const lines = pending[ch];
 250      if (!lines.length) continue;
 251      pending[ch] = [];
 252      const el = logEls[ch];
 253      const atBottom = el.scrollHeight - el.scrollTop - el.clientHeight < 40;
 254      const frag = document.createDocumentFragment();
 255      for (const line of lines) {
 256        const div = document.createElement('div');
 257        const cls = classify(line);
 258        if (cls) div.className = cls;
 259        div.textContent = line || ' ';
 260        frag.append(div);
 261      }
 262      el.append(frag);
 263      while (el.childElementCount > MAX_LOG_LINES) el.firstElementChild.remove();
 264      logCounts[ch] += lines.length;
 265      $(`[data-count=${ch}]`).textContent = logCounts[ch];
 266      if (atBottom) el.scrollTop = el.scrollHeight;
 267    }
 268  }
 269  
 270  function resetLogs(history) {
 271    for (const ch of Object.keys(logEls)) {
 272      logEls[ch].textContent = '';
 273      logCounts[ch] = 0;
 274      pending[ch] = [];
 275      for (const e of history[ch] || []) pending[ch].push(e.line);
 276      S.historyUntil = Math.max(S.historyUntil, ...(history[ch] || []).map(e => e.time));
 277    }
 278    flushLogs();
 279    for (const el of Object.values(logEls)) el.scrollTop = el.scrollHeight;
 280  }
 281  
 282  function showTab(tab) {
 283    for (const b of $$('.tabs button')) b.classList.toggle('active', b.dataset.tab === tab);
 284    for (const ch of ['build', 'stream']) logEls[ch].classList.toggle('active', ch === tab);
 285    $('#console').classList.remove('collapsed');
 286    logEls[tab].scrollTop = logEls[tab].scrollHeight;
 287  }
 288  
 289  // ---------------------------------------------------------------- state & events
 290  
 291  function renderDestinations() {
 292    const extras = S.settings.extra_destinations || [];
 293    $('#destsLabel').title = `Computers that get the packets on UDP port 31534. Always sent to: ${(S.baseDestinations || []).join(', ') || 'none'}`;
 294    const box = $('#extraDests');
 295    box.textContent = '';
 296    for (const ip of extras) {
 297      const chip = document.createElement('span');
 298      chip.className = 'chip dest-chip';
 299      chip.textContent = ip;
 300      const x = document.createElement('button');
 301      x.type = 'button';
 302      x.title = `Stop sending to ${ip}`;
 303      x.textContent = '×';
 304      x.onclick = () => act(async () => {
 305        S.settings.extra_destinations = (await api('/api/destinations/remove', { ip })).extra_destinations;
 306        renderDestinations();
 307      });
 308      chip.append(x);
 309      box.append(chip);
 310    }
 311    // Quick add for the computer this page is open on (as the server sees it)
 312    const me = $('#destAddMe');
 313    const ip = S.clientIp;
 314    const usable = ip && !ip.startsWith('127.') && !extras.includes(ip) && !(S.baseDestinations || []).includes(ip);
 315    me.hidden = !usable;
 316    if (usable) me.textContent = `+ This Computer (${ip})`;
 317  }
 318  
 319  async function addDestination(ip) {
 320    S.settings.extra_destinations = (await api('/api/destinations/add', { ip })).extra_destinations;
 321    renderDestinations();
 322    toast(`Now also sending to ${ip}`, 'success', 3000);
 323  }
 324  
 325  async function loadState() {
 326    const st = await api('/api/state');
 327    S.features = st.features || { mode: 'lab', phasespace: true, hardware: true, simulation: false };
 328    applyFeatures(st);
 329    if (S.features.mode === 'local') {
 330      S.host = st.host;
 331      S.projectDir = st.project_dir;
 332      renderHost();
 333      resetLogs(st.logs);
 334      simulation.onState(st);
 335      experiment.onState(st);
 336      results.onMatlab(st.matlab || { connected: false });
 337      S.lab = st.lab || null;
 338      renderConn();
 339      designer.onState(st);
 340      calibration.onState(st);
 341      return;
 342    }
 343    S.clientIp = st.client_ip;
 344    S.baseDestinations = st.base_destinations;
 345    S.host = st.host;
 346    S.projectDir = st.project_dir;
 347    S.settings = st.settings;
 348    S.toolchains = st.toolchains;
 349    S.build = st.build;
 350    S.stream = st.stream;
 351    S.monitor = st.monitor;
 352    S.ledOwner = st.led_owner || {};
 353    viewer?.setLedOwner(S.ledOwner);
 354    resetLogs(st.logs);
 355    renderHost(); renderSettings(); renderTargets(); renderStream(); renderMonitor(); renderDestinations();
 356    hardware.setAll(st.hardware);
 357    interactive.onStream(st.stream);
 358    interactive.onHardware(st.hardware);
 359    interactive.setState(st.interactive, st.interactive_info);
 360    designer.onState(st);
 361    calibration.onState(st);
 362  }
 363  
 364  function setConn(kind) {
 365    S.conn = kind;
 366    renderConn();
 367  }
 368  
 369  // The badge in the top right. On the lab console: whether this page reaches it. On the local
 370  // console (SPOT in MATLAB), which always runs on this computer: whether the lab console (TX2) is
 371  // reachable, since that is what decides if experiments and the Lab console link work.
 372  function renderConn() {
 373    const b = $('#connBadge'), link = $('#labLink');
 374    let kind = S.conn || 'warn';
 375    let text = kind === 'ok' ? 'Connected' : kind === 'bad' ? 'Server Offline' : 'Connecting';
 376    let title = kind === 'bad' ? 'This page lost its server' : '';
 377    const local = S.features && S.features.mode === 'local';
 378    if (local && kind === 'ok' && S.lab) {
 379      const why = (S.lab.error || '').replace(/^Not reachable \((.*)\)$/, '$1');
 380      kind = S.lab.reachable ? 'ok' : 'bad';
 381      text = S.lab.reachable ? 'Lab Connected' : 'Lab Disconnected';
 382      title = S.lab.reachable ? `The lab console (${S.lab.url}) is reachable`
 383        : `The lab console (${S.lab.url || 'TX2'}) isn't reachable${why ? `: ${why}` : ''}. Connect to the lab network (SRCL Wi-Fi or wired) for experiments and the live view.`;
 384    }
 385    b.className = `badge badge-${kind}`;
 386    b.innerHTML = `<span class="dot"></span>${text}`;
 387    b.title = title;
 388    if (local) {
 389      const off = !(S.lab && S.lab.reachable) || S.conn !== 'ok';
 390      link.classList.toggle('disabled', off);
 391      link.setAttribute('aria-disabled', String(off));
 392      link.tabIndex = off ? -1 : 0;
 393      link.title = off ? "The lab console isn't reachable from this network"
 394        : 'PhaseSpace and the platform hardware run on the lab console (the TX2)';
 395    }
 396  }
 397  
 398  function connect() {
 399    const es = new EventSource('/api/events');
 400    const on = (name, fn) => es.addEventListener(name, e => fn(JSON.parse(e.data)));
 401    es.onopen = () => { setConn('ok'); act(loadState); };
 402    es.onerror = () => setConn('bad');
 403    on('log', e => { if (e.time > S.historyUntil) queueLog(e); });
 404    on('toolchains', d => { S.toolchains = d; renderTargets(); });
 405    on('build_status', d => {
 406      const finished = S.build.running && !d.running;
 407      if (d.running && !S.build.running) showTab('build');
 408      S.build = d; renderTargets();
 409      if (finished && d.last) toast(d.last.ok ? `StreamData compiled into ${d.last.out_dir}` : 'Build failed; see the Build log.',
 410                                    d.last.ok ? 'success' : 'error');
 411    });
 412    on('stream_status', d => {
 413      if (d.running && !S.stream.running) showTab('stream');
 414      // Let everyone watching know when someone else (another browser or a script) starts or stops it
 415      if (Boolean(d.running) !== Boolean(S.stream.running) && !S.ownStreamAction && S.stream.exe) {
 416        if (d.running) toast(`Streaming started at ${d.frequency} Hz by ${d.started_by}`, 'success');
 417        else toast(d.stopped_by ? `Streaming stopped by ${d.stopped_by}` : `StreamData exited (code ${d.last_exit})`,
 418                   d.stopped_by ? 'info' : 'error');
 419      }
 420      S.stream = d; renderStream();
 421      interactive.onStream(d);
 422      calibration.onStream(d);
 423    });
 424    on('settings', d => { S.settings = d; renderSettings(); renderDestinations(); });
 425    on('monitor', d => { S.monitor = d; renderMonitor(); });
 426    on('hardware_platform', p => { hardware.setPlatform(p); interactive.onHardwarePlatform(p); });
 427    on('interactive', d => interactive.onStatus(d));
 428    on('matlab', m => { simulation.onMatlab(m); experiment.onMatlab(m); results.onMatlab(m); });
 429    on('build', b => experiment.onBuild(b));
 430    on('build_log', e => experiment.onBuildLog(e));
 431    on('lab', l => {
 432      S.lab = l;
 433      if (l && l.url) $('#labLink').href = l.url;   // (the address can change: Experiment tab, Lab)
 434      renderConn(); experiment.onLab(l); designer.onLab(l); calibration.onLab(l);
 435    });
 436    on('design', d => designer.onDesign(d));
 437    on('lab_design', d => designer.onLabDesign(d));
 438    on('calibration', d => {
 439      calibration.onCalibration(d);
 440      // (lab console: Interactive Mode's controller now flies with the new calibrated thrust; its card says so)
 441      if (S.features && S.features.mode !== 'local') act(async () => { const r = await api('/api/interactive'); interactive.setState(r, r.controller); });
 442    });
 443    on('calibration_run', d => calibration.onRun(d));
 444    on('lab_calibration', d => calibration.onLabCalibration(d));
 445    on('lab_frame', f => experiment.onFrame(f));
 446    on('lab_stream', d => experiment.onStream(d));
 447    on('lab_monitor', d => experiment.onMonitor(d));
 448    on('lab_hardware', d => experiment.onHardware(d));
 449    on('lab_experiment', d => experiment.onExperiment(d));
 450    on('lab_live_data', d => livePlot.experimentData(d));
 451    on('sim', d => { if (!d.running && d.ok) results.onSimFinished(); simulation.onSim(d); });
 452    on('matlab_event', e => simulation.onMatlabEvent(e));
 453    on('frame', frame => {
 454      S.lastFrameAt = performance.now();
 455      viewer?.update(frame);
 456      interactive.onFrame(frame);
 457      if (S.lastFrameAt - S.lastTelemetryAt > 100) { S.lastTelemetryAt = S.lastFrameAt; renderTelemetry(frame); }
 458      $('#viewerEmpty').classList.add('hidden');
 459    });
 460  }
 461  
 462  // ---------------------------------------------------------------- folder dialog
 463  
 464  const dialog = $('#folderDialog');
 465  let folder = null;
 466  
 467  async function openFolder(path) {
 468    const d = await api('/api/browse?path=' + encodeURIComponent(path || ''));
 469    folder = d;
 470    $('#folderPath').value = d.path;
 471    $('#folderUp').disabled = !d.parent;
 472    const list = $('#folderList');
 473    list.textContent = '';
 474    if (d.error) {
 475      list.innerHTML = `<li class="empty"></li>`;
 476      list.firstChild.textContent = `Can't open this folder: ${d.error}`;
 477    } else if (!d.dirs.length) {
 478      list.innerHTML = '<li class="empty">No Subfolders</li>';
 479    }
 480    for (const name of d.dirs) {
 481      const li = document.createElement('li');
 482      li.textContent = name;
 483      li.title = name;
 484      li.onclick = () => act(() => openFolder(d.path.endsWith(d.sep) ? d.path + name : d.path + d.sep + name));
 485      list.append(li);
 486    }
 487    const places = $('#folderPlaces');
 488    places.textContent = '';
 489    let drivesStarted = false;
 490    for (const p of d.places) {
 491      if (p.drive && !drivesStarted) { drivesStarted = true; places.append(document.createElement('hr')); }
 492      const b = document.createElement('button');
 493      b.type = 'button';
 494      b.textContent = p.label;
 495      b.title = p.path;
 496      b.onclick = () => act(() => openFolder(p.path));
 497      places.append(b);
 498    }
 499  }
 500  
 501  $('#browseBtn').onclick = () => act(async () => {
 502    await openFolder(S.settings.output_dir);
 503    dialog.showModal();
 504  });
 505  $('#folderUp').onclick = () => folder?.parent && act(() => openFolder(folder.parent));
 506  $('#folderGo').onclick = () => act(() => openFolder($('#folderPath').value));
 507  $('#folderPath').addEventListener('keydown', e => {
 508    if (e.key === 'Enter') { e.preventDefault(); act(() => openFolder($('#folderPath').value)); }
 509  });
 510  $('#newFolderName').addEventListener('keydown', e => {
 511    if (e.key === 'Enter') { e.preventDefault(); $('#newFolderBtn').click(); }
 512  });
 513  $('#newFolderBtn').onclick = () => act(async () => {
 514    const name = $('#newFolderName').value.trim();
 515    if (!name) return;
 516    const r = await api('/api/mkdir', { parent: folder.path, name });
 517    $('#newFolderName').value = '';
 518    await openFolder(r.path);
 519  });
 520  $('#folderSelect').onclick = () => act(async () => {
 521    S.settings = await api('/api/settings', { output_dir: folder.path });
 522    renderSettings();
 523    dialog.close();
 524    toast(`Output folder set to ${folder.path}`, 'success', 4000);
 525  });
 526  
 527  // ---------------------------------------------------------------- controls
 528  
 529  $('[data-role=build]').onclick = () => act(async () => {
 530    S.build = await api('/api/build', {});
 531    renderTargets();
 532    showTab('build');
 533  });
 534  $('#recheckBtn').onclick = () => act(async () => { S.toolchains = await api('/api/toolchains/refresh', {}); renderTargets(); });
 535  
 536  $('#streamBtn').onclick = () => act(async () => {
 537    const btn = $('#streamBtn');
 538    btn.disabled = true;
 539    S.ownStreamAction = true;
 540    try {
 541      S.stream = S.stream.running
 542        ? await api('/api/stream/stop', {})
 543        : await api('/api/stream/start', { frequency: Number($('#frequency').value) });
 544      if (S.stream.running) showTab('stream');
 545    } finally {
 546      S.ownStreamAction = false;
 547      renderStream();
 548    }
 549  });
 550  $('#frequency').addEventListener('change', () => act(async () => {
 551    S.settings = await api('/api/settings', { frequency: Number($('#frequency').value) });
 552    renderSettings();
 553  }));
 554  for (const b of $$('[data-transport]')) {
 555    b.onclick = () => act(async () => {
 556      S.settings = await api('/api/settings', { transport: b.dataset.transport });
 557      renderSettings();
 558    });
 559  }
 560  $('#destAdd').onclick = () => act(async () => {
 561    const ip = $('#destInput').value.trim();
 562    if (!ip) return;
 563    await addDestination(ip);
 564    $('#destInput').value = '';
 565  });
 566  $('#destInput').addEventListener('keydown', e => { if (e.key === 'Enter') { e.preventDefault(); $('#destAdd').click(); } });
 567  $('#destAddMe').onclick = () => act(() => addDestination(S.clientIp));
 568  $('#savePortBtn').onclick = () => act(async () => {
 569    S.settings = await api('/api/settings', { monitor_port: Number($('#monitorPort').value) });
 570    renderSettings();
 571    toast(`Viewer port set to ${S.settings.monitor_port}`, 'success', 3000);
 572  });
 573  
 574  for (const b of $$('.tabs button')) b.onclick = () => showTab(b.dataset.tab);
 575  $('#clearLog').onclick = () => {
 576    const tab = $('.tabs button.active').dataset.tab;
 577    logEls[tab].textContent = '';
 578    logCounts[tab] = 0;
 579    $(`[data-count=${tab}]`).textContent = '0';
 580  };
 581  $('#clearHwLog').onclick = () => {
 582    logEls.hardware.textContent = '';
 583    logCounts.hardware = 0;
 584    $('[data-count=hardware]').textContent = '0';
 585  };
 586  $('#toggleConsole').onclick = () => {
 587    const c = $('#console');
 588    c.classList.toggle('collapsed');
 589    $('#toggleConsole').textContent = c.classList.contains('collapsed') ? '▴' : '▾';
 590  };
 591  
 592  // ---------------------------------------------------------------- pages
 593  
 594  const PAGES = { simulation: $('#pageSimulation'), experiment: $('#pageExperiment'), results: $('#pageResults'),
 595                  phasespace: $('#pagePhasespace'), hardware: $('#pageHardware'), designer: $('#pageDesigner'),
 596                  calibration: $('#pageCalibration') };
 597  
 598  function pageAvailable(page) {
 599    return Boolean(PAGES[page]) && (!S.features || S.features[page]);
 600  }
 601  
 602  function showPage(page) {
 603    if (!pageAvailable(page)) page = S.features && S.features.mode === 'local' ? 'simulation' : 'phasespace';
 604    for (const b of $$('[data-page]')) b.classList.toggle('active', b.dataset.page === page);
 605    for (const [name, el] of Object.entries(PAGES)) el.hidden = name !== page;
 606    storage.set('spot.page', page);
 607    if (location.hash !== `#${page}`) history.replaceState(null, '', `#${page}`);
 608    if (page === 'hardware') logEls.hardware.scrollTop = logEls.hardware.scrollHeight;
 609    // One 3D view for every page: it moves into the page's slot, with that page's layers
 610    const slot = $(`[data-slot="${page}"]`);
 611    const view = $('#viewer');
 612    if (slot && view.parentElement !== slot) slot.prepend(view);
 613    view.dataset.showing = page;
 614    // The MATLAB command window is shared the same way by the Simulation and Experiment pages
 615    const consoleSlot = $(`[data-console-slot="${page}"]`);
 616    if (consoleSlot && !consoleSlot.contains($('#matlabConsole'))) consoleSlot.append($('#matlabConsole'));
 617    if (page === 'simulation' || page === 'experiment') messages.show(page);   // the messages panel, like the 3D view
 618    if (['simulation', 'experiment', 'results'].includes(page)) livePlot.show(page === 'results' ? null : page);   // no Live Plot on Results
 619    if (page !== 'simulation') simulation.hide();
 620    if (page !== 'experiment') experiment.hide();
 621    if (page !== 'results') results.hide();
 622    if (page !== 'designer') designer.hide();
 623    if (page !== 'calibration') calibration.hide();
 624    if (page === 'simulation') simulation.show();
 625    else if (page === 'experiment') experiment.show();
 626    else if (page === 'results') results.show();
 627    else if (page === 'designer') designer.show();
 628    else if (page === 'calibration') calibration.show();
 629    else viewer?.setLayers({ live: true, sim: $('#toggleOverlay').checked && simulation.hasResult(), setup: false });
 630    if (page !== 'experiment') viewer?.setLivePaths(false);
 631  }
 632  
 633  function applyFeatures(st) {
 634    const f = S.features;
 635    for (const b of $$('[data-page]')) b.hidden = !f[b.dataset.page];
 636    $('.page-tabs').hidden = $$('[data-page]').filter(b => !b.hidden).length < 2;
 637    $('#labLink').hidden = f.mode !== 'local' || !st.lab_url;
 638    $('#settingsBar').hidden = f.mode !== 'local';   // settings live in MATLAB (local console only)
 639    if (st.lab_url) $('#labLink').href = st.lab_url;
 640    $('.topstats .stat').hidden = !f.phasespace;
 641    // The lab console (TX2) drives the hardware; the local one (SPOT in MATLAB) runs simulations
 642    const name = f.mode === 'local' ? 'SPOT Local Simulations' : 'SPOT Hardware Interfaces';
 643    document.title = name;
 644    $('.brand h1').textContent = name;
 645    if (!S.pageShown) {
 646      S.pageShown = true;
 647      showPage(location.hash.slice(1) || storage.get('spot.page', f.mode === 'local' ? 'simulation' : 'phasespace'));
 648    }
 649  }
 650  
 651  for (const b of $$('[data-page]')) b.onclick = () => showPage(b.dataset.page);
 652  window.addEventListener('hashchange', () => showPage(location.hash.slice(1)));
 653  const hardware = initHardware({ api, act, toast, storage, fmtTime });
 654  
 655  // ---------------------------------------------------------------- viewer
 656  
 657  try {
 658    viewer = new Viewer($('#viewer'), { capture: CAPTURE });
 659    if (CAPTURE) window.spotViewer = viewer;   // for screenshots and scripted checks of the 3D view
 660  } catch (e) {
 661    console.error(e);
 662    $('#viewerEmpty strong').textContent = '3D View Unavailable';
 663    $('#viewerEmptyDetail').textContent = 'This browser could not start WebGL. Telemetry still updates on the left.';
 664  }
 665  
 666  for (const b of $$('[data-view]')) {
 667    b.onclick = () => {
 668      for (const o of $$('[data-view]')) o.classList.toggle('active', o === b);
 669      viewer?.setView(b.dataset.view);
 670    };
 671  }
 672  const toggles = [['#toggleLedLabels', 'ledLabels', v => viewer?.setLedLabels(v)],
 673                   ['#toggleNames', 'names', v => viewer?.setNames(v)],
 674                   ['#toggleAxes', 'bodyAxes', v => viewer?.setBodyAxes(v)],
 675                   ['#toggleDrop', 'dropLines', v => viewer?.setDropLines(v)]];
 676  for (const [sel, key, apply] of toggles) {
 677    const input = $(sel);
 678    input.checked = storage.get('spot.' + key, true);
 679    apply(input.checked);
 680    input.onchange = () => { storage.set('spot.' + key, input.checked); apply(input.checked); };
 681  }
 682  
 683  // Use CAD: off unless switched on (the models are loaded then); if they can't be loaded, it goes off
 684  const cadToggle = $('#toggleCad');
 685  const useCad = on => viewer?.setUseCad(on).catch(e => {
 686    cadToggle.checked = false;
 687    storage.set('spot.useCad', false);
 688    toast('Could not load the CAD models: ' + e.message, 'error');
 689  });
 690  cadToggle.checked = storage.get('spot.useCad', false);
 691  if (cadToggle.checked) useCad(true);
 692  cadToggle.onchange = () => { storage.set('spot.useCad', cadToggle.checked); useCad(cadToggle.checked); };
 693  
 694  // ZED2 Viewpoint window: off unless switched on; its own close button also turns the switch off
 695  const redCamToggle = $('#toggleRedCam');
 696  redCamToggle.checked = storage.get('spot.redCam', false);
 697  viewer?.setRedCam(redCamToggle.checked);
 698  redCamToggle.onchange = () => { storage.set('spot.redCam', redCamToggle.checked); viewer?.setRedCam(redCamToggle.checked); };
 699  if (viewer) viewer.redCam.onClose = () => { redCamToggle.checked = false; storage.set('spot.redCam', false); };
 700  // which lens it shows (left, right or both side by side), remembered
 701  viewer?.setRedCamLens(storage.get('spot.redCamLens', 'left'));
 702  if (viewer) viewer.redCam.onLens = lens => storage.set('spot.redCamLens', lens);
 703  
 704  const store = createDataStore({ api, toast });
 705  const messages = initMessages({ storage });
 706  const livePlot = initLivePlot({ api, storage });
 707  const simulation = initSimulation({ api, act, toast, storage, viewer, store, messages, livePlot });
 708  const experiment = initExperiment({ api, act, toast, storage, viewer, messages, livePlot, getSim: () => simulation.getState(),
 709                                     setPlatform: (name, on) => simulation.setPlatform(name, on),
 710                                     setSetting: (path, value) => simulation.setSetting(path, value),
 711                                     printError: (title, details) => simulation.printError(title, details) });
 712  const results = initResults({ act, toast, storage, store, showPage });
 713  const interactive = initInteractive({ api, act, toast, viewer, isShown: () => !$('#pagePhasespace').hidden });
 714  // (on the local console the Chaser's arm in the Designer and the simulation's Arm platform are one choice;
 715  // the lab console has no settings, so its Designer stands alone)
 716  const designer = initDesigner({ api, act, toast, storage, viewer, getSim: () => simulation.getState(),
 717                                  setArm: (on, mount) => simulation.setArm(on, mount) });
 718  const calibration = initCalibration({ api, act, toast, storage, viewer, getStream: () => S.stream });
 719  // The table slope overlay on the 3D view: off unless switched on; only with a table calibration
 720  const slopeToggle = $('#toggleSlope');
 721  slopeToggle.checked = storage.get('spot.tableSlope', false);
 722  viewer?.setTableSlopeShown(slopeToggle.checked);
 723  slopeToggle.onchange = () => { storage.set('spot.tableSlope', slopeToggle.checked); viewer?.setTableSlopeShown(slopeToggle.checked); };
 724  document.addEventListener('spot-calibration', ev => {
 725    const has = Boolean(ev.detail && ev.detail.table);
 726    $('#toggleSlopeLabel').hidden = !has;
 727  });
 728  $('#toggleSlopeLabel').hidden = true;
 729  $('#toggleOverlay').onchange = () => viewer?.setLayers({ sim: $('#toggleOverlay').checked && simulation.hasResult() });
 730  
 731  // ---------------------------------------------------------------- start
 732  
 733  renderTelemetryEmpty();
 734  renderTargets();
 735  act(loadState);
 736  setInterval(() => { if (S.features && S.features.mode === 'local') return; if (S.stream.running) renderStream(); updateStale(); }, 1000);
 737  connect();
```

</details>

<details>
<summary><b>+spot/+console/webapp/static/index.html</b> (792 lines; click to show)</summary>

```html
   1  <!doctype html>
   2  <html lang="en">
   3  <head>
   4    <meta charset="utf-8">
   5    <meta name="viewport" content="width=device-width, initial-scale=1">
   6    <title>SPOT Console</title>
   7    <link rel="icon" type="image/png" href="/static/srl_logo.png">
   8    <link rel="stylesheet" href="/static/app.css">
   9    <script type="importmap">{ "imports": { "three": "/static/vendor/three.module.min.js" } }</script>
  10    <script type="module" src="/static/app.js"></script>
  11  </head>
  12  <body>
  13    <header class="topbar">
  14      <div class="brand">
  15        <img src="/static/srl_logo.png" alt="SRL" class="brand-logo">
  16        <div>
  17          <h1>SPOT Console</h1>
  18          <p id="hostLine">Connecting…</p>
  19        </div>
  20      </div>
  21      <nav class="segmented page-tabs" aria-label="Pages">
  22        <button type="button" data-page="simulation" hidden>Simulation</button>
  23        <button type="button" data-page="experiment" hidden>Experiment</button>
  24        <button type="button" data-page="results" hidden>Results</button>
  25        <button type="button" data-page="phasespace" class="active">PhaseSpace</button>
  26        <button type="button" data-page="hardware">Hardware<span id="hwLiveDot" class="live-dot" title="Something is switched on" hidden></span></button>
  27        <button type="button" data-page="designer" hidden title="Spacecraft Designer: the docking cones and solar panels on each platform">Designer</button>
  28        <button type="button" data-page="calibration" hidden title="Measure the lab (the table's slope, the thrusters, the arm) so simulations behave as the experiments do">Calibration</button>
  29      </nav>
  30      <div id="settingsBar" class="settings-bar" role="group" aria-label="Simulation and experiment settings" hidden>
  31        <span class="sb-label" title="The settings on the Simulation tab (scenario, timeline, initial conditions, platforms, control, solver), used for simulations and for building experiments. They are saved automatically to this file in the project folder.">
  32          <span class="sb-title">Settings</span><span id="settingsFile" class="sb-file mono">spot_settings.json</span></span>
  33        <button type="button" id="simOpen" class="btn btn-sm" title="Load settings from another file: a .json saved with Save As…, or a state saved by the old SPOT GUI (.mat). MATLAB's file dialog opens."><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round" d="M3 7a2 2 0 0 1 2-2h4l2 2h8a2 2 0 0 1 2 2v8a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z"/></svg><span>Load…</span></button>
  34        <button type="button" id="simSaveAs" class="btn btn-sm" title="Save a copy of the current settings to a .json file of your choice (MATLAB's file dialog opens)"><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round" d="M5 3h11l3 3v13a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2zM8 3v5h7V3M8 21v-7h8v7"/></svg><span>Save As…</span></button>
  35        <button type="button" id="simReset" class="btn btn-sm" title="Start from a clean slate: the template's default settings, the page's default view, no loaded results (you choose which)"><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" d="M4 4v5h5M4.6 15a8 8 0 1 0 1.9-8.3L4 9"/></svg><span>Reset…</span></button>
  36      </div>
  37      <div class="topstats">
  38        <a id="labLink" class="btn btn-sm" target="_blank" rel="noopener" hidden title="PhaseSpace and the platform hardware run on the lab console (the TX2)">Lab Console ↗</a>
  39        <span class="stat"><span class="stat-label">Packets</span><span id="statRate" class="stat-value">0 Hz</span></span>
  40        <span id="connBadge" class="badge badge-warn"><span class="dot"></span>Connecting</span>
  41      </div>
  42    </header>
  43  
  44    <div id="pagePhasespace" class="layout page" hidden>
  45      <aside class="sidebar">
  46  
  47        <section class="card">
  48          <div class="card-head">
  49            <h2>Compile</h2>
  50            <button id="recheckBtn" class="btn btn-sm" title="Check again which compilers are installed">Re-Check</button>
  51          </div>
  52          <label class="field-label tip" for="outputDir" title="StreamData and libowlsock.so are built into the linux-<architecture> subfolder of this folder">Output Folder</label>
  53          <div class="input-row">
  54            <input id="outputDir" class="input mono" readonly placeholder="Choose a folder…">
  55            <button id="browseBtn" class="btn">Browse…</button>
  56          </div>
  57  
  58          <div class="targets">
  59            <div class="target" data-target="linux">
  60              <div class="target-head">
  61                <svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M12 2c-2.2 0-3.6 1.9-3.6 4.6 0 1.4.3 2.3-.5 3.7C6.6 12.4 5 14.6 5 17c0 .6.1 1.1.3 1.6-.6.3-1.3.8-1.3 1.6 0 1 1.2 1.3 2.8 1.6 1.4.3 2.2.7 3 .7.9 0 1.4-.6 2.2-.6s1.3.6 2.2.6c.8 0 1.6-.4 3-.7 1.6-.3 2.8-.6 2.8-1.6 0-.8-.7-1.3-1.3-1.6.2-.5.3-1 .3-1.6 0-2.4-1.6-4.6-2.9-6.7-.8-1.4-.5-2.3-.5-3.7C15.6 3.9 14.2 2 12 2zm-1.6 4.2c.4 0 .7.5.7 1.1s-.3 1.1-.7 1.1-.7-.5-.7-1.1.3-1.1.7-1.1zm3.2 0c.4 0 .7.5.7 1.1s-.3 1.1-.7 1.1-.7-.5-.7-1.1.3-1.1.7-1.1zM12 9.4c.9 0 2 .6 2 1s-1.1 1.1-2 1.1-2-.7-2-1.1 1.1-1 2-1z"/></svg>
  62                <span>StreamData (Linux)</span>
  63              </div>
  64              <p class="target-status" data-role="status">Checking…</p>
  65              <p class="target-last" data-role="last"></p>
  66              <button class="btn btn-primary btn-block" data-role="build">Compile</button>
  67            </div>
  68          </div>
  69        </section>
  70  
  71        <section class="card">
  72          <div class="card-head">
  73            <h2>Stream</h2>
  74            <span id="streamPill" class="pill">Stopped</span>
  75          </div>
  76          <div class="stream-row">
  77            <label class="field" for="frequency">
  78              <span class="field-label">Frequency</span>
  79              <span class="input-suffix"><input id="frequency" class="input" type="number" min="1" max="960" step="1" value="100"><span>Hz</span></span>
  80            </label>
  81            <button id="streamBtn" class="btn btn-go">
  82              <svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg>
  83              <span>Start Streaming</span>
  84            </button>
  85          </div>
  86          <div class="segmented transport" role="group" aria-label="PhaseSpace transport" title="How StreamData receives frames from PhaseSpace">
  87            <button type="button" data-transport="tcp" title="Every frame arrives, in order; a late frame holds up the ones behind it">TCP</button>
  88            <button type="button" data-transport="udp" title="A late frame is dropped instead of delaying the rest (lower latency)">UDP</button>
  89          </div>
  90          <p id="transportHint" class="hint warn-text" hidden></p>
  91          <p id="exeLine" class="hint" hidden></p>
  92          <div class="dests">
  93            <div class="dests-head">
  94              <span id="destsLabel" class="field-label tip">Destinations</span>
  95              <button type="button" id="destAddMe" class="btn btn-sm" hidden></button>
  96            </div>
  97            <div id="extraDests" class="chips"></div>
  98            <div class="input-row">
  99              <input id="destInput" class="input mono" placeholder="Add an IP, e.g. 192.168.1.43" spellcheck="false">
 100              <button type="button" id="destAdd" class="btn btn-sm" title="Starts receiving immediately, even mid-stream, and stays until removed">Add</button>
 101            </div>
 102          </div>
 103  
 104          <details class="advanced">
 105            <summary>Viewer Settings</summary>
 106            <label class="field" for="monitorPort">
 107              <span class="field-label tip" title="StreamData sends a copy of every packet to 127.0.0.1 on this port for the 3D view. SPOT's own port 31534 is unaffected.">Viewer UDP Port</span>
 108              <span class="input-row"><input id="monitorPort" class="input" type="number" min="1024" max="65535"><button id="savePortBtn" class="btn btn-sm">Save</button></span>
 109            </label>
 110          </details>
 111        </section>
 112  
 113        <section class="card">
 114          <div class="card-head">
 115            <h2>Telemetry</h2>
 116            <span id="lastPacket" class="muted small">No Packets Yet</span>
 117          </div>
 118          <table class="telemetry">
 119            <thead><tr><th></th><th>x<br><span class="th-unit">[m]</span></th><th>y<br><span class="th-unit">[m]</span></th><th>θ<br><span class="th-unit">[°]</span></th><th>dx/dt<br><span class="th-unit">[m/s]</span></th><th>dy/dt<br><span class="th-unit">[m/s]</span></th><th>dθ/dt<br><span class="th-unit">[°/s]</span></th></tr></thead>
 120            <tbody id="telemetryBody"></tbody>
 121          </table>
 122          <div class="led-head"><span class="field-label">Visible LEDs</span><span id="ledCount" class="muted small">0</span></div>
 123          <div id="ledChips" class="chips"></div>
 124        </section>
 125  
 126        <section class="card ix-card">
 127          <div class="card-head">
 128            <h2 class="tip" title="Fly platforms to goals you drag on the table. This console runs the template diagram's own controller (its default LQR and thruster mixer) for each platform from the PhaseSpace stream and sends the thrusters' duty cycles to it. Needs streaming, and the platforms Ready on the Hardware tab.">Interactive Mode</h2>
 129            <label class="toggle" title="Show the controls and the goals on the table. Nothing moves until you press Fly."><input id="toggleInteractive" type="checkbox"><span></span></label>
 130          </div>
 131          <div id="ixBody" hidden>
 132            <div class="ix-rows">
 133              <div class="ix-row" data-ixrow="RED">
 134                <div class="ix-row-head">
 135                  <label class="toggle" title="Give the Chaser a goal on the table; Fly to Goals sends it there. Switching it off while it flies stops it (everything off)."><input type="checkbox" data-ixon="RED"><span></span></label>
 136                  <i class="swatch"></i><b>Chaser</b>
 137                  <span class="pill" data-role="state"></span>
 138                  <button type="button" class="btn btn-sm" data-role="here" title="Put the Chaser's goal where it is now">Here</button>
 139                </div>
 140                <div class="ix-row-body">
 141                  <span class="mono" data-role="goal"></span>
 142                  <span class="mono" data-role="error"></span>
 143                  <div class="ix-mini tip" data-role="bars" title="Thrusters T1 to T8: the duty cycle the controller sends (layout as on the Hardware tab); also drawn as plumes on the platform"><span title="T1"><i></i></span><span title="T2"><i></i></span><span title="T3"><i></i></span><span title="T4"><i></i></span><span title="T5"><i></i></span><span title="T6"><i></i></span><span title="T7"><i></i></span><span title="T8"><i></i></span></div>
 144                </div>
 145              </div>
 146              <div class="ix-row" data-ixrow="BLACK">
 147                <div class="ix-row-head">
 148                  <label class="toggle" title="Give the Target a goal on the table; Fly to Goals sends it there. Switching it off while it flies stops it (everything off)."><input type="checkbox" data-ixon="BLACK"><span></span></label>
 149                  <i class="swatch"></i><b>Target</b>
 150                  <span class="pill" data-role="state"></span>
 151                  <button type="button" class="btn btn-sm" data-role="here" title="Put the Target's goal where it is now">Here</button>
 152                </div>
 153                <div class="ix-row-body">
 154                  <span class="mono" data-role="goal"></span>
 155                  <span class="mono" data-role="error"></span>
 156                  <div class="ix-mini tip" data-role="bars" title="Thrusters T1 to T8: the duty cycle the controller sends (layout as on the Hardware tab); also drawn as plumes on the platform"><span title="T1"><i></i></span><span title="T2"><i></i></span><span title="T3"><i></i></span><span title="T4"><i></i></span><span title="T5"><i></i></span><span title="T6"><i></i></span><span title="T7"><i></i></span><span title="T8"><i></i></span></div>
 157                </div>
 158              </div>
 159              <div class="ix-row" data-ixrow="BLUE">
 160                <div class="ix-row-head">
 161                  <label class="toggle" title="Give the Obstacle a goal on the table; Fly to Goals sends it there. Switching it off while it flies stops it (everything off)."><input type="checkbox" data-ixon="BLUE"><span></span></label>
 162                  <i class="swatch"></i><b>Obstacle</b>
 163                  <span class="pill" data-role="state"></span>
 164                  <button type="button" class="btn btn-sm" data-role="here" title="Put the Obstacle's goal where it is now">Here</button>
 165                </div>
 166                <div class="ix-row-body">
 167                  <span class="mono" data-role="goal"></span>
 168                  <span class="mono" data-role="error"></span>
 169                  <div class="ix-mini tip" data-role="bars" title="Thrusters T1 to T8: the duty cycle the controller sends (layout as on the Hardware tab); also drawn as plumes on the platform"><span title="T1"><i></i></span><span title="T2"><i></i></span><span title="T3"><i></i></span><span title="T4"><i></i></span><span title="T5"><i></i></span><span title="T6"><i></i></span><span title="T7"><i></i></span><span title="T8"><i></i></span></div>
 170                </div>
 171              </div>
 172            </div>
 173            <p id="ixHint" class="hint"></p>
 174            <div class="ix-actions">
 175              <button type="button" id="ixFly" class="btn btn-go" disabled>Fly to Goals</button>
 176              <button type="button" id="ixStop" class="btn btn-stop" hidden>Stop All · Esc</button>
 177            </div>
 178            <p id="ixStats" class="hint mono"></p>
 179            <details class="advanced">
 180              <summary>Controller</summary>
 181              <div id="ixController"></div>
 182            </details>
 183          </div>
 184        </section>
 185      </aside>
 186  
 187      <main class="main">
 188        <div class="viewer-slot" data-slot="phasespace">
 189        <div id="viewer" class="viewer">
 190          <div class="viewer-toolbar">
 191            <div class="segmented" role="group" aria-label="Camera view">
 192              <button data-view="3d" class="active">3D</button>
 193              <button data-view="top">Top</button>
 194              <button data-view="side">Side</button>
 195            </div>
 196            <div class="segmented pose-picker local-only" role="group" aria-label="Initial conditions shown on the table"
 197                 title="Which poses to show on the table: where the platforms start (Initial) or where they go back to (Home). Drag them on the Simulation tab to move them.">
 198              <span class="seg-label">Initial Conditions</span>
 199              <button type="button" data-pose="off">Off</button>
 200              <button type="button" data-pose="initial">Initial</button>
 201              <button type="button" data-pose="home">Home</button>
 202            </div>
 203            <label class="toggle live-only"><input id="toggleLedLabels" type="checkbox" checked><span></span>LED IDs</label>
 204            <label class="toggle" title="Show the name tags on the platforms and the initial conditions"><input id="toggleNames" type="checkbox" checked><span></span>Labels</label>
 205            <label class="toggle" title="Show each platform's own +X/+Y/+Z axes (the table's frame stays)"><input id="toggleAxes" type="checkbox" checked><span></span>Axes</label>
 206            <label class="toggle" title="Draw the platforms from the lab's CAD instead of cubes: the Chaser in silver MLI with its ZED2 and arm, the Target in gold MLI, and the Obstacle with blue plexiglass panels on an aluminium frame, each with the docking cone and solar panels set on the Designer tab. Loaded the first time it is switched on (about 1 MB)."><input id="toggleCad" type="checkbox"><span></span>Use CAD</label>
 207            <label class="toggle" title="A window in the corner showing what the Chaser's ZED2 camera sees, with the ZED2's default HD720 intrinsics (fx = fy = 528 px, 68.6° vertical field of view), from its left lens, its right lens or both (120 mm apart, on the front face 0.28 m up, the right lens 0.08 m right of centre), facing along the Chaser's +X. It follows the Chaser live, in a replay, or at its initial conditions."><input id="toggleRedCam" type="checkbox"><span></span>ZED2 Viewpoint</label>
 208            <label class="toggle live-only" title="A vertical line from each LED PhaseSpace sees down to the table, to show how high it is"><input id="toggleDrop" type="checkbox" checked><span></span>LED Height Lines</label>
 209            <label class="toggle" id="toggleSlopeLabel" title="Colour the table by its height, from the table slope calibration (Calibration tab): blue low, red high, with contour lines"><input id="toggleSlope" type="checkbox"><span></span>Table Slope</label>
 210            <label class="toggle live-only" id="overlayToggle" hidden title="Show the last simulation on top of the live view"><input id="toggleOverlay" type="checkbox"><span></span>Simulation</label>
 211            <span class="info-tip" title="Table 3.5 × 2.4 m, origin (0, 0, 0) at the corner with the axes.&#10;Drag to orbit, right-drag to pan, scroll to zoom.">?</span>
 212          </div>
 213          <div class="legend live-only">
 214            <span><i style="background:#aeb5bd"></i>Chaser</span>
 215            <span><i style="background:#d4a326"></i>Target</span>
 216            <span><i style="background:#1c7ed6"></i>Obstacle</span>
 217            <span><i class="led" style="background:#e599f7"></i>LED Not on a Body</span>
 218          </div>
 219          <div id="viewerEmpty" class="viewer-empty">
 220            <strong>Waiting for Packets</strong>
 221            <span id="viewerEmptyDetail">Press Start Streaming to see the spacecraft.</span>
 222          </div>
 223        </div>
 224        </div>
 225  
 226        <section id="console" class="console">
 227          <div class="console-head">
 228            <div class="tabs" role="tablist">
 229              <button role="tab" data-tab="build" class="active">Build Log <span class="count" data-count="build">0</span></button>
 230              <button role="tab" data-tab="stream">Stream Log <span class="count" data-count="stream">0</span></button>
 231            </div>
 232            <div class="console-actions">
 233              <button id="clearLog" class="btn btn-sm">Clear</button>
 234              <button id="toggleConsole" class="btn btn-ghost btn-sm" aria-label="Collapse log">▾</button>
 235            </div>
 236          </div>
 237          <pre id="logBuild" class="log active" data-log="build"></pre>
 238          <pre id="logStream" class="log" data-log="stream"></pre>
 239        </section>
 240      </main>
 241    </div>
 242  
 243  
 244    <div id="pageSimulation" class="layout page sim-page with-msgs" hidden>
 245      <aside class="sidebar sim-sidebar">
 246        <div class="segmented sim-tabs" role="tablist" aria-label="Simulation settings">
 247          <button type="button" data-simtab="scenario" class="active">Scenario</button>
 248          <button type="button" data-simtab="platforms">Platforms</button>
 249          <button type="button" data-simtab="control">Control</button>
 250          <button type="button" data-simtab="solver">Solver</button>
 251          <button type="button" data-simtab="workspace">Workspace</button>
 252        </div>
 253        <div id="simNotice" class="hw-detail warn" hidden></div>
 254        <div id="simPanes" class="sim-panes"></div>
 255        <div id="wsPane" class="ws-pane" hidden>
 256          <div class="wspace-head">
 257            <input id="wsFilter" class="input" placeholder="Filter Variables" spellcheck="false">
 258            <span id="wsCount" class="muted small"></span>
 259            <button type="button" id="wsRefresh" class="btn btn-ghost btn-sm" title="Read the workspace again">↻</button>
 260          </div>
 261          <div class="wspace-table">
 262            <table class="ws-table">
 263              <thead><tr><th>Name</th><th>Value</th><th>Size</th><th>Class</th></tr></thead>
 264              <tbody id="wsBody"></tbody>
 265            </table>
 266          </div>
 267          <div id="wsInspect" class="ws-inspect" hidden></div>
 268        </div>
 269        <div class="sim-run">
 270          <label class="toggle sim-realtime" title="Run the simulation at the clock's pace (Simulink's simulation pacing): one second of simulation per second, to watch the Live Plot, the messages and the 3D view as an experiment would show them. Stop ends it early."><input id="simRealTime" type="checkbox"><span></span>Real Time <span id="simRealTimeHint" class="hint"></span></label>
 271          <div class="sim-run-row">
 272            <button type="button" id="simRun" class="btn btn-go" disabled>
 273              <svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg>
 274              <span>Run Simulation</span>
 275            </button>
 276            <button type="button" id="simSaveResult" class="btn sim-save-result" hidden title="Save this simulation's results (dataClass, with the settings it ran with) in Saved Data, to replay or plot them later on the Results tab"><svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round" d="M5 3h11l3 3v13a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2zM8 3v5h7V3M8 21v-7h8v7"/></svg><span>Save Results</span></button>
 277          </div>
 278          <p id="simStatus" class="hint">Waiting for MATLAB…</p>
 279        </div>
 280      </aside>
 281  
 282      <main class="main">
 283        <div class="viewer-slot" data-slot="simulation">
 284          <div id="simEmpty" class="viewer-empty sim-empty">
 285            <strong>No Simulation Yet</strong>
 286            <span>Run one to replay it here. Drag the platforms on the table to set their positions.</span>
 287            <button type="button" id="simEmptyLoad" class="btn btn-sm sim-empty-load" title="Replay an experiment or a saved simulation">Replay Saved Results…</button>
 288          </div>
 289          <div id="playback" class="playback" hidden>
 290            <button type="button" id="pbPlay" class="btn btn-sm pb-play" title="Play / pause (Space)">▶</button>
 291            <span id="pbTime" class="pb-time mono">0.0 / 0.0 s</span>
 292            <div class="pb-track">
 293              <div id="pbPhases" class="pb-phases"></div>
 294              <input id="pbScrub" type="range" min="0" max="1000" step="1" value="0" aria-label="Time">
 295            </div>
 296            <span id="pbPhase" class="pb-phase">Phase 0</span>
 297            <select id="pbSpeed" class="input pb-speed" title="Playback speed">
 298              <option value="0.5">0.5×</option><option value="1">1×</option><option value="2">2×</option>
 299              <option value="5" selected>5×</option><option value="10">10×</option><option value="20">20×</option>
 300            </select>
 301            <button type="button" id="pbCompare" class="btn btn-sm pb-compare" aria-haspopup="true" aria-expanded="false" title="Replay loaded results (experiments, earlier simulations) over this simulation">
 302              <svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round" d="M12 3 2 8l10 5 10-5-10-5zM2 13l10 5 10-5M2 17.5l10 5 10-5"/></svg>
 303              <span class="pb-compare-text">Compare</span><span class="pb-compare-count" hidden></span><span class="pb-caret">▾</span></button>
 304            <label class="toggle"><input id="pbTraces" type="checkbox" checked><span></span>Paths</label>
 305            <div id="pbOverlays" class="pb-overlays" hidden></div>
 306          </div>
 307        </div>
 308  
 309        <div class="console-slot" data-console-slot="simulation">
 310        <section id="matlabConsole" class="console matlab-console">
 311          <div class="console-head">
 312            <div class="console-tabs" role="tablist" aria-label="MATLAB">
 313              <button type="button" role="tab" data-ctab="cmd" class="active" title="MATLAB's command window">MATLAB <span id="matlabBadge" class="pill err">Not Connected</span></button>
 314              <button type="button" role="tab" data-ctab="plot" title="Plot the signals the diagram's Live Stream blocks send (its Live Data Streams subsystem), in simulations and experiments">Live Plot <span id="liveCount" class="count"></span></button>
 315            </div>
 316            <div class="console-actions">
 317              <button type="button" id="cmdClear" class="btn btn-sm">Clear</button>
 318              <button type="button" id="toggleMatlab" class="btn btn-ghost btn-sm" aria-label="Collapse">▾</button>
 319            </div>
 320          </div>
 321          <div class="matlab-body">
 322            <div class="cmdwin">
 323              <pre id="cmdOut" class="cmd-out"></pre>
 324              <div class="cmd-input">
 325                <span class="prompt">&gt;&gt;</span>
 326                <textarea id="cmdIn" rows="1" spellcheck="false" placeholder="MATLAB command (Enter runs, Shift+Enter for a new line, ↑ for history)"></textarea>
 327              </div>
 328            </div>
 329            <div id="livePane" class="live-pane" hidden>
 330              <aside class="live-list">
 331                <div class="live-list-head" title="The diagram's Live Stream blocks (its Live Data Streams subsystem), each named after what is connected to it. Tick one to plot it.">Data Available to Stream</div>
 332                <div id="liveStreams" class="live-streams"></div>
 333              </aside>
 334              <div class="live-main">
 335                <div class="live-tools">
 336                  <span id="liveSource" class="live-source"></span>
 337                  <div id="liveWindow" class="segmented seg-sm" role="group" aria-label="Time window"></div>
 338                  <button type="button" id="livePause" class="btn btn-sm">Pause</button>
 339                </div>
 340                <div id="liveLegend" class="live-legend"></div>
 341                <div class="live-canvas-wrap">
 342                  <canvas id="liveCanvas"></canvas>
 343                  <div id="liveEmpty" class="live-empty"></div>
 344                </div>
 345              </div>
 346            </div>
 347          </div>
 348        </section>
 349        </div>
 350      </main>
 351      <div class="msg-slot" data-msg-slot="simulation">
 352        <aside id="msgPanel" class="msg-panel" aria-label="Platform messages">
 353          <div class="msg-head">
 354            <button type="button" id="msgToggle" class="btn btn-ghost btn-sm msg-toggle" aria-expanded="true">›</button>
 355            <h2 title="Each platform's debug message: the text the diagram writes to RED_Logger (Chaser), BLACK_Logger (Target) and BLUE_Logger (Obstacle). Only changes are listed.">Messages</h2>
 356            <span id="msgCount" class="count"></span>
 357            <span id="msgSource" class="msg-source"></span>
 358          </div>
 359          <section class="msg-plat" data-plat="RED"><div class="msg-plat-head"><i class="swatch"></i><b>Chaser</b><span class="msg-now"></span><span class="msg-seen"></span></div><ol class="msg-list"></ol></section>
 360          <section class="msg-plat" data-plat="BLACK"><div class="msg-plat-head"><i class="swatch"></i><b>Target</b><span class="msg-now"></span><span class="msg-seen"></span></div><ol class="msg-list"></ol></section>
 361          <section class="msg-plat" data-plat="BLUE"><div class="msg-plat-head"><i class="swatch"></i><b>Obstacle</b><span class="msg-now"></span><span class="msg-seen"></span></div><ol class="msg-list"></ol></section>
 362        </aside>
 363      </div>
 364    </div>
 365  
 366    <div id="pageExperiment" class="layout page exp-page with-msgs" hidden>
 367      <aside class="sidebar exp-sidebar">
 368        <div id="expNotice" class="hw-detail err" hidden></div>
 369  
 370        <section class="card">
 371          <div class="card-head"><h2>Lab</h2><span id="expLabPill" class="pill">Checking…</span></div>
 372          <label class="field" for="expLabAddress" title="The Jetson TX2 that runs the lab console (the labConsoleAddress setting): it streams PhaseSpace to the platforms, builds and deploys the experiments, and receives the platforms' clocks, debug messages and live streams. After changing it, build again.">
 373            <span class="field-label">Lab Console (TX2) Address</span>
 374            <input id="expLabAddress" class="input mono lab-address" spellcheck="false" autocomplete="off" placeholder="192.168.1.116">
 375          </label>
 376          <p class="hint lab-url mono" id="expLabUrl"></p>
 377          <div class="exp-row">
 378            <div>
 379              <span class="field-label">PhaseSpace</span>
 380              <span id="expStream" class="exp-state"></span>
 381            </div>
 382          </div>
 383          <div class="stream-row sp-top">
 384            <div class="field" title="The PhaseSpace data rate (dataRate in the settings, the Simulation tab's Data Rate): the cameras start at it. Locked; fixed while they stream.">
 385              <span class="field-label lock-label"><label for="expFrequency">Data Rate</label><button type="button" class="rate-lock" data-ratelock aria-pressed="false" aria-label="Unlock the data rate"></button></span>
 386              <span class="input-suffix"><input id="expFrequency" class="input" type="number" min="1" max="960" step="1" value="10"><span>Hz</span></span>
 387            </div>
 388            <button type="button" id="expCameras" class="btn btn-sm" disabled>Start Cameras</button>
 389          </div>
 390          <p id="expStreamHint" class="hint"></p>
 391        </section>
 392  
 393        <section class="card">
 394          <div class="card-head"><h2>Platforms</h2><span class="muted small" id="expDiagram"></span></div>
 395          <div id="expPlatforms" class="exp-platforms"></div>
 396        </section>
 397  
 398        <section class="card">
 399          <div class="card-head"><h2>Build</h2><span id="expBuildPill" class="pill" hidden></span></div>
 400          <ol class="exp-steps">
 401            <li>
 402              <div class="exp-row"><span>Check the Setup</span>
 403                <button type="button" id="expPlan" class="btn btn-sm" title="Loads the diagram and lists what the build will do (nothing is compiled)">Check</button></div>
 404              <div id="expPlanOut" class="hint"></div>
 405            </li>
 406            <li>
 407              <div class="exp-row"><span>Connect MATLAB to the TX2</span>
 408                <button type="button" id="expConnect" class="btn btn-sm">Connect</button></div>
 409              <input id="expPassword" class="input" type="password" autocomplete="off" placeholder="TX2 Password (only needed the first time)">
 410              <div id="expConnectOut" class="hint"></div>
 411            </li>
 412            <li>
 413              <div class="exp-row"><span>Build on the TX2</span>
 414                <button type="button" id="expBuild" class="btn btn-primary btn-sm">Build</button></div>
 415              <div id="expBuildOut" class="hint"></div>
 416              <pre id="expBuildLive" class="exp-log exp-live" hidden></pre>
 417            </li>
 418          </ol>
 419        </section>
 420  
 421        <section class="card">
 422          <div class="card-head"><h2>Deploy &amp; Run</h2><span id="expPhasePill" class="pill" hidden></span></div>
 423          <div class="exp-row">
 424            <span id="expProgram" class="hint mono">No build found on the TX2 yet.</span>
 425            <button type="button" id="expDeploy" class="btn btn-sm">Deploy</button>
 426          </div>
 427          <div class="exp-run">
 428            <button type="button" id="expStart" class="btn btn-go">
 429              <svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg><span>Start Experiment</span></button>
 430            <button type="button" id="expStop" class="btn btn-stop">
 431              <svg viewBox="0 0 24 24" aria-hidden="true"><rect x="6" y="6" width="12" height="12" rx="2" fill="currentColor"/></svg><span>Stop</span></button>
 432          </div>
 433          <p id="expRunOut" class="hint"></p>
 434        </section>
 435  
 436        <section class="card">
 437          <div class="card-head"><h2>Data</h2></div>
 438          <div class="exp-row">
 439            <span class="hint">Copies the platforms' ExpLog files into Saved Data and converts them with MATLAB.</span>
 440            <button type="button" id="expData" class="btn btn-sm">Collect</button>
 441          </div>
 442          <p id="expDataOut" class="hint mono"></p>
 443        </section>
 444      </aside>
 445  
 446      <main class="main">
 447        <div class="viewer-slot" data-slot="experiment">
 448          <div class="exp-overlay">
 449            <div id="expClocks" class="exp-clocks"></div>
 450            <label class="toggle"><input id="expLivePaths" type="checkbox" checked><span></span>Live Paths</label>
 451            <label class="toggle"><input id="expSimOverlay" type="checkbox"><span></span>Simulation</label>
 452            <button type="button" id="expClearPaths" class="btn btn-sm">Clear Paths</button>
 453          </div>
 454          <div id="expEmpty" class="viewer-empty sim-empty">
 455            <strong>No Live Data</strong>
 456            <span id="expEmptyDetail">Start the cameras to see the platforms here.</span>
 457          </div>
 458        </div>
 459        <div class="console-slot" data-console-slot="experiment"></div>
 460      </main>
 461      <div class="msg-slot" data-msg-slot="experiment"></div>
 462    </div>
 463  
 464  
 465    <div id="pageResults" class="layout page res-page" hidden>
 466      <aside class="sidebar res-sidebar">
 467        <div id="resMatlab" class="hw-detail warn" hidden>MATLAB is not connected: run SPOT in MATLAB to load results.</div>
 468        <section class="card">
 469          <div class="card-head"><h2>Datasets</h2></div>
 470          <div class="res-load">
 471            <button type="button" id="resLoadSim" class="btn btn-sm" disabled title="A copy of the last simulation (dataClass), to compare with later ones">Last Simulation</button>
 472            <button type="button" id="resLoadSaved" class="btn btn-sm" title="Experiment runs and saved simulations in the project's Saved Data folder">Saved Data…</button>
 473            <button type="button" id="resLoadFile" class="btn btn-sm" title="Any .mat file with logged signals (timeseries), in MATLAB's file dialog">Open .mat…</button>
 474          </div>
 475          <div id="resDatasets" class="res-datasets"></div>
 476        </section>
 477        <section class="card res-signals-card">
 478          <div class="card-head"><h2>Signals</h2><span id="resSigFor" class="muted small"></span></div>
 479          <input id="resSigFilter" class="input" placeholder="Filter Signals" spellcheck="false">
 480          <div id="resSignals" class="res-signals"></div>
 481        </section>
 482      </aside>
 483  
 484      <main class="main res-main">
 485        <div class="res-toolbar">
 486          <button type="button" id="resAddPlot" class="btn btn-sm" title="Another plot below (one y axis each)">+ Plot</button>
 487          <label class="toggle" title="Time plots zoom and hover together"><input id="resLink" type="checkbox" checked><span></span>Link Time</label>
 488          <label class="res-smooth" title="Gaussian smoothing over this window (the old GUI's filter)">Smoothing
 489            <select id="resSmooth" class="input"><option value="0">Off</option><option value="0.1">0.1 s</option><option value="0.25">0.25 s</option>
 490              <option value="0.5">0.5 s</option><option value="1">1 s</option><option value="2">2 s</option></select></label>
 491          <span class="info-tip" title="Click a signal on the left to add it to the selected plot (click again to remove).&#10;Drag across a plot to zoom, double-click to zoom out.">?</span>
 492          <span class="res-spacer"></span>
 493          <details class="res-export">
 494            <summary class="btn btn-sm" title="MATLAB draws the plots in the lab's publication style (GetStarted templates) and saves or shows them">Export ▾</summary>
 495            <div class="res-export-body">
 496              <span class="hint">Drawn by MATLAB in the lab's style: Times New Roman, platform colours (Chaser silver-grey, Target gold, Obstacle blue), every sample.</span>
 497              <label>Width<input id="resExpW" class="input" type="number" min="1" max="20" step="0.1"><span>in</span></label>
 498              <label>Height<input id="resExpH" class="input" type="number" min="1" max="20" step="0.1" placeholder="auto"><span>in</span></label>
 499              <label>Font<input id="resExpF" class="input" type="number" min="6" max="20" step="1"><span>pt</span></label>
 500              <div class="res-export-actions">
 501                <button type="button" id="resSaveFig" class="btn btn-primary btn-sm" title="Choose the file in MATLAB's dialog: PDF (default), EPS, SVG or PNG at 700 dpi">Save PDF…</button>
 502                <button type="button" id="resFigure" class="btn btn-sm" title="Open it as a MATLAB figure, to adjust before saving">Open in MATLAB</button>
 503              </div>
 504            </div>
 505          </details>
 506        </div>
 507        <div id="resPlots" class="res-plots">
 508          <div id="resEmpty" class="res-empty"><strong>Nothing Plotted Yet</strong>
 509            <span>Load a dataset, then click its signals on the left.</span></div>
 510        </div>
 511        <div class="console-slot" data-console-slot="results"></div>
 512      </main>
 513    </div>
 514  
 515    <div id="pageDesigner" class="layout page dz-page" hidden>
 516      <aside class="sidebar dz-sidebar" id="dzSide" aria-label="Spacecraft Designer"></aside>
 517      <main class="main">
 518        <div class="viewer-slot" data-slot="designer">
 519          <div class="dz-overlay"><b>Spacecraft Designer</b><span>Drag to orbit, right-drag to pan, scroll to zoom. Click a disc to move the docking cone there.</span></div>
 520        </div>
 521      </main>
 522    </div>
 523  
 524    <div id="pageCalibration" class="page cal-page" hidden>
 525      <div class="cal-scroll">
 526        <div class="cal-head">
 527          <div>
 528            <h2>Calibration</h2>
 529            <p class="hint">Measurements of the lab that make simulations behave as the experiments do. The lab console
 530              runs them and keeps the lab's calibration; each project keeps a copy (resources/spot_calibration.json),
 531              which its simulations use.</p>
 532          </div>
 533          <div class="segmented cal-sections" role="group" aria-label="Calibration">
 534            <button type="button" data-calsec="table" class="active">Table Slope</button>
 535            <button type="button" data-calsec="thrusters">Thrusters</button>
 536            <button type="button" data-calsec="arm">Arm</button>
 537          </div>
 538        </div>
 539        <div id="calSync"></div>
 540        <div id="calBody" class="cal-grid"></div>
 541      </div>
 542    </div>
 543  
 544    <div id="pageHardware" class="page hw-page" hidden>
 545      <div class="hw-scroll">
 546        <div class="hw-head">
 547          <div>
 548            <h2>Platform Hardware</h2>
 549            <p class="hint">Pucks and thrusters on each platform's Teensy, sent from this console over SSH. The Teensy
 550              switches everything off when commands stop for 2 s, so things stay on only while this console is running
 551              and can reach the platform. Controls lock while another program (an experiment) has the Teensy open.</p>
 552          </div>
 553        </div>
 554        <div id="hwCards" class="hw-grid"></div>
 555      </div>
 556      <section class="console hw-console">
 557        <div class="console-head">
 558          <span class="console-title">Hardware Log <span class="count" data-count="hardware">0</span></span>
 559          <div class="console-actions">
 560            <button type="button" id="clearHwLog" class="btn btn-sm">Clear</button>
 561          </div>
 562        </div>
 563        <pre id="logHardware" class="log active" data-log="hardware"></pre>
 564      </section>
 565    </div>
 566  
 567    <dialog id="folderDialog" class="dialog">
 568      <form method="dialog" class="dialog-inner">
 569        <div class="dialog-head">
 570          <h2>Choose Output Folder</h2>
 571          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 572        </div>
 573        <div class="dialog-body">
 574          <nav id="folderPlaces" class="places"></nav>
 575          <div class="browser">
 576            <div class="input-row">
 577              <button type="button" id="folderUp" class="btn btn-sm" title="Up one level">↑</button>
 578              <input id="folderPath" class="input mono" spellcheck="false">
 579              <button type="button" id="folderGo" class="btn btn-sm">Go</button>
 580            </div>
 581            <ul id="folderList" class="folder-list"></ul>
 582            <div class="input-row">
 583              <input id="newFolderName" class="input" placeholder="New Folder Name">
 584              <button type="button" id="newFolderBtn" class="btn btn-sm">Create</button>
 585            </div>
 586          </div>
 587        </div>
 588        <div class="dialog-foot">
 589          <span class="hint">Click a folder to open it, then choose <b>Use This Folder</b>.</span>
 590          <div>
 591            <button value="cancel" class="btn">Cancel</button>
 592            <button type="button" id="folderSelect" class="btn btn-primary">Use This Folder</button>
 593          </div>
 594        </div>
 595      </form>
 596    </dialog>
 597  
 598  
 599    <dialog id="resetDialog" class="dialog reset-dialog">
 600      <form method="dialog" class="dialog-inner">
 601        <div class="dialog-head">
 602          <h2>Reset to Defaults</h2>
 603          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 604        </div>
 605        <div class="reset-body">
 606          <p class="hint">Start from a clean slate. Choose what goes back to its default:</p>
 607          <label class="reset-opt"><input type="checkbox" data-reset="settings" checked>
 608            <span><b>Simulation Settings</b><span class="hint">Scenario, timeline, initial conditions, mass properties, control and solver, as in the template.
 609              The diagram stays selected. The current settings are kept in .spot_cache, so Load… can bring them back.</span></span></label>
 610          <label class="reset-opt"><input type="checkbox" data-reset="view" checked>
 611            <span><b>Page and View</b><span class="hint">The tab shown, 3D view options, initial conditions shown, playback speed, and the Results tab's plots.
 612              The page reloads. (The MATLAB command history is kept.)</span></span></label>
 613          <label class="reset-opt"><input type="checkbox" data-reset="results" checked>
 614            <span><b>Loaded Results</b><span class="hint">Closes the datasets loaded for plotting and comparing. Nothing is deleted from Saved Data.</span></span></label>
 615          <p class="hint">Your own SPOT_User_Parameters.m and the files in Saved Data are not touched.</p>
 616        </div>
 617        <div class="dialog-foot">
 618          <div>
 619            <button value="cancel" class="btn">Cancel</button>
 620            <button type="button" id="resetGo" class="btn btn-stop">Reset</button>
 621          </div>
 622        </div>
 623      </form>
 624    </dialog>
 625  
 626    <dialog id="rateLockDialog" class="dialog reset-dialog">
 627      <form method="dialog" class="dialog-inner">
 628        <div class="dialog-head">
 629          <h2>Unlock the Data Rate?</h2>
 630          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 631        </div>
 632        <div class="reset-body">
 633          <p>The <b>data rate</b> is how often the PhaseSpace cameras send each platform's position during an experiment:
 634            the lab console streams at this rate when the Experiment tab starts the cameras. The Simulation tab's Data Rate
 635            and the Experiment tab's are the same setting (<span class="mono">dataRate</span>).</p>
 636          <p>It is locked because the program is built around it:</p>
 637          <ul class="lock-reasons">
 638            <li>The default controllers' LQR gains are designed for measurements at this rate (MATLAB's
 639              <span class="mono">dataRate</span> is its period), so another rate gives other gains.</li>
 640            <li>The measured positions, and the speeds worked out from them, come at this rate, in simulations as in experiments.</li>
 641            <li>The program on the platforms keeps the rate it was built with: after a change, build again, and start the
 642              cameras at the new rate before you run it.</li>
 643          </ul>
 644          <p class="hint">Keep the rate the experiment was designed for (10 Hz by default); it is a whole number of Hz, 1 to 960.
 645            The lock comes back when the page is reloaded.</p>
 646        </div>
 647        <div class="dialog-foot">
 648          <div>
 649            <button value="cancel" class="btn">Keep It Locked</button>
 650            <button type="button" id="rateUnlockGo" class="btn btn-stop">Unlock</button>
 651          </div>
 652        </div>
 653      </form>
 654    </dialog>
 655  
 656    <dialog id="pwmLockDialog" class="dialog reset-dialog">
 657      <form method="dialog" class="dialog-inner">
 658        <div class="dialog-head">
 659          <h2>Unlock the Thruster PWM?</h2>
 660          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 661        </div>
 662        <div class="reset-body">
 663          <p>The <b>thruster PWM frequency</b> is how often each thruster valve opens and closes: a duty cycle of 40 %
 664            at 5 Hz opens the valve for 80 ms every 200 ms (<span class="mono">PWMFreq</span> in the diagram).</p>
 665          <p>It is locked because it has to match the hardware:</p>
 666          <ul class="lock-reasons">
 667            <li>The Teensy on each platform runs its PWM at a fixed 5 Hz (<span class="mono">PWM_FREQUENCY</span> in
 668              <span class="mono">teensy_pwm_control.ino</span>). Another value here makes the diagram's duty cycles wrong on
 669              the real platforms unless every Teensy is flashed with the same value.</li>
 670            <li>The diagram drops valve openings shorter than 7 ms (the valve's opening time), so a higher frequency loses
 671              the small thrusts, and holds each duty cycle for one PWM period.</li>
 672            <li>The program on the platforms keeps the value it was built with: after a change, build again.</li>
 673          </ul>
 674          <p class="hint">Keep 5 Hz unless the firmware was changed too. The lock comes back when the page is reloaded.</p>
 675        </div>
 676        <div class="dialog-foot">
 677          <div>
 678            <button value="cancel" class="btn">Keep It Locked</button>
 679            <button type="button" id="pwmUnlockGo" class="btn btn-stop">Unlock</button>
 680          </div>
 681        </div>
 682      </form>
 683    </dialog>
 684  
 685    <dialog id="calSyncDialog" class="dialog reset-dialog">
 686      <form method="dialog" class="dialog-inner">
 687        <div class="dialog-head">
 688          <h2>Newer Calibration on the Lab Console</h2>
 689          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 690        </div>
 691        <div class="reset-body">
 692          <p>The lab console has a newer calibration than this project:</p>
 693          <ul id="calSyncList" class="cal-sync-list"></ul>
 694          <p class="hint">Updating copies it into resources/spot_calibration.json; the next simulation uses it (the table's
 695            slope, the thrusters' thrust). Not Now keeps this project's, and doesn't ask again until the lab's changes.</p>
 696          <label class="reset-opt"><input type="checkbox" id="calSyncNever"><span><b>Never Ask Again</b><span class="hint">For this project.
 697            The Calibration tab can turn asking back on, and update at any time.</span></span></label>
 698        </div>
 699        <div class="dialog-foot">
 700          <div>
 701            <button type="button" id="calSyncLater" class="btn">Not Now</button>
 702            <button type="button" id="calSyncUpdate" class="btn btn-primary">Update</button>
 703          </div>
 704        </div>
 705      </form>
 706    </dialog>
 707  
 708    <dialog id="calThrustDialog" class="dialog reset-dialog">
 709      <form method="dialog" class="dialog-inner">
 710        <div class="dialog-head">
 711          <h2>Before the Thruster Calibration</h2>
 712          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 713        </div>
 714        <div class="reset-body">
 715          <p>The thrust is worked out from how each burn moves the platform, so it is only as good as what the
 716            console takes the <b id="calThrustName">platform</b>'s mass, inertia and centre of gravity to be:</p>
 717          <div class="cal-assume">
 718            <label><span class="field-label">Mass [kg]</span><input class="input" type="number" step="0.01" data-assume="mass"></label>
 719            <label><span class="field-label">Inertia [kg m²]</span><input class="input" type="number" step="0.0001" data-assume="inertia"></label>
 720            <label title="The centre of gravity from the body's centre, along +x (the front)"><span class="field-label">CG x [mm]</span><input class="input" type="number" step="0.1" data-assume="cgx"></label>
 721            <label title="The centre of gravity from the body's centre, along +y"><span class="field-label">CG y [mm]</span><input class="input" type="number" step="0.1" data-assume="cgy"></label>
 722          </div>
 723          <ul class="lock-reasons">
 724            <li>These are SPOT's default mass properties (from the default scale readings and bifilar test), as the lab
 725              console's controller uses them. Change them here if the platform carries something else today.</li>
 726            <li>A mass 5 % off gives thrusts 5 % off. The torque each burn gives checks the centre of gravity and the
 727              inertia: a <b>torque ratio</b> far from 1 says they are off.</li>
 728            <li id="calThrustTable"></li>
 729            <li>Each thruster fires at full thrust for the burn time with the platform floating; the console flies it
 730              back to where it started between burns. Clear the table around it.</li>
 731          </ul>
 732          <label class="reset-opt"><input type="checkbox" id="calThrustAck"><span><b>I understand</b><span class="hint">The
 733            thrust comes from these values, and the platform will float and fire each thruster in turn.</span></span></label>
 734        </div>
 735        <div class="dialog-foot">
 736          <div>
 737            <button value="cancel" class="btn">Cancel</button>
 738            <button type="button" id="calThrustGo" class="btn btn-go" disabled>Start</button>
 739          </div>
 740        </div>
 741      </form>
 742    </dialog>
 743  
 744    <dialog id="calArmDialog" class="dialog reset-dialog">
 745      <form method="dialog" class="dialog-inner">
 746        <div class="dialog-head">
 747          <h2>Before the Arm Calibration</h2>
 748          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 749        </div>
 750        <div class="reset-body">
 751          <p>The calibration moves the Chaser's arm, one joint at a time, with the real servos:</p>
 752          <ul class="lock-reasons">
 753            <li>The Chaser must <b>rest on the table</b> (not float): the arm's motion would push it. The console keeps
 754              it resting while the calibration runs.</li>
 755            <li>Each tested joint moves up to <b>25° either way</b> from where it is now. Clear the space around the arm,
 756              and start with the joints away from their limits.</li>
 757            <li>Keep the arm's power switch at hand. <b>Stop</b> (or Esc) on this tab switches the servos' torque off at once;
 758              so does switching the Chaser off on the Hardware tab.</li>
 759            <li id="calArmTime"></li>
 760          </ul>
 761          <label class="reset-opt"><input type="checkbox" id="calArmAck"><span><b>The Chaser rests, and the arm is clear to move</b><span class="hint">Someone is
 762            by the arm with the power switch at hand.</span></span></label>
 763        </div>
 764        <div class="dialog-foot">
 765          <div>
 766            <button value="cancel" class="btn">Cancel</button>
 767            <button type="button" id="calArmGo" class="btn btn-go" disabled>Start</button>
 768          </div>
 769        </div>
 770      </form>
 771    </dialog>
 772  
 773    <dialog id="resultsDialog" class="dialog results-dialog">
 774      <form method="dialog" class="dialog-inner">
 775        <div class="dialog-head">
 776          <h2>Saved Data</h2>
 777          <button value="cancel" class="btn btn-ghost btn-sm" aria-label="Close">✕</button>
 778        </div>
 779        <div id="resultsList" class="results-list"></div>
 780        <div class="dialog-foot">
 781          <span class="hint">Newest first. Experiments are collected on the Experiment tab; simulations with Save Results.</span>
 782          <div>
 783            <button type="button" id="resultsRefresh" class="btn btn-ghost">↻</button>
 784            <button type="button" id="resultsOpenFile" class="btn">Other .mat File…</button>
 785          </div>
 786        </div>
 787      </form>
 788    </dialog>
 789  
 790    <div id="toasts" class="toasts" aria-live="polite"></div>
 791  </body>
 792  </html>
```

</details>
<!-- /listing -->
