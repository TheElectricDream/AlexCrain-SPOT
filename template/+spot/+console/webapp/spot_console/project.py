"""The local console's design and calibration: views over Constants, written through MATLAB.

A project keeps everything in its Constants.m: the Spacecraft Designer's
choices (DOCK_*, SOLAR_*), the lab calibration it took (LAB_*), the
values it overrides (OVERRIDE_*), the LEDs' positions (LED_XY_*) and
whether to offer the lab's newer calibration (LAB_SYNC_*); the lab's full
record (drift paths, burns, sweeps) is measurement data kept in Saved
Data (spot.params.calibrationRecord), drawn under those numbers. The MATLAB
worker is the only writer of that file (spot.params.writeConstants),
so these two classes read the constants (C) and send every change to
MATLAB as "constants.set" or "calibration.take"; after the write the
constants are read again from MATLAB and the pages told (local.py's
after_constants_change). They offer the lab console's DesignStore and
CalibrationStore methods (design.py, calibration.py), so the server's
routes are the same on both consoles.

Part of the SPOT Console (webapp/app.py starts it).
"""

import copy
import json

from . import calibration as calmod
from . import design as designmod
from .common import ApiError
from .constants import C, ConstantsError, json_safe

PLATFORMS = ("RED", "BLACK", "BLUE")
M_PER_MM = 1e-3


def _finite(v):
    """Return a number, or None for NaN/None."""
    v = json_safe(v)
    return v if isinstance(v, (int, float)) and not isinstance(v, bool) else None


def _list(v):
    """Return a constant's vector as a list (a scalar or [] as given)."""
    return list(v) if isinstance(v, list) else v


def _merge(record, numbers):
    """Return a record's section with the numbers on top (dicts merged a level down)."""
    out = copy.deepcopy(record)
    for key, value in numbers.items():
        if isinstance(value, dict) and isinstance(out.get(key), dict):
            out[key] = dict(out[key], **value)
        else:
            out[key] = value
    return out


def _with_record(cal, record):
    """Return the project's numbers with the lab record's measurements under them.

    Each section takes the record's only when the two were measured at
    the same time (its "updated"), so a value changed in Constants.m
    never shows beside an old record. The numbers always win.
    """
    if not isinstance(record, dict):
        return cal
    table = record.get("table")
    if cal.get("table") and isinstance(table, dict) \
            and table.get("updated") == cal["table"].get("updated"):
        cal["table"] = _merge(table, cal["table"])
    for p, entry in (cal.get("thrusters") or {}).items():
        theirs = (record.get("thrusters") or {}).get(p)
        if isinstance(theirs, dict) and theirs.get("updated") == entry.get("updated"):
            cal["thrusters"][p] = _merge(theirs, entry)
    arm = record.get("arm")
    if cal.get("arm") and isinstance(arm, dict) \
            and (arm.get("updated") or arm.get("created")) == cal["arm"].get("updated"):
        joints = dict((j.get("index"), j) for j in arm.get("joints", []) if isinstance(j, dict))
        mine = [_merge(joints.get(j["index"], {}), j) for j in cal["arm"]["joints"]]
        cal["arm"] = _merge(arm, dict(cal["arm"], joints=mine))
    return cal


class ProjectDesign:
    """The project's design (Constants.DOCK_*, SOLAR_*)."""

    def __init__(self, matlab, after_change):
        """Keep the MATLAB bridge and what to do after a write."""
        self._matlab = matlab
        self._after_change = after_change

    @staticmethod
    def design():
        """Return the design, as design.py shapes it, from the constants."""
        plats = {}
        for p in PLATFORMS:
            plats[p] = {"dock": {"type": C.platform("DOCK", p), "face": C.platform("DOCK_FACE", p),
                                 "side": C.platform("DOCK_SIDE", p)},
                        "solar": {"on": bool(C.platform("SOLAR", p)),
                                  "axis": C.platform("SOLAR_AXIS", p)}}
        return designmod.validate({"version": 1, "platforms": plats})

    def get(self):
        """Return the design, where it lives and that it is saved."""
        return {"design": self.design(), "path": "Constants.m", "saved": True}

    def set(self, design, requester=None):
        """Check a design and write its constants through MATLAB.

        Raises:
            ApiError: if the design isn't valid, or MATLAB can't write it.
        """
        checked = designmod.validate(design)
        values = {}
        for p in PLATFORMS:
            entry = checked["platforms"][p]
            values["DOCK_" + p] = entry["dock"]["type"]
            values["DOCK_FACE_" + p] = entry["dock"]["face"]
            values["DOCK_SIDE_" + p] = entry["dock"]["side"]
            values["SOLAR_" + p] = entry["solar"]["on"]
            values["SOLAR_AXIS_" + p] = entry["solar"]["axis"]
        self._matlab.request("constants.set", {"values": values}, timeout=120)
        self._after_change()
        return self.get()


class ProjectCalibration:
    """The lab's calibration as the project took it (Constants.LAB_*), its overrides, LEDs."""

    local = True

    def __init__(self, matlab, after_change):
        """Keep the MATLAB bridge and what to do after a write; read the record."""
        self._matlab = matlab
        self._after_change = after_change
        self.record = None
        self.reload_record()

    def reload_record(self):
        """Read the lab's record from MATLAB (spot.params.calibrationRecord); None if none.

        Through the engine, so only while MATLAB is idle (at start, and
        right after a worker command).
        """
        try:
            text = C.evaluate("spot.params.calibrationRecord()")
            self.record = json.loads(text) if text else None
        except (ConstantsError, ValueError):
            self.record = None
        return self.record

    @property
    def cal(self):
        """The calibration, as calibration.py shapes a project's copy."""
        return self.calibration()

    def calibration(self):
        """Return the project's calibration: its numbers, with the lab's record under them.

        The numbers come from the constants (LAB_*, OVERRIDE_*, LED_XY_*);
        the measurements behind them (drift paths, burns, sweeps) from
        the record the project took, where its times match. The shape is
        the lab console's file's, so the page reads it unchanged.
        """
        cal = _with_record(self.numbers(), self.record)
        for entry in (cal.get("thrusters") or {}).values():
            entry.setdefault("valid", [True] * calmod.NUM_THRUSTERS)
            entry.setdefault("thrust_sd", [None] * calmod.NUM_THRUSTERS)
            entry.setdefault("burns", [])
        return cal

    @staticmethod
    def numbers():
        """Return the project's calibration from the constants alone (no record)."""
        cal = {"format": "spot-calibration", "version": 1}
        if C.LAB_TABLE_UPDATED:
            cal["table"] = {
                "updated": C.LAB_TABLE_UPDATED, "model": C.LAB_TABLE_MODEL or "quadratic",
                "height": _list(C.LAB_TABLE_HEIGHT), "centre": _list(C.LAB_TABLE_CENTRE),
                "damping": dict((p, _finite(C.platform("LAB_TABLE_DAMPING", p)))
                                for p in PLATFORMS
                                if _finite(C.platform("LAB_TABLE_DAMPING", p)) is not None),
                "tilt_centre_mrad": _finite(C.LAB_TABLE_TILT_CENTRE_MRAD),
                "tilt_max_mrad": _finite(C.LAB_TABLE_TILT_MAX_MRAD),
                "downhill_deg": _finite(C.LAB_TABLE_DOWNHILL_DEG),
            }
        thrusters = {}
        for p in PLATFORMS:
            if not C.platform("LAB_THRUST_UPDATED", p):
                continue
            entry = {"updated": C.platform("LAB_THRUST_UPDATED", p),
                     "thrust": _list(C.platform("LAB_THRUST", p)),
                     "angle": json_safe(_list(C.platform("LAB_THRUST_ANGLE", p))) or None,
                     "torque_ratio": json_safe(_list(C.platform("LAB_TORQUE_RATIO", p))) or None}
            decay = _finite(C.platform("LAB_THRUST_DECAY", p))
            if decay is not None:
                entry["decay"] = {"model": "linear", "slope": decay,
                                  "updated": entry["updated"]}
            thrusters[p] = entry
        if thrusters:
            cal["thrusters"] = thrusters
        if C.LAB_ARM_UPDATED:
            joints = []
            for j in range(3):
                joint = {"index": j + 1}
                for key, name in (("static", "LAB_ARM_STATIC"), ("coulomb", "LAB_ARM_COULOMB"),
                                  ("stribeck", "LAB_ARM_STRIBECK"),
                                  ("viscous", "LAB_ARM_VISCOUS"),
                                  ("back_emf", "LAB_ARM_BACK_EMF"),
                                  ("torque_per_volt", "LAB_ARM_TORQUE_PER_VOLT"),
                                  ("armature", "LAB_ARM_ARMATURE")):
                    values = _list(C.get(name))
                    value = _finite(values[j]) if isinstance(values, list) \
                        and len(values) > j else None
                    if value is not None:
                        joint[key] = value
                if len(joint) > 1:
                    joints.append(joint)
            cal["arm"] = {"format": "spot-arm-calibration",
                          "version": C.int("ARM_CALIBRATION_FORMAT"), "control": "pwm",
                          "created": C.LAB_ARM_UPDATED, "updated": C.LAB_ARM_UPDATED,
                          "joints": joints}
        cal["updated"] = calmod.newest(cal)
        cal["sync"] = {"ask": bool(C.LAB_SYNC_ASK), "declined": C.LAB_SYNC_DECLINED or None}

        # The overrides and the LEDs, as the file held them (null: not overridden; mm).
        overrides = {"table": {"height": json_safe(_list(C.OVERRIDE_TABLE_HEIGHT)),
                               "damping": dict((p, _finite(C.platform("OVERRIDE_TABLE_DAMPING", p)))
                                               for p in PLATFORMS)},
                     "thrusters": dict((p, {"thrust": json_safe(_list(C.platform("OVERRIDE_THRUST", p))),
                                            "decay": _finite(C.platform("OVERRIDE_THRUST_DECAY", p))})
                                       for p in PLATFORMS),
                     "arm": dict((key, json_safe(_list(C.get("OVERRIDE_ARM_" + key.upper()))))
                                 for key in calmod.ARM_FIELDS)}
        cal["overrides"] = calmod.check_overrides(overrides)
        markers = {}
        for p in PLATFORMS:
            ids = C.platform("LED_IDS", p)
            xy = C.platform("LED_XY", p)
            markers[p] = [{"id": int(ids[k]), "x": round(xy[k][0] / M_PER_MM, 3),
                           "y": round(xy[k][1] / M_PER_MM, 3)} for k in range(len(ids))]
        cal["markers"] = calmod.check_markers(markers)
        return cal

    def get(self):
        """Return the calibration, where it lives and that it is saved."""
        return {"calibration": self.calibration(), "path": "Constants.m", "saved": True}

    def _write(self, values):
        """Write constants through MATLAB and tell the pages."""
        self._matlab.request("constants.set", {"values": values}, timeout=120)
        self._after_change()
        return self.get()

    def take(self, lab_cal):
        """Make the project's calibration the lab's (its numbers).

        MATLAB maps it to the LAB_* constants (spot.worker.labConstants);
        an arm calibration measured with the motor current is left out
        there. The raw runs are not copied.
        """
        cal = calmod.validate(lab_cal)
        slim = {}
        if cal.get("table"):
            slim["table"] = dict((k, v) for k, v in cal["table"].items()
                                 if k in ("updated", "model", "height", "centre", "damping",
                                          "tilt_centre_mrad", "tilt_max_mrad", "downhill_deg"))
        if cal.get("thrusters"):
            slim["thrusters"] = {}
            for p, entry in cal["thrusters"].items():
                keep = dict((k, v) for k, v in entry.items()
                            if k in ("updated", "thrust", "angle", "torque_ratio"))
                decay = entry.get("decay") or {}
                if isinstance(decay, dict) and "slope" in decay:
                    keep["decay"] = {"model": decay.get("model", "linear"),
                                     "slope": decay["slope"]}
                slim["thrusters"][p] = keep
        if cal.get("arm"):
            arm = cal["arm"]
            slim["arm"] = dict((k, v) for k, v in arm.items()
                               if k in ("format", "version", "control", "created", "updated"))
            slim["arm"]["joints"] = [
                dict((k, v) for k, v in joint.items()
                     if k in ("index", "static", "coulomb", "stribeck", "viscous", "back_emf",
                              "torque_per_volt", "armature", "inertia", "pose"))
                for joint in arm.get("joints", []) if isinstance(joint, dict)]
        record = json.dumps(dict((k, v) for k, v in cal.items() if k in ("table", "thrusters", "arm")),
                            allow_nan=False)
        self._matlab.request("calibration.take", {"calibration": slim, "record": record},
                             timeout=120)
        self._after_change()
        return self.get()

    def set_leds(self, platform, leds):
        """Set where a platform's LEDs are (LED_XY_<PLATFORM>, in m).

        Args:
            platform: "RED", "BLACK" or "BLUE".
            leds: Its four LEDs, [{"id", "x", "y"}] [mm from the body
                centre]; None for the default layout.
        """
        platform = calmod.parse_platform(platform)
        leds = calmod.default_leds(platform) if leds is None else calmod.check_leds(platform, leds)
        by_id = dict((led["id"], led) for led in leds)
        xy = [[by_id[int(i)]["x"] * M_PER_MM, by_id[int(i)]["y"] * M_PER_MM]
              for i in C.platform("LED_IDS", platform)]
        return self._write({"LED_XY_" + platform: xy})

    def set_override(self, path, value):
        """Override a value (OVERRIDE_*), or stop overriding it (None)."""
        keys, index, low, high = calmod.parse_override(path)
        value = calmod._override_value(value, low, high, path)
        nan = float("nan")
        new = nan if value is None else value
        if keys[0] == "table" and keys[1] == "height":
            vec = list(C.OVERRIDE_TABLE_HEIGHT)
            vec[index] = new
            return self._write({"OVERRIDE_TABLE_HEIGHT": vec})
        if keys[0] == "table":
            return self._write({"OVERRIDE_TABLE_DAMPING_" + keys[2]: new})
        if keys[0] == "thrusters" and keys[2] == "thrust":
            vec = list(C.platform("OVERRIDE_THRUST", keys[1]))
            vec[index] = new
            return self._write({"OVERRIDE_THRUST_" + keys[1]: vec})
        if keys[0] == "thrusters":
            return self._write({"OVERRIDE_THRUST_DECAY_" + keys[1]: new})
        name = "OVERRIDE_ARM_" + keys[1].upper()
        vec = list(C.get(name))
        vec[index] = new
        return self._write({name: vec})

    def clear_overrides(self, part=None):
        """Stop overriding a part's values (all, if part is None)."""
        nan = float("nan")
        values = {}
        table = {"OVERRIDE_TABLE_HEIGHT": [nan] * calmod.NUM_HEIGHT}
        table.update(("OVERRIDE_TABLE_DAMPING_" + p, nan) for p in PLATFORMS)
        thrusters = dict(("OVERRIDE_THRUST_" + p, [nan] * calmod.NUM_THRUSTERS) for p in PLATFORMS)
        thrusters.update(("OVERRIDE_THRUST_DECAY_" + p, nan) for p in PLATFORMS)
        arm = dict(("OVERRIDE_ARM_" + key.upper(), [nan] * calmod.NUM_JOINTS)
                   for key in calmod.ARM_FIELDS)
        if part is None:
            for group in (table, thrusters, arm):
                values.update(group)
        elif part == "table":
            values = table
        elif part == "thrusters":
            values = thrusters
        elif part == "arm":
            values = arm
        elif part.startswith("thrusters.") and part.split(".", 1)[1] in PLATFORMS:
            p = part.split(".", 1)[1]
            values = {"OVERRIDE_THRUST_" + p: [nan] * calmod.NUM_THRUSTERS,
                      "OVERRIDE_THRUST_DECAY_" + p: nan}
        else:
            raise ApiError("No such part of the overrides: %s." % part)
        return self._write(values)

    def set_sync(self, ask=None, declined=None):
        """Change whether to offer the lab's newer calibration (LAB_SYNC_*)."""
        values = {}
        if ask is not None:
            values["LAB_SYNC_ASK"] = bool(ask)
        if declined is not None:
            values["LAB_SYNC_DECLINED"] = str(declined)
        if not values:
            return self.get()
        return self._write(values)

    def set_section(self, section, value, platform=None):
        """The lab console's: a project has no sections to replace."""
        raise ApiError("A project's calibration comes from the lab console: use Update from Lab.")
