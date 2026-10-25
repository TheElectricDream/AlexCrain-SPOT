"""Shared helpers: project paths, the API error, value parsing.

Part of the SPOT Console (webapp/app.py starts it).
"""

import math
import re
import shlex
import sys
from pathlib import Path


# Paths

# webapp/ (this file is in webapp/spot_console).
APP_DIR = Path(__file__).resolve().parent.parent

STATIC_DIR = APP_DIR / "static"

# The console's folder: +interface/+console here, ~/PhaseSpace_Server_SendAll
# on the TX2.
PROJECT_DIR = APP_DIR.parent

# StreamData's source, Makefile and the PhaseSpace library.
PHASESPACE_DIR = PROJECT_DIR / "phasespace"


# Platform names

# What people call the platforms. The API, the settings, the files and
# the platforms' own names keep RED, BLACK and BLUE; messages meant to
# be read use these.
PLATFORM_NAMES = {"RED": "Chaser", "BLACK": "Target", "BLUE": "Obstacle"}


def shown(name):
    """Return the name to show for a platform key ("RED" -> "Chaser").

    Other text is returned unchanged.
    """
    return PLATFORM_NAMES.get(name, name)


def shown_list(names):
    """Return the shown names of several platforms, comma-separated."""
    return ", ".join(shown(n) for n in names)


# Errors and process output


class ApiError(Exception):
    """An error message meant for the user (sent back as HTTP 400).

    Attributes:
        details: More about it for the page to show on request
            (MATLAB's errors: interface.worker.errorDetails), or None.
    """

    def __init__(self, message, details=None):
        """Store the message and the optional details."""
        super().__init__(message)
        self.details = details


def decode(raw):
    """Decode process output as text.

    Also handles wsl.exe's own UTF-16 messages (NUL characters are
    dropped) and strips the trailing line break.
    """
    if isinstance(raw, str):
        text = raw
    else:
        try:
            text = raw.decode("utf-8")
        except UnicodeDecodeError:
            text = raw.decode(sys.getfilesystemencoding() or "latin-1", errors="replace")
    return text.replace("\x00", "").rstrip("\r\n")


def finite(v):
    """Return v if it is a finite float, else None."""
    return v if isinstance(v, float) and math.isfinite(v) else None


def quote_cmd(cmd):
    """Return a command (a list of arguments) as a shell-quoted line."""
    return " ".join(shlex.quote(str(c)) for c in cmd)


# Request values


def parse_bool(value, what):
    """Return a request value as a bool.

    Args:
        value: True/False, 1/0, or "true"/"on"/"yes" and their
            opposites.
        what: The value's name, for the error message.

    Raises:
        ApiError: if value isn't one of those.
    """
    if value in (True, 1, "1", "true", "on", "yes"):
        return True
    if value in (False, 0, "0", "false", "off", "no"):
        return False
    raise ApiError("%s must be true or false." % what)


def parse_number(value, what, low, high):
    """Return a request value as a float within [low, high].

    Args:
        value: Anything float() accepts.
        what: The value's name, for the error message.
        low: The smallest value allowed.
        high: The largest value allowed.

    Raises:
        ApiError: if value isn't a finite number within the range.
    """
    try:
        number = float(value)
    except (TypeError, ValueError):
        raise ApiError("%s must be a number." % what)
    if not (math.isfinite(number) and low <= number <= high):
        raise ApiError("%s must be between %g and %g." % (what, low, high))
    return number


# A host name or IPv4 address, and a Linux user name.
HOST_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9.-]*$")
USER_RE = re.compile(r"^[a-z_][a-z0-9_-]*$")
