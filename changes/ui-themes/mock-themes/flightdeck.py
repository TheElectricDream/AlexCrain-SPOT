"""FLIGHT DECK: colour that means something.

A GNC lab's console drawn by cockpit-display rules: black glass, outlines instead of filled
cards, and the avionics colour code used literally. Cyan is what you can set, green is what is
engaged or healthy, white is a label, amber is a caution, red is a warning, magenta is the
commanded value. Set in B612, the typeface Airbus commissioned for cockpit displays.
"""
from _common import face, viewer_js, ink_js

CSS = r"""
:root {
  --bg: #04060a; --panel: #04060a; --panel-2: #10161b; --border: #2a3a43; --border-strong: #4a606b;
  --text: #eef3f5; --muted: #8fa3ad; --accent: #2fd9f4; --accent-hover: #7beaff;
  --go: #35e06f; --go-hover: #6bf098; --stop: #ff3b30; --stop-hover: #ff6259;
  --warn: #ffb000; --ok: #35e06f; --radius: 0px; --radius-sm: 0px;
  --sans: "B612", system-ui, sans-serif; --mono: "IBM Plex Mono", ui-monospace, monospace;
  --cyan: #2fd9f4; --green: #35e06f; --amber: #ffb000; --magenta: #ff5cf0; --white: #eef3f5;
}
body { font: 400 13px/1.45 var(--sans); background: #04060a; }
.mono { font-size: 12px; }

/* ----- annunciator strip ----- */
.topbar { background: #04060a; border-bottom: 1px solid var(--border-strong); padding: 8px 12px; gap: 8px; }
.brand { flex: none; }
.brand h1 { font: 700 13px var(--sans); text-transform: uppercase; letter-spacing: .08em; }
.brand p { font: 400 11px var(--mono); color: var(--green); text-transform: uppercase; }
.topbar .page-tabs { background: transparent; border: 0; padding: 0; gap: 0; margin-left: 4px; }
.settings-bar .btn, .topstats .btn { padding: 0 8px; letter-spacing: .06em; }
.topstats { gap: 6px; }
.settings-bar { gap: 5px; padding-left: 8px; padding-right: 5px; }
.sb-file { max-width: 120px; }
.topbar .badge { padding: 5px 7px; letter-spacing: .06em; }
.page-tabs button { font: 700 11.5px var(--sans); text-transform: uppercase; letter-spacing: .08em; color: var(--white); padding: 8px 10px; border-radius: 0; border: 1px solid transparent; border-right-color: var(--border); }
.page-tabs button:last-child { border-right-color: transparent; }
.page-tabs button:hover { color: var(--cyan); }
.page-tabs button.active { background: transparent; color: var(--green); border: 1px solid var(--white); box-shadow: none; }
.settings-bar { background: transparent; border: 1px solid var(--border); border-radius: 0; }
.sb-title, .stat-label, .field-label, .pb-ov-head { font: 400 10.5px var(--sans); text-transform: uppercase; letter-spacing: .12em; color: var(--muted); }
.sb-file { color: var(--green); }
.badge { border-radius: 0; background: transparent; border: 1px solid currentColor; font: 700 11.5px var(--sans); text-transform: uppercase; letter-spacing: .1em; padding: 5px 10px; }
.badge .dot { border-radius: 0; width: 8px; height: 8px; }
.badge-bad { background: var(--stop); color: #fff; border-color: var(--stop); }
.badge-bad .dot { display: none; }

/* ----- pages are outlined, never filled ----- */
.sidebar, .msg-slot, .sim-run { background: #04060a; }
.card { background: transparent; border: 1px solid var(--border-strong); border-radius: 0; box-shadow: none; padding: 0 12px 14px; }
.card-head { margin: 0 -12px 12px; padding: 7px 12px; border-bottom: 1px solid var(--border-strong); }
.card-head h2, .msg-head h2 { font: 700 12.5px var(--sans); letter-spacing: .16em; color: var(--white); text-decoration: underline; text-underline-offset: 4px; text-decoration-thickness: 1px; }
.hint { color: var(--muted); }

/* ----- soft keys ----- */
.btn { border-radius: 0; background: #151c22; border: 1px solid #5d7480; color: var(--white); font: 700 11.5px var(--sans); text-transform: uppercase; letter-spacing: .09em; box-shadow: inset 0 1px 0 rgba(255,255,255,.12), inset 0 -2px 0 rgba(0,0,0,.5); }
.btn:hover { background: #1e2830; border-color: var(--white); }
.btn-ghost { background: transparent; border-color: transparent; box-shadow: none; }
.btn-primary { background: #151c22; border-color: var(--cyan); color: var(--cyan); }
.btn-primary:hover { background: #10262c; border-color: var(--cyan); }
.btn-go { background: var(--green); border-color: var(--green); color: #02140a; font-size: 13px; letter-spacing: .14em; box-shadow: none; }
.btn-go:hover { background: #6bf098; border-color: #6bf098; }
.btn-stop { background: var(--stop); border-color: var(--stop); color: #fff; }
/* cyan: a value you can set.  green: a value you can only read. */
.input { border-radius: 0; background: #04060a; border: 1px solid var(--cyan); color: var(--cyan); font: 700 13px var(--mono); }
select.input { font-family: var(--sans); }
.input:focus { border-color: var(--white); box-shadow: 0 0 0 1px var(--white); }
.input[readonly], .input:disabled { color: var(--green); border-color: var(--border-strong); opacity: 1; }
.input-suffix span { color: var(--muted); }
.segmented { background: transparent; border: 1px solid var(--border-strong); border-radius: 0; padding: 0; }
.segmented button { border-radius: 0; font: 700 11px var(--sans); text-transform: uppercase; letter-spacing: .1em; color: var(--white); padding: 7px 12px; }
.segmented button.active { background: transparent; color: var(--green); box-shadow: inset 0 0 0 1px var(--white); }
.page-tabs button.active { box-shadow: none; }
.toggle { color: var(--white); }
.toggle > input + span { border-radius: 0; background: #04060a; box-shadow: inset 0 0 0 1px #5d7480; }
.toggle > input + span::after { border-radius: 0; background: #5d7480; }
.toggle input:checked + span { background: #04060a; box-shadow: inset 0 0 0 1px var(--green); }
.toggle input:checked + span::after { background: var(--green); }
.pill { border-radius: 0; background: transparent; border: 1px solid var(--border-strong); color: var(--green); font: 400 11.5px var(--mono); text-transform: uppercase; }
.pill.live { background: var(--green); color: #02140a; border-color: var(--green); }
.pill.err { background: var(--stop); color: #fff; border-color: var(--stop); }
.plat-chip { border-radius: 0 !important; background: transparent; font: 700 11.5px var(--sans); text-transform: uppercase; letter-spacing: .09em; }
.count { border-radius: 0; background: transparent; color: var(--green); box-shadow: inset 0 0 0 1px var(--border-strong); }

/* ----- timeline ----- */
.timeline-bar { border-radius: 0; border-color: var(--white); }
.tl-swatch { border-radius: 0; width: 4px; }
.tl-name b { font-weight: 700; text-transform: uppercase; letter-spacing: .08em; font-size: 12px; }
.tl-when { color: var(--green); font-family: var(--mono); }
.tl-sum { color: var(--green); }
.tl-row.off .tl-when { color: var(--amber); }

/* ----- the 3D view is the navigation display ----- */
.viewer { background: #000; }
.viewer-toolbar, .legend, .viewer-empty, .playback { border-radius: 0; background: rgba(4,6,10,.92); border: 1px solid var(--border-strong); backdrop-filter: none; }
.pb-time { color: var(--green); font: 700 13px var(--mono); }
.pb-phase { color: var(--green); font: 700 12px var(--sans); text-transform: uppercase; letter-spacing: .1em; box-shadow: inset 0 0 0 1px var(--white); padding: 5px 9px; text-align: center; }
.pb-track input { accent-color: var(--magenta); }
.pb-phases i { background: var(--white); opacity: .55; }
.craft-label { border-radius: 0; font: 700 10.5px var(--sans); text-transform: uppercase; letter-spacing: .08em; box-shadow: none; }
.tick-label { color: var(--cyan); opacity: .8; }
.origin-label { border-radius: 0; color: var(--green); }

/* ----- console and messages ----- */
.console { background: #04060a; border-top: 1px solid var(--border-strong); }
.console-head { border-bottom: 1px solid var(--border-strong); }
.tabs button { font: 700 11.5px var(--sans); text-transform: uppercase; letter-spacing: .1em; color: var(--white); }
.tabs button.active { color: var(--green); border-bottom-color: var(--green); }
.log { color: var(--green); font-family: var(--mono); }
.log .l-cmd { color: var(--cyan); } .log .l-head { color: var(--white); } .log .l-err { color: var(--stop); } .log .l-warn { color: var(--amber); }
.msg-slot { border-left: 1px solid var(--border-strong); }
.msg-head { border-bottom: 1px solid var(--border-strong); }
.msg-plat { border-bottom: 1px solid var(--border-strong); }
.msg-plat-head b { font-weight: 700; text-transform: uppercase; letter-spacing: .12em; font-size: 12px; }
.msg-plat-head .swatch { border-radius: 0; }
.msg-now { color: var(--green); }
.msg-list { font-size: 11.5px; }
.msg-list li { border-radius: 0; color: var(--white); }
.msg-list li.future { color: #4a606b; }
.msg-list li.current { background: transparent; color: var(--green); box-shadow: inset 0 0 0 1px var(--white); }
.msg-t { font-family: var(--mono); color: var(--muted); }
.dialog { border-radius: 0; border: 1px solid var(--white); }
"""

FONTS = (face("IBM Plex Mono", "PlexMono-Regular.ttf", "400") + face("IBM Plex Mono", "PlexMono-Medium.ttf", "500 700")
         + face("B612", "B612-Regular.ttf", "400 500") + face("B612", "B612-Bold.ttf", "600 700")
         + face("B612 Mono", "B612Mono-Regular.ttf", "400 500") + face("B612 Mono", "B612Mono-Bold.ttf", "600 700"))

THEME = {
    "ramp": [(0.0, "#000000"), (0.055, "#04060a"), (0.09, "#080c11"), (0.125, "#10161b"),
             (0.18, "#2a3a43"), (0.24, "#4a606b"), (0.41, "#5d7480"), (0.59, "#8fa3ad"),
             (0.80, "#c8d3d9"), (0.92, "#eef3f5"), (1.0, "#ffffff")],
    "accent": "#2fd9f4",
    "css": CSS,
    "fontface": FONTS,
    "js": {"viewer.js": viewer_js("000000", "020405", "0f3540", "0a1f26"),
           "results.js": ink_js("#eef3f5", "#8fa3ad", "#16222a", "#4a606b", "#04060a")},
}
