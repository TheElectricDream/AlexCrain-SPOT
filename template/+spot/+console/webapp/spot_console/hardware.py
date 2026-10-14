"""The platforms' hardware, from the lab console: Teensy command packets, the SSH link to each
platform's agent (~/.spot/agent.sh), pucks, thrusters and thruster checks, handing a platform to
Interactive Mode (interactive.py) or a calibration (calibration.py), and running agent commands on
several platforms at once.

Part of the SPOT Console (webapp/app.py starts it).
"""

import collections
import re
import struct
import subprocess
import threading
import time
from pathlib import Path

from .common import ApiError, HOST_RE, USER_RE, decode, shown
from .events import DEFAULT_PLATFORMS, hub, run_quiet, settings


NUM_THRUSTERS = 8


HW_SEND_PERIOD = 0.2


HW_IDLE_HOLD = 1.0  # after everything is off, keep sending "off" this long before letting go of the port


HW_PROBE_PERIOD = {False: 4.0, True: 2.0}  # seconds between status checks: idle / while driving


HW_KEY = Path.home() / ".ssh" / "id_ed25519_spot_console"  # see +spot/+console/deploy/setup_hardware_link.sh


HW_LINK_NAME = "spot-teensy-link"  # how this app's own link shows up in a platform's "status"


HW_SETUP_HINT = "run +spot/+console/deploy/setup_hardware_link.sh from a lab computer (see STATUS.md in the project folder)"


def teensy_packet(duty, pucks):
    return struct.pack("<B9f", 0xAA, *([float(d) for d in duty] + [1.0 if pucks else 0.0]))


def platform_ssh(cfg, command):
    return ["ssh", "-i", str(HW_KEY), "-o", "IdentitiesOnly=yes", "-o", "BatchMode=yes",
            "-o", "ConnectTimeout=4", "-o", "StrictHostKeyChecking=accept-new",
            # a dead link is noticed within ~3 s (the Teensy has already switched off after 2 s)
            "-o", "ServerAliveInterval=1", "-o", "ServerAliveCountMax=3",
            "%s@%s" % (cfg["user"], cfg["host"]), command]


def describe_ssh_failure(cfg, rc, out):
    """(state, message) for a failed status check."""
    where = "%s@%s" % (cfg["user"], cfg["host"])
    if rc == -1 or re.search(r"timed out|No route|unreachable|Connection refused|Could not resolve", out):
        return "offline", "%s is not answering (switched off, or not on the network?)." % where
    if "Permission denied" in out:
        return "denied", "%s doesn't accept this console's key; %s." % (where, HW_SETUP_HINT)
    if "Host key verification failed" in out or "REMOTE HOST IDENTIFICATION HAS CHANGED" in out:
        return "error", ("%s's host key changed (reinstalled?). On the TX2 run: ssh-keygen -R %s"
                         % (where, cfg["host"]))
    lines = [l for l in out.splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
    return "error", "%s: %s" % (where, lines[-1] if lines else "status check failed (code %s)" % rc)


def hw_log(line):
    hub.log("hardware", "%s  %s" % (time.strftime("%H:%M:%S"), line))


class Platform:
    """One platform's Teensy: what should be on, the SSH link that sends it, and a status check."""

    def __init__(self, name):
        self.name = name
        self._lock = threading.Lock()
        self._wake = threading.Event()        # makes the driver send a change right away
        self._probe_wake = threading.Event()  # makes the status check run right away
        self.duty = [0.0] * NUM_THRUSTERS
        self.pucks = False
        self.check = None       # running thruster check: {"duration", "started", "index", "by"}
        self.last_check = None  # how the last one ended: {"ok", "finished", "note"}
        self.last_action = None  # {"text", "by", "time"}
        self.link = None        # ssh process copying packets to the Teensy, while driving
        self.link_error = None
        self._release = False   # let go of the port now, without the idle hold
        self.owner = None       # Interactive Mode's session, or a calibration, while it drives this platform
                                # (claim/drive/release); its .label says which ("Interactive Mode", "Calibration")
        self._published = 0.0   # when drive() last told the pages (it runs many times a second)
        self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [], "time": None}

    def config(self):
        return settings.get("platforms").get(self.name) or DEFAULT_PLATFORMS[self.name]

    def start(self):
        threading.Thread(target=self._drive_loop, daemon=True).start()
        threading.Thread(target=self._probe_loop, daemon=True).start()

    def status(self):
        cfg = self.config()
        with self._lock:
            check = None
            if self.check:
                check = dict(self.check, total=NUM_THRUSTERS, elapsed=time.time() - self.check["started"])
            return {"name": self.name, "host": cfg["host"], "user": cfg["user"], "probe": dict(self.probe),
                    "duty": list(self.duty), "pucks": self.pucks, "check": check,
                    "last_check": self.last_check, "last_action": self.last_action,
                    "driving": self.link is not None, "link_error": self.link_error,
                    "interactive": self.owner is not None, "owner": self._owner_label()}

    def _owner_label(self):
        return getattr(self.owner, "label", "Interactive Mode") if self.owner is not None else None

    def _publish(self):
        hub.publish("hardware_platform", self.status())

    def _note(self, text, requester=None):
        """Record and log a change (call with the lock held). No requester: the console itself."""
        self.last_action = {"text": text, "by": requester, "time": time.time()}
        hw_log("%s: %s%s" % (shown(self.name), text, " (by %s)" % requester if requester else ""))

    # ---- commands (from the web page or a script)

    def _require_manual(self):
        """Raise ApiError while Interactive Mode or a calibration drives this platform (call with the
        lock held). Switching everything off (all_off, Esc on the Hardware tab) always works, and ends it."""
        if self.owner is not None:
            where = "the Calibration tab" if self._owner_label() == "Calibration" else "the PhaseSpace tab"
            raise ApiError("%s is driving the %s: stop it on %s first "
                           "(or press Esc on the Hardware tab to switch everything off)."
                           % (self._owner_label(), shown(self.name), where))

    def _require_ready(self):
        """Raise ApiError unless something may be switched on (call with the lock held)."""
        p = self.probe
        if self.check:
            raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
        if p["state"] == "checking":
            raise ApiError("Still checking the %s; try again in a moment." % shown(self.name))
        if p["state"] != "ok":
            raise ApiError(p["detail"])
        if not p["teensy"]:
            raise ApiError("The %s has no Teensy at /dev/ttyACM0 (unplugged?)." % shown(self.name))
        if p["others"]:
            raise ApiError("The %s's Teensy is in use by %s (an experiment?); stop that first."
                           % (shown(self.name), ", ".join(p["others"])))

    def set_pucks(self, on, requester):
        with self._lock:
            self._require_manual()
            if on:
                self._require_ready()
                self.link_error = None
            elif self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
            if self.pucks != on:
                self.pucks = on
                self._note("pucks %s" % ("on" if on else "off"), requester)
        self._wake.set()
        self._publish()

    def set_thruster(self, index, duty, requester):
        with self._lock:
            self._require_manual()
            if duty > 0:
                self._require_ready()
                self.link_error = None
            elif self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
            if self.duty[index] != duty:
                self.duty[index] = duty
                self._note("thruster %d %s" % (index + 1, "on (%g %%)" % duty if duty else "off"), requester)
        self._wake.set()
        self._publish()

    def _all_off_locked(self, requester, note=None):
        """Switch everything off (call with the lock held). Returns whether anything was on."""
        was_on = bool(self.pucks or any(self.duty) or self.check)
        self.owner = None   # also ends Interactive Mode here (it sees it has lost the platform)
        if self.check:
            self.last_check = {"ok": False, "finished": time.time(),
                               "note": note or "stopped by %s" % requester}
            self.check = None
        self.duty = [0.0] * NUM_THRUSTERS
        self.pucks = False
        if was_on:
            self._note(note or "all off", requester)
        return was_on

    def all_off(self, requester, note=None):
        with self._lock:
            self._all_off_locked(requester, note)
        self._wake.set()
        self._publish()

    def start_check(self, duration, requester):
        with self._lock:
            self._require_manual()
            self._require_ready()
            self.link_error = None
            self.duty = [0.0] * NUM_THRUSTERS
            self.pucks = False  # as before: the platform sits on the table while each thruster fires
            self.check = {"duration": duration, "started": time.time(), "index": 0, "by": requester}
            self.last_check = None
            self._note("thruster check started, %g s per thruster" % duration, requester)
        self._wake.set()
        self._publish()

    def cancel_check(self, requester):
        with self._lock:
            if self.check:
                self._all_off_locked(requester, "thruster check cancelled")
        self._wake.set()
        self._publish()

    # ---- Interactive Mode (interactive.py) or a calibration (calibration.py): one owner drives the
    # thrusters and pucks at its own rate

    def claim(self, owner, requester):
        """Hand this platform to OWNER (Interactive Mode's session, or a calibration): thrusters off,
        pucks on."""
        with self._lock:
            self._require_manual()
            self._require_ready()
            self.owner = owner
            self.link_error = None
            self.duty = [0.0] * NUM_THRUSTERS
            self.pucks = True
            self._note("%s: pucks on, flying" % self._owner_label(), requester)
        self._wake.set()
        self._publish()

    def reserve(self, owner, requester):
        """Hand this platform to OWNER with everything off, and keep it off (the arm calibration: the
        platform must rest while its arm moves). Nothing else can switch anything on meanwhile, and
        switching it off (Esc, the Hardware tab) takes it back, as for claim."""
        with self._lock:
            self._require_manual()
            if self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first." % shown(self.name))
            if self.probe["state"] != "ok":
                raise ApiError(self.probe["detail"] or "The %s doesn't answer." % shown(self.name))
            if self.pucks or any(self.duty):
                raise ApiError("The %s is floating or firing; switch it off first (Hardware tab)." % shown(self.name))
            self.owner = owner
            self._note("%s: kept resting (nothing on)" % self._owner_label(), requester)
        self._publish()

    def owned_by(self, owner):
        with self._lock:
            return self.owner is owner

    def drive(self, owner, duty, pucks=True):
        """Set the 8 thruster duty cycles [%] and the pucks, sent at once. Returns False, and does
        nothing, once OWNER no longer has the platform (someone switched it off, the link was
        lost, another program opened the Teensy)."""
        with self._lock:
            if self.owner is not owner:
                return False
            self.duty = [min(100.0, max(0.0, float(d))) for d in duty]
            self.pucks = bool(pucks)
            now = time.time()
            publish = now - self._published >= 0.2
            if publish:
                self._published = now
        self._wake.set()
        if publish:
            self._publish()
        return True

    def release(self, owner, note):
        """OWNER is done: everything off, if it still has the platform."""
        with self._lock:
            if self.owner is not owner:
                return
            self._all_off_locked(None, note)
        self._wake.set()
        self._publish()

    def reconfigure(self):
        """The address changed: drop the link to the old one and check the new one."""
        with self._lock:
            self._all_off_locked(None, "all off (address changed)")
            self._release = True
            self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [], "time": None}
        self._wake.set()
        self._probe_wake.set()
        self._publish()

    def shutdown(self):
        with self._lock:
            self.duty, self.pucks, self.check, self.owner = [0.0] * NUM_THRUSTERS, False, None, None
            driving = self.link is not None
        if driving:
            self._send(teensy_packet([0.0] * NUM_THRUSTERS, False))
            self._close_link()

    def make_way(self, requester):
        """Before an experiment starts here: everything off, and let go of the Teensy right away."""
        with self._lock:
            self._all_off_locked(None, "all off: an experiment is starting (by %s)" % requester)
            self._release = True
        self._wake.set()
        self._publish()

    def send_off(self):
        """After an experiment has stopped here: a short burst of all-off packets, so nothing stays
        on until the Teensy's 2 s watchdog (the old GUI's CleanShutdown). Skipped if the console is
        driving it. (The last status check may still list the program that just stopped.)"""
        with self._lock:
            if self.pucks or any(self.duty) or self.check:
                return
        zeros = teensy_packet([0.0] * NUM_THRUSTERS, False)
        for _ in range(3):
            self._send(zeros)
            time.sleep(0.2)
        self._close_link()
        self._probe_wake.set()   # show the Teensy as free again soon

    # ---- driver: sends the packet every HW_SEND_PERIOD while anything is on

    def _drive_loop(self):
        last_on = 0.0
        while True:
            now = time.time()
            wait, changed = HW_SEND_PERIOD, False
            with self._lock:
                c = self.check
                if c:
                    index = int((now - c["started"]) / c["duration"])
                    if index >= NUM_THRUSTERS:
                        self.check, self.duty = None, [0.0] * NUM_THRUSTERS
                        self.last_check = {"ok": True, "finished": now, "note": "all %d thrusters fired for %g s each"
                                           % (NUM_THRUSTERS, c["duration"])}
                        hw_log("%s: thruster check finished" % shown(self.name))
                        changed = True
                    else:
                        changed = index != c["index"]
                        c["index"] = index
                        self.duty = [100.0 if i == index else 0.0 for i in range(NUM_THRUSTERS)]
                        wait = min(wait, c["started"] + (index + 1) * c["duration"] - now)
                duty, pucks, release = list(self.duty), self.pucks, self._release
                self._release = False
            on = pucks or any(d > 0 for d in duty)
            if on:
                last_on = now
            if release:
                last_on = 0.0
                self._close_link()
            elif on or now - last_on < HW_IDLE_HOLD:
                self._send(teensy_packet(duty, pucks))
            elif self.link is not None:
                self._close_link()
            if changed:
                self._publish()
            self._wake.wait(max(0.005, wait))
            self._wake.clear()

    def _send(self, packet):
        with self._lock:
            proc = self.link
        if proc is None:
            try:
                proc = subprocess.Popen(platform_ssh(self.config(), "stream"), stdin=subprocess.PIPE,
                                        stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, bufsize=0)
            except OSError as e:
                self._link_lost("could not run ssh (%s)" % e)
                return
            with self._lock:
                self.link = proc
            threading.Thread(target=self._watch_link, args=(proc,), daemon=True).start()
            self._probe_wake.set()  # check more often while driving
            self._publish()
        try:
            proc.stdin.write(packet)  # buffered by the pipe while ssh is still connecting
        except (OSError, ValueError):
            pass  # _watch_link reports why the link closed

    def _close_link(self):
        with self._lock:
            proc, self.link = self.link, None
        if proc is None:
            return
        try:
            proc.stdin.close()  # the Teensy end (cat) sees end of input and exits
        except OSError:
            pass
        try:
            proc.wait(timeout=3)
        except subprocess.TimeoutExpired:
            proc.kill()
        self._publish()

    def _watch_link(self, proc):
        err = decode(proc.stderr.read())
        rc = proc.wait()
        with self._lock:
            expected = self.link is not proc  # _close_link clears it first
            if not expected:
                self.link = None
        if expected:
            return
        lines = [l for l in err.splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
        self._link_lost(lines[-1] if lines else "link closed (code %s)" % rc)

    def _link_lost(self, reason):
        # Whatever was on stops anyway (Teensy watchdog); clearing it here means nothing comes back
        # on by itself when the link returns
        note = "link to the Teensy lost (%s); all off" % reason
        with self._lock:
            self.link_error = reason
            self._release = True  # no "off" hold over a new link: the Teensy is already off
            if not self._all_off_locked(None, note):
                hw_log("%s: %s" % (shown(self.name), note))
        self._wake.set()
        self._publish()
        self._probe_wake.set()

    # ---- status check: reachable? Teensy plugged in? anyone else using it?

    def _probe_loop(self):
        while True:
            soon = self._probe()
            with self._lock:
                driving = self.link is not None
            self._probe_wake.wait(0.5 if soon else HW_PROBE_PERIOD[driving])
            self._probe_wake.clear()

    def _probe(self):
        cfg = self.config()
        result = {"state": "ok", "detail": None, "teensy": None, "others": [], "time": time.time()}
        if not HW_KEY.exists():
            result.update(state="nokey", detail="This console has no SSH key for the platforms yet; %s." % HW_SETUP_HINT)
        else:
            rc, out = run_quiet(platform_ssh(cfg, "status"), timeout=10)
            if rc != 0 or not re.search(r"^teensy (yes|no)$", out, re.M):
                result["state"], result["detail"] = describe_ssh_failure(cfg, rc, out)
            else:
                result["teensy"] = bool(re.search(r"^teensy yes$", out, re.M))
                result["others"] = sorted({m.group(2) or "pid %s" % m.group(1)
                                           for m in re.finditer(r"^user (\d+) ?(.*)$", out, re.M)
                                           if m.group(2) != HW_LINK_NAME})
        soon = False
        with self._lock:
            old = self.probe
            self.probe = result
            changed = any(old.get(k) != result[k] for k in ("state", "detail", "teensy", "others"))
            active = self.link is not None or self.pucks or any(self.duty) or self.check
            if result["others"] and active:
                if old["others"]:
                    # Seen twice in a row (e.g. an experiment started): let it have the Teensy
                    # instead of fighting over it
                    self._all_off_locked(None, "%s opened the Teensy, so this console let go of it; all off"
                                         % ", ".join(result["others"]))
                    self._release = True
                    changed = True
                else:
                    soon = True  # confirm quickly before letting go
        if changed:
            if old["state"] != "checking" and old["state"] != result["state"]:
                hw_log("%s: %s" % (shown(self.name), "reachable" if result["state"] == "ok" else result["detail"]))
            self._wake.set()
            self._publish()
        return soon


class Hardware:
    def __init__(self):
        self.platforms = collections.OrderedDict((n, Platform(n)) for n in DEFAULT_PLATFORMS)

    def start(self):
        for p in self.platforms.values():
            p.start()

    def get(self, name):
        p = self.platforms.get(str(name or "").strip().upper())
        if p is None:
            raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
        return p

    def status(self):
        return {"platforms": [p.status() for p in self.platforms.values()],
                "key": str(HW_KEY), "key_exists": HW_KEY.exists()}

    def all_off(self, requester):
        for p in self.platforms.values():
            p.all_off(requester)

    def shutdown(self):
        for p in self.platforms.values():
            p.shutdown()


hardware = Hardware()


def parse_platforms(value):
    """Validate a {"RED": {"host": ..., "user": ...}, ...} settings change (merged into the current one)."""
    if not isinstance(value, dict):
        raise ApiError("platforms must be an object like {\"RED\": {\"host\": ..., \"user\": ...}}.")
    out = {n: dict(c) for n, c in settings.get("platforms").items()}
    for name, cfg in value.items():
        name = str(name).upper()
        if name not in DEFAULT_PLATFORMS or not isinstance(cfg, dict):
            raise ApiError("Unknown platform %r." % name)
        host, user = str(cfg.get("host", "")).strip(), str(cfg.get("user", "")).strip()
        if not HOST_RE.match(host):
            raise ApiError("%s: %r is not an IP address or host name." % (shown(name), host))
        if not USER_RE.match(user):
            raise ApiError("%s: %r is not a user name." % (shown(name), user))
        out[name] = {"host": host, "user": user}
    return out


# The local console (SPOT in MATLAB) builds a diagram with this computer as the "board": MATLAB copies
# the generated code here and compiles it under EXP_BUILD_ROOT. This console then copies only the
# program (and any shared libraries next to it) to each platform that takes part, starts and stops
# it there, and collects the data files it writes. Everything on the platforms goes through
# spot_agent.sh, the only thing this computer's key may run there.


def platform_run(name, command, data=None, timeout=60):
    """Run one spot_agent.sh command on a platform: (exit code, stdout bytes, stderr text)."""
    cfg = hardware.get(name).config()
    # Python 3.6 (the TX2) refuses input= together with any stdin= argument, even stdin=None
    io_args = {"input": data} if data is not None else {"stdin": subprocess.DEVNULL}
    try:
        r = subprocess.run(platform_ssh(cfg, command), timeout=timeout,
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE, **io_args)
        err = [l for l in decode(r.stderr).splitlines() if l.strip() and not l.startswith("Warning: Permanently added")]
        return r.returncode, r.stdout, "\n".join(err)
    except subprocess.TimeoutExpired:
        return -1, b"", "no answer within %d s" % timeout
    except OSError as e:
        return -1, b"", str(e)


class StepFailed(object):
    """What in_parallel returns for a platform whose step raised an exception (it is falsy)."""

    def __init__(self, error):
        self.error = error

    def __bool__(self):
        return False

    def __repr__(self):
        return "StepFailed(%r)" % self.error


def in_parallel(names, fn):
    """fn(name) for each platform at once; returns {name: result} for every name. A step that
    raises gives a (falsy) StepFailed instead of vanishing, so it can't count as a success."""
    results, threads = {}, []

    def run(n):
        try:
            results[n] = fn(n)
        except Exception as e:  # report it rather than lose it in a background thread
            results[n] = StepFailed("%s: %s" % (type(e).__name__, e))
            hw_log("%s: internal error: %s" % (shown(n), results[n].error))

    for n in names:
        t = threading.Thread(target=run, args=(n,), daemon=True)
        t.start()
        threads.append(t)
    for t in threads:
        t.join()
    return results
