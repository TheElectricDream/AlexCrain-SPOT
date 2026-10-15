// Messages panel (local console): each platform's debug messages, the text the diagram writes to
// RED_Logger / BLACK_Logger / BLUE_Logger (e.g. "Chaser: Running Phase 1"). One panel, moved
// into the page shown like the 3D view, split into the Chaser (RED), Target (BLACK) and
// Obstacle (BLUE).
//   Simulation tab: the messages of the last simulation (recorded by
//                   spot.hardware.debugMessages while it ran, shown live during the run); the
//                   playback highlights the one in effect at its time, so a simulation shows
//                   what an experiment will show.
//   Experiment tab: the platforms' messages as the lab console receives them (UDP
//                   55001-55003).
// Only changes are listed (the platforms repeat the current message), with how often it came.

import { PLATFORM_KEYS as PLATFORMS, COLORS as SWATCH } from './platforms.js';

const MAX_ROWS = 500;
const STALE_AFTER = 3;   // [s] a platform silent this long is shown as stale
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
const $ = s => document.querySelector(s);

/**
 * Sets up the Messages panel.
 * @param {{storage: {get: function(string, *): *, set: function(string, *)}}} deps The page's
 *     settings storage (whether the panel is tucked away).
 * @return {!Object} What the pages call: show, setSelected, simStarted, simMessages,
 *     simResult, simTime, experiment.
 */
export function initMessages({ storage }) {
  const panel = $('#msgPanel');
  const S = {
    mode: 'simulation',
    sim: emptyLog(), simLive: false, simT: null,
    exp: emptyLog(), expRunning: false,
    selected: { RED: true, BLACK: true, BLUE: true },
  };

  // An empty message list per platform.
  function emptyLog() {
    return { RED: [], BLACK: [], BLUE: [] };
  }

  // Tucking it away

  // Collapse or expand the panel (and its slots), remembered; the 3D view takes the room.
  function setCollapsed(on) {
    panel.classList.toggle('collapsed', on);
    for (const slot of document.querySelectorAll('[data-msg-slot]')) {
      slot.classList.toggle('collapsed', on);
    }
    $('#msgToggle').setAttribute('aria-expanded', String(!on));
    $('#msgToggle').textContent = on ? '‹' : '›';
    $('#msgToggle').title = on ? 'Show the platforms\' messages'
      : 'Tuck the messages away (more room for the 3D view)';
    storage.set('spot.msgCollapsed', on);
    window.dispatchEvent(new Event('resize'));
  }
  $('#msgToggle').onclick = () => setCollapsed(!panel.classList.contains('collapsed'));
  setCollapsed(storage.get('spot.msgCollapsed', false));

  // Data in

  // spot.sim.results' debug: { NAME: { t: [...], text: [...] } } (MATLAB may unwrap one entry),
  // as each platform's rows.
  function simEntries(debug) {
    const out = emptyLog();
    for (const n of PLATFORMS) {
      const d = (debug || {})[n];
      if (!d) {
        continue;
      }
      const t = list(d.t);
      const text = list(d.text);
      out[n] = t.map((tk, k) => ({ t: tk, text: text[k], count: 1 }));
    }
    return out;
  }

  const api = {
    // The page shown: 'simulation' or 'experiment'; the panel moves into its slot.
    show(mode) {
      S.mode = mode;
      const slot = document.querySelector(`[data-msg-slot="${mode}"]`);
      if (slot && panel.parentElement !== slot) {
        slot.append(panel);
      }
      render();
    },
    // The settings' platforms: the others are shown as not taking part.
    setSelected(platforms) {
      S.selected = {
        RED: Boolean(platforms && platforms.RED), BLACK: Boolean(platforms && platforms.BLACK),
        BLUE: Boolean(platforms && platforms.BLUE),
      };
      render();
    },
    // A new simulation: its messages arrive while it runs.
    simStarted() {
      S.sim = emptyLog();
      S.simLive = true;
      S.simT = null;
      if (S.mode === 'simulation') {
        render();
      }
    },
    // New messages during a simulation: [{ platform, t, text }].
    simMessages(messages) {
      for (const m of list(messages)) {
        const rows = S.sim[m.platform];
        if (rows) {
          rows.push({ t: m.t, text: m.text, count: 1 });
        }
      }
      if (S.mode === 'simulation') {
        render();
      }
    },
    // The finished simulation's full record.
    simResult(debug) {
      S.sim = simEntries(debug);
      S.simLive = false;
      if (S.mode === 'simulation') {
        render();
      }
    },
    // Playback time: highlight the message in effect.
    simTime(t) {
      S.simT = t;
      if (S.mode === 'simulation') {
        highlight();
      }
    },
    // The lab console's experiment status (debug: { NAME: { entries, seen } }).
    experiment(exp) {
      const d = (exp && exp.debug) || {};
      for (const n of PLATFORMS) {
        S.exp[n] = list(d[n] && d[n].entries);
      }
      S.expSeen = Object.fromEntries(PLATFORMS.map(n => [n, d[n] ? d[n].seen : null]));
      S.expRunning = Boolean(exp && ['starting', 'running'].includes(exp.phase));
      if (S.mode === 'experiment') {
        render();
      }
    },
  };

  // Drawing

  const fmtT = t => (t == null || !isFinite(t) ? '' : `${Number(t).toFixed(2)} s`);
  const fmtClock = sec => new Date(sec * 1000).toLocaleTimeString();

  // Every platform's list, its latest message and (live) whether it has gone silent.
  function render() {
    const sim = S.mode === 'simulation';
    const log = sim ? S.sim : S.exp;
    $('#msgSource').textContent = sim
      ? (S.simLive ? 'simulation running' : 'last simulation')
      : (S.expRunning ? 'live from the platforms' : 'last experiment');
    let total = 0;
    for (const n of PLATFORMS) {
      const sec = panel.querySelector(`[data-plat="${n}"]`);
      const rows = log[n].slice(-MAX_ROWS);
      total += rows.length;
      const listEl = sec.querySelector('.msg-list');
      const atBottom = listEl.scrollHeight - listEl.scrollTop - listEl.clientHeight < 30;
      sec.classList.toggle('off', !S.selected[n] && !rows.length);

      // The list: why it is empty, or each message with its time and repeat count.
      if (!rows.length) {
        listEl.innerHTML = `<li class="msg-empty">${!S.selected[n] ? 'Not in this run.'
          : sim ? (S.simLive ? 'No message yet.' : 'No messages: run a simulation.')
          : 'No messages yet. They appear here while the experiment runs.'}</li>`;
      } else {
        listEl.innerHTML = rows.map((r, k) => {
          const when = r.t != null && isFinite(r.t) ? fmtT(r.t) : r.first ? fmtClock(r.first) : '';
          const count = r.count > 1
            ? `<span class="msg-count" title="Received ${r.count} times in a row">` +
              `×${r.count}</span>`
            : '';
          return `<li data-k="${k}" data-t="${r.t == null ? '' : r.t}">` +
            `<span class="msg-t mono">${esc(when)}</span>` +
            `<span class="msg-text">${esc(r.text)}</span>${count}</li>`;
        }).join('');
      }

      // The latest message.
      const last = rows[rows.length - 1];
      const now = sec.querySelector('.msg-now');
      now.textContent = last ? last.text : '';
      now.title = last ? last.text : '';

      // Live: silent for more than STALE_AFTER s.
      if (!sim && S.expSeen && S.expSeen[n] != null && S.expRunning) {
        sec.classList.toggle('stale', S.expSeen[n] > STALE_AFTER);
        sec.querySelector('.msg-seen').textContent = S.expSeen[n] > STALE_AFTER
          ? `silent for ${Math.round(S.expSeen[n])} s` : '';
      } else {
        sec.classList.remove('stale');
        sec.querySelector('.msg-seen').textContent = '';
      }
      if (atBottom || S.mode === 'experiment') {
        listEl.scrollTop = listEl.scrollHeight;
      }
    }
    $('#msgCount').textContent = total ? String(total) : '';

    // The message in effect: at the playback time, or (live) the newest.
    if (sim) {
      highlight();
    } else if (S.expRunning) {
      for (const ol of panel.querySelectorAll('.msg-list')) {
        const last = ol.lastElementChild;
        if (last && last.dataset.k !== undefined) {
          last.classList.add('current');
        }
      }
    }
  }

  // Simulation playback: the message in effect at the playback time, earlier ones normal,
  // later ones faint; scrolled into view.
  function highlight() {
    const t = S.simT;
    for (const n of PLATFORMS) {
      const items = panel.querySelectorAll(`[data-plat="${n}"] .msg-list li[data-k]`);
      let current = null;
      for (const li of items) {
        const tk = parseFloat(li.dataset.t);
        const future = t != null && !S.simLive && tk > t + 1e-9;
        li.classList.toggle('future', future);
        li.classList.remove('current');
        if (!future) {
          current = li;
        }
      }
      if (current && t != null && !S.simLive) {
        current.classList.add('current');
        const listEl = current.parentElement;
        const top = current.offsetTop - listEl.offsetTop;
        if (top < listEl.scrollTop ||
            top > listEl.scrollTop + listEl.clientHeight - current.offsetHeight) {
          listEl.scrollTop = top - listEl.clientHeight / 2;
        }
        const now = panel.querySelector(`[data-plat="${n}"] .msg-now`);
        now.textContent = current.querySelector('.msg-text').textContent;
      }
    }
  }

  // Each platform's swatch.
  for (const n of PLATFORMS) {
    panel.querySelector(`[data-plat="${n}"] .swatch`).style.background = SWATCH[n];
  }
  render();
  return api;
}
