// Lab Setup (local console): the deployment scripts of +interface/+console/deploy, run from the
// page (spot_console/deploy.py). A small button next to Lab Console opens a dialog with each one:
// what it does, what to mind, Run; what the script prints follows below as it runs. Only on the
// lab network: the button is there but disabled, and the server refuses, when the lab console
// isn't reachable.

const $ = s => document.querySelector(s);
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const BOARD_NAMES = { red: 'Chaser', black: 'Target', blue: 'Obstacle' };
const LOG_LINES = 2000;   // kept in the dialog's log

// A time of day (hh:mm) from a POSIX time; '' for none.
const when = t => (t
  ? new Date(t * 1000).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
  : '');

/**
 * Sets up the Lab Setup button and dialog.
 * @param {{api: function(string, !Object): !Promise, act: function(function(): !Promise),
 *     toast: function(string, string, number)}} deps The page's API call, its action
 *     wrapper (reports errors) and its toast.
 * @return {!Object} Handlers for the page's events: onState, onLab, onStatus, onLog.
 */
export function initLabSetup({ api, act, toast }) {
  const S = {
    status: null, reachable: false, local: false,
    boards: new Set(['red', 'black', 'blue']), lines: [],
  };
  const btn = $('#labSetupBtn');
  const dialog = $('#labSetupDialog');
  const log = $('#labSetupLog');

  // The topbar button: local console only; enabled on the lab network.
  function renderButton() {
    btn.hidden = !S.local;
    btn.disabled = !S.reachable;
    btn.title = S.reachable
      ? 'Deploy the lab\'s software from this computer: the lab console (TX2), ' +
        'SPOT Vision (Orin), the platforms\' links'
      : 'Lab Setup needs the lab network (the lab console isn\'t reachable)';
    btn.classList.toggle('running', Boolean(S.status && S.status.running));
  }

  // The dialog: why nothing can run (if so), and each action with its last result.
  function render() {
    renderButton();
    const st = S.status;
    if (!st) {
      return;
    }
    const busy = st.running;
    const problem = st.problem || (!S.reachable
      ? 'The lab console isn\'t reachable: connect to the lab network.' : '');
    $('#labSetupProblem').hidden = !problem;
    $('#labSetupProblem').textContent = problem;
    $('#labSetupActions').innerHTML = st.actions.map(a => {
      // Its last run: running, done or failed.
      const last = st.last && st.last.action === a.id ? st.last : null;
      const state = !last ? '' : last.finished == null ? '<span class="pill live">Running</span>'
        : last.ok ? `<span class="pill ok" title="Finished at ${when(last.finished)}">` +
          `Done ${when(last.finished)}</span>`
        : `<span class="pill err" title="Exit code ${last.code}">` +
          `Failed ${when(last.finished)}</span>`;

      // The platforms to link (that action only).
      const boards = a.id === 'platforms' ? `<div class="ls-boards">${st.boards.map(b => `<label class="ls-board"><input type="checkbox" data-board="${b}"
          ${S.boards.has(b) ? 'checked' : ''} ${busy ? 'disabled' : ''}>${BOARD_NAMES[b] || b}</label>`).join('')}</div>` : '';
      return `<div class="ls-action">
          <div class="ls-head"><b>${esc(a.title)}</b><span class="mono ls-target">${esc(a.target)} · about ${a.minutes} min</span>${state}</div>
          <p class="hint">${esc(a.what)}</p>
          <p class="hint warn-text">${esc(a.careful)}</p>
          <div class="ls-run">${boards}<span class="vision-spacer"></span>
            <button type="button" class="btn btn-sm btn-primary" data-run="${a.id}" ${busy || problem ? 'disabled' : ''}
              title="Runs +interface/+console/deploy/${esc(a.script)}">Run</button></div>
        </div>`;
    }).join('');
    $('#labSetupCancel').hidden = !busy;
  }

  // Adds lines to the log, keeping the newest LOG_LINES; stays at the bottom if it was there.
  function appendLines(lines) {
    if (!lines.length) {
      return;
    }
    const stick = log.scrollTop + log.clientHeight >= log.scrollHeight - 20;
    S.lines.push(...lines);
    if (S.lines.length > LOG_LINES) {
      S.lines.splice(0, S.lines.length - LOG_LINES);
    }
    log.textContent = S.lines.join('\n');
    if (stick) {
      log.scrollTop = log.scrollHeight;
    }
  }

  // Open the dialog at the log's end.
  btn.onclick = () => {
    render();
    dialog.showModal();
    log.scrollTop = log.scrollHeight;
  };

  // The platforms chosen.
  $('#labSetupActions').addEventListener('change', ev => {
    const b = ev.target.dataset.board;
    if (b) {
      if (ev.target.checked) {
        S.boards.add(b);
      } else {
        S.boards.delete(b);
      }
    }
  });

  // Run: confirm, then start it.
  $('#labSetupActions').addEventListener('click', ev => act(async () => {
    const b = ev.target.closest('[data-run]');
    if (!b || b.disabled) {
      return;
    }
    const a = S.status.actions.find(x => x.id === b.dataset.run);
    if (!confirm(`${a.title}?\n\n${a.what}\n\n${a.careful}`)) {
      return;
    }
    S.status = await api('/api/deploy/run', { action: a.id, boards: [...S.boards] });
    render();
  }));

  // Cancel the running script; clear the log.
  $('#labSetupCancel').onclick = () => act(async () => {
    S.status = await api('/api/deploy/cancel', {});
    render();
  });
  $('#labSetupClear').onclick = () => {
    S.lines = [];
    log.textContent = '';
  };

  return {
    // The page's state: local console?, lab reachable?, the scripts and their log.
    onState(st) {
      S.local = st.features && st.features.mode === 'local';
      S.reachable = Boolean(st.lab && st.lab.reachable);
      S.status = st.deploy || null;
      S.lines = ((st.logs && st.logs.deploy) || []).map(e => e.line);
      log.textContent = S.lines.join('\n');
      render();
    },
    // The lab link changed.
    onLab(l) {
      S.reachable = Boolean(l && l.reachable);
      render();
    },
    // The scripts' status; a toast when one ends.
    onStatus(d) {
      const before = S.status && S.status.running;
      S.status = d;
      render();
      if (before && !d.running && d.last) {
        toast(`${d.last.title}: ` +
          `${d.last.ok ? 'done' : `failed (code ${d.last.code}); its output is in Lab Setup`}.`,
          d.last.ok ? 'success' : 'error', 7000);
      }
    },
    // A line of a script's output.
    onLog(e) {
      appendLines([e.line]);
    },
  };
}
