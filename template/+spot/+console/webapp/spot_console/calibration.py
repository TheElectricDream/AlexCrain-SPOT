"""The Calibration tab: measuring the lab so that simulations behave as the experiments do, and the
calibration file that holds the results.

The file (one per console): the lab console keeps the lab's in its home (~/.spot_calibration.json),
next to the raw data of every run (~/SPOTCalibration/<stamp>_<kind>/run.json); a project keeps a copy in
resources/spot_calibration.json, which spot.params.apply reads (spot.params.loadCalibration) for the
simulation. Whenever the local console sees the lab console and the lab's file is newer, its page
offers to update the project's copy (which can be declined, or never asked again: the copy's "sync").

    {"format": "spot-calibration", "version": 1,
     "updated": "2026-10-13T15:02:11Z",                  (the newest section's; UTC, so it sorts as text)
     "table": {"updated", "by", "model": "quadratic" | "plane", "height": [a1 .. a5] (calfit.py),
               "centre": [x, y], "damping": {"RED": c, ...}, "tilt_centre_mrad", ..., "drifts": [...]},
     "thrusters": {"RED": {"updated", "by", "thrust": [8 x N], ...}, ...},
     "arm": {... spot-arm-calibration ...},
     "sync": {"ask": true, "declined": null}}           (a project's copy only)

The table slope calibration (TableRun): with PhaseSpace streaming and the platform resting on the table,
  1. record it sitting still (the cameras' noise);
  2. float it (pucks on, thrusters off) and record it drifting from where it is;
  3. fly it (the diagram's own controller, as Interactive Mode) to each point of a grid over the table,
     let it settle, switch the thrusters off and record it drifting again;
  4. pucks off, and fit the table's height surface and the air bearings' drag to the drifts (calfit.py).
A drift ends after its time, or at once (pucks off: the platform stops on the table) when the platform
nears an edge or another platform, or goes too fast.

The thruster calibration (ThrusterRun), with PhaseSpace streaming and the platform resting on the table:
for each thruster 1-8 (and each repeat),
  1. float it (pucks on) where it rests;
  2. fire the thruster at 100 % for the burn time, recording every PhaseSpace frame;
  3. fly it back (the diagram's own controller) to where it started;
  4. pucks off, and let it rest.
Each burn's thrust comes from the motion it gave the platform (calfit.fit_burn), with the platform's
mass, inertia and centre of gravity taken as known (the person starting it confirms them; the controller's
defaults unless changed) and the table's pull (its calibration) taken out. The lab's calibrated thrust is
then what Interactive Mode, the calibrations' own flights and (through the projects' copies) simulations
and experiments use.

The arm calibration (ArmRun) runs the arm calibration script (calibrate_arm.py, from the template's
resources/arm_calibration; on the TX2 next to the console) on the Chaser, through its agent
(spot_agent.sh: arm-cal-check, -receive, -start, -status, -stop, -data): it checks the Chaser has what the
script needs (python3, the Dynamixel SDK, the U2D2 and access to it, nothing else holding it), keeps the
Chaser resting (nothing can float it meanwhile), sends the script, starts it, follows its progress, and
collects its data and fit. A rehearsal runs the script's simulated servos instead: it needs none of that
hardware, and its result is shown but never becomes the lab's.

A calibration drives one platform at a time, through the hardware driver (hardware.py: claim, drive,
release), as Interactive Mode does: it can't start while an experiment or Interactive Mode runs, and
they can't start while it runs. Everything goes off, and the run stops, when the Calibration tab's Stop
(or Esc) is pressed, the Hardware tab switches the platform off, PhaseSpace loses it, it leaves the
table, the link to it is lost, or the console stops. And its Teensy switches everything off by itself
when packets stop for 2 s.

Part of the SPOT Console (webapp/app.py starts it).
"""

import collections
import copy
import io
import json
import math
import re
import tarfile
import threading
import time
from pathlib import Path

from . import calfit
from .common import PROJECT_DIR, ApiError, decode, shown
from .control import DEFAULTS
from .events import hub
from .experiment import experiment
from .hardware import NUM_THRUSTERS, hardware, hw_log, platform_run
from .interactive import EXPERIMENT_BUSY, TABLE, ControllerProcess, fresh_pose, interactive, wrap
from .phasespace import monitor, streamer


PLATFORMS = ("RED", "BLACK", "BLUE")
SECTIONS = ("table", "thrusters", "arm")
RUNS_DIR = Path.home() / "SPOTCalibration"   # the lab console's raw data of each run
MAX_FILE = 4 * 1024 * 1024                   # [bytes] a calibration file is a few hundred kB at most

# Flying between points (fly_to): the goal moves towards the point at these speeds, so the
# controller isn't kicked by a jump in its goal (as a dragged goal in Interactive Mode moves)
GOAL_SPEED = 0.10            # [m/s]
GOAL_TURN = math.radians(15)  # [rad/s]
# There: within SETTLED_POS of the point and SETTLED_ATT of its attitude, and slower than SETTLED_SPEED,
# for SETTLED_TIME. Only roughly at the point: the diagram's LQR has no integral action, so on a sloping
# table it holds the platform a few centimetres downhill of its goal; what matters is that it is at rest.
SETTLED_POS = 0.10           # [m]
SETTLED_ATT = math.radians(5)
SETTLED_SPEED = 0.01         # [m/s]
SETTLED_TIME = 1.5           # [s]
FLY_TIMEOUT = 90.0           # [s] to reach a point
LOST_TIMEOUT = 1.0           # [s] PhaseSpace may lose the platform this long (thrusters off meanwhile)
CLEARANCE = 0.45             # [m] from other platforms (centre to centre), as Interactive Mode's goals
STOP_PAUSE = 2.0             # [s] after the pucks go off to end a drift: the platform comes to rest
# The data's own check: from one frame to the next a drifting platform moves at most a few mm, so a
# bigger jump is a glitch (PhaseSpace mis-tracking it, or a second stream on the monitor's port); with
# more than GLITCH_FRACTION of them, the run is not saved
GLITCH_STEP = 0.02           # [m]
GLITCH_FRACTION = 0.005
# The fit's: a position residual above POOR_FIT_FACTOR times the cameras' noise (at least POOR_FIT_FLOOR)
# means the model doesn't describe the table well (a plane on a curved table: more grid points give a
# surface), which the result says; above REJECT_RESIDUAL something else moved the platform: not saved
POOR_FIT_FACTOR = 5.0
POOR_FIT_FLOOR = 1.0         # [mm]
REJECT_RESIDUAL = 20.0       # [mm]


def now_iso():
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def stamp():
    return time.strftime("%Y%m%d_%H%M%S")


class Abort(Exception):
    """A calibration run stops (the message says why)."""


# ---------------------------------------------------------------------------- the file

def _finite(v, what):
    if not isinstance(v, (int, float)) or isinstance(v, bool) or not math.isfinite(v):
        raise ApiError("%s must be a finite number." % what)
    return float(v)


def _numbers(v, n, what):
    if not isinstance(v, list) or len(v) != n:
        raise ApiError("%s must be a list of %d numbers." % (what, n))
    return [_finite(x, what) for x in v]


def validate(cal, local=False):
    """A checked copy of the calibration CAL: the values the simulation reads are checked (the rest
    is what the pages show). LOCAL: a project's copy (it keeps "sync")."""
    if not isinstance(cal, dict):
        raise ApiError("The calibration must be an object.")
    if cal.get("format", "spot-calibration") != "spot-calibration":
        raise ApiError("This is not a SPOT calibration file.")
    out = {"format": "spot-calibration", "version": 1}
    table = cal.get("table")
    if table is not None:
        if not isinstance(table, dict):
            raise ApiError("calibration.table must be an object.")
        t = copy.deepcopy(table)
        t["height"] = _numbers(table.get("height"), 5, "table.height")
        t["centre"] = _numbers(table.get("centre", list(calfit.CENTRE)), 2, "table.centre")
        damping = table.get("damping") or {}
        if not isinstance(damping, dict) or any(k not in PLATFORMS for k in damping):
            raise ApiError("table.damping must map RED, BLACK, BLUE to numbers.")
        t["damping"] = dict((k, _finite(v, "table.damping.%s" % k)) for k, v in damping.items())
        if t.get("model") not in ("quadratic", "plane"):
            raise ApiError("table.model must be quadratic or plane.")
        out["table"] = t
    thrusters = cal.get("thrusters")
    if thrusters is not None:
        if not isinstance(thrusters, dict) or any(k not in PLATFORMS for k in thrusters):
            raise ApiError("calibration.thrusters must map RED, BLACK, BLUE to their calibration.")
        out["thrusters"] = {}
        for k, v in thrusters.items():
            if not isinstance(v, dict):
                raise ApiError("thrusters.%s must be an object." % k)
            c = copy.deepcopy(v)
            c["thrust"] = _numbers(v.get("thrust"), NUM_THRUSTERS, "thrusters.%s.thrust" % k)
            if not all(0.01 <= f <= 2.0 for f in c["thrust"]):
                raise ApiError("thrusters.%s.thrust must be 0.01 to 2 N each." % k)
            out["thrusters"][k] = c
    arm = cal.get("arm")
    if arm is not None:
        if not isinstance(arm, dict) or arm.get("format") != "spot-arm-calibration":
            raise ApiError("calibration.arm must be an arm calibration (calibrate_arm.py's).")
        out["arm"] = copy.deepcopy(arm)
    out["updated"] = newest(out)
    if local:
        sync = cal.get("sync") or {}
        out["sync"] = {"ask": bool(sync.get("ask", True)),
                       "declined": sync.get("declined") if isinstance(sync.get("declined"), str) else None}
    return out


def newest(cal):
    """The newest section's time (None for an empty calibration)."""
    times = [cal["table"].get("updated")] if cal.get("table") else []
    times += [v.get("updated") for v in (cal.get("thrusters") or {}).values()]
    if cal.get("arm"):
        times.append(cal["arm"].get("updated"))
    times = [t for t in times if isinstance(t, str)]
    return max(times) if times else None


def sections_newer(lab, mine):
    """Which of the lab's sections are newer than (or missing from) this project's: ["table",
    "thrusters RED", "arm", ...]."""
    out = []
    lt, mt = lab.get("table"), mine.get("table")
    if lt and (not mt or lt.get("updated", "") > mt.get("updated", "")):
        out.append("table")
    for name, c in sorted((lab.get("thrusters") or {}).items()):
        m = (mine.get("thrusters") or {}).get(name)
        if not m or c.get("updated", "") > m.get("updated", ""):
            out.append("thrusters " + name)
    la, ma = lab.get("arm"), mine.get("arm")
    if la and (not ma or la.get("updated", "") > ma.get("updated", "")):
        out.append("arm")
    return out


class CalibrationStore:
    """This console's calibration file (as design.py's DesignStore keeps the design)."""

    def __init__(self):
        self._lock = threading.Lock()
        self.path = None
        self.local = False
        self.cal = validate({})
        self.saved = False

    def open(self, path, local=False):
        self.path, self.local = Path(path), local
        self.cal = validate({}, local)
        try:
            text = self.path.read_text(encoding="utf-8")
            self.cal = validate(json.loads(text), local)
            self.saved = True
        except FileNotFoundError:
            pass
        except (OSError, ValueError, ApiError) as e:
            hub.log("hardware", "The calibration file %s could not be read (%s); no calibration is used." % (self.path, e))

    def get(self):
        with self._lock:
            return {"calibration": copy.deepcopy(self.cal), "path": str(self.path) if self.path else None,
                    "saved": self.saved}

    def _write(self, cal):
        """Save CAL (checked) and tell the pages (call without the lock)."""
        cal = validate(cal, self.local)
        data = json.dumps(cal, indent=1, allow_nan=False) + "\n"
        if len(data) > MAX_FILE:
            raise ApiError("The calibration would be %d kB; at most %d kB." % (len(data) // 1024, MAX_FILE // 1024))
        with self._lock:
            self.cal = cal
            if self.path:
                self.path.parent.mkdir(parents=True, exist_ok=True)
                tmp = self.path.with_name(self.path.name + ".tmp")
                tmp.write_text(data, encoding="utf-8")
                tmp.replace(self.path)
                self.saved = True
        hub.publish("calibration", self.get())
        return self.get()

    def set_section(self, section, value, platform=None):
        """Replace one section (a platform's, for thrusters) with VALUE (None: remove it)."""
        with self._lock:
            cal = copy.deepcopy(self.cal)
        if section == "thrusters":
            th = cal.get("thrusters") or {}
            if value is None:
                th.pop(platform, None)
            else:
                th[platform] = value
            cal["thrusters"] = th or None
        else:
            cal[section] = value
        cal = dict((k, v) for k, v in cal.items() if v is not None)
        return self._write(cal)

    def take(self, lab_cal):
        """A project's copy becomes the lab's (keeping its own "sync")."""
        with self._lock:
            sync = copy.deepcopy(self.cal.get("sync"))
        cal = dict(validate(lab_cal), sync=sync)
        return self._write(cal)

    def set_sync(self, ask=None, declined=None):
        with self._lock:
            cal = copy.deepcopy(self.cal)
        sync = cal.get("sync") or {"ask": True, "declined": None}
        if ask is not None:
            sync["ask"] = bool(ask)
        if declined is not None:
            sync["declined"] = str(declined)
        cal["sync"] = sync
        return self._write(cal)


store = CalibrationStore()


# ---------------------------------------------------------------------------- a run

class Run:
    """One calibration being run: what the page shows of it."""

    label = "Calibration"   # what the hardware tab says drives the platform

    def __init__(self, kind, platform, params, requester):
        self.kind = kind
        self.platform = platform
        self.params = params
        self.by = requester
        self.started = time.time()
        self.stamp = stamp()
        self.state = "running"            # running, stopping, done, failed, stopped
        self.step = None                  # the step under way (an index into steps)
        self.steps = []                   # [{"name", "state": waiting/running/done/skipped/failed, "note"}]
        self.message = "Starting…"
        self.log = collections.deque(maxlen=200)
        self.stop_reason = None
        self.pose = None                  # [x, y, theta] now
        self.goal = None                  # where it is flying to
        self.path = []                    # the current drift or burn, thinned: [[x, y], ...]
        self.records = []                 # finished drifts or burns (thinned, for the page)
        self.result = None
        self.error = None
        self.finished = None
        self.recording = None             # frames being recorded: [[t, x, y, theta], ...]
        self.controller = None            # ControllerProcess while it flies

    def status(self):
        return {"kind": self.kind, "platform": self.platform, "params": self.params, "by": self.by,
                "started": self.started, "state": self.state, "step": self.step, "steps": self.steps,
                "message": self.message, "log": list(self.log), "pose": self.pose, "goal": self.goal,
                "path": self.path[-400:], "records": self.records, "result": self.result,
                "points": getattr(self, "points", None),
                "check": getattr(self, "check", None), "armlog": getattr(self, "armlog", [])[-40:],
                "error": self.error, "finished": self.finished}


class Calibrator:
    """Runs one calibration at a time on the lab console."""

    def __init__(self):
        self._lock = threading.Lock()
        self.run = None        # the run under way, or the last one
        self._published = 0.0

    def active(self):
        with self._lock:
            return self.run is not None and self.run.state in ("running", "stopping")

    def status(self):
        with self._lock:
            return {"active": self.run is not None and self.run.state in ("running", "stopping"),
                    "run": self.run.status() if self.run else None}

    def publish(self, force=True):
        now = time.monotonic()
        if not force and now - self._published < 0.2:
            return
        self._published = now
        hub.publish("calibration_run", self.status())

    # ---- requests

    def _check_free(self, platform):
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first (PhaseSpace tab): the calibration measures the "
                           "platform's motion with it.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; calibrate once it has stopped.")
        if interactive.status()["active"]:
            raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
        if self.active():
            raise ApiError("A calibration is already running; stop it first.")
        if fresh_pose(platform, time.time()) is None:
            raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)." % shown(platform))

    def start_table(self, body, requester):
        platform = parse_platform(body.get("platform"))
        params = {
            "nx": int(number(body.get("nx", 3), "Grid columns", 1, 5)),
            "ny": int(number(body.get("ny", 2), "Grid rows", 1, 4)),
            "drift": number(body.get("drift", 12), "Drift time", 4, 40),
            "margin": number(body.get("margin", 0.35), "Edge margin", 0.25, 0.8),
            "max_speed": number(body.get("max_speed", 0.15), "Highest speed", 0.03, 0.3),
            "model": str(body.get("model") or "auto"),
        }
        if params["model"] not in ("auto", "quadratic", "plane"):
            raise ApiError("model must be auto, quadratic or plane.")
        self._check_free(platform)
        run = Run("table", platform, params, requester)
        self._begin(run, TableRun(self, run).main)
        return self.status()

    def start_thrusters(self, body, requester):
        platform = parse_platform(body.get("platform"))
        if body.get("acknowledged") is not True:
            raise ApiError("Confirm first that the platform's mass, inertia and centre of gravity are right: the "
                           "thrust is worked out from them.")
        defaults = assumed_defaults()[platform]
        a = body.get("assumed") or {}
        cg = a.get("cg_mm", defaults["cg_mm"])
        if not isinstance(cg, list) or len(cg) != 2:
            raise ApiError("cg_mm must be [x, y] in mm.")
        assumed = {"mass": number(a.get("mass", defaults["mass"]), "Mass", 1, 50),
                   "inertia": number(a.get("inertia", defaults["inertia"]), "Inertia", 0.01, 5),
                   "cg_mm": [number(cg[0], "CG x", -60, 60), number(cg[1], "CG y", -60, 60)]}
        assumed["dist2CG"] = dist_from_cg(assumed["cg_mm"])
        assumed["changed"] = any(abs(assumed[k] - defaults[k]) > 1e-9 for k in ("mass", "inertia")) or \
            any(abs(u - v) > 1e-9 for u, v in zip(assumed["cg_mm"], defaults["cg_mm"]))
        params = {
            "repeats": int(number(body.get("repeats", 2), "Repeats", 1, 3)),
            "burn": number(body.get("burn", 2.0), "Burn time", 0.5, 3.0),
            "margin": number(body.get("margin", 0.35), "Edge margin", 0.25, 0.8),
            "assumed": assumed,
        }
        self._check_free(platform)
        run = Run("thrusters", platform, params, requester)
        self._begin(run, ThrusterRun(self, run).main)
        return self.status()

    def start_arm(self, body, requester):
        joints = body.get("joints", [1, 2, 3])
        tests = body.get("tests", list(ARM_TESTS))
        if not isinstance(joints, list) or not joints or any(j not in (1, 2, 3) for j in joints) or len(set(joints)) != len(joints):
            raise ApiError("joints must be some of 1 (shoulder), 2 (elbow), 3 (wrist).")
        if not isinstance(tests, list) or not tests or any(t not in ARM_TESTS for t in tests) or len(set(tests)) != len(tests):
            raise ApiError("tests must be some of %s." % ", ".join(ARM_TESTS))
        simulate = body.get("simulate") is True
        if not simulate and body.get("acknowledged") is not True:
            raise ApiError("Confirm first that the Chaser rests on the table and its arm is clear to move.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; calibrate once it has stopped.")
        if interactive.status()["active"]:
            raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
        if self.active():
            raise ApiError("A calibration is already running; stop it first.")
        params = {"joints": sorted(joints), "tests": [t for t in ARM_TESTS if t in tests], "simulate": simulate}
        run = Run("arm", "RED", params, requester)
        self._begin(run, ArmRun(self, run).main)
        return self.status()

    def arm_check(self):
        """What the Chaser has for the arm calibration (arm-cal-check), without starting anything."""
        return arm_check()

    def _begin(self, run, main):
        with self._lock:
            if self.run is not None and self.run.state in ("running", "stopping"):
                raise ApiError("A calibration is already running; stop it first.")
            self.run = run
        threading.Thread(target=main, daemon=True).start()
        self.publish()

    def stop(self, requester, reason=None):
        with self._lock:
            run = self.run
            if run is None or run.state not in ("running", "stopping"):
                return self.status_unlocked()
            run.state = "stopping"
            run.stop_reason = reason or "stopped by %s" % requester
        hardware.get(run.platform).release(run, "Calibration stopped (%s); all off" % run.stop_reason)
        self.publish()
        return self.status()

    def status_unlocked(self):
        return {"active": False, "run": self.run.status() if self.run else None}

    def shutdown(self):
        self.stop("the console", "the console is shutting down")


calibrator = Calibrator()


def parse_platform(name):
    name = str(name or "").strip().upper()
    if name not in PLATFORMS:
        raise ApiError("Choose the platform: RED, BLACK or BLUE.")
    return name


def number(value, what, low, high):
    try:
        v = float(value)
    except (TypeError, ValueError):
        raise ApiError("%s must be a number." % what)
    if not (math.isfinite(v) and low <= v <= high):
        raise ApiError("%s must be %g to %g." % (what, low, high))
    return v


def thin(points, n):
    """At most N of POINTS, evenly spread, the last one kept."""
    if len(points) <= n:
        return list(points)
    step = len(points) / float(n)
    out = [points[int(i * step)] for i in range(n)]
    out[-1] = points[-1]
    return out


# ---------------------------------------------------------------------------- driving a platform

class Driver:
    """What every calibration does with its platform: records PhaseSpace, floats it, flies it to a
    point with the diagram's controller, and stops when it must. Every wait goes through tick(),
    which keeps the platform's packets going and raises Abort when the run must stop."""

    def __init__(self, cal, run):
        self.cal = cal
        self.run = run
        self.name = run.platform
        self.hw = hardware.get(run.platform)
        self.period = 1.0 / DEFAULTS["sample_rate"]
        self.duty = [0.0] * NUM_THRUSTERS
        self.pucks = False
        self.claimed = False
        self.lost_since = None
        self.backwards = 0      # frames recorded whose time went backwards (see _on_frame)

    # ---- reporting

    def say(self, message, log=True):
        self.run.message = message
        if log:
            self.run.log.append("%s  %s" % (time.strftime("%H:%M:%S"), message))
            hw_log("%s: calibration: %s" % (shown(self.name), message))
        self.cal.publish(force=False)

    def begin_step(self, index, note=None):
        r = self.run
        if r.step is not None and r.steps[r.step]["state"] == "running":
            r.steps[r.step]["state"] = "done"
        r.step = index
        r.steps[index]["state"] = "running"
        if note:
            r.steps[index]["note"] = note
        self.cal.publish()

    def end_step(self, state="done", note=None):
        r = self.run
        if r.step is not None:
            r.steps[r.step]["state"] = state
            if note:
                r.steps[r.step]["note"] = note
        self.cal.publish()

    # ---- recording every PhaseSpace frame

    def _on_frame(self, frame):
        rec = self.run.recording
        c = frame["crafts"].get(self.name)
        if rec is None or not c or not c["tracked"] or c["x"] is None or c["y"] is None or c["att"] is None:
            return
        t = frame["t"] if frame.get("t") is not None else frame["received"]
        if rec and t <= rec[-1][0]:
            self.backwards += 1   # PhaseSpace's time went back: a second stream on the monitor's port?
            return
        rec.append([t, c["x"], c["y"], c["att"], frame["received"]])

    def start_recording(self):
        self.run.recording = []
        self.run.path = []
        monitor.add_listener(self._on_frame)

    def stop_recording(self):
        monitor.remove_listener(self._on_frame)
        rec, self.run.recording = self.run.recording or [], None
        return rec

    # ---- the platform

    def claim(self):
        self.hw.claim(self.run, self.run.by)   # raises ApiError if it can't be driven now
        self.claimed = True
        self.pucks = True

    def release(self, note):
        if self.claimed:
            self.hw.release(self.run, note)
            self.claimed = False

    def set(self, duty=None, pucks=None):
        if duty is not None:
            self.duty = list(duty)
        if pucks is not None:
            self.pucks = pucks

    def check(self):
        """Raise Abort if the run must stop; returns the platform's pose (None while lost)."""
        r = self.run
        if r.state != "running":
            raise Abort(r.stop_reason or "stopped")
        now = time.time()
        pose = fresh_pose(self.name, now)
        if pose is None:
            self.lost_since = self.lost_since or now
            if now - self.lost_since > LOST_TIMEOUT:
                raise Abort("PhaseSpace lost the %s for more than %g s" % (shown(self.name), LOST_TIMEOUT))
            self.duty = [0.0] * NUM_THRUSTERS
        else:
            self.lost_since = None
            if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
                raise Abort("PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)" % (shown(self.name), pose[0], pose[1]))
            r.pose = [pose[0], pose[1], math.degrees(pose[2])]
            if r.recording is not None:
                if not r.path or math.hypot(pose[0] - r.path[-1][0], pose[1] - r.path[-1][1]) > 0.003:
                    r.path.append([round(pose[0], 4), round(pose[1], 4)])
        if self.claimed and not self.hw.drive(r, self.duty, self.pucks):
            la = self.hw.status()["last_action"]
            raise Abort("the %s was taken back (%s)" % (shown(self.name), la["text"] if la else "switched off"))
        self.cal.publish(force=False)
        return pose

    def wait(self, seconds, until=None):
        """Keep the platform going for SECONDS (or until UNTIL(pose) is true); returns the last pose."""
        end = time.monotonic() + seconds
        pose = None
        while True:
            pose = self.check()
            if until is not None and pose is not None and until(pose):
                return pose
            left = end - time.monotonic()
            if left <= 0:
                return pose
            time.sleep(min(self.period, left))

    def others(self):
        """The other platforms PhaseSpace sees: {NAME: [x, y]}."""
        now, out = time.time(), {}
        for n in PLATFORMS:
            if n != self.name:
                p = fresh_pose(n, now)
                if p is not None:
                    out[n] = p[:2]
        return out

    def path_clear(self, a, b):
        """Whether the straight path from A to B keeps CLEARANCE from the other platforms; else which one."""
        for n, p in self.others().items():
            if segment_distance(p, a, b) < CLEARANCE:
                return n
        return None

    def fly_to(self, goal):
        """Fly the platform (pucks on) to GOAL = [x, y, theta] with the diagram's controller, the goal
        moving there gradually, until it has settled there."""
        r = self.run
        if r.controller is None:
            r.controller = ControllerProcess(self.name, self.controller_params())
            r.controller.wait_ready()
        pose = self.check()
        if pose is None:
            raise Abort("PhaseSpace doesn't see the %s" % shown(self.name))
        target = [goal[0], goal[1], pose[2] + wrap(goal[2] - pose[2])]
        current = [pose[0], pose[1], pose[2]]
        r.goal = [goal[0], goal[1], math.degrees(goal[2])]
        self.pucks = True
        started, settled_since = time.monotonic(), None
        next_step = time.monotonic()
        while True:
            # the goal moves towards the target
            d = math.hypot(target[0] - current[0], target[1] - current[1])
            step = GOAL_SPEED * self.period
            if d > step:
                current[0] += (target[0] - current[0]) * step / d
                current[1] += (target[1] - current[1]) * step / d
            else:
                current[0], current[1] = target[0], target[1]
            dth, turn = target[2] - current[2], GOAL_TURN * self.period
            current[2] = target[2] if abs(dth) <= turn else current[2] + math.copysign(turn, dth)
            pose = fresh_pose(self.name, time.time())
            if pose is not None:
                _u, duty, _b = r.controller.step(pose, list(current))
                self.duty = [100 * v for v in duty]
            pose = self.check()
            if pose is not None and current == target:
                close = (math.hypot(pose[0] - target[0], pose[1] - target[1]) < SETTLED_POS and
                         abs(wrap(pose[2] - target[2])) < SETTLED_ATT and
                         math.hypot(pose[3], pose[4]) < SETTLED_SPEED and abs(pose[5]) < math.radians(2))
                if close:
                    settled_since = settled_since or time.monotonic()
                    if time.monotonic() - settled_since >= SETTLED_TIME:
                        self.duty = [0.0] * NUM_THRUSTERS
                        r.goal = None
                        return pose
                else:
                    settled_since = None
            if time.monotonic() - started > FLY_TIMEOUT:
                why = ""
                if pose is not None:
                    why = " (it is %.0f mm and %.1f deg from it, moving at %.0f mm/s and %.1f deg/s)" % (
                        1000 * math.hypot(pose[0] - target[0], pose[1] - target[1]), math.degrees(abs(wrap(pose[2] - target[2]))),
                        1000 * math.hypot(pose[3], pose[4]), math.degrees(abs(pose[5])))
                raise Abort("the %s didn't settle at (%.2f m, %.2f m) within %g s%s"
                            % (shown(self.name), goal[0], goal[1], FLY_TIMEOUT, why))
            next_step += self.period
            delay = next_step - time.monotonic()
            if delay > 0:
                time.sleep(delay)
            else:
                next_step = time.monotonic()

    def controller_params(self):
        """The controller's parameters: SPOT's defaults, with the lab's calibrated thrust."""
        return controller_params(self.name)

    def finish(self, ok, note):
        r = self.run
        if r.recording is not None:
            self.stop_recording()
        self.release(note)
        if r.controller is not None:
            r.controller.close()
            r.controller = None
        r.goal = None


def segment_distance(p, a, b):
    """The distance from point P to the segment AB."""
    ax, ay, bx, by = a[0], a[1], b[0], b[1]
    dx, dy = bx - ax, by - ay
    L = dx * dx + dy * dy
    s = 0.0 if L == 0 else max(0.0, min(1.0, ((p[0] - ax) * dx + (p[1] - ay) * dy) / L))
    return math.hypot(p[0] - (ax + s * dx), p[1] - (ay + s * dy))


def save_run(run, data):
    """The raw data of a run, kept on the lab console (~/SPOTCalibration/<stamp>_<kind>/run.json)."""
    try:
        folder = RUNS_DIR / ("%s_%s_%s" % (run.stamp, run.kind, run.platform))
        folder.mkdir(parents=True, exist_ok=True)
        (folder / "run.json").write_text(json.dumps(data, allow_nan=False) + "\n", encoding="utf-8")
        return str(folder)
    except (OSError, ValueError) as e:
        hw_log("calibration: could not save the run's data (%s)" % e)
        return None


# ---------------------------------------------------------------------------- the table's slope

def grid_points(nx, ny, margin):
    """The grid of start points: NX x NY, spread over the table inside MARGIN + 0.35 m, in a snake."""
    inset = margin + 0.35
    xs = [TABLE[0] / 2] if nx == 1 else [inset + (TABLE[0] - 2 * inset) * i / (nx - 1) for i in range(nx)]
    ys = [TABLE[1] / 2] if ny == 1 else [inset + (TABLE[1] - 2 * inset) * j / (ny - 1) for j in range(ny)]
    out = []
    for j, y in enumerate(ys):
        row = xs if j % 2 == 0 else list(reversed(xs))
        out.extend([x, y] for x in row)
    return out


class TableRun:
    def __init__(self, cal, run):
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)

    def main(self):
        r, d, p = self.run, self.d, self.run.params
        points = grid_points(p["nx"], p["ny"], p["margin"])
        r.steps = ([{"name": "Still", "state": "waiting", "note": "Noise while resting"},
                    {"name": "Float Here", "state": "waiting", "note": "Drift from where it is"}] +
                   [{"name": "Point %d" % (i + 1), "state": "waiting", "note": "(%.2f, %.2f) m" % tuple(pt)}
                    for i, pt in enumerate(points)] +
                   [{"name": "Fit", "state": "waiting", "note": ""}])
        r.points = points
        drifts, still = [], None
        ok, note = False, None
        try:
            # 1. resting: the cameras' noise
            d.begin_step(0)
            d.say("Recording the %s resting on the table (2 s)." % shown(r.platform))
            d.start_recording()
            d.wait(2.0)
            still = d.stop_recording()
            noise = noise_mm(still)
            d.end_step(note="%.2f mm" % noise if noise is not None else "no data")
            # 2. float it, and let it drift from where it is
            d.begin_step(1)
            d.claim()
            pose = d.check()
            if pose is not None and self.inside(pose, extra=0.1):
                drifts.append(self.drift(1))
            else:
                d.end_step("skipped", "too close to an edge or a platform")
            # 3. each point of the grid
            for i, pt in enumerate(points):
                d.begin_step(2 + i)
                pose = d.check()
                if pose is None:
                    raise Abort("PhaseSpace doesn't see the %s" % shown(r.platform))
                blocker = d.path_clear(pose[:2], pt)
                if blocker:
                    d.say("Point %d skipped: the path passes the %s." % (i + 1, shown(blocker)))
                    d.end_step("skipped", "path passes the %s" % shown(blocker))
                    continue
                d.say("Flying to point %d (%.2f m, %.2f m)." % (i + 1, pt[0], pt[1]))
                d.fly_to([pt[0], pt[1], pose[2]])
                drifts.append(self.drift(2 + i))
            d.finish(True, "Calibration finished; all off")
            # 4. fit
            d.begin_step(len(r.steps) - 1)
            d.say("Fitting the table's slope to %d drifts." % len(drifts))
            records = [x for x in drifts if x and len(x["t"]) >= 20]
            if not records:
                raise Abort("no drift was long enough to fit")
            samples = sum(len(x["t"]) for x in records)
            jumps = sum(x["glitches"] for x in records)
            if jumps + d.backwards > GLITCH_FRACTION * samples:
                save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still, "drifts": drifts,
                             "rejected": "glitches"})
                raise Abort("PhaseSpace's data isn't consistent: %d of %d positions jumped, and %d frames went back in time "
                            "(mis-tracking, or a second stream on the console's port?); the lab's calibration was not "
                            "changed" % (jumps, samples, d.backwards))
            model = None if p["model"] == "auto" else p["model"]
            fit = calfit.fit_table(records, model)
            noise = noise_mm(still)
            if fit["rms_mm"] > REJECT_RESIDUAL:
                save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still,
                             "drifts": drifts, "rejected_fit": fit})
                raise Abort("the drifts don't follow the model at all (position residual %.1f mm): something else moved "
                            "the platform; the lab's calibration was not changed" % fit["rms_mm"])
            if fit["rms_mm"] > max(POOR_FIT_FLOOR, POOR_FIT_FACTOR * (noise or 0.2)):
                fit["warning"] = ("The drifts follow the %s only to %.1f mm (the cameras' noise is %.2f mm): the table "
                                  "isn't that simple here.%s" % (fit["model"], fit["rms_mm"], noise or 0.0,
                                  " A larger grid (3 x 2 or more) fits a surface." if fit["model"] == "plane" else ""))
            result = dict(fit, updated=now_iso(), by=r.by, platform=r.platform, noise_mm=noise_mm(still),
                          run=r.stamp, params=p,
                          paths=[{"platform": x["platform"], "cut": x["cut"],
                                  "points": [[round(a, 4), round(b, 4)] for a, b in thin(list(zip(x["x"], x["y"])), 150)]}
                                 for x in records])
            folder = save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still,
                                  "drifts": drifts, "fit": result})
            result["folder"] = folder
            store.set_section("table", result)
            r.result = result
            d.end_step(note="%s, %.2f mrad at the centre" % (fit["model"], fit["tilt_centre_mrad"]))
            d.say("Done: the table tilts %.2f mrad at its centre (%.2f mrad at most), downhill towards %.0f deg; "
                  "position residual %.2f mm.%s" % (fit["tilt_centre_mrad"], fit["tilt_max_mrad"], fit["downhill_deg"],
                                                    fit["rms_mm"], " " + fit["warning"] if fit.get("warning") else ""))
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:   # never leave the platform floating after a bug
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            d.finish(ok, "Calibration %s; all off" % ("finished" if ok else "stopped (%s)" % note))
        self.done(ok, note, drifts, still)

    def inside(self, pose, extra=0.0):
        """Whether the platform is clear of the edges (the margin) and the other platforms."""
        m = self.run.params["margin"] + extra
        if not (m <= pose[0] <= TABLE[0] - m and m <= pose[1] <= TABLE[1] - m):
            return False
        return all(math.hypot(pose[0] - o[0], pose[1] - o[1]) >= CLEARANCE for o in self.d.others().values())

    def drift(self, step):
        """Thrusters off, pucks on: record the platform drifting until the drift time is up, or it nears an
        edge or another platform or goes too fast (then the pucks go off at once, and it stops)."""
        r, d, p = self.run, self.d, self.run.params
        d.set(duty=[0.0] * NUM_THRUSTERS, pucks=True)
        d.say("Drifting (up to %g s)." % p["drift"])
        d.start_recording()
        why = []

        def stop(pose):
            if not self.inside(pose):
                why.append("near an edge or a platform")
                return True
            if math.hypot(pose[3], pose[4]) > p["max_speed"]:
                why.append("faster than %g m/s" % p["max_speed"])
                return True
            return False
        d.wait(p["drift"], until=stop)
        rec = d.stop_recording()
        cut = why[0] if why else None
        if cut:
            d.set(pucks=False)
            d.say("Drift cut short (%s): pucks off." % cut)
            d.wait(STOP_PAUSE)
            d.set(pucks=True)
        moved = math.hypot(rec[-1][1] - rec[0][1], rec[-1][2] - rec[0][2]) if len(rec) > 1 else 0.0
        secs = rec[-1][0] - rec[0][0] if len(rec) > 1 else 0.0
        note = "%.1f s, %.0f mm%s" % (secs, 1000 * moved, " (cut: %s)" % cut if cut else "")
        d.end_step(note=note)
        glitches = sum(1 for a, b in zip(rec, rec[1:]) if math.hypot(b[1] - a[1], b[2] - a[2]) > GLITCH_STEP)
        record = {"platform": r.platform, "t": [s[0] for s in rec], "x": [s[1] for s in rec], "y": [s[2] for s in rec],
                  "theta": [s[3] for s in rec], "cut": cut, "glitches": glitches}
        r.records.append({"points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 150)], "cut": cut})
        return record

    def done(self, ok, note, drifts, still):
        r = self.run
        if not ok:
            if r.step is not None and r.steps[r.step]["state"] == "running":
                r.steps[r.step]["state"] = "failed"
            r.error = note if r.state == "running" else None
            stopped = r.state == "stopping"
            r.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
            if drifts:
                save_run(r, {"kind": "table", "platform": r.platform, "params": r.params, "still": still,
                             "drifts": drifts, "aborted": note})
        else:
            r.state = "done"
        r.finished = time.time()
        self.cal.publish()


# ---------------------------------------------------------------------------- the thrusters

THRUSTER_INSET = 0.081       # [m] the thrusters sit this far in from the body's edges (spot.massprops.centerOfGravity)
BURN_LATENCY = 0.15          # [s] from the command to the thruster pushing (SSH, the Teensy, the valve): skipped
BURN_LIFT = 0.5              # [s] the pucks lift the platform before the burn
REST_TIME = 1.0              # [s] resting between burns
BURN_RMS_LIMIT = 3.0         # [mm] a burn whose motion doesn't follow a constant force better than this is left out


def dist_from_cg(cg_mm):
    """The mixer's thruster_dist2CG [mm] for a centre of gravity at CG_MM [x, y] [mm] from the body's centre,
    as spot.massprops.centerOfGravity makes them."""
    e = 0.15 - THRUSTER_INSET
    X, Y = cg_mm[0] / 1000.0, cg_mm[1] / 1000.0
    return [1000 * v for v in (e - Y, -(e + Y), e - X, -(e + X), e + Y, -(e - Y), e + X, -(e - X))]


def assumed_defaults():
    """What the console takes each platform's mass, inertia and centre of gravity to be: the controller's
    defaults (control.DEFAULTS: SPOT's default settings), the CG worked back from its thruster offsets."""
    e = 0.15 - THRUSTER_INSET
    out = {}
    for n in PLATFORMS:
        p = DEFAULTS["platforms"][n]
        d = p["thruster_dist2CG"]
        out[n] = {"mass": p["mass"], "inertia": p["inertia"], "thrust": p["thrust"],
                  "cg_mm": [round(1000 * e - d[2], 3), round(1000 * e - d[0], 3)]}
    return out


def controller_params(platform):
    """The controller's parameters (control.Controller) for PLATFORM: SPOT's defaults, with the lab's
    calibrated thrust of each thruster when the lab has a thruster calibration for it."""
    params = copy.deepcopy(DEFAULTS)
    th = (store.get()["calibration"].get("thrusters") or {}).get(platform)
    if th:
        params["platforms"][platform]["thrust"] = list(th["thrust"])
    return params


class ThrusterRun:
    def __init__(self, cal, run):
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)

    def main(self):
        r, d, p = self.run, self.d, self.run.params
        order = [k for k in range(1, NUM_THRUSTERS + 1)]
        r.steps = ([{"name": "Start", "state": "waiting", "note": "A clear place to start"}] +
                   [{"name": "Thruster %d" % k, "state": "waiting", "note": ""} for k in order] +
                   [{"name": "Fit", "state": "waiting", "note": ""}])
        burns, ok, note = [], False, None
        try:
            d.begin_step(0)
            pose = d.check()
            if pose is None:
                raise Abort("PhaseSpace doesn't see the %s" % shown(r.platform))
            d.claim()
            start = self.start_place(pose)
            if start:
                d.say("Flying to a clear place to start (%.2f m, %.2f m)." % (start[0], start[1]))
                d.fly_to(start)
            d.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
            d.wait(REST_TIME)
            d.end_step(note="(%.2f, %.2f) m" % tuple(d.check()[:2]))
            for k in order:
                d.begin_step(k)
                notes = []
                for rep in range(p["repeats"]):
                    b = self.burn(k, rep)
                    burns.append(b)
                    notes.append("%.3f N" % b["fit"]["thrust"] if b.get("fit") else "no fit")
                    d.end_step("running", " · ".join(notes))
                d.end_step(note=" · ".join(notes))
            d.finish(True, "Calibration finished; all off")
            d.begin_step(len(r.steps) - 1)
            result = self.result(burns)
            folder = save_run(r, {"kind": "thrusters", "platform": r.platform, "params": p, "burns": burns,
                                  "result": result})
            result["folder"] = folder
            store.set_section("thrusters", result, r.platform)
            r.result = result
            bad = [k + 1 for k in range(NUM_THRUSTERS) if not result["valid"][k]]
            d.end_step("done" if not bad else "failed",
                       "%.3f-%.3f N" % (min(result["thrust"]), max(result["thrust"])))
            d.say("Done: thrust %s N.%s" % (", ".join("%.3f" % f for f in result["thrust"]),
                  " No good burn of thruster %s: it keeps the nominal thrust." % ", ".join(map(str, bad)) if bad else ""))
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:   # never leave the platform floating after a bug
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            d.finish(ok, "Calibration %s; all off" % ("finished" if ok else "stopped (%s)" % note))
        self.done(ok, note, burns)

    def start_place(self, pose):
        """Where to start: where it is if that leaves room for a burn (the margin plus 0.25 m from the edges,
        clear of the other platforms), else the nearest such place; None for here."""
        m = self.run.params["margin"] + 0.25
        x = min(max(pose[0], m), TABLE[0] - m)
        y = min(max(pose[1], m), TABLE[1] - m)
        here = abs(x - pose[0]) < 1e-9 and abs(y - pose[1]) < 1e-9
        clear = all(math.hypot(x - o[0], y - o[1]) >= CLEARANCE + 0.15 for o in self.d.others().values())
        if not clear:
            raise Abort("another platform is within %.2f m of where the %s would start; move it, or the %s"
                        % (CLEARANCE + 0.15, shown(self.run.platform), shown(self.run.platform)))
        return None if here else [x, y, pose[2]]

    def burn(self, k, rep):
        """One burn of thruster K: float, fire, back to the start, pucks off, rest. Returns its record and fit."""
        r, d, p = self.run, self.d, self.run.params
        pose = d.check()
        start = [pose[0], pose[1], pose[2]]
        d.say("Thruster %d%s: floating, then firing for %g s." % (k, " (repeat %d)" % (rep + 1) if p["repeats"] > 1 else "",
                                                                     p["burn"]))
        d.set(duty=[0.0] * NUM_THRUSTERS, pucks=True)
        d.start_recording()
        d.wait(BURN_LIFT)
        duty = [0.0] * NUM_THRUSTERS
        duty[k - 1] = 100.0
        d.set(duty=duty)
        t_on = time.time()
        cut = []

        def stop(pose):
            m = p["margin"]
            if not (m <= pose[0] <= TABLE[0] - m and m <= pose[1] <= TABLE[1] - m):
                cut.append("near an edge")
                return True
            return False
        d.wait(p["burn"], until=stop)
        t_off = time.time()
        d.set(duty=[0.0] * NUM_THRUSTERS)
        d.wait(0.2)
        rec = d.stop_recording()
        if cut:
            d.say("Thruster %d: burn cut short (%s)." % (k, cut[0]))
        r.records.append({"thruster": k, "points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 80)],
                          "cut": cut[0] if cut else None})
        record = {"thruster": k, "repeat": rep, "t_on": t_on, "t_off": t_off, "cut": cut[0] if cut else None,
                  "frames": rec, "start": start}
        record["fit"], record["error"] = self.fit(k, rec, t_on, t_off)
        # back where it started, then rest
        d.say("Thruster %d: flying back to the start." % k)
        d.fly_to(start)
        d.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
        d.wait(REST_TIME)
        return record

    def fit(self, k, rec, t_on, t_off):
        """(the burn's fit, or None, and why not)."""
        a = self.run.params["assumed"]
        burn = [s for s in rec if t_on + BURN_LATENCY <= s[4] <= t_off]
        if len(burn) < 20:
            return None, "too few frames during the burn (%d)" % len(burn)
        jumps = sum(1 for u, v in zip(burn, burn[1:]) if math.hypot(v[1] - u[1], v[2] - u[2]) > GLITCH_STEP)
        if jumps:
            return None, "PhaseSpace's positions jumped %d times" % jumps
        table = store.get()["calibration"].get("table")
        slope = table["height"] if table else None
        damping = (table or {}).get("damping", {}).get(self.run.platform, 0.0) if table else 0.0
        f = calfit.fit_burn([s[0] for s in burn], [s[1] for s in burn], [s[2] for s in burn], [s[3] for s in burn],
                            a["mass"], a["inertia"], k, a["dist2CG"][k - 1], slope=slope, damping=damping)
        if f["rms_mm"] > BURN_RMS_LIMIT:
            return None, "the motion doesn't follow a constant thrust (residual %.1f mm)" % f["rms_mm"]
        if not 0.02 <= f["thrust"] <= 1.5:
            return None, "an unlikely thrust (%.3f N): did the thruster fire?" % f["thrust"]
        return f, None

    def result(self, burns):
        """The calibration: each thruster's thrust (the mean of its good burns; the nominal one without any),
        its spread, how far it is turned, its torque ratio, and each burn, for the page."""
        r, p = self.run, self.run.params
        nominal = assumed_defaults()[r.platform]["thrust"]
        out = {"updated": now_iso(), "by": r.by, "platform": r.platform, "run": r.stamp, "params": p,
               "nominal": nominal, "table_used": bool(store.get()["calibration"].get("table")),
               "thrust": [], "thrust_sd": [], "angle": [], "torque_ratio": [], "valid": [], "burns": []}
        for k in range(1, NUM_THRUSTERS + 1):
            fits = [b["fit"] for b in burns if b["thruster"] == k and b["fit"]]
            mean, sd = calfit.summarise([f["thrust"] for f in fits])
            angle, _ = calfit.summarise([f["angle"] for f in fits])
            ratios = [f["torque_ratio"] for f in fits if f["torque_ratio"] is not None]
            ratio, _ = calfit.summarise(ratios)
            out["valid"].append(bool(fits))
            out["thrust"].append(mean if fits else nominal)
            out["thrust_sd"].append(sd if len(fits) > 1 else None)   # (one burn: no spread to give)
            out["angle"].append(angle)
            out["torque_ratio"].append(ratio)
        for b in burns:
            f = b["fit"]
            out["burns"].append({"thruster": b["thruster"], "repeat": b["repeat"], "cut": b["cut"], "error": b["error"],
                                 "thrust": f and f["thrust"], "angle": f and f["angle"],
                                 "torque_ratio": f and f["torque_ratio"], "rms_mm": f and f["rms_mm"],
                                 "points": [[round(s[1] - b["start"][0], 4), round(s[2] - b["start"][1], 4)]
                                            for s in thin([s for s in b["frames"] if s[4] >= b["t_on"]], 60)],
                                 "start_deg": math.degrees(b["start"][2])})
        return out

    def done(self, ok, note, burns):
        r = self.run
        if not ok:
            if r.step is not None and r.steps[r.step]["state"] == "running":
                r.steps[r.step]["state"] = "failed"
            stopped = r.state == "stopping"
            r.error = None if stopped else note
            r.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
            if burns:
                save_run(r, {"kind": "thrusters", "platform": r.platform, "params": r.params, "burns": burns,
                             "aborted": note})
        else:
            r.state = "done"
        r.finished = time.time()
        self.cal.publish()


# ---------------------------------------------------------------------------- the arm

ARM_TESTS = ("breakaway", "sweep", "inertia", "steps")
JOINT_NAMES = {1: "Shoulder", 2: "Elbow", 3: "Wrist"}
ARM_POLL = 2.0               # [s] between status checks while the script runs
ARM_TIMEOUT = 45 * 60        # [s] the longest a run may take (three joints, every test: about 15 min)


def arm_script():
    """The arm calibration script: next to the console on the TX2 (deploy_tx2.sh copies it there), or in
    the template's resources/arm_calibration."""
    for p in (PROJECT_DIR / "arm_calibration" / "calibrate_arm.py",
              PROJECT_DIR.parent.parent / "resources" / "arm_calibration" / "calibrate_arm.py"):
        if p.is_file():
            return p
    raise ApiError("calibrate_arm.py is not on the lab console: run deploy_tx2.sh (it copies "
                   "resources/arm_calibration/calibrate_arm.py there).")


def arm_check():
    """{"ok": whether a real run can start, "items": [{"what", "ok", "value", "fix"}], "raw"} from the Chaser's
    arm-cal-check: what calibrate_arm.py needs there."""
    rc, out, err = platform_run("RED", "arm-cal-check", timeout=20)
    text = decode(out)
    if rc != 0 or not text:
        reason = err or text or "no answer (code %s)" % rc
        if "only accepts" in reason:
            reason = "the Chaser's agent is older than this console: run setup_hardware_link.sh to update it"
        return {"ok": False, "reachable": False, "items": [], "error": reason, "raw": text}
    kv, users = {}, []
    for line in text.splitlines():
        key, _, value = line.partition(" ")
        if key == "user":
            users.append(value)
        else:
            kv[key] = value
    py = kv.get("python", "no")
    items = [
        {"what": "Python 3", "ok": py != "no", "value": py if py != "no" else "missing",
         "fix": "Install python3 on the Chaser."},
        {"what": "Dynamixel SDK", "ok": kv.get("sdk") == "yes", "value": "installed" if kv.get("sdk") == "yes" else "missing",
         "fix": "On the Chaser: pip3 install --user dynamixel-sdk"},
        {"what": "U2D2 at /dev/ttyUSB0", "ok": kv.get("port") == "yes", "value": "found" if kv.get("port") == "yes" else "not found",
         "fix": "Plug in the U2D2 and power the arm."},
        {"what": "Access to the port", "ok": kv.get("access") == "yes", "value": "yes" if kv.get("access") == "yes" else "no",
         "fix": "Run +spot/+hardware/arm/setup_arm_serial.sh for the Chaser (dialout group, udev rule), then log in again."},
        {"what": "Port free", "ok": not users, "value": ", ".join(users) if users else "free",
         "fix": "Stop the program holding it (an experiment?)."},
        {"what": "Latency timer", "ok": True, "warn": kv.get("latency") not in ("1",),
         "value": "%s ms" % kv["latency"] if kv.get("latency", "unknown") != "unknown" else "unknown",
         "fix": "setup_arm_serial.sh sets it to 1 ms (16 ms works, but the data comes slower)."},
    ]
    running = kv.get("running", "no")
    return {"ok": all(i["ok"] for i in items) and running == "no", "reachable": True, "items": items,
            "running": running != "no", "script": kv.get("script") == "yes", "raw": text}


class ArmRun:
    def __init__(self, cal, run):
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)
        self.hw = hardware.get("RED")
        self.reserved = False

    def main(self):
        r, d, p = self.run, self.d, self.run.params
        r.steps = ([{"name": "Check", "state": "waiting", "note": "What the Chaser has"},
                    {"name": "Send Script", "state": "waiting", "note": "calibrate_arm.py"}] +
                   [{"name": "%s %s" % (JOINT_NAMES[j], t.capitalize()), "state": "waiting", "note": "", "joint": j, "test": t}
                    for j in p["joints"] for t in p["tests"]] +
                   [{"name": "Collect", "state": "waiting", "note": "Data and fit"}])
        r.check = None
        ok, note, started = False, None, False
        try:
            d.begin_step(0)
            chk = arm_check()
            r.check = chk
            if not chk["reachable"]:
                raise Abort("the Chaser doesn't answer: %s" % chk["error"])
            if chk["running"]:
                raise Abort("a calibration is already running on the Chaser (stop it, or wait for it)")
            missing = [i["what"] for i in chk["items"] if not i["ok"]]
            if missing and not p["simulate"]:
                raise Abort("the Chaser isn't ready: %s (see the checks)" % ", ".join(missing))
            d.end_step(note="ready" if not missing else "rehearsal (%s missing)" % ", ".join(missing))
            if not p["simulate"]:   # the platform rests while the arm moves: nothing may float it
                self.hw.reserve(r, r.by)
                self.reserved = True
            d.begin_step(1)
            buf = io.BytesIO()
            with tarfile.open(fileobj=buf, mode="w") as tar:
                tar.add(str(arm_script()), arcname="calibrate_arm.py")
            rc, out, err = platform_run("RED", "arm-cal-receive", data=buf.getvalue(), timeout=30)
            if rc != 0:
                raise Abort("could not send the script: %s" % (err or decode(out)))
            d.end_step()
            cmd = "arm-cal-start %s %s%s" % ("".join(str(j) for j in p["joints"]), ",".join(p["tests"]),
                                             " simulate" if p["simulate"] else "")
            rc, out, err = platform_run("RED", cmd, timeout=30)
            if rc != 0 or "started" not in decode(out):
                raise Abort("the script didn't start: %s" % (err or decode(out)))
            started = True
            d.say("%s started on the Chaser (%s; %s)." % ("A rehearsal (simulated servos)" if p["simulate"] else "The arm calibration",
                                                          ", ".join(JOINT_NAMES[j] for j in p["joints"]), ", ".join(p["tests"])))
            code = self.follow()
            started = False
            if code != 0:
                raise Abort("the script stopped (exit %s): %s" % (code, self.last_words()))
            d.begin_step(len(r.steps) - 1)
            result = self.collect()
            if p["simulate"]:
                r.result = result
                d.end_step(note="rehearsal: not saved")
                d.say("Rehearsal done: the fit of the simulated servos is shown; the lab's arm calibration is unchanged.")
            else:
                store.set_section("arm", result)
                r.result = result
                d.end_step(note="saved")
                d.say("Done: the arm calibration is saved; projects get it with Update from Lab.")
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            if started:   # never leave the script moving the arm when the run ends
                platform_run("RED", "arm-cal-stop", timeout=20)
            if self.reserved:
                self.hw.release(r, "Arm calibration %s" % ("finished" if ok else "stopped"))
        self.done(ok, note)

    def follow(self):
        """Follow the script until it ends (its exit code); stops it when the run is stopped, the Chaser is taken
        back (Esc or all off on the Hardware tab), or it runs too long."""
        r = self.run
        t0 = time.monotonic()
        while True:
            if r.state != "running":
                platform_run("RED", "arm-cal-stop", timeout=20)
                raise Abort(r.stop_reason or "stopped")
            if self.reserved and not self.hw.owned_by(r):
                platform_run("RED", "arm-cal-stop", timeout=20)
                raise Abort("the Chaser was taken back (switched off on the Hardware tab)")
            if time.monotonic() - t0 > ARM_TIMEOUT:
                platform_run("RED", "arm-cal-stop", timeout=20)
                raise Abort("it ran for more than %d minutes" % (ARM_TIMEOUT // 60))
            rc, out, err = platform_run("RED", "arm-cal-status", timeout=20)
            if rc == 0:
                text = decode(out)
                lines = [l[4:] for l in text.splitlines() if l.startswith("log ")]
                self.progress(lines)
                code = re.search(r"^exit (\d+)$", text, re.M)
                running = re.search(r"^running (\d+)$", text, re.M)
                if not running and code:
                    return int(code.group(1))
                if not running and not code:
                    return -1
            time.sleep(ARM_POLL)

    def progress(self, lines):
        """The script's log on the page, and its PROGRESS lines as the steps."""
        r = self.run
        r.armlog = lines
        last = None
        for l in lines:
            if l.startswith("PROGRESS "):
                try:
                    last = json.loads(l[9:])
                except ValueError:
                    pass
        if last:
            for k, st in enumerate(r.steps):
                if st.get("joint") == last["joint"] and st.get("test") == last["test"]:
                    if r.step != k:
                        for prev in r.steps[2:k]:
                            if prev["state"] in ("waiting", "running"):
                                prev["state"] = "done"
                        self.d.begin_step(k, "%d of %d" % (last["index"], last["total"]))
                        self.d.say("%s: %s (%d of %d)." % (JOINT_NAMES[last["joint"]], last["test"], last["index"], last["total"]),
                                   log=False)
        tail = [l for l in lines if l.strip() and not l.startswith("PROGRESS ")]
        if tail:
            r.message = tail[-1].strip()[:200]
        self.cal.publish(force=False)

    def last_words(self):
        lines = [l for l in getattr(self.run, "armlog", []) if l.strip() and not l.startswith(("PROGRESS ", "EXIT "))]
        return lines[-1].strip() if lines else "no output"

    def collect(self):
        """The run's files from the Chaser into ~/SPOTCalibration/<stamp>_arm_RED/, and the calibration: the
        script's fit (arm_calibration.json) with what the page draws (each joint's friction points, its
        breakaway torques, its position steps, thinned)."""
        r = self.run
        rc, out, err = platform_run("RED", "arm-cal-data", timeout=120)
        if rc != 0 or not out:
            raise Abort("could not collect the data: %s" % (err or "nothing came back"))
        folder = RUNS_DIR / ("%s_arm_RED%s" % (r.stamp, "_rehearsal" if r.params["simulate"] else ""))
        folder.mkdir(parents=True, exist_ok=True)
        files = {}
        with tarfile.open(fileobj=io.BytesIO(out), mode="r") as tar:
            for m in tar.getmembers():
                name = m.name.rsplit("/", 1)[-1]
                if not m.isfile() or not re.match(r"^[A-Za-z0-9_.-]+\.(json|csv)$", name) or m.size > 20 * 1024 * 1024:
                    continue
                data = tar.extractfile(m).read()
                (folder / name).write_bytes(data)
                files[name] = data.decode("utf-8", "replace")
        if "arm_calibration.json" not in files:
            raise Abort("the run has no arm_calibration.json (did it finish?)")
        cal = json.loads(files["arm_calibration.json"])
        if cal.get("format") != "spot-arm-calibration":
            raise Abort("arm_calibration.json isn't an arm calibration")
        cal.pop("folder", None)
        cal.update(updated=now_iso(), by=r.by, run=r.stamp, simulated=r.params["simulate"], lab_folder=str(folder),
                   tests=r.params["tests"], plots=arm_plots(files, cal))
        return cal

    def done(self, ok, note):
        r = self.run
        if not ok:
            if r.step is not None and r.steps[r.step]["state"] == "running":
                r.steps[r.step]["state"] = "failed"
            stopped = r.state == "stopping"
            r.error = None if stopped else note
            r.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
        else:
            r.state = "done"
        r.finished = time.time()
        self.cal.publish()


def read_csv_text(text):
    rows = [l.split(",") for l in text.strip().splitlines()]
    if not rows:
        return [], []
    head = rows[0]
    out = []
    for row in rows[1:]:
        try:
            out.append([float(v) for v in row])
        except ValueError:
            continue
    return head, out


def arm_plots(files, cal):
    """What the page draws of an arm calibration, per joint: the friction against speed (the sweep's steady
    points as torque, and the fitted curve's parameters are in the joint's fit), the breakaway torques, and
    the position steps (goal and measured, thinned)."""
    kt = cal.get("kt") or {}
    plots = {}
    for j in (1, 2, 3):
        p = {}
        k = kt.get(str(j)) if isinstance(kt, dict) else kt
        pts = files.get("sweep_points_j%d.json" % j)
        if pts and k:
            try:
                p["sweep"] = [[round(q["velocity"], 5), round(k * q["current"], 5)] for q in json.loads(pts)]
            except (ValueError, KeyError, TypeError):
                pass
        if "steps_j%d.csv" % j in files:
            head, rows = read_csv_text(files["steps_j%d.csv" % j])
            col = "q%d" % j
            if rows and "t" in head and "goal" in head and col in head:
                ti, gi, qi = head.index("t"), head.index("goal"), head.index(col)
                p["steps"] = [[round(row[ti], 3), round(row[gi], 5), round(row[qi], 5)] for row in thin(rows, 400)]
        if p:
            plots[str(j)] = p
    return plots


def noise_mm(samples):
    """The cameras' noise on a resting platform: the standard deviation of its position [mm]."""
    if not samples or len(samples) < 5:
        return None
    mx = sum(s[1] for s in samples) / len(samples)
    my = sum(s[2] for s in samples) / len(samples)
    var = sum((s[1] - mx) ** 2 + (s[2] - my) ** 2 for s in samples) / (2 * (len(samples) - 1))
    return 1000 * math.sqrt(var)
