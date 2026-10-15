# designer.js

**File:** `+spot/+console/webapp/static/designer.js` · **Tab:** [[Spacecraft Designer|Designer-Tab]] · **Server side:** [[design.py|Console-design]]

<!-- source: +spot/+console/webapp/static/designer.js -->

## What it is for

The Spacecraft Designer (both consoles): what is mounted on each platform. The 3D view shows the chosen platform's CAD model close up; the sidebar picks its docking cone (none, male, female; on the Chaser also the arm) and its place (six: the +X, +Y and −Y faces × left/right), and whether it carries the solar panels (along X or Y). Each change is saved by this console's server, which every open page follows. The local console can send its design to the lab console or take the lab console's. On the local console the Chaser's arm and the simulation's Arm platform are **one choice**: picking the arm turns the Arm platform on (and sets `mass.arm.mount`); the Simulation tab's Arm switch picks or drops the arm here.

## Function by function

**Lines 14–23.** The platforms, the faces with their labels and which faces each one's edges meet, the arm's usual mount (+y, left), helpers.

**Lines 36–100.** `render`: the sidebar: the platform picker; the Docking Cone (or Arm) card with the type buttons, the place (the four faces drawn as seen from outside with two slots each, the back marked as the control panel; the arm coupler's place and blocked places highlighted), and hints (blocked by a part, on the arm's place, the arm's description, nothing chosen); the Solar Panels card (Include, Along X / Along Y, a warning when the bracket crosses the ZED2); the Lab Console card on the local console; and where the design is saved.

**Lines 102–121.** `labCardHtml`: the lab console's design compared with this one (Not Reachable / Needs Update / Same / Differs) and the Send / Use buttons.

**Lines 123–126.** `blockedBy`: the CAD's list of parts that stand out at a place.

**Lines 130–142.** `change(fn)`: apply a change locally, send the whole design to `/api/design`, and roll back if the server refuses.

**Lines 144–170.** Clicks: platform, cone type, place, solar axis, Send to / Use the lab console's; the solar checkbox.

**Lines 173–195.** The two-way link with the simulation: `syncArm` (the Chaser's arm choice switches the simulation's Arm platform and mount) and `followSettings` (the settings' Arm switch picks the arm here, or replaces it with the male cone when dropped; a changed initial pose redraws the arm).

**Lines 199–217.** `apply` (render, give the viewer the design, show the view) and `showView` (the viewer's designer mode for the chosen platform, with the arm at its initial pose from the settings, and the slot-click callback; the blocked places become known once the CAD is loaded).

**Lines 221–261.** From the server: `onDesign` (the design, its path and whether it is saved; the first time, follow the settings), `onState`, `onLab`/`onLabDesign` (the lab console's design for the card), `show`/`hide` (the viewer's layers off; leave designer mode).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/designer.js</b> (261 lines; click to show)</summary>

```javascript
   1  // Spacecraft Designer (both consoles): what is mounted on each platform. The 3D view shows the
   2  // chosen platform's CAD model close up; the sidebar picks its docking cone (none, male or female; on
   3  // the Chaser also the arm, whose end effector is the male cone, so it's one or the other) and
   4  // its place (one of 6: the +X, +Y and -Y faces x left/right, in the upper half, symmetric about each
   5  // face's vertical centre line; not the back, -X, where the control panel is), and whether it carries the pair of solar panels (off the +-X or the +-Y faces).
   6  // Each change is saved by this console's server (spot_console/design.py), which every open page
   7  // follows; the 3D views draw it whenever Use CAD is on. The local console can also send its design
   8  // to the lab console, or take the lab console's. On the local console, the Chaser's arm and the
   9  // simulation's Arm platform are the same choice: picking the arm turns it on (and sets its mount,
  10  // s.mass.arm.mount), and the Simulation tab's Arm switch picks or drops the arm here.
  11  
  12  import { platformName, COLORS } from './platforms.js';
  13  
  14  const NAMES = ['RED', 'BLACK', 'BLUE'];
  15  const FACES = [['+x', '+X Face', 'front'], ['+y', '+Y Face', ''], ['-x', '−X Face', 'back'], ['-y', '−Y Face', '']];
  16  // Seen from outside a face, which faces its left and right edges meet
  17  const EDGES = { '+x': ['−Y', '+Y'], '+y': ['+X', '−X'], '-x': ['+Y', '−Y'], '-y': ['−X', '+X'] };
  18  const ARM_MOUNT = { face: '+y', side: 'left' };   // the Chaser's arm coupler (info.mount.armMount)
  19  const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  20  const $ = s => document.querySelector(s);
  21  const same = (a, b) => Boolean(a && b) && a.face === b.face && a.side === b.side;
  22  const slotText = s => `${FACES.find(f => f[0] === s.face)[1]} · ${s.side === 'left' ? 'Left' : 'Right'}`;
  23  const clone = o => JSON.parse(JSON.stringify(o));
  24  
  25  export function initDesigner({ api, act, toast, storage, viewer, getSim = null, setArm = null }) {
  26    const S = {
  27      shown: false, local: false, platform: storage.get('spot.designer.platform', 'RED'),
  28      design: null, path: null, saved: false,         // this console's design (design.py)
  29      lab: null, labDesign: null,                      // local console: the lab console and its design
  30      blocked: null,                                   // {RED: {"face|side": [part names]}} from the CAD
  31    };
  32    const side = $('#dzSide');
  33  
  34    // ---------------------------------------------------------------- the sidebar
  35  
  36    function render() {
  37      if (!S.design) { side.innerHTML = '<p class="hint">Loading the design…</p>'; return; }
  38      const n = S.platform, p = S.design.platforms[n], dock = p.dock, solar = p.solar;
  39      const picker = NAMES.map(k => `<button type="button" data-dzplat="${k}" class="${k === n ? 'active' : ''}">
  40          <i class="swatch" style="background:${COLORS[k]}"></i>${platformName(k)}</button>`).join('');
  41      const typeBtn = (v, label, title) => `<button type="button" data-dztype="${v}" class="${dock.type === v ? 'active' : ''}" title="${title}">${label}</button>`;
  42      const isArm = dock.type === 'arm';
  43      const typeName = { none: 'None', male: 'Male', female: 'Female', arm: 'Arm' }[dock.type];
  44      const faces = FACES.map(([f, label, note]) => {
  45        if (f === '-x') {   // the back: the control panel is there, so no docking cone
  46          return `<div class="dz-face off" title="The control panel is on this face, so it takes no docking cone">
  47              <div class="dz-face-title">${label} <span class="muted">(back)</span></div>
  48              <div class="dz-face-body"><div class="dz-face-grid dz-face-none">Control Panel</div></div></div>`;
  49        }
  50        const slots = ['left', 'right'].map(sd => {
  51          const s = { face: f, side: sd };
  52          const chosen = same(s, dock), arm = n === 'RED' && same(s, ARM_MOUNT), blocked = blockedBy(n, s);
  53          const cls = ['dz-slot', chosen ? (dock.type === 'none' ? 'chosen-none' : 'chosen') : '', arm ? 'arm' : '',
  54                       blocked ? 'blocked' : ''].join(' ');
  55          const tip = `${slotText(s)}${arm ? ' (the arm\'s usual place, on the arm coupler)' : ''}` +
  56            `${blocked ? ` (blocked by ${blocked})` : ''}`;
  57          return `<button type="button" class="${cls}" data-slot="${f}|${sd}" title="${esc(tip)}" aria-label="${esc(tip)}"></button>`;
  58        }).join('');
  59        const [l, r] = EDGES[f];
  60        return `<div class="dz-face${dock.face === f ? ' on' : ''}">
  61            <div class="dz-face-title">${label}${note ? ` <span class="muted">(${note})</span>` : ''}</div>
  62            <div class="dz-face-body"><span class="dz-edge" title="This edge meets the ${l} face">${l}</span>
  63              <div class="dz-face-grid">${slots}</div><span class="dz-edge" title="This edge meets the ${r} face">${r}</span></div></div>`;
  64      }).join('');
  65      const onArm = n === 'RED' && (dock.type === 'male' || dock.type === 'female') && same(dock, ARM_MOUNT);
  66      const blockedHere = dock.type !== 'none' && blockedBy(n, dock);
  67      const solarOnZed = n === 'RED' && solar.on && solar.axis === 'x';
  68      const labCard = S.local ? labCardHtml() : '';
  69      side.innerHTML = `
  70        <div class="segmented dz-picker" role="group" aria-label="Platform">${picker}</div>
  71        <section class="card">
  72          <div class="card-head"><h2>${n === 'RED' ? 'Docking Cone or Arm' : 'Docking Cone'}</h2><span class="pill${dock.type === 'none' ? '' : ' ok'}">${typeName}</span></div>
  73          <div class="segmented dz-types" role="group" aria-label="Docking cone">
  74            ${typeBtn('none', 'None', 'No docking cone')}
  75            ${typeBtn('male', 'Male', 'The cone that goes in, on the interface bracket, with the steel disk at its tip')}
  76            ${typeBtn('female', 'Female', 'The cup that receives it, on an arm coupler\'s plate, with the electromagnet at its bottom')}
  77            ${n === 'RED' ? typeBtn('arm', 'Arm', 'The robotic arm, on an arm coupler\'s plate; its end effector is the male cone') : ''}
  78          </div>
  79          <div class="dz-where"><span class="field-label">${isArm ? 'Arm Mount' : 'Position'}</span><span class="mono">${slotText(dock)}</span></div>
  80          <div class="dz-faces">${faces}</div>
  81          <p class="hint">Each face as seen from outside, with the faces its edges meet. Click a place here or a disc on the model.
  82            <span class="dz-key"><i class="dz-key-arm"></i>Arm coupler</span> <span class="dz-key"><i class="dz-key-blocked"></i>Blocked</span></p>
  83          ${blockedHere ? `<p class="hint bad">This place is blocked by ${esc(blockedHere)}, which stands out of the face here.</p>` : ''}
  84          ${onArm ? '<p class="hint">On the arm\'s usual place (the arm coupler): the cone, on its own bracket, takes the coupler\'s place. Choose Arm to fly the arm instead.</p>' : ''}
  85          ${isArm ? `<p class="hint">The arm mounts on an arm coupler's plate (the female cup's bracket) at the place you pick, and its end effector is the male docking cone, so the Chaser carries no other cone. Its shoulder's 0° points straight out from this face.${S.local ? ' The simulation flies it (Simulation tab: the Arm platform, which follows this choice).' : ''}</p>` : ''}
  86          ${dock.type === 'none' ? `<p class="hint">Choose Male or Female to fit a cone at this place${n === 'RED' ? ', or Arm for the arm' : ''}.</p>` : ''}
  87        </section>
  88        <section class="card">
  89          <div class="card-head"><h2>Solar Panels</h2>
  90            <label class="toggle" title="The pair of panels on their bracket over the top"><input type="checkbox" data-dzsolar${solar.on ? ' checked' : ''}><span></span>Include</label></div>
  91          <div class="segmented dz-axis" role="group" aria-label="Solar panel position">
  92            <button type="button" data-dzaxis="x" class="${solar.axis === 'x' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +X and −X faces">Along X</button>
  93            <button type="button" data-dzaxis="y" class="${solar.axis === 'y' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +Y and −Y faces">Along Y</button>
  94          </div>
  95          <p class="hint">The bracket's top bar crosses the top; its legs run down the middle of the two faces the panels stand off, and the bend in one leg goes round the control panel (on −X). A 1 mm panel on each side of each frame: black cells on a white backsheet, as in the lab.</p>
  96          ${solarOnZed ? '<p class="hint warn-text">Along X, the bracket\'s leg crosses the front face, where the Chaser\'s ZED2 is.</p>' : ''}
  97        </section>
  98        ${labCard}
  99        <p class="hint dz-file" title="${esc(S.path || '')}">${S.saved ? 'Saved' : 'The defaults (saved at the first change)'}${S.path ? ` to ${esc(S.path.split(/[\\/]/).pop())}` : ''}; every page of this console follows it. The 3D views draw it with Use CAD on.</p>`;
 100    }
 101  
 102    function labCardHtml() {
 103      const lab = S.lab || {};
 104      let status, kind = '', canSend = false, canTake = false;
 105      if (!lab.reachable) status = 'Lab Console Not Reachable';
 106      else if (!S.labDesign) status = 'Lab Console Needs Update';
 107      else {
 108        const sameDesign = JSON.stringify(S.labDesign.platforms) === JSON.stringify(S.design.platforms);
 109        status = sameDesign ? 'Same as the Lab Console' : 'Differs from the Lab Console';
 110        kind = sameDesign ? 'ok' : 'warn';
 111        canSend = canTake = !sameDesign;
 112      }
 113      return `<section class="card">
 114          <div class="card-head"><h2>Lab Console</h2><span class="pill ${kind}">${status}</span></div>
 115          <p class="hint">The lab console (TX2) keeps its own design, which its PhaseSpace page draws.</p>
 116          <div class="dz-lab">
 117            <button type="button" class="btn btn-sm" data-dzlab="send" ${canSend ? '' : 'disabled'} title="Make the lab console draw this design">Send to Lab Console</button>
 118            <button type="button" class="btn btn-sm" data-dzlab="take" ${canTake ? '' : 'disabled'} title="Replace this project's design with the lab console's">Use Lab Console's Design</button>
 119          </div>
 120        </section>`;
 121    }
 122  
 123    function blockedBy(name, s) {
 124      const parts = S.blocked && S.blocked[name] && S.blocked[name][`${s.face}|${s.side}`];
 125      return parts && parts.length ? parts.join(' and ') : '';
 126    }
 127  
 128    // ---------------------------------------------------------------- changes
 129  
 130    async function change(fn, platform = S.platform) {
 131      const before = clone(S.design);
 132      fn(S.design.platforms[platform]);
 133      apply();
 134      try {
 135        const r = await api('/api/design', { design: S.design });
 136        onDesign(r);
 137      } catch (e) {
 138        S.design = before;
 139        apply();
 140        throw e;
 141      }
 142    }
 143  
 144    side.addEventListener('click', ev => act(async () => {
 145      const b = ev.target.closest('button');
 146      if (!b || b.disabled) return;
 147      if (b.dataset.dzplat) {
 148        S.platform = b.dataset.dzplat;
 149        storage.set('spot.designer.platform', S.platform);
 150        apply();
 151      } else if (b.dataset.dztype) {
 152        await change(p => { p.dock.type = b.dataset.dztype; });
 153        syncArm();
 154      } else if (b.dataset.slot) {
 155        const [face, sd] = b.dataset.slot.split('|');
 156        await change(p => { Object.assign(p.dock, { face, side: sd }); delete p.dock.level; });
 157        syncArm();
 158      } else if (b.dataset.dzaxis) {
 159        await change(p => { p.solar.axis = b.dataset.dzaxis; });
 160      } else if (b.dataset.dzlab === 'send') {
 161        await api('/api/lab', { path: 'design', body: { design: S.design } });
 162        toast('The lab console now draws this design.', 'success', 3500);
 163      } else if (b.dataset.dzlab === 'take') {
 164        onDesign(await api('/api/design', { design: S.labDesign }));
 165        toast('This project now uses the lab console\'s design.', 'success', 3500);
 166      }
 167    }));
 168    side.addEventListener('change', ev => act(async () => {
 169      if (ev.target.matches('[data-dzsolar]')) await change(p => { p.solar.on = ev.target.checked; });
 170    }));
 171  
 172    // The simulation's Arm platform and mount follow the Chaser's choice here (local console)
 173    function syncArm() {
 174      if (!setArm || !S.design) return;
 175      const dock = S.design.platforms.RED.dock, s = getSim && getSim().settings;
 176      if (!s) return;
 177      const mount = s.mass && s.mass.arm && s.mass.arm.mount;
 178      if (dock.type === 'arm' && (!s.platforms.ARM || !same(mount, dock))) setArm(true, { face: dock.face, side: dock.side });
 179      else if (dock.type !== 'arm' && s.platforms.ARM) setArm(false);
 180    }
 181  
 182    // ... and the other way: the Simulation tab's Arm switch (or settings loaded from a file) picks or
 183    // drops the arm here, at the settings' mount
 184    async function followSettings(s) {
 185      if (!setArm || !S.design || !s || !s.platforms) return;
 186      const dock = S.design.platforms.RED.dock, mount = (s.mass && s.mass.arm && s.mass.arm.mount) || ARM_MOUNT;
 187      if (s.platforms.ARM && (dock.type !== 'arm' || !same(dock, mount))) {
 188        await change(p => { Object.assign(p.dock, { type: 'arm', face: mount.face, side: mount.side }); }, 'RED');
 189      } else if (!s.platforms.ARM && dock.type === 'arm') {
 190        await change(p => { p.dock.type = 'male'; }, 'RED');   // the lab's usual Chaser: the male cone
 191      } else if (S.shown && S.platform === 'RED') {
 192        showView();   // e.g. the arm's initial pose changed
 193      }
 194    }
 195    document.addEventListener('spot-settings', ev => act(() => followSettings(ev.detail)));
 196  
 197    // ---------------------------------------------------------------- the 3D view
 198  
 199    function apply() {
 200      render();
 201      if (!S.design) return;
 202      viewer?.setDesign(S.design.platforms);
 203      if (S.shown) showView();
 204    }
 205  
 206    function showView() {
 207      const dock = S.design.platforms[S.platform].dock;
 208      const s = getSim && getSim().settings;
 209      const arm = s && s.initial && s.mass ? { q: s.initial.ARM.initial.map(a => a * Math.PI / 180), geometry: s.mass.arm } : null;
 210      viewer?.setDesigner({
 211        platform: S.platform, selected: dock.type === 'none' ? null : dock, arm,
 212        onSlot: slot => act(async () => { await change(p => Object.assign(p.dock, slot)); syncArm(); }),
 213      }).then(() => {
 214        // which places parts of the platform stand out at (known once the CAD models are loaded)
 215        if (!S.blocked && viewer?.cad?.lib) { S.blocked = viewer.cad.lib.info.blocked || {}; render(); }
 216      }).catch(e => toast('Could not load the CAD models: ' + e.message, 'error'));
 217    }
 218  
 219    // ---------------------------------------------------------------- from the server
 220  
 221    function onDesign(d) {
 222      if (!d || !d.design) return;
 223      const first = !S.design;
 224      S.design = d.design;
 225      S.path = d.path;
 226      S.saved = d.saved;
 227      apply();
 228      if (first && getSim) act(() => followSettings(getSim().settings));   // the settings came first
 229    }
 230  
 231    return {
 232      onState(st) {
 233        S.local = st.features && st.features.mode === 'local';
 234        if (S.local) {
 235          S.lab = st.lab || null;
 236          const snap = S.lab && S.lab.snapshot && S.lab.snapshot.design;
 237          S.labDesign = snap ? snap.design : null;
 238        }
 239        onDesign(st.design);
 240      },
 241      onDesign,
 242      onLab(l) {
 243        S.lab = l;
 244        if (l && !l.reachable) S.labDesign = null;
 245        else if (l && l.snapshot && l.snapshot.design) S.labDesign = l.snapshot.design.design;
 246        if (S.local) render();
 247      },
 248      onLabDesign(d) { S.labDesign = d && d.design; if (S.local) render(); },
 249      show() {
 250        S.shown = true;
 251        viewer?.setLayers({ live: false, sim: false, setup: false });
 252        viewer?.setLivePaths(false);
 253        if (S.design) showView();
 254      },
 255      hide() {
 256        if (!S.shown) return;
 257        S.shown = false;
 258        viewer?.setDesigner(null);
 259      },
 260    };
 261  }
```

</details>
<!-- /listing -->
