# results.js and data.js

**Files:** `+spot/+console/webapp/static/results.js`, `data.js` · **Tab:** [[Results Tab|Results-Tab]] · **MATLAB side:** [[spot.data|spot-data]]

<!-- source: +spot/+console/webapp/static/data.js -->
<!-- source: +spot/+console/webapp/static/results.js -->

## What they are for

`data.js` is the **data store**: the loaded datasets (which live in MATLAB; the page only keeps their summaries), the Saved Data dialog, and cached requests for series and trajectories. It is shared by the Results tab (plots) and the Simulation tab (Compare overlays).

`results.js` is the **Results tab**: up to six plots, each one y axis with up to eight lines (a column of a signal of a dataset), against time or against another signal (x–y, e.g. a trajectory). Drag to zoom, double-click to zoom out; with *Link Time* the time plots zoom and hover together. Export draws the same plots in MATLAB in the lab's publication style.

## `data.js`, function by function

**Lines 11–14.** Helpers and the kind tags (SIM / EXP / MAT).

**Lines 17–36.** `createDataStore`: the state (saved runs, loaded datasets, whether a last simulation exists, listeners, caches, overlay switches) and `addLoaded`/`normalize`.

**Lines 38–107.** The store: `refresh` (`data.list`: the saved runs, the loaded datasets, whether there is a simulation; drop caches of datasets that are gone), `load(spec)` (`data.load`; a cancelled dialog returns nothing), `unload`, `series(items)` (several signals in one `data.series` request, cached by request, at most 400), `trajectory(id)` (cached promise), `keepSimulation`, `saveSimulation` (`data.saveSimulation`, then refresh), `figure` (`data.figure`), `openFile` (MATLAB's dialog), `pick` (the Saved Data dialog).

**Lines 111–184.** The Saved Data dialog: a run per block (kind, label, folder name, date) with a button per item (a run number with its platforms, a file, or *Convert ExpLog Files* which runs `experiment.decode` and refreshes); choosing loads it with the run's settings; *Other .mat File…*; ↻; `open` returns a promise resolved with what was loaded.

## `results.js`, function by function

**Lines 13–37.** The palette (colour-vision safe), ink colours, the signal groups (RED, BLACK, BLUE, ARM, Other), limits (8 lines, 6 plots), margins, formatting, `niceTicks`.

**Lines 39–53.** `initResults`: the plots (restored from storage, zoom cleared), the active plot, the selected dataset, Link Time, smoothing, the data cache.

**Lines 56–138.** Datasets and signals: `renderDatasets` (a row per loaded dataset: kind, name, platforms, duration, *3D ▸* to replay it on the Simulation tab, ✕ to unload); `renderSignals` (the selected dataset's signals grouped by platform in collapsible groups, each a button, with column buttons for multi-column signals, greyed when all zero, filtered by the text box); clicks toggle traces, unload or replay a dataset, select a dataset; the three load buttons.

**Lines 142–165.** Trace helpers (labels, signal info, the cache key) and `toggleTrace` (add or remove a line in the active plot, giving it a free colour slot).

**Lines 167–280.** The plots: `renderPlots` (one `<section>` per plot, kept between renders), `plotShell`, `markActive`, `renderHead` (title, the x selector with time or any signal of the datasets in the plot, *Equal* for x–y, a Mixed Units warning, Zoom Out, the Axes limits form, close, the legend with remove buttons); the click/change handlers for all of those; *+ Plot*, Link Time, Smoothing.

**Lines 283–316.** `refreshPlot`: ask MATLAB for the series that aren't cached (at about two points per pixel), and when zoomed into a thinned signal, for that stretch in detail; `seriesFor` picks the zoomed series when there is one.

**Lines 320–437.** `draw` (SVG): the domains (zoom or everything; y over what is in the x range; padding; equal scales for x–y), the grid and tick labels, the axis labels (units from MATLAB), the lines as paths clipped to the plot area (a missing value or an x–y (0, 0) breaks the line), line names at the right end for up to four lines (pushed apart), the hover layer and the zoom selection rectangle.

**Lines 441–499.** Hover: `crosshair` (the nearest sample of each line at a time, a dot each, and a tooltip with the values; linked across time plots), `showTip`, `hoverXY` (the nearest point in an x–y plot).

**Lines 501–561.** `bindChart`: pointer events on the SVG for hover, drag-to-zoom (x only on time plots, a box on x–y) and double-click to zoom out; `zoomTo` applies to the linked plots.

**Lines 567–600.** Export: the width/height/font options (remembered), the panels (each plot's traces, x, smoothing, zoom as limits), `exportFigure` (`data.figure` with `save` for *Save PDF…* or without for *Open in MATLAB*).

**Lines 604–633.** Follow the store (drop lines of unloaded datasets), `show` (first show refreshes the list), `hide`, `onMatlab` (a new MATLAB session has no datasets: clear the cache), `onSimFinished`.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/data.js</b> (187 lines; click to show)</summary>

```javascript
   1  // Loaded results (local console), shared by the Results page (plots) and the Simulation tab
   2  // (3D playback with overlays). The data stays in MATLAB (spot.worker.Worker keeps each loaded
   3  // dataset under an id); this page asks for what it draws: signals thinned to the plot's width
   4  // (data.series) and the platforms' motion (data.trajectory).
   5  //
   6  // Also the "Saved Data" dialog, from which both pages load runs.
   7  
   8  import { platformName } from './platforms.js';
   9  
  10  const $ = (sel, el = document) => el.querySelector(sel);
  11  export const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
  12  export const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  13  const KIND = { experiment: 'EXP', legacy: 'EXP', simulation: 'SIM', files: 'MAT', file: 'MAT' };
  14  export const kindTag = kind => KIND[kind] || 'MAT';
  15  
  16  
  17  export function createDataStore({ api, toast }) {
  18    const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
  19    const D = {
  20      saved: [], loaded: [], simulation: false, listeners: new Set(),
  21      series: new Map(), trajectories: new Map(),   // caches (by request / by id)
  22      overlay: new Map(),   // id -> {on, offset}: replayed over the simulation (Simulation tab)
  23    };
  24    const emit = () => { for (const fn of D.listeners) fn(); };
  25  
  26    function addLoaded(datasets) {
  27      const got = list(datasets).map(normalize);
  28      D.loaded = [...D.loaded.filter(d => !got.some(g => g.id === d.id)), ...got];
  29      emit();
  30      return got;
  31    }
  32    function normalize(d) {
  33      d.signals = list(d.signals);
  34      d.crafts = list(d.crafts);
  35      return d;
  36    }
  37  
  38    const store = {
  39      get saved() { return D.saved; },
  40      get loaded() { return D.loaded; },
  41      get simulation() { return D.simulation; },
  42      byId: id => D.loaded.find(d => d.id === id),
  43      overlay: id => D.overlay.get(id) || { on: false, offset: 0 },
  44      setOverlay(id, patch) { D.overlay.set(id, { ...store.overlay(id), ...patch }); emit(); },
  45      on(fn) { D.listeners.add(fn); return () => D.listeners.delete(fn); },
  46  
  47      async refresh() {
  48        const r = await matlab('data.list');
  49        D.saved = list(r.saved).map(run => ({ ...run, items: list(run.items) }));
  50        D.loaded = list(r.loaded).map(normalize);
  51        D.simulation = Boolean(r.simulation);
  52        for (const id of [...D.trajectories.keys()]) if (!store.byId(id)) D.trajectories.delete(id);
  53        for (const id of [...D.overlay.keys()]) if (!store.byId(id)) D.overlay.delete(id);
  54        emit();
  55      },
  56  
  57      async load(spec) {
  58        const r = await matlab('data.load', spec);
  59        if (!r || r.cancelled) return [];
  60        const got = addLoaded(r.datasets);
  61        toast(`Loaded ${got.map(d => d.name).join(', ')}`, 'success', 3500);
  62        return got;
  63      },
  64  
  65      async unload(id) {
  66        D.loaded = D.loaded.filter(d => d.id !== id);
  67        D.trajectories.delete(id);
  68        D.overlay.delete(id);
  69        for (const k of [...D.series.keys()]) if (k.startsWith(id + '|')) D.series.delete(k);
  70        emit();
  71        await matlab('data.unload', { id }).catch(() => {});
  72      },
  73  
  74      async series(items) {
  75        // items: [{id, signal, col, x, smooth, range, points}] -> the same order, from the cache when asked before
  76        const key = it => `${it.id}|${JSON.stringify(it)}`;
  77        const missing = items.filter(it => !D.series.has(key(it)));
  78        if (missing.length) {
  79          const out = list(await matlab('data.series', { items: missing }));
  80          missing.forEach((it, k) => D.series.set(key(it), out[k] || { error: 'No answer' }));
  81          while (D.series.size > 400) D.series.delete(D.series.keys().next().value);
  82        }
  83        return items.map(it => D.series.get(key(it)));
  84      },
  85  
  86      async trajectory(id) {
  87        if (!D.trajectories.has(id)) {
  88          const p = matlab('data.trajectory', { id }).then(r => { r.crafts = r.crafts || {}; return r; });
  89          D.trajectories.set(id, p);
  90          p.catch(() => D.trajectories.delete(id));
  91        }
  92        return D.trajectories.get(id);
  93      },
  94  
  95      async keepSimulation() {
  96        return store.load({ source: 'simulation' });
  97      },
  98      async saveSimulation() {
  99        const r = await matlab('data.saveSimulation');
 100        toast(`Saved the simulation in ${r.folder}`, 'success', 6000);
 101        store.refresh().catch(() => {});
 102        return r;
 103      },
 104      figure: (panels, options, save = false) => matlab('data.figure', { panels, options, save }),
 105      openFile: () => { toast('Choose the file in MATLAB\'s dialog', 'info', 4000); return store.load({ dialog: true }); },
 106      pick: opts => picker.open(opts),
 107    };
 108  
 109    // ---------------------------------------------------------------- the Saved Data dialog
 110  
 111    const picker = (() => {
 112      const dlg = $('#resultsDialog');
 113      let resolve = null;
 114      function render() {
 115        const body = $('#resultsList');
 116        if (!D.saved.length) {
 117          body.innerHTML = '<p class="hint">Nothing in Saved Data yet. Experiments collected on the Experiment tab, and simulations saved from the Simulation tab, appear here.</p>';
 118          return;
 119        }
 120        body.innerHTML = D.saved.map((run, r) => {
 121          const when = run.modified ? new Date(run.modified * 1000).toLocaleString([], { month: 'short', day: 'numeric', hour: '2-digit', minute: '2-digit' }) : '';
 122          const items = run.items.map((it, k) => {
 123            if (it.convert) return `<button type="button" class="btn btn-sm" data-convert="${r}" title="Convert the platforms' ExpLog files to .mat with MATLAB (compiles the diagram)">Convert ExpLog Files</button>`;
 124            const plats = Object.keys(it.files || {});
 125            return `<button type="button" class="btn btn-sm" data-run="${r}" data-item="${k}" title="${esc(it.file || Object.values(it.files).join('\n'))}">${esc(it.label)}${plats.length ? ` <span class="muted">${plats.map(platformName).join(' ')}</span>` : ''}</button>`;
 126          }).join('');
 127          return `<div class="saved-run"><div class="saved-head"><span class="pill">${kindTag(run.kind)}</span>
 128            <b>${esc(run.label || run.name)}</b><span class="muted small mono" title="${esc(run.name)}">${esc(run.name)}</span><span class="muted small">${esc(when)}</span></div>
 129            <div class="saved-items">${items}</div></div>`;
 130        }).join('');
 131      }
 132      async function choose(run, it) {
 133        // The dataset's name comes from MATLAB (spot.data.listSaved): "10-07 15:02 · Run 2", or the file's own
 134        const spec = it.file ? { file: it.file } : { files: it.files };
 135        if (it.name) spec.name = it.name;
 136        if (run.settings) spec.settings = run.settings;
 137        const got = await store.load(spec);
 138        if (got.length) finish(got);
 139      }
 140      function finish(got) {
 141        dlg.close();
 142        const r = resolve;
 143        resolve = null;
 144        r?.(got);
 145      }
 146      $('#resultsList').addEventListener('click', async ev => {
 147        const b = ev.target.closest('button');
 148        if (!b) return;
 149        b.disabled = true;
 150        try {
 151          if (b.dataset.convert) {
 152            const run = D.saved[Number(b.dataset.convert)];
 153            b.innerHTML = '<span class="spinner dark"></span> Converting…';
 154            await matlab('experiment.decode', { folder: run.folder });
 155            await store.refresh();
 156            render();
 157          } else {
 158            const run = D.saved[Number(b.dataset.run)];
 159            b.innerHTML = '<span class="spinner dark"></span> Loading…';
 160            await choose(run, run.items[Number(b.dataset.item)]);
 161          }
 162        } catch (e) {
 163          toast(e.message, 'error', 9000);
 164          render();
 165        }
 166      });
 167      $('#resultsOpenFile').onclick = async () => {
 168        try {
 169          const got = await store.openFile();
 170          if (got.length) finish(got);
 171        } catch (e) { toast(e.message, 'error', 9000); }
 172      };
 173      $('#resultsRefresh').onclick = () => store.refresh().then(render).catch(e => toast(e.message, 'error'));
 174      dlg.addEventListener('close', () => { if (resolve) { const r = resolve; resolve = null; r([]); } });
 175      return {
 176        async open() {
 177          $('#resultsList').innerHTML = '<p class="hint"><span class="spinner dark"></span> Reading Saved Data…</p>';
 178          dlg.showModal();
 179          try { await store.refresh(); } catch (e) { toast(e.message, 'error'); }
 180          render();
 181          return new Promise(r => { resolve = r; });
 182        },
 183      };
 184    })();
 185  
 186    return store;
 187  }
```

</details>

<details>
<summary><b>+spot/+console/webapp/static/results.js</b> (633 lines; click to show)</summary>

```javascript
   1  // Results page (local console): load simulation and experiment results and plot their signals,
   2  // in the spirit of the old GUI's Data Viewer. Loaded datasets live in MATLAB (see data.js); each
   3  // plot is one y axis with up to eight lines (a signal column of a dataset each), against time or
   4  // against another signal (x-y, e.g. a trajectory). Drag to zoom, double-click to zoom back out;
   5  // with "Link time" the time plots zoom and hover together.
   6  
   7  import { list, esc, kindTag } from './data.js';
   8  import { COLORS, platformName } from './platforms.js';
   9  
  10  const $ = (sel, el = document) => el.querySelector(sel);
  11  const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
  12  // Categorical lines, in this order (validated for colour-vision deficiency on the plot surface)
  13  const PALETTE = ['#3987e5', '#d95926', '#199e70', '#c98500', '#d55181', '#008300', '#9085e9', '#e66767'];
  14  const INK = { text: '#e6e9ef', muted: '#8b93a3', grid: '#232833', axis: '#3a4150', surface: '#0f1217' };
  15  const SWATCH = { RED: COLORS.RED, BLACK: COLORS.BLACK, BLUE: COLORS.BLUE };
  16  const GROUPS = ['RED', 'BLACK', 'BLUE', 'ARM'];
  17  const MAX_LINES = 8, MAX_PLOTS = 6;
  18  const M = { l: 58, r: 104, t: 10, b: 28 };   // plot margins [px]; the right one holds the line labels
  19  
  20  const fmt = (v, d = 4) => (v == null || !isFinite(v) ? '—' : Number(v).toPrecision(d).replace(/\.?0+(e|$)/, '$1'));
  21  // Units and readable names come with each signal from MATLAB (spot.data.summary, spot.plotting.signalLabel)
  22  const unitOf = info => (info && info.unit) || '';
  23  const svgEl = (tag, attrs = {}) => {
  24    const el = document.createElementNS('http://www.w3.org/2000/svg', tag);
  25    for (const [k, v] of Object.entries(attrs)) el.setAttribute(k, v);
  26    return el;
  27  };
  28  
  29  function niceTicks(lo, hi, n) {
  30    const span = hi - lo;
  31    if (!(span > 0)) return [lo];
  32    const raw = span / Math.max(1, n), p = 10 ** Math.floor(Math.log10(raw));
  33    const step = [1, 2, 2.5, 5, 10].map(k => k * p).find(s => s >= raw) || 10 * p;
  34    const out = [];
  35    for (let v = Math.ceil(lo / step) * step; v <= hi + step * 1e-9; v += step) out.push(Math.abs(v) < step * 1e-9 ? 0 : v);
  36    return out;
  37  }
  38  
  39  export function initResults({ act, toast, storage, store, showPage }) {
  40    let nextPlot = 1;
  41    const newPlot = () => ({ id: nextPlot++, x: null, equal: false, traces: [], zoom: null });
  42    const R = {
  43      plots: storage.get('spot.res.plots', null) || [newPlot()],
  44      active: null, selected: storage.get('spot.res.selected', null),
  45      link: storage.get('spot.res.link', true), smooth: storage.get('spot.res.smooth', 0),
  46      hoverT: null, shown: false, filter: '', loaded: false,
  47      data: new Map(),   // trace key -> series (full span), and key + '@' -> series for the zoomed span
  48    };
  49    R.active = R.plots[0].id;
  50    nextPlot = Math.max(0, ...R.plots.map(p => p.id)) + 1;
  51    const save = () => storage.set('spot.res.plots', R.plots.map(p => ({ ...p, zoom: null })));
  52    const plotsEl = $('#resPlots');
  53  
  54    // ---------------------------------------------------------------- datasets and signals
  55  
  56    function renderDatasets() {
  57      const sets = store.loaded;
  58      if (!sets.some(d => d.id === R.selected)) R.selected = sets.length ? sets[sets.length - 1].id : null;
  59      $('#resLoadSim').disabled = !store.simulation;
  60      $('#resDatasets').innerHTML = sets.length ? sets.map(d => {
  61        const dur = isFinite(d.t1 - d.t0) ? `${fmt(d.t1 - d.t0, 4)} s` : '';
  62        const plats = d.crafts.map(n => `<i class="swatch" title="${platformName(n)}" style="background:${SWATCH[n]}"></i>`).join('');
  63        return `<div class="res-ds${d.id === R.selected ? ' sel' : ''}" data-ds="${d.id}" title="${esc(d.source)}">
  64          <span class="pill">${kindTag(d.kind)}</span><span class="res-ds-name">${esc(d.name)}</span>
  65          <span class="res-ds-meta">${plats}<span class="muted small">${dur}</span></span>
  66          ${d.crafts.length ? `<button type="button" class="btn btn-sm" data-play="${d.id}" title="Replay on the table (Simulation tab), over the simulation">3D ▸</button>` : ''}
  67          <button type="button" class="btn btn-ghost btn-sm" data-unload="${d.id}" title="Close (frees it in MATLAB)">✕</button></div>`;
  68      }).join('') : '<p class="hint">Nothing loaded. Load the last simulation, a run from Saved Data, or any .mat file with logged signals.</p>';
  69      renderSignals();
  70    }
  71  
  72    function renderSignals() {
  73      const d = store.byId(R.selected);
  74      $('#resSigFor').textContent = d ? d.name : '';
  75      const box = $('#resSignals');
  76      if (!d) { box.innerHTML = ''; return; }
  77      const plot = activePlot();
  78      const f = R.filter.toLowerCase();
  79      const groups = {};
  80      for (const s of d.signals) {
  81        if (f && !s.name.toLowerCase().includes(f)) continue;
  82        const g = GROUPS.find(p => s.name.startsWith(p + '_')) || 'Other';
  83        (groups[g] = groups[g] || []).push(s);
  84      }
  85      const inPlot = (name, col) => plot && plot.traces.some(t => t.id === d.id && t.signal === name && t.col === col);
  86      const open = storage.get('spot.res.closedGroups', []);
  87      box.innerHTML = [...GROUPS, 'Other'].filter(g => groups[g]).map(g => `<details data-group="${g}"${open.includes(g) ? '' : ' open'}>
  88        <summary>${SWATCH[g] ? `<i class="swatch" style="background:${SWATCH[g]}"></i>` : ''}${platformName(g)}<span class="muted small">${groups[g].length}</span></summary>
  89        ${groups[g].map(s => {
  90          const short = SWATCH[g] || g === 'ARM' ? s.name.slice(g.length + 1) : s.name;
  91          const cols = s.cols > 1 ? `<span class="res-cols">${Array.from({ length: Math.min(s.cols, 12) }, (_, k) =>
  92            `<button type="button" class="${inPlot(s.name, k + 1) ? 'on' : ''}" data-sig="${esc(s.name)}" data-col="${k + 1}" title="Column ${k + 1}">${k + 1}</button>`).join('')}</span>` : '';
  93          return `<div class="res-sig${s.zero ? ' zero' : ''}${s.cols === 1 && inPlot(s.name, 1) ? ' on' : ''}" title="${esc(s.name)}${s.zero ? ' (all zero)' : ''}${s.cols > 1 ? ` · ${s.cols} columns` : ''}">
  94            <button type="button" class="res-sig-name" data-sig="${esc(s.name)}" data-col="1">${esc(short)}</button>${cols}</div>`;
  95        }).join('')}</details>`).join('') || '<p class="hint">No signal matches.</p>';
  96    }
  97  
  98    $('#resSignals').addEventListener('toggle', ev => {
  99      const det = ev.target.closest('details[data-group]');
 100      if (!det) return;
 101      const closed = new Set(storage.get('spot.res.closedGroups', []));
 102      if (det.open) closed.delete(det.dataset.group); else closed.add(det.dataset.group);
 103      storage.set('spot.res.closedGroups', [...closed]);
 104    }, true);
 105    $('#resSignals').addEventListener('click', ev => {
 106      const b = ev.target.closest('[data-sig]');
 107      if (b) toggleTrace(R.selected, b.dataset.sig, Number(b.dataset.col));
 108    });
 109    $('#resSigFilter').addEventListener('input', ev => { R.filter = ev.target.value.trim(); renderSignals(); });
 110  
 111    $('#resDatasets').addEventListener('click', ev => {
 112      const un = ev.target.closest('[data-unload]');
 113      if (un) {
 114        const id = un.dataset.unload;
 115        for (const p of R.plots) p.traces = p.traces.filter(t => t.id !== id);
 116        save();
 117        act(() => store.unload(id));
 118        return;
 119      }
 120      const play = ev.target.closest('[data-play]');
 121      if (play) {
 122        store.setOverlay(play.dataset.play, { on: true });
 123        showPage('simulation');
 124        return;
 125      }
 126      const row = ev.target.closest('[data-ds]');
 127      if (row) { R.selected = row.dataset.ds; storage.set('spot.res.selected', R.selected); renderDatasets(); }
 128    });
 129  
 130    $('#resLoadSim').onclick = () => act(async () => select(await store.keepSimulation()));
 131    $('#resLoadSaved').onclick = () => act(async () => select(await store.pick()));
 132    $('#resLoadFile').onclick = () => act(async () => select(await store.openFile()));
 133    function select(got) {
 134      if (!got || !got.length) return;
 135      R.selected = got[got.length - 1].id;
 136      storage.set('spot.res.selected', R.selected);
 137      renderDatasets();
 138    }
 139  
 140    // ---------------------------------------------------------------- plots
 141  
 142    const activePlot = () => R.plots.find(p => p.id === R.active) || R.plots[0];
 143    const dsName = id => (store.byId(id) || { name: '?' }).name;
 144    const traceLabel = t => `${t.signal}${t.col > 1 || (signalInfo(t) || {}).cols > 1 ? ` [${t.col}]` : ''}`;
 145    const signalInfo = t => (store.byId(t.id) || { signals: [] }).signals.find(s => s.name === t.signal);
 146    const xInfo = plot => plot.x && plot.traces.length
 147      ? (store.byId(plot.traces[0].id) || { signals: [] }).signals.find(s => s.name === plot.x.signal) : null;
 148    // (A platform's Py against a Px is paired with its own Px by MATLAB: spot.data.series)
 149    const traceKey = (t, plot) => JSON.stringify([t.id, t.signal, t.col, plot.x, R.smooth]);
 150  
 151    function toggleTrace(id, signal, col) {
 152      if (!id) return;
 153      const plot = activePlot();
 154      const k = plot.traces.findIndex(t => t.id === id && t.signal === signal && t.col === col);
 155      if (k >= 0) plot.traces.splice(k, 1);
 156      else {
 157        if (plot.traces.length >= MAX_LINES) return toast(`A plot holds up to ${MAX_LINES} lines: add another plot.`, 'info', 5000);
 158        // Each line keeps its colour for as long as it is in the plot
 159        const used = new Set(plot.traces.map(t => t.slot));
 160        plot.traces.push({ id, signal, col, slot: [...PALETTE.keys()].find(s => !used.has(s)) });
 161      }
 162      save();
 163      renderSignals();
 164      renderPlots();
 165    }
 166  
 167    function renderPlots() {
 168      // Keep each plot's element (and its last drawing while new data loads)
 169      const have = new Map($$('.res-plot', plotsEl).map(el => [Number(el.dataset.plot), el]));
 170      for (const [id, el] of have) if (!R.plots.some(p => p.id === id)) el.remove();
 171      R.plots.forEach(plot => {
 172        let el = have.get(plot.id);
 173        if (!el) { el = plotShell(plot); plotsEl.append(el); }
 174        plotsEl.append(el);   // keeps the order
 175        renderHead(plot, el);
 176        refreshPlot(plot, el);
 177      });
 178      $('#resAddPlot').disabled = R.plots.length >= MAX_PLOTS;
 179      $('#resEmpty').hidden = R.plots.some(p => p.traces.length);
 180    }
 181  
 182    function plotShell(plot) {
 183      const el = document.createElement('section');
 184      el.className = 'res-plot';
 185      el.dataset.plot = plot.id;
 186      el.innerHTML = `<div class="res-plot-head"></div><div class="res-chart"><svg class="res-svg"></svg><div class="res-tip" hidden></div></div>`;
 187      bindChart(plot.id, el);
 188      el.addEventListener('pointerdown', () => { if (R.active !== plot.id) { R.active = plot.id; markActive(); renderSignals(); } }, true);
 189      return el;
 190    }
 191  
 192    function markActive() {
 193      for (const el of $$('.res-plot', plotsEl)) el.classList.toggle('active', Number(el.dataset.plot) === R.active && R.plots.length > 1);
 194    }
 195  
 196    function renderHead(plot, el) {
 197      const head = $('.res-plot-head', el);
 198      const units = [...new Set(plot.traces.map(t => unitOf(signalInfo(t))))];
 199      const xOptions = new Set();
 200      for (const t of plot.traces) for (const s of (store.byId(t.id) || { signals: [] }).signals) if (!s.zero || (plot.x && plot.x.signal === s.name)) xOptions.add(s.name);
 201      if (plot.x) xOptions.add(plot.x.signal);
 202      const legend = plot.traces.map((t, k) => `<span class="res-key" title="${esc(`${t.signal} · column ${t.col} · ${dsName(t.id)}`)}">
 203          <i style="background:${PALETTE[t.slot]}"></i>${esc(traceLabel(t))}<span class="muted"> · ${esc(dsName(t.id))}</span>
 204          <button type="button" data-remove="${k}" title="Remove this line">×</button></span>`).join('');
 205      head.innerHTML = `
 206        <div class="res-head-row">
 207          <span class="res-plot-title">Plot ${R.plots.indexOf(plot) + 1}</span>
 208          <label class="res-x" title="Plot against time, or against another signal of the same dataset (x-y)">x
 209            <select class="input">${['<option value="">Time [s]</option>', ...[...xOptions].sort().map(n => `<option value="${esc(n)}"${plot.x && plot.x.signal === n ? ' selected' : ''}>${esc(n)}</option>`)].join('')}</select></label>
 210          ${plot.x ? `<label class="toggle" title="Same scale on both axes (true shapes on the table)"><input type="checkbox" data-equal${plot.equal ? ' checked' : ''}><span></span>Equal</label>` : ''}
 211          ${units.length > 1 ? `<span class="pill warn" title="These lines share one y axis: put signals with different units in separate plots">Mixed Units: ${esc(units.map(u => u || 'none').join(', '))}</span>` : ''}
 212          <span class="res-spacer"></span>
 213          ${plot.zoom ? '<button type="button" class="btn btn-sm" data-unzoom title="Show everything (or double-click the plot)">Zoom Out</button>' : ''}
 214          <details class="res-ranges"><summary class="btn btn-sm" title="Set the axis limits">Axes</summary>
 215            <div class="res-ranges-body">
 216              <span>x</span><input class="input" data-r="x0" type="number" step="any"><input class="input" data-r="x1" type="number" step="any">
 217              <span>y</span><input class="input" data-r="y0" type="number" step="any"><input class="input" data-r="y1" type="number" step="any">
 218              <button type="button" class="btn btn-sm" data-apply>Apply</button><button type="button" class="btn btn-sm" data-auto>Auto</button>
 219            </div></details>
 220          ${R.plots.length > 1 ? '<button type="button" class="btn btn-ghost btn-sm" data-close title="Remove this plot">✕</button>' : ''}
 221        </div>
 222        <div class="res-legend">${legend || '<span class="hint">Choose signals on the left to plot them here.</span>'}</div>`;
 223      markActive();
 224    }
 225  
 226    plotsEl.addEventListener('click', ev => {
 227      const el = ev.target.closest('.res-plot');
 228      if (!el) return;
 229      const plot = R.plots.find(p => p.id === Number(el.dataset.plot));
 230      const rm = ev.target.closest('[data-remove]');
 231      if (rm) { plot.traces.splice(Number(rm.dataset.remove), 1); save(); renderSignals(); return renderPlots(); }
 232      if (ev.target.closest('[data-close]')) {
 233        R.plots = R.plots.filter(p => p !== plot);
 234        if (R.active === plot.id) R.active = R.plots[0].id;
 235        save(); renderSignals(); return renderPlots();
 236      }
 237      if (ev.target.closest('[data-unzoom]')) return zoomTo(plot, null);
 238      const ranges = ev.target.closest('.res-ranges');
 239      if (ranges && ev.target.closest('summary')) {
 240        // Fill in what is shown now
 241        const v = plot.view || {};
 242        const set = (k, x) => { $(`[data-r="${k}"]`, ranges).value = x == null ? '' : Number(fmt(x, 6)); };
 243        set('x0', v.x && v.x[0]); set('x1', v.x && v.x[1]); set('y0', v.y && v.y[0]); set('y1', v.y && v.y[1]);
 244      }
 245      if (ev.target.closest('[data-apply]')) {
 246        const g = k => Number($(`[data-r="${k}"]`, ranges).value);
 247        const x = [g('x0'), g('x1')], y = [g('y0'), g('y1')];
 248        if (!(x[1] > x[0]) || !(y[1] > y[0])) return toast('Give each axis a minimum below its maximum.', 'error');
 249        ranges.open = false;
 250        return zoomTo(plot, { x, y });
 251      }
 252      if (ev.target.closest('[data-auto]')) { ranges.open = false; return zoomTo(plot, null); }
 253    });
 254    plotsEl.addEventListener('change', ev => {
 255      const el = ev.target.closest('.res-plot');
 256      if (!el) return;
 257      const plot = R.plots.find(p => p.id === Number(el.dataset.plot));
 258      if (ev.target.matches('.res-x select')) {
 259        plot.x = ev.target.value ? { signal: ev.target.value, col: 1 } : null;
 260        plot.zoom = null;
 261        save(); renderPlots();
 262      } else if (ev.target.matches('[data-equal]')) {
 263        plot.equal = ev.target.checked;
 264        save(); draw(plot, el);
 265      }
 266    });
 267  
 268    $('#resAddPlot').onclick = () => {
 269      if (R.plots.length >= MAX_PLOTS) return;
 270      const p = newPlot();
 271      R.plots.push(p);
 272      R.active = p.id;
 273      save(); renderSignals(); renderPlots();
 274    };
 275    const link = $('#resLink'), smooth = $('#resSmooth');
 276    link.checked = R.link;
 277    link.onchange = () => { R.link = link.checked; storage.set('spot.res.link', R.link); };
 278    smooth.value = String(R.smooth);
 279    smooth.onchange = () => { R.smooth = Number(smooth.value); storage.set('spot.res.smooth', R.smooth); renderPlots(); };
 280  
 281    // ---------------------------------------------------------------- data
 282  
 283    async function refreshPlot(plot, el) {
 284      const want = plot.traces.filter(t => store.byId(t.id));
 285      const points = Math.max(600, Math.min(6000, Math.round(plotsEl.clientWidth * 2)));
 286      const missing = want.filter(t => !R.data.has(traceKey(t, plot)));
 287      // Zoomed into a stretch of a long (thinned) signal: ask for that stretch in detail
 288      const zoomed = plot.zoom && !plot.x ? want.filter(t => {
 289        const full = R.data.get(traceKey(t, plot));
 290        return full && full.n > full.t.length && !R.data.has(traceKey(t, plot) + '@' + plot.zoom.x.join());
 291      }) : [];
 292      if (missing.length || zoomed.length) {
 293        el.classList.add('loading');
 294        draw(plot, el);
 295        try {
 296          const req = t => ({ id: t.id, signal: t.signal, col: t.col, x: plot.x, smooth: R.smooth, points });
 297          const items = [...missing.map(req), ...zoomed.map(t => ({ ...req(t), range: plot.zoom.x }))];
 298          const out = await store.series(items);
 299          missing.forEach((t, k) => R.data.set(traceKey(t, plot), clean(out[k])));
 300          zoomed.forEach((t, k) => R.data.set(traceKey(t, plot) + '@' + plot.zoom.x.join(), clean(out[missing.length + k])));
 301        } catch (e) {
 302          toast(e.message, 'error', 8000);
 303        } finally {
 304          el.classList.remove('loading');
 305        }
 306        if (R.data.size > 300) for (const k of [...R.data.keys()].slice(0, R.data.size - 300)) R.data.delete(k);
 307      }
 308      draw(plot, el);
 309    }
 310    const clean = r => (r && !r.error ? { ...r, t: list(r.t), y: list(r.y), x: r.x === undefined ? undefined : list(r.x) } : r);
 311  
 312    function seriesFor(plot, t) {
 313      const full = R.data.get(traceKey(t, plot));
 314      if (plot.zoom && !plot.x) return R.data.get(traceKey(t, plot) + '@' + plot.zoom.x.join()) || full;
 315      return full;
 316    }
 317  
 318    // ---------------------------------------------------------------- drawing
 319  
 320    function draw(plot, el) {
 321      const svg = $('svg', el);
 322      const W = svg.clientWidth, H = svg.clientHeight;
 323      svg.textContent = '';
 324      if (!W || !H) return;
 325      svg.setAttribute('viewBox', `0 0 ${W} ${H}`);
 326      svg.setAttribute('font-family', getComputedStyle(document.body).fontFamily);
 327      svg.setAttribute('font-size', '11');
 328      const xy = Boolean(plot.x);
 329      const lines = plot.traces.map(t => ({ t, s: seriesFor(plot, t) })).filter(l => l.s && !l.s.error);
 330      for (const t of plot.traces) {
 331        const s = seriesFor(plot, t);
 332        if (s && s.error) $$('.res-key', el)[plot.traces.indexOf(t)]?.classList.add('bad');
 333      }
 334      const X = l => (xy ? l.s.x : l.s.t);
 335      // Domains: the zoom, or everything (y: what is inside the x range)
 336      let x0 = Infinity, x1 = -Infinity;
 337      for (const l of lines) for (const v of X(l)) if (v != null && isFinite(v)) { if (v < x0) x0 = v; if (v > x1) x1 = v; }
 338      if (plot.zoom) [x0, x1] = plot.zoom.x;
 339      let y0 = Infinity, y1 = -Infinity;
 340      for (const l of lines) {
 341        const xs = X(l);
 342        l.s.y.forEach((v, i) => { if (v != null && isFinite(v) && xs[i] >= x0 && xs[i] <= x1) { if (v < y0) y0 = v; if (v > y1) y1 = v; } });
 343      }
 344      if (plot.zoom && plot.zoom.y) [y0, y1] = plot.zoom.y;
 345      if (!isFinite(x0)) { x0 = 0; x1 = 1; }
 346      if (!isFinite(y0)) { y0 = 0; y1 = 1; }
 347      if (x1 === x0) { x0 -= 0.5; x1 += 0.5; }
 348      if (y1 === y0) { const d = Math.abs(y0) * 0.1 || 0.5; y0 -= d; y1 += d; }
 349      else if (!(plot.zoom && plot.zoom.y)) { const pad = (y1 - y0) * 0.06; y0 -= pad; y1 += pad; }
 350      const right = xy ? 18 : (plot.traces.length <= 4 ? M.r : 18);
 351      const pw = Math.max(10, W - M.l - right), ph = Math.max(10, H - M.t - M.b);
 352      if (xy && plot.equal) {
 353        // Same metres per pixel on both axes: widen whichever range is too narrow
 354        const sx = (x1 - x0) / pw, sy = (y1 - y0) / ph, s = Math.max(sx, sy);
 355        const cx = (x0 + x1) / 2, cy = (y0 + y1) / 2;
 356        x0 = cx - s * pw / 2; x1 = cx + s * pw / 2; y0 = cy - s * ph / 2; y1 = cy + s * ph / 2;
 357      }
 358      plot.view = { x: [x0, x1], y: [y0, y1], pw, ph, xy };
 359      const sx = v => M.l + (v - x0) / (x1 - x0) * pw, sy = v => M.t + ph - (v - y0) / (y1 - y0) * ph;
 360      plot.view.sx = sx; plot.view.sy = sy;
 361  
 362      // Grid and axes
 363      const g = svgEl('g');
 364      for (const v of niceTicks(y0, y1, Math.max(2, Math.round(ph / 46)))) {
 365        g.append(svgEl('line', { x1: M.l, x2: M.l + pw, y1: sy(v), y2: sy(v), stroke: v === 0 ? INK.axis : INK.grid, 'stroke-width': 1 }));
 366        const tx = svgEl('text', { x: M.l - 8, y: sy(v) + 4, 'text-anchor': 'end', fill: INK.muted, 'font-variant-numeric': 'tabular-nums' });
 367        tx.textContent = fmt(v, 4);
 368        g.append(tx);
 369      }
 370      for (const v of niceTicks(x0, x1, Math.max(2, Math.round(pw / 90)))) {
 371        if (xy) g.append(svgEl('line', { x1: sx(v), x2: sx(v), y1: M.t, y2: M.t + ph, stroke: v === 0 ? INK.axis : INK.grid }));
 372        g.append(svgEl('line', { x1: sx(v), x2: sx(v), y1: M.t + ph, y2: M.t + ph + 4, stroke: INK.axis }));
 373        const tx = svgEl('text', { x: sx(v), y: M.t + ph + 17, 'text-anchor': 'middle', fill: INK.muted, 'font-variant-numeric': 'tabular-nums' });
 374        tx.textContent = fmt(v, 4);
 375        g.append(tx);
 376      }
 377      g.append(svgEl('line', { x1: M.l, x2: M.l + pw, y1: M.t + ph, y2: M.t + ph, stroke: INK.axis }));
 378      const xl = svgEl('text', { x: M.l + pw, y: H - 2, 'text-anchor': 'end', fill: INK.muted });
 379      xl.textContent = xy ? ((xInfo(plot) || {}).label || plot.x.signal) : 'Time [s]';
 380      g.append(xl);
 381      const units = [...new Set(plot.traces.map(t => unitOf(signalInfo(t))))].filter(Boolean);
 382      if (units.length === 1) {
 383        const yl = svgEl('text', { x: 12, y: M.t + ph / 2, 'text-anchor': 'middle', fill: INK.muted, transform: `rotate(-90 12 ${M.t + ph / 2})` });
 384        yl.textContent = units[0];
 385        g.append(yl);
 386      }
 387      svg.append(g);
 388  
 389      // Lines, clipped to the plot area; a missing value breaks the line
 390      const clipId = `clip${plot.id}`;
 391      const defs = svgEl('defs');
 392      const clip = svgEl('clipPath', { id: clipId });
 393      clip.append(svgEl('rect', { x: M.l, y: M.t - 1, width: pw, height: ph + 2 }));
 394      defs.append(clip);
 395      svg.append(defs);
 396      const lg = svgEl('g', { 'clip-path': `url(#${clipId})` });
 397      for (const l of lines) {
 398        const xs = X(l);
 399        let d = '', pen = false;
 400        l.s.y.forEach((v, i) => {
 401          const x = xs[i];
 402          // (in an x-y plot, (0, 0) is a platform PhaseSpace hadn't seen yet)
 403          if (v == null || x == null || !isFinite(v) || !isFinite(x) || (xy && x === 0 && v === 0)) { pen = false; return; }
 404          d += `${pen ? 'L' : 'M'}${sx(x).toFixed(1)} ${sy(v).toFixed(1)}`;
 405          pen = true;
 406        });
 407        lg.append(svgEl('path', { d, fill: 'none', stroke: PALETTE[l.t.slot], 'stroke-width': 2, 'stroke-linejoin': 'round', 'stroke-linecap': 'round' }));
 408      }
 409      svg.append(lg);
 410  
 411      // Line names at the right end (up to four lines), pushed apart so they don't overlap
 412      if (!xy && plot.traces.length <= 4 && lines.length) {
 413        const ends = lines.map(l => {
 414          let k = l.s.t.length - 1;
 415          while (k > 0 && (l.s.t[k] > x1 || l.s.y[k] == null)) k--;
 416          return { l, y: Math.min(Math.max(sy(l.s.y[k]), M.t + 6), M.t + ph - 2) };
 417        }).sort((a, b) => a.y - b.y);
 418        for (let k = 1; k < ends.length; k++) ends[k].y = Math.max(ends[k].y, ends[k - 1].y + 13);
 419        const over = ends.length ? ends[ends.length - 1].y - (M.t + ph) : 0;
 420        if (over > 0) for (const e of ends) e.y -= over;
 421        // The signal's name, or the dataset's when the same signal is there from several datasets
 422        const short = t => t.signal.replace(/^(RED|BLACK|BLUE|ARM)_/, (m, k) => `${platformName(k)} `) + (t.col > 1 ? `[${t.col}]` : '');
 423        const twice = new Set(lines.map(l => short(l.t)).filter((n, k, a) => a.indexOf(n) !== k));
 424        for (const e of ends) {
 425          svg.append(svgEl('line', { x1: M.l + pw + 4, x2: M.l + pw + 14, y1: e.y, y2: e.y, stroke: PALETTE[e.l.t.slot], 'stroke-width': 2 }));
 426          const tx = svgEl('text', { x: M.l + pw + 18, y: e.y + 4, fill: INK.text });
 427          const name = twice.has(short(e.l.t)) ? dsName(e.l.t.id) : short(e.l.t);
 428          tx.textContent = name.length > 14 ? name.slice(0, 13) + '…' : name;
 429          svg.append(tx);
 430        }
 431      }
 432  
 433      // Hover layer, and the zoom selection
 434      svg.append(svgEl('g', { class: 'res-hover' }));
 435      svg.append(svgEl('rect', { class: 'res-sel', x: 0, y: 0, width: 0, height: 0, fill: '#4c8dff', 'fill-opacity': 0.12, stroke: '#4c8dff', 'stroke-opacity': 0.5 }));
 436      if (R.hoverT != null && !xy) crosshair(plot, el, R.hoverT, false);
 437    }
 438  
 439    // ---------------------------------------------------------------- hover and zoom
 440  
 441    function nearest(ts, t) {
 442      let lo = 0, hi = ts.length - 1;
 443      if (hi < 0) return -1;
 444      while (hi - lo > 1) { const mid = (lo + hi) >> 1; if (ts[mid] <= t) lo = mid; else hi = mid; }
 445      return Math.abs(ts[hi] - t) < Math.abs(ts[lo] - t) ? hi : lo;
 446    }
 447  
 448    function crosshair(plot, el, t, withTip, px) {
 449      const v = plot.view;
 450      const layer = $('.res-hover', el), tip = $('.res-tip', el);
 451      if (!v || !layer) return;
 452      layer.textContent = '';
 453      if (t == null || t < v.x[0] || t > v.x[1]) { tip.hidden = true; return; }
 454      layer.append(svgEl('line', { x1: v.sx(t), x2: v.sx(t), y1: M.t, y2: M.t + v.ph, stroke: INK.muted, 'stroke-width': 1 }));
 455      const rows = [];
 456      for (const tr of plot.traces) {
 457        const s = seriesFor(plot, tr);
 458        if (!s || s.error) continue;
 459        const i = nearest(s.t, t);
 460        const y = i >= 0 ? s.y[i] : null;
 461        if (y != null && y >= v.y[0] && y <= v.y[1]) {
 462          layer.append(svgEl('circle', { cx: v.sx(s.t[i]), cy: v.sy(y), r: 4.5, fill: PALETTE[tr.slot], stroke: INK.surface, 'stroke-width': 2 }));
 463        }
 464        rows.push({ tr, y });
 465      }
 466      if (!withTip) { tip.hidden = true; return; }
 467      showTip(el, tip, px, `<div class="res-tip-head">t = ${fmt(t, 6)} s</div>` + rows.map(r =>
 468        `<div class="res-tip-row"><i style="background:${PALETTE[r.tr.slot]}"></i><b>${fmt(r.y, 6)}</b><span>${esc(traceLabel(r.tr))} · ${esc(dsName(r.tr.id))}</span></div>`).join(''));
 469    }
 470  
 471    function showTip(el, tip, px, html) {
 472      tip.innerHTML = html;
 473      tip.hidden = false;
 474      const box = $('.res-chart', el).getBoundingClientRect();
 475      const w = tip.offsetWidth;
 476      tip.style.left = `${px.x + 16 + w > box.width ? px.x - 16 - w : px.x + 16}px`;
 477      tip.style.top = `${Math.max(4, Math.min(px.y - 10, box.height - tip.offsetHeight - 4))}px`;
 478    }
 479  
 480    function hoverXY(plot, el, px) {
 481      const v = plot.view, layer = $('.res-hover', el), tip = $('.res-tip', el);
 482      layer.textContent = '';
 483      let best = null;
 484      for (const tr of plot.traces) {
 485        const s = seriesFor(plot, tr);
 486        if (!s || s.error || !s.x) continue;
 487        s.y.forEach((y, i) => {
 488          const x = s.x[i];
 489          if (x == null || y == null) return;
 490          const d = (v.sx(x) - px.x) ** 2 + (v.sy(y) - px.y) ** 2;
 491          if (!best || d < best.d) best = { d, tr, i, x, y, t: s.t[i] };
 492        });
 493      }
 494      if (!best || best.d > 60 ** 2) { tip.hidden = true; return; }
 495      layer.append(svgEl('circle', { cx: v.sx(best.x), cy: v.sy(best.y), r: 5, fill: PALETTE[best.tr.slot], stroke: INK.surface, 'stroke-width': 2 }));
 496      showTip(el, tip, px, `<div class="res-tip-row"><i style="background:${PALETTE[best.tr.slot]}"></i><b>${fmt(best.y, 6)}</b><span>${esc(traceLabel(best.tr))}</span></div>
 497        <div class="res-tip-row"><i></i><b>${fmt(best.x, 6)}</b><span>${esc(plot.x.signal)}</span></div>
 498        <div class="res-tip-row"><i></i><b>${fmt(best.t, 6)} s</b><span>${esc(dsName(best.tr.id))}</span></div>`);
 499    }
 500  
 501    function bindChart(id, el) {
 502      const svg = $('svg', el);
 503      const plotOf = () => R.plots.find(p => p.id === id);
 504      let drag = null;
 505      const at = ev => { const r = svg.getBoundingClientRect(); return { x: ev.clientX - r.left, y: ev.clientY - r.top }; };
 506      const inside = (v, p) => p.x >= M.l && p.x <= M.l + v.pw && p.y >= M.t && p.y <= M.t + v.ph;
 507      const tOf = (v, p) => v.x[0] + (p.x - M.l) / v.pw * (v.x[1] - v.x[0]);
 508      const yOf = (v, p) => v.y[0] + (M.t + v.ph - p.y) / v.ph * (v.y[1] - v.y[0]);
 509      svg.addEventListener('pointermove', ev => {
 510        const plot = plotOf(), v = plot && plot.view;
 511        if (!v) return;
 512        const p = at(ev);
 513        if (drag) {
 514          const sel = $('.res-sel', el);
 515          const xa = Math.max(M.l, Math.min(drag.x, p.x)), xb = Math.min(M.l + v.pw, Math.max(drag.x, p.x));
 516          const ya = v.xy ? Math.max(M.t, Math.min(drag.y, p.y)) : M.t, yb = v.xy ? Math.min(M.t + v.ph, Math.max(drag.y, p.y)) : M.t + v.ph;
 517          Object.entries({ x: xa, y: ya, width: xb - xa, height: yb - ya }).forEach(([k, val]) => sel.setAttribute(k, val));
 518          return;
 519        }
 520        if (!inside(v, p)) return leave();
 521        if (v.xy) return hoverXY(plot, el, p);
 522        const t = tOf(v, p);
 523        R.hoverT = t;
 524        crosshair(plot, el, t, true, p);
 525        if (R.link) for (const other of R.plots) if (other !== plot && !other.x) crosshair(other, plotsEl.querySelector(`[data-plot="${other.id}"]`), t, false);
 526      });
 527      const leave = () => {
 528        R.hoverT = null;
 529        for (const other of R.plots) {
 530          const oel = plotsEl.querySelector(`[data-plot="${other.id}"]`);
 531          if (oel && $('.res-hover', oel)) { $('.res-hover', oel).textContent = ''; $('.res-tip', oel).hidden = true; }
 532        }
 533      };
 534      svg.addEventListener('pointerleave', () => { if (!drag) leave(); });
 535      svg.addEventListener('pointerdown', ev => {
 536        const plot = plotOf(), v = plot && plot.view;
 537        if (ev.button !== 0 || !v || !plot.traces.length) return;
 538        const p = at(ev);
 539        if (!inside(v, p)) return;
 540        drag = p;
 541        try { svg.setPointerCapture(ev.pointerId); } catch { /* synthetic events */ }
 542      });
 543      svg.addEventListener('pointerup', ev => {
 544        if (!drag) return;
 545        const plot = plotOf(), v = plot.view, p = at(ev), start = drag;
 546        drag = null;
 547        $('.res-sel', el).setAttribute('width', 0);
 548        if (Math.abs(p.x - start.x) < 6 && (!v.xy || Math.abs(p.y - start.y) < 6)) return;
 549        const clampP = q => ({ x: Math.max(M.l, Math.min(M.l + v.pw, q.x)), y: Math.max(M.t, Math.min(M.t + v.ph, q.y)) });
 550        const a = clampP(start), b = clampP(p);
 551        const x = [tOf(v, a), tOf(v, b)].sort((m, n) => m - n);
 552        zoomTo(plot, v.xy ? { x, y: [yOf(v, a), yOf(v, b)].sort((m, n) => m - n) } : { x });
 553      });
 554      svg.addEventListener('dblclick', () => zoomTo(plotOf(), null));
 555    }
 556  
 557    function zoomTo(plot, zoom) {
 558      const targets = !plot.x && R.link ? R.plots.filter(p => !p.x) : [plot];
 559      for (const p of targets) p.zoom = zoom && (p === plot || !zoom.y) ? zoom : zoom ? { x: zoom.x } : null;
 560      renderPlots();
 561    }
 562  
 563    new ResizeObserver(() => { if (R.shown) for (const plot of R.plots) { const el = plotsEl.querySelector(`[data-plot="${plot.id}"]`); if (el) draw(plot, el); } }).observe(plotsEl);
 564  
 565    // ---------------------------------------------------------------- export
 566  
 567    // Everything is drawn and saved by MATLAB (spot.data.toFigure / exportFigure), in the lab's
 568    // publication style, so a figure for a paper never comes from the browser
 569    const expW = $('#resExpW'), expH = $('#resExpH'), expF = $('#resExpF');
 570    const expOpts = storage.get('spot.res.export', { width: 7, height: null, fontSize: 10 });
 571    expW.value = expOpts.width; expH.value = expOpts.height || ''; expF.value = expOpts.fontSize;
 572    for (const el of [expW, expH, expF]) el.onchange = () => storage.set('spot.res.export', exportOptions());
 573    function exportOptions() {
 574      const n = v => (v === '' || !isFinite(Number(v)) ? null : Number(v));
 575      return { width: n(expW.value) || 7, height: n(expH.value), fontSize: n(expF.value) || 10 };
 576    }
 577    function figurePanels() {
 578      return R.plots.filter(p => p.traces.length).map(p => ({
 579        x: p.x, smooth: R.smooth,
 580        xlim: p.zoom ? p.zoom.x : [], ylim: p.zoom && p.zoom.y ? p.zoom.y : [],
 581        traces: p.traces.map(t => ({ id: t.id, signal: t.signal, col: t.col })),
 582      }));
 583    }
 584    async function exportFigure(save) {
 585      const panels = figurePanels();
 586      if (!panels.length) return toast('Nothing plotted yet.', 'info');
 587      $('.res-export').open = false;
 588      if (save) toast('Choose where to save in MATLAB\'s dialog', 'info', 4000);
 589      const opts = exportOptions();
 590      if (opts.height == null) delete opts.height;
 591      const r = await store.figure(panels, opts, save);
 592      if (r.cancelled) return;
 593      toast(save ? `Saved ${r.file}` : `Opened Figure ${r.figure} in MATLAB`, 'success', 7000);
 594    }
 595    $('#resSaveFig').onclick = () => act(() => exportFigure(true));
 596    $('#resFigure').onclick = () => act(() => exportFigure(false));
 597    document.addEventListener('pointerdown', ev => {
 598      const d = $('.res-export');
 599      if (d.open && !ev.target.closest('.res-export')) d.open = false;
 600    });
 601  
 602    // ---------------------------------------------------------------- store and page
 603  
 604    store.on(() => {
 605      // A dataset closed elsewhere (or MATLAB restarted): drop its lines
 606      let changed = false;
 607      for (const p of R.plots) {
 608        const keep = p.traces.filter(t => store.byId(t.id));
 609        if (keep.length !== p.traces.length) { p.traces = keep; changed = true; }
 610      }
 611      if (changed) save();
 612      renderDatasets();
 613      if (R.shown) renderPlots();
 614    });
 615  
 616    return {
 617      show() {
 618        R.shown = true;
 619        renderDatasets();
 620        renderPlots();
 621        if (!R.loaded) { R.loaded = true; act(() => store.refresh()); }
 622      },
 623      hide() { R.shown = false; },
 624      onMatlab(m) {
 625        // Datasets live in MATLAB: a new MATLAB session has none
 626        if (m.connected && !R.wasConnected) { R.data.clear(); act(() => store.refresh()); }
 627        R.wasConnected = m.connected;
 628        $('#resMatlab').hidden = m.connected;
 629        for (const b of ['#resLoadSaved', '#resLoadFile', '#resFigure', '#resSaveFig']) $(b).disabled = !m.connected;
 630      },
 631      onSimFinished() { store.refresh().catch(() => {}); },
 632    };
 633  }
```

</details>
<!-- /listing -->
