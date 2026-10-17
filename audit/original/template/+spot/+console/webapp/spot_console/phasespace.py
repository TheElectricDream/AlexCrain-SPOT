"""PhaseSpace streaming on the lab console.

Compiling StreamData, starting and stopping it, the UDP monitor that
decodes its packets for the 3D view, and the marker calibration file.

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
from .constants import C
from .events import hub, run_logged, run_quiet, settings


# StreamData and its packets

# Builds go into <output folder>/linux-x86_64, linux-aarch64, ...
PLATFORM_DIR = "linux-" + platform.machine()

EXE_NAME = "StreamData"

# UDP packet layout (Constants.m's PhaseSpace packet section, see
# StreamData.cc). Positions arrive in mm and are sent to the browser in
# m.
NUM_RIGID_FIELDS = C.int("PACKET_RIGID_FIELDS")
MM_PER_M = 1000

# (platform, index of its x, index of its vx) in a packet, 0-based.
CRAFTS = tuple((p, C.index("PACKET_POSE_" + p), C.index("PACKET_RATE_" + p))
               for p in C.PLATFORMS)

# SPOT client port and StreamData's own port.
RESERVED_PORTS = (C.int("PORT_PHASESPACE_CLIENT"), C.int("PORT_STREAMDATA"))

MAX_FREQUENCY = C.int("MAX_DATA_RATE")   # [Hz] PhaseSpace's fastest

TOOL_TIMEOUT = 10          # [s] for wslinfo, cmd.exe and wslpath
STOP_TIMEOUT = 5           # [s] for StreamData to exit after Ctrl-C
RECEIVE_BUFFER = 1 << 20   # [bytes] about 2 s of packets at MAX_FREQUENCY
BROADCAST_RATE = 60        # [Hz] frames to the browsers, at most
STATUS_PERIOD = 2          # [s] monitor status heartbeat


def read_base_destinations():
    """Return StreamData.cc's fixed client_addresses list."""
    try:
        src = (PHASESPACE_DIR / "StreamData.cc").read_text(encoding="utf-8-sig", errors="replace")
    except OSError:
        return []
    m = re.search(r"client_addresses\s*=\s*\{([^}]*)\}", src)
    return re.findall(r'"([0-9.]+)"', m.group(1)) if m else []


def read_led_owner():
    """Return which rigid body each LED id belongs to.

    Read from the assignMarker calls in StreamData.cc (commented-out
    lines skipped).

    Returns:
        {LED id: "RED", "BLACK" or "BLUE"}.
    """
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


# Compilers


def detect_wsl():
    """Return whether this is WSL, and which kind.

    Returns:
        None outside WSL, else {"version": 1 or 2, "networking":
        "nat", "mirrored", "shared" or "unknown"}.
    """
    try:
        release = Path("/proc/sys/kernel/osrelease").read_text().lower()
    except OSError:
        return None
    if "microsoft" not in release:
        return None

    # WSL 1 uses Windows' own network stack.
    if "wsl2" not in release and "microsoft-standard" not in release:
        return {"version": 1, "networking": "shared"}

    # WSL 2: its networking mode, if wslinfo can tell.
    mode = "unknown"
    if shutil.which("wslinfo"):
        rc, out = run_quiet(["wslinfo", "--networking-mode"], timeout=TOOL_TIMEOUT)
        if rc == 0 and out.strip():
            mode = out.strip().splitlines()[-1].strip().lower()
    return {"version": 2, "networking": mode}


WSL_INFO = detect_wsl()


def windows_home():
    """Return the Windows user folder as a WSL path, or None.

    For example /mnt/c/Users/name.
    """
    if not WSL_INFO or not shutil.which("cmd.exe"):
        return None

    # %USERPROFILE% from Windows, then converted by wslpath.
    rc, out = run_quiet(["cmd.exe", "/c", "echo %USERPROFILE%"], timeout=TOOL_TIMEOUT,
                        cwd="/mnt/c")
    lines = [line.strip() for line in out.splitlines() if line.strip()]
    if rc != 0 or not lines or "%" in lines[-1]:
        return None
    rc, out = run_quiet(["wslpath", "-u", lines[-1]], timeout=TOOL_TIMEOUT)
    return out.strip() if rc == 0 and out.strip() else None


def detect_linux():
    """Return whether g++ and make are installed, and their version.

    Returns:
        {"available": True, "via", "make"}, or {"available": False,
        "reason"} with how to install them.
    """
    cxx = shutil.which("g++")
    make = shutil.which("make")
    missing = [n for n, p in (("g++", cxx), ("make", make)) if not p]
    if missing:
        how = ("Install with \"sudo apt install g++ make\" in WSL"
               if WSL_INFO else "Install with \"sudo apt install g++ make\" (Ubuntu) or "
               "\"sudo dnf install gcc-c++ make\" (Fedora)")
        return {"available": False,
                "reason": "Missing %s. %s, then click Re-check." % (", ".join(missing), how)}
    rc, out = run_quiet([cxx, "--version"])
    version = out.splitlines()[0] if rc == 0 and out else "g++"
    return {"available": True, "via": "%s (%s)" % (version, platform.machine()), "make": make}


class Toolchains:
    """Compiler detection, in the background (WSL starts slowly)."""

    def __init__(self):
        """Start with nothing detected yet."""
        self._lock = threading.Lock()
        self._result = None
        self._checking = False

    def get(self):
        """Return the last result, or {"checking": True}."""
        with self._lock:
            if self._result is not None:
                return dict(self._result, checking=self._checking)
        return {"checking": True}

    def refresh(self):
        """Detect the compilers again, unless already doing so."""
        with self._lock:
            if self._checking:
                return
            self._checking = True
        hub.publish("toolchains", self.get())
        threading.Thread(target=self._detect, daemon=True).start()

    def _detect(self):
        """Detect and publish the result."""
        result = {"linux": detect_linux()}
        with self._lock:
            self._result = result
            self._checking = False
        hub.publish("toolchains", self.get())


toolchains = Toolchains()


class Builder:
    """Compiles StreamData with make, one build at a time."""

    def __init__(self):
        """Start with no build."""
        self._lock = threading.Lock()
        self.running = False
        self.last = None

    def status(self):
        """Return whether a build runs, and the last one's result."""
        with self._lock:
            return {"running": self.running, "last": self.last}

    def start(self):
        """Start a build in the background.

        Raises:
            ApiError: if the compilers are still being detected or are
                missing, or a build is already running.
        """
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
        """Build, log the result and publish it."""
        t0 = time.time()
        hub.log("build", "=== Compiling StreamData into %s ===" % out_dir)

        # Report anything unexpected in the log instead of losing it.
        ok = False
        try:
            out_dir.mkdir(parents=True, exist_ok=True)
            ok = self._make_build(tc, out_dir)
        except Exception as e:
            hub.log("build", "Build error: %s" % e)

        # Succeeded only if the executable is there.
        exe = out_dir / EXE_NAME
        ok = ok and exe.exists()
        seconds = round(time.time() - t0, 1)
        hub.log("build", "=== %s (%.1f s) ==="
                % ("Build succeeded: %s" % exe if ok else "Build FAILED", seconds))
        with self._lock:
            self.running = False
            self.last = {"ok": ok, "finished": time.time(), "seconds": seconds,
                         "out_dir": str(out_dir)}
        hub.publish("build_status", self.status())
        hub.publish("stream_status", streamer.status())  # the executable may now exist

    def _make_build(self, tc, out_dir):
        """Run make; return whether it succeeded.

        The Makefile keeps object files in ~/.cache, so only changed
        files are recompiled.
        """
        jobs = str(os.cpu_count() or 1)
        return run_logged([tc["make"], "-C", PHASESPACE_DIR, "-j", jobs, "OUT=" + str(out_dir)],
                          "build") == 0


builder = Builder()


# Streaming


class Streamer:
    """Runs StreamData: start, stop, its output and live commands."""

    def __init__(self):
        """Start with nothing running."""
        self._lock = threading.Lock()
        self.proc = None
        self.started = None
        self.frequency = None
        self.last_exit = None
        self.started_by = None   # who asked: "<ip> (webapp)" or "<ip>" for a remote command
        self.stopped = None
        self.stopped_by = None

    def exe_path(self):
        """Return where the build for this computer is."""
        return Path(settings.get("output_dir")) / PLATFORM_DIR / EXE_NAME

    def status(self):
        """Return whether it streams, since when, how fast, by whom."""
        exe = self.exe_path()
        with self._lock:
            return {"running": self.proc is not None,
                    "pid": self.proc.pid if self.proc else None,
                    "started": self.started, "frequency": self.frequency,
                    "last_exit": self.last_exit, "started_by": self.started_by,
                    "stopped": self.stopped, "stopped_by": self.stopped_by,
                    "exe": str(exe), "exe_exists": exe.exists()}

    def start(self, frequency, requester):
        """Start streaming at a rate.

        A repeated start at the same rate is fine.

        Args:
            frequency: The camera rate [Hz].
            requester: Who asked.

        Raises:
            ApiError: if there is no build, it streams at another rate
                already, or it can't be started.
        """
        exe = self.exe_path()
        if not exe.exists():
            raise ApiError("No build found at %s. Compile StreamData first." % exe)

        # The monitor listens for StreamData's local copy of packets.
        port = settings.get("monitor_port")
        monitor.ensure(port)

        # StreamData <frequency> <monitor port> [--udp] [--dest <ip>]...
        cmd = [str(exe), str(frequency), str(port)] + (
            ["--udp"] if settings.get("transport") == "udp" else [])
        for ip in settings.get("extra_destinations"):
            cmd += ["--dest", ip]

        with self._lock:
            if self.proc is not None:
                if self.frequency == frequency:
                    return
                raise ApiError("Already streaming at %s Hz; stop it first to change to %s Hz."
                               % (self.frequency, frequency))
            hub.log("stream", "=== Start requested by %s ===" % requester)
            hub.log("stream", "=== Starting: %s (in %s) ===" % (quote_cmd(cmd), exe.parent))

            # In its own folder, so tracker_positions_phasespace.txt is
            # read from there; stdin stays open so destinations can be
            # added or removed while streaming.
            try:
                proc = subprocess.Popen(cmd, cwd=str(exe.parent), stdout=subprocess.PIPE,
                                        stderr=subprocess.STDOUT, stdin=subprocess.PIPE)
            except OSError as e:
                raise ApiError("Could not start %s: %s" % (exe, e))
            self.proc, self.started, self.frequency, self.last_exit = (
                proc, time.time(), frequency, None)
            self.started_by, self.stopped, self.stopped_by = requester, None, None
        threading.Thread(target=self._pump, args=(proc,), daemon=True).start()
        hub.publish("stream_status", self.status())

    def command(self, line):
        """Send a command line to the running StreamData.

        For example "dest add 192.168.1.43". Ignored if it isn't
        running, or is exiting (the next start uses the saved list
        anyway).
        """
        with self._lock:
            proc = self.proc
            if proc is None:
                return
            try:
                proc.stdin.write((line + "\n").encode())
                proc.stdin.flush()
            except OSError:
                pass

    def _pump(self, proc):
        """Log StreamData's output until it exits; record the exit."""
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
        """Stop streaming: Ctrl-C, then kill after STOP_TIMEOUT."""
        with self._lock:
            proc = self.proc
            if proc is not None:
                self.stopped, self.stopped_by = time.time(), requester
        if proc is None:
            return
        hub.log("stream", "=== Stop requested by %s ===" % requester)

        # SIGINT: StreamData closes the PhaseSpace connection cleanly.
        try:
            proc.send_signal(signal.SIGINT)
        except OSError:
            pass
        try:
            proc.wait(timeout=STOP_TIMEOUT)
        except subprocess.TimeoutExpired:
            hub.log("stream", "StreamData did not stop within 5 s; killing it.")
            proc.kill()

        # Wait (briefly) for the output thread to record the exit, so
        # the reply already says "stopped".
        for _ in range(50):
            with self._lock:
                if self.proc is not proc:
                    break
            time.sleep(0.02)


streamer = Streamer()


# UDP monitor


def parse_packet(data):
    """Decode one StreamData packet.

    Returns:
        {"t", "crafts": {NAME: {"tracked", "x", "y", "att", "vx", "vy",
        "w"}} [m, rad], "leds": [[id, x, y, z]] [m], "values"}; None
        if the packet is too short.
    """
    n = len(data) // 4
    if n < NUM_RIGID_FIELDS:
        return None
    values = struct.unpack("<%df" % n, data[:4 * n])

    # Each platform; all zero until PhaseSpace first sees it.
    crafts = {}
    for name, pos, vel in CRAFTS:
        x, y, att = values[pos:pos + 3]
        crafts[name] = {
            "tracked": not (x == 0 and y == 0 and att == 0),
            "x": finite(x / MM_PER_M), "y": finite(y / MM_PER_M), "att": finite(att),
            "vx": finite(values[vel] / MM_PER_M), "vy": finite(values[vel + 1] / MM_PER_M),
            "w": finite(values[vel + 2]),
        }

    # The LEDs seen (finite positions), after the rigid-body fields.
    leds = [[k, values[i] / MM_PER_M, values[i + 1] / MM_PER_M, values[i + 2] / MM_PER_M]
            for k, i in enumerate(range(NUM_RIGID_FIELDS, n - 2, 3))
            if all(math.isfinite(v) for v in values[i:i + 3])]
    return {"t": finite(values[0]), "crafts": crafts, "leds": leds, "values": n}


class Monitor:
    """Receives StreamData's local copy of each packet.

    On 127.0.0.1:<monitor_port>.
    """

    def __init__(self):
        """Start without listening."""
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
        # Called with every frame, in this thread (keep them short):
        # calibration.py.
        self.listeners = []

    def add_listener(self, fn):
        """Call fn(frame) for every frame."""
        with self._lock:
            self.listeners = self.listeners + [fn]

    def remove_listener(self, fn):
        """Stop calling fn.

        Compared with != rather than "is not": each obj.method gives a
        new bound-method object, equal to the earlier one.
        """
        with self._lock:
            self.listeners = [f for f in self.listeners if f != fn]

    def status(self):
        """Return the port, whether it listens, and the packet rate."""
        with self._lock:
            return {"port": self.port, "listening": self._sock is not None, "error": self.error,
                    "count": self.count, "rate": self.rate, "last_time": self.last_time}

    def ensure(self, port):
        """Listen on a port (a no-op if already listening there).

        A port that can't be bound is reported in status()["error"].
        """
        with self._lock:
            if port == self.port and self._sock is not None:
                return

            # Stop the old listener.
            if self._stop:
                self._stop.set()
            self._sock, self._stop, self.port, self.error = None, None, port, None

            # Room for ~2 s of packets at MAX_FREQUENCY, in case this
            # thread
            # is briefly busy.
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, RECEIVE_BUFFER)
            try:
                sock.bind(("127.0.0.1", port))
            except OSError as e:
                sock.close()
                self.error = ("Cannot listen on UDP port %d (%s). Is another program (or another "
                              "copy of this app) using it?" % (port, e))
            else:
                sock.settimeout(0.5)
                self._sock, self._stop = sock, threading.Event()
                threading.Thread(target=self._run, args=(sock, self._stop), daemon=True).start()
        hub.publish("monitor", self.status())

    def _run(self, sock, stop):
        """Receive and decode packets until stopped; count the rate."""
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

            # Packets in the last second.
            now = time.time()
            arrivals.append(now)
            while arrivals and now - arrivals[0] > 1.0:
                arrivals.popleft()

            # Keep it as the latest.
            with self._lock:
                self.count += 1
                self.rate = len(arrivals)
                self.last_time = now
                self.sender = addr[0]
                frame.update(count=self.count, rate=self.rate, received=now)
                self.latest = frame
                self.seq += 1
                listeners = self.listeners

            # The listeners; one failing doesn't stop the others.
            for fn in listeners:
                try:
                    fn(frame)
                except Exception:
                    pass
        sock.close()


monitor = Monitor()


def broadcast_loop():
    """Send frames to the browsers forever.

    The newest frame at up to BROADCAST_RATE, and a status heartbeat
    every STATUS_PERIOD.
    """
    last_seq, last_status = -1, 0.0
    while True:
        time.sleep(1 / BROADCAST_RATE)
        with monitor._lock:
            seq, frame = monitor.seq, monitor.latest
        if seq != last_seq and frame is not None:
            last_seq = seq
            hub.publish("frame", frame)
        if time.time() - last_status > STATUS_PERIOD:
            last_status = time.time()
            hub.publish("monitor", monitor.status())


# Requests


def parse_frequency(value):
    """Return a camera rate from a request.

    Raises:
        ApiError: if it isn't a whole number of Hz from 1 to
            MAX_FREQUENCY.
    """
    try:
        frequency = int(value)
    except (TypeError, ValueError):
        raise ApiError("Frequency must be a whole number of Hz.")
    if not 1 <= frequency <= MAX_FREQUENCY:
        raise ApiError("Frequency must be between 1 and %d Hz." % MAX_FREQUENCY)
    return frequency


def stream_summary():
    """Return a short status for scripts (GET /api/stream).

    Whether it is streaming, and whether packets are arriving.
    """
    mon = monitor.status()
    age = time.time() - mon["last_time"] if mon["last_time"] else None
    return dict(streamer.status(), packets_per_second=mon["rate"],
                seconds_since_last_packet=round(age, 2) if age is not None else None)


# Marker calibration

TRACKER_FILE = "tracker_positions_phasespace.txt"  # read by StreamData at start

TRACKER_LINE = re.compile(r"^(tracker_id_\w+_pos_string),\s*([-+0-9.eE]+),\s*([-+0-9.eE]+),"
                          r"\s*([-+0-9.eE]+)$")


def tracker_positions_path():
    """Return where StreamData reads the calibration: its own folder."""
    return streamer.exe_path().parent / TRACKER_FILE


def save_tracker_positions(text, requester):
    """Store a marker calibration file.

    Sent by the SPOT GUI before it starts streaming. Written only if it
    changed; it takes effect the next time streaming starts.

    Args:
        text: The file's contents.
        requester: Who sent it.

    Returns:
        {"path", "markers", "changed", "note"}.

    Raises:
        ApiError: if a line isn't a marker calibration line.
    """
    if not isinstance(text, str) or not text.strip():
        raise ApiError("Send the calibration file's contents as \"text\".")

    # Every line: tracker_id_..._pos_string,x,y,z.
    lines = [line.strip() for line in text.replace("\r", "").split("\n") if line.strip()]
    for line in lines:
        m = TRACKER_LINE.match(line)
        if not m:
            raise ApiError("Not a marker calibration line: %r "
                           "(expected tracker_id_..._pos_string,x,y,z)." % line)
        try:
            [float(v) for v in m.groups()[1:]]
        except ValueError:
            raise ApiError("Bad number in calibration line: %r" % line)

    # Write it if it changed.
    path = tracker_positions_path()
    new = "\n".join(lines) + "\n"
    old = path.read_text(encoding="utf-8") if path.exists() else None
    if new != old:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(new, encoding="utf-8")
        hub.log("stream", "=== Marker calibration updated by %s (%d markers) ==="
                % (requester, len(lines)))
    running = streamer.status()["running"]
    return {"path": str(path), "markers": len(lines), "changed": new != old,
            "note": "takes effect the next time streaming starts"
                    if (running and new != old) else None}
