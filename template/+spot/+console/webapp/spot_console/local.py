"""The local console (started by SPOT in MATLAB): the bridge to the user's MATLAB worker, the
simulation and build runners, and the link to the lab console (TX2).

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
from pathlib import Path

from .common import ApiError
from .events import DEFAULT_PLATFORMS, hub
from .experiment import NAME_RE


LOCAL = {"enabled": False, "token": "", "root": None, "ready_file": None, "server": None}

# Commands the page may send, and how long each may take (seconds; None = no limit)


MATLAB_COMMANDS = {
    "ping": 10, "info": 10, "settings.get": 120, "settings.set": 120, "settings.reset": 120,
    "settings.open": 900, "settings.saveAs": 900, "diagram.open": 120, "diagram.showBlock": 120,
    "massprops.writeTracker": 30,
    "workspace.list": 60, "workspace.inspect": 60, "workspace.eval": None,
    "experiment.plan": 120, "experiment.connect": 600, "experiment.decode": 900,
    # Results page: Saved Data, loaded datasets, plots and 3D playback (data.load may open a dialog)
    "data.list": 120, "data.load": 900, "data.unload": 30, "data.series": 300, "data.trajectory": 300,
    "data.saveSimulation": 120, "data.figure": 900,
}


def check_token(value):
    return bool(LOCAL["token"]) and hmac.compare_digest(str(value or ""), LOCAL["token"])


class MatlabBridge:
    def __init__(self):
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
        with self._state:
            return {"connected": self._conn is not None, "info": self.info, "since": self.connected_at,
                    "busy": dict(self.busy) if self.busy else None}

    def _publish(self):
        hub.publish("matlab", self.status())

    def listen(self, port):
        srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        srv.bind(("127.0.0.1", port))   # this computer only
        srv.listen(2)
        self.port = srv.getsockname()[1]
        threading.Thread(target=self._accept_loop, args=(srv,), daemon=True).start()
        return self.port

    def _accept_loop(self, srv):
        while True:
            conn, _ = srv.accept()
            threading.Thread(target=self._serve, args=(conn,), daemon=True).start()

    def _serve(self, conn):
        f = conn.makefile("rb")
        try:
            conn.settimeout(10)
            hello = json.loads(f.readline().decode("utf-8") or "{}")
            if hello.get("hello") != "spot-worker" or not check_token(hello.get("token")):
                conn.close()
                return
            conn.settimeout(None)
        except (OSError, ValueError):
            conn.close()
            return
        with self._state:
            old = self._conn
            self._conn, self._file, self.info, self.connected_at = conn, f, hello.get("info"), time.time()
            self.busy = None
        if old is not None:   # a new MATLAB session replaces the old one
            try:
                old.close()
            except OSError:
                pass
        hub.log("matlab", "MATLAB %s connected (%s)" % ((self.info or {}).get("release", "?"), (self.info or {}).get("root", "")))
        self._publish()
        try:
            for raw in f:
                try:
                    text = raw.decode("utf-8")
                except UnicodeDecodeError:
                    text = raw.decode("latin-1")   # an older worker writing other encodings
                try:
                    msg = json.loads(text)
                except ValueError:
                    hub.log("matlab", "Unreadable message from MATLAB: %s" % text[:200])
                    continue
                if "event" in msg:   # pushed by MATLAB, e.g. simulation progress
                    hub.publish("matlab_event", msg)
                    continue
                with self._state:
                    waiter = self._pending.get(msg.get("id"))
                if waiter:
                    waiter[1] = msg
                    waiter[0].set()
        except OSError:
            pass
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
        busy = self.busy
        return bool(busy) and busy.get("cmd") in ("sim.run", "experiment.build", "experiment.decode", "workspace.eval")

    def request(self, cmd, args=None, timeout=60):
        # A quick command (settings, a plot) finishes within seconds: wait for it; a simulation doesn't
        if not self._lock.acquire(timeout=0.5 if self._long_running() else 6):
            busy = self.busy or {}
            raise ApiError("MATLAB is busy (%s for %d s). Try again when it has finished."
                           % (busy.get("label", busy.get("cmd", "working")), time.time() - busy.get("since", time.time())))
        try:
            with self._state:
                conn = self._conn
                if conn is None:
                    raise ApiError("MATLAB is not connected. Run SPOT in MATLAB (in this project's folder).")
                rid = self._next_id
                self._next_id += 1
                waiter = [threading.Event(), None]
                self._pending[rid] = waiter
                self.busy = {"cmd": cmd, "since": time.time(), "label": BUSY_LABELS.get(cmd, cmd)}
            self._publish()
            try:
                conn.sendall((json.dumps({"id": rid, "cmd": cmd, "args": args or {}}) + "\n").encode("utf-8"))
                if not waiter[0].wait(timeout):
                    raise ApiError("MATLAB did not answer within %d s (%s)." % (timeout, cmd))
            except OSError:
                raise ApiError("Lost the connection to MATLAB.")
            finally:
                with self._state:
                    self._pending.pop(rid, None)
                    self.busy = None
                self._publish()
            reply = waiter[1] or {}
            if not reply.get("ok"):
                raise ApiError(reply.get("error") or "MATLAB reported an error.", reply.get("details"))
            return reply.get("result")
        finally:
            self._lock.release()


    def interrupt(self, cmd, timeout=15):
        """Send a command while another is running (e.g. sim.stop during sim.run). MATLAB handles
        it when the running command lets callbacks through, as Simulink does while simulating."""
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


BUSY_LABELS = {"sim.run": "running the simulation", "experiment.build": "building the experiment",
               "experiment.connect": "connecting to the TX2", "experiment.decode": "converting the data", "workspace.eval": "running a command",
               "settings.open": "waiting for the file dialog", "settings.saveAs": "waiting for the file dialog",
               "diagram.open": "opening the diagram", "diagram.showBlock": "opening the diagram", "data.load": "loading results", "data.series": "reading signals",
               "data.trajectory": "preparing the playback", "data.figure": "drawing the figure (or waiting for its file dialog)",
               "data.saveSimulation": "saving the simulation"}


matlab = MatlabBridge()


class SimRunner:
    """Runs a long MATLAB command in the background (a simulation, or an experiment build) and
    reports through events named after it ("sim", "build")."""

    def __init__(self, event="sim", cmd="sim.run", what="Simulation", log_name=None):
        self.event, self.cmd, self.what = event, cmd, what
        self.log_name = log_name   # a file in .spot_cache that MATLAB writes while working (shown live)
        self._lock = threading.Lock()
        self.state = {"running": False, "started": None, "finished": None, "ok": None, "error": None,
                      "error_details": None, "summary": None, "result_id": 0}

    def status(self):
        with self._lock:
            return dict(self.state)

    def start(self, requester):
        with self._lock:
            if self.state["running"]:
                raise ApiError("A simulation is already running.")
            self.state.update(running=True, started=time.time(), finished=None, ok=None, error=None,
                              error_details=None, log_tail=[])
        hub.log("matlab", "%s started (by %s)" % (self.what, requester))
        hub.publish(self.event, self.status())
        threading.Thread(target=self._run, daemon=True).start()
        if self.log_name:
            threading.Thread(target=self._follow_log, daemon=True).start()
        return self.status()

    def _follow_log(self):
        """Send what MATLAB writes to the log file while the job runs (it can't answer meanwhile)."""
        path = Path(LOCAL["root"]) / ".spot_cache" / self.log_name
        started = self.status()["started"]
        pos, partial = 0, ""
        while True:
            running = self.status()["running"]
            try:
                if path.is_file() and path.stat().st_mtime >= started - 1:
                    with open(str(path), "r", encoding="utf-8", errors="replace") as f:
                        f.seek(pos)
                        chunk = f.read()
                        pos = f.tell()
                    if chunk:
                        text = re.sub(r"<a [^>]*>|</a>|[\x08]", "", partial + chunk)
                        lines = text.split("\n")
                        partial = lines.pop()   # an unfinished line waits for the rest
                        lines = [l.rstrip() for l in lines if l.strip()]
                        if lines:
                            with self._lock:
                                self.state["log_tail"] = ((self.state.get("log_tail") or []) + lines)[-300:]
                            hub.publish(self.event + "_log", {"lines": lines})
            except OSError:
                pass
            if not running:
                return
            time.sleep(0.5)

    def _run(self):
        ok, error, details, summary = False, None, None, None
        try:
            summary = matlab.request(self.cmd, {}, timeout=None)
            ok = True
        except ApiError as e:
            error, details = str(e), e.details
        with self._lock:
            self.state.update(running=False, finished=time.time(), ok=ok, error=error, error_details=details,
                              summary=summary)
            if ok:
                self.state["result_id"] += 1
        hub.log("matlab", "%s %s" % (self.what, "finished in %.1f s" % summary.get("seconds", 0) if ok else "failed: %s" % error))
        hub.publish(self.event, self.status())

    def result_path(self):
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


class LabLink:
    """The local console's view of the lab console (TX2): follows its event stream and passes
    PhaseSpace frames, hardware, stream and experiment status on to the page (as lab_* events), and
    forwards the lab actions the Experiment tab may take. Unreachable (not on the lab network):
    those actions are refused."""

    RELAY = {"frame": "lab_frame", "monitor": "lab_monitor", "stream_status": "lab_stream",
             "hardware_platform": "lab_hardware", "experiment": "lab_experiment", "live_data": "lab_live_data",
             "design": "lab_design", "calibration": "lab_calibration"}
    ACTIONS = ("stream/start", "stream/stop", "tracker_positions", "experiment/deploy", "experiment/start",
               "experiment/stop", "experiment/fetch", "hardware/off", "design")
    READS = ("experiment/build", "experiment/live", "calibration")   # GET requests the page may make through the local server

    PORT = 8080   # the lab console's web port

    def __init__(self, url):
        self.url = url.rstrip("/")
        self._lock = threading.Lock()
        self._response = None   # the event stream being read (closed to switch to a new address)
        self.reachable = False
        self.experiments = False   # whether its version has the experiment API
        self.error = "Connecting…"
        self.since = None
        self.snapshot = {}      # stream, monitor, hardware, experiment as of the last event
        self._last_frame = 0.0

    def status(self):
        with self._lock:
            return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
                    "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}

    def _set(self, reachable, error=None):
        with self._lock:
            changed = reachable != self.reachable or error != self.error
            self.reachable, self.error = reachable, error
            if changed:
                self.since = time.time()
        if changed:
            hub.publish("lab", self.status())

    def start(self):
        threading.Thread(target=self._run, daemon=True).start()

    @staticmethod
    def url_for(address):
        """The lab console's URL for an IPv4 address (the settings' labConsoleAddress)."""
        parts = str(address).strip().split(".")
        if len(parts) != 4 or not all(p.isdigit() and int(p) <= 255 for p in parts):
            raise ApiError("The lab console address must be an IPv4 address such as 192.168.1.116.")
        return "http://%s:%d" % (".".join(str(int(p)) for p in parts), LabLink.PORT)

    def set_address(self, address):
        """Follow the lab console at another address (from the Experiment tab)."""
        url = self.url_for(address)
        with self._lock:
            if url == self.url:
                return self.status_unlocked()
            self.url = url
            self.reachable, self.experiments, self.snapshot = False, False, {}
            self.error, self.since = "Connecting…", time.time()
            response, self._response = self._response, None
        if response is not None:
            try:
                response.close()   # the reading loop then reconnects, to the new address
            except Exception:
                pass
        hub.publish("lab", self.status())
        return self.status()

    def status_unlocked(self):
        return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
                "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}

    def _run(self):
        import urllib.request
        while True:
            url = self.url
            try:
                with urllib.request.urlopen(url + "/api/state", timeout=4) as r:
                    st = json.loads(r.read().decode("utf-8"))
                mode = (st.get("features") or {}).get("mode")
                if mode == "local" or (mode is None and not ("hardware" in st and "stream" in st)):
                    raise OSError("%s is not the lab console" % url)
                with self._lock:
                    if url != self.url:   # the address changed meanwhile
                        continue
                    # Older lab consoles (before the experiment API) still stream and drive the hardware
                    self.experiments = "experiment" in st
                    self.snapshot = {"stream": st.get("stream"), "monitor": st.get("monitor"),
                                     "hardware": st.get("hardware"), "experiment": st.get("experiment"),
                                     "led_owner": st.get("led_owner"), "design": st.get("design"),
                                     "calibration": st.get("calibration")}
                self._set(True)
                with urllib.request.urlopen(url + "/api/events", timeout=40) as r:
                    with self._lock:
                        self._response = r
                    event = None
                    for raw in r:
                        line = raw.decode("utf-8", "replace").rstrip("\r\n")
                        if line.startswith("event: "):
                            event = line[7:]
                        elif line.startswith("data: ") and event in self.RELAY:
                            self._relay(event, line[6:])
            except (OSError, ValueError, AttributeError) as e:
                if url != self.url:   # closed to follow a new address: try it at once
                    continue
                reason = getattr(e, "reason", None) or e
                self._set(False, "Not reachable (%s)" % reason)
            if url != self.url:
                continue
            time.sleep(3)

    def _relay(self, event, data):
        if event == "frame":   # at most 30 Hz to the page
            now = time.time()
            if now - self._last_frame < 1 / 30:
                return
            self._last_frame = now
        try:
            payload = json.loads(data)
        except ValueError:
            return
        key = {"monitor": "monitor", "stream_status": "stream", "experiment": "experiment", "design": "design",
               "calibration": "calibration"}.get(event)
        with self._lock:
            if key:
                self.snapshot[key] = payload
            elif event == "hardware_platform" and self.snapshot.get("hardware"):
                plats = self.snapshot["hardware"].get("platforms", [])
                self.snapshot["hardware"]["platforms"] = [payload if p.get("name") == payload.get("name") else p for p in plats]
        hub.publish(self.RELAY[event], payload)

    def request(self, method, path, body=None, timeout=60, raw=False):
        import urllib.request
        import urllib.error
        st = self.status()
        if not st["reachable"]:
            raise ApiError("The lab console (%s) isn't reachable: experiments and calibrations need the "
                           "lab network (SRCL Wi-Fi or wired)." % self.url)
        if path.startswith("experiment/") and not st["experiments"]:
            raise ApiError("The lab console (%s) runs an older version without experiments: update it "
                           "with +spot/+console/deploy/deploy_tx2.sh." % self.url)
        headers = {"X-SPOT-Requester": "%s, local console" % socket.gethostname()}
        data = None
        if method == "POST":
            headers["Content-Type"] = "application/json"
            data = json.dumps(body or {}).encode("utf-8")
        req = urllib.request.Request(self.url + "/api/" + path, data=data, headers=headers, method=method)
        try:
            with urllib.request.urlopen(req, timeout=timeout) as r:
                out = r.read()
        except urllib.error.HTTPError as e:
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
    """Copy a run's data from the lab console into <project>/Saved Data/<run>/<PLATFORM>/ExpLog*."""
    if not NAME_RE.match(run or ""):
        raise ApiError("%r is not a run name." % run)
    data = lab.request("GET", "experiment/download?run=" + run, timeout=300, raw=True)
    target = Path(LOCAL["root"]) / "Saved Data"
    files = []
    with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as tar:
        for member in tar.getmembers():
            parts = Path(member.name).parts   # run/PLATFORM/ExpLog...
            if (member.isfile() and len(parts) == 3 and parts[0] == run and parts[1] in DEFAULT_PLATFORMS
                    and NAME_RE.match(parts[2])):
                dest = target / run / parts[1] / parts[2]
                dest.parent.mkdir(parents=True, exist_ok=True)
                with tar.extractfile(member) as src:
                    dest.write_bytes(src.read())
                files.append(str(dest.relative_to(target)))
    return {"folder": str(target / run), "files": files}


def write_ready_file(web_port):
    """Tell SPOT.m (spot.console.start) the ports and token: .spot_cache/console.json."""
    path = Path(LOCAL["ready_file"])
    path.parent.mkdir(parents=True, exist_ok=True)
    data = {"web_port": web_port, "worker_port": matlab.port, "token": LOCAL["token"], "pid": os.getpid(),
            "root": LOCAL["root"]}
    tmp = path.with_suffix(".tmp")
    fd = os.open(str(tmp), os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o600)
    with os.fdopen(fd, "w") as fh:
        json.dump(data, fh)
    os.replace(str(tmp), str(path))


# --------------------------------------------------------------------------- folder browser
