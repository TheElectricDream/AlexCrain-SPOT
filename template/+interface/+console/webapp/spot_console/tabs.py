"""Project tabs: tabs a project adds to the local console (local console).

A project can add its own tabs without changing the console: a folder
tabs/<name>/ in the project with a tab.json,

    {"id": "evtomo", "title": "Event Tomography", "tooltip": "...",
     "server": "server.py", "script": "static/tab.js", "style": "static/tab.css"}

The server module (optional) is imported as a package (its folder), so it
can import its neighbours (from . import x); its open(context) returns the
tab's handler, with any of

    get(path, query)              GET  /api/tab/<id>/<path>: a dict (JSON)
                                  or (bytes, content type)
    post(path, body, requester)   POST /api/tab/<id>/<path>: a dict
    status()                      the tab's state in GET /api/tabs

context: root (the project, a Path), tab_dir, matlab (the MATLAB bridge:
request(cmd, args, timeout, label=...); a tab's MATLAB commands are
"<package>.<name>", run by that package's consoleCommand), lab (a function
returning the lab link: status(), request(method, path, body)), hub (the
event hub: publish, subscribe), ApiError.

The page loads each tab's script (an ES module) and calls its
init({api, act, toast, storage, root, showPage}), which returns
{show, hide}; its files are served from the tab's folder at
/tabs/<id>/<path> (static/ only). The template's own pages are never
changed by a tab.

Part of the SPOT Console (webapp/app.py starts it).
"""

import importlib.util
import json
import re
import sys
import traceback
from pathlib import Path

from .common import ApiError
from .events import hub

ID_RE = re.compile(r"^[a-z][a-z0-9_]{0,30}$")
# The console's own pages: a tab can't take their names.
RESERVED = {"simulation", "experiment", "results", "phasespace", "hardware", "designer",
            "calibration", "vision", "docs", "mode"}

_tabs = {}   # id -> {"meta", "dir", "handler", "error"}


def open_tabs(root, matlab, lab):
    """Find and start the project's tabs (tabs/*/tab.json).

    Args:
        root: The project folder.
        matlab: The MATLAB bridge.
        lab: A function returning the lab link (it is made later).
    """
    base = Path(root) / "tabs"
    if not base.is_dir():
        return
    for meta_file in sorted(base.glob("*/tab.json")):
        tab_dir = meta_file.parent
        try:
            meta = json.loads(meta_file.read_text(encoding="utf-8"))
            tab_id = str(meta.get("id", ""))
        except (OSError, ValueError) as e:
            hub.log("matlab", "Project tab %s: unreadable tab.json (%s)" % (tab_dir.name, e))
            continue
        if not ID_RE.match(tab_id) or tab_id in RESERVED or tab_id in _tabs:
            hub.log("matlab", "Project tab %s: the id %r can't be used" % (tab_dir.name, tab_id))
            continue
        entry = {"meta": meta, "dir": tab_dir, "handler": None, "error": None}
        _tabs[tab_id] = entry
        if not meta.get("server"):
            continue
        try:
            entry["handler"] = _load(tab_id, tab_dir, meta["server"]).open({
                "root": Path(root), "tab_dir": tab_dir, "matlab": matlab, "lab": lab,
                "hub": hub, "ApiError": ApiError})
        except Exception as e:   # a broken tab must not stop the console
            entry["error"] = "%s: %s" % (type(e).__name__, e)
            hub.log("matlab", "Project tab %s could not start: %s\n%s"
                    % (tab_id, entry["error"], traceback.format_exc()))


def _load(tab_id, tab_dir, server):
    """Import a tab's server module as a package named spot_tab_<id>."""
    path = (tab_dir / server).resolve()
    if tab_dir.resolve() not in path.parents:
        raise ApiError("The server module must be in the tab's folder.")
    name = "spot_tab_%s" % tab_id
    spec = importlib.util.spec_from_file_location(
        name, str(path), submodule_search_locations=[str(tab_dir)])
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def features():
    """Return {id: True} for each tab (the console's features)."""
    return {tab_id: True for tab_id in _tabs}


def listing():
    """Return the tabs for the page: id, title, tooltip, script, style, state."""
    out = []
    for tab_id, entry in _tabs.items():
        meta = entry["meta"]
        item = {"id": tab_id, "title": str(meta.get("title", tab_id)),
                "tooltip": str(meta.get("tooltip", "")), "error": entry["error"],
                "script": "/tabs/%s/%s" % (tab_id, meta["script"]) if meta.get("script") else None,
                "style": "/tabs/%s/%s" % (tab_id, meta["style"]) if meta.get("style") else None}
        handler = entry["handler"]
        if handler is not None and hasattr(handler, "status"):
            try:
                item["status"] = handler.status()
            except Exception as e:
                item["error"] = "%s: %s" % (type(e).__name__, e)
        out.append(item)
    return out


def static_file(rest):
    """Return the file for /tabs/<id>/<path> (in the tab's static/ folder), or None."""
    tab_id, _, rel = rest.partition("/")
    entry = _tabs.get(tab_id)
    if entry is None or not rel:
        return None
    static = (entry["dir"] / "static").resolve()
    target = (entry["dir"] / rel).resolve()
    if static != target and static not in target.parents:
        return None
    return target if target.is_file() else None


def _handler(rest, method):
    """Return (handler, sub-path) for /api/tab/<id>/<sub-path>."""
    tab_id, _, sub = rest.partition("/")
    entry = _tabs.get(tab_id)
    if entry is None:
        raise ApiError("No tab %r in this project." % tab_id)
    handler = entry["handler"]
    if handler is None or not hasattr(handler, method):
        raise ApiError("The %s tab has no such request%s."
                       % (tab_id, " (it failed to start: %s)" % entry["error"]
                          if entry["error"] else ""))
    return handler, sub


def get(rest, query):
    """Serve GET /api/tab/<id>/<path> (query: parse_qs's dict)."""
    handler, sub = _handler(rest, "get")
    return handler.get(sub, {k: v[-1] for k, v in query.items()})


def post(rest, body, requester):
    """Serve POST /api/tab/<id>/<path>."""
    handler, sub = _handler(rest, "post")
    return handler.post(sub, body, requester)
