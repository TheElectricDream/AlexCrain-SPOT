"""Shared helpers: project paths, the API error, value parsing.

Part of the SPOT Console (webapp/app.py starts it).
"""

import math
import re
import shlex
import sys
from pathlib import Path


APP_DIR = Path(__file__).resolve().parent.parent   # webapp/ (this file is in webapp/spot_console)


STATIC_DIR = APP_DIR / "static"


PROJECT_DIR = APP_DIR.parent   # the console's folder: +spot/+console here, ~/PhaseSpace_Server_SendAll on the TX2
PHASESPACE_DIR = PROJECT_DIR / "phasespace"   # StreamData's source, Makefile and the PhaseSpace library
# Kept in the home folder (not in Dropbox) so the laptop and the lab PC each keep their own paths


# What people call the platforms: the API, the settings, the files and the platforms' own names
# keep RED, BLACK and BLUE; messages meant to be read use these
PLATFORM_NAMES = {"RED": "Chaser", "BLACK": "Target", "BLUE": "Obstacle"}


def shown(name):
    """The name to show for a platform key ("RED" -> "Chaser"); other text unchanged."""
    return PLATFORM_NAMES.get(name, name)


def shown_list(names):
    return ", ".join(shown(n) for n in names)


class ApiError(Exception):
    """An error message meant for the user (sent back as HTTP 400). details: more about it for
    the page to show on request (MATLAB's errors: spot.sim.errorDetails), or None."""

    def __init__(self, message, details=None):
        super().__init__(message)
        self.details = details


def decode(raw):
    """Decode process output; also handles wsl.exe's own UTF-16 messages."""
    if isinstance(raw, str):
        text = raw
    else:
        try:
            text = raw.decode("utf-8")
        except UnicodeDecodeError:
            text = raw.decode(sys.getfilesystemencoding() or "latin-1", errors="replace")
    return text.replace("\x00", "").rstrip("\r\n")


def finite(v):
    return v if isinstance(v, float) and math.isfinite(v) else None


def quote_cmd(cmd):
    return " ".join(shlex.quote(str(c)) for c in cmd)


# --------------------------------------------------------------------------- settings / events


def parse_bool(value, what):
    if value in (True, 1, "1", "true", "on", "yes"):
        return True
    if value in (False, 0, "0", "false", "off", "no"):
        return False
    raise ApiError("%s must be true or false." % what)


def parse_number(value, what, low, high):
    try:
        v = float(value)
    except (TypeError, ValueError):
        raise ApiError("%s must be a number." % what)
    if not (math.isfinite(v) and low <= v <= high):
        raise ApiError("%s must be between %g and %g." % (what, low, high))
    return v


HOST_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9.-]*$")


USER_RE = re.compile(r"^[a-z_][a-z0-9_-]*$")
