// Live Plot (local console): a tab of the MATLAB command window that plots the signals the
// diagram's Live Stream blocks send (the "Live Data Streams" subsystem, spot.hardware.liveStream).
//   Simulation tab: the simulation's streams, live while it runs (best with Real time on), then
//                   the whole run with the replay's time marked.
//   Experiment tab: the platforms' streams as the lab console receives them (UDP 55011-55013).
// "Data available to stream" lists every stream (named after what is connected to its block);
// tick one to plot it, and pick its elements with the [1] [2] ... chips.

import { platformName } from './platforms.js';

const PALETTE = [
  '#4dabf7', '#ff8787', '#69db7c', '#ffd43b', '#da77f2', '#ffa94d', '#3bc9db', '#f783ac',
];
const WINDOWS = [['10', 10], ['30', 30], ['60', 60], ['All', 0]];
const KEEP = 12000;   // samples kept per stream in the page
const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const $ = s => document.querySelector(s);

/**
 * Sets up the Live Plot tab of the MATLAB command window.
 * @param {{api: function(string, !Object): !Promise,
 *     storage: {get: function(string, *): *, set: function(string, *)}}} deps The page's API
 *     call and its settings storage.
 * @return {!Object} What the pages call: show, simStarted, simData, simResult, simTime,
 *     experiment, experimentData.
 */
export function initLivePlot({ api, storage }) {
  const S = {
    mode: 'simulation', tab: storage.get('spot.console.tab', 'cmd'),
    sim: new Map(), simLive: false, simT: null,
    exp: new Map(), expStarted: null, expLoaded: false,
    sel: storage.get('spot.live.sel', {}),   // { 'sim:NAME' | 'exp:PLAT/NAME': [element indexes] }
    win: storage.get('spot.live.window', 30), paused: false, hover: null, dirty: true,
  };
  const consoleEl = $('#matlabConsole');
  const canvas = $('#liveCanvas');
  const ctx = canvas.getContext('2d');

  // Tabs (MATLAB | Live Plot)

  // Shows a tab of the command window (remembered unless switched for another page).
  function setTab(tab, remember = true) {
    S.tab = tab;
    consoleEl.dataset.tab = tab;
    for (const b of document.querySelectorAll('[data-ctab]')) {
      b.classList.toggle('active', b.dataset.ctab === tab);
      b.setAttribute('aria-selected', String(b.dataset.ctab === tab));
    }
    $('#livePane').hidden = tab !== 'plot';
    $('.matlab-body .cmdwin').hidden = tab === 'plot';
    $('#cmdClear').title = tab === 'plot' ? 'Clear the plotted data' : 'Clear the command window';
    if (remember) {
      storage.set('spot.console.tab', tab);
    }
    S.dirty = true;
  }
  for (const b of document.querySelectorAll('[data-ctab]')) {
    b.onclick = () => {
      S.restorePlot = false;
      setTab(b.dataset.ctab);
    };
  }

  // Clear, on the Live Plot tab: the plotted data.
  $('#cmdClear').addEventListener('click', () => {
    if (S.tab !== 'plot') {
      return;
    }
    (S.mode === 'simulation' ? S.sim : S.exp).forEach(s => {
      s.t = [];
      s.v = [];
    });
    S.dirty = true;
  });
  setTab(S.tab);

  // The controls

  // The time window and Pause.
  $('#liveWindow').innerHTML = WINDOWS.map(([label, w]) =>
    `<button type="button" data-win="${w}" ` +
    `title="${w ? `The last ${w} s` : 'Everything received'}">` +
    `${label === 'All' ? 'All' : `${label} s`}</button>`).join('');
  $('#liveWindow').onclick = ev => {
    const b = ev.target.closest('[data-win]');
    if (!b) {
      return;
    }
    S.win = Number(b.dataset.win);
    storage.set('spot.live.window', S.win);
    renderControls();
  };
  $('#livePause').onclick = () => {
    S.paused = !S.paused;
    renderControls();
  };

  // Shows the window chosen and Pause or Resume.
  function renderControls() {
    for (const b of $('#liveWindow').children) {
      b.classList.toggle('active', Number(b.dataset.win) === S.win);
    }
    $('#livePause').textContent = S.paused ? 'Resume' : 'Pause';
    $('#livePause').title = S.paused ? 'Follow the data again'
      : 'Hold the plot where it is (data keeps arriving)';
    S.dirty = true;
  }
  renderControls();

  // The stream list: tick a stream to plot it (its first three elements); its chips choose the
  // elements.
  $('#liveStreams').addEventListener('change', ev => {
    const box = ev.target.closest('input[data-id]');
    if (!box) {
      return;
    }
    const st = streams().find(s => s.id === box.dataset.id);
    if (box.checked) {
      S.sel[box.dataset.id] = st && st.n > 3 ? [0, 1, 2] : [...Array(st ? st.n : 1).keys()];
    } else {
      delete S.sel[box.dataset.id];
    }
    saveSel();
  });
  $('#liveStreams').addEventListener('click', ev => {
    const chip = ev.target.closest('[data-el]');
    if (!chip) {
      return;
    }
    const id = chip.closest('[data-stream]').dataset.stream;
    const k = Number(chip.dataset.el);
    const on = new Set(S.sel[id] || []);
    on.has(k) ? on.delete(k) : on.add(k);
    if (on.size) {
      S.sel[id] = [...on].sort((a, b) => a - b);
    } else {
      delete S.sel[id];
    }
    saveSel();
  });

  // Remembers what is plotted.
  function saveSel() {
    storage.set('spot.live.sel', S.sel);
    renderList();
    S.dirty = true;
  }

  // Data in

  // A stream's record (a new one if its element count changed).
  function store(map, id, fields) {
    let s = map.get(id);
    if (!s || s.n !== fields.n) {
      s = { id, t: [], v: [], ...fields };
      map.set(id, s);
      S.listDirty = true;
    }
    return s;
  }

  // Adds samples to a stream, keeping the newest KEEP.
  function append(s, t, rows) {
    for (let k = 0; k < t.length; k++) {
      s.t.push(t[k]);
      s.v.push(rows[k]);
    }
    if (s.t.length > KEEP) {
      s.t.splice(0, s.t.length - KEEP);
      s.v.splice(0, s.v.length - KEEP);
    }
  }

  // MATLAB's JSON: t is a number for one sample; v a flat array for one sample or one column.
  function rowsOf(t, v) {
    const ts = list(t);
    if (!Array.isArray(t)) {
      return [ts, [list(v)]];
    }
    const vs = list(v);
    return [ts, vs.length && !Array.isArray(vs[0]) ? vs.map(x => [x]) : vs];
  }

  // A simulation stream's record and samples.
  function simStream(m) {
    const [t, rows] = rowsOf(m.t, m.v);
    const n = rows.length ? rows[0].length : 1;
    return [store(S.sim, `sim:${m.name}`, { name: m.name, platform: '', n }), t, rows];
  }

  const api_ = {
    // The page shown: 'simulation' or 'experiment'; null on a page without the Live Plot tab
    // (Results), which shows the command window and brings the plot back on the way out.
    show(mode) {
      $('[data-ctab="plot"]').hidden = !mode;
      if (!mode) {
        if (S.tab === 'plot') {
          S.restorePlot = true;
          setTab('cmd', false);
        }
        return;
      }
      if (S.restorePlot) {
        S.restorePlot = false;
        setTab('plot', false);
      }
      if (mode === S.mode && S.shownOnce) {
        return;
      }
      S.mode = mode;
      S.shownOnce = true;
      if (mode === 'experiment' && !S.expLoaded) {
        loadExperiment();
      }
      S.listDirty = true;
      S.dirty = true;
    },
    // A new simulation.
    simStarted() {
      S.sim = new Map();
      S.simLive = true;
      S.simT = null;
      S.listDirty = S.dirty = true;
    },
    // New samples during a simulation (the worker's sim_live event).
    simData(streams) {
      for (const m of list(streams)) {
        const [s, t, rows] = simStream(m);
        append(s, t, rows);
      }
      S.dirty = true;
    },
    // The finished simulation's streams (thinned to a few thousand samples each).
    simResult(live) {
      S.sim = new Map();
      for (const m of list(live)) {
        const [s, t, rows] = simStream(m);
        append(s, t, rows);
      }
      S.simLive = false;
      S.listDirty = S.dirty = true;
    },
    // The replay's time.
    simTime(t) {
      S.simT = t;
      if (S.mode === 'simulation' && !S.simLive) {
        S.dirty = true;
      }
    },
    // The lab console's experiment status: its list of streams, and a new start clears them.
    experiment(exp) {
      if (!exp) {
        return;
      }
      if (exp.started && exp.started !== S.expStarted) {
        if (S.expStarted !== null) {
          S.exp = new Map();
        }
        S.expStarted = exp.started;
      }
      for (const l of list(exp.live)) {
        store(S.exp, `exp:${l.key}`, { name: l.name, platform: l.platform, n: l.n });
      }
    },
    // New samples from the lab console (its live_data event).
    experimentData(batch) {
      for (const [key, b] of Object.entries(batch || {})) {
        const s = store(S.exp, `exp:${key}`, { name: b.name, platform: b.platform, n: b.n });
        append(s, b.rows.map(r => r[0]), b.rows.map(r => r.slice(1)));
      }
      S.dirty = true;
    },
  };

  // What the lab console has kept so far (a page opened during an experiment); if it isn't
  // reachable (or is older), retried later.
  async function loadExperiment() {
    S.expLoaded = true;
    try {
      const r = await api('/api/lab', { path: 'experiment/live' });
      for (const [key, b] of Object.entries((r.result || {}).streams || {})) {
        const s = store(S.exp, `exp:${key}`, { name: b.name, platform: b.platform, n: b.n });
        if (!s.t.length) {
          append(s, b.rows.map(x => x[0]), b.rows.map(x => x.slice(1)));
        }
      }
      S.listDirty = S.dirty = true;
    } catch {
      S.expLoaded = false;
    }
  }

  // The stream list

  // The page's streams, by platform and name; those ticked.
  const streams = () => [...(S.mode === 'simulation' ? S.sim : S.exp).values()]
    .sort((a, b) => (a.platform + a.name).localeCompare(b.platform + b.name));
  const selected = () => streams().filter(s => S.sel[s.id]);

  // Each stream with its tick box and (ticked, several elements) its element chips, coloured as
  // their lines.
  function renderList() {
    S.listDirty = false;
    const all = streams();
    $('#liveCount').textContent = all.length ? String(all.length) : '';
    $('#liveSource').textContent = S.mode === 'simulation'
      ? (S.simLive ? 'Simulation Running' : 'Last Simulation')
      : 'From the Platforms';
    if (!all.length) {
      $('#liveStreams').innerHTML = `<p class="live-none">${S.mode === 'simulation'
        ? 'No streams yet. Run a simulation of a diagram with a <b>Live Data Streams</b> ' +
          'subsystem: each Live Stream block in it appears here, named after what is ' +
          'connected to it.'
        : 'No streams yet. They appear while an experiment runs, from the Live Stream ' +
          'blocks of its diagram.'}</p>`;
      return;
    }
    let colour = 0;
    $('#liveStreams').innerHTML = all.map(s => {
      // A ticked stream takes its colours whether or not it has chips.
      const on = S.sel[s.id];
      const chips = on && s.n > 1
        ? `<div class="live-els">${[...Array(s.n).keys()].map(k => {
          const c = on.includes(k) ? PALETTE[colour++ % PALETTE.length] : '';
          return `<button type="button" class="live-el${c ? ' on' : ''}" data-el="${k}" ` +
            `style="${c ? `--c:${c}` : ''}" title="Element ${k + 1} of ${s.n}">${k + 1}</button>`;
        }).join('')}</div>`
        : on ? (colour++, '') : '';
      return `<div class="live-stream" data-stream="${esc(s.id)}">
        <label class="live-check" title="${esc(s.name)}${s.platform ? ` from the ${platformName(s.platform)}` : ''} · ${s.n} value${s.n === 1 ? '' : 's'} per sample">
          <input type="checkbox" data-id="${esc(s.id)}" ${on ? 'checked' : ''}>
          <span class="live-name">${esc(s.name)}</span>${s.platform ? `<span class="live-plat">${esc(platformName(s.platform))}</span>` : ''}
          <span class="live-n">${s.n}</span></label>${chips}</div>`;
    }).join('');
  }

  // Drawing

  // The lines to draw: [{ s, k, colour, label }].
  function series() {
    const out = [];
    let colour = 0;
    for (const s of selected()) {
      for (const k of S.sel[s.id].filter(k => k < s.n)) {
        out.push({
          s, k, colour: PALETTE[colour++ % PALETTE.length],
          label: `${s.platform ? platformName(s.platform) + ' · ' : ''}${s.name}` +
            `${s.n > 1 ? ` [${k + 1}]` : ''}`,
        });
      }
    }
    return out;
  }

  // A round tick step (1, 2, 2.5 or 5 x 10^n) giving about this many ticks over the span.
  function nice(span, ticks) {
    const raw = span / ticks;
    const p = Math.pow(10, Math.floor(Math.log10(raw)));
    return [1, 2, 2.5, 5, 10].map(m => m * p).find(step => step >= raw) || raw;
  }

  // The first index of a sorted array whose value is >= x.
  function lowerBound(arr, x) {
    let lo = 0;
    let hi = arr.length;
    while (lo < hi) {
      const mid = (lo + hi) >> 1;
      if (arr[mid] < x) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    return lo;
  }

  // Draws the plot and its legend.
  function draw() {
    S.dirty = false;

    // The canvas at the screen's resolution (at most 2x).
    const dpr = Math.min(window.devicePixelRatio || 1, 2);
    const w = canvas.clientWidth;
    const h = canvas.clientHeight;
    if (!w || !h) {
      return;
    }
    if (canvas.width !== Math.round(w * dpr) || canvas.height !== Math.round(h * dpr)) {
      canvas.width = Math.round(w * dpr);
      canvas.height = Math.round(h * dpr);
    }
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.clearRect(0, 0, w, h);

    // Nothing ticked.
    const lines = series();
    const empty = $('#liveEmpty');
    empty.hidden = lines.length > 0;
    if (!lines.length) {
      empty.textContent = streams().length ? 'Tick a stream on the left to plot it.' : '';
      $('#liveLegend').innerHTML = '';
      return;
    }

    // Time range: the window, ending at the newest sample (or the replay's time, or held).
    let tEnd = -Infinity;
    let tStart = Infinity;
    for (const { s } of lines) {
      if (s.t.length) {
        tEnd = Math.max(tEnd, s.t[s.t.length - 1]);
        tStart = Math.min(tStart, s.t[0]);
      }
    }
    if (!isFinite(tEnd)) {
      tEnd = 1;
      tStart = 0;
    }
    const replay = S.mode === 'simulation' && !S.simLive && S.simT != null;
    if (S.paused && S.held) {
      tEnd = S.held;
    } else if (replay && S.win) {
      tEnd = Math.max(S.simT, tStart + S.win);
    }
    S.held = tEnd;
    const x0 = S.win ? tEnd - S.win : tStart;
    const x1 = Math.max(tEnd, x0 + 1e-6);

    // Value range over what is shown, with a margin.
    let y0 = Infinity;
    let y1 = -Infinity;
    for (const { s, k } of lines) {
      for (let i = Math.max(0, lowerBound(s.t, x0) - 1); i < s.t.length && s.t[i] <= x1; i++) {
        const y = s.v[i][k];
        if (y != null && isFinite(y)) {
          if (y < y0) {
            y0 = y;
          }
          if (y > y1) {
            y1 = y;
          }
        }
      }
    }
    if (!isFinite(y0)) {
      y0 = -1;
      y1 = 1;
    }
    if (y1 - y0 < 1e-9) {
      y0 -= 1;
      y1 += 1;
    }
    const pad = (y1 - y0) * 0.06;
    y0 -= pad;
    y1 += pad;

    // The plot area (margins [px]: left, right, top, bottom) and its scales.
    const L = 58;
    const R = 12;
    const T = 10;
    const B = 24;
    const pw = w - L - R;
    const ph = h - T - B;
    const X = t => L + (t - x0) / (x1 - x0) * pw;
    const Y = y => T + (1 - (y - y0) / (y1 - y0)) * ph;

    // Grid and ticks: values on the left, times below.
    ctx.font = '11px ui-monospace, monospace';
    ctx.strokeStyle = 'rgba(255,255,255,0.07)';
    ctx.fillStyle = '#8d96a6';
    ctx.lineWidth = 1;
    const ys = nice(y1 - y0, Math.max(2, Math.floor(ph / 40)));
    ctx.textAlign = 'right';
    ctx.textBaseline = 'middle';
    for (let y = Math.ceil(y0 / ys) * ys; y <= y1; y += ys) {
      ctx.beginPath();
      ctx.moveTo(L, Y(y));
      ctx.lineTo(L + pw, Y(y));
      ctx.stroke();
      ctx.fillText(Math.abs(y) < ys * 1e-6 ? '0' : Number(y.toPrecision(4)).toString(),
        L - 6, Y(y));
    }
    const xs = nice(x1 - x0, Math.max(2, Math.floor(pw / 80)));
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    for (let x = Math.ceil(x0 / xs) * xs; x <= x1; x += xs) {
      ctx.beginPath();
      ctx.moveTo(X(x), T);
      ctx.lineTo(X(x), T + ph);
      ctx.stroke();
      ctx.fillText(`${Number(x.toPrecision(5))}`, X(x), T + ph + 5);
    }
    ctx.textAlign = 'right';
    ctx.fillText('t [s]', L + pw, T + ph + 5 - 14);

    // The lines (thinned to about one point per pixel), broken at missing values.
    ctx.save();
    ctx.beginPath();
    ctx.rect(L, T, pw, ph);
    ctx.clip();
    for (const { s, k, colour } of lines) {
      const i0 = Math.max(0, lowerBound(s.t, x0) - 1);
      const i1 = Math.min(s.t.length, lowerBound(s.t, x1) + 1);
      const step = Math.max(1, Math.floor((i1 - i0) / (pw * 1.5)));
      ctx.strokeStyle = colour;
      ctx.lineWidth = 1.6;
      ctx.beginPath();
      let pen = false;
      for (let i = i0; i < i1; i += step) {
        const y = s.v[i][k];
        if (y == null || !isFinite(y)) {
          pen = false;
          continue;
        }
        if (pen) {
          ctx.lineTo(X(s.t[i]), Y(y));
        } else {
          ctx.moveTo(X(s.t[i]), Y(y));
          pen = true;
        }
      }
      ctx.stroke();
    }

    // The replay's time, and the hover readout's time.
    const mark = (t, colour, dash) => {
      if (t == null || t < x0 || t > x1) {
        return;
      }
      ctx.strokeStyle = colour;
      ctx.setLineDash(dash);
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.moveTo(X(t), T);
      ctx.lineTo(X(t), T + ph);
      ctx.stroke();
      ctx.setLineDash([]);
    };
    if (replay) {
      mark(S.simT, 'rgba(255,255,255,0.55)', []);
    }
    const ht = S.hover != null ? x0 + (S.hover - L) / pw * (x1 - x0) : null;
    if (ht != null && ht >= x0 && ht <= x1) {
      mark(ht, 'rgba(255,212,59,0.7)', [4, 3]);
    }
    ctx.restore();

    // Legend, with the values at the hover (or the newest / replay time): the last sample at
    // or before that time.
    const at = ht != null && ht >= x0 && ht <= x1 ? ht : replay ? S.simT : null;
    $('#liveLegend').innerHTML = lines.map(({ s, k, colour, label }) => {
      let val = '';
      if (s.t.length) {
        let i = s.t.length - 1;
        if (at != null) {
          i = lowerBound(s.t, at);
          if (i >= s.t.length || s.t[i] > at) {
            i -= 1;
          }
          i = Math.max(0, i);
        }
        const y = s.v[i][k];
        val = y == null || !isFinite(y) ? '—' : Number(y.toPrecision(5)).toString();
      }
      return `<span><i style="background:${colour}"></i>${esc(label)}` +
        `<b class="mono">${val}</b></span>`;
    }).join('') +
      (at != null ? `<span class="live-at mono">t = ${at.toFixed(2)} s</span>` : '');
  }

  // The hover readout follows the mouse.
  canvas.addEventListener('mousemove', ev => {
    S.hover = ev.offsetX;
    S.dirty = true;
  });
  canvas.addEventListener('mouseleave', () => {
    S.hover = null;
    S.dirty = true;
  });

  // Redraw each animation frame, only when something changed and the plot is visible.
  (function frame() {
    if (S.tab === 'plot' && !consoleEl.classList.contains('collapsed') &&
        consoleEl.offsetParent) {
      if (S.listDirty) {
        renderList();
      }
      if (S.dirty) {
        draw();
      }
    }
    requestAnimationFrame(frame);
  })();
  new ResizeObserver(() => {
    S.dirty = true;
  }).observe(canvas);
  return api_;
}
