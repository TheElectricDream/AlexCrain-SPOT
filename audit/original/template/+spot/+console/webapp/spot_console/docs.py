"""The Documentation tab: the index of the code, the guides, saving.

The index is built from the project's own files every time one
changes (docparse reads them), so the reference pages always match the
code. The guides and the notes on functions are Markdown files in
webapp/docs:

    docs/toc.json          the guides' sections and pages
    docs/guides/*.md       the guides
    docs/notes/<path>.md   notes on a source file's functions: text
                           before the first "## name" heading is
                           about the file, each "## name" section is
                           about that function (its name in the file,
                           e.g. "Handler.do_GET" or "cleanMessages")
    docs/images/           pictures the guides show

The page edits code and documentation through save(): only on the
local console (the request carries its token), only files the index
lists, never over a change made since the page read the file, and with
a backup of the old version in .spot_cache/docs_backups.

Part of the SPOT Console (webapp/app.py starts it).
"""

import gzip
import hashlib
import json
import os
import re
import shutil
import threading
import time
from pathlib import Path, PurePosixPath

from . import doclint, docparse
from .common import APP_DIR, PROJECT_DIR, ApiError
from .events import hub
from .local import LOCAL, matlab


DOCS_DIR = APP_DIR / "docs"
DOCS_PREFIX = "@docs/"      # paths of the documentation's own files
MAX_FILE_BYTES = 2000000    # [bytes] larger source files are left out
MAX_SAVE_BYTES = 4000000    # [bytes] largest file the editor may save
BACKUPS_KEPT = 40           # saves kept in .spot_cache/docs_backups
GZIP_LEVEL = 6

# Folders and files the reference leaves out: third-party code and the
# tools that bundle it, models, caches, this documentation (shown as
# guides) and backups.
SKIP_PARTS = ("vendor", "libowl-src-410", "models", "__pycache__", "docs", ".git",
              ".spot_cache", "output", "node_modules", "tools")
SKIP_SUFFIXES = (".bak", ".pyc", ".png", ".jpg", ".gz", ".slx", ".mat", ".so", ".o",
                 ".pdf", ".zip")

# The reference's groups, in order.
GROUPS = [
    ("matlab", "MATLAB Code"),
    ("server", "Console Server (Python)"),
    ("page", "Console Page (JavaScript)"),
    ("lab", "Lab Programs"),
    ("diagram", "Simulink Diagrams"),
]

# Areas of the lab programs, by a folder in their path.
LAB_AREAS = [
    ("/phasespace/", "phasespace", "StreamData: the PhaseSpace Server",
     "The C++ program on the lab console that reads PhaseSpace and sends each frame to the "
     "platforms and the console."),
    ("/csrc/", "csrc", "Platform Blocks' C and C++ Code",
     "The C and C++ code that the diagram's hardware blocks (spot.hardware) compile into "
     "the platforms' programs."),
    ("/teensy/", "teensy", "Teensy Firmware",
     "The micro-controller on each platform that drives the thruster valves and the pucks."),
    ("/+hardware/arm/", "arm-serial", "Arm Serial Setup",
     "Setting up the serial link to the arm's servos on the Chaser."),
    ("/deploy/", "deploy", "Deployment Scripts",
     "Scripts that install the lab console, SPOT Vision and the platforms' links."),
    ("/vision/", "vision", "SPOT Vision (Jetson Orin)",
     "The camera service on the Orin that the Vision tab streams from."),
    ("/calibration/", "arm-calibration", "Arm Calibration Script",
     "The script the Calibration tab runs on the Chaser to measure the arm's friction."),
    ("arm_calibration/", "arm-calibration", "Arm Calibration Script",
     "The script the Calibration tab runs on the Chaser to measure the arm's friction."),
]


# Finding the source


def source_root():
    """Return the folder the documented code is read from.

    The local console: the project. A checkout of the template (the
    webapp in <root>/+spot/+console/webapp): that root. The lab console
    (TX2): the copy deploy_tx2.sh puts in docs_source, or else its own
    folders only.
    """
    if LOCAL["enabled"] and LOCAL["root"]:
        return Path(LOCAL["root"])
    console = APP_DIR.parent
    if console.name == "+console" and console.parent.name == "+spot":
        return console.parent.parent
    if (PROJECT_DIR / "docs_source").is_dir():
        return PROJECT_DIR / "docs_source"
    return PROJECT_DIR


def wanted(rel):
    """Return whether a file (relative POSIX path) is documented."""
    parts = PurePosixPath(rel).parts
    if any(p in SKIP_PARTS or p.startswith(".") for p in parts[:-1]):
        return False
    name = parts[-1]
    if name.startswith(".") or name.endswith(SKIP_SUFFIXES) or ".bak" in name:
        return False
    return docparse.language_of(rel) is not None


def list_sources(root):
    """Return the documented files under root: {relative path: Path}.

    The project's top-level MATLAB files, everything in +spot (or the
    console's folders on a lab console without a copy), and the
    diagrams (.slx) at the top.
    """
    found = {}
    tops = [p for p in root.iterdir() if p.is_file()] if root.is_dir() else []
    for path in tops:
        if path.suffix in (".m", ".slx"):
            found[path.name] = path
    folders = [root / "+spot"] if (root / "+spot").is_dir() else [
        root / name for name in ("webapp", "phasespace", "deploy", "arm_calibration")]
    for folder in folders:
        if not folder.is_dir():
            continue
        for current, dirs, files in os.walk(str(folder)):
            dirs[:] = sorted(d for d in dirs if d not in SKIP_PARTS and not d.startswith("."))
            for name in sorted(files):
                path = Path(current) / name
                rel = path.relative_to(root).as_posix()
                if wanted(rel):
                    found[rel] = path
    return found


def classify(rel, contents):
    """Return (group, area key, area title, area summary) for a file.

    Args:
        rel: The file's path relative to the source root.
        contents: {package: (title, summary)} from the Contents.m files.
    """
    parts = PurePosixPath(rel).parts
    lab = next((a for a in LAB_AREAS if a[0] in "/" + rel), None)
    if rel.endswith(".slx") or ".slx/" in rel:
        model = rel.split(".slx")[0].split("/")[-1]
        return ("diagram", "diagram:" + model, model,
                "The MATLAB Function blocks of the diagram %s.slx." % model)
    if "webapp" in parts and "static" in parts:
        return ("page", "static", "The Console's Page",
                "The JavaScript, HTML and CSS of the page the browser shows (webapp/static).")
    if "webapp" in parts:
        return ("server", "spot_console", "The Console's Server",
                "The Python web server behind the page (webapp/app.py, webapp/spot_console).")
    if lab is not None:
        return ("lab", lab[1], lab[2], lab[3])
    packages = [p[1:] for p in parts[:-1] if p.startswith("+")]
    if len(packages) >= 2:
        key = ".".join(packages[:2])
        title, summary = contents.get(key, ("", ""))
        return ("matlab", key, key, summary or title)
    return ("matlab", "spot", "The Project and spot",
            "SPOT.m, your own parameters, and the spot package's overview and coding standard.")


def read_text(path):
    """Return a file's text with \\n line ends, and how it was stored.

    Returns:
        (text, {"crlf": bool, "bom": bool}).
    """
    raw = path.read_bytes()
    bom = raw.startswith(b"\xef\xbb\xbf")
    text = raw.decode("utf-8", errors="replace")
    if bom:
        text = text[1:]
    crlf = "\r\n" in text
    return text.replace("\r\n", "\n"), {"crlf": crlf, "bom": bom}


def sha_of(text):
    """Return a short hash of some text (to notice changes)."""
    return hashlib.sha1(text.encode("utf-8")).hexdigest()[:16]


def package_contents(root, found):
    """Return {package: (title, summary)} from its Contents.m."""
    out = {}
    for rel, path in found.items():
        if not rel.endswith("/Contents.m"):
            continue
        packages = [p[1:] for p in PurePosixPath(rel).parts[:-1] if p.startswith("+")]
        if len(packages) < 2:
            continue
        try:
            first = path.read_text(encoding="utf-8", errors="replace").split("\n")[0]
        except OSError:
            continue
        match = re.match(r"^%\s*(\w+)\s+(.*)$", first)
        summary = match.group(2).strip() if match else first.lstrip("% ")
        out[".".join(packages[:2])] = (summary, summary)
    return out


# Guides and notes


def read_guides():
    """Return the guides: their sections, and each page's text.

    Returns:
        {"sections": [{"title", "pages": [id]}], "pages": {id: {"id",
        "title", "path", "text", "section"}}, "errors": [...]}.
    """
    out = {"sections": [], "pages": {}, "errors": []}
    try:
        toc = json.loads((DOCS_DIR / "toc.json").read_text(encoding="utf-8"))
    except (OSError, ValueError) as e:
        out["errors"].append("docs/toc.json could not be read: %s" % e)
        return out
    for section in toc.get("sections", []):
        ids = []
        for page in section.get("pages", []):
            page_id = page.get("id")
            rel = page.get("file", "")
            path = DOCS_DIR / rel
            if not page_id or not rel:
                continue
            try:
                text = read_text(path)[0]
            except OSError:
                out["errors"].append("%s (in docs/toc.json) is missing." % rel)
                continue
            title = page.get("title")
            if not title:
                match = re.search(r"^#\s+(.+)$", text, re.M)
                title = match.group(1).strip() if match else page_id
            out["pages"][page_id] = {"id": page_id, "title": title, "text": text,
                                     "path": DOCS_PREFIX + rel, "section": section.get("title"),
                                     "summary": page.get("summary", "")}
            ids.append(page_id)
        out["sections"].append({"title": section.get("title", ""), "pages": ids,
                                "icon": section.get("icon", "")})
    return out


def read_notes():
    """Return the notes: {source: {"file", "entries", "path"}}."""
    out = {}
    folder = DOCS_DIR / "notes"
    if not folder.is_dir():
        return out
    for path in sorted(folder.rglob("*.md")):
        source = path.relative_to(folder).as_posix()[:-len(".md")]
        try:
            text = read_text(path)[0]
        except OSError:
            continue
        parts = re.split(r"^##\s+(.+?)\s*$", text, flags=re.M)
        entries = {}
        for k in range(1, len(parts) - 1, 2):
            entries[parts[k].strip("` ")] = parts[k + 1].strip()
        out[source] = {"file": parts[0].strip(), "entries": entries,
                       "path": DOCS_PREFIX + "notes/" + source + ".md"}
    return out


def docs_files():
    """Return the documentation's own files: {@docs/ path: Path}."""
    out = {}
    if not DOCS_DIR.is_dir():
        return out
    for path in sorted(DOCS_DIR.rglob("*")):
        if path.is_file() and path.suffix in (".md", ".json"):
            out[DOCS_PREFIX + path.relative_to(DOCS_DIR).as_posix()] = path
    return out


# The index


class DocIndex:
    """The index of the code and guides, rebuilt when files change."""

    def __init__(self):
        """Start with nothing parsed."""
        self._lock = threading.Lock()
        self._parsed = {}         # rel -> (stamp, record, entries)
        self._version = None
        self._payload = None      # (json bytes, gzip bytes)
        self._paths = {}          # rel -> Path, as last listed
        self._root = None
        self._known = {}          # what Markdown may link to (for lint)

    def _stamp(self, path):
        """Return what tells a file has changed: its size and time."""
        try:
            st = path.stat()
        except OSError:
            return None
        return (st.st_size, int(st.st_mtime * 1000))

    def _state(self, root, found):
        """Return a version string of the files the index reads."""
        digest = hashlib.sha1(str(root).encode("utf-8"))
        for rel in sorted(found):
            digest.update(("%s %s\n" % (rel, self._stamp(found[rel]))).encode("utf-8"))
        for rel, path in docs_files().items():
            digest.update(("%s %s\n" % (rel, self._stamp(path))).encode("utf-8"))
        digest.update(json.dumps(LOCAL["enabled"]).encode("utf-8"))
        return digest.hexdigest()[:16]

    def payload(self):
        """Return (version, JSON, gzip'd JSON), rebuilding if needed."""
        with self._lock:
            root = source_root()
            found = list_sources(root)
            version = self._state(root, found)
            if version != self._version or self._payload is None:
                data = self._build(root, found, version)
                body = json.dumps(data, separators=(",", ":")).encode("utf-8")
                self._payload = (body, gzip.compress(body, GZIP_LEVEL))
                self._known = doclint.known_targets(data)
                self._version = version
                self._paths = found
                self._root = root
            return self._version, self._payload[0], self._payload[1]

    def _parse(self, rel, path):
        """Return one file's records, parsed again if it changed."""
        stamp = self._stamp(path)
        cached = self._parsed.get(rel)
        if cached and cached[0] == stamp:
            return cached[1], cached[2]
        records = []
        if rel.endswith(".slx"):
            records = self._parse_diagram(rel, path)
        else:
            try:
                if path.stat().st_size > MAX_FILE_BYTES:
                    return None, []
                text, form = read_text(path)
            except OSError:
                return None, []
            lang = docparse.language_of(rel)
            try:
                record, entries = docparse.parse_file(rel, text, lang)
            except Exception as e:   # a parser bug must not hide the rest
                record, entries = {"path": rel, "lang": lang, "entries": [],
                                   "lines": text.count("\n") + 1,
                                   "error": "This file could not be read for the "
                                            "documentation: %s" % e}, []
            record.update(text=text, sha=sha_of(text), editable=LOCAL["enabled"], **form)
            records = [(record, entries)]
        self._parsed[rel] = (stamp, records, None)
        return records, None

    def _parse_diagram(self, rel, path):
        """Return a record per distinct MATLAB Function block."""
        try:
            blocks = docparse.diagram_blocks(path.read_bytes())
        except OSError:
            return []
        model = PurePosixPath(rel).stem
        scripts = {}
        exact = {}
        for block, raw, script in blocks:
            scripts.setdefault(script, []).append(block)
            exact[block] = "%s/%s" % (model, raw)
        out = []
        used = set()
        for script, paths in scripts.items():
            vpath = "%s/%s" % (rel, paths[0])
            if vpath in used:
                vpath += " (%d)" % len(used)
            used.add(vpath)
            record, entries = docparse.parse_file(vpath, script, "matlab")
            module = entries[0]
            module.update(kind="block", name=paths[0].split("/")[-1],
                          qual="%s/%s" % (model, paths[0]))
            main = next((e for e in entries[1:] if e["kind"] in ("function", "file")), None)
            if main is not None and main.get("doc") and not module.get("doc"):
                module["doc"] = main["doc"]
            record.update(text=script, sha=sha_of(script), editable=False, virtual=True,
                          diagram=model, slx=rel, blocks=paths, crlf=False, bom=False,
                          block_paths=[exact[p] for p in paths])
            record["summary"] = (module.get("doc") or {}).get("summary", "")
            out.append((record, entries))
        return out

    def _build(self, root, found, version):
        """Build the whole index (see the module's docstring)."""
        started = time.time()
        contents = package_contents(root, found)
        files = {}
        entries = {}
        areas = {}
        for rel in sorted(found):
            records, _ = self._parse(rel, found[rel])
            for record, ents in records or []:
                if record is None:
                    continue
                group, key, title, summary = classify(record["path"], contents)
                record["group"] = group
                record["area"] = key
                files[record["path"]] = record
                for entry in ents:
                    entries[entry["id"]] = json.loads(json.dumps(entry))
                area = areas.setdefault(key, {"key": key, "group": group, "title": title,
                                              "summary": summary, "files": []})
                area["files"].append(record["path"])
        for rel in list(self._parsed):
            if rel not in found:
                del self._parsed[rel]

        # Links between the entries, and the console's routes.
        links = docparse.cross_reference(files, entries)

        # The groups, each with its areas (MATLAB packages in order).
        groups = []
        for key, title in GROUPS:
            mine = sorted((a for a in areas.values() if a["group"] == key),
                          key=lambda a: (a["key"] != "spot", a["title"].lower()))
            if mine:
                groups.append({"key": key, "title": title, "areas": mine})

        guides = read_guides()
        return {
            "version": version,
            "generated": time.time(),
            "seconds": round(time.time() - started, 2),
            "mode": "local" if LOCAL["enabled"] else "lab",
            "editable": bool(LOCAL["enabled"]),
            "root": str(root),
            "groups": groups,
            "files": files,
            "entries": entries,
            "routes": links["routes"],
            "commands": links["commands"],
            "guides": guides,
            "notes": read_notes(),
            "docs_files": sorted(docs_files()),
        }

    def resolve(self, rel):
        """Return the Path of an indexed source file or a docs file.

        Raises:
            ApiError: if the path is not one the documentation shows.
        """
        if rel.startswith(DOCS_PREFIX):
            inner = rel[len(DOCS_PREFIX):]
            target = (DOCS_DIR / inner).resolve()
            if DOCS_DIR.resolve() not in target.parents or target.suffix not in (".md", ".json"):
                raise ApiError("%s is not a documentation file." % rel)
            return target
        if not self._paths:
            self.payload()
        path = self._paths.get(rel)
        if path is None:
            raise ApiError("%s is not one of the documented files." % rel)
        return path

    def read(self, rel):
        """Return a file for the editor: text, hash, can it save."""
        path = self.resolve(rel)
        try:
            text, form = read_text(path) if path.exists() else ("", {})
        except OSError as e:
            raise ApiError("Could not read %s: %s" % (rel, e))
        return {"path": rel, "text": text, "sha": sha_of(text) if path.exists() else None,
                "exists": path.exists(), "lang": docparse.language_of(rel) or "text",
                "editable": bool(LOCAL["enabled"])}

    def save(self, body, requester):
        """Save a file from the editor (local console only).

        Args:
            body: {"path", "text", "sha": the hash of the version the
                editor started from (None for a new notes file),
                "force": save over a newer version}.
            requester: Who asked (for the log).

        Returns:
            {"path", "sha", "backup", "note"}.

        Raises:
            ApiError: if saving isn't allowed, the file changed since it
                was read, or it can't be written.
        """
        if not LOCAL["enabled"]:
            raise ApiError("Files can only be saved on the local console (SPOT in MATLAB). "
                           "Download the file instead.")
        rel = str(body.get("path", ""))
        text = body.get("text")
        if not isinstance(text, str):
            raise ApiError("text must be the file's contents.")
        if len(text.encode("utf-8")) > MAX_SAVE_BYTES:
            raise ApiError("The file is too large to save from the page.")
        path = self.resolve(rel)
        new_note = rel.startswith(DOCS_PREFIX + "notes/") and not path.exists()

        # Not over a change made since the editor read the file.
        old_text, form = ("", {"crlf": False, "bom": False})
        if path.exists():
            old_text, form = read_text(path)
            if body.get("sha") != sha_of(old_text) and not body.get("force"):
                raise ApiError("%s has changed on disk since you opened it. Reload it (your "
                               "edits are kept in the editor until you do), or save anyway "
                               "to replace that version." % rel, {"conflict": True,
                                                                 "sha": sha_of(old_text)})
        elif not new_note:
            raise ApiError("%s no longer exists." % rel)

        # Keep the old version, then write the new one in its place,
        # with the file's own line ends.
        backup = self._backup(rel, old_text, form) if path.exists() else None
        out = text.replace("\r\n", "\n")
        if form.get("crlf"):
            out = out.replace("\n", "\r\n")
        data = (b"\xef\xbb\xbf" if form.get("bom") else b"") + out.encode("utf-8")
        try:
            path.parent.mkdir(parents=True, exist_ok=True)
            tmp = path.with_name(path.name + ".spot-tmp")
            tmp.write_bytes(data)
            os.replace(str(tmp), str(path))
        except OSError as e:
            raise ApiError("Could not save %s: %s" % (rel, e))

        hub.log("matlab", "Documentation: saved %s (by %s)" % (rel, requester))
        sha = sha_of(text.replace("\r\n", "\n"))
        hub.publish("docs", {"saved": rel, "sha": sha})

        # MATLAB checks its files' times at the prompt, not while the
        # worker runs: tell it about a saved .m file.
        if rel.endswith(".m") and matlab.status()["connected"]:
            threading.Thread(target=self._rehash, daemon=True).start()
        return {"path": rel, "sha": sha, "backup": backup, "note": after_save_note(rel)}

    def _rehash(self):
        """Ask MATLAB to read changed files again (rehash), if free."""
        try:
            if not matlab.status()["busy"]:
                matlab.request("code.rehash", {}, timeout=30, quiet=True)
        except ApiError:
            pass

    def lint(self, body):
        """Check code from the editor (doclint), as it would be saved.

        Args:
            body: {"path", "text", "lang" (optional)}.

        Returns:
            {"diagnostics", "checks", "notes"}.
        """
        rel = str(body.get("path", ""))
        text = body.get("text")
        if not isinstance(text, str):
            raise ApiError("text must be the code to check.")
        lang = str(body.get("lang") or docparse.language_of(rel) or "text")
        saved = None
        try:
            path = self.resolve(rel)
            saved = doclint.read_saved(path) if path.exists() else None
        except ApiError:
            path = None
        if lang == "markdown" and not self._known:
            self.payload()
        return doclint.lint(rel, lang, text, root=self._root or source_root(),
                            local=LOCAL["enabled"], matlab=matlab if LOCAL["enabled"] else None,
                            saved_text=saved, known=self._known)

    def _backup(self, rel, text, form):
        """Copy a file's current version to .spot_cache/docs_backups."""
        folder = Path(LOCAL["root"]) / ".spot_cache" / "docs_backups"
        stamp = time.strftime("%Y%m%d-%H%M%S")
        target = folder / stamp / rel.replace(DOCS_PREFIX, "docs/")
        try:
            target.parent.mkdir(parents=True, exist_ok=True)
            out = text.replace("\n", "\r\n") if form.get("crlf") else text
            target.write_bytes(out.encode("utf-8"))
            saves = sorted(p for p in folder.iterdir() if p.is_dir())
            for old in saves[:-BACKUPS_KEPT]:
                shutil.rmtree(str(old), ignore_errors=True)
        except OSError:
            return None
        return str(target)


def after_save_note(rel):
    """Return what the user needs to know after saving a file."""
    if rel.endswith(".m"):
        return "MATLAB uses the new version from its next call."
    if rel.endswith(".py") and "/webapp/" in rel:
        return "The console's server runs the new code after it restarts (SPOT stop, then SPOT)."
    if rel.endswith((".js", ".css", ".html")) and "/static/" in rel:
        return "Reload the page to run the new code."
    if rel.endswith((".cc", ".c", ".cpp", ".h", ".ino", ".sh", ".service")):
        return "Deploy it to the lab (Lab Setup) for the lab's computers to use it."
    if rel.startswith(DOCS_PREFIX):
        return "The documentation shows it now."
    return ""


doc_index = DocIndex()
