"""Spacecraft Designer: what is mounted on each platform (docking cone, solar panels).

Part of the SPOT Console. One design per console, kept in a JSON file: the local console's in the
project folder (spot_design.json, next to spot_settings.json), the lab console's in its home
(~/.spot_design.json). The 3D views draw it when Use CAD is on; the local console can send its
design to the lab console, or take the lab console's.

    {"version": 1,
     "platforms": {"RED": {"dock": {"type": "male", "face": "+y", "side": "left"},
                           "solar": {"on": false, "axis": "x"}}, "BLACK": {...}, "BLUE": {...}}}

dock.type: none, male (the cone that goes in, on the interface bracket), female (the cup with the
electromagnet, on an arm coupler's plate) or, on the Chaser only, arm (the robotic arm on an arm
coupler's plate, the female cup's; its end effector is the male cone, so it is one or the other; the
local console keeps the simulation's Arm platform and s.mass.arm.mount in step with it). It sits on one of the four side faces (+x is the front,
where the Chaser's ZED2 is; not the back, -x, where the control panel is), on the left or right of
the face as seen from outside, in the upper half: 3 x 2 = 6 places, symmetric about the face's
vertical centre line. The left place on +y is
the Chaser's arm coupler: a cone there takes the coupler's place. solar: the pair of panels on its bracket over the top, with the panels
off the +-x faces (axis x) or the +-y faces (axis y).
"""

import copy
import json
import threading
from pathlib import Path

from .common import ApiError
from .events import hub

PLATFORMS = ("RED", "BLACK", "BLUE")
DOCK_TYPES = ("none", "male", "female", "arm")   # arm: the Chaser's only
FACES = ("+x", "+y", "-y")   # not -x (the back): the control panel is there
SIDES = ("left", "right")
AXES = ("x", "y")

DEFAULT = {
    "version": 1,
    "platforms": {
        # As in the lab: the Chaser's male cone on its arm coupler, the Target's cup at the same place
        "RED": {"dock": {"type": "male", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
        "BLACK": {"dock": {"type": "female", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
        "BLUE": {"dock": {"type": "none", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
    },
}


def validate(design):
    """A complete, checked copy of DESIGN (missing parts filled from the defaults)."""
    if not isinstance(design, dict):
        raise ApiError("The design must be an object.")
    plats = design.get("platforms", {})
    if not isinstance(plats, dict):
        raise ApiError("design.platforms must be an object.")
    out = copy.deepcopy(DEFAULT)
    for name, p in plats.items():
        if name not in PLATFORMS:
            raise ApiError("Unknown platform %r in the design." % name)
        if not isinstance(p, dict):
            raise ApiError("The design of %s must be an object." % name)
        dock, solar = p.get("dock", {}) or {}, p.get("solar", {}) or {}
        d, s = out["platforms"][name]["dock"], out["platforms"][name]["solar"]
        # (a "level" from the first version, when there were lower places too, is ignored, and a cone
        # on the back face, which is no longer offered, moves to +y)
        if dock.get("face") == "-x":
            dock = dict(dock, face="+y")
        for key, allowed in (("type", DOCK_TYPES), ("face", FACES), ("side", SIDES)):
            if key in dock:
                if dock[key] not in allowed:
                    raise ApiError("%s's dock %s must be one of %s." % (name, key, ", ".join(allowed)))
                d[key] = dock[key]
        if d["type"] == "arm" and name != "RED":
            raise ApiError("Only the Chaser carries the arm.")
        if "on" in solar:
            if not isinstance(solar["on"], bool):
                raise ApiError("%s's solar.on must be true or false." % name)
            s["on"] = solar["on"]
        if "axis" in solar:
            if solar["axis"] not in AXES:
                raise ApiError("%s's solar.axis must be x or y." % name)
            s["axis"] = solar["axis"]
    return out


class DesignStore:
    def __init__(self):
        self._lock = threading.Lock()
        self.path = None
        self.design = copy.deepcopy(DEFAULT)
        self.saved = False   # whether the file exists (else the defaults are shown)

    def open(self, path):
        """Use PATH (read it if it exists; a bad file is reported and the defaults are used)."""
        self.path = Path(path)
        try:
            self.design = validate(json.loads(self.path.read_text(encoding="utf-8")))
            self.saved = True
        except FileNotFoundError:
            pass
        except (OSError, ValueError, ApiError) as e:
            hub.log("stream", "Spacecraft design %s could not be read (%s); using the defaults." % (self.path, e))

    def get(self):
        with self._lock:
            return {"design": copy.deepcopy(self.design), "path": str(self.path) if self.path else None,
                    "saved": self.saved}

    def set(self, design, requester=None):
        d = validate(design)
        with self._lock:
            self.design = d
            if self.path:
                tmp = self.path.with_name(self.path.name + ".tmp")
                tmp.write_text(json.dumps(d, indent=2) + "\n", encoding="utf-8")
                tmp.replace(self.path)
                self.saved = True
        hub.publish("design", self.get())
        return self.get()


design_store = DesignStore()
