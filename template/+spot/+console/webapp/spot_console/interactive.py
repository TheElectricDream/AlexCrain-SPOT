"""Interactive Mode, on the lab console's PhaseSpace tab: fly one or more platforms, each to a pose
chosen in the 3D view. The template diagram's own controller (control.py: its default LQR law and
thruster mixer, ported from +spot/+control) closes each platform's loop here on the lab console.
Every baseRate (0.05 s at SPOT's default 20 Hz, as the diagram's controller and thruster code run)
it takes the platform's pose and velocity from PhaseSpace (the monitor), computes the 8 duty
cycles, and the hardware driver sends them straight to the platform's Teensy. Each platform has its
own loop (a thread), so one platform's trouble doesn't hold up the others, and its own controller
process (ControllerProcess), so the platforms' mixers run side by side on separate CPU cores.

A flight belongs to the page that started it: only that page adds platforms or moves goals, and it
keeps the flight alive with heartbeats. Any page can stop it.

Everything on a platform goes off, and that platform leaves the flight, when
  - its toggle is switched off on the page; or the hardware tab switches it off (Esc there, or
    POST /api/hardware/off)
  - PhaseSpace hasn't seen it for LOST_TIMEOUT (its thrusters are off already after STALE_HOLD)
  - PhaseSpace sees it off the table
  - an experiment starts, another program opens its Teensy, or the link to it is lost (hardware.py)
  - its controller process doesn't answer within STEP_TIMEOUT (or a step fails).
Every platform in the flight goes off when
  - Stop all is pressed on the page, or Esc on the PhaseSpace tab
  - the page flying them stops sending heartbeats for HEARTBEAT_TIMEOUT (closed, asleep, off the network)
  - the console stops.
And each Teensy switches everything off by itself when packets stop for 2 s.

Goals stay MARGIN inside the table's edges, and at least MIN_SEPARATION from each other and from
platforms resting on the table. The controller flies each platform straight at its goal with no
idea of the others, so their paths can still cross: that is for whoever places the goals to avoid.

Part of the SPOT Console (webapp/app.py starts it).
"""

import math
import multiprocessing
import threading
import time

from . import control
from .common import ApiError, shown, shown_list
from .control import DEFAULTS, Controller, platform_gains
from .events import hub
from .experiment import experiment
from .hardware import NUM_THRUSTERS, hardware, hw_log
from .phasespace import monitor, streamer


TABLE = (3.5, 2.4)        # [m] the table top, as the 3D view draws it (static/viewer.js)
MARGIN = 0.2              # [m] goals stay this far inside the edges (half a platform and 5 cm)
MIN_SEPARATION = 0.45     # [m] between goals (centre to centre): the platforms are 0.3 m square,
                          # 0.42 m across the corners
STALE_HOLD = 0.25         # [s] no fresh PhaseSpace pose for this long: thrusters off, pucks stay on
LOST_TIMEOUT = 1.0        # [s] ... for this long: that platform stops
HEARTBEAT_TIMEOUT = 3.0   # [s] the page flying the platforms sends one every 0.5 s (a background
                          # tab's timers may only run once a second)
PUBLISH_PERIOD = 0.2      # [s] how often the pages get the loops' status
ENDED_KEPT = 6            # how many recent endings the status lists
START_TIMEOUT = 15.0      # [s] for a controller process to start
STEP_TIMEOUT = 1.0        # [s] for a controller process to answer one step
EXPERIMENT_BUSY = ("deploying", "starting", "running", "stopping")


def wrap(angle):
    """The diagram's Wrap block (atan2 of sin and cos): an angle in (-pi, pi]."""
    return math.atan2(math.sin(angle), math.cos(angle))


def parse_goal(body):
    """{"x", "y" [m], "att" [deg]} -> [x, y, theta] [m, m, rad], x and y kept inside the margin.
    The angle is not wrapped: the page sends it continuously, so its rate stays smooth."""
    try:
        x, y, att = (float(body[k]) for k in ("x", "y", "att"))
    except (KeyError, TypeError, ValueError):
        raise ApiError("A goal needs x and y [m] and att [deg].")
    if not all(math.isfinite(v) for v in (x, y, att)) or abs(att) > 1e5:
        raise ApiError("The goal must be finite numbers.")
    return [min(max(x, MARGIN), TABLE[0] - MARGIN), min(max(y, MARGIN), TABLE[1] - MARGIN), math.radians(att)]


def parse_name(name):
    name = str(name or "").strip().upper()
    if name not in DEFAULTS["platforms"]:
        raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
    return name


def fresh_pose(name, now):
    """The platform's [x, y, theta, xdot, ydot, thetadot] from PhaseSpace's newest frame, or None
    if that frame is older than STALE_HOLD or doesn't have it."""
    with monitor._lock:
        frame = monitor.latest
    if not frame or now - frame["received"] > STALE_HOLD:
        return None
    c = frame["crafts"].get(name)
    if not c or not c["tracked"]:
        return None
    pose = [c["x"], c["y"], c["att"], c["vx"], c["vy"], c["w"]]
    if any(v is None for v in pose):
        return None
    pose[2] = wrap(pose[2])   # as the diagram's "Store <PLATFORM> Data" does before control
    return pose


def check_separation(goals, now, moved=None):
    """Raise ApiError if two goals, or a goal and a platform resting on the table (tracked by
    PhaseSpace and not in GOALS), are closer than MIN_SEPARATION. GOALS: {NAME: [x, y, theta]};
    MOVED: the platform whose goal is being moved, named first in the message."""
    names = sorted(goals, key=lambda n: (n != moved, n))
    points = [(n, goals[n][0], goals[n][1], "goal") for n in names]
    for n in DEFAULTS["platforms"]:
        pose = None if n in goals else fresh_pose(n, now)
        if pose is not None:
            points.append((n, pose[0], pose[1], "resting place"))
    for i, (a, xa, ya, _) in enumerate(points):
        for b, xb, yb, what in points[i + 1:]:
            d = math.hypot(xa - xb, ya - yb)
            if d < MIN_SEPARATION:
                raise ApiError("The %s's goal is %.2f m from the %s's %s; keep them at least %.2f m apart "
                               "(centre to centre) so the platforms don't touch."
                               % (shown(a), d, shown(b), what, MIN_SEPARATION))


class ControllerProcess:
    """One platform's Controller (control.py), run in a process of its own by control.serve. Python
    runs one thread at a time per process: in the console's own process, the platforms' mixers would
    take turns on one CPU core (a step at full thrust takes tens of ms each); in their own processes
    they run side by side. Started fresh ("spawn"), so nothing of the console's threads is inherited."""

    _context = multiprocessing.get_context("spawn")

    def __init__(self, name, params=None):
        self.name = name
        self.conn, child = self._context.Pipe()
        self.proc = self._context.Process(target=control.serve, args=(child, name, params),
                                          name="spot-controller-" + name, daemon=True)
        self.proc.start()
        child.close()

    def wait_ready(self):
        if not self.conn.poll(START_TIMEOUT) or self.conn.recv() != "ready":
            raise ApiError("The %s's controller process didn't start within %g s." % (shown(self.name), START_TIMEOUT))

    def step(self, measured, desired):
        """Controller.step in the process: (u, duty cycles, body-frame command)."""
        self.conn.send((measured, desired))
        if not self.conn.poll(STEP_TIMEOUT):
            raise RuntimeError("its controller process didn't answer within %g s" % STEP_TIMEOUT)
        return self.conn.recv()

    def close(self):
        try:
            self.conn.send(None)
        except (OSError, ValueError):
            pass
        self.proc.join(1)
        if self.proc.is_alive():
            self.proc.terminate()
        self.conn.close()


class Session:
    """One platform being flown: where to, and the controller's last step."""

    label = "Interactive Mode"   # what the hardware tab says drives the platform

    def __init__(self, name, flight, goal, process):
        self.name = name
        self.flight = flight
        self.goal = goal              # [x, y, theta] [m, m, rad]
        self.controller = Controller(name)   # its parameters here; it runs in PROCESS
        self.process = process
        self.started = time.time()
        self.lost_since = None
        self.measured = None
        self.u = [0.0] * 3
        self.body = [0.0] * 3
        self.duty = [0.0] * NUM_THRUSTERS
        self.step_ms = 0.0            # how long a step takes (smoothed)
        self.late = 0                 # steps that started late (the loop couldn't keep up)
        self.steps = 0
        self.done = False


class Flight:
    """The platforms one page is flying."""

    def __init__(self, client, requester):
        self.client = client          # the page flying them (its own random id)
        self.requester = requester
        self.started = self.beat = time.time()
        self.sessions = {}            # NAME -> Session


class Interactive:
    def __init__(self):
        self._lock = threading.Lock()
        self.flight = None
        self.ended = []    # recent endings, newest last: {"platform", "reason", "time"}
        self._published = 0.0
        self._gains = None   # each platform's LQR gain (info)

    def info(self):
        """The controller as the page shows it: the parameters and each platform's gain (thrust: the lab's
        calibrated thrust of each thruster where there is one, calibration.py)."""
        from .calibration import controller_params   # (calibration.py imports this module)
        if self._gains is None:   # (they don't depend on the thrust)
            self._gains = dict((n, platform_gains(p, DEFAULTS["data_rate"])) for n, p in DEFAULTS["platforms"].items())
        plats = dict((n, dict(p, K=self._gains[n], thrust=controller_params(n)["platforms"][n]["thrust"]))
                     for n, p in DEFAULTS["platforms"].items())
        return {"sample_rate": DEFAULTS["sample_rate"], "period": 1.0 / DEFAULTS["sample_rate"],
                "max_iters": DEFAULTS["max_iters"], "tol": DEFAULTS["tol"], "platforms": plats,
                "table": TABLE, "margin": MARGIN, "min_separation": MIN_SEPARATION}

    def status(self):
        with self._lock:
            f = self.flight
            out = {"active": f is not None, "ended": list(self.ended), "period": 1.0 / DEFAULTS["sample_rate"]}
            if f is None:
                return out
            plats = {}
            for name, s in f.sessions.items():
                m = s.measured
                plats[name] = {
                    "goal": [s.goal[0], s.goal[1], math.degrees(s.goal[2])],
                    "measured": m and [m[0], m[1], math.degrees(m[2])],
                    "error": m and [m[0] - s.goal[0], m[1] - s.goal[1], math.degrees(wrap(m[2] - s.goal[2]))],
                    "u": s.u, "body": s.body, "duty": [100 * d for d in s.duty],
                    "lost": s.lost_since is not None, "started": s.started,
                    "step_ms": round(s.step_ms, 1), "late": s.late, "steps": s.steps}
            out.update({"client": f.client, "by": f.requester, "started": f.started, "platforms": plats})
            return out

    def _publish(self, force=True):
        now = time.monotonic()
        if not force and now - self._published < PUBLISH_PERIOD:
            return
        self._published = now
        hub.publish("interactive", self.status())

    # ---- requests from the page

    def start(self, goals, client, requester):
        """Fly the platforms in GOALS ({NAME: {"x", "y", "att"} or None for where it is now}). Those
        already flying get the new goal; the others join the flight, all or none of them."""
        if not isinstance(goals, dict) or not goals:
            raise ApiError("Say which platforms to fly: {\"RED\": {\"x\": ..., \"y\": ..., \"att\": ...}, ...}.")
        if not client:
            raise ApiError("Interactive Mode needs the page's id (client).")
        goals = {parse_name(n): g for n, g in goals.items()}
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first: Interactive Mode flies the platforms from their "
                           "PhaseSpace poses.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; Interactive Mode waits until it has stopped.")
        from .calibration import calibrator   # (calibration.py imports this module)
        if calibrator.active():
            raise ApiError("A calibration is running (Calibration tab); Interactive Mode waits until it has finished.")
        now = time.time()
        parsed = {}
        for name, g in goals.items():
            pose = fresh_pose(name, now)
            if pose is None:
                raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)." % shown(name))
            parsed[name] = parse_goal(g) if g else [pose[0], pose[1], pose[2]]
        with self._lock:
            f = self.flight
            if f is not None and f.client != client:
                raise ApiError("Interactive Mode is flying the %s for %s; stop it first."
                               % (shown_list(sorted(f.sessions)), f.requester))
            flying = dict((n, s.goal) for n, s in f.sessions.items()) if f else {}
        check_separation(dict(flying, **parsed), now)

        # The platforms already flying get their new goal; the others get a controller process and
        # are claimed, all or none
        new = [n for n in sorted(parsed) if n not in flying]
        flight = f or Flight(client, requester)
        # (each with the lab's calibrated thrust of its thrusters, where there is one)
        from .calibration import controller_params
        processes = dict((n, ControllerProcess(n, controller_params(n))) for n in new)   # they start side by side
        claimed = []
        try:
            for name in new:
                processes[name].wait_ready()
            for name in new:
                s = Session(name, flight, parsed[name], processes[name])
                hardware.get(name).claim(s, requester)   # raises if the platform can't be driven now
                claimed.append(s)
        except Exception as e:   # ApiError: not ready; anything else: the processes failed
            for s in claimed:
                hardware.get(s.name).release(s, "Interactive Mode did not start (%s); all off" % e)
            for p in processes.values():
                p.close()
            raise
        with self._lock:
            if self.flight is not None and self.flight is not flight:
                lost = True
            else:
                lost = False
                self.flight = flight
                flight.beat = time.time()
                for name, goal in parsed.items():
                    if name in flight.sessions:
                        flight.sessions[name].goal = goal
                for s in claimed:
                    flight.sessions[s.name] = s
        if lost:   # another page started a flight meanwhile
            for s in claimed:
                hardware.get(s.name).release(s, "Interactive Mode did not start (another page started first); all off")
                s.process.close()
            raise ApiError("Another page started Interactive Mode at the same moment; try again.")
        for s in claimed:
            g = s.goal
            hw_log("%s: Interactive Mode started, goal x %.3f m, y %.3f m, %.1f deg (by %s)"
                   % (shown(s.name), g[0], g[1], math.degrees(g[2]), requester))
            threading.Thread(target=self._run, args=(s,), daemon=True).start()
        self._publish()
        return self.status()

    def set_goal(self, name, goal, client, final=True):
        name = parse_name(name)
        goal = parse_goal(goal)
        with self._lock:
            f = self._own(client)
            s = f.sessions.get(name)
            if s is None:
                raise ApiError("Interactive Mode isn't flying the %s." % shown(name))
            others = dict((n, o.goal) for n, o in f.sessions.items() if n != name)
            f.beat = time.time()
        check_separation(dict(others, **{name: goal}), time.time(), moved=name)
        with self._lock:
            s.goal = goal
        if final:
            hw_log("%s: Interactive Mode goal x %.3f m, y %.3f m, %.1f deg"
                   % (shown(name), goal[0], goal[1], math.degrees(goal[2])))
        self._publish()
        return self.status()

    def heartbeat(self, client):
        with self._lock:
            f = self.flight
            if f is not None and f.client == client:
                f.beat = time.time()
                return {"active": True}
        return {"active": False}

    def stop(self, requester, name=None, reason=None):
        """Stop one platform (NAME), or the whole flight."""
        name = parse_name(name) if name else None
        with self._lock:
            f = self.flight
            if f is None:
                stopping = []
            elif name:
                s = f.sessions.pop(name, None)
                stopping = [s] if s else []
                if not f.sessions:
                    self.flight = None
            else:
                stopping = list(f.sessions.values())
                f.sessions = {}
                self.flight = None
        for s in stopping:
            self._finish(s, reason or "stopped by %s" % requester)
        return self.status()

    def _own(self, client):
        """The flight, if the page CLIENT flies it (call with the lock held)."""
        f = self.flight
        if f is None:
            raise ApiError("Interactive Mode isn't flying anything.")
        if f.client != client:
            raise ApiError("Interactive Mode is flying the %s for %s." % (shown_list(sorted(f.sessions)), f.requester))
        return f

    # ---- the loops, one per platform

    def _current(self, s):
        """Whether S is still in the flight (call with the lock held)."""
        return self.flight is s.flight and s.flight.sessions.get(s.name) is s

    def _run(self, s):
        p = hardware.get(s.name)
        Ts = s.controller.period
        next_step = time.monotonic()
        reason = None
        while reason is None:
            with self._lock:
                if not self._current(s):
                    return   # stopped (stop() has switched it off)
            t0 = time.monotonic()
            try:
                reason = self._step(s, p)
            except Exception as e:  # never leave the thrusters on after a bug
                reason = "internal error: %s: %s" % (type(e).__name__, e)
            took = (time.monotonic() - t0) * 1000
            s.step_ms = 0.9 * s.step_ms + 0.1 * took if s.steps else took
            s.steps += 1
            self._publish(force=False)
            next_step += Ts
            delay = next_step - time.monotonic()
            if delay > 0:
                time.sleep(delay)
            else:
                s.late += 1
                next_step = time.monotonic()
        if reason == "heartbeat":
            # The page is gone: the whole flight stops (each loop notices; the first stops them all)
            self.stop("the console", reason="the page flying it stopped answering (closed, asleep, or off the network)")
            return
        with self._lock:
            if self._current(s):
                del s.flight.sessions[s.name]
                if not s.flight.sessions:
                    self.flight = None
        self._finish(s, reason)

    def _step(self, s, p):
        """One control step; returns why this platform must stop ("heartbeat": the whole flight), or None."""
        now = time.time()
        if now - s.flight.beat > HEARTBEAT_TIMEOUT:
            return "heartbeat"
        pose = fresh_pose(s.name, now)
        if pose is None:
            s.lost_since = s.lost_since or now
            if now - s.lost_since > LOST_TIMEOUT:
                return "PhaseSpace lost the %s for more than %g s" % (shown(s.name), LOST_TIMEOUT)
            s.duty = [0.0] * NUM_THRUSTERS
            return None if p.drive(s, s.duty) else self._taken(p)
        s.lost_since = None
        if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
            return "PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)" % (shown(s.name), pose[0], pose[1])
        with self._lock:
            goal = list(s.goal)
        u, duty, body = s.process.step(pose, goal)
        if not p.drive(s, [100 * d for d in duty]):
            return self._taken(p)
        s.measured, s.u, s.body, s.duty = pose, u, body, duty
        return None

    @staticmethod
    def _taken(p):
        la = p.status()["last_action"]
        return "the %s was taken back (%s)" % (shown(p.name), la["text"] if la else "switched off")

    def _finish(self, s, reason):
        with self._lock:
            if s.done:
                return
            s.done = True
            self.ended = (self.ended + [{"platform": s.name, "reason": reason, "time": time.time()}])[-ENDED_KEPT:]
        hardware.get(s.name).release(s, "Interactive Mode stopped (%s); all off" % reason)
        s.process.close()
        hw_log("%s: Interactive Mode ended after %.0f s: %s" % (shown(s.name), time.time() - s.started, reason))
        self._publish()

    def shutdown(self):
        self.stop("the console", reason="the console is shutting down")


interactive = Interactive()
