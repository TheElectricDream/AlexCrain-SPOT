"""Console settings (~/.spot_phasespace_console.json), the event hub that feeds every open page
(server-sent events and logs), and running commands with their output logged.

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


SETTINGS_FILE = Path.home() / ".spot_phasespace_console.json"


DEFAULT_PLATFORMS = {
    "RED": {"host": "192.168.1.110", "user": "spot-red"},
    "BLACK": {"host": "192.168.1.111", "user": "spot-black"},
    "BLUE": {"host": "192.168.1.112", "user": "spot-blue"},
}


DEFAULT_SETTINGS = {
    "output_dir": str(PROJECT_DIR / "output"),
    "frequency": 100,
    "monitor_port": 31540,
    "transport": "tcp",  # how StreamData receives frames from PhaseSpace: "tcp" or "udp"
    "extra_destinations": [],  # computers that also get the packets, on top of StreamData.cc's list
    "platforms": DEFAULT_PLATFORMS,  # where the Hardware tab logs in
}


class Settings:
    def __init__(self):
        self._lock = threading.Lock()
        self._data = dict(DEFAULT_SETTINGS)
        try:
            self._data.update(json.loads(SETTINGS_FILE.read_text(encoding="utf-8")))
        except (OSError, ValueError):
            pass

    def get(self, key):
        with self._lock:
            return self._data[key]

    def all(self):
        with self._lock:
            return dict(self._data)

    def update(self, values):
        with self._lock:
            self._data.update(values)
            data = dict(self._data)
        try:
            SETTINGS_FILE.write_text(json.dumps(data, indent=2), encoding="utf-8")
        except OSError as e:
            print("Could not save settings to %s: %s" % (SETTINGS_FILE, e))
        return data


class EventHub:
    """Fans server-sent events out to every open browser tab and keeps recent log lines."""

    def __init__(self):
        self._lock = threading.Lock()
        self._clients = set()
        self.logs = {"build": collections.deque(maxlen=3000), "stream": collections.deque(maxlen=3000),
                     "hardware": collections.deque(maxlen=3000), "matlab": collections.deque(maxlen=3000)}

    def subscribe(self):
        q = queue.Queue(maxsize=2000)
        with self._lock:
            self._clients.add(q)
        return q

    def unsubscribe(self, q):
        with self._lock:
            self._clients.discard(q)

    def publish(self, event, data):
        msg = "event: %s\ndata: %s\n\n" % (event, json.dumps(data, allow_nan=False))
        with self._lock:
            clients = list(self._clients)
        for q in clients:
            try:
                q.put_nowait(msg)
            except queue.Full:
                pass  # a stalled tab just misses messages

    def log(self, channel, line):
        entry = {"channel": channel, "time": time.time(), "line": line}
        with self._lock:
            self.logs[channel].append(entry)
        self.publish("log", entry)

    def history(self):
        with self._lock:
            return {k: list(v) for k, v in self.logs.items()}


settings = Settings()


hub = EventHub()


def run_logged(cmd, channel, cwd=None):
    """Run a command, streaming its output to a log channel. Returns the exit code."""
    hub.log(channel, "$ " + quote_cmd(cmd))
    try:
        proc = subprocess.Popen([str(c) for c in cmd], cwd=cwd, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    except OSError as e:
        hub.log(channel, "Could not run %s: %s" % (cmd[0], e))
        return -1
    for raw in proc.stdout:
        hub.log(channel, decode(raw))
    return proc.wait()


def run_quiet(cmd, timeout=30, cwd=None):
    try:
        r = subprocess.run([str(c) for c in cmd], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           stdin=subprocess.DEVNULL, timeout=timeout, cwd=cwd)
        return r.returncode, decode(r.stdout)
    except (OSError, subprocess.TimeoutExpired) as e:
        return -1, str(e)
