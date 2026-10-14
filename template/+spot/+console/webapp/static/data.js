// Loaded results (local console), shared by the Results page (plots) and the Simulation tab
// (3D playback with overlays). The data stays in MATLAB (spot.worker.Worker keeps each loaded
// dataset under an id); this page asks for what it draws: signals thinned to the plot's width
// (data.series) and the platforms' motion (data.trajectory).
//
// Also the "Saved Data" dialog, from which both pages load runs.

import { platformName } from './platforms.js';

const $ = (sel, el = document) => el.querySelector(sel);
export const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
export const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const KIND = { experiment: 'EXP', legacy: 'EXP', simulation: 'SIM', files: 'MAT', file: 'MAT' };
export const kindTag = kind => KIND[kind] || 'MAT';


export function createDataStore({ api, toast }) {
  const matlab = (cmd, args = {}) => api('/api/matlab', { cmd, args }).then(r => r.result);
  const D = {
    saved: [], loaded: [], simulation: false, listeners: new Set(),
    series: new Map(), trajectories: new Map(),   // caches (by request / by id)
    overlay: new Map(),   // id -> {on, offset}: replayed over the simulation (Simulation tab)
  };
  const emit = () => { for (const fn of D.listeners) fn(); };

  function addLoaded(datasets) {
    const got = list(datasets).map(normalize);
    D.loaded = [...D.loaded.filter(d => !got.some(g => g.id === d.id)), ...got];
    emit();
    return got;
  }
  function normalize(d) {
    d.signals = list(d.signals);
    d.crafts = list(d.crafts);
    return d;
  }

  const store = {
    get saved() { return D.saved; },
    get loaded() { return D.loaded; },
    get simulation() { return D.simulation; },
    byId: id => D.loaded.find(d => d.id === id),
    overlay: id => D.overlay.get(id) || { on: false, offset: 0 },
    setOverlay(id, patch) { D.overlay.set(id, { ...store.overlay(id), ...patch }); emit(); },
    on(fn) { D.listeners.add(fn); return () => D.listeners.delete(fn); },

    async refresh() {
      const r = await matlab('data.list');
      D.saved = list(r.saved).map(run => ({ ...run, items: list(run.items) }));
      D.loaded = list(r.loaded).map(normalize);
      D.simulation = Boolean(r.simulation);
      for (const id of [...D.trajectories.keys()]) if (!store.byId(id)) D.trajectories.delete(id);
      for (const id of [...D.overlay.keys()]) if (!store.byId(id)) D.overlay.delete(id);
      emit();
    },

    async load(spec) {
      const r = await matlab('data.load', spec);
      if (!r || r.cancelled) return [];
      const got = addLoaded(r.datasets);
      toast(`Loaded ${got.map(d => d.name).join(', ')}`, 'success', 3500);
      return got;
    },

    async unload(id) {
      D.loaded = D.loaded.filter(d => d.id !== id);
      D.trajectories.delete(id);
      D.overlay.delete(id);
      for (const k of [...D.series.keys()]) if (k.startsWith(id + '|')) D.series.delete(k);
      emit();
      await matlab('data.unload', { id }).catch(() => {});
    },

    async series(items) {
      // items: [{id, signal, col, x, smooth, range, points}] -> the same order, from the cache when asked before
      const key = it => `${it.id}|${JSON.stringify(it)}`;
      const missing = items.filter(it => !D.series.has(key(it)));
      if (missing.length) {
        const out = list(await matlab('data.series', { items: missing }));
        missing.forEach((it, k) => D.series.set(key(it), out[k] || { error: 'No answer' }));
        while (D.series.size > 400) D.series.delete(D.series.keys().next().value);
      }
      return items.map(it => D.series.get(key(it)));
    },

    async trajectory(id) {
      if (!D.trajectories.has(id)) {
        const p = matlab('data.trajectory', { id }).then(r => { r.crafts = r.crafts || {}; return r; });
        D.trajectories.set(id, p);
        p.catch(() => D.trajectories.delete(id));
      }
      return D.trajectories.get(id);
    },

    async keepSimulation() {
      return store.load({ source: 'simulation' });
    },
    async saveSimulation() {
      const r = await matlab('data.saveSimulation');
      toast(`Saved the simulation in ${r.folder}`, 'success', 6000);
      store.refresh().catch(() => {});
      return r;
    },
    figure: (panels, options, save = false) => matlab('data.figure', { panels, options, save }),
    openFile: () => { toast('Choose the file in MATLAB\'s dialog', 'info', 4000); return store.load({ dialog: true }); },
    pick: opts => picker.open(opts),
  };

  // ---------------------------------------------------------------- the Saved Data dialog

  const picker = (() => {
    const dlg = $('#resultsDialog');
    let resolve = null;
    function render() {
      const body = $('#resultsList');
      if (!D.saved.length) {
        body.innerHTML = '<p class="hint">Nothing in Saved Data yet. Experiments collected on the Experiment tab, and simulations saved from the Simulation tab, appear here.</p>';
        return;
      }
      body.innerHTML = D.saved.map((run, r) => {
        const when = run.modified ? new Date(run.modified * 1000).toLocaleString([], { month: 'short', day: 'numeric', hour: '2-digit', minute: '2-digit' }) : '';
        const items = run.items.map((it, k) => {
          if (it.convert) return `<button type="button" class="btn btn-sm" data-convert="${r}" title="Convert the platforms' ExpLog files to .mat with MATLAB (compiles the diagram)">Convert ExpLog Files</button>`;
          const plats = Object.keys(it.files || {});
          return `<button type="button" class="btn btn-sm" data-run="${r}" data-item="${k}" title="${esc(it.file || Object.values(it.files).join('\n'))}">${esc(it.label)}${plats.length ? ` <span class="muted">${plats.map(platformName).join(' ')}</span>` : ''}</button>`;
        }).join('');
        return `<div class="saved-run"><div class="saved-head"><span class="pill">${kindTag(run.kind)}</span>
          <b>${esc(run.label || run.name)}</b><span class="muted small mono" title="${esc(run.name)}">${esc(run.name)}</span><span class="muted small">${esc(when)}</span></div>
          <div class="saved-items">${items}</div></div>`;
      }).join('');
    }
    async function choose(run, it) {
      // The dataset's name comes from MATLAB (spot.data.listSaved): "10-07 15:02 · Run 2", or the file's own
      const spec = it.file ? { file: it.file } : { files: it.files };
      if (it.name) spec.name = it.name;
      if (run.settings) spec.settings = run.settings;
      const got = await store.load(spec);
      if (got.length) finish(got);
    }
    function finish(got) {
      dlg.close();
      const r = resolve;
      resolve = null;
      r?.(got);
    }
    $('#resultsList').addEventListener('click', async ev => {
      const b = ev.target.closest('button');
      if (!b) return;
      b.disabled = true;
      try {
        if (b.dataset.convert) {
          const run = D.saved[Number(b.dataset.convert)];
          b.innerHTML = '<span class="spinner dark"></span> Converting…';
          await matlab('experiment.decode', { folder: run.folder });
          await store.refresh();
          render();
        } else {
          const run = D.saved[Number(b.dataset.run)];
          b.innerHTML = '<span class="spinner dark"></span> Loading…';
          await choose(run, run.items[Number(b.dataset.item)]);
        }
      } catch (e) {
        toast(e.message, 'error', 9000);
        render();
      }
    });
    $('#resultsOpenFile').onclick = async () => {
      try {
        const got = await store.openFile();
        if (got.length) finish(got);
      } catch (e) { toast(e.message, 'error', 9000); }
    };
    $('#resultsRefresh').onclick = () => store.refresh().then(render).catch(e => toast(e.message, 'error'));
    dlg.addEventListener('close', () => { if (resolve) { const r = resolve; resolve = null; r([]); } });
    return {
      async open() {
        $('#resultsList').innerHTML = '<p class="hint"><span class="spinner dark"></span> Reading Saved Data…</p>';
        dlg.showModal();
        try { await store.refresh(); } catch (e) { toast(e.message, 'error'); }
        render();
        return new Promise(r => { resolve = r; });
      },
    };
  })();

  return store;
}
