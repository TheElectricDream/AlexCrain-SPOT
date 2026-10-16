// Results page (local console): load simulation and experiment results and plot their signals,
// in the spirit of the old GUI's Data Viewer. Loaded datasets live in MATLAB (see data.js); each
// plot is one y axis with up to eight lines (a signal column of a dataset each), against time or
// against another signal (x-y, e.g. a trajectory). Drag to zoom, double-click to zoom back out;
// with "Link time" the time plots zoom and hover together.

import { list, esc, kindTag } from './data.js';
import { COLORS, platformName } from './platforms.js';

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];

// Categorical lines, in this order (validated for colour-vision deficiency on the plot surface).
const PALETTE = [
  '#3987e5', '#d95926', '#199e70', '#c98500', '#d55181', '#008300', '#9085e9', '#e66767',
];
const INK = {
  text: '#e6e9ef', muted: '#8b93a3', grid: '#232833', axis: '#3a4150', surface: '#0f1217',
};
const SWATCH = { RED: COLORS.RED, BLACK: COLORS.BLACK, BLUE: COLORS.BLUE };
const GROUPS = ['RED', 'BLACK', 'BLUE', 'ARM'];
const MAX_LINES = 8;
const MAX_PLOTS = 6;
// Plot margins [px]; the right one holds the line labels.
const M = { l: 58, r: 104, t: 10, b: 28 };

// A number to D significant digits, without trailing zeros; '—' for none.
const fmt = (v, d = 4) =>
  (v == null || !isFinite(v) ? '—' : Number(v).toPrecision(d).replace(/\.?0+(e|$)/, '$1'));

// Units and readable names come with each signal from MATLAB (spot.data.summary,
// spot.plotting.signalLabel).
const unitOf = info => (info && info.unit) || '';

// An SVG element with attributes.
const svgEl = (tag, attrs = {}) => {
  const el = document.createElementNS('http://www.w3.org/2000/svg', tag);
  for (const [k, v] of Object.entries(attrs)) {
    el.setAttribute(k, v);
  }
  return el;
};

// Round tick values (steps of 1, 2, 2.5 or 5 x 10^n) from LO to HI, about N of them.
function niceTicks(lo, hi, n) {
  const span = hi - lo;
  if (!(span > 0)) {
    return [lo];
  }
  const raw = span / Math.max(1, n);
  const p = 10 ** Math.floor(Math.log10(raw));
  const step = [1, 2, 2.5, 5, 10].map(k => k * p).find(s => s >= raw) || 10 * p;
  const out = [];
  for (let v = Math.ceil(lo / step) * step; v <= hi + step * 1e-9; v += step) {
    out.push(Math.abs(v) < step * 1e-9 ? 0 : v);
  }
  return out;
}

/**
 * Sets up the Results page.
 * @param {{act: function(function(): !Promise), toast: function(string, string, number),
 *     storage: {get: function(string, *): *, set: function(string, *)}, store: !Object,
 *     showPage: function(string)}} deps The page's action wrapper (reports errors), its
 *     toast and settings storage, the data store (data.js), and a way to switch pages.
 * @return {!Object} Handlers for the page's events: show, hide, onMatlab, onSimFinished.
 */
export function initResults({ act, toast, storage, store, showPage }) {
  let nextPlot = 1;
  const newPlot = () => ({ id: nextPlot++, x: null, equal: false, traces: [], zoom: null });
  const R = {
    plots: storage.get('spot.res.plots', null) || [newPlot()],
    active: null, selected: storage.get('spot.res.selected', null),
    link: storage.get('spot.res.link', true), smooth: storage.get('spot.res.smooth', 0),
    hoverT: null, shown: false, filter: '', loaded: false,
    // trace key -> series (full span), and key + '@' -> series for the zoomed span
    data: new Map(),
  };
  R.active = R.plots[0].id;
  nextPlot = Math.max(0, ...R.plots.map(p => p.id)) + 1;
  const save = () => storage.set('spot.res.plots', R.plots.map(p => ({ ...p, zoom: null })));
  const plotsEl = $('#resPlots');

  // Datasets and signals

  // The loaded datasets (the newest selected if the selected one is gone), then its signals.
  function renderDatasets() {
    const sets = store.loaded;
    if (!sets.some(d => d.id === R.selected)) {
      R.selected = sets.length ? sets[sets.length - 1].id : null;
    }
    $('#resLoadSim').disabled = !store.simulation;
    $('#resDatasets').innerHTML = sets.length ? sets.map(d => {
      const dur = isFinite(d.t1 - d.t0) ? `${fmt(d.t1 - d.t0, 4)} s` : '';
      const plats = d.crafts.map(n => `<i class="swatch" title="${platformName(n)}" ` +
        `style="background:${SWATCH[n]}"></i>`).join('');
      return `<div class="res-ds${d.id === R.selected ? ' sel' : ''}" data-ds="${d.id}" title="${esc(d.source)}">
        <span class="pill">${kindTag(d.kind)}</span><span class="res-ds-name">${esc(d.name)}</span>
        <span class="res-ds-meta">${plats}<span class="muted small">${dur}</span></span>
        ${d.crafts.length ? `<button type="button" class="btn btn-sm" data-play="${d.id}" title="Replay on the table (Simulation tab), over the simulation">3D ▸</button>` : ''}
        <button type="button" class="btn btn-ghost btn-sm" data-unload="${d.id}" title="Close (frees it in MATLAB)">✕</button></div>`;
    }).join('') : '<p class="hint" title="Load the last simulation, a run from Saved Data, or ' +
      'any .mat file with logged signals">Nothing loaded.</p>';
    renderSignals();
  }

  // The selected dataset's signals by platform (filtered), each with its column buttons; those
  // in the active plot are lit.
  function renderSignals() {
    const d = store.byId(R.selected);
    $('#resSigFor').textContent = d ? d.name : '';
    const box = $('#resSignals');
    if (!d) {
      box.innerHTML = '';
      return;
    }
    const plot = activePlot();
    const f = R.filter.toLowerCase();
    const groups = {};
    for (const s of d.signals) {
      if (f && !s.name.toLowerCase().includes(f)) {
        continue;
      }
      const g = GROUPS.find(p => s.name.startsWith(p + '_')) || 'Other';
      (groups[g] = groups[g] || []).push(s);
    }
    const inPlot = (name, col) =>
      plot && plot.traces.some(t => t.id === d.id && t.signal === name && t.col === col);
    const open = storage.get('spot.res.closedGroups', []);
    box.innerHTML = [...GROUPS, 'Other'].filter(g => groups[g]).map(g => `<details data-group="${g}"${open.includes(g) ? '' : ' open'}>
      <summary>${SWATCH[g] ? `<i class="swatch" style="background:${SWATCH[g]}"></i>` : ''}${platformName(g)}<span class="muted small">${groups[g].length}</span></summary>
      ${groups[g].map(s => {
        const short = SWATCH[g] || g === 'ARM' ? s.name.slice(g.length + 1) : s.name;
        const cols = s.cols > 1 ? `<span class="res-cols">${Array.from({ length: Math.min(s.cols, 12) }, (_, k) =>
          `<button type="button" class="${inPlot(s.name, k + 1) ? 'on' : ''}" data-sig="${esc(s.name)}" data-col="${k + 1}" title="Column ${k + 1}">${k + 1}</button>`).join('')}</span>` : '';
        return `<div class="res-sig${s.zero ? ' zero' : ''}${s.cols === 1 && inPlot(s.name, 1) ? ' on' : ''}" title="${esc(s.name)}${s.zero ? ' (all zero)' : ''}${s.cols > 1 ? ` · ${s.cols} columns` : ''}">
          <button type="button" class="res-sig-name" data-sig="${esc(s.name)}" data-col="1">${esc(short)}</button>${cols}</div>`;
      }).join('')}</details>`).join('') || '<p class="hint">No signal matches.</p>';
  }

  // Which groups are folded, remembered.
  $('#resSignals').addEventListener('toggle', ev => {
    const det = ev.target.closest('details[data-group]');
    if (!det) {
      return;
    }
    const closed = new Set(storage.get('spot.res.closedGroups', []));
    if (det.open) {
      closed.delete(det.dataset.group);
    } else {
      closed.add(det.dataset.group);
    }
    storage.set('spot.res.closedGroups', [...closed]);
  }, true);

  // A signal (or column) in or out of the active plot; the filter.
  $('#resSignals').addEventListener('click', ev => {
    const b = ev.target.closest('[data-sig]');
    if (b) {
      toggleTrace(R.selected, b.dataset.sig, Number(b.dataset.col));
    }
  });
  $('#resSigFilter').addEventListener('input', ev => {
    R.filter = ev.target.value.trim();
    renderSignals();
  });

  // A dataset's buttons: close it (its lines go), replay it in 3D, or select it.
  $('#resDatasets').addEventListener('click', ev => {
    const un = ev.target.closest('[data-unload]');
    if (un) {
      const id = un.dataset.unload;
      for (const p of R.plots) {
        p.traces = p.traces.filter(t => t.id !== id);
      }
      save();
      act(() => store.unload(id));
      return;
    }
    const play = ev.target.closest('[data-play]');
    if (play) {
      store.setOverlay(play.dataset.play, { on: true });
      showPage('simulation');
      return;
    }
    const row = ev.target.closest('[data-ds]');
    if (row) {
      R.selected = row.dataset.ds;
      storage.set('spot.res.selected', R.selected);
      renderDatasets();
    }
  });

  // Load the last simulation, a saved run or a file; the last one loaded is selected.
  $('#resLoadSim').onclick = () => act(async () => select(await store.keepSimulation()));
  $('#resLoadSaved').onclick = () => act(async () => select(await store.pick()));
  $('#resLoadFile').onclick = () => act(async () => select(await store.openFile()));
  function select(got) {
    if (!got || !got.length) {
      return;
    }
    R.selected = got[got.length - 1].id;
    storage.set('spot.res.selected', R.selected);
    renderDatasets();
  }

  // Plots

  // The active plot; a dataset's name; a line's label; its signal's info; the x signal's info.
  const activePlot = () => R.plots.find(p => p.id === R.active) || R.plots[0];
  const dsName = id => (store.byId(id) || { name: '?' }).name;
  const traceLabel = t =>
    `${t.signal}${t.col > 1 || (signalInfo(t) || {}).cols > 1 ? ` [${t.col}]` : ''}`;
  const signalInfo = t =>
    (store.byId(t.id) || { signals: [] }).signals.find(s => s.name === t.signal);
  const xInfo = plot => plot.x && plot.traces.length
    ? (store.byId(plot.traces[0].id) || { signals: [] }).signals.find(s => s.name === plot.x.signal)
    : null;

  // A line's data's key. (A platform's Py against a Px is paired with its own Px by MATLAB:
  // spot.data.series.)
  const traceKey = (t, plot) => JSON.stringify([t.id, t.signal, t.col, plot.x, R.smooth]);

  // A signal column in or out of the active plot; each line keeps its colour for as long as it
  // is in the plot.
  function toggleTrace(id, signal, col) {
    if (!id) {
      return;
    }
    const plot = activePlot();
    const k = plot.traces.findIndex(t => t.id === id && t.signal === signal && t.col === col);
    if (k >= 0) {
      plot.traces.splice(k, 1);
    } else {
      if (plot.traces.length >= MAX_LINES) {
        return toast(`A plot holds up to ${MAX_LINES} lines: add another plot.`, 'info', 5000);
      }
      const used = new Set(plot.traces.map(t => t.slot));
      plot.traces.push({ id, signal, col, slot: [...PALETTE.keys()].find(s => !used.has(s)) });
    }
    save();
    renderSignals();
    renderPlots();
  }

  // Every plot, in order: keep each plot's element (and its last drawing while new data loads).
  function renderPlots() {
    const have = new Map($$('.res-plot', plotsEl).map(el => [Number(el.dataset.plot), el]));
    for (const [id, el] of have) {
      if (!R.plots.some(p => p.id === id)) {
        el.remove();
      }
    }
    R.plots.forEach(plot => {
      let el = have.get(plot.id);
      if (!el) {
        el = plotShell(plot);
        plotsEl.append(el);
      }
      plotsEl.append(el);   // keeps the order
      renderHead(plot, el);
      refreshPlot(plot, el);
    });
    $('#resAddPlot').disabled = R.plots.length >= MAX_PLOTS;
    $('#resEmpty').hidden = R.plots.some(p => p.traces.length);
  }

  // A plot's element: its head, its chart and its tooltip; clicking it makes it active.
  function plotShell(plot) {
    const el = document.createElement('section');
    el.className = 'res-plot';
    el.dataset.plot = plot.id;
    el.innerHTML = '<div class="res-plot-head"></div><div class="res-chart">' +
      '<svg class="res-svg"></svg><div class="res-tip" hidden></div></div>';
    bindChart(plot.id, el);
    el.addEventListener('pointerdown', () => {
      if (R.active !== plot.id) {
        R.active = plot.id;
        markActive();
        renderSignals();
      }
    }, true);
    return el;
  }

  // The active plot is outlined (when there are several).
  function markActive() {
    for (const el of $$('.res-plot', plotsEl)) {
      el.classList.toggle('active', Number(el.dataset.plot) === R.active && R.plots.length > 1);
    }
  }

  // A plot's head: its x choice (time, or a signal of its datasets), Equal, a mixed-units
  // warning, Zoom Out, the axis limits, close, and its legend.
  function renderHead(plot, el) {
    const head = $('.res-plot-head', el);
    const units = [...new Set(plot.traces.map(t => unitOf(signalInfo(t))))];
    const xOptions = new Set();
    for (const t of plot.traces) {
      for (const s of (store.byId(t.id) || { signals: [] }).signals) {
        if (!s.zero || (plot.x && plot.x.signal === s.name)) {
          xOptions.add(s.name);
        }
      }
    }
    if (plot.x) {
      xOptions.add(plot.x.signal);
    }
    const legend = plot.traces.map((t, k) => `<span class="res-key" title="${esc(`${t.signal} · column ${t.col} · ${dsName(t.id)}`)}">
        <i style="background:${PALETTE[t.slot]}"></i>${esc(traceLabel(t))}<span class="muted"> · ${esc(dsName(t.id))}</span>
        <button type="button" data-remove="${k}" title="Remove this line">×</button></span>`).join('');
    head.innerHTML = `
      <div class="res-head-row">
        <span class="res-plot-title">Plot ${R.plots.indexOf(plot) + 1}</span>
        <label class="res-x" title="Plot against time, or against another signal of the same dataset (x-y)">x
          <select class="input">${['<option value="">Time [s]</option>', ...[...xOptions].sort().map(n => `<option value="${esc(n)}"${plot.x && plot.x.signal === n ? ' selected' : ''}>${esc(n)}</option>`)].join('')}</select></label>
        ${plot.x ? `<label class="toggle" title="Same scale on both axes (true shapes on the table)"><input type="checkbox" data-equal${plot.equal ? ' checked' : ''}><span></span>Equal</label>` : ''}
        ${units.length > 1 ? `<span class="pill warn" title="These lines share one y axis: put signals with different units in separate plots">Mixed Units: ${esc(units.map(u => u || 'none').join(', '))}</span>` : ''}
        <span class="res-spacer"></span>
        ${plot.zoom ? '<button type="button" class="btn btn-sm" data-unzoom title="Show everything (or double-click the plot)">Zoom Out</button>' : ''}
        <details class="res-ranges"><summary class="btn btn-sm" title="Set the axis limits">Axes</summary>
          <div class="res-ranges-body">
            <span>x</span><input class="input" data-r="x0" type="number" step="any"><input class="input" data-r="x1" type="number" step="any">
            <span>y</span><input class="input" data-r="y0" type="number" step="any"><input class="input" data-r="y1" type="number" step="any">
            <button type="button" class="btn btn-sm" data-apply>Apply</button><button type="button" class="btn btn-sm" data-auto>Auto</button>
          </div></details>
        ${R.plots.length > 1 ? '<button type="button" class="btn btn-ghost btn-sm" data-close title="Remove this plot">✕</button>' : ''}
      </div>
      <div class="res-legend">${legend || '<span class="hint">Choose signals on the left to plot them here.</span>'}</div>`;
    markActive();
  }

  // A plot's head buttons: remove a line, close the plot, zoom out, fill in or apply the axis
  // limits, or go back to automatic limits.
  plotsEl.addEventListener('click', ev => {
    const el = ev.target.closest('.res-plot');
    if (!el) {
      return;
    }
    const plot = R.plots.find(p => p.id === Number(el.dataset.plot));
    const rm = ev.target.closest('[data-remove]');
    if (rm) {
      plot.traces.splice(Number(rm.dataset.remove), 1);
      save();
      renderSignals();
      return renderPlots();
    }
    if (ev.target.closest('[data-close]')) {
      R.plots = R.plots.filter(p => p !== plot);
      if (R.active === plot.id) {
        R.active = R.plots[0].id;
      }
      save();
      renderSignals();
      return renderPlots();
    }
    if (ev.target.closest('[data-unzoom]')) {
      return zoomTo(plot, null);
    }

    // Opening the Axes box: fill in what is shown now.
    const ranges = ev.target.closest('.res-ranges');
    if (ranges && ev.target.closest('summary')) {
      const v = plot.view || {};
      const set = (k, x) => {
        $(`[data-r="${k}"]`, ranges).value = x == null ? '' : Number(fmt(x, 6));
      };
      set('x0', v.x && v.x[0]);
      set('x1', v.x && v.x[1]);
      set('y0', v.y && v.y[0]);
      set('y1', v.y && v.y[1]);
    }
    if (ev.target.closest('[data-apply]')) {
      const g = k => Number($(`[data-r="${k}"]`, ranges).value);
      const x = [g('x0'), g('x1')];
      const y = [g('y0'), g('y1')];
      if (!(x[1] > x[0]) || !(y[1] > y[0])) {
        return toast('Give each axis a minimum below its maximum.', 'error');
      }
      ranges.open = false;
      return zoomTo(plot, { x, y });
    }
    if (ev.target.closest('[data-auto]')) {
      ranges.open = false;
      return zoomTo(plot, null);
    }
  });

  // A plot's x choice (zoom reset) and Equal.
  plotsEl.addEventListener('change', ev => {
    const el = ev.target.closest('.res-plot');
    if (!el) {
      return;
    }
    const plot = R.plots.find(p => p.id === Number(el.dataset.plot));
    if (ev.target.matches('.res-x select')) {
      plot.x = ev.target.value ? { signal: ev.target.value, col: 1 } : null;
      plot.zoom = null;
      save();
      renderPlots();
    } else if (ev.target.matches('[data-equal]')) {
      plot.equal = ev.target.checked;
      save();
      draw(plot, el);
    }
  });

  // Add Plot (made active); Link time; Smooth.
  $('#resAddPlot').onclick = () => {
    if (R.plots.length >= MAX_PLOTS) {
      return;
    }
    const p = newPlot();
    R.plots.push(p);
    R.active = p.id;
    save();
    renderSignals();
    renderPlots();
  };
  const link = $('#resLink');
  const smooth = $('#resSmooth');
  link.checked = R.link;
  link.onchange = () => {
    R.link = link.checked;
    storage.set('spot.res.link', R.link);
  };
  smooth.value = String(R.smooth);
  smooth.onchange = () => {
    R.smooth = Number(smooth.value);
    storage.set('spot.res.smooth', R.smooth);
    renderPlots();
  };

  // Data

  // Asks MATLAB for a plot's lines it doesn't have yet (thinned to about two points per pixel),
  // and for a zoomed stretch of a long (thinned) signal in detail; draws it before and after.
  async function refreshPlot(plot, el) {
    const want = plot.traces.filter(t => store.byId(t.id));
    const points = Math.max(600, Math.min(6000, Math.round(plotsEl.clientWidth * 2)));
    const missing = want.filter(t => !R.data.has(traceKey(t, plot)));
    const zoomed = plot.zoom && !plot.x ? want.filter(t => {
      const full = R.data.get(traceKey(t, plot));
      return full && full.n > full.t.length &&
        !R.data.has(traceKey(t, plot) + '@' + plot.zoom.x.join());
    }) : [];
    if (missing.length || zoomed.length) {
      el.classList.add('loading');
      draw(plot, el);
      try {
        const req = t =>
          ({ id: t.id, signal: t.signal, col: t.col, x: plot.x, smooth: R.smooth, points });
        const items = [
          ...missing.map(req), ...zoomed.map(t => ({ ...req(t), range: plot.zoom.x })),
        ];
        const out = await store.series(items);
        missing.forEach((t, k) => R.data.set(traceKey(t, plot), clean(out[k])));
        zoomed.forEach((t, k) => R.data.set(traceKey(t, plot) + '@' + plot.zoom.x.join(),
          clean(out[missing.length + k])));
      } catch (e) {
        toast(e.message, 'error', 8000);
      } finally {
        el.classList.remove('loading');
      }

      // Keep the newest 300 series.
      if (R.data.size > 300) {
        for (const k of [...R.data.keys()].slice(0, R.data.size - 300)) {
          R.data.delete(k);
        }
      }
    }
    draw(plot, el);
  }

  // A series' arrays as arrays (MATLAB unwraps one-element ones).
  const clean = r => (r && !r.error
    ? { ...r, t: list(r.t), y: list(r.y), x: r.x === undefined ? undefined : list(r.x) }
    : r);

  // A line's data: the zoomed stretch's if there is one, else the full span's.
  function seriesFor(plot, t) {
    const full = R.data.get(traceKey(t, plot));
    if (plot.zoom && !plot.x) {
      return R.data.get(traceKey(t, plot) + '@' + plot.zoom.x.join()) || full;
    }
    return full;
  }

  // Drawing

  // Draws a plot: its grid and axes, its lines, their names at the right end, and the hover and
  // zoom layers.
  function draw(plot, el) {
    const svg = $('svg', el);
    const W = svg.clientWidth;
    const H = svg.clientHeight;
    svg.textContent = '';
    if (!W || !H) {
      return;
    }
    svg.setAttribute('viewBox', `0 0 ${W} ${H}`);
    svg.setAttribute('font-family', getComputedStyle(document.body).fontFamily);
    svg.setAttribute('font-size', '11');

    // The lines with data; a line whose data failed is marked in the legend.
    const xy = Boolean(plot.x);
    const lines = plot.traces.map(t => ({ t, s: seriesFor(plot, t) }))
      .filter(l => l.s && !l.s.error);
    for (const t of plot.traces) {
      const s = seriesFor(plot, t);
      if (s && s.error) {
        $$('.res-key', el)[plot.traces.indexOf(t)]?.classList.add('bad');
      }
    }
    const X = l => (xy ? l.s.x : l.s.t);

    // Domains: the zoom, or everything (y: what is inside the x range), with a margin.
    let x0 = Infinity;
    let x1 = -Infinity;
    for (const l of lines) {
      for (const v of X(l)) {
        if (v != null && isFinite(v)) {
          if (v < x0) {
            x0 = v;
          }
          if (v > x1) {
            x1 = v;
          }
        }
      }
    }
    if (plot.zoom) {
      [x0, x1] = plot.zoom.x;
    }
    let y0 = Infinity;
    let y1 = -Infinity;
    for (const l of lines) {
      const xs = X(l);
      l.s.y.forEach((v, i) => {
        if (v != null && isFinite(v) && xs[i] >= x0 && xs[i] <= x1) {
          if (v < y0) {
            y0 = v;
          }
          if (v > y1) {
            y1 = v;
          }
        }
      });
    }
    if (plot.zoom && plot.zoom.y) {
      [y0, y1] = plot.zoom.y;
    }
    if (!isFinite(x0)) {
      x0 = 0;
      x1 = 1;
    }
    if (!isFinite(y0)) {
      y0 = 0;
      y1 = 1;
    }
    if (x1 === x0) {
      x0 -= 0.5;
      x1 += 0.5;
    }
    if (y1 === y0) {
      const d = Math.abs(y0) * 0.1 || 0.5;
      y0 -= d;
      y1 += d;
    } else if (!(plot.zoom && plot.zoom.y)) {
      const pad = (y1 - y0) * 0.06;
      y0 -= pad;
      y1 += pad;
    }

    // The plot area (room for the line names on the right with up to four lines).
    const right = xy ? 18 : (plot.traces.length <= 4 ? M.r : 18);
    const pw = Math.max(10, W - M.l - right);
    const ph = Math.max(10, H - M.t - M.b);
    if (xy && plot.equal) {
      // Same metres per pixel on both axes: widen whichever range is too narrow.
      const sx = (x1 - x0) / pw;
      const sy = (y1 - y0) / ph;
      const s = Math.max(sx, sy);
      const cx = (x0 + x1) / 2;
      const cy = (y0 + y1) / 2;
      x0 = cx - s * pw / 2;
      x1 = cx + s * pw / 2;
      y0 = cy - s * ph / 2;
      y1 = cy + s * ph / 2;
    }
    plot.view = { x: [x0, x1], y: [y0, y1], pw, ph, xy };
    const sx = v => M.l + (v - x0) / (x1 - x0) * pw;
    const sy = v => M.t + ph - (v - y0) / (y1 - y0) * ph;
    plot.view.sx = sx;
    plot.view.sy = sy;

    // Grid and axes.
    const g = svgEl('g');
    for (const v of niceTicks(y0, y1, Math.max(2, Math.round(ph / 46)))) {
      g.append(svgEl('line', {
        x1: M.l, x2: M.l + pw, y1: sy(v), y2: sy(v), stroke: v === 0 ? INK.axis : INK.grid,
        'stroke-width': 1,
      }));
      const tx = svgEl('text', {
        x: M.l - 8, y: sy(v) + 4, 'text-anchor': 'end', fill: INK.muted,
        'font-variant-numeric': 'tabular-nums',
      });
      tx.textContent = fmt(v, 4);
      g.append(tx);
    }
    for (const v of niceTicks(x0, x1, Math.max(2, Math.round(pw / 90)))) {
      if (xy) {
        g.append(svgEl('line', {
          x1: sx(v), x2: sx(v), y1: M.t, y2: M.t + ph, stroke: v === 0 ? INK.axis : INK.grid,
        }));
      }
      g.append(svgEl('line', {
        x1: sx(v), x2: sx(v), y1: M.t + ph, y2: M.t + ph + 4, stroke: INK.axis,
      }));
      const tx = svgEl('text', {
        x: sx(v), y: M.t + ph + 17, 'text-anchor': 'middle', fill: INK.muted,
        'font-variant-numeric': 'tabular-nums',
      });
      tx.textContent = fmt(v, 4);
      g.append(tx);
    }
    g.append(svgEl('line', {
      x1: M.l, x2: M.l + pw, y1: M.t + ph, y2: M.t + ph, stroke: INK.axis,
    }));
    const xl = svgEl('text', { x: M.l + pw, y: H - 2, 'text-anchor': 'end', fill: INK.muted });
    xl.textContent = xy ? ((xInfo(plot) || {}).label || plot.x.signal) : 'Time [s]';
    g.append(xl);

    // The y axis's unit, if the lines share one.
    const units = [...new Set(plot.traces.map(t => unitOf(signalInfo(t))))].filter(Boolean);
    if (units.length === 1) {
      const yl = svgEl('text', {
        x: 12, y: M.t + ph / 2, 'text-anchor': 'middle', fill: INK.muted,
        transform: `rotate(-90 12 ${M.t + ph / 2})`,
      });
      yl.textContent = units[0];
      g.append(yl);
    }
    svg.append(g);

    // Lines, clipped to the plot area; a missing value breaks the line (and in an x-y plot,
    // so does (0, 0): a platform PhaseSpace hadn't seen yet).
    const clipId = `clip${plot.id}`;
    const defs = svgEl('defs');
    const clip = svgEl('clipPath', { id: clipId });
    clip.append(svgEl('rect', { x: M.l, y: M.t - 1, width: pw, height: ph + 2 }));
    defs.append(clip);
    svg.append(defs);
    const lg = svgEl('g', { 'clip-path': `url(#${clipId})` });
    for (const l of lines) {
      const xs = X(l);
      let d = '';
      let pen = false;
      l.s.y.forEach((v, i) => {
        const x = xs[i];
        if (v == null || x == null || !isFinite(v) || !isFinite(x) || (xy && x === 0 && v === 0)) {
          pen = false;
          return;
        }
        d += `${pen ? 'L' : 'M'}${sx(x).toFixed(1)} ${sy(v).toFixed(1)}`;
        pen = true;
      });
      lg.append(svgEl('path', {
        d, fill: 'none', stroke: PALETTE[l.t.slot], 'stroke-width': 2, 'stroke-linejoin': 'round',
        'stroke-linecap': 'round',
      }));
    }
    svg.append(lg);

    // Line names at the right end (up to four lines), pushed apart so they don't overlap.
    if (!xy && plot.traces.length <= 4 && lines.length) {
      const ends = lines.map(l => {
        let k = l.s.t.length - 1;
        while (k > 0 && (l.s.t[k] > x1 || l.s.y[k] == null)) {
          k--;
        }
        return { l, y: Math.min(Math.max(sy(l.s.y[k]), M.t + 6), M.t + ph - 2) };
      }).sort((a, b) => a.y - b.y);
      for (let k = 1; k < ends.length; k++) {
        ends[k].y = Math.max(ends[k].y, ends[k - 1].y + 13);
      }
      const over = ends.length ? ends[ends.length - 1].y - (M.t + ph) : 0;
      if (over > 0) {
        for (const e of ends) {
          e.y -= over;
        }
      }

      // The signal's name, or the dataset's when the same signal is there from several datasets.
      const short = t =>
        t.signal.replace(/^(RED|BLACK|BLUE|ARM)_/, (m, k) => `${platformName(k)} `) +
        (t.col > 1 ? `[${t.col}]` : '');
      const twice = new Set(lines.map(l => short(l.t)).filter((n, k, a) => a.indexOf(n) !== k));
      for (const e of ends) {
        svg.append(svgEl('line', {
          x1: M.l + pw + 4, x2: M.l + pw + 14, y1: e.y, y2: e.y, stroke: PALETTE[e.l.t.slot],
          'stroke-width': 2,
        }));
        const tx = svgEl('text', { x: M.l + pw + 18, y: e.y + 4, fill: INK.text });
        const name = twice.has(short(e.l.t)) ? dsName(e.l.t.id) : short(e.l.t);
        tx.textContent = name.length > 14 ? name.slice(0, 13) + '…' : name;
        svg.append(tx);
      }
    }

    // Hover layer, and the zoom selection.
    svg.append(svgEl('g', { class: 'res-hover' }));
    svg.append(svgEl('rect', {
      class: 'res-sel', x: 0, y: 0, width: 0, height: 0, fill: '#4c8dff', 'fill-opacity': 0.12,
      stroke: '#4c8dff', 'stroke-opacity': 0.5,
    }));
    if (R.hoverT != null && !xy) {
      crosshair(plot, el, R.hoverT, false);
    }
  }

  // Hover and zoom

  // The index of the sample time nearest T (binary search); -1 if none.
  function nearest(ts, t) {
    let lo = 0;
    let hi = ts.length - 1;
    if (hi < 0) {
      return -1;
    }
    while (hi - lo > 1) {
      const mid = (lo + hi) >> 1;
      if (ts[mid] <= t) {
        lo = mid;
      } else {
        hi = mid;
      }
    }
    return Math.abs(ts[hi] - t) < Math.abs(ts[lo] - t) ? hi : lo;
  }

  // A time plot's crosshair at time T: a line, a dot on each line, and (with the tip) each
  // line's value.
  function crosshair(plot, el, t, withTip, px) {
    const v = plot.view;
    const layer = $('.res-hover', el);
    const tip = $('.res-tip', el);
    if (!v || !layer) {
      return;
    }
    layer.textContent = '';
    if (t == null || t < v.x[0] || t > v.x[1]) {
      tip.hidden = true;
      return;
    }
    layer.append(svgEl('line', {
      x1: v.sx(t), x2: v.sx(t), y1: M.t, y2: M.t + v.ph, stroke: INK.muted, 'stroke-width': 1,
    }));
    const rows = [];
    for (const tr of plot.traces) {
      const s = seriesFor(plot, tr);
      if (!s || s.error) {
        continue;
      }
      const i = nearest(s.t, t);
      const y = i >= 0 ? s.y[i] : null;
      if (y != null && y >= v.y[0] && y <= v.y[1]) {
        layer.append(svgEl('circle', {
          cx: v.sx(s.t[i]), cy: v.sy(y), r: 4.5, fill: PALETTE[tr.slot], stroke: INK.surface,
          'stroke-width': 2,
        }));
      }
      rows.push({ tr, y });
    }
    if (!withTip) {
      tip.hidden = true;
      return;
    }
    showTip(el, tip, px, `<div class="res-tip-head">t = ${fmt(t, 6)} s</div>` + rows.map(r =>
      `<div class="res-tip-row"><i style="background:${PALETTE[r.tr.slot]}"></i>` +
      `<b>${fmt(r.y, 6)}</b><span>${esc(traceLabel(r.tr))} · ${esc(dsName(r.tr.id))}</span></div>`)
      .join(''));
  }

  // Shows the tooltip beside the pointer (on the left if it would leave the chart).
  function showTip(el, tip, px, html) {
    tip.innerHTML = html;
    tip.hidden = false;
    const box = $('.res-chart', el).getBoundingClientRect();
    const w = tip.offsetWidth;
    tip.style.left = `${px.x + 16 + w > box.width ? px.x - 16 - w : px.x + 16}px`;
    tip.style.top = `${Math.max(4, Math.min(px.y - 10, box.height - tip.offsetHeight - 4))}px`;
  }

  // An x-y plot's hover: the sample nearest the pointer (within 60 px), with its values and time.
  function hoverXY(plot, el, px) {
    const v = plot.view;
    const layer = $('.res-hover', el);
    const tip = $('.res-tip', el);
    layer.textContent = '';
    let best = null;
    for (const tr of plot.traces) {
      const s = seriesFor(plot, tr);
      if (!s || s.error || !s.x) {
        continue;
      }
      s.y.forEach((y, i) => {
        const x = s.x[i];
        if (x == null || y == null) {
          return;
        }
        const d = (v.sx(x) - px.x) ** 2 + (v.sy(y) - px.y) ** 2;
        if (!best || d < best.d) {
          best = { d, tr, i, x, y, t: s.t[i] };
        }
      });
    }
    if (!best || best.d > 60 ** 2) {
      tip.hidden = true;
      return;
    }
    layer.append(svgEl('circle', {
      cx: v.sx(best.x), cy: v.sy(best.y), r: 5, fill: PALETTE[best.tr.slot], stroke: INK.surface,
      'stroke-width': 2,
    }));
    showTip(el, tip, px, `<div class="res-tip-row"><i style="background:${PALETTE[best.tr.slot]}"></i><b>${fmt(best.y, 6)}</b><span>${esc(traceLabel(best.tr))}</span></div>
      <div class="res-tip-row"><i></i><b>${fmt(best.x, 6)}</b><span>${esc(plot.x.signal)}</span></div>
      <div class="res-tip-row"><i></i><b>${fmt(best.t, 6)} s</b><span>${esc(dsName(best.tr.id))}</span></div>`);
  }

  // A chart's pointer: hover (linked across time plots), drag to zoom (a box in x-y plots, a
  // time span otherwise), double-click to zoom out.
  function bindChart(id, el) {
    const svg = $('svg', el);
    const plotOf = () => R.plots.find(p => p.id === id);
    let drag = null;

    // The pointer in the chart [px]; whether it is in the plot area; its time and value.
    const at = ev => {
      const r = svg.getBoundingClientRect();
      return { x: ev.clientX - r.left, y: ev.clientY - r.top };
    };
    const inside = (v, p) => p.x >= M.l && p.x <= M.l + v.pw && p.y >= M.t && p.y <= M.t + v.ph;
    const tOf = (v, p) => v.x[0] + (p.x - M.l) / v.pw * (v.x[1] - v.x[0]);
    const yOf = (v, p) => v.y[0] + (M.t + v.ph - p.y) / v.ph * (v.y[1] - v.y[0]);

    // Moving: stretch the zoom selection, or hover.
    svg.addEventListener('pointermove', ev => {
      const plot = plotOf();
      const v = plot && plot.view;
      if (!v) {
        return;
      }
      const p = at(ev);
      if (drag) {
        const sel = $('.res-sel', el);
        const xa = Math.max(M.l, Math.min(drag.x, p.x));
        const xb = Math.min(M.l + v.pw, Math.max(drag.x, p.x));
        const ya = v.xy ? Math.max(M.t, Math.min(drag.y, p.y)) : M.t;
        const yb = v.xy ? Math.min(M.t + v.ph, Math.max(drag.y, p.y)) : M.t + v.ph;
        Object.entries({ x: xa, y: ya, width: xb - xa, height: yb - ya })
          .forEach(([k, val]) => sel.setAttribute(k, val));
        return;
      }
      if (!inside(v, p)) {
        return leave();
      }
      if (v.xy) {
        return hoverXY(plot, el, p);
      }
      const t = tOf(v, p);
      R.hoverT = t;
      crosshair(plot, el, t, true, p);
      if (R.link) {
        for (const other of R.plots) {
          if (other !== plot && !other.x) {
            crosshair(other, plotsEl.querySelector(`[data-plot="${other.id}"]`), t, false);
          }
        }
      }
    });

    // Leaving: clear every plot's hover.
    const leave = () => {
      R.hoverT = null;
      for (const other of R.plots) {
        const oel = plotsEl.querySelector(`[data-plot="${other.id}"]`);
        if (oel && $('.res-hover', oel)) {
          $('.res-hover', oel).textContent = '';
          $('.res-tip', oel).hidden = true;
        }
      }
    };
    svg.addEventListener('pointerleave', () => {
      if (!drag) {
        leave();
      }
    });

    // Pressing in the plot area starts a zoom selection (synthetic events can't capture).
    svg.addEventListener('pointerdown', ev => {
      const plot = plotOf();
      const v = plot && plot.view;
      if (ev.button !== 0 || !v || !plot.traces.length) {
        return;
      }
      const p = at(ev);
      if (!inside(v, p)) {
        return;
      }
      drag = p;
      try {
        svg.setPointerCapture(ev.pointerId);
      } catch {
        // Synthetic events.
      }
    });

    // Releasing zooms to the selection (unless it is under 6 px: a click).
    svg.addEventListener('pointerup', ev => {
      if (!drag) {
        return;
      }
      const plot = plotOf();
      const v = plot.view;
      const p = at(ev);
      const start = drag;
      drag = null;
      $('.res-sel', el).setAttribute('width', 0);
      if (Math.abs(p.x - start.x) < 6 && (!v.xy || Math.abs(p.y - start.y) < 6)) {
        return;
      }
      const clampP = q => ({
        x: Math.max(M.l, Math.min(M.l + v.pw, q.x)), y: Math.max(M.t, Math.min(M.t + v.ph, q.y)),
      });
      const a = clampP(start);
      const b = clampP(p);
      const x = [tOf(v, a), tOf(v, b)].sort((m, n) => m - n);
      zoomTo(plot, v.xy ? { x, y: [yOf(v, a), yOf(v, b)].sort((m, n) => m - n) } : { x });
    });
    svg.addEventListener('dblclick', () => zoomTo(plotOf(), null));
  }

  // Zooms a plot (null: everything); with Link time, every time plot takes its time span.
  function zoomTo(plot, zoom) {
    const targets = !plot.x && R.link ? R.plots.filter(p => !p.x) : [plot];
    for (const p of targets) {
      p.zoom = zoom && (p === plot || !zoom.y) ? zoom : zoom ? { x: zoom.x } : null;
    }
    renderPlots();
  }

  // Redraw every plot when the page's width changes (while shown).
  new ResizeObserver(() => {
    if (R.shown) {
      for (const plot of R.plots) {
        const el = plotsEl.querySelector(`[data-plot="${plot.id}"]`);
        if (el) {
          draw(plot, el);
        }
      }
    }
  }).observe(plotsEl);

  // Export

  // Everything is drawn and saved by MATLAB (spot.data.toFigure / exportFigure), in the lab's
  // publication style, so a figure for a paper never comes from the browser. The size [in] and
  // font size [pt] are remembered.
  const expW = $('#resExpW');
  const expH = $('#resExpH');
  const expF = $('#resExpF');
  const expOpts = storage.get('spot.res.export', { width: 7, height: null, fontSize: 10 });
  expW.value = expOpts.width;
  expH.value = expOpts.height || '';
  expF.value = expOpts.fontSize;
  for (const el of [expW, expH, expF]) {
    el.onchange = () => storage.set('spot.res.export', exportOptions());
  }
  function exportOptions() {
    const n = v => (v === '' || !isFinite(Number(v)) ? null : Number(v));
    return { width: n(expW.value) || 7, height: n(expH.value), fontSize: n(expF.value) || 10 };
  }

  // The plots with lines, as MATLAB's figure panels.
  function figurePanels() {
    return R.plots.filter(p => p.traces.length).map(p => ({
      x: p.x, smooth: R.smooth,
      xlim: p.zoom ? p.zoom.x : [], ylim: p.zoom && p.zoom.y ? p.zoom.y : [],
      traces: p.traces.map(t => ({ id: t.id, signal: t.signal, col: t.col })),
    }));
  }

  // Opens the figure in MATLAB, or saves it (MATLAB asks where).
  async function exportFigure(save) {
    const panels = figurePanels();
    if (!panels.length) {
      return toast('Nothing plotted yet.', 'info');
    }
    $('.res-export').open = false;
    if (save) {
      toast('Choose where to save in MATLAB\'s dialog', 'info', 4000);
    }
    const opts = exportOptions();
    if (opts.height == null) {
      delete opts.height;
    }
    const r = await store.figure(panels, opts, save);
    if (r.cancelled) {
      return;
    }
    toast(save ? `Saved ${r.file}` : `Opened Figure ${r.figure} in MATLAB`, 'success', 7000);
  }
  $('#resSaveFig').onclick = () => act(() => exportFigure(true));
  $('#resFigure').onclick = () => act(() => exportFigure(false));

  // The export box closes on a click outside it.
  document.addEventListener('pointerdown', ev => {
    const d = $('.res-export');
    if (d.open && !ev.target.closest('.res-export')) {
      d.open = false;
    }
  });

  // Store and page

  // A dataset closed elsewhere (or MATLAB restarted): drop its lines.
  store.on(() => {
    let changed = false;
    for (const p of R.plots) {
      const keep = p.traces.filter(t => store.byId(t.id));
      if (keep.length !== p.traces.length) {
        p.traces = keep;
        changed = true;
      }
    }
    if (changed) {
      save();
    }
    renderDatasets();
    if (R.shown) {
      renderPlots();
    }
  });

  return {
    // The page is shown; the datasets are read the first time.
    show() {
      R.shown = true;
      renderDatasets();
      renderPlots();
      if (!R.loaded) {
        R.loaded = true;
        act(() => store.refresh());
      }
    },
    hide() {
      R.shown = false;
    },
    // Datasets live in MATLAB: a new MATLAB session has none.
    onMatlab(m) {
      if (m.connected && !R.wasConnected) {
        R.data.clear();
        act(() => store.refresh());
      }
      R.wasConnected = m.connected;
      $('#resMatlab').hidden = m.connected;
      for (const b of ['#resLoadSaved', '#resLoadFile', '#resFigure', '#resSaveFig']) {
        $(b).disabled = !m.connected;
      }
    },
    onSimFinished() {
      store.refresh().catch(() => {});
    },
  };
}
