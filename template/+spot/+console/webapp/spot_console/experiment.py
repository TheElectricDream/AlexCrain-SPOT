"""Experiments on the platforms, run from the lab console.

Finding the build, deploying it, starting and stopping it, following
the platforms' clocks, debug messages and live streams, recording the
Orin's cameras (SPOT Vision, vision.py) with it, and collecting the
data files.

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

from . import vision
from .common import ApiError, decode, shown, shown_list
from .events import hub, settings
from .hardware import StepFailed, hardware, in_parallel, platform_run
from .phasespace import streamer


# Where builds and data are

EXP_BUILD_ROOT = Path.home() / "ExperimentSoftware" / "MATLAB_ws"

EXP_DATA_ROOT = Path.home() / "ExperimentData"

# Each platform sends its experiment time here.
EXP_CLOCK_PORTS = {"RED": 48720, "BLACK": 48721, "BLUE": 48722}

# The agreed start: once every platform's program is running (each
# sends its clock, still 0), StreamData is told to start the experiment
# clock this many seconds later, in the PhaseSpace stream all platforms
# receive (spot.hardware.experimentTime), so they start together.
SYNC_COUNTDOWN = 3.0
SYNC_READY_TIMEOUT = 90.0   # [s] for every program to start sending its clock

# The platforms' debug messages (spot.hardware.debugMessages):
# [experiment time, double][text], each platform on its own port. The
# original diagram broadcast the bare text to 55000 instead; that is
# still understood (the platform is known from its address).
EXP_DEBUG_PORTS = {"RED": 55001, "BLACK": 55002, "BLUE": 55003}
EXP_DEBUG_LEGACY_PORT = 55000
DEBUG_KEEP = 300   # messages kept per platform (only changes are kept)
DEBUG_SHOW = 100   # of which the status carries the newest

# The Live Stream blocks' signals (spot.hardware.liveStream), for the
# Live Plot, one port per platform: [uint8 version 1][uint8 name length
# L][uint16 count n][double t][L name characters][n doubles].
EXP_LIVE_PORTS = {"RED": 55011, "BLACK": 55012, "BLUE": 55013}
LIVE_KEEP = 12000      # samples kept per stream (10 min at 20 Hz)
LIVE_PUBLISH = 0.2     # [s] new samples go to the pages this often

NAME_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,99}$")

BUSY_PHASES = ("deploying", "starting", "running", "stopping")
CLOCK_FRESH = 1.0          # [s] a clock older than this isn't compared
CLOCK_PUBLISH = 0.5        # [s] clocks go to the pages at most this often
MONITOR_PERIOD = 2         # [s] between program checks while running
RECEIVE_BUFFER = 1 << 20   # [bytes] for the live streams' socket
DEPLOY_TIMEOUT = 180       # [s] agent command timeouts
START_TIMEOUT = 30
STATUS_TIMEOUT = 15
STOP_TIMEOUT = 30
DATA_TIMEOUT = 300
CLEAN_TIMEOUT = 30
RECORD_WAIT = 90           # [s] Collect waits this long for a recording to close


class Experiment:
    """One experiment at a time for the whole lab.

    Deploy, start, stop, and collect the data.
    """

    def __init__(self):
        """Start idle, with no experiment."""
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
        self.run = None       # the run's name, from its first collection
        self.clocks = {}      # NAME -> [experiment time, received]
        # The agreed start: idle, waiting, counting, running, failed.
        self.sync = {"state": "idle"}
        # NAME -> [{t, text, first, last, count}]
        self.debug = {n: [] for n in EXP_DEBUG_PORTS}
        self.debug_seen = {}  # NAME -> when its last message (changed or not) arrived
        # "PLATFORM/name" -> {"platform", "name", "n", "rows": deque of
        # [t, v...], "last"}
        self.live = {}
        self.live_new = {}    # "PLATFORM/name" -> rows not yet sent to the pages
        # The cameras recorded with the last start (SPOT Vision):
        # {"session", "cameras", "url", "state" (recording, stopping,
        # stopped, failed), "started", "result", "error"}; None
        # without.
        self.recording = None

    def status(self):
        """Return everything the Experiment tab shows."""
        with self._lock:
            now = time.time()
            return {
                "phase": self.phase, "model": self.model, "build": self.build, "by": self.by,
                "started": self.started, "stopped": self.stopped, "note": self.note,
                "platforms": {n: dict(p) for n, p in self.platforms.items()},
                "last_run": self.last_run,
                "recording": dict(self.recording) if self.recording else None,
                "clocks": {n: {"t": clock[0], "age": round(now - clock[1], 2)}
                           for n, clock in self.clocks.items()},
                "sync": dict(self.sync, spread_ms=self._spread_ms(now)),
                "live": [{"key": k, "platform": stream["platform"], "name": stream["name"],
                          "n": stream["n"], "samples": len(stream["rows"]),
                          "age": round(now - stream["last"], 1)}
                         for k, stream in sorted(self.live.items())],
                "debug": {n: {"entries": entries[-DEBUG_SHOW:],
                              "seen": round(now - self.debug_seen[n], 1)
                                      if n in self.debug_seen else None}
                          for n, entries in self.debug.items()},
                "build_root": str(EXP_BUILD_ROOT)}

    def _spread_ms(self, now):
        """Return how far apart the clocks are now [ms], or None.

        Each clock is moved on by the time since it arrived; the
        network's delays are part of the result.
        """
        fresh = [clock[0] + (now - clock[1]) for n, clock in self.clocks.items()
                 if self.platforms.get(n, {}).get("state") == "running"
                 and now - clock[1] < CLOCK_FRESH and clock[0] > 0]
        if len(fresh) < 2:
            return None
        return round(1000 * (max(fresh) - min(fresh)), 1)

    def _publish(self):
        """Send the status to the pages."""
        hub.publish("experiment", self.status())

    def _update(self, name, **fields):
        """Update a platform's entry and publish."""
        with self._lock:
            self.platforms.setdefault(name, {}).update(fields)
        self._publish()

    def _log(self, line):
        """Add a time-stamped line to the hardware log."""
        hub.log("hardware", "%s  Experiment: %s" % (time.strftime("%H:%M:%S"), line))

    def _mark_crashed(self, results):
        """Mark platforms whose step raised (StepFailed) failed."""
        for name, result in results.items():
            if isinstance(result, StepFailed):
                self._update(name, state="failed", detail="internal error: %s" % result.error)

    # The build MATLAB left on this computer

    @staticmethod
    def find_build(model):
        """Return the newest <model>.elf under EXP_BUILD_ROOT.

        Returns:
            (path, modification time), or None.

        Raises:
            ApiError: if model isn't a valid diagram name.
        """
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
        """Return the newest build's path, built, size; or None."""
        found = self.find_build(model)
        if not found:
            return None
        path, mtime = found
        return {"path": str(path), "built": mtime, "size": path.stat().st_size}

    # Deploy, start, stop

    def _check_platforms(self, platforms):
        """Return the platform keys, upper case, without repeats.

        Raises:
            ApiError: for an unknown platform, or none.
        """
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
        """Copy a diagram's newest build to platforms (background).

        Raises:
            ApiError: if there is no build, or an experiment is busy.
        """
        names = self._check_platforms(platforms)
        info = self.build_info(model)
        if not info:
            raise ApiError("There is no build of %s on the lab console yet (looked for %s.elf "
                           "under %s). Build it from the local console first."
                           % (model, model, EXP_BUILD_ROOT))
        with self._lock:
            if self.phase in BUSY_PHASES:
                raise ApiError("An experiment is %s (by %s); wait for it, or stop it first."
                               % (self.phase, self.by))
            self.phase, self.model, self.build, self.by, self.note = (
                "deploying", model, info, requester, None)
            self.started = self.stopped = None
            self.platforms = {n: {"state": "copying", "detail": None, "pid": None, "missing": [],
                                  "data": None, "log": []} for n in names}
        self._log("deploying %s to %s (by %s)" % (model, shown_list(names), requester))
        self._publish()
        threading.Thread(target=self._deploy, args=(model, names, Path(info["path"])),
                         daemon=True).start()
        return self.status()

    def _deploy(self, model, names, elf):
        """Copy the program to each platform at once; record it."""
        # Only what the program needs to run: itself and any shared
        # libraries built next to it.
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode="w") as tar:
            tar.add(str(elf), arcname=elf.name)
            for lib in sorted(elf.parent.glob("*.so*")):
                if lib.is_file():
                    tar.add(str(lib), arcname=lib.name)
        data = buf.getvalue()

        def one(key):
            """Copy to one platform; return whether it worked."""
            rc, out, err = platform_run(key, "exp-receive " + model, data=data,
                                        timeout=DEPLOY_TIMEOUT)
            text = decode(out)

            # Libraries the platform lacks, as "missing <name>" lines.
            missing = [line[len("missing "):] for line in text.splitlines()
                       if line.startswith("missing ")]
            if rc == 0:
                self._update(key, state="deployed",
                             detail=text.splitlines()[-1] if text else "copied", missing=missing)
            else:
                self._update(key, state="failed", detail=err or "copy failed (code %s)" % rc)
            return rc == 0

        ok = in_parallel(names, one)
        self._mark_crashed(ok)
        with self._lock:
            self.phase = "deployed" if all(ok.values()) else "idle"
            self.note = None if all(ok.values()) else "Copying failed on %s." % shown_list(
                n for n, v in ok.items() if not v)
        self._log("deploy %s" % ("done" if all(ok.values()) else "failed: " + self.note))
        self._publish()

    def start(self, platforms, requester, cameras=None):
        """Start the deployed program on platforms.

        Args:
            platforms: The platforms' keys.
            requester: Who asked.
            cameras: The Orin's cameras to record (their ids), or none.
                They all record before any program starts.

        Raises:
            ApiError: if it isn't deployed there, PhaseSpace isn't
                streaming, a calibration runs, a platform isn't ready,
                or a camera can't record.
        """
        names = self._check_platforms(platforms)

        # Deployed to every platform asked for.
        with self._lock:
            if self.phase not in ("deployed", "stopped"):
                raise ApiError("Deploy the experiment first (it is %s)." % self.phase)
            not_here = [n for n in names if self.platforms.get(n, {}).get("state")
                        not in ("deployed", "stopped", "exited")]
            if not_here:
                raise ApiError("%s %s not been deployed to."
                               % (shown_list(not_here), "has" if len(not_here) == 1 else "have"))
            model = self.model

        # PhaseSpace streams (the experiment clock comes from it), and
        # no calibration runs.
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first: the platforms take their "
                           "experiment clock from it, and wait at time 0 without it.")
        from .calibration import calibrator   # (calibration.py imports this module)
        if calibrator.active():
            raise ApiError("A calibration is running on the lab console (Calibration tab); "
                           "start the experiment once it has finished, or stop it there.")

        # Each platform answers, with its Teensy free.
        for n in names:
            probe = hardware.get(n).status()["probe"]
            if probe["state"] != "ok":
                raise ApiError("%s: %s" % (shown(n), probe["detail"] or probe["state"]))
            if not probe["teensy"]:
                raise ApiError("The %s has no Teensy at /dev/ttyACM0." % shown(n))
            if probe["others"]:
                raise ApiError("The %s's Teensy is already in use by %s."
                               % (shown(n), ", ".join(probe["others"])))

        # The cameras: every one recording before the programs start, or
        # the experiment doesn't start.
        cameras = [str(c) for c in cameras or []]
        recording = None
        if cameras:
            session = "%s_%s" % (model, time.strftime("%Y%m%d_%H%M%S"))
            try:
                vision.record_start(session, cameras)
            except ApiError as e:
                raise ApiError("The cameras couldn't start recording, so the experiment wasn't "
                               "started: %s" % e)
            recording = {"session": session, "cameras": cameras, "url": vision.url(),
                         "state": "recording", "started": time.time(), "result": None,
                         "error": None}
            self._log("recording %s on the Orin (%s)" % (", ".join(cameras), session))
        with self._lock:
            self.recording = recording

        # The programs; a recording doesn't outlive a start that failed.
        try:
            return self._start_programs(names, model, requester)
        except Exception:
            self._stop_recording()
            raise

    def _start_programs(self, names, model, requester):
        """Start the program on each platform; agree the start.

        start's second half, once everything is checked (and the
        cameras record).
        """
        # A fresh start: no clocks, messages or streams from an earlier
        # run, and no experiment clock in the stream (the new programs
        # wait at 0).
        with self._lock:
            self.phase, self.by, self.started, self.stopped, self.note = (
                "starting", requester, time.time(), None, None)
            self.run = None
            self.clocks = {}
            self.sync = {"state": "waiting", "waiting_for": list(names)}
            self.debug = {n: [] for n in EXP_DEBUG_PORTS}
            self.debug_seen = {}
            self.live, self.live_new = {}, {}
        streamer.command("exp clear")
        self._log("starting %s on %s (by %s)" % (model, shown_list(names), requester))

        # The console lets go of the platforms' Teensys.
        for n in names:
            hardware.get(n).make_way(requester)
        self._publish()
        time.sleep(0.5)   # let the hardware links close

        def one(key):
            """Start one platform's program; return if it runs."""
            rc, out, err = platform_run(key, "exp-start %s %s.elf" % (model, model),
                                        timeout=START_TIMEOUT)
            match = re.search(r"(started|already running) (\d+)", decode(out))
            if rc == 0 and match:
                self._update(key, state="running", pid=int(match.group(2)), detail=None)
                return True
            self._update(key, state="failed",
                         detail=err or decode(out) or "could not start (code %s)" % rc)
            return False

        ok = in_parallel(names, one)
        self._mark_crashed(ok)

        # Don't leave half an experiment running.
        if not all(ok.values()):
            self.stop("the console (a platform failed to start)",
                      note="Starting failed on %s, so the others were stopped."
                           % ", ".join(n for n, v in ok.items() if not v))
            return self.status()

        # Running: watch the programs, and agree the start.
        with self._lock:
            self.phase = "running"
        self._publish()
        threading.Thread(target=self._monitor, daemon=True).start()
        threading.Thread(target=self._agree_start, args=(names,), daemon=True).start()
        return self.status()

    def _agree_start(self, names):
        """Start the experiment clock for all platforms at once.

        Once every platform's program runs (it sends its clock, still
        at 0), the experiment clock starts in the PhaseSpace stream
        SYNC_COUNTDOWN s later.
        """
        with self._lock:
            started = self.started

        # Wait for every platform's clock (since this start).
        deadline = time.time() + SYNC_READY_TIMEOUT
        while True:
            with self._lock:
                if self.phase != "running" or self.started != started:
                    return
                waiting = [n for n in names
                           if not (n in self.clocks and self.clocks[n][1] > started)]
                self.sync = {"state": "waiting", "waiting_for": waiting}
            if not waiting:
                break
            if time.time() > deadline:
                with self._lock:
                    self.sync = {
                        "state": "failed", "waiting_for": waiting,
                        "detail": "No experiment clock from %s within %d s, so the experiment "
                                  "was not started (the platforms wait at time 0). Is the "
                                  "program built from a diagram with the PhaseSpace "
                                  "experiment clock?" % (shown_list(waiting), SYNC_READY_TIMEOUT)}
                self._log("no experiment clock from %s: not started" % shown_list(waiting))
                self._publish()
                return
            time.sleep(0.2)

        # The clock comes from StreamData: it must still stream.
        if not streamer.status()["running"]:
            with self._lock:
                self.sync = {"state": "failed",
                             "detail": "PhaseSpace streaming stopped, so the experiment could not "
                                       "start."}
            self._publish()
            return

        # Count down, then running.
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
        """Check the programs until all have ended."""
        while True:
            time.sleep(MONITOR_PERIOD)
            with self._lock:
                if self.phase != "running":
                    return
                names = [n for n, p in self.platforms.items() if p.get("state") == "running"]
                model = self.model

            def one(key):
                """Check one platform's program; return whether it runs.

                Also records its data size and log lines.
                """
                rc, out, _ = platform_run(key, "exp-status " + model, timeout=STATUS_TIMEOUT)
                lines = decode(out).splitlines()
                running = any(re.match(r"running \d+", line) for line in lines)
                data = next((int(line.split()[1]) for line in lines if line.startswith("data ")),
                            None)
                log = [line[4:] for line in lines if line.startswith("log ")]
                if rc == 0:
                    self._update(key, state="running" if running else "exited", data=data,
                                 log=log)
                return running

            # All ended (and none failed to answer): stopped.
            alive = in_parallel(names, one)
            crashed = any(isinstance(v, StepFailed) for v in alive.values())
            if names and not crashed and not any(alive.values()):
                with self._lock:
                    if self.phase == "running":
                        self.phase, self.stopped = "stopped", time.time()
                        self.note = "The program has ended on every platform."
                self._log("the program has ended on every platform")
                self._publish()
                self._stop_recording()
                return

    def stop(self, requester, note=None):
        """Stop the program on every platform it ran on.

        Thrusters and pucks go off at once, not after the watchdog.
        """
        with self._lock:
            names = [n for n, p in self.platforms.items()
                     if p.get("state") in ("running", "exited", "failed")]
            if self.phase not in ("starting", "running", "stopped") or not names:
                return self.status()
            self.phase = "stopping"
            model = self.model
        self._log("stopping %s on %s (by %s)" % (model, shown_list(names), requester))
        self._publish()

        def one(key):
            """Stop one platform's program; return if confirmed."""
            rc, out, err = platform_run(key, "exp-stop " + model, timeout=STOP_TIMEOUT)
            if rc == 0:
                self._update(key, state="stopped", pid=None, detail=decode(out).strip())
                hardware.get(key).send_off()
            else:
                self._update(key, state="failed", detail="could not stop: %s" % (err or rc))
            return rc == 0

        ok = in_parallel(names, one)
        self._mark_crashed(ok)

        # The experiment clock ends with the programs; then the
        # recording (in the background: closing its files takes a
        # moment).
        streamer.command("exp clear")
        with self._lock:
            self.sync = {"state": "idle"}
            self.phase, self.stopped = "stopped", time.time()
            self.note = (
                note if all(ok.values())
                else "Could not confirm the stop on %s: use the E-stop." % shown_list(
                    n for n, v in ok.items() if not v))
        self._publish()
        threading.Thread(target=self._stop_recording, daemon=True).start()
        return self.status()

    def _stop_recording(self):
        """Stop the cameras' recording, if one is going.

        Keeps a summary of it (size, frames, errors) for the pages; the
        files stay on the Orin until collected.
        """
        with self._lock:
            rec = self.recording
            if not rec or rec["state"] != "recording":
                return
            rec["state"] = "stopping"
        self._publish()
        result, error = None, None
        try:
            result = vision.record_stop(rec["session"])
            if result is None:
                error = "SPOT Vision wasn't recording it any more (restarted?)"
        except ApiError as e:
            error = str(e)
        with self._lock:
            rec["error"] = error
            rec["state"] = "stopped" if result else "failed"
            if result:
                rec["result"] = {
                    "bytes": result.get("bytes"), "note": result.get("note"),
                    "clock_samples": result.get("clock_samples"),
                    "timed": bool((result.get("clock") or {}).get("experiment")),
                    "cameras": {cid: {k: cam.get(k) for k in ("name", "frames", "error", "note")}
                                for cid, cam in (result.get("cameras") or {}).items()}}
        self._log("recording %s: %s" % (rec["session"], error or "stopped, %.1f GB"
                                        % ((result.get("bytes") or 0) / 1e9)))
        self._publish()

    # Data

    def fetch(self, requester):
        """Collect every platform's ExpLog files (EXP_DATA_ROOT).

        Also writes each platform's debug messages, as the console
        received them, and frees the platform's disk once copied.
        Collecting the same experiment again uses the same run (and
        folder), e.g. after copying its recording failed.

        Raises:
            ApiError: while an experiment is busy, or with none.
        """
        with self._lock:
            if self.phase in BUSY_PHASES:
                raise ApiError("Stop the experiment before collecting its data.")
            if not self.model or not self.platforms:
                raise ApiError("No experiment to collect data from.")
            model, names = self.model, list(self.platforms)

        # The cameras' recording, closed first (the local console takes
        # it from the Orin).
        deadline = time.time() + RECORD_WAIT
        while True:
            with self._lock:
                rec = dict(self.recording) if self.recording else None
            if not rec or rec["state"] not in ("recording", "stopping") or time.time() > deadline:
                break
            if rec["state"] == "recording":
                self._stop_recording()
            time.sleep(0.5)
        with self._lock:
            run = self.run = self.run or "%s_%s" % (model, time.strftime("%Y%m%d_%H%M%S"))
        folder = EXP_DATA_ROOT / run

        def one(key):
            """Collect one platform's files: {"files", "error"}."""
            target = folder / key

            # Its debug messages (written even without data).
            with self._lock:
                messages = list(self.debug.get(key, []))
            if messages:
                target.mkdir(parents=True, exist_ok=True)
                (target / "debug_messages.txt").write_text(
                    "experiment time [s]\ttimes received\tmessage\n" + "".join(
                        "%s\t%d\t%s\n" % ("" if m["t"] is None else "%.3f" % m["t"],
                                          m["count"], m["text"])
                        for m in messages),
                    encoding="utf-8")

            # Its data, as a tar of ExpLog files.
            rc, out, err = platform_run(key, "exp-data " + model, timeout=DATA_TIMEOUT)
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

            # Copied off: free the platform's disk.
            if files:
                rc, _, _ = platform_run(key, "exp-clean " + model, timeout=CLEAN_TIMEOUT)
                if rc == 0:
                    self._update(key, data=0)
            return {"files": files, "error": None}

        result = in_parallel(names, one)
        result = {n: ({"files": [], "error": r.error} if isinstance(r, StepFailed) else r)
                  for n, r in result.items()}
        recorded = None
        if rec and rec["state"] == "stopped":
            recorded = {k: rec[k] for k in ("session", "cameras", "url", "result")}
        with self._lock:
            self.last_run = {"run": run, "files": result, "at": time.time(),
                             "recording": recorded}
        self._log("collected data into %s (by %s)" % (folder, requester))
        self._publish()
        return self.status()

    def archive(self, run):
        """Return a run's data as a .tar.gz (for the local console).

        Raises:
            ApiError: if there is no such run.
        """
        if not NAME_RE.match(run or "") or not (EXP_DATA_ROOT / run).is_dir():
            raise ApiError("No data for %r." % run)
        buf = io.BytesIO()
        with tarfile.open(fileobj=buf, mode="w:gz") as tar:
            tar.add(str(EXP_DATA_ROOT / run), arcname=run)
        return buf.getvalue()

    # The platforms' experiment clocks (UDP from the diagrams)

    def listen_clocks(self):
        """Listen for each platform's experiment time, one port each."""
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
        """Record a platform's clock packets (a double) as they come."""
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
            if time.time() - last_publish > CLOCK_PUBLISH:
                last_publish = time.time()
                self._publish()

    # The platforms' debug messages (UDP from the diagrams)

    def listen_debug(self):
        """Listen for debug messages: each port, and the old one."""
        ports = ([(name, port) for name, port in EXP_DEBUG_PORTS.items()]
                 + [(None, EXP_DEBUG_LEGACY_PORT)])
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
        """Record debug messages from one port as they come.

        Args:
            name: The platform the port belongs to; None for the
                original diagram's shared port.
            sock: The bound socket.
        """
        while True:
            try:
                data, addr = sock.recvfrom(4096)
            except OSError:
                return
            if name is None:
                # The original diagram: the bare text; the platform from
                # its address.
                hosts = {p.get("host"): n for n, p in (settings.get("platforms") or {}).items()}
                plat, t, raw = hosts.get(addr[0]), None, data
            else:
                # [experiment time, double][text].
                if len(data) < 8:
                    continue
                plat, raw = name, data[8:]
                t = struct.unpack("<d", data[:8])[0]
                t = round(t, 3) if math.isfinite(t) else None
            if plat not in self.debug:
                continue
            self.add_debug(plat, t, raw.split(b"\0")[0].decode("ascii", "replace").strip())

    def add_debug(self, name, t, text):
        """Record a message: a change adds an entry, a repeat counts."""
        now = time.time()
        with self._lock:
            entries = self.debug[name]
            self.debug_seen[name] = now

            # A repeat.
            if entries and entries[-1]["text"] == text:
                entries[-1]["count"] += 1
                entries[-1]["last"] = now
                return

            # Nothing to show yet.
            if not text and not entries:
                return
            entries.append({"t": t, "text": text, "first": now, "last": now, "count": 1})
            del entries[:-DEBUG_KEEP]
        self._publish()

    # The Live Stream blocks' signals, for the Live Plot

    def listen_live(self):
        """Listen for live streams (a port per platform); publish."""
        for name, port in EXP_LIVE_PORTS.items():
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, RECEIVE_BUFFER)
            try:
                sock.bind(("", port))
            except OSError as e:
                print("Not listening for live streams on UDP %d: %s" % (port, e))
                continue
            threading.Thread(target=self._live_loop, args=(name, sock), daemon=True).start()
        threading.Thread(target=self._live_publisher, daemon=True).start()

    def _live_loop(self, platform, sock):
        """Record a platform's live stream packets as they come."""
        while True:
            try:
                data = sock.recv(65535)
            except OSError:
                return
            row = parse_live(data)
            if row:
                self.add_live(platform, *row)

    def add_live(self, platform, name, t, values):
        """Record one sample of a stream.

        A new stream, or one whose size changed, starts again.
        """
        key = "%s/%s" % (platform, name)
        now = time.time()
        with self._lock:
            stream = self.live.get(key)
            if stream is None or stream["n"] != len(values):
                stream = self.live[key] = {"platform": platform, "name": name, "n": len(values),
                                           "rows": collections.deque(maxlen=LIVE_KEEP),
                                           "last": now}
                self.live_new[key] = []
            row = [round(t, 4)] + values
            stream["rows"].append(row)
            stream["last"] = now
            self.live_new.setdefault(key, []).append(row)

    def _live_publisher(self):
        """Send new samples to the pages a few times a second.

        As event live_data, not one event per packet; a new stream also
        refreshes the pages' lists.
        """
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
            if streams != known:
                known = streams
                self._publish()

    def live_data(self, keep=3000):
        """Return everything kept so far, for a page opened later.

        At most keep samples per stream, evenly spaced, plus the last.
        """
        with self._lock:
            out = {}
            for k, stream in self.live.items():
                rows = list(stream["rows"])
                if len(rows) > keep:
                    step = len(rows) / float(keep)
                    rows = [rows[int(i * step)] for i in range(keep)] + [rows[-1]]
                out[k] = {"platform": stream["platform"], "name": stream["name"],
                          "n": stream["n"], "rows": rows}
            return out


def parse_live(data):
    """Decode a Live Stream packet.

    Returns:
        (name, t, [values]), non-finite values as None; None if it
        isn't a valid packet.
    """
    # Version 1, and the length the header says.
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
