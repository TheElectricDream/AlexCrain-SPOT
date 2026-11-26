# The Console's Code

The SPOT Console is a small web application: a Python server that needs only the standard library, and a page of plain JavaScript modules. One program, `+interface/+console/webapp/app.py`, is both consoles: started by `SPOT` in MATLAB with `--local` it is the **local console**; started on the TX2 by its service it is the **lab console**. Every file has its page in the Reference (Console Server, Console Page).

## The server (`webapp/spot_console/`)

One Python module per area. `server.py` serves the page and the JSON API, routes each request to the module that handles it, and starts the right console; `features()` says which tabs a console offers.

| Module | What it does |
|---|---|
| [server.py](file:server.py) | the HTTP server: the page, the API (`do_GET`, `do_POST`, `_local_post`), the event stream, start-up |
| [common.py](file:common.py) | project paths, the API error (`ApiError`, shown to the user), checking request values |
| [events.py](file:events.py) | the lab console's settings, the event hub that feeds every open page, running commands |
| [local.py](file:local.py) | the local console: the bridge to the MATLAB worker, simulation and build runs, the link to the lab console |
| [phasespace.py](file:phasespace.py) | compiling and running StreamData, the packet monitor for the 3D view, the marker file |
| [hardware.py](file:hardware.py) | the platforms' Teensies over SSH: pucks, thrusters, checks |
| [control.py](file:control.py) | the template diagram's LQR controller and thruster mixer, ported from `spot.control` |
| [interactive.py](file:interactive.py) | Interactive Mode: flying platforms to goals |
| [experiment.py](file:experiment.py) | deploying, starting, stopping and collecting experiments; the platforms' clocks, messages and streams |
| [calibration.py](file:calibration.py), [calfit.py](file:calfit.py) | the calibrations and their fits; the calibration file |
| [design.py](file:design.py) | the Spacecraft Designer's design |
| [vision.py](file:vision.py) | where SPOT Vision is, for the Vision tab |
| [deploy.py](file:deploy.py) | Lab Setup: the deployment scripts, run from the page |
| [files.py](file:files.py) | the folder browser |
| [tabs.py](file:tabs.py) | the project's own tabs (`tabs/*/tab.json`): their server modules, routes and files |
| [themes.py](file:themes.py) | the page's colour themes: `/theme/<theme>.css`, the page's stylesheets re-coloured for a theme, then the theme's own rules |
| [docs.py](file:docs.py), [docparse.py](file:docparse.py), [doclint.py](file:doclint.py) | this documentation: the index of the code, the source parser, the linters |

**The API.** The page reads with GET and acts with POST (JSON bodies, which other web sites can't send through the browser). [API Routes and MATLAB Commands](page:routes) lists every route with the code that handles it and the code that requests it. On the local console, POST requests need the session token, and `/api/matlab` passes a command to MATLAB.

**Events.** `GET /api/events` is a server-sent event stream: the server publishes each change (`hub.publish("stream_status", ...)`), and every open page receives it, so all pages show the same state. The local console relays the lab console's events, prefixed `lab_`.

**MATLAB.** The worker (`interface.worker.Worker`) connects to the local console on a second local port and answers one JSON command per line (`sim.run`, `settings.set`, `data.series`, ...); `local.py`'s `MATLAB_COMMANDS` lists them with their time limits, and `Worker.dispatch` runs them.

## The page (`webapp/static/`)

`index.html` holds every tab's markup; `app.js` connects to the server, receives the events and shows the tabs; each tab is a module that `app.js` starts (`initSimulation`, `initExperiment`, ...), with `show()` and `hide()` and an `on...` handler for each event it follows.

| Module | Tab or part |
|---|---|
| [app.js](file:app.js) | the shell: state, the API call, events, the PhaseSpace tab's cards, switching tabs |
| [sim.js](file:sim.js), [layout.js](file:layout.js), [data.js](file:data.js) | the Simulation tab, its Thruster and LED Layout window; loaded results |
| [experiment.js](file:experiment.js), [results.js](file:results.js) | the Experiment and Results tabs |
| [viewer.js](file:viewer.js), [cad.js](file:cad.js), [export3d.js](file:export3d.js) | the 3D view (three.js), the CAD models, and saving a model as .glb or .stl |
| [designer.js](file:designer.js), [hardware.js](file:hardware.js), [interactive.js](file:interactive.js) | the Designer, Hardware and Interactive Mode |
| [calibration.js](file:calibration.js), [calviz.js](file:calviz.js) | the calibrations and their figures |
| [vision.js](file:vision.js), [labsetup.js](file:labsetup.js) | the Vision tab, Lab Setup |
| [liveplot.js](file:liveplot.js), [messages.js](file:messages.js) | the Live Plot, the Messages panel |
| [docs.js](file:docs.js) | this tab |
| `platforms.js`, `ratelock.js` | the platforms' names and colours; the padlocks on the rates |
| [theme.js](file:theme.js), `themes/` | the Theme menu under the logo, the theme's colours for the scripts (`themeColor`); each theme's entry in `themes.json` and its own stylesheet |
| `srl_logo.png`, `srl_logo_full.png` | the lab's logo: the page's 128 px copy (top bar and tab icon), and the full-size original it is made from |

Third-party code is in `static/vendor/`: three.js for the 3D view, and CodeMirror, ESLint, marked and KaTeX for this tab (rebuilt with `webapp/tools/editor-bundle/`). The Flight Deck theme's typefaces are in `static/vendor/fonts/` (B612 and IBM Plex Mono, both under the SIL Open Font License, with their licences), since the console has to work without the internet.

## Adding to it

- **A new API route:** handle it in `server.py`'s `do_GET` or `do_POST` (or `_local_post` for the local console), in the module of its area, raising `ApiError` for anything the user should be told; call it from the page with `api('/api/...', body)`.
- **A new MATLAB command:** a `case` in `Worker.dispatch`, its name and time limit in `local.py`'s `MATLAB_COMMANDS`, and from the page `api('/api/matlab', { cmd, args })`.
- **A new tab:** its markup in `index.html` (a `data-page` button and a `page` element), a module with `init...` returning `show`/`hide`, and an entry in `app.js`'s `PAGES`, `showPage` and `server.py`'s `features()`.
- **A new theme:** an entry in `static/themes/themes.json` (its `name`, `about`, a `ramp` of `[lightness, colour]` stops for the greys and an `accent`) and a stylesheet `static/themes/<theme>.css` with the rules that are the theme's own. Nothing else: the Theme menu lists it, and [themes.py](file:themes.py) makes its stylesheet from `app.css` and `docs.css` as they are (their greys onto the ramp by lightness, their accent blues onto the accent, every other hue and black shadows kept), followed by the theme's rules, so they win over the page's rules with the same selector. A rule added to `app.css` is therefore themed without being copied. `index.html` loads the stylesheet before the page is drawn and sets `data-theme` on `<html>`. The colours the scripts draw with are custom properties the theme may set (`--scene-bg`, `--scene-floor`, `--scene-grid`, `--scene-grid-2` for the 3D scene; `--plot-text`, `--plot-muted`, `--plot-grid`, `--plot-axis`, `--plot-surface`, `--plot-select` for the plots), read with `themeColor(name, fallback)`. The page's own look (Default) is `app.css` and `docs.css` untouched; keep theme-only rules out of them.
- **A project's own tab** (local console), without changing the console: a folder `tabs/<name>/` in the project with a `tab.json` (`id`, `title`, `tooltip`, `server`, `script`, `style`). [tabs.py](file:tabs.py) imports its server module and routes `/api/tab/<id>/...` to its `get`/`post`; `app.js` adds its button and page and calls its script's `init({api, act, toast, storage, root, showPage})`, which returns `show`/`hide`. Its MATLAB commands are `"<package>.<name>"`: `Worker.dispatch` passes them to that package's `consoleCommand(name, args, root)`. A command sent without a time limit counts as a long one (`local.py`). A tab sends MATLAB commands only when the person asks for something, never on its own (when MATLAB connects, say): the console has one link to MATLAB, and while a tab's command holds it the page's own requests give up after 6 s, so the page doesn't finish loading. The MATLAB path is the tab's to set inside its own commands (a diagram opened without its System objects on the path loads those blocks without their settings).
- Follow [The Coding Standard](page:dev-code-standard); the lab console runs Python 3.6. Deploy a change to the lab console with Lab Setup.
