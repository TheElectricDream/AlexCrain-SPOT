"""The player's progress: chunk statuses, rewrites, flags, the log,
achievements, and the work copy of every rewritten file.

Everything is kept in audit/progress.json, written whole after each
change. Rewrites are keyed by the chunk's key (a hash of its file and
text), so progress survives a new snapshot wherever the text is the
same.

Part of the SPOT audit game (audit/app.py starts it).
"""

import json
import os
import re
import shutil
import threading
import time
from datetime import datetime, date, timedelta

from .catalog import ORIGINAL_DIR, WORK_DIR, BACKUP_DIR, REPO, AUDIT_DIR
from .chunker import compare_code

SAVES_DIR = AUDIT_DIR / "saves"
AUTOSAVE = SAVES_DIR / "autosave.json"
OLD_PROGRESS = AUDIT_DIR / "progress.json"
HEARTBEAT_MAX = 45.0      # [s] the most one heartbeat adds to the time played
SAVE_VERSION = 2

XP_ACCEPT = 10
XP_REWRITE = 25
XP_RESOLVE = 5
MAX_EVENTS = 5000

LEVEL_TITLES = ["Reader", "Reviewer", "Proofreader", "Inspector", "Auditor", "Examiner",
                "Archivist", "Curator", "Grandmaster", "Oracle"]

# Static achievements: id, title, description, and a test on the stats.
ACHIEVEMENTS = [
    ("first_clear", "First Steps", "Clear your first chunk.", lambda s: s["cleared"] >= 1),
    ("first_rewrite", "In Your Own Words", "Save your first rewrite.", lambda s: s["rewritten"] >= 1),
    ("first_flag", "Red Flag", "Flag something to come back to.", lambda s: s["flags_raised"] >= 1),
    ("first_resolve", "All Clear", "Resolve a flag.", lambda s: s["flags_resolved"] >= 1),
    ("first_file", "Closed the Book", "Clear every chunk of one file.", lambda s: s["files_done"] >= 1),
    ("surgeon", "Surgeon", "Save a rewrite that changes the code itself, not just its comments.",
     lambda s: s["code_changes"] >= 1),
    ("clear_50", "Warming Up", "Clear 50 chunks.", lambda s: s["cleared"] >= 50),
    ("clear_250", "Getting Serious", "Clear 250 chunks.", lambda s: s["cleared"] >= 250),
    ("clear_1000", "Kilochunk", "Clear 1,000 chunks.", lambda s: s["cleared"] >= 1000),
    ("clear_2500", "Marathon Mind", "Clear 2,500 chunks.", lambda s: s["cleared"] >= 2500),
    ("pct_25", "A Quarter Done", "Clear a quarter of the whole audit.", lambda s: s["pct"] >= 25),
    ("pct_50", "Halfway There", "Clear half of the whole audit.", lambda s: s["pct"] >= 50),
    ("pct_75", "Three Quarters", "Clear three quarters of the whole audit.", lambda s: s["pct"] >= 75),
    ("pct_100", "Audit Complete", "Clear every chunk in every chapter.", lambda s: s["pct"] >= 100),
    ("rewrites_25", "Editor", "Save 25 rewrites.", lambda s: s["rewritten"] >= 25),
    ("rewrites_100", "Wordsmith", "Save 100 rewrites.", lambda s: s["rewritten"] >= 100),
    ("rewrites_500", "Ghostwriter", "Save 500 rewrites.", lambda s: s["rewritten"] >= 500),
    ("flags_10", "Sharp Eye", "Raise 10 flags.", lambda s: s["flags_raised"] >= 10),
    ("loose_ends", "Loose Ends Tied", "Resolve every flag (at least five of them).",
     lambda s: s["flags_raised"] >= 5 and s["flags_open"] == 0),
    ("day_50", "Productive Day", "Clear 50 chunks in one day.", lambda s: s["today"] >= 50),
    ("day_150", "On Fire", "Clear 150 chunks in one day.", lambda s: s["today"] >= 150),
    ("night_owl", "Night Owl", "Clear a chunk between midnight and 5 am.", lambda s: 0 <= s["hour"] < 5),
    ("early_bird", "Early Bird", "Clear a chunk between 5 and 7 am.", lambda s: 5 <= s["hour"] < 7),
    ("polyglot", "Polyglot", "Clear a chunk in every chapter.", lambda s: s["chapters_touched"] >= s["chapters"]),
    ("deep_dive", "Deep Dive", "Clear a file of 80 chunks or more.", lambda s: s["big_file_done"]),
    ("files_25", "Shelf Clearer", "Clear 25 files.", lambda s: s["files_done"] >= 25),
    ("files_100", "Librarian", "Clear 100 files.", lambda s: s["files_done"] >= 100),
    ("streak_3", "Three in a Row", "Audit on three days in a row.", lambda s: s["streak"] >= 3),
    ("streak_7", "A Week of Audits", "Audit on seven days in a row.", lambda s: s["streak"] >= 7),
]


def now_iso():
    return datetime.now().isoformat(timespec="seconds")


class Progress:
    """The progress file and everything that follows from it."""

    def __init__(self, catalog):
        self.path = str(AUTOSAVE)
        self.catalog = catalog
        self.lock = threading.RLock()
        self.last_beat = time.time()
        SAVES_DIR.mkdir(parents=True, exist_ok=True)
        # The progress file of earlier versions moves into saves/.
        if OLD_PROGRESS.is_file() and not AUTOSAVE.is_file():
            OLD_PROGRESS.replace(AUTOSAVE)
        self.data = self._empty()
        if AUTOSAVE.is_file():
            try:
                self._adopt(self._read(AUTOSAVE))
            except (OSError, ValueError):
                pass
        self._write_all_work()

    @staticmethod
    def _empty():
        return {"version": SAVE_VERSION, "started": now_iso(), "chunks": {}, "events": [],
                "achievements": {}, "time_played": 0.0,
                "settings": {"autoAdvance": True, "context": False, "showDiff": False}}

    def _adopt(self, loaded):
        """Take a save file's content as the current progress."""
        data = self._empty()
        settings = dict(data["settings"], **(loaded.get("settings") or {}))
        data.update({k: v for k, v in loaded.items() if k != "meta"})
        data["settings"] = settings
        data["time_played"] = float(data.get("time_played") or 0.0)
        self.data = data

    def _write_all_work(self):
        """Work files follow the rewrites: make sure they are all there."""
        for rec in self.catalog.files:
            self._write_work(rec["rel"])

    # Saving

    @staticmethod
    def _read(path):
        with open(str(path), encoding="utf-8") as f:
            return json.load(f)

    def meta(self, name="Autosave", kind="auto"):
        """What a save file says about itself (shown in the Load menu)."""
        st = self.stats()
        return {"name": name, "kind": kind, "saved_at": now_iso(),
                "time_played": round(self.data["time_played"]),
                "cleared": st["cleared"], "total": st["total"], "pct": round(st["pct"], 1),
                "rewritten": st["rewritten"], "flags_open": st["flags_open"],
                "files_done": st["files_done"], "xp": st["xp"], "level": st["level"],
                "level_title": st["level_title"], "achievements": len(self.data["achievements"]),
                "events": len(self.data["events"])}

    def _write(self, path, name, kind):
        payload = dict(self.data, meta=self.meta(name, kind))
        tmp = str(path) + ".tmp"
        with open(tmp, "w", encoding="utf-8") as f:
            json.dump(payload, f, indent=1, ensure_ascii=False)
        os.replace(tmp, str(path))

    def save(self):
        """Write the automatic save (after every change)."""
        self._write(AUTOSAVE, "Autosave", "auto")

    def heartbeat(self):
        """The page is open and visible: count the time since the last beat."""
        with self.lock:
            now = time.time()
            self.data["time_played"] += min(HEARTBEAT_MAX, max(0.0, now - self.last_beat))
            self.last_beat = now
            return self.data["time_played"]

    # Save slots

    @staticmethod
    def slot_name(name):
        """The file name for a manual save."""
        slug = re.sub(r"[^\w\- ]+", "", name).strip().replace(" ", "_")[:48] or "save"
        if slug.lower() == "autosave":
            slug = "save_" + slug
        return slug + ".json"

    def list_saves(self):
        """Every save file: the autosave first, then the manual ones, newest first."""
        out = []
        for path in SAVES_DIR.glob("*.json"):
            try:
                loaded = self._read(path)
            except (OSError, ValueError):
                continue
            meta = loaded.get("meta") or {"name": path.stem, "kind": "manual", "saved_at": ""}
            out.append(dict(meta, file=path.name, kind="auto" if path == AUTOSAVE else "manual"))
        auto = [m for m in out if m["kind"] == "auto"]
        manual = sorted((m for m in out if m["kind"] != "auto"),
                        key=lambda m: m.get("saved_at", ""), reverse=True)
        return auto + manual

    def save_slot(self, name, overwrite=False):
        """Write the current progress to a manual save file."""
        with self.lock:
            self.save()
            fname = self.slot_name(name)
            path = SAVES_DIR / fname
            if path.exists() and not overwrite:
                return {"exists": True, "file": fname}
            self._write(path, name.strip() or path.stem, "manual")
            self._event("save", None, name)
            self.save()
            return {"exists": False, "file": fname, "saves": self.list_saves()}

    def load_slot(self, fname):
        """Replace the current progress with a save file's (the autosave follows)."""
        path = SAVES_DIR / os.path.basename(fname)
        if not path.is_file():
            raise ValueError("No such save file: %s" % fname)
        with self.lock:
            loaded = self._read(path)
            self._adopt(loaded)
            self.last_beat = time.time()
            self._event("load", None, (loaded.get("meta") or {}).get("name", path.stem))
            self._write_all_work()
            self.save()
            return self.list_saves()

    def delete_slot(self, fname):
        """Delete a manual save file (never the autosave)."""
        path = SAVES_DIR / os.path.basename(fname)
        if path == AUTOSAVE:
            raise ValueError("The autosave cannot be deleted.")
        if path.is_file():
            path.unlink()
        return self.list_saves()

    def _event(self, kind, key=None, detail=None):
        c = self.catalog.chunks.get(key) if key else None
        ev = {"at": now_iso(), "type": kind}
        if c:
            ev.update({"key": key, "file": c["file"], "title": c["title"], "index": c["index"],
                       "chapter": c["chapter"]})
        if detail:
            ev["detail"] = detail
        self.data["events"].append(ev)
        if len(self.data["events"]) > MAX_EVENTS:
            del self.data["events"][:len(self.data["events"]) - MAX_EVENTS]
        return ev

    # Chunk actions

    def record(self, key):
        return self.data["chunks"].get(key)

    def _set(self, key, **fields):
        rec = self.data["chunks"].setdefault(key, {})
        rec.update(fields)
        return rec

    def accept(self, key):
        """Accept a chunk as it is (a rewrite, if any, is dropped)."""
        with self.lock:
            c = self.catalog.chunks[key]
            rec = self._set(key, status="accepted", at=now_iso())
            rec.pop("rewrite", None)
            rec.pop("code_changed", None)
            self._event("accept", key)
            self._write_work(c["file"])
            return self._after(key)

    def rewrite(self, key, text):
        """Save a rewrite of a chunk, in the player's words."""
        with self.lock:
            c = self.catalog.chunks[key]
            if text == c["text"]:
                return self.accept(key)
            check = compare_code(c["text"], text, c["family"])
            rec = self._set(key, status="rewritten", at=now_iso(), rewrite=text,
                            code_changed=check.get("changed"))
            self._event("rewrite", key, "code changed" if check.get("changed") else None)
            self._write_work(c["file"])
            out = self._after(key)
            out["check"] = check
            return out

    def skip(self, key):
        """Leave a chunk for later (it comes back after the pending ones)."""
        with self.lock:
            rec = self.record(key)
            if rec and rec.get("status") in ("accepted", "rewritten"):
                return self._after(key)
            self._set(key, status="skipped", at=now_iso())
            self._event("skip", key)
            return self._after(key)

    def reset(self, key):
        """Put a chunk back to pending (its rewrite is dropped, its flag kept)."""
        with self.lock:
            c = self.catalog.chunks[key]
            rec = self.record(key)
            if rec:
                for f in ("status", "at", "rewrite", "code_changed"):
                    rec.pop(f, None)
                if not rec:
                    del self.data["chunks"][key]
            self._event("reset", key)
            self._write_work(c["file"])
            return self._after(key)

    def flag(self, key, note):
        """Flag a chunk to come back to, with a note."""
        with self.lock:
            rec = self.record(key) or {}
            old = rec.get("flag")
            if old and not old.get("resolved"):
                old["note"] = note
                old["edited"] = now_iso()
                self._event("flag_edit", key, note)
            else:
                self._set(key, flag={"note": note, "at": now_iso(), "resolved": None})
                self._event("flag", key, note)
            return self._after(key)

    def unflag(self, key):
        """Remove a chunk's flag altogether."""
        with self.lock:
            rec = self.record(key)
            if rec and "flag" in rec:
                del rec["flag"]
                if not rec:
                    del self.data["chunks"][key]
                self._event("unflag", key)
            return self._after(key)

    def resolve(self, key, resolved=True):
        """Mark a flag resolved (or open again)."""
        with self.lock:
            rec = self.record(key)
            if rec and rec.get("flag"):
                rec["flag"]["resolved"] = now_iso() if resolved else None
                self._event("resolve" if resolved else "reopen", key)
            return self._after(key)

    def set_settings(self, values):
        with self.lock:
            self.data["settings"].update(values)
            self.save()
            return self.data["settings"]

    def _after(self, key):
        """Save, check achievements, and return what changed."""
        unlocked = self._check_achievements()
        milestones = self._milestones(key)
        self.save()
        return {"record": self.record(key), "unlocked": unlocked, "milestones": milestones,
                "summary": self.summary()}

    # The work copy

    def file_text(self, rel):
        """A file's text with every rewrite in place."""
        rec = self.catalog.by_file[rel]
        parts = []
        for key in rec["keys"]:
            r = self.record(key)
            if r and r.get("status") == "rewritten":
                parts.append(r["rewrite"])
            else:
                parts.append(self.catalog.chunks[key]["text"])
        return "".join(parts)

    def file_changed(self, rel):
        rec = self.catalog.by_file[rel]
        return any((self.record(k) or {}).get("status") == "rewritten" for k in rec["keys"])

    def _write_work(self, rel):
        path = WORK_DIR / rel
        if self.file_changed(rel):
            path.parent.mkdir(parents=True, exist_ok=True)
            text = self.file_text(rel)
            if not path.is_file() or path.read_text(encoding="utf-8") != text:
                path.write_text(text, encoding="utf-8", newline="")
        elif path.is_file():
            path.unlink()

    def apply(self, rels):
        """Copy the work copies of some files over the project's, with a backup.

        Returns:
            A list of {rel, ok, note}.
        """
        stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        out = []
        with self.lock:
            for rel in rels:
                src = WORK_DIR / rel
                dst = REPO / rel
                if not src.is_file():
                    out.append({"rel": rel, "ok": False, "note": "no rewrite to apply"})
                    continue
                try:
                    if dst.is_file():
                        bak = BACKUP_DIR / stamp / rel
                        bak.parent.mkdir(parents=True, exist_ok=True)
                        shutil.copy2(dst, bak)
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(src, dst)
                    out.append({"rel": rel, "ok": True, "note": "copied (backup in audit/backup/%s)" % stamp})
                except OSError as e:
                    out.append({"rel": rel, "ok": False, "note": str(e)})
            self._event("apply", None, "%d file(s)" % sum(1 for o in out if o["ok"]))
            self.save()
        return out

    def project_drift(self, rel):
        """Whether the project's file differs from the snapshot now."""
        try:
            return (REPO / rel).read_text(encoding="utf-8") != self.catalog.original_text(rel)
        except OSError:
            return True

    # Sums

    def status_of(self, key):
        r = self.record(key)
        if not r:
            return "pending"
        return r.get("status", "pending")

    def file_counts(self, rel):
        rec = self.catalog.by_file[rel]
        counts = {"total": len(rec["keys"]), "accepted": 0, "rewritten": 0, "skipped": 0,
                  "flags": 0, "open_flags": 0}
        for k in rec["keys"]:
            r = self.record(k)
            if not r:
                continue
            st = r.get("status")
            if st in counts:
                counts[st] += 1
            if r.get("flag"):
                counts["flags"] += 1
                if not r["flag"].get("resolved"):
                    counts["open_flags"] += 1
        counts["cleared"] = counts["accepted"] + counts["rewritten"]
        return counts

    def summary(self):
        """Totals per chapter, section and file, and the player's standing."""
        chapters = []
        total = 0
        cleared = 0
        for ch in self.catalog.chapters:
            ch_counts = {"total": 0, "cleared": 0, "rewritten": 0, "flags": 0, "open_flags": 0,
                         "skipped": 0}
            sections = []
            for sec in ch["sections"]:
                s_counts = {"total": 0, "cleared": 0, "rewritten": 0, "flags": 0,
                            "open_flags": 0, "skipped": 0}
                files = []
                for rel in sec["files"]:
                    fc = self.file_counts(rel)
                    files.append({"rel": rel, "name": rel.rsplit("/", 1)[-1],
                                  "lang": self.catalog.by_file[rel]["lang"], **fc})
                    for k in s_counts:
                        s_counts[k] += fc[k]
                sections.append({"title": sec["title"], "files": files, **s_counts})
                for k in ch_counts:
                    ch_counts[k] += s_counts[k]
            chapters.append({"id": ch["id"], "title": ch["title"], "icon": ch["icon"],
                             "blurb": ch["blurb"], "sections": sections, **ch_counts})
            total += ch_counts["total"]
            cleared += ch_counts["cleared"]
        st = self.stats()
        return {"chapters": chapters, "total": total, "cleared": cleared,
                "pct": (100.0 * cleared / total) if total else 0, "xp": st["xp"],
                "level": st["level"], "level_title": st["level_title"],
                "level_xp": st["level_xp"], "next_xp": st["next_xp"], "streak": st["streak"],
                "today": st["today"], "today_rewrites": st["today_rewrites"],
                "rewritten": st["rewritten"], "accepted": st["accepted"],
                "flags_open": st["flags_open"], "flags_raised": st["flags_raised"],
                "files_done": st["files_done"], "files_total": len(self.catalog.files),
                "achievements": len(self.data["achievements"]),
                "achievements_total": len(ACHIEVEMENTS) + sum(
                    1 + len(c["sections"]) for c in self.catalog.chapters),
                "time_played": round(self.data["time_played"])}

    def stats(self):
        accepted = rewritten = skipped = 0
        flags_raised = flags_resolved = code_changes = 0
        chapters_touched = set()
        for key, r in self.data["chunks"].items():
            c = self.catalog.chunks.get(key)
            st = r.get("status")
            if st == "accepted":
                accepted += 1
            elif st == "rewritten":
                rewritten += 1
                if r.get("code_changed"):
                    code_changes += 1
            elif st == "skipped":
                skipped += 1
            if st in ("accepted", "rewritten") and c:
                chapters_touched.add(c["chapter"])
            if r.get("flag"):
                flags_raised += 1
                if r["flag"].get("resolved"):
                    flags_resolved += 1
        files_done = 0
        big_file_done = False
        for rec in self.catalog.files:
            fc = self.file_counts(rec["rel"])
            if fc["total"] and fc["cleared"] == fc["total"]:
                files_done += 1
                if fc["total"] >= 80:
                    big_file_done = True
        cleared = accepted + rewritten
        total = len(self.catalog.order)
        xp = accepted * XP_ACCEPT + rewritten * XP_REWRITE + flags_resolved * XP_RESOLVE
        level = int((xp / 150.0) ** 0.5) + 1
        today_s = date.today().isoformat()
        today = sum(1 for e in self.data["events"]
                    if e["type"] in ("accept", "rewrite") and e["at"].startswith(today_s))
        today_rewrites = sum(1 for e in self.data["events"]
                             if e["type"] == "rewrite" and e["at"].startswith(today_s))
        return {
            "accepted": accepted, "rewritten": rewritten, "skipped": skipped, "cleared": cleared,
            "total": total, "pct": (100.0 * cleared / total) if total else 0.0,
            "flags_raised": flags_raised, "flags_resolved": flags_resolved,
            "flags_open": flags_raised - flags_resolved, "code_changes": code_changes,
            "files_done": files_done, "big_file_done": big_file_done,
            "chapters_touched": len(chapters_touched), "chapters": len(self.catalog.chapters),
            "today": today, "today_rewrites": today_rewrites, "hour": datetime.now().hour,
            "streak": self.streak(), "xp": xp, "level": level,
            "level_title": LEVEL_TITLES[min(level, len(LEVEL_TITLES)) - 1],
            "level_xp": 150 * (level - 1) ** 2, "next_xp": 150 * level ** 2,
        }

    def streak(self):
        days = {e["at"][:10] for e in self.data["events"] if e["type"] in ("accept", "rewrite")}
        if not days:
            return 0
        d = date.today()
        if d.isoformat() not in days:
            d = d - timedelta(days=1)
            if d.isoformat() not in days:
                return 0
        n = 0
        while d.isoformat() in days:
            n += 1
            d = d - timedelta(days=1)
        return n

    def days(self):
        """Chunks cleared per day, for the activity chart."""
        out = {}
        for e in self.data["events"]:
            if e["type"] in ("accept", "rewrite"):
                out[e["at"][:10]] = out.get(e["at"][:10], 0) + 1
        return out

    # Achievements

    def achievements(self):
        """Every achievement, static and per section and chapter, with when it was earned."""
        got = self.data["achievements"]
        out = []
        for aid, title, desc, _ in ACHIEVEMENTS:
            out.append({"id": aid, "title": title, "desc": desc, "at": got.get(aid),
                        "group": "general"})
        for ch in self.catalog.chapters:
            for sec in ch["sections"]:
                aid = "section:%s/%s" % (ch["id"], sec["title"])
                out.append({"id": aid, "title": "%s cleared" % sec["title"],
                            "desc": "Every chunk of %s in %s." % (sec["title"], ch["title"]),
                            "at": got.get(aid), "group": "sections", "chapter": ch["id"]})
            aid = "chapter:%s" % ch["id"]
            out.append({"id": aid, "title": "Chapter complete: %s" % ch["title"],
                        "desc": "Every chunk of the %s chapter." % ch["title"],
                        "at": got.get(aid), "group": "chapters", "chapter": ch["id"]})
        return out

    def _check_achievements(self):
        st = self.stats()
        got = self.data["achievements"]
        new = []
        for aid, title, desc, test in ACHIEVEMENTS:
            if aid not in got and test(st):
                got[aid] = now_iso()
                new.append({"id": aid, "title": title, "desc": desc})
        for ch in self.catalog.chapters:
            all_done = True
            for sec in ch["sections"]:
                done = all(self.status_of(k) in ("accepted", "rewritten")
                           for rel in sec["files"] for k in self.catalog.by_file[rel]["keys"])
                aid = "section:%s/%s" % (ch["id"], sec["title"])
                if done and aid not in got:
                    got[aid] = now_iso()
                    new.append({"id": aid, "title": "%s cleared" % sec["title"],
                                "desc": "Every chunk of %s in %s." % (sec["title"], ch["title"])})
                all_done = all_done and done
            aid = "chapter:%s" % ch["id"]
            if all_done and ch["sections"] and aid not in got:
                got[aid] = now_iso()
                new.append({"id": aid, "title": "Chapter complete: %s" % ch["title"],
                            "desc": "Every chunk of the %s chapter." % ch["title"]})
        for a in new:
            self._event("achievement", None, a["title"])
        return new

    def _milestones(self, key):
        """Whether this action finished the chunk's file."""
        c = self.catalog.chunks[key]
        fc = self.file_counts(c["file"])
        return {"file_done": fc["cleared"] == fc["total"], "file": c["file"], **fc}

    # Navigation

    def next_pending(self, after_key=None, same_file=False):
        """The next chunk to do, in reading order from after_key (wrapping).

        Pending chunks first; when none are left, skipped ones.
        """
        order = self.catalog.order
        start = 0
        if after_key in self.catalog.chunks:
            start = self.catalog.chunks[after_key]["ordinal"] + 1
        n = len(order)
        rel = self.catalog.chunks[after_key]["file"] if after_key in self.catalog.chunks else None
        for want in ("pending", "skipped"):
            for i in range(n):
                key = order[(start + i) % n]
                if same_file and self.catalog.chunks[key]["file"] != rel:
                    continue
                if self.status_of(key) == want:
                    return key
        return None
