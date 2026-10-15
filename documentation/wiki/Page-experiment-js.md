# experiment.js

**File:** `+spot/+console/webapp/static/experiment.js` · **Tab:** [[Experiment Tab|Experiment-Tab]] · **Server side:** [[local.py|Console-local]] (the lab link), [[experiment.py|Console-experiment]] (on the lab console)

<!-- source: +spot/+console/webapp/static/experiment.js -->

## What it is for

The Experiment tab of the local console: build the selected diagram on the lab console from this MATLAB, have the lab console deploy it to the platforms, start and stop it, watch it live, and collect the data. Everything on the hardware is done by the lab console; this page reaches it only through the local server (`POST /api/lab`), which refuses when the lab network isn't reachable.

## Function by function

**Lines 18–29.** `initExperiment` receives the shared helpers (API, toasts, storage, the 3D viewer, the messages and live-plot panels, the Simulation tab's state, and callbacks to switch a platform off or change a setting). `selected()` is the platforms ticked in the Scenario; `model()` the diagram's name without `.slx`.

**Lines 33–166, `render`.** Everything on the sidebar from the state:
- *Lab* (40–57): the notice when the lab console is unreachable or too old; the pill (Connected / Needs Update / Not Reachable); the address field shows the settings' `labConsoleAddress` (not while you are typing in it), and `followAddress` re-points the local server when the settings' address differs from the link's.
- *PhaseSpace* (59–74): streaming state and packet rate; the Start/Stop Cameras button; the Data Rate field (the settings' `dataRate`, disabled while streaming or locked); the hint.
- *Platforms* (76–101): a card per selected platform with its hardware state (Ready / No Teensy / In Use / Offline / No Access), its experiment clock, its program's state, pid and data-file count, missing libraries, failures and the log's tail; a trash button to drop it from the scenario. The messages and live-plot panels are fed the experiment status.
- *Build* (103–125): the pill and text (building with elapsed time and whether the program has already appeared on the TX2; built with the log; failed with a *Details* button); the live build output; Check / Connect / Build enabled only when it makes sense.
- *Deploy & Run* (127–148): the program found on the TX2 (name, time, size); the phase pill; Deploy / Start / Stop enabled by phase; the status line from `syncText`.
- *Data* (150–156): Collect enabled when stopped; the last collection's counts.
- (158–165): re-check the build on the TX2 when the diagram's name becomes known or changes; the platforms' clocks and their spread over the 3D view; the empty-view message.

**Lines 170–187.** `syncText`: the agreed start as a sentence (waiting for which platforms, counting down, running since, or failed); `fmtMs`.

**Lines 189–195.** `appendBuildLog`: append live build lines, trimming at 60 000 characters, keeping the scroll at the bottom.

**Lines 198–217.** `dataRate` (the settings' data rate, 1–960) and `labAddress` / `validAddress` / `followAddress` (described above; once per address).

**Lines 219–224.** `updateEmpty`: the overlay on the 3D view when no live frame arrived in 2 s.

**Lines 228–237.** `refreshBuildOnLab`: `GET experiment/build?model=` through the lab link.

**Lines 241–251.** *Start Cameras*: stop if streaming; else send this project's marker calibration (`tracker_positions`) and start the stream at the data rate.

**Lines 253–255.** The data-rate lock buttons ([[ratelock.js|Page-small-modules]]).

**Lines 257–279.** The address field: validate, store as the setting, re-point the link, toast a reminder to build again (the address is compiled into the programs). The Data Rate field: validate 1–960 and store as the setting.

**Lines 281–292.** *Check*: `experiment.plan` in MATLAB, shown with the segments that will run, the program path, the changes and any warnings.

**Lines 294–307.** *Connect*: `experiment.connect` with the password typed (cleared afterwards).

**Lines 310–321.** The trash buttons; the failed build's *Details* opens the error in the command window.

**Lines 323–361.** *Build* (after a confirmation; clears the live log), *Deploy*, *Start* (confirmations, especially with the cameras off; clears the live paths), *Stop*, *Collect* (fetch on the lab console → download into `Saved Data/` → `experiment.decode` in MATLAB).

**Lines 365–381.** The view's layer switches (live paths, simulation overlay, stored in the browser); the overlay replays the simulation whole until the experiment's clock runs, then in step with it.

**Lines 384–386.** Timers: re-render every second while shown; poll the TX2 for the compiled program every 5 s while building.

**Lines 388–454.** The event handlers the app calls: the initial state, lab link changes (toast on connect/disconnect), live frames (to the viewer), stream/monitor/hardware/experiment updates (the simulation overlay follows the experiment clock), build log lines, build finished (toast; the error into the command window), MATLAB status, show/hide.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/experiment.js</b> (455 lines; click to show)</summary>

```javascript
   1  // Experiment tab (local console): build the selected diagram on the lab console (TX2) from this
   2  // MATLAB, have the lab console deploy it to the platforms, start and stop it, watch it live, and
   3  // collect the data. The lab console does everything on the hardware; this page only reaches it
   4  // through the local server (POST /api/lab), which refuses when the lab network isn't reachable.
   5  
   6  import { PLATFORM_KEYS as PLATFORMS, platformName, swatch } from './platforms.js';
   7  import { handleLockClick, isUnlocked, onLockChange, paintLocks } from './ratelock.js';
   8  
   9  const $ = (sel, el = document) => el.querySelector(sel);
  10  const TRASH = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" d="M4 7h16M10 11v6M14 11v6M5 7l1 12a2 2 0 0 0 2 2h8a2 2 0 0 0 2-2l1-12M9 7V4h6v3"/></svg>';
  11  const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  12  const fmtTime = t => new Date(t * 1000).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  13  // Replace an element's HTML only when it changed (keeps open <details> and scroll positions)
  14  function setHTML(el, html) {
  15    if (el.dataset.html !== html) { el.dataset.html = html; el.innerHTML = html; }
  16  }
  17  
  18  export function initExperiment({ api, act, toast, storage, viewer, messages, livePlot, getSim, setPlatform, setSetting, printError }) {
  19    const S = { lab: { reachable: false }, stream: {}, monitor: {}, hardware: {}, experiment: {}, build: {},
  20                matlab: { connected: false }, plan: null, connected: null, lastFrame: 0, shown: false,
  21                buildOnLab: null };
  22    const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
  23    const lab = (path, body = {}) => api('/api/lab', { path, body }).then(r => r.result);
  24  
  25    function selected() {
  26      const s = getSim().settings;
  27      return s ? PLATFORMS.filter(n => s.platforms[n]) : [];
  28    }
  29    const model = () => ((getSim().settings || {}).diagram || '').replace(/\.slx$/, '');
  30  
  31    // ---------------------------------------------------------------- rendering
  32  
  33    function render() {
  34      const reachable = Boolean(S.lab.reachable);
  35      const canExp = reachable && Boolean(S.lab.experiments);   // older lab consoles: cameras and live view only
  36      const exp = S.experiment || {};
  37      const running = ['starting', 'running', 'stopping'].includes(exp.phase);
  38      const busy = Boolean(S.matlab.busy) || Boolean(S.build.running);
  39  
  40      // Lab
  41      const notice = $('#expNotice');
  42      notice.hidden = canExp;
  43      notice.className = `hw-detail ${reachable ? 'warn' : 'err'}`;
  44      notice.textContent = !reachable
  45        ? `The lab console isn't reachable${S.lab.error ? ` (${S.lab.error.replace(/^Not reachable \(|\)$/g, '')})` : ''}. ` +
  46          'Experiments can only be built, deployed and run from the lab network (SRCL Wi-Fi or wired); everything below is locked until then.'
  47        : 'The lab console (TX2) is reachable, but runs an older version without experiments. Update it with ' +
  48          '+spot/+console/deploy/deploy_tx2.sh; until then only the cameras and the live view work here.';
  49      const lp = $('#expLabPill');
  50      lp.className = `pill ${canExp ? 'ok' : reachable ? 'warn' : 'err'}`;
  51      lp.textContent = canExp ? 'Connected' : reachable ? 'Needs Update' : 'Not Reachable';
  52      // The lab console's address: the settings' labConsoleAddress (the local server follows it)
  53      const address = labAddress(), addr = $('#expLabAddress');
  54      addr.disabled = !getSim().settings;
  55      if (document.activeElement !== addr && address && !addr.classList.contains('invalid')) addr.value = address;
  56      $('#expLabUrl').textContent = S.lab.url ? `Lab console at ${S.lab.url}` : '';
  57      followAddress(address);
  58  
  59      // PhaseSpace
  60      const st = S.stream || {}, mon = S.monitor || {};
  61      const streaming = Boolean(st.running);
  62      $('#expStream').textContent = !reachable ? 'Lab Console Not Reachable' : streaming
  63        ? `Streaming at ${st.frequency} Hz · ${mon.rate || 0} packets/s` : 'Stopped';
  64      $('#expStream').className = `exp-state ${streaming ? 'on' : ''}`;
  65      const cam = $('#expCameras');
  66      cam.textContent = streaming ? 'Stop Cameras' : 'Start Cameras';
  67      cam.className = `btn btn-sm ${streaming ? 'btn-stop' : ''}`;
  68      cam.disabled = !reachable || (streaming && running);
  69      // Frequency: the settings' dataRate, as the lab console's Frequency (fixed while streaming)
  70      const rate = dataRate(), freq = $('#expFrequency');
  71      freq.disabled = streaming || !getSim().settings || !isUnlocked();
  72      if (document.activeElement !== freq && rate && !freq.classList.contains('invalid')) freq.value = rate;
  73      $('#expStreamHint').textContent = streaming && st.started_by ? `Started ${fmtTime(st.started)} by ${st.started_by}`
  74        : rate ? `Starts at ${rate} Hz after sending this project's marker calibration.` : 'Waiting for the settings from MATLAB.';
  75  
  76      // Platforms
  77      $('#expDiagram').textContent = model() ? `${model()}.slx` : '';
  78      const names = selected();
  79      const hw = {};
  80      for (const p of (S.hardware.platforms || [])) hw[p.name] = p;
  81      const clocks = exp.clocks || {};
  82      const sync = exp.sync || {};
  83      messages?.experiment(exp);   // the platforms' messages, as the lab console received them
  84      livePlot?.experiment(exp);   // the live streams' list (their samples come as lab_live_data)
  85      setHTML($('#expPlatforms'), names.length ? names.map(n => {
  86        const h = hw[n], pr = h && h.probe, ep = (exp.platforms || {})[n];
  87        const [hwText, hwKind] = !reachable ? ['', ''] : !pr ? ['Checking…', ''] : pr.state !== 'ok' ? [pr.state === 'offline' ? 'Offline' : 'No Access', 'err']
  88          : !pr.teensy ? ['No Teensy', 'warn'] : pr.others.length ? [`In Use: ${pr.others.join(', ')}`, 'warn'] : ['Ready', 'ok'];
  89        const c = clocks[n];
  90        const clock = c && c.age < 3 ? `${c.t.toFixed(1)} s` : '';
  91        const expText = ep ? ep.state + (ep.pid ? ` · pid ${ep.pid}` : '') + (ep.data != null ? ` · ${ep.data} data file${ep.data === 1 ? '' : 's'}` : '') : '';
  92        const missing = ep && ep.missing && ep.missing.length ? `<div class="hint bad">Missing libraries: ${esc(ep.missing.join(', '))}</div>` : '';
  93        const detail = ep && ep.detail && ep.state === 'failed' ? `<div class="hint bad">${esc(ep.detail)}</div>` : '';
  94        const log = ep && ep.log && ep.log.length && ep.state !== 'running' ? `<pre class="exp-log">${esc(ep.log.join('\n'))}</pre>` : '';
  95        return `<div class="exp-plat">
  96          <div class="exp-row"><span class="exp-name">${swatch(n)}${platformName(n)}</span>
  97            <span class="exp-clock mono">${clock}</span>${hwText ? `<span class="pill ${hwKind}" title="${esc(hwText)}">${esc(hwText)}</span>` : ''}
  98            <button type="button" class="btn btn-ghost btn-sm exp-remove" data-remove="${n}" ${running ? 'disabled' : ''}
  99              title="${running ? 'Stop the experiment to change its platforms' : `Remove the ${platformName(n)} from the experiment${n === 'RED' ? ' (and the arm)' : ''}: it is switched off in Scenario's platforms`}" aria-label="Remove the ${platformName(n)}">${TRASH}</button></div>
 100          ${expText ? `<div class="hint">${esc(expText)}</div>` : ''}${missing}${detail}${log}</div>`;
 101      }).join('') : '<p class="hint">No platform selected: choose them on the Simulation tab (Scenario).</p>');
 102  
 103      // Build
 104      const b = S.build || {};
 105      const bp = $('#expBuildPill');
 106      bp.className = `pill ${b.running ? 'warn' : b.ok ? 'ok' : b.ok === false ? 'err' : ''}`;
 107      bp.textContent = b.running ? `Building · ${Math.round(Date.now() / 1000 - b.started)} s` : b.ok ? 'Built' : b.ok === false ? 'Failed' : '';
 108      bp.hidden = !bp.textContent;   // nothing built yet: no pill
 109      const canMatlab = S.matlab.connected && !busy;
 110      $('#expPlan').disabled = !S.matlab.connected || busy || !model();
 111      $('#expConnect').disabled = !canExp || !canMatlab;
 112      $('#expPassword').disabled = !canExp;
 113      $('#expBuild').disabled = !canExp || !canMatlab || !names.length || running;
 114      const bo = $('#expBuildOut');
 115      // While building: what slbuild prints, live, and whether the program has appeared on the TX2
 116      const compiled = b.running && S.buildOnLab && S.buildOnLab.built > b.started
 117        ? `<div class="ok">✓ The program was compiled on the TX2 at ${fmtTime(S.buildOnLab.built)}; MATLAB is finishing.</div>` : '';
 118      setHTML(bo, b.running ? `<span class="spinner dark"></span> Building in MATLAB (output below)${compiled}`
 119        : b.ok && b.summary ? `✓ Built in ${Math.round(b.summary.seconds)} s` +
 120          (b.summary.log ? ` <details><summary>Build Log</summary><pre class="exp-log">${esc(b.summary.log)}</pre></details>` : '')
 121        : b.ok === false ? `<span class="bad">✕ ${esc(b.error)}</span>` +
 122          (b.error_details ? ' <button type="button" class="btn btn-sm" data-err="details" title="Show the causes, the blocks and MATLAB\'s full report in the command window">Details</button>' : '') : '');
 123      const live = $('#expBuildLive');
 124      live.hidden = !b.running;
 125      if (b.running && !live.textContent && (b.log_tail || []).length) appendBuildLog(b.log_tail);
 126  
 127      // Deploy and run
 128      const info = S.buildOnLab;
 129      $('#expProgram').textContent = info ? `${model()}.elf on the TX2, built ${fmtTime(info.built)} (${Math.round(info.size / 1024)} kB)`
 130        : canExp ? `No build of ${model() || 'the diagram'} on the TX2 yet.` : '';
 131      $('#expProgram').title = info ? info.path : '';
 132      const pp = $('#expPhasePill');
 133      pp.className = `pill ${exp.phase === 'running' ? 'live' : ['deploying', 'starting', 'stopping'].includes(exp.phase) ? 'warn' : ''}`;
 134      pp.textContent = exp.phase ? exp.phase[0].toUpperCase() + exp.phase.slice(1) : '';
 135      pp.hidden = !pp.textContent;
 136      $('#expDeploy').disabled = !canExp || running || exp.phase === 'deploying' || !info || !names.length;
 137      const deployedHere = exp.model === model() && ['deployed', 'stopped'].includes(exp.phase);
 138      $('#expStart').disabled = !canExp || !deployedHere || !names.length;
 139      $('#expStop').disabled = !canExp || !['starting', 'running', 'stopping'].includes(exp.phase);
 140      const ro = $('#expRunOut');
 141      const who = exp.by ? ` by ${exp.by}` : '';
 142      ro.className = `hint ${(exp.note && /E-stop|failed/i.test(exp.note)) || (exp.phase === 'running' && sync.state === 'failed') ? 'bad' : ''}`;
 143      ro.textContent = exp.phase === 'running' ? `${syncText(sync, exp)}${who}. Stop sends all-off to the Teensy; keep the E-stop at hand.`
 144        : exp.phase === 'deployed' ? `${exp.model} is deployed${who}: start it when the cameras run.`
 145        : exp.note || (exp.phase === 'stopped' && exp.stopped ? `Stopped ${fmtTime(exp.stopped)}.` : '');
 146      if (exp.model && exp.model !== model() && exp.phase && exp.phase !== 'idle') {
 147        ro.textContent = `The lab's current experiment is ${exp.model} (${exp.phase}${who}), not ${model()}.`;
 148      }
 149  
 150      // Data
 151      $('#expData').disabled = !canExp || running || !exp.model || !S.matlab.connected;
 152      const lr = exp.last_run;
 153      if (lr && !$('#expDataOut').dataset.local) {
 154        const counts = Object.entries(lr.files || {}).map(([n, r]) => `${platformName(n)}: ${r.files.length}${r.error ? ` (${r.error})` : ''}`).join(' · ');
 155        $('#expDataOut').textContent = `Last collected: ${lr.run} — ${counts}`;
 156      }
 157  
 158      // The diagram became known (settings arrive from MATLAB after the page) or changed: look again
 159      if (canExp && model() && model() !== S.buildFor) refreshBuildOnLab();
 160  
 161      // Clocks over the view
 162      setHTML($('#expClocks'), Object.entries(clocks).filter(([, c]) => c.age < 3)
 163        .map(([n, c]) => `<span>${swatch(n)}${platformName(n)} ${c.t.toFixed(1)} s</span>`).join('') +
 164        (sync.spread_ms != null ? `<span title="How far apart the platforms' experiment clocks are (network delays included)">within ${fmtMs(sync.spread_ms)}</span>` : ''));
 165      updateEmpty();
 166    }
 167  
 168    // The agreed start (lab console): every program running, then a countdown in the PhaseSpace
 169    // stream that all platforms follow (spot.hardware.experimentTime)
 170    function syncText(sync, exp) {
 171      const since = `Running ${exp.model} since ${fmtTime(exp.started)}`;
 172      if (sync.state === 'waiting') {
 173        const w = sync.waiting_for || [];
 174        return `${since}: waiting for ${w.length ? w.map(platformName).join(', ') : 'the platforms'} to start ${w.length === 1 ? 'its program' : 'their programs'}; ` +
 175          'the experiment starts together a few seconds after the last one';
 176      }
 177      if (sync.state === 'counting') {
 178        const left = Math.max(0, Math.ceil(sync.start_at - Date.now() / 1000));
 179        return `${since}: every platform is ready, starting together in ${left} s`;
 180      }
 181      if (sync.state === 'running') {
 182        return `${since}; started together at ${fmtTime(sync.since)}` + (sync.spread_ms != null ? `, clocks within ${fmtMs(sync.spread_ms)}` : '');
 183      }
 184      if (sync.state === 'failed') return `${since}, but not started: ${sync.detail || 'no experiment clock'}`;
 185      return since;
 186    }
 187    const fmtMs = ms => (ms < 10 ? `${ms.toFixed(1)} ms` : `${Math.round(ms)} ms`);
 188  
 189    function appendBuildLog(lines) {
 190      const live = $('#expBuildLive');
 191      const atBottom = live.scrollHeight - live.scrollTop - live.clientHeight < 30;
 192      live.textContent += lines.join('\n') + '\n';
 193      if (live.textContent.length > 60000) live.textContent = live.textContent.slice(-50000);
 194      if (atBottom) live.scrollTop = live.scrollHeight;
 195    }
 196  
 197    // The PhaseSpace data rate [Hz]: the settings' dataRate (whole Hz, 1 to 960)
 198    const validRate = v => Number.isInteger(v) && v >= 1 && v <= 960;
 199    function dataRate() {
 200      const s = getSim().settings, d = getSim().derived;
 201      return s && validRate(Number(s.dataRate)) ? Number(s.dataRate) : d && d.cameraRate ? d.cameraRate : null;
 202    }
 203  
 204    // The lab console's address [IPv4 text]: the settings' labConsoleAddress
 205    const validAddress = a => /^\d{1,3}(\.\d{1,3}){3}$/.test(a) && a.split('.').every(p => Number(p) <= 255);
 206    function labAddress() {
 207      const s = getSim().settings;
 208      return s && typeof s.labConsoleAddress === 'string' ? s.labConsoleAddress.trim() : '';
 209    }
 210    // Point the local server's link at the settings' address when they differ (e.g. settings loaded
 211    // from another file); once per address
 212    function followAddress(address) {
 213      if (!address || !validAddress(address) || !S.lab.url || S.lab.url.split('//')[1]?.split(':')[0] === address
 214          || S.addressSent === address) return;
 215      S.addressSent = address;
 216      api('/api/lab/address', { address }).catch(e => toast(`Lab console address: ${e.message}`, 'error', 6000));
 217    }
 218  
 219    function updateEmpty() {
 220      const live = performance.now() - S.lastFrame < 2000;
 221      $('#expEmpty').classList.toggle('hidden', live);
 222      $('#expEmptyDetail').textContent = !S.lab.reachable ? 'The lab console isn\'t reachable from here.'
 223        : S.stream.running ? 'Cameras are on; waiting for the platforms to be seen…' : 'Start the cameras to see the platforms here.';
 224    }
 225  
 226    // ---------------------------------------------------------------- the build on the TX2
 227  
 228    async function refreshBuildOnLab() {
 229      S.buildFor = model();
 230      if (!S.lab.reachable || !S.lab.experiments || !model()) { S.buildOnLab = null; return render(); }
 231      try {
 232        S.buildOnLab = (await lab(`experiment/build?model=${encodeURIComponent(model())}`)).build || null;
 233      } catch {
 234        S.buildOnLab = null;
 235      }
 236      render();
 237    }
 238  
 239    // ---------------------------------------------------------------- actions
 240  
 241    $('#expCameras').onclick = () => act(async () => {
 242      if (S.stream.running) {
 243        await lab('stream/stop');
 244        return;
 245      }
 246      const rate = dataRate();
 247      if (!rate) throw new Error('No data rate yet: the settings haven\'t come from MATLAB.');
 248      await lab('tracker_positions');
 249      await lab(`stream/start?frequency=${rate}`);
 250      toast(`Cameras starting at ${rate} Hz`, 'success', 3000);
 251    });
 252  
 253    paintLocks();
 254    onLockChange(() => render());
 255    for (const b of document.querySelectorAll('#pageExperiment [data-ratelock]')) b.onclick = handleLockClick;
 256  
 257    $('#expLabAddress').addEventListener('change', ev => {
 258      const el = ev.target, v = el.value.trim();
 259      const ok = validAddress(v);
 260      el.classList.toggle('invalid', !ok);
 261      if (!ok) return toast('The lab console address must be an IPv4 address such as 192.168.1.116.', 'error', 6000);
 262      if (v === labAddress()) return;
 263      if (!setSetting('labConsoleAddress', v)) return toast('The settings haven\'t come from MATLAB yet.', 'error', 5000);
 264      S.addressSent = v;
 265      act(async () => {
 266        await api('/api/lab/address', { address: v });
 267        toast(`Lab console address set to ${v}. Build again: the platforms send their clocks, messages and live streams to it.`, 'success', 8000);
 268      });
 269      render();
 270    });
 271  
 272    $('#expFrequency').addEventListener('change', ev => {
 273      const el = ev.target, v = Number(el.value);
 274      const ok = el.value !== '' && validRate(v);
 275      el.classList.toggle('invalid', !ok);
 276      if (!ok) return toast('Frequency must be a whole number of Hz, 1 to 960.', 'error', 5000);
 277      if (!setSetting('dataRate', v)) toast('The settings haven\'t come from MATLAB yet.', 'error', 5000);
 278      render();
 279    });
 280  
 281    $('#expPlan').onclick = () => act(async () => {
 282      const p = await matlab('experiment.plan');
 283      S.plan = p;
 284      const warn = (p.warnings || []).map(w => `<div class="warn-text">⚠ ${esc(w)}</div>`).join('');
 285      const plats = Array.isArray(p.platforms) ? p.platforms : [p.platforms].filter(Boolean);
 286      const segs = Array.isArray(p.segments) ? p.segments : [p.segments].filter(Boolean);
 287      const all = segs.length === 9;
 288      $('#expPlanOut').innerHTML = `Builds <b>${esc(p.model)}</b> on ${esc(p.board.user)}@${esc(p.board.address)} for ${esc(plats.map(platformName).join(', ') || 'no platform')}.
 289        <div class="${all ? '' : 'warn-text'}">Runs ${all ? 'every phase' : esc(segs.map(n => n.replace('Phase ', '')).join(', ')) + ' (others skipped)'}: ${esc(String(Math.round(p.tsim * 10) / 10))} s.</div>
 290        <details><summary>Details</summary><div class="mono small">Program: ${esc(p.program)}</div>
 291        <div>For the build only (undone afterwards, diagram not saved): ${esc((p.changes || []).join('; '))}.</div></details>${warn}`;
 292    });
 293  
 294    $('#expConnect').onclick = () => act(async () => {
 295      const pw = $('#expPassword').value;
 296      $('#expConnectOut').innerHTML = '<span class="spinner dark"></span> Connecting (the first time, MATLAB sets up its server on the TX2)…';
 297      try {
 298        const r = await matlab('experiment.connect', pw ? { password: pw } : {});
 299        $('#expConnectOut').textContent = `✓ MATLAB is connected to ${r.user}@${r.address}.`;
 300        $('#expConnectOut').className = 'hint ok';
 301      } catch (e) {
 302        $('#expConnectOut').textContent = `✕ ${e.message}`;
 303        $('#expConnectOut').className = 'hint bad';
 304      } finally {
 305        $('#expPassword').value = '';
 306      }
 307    });
 308  
 309    // A platform's trash button: take it out of the experiment (Scenario's platform switches)
 310    $('#expPlatforms').addEventListener('click', ev => {
 311      const b = ev.target.closest('[data-remove]');
 312      if (b && !b.disabled) setPlatform(b.dataset.remove, false);
 313    });
 314  
 315    // A failed build's Details: its error entry in the command window, opened
 316    $('#expBuildOut').addEventListener('click', ev => {
 317      if (!ev.target.closest('[data-err="details"]') || !S.build.error_details) return;
 318      if (!S.errorEntry || !S.errorEntry.isConnected) S.errorEntry = printError('Build failed', S.build.error_details);
 319      S.errorEntry.open = true;
 320      S.errorEntry.scrollIntoView({ block: 'nearest' });
 321    });
 322  
 323    $('#expBuild').onclick = () => act(async () => {
 324      if (!confirm(`Build ${model()} on the TX2? MATLAB generates the code and compiles it there (a few minutes). The diagram file isn't changed.`)) return;
 325      $('#expBuildLive').textContent = '';
 326      S.build = await api('/api/experiment/build', {});
 327      render();
 328    });
 329  
 330    $('#expDeploy').onclick = () => act(async () => {
 331      await lab('experiment/deploy', { model: model(), platforms: selected() });
 332    });
 333  
 334    $('#expStart').onclick = () => act(async () => {
 335      if (!S.stream.running && !confirm('The cameras are off: the platforms will not know where they are. Start anyway?')) return;
 336      if (!confirm(`Start ${model()} on the ${selected().map(platformName).join(', ')}? Make sure the table is clear and the E-stop is at hand.`)) return;
 337      viewer?.clearLivePaths();
 338      await lab('experiment/start', { platforms: selected() });
 339    });
 340  
 341    $('#expStop').onclick = () => act(async () => {
 342      await lab('experiment/stop');
 343    });
 344  
 345    $('#expData').onclick = () => act(async () => {
 346      const out = $('#expDataOut');
 347      out.dataset.local = '1';
 348      out.textContent = 'Collecting the data files from the platforms…';
 349      const st = await lab('experiment/fetch');
 350      const run = st.last_run && st.last_run.run;
 351      if (!run) throw new Error('Nothing was collected.');
 352      const got = await api('/api/lab/download', { run });
 353      if (!got.files.length) {
 354        out.textContent = `No data files came back (${run}).`;
 355        return;
 356      }
 357      out.textContent = `Saved ${got.files.length} file(s) in ${got.folder}; converting with MATLAB…`;
 358      const dec = await matlab('experiment.decode', { folder: got.folder });
 359      const files = Array.isArray(dec.files) ? dec.files : [dec.files].filter(Boolean);
 360      out.textContent = `✓ ${got.folder}: ${files.length} .mat file(s) (dataClass_rt).`;
 361    });
 362  
 363    // ---------------------------------------------------------------- the view
 364  
 365    const livePaths = $('#expLivePaths'), simOverlay = $('#expSimOverlay');
 366    livePaths.checked = storage.get('spot.exp.livePaths', true);
 367    simOverlay.checked = storage.get('spot.exp.simOverlay', false);
 368    function layers() {
 369      if (!S.shown) return;
 370      const res = getSim().result;
 371      // Poses: as chosen in the view's toolbar (shared with the Simulation tab)
 372      viewer?.setLayers({ live: true, sim: simOverlay.checked && Boolean(res), setup: getSim().pose !== 'off' });
 373      viewer?.setPrimaryShown(true);
 374      viewer?.setLivePaths(livePaths.checked);
 375      if (res && !S.clockSync) viewer?.setSimTime(res.t[res.t.length - 1]);   // whole paths until the experiment's clock runs
 376    }
 377    document.addEventListener('spot-pose', layers);
 378    for (const [el, key] of [[livePaths, 'livePaths'], [simOverlay, 'simOverlay']]) {
 379      el.onchange = () => { storage.set(`spot.exp.${key}`, el.checked); layers(); };
 380    }
 381    $('#expClearPaths').onclick = () => viewer?.clearLivePaths();
 382  
 383    // While shown: follow settings changed on the Simulation tab, and the build timer
 384    setInterval(() => { if (S.shown) { updateEmpty(); render(); } }, 1000);
 385    // While building: look for the compiled program on the TX2 every 5 s
 386    setInterval(() => { if (S.build.running && S.lab.experiments) refreshBuildOnLab(); }, 5000);
 387  
 388    return {
 389      onState(st) {
 390        S.lab = st.lab || S.lab;
 391        const snap = S.lab.snapshot || {};
 392        S.stream = snap.stream || {};
 393        S.monitor = snap.monitor || {};
 394        S.hardware = snap.hardware || {};
 395        S.experiment = snap.experiment || {};
 396        S.build = st.build || {};
 397        S.matlab = st.matlab || S.matlab;
 398        if (snap.led_owner) viewer?.setLedOwner(snap.led_owner);
 399        render();
 400        refreshBuildOnLab();
 401      },
 402      onLab(l) {
 403        const was = S.lab.reachable;
 404        S.lab = l;
 405        const snap = l.snapshot || {};
 406        if (l.reachable) {
 407          S.stream = snap.stream || S.stream;
 408          S.monitor = snap.monitor || S.monitor;
 409          S.hardware = snap.hardware || S.hardware;
 410          S.experiment = snap.experiment || S.experiment;
 411        }
 412        render();
 413        if (l.reachable !== was) {
 414          toast(l.reachable ? 'Connected to the lab console' : 'Lost the lab console', l.reachable ? 'success' : 'error', 4000);
 415          refreshBuildOnLab();
 416        }
 417      },
 418      onFrame(frame) {
 419        S.lastFrame = performance.now();
 420        if (S.shown) viewer?.update(frame);
 421      },
 422      onStream(st) { S.stream = st; render(); },
 423      onMonitor(m) { S.monitor = m; render(); },
 424      onHardware(p) {
 425        const list = (S.hardware.platforms || []).filter(x => x.name !== p.name);
 426        S.hardware = { ...S.hardware, platforms: [...list, p] };
 427        render();
 428      },
 429      onExperiment(e) {
 430        const before = S.experiment.phase;
 431        S.experiment = e;
 432        // Overlay the simulation in step with the experiment, by the platforms' own clock
 433        const res = getSim().result;
 434        const c = Object.values(e.clocks || {}).find(x => x.age < 3);
 435        S.clockSync = Boolean(c && res);
 436        if (S.clockSync && S.shown && simOverlay.checked) viewer?.setSimTime(c.t);
 437        if (e.phase !== before && e.phase === 'deployed') refreshBuildOnLab();
 438        render();
 439      },
 440      onBuildLog(e) { appendBuildLog(e.lines || []); },
 441      onBuild(b) {
 442        const finished = S.build.running && !b.running;
 443        S.build = b;
 444        render();
 445        if (finished) {
 446          toast(b.ok ? 'Build finished' : `Build failed: ${b.error}`, b.ok ? 'success' : 'error', b.ok ? 4000 : 12000);
 447          if (!b.ok && b.error_details) S.errorEntry = printError('Build failed', b.error_details);
 448          refreshBuildOnLab();
 449        }
 450      },
 451      onMatlab(m) { S.matlab = m; render(); },
 452      show() { S.shown = true; layers(); render(); refreshBuildOnLab(); },
 453      hide() { S.shown = false; },
 454    };
 455  }
```

</details>
<!-- /listing -->
