// Spacecraft Designer (both consoles): what is mounted on each platform. The 3D view shows the
// chosen platform's CAD model close up; the sidebar picks:
//   - its docking cone (none, male or female; on the Chaser also the arm, whose end effector is
//     the male cone, so it's one or the other);
//   - its place (one of 6: the +X, +Y and -Y faces x left/right, in the upper half, symmetric
//     about each face's vertical centre line; not the back, -X, where the control panel is);
//   - whether it carries the pair of solar panels (off the +-X or the +-Y faces).
// Each change is saved by this console's server (spot_console/design.py), which every open page
// follows; the 3D views draw it whenever Use CAD is on. The local console can also send its
// design to the lab console, or take the lab console's. On the local console, the Chaser's arm
// and the simulation's Arm platform are the same choice: picking the arm turns it on (and sets
// its mount, s.mass.arm.mount), and the Simulation tab's Arm switch picks or drops the arm here.
// A run flown with the arm is replayed without it while the Chaser here has none; the page asks
// whether to show it anyway (Show the Arm, Not Now, or Never Ask Again on this browser).

import { platformName, COLORS } from './platforms.js';

const NAMES = ['RED', 'BLACK', 'BLUE'];

// Each face: [key, title, note].
const FACES = [
  ['+x', '+X Face', 'front'], ['+y', '+Y Face', ''], ['-x', '−X Face', 'back'],
  ['-y', '−Y Face', ''],
];

// Seen from outside a face, which faces its left and right edges meet.
const EDGES = { '+x': ['−Y', '+Y'], '+y': ['+X', '−X'], '-x': ['+Y', '−Y'], '-y': ['−X', '+X'] };
const ARM_MOUNT = { face: '+y', side: 'left' };   // the Chaser's arm coupler (info.mount.armMount)
const ARM_NEVER_ASK = 'spot.designer.armNeverAsk';   // never ask to show a run's arm (storage)
const DOCK_NAMES = {
  none: 'no docking cone', male: 'the male docking cone', female: 'the female cup',
};
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const $ = s => document.querySelector(s);

// Whether two places are the same; a place as text ("+Y Face · Left").
const same = (a, b) => Boolean(a && b) && a.face === b.face && a.side === b.side;
const slotText = s =>
  `${FACES.find(f => f[0] === s.face)[1]} · ${s.side === 'left' ? 'Left' : 'Right'}`;
const clone = o => JSON.parse(JSON.stringify(o));

/**
 * Sets up the Spacecraft Designer.
 * @param {{api: function(string, !Object): !Promise, act: function(function(): !Promise),
 *     toast: function(string, string, number),
 *     storage: {get: function(string, *): *, set: function(string, *)}, viewer: ?Object,
 *     getSim: ?function(): !Object, setArm: ?function(boolean, !Object=)}} deps The page's
 *     API call, its action wrapper (reports errors), its toast, its settings storage, the 3D
 *     view, and (local console) the simulation's state and its Arm switch.
 * @return {!Object} Handlers for the page's events: onState, onDesign, onLab, onLabDesign,
 *     show, hide.
 */
export function initDesigner({ api, act, toast, storage, viewer, getSim = null, setArm = null }) {
  const S = {
    shown: false, local: false, platform: storage.get('spot.designer.platform', 'RED'),
    design: null, path: null, saved: false,   // this console's design (design.py)
    lab: null, labDesign: null,   // local console: the lab console and its design
    blocked: null,   // {RED: {"face|side": [part names]}} from the CAD
  };
  const side = $('#dzSide');

  // The sidebar

  // The chosen platform's docking cone (or arm), its place and its solar panels.
  function render() {
    if (!S.design) {
      side.innerHTML = '<p class="hint">Loading the design…</p>';
      return;
    }
    const n = S.platform;
    const p = S.design.platforms[n];
    const dock = p.dock;
    const solar = p.solar;

    // The platform picker and the cone type buttons.
    const picker = NAMES.map(k => `<button type="button" data-dzplat="${k}" class="${k === n ? 'active' : ''}">
        <i class="swatch" style="background:${COLORS[k]}"></i>${platformName(k)}</button>`).join('');
    const typeBtn = (v, label, title) => `<button type="button" data-dztype="${v}" ` +
      `class="${dock.type === v ? 'active' : ''}" title="${title}">${label}</button>`;
    const isArm = dock.type === 'arm';
    const typeName = { none: 'None', male: 'Male', female: 'Female', arm: 'Arm' }[dock.type];

    // Each face with its two places; the back has the control panel, so no docking cone.
    const faces = FACES.map(([f, label, note]) => {
      if (f === '-x') {
        return `<div class="dz-face off" title="The control panel is on this face, so it takes no docking cone">
            <div class="dz-face-title">${label} <span class="muted">(back)</span></div>
            <div class="dz-face-body"><div class="dz-face-grid dz-face-none">Control Panel</div></div></div>`;
      }
      const slots = ['left', 'right'].map(sd => {
        const s = { face: f, side: sd };
        const chosen = same(s, dock);
        const blocked = blockedBy(n, s);
        const cls = [
          'dz-slot', chosen ? (dock.type === 'none' ? 'chosen-none' : 'chosen') : '',
          blocked ? 'blocked' : '',
        ].join(' ');
        const tip = `${slotText(s)}${blocked ? ` (blocked by ${blocked})` : ''}`;
        return `<button type="button" class="${cls}" data-slot="${f}|${sd}" ` +
          `title="${esc(tip)}" aria-label="${esc(tip)}"></button>`;
      }).join('');
      const [l, r] = EDGES[f];
      return `<div class="dz-face${dock.face === f ? ' on' : ''}">
          <div class="dz-face-title">${label}${note ? ` <span class="muted">(${note})</span>` : ''}</div>
          <div class="dz-face-body"><span class="dz-edge" title="This edge meets the ${l} face">${l}</span>
            <div class="dz-face-grid">${slots}</div><span class="dz-edge" title="This edge meets the ${r} face">${r}</span></div></div>`;
    }).join('');

    // Notes: a blocked place, a leg across the ZED2.
    const blockedHere = dock.type !== 'none' && blockedBy(n, dock);
    const solarOnZed = n === 'RED' && solar.on && solar.axis === 'x';
    const labCard = S.local ? labCardHtml() : '';

    // The heading's tooltip.
    const simNote = S.local
      ? ' The simulation flies it (Simulation tab: the Arm platform, which follows this choice).'
      : '';
    const dockTip = isArm
      ? 'The arm mounts on an arm coupler\'s plate (the female cup\'s bracket) at the place you ' +
        'pick, and its end effector is the male docking cone, so the Chaser carries no other ' +
        `cone. Its shoulder's 0° points straight out from this face.${simNote}`
      : `Choose None, Male or Female${n === 'RED' ? ', or Arm for the arm' : ''}, then its place.`;
    side.innerHTML = `
      <div class="segmented dz-picker" role="group" aria-label="Platform">${picker}</div>
      <section class="card">
        <div class="card-head"><h2 class="tip" title="${esc(dockTip)}">${n === 'RED' ? 'Docking Cone or Arm' : 'Docking Cone'}</h2><span class="pill${dock.type === 'none' ? '' : ' ok'}">${typeName}</span></div>
        <div class="segmented dz-types" role="group" aria-label="Docking cone">
          ${typeBtn('none', 'None', 'No docking cone')}
          ${typeBtn('male', 'Male', 'The cone that goes in, on the interface bracket, with the steel disk at its tip')}
          ${typeBtn('female', 'Female', 'The cup that receives it, on an arm coupler\'s plate, with the electromagnet at its bottom')}
          ${n === 'RED' ? typeBtn('arm', 'Arm', 'The robotic arm, on an arm coupler\'s plate; its end effector is the male cone') : ''}
        </div>
        <div class="dz-where"><span class="field-label tip" title="Each face as seen from outside, with the faces its edges meet. Click a place here or a disc on the model.">${isArm ? 'Arm Mount' : 'Position'}</span><span class="mono">${slotText(dock)}</span></div>
        <div class="dz-faces">${faces}</div>
        ${blockedHere ? `<p class="hint bad" title="${esc(blockedHere)} stands out of the face here">Blocked by ${esc(blockedHere)}.</p>` : ''}
        ${n === 'RED' && !isArm && storage.get(ARM_NEVER_ASK, false) ? `<p class="hint dz-armask">Runs flown with the arm are replayed without it, without asking.</p><button type="button" class="btn btn-sm" data-dzarmask title="Ask again whether to show a run's arm while the Chaser has none">Ask Again</button>` : ''}
      </section>
      <section class="card">
        <div class="card-head"><h2 class="tip" title="The bracket's top bar crosses the top; its legs run down the middle of the two faces the panels stand off, and the bend in one leg goes round the control panel (on −X). A 1 mm panel on each side of each frame: black cells on a white backsheet, as in the lab.">Solar Panels</h2>
          <label class="toggle" title="The pair of panels on their bracket over the top"><input type="checkbox" data-dzsolar${solar.on ? ' checked' : ''}><span></span>Include</label></div>
        <div class="segmented dz-axis" role="group" aria-label="Solar panel position">
          <button type="button" data-dzaxis="x" class="${solar.axis === 'x' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +X and −X faces">Along X</button>
          <button type="button" data-dzaxis="y" class="${solar.axis === 'y' ? 'active' : ''}" ${solar.on ? '' : 'disabled'} title="The panels off the +Y and −Y faces">Along Y</button>
        </div>
        ${solarOnZed ? '<p class="hint warn-text">Along X, a leg crosses the ZED2\'s face.</p>' : ''}
      </section>
      ${labCard}
      <p class="hint dz-file" title="${esc(S.path || '')}: every page of this console follows it, and the 3D views draw it with Use CAD on.">${S.saved ? 'Saved' : 'Defaults (saved at the first change)'}${S.path ? ` · ${esc(S.path.split(/[\\/]/).pop())}` : ''}</p>`;
  }

  // Local console: whether the lab console's design is the same, with Send and Use buttons.
  function labCardHtml() {
    const lab = S.lab || {};
    let status;
    let kind = '';
    let canSend = false;
    let canTake = false;
    if (!lab.reachable) {
      status = 'Lab Console Not Reachable';
    } else if (!S.labDesign) {
      status = 'Lab Console Needs Update';
    } else {
      const sameDesign =
        JSON.stringify(S.labDesign.platforms) === JSON.stringify(S.design.platforms);
      status = sameDesign ? 'Same as the Lab Console' : 'Differs from the Lab Console';
      kind = sameDesign ? 'ok' : 'warn';
      canSend = canTake = !sameDesign;
    }
    return `<section class="card">
        <div class="card-head"><h2 class="tip" title="The lab console (TX2) keeps its own design, which its PhaseSpace page draws.">Lab Console</h2><span class="pill ${kind}">${status}</span></div>
        <div class="dz-lab">
          <button type="button" class="btn btn-sm" data-dzlab="send" ${canSend ? '' : 'disabled'} title="Make the lab console draw this design">Send to Lab Console</button>
          <button type="button" class="btn btn-sm" data-dzlab="take" ${canTake ? '' : 'disabled'} title="Replace this project's design with the lab console's">Use Lab Console's Design</button>
        </div>
      </section>`;
  }

  // The parts that stand out at a place ('' if none).
  function blockedBy(name, s) {
    const parts = S.blocked && S.blocked[name] && S.blocked[name][`${s.face}|${s.side}`];
    return parts && parts.length ? parts.join(' and ') : '';
  }

  // Changes

  // Changes the design here at once and saves it; puts it back if the save fails.
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

  // The sidebar's buttons: platform, cone type, place, panel axis, Send and Use.
  side.addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('button');
    if (!b || b.disabled) {
      return;
    }
    if (b.dataset.dzplat) {
      S.platform = b.dataset.dzplat;
      storage.set('spot.designer.platform', S.platform);
      apply();
    } else if (b.dataset.dztype) {
      await change(p => {
        p.dock.type = b.dataset.dztype;
      });
      syncArm();
    } else if (b.dataset.slot) {
      const [face, sd] = b.dataset.slot.split('|');
      await change(p => {
        Object.assign(p.dock, { face, side: sd });
        delete p.dock.level;
      });
      syncArm();
    } else if (b.dataset.dzaxis) {
      await change(p => {
        p.solar.axis = b.dataset.dzaxis;
      });
    } else if ('dzarmask' in b.dataset) {
      storage.set(ARM_NEVER_ASK, false);
      render();
      viewer?.askArmsAgain();
    } else if (b.dataset.dzlab === 'send') {
      await api('/api/lab', { path: 'design', body: { design: S.design } });
      toast('The lab console now draws this design.', 'success', 3500);
    } else if (b.dataset.dzlab === 'take') {
      onDesign(await api('/api/design', { design: S.labDesign }));
      toast('This project now uses the lab console\'s design.', 'success', 3500);
    }
  }));

  // The solar panels' Include switch.
  side.addEventListener('change', ev => act(async () => {
    if (ev.target.matches('[data-dzsolar]')) {
      await change(p => {
        p.solar.on = ev.target.checked;
      });
    }
  }));

  // The simulation's Arm platform and mount follow the Chaser's choice here (local console).
  function syncArm() {
    if (!setArm || !S.design) {
      return;
    }
    const dock = S.design.platforms.RED.dock;
    const s = getSim && getSim().settings;
    if (!s) {
      return;
    }
    const mount = s.mass && s.mass.arm && s.mass.arm.mount;
    if (dock.type === 'arm' && (!s.platforms.ARM || !same(mount, dock))) {
      setArm(true, { face: dock.face, side: dock.side });
    } else if (dock.type !== 'arm' && s.platforms.ARM) {
      setArm(false);
    }
  }

  // ... and the other way: the Simulation tab's Arm switch (or settings loaded from a file) picks
  // or drops the arm here, at the settings' mount.
  async function followSettings(s) {
    if (!setArm || !S.design || !s || !s.platforms) {
      return;
    }
    const dock = S.design.platforms.RED.dock;
    const mount = (s.mass && s.mass.arm && s.mass.arm.mount) || ARM_MOUNT;
    if (s.platforms.ARM && (dock.type !== 'arm' || !same(dock, mount))) {
      await change(p => {
        Object.assign(p.dock, { type: 'arm', face: mount.face, side: mount.side });
      }, 'RED');
    } else if (!s.platforms.ARM && dock.type === 'arm') {
      // The lab's usual Chaser: the male cone.
      await change(p => {
        p.dock.type = 'male';
      }, 'RED');
    } else if (S.shown && S.platform === 'RED') {
      showView();   // e.g. the arm's initial pose changed
    }
  }
  document.addEventListener('spot-settings', ev => act(() => followSettings(ev.detail)));

  // A run's arm while the Chaser has none

  // The 3D view hides a replayed arm while the Chaser here has none, and says so once per run in
  // view: ask whether that was intended and the arm should show anyway. Runs that arrive while
  // the prompt is open join it.
  const armDialog = $('#armHiddenDialog');
  const armAsk = { labels: [], shows: [] };
  if (viewer && armDialog) {
    viewer.armChoice.onHidden = (labels, show) => {
      if (storage.get(ARM_NEVER_ASK, false)) {
        return;
      }
      armAsk.labels.push(...labels.map(l => l || 'The last simulation'));
      armAsk.shows.push(show);
      const dock = S.design ? S.design.platforms.RED.dock.type : 'none';
      $('#armHiddenPart').textContent = DOCK_NAMES[dock] || dock;
      $('#armHiddenList').innerHTML = armAsk.labels.map(l => `<li><b>${esc(l)}</b></li>`).join('');
      if (!armDialog.open) {
        armDialog.returnValue = '';
        armDialog.showModal();
      }
    };

    // Show the Arm, Not Now (also the close button and Escape), or Never Ask Again.
    armDialog.addEventListener('close', () => {
      const shows = armAsk.shows;
      armAsk.labels = [];
      armAsk.shows = [];
      if (armDialog.returnValue === 'show') {
        shows.forEach(show => show());
      } else if (armDialog.returnValue === 'never') {
        storage.set(ARM_NEVER_ASK, true);
        render();
        toast('Runs flown with the arm are replayed without it while the Chaser has none. ' +
          'The Designer\'s Chaser card can turn asking back on.', 'info', 6000);
      }
    });
  }

  // The 3D view

  // Shows the design: the sidebar, every 3D view, and (shown) the close-up.
  function apply() {
    render();
    if (!S.design) {
      return;
    }
    viewer?.setDesign(S.design.platforms);
    if (S.shown) {
      showView();
    }
  }

  // The chosen platform close up, with its arm (if any) at the simulation's initial pose;
  // clicking a place on the model picks it.
  function showView() {
    const dock = S.design.platforms[S.platform].dock;
    const s = getSim && getSim().settings;
    const arm = s && s.initial && s.mass
      ? { q: s.initial.ARM.initial.map(a => a * Math.PI / 180), geometry: s.mass.arm }
      : null;
    viewer?.setDesigner({
      platform: S.platform, selected: dock.type === 'none' ? null : dock, arm,
      onSlot: slot => act(async () => {
        await change(p => Object.assign(p.dock, slot));
        syncArm();
      }),
    }).then(() => {
      // Which places parts of the platform stand out at (known once the CAD models are loaded).
      if (!S.blocked && viewer?.cad?.lib) {
        S.blocked = viewer.cad.lib.info.blocked || {};
        render();
      }
    }).catch(e => toast('Could not load the CAD models: ' + e.message, 'error'));
  }

  // From the server

  // This console's design; the first time, the simulation's settings (which came first) are
  // applied to it.
  function onDesign(d) {
    if (!d || !d.design) {
      return;
    }
    const first = !S.design;
    S.design = d.design;
    S.path = d.path;
    S.saved = d.saved;
    apply();
    if (first && getSim) {
      act(() => followSettings(getSim().settings));
    }
  }

  return {
    // The page's state: local console?, this design and (local) the lab console's.
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
    // The lab link changed.
    onLab(l) {
      S.lab = l;
      if (l && !l.reachable) {
        S.labDesign = null;
      } else if (l && l.snapshot && l.snapshot.design) {
        S.labDesign = l.snapshot.design.design;
      }
      if (S.local) {
        render();
      }
    },
    // The lab console's design changed.
    onLabDesign(d) {
      S.labDesign = d && d.design;
      if (S.local) {
        render();
      }
    },
    // The tab is shown: the 3D view shows only the close-up.
    show() {
      S.shown = true;
      viewer?.setLayers({ live: false, sim: false, setup: false });
      viewer?.setLivePaths(false);
      if (S.design) {
        showView();
      }
    },
    // The tab is left.
    hide() {
      if (!S.shown) {
        return;
      }
      S.shown = false;
      viewer?.setDesigner(null);
    },
  };
}
