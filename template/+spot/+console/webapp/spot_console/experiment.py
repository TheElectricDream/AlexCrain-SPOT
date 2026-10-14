"""Experiments on the platforms, run from the lab console: finding the build, deploying it, starting
and stopping it, following the platforms' clocks, and collecting the data files.

Part of the SPOT Console (webapp/app.py starts it).
"""

import collections
import io
import math
import re
import socket
import struct
import tarfile
import threading
import time
from pathlib import Path

from .common import ApiError, decode, shown, shown_list
from .events import hub, settings
from .hardware import StepFailed, hardware, in_parallel, platform_run
from .phasespace import streamer


EXP_BUILD_ROOT = Path.home() / "ExperimentSoftware" / "MATLAB_ws"


EXP_DATA_ROOT = Path.home() / "ExperimentData"


EXP_CLOCK_PORTS = {"RED": 48720, "BLACK": 48721, "BLUE": 48722}   # each platform sends its experiment time

# The agreed start: once every platform's program is running (each sends its clock, still 0),
# StreamData is told to start the experiment clock this many seconds later, in the PhaseSpace
# stream all platforms receive (spot.hardware.experimentTime), so they start together.
SYNC_COUNTDOWN = 3.0
SYNC_READY_TIMEOUT = 90.0   # [s] for every program to start sending its clock


# The platforms' debug messages (spot.hardware.debugMessages): [experiment time, double][text],
# each platform on its own port. The original diagram broadcast the bare text to 55000 instead;
# that is still understood (the platform is known from its address).
EXP_DEBUG_PORTS = {"RED": 55001, "BLACK": 55002, "BLUE": 55003}
EXP_DEBUG_LEGACY_PORT = 55000
DEBUG_KEEP = 300   # messages kept per platform (only changes are kept)
DEBUG_SHOW = 100   # of which the status carries the newest

# The Live Stream blocks' signals (spot.hardware.liveStream), for the Live Plot: per platform port,
# [uint8 version 1][uint8 name length L][uint16 count n][double t][L name characters][n doubles]
EXP_LIVE_PORTS = {"RED": 55011, "BLACK": 55012, "BLUE": 55013}
LIVE_KEEP = 12000      # samples kept per stream (10 min at 20 Hz)
LIVE_PUBLISH = 0.2     # [s] new samples go to the pages this often


NAME_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,99}$")


class Experiment:
    """One experiment at a time for the whole lab: deploy, start, stop, collect the data."""

    def __init__(self):
        self._lock = threading.Lock()
        self.phase = "idle"   # idle, deploying, deployed, starting, running, stopping, stopped
        self.model = None
        self.build = None     # {"path", "built", "size"} of the program deployed
        self.platforms = {}   # NAME -> {"state", "detail", "pid", "missing", "data", "log"}
        self.by = None
        self.started = None
        self.stopped = None
        self.note = None
        self.last_run = None  # {"run", "files"} from the last data collection
        self.clocks = {}      # NAME -> [experiment time, received]
        self.sync = {"state": "idle"}   # the agreed start: idle, waiting, counting, running, failed
        self.debug = {n: [] for n in EXP_DEBUG_PORTS}   # NAME -> [{t, text, first, last, count}]
        self.debug_seen = {}  # NAME -> when its last message (changed or not) arrived
        self.live = {}        # "PLATFORM/name" -> {"platform", "name", "n", "rows": deque of [t, v...], "last"}
        self.live_new = {}    # "PLATFORM/name" -> rows not yet sent to the pages

    def status(self):
        with self._lock:
            now = time.time()
            return {"phase": self.phase, "model": self.model, "build": self.build, "by": self.by,
                    "started": self.started, "stopped": self.stopped, "note": self.note,
                    "platforms": {n: dict(p) for n, p in self.platforms.items()},
                    "last_run": self.last_run,
                    "clocks": {n: {"t": c[0], "age": round(now - c[1], 2)} for n, c in self.clocks.items()},
                    "sync": dict(self.sync, spread_ms=self._spread_ms(now)),
                    "live": [{"key": k, "platform": s["platform"], "name": s["name"], "n": s["n"],
                              "samples": len(s["rows"]), "age": round(now - s["last"], 1)}
                             for k, s in sorted(self.live.items())],
                    "debug": {n: {"entries": e[-DEBUG_SHOW:],
                                  "seen": round(now - self.debug_seen[n], 1) if n in self.debug_seen else None}
                              for n, e in self.debug.items()},
                    "build_root": str(EXP_BUILD_ROOT)}

    def _spread_ms(self, now):
        """How far apart the platforms' experiment clocks are now [ms], or None. Each clock is
        moved on by the time since it arrived; the network's delays are part of the result."""
        fresh = [c[0] + (now - c[1]) for n, c in self.clocks.items()
                 if self.platforms.get(n, {}).get("state") == "running" and now - c[1] < 1.0 and c[0] > 0]
        if len(fresh) < 2:
            return None
        return round(1000 * (max(fresh) - min(fresh)), 1)

    def _publish(self):
        hub.publish("experiment", self.status())

    def _update(self, name, **fields):
        with self._lock:
            self.platforms.setdefault(name, {}).update(fields)
        self._publish()

    def _log(self, line):
        hub.log("hardware", "%s  Experiment: %s" % (time.strftime("%H:%M:%S"), line))

    def _mark_crashed(self, results):
        for n, r in results.items():
            if isinstance(r, StepFailed):
                self._update(n, state="failed", detail="internal error: %s" % r.error)

    # ---- the build MATLAB left on this computer

    @staticmethod
    def find_build(model):
        """The newest <model>.elf under EXP_BUILD_ROOT, or None."""
        if not NAME_RE.match(model or ""):
            raise ApiError("%r is not a diagram name." % model)
        best = None
        if EXP_BUILD_ROOT.is_dir():
            for path in EXP_BUILD_ROOT.rglob(model + ".elf"):
                try:
                    mtime = path.stat().st_mtime
                except OSError:
                    continue
                if path.is_file() and (best is None or mtime > best[1]):
                    best = (path, mtime)
        return best

    def build_info(self, model):
        found = self.find_build(model)
        if not found:
            return None
        path, mtime = found
        return {"path": str(path), "built": mtime, "size": path.stat().st_size}

    # ---- deploy, start, stop

    def _check_platforms(self, platforms):
        names = []
        for n in platforms or []:
            n = str(n).upper()
            hardware.get(n)   # unknown names raise
            if n not in names:
                names.append(n)
        if not names:
            raise ApiError("Choose at least one platform.")
        return names

    def deploy(self, model, platforms, requester):
        names = self._check_platforms(platforms)
        info = self.build_info(model)
        if not info:
            raise ApiError("There is no build of %s on the lab console yet (looked for %s.elf under %s). "
                           "Build it from the local console first." % (model, model, EXP_BUILD_ROOT))
        with self._lock:
            if self.phase in ("deploying", "starting", "running", "stopping"):
                raise ApiError("An experiment is %s (by %s); wait for it, or stop it first." % (self.phase, self.by))
            self.phase, self.model, self.build, self.by, self.note = "deploying", model, info, requester, None
            self.started = self.stopped = None
            self.platforms = {n: {"state": "copying", "detail": None, "pid": None, "missing": [], "data": None,
                                  "log": []} for n in names}
        self._log("deploying %s to %s (by %s)" % (model, shown_list(names), requester))
        self._publish()
        threading.Thread(target=self._deploy, args=(model, names, Path(info["path"])), daemon=True).start()
        return self.status()

    def _deploy(self, model, names, elf):
        # Only what the program needs to run: itself and any shared libraries built next to it
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode="w") as tar:
            tar.add(str(elf), arcname=elf.name)
            for lib in sorted(elf.parent.glob("*.so*")):
                if lib.is_file():
                    tar.add(str(lib), arcname=lib.name)
        data = buf.getvalue()

        def one(n):
            rc, out, err = platform_run(n, "exp-receive " + model, data=data, timeout=180)
            text = decode(out)
            missing = [l[len("missing "):] for l in text.splitlines() if l.startswith("missing ")]
            if rc == 0:
                self._update(n, state="deployed", detail=text.splitlines()[-1] if text else "copied",
                             missing=missing)
            else:
                self._update(n, state="failed", detail=err or "copy failed (code %s)" % rc)
            return rc == 0
        ok = in_parallel(names, one)
        self._mark_crashed(ok)
        with self._lock:
            self.phase = "deployed" if all(ok.values()) else "idle"
            self.note = None if all(ok.values()) else "Copying failed on %s." % shown_list(n for n, v in ok.items() if not v)
        self._log("deploy %s" % ("done" if all(ok.values()) else "failed: " + self.note))
        self._publish()

    def start(self, platforms, requester):
        names = self._check_platforms(platforms)
        with self._lock:
            if self.phase not in ("deployed", "stopped"):
                raise ApiError("Deploy the experiment first (it is %s)." % self.phase)
            not_here = [n for n in names if self.platforms.get(n, {}).get("state") not in ("deployed", "stopped", "exited")]
            if not_here:
                raise ApiError("%s %s not been deployed to." % (shown_list(not_here), "has" if len(not_here) == 1 else "have"))
            model = self.model
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first: the platforms take their experiment clock from it, "
                           "and wait at time 0 without it.")
        from .calibration import calibrator   # (calibration.py imports this module)
        if calibrator.active():
            raise ApiError("A calibration is running on the lab console (Calibration tab); start the experiment "
                           "once it has finished, or stop it there.")
        for n in names:   # the platform has to answer, with its Teensy free
            pr = hardware.get(n).status()["probe"]
            if pr["state"] != "ok":
                raise ApiError("%s: %s" % (shown(n), pr["detail"] or pr["state"]))
            if not pr["teensy"]:
                raise ApiError("The %s has no Teensy at /dev/ttyACM0." % shown(n))
            if pr["others"]:
                raise ApiError("The %s's Teensy is already in use by %s." % (shown(n), ", ".join(pr["others"])))
        with self._lock:
            self.phase, self.by, self.started, self.stopped, self.note = "starting", requester, time.time(), None, None
            self.clocks = {}
            self.sync = {"state": "waiting", "waiting_for": list(names)}
            self.debug = {n: [] for n in EXP_DEBUG_PORTS}
            self.debug_seen = {}
            self.live, self.live_new = {}, {}
        streamer.command("exp clear")   # no clock from an earlier run: the new programs wait at 0
        self._log("starting %s on %s (by %s)" % (model, shown_list(names), requester))
        for n in names:
            hardware.get(n).make_way(requester)
        self._publish()
        time.sleep(0.5)   # let the hardware links close

        def one(n):
            rc, out, err = platform_run(n, "exp-start %s %s.elf" % (model, model), timeout=30)
            m = re.search(r"(started|already running) (\d+)", decode(out))
            if rc == 0 and m:
                self._update(n, state="running", pid=int(m.group(2)), detail=None)
                return True
            self._update(n, state="failed", detail=err or decode(out) or "could not start (code %s)" % rc)
            return False
        ok = in_parallel(names, one)
        self._mark_crashed(ok)
        if not all(ok.values()):
            # Don't leave half an experiment running
            self.stop("the console (a platform failed to start)", note="Starting failed on %s, so the others were stopped."
                      % ", ".join(n for n, v in ok.items() if not v))
            return self.status()
        with self._lock:
            self.phase = "running"
        self._publish()
        threading.Thread(target=self._monitor, daemon=True).start()
        threading.Thread(target=self._agree_start, args=(names,), daemon=True).start()
        return self.status()

    def _agree_start(self, names):
        """Once every platform's program runs (it sends its clock, still at 0), start the
        experiment clock in the PhaseSpace stream SYNC_COUNTDOWN s later, for all of them."""
        with self._lock:
            started = self.started
        deadline = time.time() + SYNC_READY_TIMEOUT
        while True:
            with self._lock:
                if self.phase != "running" or self.started != started:
                    return
                waiting = [n for n in names if not (n in self.clocks and self.clocks[n][1] > started)]
                self.sync = {"state": "waiting", "waiting_for": waiting}
            if not waiting:
                break
            if time.time() > deadline:
                with self._lock:
                    self.sync = {"state": "failed", "waiting_for": waiting,
                                 "detail": "No experiment clock from %s within %d s, so the experiment was not started "
                                           "(the platforms wait at time 0). Is the program built from a diagram with the "
                                           "PhaseSpace experiment clock?" % (shown_list(waiting), SYNC_READY_TIMEOUT)}
                self._log("no experiment clock from %s: not started" % shown_list(waiting))
                self._publish()
                return
            time.sleep(0.2)
        if not streamer.status()["running"]:
            with self._lock:
                self.sync = {"state": "failed", "detail": "PhaseSpace streaming stopped, so the experiment could not start."}
            self._publish()
            return
        streamer.command("exp start %.3f" % SYNC_COUNTDOWN)
        with self._lock:
            self.sync = {"state": "counting", "start_at": time.time() + SYNC_COUNTDOWN}
        self._log("every platform is running: the experiment starts in %.0f s" % SYNC_COUNTDOWN)
        self._publish()
        time.sleep(SYNC_COUNTDOWN)
        with self._lock:
            if self.sync.get("state") == "counting" and self.started == started:
                self.sync = {"state": "running", "since": time.time()}
        self._publish()

    def _monitor(self):
        while True:
            time.sleep(2)
            with self._lock:
                if self.phase != "running":
                    return
                names = [n for n, p in self.platforms.items() if p.get("state") == "running"]
                model = self.model
            def one(n):
                rc, out, _ = platform_run(n, "exp-status " + model, timeout=15)
                lines = decode(out).splitlines()
                running = any(re.match(r"running \d+", l) for l in lines)
                data = next((int(l.split()[1]) for l in lines if l.startswith("data ")), None)
                log = [l[4:] for l in lines if l.startswith("log ")]
                if rc == 0:
                    self._update(n, state="running" if running else "exited", data=data, log=log)
                return running
            alive = in_parallel(names, one)
            crashed = any(isinstance(v, StepFailed) for v in alive.values())
            if names and not crashed and not any(alive.values()):
                with self._lock:
                    if self.phase == "running":
                        self.phase, self.stopped = "stopped", time.time()
                        self.note = "The program has ended on every platform."
                self._log("the program has ended on every platform")
                self._publish()
                return

    def stop(self, requester, note=None):
        with self._lock:
            names = [n for n, p in self.platforms.items() if p.get("state") in ("running", "exited", "failed")]
            if self.phase not in ("starting", "running", "stopped") or not names:
                return self.status()
            self.phase = "stopping"
            model = self.model
        self._log("stopping %s on %s (by %s)" % (model, shown_list(names), requester))
        self._publish()

        def one(n):
            rc, out, err = platform_run(n, "exp-stop " + model, timeout=30)
            if rc == 0:
                self._update(n, state="stopped", pid=None, detail=decode(out).strip())
                hardware.get(n).send_off()   # thrusters and pucks off now, not after the watchdog
            else:
                self._update(n, state="failed", detail="could not stop: %s" % (err or rc))
            return rc == 0
        ok = in_parallel(names, one)
        self._mark_crashed(ok)
        streamer.command("exp clear")   # the experiment clock ends with the programs
        with self._lock:
            self.sync = {"state": "idle"}
            self.phase, self.stopped = "stopped", time.time()
            self.note = note if all(ok.values()) else \
                "Could not confirm the stop on %s: use the E-stop." % shown_list(n for n, v in ok.items() if not v)
        self._publish()
        return self.status()

    # ---- data

    def fetch(self, requester):
        with self._lock:
            if self.phase in ("deploying", "starting", "running", "stopping"):
                raise ApiError("Stop the experiment before collecting its data.")
            if not self.model or not self.platforms:
                raise ApiError("No experiment to collect data from.")
            model, names = self.model, list(self.platforms)
        run = "%s_%s" % (model, time.strftime("%Y%m%d_%H%M%S"))
        folder = EXP_DATA_ROOT / run

        def one(n):
            target = folder / n
            with self._lock:
                messages = list(self.debug.get(n, []))
            if messages:   # the platform's debug messages, as the console received them (also without data)
                target.mkdir(parents=True, exist_ok=True)
                (target / "debug_messages.txt").write_text(
                    "experiment time [s]\ttimes received\tmessage\n" + "".join(
                        "%s\t%d\t%s\n" % ("" if m["t"] is None else "%.3f" % m["t"], m["count"], m["text"]) for m in messages),
                    encoding="utf-8")
            rc, out, err = platform_run(n, "exp-data " + model, timeout=300)
            if rc != 0:
                return {"files": [], "error": err or "no data (code %s)" % rc}
            files = []
            target.mkdir(parents=True, exist_ok=True)
            try:
                with tarfile.open(fileobj=io.BytesIO(out), mode="r:") as tar:
                    for member in tar.getmembers():
                        name = Path(member.name).name
                        if member.isfile() and name.startswith("ExpLog") and NAME_RE.match(name):
                            with tar.extractfile(member) as src:
                                (target / name).write_bytes(src.read())
                            files.append(name)
            except (tarfile.TarError, OSError) as e:
                return {"files": files, "error": "could not unpack the data: %s" % e}
            if files:
                rc, _, _ = platform_run(n, "exp-clean " + model, timeout=30)   # copied off: free the platform's disk
                if rc == 0:
                    self._update(n, data=0)
            return {"files": files, "error": None}
        result = in_parallel(names, one)
        result = {n: ({"files": [], "error": r.error} if isinstance(r, StepFailed) else r) for n, r in result.items()}
        with self._lock:
            self.last_run = {"run": run, "files": result, "at": time.time()}
        self._log("collected data into %s (by %s)" % (folder, requester))
        self._publish()
        return self.status()

    def archive(self, run):
        """The collected data of a run as a .tar.gz (for the local console to download)."""
        if not NAME_RE.match(run or "") or not (EXP_DATA_ROOT / run).is_dir():
            raise ApiError("No data for %r." % run)
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode="w:gz") as tar:
            tar.add(str(EXP_DATA_ROOT / run), arcname=run)
        return buf.getvalue()

    # ---- the platforms' experiment clocks (UDP from the diagrams, to this computer)

    def listen_clocks(self):
        for name, port in EXP_CLOCK_PORTS.items():
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            try:
                sock.bind(("", port))
            except OSError as e:
                print("Not listening for %s's experiment time on UDP %d: %s" % (name, port, e))
                continue
            threading.Thread(target=self._clock_loop, args=(name, sock), daemon=True).start()

    def _clock_loop(self, name, sock):
        last_publish = 0.0
        while True:
            try:
                data = sock.recv(64)
            except OSError:
                return
            if len(data) < 8:
                continue
            t = struct.unpack("<d", data[:8])[0]
            if not math.isfinite(t):
                continue
            with self._lock:
                self.clocks[name] = [t, time.time()]
            if time.time() - last_publish > 0.5:
                last_publish = time.time()
                self._publish()


    # ---- the platforms' debug messages (UDP from the diagrams, to this computer)

    def listen_debug(self):
        ports = [(name, port) for name, port in EXP_DEBUG_PORTS.items()] + [(None, EXP_DEBUG_LEGACY_PORT)]
        for name, port in ports:
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            try:
                sock.bind(("", port))
            except OSError as e:
                print("Not listening for debug messages on UDP %d: %s" % (port, e))
                continue
            threading.Thread(target=self._debug_loop, args=(name, sock), daemon=True).start()

    def _debug_loop(self, name, sock):
        while True:
            try:
                data, addr = sock.recvfrom(4096)
            except OSError:
                return
            if name is None:   # the original diagram: the bare text; the platform from its address
                hosts = {p.get("host"): n for n, p in (settings.get("platforms") or {}).items()}
                plat, t, raw = hosts.get(addr[0]), None, data
            else:
                if len(data) < 8:
                    continue
                plat, raw = name, data[8:]
                t = struct.unpack("<d", data[:8])[0]
                t = round(t, 3) if math.isfinite(t) else None
            if plat not in self.debug:
                continue
            self.add_debug(plat, t, raw.split(b"\0")[0].decode("ascii", "replace").strip())

    def add_debug(self, name, t, text):
        """A platform's message: a change starts a new entry, a repeat only counts."""
        now = time.time()
        with self._lock:
            entries = self.debug[name]
            self.debug_seen[name] = now
            if entries and entries[-1]["text"] == text:
                entries[-1]["count"] += 1
                entries[-1]["last"] = now
                return
            if not text and not entries:
                return   # nothing to show yet
            entries.append({"t": t, "text": text, "first": now, "last": now, "count": 1})
            del entries[:-DEBUG_KEEP]
        self._publish()


    # ---- the Live Stream blocks' signals, for the Live Plot

    def listen_live(self):
        for name, port in EXP_LIVE_PORTS.items():
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1 << 20)
            try:
                sock.bind(("", port))
            except OSError as e:
                print("Not listening for live streams on UDP %d: %s" % (port, e))
                continue
            threading.Thread(target=self._live_loop, args=(name, sock), daemon=True).start()
        threading.Thread(target=self._live_publisher, daemon=True).start()

    def _live_loop(self, platform, sock):
        while True:
            try:
                data = sock.recv(65535)
            except OSError:
                return
            row = parse_live(data)
            if row:
                self.add_live(platform, *row)

    def add_live(self, platform, name, t, values):
        key = "%s/%s" % (platform, name)
        now = time.time()
        with self._lock:
            s = self.live.get(key)
            if s is None or s["n"] != len(values):   # new (or its size changed: start again)
                s = self.live[key] = {"platform": platform, "name": name, "n": len(values),
                                      "rows": collections.deque(maxlen=LIVE_KEEP), "last": now}
                self.live_new[key] = []
            row = [round(t, 4)] + values
            s["rows"].append(row)
            s["last"] = now
            self.live_new.setdefault(key, []).append(row)

    def _live_publisher(self):
        """New samples to the pages a few times a second (event live_data), not one event per packet."""
        known = 0
        while True:
            time.sleep(LIVE_PUBLISH)
            with self._lock:
                batch = {k: {"platform": self.live[k]["platform"], "name": self.live[k]["name"],
                             "n": self.live[k]["n"], "rows": rows}
                         for k, rows in self.live_new.items() if rows and k in self.live}
                self.live_new = {k: [] for k in self.live}
                streams = len(self.live)
            if batch:
                hub.publish("live_data", batch)
            if streams != known:   # a new stream: the lists in the pages
                known = streams
                self._publish()

    def live_data(self, keep=3000):
        """Everything kept so far, at most keep samples per stream (for a page opened later)."""
        with self._lock:
            out = {}
            for k, s in self.live.items():
                rows = list(s["rows"])
                if len(rows) > keep:
                    step = len(rows) / float(keep)
                    rows = [rows[int(i * step)] for i in range(keep)] + [rows[-1]]
                out[k] = {"platform": s["platform"], "name": s["name"], "n": s["n"], "rows": rows}
            return out


def parse_live(data):
    """A Live Stream packet: (name, t, [values]), or None if it isn't one."""
    if len(data) < 12 or data[0] != 1:
        return None
    length = data[1]
    n = struct.unpack("<H", data[2:4])[0]
    if len(data) != 12 + length + 8 * n:
        return None
    t = struct.unpack("<d", data[4:12])[0]
    name = data[12:12 + length].decode("ascii", "replace").strip() or "stream"
    values = list(struct.unpack("<%dd" % n, data[12 + length:]))
    if not math.isfinite(t):
        return None
    return name, t, [v if math.isfinite(v) else None for v in values]


experiment = Experiment()


# In local mode (app.py --local, started by SPOT.m in MATLAB) the Simulation tab runs its commands
# in the user's MATLAB session: the MATLAB worker (+spot/+worker/Worker.m) connects to
# 127.0.0.1:<worker port>, proves it was started with this server's token, and then answers one
# JSON command per line. MATLAB runs one command at a time, so requests are sent one at a time.
