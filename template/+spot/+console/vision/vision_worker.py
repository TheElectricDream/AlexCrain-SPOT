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
    {"quit": true}
Frames and news go to standard output:
    b"F <view> <length>\\n" followed by that many bytes of JPEG
    b"S <json>\\n"  {"state": "open" | "streaming" | "busy" | "error",
                    "detail": "...", "model", "serial", "housing_c"}
    b"P <json>\\n"  {"params": [setting, ...], "errors": {"<id>":
                    "why"}}: the camera's settings, after a change
A setting: {"id", "label", "group", "type": "range" | "bool" | "enum",
"min", "max", "step", "value", "default", "options": [[value, label],
...], "reopen": (changing it reopens the camera), "help", "unit",
"inactive"}.

The camera is opened when the first view is asked for and closed when
the process ends; spot_vision.py ends it a few seconds after the last
viewer leaves, so experiments on the Orin can have the camera the rest
of the time.
"""
import json
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
        """Start with nothing to send."""
        self.settings = {"views": [], "fps": 15, "quality": 80, "width": 0}
        self.pending = {}
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
    cfg = {"resolution": "HD720", "camera_fps": 30, "depth_mode": "PERFORMANCE",
           "depth_min": 0.3, "depth_max": 20.0, "confidence": 95, "texture_confidence": 100,
           "fill_mode": False}
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

    mat = sl.Mat()
    runtime = runtime_params()
    last, warm = 0.0, time.time() + WARMUP
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
                cfg.update(changes)
                errors = {}
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

            # Grab; send the views wanted when a frame is due.
            wanted = cmd.settings
            views = [v for v in wanted["views"] if v in VIEWS]
            if cam.grab(runtime) != sl.ERROR_CODE.SUCCESS:
                time.sleep(0.01)
                continue
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

    pipe, running_views, last, warm = None, None, 0.0, 0.0
    errors = apply(saved)
    report(params(), errors)
    try:
        while not cmd.quit:
            # Settings changes: a new mode restarts the streams (the
            # settings report follows).
            changed = cmd.poll(0.0 if pipe else 0.2)
            changes = cmd.take()
            restart = False
            if changes:
                chosen = {k: changes.pop(k) for k in [k for k in changes if k.startswith("mode.")]}
                modes.update(chosen)
                restart = bool(chosen)
                errors = apply(changes)
                if not restart:
                    report(params(), errors)
            wanted = cmd.settings
            views = sorted(v for v in wanted["views"] if v in STREAMS)

            # (Re)start the streams the views need.
            if restart or views != running_views or changed and pipe is None:
                if pipe:
                    pipe.stop()
                    pipe = None
                running_views = views
                if not views:
                    if restart:
                        report(params())
                    continue

                # The chosen mode of each stream, else the first that
                # works.
                why = ""
                for attempt in range(3):
                    cfg = rs.config()
                    cfg.enable_device(dev.get_info(rs.camera_info.serial_number))
                    for view in views:
                        stream, fmt, fallback = STREAMS[view]
                        chosen = modes.get("mode." + view)
                        match = re.match(r"(\d+)x(\d+)@(\d+)$", chosen or "")
                        if match and attempt == 0:
                            width, height, rate = (int(match.group(1)), int(match.group(2)),
                                                   int(match.group(3)))
                        else:
                            width, height, rate = fallback[min(attempt, len(fallback) - 1)]
                        cfg.enable_stream(stream, width, height, fmt, rate)
                    try:
                        candidate = rs.pipeline(ctx)
                        candidate.start(cfg)
                        pipe = candidate
                        break
                    except RuntimeError as e:
                        why = str(e)
                if pipe is None:
                    say("busy" if "busy" in why.lower() else "error",
                        "librealsense couldn't start %s: %s" % (", ".join(views), why))
                    running_views = None
                    time.sleep(2)
                    continue
                if restart:
                    report(params())
                say("streaming", views=views, model=dev.get_info(rs.camera_info.name),
                    serial=dev.get_info(rs.camera_info.serial_number))
                last, warm = 0.0, time.time() + WARMUP
            if not pipe:
                continue

            # The next frames; send the views when a frame is due.
            try:
                frames = pipe.wait_for_frames(FRAME_WAIT_MS)
            except RuntimeError as e:
                say("error", "no frames from the L515: %s" % e)
                continue
            now = time.time()
            if now < warm or not cmd.due(last):
                continue
            last = now
            scale = dev.first_depth_sensor().get_depth_scale() if "depth" in views else 0.00025
            for view in views:
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

    errors = apply(cmd.take())
    report(params(), errors)
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

        # The next events: kept for the window, and added to the
        # accumulated view (faded towards 0 first; + brighter, -
        # darker).
        batch = cap.getNextEventBatch()
        if batch is not None and len(batch) > 0:
            ev = batch.numpy()
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
    raw 14-bit counts at 59.6 Hz. The counts aren't temperatures (the
    SDK has no radiometric calibration yet): the view stretches a range
    of them over a palette, the picture's own (automatic) or one set
    by hand. The camera closes its shutter now and then to recalibrate
    (flat-field correction); the frames it sends while closed are all
    one value, and are skipped, so the picture holds meanwhile.

    Args:
        _: Unused (there is one such camera).
        cmd: The Commands.
    """
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "vendor"))
    try:
        import usb1
        from ici9320 import ICI9320, protocol as P
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
           "equalize": False, "readout": True}

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
        return errors

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

    newest, housing = None, None
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

            # Every frame that came (the SDK queues them all): each one
            # classified, as its shutter detection counts frames, and
            # the newest whole one kept.
            raw = cam.read_frame(0.05)
            while raw is not None:
                got_at = time.time()
                if len(raw) == P.FRAME_BYTES:
                    frame = np.frombuffer(raw, dtype="<u2").reshape(P.HEIGHT, P.WIDTH)
                    if cam.classify_frame(frame) == "flat":
                        shutter_at = got_at
                    else:
                        newest = frame
                raw = cam.read_frame(0.0)
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


def run_uvc(device, cmd):
    """Run a USB (UVC) camera until told to quit.

    Args:
        device: Its /dev/videoN.
        cmd: The Commands.
    """
    saved = cmd.take()
    size = saved.pop("size", "1280x720")

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
                                 for s in ("640x480", "800x600", "1280x720", "1920x1080")])
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
    say("open")
    errors = apply(saved)
    report(params(), errors)
    last, warm = 0.0, time.time() + WARMUP
    try:
        while not cmd.quit:
            # Settings changes: a new size reopens it.
            cmd.poll()
            changes = cmd.take()
            if changes:
                if "size" in changes:
                    size = changes.pop("size")
                    cap.release()
                    cap = open_cap()
                    warm = time.time() + WARMUP
                errors = apply(changes)
                report(params(), errors)

            # The next frame; sent when due.
            ok, img = cap.read()
            if not ok:
                say("error", "no frames from %s" % device)
                time.sleep(1)
                continue
            wanted = cmd.settings
            now = time.time()
            if "video" in wanted["views"] and now >= warm and cmd.due(last):
                last = now
                send("video", img, wanted["quality"], wanted["width"])
    finally:
        cap.release()


# A test pattern (--demo)


def run_demo(_, cmd):
    """Send a moving test pattern until told to quit."""
    cfg = {"brightness": 0, "speed": 1.0, "pattern": "bars", "invert": False, "noise": 10}
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
                        step=1, help="The noise view's amount")]

    say("open", model="Test Pattern")
    report(params())
    t0, last = time.time(), 0.0
    bars = np.array([[255, 255, 255], [0, 255, 255], [255, 255, 0], [0, 255, 0], [255, 0, 255],
                     [0, 0, 255], [255, 0, 0], [0, 0, 0]], np.uint8)
    while not cmd.quit:
        cmd.poll(0.01)
        changes = cmd.take()
        if changes:
            cfg.update({k: v for k, v in changes.items() if k in cfg})
            report(params())
        wanted = cmd.settings
        if not wanted["views"] or not cmd.due(last):
            continue
        last = time.time()

        # The pattern, moving sideways; brightness, inversion, the time.
        shift = int((last - t0) * 60 * float(cfg["speed"]))
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

        # The views: the pattern, and the pattern with noise.
        if "pattern" in wanted["views"]:
            send("pattern", img, wanted["quality"], wanted["width"])
        if "noise" in wanted["views"]:
            noise = np.random.normal(0, float(cfg["noise"]), img.shape)
            send("noise", np.clip(img + noise, 0, 255).astype(np.uint8), wanted["quality"],
                 wanted["width"])


def main():
    """Run the camera of the kind given, once a view is wanted.

    Errors go to the server as the camera's state, rather than the
    worker dying silently.
    """
    kind, arg = sys.argv[1], (sys.argv[2] if len(sys.argv) > 2 else None)
    cmd = Commands()

    # Nothing to open until someone wants a view.
    while not cmd.settings["views"] and not cmd.quit:
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
