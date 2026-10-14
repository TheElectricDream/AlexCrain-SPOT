// Spacecraft Designer (both consoles): what is mounted on each platform. The 3D view shows the
// chosen platform's CAD model close up; the sidebar picks its docking cone (none, male or female; on
// the Chaser also the arm, whose end effector is the male cone, so it's one or the other) and
// its place (one of 6: the +X, +Y and -Y faces x left/right, in the upper half, symmetric about each
// face's vertical centre line; not the back, -X, where the control panel is), and whether it carries the pair of solar panels (off the +-X or the +-Y faces).
// Each change is saved by this console's server (spot_console/design.py), which every open page
// follows; the 3D views draw it whenever Use CAD is on. The local console can also send its design
// to the lab console, or take the lab console's. On the local console, the Chaser's arm and the
// simulation's Arm platform are the same choice: picking the arm turns it on (and sets its mount,
// s.mass.arm.mount), and the Simulation tab's Arm switch picks or drops the arm here.

import { platformName, COLORS } from './platforms.js';

const NAMES = ['RED', 'BLACK', 'BLUE'];
const FACES = [['+x', '+X Face', 'front'], ['+y', '+Y Face', ''], ['-x', '−X Face', 'back'], ['-y', '−Y Face', '']];
// Seen from outside a face, which faces its left and right edges meet
const EDGES = { '+x': ['−Y', '+Y'], '+y': ['+X', '−X'], '-x': ['+Y', '−Y'], '-y': ['−X', '+X'] };
const ARM_MOUNT = { face: '+y', side: 'left' };   // the Chaser's arm coupler (info.mount.armMount)
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const $ = s => document.querySelector(s);
const same = (a, b) => Boolean(a && b) && a.face === b.face && a.side === b.side;
const slotText = s => `${FACES.find(f => f[0] === s.face)[1]} · ${s.side === 'left' ? 'Left' : 'Right'}`;
const clone = o => JSON.parse(JSON.stringify(o));

export function initDesigner({ api, act, toast, storage, viewer, getSim = null, setArm = null }) {
  const S = {
    shown: false, local: false, platform: storage.get('spot.designer.platform', 'RED'),
    design: null, path: null, saved: false,         // this console's design (design.py)
    lab: null, labDesign: null,                      // local console: the lab console and its design
    blocked: null,                                   // {RED: {"face|side": [part names]}} from the CAD
  };
  const side = $('#dzSide');

  // ---------------------------------------------------------------- the sidebar

  function render() {
    if (!S.design) { side.innerHTML = '<p class="hint">Loading the design…</p>'; return; }
    const n = S.platform, p = S.design.platforms[n], dock = p.dock, solar = p.solar;
    const picker = NAMES.map(k => `<button type="button" data-dzplat="${k}" class="${k === n ? 'active' : ''}">
        <i class="swatch" style="background:${COLORS[k]}"></i>${platformName(k)}</button>`).join('');
    const typeBtn = (v, label, title) => `<button type="button" data-dztype="${v}" class="${dock.type === v ? 'active' : ''}" title="${title}">${label}</button>`;
    const isArm = dock.type === 'arm';
    const typeName = { none: 'None', male: 'Male', female: 'Female', arm: 'Arm' }[dock.type];
    const faces = FACES.map(([f, label, note]) => {
      if (f === '-x') {   // the back: the control panel is there, so no docking cone
        return `<div class="dz-face off" title="The control panel is on this face, so it takes no docking cone">
            <div class="dz-face-title">${label} <span class="muted">(back)</span></div>
            <div class="dz-face-body"><div class="dz-face-grid dz-face-none">Control Panel</div></div></div>`;
      }
      const slots = ['left', 'right'].map(sd => {
        const s = { face: f, side: sd };
        const chosen = same(s, dock), arm = n === 'RED' && same(s, ARM_MOUNT), blocked = blockedBy(n, s);
        const cls = ['dz-slot', chosen ? (dock.type === 'none' ? 'chosen-none' : 'chosen') : '', arm ? 'arm' : '',
                     blocked ? 'blocked' : ''].join(' ');
        const tip = `${slotText(s)}${arm ? ' (the arm\'s usual place, on the arm coupler)' : ''}` +
          `${blocked ? ` (blocked by ${blocked})` : ''}`;
        return `<button type="button" class="${cls}" data-slot="${f}|${sd}" title="${esc(tip)}" aria-label="${esc(tip)}"></button>`;
      }).join('');
      const [l, r] = EDGES[f];
      return `<div class="dz-face${dock.face === f ? ' on' : ''}">
          <div class="dz-face-title">${label}${note ? ` <span class="muted">(${note})</span>` : ''}</div>
          <div class="dz-face-body"><span class="dz-edge" title="This edge meets the ${l} face">${l}</span>
            <div class="dz-face-grid">${slots}</div><span class="dz-edge" title="This edge meets the ${r} face">${r}</span></div></div>`;
    }).join('');
    const onArm = n === 'RED' && (dock.type === 'male' || dock.type === 'female') && same(dock, ARM_MOUNT);
    const blockedHere = dock.type !== 'none' && blockedBy(n, dock);
    const solarOnZed = n === 'RED' && solar.on && solar.axis === 'x';
    const labCard = S.local ? labCardHtml() : '';
    side.innerHTML = `
      <div class="segmented dz-picker" role="group" aria-label="Platform">${picker}</div>
      <section class="card">
        <div class="card-head"><h2>${n === 'RED' ? 'Docking Cone or Arm' : 'Docking Cone'}</h2><span class="pill${dock.type === 'none' ? '' : ' ok'}">${typeName}</span></div>
        <div class="segmented dz-types" role="group" aria-label="Docking cone">
          ${typeBtn('none', 'None', 'No docking cone')}
          ${typeBtn('male', 'Male', 'The cone that goes in, on the interface bracket, with the steel disk at its tip')}
          ${typeBtn('female', 'Female', 'The cup that receives it, on an arm coupler\'s plate, with the electromagnet at its bottom')}
          ${n === 'RED' ? typeBtn('arm', 'Arm', 'The robotic arm, on an arm coupler\'s plate; its end effector is the male cone') : ''}
        </div>
        <div class="dz-where"><span class="field-label">${isArm ? 'Arm Mount' : 'Position'}</span><span class="mono">${slotText(dock)}</span></div>
        <div class="dz-faces">${faces}</div>
        <p class="hint">Each face as seen from outside, with the faces its edges meet. Click a place here or a disc on the model.
          <span class="dz-key"><i class="dz-key-arm"></i>Arm coupler</span> <span class="dz-key"><i class="dz-key-blocked"></i>Blocked</span></p>
        ${blockedHere ? `<p class="hint bad">This place is blocked by ${esc(blockedHere)}, which stands out of the face here.</p>` : ''}
        ${onArm ? '<p class="hint">On the arm\'s usual place (the arm coupler): the cone, on its own bracket, takes the coupler\'s place. Choose Arm to fly the arm instead.</p>' : ''}
        ${isArm ? `<p class="hint">The arm mounts on an arm coupler's plate (the female cup's bracket) at the place you pick, and its end effector is the male docking cone, so the Chaser carries no other cone. Its shoulder's 0° points straight out from this face.${S.local ? ' The simulation flies it (Simulation tab: the Arm platform, which follows this choice).' : ''}</p>` : ''}
        ${dock.type === 'none' ? `<p class="hint">Choose Male or Female to fit a cone at this place${n === 'RED' ? ', or Arm for the arm' : ''}.</p>` : ''}
      </section>
      <section class="card">
        <div class="card-head"><h2>Solar Panels</h2>
          <label class="toggle" title="The pair of panels on their bracket over the top"><input type="checkbox" data-dzsolar${solar.on ? ' checked' : ''}><span></span>Include</label></div>
        <div class="segmented dz-axis" role="group" aria-label="Solar panel position">
          <button type="button" data-dzaxis="x" class="${solar.axis === 'x' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +X and −X faces">Along X</button>
          <button type="button" data-dzaxis="y" class="${solar.axis === 'y' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +Y and −Y faces">Along Y</button>
        </div>
        <p class="hint">The bracket's top bar crosses the top; its legs run down the middle of the two faces the panels stand off, and the bend in one leg goes round the control panel (on −X). A 1 mm panel on each side of each frame: black cells on a white backsheet, as in the lab.</p>
        ${solarOnZed ? '<p class="hint warn-text">Along X, the bracket\'s leg crosses the front face, where the Chaser\'s ZED2 is.</p>' : ''}
      </section>
      ${labCard}
      <p class="hint dz-file" title="${esc(S.path || '')}">${S.saved ? 'Saved' : 'The defaults (saved at the first change)'}${S.path ? ` to ${esc(S.path.split(/[\\/]/).pop())}` : ''}; every page of this console follows it. The 3D views draw it with Use CAD on.</p>`;
  }

  function labCardHtml() {
    const lab = S.lab || {};
    let status, kind = '', canSend = false, canTake = false;
    if (!lab.reachable) status = 'Lab Console Not Reachable';
    else if (!S.labDesign) status = 'Lab Console Needs Update';
    else {
      const sameDesign = JSON.stringify(S.labDesign.platforms) === JSON.stringify(S.design.platforms);
      status = sameDesign ? 'Same as the Lab Console' : 'Differs from the Lab Console';
      kind = sameDesign ? 'ok' : 'warn';
      canSend = canTake = !sameDesign;
    }
    return `<section class="card">
        <div class="card-head"><h2>Lab Console</h2><span class="pill ${kind}">${status}</span></div>
        <p class="hint">The lab console (TX2) keeps its own design, which its PhaseSpace page draws.</p>
        <div class="dz-lab">
          <button type="button" class="btn btn-sm" data-dzlab="send" ${canSend ? '' : 'disabled'} title="Make the lab console draw this design">Send to Lab Console</button>
          <button type="button" class="btn btn-sm" data-dzlab="take" ${canTake ? '' : 'disabled'} title="Replace this project's design with the lab console's">Use Lab Console's Design</button>
        </div>
      </section>`;
  }

  function blockedBy(name, s) {
    const parts = S.blocked && S.blocked[name] && S.blocked[name][`${s.face}|${s.side}`];
    return parts && parts.length ? parts.join(' and ') : '';
  }

  // ---------------------------------------------------------------- changes

  async function change(fn, platform = S.platform) {
    const before = clone(S.design);
    fn(S.design.platforms[platform]);
    apply();
    try {
      const r = await api('/api/design', { design: S.design });
      onDesign(r);
    } catch (e) {
      S.design = before;
      apply();
      throw e;
    }
  }

  side.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (!b || b.disabled) return;
    if (b.dataset.dzplat) {
      S.platform = b.dataset.dzplat;
      storage.set('spot.designer.platform', S.platform);
      apply();
    } else if (b.dataset.dztype) {
      await change(p => { p.dock.type = b.dataset.dztype; });
      syncArm();
    } else if (b.dataset.slot) {
      const [face, sd] = b.dataset.slot.split('|');
      await change(p => { Object.assign(p.dock, { face, side: sd }); delete p.dock.level; });
      syncArm();
    } else if (b.dataset.dzaxis) {
      await change(p => { p.solar.axis = b.dataset.dzaxis; });
    } else if (b.dataset.dzlab === 'send') {
      await api('/api/lab', { path: 'design', body: { design: S.design } });
      toast('The lab console now draws this design.', 'success', 3500);
    } else if (b.dataset.dzlab === 'take') {
      onDesign(await api('/api/design', { design: S.labDesign }));
      toast('This project now uses the lab console\'s design.', 'success', 3500);
    }
  }));
  side.addEventListener('change', ev => act(async () => {
    if (ev.target.matches('[data-dzsolar]')) await change(p => { p.solar.on = ev.target.checked; });
  }));

  // The simulation's Arm platform and mount follow the Chaser's choice here (local console)
  function syncArm() {
    if (!setArm || !S.design) return;
    const dock = S.design.platforms.RED.dock, s = getSim && getSim().settings;
    if (!s) return;
    const mount = s.mass && s.mass.arm && s.mass.arm.mount;
    if (dock.type === 'arm' && (!s.platforms.ARM || !same(mount, dock))) setArm(true, { face: dock.face, side: dock.side });
    else if (dock.type !== 'arm' && s.platforms.ARM) setArm(false);
  }

  // ... and the other way: the Simulation tab's Arm switch (or settings loaded from a file) picks or
  // drops the arm here, at the settings' mount
  async function followSettings(s) {
    if (!setArm || !S.design || !s || !s.platforms) return;
    const dock = S.design.platforms.RED.dock, mount = (s.mass && s.mass.arm && s.mass.arm.mount) || ARM_MOUNT;
    if (s.platforms.ARM && (dock.type !== 'arm' || !same(dock, mount))) {
      await change(p => { Object.assign(p.dock, { type: 'arm', face: mount.face, side: mount.side }); }, 'RED');
    } else if (!s.platforms.ARM && dock.type === 'arm') {
      await change(p => { p.dock.type = 'male'; }, 'RED');   // the lab's usual Chaser: the male cone
    } else if (S.shown && S.platform === 'RED') {
      showView();   // e.g. the arm's initial pose changed
    }
  }
  document.addEventListener('spot-settings', ev => act(() => followSettings(ev.detail)));

  // ---------------------------------------------------------------- the 3D view

  function apply() {
    render();
    if (!S.design) return;
    viewer?.setDesign(S.design.platforms);
    if (S.shown) showView();
  }

  function showView() {
    const dock = S.design.platforms[S.platform].dock;
    const s = getSim && getSim().settings;
    const arm = s && s.initial && s.mass ? { q: s.initial.ARM.initial.map(a => a * Math.PI / 180), geometry: s.mass.arm } : null;
    viewer?.setDesigner({
      platform: S.platform, selected: dock.type === 'none' ? null : dock, arm,
      onSlot: slot => act(async () => { await change(p => Object.assign(p.dock, slot)); syncArm(); }),
    }).then(() => {
      // which places parts of the platform stand out at (known once the CAD models are loaded)
      if (!S.blocked && viewer?.cad?.lib) { S.blocked = viewer.cad.lib.info.blocked || {}; render(); }
    }).catch(e => toast('Could not load the CAD models: ' + e.message, 'error'));
  }

  // ---------------------------------------------------------------- from the server

  function onDesign(d) {
    if (!d || !d.design) return;
    const first = !S.design;
    S.design = d.design;
    S.path = d.path;
    S.saved = d.saved;
    apply();
    if (first && getSim) act(() => followSettings(getSim().settings));   // the settings came first
  }

  return {
    onState(st) {
      S.local = st.features && st.features.mode === 'local';
      if (S.local) {
        S.lab = st.lab || null;
        const snap = S.lab && S.lab.snapshot && S.lab.snapshot.design;
        S.labDesign = snap ? snap.design : null;
      }
      onDesign(st.design);
    },
    onDesign,
    onLab(l) {
      S.lab = l;
      if (l && !l.reachable) S.labDesign = null;
      else if (l && l.snapshot && l.snapshot.design) S.labDesign = l.snapshot.design.design;
      if (S.local) render();
    },
    onLabDesign(d) { S.labDesign = d && d.design; if (S.local) render(); },
    show() {
      S.shown = true;
      viewer?.setLayers({ live: false, sim: false, setup: false });
      viewer?.setLivePaths(false);
      if (S.design) showView();
    },
    hide() {
      if (!S.shown) return;
      S.shown = false;
      viewer?.setDesigner(null);
    },
  };
}
