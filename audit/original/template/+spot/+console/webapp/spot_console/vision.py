"""The Vision tab (lab console): the cameras on the Jetson Orin.

The Orin's SPOT Vision service (+spot/+console/vision/spot_vision.py,
installed by deploy/deploy_orin.sh) lists them. This console only asks
the Orin for the list; its address is a setting ("vision": host and
port). The video goes from the Orin straight to the page, so it never
loads the TX2.

Part of the SPOT Console (webapp/app.py starts it).
"""

import ipaddress
import json
import urllib.error
import urllib.request

from .common import ApiError
from .events import settings

DEFAULT_VISION = {"host": "192.168.1.115", "port": 8090, "user": "spot-vision"}

STATUS_TIMEOUT = 3   # [s] asking for the camera list


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
        (the Orin's name) and error (None, or why it can't be
        reached).
    """
    base = url()
    out = {"url": base, "config": config(), "reachable": False, "cameras": [], "host": None,
           "error": None}
    try:
        with urllib.request.urlopen(base + "/api/cameras", timeout=STATUS_TIMEOUT) as response:
            data = json.loads(response.read().decode("utf-8"))
        out.update(reachable=True, cameras=data.get("cameras", []), host=data.get("host"))
    except urllib.error.URLError as e:
        # Refused: the service isn't running; otherwise the Orin isn't
        # answering.
        reason = getattr(e, "reason", e)
        if "refused" in str(reason).lower():
            out["error"] = ("SPOT Vision isn't running on %s (deploy it with "
                            "+spot/+console/deploy/deploy_orin.sh)" % base)
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
