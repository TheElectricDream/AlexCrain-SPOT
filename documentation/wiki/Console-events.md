# events.py

**File:** `+spot/+console/webapp/spot_console/events.py` · **Part of:** [[the console's server|Console-Server]]

<!-- source: +spot/+console/webapp/spot_console/events.py -->

## What it is for

Three things shared by the whole server: the console's own settings file, the **event hub** that pushes updates to every open browser tab (server-sent events), and running a command with its output going to a log channel.

## Function by function

**Lines 18–35.** The settings file `~/.spot_phasespace_console.json` and its defaults: the StreamData output folder, the stream frequency (100 Hz), the monitor port (31540, where StreamData sends a local copy of each packet), the transport (`tcp`/`udp`), extra destinations, and each platform's SSH host and user. These are the *lab console's* settings; the project's `spot_settings.json` is MATLAB's.

**Lines 38–63.** `Settings`: loaded once, read and updated under a lock, written back as JSON on every update (a failure is only printed).

**Lines 66–103.** `EventHub`: every open page has a queue (`subscribe`/`unsubscribe`); `publish(event, data)` formats a server-sent event (`event: name`, `data: json`) and puts it on every queue without waiting (a stalled tab just misses messages); `log(channel, line)` keeps the last 3000 lines of each of the four channels (`build`, `stream`, `hardware`, `matlab`) and publishes a `log` event; `history` returns them for a page that just opened.

**Lines 106–109.** The one `settings` and the one `hub`.

**Lines 112–132.** `run_logged`: run a command, send each output line to a log channel as it comes, return the exit code (−1 if it couldn't start). `run_quiet`: run with a timeout and return (code, output).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/events.py</b> (132 lines; click to show)</summary>

```python
   1  """Console settings (~/.spot_phasespace_console.json), the event hub that feeds every open page
   2  (server-sent events and logs), and running commands with their output logged.
   3  
   4  Part of the SPOT Console (webapp/app.py starts it).
   5  """
   6  
   7  import collections
   8  import json
   9  import queue
  10  import subprocess
  11  import threading
  12  import time
  13  from pathlib import Path
  14  
  15  from .common import PROJECT_DIR, decode, quote_cmd
  16  
  17  
  18  SETTINGS_FILE = Path.home() / ".spot_phasespace_console.json"
  19  
  20  
  21  DEFAULT_PLATFORMS = {
  22      "RED": {"host": "192.168.1.110", "user": "spot-red"},
  23      "BLACK": {"host": "192.168.1.111", "user": "spot-black"},
  24      "BLUE": {"host": "192.168.1.112", "user": "spot-blue"},
  25  }
  26  
  27  
  28  DEFAULT_SETTINGS = {
  29      "output_dir": str(PROJECT_DIR / "output"),
  30      "frequency": 100,
  31      "monitor_port": 31540,
  32      "transport": "tcp",  # how StreamData receives frames from PhaseSpace: "tcp" or "udp"
  33      "extra_destinations": [],  # computers that also get the packets, on top of StreamData.cc's list
  34      "platforms": DEFAULT_PLATFORMS,  # where the Hardware tab logs in
  35  }
  36  
  37  
  38  class Settings:
  39      def __init__(self):
  40          self._lock = threading.Lock()
  41          self._data = dict(DEFAULT_SETTINGS)
  42          try:
  43              self._data.update(json.loads(SETTINGS_FILE.read_text(encoding="utf-8")))
  44          except (OSError, ValueError):
  45              pass
  46  
  47      def get(self, key):
  48          with self._lock:
  49              return self._data[key]
  50  
  51      def all(self):
  52          with self._lock:
  53              return dict(self._data)
  54  
  55      def update(self, values):
  56          with self._lock:
  57              self._data.update(values)
  58              data = dict(self._data)
  59          try:
  60              SETTINGS_FILE.write_text(json.dumps(data, indent=2), encoding="utf-8")
  61          except OSError as e:
  62              print("Could not save settings to %s: %s" % (SETTINGS_FILE, e))
  63          return data
  64  
  65  
  66  class EventHub:
  67      """Fans server-sent events out to every open browser tab and keeps recent log lines."""
  68  
  69      def __init__(self):
  70          self._lock = threading.Lock()
  71          self._clients = set()
  72          self.logs = {"build": collections.deque(maxlen=3000), "stream": collections.deque(maxlen=3000),
  73                       "hardware": collections.deque(maxlen=3000), "matlab": collections.deque(maxlen=3000)}
  74  
  75      def subscribe(self):
  76          q = queue.Queue(maxsize=2000)
  77          with self._lock:
  78              self._clients.add(q)
  79          return q
  80  
  81      def unsubscribe(self, q):
  82          with self._lock:
  83              self._clients.discard(q)
  84  
  85      def publish(self, event, data):
  86          msg = "event: %s\ndata: %s\n\n" % (event, json.dumps(data, allow_nan=False))
  87          with self._lock:
  88              clients = list(self._clients)
  89          for q in clients:
  90              try:
  91                  q.put_nowait(msg)
  92              except queue.Full:
  93                  pass  # a stalled tab just misses messages
  94  
  95      def log(self, channel, line):
  96          entry = {"channel": channel, "time": time.time(), "line": line}
  97          with self._lock:
  98              self.logs[channel].append(entry)
  99          self.publish("log", entry)
 100  
 101      def history(self):
 102          with self._lock:
 103              return {k: list(v) for k, v in self.logs.items()}
 104  
 105  
 106  settings = Settings()
 107  
 108  
 109  hub = EventHub()
 110  
 111  
 112  def run_logged(cmd, channel, cwd=None):
 113      """Run a command, streaming its output to a log channel. Returns the exit code."""
 114      hub.log(channel, "$ " + quote_cmd(cmd))
 115      try:
 116          proc = subprocess.Popen([str(c) for c in cmd], cwd=cwd, stdout=subprocess.PIPE,
 117                                  stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
 118      except OSError as e:
 119          hub.log(channel, "Could not run %s: %s" % (cmd[0], e))
 120          return -1
 121      for raw in proc.stdout:
 122          hub.log(channel, decode(raw))
 123      return proc.wait()
 124  
 125  
 126  def run_quiet(cmd, timeout=30, cwd=None):
 127      try:
 128          r = subprocess.run([str(c) for c in cmd], stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
 129                             stdin=subprocess.DEVNULL, timeout=timeout, cwd=cwd)
 130          return r.returncode, decode(r.stdout)
 131      except (OSError, subprocess.TimeoutExpired) as e:
 132          return -1, str(e)
```

</details>
<!-- /listing -->
