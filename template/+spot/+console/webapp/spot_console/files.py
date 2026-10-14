"""The folder browser (choosing StreamData's output folder).

Part of the SPOT Console (webapp/app.py starts it).
"""

import os
from pathlib import Path

from .common import ApiError, PROJECT_DIR
from .events import settings
from .phasespace import WSL_INFO, windows_home


_windows_home = []  # cached: [path or None]


def places():
    out = [{"label": "Project Output", "path": str(PROJECT_DIR / "output")},
           {"label": "Project", "path": str(PROJECT_DIR)},
           {"label": "Home", "path": str(Path.home())}]
    if WSL_INFO:
        if not _windows_home:
            _windows_home.append(windows_home())
        if _windows_home[0]:
            out.append({"label": "Windows Home", "path": _windows_home[0]})
        for d in sorted(Path("/mnt").glob("[a-z]")):
            if d.is_dir():
                out.append({"label": "%s: (Windows)" % d.name.upper(), "path": str(d), "drive": True})
    for name in ("Desktop", "Documents"):
        p = Path.home() / name
        if p.is_dir():
            out.append({"label": name, "path": str(p)})
    out.append({"label": "/", "path": "/", "drive": True})
    return out


def browse(path):
    p = Path(path or settings.get("output_dir")).expanduser()
    if not p.is_absolute():
        p = Path.home() / p
    while not p.is_dir() and p.parent != p:  # fall back to the nearest folder that exists
        p = p.parent
    dirs, error = [], None
    try:
        with os.scandir(p) as it:
            for e in it:
                try:
                    if e.is_dir() and not e.name.startswith((".", "$")):
                        dirs.append(e.name)
                except OSError:
                    pass
    except OSError as e:
        error = str(e)
    dirs.sort(key=str.lower)
    return {"path": str(p), "parent": str(p.parent) if p.parent != p else None, "dirs": dirs,
            "sep": os.sep, "error": error, "places": places()}


# --------------------------------------------------------------------------- HTTP


def make_folder(body):
    parent, name = Path(str(body.get("parent", ""))), str(body.get("name", "")).strip()
    if not name or any(c in name for c in '/\\:*?"<>|') or name in (".", ".."):
        raise ApiError("That is not a valid folder name.")
    if not parent.is_dir():
        raise ApiError("%s is not a folder." % parent)
    target = parent / name
    try:
        target.mkdir()
    except FileExistsError:
        pass
    except OSError as e:
        raise ApiError("Could not create %s: %s" % (target, e))
    return {"path": str(target)}
