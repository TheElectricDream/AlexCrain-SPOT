// The Constants window (local console): every constant of the project's Constants.m, in the
// file's own sections, with its value, unit and meaning, editable in place. MATLAB reads the file
// (spot.params.constantsCatalog) and writes the edits (spot.params.parseConstant, then
// writeConstants: only the lines that changed, comments kept); the worker then applies the
// settings again and the server sends the new constants, design and calibration to every page,
// so the rest of the console follows.


// Short words that stay lowercase in a title (UI text style), unless first or after a colon.
const SMALL_WORDS = new Set(['a', 'an', 'the', 'and', 'or', 'of', 'to', 'in', 'on', 'for',
  'at', 'by', 'as', 'its']);

/**
 * A heading in title case.
 * @param {string} text The file's section title ("The table and gravity").
 * @return {string} "The Table and Gravity".
 */
function titleCase(text) {
  let first = true;
  return text.split(' ').map(word => {
    const lower = word.toLowerCase();
    const out = !first && SMALL_WORDS.has(lower) ? lower
      : word.charAt(0).toUpperCase() + word.slice(1);
    first = word.endsWith(':');
    return out;
  }).join(' ');
}

const esc = s => String(s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));

/**
 * A value as the window shows and edits it: numbers in full precision (an edit of one element
 * must not round the others), NaN for MATLAB's NaN (null in JSON), [a b; c d] for a matrix.
 * @param {*} v The value from Constants.all().
 * @return {string} Its text.
 */
function valueText(v) {
  const num = x => (x == null ? 'NaN' : String(x));
  if (typeof v === 'string') {
    return v;
  }
  if (!Array.isArray(v)) {
    return num(v);
  }
  if (v.length && Array.isArray(v[0])) {
    return `[${v.map(row => row.map(num).join(' ')).join('; ')}]`;
  }
  return `[${v.map(num).join(' ')}]`;
}

/**
 * What kind of value a constant holds, which decides its editor and its check.
 * @param {*} v The value.
 * @return {string} 'bool', 'text', 'number', 'bools' (a logical vector) or 'numbers'.
 */
function kindOf(v) {
  if (typeof v === 'boolean') {
    return 'bool';
  }
  if (typeof v === 'string') {
    return 'text';
  }
  if (Array.isArray(v)) {
    return v.length && v.flat().every(x => typeof x === 'boolean') ? 'bools' : 'numbers';
  }
  return 'number';
}

/**
 * Whether typed text reads as a value of that kind (MATLAB checks again: nothing is evaluated).
 * @param {string} kind kindOf the constant.
 * @param {string} text What was typed.
 * @return {boolean} True if valid.
 */
function validText(kind, text) {
  if (kind === 'text') {
    return true;
  }
  const inner = text.trim().replace(/^\[(.*)\]$/s, '$1').trim();
  if (inner === '') {
    return kind !== 'number';
  }
  const rows = inner.split(';').map(r => r.trim().split(/[\s,]+/).filter(Boolean));
  if (rows.some(r => r.length !== rows[0].length)) {
    return false;
  }
  const items = rows.flat();
  if (kind === 'number' && items.length !== 1) {
    return false;
  }
  return items.every(x => (kind === 'bools' ? /^(true|false|0|1)$/i.test(x)
    : /^nan$/i.test(x) || /^[-+]?inf$/i.test(x) || Number.isFinite(Number(x))));
}

/**
 * Sets up the Constants window.
 * @param {{matlab: function(string, Object=): !Promise, act: function(function()),
 *     toast: function(string, string=, number=), onSaved: function(!Object)}} deps MATLAB's
 *     command call, the page's action wrapper (reports errors), its toast, and what to do with
 *     the worker's reply after a save (the Simulation tab applies the settings in it).
 * @return {{open: function(), refresh: function()}} Opening it, and reading it again (the
 *     constants changed elsewhere).
 */
export function initConstantsDialog({ matlab, act, toast, onSaved }) {
  const dialog = document.getElementById('constantsDialog');
  const nav = document.getElementById('constNav');
  const list = document.getElementById('constList');
  const search = document.getElementById('constSearch');
  const editedOnly = document.getElementById('constEdited');
  const status = document.getElementById('constStatus');
  const saveBtn = document.getElementById('constSave');
  const revertBtn = document.getElementById('constRevert');
  const S = { catalog: null, edits: new Map(), saving: false };

  // Reading the catalog

  async function load() {
    S.catalog = await matlab('constants.catalog');
    document.getElementById('constFile').textContent = S.catalog.file;
    render();
  }

  // The entries of a section (a struct array of one comes back as an object).
  const entriesOf = section => [].concat(section.entries || []);

  // Drawing

  function render() {
    const sections = [].concat(S.catalog.sections || []);
    nav.innerHTML = sections.map((s, i) => `<button type="button" data-constsec="${i}" ` +
      `title="${esc(s.description)}">${esc(titleCase(s.title))}` +
      `<span class="const-count">${entriesOf(s).length}</span></button>`).join('');
    list.innerHTML = sections.map((s, i) => `<section class="const-section" data-section="${i}">
        <h3>${esc(titleCase(s.title))}</h3>
        ${s.description ? `<p class="hint">${esc(s.description)}</p>` : ''}
        <div class="const-rows">${entriesOf(s).map(row).join('')}</div>
      </section>`).join('');
    applyFilter();
    updateStatus();
  }

  // One constant: its name (and the note above it in the file), its editor, its comment.
  function row(entry) {
    const value = S.catalog.values[entry.name];
    const kind = kindOf(value);
    const original = valueText(value);
    const text = S.edits.has(entry.name) ? S.edits.get(entry.name) : original;
    const edited = S.edits.has(entry.name);
    const formula = entry.derived
      ? `<div class="const-formula" title="Worked out from other constants. Typing a value here replaces the formula with that number in Constants.m.">= ${esc(entry.expression)}</div>`
      : '';
    const editor = kind === 'bool'
      ? `<label class="toggle"><input type="checkbox" data-const="${entry.name}" ${text === 'true' ? 'checked' : ''}><span></span>${text === 'true' ? 'True' : 'False'}</label>`
      : `<input class="input mono${validText(kind, text) ? '' : ' invalid'}" data-const="${entry.name}" data-kind="${kind}" value="${esc(text)}" spellcheck="false" aria-label="${entry.name}">`;
    const search = [entry.name, text, entry.comment, entry.note, entry.expression]
      .join(' ').toLowerCase();
    return `<div class="const-row${edited ? ' edited' : ''}" data-name="${entry.name}" data-search="${esc(search)}">
        <div class="const-name"><span class="mono">${entry.name}</span>${entry.note ? `<span class="hint">${esc(entry.note)}</span>` : ''}</div>
        <div class="const-value">${editor}${formula}</div>
        <div class="const-comment">${esc(entry.comment || '')}${edited ? `<span class="hint">Was ${esc(original)}</span>` : ''}</div>
      </div>`;
  }

  // Search and Edited Only: rows that don't match are hidden, and sections left empty.
  function applyFilter() {
    const words = search.value.trim().toLowerCase().split(/\s+/).filter(Boolean);
    for (const sec of list.querySelectorAll('.const-section')) {
      let shown = 0;
      for (const r of sec.querySelectorAll('.const-row')) {
        const ok = words.every(w => r.dataset.search.includes(w)) &&
          (!editedOnly.checked || S.edits.has(r.dataset.name));
        r.hidden = !ok;
        shown += ok ? 1 : 0;
      }
      sec.hidden = shown === 0;
      const btn = nav.querySelector(`[data-constsec="${sec.dataset.section}"]`);
      if (btn) {
        btn.hidden = shown === 0;
      }
    }
  }

  function updateStatus() {
    const n = S.edits.size;
    const bad = [...list.querySelectorAll('input.invalid')].length;
    status.textContent = bad ? `${bad} value${bad > 1 ? 's' : ''} to fix before saving`
      : n ? `${n} edited, not saved yet` : 'Every value as MATLAB has it now';
    saveBtn.disabled = !n || bad > 0 || S.saving;
    revertBtn.disabled = !n || S.saving;
    saveBtn.textContent = n ? `Save ${n} Change${n > 1 ? 's' : ''}` : 'Save Changes';
  }

  // Editing

  function edit(name, text) {
    const original = valueText(S.catalog.values[name]);
    if (text === original) {
      S.edits.delete(name);
    } else {
      S.edits.set(name, text);
    }
    const r = list.querySelector(`.const-row[data-name="${name}"]`);
    if (r) {
      r.classList.toggle('edited', S.edits.has(name));
      const comment = r.querySelector('.const-comment');
      const was = comment.querySelector('.hint');
      if (S.edits.has(name) && !was) {
        comment.insertAdjacentHTML('beforeend', `<span class="hint">Was ${esc(original)}</span>`);
      } else if (!S.edits.has(name) && was) {
        was.remove();
      }
    }
    updateStatus();
  }

  list.addEventListener('input', ev => {
    const el = ev.target.closest('input[data-const]');
    if (!el || el.type === 'checkbox') {
      return;
    }
    el.classList.toggle('invalid', !validText(el.dataset.kind, el.value));
    edit(el.dataset.const, el.value);
  });
  list.addEventListener('change', ev => {
    const el = ev.target.closest('input[type=checkbox][data-const]');
    if (el) {
      el.nextElementSibling.nextSibling.textContent = el.checked ? 'True' : 'False';
      edit(el.dataset.const, String(el.checked));
    }
  });

  // Saving: MATLAB reads each value as its constant's type, writes the changed lines and applies
  // the settings; the server then sends the new constants to every page.
  saveBtn.onclick = () => act(async () => {
    S.saving = true;
    updateStatus();
    try {
      const edits = [...S.edits].map(([name, text]) => ({ name, text }));
      const r = await matlab('constants.setText', { edits });
      S.edits.clear();
      onSaved(r);
      const n = [].concat(r.changed || []).length;
      toast(n ? `Wrote ${n} constant${n > 1 ? 's' : ''} to Constants.m` +
        `${r.error ? `; the settings don't apply: ${r.error}` : ''}`
        : 'Nothing changed: the values were the same.', r.error ? 'error' : 'success', 6000);
      await load();
    } finally {
      S.saving = false;
      updateStatus();
    }
  });
  revertBtn.onclick = () => {
    S.edits.clear();
    render();
  };

  // The tools: search, Edited Only, a section from the list, MATLAB's editor, closing.
  search.oninput = applyFilter;
  editedOnly.onchange = applyFilter;
  nav.addEventListener('click', ev => {
    const b = ev.target.closest('[data-constsec]');
    if (b) {
      list.querySelector(`[data-section="${b.dataset.constsec}"]`)
        ?.scrollIntoView({ behavior: 'smooth', block: 'start' });
    }
  });
  document.getElementById('constEditor').onclick = () => act(async () => {
    const r = await matlab('constants.edit');
    toast(`${r.file} is open in MATLAB's editor. Save it there: this window and the page follow.`,
      'info', 6000);
  });
  const close = () => {
    if (S.edits.size && !window.confirm(`Close without saving ${S.edits.size} edited value` +
        `${S.edits.size > 1 ? 's' : ''}?`)) {
      return;
    }
    S.edits.clear();
    dialog.close();
  };
  document.getElementById('constClose').onclick = close;
  dialog.addEventListener('cancel', ev => {
    ev.preventDefault();
    close();
  });

  return {
    open() {
      search.value = '';
      editedOnly.checked = false;
      if (!dialog.open) {
        dialog.showModal();
      }
      list.innerHTML = '<p class="hint const-wait">Reading Constants.m in MATLAB…</p>';
      act(load);
    },
    // The constants changed elsewhere (a hand edit, another page, Update from Lab): read them
    // again, keeping what was typed here and not saved yet.
    refresh() {
      if (dialog.open && !S.saving) {
        act(load);
      }
    },
  };
}
