"""The HTTP server: the page, the JSON API, start-up.

Python's standard library only, like the SPOT Console's own server.

Part of the SPOT audit game (audit/app.py starts it).
"""

import argparse
import difflib
import json
import os
import sys
import threading
import webbrowser
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from socketserver import ThreadingMixIn
from urllib.parse import parse_qs, urlparse

from .catalog import Catalog, AUDIT_DIR, REPO, WORK_DIR
from .chunker import compare_code
from .progress import Progress, SAVES_DIR

STATIC_DIR = AUDIT_DIR / "static"
SPOT_STATIC = REPO / "template" / "+interface" / "+console" / "webapp" / "static"

CONTENT_TYPES = {".html": "text/html; charset=utf-8", ".js": "text/javascript; charset=utf-8",
                 ".css": "text/css; charset=utf-8", ".svg": "image/svg+xml", ".png": "image/png",
                 ".json": "application/json", ".woff2": "font/woff2", ".woff": "font/woff",
                 ".ttf": "font/ttf", ".txt": "text/plain; charset=utf-8"}
RECENT_EVENTS = 80


class ApiError(Exception):
    """An error for the page (HTTP 400)."""


class State:
    """The catalog and the progress, shared by every request."""

    catalog = None
    progress = None


def chunk_payload(key):
    """Everything the page shows for one chunk."""
    cat = State.catalog
    prog = State.progress
    if key not in cat.chunks:
        raise ApiError("No such chunk: %s" % key)
    c = cat.chunks[key]
    f = cat.by_file[c["file"]]
    pos = f["keys"].index(key)
    strip = []
    for k in f["keys"]:
        r = prog.record(k) or {}
        strip.append({"key": k, "status": r.get("status", "pending"),
                      "flag": bool(r.get("flag")) and not r["flag"].get("resolved"),
                      "title": cat.chunks[k]["title"], "start": cat.chunks[k]["start"],
                      "end": cat.chunks[k]["end"]})
    chapter = next(ch for ch in cat.chapters if ch["id"] == c["chapter"])
    section = next(s for s in chapter["sections"] if s["title"] == c["section"])
    file_pos = section["files"].index(c["file"])
    out = {k: v for k, v in c.items()}
    out.update({
        "record": prog.record(key), "context": cat.neighbours(key),
        "file_info": {"rel": c["file"], "name": f["name"], "lang": f["lang"],
                      "lines": f["lines"], "pos": pos, "count": len(f["keys"]),
                      "chapter": chapter["title"], "chapter_id": chapter["id"],
                      "section": c["section"], "file_pos": file_pos,
                      "file_count": len(section["files"]),
                      "next_file": section["files"][file_pos + 1]
                      if file_pos + 1 < len(section["files"]) else None},
        "strip": strip,
        "prev": cat.order[c["ordinal"] - 1] if c["ordinal"] > 0 else None,
        "next": cat.order[c["ordinal"] + 1] if c["ordinal"] + 1 < len(cat.order) else None,
        "next_pending": prog.next_pending(key),
        "next_pending_in_file": prog.next_pending(key, same_file=True),
    })
    return out


def flags_payload():
    cat = State.catalog
    out = []
    for key, r in State.progress.data["chunks"].items():
        if r.get("flag") and key in cat.chunks:
            c = cat.chunks[key]
            out.append({"key": key, "file": c["file"], "title": c["title"], "index": c["index"],
                        "start": c["start"], "end": c["end"], "chapter": c["chapter"],
                        "status": r.get("status", "pending"), **r["flag"]})
    out.sort(key=lambda f: (bool(f.get("resolved")), f["at"]), reverse=False)
    out.sort(key=lambda f: bool(f.get("resolved")))
    return out


def state_payload():
    cat = State.catalog
    prog = State.progress
    return {
        "taken": cat.taken, "summary": prog.summary(), "settings": prog.data["settings"],
        "achievements": prog.achievements(), "flags": flags_payload(),
        "events": list(reversed(prog.data["events"][-RECENT_EVENTS:])),
        "started": prog.data["started"], "days": prog.days(),
        "first_pending": prog.next_pending(None),
        "repo": str(REPO), "work_dir": str(WORK_DIR), "saves_dir": str(SAVES_DIR),
    }


def export_payload():
    cat = State.catalog
    prog = State.progress
    out = []
    for rec in cat.files:
        if not prog.file_changed(rec["rel"]):
            continue
        a = rec["text"].splitlines()
        b = prog.file_text(rec["rel"]).splitlines()
        added = removed = 0
        for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b).get_opcodes():
            if tag in ("replace", "delete"):
                removed += i2 - i1
            if tag in ("replace", "insert"):
                added += j2 - j1
        fc = prog.file_counts(rec["rel"])
        out.append({"rel": rec["rel"], "name": rec["name"], "lang": rec["lang"],
                    "rewritten": fc["rewritten"], "total": fc["total"], "added": added,
                    "removed": removed, "drift": prog.project_drift(rec["rel"]),
                    "code_changed": any((prog.record(k) or {}).get("code_changed")
                                        for k in rec["keys"])})
    return {"files": out, "work_dir": str(WORK_DIR), "repo": str(REPO)}


def unified_diff(rel):
    cat = State.catalog
    if rel not in cat.by_file:
        raise ApiError("Not in the audit: %s" % rel)
    a = cat.by_file[rel]["text"].splitlines(keepends=True)
    b = State.progress.file_text(rel).splitlines(keepends=True)
    return "".join(difflib.unified_diff(a, b, "original/" + rel, "work/" + rel))


class Handler(BaseHTTPRequestHandler):
    """Routes: the page and its files, /vendor from the SPOT Console, /api."""

    server_version = "SPOTAudit/1.0"

    def log_message(self, fmt, *args):
        """Keep the terminal quiet."""

    def send_json(self, data, status=200):
        body = json.dumps(data).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def send_file(self, path, cache=False):
        path = Path(path)
        if not path.is_file():
            self.send_error(404)
            return
        body = path.read_bytes()
        self.send_response(200)
        self.send_header("Content-Type", CONTENT_TYPES.get(path.suffix.lower(),
                                                           "application/octet-stream"))
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "max-age=86400" if cache else "no-store")
        self.end_headers()
        self.wfile.write(body)

    def read_json(self):
        n = int(self.headers.get("Content-Length") or 0)
        if not n:
            return {}
        try:
            return json.loads(self.rfile.read(n).decode("utf-8"))
        except ValueError:
            raise ApiError("The request is not JSON.")

    def safe_under(self, root, rel):
        p = (root / rel.lstrip("/")).resolve()
        if root.resolve() not in p.parents:
            raise ApiError("Not allowed.")
        return p

    def do_GET(self):
        url = urlparse(self.path)
        q = {k: v[0] for k, v in parse_qs(url.query).items()}
        path = url.path
        try:
            if path == "/" or path == "/index.html":
                self.send_file(STATIC_DIR / "index.html")
            elif path.startswith("/static/"):
                self.send_file(self.safe_under(STATIC_DIR, path[len("/static/"):]))
            elif path.startswith("/vendor/"):
                self.send_file(self.safe_under(SPOT_STATIC / "vendor", path[len("/vendor/"):]),
                               cache=True)
            elif path == "/logo.png":
                self.send_file(SPOT_STATIC / "srl_logo.png", cache=True)
            elif path == "/api/state":
                self.send_json(state_payload())
            elif path == "/api/chunk":
                self.send_json(chunk_payload(q.get("key", "")))
            elif path == "/api/next":
                key = State.progress.next_pending(q.get("after") or None,
                                                  same_file=q.get("same_file") == "1")
                self.send_json({"key": key})
            elif path == "/api/file":
                rel = q.get("rel", "")
                cat = State.catalog
                if rel not in cat.by_file:
                    raise ApiError("Not in the audit: %s" % rel)
                f = cat.by_file[rel]
                self.send_json({"rel": rel, "lang": f["lang"], "original": f["text"],
                                "work": State.progress.file_text(rel),
                                "first_key": f["keys"][0] if f["keys"] else None,
                                "first_pending": next((k for k in f["keys"] if
                                                       State.progress.status_of(k) == "pending"),
                                                      None)})
            elif path == "/api/export":
                self.send_json(export_payload())
            elif path == "/api/export/diff":
                self.send_json({"rel": q.get("rel", ""), "diff": unified_diff(q.get("rel", ""))})
            elif path == "/api/saves":
                self.send_json({"saves": State.progress.list_saves(), "dir": str(SAVES_DIR)})
            elif path == "/api/log":
                n = int(q.get("limit", 300))
                evs = State.progress.data["events"]
                self.send_json({"events": list(reversed(evs[-n:])), "total": len(evs)})
            else:
                self.send_error(404)
        except ApiError as e:
            self.send_json({"error": str(e)}, 400)
        except Exception as e:  # noqa: BLE001
            self.send_json({"error": "%s: %s" % (type(e).__name__, e)}, 500)

    def do_POST(self):
        path = urlparse(self.path).path
        prog = State.progress
        try:
            body = self.read_json()
            key = body.get("key", "")
            if path.startswith("/api/chunk/"):
                if key not in State.catalog.chunks:
                    raise ApiError("No such chunk.")
                action = path[len("/api/chunk/"):]
                if action == "accept":
                    out = prog.accept(key)
                elif action == "rewrite":
                    text = body.get("text")
                    if not isinstance(text, str) or not text.strip():
                        raise ApiError("The rewrite is empty.")
                    out = prog.rewrite(key, text)
                elif action == "skip":
                    out = prog.skip(key)
                elif action == "reset":
                    out = prog.reset(key)
                elif action == "flag":
                    out = prog.flag(key, (body.get("note") or "").strip())
                elif action == "unflag":
                    out = prog.unflag(key)
                elif action == "resolve":
                    out = prog.resolve(key, True)
                elif action == "reopen":
                    out = prog.resolve(key, False)
                else:
                    raise ApiError("Unknown action: %s" % action)
                out["flags"] = flags_payload()
                self.send_json(out)
            elif path == "/api/check":
                if key not in State.catalog.chunks:
                    raise ApiError("No such chunk.")
                c = State.catalog.chunks[key]
                self.send_json(compare_code(c["text"], body.get("text") or "", c["family"]))
            elif path == "/api/heartbeat":
                self.send_json({"time_played": round(prog.heartbeat())})
            elif path == "/api/saves/save":
                name = (body.get("name") or "").strip()
                if not name:
                    raise ApiError("Give the save a name.")
                self.send_json(prog.save_slot(name, bool(body.get("overwrite"))))
            elif path == "/api/saves/load":
                try:
                    saves = prog.load_slot(body.get("file") or "")
                except ValueError as e:
                    raise ApiError(str(e))
                self.send_json({"saves": saves, "state": state_payload()})
            elif path == "/api/saves/delete":
                try:
                    self.send_json({"saves": prog.delete_slot(body.get("file") or "")})
                except ValueError as e:
                    raise ApiError(str(e))
            elif path == "/api/settings":
                self.send_json({"settings": prog.set_settings(body.get("settings") or {})})
            elif path == "/api/export/apply":
                rels = body.get("rels") or []
                self.send_json({"results": prog.apply(rels), "export": export_payload()})
            else:
                self.send_error(404)
        except ApiError as e:
            self.send_json({"error": str(e)}, 400)
        except Exception as e:  # noqa: BLE001
            self.send_json({"error": "%s: %s" % (type(e).__name__, e)}, 500)


class Server(ThreadingMixIn, HTTPServer):
    daemon_threads = True
    allow_reuse_address = True


def open_browser(url):
    """Open the page, keeping the browser's own messages out of the terminal.

    The browser inherits this process's output while it is started, so
    stdout and stderr point at /dev/null for just that moment.
    """
    saved = (os.dup(1), os.dup(2))
    devnull = os.open(os.devnull, os.O_RDWR)
    try:
        os.dup2(devnull, 1)
        os.dup2(devnull, 2)
        webbrowser.open(url)
    finally:
        os.dup2(saved[0], 1)
        os.dup2(saved[1], 2)
        os.close(devnull)
        os.close(saved[0])
        os.close(saved[1])


def main(argv=None):
    """Start the audit game: build the catalog, load the progress, serve."""
    parser = argparse.ArgumentParser(description="SPOT audit game")
    parser.add_argument("--port", type=int, default=8090)
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--no-browser", action="store_true")
    args = parser.parse_args(argv)

    print("Reading the snapshot and cutting the chunks...")
    State.catalog = Catalog()
    State.progress = Progress(State.catalog)
    s = State.progress.summary()
    print("  %d files, %d chunks; %d cleared (%.1f %%)" % (
        len(State.catalog.files), s["total"], s["cleared"], s["pct"]))

    httpd = Server((args.host, args.port), Handler)
    url = "http://%s:%d/" % ("localhost" if args.host in ("127.0.0.1", "0.0.0.0") else args.host,
                             args.port)
    print("SPOT Audit at %s  (Ctrl+C stops it)" % url)
    if not args.no_browser:
        threading.Timer(0.6, lambda: open_browser(url)).start()
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        httpd.server_close()
