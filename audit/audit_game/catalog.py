"""The catalog: the files the audit covers, in chapters and sections.

The game plays on a snapshot of the project (audit/original/): the
first start copies every file the audit covers there, and the chunks
are cut from that copy, so the game's content stays fixed while the
project itself may move on. Rewrites are written to audit/work/ with
the same paths, ready to be copied over the project when the audit
is done.

Part of the SPOT audit game (audit/app.py starts it).
"""

import json
import os
import shutil
from datetime import datetime
from pathlib import Path

from .chunker import chunk_file, language_of

REPO = Path(__file__).resolve().parents[2]
AUDIT_DIR = REPO / "audit"
ORIGINAL_DIR = AUDIT_DIR / "original"
WORK_DIR = AUDIT_DIR / "work"
BACKUP_DIR = AUDIT_DIR / "backup"
MANIFEST = ORIGINAL_DIR / "MANIFEST.json"

# What the audit covers, relative to the repository.
SOURCES = ["template", "New_SPOT_Project.m", "changes/patch-notes.tex", "designs/cad_tools"]
EXCLUDED_PARTS = {"slprj", "node_modules", "__pycache__", ".spot_cache", "output",
                  "libowl-src-410", "vendor", "images", "models", "audit"}
EXCLUDED_FILES = {"spot_calibration.json", "LICENSE.txt"}
EXCLUDED_SUFFIXES = (".bak", ".slxc", ".slx", ".pyc")

WEBAPP = "template/+interface/+console/webapp"
# The MATLAB packages, in reading order: spot (what the diagrams and scripts call), then
# interface (the SPOT Console's own code).
MATLAB_PACKAGES = {
    "spot": ["params", "massprops", "guidance", "control", "dynamics", "arm", "buses", "hardware",
             "sim", "experiment", "data", "plotting", "camera"],
    "interface": ["console", "worker", "workspace", "results"],
}

CHAPTERS = [
    {"id": "docs", "title": "Documentation", "icon": "book",
     "blurb": "The guides, the notes on the code, the project's READMEs and the release notes."},
    {"id": "matlab", "title": "MATLAB", "icon": "matlab",
     "blurb": "The +spot and +interface packages, the launcher, the user parameters and "
              "New_SPOT_Project."},
    {"id": "python", "title": "Python", "icon": "python",
     "blurb": "The console server, SPOT Vision, the arm calibration and the CAD tools."},
    {"id": "javascript", "title": "JavaScript and the Page", "icon": "js",
     "blurb": "The console's page scripts, its markup and styles, and the editor bundle."},
    {"id": "shell", "title": "Shell Scripts", "icon": "shell",
     "blurb": "Deployment, the platforms' agent and serial links, builds and services."},
    {"id": "firmware", "title": "Arduino and C/C++", "icon": "chip",
     "blurb": "The Teensy firmware, the platforms' C code and StreamData."},
    {"id": "config", "title": "Configuration", "icon": "gear",
     "blurb": "The settings, design and table-of-contents files, and the bundle's package.json."},
]
SECTION_ORDER = {
    "docs": ["Guides · Start Here", "Guides · The Lab", "Guides · The Local Console",
             "Guides · The Lab Console", "Guides · Working in MATLAB",
             "Guides · The Simulink Diagram", "Guides · The Maths", "Guides · For Developers",
             "Guides · Other",
             "Notes on the Code", "Project Notes and READMEs", "Release Notes"],
    "matlab": ["Project Root"] + [section for top, packages in MATLAB_PACKAGES.items()
                                  for section in ["+" + top] + ["+%s/+%s" % (top, p)
                                                                 for p in packages]],
    "python": ["Console Server (spot_console)", "SPOT Vision", "Arm Calibration", "CAD Tools"],
    "javascript": ["Console Page Scripts", "Page Markup and Styles", "Editor Bundle"],
    "shell": ["Deployment", "Platforms", "Builds", "Services"],
    "firmware": ["Teensy Firmware", "Platform C Code (csrc)", "StreamData (PhaseSpace)"],
    "config": ["JSON Files"],
}
FIRST_FILES = {"Contents.m", "app.js", "index.html", "__init__.py", "app.py", "SPOT.m",
               "welcome.md", "CODE_TEMPLATE.md"}


# The snapshot


def covered(rel):
    """Whether a repository file (posix relative path) is in the audit."""
    parts = rel.split("/")
    if any(p in EXCLUDED_PARTS for p in parts[:-1]):
        return False
    name = parts[-1]
    if name in EXCLUDED_FILES or name.endswith(EXCLUDED_SUFFIXES):
        return False
    return language_of(rel) is not None


def list_repo_files():
    """Return the covered files, as posix paths relative to the repository."""
    out = []
    for src in SOURCES:
        root = REPO / src
        if root.is_file():
            if covered(src):
                out.append(src)
            continue
        if not root.is_dir():
            continue
        for dirpath, dirnames, filenames in os.walk(root):
            dirnames[:] = sorted(d for d in dirnames if d not in EXCLUDED_PARTS)
            for f in sorted(filenames):
                rel = Path(dirpath, f).relative_to(REPO).as_posix()
                if covered(rel):
                    out.append(rel)
    return out


def ensure_snapshot():
    """Copy the covered files to audit/original/ if that is not done yet."""
    if MANIFEST.is_file():
        return json.loads(MANIFEST.read_text(encoding="utf-8"))
    files = list_repo_files()
    for rel in files:
        dst = ORIGINAL_DIR / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO / rel, dst)
    manifest = {"taken": datetime.now().isoformat(timespec="seconds"), "files": files}
    MANIFEST.write_text(json.dumps(manifest, indent=1), encoding="utf-8")
    return manifest


# Where a file goes


def place(rel, toc_sections):
    """Return (chapter id, section title, sort key) for a covered file."""
    name = rel.rsplit("/", 1)[-1]
    lang, family = language_of(rel)
    low = name.lower()
    if rel.startswith(WEBAPP + "/docs/guides/"):
        page = rel[len(WEBAPP) + len("/docs/"):]
        if page in toc_sections:
            title, order = toc_sections[page]
            return "docs", "Guides · " + title, (order, name)
        return "docs", "Guides · Other", (0, name)
    if rel.startswith(WEBAPP + "/docs/notes/"):
        return "docs", "Notes on the Code", (0 if name == "diagram.md" else 1, rel)
    if low.endswith(".md"):
        return "docs", "Project Notes and READMEs", (0 if name in FIRST_FILES else 1, rel)
    if low.endswith(".tex"):
        return "docs", "Release Notes", (0, name)
    if low.endswith(".m"):
        for top in MATLAB_PACKAGES:
            if "/+%s/+" % top in rel:
                pkg = rel.split("/+%s/+" % top, 1)[1].split("/", 1)[0]
                return "matlab", "+%s/+%s" % (top, pkg), (0 if name == "Contents.m" else 1, rel)
            if rel.endswith("/+%s/Contents.m" % top):
                return "matlab", "+" + top, (0, name)
        return "matlab", "Project Root", (0 if name == "SPOT.m" else 1, name)
    if low.endswith(".py"):
        if rel.startswith("designs/"):
            return "python", "CAD Tools", (0, name)
        if "/vision/" in rel:
            return "python", "SPOT Vision", (0, name)
        if "/+arm/" in rel:
            return "python", "Arm Calibration", (0, name)
        return "python", "Console Server (spot_console)", (0 if name in FIRST_FILES else 1, name)
    if low.endswith((".js", ".mjs")):
        if "/tools/" in rel:
            return "javascript", "Editor Bundle", (0, name)
        return "javascript", "Console Page Scripts", (0 if name in FIRST_FILES else 1, name)
    if low.endswith((".html", ".css")):
        return "javascript", "Page Markup and Styles", (0 if name in FIRST_FILES else 1, name)
    if low.endswith((".ino",)):
        return "firmware", "Teensy Firmware", (0, name)
    if low.endswith((".c", ".cpp", ".cc", ".h", ".hpp")):
        if "/phasespace/" in rel:
            return "firmware", "StreamData (PhaseSpace)", (0, name)
        return "firmware", "Platform C Code (csrc)", (0, rel)
    if low.endswith(".service"):
        return "shell", "Services", (0, name)
    if low.endswith(".sh") or name == "Makefile":
        if "/deploy/" in rel:
            return "shell", "Deployment", (0, name)
        if "/+hardware/" in rel:
            return "shell", "Platforms", (0, name)
        return "shell", "Builds", (0, rel)
    if low.endswith(".json"):
        return "config", "JSON Files", (0, rel)
    return "config", "Other", (0, rel)


def read_toc_sections():
    """Map each guide (docs-relative path) to its toc.json section and order."""
    toc = ORIGINAL_DIR / WEBAPP / "docs" / "toc.json"
    out = {}
    if not toc.is_file():
        return out
    try:
        data = json.loads(toc.read_text(encoding="utf-8"))
    except ValueError:
        return out
    for section in data.get("sections", []):
        for k, page in enumerate(section.get("pages", [])):
            if page.get("file"):
                out[page["file"]] = (section.get("title", "Other"), k)
    return out


# The catalog


class Catalog:
    """Every covered file, its chunks, and the chapters they sit in.

    Attributes:
        files: Ordered list of file records (rel, name, chapter, section,
            lang, family, keys, lines).
        chunks: Chunk key -> chunk record (with file, chapter, section,
            ordinal added).
        chapters: Ordered list of {id, title, blurb, sections: [{title,
            files: [rel]}]}.
        order: Every chunk key, in reading order.
    """

    def __init__(self):
        manifest = ensure_snapshot()
        self.taken = manifest["taken"]
        toc_sections = read_toc_sections()
        placed = []
        for rel in manifest["files"]:
            path = ORIGINAL_DIR / rel
            if not path.is_file():
                continue
            chapter, section, key = place(rel, toc_sections)
            placed.append((chapter, section, key, rel))

        chapter_index = {c["id"]: k for k, c in enumerate(CHAPTERS)}

        def section_rank(chapter, section):
            order = SECTION_ORDER.get(chapter, [])
            return (order.index(section), "") if section in order else (len(order), section)

        placed.sort(key=lambda t: (chapter_index.get(t[0], 99), section_rank(t[0], t[1]), t[2]))

        self.files = []
        self.by_file = {}
        self.chunks = {}
        self.order = []
        self.chapters = [dict(c, sections=[]) for c in CHAPTERS]
        chapter_rec = {c["id"]: c for c in self.chapters}
        for chapter, section, _, rel in placed:
            text = (ORIGINAL_DIR / rel).read_text(encoding="utf-8", errors="replace")
            lang, family = language_of(rel)
            chunks = chunk_file(rel, text)
            keys = []
            for c in chunks:
                c["file"] = rel
                c["chapter"] = chapter
                c["section"] = section
                c["lang"] = lang
                c["family"] = family
                c["ordinal"] = len(self.order)
                self.chunks[c["key"]] = c
                self.order.append(c["key"])
                keys.append(c["key"])
            rec = {"rel": rel, "name": rel.rsplit("/", 1)[-1], "chapter": chapter,
                   "section": section, "lang": lang, "family": family, "keys": keys,
                   "lines": text.count("\n") + (0 if text.endswith("\n") or not text else 1),
                   "text": text}
            self.files.append(rec)
            self.by_file[rel] = rec
            ch = chapter_rec[chapter]
            sec = next((s for s in ch["sections"] if s["title"] == section), None)
            if sec is None:
                sec = {"title": section, "files": []}
                ch["sections"].append(sec)
            sec["files"].append(rel)

    def original_text(self, rel):
        """The snapshot's text of a file."""
        return self.by_file[rel]["text"]

    def neighbours(self, key, before=12, after=12):
        """Lines of the file around a chunk, for context."""
        c = self.chunks[key]
        lines = self.by_file[c["file"]]["text"].splitlines(keepends=True)
        lo = max(0, c["start"] - 1 - before)
        hi = min(len(lines), c["end"] + after)
        return {"before": "".join(lines[lo:c["start"] - 1]), "before_start": lo + 1,
                "after": "".join(lines[c["end"]:hi]), "after_start": c["end"] + 1}
