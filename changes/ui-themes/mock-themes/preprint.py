"""PREPRINT: the console set like the lab's papers.

What the lab ships is journal articles, so the interface is typeset as one: Computer Modern (the
KaTeX fonts already vendored in static/vendor/katex, so nothing new to install), numbered
sections instead of cards, booktabs rules, the 3D view as a figure on white paper, black ink
with Carleton red kept for the one thing that is selected.
"""
from _common import viewer_js, ink_js

K = "/static/vendor/katex/fonts/KaTeX_"
FONTS = "".join(
    "@font-face { font-family: '%s'; src: url('%s%s.woff2') format('woff2'); font-weight: %s; font-style: %s; font-display: block; }\n"
    % row for row in [
        ("CM", K, "Main-Regular", "400", "normal"), ("CM", K, "Main-Italic", "400", "italic"),
        ("CM", K, "Main-Bold", "600 700", "normal"), ("CM", K, "Main-BoldItalic", "600 700", "italic"),
        ("CM Sans", K, "SansSerif-Regular", "400 500", "normal"), ("CM Sans", K, "SansSerif-Bold", "600 700", "normal"),
        ("CM Mono", K, "Typewriter-Regular", "400 700", "normal")])

CSS = r"""
:root {
  --bg: #fbfaf5; --panel: #fbfaf5; --panel-2: #f1efe7; --border: #c9c5b8; --border-strong: #1a1a1a;
  --text: #141414; --muted: #5a574f; --accent: #c8102e; --accent-hover: #e11d3c;
  --go: #141414; --go-hover: #333; --stop: #c8102e; --stop-hover: #e11d3c;
  --warn: #9a5b00; --ok: #1b6e3c; --radius: 0px; --radius-sm: 0px;
  --sans: "CM", "Latin Modern Roman", Georgia, serif; --mono: "CM Mono", ui-monospace, monospace;
  --paper: #fbfaf5; --ink: #141414;
  color-scheme: light;
}
body { font: 400 15.5px/1.35 var(--sans); background: var(--paper); color: var(--ink); -webkit-font-smoothing: auto; }
* { scrollbar-color: #b9b5a8 transparent; }
.mono { font-size: 13px; }

/* ----- title block ----- */
.topbar { background: var(--paper); border-bottom: 2px solid var(--ink); box-shadow: 0 3px 0 var(--paper), 0 3.5px 0 var(--ink); padding: 10px 22px 9px; margin-bottom: 4px; z-index: 5; position: relative; }
.brand { flex: none; }
.brand h1 { font: 700 19px/1.1 var(--sans); letter-spacing: 0; }
.brand p { font: italic 400 13.5px var(--sans); color: #3a3832; }
.brand-logo { width: 38px; height: 38px; }
.topbar .page-tabs { background: transparent; border: 0; padding: 0; gap: 0; margin-left: 14px; }
.page-tabs button { font: 400 15px var(--sans); font-variant: small-caps; letter-spacing: .03em; color: #3a3832; padding: 6px 9px; border-radius: 0; position: relative; }
.page-tabs button + button::before { content: "·"; position: absolute; left: -3px; color: #8a877c; }
.page-tabs button:hover { color: var(--accent); }
.page-tabs button.active { background: transparent; color: var(--ink); font-weight: 700; box-shadow: inset 0 -2px 0 var(--accent); }
.settings-bar { background: transparent; border: 0; border-left: .5px solid var(--ink); border-radius: 0; padding-left: 14px; }
.sb-title, .stat-label { font: 400 12.5px var(--sans); font-variant: small-caps; letter-spacing: .08em; text-transform: none; color: #3a3832; }
.sb-file { color: var(--ink); font-size: 13px; }
.badge { border-radius: 0; background: transparent; border: 0; border-bottom: 1px solid currentColor; padding: 2px 2px; font: italic 400 14px var(--sans); }
.badge .dot { width: 7px; height: 7px; }

/* ----- sections, not cards ----- */
.sidebar, .msg-slot, .sim-run { background: var(--paper); }
.sidebar, .sim-panes { counter-reset: sec; }
.sidebar { border-right: .5px solid var(--ink); gap: 22px; }
.sim-panes > div { gap: 22px; }
.card { background: transparent; border: 0; border-top: 1.5px solid var(--ink); border-radius: 0; box-shadow: none; padding: 8px 0 0; counter-increment: sec; }
.card-head { margin-bottom: 10px; padding-bottom: 6px; border-bottom: .5px solid var(--ink); }
.card-head h2 { font: 700 16.5px var(--sans); text-transform: none; letter-spacing: 0; color: var(--ink); }
.card-head h2::first-letter { text-transform: uppercase; }
.card-head h2::before { content: counter(sec) "\2003"; }
.field-label { font: 400 13.5px var(--sans); font-variant: small-caps; letter-spacing: .05em; color: #3a3832; margin-bottom: 4px; }
.hint { font: italic 400 13.5px/1.3 var(--sans); color: #4a473f; }
.msg-head h2 { font: 700 16.5px var(--sans); text-transform: none; letter-spacing: 0; color: var(--ink); font-variant: normal; }

/* ----- controls: ruled, square, ink ----- */
.btn { border-radius: 0; background: var(--paper); border: 1px solid var(--ink); color: var(--ink); font: 400 14.5px var(--sans); font-variant: small-caps; letter-spacing: .04em; }
.btn:hover { background: var(--ink); color: var(--paper); border-color: var(--ink); }
.btn-sm { font-size: 13.5px; }
.btn-ghost { border-color: transparent; }
.btn-primary { background: var(--ink); color: var(--paper); border-color: var(--ink); }
.btn-primary:hover { background: var(--accent); border-color: var(--accent); }
.btn-go { background: var(--ink); border-color: var(--ink); color: var(--paper); font: 700 16px var(--sans); font-variant: small-caps; letter-spacing: .06em; }
.btn-go:hover { background: var(--accent); border-color: var(--accent); color: #fff; }
.btn-stop { background: var(--accent); border-color: var(--accent); color: #fff; }
.input { border-radius: 0; background: transparent; border: 0; border-bottom: 1px solid var(--ink); color: var(--ink); font: 400 15px var(--sans); padding: 0 4px; height: 30px; }
.input.mono { font-family: var(--mono); font-size: 13.5px; }
select.input { border: 1px solid var(--ink); padding: 0 8px; height: 32px; }
.input:focus { border-color: var(--accent); box-shadow: 0 1px 0 var(--accent); }
.input-suffix span { font-style: italic; color: #3a3832; font-size: 13.5px; }
.segmented { background: transparent; border: 0; border-bottom: .5px solid var(--ink); border-radius: 0; padding: 0; }
.segmented button { border-radius: 0; font: 400 14.5px var(--sans); font-variant: small-caps; letter-spacing: .05em; color: #4a473f; }
.segmented button.active { background: transparent; color: var(--ink); font-weight: 700; box-shadow: inset 0 -2px 0 var(--accent); }
.viewer-toolbar .segmented { border-bottom: 0; }
.toggle { color: var(--ink); font-size: 14px; }
.toggle > input + span { border-radius: 0; background: var(--paper); box-shadow: inset 0 0 0 1px var(--ink); }
.toggle > input + span::after { border-radius: 0; background: var(--ink); }
.toggle input:checked + span { background: var(--ink); }
.toggle input:checked + span::after { background: var(--paper); }
.pill { border-radius: 0; background: transparent; border: 0; padding: 0; font: italic 400 14px var(--sans); color: #3a3832; }
.pill.live { color: var(--ok); background: transparent; }
.pill.err { color: var(--accent); background: transparent; }
.plat-chip { border-radius: 0 !important; background: transparent; font: 400 14.5px var(--sans); font-variant: small-caps; letter-spacing: .05em; color: #6b685f; border-color: #8a877c; }
.plat-chip.on { background: color-mix(in srgb, var(--c) 22%, var(--paper)); color: var(--ink); font-weight: 700; }
.count { border-radius: 0; background: transparent; color: #3a3832; font: italic 400 13px var(--sans); }
.count::before { content: "("; } .count::after { content: ")"; }

/* ----- timeline: a small table ----- */
.timeline-bar { border-radius: 0; border: 1px solid var(--ink); }
.tl-swatch { border-radius: 0; width: 6px; }
.tl-name b { font-weight: 700; font-size: 15px; }
.tl-name span { font-style: italic; font-size: 14.5px; color: #3a3832; }
.tl-when { font: 400 12.5px var(--mono); }
.tl-sum { color: var(--ink); }

/* ----- the 3D view is Figure 1 ----- */
.viewer { background: #ffffff; border-bottom: .5px solid var(--ink); }
#viewer[data-showing="simulation"]::after { content: "Figure 1. The 3.51 m × 2.42 m granite table, with each platform's simulated path."; bottom: 78px; }
.viewer::after { content: "Figure 1. The 3.51 m × 2.42 m granite table."; position: absolute; left: 18px; bottom: 14px; z-index: 2;
  font: italic 400 14px var(--sans); color: #3a3832; pointer-events: none; }
.viewer-toolbar, .legend, .viewer-empty, .playback { border-radius: 0; background: rgba(251,250,245,.96); border: 1px solid var(--ink); backdrop-filter: none; box-shadow: none; }
.legend { color: var(--ink); }
.pb-time { color: var(--ink); font-family: var(--mono); }
.pb-phase { color: var(--accent); font: italic 700 14.5px var(--sans); }
.pb-phases i { background: rgba(0,0,0,.45); }
.pb-track input { accent-color: var(--ink); }
.craft-label { border-radius: 0; font: 400 12.5px var(--sans); font-variant: small-caps; letter-spacing: .05em; box-shadow: none; outline: .5px solid var(--ink); }
.tick-label { color: #3a3832; text-shadow: 0 0 2px #fff, 0 0 2px #fff; }
.origin-label { color: var(--ink); background: rgba(255,255,255,.85); outline: .5px solid var(--ink); border-radius: 0; }
.axis-label { text-shadow: 0 0 2px #fff, 0 0 3px #fff; }

/* ----- listing and messages ----- */
.console { background: var(--paper); border-top: 1.5px solid var(--ink); }
.console-head { border-bottom: .5px solid var(--ink); }
.tabs button { font: 400 15px var(--sans); font-variant: small-caps; letter-spacing: .05em; color: #4a473f; }
.tabs button.active { color: var(--ink); font-weight: 700; border-bottom-color: var(--accent); }
.log { font: 400 13.5px/1.45 var(--mono); color: #26251f; background: #f4f2ea; }
.msg-slot { border-left: .5px solid var(--ink); }
.msg-head { border-bottom: 1.5px solid var(--ink); }
.msg-plat { border-bottom: .5px solid var(--ink); }
.msg-plat-head b { font: 700 15.5px var(--sans); font-variant: small-caps; letter-spacing: .05em; }
.msg-plat-head .swatch { border-radius: 0; outline: .5px solid var(--ink); }
.msg-now { color: var(--ink); font-style: italic; }
.msg-list { font-size: 13px; }
.settings-bar .btn, .topstats .btn { padding: 0 7px; font-size: 13px; }
.topbar { gap: 8px; padding-left: 16px; padding-right: 16px; }
.topstats { gap: 8px; }
.settings-bar { gap: 5px; padding-left: 10px; }
.sb-file { max-width: 120px; }
.msg-list li { color: #26251f; border-radius: 0; padding-top: 1px; padding-bottom: 1px; }
.msg-list li.future { color: #a09d92; }
.msg-list li.current { background: transparent; color: var(--ink); font-weight: 700; box-shadow: inset 0 -1px 0 var(--accent); }
.msg-t { font-family: var(--mono); font-size: 12.5px; color: #4a473f; }
.dialog { border-radius: 0; border: 1px solid var(--ink); box-shadow: 6px 6px 0 rgba(20,20,20,.18); }
"""

THEME = {
    "light": True,
    "ramp": [(0.0, "#ffffff"), (0.055, "#fbfaf5"), (0.092, "#fbfaf5"), (0.125, "#f1efe7"),
             (0.18, "#c9c5b8"), (0.24, "#8a877c"), (0.41, "#8a877c"), (0.59, "#5a574f"),
             (0.80, "#3a3832"), (0.92, "#141414"), (1.0, "#000000")],
    "accent": "#c8102e",
    "shadow": "#141414", "shadow_k": 0.3,
    "css": CSS,
    "fontface": FONTS,
    "js": {"viewer.js": viewer_js("ffffff", "f6f5f0", "d5d2c6", "e6e3d9"),
           "results.js": ink_js("#141414", "#5a574f", "#e2dfd4", "#141414", "#ffffff"),
           "platforms.js": [("LINE_COLORS = { RED: '#e9ecef'", "LINE_COLORS = { RED: '#6c757d'")]},
}
