# local.py

**File:** `+spot/+console/webapp/spot_console/local.py` · **Part of:** [[the console's server|Console-Server]] · **MATLAB side:** [[spot.worker|spot-worker]]

<!-- source: +spot/+console/webapp/spot_console/local.py -->

## What it is for

Everything that only the **local console** (the one `SPOT` starts from MATLAB) has: the bridge to the MATLAB worker, the background runners for a simulation and an experiment build, the link to the lab console, downloading a collected run, and the ready file that tells MATLAB the ports.

## Function by function

**Lines 23–37.** `LOCAL`: whether local mode is on, the session token, the project folder, the ready file. `MATLAB_COMMANDS`: every command the page may send to MATLAB and how long each may take (seconds; `None` = no limit); a command not in this table is refused by the server.

**Lines 40–41.** `check_token`: constant-time comparison with the session token.

**Lines 44–197, `MatlabBridge`.** The socket the worker connects to:
- `listen` (65–72): a TCP server on `127.0.0.1` (this computer only), port 0 = any free port; `_accept_loop` serves each connection in a thread.
- `_serve` (79–132): the first line must be the worker's hello with the right token; a new MATLAB session replaces the old connection; then every line is either an **event** pushed by MATLAB (simulation progress, debug messages, live samples → published as `matlab_event`) or a **reply** matched to its pending request by id. On disconnect, every pending request fails with "MATLAB disconnected".
- `request` (138–171): one command at a time (a lock); if MATLAB is busy with a long command, give up after 0.5 s (else 6 s) with "MATLAB is busy (… for N s)"; send `{id, cmd, args}`, wait up to the timeout, raise the reply's error (with details) or return its result. `busy` is published so the page's badge shows what MATLAB is doing (`BUSY_LABELS`, 200–205).
- `interrupt` (174–197): send a command while another is running (`sim.stop` during `sim.run`); MATLAB handles it when Simulink lets callbacks through.

**Lines 211–298, `SimRunner`.** A long MATLAB command in a background thread, with events named after it (`sim`, `build`): `start` refuses a second run, logs who asked, starts `_run` (and `_follow_log` when the job writes a log file: the build's `build.log` is streamed as `build_log` events while MATLAB can't answer); `_run` records ok/error/details/summary and bumps `result_id`; `result_path` is the result file, only if it is inside `.spot_cache`. Two instances: the simulation and the experiment build.

**Lines 301–463, `LabLink`.** The local console's view of the lab console: `_run` loops forever: GET `/api/state` (must look like a lab console), take a snapshot, then read `/api/events` and **relay** the events the page needs (`frame` at most 30 Hz, `monitor`, `stream_status`, `hardware_platform`, `experiment`, `live_data`, `design`, `calibration`) as `lab_*` events, updating the snapshot (which also holds the lab's `calibration`); on any error mark unreachable with the reason and retry after 3 s. `set_address` switches to another address (from the Experiment tab's field) by closing the current stream. `request` forwards an action to the lab console (POST or GET, with a requester header; the GETs the page may make are `READS`, the lab's calibration among them), refusing when unreachable ("experiments and calibrations need the lab network") or when the lab console is too old for experiments.

**Lines 466–483.** `download_run`: fetch a run's data from the lab console as a tar.gz and unpack only well-formed `run/PLATFORM/ExpLog*` members into `Saved Data/<run>/`.

**Lines 486–496.** `write_ready_file`: `.spot_cache/console.json` with the web port, worker port, token, pid and root, written atomically with mode 600 (the token is a secret).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/local.py</b> (499 lines; click to show)</summary>

```python
   1  """The local console (started by SPOT in MATLAB): the bridge to the user's MATLAB worker, the
   2  simulation and build runners, and the link to the lab console (TX2).
   3  
   4  Part of the SPOT Console (webapp/app.py starts it).
   5  """
   6  
   7  import hmac
   8  import io
   9  import json
  10  import os
  11  import re
  12  import socket
  13  import tarfile
  14  import threading
  15  import time
  16  from pathlib import Path
  17  
  18  from .common import ApiError
  19  from .events import DEFAULT_PLATFORMS, hub
  20  from .experiment import NAME_RE
  21  
  22  
  23  LOCAL = {"enabled": False, "token": "", "root": None, "ready_file": None, "server": None}
  24  
  25  # Commands the page may send, and how long each may take (seconds; None = no limit)
  26  
  27  
  28  MATLAB_COMMANDS = {
  29      "ping": 10, "info": 10, "settings.get": 120, "settings.set": 120, "settings.reset": 120,
  30      "settings.open": 900, "settings.saveAs": 900, "diagram.open": 120, "diagram.showBlock": 120,
  31      "massprops.writeTracker": 30,
  32      "workspace.list": 60, "workspace.inspect": 60, "workspace.eval": None,
  33      "experiment.plan": 120, "experiment.connect": 600, "experiment.decode": 900,
  34      # Results page: Saved Data, loaded datasets, plots and 3D playback (data.load may open a dialog)
  35      "data.list": 120, "data.load": 900, "data.unload": 30, "data.series": 300, "data.trajectory": 300,
  36      "data.saveSimulation": 120, "data.figure": 900,
  37  }
  38  
  39  
  40  def check_token(value):
  41      return bool(LOCAL["token"]) and hmac.compare_digest(str(value or ""), LOCAL["token"])
  42  
  43  
  44  class MatlabBridge:
  45      def __init__(self):
  46          self._lock = threading.Lock()        # one command at a time
  47          self._state = threading.Lock()
  48          self._conn = None
  49          self._file = None
  50          self._pending = {}                   # id -> [event, reply]
  51          self._next_id = 1
  52          self.info = None
  53          self.connected_at = None
  54          self.busy = None                     # {"cmd", "since"} while MATLAB is working
  55          self.port = None
  56  
  57      def status(self):
  58          with self._state:
  59              return {"connected": self._conn is not None, "info": self.info, "since": self.connected_at,
  60                      "busy": dict(self.busy) if self.busy else None}
  61  
  62      def _publish(self):
  63          hub.publish("matlab", self.status())
  64  
  65      def listen(self, port):
  66          srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  67          srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
  68          srv.bind(("127.0.0.1", port))   # this computer only
  69          srv.listen(2)
  70          self.port = srv.getsockname()[1]
  71          threading.Thread(target=self._accept_loop, args=(srv,), daemon=True).start()
  72          return self.port
  73  
  74      def _accept_loop(self, srv):
  75          while True:
  76              conn, _ = srv.accept()
  77              threading.Thread(target=self._serve, args=(conn,), daemon=True).start()
  78  
  79      def _serve(self, conn):
  80          f = conn.makefile("rb")
  81          try:
  82              conn.settimeout(10)
  83              hello = json.loads(f.readline().decode("utf-8") or "{}")
  84              if hello.get("hello") != "spot-worker" or not check_token(hello.get("token")):
  85                  conn.close()
  86                  return
  87              conn.settimeout(None)
  88          except (OSError, ValueError):
  89              conn.close()
  90              return
  91          with self._state:
  92              old = self._conn
  93              self._conn, self._file, self.info, self.connected_at = conn, f, hello.get("info"), time.time()
  94              self.busy = None
  95          if old is not None:   # a new MATLAB session replaces the old one
  96              try:
  97                  old.close()
  98              except OSError:
  99                  pass
 100          hub.log("matlab", "MATLAB %s connected (%s)" % ((self.info or {}).get("release", "?"), (self.info or {}).get("root", "")))
 101          self._publish()
 102          try:
 103              for raw in f:
 104                  try:
 105                      text = raw.decode("utf-8")
 106                  except UnicodeDecodeError:
 107                      text = raw.decode("latin-1")   # an older worker writing other encodings
 108                  try:
 109                      msg = json.loads(text)
 110                  except ValueError:
 111                      hub.log("matlab", "Unreadable message from MATLAB: %s" % text[:200])
 112                      continue
 113                  if "event" in msg:   # pushed by MATLAB, e.g. simulation progress
 114                      hub.publish("matlab_event", msg)
 115                      continue
 116                  with self._state:
 117                      waiter = self._pending.get(msg.get("id"))
 118                  if waiter:
 119                      waiter[1] = msg
 120                      waiter[0].set()
 121          except OSError:
 122              pass
 123          with self._state:
 124              if self._conn is not conn:
 125                  return
 126              self._conn, self._file = None, None
 127              for waiter in self._pending.values():
 128                  waiter[1] = {"ok": False, "error": "MATLAB disconnected."}
 129                  waiter[0].set()
 130              self.busy = None
 131          hub.log("matlab", "MATLAB disconnected")
 132          self._publish()
 133  
 134      def _long_running(self):
 135          busy = self.busy
 136          return bool(busy) and busy.get("cmd") in ("sim.run", "experiment.build", "experiment.decode", "workspace.eval")
 137  
 138      def request(self, cmd, args=None, timeout=60):
 139          # A quick command (settings, a plot) finishes within seconds: wait for it; a simulation doesn't
 140          if not self._lock.acquire(timeout=0.5 if self._long_running() else 6):
 141              busy = self.busy or {}
 142              raise ApiError("MATLAB is busy (%s for %d s). Try again when it has finished."
 143                             % (busy.get("label", busy.get("cmd", "working")), time.time() - busy.get("since", time.time())))
 144          try:
 145              with self._state:
 146                  conn = self._conn
 147                  if conn is None:
 148                      raise ApiError("MATLAB is not connected. Run SPOT in MATLAB (in this project's folder).")
 149                  rid = self._next_id
 150                  self._next_id += 1
 151                  waiter = [threading.Event(), None]
 152                  self._pending[rid] = waiter
 153                  self.busy = {"cmd": cmd, "since": time.time(), "label": BUSY_LABELS.get(cmd, cmd)}
 154              self._publish()
 155              try:
 156                  conn.sendall((json.dumps({"id": rid, "cmd": cmd, "args": args or {}}) + "\n").encode("utf-8"))
 157                  if not waiter[0].wait(timeout):
 158                      raise ApiError("MATLAB did not answer within %d s (%s)." % (timeout, cmd))
 159              except OSError:
 160                  raise ApiError("Lost the connection to MATLAB.")
 161              finally:
 162                  with self._state:
 163                      self._pending.pop(rid, None)
 164                      self.busy = None
 165                  self._publish()
 166              reply = waiter[1] or {}
 167              if not reply.get("ok"):
 168                  raise ApiError(reply.get("error") or "MATLAB reported an error.", reply.get("details"))
 169              return reply.get("result")
 170          finally:
 171              self._lock.release()
 172  
 173  
 174      def interrupt(self, cmd, timeout=15):
 175          """Send a command while another is running (e.g. sim.stop during sim.run). MATLAB handles
 176          it when the running command lets callbacks through, as Simulink does while simulating."""
 177          with self._state:
 178              conn = self._conn
 179              if conn is None:
 180                  raise ApiError("MATLAB is not connected.")
 181              rid = self._next_id
 182              self._next_id += 1
 183              waiter = [threading.Event(), None]
 184              self._pending[rid] = waiter
 185          try:
 186              conn.sendall((json.dumps({"id": rid, "cmd": cmd, "args": {}}) + "\n").encode("utf-8"))
 187              if not waiter[0].wait(timeout):
 188                  raise ApiError("MATLAB did not respond to %s." % cmd)
 189          except OSError:
 190              raise ApiError("Lost the connection to MATLAB.")
 191          finally:
 192              with self._state:
 193                  self._pending.pop(rid, None)
 194          reply = waiter[1] or {}
 195          if not reply.get("ok"):
 196              raise ApiError(reply.get("error") or "MATLAB reported an error.")
 197          return reply.get("result")
 198  
 199  
 200  BUSY_LABELS = {"sim.run": "running the simulation", "experiment.build": "building the experiment",
 201                 "experiment.connect": "connecting to the TX2", "experiment.decode": "converting the data", "workspace.eval": "running a command",
 202                 "settings.open": "waiting for the file dialog", "settings.saveAs": "waiting for the file dialog",
 203                 "diagram.open": "opening the diagram", "diagram.showBlock": "opening the diagram", "data.load": "loading results", "data.series": "reading signals",
 204                 "data.trajectory": "preparing the playback", "data.figure": "drawing the figure (or waiting for its file dialog)",
 205                 "data.saveSimulation": "saving the simulation"}
 206  
 207  
 208  matlab = MatlabBridge()
 209  
 210  
 211  class SimRunner:
 212      """Runs a long MATLAB command in the background (a simulation, or an experiment build) and
 213      reports through events named after it ("sim", "build")."""
 214  
 215      def __init__(self, event="sim", cmd="sim.run", what="Simulation", log_name=None):
 216          self.event, self.cmd, self.what = event, cmd, what
 217          self.log_name = log_name   # a file in .spot_cache that MATLAB writes while working (shown live)
 218          self._lock = threading.Lock()
 219          self.state = {"running": False, "started": None, "finished": None, "ok": None, "error": None,
 220                        "error_details": None, "summary": None, "result_id": 0}
 221  
 222      def status(self):
 223          with self._lock:
 224              return dict(self.state)
 225  
 226      def start(self, requester):
 227          with self._lock:
 228              if self.state["running"]:
 229                  raise ApiError("A simulation is already running.")
 230              self.state.update(running=True, started=time.time(), finished=None, ok=None, error=None,
 231                                error_details=None, log_tail=[])
 232          hub.log("matlab", "%s started (by %s)" % (self.what, requester))
 233          hub.publish(self.event, self.status())
 234          threading.Thread(target=self._run, daemon=True).start()
 235          if self.log_name:
 236              threading.Thread(target=self._follow_log, daemon=True).start()
 237          return self.status()
 238  
 239      def _follow_log(self):
 240          """Send what MATLAB writes to the log file while the job runs (it can't answer meanwhile)."""
 241          path = Path(LOCAL["root"]) / ".spot_cache" / self.log_name
 242          started = self.status()["started"]
 243          pos, partial = 0, ""
 244          while True:
 245              running = self.status()["running"]
 246              try:
 247                  if path.is_file() and path.stat().st_mtime >= started - 1:
 248                      with open(str(path), "r", encoding="utf-8", errors="replace") as f:
 249                          f.seek(pos)
 250                          chunk = f.read()
 251                          pos = f.tell()
 252                      if chunk:
 253                          text = re.sub(r"<a [^>]*>|</a>|[\x08]", "", partial + chunk)
 254                          lines = text.split("\n")
 255                          partial = lines.pop()   # an unfinished line waits for the rest
 256                          lines = [l.rstrip() for l in lines if l.strip()]
 257                          if lines:
 258                              with self._lock:
 259                                  self.state["log_tail"] = ((self.state.get("log_tail") or []) + lines)[-300:]
 260                              hub.publish(self.event + "_log", {"lines": lines})
 261              except OSError:
 262                  pass
 263              if not running:
 264                  return
 265              time.sleep(0.5)
 266  
 267      def _run(self):
 268          ok, error, details, summary = False, None, None, None
 269          try:
 270              summary = matlab.request(self.cmd, {}, timeout=None)
 271              ok = True
 272          except ApiError as e:
 273              error, details = str(e), e.details
 274          with self._lock:
 275              self.state.update(running=False, finished=time.time(), ok=ok, error=error, error_details=details,
 276                                summary=summary)
 277              if ok:
 278                  self.state["result_id"] += 1
 279          hub.log("matlab", "%s %s" % (self.what, "finished in %.1f s" % summary.get("seconds", 0) if ok else "failed: %s" % error))
 280          hub.publish(self.event, self.status())
 281  
 282      def result_path(self):
 283          summary = self.status().get("summary") or {}
 284          path = Path(summary.get("file") or "")
 285          cache = (Path(LOCAL["root"]) / ".spot_cache").resolve() if LOCAL["root"] else None
 286          try:
 287              path = path.resolve()
 288          except OSError:
 289              return None
 290          if not cache or cache not in path.parents or not path.is_file():
 291              return None
 292          return path
 293  
 294  
 295  sim_runner = SimRunner()
 296  
 297  
 298  build_runner = SimRunner("build", "experiment.build", "Experiment build", log_name="build.log")
 299  
 300  
 301  class LabLink:
 302      """The local console's view of the lab console (TX2): follows its event stream and passes
 303      PhaseSpace frames, hardware, stream and experiment status on to the page (as lab_* events), and
 304      forwards the lab actions the Experiment tab may take. Unreachable (not on the lab network):
 305      those actions are refused."""
 306  
 307      RELAY = {"frame": "lab_frame", "monitor": "lab_monitor", "stream_status": "lab_stream",
 308               "hardware_platform": "lab_hardware", "experiment": "lab_experiment", "live_data": "lab_live_data",
 309               "design": "lab_design", "calibration": "lab_calibration"}
 310      ACTIONS = ("stream/start", "stream/stop", "tracker_positions", "experiment/deploy", "experiment/start",
 311                 "experiment/stop", "experiment/fetch", "hardware/off", "design")
 312      READS = ("experiment/build", "experiment/live", "calibration")   # GET requests the page may make through the local server
 313  
 314      PORT = 8080   # the lab console's web port
 315  
 316      def __init__(self, url):
 317          self.url = url.rstrip("/")
 318          self._lock = threading.Lock()
 319          self._response = None   # the event stream being read (closed to switch to a new address)
 320          self.reachable = False
 321          self.experiments = False   # whether its version has the experiment API
 322          self.error = "Connecting…"
 323          self.since = None
 324          self.snapshot = {}      # stream, monitor, hardware, experiment as of the last event
 325          self._last_frame = 0.0
 326  
 327      def status(self):
 328          with self._lock:
 329              return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
 330                      "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}
 331  
 332      def _set(self, reachable, error=None):
 333          with self._lock:
 334              changed = reachable != self.reachable or error != self.error
 335              self.reachable, self.error = reachable, error
 336              if changed:
 337                  self.since = time.time()
 338          if changed:
 339              hub.publish("lab", self.status())
 340  
 341      def start(self):
 342          threading.Thread(target=self._run, daemon=True).start()
 343  
 344      @staticmethod
 345      def url_for(address):
 346          """The lab console's URL for an IPv4 address (the settings' labConsoleAddress)."""
 347          parts = str(address).strip().split(".")
 348          if len(parts) != 4 or not all(p.isdigit() and int(p) <= 255 for p in parts):
 349              raise ApiError("The lab console address must be an IPv4 address such as 192.168.1.116.")
 350          return "http://%s:%d" % (".".join(str(int(p)) for p in parts), LabLink.PORT)
 351  
 352      def set_address(self, address):
 353          """Follow the lab console at another address (from the Experiment tab)."""
 354          url = self.url_for(address)
 355          with self._lock:
 356              if url == self.url:
 357                  return self.status_unlocked()
 358              self.url = url
 359              self.reachable, self.experiments, self.snapshot = False, False, {}
 360              self.error, self.since = "Connecting…", time.time()
 361              response, self._response = self._response, None
 362          if response is not None:
 363              try:
 364                  response.close()   # the reading loop then reconnects, to the new address
 365              except Exception:
 366                  pass
 367          hub.publish("lab", self.status())
 368          return self.status()
 369  
 370      def status_unlocked(self):
 371          return {"url": self.url, "reachable": self.reachable, "experiments": self.experiments,
 372                  "error": self.error, "since": self.since, "snapshot": dict(self.snapshot)}
 373  
 374      def _run(self):
 375          import urllib.request
 376          while True:
 377              url = self.url
 378              try:
 379                  with urllib.request.urlopen(url + "/api/state", timeout=4) as r:
 380                      st = json.loads(r.read().decode("utf-8"))
 381                  mode = (st.get("features") or {}).get("mode")
 382                  if mode == "local" or (mode is None and not ("hardware" in st and "stream" in st)):
 383                      raise OSError("%s is not the lab console" % url)
 384                  with self._lock:
 385                      if url != self.url:   # the address changed meanwhile
 386                          continue
 387                      # Older lab consoles (before the experiment API) still stream and drive the hardware
 388                      self.experiments = "experiment" in st
 389                      self.snapshot = {"stream": st.get("stream"), "monitor": st.get("monitor"),
 390                                       "hardware": st.get("hardware"), "experiment": st.get("experiment"),
 391                                       "led_owner": st.get("led_owner"), "design": st.get("design"),
 392                                       "calibration": st.get("calibration")}
 393                  self._set(True)
 394                  with urllib.request.urlopen(url + "/api/events", timeout=40) as r:
 395                      with self._lock:
 396                          self._response = r
 397                      event = None
 398                      for raw in r:
 399                          line = raw.decode("utf-8", "replace").rstrip("\r\n")
 400                          if line.startswith("event: "):
 401                              event = line[7:]
 402                          elif line.startswith("data: ") and event in self.RELAY:
 403                              self._relay(event, line[6:])
 404              except (OSError, ValueError, AttributeError) as e:
 405                  if url != self.url:   # closed to follow a new address: try it at once
 406                      continue
 407                  reason = getattr(e, "reason", None) or e
 408                  self._set(False, "Not reachable (%s)" % reason)
 409              if url != self.url:
 410                  continue
 411              time.sleep(3)
 412  
 413      def _relay(self, event, data):
 414          if event == "frame":   # at most 30 Hz to the page
 415              now = time.time()
 416              if now - self._last_frame < 1 / 30:
 417                  return
 418              self._last_frame = now
 419          try:
 420              payload = json.loads(data)
 421          except ValueError:
 422              return
 423          key = {"monitor": "monitor", "stream_status": "stream", "experiment": "experiment", "design": "design",
 424                 "calibration": "calibration"}.get(event)
 425          with self._lock:
 426              if key:
 427                  self.snapshot[key] = payload
 428              elif event == "hardware_platform" and self.snapshot.get("hardware"):
 429                  plats = self.snapshot["hardware"].get("platforms", [])
 430                  self.snapshot["hardware"]["platforms"] = [payload if p.get("name") == payload.get("name") else p for p in plats]
 431          hub.publish(self.RELAY[event], payload)
 432  
 433      def request(self, method, path, body=None, timeout=60, raw=False):
 434          import urllib.request
 435          import urllib.error
 436          st = self.status()
 437          if not st["reachable"]:
 438              raise ApiError("The lab console (%s) isn't reachable: experiments and calibrations need the "
 439                             "lab network (SRCL Wi-Fi or wired)." % self.url)
 440          if path.startswith("experiment/") and not st["experiments"]:
 441              raise ApiError("The lab console (%s) runs an older version without experiments: update it "
 442                             "with +spot/+console/deploy/deploy_tx2.sh." % self.url)
 443          headers = {"X-SPOT-Requester": "%s, local console" % socket.gethostname()}
 444          data = None
 445          if method == "POST":
 446              headers["Content-Type"] = "application/json"
 447              data = json.dumps(body or {}).encode("utf-8")
 448          req = urllib.request.Request(self.url + "/api/" + path, data=data, headers=headers, method=method)
 449          try:
 450              with urllib.request.urlopen(req, timeout=timeout) as r:
 451                  out = r.read()
 452          except urllib.error.HTTPError as e:
 453              try:
 454                  msg = json.loads(e.read().decode("utf-8")).get("error")
 455              except ValueError:
 456                  msg = None
 457              raise ApiError("Lab console: %s" % (msg or e))
 458          except OSError as e:
 459              raise ApiError("Lab console not reachable: %s" % (getattr(e, "reason", None) or e))
 460          return out if raw else json.loads(out.decode("utf-8"))
 461  
 462  
 463  lab = None   # LabLink, local mode only
 464  
 465  
 466  def download_run(run):
 467      """Copy a run's data from the lab console into <project>/Saved Data/<run>/<PLATFORM>/ExpLog*."""
 468      if not NAME_RE.match(run or ""):
 469          raise ApiError("%r is not a run name." % run)
 470      data = lab.request("GET", "experiment/download?run=" + run, timeout=300, raw=True)
 471      target = Path(LOCAL["root"]) / "Saved Data"
 472      files = []
 473      with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as tar:
 474          for member in tar.getmembers():
 475              parts = Path(member.name).parts   # run/PLATFORM/ExpLog...
 476              if (member.isfile() and len(parts) == 3 and parts[0] == run and parts[1] in DEFAULT_PLATFORMS
 477                      and NAME_RE.match(parts[2])):
 478                  dest = target / run / parts[1] / parts[2]
 479                  dest.parent.mkdir(parents=True, exist_ok=True)
 480                  with tar.extractfile(member) as src:
 481                      dest.write_bytes(src.read())
 482                  files.append(str(dest.relative_to(target)))
 483      return {"folder": str(target / run), "files": files}
 484  
 485  
 486  def write_ready_file(web_port):
 487      """Tell SPOT.m (spot.console.start) the ports and token: .spot_cache/console.json."""
 488      path = Path(LOCAL["ready_file"])
 489      path.parent.mkdir(parents=True, exist_ok=True)
 490      data = {"web_port": web_port, "worker_port": matlab.port, "token": LOCAL["token"], "pid": os.getpid(),
 491              "root": LOCAL["root"]}
 492      tmp = path.with_suffix(".tmp")
 493      fd = os.open(str(tmp), os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o600)
 494      with os.fdopen(fd, "w") as fh:
 495          json.dump(data, fh)
 496      os.replace(str(tmp), str(path))
 497  
 498  
 499  # --------------------------------------------------------------------------- folder browser
```

</details>
<!-- /listing -->
