// SPOT Audit: the page. One chunk at a time: the original on the left, your version on the
// right; accept, rewrite, flag or skip. Talks to audit/app.py over JSON. The editor and the
// Markdown renderer are the SPOT Console's own bundles (served under /vendor).

const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
const esc = s => String(s ?? '').replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));

const CM_URL = '/vendor/codemirror/codemirror.js';
const MD_URL = '/vendor/markdown/markdown.js';
const CHECK_DELAY = 350;   // [ms] after the last keystroke before the code check
const DAYS_SHOWN = 30;
const HEARTBEAT_MS = 30000;   // the server counts time played from these, while the page is visible
const INDENT = { matlab: '    ', python: '    ' };
const ACH_ICONS = {
  first_clear: '🚶', first_rewrite: '✍️', first_flag: '🚩', first_resolve: '✅', first_file: '📕',
  surgeon: '🩺', clear_50: '🔥', clear_250: '💪', clear_1000: '🏔️', clear_2500: '🚀',
  pct_25: '🥉', pct_50: '🥈', pct_75: '🥇', pct_100: '🏆', rewrites_25: '📝', rewrites_100: '📚',
  rewrites_500: '👻', flags_10: '🔍', loose_ends: '🧵', day_50: '☀️', day_150: '🌋',
  night_owl: '🦉', early_bird: '🐦', polyglot: '🌐', deep_dive: '🤿', files_25: '🗂️',
  files_100: '🏛️', streak_3: '📅', streak_7: '🗓️',
};
const CHAPTER_ICONS = { docs: '📖', matlab: '🟧', python: '🐍', javascript: '🟨', shell: '🐚',
  firmware: '🔌', config: '⚙️' };

const S = {
  state: null, chunk: null, page: 'play', origMode: 'code', mineMode: 'edit', view: null,
  lib: { cm: null, md: null }, marked: null, checkTimer: null, busy: false,
  open: {}, dirty: false,
};
try {
  S.open = JSON.parse(localStorage.getItem('audit.open') || '{}');
} catch {
  S.open = {};
}

// Helpers

/**
 * Calls the server: GET without a body, else POST it as JSON.
 * @param {string} path
 * @param {!Object=} body
 * @return {!Promise<!Object>}
 */
async function api(path, body) {
  const opts = body === undefined ? {} : {
    method: 'POST', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify(body),
  };
  const res = await fetch(path, opts);
  let data = {};
  try {
    data = await res.json();
  } catch {
    // Not JSON.
  }
  if (!res.ok) {
    throw new Error(data.error || `Request failed (${res.status})`);
  }
  return data;
}

function toast(message, kind = 'info', ms = 5000, sub = '') {
  const el = document.createElement('div');
  el.className = `toast ${kind}`;
  el.innerHTML = `${kind === 'trophy' ? '🏆 ' : ''}${kind === 'trophy' ? `<b>${esc(message)}</b>` : esc(message)}` +
    (sub ? `<span class="t-sub">${esc(sub)}</span>` : '');
  $('#toasts').append(el);
  setTimeout(() => el.remove(), ms);
}

async function act(fn) {
  if (S.busy) {
    return undefined;
  }
  S.busy = true;
  setSaving(true);
  try {
    return await fn();
  } catch (e) {
    toast(e.message, 'error', 9000);
    return undefined;
  } finally {
    S.busy = false;
    setSaving(false);
  }
}

function setSaving(on) {
  const b = $('#saveBadge');
  b.className = `badge ${on ? 'badge-warn' : 'badge-ok'}`;
  b.lastChild.textContent = on ? 'Saving' : 'Saved';
}

function when(iso) {
  if (!iso) {
    return '';
  }
  const d = new Date(iso);
  const now = new Date();
  const diff = (now - d) / 1000;
  if (diff < 60) {
    return 'just now';
  }
  if (diff < 3600) {
    return `${Math.floor(diff / 60)} min ago`;
  }
  if (d.toDateString() === now.toDateString()) {
    return d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  }
  return d.toLocaleDateString([], { month: 'short', day: 'numeric' });
}

const pct = (a, b) => (b ? Math.round(1000 * a / b) / 10 : 0);

/** Seconds as "2 h 05 min" (or "12 min", "40 s"). */
function played(sec) {
  const s = Math.round(sec || 0);
  if (s < 60) {
    return `${s} s`;
  }
  const h = Math.floor(s / 3600);
  const m = Math.floor((s % 3600) / 60);
  return h ? `${h} h ${String(m).padStart(2, '0')} min` : `${m} min`;
}
const shortPath = rel => rel.replace(/^template\/\+spot\//, '').replace(/^template\//, '');

// Boot

async function boot() {
  const [cm, md] = await Promise.all([import(CM_URL), import(MD_URL)]);
  S.lib.cm = cm;
  S.lib.md = md;
  S.marked = makeMarked(md);
  wireUi();
  await loadState();
  const m = /k=([^&]+)/.exec(location.hash);
  let key = m ? decodeURIComponent(m[1]) : S.state.first_pending;
  if (!key) {
    const ch = S.state.summary.chapters.find(c => c.sections.length);
    const f = await api(`/api/file?rel=${encodeURIComponent(ch.sections[0].files[0].rel)}`);
    key = f.first_key;
  }
  await openChunk(key);
  const pm = /p=(\w+)/.exec(location.hash);
  if (pm) {
    showPage(pm[1]);
  }
  setInterval(heartbeat, HEARTBEAT_MS);
}

/** Tells the server the page is open, so it counts the time played. */
async function heartbeat() {
  if (document.visibilityState !== 'visible') {
    return;
  }
  try {
    const r = await api('/api/heartbeat', {});
    $('#xpPlayed').textContent = played(r.time_played);
  } catch {
    // The server is away; the next beat will tell.
  }
}

// Save files

async function openSaves(mode) {
  await act(async () => {
    const r = await api('/api/saves');
    $('#savesTitle').textContent = mode === 'save' ? 'Save Game' : 'Load Game';
    $('#savesNew').hidden = mode !== 'save';
    $('#savesWhere').textContent = `Save files are JSON files in ${r.dir}. The first is the automatic save, written after every change; the others are yours.`;
    renderSaves(r.saves);
    $('#savesDialog').showModal();
    if (mode === 'save') {
      $('#saveName').focus();
    }
  });
}

function renderSaves(saves) {
  const cur = S.state.summary;
  $('#savesList').innerHTML = saves.length ? saves.map(m => {
    const auto = m.kind === 'auto';
    const p = m.total ? 100 * m.cleared / m.total : 0;
    return `<div class="save-row${auto ? ' auto' : ''}" data-file="${esc(m.file)}" data-name="${esc(m.name)}">
      <div class="nm">${auto ? '<span class="kind">Latest Autosave</span>' : ''}<span title="${esc(m.file)}">${esc(m.name)}</span></div>
      <div class="meta"><span>saved <b>${esc(m.saved_at ? m.saved_at.replace('T', ' ').slice(0, 16) : '?')}</b></span>
        <span>played <b>${played(m.time_played)}</b></span>
        <span>cleared <b>${m.cleared ?? '?'} / ${m.total ?? '?'}</b> (${(m.pct ?? 0).toFixed(1)} %)</span>
        <span>level <b>${m.level ?? '?'}</b> ${esc(m.level_title || '')}</span>
        <span>rewrites <b>${m.rewritten ?? 0}</b></span><span>open flags <b>${m.flags_open ?? 0}</b></span>
        <span>files done <b>${m.files_done ?? 0}</b></span></div>
      <div class="bar"><i style="width:${p}%"></i></div>
      <div class="row">
        <button type="button" class="btn btn-sm btn-primary" data-act="load">Load</button>
        ${auto ? '' : '<button type="button" class="btn btn-sm" data-act="overwrite">Overwrite</button><button type="button" class="btn btn-sm btn-ghost" data-act="delete">Delete</button>'}
      </div></div>`;
  }).join('') : '<p class="empty">No save files yet.</p>';
  $('#savesWhere').dataset.cleared = cur.cleared;
}

async function saveSlot(name, overwrite) {
  await act(async () => {
    const r = await api('/api/saves/save', { name, overwrite: !!overwrite });
    if (r.exists) {
      const ok = await confirm('Overwrite this save?', `<p>A save file named <b>${esc(name)}</b> exists already (<span class="mono">${esc(r.file)}</span>). Replace it with your current progress?</p>`);
      if (ok) {
        await saveSlot(name, true);
      }
      return;
    }
    toast(`Saved as "${name}".`, 'success');
    $('#saveName').value = '';
    renderSaves(r.saves);
    loadState().catch(() => {});
  });
}

async function loadSlot(file, name) {
  const ok = await confirm('Load this save?', `<p>Your current progress will be replaced by <b>${esc(name)}</b>. The automatic save follows the loaded state, so save the current progress to a slot first if you want to keep it.</p>`);
  if (!ok) {
    return;
  }
  await act(async () => {
    const r = await api('/api/saves/load', { file });
    S.state = r.state;
    renderAll();
    renderSaves(r.saves);
    $('#savesDialog').close();
    toast(`Loaded "${name}".`, 'success');
    if (S.chunk) {
      await openChunk(S.chunk.key);
    }
  });
}

async function deleteSlot(file, name) {
  const ok = await confirm('Delete this save?', `<p><b>${esc(name)}</b> (<span class="mono">${esc(file)}</span>) will be deleted. This cannot be undone.</p>`);
  if (!ok) {
    return;
  }
  await act(async () => {
    const r = await api('/api/saves/delete', { file });
    renderSaves(r.saves);
    toast(`Deleted "${name}".`, 'success');
  });
}

async function loadState() {
  S.state = await api('/api/state');
  renderAll();
}

function renderAll() {
  $('#autoAdvance').checked = S.state.settings.autoAdvance !== false;
  renderTopStats();
  renderXp();
  renderTree();
  renderFlagsMini();
  renderRecent();
  if (S.page === 'progress') {
    renderProgressPage();
  } else if (S.page === 'flags') {
    renderFlagsPage();
  } else if (S.page === 'achievements') {
    renderAchievementsPage();
  }
}

// Pages

function showPage(name) {
  S.page = name;
  $$('.page-tabs button').forEach(b => b.classList.toggle('active', b.dataset.page === name));
  $('#pagePlay').hidden = name !== 'play';
  $('#pageProgress').hidden = name !== 'progress';
  $('#pageFlags').hidden = name !== 'flags';
  $('#pageAchievements').hidden = name !== 'achievements';
  $('#pageExport').hidden = name !== 'export';
  if (name === 'progress') {
    renderProgressPage();
    loadLog();
  } else if (name === 'flags') {
    renderFlagsPage();
  } else if (name === 'achievements') {
    renderAchievementsPage();
  } else if (name === 'export') {
    loadExport();
  }
  updateHash();
}

function updateHash() {
  const key = S.chunk ? `k=${encodeURIComponent(S.chunk.key)}` : '';
  const page = S.page !== 'play' ? `p=${S.page}` : '';
  history.replaceState(null, '', `#${[key, page].filter(Boolean).join('&')}`);
}

// The top bar and the level card

function renderTopStats() {
  const s = S.state.summary;
  $('#statCleared').textContent = `${s.cleared} / ${s.total}`;
  $('#statLevel').textContent = `${s.level} · ${s.level_title}`;
  $('#statStreak').textContent = `${s.streak} d`;
  $('#hostLine').textContent = `${s.pct.toFixed(1)} % of ${s.total} chunks in ${s.files_total} files · ` +
    `snapshot ${S.state.taken.slice(0, 10)}`;
  const fc = $('#flagCount');
  fc.textContent = s.flags_open ? String(s.flags_open) : '';
  fc.classList.toggle('hot', s.flags_open > 0);
}

function renderXp() {
  const s = S.state.summary;
  const span = s.next_xp - s.level_xp;
  const p = span ? Math.min(100, 100 * (s.xp - s.level_xp) / span) : 0;
  $('#xpRing').style.setProperty('--pct', `${p}%`);
  $('#xpLevel').textContent = s.level;
  $('#xpTitle').textContent = `Level ${s.level} · ${s.level_title}`;
  $('#xpSub').textContent = `${s.xp} XP · ${s.next_xp - s.xp} to level ${s.level + 1} · ` +
    `${s.achievements} / ${s.achievements_total} achievements`;
  $('#xpPlayed').textContent = played(s.time_played);
  $('#xpToday').textContent = s.today;
  $('#xpTodayRw').textContent = s.today_rewrites;
  $('#xpFiles').textContent = `${s.files_done} / ${s.files_total}`;
}

// The chapters tree

function saveOpen() {
  try {
    localStorage.setItem('audit.open', JSON.stringify(S.open));
  } catch {
    // Storage blocked.
  }
}

function fileRow(f, current) {
  const done = f.total && f.cleared === f.total;
  const w = f.total ? 100 * f.cleared / f.total : 0;
  return `<div class="tree-file${done ? ' done' : ''}${current ? ' current' : ''}" data-rel="${esc(f.rel)}" title="${esc(f.rel)}">
    <span class="box">${done ? '' : `<i style="width:${w}%"></i>`}</span>
    <span class="nm">${esc(f.name)}</span>
    ${f.open_flags ? '<span class="fl" title="Open flags"></span>' : ''}
    <span class="ct">${f.cleared}/${f.total}</span></div>`;
}

function renderTree() {
  const cur = S.chunk ? S.chunk.file : null;
  const html = S.state.summary.chapters.map(ch => {
    const open = S.open[ch.id] || (S.chunk && S.chunk.chapter === ch.id);
    const done = ch.total && ch.cleared === ch.total;
    const secs = ch.sections.map(sec => {
      const sid = `${ch.id}/${sec.title}`;
      const sopen = S.open[sid] || (S.chunk && S.chunk.chapter === ch.id && S.chunk.section === sec.title);
      const sdone = sec.total && sec.cleared === sec.total;
      return `<div class="tree-sec${sopen ? ' open' : ''}${sdone ? ' done' : ''}" data-sid="${esc(sid)}">
        <div class="tree-sec-head"><span>${esc(sec.title)}</span><span class="ct">${sec.cleared}/${sec.total}</span></div>
        <div class="files">${sec.files.map(f => fileRow(f, f.rel === cur)).join('')}</div></div>`;
    }).join('');
    return `<div class="tree-ch${open ? ' open' : ''}${done ? ' done' : ''}" data-ch="${ch.id}">
      <div class="tree-ch-head"><span class="twisty">▶</span><span class="nm">${CHAPTER_ICONS[ch.id] || ''} ${esc(ch.title)}</span>
        <span class="ct">${ch.cleared}/${ch.total}${ch.open_flags ? ` · ${ch.open_flags} ⚑` : ''}</span>
        <div class="bar"><i style="width:${pct(ch.cleared, ch.total)}%"></i></div></div>
      <div class="tree-body">${secs}</div></div>`;
  }).join('');
  $('#chapterTree').innerHTML = html;
}

function wireTree(root) {
  root.addEventListener('click', e => {
    const chHead = e.target.closest('.tree-ch-head');
    if (chHead) {
      const ch = chHead.parentElement;
      ch.classList.toggle('open');
      S.open[ch.dataset.ch] = ch.classList.contains('open');
      saveOpen();
      return;
    }
    const secHead = e.target.closest('.tree-sec-head');
    if (secHead) {
      const sec = secHead.parentElement;
      sec.classList.toggle('open');
      S.open[sec.dataset.sid] = sec.classList.contains('open');
      saveOpen();
      return;
    }
    const file = e.target.closest('.tree-file');
    if (file) {
      openFile(file.dataset.rel);
    }
  });
}

async function openFile(rel, which = 'pending') {
  await act(async () => {
    const f = await api(`/api/file?rel=${encodeURIComponent(rel)}`);
    const key = which === 'pending' ? (f.first_pending || f.first_key) : f.first_key;
    showPage('play');
    await openChunk(key);
  });
}

// The sidebar's lists

function renderFlagsMini() {
  const flags = S.state.flags.filter(f => !f.resolved);
  $('#flagsOpen').textContent = flags.length ? `${flags.length} open` : 'none';
  $('#flagsMini').innerHTML = flags.length ? flags.slice(0, 8).map(f =>
    `<li data-key="${esc(f.key)}"><span class="ev flag"></span><span class="nm">${esc(shortPath(f.file))}</span>` +
    `<span class="tx">${esc(f.note || f.title)}</span><span class="tm">${when(f.at)}</span></li>`).join('') +
    (flags.length > 8 ? `<li class="empty" data-page-link="flags">and ${flags.length - 8} more…</li>` : '')
    : '<li class="empty">Nothing flagged. Press F on a chunk to flag it.</li>';
}

function eventLine(e) {
  const what = {
    accept: 'Accepted', rewrite: 'Rewrote', skip: 'Skipped', reset: 'Reopened', flag: 'Flagged',
    flag_edit: 'Edited flag', unflag: 'Unflagged', resolve: 'Resolved', reopen: 'Reopened flag',
    achievement: '🏆', apply: 'Applied to project',
  }[e.type] || e.type;
  const text = e.type === 'achievement' ? e.detail : e.type === 'apply' ? e.detail
    : `${what} · ${e.title || ''}${e.detail ? ` (${e.detail})` : ''}`;
  return `<li${e.key ? ` data-key="${esc(e.key)}"` : ''}><span class="ev ${e.type}"></span>` +
    (e.file ? `<span class="nm">${esc(shortPath(e.file))}</span>` : '') +
    `<span class="tx" title="${esc(text)}">${esc(text)}</span><span class="tm">${when(e.at)}</span></li>`;
}

function renderRecent() {
  const evs = S.state.events.slice(0, 12);
  $('#recentMini').innerHTML = evs.length ? evs.map(eventLine).join('')
    : '<li class="empty">Nothing yet. Accept or rewrite your first chunk!</li>';
}

// The chunk

async function openChunk(key) {
  if (!key) {
    toast('Nothing left to do: every chunk is cleared. 🎉', 'success');
    return;
  }
  const c = await api(`/api/chunk?key=${encodeURIComponent(key)}`);
  S.chunk = c;
  S.dirty = false;
  $('#celebrate').hidden = true;
  if (S.origMode === 'preview' && c.lang !== 'markdown') {
    S.origMode = 'code';
  }
  $('#origMode button[data-mode="preview"]').hidden = c.lang !== 'markdown';
  $$('#origMode button').forEach(b => b.classList.toggle('active', b.dataset.mode === S.origMode));
  renderCrumbs();
  setMineMode('edit');
  setEditor((c.record && c.record.rewrite) ?? c.text, c.lang, c.start);
  renderOriginal();
  renderStatus();
  renderStrip();
  renderCheckFromRecord();
  renderTree();
  updateHash();
  const row = $(`.tree-file[data-rel="${CSS.escape(c.file)}"]`);
  if (row) {
    row.scrollIntoView({ block: 'nearest' });
  }
}

function renderCrumbs() {
  const c = S.chunk;
  const fi = c.file_info;
  $('#crumbs').innerHTML = `<span>${esc(fi.chapter)}</span><span class="sep">›</span><span>${esc(fi.section)}</span>` +
    `<span class="sep">›</span><span class="here" title="${esc(c.file)}">${esc(shortPath(c.file))}</span>` +
    `<span class="sep">›</span><span class="ttl" title="${esc(c.title)}">${esc(c.title)}</span>`;
  $('#chunkPos').textContent = `${fi.pos + 1} / ${fi.count} · lines ${c.start}–${c.end}` +
    (c.part ? ` · part ${c.part[0]}/${c.part[1]}` : '');
  const lb = $('#langBadge');
  lb.textContent = c.lang;
  lb.dataset.lang = c.lang;
  $('#prevBtn').disabled = !c.prev;
  $('#nextBtn').disabled = !c.next;
}

function renderStatus() {
  const r = S.chunk.record || {};
  const st = r.status || 'pending';
  const flagged = r.flag && !r.flag.resolved;
  const pill = $('#statusPill');
  pill.className = `pill ${flagged ? 'flagged' : st}`;
  pill.textContent = { pending: 'Pending', accepted: 'Accepted', rewritten: 'Rewritten', skipped: 'Skipped' }[st] +
    (flagged ? ' · Flagged' : '') + (st === 'rewritten' && r.code_changed ? ' · Code Changed' : '');
  pill.title = r.at ? `${st} ${when(r.at)}${flagged ? `\nFlag: ${r.flag.note}` : ''}` : '';
  $('#resetBtn').hidden = st === 'pending';
  $('#flagBtn').textContent = '';
  $('#flagBtn').innerHTML = `${flagged ? 'Edit Flag…' : 'Flag…'} <kbd>F</kbd>`;
  $('#flagBtn').classList.toggle('btn-warn', !!flagged);
}

function renderStrip() {
  const c = S.chunk;
  const fi = c.file_info;
  $('#strip').innerHTML = c.strip.map((s, i) =>
    `<i class="${s.status}${s.flag ? ' flag' : ''}${s.key === c.key ? ' current' : ''}" data-key="${esc(s.key)}" ` +
    `title="${i + 1}. ${esc(s.title)} (lines ${s.start}–${s.end}) · ${s.status}${s.flag ? ' · flagged' : ''}"></i>`).join('');
  const cleared = c.strip.filter(s => s.status === 'accepted' || s.status === 'rewritten').length;
  const rw = c.strip.filter(s => s.status === 'rewritten').length;
  const fl = c.strip.filter(s => s.flag).length;
  $('#stripLabel').innerHTML = `<b>${esc(fi.name)}</b> · ${cleared}/${fi.count} cleared` +
    `${rw ? ` · ${rw} rewritten` : ''}${fl ? ` · ${fl} ⚑` : ''}<br>` +
    `<span class="muted">file ${fi.file_pos + 1} of ${fi.file_count} in ${esc(fi.section)}</span>`;
}

// The original pane

function highlighted(text, lang) {
  try {
    return S.lib.cm.highlightLines(text, lang);
  } catch {
    return text.split('\n').map(esc);
  }
}

function codeBlock(rows) {
  return `<div class="docs-code">${rows.map(r =>
    `<div class="l${r.cls ? ` ${r.cls}` : ''}"><span class="n">${r.n ?? ''}</span><span class="t">${r.html}</span></div>`).join('')}</div>`;
}

function renderOriginal() {
  const c = S.chunk;
  const body = $('#origBody');
  if (S.origMode === 'preview') {
    body.innerHTML = `<div class="docs-content">${S.marked.parse(c.text)}</div>`;
    return;
  }
  if (S.origMode === 'diff') {
    const mine = S.view ? S.view.state.doc.toString() : c.text;
    const ops = lineDiff(c.text.replace(/\n$/, '').split('\n'), mine.replace(/\n$/, '').split('\n'));
    if (!ops.some(o => o.t !== '=')) {
      body.innerHTML = '<div class="docs-code"><div class="l"><span class="n"></span><span class="t muted">No difference yet: your version is the original.</span></div></div>';
      return;
    }
    let a = c.start;
    let b = c.start;
    const rows = ops.map(o => {
      if (o.t === '=') {
        a++;
        b++;
        return { n: a - 1, html: esc(o.s) };
      }
      if (o.t === '-') {
        return { n: a++, html: esc(o.s), cls: 'del' };
      }
      return { n: b++, html: esc(o.s), cls: 'add' };
    });
    body.innerHTML = codeBlock(rows);
    return;
  }
  const ctx = $('#ctxToggle').checked ? c.context : null;
  const chunkLines = c.text.replace(/\n$/, '').split('\n');
  if (ctx && (ctx.before || ctx.after)) {
    const before = ctx.before.replace(/\n$/, '').split('\n');
    const after = ctx.after.replace(/\n$/, '').split('\n');
    const all = highlighted([ctx.before, c.text, ctx.after].join(''), c.lang);
    const rows = [];
    let n = ctx.before_start;
    let i = 0;
    const take = (count, cls) => {
      for (let k = 0; k < count; k++, i++, n++) {
        rows.push({ n, html: all[i] ?? '', cls });
      }
    };
    if (ctx.before) {
      take(before.length, 'ctx');
    }
    take(chunkLines.length, '');
    if (ctx.after) {
      take(after.length, 'ctx');
    }
    body.innerHTML = codeBlock(rows);
    return;
  }
  const lines = highlighted(c.text, c.lang);
  body.innerHTML = codeBlock(chunkLines.map((_, i) => ({ n: c.start + i, html: lines[i] ?? '' })));
}

/**
 * A line diff (longest common subsequence).
 * @param {!Array<string>} a
 * @param {!Array<string>} b
 * @return {!Array<{t: string, s: string}>} Lines with t '=', '-' or '+'.
 */
function lineDiff(a, b) {
  const n = a.length;
  const m = b.length;
  const L = Array.from({ length: n + 1 }, () => new Uint16Array(m + 1));
  for (let i = n - 1; i >= 0; i--) {
    for (let j = m - 1; j >= 0; j--) {
      L[i][j] = a[i] === b[j] ? L[i + 1][j + 1] + 1 : Math.max(L[i + 1][j], L[i][j + 1]);
    }
  }
  const out = [];
  let i = 0;
  let j = 0;
  while (i < n && j < m) {
    if (a[i] === b[j]) {
      out.push({ t: '=', s: a[i] });
      i++;
      j++;
    } else if (L[i + 1][j] >= L[i][j + 1]) {
      out.push({ t: '-', s: a[i++] });
    } else {
      out.push({ t: '+', s: b[j++] });
    }
  }
  while (i < n) {
    out.push({ t: '-', s: a[i++] });
  }
  while (j < m) {
    out.push({ t: '+', s: b[j++] });
  }
  return out;
}

// The editor

function setEditor(text, lang, start) {
  const cm = S.lib.cm;
  const extensions = [
    cm.lineNumbers({ formatNumber: n => String(n + start - 1) }),
    cm.highlightActiveLineGutter(), cm.highlightSpecialChars(), cm.history(),
    cm.drawSelection(), cm.dropCursor(), cm.indentOnInput(),
    cm.syntaxHighlighting(cm.classHighlighter), cm.bracketMatching(), cm.closeBrackets(),
    cm.highlightActiveLine(), cm.highlightSelectionMatches(), cm.highlightTrailingWhitespace(),
    cm.search({ top: true }), cm.indentUnit.of(INDENT[lang] || '  '), cm.EditorState.tabSize.of(4),
    cm.keymap.of([
      { key: 'Ctrl-Enter', mac: 'Cmd-Enter', run: () => { saveRewrite(); return true; } },
      { key: 'Escape', run: v => { v.contentDOM.blur(); return true; } },
      ...cm.closeBracketsKeymap, ...cm.defaultKeymap, ...cm.searchKeymap, ...cm.historyKeymap,
      cm.indentWithTab,
    ]),
    cm.languageFor(lang),
    cm.EditorView.updateListener.of(u => {
      if (u.docChanged) {
        onEdit();
      }
    }),
    cm.EditorView.theme({}, { dark: true }),
  ];
  extensions.push(cm.EditorView.lineWrapping);
  const state = cm.EditorState.create({ doc: text, extensions });
  if (!S.view) {
    S.view = new cm.EditorView({ state, parent: $('#editorHost') });
  } else {
    S.view.setState(state);
  }
}

function editorText() {
  return S.view ? S.view.state.doc.toString() : '';
}

function onEdit() {
  S.dirty = true;
  clearTimeout(S.checkTimer);
  S.checkTimer = setTimeout(runCheck, CHECK_DELAY);
  if (S.origMode === 'diff') {
    renderOriginal();
  }
  if (S.mineMode === 'preview') {
    renderMinePreview();
  }
}

/** Your version rendered: Markdown as a page, anything else highlighted. */
function renderMinePreview() {
  const c = S.chunk;
  const text = editorText();
  const el = $('#minePreview');
  if (c.lang === 'markdown') {
    el.innerHTML = `<div class="docs-content">${S.marked.parse(text)}</div>`;
    return;
  }
  const lines = text.replace(/\n$/, '').split('\n');
  const html = highlighted(text, c.lang);
  el.innerHTML = codeBlock(lines.map((_, i) => ({ n: c.start + i, html: html[i] ?? '' })));
}

function setMineMode(mode) {
  S.mineMode = mode;
  $$('#mineMode button').forEach(b => b.classList.toggle('active', b.dataset.mode === mode));
  $('#editorHost').hidden = mode !== 'edit';
  $('#minePreview').hidden = mode !== 'preview';
  if (mode === 'preview') {
    renderMinePreview();
  } else if (S.view) {
    S.view.requestMeasure();
  }
}

function setCheck(cls, text, detail = '') {
  const el = $('#checkLine');
  el.className = `check-line ${cls}`;
  el.textContent = text;
  const d = $('#checkDetail');
  d.hidden = !detail;
  d.innerHTML = detail;
}

function renderCheckFromRecord() {
  const r = S.chunk.record || {};
  if (r.status === 'rewritten') {
    if (r.code_changed === null || r.code_changed === undefined) {
      setCheck('prose', 'Rewritten');
    } else if (r.code_changed) {
      setCheck('code', 'Rewritten · the code changed');
      runCheck();
    } else {
      setCheck('comments', 'Rewritten · comments only');
    }
  } else {
    setCheck('same', 'Unchanged');
  }
}

async function runCheck() {
  const c = S.chunk;
  if (!c) {
    return;
  }
  const text = editorText();
  if (text === c.text) {
    setCheck('same', 'Unchanged');
    return;
  }
  if (c.kind === 'prose') {
    setCheck('prose', 'Rewritten');
    return;
  }
  try {
    const r = await api('/api/check', { key: c.key, text });
    if (S.chunk !== c || editorText() !== text) {
      return;
    }
    if (r.changed) {
      setCheck('code', 'The code changed, not just the comments',
        `First difference at token ${r.where + 1} of ${r.tokens}:\n<b>was</b>  ${esc(r.was)}\n<b>now</b>  ${esc(r.now)}`);
    } else {
      setCheck('comments', 'Comments only · the code is unchanged');
    }
  } catch (e) {
    setCheck('same', e.message);
  }
}

// Actions

function nextKeyAfterAction() {
  return S.chunk.next_pending_in_file || S.chunk.next_pending;
}

async function afterAction(resp, message) {
  const c = S.chunk;
  c.record = resp.record;
  S.state.summary = resp.summary;
  S.state.flags = resp.flags;
  const mine = c.strip.find(s => s.key === c.key);
  if (mine) {
    mine.status = (resp.record && resp.record.status) || 'pending';
    mine.flag = !!(resp.record && resp.record.flag && !resp.record.flag.resolved);
  }
  renderTopStats();
  renderXp();
  renderTree();
  renderFlagsMini();
  renderStatus();
  renderStrip();
  if (message) {
    toast(message, 'success', 2500);
  }
  (resp.unlocked || []).forEach(a => toast(a.title, 'trophy', 8000, a.desc));
  loadState().catch(() => {});
  const cleared = resp.record && ['accepted', 'rewritten'].includes(resp.record.status);
  const m = resp.milestones;
  if (cleared && m && m.file_done && !c.celebrated) {
    c.celebrated = true;
    showCelebrate(m, resp.unlocked || []);
    return;
  }
  if (cleared && $('#autoAdvance').checked) {
    const nk = nextKeyAfterAction();
    if (nk) {
      await openChunk(nk);
    } else {
      toast('That was the last pending chunk. 🎉', 'success', 8000);
    }
  }
}

async function accept() {
  await act(async () => {
    const resp = await api('/api/chunk/accept', { key: S.chunk.key });
    await afterAction(resp, 'Accepted');
  });
}

async function saveRewrite() {
  await act(async () => {
    const text = editorText();
    if (text === S.chunk.text) {
      const resp = await api('/api/chunk/accept', { key: S.chunk.key });
      await afterAction(resp, 'No change: accepted as it is');
      return;
    }
    if (!text.trim()) {
      throw new Error('The rewrite is empty. To drop a chunk, write a comment saying so.');
    }
    const resp = await api('/api/chunk/rewrite', { key: S.chunk.key, text });
    const changed = resp.check && resp.check.changed;
    await afterAction(resp, changed ? 'Rewrite saved (the code changed)' : 'Rewrite saved');
  });
}

async function skip() {
  await act(async () => {
    const resp = await api('/api/chunk/skip', { key: S.chunk.key });
    await afterAction(resp, '');
    const nk = S.chunk.next_pending;
    if (nk) {
      await openChunk(nk);
    }
  });
}

async function reset() {
  await act(async () => {
    const resp = await api('/api/chunk/reset', { key: S.chunk.key });
    await afterAction(resp, 'Back to pending');
    setEditor(S.chunk.text, S.chunk.lang, S.chunk.start);
    renderCheckFromRecord();
  });
}

function openFlagDialog(key, file, title, note, exists) {
  const dlg = $('#flagDialog');
  dlg.dataset.key = key;
  $('#flagDialogTitle').textContent = exists ? 'Edit the Flag' : 'Flag This Chunk';
  $('#flagDialogWhere').textContent = `${shortPath(file)} · ${title}`;
  $('#flagNote').value = note || '';
  $('#flagRemove').hidden = !exists;
  dlg.showModal();
  $('#flagNote').focus();
}

function flagCurrent() {
  const c = S.chunk;
  const f = c.record && c.record.flag && !c.record.flag.resolved ? c.record.flag : null;
  openFlagDialog(c.key, c.file, c.title, f ? f.note : '', !!f);
}

async function flagAction(action, key, note) {
  await act(async () => {
    const resp = await api(`/api/chunk/${action}`, { key, note });
    if (S.chunk && S.chunk.key === key) {
      await afterAction(resp, { flag: 'Flagged', unflag: 'Flag removed', resolve: 'Flag resolved',
        reopen: 'Flag reopened' }[action]);
    } else {
      S.state.summary = resp.summary;
      S.state.flags = resp.flags;
      (resp.unlocked || []).forEach(a => toast(a.title, 'trophy', 8000, a.desc));
      renderAll();
      loadState().catch(() => {});
    }
    if (S.page === 'flags') {
      renderFlagsPage();
    }
  });
}

// Celebration

function showCelebrate(m, unlocked) {
  const c = S.chunk;
  const big = unlocked.find(a => a.id.startsWith('chapter:'));
  const sec = unlocked.find(a => a.id.startsWith('section:'));
  const el = $('#celebrate');
  const gold = big || sec;
  el.innerHTML = `<div class="confetti">${gold ? confetti() : ''}</div>
    <div class="celebrate-card${gold ? ' gold' : ''}">
      <div class="big">${big ? '🏆' : sec ? '🎉' : '✅'}</div>
      <h2>${big ? esc(big.title) : sec ? esc(sec.title) : 'File cleared!'}</h2>
      <p>${esc(c.file_info.name)} · every one of its ${m.total} chunks is done</p>
      <div class="stats">
        <div><b>${m.accepted}</b><span>accepted</span></div>
        <div><b>${m.rewritten}</b><span>rewritten</span></div>
        <div><b>${m.open_flags}</b><span>open flags</span></div>
      </div>
      ${unlocked.map(a => `<div class="ach">🏆 ${esc(a.title)} — <span class="muted">${esc(a.desc)}</span></div>`).join('')}
      <div class="row">
        <button type="button" class="btn btn-go" id="celebrateNext">${c.next_pending ? 'Continue' : 'All Done'} <kbd>⏎</kbd></button>
        <button type="button" class="btn" id="celebrateStay">Stay Here <kbd>Esc</kbd></button>
      </div>
    </div>`;
  el.hidden = false;
  $('#celebrateNext').onclick = () => {
    el.hidden = true;
    if (c.next_pending) {
      act(() => openChunk(c.next_pending));
    }
  };
  $('#celebrateStay').onclick = () => {
    el.hidden = true;
  };
  $('#celebrateNext').focus();
}

function confetti() {
  const colors = ['#4c8dff', '#3ccf7e', '#f5a524', '#e5484d', '#f2c14e', '#b79cf2'];
  return Array.from({ length: 60 }, (_, i) =>
    `<i style="left:${Math.random() * 100}%;background:${colors[i % colors.length]};` +
    `animation-delay:${Math.random() * 1.2}s;animation-duration:${2 + Math.random() * 2}s"></i>`).join('');
}

// Navigation

function keyHandler(e) {
  if ($$('dialog[open]').length) {
    return;
  }
  const cel = $('#celebrate');
  if (!cel.hidden) {
    if (e.key === 'Enter') {
      $('#celebrateNext').click();
    } else if (e.key === 'Escape') {
      cel.hidden = true;
    }
    return;
  }
  if (S.page !== 'play') {
    return;
  }
  const t = e.target;
  const inEditor = t.closest && (t.closest('.cm-editor') || ['INPUT', 'TEXTAREA'].includes(t.tagName));
  if (inEditor || e.ctrlKey || e.metaKey || e.altKey) {
    return;
  }
  const k = e.key.toLowerCase();
  if (k === 'a') {
    accept();
  } else if (k === 's') {
    skip();
  } else if (k === 'f') {
    flagCurrent();
  } else if (k === 'e' || k === 'r') {
    setMineMode('edit');
    S.view.focus();
  } else if (k === 'enter') {
    goNextPending();
  } else if (k === 'arrowright' || k === 'n') {
    if (S.chunk.next) {
      act(() => openChunk(S.chunk.next));
    }
  } else if (k === 'arrowleft' || k === 'p') {
    if (S.chunk.prev) {
      act(() => openChunk(S.chunk.prev));
    }
  } else if (k === 'd') {
    setOrigMode(S.origMode === 'diff' ? 'code' : 'diff');
  } else if (k === 'v') {
    setMineMode(S.mineMode === 'preview' ? 'edit' : 'preview');
  } else if (k === 'c') {
    $('#ctxToggle').checked = !$('#ctxToggle').checked;
    renderOriginal();
  } else if (k === '?') {
    $('#helpDialog').showModal();
  } else {
    return;
  }
  e.preventDefault();
}

function goNextPending() {
  act(async () => {
    const r = await api(`/api/next?after=${encodeURIComponent(S.chunk ? S.chunk.key : '')}`);
    if (r.key) {
      await openChunk(r.key);
    } else {
      toast('Nothing pending: every chunk is cleared. 🎉', 'success');
    }
  });
}

function setOrigMode(mode) {
  S.origMode = mode;
  $$('#origMode button').forEach(b => b.classList.toggle('active', b.dataset.mode === mode));
  renderOriginal();
}

async function neighbourFile(dir) {
  const chs = S.state.summary.chapters;
  const all = chs.flatMap(ch => ch.sections.flatMap(s => s.files.map(f => f.rel)));
  const i = all.indexOf(S.chunk.file);
  const rel = all[i + dir];
  if (rel) {
    await openFile(rel, 'first');
  }
}

// The progress page

function renderProgressPage() {
  const s = S.state.summary;
  $('#progressTiles').innerHTML = [
    [`${s.pct.toFixed(1)} %`, `${s.cleared} of ${s.total} chunks cleared`],
    [`${s.files_done} / ${s.files_total}`, 'files cleared'],
    [`${s.rewritten}`, `rewrites (${s.accepted} accepted as is)`],
    [`${s.xp} XP`, `level ${s.level} · ${s.level_title}`],
    [`${s.streak} d`, 'streak'],
    [`${s.flags_open}`, `open flags of ${s.flags_raised} raised`],
    [`${s.achievements} / ${s.achievements_total}`, 'achievements'],
    [`${s.today}`, 'cleared today'],
  ].map(([b, t]) => `<div class="stat-tile"><b>${esc(b)}</b><span>${esc(t)}</span></div>`).join('');

  const days = [];
  const d = new Date();
  for (let i = DAYS_SHOWN - 1; i >= 0; i--) {
    const x = new Date(d);
    x.setDate(d.getDate() - i);
    const iso = x.toISOString().slice(0, 10);
    days.push({ iso, n: S.state.days[iso] || 0 });
  }
  const max = Math.max(1, ...days.map(x => x.n));
  $('#days').innerHTML = days.map((x, i) =>
    `<i class="${i === days.length - 1 ? 'today' : ''}" style="height:${Math.max(3, 100 * x.n / max)}%" title="${x.iso}: ${x.n}"></i>`).join('');
  $('#daysAxis').innerHTML = `<span>${days[0].iso}</span><span>today</span>`;
  $('#daysTotal').textContent = `${days.reduce((a, x) => a + x.n, 0)} in the last ${DAYS_SHOWN} days`;

  const cur = S.chunk ? S.chunk.file : null;
  $('#checklist').innerHTML = s.chapters.map(ch => {
    const done = ch.total && ch.cleared === ch.total;
    return `<div class="check-ch${done ? ' done' : ''}">
      <div class="check-ch-head"><h3>${CHAPTER_ICONS[ch.id] || ''} ${esc(ch.title)}</h3><span class="blurb">${esc(ch.blurb)}</span>
        <span class="ct">${ch.cleared}/${ch.total} · ${pct(ch.cleared, ch.total)} %${ch.rewritten ? ` · ${ch.rewritten} rewritten` : ''}${ch.open_flags ? ` · ${ch.open_flags} ⚑` : ''}</span>
        <div class="bar lg"><i style="width:${pct(ch.cleared, ch.total)}%"></i></div></div>
      ${ch.sections.map(sec => `<div class="check-sec${sec.total && sec.cleared === sec.total ? ' done' : ''}">
        <div class="check-sec-head"><span>${esc(sec.title)}</span><span class="ct">${sec.cleared}/${sec.total}</span></div>
        <div class="check-files">${sec.files.map(f => fileRow(f, f.rel === cur)).join('')}</div></div>`).join('')}
    </div>`;
  }).join('');
}

async function loadLog() {
  try {
    const r = await api('/api/log?limit=200');
    $('#logTotal').textContent = `${r.total} entries`;
    $('#logList').innerHTML = r.events.length ? r.events.map(eventLine).join('')
      : '<li class="empty">Nothing yet.</li>';
  } catch (e) {
    toast(e.message, 'error');
  }
}

// The flags page

function renderFlagsPage() {
  const flags = S.state.flags;
  const open = flags.filter(f => !f.resolved);
  const done = flags.filter(f => f.resolved);
  const card = f => `<div class="flag-card${f.resolved ? ' resolved' : ''}" data-key="${esc(f.key)}">
    <div class="where"><span class="lang-badge" data-lang="${esc(S.state.summary.chapters.find(c => c.id === f.chapter)?.id || '')}">${esc(f.chapter)}</span>
      <span class="nm" title="${esc(f.file)}">${esc(shortPath(f.file))}</span><span class="muted">· lines ${f.start}–${f.end} · ${esc(f.status)}</span></div>
    <div class="title">${esc(f.title)}</div>
    <div class="note">${esc(f.note)}</div>
    <div class="meta">flagged ${when(f.at)}${f.resolved ? ` · resolved ${when(f.resolved)}` : ''}</div>
    <div class="row">
      <button type="button" class="btn btn-sm btn-primary" data-act="go">Go to Chunk</button>
      <button type="button" class="btn btn-sm" data-act="edit">Edit Note…</button>
      <button type="button" class="btn btn-sm ${f.resolved ? '' : 'btn-go'}" data-act="${f.resolved ? 'reopen' : 'resolve'}">${f.resolved ? 'Reopen' : 'Resolve'}</button>
      <button type="button" class="btn btn-sm btn-ghost" data-act="unflag">Remove</button>
    </div></div>`;
  $('#flagsList').innerHTML = (open.length ? `<h3 class="muted small" style="letter-spacing:.09em;text-transform:uppercase">Open · ${open.length}</h3>` + open.map(card).join('')
    : '<div class="card"><p class="empty">No open flags. Press F on any chunk to flag it with a note.</p></div>') +
    (done.length ? `<h3 class="muted small" style="letter-spacing:.09em;text-transform:uppercase;margin-top:12px">Resolved · ${done.length}</h3>` + done.map(card).join('') : '');
}

// The achievements page

function renderAchievementsPage() {
  const groups = [['general', 'Milestones'], ['sections', 'Sections'], ['chapters', 'Chapters']];
  const achs = S.state.achievements;
  $('#achList').innerHTML = groups.map(([g, title]) => {
    const list = achs.filter(a => a.group === g);
    const got = list.filter(a => a.at).length;
    return `<div class="ach-group"><h3>${title} · ${got} / ${list.length}</h3><div class="ach-grid">${list.map(a => {
      const icon = ACH_ICONS[a.id] || (a.id.startsWith('chapter:') ? '🏆' : CHAPTER_ICONS[a.chapter] || '⭐');
      return `<div class="ach ${a.at ? 'got' : 'locked'}"><div class="ic">${a.at ? icon : '🔒'}</div>
        <div class="tx"><div class="nm">${esc(a.title)}</div><div class="ds">${esc(a.desc)}</div>${a.at ? `<div class="at">earned ${esc(a.at.replace('T', ' ').slice(0, 16))}</div>` : ''}</div></div>`;
    }).join('')}</div></div>`;
  }).join('');
}

// The export page

async function loadExport() {
  try {
    const r = await api('/api/export');
    renderExport(r);
  } catch (e) {
    toast(e.message, 'error');
  }
}

function renderExport(r) {
  $('#exportNote').innerHTML = `Every file with a rewrite is kept whole, with your chunks in place of the originals, in ` +
    `<code>${esc(r.work_dir)}</code>, under the same path as in the project (<code>${esc(r.repo)}</code>). ` +
    `When you are done, apply a file (or all of them) to copy it over the project's copy; the old copy goes to ` +
    `<code>audit/backup/&lt;date&gt;/</code> first. A file marked <b>changed since the snapshot</b> was edited in the project after the game started: look at it before applying.`;
  $('#applyAllBtn').disabled = !r.files.length;
  if (!r.files.length) {
    $('#exportBody').innerHTML = '<p class="empty">No rewrites yet. Files appear here once you save a rewrite.</p>';
    return;
  }
  $('#exportBody').innerHTML = `<table class="export-table"><thead><tr><th>File</th><th></th><th>Chunks</th><th>Lines</th><th></th></tr></thead><tbody>` +
    r.files.map(f => `<tr data-rel="${esc(f.rel)}">
      <td class="nm">${esc(f.rel)}${f.drift ? '<br><span class="drift">⚠ changed in the project since the snapshot</span>' : ''}</td>
      <td><span class="lang-badge" data-lang="${esc(f.lang)}">${esc(f.lang)}</span>${f.code_changed ? ' <span class="drift" title="At least one rewrite changed the code, not just its comments">code</span>' : ''}</td>
      <td class="num">${f.rewritten} / ${f.total}</td>
      <td class="num"><span class="add">+${f.added}</span> <span class="del">−${f.removed}</span></td>
      <td><div class="row"><button type="button" class="btn btn-sm" data-act="diff">Diff</button>
        <button type="button" class="btn btn-sm" data-act="open">Open</button>
        <button type="button" class="btn btn-sm btn-warn" data-act="apply">Apply…</button></div></td></tr>`).join('') +
    '</tbody></table>';
}

async function showDiff(rel) {
  await act(async () => {
    const r = await api(`/api/export/diff?rel=${encodeURIComponent(rel)}`);
    $('#diffTitle').textContent = rel;
    $('#diffBody').innerHTML = r.diff.split('\n').map(l => {
      const cls = l.startsWith('+++') || l.startsWith('---') ? 'h' : l.startsWith('@@') ? 'm'
        : l.startsWith('+') ? 'a' : l.startsWith('-') ? 'd' : '';
      return `<span class="${cls}">${esc(l)}</span>`;
    }).join('\n');
    $('#diffDialog').showModal();
  });
}

function confirm(title, body) {
  return new Promise(resolve => {
    const dlg = $('#confirmDialog');
    $('#confirmTitle').textContent = title;
    $('#confirmBody').innerHTML = body;
    const done = v => {
      dlg.close();
      resolve(v);
    };
    $('#confirmYes').onclick = () => done(true);
    $('#confirmNo').onclick = () => done(false);
    dlg.oncancel = () => done(false);
    dlg.showModal();
  });
}

async function applyFiles(rels) {
  const ok = await confirm(rels.length === 1 ? 'Apply this file to the project?' : `Apply ${rels.length} files to the project?`,
    `<p>The project's copies will be replaced by your rewritten versions. The old copies are saved under <code>audit/backup/</code> first.</p>` +
    `<ul>${rels.map(r => `<li class="mono">${esc(r)}</li>`).join('')}</ul>`);
  if (!ok) {
    return;
  }
  await act(async () => {
    const r = await api('/api/export/apply', { rels });
    const good = r.results.filter(x => x.ok).length;
    const bad = r.results.filter(x => !x.ok);
    toast(`${good} file${good === 1 ? '' : 's'} applied to the project.`, 'success', 6000, good ? r.results.find(x => x.ok).note : '');
    bad.forEach(x => toast(`${x.rel}: ${x.note}`, 'error', 9000));
    renderExport(r.export);
    loadState().catch(() => {});
  });
}

// Markdown

function makeMarked(lib) {
  const math = (tex, display) => {
    try {
      return lib.katex.renderToString(tex, { displayMode: display, throwOnError: false });
    } catch {
      return `<code>${esc(tex)}</code>`;
    }
  };
  const marked = new lib.Marked({ gfm: true });
  marked.use({
    extensions: [{
      name: 'mathBlock', level: 'block',
      start(src) {
        const i = src.indexOf('$$');
        return i < 0 ? undefined : i;
      },
      tokenizer(src) {
        const m = /^\$\$([\s\S]+?)\$\$[^\S\n]*(?:\n|$)/.exec(src);
        return m ? { type: 'mathBlock', raw: m[0], text: m[1].trim() } : undefined;
      },
      renderer: tok => `<div class="docs-math">${math(tok.text, true)}</div>`,
    }, {
      name: 'mathInline', level: 'inline',
      start(src) {
        const i = src.indexOf('$');
        return i < 0 ? undefined : i;
      },
      tokenizer(src) {
        const block = /^\$\$([\s\S]+?)\$\$/.exec(src);
        if (block) {
          return { type: 'mathInline', raw: block[0], text: block[1].trim(), display: true };
        }
        const m = /^\$(?!\s)((?:\\\$|[^$\n])+?)\$(?!\d)/.exec(src);
        return m ? { type: 'mathInline', raw: m[0], text: m[1] } : undefined;
      },
      renderer: tok => math(tok.text, !!tok.display),
    }],
    renderer: {
      image(a, b, c) {
        const href = typeof a === 'object' ? a.href : a;
        const text = typeof a === 'object' ? a.text : c;
        return `<em class="muted">[image: ${esc(text || href)}]</em>`;
      },
      link(a, b, c) {
        const href = typeof a === 'object' ? a.href : a;
        const text = typeof a === 'object' ? a.text : c;
        if (/^(page|file|ref):/.test(href || '')) {
          return `<a title="${esc(href)}">${text}</a>`;
        }
        return `<a href="${esc(href)}" target="_blank" rel="noopener">${text}</a>`;
      },
      code(a, b) {
        const text = typeof a === 'object' ? a.text : a;
        const lang = typeof a === 'object' ? a.lang : b;
        const l = (lang || '').split(/\s/)[0];
        return `<pre>${highlighted(text, l).join('\n')}</pre>`;
      },
    },
  });
  return marked;
}

// Wiring

function wireUi() {
  $$('.page-tabs button').forEach(b => b.addEventListener('click', () => showPage(b.dataset.page)));
  document.addEventListener('click', e => {
    const link = e.target.closest('[data-page-link]');
    if (link) {
      showPage(link.dataset.pageLink);
    }
    const close = e.target.closest('[data-close]');
    if (close) {
      close.closest('dialog').close();
    }
  });
  wireTree($('#chapterTree'));
  $('#checklist').addEventListener('click', e => {
    const file = e.target.closest('.tree-file');
    if (file) {
      openFile(file.dataset.rel);
    }
  });
  const goKey = e => {
    const li = e.target.closest('li[data-key]');
    if (li) {
      showPage('play');
      act(() => openChunk(li.dataset.key));
    }
  };
  $('#flagsMini').addEventListener('click', goKey);
  $('#recentMini').addEventListener('click', goKey);
  $('#logList').addEventListener('click', goKey);
  $('#nextPendingBtn').addEventListener('click', goNextPending);
  $('#prevBtn').addEventListener('click', () => S.chunk.prev && act(() => openChunk(S.chunk.prev)));
  $('#nextBtn').addEventListener('click', () => S.chunk.next && act(() => openChunk(S.chunk.next)));
  $('#filePrevBtn').addEventListener('click', () => neighbourFile(-1));
  $('#fileNextBtn').addEventListener('click', () => neighbourFile(1));
  $('#helpBtn').addEventListener('click', () => $('#helpDialog').showModal());
  $('#origMode').addEventListener('click', e => {
    const b = e.target.closest('button');
    if (b) {
      setOrigMode(b.dataset.mode);
    }
  });
  $('#ctxToggle').addEventListener('change', renderOriginal);
  $('#mineMode').addEventListener('click', e => {
    const b = e.target.closest('button');
    if (b) {
      setMineMode(b.dataset.mode);
    }
  });
  $('#strip').addEventListener('click', e => {
    const i = e.target.closest('i[data-key]');
    if (i) {
      act(() => openChunk(i.dataset.key));
    }
  });
  $('#acceptBtn').addEventListener('click', accept);
  $('#saveBtn').addEventListener('click', saveRewrite);
  $('#skipBtn').addEventListener('click', skip);
  $('#resetBtn').addEventListener('click', reset);
  $('#flagBtn').addEventListener('click', flagCurrent);
  $('#restoreBtn').addEventListener('click', () => {
    setEditor(S.chunk.text, S.chunk.lang, S.chunk.start);
    runCheck();
  });
  $('#autoAdvance').checked = S.state ? S.state.settings.autoAdvance !== false : true;
  $('#autoAdvance').addEventListener('change', () => {
    api('/api/settings', { settings: { autoAdvance: $('#autoAdvance').checked } }).catch(() => {});
  });

  // The flag dialog.
  const fd = $('#flagDialog');
  $('#flagSave').addEventListener('click', () => {
    const note = $('#flagNote').value.trim();
    fd.close();
    flagAction('flag', fd.dataset.key, note);
  });
  $('#flagRemove').addEventListener('click', () => {
    fd.close();
    flagAction('unflag', fd.dataset.key, '');
  });
  $('#flagCancel').addEventListener('click', () => fd.close());
  $('#flagNote').addEventListener('keydown', e => {
    if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) {
      $('#flagSave').click();
    }
  });

  // The flags page.
  $('#flagsList').addEventListener('click', e => {
    const b = e.target.closest('button[data-act]');
    if (!b) {
      return;
    }
    const key = b.closest('.flag-card').dataset.key;
    const f = S.state.flags.find(x => x.key === key);
    if (b.dataset.act === 'go') {
      showPage('play');
      act(() => openChunk(key));
    } else if (b.dataset.act === 'edit') {
      openFlagDialog(key, f.file, f.title, f.note, true);
    } else {
      flagAction(b.dataset.act, key, '');
    }
  });

  // Save files.
  $('#saveGameBtn').addEventListener('click', () => openSaves('save'));
  $('#loadGameBtn').addEventListener('click', () => openSaves('load'));
  $('#saveSlotBtn').addEventListener('click', () => {
    const name = $('#saveName').value.trim();
    if (!name) {
      $('#saveName').focus();
      return;
    }
    saveSlot(name, false);
  });
  $('#saveName').addEventListener('keydown', e => {
    if (e.key === 'Enter') {
      e.preventDefault();
      $('#saveSlotBtn').click();
    }
  });
  $('#savesList').addEventListener('click', e => {
    const b = e.target.closest('button[data-act]');
    if (!b) {
      return;
    }
    const row = b.closest('.save-row');
    const file = row.dataset.file;
    const name = row.dataset.name;
    if (b.dataset.act === 'load') {
      loadSlot(file, name);
    } else if (b.dataset.act === 'overwrite') {
      saveSlot(name, true);
    } else if (b.dataset.act === 'delete') {
      deleteSlot(file, name);
    }
  });

  // The export page.
  $('#exportRefresh').addEventListener('click', loadExport);
  $('#applyAllBtn').addEventListener('click', () => {
    const rels = $$('#exportBody tr[data-rel]').map(tr => tr.dataset.rel);
    if (rels.length) {
      applyFiles(rels);
    }
  });
  $('#exportBody').addEventListener('click', e => {
    const b = e.target.closest('button[data-act]');
    if (!b) {
      return;
    }
    const rel = b.closest('tr').dataset.rel;
    if (b.dataset.act === 'diff') {
      showDiff(rel);
    } else if (b.dataset.act === 'open') {
      openFile(rel, 'first');
    } else if (b.dataset.act === 'apply') {
      applyFiles([rel]);
    }
  });

  document.addEventListener('keydown', keyHandler);
  window.addEventListener('hashchange', () => {
    const m = /k=([^&]+)/.exec(location.hash);
    const key = m ? decodeURIComponent(m[1]) : null;
    if (key && S.chunk && key !== S.chunk.key) {
      act(() => openChunk(key));
    }
  });
}

boot().catch(e => {
  $('#hostLine').textContent = `Could not start: ${e.message}`;
  toast(e.message, 'error', 15000);
});
