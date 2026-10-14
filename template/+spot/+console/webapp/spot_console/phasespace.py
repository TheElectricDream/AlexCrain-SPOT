"""PhaseSpace streaming on the lab console: compiling StreamData, starting and stopping it, the
UDP monitor that decodes its packets for the 3D view, and the marker calibration file.

Part of the SPOT Console (webapp/app.py starts it).
"""

import collections
import math
import os
import platform
import re
import shutil
import signal
import socket
import struct
import subprocess
import threading
import time
from pathlib import Path

from .common import ApiError, PHASESPACE_DIR, decode, finite, quote_cmd
from .events import hub, run_logged, run_quiet, settings


PLATFORM_DIR = "linux-" + platform.machine()  # builds go into <output folder>/linux-x86_64, linux-aarch64, ...


EXE_NAME = "StreamData"

# UDP packet layout, see StreamData.cc. Positions arrive in mm and are sent to the browser in m.


NUM_RIGID_FIELDS = 19


CRAFTS = (("RED", 1, 10), ("BLACK", 4, 13), ("BLUE", 7, 16))


RESERVED_PORTS = (31534, 31535)  # SPOT client port and StreamData's own port


def read_base_destinations():
    """The fixed client_addresses list in StreamData.cc (always sent to)."""
    try:
        src = (PHASESPACE_DIR / "StreamData.cc").read_text(encoding="utf-8-sig", errors="replace")
    except OSError:
        return []
    m = re.search(r"client_addresses\s*=\s*\{([^}]*)\}", src)
    return re.findall(r'"([0-9.]+)"', m.group(1)) if m else []


def read_led_owner():
    """Which rigid body each LED id belongs to, read from the assignMarker calls in StreamData.cc."""
    try:
        src = (PHASESPACE_DIR / "StreamData.cc").read_text(encoding="utf-8-sig", errors="replace")
    except OSError:
        return {}
    owner = {}
    for line in src.splitlines():
        if line.lstrip().startswith("//"):
            continue
        m = re.search(r"assignMarker\(\s*tracker_id_(RED|BLACK|BLUE)\s*,\s*(\d+)", line)
        if m:
            owner[int(m.group(2))] = m.group(1)
    return owner


# --------------------------------------------------------------------------- compilers


def detect_wsl():
    """None outside WSL, else {"version": 1 or 2, "networking": "nat", "mirrored", "shared" or "unknown"}."""
    try:
        release = Path("/proc/sys/kernel/osrelease").read_text().lower()
    except OSError:
        return None
    if "microsoft" not in release:
        return None
    if "wsl2" not in release and "microsoft-standard" not in release:
        return {"version": 1, "networking": "shared"}  # WSL 1 uses Windows' own network stack
    mode = "unknown"
    if shutil.which("wslinfo"):
        rc, out = run_quiet(["wslinfo", "--networking-mode"], timeout=10)
        if rc == 0 and out.strip():
            mode = out.strip().splitlines()[-1].strip().lower()
    return {"version": 2, "networking": mode}


WSL_INFO = detect_wsl()


def windows_home():
    """The Windows user folder as a WSL path (e.g. /mnt/c/Users/name), or None."""
    if not WSL_INFO or not shutil.which("cmd.exe"):
        return None
    rc, out = run_quiet(["cmd.exe", "/c", "echo %USERPROFILE%"], timeout=10, cwd="/mnt/c")
    lines = [l.strip() for l in out.splitlines() if l.strip()]
    if rc != 0 or not lines or "%" in lines[-1]:
        return None
    rc, out = run_quiet(["wslpath", "-u", lines[-1]], timeout=10)
    return out.strip() if rc == 0 and out.strip() else None


def detect_linux():
    cxx = shutil.which("g++")
    make = shutil.which("make")
    missing = [n for n, p in (("g++", cxx), ("make", make)) if not p]
    if missing:
        how = ("Install with \"sudo apt install g++ make\" in WSL"
               if WSL_INFO else "Install with \"sudo apt install g++ make\" (Ubuntu) or "
               "\"sudo dnf install gcc-c++ make\" (Fedora)")
        return {"available": False, "reason": "Missing %s. %s, then click Re-check." % (", ".join(missing), how)}
    rc, out = run_quiet([cxx, "--version"])
    version = out.splitlines()[0] if rc == 0 and out else "g++"
    return {"available": True, "via": "%s (%s)" % (version, platform.machine()), "make": make}


class Toolchains:
    """Compiler detection runs in the background (starting WSL can take a while)."""

    def __init__(self):
        self._lock = threading.Lock()
        self._result = None
        self._checking = False

    def get(self):
        with self._lock:
            if self._result is not None:
                return dict(self._result, checking=self._checking)
        return {"checking": True}

    def refresh(self):
        with self._lock:
            if self._checking:
                return
            self._checking = True
        hub.publish("toolchains", self.get())
        threading.Thread(target=self._detect, daemon=True).start()

    def _detect(self):
        result = {"linux": detect_linux()}
        with self._lock:
            self._result = result
            self._checking = False
        hub.publish("toolchains", self.get())


toolchains = Toolchains()


class Builder:
    def __init__(self):
        self._lock = threading.Lock()
        self.running = False
        self.last = None

    def status(self):
        with self._lock:
            return {"running": self.running, "last": self.last}

    def start(self):
        tc = toolchains.get().get("linux")
        if tc is None:
            raise ApiError("Still checking which compilers are installed; try again in a moment.")
        if not tc["available"]:
            raise ApiError(tc["reason"])
        out_dir = Path(settings.get("output_dir")) / PLATFORM_DIR
        with self._lock:
            if self.running:
                raise ApiError("StreamData is already being compiled.")
            self.running = True
        threading.Thread(target=self._run, args=(tc, out_dir), daemon=True).start()
        hub.publish("build_status", self.status())

    def _run(self, tc, out_dir):
        t0 = time.time()
        hub.log("build", "=== Compiling StreamData into %s ===" % out_dir)
        ok = False
        try:
            out_dir.mkdir(parents=True, exist_ok=True)
            ok = self._make_build(tc, out_dir)
        except Exception as e:  # report anything unexpected in the log instead of losing it
            hub.log("build", "Build error: %s" % e)
        exe = out_dir / EXE_NAME
        ok = ok and exe.exists()
        seconds = round(time.time() - t0, 1)
        hub.log("build", "=== %s (%.1f s) ===" % ("Build succeeded: %s" % exe if ok else "Build FAILED", seconds))
        with self._lock:
            self.running = False
            self.last = {"ok": ok, "finished": time.time(), "seconds": seconds, "out_dir": str(out_dir)}
        hub.publish("build_status", self.status())
        hub.publish("stream_status", streamer.status())  # the executable may now exist

    def _make_build(self, tc, out_dir):
        # The Makefile keeps object files in ~/.cache, so only changed files are recompiled
        jobs = str(os.cpu_count() or 1)
        return run_logged([tc["make"], "-C", PHASESPACE_DIR, "-j", jobs, "OUT=" + str(out_dir)], "build") == 0


builder = Builder()


# --------------------------------------------------------------------------- streaming


class Streamer:
    def __init__(self):
        self._lock = threading.Lock()
        self.proc = None
        self.started = None
        self.frequency = None
        self.last_exit = None
        self.started_by = None   # who asked: "<ip> (webapp)" or "<ip>" for a remote command
        self.stopped = None
        self.stopped_by = None

    def exe_path(self):
        return Path(settings.get("output_dir")) / PLATFORM_DIR / EXE_NAME

    def status(self):
        exe = self.exe_path()
        with self._lock:
            return {"running": self.proc is not None, "pid": self.proc.pid if self.proc else None,
                    "started": self.started, "frequency": self.frequency, "last_exit": self.last_exit,
                    "started_by": self.started_by, "stopped": self.stopped, "stopped_by": self.stopped_by,
                    "exe": str(exe), "exe_exists": exe.exists()}

    def start(self, frequency, requester):
        exe = self.exe_path()
        if not exe.exists():
            raise ApiError("No build found at %s. Compile StreamData first." % exe)
        port = settings.get("monitor_port")
        monitor.ensure(port)
        cmd = [str(exe), str(frequency), str(port)] + (["--udp"] if settings.get("transport") == "udp" else [])
        for ip in settings.get("extra_destinations"):
            cmd += ["--dest", ip]
        with self._lock:
            if self.proc is not None:
                if self.frequency == frequency:
                    return  # already streaming at this rate, so a repeated start is fine
                raise ApiError("Already streaming at %s Hz; stop it first to change to %s Hz." % (self.frequency, frequency))
            hub.log("stream", "=== Start requested by %s ===" % requester)
            hub.log("stream", "=== Starting: %s (in %s) ===" % (quote_cmd(cmd), exe.parent))
            try:
                # Runs in its own folder, so tracker_positions_phasespace.txt is read from there
                # stdin stays open so destinations can be added/removed while streaming
                proc = subprocess.Popen(cmd, cwd=str(exe.parent), stdout=subprocess.PIPE,
                                        stderr=subprocess.STDOUT, stdin=subprocess.PIPE)
            except OSError as e:
                raise ApiError("Could not start %s: %s" % (exe, e))
            self.proc, self.started, self.frequency, self.last_exit = proc, time.time(), frequency, None
            self.started_by, self.stopped, self.stopped_by = requester, None, None
        threading.Thread(target=self._pump, args=(proc,), daemon=True).start()
        hub.publish("stream_status", self.status())

    def command(self, line):
        """Send a command line to the running StreamData (e.g. "dest add 192.168.1.43")."""
        with self._lock:
            proc = self.proc
            if proc is None:
                return
            try:
                proc.stdin.write((line + "\n").encode())
                proc.stdin.flush()
            except OSError:
                pass  # it is exiting; the next start uses the saved list anyway

    def _pump(self, proc):
        for raw in proc.stdout:
            hub.log("stream", decode(raw))
        rc = proc.wait()
        try:
            proc.stdin.close()
        except OSError:
            pass
        with self._lock:
            if self.proc is proc:
                self.proc = None
                self.last_exit = rc
        hub.log("stream", "=== StreamData exited (code %s) ===" % rc)
        hub.publish("stream_status", self.status())

    def stop(self, requester):
        with self._lock:
            proc = self.proc
            if proc is not None:
                self.stopped, self.stopped_by = time.time(), requester
        if proc is None:
            return
        hub.log("stream", "=== Stop requested by %s ===" % requester)
        try:
            proc.send_signal(signal.SIGINT)  # StreamData closes the PhaseSpace connection cleanly
        except OSError:
            pass
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            hub.log("stream", "StreamData did not stop within 5 s; killing it.")
            proc.kill()
        # Wait (briefly) for the output thread to record the exit, so the reply already says "stopped"
        for _ in range(50):
            with self._lock:
                if self.proc is not proc:
                    break
            time.sleep(0.02)


streamer = Streamer()


# --------------------------------------------------------------------------- UDP monitor


def parse_packet(data):
    n = len(data) // 4
    if n < NUM_RIGID_FIELDS:
        return None
    p = struct.unpack("<%df" % n, data[:4 * n])
    crafts = {}
    for name, pos, vel in CRAFTS:
        x, y, att = p[pos:pos + 3]
        crafts[name] = {
            "tracked": not (x == 0 and y == 0 and att == 0),  # all zero until first seen
            "x": finite(x / 1000), "y": finite(y / 1000), "att": finite(att),
            "vx": finite(p[vel] / 1000), "vy": finite(p[vel + 1] / 1000), "w": finite(p[vel + 2]),
        }
    leds = [[k, p[i] / 1000, p[i + 1] / 1000, p[i + 2] / 1000]
            for k, i in enumerate(range(NUM_RIGID_FIELDS, n - 2, 3))
            if all(math.isfinite(v) for v in p[i:i + 3])]
    return {"t": finite(p[0]), "crafts": crafts, "leds": leds, "values": n}


class Monitor:
    """Receives StreamData's local copy of each packet on 127.0.0.1:<monitor_port>."""

    def __init__(self):
        self._lock = threading.Lock()
        self._sock = None
        self._stop = None
        self.port = None
        self.error = None
        self.latest = None
        self.seq = 0
        self.count = 0
        self.rate = 0
        self.last_time = None
        self.sender = None
        self.listeners = []   # called with every frame, in this thread (keep them short): calibration.py

    def add_listener(self, fn):
        with self._lock:
            self.listeners = self.listeners + [fn]

    def remove_listener(self, fn):
        with self._lock:
            self.listeners = [f for f in self.listeners if f is not fn]

    def status(self):
        with self._lock:
            return {"port": self.port, "listening": self._sock is not None, "error": self.error,
                    "count": self.count, "rate": self.rate, "last_time": self.last_time}

    def ensure(self, port):
        with self._lock:
            if port == self.port and self._sock is not None:
                return
            if self._stop:
                self._stop.set()
            self._sock, self._stop, self.port, self.error = None, None, port, None
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            # Room for ~2 s of packets at 960 Hz, in case this thread is briefly busy
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1 << 20)
            try:
                sock.bind(("127.0.0.1", port))
            except OSError as e:
                sock.close()
                self.error = "Cannot listen on UDP port %d (%s). Is another program (or another copy of " \
                             "this app) using it?" % (port, e)
            else:
                sock.settimeout(0.5)
                self._sock, self._stop = sock, threading.Event()
                threading.Thread(target=self._run, args=(sock, self._stop), daemon=True).start()
        hub.publish("monitor", self.status())

    def _run(self, sock, stop):
        arrivals = collections.deque()
        while not stop.is_set():
            try:
                data, addr = sock.recvfrom(65535)
            except socket.timeout:
                continue
            except OSError:
                break
            frame = parse_packet(data)
            if frame is None:
                continue
            now = time.time()
            arrivals.append(now)
            while arrivals and now - arrivals[0] > 1.0:
                arrivals.popleft()
            with self._lock:
                self.count += 1
                self.rate = len(arrivals)
                self.last_time = now
                self.sender = addr[0]
                frame.update(count=self.count, rate=self.rate, received=now)
                self.latest = frame
                self.seq += 1
                listeners = self.listeners
            for fn in listeners:
                try:
                    fn(frame)
                except Exception:
                    pass
        sock.close()


monitor = Monitor()


def broadcast_loop():
    """Send the newest frame to the browsers at up to 60 Hz, and a status heartbeat every 2 s."""
    last_seq, last_status = -1, 0.0
    while True:
        time.sleep(1 / 60)
        with monitor._lock:
            seq, frame = monitor.seq, monitor.latest
        if seq != last_seq and frame is not None:
            last_seq = seq
            hub.publish("frame", frame)
        if time.time() - last_status > 2:
            last_status = time.time()
            hub.publish("monitor", monitor.status())


# Each platform's Teensy (teensy_pwm_control.ino) takes 37-byte packets: 0xAA, then the 8 thruster
# duty cycles (0-100 %) and the pucks (on if >= 0.5) as little-endian float32. It switches
# everything off when no packet has arrived for 2 s, so whatever is on is re-sent every
# HW_SEND_PERIOD; if this app or the network stops, the platform shuts down by itself.


def parse_frequency(value):
    try:
        f = int(value)
    except (TypeError, ValueError):
        raise ApiError("Frequency must be a whole number of Hz.")
    if not 1 <= f <= 960:
        raise ApiError("Frequency must be between 1 and 960 Hz.")
    return f


def stream_summary():
    """Short status for scripts (GET /api/stream): is it streaming, and are packets arriving?"""
    m = monitor.status()
    age = time.time() - m["last_time"] if m["last_time"] else None
    return dict(streamer.status(), packets_per_second=m["rate"],
                seconds_since_last_packet=round(age, 2) if age is not None else None)


TRACKER_FILE = "tracker_positions_phasespace.txt"  # marker calibration StreamData reads at start


TRACKER_LINE = re.compile(r"^(tracker_id_\w+_pos_string),\s*([-+0-9.eE]+),\s*([-+0-9.eE]+),\s*([-+0-9.eE]+)$")


def tracker_positions_path():
    """StreamData reads the calibration from its working folder, which is its own folder."""
    return streamer.exe_path().parent / TRACKER_FILE


def save_tracker_positions(text, requester):
    """Store a marker calibration file (sent by the SPOT GUI before it starts streaming)."""
    if not isinstance(text, str) or not text.strip():
        raise ApiError("Send the calibration file's contents as \"text\".")
    lines = [l.strip() for l in text.replace("\r", "").split("\n") if l.strip()]
    for line in lines:
        m = TRACKER_LINE.match(line)
        if not m:
            raise ApiError("Not a marker calibration line: %r (expected tracker_id_..._pos_string,x,y,z)." % line)
        try:
            [float(v) for v in m.groups()[1:]]
        except ValueError:
            raise ApiError("Bad number in calibration line: %r" % line)
    path = tracker_positions_path()
    new = "\n".join(lines) + "\n"
    old = path.read_text(encoding="utf-8") if path.exists() else None
    if new != old:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(new, encoding="utf-8")
        hub.log("stream", "=== Marker calibration updated by %s (%d markers) ===" % (requester, len(lines)))
    running = streamer.status()["running"]
    return {"path": str(path), "markers": len(lines), "changed": new != old,
            "note": "takes effect the next time streaming starts" if (running and new != old) else None}
