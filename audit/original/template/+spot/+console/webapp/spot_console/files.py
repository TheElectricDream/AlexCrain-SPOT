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
    """Return the shortcuts shown beside the folder list.

    Returns:
        A list of {"label", "path"} dicts; drives also have
        "drive": True. Under WSL, the Windows home and drives are
        included.
    """
    # The project's folders and the home folder.
    out = [{"label": "Project Output", "path": str(PROJECT_DIR / "output")},
           {"label": "Project", "path": str(PROJECT_DIR)},
           {"label": "Home", "path": str(Path.home())}]

    # Under WSL: the Windows home (looked up once) and each drive.
    if WSL_INFO:
        if not _windows_home:
            _windows_home.append(windows_home())
        if _windows_home[0]:
            out.append({"label": "Windows Home", "path": _windows_home[0]})
        for drive in sorted(Path("/mnt").glob("[a-z]")):
            if drive.is_dir():
                out.append({"label": "%s: (Windows)" % drive.name.upper(),
                            "path": str(drive), "drive": True})

    # Desktop and Documents, where they exist, and the root.
    for name in ("Desktop", "Documents"):
        folder = Path.home() / name
        if folder.is_dir():
            out.append({"label": name, "path": str(folder)})
    out.append({"label": "/", "path": "/", "drive": True})
    return out


def browse(path):
    """List the subfolders of a folder.

    Args:
        path: The folder; empty for the settings' output folder.
            Relative paths are taken from the home folder, and a
            missing folder falls back to its nearest existing parent.

    Returns:
        A dict with path, parent (None at the root), dirs (names,
        hidden and $ folders left out), sep, error (None, or why the
        folder couldn't be read) and places.
    """
    # The folder, made absolute; the nearest one that exists.
    folder = Path(path or settings.get("output_dir")).expanduser()
    if not folder.is_absolute():
        folder = Path.home() / folder
    while not folder.is_dir() and folder.parent != folder:
        folder = folder.parent

    # Its visible subfolders, sorted case-insensitively.
    dirs, error = [], None
    try:
        with os.scandir(folder) as it:
            for e in it:
                try:
                    if e.is_dir() and not e.name.startswith((".", "$")):
                        dirs.append(e.name)
                except OSError:
                    pass
    except OSError as e:
        error = str(e)
    dirs.sort(key=str.lower)

    return {"path": str(folder),
            "parent": str(folder.parent) if folder.parent != folder else None,
            "dirs": dirs, "sep": os.sep, "error": error, "places": places()}


# HTTP


def make_folder(body):
    """Create a folder (POST from the folder browser).

    Args:
        body: The request: parent (the folder to create it in) and
            name (a single folder name).

    Returns:
        {"path": the new folder}; an existing folder is fine.

    Raises:
        ApiError: if the name or the parent isn't valid, or the folder
            can't be created.
    """
    parent = Path(str(body.get("parent", "")))
    name = str(body.get("name", "")).strip()

    # A single, valid folder name in an existing folder.
    if not name or any(c in name for c in '/\\:*?"<>|') or name in (".", ".."):
        raise ApiError("That is not a valid folder name.")
    if not parent.is_dir():
        raise ApiError("%s is not a folder." % parent)

    # Create it; it may already exist.
    target = parent / name
    try:
        target.mkdir()
    except FileExistsError:
        pass
    except OSError as e:
        raise ApiError("Could not create %s: %s" % (target, e))
    return {"path": str(target)}
