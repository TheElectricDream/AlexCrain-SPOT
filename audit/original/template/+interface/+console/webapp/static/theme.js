// The console's colour themes. The page's own look is "default" (app.css and docs.css as they
// are). Any other theme is listed in themes/themes.json and has a stylesheet of its own rules,
// themes/<theme>.css; the server makes the page's theme stylesheet from them
// (spot_console/themes.py, /theme/<theme>.css), and index.html loads it before the page is drawn.
//
// The choice is kept in a cookie, not in the page's storage: the local console is on a different
// port every time MATLAB starts it, and storage is kept per port, but a cookie is kept for the
// computer. ?theme=<theme> in the address shows a theme for one visit without choosing it.
//
// The colours the scripts draw with (the 3D scene, the plots) are the theme's too: a theme sets
// them as custom properties (--scene-bg, --plot-text, ...) and the scripts read them here.

const COOKIE = 'spot_theme';
const YEAR = 365 * 24 * 3600;   // [s] how long the choice is kept
const DEFAULT = { id: 'default', name: 'Default' };

const root = document.documentElement;
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));

/**
 * The theme this page shows.
 * @return {string} Its id: 'default', or a key of themes.json.
 */
export const themeId = () => root.dataset.theme || DEFAULT.id;

/**
 * A colour the scripts draw with, as the theme sets it.
 * @param {string} name The custom property, without its dashes ('scene-bg').
 * @param {string} fallback The page's own colour, used when the theme doesn't set one.
 * @return {string} The colour, as CSS.
 */
export function themeColor(name, fallback) {
  return getComputedStyle(root).getPropertyValue('--' + name).trim() || fallback;
}

/**
 * Keeps a choice of theme for this computer, and shows it (the page is loaded again).
 * @param {string} id The theme's id; 'default' for the page's own look.
 */
export function chooseTheme(id) {
  // The cookie; the page's own look needs none.
  document.cookie = id === DEFAULT.id
    ? `${COOKIE}=; Path=/; Max-Age=0; SameSite=Lax`
    : `${COOKIE}=${id}; Path=/; Max-Age=${YEAR}; SameSite=Lax`;

  // Load the page again, without a ?theme= that would override the choice.
  const url = new URL(location.href);
  url.searchParams.delete('theme');
  if (url.href === location.href) {
    location.reload();
  } else {
    location.replace(url.href);
  }
}

/**
 * Sets up the Theme menu in the top bar: the page's own look and every theme in themes.json.
 * @return {!Promise<void>} Resolved when the menu is filled.
 */
export async function initThemeMenu() {
  const btn = document.querySelector('#themeBtn');
  const menu = document.querySelector('#themeMenu');
  if (!btn || !menu) {
    return;
  }

  // The themes; without the list there is only the page's own look.
  let themes = [DEFAULT];
  try {
    const found = await (await fetch('/static/themes/themes.json', { cache: 'no-cache' })).json();
    themes = themes.concat(Object.entries(found).map(([id, t]) =>
      ({ id, name: t.name || id, about: t.about || '' })));
  } catch {
    // No list: the menu offers the page's own look only.
  }

  // A remembered theme that no longer exists: back to the page's own look.
  if (!themes.some(t => t.id === themeId())) {
    document.cookie = `${COOKIE}=; Path=/; Max-Age=0; SameSite=Lax`;
    delete root.dataset.theme;
  }

  // The menu: one entry a theme, the one shown ticked.
  menu.innerHTML = '<span class="view-opts-head">Theme</span>' + themes.map(t =>
    `<button type="button" role="menuitemradio" data-theme-id="${esc(t.id)}"
      aria-checked="${t.id === themeId()}" title="${esc(t.about || '')}">
      <span class="theme-tick" aria-hidden="true">✓</span>${esc(t.name)}</button>`).join('');
  menu.addEventListener('click', ev => {
    const pick = ev.target.closest('[data-theme-id]');
    if (pick && pick.dataset.themeId !== themeId()) {
      chooseTheme(pick.dataset.themeId);
    }
  });

  // Opening and closing it (a click elsewhere, or Esc, closes it).
  const show = open => {
    menu.hidden = !open;
    btn.setAttribute('aria-expanded', String(open));
  };
  btn.onclick = () => show(menu.hidden);
  document.addEventListener('pointerdown', ev => {
    if (!menu.hidden && !ev.target.closest('#themeMenu, #themeBtn')) {
      show(false);
    }
  });
  document.addEventListener('keydown', ev => {
    if (ev.key === 'Escape' && !menu.hidden) {
      show(false);
    }
  });
}
