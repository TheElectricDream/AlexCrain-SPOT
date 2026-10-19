"""The platforms' hardware, from the lab console.

Teensy command packets, the SSH link to each platform's agent
(~/.spot/agent.sh), pucks, thrusters and thruster checks, handing a
platform to Interactive Mode (interactive.py) or a calibration
(calibration.py), and running agent commands on several platforms at
once.

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

# Each platform's Teensy (teensy_pwm_control.ino) takes 37-byte packets:
# 0xAA, then the 8 thruster duty cycles (0-100 %) and the pucks (on if
# >= 0.5) as little-endian float32. It switches everything off when no
# packet has arrived for 2 s, so whatever is on is re-sent every
# HW_SEND_PERIOD; if this app or the network stops, the platform shuts
# down by itself.
HW_SEND_PERIOD = 0.2

# After everything is off, keep sending "off" this long [s] before
# letting go of the port.
HW_IDLE_HOLD = 1.0

# Seconds between status checks: idle / while driving.
HW_PROBE_PERIOD = {False: 4.0, True: 2.0}

# See +spot/+console/deploy/setup_hardware_link.sh.
HW_KEY = Path.home() / ".ssh" / "id_ed25519_spot_console"

# How this app's own link shows up in a platform's "status".
HW_LINK_NAME = "spot-teensy-link"

HW_SETUP_HINT = ("run +spot/+console/deploy/setup_hardware_link.sh from a lab computer "
                 "(see STATUS.md in the project folder)")

PACKET_HEADER = 0xAA
PUBLISH_PERIOD = 0.2       # [s] drive() tells the pages at most this often
PROBE_TIMEOUT = 10         # [s] for a status check
PROBE_SOON = 0.5           # [s] re-check this soon to confirm another user
LINK_CLOSE_TIMEOUT = 3     # [s] for ssh to exit after its input closes
MIN_WAIT = 0.005           # [s] shortest driver wait
SSH_NOISE = "Warning: Permanently added"   # ssh's first-connection message


def teensy_packet(duty, pucks):
    """Return a Teensy packet: 0xAA, 8 duty cycles [%], pucks."""
    return struct.pack("<B9f", PACKET_HEADER,
                       *([float(d) for d in duty] + [1.0 if pucks else 0.0]))


def platform_ssh(cfg, command):
    """Return the ssh command that runs an agent command on a platform.

    A dead link is noticed within ~3 s (the Teensy has already
    switched off after 2 s).

    Args:
        cfg: The platform's {"host", "user"}.
        command: The agent command.
    """
    return ["ssh", "-i", str(HW_KEY), "-o", "IdentitiesOnly=yes", "-o", "BatchMode=yes",
            "-o", "ConnectTimeout=4", "-o", "StrictHostKeyChecking=accept-new",
            "-o", "ServerAliveInterval=1", "-o", "ServerAliveCountMax=3",
            "%s@%s" % (cfg["user"], cfg["host"]), command]


def describe_ssh_failure(cfg, rc, out):
    """Return (state, message) for a failed status check.

    state is "offline", "denied" or "error".
    """
    where = "%s@%s" % (cfg["user"], cfg["host"])
    offline = r"timed out|No route|unreachable|Connection refused|Could not resolve"
    if rc == -1 or re.search(offline, out):
        return "offline", "%s is not answering (switched off, or not on the network?)." % where
    if "Permission denied" in out:
        return "denied", "%s doesn't accept this console's key; %s." % (where, HW_SETUP_HINT)
    if "Host key verification failed" in out or "REMOTE HOST IDENTIFICATION HAS CHANGED" in out:
        return "error", ("%s's host key changed (reinstalled?). On the TX2 run: ssh-keygen -R %s"
                         % (where, cfg["host"]))

    # Otherwise: ssh's last meaningful line.
    lines = [line for line in out.splitlines()
             if line.strip() and not line.startswith(SSH_NOISE)]
    return "error", "%s: %s" % (where, lines[-1] if lines else
                                "status check failed (code %s)" % rc)


def hw_log(line):
    """Add a time-stamped line to the hardware log."""
    hub.log("hardware", "%s  %s" % (time.strftime("%H:%M:%S"), line))


# One platform


class Platform:
    """One platform's Teensy.

    What should be on, the SSH link that sends it, and a status check.
    """

    def __init__(self, name):
        """Start with everything off and the status unknown."""
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
        # Interactive Mode's session, or a calibration, while it drives
        # this platform (claim/drive/release); its .label says which.
        self.owner = None
        # The owner flies it (claim, not reserve): the link stays open
        # while all is off, so the pucks come back on at once (a
        # calibration rests it between drifts).
        self.claimed = False
        self._published = 0.0   # when drive() last told the pages (many times a second)
        self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [],
                      "time": None}

    def config(self):
        """Return the platform's {"host", "user"} from the settings."""
        return settings.get("platforms").get(self.name) or DEFAULT_PLATFORMS[self.name]

    def start(self):
        """Start the driver and the status check."""
        threading.Thread(target=self._drive_loop, daemon=True).start()
        threading.Thread(target=self._probe_loop, daemon=True).start()

    def status(self):
        """Return all the Hardware tab shows about the platform."""
        cfg = self.config()
        with self._lock:
            check = None
            if self.check:
                check = dict(self.check, total=NUM_THRUSTERS,
                             elapsed=time.time() - self.check["started"])
            return {"name": self.name, "host": cfg["host"], "user": cfg["user"],
                    "probe": dict(self.probe), "duty": list(self.duty), "pucks": self.pucks,
                    "check": check, "last_check": self.last_check,
                    "last_action": self.last_action, "driving": self.link is not None,
                    "link_error": self.link_error, "interactive": self.owner is not None,
                    "owner": self._owner_label()}

    def _owner_label(self):
        """Return what drives the platform, or None."""
        if self.owner is not None:
            return getattr(self.owner, "label", "Interactive Mode")
        return None

    def _publish(self):
        """Send the status to the pages."""
        hub.publish("hardware_platform", self.status())

    def _note(self, text, requester=None):
        """Record and log a change (lock held).

        Args:
            text: What changed.
            requester: Who asked; None for the console itself.
        """
        self.last_action = {"text": text, "by": requester, "time": time.time()}
        hw_log("%s: %s%s" % (shown(self.name), text, " (by %s)" % requester if requester else ""))

    # Commands (from the web page or a script)

    def _require_manual(self):
        """Refuse while an owner drives the platform (lock held).

        Switching everything off (all_off, Esc on the Hardware tab)
        always works, and ends it.

        Raises:
            ApiError: if an owner drives the platform.
        """
        if self.owner is not None:
            where = ("the Calibration tab" if self._owner_label() == "Calibration"
                     else "the PhaseSpace tab")
            raise ApiError("%s is driving the %s: stop it on %s first "
                           "(or press Esc on the Hardware tab to switch everything off)."
                           % (self._owner_label(), shown(self.name), where))

    def _require_ready(self):
        """Refuse unless something may be switched on (lock held).

        Raises:
            ApiError: if a check is running, the platform isn't
                reachable, has no Teensy, or another program uses it.
        """
        probe = self.probe
        if self.check:
            raise ApiError("A thruster check is running on the %s; cancel it first."
                           % shown(self.name))
        if probe["state"] == "checking":
            raise ApiError("Still checking the %s; try again in a moment." % shown(self.name))
        if probe["state"] != "ok":
            raise ApiError(probe["detail"])
        if not probe["teensy"]:
            raise ApiError("The %s has no Teensy at /dev/ttyACM0 (unplugged?)." % shown(self.name))
        if probe["others"]:
            raise ApiError("The %s's Teensy is in use by %s (an experiment?); stop that first."
                           % (shown(self.name), ", ".join(probe["others"])))

    def set_pucks(self, on, requester):
        """Switch the pucks on or off.

        Raises:
            ApiError: if switching on isn't allowed now, or (off) a
                check is running.
        """
        with self._lock:
            self._require_manual()
            if on:
                self._require_ready()
                self.link_error = None
            elif self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first."
                               % shown(self.name))
            if self.pucks != on:
                self.pucks = on
                self._note("pucks %s" % ("on" if on else "off"), requester)
        self._wake.set()
        self._publish()

    def set_thruster(self, index, duty, requester):
        """Set one thruster's duty cycle [%] (0: off).

        Raises:
            ApiError: as set_pucks.
        """
        with self._lock:
            self._require_manual()
            if duty > 0:
                self._require_ready()
                self.link_error = None
            elif self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first."
                               % shown(self.name))
            if self.duty[index] != duty:
                self.duty[index] = duty
                self._note("thruster %d %s" % (index + 1, "on (%g %%)" % duty if duty else "off"),
                           requester)
        self._wake.set()
        self._publish()

    def _all_off_locked(self, requester, note=None):
        """Switch all off (lock held); return whether anything was on.

        Also ends Interactive Mode or a calibration here (it sees it
        has lost the platform), and a running check.
        """
        was_on = bool(self.pucks or any(self.duty) or self.check)
        self.owner = None
        self.claimed = False
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
        """Switch everything off (always allowed)."""
        with self._lock:
            self._all_off_locked(requester, note)
        self._wake.set()
        self._publish()

    def start_check(self, duration, requester):
        """Fire each thruster in turn for duration seconds.

        The platform sits on the table (pucks off) while each fires.

        Raises:
            ApiError: if an owner drives it or it isn't ready.
        """
        with self._lock:
            self._require_manual()
            self._require_ready()
            self.link_error = None
            self.duty = [0.0] * NUM_THRUSTERS
            self.pucks = False
            self.check = {"duration": duration, "started": time.time(), "index": 0,
                          "by": requester}
            self.last_check = None
            self._note("thruster check started, %g s per thruster" % duration, requester)
        self._wake.set()
        self._publish()

    def cancel_check(self, requester):
        """Cancel a running thruster check (everything off)."""
        with self._lock:
            if self.check:
                self._all_off_locked(requester, "thruster check cancelled")
        self._wake.set()
        self._publish()

    # Interactive Mode (interactive.py) or a calibration
    # (calibration.py): one owner drives the thrusters and pucks at its
    # own rate.

    def claim(self, owner, requester):
        """Hand this platform to an owner: thrusters off, pucks on.

        Args:
            owner: Interactive Mode's session, or a calibration.
            requester: Who asked.

        Raises:
            ApiError: if it is driven already or isn't ready.
        """
        with self._lock:
            self._require_manual()
            self._require_ready()
            self.owner = owner
            self.claimed = True
            self.link_error = None
            self.duty = [0.0] * NUM_THRUSTERS
            self.pucks = True
            self._note("%s: pucks on, flying" % self._owner_label(), requester)
        self._wake.set()
        self._publish()

    def reserve(self, owner, requester):
        """Hand this platform to an owner with everything off, kept off.

        For the arm calibration: the platform must rest while its arm
        moves. Nothing else can switch anything on meanwhile, and
        switching it off (Esc, the Hardware tab) takes it back, as for
        claim.

        Raises:
            ApiError: if it is driven, a check runs, it doesn't answer,
                or something is on.
        """
        with self._lock:
            self._require_manual()
            if self.check:
                raise ApiError("A thruster check is running on the %s; cancel it first."
                               % shown(self.name))
            if self.probe["state"] != "ok":
                raise ApiError(self.probe["detail"] or "The %s doesn't answer." % shown(self.name))
            if self.pucks or any(self.duty):
                raise ApiError("The %s is floating or firing; switch it off first (Hardware tab)."
                               % shown(self.name))
            self.owner = owner
            self._note("%s: kept resting (nothing on)" % self._owner_label(), requester)
        self._publish()

    def owned_by(self, owner):
        """Return whether owner still has the platform."""
        with self._lock:
            return self.owner is owner

    def drive(self, owner, duty, pucks=True):
        """Set the 8 duty cycles [%] and the pucks, sent at once.

        Returns:
            False, doing nothing, once owner no longer has the platform
            (someone switched it off, the link was lost, another
            program opened the Teensy); else True.
        """
        with self._lock:
            if self.owner is not owner:
                return False
            self.duty = [min(100.0, max(0.0, float(d))) for d in duty]
            self.pucks = bool(pucks)

            # Tell the pages at most every PUBLISH_PERIOD.
            now = time.time()
            publish = now - self._published >= PUBLISH_PERIOD
            if publish:
                self._published = now
        self._wake.set()
        if publish:
            self._publish()
        return True

    def release(self, owner, note):
        """The owner is done: all off, if it still has the platform."""
        with self._lock:
            if self.owner is not owner:
                return
            self._all_off_locked(None, note)
        self._wake.set()
        self._publish()

    def reconfigure(self):
        """The address changed: drop the link and check the new one."""
        with self._lock:
            self._all_off_locked(None, "all off (address changed)")
            self._release = True
            self.probe = {"state": "checking", "detail": None, "teensy": None, "others": [],
                          "time": None}
        self._wake.set()
        self._probe_wake.set()
        self._publish()

    def shutdown(self):
        """Everything off, a last off packet, and the link closed."""
        with self._lock:
            self.duty = [0.0] * NUM_THRUSTERS
            self.pucks = False
            self.check = None
            self.owner = None
            self.claimed = False
            driving = self.link is not None
        if driving:
            self._send(teensy_packet([0.0] * NUM_THRUSTERS, False))
            self._close_link()

    def make_way(self, requester):
        """Before an experiment here: all off, let go of the Teensy."""
        with self._lock:
            self._all_off_locked(None, "all off: an experiment is starting (by %s)" % requester)
            self._release = True
        self._wake.set()
        self._publish()

    def send_off(self):
        """After an experiment here: a burst of all-off packets.

        So nothing stays on until the Teensy's 2 s watchdog (the old
        GUI's CleanShutdown). Skipped if the console is driving it.
        (The last status check may still list the program that just
        stopped.)
        """
        with self._lock:
            if self.pucks or any(self.duty) or self.check:
                return
        zeros = teensy_packet([0.0] * NUM_THRUSTERS, False)
        for _ in range(3):
            self._send(zeros)
            time.sleep(0.2)
        self._close_link()
        self._probe_wake.set()   # show the Teensy as free again soon

    # The driver: sends the packet every HW_SEND_PERIOD while anything
    # is on.

    def _drive_loop(self):
        """Send the current packet whenever something is on, forever."""
        last_on = 0.0
        while True:
            now = time.time()
            wait, changed = HW_SEND_PERIOD, False
            with self._lock:
                # A thruster check: fire the thruster whose turn it is,
                # until all have fired.
                check = self.check
                if check:
                    index = int((now - check["started"]) / check["duration"])
                    if index >= NUM_THRUSTERS:
                        self.check, self.duty = None, [0.0] * NUM_THRUSTERS
                        self.last_check = {"ok": True, "finished": now,
                                           "note": "all %d thrusters fired for %g s each"
                                                   % (NUM_THRUSTERS, check["duration"])}
                        hw_log("%s: thruster check finished" % shown(self.name))
                        changed = True
                    else:
                        changed = index != check["index"]
                        check["index"] = index
                        self.duty = [100.0 if i == index else 0.0 for i in range(NUM_THRUSTERS)]
                        wait = min(wait, check["started"] + (index + 1) * check["duration"] - now)
                duty, pucks, release, claimed = (list(self.duty), self.pucks, self._release,
                                                 self.claimed)
                self._release = False

            # Send while on, and for HW_IDLE_HOLD after; then (or on a
            # release) close the link.
            on = pucks or any(d > 0 for d in duty) or claimed
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

            # Until the next send, or a change.
            self._wake.wait(max(MIN_WAIT, wait))
            self._wake.clear()

    def _send(self, packet):
        """Send a packet over the link, opening it if needed."""
        with self._lock:
            proc = self.link

        # Open the link: ssh running the agent's "stream".
        if proc is None:
            try:
                proc = subprocess.Popen(platform_ssh(self.config(), "stream"),
                                        stdin=subprocess.PIPE, stdout=subprocess.DEVNULL,
                                        stderr=subprocess.PIPE, bufsize=0)
            except OSError as e:
                self._link_lost("could not run ssh (%s)" % e)
                return
            with self._lock:
                self.link = proc
            threading.Thread(target=self._watch_link, args=(proc,), daemon=True).start()
            self._probe_wake.set()  # check more often while driving
            self._publish()

        # Buffered by the pipe while ssh is still connecting;
        # _watch_link reports why a link closed.
        try:
            proc.stdin.write(packet)
        except (OSError, ValueError):
            pass

    def _close_link(self):
        """Close the link (the Teensy end sees end of input, exits)."""
        with self._lock:
            proc, self.link = self.link, None
        if proc is None:
            return
        try:
            proc.stdin.close()
        except OSError:
            pass
        try:
            proc.wait(timeout=LINK_CLOSE_TIMEOUT)
        except subprocess.TimeoutExpired:
            proc.kill()
        self._publish()

    def _watch_link(self, proc):
        """Wait for the link to end; report it unless intended."""
        err = decode(proc.stderr.read())
        rc = proc.wait()

        # _close_link clears self.link first.
        with self._lock:
            expected = self.link is not proc
            if not expected:
                self.link = None
        if expected:
            return
        lines = [line for line in err.splitlines()
                 if line.strip() and not line.startswith(SSH_NOISE)]
        self._link_lost(lines[-1] if lines else "link closed (code %s)" % rc)

    def _link_lost(self, reason):
        """The link was lost: everything off, without an off hold.

        Whatever was on stops anyway (Teensy watchdog); clearing it
        here means nothing comes back on by itself when the link
        returns. No "off" hold over a new link: the Teensy is already
        off.
        """
        note = "link to the Teensy lost (%s); all off" % reason
        with self._lock:
            self.link_error = reason
            self._release = True
            if not self._all_off_locked(None, note):
                hw_log("%s: %s" % (shown(self.name), note))
        self._wake.set()
        self._publish()
        self._probe_wake.set()

    # The status check: reachable? Teensy plugged in? Anyone else using
    # it?

    def _probe_loop(self):
        """Check the platform every HW_PROBE_PERIOD, forever."""
        while True:
            soon = self._probe()
            with self._lock:
                driving = self.link is not None
            self._probe_wake.wait(PROBE_SOON if soon else HW_PROBE_PERIOD[driving])
            self._probe_wake.clear()

    def _probe(self):
        """Check the platform; return whether to check again soon."""
        cfg = self.config()
        result = {"state": "ok", "detail": None, "teensy": None, "others": [],
                  "time": time.time()}

        # The agent's "status": "teensy yes|no" and "user <pid> <name>"
        # lines for other programs using it.
        if not HW_KEY.exists():
            result.update(state="nokey", detail="This console has no SSH key for the platforms "
                                                "yet; %s." % HW_SETUP_HINT)
        else:
            rc, out = run_quiet(platform_ssh(cfg, "status"), timeout=PROBE_TIMEOUT)
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

            # Another program opened the Teensy while this console uses
            # it: seen twice in a row (e.g. an experiment started), let
            # it have the Teensy instead of fighting over it; seen once,
            # confirm quickly first.
            if result["others"] and active:
                if old["others"]:
                    self._all_off_locked(None, "%s opened the Teensy, so this console let go "
                                               "of it; all off" % ", ".join(result["others"]))
                    self._release = True
                    changed = True
                else:
                    soon = True

        # Log reachability changes and tell the pages.
        if changed:
            if old["state"] != "checking" and old["state"] != result["state"]:
                hw_log("%s: %s" % (shown(self.name),
                                   "reachable" if result["state"] == "ok" else result["detail"]))
            self._wake.set()
            self._publish()
        return soon


# All platforms


class Hardware:
    """The three platforms."""

    def __init__(self):
        """One Platform per default platform, in order."""
        self.platforms = collections.OrderedDict((n, Platform(n)) for n in DEFAULT_PLATFORMS)

    def start(self):
        """Start every platform's driver and status check."""
        for plat in self.platforms.values():
            plat.start()

    def get(self, name):
        """Return a platform by key (any case).

        Raises:
            ApiError: if it isn't RED, BLACK or BLUE.
        """
        plat = self.platforms.get(str(name or "").strip().upper())
        if plat is None:
            raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
        return plat

    def status(self):
        """Return every platform's status and the SSH key's."""
        return {"platforms": [plat.status() for plat in self.platforms.values()],
                "key": str(HW_KEY), "key_exists": HW_KEY.exists()}

    def all_off(self, requester):
        """Switch everything off on every platform."""
        for plat in self.platforms.values():
            plat.all_off(requester)

    def shutdown(self):
        """Shut every platform down (the console is stopping)."""
        for plat in self.platforms.values():
            plat.shutdown()


hardware = Hardware()


def parse_platforms(value):
    """Validate a platforms settings change, merged into the current.

    Args:
        value: {"RED": {"host": ..., "user": ...}, ...}.

    Raises:
        ApiError: for an unknown platform, a bad host or user name.
    """
    if not isinstance(value, dict):
        raise ApiError("platforms must be an object like "
                       "{\"RED\": {\"host\": ..., \"user\": ...}}.")
    out = {n: dict(c) for n, c in settings.get("platforms").items()}
    for name, cfg in value.items():
        name = str(name).upper()
        if name not in DEFAULT_PLATFORMS or not isinstance(cfg, dict):
            raise ApiError("Unknown platform %r." % name)
        host = str(cfg.get("host", "")).strip()
        user = str(cfg.get("user", "")).strip()
        if not HOST_RE.match(host):
            raise ApiError("%s: %r is not an IP address or host name." % (shown(name), host))
        if not USER_RE.match(user):
            raise ApiError("%s: %r is not a user name." % (shown(name), user))
        out[name] = {"host": host, "user": user}
    return out


# Agent commands
#
# The local console (SPOT in MATLAB) builds a diagram with this
# computer as the "board": MATLAB copies the generated code here and
# compiles it under EXP_BUILD_ROOT (experiment.py). This console then
# copies only the program (and any shared libraries next to it) to each
# platform that takes part, starts and stops it there, and collects
# the data files it writes. Everything on the platforms goes through
# spot_agent.sh, the only thing this computer's key may run there.


def platform_run(name, command, data=None, timeout=60):
    """Run one spot_agent.sh command on a platform.

    Args:
        name: The platform.
        command: The agent command.
        data: Bytes for its standard input, or None.
        timeout: Seconds before giving up.

    Returns:
        (exit code, stdout bytes, stderr text); exit code -1 if it
        didn't run or answer.
    """
    cfg = hardware.get(name).config()

    # Python 3.6 (the TX2) refuses input= together with any stdin=
    # argument, even stdin=None.
    io_args = {"input": data} if data is not None else {"stdin": subprocess.DEVNULL}
    try:
        result = subprocess.run(platform_ssh(cfg, command), timeout=timeout,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, **io_args)
        err = [line for line in decode(result.stderr).splitlines()
               if line.strip() and not line.startswith(SSH_NOISE)]
        return result.returncode, result.stdout, "\n".join(err)
    except subprocess.TimeoutExpired:
        return -1, b"", "no answer within %d s" % timeout
    except OSError as e:
        return -1, b"", str(e)


class StepFailed(object):
    """What in_parallel returns for a step that raised (falsy)."""

    def __init__(self, error):
        """Keep the error message."""
        self.error = error

    def __bool__(self):
        """Return False: a failed step never counts as a success."""
        return False

    def __repr__(self):
        """Return StepFailed('<error>')."""
        return "StepFailed(%r)" % self.error


def in_parallel(names, fn):
    """Run fn(name) for each platform at once.

    Returns:
        {name: result} for every name. A step that raises gives a
        (falsy) StepFailed instead of vanishing, so it can't count as a
        success.
    """
    results, threads = {}, []

    def run(n):
        """Run one step; report an exception rather than lose it."""
        try:
            results[n] = fn(n)
        except Exception as e:
            results[n] = StepFailed("%s: %s" % (type(e).__name__, e))
            hw_log("%s: internal error: %s" % (shown(n), results[n].error))

    for n in names:
        t = threading.Thread(target=run, args=(n,), daemon=True)
        t.start()
        threads.append(t)
    for t in threads:
        t.join()
    return results
