// The Vision tab (lab console): the cameras plugged into the Jetson Orin, streamed by its SPOT
// Vision service (+spot/+console/vision/spot_vision.py). A tab per camera, one stream at a time;
// on the left, the chosen camera's settings, as its own library offers them (the Orin lists
// them, with their ranges and defaults). This console says where the Orin is and passes the
// settings on (/api/vision, /api/vision/params); the video comes from the Orin straight to this
// page (MJPEG in an <img>), never through the TX2. A camera is opened only while it streams (or
// a snapshot is taken), and closed a few seconds after, so experiments can have it.

const $ = s => document.querySelector(s);
const esc = s => String(s == null ? '' : s).replace(/[&<>"]/g, c =>
  ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
const KIND = {
  zed: 'Stereo Camera', realsense: 'LiDAR', dvs: 'Event Camera', uvc: 'USB Camera',
  demo: 'Test Pattern',
};
const SIZES = [['640', 'Small'], ['960', 'Medium'], ['0', 'Full']];
const RATES = [5, 10, 15, 30];
const POLL = 3000;   // [ms] the camera list, while the tab is shown

/**
 * Sets up the Vision tab.
 * @param {{api: function(string, !Object=): !Promise, act: function(function(): !Promise),
 *     toast: function(string, string, number),
 *     storage: {get: function(string, *): *, set: function(string, *)}}} deps The page's API
 *     call, its action wrapper (reports errors), its toast and its settings storage.
 * @return {{show: function(), hide: function()}} The tab shown or left.
 */
export function initVision({ api, act, toast, storage }) {
  const S = {
    shown: false, status: null, timer: 0,
    selected: storage.get('spot.vision.selected', null),
    prefs: storage.get('spot.vision', {}),   // camera id -> {view, fps, size}
    live: false,   // streaming: switching tabs keeps it going on the new camera
    failed: null,   // why the selected camera's stream stopped
    // camera id -> its settings (the Orin's list), and which is drawn
    params: {}, paramsFor: null,
  };
  const img = $('#visionImg');

  // The cameras, the one chosen, and its view, rate and size (remembered per camera).
  const cameras = () => (S.status && S.status.cameras) || [];
  const current = () => cameras().find(c => c.id === S.selected) || cameras()[0] || null;
  const prefs = cam => {
    const p = S.prefs[cam.id] || {};
    const views = cam.views.map(v => v.id);
    return {
      view: views.includes(p.view) ? p.view : views[0], fps: p.fps || 15,
      size: p.size || '960',
    };
  };
  const setPref = (id, k, v) => {
    S.prefs[id] = { ...(S.prefs[id] || {}), [k]: v };
    storage.set('spot.vision', S.prefs);
  };

  // The stream's URL (the time makes each one new).
  const streamUrl = (cam, p) =>
    `${S.status.url}/stream/${encodeURIComponent(cam.id)}/${p.view}.mjpg` +
    `?fps=${p.fps}&w=${p.size}&q=80&t=${Date.now()}`;

  // The page

  // The Orin's status, the camera tabs, the chosen camera and its settings.
  function render() {
    const st = S.status;
    const c = st ? st.config : { host: '' };
    $('#visionPill').className = `pill ${!st ? '' : st.reachable ? 'ok' : 'err'}`;
    $('#visionPill').textContent = !st ? 'Checking…'
      : st.reachable ? `${cameras().length} Camera${cameras().length === 1 ? '' : 's'}`
      : 'Orin Not Reachable';
    const addr = $('#visionHost');
    if (document.activeElement !== addr) {
      addr.value = c.host || '';
    }
    $('#visionError').hidden = !(st && st.error);
    $('#visionError').textContent = st && st.error ? st.error : '';
    const cam = current();
    $('#visionEmpty').hidden = Boolean(cam) || !(st && st.reachable);
    $('#visionStage').hidden = !cam;
    renderTabs(cam);
    renderStage(cam);
    renderParams(cam);
  }

  // A tab per camera, with a dot for its state.
  function renderTabs(cam) {
    $('#visionTabs').innerHTML = cameras().map(c => {
      const dot = c.state === 'streaming' ? 'live'
        : c.state === 'busy' ? 'busy'
        : c.state === 'error' ? 'err'
        : '';
      return `<button type="button" role="tab" data-vtab="${esc(c.id)}" class="${cam && c.id === cam.id ? 'active' : ''}"
        aria-selected="${cam && c.id === cam.id}" title="${esc(c.detail || c.id)}"><i class="${dot}"></i>${esc(c.name)}<small>${esc(KIND[c.kind] || c.kind)}</small></button>`;
    }).join('');
  }

  // The chosen camera: its view, rate and size, what it is, Play/Stop, and why it isn't
  // streaming (if so).
  function renderStage(cam) {
    if (!cam) {
      return;
    }
    const p = prefs(cam);
    $('#visionViews').innerHTML = cam.views.map(v =>
      `<button type="button" data-vview="${v.id}" class="${v.id === p.view ? 'active' : ''}">` +
      `${esc(v.label)}</button>`).join('');
    $('#visionFps').innerHTML = RATES.map(r =>
      `<option value="${r}" ${r === p.fps ? 'selected' : ''}>${r} fps</option>`).join('');
    $('#visionSize').innerHTML = SIZES.map(([v, l]) =>
      `<option value="${v}" ${v === p.size ? 'selected' : ''}>${l}</option>`).join('');

    // Model, serial number and USB speed (a warning for a USB 3 camera on USB 2).
    const usb = Number(cam.usb_speed_mbps);
    $('#visionMeta').innerHTML = [
      cam.model && cam.model !== cam.name ? esc(cam.model) : '',
      cam.serial ? `#${esc(cam.serial)}` : '',
      usb
        ? (usb < 5000 && cam.kind !== 'uvc'
          ? '<span class="warn-text" title="Plug it into a USB 3 port: over USB 2 it offers ' +
            'lower resolutions and frame rates">USB 2</span>'
          : `USB ${usb >= 5000 ? '3' : '2'}`)
        : '',
    ].filter(Boolean).join(' · ');

    // Play/Stop, and the message over the picture until frames arrive.
    const playing = S.live && img.getAttribute('src');
    const btn = $('#visionPlay');
    btn.textContent = S.live ? 'Stop' : 'Play';
    btn.className = `btn btn-sm ${S.live ? 'btn-stop' : 'btn-go'}`;
    const why = S.failed || (cam.state === 'busy' || cam.state === 'error' ? cam.detail : '');
    const msg = $('#visionMsg');
    msg.hidden = Boolean(playing && img.dataset.live === '1' && !why);
    msg.innerHTML = why
      ? `<b>${cam.state === 'busy' ? 'In Use' : 'Couldn\'t Stream'}</b><span>${esc(why)}</span>`
      : S.live ? '<span class="spinner"></span><span>Opening the camera…</span>'
      : `<b>${esc(cam.name)}</b><span>Press Play to stream.</span>`;
  }

  // Settings

  // The chosen camera's settings, in folding groups.
  function renderParams(cam) {
    const box = $('#visionParams');
    $('#visionParamsTitle').textContent = cam ? `${cam.name} Settings` : 'Camera Settings';
    $('#visionReset').disabled = !cam;
    if (!cam) {
      box.innerHTML = '';
      S.paramsFor = null;
      return;
    }
    const d = S.params[cam.id];
    if (!d || !d.params || !d.params.length) {
      box.innerHTML = `<p class="hint">${d && d.open
        ? 'Reading the camera\'s settings…'
        : 'Press Play: its settings come from the camera once it is open.'}</p>`;
      S.paramsFor = null;
      return;
    }

    // Drawn again only when the camera or its list changes, so a slider being dragged isn't
    // replaced under the pointer.
    const key = cam.id + d.open +
      JSON.stringify(d.params.map(p =>
        [p.id, p.value, p.inactive, p.options && p.options.length])) +
      JSON.stringify(d.errors);
    if (S.paramsFor === key) {
      return;
    }
    S.paramsFor = key;

    // The settings by group; each group's folding is remembered.
    const groups = [];
    for (const p of d.params) {
      let g = groups.find(x => x.name === p.group);
      if (!g) {
        groups.push(g = { name: p.group, items: [] });
      }
      g.items.push(p);
    }
    const open = storage.get('spot.vision.groups', {});
    box.innerHTML = groups.map(g => `<details data-vgroup="${esc(g.name)}" ${open[g.name] === false ? '' : 'open'}><summary>${esc(g.name)}</summary>
      ${g.items.map(p => paramHtml(p, d.errors && d.errors[p.id])).join('')}</details>`).join('') +
      (d.open ? ''
        : '<p class="hint">The camera is closed: changes are kept and applied when it opens.</p>');
  }

  // One setting: its label (with unit, help and default) and its control (switch, list, or
  // number and slider), with its error if the camera refused it.
  function paramHtml(p, err) {
    const id = esc(p.id);
    const tip = esc([
      p.help, p.reopen ? 'Changing it reopens the camera' : '',
      p.default != null ? `Default: ${p.default}` : '',
    ].filter(Boolean).join('. '));
    const text = `${esc(p.label)}${p.unit ? ` [${esc(p.unit)}]` : ''}` +
      `${p.reopen ? '<span class="reopen">↻</span>' : ''}`;
    const dis = p.inactive ? 'disabled' : '';
    let input;
    if (p.type === 'bool') {
      input = `<label class="toggle"><input type="checkbox" data-vparam="${id}" ` +
        `${p.value ? 'checked' : ''} ${dis}><span></span></label>`;
    } else if (p.type === 'enum') {
      input = `<select class="input" data-vparam="${id}" ${dis}>` +
        `${(p.options || []).map(([v, l]) => `<option value="${esc(JSON.stringify(v))}" ` +
          `${String(v) === String(p.value) ? 'selected' : ''}>${esc(l)}</option>`).join('')}` +
        '</select>';
    } else {
      const step = p.step || 1;
      input = `<input class="input vp-num" type="number" data-vparam="${id}" min="${p.min}" max="${p.max}" step="${step}" value="${p.value}" ${dis}>
        <input type="range" data-vrange="${id}" min="${p.min}" max="${p.max}" step="${step}" value="${p.value}" ${dis}>`;
    }
    return `<div class="vision-param${p.inactive ? ' inactive' : ''}">` +
      `<label title="${tip}">${text}</label>${input}` +
      `${err ? `<span class="vp-err">${esc(err)}</span>` : ''}</div>`;
  }

  // Reads a camera's settings from the Orin.
  async function fetchParams(cam) {
    if (!cam) {
      return;
    }
    S.params[cam.id] = await api(`/api/vision/params?camera=${encodeURIComponent(cam.id)}`);
    if (S.shown) {
      renderParams(current());
    }
  }

  // Changes (or resets) a camera's settings; the answer is its new list.
  async function sendParams(cam, body) {
    S.params[cam.id] = await api('/api/vision/params', { camera: cam.id, ...body });
    S.paramsFor = null;
    renderParams(current());
  }

  // The stream

  // Streams a camera; its settings are read once it has opened.
  function play(cam) {
    S.live = true;
    S.failed = null;
    img.dataset.live = '0';

    // The first frame: the message goes, and the settings are read if not yet.
    img.onload = () => {
      img.dataset.live = '1';
      renderStage(current());
      if (!(S.params[cam.id] || {}).params?.length) {
        act(() => fetchParams(cam));
      }
    };

    // The stream ended or couldn't start: say why (the camera list has the detail).
    img.onerror = () => {
      if (!S.live) {
        return;
      }
      S.live = false;
      img.removeAttribute('src');
      act(async () => {
        await refresh();
        const now = cameras().find(c => c.id === cam.id);
        S.failed = (now && now.detail) ||
          'the stream stopped (the camera unplugged, or another program took it?)';
        render();
      });
    };
    img.src = streamUrl(cam, prefs(cam));
    renderStage(cam);
    setTimeout(() => act(() => fetchParams(cam)), 2500);
  }

  // Closes the stream's connection, so the Orin lets the camera go.
  function stopImg() {
    img.onerror = null;
    img.onload = null;
    img.removeAttribute('src');
    img.dataset.live = '0';
  }

  // Stops streaming.
  function stop() {
    S.live = false;
    stopImg();
    renderStage(current());
  }

  // Chooses a camera; a stream carries on with the new one.
  function select(id) {
    if (id === S.selected) {
      return;
    }
    const wasLive = S.live;
    stopImg();
    S.selected = id;
    S.failed = null;
    S.paramsFor = null;
    storage.set('spot.vision.selected', id);
    render();
    const cam = current();
    act(() => fetchParams(cam));
    if (wasLive && cam) {
      play(cam);
    }
  }

  // Input

  // Camera tabs, and the view: a stream restarts with the new view.
  $('#visionTabs').addEventListener('click', ev => {
    const b = ev.target.closest('[data-vtab]');
    if (b) {
      select(b.dataset.vtab);
    }
  });
  $('#visionViews').addEventListener('click', ev => {
    const b = ev.target.closest('[data-vview]');
    const cam = current();
    if (!b || !cam) {
      return;
    }
    setPref(cam.id, 'view', b.dataset.vview);
    for (const o of document.querySelectorAll('#visionViews [data-vview]')) {
      o.classList.toggle('active', o === b);
    }
    if (S.live) {
      play(cam);
    }
  });

  // Rate and size: remembered; a stream restarts with them.
  for (const [sel, key, conv] of [['#visionFps', 'fps', Number], ['#visionSize', 'size', String]]) {
    $(sel).onchange = ev => {
      const cam = current();
      if (!cam) {
        return;
      }
      setPref(cam.id, key, conv(ev.target.value));
      if (S.live) {
        play(cam);
      }
    };
  }

  // Play/Stop, a full-resolution snapshot in a new tab, full screen, refresh.
  $('#visionPlay').onclick = () => {
    const cam = current();
    if (cam) {
      if (S.live) {
        stop();
      } else {
        play(cam);
      }
    }
  };
  $('#visionSnap').onclick = () => {
    const cam = current();
    if (cam) {
      window.open(`${S.status.url}/snapshot/${encodeURIComponent(cam.id)}/${prefs(cam).view}.jpg` +
        `?q=95&w=0&t=${Date.now()}`, '_blank', 'noopener');
    }
  };
  $('#visionFull').onclick = () => $('#visionFrame').requestFullscreen?.();
  $('#visionRefresh').onclick = () => act(refresh);

  // Save the Orin's address; reset the camera's settings.
  $('#visionSave').onclick = () => act(async () => {
    const host = $('#visionHost').value.trim();
    stop();
    await api('/api/settings', { vision: { host } });
    await refresh();
    toast(`The Vision tab now looks for SPOT Vision at ${S.status.url}.`, 'success', 4000);
  });
  $('#visionReset').onclick = () => act(async () => {
    const cam = current();
    if (cam && confirm(`Put every setting of the ${cam.name} back to its default?`)) {
      await sendParams(cam, { reset: true });
    }
  });

  // The settings pane: a slider shows its value as it moves, and sends it when let go; the rest
  // send at once.
  const paramsBox = $('#visionParams');
  paramsBox.addEventListener('input', ev => {
    const id = ev.target.dataset.vrange;
    if (id) {
      const num = paramsBox.querySelector(`[data-vparam="${CSS.escape(id)}"]`);
      if (num) {
        num.value = ev.target.value;
      }
    }
  });
  paramsBox.addEventListener('change', ev => {
    const cam = current();
    const t = ev.target;
    const id = t.dataset.vparam || t.dataset.vrange;
    if (!cam || !id) {
      return;
    }
    let value;
    if (t.type === 'checkbox') {
      value = t.checked;
    } else if (t.tagName === 'SELECT') {
      value = JSON.parse(t.value);
    } else {
      value = Number(t.value);
    }
    act(() => sendParams(cam, { values: { [id]: value } }));
  });

  // Which groups are folded, remembered.
  paramsBox.addEventListener('toggle', ev => {
    const g = ev.target.dataset && ev.target.dataset.vgroup;
    if (!g) {
      return;
    }
    const open = storage.get('spot.vision.groups', {});
    open[g] = ev.target.open;
    storage.set('spot.vision.groups', open);
  }, true);

  // From the server

  // The Orin's status and cameras; the first camera if the chosen one is gone.
  async function refresh() {
    S.status = await api('/api/vision');
    if (!cameras().some(c => c.id === S.selected) && cameras().length) {
      S.selected = cameras()[0].id;
    }
    if (S.shown) {
      render();
    }
  }

  // Reads the camera list every POLL ms while the tab is shown.
  function schedule() {
    clearTimeout(S.timer);
    if (!S.shown) {
      return;
    }
    S.timer = setTimeout(async () => {
      try {
        await refresh();
      } catch {
        // Shown on the next one.
      }
      schedule();
    }, POLL);
  }

  return {
    // The tab is shown: read the cameras and the chosen one's settings, and keep polling.
    show() {
      S.shown = true;
      act(async () => {
        await refresh();
        await fetchParams(current());
      });
      schedule();
    },
    // The tab is left: stop polling and streaming (the camera is let go).
    hide() {
      if (!S.shown) {
        return;
      }
      S.shown = false;
      clearTimeout(S.timer);
      S.live = false;
      stopImg();
    },
  };
}
