// The Thruster and LED Layout window (the Simulation tab's Platforms pane): a platform's top
// view, to scale, with every dimension from its centre of gravity: the eight thrusters (each
// one's line of action), the four PhaseSpace LEDs, and the CG the scales give. Beside it, the
// scales (settings: mass.<PLATFORM>.scales) and where each LED is (the project's calibration
// file, spot_calibration.json "markers", through /api/calibration/markers); the drawing follows
// as they are typed. MATLAB works out the same geometry for the simulation, and the marker file
// the Experiment tab sends the lab console (spot.massprops.centerOfGravity,
// spot.massprops.trackerPositions).

import { platformName, swatch } from './platforms.js';
import { constant, constantPlatform } from './constants.js';

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
const PLATFORMS = ['RED', 'BLACK', 'BLUE'];
const SCALE_TITLES = ['A · left middle edge', 'B · right back corner', 'C · right front corner'];

// Metres to mm, to a micrometre (so 0.15 m is 150, not 150.00000000000003).
const toMm = m => Math.round(m * 1e6) / 1e3;

/**
 * The platform's geometry [mm from the body centre] (spot.massprops.centerOfGravity,
 * spot.massprops.markerLayout), read when drawn since the constants arrive after this module
 * loads: HALF_SIDE, centre to edge (BODY_HALF_SIDE); THRUSTER_LINE, the thrusters' lines of
 * action from the centre (THRUSTER_ARM); SCALES, where the three scales hold the platform [x, y]
 * (SCALE_A, SCALE_B, SCALE_C); THRUSTERS (viewer.js's nozzles), each nozzle [x, y] on the body's
 * edge and the way the gas leaves it; LED_IDS, each platform's LED ids (LED_IDS_<PLATFORM>) in
 * the corner order LED_CORNERS, (+x,+y), (+x,-y), (-x,-y), (-x,+y); LED_DEFAULT, where they sit
 * by default (LED_DEFAULT_OFFSET); MAX_LED, as far as the console lets one be (LED_MAX_OFFSET).
 * @return {!Object} The geometry.
 */
function geom() {
  const HALF_SIDE = toMm(constant('BODY_HALF_SIDE'));
  const THRUSTER_LINE = toMm(constant('THRUSTER_ARM'));
  return {
    HALF_SIDE, THRUSTER_LINE,
    SCALES: ['SCALE_A', 'SCALE_B', 'SCALE_C'].map(name => constant(name).map(toMm)),
    THRUSTERS: [
      [[HALF_SIDE, THRUSTER_LINE], [1, 0]], [[HALF_SIDE, -THRUSTER_LINE], [1, 0]],
      [[THRUSTER_LINE, -HALF_SIDE], [0, -1]], [[-THRUSTER_LINE, -HALF_SIDE], [0, -1]],
      [[-HALF_SIDE, -THRUSTER_LINE], [-1, 0]], [[-HALF_SIDE, THRUSTER_LINE], [-1, 0]],
      [[-THRUSTER_LINE, HALF_SIDE], [0, 1]], [[THRUSTER_LINE, HALF_SIDE], [0, 1]],
    ],
    LED_IDS: Object.fromEntries(PLATFORMS.map(p => [p, constantPlatform('LED_IDS', p)])),
    LED_CORNERS: constant('LED_CORNERS'),
    LED_DEFAULT: toMm(constant('LED_DEFAULT_OFFSET')),
    MAX_LED: toMm(constant('LED_MAX_OFFSET')),
  };
}

// The drawing, in mm of the body: nozzle size, where the dimension rows go out from the body's
// edge, extension lines' gap and overshoot, label size; features closer than MERGE share a
// dimension, and one shorter than SHORT has its value outside.
const NOZZLE_W = 5;
const NOZZLE_L = 14;
const FIRST_ROW = 44;
const ROW_GAP = 22;
const EXT_GAP = 4;
const EXT_OVER = 5;
const FONT = 12;
const MERGE = 0.05;
const SHORT = 40;
const COLORS = { thruster: '#f5a524', led: '#3bc9db', cg: '#4c8dff', muted: '#8b93a3' };

const esc = s => String(s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
// A length to 0.1 mm (no "-0.0").
const mm = v => (Math.abs(v) < 0.05 ? 0 : v).toFixed(1);
const r1 = v => Math.round(v * 10) / 10;
// A point of the body frame [mm] on the drawing [u, v]: +x up, +y left (the Hardware tab's).
const toView = (x, y) => [-y, -x];

/**
 * The centre of gravity from the three scale readings, as spot.massprops.centerOfGravity
 * works it out.
 * @param {!Array<number>} scales The readings A, B, C [kg].
 * @return {?Array<number>} [x, y] [mm from the body centre], or null if they don't add up to
 *     more than 0 kg.
 */
export function centreOfGravity(scales) {
  const mass = scales.reduce((a, v) => a + v, 0);
  if (!(mass > 0)) {
    return null;
  }
  const { SCALES } = geom();
  return [0, 1].map(k => scales.reduce((a, v, i) => a + v * SCALES[i][k], 0) / mass);
}

/**
 * Each thruster's line of action from the CG, signed as the mixer takes it
 * (thruster_dist2CG_<PLATFORM>, spot.massprops.centerOfGravity).
 * @param {!Array<number>} cg [x, y] [mm from the body centre].
 * @return {!Array<number>} T1 to T8 [mm].
 */
export function thrusterArms([x, y]) {
  const e = geom().THRUSTER_LINE;
  return [e - y, -(e + y), e - x, -(e + x), e + y, -(e - y), e + x, -(e - x)];
}

// A platform's LEDs where they sit by default: [{id, x, y}] [mm from the body centre].
const defaultLeds = n => {
  const { LED_IDS, LED_CORNERS, LED_DEFAULT } = geom();
  return LED_IDS[n].map((id, k) =>
    ({ id, x: LED_CORNERS[k][0] * LED_DEFAULT, y: LED_CORNERS[k][1] * LED_DEFAULT }));
};

// The drawing

// A side's dimensions from the datum (the CG's coordinate along that side): features within
// MERGE of each other share one, those on the datum have none; on each side of the datum they
// stack in rows out from the body, the shortest nearest. Each keeps where its extension lines
// start (from: the features' coordinates across the side).
function placeDims(datum, features) {
  const groups = [];
  for (const f of features) {
    const g = groups.find(h => Math.abs(h.at - f.at) < MERGE);
    if (g) {
      g.from.push(f.from);
    } else {
      groups.push({ at: f.at, from: [f.from] });
    }
  }
  const placed = [];
  for (const sign of [-1, 1]) {
    groups.filter(g => Math.sign(g.at - datum) === sign && Math.abs(g.at - datum) >= MERGE)
        .sort((a, b) => Math.abs(a.at - datum) - Math.abs(b.at - datum))
        .forEach((g, row) => placed.push({ ...g, d: Math.abs(g.at - datum), row }));
  }
  return placed;
}

// How far out from the body centre a side's dimensions reach (its last row's extension lines).
function sideReach(placed) {
  const rows = placed.length ? Math.max(...placed.map(g => g.row)) : -1;
  const { HALF_SIDE } = geom();
  return rows < 0 ? HALF_SIDE + EXT_OVER : HALF_SIDE + FIRST_ROW + rows * ROW_GAP + EXT_OVER;
}

// A line on the drawing.
const line = ([u1, v1], [u2, v2], cls, extra = '') =>
  `<line class="${cls}" x1="${r1(u1)}" y1="${r1(v1)}" x2="${r1(u2)}" y2="${r1(v2)}"${extra}/>`;

// A text label, centred on [u, v] (turned to read upwards on a vertical dimension).
function label([u, v], text, cls, vertical = false) {
  const turn = vertical ? ` transform="rotate(-90 ${r1(u)} ${r1(v)})"` : '';
  return `<text class="${cls}" x="${r1(u)}" y="${r1(v)}"${turn}>${esc(text)}</text>`;
}

// A side's dimensions drawn: each feature's extension line out to its row, the dimension line
// from the datum with arrows at both ends, and its length (outside the end when short). The
// top and bottom measure along u, the left and right along v.
function drawSide(side, datum, placed, kind) {
  const out = side === 'top' || side === 'left' ? -1 : 1;
  const along = side === 'top' || side === 'bottom';
  const pt = (a, c) => (along ? [a, c] : [c, a]);
  const arrows = ` marker-start="url(#lyArrow-${kind})" marker-end="url(#lyArrow-${kind})"`;
  const { HALF_SIDE } = geom();
  return placed.map(g => {
    const row = out * (HALF_SIDE + FIRST_ROW + g.row * ROW_GAP);
    const start = out * Math.max(...g.from.map(f => out * f)) + out * EXT_GAP;
    const text = mm(g.d);
    const away = Math.sign(g.at - datum);
    const at = g.d < SHORT
      ? g.at + away * (6 + text.length * FONT * 0.32)
      : (g.at + datum) / 2;
    return line(pt(g.at, start), pt(g.at, row + out * EXT_OVER), `ly-ext ${kind}`) +
      line(pt(datum, row), pt(g.at, row), `ly-dim ${kind}`, arrows) +
      label(pt(at, row), text, `ly-val ${kind}`, !along);
  }).join('');
}

// The platform's top view: the body, the thrusters' lines of action, the CG's lines, every
// dimension, then the nozzles, LEDs, scales and CG on top.
function drawing(cg, leds) {
  const { HALF_SIDE, THRUSTER_LINE, THRUSTERS, SCALES } = geom();
  const [cu, cv] = toView(cg[0], cg[1]);
  const tip = HALF_SIDE + NOZZLE_L;
  const ledsView = leds.map(l => toView(l.x, l.y));

  // The dimensions: the thrusters' lines of action across the top (T1 and T6, T2 and T5) and
  // down the right (T3 and T8, T4 and T7); the LEDs along the bottom and the left.
  const dims = {
    top: placeDims(cu, [-THRUSTER_LINE, THRUSTER_LINE].map(at => ({ at, from: -tip }))),
    right: placeDims(cv, [-THRUSTER_LINE, THRUSTER_LINE].map(at => ({ at, from: tip }))),
    bottom: placeDims(cu, ledsView.map(([u, v]) => ({ at: u, from: v }))),
    left: placeDims(cv, ledsView.map(([u, v]) => ({ at: v, from: u }))),
  };
  const reach = Object.fromEntries(Object.entries(dims).map(([k, p]) => [k, sideReach(p)]));
  const half = Math.max(...Object.values(reach)) + FONT + 8;

  // The body, hatched as on the Hardware tab, and its centre.
  const body = `<rect class="ly-hull" x="${-HALF_SIDE}" y="${-HALF_SIDE}" ` +
    `width="${2 * HALF_SIDE}" height="${2 * HALF_SIDE}" rx="14"/>` +
    line([-6, 0], [6, 0], 'ly-centre') + line([0, -6], [0, 6], 'ly-centre');

  // Each pair of thrusters' line of action, across the body.
  const actions = [-THRUSTER_LINE, THRUSTER_LINE].map(c =>
    line([c, -HALF_SIDE], [c, HALF_SIDE], 'ly-action') +
    line([-HALF_SIDE, c], [HALF_SIDE, c], 'ly-action')).join('');

  // The CG's lines out to the dimensions it is measured from.
  const cgLines = line([cu, -reach.top], [cu, reach.bottom], 'ly-cgline') +
    line([-reach.left, cv], [reach.right, cv], 'ly-cgline');
  const dimensions = drawSide('top', cu, dims.top, 'thr') +
    drawSide('right', cv, dims.right, 'thr') +
    drawSide('bottom', cu, dims.bottom, 'led') + drawSide('left', cv, dims.left, 'led');

  // The nozzles, thin so the dimensions stay readable, each named beside it.
  const nozzles = THRUSTERS.map(([[x, y], [dx, dy]], k) => {
    const [u, v] = toView(x, y);
    const [du, dv] = toView(dx, dy);
    const w = du ? NOZZLE_L : NOZZLE_W;
    const h = du ? NOZZLE_W : NOZZLE_L;
    const ru = du ? (du > 0 ? u : u - NOZZLE_L) : u - NOZZLE_W / 2;
    const rv = du ? v - NOZZLE_W / 2 : (dv > 0 ? v : v - NOZZLE_L);
    const name = du
      ? [u + du * NOZZLE_L / 2, v + Math.sign(v) * 15]
      : [u + Math.sign(u) * 15, v + dv * NOZZLE_L / 2];
    return `<rect class="ly-nozzle" x="${r1(ru)}" y="${r1(rv)}" width="${w}" height="${h}" ` +
      'rx="1.5"/>' + label(name, `T${k + 1}`, 'ly-name');
  }).join('');

  // The LEDs, each with its id towards the body centre.
  const ledMarks = ledsView.map(([u, v], k) =>
    `<circle class="ly-led" cx="${r1(u)}" cy="${r1(v)}" r="5.5"/>` +
    `<circle class="ly-led-core" cx="${r1(u)}" cy="${r1(v)}" r="2"/>` +
    label([u - Math.sign(u || 1) * 15, v - Math.sign(v || 1) * 13], leds[k].id, 'ly-name'))
      .join('');

  // The scales the CG comes from, named outside the body.
  const scales = SCALES.map(([x, y], k) => {
    const [u, v] = toView(x, y);
    return `<rect class="ly-scale" x="${u - 4}" y="${v - 4}" width="8" height="8" ` +
      `transform="rotate(45 ${u} ${v})"/>` +
      label([u + Math.sign(u) * 14, v + Math.sign(v) * 14], 'ABC'[k], 'ly-scale-name');
  }).join('');

  // The CG: the usual quartered circle.
  const cgMark = `<circle class="ly-cg-ring" cx="${r1(cu)}" cy="${r1(cv)}" r="7"/>` +
    `<path class="ly-cg-fill" d="M${r1(cu)} ${r1(cv)}h7a7 7 0 0 0-7-7zM${r1(cu)} ${r1(cv)}` +
    'h-7a7 7 0 0 0 7 7z"/>' + label([cu + 15, cv - 13], 'CG', 'ly-cg-name');

  // Which way is which, in the bottom-left corner.
  const [au, av] = [-half + 30, half - 14];
  const axes = line([au, av], [au, av - 30], 'ly-axis', ' marker-end="url(#lyArrow-axis)"') +
    line([au, av], [au - 22, av], 'ly-axis', ' marker-end="url(#lyArrow-axis)"') +
    label([au + 12, av - 30], '+x', 'ly-axis-name') +
    label([au - 22, av - 10], '+y', 'ly-axis-name');

  const marker = (kind, color) => `<marker id="lyArrow-${kind}" viewBox="0 0 10 10" refX="9" ` +
    'refY="5" markerWidth="7" markerHeight="7" markerUnits="userSpaceOnUse" ' +
    `orient="auto-start-reverse"><path d="M0 1L10 5L0 9z" fill="${color}"/></marker>`;
  const defs = '<defs><pattern id="lyHatch" width="16" height="16" ' +
    'patternUnits="userSpaceOnUse" patternTransform="rotate(45)">' +
    '<rect width="16" height="16" fill="#161a21"/><rect width="8" height="16" fill="#181c24"/>' +
    `</pattern>${marker('thr', COLORS.thruster)}${marker('led', COLORS.led)}` +
    `${marker('axis', COLORS.muted)}</defs>`;
  return {
    viewBox: `${r1(-half)} ${r1(-half)} ${r1(2 * half)} ${r1(2 * half)}`,
    svg: defs + body + actions + cgLines + dimensions + nozzles + ledMarks + scales + cgMark +
      axes,
  };
}

// The window

/**
 * Sets up the Thruster and LED Layout window.
 * @param {{api: function(string, !Object): !Promise, act: function(function(): !Promise),
 *     getSettings: function(): ?Object, getMarkers: function(): ?Object,
 *     setSetting: function(string, *)}} deps The page's API call and its action wrapper
 *     (reports errors); the settings and the project's LEDs (its calibration's "markers")
 *     as they are now; and how to change a setting (the Simulation tab sends it to MATLAB).
 * @return {{open: function(string), refresh: function()}} Opens the window on a platform;
 *     shows the settings and LEDs as they are now (call when either changes).
 */
export function initLayout({ api, act, getSettings, getMarkers, setSetting }) {
  const dialog = $('#layoutDialog');
  const side = $('#layoutSide');
  const svg = $('#layoutSvg');
  // The platform shown, and LEDs typed but not saved yet (per platform).
  const S = { platform: 'RED', draft: {} };

  // The platform's scale readings, and its LEDs: as typed, else the project's, else the default.
  const scalesOf = n => {
    const m = ((getSettings() || {}).mass || {})[n];
    return m ? m.scales.map(Number) : null;
  };
  const ledsOf = n => S.draft[n] || ((getMarkers() || {})[n]) || defaultLeds(n);

  // The side panel for a platform: its picker, scales, LEDs and thrusters.
  function build() {
    const n = S.platform;
    const picker = PLATFORMS.map(p => `<button type="button" data-lpick="${p}" ` +
      `class="${p === n ? 'active' : ''}">${swatch(p)}${platformName(p)}</button>`).join('');
    const scales = [0, 1, 2].map(k => `<label class="sfield" title="${esc(SCALE_TITLES[k])}">
        <span class="sfield-label">${'ABC'[k]}</span>
        <span class="sfield-input"><input class="input" type="number" step="any" min="0"
          data-scale="${k}" aria-label="Scale ${'ABC'[k]}"><span class="unit">kg</span></span>
      </label>`).join('');
    const ledRows = geom().LED_IDS[n].map((id, k) => `<tr><th>${id}</th>
        ${['x', 'y'].map(a => `<td><input class="input" type="number" step="0.1"
          data-led="${k}" data-axis="${a}" aria-label="LED ${id} ${a}"></td>`).join('')}
        <td data-cgx="${k}"></td><td data-cgy="${k}"></td></tr>`).join('');
    const thrRow = ks => `<tr>${ks.map(k => `<th>T${k}</th>`).join('')}</tr>` +
      `<tr>${ks.map(k => `<td data-arm="${k - 1}"></td>`).join('')}</tr>`;
    side.innerHTML = `
      <div class="segmented plat-picker">${picker}</div>
      <section class="ly-sec">
        <span class="field-label tip" title="The platform on three scales: the CG is where their readings balance (the Mass Properties guide)">Scales</span>
        <div class="fgrid cols-3">${scales}</div>
        <p class="derived-row" data-lcg></p>
      </section>
      <section class="ly-sec">
        <div class="ly-sec-head">
          <span class="field-label tip" title="Where each PhaseSpace LED is on the platform's top, from the body centre (+x forward, +y left). Kept in this project's spot_calibration.json (markers).">LEDs from the Body Centre (mm)</span>
          <button type="button" class="btn btn-sm" data-lreset title="Put the LEDs back where they sit by default: ${toMm(constant('LED_INSET'))} mm in from each corner">Default Layout</button>
        </div>
        <table class="gain-table ly-table"><thead><tr><th>LED</th><th>x</th><th>y</th>
          <th class="tip" title="From the CG: what the lab console's marker file holds">x · CG</th><th class="tip" title="From the CG: what the lab console's marker file holds">y · CG</th></tr></thead>
          <tbody>${ledRows}</tbody></table>
        <p class="hint">Start Cameras (Experiment tab) sends the LEDs' positions from each platform's CG to the lab console, which PhaseSpace needs to report the poses of the CGs.</p>
      </section>
      <section class="ly-sec">
        <span class="field-label tip" title="The distance from the CG to each thruster's line of action, signed as the mixer takes it (thruster_dist2CG). The thrusters sit 81 mm in from the body's edges.">Thrusters · Line of Action from the CG (mm)</span>
        <table class="gain-table thr-table"><tbody>${thrRow([1, 2, 3, 4])}${thrRow([5, 6, 7, 8])}</tbody></table>
      </section>`;
    $('h2', dialog).innerHTML = `${swatch(n)}${platformName(n)} · Thruster and LED Layout`;
  }

  // The values (except one being typed), what follows from them, and the drawing.
  function refresh() {
    if (!dialog.open) {
      return;
    }
    const n = S.platform;
    const scales = scalesOf(n);
    const leds = ledsOf(n);
    const typing = document.activeElement;
    for (const el of $$('[data-scale]', side)) {
      if (el !== typing && scales) {
        el.value = scales[Number(el.dataset.scale)];
        el.classList.remove('invalid');
      }
    }
    for (const el of $$('[data-led]', side)) {
      if (el !== typing) {
        el.value = leds[Number(el.dataset.led)][el.dataset.axis];
        el.classList.remove('invalid');
      }
    }

    // The CG from the scales as typed (MATLAB's comes back the same once they are sent).
    const typed = $$('[data-scale]', side).map(el => Number(el.value));
    const cg = centreOfGravity(typed.every(v => isFinite(v) && v >= 0) ? typed : scales || []);
    const cgText = $('[data-lcg]', side);
    if (!cg) {
      cgText.innerHTML = '<span class="warn-text">The readings must add up to more than 0 kg.' +
        '</span>';
      return;
    }
    const mass = typed.reduce((a, v) => a + v, 0);
    cgText.innerHTML = `Mass <b>${mass.toFixed(2)} kg</b> · CG <b>${mm(cg[0])}, ${mm(cg[1])} ` +
      'mm</b> from the body centre';
    leds.forEach((l, k) => {
      $(`[data-cgx="${k}"]`, side).textContent = mm(l.x - cg[0]);
      $(`[data-cgy="${k}"]`, side).textContent = mm(l.y - cg[1]);
    });
    thrusterArms(cg).forEach((a, k) => {
      $(`[data-arm="${k}"]`, side).textContent = mm(a);
    });
    const d = drawing(cg, leds);
    svg.setAttribute('viewBox', d.viewBox);
    svg.innerHTML = d.svg;
  }

  // The LEDs as typed: [{id, x, y}], or null if a value isn't a plausible position.
  function typedLeds() {
    const n = S.platform;
    const leds = ledsOf(n).map(l => ({ ...l }));
    let ok = true;
    for (const el of $$('[data-led]', side)) {
      const v = el.value.trim() === '' ? NaN : Number(el.value);
      const good = isFinite(v) && Math.abs(v) <= geom().MAX_LED;
      el.classList.toggle('invalid', !good);
      ok = ok && good;
      if (good) {
        leds[Number(el.dataset.led)][el.dataset.axis] = v;
      }
    }
    return ok ? leds : null;
  }

  // Saves the platform's LEDs (null: the default layout) in the project's calibration; the
  // page hears the new calibration, and MATLAB works out the marker file again.
  function saveLeds(leds) {
    const n = S.platform;
    act(async () => {
      await api('/api/calibration/markers', { platform: n, leds });
      delete S.draft[n];
      refresh();
    });
  }

  // Typing: the drawing follows at once; a scale is sent to MATLAB shortly, an LED saved when
  // the field is left (or Enter).
  side.addEventListener('input', ev => {
    const el = ev.target;
    if (el.dataset.scale !== undefined) {
      const v = Number(el.value);
      const good = el.value.trim() !== '' && isFinite(v) && v >= 0;
      el.classList.toggle('invalid', !good);
      if (good) {
        setSetting(`mass.${S.platform}.scales.${el.dataset.scale}`, v);
      }
      refresh();
    } else if (el.dataset.led !== undefined) {
      const leds = typedLeds();
      if (leds) {
        S.draft[S.platform] = leds;
      }
      refresh();
    }
  });
  side.addEventListener('change', ev => {
    if (ev.target.dataset.led !== undefined) {
      const leds = typedLeds();
      if (leds) {
        saveLeds(leds);
      }
    }
  });
  side.addEventListener('keydown', ev => {
    if (ev.key === 'Enter' && ev.target.dataset.led !== undefined) {
      ev.preventDefault();
      ev.target.blur();
    }
  });
  side.addEventListener('click', ev => {
    const b = ev.target.closest('button');
    if (b && b.dataset.lpick) {
      S.platform = b.dataset.lpick;
      build();
      refresh();
    } else if (b && b.dataset.lreset !== undefined) {
      delete S.draft[S.platform];
      saveLeds(null);
    }
  });
  $('[data-lclose]', dialog).onclick = () => dialog.close();

  return {
    open(platform) {
      S.platform = PLATFORMS.includes(platform) ? platform : 'RED';
      build();
      dialog.showModal();
      refresh();
    },
    refresh,
  };
}
