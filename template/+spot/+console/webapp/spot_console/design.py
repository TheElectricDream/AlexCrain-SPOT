"""Spacecraft Designer: what is mounted on each platform.

Part of the SPOT Console. One design per console, kept in a JSON file:
the local console's in the project folder (spot_design.json, next to
spot_settings.json), the lab console's in its home
(~/.spot_design.json). The 3D views draw it when Use CAD is on; the
local console can send its design to the lab console, or take the lab
console's.

    {"version": 1,
     "platforms": {
         "RED": {"dock": {"type": "male", "face": "+y",
                          "side": "left"},
                 "solar": {"on": false, "axis": "x"}},
         "BLACK": {...}, "BLUE": {...}}}

dock.type is one of:
    none
    male    the cone that goes in, on the interface bracket
    female  the cup with the electromagnet, on an arm coupler's plate
    arm     Chaser only: the robotic arm on an arm coupler's plate
            (the female cup's). Its end effector is the male cone, so
            it is one or the other. The local console keeps the
            simulation's Arm platform and s.mass.arm.mount in step.

The dock sits on one of three side faces (+x is the front, where the
Chaser's ZED2 is; not -x, the back, where the control panel is), on
the left or right of the face as seen from outside, in the upper half:
3 x 2 = 6 places, symmetric about the face's vertical centre line. The
left place on +y is the Chaser's arm coupler: a cone there takes the
coupler's place.

solar is the pair of panels on a bracket over the top, sticking out
from the +-x faces (axis x) or the +-y faces (axis y).
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

# As in the lab: the Chaser's male cone on its arm coupler, the Target's
# cup at the same place.
DEFAULT = {
    "version": 1,
    "platforms": {
        "RED": {"dock": {"type": "male", "face": "+y", "side": "left"},
                "solar": {"on": False, "axis": "x"}},
        "BLACK": {"dock": {"type": "female", "face": "+y", "side": "left"},
                  "solar": {"on": False, "axis": "x"}},
        "BLUE": {"dock": {"type": "none", "face": "+y", "side": "left"},
                 "solar": {"on": False, "axis": "x"}},
    },
}


def validate(design):
    """Return a complete, checked copy of a design.

    Args:
        design: A design as received; missing parts are filled from
            DEFAULT.

    Raises:
        ApiError: if any part is the wrong type or not an allowed
            value.
    """
    if not isinstance(design, dict):
        raise ApiError("The design must be an object.")
    plats = design.get("platforms", {})
    if not isinstance(plats, dict):
        raise ApiError("design.platforms must be an object.")

    out = copy.deepcopy(DEFAULT)
    for name, entry in plats.items():
        if name not in PLATFORMS:
            raise ApiError("Unknown platform %r in the design." % name)
        if not isinstance(entry, dict):
            raise ApiError("The design of %s must be an object." % name)
        dock = entry.get("dock", {}) or {}
        solar = entry.get("solar", {}) or {}
        dock_out = out["platforms"][name]["dock"]
        solar_out = out["platforms"][name]["solar"]

        # Older designs: a "level" (from when there were lower places)
        # is ignored, and a cone on the back face, no longer offered,
        # moves to +y.
        if dock.get("face") == "-x":
            dock = dict(dock, face="+y")

        # The dock: each given part must be an allowed value.
        for key, allowed in (("type", DOCK_TYPES), ("face", FACES), ("side", SIDES)):
            if key in dock:
                if dock[key] not in allowed:
                    raise ApiError("%s's dock %s must be one of %s."
                                   % (name, key, ", ".join(allowed)))
                dock_out[key] = dock[key]
        if dock_out["type"] == "arm" and name != "RED":
            raise ApiError("Only the Chaser carries the arm.")

        # The solar panels.
        if "on" in solar:
            if not isinstance(solar["on"], bool):
                raise ApiError("%s's solar.on must be true or false." % name)
            solar_out["on"] = solar["on"]
        if "axis" in solar:
            if solar["axis"] not in AXES:
                raise ApiError("%s's solar.axis must be x or y." % name)
            solar_out["axis"] = solar["axis"]
    return out


class DesignStore:
    """The console's design, its file, and updates to the pages."""

    def __init__(self):
        """Start with the defaults and no file."""
        self._lock = threading.Lock()
        self.path = None
        self.design = copy.deepcopy(DEFAULT)
        self.saved = False   # whether the file exists (else the defaults are shown)

    def open(self, path):
        """Use a design file, reading it if it exists.

        A file that can't be read is reported in the log, and the
        defaults are used.
        """
        self.path = Path(path)
        try:
            self.design = validate(json.loads(self.path.read_text(encoding="utf-8")))
            self.saved = True
        except FileNotFoundError:
            pass
        except (OSError, ValueError, ApiError) as e:
            hub.log("stream", "Spacecraft design %s could not be read (%s); using the defaults."
                    % (self.path, e))

    def get(self):
        """Return the design, its file and whether it has been saved."""
        with self._lock:
            return {"design": copy.deepcopy(self.design),
                    "path": str(self.path) if self.path else None,
                    "saved": self.saved}

    def set(self, design, requester=None):
        """Check, keep and save a new design, and tell the pages.

        The file is replaced atomically (written to .tmp, then
        renamed).

        Raises:
            ApiError: if the design isn't valid.
        """
        checked = validate(design)
        with self._lock:
            self.design = checked
            if self.path:
                tmp = self.path.with_name(self.path.name + ".tmp")
                tmp.write_text(json.dumps(checked, indent=2) + "\n", encoding="utf-8")
                tmp.replace(self.path)
                self.saved = True
        hub.publish("design", self.get())
        return self.get()


design_store = DesignStore()
