// Messages panel (local console): each platform's debug messages, the text the diagram writes to
// RED_Logger / BLACK_Logger / BLUE_Logger (e.g. "Chaser: Running Phase 1"). One panel, moved into
// the page shown like the 3D view, split into the Chaser (RED), Target (BLACK) and Obstacle (BLUE).
//   Simulation tab: the messages of the last simulation (recorded by spot.hardware.debugMessages
//                   while it ran, shown live during the run); the playback highlights the one
//                   in effect at its time, so a simulation shows what an experiment will show.
//   Experiment tab: the platforms' messages as the lab console receives them (UDP 55001-55003).
// Only changes are listed (the platforms repeat the current message), with how often it came.

import { PLATFORM_KEYS as PLATFORMS, COLORS as SWATCH } from './platforms.js';

const MAX_ROWS = 500;
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const list = x => (Array.isArray(x) ? x : x == null ? [] : [x]);
const $ = s => document.querySelector(s);

export function initMessages({ storage }) {
  const panel = $('#msgPanel');
  const S = {
    mode: 'simulation',
    sim: emptyLog(), simLive: false, simT: null,
    exp: emptyLog(), expRunning: false,
    selected: { RED: true, BLACK: true, BLUE: true },
  };
  function emptyLog() { return { RED: [], BLACK: [], BLUE: [] }; }

  // ---------------------------------------------------------------- tucking it away
  function setCollapsed(on) {
    panel.classList.toggle('collapsed', on);
    for (const slot of document.querySelectorAll('[data-msg-slot]')) slot.classList.toggle('collapsed', on);
    $('#msgToggle').setAttribute('aria-expanded', String(!on));
    $('#msgToggle').textContent = on ? '‹' : '›';
    $('#msgToggle').title = on ? 'Show the platforms\' messages' : 'Tuck the messages away (more room for the 3D view)';
    storage.set('spot.msgCollapsed', on);
    window.dispatchEvent(new Event('resize'));   // the 3D view takes the room
  }
  $('#msgToggle').onclick = () => setCollapsed(!panel.classList.contains('collapsed'));
  setCollapsed(storage.get('spot.msgCollapsed', false));

  // ---------------------------------------------------------------- data in
  function simEntries(debug) {
    // spot.sim.results' debug: { NAME: { t: [...], text: [...] } } (MATLAB may unwrap one entry)
    const out = emptyLog();
    for (const n of PLATFORMS) {
      const d = (debug || {})[n];
      if (!d) continue;
      const t = list(d.t), text = list(d.text);
      out[n] = t.map((tk, k) => ({ t: tk, text: text[k], count: 1 }));
    }
    return out;
  }

  const api = {
    show(mode) {
      // The page shown: 'simulation' or 'experiment'
      S.mode = mode;
      const slot = document.querySelector(`[data-msg-slot="${mode}"]`);
      if (slot && panel.parentElement !== slot) slot.append(panel);
      render();
    },
    setSelected(platforms) {
      // The settings' platforms: the others are shown as not taking part
      S.selected = { RED: Boolean(platforms && platforms.RED), BLACK: Boolean(platforms && platforms.BLACK),
                     BLUE: Boolean(platforms && platforms.BLUE) };
      render();
    },
    simStarted() {
      // A new simulation: its messages arrive while it runs
      S.sim = emptyLog();
      S.simLive = true;
      S.simT = null;
      if (S.mode === 'simulation') render();
    },
    simMessages(messages) {
      // New messages during a simulation: [{ platform, t, text }]
      for (const m of list(messages)) {
        const rows = S.sim[m.platform];
        if (rows) rows.push({ t: m.t, text: m.text, count: 1 });
      }
      if (S.mode === 'simulation') render();
    },
    simResult(debug) {
      // The finished simulation's full record
      S.sim = simEntries(debug);
      S.simLive = false;
      if (S.mode === 'simulation') render();
    },
    simTime(t) {
      // Playback time: highlight the message in effect
      S.simT = t;
      if (S.mode === 'simulation') highlight();
    },
    experiment(exp) {
      // The lab console's experiment status (debug: { NAME: { entries, seen } })
      const d = (exp && exp.debug) || {};
      for (const n of PLATFORMS) S.exp[n] = list(d[n] && d[n].entries);
      S.expSeen = Object.fromEntries(PLATFORMS.map(n => [n, d[n] ? d[n].seen : null]));
      S.expRunning = Boolean(exp && ['starting', 'running'].includes(exp.phase));
      if (S.mode === 'experiment') render();
    },
  };

  // ---------------------------------------------------------------- drawing
  const fmtT = t => (t == null || !isFinite(t) ? '' : `${Number(t).toFixed(2)} s`);
  const fmtClock = sec => new Date(sec * 1000).toLocaleTimeString();

  function render() {
    const sim = S.mode === 'simulation';
    const log = sim ? S.sim : S.exp;
    $('#msgSource').textContent = sim ? (S.simLive ? 'simulation running' : 'last simulation') : (S.expRunning ? 'live from the platforms' : 'last experiment');
    let total = 0;
    for (const n of PLATFORMS) {
      const sec = panel.querySelector(`[data-plat="${n}"]`);
      const rows = log[n].slice(-MAX_ROWS);
      total += rows.length;
      const listEl = sec.querySelector('.msg-list');
      const atBottom = listEl.scrollHeight - listEl.scrollTop - listEl.clientHeight < 30;
      sec.classList.toggle('off', !S.selected[n] && !rows.length);
      if (!rows.length) {
        listEl.innerHTML = `<li class="msg-empty">${!S.selected[n] ? 'Not in this run.'
          : sim ? (S.simLive ? 'No message yet.' : 'No messages: run a simulation.')
          : 'No messages yet. They appear here while the experiment runs.'}</li>`;
      } else {
        listEl.innerHTML = rows.map((r, k) => {
          const when = r.t != null && isFinite(r.t) ? fmtT(r.t) : r.first ? fmtClock(r.first) : '';
          const count = r.count > 1 ? `<span class="msg-count" title="Received ${r.count} times in a row">×${r.count}</span>` : '';
          return `<li data-k="${k}" data-t="${r.t == null ? '' : r.t}"><span class="msg-t mono">${esc(when)}</span><span class="msg-text">${esc(r.text)}</span>${count}</li>`;
        }).join('');
      }
      const last = rows[rows.length - 1];
      const now = sec.querySelector('.msg-now');
      now.textContent = last ? last.text : '';
      now.title = last ? last.text : '';
      if (!sim && S.expSeen && S.expSeen[n] != null && S.expRunning) {
        sec.classList.toggle('stale', S.expSeen[n] > 3);
        sec.querySelector('.msg-seen').textContent = S.expSeen[n] > 3 ? `silent for ${Math.round(S.expSeen[n])} s` : '';
      } else {
        sec.classList.remove('stale');
        sec.querySelector('.msg-seen').textContent = '';
      }
      if (atBottom || S.mode === 'experiment') listEl.scrollTop = listEl.scrollHeight;
    }
    $('#msgCount').textContent = total ? String(total) : '';
    if (sim) highlight();
    else if (S.expRunning) {   // live: the newest message is the one in effect
      for (const ol of panel.querySelectorAll('.msg-list')) {
        const last = ol.lastElementChild;
        if (last && last.dataset.k !== undefined) last.classList.add('current');
      }
    }
  }

  function highlight() {
    // Simulation playback: the message in effect at the playback time, earlier ones normal,
    // later ones faint
    const t = S.simT;
    for (const n of PLATFORMS) {
      const items = panel.querySelectorAll(`[data-plat="${n}"] .msg-list li[data-k]`);
      let current = null;
      for (const li of items) {
        const tk = parseFloat(li.dataset.t);
        const future = t != null && !S.simLive && tk > t + 1e-9;
        li.classList.toggle('future', future);
        li.classList.remove('current');
        if (!future) current = li;
      }
      if (current && t != null && !S.simLive) {
        current.classList.add('current');
        const listEl = current.parentElement;
        const top = current.offsetTop - listEl.offsetTop;
        if (top < listEl.scrollTop || top > listEl.scrollTop + listEl.clientHeight - current.offsetHeight) {
          listEl.scrollTop = top - listEl.clientHeight / 2;
        }
        const now = panel.querySelector(`[data-plat="${n}"] .msg-now`);
        now.textContent = current.querySelector('.msg-text').textContent;
      }
    }
  }

  for (const n of PLATFORMS) {
    panel.querySelector(`[data-plat="${n}"] .swatch`).style.background = SWATCH[n];
  }
  render();
  return api;
}
