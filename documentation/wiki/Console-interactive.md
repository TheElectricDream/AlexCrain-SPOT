# interactive.py

**File:** `+spot/+console/webapp/spot_console/interactive.py` · **Part of:** [[the console's server|Console-Server]] · **Page:** [[Interactive Mode|Interactive-Mode]]

<!-- source: +spot/+console/webapp/spot_console/interactive.py -->

## What it is for

Interactive Mode: fly one or more platforms, each to a pose dragged on the lab console's 3D table, **without MATLAB**. The template diagram's own controller ([[control.py|Console-control]]: its LQR law and thruster mixer) closes each platform's loop on the lab console: every `baseRate` (50 ms) take the platform's pose and velocity from PhaseSpace (the monitor), compute the eight duty cycles, and have the hardware driver send them to the Teensy. Each platform has its own thread, and its own **controller process** so the mixers run on separate CPU cores. The controller's parameters are SPOT's defaults, except the thrust: where the lab has a thruster calibration for a platform ([[Calibration Tab|Calibration-Tab]]), it flies with that calibration's thrust of each of its eight thrusters (`calibration.controller_params`, [[calibration.py|Console-calibration]]).

A flight belongs to the page that started it (only it adds platforms or moves goals; it keeps the flight alive with heartbeats); any page can stop it. A platform stops when its toggle is switched off, the Hardware tab switches it off, PhaseSpace loses it for 1 s (thrusters already off after 0.25 s) or sees it off the table, an experiment starts, something else opens its Teensy, or its controller process doesn't answer. The whole flight stops on *Stop all*, Esc, lost heartbeats (3 s) or console shutdown; and each Teensy's own 2 s watchdog is the last line.

Goals stay 0.2 m inside the table's edges and at least 0.45 m from each other and from resting platforms. The controller flies straight at the goal with no idea of the others: crossing paths are for whoever places the goals.

## Function by function

**Lines 47–59.** The constants above.

**Lines 62–100.** `wrap` (the diagram's Wrap block), `parse_goal` (x, y clamped inside the margin; the angle in degrees, not wrapped so its rate stays smooth), `parse_name`, `fresh_pose` (the platform's six-state pose from the monitor's newest frame, or None if stale or untracked; the attitude wrapped as the diagram does before control).

**Lines 103–119.** `check_separation`: no two goals, and no goal and a resting tracked platform, closer than 0.45 m.

**Lines 122–157.** `ControllerProcess(name, params=None)`: a `spawn`ed process running `control.serve` for one platform (with `params` for its `Controller`; None: the defaults), talked to over a pipe: `wait_ready`, `step(measured, desired)` with a 1 s timeout, `close`.

**Lines 160–190.** `Session` (one platform being flown: goal, last step's numbers, timing stats; its `label`, "Interactive Mode", is what the Hardware tab says drives the platform) and `Flight` (the page's id, who started it, the last heartbeat, its sessions).

**Lines 193–237.** `Interactive`: `info` (the controller's parameters and each platform's gain, for the page's panel; not cached: only the LQR gains are kept, in `self._gains`, since they don't depend on the thrust, and each platform's `thrust` is the lab's calibrated thrust of its eight thrusters where there is one, from `calibration.controller_params`, imported inside the function), `status` (per platform: goal, measured, error, forces, duty cycles, lost, timing), `_publish` at most every 0.2 s.

**Lines 241–316.** `start`: checks (goals given, page id, streaming on, no experiment under way, no calibration running (`calibrator.active()`, imported inside the function because [[calibration.py|Console-calibration]] imports this module), every platform seen by PhaseSpace, no other page's flight, separation); platforms already flying get their new goal; new ones get a controller process each (started side by side, each with `controller_params(name)`, so it flies with the lab's calibrated thrust where there is one), are claimed from the hardware driver (all or none: on failure everything claimed is released), and get a loop thread.

**Lines 318–372.** `set_goal` (only the owning page; separation checked with the moved one named first), `heartbeat`, `stop` (one platform or all), `_own` (the flight, if this page flies it).

**Lines 376–414.** `_current` (whether a session is still in the flight) and `_run`, one platform's loop: every period, `_step`; on a late step, count it and resync; a `heartbeat` reason stops the whole flight; any other reason removes the platform and finishes it.

**Lines 416–442.** `_step`: heartbeat check; pose (if stale: thrusters off, and after 1 s give up); off the table → stop; otherwise ask the controller process for the duty cycles and drive the hardware (if the driver says the platform was taken, stop with that reason).

**Lines 444–456.** `_finish`: record the ending, release the hardware, close the process, log it. `shutdown` stops everything.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/interactive.py</b> (459 lines; click to show)</summary>

```python
   1  """Interactive Mode, on the lab console's PhaseSpace tab: fly one or more platforms, each to a pose
   2  chosen in the 3D view. The template diagram's own controller (control.py: its default LQR law and
   3  thruster mixer, ported from +spot/+control) closes each platform's loop here on the lab console.
   4  Every baseRate (0.05 s at SPOT's default 20 Hz, as the diagram's controller and thruster code run)
   5  it takes the platform's pose and velocity from PhaseSpace (the monitor), computes the 8 duty
   6  cycles, and the hardware driver sends them straight to the platform's Teensy. Each platform has its
   7  own loop (a thread), so one platform's trouble doesn't hold up the others, and its own controller
   8  process (ControllerProcess), so the platforms' mixers run side by side on separate CPU cores.
   9  
  10  A flight belongs to the page that started it: only that page adds platforms or moves goals, and it
  11  keeps the flight alive with heartbeats. Any page can stop it.
  12  
  13  Everything on a platform goes off, and that platform leaves the flight, when
  14    - its toggle is switched off on the page; or the hardware tab switches it off (Esc there, or
  15      POST /api/hardware/off)
  16    - PhaseSpace hasn't seen it for LOST_TIMEOUT (its thrusters are off already after STALE_HOLD)
  17    - PhaseSpace sees it off the table
  18    - an experiment starts, another program opens its Teensy, or the link to it is lost (hardware.py)
  19    - its controller process doesn't answer within STEP_TIMEOUT (or a step fails).
  20  Every platform in the flight goes off when
  21    - Stop all is pressed on the page, or Esc on the PhaseSpace tab
  22    - the page flying them stops sending heartbeats for HEARTBEAT_TIMEOUT (closed, asleep, off the network)
  23    - the console stops.
  24  And each Teensy switches everything off by itself when packets stop for 2 s.
  25  
  26  Goals stay MARGIN inside the table's edges, and at least MIN_SEPARATION from each other and from
  27  platforms resting on the table. The controller flies each platform straight at its goal with no
  28  idea of the others, so their paths can still cross: that is for whoever places the goals to avoid.
  29  
  30  Part of the SPOT Console (webapp/app.py starts it).
  31  """
  32  
  33  import math
  34  import multiprocessing
  35  import threading
  36  import time
  37  
  38  from . import control
  39  from .common import ApiError, shown, shown_list
  40  from .control import DEFAULTS, Controller, platform_gains
  41  from .events import hub
  42  from .experiment import experiment
  43  from .hardware import NUM_THRUSTERS, hardware, hw_log
  44  from .phasespace import monitor, streamer
  45  
  46  
  47  TABLE = (3.5, 2.4)        # [m] the table top, as the 3D view draws it (static/viewer.js)
  48  MARGIN = 0.2              # [m] goals stay this far inside the edges (half a platform and 5 cm)
  49  MIN_SEPARATION = 0.45     # [m] between goals (centre to centre): the platforms are 0.3 m square,
  50                            # 0.42 m across the corners
  51  STALE_HOLD = 0.25         # [s] no fresh PhaseSpace pose for this long: thrusters off, pucks stay on
  52  LOST_TIMEOUT = 1.0        # [s] ... for this long: that platform stops
  53  HEARTBEAT_TIMEOUT = 3.0   # [s] the page flying the platforms sends one every 0.5 s (a background
  54                            # tab's timers may only run once a second)
  55  PUBLISH_PERIOD = 0.2      # [s] how often the pages get the loops' status
  56  ENDED_KEPT = 6            # how many recent endings the status lists
  57  START_TIMEOUT = 15.0      # [s] for a controller process to start
  58  STEP_TIMEOUT = 1.0        # [s] for a controller process to answer one step
  59  EXPERIMENT_BUSY = ("deploying", "starting", "running", "stopping")
  60  
  61  
  62  def wrap(angle):
  63      """The diagram's Wrap block (atan2 of sin and cos): an angle in (-pi, pi]."""
  64      return math.atan2(math.sin(angle), math.cos(angle))
  65  
  66  
  67  def parse_goal(body):
  68      """{"x", "y" [m], "att" [deg]} -> [x, y, theta] [m, m, rad], x and y kept inside the margin.
  69      The angle is not wrapped: the page sends it continuously, so its rate stays smooth."""
  70      try:
  71          x, y, att = (float(body[k]) for k in ("x", "y", "att"))
  72      except (KeyError, TypeError, ValueError):
  73          raise ApiError("A goal needs x and y [m] and att [deg].")
  74      if not all(math.isfinite(v) for v in (x, y, att)) or abs(att) > 1e5:
  75          raise ApiError("The goal must be finite numbers.")
  76      return [min(max(x, MARGIN), TABLE[0] - MARGIN), min(max(y, MARGIN), TABLE[1] - MARGIN), math.radians(att)]
  77  
  78  
  79  def parse_name(name):
  80      name = str(name or "").strip().upper()
  81      if name not in DEFAULTS["platforms"]:
  82          raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
  83      return name
  84  
  85  
  86  def fresh_pose(name, now):
  87      """The platform's [x, y, theta, xdot, ydot, thetadot] from PhaseSpace's newest frame, or None
  88      if that frame is older than STALE_HOLD or doesn't have it."""
  89      with monitor._lock:
  90          frame = monitor.latest
  91      if not frame or now - frame["received"] > STALE_HOLD:
  92          return None
  93      c = frame["crafts"].get(name)
  94      if not c or not c["tracked"]:
  95          return None
  96      pose = [c["x"], c["y"], c["att"], c["vx"], c["vy"], c["w"]]
  97      if any(v is None for v in pose):
  98          return None
  99      pose[2] = wrap(pose[2])   # as the diagram's "Store <PLATFORM> Data" does before control
 100      return pose
 101  
 102  
 103  def check_separation(goals, now, moved=None):
 104      """Raise ApiError if two goals, or a goal and a platform resting on the table (tracked by
 105      PhaseSpace and not in GOALS), are closer than MIN_SEPARATION. GOALS: {NAME: [x, y, theta]};
 106      MOVED: the platform whose goal is being moved, named first in the message."""
 107      names = sorted(goals, key=lambda n: (n != moved, n))
 108      points = [(n, goals[n][0], goals[n][1], "goal") for n in names]
 109      for n in DEFAULTS["platforms"]:
 110          pose = None if n in goals else fresh_pose(n, now)
 111          if pose is not None:
 112              points.append((n, pose[0], pose[1], "resting place"))
 113      for i, (a, xa, ya, _) in enumerate(points):
 114          for b, xb, yb, what in points[i + 1:]:
 115              d = math.hypot(xa - xb, ya - yb)
 116              if d < MIN_SEPARATION:
 117                  raise ApiError("The %s's goal is %.2f m from the %s's %s; keep them at least %.2f m apart "
 118                                 "(centre to centre) so the platforms don't touch."
 119                                 % (shown(a), d, shown(b), what, MIN_SEPARATION))
 120  
 121  
 122  class ControllerProcess:
 123      """One platform's Controller (control.py), run in a process of its own by control.serve. Python
 124      runs one thread at a time per process: in the console's own process, the platforms' mixers would
 125      take turns on one CPU core (a step at full thrust takes tens of ms each); in their own processes
 126      they run side by side. Started fresh ("spawn"), so nothing of the console's threads is inherited."""
 127  
 128      _context = multiprocessing.get_context("spawn")
 129  
 130      def __init__(self, name, params=None):
 131          self.name = name
 132          self.conn, child = self._context.Pipe()
 133          self.proc = self._context.Process(target=control.serve, args=(child, name, params),
 134                                            name="spot-controller-" + name, daemon=True)
 135          self.proc.start()
 136          child.close()
 137  
 138      def wait_ready(self):
 139          if not self.conn.poll(START_TIMEOUT) or self.conn.recv() != "ready":
 140              raise ApiError("The %s's controller process didn't start within %g s." % (shown(self.name), START_TIMEOUT))
 141  
 142      def step(self, measured, desired):
 143          """Controller.step in the process: (u, duty cycles, body-frame command)."""
 144          self.conn.send((measured, desired))
 145          if not self.conn.poll(STEP_TIMEOUT):
 146              raise RuntimeError("its controller process didn't answer within %g s" % STEP_TIMEOUT)
 147          return self.conn.recv()
 148  
 149      def close(self):
 150          try:
 151              self.conn.send(None)
 152          except (OSError, ValueError):
 153              pass
 154          self.proc.join(1)
 155          if self.proc.is_alive():
 156              self.proc.terminate()
 157          self.conn.close()
 158  
 159  
 160  class Session:
 161      """One platform being flown: where to, and the controller's last step."""
 162  
 163      label = "Interactive Mode"   # what the hardware tab says drives the platform
 164  
 165      def __init__(self, name, flight, goal, process):
 166          self.name = name
 167          self.flight = flight
 168          self.goal = goal              # [x, y, theta] [m, m, rad]
 169          self.controller = Controller(name)   # its parameters here; it runs in PROCESS
 170          self.process = process
 171          self.started = time.time()
 172          self.lost_since = None
 173          self.measured = None
 174          self.u = [0.0] * 3
 175          self.body = [0.0] * 3
 176          self.duty = [0.0] * NUM_THRUSTERS
 177          self.step_ms = 0.0            # how long a step takes (smoothed)
 178          self.late = 0                 # steps that started late (the loop couldn't keep up)
 179          self.steps = 0
 180          self.done = False
 181  
 182  
 183  class Flight:
 184      """The platforms one page is flying."""
 185  
 186      def __init__(self, client, requester):
 187          self.client = client          # the page flying them (its own random id)
 188          self.requester = requester
 189          self.started = self.beat = time.time()
 190          self.sessions = {}            # NAME -> Session
 191  
 192  
 193  class Interactive:
 194      def __init__(self):
 195          self._lock = threading.Lock()
 196          self.flight = None
 197          self.ended = []    # recent endings, newest last: {"platform", "reason", "time"}
 198          self._published = 0.0
 199          self._gains = None   # each platform's LQR gain (info)
 200  
 201      def info(self):
 202          """The controller as the page shows it: the parameters and each platform's gain (thrust: the lab's
 203          calibrated thrust of each thruster where there is one, calibration.py)."""
 204          from .calibration import controller_params   # (calibration.py imports this module)
 205          if self._gains is None:   # (they don't depend on the thrust)
 206              self._gains = dict((n, platform_gains(p, DEFAULTS["data_rate"])) for n, p in DEFAULTS["platforms"].items())
 207          plats = dict((n, dict(p, K=self._gains[n], thrust=controller_params(n)["platforms"][n]["thrust"]))
 208                       for n, p in DEFAULTS["platforms"].items())
 209          return {"sample_rate": DEFAULTS["sample_rate"], "period": 1.0 / DEFAULTS["sample_rate"],
 210                  "max_iters": DEFAULTS["max_iters"], "tol": DEFAULTS["tol"], "platforms": plats,
 211                  "table": TABLE, "margin": MARGIN, "min_separation": MIN_SEPARATION}
 212  
 213      def status(self):
 214          with self._lock:
 215              f = self.flight
 216              out = {"active": f is not None, "ended": list(self.ended), "period": 1.0 / DEFAULTS["sample_rate"]}
 217              if f is None:
 218                  return out
 219              plats = {}
 220              for name, s in f.sessions.items():
 221                  m = s.measured
 222                  plats[name] = {
 223                      "goal": [s.goal[0], s.goal[1], math.degrees(s.goal[2])],
 224                      "measured": m and [m[0], m[1], math.degrees(m[2])],
 225                      "error": m and [m[0] - s.goal[0], m[1] - s.goal[1], math.degrees(wrap(m[2] - s.goal[2]))],
 226                      "u": s.u, "body": s.body, "duty": [100 * d for d in s.duty],
 227                      "lost": s.lost_since is not None, "started": s.started,
 228                      "step_ms": round(s.step_ms, 1), "late": s.late, "steps": s.steps}
 229              out.update({"client": f.client, "by": f.requester, "started": f.started, "platforms": plats})
 230              return out
 231  
 232      def _publish(self, force=True):
 233          now = time.monotonic()
 234          if not force and now - self._published < PUBLISH_PERIOD:
 235              return
 236          self._published = now
 237          hub.publish("interactive", self.status())
 238  
 239      # ---- requests from the page
 240  
 241      def start(self, goals, client, requester):
 242          """Fly the platforms in GOALS ({NAME: {"x", "y", "att"} or None for where it is now}). Those
 243          already flying get the new goal; the others join the flight, all or none of them."""
 244          if not isinstance(goals, dict) or not goals:
 245              raise ApiError("Say which platforms to fly: {\"RED\": {\"x\": ..., \"y\": ..., \"att\": ...}, ...}.")
 246          if not client:
 247              raise ApiError("Interactive Mode needs the page's id (client).")
 248          goals = {parse_name(n): g for n, g in goals.items()}
 249          if not streamer.status()["running"]:
 250              raise ApiError("Start PhaseSpace streaming first: Interactive Mode flies the platforms from their "
 251                             "PhaseSpace poses.")
 252          if experiment.status()["phase"] in EXPERIMENT_BUSY:
 253              raise ApiError("An experiment is under way; Interactive Mode waits until it has stopped.")
 254          from .calibration import calibrator   # (calibration.py imports this module)
 255          if calibrator.active():
 256              raise ApiError("A calibration is running (Calibration tab); Interactive Mode waits until it has finished.")
 257          now = time.time()
 258          parsed = {}
 259          for name, g in goals.items():
 260              pose = fresh_pose(name, now)
 261              if pose is None:
 262                  raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)." % shown(name))
 263              parsed[name] = parse_goal(g) if g else [pose[0], pose[1], pose[2]]
 264          with self._lock:
 265              f = self.flight
 266              if f is not None and f.client != client:
 267                  raise ApiError("Interactive Mode is flying the %s for %s; stop it first."
 268                                 % (shown_list(sorted(f.sessions)), f.requester))
 269              flying = dict((n, s.goal) for n, s in f.sessions.items()) if f else {}
 270          check_separation(dict(flying, **parsed), now)
 271  
 272          # The platforms already flying get their new goal; the others get a controller process and
 273          # are claimed, all or none
 274          new = [n for n in sorted(parsed) if n not in flying]
 275          flight = f or Flight(client, requester)
 276          # (each with the lab's calibrated thrust of its thrusters, where there is one)
 277          from .calibration import controller_params
 278          processes = dict((n, ControllerProcess(n, controller_params(n))) for n in new)   # they start side by side
 279          claimed = []
 280          try:
 281              for name in new:
 282                  processes[name].wait_ready()
 283              for name in new:
 284                  s = Session(name, flight, parsed[name], processes[name])
 285                  hardware.get(name).claim(s, requester)   # raises if the platform can't be driven now
 286                  claimed.append(s)
 287          except Exception as e:   # ApiError: not ready; anything else: the processes failed
 288              for s in claimed:
 289                  hardware.get(s.name).release(s, "Interactive Mode did not start (%s); all off" % e)
 290              for p in processes.values():
 291                  p.close()
 292              raise
 293          with self._lock:
 294              if self.flight is not None and self.flight is not flight:
 295                  lost = True
 296              else:
 297                  lost = False
 298                  self.flight = flight
 299                  flight.beat = time.time()
 300                  for name, goal in parsed.items():
 301                      if name in flight.sessions:
 302                          flight.sessions[name].goal = goal
 303                  for s in claimed:
 304                      flight.sessions[s.name] = s
 305          if lost:   # another page started a flight meanwhile
 306              for s in claimed:
 307                  hardware.get(s.name).release(s, "Interactive Mode did not start (another page started first); all off")
 308                  s.process.close()
 309              raise ApiError("Another page started Interactive Mode at the same moment; try again.")
 310          for s in claimed:
 311              g = s.goal
 312              hw_log("%s: Interactive Mode started, goal x %.3f m, y %.3f m, %.1f deg (by %s)"
 313                     % (shown(s.name), g[0], g[1], math.degrees(g[2]), requester))
 314              threading.Thread(target=self._run, args=(s,), daemon=True).start()
 315          self._publish()
 316          return self.status()
 317  
 318      def set_goal(self, name, goal, client, final=True):
 319          name = parse_name(name)
 320          goal = parse_goal(goal)
 321          with self._lock:
 322              f = self._own(client)
 323              s = f.sessions.get(name)
 324              if s is None:
 325                  raise ApiError("Interactive Mode isn't flying the %s." % shown(name))
 326              others = dict((n, o.goal) for n, o in f.sessions.items() if n != name)
 327              f.beat = time.time()
 328          check_separation(dict(others, **{name: goal}), time.time(), moved=name)
 329          with self._lock:
 330              s.goal = goal
 331          if final:
 332              hw_log("%s: Interactive Mode goal x %.3f m, y %.3f m, %.1f deg"
 333                     % (shown(name), goal[0], goal[1], math.degrees(goal[2])))
 334          self._publish()
 335          return self.status()
 336  
 337      def heartbeat(self, client):
 338          with self._lock:
 339              f = self.flight
 340              if f is not None and f.client == client:
 341                  f.beat = time.time()
 342                  return {"active": True}
 343          return {"active": False}
 344  
 345      def stop(self, requester, name=None, reason=None):
 346          """Stop one platform (NAME), or the whole flight."""
 347          name = parse_name(name) if name else None
 348          with self._lock:
 349              f = self.flight
 350              if f is None:
 351                  stopping = []
 352              elif name:
 353                  s = f.sessions.pop(name, None)
 354                  stopping = [s] if s else []
 355                  if not f.sessions:
 356                      self.flight = None
 357              else:
 358                  stopping = list(f.sessions.values())
 359                  f.sessions = {}
 360                  self.flight = None
 361          for s in stopping:
 362              self._finish(s, reason or "stopped by %s" % requester)
 363          return self.status()
 364  
 365      def _own(self, client):
 366          """The flight, if the page CLIENT flies it (call with the lock held)."""
 367          f = self.flight
 368          if f is None:
 369              raise ApiError("Interactive Mode isn't flying anything.")
 370          if f.client != client:
 371              raise ApiError("Interactive Mode is flying the %s for %s." % (shown_list(sorted(f.sessions)), f.requester))
 372          return f
 373  
 374      # ---- the loops, one per platform
 375  
 376      def _current(self, s):
 377          """Whether S is still in the flight (call with the lock held)."""
 378          return self.flight is s.flight and s.flight.sessions.get(s.name) is s
 379  
 380      def _run(self, s):
 381          p = hardware.get(s.name)
 382          Ts = s.controller.period
 383          next_step = time.monotonic()
 384          reason = None
 385          while reason is None:
 386              with self._lock:
 387                  if not self._current(s):
 388                      return   # stopped (stop() has switched it off)
 389              t0 = time.monotonic()
 390              try:
 391                  reason = self._step(s, p)
 392              except Exception as e:  # never leave the thrusters on after a bug
 393                  reason = "internal error: %s: %s" % (type(e).__name__, e)
 394              took = (time.monotonic() - t0) * 1000
 395              s.step_ms = 0.9 * s.step_ms + 0.1 * took if s.steps else took
 396              s.steps += 1
 397              self._publish(force=False)
 398              next_step += Ts
 399              delay = next_step - time.monotonic()
 400              if delay > 0:
 401                  time.sleep(delay)
 402              else:
 403                  s.late += 1
 404                  next_step = time.monotonic()
 405          if reason == "heartbeat":
 406              # The page is gone: the whole flight stops (each loop notices; the first stops them all)
 407              self.stop("the console", reason="the page flying it stopped answering (closed, asleep, or off the network)")
 408              return
 409          with self._lock:
 410              if self._current(s):
 411                  del s.flight.sessions[s.name]
 412                  if not s.flight.sessions:
 413                      self.flight = None
 414          self._finish(s, reason)
 415  
 416      def _step(self, s, p):
 417          """One control step; returns why this platform must stop ("heartbeat": the whole flight), or None."""
 418          now = time.time()
 419          if now - s.flight.beat > HEARTBEAT_TIMEOUT:
 420              return "heartbeat"
 421          pose = fresh_pose(s.name, now)
 422          if pose is None:
 423              s.lost_since = s.lost_since or now
 424              if now - s.lost_since > LOST_TIMEOUT:
 425                  return "PhaseSpace lost the %s for more than %g s" % (shown(s.name), LOST_TIMEOUT)
 426              s.duty = [0.0] * NUM_THRUSTERS
 427              return None if p.drive(s, s.duty) else self._taken(p)
 428          s.lost_since = None
 429          if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
 430              return "PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)" % (shown(s.name), pose[0], pose[1])
 431          with self._lock:
 432              goal = list(s.goal)
 433          u, duty, body = s.process.step(pose, goal)
 434          if not p.drive(s, [100 * d for d in duty]):
 435              return self._taken(p)
 436          s.measured, s.u, s.body, s.duty = pose, u, body, duty
 437          return None
 438  
 439      @staticmethod
 440      def _taken(p):
 441          la = p.status()["last_action"]
 442          return "the %s was taken back (%s)" % (shown(p.name), la["text"] if la else "switched off")
 443  
 444      def _finish(self, s, reason):
 445          with self._lock:
 446              if s.done:
 447                  return
 448              s.done = True
 449              self.ended = (self.ended + [{"platform": s.name, "reason": reason, "time": time.time()}])[-ENDED_KEPT:]
 450          hardware.get(s.name).release(s, "Interactive Mode stopped (%s); all off" % reason)
 451          s.process.close()
 452          hw_log("%s: Interactive Mode ended after %.0f s: %s" % (shown(s.name), time.time() - s.started, reason))
 453          self._publish()
 454  
 455      def shutdown(self):
 456          self.stop("the console", reason="the console is shutting down")
 457  
 458  
 459  interactive = Interactive()
```

</details>
<!-- /listing -->
