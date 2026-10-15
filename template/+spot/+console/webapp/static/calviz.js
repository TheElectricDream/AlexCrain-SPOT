// The Calibration tab's pictures (calibration.js), and the table slope overlay on the 3D view
// (viewer.js): the jet colour map, the table's height surface (spot_console/calfit.py), and a
// top-down drawing of the table with the height map, its contour lines, downhill arrows, the drift
// paths and the platform.

export const TABLE = { x: 3.5, y: 2.4 };
export const G = 9.80665;
const DEFAULT_CENTRE = [1.75, 1.2];

/**
 * The jet colour map: dark blue through cyan, yellow to dark red.
 * @param {number} t In [0, 1] (clamped).
 * @return {!Array<number>} [r, g, b] (0-255).
 */
export function jet(t) {
  t = Math.min(1, Math.max(0, t));
  const c = x => Math.round(255 * Math.min(1, Math.max(0, x)));
  return [
    c(1.5 - Math.abs(4 * t - 3)), c(1.5 - Math.abs(4 * t - 2)), c(1.5 - Math.abs(4 * t - 1)),
  ];
}
export const jetCss = t => `rgb(${jet(t).join(',')})`;

/**
 * Height of the table top (calfit.height).
 * @param {!Array<number>} a Height coefficients [a1 .. a5].
 * @param {number} x [m]
 * @param {number} y [m]
 * @param {!Array<number>=} centre The fit's centre [m].
 * @return {number} [m]
 */
export function height(a, x, y, centre = DEFAULT_CENTRE) {
  const u = x - centre[0];
  const v = y - centre[1];
  return a[0] * u + a[1] * v + a[2] * u * u + a[3] * u * v + a[4] * v * v;
}

/**
 * The table's slope (the floating platform accelerates at -g times it).
 * @param {!Array<number>} a Height coefficients [a1 .. a5].
 * @param {number} x [m]
 * @param {number} y [m]
 * @param {!Array<number>=} centre The fit's centre [m].
 * @return {!Array<number>} [dh/dx, dh/dy].
 */
export function gradient(a, x, y, centre = DEFAULT_CENTRE) {
  const u = x - centre[0];
  const v = y - centre[1];
  return [a[0] + 2 * a[2] * u + a[3] * v, a[1] + a[3] * u + 2 * a[4] * v];
}

/**
 * Heights on an NX x NY grid over the table, with the lowest at 0.
 * @param {!Object} cal The table calibration ({height, centre}).
 * @param {number=} nx
 * @param {number=} ny
 * @return {{z: !Float64Array, nx: number, ny: number, range: number}} Heights [um] (row j at
 *     y = (j + 0.5) / ny of the table) and their range.
 */
export function heightGrid(cal, nx = 140, ny = 96) {
  const a = cal.height;
  const centre = cal.centre || DEFAULT_CENTRE;
  const z = new Float64Array(nx * ny);
  let lo = Infinity;
  let hi = -Infinity;
  for (let j = 0; j < ny; j++) {
    for (let i = 0; i < nx; i++) {
      const h = 1e6 * height(a, (i + 0.5) * TABLE.x / nx, (j + 0.5) * TABLE.y / ny, centre);
      z[j * nx + i] = h;
      if (h < lo) {
        lo = h;
      }
      if (h > hi) {
        hi = h;
      }
    }
  }
  for (let k = 0; k < z.length; k++) {
    z[k] -= lo;
  }
  return { z, nx, ny, range: Math.max(hi - lo, 1e-9) };
}

/**
 * A canvas of the height map alone (jet, low = blue), for the 3D view's overlay: the canvas's x
 * runs along the table's x, its rows from y = 2.4 (top) to 0.
 * @param {!Object} cal The table calibration.
 * @param {number=} nx
 * @param {number=} ny
 * @return {!HTMLCanvasElement}
 */
export function heightCanvas(cal, nx = 350, ny = 240) {
  const g = heightGrid(cal, nx, ny);
  const cv = document.createElement('canvas');
  cv.width = nx;
  cv.height = ny;
  const ctx = cv.getContext('2d');
  const img = ctx.createImageData(nx, ny);
  for (let j = 0; j < ny; j++) {
    for (let i = 0; i < nx; i++) {
      const [r, gg, b] = jet(g.z[j * nx + i] / g.range);
      const k = ((ny - 1 - j) * nx + i) * 4;
      img.data[k] = r;
      img.data[k + 1] = gg;
      img.data[k + 2] = b;
      img.data[k + 3] = 255;
    }
  }
  ctx.putImageData(img, 0, 0);
  contours(ctx, g, (i, j) => [i * nx / g.nx, ny - j * ny / g.ny], 'rgba(255,255,255,0.45)', 1);
  return cv;
}

// Contour lines of a height grid (marching squares), at 8 levels; MAP(i, j) puts grid point
// (i, j) (cell centres at i + 0.5) on the canvas.
function contours(ctx, g, map, color, width) {
  const { z, nx, ny, range } = g;
  ctx.strokeStyle = color;
  ctx.lineWidth = width;
  ctx.beginPath();
  for (let l = 1; l < 8; l++) {
    const level = range * l / 8;
    for (let j = 0; j < ny - 1; j++) {
      for (let i = 0; i < nx - 1; i++) {
        // Where the level crosses each edge of the cell; joined in pairs.
        const v = [z[j * nx + i], z[j * nx + i + 1], z[(j + 1) * nx + i + 1], z[(j + 1) * nx + i]];
        const corners = [[i, j], [i + 1, j], [i + 1, j + 1], [i, j + 1]];
        const pts = [];
        for (let e = 0; e < 4; e++) {
          const a = v[e];
          const b = v[(e + 1) % 4];
          if ((a < level) !== (b < level)) {
            const t = (level - a) / (b - a);
            const [ia, ja] = corners[e];
            const [ib, jb] = corners[(e + 1) % 4];
            pts.push(map(ia + t * (ib - ia) + 0.5, ja + t * (jb - ja) + 0.5));
          }
        }
        for (let p = 0; p + 1 < pts.length; p += 2) {
          ctx.moveTo(pts[p][0], pts[p][1]);
          ctx.lineTo(pts[p + 1][0], pts[p + 1][1]);
        }
      }
    }
  }
  ctx.stroke();
}

/**
 * Draws the table seen from above into CANVAS (sized to its box, for the screen's pixel ratio).
 * @param {!HTMLCanvasElement} canvas
 * @param {{cal: ?Object, paths: !Array, live: ?Object, points: ?Array, current: ?number,
 *     color: string}=} opts
 *     cal: the table calibration ({height, centre, ...}) for the height map, or null;
 *     paths: [{points: [[x, y], ...], cut}] finished drifts;
 *     live: {pose: [x, y, thetaDeg], path: [[x, y]...], goal: [x, y, thetaDeg]} while a run
 *     goes on;
 *     points: [[x, y], ...] a grid of start points; current: the index of the one under way;
 *     color: the platform's colour.
 * @return {?{X: function(number): number, Y: function(number): number, s: number}} The table to
 *     canvas mapping (null if the canvas has no size).
 */
export function drawTable(canvas, {
  cal = null, paths = [], live = null, points = null, current = null, color = '#aeb5bd',
} = {}) {
  // The canvas at the screen's resolution.
  const dpr = window.devicePixelRatio || 1;
  const w = canvas.clientWidth;
  const h = canvas.clientHeight;
  if (!w || !h) {
    return null;
  }
  if (canvas.width !== Math.round(w * dpr) || canvas.height !== Math.round(h * dpr)) {
    canvas.width = Math.round(w * dpr);
    canvas.height = Math.round(h * dpr);
  }
  const ctx = canvas.getContext('2d');
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
  ctx.clearRect(0, 0, w, h);

  // The table, centred in the canvas inside margins for the ticks [px].
  const pad = { l: 34, r: 26, t: 12, b: 28 };
  const s = Math.min((w - pad.l - pad.r) / TABLE.x, (h - pad.t - pad.b) / TABLE.y);
  const ox = pad.l + ((w - pad.l - pad.r) - s * TABLE.x) / 2;
  const oy = pad.t + ((h - pad.t - pad.b) - s * TABLE.y) / 2;
  const X = x => ox + x * s;
  const Y = y => oy + (TABLE.y - y) * s;

  // The table: the height map, or plain granite grey.
  if (cal && cal.height) {
    const g = heightGrid(cal, 175, 120);
    const img = ctx.createImageData(g.nx, g.ny);
    for (let j = 0; j < g.ny; j++) {
      for (let i = 0; i < g.nx; i++) {
        const [r, gg, b] = jet(g.z[j * g.nx + i] / g.range);
        const k = ((g.ny - 1 - j) * g.nx + i) * 4;
        img.data[k] = r;
        img.data[k + 1] = gg;
        img.data[k + 2] = b;
        img.data[k + 3] = 255;
      }
    }
    const off = document.createElement('canvas');
    off.width = g.nx;
    off.height = g.ny;
    off.getContext('2d').putImageData(img, 0, 0);
    ctx.imageSmoothingEnabled = true;
    ctx.globalAlpha = 0.92;
    ctx.drawImage(off, X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
    ctx.globalAlpha = 1;
    contours(ctx, g, (i, j) => [X(i * TABLE.x / g.nx), Y(j * TABLE.y / g.ny)],
      'rgba(255,255,255,0.5)', 1);

    // Downhill arrows on a 7 x 5 grid, their length growing with the slope.
    const centre = cal.centre || DEFAULT_CENTRE;
    let max = 1e-12;
    const grid = [];
    for (let i = 0; i < 7; i++) {
      for (let j = 0; j < 5; j++) {
        const x = (i + 0.5) * TABLE.x / 7;
        const y = (j + 0.5) * TABLE.y / 5;
        const gr = gradient(cal.height, x, y, centre);
        grid.push([x, y, gr]);
        max = Math.max(max, Math.hypot(gr[0], gr[1]));
      }
    }
    ctx.strokeStyle = 'rgba(10,12,16,0.85)';
    ctx.fillStyle = 'rgba(10,12,16,0.85)';
    ctx.lineWidth = 1.6;
    for (const [x, y, gr] of grid) {
      const m = Math.hypot(gr[0], gr[1]);
      if (m < 1e-12) {
        continue;
      }
      const len = 0.16 * s * (0.35 + 0.65 * m / max);
      const ux = -gr[0] / m;
      const uy = -gr[1] / m;
      const x0 = X(x) - ux * len / 2;
      const y0 = Y(y) + uy * len / 2;
      const x1 = X(x) + ux * len / 2;
      const y1 = Y(y) - uy * len / 2;
      ctx.beginPath();
      ctx.moveTo(x0, y0);
      ctx.lineTo(x1, y1);
      ctx.stroke();
      const a = Math.atan2(y1 - y0, x1 - x0);
      ctx.beginPath();
      ctx.moveTo(x1, y1);
      ctx.lineTo(x1 - 7 * Math.cos(a - 0.45), y1 - 7 * Math.sin(a - 0.45));
      ctx.lineTo(x1 - 7 * Math.cos(a + 0.45), y1 - 7 * Math.sin(a + 0.45));
      ctx.closePath();
      ctx.fill();
    }
  } else {
    ctx.fillStyle = '#2a2d33';
    ctx.fillRect(X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
  }

  // 0.5 m grid, outline and ticks.
  ctx.strokeStyle = 'rgba(255,255,255,0.12)';
  ctx.lineWidth = 1;
  ctx.beginPath();
  for (let x = 0.5; x < TABLE.x; x += 0.5) {
    ctx.moveTo(X(x), Y(0));
    ctx.lineTo(X(x), Y(TABLE.y));
  }
  for (let y = 0.5; y < TABLE.y; y += 0.5) {
    ctx.moveTo(X(0), Y(y));
    ctx.lineTo(X(TABLE.x), Y(y));
  }
  ctx.stroke();
  ctx.strokeStyle = 'rgba(255,255,255,0.5)';
  ctx.strokeRect(X(0), Y(TABLE.y), s * TABLE.x, s * TABLE.y);
  ctx.fillStyle = '#8a93a3';
  ctx.font = '11px system-ui, sans-serif';
  ctx.textAlign = 'center';
  ctx.textBaseline = 'top';
  for (let x = 0; x <= TABLE.x + 1e-6; x += 0.5) {
    ctx.fillText(x >= TABLE.x - 1e-6 ? `${x.toFixed(1)} m` : x.toFixed(1), X(x), Y(0) + 5);
  }
  ctx.textAlign = 'right';
  ctx.textBaseline = 'middle';
  for (let y = 0; y <= TABLE.y + 1e-6; y += 0.5) {
    ctx.fillText(y.toFixed(1), X(0) - 5, Y(y));
  }

  // The drifts: each a line over a dark outline, from a dot; a red cross where the pucks went
  // off.
  const line = (pts, style, width, dash = []) => {
    if (!pts || pts.length < 2) {
      return;
    }
    ctx.setLineDash(dash);
    ctx.strokeStyle = 'rgba(0,0,0,0.6)';
    ctx.lineWidth = width + 2.5;
    ctx.beginPath();
    pts.forEach(([x, y], k) => (k ? ctx.lineTo(X(x), Y(y)) : ctx.moveTo(X(x), Y(y))));
    ctx.stroke();
    ctx.strokeStyle = style;
    ctx.lineWidth = width;
    ctx.beginPath();
    pts.forEach(([x, y], k) => (k ? ctx.lineTo(X(x), Y(y)) : ctx.moveTo(X(x), Y(y))));
    ctx.stroke();
    ctx.setLineDash([]);
  };
  for (const p of paths) {
    line(p.points, '#ffffff', 2);
    const [x, y] = p.points[0];
    ctx.fillStyle = '#ffffff';
    ctx.beginPath();
    ctx.arc(X(x), Y(y), 3.5, 0, 2 * Math.PI);
    ctx.fill();
    if (p.cut) {
      const [xe, ye] = p.points[p.points.length - 1];
      ctx.strokeStyle = '#ff6b6b';
      ctx.lineWidth = 2;
      ctx.beginPath();
      ctx.moveTo(X(xe) - 5, Y(ye) - 5);
      ctx.lineTo(X(xe) + 5, Y(ye) + 5);
      ctx.moveTo(X(xe) + 5, Y(ye) - 5);
      ctx.lineTo(X(xe) - 5, Y(ye) + 5);
      ctx.stroke();
    }
  }

  // The grid of start points, numbered; the current one in yellow.
  if (points) {
    points.forEach(([x, y], k) => {
      const now = k === current;
      ctx.strokeStyle = now ? '#ffd43b' : 'rgba(255,255,255,0.7)';
      ctx.lineWidth = now ? 2.5 : 1.5;
      ctx.setLineDash(now ? [] : [4, 3]);
      ctx.beginPath();
      ctx.arc(X(x), Y(y), 9, 0, 2 * Math.PI);
      ctx.stroke();
      ctx.setLineDash([]);
      ctx.fillStyle = now ? '#ffd43b' : 'rgba(255,255,255,0.85)';
      ctx.font = '600 11px system-ui, sans-serif';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(String(k + 1), X(x), Y(y) - 18);
    });
  }

  // The run under way: the path so far, the goal (a dashed circle, and a dashed line to it)
  // and the platform.
  if (live) {
    line(live.path, '#ffd43b', 2.5);
    if (live.goal) {
      ctx.strokeStyle = '#ffd43b';
      ctx.lineWidth = 1.5;
      ctx.setLineDash([5, 4]);
      ctx.beginPath();
      ctx.arc(X(live.goal[0]), Y(live.goal[1]), 0.15 * s, 0, 2 * Math.PI);
      ctx.stroke();
      if (live.pose) {
        ctx.beginPath();
        ctx.moveTo(X(live.pose[0]), Y(live.pose[1]));
        ctx.lineTo(X(live.goal[0]), Y(live.goal[1]));
        ctx.stroke();
      }
      ctx.setLineDash([]);
    }
    if (live.pose) {
      platform(ctx, X(live.pose[0]), Y(live.pose[1]), live.pose[2] * Math.PI / 180, 0.3 * s,
        color);
    }
  }
  return { X, Y, s };
}

// A platform seen from above: a square with a tick to its front (+x).
function platform(ctx, cx, cy, theta, size, color) {
  ctx.save();
  ctx.translate(cx, cy);
  ctx.rotate(-theta);
  ctx.fillStyle = color + 'cc';
  ctx.strokeStyle = '#0b0d11';
  ctx.lineWidth = 2;
  ctx.fillRect(-size / 2, -size / 2, size, size);
  ctx.strokeRect(-size / 2, -size / 2, size, size);
  ctx.strokeStyle = '#0b0d11';
  ctx.lineWidth = 2.5;
  ctx.beginPath();
  ctx.moveTo(0, 0);
  ctx.lineTo(size / 2, 0);
  ctx.stroke();
  ctx.restore();
}

/**
 * A colour bar for the height map: low (blue) to high (red).
 * @param {number} range The height range [um].
 * @return {string} HTML.
 */
export function colorBar(range) {
  const stops = Array.from({ length: 11 }, (_, k) => `${jetCss(k / 10)} ${k * 10}%`).join(', ');
  return '<div class="cal-bar"><span>0 µm</span>' +
    `<i style="background:linear-gradient(90deg, ${stops})"></i>` +
    `<span>${range < 10 ? range.toFixed(1) : Math.round(range)} µm</span></div>`;
}

// The thrusters

// The 8 thrusters in the body frame (as viewer.js): the nozzle [x, y] [m] and the way each pushes
// the platform.
export const THRUSTERS = [
  [[0.1547, 0.069], [-1, 0]], [[0.1547, -0.069], [-1, 0]],     // T1, T2 push along -x
  [[0.069, -0.1547], [0, 1]], [[-0.069, -0.1547], [0, 1]],     // T3, T4 along +y
  [[-0.1547, -0.069], [1, 0]], [[-0.1547, 0.069], [1, 0]],     // T5, T6 along +x
  [[-0.069, 0.1547], [0, -1]], [[0.069, 0.1547], [0, -1]],     // T7, T8 along -y
];

/**
 * The colour of a thrust against the nominal one: within 3 % neutral, weaker amber to red,
 * stronger blue.
 * @param {?number} ratio Thrust over nominal.
 * @return {string} A CSS colour.
 */
export function deviationColor(ratio) {
  if (ratio == null || !Number.isFinite(ratio)) {
    return '#8a93a3';
  }
  const d = ratio - 1;
  if (Math.abs(d) < 0.03) {
    return '#3ccf7e';
  }
  if (d < 0) {
    return d < -0.1 ? '#ff6b6b' : '#f5a524';
  }
  return d > 0.1 ? '#4c8dff' : '#74c0fc';
}

/**
 * The platform from above (+x up, +y left, as the Hardware tab draws it), with each thruster's
 * measured push drawn outside the body as an arrow onto its nozzle: length by thrust, turned by
 * its measured angle, coloured against nominal, its thrust beyond it.
 * @param {{thrust: ?Array<number>, nominal: number, angle: ?Array<number>, firing: ?number}=}
 *     opts Thrust [N] and angle [deg] per thruster; FIRING: a thruster to light up (a run under
 *     way: its plume, the gas leaving the nozzle).
 * @return {string} SVG.
 */
export function thrusterDiagram({
  thrust = null, nominal = 0.2825, angle = null, firing = null,
} = {}) {
  const S = 520;   // px per m
  const P = ([x, y]) => [150 - y * S, 150 - x * S];   // body (x up, y left) -> svg
  const parts = [
    `<rect x="${150 - 0.15 * S}" y="${150 - 0.15 * S}" width="${0.3 * S}" height="${0.3 * S}" ` +
    'rx="10" class="cal-body"/>',
    '<path d="M150 170V122M143 131l7-9 7 9" class="cal-front"/>' +
    '<text x="150" y="114" class="cal-front-t">+x</text>',
  ];
  THRUSTERS.forEach(([nozzle, push], k) => {
    const [nx, ny] = P(nozzle);

    // The measured push: an arrow from beyond the nozzle onto it, turned by the angle, and its
    // thrust further out along the nominal push.
    if (thrust) {
      const a = (angle && Number.isFinite(angle[k]) ? angle[k] : 0) * Math.PI / 180;
      const px = push[0] * Math.cos(a) - push[1] * Math.sin(a);
      const py = push[0] * Math.sin(a) + push[1] * Math.cos(a);
      const len = 60 * thrust[k] / 0.3;
      const color = deviationColor(thrust[k] / nominal);
      const [tx, ty] = P([nozzle[0] - px * (len + 14) / S, nozzle[1] - py * (len + 14) / S]);
      const [hx, hy] = P([nozzle[0] - px * 14 / S, nozzle[1] - py * 14 / S]);
      const [lx, ly] =
        P([nozzle[0] - push[0] * (len + 34) / S, nozzle[1] - push[1] * (len + 34) / S]);
      parts.push(
        `<defs><marker id="calArrow${k}" viewBox="0 0 10 10" refX="7" refY="5" ` +
        'markerWidth="5" markerHeight="5" orient="auto">' +
        `<path d="M0 0L10 5L0 10z" fill="${color}"/></marker></defs>`,
        `<line x1="${tx}" y1="${ty}" x2="${hx}" y2="${hy}" stroke="${color}" stroke-width="3.5" ` +
        `marker-end="url(#calArrow${k})"/>`,
        `<text x="${lx}" y="${ly + 4}" class="cal-thrust-t" fill="${color}">` +
        `${thrust[k].toFixed(3)}</text>`);
    }

    // The plume: the gas leaves the nozzle, away from the push.
    if (firing === k + 1) {
      const [fx, fy] = P([nozzle[0] - push[0] * 0.09, nozzle[1] - push[1] * 0.09]);
      parts.push(`<line x1="${nx}" y1="${ny}" x2="${fx}" y2="${fy}" class="cal-plume"/>`);
    }

    // The nozzle, numbered.
    parts.push(
      `<circle cx="${nx}" cy="${ny}" r="9" class="cal-nozzle${firing === k + 1 ? ' lit' : ''}"/>`,
      `<text x="${nx}" y="${ny + 3.5}" class="cal-nozzle-t">${k + 1}</text>`);
  });
  return '<svg viewBox="-70 -70 440 440" class="cal-thr-svg" role="img" ' +
    `aria-label="The platform from above, with each thruster's measured push">${parts.join('')}` +
    '</svg>';
}

/**
 * Bars: each thruster's thrust with its spread (error bar), and the nominal thrust (dashed).
 * @param {{thrust: !Array<number>, sd: !Array<number>, nominal: number,
 *     valid: !Array<boolean>}} opts Thrust and its standard deviation [N] per thruster, and
 *     which had a good burn.
 * @return {string} SVG and its key.
 */
export function thrustBars({ thrust, sd = [], nominal, valid = [] }) {
  // The plot [px] and its scale: up to the highest bar (with its spread) or 1.25 x nominal.
  const W = 560;
  const H = 230;
  const L = 46;
  const B = 30;
  const T = 14;
  const top = Math.max(nominal * 1.25, ...thrust.map((f, k) => f + (sd[k] || 0))) * 1.05;
  const Y = v => H - B - (H - B - T) * v / top;
  const bw = (W - L - 10) / 8;

  // Grid lines every 0.1 N.
  const parts = [];
  for (const v of [0, 0.1, 0.2, 0.3, 0.4, 0.5].filter(v => v <= top)) {
    parts.push(`<line x1="${L}" x2="${W - 6}" y1="${Y(v)}" y2="${Y(v)}" class="cal-grid"/>` +
      `<text x="${L - 6}" y="${Y(v) + 4}" class="cal-ax-t" style="text-anchor:end">` +
      `${v.toFixed(1)}</text>`);
  }

  // Each bar, coloured against nominal (grey: no good burn), with its error bar and labels.
  thrust.forEach((f, k) => {
    const x = L + k * bw + bw * 0.18;
    const w = bw * 0.64;
    const color = valid[k] === false ? '#4a515e' : deviationColor(f / nominal);
    parts.push(`<rect x="${x}" y="${Y(f)}" width="${w}" height="${Y(0) - Y(f)}" rx="3" ` +
      `fill="${color}" opacity="0.85"><title>T${k + 1}: ${f.toFixed(4)} N` +
      `${sd[k] ? ` ± ${sd[k].toFixed(4)}` : ''}` +
      `${valid[k] === false ? ' (no good burn: nominal)' : ''}</title></rect>`);
    if (sd[k]) {
      const cx = x + w / 2;
      parts.push(`<path d="M${cx} ${Y(f + sd[k])}V${Y(f - sd[k])}` +
        `M${cx - 6} ${Y(f + sd[k])}h12M${cx - 6} ${Y(f - sd[k])}h12" class="cal-err"/>`);
    }
    parts.push(
      `<text x="${x + w / 2}" y="${H - 10}" class="cal-ax-t" text-anchor="middle">T${k + 1}</text>`,
      `<text x="${x + w / 2}" y="${Y(f) - 6}" class="cal-bar-t" text-anchor="middle">` +
      `${f.toFixed(3)}</text>`);
  });

  // The nominal line and the axis label.
  parts.push(
    `<line x1="${L}" x2="${W - 6}" y1="${Y(nominal)}" y2="${Y(nominal)}" ` +
    `class="cal-nominal"><title>nominal ${nominal} N</title></line>`,
    `<text x="12" y="${T + (H - B - T) / 2}" class="cal-ax-t" text-anchor="middle" ` +
    `transform="rotate(-90 12 ${T + (H - B - T) / 2})">thrust [N]</text>`);
  return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Each thruster's measured thrust against nominal">${parts.join('')}</svg>
    <p class="cal-key"><i class="dash"></i>nominal ${nominal} N <i style="background:#3ccf7e"></i>within 3 % <i style="background:#f5a524"></i>weaker
      <i style="background:#ff6b6b"></i>over 10 % weaker <i style="background:#74c0fc"></i>stronger <i style="background:#4c8dff"></i>over 10 % stronger</p>`;
}

/**
 * The fitted share of each thruster's thrust alone with N open: linear 1 - m (N - 1), or
 * rational 1 / (1 + k (N - 1)).
 * @param {{model: string, slope: number, k: number}} decay The fit.
 * @param {number} n Thrusters open.
 * @return {number}
 */
export function decayShare({ model = 'linear', slope = 0, k = 0 } = {}, n) {
  return model === 'rational' ? 1 / (1 + k * (n - 1)) : Math.max(0, 1 - slope * (n - 1));
}
export const decayFormula = d =>
  (d && d.model === 'rational' ? '1 / (1 + k (N − 1))' : '1 − m (N − 1)');

/**
 * Thrusters together: each thruster's share of its thrust alone [%] against how many are open,
 * the firings (filled: fitted; hollow: left out, their pushes cancel) over the fitted decay.
 * @param {!Object=} dec The decay fit ({model, slope, k, scale, firings}).
 * @return {string} SVG and its key.
 */
export function decayPlot(dec = {}) {
  const { scale = 1, firings = [] } = dec;

  // The plot [px].
  const W = 560;
  const H = 230;
  const L = 46;
  const B = 34;
  const T = 14;
  const R = 12;

  // The firings as shares [%] of the fit's scale; the range down to the lowest used one.
  const pts = firings.filter(f => f.ratio != null).map(f => ({
    n: f.n, v: 100 * f.ratio / (scale || 1),
    e: f.ratio_sd != null ? 100 * f.ratio_sd / (scale || 1) : 0, used: f.used,
  }));
  const fitted = n => 100 * decayShare(dec, n);
  const vals = [fitted(8), ...pts.filter(p => p.used).map(p => p.v - p.e)];
  const lo = Math.max(0, Math.floor((Math.min(...vals) - 5) / 10) * 10);
  const hi = 105;
  const X = n => L + (W - L - R) * (n - 1) / 7;
  const Y = v => T + (H - B - T) * (hi - Math.min(hi, Math.max(lo, v))) / (hi - lo);

  // Grid lines every 10 %, and 1 to 8 along the bottom.
  const parts = [];
  for (let v = lo; v <= 100; v += 10) {
    parts.push(`<line x1="${L}" x2="${W - R}" y1="${Y(v)}" y2="${Y(v)}" class="cal-grid"/>` +
      `<text x="${L - 6}" y="${Y(v) + 4}" class="cal-ax-t" style="text-anchor:end">${v}</text>`);
  }
  for (let n = 1; n <= 8; n++) {
    parts.push(`<text x="${X(n)}" y="${H - B + 16}" class="cal-ax-t">${n}</text>`);
  }

  // The fitted curve, then the firings with their error bars.
  const curve = Array.from({ length: 71 }, (_, i) => {
    const n = 1 + 7 * i / 70;
    return `${X(n).toFixed(1)},${Y(fitted(n)).toFixed(1)}`;
  }).join(' ');
  parts.push(`<polyline points="${curve}" fill="none" stroke="#ffd43b" stroke-width="2"/>`);
  for (const p of pts) {
    if (p.used && p.e) {
      parts.push(`<path d="M${X(p.n)} ${Y(p.v + p.e)}V${Y(p.v - p.e)}" class="cal-err"/>`);
    }
    parts.push(`<circle cx="${X(p.n)}" cy="${Y(p.v)}" r="4.5" ` +
      `${p.used ? 'fill="#4c8dff"' : 'fill="none" stroke="#6b7385" stroke-width="1.5"'}>` +
      `<title>${p.n} open: ${p.v.toFixed(1)} %` +
      `${p.used ? '' : ' (left out: their pushes cancel)'}</title></circle>`);
  }
  parts.push(`<text x="${(L + W) / 2}" y="${H - 4}" class="cal-ax-t">thrusters open</text>`,
    `<text x="12" y="${(T + H - B) / 2}" class="cal-ax-t" ` +
    `transform="rotate(-90 12 ${(T + H - B) / 2})">% of thrust alone</text>`);
  return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Each thruster's output against the number open">${parts.join('')}</svg>
    <p class="cal-key"><i style="background:#4c8dff"></i>measured <i style="background:#ffd43b"></i>fitted ${decayFormula(dec)}</p>`;
}

/**
 * Small multiples: each thruster's burns, the platform's path from where it started (table
 * frame, mm), all at one scale.
 * @param {!Array<!Object>} burns Each {thruster, points: [[x, y], ...] [m], thrust, error}.
 * @return {string} HTML.
 */
export function burnPaths(burns) {
  // Each thruster's burns that moved.
  const by = Array.from({ length: 8 }, () => []);
  for (const b of burns) {
    if (b.points && b.points.length > 1) {
      by[b.thruster - 1].push(b);
    }
  }

  // One scale for all: the furthest any burn went [m], to a box of half-width R [px].
  const reach = Math.max(0.01, ...burns.flatMap(b =>
    (b.points || []).map(([x, y]) => Math.max(Math.abs(x), Math.abs(y)))));
  const R = 40;
  const s = (R - 6) / reach;
  return `<div class="cal-burns">${by.map((list, k) => {
    const lines = list.map(b => `<polyline points="${b.points.map(([x, y]) =>
      `${(R + x * s).toFixed(1)},${(R - y * s).toFixed(1)}`).join(' ')}" ` +
      `class="cal-burn${b.error ? ' bad' : ''}">` +
      `<title>${b.error ? esc(b.error) : `${b.thrust.toFixed(4)} N`}</title></polyline>`).join('');
    return `<figure><svg viewBox="0 0 ${2 * R} ${2 * R}">` +
      `<path d="M${R} 4V${2 * R - 4}M4 ${R}H${2 * R - 4}" class="cal-grid"/>${lines}` +
      `<circle cx="${R}" cy="${R}" r="2.5" fill="#fff"/></svg>` +
      `<figcaption>T${k + 1}</figcaption></figure>`;
  }).join('')}<p class="hint tip" title="Each burn's path from where the platform started, ` +
    `on the table, all at one scale">${Math.round(reach * 1000)} mm to the edge of a box</p></div>`;
}

const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));

// The arm

// What each test does to a joint over time (for the animation): its angle from where it started
// [rad] at time t [s] of the test's loop (about 6 s), and a word for the moment.
const D = Math.PI / 180;
const STEP_DEG = [10, 0, -10, 20, 0, -20, 5, 0];
export const ARM_TEST_MOTION = {
  // The current creeps up until the joint breaks away, then it is held and ramped the other way.
  breakaway: t => {
    const u = t % 6;
    const s = u < 3 ? 1 : -1;
    const v = u % 3;
    return [
      v < 2.2 ? 0 : s * Math.min(8, (v - 2.2) * 30) * D,
      v < 2.2 ? 'current rising' : 'breakaway',
    ];
  },
  // Constant speeds back and forth, faster each pass.
  sweep: t => {
    const u = t % 6;
    const pass = Math.floor(u / 1.5);
    const f = (u % 1.5) / 1.5;
    const dir = pass % 2 ? -1 : 1;
    return [dir * (-25 + 50 * f) * D, `speed ${pass + 1}`];
  },
  // A torque step from rest: it accelerates, then it is caught.
  inertia: t => {
    const u = t % 3;
    return [
      u < 1 ? 0 : Math.min(15, 15 * ((u - 1) / 0.6) ** 2) * D,
      u < 1 ? 'at rest' : 'torque step',
    ];
  },
  // Position goals 10, 0, -10, 20, 0, -20, 5, 0 deg, as the servo follows them.
  steps: t => {
    const k = Math.floor(t / 0.75) % STEP_DEG.length;
    const prev = STEP_DEG[(k + STEP_DEG.length - 1) % STEP_DEG.length];
    const f = Math.min(1, (t % 0.75) / 0.35);
    return [(prev + (STEP_DEG[k] - prev) * (1 - (1 - f) ** 3)) * D, `goal ${STEP_DEG[k]}°`];
  },
};

/**
 * The arm from above on the Chaser's side, its three links, JOINT moving as TEST does at time T.
 * @param {string} test A key of ARM_TEST_MOTION.
 * @param {number} joint 1-3.
 * @param {number} t [s]
 * @return {string} SVG.
 */
export function armFigure(test, joint, t) {
  const L = [58, 54, 30];
  const base = [52, 148];
  const q = [0, 0, 0];
  const [angle, word] = (ARM_TEST_MOTION[test] || (() => [0, '']))(t);
  q[joint - 1] = angle;

  // The pose shown: up and out from the Chaser, the elbow and wrist bent outward.
  const pts = [base];
  let th = -1.0;
  let [x, y] = base;
  for (let k = 0; k < 3; k++) {
    th += q[k] + (k ? 0.55 : 0);
    x += L[k] * Math.cos(th);
    y += L[k] * Math.sin(th);
    pts.push([x, y]);
  }
  const links = pts.slice(1).map((p, k) =>
    `<line x1="${pts[k][0]}" y1="${pts[k][1]}" x2="${p[0]}" y2="${p[1]}" ` +
    `class="cal-link${k === joint - 1 ? ' on' : ''}"/>`).join('');
  const joints = pts.slice(0, 3).map(([px, py], k) =>
    `<circle cx="${px}" cy="${py}" r="${k === joint - 1 ? 8 : 6}" ` +
    `class="cal-joint${k === joint - 1 ? ' on' : ''}"/>`).join('');
  return `<svg viewBox="0 0 260 180" class="cal-arm-svg" role="img" aria-label="The arm, its ${['shoulder', 'elbow', 'wrist'][joint - 1]} moving as in the ${test} test">
    <rect x="8" y="148" width="110" height="24" rx="4" class="cal-body"/><text x="63" y="164" class="cal-ax-t">Chaser</text>
    ${links}${joints}<text x="252" y="18" class="cal-ax-t" style="text-anchor:end">${word}</text></svg>`;
}

/**
 * Friction against speed for each joint: the sweep's points (torque [N m] at each steady speed
 * [rad/s]) and the fitted curve, coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous v.
 * @param {!Array<!Object>} joints Each {index, name, coulomb, static, stribeck, viscous}.
 * @param {!Object} plots By joint index: {sweep: [[speed, torque], ...]}.
 * @return {string} SVG and its key.
 */
export function frictionPlot(joints, plots) {
  // The plot [px], symmetric about 0 in speed and torque.
  const W = 520;
  const H = 250;
  const L = 52;
  const B = 34;
  const T = 12;
  const R = 10;
  const colors = { 1: '#4c8dff', 2: '#f5a524', 3: '#3ccf7e' };
  const pts = joints.flatMap(j => ((plots[String(j.index)] || {}).sweep || []));
  const vmax = Math.max(0.2, ...pts.map(p => Math.abs(p[0]))) * 1.08;
  const tmax = Math.max(0.1, ...pts.map(p => Math.abs(p[1])),
    ...joints.map(j => (j.static || 0) * 1.05)) * 1.1;
  const X = v => L + (W - L - R) * (v + vmax) / (2 * vmax);
  const Y = tq => T + (H - B - T) * (tmax - tq) / (2 * tmax);

  // Axes through 0, and tick labels.
  const parts = [
    `<line x1="${L}" x2="${W - R}" y1="${Y(0)}" y2="${Y(0)}" class="cal-grid"/>`,
    `<line x1="${X(0)}" x2="${X(0)}" y1="${T}" y2="${H - B}" class="cal-grid"/>`,
  ];
  for (const v of [-vmax * 0.9, -vmax / 2, vmax / 2, vmax * 0.9]) {
    parts.push(`<text x="${X(v)}" y="${H - B + 14}" class="cal-ax-t">${(v).toFixed(2)}</text>`);
  }
  for (const tq of [-tmax * 0.8, tmax * 0.8]) {
    parts.push(`<text x="${L - 6}" y="${Y(tq) + 4}" class="cal-ax-t" style="text-anchor:end">` +
      `${tq.toFixed(2)}</text>`);
  }

  // Each joint: its fitted curve (each side of 0) and its measured points.
  for (const j of joints) {
    const c = colors[j.index];
    const f = v => Math.sign(v) * ((j.coulomb || 0) + ((j.static || 0) - (j.coulomb || 0)) *
      Math.exp(-((v / (j.stribeck || 0.05)) ** 2))) + (j.viscous || 0) * v;
    if (j.coulomb != null) {
      for (const side of [-1, 1]) {
        const d = Array.from({ length: 60 }, (_, k) => {
          const v = side * (0.002 + vmax * k / 59);
          return `${X(v).toFixed(1)},${Y(f(v)).toFixed(1)}`;
        }).join(' ');
        parts.push(`<polyline points="${d}" fill="none" stroke="${c}" stroke-width="2"/>`);
      }
    }
    for (const [v, tq] of ((plots[String(j.index)] || {}).sweep || [])) {
      parts.push(`<circle cx="${X(v)}" cy="${Y(tq)}" r="3.2" fill="${c}" opacity="0.85"/>`);
    }
  }

  // Axis labels and the key.
  parts.push(`<text x="${(L + W) / 2}" y="${H - 4}" class="cal-ax-t">joint speed [rad/s]</text>`,
    `<text x="14" y="${(T + H - B) / 2}" class="cal-ax-t" ` +
    `transform="rotate(-90 14 ${(T + H - B) / 2})">friction torque [N m]</text>`);
  const key = joints.map(j =>
    `<i style="background:${colors[j.index]}"></i>${esc(j.name || `joint ${j.index}`)}`).join(' ');
  return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Friction torque against speed, measured and fitted">${parts.join('')}</svg>
    <p class="cal-key">${key} <span>dots: measured · lines: the fitted model</span></p>`;
}

/**
 * A joint's position steps: the goal (dashed) and the measured angle [deg] against time [s].
 * @param {?Array<!Array<number>>} rows Each [t [s], goal [rad], measured [rad]].
 * @return {string} SVG and its key.
 */
export function stepsPlot(rows) {
  if (!rows || rows.length < 2) {
    return '<p class="hint">No position steps in this run.</p>';
  }

  // The plot [px], over the run's time and angles (with 2 deg of margin).
  const W = 520;
  const H = 200;
  const L = 44;
  const B = 30;
  const T = 10;
  const R = 10;
  const t0 = rows[0][0];
  const t1 = rows[rows.length - 1][0];
  const all = rows.flatMap(r => [r[1], r[2]]).map(v => v / D);
  const lo = Math.min(...all) - 2;
  const hi = Math.max(...all) + 2;
  const X = t => L + (W - L - R) * (t - t0) / Math.max(t1 - t0, 1e-6);
  const Y = v => T + (H - B - T) * (hi - v) / (hi - lo);
  const line = k => rows.map(r => `${X(r[0]).toFixed(1)},${Y(r[k] / D).toFixed(1)}`).join(' ');

  // Grid lines every 10 deg, the goal and measured lines, and the axis labels.
  const parts = [];
  for (let v = Math.ceil(lo / 10) * 10; v <= hi; v += 10) {
    parts.push(`<line x1="${L}" x2="${W - R}" y1="${Y(v)}" y2="${Y(v)}" class="cal-grid"/>` +
      `<text x="${L - 6}" y="${Y(v) + 4}" class="cal-ax-t" style="text-anchor:end">${v}</text>`);
  }
  parts.push(
    `<polyline points="${line(1)}" fill="none" stroke="#c3c9d4" stroke-width="1.5" ` +
    'stroke-dasharray="5 4"/>',
    `<polyline points="${line(2)}" fill="none" stroke="#ffd43b" stroke-width="2"/>`,
    `<text x="${(L + W) / 2}" y="${H - 4}" class="cal-ax-t">` +
    `time [s] (${(t1 - t0).toFixed(0)} s)</text>`,
    `<text x="12" y="${(T + H - B) / 2}" class="cal-ax-t" ` +
    `transform="rotate(-90 12 ${(T + H - B) / 2})">angle [deg]</text>`);
  return `<svg viewBox="0 0 ${W} ${H}" class="cal-bars-svg" role="img" aria-label="Position steps: goal and measured">${parts.join('')}</svg>
    <p class="cal-key"><i class="dash"></i>goal <i style="background:#ffd43b"></i>measured</p>
    <p class="hint tip" title="spot.arm.compareSteps replays them in the model">With SPOT's servo settings.</p>`;
}
