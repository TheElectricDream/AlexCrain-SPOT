"""One camera, run by spot_vision.py in a process of its own.

Run with the Python whose library the camera needs: the ZED SDK's
pyzed in the system Python, the RealSense L515's pyrealsense2 2.54 in
its own environment, iniVation's dv-processing for the DVXplorer,
python-libusb1 and pyusb for the ICI 9320P thermal camera (its SDK,
ici9320, is in vendor/), OpenCV and v4l2-ctl for any other USB camera.
Python 3.8+, numpy and OpenCV.

    python vision_worker.py zed [serial]
    python vision_worker.py realsense [serial]
    python vision_worker.py dvs [name]
    python vision_worker.py thermal
    python vision_worker.py uvc /dev/videoN
    python vision_worker.py demo    (a test pattern: the Vision tab
                                     without a camera)

Commands come on standard input, one JSON object a line:
    {"views": ["left", "depth"], "fps": 15, "quality": 80,
     "width": 960}                  what to send (again when it changes)
    {"params": {"<id>": value, ...}}  settings to apply
    {"record": {"folder": "/path"}}   record into this folder (new)
    {"record": null}                  stop recording
    {"quit": true}
Frames and news go to standard output:
    b"F <view> <length>\\n" followed by that many bytes of JPEG
    b"S <json>\\n"  {"state": "open" | "streaming" | "busy" | "error",
                    "detail": "...", "model", "serial", "housing_c"}
    b"P <json>\\n"  {"params": [setting, ...], "errors": {"<id>":
                    "why"}}: the camera's settings, after a change
    b"R <json>\\n"  {"recording": bool, "frames", "bytes", "files",
                    "error", "note", "done"}: a recording's progress
                    (each RECORD_REPORT s), and "done" once closed
A setting: {"id", "label", "group", "type": "range" | "bool" | "enum",
"min", "max", "step", "value", "default", "options": [[value, label],
...], "reopen": (changing it reopens the camera), "help", "unit",
"inactive"}.

The camera is opened when the first view (or a recording) is asked for
and closed when the process ends; spot_vision.py ends it a few seconds
after the last viewer leaves and the recording ends, so experiments on
the Orin can have the camera the rest of the time.

A recording is the camera's own data in its library's format where it
has one (the ZED's SVO, the RealSense's bag, the DVXplorer's AEDAT4
events; the thermal camera's raw counts; other cameras as MJPEG
video), whatever is being watched, and frames.csv: a row per frame (per
event batch for the DVXplorer) with its time on the Orin's monotonic
clock (orin_time [s], the clock spot_vision.py's experiment clock is
fitted to) and the camera's own time stamp.
"""
import json
import math
import os
import re
import select
import subprocess
import sys
import time

import cv2
import numpy as np

OUT = sys.stdout.buffer

# [s] frames thrown away after a camera starts: its auto-exposure
# settles (the L515's first colour frames are black).
WARMUP = 1.0

TOOL_TIMEOUT = 5         # [s] for v4l2-ctl
FRAME_WAIT_MS = 2000     # [ms] for the L515's next frames
RECORD_REPORT = 1.0      # [s] between a recording's progress reports
FRAME_LOG = "frames.csv"


# Talking to spot_vision.py


def say(state, detail=None, **extra):
    """Send the camera's state (and why, and any extra facts)."""
    msg = dict(extra, state=state)
    if detail:
        msg["detail"] = detail
    OUT.write(b"S " + json.dumps(msg).encode() + b"\n")
    OUT.flush()


def report(params, errors=None):
    """Send the camera's settings list, and why any change failed."""
    OUT.write(b"P " + json.dumps({"params": params, "errors": errors or {}},
                                 default=float).encode() + b"\n")
    OUT.flush()


def send(view, image, quality, width):
    """Send a view's frame as JPEG, scaled down to width (0: full)."""
    if width and image.shape[1] > width:
        h = int(round(image.shape[0] * width / image.shape[1]))
        image = cv2.resize(image, (width, h), interpolation=cv2.INTER_AREA)
    ok, jpeg = cv2.imencode(".jpg", image, [cv2.IMWRITE_JPEG_QUALITY, int(quality)])
    if ok:
        data = jpeg.tobytes()
        OUT.write(b"F %s %d\n" % (view.encode(), len(data)))
        OUT.write(data)
        OUT.flush()


def setting(setting_id, label, group, kind, value, default=None, **kw):
    """Return a setting for the settings list (see the module's help).

    Args:
        setting_id: Its id.
        label: Its name on the page.
        group: The group it is shown in.
        kind: "range", "bool" or "enum".
        value: Its value now.
        default: Its default.
        **kw: min, max, step, options, reopen, help, unit, inactive
            (None: left out).
    """
    entry = {"id": setting_id, "label": label, "group": group, "type": kind, "value": value,
             "default": default}
    entry.update({k: v for k, v in kw.items() if v is not None})
    return entry


class Commands:
    """The latest commands from standard input, read without blocking.

    settings: what to send; pending: settings changes not yet applied.
    """

    def __init__(self):
        """Start with nothing to send, and not recording."""
        self.settings = {"views": [], "fps": 15, "quality": 80, "width": 0}
        self.pending = {}
        self.record = None    # {"folder"} while asked to record
        self.quit = False
        self._buf = b""

    def poll(self, timeout=0.0):
        """Read whatever has come; return whether what to send changed.

        Args:
            timeout: Seconds to wait for the first input.
        """
        changed = False
        while True:
            ready, _, _ = select.select([sys.stdin], [], [], timeout)
            timeout = 0.0
            if not ready:
                return changed

            # End of input: spot_vision.py has gone.
            chunk = os.read(sys.stdin.fileno(), 65536)
            if not chunk:
                self.quit = True
                return changed

            # Each whole line: a JSON command.
            self._buf += chunk
            while b"\n" in self._buf:
                line, self._buf = self._buf.split(b"\n", 1)
                try:
                    msg = json.loads(line)
                except ValueError:
                    continue
                if msg.get("quit"):
                    self.quit = True
                    return changed
                if isinstance(msg.get("params"), dict):
                    self.pending.update(msg["params"])
                if "record" in msg:
                    record = msg["record"]
                    self.record = record if isinstance(record, dict) and record.get(
                        "folder") else None
                for k in ("views", "fps", "quality", "width"):
                    if k in msg:
                        self.settings[k] = msg[k]
                        changed = True

    def take(self):
        """Return the pending settings changes, and clear them."""
        pending, self.pending = self.pending, {}
        return pending

    def due(self, last):
        """Return whether the next frame is due (at the wanted fps)."""
        return time.time() - last >= 1.0 / max(1, float(self.settings["fps"]))

    def wanted(self):
        """Return whether anything is wanted: a view or a recording."""
        return bool(self.settings["views"]) or self.record is not None


class Recording:
    """A recording in progress: its folder, frame log and progress.

    The camera's own files go in the folder too (path(name)); the frame
    log is frames.csv, with the columns given. Progress goes to
    spot_vision.py as R lines.
    """

    def __init__(self, folder, columns):
        """Start one: the folder, and the frame log's header.

        Args:
            folder: Where it goes (made if missing).
            columns: The frame log's column names (after "frame", the
                index log() adds).
        """
        self.folder = folder
        os.makedirs(folder, exist_ok=True)
        self._log = open(os.path.join(folder, FRAME_LOG), "w")
        self._log.write(",".join(["frame"] + list(columns)) + "\n")
        self.frames = 0
        self.error = None
        self.note = None      # something to know, not a failure
        self._reported = 0.0
        self.report()

    def path(self, name):
        """Return the path of one of its files."""
        return os.path.join(self.folder, name)

    def log(self, *values):
        """Add a frame's row to the frame log (its index first)."""
        self._log.write(",".join([str(self.frames)] + [
            "" if v is None else ("%.6f" % v if isinstance(v, float) else str(v))
            for v in values]) + "\n")
        self.frames += 1
        if time.time() - self._reported >= RECORD_REPORT:
            self.report()

    def report(self, done=False):
        """Send the recording's progress (done: it is closed)."""
        self._reported = time.time()
        files, size = [], 0
        for name in sorted(os.listdir(self.folder)):
            try:
                size += os.path.getsize(os.path.join(self.folder, name))
                files.append(name)
            except OSError:
                pass
        OUT.write(b"R " + json.dumps({"recording": not done, "frames": self.frames,
                                      "bytes": size, "files": files, "error": self.error,
                                      "note": self.note, "done": done}).encode() + b"\n")
        OUT.flush()

    def fail(self, why):
        """Note why the recording went wrong (it goes on if it can)."""
        self.error = why
        self.report()

    def close_log(self):
        """Close the frame log (no more frames), to finish its rows."""
        if not self._log.closed:
            self._log.close()

    def close(self):
        """Close the frame log and send the last report."""
        self.close_log()
        self.report(done=True)


def not_recording(folder, why):
    """Tell spot_vision.py a recording couldn't start, and why."""
    OUT.write(b"R " + json.dumps({"recording": False, "frames": 0, "bytes": 0, "files": [],
                                  "error": why, "done": True, "folder": folder}).encode() + b"\n")
    OUT.flush()


def frame_writer(rec, name, size, fps):
    """Open an MJPEG .avi in a recording, for 8-bit pictures.

    Args:
        rec: The Recording.
        name: The file's name.
        size: (width, height).
        fps: The nominal frame rate (the frame log has the real times).

    Returns:
        A cv2.VideoWriter, or None (the recording notes why).
    """
    writer = cv2.VideoWriter(rec.path(name), cv2.VideoWriter_fourcc(*"MJPG"), float(fps),
                             (int(size[0]), int(size[1])))
    if not writer.isOpened():
        rec.fail("OpenCV couldn't open %s for writing" % name)
        return None
    return writer


def host_to_orin(seconds):
    """Return a time on this computer's wall clock as orin_time.

    The cameras' libraries stamp frames on the wall clock; the frame
    log's orin_time is the monotonic clock. None if the time is more
    than a minute from now (a camera's own clock, not the host's).

    Args:
        seconds: The wall-clock time [s since 1970].
    """
    now = time.monotonic()
    t = seconds - (time.time() - now)
    return t if abs(t - now) < 60 else None


def sync_recording(rec, cmd, start, stop=None):
    """Start or stop a recording as cmd.record asks.

    Args:
        rec: The Recording now, or None.
        cmd: The Commands.
        start: start(folder) -> a Recording, or None if it couldn't
            (it has said why).
        stop: stop(rec), called before it closes; or None.

    Returns:
        The Recording now, or None.
    """
    want = cmd.record["folder"] if cmd.record else None
    if rec is not None and rec.folder != want:
        if stop:
            try:
                stop(rec)
            except Exception as e:
                rec.error = rec.error or "%s: %s" % (type(e).__name__, e)
        rec.close()
        rec = None
    if want and rec is None:
        rec = start(want)
        if rec is None:
            cmd.record = None   # said why; not tried again each loop
    return rec


def colour_depth(depth_m, near=0.2, far=5.0):
    """Return depth [m] as colour: near red, far blue, none black."""
    d = np.clip((depth_m - near) / max(far - near, 1e-3), 0, 1)
    img = cv2.applyColorMap((255 * (1 - d)).astype(np.uint8), cv2.COLORMAP_JET)
    img[depth_m <= 0] = 0
    return img


def as_number(v, default=0.0):
    """Return v as a float, or default."""
    try:
        return float(v)
    except (TypeError, ValueError):
        return default


# ZED (pyzed)


def run_zed(serial, cmd):
    """Run a ZED camera until told to quit.

    Args:
        serial: Its serial number, or None for the first.
        cmd: The Commands.
    """
    import pyzed.sl as sl
    VIEWS = {"left": sl.VIEW.LEFT, "right": sl.VIEW.RIGHT, "side": sl.VIEW.SIDE_BY_SIDE,
             "depth": sl.VIEW.DEPTH}
    V = sl.VIDEO_SETTINGS

    # The SDK's video settings (live): id, label, attribute, min, max,
    # step, default, help.
    VIDEO = [("brightness", "Brightness", "BRIGHTNESS", 0, 8, 1, 4, None),
             ("contrast", "Contrast", "CONTRAST", 0, 8, 1, 4, None),
             ("hue", "Hue", "HUE", 0, 11, 1, 0, None),
             ("saturation", "Saturation", "SATURATION", 0, 8, 1, 4, None),
             ("sharpness", "Sharpness", "SHARPNESS", 0, 8, 1, 4, None),
             ("gamma", "Gamma", "GAMMA", 1, 9, 1, 8, None),
             ("gain", "Gain", "GAIN", 0, 100, 1, 50, "Turns automatic exposure and gain off"),
             ("exposure", "Exposure", "EXPOSURE", 0, 100, 1, 50,
              "Percentage of the frame time; turns automatic exposure and gain off"),
             ("whitebalance_temperature", "White Balance", "WHITEBALANCE_TEMPERATURE", 2800,
              6500, 100, 4600, "Colour temperature [K]; turns automatic white balance off")]
    FLAGS = [("aec_agc", "Auto Exposure and Gain", "AEC_AGC", True),
             ("whitebalance_auto", "Auto White Balance", "WHITEBALANCE_AUTO", True),
             ("led_status", "Front LED", "LED_STATUS", True)]

    # The resolutions and depth modes this SDK has.
    RESOLUTIONS = [(n, label) for n, label in (("HD2K", "2208 × 1242"), ("HD1080", "1920 × 1080"),
                                               ("HD720", "1280 × 720"), ("VGA", "672 × 376"))
                   if hasattr(sl.RESOLUTION, n)]
    DEPTH_MODES = [n for n in ("NONE", "PERFORMANCE", "QUALITY", "ULTRA", "NEURAL_LIGHT",
                               "NEURAL", "NEURAL_PLUS") if hasattr(sl.DEPTH_MODE, n)]

    # The settings: defaults, then what was saved for it (the live ones
    # applied once open).
    # SVO compression modes this SDK has (H264 and H265 use the Orin's
    # hardware encoder).
    COMPRESSIONS = [n for n in ("H264", "H265", "LOSSLESS")
                    if hasattr(getattr(sl, "SVO_COMPRESSION_MODE", None), n)]

    cfg = {"resolution": "HD720", "camera_fps": 30, "depth_mode": "PERFORMANCE",
           "depth_min": 0.3, "depth_max": 20.0, "confidence": 95, "texture_confidence": 100,
           "fill_mode": False, "svo_compression": "H264"}
    cfg.update({k: v for k, v in cmd.take().items()})
    live = {k: v for k, v in cfg.items() if k in {x[0] for x in VIDEO} | {x[0] for x in FLAGS}}

    def get(attr):
        """Return a video setting's value now."""
        result = cam.get_camera_settings(getattr(V, attr))
        return result[1] if isinstance(result, tuple) else result

    def params():
        """Return the camera's settings list."""
        out = []

        # Image: the live video settings and flags this camera has.
        for setting_id, label, attr, lo, hi, step, default, tip in VIDEO:
            if hasattr(V, attr):
                out.append(setting(setting_id, label, "Image", "range", get(attr), default,
                                   min=lo, max=hi, step=step, help=tip))
        for setting_id, label, attr, default in FLAGS:
            if hasattr(V, attr):
                out.append(setting(setting_id, label, "Image", "bool", bool(get(attr)), default))

        # Camera (reopens it) and depth.
        out += [
            setting("resolution", "Resolution", "Camera", "enum", cfg["resolution"], "HD720",
                    options=RESOLUTIONS, reopen=True),
            setting("camera_fps", "Camera Frame Rate", "Camera", "enum", int(cfg["camera_fps"]),
                    30, options=[[f, "%d fps" % f] for f in (15, 30, 60, 100)], reopen=True,
                    help="The fastest the resolution allows: HD2K 15, HD1080 30, HD720 60, "
                         "VGA 100"),
            setting("depth_mode", "Depth Mode", "Camera", "enum", cfg["depth_mode"],
                    "PERFORMANCE",
                    options=[[m, m.replace("_", " ").title()] for m in DEPTH_MODES], reopen=True),
            setting("depth_min", "Nearest Depth", "Camera", "range", cfg["depth_min"], 0.3,
                    min=0.1, max=3.0, step=0.05, unit="m", reopen=True),
            setting("depth_max", "Farthest Depth", "Camera", "range", cfg["depth_max"], 20.0,
                    min=1.0, max=40.0, step=0.5, unit="m", reopen=True),
            setting("confidence", "Depth Confidence", "Depth", "range", cfg["confidence"], 95,
                    min=1, max=100, step=1, help="Lower: fewer depth points, but surer ones"),
            setting("texture_confidence", "Texture Confidence", "Depth", "range",
                    cfg["texture_confidence"], 100, min=1, max=100, step=1,
                    help="Lower: drops depth on surfaces with little texture"),
            setting("fill_mode", "Fill Holes", "Depth", "bool", bool(cfg["fill_mode"]), False,
                    help="A dense depth map, the holes filled in"),
        ]
        if COMPRESSIONS:
            out.append(setting(
                "svo_compression", "SVO Compression", "Recording", "enum",
                cfg["svo_compression"], "H264",
                options=[[m, {"H264": "H.264", "H265": "H.265 (smaller)",
                              "LOSSLESS": "Lossless (large)"}[m]] for m in COMPRESSIONS],
                help="How an experiment's recording (an SVO file: both images, every "
                     "frame, readable by the ZED SDK, which can compute depth from it "
                     "again) is compressed"))
        return out

    def open_camera():
        """Open the camera with its settings; return the SDK's code."""
        init = sl.InitParameters()
        init.camera_resolution = getattr(sl.RESOLUTION, cfg["resolution"], sl.RESOLUTION.HD720)
        init.camera_fps = int(cfg["camera_fps"])
        init.depth_mode = getattr(sl.DEPTH_MODE, cfg["depth_mode"], sl.DEPTH_MODE.PERFORMANCE)
        init.coordinate_units = sl.UNIT.METER
        init.depth_minimum_distance = float(cfg["depth_min"])
        init.depth_maximum_distance = float(cfg["depth_max"])
        init.sdk_verbose = 0
        if serial:
            init.set_from_serial_number(int(serial))
        return cam.open(init)

    def apply_live(values):
        """Apply live video settings; return {id: why} for failures.

        The SDK raises for a setting this camera hasn't.
        """
        errors = {}
        for k, v in values.items():
            attr = {x[0]: x[2]
                    for x in VIDEO + [(flag[0], None, flag[2]) for flag in FLAGS]}.get(k)
            if attr and hasattr(V, attr):
                try:
                    cam.set_camera_settings(getattr(V, attr),
                                            int(v) if not isinstance(v, bool) else int(v))
                except Exception as e:
                    errors[k] = str(e)
        return errors

    def runtime_params():
        """Return the depth's runtime parameters from the settings."""
        runtime = sl.RuntimeParameters()
        runtime.confidence_threshold = int(cfg["confidence"])
        runtime.texture_confidence_threshold = int(cfg["texture_confidence"])
        if hasattr(runtime, "enable_fill_mode"):
            runtime.enable_fill_mode = bool(cfg["fill_mode"])
        return runtime

    # Open it (in use elsewhere: busy).
    cam = sl.Camera()
    err = open_camera()
    if err != sl.ERROR_CODE.SUCCESS:
        text = str(err)
        busy = any(k in text.upper() for k in ("IN USE", "IN_USE", "BUSY"))
        say("busy" if busy else "error", "the ZED SDK couldn't open the camera: %s" % text)
        return
    info = cam.get_camera_information()
    say("open", model=str(info.camera_model), serial=info.serial_number)
    errors = apply_live(live)
    report(params(), errors)

    def start_recording(folder):
        """Record every frame into an SVO file; return the Recording."""
        version = str(getattr(sl.Camera, "get_sdk_version", lambda: "3")())
        major_minor = tuple(int(n) for n in re.findall(r"\d+", version)[:2])
        name = "zed.svo2" if major_minor >= (4, 1) else "zed.svo"
        rec = Recording(folder, ["orin_time", "arrival_time", "camera_time_ns"])
        mode = getattr(sl.SVO_COMPRESSION_MODE, str(cfg["svo_compression"]),
                       sl.SVO_COMPRESSION_MODE.H264)
        err = cam.enable_recording(sl.RecordingParameters(rec.path(name), mode))
        if err != sl.ERROR_CODE.SUCCESS:
            rec.fail("the ZED SDK couldn't record: %s" % err)
            rec.close()
            return None
        return rec

    mat = sl.Mat()
    runtime = runtime_params()
    last, warm = 0.0, time.time() + WARMUP
    rec = None
    try:
        while not cmd.quit:
            # Settings changes: camera ones reopen it, the others apply
            # live.
            cmd.poll()
            changes = cmd.take()
            if changes:
                reopen = {k: v for k, v in changes.items()
                          if k in ("resolution", "camera_fps", "depth_mode", "depth_min",
                                   "depth_max")}
                errors = {}
                if rec and reopen:
                    # Reopening would end the SVO file mid-run.
                    errors = {k: "not while recording (it reopens the camera)" for k in reopen}
                    changes = {k: v for k, v in changes.items() if k not in reopen}
                    reopen = {}
                cfg.update(changes)
                if reopen:
                    cam.close()
                    err = open_camera()
                    if err != sl.ERROR_CODE.SUCCESS:
                        say("error", "the ZED SDK couldn't reopen the camera: %s" % err)
                        return
                    errors = apply_live({k: v for k, v in cfg.items()
                                         if k in live or k in {x[0] for x in VIDEO}})
                    warm = time.time() + WARMUP
                errors.update(apply_live({k: v for k, v in changes.items() if k not in reopen}))
                runtime = runtime_params()
                report(params(), errors)

            # Recording: every grabbed frame goes into the SVO file.
            rec = sync_recording(rec, cmd, start_recording, lambda _: cam.disable_recording())

            # Grab; send the views wanted when a frame is due.
            wanted = cmd.settings
            views = [v for v in wanted["views"] if v in VIEWS]
            if cam.grab(runtime) != sl.ERROR_CODE.SUCCESS:
                time.sleep(0.01)
                continue
            if rec:
                stamp = cam.get_timestamp(sl.TIME_REFERENCE.IMAGE).get_nanoseconds()
                arrival = time.monotonic()
                rec.log(host_to_orin(stamp / 1e9) or arrival, arrival, stamp)
            now = time.time()
            if not views or now < warm or not cmd.due(last):
                continue
            last = now
            for view in views:
                cam.retrieve_image(mat, VIEWS[view])
                img = mat.get_data()
                send(view, cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)
                     if img.ndim == 3 and img.shape[2] == 4 else img,
                     wanted["quality"], wanted["width"])
    finally:
        if rec:
            cam.disable_recording()
            rec.close()
        cam.close()


# RealSense L515 (pyrealsense2 2.54)


def run_realsense(serial, cmd):
    """Run a RealSense camera until told to quit.

    Args:
        serial: Its serial number, or None for the first.
        cmd: The Commands.
    """
    import pyrealsense2 as rs

    # Each view: the stream it needs, its format, and the modes to try
    # when none is chosen (fewer over USB 2).
    STREAMS = {
        "color": (rs.stream.color, rs.format.bgr8,
                  [(1280, 720, 30), (960, 540, 30), (640, 480, 30), (640, 360, 30)]),
        "depth": (rs.stream.depth, rs.format.z16,
                  [(1024, 768, 30), (640, 480, 30), (320, 240, 30)]),
        "infrared": (rs.stream.infrared, rs.format.y8,
                     [(1024, 768, 30), (640, 480, 30), (320, 240, 30)]),
    }

    # The device.
    ctx = rs.context()
    devices = [device for device in ctx.query_devices()
               if not serial
               or device.get_info(rs.camera_info.serial_number).lower() == str(serial).lower()]
    if not devices:
        say("error", "librealsense sees no RealSense%s (the L515 needs librealsense 2.54 or older)"
            % (" " + serial if serial else ""))
        return
    dev = devices[0]

    # What was saved: display settings, chosen modes, and the device's
    # own options.
    saved = cmd.take()
    display = {"depth_near": as_number(saved.pop("depth_near", 0.2), 0.2),
               "depth_far": as_number(saved.pop("depth_far", 5.0), 5.0)}
    modes = {k: saved.pop(k) for k in list(saved) if k.startswith("mode.")}

    def nice(name):
        """Return an option's name as a label."""
        return name.replace("option.", "").replace("_", " ").title()

    def options():
        """Return {id: (sensor, option)} for every writable option."""
        out = {}
        for sen in dev.query_sensors():
            sname = sen.get_info(rs.camera_info.name)
            for opt in sen.get_supported_options():
                try:
                    if sen.is_option_read_only(opt):
                        continue
                except RuntimeError:
                    continue
                out["%s/%s" % (sname, str(opt).replace("option.", ""))] = (sen, opt)
        return out

    def stream_modes(view):
        """Return a view's modes [(w, h, fps)], largest first."""
        stream, fmt, _ = STREAMS[view]
        found = set()
        for sen in dev.query_sensors():
            for profile in sen.get_stream_profiles():
                if (profile.stream_type() == stream and profile.format() == fmt
                        and profile.is_video_stream_profile()):
                    vprofile = profile.as_video_stream_profile()
                    found.add((vprofile.width(), vprofile.height(), vprofile.fps()))
        return sorted(found, reverse=True)

    def params():
        """Return the camera's settings list."""
        out = []

        # Each stream's mode (reopens it).
        for view in ("color", "depth", "infrared"):
            found_modes = stream_modes(view)
            if found_modes:
                out.append(setting(
                    "mode." + view,
                    {"color": "Colour Mode", "depth": "Depth Mode",
                     "infrared": "Infrared Mode"}[view],
                    "Streams", "enum", modes.get("mode." + view, ""), "",
                    options=[["", "Automatic"]] + [["%dx%d@%d" % m, "%d × %d, %d fps" % m]
                                                   for m in found_modes],
                    reopen=True))

        # The depth view's colour scale.
        out += [setting("depth_near", "Depth Colours From", "Display", "range",
                        display["depth_near"], 0.2, min=0.0, max=5.0, step=0.05, unit="m",
                        help="The depth view's colour scale: red at this distance"),
                setting("depth_far", "Depth Colours To", "Display", "range", display["depth_far"],
                        5.0, min=0.5, max=10.0, step=0.1, unit="m",
                        help="... and blue at this one")]

        # The device's options: on/off, a few named values, or a range.
        for option_id, (sen, opt) in sorted(options().items()):
            try:
                rng = sen.get_option_range(opt)
                value = sen.get_option(opt)
                desc = sen.get_option_description(opt)
            except RuntimeError:
                continue
            group = option_id.split("/")[0]
            label = nice(str(opt))
            if rng.min == 0 and rng.max == 1 and rng.step == 1:
                out.append(setting(option_id, label, group, "bool", bool(value), bool(rng.default),
                                   help=desc))
                continue

            # A few values: named, if the device names them.
            names = []
            if rng.step >= 1 and (rng.max - rng.min) / rng.step <= 16:
                for k in range(int(round((rng.max - rng.min) / rng.step)) + 1):
                    val = rng.min + k * rng.step
                    try:
                        names.append([val, sen.get_option_value_description(opt, val)
                                      or ("%g" % val)])
                    except RuntimeError:
                        names = []
                        break
            if names and any(entry[1] != "%g" % entry[0] for entry in names):
                out.append(setting(option_id, label, group, "enum", value, rng.default,
                                   options=names, help=desc))
            else:
                out.append(setting(option_id, label, group, "range", value, rng.default,
                                   min=rng.min, max=rng.max, step=rng.step or 1, help=desc))
        return out

    def apply(values):
        """Apply display settings and options; return {id: why}."""
        errors = {}
        opts = options()
        for k, v in values.items():
            if k in display:
                display[k] = as_number(v, display[k])
            elif k in opts:
                sen, opt = opts[k]
                try:
                    sen.set_option(opt, float(v))
                except RuntimeError as e:
                    errors[k] = str(e)
        return errors

    # A recording keeps the colour and depth streams (and any other
    # being watched as it starts) in a bag file, librealsense's own
    # format.
    REC_STREAMS = ["color", "depth"]

    def stamp(frames, arrival):
        """Return a frameset's orin_time, from its global time stamp.

        librealsense's global time domain (and the system one) is this
        computer's clock [ms]; otherwise its arrival time.
        """
        domain = str(frames.get_frame_timestamp_domain()).lower()
        if "global" in domain or "system" in domain:
            return host_to_orin(frames.get_timestamp() / 1000.0) or arrival
        return arrival

    pipe, running, last, warm = None, None, 0.0, 0.0
    rec, segment, bag = None, 0, None
    errors = apply(saved)
    report(params(), errors)
    try:
        while not cmd.quit:
            # Settings changes: a new mode restarts the streams (the
            # settings report follows), except while recording.
            changed = cmd.poll(0.0 if pipe else 0.2)
            changes = cmd.take()
            restart = False
            if changes:
                chosen = {k: changes.pop(k) for k in [k for k in changes if k.startswith("mode.")]}
                refused = {}
                if rec and chosen:
                    refused = {k: "not while recording (it restarts the streams)" for k in chosen}
                    chosen = {}
                modes.update(chosen)
                restart = bool(chosen)
                errors = apply(changes)
                errors.update(refused)
                if not restart:
                    report(params(), errors)
            wanted = cmd.settings
            views = sorted(v for v in wanted["views"] if v in STREAMS)

            # Recording: starting or stopping it restarts the streams
            # (stopping them closes the bag file).
            want = cmd.record["folder"] if cmd.record else None
            if (rec.folder if rec else None) != want:
                if pipe:
                    pipe.stop()
                    pipe, running = None, None
                if rec:
                    rec.close()
                rec, segment = None, 0
                if want:
                    rec = Recording(want, ["orin_time", "arrival_time", "camera_time_ms", "file",
                                           "color_frame", "depth_frame"])
                restart = True

            # The streams: the views', and a recording's. While it
            # records they stay as they are (a view it lacks shows
            # nothing).
            streams = sorted(set(views) | set(REC_STREAMS)) if rec else views
            if rec and pipe is not None and not restart:
                streams = running

            # (Re)start the streams.
            if restart or streams != running or changed and pipe is None:
                if pipe:
                    pipe.stop()
                    pipe = None
                running = streams
                if not streams:
                    if restart and not rec:
                        report(params())
                    continue
                if rec:
                    segment += 1
                    bag = "realsense_%d.bag" % segment

                # The chosen mode of each stream, else the first that
                # works.
                why = ""
                for attempt in range(3):
                    cfg = rs.config()
                    cfg.enable_device(dev.get_info(rs.camera_info.serial_number))
                    for view in streams:
                        stream, fmt, fallback = STREAMS[view]
                        chosen = modes.get("mode." + view)
                        match = re.match(r"(\d+)x(\d+)@(\d+)$", chosen or "")
                        if match and attempt == 0:
                            width, height, rate = (int(match.group(1)), int(match.group(2)),
                                                   int(match.group(3)))
                        else:
                            width, height, rate = fallback[min(attempt, len(fallback) - 1)]
                        cfg.enable_stream(stream, width, height, fmt, rate)
                    if rec:
                        cfg.enable_record_to_file(rec.path(bag))
                    try:
                        candidate = rs.pipeline(ctx)
                        candidate.start(cfg)
                        pipe = candidate
                        break
                    except RuntimeError as e:
                        why = str(e)
                if pipe is None:
                    say("busy" if "busy" in why.lower() else "error",
                        "librealsense couldn't start %s: %s" % (", ".join(streams), why))
                    if rec:
                        rec.fail("librealsense couldn't start the streams: %s" % why)
                        rec.close()
                        rec, cmd.record = None, None
                    running = None
                    time.sleep(2)
                    continue
                if restart and not rec:
                    report(params())
                say("streaming", views=streams, model=dev.get_info(rs.camera_info.name),
                    serial=dev.get_info(rs.camera_info.serial_number))
                last, warm = 0.0, time.time() + WARMUP
            if not pipe:
                continue

            # The next frames (logged when recording); send the views
            # when a frame is due.
            try:
                frames = pipe.wait_for_frames(FRAME_WAIT_MS)
            except RuntimeError as e:
                say("error", "no frames from the L515: %s" % e)
                continue
            if rec:
                arrival = time.monotonic()
                color, depth = frames.get_color_frame(), frames.get_depth_frame()
                rec.log(stamp(frames, arrival), arrival, frames.get_timestamp(), bag,
                        color.get_frame_number() if color else None,
                        depth.get_frame_number() if depth else None)
            now = time.time()
            if now < warm or not cmd.due(last):
                continue
            last = now
            scale = dev.first_depth_sensor().get_depth_scale() if "depth" in running else 0.00025
            for view in views:
                if view not in running:
                    continue
                if view == "color":
                    frame = frames.get_color_frame()
                    if frame:
                        send(view, np.asanyarray(frame.get_data()), wanted["quality"],
                             wanted["width"])
                elif view == "depth":
                    frame = frames.get_depth_frame()
                    if frame:
                        img = colour_depth(np.asanyarray(frame.get_data()).astype(np.float32)
                                           * scale, display["depth_near"], display["depth_far"])
                        send(view, img, wanted["quality"], wanted["width"])
                elif view == "infrared":
                    frame = frames.get_infrared_frame()
                    if frame:
                        send(view, np.asanyarray(frame.get_data()), wanted["quality"],
                             wanted["width"])
    finally:
        if pipe:
            pipe.stop()
        if rec:
            rec.close()

# DVXplorer Micro (iniVation dv-processing)


def run_dvs(name, cmd):
    """Run the event camera until told to quit.

    Events drawn over the last time window ("events": positive in one
    colour, negative in another), or summed with a decay
    ("accumulated"). Its settings are dv-processing's CameraCapture
    setters, used when this version has them (they only set: the
    values shown are what was last set).

    Args:
        name: The camera's name, or None for the first.
        cmd: The Commands.
    """
    import dv_processing as dv
    CC = dv.io.CameraCapture

    # Open it (busy, or none plugged in: say so).
    try:
        cap = CC(name or "")
    except Exception as e:
        text = str(e)
        say("busy" if "busy" in text.lower() or "claim" in text.lower() else "error",
            "dv-processing couldn't open the camera: %s" % text)
        return
    if not cap.isEventStreamAvailable():
        say("error", "this camera has no event stream")
        return
    res = cap.getEventResolution()
    W, H = (int(res[0]), int(res[1])) if res is not None else (640, 480)
    say("open", model=getattr(cap, "getCameraName", lambda: "DVXplorer")(), serial=name)

    def members(enum):
        """Return an enum's value names (capitalised, not callable)."""
        return [m for m in dir(enum) if not m.startswith("_") and m[0].isupper()
                and not callable(getattr(enum, m, None))] if enum else []

    # The settings this version has, and their values.
    sens = getattr(CC, "BiasSensitivity", None)
    efps = getattr(CC, "DVXeFPS", None)
    cfg = {"window_ms": 33, "decay_ms": 200, "contrast": 1.0,
           "sensitivity": "Default" if sens and hasattr(sens, "Default") else None,
           "global_hold": True, "global_reset": False, "efps": None}

    def params():
        """Return the camera's settings list."""
        # The display.
        out = [setting("window_ms", "Time Window", "Display", "range", cfg["window_ms"], 33,
                       min=2, max=500, step=1, unit="ms",
                       help="Events view: the events of this long are drawn in each frame"),
               setting("decay_ms", "Decay", "Display", "range", cfg["decay_ms"], 200, min=10,
                       max=5000, step=10, unit="ms",
                       help="Accumulated view: how fast the picture fades back to grey"),
               setting("contrast", "Contrast", "Display", "range", cfg["contrast"], 1.0, min=0.1,
                       max=10.0, step=0.1,
                       help="Accumulated view: the brightness change each event makes")]

        # The sensor's, where this version has the setter.
        if sens and hasattr(cap, "setDVSBiasSensitivity"):
            out.append(setting("sensitivity", "Bias Sensitivity", "Sensor", "enum",
                               cfg["sensitivity"], "Default",
                               options=[[m, re.sub(r"(?<!^)(?=[A-Z])", " ", m)]
                                        for m in members(sens)],
                               help="How large a brightness change makes an event (higher: "
                                    "more events, more noise)"))
        if hasattr(cap, "setDVSGlobalHold"):
            out.append(setting("global_hold", "Global Hold", "Sensor", "bool", cfg["global_hold"],
                               True))
        if hasattr(cap, "setDVXplorerGlobalReset"):
            out.append(setting("global_reset", "Global Reset", "Sensor", "bool",
                               cfg["global_reset"], False))
        if efps and hasattr(cap, "setDVXplorerEFPS"):
            names = members(efps)
            out.append(setting("efps", "Event Frame Rate", "Sensor", "enum",
                               cfg["efps"] or (names[0] if names else ""),
                               names[0] if names else "",
                               options=[[m, m.replace("EFPS_", "").replace("_", " ").title()]
                                        for m in names],
                               help="The sensor's readout: constant, or variable with the scene"))
        return out

    def apply(values):
        """Apply settings; return {id: why} for those that failed."""
        errors = {}
        for k, v in values.items():
            try:
                if k in ("window_ms", "decay_ms", "contrast"):
                    cfg[k] = as_number(v, cfg[k])
                elif k == "sensitivity" and sens:
                    cap.setDVSBiasSensitivity(getattr(sens, str(v)))
                    cfg[k] = v
                elif k == "global_hold":
                    cap.setDVSGlobalHold(bool(v))
                    cfg[k] = bool(v)
                elif k == "global_reset":
                    cap.setDVXplorerGlobalReset(bool(v))
                    cfg[k] = bool(v)
                elif k == "efps" and efps:
                    cap.setDVXplorerEFPS(getattr(efps, str(v)))
                    cfg[k] = v
            except Exception as e:
                errors[k] = str(e)
        return errors

    def start_recording(folder):
        """Record the events: AEDAT4 (dv-processing's), else raw.

        Returns:
            The Recording, with .writer (a MonoCameraWriter) or .raw (a
            file of the event array's rows, its layout in
            events_bin.json).
        """
        rec = Recording(folder, ["orin_time", "arrival_time", "first_us", "last_us", "events"])
        rec.writer = rec.raw = None
        try:
            rec.writer = dv.io.MonoCameraWriter(rec.path("events.aedat4"), cap)
        except Exception as e:
            rec.raw = open(rec.path("events.bin"), "wb")
            rec.note = ("no AEDAT4 writer (%s: %s): the events are in events.bin instead"
                        % (type(e).__name__, e))
        return rec

    def stop_recording(rec):
        """Close the events file (the AEDAT4 writer writes on close)."""
        if rec.raw:
            rec.raw.close()
        rec.writer = None

    def record_batch(rec, batch, ev):
        """Write an event batch; log it (at its newest event's time)."""
        arrival = time.monotonic()
        if rec.writer is not None:
            rec.writer.writeEvents(batch)
        else:
            if rec.frames == 0:
                with open(rec.path("events_bin.json"), "w") as f:
                    json.dump({"dtype": [list(field) for field in ev.dtype.descr],
                               "byteorder": sys.byteorder, "rows": "one per event"}, f)
            rec.raw.write(ev.tobytes())
        first, last_us = int(ev["timestamp"][0]), int(ev["timestamp"][-1])
        rec.log(host_to_orin(last_us / 1e6) or arrival, arrival, first, last_us, len(ev))

    errors = apply(cmd.take())
    report(params(), errors)
    rec = None
    recent = []                                     # event batches (numpy) of the last window
    acc = np.zeros((H, W), np.float32)
    acc_t = time.time()
    last, warm = 0.0, time.time() + 0.2
    while not cmd.quit and cap.isRunning():
        cmd.poll()
        changes = cmd.take()
        if changes:
            errors = apply(changes)
            report(params(), errors)
        rec = sync_recording(rec, cmd, start_recording, stop_recording)

        # The next events: recorded, kept for the window, and added to
        # the accumulated view (faded towards 0 first; + brighter, -
        # darker).
        batch = cap.getNextEventBatch()
        if batch is not None and len(batch) > 0:
            ev = batch.numpy()
            if rec:
                record_batch(rec, batch, ev)
            recent.append(ev)
            now = time.time()
            acc *= float(np.exp(-(now - acc_t) / max(cfg["decay_ms"] / 1000.0, 1e-3)))
            acc_t = now
            sign = np.where(ev["polarity"].astype(bool), cfg["contrast"],
                            -cfg["contrast"]).astype(np.float32)
            np.add.at(acc, (ev["y"].astype(np.intp), ev["x"].astype(np.intp)), sign)
        else:
            time.sleep(0.002)

        # Keep only the window.
        if recent:
            t_end = int(recent[-1]["timestamp"][-1])
            cutoff = t_end - int(cfg["window_ms"] * 1000)
            while recent and int(recent[0]["timestamp"][-1]) < cutoff:
                recent.pop(0)

        # Send the views wanted when a frame is due.
        wanted = cmd.settings
        now = time.time()
        if now < warm or not wanted["views"] or not cmd.due(last):
            continue
        last = now
        if "events" in wanted["views"]:
            # The window's events: positive blue-ish, negative red-ish
            # (BGR).
            img = np.full((H, W, 3), 24, np.uint8)
            if recent:
                ev = np.concatenate(recent)
                ev = ev[ev["timestamp"] >= cutoff]
                pos = ev["polarity"].astype(bool)
                img[ev["y"][pos], ev["x"][pos]] = (255, 170, 60)
                img[ev["y"][~pos], ev["x"][~pos]] = (60, 80, 255)
            send("events", img, wanted["quality"], wanted["width"])
        if "accumulated" in wanted["views"]:
            send("accumulated", np.clip(128 + 40 * acc, 0, 255).astype(np.uint8),
                 wanted["quality"], wanted["width"])
    if rec:
        stop_recording(rec)
        rec.close()
    if not cap.isRunning():
        say("error", "the event camera stopped (unplugged?)")


# ICI 9320P thermal camera (the ici9320 SDK, in vendor/)


THERMAL_LOST = 3.0          # [s] without a frame: the camera is gone
THERMAL_TEMP_PERIOD = 2.0   # [s] between housing temperature readings
THERMAL_MAX = 16383         # the sensor's largest count (14 bits)
THERMAL_MIN_SPAN = 20       # [counts] the narrowest automatic range
THERMAL_SMOOTH = 0.2        # how far the automatic range moves each frame
THERMAL_SHUTTER_SHOWN = 1.0  # [s] the readout says SHUTTER after one

# The palettes: id, label, and the OpenCV colour map (None: built here).
THERMAL_PALETTES = [("iron", "Iron", None), ("white_hot", "White Hot", None),
                    ("black_hot", "Black Hot", None), ("inferno", "Inferno", "COLORMAP_INFERNO"),
                    ("rainbow", "Rainbow", "COLORMAP_JET")]

# Iron (the SDK viewer's "ironbow"): black, purple, red, orange, yellow,
# white; (position, RGB).
IRON_STOPS = [(0.0, (0, 0, 0)), (0.15, (32, 0, 96)), (0.35, (140, 0, 160)),
              (0.55, (220, 50, 40)), (0.75, (255, 150, 0)), (0.9, (255, 230, 60)),
              (1.0, (255, 255, 255))]


def thermal_luts():
    """Return each palette as a lookup table: {id: 256 x 3 BGR}."""
    ramp = np.arange(256, dtype=np.uint8)
    pos = [s[0] for s in IRON_STOPS]
    iron_rgb = np.stack([np.interp(ramp / 255.0, pos, [s[1][k] for s in IRON_STOPS])
                         for k in range(3)], 1)
    luts = {"iron": iron_rgb[:, ::-1].astype(np.uint8),
            "white_hot": np.stack([ramp] * 3, 1),
            "black_hot": np.stack([255 - ramp] * 3, 1)}
    for pid, _, cmap in THERMAL_PALETTES:
        if cmap:
            luts[pid] = cv2.applyColorMap(ramp.reshape(256, 1), getattr(cv2, cmap)).reshape(256, 3)
    return luts


def run_thermal(_, cmd):
    """Run the ICI 9320P thermal camera until told to quit.

    The SDK loads the camera's firmware when it is fresh from power-up
    (it then comes back as another USB device) and streams 320 x 240
    raw 14-bit counts at 59.6 Hz, each frame timed on the sensor's own
    clock (the SDK's frame clock: ici9320.timing). The counts aren't
    temperatures (the SDK has no radiometric calibration yet): the view
    stretches a range of them over a palette, the picture's own
    (automatic) or one set by hand. The camera closes its shutter now
    and then to recalibrate (flat-field correction); the frames it sends
    while closed are all one value, and are skipped, so the picture
    holds meanwhile.

    Args:
        _: Unused (there is one such camera).
        cmd: The Commands.
    """
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "vendor"))
    try:
        import usb1
        from ici9320 import ICI9320, protocol as P, retime
    except ImportError as e:
        # The SDK needs python-libusb1, and pyusb for its package's
        # firmware loader.
        say("error", "this Python (%s) has no %s, which the ici9320 SDK needs (pip install "
            "--user libusb1 pyusb)" % (sys.executable,
                                       {"usb1": "python-libusb1", "usb": "pyusb"}.get(e.name,
                                                                                     e.name)))
        return

    luts = thermal_luts()
    cfg = {"palette": "iron", "auto_range": True, "range_lo": 5000, "range_hi": 5500,
           "equalize": False, "readout": True, "record_fps": 30}

    def params():
        """Return the camera's settings list."""
        auto = bool(cfg["auto_range"])
        return [
            setting("palette", "Palette", "Display", "enum", cfg["palette"], "iron",
                    options=[[pid, label] for pid, label, _ in THERMAL_PALETTES]),
            setting("auto_range", "Automatic Range", "Display", "bool", auto, True,
                    help="Stretch the palette over the picture's own counts (0.5 % to 99.5 %), "
                         "smoothed so it doesn't flicker; the readout shows it"),
            setting("range_lo", "Range From", "Display", "range", cfg["range_lo"], 5000, min=0,
                    max=THERMAL_MAX, step=1, unit="counts", inactive=auto or None,
                    help="Without the automatic range: the palette's coldest colour at these "
                         "raw counts"),
            setting("range_hi", "Range To", "Display", "range", cfg["range_hi"], 5500, min=0,
                    max=THERMAL_MAX, step=1, unit="counts", inactive=auto or None,
                    help="... and its hottest at these"),
            setting("equalize", "Histogram Equalisation", "Display", "bool",
                    bool(cfg["equalize"]), False,
                    help="Spread the colours by how many pixels have each count: more detail, "
                         "but no longer proportional to the counts"),
            setting("readout", "Readout", "Display", "bool", bool(cfg["readout"]), True,
                    help="The range in raw counts, the housing temperature and the shutter, "
                         "written on the picture"),
            setting("record_fps", "Recording Rate", "Recording", "enum", int(cfg["record_fps"]),
                    30, options=[[0, "Every Frame (59.6 fps)"], [30, "30 fps"], [15, "15 fps"],
                                 [10, "10 fps"], [5, "5 fps"]],
                    help="How many raw frames an experiment's recording keeps a second (each "
                         "is 150 kB: 9 MB/s for every frame)"),
        ]

    def apply(values):
        """Apply settings; return {id: why} for those that failed."""
        errors = {}
        for k, v in values.items():
            if k == "palette":
                if v in luts:
                    cfg[k] = v
                else:
                    errors[k] = "no palette %s" % v
            elif k in ("auto_range", "equalize", "readout"):
                cfg[k] = bool(v)
            elif k in ("range_lo", "range_hi"):
                cfg[k] = int(max(0, min(THERMAL_MAX, as_number(v, cfg[k]))))
            elif k == "record_fps":
                cfg[k] = int(max(0, min(60, as_number(v, cfg[k]))))
        return errors

    def start_recording(folder):
        """Record raw frames: thermal_raw.bin, 240 x 320 uint16 each.

        orin_time is the middle of the frame's readout; sensor_frame
        its number (a gap: frames not kept, or lost).
        """
        rec = Recording(folder, ["orin_time", "arrival_time", "sensor_frame", "shutter",
                                 "housing_c"])
        rec.raw = open(rec.path("thermal_raw.bin"), "wb")
        rec.last = -math.inf
        rec.arrivals = []   # every frame's arrival while recording, kept or not
        rec.kept = []       # each kept frame's place in arrivals
        return rec

    def stop_recording(rec):
        """Close the files; time the frames again over the whole run.

        ici9320.retime fits the sensor's clock to every frame's arrival
        at once, so the frames just after a lost one (which the live
        clock numbers late) and those that waited out a stall get their
        own times; the frame log's orin_time and sensor_frame are
        rewritten with them.
        """
        rec.raw.close()
        rec.close_log()
        if not rec.kept:
            return
        times, index = retime(rec.arrivals)
        log = rec.path(FRAME_LOG)
        with open(log) as src:
            rows = [line.rstrip("\n").split(",") for line in src]
        for row, place in zip(rows[1:], rec.kept):
            row[1] = "%.6f" % (times[place] - P.READOUT_S / 2)
            row[3] = str(int(index[place]))
        with open(log + ".tmp", "w") as dst:
            dst.writelines(",".join(row) + "\n" for row in rows)
        os.replace(log + ".tmp", log)

    # Open it (loading its firmware if it was just plugged in).
    cam = ICI9320()
    try:
        cam.open()
    except usb1.USBErrorAccess:
        say("error", "no permission to open the thermal camera: SPOT Vision's udev rule isn't "
            "installed (deploy_orin.sh says how)")
        return
    except usb1.USBErrorBusy:
        say("busy", "another program has the thermal camera")
        return
    except (RuntimeError, usb1.USBError) as e:
        say("error", "the ici9320 SDK couldn't open the camera: %s" % e)
        return
    model = "ICI 9320P, firmware %d.%d" % cam.fw_version
    say("open", model=model)
    errors = apply(cmd.take())
    report(params(), errors)

    # The view: the counts stretched over the range, coloured, with the
    # readout.
    span = None   # the automatic range [lo, hi], smoothed

    def render(frame, housing, shutter):
        """Return a frame (raw counts) as the thermal view (BGR)."""
        nonlocal span
        counts = frame.astype(np.float32)
        if cfg["auto_range"]:
            lo, hi = np.percentile(counts, (0.5, 99.5))
            hi = max(hi, lo + THERMAL_MIN_SPAN)
            if span is None:
                span = [lo, hi]
            else:
                span = [span[0] + THERMAL_SMOOTH * (lo - span[0]),
                        span[1] + THERMAL_SMOOTH * (hi - span[1])]
            lo, hi = span
        else:
            lo, hi = sorted((float(cfg["range_lo"]), float(cfg["range_hi"])))
            hi = max(hi, lo + 1)
        img8 = np.clip((counts - lo) / (hi - lo) * 255, 0, 255).astype(np.uint8)
        if cfg["equalize"]:
            img8 = cv2.equalizeHist(img8)
        img = luts[cfg["palette"]][img8]
        if cfg["readout"]:
            text = "%d-%d counts" % (lo, hi)
            if housing is not None:
                text += "  housing %.1f C" % housing
            if shutter:
                text += "  SHUTTER"
            for colour, width in (((0, 0, 0), 3), ((255, 255, 255), 1)):
                cv2.putText(img, text, (4, img.shape[0] - 6), cv2.FONT_HERSHEY_SIMPLEX, 0.38,
                            colour, width, cv2.LINE_AA)
        return img

    newest, housing, rec = None, None, None
    last = shutter_at = temp_at = 0.0
    got_at = time.time()
    try:
        cam.start_stream()
        while not cmd.quit:
            cmd.poll()
            changes = cmd.take()
            if changes:
                errors = apply(changes)
                report(params(), errors)
            rec = sync_recording(rec, cmd, start_recording, stop_recording)

            # Every frame that came (the SDK queues them, timed): each
            # one classified, as its shutter detection counts frames;
            # recorded raw at the chosen rate, with its shutter state;
            # the newest whole one kept for the view.
            frame = cam.read_stamped(0.05)
            while frame is not None:
                got_at = time.time()
                if rec:
                    rec.arrivals.append(frame.arrival)
                if len(frame.data) == P.FRAME_BYTES:
                    counts = np.frombuffer(frame.data, dtype="<u2").reshape(P.HEIGHT, P.WIDTH)
                    kind = cam.classify_frame(counts)
                    if kind == "flat":
                        shutter_at = got_at
                    else:
                        newest = counts
                    fps = cfg["record_fps"]
                    spacing = 1.0 / fps - P.FRAME_PERIOD_S / 2 if fps else 0.0
                    if rec and frame.time - rec.last >= spacing:
                        rec.last = frame.time
                        rec.raw.write(frame.data)
                        rec.kept.append(len(rec.arrivals) - 1)
                        rec.log(frame.time - P.READOUT_S / 2, frame.arrival, frame.index, kind,
                                None if housing is None else round(housing, 2))
                frame = cam.read_stamped(0.0)
            now = time.time()
            if now - got_at > THERMAL_LOST:
                say("error", "no frames from the thermal camera for %g s (unplugged?)"
                    % THERMAL_LOST)
                return

            # The housing temperature, now and then (for the tab too).
            if now - temp_at >= THERMAL_TEMP_PERIOD:
                temp_at = now
                try:
                    housing = cam.temperature_c()
                    say("streaming", model=model, housing_c=round(housing, 2))
                except usb1.USBError:
                    housing = None

            # Send the view when a frame is due.
            wanted = cmd.settings
            if newest is None or "thermal" not in wanted["views"] or not cmd.due(last):
                continue
            last = now
            send("thermal", render(newest, housing, now - shutter_at < THERMAL_SHUTTER_SHOWN),
                 wanted["quality"], wanted["width"])
            newest = None
    finally:
        if rec:
            stop_recording(rec)
            rec.close()
        cam.close()


# Any other USB camera (OpenCV, v4l2-ctl)


def v4l2_controls(device):
    """Return the camera's V4L2 controls as settings.

    From v4l2-ctl --list-ctrls-menus; [] if v4l2-ctl isn't there.
    """
    try:
        text = subprocess.run(["v4l2-ctl", "-d", device, "--list-ctrls-menus"],
                              capture_output=True, text=True, timeout=TOOL_TIMEOUT).stdout
    except (OSError, subprocess.TimeoutExpired):
        return []
    out, group = [], "Controls"
    for line in text.splitlines():
        # A control: "<name> 0x... (<kind>) : min=... value=...".
        match = re.match(r"\s*(\w+) 0x[0-9a-f]+ \((\w+)\)\s*: (.*)$", line)
        if match:
            name, kind, rest = match.groups()
            fields = dict(re.findall(r"(\w+)=(-?\d+)", rest))
            inactive = "inactive" in rest
            label = name.replace("_", " ").title()
            if kind == "bool":
                out.append(setting(name, label, group, "bool", fields.get("value") == "1",
                                   fields.get("default") == "1", inactive=inactive or None))
            elif kind in ("int", "int64"):
                out.append(setting(name, label, group, "range", int(fields.get("value", 0)),
                                   int(fields.get("default", 0)), min=int(fields.get("min", 0)),
                                   max=int(fields.get("max", 100)),
                                   step=int(fields.get("step", 1)) or 1,
                                   inactive=inactive or None))
            elif kind in ("menu", "intmenu"):
                out.append(setting(name, label, group, "enum", int(fields.get("value", 0)),
                                   int(fields.get("default", 0)), options=[],
                                   inactive=inactive or None))
            continue

        # A menu entry of the control above, or a new group ("User
        # Controls", "Camera Controls").
        match = re.match(r"\s+(\d+): (.*)$", line)
        if match and out and out[-1]["type"] == "enum":
            out[-1]["options"].append([int(match.group(1)), match.group(2).strip()])
        elif line.strip() and not line.startswith(" "):
            group = line.strip().title()
    return out


RECORD_RATES = [[0, "Every Frame"], [30, "30 fps"], [15, "15 fps"], [10, "10 fps"],
                [5, "5 fps"]]


def rate_due(last, fps):
    """Return whether a recorded frame is due (fps 0: every frame)."""
    return not fps or time.monotonic() - last >= 1.0 / fps


def run_uvc(device, cmd):
    """Run a USB (UVC) camera until told to quit.

    Args:
        device: Its /dev/videoN.
        cmd: The Commands.
    """
    saved = cmd.take()
    size = saved.pop("size", "1280x720")
    record_fps = int(as_number(saved.pop("record_fps", 0)))

    def open_cap():
        """Open it at the chosen size, as MJPEG."""
        capture = cv2.VideoCapture(device, cv2.CAP_V4L2)
        if capture.isOpened():
            capture.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"MJPG"))
            w, h = (int(x) for x in size.split("x"))
            capture.set(cv2.CAP_PROP_FRAME_WIDTH, w)
            capture.set(cv2.CAP_PROP_FRAME_HEIGHT, h)
        return capture

    def params():
        """Return the settings: the size, then its V4L2 controls."""
        return [setting("size", "Resolution", "Stream", "enum", size, "1280x720", reopen=True,
                        options=[[s, s.replace("x", " × ")]
                                 for s in ("640x480", "800x600", "1280x720", "1920x1080")]),
                setting("record_fps", "Recording Rate", "Recording", "enum", record_fps, 0,
                        options=RECORD_RATES,
                        help="How many frames an experiment's recording (MJPEG video) keeps a "
                             "second")
                ] + v4l2_controls(device)

    def apply(values):
        """Set V4L2 controls; return {id: why} for those that failed."""
        errors = {}
        for k, v in values.items():
            value = int(bool(v)) if isinstance(v, bool) else int(as_number(v))
            result = subprocess.run(["v4l2-ctl", "-d", device, "-c", "%s=%d" % (k, value)],
                                    capture_output=True, text=True, timeout=TOOL_TIMEOUT)
            if result.returncode:
                errors[k] = (result.stderr or result.stdout).strip() or "v4l2-ctl failed"
        return errors

    cap = open_cap()
    if not cap.isOpened():
        say("busy", "OpenCV couldn't open %s (another program?)" % device)
        return
    def start_recording(folder):
        """Record MJPEG video; the file opens with the first frame."""
        rec = Recording(folder, ["orin_time", "arrival_time"])
        rec.writer, rec.last = None, 0.0
        return rec

    def stop_recording(rec):
        """Close the video file."""
        if rec.writer is not None:
            rec.writer.release()

    say("open")
    errors = apply(saved)
    report(params(), errors)
    last, warm, rec = 0.0, time.time() + WARMUP, None
    try:
        while not cmd.quit:
            # Settings changes: a new size reopens it (not while
            # recording).
            cmd.poll()
            changes = cmd.take()
            if changes:
                refused = {}
                if "size" in changes and rec:
                    changes.pop("size")
                    refused["size"] = "not while recording (it reopens the camera)"
                elif "size" in changes:
                    size = changes.pop("size")
                    cap.release()
                    cap = open_cap()
                    warm = time.time() + WARMUP
                if "record_fps" in changes:
                    record_fps = int(as_number(changes.pop("record_fps"), record_fps))
                errors = apply(changes)
                errors.update(refused)
                report(params(), errors)
            rec = sync_recording(rec, cmd, start_recording, stop_recording)

            # The next frame: recorded at its rate, sent when due.
            ok, img = cap.read()
            if not ok:
                say("error", "no frames from %s" % device)
                time.sleep(1)
                continue
            if rec and rate_due(rec.last, record_fps):
                rec.last = arrival = time.monotonic()
                if rec.writer is None:
                    rec.writer = frame_writer(rec, "video.avi", (img.shape[1], img.shape[0]),
                                              record_fps or cap.get(cv2.CAP_PROP_FPS) or 30)
                if rec.writer is not None:
                    rec.writer.write(img)
                    rec.log(arrival, arrival)
            wanted = cmd.settings
            now = time.time()
            if "video" in wanted["views"] and now >= warm and cmd.due(last):
                last = now
                send("video", img, wanted["quality"], wanted["width"])
    finally:
        if rec:
            stop_recording(rec)
            rec.close()
        cap.release()


# A test pattern (--demo)


def run_demo(_, cmd):
    """Send a moving test pattern until told to quit."""
    cfg = {"brightness": 0, "speed": 1.0, "pattern": "bars", "invert": False, "noise": 10,
           "record_fps": 30}
    cfg.update(cmd.take())

    def params():
        """Return the pattern's settings list."""
        return [setting("brightness", "Brightness", "Image", "range", cfg["brightness"], 0,
                        min=-100, max=100, step=1),
                setting("pattern", "Pattern", "Image", "enum", cfg["pattern"], "bars",
                        options=[["bars", "Colour Bars"], ["checker", "Checkerboard"]]),
                setting("invert", "Invert", "Image", "bool", cfg["invert"], False),
                setting("speed", "Speed", "Motion", "range", cfg["speed"], 1.0, min=0, max=5,
                        step=0.1, unit="×"),
                setting("noise", "Noise", "Motion", "range", cfg["noise"], 10, min=0, max=60,
                        step=1, help="The noise view's amount"),
                setting("record_fps", "Recording Rate", "Recording", "enum",
                        int(cfg["record_fps"]), 30, options=RECORD_RATES[1:],
                        help="How many frames of the pattern an experiment's recording (MJPEG "
                             "video) keeps a second")]

    def start_recording(folder):
        """Record the pattern as MJPEG video."""
        rec = Recording(folder, ["orin_time", "arrival_time"])
        rec.writer = frame_writer(rec, "pattern.avi", (1280, 720), cfg["record_fps"])
        rec.last = 0.0
        if rec.writer is None:
            rec.close()
            return None
        return rec

    say("open", model="Test Pattern")
    report(params())
    t0, last, rec = time.time(), 0.0, None
    bars = np.array([[255, 255, 255], [0, 255, 255], [255, 255, 0], [0, 255, 0], [255, 0, 255],
                     [0, 0, 255], [255, 0, 0], [0, 0, 0]], np.uint8)
    while not cmd.quit:
        cmd.poll(0.01)
        changes = cmd.take()
        if changes:
            cfg.update({k: v for k, v in changes.items() if k in cfg})
            report(params())
        rec = sync_recording(rec, cmd, start_recording, lambda r: r.writer.release())
        wanted = cmd.settings
        show = bool(wanted["views"]) and cmd.due(last)
        keep = rec is not None and rate_due(rec.last, int(as_number(cfg["record_fps"], 30)))
        if not show and not keep:
            continue
        drawn = time.time()
        if show:
            last = drawn

        # The pattern, moving sideways; brightness, inversion, the time.
        shift = int((drawn - t0) * 60 * float(cfg["speed"]))
        x = (np.arange(1280) + shift) % 1280
        if cfg["pattern"] == "checker":
            yy, xx = np.mgrid[0:720, 0:1280]
            img = ((((xx + shift) // 80 + yy // 80) % 2) * 255).astype(np.uint8)
            img = img[..., None].repeat(3, 2)
        else:
            img = bars[(x // 160)][None, :, :].repeat(720, 0)
        img = np.clip(img.astype(np.int16) + int(cfg["brightness"]), 0, 255).astype(np.uint8)
        if cfg["invert"]:
            img = 255 - img
        cv2.putText(img, time.strftime("%H:%M:%S"), (40, 680), cv2.FONT_HERSHEY_SIMPLEX, 2,
                    (40, 40, 40), 5)

        # Recorded; the views: the pattern, and the pattern with noise.
        if keep:
            rec.last = time.monotonic()
            rec.writer.write(img)
            rec.log(rec.last, rec.last)
        if not show:
            continue
        if "pattern" in wanted["views"]:
            send("pattern", img, wanted["quality"], wanted["width"])
        if "noise" in wanted["views"]:
            noise = np.random.normal(0, float(cfg["noise"]), img.shape)
            send("noise", np.clip(img + noise, 0, 255).astype(np.uint8), wanted["quality"],
                 wanted["width"])
    if rec:
        rec.writer.release()
        rec.close()


def main():
    """Run the camera of the kind given, once a view is wanted.

    Errors go to the server as the camera's state, rather than the
    worker dying silently.
    """
    kind, arg = sys.argv[1], (sys.argv[2] if len(sys.argv) > 2 else None)
    cmd = Commands()

    # Nothing to open until someone wants a view or a recording.
    while not cmd.wanted() and not cmd.quit:
        cmd.poll(1.0)
    if cmd.quit:
        return
    try:
        {"zed": run_zed, "realsense": run_realsense, "dvs": run_dvs, "thermal": run_thermal,
         "uvc": run_uvc, "demo": run_demo}[kind](arg, cmd)
    except ImportError as e:
        say("error", "this Python (%s) has no %s" % (sys.executable, e.name))
    except BrokenPipeError:
        pass
    except Exception as e:
        say("error", "%s: %s" % (type(e).__name__, e))


if __name__ == "__main__":
    main()
