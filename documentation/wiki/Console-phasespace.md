# phasespace.py

**File:** `+spot/+console/webapp/spot_console/phasespace.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Lab Console: PhaseSpace Tab|Lab-Console-PhaseSpace-Tab]]

<!-- source: +spot/+console/webapp/spot_console/phasespace.py -->

## What it is for

The lab console's PhaseSpace side: compiling [[StreamData|Hardware-StreamData]], starting and stopping it, the UDP **monitor** that decodes its packets for the 3D view, and the marker calibration file.

## Function by function

**Lines 25–39.** Builds go into `<output>/linux-<arch>/StreamData`; the packet layout constants (19 rigid-body fields; each craft's position and velocity slots); the two reserved ports.

**Lines 42–65.** `read_base_destinations` and `read_led_owner`: read `StreamData.cc` itself to learn the fixed destination list and which rigid body each LED belongs to (from the `assignMarker` calls), so the page's LED colours follow the source.

**Lines 71–101.** WSL detection (version and networking mode) and the Windows home folder, for running on a Windows laptop.

**Lines 104–148.** `detect_linux` (are `g++` and `make` there, with install hints) and `Toolchains`, which runs the detection in the background and publishes `toolchains`.

**Lines 151–200.** `Builder`: *Compile*: refuse without a compiler or while building; in a thread, `make -C phasespace -j N OUT=<folder>` with its output on the `build` log channel; record the outcome and publish `build_status` and `stream_status` (the executable may now exist).

**Lines 206–308.** `Streamer`: *Start Streaming*: the executable must exist; make sure the monitor listens on the monitor port; the command line `StreamData <frequency> <monitor_port> [--udp] [--dest ip …]`; refuse to change the rate while running (a repeated start at the same rate is fine); start it in its own folder (where it reads `tracker_positions_phasespace.txt`) with stdin open; `_pump` sends its output to the `stream` log and records the exit; `command` writes a line to its stdin (`dest add …`, `exp start …`); `stop` sends SIGINT (StreamData closes the PhaseSpace connection cleanly), kills after 5 s, and waits briefly so the reply already says stopped.

**Lines 314–330.** `parse_packet`: the floats of a packet → each craft's x, y [m], attitude, velocities and `tracked` (not all zero), and the visible LEDs as `[id, x, y, z]` in metres.

**Lines 333–418.** `Monitor`: a UDP socket on `127.0.0.1:<monitor_port>` with a 1 MB buffer; `_run` receives each packet, parses it, keeps the newest frame with a count, the rate over the last second and the arrival time, and calls each of its `listeners` with the frame, in the receive thread (an error in one is ignored). `add_listener` and `remove_listener` add and remove one; a calibration ([[calibration.py|Console-calibration]]) uses this to record every PhaseSpace frame, not only the ones the pages get.

**Lines 421–433.** `broadcast_loop` (a thread started by the server): the newest frame to the pages at up to 60 Hz as `frame` events, and a `monitor` status every 2 s.

**Lines 442–457.** `parse_frequency` (1–960 whole Hz) and `stream_summary` (for scripts: `GET /api/stream`).

**Lines 460–493.** The marker calibration: the file StreamData reads from its folder; `save_tracker_positions` checks every line is `tracker_id_…_pos_string,x,y,z`, writes it only if it changed, and notes that a change takes effect at the next start.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/phasespace.py</b> (493 lines; click to show)</summary>

```python
   1  """PhaseSpace streaming on the lab console: compiling StreamData, starting and stopping it, the
   2  UDP monitor that decodes its packets for the 3D view, and the marker calibration file.
   3  
   4  Part of the SPOT Console (webapp/app.py starts it).
   5  """
   6  
   7  import collections
   8  import math
   9  import os
  10  import platform
  11  import re
  12  import shutil
  13  import signal
  14  import socket
  15  import struct
  16  import subprocess
  17  import threading
  18  import time
  19  from pathlib import Path
  20  
  21  from .common import ApiError, PHASESPACE_DIR, decode, finite, quote_cmd
  22  from .events import hub, run_logged, run_quiet, settings
  23  
  24  
  25  PLATFORM_DIR = "linux-" + platform.machine()  # builds go into <output folder>/linux-x86_64, linux-aarch64, ...
  26  
  27  
  28  EXE_NAME = "StreamData"
  29  
  30  # UDP packet layout, see StreamData.cc. Positions arrive in mm and are sent to the browser in m.
  31  
  32  
  33  NUM_RIGID_FIELDS = 19
  34  
  35  
  36  CRAFTS = (("RED", 1, 10), ("BLACK", 4, 13), ("BLUE", 7, 16))
  37  
  38  
  39  RESERVED_PORTS = (31534, 31535)  # SPOT client port and StreamData's own port
  40  
  41  
  42  def read_base_destinations():
  43      """The fixed client_addresses list in StreamData.cc (always sent to)."""
  44      try:
  45          src = (PHASESPACE_DIR / "StreamData.cc").read_text(encoding="utf-8-sig", errors="replace")
  46      except OSError:
  47          return []
  48      m = re.search(r"client_addresses\s*=\s*\{([^}]*)\}", src)
  49      return re.findall(r'"([0-9.]+)"', m.group(1)) if m else []
  50  
  51  
  52  def read_led_owner():
  53      """Which rigid body each LED id belongs to, read from the assignMarker calls in StreamData.cc."""
  54      try:
  55          src = (PHASESPACE_DIR / "StreamData.cc").read_text(encoding="utf-8-sig", errors="replace")
  56      except OSError:
  57          return {}
  58      owner = {}
  59      for line in src.splitlines():
  60          if line.lstrip().startswith("//"):
  61              continue
  62          m = re.search(r"assignMarker\(\s*tracker_id_(RED|BLACK|BLUE)\s*,\s*(\d+)", line)
  63          if m:
  64              owner[int(m.group(2))] = m.group(1)
  65      return owner
  66  
  67  
  68  # --------------------------------------------------------------------------- compilers
  69  
  70  
  71  def detect_wsl():
  72      """None outside WSL, else {"version": 1 or 2, "networking": "nat", "mirrored", "shared" or "unknown"}."""
  73      try:
  74          release = Path("/proc/sys/kernel/osrelease").read_text().lower()
  75      except OSError:
  76          return None
  77      if "microsoft" not in release:
  78          return None
  79      if "wsl2" not in release and "microsoft-standard" not in release:
  80          return {"version": 1, "networking": "shared"}  # WSL 1 uses Windows' own network stack
  81      mode = "unknown"
  82      if shutil.which("wslinfo"):
  83          rc, out = run_quiet(["wslinfo", "--networking-mode"], timeout=10)
  84          if rc == 0 and out.strip():
  85              mode = out.strip().splitlines()[-1].strip().lower()
  86      return {"version": 2, "networking": mode}
  87  
  88  
  89  WSL_INFO = detect_wsl()
  90  
  91  
  92  def windows_home():
  93      """The Windows user folder as a WSL path (e.g. /mnt/c/Users/name), or None."""
  94      if not WSL_INFO or not shutil.which("cmd.exe"):
  95          return None
  96      rc, out = run_quiet(["cmd.exe", "/c", "echo %USERPROFILE%"], timeout=10, cwd="/mnt/c")
  97      lines = [l.strip() for l in out.splitlines() if l.strip()]
  98      if rc != 0 or not lines or "%" in lines[-1]:
  99          return None
 100      rc, out = run_quiet(["wslpath", "-u", lines[-1]], timeout=10)
 101      return out.strip() if rc == 0 and out.strip() else None
 102  
 103  
 104  def detect_linux():
 105      cxx = shutil.which("g++")
 106      make = shutil.which("make")
 107      missing = [n for n, p in (("g++", cxx), ("make", make)) if not p]
 108      if missing:
 109          how = ("Install with \"sudo apt install g++ make\" in WSL"
 110                 if WSL_INFO else "Install with \"sudo apt install g++ make\" (Ubuntu) or "
 111                 "\"sudo dnf install gcc-c++ make\" (Fedora)")
 112          return {"available": False, "reason": "Missing %s. %s, then click Re-check." % (", ".join(missing), how)}
 113      rc, out = run_quiet([cxx, "--version"])
 114      version = out.splitlines()[0] if rc == 0 and out else "g++"
 115      return {"available": True, "via": "%s (%s)" % (version, platform.machine()), "make": make}
 116  
 117  
 118  class Toolchains:
 119      """Compiler detection runs in the background (starting WSL can take a while)."""
 120  
 121      def __init__(self):
 122          self._lock = threading.Lock()
 123          self._result = None
 124          self._checking = False
 125  
 126      def get(self):
 127          with self._lock:
 128              if self._result is not None:
 129                  return dict(self._result, checking=self._checking)
 130          return {"checking": True}
 131  
 132      def refresh(self):
 133          with self._lock:
 134              if self._checking:
 135                  return
 136              self._checking = True
 137          hub.publish("toolchains", self.get())
 138          threading.Thread(target=self._detect, daemon=True).start()
 139  
 140      def _detect(self):
 141          result = {"linux": detect_linux()}
 142          with self._lock:
 143              self._result = result
 144              self._checking = False
 145          hub.publish("toolchains", self.get())
 146  
 147  
 148  toolchains = Toolchains()
 149  
 150  
 151  class Builder:
 152      def __init__(self):
 153          self._lock = threading.Lock()
 154          self.running = False
 155          self.last = None
 156  
 157      def status(self):
 158          with self._lock:
 159              return {"running": self.running, "last": self.last}
 160  
 161      def start(self):
 162          tc = toolchains.get().get("linux")
 163          if tc is None:
 164              raise ApiError("Still checking which compilers are installed; try again in a moment.")
 165          if not tc["available"]:
 166              raise ApiError(tc["reason"])
 167          out_dir = Path(settings.get("output_dir")) / PLATFORM_DIR
 168          with self._lock:
 169              if self.running:
 170                  raise ApiError("StreamData is already being compiled.")
 171              self.running = True
 172          threading.Thread(target=self._run, args=(tc, out_dir), daemon=True).start()
 173          hub.publish("build_status", self.status())
 174  
 175      def _run(self, tc, out_dir):
 176          t0 = time.time()
 177          hub.log("build", "=== Compiling StreamData into %s ===" % out_dir)
 178          ok = False
 179          try:
 180              out_dir.mkdir(parents=True, exist_ok=True)
 181              ok = self._make_build(tc, out_dir)
 182          except Exception as e:  # report anything unexpected in the log instead of losing it
 183              hub.log("build", "Build error: %s" % e)
 184          exe = out_dir / EXE_NAME
 185          ok = ok and exe.exists()
 186          seconds = round(time.time() - t0, 1)
 187          hub.log("build", "=== %s (%.1f s) ===" % ("Build succeeded: %s" % exe if ok else "Build FAILED", seconds))
 188          with self._lock:
 189              self.running = False
 190              self.last = {"ok": ok, "finished": time.time(), "seconds": seconds, "out_dir": str(out_dir)}
 191          hub.publish("build_status", self.status())
 192          hub.publish("stream_status", streamer.status())  # the executable may now exist
 193  
 194      def _make_build(self, tc, out_dir):
 195          # The Makefile keeps object files in ~/.cache, so only changed files are recompiled
 196          jobs = str(os.cpu_count() or 1)
 197          return run_logged([tc["make"], "-C", PHASESPACE_DIR, "-j", jobs, "OUT=" + str(out_dir)], "build") == 0
 198  
 199  
 200  builder = Builder()
 201  
 202  
 203  # --------------------------------------------------------------------------- streaming
 204  
 205  
 206  class Streamer:
 207      def __init__(self):
 208          self._lock = threading.Lock()
 209          self.proc = None
 210          self.started = None
 211          self.frequency = None
 212          self.last_exit = None
 213          self.started_by = None   # who asked: "<ip> (webapp)" or "<ip>" for a remote command
 214          self.stopped = None
 215          self.stopped_by = None
 216  
 217      def exe_path(self):
 218          return Path(settings.get("output_dir")) / PLATFORM_DIR / EXE_NAME
 219  
 220      def status(self):
 221          exe = self.exe_path()
 222          with self._lock:
 223              return {"running": self.proc is not None, "pid": self.proc.pid if self.proc else None,
 224                      "started": self.started, "frequency": self.frequency, "last_exit": self.last_exit,
 225                      "started_by": self.started_by, "stopped": self.stopped, "stopped_by": self.stopped_by,
 226                      "exe": str(exe), "exe_exists": exe.exists()}
 227  
 228      def start(self, frequency, requester):
 229          exe = self.exe_path()
 230          if not exe.exists():
 231              raise ApiError("No build found at %s. Compile StreamData first." % exe)
 232          port = settings.get("monitor_port")
 233          monitor.ensure(port)
 234          cmd = [str(exe), str(frequency), str(port)] + (["--udp"] if settings.get("transport") == "udp" else [])
 235          for ip in settings.get("extra_destinations"):
 236              cmd += ["--dest", ip]
 237          with self._lock:
 238              if self.proc is not None:
 239                  if self.frequency == frequency:
 240                      return  # already streaming at this rate, so a repeated start is fine
 241                  raise ApiError("Already streaming at %s Hz; stop it first to change to %s Hz." % (self.frequency, frequency))
 242              hub.log("stream", "=== Start requested by %s ===" % requester)
 243              hub.log("stream", "=== Starting: %s (in %s) ===" % (quote_cmd(cmd), exe.parent))
 244              try:
 245                  # Runs in its own folder, so tracker_positions_phasespace.txt is read from there
 246                  # stdin stays open so destinations can be added/removed while streaming
 247                  proc = subprocess.Popen(cmd, cwd=str(exe.parent), stdout=subprocess.PIPE,
 248                                          stderr=subprocess.STDOUT, stdin=subprocess.PIPE)
 249              except OSError as e:
 250                  raise ApiError("Could not start %s: %s" % (exe, e))
 251              self.proc, self.started, self.frequency, self.last_exit = proc, time.time(), frequency, None
 252              self.started_by, self.stopped, self.stopped_by = requester, None, None
 253          threading.Thread(target=self._pump, args=(proc,), daemon=True).start()
 254          hub.publish("stream_status", self.status())
 255  
 256      def command(self, line):
 257          """Send a command line to the running StreamData (e.g. "dest add 192.168.1.43")."""
 258          with self._lock:
 259              proc = self.proc
 260              if proc is None:
 261                  return
 262              try:
 263                  proc.stdin.write((line + "\n").encode())
 264                  proc.stdin.flush()
 265              except OSError:
 266                  pass  # it is exiting; the next start uses the saved list anyway
 267  
 268      def _pump(self, proc):
 269          for raw in proc.stdout:
 270              hub.log("stream", decode(raw))
 271          rc = proc.wait()
 272          try:
 273              proc.stdin.close()
 274          except OSError:
 275              pass
 276          with self._lock:
 277              if self.proc is proc:
 278                  self.proc = None
 279                  self.last_exit = rc
 280          hub.log("stream", "=== StreamData exited (code %s) ===" % rc)
 281          hub.publish("stream_status", self.status())
 282  
 283      def stop(self, requester):
 284          with self._lock:
 285              proc = self.proc
 286              if proc is not None:
 287                  self.stopped, self.stopped_by = time.time(), requester
 288          if proc is None:
 289              return
 290          hub.log("stream", "=== Stop requested by %s ===" % requester)
 291          try:
 292              proc.send_signal(signal.SIGINT)  # StreamData closes the PhaseSpace connection cleanly
 293          except OSError:
 294              pass
 295          try:
 296              proc.wait(timeout=5)
 297          except subprocess.TimeoutExpired:
 298              hub.log("stream", "StreamData did not stop within 5 s; killing it.")
 299              proc.kill()
 300          # Wait (briefly) for the output thread to record the exit, so the reply already says "stopped"
 301          for _ in range(50):
 302              with self._lock:
 303                  if self.proc is not proc:
 304                      break
 305              time.sleep(0.02)
 306  
 307  
 308  streamer = Streamer()
 309  
 310  
 311  # --------------------------------------------------------------------------- UDP monitor
 312  
 313  
 314  def parse_packet(data):
 315      n = len(data) // 4
 316      if n < NUM_RIGID_FIELDS:
 317          return None
 318      p = struct.unpack("<%df" % n, data[:4 * n])
 319      crafts = {}
 320      for name, pos, vel in CRAFTS:
 321          x, y, att = p[pos:pos + 3]
 322          crafts[name] = {
 323              "tracked": not (x == 0 and y == 0 and att == 0),  # all zero until first seen
 324              "x": finite(x / 1000), "y": finite(y / 1000), "att": finite(att),
 325              "vx": finite(p[vel] / 1000), "vy": finite(p[vel + 1] / 1000), "w": finite(p[vel + 2]),
 326          }
 327      leds = [[k, p[i] / 1000, p[i + 1] / 1000, p[i + 2] / 1000]
 328              for k, i in enumerate(range(NUM_RIGID_FIELDS, n - 2, 3))
 329              if all(math.isfinite(v) for v in p[i:i + 3])]
 330      return {"t": finite(p[0]), "crafts": crafts, "leds": leds, "values": n}
 331  
 332  
 333  class Monitor:
 334      """Receives StreamData's local copy of each packet on 127.0.0.1:<monitor_port>."""
 335  
 336      def __init__(self):
 337          self._lock = threading.Lock()
 338          self._sock = None
 339          self._stop = None
 340          self.port = None
 341          self.error = None
 342          self.latest = None
 343          self.seq = 0
 344          self.count = 0
 345          self.rate = 0
 346          self.last_time = None
 347          self.sender = None
 348          self.listeners = []   # called with every frame, in this thread (keep them short): calibration.py
 349  
 350      def add_listener(self, fn):
 351          with self._lock:
 352              self.listeners = self.listeners + [fn]
 353  
 354      def remove_listener(self, fn):
 355          with self._lock:
 356              self.listeners = [f for f in self.listeners if f is not fn]
 357  
 358      def status(self):
 359          with self._lock:
 360              return {"port": self.port, "listening": self._sock is not None, "error": self.error,
 361                      "count": self.count, "rate": self.rate, "last_time": self.last_time}
 362  
 363      def ensure(self, port):
 364          with self._lock:
 365              if port == self.port and self._sock is not None:
 366                  return
 367              if self._stop:
 368                  self._stop.set()
 369              self._sock, self._stop, self.port, self.error = None, None, port, None
 370              sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 371              # Room for ~2 s of packets at 960 Hz, in case this thread is briefly busy
 372              sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1 << 20)
 373              try:
 374                  sock.bind(("127.0.0.1", port))
 375              except OSError as e:
 376                  sock.close()
 377                  self.error = "Cannot listen on UDP port %d (%s). Is another program (or another copy of " \
 378                               "this app) using it?" % (port, e)
 379              else:
 380                  sock.settimeout(0.5)
 381                  self._sock, self._stop = sock, threading.Event()
 382                  threading.Thread(target=self._run, args=(sock, self._stop), daemon=True).start()
 383          hub.publish("monitor", self.status())
 384  
 385      def _run(self, sock, stop):
 386          arrivals = collections.deque()
 387          while not stop.is_set():
 388              try:
 389                  data, addr = sock.recvfrom(65535)
 390              except socket.timeout:
 391                  continue
 392              except OSError:
 393                  break
 394              frame = parse_packet(data)
 395              if frame is None:
 396                  continue
 397              now = time.time()
 398              arrivals.append(now)
 399              while arrivals and now - arrivals[0] > 1.0:
 400                  arrivals.popleft()
 401              with self._lock:
 402                  self.count += 1
 403                  self.rate = len(arrivals)
 404                  self.last_time = now
 405                  self.sender = addr[0]
 406                  frame.update(count=self.count, rate=self.rate, received=now)
 407                  self.latest = frame
 408                  self.seq += 1
 409                  listeners = self.listeners
 410              for fn in listeners:
 411                  try:
 412                      fn(frame)
 413                  except Exception:
 414                      pass
 415          sock.close()
 416  
 417  
 418  monitor = Monitor()
 419  
 420  
 421  def broadcast_loop():
 422      """Send the newest frame to the browsers at up to 60 Hz, and a status heartbeat every 2 s."""
 423      last_seq, last_status = -1, 0.0
 424      while True:
 425          time.sleep(1 / 60)
 426          with monitor._lock:
 427              seq, frame = monitor.seq, monitor.latest
 428          if seq != last_seq and frame is not None:
 429              last_seq = seq
 430              hub.publish("frame", frame)
 431          if time.time() - last_status > 2:
 432              last_status = time.time()
 433              hub.publish("monitor", monitor.status())
 434  
 435  
 436  # Each platform's Teensy (teensy_pwm_control.ino) takes 37-byte packets: 0xAA, then the 8 thruster
 437  # duty cycles (0-100 %) and the pucks (on if >= 0.5) as little-endian float32. It switches
 438  # everything off when no packet has arrived for 2 s, so whatever is on is re-sent every
 439  # HW_SEND_PERIOD; if this app or the network stops, the platform shuts down by itself.
 440  
 441  
 442  def parse_frequency(value):
 443      try:
 444          f = int(value)
 445      except (TypeError, ValueError):
 446          raise ApiError("Frequency must be a whole number of Hz.")
 447      if not 1 <= f <= 960:
 448          raise ApiError("Frequency must be between 1 and 960 Hz.")
 449      return f
 450  
 451  
 452  def stream_summary():
 453      """Short status for scripts (GET /api/stream): is it streaming, and are packets arriving?"""
 454      m = monitor.status()
 455      age = time.time() - m["last_time"] if m["last_time"] else None
 456      return dict(streamer.status(), packets_per_second=m["rate"],
 457                  seconds_since_last_packet=round(age, 2) if age is not None else None)
 458  
 459  
 460  TRACKER_FILE = "tracker_positions_phasespace.txt"  # marker calibration StreamData reads at start
 461  
 462  
 463  TRACKER_LINE = re.compile(r"^(tracker_id_\w+_pos_string),\s*([-+0-9.eE]+),\s*([-+0-9.eE]+),\s*([-+0-9.eE]+)$")
 464  
 465  
 466  def tracker_positions_path():
 467      """StreamData reads the calibration from its working folder, which is its own folder."""
 468      return streamer.exe_path().parent / TRACKER_FILE
 469  
 470  
 471  def save_tracker_positions(text, requester):
 472      """Store a marker calibration file (sent by the SPOT GUI before it starts streaming)."""
 473      if not isinstance(text, str) or not text.strip():
 474          raise ApiError("Send the calibration file's contents as \"text\".")
 475      lines = [l.strip() for l in text.replace("\r", "").split("\n") if l.strip()]
 476      for line in lines:
 477          m = TRACKER_LINE.match(line)
 478          if not m:
 479              raise ApiError("Not a marker calibration line: %r (expected tracker_id_..._pos_string,x,y,z)." % line)
 480          try:
 481              [float(v) for v in m.groups()[1:]]
 482          except ValueError:
 483              raise ApiError("Bad number in calibration line: %r" % line)
 484      path = tracker_positions_path()
 485      new = "\n".join(lines) + "\n"
 486      old = path.read_text(encoding="utf-8") if path.exists() else None
 487      if new != old:
 488          path.parent.mkdir(parents=True, exist_ok=True)
 489          path.write_text(new, encoding="utf-8")
 490          hub.log("stream", "=== Marker calibration updated by %s (%d markers) ===" % (requester, len(lines)))
 491      running = streamer.status()["running"]
 492      return {"path": str(path), "markers": len(lines), "changed": new != old,
 493              "note": "takes effect the next time streaming starts" if (running and new != old) else None}
```

</details>
<!-- /listing -->
