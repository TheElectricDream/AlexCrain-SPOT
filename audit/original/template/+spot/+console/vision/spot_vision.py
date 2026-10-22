#!/usr/bin/env python3
"""SPOT Vision: the Jetson Orin's cameras, streamed to the Vision tab.

The cameras plugged into the Orin (spot-vision), for the lab console's
Vision tab.

    python3 spot_vision.py [--port 8090] [--realsense-python PATH]
                           [--dvs-python PATH] [--thermal-python PATH]
                           [--recordings FOLDER] [--demo]

    GET  /api/cameras
        the cameras: [{id, kind, name, serial, views: [{id, label}],
        state, detail, model, housing_c}]
    GET  /api/cameras/<camera>/params
        its settings: {"params": [setting, ...], "open", "saved",
        "errors"}
    POST /api/cameras/<camera>/params
        {"values": {"<id>": value}} or {"reset": true}: applied now if
        it is open, and kept (~/.spot_vision_settings.json) for each
        time it opens
    GET  /stream/<camera>/<view>.mjpg
        live MJPEG (?fps=15&q=80&w=960: frame rate, JPEG quality,
        width; 0 = full)
    GET  /snapshot/<camera>/<view>.jpg
        one frame (?q=95&w=0)
    POST /api/record/start  {"session": name, "cameras": [id, ...]}
        record them (all, or none and why: 409) until stopped
    POST /api/record/stop   {"session": name}
        stop; returns its session.json (with "files" and "bytes")
    GET  /api/record
        {"active", "recordings": [...], "disk": {"free", "total"}}
    GET  /api/record/<session>
        its files [{"path", "size"}] and session.json
    GET  /recordings/<session>/<path>
        one of its files
    POST /api/record/delete {"session": name}
    GET  /
        a plain page listing them (for checking by hand)

Experiments record cameras through the lab console (its experiment.py):
each camera's worker writes into RECORD_ROOT/<session>/<camera>/ on
the Orin's disk (nothing goes over the network while it runs), and the
lab console relays the experiment clock of every PhaseSpace packet to
UDP CLOCK_PORT. When the recording stops, the clock is fitted to the
Orin's monotonic clock and every frame log gets experiment_time
(README.txt in the folder says what each file is). The local console
then copies the folder into the run's Saved Data and deletes it here.
A recording stops itself when the disk is nearly full (DISK_STOP).

Each camera runs in a process of its own, vision_worker.py, with the
Python whose library it needs: the ZED SDK's pyzed (this Python); the
RealSense L515's pyrealsense2 2.54, the last librealsense that
supports it, in its own environment (--realsense-python); the
iniVation DVXplorer's dv-processing (--dvs-python); the ICI 9320P
thermal camera's SDK (vendor/ici9320, which needs python-libusb1 and
pyusb: --thermal-python); any other USB camera: OpenCV and v4l2-ctl;
--demo adds a test pattern. A camera's settings are the ones its
library offers (the worker lists them, with their ranges and
defaults).

A camera is opened when its first viewer comes and closed IDLE_CLOSE s
after the last one leaves, so the experiments' own vision code can
have it the rest of the time (a camera another program holds shows as
busy). The page reaches this server directly (the video doesn't go
through the lab console).

Python 3.8+, standard library only; installed by
+spot/+console/deploy/deploy_orin.sh.
"""
import argparse
import glob
import json
import math
import os
import re
import shutil
import socket
import struct
import subprocess
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, urlparse

HERE = os.path.dirname(os.path.abspath(__file__))
WORKER = os.path.join(HERE, "vision_worker.py")
IDLE_CLOSE = 5.0     # [s] a camera with no viewer this long is closed
MAX_FPS = 30

# Each kind of camera's views: (id, label).
VIEWS = {
    "zed": [("left", "Left"), ("right", "Right"), ("side", "Side by Side"), ("depth", "Depth")],
    "realsense": [("color", "Colour"), ("depth", "Depth"), ("infrared", "Infrared")],
    "dvs": [("events", "Events"), ("accumulated", "Accumulated")],
    "thermal": [("thermal", "Thermal")],
    "uvc": [("video", "Video")],
    "demo": [("pattern", "Pattern"), ("noise", "Noise")],
}

# USB vendor ids.
ZED_VENDOR = "2b03"
INTEL_VENDOR = "8086"
INIVATION_VENDOR = "152a"   # iniVation's event cameras (DVXplorer, DAVIS)
ICI_VENDOR = "2aa9"         # the ICI 9320P thermal camera...
ICI_PRODUCTS = ("9f00", "9f10")   # ... at power-up, and with the SDK's firmware

# Outside the deployed folder, so a redeploy keeps it.
SETTINGS_FILE = os.path.expanduser("~/.spot_vision_settings.json")
DEMO = False

# Experiments' recordings: a folder per session (RECORD_ROOT/<session>/
# <camera>/), and the experiment clock the lab console relays.
RECORD_ROOT = os.path.expanduser("~/spot_vision_recordings")
RATES_FILE = os.path.expanduser("~/.spot_vision_rates.json")   # measured bytes/s
SESSION_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,119}$")
CLOCK_PORT = 31545           # UDP, from the lab console during a recording
CLOCK_MAGIC = b"SPCK"
CLOCK_FORMAT = "<4sIdd"      # magic, sequence, packet time [s], experiment clock [s] (NaN: none)
CLOCK_FILE = "clock.csv"
FRAME_LOG = "frames.csv"     # each camera's, from its worker
DISK_REFUSE = 2e9            # [bytes] free: no recording starts below this
DISK_STOP = 1e9              # [bytes] free: a recording stops below this
RECORD_START_TIMEOUT = 25.0  # [s] for a camera to start recording (the ZED opens slowly)
RECORD_STOP_TIMEOUT = 30.0   # [s] for it to close its files
FIT_QUANTILE = 0.9           # the clock fit's offset: the least delayed packets (see fit_clock)
FILE_CHUNK = 1 << 20         # [bytes] a recording's files are sent this much at a time

# Rough recording rates before a camera has recorded once (then its
# measured rate is used): bytes a pixel, by kind and format.
ZED_SIZES = {"HD2K": (2208, 1242, 15), "HD1080": (1920, 1080, 30), "HD720": (1280, 720, 60),
             "VGA": (672, 376, 100)}   # width, height, fastest fps
ZED_BYTES_PER_PIXEL = {"H264": 0.05, "H265": 0.03, "LOSSLESS": 1.0}   # both images, compressed
BAG_COMPRESSION = 0.8        # the RealSense bag's LZ4 on raw colour and depth
MJPEG_BYTES_PER_PIXEL = 0.15
DVS_RATE = 2e6               # [bytes/s] events depend on the scene
THERMAL_FRAME = 320 * 240 * 2
THERMAL_RATE = 59.6          # [Hz]

FIRST_FRAME_TIMEOUT = 15.0   # [s] opening the ZED takes a few seconds
FRAME_TIMEOUT = 5.0          # [s] between frames of a stream
HOUSEKEEPING_PERIOD = 1.0    # [s] idle cameras are checked this often
PARAMS_TIMEOUT = 4.0         # [s] for the worker's new settings list
QUIT_TIMEOUT = 5             # [s] for a worker to quit


def read(path, default=""):
    """Return a small file's text, stripped; default if unreadable."""
    try:
        with open(path) as f:
            return f.read().strip()
    except OSError:
        return default


def usb_parent(path):
    """Return the USB device folder a sysfs path is under, or None."""
    folder = os.path.realpath(path)
    while folder and folder != "/":
        if os.path.exists(os.path.join(folder, "idVendor")):
            return folder
        folder = os.path.dirname(folder)
    return None


def discover():
    """Return what is plugged in: {camera id: {kind, name, ...}}."""
    found = {}
    sdk_usb = set()

    # Cameras with their own SDK, by USB vendor: ZED, RealSense,
    # iniVation, ICI.
    for dev in sorted(glob.glob("/sys/bus/usb/devices/*")):
        vendor = read(os.path.join(dev, "idVendor"))
        product = read(os.path.join(dev, "idProduct"))
        name, serial = read(os.path.join(dev, "product")), read(os.path.join(dev, "serial"))
        if vendor == ZED_VENDOR:
            cid = "zed-%s" % (serial or product)
            found[cid] = {"kind": "zed", "name": "Stereolabs %s" % (name or "ZED"),
                          "serial": serial, "arg": None, "usb": read(os.path.join(dev, "speed"))}
            sdk_usb.add(os.path.realpath(dev))
        elif vendor == INTEL_VENDOR and "realsense" in name.lower():
            cid = "rs-%s" % (serial or product)
            # USB pads the serial with zeros, in capitals: librealsense
            # knows it as e.g. f1422074.
            found[cid] = {"kind": "realsense",
                          "name": name.replace("(R)", "").replace("(TM)", "").strip(),
                          "serial": serial, "arg": serial.lstrip("0").lower() or None,
                          "usb": read(os.path.join(dev, "speed"))}
            sdk_usb.add(os.path.realpath(dev))
        elif vendor == INIVATION_VENDOR:
            cid = "dvs-%s" % (serial or product)
            found[cid] = {"kind": "dvs", "name": "iniVation %s" % (name or "Event Camera"),
                          "serial": serial, "arg": None, "usb": read(os.path.join(dev, "speed"))}
            sdk_usb.add(os.path.realpath(dev))
        elif vendor == ICI_VENDOR and product in ICI_PRODUCTS:
            # No serial number, and a new USB device once its firmware
            # is loaded: known by its USB port (e.g. 1-2.3) instead.
            cid = "thermal-%s" % os.path.basename(dev)
            found[cid] = {"kind": "thermal", "name": "ICI 9320P Thermal Camera", "serial": "",
                          "arg": None, "usb": read(os.path.join(dev, "speed"))}
            sdk_usb.add(os.path.realpath(dev))
    if DEMO:
        found["demo"] = {"kind": "demo", "name": "Test Pattern", "serial": "", "arg": None,
                         "usb": ""}

    # Any other USB camera: its video capture node (index 0), not one
    # of the SDK cameras'.
    for node in sorted(glob.glob("/sys/class/video4linux/video*"),
                       key=lambda path: int(re.sub(r"\D", "", path) or 0)):
        if read(os.path.join(node, "index"), "0") != "0":
            continue
        usb = usb_parent(os.path.join(node, "device"))
        if not usb or usb in sdk_usb:
            continue
        dev = "/dev/" + os.path.basename(node)
        cid = "uvc-%s" % os.path.basename(node)
        found[cid] = {"kind": "uvc", "name": read(os.path.join(node, "name")) or dev,
                      "serial": read(os.path.join(usb, "serial")), "arg": dev,
                      "usb": read(os.path.join(usb, "speed"))}
    return found


class Camera:
    """One camera: its worker, newest frames and viewers."""

    def __init__(self, cid, info, python):
        """Set up a camera, closed.

        Args:
            cid: Its id.
            info: discover()'s entry.
            python: The Python to run its worker with.
        """
        self.id, self.info, self.python = cid, info, python
        self.lock = threading.Condition()
        self.proc = None
        self.frames = {}          # view -> (seq, jpeg)
        self.seq = 0
        self.subs = []            # [{"view", "fps", "q", "w"}]
        self.state, self.detail = "idle", None
        self.extra = {}
        self.idle_since = time.time()
        self.sent = None
        # The worker's last settings list (kept while closed).
        self.params, self.param_errors, self.params_seq = [], {}, 0
        # What was set: sent each time it opens.
        self.saved = load_settings().get(cid, {})
        # A recording: {"folder", "frames", "bytes", "files", "error",
        # "note", "done"} from the worker's R lines; None when none.
        self.recording = None

    def status(self):
        """Return what the Vision tab shows of the camera."""
        with self.lock:
            return dict({"id": self.id, "kind": self.info["kind"], "name": self.info["name"],
                         "serial": self.info["serial"], "usb_speed_mbps": self.info.get("usb"),
                         "views": [{"id": view_id, "label": label}
                                   for view_id, label in VIEWS[self.info["kind"]]],
                         "state": self.state, "detail": self.detail,
                         "viewers": len(self.subs), "record_rate": record_rate(self),
                         "recording": dict(self.recording) if self.recording else None},
                        **self.extra)

    # Viewers

    def subscribe(self, view, fps, q, w):
        """Add a viewer (opening the camera); return its handle.

        Args:
            view: The view's id.
            fps: Frames per second.
            q: JPEG quality.
            w: Width (0 = full).
        """
        sub = {"view": view, "fps": fps, "q": q, "w": w}
        with self.lock:
            self.subs.append(sub)
            self._ensure_worker()
            self._send_settings()
        return sub

    def unsubscribe(self, sub):
        """Remove a viewer; the last one starts the idle time."""
        with self.lock:
            if sub in self.subs:
                self.subs.remove(sub)
            if not self.subs:
                self.idle_since = time.time()
            self._send_settings()

    def wait_frame(self, view, after, timeout=FRAME_TIMEOUT):
        """Return the next frame of a view newer than seq after.

        Returns:
            (seq, jpeg), or None (timeout, or the camera failed).
        """
        end = time.time() + timeout
        with self.lock:
            while True:
                frame = self.frames.get(view)
                if frame and frame[0] > after:
                    return frame
                if self.state in ("busy", "error") or self.proc is None:
                    return None
                left = end - time.time()
                if left <= 0:
                    return None
                self.lock.wait(left)

    # Settings

    def settings(self):
        """Return its settings, errors, what was set, whether open."""
        with self.lock:
            return {"params": self.params, "errors": self.param_errors, "saved": self.saved,
                    "open": self.proc is not None and self.state not in ("busy", "error")}

    def set_params(self, values, reset=False, timeout=PARAMS_TIMEOUT):
        """Keep settings and send them to the worker if it runs.

        Args:
            values: {setting id: value}.
            reset: Forget them all instead: back to each setting's
                default.
            timeout: Seconds to wait for the worker's new list.
        """
        with self.lock:
            # Keep them (reset: the defaults, nothing kept).
            if reset:
                values = {param["id"]: param["default"] for param in self.params
                          if param.get("default") is not None}
                self.saved = {}
            else:
                self.saved.update(values)
            save_settings(self.id, self.saved)

            # Send them to a running worker, and wait for its new list.
            seq = self.params_seq
            running = self.proc is not None and self.proc.poll() is None
            if running and values:
                try:
                    self.proc.stdin.write((json.dumps({"params": values}) + "\n").encode())
                except (BrokenPipeError, OSError):
                    running = False
            end = time.time() + timeout
            while running and self.params_seq == seq and time.time() < end:
                self.lock.wait(end - time.time())
        return self.settings()

    # Recording (for an experiment: Vision.record_start)

    def start_recording(self, folder, timeout=RECORD_START_TIMEOUT):
        """Have the worker record into folder (opening the camera).

        Returns:
            The recording's state once the worker has started it (or
            said why not); "done" is set if it didn't start.
        """
        with self.lock:
            self.recording = {"folder": folder, "frames": 0, "bytes": 0, "files": [],
                              "error": None, "note": None, "done": False, "started": False}
            self._ensure_worker()
            self._send_settings()
            self._command({"record": {"folder": folder}})
            end = time.time() + timeout
            while (not self.recording["started"] and not self.recording["done"]
                   and time.time() < end):
                self.lock.wait(end - time.time())
            if not self.recording["started"] and not self.recording["done"]:
                self.recording.update(done=True, error="the camera didn't start recording "
                                                       "within %g s" % timeout)
            return dict(self.recording)

    def stop_recording(self, timeout=RECORD_STOP_TIMEOUT):
        """Have the worker close its recording; return its state."""
        with self.lock:
            if not self.recording:
                return None
            if not self.recording["done"]:
                self._command({"record": None})
                end = time.time() + timeout
                while not self.recording["done"] and time.time() < end:
                    self.lock.wait(end - time.time())
                if not self.recording["done"]:
                    self.recording.update(done=True, error="the camera didn't close its "
                                                           "recording within %g s" % timeout)
            self.idle_since = time.time()
            last, self.recording = dict(self.recording), None
            return last

    # The worker (call with the lock held)

    def _command(self, msg):
        """Send the worker a command, if it runs."""
        if self.proc and self.proc.poll() is None:
            try:
                self.proc.stdin.write((json.dumps(msg) + "\n").encode())
            except (BrokenPipeError, OSError):
                pass

    def _ensure_worker(self):
        """Start the worker (if not running), with kept settings."""
        if self.proc and self.proc.poll() is None:
            return
        args = [self.python, "-u", WORKER, self.info["kind"]] + (
            [self.info["arg"]] if self.info["arg"] else [])
        self.proc = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE, bufsize=0)
        self.state, self.detail, self.frames, self.sent = "opening", None, {}, None

        # What was set, applied as it opens.
        if self.saved:
            try:
                self.proc.stdin.write((json.dumps({"params": self.saved}) + "\n").encode())
            except (BrokenPipeError, OSError):
                pass
        threading.Thread(target=self._read, args=(self.proc,), daemon=True).start()
        threading.Thread(target=self._stderr, args=(self.proc,), daemon=True).start()

    def _send_settings(self):
        """Tell the worker what the viewers need (if it changed).

        The views watched, the highest frame rate (at most MAX_FPS) and
        quality, and the largest width (any 0: full).
        """
        if not self.proc or self.proc.poll() is not None:
            return
        views = sorted({sub["view"] for sub in self.subs})
        settings = {"views": views,
                    "fps": min(MAX_FPS, max([sub["fps"] for sub in self.subs] or [1])),
                    "quality": max([sub["q"] for sub in self.subs] or [80]),
                    "width": 0 if any(sub["w"] == 0 for sub in self.subs)
                    else max([sub["w"] for sub in self.subs] or [0])}
        if settings != self.sent:
            self.sent = settings
            try:
                self.proc.stdin.write((json.dumps(settings) + "\n").encode())
            except (BrokenPipeError, OSError):
                pass

    def _read(self, proc):
        """Read the worker's output until it exits.

        Lines: "F <view> <n>" then n bytes of JPEG; "P <json>" its
        settings list; "S <json>" its state.
        """
        out = proc.stdout
        while True:
            line = out.readline()
            if not line:
                break
            if line.startswith(b"F "):
                # A frame.
                _, view, n = line.split()
                data = out.read(int(n))
                with self.lock:
                    self.seq += 1
                    self.frames[view.decode()] = (self.seq, data)
                    if self.state != "streaming":
                        self.state, self.detail = "streaming", None
                    self.lock.notify_all()
            elif line.startswith(b"P "):
                # Its settings list.
                try:
                    msg = json.loads(line[2:])
                except ValueError:
                    continue
                with self.lock:
                    self.params, self.param_errors = msg.get("params", []), msg.get("errors", {})
                    self.params_seq += 1
                    self.lock.notify_all()
            elif line.startswith(b"R "):
                # A recording's progress (and its end).
                try:
                    msg = json.loads(line[2:])
                except ValueError:
                    continue
                with self.lock:
                    if self.recording is not None:
                        self.recording.update({k: msg.get(k) for k in (
                            "frames", "bytes", "files", "error", "note", "done")})
                        self.recording["started"] = True
                        self.lock.notify_all()
            elif line.startswith(b"S "):
                # Its state, and the model and serial it found.
                try:
                    msg = json.loads(line[2:])
                except ValueError:
                    continue
                with self.lock:
                    self.state, self.detail = msg.pop("state", self.state), msg.pop("detail", None)
                    self.extra.update({k: v for k, v in msg.items()
                                       if k in ("model", "serial", "housing_c")})
                    self.lock.notify_all()

        # It exited: idle, or an error with its code.
        code = proc.wait()
        with self.lock:
            if self.proc is proc and self.recording and not self.recording["done"]:
                self.recording.update(done=True, error=self.recording["error"] or self.detail or
                                      "its worker stopped (code %s)" % code)
            if self.proc is proc:
                self.proc = None
                if self.state not in ("busy", "error"):
                    self.state = "idle" if code == 0 else "error"
                    if code:
                        self.detail = self.detail or "its worker stopped (code %s)" % code
            self.lock.notify_all()

    def _stderr(self, proc):
        """Pass the worker's stderr on (the SDKs talk there)."""
        for raw in proc.stderr:
            sys.stderr.write("[%s] %s" % (self.id, raw.decode("utf-8", "replace")))

    def reap(self):
        """Close the camera once nobody has watched it for IDLE_CLOSE s.

        Called from the housekeeping thread.
        """
        with self.lock:
            proc = self.proc
            if (not proc or self.subs or self.recording
                    or time.time() - self.idle_since < IDLE_CLOSE):
                return
            self.proc = None
            self.state, self.detail = "idle", None

        # Ask it to quit; kill it if it doesn't.
        try:
            proc.stdin.write(b'{"quit": true}\n')
            proc.stdin.close()
        except (BrokenPipeError, OSError):
            pass
        try:
            proc.wait(QUIT_TIMEOUT)
        except subprocess.TimeoutExpired:
            proc.kill()


# Recording rates (for the Experiment tab's estimate)


def mode_size(value, default):
    """Return (width, height, fps) from "WxH@fps" (or default)."""
    match = re.match(r"(\d+)x(\d+)@(\d+)$", str(value or ""))
    return tuple(int(g) for g in match.groups()) if match else default


def formula_rate(kind, saved):
    """Return a camera's rough recording rate [bytes/s], by settings."""
    if kind == "zed":
        w, h, top = ZED_SIZES.get(saved.get("resolution"), ZED_SIZES["HD720"])
        fps = min(int(saved.get("camera_fps", 30)), top)
        return 2 * w * h * fps * ZED_BYTES_PER_PIXEL.get(saved.get("svo_compression"), 0.05)
    if kind == "realsense":
        cw, ch, cf = mode_size(saved.get("mode.color"), (1280, 720, 30))
        dw, dh, df = mode_size(saved.get("mode.depth"), (1024, 768, 30))
        return BAG_COMPRESSION * (cw * ch * 3 * cf + dw * dh * 2 * df)
    if kind == "dvs":
        return DVS_RATE
    if kind == "thermal":
        return THERMAL_FRAME * (float(saved.get("record_fps", 30)) or THERMAL_RATE)
    if kind == "uvc":
        w, h = (int(x) for x in str(saved.get("size", "1280x720")).split("x"))
        return w * h * MJPEG_BYTES_PER_PIXEL * (float(saved.get("record_fps", 0)) or 30)
    return 1280 * 720 * MJPEG_BYTES_PER_PIXEL * float(saved.get("record_fps", 30))


def record_rate(cam):
    """Return {"bytes_per_s", "measured"}: a camera's recording rate.

    Measured, if it has recorded with the settings it has now; else
    estimated from them.
    """
    kept = load_json(RATES_FILE).get(cam.id)
    if kept and kept.get("settings") == cam.saved and kept.get("bytes_per_s"):
        return {"bytes_per_s": kept["bytes_per_s"], "measured": True}
    try:
        rate = formula_rate(cam.info["kind"], cam.saved)
    except (TypeError, ValueError):
        rate = formula_rate(cam.info["kind"], {})
    return {"bytes_per_s": round(rate), "measured": False}


def disk():
    """Return the recordings' disk: {"path", "free", "total"} [B]."""
    os.makedirs(RECORD_ROOT, exist_ok=True)
    usage = shutil.disk_usage(RECORD_ROOT)
    return {"path": RECORD_ROOT, "free": usage.free, "total": usage.total}


# The experiment clock: fitted to this computer's monotonic clock


def fit_clock(samples):
    """Fit a clock to orin_time: clock = offset + slope * orin_time.

    Each sample is the clock's value in a packet and when it arrived:
    the clock had moved on by the packet's delay, so the slope is a
    least-squares fit and the offset the FIT_QUANTILE of the clock less
    slope * orin_time, near the least delayed packets.

    Args:
        samples: [(orin_time, clock)] with finite values.

    Returns:
        {"offset", "slope", "samples", "spread_ms"}, or None (none).
    """
    n = len(samples)
    if not n:
        return None
    if n == 1:
        slope = 1.0
    else:
        mx = sum(t for t, _ in samples) / n
        my = sum(c for _, c in samples) / n
        sxx = sum((t - mx) ** 2 for t, _ in samples)
        slope = (sum((t - mx) * (c - my) for t, c in samples) / sxx) if sxx > 0 else 1.0
    rest = sorted(c - slope * t for t, c in samples)
    offset = rest[min(n - 1, int(FIT_QUANTILE * n))]
    return {"offset": offset, "slope": slope, "samples": n,
            "spread_ms": round(1000 * (rest[-1] - rest[0]), 2)}


def clock_fits(path):
    """Return the experiment clock's and the packet time's fits.

    From a session's clock.csv (orin_time, packet_time,
    experiment_clock). The packet time is fitted over its last stretch
    without a jump back (StreamData restarts it with the stream).
    """
    exp, packet = [], []
    try:
        with open(path) as f:
            next(f, None)
            for line in f:
                parts = line.strip().split(",")
                if len(parts) < 3:
                    continue
                try:
                    t, pt, ec = (float(v) for v in parts[:3])
                except ValueError:
                    continue
                if math.isfinite(ec):
                    exp.append((t, ec))
                if math.isfinite(pt):
                    if packet and pt < packet[-1][1]:
                        packet = []
                    packet.append((t, pt))
    except OSError:
        pass
    return {"experiment": fit_clock(exp), "phasespace": fit_clock(packet)}


def add_clock_columns(path, fits):
    """Add experiment_time and phasespace_time to a frame log.

    From its orin_time column (empty where a clock has no fit).
    """
    def at(fit, t):
        """Return a fit's clock at orin_time t, as text."""
        return "" if fit is None or t is None else "%.6f" % (fit["offset"] + fit["slope"] * t)

    tmp = path + ".tmp"
    with open(path) as src, open(tmp, "w") as dst:
        header = next(src, "").rstrip("\n").split(",")
        column = header.index("orin_time") if "orin_time" in header else None
        dst.write(",".join(header + ["experiment_time", "phasespace_time"]) + "\n")
        for line in src:
            parts = line.rstrip("\n").split(",")
            try:
                t = float(parts[column]) if column is not None else None
            except (IndexError, ValueError):
                t = None
            dst.write(",".join(parts + [at(fits["experiment"], t),
                                        at(fits["phasespace"], t)]) + "\n")
    os.replace(tmp, path)


SESSION_README = """SPOT Vision recording: {session}
Recorded on {host} from {started} to {stopped} (local time), for an experiment
started from the SPOT Console. session.json has the details.

Each camera has a folder:
  frames.csv       a row per frame (per event batch for an event camera):
                     frame            its index in the camera's file
                     orin_time        when it was taken, on the Orin's monotonic clock [s]
                                      (from the camera's own time stamp where it is on the
                                      Orin's clock, else when the frame arrived)
                     arrival_time     when it reached SPOT Vision [s, same clock]
                     ...              the camera's own columns (its time stamp, file, ...)
                     experiment_time  the experiment's clock [s]: 0 when the platforms left
                                      phase 0, negative before (empty if the experiment
                                      never started)
                     phasespace_time  the PhaseSpace stream's packet time [s]
  ZED              zed.svo2 / zed.svo: both images, every frame (the ZED SDK reads it and
                   can compute depth from it); camera_time_ns is the SDK's image time stamp
  RealSense        realsense_<n>.bag: the colour and depth streams (librealsense, ROS bag)
  DVXplorer        events.aedat4 (dv-processing), or events.bin + events_bin.json (raw
                   event rows); a frames.csv row per batch of events (first_us, last_us)
  Thermal          thermal_raw.bin: 240 x 320 little-endian uint16 raw counts per frame
                   (14-bit; not temperatures); orin_time is the middle of the frame's readout,
                   on the sensor's clock (its SDK's ici9320.retime); sensor_frame its number
                   (a gap: not kept, or lost); shutter: ok, flat (shutter closed) or settling
  Other cameras    video.avi / pattern.avi: MJPEG

clock.csv holds the clock packets the lab console relayed (orin_time, packet_time,
experiment_clock); experiment_time and phasespace_time are straight-line fits of those
to orin_time (session.json: clock), so they can be fitted again.
"""


# Kept settings

_settings_lock = threading.Lock()


def load_json(path):
    """Return a JSON object from a file; {} if missing or not one."""
    try:
        with open(path) as f:
            data = json.load(f)
        return data if isinstance(data, dict) else {}
    except (OSError, ValueError):
        return {}


def save_json(path, data):
    """Write a JSON file atomically."""
    tmp = path + ".tmp"
    with open(tmp, "w") as f:
        json.dump(data, f, indent=1)
    os.replace(tmp, path)


def load_settings():
    """Return the kept settings: {camera id: {setting id: value}}."""
    try:
        with open(SETTINGS_FILE) as f:
            data = json.load(f)
        return data if isinstance(data, dict) else {}
    except (OSError, ValueError):
        return {}


def save_settings(cid, values):
    """Keep a camera's settings (none: forget it), atomically."""
    with _settings_lock:
        data = load_settings()
        if values:
            data[cid] = values
        else:
            data.pop(cid, None)
        tmp = SETTINGS_FILE + ".tmp"
        with open(tmp, "w") as f:
            json.dump(data, f, indent=1)
        os.replace(tmp, SETTINGS_FILE)


# Experiments' recordings


class RecordError(Exception):
    """A recording can't start (or there is no such recording)."""


def in_parallel(items, fn):
    """Return {item: fn(item)}, each in a thread of its own."""
    out = {}

    def one(item):
        """Run fn on one item."""
        out[item] = fn(item)

    threads = [threading.Thread(target=one, args=(item,)) for item in items]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    return out


def session_folder(name):
    """Return a recording's folder.

    Raises:
        RecordError: if name isn't a recording's name.
    """
    if not SESSION_RE.match(name or ""):
        raise RecordError("%r isn't a recording's name" % name)
    return os.path.join(RECORD_ROOT, name)


def session_files(name):
    """Return a recording's files: [{"path", "size"}], paths with "/".

    Raises:
        RecordError: if there is no such recording.
    """
    folder = session_folder(name)
    if not os.path.isdir(folder):
        raise RecordError("no recording %s" % name)
    out = []
    for dirpath, dirnames, filenames in os.walk(folder):
        dirnames.sort()
        for f in sorted(filenames):
            path = os.path.join(dirpath, f)
            out.append({"path": os.path.relpath(path, folder).replace(os.sep, "/"),
                        "size": os.path.getsize(path)})
    return out


class Session:
    """A recording in progress: its cameras and the clock's samples."""

    def __init__(self, name, cams):
        """Start one: its folder (made) and clock.csv.

        Args:
            name: Its name (the folder's).
            cams: {camera id: Camera}.
        """
        self.name, self.cams = name, cams
        self.folder = session_folder(name)
        os.makedirs(self.folder)
        self.started, self.stopped, self.note = time.time(), None, None
        self.camera_started = {}    # camera id -> when it began recording
        self.stopping = False
        self._clock_lock = threading.Lock()
        self._clock = open(os.path.join(self.folder, CLOCK_FILE), "w")
        self._clock.write("orin_time,packet_time,experiment_clock\n")
        self.clock_count, self.clock_last, self.clock_value = 0, None, None

    def add_clock(self, t, packet_time, exp_clock):
        """Keep a clock packet that arrived at orin_time t."""
        with self._clock_lock:
            if self._clock.closed:
                return
            self._clock.write("%.6f,%.6f,%s\n" % (t, packet_time, "%.6f" % exp_clock
                                                   if math.isfinite(exp_clock) else "nan"))
            self.clock_count += 1
            self.clock_last, self.clock_value = t, exp_clock

    def close_clock(self):
        """Close clock.csv."""
        with self._clock_lock:
            self._clock.close()

    def status(self):
        """Return the recording's state for the pages."""
        now = time.monotonic()
        cams = {}
        for cid, cam in self.cams.items():
            with cam.lock:
                rec = dict(cam.recording) if cam.recording else None
            cams[cid] = {"name": cam.info["name"], "kind": cam.info["kind"], "recording": rec}
        return {"session": self.name, "started": self.started, "note": self.note,
                "bytes": sum((c["recording"] or {}).get("bytes") or 0 for c in cams.values()),
                "cameras": cams,
                "clock": {"samples": self.clock_count,
                          "age": None if self.clock_last is None
                          else round(now - self.clock_last, 2),
                          "experiment_clock": self.clock_value
                          if self.clock_value is None or math.isfinite(self.clock_value)
                          else None}}


# All cameras


class Vision:
    """The cameras plugged in, and closing idle ones."""

    def __init__(self, pythons):
        """Find the cameras and start the housekeeping.

        Args:
            pythons: {camera kind: the Python for its worker}.
        """
        self.pythons = pythons
        self.lock = threading.Lock()
        self.cams = {}
        self.session = None                    # the recording in progress
        self.record_lock = threading.Lock()   # one start or stop at a time
        self.refresh()
        threading.Thread(target=self._housekeeping, daemon=True).start()
        threading.Thread(target=self._clock_listener, daemon=True).start()

    def refresh(self):
        """Look for cameras again; return them all.

        New ones are added; an unplugged one is dropped, or shows as
        unplugged while it is still watched or open. Not while it opens:
        the thermal camera leaves the bus for a moment as it takes its
        firmware (its worker says if it doesn't come back).
        """
        found = discover()
        with self.lock:
            for cid, info in found.items():
                if cid not in self.cams:
                    self.cams[cid] = Camera(cid, info, self.pythons[info["kind"]])
            for cid in list(self.cams):
                cam = self.cams[cid]
                if cid not in found and not cam.subs and cam.proc is None:
                    del self.cams[cid]
                elif cid not in found and cam.state != "opening":
                    cam.state, cam.detail = "error", "unplugged"
            return list(self.cams.values())

    def get(self, cid):
        """Return a camera by id, or None."""
        with self.lock:
            return self.cams.get(cid)

    def _housekeeping(self):
        """Close idle cameras, and stop a recording on a full disk."""
        while True:
            time.sleep(HOUSEKEEPING_PERIOD)
            with self.lock:
                cams = list(self.cams.values())
            for cam in cams:
                cam.reap()
            session = self.session
            if session and not session.stopping and disk()["free"] < DISK_STOP:
                session.stopping = True
                threading.Thread(target=self.record_stop, kwargs={
                    "note": "stopped early: the Orin's disk was nearly full "
                            "(%.1f GB free)" % (disk()["free"] / 1e9)}, daemon=True).start()

    # Recording (the lab console starts and stops it with an experiment)

    def record_start(self, name, cids):
        """Start recording cameras, all of them or none.

        Args:
            name: The recording's name (its folder's).
            cids: The cameras' ids.

        Returns:
            The recording's status.

        Raises:
            RecordError: if one is going, the name is taken, the disk is
                nearly full, a camera is unknown, or one can't record
                (the others are stopped again).
        """
        with self.record_lock:
            folder = session_folder(name)
            if self.session:
                raise RecordError("SPOT Vision is already recording (%s)" % self.session.name)
            if os.path.exists(folder):
                raise RecordError("there is already a recording named %s" % name)
            free = disk()["free"]
            if free < DISK_REFUSE:
                raise RecordError("the Orin's disk has only %.1f GB free" % (free / 1e9))
            cids = list(dict.fromkeys(str(c) for c in cids or []))
            if not cids:
                raise RecordError("no cameras to record")
            cams = {}
            for cid in cids:
                cam = self.get(cid)
                if cam is None:
                    raise RecordError("there is no camera %s on the Orin now" % cid)
                cams[cid] = cam

            # The clock samples count from now; each camera starts.
            session = self.session = Session(name, cams)
            results = in_parallel(cids, lambda c: cams[c].start_recording(
                os.path.join(session.folder, c)))
            for cid in cids:
                session.camera_started[cid] = time.time()
            failed = {c: r for c, r in results.items() if r["done"]}
            if failed:
                in_parallel(cids, lambda c: cams[c].stop_recording())
                session.close_clock()
                self.session = None
                shutil.rmtree(session.folder, ignore_errors=True)
                raise RecordError("; ".join("the %s couldn't record: %s"
                                            % (cams[c].info["name"], r["error"] or "no reason")
                                            for c, r in failed.items()))
            return session.status()

    def record_stop(self, name=None, note=None):
        """Stop the recording; finish its files.

        Adds the clocks to each frame log, writes session.json and
        README.txt, and keeps each camera's measured rate.

        Args:
            name: Stop only this recording (None: whichever).
            note: Why it stopped, if not asked to.

        Returns:
            Its session.json, or None if nothing was recording.
        """
        with self.record_lock:
            session = self.session
            if session is None or (name and name != session.name):
                return None
            session.stopping = True
            session.note = note
            results = in_parallel(list(session.cams), lambda c: session.cams[c].stop_recording())
            session.close_clock()
            session.stopped = time.time()

            # The clocks, into each frame log.
            fits = clock_fits(os.path.join(session.folder, CLOCK_FILE))
            for cid in session.cams:
                log = os.path.join(session.folder, cid, FRAME_LOG)
                if os.path.isfile(log):
                    try:
                        add_clock_columns(log, fits)
                    except (OSError, ValueError) as e:
                        results[cid] = dict(results[cid] or {}, error="couldn't add the clocks "
                                            "to frames.csv: %s" % e)

            # Each camera's measured rate, for the next estimate.
            rates = load_json(RATES_FILE)
            for cid, result in results.items():
                seconds = session.stopped - session.camera_started.get(cid, session.started)
                if result and not result.get("error") and result.get("bytes") and seconds > 5:
                    rates[cid] = {"bytes_per_s": round(result["bytes"] / seconds),
                                  "settings": session.cams[cid].saved,
                                  "measured": time.strftime("%Y-%m-%d %H:%M:%S")}
            save_json(RATES_FILE, rates)

            # What it holds.
            manifest = {
                "session": session.name, "host": os.uname().nodename, "started": session.started,
                "stopped": session.stopped, "note": note, "clock": fits,
                "clock_samples": session.clock_count,
                "cameras": {cid: {"kind": cam.info["kind"], "name": cam.info["name"],
                                  "model": cam.extra.get("model"),
                                  "serial": cam.info["serial"] or cam.extra.get("serial"),
                                  "settings": cam.saved,
                                  "frames": (results[cid] or {}).get("frames"),
                                  "error": (results[cid] or {}).get("error"),
                                  "note": (results[cid] or {}).get("note")}
                            for cid, cam in session.cams.items()}}
            save_json(os.path.join(session.folder, "session.json"), manifest)
            with open(os.path.join(session.folder, "README.txt"), "w") as f:
                f.write(SESSION_README.format(
                    session=session.name, host=manifest["host"],
                    started=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(session.started)),
                    stopped=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(session.stopped))))
            self.session = None
            manifest["files"] = session_files(session.name)
            manifest["bytes"] = sum(f["size"] for f in manifest["files"])
            return manifest

    def recordings(self):
        """Return the recordings kept, newest first, and the disk."""
        out = []
        active = self.session.name if self.session else None
        if os.path.isdir(RECORD_ROOT):
            for name in sorted(os.listdir(RECORD_ROOT), reverse=True):
                folder = os.path.join(RECORD_ROOT, name)
                if not os.path.isdir(folder) or not SESSION_RE.match(name):
                    continue
                manifest = load_json(os.path.join(folder, "session.json"))
                out.append({"session": name, "active": name == active,
                            "started": manifest.get("started"), "stopped": manifest.get("stopped"),
                            "cameras": sorted(manifest.get("cameras") or []),
                            "bytes": sum(f["size"] for f in session_files(name))})
        return {"active": self.session.status() if self.session else None, "recordings": out,
                "disk": disk()}

    def record_delete(self, name):
        """Delete a recording (not one in progress).

        Raises:
            RecordError: if it is recording, or there is none.
        """
        folder = session_folder(name)
        if self.session and self.session.name == name:
            raise RecordError("%s is still recording" % name)
        if not os.path.isdir(folder):
            raise RecordError("no recording %s" % name)
        shutil.rmtree(folder)
        return self.recordings()

    def _clock_listener(self):
        """Keep the experiment clock's packets during a recording.

        The lab console relays each PhaseSpace packet's clock (UDP
        CLOCK_PORT, CLOCK_FORMAT); each is kept with when it arrived.
        """
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            sock.bind(("", CLOCK_PORT))
        except OSError as e:
            print("Not listening for the experiment clock on UDP %d: %s" % (CLOCK_PORT, e))
            return
        size = struct.calcsize(CLOCK_FORMAT)
        while True:
            try:
                data = sock.recv(256)
            except OSError:
                continue
            t = time.monotonic()
            if len(data) < size:
                continue
            magic, _, packet_time, exp_clock = struct.unpack(CLOCK_FORMAT, data[:size])
            session = self.session
            if magic == CLOCK_MAGIC and session is not None:
                session.add_clock(t, packet_time, exp_clock)


def number(qs, key, default, lo, hi):
    """Return a query value as an int in [lo, hi], else default."""
    try:
        return max(lo, min(hi, int(float(qs.get(key, [default])[0]))))
    except (TypeError, ValueError):
        return default


# Requests


class Handler(BaseHTTPRequestHandler):
    """One HTTP request: cameras, settings, streams, snapshots."""

    server_version = "SPOTVision/1.0"
    vision = None

    def log_message(self, fmt, *args):
        """Log nothing."""
        pass

    def _send(self, code, body, ctype, extra=()):
        """Send a reply (any origin may read it; not cached)."""
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Cache-Control", "no-store")
        for name, value in extra:
            self.send_header(name, value)
        self.end_headers()
        self.wfile.write(body)

    def _json(self, obj, code=200):
        """Send obj as JSON."""
        self._send(code, json.dumps(obj).encode(), "application/json")

    def do_GET(self):
        """Serve cameras, settings, a stream, a snapshot or the page."""
        url = urlparse(self.path)
        qs = parse_qs(url.query)
        try:
            # The cameras.
            if url.path == "/api/cameras":
                session = self.vision.session
                return self._json({"cameras": [cam.status() for cam in self.vision.refresh()],
                                   "host": os.uname().nodename, "time": time.time(),
                                   "disk": disk(),
                                   "recording": session.status() if session else None})

            # The recordings, and one's files.
            if url.path == "/api/record":
                return self._json(self.vision.recordings())
            match = re.match(r"^/api/record/([\w.-]+)$", url.path)
            if match:
                try:
                    files = session_files(match.group(1))
                except RecordError as e:
                    return self._json({"error": str(e)}, 404)
                return self._json({"session": match.group(1), "files": files,
                                   "bytes": sum(f["size"] for f in files),
                                   "manifest": load_json(os.path.join(
                                       session_folder(match.group(1)), "session.json"))})

            # A recording's file.
            match = re.match(r"^/recordings/([\w.-]+)/([\w./-]+)$", url.path)
            if match:
                return self._recording_file(match.group(1), match.group(2))

            # A camera's settings.
            match = re.match(r"^/api/cameras/([\w.-]+)/params$", url.path)
            if match:
                cam = self.vision.get(match.group(1))
                return self._json(cam.settings()) if cam else self._json(
                    {"error": "no camera %s" % match.group(1)}, 404)

            # A stream or a snapshot of one of its views.
            match = re.match(r"^/(stream|snapshot)/([\w.-]+)/(\w+)\.(mjpg|jpg)$", url.path)
            if match:
                cam = self.vision.get(match.group(2))
                if cam is None:
                    return self._json({"error": "no camera %s" % match.group(2)}, 404)
                if match.group(3) not in [view_id for view_id, _ in VIEWS[cam.info["kind"]]]:
                    return self._json({"error": "the %s has no %s view"
                                                % (cam.info["name"], match.group(3))}, 404)
                if match.group(1) == "stream":
                    return self._stream(cam, match.group(3), number(qs, "fps", 15, 1, MAX_FPS),
                                        number(qs, "q", 80, 20, 95), number(qs, "w", 960, 0, 4096))
                return self._snapshot(cam, match.group(3), number(qs, "q", 95, 20, 100),
                                      number(qs, "w", 0, 0, 4096))

            # A plain page listing them.
            if url.path == "/":
                rows = "".join(
                    '<li>%s (%s) %s: %s</li>' % (
                        cam.info["name"], cam.id, cam.state,
                        " ".join('<a href="/stream/%s/%s.mjpg">%s</a>' % (cam.id, view_id, label)
                                 for view_id, label in VIEWS[cam.info["kind"]]))
                    for cam in self.vision.refresh())
                return self._send(200, ("<!doctype html><title>SPOT Vision</title><h1>SPOT Vision"
                                        "</h1><ul>%s</ul>" % rows).encode(),
                                  "text/html; charset=utf-8")
            return self._json({"error": "not found"}, 404)
        except (BrokenPipeError, ConnectionResetError):
            pass

    def do_OPTIONS(self):
        """Allow cross-origin JSON posts (the browser asks first)."""
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def do_POST(self):
        """Camera settings; starting, stopping, deleting recordings."""
        url = urlparse(self.path)
        try:
            # The body: JSON.
            length = int(self.headers.get("Content-Length") or 0)
            try:
                body = json.loads(self.rfile.read(length) or b"{}") if length else {}
            except ValueError:
                return self._json({"error": "send JSON"}, 400)
            if not isinstance(body, dict):
                return self._json({"error": "send a JSON object"}, 400)

            # Recordings: {"session", "cameras"} to start; {"session"}
            # to stop or delete.
            if url.path in ("/api/record/start", "/api/record/stop", "/api/record/delete"):
                name = str(body.get("session") or "")
                try:
                    if url.path == "/api/record/start":
                        cameras = body.get("cameras")
                        if not isinstance(cameras, list):
                            return self._json({"error": "cameras must be a list of ids"}, 400)
                        return self._json(self.vision.record_start(name, cameras))
                    if url.path == "/api/record/stop":
                        return self._json({"stopped": self.vision.record_stop(name or None)})
                    return self._json(self.vision.record_delete(name))
                except RecordError as e:
                    return self._json({"error": str(e)}, 409)

            # A camera's settings.
            match = re.match(r"^/api/cameras/([\w.-]+)/params$", url.path)
            if not match:
                return self._json({"error": "not found"}, 404)
            cam = self.vision.get(match.group(1))
            if cam is None:
                return self._json({"error": "no camera %s" % match.group(1)}, 404)

            # {"values": {id: value}} or {"reset": true}.
            values = body.get("values") or {}
            if not isinstance(values, dict) or not all(isinstance(k, str) for k in values):
                return self._json({"error": "values must be {setting id: value}"}, 400)
            return self._json(cam.set_params(values, reset=bool(body.get("reset"))))
        except (BrokenPipeError, ConnectionResetError):
            pass

    def _recording_file(self, name, rel):
        """Send one of a recording's files, a chunk at a time."""
        try:
            folder = os.path.realpath(session_folder(name))
        except RecordError as e:
            return self._json({"error": str(e)}, 404)
        path = os.path.realpath(os.path.join(folder, rel))
        if not path.startswith(folder + os.sep) or not os.path.isfile(path):
            return self._json({"error": "no file %s in %s" % (rel, name)}, 404)
        size = os.path.getsize(path)
        self.send_response(200)
        self.send_header("Content-Type", "application/octet-stream")
        self.send_header("Content-Length", str(size))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        with open(path, "rb") as f:
            while True:
                chunk = f.read(FILE_CHUNK)
                if not chunk:
                    break
                self.wfile.write(chunk)

    def _stream(self, cam, view, fps, q, w):
        """Send a view as MJPEG until the viewer or camera goes."""
        sub = cam.subscribe(view, fps, q, w)
        try:
            # The first frame (opening the ZED takes a few seconds).
            first = cam.wait_frame(view, 0, timeout=FIRST_FRAME_TIMEOUT)
            if first is None:
                state = cam.status()
                return self._json({"error": state["detail"] or "the camera didn't start",
                                   "state": state["state"]}, 503)
            self.send_response(200)
            self.send_header("Content-Type", "multipart/x-mixed-replace; boundary=spotframe")
            self.send_header("Cache-Control", "no-store")
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()

            # Each frame, at most fps a second.
            seq, data, period, sent = first[0], first[1], 1.0 / fps, 0.0
            while True:
                now = time.time()
                if now - sent < period:
                    time.sleep(period - (now - sent))
                self.wfile.write(b"--spotframe\r\nContent-Type: image/jpeg\r\nContent-Length: "
                                 b"%d\r\n\r\n" % len(data))
                self.wfile.write(data + b"\r\n")
                sent = time.time()
                frame = cam.wait_frame(view, seq, timeout=FRAME_TIMEOUT)
                if frame is None:
                    break
                seq, data = frame
        except (BrokenPipeError, ConnectionResetError, OSError):
            pass
        finally:
            cam.unsubscribe(sub)

    def _snapshot(self, cam, view, q, w):
        """Send one frame of a view, as a named JPEG."""
        sub = cam.subscribe(view, 5, q, w)
        try:
            frame = cam.wait_frame(view, 0, timeout=FIRST_FRAME_TIMEOUT)
            if frame is None:
                state = cam.status()
                return self._json({"error": state["detail"] or "no frame",
                                   "state": state["state"]}, 503)
            name = "%s_%s_%s.jpg" % (cam.id, view, time.strftime("%Y%m%d_%H%M%S"))
            self._send(200, frame[1], "image/jpeg",
                       [("Content-Disposition", 'inline; filename="%s"' % name)])
        finally:
            cam.unsubscribe(sub)


def main():
    """Start SPOT Vision: find the cameras, serve until Ctrl+C."""
    global DEMO, RECORD_ROOT
    parser = argparse.ArgumentParser(description="SPOT Vision: stream the Orin's cameras")
    parser.add_argument("--port", type=int, default=8090)
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--zed-python", default=sys.executable,
                        help="a Python with pyzed (the ZED SDK's)")
    parser.add_argument("--realsense-python", default=sys.executable,
                        help="a Python with pyrealsense2 (2.54 for the L515)")
    parser.add_argument("--dvs-python", default=sys.executable,
                        help="a Python with dv-processing (iniVation's, for the DVXplorer)")
    parser.add_argument("--thermal-python", default=sys.executable,
                        help="a Python with python-libusb1, pyusb, numpy and OpenCV "
                             "(the ICI 9320P thermal camera)")
    parser.add_argument("--uvc-python", default=sys.executable, help="a Python with OpenCV")
    parser.add_argument("--demo", action="store_true",
                        help="also a test pattern camera (to try the Vision tab without one)")
    parser.add_argument("--recordings", default=RECORD_ROOT,
                        help="where experiments' recordings go (a folder per recording)")
    args = parser.parse_args()

    # The cameras, each kind with its Python.
    DEMO = args.demo
    RECORD_ROOT = os.path.abspath(os.path.expanduser(args.recordings))
    Handler.vision = Vision({"zed": args.zed_python, "realsense": args.realsense_python,
                             "dvs": args.dvs_python, "thermal": args.thermal_python,
                             "uvc": args.uvc_python,
                             "demo": sys.executable})

    # Serve.
    server = ThreadingHTTPServer((args.host, args.port), Handler)
    server.daemon_threads = True
    print("SPOT Vision on http://%s:%d (cameras: %s)"
          % (args.host, args.port,
             ", ".join(cam.id for cam in Handler.vision.refresh()) or "none"))
    sys.stdout.flush()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
