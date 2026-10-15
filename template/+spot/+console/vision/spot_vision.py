#!/usr/bin/env python3
"""SPOT Vision: the Jetson Orin's cameras, streamed to the Vision tab.

The cameras plugged into the Orin (spot-vision), for the lab console's
Vision tab.

    python3 spot_vision.py [--port 8090] [--realsense-python PATH]
                           [--dvs-python PATH] [--demo]

    GET  /api/cameras
        the cameras: [{id, kind, name, serial, views: [{id, label}],
        state, detail}]
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
    GET  /
        a plain page listing them (for checking by hand)

Each camera runs in a process of its own, vision_worker.py, with the
Python whose library it needs: the ZED SDK's pyzed (this Python); the
RealSense L515's pyrealsense2 2.54, the last librealsense that
supports it, in its own environment (--realsense-python); the
iniVation DVXplorer's dv-processing (--dvs-python); any other USB
camera: OpenCV and v4l2-ctl; --demo adds a test pattern. A camera's
settings are the ones its library offers (the worker lists them, with
their ranges and defaults).

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
import os
import re
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
    "uvc": [("video", "Video")],
    "demo": [("pattern", "Pattern"), ("noise", "Noise")],
}

# USB vendor ids.
ZED_VENDOR = "2b03"
INTEL_VENDOR = "8086"
INIVATION_VENDOR = "152a"   # iniVation's event cameras (DVXplorer, DAVIS)

# Outside the deployed folder, so a redeploy keeps it.
SETTINGS_FILE = os.path.expanduser("~/.spot_vision_settings.json")
DEMO = False

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
    # iniVation.
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

    def status(self):
        """Return what the Vision tab shows of the camera."""
        with self.lock:
            return dict({"id": self.id, "kind": self.info["kind"], "name": self.info["name"],
                         "serial": self.info["serial"], "usb_speed_mbps": self.info.get("usb"),
                         "views": [{"id": view_id, "label": label}
                                   for view_id, label in VIEWS[self.info["kind"]]],
                         "state": self.state, "detail": self.detail,
                         "viewers": len(self.subs)}, **self.extra)

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

    # The worker (call with the lock held)

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
            elif line.startswith(b"S "):
                # Its state, and the model and serial it found.
                try:
                    msg = json.loads(line[2:])
                except ValueError:
                    continue
                with self.lock:
                    self.state, self.detail = msg.pop("state", self.state), msg.pop("detail", None)
                    self.extra.update({k: v for k, v in msg.items() if k in ("model", "serial")})
                    self.lock.notify_all()

        # It exited: idle, or an error with its code.
        code = proc.wait()
        with self.lock:
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
            if not proc or self.subs or time.time() - self.idle_since < IDLE_CLOSE:
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


# Kept settings

_settings_lock = threading.Lock()


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
        self.refresh()
        threading.Thread(target=self._housekeeping, daemon=True).start()

    def refresh(self):
        """Look for cameras again; return them all.

        New ones are added; an unplugged one is dropped, or shows as
        unplugged while it is still watched or open.
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
                elif cid not in found:
                    cam.state, cam.detail = "error", "unplugged"
            return list(self.cams.values())

    def get(self, cid):
        """Return a camera by id, or None."""
        with self.lock:
            return self.cams.get(cid)

    def _housekeeping(self):
        """Close idle cameras, forever."""
        while True:
            time.sleep(HOUSEKEEPING_PERIOD)
            with self.lock:
                cams = list(self.cams.values())
            for cam in cams:
                cam.reap()


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
                return self._json({"cameras": [cam.status() for cam in self.vision.refresh()],
                                   "host": os.uname().nodename, "time": time.time()})

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
        """Change a camera's settings."""
        url = urlparse(self.path)
        try:
            # The camera.
            match = re.match(r"^/api/cameras/([\w.-]+)/params$", url.path)
            if not match:
                return self._json({"error": "not found"}, 404)
            cam = self.vision.get(match.group(1))
            if cam is None:
                return self._json({"error": "no camera %s" % match.group(1)}, 404)

            # The body: {"values": {id: value}} or {"reset": true}.
            length = int(self.headers.get("Content-Length") or 0)
            try:
                body = json.loads(self.rfile.read(length) or b"{}") if length else {}
            except ValueError:
                return self._json({"error": "send JSON"}, 400)
            values = body.get("values") or {}
            if not isinstance(values, dict) or not all(isinstance(k, str) for k in values):
                return self._json({"error": "values must be {setting id: value}"}, 400)
            return self._json(cam.set_params(values, reset=bool(body.get("reset"))))
        except (BrokenPipeError, ConnectionResetError):
            pass

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
    parser = argparse.ArgumentParser(description="SPOT Vision: stream the Orin's cameras")
    parser.add_argument("--port", type=int, default=8090)
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--zed-python", default=sys.executable,
                        help="a Python with pyzed (the ZED SDK's)")
    parser.add_argument("--realsense-python", default=sys.executable,
                        help="a Python with pyrealsense2 (2.54 for the L515)")
    parser.add_argument("--dvs-python", default=sys.executable,
                        help="a Python with dv-processing (iniVation's, for the DVXplorer)")
    parser.add_argument("--uvc-python", default=sys.executable, help="a Python with OpenCV")
    parser.add_argument("--demo", action="store_true",
                        help="also a test pattern camera (to try the Vision tab without one)")
    args = parser.parse_args()

    # The cameras, each kind with its Python.
    global DEMO
    DEMO = args.demo
    Handler.vision = Vision({"zed": args.zed_python, "realsense": args.realsense_python,
                             "dvs": args.dvs_python, "uvc": args.uvc_python,
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
