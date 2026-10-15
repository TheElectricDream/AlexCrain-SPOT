# hardware.py

**File:** `+spot/+console/webapp/spot_console/hardware.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Lab Console: Hardware Tab|Lab-Console-Hardware-Tab]] · **On the platforms:** [[spot_agent.sh|Hardware-Deploy-Scripts]]

<!-- source: +spot/+console/webapp/spot_console/hardware.py -->

## What it is for

Driving the platforms' Teensies from the lab console: the Hardware tab's pucks, thrusters and thruster checks, Interactive Mode's flights, and the calibrations' runs ([[calibration.py|Console-calibration]]). The console logs into each platform over SSH with a key that can only run `spot_agent.sh`; `stream` there copies stdin to the Teensy's port, so the console writes command packets into the SSH session. Also the helpers the experiments use to run agent commands on several platforms at once.

## Function by function

**Lines 21–39.** Constants: 8 thrusters; packets are re-sent every 0.2 s while anything is on (the Teensy switches off after 2 s without one, so if the console or the network dies the platform shuts down by itself); after everything is off, "off" is kept being sent for 1 s before the link is dropped; status checks every 4 s idle / 2 s while driving; the SSH key `~/.ssh/id_ed25519_spot_console`; the name the console's own link shows up as; the setup hint the errors give.

**Lines 42–43.** `teensy_packet`: `0xAA` + nine little-endian floats (the same 37 bytes as [[spot.hardware.packCommand|spot-hardware-packCommand]]).

**Lines 46–65.** `platform_ssh`: the ssh command line (key only, batch mode, 4 s connect timeout, keep-alives so a dead link is noticed within ~3 s); `describe_ssh_failure` turns ssh's output into a state (`offline`, `denied`, `error`) and a readable reason.

**Lines 72–465, `Platform`** (one per RED/BLACK/BLUE):
- state (75–91): the duty cycles and pucks that *should* be on, a running thruster check, the last action, the ssh link process, the owner (Interactive Mode's session or a calibration, whose `label` is "Interactive Mode" or "Calibration"), and the latest probe result.
- `status` (100–113): what the pages show, including `interactive` (someone owns it) and `owner` (the owner's label, or None).
- `set_pucks`, `set_thruster`, `all_off`, `start_check`, `cancel_check` (125–215): the Hardware tab's commands. Switching something on requires `_require_ready` (not checking, reachable, Teensy present, nobody else using it) and `_require_manual` (no owner; its error names who drives the platform and where to stop it, the PhaseSpace tab or the Calibration tab); switching off always works and ends Interactive Mode or the calibration. A thruster check fires each thruster at 100 % in turn for `duration` seconds with the pucks off.
- `claim`, `reserve`, `owned_by`, `drive`, `release` (217–279): the hand-over to Interactive Mode or a calibration: `claim` (thrusters off, pucks on, owner set; the note names the owner's label); `reserve` (the arm calibration: the owner gets the platform with everything off and keeps it off, since the Chaser must rest while its arm moves; refused while a thruster check runs, when the platform doesn't answer, or while it floats or fires; Esc or all off takes it back, as for `claim`); `owned_by(owner)` (whether OWNER still has it); `drive(owner, duty, pucks=True)` sets the eight duty cycles and the pucks many times a second (publishing to the pages only every 0.2 s; a calibration turns the pucks off to end a drift) and returns False once the platform was taken away, `release` switches off.
- `reconfigure`, `shutdown`, `make_way`, `send_off` (281–319): address changed; console stopping; before an experiment starts (everything off, let go of the port at once); after it stops (three all-off packets so nothing stays on until the watchdog).
- `_drive_loop` (321–358): the sender thread: advance a thruster check, then send the packet if anything is on (or within the 1 s hold), else close the link; sleep until the next period or a wake-up.
- `_send`, `_close_link`, `_watch_link`, `_link_lost` (360–418): open the `stream` ssh session on first use and write packets into its stdin; close by closing stdin (the remote `cat` exits); a link that dies unexpectedly switches everything off and records the reason.
- `_probe_loop`, `_probe` (420–465): the status check: no key → `nokey`; run `status` on the platform; parse `teensy yes/no` and the other programs using the port. If someone else opens the Teensy while the console is driving (seen twice in a row), the console lets go rather than fight.

**Lines 468–495.** `Hardware`: the three platforms, started together; `get` by name; status; all off; shutdown.

**Lines 498–513.** `parse_platforms`: a change of a platform's host/user from the page, validated.

**Lines 516–570.** For the experiments: `platform_run` (one agent command with optional stdin data, returning code, stdout bytes and cleaned stderr), `StepFailed` (a falsy result for a step that raised) and `in_parallel` (a function on every platform at once, in threads).

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/hardware.py</b> (570 lines; click to show)</summary>

```python
   1  """The platforms' hardware, from the lab console: Teensy command packets, the SSH link to each
   2  platform's agent (~/.spot/agent.sh), pucks, thrusters and thruster checks, handing a platform to
   3  Interactive Mode (interactive.py) or a calibration (calibration.py), and running agent commands on
   4  several platforms at once.
   5  
   6  Part of the SPOT Console (webapp/app.py starts it).
   7  """
   8  
   9  import collections
  10  import re
  11  import struct
  12  import subprocess
  13  import threading
  14  import time
  15  from pathlib import Path
  16  
  17  from .common import ApiError, HOST_RE, USER_RE, decode, shown
  18  from .events import DEFAULT_PLATFORMS, hub, run_quiet, settings
  19  
  20  
  21  NUM_THRUSTERS = 8
  22  
  23  
  24  HW_SEND_PERIOD = 0.2
  25  
  26  
  27  HW_IDLE_HOLD = 1.0  # after everything is off, keep sending "off" this long before letting go of the port
  28  
  29  
  30  HW_PROBE_PERIOD = {False: 4.0, True: 2.0}  # seconds between status checks: idle / while driving
  31  
  32  
  33  HW_KEY = Path.home() / ".ssh" / "id_ed25519_spot_console"  # see +spot/+console/deploy/setup_hardware_link.sh
  34  
  35  
  36  HW_LINK_NAME = "spot-teensy-link"  # how this app's own link shows up in a platform's "status"
  37  
  38  
  39  HW_SETUP_HINT = "run +spot/+console/deploy/setup_hardware_link.sh from a lab computer (see STATUS.md in the project folder)"
  40  
  41  
  42  def teensy_packet(duty, pucks):
  43      return struct.pack("<B9f", 0xAA, *([float(d) for d in duty] + [1.0 if pucks else 0.0]))
  44  
  45  
  46  def platform_ssh(cfg, command):
  47      return ["ssh", "-i", str(HW_KEY), "-o", "IdentitiesOnly=yes", "-o", "BatchMode=yes",
  48              "-o", "ConnectTimeout=4", "-o", "StrictHostKeyChecking=accept-new",
  49              # a dead link is noticed within ~3 s (the Teensy has already switched off after 2 s)
  50              "-o", "ServerAliveInterval=1", "-o", "ServerAliveCountMax=3",
  51              "%s@%s" % (cfg["user"], cfg["host"]), command]
  52  
  53  
  54  def describe_ssh_failure(cfg, rc, out):
  55      """(state, message) for a failed status check."""
  56      where = "%s@%s" % (cfg["user"], cfg["host"])
  57      if rc == -1 or re.search(r"timed out|No route|unreachable|Connection refused|Could not resolve", out):
  58          return "offline", "%s is not answering (switched off, or not on the network?)." % where
  59      if "Permission denied" in out:
  60          return "denied", "%s doesn't accept this console's key; %s." % (where, HW_SETUP_HINT)
  61      if "Host key verification failed" in out or "REMOTE HOST IDENTIFICATION HAS CHANGED" in out:
  62          return "error", ("%s's host key changed (reinstalled?). On the TX2 run: ssh-keygen -R %s"
  63                           % (where, cfg["host"]))
  64      lines = [l for l in out.splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
  65      return "error", "%s: %s" % (where, lines[-1] if lines else "status check failed (code %s)" % rc)
  66  
  67  
  68  def hw_log(line):
  69      hub.log("hardware", "%s  %s" % (time.strftime("%H:%M:%S"), line))
  70  
  71  
  72  class Platform:
  73      """One platform's Teensy: what should be on, the SSH link that sends it, and a status check."""
  74  
  75      def __init__(self, name):
  76          self.name = name
  77          self._lock = threading.Lock()
  78          self._wake = threading.Event()        # makes the driver send a change right away
  79          self._probe_wake = threading.Event()  # makes the status check run right away
  80          self.duty = [0.0] * NUM_THRUSTERS
  81          self.pucks = False
  82          self.check = None       # running thruster check: {"duration", "started", "index", "by"}
  83          self.last_check = None  # how the last one ended: {"ok", "finished", "note"}
  84          self.last_action = None  # {"text", "by", "time"}
  85          self.link = None        # ssh process copying packets to the Teensy, while driving
  86          self.link_error = None
  87          self._release = False   # let go of the port now, without the idle hold
  88          self.owner = None       # Interactive Mode's session, or a calibration, while it drives this platform
  89                                  # (claim/drive/release); its .label says which ("Interactive Mode", "Calibration")
  90          self._published = 0.0   # when drive() last told the pages (it runs many times a second)
  91          self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [], "time": None}
  92  
  93      def config(self):
  94          return settings.get("platforms").get(self.name) or DEFAULT_PLATFORMS[self.name]
  95  
  96      def start(self):
  97          threading.Thread(target=self._drive_loop, daemon=True).start()
  98          threading.Thread(target=self._probe_loop, daemon=True).start()
  99  
 100      def status(self):
 101          cfg = self.config()
 102          with self._lock:
 103              check = None
 104              if self.check:
 105                  check = dict(self.check, total=NUM_THRUSTERS, elapsed=time.time() - self.check["started"])
 106              return {"name": self.name, "host": cfg["host"], "user": cfg["user"], "probe": dict(self.probe),
 107                      "duty": list(self.duty), "pucks": self.pucks, "check": check,
 108                      "last_check": self.last_check, "last_action": self.last_action,
 109                      "driving": self.link is not None, "link_error": self.link_error,
 110                      "interactive": self.owner is not None, "owner": self._owner_label()}
 111  
 112      def _owner_label(self):
 113          return getattr(self.owner, "label", "Interactive Mode") if self.owner is not None else None
 114  
 115      def _publish(self):
 116          hub.publish("hardware_platform", self.status())
 117  
 118      def _note(self, text, requester=None):
 119          """Record and log a change (call with the lock held). No requester: the console itself."""
 120          self.last_action = {"text": text, "by": requester, "time": time.time()}
 121          hw_log("%s: %s%s" % (shown(self.name), text, " (by %s)" % requester if requester else ""))
 122  
 123      # ---- commands (from the web page or a script)
 124  
 125      def _require_manual(self):
 126          """Raise ApiError while Interactive Mode or a calibration drives this platform (call with the
 127          lock held). Switching everything off (all_off, Esc on the Hardware tab) always works, and ends it."""
 128          if self.owner is not None:
 129              where = "the Calibration tab" if self._owner_label() == "Calibration" else "the PhaseSpace tab"
 130              raise ApiError("%s is driving the %s: stop it on %s first "
 131                             "(or press Esc on the Hardware tab to switch everything off)."
 132                             % (self._owner_label(), shown(self.name), where))
 133  
 134      def _require_ready(self):
 135          """Raise ApiError unless something may be switched on (call with the lock held)."""
 136          p = self.probe
 137          if self.check:
 138              raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
 139          if p["state"] == "checking":
 140              raise ApiError("Still checking the %s; try again in a moment." % shown(self.name))
 141          if p["state"] != "ok":
 142              raise ApiError(p["detail"])
 143          if not p["teensy"]:
 144              raise ApiError("The %s has no Teensy at /dev/ttyACM0 (unplugged?)." % shown(self.name))
 145          if p["others"]:
 146              raise ApiError("The %s's Teensy is in use by %s (an experiment?); stop that first."
 147                             % (shown(self.name), ", ".join(p["others"])))
 148  
 149      def set_pucks(self, on, requester):
 150          with self._lock:
 151              self._require_manual()
 152              if on:
 153                  self._require_ready()
 154                  self.link_error = None
 155              elif self.check:
 156                  raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
 157              if self.pucks != on:
 158                  self.pucks = on
 159                  self._note("pucks %s" % ("on" if on else "off"), requester)
 160          self._wake.set()
 161          self._publish()
 162  
 163      def set_thruster(self, index, duty, requester):
 164          with self._lock:
 165              self._require_manual()
 166              if duty > 0:
 167                  self._require_ready()
 168                  self.link_error = None
 169              elif self.check:
 170                  raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
 171              if self.duty[index] != duty:
 172                  self.duty[index] = duty
 173                  self._note("thruster %d %s" % (index + 1, "on (%g %%)" % duty if duty else "off"), requester)
 174          self._wake.set()
 175          self._publish()
 176  
 177      def _all_off_locked(self, requester, note=None):
 178          """Switch everything off (call with the lock held). Returns whether anything was on."""
 179          was_on = bool(self.pucks or any(self.duty) or self.check)
 180          self.owner = None   # also ends Interactive Mode here (it sees it has lost the platform)
 181          if self.check:
 182              self.last_check = {"ok": False, "finished": time.time(),
 183                                 "note": note or "stopped by %s" % requester}
 184              self.check = None
 185          self.duty = [0.0] * NUM_THRUSTERS
 186          self.pucks = False
 187          if was_on:
 188              self._note(note or "all off", requester)
 189          return was_on
 190  
 191      def all_off(self, requester, note=None):
 192          with self._lock:
 193              self._all_off_locked(requester, note)
 194          self._wake.set()
 195          self._publish()
 196  
 197      def start_check(self, duration, requester):
 198          with self._lock:
 199              self._require_manual()
 200              self._require_ready()
 201              self.link_error = None
 202              self.duty = [0.0] * NUM_THRUSTERS
 203              self.pucks = False  # as before: the platform sits on the table while each thruster fires
 204              self.check = {"duration": duration, "started": time.time(), "index": 0, "by": requester}
 205              self.last_check = None
 206              self._note("thruster check started, %g s per thruster" % duration, requester)
 207          self._wake.set()
 208          self._publish()
 209  
 210      def cancel_check(self, requester):
 211          with self._lock:
 212              if self.check:
 213                  self._all_off_locked(requester, "thruster check cancelled")
 214          self._wake.set()
 215          self._publish()
 216  
 217      # ---- Interactive Mode (interactive.py) or a calibration (calibration.py): one owner drives the
 218      # thrusters and pucks at its own rate
 219  
 220      def claim(self, owner, requester):
 221          """Hand this platform to OWNER (Interactive Mode's session, or a calibration): thrusters off,
 222          pucks on."""
 223          with self._lock:
 224              self._require_manual()
 225              self._require_ready()
 226              self.owner = owner
 227              self.link_error = None
 228              self.duty = [0.0] * NUM_THRUSTERS
 229              self.pucks = True
 230              self._note("%s: pucks on, flying" % self._owner_label(), requester)
 231          self._wake.set()
 232          self._publish()
 233  
 234      def reserve(self, owner, requester):
 235          """Hand this platform to OWNER with everything off, and keep it off (the arm calibration: the
 236          platform must rest while its arm moves). Nothing else can switch anything on meanwhile, and
 237          switching it off (Esc, the Hardware tab) takes it back, as for claim."""
 238          with self._lock:
 239              self._require_manual()
 240              if self.check:
 241                  raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
 242              if self.probe["state"] != "ok":
 243                  raise ApiError(self.probe["detail"] or "The %s doesn't answer." % shown(self.name))
 244              if self.pucks or any(self.duty):
 245                  raise ApiError("The %s is floating or firing; switch it off first (Hardware tab)." % shown(self.name))
 246              self.owner = owner
 247              self._note("%s: kept resting (nothing on)" % self._owner_label(), requester)
 248          self._publish()
 249  
 250      def owned_by(self, owner):
 251          with self._lock:
 252              return self.owner is owner
 253  
 254      def drive(self, owner, duty, pucks=True):
 255          """Set the 8 thruster duty cycles [%] and the pucks, sent at once. Returns False, and does
 256          nothing, once OWNER no longer has the platform (someone switched it off, the link was
 257          lost, another program opened the Teensy)."""
 258          with self._lock:
 259              if self.owner is not owner:
 260                  return False
 261              self.duty = [min(100.0, max(0.0, float(d))) for d in duty]
 262              self.pucks = bool(pucks)
 263              now = time.time()
 264              publish = now - self._published >= 0.2
 265              if publish:
 266                  self._published = now
 267          self._wake.set()
 268          if publish:
 269              self._publish()
 270          return True
 271  
 272      def release(self, owner, note):
 273          """OWNER is done: everything off, if it still has the platform."""
 274          with self._lock:
 275              if self.owner is not owner:
 276                  return
 277              self._all_off_locked(None, note)
 278          self._wake.set()
 279          self._publish()
 280  
 281      def reconfigure(self):
 282          """The address changed: drop the link to the old one and check the new one."""
 283          with self._lock:
 284              self._all_off_locked(None, "all off (address changed)")
 285              self._release = True
 286              self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [], "time": None}
 287          self._wake.set()
 288          self._probe_wake.set()
 289          self._publish()
 290  
 291      def shutdown(self):
 292          with self._lock:
 293              self.duty, self.pucks, self.check, self.owner = [0.0] * NUM_THRUSTERS, False, None, None
 294              driving = self.link is not None
 295          if driving:
 296              self._send(teensy_packet([0.0] * NUM_THRUSTERS, False))
 297              self._close_link()
 298  
 299      def make_way(self, requester):
 300          """Before an experiment starts here: everything off, and let go of the Teensy right away."""
 301          with self._lock:
 302              self._all_off_locked(None, "all off: an experiment is starting (by %s)" % requester)
 303              self._release = True
 304          self._wake.set()
 305          self._publish()
 306  
 307      def send_off(self):
 308          """After an experiment has stopped here: a short burst of all-off packets, so nothing stays
 309          on until the Teensy's 2 s watchdog (the old GUI's CleanShutdown). Skipped if the console is
 310          driving it. (The last status check may still list the program that just stopped.)"""
 311          with self._lock:
 312              if self.pucks or any(self.duty) or self.check:
 313                  return
 314          zeros = teensy_packet([0.0] * NUM_THRUSTERS, False)
 315          for _ in range(3):
 316              self._send(zeros)
 317              time.sleep(0.2)
 318          self._close_link()
 319          self._probe_wake.set()   # show the Teensy as free again soon
 320  
 321      # ---- driver: sends the packet every HW_SEND_PERIOD while anything is on
 322  
 323      def _drive_loop(self):
 324          last_on = 0.0
 325          while True:
 326              now = time.time()
 327              wait, changed = HW_SEND_PERIOD, False
 328              with self._lock:
 329                  c = self.check
 330                  if c:
 331                      index = int((now - c["started"]) / c["duration"])
 332                      if index >= NUM_THRUSTERS:
 333                          self.check, self.duty = None, [0.0] * NUM_THRUSTERS
 334                          self.last_check = {"ok": True, "finished": now, "note": "all %d thrusters fired for %g s each"
 335                                             % (NUM_THRUSTERS, c["duration"])}
 336                          hw_log("%s: thruster check finished" % shown(self.name))
 337                          changed = True
 338                      else:
 339                          changed = index != c["index"]
 340                          c["index"] = index
 341                          self.duty = [100.0 if i == index else 0.0 for i in range(NUM_THRUSTERS)]
 342                          wait = min(wait, c["started"] + (index + 1) * c["duration"] - now)
 343                  duty, pucks, release = list(self.duty), self.pucks, self._release
 344                  self._release = False
 345              on = pucks or any(d > 0 for d in duty)
 346              if on:
 347                  last_on = now
 348              if release:
 349                  last_on = 0.0
 350                  self._close_link()
 351              elif on or now - last_on < HW_IDLE_HOLD:
 352                  self._send(teensy_packet(duty, pucks))
 353              elif self.link is not None:
 354                  self._close_link()
 355              if changed:
 356                  self._publish()
 357              self._wake.wait(max(0.005, wait))
 358              self._wake.clear()
 359  
 360      def _send(self, packet):
 361          with self._lock:
 362              proc = self.link
 363          if proc is None:
 364              try:
 365                  proc = subprocess.Popen(platform_ssh(self.config(), "stream"), stdin=subprocess.PIPE,
 366                                          stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, bufsize=0)
 367              except OSError as e:
 368                  self._link_lost("could not run ssh (%s)" % e)
 369                  return
 370              with self._lock:
 371                  self.link = proc
 372              threading.Thread(target=self._watch_link, args=(proc,), daemon=True).start()
 373              self._probe_wake.set()  # check more often while driving
 374              self._publish()
 375          try:
 376              proc.stdin.write(packet)  # buffered by the pipe while ssh is still connecting
 377          except (OSError, ValueError):
 378              pass  # _watch_link reports why the link closed
 379  
 380      def _close_link(self):
 381          with self._lock:
 382              proc, self.link = self.link, None
 383          if proc is None:
 384              return
 385          try:
 386              proc.stdin.close()  # the Teensy end (cat) sees end of input and exits
 387          except OSError:
 388              pass
 389          try:
 390              proc.wait(timeout=3)
 391          except subprocess.TimeoutExpired:
 392              proc.kill()
 393          self._publish()
 394  
 395      def _watch_link(self, proc):
 396          err = decode(proc.stderr.read())
 397          rc = proc.wait()
 398          with self._lock:
 399              expected = self.link is not proc  # _close_link clears it first
 400              if not expected:
 401                  self.link = None
 402          if expected:
 403              return
 404          lines = [l for l in err.splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
 405          self._link_lost(lines[-1] if lines else "link closed (code %s)" % rc)
 406  
 407      def _link_lost(self, reason):
 408          # Whatever was on stops anyway (Teensy watchdog); clearing it here means nothing comes back
 409          # on by itself when the link returns
 410          note = "link to the Teensy lost (%s); all off" % reason
 411          with self._lock:
 412              self.link_error = reason
 413              self._release = True  # no "off" hold over a new link: the Teensy is already off
 414              if not self._all_off_locked(None, note):
 415                  hw_log("%s: %s" % (shown(self.name), note))
 416          self._wake.set()
 417          self._publish()
 418          self._probe_wake.set()
 419  
 420      # ---- status check: reachable? Teensy plugged in? anyone else using it?
 421  
 422      def _probe_loop(self):
 423          while True:
 424              soon = self._probe()
 425              with self._lock:
 426                  driving = self.link is not None
 427              self._probe_wake.wait(0.5 if soon else HW_PROBE_PERIOD[driving])
 428              self._probe_wake.clear()
 429  
 430      def _probe(self):
 431          cfg = self.config()
 432          result = {"state": "ok", "detail": None, "teensy": None, "others": [], "time": time.time()}
 433          if not HW_KEY.exists():
 434              result.update(state="nokey", detail="This console has no SSH key for the platforms yet; %s." % HW_SETUP_HINT)
 435          else:
 436              rc, out = run_quiet(platform_ssh(cfg, "status"), timeout=10)
 437              if rc != 0 or not re.search(r"^teensy (yes|no)$", out, re.M):
 438                  result["state"], result["detail"] = describe_ssh_failure(cfg, rc, out)
 439              else:
 440                  result["teensy"] = bool(re.search(r"^teensy yes$", out, re.M))
 441                  result["others"] = sorted({m.group(2) or "pid %s" % m.group(1)
 442                                             for m in re.finditer(r"^user (\d+) ?(.*)$", out, re.M)
 443                                             if m.group(2) != HW_LINK_NAME})
 444          soon = False
 445          with self._lock:
 446              old = self.probe
 447              self.probe = result
 448              changed = any(old.get(k) != result[k] for k in ("state", "detail", "teensy", "others"))
 449              active = self.link is not None or self.pucks or any(self.duty) or self.check
 450              if result["others"] and active:
 451                  if old["others"]:
 452                      # Seen twice in a row (e.g. an experiment started): let it have the Teensy
 453                      # instead of fighting over it
 454                      self._all_off_locked(None, "%s opened the Teensy, so this console let go of it; all off"
 455                                           % ", ".join(result["others"]))
 456                      self._release = True
 457                      changed = True
 458                  else:
 459                      soon = True  # confirm quickly before letting go
 460          if changed:
 461              if old["state"] != "checking" and old["state"] != result["state"]:
 462                  hw_log("%s: %s" % (shown(self.name), "reachable" if result["state"] == "ok" else result["detail"]))
 463              self._wake.set()
 464              self._publish()
 465          return soon
 466  
 467  
 468  class Hardware:
 469      def __init__(self):
 470          self.platforms = collections.OrderedDict((n, Platform(n)) for n in DEFAULT_PLATFORMS)
 471  
 472      def start(self):
 473          for p in self.platforms.values():
 474              p.start()
 475  
 476      def get(self, name):
 477          p = self.platforms.get(str(name or "").strip().upper())
 478          if p is None:
 479              raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
 480          return p
 481  
 482      def status(self):
 483          return {"platforms": [p.status() for p in self.platforms.values()],
 484                  "key": str(HW_KEY), "key_exists": HW_KEY.exists()}
 485  
 486      def all_off(self, requester):
 487          for p in self.platforms.values():
 488              p.all_off(requester)
 489  
 490      def shutdown(self):
 491          for p in self.platforms.values():
 492              p.shutdown()
 493  
 494  
 495  hardware = Hardware()
 496  
 497  
 498  def parse_platforms(value):
 499      """Validate a {"RED": {"host": ..., "user": ...}, ...} settings change (merged into the current one)."""
 500      if not isinstance(value, dict):
 501          raise ApiError("platforms must be an object like {\"RED\": {\"host\": ..., \"user\": ...}}.")
 502      out = {n: dict(c) for n, c in settings.get("platforms").items()}
 503      for name, cfg in value.items():
 504          name = str(name).upper()
 505          if name not in DEFAULT_PLATFORMS or not isinstance(cfg, dict):
 506              raise ApiError("Unknown platform %r." % name)
 507          host, user = str(cfg.get("host", "")).strip(), str(cfg.get("user", "")).strip()
 508          if not HOST_RE.match(host):
 509              raise ApiError("%s: %r is not an IP address or host name." % (shown(name), host))
 510          if not USER_RE.match(user):
 511              raise ApiError("%s: %r is not a user name." % (shown(name), user))
 512          out[name] = {"host": host, "user": user}
 513      return out
 514  
 515  
 516  # The local console (SPOT in MATLAB) builds a diagram with this computer as the "board": MATLAB copies
 517  # the generated code here and compiles it under EXP_BUILD_ROOT. This console then copies only the
 518  # program (and any shared libraries next to it) to each platform that takes part, starts and stops
 519  # it there, and collects the data files it writes. Everything on the platforms goes through
 520  # spot_agent.sh, the only thing this computer's key may run there.
 521  
 522  
 523  def platform_run(name, command, data=None, timeout=60):
 524      """Run one spot_agent.sh command on a platform: (exit code, stdout bytes, stderr text)."""
 525      cfg = hardware.get(name).config()
 526      # Python 3.6 (the TX2) refuses input= together with any stdin= argument, even stdin=None
 527      io_args = {"input": data} if data is not None else {"stdin": subprocess.DEVNULL}
 528      try:
 529          r = subprocess.run(platform_ssh(cfg, command), timeout=timeout,
 530                             stdout=subprocess.PIPE, stderr=subprocess.PIPE, **io_args)
 531          err = [l for l in decode(r.stderr).splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
 532          return r.returncode, r.stdout, "\n".join(err)
 533      except subprocess.TimeoutExpired:
 534          return -1, b"", "no answer within %d s" % timeout
 535      except OSError as e:
 536          return -1, b"", str(e)
 537  
 538  
 539  class StepFailed(object):
 540      """What in_parallel returns for a platform whose step raised an exception (it is falsy)."""
 541  
 542      def __init__(self, error):
 543          self.error = error
 544  
 545      def __bool__(self):
 546          return False
 547  
 548      def __repr__(self):
 549          return "StepFailed(%r)" % self.error
 550  
 551  
 552  def in_parallel(names, fn):
 553      """fn(name) for each platform at once; returns {name: result} for every name. A step that
 554      raises gives a (falsy) StepFailed instead of vanishing, so it can't count as a success."""
 555      results, threads = {}, []
 556  
 557      def run(n):
 558          try:
 559              results[n] = fn(n)
 560          except Exception as e:  # report it rather than lose it in a background thread
 561              results[n] = StepFailed("%s: %s" % (type(e).__name__, e))
 562              hw_log("%s: internal error: %s" % (shown(n), results[n].error))
 563  
 564      for n in names:
 565          t = threading.Thread(target=run, args=(n,), daemon=True)
 566          t.start()
 567          threads.append(t)
 568      for t in threads:
 569          t.join()
 570      return results
```

</details>
<!-- /listing -->
