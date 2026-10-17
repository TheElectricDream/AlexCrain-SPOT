// Experiment tab (local console): build the selected diagram on the lab console (TX2) from this
// MATLAB, have the lab console deploy it to the platforms, start and stop it, watch it live, and
// collect the data. The lab console does everything on the hardware; this page only reaches it
// through the local server (POST /api/lab), which refuses when the lab network isn't reachable.

import { PLATFORM_KEYS as PLATFORMS, platformName, swatch } from './platforms.js';
import { handleLockClick, isUnlocked, onLockChange, paintLocks } from './ratelock.js';
import { constant } from './constants.js';

const $ = (sel, el = document) => el.querySelector(sel);
const TRASH = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" ' +
  'stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" ' +
  'd="M4 7h16M10 11v6M14 11v6M5 7l1 12a2 2 0 0 0 2 2h8a2 2 0 0 0 2-2l1-12M9 7V4h6v3"/></svg>';
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const fmtTime = t =>
  new Date(t * 1000).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });

// Replace an element's HTML only when it changed (keeps open <details> and scroll positions).
function setHTML(el, html) {
  if (el.dataset.html !== html) {
    el.dataset.html = html;
    el.innerHTML = html;
  }
}

/**
 * Sets up the Experiment tab.
 * @param {{api: function(string, !Object): !Promise, act: function(function(): !Promise),
 *     toast: function(string, string, number),
 *     storage: {get: function(string, *): *, set: function(string, *)}, viewer: ?Object,
 *     messages: ?Object, livePlot: ?Object, getSim: function(): !Object,
 *     setPlatform: function(string, boolean), setSetting: function(string, *): boolean,
 *     printError: function(string, !Object): !Element}} deps The page's API call, its action
 *     wrapper (reports errors), its toast and settings storage, the 3D view, the Messages and
 *     Live Plot panels, the Simulation tab's state, setters for its settings, and the command
 *     window's error entry.
 * @return {!Object} Handlers for the page's events: onState, onLab, onFrame, onStream,
 *     onMonitor, onHardware, onExperiment, onBuildLog, onBuild, onMatlab, show, hide.
 */
export function initExperiment({
  api, act, toast, storage, viewer, messages, livePlot, getSim, setPlatform, setSetting,
  printError,
}) {
  const S = {
    lab: { reachable: false }, stream: {}, monitor: {}, hardware: {}, experiment: {}, build: {},
    matlab: { connected: false }, plan: null, connected: null, lastFrame: 0, shown: false,
    buildOnLab: null,
  };
  const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
  const lab = (path, body = {}) => api('/api/lab', { path, body }).then(r => r.result);

  // The platforms switched on in the settings; the diagram's name.
  function selected() {
    const s = getSim().settings;
    return s ? PLATFORMS.filter(n => s.platforms[n]) : [];
  }
  const model = () => ((getSim().settings || {}).diagram || '').replace(/\.slx$/, '');

  // Rendering

  // The whole tab, from the lab console's and MATLAB's state.
  function render() {
    const reachable = Boolean(S.lab.reachable);
    // Older lab consoles: cameras and live view only.
    const canExp = reachable && Boolean(S.lab.experiments);
    const exp = S.experiment || {};
    const running = ['starting', 'running', 'stopping'].includes(exp.phase);
    const busy = Boolean(S.matlab.busy) || Boolean(S.build.running);

    // Lab: why it can't be used (if so), and its address.
    const notice = $('#expNotice');
    notice.hidden = canExp;
    notice.className = `hw-detail ${reachable ? 'warn' : 'err'}`;
    notice.textContent = !reachable
      ? 'The lab console isn\'t reachable' +
        `${S.lab.error ? ` (${S.lab.error.replace(/^Not reachable \(|\)$/g, '')})` : ''}. ` +
        'Experiments can only be built, deployed and run from the lab network (SRCL Wi-Fi or ' +
        'wired); everything below is locked until then.'
      : 'The lab console (TX2) is reachable, but runs an older version without experiments. ' +
        'Update it with +spot/+console/deploy/deploy_tx2.sh; until then only the cameras and ' +
        'the live view work here.';
    const lp = $('#expLabPill');
    lp.className = `pill ${canExp ? 'ok' : reachable ? 'warn' : 'err'}`;
    lp.textContent = canExp ? 'Connected' : reachable ? 'Needs Update' : 'Not Reachable';

    // The lab console's address: the settings' labConsoleAddress (the local server follows it).
    const address = labAddress();
    const addr = $('#expLabAddress');
    addr.disabled = !getSim().settings;
    if (document.activeElement !== addr && address && !addr.classList.contains('invalid')) {
      addr.value = address;
    }
    $('#expLabUrl').textContent = S.lab.url ? `Lab console at ${S.lab.url}` : '';
    followAddress(address);

    // PhaseSpace: streaming, and the Cameras button (not stopped during an experiment).
    const st = S.stream || {};
    const mon = S.monitor || {};
    const streaming = Boolean(st.running);
    $('#expStream').textContent = !reachable ? 'Lab Console Not Reachable'
      : streaming ? `Streaming at ${st.frequency} Hz · ${mon.rate || 0} packets/s`
      : 'Stopped';
    $('#expStream').className = `exp-state ${streaming ? 'on' : ''}`;
    const cam = $('#expCameras');
    cam.textContent = streaming ? 'Stop Cameras' : 'Start Cameras';
    cam.className = `btn btn-sm ${streaming ? 'btn-stop' : ''}`;
    cam.disabled = !reachable || (streaming && running);

    // Frequency: the settings' dataRate, as the lab console's Frequency (fixed while streaming).
    const rate = dataRate();
    const freq = $('#expFrequency');
    freq.disabled = streaming || !getSim().settings || !isUnlocked();
    if (document.activeElement !== freq && rate && !freq.classList.contains('invalid')) {
      freq.value = rate;
    }
    $('#expStreamHint').textContent = streaming && st.started_by
      ? `Started ${fmtTime(st.started)} by ${st.started_by}`
      : rate ? `Starts at ${rate} Hz after sending this project's marker calibration.`
      : 'Waiting for the settings from MATLAB.';

    // Platforms: the lab console's view of each (its messages and live streams to their
    // panels; their samples come as lab_live_data).
    $('#expDiagram').textContent = model() ? `${model()}.slx` : '';
    const names = selected();
    const hw = {};
    for (const p of (S.hardware.platforms || [])) {
      hw[p.name] = p;
    }
    const clocks = exp.clocks || {};
    const sync = exp.sync || {};
    messages?.experiment(exp);
    livePlot?.experiment(exp);
    setHTML($('#expPlatforms'), names.length ? names.map(n => {
      const h = hw[n];
      const pr = h && h.probe;
      const ep = (exp.platforms || {})[n];

      // Its hardware, its experiment clock, and its program's state, libraries, error and log.
      const [hwText, hwKind] = !reachable ? ['', '']
        : !pr || pr.state === 'checking' ? ['Checking…', '']
        : pr.state !== 'ok' ? [pr.state === 'offline' ? 'Offline' : 'No Access', 'err']
        : !pr.teensy ? ['No Teensy', 'warn']
        : pr.others.length ? ['In Use', 'warn']
        : ['Ready', 'ok'];
      const hwTip = pr && pr.others && pr.others.length
        ? `The Teensy is open in ${pr.others.join(', ')}`
        : hwText;
      const c = clocks[n];
      const clock = c && c.age < 3 ? `${c.t.toFixed(1)} s` : '';
      const expText = ep
        ? ep.state + (ep.pid ? ` · pid ${ep.pid}` : '') +
          (ep.data != null ? ` · ${ep.data} data file${ep.data === 1 ? '' : 's'}` : '')
        : '';
      const missing = ep && ep.missing && ep.missing.length
        ? `<div class="hint bad">Missing libraries: ${esc(ep.missing.join(', '))}</div>`
        : '';
      const detail = ep && ep.detail && ep.state === 'failed'
        ? `<div class="hint bad">${esc(ep.detail)}</div>`
        : '';
      const log = ep && ep.log && ep.log.length && ep.state !== 'running'
        ? `<pre class="exp-log">${esc(ep.log.join('\n'))}</pre>`
        : '';

      // One line each: the name (and remove), the hardware's state and the experiment's clock,
      // the program's state.
      return `<div class="exp-plat">
        <div class="exp-plat-head"><span class="exp-name">${swatch(n)}${platformName(n)}</span>
          <button type="button" class="btn btn-ghost btn-sm exp-remove" data-remove="${n}" ${running ? 'disabled' : ''}
            title="${running ? 'Stop the experiment to change its platforms' : `Remove the ${platformName(n)} from the experiment${n === 'RED' ? ' (and the arm)' : ''}: it is switched off in Scenario's platforms`}" aria-label="Remove the ${platformName(n)}">${TRASH}</button></div>
        ${hwText || clock ? `<div class="exp-plat-status">${hwText ? `<span class="pill ${hwKind}" title="${esc(hwTip)}">${esc(hwText)}</span>` : ''}${clock
          ? `<span class="exp-clock mono" title="The platform's experiment clock">${clock}</span>` : ''}</div>` : ''}
        ${expText ? `<div class="hint exp-prog">${esc(expText)}</div>` : ''}${missing}${detail}${log}</div>`;
    }).join('') : '<p class="hint">No platform selected (Simulation tab: Scenario).</p>');

    // Build: its state (no pill if nothing was built yet) and the buttons.
    const b = S.build || {};
    const bp = $('#expBuildPill');
    bp.className = `pill ${b.running ? 'warn' : b.ok ? 'ok' : b.ok === false ? 'err' : ''}`;
    bp.textContent = b.running ? `Building · ${Math.round(Date.now() / 1000 - b.started)} s`
      : b.ok ? 'Built'
      : b.ok === false ? 'Failed'
      : '';
    bp.hidden = !bp.textContent;
    const canMatlab = S.matlab.connected && !busy;
    $('#expPlan').disabled = !S.matlab.connected || busy || !model();
    $('#expConnect').disabled = !canExp || !canMatlab;
    $('#expPassword').disabled = !canExp;
    $('#expBuild').disabled = !canExp || !canMatlab || !names.length || running;

    // While building: what slbuild prints, live, and whether the program has appeared on the
    // TX2; then the result (its log, or the error with a Details button).
    const bo = $('#expBuildOut');
    const compiled = b.running && S.buildOnLab && S.buildOnLab.built > b.started
      ? '<div class="ok">✓ The program was compiled on the TX2 at ' +
        `${fmtTime(S.buildOnLab.built)}; MATLAB is finishing.</div>`
      : '';
    setHTML(bo, b.running
      ? `<span class="spinner dark"></span> Building in MATLAB (output below)${compiled}`
      : b.ok && b.summary
        ? `✓ Built in ${Math.round(b.summary.seconds)} s` +
          (b.summary.log
            ? ' <details><summary>Build Log</summary>' +
              `<pre class="exp-log">${esc(b.summary.log)}</pre></details>`
            : '')
        : b.ok === false
          ? `<span class="bad">✕ ${esc(b.error)}</span>` +
            (b.error_details
              ? ' <button type="button" class="btn btn-sm" data-err="details" ' +
                'title="Show the causes, the blocks and MATLAB\'s full report in the command ' +
                'window">Details</button>'
              : '')
          : '');
    const live = $('#expBuildLive');
    live.hidden = !b.running;
    if (b.running && !live.textContent && (b.log_tail || []).length) {
      appendBuildLog(b.log_tail);
    }

    // Deploy and run: the program on the TX2, the experiment's phase and the buttons.
    const info = S.buildOnLab;
    $('#expProgram').textContent = info
      ? `${model()}.elf on the TX2, built ${fmtTime(info.built)} ` +
        `(${Math.round(info.size / 1024)} kB)`
      : canExp ? `No build of ${model() || 'the diagram'} on the TX2 yet.` : '';
    $('#expProgram').title = info ? info.path : '';
    const pp = $('#expPhasePill');
    pp.className = `pill ${exp.phase === 'running' ? 'live'
      : ['deploying', 'starting', 'stopping'].includes(exp.phase) ? 'warn' : ''}`;
    pp.textContent = exp.phase ? exp.phase[0].toUpperCase() + exp.phase.slice(1) : '';
    pp.hidden = !pp.textContent;
    $('#expDeploy').disabled = !canExp || running || exp.phase === 'deploying' || !info ||
      !names.length;
    const deployedHere = exp.model === model() && ['deployed', 'stopped'].includes(exp.phase);
    $('#expStart').disabled = !canExp || !deployedHere || !names.length;
    $('#expStop').disabled = !canExp || !['starting', 'running', 'stopping'].includes(exp.phase);

    // What the experiment is doing (or another diagram's experiment, if the lab runs one).
    const ro = $('#expRunOut');
    const who = exp.by ? ` by ${exp.by}` : '';
    ro.className = `hint ${(exp.note && /E-stop|failed/i.test(exp.note))
      || (exp.phase === 'running' && sync.state === 'failed') ? 'bad' : ''}`;
    ro.textContent = exp.phase === 'running'
      ? `${syncText(sync, exp)}${who}. Stop sends all-off to the Teensy; keep the E-stop at hand.`
      : exp.phase === 'deployed' ? `${exp.model} is deployed${who}: start it when the cameras run.`
      : exp.note ||
        (exp.phase === 'stopped' && exp.stopped ? `Stopped ${fmtTime(exp.stopped)}.` : '');
    if (exp.model && exp.model !== model() && exp.phase && exp.phase !== 'idle') {
      ro.textContent = `The lab's current experiment is ${exp.model} (${exp.phase}${who}), ` +
        `not ${model()}.`;
    }

    // Data: the last run's files per platform.
    $('#expData').disabled = !canExp || running || !exp.model || !S.matlab.connected;
    const lr = exp.last_run;
    if (lr && !$('#expDataOut').dataset.local) {
      const counts = Object.entries(lr.files || {}).map(([n, r]) =>
        `${platformName(n)}: ${r.files.length}${r.error ? ` (${r.error})` : ''}`).join(' · ');
      $('#expDataOut').textContent = `Last collected: ${lr.run} — ${counts}`;
    }

    // The diagram became known (settings arrive from MATLAB after the page) or changed: look
    // again.
    if (canExp && model() && model() !== S.buildFor) {
      refreshBuildOnLab();
    }

    // Clocks over the view.
    setHTML($('#expClocks'), Object.entries(clocks).filter(([, c]) => c.age < 3)
      .map(([n, c]) => `<span>${swatch(n)}${platformName(n)} ${c.t.toFixed(1)} s</span>`).join('') +
      (sync.spread_ms != null
        ? '<span title="How far apart the platforms\' experiment clocks are (network delays ' +
          `included)">within ${fmtMs(sync.spread_ms)}</span>`
        : ''));
    updateEmpty();
  }

  // The agreed start (lab console): every program running, then a countdown in the PhaseSpace
  // stream that all platforms follow (spot.hardware.experimentTime).
  function syncText(sync, exp) {
    const since = `Running ${exp.model} since ${fmtTime(exp.started)}`;
    if (sync.state === 'waiting') {
      const w = sync.waiting_for || [];
      return `${since}: waiting for ` +
        `${w.length ? w.map(platformName).join(', ') : 'the platforms'} ` +
        `to start ${w.length === 1 ? 'its program' : 'their programs'}; ` +
        'the experiment starts together a few seconds after the last one';
    }
    if (sync.state === 'counting') {
      const left = Math.max(0, Math.ceil(sync.start_at - Date.now() / 1000));
      return `${since}: every platform is ready, starting together in ${left} s`;
    }
    if (sync.state === 'running') {
      return `${since}; started together at ${fmtTime(sync.since)}` +
        (sync.spread_ms != null ? `, clocks within ${fmtMs(sync.spread_ms)}` : '');
    }
    if (sync.state === 'failed') {
      return `${since}, but not started: ${sync.detail || 'no experiment clock'}`;
    }
    return since;
  }
  const fmtMs = ms => (ms < 10 ? `${ms.toFixed(1)} ms` : `${Math.round(ms)} ms`);

  // Adds lines to the live build log (keeping its last 50000 characters past 60000); stays at
  // the bottom if it was there.
  function appendBuildLog(lines) {
    const live = $('#expBuildLive');
    const atBottom = live.scrollHeight - live.scrollTop - live.clientHeight < 30;
    live.textContent += lines.join('\n') + '\n';
    if (live.textContent.length > 60000) {
      live.textContent = live.textContent.slice(-50000);
    }
    if (atBottom) {
      live.scrollTop = live.scrollHeight;
    }
  }

  // The PhaseSpace data rate [Hz]: the settings' dataRate (whole Hz, 1 to MAX_DATA_RATE).
  const validRate = v => Number.isInteger(v) && v >= 1 && v <= constant('MAX_DATA_RATE');
  function dataRate() {
    const s = getSim().settings;
    const d = getSim().derived;
    return s && validRate(Number(s.dataRate)) ? Number(s.dataRate)
      : d && d.cameraRate ? d.cameraRate
      : null;
  }

  // The lab console's address [IPv4 text]: the settings' labConsoleAddress.
  const validAddress = a =>
    /^\d{1,3}(\.\d{1,3}){3}$/.test(a) && a.split('.').every(p => Number(p) <= 255);
  function labAddress() {
    const s = getSim().settings;
    return s && typeof s.labConsoleAddress === 'string' ? s.labConsoleAddress.trim() : '';
  }

  // Point the local server's link at the settings' address when they differ (e.g. settings
  // loaded from another file); once per address.
  function followAddress(address) {
    if (!address || !validAddress(address) || !S.lab.url ||
        S.lab.url.split('//')[1]?.split(':')[0] === address ||
        S.addressSent === address) {
      return;
    }
    S.addressSent = address;
    api('/api/lab/address', { address })
      .catch(e => toast(`Lab console address: ${e.message}`, 'error', 6000));
  }

  // The view's message until the platforms are seen.
  function updateEmpty() {
    const live = performance.now() - S.lastFrame < 2000;
    $('#expEmpty').classList.toggle('hidden', live);
    $('#expEmptyDetail').textContent = !S.lab.reachable
      ? 'The lab console isn\'t reachable from here.'
      : S.stream.running ? 'Cameras are on; waiting for the platforms to be seen…'
      : 'Start the cameras to see the platforms here.';
  }

  // The build on the TX2

  // Looks for the diagram's compiled program on the TX2.
  async function refreshBuildOnLab() {
    S.buildFor = model();
    if (!S.lab.reachable || !S.lab.experiments || !model()) {
      S.buildOnLab = null;
      return render();
    }
    try {
      S.buildOnLab =
        (await lab(`experiment/build?model=${encodeURIComponent(model())}`)).build || null;
    } catch {
      S.buildOnLab = null;
    }
    render();
  }

  // Actions

  // Cameras: stop, or send the marker file (each LED from its platform's CG: MATLAB's
  // trackerPositions, from the project's LED positions and the scales) and start at the data
  // rate. The lab console writes the file next to StreamData, which reads it as it starts.
  $('#expCameras').onclick = () => act(async () => {
    if (S.stream.running) {
      await lab('stream/stop');
      return;
    }
    const rate = dataRate();
    const markers = (getSim().derived || {}).trackerPositions;
    if (!rate || !markers) {
      throw new Error('No data rate or LED positions yet: the settings haven\'t come from ' +
        'MATLAB.');
    }
    await lab('tracker_positions', { text: markers });
    await lab(`stream/start?frequency=${rate}`);
    toast(`Cameras starting at ${rate} Hz`, 'success', 3000);
  });

  // The Frequency's padlock.
  paintLocks();
  onLockChange(() => render());
  for (const b of document.querySelectorAll('#pageExperiment [data-ratelock]')) {
    b.onclick = handleLockClick;
  }

  // The lab console's address: into the settings, and the local server's link.
  $('#expLabAddress').addEventListener('change', ev => {
    const el = ev.target;
    const v = el.value.trim();
    const ok = validAddress(v);
    el.classList.toggle('invalid', !ok);
    if (!ok) {
      return toast('The lab console address must be an IPv4 address such as ' +
        `${constant('IP_LAB_CONSOLE')}.`, 'error', 6000);
    }
    if (v === labAddress()) {
      return;
    }
    if (!setSetting('labConsoleAddress', v)) {
      return toast('The settings haven\'t come from MATLAB yet.', 'error', 5000);
    }
    S.addressSent = v;
    act(async () => {
      await api('/api/lab/address', { address: v });
      toast(`Lab console address set to ${v}. Build again: the platforms send their clocks, ` +
        'messages and live streams to it.', 'success', 8000);
    });
    render();
  });

  // The Frequency: into the settings' dataRate.
  $('#expFrequency').addEventListener('change', ev => {
    const el = ev.target;
    const v = Number(el.value);
    const ok = el.value !== '' && validRate(v);
    el.classList.toggle('invalid', !ok);
    if (!ok) {
      return toast(`Frequency must be a whole number of Hz, 1 to ${constant('MAX_DATA_RATE')}.`,
        'error', 5000);
    }
    if (!setSetting('dataRate', v)) {
      toast('The settings haven\'t come from MATLAB yet.', 'error', 5000);
    }
    render();
  });

  // Plan: what the build would do (MATLAB's experiment.plan).
  $('#expPlan').onclick = () => act(async () => {
    const p = await matlab('experiment.plan');
    S.plan = p;
    const warn = (p.warnings || []).map(w => `<div class="warn-text">⚠ ${esc(w)}</div>`).join('');
    const plats = Array.isArray(p.platforms) ? p.platforms : [p.platforms].filter(Boolean);
    const segs = Array.isArray(p.segments) ? p.segments : [p.segments].filter(Boolean);
    const all = segs.length === 9;
    $('#expPlanOut').innerHTML = `Builds <b>${esc(p.model)}</b> on ${esc(p.board.user)}@${esc(p.board.address)} for ${esc(plats.map(platformName).join(', ') || 'no platform')}.
      <div class="${all ? '' : 'warn-text'}">Runs ${all ? 'every phase' : esc(segs.map(n => n.replace('Phase ', '')).join(', ')) + ' (others skipped)'}: ${esc(String(Math.round(p.tsim * 10) / 10))} s.</div>
      <details><summary>Details</summary><div class="mono small">Program: ${esc(p.program)}</div>
      <div>For the build only (undone afterwards, diagram not saved): ${esc((p.changes || []).join('; '))}.</div></details>${warn}`;
  });

  // Connect MATLAB to the TX2 (with its password the first time).
  $('#expConnect').onclick = () => act(async () => {
    const pw = $('#expPassword').value;
    $('#expConnectOut').innerHTML = '<span class="spinner dark"></span> Connecting ' +
      '(the first time, MATLAB sets up its server on the TX2)…';
    try {
      const r = await matlab('experiment.connect', pw ? { password: pw } : {});
      $('#expConnectOut').textContent = `✓ MATLAB is connected to ${r.user}@${r.address}.`;
      $('#expConnectOut').className = 'hint ok';
    } catch (e) {
      $('#expConnectOut').textContent = `✕ ${e.message}`;
      $('#expConnectOut').className = 'hint bad';
    } finally {
      $('#expPassword').value = '';
    }
  });

  // A platform's trash button: take it out of the experiment (Scenario's platform switches).
  $('#expPlatforms').addEventListener('click', ev => {
    const b = ev.target.closest('[data-remove]');
    if (b && !b.disabled) {
      setPlatform(b.dataset.remove, false);
    }
  });

  // A failed build's Details: its error entry in the command window, opened.
  $('#expBuildOut').addEventListener('click', ev => {
    if (!ev.target.closest('[data-err="details"]') || !S.build.error_details) {
      return;
    }
    if (!S.errorEntry || !S.errorEntry.isConnected) {
      S.errorEntry = printError('Build failed', S.build.error_details);
    }
    S.errorEntry.open = true;
    S.errorEntry.scrollIntoView({ block: 'nearest' });
  });

  // Build, deploy, start and stop.
  $('#expBuild').onclick = () => act(async () => {
    if (!confirm(`Build ${model()} on the TX2? MATLAB generates the code and compiles it there ` +
        '(a few minutes). The diagram file isn\'t changed.')) {
      return;
    }
    $('#expBuildLive').textContent = '';
    S.build = await api('/api/experiment/build', {});
    render();
  });

  $('#expDeploy').onclick = () => act(async () => {
    await lab('experiment/deploy', { model: model(), platforms: selected() });
  });

  $('#expStart').onclick = () => act(async () => {
    if (!S.stream.running && !confirm('The cameras are off: the platforms will not know where ' +
        'they are. Start anyway?')) {
      return;
    }
    if (!confirm(`Start ${model()} on the ${selected().map(platformName).join(', ')}? ` +
        'Make sure the table is clear and the E-stop is at hand.')) {
      return;
    }
    viewer?.clearLivePaths();
    await lab('experiment/start', { platforms: selected() });
  });

  $('#expStop').onclick = () => act(async () => {
    await lab('experiment/stop');
  });

  // Data: the lab console collects the platforms' files, this server downloads them, and
  // MATLAB converts them to .mat.
  $('#expData').onclick = () => act(async () => {
    const out = $('#expDataOut');
    out.dataset.local = '1';
    out.textContent = 'Collecting the data files from the platforms…';
    const st = await lab('experiment/fetch');
    const run = st.last_run && st.last_run.run;
    if (!run) {
      throw new Error('Nothing was collected.');
    }
    const got = await api('/api/lab/download', { run });
    if (!got.files.length) {
      out.textContent = `No data files came back (${run}).`;
      return;
    }
    out.textContent = `Saved ${got.files.length} file(s) in ${got.folder}; converting with MATLAB…`;
    const dec = await matlab('experiment.decode', { folder: got.folder });
    const files = Array.isArray(dec.files) ? dec.files : [dec.files].filter(Boolean);
    out.textContent = `✓ ${got.folder}: ${files.length} .mat file(s) (dataClass_rt).`;
  });

  // The view

  const livePaths = $('#expLivePaths');
  const simOverlay = $('#expSimOverlay');
  livePaths.checked = storage.get('spot.exp.livePaths', true);
  simOverlay.checked = storage.get('spot.exp.simOverlay', false);

  // What the view shows: the live platforms, the simulation over them (if chosen), and poses as
  // chosen in the view's toolbar (shared with the Simulation tab). The simulation's whole paths
  // show until the experiment's clock runs.
  function layers() {
    if (!S.shown) {
      return;
    }
    const res = getSim().result;
    viewer?.setLayers({
      live: true, sim: simOverlay.checked && Boolean(res), setup: getSim().pose !== 'off',
    });
    viewer?.setPrimaryShown(true);
    viewer?.setLivePaths(livePaths.checked);
    if (res && !S.clockSync) {
      viewer?.setSimTime(res.t[res.t.length - 1]);
    }
  }
  document.addEventListener('spot-pose', layers);
  for (const [el, key] of [[livePaths, 'livePaths'], [simOverlay, 'simOverlay']]) {
    el.onchange = () => {
      storage.set(`spot.exp.${key}`, el.checked);
      layers();
    };
  }
  $('#expClearPaths').onclick = () => viewer?.clearLivePaths();

  // While shown: follow settings changed on the Simulation tab, and the build timer.
  setInterval(() => {
    if (S.shown) {
      updateEmpty();
      render();
    }
  }, 1000);

  // While building: look for the compiled program on the TX2 every 5 s.
  setInterval(() => {
    if (S.build.running && S.lab.experiments) {
      refreshBuildOnLab();
    }
  }, 5000);

  return {
    // The page's state: the lab console's snapshot, the build and MATLAB.
    onState(st) {
      S.lab = st.lab || S.lab;
      const snap = S.lab.snapshot || {};
      S.stream = snap.stream || {};
      S.monitor = snap.monitor || {};
      S.hardware = snap.hardware || {};
      S.experiment = snap.experiment || {};
      S.build = st.build || {};
      S.matlab = st.matlab || S.matlab;
      if (snap.led_owner) {
        viewer?.setLedOwner(snap.led_owner);
      }
      render();
      refreshBuildOnLab();
    },
    // The lab link changed: a toast when it is gained or lost.
    onLab(l) {
      const was = S.lab.reachable;
      S.lab = l;
      const snap = l.snapshot || {};
      if (l.reachable) {
        S.stream = snap.stream || S.stream;
        S.monitor = snap.monitor || S.monitor;
        S.hardware = snap.hardware || S.hardware;
        S.experiment = snap.experiment || S.experiment;
      }
      render();
      if (l.reachable !== was) {
        toast(l.reachable ? 'Connected to the lab console' : 'Lost the lab console',
          l.reachable ? 'success' : 'error', 4000);
        refreshBuildOnLab();
      }
    },
    // A PhaseSpace frame.
    onFrame(frame) {
      S.lastFrame = performance.now();
      if (S.shown) {
        viewer?.update(frame);
      }
    },
    onStream(st) {
      S.stream = st;
      render();
    },
    onMonitor(m) {
      S.monitor = m;
      render();
    },
    // A platform's hardware state.
    onHardware(p) {
      const list = (S.hardware.platforms || []).filter(x => x.name !== p.name);
      S.hardware = { ...S.hardware, platforms: [...list, p] };
      render();
    },
    // The experiment's status: the simulation overlaid in step with it, by the platforms' own
    // clock.
    onExperiment(e) {
      const before = S.experiment.phase;
      S.experiment = e;
      const res = getSim().result;
      const c = Object.values(e.clocks || {}).find(x => x.age < 3);
      S.clockSync = Boolean(c && res);
      if (S.clockSync && S.shown && simOverlay.checked) {
        viewer?.setSimTime(c.t);
      }
      if (e.phase !== before && e.phase === 'deployed') {
        refreshBuildOnLab();
      }
      render();
    },
    onBuildLog(e) {
      appendBuildLog(e.lines || []);
    },
    // The build's status: a toast (and its error entry) when it ends.
    onBuild(b) {
      const finished = S.build.running && !b.running;
      S.build = b;
      render();
      if (finished) {
        toast(b.ok ? 'Build finished' : `Build failed: ${b.error}`, b.ok ? 'success' : 'error',
          b.ok ? 4000 : 12000);
        if (!b.ok && b.error_details) {
          S.errorEntry = printError('Build failed', b.error_details);
        }
        refreshBuildOnLab();
      }
    },
    onMatlab(m) {
      S.matlab = m;
      render();
    },
    show() {
      S.shown = true;
      layers();
      render();
      refreshBuildOnLab();
    },
    hide() {
      S.shown = false;
    },
  };
}
