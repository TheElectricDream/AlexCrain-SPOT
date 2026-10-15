# hardware.js

**File:** `+spot/+console/webapp/static/hardware.js` · **Tab:** [[Lab Console: Hardware Tab|Lab-Console-Hardware-Tab]] · **Server side:** [[hardware.py|Console-hardware]]

<!-- source: +spot/+console/webapp/static/hardware.js -->

## What it is for

The Hardware tab of the lab console: one card per platform with its pucks, its eight thrusters on a top-view map, a thruster check, the last action and its SSH address. Every change goes to `/api/hardware/*` and comes back as a `hardware_platform` event, so all open pages stay in step.

## Function by function

**Lines 9–15.** The thrusters' positions on the top-view drawing (as in `resources/ThrusterConfig.png`: +x up, +y left) and the platforms' swatch colours.

**Lines 17–72.** `CARD_HTML`: a card's markup: name and pill, address, a detail line, the pucks row, the thruster map with the duty-cycle field, the thruster check with its progress bar, the last action, and the Address details (user @ host, Save).

**Lines 78–104.** `isOn` (anything on), `ready` (reachable, Teensy present, nobody else using it), `pill` (Checking… / Offline / No Access / No Teensy / In Use / Interactive or Calibrating (by the status's `owner`) / Thruster Check / Live / Ready) and `detail` (the explanation line: link lost, unreachable, no Teensy, in use by, flown by Interactive Mode, or driven by a calibration (Calibration tab); either way the controls are locked and Esc switches everything off).

**Lines 106–138.** `makeCard`: build the card, restore the duty and check-duration fields from storage, and wire the buttons: pucks on/off, each thruster on at the typed duty or off, Run Check / Cancel, Save address.

**Lines 140–208.** `render`: fill the card from a platform's status: the pill and detail, the pucks button and state (the body "floats" on the drawing), each thruster button's on state and tooltip (disabled while a check runs, Interactive Mode or a calibration drives it, or the platform isn't ready), the check button and progress bar (animated to the end of the current thruster over the time it has left) or the last check's result, the last action, the address fields (not while being edited), and the tab's live dot.

**Lines 210–222.** `setPlatform`/`setAll`/`allOff`.

**Lines 224–227.** **Esc** on the Hardware tab switches everything off on every platform (unless a dialog is open).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/hardware.js</b> (230 lines; click to show)</summary>

```javascript
   1  // Hardware tab: pucks, thrusters and thruster checks on the Chaser, Target and Obstacle (RED/BLACK/BLUE
   2  // to the API), through /api/hardware/*.
   3  // Every change comes back as a "hardware_platform" event, so all open pages stay in step.
   4  
   5  import { COLORS, platformName } from './platforms.js';
   6  
   7  const $ = (sel, el = document) => el.querySelector(sel);
   8  
   9  // Thruster positions on the top view (+x up, +y left), as in resources/ThrusterConfig.png:
  10  // [side the nozzle is on, x %, y %] of the drawing
  11  const THRUSTERS = {
  12    1: ['top', 32, 14], 2: ['top', 68, 14], 3: ['right', 86, 33], 4: ['right', 86, 67],
  13    5: ['bottom', 68, 86], 6: ['bottom', 32, 86], 7: ['left', 14, 67], 8: ['left', 14, 33],
  14  };
  15  const SWATCH = { RED: `background:${COLORS.RED}`, BLACK: `background:${COLORS.BLACK}`, BLUE: `background:${COLORS.BLUE}` };
  16  
  17  const CARD_HTML = `
  18    <div class="card-head">
  19      <h2 class="hw-name"><i class="swatch"></i><span data-role="name"></span></h2>
  20      <span class="pill" data-role="pill">Checking…</span>
  21    </div>
  22    <p class="hw-addr mono" data-role="addr"></p>
  23    <p class="hw-detail" data-role="detail" hidden></p>
  24  
  25    <div class="hw-section hw-row">
  26      <div>
  27        <span class="field-label">Pucks (Air Bearings)</span>
  28        <span class="hw-state" data-role="pucksState">Off</span>
  29      </div>
  30      <button type="button" class="btn" data-role="pucks">Turn On</button>
  31    </div>
  32  
  33    <div class="hw-section">
  34      <div class="hw-row">
  35        <span class="field-label">Thrusters <span class="muted">· click to switch on/off</span></span>
  36        <label class="input-suffix duty" title="Duty cycle used when you switch a thruster on">
  37          <input class="input" type="number" min="1" max="100" step="1" value="100" data-role="duty"><span>%</span>
  38        </label>
  39      </div>
  40      <div class="thruster-map">
  41        <div class="hw-body" data-role="body"><span class="hw-axis">+x ↑</span></div>
  42        ${Object.entries(THRUSTERS).map(([n, [side, x, y]]) =>
  43          `<button type="button" class="thruster" data-thruster="${n}" data-side="${side}" style="left:${x}%;top:${y}%">T${n}</button>`).join('')}
  44      </div>
  45    </div>
  46  
  47    <div class="hw-section">
  48      <div class="hw-row">
  49        <label class="field">
  50          <span class="field-label">Thruster Check</span>
  51          <span class="input-suffix wide"><input class="input" type="number" min="0.2" max="10" step="0.1" value="0.5" data-role="checkDur"><span>s each</span></span>
  52        </label>
  53        <button type="button" class="btn btn-primary" data-role="check">Run Check</button>
  54      </div>
  55      <div class="progress" data-role="progress" hidden><div></div></div>
  56      <p class="hint" data-role="checkNote"></p>
  57    </div>
  58  
  59    <div class="hw-foot">
  60      <span class="hint" data-role="lastAction"></span>
  61    </div>
  62  
  63    <details class="advanced">
  64      <summary>Address</summary>
  65      <div class="input-row">
  66        <input class="input mono" data-role="user" placeholder="user" spellcheck="false">
  67        <span class="muted">@</span>
  68        <input class="input mono" data-role="host" placeholder="IP Address" spellcheck="false">
  69        <button type="button" class="btn btn-sm" data-role="saveAddr">Save</button>
  70      </div>
  71      <p class="hint">Where this console logs in (with its own key, set up by setup_hardware_link.sh).</p>
  72    </details>`;
  73  
  74  export function initHardware({ api, act, toast, storage, fmtTime }) {
  75    const cards = {};
  76    const state = {};
  77  
  78    const isOn = p => p.pucks || p.duty.some(d => d > 0) || Boolean(p.check);
  79    const ready = p => p.probe.state === 'ok' && p.probe.teensy && !p.probe.others.length;
  80  
  81    function pill(p) {
  82      const pr = p.probe;
  83      if (pr.state === 'checking') return ['Checking…', ''];
  84      if (pr.state === 'offline') return ['Offline', 'err'];
  85      if (pr.state !== 'ok') return ['No Access', 'err'];
  86      if (!pr.teensy) return ['No Teensy', 'warn'];
  87      if (pr.others.length) return ['In Use', 'warn'];
  88      if (p.interactive) return [p.owner === 'Calibration' ? 'Calibrating' : 'Interactive', 'live'];
  89      if (p.check) return ['Thruster Check', 'live'];
  90      if (isOn(p)) return ['Live', 'live'];
  91      return ['Ready', 'ok'];
  92    }
  93  
  94    function detail(p) {
  95      const pr = p.probe;
  96      if (p.link_error) return [`Link lost: ${p.link_error}. Everything on the ${platformName(p.name)} was switched off.`, 'err'];
  97      if (pr.state !== 'ok' && pr.state !== 'checking') return [pr.detail, 'err'];
  98      if (pr.state === 'ok' && !pr.teensy) return ['No Teensy at /dev/ttyACM0. Check its USB cable.', 'warn'];
  99      if (pr.others.length) return [`In use by ${pr.others.join(', ')}. Controls unlock when it closes the port.`, 'warn'];
 100      if (p.interactive) return [p.owner === 'Calibration'
 101        ? 'Driven by a calibration (Calibration tab), so these controls are locked. Esc here switches everything off and stops it.'
 102        : 'Flown by Interactive Mode (PhaseSpace tab), so these controls are locked. Esc here switches everything off.', 'warn'];
 103      return [null, ''];
 104    }
 105  
 106    function makeCard(name) {
 107      const el = document.createElement('section');
 108      el.className = 'card hw-card';
 109      el.dataset.platform = name;
 110      el.innerHTML = CARD_HTML;
 111      $('[data-role=name]', el).textContent = platformName(name);
 112      $('.swatch', el).style.cssText = SWATCH[name] || '';
 113      const duty = $('[data-role=duty]', el), dur = $('[data-role=checkDur]', el);
 114      duty.value = storage.get(`spot.hw.${name}.duty`, 100);
 115      dur.value = storage.get(`spot.hw.${name}.checkDur`, 0.5);
 116      duty.onchange = () => storage.set(`spot.hw.${name}.duty`, Number(duty.value));
 117      dur.onchange = () => storage.set(`spot.hw.${name}.checkDur`, Number(dur.value));
 118  
 119      const send = (path, body) => act(async () => setPlatform(await api(path, { platform: name, ...body })));
 120      $('[data-role=pucks]', el).onclick = () => send('/api/hardware/pucks', { on: !state[name].pucks });
 121      for (const b of el.querySelectorAll('[data-thruster]')) {
 122        b.onclick = () => {
 123          const n = Number(b.dataset.thruster);
 124          const on = state[name].duty[n - 1] > 0;
 125          send('/api/hardware/thruster', { thruster: n, duty: on ? 0 : Number(duty.value) });
 126        };
 127      }
 128      $('[data-role=check]', el).onclick = () => state[name].check
 129        ? send('/api/hardware/check/cancel', {})
 130        : send('/api/hardware/check', { duration: Number(dur.value) });
 131      $('[data-role=saveAddr]', el).onclick = () => act(async () => {
 132        const cfg = { host: $('[data-role=host]', el).value.trim(), user: $('[data-role=user]', el).value.trim() };
 133        await api('/api/settings', { platforms: { [name]: cfg } });
 134        toast(`The ${platformName(name)} is now ${cfg.user}@${cfg.host}`, 'success', 4000);
 135      });
 136      $('#hwCards').append(el);
 137      return el;
 138    }
 139  
 140    function render(p) {
 141      const el = cards[p.name] || (cards[p.name] = makeCard(p.name));
 142      const ok = ready(p);
 143      const [pillText, pillKind] = pill(p);
 144      const pillEl = $('[data-role=pill]', el);
 145      pillEl.textContent = pillText;
 146      pillEl.className = `pill ${pillKind}`;
 147      $('[data-role=addr]', el).textContent = `${p.user}@${p.host}`;
 148      const [text, kind] = detail(p);
 149      const det = $('[data-role=detail]', el);
 150      det.hidden = !text;
 151      det.textContent = text || '';
 152      det.className = `hw-detail ${kind}`;
 153  
 154      const pb = $('[data-role=pucks]', el);
 155      pb.textContent = p.pucks ? 'Turn Off' : 'Turn On';
 156      pb.className = `btn ${p.pucks ? 'btn-stop' : 'btn-go'}`;
 157      pb.disabled = Boolean(p.check) || p.interactive || (!p.pucks && !ok);
 158      const ps = $('[data-role=pucksState]', el);
 159      ps.textContent = p.pucks ? 'Floating' : 'Off';
 160      ps.classList.toggle('on', p.pucks);
 161      $('[data-role=body]', el).classList.toggle('floating', p.pucks);
 162  
 163      for (const b of el.querySelectorAll('[data-thruster]')) {
 164        const d = p.duty[Number(b.dataset.thruster) - 1];
 165        b.classList.toggle('on', d > 0);
 166        b.disabled = Boolean(p.check) || p.interactive || (!(d > 0) && !ok);
 167        b.title = `Thruster ${b.dataset.thruster}: ${d > 0 ? `on at ${d} %` : 'off'}`;
 168      }
 169  
 170      const cb = $('[data-role=check]', el);
 171      const bar = $('[data-role=progress]', el);
 172      const note = $('[data-role=checkNote]', el);
 173      if (p.check) {
 174        const c = p.check;
 175        cb.textContent = 'Cancel';
 176        cb.className = 'btn btn-stop';
 177        cb.disabled = false;
 178        bar.hidden = false;
 179        // Fill to the end of the current thruster over the time it has left
 180        const fill = bar.firstElementChild;
 181        const left = Math.max(0, (c.index + 1) * c.duration - c.elapsed);
 182        fill.style.transition = 'none';
 183        fill.style.width = `${(c.index / c.total) * 100 + (1 - left / c.duration) * 100 / c.total}%`;
 184        void fill.offsetWidth;
 185        fill.style.transition = `width ${left}s linear`;
 186        fill.style.width = `${((c.index + 1) / c.total) * 100}%`;
 187        note.textContent = `Firing thruster ${c.index + 1} of ${c.total} (${c.duration} s each, pucks off)`;
 188      } else {
 189        cb.textContent = 'Run Check';
 190        cb.className = 'btn btn-primary';
 191        cb.disabled = !ok || p.interactive;
 192        bar.hidden = true;
 193        const lc = p.last_check;
 194        note.textContent = lc ? `${lc.ok ? '✓' : '✕'} Last check ${fmtTime(lc.finished)}: ${lc.note}`
 195                              : 'Fires T1 to T8 one at a time at 100 %, with the pucks off.';
 196        note.className = `hint ${lc ? (lc.ok ? 'ok' : 'bad') : ''}`;
 197      }
 198  
 199      const la = p.last_action;
 200      const lastEl = $('[data-role=lastAction]', el);
 201      lastEl.textContent = la ? `Last: ${la.text} · ${fmtTime(la.time)}${la.by ? ` · ${la.by}` : ''}` : '';
 202      lastEl.parentElement.hidden = !la;
 203      for (const key of ['host', 'user']) {
 204        const input = $(`[data-role=${key}]`, el);
 205        if (document.activeElement !== input) input.value = p[key];
 206      }
 207      $('#hwLiveDot').hidden = !Object.values(state).some(isOn);
 208    }
 209  
 210    function setPlatform(p) {
 211      state[p.name] = p;
 212      render(p);
 213    }
 214  
 215    function setAll(hw) {
 216      for (const p of hw.platforms) setPlatform(p);
 217    }
 218  
 219    async function allOff() {
 220      setAll(await api('/api/hardware/off', {}));
 221      toast('Switched everything off on every platform', 'success', 3000);
 222    }
 223  
 224    // Esc on the Hardware tab: everything off
 225    document.addEventListener('keydown', e => {
 226      if (e.key === 'Escape' && !$('#pageHardware').hidden && !document.querySelector('dialog[open]')) act(allOff);
 227    });
 228  
 229    return { setAll, setPlatform };
 230  }
```

</details>
<!-- /listing -->
