"""The Calibration tab: measuring the lab, and the calibration file.

Measuring the lab so that simulations behave as the experiments do,
and the calibration file that holds the results.

The file (one per console): the lab console keeps the lab's in its
home (~/.spot_calibration.json), next to the raw data of every run
(~/SPOTCalibration/<stamp>_<kind>/run.json); a project keeps a copy in
spot_calibration.json (next to spot_settings.json), which
spot.params.apply reads (spot.params.loadCalibration) for the
simulation. Whenever the local console sees the lab console and the
lab's file is newer, its page offers to update the project's copy
(which can be declined, or never asked again: the copy's "sync").

A project's copy can also override single values ("overrides", set on
the Simulation tab's Calibration pane). Updating from the lab keeps
them, and a part of the lab's calibration whose every value is
overridden is not offered as newer (sections_newer).

    {"format": "spot-calibration", "version": 1,
     "updated": "2026-10-13T15:02:11Z",  (the newest section's; UTC,
                                          so it sorts as text)
     "table": {"updated", "by", "model": "quadratic" | "plane",
               "height": [a1 .. a5] (calfit.py), "centre": [x, y],
               "damping": {"RED": c, ...}, "tilt_centre_mrad", ...,
               "drifts": [...]},
     "thrusters": {"RED": {"updated", "by", "thrust": [8 x N], ...},
                   ...},
     "arm": {... spot-arm-calibration ...},
     "sync": {"ask": true, "declined": null},  (a project's copy only)
     "overrides": {                            (a project's copy only;
       "table": {"height": [5 x number or null],     SI units, null:
                 "damping": {"RED": c, ...}},        not overridden)
       "thrusters": {"RED": {"thrust": [8 x number or null],
                             "decay": d}, ...},
       "arm": {"static": [3 x number or null], "coulomb", "stribeck",
               "viscous", "armature"}}}

The table slope calibration (TableRun), with PhaseSpace streaming and
the platform resting on the table:
  1. record it sitting still (the cameras' noise);
  2. float it (pucks on, thrusters off) and record it drifting from
     where it is;
  3. fly it (the diagram's own controller, as Interactive Mode) to
     each point of a grid over the table, let it settle, switch the
     thrusters and pucks off so it comes to rest there, then float it
     again and record it drifting;
  4. pucks off, and fit the table's height surface and the air
     bearings' drag to the drifts (calfit.py).
A drift ends after its time, or at once (pucks off: the platform stops
on the table) when the platform nears an edge or another platform, or
goes too fast.

The thruster calibration (ThrusterRun), with PhaseSpace streaming and
the platform resting on the table, for each thruster 1-8 (and each
repeat):
  1. fly it (pucks on, the diagram's own controller) to the table's
     centre;
  2. pucks off, and let it come to rest (REST_TIME);
  3. pucks on and the thruster at 100 % at once, for the burn time,
     recording every PhaseSpace frame.
Each burn's thrust comes from the motion it gave the platform
(calfit.fit_burn), with the platform's mass, inertia and centre of
gravity taken as known (the person starting it confirms them; the
controller's defaults unless changed) and the table's pull (its
calibration) taken out. The lab's calibrated thrust is then what
Interactive Mode, the calibrations' own flights and (through the
projects' copies) simulations and experiments use.

The thrusters together (DecayRun), the second stage, after a thruster
calibration of that platform: each firing opens thrusters 1, then 1-2,
1-3, ... 1-8 at once (as one burn above: centre, pucks off and rest,
pucks on and the thrusters firing), and its push is compared with the
sum of what each gives alone (the thruster calibration).
calfit.fit_decay fits how the output falls with the number open; the
result is kept with the platform's thruster calibration (its "decay").
All eight together give no net push (opposite thrusters cancel), so
that firing is only a check; the fit covers it.

The arm calibration (ArmRun) runs the arm calibration script
(calibrate_arm.py, from the template's +spot/+arm/calibration; on the
TX2 next to the console) on the Chaser, through its agent
(spot_agent.sh: arm-cal-check, -receive, -start, -status, -stop,
-data). It checks the Chaser has what the script needs (python3, the
Dynamixel SDK, the U2D2 and access to it, nothing else holding it),
keeps the Chaser resting (nothing can float it meanwhile), sends the
script, starts it, follows its progress, and collects its data and
fit. A rehearsal runs the script's simulated servos instead: it needs
none of that hardware, and its result is shown but never becomes the
lab's.

A calibration drives one platform at a time, through the hardware
driver (hardware.py: claim, drive, release), as Interactive Mode does:
it can't start while an experiment or Interactive Mode runs, and they
can't start while it runs. Everything goes off, and the run stops,
when the Calibration tab's Stop (or Esc) is pressed, the Hardware tab
switches the platform off, PhaseSpace loses it, it leaves the table,
the link to it is lost, or the console stops. And its Teensy switches
everything off by itself when packets stop for 2 s.

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
MAX_FILE = 4 * 1024 * 1024                   # [bytes] a calibration is a few hundred kB at most

# Flying between points (fly_to): the goal moves towards the point at
# these speeds, so the controller isn't kicked by a jump in its goal
# (as a dragged goal in Interactive Mode moves).
GOAL_SPEED = 0.10            # [m/s]
GOAL_TURN = math.radians(15)  # [rad/s]

# There: within SETTLED_POS of the point and SETTLED_ATT of its
# attitude, and slower than SETTLED_SPEED, for SETTLED_TIME. Only
# roughly at the point: the diagram's LQR has no integral action, so on
# a sloping table it holds the platform a few centimetres downhill of
# its goal; what matters is that it is at rest.
SETTLED_POS = 0.10           # [m]
SETTLED_ATT = math.radians(5)
SETTLED_SPEED = 0.01         # [m/s]
SETTLED_TIME = 1.5           # [s]
FLY_TIMEOUT = 90.0           # [s] to reach a point
LOST_TIMEOUT = 1.0           # [s] PhaseSpace may lose the platform this long (thrusters off)
CLEARANCE = 0.45             # [m] from other platforms (centre to centre), as Interactive Mode
# [s] after the pucks go off (at each point before its drift, or to end
# a drift early): the platform comes to rest.
STOP_PAUSE = 5.0
DRIFT_LIFT = 5.0             # [s] the pucks lift the platform before a drift is recorded

# The data's own check: from one frame to the next a drifting platform
# moves at most a few mm, so a bigger jump is a glitch (PhaseSpace
# mis-tracking it, or a second stream on the monitor's port); with more
# than GLITCH_FRACTION of them, the run is not saved.
GLITCH_STEP = 0.02           # [m]
GLITCH_FRACTION = 0.005

# The fit's check: a position residual above POOR_FIT_FACTOR times the
# cameras' noise (at least POOR_FIT_FLOOR) means the model doesn't
# describe the table well (a plane on a curved table: more grid points
# give a surface), which the result says; above REJECT_RESIDUAL
# something else moved the platform: not saved.
POOR_FIT_FACTOR = 5.0
POOR_FIT_FLOOR = 1.0         # [mm]
REJECT_RESIDUAL = 20.0       # [mm]

MIN_THRUST = 0.01            # [N] plausible calibrated thrust
MAX_THRUST = 2.0             # [N]

# Values a project's copy can override, with their plausible ranges (SI
# units, as in the file): the table's height coefficients [a1 a2: m/m;
# a3 a4 a5: 1/m] and air-bearing drag [1/s], each thruster's thrust [N]
# and the thrust decay per extra valve, and each arm joint's friction
# [N m, rad/s, N m s/rad] and armature [kg m^2].
NUM_HEIGHT = 5
NUM_JOINTS = 3
MAX_HEIGHT = 1.0
MAX_DAMPING = 10.0           # [1/s]
MAX_DECAY = 0.5              # as spot.params.loadCalibration
ARM_FIELDS = {"static": (0.0, 10.0), "coulomb": (0.0, 10.0), "stribeck": (1e-4, 10.0),
              "viscous": (0.0, 10.0), "armature": (1e-6, 1.0)}
PUBLISH_PERIOD = 0.2         # [s] run status to the pages, at most this often


def now_iso():
    """Return the time now, ISO 8601 in UTC (so it sorts as text)."""
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def stamp():
    """Return the local time as a folder stamp: YYYYmmdd_HHMMSS."""
    return time.strftime("%Y%m%d_%H%M%S")


class Abort(Exception):
    """A calibration run stops (the message says why)."""


# The file


def _finite(v, what):
    """Return v as a float if it is a finite number (not a bool).

    Raises:
        ApiError: otherwise, naming what.
    """
    if not isinstance(v, (int, float)) or isinstance(v, bool) or not math.isfinite(v):
        raise ApiError("%s must be a finite number." % what)
    return float(v)


def _numbers(v, n, what):
    """Return v as a list of n finite floats.

    Raises:
        ApiError: otherwise, naming what.
    """
    if not isinstance(v, list) or len(v) != n:
        raise ApiError("%s must be a list of %d numbers." % (what, n))
    return [_finite(x, what) for x in v]


def _override_value(v, low, high, what):
    """Return an override as a float in [low, high] (None stays None).

    Raises:
        ApiError: if it is neither null nor such a number.
    """
    if v is None:
        return None
    v = _finite(v, what)
    if not low <= v <= high:
        raise ApiError("%s must be %g to %g." % (what, low, high))
    return v


def _override_list(v, n, low, high, what):
    """Return n overrides (None where not overridden), or None if none.

    Raises:
        ApiError: if it isn't a list of n numbers or nulls in range.
    """
    if v is None:
        return None
    if not isinstance(v, list) or len(v) != n:
        raise ApiError("%s must be a list of %d numbers or nulls." % (what, n))
    out = [_override_value(x, low, high, what) for x in v]
    return out if any(x is not None for x in out) else None


def check_overrides(ov):
    """Return a project's overrides, checked, with empty parts left out.

    Args:
        ov: The "overrides" object (or None).

    Raises:
        ApiError: if a value isn't a plausible number in its place.
    """
    if ov is None:
        return {}
    if not isinstance(ov, dict):
        raise ApiError("calibration.overrides must be an object.")
    out = {}

    # The table: its height coefficients and each platform's drag.
    table = ov.get("table") or {}
    if not isinstance(table, dict):
        raise ApiError("overrides.table must be an object.")
    checked = {}
    height = _override_list(table.get("height"), NUM_HEIGHT, -MAX_HEIGHT, MAX_HEIGHT,
                            "overrides.table.height")
    if height:
        checked["height"] = height
    damping = table.get("damping") or {}
    if not isinstance(damping, dict) or any(k not in PLATFORMS for k in damping):
        raise ApiError("overrides.table.damping must map RED, BLACK, BLUE to numbers.")
    damping = dict((k, _override_value(v, 0.0, MAX_DAMPING, "overrides.table.damping.%s" % k))
                   for k, v in damping.items())
    damping = dict((k, v) for k, v in damping.items() if v is not None)
    if damping:
        checked["damping"] = damping
    if checked:
        out["table"] = checked

    # Each platform's thrusts and thrust decay.
    thrusters = ov.get("thrusters") or {}
    if not isinstance(thrusters, dict) or any(k not in PLATFORMS for k in thrusters):
        raise ApiError("overrides.thrusters must map RED, BLACK, BLUE to their overrides.")
    checked = {}
    for k, v in thrusters.items():
        if not isinstance(v, dict):
            raise ApiError("overrides.thrusters.%s must be an object." % k)
        mine = {}
        thrust = _override_list(v.get("thrust"), NUM_THRUSTERS, MIN_THRUST, MAX_THRUST,
                                "overrides.thrusters.%s.thrust" % k)
        if thrust:
            mine["thrust"] = thrust
        decay = _override_value(v.get("decay"), 0.0, MAX_DECAY,
                                "overrides.thrusters.%s.decay" % k)
        if decay is not None:
            mine["decay"] = decay
        if mine:
            checked[k] = mine
    if checked:
        out["thrusters"] = checked

    # Each arm joint's friction and armature.
    arm = ov.get("arm") or {}
    if not isinstance(arm, dict) or any(k not in ARM_FIELDS for k in arm):
        raise ApiError("overrides.arm may only hold %s." % ", ".join(sorted(ARM_FIELDS)))
    checked = {}
    for name, (low, high) in ARM_FIELDS.items():
        values = _override_list(arm.get(name), NUM_JOINTS, low, high, "overrides.arm.%s" % name)
        if values:
            checked[name] = values
    if checked:
        out["arm"] = checked
    return out


def parse_override(path):
    """Return where an override goes, and its range.

    Args:
        path: "table.height.<0-4>", "table.damping.<PLATFORM>",
            "thrusters.<PLATFORM>.thrust.<0-7>",
            "thrusters.<PLATFORM>.decay" or "arm.<field>.<0-2>".

    Returns:
        (keys, index, low, high): the keys down to the value (a list's
        when index is not None), and its plausible range.

    Raises:
        ApiError: if the path names no value that can be overridden.
    """
    parts = str(path).split(".")

    def index(n):
        """Return the list index the path ends with (below n)."""
        if not parts[-1].isdigit() or int(parts[-1]) >= n:
            raise ApiError("No such value to override: %s." % path)
        return int(parts[-1])

    if parts[:2] == ["table", "height"] and len(parts) == 3:
        return ("table", "height"), index(NUM_HEIGHT), -MAX_HEIGHT, MAX_HEIGHT
    if parts[:2] == ["table", "damping"] and len(parts) == 3 and parts[2] in PLATFORMS:
        return ("table", "damping", parts[2]), None, 0.0, MAX_DAMPING
    if parts[0] == "thrusters" and len(parts) >= 3 and parts[1] in PLATFORMS:
        if parts[2] == "thrust" and len(parts) == 4:
            return (("thrusters", parts[1], "thrust"), index(NUM_THRUSTERS),
                    MIN_THRUST, MAX_THRUST)
        if parts[2] == "decay" and len(parts) == 3:
            return ("thrusters", parts[1], "decay"), None, 0.0, MAX_DECAY
    if parts[0] == "arm" and len(parts) == 3 and parts[1] in ARM_FIELDS:
        low, high = ARM_FIELDS[parts[1]]
        return ("arm", parts[1]), index(NUM_JOINTS), low, high
    raise ApiError("No such value to override: %s." % path)


def validate(cal, local=False):
    """Return a checked copy of a calibration.

    The values the simulation reads are checked; the rest is what the
    pages show.

    Args:
        cal: The calibration.
        local: A project's copy (it keeps "sync" and "overrides").

    Raises:
        ApiError: if it isn't a SPOT calibration, or a value the
            simulation reads isn't valid.
    """
    if not isinstance(cal, dict):
        raise ApiError("The calibration must be an object.")
    if cal.get("format", "spot-calibration") != "spot-calibration":
        raise ApiError("This is not a SPOT calibration file.")
    out = {"format": "spot-calibration", "version": 1}

    # The table: the height coefficients, the centre, each platform's
    # drag and the model.
    table = cal.get("table")
    if table is not None:
        if not isinstance(table, dict):
            raise ApiError("calibration.table must be an object.")
        checked = copy.deepcopy(table)
        checked["height"] = _numbers(table.get("height"), 5, "table.height")
        checked["centre"] = _numbers(table.get("centre", list(calfit.CENTRE)), 2, "table.centre")
        damping = table.get("damping") or {}
        if not isinstance(damping, dict) or any(k not in PLATFORMS for k in damping):
            raise ApiError("table.damping must map RED, BLACK, BLUE to numbers.")
        checked["damping"] = dict((k, _finite(v, "table.damping.%s" % k))
                                  for k, v in damping.items())
        if checked.get("model") not in ("quadratic", "plane"):
            raise ApiError("table.model must be quadratic or plane.")
        out["table"] = checked

    # The thrusters: each platform's 8 plausible thrusts.
    thrusters = cal.get("thrusters")
    if thrusters is not None:
        if not isinstance(thrusters, dict) or any(k not in PLATFORMS for k in thrusters):
            raise ApiError("calibration.thrusters must map RED, BLACK, BLUE to their "
                           "calibration.")
        out["thrusters"] = {}
        for k, v in thrusters.items():
            if not isinstance(v, dict):
                raise ApiError("thrusters.%s must be an object." % k)
            checked = copy.deepcopy(v)
            checked["thrust"] = _numbers(v.get("thrust"), NUM_THRUSTERS,
                                         "thrusters.%s.thrust" % k)
            if not all(MIN_THRUST <= f <= MAX_THRUST for f in checked["thrust"]):
                raise ApiError("thrusters.%s.thrust must be 0.01 to 2 N each." % k)
            out["thrusters"][k] = checked

    # The arm: calibrate_arm.py's own format.
    arm = cal.get("arm")
    if arm is not None:
        if not isinstance(arm, dict) or arm.get("format") != "spot-arm-calibration":
            raise ApiError("calibration.arm must be an arm calibration (calibrate_arm.py's).")
        out["arm"] = copy.deepcopy(arm)

    # The newest section's time, and a project copy's "sync" and
    # "overrides".
    out["updated"] = newest(out)
    if local:
        sync = cal.get("sync") or {}
        out["sync"] = {"ask": bool(sync.get("ask", True)),
                       "declined": sync.get("declined")
                                   if isinstance(sync.get("declined"), str) else None}
        out["overrides"] = check_overrides(cal.get("overrides"))
    return out


def thruster_updated(c):
    """Return a thruster calibration's time (thrusts' or decay's)."""
    times = [c.get("updated"), (c.get("decay") or {}).get("updated")]
    return max([t for t in times if isinstance(t, str)] or [""])


def newest(cal):
    """Return the newest section's time (None if empty)."""
    times = [cal["table"].get("updated")] if cal.get("table") else []
    times += [thruster_updated(v) for v in (cal.get("thrusters") or {}).values()]
    if cal.get("arm"):
        times.append(cal["arm"].get("updated"))
    times = [t for t in times if isinstance(t, str)]
    return max(times) if times else None


def sections_newer(lab, mine):
    """Return the parts of the lab's calibration a project should take.

    A part counts when the lab's is newer than (or not in) the
    project's, and at least one of its values is not overridden in the
    project (calibration.js's newerSections does the same for the
    page).

    Returns:
        For example ["table", "thrusters RED", "decay RED", "arm"].
    """
    ov = mine.get("overrides") or {}
    out = []

    def open_values(overridden, n):
        """Return whether a part's n values aren't all overridden."""
        return n > sum(1 for v in (overridden or []) if v is not None)

    # The table: its height coefficients and the drags it measured.
    lab_table, my_table = lab.get("table"), mine.get("table")
    if lab_table and (not my_table
                      or lab_table.get("updated", "") > my_table.get("updated", "")):
        table_ov = ov.get("table") or {}
        drags = [k for k in (lab_table.get("damping") or {})
                 if k not in (table_ov.get("damping") or {})]
        if open_values(table_ov.get("height"), NUM_HEIGHT) or drags:
            out.append("table")

    # Each platform's thrusts, and its thrust decay.
    for name, c in sorted((lab.get("thrusters") or {}).items()):
        my_cal = (mine.get("thrusters") or {}).get(name) or {}
        my_ov = (ov.get("thrusters") or {}).get(name) or {}
        if (c.get("updated", "") > my_cal.get("updated", "")
                and open_values(my_ov.get("thrust"), NUM_THRUSTERS)):
            out.append("thrusters " + name)
        lab_decay, my_decay = c.get("decay") or {}, my_cal.get("decay") or {}
        if (lab_decay and lab_decay.get("updated", "") > my_decay.get("updated", "")
                and my_ov.get("decay") is None):
            out.append("decay " + name)

    # The arm: each field of the joints it measured.
    lab_arm, my_arm = lab.get("arm"), mine.get("arm")
    if lab_arm and (not my_arm or lab_arm.get("updated", "") > my_arm.get("updated", "")):
        arm_ov = ov.get("arm") or {}
        joints = [j.get("index") for j in (lab_arm.get("joints") or [])
                  if isinstance(j, dict) and j.get("index") in (1, 2, 3)]
        if any((arm_ov.get(f) or [None] * NUM_JOINTS)[j - 1] is None
               for j in joints for f in ARM_FIELDS):
            out.append("arm")
    return out


class CalibrationStore:
    """This console's calibration file (as DesignStore, design.py)."""

    def __init__(self):
        """Start with an empty calibration and no file."""
        self._lock = threading.Lock()

        # Held through each change (read, change, write), so that
        # changes made at the same time (the server's threads) don't
        # undo each other.
        self._changing = threading.Lock()
        self.path = None
        self.local = False
        self.cal = validate({})
        self.saved = False

    def open(self, path, local=False):
        """Use a calibration file, reading it if it exists.

        A file that can't be read is reported in the log, and no
        calibration is used.

        Args:
            path: The file.
            local: A project's copy (it keeps "sync").
        """
        self.path, self.local = Path(path), local
        self.cal = validate({}, local)
        try:
            text = self.path.read_text(encoding="utf-8")
            self.cal = validate(json.loads(text), local)
            self.saved = True
        except FileNotFoundError:
            pass
        except (OSError, ValueError, ApiError) as e:
            hub.log("hardware", "The calibration file %s could not be read (%s); no calibration "
                                "is used." % (self.path, e))

    def get(self):
        """Return the calibration, its file and whether it is saved."""
        with self._lock:
            return {"calibration": copy.deepcopy(self.cal),
                    "path": str(self.path) if self.path else None, "saved": self.saved}

    def _write(self, cal):
        """Save a calibration (checked) and tell the pages.

        Call without the lock. The file is replaced atomically.

        Raises:
            ApiError: if it isn't valid, or is larger than MAX_FILE.
        """
        cal = validate(cal, self.local)
        data = json.dumps(cal, indent=1, allow_nan=False) + "\n"
        if len(data) > MAX_FILE:
            raise ApiError("The calibration would be %d kB; at most %d kB."
                           % (len(data) // 1024, MAX_FILE // 1024))
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
        """Replace one section (a platform's, for thrusters).

        Args:
            section: "table", "thrusters" or "arm".
            value: The new section; None to remove it.
            platform: For thrusters, the platform.
        """
        with self._changing:
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

            # Removed sections are left out.
            cal = dict((k, v) for k, v in cal.items() if v is not None)
            return self._write(cal)

    def take(self, lab_cal):
        """Make a project's copy the lab's.

        The copy keeps its own "sync" and "overrides".
        """
        with self._changing:
            with self._lock:
                sync = copy.deepcopy(self.cal.get("sync"))
                overrides = copy.deepcopy(self.cal.get("overrides"))
            cal = dict(validate(lab_cal), sync=sync, overrides=overrides)
            return self._write(cal)

    def set_override(self, path, value):
        """Override a value of a project's copy, or stop overriding it.

        Args:
            path: The value (parse_override).
            value: Its new value (SI units); None to use the measured
                one again.

        Raises:
            ApiError: on the lab console, or if the path or value isn't
                valid.
        """
        with self._changing:
            if not self.local:
                raise ApiError("Only a project's calibration can be overridden.")
            keys, index, low, high = parse_override(path)
            value = _override_value(value, low, high, path)
            with self._lock:
                cal = copy.deepcopy(self.cal)

            # Down to the value's parent, making the objects on the way.
            node = cal.setdefault("overrides", {})
            for key in keys[:-1]:
                node = node.setdefault(key, {})
            if index is None:
                node[keys[-1]] = value
            else:
                size = NUM_HEIGHT if keys[0] == "table" else (
                    NUM_THRUSTERS if keys[0] == "thrusters" else NUM_JOINTS)
                values = node.get(keys[-1]) or [None] * size
                values[index] = value
                node[keys[-1]] = values

            # Values set back to null are left out (check_overrides).
            if index is None and value is None:
                node.pop(keys[-1], None)
            return self._write(cal)

    def clear_overrides(self, part=None):
        """Stop overriding a part's values (all, if part is None).

        Args:
            part: "table", "thrusters", "thrusters.<PLATFORM>" or "arm".

        Raises:
            ApiError: on the lab console, or for another part.
        """
        with self._changing:
            if not self.local:
                raise ApiError("Only a project's calibration can be overridden.")
            with self._lock:
                cal = copy.deepcopy(self.cal)
            ov = cal.get("overrides") or {}
            if part is None:
                ov = {}
            elif part in ("table", "thrusters", "arm"):
                ov.pop(part, None)
            elif part.startswith("thrusters.") and part.split(".", 1)[1] in PLATFORMS:
                (ov.get("thrusters") or {}).pop(part.split(".", 1)[1], None)
            else:
                raise ApiError("No such part of the overrides: %s." % part)
            cal["overrides"] = ov
            return self._write(cal)

    def set_sync(self, ask=None, declined=None):
        """Change whether to offer the lab's newer calibration.

        Args:
            ask: False: never ask again.
            declined: The lab calibration's time that was declined.
        """
        with self._changing:
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


# A run


class Run:
    """One calibration being run: what the page shows of it."""

    label = "Calibration"   # what the hardware tab says drives the platform

    def __init__(self, kind, platform, params, requester):
        """Start a run's record.

        Args:
            kind: "table", "thrusters", "decay" or "arm".
            platform: The platform's key.
            params: The run's settings.
            requester: Who started it.
        """
        self.kind = kind
        self.platform = platform
        self.params = params
        self.by = requester
        self.started = time.time()
        self.stamp = stamp()
        self.state = "running"            # running, stopping, done, failed, stopped
        self.step = None                  # the step under way (an index into steps)
        # [{"name", "state", "note"}]; state: waiting, running, done,
        # skipped or failed.
        self.steps = []
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
        """Return what the page shows of the run."""
        return {"kind": self.kind, "platform": self.platform, "params": self.params,
                "by": self.by, "started": self.started, "state": self.state,
                "step": self.step, "steps": self.steps, "message": self.message,
                "log": list(self.log), "pose": self.pose, "goal": self.goal,
                "path": self.path[-400:], "records": self.records, "result": self.result,
                "points": getattr(self, "points", None),
                "check": getattr(self, "check", None),
                "armlog": getattr(self, "armlog", [])[-40:],
                "error": self.error, "finished": self.finished}


class Calibrator:
    """Runs one calibration at a time on the lab console."""

    def __init__(self):
        """Start with no run."""
        self._lock = threading.Lock()
        self.run = None        # the run under way, or the last one
        self._published = 0.0

    def active(self):
        """Return whether a calibration is running (or stopping)."""
        with self._lock:
            return self.run is not None and self.run.state in ("running", "stopping")

    def status(self):
        """Return whether one runs, and the current or last run."""
        with self._lock:
            return {"active": self.run is not None and self.run.state in ("running", "stopping"),
                    "run": self.run.status() if self.run else None}

    def publish(self, force=True):
        """Send the status to the pages (unforced: every 0.2 s)."""
        now = time.monotonic()
        if not force and now - self._published < PUBLISH_PERIOD:
            return
        self._published = now
        hub.publish("calibration_run", self.status())

    # Requests

    def _check_free(self, platform):
        """Refuse unless the platform can be calibrated now.

        Raises:
            ApiError: if PhaseSpace isn't streaming or doesn't see it,
                or an experiment, Interactive Mode or a calibration
                runs.
        """
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first (PhaseSpace tab): the calibration "
                           "measures the platform's motion with it.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; calibrate once it has stopped.")
        if interactive.status()["active"]:
            raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
        if self.active():
            raise ApiError("A calibration is already running; stop it first.")
        if fresh_pose(platform, time.time()) is None:
            raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)."
                           % shown(platform))

    def start_table(self, body, requester):
        """Start a table slope calibration.

        Args:
            body: platform, and optional nx, ny (grid), drift [s],
                margin [m], max_speed [m/s], model ("auto",
                "quadratic", "plane").
            requester: Who asked.

        Raises:
            ApiError: for a bad setting, or if it can't start now.
        """
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

    def load_table(self, body, requester):
        """Set the lab's table calibration from a saved run.

        Its drifts are fitted again, with body's model ("auto",
        "quadratic" or "plane"). The run's data isn't changed.

        Raises:
            ApiError: if a calibration runs, or the run is missing or
                can't be used.
        """
        if self.active():
            raise ApiError("A calibration is running; wait for it to finish.")

        # The saved run, and the model.
        name = str(body.get("run") or "")
        match = TABLE_RUN_RE.match(name)
        folder = RUNS_DIR / name
        if not match or not (folder / "run.json").is_file():
            raise ApiError("The lab console has no saved table run %s."
                           % (name or "(none chosen)"))
        model = str(body.get("model") or "auto")
        if model not in ("auto", "quadratic", "plane"):
            raise ApiError("model must be auto, quadratic or plane.")

        # Fit its drifts again.
        try:
            data = json.loads((folder / "run.json").read_text(encoding="utf-8"))
            fit = fit_drifts(data.get("drifts"), data.get("still"),
                             None if model == "auto" else model)
            saved = data.get("params")
            params = dict(saved if isinstance(saved, dict) else {}, model=model)
        except Abort as e:
            raise ApiError("Run %s can't be used: %s." % (name, e))
        except (OSError, ValueError, AttributeError, KeyError, TypeError) as e:
            raise ApiError("Run %s's data can't be read (%s)." % (name, e))

        # Keep it as the lab's.
        result = dict(fit, updated=now_iso(), by=requester, platform=match.group(3),
                      run="%s_%s" % (match.group(1), match.group(2)), params=params,
                      folder=str(folder), loaded=True)
        out = store.set_section("table", result)
        hw_log("calibration: table slope from saved run %s (%s, %.2f mrad at the centre), by %s"
               % (name, fit["model"], fit["tilt_centre_mrad"], requester))
        return out

    def load_thrusters(self, body, requester):
        """Set a platform's thruster calibration from a saved run.

        Each burn is fitted again from its frames, with the run's mass
        properties and the lab's table calibration now. The run's data
        isn't changed.

        Raises:
            ApiError: if a calibration runs, the run is missing or
                can't be read, or no burn can be fitted.
        """
        if self.active():
            raise ApiError("A calibration is running; wait for it to finish.")

        # The saved run.
        name = str(body.get("run") or "")
        match = RUN_RE["thrusters"].match(name)
        folder = RUNS_DIR / name
        if not match or not (folder / "run.json").is_file():
            raise ApiError("The lab console has no saved thruster run %s."
                           % (name or "(none chosen)"))
        platform = match.group(3)

        # Fit each burn again.
        try:
            data = json.loads((folder / "run.json").read_text(encoding="utf-8"))
            burns = usable_burns(data.get("burns"))
            params = data["params"]
            run = Run("thrusters", platform, params, requester)
            run.stamp = "%s_%s" % (match.group(1), match.group(2))
            refit = ThrusterRun(self, run)
            for b in burns:
                b["fit"], b["error"] = refit.fit(b["thruster"], b["frames"], b["t_on"],
                                                 b["t_off"])
            result = refit.result(burns)
        except (OSError, ValueError, AttributeError, KeyError, TypeError) as e:
            raise ApiError("Run %s's data can't be read (%s)." % (name, e))
        if not any(result["valid"]):
            why = next((b["error"] for b in burns if b.get("error")), "it has no burns")
            raise ApiError("Run %s has no burn that can be fitted (%s); the lab's calibration "
                           "was not changed." % (name, why))

        # Keep it as the lab's, with the platform's decay.
        result.update(folder=str(folder), loaded=True)
        out = store.set_section("thrusters", keep_decay(result, platform), platform)
        good = sum(1 for b in burns if b["fit"])
        hw_log("calibration: %s thrusters from saved run %s (%d of %d burns fitted), by %s"
               % (shown(platform), name, good, len(burns), requester))
        return out

    def load_decay(self, body, requester):
        """Set a platform's thrust decay from a saved Together run.

        Each firing is fitted again from its frames (the run's mass
        properties and single thrusts, the lab's table calibration
        now), and kept with the platform's thruster calibration. The
        run's data isn't changed.

        Raises:
            ApiError: if a calibration runs, the run is missing or
                can't be used, or the platform has no thruster
                calibration.
        """
        if self.active():
            raise ApiError("A calibration is running; wait for it to finish.")

        # The saved run, and the thruster calibration it goes with.
        name = str(body.get("run") or "")
        match = RUN_RE["decay"].match(name)
        folder = RUNS_DIR / name
        if not match or not (folder / "run.json").is_file():
            raise ApiError("The lab console has no saved Together run %s."
                           % (name or "(none chosen)"))
        platform = match.group(3)
        single = (store.get()["calibration"].get("thrusters") or {}).get(platform)
        if not single:
            raise ApiError("The %s has no thruster calibration for the decay to go with."
                           % shown(platform))

        # Fit each firing again.
        try:
            data = json.loads((folder / "run.json").read_text(encoding="utf-8"))
            burns = usable_burns(data.get("burns"))
            run = Run("decay", platform, data["params"], requester)
            run.stamp = "%s_%s" % (match.group(1), match.group(2))
            refit = DecayRun(self, run)
            for b in burns:
                b["fit"], b["error"] = refit.fit(b["thruster"], b["frames"], b["t_on"],
                                                 b["t_off"])
            result = refit.result(burns)
        except Abort as e:
            raise ApiError("Run %s can't be used: %s." % (name, e))
        except (OSError, ValueError, AttributeError, KeyError, TypeError) as e:
            raise ApiError("Run %s's data can't be read (%s)." % (name, e))

        # Keep it with the thruster calibration.
        result.update(folder=str(folder), loaded=True)
        out = store.set_section("thrusters", dict(single, decay=result), platform)
        hw_log("calibration: %s thrust decay from saved run %s (%.1f %% per valve), by %s"
               % (shown(platform), name, 100 * result["slope"], requester))
        return out

    def start_thrusters(self, body, requester):
        """Start a thruster calibration (body: see _thruster_params)."""
        platform = parse_platform(body.get("platform"))
        run = self._thruster_params(platform, body, requester, "thrusters")
        self._begin(run, ThrusterRun(self, run).main)
        return self.status()

    def _thruster_params(self, platform, body, requester, kind):
        """Return a thruster run, checked but not started.

        Args:
            platform: The platform's key.
            body: acknowledged (must be true: the mass properties are
                right), assumed ({mass, inertia, cg_mm}; default the
                controller's), repeats, burn [s], margin [m].
            requester: Who asked.
            kind: "thrusters" or "decay".

        Raises:
            ApiError: if not acknowledged, a value is out of range, or
                the platform can't be calibrated now.
        """
        if body.get("acknowledged") is not True:
            raise ApiError("Confirm first that the platform's mass, inertia and centre of "
                           "gravity are right: the thrust is worked out from them.")

        # The mass properties assumed, and whether they differ from the
        # defaults.
        defaults = assumed_defaults()[platform]
        given = body.get("assumed") or {}
        cg = given.get("cg_mm", defaults["cg_mm"])
        if not isinstance(cg, list) or len(cg) != 2:
            raise ApiError("cg_mm must be [x, y] in mm.")
        assumed = {"mass": number(given.get("mass", defaults["mass"]), "Mass", 1, 50),
                   "inertia": number(given.get("inertia", defaults["inertia"]), "Inertia",
                                     0.01, 5),
                   "cg_mm": [number(cg[0], "CG x", -60, 60), number(cg[1], "CG y", -60, 60)]}
        assumed["dist2CG"] = dist_from_cg(assumed["cg_mm"])
        assumed["changed"] = (
            any(abs(assumed[k] - defaults[k]) > 1e-9 for k in ("mass", "inertia"))
            or any(abs(u - v) > 1e-9 for u, v in zip(assumed["cg_mm"], defaults["cg_mm"])))

        params = {
            "repeats": int(number(body.get("repeats", 2), "Repeats", 1, 3)),
            "burn": number(body.get("burn", 2.0), "Burn time", 0.5, 3.0),
            "margin": number(body.get("margin", 0.35), "Edge margin", 0.25, 0.8),
            "assumed": assumed,
        }
        self._check_free(platform)
        return Run(kind, platform, params, requester)

    def start_decay(self, body, requester):
        """Start the second stage (DecayRun) on a calibrated platform.

        body as for start_thrusters (one repeat by default).

        Raises:
            ApiError: if the platform has no thruster calibration.
        """
        platform = parse_platform(body.get("platform"))
        single = (store.get()["calibration"].get("thrusters") or {}).get(platform)
        if not single or not any(single.get("valid") or []):
            raise ApiError("Calibrate the %s's thrusters one at a time first: the combined "
                           "firings are measured against what each gives alone."
                           % shown(platform))
        body = dict(body, repeats=body.get("repeats", 1))
        run = self._thruster_params(platform, body, requester, "decay")
        run.params["single"] = {"thrust": list(single["thrust"]), "run": single.get("run")}
        self._begin(run, DecayRun(self, run).main)
        return self.status()

    def start_arm(self, body, requester):
        """Start the arm calibration on the Chaser.

        Args:
            body: joints (some of 1, 2, 3), tests (some of ARM_TESTS),
                simulate (a rehearsal with simulated servos), and
                acknowledged (must be true for a real run: the arm is
                clear to move).
            requester: Who asked.

        Raises:
            ApiError: for bad joints or tests, if not acknowledged, or
                if an experiment, Interactive Mode or a calibration
                runs.
        """
        # Joints and tests: non-empty lists of known ones, no repeats.
        joints = body.get("joints", [1, 2, 3])
        tests = body.get("tests", list(ARM_TESTS))
        if (not isinstance(joints, list) or not joints or any(j not in (1, 2, 3) for j in joints)
                or len(set(joints)) != len(joints)):
            raise ApiError("joints must be some of 1 (shoulder), 2 (elbow), 3 (wrist).")
        if (not isinstance(tests, list) or not tests or any(t not in ARM_TESTS for t in tests)
                or len(set(tests)) != len(tests)):
            raise ApiError("tests must be some of %s." % ", ".join(ARM_TESTS))

        # A real run needs the arm clear, and nothing else running.
        simulate = body.get("simulate") is True
        if not simulate and body.get("acknowledged") is not True:
            raise ApiError("Confirm first that the Chaser rests on the table and its arm is "
                           "clear to move.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; calibrate once it has stopped.")
        if interactive.status()["active"]:
            raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
        if self.active():
            raise ApiError("A calibration is already running; stop it first.")

        params = {"joints": sorted(joints), "tests": [t for t in ARM_TESTS if t in tests],
                  "simulate": simulate}
        run = Run("arm", "RED", params, requester)
        self._begin(run, ArmRun(self, run).main)
        return self.status()

    def arm_check(self):
        """Return what the Chaser has for the arm calibration."""
        return arm_check()

    def _begin(self, run, main):
        """Make run the current one and run main in the background.

        Raises:
            ApiError: if another run is under way.
        """
        with self._lock:
            if self.run is not None and self.run.state in ("running", "stopping"):
                raise ApiError("A calibration is already running; stop it first.")
            self.run = run
        threading.Thread(target=main, daemon=True).start()
        self.publish()

    def stop(self, requester, reason=None):
        """Stop the run under way: everything off on its platform."""
        with self._lock:
            run = self.run
            if run is None or run.state not in ("running", "stopping"):
                return self.status_unlocked()
            run.state = "stopping"
            run.stop_reason = reason or "stopped by %s" % requester
        hardware.get(run.platform).release(run, "Calibration stopped (%s); all off"
                                           % run.stop_reason)
        self.publish()
        return self.status()

    def status_unlocked(self):
        """Return the status (nothing running) without the lock."""
        return {"active": False, "run": self.run.status() if self.run else None}

    def shutdown(self):
        """Stop the run (the console is shutting down)."""
        self.stop("the console", "the console is shutting down")


calibrator = Calibrator()


def parse_platform(name):
    """Return a platform key from a request.

    Raises:
        ApiError: if it isn't RED, BLACK or BLUE.
    """
    name = str(name or "").strip().upper()
    if name not in PLATFORMS:
        raise ApiError("Choose the platform: RED, BLACK or BLUE.")
    return name


def number(value, what, low, high):
    """Return a request value as a float within [low, high].

    Raises:
        ApiError: if it isn't a finite number within the range.
    """
    try:
        v = float(value)
    except (TypeError, ValueError):
        raise ApiError("%s must be a number." % what)
    if not (math.isfinite(v) and low <= v <= high):
        raise ApiError("%s must be %g to %g." % (what, low, high))
    return v


def thin(points, n):
    """Return at most n of points, evenly spread, the last one kept."""
    if len(points) <= n:
        return list(points)
    step = len(points) / float(n)
    out = [points[int(i * step)] for i in range(n)]
    out[-1] = points[-1]
    return out


# Driving a platform


class Driver:
    """What every calibration does with its platform.

    Records PhaseSpace, floats it, flies it to a point with the
    diagram's controller, and stops when it must. Every wait goes
    through check(), which keeps the platform's packets going and
    raises Abort when the run must stop.
    """

    def __init__(self, cal, run):
        """Set up the driver for a run.

        Args:
            cal: The Calibrator (for publishing).
            run: The Run.
        """
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

    # Reporting

    def say(self, message, log=True):
        """Show a message on the page (and log it, unless told not)."""
        self.run.message = message
        if log:
            self.run.log.append("%s  %s" % (time.strftime("%H:%M:%S"), message))
            hw_log("%s: calibration: %s" % (shown(self.name), message))
        self.cal.publish(force=False)

    def begin_step(self, index, note=None):
        """Start step index; the running one (if any) is done."""
        run = self.run
        if run.step is not None and run.steps[run.step]["state"] == "running":
            run.steps[run.step]["state"] = "done"
        run.step = index
        run.steps[index]["state"] = "running"
        if note:
            run.steps[index]["note"] = note
        self.cal.publish()

    def end_step(self, state="done", note=None):
        """End the current step (done, skipped or failed)."""
        run = self.run
        if run.step is not None:
            run.steps[run.step]["state"] = state
            if note:
                run.steps[run.step]["note"] = note
        self.cal.publish()

    # Recording every PhaseSpace frame

    def _on_frame(self, frame):
        """Record a frame of the platform while recording.

        Frames whose time goes back (a second stream on the monitor's
        port?) are counted, not recorded.
        """
        rec = self.run.recording
        craft = frame["crafts"].get(self.name)
        if (rec is None or not craft or not craft["tracked"] or craft["x"] is None
                or craft["y"] is None or craft["att"] is None):
            return
        t = frame["t"] if frame.get("t") is not None else frame["received"]
        if rec and t <= rec[-1][0]:
            self.backwards += 1
            return
        rec.append([t, craft["x"], craft["y"], craft["att"], frame["received"]])

    def start_recording(self):
        """Start recording every frame (and a new path for the page)."""
        self.run.recording = []
        self.run.path = []
        monitor.add_listener(self._on_frame)

    def stop_recording(self):
        """Stop recording; return [[t, x, y, theta, received]]."""
        monitor.remove_listener(self._on_frame)
        rec, self.run.recording = self.run.recording or [], None
        return rec

    # The platform

    def claim(self):
        """Take the platform (pucks on); ApiError if it can't be."""
        self.hw.claim(self.run, self.run.by)
        self.claimed = True
        self.pucks = True

    def release(self, note):
        """Give the platform back (everything off), with a note."""
        if self.claimed:
            self.hw.release(self.run, note)
            self.claimed = False

    def set(self, duty=None, pucks=None):
        """Set the duty cycles [%] and/or pucks (sent next check)."""
        if duty is not None:
            self.duty = list(duty)
        if pucks is not None:
            self.pucks = pucks

    def check(self):
        """Keep the platform going; return its pose (None while lost).

        Raises:
            Abort: if the run is stopping, PhaseSpace lost the platform
                for LOST_TIMEOUT, it is off the table, or it was taken
                back.
        """
        run = self.run
        if run.state != "running":
            raise Abort(run.stop_reason or "stopped")

        # The pose; while lost, thrusters off (until LOST_TIMEOUT).
        now = time.time()
        pose = fresh_pose(self.name, now)
        if pose is None:
            self.lost_since = self.lost_since or now
            if now - self.lost_since > LOST_TIMEOUT:
                raise Abort("PhaseSpace lost the %s for more than %g s"
                            % (shown(self.name), LOST_TIMEOUT))
            self.duty = [0.0] * NUM_THRUSTERS
        else:
            self.lost_since = None
            if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
                raise Abort("PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)"
                            % (shown(self.name), pose[0], pose[1]))

            # For the page: the pose, and the path while recording
            # (points 3 mm apart).
            run.pose = [pose[0], pose[1], math.degrees(pose[2])]
            if run.recording is not None:
                if not run.path or math.hypot(pose[0] - run.path[-1][0],
                                              pose[1] - run.path[-1][1]) > 0.003:
                    run.path.append([round(pose[0], 4), round(pose[1], 4)])

        # Send the packet; taken back: stop.
        if self.claimed and not self.hw.drive(run, self.duty, self.pucks):
            last = self.hw.status()["last_action"]
            raise Abort("the %s was taken back (%s)"
                        % (shown(self.name), last["text"] if last else "switched off"))
        self.cal.publish(force=False)
        return pose

    def wait(self, seconds, until=None):
        """Keep the platform going for some seconds.

        Args:
            seconds: How long.
            until: Optional until(pose); stop early when it is true.

        Returns:
            The last pose.
        """
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
        """Return the other platforms seen: {NAME: [x, y]}."""
        now, out = time.time(), {}
        for name in PLATFORMS:
            if name != self.name:
                pose = fresh_pose(name, now)
                if pose is not None:
                    out[name] = pose[:2]
        return out

    def path_clear(self, a, b):
        """Return which platform a straight path a-b passes, or None.

        A platform within CLEARANCE of the path blocks it.
        """
        for name, pos in self.others().items():
            if segment_distance(pos, a, b) < CLEARANCE:
                return name
        return None

    def fly_to(self, goal):
        """Fly the platform (pucks on) to a goal, as the diagram does.

        The goal moves there gradually, at GOAL_SPEED and GOAL_TURN.

        Args:
            goal: [x, y, theta] [m, m, rad].

        Returns:
            The pose once it has settled there (SETTLED_* for
            SETTLED_TIME).

        Raises:
            Abort: if PhaseSpace doesn't see it, or it doesn't settle
                within FLY_TIMEOUT.
        """
        run = self.run

        # The controller process, started on the first flight.
        if run.controller is None:
            run.controller = ControllerProcess(self.name, self.controller_params())
            run.controller.wait_ready()
        pose = self.check()
        if pose is None:
            raise Abort("PhaseSpace doesn't see the %s" % shown(self.name))

        # The target (the shorter way round), and the moving goal.
        target = [goal[0], goal[1], pose[2] + wrap(goal[2] - pose[2])]
        current = [pose[0], pose[1], pose[2]]
        run.goal = [goal[0], goal[1], math.degrees(goal[2])]
        self.pucks = True
        started, settled_since = time.monotonic(), None
        next_step = time.monotonic()
        while True:
            # The goal moves towards the target.
            dist = math.hypot(target[0] - current[0], target[1] - current[1])
            step = GOAL_SPEED * self.period
            if dist > step:
                current[0] += (target[0] - current[0]) * step / dist
                current[1] += (target[1] - current[1]) * step / dist
            else:
                current[0], current[1] = target[0], target[1]
            dth, turn = target[2] - current[2], GOAL_TURN * self.period
            current[2] = target[2] if abs(dth) <= turn else current[2] + math.copysign(turn, dth)

            # One control step.
            pose = fresh_pose(self.name, time.time())
            if pose is not None:
                _u, duty, _b = run.controller.step(pose, list(current))
                self.duty = [100 * v for v in duty]

            # Settled at the target: close, slow, for SETTLED_TIME.
            pose = self.check()
            if pose is not None and current == target:
                close = (math.hypot(pose[0] - target[0], pose[1] - target[1]) < SETTLED_POS
                         and abs(wrap(pose[2] - target[2])) < SETTLED_ATT
                         and math.hypot(pose[3], pose[4]) < SETTLED_SPEED
                         and abs(pose[5]) < math.radians(2))
                if close:
                    settled_since = settled_since or time.monotonic()
                    if time.monotonic() - settled_since >= SETTLED_TIME:
                        self.duty = [0.0] * NUM_THRUSTERS
                        run.goal = None
                        return pose
                else:
                    settled_since = None

            # Too long: say how far off it is.
            if time.monotonic() - started > FLY_TIMEOUT:
                why = ""
                if pose is not None:
                    why = (" (it is %.0f mm and %.1f deg from it, moving at %.0f mm/s and "
                           "%.1f deg/s)" % (
                               1000 * math.hypot(pose[0] - target[0], pose[1] - target[1]),
                               math.degrees(abs(wrap(pose[2] - target[2]))),
                               1000 * math.hypot(pose[3], pose[4]),
                               math.degrees(abs(pose[5]))))
                raise Abort("the %s didn't settle at (%.2f m, %.2f m) within %g s%s"
                            % (shown(self.name), goal[0], goal[1], FLY_TIMEOUT, why))

            # The next step, at the controller's rate.
            next_step += self.period
            delay = next_step - time.monotonic()
            if delay > 0:
                time.sleep(delay)
            else:
                next_step = time.monotonic()

    def controller_params(self):
        """Return the controller's parameters (calibrated thrust)."""
        return controller_params(self.name)

    def finish(self, ok, note):
        """End driving: stop recording, release, end controller."""
        run = self.run
        if run.recording is not None:
            self.stop_recording()
        self.release(note)
        if run.controller is not None:
            run.controller.close()
            run.controller = None
        run.goal = None


def segment_distance(p, a, b):
    """Return the distance from point p to the segment a-b."""
    ax, ay, bx, by = a[0], a[1], b[0], b[1]
    dx, dy = bx - ax, by - ay
    length_sq = dx * dx + dy * dy

    # The nearest point's fraction along the segment, in [0, 1].
    frac = 0.0 if length_sq == 0 else max(0.0, min(1.0, ((p[0] - ax) * dx + (p[1] - ay) * dy)
                                                   / length_sq))
    return math.hypot(p[0] - (ax + frac * dx), p[1] - (ay + frac * dy))


def save_run(run, data):
    """Keep a run's raw data on the lab console.

    In ~/SPOTCalibration/<stamp>_<kind>_<platform>/run.json.

    Returns:
        The folder, or None if it couldn't be saved (logged).
    """
    try:
        folder = RUNS_DIR / ("%s_%s_%s" % (run.stamp, run.kind, run.platform))
        folder.mkdir(parents=True, exist_ok=True)
        (folder / "run.json").write_text(json.dumps(data, allow_nan=False) + "\n",
                                         encoding="utf-8")
        return str(folder)
    except (OSError, ValueError) as e:
        hw_log("calibration: could not save the run's data (%s)" % e)
        return None


# The table's slope


def grid_points(nx, ny, margin):
    """Return the grid of start points, in a snake.

    nx x ny points, spread over the table inside margin + 0.35 m.
    """
    inset = margin + 0.35
    xs = [TABLE[0] / 2] if nx == 1 else [inset + (TABLE[0] - 2 * inset) * i / (nx - 1)
                                         for i in range(nx)]
    ys = [TABLE[1] / 2] if ny == 1 else [inset + (TABLE[1] - 2 * inset) * j / (ny - 1)
                                         for j in range(ny)]

    # Alternate rows reversed, so each point is next to the last.
    out = []
    for j, y in enumerate(ys):
        row = xs if j % 2 == 0 else list(reversed(xs))
        out.extend([x, y] for x in row)
    return out


# A drift needs this many frames to be fitted (calfit.fit_table's
# min_samples).
MIN_DRIFT_SAMPLES = 20

# A saved run's folder (save_run): <stamp>_<kind>_<platform>.
RUN_RE = {kind: re.compile(r"^(\d{8})_(\d{6})_%s_(RED|BLACK|BLUE)$" % kind)
          for kind in ("table", "thrusters", "decay")}
TABLE_RUN_RE = RUN_RE["table"]


class Rejected(Abort):
    """The drifts can't give a calibration.

    Attributes:
        data: What the run's saved data records about it.
    """

    def __init__(self, message, data):
        """Keep the message and the data."""
        super().__init__(message)
        self.data = data


def usable_drifts(drifts):
    """Return the drifts long enough to fit."""
    return [x for x in drifts or [] if isinstance(x, dict) and isinstance(x.get("t"), list)
            and len(x["t"]) >= MIN_DRIFT_SAMPLES]


def fit_drifts(drifts, still, model=None, backwards=0):
    """Fit the table's slope to drifts.

    Args:
        drifts: TableRun.drift's records, from a run just made or a
            saved one.
        still: The resting frames, for the cameras' noise.
        model: "quadratic", "plane" or None (calfit chooses).
        backwards: The frames whose time went back while recording.

    Returns:
        calfit.fit_table's fit, with noise_mm, paths (thinned, for the
        page) and a warning if it fits poorly.

    Raises:
        Abort: if no drift is long enough.
        Rejected: if PhaseSpace's data or the fit is too poor to use.
    """
    records = usable_drifts(drifts)
    if not records:
        raise Abort("no drift was long enough to fit")

    # PhaseSpace's data must be consistent.
    samples = sum(len(x["t"]) for x in records)
    jumps = sum(x.get("glitches", 0) for x in records)
    if jumps + backwards > GLITCH_FRACTION * samples:
        raise Rejected("PhaseSpace's data isn't consistent: %d of %d positions jumped, and %d "
                       "frames went back in time (mis-tracking, or a second stream on the "
                       "console's port?); the lab's calibration was not changed"
                       % (jumps, samples, backwards), {"rejected": "glitches"})

    # Fit, and judge the residual against the cameras' noise.
    fit = calfit.fit_table(records, model)
    noise = noise_mm(still)
    if fit["rms_mm"] > REJECT_RESIDUAL:
        raise Rejected("the drifts don't follow the model at all (position residual %.1f mm): "
                       "something else moved the platform; the lab's calibration was not "
                       "changed" % fit["rms_mm"], {"rejected_fit": fit})
    if fit["rms_mm"] > max(POOR_FIT_FLOOR, POOR_FIT_FACTOR * (noise or 0.2)):
        fit["warning"] = ("The drifts follow the %s only to %.1f mm (the cameras' noise is "
                          "%.2f mm): the table isn't that simple here.%s"
                          % (fit["model"], fit["rms_mm"], noise or 0.0,
                             " A larger grid (3 x 2 or more) fits a surface."
                             if fit["model"] == "plane" else ""))
    return dict(fit, noise_mm=noise,
                paths=[{"platform": x["platform"], "cut": x.get("cut"),
                        "points": [[round(a, 4), round(b, 4)]
                                   for a, b in thin(list(zip(x["x"], x["y"])), 150)]}
                       for x in records])


def usable_burns(burns):
    """Return the saved burns that can be fitted again.

    ThrusterRun.burn's records, each with its frames and times.
    """
    return [b for b in burns or [] if isinstance(b, dict) and isinstance(b.get("frames"), list)
            and b["frames"] and b.get("thruster") in range(1, NUM_THRUSTERS + 1)
            and isinstance(b.get("t_on"), (int, float))
            and isinstance(b.get("t_off"), (int, float))
            and isinstance(b.get("start"), list)]


def saved_runs(kind):
    """Return the saved runs of a kind with data to fit, newest first.

    What Calibrator.load_table / load_thrusters / load_decay can make
    the lab's calibration again.

    Args:
        kind: "table", "thrusters" or "decay".

    Returns:
        [{"id", "platform", "started", "count", "seconds", "stopped",
        "note", "current"}].
    """
    cal = store.get()["calibration"]
    out = []
    folders = sorted(RUNS_DIR.iterdir(), reverse=True) if RUNS_DIR.is_dir() else []
    for folder in folders:
        match = RUN_RE[kind].match(folder.name)
        if not match:
            continue
        platform = match.group(3)

        # Its usable drifts or burns (and the drifts' total time).
        try:
            data = json.loads((folder / "run.json").read_text(encoding="utf-8"))
            if kind == "table":
                parts = usable_drifts(data.get("drifts"))
                seconds = sum(x["t"][-1] - x["t"][0] for x in parts)
            else:
                parts = usable_burns(data.get("burns"))
                seconds = None
        except (OSError, ValueError, AttributeError, TypeError):
            continue
        if not parts:
            continue

        # Whether it is the lab's calibration now.
        current = (cal.get("table") if kind == "table"
                   else (cal.get("thrusters") or {}).get(platform)) or {}
        if kind == "decay":
            current = current.get("decay") or {}

        day, hms = match.group(1), match.group(2)
        out.append({"id": folder.name, "platform": platform,
                    "started": "%s-%s-%s %s:%s" % (day[:4], day[4:6], day[6:], hms[:2], hms[2:4]),
                    "count": len(parts), "seconds": None if seconds is None else round(seconds, 1),
                    "stopped": bool(data.get("aborted")), "note": data.get("aborted"),
                    "current": current.get("folder") == str(folder)})
    return out


class TableRun:
    """A table slope calibration (see the module's help)."""

    def __init__(self, cal, run):
        """Set up the run with its Driver."""
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)

    def main(self):
        """Run it: still, float here, each grid point, fit and save."""
        run, drv, params = self.run, self.d, self.run.params
        points = grid_points(params["nx"], params["ny"], params["margin"])
        run.steps = ([{"name": "Still", "state": "waiting", "note": "Noise while resting"},
                      {"name": "Float Here", "state": "waiting",
                       "note": "Drift from where it is"}]
                     + [{"name": "Point %d" % (i + 1), "state": "waiting",
                         "note": "(%.2f, %.2f) m" % tuple(pt)}
                        for i, pt in enumerate(points)]
                     + [{"name": "Fit", "state": "waiting", "note": ""}])
        run.points = points
        drifts, still = [], None
        ok, note = False, None
        try:
            # 1. Resting: the cameras' noise.
            drv.begin_step(0)
            drv.say("Recording the %s resting on the table (2 s)." % shown(run.platform))
            drv.start_recording()
            drv.wait(2.0)
            still = drv.stop_recording()
            noise = noise_mm(still)
            drv.end_step(note="%.2f mm" % noise if noise is not None else "no data")

            # 2. Float it, and let it drift from where it is.
            drv.begin_step(1)
            drv.claim()
            pose = drv.check()
            if pose is not None and self.inside(pose, extra=0.1):
                drifts.append(self.drift(1))
            else:
                drv.end_step("skipped", "too close to an edge or a platform")

            # 3. Each point of the grid: fly there (unless another
            # platform is in the way), rest, drift.
            for i, pt in enumerate(points):
                drv.begin_step(2 + i)
                pose = drv.check()
                if pose is None:
                    raise Abort("PhaseSpace doesn't see the %s" % shown(run.platform))
                blocker = drv.path_clear(pose[:2], pt)
                if blocker:
                    drv.say("Point %d skipped: the path passes the %s." % (i + 1, shown(blocker)))
                    drv.end_step("skipped", "path passes the %s" % shown(blocker))
                    continue
                drv.say("Flying to point %d (%.2f m, %.2f m)." % (i + 1, pt[0], pt[1]))
                drv.fly_to([pt[0], pt[1], pose[2]])
                drv.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
                drv.say("At point %d: pucks off, resting (%g s)." % (i + 1, STOP_PAUSE))
                drv.wait(STOP_PAUSE)
                drifts.append(self.drift(2 + i))
            drv.finish(True, "Calibration finished; all off")

            # 4. Fit (a rejected fit's data is still saved).
            drv.begin_step(len(run.steps) - 1)
            drv.say("Fitting the table's slope to %d drifts." % len(drifts))
            try:
                fit = fit_drifts(drifts, still,
                                 None if params["model"] == "auto" else params["model"],
                                 drv.backwards)
            except Rejected as e:
                save_run(run, dict({"kind": "table", "platform": run.platform, "params": params,
                                    "still": still, "drifts": drifts}, **e.data))
                raise

            # Save the run and make the fit the lab's.
            result = dict(fit, updated=now_iso(), by=run.by, platform=run.platform,
                          run=run.stamp, params=params)
            folder = save_run(run, {"kind": "table", "platform": run.platform, "params": params,
                                    "still": still, "drifts": drifts, "fit": result})
            result["folder"] = folder
            store.set_section("table", result)
            run.result = result
            drv.end_step(note="%s, %.2f mrad at the centre"
                              % (fit["model"], fit["tilt_centre_mrad"]))
            drv.say("Done: the table tilts %.2f mrad at its centre (%.2f mrad at most), downhill "
                    "towards %.0f deg; position residual %.2f mm.%s"
                    % (fit["tilt_centre_mrad"], fit["tilt_max_mrad"], fit["downhill_deg"],
                       fit["rms_mm"], " " + fit["warning"] if fit.get("warning") else ""))
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:
            # Never leave the platform floating after a bug.
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            drv.finish(ok, "Calibration %s; all off"
                       % ("finished" if ok else "stopped (%s)" % note))
        self.done(ok, note, drifts, still)

    def inside(self, pose, extra=0.0):
        """Return whether it is clear of the edges and the others.

        The edges: the run's margin, plus extra.
        """
        margin = self.run.params["margin"] + extra
        if not (margin <= pose[0] <= TABLE[0] - margin
                and margin <= pose[1] <= TABLE[1] - margin):
            return False
        return all(math.hypot(pose[0] - o[0], pose[1] - o[1]) >= CLEARANCE
                   for o in self.d.others().values())

    def drift(self, step):
        """Record the platform drifting, thrusters off, pucks on.

        Once the pucks have lifted it, record until the drift time is
        up, or it nears an edge or another platform or goes too fast
        (then the pucks go off at once, and it stops). The same checks
        run while the pucks lift it.

        Returns:
            The drift's record: platform, t, x, y, theta, cut (why it
            was cut short, or None), glitches.
        """
        run, drv, params = self.run, self.d, self.run.params
        why = []

        def stop(pose):
            """Return whether the drift must stop now (record why)."""
            if not self.inside(pose):
                why.append("near an edge or a platform")
                return True
            if math.hypot(pose[3], pose[4]) > params["max_speed"]:
                why.append("faster than %g m/s" % params["max_speed"])
                return True
            return False

        # Lift, then record.
        drv.set(duty=[0.0] * NUM_THRUSTERS, pucks=True)
        drv.say("Pucks on, lifting (%g s)." % DRIFT_LIFT)
        drv.wait(DRIFT_LIFT, until=stop)
        drv.start_recording()
        if not why:
            drv.say("Drifting (up to %g s)." % params["drift"])
            drv.wait(params["drift"], until=stop)
        rec = drv.stop_recording()

        # Cut short: pucks off until it rests, then on again.
        cut = why[0] if why else None
        if cut:
            drv.set(pucks=False)
            drv.say("Drift cut short (%s): pucks off." % cut)
            drv.wait(STOP_PAUSE)
            drv.set(pucks=True)

        # How far and long, and the jumps between frames.
        moved = math.hypot(rec[-1][1] - rec[0][1], rec[-1][2] - rec[0][2]) if len(rec) > 1 else 0.0
        secs = rec[-1][0] - rec[0][0] if len(rec) > 1 else 0.0
        note = "%.1f s, %.0f mm%s" % (secs, 1000 * moved, " (cut: %s)" % cut if cut else "")
        drv.end_step(note=note)
        glitches = sum(1 for a, b in zip(rec, rec[1:])
                       if math.hypot(b[1] - a[1], b[2] - a[2]) > GLITCH_STEP)

        record = {"platform": run.platform, "t": [s[0] for s in rec], "x": [s[1] for s in rec],
                  "y": [s[2] for s in rec], "theta": [s[3] for s in rec], "cut": cut,
                  "glitches": glitches}
        run.records.append({"points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 150)],
                            "cut": cut})
        return record

    def done(self, ok, note, drifts, still):
        """Record how the run ended; keep a stopped run's drifts."""
        run = self.run
        if not ok:
            if run.step is not None and run.steps[run.step]["state"] == "running":
                run.steps[run.step]["state"] = "failed"
            run.error = note if run.state == "running" else None
            stopped = run.state == "stopping"
            run.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.")
                       % (run.stop_reason if stopped else note))
            if drifts:
                save_run(run, {"kind": "table", "platform": run.platform, "params": run.params,
                               "still": still, "drifts": drifts, "aborted": note})
        else:
            run.state = "done"
        run.finished = time.time()
        self.cal.publish()


# The thrusters

# [m] the thrusters sit this far in from the body's edges
# (spot.massprops.centerOfGravity).
THRUSTER_INSET = 0.081
# [s] from the command to the thruster pushing (SSH, the Teensy, the
# valve): skipped.
BURN_LATENCY = 0.15
# [s] pucks off before each burn, at the table's centre: the platform
# comes to rest.
REST_TIME = 5.0
# [mm] a burn whose motion doesn't follow a constant force better than
# this is left out.
BURN_RMS_LIMIT = 3.0
# A burn needs this many frames to fit its 6 unknowns (2 s at the 10 Hz
# data rate gives ~18).
MIN_BURN_FRAMES = 10
HALF_SIDE = 0.15             # [m] body centre to edge
MIN_FIT_THRUST = 0.02        # [N] a fitted thrust outside this range is unlikely
MAX_FIT_THRUST = 1.5         # [N]
START_CLEARANCE = CLEARANCE + 0.15   # [m] others from the centre, where burns start


def dist_from_cg(cg_mm):
    """Return the mixer's thruster_dist2CG [mm] for a centre of gravity.

    As spot.massprops.centerOfGravity makes them.

    Args:
        cg_mm: [x, y] [mm] from the body's centre.
    """
    e = HALF_SIDE - THRUSTER_INSET
    X, Y = cg_mm[0] / 1000.0, cg_mm[1] / 1000.0
    return [1000 * v for v in (e - Y, -(e + Y), e - X, -(e + X), e + Y, -(e - Y), e + X, -(e - X))]


def assumed_defaults():
    """Return each platform's mass properties as the console takes them.

    The controller's defaults (control.DEFAULTS: SPOT's default
    settings), the CG worked back from its thruster offsets.

    Returns:
        {NAME: {"mass", "inertia", "thrust", "thrust_decay", "cg_mm"}}.
    """
    e = HALF_SIDE - THRUSTER_INSET
    out = {}
    for name in PLATFORMS:
        plat = DEFAULTS["platforms"][name]
        dist = plat["thruster_dist2CG"]
        out[name] = {"mass": plat["mass"], "inertia": plat["inertia"], "thrust": plat["thrust"],
                     "thrust_decay": plat["thrust_decay"],
                     "cg_mm": [round(1000 * e - dist[2], 3), round(1000 * e - dist[0], 3)]}
    return out


def controller_params(platform):
    """Return the controller's parameters for a platform.

    SPOT's defaults, with the lab's calibrated thrust of each thruster
    and its thrust decay (the Together stage) when the lab has them.
    """
    params = copy.deepcopy(DEFAULTS)
    thrusters_cal = (store.get()["calibration"].get("thrusters") or {}).get(platform)
    if thrusters_cal:
        params["platforms"][platform]["thrust"] = list(thrusters_cal["thrust"])
        decay = thrusters_cal.get("decay") or {}
        if (decay.get("model", "linear") == "linear"
                and isinstance(decay.get("slope"), (int, float))):
            params["platforms"][platform]["thrust_decay"] = decay["slope"]
    return params


class ThrusterRun:
    """A thruster calibration (see the module's help)."""

    def __init__(self, cal, run):
        """Set up the run with its Driver."""
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)

    def main(self):
        """Run it: each thruster's burns, then fit and save."""
        run, drv, params = self.run, self.d, self.run.params
        order = [k for k in range(1, NUM_THRUSTERS + 1)]
        run.steps = ([{"name": "Start", "state": "waiting", "note": "A clear place to start"}]
                     + [{"name": "Thruster %d" % k, "state": "waiting", "note": ""} for k in order]
                     + [{"name": "Fit", "state": "waiting", "note": ""}])
        burns, ok, note = [], False, None
        try:
            # Where every burn starts, and take the platform.
            drv.begin_step(0)
            pose = drv.check()
            if pose is None:
                raise Abort("PhaseSpace doesn't see the %s" % shown(run.platform))
            start = self.start_place(pose)
            drv.claim()
            drv.end_step(note="(%.2f, %.2f) m" % tuple(start[:2]))

            # Each thruster, repeated.
            for k in order:
                drv.begin_step(k)
                notes = []
                for rep in range(params["repeats"]):
                    b = self.burn(k, rep, start)
                    burns.append(b)
                    notes.append("%.3f N" % b["fit"]["thrust"] if b.get("fit") else "no fit")
                    drv.end_step("running", " · ".join(notes))
                drv.end_step(note=" · ".join(notes))
            drv.finish(True, "Calibration finished; all off")

            # Fit: at least one good burn.
            drv.begin_step(len(run.steps) - 1)
            result = self.result(burns)
            if not any(result["valid"]):
                why = next((b["error"] for b in burns if b.get("error")), "no burn was recorded")
                raise Abort("no burn could be fitted (%s); the lab's calibration was not changed"
                            % why)

            # Save the run and make it the lab's (keeping the decay).
            folder = save_run(run, {"kind": "thrusters", "platform": run.platform,
                                    "params": params, "burns": burns, "result": result})
            result["folder"] = folder
            store.set_section("thrusters", keep_decay(result, run.platform), run.platform)
            run.result = result
            bad = [k + 1 for k in range(NUM_THRUSTERS) if not result["valid"][k]]
            drv.end_step("done" if not bad else "failed",
                         "%.3f-%.3f N" % (min(result["thrust"]), max(result["thrust"])))
            drv.say("Done: thrust %s N.%s"
                    % (", ".join("%.3f" % f for f in result["thrust"]),
                       " No good burn of thruster %s: it keeps the nominal thrust."
                       % ", ".join(map(str, bad)) if bad else ""))
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:
            # Never leave the platform floating after a bug.
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            drv.finish(ok, "Calibration %s; all off"
                       % ("finished" if ok else "stopped (%s)" % note))
        self.done(ok, note, burns)

    def start_place(self, pose):
        """Return where burns start: the table's centre, as turned now.

        Raises:
            Abort: if another platform is too close to it.
        """
        x, y = TABLE[0] / 2, TABLE[1] / 2
        if not all(math.hypot(x - o[0], y - o[1]) >= START_CLEARANCE
                   for o in self.d.others().values()):
            raise Abort("another platform is within %.2f m of the table's centre, where every "
                        "burn starts; move it" % (START_CLEARANCE))
        return [x, y, pose[2]]

    def burn(self, k, rep, start, opened=None):
        """Fire one burn and record it.

        Fly to start (the table's centre), pucks off and rest, then
        pucks on and the thruster(s) firing at once.

        Args:
            k: The thruster (or, with opened, their number).
            rep: The repeat, from 0.
            start: [x, y, theta] where it starts.
            opened: The thrusters fired together; default [k].

        Returns:
            The burn's record, with its fit (self.fit).
        """
        run, drv, params = self.run, self.d, self.run.params
        opened = opened or [k]
        what = ("Thruster %d" % k if len(opened) == 1
                else "Thrusters %d-%d" % (opened[0], opened[-1]))
        again = " (repeat %d)" % (rep + 1) if params["repeats"] > 1 else ""

        # To the centre, then rest there.
        drv.say("%s%s: flying to the centre." % (what, again))
        drv.fly_to(start)
        drv.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
        drv.say("%s%s: pucks off, resting (%g s)." % (what, again, REST_TIME))
        here = drv.wait(REST_TIME)
        if here is None:
            raise Abort("PhaseSpace doesn't see the %s" % shown(run.platform))

        # Fire (pucks and thrusters sent together, in the next packet),
        # recording; cut short near an edge.
        drv.say("%s%s: pucks on, firing for %g s." % (what, again, params["burn"]))
        duty = [100.0 if j + 1 in opened else 0.0 for j in range(NUM_THRUSTERS)]
        drv.start_recording()
        drv.set(duty=duty, pucks=True)
        t_on = time.time()
        cut = []

        def stop(pose):
            """Return whether the burn must stop: near an edge."""
            margin = params["margin"]
            if not (margin <= pose[0] <= TABLE[0] - margin
                    and margin <= pose[1] <= TABLE[1] - margin):
                cut.append("near an edge")
                return True
            return False

        drv.wait(params["burn"], until=stop)
        t_off = time.time()
        drv.set(duty=[0.0] * NUM_THRUSTERS)
        drv.wait(0.2)
        rec = drv.stop_recording()
        if cut:
            drv.say("%s: burn cut short (%s)." % (what, cut[0]))

        # The record, for the page and the fit.
        run.records.append({"thruster": k,
                            "points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 80)],
                            "cut": cut[0] if cut else None})
        record = {"thruster": k, "open": opened, "repeat": rep, "t_on": t_on, "t_off": t_off,
                  "cut": cut[0] if cut else None, "frames": rec, "start": here[:3]}
        record["fit"], record["error"] = self.fit(k, rec, t_on, t_off)
        return record

    @staticmethod
    def burn_frames(rec, t_on, t_off):
        """Return the frames while the thrusters pushed, or None."""
        burn = [s for s in rec if t_on + BURN_LATENCY <= s[4] <= t_off]
        if len(burn) < MIN_BURN_FRAMES:
            return None, ("too few frames during the burn (%d; stream PhaseSpace faster or burn "
                          "longer)" % len(burn))
        jumps = sum(1 for u, v in zip(burn, burn[1:])
                    if math.hypot(v[1] - u[1], v[2] - u[2]) > GLITCH_STEP)
        if jumps:
            return None, "PhaseSpace's positions jumped %d times" % jumps
        return burn, None

    def table_pull(self):
        """Return the table's slope and this platform's drag.

        (None, 0) without a table calibration.
        """
        table = store.get()["calibration"].get("table")
        if not table:
            return None, 0.0
        return table["height"], table.get("damping", {}).get(self.run.platform, 0.0)

    def fit(self, k, rec, t_on, t_off):
        """Fit a burn of thruster k.

        Returns:
            (the fit, None), or (None, why not).
        """
        assumed = self.run.params["assumed"]
        burn, why = self.burn_frames(rec, t_on, t_off)
        if burn is None:
            return None, why
        slope, damping = self.table_pull()
        fitted = calfit.fit_burn([s[0] for s in burn], [s[1] for s in burn],
                                 [s[2] for s in burn], [s[3] for s in burn],
                                 assumed["mass"], assumed["inertia"], k,
                                 assumed["dist2CG"][k - 1], slope=slope, damping=damping)

        # A constant thrust, of a likely size.
        if fitted["rms_mm"] > BURN_RMS_LIMIT:
            return None, ("the motion doesn't follow a constant thrust (residual %.1f mm)"
                          % fitted["rms_mm"])
        if not MIN_FIT_THRUST <= fitted["thrust"] <= MAX_FIT_THRUST:
            return None, "an unlikely thrust (%.3f N): did the thruster fire?" % fitted["thrust"]
        return fitted, None

    def result(self, burns):
        """Return the calibration from the burns.

        Each thruster's thrust (the mean of its good burns; the nominal
        one without any), its spread, how far it is turned, its torque
        ratio, and each burn, for the page.
        """
        run, params = self.run, self.run.params
        nominal = assumed_defaults()[run.platform]["thrust"]
        out = {"updated": now_iso(), "by": run.by, "platform": run.platform, "run": run.stamp,
               "params": params, "nominal": nominal,
               "table_used": bool(store.get()["calibration"].get("table")),
               "thrust": [], "thrust_sd": [], "angle": [], "torque_ratio": [], "valid": [],
               "burns": []}

        # Each thruster from its good burns (one burn: no spread).
        for k in range(1, NUM_THRUSTERS + 1):
            fits = [b["fit"] for b in burns if b["thruster"] == k and b["fit"]]
            mean, sd = calfit.summarise([fitted["thrust"] for fitted in fits])
            angle, _ = calfit.summarise([fitted["angle"] for fitted in fits])
            ratios = [fitted["torque_ratio"] for fitted in fits
                      if fitted["torque_ratio"] is not None]
            ratio, _ = calfit.summarise(ratios)
            out["valid"].append(bool(fits))
            out["thrust"].append(mean if fits else nominal)
            out["thrust_sd"].append(sd if len(fits) > 1 else None)
            out["angle"].append(angle)
            out["torque_ratio"].append(ratio)

        # Each burn, its path relative to its start, for the page.
        for b in burns:
            fitted = b["fit"]
            out["burns"].append({
                "thruster": b["thruster"], "repeat": b["repeat"], "cut": b["cut"],
                "error": b["error"], "thrust": fitted and fitted["thrust"],
                "angle": fitted and fitted["angle"],
                "torque_ratio": fitted and fitted["torque_ratio"],
                "rms_mm": fitted and fitted["rms_mm"],
                "points": [[round(s[1] - b["start"][0], 4), round(s[2] - b["start"][1], 4)]
                           for s in thin([s for s in b["frames"] if s[4] >= b["t_on"]], 60)],
                "start_deg": math.degrees(b["start"][2])})
        return out

    def done(self, ok, note, burns):
        """Record how the run ended; keep a stopped run's burns."""
        run = self.run
        if not ok:
            if run.step is not None and run.steps[run.step]["state"] == "running":
                run.steps[run.step]["state"] = "failed"
            stopped = run.state == "stopping"
            run.error = None if stopped else note
            run.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.")
                       % (run.stop_reason if stopped else note))
            if burns:
                save_run(run, {"kind": "thrusters", "platform": run.platform,
                               "params": run.params, "burns": burns, "aborted": note})
        else:
            run.state = "done"
        run.finished = time.time()
        self.cal.publish()


def keep_decay(result, platform):
    """Return a thruster calibration with the current decay."""
    old = (store.get()["calibration"].get("thrusters") or {}).get(platform) or {}
    if old.get("decay"):
        result = dict(result, decay=old["decay"])
    return result


# A firing whose thrusters alone push less than this many single
# thrusts is left out.
DECAY_MIN_PUSH = 0.5


class DecayRun(ThrusterRun):
    """The second stage: thrusters 1, 1-2, ... 1-8 fired together.

    Measured against the platform's thruster calibration.
    """

    def main(self):
        """Run it: each set of thrusters, then fit and save."""
        run, drv, params = self.run, self.d, self.run.params
        sets = [list(range(1, n + 1)) for n in range(1, NUM_THRUSTERS + 1)]
        run.steps = ([{"name": "Start", "state": "waiting", "note": "The table's centre"}]
                     + [{"name": "Thrusters 1" if n == 1 else "Thrusters 1-%d" % n,
                         "state": "waiting", "note": ""}
                        for n in range(1, NUM_THRUSTERS + 1)]
                     + [{"name": "Fit", "state": "waiting", "note": ""}])
        burns, ok, note = [], False, None
        try:
            # Where every firing starts, and take the platform.
            drv.begin_step(0)
            pose = drv.check()
            if pose is None:
                raise Abort("PhaseSpace doesn't see the %s" % shown(run.platform))
            start = self.start_place(pose)
            drv.claim()
            drv.end_step(note="(%.2f, %.2f) m" % tuple(start[:2]))

            # Each set, repeated.
            for n, opened in enumerate(sets, 1):
                drv.begin_step(n)
                notes = []
                for rep in range(params["repeats"]):
                    b = self.burn(n, rep, start, opened)
                    burns.append(b)
                    fitted = b.get("fit")
                    notes.append("%.0f %%" % (100 * fitted["ratio"]) if fitted and fitted["used"]
                                 else "cancels" if fitted else "no fit")
                    drv.end_step("running", " · ".join(notes))
                drv.end_step(note=" · ".join(notes))
            drv.finish(True, "Calibration finished; all off")

            # Fit, save the run, and keep it with the thruster
            # calibration.
            drv.begin_step(len(run.steps) - 1)
            result = self.result(burns)
            folder = save_run(run, {"kind": "decay", "platform": run.platform, "params": params,
                                    "burns": burns, "result": result})
            result["folder"] = folder
            entry = dict((store.get()["calibration"].get("thrusters") or {})[run.platform],
                         decay=result)
            store.set_section("thrusters", entry, run.platform)
            run.result = result
            how = ("1 - %.3f (N - 1)" % result["slope"] if result["model"] == "linear"
                   else "1 / (1 + %.3f (N - 1))" % result["k"])
            drv.end_step(note="%s: %s" % (result["model"], how))
            drv.say("Done: with N thrusters open each gives %s of its thrust alone: %s." % (
                how, ", ".join("%d: -%.0f %%" % (n, v)
                               for n, v in enumerate(result["drop_pct"], 1) if n > 1)))
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:
            # Never leave the platform floating after a bug.
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            drv.finish(ok, "Calibration %s; all off"
                       % ("finished" if ok else "stopped (%s)" % note))
        self.done(ok, note, burns)

    def alone(self):
        """Return each thruster's thrust alone [N]: {1: F1, ..., 8: F8}.

        From the platform's thruster calibration.
        """
        return dict(enumerate(self.run.params["single"]["thrust"], 1))

    def fit(self, n, rec, t_on, t_off):
        """Fit a firing of thrusters 1-n against their pushes alone.

        Returns:
            (the fit, None), or (None, why not). A firing whose
            thrusters alone would push little (they cancel) isn't used
            for the decay.
        """
        assumed = self.run.params["assumed"]
        burn, why = self.burn_frames(rec, t_on, t_off)
        if burn is None:
            return None, why

        # The push it gave: a constant force.
        slope, damping = self.table_pull()
        fitted = calfit.fit_force([s[0] for s in burn], [s[1] for s in burn],
                                  [s[2] for s in burn], [s[3] for s in burn],
                                  assumed["mass"], slope=slope, damping=damping)
        if fitted["rms_mm"] > BURN_RMS_LIMIT:
            return None, ("the motion doesn't follow a constant push (residual %.1f mm)"
                          % fitted["rms_mm"])

        # Against their pushes alone.
        alone = self.alone()
        ratio, ratio_sd, P = calfit.combined_ratio(fitted["Fb"], fitted["sd"],
                                                   {k: alone[k] for k in range(1, n + 1)})
        single = sum(alone.values()) / len(alone)
        used = ratio is not None and math.hypot(*P) >= DECAY_MIN_PUSH * single
        return {"Fb": fitted["Fb"], "push_alone": P, "ratio": ratio, "ratio_sd": ratio_sd,
                "rms_mm": fitted["rms_mm"], "used": used,
                "net_N": math.hypot(*fitted["Fb"])}, None

    def result(self, burns):
        """Return the decay: each firing's ratio, and the fitted curve.

        Raises:
            Abort: with fewer than 3 different firings that pushed.
        """
        run, params = self.run, self.run.params
        rows = [(b["thruster"], b["fit"]["ratio"], b["fit"]["ratio_sd"]) for b in burns
                if b.get("fit") and b["fit"]["used"]]
        if len({n for n, _, _ in rows}) < 3:
            why = next((b["error"] for b in burns if b.get("error")),
                       "too few firings with a push to measure")
            raise Abort("the decay can't be fitted (%s); the lab's calibration was not changed"
                        % why)
        fit = calfit.fit_decay(rows)

        # Each firing, for the page.
        firings = []
        for b in burns:
            fitted = b.get("fit") or {}
            firings.append({"n": b["thruster"], "repeat": b["repeat"], "open": b["open"],
                            "error": b.get("error"), "ratio": fitted.get("ratio"),
                            "ratio_sd": fitted.get("ratio_sd"), "used": bool(fitted.get("used")),
                            "net_N": fitted.get("net_N"),
                            "push_alone_N": math.hypot(*fitted["push_alone"]) if fitted else None,
                            "rms_mm": fitted.get("rms_mm")})
        return dict(fit, updated=now_iso(), by=run.by, run=run.stamp,
                    single_run=params["single"].get("run"),
                    params={k: v for k, v in params.items() if k != "single"},
                    firings=firings, table_used=bool(store.get()["calibration"].get("table")))


# The arm

ARM_TESTS = ("breakaway", "sweep", "inertia", "steps")
JOINT_NAMES = {1: "Shoulder", 2: "Elbow", 3: "Wrist"}
ARM_POLL = 2.0               # [s] between status checks while the script runs
# [s] the longest a run may take (three joints, every test: about 15
# min).
ARM_TIMEOUT = 45 * 60
ARM_FILE_LIMIT = 20 * 1024 * 1024   # [bytes] a data file larger than this is left out
AGENT_TIMEOUT = 20           # [s] agent commands
SEND_TIMEOUT = 30            # [s] sending and starting the script
DATA_TIMEOUT = 120           # [s] collecting its data


def arm_script():
    """Return the arm calibration script's path.

    Next to the console on the TX2 (deploy_tx2.sh copies it there), or
    in the template's +spot/+arm/calibration.

    Raises:
        ApiError: if it is in neither place.
    """
    for path in (PROJECT_DIR / "arm_calibration" / "calibrate_arm.py",
                 PROJECT_DIR.parent / "+arm" / "calibration" / "calibrate_arm.py"):
        if path.is_file():
            return path
    raise ApiError("calibrate_arm.py is not on the lab console: run deploy_tx2.sh (it copies "
                   "+spot/+arm/calibration/calibrate_arm.py there).")


def arm_check():
    """Return what calibrate_arm.py needs on the Chaser.

    Returns:
        {"ok": whether a real run can start, "reachable", "items":
        [{"what", "ok", "value", "fix"}], "running", "script", "raw"};
        or, if the Chaser doesn't answer, {"ok": False, "reachable":
        False, "error"}.
    """
    rc, out, err = platform_run("RED", "arm-cal-check", timeout=AGENT_TIMEOUT)
    text = decode(out)
    if rc != 0 or not text:
        reason = err or text or "no answer (code %s)" % rc
        if "only accepts" in reason:
            reason = ("the Chaser's agent is older than this console: run "
                      "setup_hardware_link.sh to update it")
        return {"ok": False, "reachable": False, "items": [], "error": reason, "raw": text}

    # Its lines: "<key> <value>", "user <program>" and "servos <baud>
    # <id>:<model> ...".
    fields, users, servos = {}, [], {}
    for line in text.splitlines():
        key, _, value = line.partition(" ")
        if key == "user":
            users.append(value)
        elif key == "servos" and value != "error":
            baud, _, found = value.partition(" ")
            servos[baud] = {int(i): int(m)
                            for i, m in (x.split(":") for x in found.split() if ":" in x)}
        else:
            fields[key] = value

    # One item per requirement, with how to fix it.
    python_ver = fields.get("python", "no")
    items = [
        {"what": "Python 3", "ok": python_ver != "no",
         "value": python_ver if python_ver != "no" else "missing",
         "fix": "Install python3 on the Chaser."},
        {"what": "Dynamixel SDK", "ok": fields.get("sdk") == "yes",
         "value": "installed" if fields.get("sdk") == "yes" else "missing",
         "fix": "On the Chaser: pip3 install --user dynamixel-sdk"},
        {"what": "U2D2 at /dev/ttyUSB0", "ok": fields.get("port") == "yes",
         "value": "found" if fields.get("port") == "yes" else "not found",
         "fix": "Plug in the U2D2 and power the arm."},
        {"what": "Access to the port", "ok": fields.get("access") == "yes",
         "value": "yes" if fields.get("access") == "yes" else "no",
         "fix": "Run +spot/+hardware/arm/setup_arm_serial.sh for the Chaser (dialout group, "
                "udev rule), then log in again."},
        {"what": "Port free", "ok": not users, "value": ", ".join(users) if users else "free",
         "fix": "Stop the program holding it (an experiment?)."},
        {"what": "Latency timer", "ok": True, "warn": fields.get("latency") not in ("1",),
         "value": "%s ms" % fields["latency"]
                  if fields.get("latency", "unknown") != "unknown" else "unknown",
         "fix": "setup_arm_serial.sh sets it to 1 ms (16 ms works, but the data comes slower)."},
    ]
    items.append(servo_item(fields, servos, users))
    running = fields.get("running", "no")
    return {"ok": all(i["ok"] for i in items) and running == "no", "reachable": True,
            "items": items, "running": running != "no", "script": fields.get("script") == "yes",
            "raw": text}


# Model numbers (calibrate_arm.py's MODELS).
ARM_SERVO_NAMES = {311: "MX-64", 321: "MX-106", 30: "MX-28", 1020: "XM430", 1120: "XM540"}


def servo_item(fields, servos, users):
    """Return the setup check's servo line.

    Whether IDs 1, 2 and 3 answer at the bus's 1 Mbps (the agent's
    scan), and if not, what to check.
    """
    what = "Servos 1-3 (1 Mbps)"

    # Not scanned: the port was busy, the scan failed, or the agent is
    # old.
    if not servos:
        why = ("not scanned: the port is in use" if users or fields.get("running", "no") != "no"
               else "not scanned" + (": the scan failed" if "servos" in fields
                                     else ": the Chaser's agent is older (run "
                                          "+spot/+console/deploy/setup_hardware_link.sh red)"))
        return {"what": what, "ok": True, "warn": True, "value": why,
                "fix": "The run pings them first anyway."}

    # The servos at 1 Mbps; all three there: fine.
    bus = servos.get("1000000", {})
    names = {1: "shoulder", 2: "elbow", 3: "wrist"}
    missing = [i for i in (1, 2, 3) if i not in bus]
    found = ", ".join("%d %s" % (i, ARM_SERVO_NAMES.get(m, m))
                      for i, m in sorted(bus.items())) or "none"
    slow = servos.get("57600", {})
    if not missing:
        return {"what": what, "ok": True, "value": found, "fix": ""}

    # Missing: the cables, a reset servo at 57600 baud, or a wrong ID.
    fix = ("No answer from %s. Check its cable in the daisy chain (and the next one along) and "
           "the arm's power." % ", ".join("ID %d (%s)" % (i, names[i]) for i in missing))
    if slow:
        fix += (" At 57600 baud (the factory default: a servo reset or replaced) %s answer%s: "
                "give it its ID and 1 Mbps in Dynamixel Wizard."
                % (", ".join("ID %d" % i for i in sorted(slow)), "s" if len(slow) == 1 else ""))
    others = sorted(set(bus) - {1, 2, 3})
    if others:
        fix += (" Also answering at 1 Mbps: %s (a servo with the wrong ID?)."
                % ", ".join("ID %d" % i for i in others))
    return {"what": what, "ok": not missing, "value": found, "fix": fix}


class ArmRun:
    """An arm calibration (see the module's help)."""

    def __init__(self, cal, run):
        """Set up the run with its Driver and the Chaser."""
        self.cal = cal
        self.run = run
        self.d = Driver(cal, run)
        self.hw = hardware.get("RED")
        self.reserved = False

    def main(self):
        """Run it: check, send the script, follow it, collect."""
        run, drv, params = self.run, self.d, self.run.params
        run.steps = ([{"name": "Check", "state": "waiting", "note": "What the Chaser has"},
                      {"name": "Send Script", "state": "waiting", "note": "calibrate_arm.py"}]
                     + [{"name": "%s %s" % (JOINT_NAMES[j], t.capitalize()), "state": "waiting",
                         "note": "", "joint": j, "test": t}
                        for j in params["joints"] for t in params["tests"]]
                     + [{"name": "Collect", "state": "waiting", "note": "Data and fit"}])
        run.check = None
        ok, note, started = False, None, False
        try:
            # 1. The Chaser has what the script needs (a rehearsal needs
            # none of the hardware).
            drv.begin_step(0)
            checks = arm_check()
            run.check = checks
            if not checks["reachable"]:
                raise Abort("the Chaser doesn't answer: %s" % checks["error"])
            if checks["running"]:
                raise Abort("a calibration is already running on the Chaser (stop it, or wait "
                            "for it)")
            missing = [i["what"] for i in checks["items"] if not i["ok"]]
            if missing and not params["simulate"]:
                raise Abort("the Chaser isn't ready: %s (see the checks)" % ", ".join(missing))
            drv.end_step(note="ready" if not missing
                              else "rehearsal (%s missing)" % ", ".join(missing))

            # The platform rests while the arm moves: nothing may float
            # it.
            if not params["simulate"]:
                self.hw.reserve(run, run.by)
                self.reserved = True

            # 2. Send the script.
            drv.begin_step(1)
            buf = io.BytesIO()
            with tarfile.open(fileobj=buf, mode="w") as tar:
                tar.add(str(arm_script()), arcname="calibrate_arm.py")
            rc, out, err = platform_run("RED", "arm-cal-receive", data=buf.getvalue(),
                                        timeout=SEND_TIMEOUT)
            if rc != 0:
                raise Abort("could not send the script: %s" % (err or decode(out)))
            drv.end_step()

            # 3. Start it, and follow it to the end.
            cmd = "arm-cal-start %s %s%s" % ("".join(str(j) for j in params["joints"]),
                                             ",".join(params["tests"]),
                                             " simulate" if params["simulate"] else "")
            rc, out, err = platform_run("RED", cmd, timeout=SEND_TIMEOUT)
            if rc != 0 or "started" not in decode(out):
                raise Abort("the script didn't start: %s" % (err or decode(out)))
            started = True
            drv.say("%s started on the Chaser (%s; %s)."
                    % ("A rehearsal (simulated servos)" if params["simulate"]
                       else "The arm calibration",
                       ", ".join(JOINT_NAMES[j] for j in params["joints"]),
                       ", ".join(params["tests"])))
            code = self.follow()
            started = False
            if code != 0:
                raise Abort("the script stopped (exit %s): %s" % (code, self.last_words()))

            # 4. Collect: a rehearsal's result is shown, a real one's
            # becomes the lab's.
            drv.begin_step(len(run.steps) - 1)
            result = self.collect()
            if params["simulate"]:
                run.result = result
                drv.end_step(note="rehearsal: not saved")
                drv.say("Rehearsal done: the fit of the simulated servos is shown; the lab's arm "
                        "calibration is unchanged.")
            else:
                store.set_section("arm", result)
                run.result = result
                drv.end_step(note="saved")
                drv.say("Done: the arm calibration is saved; projects get it with Update from "
                        "Lab.")
            ok = True
        except Abort as e:
            note = str(e)
        except ApiError as e:
            note = str(e)
        except Exception as e:
            note = "internal error: %s: %s" % (type(e).__name__, e)
        finally:
            # Never leave the script moving the arm when the run ends.
            if started:
                platform_run("RED", "arm-cal-stop", timeout=AGENT_TIMEOUT)
            if self.reserved:
                self.hw.release(run, "Arm calibration %s" % ("finished" if ok else "stopped"))
        self.done(ok, note)

    def follow(self):
        """Follow the script until it ends; return its exit code.

        Stops it when the run is stopped, the Chaser is taken back (Esc
        or all off on the Hardware tab), or it runs too long.

        Raises:
            Abort: in those cases.
        """
        run = self.run
        t0 = time.monotonic()
        while True:
            # Reasons to stop it.
            if run.state != "running":
                platform_run("RED", "arm-cal-stop", timeout=AGENT_TIMEOUT)
                raise Abort(run.stop_reason or "stopped")
            if self.reserved and not self.hw.owned_by(run):
                platform_run("RED", "arm-cal-stop", timeout=AGENT_TIMEOUT)
                raise Abort("the Chaser was taken back (switched off on the Hardware tab)")
            if time.monotonic() - t0 > ARM_TIMEOUT:
                platform_run("RED", "arm-cal-stop", timeout=AGENT_TIMEOUT)
                raise Abort("it ran for more than %d minutes" % (ARM_TIMEOUT // 60))

            # Its log and state: running, or ended with an exit code
            # (none: -1).
            rc, out, err = platform_run("RED", "arm-cal-status", timeout=AGENT_TIMEOUT)
            if rc == 0:
                text = decode(out)
                lines = [line[4:] for line in text.splitlines() if line.startswith("log ")]
                self.progress(lines)
                code = re.search(r"^exit (\d+)$", text, re.M)
                running = re.search(r"^running (\d+)$", text, re.M)
                if not running and code:
                    return int(code.group(1))
                if not running and not code:
                    return -1
            time.sleep(ARM_POLL)

    def progress(self, lines):
        """Show the script's log; its PROGRESS lines as steps."""
        run = self.run
        run.armlog = lines

        # The last PROGRESS line: {"joint", "test", "index", "total"}.
        last = None
        for line in lines:
            if line.startswith("PROGRESS "):
                try:
                    last = json.loads(line[9:])
                except ValueError:
                    pass

        # Its step runs; the steps before it are done.
        if last:
            for k, step in enumerate(run.steps):
                if step.get("joint") == last["joint"] and step.get("test") == last["test"]:
                    if run.step != k:
                        for prev in run.steps[2:k]:
                            if prev["state"] in ("waiting", "running"):
                                prev["state"] = "done"
                        self.d.begin_step(k, "%d of %d" % (last["index"], last["total"]))
                        self.d.say("%s: %s (%d of %d)." % (JOINT_NAMES[last["joint"]],
                                                          last["test"], last["index"],
                                                          last["total"]),
                                   log=False)

        # The last other line as the message.
        tail = [line for line in lines if line.strip() and not line.startswith("PROGRESS ")]
        if tail:
            run.message = tail[-1].strip()[:200]
        self.cal.publish(force=False)

    def last_words(self):
        """Return the script's last output line (not PROGRESS/EXIT)."""
        lines = [line for line in getattr(self.run, "armlog", [])
                 if line.strip() and not line.startswith(("PROGRESS ", "EXIT "))]
        return lines[-1].strip() if lines else "no output"

    def collect(self):
        """Collect the run's files and return the calibration.

        The files go into ~/SPOTCalibration/<stamp>_arm_RED/; the
        calibration is the script's fit (arm_calibration.json) with
        what the page draws (each joint's friction points, its
        breakaway torques, its position steps, thinned).

        Raises:
            Abort: if nothing came back, or there is no valid
                arm_calibration.json.
        """
        run = self.run
        rc, out, err = platform_run("RED", "arm-cal-data", timeout=DATA_TIMEOUT)
        if rc != 0 or not out:
            raise Abort("could not collect the data: %s" % (err or "nothing came back"))
        folder = RUNS_DIR / ("%s_arm_RED%s" % (run.stamp,
                                               "_rehearsal" if run.params["simulate"] else ""))
        folder.mkdir(parents=True, exist_ok=True)

        # The .json and .csv files (of a sane size and name).
        files = {}
        with tarfile.open(fileobj=io.BytesIO(out), mode="r") as tar:
            for member in tar.getmembers():
                name = member.name.rsplit("/", 1)[-1]
                if (not member.isfile() or not re.match(r"^[A-Za-z0-9_.-]+\.(json|csv)$", name)
                        or member.size > ARM_FILE_LIMIT):
                    continue
                data = tar.extractfile(member).read()
                (folder / name).write_bytes(data)
                files[name] = data.decode("utf-8", "replace")

        # The script's fit, with the run's details and the plots.
        if "arm_calibration.json" not in files:
            raise Abort("the run has no arm_calibration.json (did it finish?)")
        cal = json.loads(files["arm_calibration.json"])
        if cal.get("format") != "spot-arm-calibration":
            raise Abort("arm_calibration.json isn't an arm calibration")
        cal.pop("folder", None)
        cal.update(updated=now_iso(), by=run.by, run=run.stamp, simulated=run.params["simulate"],
                   lab_folder=str(folder), tests=run.params["tests"], plots=arm_plots(files, cal))
        return cal

    def done(self, ok, note):
        """Record how the run ended."""
        run = self.run
        if not ok:
            if run.step is not None and run.steps[run.step]["state"] == "running":
                run.steps[run.step]["state"] = "failed"
            stopped = run.state == "stopping"
            run.error = None if stopped else note
            run.state = "stopped" if stopped else "failed"
            self.d.say(("Stopped: %s." if stopped else "Failed: %s.")
                       % (run.stop_reason if stopped else note))
        else:
            run.state = "done"
        run.finished = time.time()
        self.cal.publish()


def read_csv_text(text):
    """Return a CSV's header and its numeric rows (others skipped)."""
    rows = [line.split(",") for line in text.strip().splitlines()]
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
    """Return what the page draws of an arm calibration, per joint.

    The friction against speed (the sweep's steady points as torque;
    the fitted curve's parameters are in the joint's fit), and the
    position steps (goal and measured, thinned).
    """
    kt = cal.get("kt") or {}
    plots = {}
    for j in (1, 2, 3):
        plot = {}

        # The sweep: [velocity, torque] (current x torque constant).
        kt_joint = kt.get(str(j)) if isinstance(kt, dict) else kt
        pts = files.get("sweep_points_j%d.json" % j)
        if pts and kt_joint:
            try:
                plot["sweep"] = [[round(point["velocity"], 5),
                                  round(kt_joint * point["current"], 5)]
                                 for point in json.loads(pts)]
            except (ValueError, KeyError, TypeError):
                pass

        # The steps: [t, goal, measured].
        if "steps_j%d.csv" % j in files:
            head, rows = read_csv_text(files["steps_j%d.csv" % j])
            col = "q%d" % j
            if rows and "t" in head and "goal" in head and col in head:
                ti, gi, qi = head.index("t"), head.index("goal"), head.index(col)
                plot["steps"] = [[round(row[ti], 3), round(row[gi], 5), round(row[qi], 5)]
                                 for row in thin(rows, 400)]
        if plot:
            plots[str(j)] = plot
    return plots


def noise_mm(samples):
    """Return the cameras' noise on a resting platform [mm].

    The standard deviation of its position; None with fewer than 5
    samples.
    """
    if not samples or len(samples) < 5:
        return None
    mx = sum(s[1] for s in samples) / len(samples)
    my = sum(s[2] for s in samples) / len(samples)
    var = sum((s[1] - mx) ** 2 + (s[2] - my) ** 2 for s in samples) / (2 * (len(samples) - 1))
    return 1000 * math.sqrt(var)
