# The Console's Code

The SPOT Console is a small web application: a Python server that needs only the standard library, and a page of plain JavaScript modules. One program, `+spot/+console/webapp/app.py`, is both consoles: started by `SPOT` in MATLAB with `--local` it is the **local console**; started on the TX2 by its service it is the **lab console**. Every file has its page in the Reference (Console Server, Console Page).

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
| [docs.py](file:docs.py), [docparse.py](file:docparse.py), [doclint.py](file:doclint.py) | this documentation: the index of the code, the source parser, the linters |

**The API.** The page reads with GET and acts with POST (JSON bodies, which other web sites can't send through the browser). [API Routes and MATLAB Commands](page:routes) lists every route with the code that handles it and the code that requests it. On the local console, POST requests need the session token, and `/api/matlab` passes a command to MATLAB.

**Events.** `GET /api/events` is a server-sent event stream: the server publishes each change (`hub.publish("stream_status", ...)`), and every open page receives it, so all pages show the same state. The local console relays the lab console's events, prefixed `lab_`.

**MATLAB.** The worker (`spot.worker.Worker`) connects to the local console on a second local port and answers one JSON command per line (`sim.run`, `settings.set`, `data.series`, ...); `local.py`'s `MATLAB_COMMANDS` lists them with their time limits, and `Worker.dispatch` runs them.

## The page (`webapp/static/`)

`index.html` holds every tab's markup; `app.js` connects to the server, receives the events and shows the tabs; each tab is a module that `app.js` starts (`initSimulation`, `initExperiment`, ...), with `show()` and `hide()` and an `on...` handler for each event it follows.

| Module | Tab or part |
|---|---|
| [app.js](file:app.js) | the shell: state, the API call, events, the PhaseSpace tab's cards, switching tabs |
| [sim.js](file:sim.js), [layout.js](file:layout.js), [data.js](file:data.js) | the Simulation tab, its Thruster and LED Layout window; loaded results |
| [experiment.js](file:experiment.js), [results.js](file:results.js) | the Experiment and Results tabs |
| [viewer.js](file:viewer.js), [cad.js](file:cad.js) | the 3D view (three.js) and the CAD models |
| [designer.js](file:designer.js), [hardware.js](file:hardware.js), [interactive.js](file:interactive.js) | the Designer, Hardware and Interactive Mode |
| [calibration.js](file:calibration.js), [calviz.js](file:calviz.js) | the calibrations and their figures |
| [vision.js](file:vision.js), [labsetup.js](file:labsetup.js) | the Vision tab, Lab Setup |
| [liveplot.js](file:liveplot.js), [messages.js](file:messages.js) | the Live Plot, the Messages panel |
| [docs.js](file:docs.js) | this tab |
| `platforms.js`, `ratelock.js` | the platforms' names and colours; the padlocks on the rates |
| `srl_logo.png`, `srl_logo_full.png` | the lab's logo: the page's 128 px copy (top bar and tab icon), and the full-size original it is made from |

Third-party code is in `static/vendor/`: three.js for the 3D view, and CodeMirror, ESLint, marked and KaTeX for this tab (rebuilt with `webapp/tools/editor-bundle/`).

## Adding to it

- **A new API route:** handle it in `server.py`'s `do_GET` or `do_POST` (or `_local_post` for the local console), in the module of its area, raising `ApiError` for anything the user should be told; call it from the page with `api('/api/...', body)`.
- **A new MATLAB command:** a `case` in `Worker.dispatch`, its name and time limit in `local.py`'s `MATLAB_COMMANDS`, and from the page `api('/api/matlab', { cmd, args })`.
- **A new tab:** its markup in `index.html` (a `data-page` button and a `page` element), a module with `init...` returning `show`/`hide`, and an entry in `app.js`'s `PAGES`, `showPage` and `server.py`'s `features()`.
- Follow [The Coding Standard](page:dev-code-standard); the lab console runs Python 3.6. Deploy a change to the lab console with Lab Setup.
