# experiment.py

**File:** `+spot/+console/webapp/spot_console/experiment.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Experiment Tab|Experiment-Tab]] · **MATLAB side:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+console/webapp/spot_console/experiment.py -->

## What it is for

Experiments, from the lab console: find the program MATLAB built on this computer, deploy it to the platforms, start them **together**, watch them, stop them, collect their data; and receive what the running programs send back: their clocks, their debug messages and their live streams.

The synchronised start: every platform's program is started; each one sends its experiment clock (still 0) by UDP as soon as it runs; when all have been heard from, the lab console tells StreamData `exp start 3`, so three seconds later the PhaseSpace stream carries a clock that every platform reads ([[spot.hardware.experimentTime|spot-hardware-experimentTime]]) and they all leave phase 0 at the same instant.

## Function by function

**Lines 24–55.** Where MATLAB's builds land (`~/ExperimentSoftware/MATLAB_ws`), where collected data goes (`~/ExperimentData`), the UDP ports (clocks 48720–48722, debug messages 55001–55003 plus the old broadcast port 55000, live streams 55011–55013), the countdown (3 s) and how long to wait for every program (90 s), how much is kept, and the pattern a run or file name must match.

**Lines 57–119.** `Experiment` state (phase `idle → deploying → deployed → starting → running → stopping → stopped`, the model, the build, each platform's state, the clocks, the sync state, the messages, the live streams) and `status` (what the pages get: including each clock's age, the clocks' spread in ms (`_spread_ms`, 95–102: each clock advanced by its age; network delay is part of it), the newest 100 messages per platform and the stream list).

**Lines 123–143.** `find_build` / `build_info`: the newest `<model>.elf` under the build root.

**Lines 147–202.** `deploy`: check the platforms and the build; refuse during another experiment; set the phase; in a thread, pack the `.elf` and any `.so` next to it into a tar and send it to each platform in parallel (`exp-receive`), recording "deployed" with any missing libraries, or "failed".

**Lines 204–262.** `start`: the platforms must be deployed, PhaseSpace must be streaming (the programs take their clock from it), no calibration may be running (`calibrator.active()`, imported inside the function because [[calibration.py|Console-calibration]] imports this module), each platform's Teensy must be free; reset the clocks, sync, messages and live streams; `exp clear` to StreamData; tell the hardware driver to let go of each Teensy (`make_way`); start each program (`exp-start`) in parallel; if any failed, stop the others (never half an experiment); then start the monitor thread and the agreed-start thread.

**Lines 264–302.** `_agree_start`: wait until every platform's clock has arrived since the start (up to 90 s; otherwise `failed` with an explanation); if streaming stopped meanwhile, fail; otherwise `exp start 3`, `counting`, then `running`.

**Lines 304–330.** `_monitor`: every 2 s, `exp-status` on each running platform (running or exited, data file count, the log's tail); when every program has ended, the phase becomes `stopped`.

**Lines 332–359.** `stop`: `exp-stop` on each platform in parallel, then `send_off` (all-off packets to the Teensy now rather than after its watchdog); `exp clear`; note any platform that couldn't be confirmed stopped ("use the E-stop").

**Lines 363–409.** `fetch`: not while running; a run name `<model>_<stamp>`; per platform in parallel: write the received debug messages as `debug_messages.txt`, `exp-data` (a tar of the `ExpLog*` files) unpacked into `~/ExperimentData/<run>/<PLATFORM>/`, then `exp-clean` on the platform; record `last_run`.

**Lines 411–418.** `archive`: a run's folder as tar.gz for the local console to download ([[local.py|Console-local]]'s `download_run`).

**Lines 422–449.** The clock listeners: one UDP socket per platform; each 8-byte double received is that platform's experiment time, stored with its arrival time; published at most twice a second.

**Lines 454–499.** The debug message listeners: per-platform ports carry `[double t][text]`; the legacy port carries bare text (the platform known from its address). `add_debug`: a repeat of the last text only counts, a change starts a new entry; at most 300 kept.

**Lines 504–568.** The live stream listeners: `parse_live` (571–584) decodes the packet (version 1, name length, count, time, name, doubles; NaN → null); `add_live` keeps up to 12 000 rows per `PLATFORM/name` (restarting if the width changes); `_live_publisher` sends new rows to the pages every 0.2 s as `live_data` and refreshes the status when a stream appears; `live_data` returns everything kept, thinned to 3000 rows per stream, for a page opened later.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/experiment.py</b> (593 lines; click to show)</summary>

```python
   1  """Experiments on the platforms, run from the lab console: finding the build, deploying it, starting
   2  and stopping it, following the platforms' clocks, and collecting the data files.
   3  
   4  Part of the SPOT Console (webapp/app.py starts it).
   5  """
   6  
   7  import collections
   8  import io
   9  import math
  10  import re
  11  import socket
  12  import struct
  13  import tarfile
  14  import threading
  15  import time
  16  from pathlib import Path
  17  
  18  from .common import ApiError, decode, shown, shown_list
  19  from .events import hub, settings
  20  from .hardware import StepFailed, hardware, in_parallel, platform_run
  21  from .phasespace import streamer
  22  
  23  
  24  EXP_BUILD_ROOT = Path.home() / "ExperimentSoftware" / "MATLAB_ws"
  25  
  26  
  27  EXP_DATA_ROOT = Path.home() / "ExperimentData"
  28  
  29  
  30  EXP_CLOCK_PORTS = {"RED": 48720, "BLACK": 48721, "BLUE": 48722}   # each platform sends its experiment time
  31  
  32  # The agreed start: once every platform's program is running (each sends its clock, still 0),
  33  # StreamData is told to start the experiment clock this many seconds later, in the PhaseSpace
  34  # stream all platforms receive (spot.hardware.experimentTime), so they start together.
  35  SYNC_COUNTDOWN = 3.0
  36  SYNC_READY_TIMEOUT = 90.0   # [s] for every program to start sending its clock
  37  
  38  
  39  # The platforms' debug messages (spot.hardware.debugMessages): [experiment time, double][text],
  40  # each platform on its own port. The original diagram broadcast the bare text to 55000 instead;
  41  # that is still understood (the platform is known from its address).
  42  EXP_DEBUG_PORTS = {"RED": 55001, "BLACK": 55002, "BLUE": 55003}
  43  EXP_DEBUG_LEGACY_PORT = 55000
  44  DEBUG_KEEP = 300   # messages kept per platform (only changes are kept)
  45  DEBUG_SHOW = 100   # of which the status carries the newest
  46  
  47  # The Live Stream blocks' signals (spot.hardware.liveStream), for the Live Plot: per platform port,
  48  # [uint8 version 1][uint8 name length L][uint16 count n][double t][L name characters][n doubles]
  49  EXP_LIVE_PORTS = {"RED": 55011, "BLACK": 55012, "BLUE": 55013}
  50  LIVE_KEEP = 12000      # samples kept per stream (10 min at 20 Hz)
  51  LIVE_PUBLISH = 0.2     # [s] new samples go to the pages this often
  52  
  53  
  54  NAME_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,99}$")
  55  
  56  
  57  class Experiment:
  58      """One experiment at a time for the whole lab: deploy, start, stop, collect the data."""
  59  
  60      def __init__(self):
  61          self._lock = threading.Lock()
  62          self.phase = "idle"   # idle, deploying, deployed, starting, running, stopping, stopped
  63          self.model = None
  64          self.build = None     # {"path", "built", "size"} of the program deployed
  65          self.platforms = {}   # NAME -> {"state", "detail", "pid", "missing", "data", "log"}
  66          self.by = None
  67          self.started = None
  68          self.stopped = None
  69          self.note = None
  70          self.last_run = None  # {"run", "files"} from the last data collection
  71          self.clocks = {}      # NAME -> [experiment time, received]
  72          self.sync = {"state": "idle"}   # the agreed start: idle, waiting, counting, running, failed
  73          self.debug = {n: [] for n in EXP_DEBUG_PORTS}   # NAME -> [{t, text, first, last, count}]
  74          self.debug_seen = {}  # NAME -> when its last message (changed or not) arrived
  75          self.live = {}        # "PLATFORM/name" -> {"platform", "name", "n", "rows": deque of [t, v...], "last"}
  76          self.live_new = {}    # "PLATFORM/name" -> rows not yet sent to the pages
  77  
  78      def status(self):
  79          with self._lock:
  80              now = time.time()
  81              return {"phase": self.phase, "model": self.model, "build": self.build, "by": self.by,
  82                      "started": self.started, "stopped": self.stopped, "note": self.note,
  83                      "platforms": {n: dict(p) for n, p in self.platforms.items()},
  84                      "last_run": self.last_run,
  85                      "clocks": {n: {"t": c[0], "age": round(now - c[1], 2)} for n, c in self.clocks.items()},
  86                      "sync": dict(self.sync, spread_ms=self._spread_ms(now)),
  87                      "live": [{"key": k, "platform": s["platform"], "name": s["name"], "n": s["n"],
  88                                "samples": len(s["rows"]), "age": round(now - s["last"], 1)}
  89                               for k, s in sorted(self.live.items())],
  90                      "debug": {n: {"entries": e[-DEBUG_SHOW:],
  91                                    "seen": round(now - self.debug_seen[n], 1) if n in self.debug_seen else None}
  92                                for n, e in self.debug.items()},
  93                      "build_root": str(EXP_BUILD_ROOT)}
  94  
  95      def _spread_ms(self, now):
  96          """How far apart the platforms' experiment clocks are now [ms], or None. Each clock is
  97          moved on by the time since it arrived; the network's delays are part of the result."""
  98          fresh = [c[0] + (now - c[1]) for n, c in self.clocks.items()
  99                   if self.platforms.get(n, {}).get("state") == "running" and now - c[1] < 1.0 and c[0] > 0]
 100          if len(fresh) < 2:
 101              return None
 102          return round(1000 * (max(fresh) - min(fresh)), 1)
 103  
 104      def _publish(self):
 105          hub.publish("experiment", self.status())
 106  
 107      def _update(self, name, **fields):
 108          with self._lock:
 109              self.platforms.setdefault(name, {}).update(fields)
 110          self._publish()
 111  
 112      def _log(self, line):
 113          hub.log("hardware", "%s  Experiment: %s" % (time.strftime("%H:%M:%S"), line))
 114  
 115      def _mark_crashed(self, results):
 116          for n, r in results.items():
 117              if isinstance(r, StepFailed):
 118                  self._update(n, state="failed", detail="internal error: %s" % r.error)
 119  
 120      # ---- the build MATLAB left on this computer
 121  
 122      @staticmethod
 123      def find_build(model):
 124          """The newest <model>.elf under EXP_BUILD_ROOT, or None."""
 125          if not NAME_RE.match(model or ""):
 126              raise ApiError("%r is not a diagram name." % model)
 127          best = None
 128          if EXP_BUILD_ROOT.is_dir():
 129              for path in EXP_BUILD_ROOT.rglob(model + ".elf"):
 130                  try:
 131                      mtime = path.stat().st_mtime
 132                  except OSError:
 133                      continue
 134                  if path.is_file() and (best is None or mtime > best[1]):
 135                      best = (path, mtime)
 136          return best
 137  
 138      def build_info(self, model):
 139          found = self.find_build(model)
 140          if not found:
 141              return None
 142          path, mtime = found
 143          return {"path": str(path), "built": mtime, "size": path.stat().st_size}
 144  
 145      # ---- deploy, start, stop
 146  
 147      def _check_platforms(self, platforms):
 148          names = []
 149          for n in platforms or []:
 150              n = str(n).upper()
 151              hardware.get(n)   # unknown names raise
 152              if n not in names:
 153                  names.append(n)
 154          if not names:
 155              raise ApiError("Choose at least one platform.")
 156          return names
 157  
 158      def deploy(self, model, platforms, requester):
 159          names = self._check_platforms(platforms)
 160          info = self.build_info(model)
 161          if not info:
 162              raise ApiError("There is no build of %s on the lab console yet (looked for %s.elf under %s). "
 163                             "Build it from the local console first." % (model, model, EXP_BUILD_ROOT))
 164          with self._lock:
 165              if self.phase in ("deploying", "starting", "running", "stopping"):
 166                  raise ApiError("An experiment is %s (by %s); wait for it, or stop it first." % (self.phase, self.by))
 167              self.phase, self.model, self.build, self.by, self.note = "deploying", model, info, requester, None
 168              self.started = self.stopped = None
 169              self.platforms = {n: {"state": "copying", "detail": None, "pid": None, "missing": [], "data": None,
 170                                    "log": []} for n in names}
 171          self._log("deploying %s to %s (by %s)" % (model, shown_list(names), requester))
 172          self._publish()
 173          threading.Thread(target=self._deploy, args=(model, names, Path(info["path"])), daemon=True).start()
 174          return self.status()
 175  
 176      def _deploy(self, model, names, elf):
 177          # Only what the program needs to run: itself and any shared libraries built next to it
 178          buf = io.BytesIO()
 179          with tarfile.open(fileobj=buf, mode="w") as tar:
 180              tar.add(str(elf), arcname=elf.name)
 181              for lib in sorted(elf.parent.glob("*.so*")):
 182                  if lib.is_file():
 183                      tar.add(str(lib), arcname=lib.name)
 184          data = buf.getvalue()
 185  
 186          def one(n):
 187              rc, out, err = platform_run(n, "exp-receive " + model, data=data, timeout=180)
 188              text = decode(out)
 189              missing = [l[len("missing "):] for l in text.splitlines() if l.startswith("missing ")]
 190              if rc == 0:
 191                  self._update(n, state="deployed", detail=text.splitlines()[-1] if text else "copied",
 192                               missing=missing)
 193              else:
 194                  self._update(n, state="failed", detail=err or "copy failed (code %s)" % rc)
 195              return rc == 0
 196          ok = in_parallel(names, one)
 197          self._mark_crashed(ok)
 198          with self._lock:
 199              self.phase = "deployed" if all(ok.values()) else "idle"
 200              self.note = None if all(ok.values()) else "Copying failed on %s." % shown_list(n for n, v in ok.items() if not v)
 201          self._log("deploy %s" % ("done" if all(ok.values()) else "failed: " + self.note))
 202          self._publish()
 203  
 204      def start(self, platforms, requester):
 205          names = self._check_platforms(platforms)
 206          with self._lock:
 207              if self.phase not in ("deployed", "stopped"):
 208                  raise ApiError("Deploy the experiment first (it is %s)." % self.phase)
 209              not_here = [n for n in names if self.platforms.get(n, {}).get("state") not in ("deployed", "stopped", "exited")]
 210              if not_here:
 211                  raise ApiError("%s %s not been deployed to." % (shown_list(not_here), "has" if len(not_here) == 1 else "have"))
 212              model = self.model
 213          if not streamer.status()["running"]:
 214              raise ApiError("Start PhaseSpace streaming first: the platforms take their experiment clock from it, "
 215                             "and wait at time 0 without it.")
 216          from .calibration import calibrator   # (calibration.py imports this module)
 217          if calibrator.active():
 218              raise ApiError("A calibration is running on the lab console (Calibration tab); start the experiment "
 219                             "once it has finished, or stop it there.")
 220          for n in names:   # the platform has to answer, with its Teensy free
 221              pr = hardware.get(n).status()["probe"]
 222              if pr["state"] != "ok":
 223                  raise ApiError("%s: %s" % (shown(n), pr["detail"] or pr["state"]))
 224              if not pr["teensy"]:
 225                  raise ApiError("The %s has no Teensy at /dev/ttyACM0." % shown(n))
 226              if pr["others"]:
 227                  raise ApiError("The %s's Teensy is already in use by %s." % (shown(n), ", ".join(pr["others"])))
 228          with self._lock:
 229              self.phase, self.by, self.started, self.stopped, self.note = "starting", requester, time.time(), None, None
 230              self.clocks = {}
 231              self.sync = {"state": "waiting", "waiting_for": list(names)}
 232              self.debug = {n: [] for n in EXP_DEBUG_PORTS}
 233              self.debug_seen = {}
 234              self.live, self.live_new = {}, {}
 235          streamer.command("exp clear")   # no clock from an earlier run: the new programs wait at 0
 236          self._log("starting %s on %s (by %s)" % (model, shown_list(names), requester))
 237          for n in names:
 238              hardware.get(n).make_way(requester)
 239          self._publish()
 240          time.sleep(0.5)   # let the hardware links close
 241  
 242          def one(n):
 243              rc, out, err = platform_run(n, "exp-start %s %s.elf" % (model, model), timeout=30)
 244              m = re.search(r"(started|already running) (\d+)", decode(out))
 245              if rc == 0 and m:
 246                  self._update(n, state="running", pid=int(m.group(2)), detail=None)
 247                  return True
 248              self._update(n, state="failed", detail=err or decode(out) or "could not start (code %s)" % rc)
 249              return False
 250          ok = in_parallel(names, one)
 251          self._mark_crashed(ok)
 252          if not all(ok.values()):
 253              # Don't leave half an experiment running
 254              self.stop("the console (a platform failed to start)", note="Starting failed on %s, so the others were stopped."
 255                        % ", ".join(n for n, v in ok.items() if not v))
 256              return self.status()
 257          with self._lock:
 258              self.phase = "running"
 259          self._publish()
 260          threading.Thread(target=self._monitor, daemon=True).start()
 261          threading.Thread(target=self._agree_start, args=(names,), daemon=True).start()
 262          return self.status()
 263  
 264      def _agree_start(self, names):
 265          """Once every platform's program runs (it sends its clock, still at 0), start the
 266          experiment clock in the PhaseSpace stream SYNC_COUNTDOWN s later, for all of them."""
 267          with self._lock:
 268              started = self.started
 269          deadline = time.time() + SYNC_READY_TIMEOUT
 270          while True:
 271              with self._lock:
 272                  if self.phase != "running" or self.started != started:
 273                      return
 274                  waiting = [n for n in names if not (n in self.clocks and self.clocks[n][1] > started)]
 275                  self.sync = {"state": "waiting", "waiting_for": waiting}
 276              if not waiting:
 277                  break
 278              if time.time() > deadline:
 279                  with self._lock:
 280                      self.sync = {"state": "failed", "waiting_for": waiting,
 281                                   "detail": "No experiment clock from %s within %d s, so the experiment was not started "
 282                                             "(the platforms wait at time 0). Is the program built from a diagram with the "
 283                                             "PhaseSpace experiment clock?" % (shown_list(waiting), SYNC_READY_TIMEOUT)}
 284                  self._log("no experiment clock from %s: not started" % shown_list(waiting))
 285                  self._publish()
 286                  return
 287              time.sleep(0.2)
 288          if not streamer.status()["running"]:
 289              with self._lock:
 290                  self.sync = {"state": "failed", "detail": "PhaseSpace streaming stopped, so the experiment could not start."}
 291              self._publish()
 292              return
 293          streamer.command("exp start %.3f" % SYNC_COUNTDOWN)
 294          with self._lock:
 295              self.sync = {"state": "counting", "start_at": time.time() + SYNC_COUNTDOWN}
 296          self._log("every platform is running: the experiment starts in %.0f s" % SYNC_COUNTDOWN)
 297          self._publish()
 298          time.sleep(SYNC_COUNTDOWN)
 299          with self._lock:
 300              if self.sync.get("state") == "counting" and self.started == started:
 301                  self.sync = {"state": "running", "since": time.time()}
 302          self._publish()
 303  
 304      def _monitor(self):
 305          while True:
 306              time.sleep(2)
 307              with self._lock:
 308                  if self.phase != "running":
 309                      return
 310                  names = [n for n, p in self.platforms.items() if p.get("state") == "running"]
 311                  model = self.model
 312              def one(n):
 313                  rc, out, _ = platform_run(n, "exp-status " + model, timeout=15)
 314                  lines = decode(out).splitlines()
 315                  running = any(re.match(r"running \d+", l) for l in lines)
 316                  data = next((int(l.split()[1]) for l in lines if l.startswith("data ")), None)
 317                  log = [l[4:] for l in lines if l.startswith("log ")]
 318                  if rc == 0:
 319                      self._update(n, state="running" if running else "exited", data=data, log=log)
 320                  return running
 321              alive = in_parallel(names, one)
 322              crashed = any(isinstance(v, StepFailed) for v in alive.values())
 323              if names and not crashed and not any(alive.values()):
 324                  with self._lock:
 325                      if self.phase == "running":
 326                          self.phase, self.stopped = "stopped", time.time()
 327                          self.note = "The program has ended on every platform."
 328                  self._log("the program has ended on every platform")
 329                  self._publish()
 330                  return
 331  
 332      def stop(self, requester, note=None):
 333          with self._lock:
 334              names = [n for n, p in self.platforms.items() if p.get("state") in ("running", "exited", "failed")]
 335              if self.phase not in ("starting", "running", "stopped") or not names:
 336                  return self.status()
 337              self.phase = "stopping"
 338              model = self.model
 339          self._log("stopping %s on %s (by %s)" % (model, shown_list(names), requester))
 340          self._publish()
 341  
 342          def one(n):
 343              rc, out, err = platform_run(n, "exp-stop " + model, timeout=30)
 344              if rc == 0:
 345                  self._update(n, state="stopped", pid=None, detail=decode(out).strip())
 346                  hardware.get(n).send_off()   # thrusters and pucks off now, not after the watchdog
 347              else:
 348                  self._update(n, state="failed", detail="could not stop: %s" % (err or rc))
 349              return rc == 0
 350          ok = in_parallel(names, one)
 351          self._mark_crashed(ok)
 352          streamer.command("exp clear")   # the experiment clock ends with the programs
 353          with self._lock:
 354              self.sync = {"state": "idle"}
 355              self.phase, self.stopped = "stopped", time.time()
 356              self.note = note if all(ok.values()) else \
 357                  "Could not confirm the stop on %s: use the E-stop." % shown_list(n for n, v in ok.items() if not v)
 358          self._publish()
 359          return self.status()
 360  
 361      # ---- data
 362  
 363      def fetch(self, requester):
 364          with self._lock:
 365              if self.phase in ("deploying", "starting", "running", "stopping"):
 366                  raise ApiError("Stop the experiment before collecting its data.")
 367              if not self.model or not self.platforms:
 368                  raise ApiError("No experiment to collect data from.")
 369              model, names = self.model, list(self.platforms)
 370          run = "%s_%s" % (model, time.strftime("%Y%m%d_%H%M%S"))
 371          folder = EXP_DATA_ROOT / run
 372  
 373          def one(n):
 374              target = folder / n
 375              with self._lock:
 376                  messages = list(self.debug.get(n, []))
 377              if messages:   # the platform's debug messages, as the console received them (also without data)
 378                  target.mkdir(parents=True, exist_ok=True)
 379                  (target / "debug_messages.txt").write_text(
 380                      "experiment time [s]\ttimes received\tmessage\n" + "".join(
 381                          "%s\t%d\t%s\n" % ("" if m["t"] is None else "%.3f" % m["t"], m["count"], m["text"]) for m in messages),
 382                      encoding="utf-8")
 383              rc, out, err = platform_run(n, "exp-data " + model, timeout=300)
 384              if rc != 0:
 385                  return {"files": [], "error": err or "no data (code %s)" % rc}
 386              files = []
 387              target.mkdir(parents=True, exist_ok=True)
 388              try:
 389                  with tarfile.open(fileobj=io.BytesIO(out), mode="r:") as tar:
 390                      for member in tar.getmembers():
 391                          name = Path(member.name).name
 392                          if member.isfile() and name.startswith("ExpLog") and NAME_RE.match(name):
 393                              with tar.extractfile(member) as src:
 394                                  (target / name).write_bytes(src.read())
 395                              files.append(name)
 396              except (tarfile.TarError, OSError) as e:
 397                  return {"files": files, "error": "could not unpack the data: %s" % e}
 398              if files:
 399                  rc, _, _ = platform_run(n, "exp-clean " + model, timeout=30)   # copied off: free the platform's disk
 400                  if rc == 0:
 401                      self._update(n, data=0)
 402              return {"files": files, "error": None}
 403          result = in_parallel(names, one)
 404          result = {n: ({"files": [], "error": r.error} if isinstance(r, StepFailed) else r) for n, r in result.items()}
 405          with self._lock:
 406              self.last_run = {"run": run, "files": result, "at": time.time()}
 407          self._log("collected data into %s (by %s)" % (folder, requester))
 408          self._publish()
 409          return self.status()
 410  
 411      def archive(self, run):
 412          """The collected data of a run as a .tar.gz (for the local console to download)."""
 413          if not NAME_RE.match(run or "") or not (EXP_DATA_ROOT / run).is_dir():
 414              raise ApiError("No data for %r." % run)
 415          buf = io.BytesIO()
 416          with tarfile.open(fileobj=buf, mode="w:gz") as tar:
 417              tar.add(str(EXP_DATA_ROOT / run), arcname=run)
 418          return buf.getvalue()
 419  
 420      # ---- the platforms' experiment clocks (UDP from the diagrams, to this computer)
 421  
 422      def listen_clocks(self):
 423          for name, port in EXP_CLOCK_PORTS.items():
 424              sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 425              sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 426              try:
 427                  sock.bind(("", port))
 428              except OSError as e:
 429                  print("Not listening for %s's experiment time on UDP %d: %s" % (name, port, e))
 430                  continue
 431              threading.Thread(target=self._clock_loop, args=(name, sock), daemon=True).start()
 432  
 433      def _clock_loop(self, name, sock):
 434          last_publish = 0.0
 435          while True:
 436              try:
 437                  data = sock.recv(64)
 438              except OSError:
 439                  return
 440              if len(data) < 8:
 441                  continue
 442              t = struct.unpack("<d", data[:8])[0]
 443              if not math.isfinite(t):
 444                  continue
 445              with self._lock:
 446                  self.clocks[name] = [t, time.time()]
 447              if time.time() - last_publish > 0.5:
 448                  last_publish = time.time()
 449                  self._publish()
 450  
 451  
 452      # ---- the platforms' debug messages (UDP from the diagrams, to this computer)
 453  
 454      def listen_debug(self):
 455          ports = [(name, port) for name, port in EXP_DEBUG_PORTS.items()] + [(None, EXP_DEBUG_LEGACY_PORT)]
 456          for name, port in ports:
 457              sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 458              sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 459              try:
 460                  sock.bind(("", port))
 461              except OSError as e:
 462                  print("Not listening for debug messages on UDP %d: %s" % (port, e))
 463                  continue
 464              threading.Thread(target=self._debug_loop, args=(name, sock), daemon=True).start()
 465  
 466      def _debug_loop(self, name, sock):
 467          while True:
 468              try:
 469                  data, addr = sock.recvfrom(4096)
 470              except OSError:
 471                  return
 472              if name is None:   # the original diagram: the bare text; the platform from its address
 473                  hosts = {p.get("host"): n for n, p in (settings.get("platforms") or {}).items()}
 474                  plat, t, raw = hosts.get(addr[0]), None, data
 475              else:
 476                  if len(data) < 8:
 477                      continue
 478                  plat, raw = name, data[8:]
 479                  t = struct.unpack("<d", data[:8])[0]
 480                  t = round(t, 3) if math.isfinite(t) else None
 481              if plat not in self.debug:
 482                  continue
 483              self.add_debug(plat, t, raw.split(b"\0")[0].decode("ascii", "replace").strip())
 484  
 485      def add_debug(self, name, t, text):
 486          """A platform's message: a change starts a new entry, a repeat only counts."""
 487          now = time.time()
 488          with self._lock:
 489              entries = self.debug[name]
 490              self.debug_seen[name] = now
 491              if entries and entries[-1]["text"] == text:
 492                  entries[-1]["count"] += 1
 493                  entries[-1]["last"] = now
 494                  return
 495              if not text and not entries:
 496                  return   # nothing to show yet
 497              entries.append({"t": t, "text": text, "first": now, "last": now, "count": 1})
 498              del entries[:-DEBUG_KEEP]
 499          self._publish()
 500  
 501  
 502      # ---- the Live Stream blocks' signals, for the Live Plot
 503  
 504      def listen_live(self):
 505          for name, port in EXP_LIVE_PORTS.items():
 506              sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 507              sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 508              sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1 << 20)
 509              try:
 510                  sock.bind(("", port))
 511              except OSError as e:
 512                  print("Not listening for live streams on UDP %d: %s" % (port, e))
 513                  continue
 514              threading.Thread(target=self._live_loop, args=(name, sock), daemon=True).start()
 515          threading.Thread(target=self._live_publisher, daemon=True).start()
 516  
 517      def _live_loop(self, platform, sock):
 518          while True:
 519              try:
 520                  data = sock.recv(65535)
 521              except OSError:
 522                  return
 523              row = parse_live(data)
 524              if row:
 525                  self.add_live(platform, *row)
 526  
 527      def add_live(self, platform, name, t, values):
 528          key = "%s/%s" % (platform, name)
 529          now = time.time()
 530          with self._lock:
 531              s = self.live.get(key)
 532              if s is None or s["n"] != len(values):   # new (or its size changed: start again)
 533                  s = self.live[key] = {"platform": platform, "name": name, "n": len(values),
 534                                        "rows": collections.deque(maxlen=LIVE_KEEP), "last": now}
 535                  self.live_new[key] = []
 536              row = [round(t, 4)] + values
 537              s["rows"].append(row)
 538              s["last"] = now
 539              self.live_new.setdefault(key, []).append(row)
 540  
 541      def _live_publisher(self):
 542          """New samples to the pages a few times a second (event live_data), not one event per packet."""
 543          known = 0
 544          while True:
 545              time.sleep(LIVE_PUBLISH)
 546              with self._lock:
 547                  batch = {k: {"platform": self.live[k]["platform"], "name": self.live[k]["name"],
 548                               "n": self.live[k]["n"], "rows": rows}
 549                           for k, rows in self.live_new.items() if rows and k in self.live}
 550                  self.live_new = {k: [] for k in self.live}
 551                  streams = len(self.live)
 552              if batch:
 553                  hub.publish("live_data", batch)
 554              if streams != known:   # a new stream: the lists in the pages
 555                  known = streams
 556                  self._publish()
 557  
 558      def live_data(self, keep=3000):
 559          """Everything kept so far, at most keep samples per stream (for a page opened later)."""
 560          with self._lock:
 561              out = {}
 562              for k, s in self.live.items():
 563                  rows = list(s["rows"])
 564                  if len(rows) > keep:
 565                      step = len(rows) / float(keep)
 566                      rows = [rows[int(i * step)] for i in range(keep)] + [rows[-1]]
 567                  out[k] = {"platform": s["platform"], "name": s["name"], "n": s["n"], "rows": rows}
 568              return out
 569  
 570  
 571  def parse_live(data):
 572      """A Live Stream packet: (name, t, [values]), or None if it isn't one."""
 573      if len(data) < 12 or data[0] != 1:
 574          return None
 575      length = data[1]
 576      n = struct.unpack("<H", data[2:4])[0]
 577      if len(data) != 12 + length + 8 * n:
 578          return None
 579      t = struct.unpack("<d", data[4:12])[0]
 580      name = data[12:12 + length].decode("ascii", "replace").strip() or "stream"
 581      values = list(struct.unpack("<%dd" % n, data[12 + length:]))
 582      if not math.isfinite(t):
 583          return None
 584      return name, t, [v if math.isfinite(v) else None for v in values]
 585  
 586  
 587  experiment = Experiment()
 588  
 589  
 590  # In local mode (app.py --local, started by SPOT.m in MATLAB) the Simulation tab runs its commands
 591  # in the user's MATLAB session: the MATLAB worker (+spot/+worker/Worker.m) connects to
 592  # 127.0.0.1:<worker port>, proves it was started with this server's token, and then answers one
 593  # JSON command per line. MATLAB runs one command at a time, so requests are sent one at a time.
```

</details>
<!-- /listing -->
