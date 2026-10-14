// UI logic for the SPOT Console. Talks to app.py over JSON + server-sent events.
import { Viewer, CRAFT_COLORS, LED_COLORS } from './viewer.js';
import { platformName } from './platforms.js';
import { initHardware } from './hardware.js';
import { initSimulation } from './sim.js';
import { initExperiment } from './experiment.js';
import { initResults } from './results.js';
import { createDataStore } from './data.js';
import { initMessages } from './messages.js';
import { initLivePlot } from './liveplot.js';
import { initInteractive } from './interactive.js';
import { initDesigner } from './designer.js';
import { initCalibration } from './calibration.js';

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];

const S = {
  host: {}, settings: {}, toolchains: { checking: true }, build: {}, stream: {}, monitor: {},
  ledOwner: {}, lastFrameAt: 0, lastTelemetryAt: 0, historyUntil: 0, monitorErrorShown: null,
};
let viewer = null;

// ---------------------------------------------------------------- helpers

// ?capture keeps the 3D view's image so it can be saved or screenshotted
const CAPTURE = new URLSearchParams(location.search).has('capture');

// The local console (started by SPOT in MATLAB) opens this page with ?token=...; it is kept for this
// tab and sent with every request, and taken out of the address bar
const TOKEN = (() => {
  const params = new URLSearchParams(location.search);
  let t = params.get('token');
  try {
    if (t) sessionStorage.setItem('spot.token', t);
    else t = sessionStorage.getItem('spot.token');
  } catch { /* storage blocked: keep it in memory */ }
  if (params.has('token')) history.replaceState(null, '', location.pathname + (CAPTURE ? '?capture' : '') + location.hash);
  return t || '';
})();

async function api(path, body) {
  const headers = TOKEN ? { 'X-SPOT-Token': TOKEN } : {};
  const opts = body === undefined ? { headers } :
    { method: 'POST', headers: { ...headers, 'Content-Type': 'application/json', 'X-SPOT-Client': 'webapp' }, body: JSON.stringify(body) };
  const res = await fetch(path, opts);
  let data = {};
  try { data = await res.json(); } catch { /* not JSON */ }
  if (!res.ok) {
    const err = new Error(data.error || `Request failed (${res.status})`);
    err.details = data.details || null;   // MATLAB's errors: causes, blocks, report (spot.sim.errorDetails)
    throw err;
  }
  return data;
}

function toast(message, kind = 'info', ms = 6000) {
  const el = document.createElement('div');
  el.className = `toast ${kind}`;
  el.textContent = message;
  $('#toasts').append(el);
  setTimeout(() => el.remove(), ms);
}

async function act(fn) {
  try { return await fn(); } catch (e) { toast(e.message, 'error', 9000); }
}

const fmtTime = t => new Date(t * 1000).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
const fmtDur = sec => {
  sec = Math.max(0, Math.floor(sec));
  const h = Math.floor(sec / 3600), m = Math.floor(sec / 60) % 60, s = sec % 60;
  return (h ? `${h}:${String(m).padStart(2, '0')}` : `${m}`) + `:${String(s).padStart(2, '0')}`;
};
const wrapDeg = rad => { let d = rad * 180 / Math.PI; d = ((d + 180) % 360 + 360) % 360 - 180; return d; };
const num = (v, digits) => (v == null ? '—' : v.toFixed(digits));
const storage = {
  get(k, dflt) { try { const v = localStorage.getItem(k); return v === null ? dflt : JSON.parse(v); } catch { return dflt; } },
  set(k, v) { try { localStorage.setItem(k, JSON.stringify(v)); } catch { /* private mode */ } },
};

// ---------------------------------------------------------------- rendering

function renderHost() {
  const h = S.host;
  const os = h.wsl ? `Linux in WSL ${h.wsl.version} (${h.wsl.networking} networking)` : h.os;
  $('#hostLine').textContent = h.name ? `${h.name} · ${os}` : 'Connecting…';
  $('#hostLine').title = S.projectDir ? `Project folder: ${S.projectDir}` : '';
}

function renderSettings() {
  const out = $('#outputDir');
  out.value = S.settings.output_dir || '';
  out.title = S.settings.output_dir || '';
  if (document.activeElement !== $('#frequency') && S.settings.frequency) $('#frequency').value = S.settings.frequency;
  if (document.activeElement !== $('#monitorPort') && S.settings.monitor_port) $('#monitorPort').value = S.settings.monitor_port;
  const transport = S.settings.transport || 'tcp';
  for (const b of $$('[data-transport]')) b.classList.toggle('active', b.dataset.transport === transport);
  // In WSL 2's default NAT networking, PhaseSpace's UDP frames can't reach StreamData
  const natUdp = transport === 'udp' && S.host.wsl && S.host.wsl.networking === 'nat';
  const hint = $('#transportHint');
  hint.hidden = !natUdp;
  hint.textContent = "UDP won't arrive through WSL's NAT networking: use TCP, or switch WSL to mirrored networking.";
}

function renderTargets() {
  const tcs = S.toolchains || {};
  const tc = tcs.linux;
  const tile = $('.target');
  const status = $('[data-role=status]', tile);
  const last = $('[data-role=last]', tile);
  const btn = $('[data-role=build]', tile);

  if (!tc) {
    status.className = 'target-status';
    status.textContent = 'Checking for a compiler…';
  } else if (tc.available) {
    status.className = 'target-status ok';
    status.textContent = 'Compiler Ready';
    status.title = tc.via;
  } else {
    status.className = 'target-status bad';
    status.textContent = tc.reason;
    status.title = tc.reason;
  }

  const lb = S.build.last;
  if (S.build.running) {
    last.className = 'target-last'; last.textContent = 'Compiling…';
  } else if (lb) {
    last.className = `target-last ${lb.ok ? 'ok' : 'bad'}`;
    last.textContent = lb.ok ? `✓ Built ${fmtTime(lb.finished)} (${lb.seconds} s)` : `✕ Failed ${fmtTime(lb.finished)}, see Build Log`;
  } else {
    last.className = 'target-last'; last.textContent = '';
  }

  btn.disabled = Boolean(S.build.running) || !tc || !tc.available;
  btn.innerHTML = S.build.running ? '<span class="spinner"></span><span>Compiling…</span>' : '<span>Compile</span>';
  $('#recheckBtn').disabled = Boolean(tcs.checking);
  $('#recheckBtn').textContent = tcs.checking ? 'Checking…' : 'Re-Check';
}

function renderStream() {
  const st = S.stream;
  const pill = $('#streamPill');
  const btn = $('#streamBtn');
  if (st.running) {
    pill.className = 'pill live';
    pill.textContent = `Streaming · ${fmtDur(Date.now() / 1000 - st.started)} · ${st.frequency} Hz`;
    btn.className = 'btn btn-stop';
    btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><rect x="6" y="6" width="12" height="12" rx="2" fill="currentColor"/></svg><span>Stop Streaming</span>';
    btn.disabled = false;
  } else {
    const failed = st.last_exit != null && st.last_exit !== 0;
    pill.className = failed ? 'pill err' : 'pill';
    pill.textContent = failed ? `Exited (Code ${st.last_exit})` : 'Stopped';
    btn.className = 'btn btn-go';
    btn.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M8 5v14l11-7z"/></svg><span>Start Streaming</span>';
    btn.disabled = !st.exe_exists;
  }
  if (st.running && st.started_by) pill.title = `Started ${fmtTime(st.started)} by ${st.started_by}`;
  else if (!st.running && st.stopped_by) pill.title = `Stopped ${fmtTime(st.stopped)} by ${st.stopped_by}`;
  else pill.title = '';
  btn.title = st.exe ? `Runs ${st.exe}` : '';
  const exe = $('#exeLine');
  exe.hidden = !st.exe || st.exe_exists;
  exe.textContent = 'Not built yet. Click Compile first.';
  exe.title = st.exe || '';
  $('#frequency').disabled = Boolean(st.running);
  for (const b of $$('[data-transport]')) b.disabled = Boolean(st.running);
}

function renderMonitor() {
  const m = S.monitor;
  if (m.error && S.monitorErrorShown !== m.error) {
    S.monitorErrorShown = m.error;
    toast(m.error, 'error', 12000);
  }
  updateStale();
}

function updateStale() {
  const age = (performance.now() - S.lastFrameAt) / 1000;
  const stale = !S.lastFrameAt || age > 2;
  const empty = $('#viewerEmpty');
  empty.classList.toggle('hidden', !stale);
  if (stale) {
    const m = S.monitor;
    $('#viewerEmptyDetail').textContent = m.error ? m.error
      : S.lastFrameAt ? `No packets for ${Math.round(age)} s (listening on 127.0.0.1:${m.port}).`
      : S.stream.running ? `Streaming started; waiting for the first packet on 127.0.0.1:${m.port}…`
      : 'Press Start Streaming to see the spacecraft.';
    $('#statRate').textContent = '0 Hz';
  }
  $('#lastPacket').textContent = S.lastFrameAt ? (stale ? `Last Packet ${Math.round(age)} s Ago` : 'Live') : 'No Packets Yet';
}

function renderTelemetry(frame) {
  const rows = Object.keys(CRAFT_COLORS).map(name => {
    const c = frame.crafts[name] || {};
    const t = c.tracked;
    return `<tr class="${t ? '' : 'untracked'}"><td><span class="craft"><i style="background:${CRAFT_COLORS[name]}"></i>${platformName(name)}</span></td>` +
      `<td>${t ? num(c.x, 3) : '—'}</td><td>${t ? num(c.y, 3) : '—'}</td>` +
      `<td>${t && c.att != null ? wrapDeg(c.att).toFixed(1) : '—'}</td>` +
      `<td>${t ? num(c.vx, 3) : '—'}</td><td>${t ? num(c.vy, 3) : '—'}</td>` +
      `<td>${t && c.w != null ? (c.w * 180 / Math.PI).toFixed(1) : '—'}</td></tr>`;
  });
  $('#telemetryBody').innerHTML = rows.join('');
  const leds = [...frame.leds].sort((a, b) => a[0] - b[0]);
  $('#ledCount').textContent = `${leds.length} Visible`;
  $('#ledChips').innerHTML = leds.map(([k, x, y, z]) => {
    const owner = S.ledOwner[k];
    const color = LED_COLORS[owner] || LED_COLORS.none;
    const title = `LED ${k}${owner ? ` (${platformName(owner)})` : ' (not on a body)'}: x ${x.toFixed(3)}  y ${y.toFixed(3)}  z ${z.toFixed(3)} m`;
    return `<span class="chip" title="${title}" style="color:${color};border-color:${color}55;background:${color}14">${k}</span>`;
  }).join('');
  $('#statRate').textContent = `${frame.rate} Hz`;
}

function renderTelemetryEmpty() {
  renderTelemetry({ crafts: {}, leds: [], rate: 0 });
}

// ---------------------------------------------------------------- logs

const logEls = { build: $('#logBuild'), stream: $('#logStream'), hardware: $('#logHardware') };
const logCounts = { build: 0, stream: 0, hardware: 0 };
const pending = { build: [], stream: [], hardware: [] };
let flushQueued = false;
const MAX_LOG_LINES = 3000;

function classify(line) {
  if (line.startsWith('$ ')) return 'l-cmd';
  if (line.startsWith('===')) return /FAILED|exited \(code [^0]/.test(line) ? 'l-err' : /succeeded/.test(line) ? 'l-ok' : 'l-head';
  if (/\berror\b|fatal|could not|cannot|failed|link to the Teensy lost|not answering|doesn't accept/i.test(line)) return 'l-err';
  if (/warning|let go of it|cancelled/i.test(line)) return 'l-warn';
  return '';
}

function queueLog(entry) {
  if (!pending[entry.channel]) return;  // a channel this page doesn't show (e.g. matlab)
  pending[entry.channel].push(entry.line);
  if (!flushQueued) { flushQueued = true; requestAnimationFrame(flushLogs); }
}

function flushLogs() {
  flushQueued = false;
  for (const ch of Object.keys(pending)) {
    const lines = pending[ch];
    if (!lines.length) continue;
    pending[ch] = [];
    const el = logEls[ch];
    const atBottom = el.scrollHeight - el.scrollTop - el.clientHeight < 40;
    const frag = document.createDocumentFragment();
    for (const line of lines) {
      const div = document.createElement('div');
      const cls = classify(line);
      if (cls) div.className = cls;
      div.textContent = line || ' ';
      frag.append(div);
    }
    el.append(frag);
    while (el.childElementCount > MAX_LOG_LINES) el.firstElementChild.remove();
    logCounts[ch] += lines.length;
    $(`[data-count=${ch}]`).textContent = logCounts[ch];
    if (atBottom) el.scrollTop = el.scrollHeight;
  }
}

function resetLogs(history) {
  for (const ch of Object.keys(logEls)) {
    logEls[ch].textContent = '';
    logCounts[ch] = 0;
    pending[ch] = [];
    for (const e of history[ch] || []) pending[ch].push(e.line);
    S.historyUntil = Math.max(S.historyUntil, ...(history[ch] || []).map(e => e.time));
  }
  flushLogs();
  for (const el of Object.values(logEls)) el.scrollTop = el.scrollHeight;
}

function showTab(tab) {
  for (const b of $$('.tabs button')) b.classList.toggle('active', b.dataset.tab === tab);
  for (const ch of ['build', 'stream']) logEls[ch].classList.toggle('active', ch === tab);
  $('#console').classList.remove('collapsed');
  logEls[tab].scrollTop = logEls[tab].scrollHeight;
}

// ---------------------------------------------------------------- state & events

function renderDestinations() {
  const extras = S.settings.extra_destinations || [];
  $('#destsLabel').title = `Computers that get the packets on UDP port 31534. Always sent to: ${(S.baseDestinations || []).join(', ') || 'none'}`;
  const box = $('#extraDests');
  box.textContent = '';
  for (const ip of extras) {
    const chip = document.createElement('span');
    chip.className = 'chip dest-chip';
    chip.textContent = ip;
    const x = document.createElement('button');
    x.type = 'button';
    x.title = `Stop sending to ${ip}`;
    x.textContent = '×';
    x.onclick = () => act(async () => {
      S.settings.extra_destinations = (await api('/api/destinations/remove', { ip })).extra_destinations;
      renderDestinations();
    });
    chip.append(x);
    box.append(chip);
  }
  // Quick add for the computer this page is open on (as the server sees it)
  const me = $('#destAddMe');
  const ip = S.clientIp;
  const usable = ip && !ip.startsWith('127.') && !extras.includes(ip) && !(S.baseDestinations || []).includes(ip);
  me.hidden = !usable;
  if (usable) me.textContent = `+ This Computer (${ip})`;
}

async function addDestination(ip) {
  S.settings.extra_destinations = (await api('/api/destinations/add', { ip })).extra_destinations;
  renderDestinations();
  toast(`Now also sending to ${ip}`, 'success', 3000);
}

async function loadState() {
  const st = await api('/api/state');
  S.features = st.features || { mode: 'lab', phasespace: true, hardware: true, simulation: false };
  applyFeatures(st);
  if (S.features.mode === 'local') {
    S.host = st.host;
    S.projectDir = st.project_dir;
    renderHost();
    resetLogs(st.logs);
    simulation.onState(st);
    experiment.onState(st);
    results.onMatlab(st.matlab || { connected: false });
    S.lab = st.lab || null;
    renderConn();
    designer.onState(st);
    calibration.onState(st);
    return;
  }
  S.clientIp = st.client_ip;
  S.baseDestinations = st.base_destinations;
  S.host = st.host;
  S.projectDir = st.project_dir;
  S.settings = st.settings;
  S.toolchains = st.toolchains;
  S.build = st.build;
  S.stream = st.stream;
  S.monitor = st.monitor;
  S.ledOwner = st.led_owner || {};
  viewer?.setLedOwner(S.ledOwner);
  resetLogs(st.logs);
  renderHost(); renderSettings(); renderTargets(); renderStream(); renderMonitor(); renderDestinations();
  hardware.setAll(st.hardware);
  interactive.onStream(st.stream);
  interactive.onHardware(st.hardware);
  interactive.setState(st.interactive, st.interactive_info);
  designer.onState(st);
  calibration.onState(st);
}

function setConn(kind) {
  S.conn = kind;
  renderConn();
}

// The badge in the top right. On the lab console: whether this page reaches it. On the local
// console (SPOT in MATLAB), which always runs on this computer: whether the lab console (TX2) is
// reachable, since that is what decides if experiments and the Lab console link work.
function renderConn() {
  const b = $('#connBadge'), link = $('#labLink');
  let kind = S.conn || 'warn';
  let text = kind === 'ok' ? 'Connected' : kind === 'bad' ? 'Server Offline' : 'Connecting';
  let title = kind === 'bad' ? 'This page lost its server' : '';
  const local = S.features && S.features.mode === 'local';
  if (local && kind === 'ok' && S.lab) {
    const why = (S.lab.error || '').replace(/^Not reachable \((.*)\)$/, '$1');
    kind = S.lab.reachable ? 'ok' : 'bad';
    text = S.lab.reachable ? 'Lab Connected' : 'Lab Disconnected';
    title = S.lab.reachable ? `The lab console (${S.lab.url}) is reachable`
      : `The lab console (${S.lab.url || 'TX2'}) isn't reachable${why ? `: ${why}` : ''}. Connect to the lab network (SRCL Wi-Fi or wired) for experiments and the live view.`;
  }
  b.className = `badge badge-${kind}`;
  b.innerHTML = `<span class="dot"></span>${text}`;
  b.title = title;
  if (local) {
    const off = !(S.lab && S.lab.reachable) || S.conn !== 'ok';
    link.classList.toggle('disabled', off);
    link.setAttribute('aria-disabled', String(off));
    link.tabIndex = off ? -1 : 0;
    link.title = off ? "The lab console isn't reachable from this network"
      : 'PhaseSpace and the platform hardware run on the lab console (the TX2)';
  }
}

function connect() {
  const es = new EventSource('/api/events');
  const on = (name, fn) => es.addEventListener(name, e => fn(JSON.parse(e.data)));
  es.onopen = () => { setConn('ok'); act(loadState); };
  es.onerror = () => setConn('bad');
  on('log', e => { if (e.time > S.historyUntil) queueLog(e); });
  on('toolchains', d => { S.toolchains = d; renderTargets(); });
  on('build_status', d => {
    const finished = S.build.running && !d.running;
    if (d.running && !S.build.running) showTab('build');
    S.build = d; renderTargets();
    if (finished && d.last) toast(d.last.ok ? `StreamData compiled into ${d.last.out_dir}` : 'Build failed; see the Build log.',
                                  d.last.ok ? 'success' : 'error');
  });
  on('stream_status', d => {
    if (d.running && !S.stream.running) showTab('stream');
    // Let everyone watching know when someone else (another browser or a script) starts or stops it
    if (Boolean(d.running) !== Boolean(S.stream.running) && !S.ownStreamAction && S.stream.exe) {
      if (d.running) toast(`Streaming started at ${d.frequency} Hz by ${d.started_by}`, 'success');
      else toast(d.stopped_by ? `Streaming stopped by ${d.stopped_by}` : `StreamData exited (code ${d.last_exit})`,
                 d.stopped_by ? 'info' : 'error');
    }
    S.stream = d; renderStream();
    interactive.onStream(d);
    calibration.onStream(d);
  });
  on('settings', d => { S.settings = d; renderSettings(); renderDestinations(); });
  on('monitor', d => { S.monitor = d; renderMonitor(); });
  on('hardware_platform', p => { hardware.setPlatform(p); interactive.onHardwarePlatform(p); });
  on('interactive', d => interactive.onStatus(d));
  on('matlab', m => { simulation.onMatlab(m); experiment.onMatlab(m); results.onMatlab(m); });
  on('build', b => experiment.onBuild(b));
  on('build_log', e => experiment.onBuildLog(e));
  on('lab', l => {
    S.lab = l;
    if (l && l.url) $('#labLink').href = l.url;   // (the address can change: Experiment tab, Lab)
    renderConn(); experiment.onLab(l); designer.onLab(l); calibration.onLab(l);
  });
  on('design', d => designer.onDesign(d));
  on('lab_design', d => designer.onLabDesign(d));
  on('calibration', d => {
    calibration.onCalibration(d);
    // (lab console: Interactive Mode's controller now flies with the new calibrated thrust; its card says so)
    if (S.features && S.features.mode !== 'local') act(async () => { const r = await api('/api/interactive'); interactive.setState(r, r.controller); });
  });
  on('calibration_run', d => calibration.onRun(d));
  on('lab_calibration', d => calibration.onLabCalibration(d));
  on('lab_frame', f => experiment.onFrame(f));
  on('lab_stream', d => experiment.onStream(d));
  on('lab_monitor', d => experiment.onMonitor(d));
  on('lab_hardware', d => experiment.onHardware(d));
  on('lab_experiment', d => experiment.onExperiment(d));
  on('lab_live_data', d => livePlot.experimentData(d));
  on('sim', d => { if (!d.running && d.ok) results.onSimFinished(); simulation.onSim(d); });
  on('matlab_event', e => simulation.onMatlabEvent(e));
  on('frame', frame => {
    S.lastFrameAt = performance.now();
    viewer?.update(frame);
    interactive.onFrame(frame);
    if (S.lastFrameAt - S.lastTelemetryAt > 100) { S.lastTelemetryAt = S.lastFrameAt; renderTelemetry(frame); }
    $('#viewerEmpty').classList.add('hidden');
  });
}

// ---------------------------------------------------------------- folder dialog

const dialog = $('#folderDialog');
let folder = null;

async function openFolder(path) {
  const d = await api('/api/browse?path=' + encodeURIComponent(path || ''));
  folder = d;
  $('#folderPath').value = d.path;
  $('#folderUp').disabled = !d.parent;
  const list = $('#folderList');
  list.textContent = '';
  if (d.error) {
    list.innerHTML = `<li class="empty"></li>`;
    list.firstChild.textContent = `Can't open this folder: ${d.error}`;
  } else if (!d.dirs.length) {
    list.innerHTML = '<li class="empty">No Subfolders</li>';
  }
  for (const name of d.dirs) {
    const li = document.createElement('li');
    li.textContent = name;
    li.title = name;
    li.onclick = () => act(() => openFolder(d.path.endsWith(d.sep) ? d.path + name : d.path + d.sep + name));
    list.append(li);
  }
  const places = $('#folderPlaces');
  places.textContent = '';
  let drivesStarted = false;
  for (const p of d.places) {
    if (p.drive && !drivesStarted) { drivesStarted = true; places.append(document.createElement('hr')); }
    const b = document.createElement('button');
    b.type = 'button';
    b.textContent = p.label;
    b.title = p.path;
    b.onclick = () => act(() => openFolder(p.path));
    places.append(b);
  }
}

$('#browseBtn').onclick = () => act(async () => {
  await openFolder(S.settings.output_dir);
  dialog.showModal();
});
$('#folderUp').onclick = () => folder?.parent && act(() => openFolder(folder.parent));
$('#folderGo').onclick = () => act(() => openFolder($('#folderPath').value));
$('#folderPath').addEventListener('keydown', e => {
  if (e.key === 'Enter') { e.preventDefault(); act(() => openFolder($('#folderPath').value)); }
});
$('#newFolderName').addEventListener('keydown', e => {
  if (e.key === 'Enter') { e.preventDefault(); $('#newFolderBtn').click(); }
});
$('#newFolderBtn').onclick = () => act(async () => {
  const name = $('#newFolderName').value.trim();
  if (!name) return;
  const r = await api('/api/mkdir', { parent: folder.path, name });
  $('#newFolderName').value = '';
  await openFolder(r.path);
});
$('#folderSelect').onclick = () => act(async () => {
  S.settings = await api('/api/settings', { output_dir: folder.path });
  renderSettings();
  dialog.close();
  toast(`Output folder set to ${folder.path}`, 'success', 4000);
});

// ---------------------------------------------------------------- controls

$('[data-role=build]').onclick = () => act(async () => {
  S.build = await api('/api/build', {});
  renderTargets();
  showTab('build');
});
$('#recheckBtn').onclick = () => act(async () => { S.toolchains = await api('/api/toolchains/refresh', {}); renderTargets(); });

$('#streamBtn').onclick = () => act(async () => {
  const btn = $('#streamBtn');
  btn.disabled = true;
  S.ownStreamAction = true;
  try {
    S.stream = S.stream.running
      ? await api('/api/stream/stop', {})
      : await api('/api/stream/start', { frequency: Number($('#frequency').value) });
    if (S.stream.running) showTab('stream');
  } finally {
    S.ownStreamAction = false;
    renderStream();
  }
});
$('#frequency').addEventListener('change', () => act(async () => {
  S.settings = await api('/api/settings', { frequency: Number($('#frequency').value) });
  renderSettings();
}));
for (const b of $$('[data-transport]')) {
  b.onclick = () => act(async () => {
    S.settings = await api('/api/settings', { transport: b.dataset.transport });
    renderSettings();
  });
}
$('#destAdd').onclick = () => act(async () => {
  const ip = $('#destInput').value.trim();
  if (!ip) return;
  await addDestination(ip);
  $('#destInput').value = '';
});
$('#destInput').addEventListener('keydown', e => { if (e.key === 'Enter') { e.preventDefault(); $('#destAdd').click(); } });
$('#destAddMe').onclick = () => act(() => addDestination(S.clientIp));
$('#savePortBtn').onclick = () => act(async () => {
  S.settings = await api('/api/settings', { monitor_port: Number($('#monitorPort').value) });
  renderSettings();
  toast(`Viewer port set to ${S.settings.monitor_port}`, 'success', 3000);
});

for (const b of $$('.tabs button')) b.onclick = () => showTab(b.dataset.tab);
$('#clearLog').onclick = () => {
  const tab = $('.tabs button.active').dataset.tab;
  logEls[tab].textContent = '';
  logCounts[tab] = 0;
  $(`[data-count=${tab}]`).textContent = '0';
};
$('#clearHwLog').onclick = () => {
  logEls.hardware.textContent = '';
  logCounts.hardware = 0;
  $('[data-count=hardware]').textContent = '0';
};
$('#toggleConsole').onclick = () => {
  const c = $('#console');
  c.classList.toggle('collapsed');
  $('#toggleConsole').textContent = c.classList.contains('collapsed') ? '▴' : '▾';
};

// ---------------------------------------------------------------- pages

const PAGES = { simulation: $('#pageSimulation'), experiment: $('#pageExperiment'), results: $('#pageResults'),
                phasespace: $('#pagePhasespace'), hardware: $('#pageHardware'), designer: $('#pageDesigner'),
                calibration: $('#pageCalibration') };

function pageAvailable(page) {
  return Boolean(PAGES[page]) && (!S.features || S.features[page]);
}

function showPage(page) {
  if (!pageAvailable(page)) page = S.features && S.features.mode === 'local' ? 'simulation' : 'phasespace';
  for (const b of $$('[data-page]')) b.classList.toggle('active', b.dataset.page === page);
  for (const [name, el] of Object.entries(PAGES)) el.hidden = name !== page;
  storage.set('spot.page', page);
  if (location.hash !== `#${page}`) history.replaceState(null, '', `#${page}`);
  if (page === 'hardware') logEls.hardware.scrollTop = logEls.hardware.scrollHeight;
  // One 3D view for every page: it moves into the page's slot, with that page's layers
  const slot = $(`[data-slot="${page}"]`);
  const view = $('#viewer');
  if (slot && view.parentElement !== slot) slot.prepend(view);
  view.dataset.showing = page;
  // The MATLAB command window is shared the same way by the Simulation and Experiment pages
  const consoleSlot = $(`[data-console-slot="${page}"]`);
  if (consoleSlot && !consoleSlot.contains($('#matlabConsole'))) consoleSlot.append($('#matlabConsole'));
  if (page === 'simulation' || page === 'experiment') messages.show(page);   // the messages panel, like the 3D view
  if (['simulation', 'experiment', 'results'].includes(page)) livePlot.show(page === 'results' ? null : page);   // no Live Plot on Results
  if (page !== 'simulation') simulation.hide();
  if (page !== 'experiment') experiment.hide();
  if (page !== 'results') results.hide();
  if (page !== 'designer') designer.hide();
  if (page !== 'calibration') calibration.hide();
  if (page === 'simulation') simulation.show();
  else if (page === 'experiment') experiment.show();
  else if (page === 'results') results.show();
  else if (page === 'designer') designer.show();
  else if (page === 'calibration') calibration.show();
  else viewer?.setLayers({ live: true, sim: $('#toggleOverlay').checked && simulation.hasResult(), setup: false });
  if (page !== 'experiment') viewer?.setLivePaths(false);
}

function applyFeatures(st) {
  const f = S.features;
  for (const b of $$('[data-page]')) b.hidden = !f[b.dataset.page];
  $('.page-tabs').hidden = $$('[data-page]').filter(b => !b.hidden).length < 2;
  $('#labLink').hidden = f.mode !== 'local' || !st.lab_url;
  $('#settingsBar').hidden = f.mode !== 'local';   // settings live in MATLAB (local console only)
  if (st.lab_url) $('#labLink').href = st.lab_url;
  $('.topstats .stat').hidden = !f.phasespace;
  // The lab console (TX2) drives the hardware; the local one (SPOT in MATLAB) runs simulations
  const name = f.mode === 'local' ? 'SPOT Local Simulations' : 'SPOT Hardware Interfaces';
  document.title = name;
  $('.brand h1').textContent = name;
  if (!S.pageShown) {
    S.pageShown = true;
    showPage(location.hash.slice(1) || storage.get('spot.page', f.mode === 'local' ? 'simulation' : 'phasespace'));
  }
}

for (const b of $$('[data-page]')) b.onclick = () => showPage(b.dataset.page);
window.addEventListener('hashchange', () => showPage(location.hash.slice(1)));
const hardware = initHardware({ api, act, toast, storage, fmtTime });

// ---------------------------------------------------------------- viewer

try {
  viewer = new Viewer($('#viewer'), { capture: CAPTURE });
  if (CAPTURE) window.spotViewer = viewer;   // for screenshots and scripted checks of the 3D view
} catch (e) {
  console.error(e);
  $('#viewerEmpty strong').textContent = '3D View Unavailable';
  $('#viewerEmptyDetail').textContent = 'This browser could not start WebGL. Telemetry still updates on the left.';
}

for (const b of $$('[data-view]')) {
  b.onclick = () => {
    for (const o of $$('[data-view]')) o.classList.toggle('active', o === b);
    viewer?.setView(b.dataset.view);
  };
}
const toggles = [['#toggleLedLabels', 'ledLabels', v => viewer?.setLedLabels(v)],
                 ['#toggleNames', 'names', v => viewer?.setNames(v)],
                 ['#toggleAxes', 'bodyAxes', v => viewer?.setBodyAxes(v)],
                 ['#toggleDrop', 'dropLines', v => viewer?.setDropLines(v)]];
for (const [sel, key, apply] of toggles) {
  const input = $(sel);
  input.checked = storage.get('spot.' + key, true);
  apply(input.checked);
  input.onchange = () => { storage.set('spot.' + key, input.checked); apply(input.checked); };
}

// Use CAD: off unless switched on (the models are loaded then); if they can't be loaded, it goes off
const cadToggle = $('#toggleCad');
const useCad = on => viewer?.setUseCad(on).catch(e => {
  cadToggle.checked = false;
  storage.set('spot.useCad', false);
  toast('Could not load the CAD models: ' + e.message, 'error');
});
cadToggle.checked = storage.get('spot.useCad', false);
if (cadToggle.checked) useCad(true);
cadToggle.onchange = () => { storage.set('spot.useCad', cadToggle.checked); useCad(cadToggle.checked); };

// ZED2 Viewpoint window: off unless switched on; its own close button also turns the switch off
const redCamToggle = $('#toggleRedCam');
redCamToggle.checked = storage.get('spot.redCam', false);
viewer?.setRedCam(redCamToggle.checked);
redCamToggle.onchange = () => { storage.set('spot.redCam', redCamToggle.checked); viewer?.setRedCam(redCamToggle.checked); };
if (viewer) viewer.redCam.onClose = () => { redCamToggle.checked = false; storage.set('spot.redCam', false); };
// which lens it shows (left, right or both side by side), remembered
viewer?.setRedCamLens(storage.get('spot.redCamLens', 'left'));
if (viewer) viewer.redCam.onLens = lens => storage.set('spot.redCamLens', lens);

const store = createDataStore({ api, toast });
const messages = initMessages({ storage });
const livePlot = initLivePlot({ api, storage });
const simulation = initSimulation({ api, act, toast, storage, viewer, store, messages, livePlot });
const experiment = initExperiment({ api, act, toast, storage, viewer, messages, livePlot, getSim: () => simulation.getState(),
                                   setPlatform: (name, on) => simulation.setPlatform(name, on),
                                   setSetting: (path, value) => simulation.setSetting(path, value),
                                   printError: (title, details) => simulation.printError(title, details) });
const results = initResults({ act, toast, storage, store, showPage });
const interactive = initInteractive({ api, act, toast, viewer, isShown: () => !$('#pagePhasespace').hidden });
// (on the local console the Chaser's arm in the Designer and the simulation's Arm platform are one choice;
// the lab console has no settings, so its Designer stands alone)
const designer = initDesigner({ api, act, toast, storage, viewer, getSim: () => simulation.getState(),
                                setArm: (on, mount) => simulation.setArm(on, mount) });
const calibration = initCalibration({ api, act, toast, storage, viewer, getStream: () => S.stream });
// The table slope overlay on the 3D view: off unless switched on; only with a table calibration
const slopeToggle = $('#toggleSlope');
slopeToggle.checked = storage.get('spot.tableSlope', false);
viewer?.setTableSlopeShown(slopeToggle.checked);
slopeToggle.onchange = () => { storage.set('spot.tableSlope', slopeToggle.checked); viewer?.setTableSlopeShown(slopeToggle.checked); };
document.addEventListener('spot-calibration', ev => {
  const has = Boolean(ev.detail && ev.detail.table);
  $('#toggleSlopeLabel').hidden = !has;
});
$('#toggleSlopeLabel').hidden = true;
$('#toggleOverlay').onchange = () => viewer?.setLayers({ sim: $('#toggleOverlay').checked && simulation.hasResult() });

// ---------------------------------------------------------------- start

renderTelemetryEmpty();
renderTargets();
act(loadState);
setInterval(() => { if (S.features && S.features.mode === 'local') return; if (S.stream.running) renderStream(); updateStale(); }, 1000);
connect();
