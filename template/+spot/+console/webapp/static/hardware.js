// Hardware tab: pucks, thrusters and thruster checks on the Chaser, Target and Obstacle (RED/BLACK/BLUE
// to the API), through /api/hardware/*.
// Every change comes back as a "hardware_platform" event, so all open pages stay in step.

import { COLORS, platformName } from './platforms.js';

const $ = (sel, el = document) => el.querySelector(sel);

// Thruster positions on the top view (+x up, +y left), as in resources/ThrusterConfig.png:
// [side the nozzle is on, x %, y %] of the drawing
const THRUSTERS = {
  1: ['top', 32, 14], 2: ['top', 68, 14], 3: ['right', 86, 33], 4: ['right', 86, 67],
  5: ['bottom', 68, 86], 6: ['bottom', 32, 86], 7: ['left', 14, 67], 8: ['left', 14, 33],
};
const SWATCH = { RED: `background:${COLORS.RED}`, BLACK: `background:${COLORS.BLACK}`, BLUE: `background:${COLORS.BLUE}` };

const CARD_HTML = `
  <div class="card-head">
    <h2 class="hw-name"><i class="swatch"></i><span data-role="name"></span></h2>
    <span class="pill" data-role="pill">Checking…</span>
  </div>
  <p class="hw-addr mono" data-role="addr"></p>
  <p class="hw-detail" data-role="detail" hidden></p>

  <div class="hw-section hw-row">
    <div>
      <span class="field-label">Pucks (Air Bearings)</span>
      <span class="hw-state" data-role="pucksState">Off</span>
    </div>
    <button type="button" class="btn" data-role="pucks">Turn On</button>
  </div>

  <div class="hw-section">
    <div class="hw-row">
      <span class="field-label">Thrusters <span class="muted">· click to switch on/off</span></span>
      <label class="input-suffix duty" title="Duty cycle used when you switch a thruster on">
        <input class="input" type="number" min="1" max="100" step="1" value="100" data-role="duty"><span>%</span>
      </label>
    </div>
    <div class="thruster-map">
      <div class="hw-body" data-role="body"><span class="hw-axis">+x ↑</span></div>
      ${Object.entries(THRUSTERS).map(([n, [side, x, y]]) =>
        `<button type="button" class="thruster" data-thruster="${n}" data-side="${side}" style="left:${x}%;top:${y}%">T${n}</button>`).join('')}
    </div>
  </div>

  <div class="hw-section">
    <div class="hw-row">
      <label class="field">
        <span class="field-label">Thruster Check</span>
        <span class="input-suffix wide"><input class="input" type="number" min="0.2" max="10" step="0.1" value="0.5" data-role="checkDur"><span>s each</span></span>
      </label>
      <button type="button" class="btn btn-primary" data-role="check">Run Check</button>
    </div>
    <div class="progress" data-role="progress" hidden><div></div></div>
    <p class="hint" data-role="checkNote"></p>
  </div>

  <div class="hw-foot">
    <span class="hint" data-role="lastAction"></span>
  </div>

  <details class="advanced">
    <summary>Address</summary>
    <div class="input-row">
      <input class="input mono" data-role="user" placeholder="user" spellcheck="false">
      <span class="muted">@</span>
      <input class="input mono" data-role="host" placeholder="IP Address" spellcheck="false">
      <button type="button" class="btn btn-sm" data-role="saveAddr">Save</button>
    </div>
    <p class="hint">Where this console logs in (with its own key, set up by setup_hardware_link.sh).</p>
  </details>`;

export function initHardware({ api, act, toast, storage, fmtTime }) {
  const cards = {};
  const state = {};

  const isOn = p => p.pucks || p.duty.some(d => d > 0) || Boolean(p.check);
  const ready = p => p.probe.state === 'ok' && p.probe.teensy && !p.probe.others.length;

  function pill(p) {
    const pr = p.probe;
    if (pr.state === 'checking') return ['Checking…', ''];
    if (pr.state === 'offline') return ['Offline', 'err'];
    if (pr.state !== 'ok') return ['No Access', 'err'];
    if (!pr.teensy) return ['No Teensy', 'warn'];
    if (pr.others.length) return ['In Use', 'warn'];
    if (p.interactive) return [p.owner === 'Calibration' ? 'Calibrating' : 'Interactive', 'live'];
    if (p.check) return ['Thruster Check', 'live'];
    if (isOn(p)) return ['Live', 'live'];
    return ['Ready', 'ok'];
  }

  function detail(p) {
    const pr = p.probe;
    if (p.link_error) return [`Link lost: ${p.link_error}. Everything on the ${platformName(p.name)} was switched off.`, 'err'];
    if (pr.state !== 'ok' && pr.state !== 'checking') return [pr.detail, 'err'];
    if (pr.state === 'ok' && !pr.teensy) return ['No Teensy at /dev/ttyACM0. Check its USB cable.', 'warn'];
    if (pr.others.length) return [`In use by ${pr.others.join(', ')}. Controls unlock when it closes the port.`, 'warn'];
    if (p.interactive) return [p.owner === 'Calibration'
      ? 'Driven by a calibration (Calibration tab), so these controls are locked. Esc here switches everything off and stops it.'
      : 'Flown by Interactive Mode (PhaseSpace tab), so these controls are locked. Esc here switches everything off.', 'warn'];
    return [null, ''];
  }

  function makeCard(name) {
    const el = document.createElement('section');
    el.className = 'card hw-card';
    el.dataset.platform = name;
    el.innerHTML = CARD_HTML;
    $('[data-role=name]', el).textContent = platformName(name);
    $('.swatch', el).style.cssText = SWATCH[name] || '';
    const duty = $('[data-role=duty]', el), dur = $('[data-role=checkDur]', el);
    duty.value = storage.get(`spot.hw.${name}.duty`, 100);
    dur.value = storage.get(`spot.hw.${name}.checkDur`, 0.5);
    duty.onchange = () => storage.set(`spot.hw.${name}.duty`, Number(duty.value));
    dur.onchange = () => storage.set(`spot.hw.${name}.checkDur`, Number(dur.value));

    const send = (path, body) => act(async () => setPlatform(await api(path, { platform: name, ...body })));
    $('[data-role=pucks]', el).onclick = () => send('/api/hardware/pucks', { on: !state[name].pucks });
    for (const b of el.querySelectorAll('[data-thruster]')) {
      b.onclick = () => {
        const n = Number(b.dataset.thruster);
        const on = state[name].duty[n - 1] > 0;
        send('/api/hardware/thruster', { thruster: n, duty: on ? 0 : Number(duty.value) });
      };
    }
    $('[data-role=check]', el).onclick = () => state[name].check
      ? send('/api/hardware/check/cancel', {})
      : send('/api/hardware/check', { duration: Number(dur.value) });
    $('[data-role=saveAddr]', el).onclick = () => act(async () => {
      const cfg = { host: $('[data-role=host]', el).value.trim(), user: $('[data-role=user]', el).value.trim() };
      await api('/api/settings', { platforms: { [name]: cfg } });
      toast(`The ${platformName(name)} is now ${cfg.user}@${cfg.host}`, 'success', 4000);
    });
    $('#hwCards').append(el);
    return el;
  }

  function render(p) {
    const el = cards[p.name] || (cards[p.name] = makeCard(p.name));
    const ok = ready(p);
    const [pillText, pillKind] = pill(p);
    const pillEl = $('[data-role=pill]', el);
    pillEl.textContent = pillText;
    pillEl.className = `pill ${pillKind}`;
    $('[data-role=addr]', el).textContent = `${p.user}@${p.host}`;
    const [text, kind] = detail(p);
    const det = $('[data-role=detail]', el);
    det.hidden = !text;
    det.textContent = text || '';
    det.className = `hw-detail ${kind}`;

    const pb = $('[data-role=pucks]', el);
    pb.textContent = p.pucks ? 'Turn Off' : 'Turn On';
    pb.className = `btn ${p.pucks ? 'btn-stop' : 'btn-go'}`;
    pb.disabled = Boolean(p.check) || p.interactive || (!p.pucks && !ok);
    const ps = $('[data-role=pucksState]', el);
    ps.textContent = p.pucks ? 'Floating' : 'Off';
    ps.classList.toggle('on', p.pucks);
    $('[data-role=body]', el).classList.toggle('floating', p.pucks);

    for (const b of el.querySelectorAll('[data-thruster]')) {
      const d = p.duty[Number(b.dataset.thruster) - 1];
      b.classList.toggle('on', d > 0);
      b.disabled = Boolean(p.check) || p.interactive || (!(d > 0) && !ok);
      b.title = `Thruster ${b.dataset.thruster}: ${d > 0 ? `on at ${d} %` : 'off'}`;
    }

    const cb = $('[data-role=check]', el);
    const bar = $('[data-role=progress]', el);
    const note = $('[data-role=checkNote]', el);
    if (p.check) {
      const c = p.check;
      cb.textContent = 'Cancel';
      cb.className = 'btn btn-stop';
      cb.disabled = false;
      bar.hidden = false;
      // Fill to the end of the current thruster over the time it has left
      const fill = bar.firstElementChild;
      const left = Math.max(0, (c.index + 1) * c.duration - c.elapsed);
      fill.style.transition = 'none';
      fill.style.width = `${(c.index / c.total) * 100 + (1 - left / c.duration) * 100 / c.total}%`;
      void fill.offsetWidth;
      fill.style.transition = `width ${left}s linear`;
      fill.style.width = `${((c.index + 1) / c.total) * 100}%`;
      note.textContent = `Firing thruster ${c.index + 1} of ${c.total} (${c.duration} s each, pucks off)`;
    } else {
      cb.textContent = 'Run Check';
      cb.className = 'btn btn-primary';
      cb.disabled = !ok || p.interactive;
      bar.hidden = true;
      const lc = p.last_check;
      note.textContent = lc ? `${lc.ok ? '✓' : '✕'} Last check ${fmtTime(lc.finished)}: ${lc.note}`
                            : 'Fires T1 to T8 one at a time at 100 %, with the pucks off.';
      note.className = `hint ${lc ? (lc.ok ? 'ok' : 'bad') : ''}`;
    }

    const la = p.last_action;
    const lastEl = $('[data-role=lastAction]', el);
    lastEl.textContent = la ? `Last: ${la.text} · ${fmtTime(la.time)}${la.by ? ` · ${la.by}` : ''}` : '';
    lastEl.parentElement.hidden = !la;
    for (const key of ['host', 'user']) {
      const input = $(`[data-role=${key}]`, el);
      if (document.activeElement !== input) input.value = p[key];
    }
    $('#hwLiveDot').hidden = !Object.values(state).some(isOn);
  }

  function setPlatform(p) {
    state[p.name] = p;
    render(p);
  }

  function setAll(hw) {
    for (const p of hw.platforms) setPlatform(p);
  }

  async function allOff() {
    setAll(await api('/api/hardware/off', {}));
    toast('Switched everything off on every platform', 'success', 3000);
  }

  // Esc on the Hardware tab: everything off
  document.addEventListener('keydown', e => {
    if (e.key === 'Escape' && !$('#pageHardware').hidden && !document.querySelector('dialog[open]')) act(allOff);
  });

  return { setAll, setPlatform };
}
