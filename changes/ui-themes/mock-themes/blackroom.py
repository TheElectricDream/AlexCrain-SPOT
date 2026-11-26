"""BLACKROOM: the lab itself.

The SPOT room is matte black block walls around a grey-green granite slab, lit by a row of warm
windows. So: a granite header bar on a black shell (the flat, dense, desktop-tool build of
PartyPlanner64, whose tan bar sits on dark brown the same way), aluminium hairlines, and
"lit window" cream for whatever is selected or about to run.
"""
from _common import face, svg, viewer_js, ink_js

SPECKLE = svg(
    "<filter id='n' x='0' y='0' width='100%' height='100%'>"
    "<feTurbulence type='fractalNoise' baseFrequency='1.15' numOctaves='2' seed='7'/>"
    "<feColorMatrix values='0 0 0 0 0.06  0 0 0 0 0.07  0 0 0 0 0.06  -1.6 -1.6 -1.6 0 2.55'/></filter>"
    "<filter id='w' x='0' y='0' width='100%' height='100%'>"
    "<feTurbulence type='fractalNoise' baseFrequency='0.8' numOctaves='1' seed='3'/>"
    "<feColorMatrix values='0 0 0 0 1  0 0 0 0 1  0 0 0 0 0.97  1.9 1.9 1.9 0 -3.35'/></filter>"
    "<rect width='240' height='240' filter='url(#n)' opacity='.55'/>"
    "<rect width='240' height='240' filter='url(#w)' opacity='.5'/>", "0 0 240 240")

# Cinder blocks: 40 x 20 running bond, barely there.
BLOCKS = svg("<path d='M0 .5H80M0 20.5H80M.5 0V20M40.5 20V40' stroke='#ffffff' stroke-opacity='.035' fill='none'/>",
             "0 0 80 40")

CSS = r"""
:root {
  --bg: #111112; --panel: #1b1b1c; --panel-2: #262627; --border: #333335; --border-strong: #4a4a4d;
  --text: #ecebe6; --muted: #98978f; --accent: #f1dfae; --accent-hover: #fbeecb;
  --go: #f1dfae; --go-hover: #fbeecb; --stop: #e0483c; --stop-hover: #ee5c50;
  --warn: #f0a63a; --ok: #6fcf8e; --radius: 2px; --radius-sm: 2px;
  --sans: "Source Sans 3", system-ui, sans-serif; --mono: "IBM Plex Mono", ui-monospace, monospace;
  --lit: #f1dfae; --alu: #c5c8cc; --granite: #8d948e;
}
body { font: 400 14.5px/1.4 var(--sans); background: #111112; }

/* ----- the slab: a granite bar across the top of a black room ----- */
.topbar {
  background: SPECKLE 0 0/240px, linear-gradient(180deg, #99a09a, #858c86); color: #0c0c0c;
  border-bottom: 1px solid #000; padding: 7px 14px; box-shadow: inset 0 1px 0 rgba(255,255,255,.35), inset 0 -3px 0 rgba(0,0,0,.18);
}
.brand h1 { font: 700 18px/1.1 var(--sans); color: #0c0c0c; letter-spacing: -.005em; }
.brand p { color: #23261f; font: 500 11.5px var(--mono); }
.brand-logo { width: 38px; height: 38px; }
.topbar .page-tabs { background: transparent; border: 0; padding: 0; gap: 2px; box-shadow: none; }
.brand { flex: none; }
.topbar .page-tabs button { font: 600 14px var(--sans); color: #171915; padding: 9px 14px; border-radius: 2px; }
.topbar .page-tabs button:hover { background: rgba(0,0,0,.2); }
.topbar .page-tabs button.active { background: #111112; color: var(--lit); box-shadow: inset 0 -2px 0 var(--lit); }
.settings-bar { background: rgba(0,0,0,.16); border: 1px solid rgba(0,0,0,.35); border-radius: 2px; }
.sb-title, .stat-label { color: #262a24; font-weight: 700; }
.sb-file { color: #0c0c0c; }
.topbar .btn { background: rgba(0,0,0,.72); border-color: #000; color: #ecebe6; }
.topbar .btn:hover { background: #000; }
.badge { border-radius: 2px; background: #111112; border-color: #000; font-weight: 600; }

/* ----- the room ----- */
.sidebar, .msg-slot, .sim-run { background: BLOCKS 0 0/80px 40px, #141415; }
.sidebar { border-right: 1px solid #000; box-shadow: inset -1px 0 0 #2a2a2c; }
.msg-slot { border-left: 1px solid #000; box-shadow: inset 1px 0 0 #2a2a2c; }
.card { border-radius: 2px; background: #1d1d1e; border: 1px solid #333335; box-shadow: 0 1px 0 #000, inset 0 1px 0 rgba(255,255,255,.04); padding: 12px 12px 14px; }
.card-head { margin: -12px -12px 12px; padding: 8px 12px; background: #262627; border-bottom: 1px solid #000; box-shadow: inset 0 -1px 0 rgba(197,200,204,.22); }
.card-head h2, .msg-head h2 { font: 700 12.5px var(--sans); letter-spacing: .13em; color: var(--alu); }
.field-label { font-size: 12.5px; color: #a4a39b; }

/* ----- controls: flat and square; what is selected is a lit window ----- */
.btn { border-radius: 2px; background: #2b2b2d; border-color: #000; box-shadow: inset 0 1px 0 rgba(255,255,255,.07); font: 600 13px var(--sans); }
.btn:hover { background: #38383a; border-color: #000; }
.btn-sm { font-size: 12.5px; }
.btn-ghost { background: transparent; border-color: transparent; box-shadow: none; }
.btn-primary { background: var(--lit); border-color: #000; color: #15130c; box-shadow: 0 0 14px rgba(241,223,174,.22); }
.btn-primary:hover { background: #fbeecb; border-color: #000; }
.btn-go { background: var(--lit); border-color: #000; color: #15130c; font: 700 15px var(--sans); letter-spacing: .02em; box-shadow: 0 0 22px rgba(241,223,174,.28), inset 0 1px 0 rgba(255,255,255,.6); }
.btn-go:hover { background: #fbeecb; border-color: #000; }
.btn-stop { border-color: #000; color: #fff; }
.input { border-radius: 2px; background: #0c0c0d; border-color: #000; box-shadow: inset 0 0 0 1px #2e2e30; }
.input:focus { border-color: #000; box-shadow: inset 0 0 0 1px var(--lit), 0 0 10px rgba(241,223,174,.18); }
.segmented { border-radius: 2px; background: #0c0c0d; border-color: #000; box-shadow: inset 0 0 0 1px #2a2a2c; padding: 3px; }
.segmented button { border-radius: 1px; font: 600 12.5px var(--sans); }
.segmented button.active { background: var(--lit); color: #15130c; box-shadow: 0 0 12px rgba(241,223,174,.25); }
.page-tabs button.active { background: #111112; color: var(--lit); box-shadow: inset 0 -2px 0 var(--lit); }
.toggle > input + span { border-radius: 2px; background: #0c0c0d; box-shadow: inset 0 0 0 1px #3a3a3c; }
.toggle > input + span::after { border-radius: 1px; background: #6a6a6c; }
.toggle input:checked + span { background: #4a432c; box-shadow: inset 0 0 0 1px var(--lit); }
.toggle input:checked + span::after { background: var(--lit); box-shadow: 0 0 8px rgba(241,223,174,.6); }
.pill { border-radius: 2px; background: #0c0c0d; border-color: #3a3a3c; font-family: var(--mono); font-size: 11.5px; }
.pill.live { color: #06150d; }
.plat-chip { border-radius: 2px !important; font-weight: 700; }
.count { border-radius: 2px; background: #0c0c0d; color: var(--lit); box-shadow: inset 0 0 0 1px #3a3a3c; }

.timeline-bar { border-radius: 2px; border-color: #000; }
.tl-swatch { border-radius: 1px; }

/* ----- 3D view ----- */
.viewer { background: #0e0e0f; }
.viewer-toolbar, .legend, .viewer-empty, .playback { border-radius: 2px; background: rgba(27,27,28,.94); border: 1px solid #000; box-shadow: inset 0 0 0 1px #3a3a3c, 0 6px 18px rgba(0,0,0,.6); backdrop-filter: none; }
.pb-track input { accent-color: var(--lit); }
.pb-phase { color: var(--lit); font-size: 13px; }
.craft-label { border-radius: 2px; }

/* ----- console and messages ----- */
.console { background: #0c0c0d; border-top: 1px solid #000; }
.console-head { background: #1b1b1c; border-bottom: 1px solid #000; }
.tabs button { font: 600 13px var(--sans); }
.tabs button.active { color: var(--lit); border-bottom-color: var(--lit); }
.log .l-cmd { color: var(--lit); }
.msg-head { background: #262627; border-bottom: 1px solid #000; box-shadow: inset 0 -1px 0 rgba(197,200,204,.22); }
.msg-plat { border-bottom-color: #000; }
.msg-plat-head b { font-weight: 700; font-size: 14px; }
.msg-list li { border-radius: 1px; }
.msg-list li.current { background: #3a3523; color: #fff6dc; box-shadow: inset 2px 0 0 var(--lit); }
.dialog { border-radius: 3px; border-color: #000; box-shadow: inset 0 0 0 1px #4a4a4d, 0 24px 64px rgba(0,0,0,.7); }
""".replace("SPECKLE", SPECKLE).replace("BLOCKS", BLOCKS)

FONTS = (face("Source Sans 3", "SourceSans3.ttf", "200 900")
         + face("IBM Plex Mono", "PlexMono-Regular.ttf", "400") + face("IBM Plex Mono", "PlexMono-Medium.ttf", "500 700"))

THEME = {
    "ramp": [(0.0, "#000000"), (0.055, "#111112"), (0.09, "#1b1b1c"), (0.125, "#262627"),
             (0.18, "#333335"), (0.24, "#4a4a4d"), (0.41, "#6c6b66"), (0.59, "#98978f"),
             (0.80, "#cfcdc5"), (0.92, "#ecebe6"), (1.0, "#ffffff")],
    "accent": "#f1dfae",
    "accent_dark_k": 0.9,
    "css": CSS,
    "fontface": FONTS,
    "js": {"viewer.js": viewer_js("0e0e0f", "0c0c0d", "262627", "1a1a1b"),
           "results.js": ink_js("#ecebe6", "#98978f", "#262627", "#4a4a4d", "#141415")},
}
