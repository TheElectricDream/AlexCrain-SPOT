"""PATCH: the SRL emblem as a design language.

Black field, Carleton red, white keylines; the emblem's four-pointed star as the one motif;
the wordmark's condensed capitals for headings and the wide italic of "SRL" for the brand.
"""
from _common import face, star, svg, STAR, viewer_js, ink_js

RED = "#e91c24"

CSS = r"""
:root {
  --bg: #070708; --panel: #0e0e10; --panel-2: #18181b; --border: #2a2a2e; --border-strong: #404046;
  --text: #f4f4f2; --muted: #96969c; --accent: %(red)s; --accent-hover: #ff3b42;
  --go: #f4f4f2; --go-hover: #ffffff; --stop: %(red)s; --stop-hover: #ff3b42;
  --warn: #ffb020; --ok: #4ad38a; --radius: 2px; --radius-sm: 2px;
  --sans: "Barlow", system-ui, sans-serif; --mono: "IBM Plex Mono", ui-monospace, monospace;
  --display: "Bebas Neue", "Barlow Condensed", sans-serif; --wide: "Michroma", sans-serif;
  --star: %(star)s;
}
body { font: 500 14px/1.45 var(--sans); letter-spacing: .005em; }

/* ----- top bar: black, with the emblem's red-and-white keyline under it ----- */
.topbar {
  background: #070708; padding: 9px 14px 10px; gap: 10px; border-bottom: 2px solid var(--accent);
  box-shadow: 0 3px 0 #070708, 0 4px 0 rgba(255,255,255,.75); position: relative; z-index: 6; margin-bottom: 4px;
}
.brand { flex: none; }
.brand-logo { width: 42px; height: 42px; }
.brand h1 { font: 400 10.5px var(--wide); text-transform: uppercase; letter-spacing: .06em; transform: skewX(-11deg); transform-origin: left bottom; }
.brand p { font: 500 11px var(--mono); letter-spacing: .02em; text-transform: uppercase; }

/* page tabs: leaning plates, like the emblem's lettering */
.page-tabs { background: transparent; border: 0; padding: 0; gap: 3px; }
.page-tabs button {
  position: relative; z-index: 0; font: 400 16px/1 var(--display); letter-spacing: .08em; text-transform: uppercase;
  padding: 9px 12px 7px; color: #b9b9be; border-radius: 0;
}
.page-tabs button::before {
  content: ""; position: absolute; inset: 0; z-index: -1; transform: skewX(-14deg);
  background: #141416; border: 1px solid var(--border);
}
.page-tabs button:hover { color: #fff; }
.page-tabs button:hover::before { border-color: #6a6a70; }
.page-tabs button.active { background: transparent; color: #fff; box-shadow: none; }
.page-tabs button.active::before { background: var(--accent); border: 1.5px solid #fff; }

.settings-bar { border-radius: 0; background: transparent; border: 0; border-left: 1px solid var(--border); padding-left: 14px; }
.sb-title, .stat-label { font: 400 12.5px var(--display); letter-spacing: .12em; }
.badge { border-radius: 0; font: 600 11.5px var(--sans); text-transform: uppercase; letter-spacing: .07em; padding: 5px 12px; background: transparent; }
.badge .dot { width: 11px; height: 11px; border-radius: 0; background: currentColor; -webkit-mask: var(--star) center/contain no-repeat; mask: var(--star) center/contain no-repeat; }

/* ----- surfaces ----- */
.sidebar, .msg-slot, .sim-run { background: #0a0a0b; }
.card { border-radius: 0; box-shadow: none; border: 1px solid var(--border); background: var(--panel); padding: 14px 14px 16px; }
.card-head { margin-bottom: 14px; padding-bottom: 8px; border-bottom: 1px solid var(--border); }
.card-head h2, .msg-head h2 {
  font: 400 19px/1 var(--display); letter-spacing: .1em; color: #fff; display: flex; align-items: center; gap: 9px;
}
.card-head h2::before, .msg-head h2::before {
  content: ""; width: 15px; height: 15px; flex: none; background: var(--star) center/contain no-repeat; margin-top: -2px;
}
.field-label { font: 600 11px var(--sans); text-transform: uppercase; letter-spacing: .09em; color: #a4a4aa; }

/* ----- controls ----- */
.btn { border-radius: 0; font: 600 12.5px var(--sans); text-transform: uppercase; letter-spacing: .07em; background: #141416; }
.btn:hover { background: #1f1f23; border-color: #fff; }
.btn-sm { font-size: 11px; }
.btn-primary { background: var(--accent); border: 1.5px solid #fff; color: #fff; }
.btn-primary:hover { background: var(--accent-hover); border-color: #fff; }
.btn-go { background: #f4f4f2; border-color: #f4f4f2; color: #070708; font: 400 21px/1 var(--display); letter-spacing: .12em; padding-top: 3px; }
.btn-go:hover { background: #fff; border-color: #fff; }
.btn-go svg { color: var(--accent); width: 18px; height: 18px; margin-top: -3px; }
.btn-stop { background: var(--accent); border: 1.5px solid #fff; color: #fff; }
.btn-ghost { background: transparent; }
.input { border-radius: 0; background: #070708; font-weight: 500; }
.input:focus { border-color: #fff; box-shadow: 0 0 0 2px var(--accent); }
.segmented { border-radius: 0; background: #070708; padding: 2px; }
.segmented button { border-radius: 0; font: 600 11.5px var(--sans); text-transform: uppercase; letter-spacing: .08em; }
.segmented button.active { background: #f4f4f2; color: #070708; box-shadow: none; }
.page-tabs button.active { color: #fff; }
.pill { border-radius: 0; border: 0; padding: 3px 14px; clip-path: polygon(8px 0, 100%% 0, calc(100%% - 8px) 100%%, 0 100%%);
  font: 600 11px var(--sans); text-transform: uppercase; letter-spacing: .07em; background: #222226; color: #d6d6da; }
.pill.live { background: var(--ok); color: #04130b; }
.pill.err { background: var(--accent); color: #fff; }
.toggle > input + span { border-radius: 0; background: #2a2a2e; outline: 1px solid #55555b; transform: skewX(-14deg); }
.settings-bar .btn, .topstats .btn { padding: 0 8px; letter-spacing: .04em; }
.topstats { gap: 8px; }
.page-tabs { margin-left: 4px; }
.settings-bar { padding-left: 10px; gap: 5px; }
.sb-file { max-width: 130px; }
.toggle > input + span::after { border-radius: 0; background: #b0b0b6; }
.toggle input:checked + span { background: var(--accent); outline-color: #fff; }
.toggle input:checked + span::after { background: #fff; }
.plat-chip { border-radius: 0 !important; font: 600 12px var(--sans); text-transform: uppercase; letter-spacing: .07em; }
.count { border-radius: 0; background: var(--accent); color: #fff; padding: 1px 6px; }

/* ----- timeline ----- */
.timeline-bar { border-radius: 0; border-color: #fff; }
.tl-swatch { border-radius: 0; }
.tl-name b { font: 400 16px/1 var(--display); letter-spacing: .07em; }

/* ----- 3D view ----- */
.viewer { background: #0a0a0b; }
.viewer-toolbar, .legend, .viewer-empty, .playback { border-radius: 0; background: rgba(7,7,8,.9); border: 1px solid #55555b; backdrop-filter: none; }
.playback { border-color: #fff; }
.pb-phase { font: 400 16px/1 var(--display); letter-spacing: .08em; color: #fff; padding-top: 2px; }
.pb-time { font-family: var(--mono); }
.pb-track input { accent-color: var(--accent); }
.craft-label { border-radius: 0; font: 600 10.5px var(--sans); text-transform: uppercase; letter-spacing: .08em; outline: 1px solid rgba(255,255,255,.85); }
.viewer-empty strong { font: 400 20px/1.1 var(--display); letter-spacing: .1em; }

/* ----- console and messages ----- */
.console { background: #070708; position: relative; }
.console::after {
  content: ""; position: absolute; right: 26px; bottom: 44px; width: 150px; height: 130px; pointer-events: none; opacity: .13;
  background: %(star_plain)s 0 34px/88px no-repeat, %(star_plain)s 92px 0/46px no-repeat, %(star_plain)s 104px 78px/40px no-repeat;
}
.tabs button { font: 400 16px/1 var(--display); letter-spacing: .1em; text-transform: uppercase; padding: 9px 12px 7px; }
.tabs button.active { border-bottom: 3px solid var(--accent); color: #fff; }
.msg-head { padding: 12px 12px 10px 8px; }
.msg-plat-head b { font: 400 17px/1 var(--display); letter-spacing: .1em; text-transform: uppercase; }
.msg-plat-head .swatch { border-radius: 0; width: 12px; height: 12px; outline: 1px solid rgba(255,255,255,.8); }
.msg-list li { border-radius: 0; }
.msg-list li.current { background: #1c1c20; color: #fff; box-shadow: inset 3px 0 0 var(--accent); }
.msg-list li.current .msg-t { color: #fff; }
.hint { color: #9c9ca2; }
.dialog { border-radius: 0; border: 1.5px solid #fff; }
""" % {"red": RED, "star": star(), "star_plain": star("#e91c24", "#ffffff", 1.0)}

FONTS = (face("Michroma", "Michroma.ttf") + face("Bebas Neue", "BebasNeue.ttf")
         + face("Barlow", "Barlow-Regular.ttf", "400") + face("Barlow", "Barlow-Medium.ttf", "500")
         + face("Barlow", "Barlow-SemiBold.ttf", "600 700")
         + face("IBM Plex Mono", "PlexMono-Regular.ttf", "400") + face("IBM Plex Mono", "PlexMono-Medium.ttf", "500 700"))

THEME = {
    "ramp": [(0.0, "#000000"), (0.055, "#070708"), (0.09, "#0e0e10"), (0.125, "#18181b"),
             (0.18, "#2a2a2e"), (0.24, "#404046"), (0.41, "#66666c"), (0.59, "#96969c"),
             (0.80, "#cfcfd2"), (0.92, "#f4f4f2"), (1.0, "#ffffff")],
    "accent": RED,
    "css": CSS,
    "fontface": FONTS,
    "js": {"viewer.js": viewer_js("0a0a0b", "0b0b0c", "222225", "17171a"),
           "results.js": ink_js("#f4f4f2", "#96969c", "#1f1f22", "#404046", "#0b0b0c")},
}
