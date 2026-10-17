"""Console settings, the event hub, and running commands.

The settings live in ~/.spot_phasespace_console.json. The event hub
feeds every open page (server-sent events and logs). Commands run with
their output logged.

Part of the SPOT Console (webapp/app.py starts it).
"""

import collections
import json
import queue
import subprocess
import threading
import time
from pathlib import Path

from .common import PROJECT_DIR, decode, quote_cmd
from .constants import C


# Settings

# Kept in the home folder (not in Dropbox), so the laptop and the lab
# PC each keep their own paths.
SETTINGS_FILE = Path.home() / ".spot_phasespace_console.json"

# Each platform's address and login name (IP_*, USER_* in Constants.m).
DEFAULT_PLATFORMS = dict((p, {"host": C.platform("IP", p), "user": C.platform("USER", p)})
                         for p in C.PLATFORMS)

# The Orin's SPOT Vision (Vision tab).
DEFAULT_VISION = {"host": C.IP_ORIN, "port": C.int("PORT_VISION"), "user": C.USER_VISION}

DEFAULT_SETTINGS = {
    "output_dir": str(PROJECT_DIR / "output"),
    "frequency": C.int("PHASESPACE_FREQUENCY"),   # [Hz] the cameras
    "monitor_port": C.int("PORT_PHASESPACE_MONITOR"),
    # How StreamData receives frames from PhaseSpace: "tcp" or "udp".
    "transport": "tcp",
    # Computers that also get the packets, besides StreamData.cc's list.
    "extra_destinations": [],
    # Where the Hardware tab logs in.
    "platforms": DEFAULT_PLATFORMS,
    "vision": DEFAULT_VISION,
}

LOG_LINES = 3000      # kept per log channel
CLIENT_QUEUE = 2000   # events queued per open page


class Settings:
    """The console's settings: loaded at start, saved on each update."""

    def __init__(self):
        """Load the settings file (if readable) over the defaults."""
        self._lock = threading.Lock()
        self._data = dict(DEFAULT_SETTINGS)
        try:
            self._data.update(json.loads(SETTINGS_FILE.read_text(encoding="utf-8")))
        except (OSError, ValueError):
            pass

    def get(self, key):
        """Return one setting."""
        with self._lock:
            return self._data[key]

    def all(self):
        """Return a copy of all the settings."""
        with self._lock:
            return dict(self._data)

    def update(self, values):
        """Change some settings and save them all.

        Args:
            values: A dict of the settings to change.

        Returns:
            A copy of all the settings. A failed save is reported on
            the console's output, not raised.
        """
        with self._lock:
            self._data.update(values)
            data = dict(self._data)
        try:
            SETTINGS_FILE.write_text(json.dumps(data, indent=2), encoding="utf-8")
        except OSError as e:
            print("Could not save settings to %s: %s" % (SETTINGS_FILE, e))
        return data


# Event hub


class EventHub:
    """Fans server-sent events out to every open browser tab.

    Also keeps the recent log lines of each channel, for pages that
    open later.
    """

    def __init__(self):
        """Start with no clients and empty logs."""
        self._lock = threading.Lock()
        self._clients = set()

        # One log per channel ("deploy": Lab Setup's scripts, local
        # console).
        self.logs = {
            "build": collections.deque(maxlen=LOG_LINES),
            "stream": collections.deque(maxlen=LOG_LINES),
            "hardware": collections.deque(maxlen=LOG_LINES),
            "matlab": collections.deque(maxlen=LOG_LINES),
            "deploy": collections.deque(maxlen=LOG_LINES),
        }

    def subscribe(self):
        """Return a new queue that receives every event."""
        q = queue.Queue(maxsize=CLIENT_QUEUE)
        with self._lock:
            self._clients.add(q)
        return q

    def unsubscribe(self, q):
        """Stop sending events to a queue."""
        with self._lock:
            self._clients.discard(q)

    def publish(self, event, data):
        """Send an event (name and JSON data) to every open page.

        A stalled page whose queue is full just misses messages.
        """
        msg = "event: %s\ndata: %s\n\n" % (event, json.dumps(data, allow_nan=False))
        with self._lock:
            clients = list(self._clients)
        for q in clients:
            try:
                q.put_nowait(msg)
            except queue.Full:
                pass

    def log(self, channel, line):
        """Add a line to a log channel and send it to the pages."""
        entry = {"channel": channel, "time": time.time(), "line": line}
        with self._lock:
            self.logs[channel].append(entry)
        self.publish("log", entry)

    def history(self):
        """Return the recent lines of every log channel."""
        with self._lock:
            return {k: list(v) for k, v in self.logs.items()}


settings = Settings()

hub = EventHub()


# Running commands


def run_logged(cmd, channel, cwd=None):
    """Run a command, streaming its output to a log channel.

    Args:
        cmd: The command, a list of arguments.
        channel: The log channel (see EventHub.logs).
        cwd: The working folder, or None.

    Returns:
        The exit code, or -1 if it couldn't be started.
    """
    hub.log(channel, "$ " + quote_cmd(cmd))
    try:
        proc = subprocess.Popen([str(c) for c in cmd], cwd=cwd, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    except OSError as e:
        hub.log(channel, "Could not run %s: %s" % (cmd[0], e))
        return -1

    # Each line as it comes.
    for raw in proc.stdout:
        hub.log(channel, decode(raw))
    return proc.wait()


def run_quiet(cmd, timeout=30, cwd=None):
    """Run a command and return its exit code and output.

    Args:
        cmd: The command, a list of arguments.
        timeout: Seconds before giving up.
        cwd: The working folder, or None.

    Returns:
        (exit code, output text); (-1, the error) if it couldn't run
        or timed out.
    """
    try:
        result = subprocess.run([str(c) for c in cmd], stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL,
                                timeout=timeout, cwd=cwd)
        return result.returncode, decode(result.stdout)
    except (OSError, subprocess.TimeoutExpired) as e:
        return -1, str(e)
