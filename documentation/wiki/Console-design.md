# design.py

**File:** `+spot/+console/webapp/spot_console/design.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Designer Tab|Designer-Tab]]

<!-- source: +spot/+console/webapp/spot_console/design.py -->

## What it is for

The Spacecraft Designer's data: what is mounted on each platform. One design per console, kept as JSON: the local console's in the project (`spot_design.json`, next to `spot_settings.json`), the lab console's in its home (`~/.spot_design.json`). The 3D views draw it when *Use CAD* is on; the local console can send its design to the lab console or take the lab console's.

```json
{"version": 1, "platforms": {"RED": {"dock": {"type": "arm", "face": "+y", "side": "left"}, "solar": {"on": false, "axis": "x"}}, ...}}
```

`dock.type` is `none`, `male` (the cone that goes in, on the interface bracket), `female` (the cup with the electromagnet, on an arm coupler's plate) or, on the Chaser only, `arm` (the arm on the coupler's plate; its end effector is the male cone, so it is one or the other; the local console keeps the simulation's Arm platform and `mass.arm.mount` in step with it). It sits on the `+x` (front), `+y` or `-y` face (not `-x`, the back, where the control panel is), `left` or `right` of the face's centre line as seen from outside: six places. `solar` is the pair of panels on their bracket over the top, off the ±x or ±y faces.

## Function by function

**Lines 31–45.** The allowed values and the default design (as in the lab: the Chaser's male cone and the Target's cup on their couplers).

**Lines 48–82.** `validate`: a complete, checked copy, missing parts filled from the defaults: platforms must be known, each value in its allowed set, the arm only on RED; a cone on the back face from an early version moves to +y, and the old `level` is ignored.

**Lines 85–121.** `DesignStore`: `open(path)` reads the file if it exists (a bad file is logged and the defaults used); `get` returns a copy with the path and whether a file exists; `set` validates, writes the file atomically (a `.tmp` then `replace`), and publishes a `design` event so every page follows.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/design.py</b> (121 lines; click to show)</summary>

```python
   1  """Spacecraft Designer: what is mounted on each platform (docking cone, solar panels).
   2  
   3  Part of the SPOT Console. One design per console, kept in a JSON file: the local console's in the
   4  project folder (spot_design.json, next to spot_settings.json), the lab console's in its home
   5  (~/.spot_design.json). The 3D views draw it when Use CAD is on; the local console can send its
   6  design to the lab console, or take the lab console's.
   7  
   8      {"version": 1,
   9       "platforms": {"RED": {"dock": {"type": "male", "face": "+y", "side": "left"},
  10                             "solar": {"on": false, "axis": "x"}}, "BLACK": {...}, "BLUE": {...}}}
  11  
  12  dock.type: none, male (the cone that goes in, on the interface bracket), female (the cup with the
  13  electromagnet, on an arm coupler's plate) or, on the Chaser only, arm (the robotic arm on an arm
  14  coupler's plate, the female cup's; its end effector is the male cone, so it is one or the other; the
  15  local console keeps the simulation's Arm platform and s.mass.arm.mount in step with it). It sits on one of the four side faces (+x is the front,
  16  where the Chaser's ZED2 is; not the back, -x, where the control panel is), on the left or right of
  17  the face as seen from outside, in the upper half: 3 x 2 = 6 places, symmetric about the face's
  18  vertical centre line. The left place on +y is
  19  the Chaser's arm coupler: a cone there takes the coupler's place. solar: the pair of panels on its bracket over the top, with the panels
  20  off the +-x faces (axis x) or the +-y faces (axis y).
  21  """
  22  
  23  import copy
  24  import json
  25  import threading
  26  from pathlib import Path
  27  
  28  from .common import ApiError
  29  from .events import hub
  30  
  31  PLATFORMS = ("RED", "BLACK", "BLUE")
  32  DOCK_TYPES = ("none", "male", "female", "arm")   # arm: the Chaser's only
  33  FACES = ("+x", "+y", "-y")   # not -x (the back): the control panel is there
  34  SIDES = ("left", "right")
  35  AXES = ("x", "y")
  36  
  37  DEFAULT = {
  38      "version": 1,
  39      "platforms": {
  40          # As in the lab: the Chaser's male cone on its arm coupler, the Target's cup at the same place
  41          "RED": {"dock": {"type": "male", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
  42          "BLACK": {"dock": {"type": "female", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
  43          "BLUE": {"dock": {"type": "none", "face": "+y", "side": "left"}, "solar": {"on": False, "axis": "x"}},
  44      },
  45  }
  46  
  47  
  48  def validate(design):
  49      """A complete, checked copy of DESIGN (missing parts filled from the defaults)."""
  50      if not isinstance(design, dict):
  51          raise ApiError("The design must be an object.")
  52      plats = design.get("platforms", {})
  53      if not isinstance(plats, dict):
  54          raise ApiError("design.platforms must be an object.")
  55      out = copy.deepcopy(DEFAULT)
  56      for name, p in plats.items():
  57          if name not in PLATFORMS:
  58              raise ApiError("Unknown platform %r in the design." % name)
  59          if not isinstance(p, dict):
  60              raise ApiError("The design of %s must be an object." % name)
  61          dock, solar = p.get("dock", {}) or {}, p.get("solar", {}) or {}
  62          d, s = out["platforms"][name]["dock"], out["platforms"][name]["solar"]
  63          # (a "level" from the first version, when there were lower places too, is ignored, and a cone
  64          # on the back face, which is no longer offered, moves to +y)
  65          if dock.get("face") == "-x":
  66              dock = dict(dock, face="+y")
  67          for key, allowed in (("type", DOCK_TYPES), ("face", FACES), ("side", SIDES)):
  68              if key in dock:
  69                  if dock[key] not in allowed:
  70                      raise ApiError("%s's dock %s must be one of %s." % (name, key, ", ".join(allowed)))
  71                  d[key] = dock[key]
  72          if d["type"] == "arm" and name != "RED":
  73              raise ApiError("Only the Chaser carries the arm.")
  74          if "on" in solar:
  75              if not isinstance(solar["on"], bool):
  76                  raise ApiError("%s's solar.on must be true or false." % name)
  77              s["on"] = solar["on"]
  78          if "axis" in solar:
  79              if solar["axis"] not in AXES:
  80                  raise ApiError("%s's solar.axis must be x or y." % name)
  81              s["axis"] = solar["axis"]
  82      return out
  83  
  84  
  85  class DesignStore:
  86      def __init__(self):
  87          self._lock = threading.Lock()
  88          self.path = None
  89          self.design = copy.deepcopy(DEFAULT)
  90          self.saved = False   # whether the file exists (else the defaults are shown)
  91  
  92      def open(self, path):
  93          """Use PATH (read it if it exists; a bad file is reported and the defaults are used)."""
  94          self.path = Path(path)
  95          try:
  96              self.design = validate(json.loads(self.path.read_text(encoding="utf-8")))
  97              self.saved = True
  98          except FileNotFoundError:
  99              pass
 100          except (OSError, ValueError, ApiError) as e:
 101              hub.log("stream", "Spacecraft design %s could not be read (%s); using the defaults." % (self.path, e))
 102  
 103      def get(self):
 104          with self._lock:
 105              return {"design": copy.deepcopy(self.design), "path": str(self.path) if self.path else None,
 106                      "saved": self.saved}
 107  
 108      def set(self, design, requester=None):
 109          d = validate(design)
 110          with self._lock:
 111              self.design = d
 112              if self.path:
 113                  tmp = self.path.with_name(self.path.name + ".tmp")
 114                  tmp.write_text(json.dumps(d, indent=2) + "\n", encoding="utf-8")
 115                  tmp.replace(self.path)
 116                  self.saved = True
 117          hub.publish("design", self.get())
 118          return self.get()
 119  
 120  
 121  design_store = DesignStore()
```

</details>
<!-- /listing -->
