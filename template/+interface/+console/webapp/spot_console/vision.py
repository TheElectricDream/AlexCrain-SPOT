"""The Vision tab (lab console): the cameras on the Jetson Orin.

The Orin's SPOT Vision service (+interface/+console/vision/spot_vision.py,
installed by deploy/deploy_orin.sh) lists them. This console only asks
the Orin for the list; its address is a setting ("vision": host and
port). The video goes from the Orin straight to the page, so it never
loads the TX2.

An experiment can also record cameras (experiment.py): SPOT Vision
records on the Orin's own disk, and while it does, this console relays
the experiment clock of each PhaseSpace packet to it (ClockRelay, UDP
CLOCK_PORT), so every frame gets its experiment time. The recordings
go from the Orin straight to the local console when the data is
collected.

Part of the SPOT Console (webapp/app.py starts it).
"""

import ipaddress
import json
import math
import socket
import struct
import threading
import urllib.error
import urllib.request

from .common import ApiError
from .events import settings
from .phasespace import monitor

DEFAULT_VISION = {"host": "192.168.1.115", "port": 8090, "user": "spot-vision"}

STATUS_TIMEOUT = 3   # [s] asking for the camera list

# Recording (spot_vision.py's CLOCK_PORT and CLOCK_FORMAT: magic,
# sequence, packet time [s], experiment clock [s], NaN until armed).
CLOCK_PORT = 31545
CLOCK_MAGIC = b"SPCK"
CLOCK_FORMAT = "<4sIdd"
RECORD_START_TIMEOUT = 40   # [s] SPOT Vision waits up to 25 s for each camera
RECORD_STOP_TIMEOUT = 60    # [s] ... and 30 s for each to close its files


def config():
    """Return the vision settings, over the defaults."""
    return dict(DEFAULT_VISION, **(settings.all().get("vision") or {}))


def url():
    """Return SPOT Vision's base URL, e.g. http://192.168.1.115:8090."""
    cfg = config()
    return "http://%s:%d" % (cfg["host"], int(cfg["port"]))


def status():
    """Return what the Orin's SPOT Vision says now.

    Returns:
        A dict with url, config, reachable, cameras (a list), host
        (the Orin's name), disk ({"free", "total"} where recordings
        go), recording (the one in progress, or None) and error (None,
        or why it can't be reached).
    """
    base = url()
    out = {"url": base, "config": config(), "reachable": False, "cameras": [], "host": None,
           "disk": None, "recording": None, "error": None}
    try:
        with urllib.request.urlopen(base + "/api/cameras", timeout=STATUS_TIMEOUT) as response:
            data = json.loads(response.read().decode("utf-8"))
        out.update(reachable=True, cameras=data.get("cameras", []), host=data.get("host"),
                   disk=data.get("disk"), recording=data.get("recording"))
    except urllib.error.URLError as e:
        # Refused: the service isn't running; otherwise the Orin isn't
        # answering.
        reason = getattr(e, "reason", e)
        if "refused" in str(reason).lower():
            out["error"] = ("SPOT Vision isn't running on %s (deploy it with "
                            "+interface/+console/deploy/deploy_orin.sh)" % base)
        else:
            out["error"] = "%s isn't answering (%s)" % (base, reason)
    except (OSError, ValueError) as e:
        out["error"] = "%s: %s" % (base, e)
    return out


def _camera_path(camera):
    """Return the API path of a camera's settings.

    Raises:
        ApiError: if the camera id is empty or has other characters
            than letters, digits, "-", "_" and ".".
    """
    camera = str(camera or "")
    if not camera or not all(c.isalnum() or c in "-_." for c in camera):
        raise ApiError("Say which camera (its id).")
    return "/api/cameras/%s/params" % camera


def _orin(path, body=None, timeout=8):
    """Call SPOT Vision's API.

    Args:
        path: The API path.
        body: A dict to POST as JSON, or None to GET.
        timeout: Seconds before giving up.

    Returns:
        The decoded JSON answer.

    Raises:
        ApiError: with SPOT Vision's own error message, or why it
            couldn't be reached.
    """
    is_post = body is not None
    req = urllib.request.Request(url() + path, method="POST" if is_post else "GET",
                                 data=json.dumps(body).encode("utf-8") if is_post else None,
                                 headers={"Content-Type": "application/json"} if is_post else {})
    try:
        with urllib.request.urlopen(req, timeout=timeout) as response:
            return json.loads(response.read().decode("utf-8"))
    except urllib.error.HTTPError as e:
        # An error SPOT Vision explains in its JSON answer.
        try:
            raise ApiError(json.loads(e.read().decode("utf-8")).get("error") or str(e))
        except ValueError:
            raise ApiError("%s: %s" % (url(), e))
    except (urllib.error.URLError, OSError, ValueError) as e:
        raise ApiError("SPOT Vision (%s) isn't answering: %s"
                       % (url(), getattr(e, "reason", e)))


def get_params(camera):
    """Return a camera's settings, as its library lists them.

    The Orin keeps the last list while the camera is closed.
    """
    return _orin(_camera_path(camera))


def set_params(camera, body):
    """Change a camera's settings.

    Args:
        camera: The camera's id.
        body: {"values": {setting id: value}} or {"reset": true}.
            Applied now if the camera is open, and kept for each time
            it opens.

    Raises:
        ApiError: if values isn't a dict.
    """
    values = body.get("values") or {}
    if not isinstance(values, dict):
        raise ApiError("values must be {setting id: value}.")
    return _orin(_camera_path(camera), {"values": values, "reset": bool(body.get("reset"))})


# Recording, for experiments


def record_start(session, cameras):
    """Have SPOT Vision record cameras, and relay the clock to it.

    Args:
        session: The recording's name (the experiment's).
        cameras: The cameras' ids.

    Returns:
        SPOT Vision's status of the recording.

    Raises:
        ApiError: if it can't start (SPOT Vision says why); then no
            camera records.
    """
    result = _orin("/api/record/start", {"session": session, "cameras": list(cameras)},
                   timeout=RECORD_START_TIMEOUT)
    relay.start(config()["host"])
    return result


def record_stop(session):
    """Stop the clock relay and the recording; return session.json.

    None if SPOT Vision wasn't recording it.

    Raises:
        ApiError: if SPOT Vision can't be reached.
    """
    relay.stop()
    return _orin("/api/record/stop", {"session": session},
                 timeout=RECORD_STOP_TIMEOUT).get("stopped")


class ClockRelay:
    """Sends SPOT Vision each PhaseSpace packet's clocks when recording.

    A monitor listener: StreamData's local copy of every packet, as the
    platforms get it, so the recording's frames are timed on the same
    experiment clock as the platforms.
    """

    def __init__(self):
        """Start not relaying."""
        self._lock = threading.Lock()
        self._target = None
        self._sock = None
        self._seq = 0

    def start(self, host):
        """Relay to SPOT Vision on host."""
        with self._lock:
            if self._sock is None:
                self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self._target, self._seq = (host, CLOCK_PORT), 0
        monitor.add_listener(self._on_frame)

    def stop(self):
        """Stop relaying."""
        monitor.remove_listener(self._on_frame)
        with self._lock:
            self._target = None

    def _on_frame(self, frame):
        """Send one packet's packet time and experiment clock."""
        with self._lock:
            target, sock = self._target, self._sock
            self._seq = (self._seq + 1) & 0xFFFFFFFF
            seq = self._seq
        if target is None:
            return
        nan = float("nan")
        t, exp = frame.get("t"), frame.get("exp")
        data = struct.pack(CLOCK_FORMAT, CLOCK_MAGIC, seq,
                           t if t is not None and math.isfinite(t) else nan,
                           exp if exp is not None else nan)
        try:
            sock.sendto(data, target)
        except OSError:
            pass


relay = ClockRelay()


def parse_vision(value):
    """Return the "vision" setting from a request.

    Args:
        value: {"host": an IPv4 address, "port": 1024-65535}; either
            may be left out to keep the current one.

    Raises:
        ApiError: if the host or port isn't valid.
    """
    if not isinstance(value, dict):
        raise ApiError("vision must be {host, port}.")
    cfg = config()

    # Host: an IPv4 address, normalised.
    if "host" in value:
        try:
            cfg["host"] = str(ipaddress.IPv4Address(str(value["host"]).strip()))
        except ValueError:
            raise ApiError("The Orin's address must be an IPv4 address, e.g. 192.168.1.115.")

    # Port: an unprivileged port number.
    if "port" in value:
        try:
            cfg["port"] = int(value["port"])
        except (TypeError, ValueError):
            raise ApiError("The vision port must be a number.")
        if not 1024 <= cfg["port"] <= 65535:
            raise ApiError("The vision port must be 1024-65535.")
    return cfg
