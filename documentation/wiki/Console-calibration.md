# calibration.py

**File:** `+spot/+console/webapp/spot_console/calibration.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Calibration Tab|Calibration-Tab]] · **Fits:** [[calfit.py|Console-calfit]]

<!-- source: +spot/+console/webapp/spot_console/calibration.py -->

## What it is for

The Calibration tab's server side: the calibration file, and the calibrations that measure the lab. The lab console runs the calibrations and keeps the lab's file (`~/.spot_calibration.json`, with each run's raw data in `~/SPOTCalibration/<stamp>_<kind>_<PLATFORM>/run.json`); a project keeps a copy (`resources/spot_calibration.json`) that its simulations use ([[spot.params.loadCalibration|spot-params-loadCalibration]]). When the local console sees the lab console and the lab's file is newer, its page offers it.

```json
{"format": "spot-calibration", "version": 1, "updated": "2026-10-13T18:32:39Z",
 "table": {"updated": "...", "by": "...", "model": "quadratic", "height": [a1, a2, a3, a4, a5], "centre": [1.75, 1.2],
           "damping": {"RED": 0.02}, "damping_fixed": [], "rms_mm": 0.2, "tilt_centre_mrad": 0.36, "tilt_max_mrad": 0.95,
           "downhill_deg": 146, "coverage": {...}, "noise_mm": 0.2, "paths": [{"platform", "cut", "points": [[x, y], ...]}], ...},
 "thrusters": {"RED": {...}}, "arm": {...},
 "sync": {"ask": true, "declined": null}}
```

Times are UTC in ISO 8601, so they compare as text; `updated` is the newest section's. `sync` exists only in a project's copy and is never taken from the lab: `ask` false is **Never Ask Again**, `declined` the lab calibration the user said **Not Now** to.

**The table slope calibration** (`TableRun`), with PhaseSpace streaming and the platform resting on the table:
1. record it sitting still for 2 s (the cameras' noise);
2. float it (pucks on, thrusters off) and record it drifting from where it is;
3. fly it (the diagram's own controller, as [[Interactive Mode|Interactive-Mode]]) to each point of a grid over the table, let it settle, switch the thrusters off, and record it drifting again;
4. pucks off; fit the table's height surface and the air bearings' drag ([[calfit.fit_table|Console-calfit]]), save the run and the result.

A drift ends after its time, or at once (pucks off: the platform stops on its pucks) when the platform comes within the edge margin of an edge, within 0.45 m of another platform, or goes faster than the highest speed. A grid point is skipped when the straight path to it passes within 0.45 m of another platform.

**The thruster calibration** (`ThrusterRun`), with PhaseSpace streaming and the platform resting on the table. Before it starts, the page asks the person to confirm (or change) the mass, inertia and centre of gravity it assumes for the platform (`assumed_defaults`: the controller's defaults); the thrust is worked out from them. Then, at a clear place (the edge margin plus 0.25 m from the edges, flown there if needed), for each thruster 1–8 and each repeat:
1. float it (pucks on) where it rests, 0.5 s for the bearings to lift it;
2. fire the thruster at 100 % for the burn time (default 2 s), recording every PhaseSpace frame (the burn ends early near an edge);
3. fly it back to where it started (the diagram's controller, with the lab's calibrated thrust once there is one);
4. pucks off, and rest 1 s.

Each burn's frames from 0.15 s after the command (the link, the Teensy and the valve) to the end of the burn go to [[calfit.fit_burn|Console-calfit]] with the assumed mass properties and the table's pull (its calibration) taken out. A burn is left out when its frames jump, its motion doesn't follow a constant thrust (residual above 3 mm), or its thrust is implausible. Each thruster's thrust is the mean of its good burns (with their spread when there are two or more); one without a good burn keeps the nominal thrust and is marked (`valid`). The result (`thrusters.<PLATFORM>`: thrust, spread, how far each push is turned, torque ratio, each burn's path, the assumptions) goes into the lab's file; `controller_params` gives Interactive Mode and the calibrations' flights the calibrated thrust, and the projects' copies give it to simulations and experiments ([[spot.params.apply|spot-params-apply]]: `F_thrusters_<PLATFORM>`).

**The arm calibration** (`ArmRun`) runs the arm calibration script, [[calibrate_arm.py|Tool-calibrate-arm]], on the Chaser (it has the arm's U2D2), through its agent ([[spot_agent.sh|Hardware-Deploy-Scripts]]: `arm-cal-check`, `-receive`, `-start`, `-status`, `-stop`, `-data`):
1. **Check** what the script needs there (`arm_check`): python3, the Dynamixel SDK, the U2D2 at `/dev/ttyUSB0` and access to it, nothing else holding it (the latency timer is a warning only). A real run doesn't start without them.
2. Keep the Chaser **resting** ([[hardware.reserve|Console-hardware]]: nothing can float it; switching it off on the Hardware tab takes it back and stops the run).
3. **Send** the script (the console's own copy: `deploy_tx2.sh` puts it next to the console on the TX2), and **start** it (`--yes`) for the chosen joints and tests.
4. **Follow** it every 2 s (`arm-cal-status`): its `PROGRESS` lines become the steps, its log the page's; Stop, the take-back, or 45 minutes stop it (`arm-cal-stop`: it switches the servos' torque off and restores their registers).
5. **Collect** the run's files into `~/SPOTCalibration/<stamp>_arm_RED/` (`*.json` and `*.csv` only) and its fit (`arm_calibration.json`), with what the page draws (`arm_plots`: each joint's friction points as torque, and its position steps, thinned). The fit becomes the lab's `arm` section.

A **rehearsal** (`simulate`) runs the script's simulated servos instead: it needs none of the hardware and doesn't reserve the Chaser, and its result is shown but never becomes the lab's.

**Checking the data.** A calibration replaces the lab's, so a run whose data can't be trusted is not saved (its raw data still is, for a look): when more than 0.5 % of the frames jumped more than 20 mm from one frame to the next, or went back in time (PhaseSpace mis-tracking the platform, or a second stream on the console's port), or when the drifts don't follow the model at all (position residual above 20 mm: something else moved the platform). A residual above five times the cameras' noise (at least 1 mm) is kept, with a warning that the model doesn't describe the table well there (a plane on a curved table: a larger grid gives a surface).

**Safety.** A calibration drives one platform through the hardware driver ([[hardware.py|Console-hardware]]: `claim`, `drive`, `release`), with its own owner (`Run`, labelled "Calibration", which the Hardware tab shows as *Calibrating*). It can't start while an experiment or Interactive Mode runs, and they can't start while it runs. Everything on the platform goes off and the run stops when the tab's **Stop** (or Esc) is pressed, the Hardware tab switches the platform off (the owner is taken back), PhaseSpace loses it for 1 s or sees it off the table, the link to it is lost, or the console stops; and its Teensy switches everything off by itself when packets stop for 2 s.

## Function by function

**Lines 78–108.** The constants: the raw data folder, the largest file, how a flight moves its goal (`GOAL_SPEED`, `GOAL_TURN`), when a platform counts as arrived (within 0.10 m and 5°, slower than 1 cm/s, for 1.5 s: the diagram's LQR has no integral action, so on a slope it holds the platform a little downhill), the flight's time limit, how long PhaseSpace may lose it, the clearance from other platforms, the pause after the pucks go off, and the data checks.

**Lines 111–122.** `now_iso` (UTC), `stamp` (a run's folder name), `Abort` (a run stops; its message says why).

**Lines 125–182.** `validate`: a checked copy of a calibration. What the simulation reads is checked (the table's coefficients, centre and drag; each platform's eight thrusts, 0.01–2 N; an arm section must be calibrate_arm.py's format); the rest is what the pages show and is kept. A project's copy keeps `sync`.

**Lines 185–209.** `newest` (the newest section's time) and `sections_newer` (which of the lab's sections are newer than the project's).

**Lines 212–291.** `CalibrationStore`: `open`, `get`, `_write` (checks, writes atomically, publishes a `calibration` event), `set_section`, `take` (a project's copy becomes the lab's, keeping its `sync`), `set_sync`.

**Lines 296–331.** `Run`: one calibration under way, as the page shows it (its steps, the message, a log, the pose, goal and path, the finished drifts or burns, the arm's checks and the script's log, the result or the error).

**Lines 334–468.** `Calibrator`: one run at a time. `_check_free`; `start_table`; `start_thrusters` (refused without `acknowledged`; the assumed mass properties); `start_arm` (joints of 1–3, tests of the four, `simulate`; a real run refused without `acknowledged`; refused during an experiment, Interactive Mode or another run); `arm_check`; `_begin`; `stop`; `publish`.

**Lines 471–497.** `parse_platform`, `number`, `thin`.

**Lines 500–712.** `Driver`: reporting (`say`, `begin_step`, `end_step`); recording every PhaseSpace frame (`start_recording`, `stop_recording`); the platform (`claim`, `release`, `set`, `check` every step, `wait`); `others`, `path_clear`; `fly_to` (its own [[ControllerProcess|Console-interactive]] with the lab's calibrated thrust); `finish`.

**Lines 715–735.** `segment_distance`, `save_run`.

**Lines 738–747.** `grid_points`.

**Lines 750–909.** `TableRun`: `main`, `inside`, `drift`, `done`.

**Lines 912–947.** The thruster constants, `dist_from_cg`, `assumed_defaults`, `controller_params`.

**Lines 950–1127.** `ThrusterRun`: `main`, `start_place`, `burn`, `fit`, `result`, `done`.

**Lines 1132–1135.** The arm's tests and joint names, how often its run is followed, and the longest it may take.

**Lines 1138–1184.** `arm_script` (where the console has calibrate_arm.py) and `arm_check` (the Chaser's `arm-cal-check`, item by item with what to do about each; an agent older than this console says so).

**Lines 1187–1363.** `ArmRun`: `main` (check, reserve, send, start, follow, collect; the script is always stopped and the Chaser released when the run ends); `follow`; `progress` (the `PROGRESS` lines as the steps); `last_words`; `collect`; `done`.

**Lines 1366–1403.** `read_csv_text` and `arm_plots`.

**Lines 1406–1413.** `noise_mm`.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/calibration.py</b> (1413 lines; click to show)</summary>

```python
   1  """The Calibration tab: measuring the lab so that simulations behave as the experiments do, and the
   2  calibration file that holds the results.
   3  
   4  The file (one per console): the lab console keeps the lab's in its home (~/.spot_calibration.json),
   5  next to the raw data of every run (~/SPOTCalibration/<stamp>_<kind>/run.json); a project keeps a copy in
   6  resources/spot_calibration.json, which spot.params.apply reads (spot.params.loadCalibration) for the
   7  simulation. Whenever the local console sees the lab console and the lab's file is newer, its page
   8  offers to update the project's copy (which can be declined, or never asked again: the copy's "sync").
   9  
  10      {"format": "spot-calibration", "version": 1,
  11       "updated": "2026-10-13T15:02:11Z",                  (the newest section's; UTC, so it sorts as text)
  12       "table": {"updated", "by", "model": "quadratic" | "plane", "height": [a1 .. a5] (calfit.py),
  13                 "centre": [x, y], "damping": {"RED": c, ...}, "tilt_centre_mrad", ..., "drifts": [...]},
  14       "thrusters": {"RED": {"updated", "by", "thrust": [8 x N], ...}, ...},
  15       "arm": {... spot-arm-calibration ...},
  16       "sync": {"ask": true, "declined": null}}           (a project's copy only)
  17  
  18  The table slope calibration (TableRun): with PhaseSpace streaming and the platform resting on the table,
  19    1. record it sitting still (the cameras' noise);
  20    2. float it (pucks on, thrusters off) and record it drifting from where it is;
  21    3. fly it (the diagram's own controller, as Interactive Mode) to each point of a grid over the table,
  22       let it settle, switch the thrusters off and record it drifting again;
  23    4. pucks off, and fit the table's height surface and the air bearings' drag to the drifts (calfit.py).
  24  A drift ends after its time, or at once (pucks off: the platform stops on the table) when the platform
  25  nears an edge or another platform, or goes too fast.
  26  
  27  The thruster calibration (ThrusterRun), with PhaseSpace streaming and the platform resting on the table:
  28  for each thruster 1-8 (and each repeat),
  29    1. float it (pucks on) where it rests;
  30    2. fire the thruster at 100 % for the burn time, recording every PhaseSpace frame;
  31    3. fly it back (the diagram's own controller) to where it started;
  32    4. pucks off, and let it rest.
  33  Each burn's thrust comes from the motion it gave the platform (calfit.fit_burn), with the platform's
  34  mass, inertia and centre of gravity taken as known (the person starting it confirms them; the controller's
  35  defaults unless changed) and the table's pull (its calibration) taken out. The lab's calibrated thrust is
  36  then what Interactive Mode, the calibrations' own flights and (through the projects' copies) simulations
  37  and experiments use.
  38  
  39  The arm calibration (ArmRun) runs the arm calibration script (calibrate_arm.py, from the template's
  40  resources/arm_calibration; on the TX2 next to the console) on the Chaser, through its agent
  41  (spot_agent.sh: arm-cal-check, -receive, -start, -status, -stop, -data): it checks the Chaser has what the
  42  script needs (python3, the Dynamixel SDK, the U2D2 and access to it, nothing else holding it), keeps the
  43  Chaser resting (nothing can float it meanwhile), sends the script, starts it, follows its progress, and
  44  collects its data and fit. A rehearsal runs the script's simulated servos instead: it needs none of that
  45  hardware, and its result is shown but never becomes the lab's.
  46  
  47  A calibration drives one platform at a time, through the hardware driver (hardware.py: claim, drive,
  48  release), as Interactive Mode does: it can't start while an experiment or Interactive Mode runs, and
  49  they can't start while it runs. Everything goes off, and the run stops, when the Calibration tab's Stop
  50  (or Esc) is pressed, the Hardware tab switches the platform off, PhaseSpace loses it, it leaves the
  51  table, the link to it is lost, or the console stops. And its Teensy switches everything off by itself
  52  when packets stop for 2 s.
  53  
  54  Part of the SPOT Console (webapp/app.py starts it).
  55  """
  56  
  57  import collections
  58  import copy
  59  import io
  60  import json
  61  import math
  62  import re
  63  import tarfile
  64  import threading
  65  import time
  66  from pathlib import Path
  67  
  68  from . import calfit
  69  from .common import PROJECT_DIR, ApiError, decode, shown
  70  from .control import DEFAULTS
  71  from .events import hub
  72  from .experiment import experiment
  73  from .hardware import NUM_THRUSTERS, hardware, hw_log, platform_run
  74  from .interactive import EXPERIMENT_BUSY, TABLE, ControllerProcess, fresh_pose, interactive, wrap
  75  from .phasespace import monitor, streamer
  76  
  77  
  78  PLATFORMS = ("RED", "BLACK", "BLUE")
  79  SECTIONS = ("table", "thrusters", "arm")
  80  RUNS_DIR = Path.home() / "SPOTCalibration"   # the lab console's raw data of each run
  81  MAX_FILE = 4 * 1024 * 1024                   # [bytes] a calibration file is a few hundred kB at most
  82  
  83  # Flying between points (fly_to): the goal moves towards the point at these speeds, so the
  84  # controller isn't kicked by a jump in its goal (as a dragged goal in Interactive Mode moves)
  85  GOAL_SPEED = 0.10            # [m/s]
  86  GOAL_TURN = math.radians(15)  # [rad/s]
  87  # There: within SETTLED_POS of the point and SETTLED_ATT of its attitude, and slower than SETTLED_SPEED,
  88  # for SETTLED_TIME. Only roughly at the point: the diagram's LQR has no integral action, so on a sloping
  89  # table it holds the platform a few centimetres downhill of its goal; what matters is that it is at rest.
  90  SETTLED_POS = 0.10           # [m]
  91  SETTLED_ATT = math.radians(5)
  92  SETTLED_SPEED = 0.01         # [m/s]
  93  SETTLED_TIME = 1.5           # [s]
  94  FLY_TIMEOUT = 90.0           # [s] to reach a point
  95  LOST_TIMEOUT = 1.0           # [s] PhaseSpace may lose the platform this long (thrusters off meanwhile)
  96  CLEARANCE = 0.45             # [m] from other platforms (centre to centre), as Interactive Mode's goals
  97  STOP_PAUSE = 2.0             # [s] after the pucks go off to end a drift: the platform comes to rest
  98  # The data's own check: from one frame to the next a drifting platform moves at most a few mm, so a
  99  # bigger jump is a glitch (PhaseSpace mis-tracking it, or a second stream on the monitor's port); with
 100  # more than GLITCH_FRACTION of them, the run is not saved
 101  GLITCH_STEP = 0.02           # [m]
 102  GLITCH_FRACTION = 0.005
 103  # The fit's: a position residual above POOR_FIT_FACTOR times the cameras' noise (at least POOR_FIT_FLOOR)
 104  # means the model doesn't describe the table well (a plane on a curved table: more grid points give a
 105  # surface), which the result says; above REJECT_RESIDUAL something else moved the platform: not saved
 106  POOR_FIT_FACTOR = 5.0
 107  POOR_FIT_FLOOR = 1.0         # [mm]
 108  REJECT_RESIDUAL = 20.0       # [mm]
 109  
 110  
 111  def now_iso():
 112      return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
 113  
 114  
 115  def stamp():
 116      return time.strftime("%Y%m%d_%H%M%S")
 117  
 118  
 119  class Abort(Exception):
 120      """A calibration run stops (the message says why)."""
 121  
 122  
 123  # ---------------------------------------------------------------------------- the file
 124  
 125  def _finite(v, what):
 126      if not isinstance(v, (int, float)) or isinstance(v, bool) or not math.isfinite(v):
 127          raise ApiError("%s must be a finite number." % what)
 128      return float(v)
 129  
 130  
 131  def _numbers(v, n, what):
 132      if not isinstance(v, list) or len(v) != n:
 133          raise ApiError("%s must be a list of %d numbers." % (what, n))
 134      return [_finite(x, what) for x in v]
 135  
 136  
 137  def validate(cal, local=False):
 138      """A checked copy of the calibration CAL: the values the simulation reads are checked (the rest
 139      is what the pages show). LOCAL: a project's copy (it keeps "sync")."""
 140      if not isinstance(cal, dict):
 141          raise ApiError("The calibration must be an object.")
 142      if cal.get("format", "spot-calibration") != "spot-calibration":
 143          raise ApiError("This is not a SPOT calibration file.")
 144      out = {"format": "spot-calibration", "version": 1}
 145      table = cal.get("table")
 146      if table is not None:
 147          if not isinstance(table, dict):
 148              raise ApiError("calibration.table must be an object.")
 149          t = copy.deepcopy(table)
 150          t["height"] = _numbers(table.get("height"), 5, "table.height")
 151          t["centre"] = _numbers(table.get("centre", list(calfit.CENTRE)), 2, "table.centre")
 152          damping = table.get("damping") or {}
 153          if not isinstance(damping, dict) or any(k not in PLATFORMS for k in damping):
 154              raise ApiError("table.damping must map RED, BLACK, BLUE to numbers.")
 155          t["damping"] = dict((k, _finite(v, "table.damping.%s" % k)) for k, v in damping.items())
 156          if t.get("model") not in ("quadratic", "plane"):
 157              raise ApiError("table.model must be quadratic or plane.")
 158          out["table"] = t
 159      thrusters = cal.get("thrusters")
 160      if thrusters is not None:
 161          if not isinstance(thrusters, dict) or any(k not in PLATFORMS for k in thrusters):
 162              raise ApiError("calibration.thrusters must map RED, BLACK, BLUE to their calibration.")
 163          out["thrusters"] = {}
 164          for k, v in thrusters.items():
 165              if not isinstance(v, dict):
 166                  raise ApiError("thrusters.%s must be an object." % k)
 167              c = copy.deepcopy(v)
 168              c["thrust"] = _numbers(v.get("thrust"), NUM_THRUSTERS, "thrusters.%s.thrust" % k)
 169              if not all(0.01 <= f <= 2.0 for f in c["thrust"]):
 170                  raise ApiError("thrusters.%s.thrust must be 0.01 to 2 N each." % k)
 171              out["thrusters"][k] = c
 172      arm = cal.get("arm")
 173      if arm is not None:
 174          if not isinstance(arm, dict) or arm.get("format") != "spot-arm-calibration":
 175              raise ApiError("calibration.arm must be an arm calibration (calibrate_arm.py's).")
 176          out["arm"] = copy.deepcopy(arm)
 177      out["updated"] = newest(out)
 178      if local:
 179          sync = cal.get("sync") or {}
 180          out["sync"] = {"ask": bool(sync.get("ask", True)),
 181                         "declined": sync.get("declined") if isinstance(sync.get("declined"), str) else None}
 182      return out
 183  
 184  
 185  def newest(cal):
 186      """The newest section's time (None for an empty calibration)."""
 187      times = [cal["table"].get("updated")] if cal.get("table") else []
 188      times += [v.get("updated") for v in (cal.get("thrusters") or {}).values()]
 189      if cal.get("arm"):
 190          times.append(cal["arm"].get("updated"))
 191      times = [t for t in times if isinstance(t, str)]
 192      return max(times) if times else None
 193  
 194  
 195  def sections_newer(lab, mine):
 196      """Which of the lab's sections are newer than (or missing from) this project's: ["table",
 197      "thrusters RED", "arm", ...]."""
 198      out = []
 199      lt, mt = lab.get("table"), mine.get("table")
 200      if lt and (not mt or lt.get("updated", "") > mt.get("updated", "")):
 201          out.append("table")
 202      for name, c in sorted((lab.get("thrusters") or {}).items()):
 203          m = (mine.get("thrusters") or {}).get(name)
 204          if not m or c.get("updated", "") > m.get("updated", ""):
 205              out.append("thrusters " + name)
 206      la, ma = lab.get("arm"), mine.get("arm")
 207      if la and (not ma or la.get("updated", "") > ma.get("updated", "")):
 208          out.append("arm")
 209      return out
 210  
 211  
 212  class CalibrationStore:
 213      """This console's calibration file (as design.py's DesignStore keeps the design)."""
 214  
 215      def __init__(self):
 216          self._lock = threading.Lock()
 217          self.path = None
 218          self.local = False
 219          self.cal = validate({})
 220          self.saved = False
 221  
 222      def open(self, path, local=False):
 223          self.path, self.local = Path(path), local
 224          self.cal = validate({}, local)
 225          try:
 226              text = self.path.read_text(encoding="utf-8")
 227              self.cal = validate(json.loads(text), local)
 228              self.saved = True
 229          except FileNotFoundError:
 230              pass
 231          except (OSError, ValueError, ApiError) as e:
 232              hub.log("hardware", "The calibration file %s could not be read (%s); no calibration is used." % (self.path, e))
 233  
 234      def get(self):
 235          with self._lock:
 236              return {"calibration": copy.deepcopy(self.cal), "path": str(self.path) if self.path else None,
 237                      "saved": self.saved}
 238  
 239      def _write(self, cal):
 240          """Save CAL (checked) and tell the pages (call without the lock)."""
 241          cal = validate(cal, self.local)
 242          data = json.dumps(cal, indent=1, allow_nan=False) + "\n"
 243          if len(data) > MAX_FILE:
 244              raise ApiError("The calibration would be %d kB; at most %d kB." % (len(data) // 1024, MAX_FILE // 1024))
 245          with self._lock:
 246              self.cal = cal
 247              if self.path:
 248                  self.path.parent.mkdir(parents=True, exist_ok=True)
 249                  tmp = self.path.with_name(self.path.name + ".tmp")
 250                  tmp.write_text(data, encoding="utf-8")
 251                  tmp.replace(self.path)
 252                  self.saved = True
 253          hub.publish("calibration", self.get())
 254          return self.get()
 255  
 256      def set_section(self, section, value, platform=None):
 257          """Replace one section (a platform's, for thrusters) with VALUE (None: remove it)."""
 258          with self._lock:
 259              cal = copy.deepcopy(self.cal)
 260          if section == "thrusters":
 261              th = cal.get("thrusters") or {}
 262              if value is None:
 263                  th.pop(platform, None)
 264              else:
 265                  th[platform] = value
 266              cal["thrusters"] = th or None
 267          else:
 268              cal[section] = value
 269          cal = dict((k, v) for k, v in cal.items() if v is not None)
 270          return self._write(cal)
 271  
 272      def take(self, lab_cal):
 273          """A project's copy becomes the lab's (keeping its own "sync")."""
 274          with self._lock:
 275              sync = copy.deepcopy(self.cal.get("sync"))
 276          cal = dict(validate(lab_cal), sync=sync)
 277          return self._write(cal)
 278  
 279      def set_sync(self, ask=None, declined=None):
 280          with self._lock:
 281              cal = copy.deepcopy(self.cal)
 282          sync = cal.get("sync") or {"ask": True, "declined": None}
 283          if ask is not None:
 284              sync["ask"] = bool(ask)
 285          if declined is not None:
 286              sync["declined"] = str(declined)
 287          cal["sync"] = sync
 288          return self._write(cal)
 289  
 290  
 291  store = CalibrationStore()
 292  
 293  
 294  # ---------------------------------------------------------------------------- a run
 295  
 296  class Run:
 297      """One calibration being run: what the page shows of it."""
 298  
 299      label = "Calibration"   # what the hardware tab says drives the platform
 300  
 301      def __init__(self, kind, platform, params, requester):
 302          self.kind = kind
 303          self.platform = platform
 304          self.params = params
 305          self.by = requester
 306          self.started = time.time()
 307          self.stamp = stamp()
 308          self.state = "running"            # running, stopping, done, failed, stopped
 309          self.step = None                  # the step under way (an index into steps)
 310          self.steps = []                   # [{"name", "state": waiting/running/done/skipped/failed, "note"}]
 311          self.message = "Starting…"
 312          self.log = collections.deque(maxlen=200)
 313          self.stop_reason = None
 314          self.pose = None                  # [x, y, theta] now
 315          self.goal = None                  # where it is flying to
 316          self.path = []                    # the current drift or burn, thinned: [[x, y], ...]
 317          self.records = []                 # finished drifts or burns (thinned, for the page)
 318          self.result = None
 319          self.error = None
 320          self.finished = None
 321          self.recording = None             # frames being recorded: [[t, x, y, theta], ...]
 322          self.controller = None            # ControllerProcess while it flies
 323  
 324      def status(self):
 325          return {"kind": self.kind, "platform": self.platform, "params": self.params, "by": self.by,
 326                  "started": self.started, "state": self.state, "step": self.step, "steps": self.steps,
 327                  "message": self.message, "log": list(self.log), "pose": self.pose, "goal": self.goal,
 328                  "path": self.path[-400:], "records": self.records, "result": self.result,
 329                  "points": getattr(self, "points", None),
 330                  "check": getattr(self, "check", None), "armlog": getattr(self, "armlog", [])[-40:],
 331                  "error": self.error, "finished": self.finished}
 332  
 333  
 334  class Calibrator:
 335      """Runs one calibration at a time on the lab console."""
 336  
 337      def __init__(self):
 338          self._lock = threading.Lock()
 339          self.run = None        # the run under way, or the last one
 340          self._published = 0.0
 341  
 342      def active(self):
 343          with self._lock:
 344              return self.run is not None and self.run.state in ("running", "stopping")
 345  
 346      def status(self):
 347          with self._lock:
 348              return {"active": self.run is not None and self.run.state in ("running", "stopping"),
 349                      "run": self.run.status() if self.run else None}
 350  
 351      def publish(self, force=True):
 352          now = time.monotonic()
 353          if not force and now - self._published < 0.2:
 354              return
 355          self._published = now
 356          hub.publish("calibration_run", self.status())
 357  
 358      # ---- requests
 359  
 360      def _check_free(self, platform):
 361          if not streamer.status()["running"]:
 362              raise ApiError("Start PhaseSpace streaming first (PhaseSpace tab): the calibration measures the "
 363                             "platform's motion with it.")
 364          if experiment.status()["phase"] in EXPERIMENT_BUSY:
 365              raise ApiError("An experiment is under way; calibrate once it has stopped.")
 366          if interactive.status()["active"]:
 367              raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
 368          if self.active():
 369              raise ApiError("A calibration is already running; stop it first.")
 370          if fresh_pose(platform, time.time()) is None:
 371              raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)." % shown(platform))
 372  
 373      def start_table(self, body, requester):
 374          platform = parse_platform(body.get("platform"))
 375          params = {
 376              "nx": int(number(body.get("nx", 3), "Grid columns", 1, 5)),
 377              "ny": int(number(body.get("ny", 2), "Grid rows", 1, 4)),
 378              "drift": number(body.get("drift", 12), "Drift time", 4, 40),
 379              "margin": number(body.get("margin", 0.35), "Edge margin", 0.25, 0.8),
 380              "max_speed": number(body.get("max_speed", 0.15), "Highest speed", 0.03, 0.3),
 381              "model": str(body.get("model") or "auto"),
 382          }
 383          if params["model"] not in ("auto", "quadratic", "plane"):
 384              raise ApiError("model must be auto, quadratic or plane.")
 385          self._check_free(platform)
 386          run = Run("table", platform, params, requester)
 387          self._begin(run, TableRun(self, run).main)
 388          return self.status()
 389  
 390      def start_thrusters(self, body, requester):
 391          platform = parse_platform(body.get("platform"))
 392          if body.get("acknowledged") is not True:
 393              raise ApiError("Confirm first that the platform's mass, inertia and centre of gravity are right: the "
 394                             "thrust is worked out from them.")
 395          defaults = assumed_defaults()[platform]
 396          a = body.get("assumed") or {}
 397          cg = a.get("cg_mm", defaults["cg_mm"])
 398          if not isinstance(cg, list) or len(cg) != 2:
 399              raise ApiError("cg_mm must be [x, y] in mm.")
 400          assumed = {"mass": number(a.get("mass", defaults["mass"]), "Mass", 1, 50),
 401                     "inertia": number(a.get("inertia", defaults["inertia"]), "Inertia", 0.01, 5),
 402                     "cg_mm": [number(cg[0], "CG x", -60, 60), number(cg[1], "CG y", -60, 60)]}
 403          assumed["dist2CG"] = dist_from_cg(assumed["cg_mm"])
 404          assumed["changed"] = any(abs(assumed[k] - defaults[k]) > 1e-9 for k in ("mass", "inertia")) or \
 405              any(abs(u - v) > 1e-9 for u, v in zip(assumed["cg_mm"], defaults["cg_mm"]))
 406          params = {
 407              "repeats": int(number(body.get("repeats", 2), "Repeats", 1, 3)),
 408              "burn": number(body.get("burn", 2.0), "Burn time", 0.5, 3.0),
 409              "margin": number(body.get("margin", 0.35), "Edge margin", 0.25, 0.8),
 410              "assumed": assumed,
 411          }
 412          self._check_free(platform)
 413          run = Run("thrusters", platform, params, requester)
 414          self._begin(run, ThrusterRun(self, run).main)
 415          return self.status()
 416  
 417      def start_arm(self, body, requester):
 418          joints = body.get("joints", [1, 2, 3])
 419          tests = body.get("tests", list(ARM_TESTS))
 420          if not isinstance(joints, list) or not joints or any(j not in (1, 2, 3) for j in joints) or len(set(joints)) != len(joints):
 421              raise ApiError("joints must be some of 1 (shoulder), 2 (elbow), 3 (wrist).")
 422          if not isinstance(tests, list) or not tests or any(t not in ARM_TESTS for t in tests) or len(set(tests)) != len(tests):
 423              raise ApiError("tests must be some of %s." % ", ".join(ARM_TESTS))
 424          simulate = body.get("simulate") is True
 425          if not simulate and body.get("acknowledged") is not True:
 426              raise ApiError("Confirm first that the Chaser rests on the table and its arm is clear to move.")
 427          if experiment.status()["phase"] in EXPERIMENT_BUSY:
 428              raise ApiError("An experiment is under way; calibrate once it has stopped.")
 429          if interactive.status()["active"]:
 430              raise ApiError("Interactive Mode is flying; stop it first (PhaseSpace tab).")
 431          if self.active():
 432              raise ApiError("A calibration is already running; stop it first.")
 433          params = {"joints": sorted(joints), "tests": [t for t in ARM_TESTS if t in tests], "simulate": simulate}
 434          run = Run("arm", "RED", params, requester)
 435          self._begin(run, ArmRun(self, run).main)
 436          return self.status()
 437  
 438      def arm_check(self):
 439          """What the Chaser has for the arm calibration (arm-cal-check), without starting anything."""
 440          return arm_check()
 441  
 442      def _begin(self, run, main):
 443          with self._lock:
 444              if self.run is not None and self.run.state in ("running", "stopping"):
 445                  raise ApiError("A calibration is already running; stop it first.")
 446              self.run = run
 447          threading.Thread(target=main, daemon=True).start()
 448          self.publish()
 449  
 450      def stop(self, requester, reason=None):
 451          with self._lock:
 452              run = self.run
 453              if run is None or run.state not in ("running", "stopping"):
 454                  return self.status_unlocked()
 455              run.state = "stopping"
 456              run.stop_reason = reason or "stopped by %s" % requester
 457          hardware.get(run.platform).release(run, "Calibration stopped (%s); all off" % run.stop_reason)
 458          self.publish()
 459          return self.status()
 460  
 461      def status_unlocked(self):
 462          return {"active": False, "run": self.run.status() if self.run else None}
 463  
 464      def shutdown(self):
 465          self.stop("the console", "the console is shutting down")
 466  
 467  
 468  calibrator = Calibrator()
 469  
 470  
 471  def parse_platform(name):
 472      name = str(name or "").strip().upper()
 473      if name not in PLATFORMS:
 474          raise ApiError("Choose the platform: RED, BLACK or BLUE.")
 475      return name
 476  
 477  
 478  def number(value, what, low, high):
 479      try:
 480          v = float(value)
 481      except (TypeError, ValueError):
 482          raise ApiError("%s must be a number." % what)
 483      if not (math.isfinite(v) and low <= v <= high):
 484          raise ApiError("%s must be %g to %g." % (what, low, high))
 485      return v
 486  
 487  
 488  def thin(points, n):
 489      """At most N of POINTS, evenly spread, the last one kept."""
 490      if len(points) <= n:
 491          return list(points)
 492      step = len(points) / float(n)
 493      out = [points[int(i * step)] for i in range(n)]
 494      out[-1] = points[-1]
 495      return out
 496  
 497  
 498  # ---------------------------------------------------------------------------- driving a platform
 499  
 500  class Driver:
 501      """What every calibration does with its platform: records PhaseSpace, floats it, flies it to a
 502      point with the diagram's controller, and stops when it must. Every wait goes through tick(),
 503      which keeps the platform's packets going and raises Abort when the run must stop."""
 504  
 505      def __init__(self, cal, run):
 506          self.cal = cal
 507          self.run = run
 508          self.name = run.platform
 509          self.hw = hardware.get(run.platform)
 510          self.period = 1.0 / DEFAULTS["sample_rate"]
 511          self.duty = [0.0] * NUM_THRUSTERS
 512          self.pucks = False
 513          self.claimed = False
 514          self.lost_since = None
 515          self.backwards = 0      # frames recorded whose time went backwards (see _on_frame)
 516  
 517      # ---- reporting
 518  
 519      def say(self, message, log=True):
 520          self.run.message = message
 521          if log:
 522              self.run.log.append("%s  %s" % (time.strftime("%H:%M:%S"), message))
 523              hw_log("%s: calibration: %s" % (shown(self.name), message))
 524          self.cal.publish(force=False)
 525  
 526      def begin_step(self, index, note=None):
 527          r = self.run
 528          if r.step is not None and r.steps[r.step]["state"] == "running":
 529              r.steps[r.step]["state"] = "done"
 530          r.step = index
 531          r.steps[index]["state"] = "running"
 532          if note:
 533              r.steps[index]["note"] = note
 534          self.cal.publish()
 535  
 536      def end_step(self, state="done", note=None):
 537          r = self.run
 538          if r.step is not None:
 539              r.steps[r.step]["state"] = state
 540              if note:
 541                  r.steps[r.step]["note"] = note
 542          self.cal.publish()
 543  
 544      # ---- recording every PhaseSpace frame
 545  
 546      def _on_frame(self, frame):
 547          rec = self.run.recording
 548          c = frame["crafts"].get(self.name)
 549          if rec is None or not c or not c["tracked"] or c["x"] is None or c["y"] is None or c["att"] is None:
 550              return
 551          t = frame["t"] if frame.get("t") is not None else frame["received"]
 552          if rec and t <= rec[-1][0]:
 553              self.backwards += 1   # PhaseSpace's time went back: a second stream on the monitor's port?
 554              return
 555          rec.append([t, c["x"], c["y"], c["att"], frame["received"]])
 556  
 557      def start_recording(self):
 558          self.run.recording = []
 559          self.run.path = []
 560          monitor.add_listener(self._on_frame)
 561  
 562      def stop_recording(self):
 563          monitor.remove_listener(self._on_frame)
 564          rec, self.run.recording = self.run.recording or [], None
 565          return rec
 566  
 567      # ---- the platform
 568  
 569      def claim(self):
 570          self.hw.claim(self.run, self.run.by)   # raises ApiError if it can't be driven now
 571          self.claimed = True
 572          self.pucks = True
 573  
 574      def release(self, note):
 575          if self.claimed:
 576              self.hw.release(self.run, note)
 577              self.claimed = False
 578  
 579      def set(self, duty=None, pucks=None):
 580          if duty is not None:
 581              self.duty = list(duty)
 582          if pucks is not None:
 583              self.pucks = pucks
 584  
 585      def check(self):
 586          """Raise Abort if the run must stop; returns the platform's pose (None while lost)."""
 587          r = self.run
 588          if r.state != "running":
 589              raise Abort(r.stop_reason or "stopped")
 590          now = time.time()
 591          pose = fresh_pose(self.name, now)
 592          if pose is None:
 593              self.lost_since = self.lost_since or now
 594              if now - self.lost_since > LOST_TIMEOUT:
 595                  raise Abort("PhaseSpace lost the %s for more than %g s" % (shown(self.name), LOST_TIMEOUT))
 596              self.duty = [0.0] * NUM_THRUSTERS
 597          else:
 598              self.lost_since = None
 599              if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
 600                  raise Abort("PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)" % (shown(self.name), pose[0], pose[1]))
 601              r.pose = [pose[0], pose[1], math.degrees(pose[2])]
 602              if r.recording is not None:
 603                  if not r.path or math.hypot(pose[0] - r.path[-1][0], pose[1] - r.path[-1][1]) > 0.003:
 604                      r.path.append([round(pose[0], 4), round(pose[1], 4)])
 605          if self.claimed and not self.hw.drive(r, self.duty, self.pucks):
 606              la = self.hw.status()["last_action"]
 607              raise Abort("the %s was taken back (%s)" % (shown(self.name), la["text"] if la else "switched off"))
 608          self.cal.publish(force=False)
 609          return pose
 610  
 611      def wait(self, seconds, until=None):
 612          """Keep the platform going for SECONDS (or until UNTIL(pose) is true); returns the last pose."""
 613          end = time.monotonic() + seconds
 614          pose = None
 615          while True:
 616              pose = self.check()
 617              if until is not None and pose is not None and until(pose):
 618                  return pose
 619              left = end - time.monotonic()
 620              if left <= 0:
 621                  return pose
 622              time.sleep(min(self.period, left))
 623  
 624      def others(self):
 625          """The other platforms PhaseSpace sees: {NAME: [x, y]}."""
 626          now, out = time.time(), {}
 627          for n in PLATFORMS:
 628              if n != self.name:
 629                  p = fresh_pose(n, now)
 630                  if p is not None:
 631                      out[n] = p[:2]
 632          return out
 633  
 634      def path_clear(self, a, b):
 635          """Whether the straight path from A to B keeps CLEARANCE from the other platforms; else which one."""
 636          for n, p in self.others().items():
 637              if segment_distance(p, a, b) < CLEARANCE:
 638                  return n
 639          return None
 640  
 641      def fly_to(self, goal):
 642          """Fly the platform (pucks on) to GOAL = [x, y, theta] with the diagram's controller, the goal
 643          moving there gradually, until it has settled there."""
 644          r = self.run
 645          if r.controller is None:
 646              r.controller = ControllerProcess(self.name, self.controller_params())
 647              r.controller.wait_ready()
 648          pose = self.check()
 649          if pose is None:
 650              raise Abort("PhaseSpace doesn't see the %s" % shown(self.name))
 651          target = [goal[0], goal[1], pose[2] + wrap(goal[2] - pose[2])]
 652          current = [pose[0], pose[1], pose[2]]
 653          r.goal = [goal[0], goal[1], math.degrees(goal[2])]
 654          self.pucks = True
 655          started, settled_since = time.monotonic(), None
 656          next_step = time.monotonic()
 657          while True:
 658              # the goal moves towards the target
 659              d = math.hypot(target[0] - current[0], target[1] - current[1])
 660              step = GOAL_SPEED * self.period
 661              if d > step:
 662                  current[0] += (target[0] - current[0]) * step / d
 663                  current[1] += (target[1] - current[1]) * step / d
 664              else:
 665                  current[0], current[1] = target[0], target[1]
 666              dth, turn = target[2] - current[2], GOAL_TURN * self.period
 667              current[2] = target[2] if abs(dth) <= turn else current[2] + math.copysign(turn, dth)
 668              pose = fresh_pose(self.name, time.time())
 669              if pose is not None:
 670                  _u, duty, _b = r.controller.step(pose, list(current))
 671                  self.duty = [100 * v for v in duty]
 672              pose = self.check()
 673              if pose is not None and current == target:
 674                  close = (math.hypot(pose[0] - target[0], pose[1] - target[1]) < SETTLED_POS and
 675                           abs(wrap(pose[2] - target[2])) < SETTLED_ATT and
 676                           math.hypot(pose[3], pose[4]) < SETTLED_SPEED and abs(pose[5]) < math.radians(2))
 677                  if close:
 678                      settled_since = settled_since or time.monotonic()
 679                      if time.monotonic() - settled_since >= SETTLED_TIME:
 680                          self.duty = [0.0] * NUM_THRUSTERS
 681                          r.goal = None
 682                          return pose
 683                  else:
 684                      settled_since = None
 685              if time.monotonic() - started > FLY_TIMEOUT:
 686                  why = ""
 687                  if pose is not None:
 688                      why = " (it is %.0f mm and %.1f deg from it, moving at %.0f mm/s and %.1f deg/s)" % (
 689                          1000 * math.hypot(pose[0] - target[0], pose[1] - target[1]), math.degrees(abs(wrap(pose[2] - target[2]))),
 690                          1000 * math.hypot(pose[3], pose[4]), math.degrees(abs(pose[5])))
 691                  raise Abort("the %s didn't settle at (%.2f m, %.2f m) within %g s%s"
 692                              % (shown(self.name), goal[0], goal[1], FLY_TIMEOUT, why))
 693              next_step += self.period
 694              delay = next_step - time.monotonic()
 695              if delay > 0:
 696                  time.sleep(delay)
 697              else:
 698                  next_step = time.monotonic()
 699  
 700      def controller_params(self):
 701          """The controller's parameters: SPOT's defaults, with the lab's calibrated thrust."""
 702          return controller_params(self.name)
 703  
 704      def finish(self, ok, note):
 705          r = self.run
 706          if r.recording is not None:
 707              self.stop_recording()
 708          self.release(note)
 709          if r.controller is not None:
 710              r.controller.close()
 711              r.controller = None
 712          r.goal = None
 713  
 714  
 715  def segment_distance(p, a, b):
 716      """The distance from point P to the segment AB."""
 717      ax, ay, bx, by = a[0], a[1], b[0], b[1]
 718      dx, dy = bx - ax, by - ay
 719      L = dx * dx + dy * dy
 720      s = 0.0 if L == 0 else max(0.0, min(1.0, ((p[0] - ax) * dx + (p[1] - ay) * dy) / L))
 721      return math.hypot(p[0] - (ax + s * dx), p[1] - (ay + s * dy))
 722  
 723  
 724  def save_run(run, data):
 725      """The raw data of a run, kept on the lab console (~/SPOTCalibration/<stamp>_<kind>/run.json)."""
 726      try:
 727          folder = RUNS_DIR / ("%s_%s_%s" % (run.stamp, run.kind, run.platform))
 728          folder.mkdir(parents=True, exist_ok=True)
 729          (folder / "run.json").write_text(json.dumps(data, allow_nan=False) + "\n", encoding="utf-8")
 730          return str(folder)
 731      except (OSError, ValueError) as e:
 732          hw_log("calibration: could not save the run's data (%s)" % e)
 733          return None
 734  
 735  
 736  # ---------------------------------------------------------------------------- the table's slope
 737  
 738  def grid_points(nx, ny, margin):
 739      """The grid of start points: NX x NY, spread over the table inside MARGIN + 0.35 m, in a snake."""
 740      inset = margin + 0.35
 741      xs = [TABLE[0] / 2] if nx == 1 else [inset + (TABLE[0] - 2 * inset) * i / (nx - 1) for i in range(nx)]
 742      ys = [TABLE[1] / 2] if ny == 1 else [inset + (TABLE[1] - 2 * inset) * j / (ny - 1) for j in range(ny)]
 743      out = []
 744      for j, y in enumerate(ys):
 745          row = xs if j % 2 == 0 else list(reversed(xs))
 746          out.extend([x, y] for x in row)
 747      return out
 748  
 749  
 750  class TableRun:
 751      def __init__(self, cal, run):
 752          self.cal = cal
 753          self.run = run
 754          self.d = Driver(cal, run)
 755  
 756      def main(self):
 757          r, d, p = self.run, self.d, self.run.params
 758          points = grid_points(p["nx"], p["ny"], p["margin"])
 759          r.steps = ([{"name": "Still", "state": "waiting", "note": "Noise while resting"},
 760                      {"name": "Float Here", "state": "waiting", "note": "Drift from where it is"}] +
 761                     [{"name": "Point %d" % (i + 1), "state": "waiting", "note": "(%.2f, %.2f) m" % tuple(pt)}
 762                      for i, pt in enumerate(points)] +
 763                     [{"name": "Fit", "state": "waiting", "note": ""}])
 764          r.points = points
 765          drifts, still = [], None
 766          ok, note = False, None
 767          try:
 768              # 1. resting: the cameras' noise
 769              d.begin_step(0)
 770              d.say("Recording the %s resting on the table (2 s)." % shown(r.platform))
 771              d.start_recording()
 772              d.wait(2.0)
 773              still = d.stop_recording()
 774              noise = noise_mm(still)
 775              d.end_step(note="%.2f mm" % noise if noise is not None else "no data")
 776              # 2. float it, and let it drift from where it is
 777              d.begin_step(1)
 778              d.claim()
 779              pose = d.check()
 780              if pose is not None and self.inside(pose, extra=0.1):
 781                  drifts.append(self.drift(1))
 782              else:
 783                  d.end_step("skipped", "too close to an edge or a platform")
 784              # 3. each point of the grid
 785              for i, pt in enumerate(points):
 786                  d.begin_step(2 + i)
 787                  pose = d.check()
 788                  if pose is None:
 789                      raise Abort("PhaseSpace doesn't see the %s" % shown(r.platform))
 790                  blocker = d.path_clear(pose[:2], pt)
 791                  if blocker:
 792                      d.say("Point %d skipped: the path passes the %s." % (i + 1, shown(blocker)))
 793                      d.end_step("skipped", "path passes the %s" % shown(blocker))
 794                      continue
 795                  d.say("Flying to point %d (%.2f m, %.2f m)." % (i + 1, pt[0], pt[1]))
 796                  d.fly_to([pt[0], pt[1], pose[2]])
 797                  drifts.append(self.drift(2 + i))
 798              d.finish(True, "Calibration finished; all off")
 799              # 4. fit
 800              d.begin_step(len(r.steps) - 1)
 801              d.say("Fitting the table's slope to %d drifts." % len(drifts))
 802              records = [x for x in drifts if x and len(x["t"]) >= 20]
 803              if not records:
 804                  raise Abort("no drift was long enough to fit")
 805              samples = sum(len(x["t"]) for x in records)
 806              jumps = sum(x["glitches"] for x in records)
 807              if jumps + d.backwards > GLITCH_FRACTION * samples:
 808                  save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still, "drifts": drifts,
 809                               "rejected": "glitches"})
 810                  raise Abort("PhaseSpace's data isn't consistent: %d of %d positions jumped, and %d frames went back in time "
 811                              "(mis-tracking, or a second stream on the console's port?); the lab's calibration was not "
 812                              "changed" % (jumps, samples, d.backwards))
 813              model = None if p["model"] == "auto" else p["model"]
 814              fit = calfit.fit_table(records, model)
 815              noise = noise_mm(still)
 816              if fit["rms_mm"] > REJECT_RESIDUAL:
 817                  save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still,
 818                               "drifts": drifts, "rejected_fit": fit})
 819                  raise Abort("the drifts don't follow the model at all (position residual %.1f mm): something else moved "
 820                              "the platform; the lab's calibration was not changed" % fit["rms_mm"])
 821              if fit["rms_mm"] > max(POOR_FIT_FLOOR, POOR_FIT_FACTOR * (noise or 0.2)):
 822                  fit["warning"] = ("The drifts follow the %s only to %.1f mm (the cameras' noise is %.2f mm): the table "
 823                                    "isn't that simple here.%s" % (fit["model"], fit["rms_mm"], noise or 0.0,
 824                                    " A larger grid (3 x 2 or more) fits a surface." if fit["model"] == "plane" else ""))
 825              result = dict(fit, updated=now_iso(), by=r.by, platform=r.platform, noise_mm=noise_mm(still),
 826                            run=r.stamp, params=p,
 827                            paths=[{"platform": x["platform"], "cut": x["cut"],
 828                                    "points": [[round(a, 4), round(b, 4)] for a, b in thin(list(zip(x["x"], x["y"])), 150)]}
 829                                   for x in records])
 830              folder = save_run(r, {"kind": "table", "platform": r.platform, "params": p, "still": still,
 831                                    "drifts": drifts, "fit": result})
 832              result["folder"] = folder
 833              store.set_section("table", result)
 834              r.result = result
 835              d.end_step(note="%s, %.2f mrad at the centre" % (fit["model"], fit["tilt_centre_mrad"]))
 836              d.say("Done: the table tilts %.2f mrad at its centre (%.2f mrad at most), downhill towards %.0f deg; "
 837                    "position residual %.2f mm.%s" % (fit["tilt_centre_mrad"], fit["tilt_max_mrad"], fit["downhill_deg"],
 838                                                      fit["rms_mm"], " " + fit["warning"] if fit.get("warning") else ""))
 839              ok = True
 840          except Abort as e:
 841              note = str(e)
 842          except ApiError as e:
 843              note = str(e)
 844          except Exception as e:   # never leave the platform floating after a bug
 845              note = "internal error: %s: %s" % (type(e).__name__, e)
 846          finally:
 847              d.finish(ok, "Calibration %s; all off" % ("finished" if ok else "stopped (%s)" % note))
 848          self.done(ok, note, drifts, still)
 849  
 850      def inside(self, pose, extra=0.0):
 851          """Whether the platform is clear of the edges (the margin) and the other platforms."""
 852          m = self.run.params["margin"] + extra
 853          if not (m <= pose[0] <= TABLE[0] - m and m <= pose[1] <= TABLE[1] - m):
 854              return False
 855          return all(math.hypot(pose[0] - o[0], pose[1] - o[1]) >= CLEARANCE for o in self.d.others().values())
 856  
 857      def drift(self, step):
 858          """Thrusters off, pucks on: record the platform drifting until the drift time is up, or it nears an
 859          edge or another platform or goes too fast (then the pucks go off at once, and it stops)."""
 860          r, d, p = self.run, self.d, self.run.params
 861          d.set(duty=[0.0] * NUM_THRUSTERS, pucks=True)
 862          d.say("Drifting (up to %g s)." % p["drift"])
 863          d.start_recording()
 864          why = []
 865  
 866          def stop(pose):
 867              if not self.inside(pose):
 868                  why.append("near an edge or a platform")
 869                  return True
 870              if math.hypot(pose[3], pose[4]) > p["max_speed"]:
 871                  why.append("faster than %g m/s" % p["max_speed"])
 872                  return True
 873              return False
 874          d.wait(p["drift"], until=stop)
 875          rec = d.stop_recording()
 876          cut = why[0] if why else None
 877          if cut:
 878              d.set(pucks=False)
 879              d.say("Drift cut short (%s): pucks off." % cut)
 880              d.wait(STOP_PAUSE)
 881              d.set(pucks=True)
 882          moved = math.hypot(rec[-1][1] - rec[0][1], rec[-1][2] - rec[0][2]) if len(rec) > 1 else 0.0
 883          secs = rec[-1][0] - rec[0][0] if len(rec) > 1 else 0.0
 884          note = "%.1f s, %.0f mm%s" % (secs, 1000 * moved, " (cut: %s)" % cut if cut else "")
 885          d.end_step(note=note)
 886          glitches = sum(1 for a, b in zip(rec, rec[1:]) if math.hypot(b[1] - a[1], b[2] - a[2]) > GLITCH_STEP)
 887          record = {"platform": r.platform, "t": [s[0] for s in rec], "x": [s[1] for s in rec], "y": [s[2] for s in rec],
 888                    "theta": [s[3] for s in rec], "cut": cut, "glitches": glitches}
 889          r.records.append({"points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 150)], "cut": cut})
 890          return record
 891  
 892      def done(self, ok, note, drifts, still):
 893          r = self.run
 894          if not ok:
 895              if r.step is not None and r.steps[r.step]["state"] == "running":
 896                  r.steps[r.step]["state"] = "failed"
 897              r.error = note if r.state == "running" else None
 898              stopped = r.state == "stopping"
 899              r.state = "stopped" if stopped else "failed"
 900              self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
 901              if drifts:
 902                  save_run(r, {"kind": "table", "platform": r.platform, "params": r.params, "still": still,
 903                               "drifts": drifts, "aborted": note})
 904          else:
 905              r.state = "done"
 906          r.finished = time.time()
 907          self.cal.publish()
 908  
 909  
 910  # ---------------------------------------------------------------------------- the thrusters
 911  
 912  THRUSTER_INSET = 0.081       # [m] the thrusters sit this far in from the body's edges (spot.massprops.centerOfGravity)
 913  BURN_LATENCY = 0.15          # [s] from the command to the thruster pushing (SSH, the Teensy, the valve): skipped
 914  BURN_LIFT = 0.5              # [s] the pucks lift the platform before the burn
 915  REST_TIME = 1.0              # [s] resting between burns
 916  BURN_RMS_LIMIT = 3.0         # [mm] a burn whose motion doesn't follow a constant force better than this is left out
 917  
 918  
 919  def dist_from_cg(cg_mm):
 920      """The mixer's thruster_dist2CG [mm] for a centre of gravity at CG_MM [x, y] [mm] from the body's centre,
 921      as spot.massprops.centerOfGravity makes them."""
 922      e = 0.15 - THRUSTER_INSET
 923      X, Y = cg_mm[0] / 1000.0, cg_mm[1] / 1000.0
 924      return [1000 * v for v in (e - Y, -(e + Y), e - X, -(e + X), e + Y, -(e - Y), e + X, -(e - X))]
 925  
 926  
 927  def assumed_defaults():
 928      """What the console takes each platform's mass, inertia and centre of gravity to be: the controller's
 929      defaults (control.DEFAULTS: SPOT's default settings), the CG worked back from its thruster offsets."""
 930      e = 0.15 - THRUSTER_INSET
 931      out = {}
 932      for n in PLATFORMS:
 933          p = DEFAULTS["platforms"][n]
 934          d = p["thruster_dist2CG"]
 935          out[n] = {"mass": p["mass"], "inertia": p["inertia"], "thrust": p["thrust"],
 936                    "cg_mm": [round(1000 * e - d[2], 3), round(1000 * e - d[0], 3)]}
 937      return out
 938  
 939  
 940  def controller_params(platform):
 941      """The controller's parameters (control.Controller) for PLATFORM: SPOT's defaults, with the lab's
 942      calibrated thrust of each thruster when the lab has a thruster calibration for it."""
 943      params = copy.deepcopy(DEFAULTS)
 944      th = (store.get()["calibration"].get("thrusters") or {}).get(platform)
 945      if th:
 946          params["platforms"][platform]["thrust"] = list(th["thrust"])
 947      return params
 948  
 949  
 950  class ThrusterRun:
 951      def __init__(self, cal, run):
 952          self.cal = cal
 953          self.run = run
 954          self.d = Driver(cal, run)
 955  
 956      def main(self):
 957          r, d, p = self.run, self.d, self.run.params
 958          order = [k for k in range(1, NUM_THRUSTERS + 1)]
 959          r.steps = ([{"name": "Start", "state": "waiting", "note": "A clear place to start"}] +
 960                     [{"name": "Thruster %d" % k, "state": "waiting", "note": ""} for k in order] +
 961                     [{"name": "Fit", "state": "waiting", "note": ""}])
 962          burns, ok, note = [], False, None
 963          try:
 964              d.begin_step(0)
 965              pose = d.check()
 966              if pose is None:
 967                  raise Abort("PhaseSpace doesn't see the %s" % shown(r.platform))
 968              d.claim()
 969              start = self.start_place(pose)
 970              if start:
 971                  d.say("Flying to a clear place to start (%.2f m, %.2f m)." % (start[0], start[1]))
 972                  d.fly_to(start)
 973              d.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
 974              d.wait(REST_TIME)
 975              d.end_step(note="(%.2f, %.2f) m" % tuple(d.check()[:2]))
 976              for k in order:
 977                  d.begin_step(k)
 978                  notes = []
 979                  for rep in range(p["repeats"]):
 980                      b = self.burn(k, rep)
 981                      burns.append(b)
 982                      notes.append("%.3f N" % b["fit"]["thrust"] if b.get("fit") else "no fit")
 983                      d.end_step("running", " · ".join(notes))
 984                  d.end_step(note=" · ".join(notes))
 985              d.finish(True, "Calibration finished; all off")
 986              d.begin_step(len(r.steps) - 1)
 987              result = self.result(burns)
 988              folder = save_run(r, {"kind": "thrusters", "platform": r.platform, "params": p, "burns": burns,
 989                                    "result": result})
 990              result["folder"] = folder
 991              store.set_section("thrusters", result, r.platform)
 992              r.result = result
 993              bad = [k + 1 for k in range(NUM_THRUSTERS) if not result["valid"][k]]
 994              d.end_step("done" if not bad else "failed",
 995                         "%.3f-%.3f N" % (min(result["thrust"]), max(result["thrust"])))
 996              d.say("Done: thrust %s N.%s" % (", ".join("%.3f" % f for f in result["thrust"]),
 997                    " No good burn of thruster %s: it keeps the nominal thrust." % ", ".join(map(str, bad)) if bad else ""))
 998              ok = True
 999          except Abort as e:
1000              note = str(e)
1001          except ApiError as e:
1002              note = str(e)
1003          except Exception as e:   # never leave the platform floating after a bug
1004              note = "internal error: %s: %s" % (type(e).__name__, e)
1005          finally:
1006              d.finish(ok, "Calibration %s; all off" % ("finished" if ok else "stopped (%s)" % note))
1007          self.done(ok, note, burns)
1008  
1009      def start_place(self, pose):
1010          """Where to start: where it is if that leaves room for a burn (the margin plus 0.25 m from the edges,
1011          clear of the other platforms), else the nearest such place; None for here."""
1012          m = self.run.params["margin"] + 0.25
1013          x = min(max(pose[0], m), TABLE[0] - m)
1014          y = min(max(pose[1], m), TABLE[1] - m)
1015          here = abs(x - pose[0]) < 1e-9 and abs(y - pose[1]) < 1e-9
1016          clear = all(math.hypot(x - o[0], y - o[1]) >= CLEARANCE + 0.15 for o in self.d.others().values())
1017          if not clear:
1018              raise Abort("another platform is within %.2f m of where the %s would start; move it, or the %s"
1019                          % (CLEARANCE + 0.15, shown(self.run.platform), shown(self.run.platform)))
1020          return None if here else [x, y, pose[2]]
1021  
1022      def burn(self, k, rep):
1023          """One burn of thruster K: float, fire, back to the start, pucks off, rest. Returns its record and fit."""
1024          r, d, p = self.run, self.d, self.run.params
1025          pose = d.check()
1026          start = [pose[0], pose[1], pose[2]]
1027          d.say("Thruster %d%s: floating, then firing for %g s." % (k, " (repeat %d)" % (rep + 1) if p["repeats"] > 1 else "",
1028                                                                       p["burn"]))
1029          d.set(duty=[0.0] * NUM_THRUSTERS, pucks=True)
1030          d.start_recording()
1031          d.wait(BURN_LIFT)
1032          duty = [0.0] * NUM_THRUSTERS
1033          duty[k - 1] = 100.0
1034          d.set(duty=duty)
1035          t_on = time.time()
1036          cut = []
1037  
1038          def stop(pose):
1039              m = p["margin"]
1040              if not (m <= pose[0] <= TABLE[0] - m and m <= pose[1] <= TABLE[1] - m):
1041                  cut.append("near an edge")
1042                  return True
1043              return False
1044          d.wait(p["burn"], until=stop)
1045          t_off = time.time()
1046          d.set(duty=[0.0] * NUM_THRUSTERS)
1047          d.wait(0.2)
1048          rec = d.stop_recording()
1049          if cut:
1050              d.say("Thruster %d: burn cut short (%s)." % (k, cut[0]))
1051          r.records.append({"thruster": k, "points": [[round(s[1], 4), round(s[2], 4)] for s in thin(rec, 80)],
1052                            "cut": cut[0] if cut else None})
1053          record = {"thruster": k, "repeat": rep, "t_on": t_on, "t_off": t_off, "cut": cut[0] if cut else None,
1054                    "frames": rec, "start": start}
1055          record["fit"], record["error"] = self.fit(k, rec, t_on, t_off)
1056          # back where it started, then rest
1057          d.say("Thruster %d: flying back to the start." % k)
1058          d.fly_to(start)
1059          d.set(duty=[0.0] * NUM_THRUSTERS, pucks=False)
1060          d.wait(REST_TIME)
1061          return record
1062  
1063      def fit(self, k, rec, t_on, t_off):
1064          """(the burn's fit, or None, and why not)."""
1065          a = self.run.params["assumed"]
1066          burn = [s for s in rec if t_on + BURN_LATENCY <= s[4] <= t_off]
1067          if len(burn) < 20:
1068              return None, "too few frames during the burn (%d)" % len(burn)
1069          jumps = sum(1 for u, v in zip(burn, burn[1:]) if math.hypot(v[1] - u[1], v[2] - u[2]) > GLITCH_STEP)
1070          if jumps:
1071              return None, "PhaseSpace's positions jumped %d times" % jumps
1072          table = store.get()["calibration"].get("table")
1073          slope = table["height"] if table else None
1074          damping = (table or {}).get("damping", {}).get(self.run.platform, 0.0) if table else 0.0
1075          f = calfit.fit_burn([s[0] for s in burn], [s[1] for s in burn], [s[2] for s in burn], [s[3] for s in burn],
1076                              a["mass"], a["inertia"], k, a["dist2CG"][k - 1], slope=slope, damping=damping)
1077          if f["rms_mm"] > BURN_RMS_LIMIT:
1078              return None, "the motion doesn't follow a constant thrust (residual %.1f mm)" % f["rms_mm"]
1079          if not 0.02 <= f["thrust"] <= 1.5:
1080              return None, "an unlikely thrust (%.3f N): did the thruster fire?" % f["thrust"]
1081          return f, None
1082  
1083      def result(self, burns):
1084          """The calibration: each thruster's thrust (the mean of its good burns; the nominal one without any),
1085          its spread, how far it is turned, its torque ratio, and each burn, for the page."""
1086          r, p = self.run, self.run.params
1087          nominal = assumed_defaults()[r.platform]["thrust"]
1088          out = {"updated": now_iso(), "by": r.by, "platform": r.platform, "run": r.stamp, "params": p,
1089                 "nominal": nominal, "table_used": bool(store.get()["calibration"].get("table")),
1090                 "thrust": [], "thrust_sd": [], "angle": [], "torque_ratio": [], "valid": [], "burns": []}
1091          for k in range(1, NUM_THRUSTERS + 1):
1092              fits = [b["fit"] for b in burns if b["thruster"] == k and b["fit"]]
1093              mean, sd = calfit.summarise([f["thrust"] for f in fits])
1094              angle, _ = calfit.summarise([f["angle"] for f in fits])
1095              ratios = [f["torque_ratio"] for f in fits if f["torque_ratio"] is not None]
1096              ratio, _ = calfit.summarise(ratios)
1097              out["valid"].append(bool(fits))
1098              out["thrust"].append(mean if fits else nominal)
1099              out["thrust_sd"].append(sd if len(fits) > 1 else None)   # (one burn: no spread to give)
1100              out["angle"].append(angle)
1101              out["torque_ratio"].append(ratio)
1102          for b in burns:
1103              f = b["fit"]
1104              out["burns"].append({"thruster": b["thruster"], "repeat": b["repeat"], "cut": b["cut"], "error": b["error"],
1105                                   "thrust": f and f["thrust"], "angle": f and f["angle"],
1106                                   "torque_ratio": f and f["torque_ratio"], "rms_mm": f and f["rms_mm"],
1107                                   "points": [[round(s[1] - b["start"][0], 4), round(s[2] - b["start"][1], 4)]
1108                                              for s in thin([s for s in b["frames"] if s[4] >= b["t_on"]], 60)],
1109                                   "start_deg": math.degrees(b["start"][2])})
1110          return out
1111  
1112      def done(self, ok, note, burns):
1113          r = self.run
1114          if not ok:
1115              if r.step is not None and r.steps[r.step]["state"] == "running":
1116                  r.steps[r.step]["state"] = "failed"
1117              stopped = r.state == "stopping"
1118              r.error = None if stopped else note
1119              r.state = "stopped" if stopped else "failed"
1120              self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
1121              if burns:
1122                  save_run(r, {"kind": "thrusters", "platform": r.platform, "params": r.params, "burns": burns,
1123                               "aborted": note})
1124          else:
1125              r.state = "done"
1126          r.finished = time.time()
1127          self.cal.publish()
1128  
1129  
1130  # ---------------------------------------------------------------------------- the arm
1131  
1132  ARM_TESTS = ("breakaway", "sweep", "inertia", "steps")
1133  JOINT_NAMES = {1: "Shoulder", 2: "Elbow", 3: "Wrist"}
1134  ARM_POLL = 2.0               # [s] between status checks while the script runs
1135  ARM_TIMEOUT = 45 * 60        # [s] the longest a run may take (three joints, every test: about 15 min)
1136  
1137  
1138  def arm_script():
1139      """The arm calibration script: next to the console on the TX2 (deploy_tx2.sh copies it there), or in
1140      the template's resources/arm_calibration."""
1141      for p in (PROJECT_DIR / "arm_calibration" / "calibrate_arm.py",
1142                PROJECT_DIR.parent.parent / "resources" / "arm_calibration" / "calibrate_arm.py"):
1143          if p.is_file():
1144              return p
1145      raise ApiError("calibrate_arm.py is not on the lab console: run deploy_tx2.sh (it copies "
1146                     "resources/arm_calibration/calibrate_arm.py there).")
1147  
1148  
1149  def arm_check():
1150      """{"ok": whether a real run can start, "items": [{"what", "ok", "value", "fix"}], "raw"} from the Chaser's
1151      arm-cal-check: what calibrate_arm.py needs there."""
1152      rc, out, err = platform_run("RED", "arm-cal-check", timeout=20)
1153      text = decode(out)
1154      if rc != 0 or not text:
1155          reason = err or text or "no answer (code %s)" % rc
1156          if "only accepts" in reason:
1157              reason = "the Chaser's agent is older than this console: run setup_hardware_link.sh to update it"
1158          return {"ok": False, "reachable": False, "items": [], "error": reason, "raw": text}
1159      kv, users = {}, []
1160      for line in text.splitlines():
1161          key, _, value = line.partition(" ")
1162          if key == "user":
1163              users.append(value)
1164          else:
1165              kv[key] = value
1166      py = kv.get("python", "no")
1167      items = [
1168          {"what": "Python 3", "ok": py != "no", "value": py if py != "no" else "missing",
1169           "fix": "Install python3 on the Chaser."},
1170          {"what": "Dynamixel SDK", "ok": kv.get("sdk") == "yes", "value": "installed" if kv.get("sdk") == "yes" else "missing",
1171           "fix": "On the Chaser: pip3 install --user dynamixel-sdk"},
1172          {"what": "U2D2 at /dev/ttyUSB0", "ok": kv.get("port") == "yes", "value": "found" if kv.get("port") == "yes" else "not found",
1173           "fix": "Plug in the U2D2 and power the arm."},
1174          {"what": "Access to the port", "ok": kv.get("access") == "yes", "value": "yes" if kv.get("access") == "yes" else "no",
1175           "fix": "Run +spot/+hardware/arm/setup_arm_serial.sh for the Chaser (dialout group, udev rule), then log in again."},
1176          {"what": "Port free", "ok": not users, "value": ", ".join(users) if users else "free",
1177           "fix": "Stop the program holding it (an experiment?)."},
1178          {"what": "Latency timer", "ok": True, "warn": kv.get("latency") not in ("1",),
1179           "value": "%s ms" % kv["latency"] if kv.get("latency", "unknown") != "unknown" else "unknown",
1180           "fix": "setup_arm_serial.sh sets it to 1 ms (16 ms works, but the data comes slower)."},
1181      ]
1182      running = kv.get("running", "no")
1183      return {"ok": all(i["ok"] for i in items) and running == "no", "reachable": True, "items": items,
1184              "running": running != "no", "script": kv.get("script") == "yes", "raw": text}
1185  
1186  
1187  class ArmRun:
1188      def __init__(self, cal, run):
1189          self.cal = cal
1190          self.run = run
1191          self.d = Driver(cal, run)
1192          self.hw = hardware.get("RED")
1193          self.reserved = False
1194  
1195      def main(self):
1196          r, d, p = self.run, self.d, self.run.params
1197          r.steps = ([{"name": "Check", "state": "waiting", "note": "What the Chaser has"},
1198                      {"name": "Send Script", "state": "waiting", "note": "calibrate_arm.py"}] +
1199                     [{"name": "%s %s" % (JOINT_NAMES[j], t.capitalize()), "state": "waiting", "note": "", "joint": j, "test": t}
1200                      for j in p["joints"] for t in p["tests"]] +
1201                     [{"name": "Collect", "state": "waiting", "note": "Data and fit"}])
1202          r.check = None
1203          ok, note, started = False, None, False
1204          try:
1205              d.begin_step(0)
1206              chk = arm_check()
1207              r.check = chk
1208              if not chk["reachable"]:
1209                  raise Abort("the Chaser doesn't answer: %s" % chk["error"])
1210              if chk["running"]:
1211                  raise Abort("a calibration is already running on the Chaser (stop it, or wait for it)")
1212              missing = [i["what"] for i in chk["items"] if not i["ok"]]
1213              if missing and not p["simulate"]:
1214                  raise Abort("the Chaser isn't ready: %s (see the checks)" % ", ".join(missing))
1215              d.end_step(note="ready" if not missing else "rehearsal (%s missing)" % ", ".join(missing))
1216              if not p["simulate"]:   # the platform rests while the arm moves: nothing may float it
1217                  self.hw.reserve(r, r.by)
1218                  self.reserved = True
1219              d.begin_step(1)
1220              buf = io.BytesIO()
1221              with tarfile.open(fileobj=buf, mode="w") as tar:
1222                  tar.add(str(arm_script()), arcname="calibrate_arm.py")
1223              rc, out, err = platform_run("RED", "arm-cal-receive", data=buf.getvalue(), timeout=30)
1224              if rc != 0:
1225                  raise Abort("could not send the script: %s" % (err or decode(out)))
1226              d.end_step()
1227              cmd = "arm-cal-start %s %s%s" % ("".join(str(j) for j in p["joints"]), ",".join(p["tests"]),
1228                                               " simulate" if p["simulate"] else "")
1229              rc, out, err = platform_run("RED", cmd, timeout=30)
1230              if rc != 0 or "started" not in decode(out):
1231                  raise Abort("the script didn't start: %s" % (err or decode(out)))
1232              started = True
1233              d.say("%s started on the Chaser (%s; %s)." % ("A rehearsal (simulated servos)" if p["simulate"] else "The arm calibration",
1234                                                            ", ".join(JOINT_NAMES[j] for j in p["joints"]), ", ".join(p["tests"])))
1235              code = self.follow()
1236              started = False
1237              if code != 0:
1238                  raise Abort("the script stopped (exit %s): %s" % (code, self.last_words()))
1239              d.begin_step(len(r.steps) - 1)
1240              result = self.collect()
1241              if p["simulate"]:
1242                  r.result = result
1243                  d.end_step(note="rehearsal: not saved")
1244                  d.say("Rehearsal done: the fit of the simulated servos is shown; the lab's arm calibration is unchanged.")
1245              else:
1246                  store.set_section("arm", result)
1247                  r.result = result
1248                  d.end_step(note="saved")
1249                  d.say("Done: the arm calibration is saved; projects get it with Update from Lab.")
1250              ok = True
1251          except Abort as e:
1252              note = str(e)
1253          except ApiError as e:
1254              note = str(e)
1255          except Exception as e:
1256              note = "internal error: %s: %s" % (type(e).__name__, e)
1257          finally:
1258              if started:   # never leave the script moving the arm when the run ends
1259                  platform_run("RED", "arm-cal-stop", timeout=20)
1260              if self.reserved:
1261                  self.hw.release(r, "Arm calibration %s" % ("finished" if ok else "stopped"))
1262          self.done(ok, note)
1263  
1264      def follow(self):
1265          """Follow the script until it ends (its exit code); stops it when the run is stopped, the Chaser is taken
1266          back (Esc or all off on the Hardware tab), or it runs too long."""
1267          r = self.run
1268          t0 = time.monotonic()
1269          while True:
1270              if r.state != "running":
1271                  platform_run("RED", "arm-cal-stop", timeout=20)
1272                  raise Abort(r.stop_reason or "stopped")
1273              if self.reserved and not self.hw.owned_by(r):
1274                  platform_run("RED", "arm-cal-stop", timeout=20)
1275                  raise Abort("the Chaser was taken back (switched off on the Hardware tab)")
1276              if time.monotonic() - t0 > ARM_TIMEOUT:
1277                  platform_run("RED", "arm-cal-stop", timeout=20)
1278                  raise Abort("it ran for more than %d minutes" % (ARM_TIMEOUT // 60))
1279              rc, out, err = platform_run("RED", "arm-cal-status", timeout=20)
1280              if rc == 0:
1281                  text = decode(out)
1282                  lines = [l[4:] for l in text.splitlines() if l.startswith("log ")]
1283                  self.progress(lines)
1284                  code = re.search(r"^exit (\d+)$", text, re.M)
1285                  running = re.search(r"^running (\d+)$", text, re.M)
1286                  if not running and code:
1287                      return int(code.group(1))
1288                  if not running and not code:
1289                      return -1
1290              time.sleep(ARM_POLL)
1291  
1292      def progress(self, lines):
1293          """The script's log on the page, and its PROGRESS lines as the steps."""
1294          r = self.run
1295          r.armlog = lines
1296          last = None
1297          for l in lines:
1298              if l.startswith("PROGRESS "):
1299                  try:
1300                      last = json.loads(l[9:])
1301                  except ValueError:
1302                      pass
1303          if last:
1304              for k, st in enumerate(r.steps):
1305                  if st.get("joint") == last["joint"] and st.get("test") == last["test"]:
1306                      if r.step != k:
1307                          for prev in r.steps[2:k]:
1308                              if prev["state"] in ("waiting", "running"):
1309                                  prev["state"] = "done"
1310                          self.d.begin_step(k, "%d of %d" % (last["index"], last["total"]))
1311                          self.d.say("%s: %s (%d of %d)." % (JOINT_NAMES[last["joint"]], last["test"], last["index"], last["total"]),
1312                                     log=False)
1313          tail = [l for l in lines if l.strip() and not l.startswith("PROGRESS ")]
1314          if tail:
1315              r.message = tail[-1].strip()[:200]
1316          self.cal.publish(force=False)
1317  
1318      def last_words(self):
1319          lines = [l for l in getattr(self.run, "armlog", []) if l.strip() and not l.startswith(("PROGRESS ", "EXIT "))]
1320          return lines[-1].strip() if lines else "no output"
1321  
1322      def collect(self):
1323          """The run's files from the Chaser into ~/SPOTCalibration/<stamp>_arm_RED/, and the calibration: the
1324          script's fit (arm_calibration.json) with what the page draws (each joint's friction points, its
1325          breakaway torques, its position steps, thinned)."""
1326          r = self.run
1327          rc, out, err = platform_run("RED", "arm-cal-data", timeout=120)
1328          if rc != 0 or not out:
1329              raise Abort("could not collect the data: %s" % (err or "nothing came back"))
1330          folder = RUNS_DIR / ("%s_arm_RED%s" % (r.stamp, "_rehearsal" if r.params["simulate"] else ""))
1331          folder.mkdir(parents=True, exist_ok=True)
1332          files = {}
1333          with tarfile.open(fileobj=io.BytesIO(out), mode="r") as tar:
1334              for m in tar.getmembers():
1335                  name = m.name.rsplit("/", 1)[-1]
1336                  if not m.isfile() or not re.match(r"^[A-Za-z0-9_.-]+\.(json|csv)$", name) or m.size > 20 * 1024 * 1024:
1337                      continue
1338                  data = tar.extractfile(m).read()
1339                  (folder / name).write_bytes(data)
1340                  files[name] = data.decode("utf-8", "replace")
1341          if "arm_calibration.json" not in files:
1342              raise Abort("the run has no arm_calibration.json (did it finish?)")
1343          cal = json.loads(files["arm_calibration.json"])
1344          if cal.get("format") != "spot-arm-calibration":
1345              raise Abort("arm_calibration.json isn't an arm calibration")
1346          cal.pop("folder", None)
1347          cal.update(updated=now_iso(), by=r.by, run=r.stamp, simulated=r.params["simulate"], lab_folder=str(folder),
1348                     tests=r.params["tests"], plots=arm_plots(files, cal))
1349          return cal
1350  
1351      def done(self, ok, note):
1352          r = self.run
1353          if not ok:
1354              if r.step is not None and r.steps[r.step]["state"] == "running":
1355                  r.steps[r.step]["state"] = "failed"
1356              stopped = r.state == "stopping"
1357              r.error = None if stopped else note
1358              r.state = "stopped" if stopped else "failed"
1359              self.d.say(("Stopped: %s." if stopped else "Failed: %s.") % (r.stop_reason if stopped else note))
1360          else:
1361              r.state = "done"
1362          r.finished = time.time()
1363          self.cal.publish()
1364  
1365  
1366  def read_csv_text(text):
1367      rows = [l.split(",") for l in text.strip().splitlines()]
1368      if not rows:
1369          return [], []
1370      head = rows[0]
1371      out = []
1372      for row in rows[1:]:
1373          try:
1374              out.append([float(v) for v in row])
1375          except ValueError:
1376              continue
1377      return head, out
1378  
1379  
1380  def arm_plots(files, cal):
1381      """What the page draws of an arm calibration, per joint: the friction against speed (the sweep's steady
1382      points as torque, and the fitted curve's parameters are in the joint's fit), the breakaway torques, and
1383      the position steps (goal and measured, thinned)."""
1384      kt = cal.get("kt") or {}
1385      plots = {}
1386      for j in (1, 2, 3):
1387          p = {}
1388          k = kt.get(str(j)) if isinstance(kt, dict) else kt
1389          pts = files.get("sweep_points_j%d.json" % j)
1390          if pts and k:
1391              try:
1392                  p["sweep"] = [[round(q["velocity"], 5), round(k * q["current"], 5)] for q in json.loads(pts)]
1393              except (ValueError, KeyError, TypeError):
1394                  pass
1395          if "steps_j%d.csv" % j in files:
1396              head, rows = read_csv_text(files["steps_j%d.csv" % j])
1397              col = "q%d" % j
1398              if rows and "t" in head and "goal" in head and col in head:
1399                  ti, gi, qi = head.index("t"), head.index("goal"), head.index(col)
1400                  p["steps"] = [[round(row[ti], 3), round(row[gi], 5), round(row[qi], 5)] for row in thin(rows, 400)]
1401          if p:
1402              plots[str(j)] = p
1403      return plots
1404  
1405  
1406  def noise_mm(samples):
1407      """The cameras' noise on a resting platform: the standard deviation of its position [mm]."""
1408      if not samples or len(samples) < 5:
1409          return None
1410      mx = sum(s[1] for s in samples) / len(samples)
1411      my = sum(s[2] for s in samples) / len(samples)
1412      var = sum((s[1] - mx) ** 2 + (s[2] - my) ** 2 for s in samples) / (2 * (len(samples) - 1))
1413      return 1000 * math.sqrt(var)
```

</details>
<!-- /listing -->
