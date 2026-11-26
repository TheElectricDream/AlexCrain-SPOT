"""BENCH: the old SPOT GUI, grown up.

The light grey of the MATLAB App Designer GUI (SPOT 3 to 5.0), its titled group boxes, folder
tabs and red italic "SPOT", crossed with a bench instrument's front panel: keycap buttons,
LED indicators, dark display windows. Plots use MATLAB's own line colours.
"""
from _common import face, viewer_js, ink_js

CSS = r"""
:root {
  --bg: #e2dfd8; --panel: #eeece6; --panel-2: #f7f6f2; --border: #bdb9af; --border-strong: #8f8b82;
  --text: #1d1d1b; --muted: #5f5c55; --accent: #0072bd; --accent-hover: #1a86cf;
  --go: #2e8f57; --go-hover: #36a465; --stop: #c8372d; --stop-hover: #d94a40;
  --warn: #a86400; --ok: #1f8a4c; --radius: 3px; --radius-sm: 3px;
  --sans: "IBM Plex Sans", system-ui, sans-serif; --mono: "IBM Plex Mono", ui-monospace, monospace;
  --face: #e2dfd8; --ink: #1d1d1b; --spot-red: #a2142f;
  color-scheme: light;
}
body { font: 400 13.5px/1.45 var(--sans); background: var(--face); }
* { scrollbar-color: #a9a59b transparent; }

/* ----- top bar: the panel's name strip, with folder tabs ----- */
.topbar {
  background: linear-gradient(180deg, #f3f1ec, #e6e3dc); border-bottom: 1px solid #7d7a72; padding: 8px 18px 0;
  align-items: flex-end; box-shadow: inset 0 3px 0 var(--spot-red);
}
.topbar > * { margin-bottom: 8px; }
.brand { flex: none; }
.brand h1 { font: italic 700 17px var(--sans); color: var(--spot-red); letter-spacing: 0; }
.brand p { font: 500 11px var(--mono); color: #5f5c55; }
.page-tabs { background: transparent; border: 0; padding: 0; gap: 2px; margin-bottom: -1px !important; align-self: flex-end; }
.page-tabs button {
  font: 500 13px var(--sans); color: #3b3a36; padding: 8px 16px 9px; border: 1px solid #a9a59b; border-bottom-color: #7d7a72;
  border-radius: 4px 4px 0 0; background: linear-gradient(180deg, #d9d6cf, #cfccc4);
}
.page-tabs button:hover { background: #e4e1da; }
.page-tabs button.active {
  background: var(--face); color: var(--ink); font-weight: 600; border-color: #7d7a72; border-bottom-color: var(--face);
  box-shadow: inset 0 2px 0 var(--accent);
}
.settings-bar { background: transparent; border: 1px solid #a9a59b; border-radius: 3px; }
.sb-title, .stat-label, .field-label, .msg-head h2, .card-head h2, .pb-ov-head {
  font: 600 11px var(--sans); font-stretch: 85%; text-transform: uppercase; letter-spacing: .07em; color: #3b3a36;
}
.sb-file { color: var(--ink); }

/* an indicator lamp */
.badge { border-radius: 3px; background: #f7f6f2; border: 1px solid #8f8b82; font: 600 11.5px var(--sans); color: var(--ink) !important; padding: 5px 10px; }
.badge .dot { width: 10px; height: 10px; box-shadow: inset 0 -1px 2px rgba(0,0,0,.35), inset 0 1px 1px rgba(255,255,255,.7), 0 0 0 1px #55524b; }
.badge-ok .dot { background: #25c26a; box-shadow: inset 0 -1px 2px rgba(0,0,0,.3), inset 0 1px 1px rgba(255,255,255,.7), 0 0 0 1px #55524b, 0 0 7px #25c26a; }
.badge-warn .dot { background: #f2a81d; }
.badge-bad .dot { background: #e03a2f; box-shadow: inset 0 -1px 2px rgba(0,0,0,.3), inset 0 1px 1px rgba(255,255,255,.6), 0 0 0 1px #55524b, 0 0 7px #e03a2f; }

/* ----- group boxes: the title sits on the border, as in the old GUI ----- */
.sidebar, .msg-slot, .sim-run { background: var(--face); }
.sidebar { border-right: 1px solid #7d7a72; gap: 20px; padding-top: 22px; }
.sim-panes > div { gap: 22px; }
.sim-panes { padding-top: 20px; }
.card {
  position: relative; background: var(--panel); border: 1px solid #8f8b82; border-radius: 3px;
  box-shadow: 0 1px 0 rgba(255,255,255,.8), inset 0 1px 0 rgba(255,255,255,.7); padding: 18px 13px 14px;
}
.card-head { justify-content: flex-end; min-height: 0; margin-bottom: 10px; margin-top: 2px; }
.card-head:has(> h2:only-child) { margin-bottom: 0; }
.card-head h2 {
  position: absolute; top: -8px; left: 50%; transform: translateX(-50%); padding: 0 9px; white-space: nowrap;
  background: linear-gradient(180deg, var(--face) 50%, var(--panel) 50%); font-size: 12px; color: var(--ink); line-height: 16px;
}
.hint { color: #5f5c55; }

/* ----- keycaps ----- */
.btn {
  border-radius: 3px; border: 1px solid #8f8b82; border-bottom: 2px solid #6f6c64; color: var(--ink);
  background: linear-gradient(180deg, #fdfcfa, #e9e6df); font: 500 12.5px var(--sans); box-shadow: 0 1px 0 rgba(255,255,255,.6);
}
.btn:hover { background: linear-gradient(180deg, #ffffff, #f1efe9); border-color: #6f6c64; }
.btn:active { transform: translateY(1px); border-bottom-width: 1px; margin-bottom: 1px; }
.btn-ghost { background: transparent; border-color: transparent; box-shadow: none; }
.btn-primary { background: linear-gradient(180deg, #1583cf, #0069ad); border-color: #00548b; border-bottom-color: #003f69; color: #fff; }
.btn-primary:hover { background: linear-gradient(180deg, #2a93dc, #0072bd); border-color: #00548b; }
.btn-go { background: linear-gradient(180deg, #38a868, #2a8650); border-color: #1e6b3e; border-bottom-color: #154f2d; color: #fff; font-weight: 600; text-shadow: 0 -1px 0 rgba(0,0,0,.25); }
.btn-go:hover { background: linear-gradient(180deg, #43b674, #2e8f57); border-color: #1e6b3e; }
.btn-stop { background: linear-gradient(180deg, #d9483d, #bb2f25); border-color: #8f1f17; border-bottom-color: #6d1610; color: #fff; }
.input {
  border-radius: 2px; background: #ffffff; border: 1px solid #8f8b82; border-top-color: #6f6c64; color: var(--ink);
  box-shadow: inset 0 1px 2px rgba(0,0,0,.13); font-family: var(--mono); font-size: 12.5px;
}
select.input { font-family: var(--sans); font-size: 13px; }
.input:focus { border-color: var(--accent); box-shadow: inset 0 1px 2px rgba(0,0,0,.13), 0 0 0 2px rgba(0,114,189,.3); }
.input[readonly] { color: #3b3a36; background: #f2f0ea; }
.segmented { background: #cfccc4; border: 1px solid #8f8b82; border-radius: 3px; padding: 2px; box-shadow: inset 0 1px 2px rgba(0,0,0,.18); }
.segmented button { color: #3b3a36; border-radius: 2px; font: 500 12px var(--sans); }
.segmented button.active { background: linear-gradient(180deg, #fdfcfa, #eceae4); color: var(--ink); font-weight: 600; box-shadow: 0 1px 1px rgba(0,0,0,.35); }
.page-tabs button.active { box-shadow: inset 0 2px 0 var(--accent); background: var(--face); }
.toggle { color: var(--ink); }
.toggle > input + span { border-radius: 3px; background: #b3afa5; box-shadow: inset 0 1px 2px rgba(0,0,0,.35); }
.toggle > input + span::after { border-radius: 2px; background: linear-gradient(180deg, #ffffff, #dedbd3); box-shadow: 0 1px 1px rgba(0,0,0,.45); }
.toggle input:checked + span { background: var(--accent); }
.toggle input:checked + span::after { background: linear-gradient(180deg, #ffffff, #e6e3dc); }
.pill { border-radius: 2px; background: #f7f6f2; border: 1px solid #8f8b82; color: #3b3a36; font-family: var(--mono); font-size: 11.5px; }
.pill.live { background: #25c26a; border-color: #1e6b3e; color: #04130b; }
.pill.err { background: #c8372d; border-color: #8f1f17; color: #fff; }
.plat-chip { border-radius: 3px !important; background: linear-gradient(180deg, #fdfcfa, #e9e6df); color: #5f5c55; border-bottom-width: 2px; }
.plat-chip.on { color: var(--ink); background: color-mix(in srgb, var(--c) 26%, #fbfaf7); box-shadow: inset 0 0 0 1px var(--c); }
.count { background: #cfccc4; color: var(--ink); border-radius: 2px; }

/* ----- timeline ----- */
.timeline-bar { border: 1px solid #55524b; border-radius: 2px; box-shadow: inset 0 1px 2px rgba(0,0,0,.3); }
.tl-sum { color: var(--ink); }

/* ----- display windows: the 3D view and the logs stay dark, behind a bezel ----- */
.viewer { background: #15171a; box-shadow: inset 0 0 0 1px #2c2b28, inset 0 2px 10px rgba(0,0,0,.6); border-bottom: 1px solid #55524b; }
.viewer-toolbar, .legend, .playback, .viewer-empty {
  border-radius: 4px; background: rgba(232,229,222,.94); border: 1px solid #55524b; backdrop-filter: none; color: var(--ink);
  box-shadow: 0 2px 8px rgba(0,0,0,.45);
}
.legend { color: var(--ink); }
.pb-time { color: var(--ink); font-family: var(--mono); background: #15171a; color: #7dffb0; padding: 4px 8px; border-radius: 2px; box-shadow: inset 0 1px 3px #000; }
.pb-phase { color: #a86400; }
.pb-phases i { background: rgba(0,0,0,.4); }
.tick-label { color: #9aa2b1; text-shadow: 0 1px 2px #000; }
.origin-label { color: #e6e9ef; background: rgba(0,0,0,.55); }
.axis-label { text-shadow: 0 1px 3px #000; }
.console { background: #15171a; border-top: 1px solid #55524b; }
.console-head { background: var(--face); border-bottom: 1px solid #55524b; padding-top: 5px; }
.tabs button { color: #5f5c55; font: 500 12.5px var(--sans); }
.tabs button.active { color: var(--ink); font-weight: 600; border-bottom: 3px solid var(--accent); }
.log { color: #c7cdd8; }
.log .l-cmd { color: #7fb0ff; } .log .l-head { color: #e6e9ef; } .log .l-ok { color: #4fe08f; }
.log .l-err { color: #ff7b7f; } .log .l-warn { color: #f5a524; }
.console .cmd-row, .console form, .console .matlab-cmd { background: #101215; }
.console input, .console textarea { color: #d7dbe2 !important; background: transparent !important; }

/* ----- messages: a printed strip per platform ----- */
.msg-slot { border-left: 1px solid #7d7a72; }
.msg-plat-head b { font-weight: 700; letter-spacing: .02em; }
.msg-now { color: var(--ink); }
.msg-list { font-size: 12px; background: #fbfaf7; margin: 0 8px 8px 12px; padding: 4px 2px; border: 1px solid #a9a59b; box-shadow: inset 0 1px 2px rgba(0,0,0,.12); }
.msg-list li { color: #3b3a36; border-radius: 0; }
.msg-list li.future { color: #a9a59b; }
.msg-list li.current { background: #cfe6f6; color: #00243d; box-shadow: inset 3px 0 0 var(--accent); }
.dialog { border-radius: 4px; box-shadow: 0 18px 50px rgba(0,0,0,.4); }
.res-plot, .res-card { background: #fff; }
"""

FONTS = (face("IBM Plex Sans", "PlexVar.ttf", "100 700", extra="font-stretch: 75% 100%;")
         + face("IBM Plex Mono", "PlexMono-Regular.ttf", "400") + face("IBM Plex Mono", "PlexMono-Medium.ttf", "500 700"))

MATLAB_LINES = ("  '#0072bd', '#d95319', '#edb120', '#7e2f8e', '#77ac30', '#4dbeee', '#a2142f', '#3b3a36',")

THEME = {
    "light": True,
    "ramp": [(0.0, "#f2f0ea"), (0.055, "#e2dfd8"), (0.092, "#eeece6"), (0.125, "#f7f6f2"),
             (0.18, "#bdb9af"), (0.24, "#8f8b82"), (0.41, "#8a867d"), (0.59, "#5f5c55"),
             (0.80, "#3b3a36"), (0.92, "#1d1d1b"), (1.0, "#000000")],
    "accent": "#0072bd",
    "shadow": "#2b2925", "shadow_k": 0.5,
    "css": CSS,
    "fontface": FONTS,
    "js": {"results.js": ink_js("#1d1d1b", "#5f5c55", "#dddad3", "#55524b", "#ffffff") + [
        ("  '#3987e5', '#d95926', '#199e70', '#c98500', '#d55181', '#008300', '#9085e9', '#e66767',",
         MATLAB_LINES)]},
}
