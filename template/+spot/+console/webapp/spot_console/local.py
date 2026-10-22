"""The local console (started by SPOT in MATLAB).

The bridge to the user's MATLAB worker, the simulation and build
runners, and the link to the lab console (TX2).

Part of the SPOT Console (webapp/app.py starts it).
"""

import hmac
import io
import json
import os
import re
import socket
import tarfile
import threading
import time
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path

from .common import ApiError
from .events import DEFAULT_PLATFORMS, hub
from .experiment import NAME_RE


LOCAL = {"enabled": False, "token": "", "root": None, "ready_file": None, "server": None}

# Commands the page may send, and how long each may take (seconds;
# None = no limit).
MATLAB_COMMANDS = {
    "ping": 10, "info": 10, "settings.get": 120, "settings.set": 120, "settings.reset": 120,
    "settings.open": 900, "settings.saveAs": 900, "diagram.open": 120, "diagram.showBlock": 120,
    "workspace.list": 60, "workspace.inspect": 60, "workspace.eval": None,
    "experiment.plan": 120, "experiment.connect": 600, "experiment.decode": 900,
    # Results page: Saved Data, loaded datasets, plots and 3D playback
    # (data.load may open a dialog).
    "data.list": 120, "data.load": 900, "data.unload": 30, "data.series": 300,
    "data.trajectory": 300, "data.saveSimulation": 120, "data.figure": 900,
    # Documentation tab: the Code Analyzer on code being edited, and
    # MATLAB noticing a saved file.
    "code.lint": 30, "code.rehash": 30,
}

HELLO_TIMEOUT = 10         # [s] for the worker's hello
LOG_TAIL = 300             # build log lines kept for a page opened later
LOG_POLL = 0.5             # [s] between reads of the build log
LAB_EVENTS_TIMEOUT = 40    # [s] reading the lab console's event stream
LAB_STATE_TIMEOUT = 4      # [s] for its snapshot
LAB_RETRY = 3              # [s] between attempts to reach it
FRAME_RATE = 30            # [Hz] lab frames passed on to the page, at most


def check_token(value):
    """Return whether value is the session's token (constant time)."""
    return bool(LOCAL["token"]) and hmac.compare_digest(str(value or ""), LOCAL["token"])


# The MATLAB worker
#
# In local mode (app.py --local, started by SPOT.m in MATLAB) the
# Simulation tab runs its commands in the user's MATLAB session: the
# MATLAB worker (+spot/+worker/Worker.m) connects to 127.0.0.1:<worker
# port>, proves it was started with this server's token, and then
# answers one JSON command per line. MATLAB runs one command at a time,
# so requests are sent one at a time.


class MatlabBridge:
    """The connection to the MATLAB worker, and its commands."""

    def __init__(self):
        """Start without a connection."""
        self._lock = threading.Lock()        # one command at a time
        self._state = threading.Lock()
        self._conn = None
        self._file = None
        self._pending = {}                   # id -> [event, reply]
        self._next_id = 1
        self.info = None
        self.connected_at = None
        self.busy = None                     # {"cmd", "since"} while MATLAB is working
        self.port = None

    def status(self):
        """Return whether MATLAB is connected and what it is doing."""
        with self._state:
            return {"connected": self._conn is not None, "info": self.info,
                    "since": self.connected_at,
                    "busy": dict(self.busy) if self.busy else None}

    def _publish(self):
        """Send the status to the pages."""
        hub.publish("matlab", self.status())

    def listen(self, port):
        """Listen for the worker on 127.0.0.1 (this computer only).

        Args:
            port: The port; 0 to pick a free one.

        Returns:
            The port listened on.
        """
        srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind(("127.0.0.1", port))
        srv.listen(2)
        self.port = srv.getsockname()[1]
        threading.Thread(target=self._accept_loop, args=(srv,), daemon=True).start()
        return self.port

    def _accept_loop(self, srv):
        """Serve each connection in a thread of its own, forever."""
        while True:
            conn, _ = srv.accept()
            threading.Thread(target=self._serve, args=(conn,), daemon=True).start()

    def _serve(self, conn):
        """Serve one worker connection until it closes."""
        reader = conn.makefile("rb")

        # The hello, with this session's token.
        try:
            conn.settimeout(HELLO_TIMEOUT)
            hello = json.loads(reader.readline().decode("utf-8") or "{}")
            if hello.get("hello") != "spot-worker" or not check_token(hello.get("token")):
                conn.close()
                return
            conn.settimeout(None)
        except (OSError, ValueError):
            conn.close()
            return

        # A new MATLAB session replaces the old one.
        with self._state:
            old = self._conn
            self._conn, self._file, self.info, self.connected_at = (conn, reader,
                                                                     hello.get("info"),
                                                                     time.time())
            self.busy = None
        if old is not None:
            try:
                old.close()
            except OSError:
                pass
        hub.log("matlab", "MATLAB %s connected (%s)" % ((self.info or {}).get("release", "?"),
                                                         (self.info or {}).get("root", "")))
        self._publish()

        # Each line: an event MATLAB pushes, or a reply to a command.
        try:
            for raw in reader:
                try:
                    text = raw.decode("utf-8")
                except UnicodeDecodeError:
                    text = raw.decode("latin-1")   # an older worker writing other encodings
                try:
                    msg = json.loads(text)
                except ValueError:
                    hub.log("matlab", "Unreadable message from MATLAB: %s" % text[:200])
                    continue
                if "event" in msg:   # e.g. simulation progress
                    hub.publish("matlab_event", msg)
                    continue
                with self._state:
                    waiter = self._pending.get(msg.get("id"))
                if waiter:
                    waiter[1] = msg
                    waiter[0].set()
        except OSError:
            pass

        # Closed: fail the commands still waiting (unless replaced).
        with self._state:
            if self._conn is not conn:
                return
            self._conn, self._file = None, None
            for waiter in self._pending.values():
                waiter[1] = {"ok": False, "error": "MATLAB disconnected."}
                waiter[0].set()
            self.busy = None
        hub.log("matlab", "MATLAB disconnected")
        self._publish()

    def _long_running(self):
        """Return whether MATLAB is busy with a long command."""
        busy = self.busy
        return bool(busy) and busy.get("cmd") in ("sim.run", "experiment.build",
                                                  "experiment.decode", "workspace.eval")

    def request(self, cmd, args=None, timeout=60, quiet=False):
        """Run a command in MATLAB and return its result.

        Args:
            cmd: The command (see MATLAB_COMMANDS).
            args: Its arguments, a dict.
            timeout: Seconds to wait for the answer; None for no limit.
            quiet: Don't tell the pages MATLAB is busy (a quick command
                the user didn't ask for, such as checking code).

        Raises:
            ApiError: if MATLAB is busy, not connected, doesn't answer,
                or reports an error (with its details).
        """
        # A quick command (settings, a plot) finishes within seconds:
        # wait for it; a simulation doesn't.
        if not self._lock.acquire(timeout=0.5 if self._long_running() else 6):
            busy = self.busy or {}
            raise ApiError("MATLAB is busy (%s for %d s). Try again when it has finished."
                           % (busy.get("label", busy.get("cmd", "working")),
                              time.time() - busy.get("since", time.time())))
        try:
            # Register the command, then send it.
            with self._state:
                conn = self._conn
                if conn is None:
                    raise ApiError("MATLAB is not connected. Run SPOT in MATLAB (in this "
                                   "project's folder).")
                rid = self._next_id
                self._next_id += 1
                waiter = [threading.Event(), None]
                self._pending[rid] = waiter
                self.busy = {"cmd": cmd, "since": time.time(),
                             "label": BUSY_LABELS.get(cmd, cmd)}
            if not quiet:
                self._publish()
            try:
                conn.sendall((json.dumps({"id": rid, "cmd": cmd, "args": args or {}})
                              + "\n").encode("utf-8"))
                if not waiter[0].wait(timeout):
                    raise ApiError("MATLAB did not answer within %d s (%s)." % (timeout, cmd))
            except OSError:
                raise ApiError("Lost the connection to MATLAB.")
            finally:
                with self._state:
                    self._pending.pop(rid, None)
                    self.busy = None
                if not quiet:
                    self._publish()

            # Its reply.
            reply = waiter[1] or {}
            if not reply.get("ok"):
                raise ApiError(reply.get("error") or "MATLAB reported an error.",
                               reply.get("details"))
            return reply.get("result")
        finally:
            self._lock.release()

    def interrupt(self, cmd, timeout=15):
        """Send a command while another runs (sim.stop during sim.run).

        MATLAB handles it when the running command lets callbacks
        through, as Simulink does while simulating.

        Raises:
            ApiError: if MATLAB isn't connected, doesn't respond, or
                reports an error.
        """
        with self._state:
            conn = self._conn
            if conn is None:
                raise ApiError("MATLAB is not connected.")
            rid = self._next_id
            self._next_id += 1
            waiter = [threading.Event(), None]
            self._pending[rid] = waiter
        try:
            conn.sendall((json.dumps({"id": rid, "cmd": cmd, "args": {}}) + "\n").encode("utf-8"))
            if not waiter[0].wait(timeout):
                raise ApiError("MATLAB did not respond to %s." % cmd)
        except OSError:
            raise ApiError("Lost the connection to MATLAB.")
        finally:
            with self._state:
                self._pending.pop(rid, None)
        reply = waiter[1] or {}
        if not reply.get("ok"):
            raise ApiError(reply.get("error") or "MATLAB reported an error.")
        return reply.get("result")


# What the page says MATLAB is doing, by command.
BUSY_LABELS = {
    "sim.run": "running the simulation",
    "experiment.build": "building the experiment",
    "experiment.connect": "connecting to the TX2",
    "experiment.decode": "converting the data",
    "workspace.eval": "running a command",
    "settings.open": "waiting for the file dialog",
    "settings.saveAs": "waiting for the file dialog",
    "diagram.open": "opening the diagram",
    "diagram.showBlock": "opening the diagram",
    "data.load": "loading results",
    "data.series": "reading signals",
    "data.trajectory": "preparing the playback",
    "data.figure": "drawing the figure (or waiting for its file dialog)",
    "data.saveSimulation": "saving the simulation",
    "code.lint": "checking code",
    "code.rehash": "reading saved code",
}


matlab = MatlabBridge()


# Long MATLAB jobs


class SimRunner:
    """Runs a long MATLAB command in the background.

    A simulation, or an experiment build; it reports through events
    named after it ("sim", "build").
    """

    def __init__(self, event="sim", cmd="sim.run", what="Simulation", log_name=None):
        """Set up a runner.

        Args:
            event: The event name for its status.
            cmd: The MATLAB command.
            what: Its name in the log.
            log_name: A file in .spot_cache that MATLAB writes while
                working (shown live), or None.
        """
        self.event, self.cmd, self.what = event, cmd, what
        self.log_name = log_name
        self._lock = threading.Lock()
        self.state = {"running": False, "started": None, "finished": None, "ok": None,
                      "error": None, "error_details": None, "summary": None, "result_id": 0}

    def status(self):
        """Return a copy of the job's state."""
        with self._lock:
            return dict(self.state)

    def start(self, requester):
        """Start the job in the background.

        Raises:
            ApiError: if it is already running.
        """
        with self._lock:
            if self.state["running"]:
                raise ApiError("A simulation is already running.")
            self.state.update(running=True, started=time.time(), finished=None, ok=None,
                              error=None, error_details=None, log_tail=[])
        hub.log("matlab", "%s started (by %s)" % (self.what, requester))
        hub.publish(self.event, self.status())
        threading.Thread(target=self._run, daemon=True).start()
        if self.log_name:
            threading.Thread(target=self._follow_log, daemon=True).start()
        return self.status()

    def _follow_log(self):
        """Send what MATLAB writes to the log file while the job runs.

        MATLAB can't answer meanwhile, so this is how the page sees
        progress. An unfinished line waits for the rest.
        """
        path = Path(LOCAL["root"]) / ".spot_cache" / self.log_name
        started = self.status()["started"]
        pos, partial = 0, ""
        while True:
            running = self.status()["running"]
            try:
                # New text in the file (written since this job started).
                if path.is_file() and path.stat().st_mtime >= started - 1:
                    with open(str(path), "r", encoding="utf-8", errors="replace") as f:
                        f.seek(pos)
                        chunk = f.read()
                        pos = f.tell()

                    # Whole lines, without hyperlinks and backspaces.
                    if chunk:
                        text = re.sub(r"<a [^>]*>|</a>|[\x08]", "", partial + chunk)
                        lines = text.split("\n")
                        partial = lines.pop()
                        lines = [line.rstrip() for line in lines if line.strip()]
                        if lines:
                            with self._lock:
                                self.state["log_tail"] = (
                                    (self.state.get("log_tail") or []) + lines)[-LOG_TAIL:]
                            hub.publish(self.event + "_log", {"lines": lines})
            except OSError:
                pass
            if not running:
                return
            time.sleep(LOG_POLL)

    def _run(self):
        """Run the command in MATLAB and record how it went."""
        ok, error, details, summary = False, None, None, None
        try:
            summary = matlab.request(self.cmd, {}, timeout=None)
            ok = True
        except ApiError as e:
            error, details = str(e), e.details
        with self._lock:
            self.state.update(running=False, finished=time.time(), ok=ok, error=error,
                              error_details=details, summary=summary)
            if ok:
                self.state["result_id"] += 1
        hub.log("matlab", "%s %s" % (self.what, "finished in %.1f s" % summary.get("seconds", 0)
                                     if ok else "failed: %s" % error))
        hub.publish(self.event, self.status())

    def result_path(self):
        """Return MATLAB's result file if in .spot_cache, else None."""
        summary = self.status().get("summary") or {}
        path = Path(summary.get("file") or "")
        cache = (Path(LOCAL["root"]) / ".spot_cache").resolve() if LOCAL["root"] else None
        try:
            path = path.resolve()
        except OSError:
            return None
        if not cache or cache not in path.parents or not path.is_file():
            return None
        return path


sim_runner = SimRunner()

build_runner = SimRunner("build", "experiment.build", "Experiment build", log_name="build.log")


# The lab console


class LabLink:
    """The local console's view of the lab console (TX2).

    Follows its event stream and passes PhaseSpace frames, hardware,
    stream and experiment status on to the page (as lab_* events), and
    forwards the lab actions the Experiment tab may take. Unreachable
    (not on the lab network): those actions are refused.
    """

    RELAY = {"frame": "lab_frame", "monitor": "lab_monitor", "stream_status": "lab_stream",
             "hardware_platform": "lab_hardware", "experiment": "lab_experiment",
             "live_data": "lab_live_data", "design": "lab_design",
             "calibration": "lab_calibration"}
    ACTIONS = ("stream/start", "stream/stop", "tracker_positions", "experiment/deploy",
               "experiment/start", "experiment/stop", "experiment/fetch", "hardware/off", "design")
    # GET requests the page may make through the local server.
    READS = ("experiment/build", "experiment/live", "calibration", "vision")

    PORT = 8080   # the lab console's web port

    def __init__(self, url):
        """Start following nothing yet (see start)."""
        self.url = url.rstrip("/")
        self._lock = threading.Lock()
        self._response = None   # the event stream being read (closed to switch address)
        self.reachable = False
        self.experiments = False   # whether its version has the experiment API
        self.error = "Connecting…"
        self.since = None
        self.snapshot = {}      # stream, monitor, hardware, experiment as of the last event
        self._last_frame = 0.0

    def status(self):
        """Return whether it is reachable, why not, its last state."""
        with self._lock:
            return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
                    "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}

    def _set(self, reachable, error=None):
        """Record reachability; tell the pages if it changed."""
        with self._lock:
            changed = reachable != self.reachable or error != self.error
            self.reachable, self.error = reachable, error
            if changed:
                self.since = time.time()
        if changed:
            hub.publish("lab", self.status())

    def start(self):
        """Follow the lab console in the background."""
        threading.Thread(target=self._run, daemon=True).start()

    @staticmethod
    def url_for(address):
        """Return the lab console's URL for an IPv4 address.

        Args:
            address: The settings' labConsoleAddress.

        Raises:
            ApiError: if it isn't an IPv4 address.
        """
        parts = str(address).strip().split(".")
        if len(parts) != 4 or not all(part.isdigit() and int(part) <= 255 for part in parts):
            raise ApiError("The lab console address must be an IPv4 address such as "
                           "192.168.1.116.")
        return "http://%s:%d" % (".".join(str(int(part)) for part in parts), LabLink.PORT)

    def set_address(self, address):
        """Follow the lab console at another address."""
        url = self.url_for(address)
        with self._lock:
            if url == self.url:
                return self.status_unlocked()
            self.url = url
            self.reachable, self.experiments, self.snapshot = False, False, {}
            self.error, self.since = "Connecting…", time.time()
            response, self._response = self._response, None

        # Closing the stream makes the reading loop reconnect, to the
        # new address.
        if response is not None:
            try:
                response.close()
            except Exception:
                pass
        hub.publish("lab", self.status())
        return self.status()

    def status_unlocked(self):
        """Return status() without taking the lock (caller holds it)."""
        return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
                "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}

    def _run(self):
        """Read the lab console's event stream forever, reconnecting."""
        import urllib.request
        while True:
            url = self.url
            try:
                # The event stream first, then the snapshot: the lab
                # only says when something changes (a platform's status
                # check, say), so a change between a snapshot and a
                # later subscription would never arrive, and the page
                # would show the old state until the next change.
                # Events sent meanwhile wait in the stream and are
                # applied over the snapshot.
                with urllib.request.urlopen(url + "/api/events",
                                            timeout=LAB_EVENTS_TIMEOUT) as events:
                    with urllib.request.urlopen(url + "/api/state",
                                                timeout=LAB_STATE_TIMEOUT) as snap_resp:
                        state = json.loads(snap_resp.read().decode("utf-8"))

                    # It must be a lab console, not another local one.
                    mode = (state.get("features") or {}).get("mode")
                    if mode == "local" or (mode is None
                                           and not ("hardware" in state and "stream" in state)):
                        raise OSError("%s is not the lab console" % url)

                    with self._lock:
                        if url != self.url:   # the address changed meanwhile
                            continue
                        # Older lab consoles (before the experiment API)
                        # still stream and drive the hardware.
                        self.experiments = "experiment" in state
                        self.snapshot = {"stream": state.get("stream"),
                                         "monitor": state.get("monitor"),
                                         "hardware": state.get("hardware"),
                                         "experiment": state.get("experiment"),
                                         "led_owner": state.get("led_owner"),
                                         "design": state.get("design"),
                                         "calibration": state.get("calibration")}
                        self._response = events
                    self._set(True)

                    # Pass the relayed events on.
                    event = None
                    for raw in events:
                        line = raw.decode("utf-8", "replace").rstrip("\r\n")
                        if line.startswith("event: "):
                            event = line[7:]
                        elif line.startswith("data: ") and event in self.RELAY:
                            self._relay(event, line[6:])
            except (OSError, ValueError, AttributeError) as e:
                # Closed to follow a new address: try it at once.
                if url != self.url:
                    continue
                reason = getattr(e, "reason", None) or e
                self._set(False, "Not reachable (%s)" % reason)
            if url != self.url:
                continue
            time.sleep(LAB_RETRY)

    def _relay(self, event, data):
        """Update the snapshot with an event; pass it to the page."""
        # Frames: at most FRAME_RATE to the page.
        if event == "frame":
            now = time.time()
            if now - self._last_frame < 1 / FRAME_RATE:
                return
            self._last_frame = now
        try:
            payload = json.loads(data)
        except ValueError:
            return

        # The snapshot's part for this event; a platform's status
        # replaces that platform's entry.
        key = {"monitor": "monitor", "stream_status": "stream", "experiment": "experiment",
               "design": "design", "calibration": "calibration"}.get(event)
        with self._lock:
            if key:
                self.snapshot[key] = payload
            elif event == "hardware_platform" and self.snapshot.get("hardware"):
                plats = self.snapshot["hardware"].get("platforms", [])
                self.snapshot["hardware"]["platforms"] = [
                    payload if plat.get("name") == payload.get("name") else plat
                    for plat in plats]
        hub.publish(self.RELAY[event], payload)

    def request(self, method, path, body=None, timeout=60, raw=False):
        """Call the lab console's API.

        Args:
            method: "GET" or "POST".
            path: The path after /api/.
            body: A dict to POST as JSON.
            timeout: Seconds before giving up.
            raw: Return the bytes instead of decoded JSON.

        Raises:
            ApiError: if it isn't reachable, too old for experiments,
                or answers with an error.
        """
        import urllib.request
        import urllib.error
        state = self.status()
        if not state["reachable"]:
            raise ApiError("The lab console (%s) isn't reachable: experiments and calibrations "
                           "need the lab network (SRCL Wi-Fi or wired)." % self.url)
        if path.startswith("experiment/") and not state["experiments"]:
            raise ApiError("The lab console (%s) runs an older version without experiments: "
                           "update it with +spot/+console/deploy/deploy_tx2.sh." % self.url)

        # The request, saying which computer asks.
        headers = {"X-SPOT-Requester": "%s, local console" % socket.gethostname()}
        data = None
        if method == "POST":
            headers["Content-Type"] = "application/json"
            data = json.dumps(body or {}).encode("utf-8")
        req = urllib.request.Request(self.url + "/api/" + path, data=data, headers=headers,
                                     method=method)
        try:
            with urllib.request.urlopen(req, timeout=timeout) as response:
                out = response.read()
        except urllib.error.HTTPError as e:
            # The lab console's own message, if it sent one.
            try:
                msg = json.loads(e.read().decode("utf-8")).get("error")
            except ValueError:
                msg = None
            raise ApiError("Lab console: %s" % (msg or e))
        except OSError as e:
            raise ApiError("Lab console not reachable: %s" % (getattr(e, "reason", None) or e))
        return out if raw else json.loads(out.decode("utf-8"))


lab = None   # LabLink, local mode only


def download_run(run):
    """Copy a run's data from the lab console into the project.

    Into <project>/Saved Data/<run>/<PLATFORM>/ExpLog*.

    Returns:
        {"folder", "files"}.

    Raises:
        ApiError: if run isn't a valid run name.
    """
    if not NAME_RE.match(run or ""):
        raise ApiError("%r is not a run name." % run)
    data = lab.request("GET", "experiment/download?run=" + run, timeout=300, raw=True)
    target = Path(LOCAL["root"]) / "Saved Data"

    # Only run/PLATFORM/ExpLog... files.
    files = []
    with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as tar:
        for member in tar.getmembers():
            parts = Path(member.name).parts
            if (member.isfile() and len(parts) == 3 and parts[0] == run
                    and parts[1] in DEFAULT_PLATFORMS and NAME_RE.match(parts[2])):
                dest = target / run / parts[1] / parts[2]
                dest.parent.mkdir(parents=True, exist_ok=True)
                with tar.extractfile(member) as src:
                    dest.write_bytes(src.read())
                files.append(str(dest.relative_to(target)))
    return {"folder": str(target / run), "files": files}


# Camera recordings: from the Orin (SPOT Vision) straight into the run


CAMERA_CHUNK = 1 << 20        # [bytes] read and written at a time
CAMERA_TIMEOUT = 60           # [s] for each read from the Orin
CAMERA_PROGRESS = 0.5         # [s] between progress events
SESSION_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,119}$")
FILE_PART_RE = re.compile(r"^[A-Za-z0-9_.-]+$")


class CameraDownload:
    """Copies an experiment's camera recordings into its run's folder.

    Into <project>/Saved Data/<run>/cameras/, file by file from SPOT
    Vision on the Orin (never through the lab console), in the
    background, with progress as camera_download events. Each file's
    size is checked; once all are here, the Orin's copy is deleted, as
    Collect frees the platforms' disks.
    """

    def __init__(self):
        """Start with nothing copied."""
        self._lock = threading.Lock()
        self.state = {"state": "idle"}

    def status(self):
        """Return the copy's state (what the page shows)."""
        with self._lock:
            return dict(self.state)

    def _set(self, **fields):
        """Update the state and tell the page."""
        with self._lock:
            self.state.update(fields)
            state = dict(self.state)
        hub.publish("camera_download", state)

    def start(self, run, recording):
        """Start copying a recording into a run's folder.

        Args:
            run: The run (its Saved Data folder's name).
            recording: The lab console's last_run["recording"]:
                {"session", "url", ...}.

        Raises:
            ApiError: if one is copying, or the names aren't valid.
        """
        recording = recording or {}
        session, base = str(recording.get("session") or ""), str(recording.get("url") or "")
        if not NAME_RE.match(run or "") or not SESSION_RE.match(session):
            raise ApiError("Not a run and recording: %r, %r." % (run, session))
        if not re.match(r"^http://[0-9.]+:\d+$", base):
            raise ApiError("Not SPOT Vision's address: %r." % base)
        with self._lock:
            if self.state.get("state") == "copying":
                raise ApiError("The cameras of %s are still being copied." % self.state["run"])
            self.state = {"state": "copying", "run": run, "session": session, "done": 0,
                          "total": None, "file": None, "error": None, "started": time.time()}
        threading.Thread(target=self._copy, args=(run, session, base), daemon=True).start()
        return self.status()

    def _copy(self, run, session, base):
        """Copy every file; check them; free the Orin's disk."""
        folder = Path(LOCAL["root"]) / "Saved Data" / run / "cameras"
        try:
            # What there is.
            with urllib.request.urlopen("%s/api/record/%s" % (base, session),
                                        timeout=CAMERA_TIMEOUT) as response:
                listing = json.loads(response.read().decode("utf-8"))
            files = listing.get("files") or []
            total = sum(int(f["size"]) for f in files)
            self._set(total=total, files=len(files))

            # Each file, a chunk at a time.
            done, last = 0, 0.0
            for f in files:
                parts = str(f["path"]).split("/")
                if not all(FILE_PART_RE.match(part) and part not in (".", "..")
                           for part in parts):
                    raise ApiError("Unexpected file name from the Orin: %r" % f["path"])
                dest = folder.joinpath(*parts)
                dest.parent.mkdir(parents=True, exist_ok=True)
                url = "%s/recordings/%s/%s" % (base, session, urllib.parse.quote(f["path"]))
                got = 0
                response = urllib.request.urlopen(url, timeout=CAMERA_TIMEOUT)
                try:
                    with open(str(dest), "wb") as out:
                        while True:
                            chunk = response.read(CAMERA_CHUNK)
                            if not chunk:
                                break
                            out.write(chunk)
                            got += len(chunk)
                            if time.time() - last >= CAMERA_PROGRESS:
                                last = time.time()
                                self._set(done=done + got, file=f["path"])
                finally:
                    response.close()
                if got != int(f["size"]) or dest.stat().st_size != int(f["size"]):
                    raise ApiError("%s arrived incomplete (%d of %d bytes)"
                                   % (f["path"], got, int(f["size"])))
                done += got

            # All here: free the Orin's disk.
            freed = True
            req = urllib.request.Request("%s/api/record/delete" % base, method="POST",
                                         data=json.dumps({"session": session}).encode("utf-8"),
                                         headers={"Content-Type": "application/json"})
            try:
                urllib.request.urlopen(req, timeout=CAMERA_TIMEOUT).read()
            except (urllib.error.URLError, OSError):
                freed = False
            self._set(state="done", done=done, file=None, folder=str(folder), freed=freed,
                      finished=time.time())
        except (ApiError, urllib.error.URLError, OSError, ValueError, KeyError) as e:
            reason = getattr(e, "reason", None) or e
            self._set(state="failed", error="%s (the recording stays on the Orin)" % reason,
                      finished=time.time())


camera_download = CameraDownload()


def write_ready_file(web_port):
    """Tell SPOT.m (spot.console.start) the ports and token.

    Written to .spot_cache/console.json, readable by this user only,
    and replaced atomically.
    """
    path = Path(LOCAL["ready_file"])
    path.parent.mkdir(parents=True, exist_ok=True)
    data = {"web_port": web_port, "worker_port": matlab.port, "token": LOCAL["token"],
            "pid": os.getpid(), "root": LOCAL["root"]}
    tmp = path.with_suffix(".tmp")
    fd = os.open(str(tmp), os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o600)
    with os.fdopen(fd, "w") as fh:
        json.dump(data, fh)
    os.replace(str(tmp), str(path))
