// Documentation tab (both consoles): guides to SPOT and the lab, and a page for every function in
// the project. The server reads the code (spot_console/docs.py, docparse.py) and sends an index:
// every file, every function with its documentation (help text, docstring, JSDoc) and its code
// split into the blocks its comments describe, what it calls and what calls it, the console's
// routes, the guides (webapp/docs) and the notes on functions (webapp/docs/notes).
//
// The page:
//   - contents on the left (Guides, or the Reference: language, package or folder, file,
//     function), the page in the middle, "On this page" on the right;
//   - search across functions, guides and the code itself, with suggestions as you type;
//   - Find (Ctrl+F) highlights words on the page;
//   - an editor (CodeMirror) for any file the reference shows and the documentation's own
//     pages, checked as you type: ESLint in the page for JavaScript, the server for the rest
//     (spot_console/doclint.py: Python, MATLAB with its Code Analyzer, shell, C/C++, links).
//     Only the local console saves (SPOT in MATLAB); the lab console offers Download.
// Addresses: #docs, #docs/g/<guide>, #docs/r/<function>, #docs/f/<file>, #docs/a/<area>,
// #docs/s/<search>, #docs/e/<file being edited>, #docs/x/routes, #docs/x/az.

const CM_URL = '/static/vendor/codemirror/codemirror.js';
const ESLINT_URL = '/static/vendor/eslint/eslint.js';
const MD_URL = '/static/vendor/markdown/markdown.js';
const KATEX_CSS = '/static/vendor/katex/katex.min.css';

// Names shown for languages and kinds of entry.
const LANG_NAMES = {
  matlab: 'MATLAB', python: 'Python', javascript: 'JavaScript', c: 'C', cpp: 'C++',
  arduino: 'Arduino', shell: 'Shell', markdown: 'Markdown', json: 'JSON', xml: 'XML',
  css: 'CSS', html: 'HTML', ini: 'Service', makefile: 'Makefile', text: 'Text',
  diagram: 'Diagram',
};
const KIND_NAMES = {
  function: 'function', local: 'local function', nested: 'nested function', method: 'method',
  class: 'class', module: 'file', file: 'function file', script: 'script', block: 'block',
  handler: 'handler',
};

// The highlighter's language for each file language, its indentation and line width.
const HIGHLIGHT = { makefile: 'shell', arduino: 'cpp', c: 'cpp', bash: 'shell', sh: 'shell',
  js: 'javascript', py: 'python', m: 'matlab', octave: 'matlab', text: '', ini: 'ini' };
const INDENT = { javascript: 2, css: 2, html: 2, json: 2, markdown: 2 };
const WIDTH = { matlab: 100, python: 99, javascript: 100, markdown: 0 };

const SUGGEST_ENTRIES = 7;
const SUGGEST_GUIDES = 3;
const SUGGEST_LINES = 4;
const RESULT_LIMIT = 60;          // functions and guides on the results page
const CODE_LIMIT = 300;           // lines of code on the results page
const LINT_DELAY = 600;           // [ms] after the last keystroke
const PREVIEW_DELAY = 250;        // [ms] Markdown preview
const RECENT = 8;                 // pages remembered on the home page

// Find matches text within one of these (a line of code, a paragraph, a cell...).
const FIND_BLOCKS = '.l, p, li, td, th, h1, h2, h3, h4, pre, .pre, .sm, .nm, a, div';

// Text made safe to put in HTML; the first element, and every element, a selector matches.
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const $ = (sel, el = document) => el.querySelector(sel);
const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];

/**
 * Turns heading text into an anchor name ("The Arm's Controllers" -> "the-arms-controllers").
 * @param {string} text The heading.
 * @return {string} The anchor.
 */
function slug(text) {
  return String(text).toLowerCase().replace(/<[^>]+>/g, '').replace(/&\w+;/g, '')
    .replace(/[^\w\s-]/g, '').trim().replace(/\s+/g, '-').slice(0, 60) || 'section';
}

/**
 * Returns a function that runs fn once calls have stopped for ms milliseconds.
 * @param {function(...*)} fn The function.
 * @param {number} ms The quiet time [ms].
 * @return {function(...*)} The debounced function.
 */
function debounce(fn, ms) {
  let timer = null;
  return (...args) => {
    clearTimeout(timer);
    timer = setTimeout(() => fn(...args), ms);
  };
}

/**
 * Builds a Documentation address.
 * @param {string} kind g, r, f, a, s, e or x (see the file's comment), or '' for the home page.
 * @param {string=} arg Its argument (a page, an entry id, a path).
 * @param {(string|number)=} extra A heading or a line.
 * @return {string} The address, #docs/...
 */
export function docsHref(kind, arg, extra) {
  if (!kind) {
    return '#docs';
  }
  let h = `#docs/${kind}/${encodeURIComponent(arg)}`;
  if (extra !== undefined && extra !== null && extra !== '') {
    h += `/${encodeURIComponent(extra)}`;
  }
  return h;
}

/**
 * Reads a Documentation address.
 * @param {string} hash location.hash.
 * @return {{kind: string, arg: string, extra: string}} What to show.
 */
function parseRoute(hash) {
  const parts = hash.replace(/^#/, '').split('/');
  if (parts[0] !== 'docs' || parts.length < 2 || !parts[1]) {
    return { kind: '', arg: '', extra: '' };
  }
  // A part of the address, decoded (as it is if it isn't valid).
  const dec = s => {
    try {
      return decodeURIComponent(s || '');
    } catch {
      return s || '';
    }
  };
  return { kind: parts[1], arg: dec(parts[2]), extra: dec(parts.slice(3).join('/')) };
}

/**
 * Sets up the Documentation tab.
 * @param {{api: function(string, !Object=): !Promise<!Object>, act: function(function()),
 *     toast: function(string, string=, number=),
 *     storage: {get: function(string, *): *, set: function(string, *)}}} deps The page's API
 *     call, its action wrapper (reports errors), its toast and its settings storage.
 * @return {!Object} show, hide and onSaved (the server's "docs" event).
 */
export function initDocs({ api, act, toast, storage }) {
  const S = {
    shown: false, ix: null, loading: null, version: null, routeKey: null,
    lib: { cm: null, md: null, eslint: null }, marked: null,
    nav: storage.get('spot.docs.nav', 'guides'), open: new Set(storage.get('spot.docs.open', [])),
    current: null, find: { hits: [], i: -1 }, pendingFind: null,
    recent: storage.get('spot.docs.recent', []),
    hl: new Map(), ed: null, suggest: { items: [], sel: -1 }, staleIndex: false,
  };
  const page = $('#pageDocs');
  const content = $('#docsContent');
  const scroller = $('#docsScroll');

  // Libraries

  // CodeMirror (the editor and the highlighter), loaded the first time the tab opens.
  async function loadCm() {
    if (!S.lib.cm) {
      S.lib.cm = await import(CM_URL);
    }
    return S.lib.cm;
  }

  // marked and KaTeX, with KaTeX's stylesheet.
  async function loadMd() {
    if (!S.lib.md) {
      S.lib.md = await import(MD_URL);
      const link = document.createElement('link');
      link.rel = 'stylesheet';
      link.href = KATEX_CSS;
      document.head.append(link);
      S.marked = makeMarked(S.lib.md);
    }
    return S.lib.md;
  }

  // ESLint (1.4 MB), only when a JavaScript file is edited.
  async function loadEslint() {
    if (!S.lib.eslint) {
      S.lib.eslint = await import(ESLINT_URL);
    }
    return S.lib.eslint;
  }

  // The index

  // Gets the index from the server (the browser revalidates it with its ETag) and prepares it:
  // lookups by name, and lower-case text for searching.
  async function loadIndex() {
    const res = await fetch('/api/docs/index', { cache: 'no-cache' });
    const data = await res.json();
    if (!res.ok) {
      throw new Error(data.error || `Could not read the documentation (${res.status})`);
    }
    if (S.ix && data.version === S.version) {
      return S.ix;
    }
    S.version = data.version;
    S.hl.clear();
    S.ix = prepare(data);
    renderStatus();
    return S.ix;
  }

  // The index once, shared by everyone who asks while it loads.
  function index() {
    if (!S.loading) {
      S.loading = loadIndex().finally(() => {
        S.loading = null;
      });
    }
    return S.loading;
  }

  /**
   * Adds lookups to the server's index.
   * @param {!Object} data The index.
   * @return {!Object} The same object, with byQual, byName, areas, lines() and lower().
   */
  function prepare(data) {
    const ix = data;
    ix.byQual = new Map();
    ix.byName = new Map();
    ix.areas = new Map();
    for (const g of ix.groups) {
      for (const a of g.areas) {
        a.groupTitle = g.title;
        ix.areas.set(a.key, a);
      }
    }
    // Functions before the files that hold them (a MATLAB file and its main function share a
    // name).
    const ranked = Object.entries(ix.entries).sort((a, b) =>
      Number(isModule(a[1])) - Number(isModule(b[1])));
    for (const [id, e] of ranked) {
      e.id = id;
      if (!ix.byQual.has(e.qual)) {
        ix.byQual.set(e.qual, e);
      }
      const list = ix.byName.get(e.name) || [];
      list.push(e);
      ix.byName.set(e.name, list);
    }
    // Each file's lines, and in lower case for searching: split the first time they are asked for.
    const lineCache = new Map();
    const lowerCache = new Map();
    ix.lines = path => {
      if (!lineCache.has(path)) {
        const f = ix.files[path];
        lineCache.set(path, f && f.text != null ? f.text.split('\n') : []);
      }
      return lineCache.get(path);
    };
    ix.lower = path => {
      if (!lowerCache.has(path)) {
        lowerCache.set(path, ix.lines(path).map(l => l.toLowerCase()));
      }
      return lowerCache.get(path);
    };
    ix.search = null;
    return ix;
  }

  // Whether an entry is a whole file (or diagram block) rather than a function in it.
  const isModule = e => ['module', 'file', 'script', 'block'].includes(e.kind) && !e.local;

  // The language shown for an entry or a file: diagram blocks are MATLAB in a diagram.
  const langOf = (e) => {
    const f = S.ix.files[e.file || e.path];
    return f && f.group === 'diagram' ? 'diagram' : (e.lang || (f && f.lang) || 'text');
  };

  // Where an entry is, in a few words: its file and line, or its block in the diagram.
  function whereText(e) {
    const f = S.ix.files[e.file];
    if (f && f.virtual) {
      return `${f.blocks[0].split('/').slice(-3, -1).join('/')}:${e.line}`;
    }
    return `${e.file.split('/').pop()}:${e.line}`;
  }

  // The name to show: MATLAB and Python by their full names, the rest by their name in the file.
  function displayName(e) {
    if (isModule(e)) {
      const f = S.ix.files[e.file];
      return f && f.virtual ? e.name : e.file.split('/').pop();
    }
    if (e.lang === 'matlab' && !(S.ix.files[e.file] || {}).virtual) {
      return e.qual;
    }
    if (e.lang === 'python') {
      return e.qual;
    }
    return e.local || e.name;
  }

  // An entry by id, full name or plain name (for links written by hand).
  function findEntry(key) {
    const ix = S.ix;
    if (!key) {
      return null;
    }
    if (ix.entries[key]) {
      return ix.entries[key];
    }
    if (ix.byQual.has(key)) {
      return ix.byQual.get(key);
    }
    const named = ix.byName.get(key);
    return named && named.length === 1 ? named[0] : null;
  }

  // The file a path names: exactly, or by its name if only one file has it.
  function findFile(key) {
    const files = S.ix.files;
    if (files[key]) {
      return files[key];
    }
    const hits = Object.values(files).filter(f => f.path.split('/').pop() === key);
    return hits.length === 1 ? hits[0] : null;
  }

  // Code, highlighted

  /**
   * The highlighted lines of a file (cached).
   * @param {string} path The file.
   * @return {!Array<string>} One HTML string per line.
   */
  function highlighted(path) {
    if (!S.hl.has(path)) {
      const f = S.ix.files[path];
      const lang = f ? (HIGHLIGHT[f.lang] ?? f.lang) : '';
      S.hl.set(path, S.lib.cm.highlightLines(f ? f.text || '' : '', lang));
    }
    return S.hl.get(path);
  }

  /**
   * Some lines of a file, numbered and highlighted.
   * @param {string} path The file.
   * @param {number} first The first line.
   * @param {number} last The last line.
   * @param {{ids: (boolean|undefined), marks: (!Object|undefined)}=} opts ids: give each line
   *     an anchor (L-12); marks: {line: html} added after a line.
   * @return {string} The HTML.
   */
  function codeBlock(path, first, last, opts = {}) {
    const lines = highlighted(path);
    const out = [];
    for (let n = Math.max(1, first); n <= Math.min(last, lines.length); n++) {
      const id = opts.ids ? ` id="L-${n}"` : '';
      const mark = opts.marks && opts.marks[n] ? opts.marks[n] : '';
      out.push(`<div class="l"${id}><span class="n" data-line="${n}">${n}</span>` +
        `<span class="t">${lines[n - 1] || ' '}${mark}</span></div>`);
    }
    return `<div class="docs-code" data-path="${esc(path)}">${out.join('')}</div>`;
  }

  // A snippet of code (not from a file), highlighted.
  function highlightSnippet(text, lang) {
    if (!S.lib.cm) {
      return esc(text);
    }
    return S.lib.cm.highlightLines(text, HIGHLIGHT[lang] ?? lang ?? '').join('\n');
  }

  // Markdown

  /**
   * A Markdown renderer with the documentation's additions: $maths$ and $$maths$$ (KaTeX),
   * > [!NOTE] callouts, page:, ref: and file: links, `names` linked to their pages, highlighted
   * code, and anchors on headings.
   * @param {!Object} lib The markdown bundle (Marked, katex).
   * @return {!Object} The marked instance.
   */
  function makeMarked(lib) {
    // Maths as HTML (an error shows the TeX as code).
    const math = (tex, display) => {
      try {
        return lib.katex.renderToString(tex, { displayMode: display, throwOnError: false });
      } catch (e) {
        return `<code>${esc(tex)}</code>`;
      }
    };
    const marked = new lib.Marked({ gfm: true });
    marked.use({
      // $$...$$ on lines of their own, and $...$ (or $$...$$) inside a paragraph. marked calls
      // start() to find where one may begin and tokenizer() to read it.
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
            return { type: 'mathInline', raw: block[0], text: block[1], display: true };
          }
          const m = /^\$(?![\s$])((?:\\\$|[^$\n])+?)(?<![\s\\])\$(?!\d)/.exec(src);
          return m ? { type: 'mathInline', raw: m[0], text: m[1] } : undefined;
        },
        renderer: tok => math(tok.text, Boolean(tok.display)),
      }],
      // How code, headings, links, pictures, quotes (callouts) and inline code are drawn.
      renderer: {
        code(code, info) {
          const lang = (info || '').split(/\s/)[0].toLowerCase();
          return `<pre class="hl" data-lang="${esc(lang)}"><code>${highlightSnippet(code, lang)}` +
            '</code><button type="button" class="docs-copy">Copy</button></pre>';
        },
        heading(text, level) {
          const id = uniqueSlug(text);
          return `<h${level} id="${id}">${text}</h${level}>`;
        },
        link(href, title, text) {
          const target = linkTarget(href || '');
          const t = title ? ` title="${esc(title)}"` : '';
          if (target === null) {
            return `<span class="docs-broken" title="No such page: ${esc(href)}">${text}</span>`;
          }
          const ext = /^https?:/.test(target) ? ' target="_blank" rel="noopener"' : '';
          return `<a href="${esc(target)}"${t}${ext}>${text}</a>`;
        },
        image(href, title, text) {
          const src = /^(https?:|data:|\/)/.test(href || '') ? href : `/docs/${String(href).replace(/^\.?\//, '')}`;
          return `<img src="${esc(src)}" alt="${esc(text)}"${title ? ` title="${esc(title)}"` : ''} loading="lazy">`;
        },
        blockquote(quote) {
          const m = /^\s*<p>\[!(NOTE|TIP|IMPORTANT|WARNING|CAUTION)\]\s*/i.exec(quote);
          if (!m) {
            return `<blockquote>${quote}</blockquote>`;
          }
          const kind = m[1].toLowerCase();
          const body = '<p>' + quote.slice(m[0].length);
          return `<div class="docs-callout ${kind}"><div class="docs-callout-title">${kind}</div>` +
            `${body.replace(/^<p>\s*<\/p>/, '')}</div>`;
        },
        codespan(text) {
          return linkCode(text);
        },
      },
    });
    return marked;
  }

  // Each heading's anchor, made unique within a page (md() starts a new set).
  let slugs = new Map();
  const uniqueSlug = text => {
    const base = slug(text);
    const n = slugs.get(base) || 0;
    slugs.set(base, n + 1);
    return n ? `${base}-${n}` : base;
  };

  // Where a link written in a page goes: page:<guide>[#heading], ref:<function>,
  // file:<path>[:line], or any URL.
  function linkTarget(href) {
    let m = /^page:([^#]+)(?:#(.*))?$/.exec(href);
    if (m && ['routes', 'az'].includes(m[1])) {
      return docsHref('x', m[1], m[2]);
    }
    if (m) {
      return S.ix && !S.ix.guides.pages[m[1]] ? null : docsHref('g', m[1], m[2]);
    }
    m = /^ref:(.+)$/.exec(href);
    if (m) {
      const e = S.ix ? findEntry(decodeURIComponent(m[1])) : null;
      return e ? docsHref('r', e.id) : null;
    }
    m = /^file:([^:]+)(?::(\d+))?$/.exec(href);
    if (m) {
      const f = S.ix ? findFile(decodeURIComponent(m[1])) : null;
      return f ? docsHref('f', f.path, m[2]) : null;
    }
    if (href.startsWith('#') && !href.startsWith('#docs')) {
      return `${location.hash.split('/').slice(0, 3).join('/')}/${href.slice(1)}`;
    }
    return href;
  }

  // Inline code: linked when it names a function or a file (`spot.sim.run`, `server.py`).
  function linkCode(escaped) {
    const raw = escaped.replace(/&amp;/g, '&').replace(/&lt;/g, '<').replace(/&gt;/g, '>')
      .replace(/&quot;/g, '"').replace(/&#39;/g, '\'');
    if (S.ix) {
      const name = raw.replace(/\(.*\)$/, '');
      const e = /^[\w.$>/]+$/.test(name) ? S.ix.byQual.get(name) : null;
      if (e) {
        return `<a href="${docsHref('r', e.id)}"><code>${escaped}</code></a>`;
      }
      const f = /^[+\w./@ -]+\.\w+$/.test(raw) ? findFile(raw) : null;
      if (f) {
        return `<a href="${docsHref('f', f.path)}"><code>${escaped}</code></a>`;
      }
    }
    return `<code>${escaped}</code>`;
  }

  /**
   * Renders Markdown as HTML.
   * @param {string} text The Markdown.
   * @return {string} The HTML.
   */
  function md(text) {
    slugs = new Map();
    try {
      return S.marked.parse(text || '');
    } catch (e) {
      return `<pre>${esc(text)}</pre>`;
    }
  }

  // Comment text (a block's explanation): paragraphs, keeping lists and aligned lines as they
  // are, with `code` and spot.* names linked.
  function commentHtml(text) {
    const parts = [];
    for (const para of String(text || '').split(/\n\s*\n/)) {
      const lines = para.split('\n');
      const aligned = lines.some(l => /^\s{2,}\S/.test(l) || /\S {3,}\S/.test(l) ||
        /^\s*([-*•]|\d+[.)])\s/.test(l));
      if (aligned && lines.length > 1) {
        parts.push(`<div class="pre">${inline(lines.join('\n'))}</div>`);
      } else {
        parts.push(`<p>${inline(lines.map(l => l.trim()).join(' '))}</p>`);
      }
    }
    return parts.join('');
  }

  // Inline text: `code` and names of functions linked, URLs clickable.
  function inline(text) {
    let html = esc(text);
    html = html.replace(/`([^`]+)`/g, (m, code) => linkCode(code));
    html = html.replace(/(^|[\s(])(spot(?:\.\w+)+)(?![\w(])/g, (m, pre, name) => {
      const e = S.ix && S.ix.byQual.get(name);
      return e ? `${pre}<a href="${docsHref('r', e.id)}">${name}</a>` : m;
    });
    html = html.replace(/(https?:\/\/[^\s<)]+)/g, '<a href="$1" target="_blank" rel="noopener">$1</a>');
    return html;
  }

  // Routing and rendering

  // Shows what the address asks for, unless it is already shown.
  async function route(force = false) {
    const r = parseRoute(location.hash);
    const key = `${r.kind}/${r.arg}/${r.extra}`;
    if (!force && key === S.routeKey) {
      return;
    }
    S.routeKey = key;
    storage.set('spot.docs.route', location.hash.startsWith('#docs/') ? location.hash : '#docs');

    // The editor has its own view; the rest share the page.
    if (r.kind === 'e') {
      const opts = S.pendingOpen || {};
      S.pendingOpen = null;
      await openEditor(r.arg, Number(r.extra) || 0, opts);
      return;
    }
    showEditor(false);
    try {
      await Promise.all([loadCm(), loadMd()]);
      if (!S.ix || S.staleIndex) {
        S.staleIndex = false;
        await index();
      }
    } catch (e) {
      content.innerHTML = `<h1>Documentation unavailable</h1><p class="docs-missing">${esc(e.message)}</p>`;
      return;
    }
    closeFind(false);
    content.classList.remove('wide');
    let title = 'Documentation';
    if (r.kind === 'g') {
      title = renderGuide(r.arg, r.extra);
    } else if (r.kind === 'r') {
      title = renderEntry(r.arg);
    } else if (r.kind === 'f') {
      title = renderFile(r.arg, Number(r.extra) || 0);
    } else if (r.kind === 'a') {
      title = renderArea(r.arg);
    } else if (r.kind === 's') {
      title = renderSearchPage(r.arg);
    } else if (r.kind === 'x' && r.arg === 'routes') {
      title = renderRoutes();
    } else if (r.kind === 'x' && r.arg === 'az') {
      title = renderAZ();
    } else {
      title = renderHome();
    }
    remember(title);
    renderTree();
    renderToc();
    if (S.pendingFind) {
      openFind(S.pendingFind);
      S.pendingFind = null;
    }
  }

  // Scrolls the page to an anchor, a line (L-12), or the top.
  function scrollToAnchor(anchor) {
    scroller.classList.add('instant');
    const el = anchor ? document.getElementById(anchor) : null;
    if (el) {
      el.scrollIntoView({ block: anchor.startsWith('L-') ? 'center' : 'start' });
      if (anchor.startsWith('L-')) {
        el.classList.add('target');
      }
    } else {
      scroller.scrollTop = 0;
    }
    requestAnimationFrame(() => scroller.classList.remove('instant'));
  }

  // The page's address bar: where it is in the documentation, and its buttons.
  function setCrumbs(parts, actions = '') {
    const html = [`<a href="#docs">Documentation</a>`];
    parts.forEach((p, i) => {
      html.push('<span class="sep">›</span>');
      html.push(i === parts.length - 1 || !p.href
        ? `<span class="here">${esc(p.label)}</span>`
        : `<a href="${p.href}">${esc(p.label)}</a>`);
    });
    $('#docsCrumbs').innerHTML = html.join('');
    $('#docsActions').innerHTML = actions;
  }

  // Remembers the pages visited, for the home page.
  function remember(title) {
    if (!location.hash.startsWith('#docs/') || location.hash.startsWith('#docs/s/')) {
      return;
    }
    const item = { href: location.hash, title };
    S.recent = [item, ...S.recent.filter(r => r.href !== item.href)].slice(0, RECENT);
    storage.set('spot.docs.recent', S.recent);
  }

  // The home page: what is here, where to start.
  function renderHome() {
    const ix = S.ix;
    const guides = ix.guides;
    const counts = {};
    for (const e of Object.values(ix.entries)) {
      if (!isModule(e)) {
        const lang = langOf(e);
        counts[lang] = (counts[lang] || 0) + 1;
      }
    }
    const total = Object.values(counts).reduce((a, b) => a + b, 0);
    const start = guides.sections.slice(0, 3).flatMap(s => s.pages.slice(0, 2)).slice(0, 6);
    // A card linking to a guide or a part of the reference.
    const card = (href, title, text, meta = '') => `<a class="docs-card" href="${href}"><h3>${esc(title)}</h3>` +
      `<p>${esc(text)}</p>${meta ? `<div class="meta">${meta}</div>` : ''}</a>`;
    const guideCards = start.map(id => {
      const p = guides.pages[id];
      return card(docsHref('g', id), p.title, p.summary || firstLine(p.text), esc(p.section));
    }).join('');
    const refCards = ix.groups.map(g => {
      const files = g.areas.reduce((n, a) => n + a.files.length, 0);
      return card(docsHref('a', g.areas[0].key), g.title, g.areas.map(a => a.title).join(', '),
        `${g.areas.length} ${g.areas.length === 1 ? 'part' : 'parts'} · ${files} files`);
    }).join('');
    const recent = S.recent.length
      ? `<h2>Recently Viewed</h2><ul class="docs-list">${S.recent.map(r =>
        `<li><a class="nm" href="${esc(r.href)}">${esc(r.title)}</a></li>`).join('')}</ul>` : '';
    const langs = Object.entries(counts).sort((a, b) => b[1] - a[1])
      .map(([l, n]) => `<span class="docs-chip"><span class="docs-badge" data-lang="${l}">${esc(LANG_NAMES[l] || l)}</span>${n}</span>`)
      .join(' ');
    content.innerHTML = `
      <div class="docs-title"><h1 class="prose">SPOT Documentation</h1></div>
      <p class="lead">Guides to SPOT and the lab, and a page for each of the ${total.toLocaleString()} functions
        in this project, read from the code itself, so they always match it.</p>
      <p>Search with the box at the top left (press <code>/</code>): it finds functions by name, by what their
        documentation says, and by any word in their code. <b>Find</b> (Ctrl+F) highlights words on the page you are
        reading. Every function's page explains what it does, then walks through its code block by block.
        ${ix.editable
    ? 'You can edit any file from its page (<b>Edit</b>); the editor checks the code as you ' +
      'type, and saving writes it to this project.'
    : 'You can open any file in the editor and check your changes; saving is done from the ' +
      'local console (SPOT in MATLAB).'}</p>
      ${guides.errors.length ? `<div class="docs-missing">${guides.errors.map(esc).join('<br>')}</div>` : ''}
      <h2>Start Here</h2><div class="docs-cards">${guideCards}</div>
      <h2>Reference</h2><div class="docs-cards">${refCards}</div>
      <p class="docs-links">${langs}</p>
      <p><a href="${docsHref('x', 'routes')}">The console's API routes and MATLAB commands</a> ·
        <a href="${docsHref('x', 'az')}">Every function, A to Z</a></p>
      ${recent}`;
    setCrumbs([]);
    scrollToAnchor(null);
    return 'Documentation';
  }

  // A guide's first line of text (for its card when it has no summary).
  const firstLine = text => (String(text || '').split('\n')
    .find(l => l.trim() && !l.startsWith('#')) || '')
    .replace(/[*_`[\]]/g, '').slice(0, 160);

  // A guide.
  function renderGuide(id, anchor) {
    const p = S.ix.guides.pages[id];
    if (!p) {
      content.innerHTML = `<h1>No such page</h1><p>There is no guide called <code>${esc(id)}</code>.</p>`;
      setCrumbs([{ label: id }]);
      return id;
    }
    content.innerHTML = md(p.text);
    const edit = `<button type="button" class="btn btn-sm" data-act="edit" data-path="${esc(p.path)}"
      title="${S.ix.editable ? 'Edit this page' : 'Open this page in the editor'}">Edit Page</button>`;
    setCrumbs([{ label: p.section, href: docsHref('g', id) }, { label: p.title }], edit);
    scrollToAnchor(anchor || null);
    return p.title;
  }

  // An entry: a function, class, method, file or diagram block.
  function renderEntry(id) {
    const ix = S.ix;
    const e = ix.entries[id] || findEntry(id);
    if (!e) {
      content.innerHTML = `<h1>Not found</h1><p>No function <code>${esc(id)}</code> in this project. ` +
        'It may have been renamed or removed: try searching for it.</p>';
      setCrumbs([{ label: id }]);
      return id;
    }
    S.current = e;
    const f = ix.files[e.file];
    const lang = langOf(e);
    const area = ix.areas.get(f.area);
    const parents = [];
    for (let p = ix.entries[e.parent]; p && !isModule(p); p = ix.entries[p.parent]) {
      parents.unshift(p);
    }
    const doc = e.doc;
    const note = noteFor(e);

    // Title, where it is, its signature and buttons.
    const kind = e.kind === 'block' ? 'MATLAB Function block' : (KIND_NAMES[e.kind] || e.kind);
    const attrs = e.attrs ? `<span class="docs-kind">${esc(e.attrs)}</span>` : '';
    const where = [
      `<a href="${docsHref('f', e.file, e.line)}" class="mono">${esc(f.virtual ? f.slx : e.file)}` +
      `${isModule(e) ? '' : `:${e.line}`}</a>`,
      isModule(e) ? `${f.lines} lines` : `lines ${e.start}–${e.end}`,
    ];
    if (parents.length) {
      const p = parents[parents.length - 1];
      where.push(`inside <a href="${docsHref('r', p.id)}" class="mono">${esc(displayName(p))}</a>`);
    }
    if (f.virtual) {
      where.push(`in ${f.blocks.length} ${f.blocks.length === 1 ? 'block' : 'blocks'} of ${esc(f.diagram)}`);
    }
    const sig = e.sig && !isModule(e)
      ? `<pre class="docs-sig hl">${highlightSnippet(e.sig, HIGHLIGHT[e.lang] ?? e.lang)}</pre>` : '';
    const buttons = entryButtons(e, f, note);

    // Its documentation: from the code, a standard method's, a group's, or none yet.
    let docHtml = '';
    if (doc && doc.standard) {
      docHtml += `<div class="docs-std">A standard method: ${esc((doc.blocks[0] || {}).text || '')}</div>`;
    } else if (doc && doc.helper) {
      docHtml += '<div class="docs-std">A small helper that the page\'s modules each define ' +
        'the same way.</div>';
    } else if (doc && doc.header) {
      docHtml += `<div class="docs-std">Described where it is declared, in
        <a href="${docsHref('f', doc.header)}" class="mono">${esc(doc.header.split('/').pop())}</a>.</div>`;
    } else if (doc && doc.block) {
      docHtml += `<div class="docs-std">Described by the comment over the block of code it is
        defined in (<a href="${docsHref('f', e.file, doc.block)}">line ${doc.block}</a>).</div>`;
    } else if (doc && doc.group) {
      const g = ix.entries[doc.group];
      docHtml += `<div class="docs-std">Described by the comment above ${g
        ? `<a href="${docsHref('r', g.id)}" class="mono">${esc(displayName(g))}</a>` : 'the function before it'},
        which covers the functions defined with it.</div>`;
    }
    if (doc) {
      docHtml += docBlocks(doc, doc.standard ? 1 : 0);
    } else if (!note && !isModule(e)) {
      docHtml += `<div class="docs-missing">The code doesn't describe this ${esc(kind)} yet. The walkthrough below
        shows what it does; ${ix.editable ? '<b>Add Note</b> to' : 'a note in <code>docs/notes</code> can'} explain it in a sentence or two.</div>`;
    }

    // Diagram blocks: where the block is.
    let blocks = '';
    if (f.virtual) {
      blocks = `<div class="docs-section"><h2 id="blocks">In the Diagram</h2><ul class="docs-list">${f.blocks.map(b =>
        `<li><span class="nm">${esc(f.diagram)}/${esc(b)}</span></li>`).join('')}</ul></div>`;
    }

    content.innerHTML = `
      <div class="docs-title"><h1>${esc(displayName(e))}</h1>
        <span class="docs-badge" data-lang="${lang}">${esc(LANG_NAMES[lang] || lang)}</span>
        <span class="docs-kind">${esc(kind)}</span>${attrs}</div>
      <div class="docs-where">${where.join('<span class="sep">·</span>')}</div>
      ${doc && doc.summary ? `<p class="lead">${inline(doc.summary)}</p>` : ''}
      ${sig}
      <div class="docs-btnrow">${buttons}</div>
      <div class="docs-doc">${docHtml}</div>
      ${note ? noteHtml(note, e) : ''}
      ${blocks}
      ${walkthrough(e)}
      ${insideList(e)}
      ${xrefHtml(e)}
      ${routesHtml(e)}`;
    const areaHref = area ? docsHref('a', area.key) : '';
    const crumbs = [{ label: area ? area.groupTitle : '', href: areaHref },
      { label: area ? area.title : '', href: areaHref }];
    if (!isModule(e)) {
      const label = f.virtual ? f.blocks[0].split('/')[0] : e.file.split('/').pop();
      crumbs.push({ label, href: docsHref('f', e.file) });
    }
    for (const p of parents) {
      crumbs.push({ label: p.name, href: docsHref('r', p.id) });
    }
    crumbs.push({ label: e.name });
    setCrumbs(crumbs.filter(c => c.label));
    scrollToAnchor(null);
    return displayName(e);
  }

  // The entry's buttons: edit, source, note, copy name, show the block.
  function entryButtons(e, f, note) {
    const b = [];
    // A button's icon from an SVG path.
    const icon = d => `<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" d="${d}"/></svg>`;
    if (!f.virtual) {
      b.push(`<button type="button" class="btn btn-sm" data-act="edit" data-path="${esc(e.file)}" data-line="${e.line}"
        data-entry="${esc(e.id)}" title="${S.ix.editable ? 'Edit the file at this function' : 'Open the file in the editor (read only here: save from the local console)'}">
        ${icon('M4 20h4L19 9l-4-4L4 16v4zM14 6l4 4')}<span>Edit</span></button>`);
    }
    b.push(`<a class="btn btn-sm" href="${docsHref('f', e.file, isModule(e) ? '' : e.line)}" title="The whole file, with this function marked">
      ${icon('M8 6l-6 6 6 6M16 6l6 6-6 6')}<span>View File</span></a>`);
    b.push(`<button type="button" class="btn btn-sm" data-act="note" data-entry="${esc(e.id)}"
      title="Notes on this function are kept in docs/notes, next to the documentation">${icon('M5 4h10l4 4v12H5zM14 4v5h5')}<span>${note ? 'Edit Note' : 'Add Note'}</span></button>`);
    b.push(`<button type="button" class="btn btn-sm" data-act="copy" data-text="${esc(displayName(e))}" title="Copy the name">
      ${icon('M8 8h11v11H8zM5 16V5h11')}<span>Copy Name</span></button>`);
    if (f.virtual && S.ix.mode === 'local') {
      b.push(`<button type="button" class="btn btn-sm" data-act="block" data-block="${esc(f.block_paths[0])}"
        title="Open the diagram in Simulink at this block (the diagram must be the one selected on the Simulation tab)">
        ${icon('M3 5h7v6H3zM14 13h7v6h-7zM10 8h4v8')}<span>Show in Simulink</span></button>`);
    }
    return b.join('');
  }

  // The structured documentation (from docparse): paragraphs, parameter tables, lists, "See also".
  function docBlocks(doc, skip = 0) {
    const out = [];
    for (const b of doc.blocks.slice(skip)) {
      if (b.title) {
        out.push(`<div class="doc-title">${esc(b.title)}</div>`);
      }
      if (b.type === 'para') {
        if (b.text) {
          out.push(`<p>${inline(b.text)}</p>`);
        }
      } else if (b.type === 'pre') {
        out.push(`<pre>${esc(b.text)}</pre>`);
      } else if (b.type === 'list') {
        const tag = b.ordered ? 'ol' : 'ul';
        out.push(`<${tag}>${b.items.map(t => `<li>${inline(t)}</li>`).join('')}</${tag}>`);
      } else if (b.type === 'params') {
        out.push(`<table class="docs-params"><tbody>${b.items.map(it => `<tr>
          <td>${esc(it.name) || '<span class="muted">—</span>'}</td>
          <td>${it.type ? `<code class="type">${esc(it.type)}</code>` : ''}${inline(it.text)}
            ${it.more ? `<pre>${esc(it.more)}</pre>` : ''}</td></tr>`).join('')}</tbody></table>`);
      } else if (b.type === 'see') {
        out.push(`<p class="see"><span class="muted">See also</span> ${b.items.map(n => {
          const t = findEntry(n);
          return t ? `<a href="${docsHref('r', t.id)}">${esc(n)}</a>` : `<code>${esc(n)}</code>`;
        }).join(' ')}</p>`);
      }
    }
    return out.join('');
  }

  // The note on an entry (docs/notes/<file>.md: its "## name" section, or the text before the
  // first section for the file itself).
  function noteFor(e) {
    const notes = S.ix.notes[noteSource(e)];
    if (!notes) {
      return null;
    }
    const key = noteKey(e);
    const text = key ? notes.entries[key] : notes.file;
    return text ? { text, path: notes.path } : null;
  }

  // The notes file an entry's notes are in: its source file's, or for a diagram block the
  // diagrams' (notes/diagram.md, the same in every project whatever its diagram is called).
  function noteSource(e) {
    const f = S.ix.files[e.file];
    return f && f.virtual ? 'diagram' : e.file;
  }

  // The "## heading" of an entry's note ('' for a file's own): its name in the file; for a
  // diagram block, the block's path (and a local function's name after it).
  function noteKey(e) {
    const f = S.ix.files[e.file];
    if (f && f.virtual) {
      const main = S.ix.entries[f.entries[1]];
      const isMain = e.id === f.entries[0] || (main && e.id === main.id);
      return isMain ? f.blocks[0] : `${f.blocks[0]} › ${e.local || e.name}`;
    }
    return isModule(e) ? '' : (e.local || e.name);
  }

  // A note's box, with its Edit Note button.
  function noteHtml(note, e) {
    return `<div class="docs-note"><div class="docs-note-head">Note
      <button type="button" class="btn btn-sm" data-act="note" data-entry="${esc(e.id)}">Edit Note</button></div>
      <div class="docs-note-body">${md(note.text)}</div></div>`;
  }

  // The walkthrough: each block's explanation over its code; functions defined inside as links.
  function walkthrough(e) {
    const steps = e.chunks || [];
    if (!steps.length) {
      return '';
    }
    const out = [];
    for (const c of steps) {
      if (c.child) {
        const k = S.ix.entries[c.child];
        if (!k) {
          continue;
        }
        out.push(`<a class="docs-child docs-step" data-depth="0" href="${docsHref('r', k.id)}">
          <span class="nm">${esc(k.kind === 'class' ? 'class ' : '')}${esc(k.name)}</span>
          <span class="sm">${inline((k.doc && k.doc.summary) || '')}</span>
          <span class="ln">lines ${k.start}–${k.end} ›</span></a>`);
        continue;
      }
      const lines = c.code ? `<span class="lines">${c.code[0]}–${c.code[1]}</span>` : '';
      const title = c.title ? `<h3 class="docs-step-title" id="step-${c.start}">${esc(c.title)} ${lines}</h3>` : '';
      const text = c.text ? `<div class="docs-step-text">${commentHtml(c.text)}</div>` : '';
      const code = c.code ? codeBlock(e.file, c.code[0], c.code[1]) : '';
      out.push(`<div class="docs-step${c.cont ? ' cont' : ''}" data-depth="${c.depth || 0}">${title}${text}${code}</div>`);
    }
    const what = isModule(e) ? 'The File, Block by Block' : 'The Code, Block by Block';
    return `<div class="docs-section"><h2 id="walkthrough">${what}</h2>
      <p class="muted small">Each block of code under the comment that explains it, as written in the code.
        Click a line number to see it in the whole file.</p>
      <div class="docs-walk">${out.join('')}</div></div>`;
  }

  // The functions defined inside an entry (classes, files, functions with nested ones).
  function insideList(e) {
    const kids = (e.children || []).map(id => S.ix.entries[id]).filter(Boolean);
    if (!kids.length || (e.chunks || []).some(c => c.child)) {
      return '';
    }
    return `<div class="docs-section"><h2 id="inside">Defined Inside <span class="count">${kids.length}</span></h2>
      ${entryList(kids)}</div>`;
  }

  // A list of entries with their summaries.
  function entryList(list) {
    return `<ul class="docs-list">${list.map(k => `<li>
      <a class="nm" href="${docsHref('r', k.id)}">${esc(k.local || k.name)}</a>
      <span class="sm">${inline((k.doc && k.doc.summary) || '')}</span>
      <span class="end"><span class="docs-kind">${esc(KIND_NAMES[k.kind] || k.kind)}</span></span></li>`).join('')}</ul>`;
  }

  // What it calls and what calls it.
  function xrefHtml(e) {
    // A link per entry, with the line of the call.
    const chips = list => list.map(([id, line]) => {
      const t = S.ix.entries[id];
      if (!t) {
        return '';
      }
      const lang = langOf(t);
      return `<a class="docs-chip" href="${docsHref('r', id)}" title="${esc((t.doc && t.doc.summary) || t.qual)}">
        <span class="docs-badge" data-lang="${lang}">${esc((LANG_NAMES[lang] || lang).slice(0, 2))}</span>
        ${esc(displayName(t))}<span class="lnum">:${line}</span></a>`;
    }).join('');
    const parts = [];
    if (e.calls && e.calls.length) {
      parts.push(`<h3 id="calls">Calls</h3><div class="docs-links">${chips(e.calls)}</div>`);
    }
    if (e.called_by && e.called_by.length) {
      parts.push(`<h3 id="called-by">Called By</h3><div class="docs-links">${chips(e.called_by)}</div>`);
    }
    return parts.length ? `<div class="docs-section"><h2 id="links">Connections</h2>
      <p class="muted small">Found by name in the code (a name used two ways can be missed); the number is the line
        of the call.</p>${parts.join('')}</div>` : '';
  }

  // The console's routes and MATLAB commands this entry handles or calls.
  function routesHtml(e) {
    const rows = [];
    for (const r of S.ix.routes) {
      const handles = r.handlers.some(([id]) => id === e.id);
      const calls = r.callers.some(([id]) => id === e.id);
      if (handles || calls) {
        rows.push(`<li><span class="nm">${esc(r.route)}</span><span class="sm">${handles ? 'handled here' : 'requested here'}</span>
          <span class="end"><a href="${docsHref('x', 'routes')}">all routes ›</a></span></li>`);
      }
    }
    for (const c of S.ix.commands) {
      const handles = c.handlers.some(([id]) => id === e.id);
      const calls = c.callers.some(([id]) => id === e.id);
      if (handles || calls) {
        rows.push(`<li><span class="nm">${esc(c.command)}</span><span class="sm">MATLAB command, ${handles ? 'run here' : 'sent from here'}</span>
          <span class="end"><a href="${docsHref('x', 'routes')}#matlab">all commands ›</a></span></li>`);
      }
    }
    return rows.length ? `<div class="docs-section"><h2 id="routes">Routes and Commands</h2>
      <ul class="docs-list">${rows.join('')}</ul></div>` : '';
  }

  // A whole file: what it is, its functions, and its code.
  function renderFile(path, line) {
    const ix = S.ix;
    const f = ix.files[path] || findFile(path);
    if (!f) {
      content.innerHTML = `<h1>Not found</h1><p>No file <code>${esc(path)}</code> is documented.</p>`;
      setCrumbs([{ label: path }]);
      return path;
    }
    content.classList.add('wide');
    const area = ix.areas.get(f.area);
    const module = ix.entries[f.entries[0]];
    const lang = f.group === 'diagram' ? 'diagram' : f.lang;
    const marks = {};
    for (const id of f.entries.slice(1)) {
      const e = ix.entries[id];
      if (e && !marks[e.line]) {
        marks[e.line] = ` <a class="go" href="${docsHref('r', id)}" title="${esc((e.doc && e.doc.summary) || '')}">↗</a>`;
      }
    }
    const top = module ? module.children.map(id => ix.entries[id]).filter(Boolean) : [];
    const note = module ? noteFor(module) : null;
    let body = '';
    if (f.lang === 'markdown') {
      body = `<div class="docs-section">${md(f.text)}</div>`;
    }
    const buttons = [];
    if (!f.virtual) {
      buttons.push(`<button type="button" class="btn btn-sm" data-act="edit" data-path="${esc(f.path)}" data-line="${line || 1}">Edit</button>`);
    }
    if (module) {
      buttons.push(`<a class="btn btn-sm" href="${docsHref('r', module.id)}">Walkthrough</a>`);
      buttons.push(`<button type="button" class="btn btn-sm" data-act="note" data-entry="${esc(module.id)}">${note ? 'Edit Note' : 'Add Note'}</button>`);
    }
    content.innerHTML = `
      <div class="docs-title"><h1>${esc(f.virtual ? f.blocks[0] : f.path.split('/').pop())}</h1>
        <span class="docs-badge" data-lang="${lang}">${esc(LANG_NAMES[lang] || lang)}</span></div>
      <div class="docs-where"><span class="mono">${esc(f.virtual ? f.slx : f.path)}</span><span>${f.lines} lines</span>
        ${area ? `<a href="${docsHref('a', area.key)}">${esc(area.title)}</a>` : ''}</div>
      ${module && module.doc && module.doc.summary ? `<p class="lead">${inline(module.doc.summary)}</p>` : ''}
      <div class="docs-btnrow">${buttons.join('')}</div>
      ${f.error ? `<div class="docs-missing">${esc(f.error)}</div>` : ''}
      ${module && module.doc ? `<div class="docs-doc">${docBlocks(module.doc)}</div>` : ''}
      ${note ? noteHtml(note, module) : ''}
      ${top.length ? `<div class="docs-section"><h2 id="functions">Functions <span class="count">${top.length}</span></h2>${entryList(top)}</div>` : ''}
      ${body}
      <div class="docs-section"><h2 id="source">The Code</h2>
        ${codeBlock(f.path, 1, f.lines, { ids: true, marks })}</div>`;
    setCrumbs([{ label: area ? area.groupTitle : '', href: area ? docsHref('a', area.key) : '' },
      { label: area ? area.title : '', href: area ? docsHref('a', area.key) : '' },
      { label: f.path.split('/').pop() }].filter(c => c.label));
    scrollToAnchor(line ? `L-${line}` : null);
    return f.path.split('/').pop();
  }

  // An area (a MATLAB package, the server, the page, a lab program, a diagram): its files and
  // functions.
  function renderArea(key) {
    const ix = S.ix;
    const area = ix.areas.get(key);
    if (!area) {
      return renderHome();
    }
    const group = ix.groups.find(g => g.areas.includes(area));
    const files = area.files.map(p => ix.files[p]).filter(Boolean);
    const contents = ix.notes[`areas/${key}`];
    const fileRows = files.map(f => {
      const module = ix.entries[f.entries[0]];
      const n = f.entries.length - 1;
      return `<li><a class="nm" href="${docsHref(module && f.virtual ? 'r' : 'f', module && f.virtual ? module.id : f.path)}">${esc(f.virtual ? f.blocks[0] : f.path.split('/').pop())}</a>
        <span class="sm">${inline(f.summary || '')}</span>
        <span class="end">${n ? `<span class="muted small">${n} ${n === 1 ? 'function' : 'functions'}</span>` : ''}
        <span class="docs-badge" data-lang="${f.group === 'diagram' ? 'diagram' : f.lang}">${esc(LANG_NAMES[f.lang] || f.lang)}</span></span></li>`;
    }).join('');
    const others = group.areas.filter(a => a !== area).map(a =>
      `<a class="docs-chip" href="${docsHref('a', a.key)}">${esc(a.title)}</a>`).join('');
    content.innerHTML = `
      <div class="docs-title"><h1 class="prose">${esc(area.title)}</h1></div>
      <p class="lead">${inline(area.summary || '')}</p>
      ${contents ? md(contents.file) : ''}
      <div class="docs-section"><h2 id="files">${group.key === 'diagram' ? 'MATLAB Function Blocks' : 'Files'}
        <span class="count">${files.length}</span></h2><ul class="docs-list">${fileRows}</ul></div>
      ${others ? `<div class="docs-section"><h2 id="more">More in ${esc(group.title)}</h2><div class="docs-links">${others}</div></div>` : ''}`;
    setCrumbs([{ label: group.title, href: docsHref('a', group.areas[0].key) },
      { label: area.title }]);
    scrollToAnchor(null);
    return area.title;
  }

  // The console's HTTP routes and the MATLAB worker's commands: who handles each, who calls it.
  function renderRoutes() {
    const ix = S.ix;
    // The entries that handle or request a route, as links ('—' for none).
    const links = list => list.length ? list.map(([id, line]) => {
      const e = ix.entries[id];
      return e ? `<a class="docs-chip" href="${docsHref('r', id)}">${esc(displayName(e))}<span class="lnum">:${line}</span></a>` : '';
    }).join(' ') : '<span class="muted">—</span>';
    const routeRows = ix.routes.map(r => `<tr><td class="mono">${esc(r.route)}</td><td>${links(r.handlers)}</td>
      <td>${links(r.callers)}</td></tr>`).join('');
    const cmdRows = ix.commands.map(c => `<tr><td class="mono">${esc(c.command)}</td><td>${links(c.handlers)}</td>
      <td>${links(c.callers)}</td></tr>`).join('');
    content.classList.add('wide');
    content.innerHTML = `
      <div class="docs-title"><h1 class="prose">API Routes and MATLAB Commands</h1></div>
      <p class="lead">How the page talks to its server, and the server to MATLAB, found in the code.</p>
      <p>The page (JavaScript) asks the server (Python) with <code>/api/...</code> addresses: GET to read, POST to act.
        On the local console, <code>/api/matlab</code> passes a command on to the MATLAB worker
        (<code>spot.worker.Worker</code>), which runs it in your MATLAB session.</p>
      <h2 id="http">HTTP Routes</h2>
      <table class="docs-params"><thead><tr><th>Route</th><th>Handled by</th><th>Requested by</th></tr></thead>
        <tbody>${routeRows}</tbody></table>
      <h2 id="matlab">MATLAB Commands</h2>
      <table class="docs-params"><thead><tr><th>Command</th><th>Run by</th><th>Sent by</th></tr></thead>
        <tbody>${cmdRows}</tbody></table>`;
    setCrumbs([{ label: 'API Routes and MATLAB Commands' }]);
    scrollToAnchor(parseRoute(location.hash).extra || null);
    return 'API Routes and MATLAB Commands';
  }

  // Every function, by name.
  function renderAZ() {
    const all = Object.values(S.ix.entries).filter(e => !isModule(e))
      .sort((a, b) => a.name.localeCompare(b.name, undefined, { sensitivity: 'base' }));
    const groups = new Map();
    for (const e of all) {
      const letter = /^[a-z]/i.test(e.name) ? e.name[0].toUpperCase() : '#';
      if (!groups.has(letter)) {
        groups.set(letter, []);
      }
      groups.get(letter).push(e);
    }
    const jump = [...groups.keys()].map(l => `<a href="#" data-jump="az-${l}">${l}</a>`).join(' ');
    const body = [...groups].map(([l, list]) => `<h2 id="az-${l}">${l}</h2><ul class="docs-list">${list.map(e => {
      const lang = langOf(e);
      return `<li><a class="nm" href="${docsHref('r', e.id)}">${esc(e.name)}</a>
        <span class="sm">${inline((e.doc && e.doc.summary) || '')}</span>
        <span class="end"><span class="muted small mono">${esc(displayName(e) !== e.name ? displayName(e) : e.file.split('/').pop())}</span>
        <span class="docs-badge" data-lang="${lang}">${esc(LANG_NAMES[lang] || lang)}</span></span></li>`;
    }).join('')}</ul>`).join('');
    content.classList.add('wide');
    content.innerHTML = `<div class="docs-title"><h1 class="prose">Every Function, A to Z</h1></div>
      <p class="lead">${all.length.toLocaleString()} functions, methods, classes and blocks.</p><p>${jump}</p>${body}`;
    setCrumbs([{ label: 'A to Z' }]);
    scrollToAnchor(null);
    return 'Every Function, A to Z';
  }

  // Contents (left)

  // The contents: the guides, or the reference's tree (opened down to what is shown).
  function renderTree() {
    const ix = S.ix;
    if (!ix) {
      return;
    }
    for (const b of $$('[data-docnav]')) {
      b.classList.toggle('active', b.dataset.docnav === S.nav);
    }
    const filter = $('#docsFilter');
    filter.hidden = S.nav !== 'reference';
    const tree = $('#docsTree');
    const here = location.hash;
    if (S.nav === 'guides') {
      const sections = ix.guides.sections.map(s => `<div class="sec">${esc(s.title)}</div><ul>${s.pages.map(id => {
        const p = ix.guides.pages[id];
        const href = docsHref('g', id);
        return `<li><a href="${href}" class="${here.startsWith(href) ? 'current' : ''}"><span class="label">${esc(p.title)}</span></a></li>`;
      }).join('')}</ul>`).join('');
      tree.innerHTML = `${sections}<div class="sec">Reference Tools</div><ul>
        <li><a href="${docsHref('x', 'routes')}" class="${here.startsWith('#docs/x/routes') ? 'current' : ''}"><span class="label">API Routes and MATLAB Commands</span></a></li>
        <li><a href="${docsHref('x', 'az')}" class="${here.startsWith('#docs/x/az') ? 'current' : ''}"><span class="label">Every Function, A to Z</span></a></li></ul>`;
    } else if (filter.value.trim()) {
      tree.innerHTML = filteredTree(filter.value.trim().toLowerCase());
    } else {
      openToCurrent();
      tree.innerHTML = ix.groups.map(g => `<div class="sec">${esc(g.title)}</div><ul>${g.areas.map(a =>
        areaNode(a)).join('')}</ul>`).join('');
    }
    const cur = $('a.current', tree);
    if (cur) {
      const box = tree.getBoundingClientRect();
      const r = cur.getBoundingClientRect();
      if (r.top < box.top || r.bottom > box.bottom) {
        cur.scrollIntoView({ block: 'center' });
      }
    }
  }

  // Opens the tree down to the entry, file or area shown.
  function openToCurrent() {
    const r = parseRoute(location.hash);
    const ix = S.ix;
    let e = r.kind === 'r' ? ix.entries[r.arg] : null;
    const f = r.kind === 'f' ? ix.files[r.arg] : (e ? ix.files[e.file] : null);
    if (f) {
      S.open.add(`a:${f.area}`);
      S.open.add(`f:${f.path}`);
    }
    if (r.kind === 'a') {
      S.open.add(`a:${r.arg}`);
    }
    for (e = e && ix.entries[e.parent]; e && !isModule(e); e = ix.entries[e.parent]) {
      S.open.add(`e:${e.id}`);
    }
  }

  // A branch of the reference's tree: an area (package or folder) with its files when open.
  function areaNode(a) {
    const key = `a:${a.key}`;
    const isOpen = S.open.has(key);
    const href = docsHref('a', a.key);
    const files = isOpen ? `<ul>${a.files.map(p => fileNode(S.ix.files[p])).join('')}</ul>` : '';
    return `<li class="${isOpen ? 'open' : ''}"><a href="${href}" data-toggle="${esc(key)}" class="${location.hash === href ? 'current' : ''}">
      <span class="twisty">▶</span><span class="label">${esc(a.title)}</span><span class="count">${a.files.length}</span></a>${files}</li>`;
  }

  // A file in the tree, with its functions when open.
  function fileNode(f) {
    if (!f) {
      return '';
    }
    const key = `f:${f.path}`;
    const module = S.ix.entries[f.entries[0]];
    const kids = module ? module.children.map(id => S.ix.entries[id]).filter(Boolean) : [];
    const isOpen = S.open.has(key) && kids.length;
    const href = f.virtual && module ? docsHref('r', module.id) : docsHref('f', f.path);
    const name = f.virtual ? f.blocks[0].split('/').slice(-2).join('/') : f.path.split('/').pop();
    const current = location.hash === href || location.hash.startsWith(`${href}/`);
    return `<li class="${isOpen ? 'open' : ''}"><a href="${href}" ${kids.length ? `data-toggle="${esc(key)}"` : ''} class="${current ? 'current' : ''}"
      title="${esc(f.summary || f.path)}"><span class="twisty">${kids.length ? '▶' : ''}</span><span class="label mono">${esc(name)}</span></a>
      ${isOpen ? `<ul>${kids.map(entryNode).join('')}</ul>` : ''}</li>`;
  }

  // A function in the tree, with the functions defined inside it when open.
  function entryNode(e) {
    const kids = (e.children || []).map(id => S.ix.entries[id]).filter(Boolean);
    const key = `e:${e.id}`;
    const isOpen = S.open.has(key) && kids.length;
    const href = docsHref('r', e.id);
    return `<li class="${isOpen ? 'open' : ''}"><a href="${href}" ${kids.length ? `data-toggle="${esc(key)}"` : ''}
      class="${location.hash === href ? 'current' : ''}" title="${esc((e.doc && e.doc.summary) || '')}">
      <span class="twisty">${kids.length ? '▶' : ''}</span><span class="label mono">${esc(e.name)}</span></a>
      ${isOpen ? `<ul>${kids.map(entryNode).join('')}</ul>` : ''}</li>`;
  }

  // The reference's entries whose names contain the filter, by file.
  function filteredTree(q) {
    const hits = Object.values(S.ix.entries)
      .filter(e => (e.local || e.name || '').toLowerCase().includes(q) ||
        e.file.toLowerCase().split('/').pop().includes(q))
      .slice(0, 400);
    if (!hits.length) {
      return '<p class="muted small">Nothing matches.</p>';
    }
    // The name with the filter's text highlighted.
    const mark = s => esc(s).replace(new RegExp(q.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'), 'ig'), m => `<mark>${m}</mark>`);
    return `<ul>${hits.map(e => `<li><a href="${docsHref('r', e.id)}" title="${esc(e.file)}">
      <span class="docs-badge" data-lang="${langOf(e)}">${esc((LANG_NAMES[langOf(e)] || '').slice(0, 2))}</span>
      <span class="label mono">${mark(isModule(e) ? e.file.split('/').pop() : (e.local || e.name))}</span></a></li>`).join('')}</ul>`;
  }

  // The footer under the contents: how much there is, and whether this console saves.
  function renderStatus() {
    const ix = S.ix;
    const n = Object.values(ix.entries).filter(e => !isModule(e)).length;
    const mode = ix.editable
      ? 'Local console: edits are saved to this project.'
      : 'Lab console: read only. Edit and save from the local console.';
    $('#docsStatus').innerHTML = `${n.toLocaleString()} functions in ${Object.keys(ix.files).length} files,
      read from the code (${ix.seconds} s).<br>${mode}`;
  }

  // On this page (right): the headings, or the editor's problems.
  function renderToc() {
    const toc = $('#docsToc');
    const heads = $$('h2[id], h3[id]', content).filter(h => !h.closest('.docs-note'));
    page.classList.toggle('no-toc', heads.length < 2);
    if (heads.length < 2) {
      toc.innerHTML = '';
      return;
    }
    toc.innerHTML = '<h3>On This Page</h3>' + heads.map(h =>
      `<a href="#" data-jump="${esc(h.id)}" class="d${h.tagName[1]}">${esc(h.textContent.replace(/\s+\d+[–-]\d+$/, '').trim())}</a>`).join('');
    spy();
  }

  // Marks the heading being read in "On this page".
  function spy() {
    const links = $$('#docsToc a[data-jump]');
    if (!links.length) {
      return;
    }
    const limit = scroller.getBoundingClientRect().top + 80;
    let active = links[0];
    for (const a of links) {
      const h = document.getElementById(a.dataset.jump);
      if (h && h.getBoundingClientRect().top <= limit) {
        active = a;
      }
    }
    for (const a of links) {
      a.classList.toggle('active', a === active);
    }
  }
  scroller.addEventListener('scroll', debounce(spy, 60), { passive: true });

  // Search

  /**
   * Builds the search's lower-case text, once per index.
   * @return {!Object} {entries: [...], guides: [...]}.
   */
  function searchData() {
    const ix = S.ix;
    if (ix.search) {
      return ix.search;
    }
    // An entry's documentation as one lower-case string.
    const flat = doc => {
      if (!doc) {
        return '';
      }
      const parts = [doc.summary];
      for (const b of doc.blocks) {
        parts.push(b.title, b.text);
        for (const it of b.items || []) {
          parts.push(typeof it === 'string' ? it : `${it.name} ${it.type} ${it.text} ${it.more || ''}`);
        }
      }
      return parts.filter(Boolean).join(' ').toLowerCase();
    };
    const entries = Object.values(ix.entries).map(e => {
      const note = noteFor(e);
      return {
        e, name: (e.name || '').toLowerCase(), local: (e.local || '').toLowerCase(),
        qual: (e.qual || '').toLowerCase(), file: e.file.toLowerCase(),
        summary: ((e.doc && e.doc.summary) || '').toLowerCase(), doc: flat(e.doc) + (note ? ` ${note.text.toLowerCase()}` : ''),
        steps: (e.chunks || []).map(c => `${c.title || ''} ${c.text || ''}`).join(' ').toLowerCase(),
      };
    });
    const guides = Object.values(ix.guides.pages).map(p => {
      const heads = [];
      p.text.split('\n').forEach((line, i) => {
        const m = /^(#{1,4})\s+(.*)$/.exec(line);
        if (m) {
          heads.push({ text: m[2].replace(/[*_`]/g, ''), line: i });
        }
      });
      return {
        p, title: p.title.toLowerCase(), text: p.text.toLowerCase(), lines: p.text.split('\n'),
        heads,
      };
    });
    ix.search = { entries, guides };
    return ix.search;
  }

  /**
   * Reads a search: its words, "phrases" and filters (lang:, kind:, in:, file:).
   * @param {string} q What was typed.
   * @return {{terms: !Array<string>, lang: string, kind: string, scope: string, file: string}}
   */
  function parseQuery(q) {
    const out = { terms: [], lang: '', kind: '', scope: '', file: '' };
    const re = /(\w+):("[^"]*"|\S+)|"([^"]+)"|(\S+)/g;
    let m;
    while ((m = re.exec(q))) {
      if (m[1] && ['lang', 'kind', 'in', 'file'].includes(m[1].toLowerCase())) {
        const v = m[2].replace(/"/g, '').toLowerCase();
        out[m[1].toLowerCase() === 'in' ? 'scope' : m[1].toLowerCase()] = v;
      } else {
        const t = (m[3] || m[0]).toLowerCase();
        if (t) {
          out.terms.push(t);
        }
      }
    }
    if (out.lang) {
      const short = { js: 'javascript', py: 'python', 'c++': 'cpp', m: 'matlab', sh: 'shell' };
      const want = short[out.lang] || out.lang;
      const named = Object.entries(LANG_NAMES).find(([k, v]) => v.toLowerCase() === want ||
        k === want);
      out.lang = named ? named[0] : want;
    }
    return out;
  }

  /**
   * Searches functions, guides and code.
   * @param {string} text What was typed.
   * @param {{entries: number, guides: number, lines: number}} limits How many of each.
   * @return {{query: !Object, entries: !Array, guides: !Array, lines: !Array, more: !Object}}
   */
  function search(text, limits) {
    const q = parseQuery(text);
    const data = searchData();
    const result = {
      query: q, entries: [], guides: [], lines: [], more: { entries: 0, guides: 0, lines: 0 },
    };
    if (!q.terms.length && !q.lang && !q.kind && !q.file) {
      return result;
    }
    const wantEntries = !q.scope || ['functions', 'function', 'docs', 'code'].includes(q.scope);
    const wantGuides = (!q.scope || q.scope.startsWith('guide')) && !q.lang && !q.kind && !q.file;
    const wantCode = (!q.scope || q.scope === 'code') && q.terms.length;

    // Functions: by name, then what their documentation and comments say, then their code.
    if (wantEntries) {
      const scored = [];
      for (const d of data.entries) {
        const e = d.e;
        if ((q.lang && langOf(e) !== q.lang && e.lang !== q.lang) ||
          (q.kind && !(KIND_NAMES[e.kind] || e.kind).includes(q.kind) && e.kind !== q.kind) ||
          (q.file && !d.file.includes(q.file))) {
          continue;
        }
        let score = 0;
        let ok = true;
        for (const t of q.terms) {
          let s = 0;
          if (d.name === t) {
            s = 100;
          } else if (d.name.startsWith(t)) {
            s = 60;
          } else if (d.name.includes(t)) {
            s = 42;
          } else if (d.local.includes(t) || d.qual.includes(t)) {
            s = 34;
          } else if (d.summary.includes(t)) {
            s = 18;
          } else if (d.doc.includes(t)) {
            s = 10;
          } else if (d.steps.includes(t)) {
            s = 6;
          } else if (q.scope !== 'docs' && codeHas(e, t)) {
            s = 3;
          }
          if (!s) {
            ok = false;
            break;
          }
          score += s;
        }
        if (!ok) {
          continue;
        }
        if (isModule(e)) {
          score -= 4;
        }
        score -= Math.min(6, (e.local || e.name).length / 12);
        scored.push([score, e]);
      }
      scored.sort((a, b) => b[0] - a[0]);
      result.entries = scored.slice(0, limits.entries).map(s => s[1]);
      result.more.entries = Math.max(0, scored.length - limits.entries);
    }

    // Guides: by title, headings, then text; each to the heading above its first match.
    if (wantGuides && q.terms.length) {
      const scored = [];
      for (const g of data.guides) {
        let score = 0;
        let ok = true;
        for (const t of q.terms) {
          const inHead = g.heads.some(h => h.text.toLowerCase().includes(t));
          const s = g.title.includes(t) ? 50 : inHead ? 25 : g.text.includes(t) ? 8 : 0;
          if (!s) {
            ok = false;
            break;
          }
          score += s;
        }
        if (ok) {
          scored.push([score, g]);
        }
      }
      scored.sort((a, b) => b[0] - a[0]);
      result.guides = scored.slice(0, limits.guides).map(([, g]) => guideHit(g, q.terms));
      result.more.guides = Math.max(0, scored.length - limits.guides);
    }

    // Code: lines with every word.
    if (wantCode) {
      let count = 0;
      for (const [path, f] of Object.entries(S.ix.files)) {
        if ((q.file && !path.toLowerCase().includes(q.file)) ||
          (q.lang && f.lang !== q.lang && !(q.lang === 'diagram' && f.group === 'diagram'))) {
          continue;
        }
        const lower = S.ix.lower(path);
        for (let i = 0; i < lower.length; i++) {
          if (q.terms.every(t => lower[i].includes(t))) {
            count++;
            if (result.lines.length < limits.lines) {
              result.lines.push({ path, line: i + 1 });
            }
          }
        }
      }
      result.more.lines = Math.max(0, count - result.lines.length);
    }
    return result;
  }

  // Whether an entry's own lines contain a word.
  function codeHas(e, t) {
    const lower = S.ix.lower(e.file);
    for (let i = e.body - 1; i < Math.min(e.end, lower.length); i++) {
      if (lower[i].includes(t)) {
        return true;
      }
    }
    return false;
  }

  // A guide that matched: the heading above the first match, and the text around it.
  function guideHit(g, terms) {
    const i = g.lines.findIndex(l => terms.some(t => l.toLowerCase().includes(t)));
    const head = [...g.heads].reverse().find(h => h.line <= i);
    const line = i >= 0 ? g.lines[i].replace(/[#*_`>|]/g, ' ').trim() : '';
    return {
      p: g.p, anchor: head && head.line > 0 ? slug(head.text) : '', head: head ? head.text : '',
      snip: line,
    };
  }

  // The innermost entry around a line of a file.
  function entryAt(path, line) {
    const f = S.ix.files[path];
    let best = null;
    for (const id of f ? f.entries : []) {
      const e = S.ix.entries[id];
      const inside = e && e.start <= line && line <= e.end;
      if (inside && (!best || e.end - e.start < best.end - best.start)) {
        best = e;
      }
    }
    return best;
  }

  // Highlights the searched words in some escaped text.
  function markTerms(html, terms) {
    let out = html;
    for (const t of terms.filter(x => x.length > 1)) {
      const re = new RegExp(esc(t).replace(/[.*+?^${}()|[\]\\]/g, '\\$&'), 'ig');
      out = out.replace(/(^|>)([^<]*)/g, (m, a, text) => a + text.replace(re, w => `<mark>${w}</mark>`));
    }
    return out;
  }

  // One result: a function, a guide, or a line of code.
  function resultHtml(item, terms, sel = false) {
    const cls = `docs-result${sel ? ' sel' : ''}`;
    if (item.e || item.kind) {
      const e = item.e || item;
      const lang = langOf(e);
      return `<a class="${cls}" href="${docsHref('r', e.id)}" data-find="${esc(terms.join(' '))}">
        <div class="top"><span class="docs-badge" data-lang="${lang}">${esc(LANG_NAMES[lang] || lang)}</span>
          <span class="nm">${markTerms(esc(displayName(e)), terms)}</span>
          <span class="docs-kind">${esc(KIND_NAMES[e.kind] || e.kind)}</span>
          <span class="where">${esc(whereText(e))}</span></div>
        <div class="sm">${markTerms(esc((e.doc && e.doc.summary) || ''), terms)}</div></a>`;
    }
    if (item.p) {
      return `<a class="${cls}" href="${docsHref('g', item.p.id, item.anchor)}" data-find="${esc(terms.join(' '))}">
        <div class="top"><span class="docs-kind">Guide</span><span class="nm">${markTerms(esc(item.p.title), terms)}</span>
          ${item.head ? `<span class="where">› ${esc(item.head)}</span>` : ''}</div>
        <div class="sm">${markTerms(esc(item.snip.slice(0, 220)), terms)}</div></a>`;
    }
    const e = entryAt(item.path, item.line);
    const text = (S.ix.lines(item.path)[item.line - 1] || '').trim();
    const href = e && !isModule(e) ? docsHref('r', e.id) : docsHref('f', item.path, item.line);
    return `<a class="${cls}" href="${href}" data-find="${esc(terms.join(' '))}">
      <div class="top"><span class="where mono">${esc(item.path.split('/').pop())}:${item.line}</span>
        ${e && !isModule(e) ? `<span class="where">in ${esc(displayName(e))}</span>` : ''}</div>
      <div class="snip">${markTerms(esc(text.slice(0, 240)), terms)}</div></a>`;
  }

  // The results page (Enter in the search box).
  function renderSearchPage(text) {
    const r = search(text, { entries: RESULT_LIMIT, guides: RESULT_LIMIT, lines: CODE_LIMIT });
    const t = r.query.terms;
    // "and N more" under a section cut short, and a section of results (none: nothing).
    const more = n => (n ? `<p class="muted small docs-more">and ${n} more: add words or a filter to narrow it.</p>` : '');
    const sect = (title, id, items, n) => items.length
      ? `<h2 id="${id}">${title} <span class="count">${items.length + n}</span></h2>${items.map(i => resultHtml(i, t)).join('')}${more(n)}`
      : '';
    const none = !r.entries.length && !r.guides.length && !r.lines.length;
    content.innerHTML = `<div class="docs-results">
      <div class="docs-title"><h1 class="prose">Search: ${esc(text)}</h1></div>
      <p class="muted">Filters: <code>lang:matlab</code> <code>kind:method</code> <code>in:code</code> <code>in:guides</code>
        <code>in:docs</code> <code>file:sim.js</code>, and <code>"a phrase"</code>.</p>
      ${none ? '<p class="docs-missing">Nothing found. Try fewer words, or part of a name.</p>' : ''}
      ${sect('Functions', 'functions', r.entries.map(e => ({ e })), r.more.entries)}
      ${sect('Guides', 'guides', r.guides, r.more.guides)}
      ${sect('In the Code', 'code', r.lines, r.more.lines)}</div>`;
    setCrumbs([{ label: `Search: ${text}` }]);
    scrollToAnchor(null);
    $('#docsSearch').value = text;
    return `Search: ${text}`;
  }

  // Suggestions under the search box as you type.
  const suggest = debounce(() => {
    const text = $('#docsSearch').value.trim();
    const box = $('#docsSuggest');
    if (!text || !S.ix) {
      box.hidden = true;
      return;
    }
    const r = search(text, {
      entries: SUGGEST_ENTRIES, guides: SUGGEST_GUIDES, lines: SUGGEST_LINES,
    });
    const t = r.query.terms;
    const items = [...r.entries.map(e => ({ e })), ...r.guides, ...r.lines];
    S.suggest = { items, sel: -1 };
    let i = 0;
    // Each suggestion numbered (for the arrow keys), in groups.
    const numbered = it => resultHtml(it, t, false)
      .replace('class="docs-result', `data-i="${i++}" class="docs-result`);
    const group = (title, list) => (list.length
      ? `<div class="grp">${title}</div>${list.map(numbered).join('')}` : '');
    const found = group('Functions', r.entries.map(e => ({ e }))) + group('Guides', r.guides) +
      group('In the Code', r.lines);
    box.innerHTML = (items.length ? found : '<div class="grp">Nothing found</div>') +
      `<div class="foot">Enter: all results · ↑↓ to choose · Esc to close</div>`;
    box.hidden = false;
  }, 90);

  // Moves the chosen suggestion up or down (wrapping round).
  function moveSuggestion(step) {
    const n = S.suggest.items.length;
    if (!n) {
      return;
    }
    S.suggest.sel = (S.suggest.sel + step + n) % n;
    for (const el of $$('#docsSuggest .docs-result')) {
      el.classList.toggle('sel', Number(el.dataset.i) === S.suggest.sel);
      if (Number(el.dataset.i) === S.suggest.sel) {
        el.scrollIntoView({ block: 'nearest' });
      }
    }
  }

  const searchBox = $('#docsSearch');
  searchBox.addEventListener('input', suggest);
  searchBox.addEventListener('focus', () => {
    if (searchBox.value.trim()) {
      suggest();
    }
  });
  searchBox.addEventListener('keydown', ev => {
    if (ev.key === 'ArrowDown' || ev.key === 'ArrowUp') {
      ev.preventDefault();
      moveSuggestion(ev.key === 'ArrowDown' ? 1 : -1);
    } else if (ev.key === 'Enter') {
      ev.preventDefault();
      const chosen = $(`#docsSuggest .docs-result[data-i="${S.suggest.sel}"]`);
      $('#docsSuggest').hidden = true;
      if (chosen && S.suggest.sel >= 0) {
        S.pendingFind = chosen.dataset.find || null;
        location.hash = chosen.getAttribute('href');
      } else if (searchBox.value.trim()) {
        location.hash = docsHref('s', searchBox.value.trim());
      }
    } else if (ev.key === 'Escape') {
      $('#docsSuggest').hidden = true;
      searchBox.blur();
    }
  });
  document.addEventListener('pointerdown', ev => {
    if (!ev.target.closest('.docs-search')) {
      $('#docsSuggest').hidden = true;
    }
  });

  // Find on this page

  // Opens the Find bar, with a word to find (or what it had), and highlights the matches.
  function openFind(text) {
    $('#docsFind').hidden = false;
    const input = $('#docsFindInput');
    if (text !== undefined) {
      input.value = longestTerm(text);
    }
    applyFind();
    input.focus();
    input.select();
  }

  // The longest word of a search, to find on the page it led to.
  function longestTerm(text) {
    const q = parseQuery(text);
    return q.terms.sort((a, b) => b.length - a.length)[0] || '';
  }

  // Closes the Find bar and removes the highlights (and its text, unless clear is false).
  function closeFind(clear = true) {
    clearFind();
    $('#docsFind').hidden = true;
    if (clear) {
      $('#docsFindInput').value = '';
    }
  }

  // Removes the highlights.
  function clearFind() {
    for (const m of $$('mark.docs-hit', content)) {
      const parent = m.parentNode;
      parent.replaceChild(document.createTextNode(m.textContent), m);
      parent.normalize();
    }
    S.find = { hits: [], i: -1 };
    $('#docsFindCount').textContent = '';
  }

  // Highlights every match on the page. Text is matched within each line or paragraph, so a
  // match may span several highlighted pieces of code.
  function applyFind() {
    clearFind();
    const q = $('#docsFindInput').value;
    if (!q) {
      return;
    }
    const exact = $('#docsFindCase').checked;
    const needle = exact ? q : q.toLowerCase();
    const blocks = new Map();
    const walker = document.createTreeWalker(content, NodeFilter.SHOW_TEXT, {
      // Not inside maths, buttons or the copy buttons.
      acceptNode: n => (n.parentElement.closest('.katex, button, .docs-copy, script, style')
        ? NodeFilter.FILTER_REJECT : NodeFilter.FILTER_ACCEPT),
    });
    for (let n = walker.nextNode(); n; n = walker.nextNode()) {
      const block = n.parentElement.closest(FIND_BLOCKS);
      if (!blocks.has(block)) {
        blocks.set(block, []);
      }
      blocks.get(block).push(n);
    }
    const marks = [];
    for (const nodes of blocks.values()) {
      const texts = nodes.map(n => n.data);
      const joined = texts.join('');
      const hay = exact ? joined : joined.toLowerCase();
      const found = [];
      for (let i = hay.indexOf(needle); i >= 0; i = hay.indexOf(needle, i + needle.length)) {
        found.push(i);
      }
      // Wrap from the last match back, so earlier offsets stay right.
      for (const start of found.reverse()) {
        const group = [];
        let offset = 0;
        for (let k = 0; k < nodes.length; k++) {
          const a = Math.max(start, offset);
          const b = Math.min(start + needle.length, offset + texts[k].length);
          if (a < b) {
            group.push([k, a - offset, b - offset]);
          }
          offset += texts[k].length;
        }
        const pieces = [];
        for (const [k, a, b] of group.reverse()) {
          const node = nodes[k];
          const mid = node.splitText(a);
          mid.splitText(b - a);
          const mark = document.createElement('mark');
          mark.className = 'docs-hit';
          mid.parentNode.replaceChild(mark, mid);
          mark.append(mid);
          pieces.unshift(mark);
        }
        marks.push(pieces);
      }
    }
    // In page order.
    const before = (a, b) => a[0].compareDocumentPosition(b[0]) & Node.DOCUMENT_POSITION_FOLLOWING;
    marks.sort((a, b) => (before(a, b) ? -1 : 1));
    S.find = { hits: marks, i: marks.length ? 0 : -1 };
    showHit();
  }

  // Marks the current match, scrolls to it, and says which of how many it is.
  function showHit() {
    const { hits, i } = S.find;
    for (const m of $$('mark.docs-hit.now', content)) {
      m.classList.remove('now');
    }
    $('#docsFindCount').textContent = hits.length ? `${i + 1} of ${hits.length}` : 'None';
    if (i >= 0) {
      for (const m of hits[i]) {
        m.classList.add('now');
      }
      hits[i][0].scrollIntoView({ block: 'center', behavior: 'smooth' });
    }
  }

  // Goes to the next (1) or previous (-1) match.
  function stepFind(step) {
    const n = S.find.hits.length;
    if (n) {
      S.find.i = (S.find.i + step + n) % n;
      showHit();
    }
  }

  $('#docsFindBtn').onclick = () => openFind();
  $('#docsFindClose').onclick = () => closeFind();
  $('#docsFindPrev').onclick = () => stepFind(-1);
  $('#docsFindNext').onclick = () => stepFind(1);
  $('#docsFindCase').onchange = applyFind;
  $('#docsFindInput').addEventListener('input', debounce(applyFind, 120));
  $('#docsFindInput').addEventListener('keydown', ev => {
    if (ev.key === 'Enter') {
      ev.preventDefault();
      stepFind(ev.shiftKey ? -1 : 1);
    } else if (ev.key === 'Escape') {
      closeFind();
    }
  });

  // Clicks on the pages

  content.addEventListener('click', ev => {
    const t = ev.target;
    const btn = t.closest('[data-act]');
    if (btn) {
      ev.preventDefault();
      pageAction(btn);
      return;
    }
    const n = t.closest('.docs-code .n');
    if (n) {
      const path = n.closest('.docs-code').dataset.path;
      location.hash = docsHref('f', path, n.dataset.line);
      return;
    }
    const jump = t.closest('[data-jump]');
    if (jump) {
      ev.preventDefault();
      scrollToAnchor(jump.dataset.jump);
      return;
    }
    const copy = t.closest('.docs-copy');
    if (copy) {
      copyText(copy.parentElement.querySelector('code').textContent);
      copy.textContent = 'Copied';
      setTimeout(() => {
        copy.textContent = 'Copy';
      }, 1200);
      return;
    }
    const link = t.closest('a[data-find]');
    if (link) {
      S.pendingFind = link.dataset.find;
    }
  });
  $('#docsActions').addEventListener('click', ev => {
    const btn = ev.target.closest('[data-act]');
    if (btn) {
      pageAction(btn);
    }
  });
  $('#docsToc').addEventListener('click', ev => {
    const a = ev.target.closest('[data-jump]');
    if (a) {
      ev.preventDefault();
      scrollToAnchor(a.dataset.jump);
      return;
    }
    const p = ev.target.closest('[data-pline]');
    if (p && S.ed) {
      gotoLine(Number(p.dataset.pline), Number(p.dataset.pcol));
    }
  });
  $('#docsSuggest').addEventListener('click', ev => {
    const a = ev.target.closest('a[data-find]');
    if (a) {
      S.pendingFind = a.dataset.find;
      $('#docsSuggest').hidden = true;
    }
  });

  // The pages' buttons: edit, note, copy, show the block in Simulink.
  function pageAction(btn) {
    const act2 = btn.dataset.act;
    if (act2 === 'edit') {
      if (btn.dataset.entry) {
        S.markEntry = btn.dataset.entry;
      }
      S.beforeEditor = location.hash;
      location.hash = docsHref('e', btn.dataset.path, btn.dataset.line || '');
    } else if (act2 === 'note') {
      openNote(S.ix.entries[btn.dataset.entry]);
    } else if (act2 === 'copy') {
      copyText(btn.dataset.text);
      toast(`Copied ${btn.dataset.text}`, 'success', 2500);
    } else if (act2 === 'block') {
      act(async () => {
        await api('/api/matlab', { cmd: 'diagram.showBlock', args: { block: btn.dataset.block } });
        toast('The block is highlighted in Simulink.', 'success');
      });
    }
  }

  // Copies text to the clipboard, where the browser allows it.
  function copyText(text) {
    if (navigator.clipboard) {
      navigator.clipboard.writeText(text).catch(() => {
        // Clipboard blocked (not a secure page): nothing to do.
      });
    }
  }

  // The contents' clicks: open or close a branch, switch Guides and Reference.
  $('#docsTree').addEventListener('click', ev => {
    const a = ev.target.closest('[data-toggle]');
    if (!a) {
      return;
    }
    const key = a.dataset.toggle;
    const onTwisty = Boolean(ev.target.closest('.twisty'));
    const isCurrent = a.classList.contains('current');
    if (onTwisty || isCurrent) {
      ev.preventDefault();
      if (S.open.has(key)) {
        S.open.delete(key);
      } else {
        S.open.add(key);
      }
    } else {
      S.open.add(key);
    }
    storage.set('spot.docs.open', [...S.open].slice(-200));
    renderTree();
  });
  // Guides or Reference; the filter; ☰ hides the contents (a drawer on narrow screens).
  for (const b of $$('[data-docnav]')) {
    b.onclick = () => {
      S.nav = b.dataset.docnav;
      storage.set('spot.docs.nav', S.nav);
      renderTree();
    };
  }
  $('#docsFilter').addEventListener('input', debounce(renderTree, 120));
  $('#docsNavToggle').onclick = () => {
    if (window.matchMedia('(max-width: 860px)').matches) {
      page.classList.toggle('nav-open');
    } else {
      page.classList.toggle('nav-hidden');
    }
  };
  $('#docsTree').addEventListener('click', ev => {
    const narrow = window.matchMedia('(max-width: 860px)').matches;
    if (ev.target.closest('a:not([data-toggle])') && narrow) {
      page.classList.remove('nav-open');
    }
  });

  // Keys: / or Ctrl+K to search, Ctrl+F to find (not in the editor, which has its own).
  document.addEventListener('keydown', ev => {
    if (!S.shown) {
      return;
    }
    const typing = ev.target.closest('input, textarea, [contenteditable="true"], .cm-editor');
    const mod = ev.ctrlKey || ev.metaKey;
    const key = ev.key.toLowerCase();
    if ((ev.key === '/' && !typing) || (mod && key === 'k')) {
      ev.preventDefault();
      searchBox.focus();
      searchBox.select();
    } else if (mod && key === 'f' && !ev.target.closest('.cm-editor') && $('#docsEditor').hidden) {
      ev.preventDefault();
      openFind();
    } else if (ev.key === 'Escape' && !$('#docsFind').hidden && !typing) {
      closeFind();
    }
  });

  // The editor

  // Shows the editor in place of the page, or the page in place of the editor.
  function showEditor(on) {
    $('#docsEditor').hidden = !on;
    scroller.hidden = on;
    $('#docsFind').hidden = on || $('#docsFind').hidden;
    $('#docsFindBtn').hidden = on;
    if (on) {
      page.classList.remove('no-toc');
    }
  }

  // The editor's language support for a file's language.
  const editorLang = lang =>
    ({ makefile: 'shell', arduino: 'cpp', c: 'cpp', text: '' }[lang] ?? lang);

  /**
   * Opens a file in the editor, at a line.
   * @param {string} path The file (as in the index, or @docs/...).
   * @param {number} line The line to show (0: keep the place).
   * @param {{append: (string|undefined), heading: (string|undefined)}=} opts append: text to
   *     add at the end (a new note); heading: a "## heading" to go to (an existing note).
   */
  async function openEditor(path, line, opts = {}) {
    const cm = await loadCm();
    await loadMd().catch(() => null);
    if (!S.ix) {
      await index().catch(() => null);
    }
    showEditor(true);
    if (S.ed && S.ed.path === path) {
      renderEditorChrome();
      if (line) {
        gotoLine(line);
      }
      S.ed.view.focus();
      return;
    }
    if (S.ed && S.ed.dirty && !confirm(`Discard your changes to ${S.ed.path}?`)) {
      location.hash = docsHref('e', S.ed.path);
      return;
    }
    let file;
    try {
      file = await api(`/api/docs/file?path=${encodeURIComponent(path)}`);
    } catch (e) {
      toast(e.message, 'error');
      showEditor(false);
      history.back();
      return;
    }
    if (S.ed) {
      S.ed.view.destroy();
    }
    const lang = file.lang;
    const markId = S.markEntry;
    S.markEntry = null;
    const entry = markId && S.ix ? S.ix.entries[markId] : null;
    S.ed = {
      path, lang, sha: file.sha, saved: file.text, exists: file.exists, editable: file.editable,
      dirty: false, diags: [], checks: [], notes: [], preview: false,
      mark: entry && entry.file === path ? [entry.start, entry.end] : null,
    };
    const view = new cm.EditorView({
      parent: $('#docsEdHost'),
      state: cm.EditorState.create({ doc: file.text, extensions: editorExtensions(cm, lang) }),
    });
    S.ed.view = view;
    renderEditorChrome();
    if (opts.append) {
      const end = view.state.doc.length;
      const text = file.text;
      const sep = !end || text.endsWith('\n\n') ? '' : (text.endsWith('\n') ? '\n' : '\n\n');
      const insert = sep + opts.append;
      view.dispatch({ changes: { from: end, insert }, selection: { anchor: end + insert.length } });
      const last = view.state.doc.length;
      view.dispatch({ effects: cm.EditorView.scrollIntoView(last, { y: 'center' }) });
    } else if (opts.heading) {
      const lines = file.text.split('\n');
      const k = lines.findIndex(l => l.replace(/[`]/g, '').trim() === `## ${opts.heading}`);
      gotoLine(k >= 0 ? k + 2 : 1);
    } else if (line) {
      gotoLine(line);
    }
    view.focus();
  }

  /**
   * The editor's features: highlighting, folding, search, completion, linting, the line-width
   * ruler, Ctrl+S, and following changes.
   * @param {!Object} cm The CodeMirror bundle.
   * @param {string} lang The file's language.
   * @return {!Array} The extensions.
   */
  function editorExtensions(cm, lang) {
    const unit = ' '.repeat(INDENT[lang] || 4);
    const width = WIDTH[lang] ?? 0;
    const ext = [
      cm.lineNumbers(), cm.highlightActiveLineGutter(), cm.highlightSpecialChars(), cm.history(),
      cm.foldGutter(), cm.drawSelection(), cm.dropCursor(),
      cm.EditorState.allowMultipleSelections.of(true), cm.indentOnInput(),
      cm.syntaxHighlighting(cm.classHighlighter), cm.bracketMatching(), cm.closeBrackets(),
      cm.autocompletion(), cm.rectangularSelection(), cm.crosshairCursor(),
      cm.highlightActiveLine(), cm.highlightSelectionMatches(), cm.highlightTrailingWhitespace(),
      cm.search({ top: true }),
      cm.indentUnit.of(unit), cm.EditorState.tabSize.of(4), cm.lintGutter(),
      cm.keymap.of([
        // Ctrl+S (Cmd+S) saves.
        { key: 'Mod-s', run: () => {
          save();
          return true;
        }, preventDefault: true },
        ...cm.closeBracketsKeymap, ...cm.defaultKeymap, ...cm.searchKeymap, ...cm.historyKeymap,
        ...cm.foldKeymap, ...cm.completionKeymap, ...cm.lintKeymap, cm.indentWithTab,
      ]),
      cm.languageFor(editorLang(lang)),
      cm.linter(view => lint(view), { delay: LINT_DELAY }),
      cm.EditorView.updateListener.of(u => {
        if (u.docChanged) {
          setDirty(u.state.doc.toString() !== S.ed.saved);
          if (S.ed.preview) {
            renderPreview();
          }
        }
        if (u.selectionSet || u.docChanged) {
          const pos = u.state.selection.main.head;
          const ln = u.state.doc.lineAt(pos);
          $('#docsEdPos').textContent = `Line ${ln.number}, column ${pos - ln.from + 1}`;
        }
      }),
      cm.EditorView.theme({}, { dark: true }),
    ];
    if (width) {
      ext.push(rulerPlugin(cm, width));
    }
    if (S.ed && S.ed.mark) {
      ext.push(markPlugin(cm, S.ed.mark));
    }
    return ext;
  }

  // A faint line at the line-width limit (CODE_TEMPLATE.md: MATLAB and JavaScript 100, Python 99).
  function rulerPlugin(cm, col) {
    return cm.ViewPlugin.fromClass(class {
      constructor(view) {
        this.el = document.createElement('div');
        this.el.className = 'docs-ruler';
        view.scrollDOM.style.position = 'relative';
        view.scrollDOM.append(this.el);
        this.place(view);
      }

      // Moves the line when the editor's size or text changes.
      update(u) {
        if (u.geometryChanged || u.docChanged) {
          this.place(u.view);
        }
      }

      // Puts the line at the column, as tall as the text.
      place(view) {
        const pad = parseFloat(getComputedStyle(view.contentDOM).paddingLeft || 0);
        const left = view.contentDOM.offsetLeft + pad + col * view.defaultCharacterWidth;
        this.el.style.left = `${left}px`;
        this.el.style.height = `${view.contentDOM.offsetHeight}px`;
      }

      // Removes the line with the editor.
      destroy() {
        this.el.remove();
      }
    });
  }

  // Marks the function the editor was opened for.
  function markPlugin(cm, [first, last]) {
    const deco = cm.Decoration.line({ class: 'cm-entry-range' });
    return cm.ViewPlugin.fromClass(class {
      constructor(view) {
        this.decorations = this.build(view);
      }

      // Marks the lines again when the text or the view changes.
      update(u) {
        if (u.docChanged || u.viewportChanged) {
          this.decorations = this.build(u.view);
        }
      }

      // A line decoration on each line of the function.
      build(view) {
        const b = new cm.RangeSetBuilder();
        const doc = view.state.doc;
        for (let n = first; n <= Math.min(last, doc.lines); n++) {
          const ln = doc.line(n);
          b.add(ln.from, ln.from, deco);
        }
        return b.finish();
      }
    }, { decorations: v => v.decorations });
  }

  // Moves the cursor to a line (and column), in the middle of the editor.
  function gotoLine(line, col = 1) {
    const view = S.ed.view;
    const cm = S.lib.cm;
    const ln = view.state.doc.line(Math.max(1, Math.min(line, view.state.doc.lines)));
    const pos = Math.min(ln.to, ln.from + Math.max(0, (col || 1) - 1));
    view.dispatch({
      selection: { anchor: pos }, effects: cm.EditorView.scrollIntoView(pos, { y: 'center' }),
    });
    view.focus();
  }

  // The editor's bar: the file, whether it is saved, what can be done.
  function renderEditorChrome() {
    const ed = S.ed;
    const lang = ed.lang;
    $('#docsEdLang').textContent = LANG_NAMES[lang] || lang;
    $('#docsEdLang').dataset.lang = lang;
    $('#docsEdPath').textContent = ed.path.replace(/^@docs\//, 'docs/');
    $('#docsEdPreview').hidden = lang !== 'markdown';
    const save = $('#docsEdSave');
    save.disabled = !ed.editable;
    save.title = ed.editable ? 'Save to the project (Ctrl+S)'
      : 'Only the local console saves (SPOT in MATLAB). Download the file instead.';
    setDirty(ed.dirty);
    const readOnly = 'The lab console shows code read only: your changes are checked as you ' +
      'type, and Download keeps them. To save, open this file in the local console (SPOT in ' +
      'MATLAB).';
    const fresh = 'A new file: it is created when you save.';
    setBanner(ed.exists ? (ed.editable ? '' : readOnly) : fresh, 'info');
    const crumbs = [{ label: 'Editor' }, { label: ed.path.split('/').pop() }];
    const entry = S.ix ? Object.values(S.ix.files).find(f => f.path === ed.path) : null;
    setCrumbs(crumbs, entry ? `<a class="btn btn-sm" href="${docsHref('f', ed.path)}">Documentation</a>` : '');
    renderProblems();
  }

  // Whether the text differs from the file: the dot in the bar, and Revert.
  function setDirty(dirty) {
    if (!S.ed) {
      return;
    }
    S.ed.dirty = dirty;
    $('#docsEdDirty').hidden = !dirty;
    $('#docsEdRevert').disabled = !dirty;
  }

  // The bar under the editor's toolbar: a conflict, read-only, a new file ('' hides it).
  function setBanner(html, kind = '') {
    const b = $('#docsEdBanner');
    b.hidden = !html;
    b.className = `docs-banner ${kind}`;
    b.innerHTML = html;
  }

  // Checks the code: ESLint here for JavaScript, CodeMirror's parser for JSON, the server for
  // the rest. Returns CodeMirror's diagnostics.
  async function lint(view) {
    const ed = S.ed;
    if (!ed || ed.view !== view) {
      return [];
    }
    const text = view.state.doc.toString();
    let diags = [];
    let checks = [];
    let notes = [];
    try {
      if (ed.lang === 'javascript') {
        diags = await eslint(text, ed.path);
        checks = ['ESLint (CODE_TEMPLATE.md rules)'];
      } else if (ed.lang === 'json') {
        const found = S.lib.cm.jsonParseLinter()(view);
        checks = ['JSON'];
        S.ed.diags = found.map(d => {
          const ln = view.state.doc.lineAt(d.from);
          const col = d.from - ln.from + 1;
          return { severity: d.severity, message: d.message, line: ln.number, col };
        });
        S.ed.checks = checks;
        renderProblems();
        return found;
      } else if (['css', 'html', 'ini', 'text', 'makefile'].includes(ed.lang)) {
        notes = ['No checks for this kind of file.'];
      } else {
        const r = await api('/api/docs/lint', { path: ed.path, lang: ed.lang, text });
        diags = r.diagnostics;
        checks = r.checks;
        notes = r.notes;
      }
    } catch (e) {
      notes = [`The checks could not run: ${e.message}`];
    }
    if (S.ed !== ed || view.state.doc.toString() !== text) {
      return [];   // changed since: CodeMirror asks again
    }
    ed.diags = diags;
    ed.checks = checks;
    ed.notes = notes;
    renderProblems();
    return diags.map(d => toCm(view, d));
  }

  // A diagnostic (lines and columns from 1) as CodeMirror's (positions in the text).
  function toCm(view, d) {
    const doc = view.state.doc;
    const ln = doc.line(Math.max(1, Math.min(d.line, doc.lines)));
    const from = Math.min(ln.to, ln.from + Math.max(0, (d.col || 1) - 1));
    let to;
    if (d.end_col) {
      const eln = doc.line(Math.max(1, Math.min(d.end_line || d.line, doc.lines)));
      to = Math.min(eln.to, eln.from + Math.max(0, d.end_col - 1));
    } else {
      const word = /^[\w.$]+/.exec(doc.sliceString(from, ln.to));
      to = from + (word ? word[0].length : 1);
    }
    if (to <= from) {
      to = Math.min(ln.to, from + 1);
    }
    const source = d.source ? `${d.source}${d.code ? ` ${d.code}` : ''}` : (d.code || '');
    const severity = ['error', 'info'].includes(d.severity) ? d.severity : 'warning';
    return { from, to: Math.max(from, to), severity, message: d.message, source };
  }

  // The problems list (right) and the counts in the bar.
  function renderProblems() {
    const ed = S.ed;
    if (!ed) {
      return;
    }
    // How many problems of each severity.
    const count = sev => ed.diags.filter(d => d.severity === sev).length;
    const [e, w, i] = [count('error'), count('warning'), count('info')];
    $('#docsEdLint').innerHTML = (e || w || i)
      ? `${e ? `<span class="e">● ${e} ${e === 1 ? 'error' : 'errors'}</span>` : ''}` +
        `${w ? `<span class="w">● ${w} ${w === 1 ? 'warning' : 'warnings'}</span>` : ''}` +
        `${i ? `<span class="i">● ${i} ${i === 1 ? 'note' : 'notes'}</span>` : ''}`
      : (ed.checks.length ? '<span class="ok">✓ No problems</span>' : '');
    $('#docsEdChecks').textContent = ed.checks.length ? `Checked by ${ed.checks.join(', ')}` : '';
    const list = [...ed.diags].sort((a, b) => a.line - b.line || a.col - b.col).map(d =>
      `<div class="prob" data-pline="${d.line}" data-pcol="${d.col || 1}"><span class="sev ${d.severity}"></span>
        <div><div>${esc(d.message)}</div><div class="where">${d.line}:${d.col || 1}${d.source ? ` · ${esc(d.source)}` : ''}
        ${d.code ? ` ${esc(d.code)}` : ''}</div></div></div>`).join('');
    const notes = ed.notes.map(n => `<div>${esc(n)}</div>`).join('');
    $('#docsToc').innerHTML = `<h3>Problems</h3><div class="probs">${list || '<div class="muted">None found.</div>'}</div>
      <div class="checks">${ed.checks.length ? `Checks: ${esc(ed.checks.join(', '))}.` : ''}${notes}</div>`;
    page.classList.remove('no-toc');
  }

  /**
   * Checks JavaScript with ESLint: the coding standard's rules (CODE_TEMPLATE.md section 8.2,
   * Google style) and ESLint's recommended checks for real mistakes.
   * @param {string} text The code.
   * @param {string} path The file.
   * @return {!Promise<!Array<!Object>>} Diagnostics (lines and columns from 1).
   */
  async function eslint(text, path) {
    const lib = await loadEslint();
    if (!S.eslint) {
      S.eslint = new lib.Linter({ configType: 'flat' });
    }
    const config = [{
      plugins: { spot: SPOT_ESLINT_PLUGIN },
      languageOptions: {
        ecmaVersion: 2022, sourceType: 'module', globals: { ...lib.browserGlobals },
      },
      rules: ESLINT_RULES,
    }];
    const messages = S.eslint.verify(text, config, path.split('/').pop());
    return messages.map(m => ({
      line: m.line || 1, col: m.column || 1, end_line: m.endLine, end_col: m.endColumn,
      severity: m.fatal || m.severity === 2 ? 'error'
        : (m.ruleId === 'spot/jsdoc-style' ? 'info' : 'warning'),
      message: m.message, source: 'ESLint', code: m.ruleId || '',
    }));
  }

  // Saves the file (local console), unless it changed on disk since it was opened.
  async function save(force = false) {
    const ed = S.ed;
    if (!ed) {
      return;
    }
    if (!ed.editable) {
      toast('Only the local console saves files (SPOT in MATLAB). Use Download to keep your ' +
        'changes.', 'info');
      return;
    }
    const text = ed.view.state.doc.toString();
    try {
      const r = await api('/api/docs/save', { path: ed.path, text, sha: ed.sha, force });
      ed.sha = r.sha;
      ed.saved = text;
      ed.exists = true;
      setDirty(false);
      setBanner('');
      toast(`Saved ${ed.path.split('/').pop()}. ${r.note || ''}`, 'success');
      S.staleIndex = true;
    } catch (e) {
      if (e.details && e.details.conflict) {
        setBanner(`${esc(e.message)} <button type="button" class="btn btn-sm" data-ed="reload">Reload from Disk</button>
          <button type="button" class="btn btn-sm" data-ed="force">Save Anyway</button>`);
      } else {
        toast(e.message, 'error', 9000);
      }
    }
  }

  // The banner's buttons: reload the file from disk, or save over it.
  $('#docsEdBanner').addEventListener('click', ev => {
    const b = ev.target.closest('[data-ed]');
    if (!b || !S.ed) {
      return;
    }
    if (b.dataset.ed === 'force') {
      save(true);
    } else if (b.dataset.ed === 'reload') {
      reloadEditor();
    }
  });

  // Replaces the editor's text with the file on disk.
  async function reloadEditor() {
    const ed = S.ed;
    const file = await api(`/api/docs/file?path=${encodeURIComponent(ed.path)}`);
    ed.view.dispatch({ changes: { from: 0, to: ed.view.state.doc.length, insert: file.text } });
    ed.sha = file.sha;
    ed.saved = file.text;
    setDirty(false);
    setBanner('');
  }

  // The editor's buttons: Save, Revert (to the saved text), Download, Close (back to where it was
  // opened from), and the Markdown Preview.
  $('#docsEdSave').onclick = () => save();
  $('#docsEdRevert').onclick = () => {
    if (S.ed && S.ed.dirty && confirm('Go back to the saved version? Your changes will be lost.')) {
      const all = { from: 0, to: S.ed.view.state.doc.length, insert: S.ed.saved };
      S.ed.view.dispatch({ changes: all });
      setDirty(false);
    }
  };
  $('#docsEdDownload').onclick = () => {
    if (!S.ed) {
      return;
    }
    const blob = new Blob([S.ed.view.state.doc.toString()], { type: 'text/plain' });
    const a = document.createElement('a');
    a.href = URL.createObjectURL(blob);
    a.download = S.ed.path.split('/').pop();
    a.click();
    setTimeout(() => URL.revokeObjectURL(a.href), 1000);
  };
  $('#docsEdClose').onclick = () => {
    if (S.ed && S.ed.dirty && !confirm(`Close ${S.ed.path.split('/').pop()} without saving?`)) {
      return;
    }
    const path = S.ed ? S.ed.path : '';
    if (S.ed) {
      S.ed.view.destroy();
      S.ed = null;
    }
    const f = S.ix && S.ix.files[path];
    const back = S.beforeEditor && !S.beforeEditor.startsWith('#docs/e/') ? S.beforeEditor : null;
    S.beforeEditor = null;
    const home = path.startsWith('@docs/guides/') ? guideOf(path) : '#docs';
    location.hash = back || (f ? docsHref('f', path) : home);
  };
  $('#docsEdPreview').onclick = () => {
    S.ed.preview = !S.ed.preview;
    $('#docsEdPreviewPane').hidden = !S.ed.preview;
    $('#docsEdPreview').classList.toggle('btn-primary', S.ed.preview);
    if (S.ed.preview) {
      renderPreview();
    }
  };

  // The guide a docs/guides file is.
  function guideOf(path) {
    const p = S.ix && Object.values(S.ix.guides.pages).find(g => g.path === path);
    return p ? docsHref('g', p.id) : '#docs';
  }

  // The Markdown preview, beside the text.
  const renderPreview = debounce(() => {
    if (S.ed && S.ed.preview && S.marked) {
      $('#docsEdPreviewPane').innerHTML = md(S.ed.view.state.doc.toString());
    }
  }, PREVIEW_DELAY);

  // Opens the notes file of an entry's source file at the entry's section, adding the section
  // (and the file) if they aren't there yet.
  function openNote(e) {
    if (!e) {
      return;
    }
    const source = noteSource(e);
    const path = `@docs/notes/${source}.md`;
    const notes = S.ix.notes[source];
    const key = noteKey(e);
    const has = notes && (key ? key in notes.entries : Boolean(notes.file));
    if (has) {
      S.pendingOpen = { heading: key };
    } else {
      const intro = notes ? '' : `<!-- Notes on ${source}. Text before the first "## name" ` +
        'heading is about the file; each "## name" section is about the function of that name ' +
        'in the file. -->\n\n';
      S.pendingOpen = { append: key ? `${intro}## ${key}\n\n` : intro };
    }
    S.beforeEditor = location.hash;
    if (S.ed && S.ed.path === path) {
      S.pendingOpen = null;   // already open: keep its edits
    }
    location.hash = docsHref('e', path, '');
  }

  // The server saved a file (from this page or another): read the index again.
  function onSaved() {
    S.staleIndex = true;
    if (S.shown && $('#docsEditor').hidden) {
      route(true);
    }
  }

  return {
    // Shows the tab (and what the address asks for).
    show() {
      S.shown = true;
      if (location.hash === '#docs' || location.hash === '') {
        const last = storage.get('spot.docs.route', '#docs');
        if (last && last !== '#docs' && !last.startsWith('#docs/e/') && !S.routeKey) {
          history.replaceState(null, '', last);
        }
      }
      if (S.staleIndex && parseRoute(location.hash).kind !== 'e') {
        S.routeKey = null;
      }
      route();
    },
    hide() {
      S.shown = false;
      $('#docsSuggest').hidden = true;
    },
    onSaved,
    isDirty: () => Boolean(S.ed && S.ed.dirty),
  };
}

// ESLint's rules for the console's JavaScript: the coding standard (CODE_TEMPLATE.md, section
// 8.2) and ESLint's checks for real mistakes.
const ESLINT_RULES = {
  'no-undef': 'error', 'no-unused-vars': ['warn', { args: 'none', caughtErrors: 'none' }],
  'no-redeclare': 'error', 'no-dupe-keys': 'error', 'no-unreachable': 'warn',
  'no-const-assign': 'error', 'no-dupe-else-if': 'warn', 'no-duplicate-case': 'error',
  'no-self-assign': 'warn', 'no-empty': ['warn', { allowEmptyCatch: true }],
  'no-fallthrough': 'warn', 'use-isnan': 'error', 'valid-typeof': 'error',
  'no-cond-assign': ['warn', 'except-parens'], 'no-unsafe-negation': 'error',
  'no-sparse-arrays': 'warn', 'no-func-assign': 'error', 'no-import-assign': 'error',
  'getter-return': 'error', 'no-setter-return': 'error', 'no-loss-of-precision': 'warn',
  'no-unexpected-multiline': 'error', 'curly': ['warn', 'all'], 'no-var': 'warn',
  'prefer-const': ['warn', { destructuring: 'all' }], 'semi': ['warn', 'always'],
  'quotes': ['warn', 'single', { avoidEscape: true, allowTemplateLiterals: true }],
  'max-len': ['warn', {
    code: 100, ignoreUrls: true, ignoreTemplateLiterals: true, ignoreRegExpLiterals: true,
  }],
  'brace-style': ['warn', '1tbs', { allowSingleLine: true }], 'one-var': ['warn', 'never'],
  'operator-linebreak': ['warn', 'after', { overrides: { '?': 'before', ':': 'before' } }],
  'no-tabs': 'warn', 'no-trailing-spaces': 'warn', 'max-statements-per-line': ['warn', { max: 1 }],
  'no-multiple-empty-lines': ['warn', { max: 1 }],
  'spot/jsdoc': 'warn', 'spot/jsdoc-style': 'warn',
};

// The standard's own rules: a comment on every top-level function and class (JSDoc preferred).
const SPOT_ESLINT_PLUGIN = {
  rules: {
    'jsdoc': {
      meta: {
        type: 'suggestion',
        messages: { m: '{{name}} has no comment: describe it with JSDoc (/** ... */).' },
      },
      // Reports a top-level function or class with no comment.
      create(context) {
        return topLevelDocs(context, (comment, node, name) => {
          if (!comment) {
            context.report({ node, messageId: 'm', data: { name } });
          }
        });
      },
    },
    'jsdoc-style': {
      meta: {
        type: 'suggestion',
        messages: { m: 'The standard asks for JSDoc (/** ... */) on top-level functions.' },
      },
      // Reports one whose comment isn't JSDoc.
      create(context) {
        return topLevelDocs(context, (comment, node) => {
          if (comment && !(comment.type === 'Block' && comment.value.startsWith('*'))) {
            context.report({ node, messageId: 'm' });
          }
        });
      },
    },
  },
};

/**
 * Calls check(comment, node, name) for each top-level function or class, with the comment just
 * above it (null if none).
 * @param {!Object} context ESLint's rule context.
 * @param {function(?Object, !Object, string)} check What to do with each.
 * @return {!Object} The rule's visitors.
 */
function topLevelDocs(context, check) {
  const sc = context.sourceCode;
  // The comment that ends on the line above a declaration (at: the export, if any).
  const visit = (node, at, name) => {
    const before = sc.getCommentsBefore(at);
    const last = before[before.length - 1];
    const touching = last && last.loc.end.line >= at.loc.start.line - 1 ? last : null;
    check(touching, node, name);
  };
  // ESLint calls these for each top-level function and class, exported or not.
  return {
    'Program > FunctionDeclaration'(n) {
      visit(n, n, n.id.name);
    },
    'Program > ExportNamedDeclaration > FunctionDeclaration'(n) {
      visit(n, n.parent, n.id.name);
    },
    'Program > ClassDeclaration'(n) {
      visit(n, n, n.id.name);
    },
    'Program > ExportNamedDeclaration > ClassDeclaration'(n) {
      visit(n, n.parent, n.id.name);
    },
  };
}
