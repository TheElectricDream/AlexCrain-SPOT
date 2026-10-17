"""Interactive Mode, on the lab console's PhaseSpace tab.

Fly one or more platforms, each to a pose chosen in the 3D view. The
template diagram's own controller (control.py: its default LQR law and
thruster mixer, ported from +spot/+control) closes each platform's
loop here on the lab console. Every baseRate (0.05 s at SPOT's default
20 Hz, as the diagram's controller and thruster code run) it takes the
platform's pose and velocity from PhaseSpace (the monitor), computes
the 8 duty cycles, and the hardware driver sends them straight to the
platform's Teensy. Each platform has its own loop (a thread), so one
platform's trouble doesn't hold up the others, and its own controller
process (ControllerProcess), so the platforms' mixers run side by side
on separate CPU cores.

A flight belongs to the page that started it: only that page adds
platforms or moves goals, and it keeps the flight alive with
heartbeats. Any page can stop it.

Everything on a platform goes off, and that platform leaves the
flight, when
  - its toggle is switched off on the page, or the hardware tab
    switches it off (Esc there, or POST /api/hardware/off)
  - PhaseSpace hasn't seen it for LOST_TIMEOUT (its thrusters are off
    already after STALE_HOLD)
  - PhaseSpace sees it off the table
  - an experiment starts, another program opens its Teensy, or the
    link to it is lost (hardware.py)
  - its controller process doesn't answer within STEP_TIMEOUT (or a
    step fails).
Every platform in the flight goes off when
  - Stop all is pressed on the page, or Esc on the PhaseSpace tab
  - the page flying them stops sending heartbeats for
    HEARTBEAT_TIMEOUT (closed, asleep, off the network)
  - the console stops.
And each Teensy switches everything off by itself when packets stop
for 2 s.

Goals stay MARGIN inside the table's edges, and at least
MIN_SEPARATION from each other and from platforms resting on the
table. The controller flies each platform straight at its goal with no
idea of the others, so their paths can still cross: that is for
whoever places the goals to avoid.

Part of the SPOT Console (webapp/app.py starts it).
"""

import math
import multiprocessing
import threading
import time

from . import control
from .common import ApiError, shown, shown_list
from .constants import C
from .control import DEFAULTS, Controller, platform_gains
from .events import hub
from .experiment import experiment
from .hardware import NUM_THRUSTERS, hardware, hw_log
from .phasespace import monitor, streamer


# The lab's parameters (Constants.m): the table top [m]; how far
# inside its edges goals stay [m] (half a platform + 5 cm); the least
# distance between goals, centre to centre [m] (0.3 m square platforms,
# 0.42 m across the corners); how long without a fresh PhaseSpace pose
# the thrusters go off (pucks stay on) and, longer, the platform stops
# [s]; and how long without a heartbeat from the flying page everything
# stops [s] (it sends one every INTERACTIVE_HEARTBEAT_PERIOD; a
# background tab's timers may only run once a second).
TABLE = (C.TABLE_X, C.TABLE_Y)
MARGIN = C.INTERACTIVE_MARGIN
MIN_SEPARATION = C.PLATFORM_CLEARANCE
STALE_HOLD = float(C.INTERACTIVE_STALE_HOLD)
LOST_TIMEOUT = float(C.INTERACTIVE_LOST_TIMEOUT)
HEARTBEAT_TIMEOUT = float(C.INTERACTIVE_HEARTBEAT_TIMEOUT)
PUBLISH_PERIOD = 0.2      # [s] how often the pages get the loops' status
ENDED_KEPT = 6            # how many recent endings the status lists
START_TIMEOUT = 15.0      # [s] for a controller process to start
STEP_TIMEOUT = 1.0        # [s] for a controller process to answer one step
EXPERIMENT_BUSY = ("deploying", "starting", "running", "stopping")
STEP_SMOOTHING = 0.9      # weight of the previous step time in the smoothed step_ms


# Goals and poses


def wrap(angle):
    """Return an angle in (-pi, pi], as the diagram's Wrap block."""
    return math.atan2(math.sin(angle), math.cos(angle))


def parse_goal(body):
    """Return a goal from a request.

    The angle is not wrapped: the page sends it continuously, so its
    rate stays smooth.

    Args:
        body: {"x", "y" [m], "att" [deg]}.

    Returns:
        [x, y, theta] [m, m, rad], x and y kept inside the margin.

    Raises:
        ApiError: if a value is missing or not a finite number.
    """
    try:
        x, y, att = (float(body[k]) for k in ("x", "y", "att"))
    except (KeyError, TypeError, ValueError):
        raise ApiError("A goal needs x and y [m] and att [deg].")
    if not all(math.isfinite(v) for v in (x, y, att)) or abs(att) > 1e5:
        raise ApiError("The goal must be finite numbers.")
    return [min(max(x, MARGIN), TABLE[0] - MARGIN), min(max(y, MARGIN), TABLE[1] - MARGIN),
            math.radians(att)]


def parse_name(name):
    """Return a platform key from a request ("red" -> "RED").

    Raises:
        ApiError: if it isn't RED, BLACK or BLUE.
    """
    name = str(name or "").strip().upper()
    if name not in DEFAULTS["platforms"]:
        raise ApiError("Unknown platform %r (use RED, BLACK or BLUE)." % name)
    return name


def fresh_pose(name, now):
    """Return a platform's state from PhaseSpace's newest frame.

    Returns:
        [x, y, theta, xdot, ydot, thetadot], theta wrapped; None if the
        frame is older than STALE_HOLD or doesn't have the platform.
    """
    with monitor._lock:
        frame = monitor.latest
    if not frame or now - frame["received"] > STALE_HOLD:
        return None
    craft = frame["crafts"].get(name)
    if not craft or not craft["tracked"]:
        return None
    pose = [craft["x"], craft["y"], craft["att"], craft["vx"], craft["vy"], craft["w"]]
    if any(v is None for v in pose):
        return None

    # Wrapped, as the diagram's "Store <PLATFORM> Data" does before
    # control.
    pose[2] = wrap(pose[2])
    return pose


def check_separation(goals, now, moved=None):
    """Check that goals keep the platforms apart.

    Args:
        goals: {NAME: [x, y, theta]}.
        now: The time, for PhaseSpace's poses.
        moved: The platform whose goal is being moved, named first in
            the message.

    Raises:
        ApiError: if two goals, or a goal and a platform resting on the
            table (tracked by PhaseSpace and not in goals), are closer
            than MIN_SEPARATION.
    """
    # The goals (the moved one first), then the resting platforms.
    names = sorted(goals, key=lambda n: (n != moved, n))
    points = [(n, goals[n][0], goals[n][1], "goal") for n in names]
    for n in DEFAULTS["platforms"]:
        pose = None if n in goals else fresh_pose(n, now)
        if pose is not None:
            points.append((n, pose[0], pose[1], "resting place"))

    # Every pair.
    for i, (a, xa, ya, _) in enumerate(points):
        for b, xb, yb, what in points[i + 1:]:
            dist = math.hypot(xa - xb, ya - yb)
            if dist < MIN_SEPARATION:
                raise ApiError("The %s's goal is %.2f m from the %s's %s; keep them at least "
                               "%.2f m apart (centre to centre) so the platforms don't touch."
                               % (shown(a), dist, shown(b), what, MIN_SEPARATION))


# Controller processes, sessions and flights


class ControllerProcess:
    """One platform's Controller (control.py) in a process of its own.

    Run by control.serve. Python runs one thread at a time per process:
    in the console's own process, the platforms' mixers would take
    turns on one CPU core (a step at full thrust takes tens of ms
    each); in their own processes they run side by side. Started fresh
    ("spawn"), so nothing of the console's threads is inherited.
    """

    _context = multiprocessing.get_context("spawn")

    def __init__(self, name, params=None):
        """Start the process without waiting (see wait_ready)."""
        self.name = name
        self.conn, child = self._context.Pipe()
        self.proc = self._context.Process(target=control.serve, args=(child, name, params),
                                          name="spot-controller-" + name, daemon=True)
        self.proc.start()
        child.close()

    def wait_ready(self):
        """Wait for the process to start.

        Raises:
            ApiError: if it doesn't report ready within START_TIMEOUT.
        """
        if not self.conn.poll(START_TIMEOUT) or self.conn.recv() != "ready":
            raise ApiError("The %s's controller process didn't start within %g s."
                           % (shown(self.name), START_TIMEOUT))

    def step(self, measured, desired):
        """Run Controller.step in the process.

        Returns:
            (u, duty cycles, body-frame command).

        Raises:
            RuntimeError: if it doesn't answer within STEP_TIMEOUT.
        """
        self.conn.send((measured, desired))
        if not self.conn.poll(STEP_TIMEOUT):
            raise RuntimeError("its controller process didn't answer within %g s"
                               % STEP_TIMEOUT)
        return self.conn.recv()

    def close(self):
        """Ask the process to end; terminate it if it hasn't in 1 s."""
        try:
            self.conn.send(None)
        except (OSError, ValueError):
            pass
        self.proc.join(1)
        if self.proc.is_alive():
            self.proc.terminate()
        self.conn.close()


class Session:
    """One platform being flown: its goal and last control step."""

    label = "Interactive Mode"   # what the hardware tab says drives the platform

    def __init__(self, name, flight, goal, process):
        """Start a session for a platform in a flight.

        Args:
            name: The platform's key.
            flight: The Flight it belongs to.
            goal: [x, y, theta] [m, m, rad].
            process: Its ControllerProcess.
        """
        self.name = name
        self.flight = flight
        self.goal = goal              # [x, y, theta] [m, m, rad]
        self.controller = Controller(name)   # its parameters here; it runs in process
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
        """Start an empty flight for a page.

        Args:
            client: The page's own random id.
            requester: Who flies it (shown on the pages).
        """
        self.client = client
        self.requester = requester
        self.started = self.beat = time.time()
        self.sessions = {}            # NAME -> Session


# Interactive Mode


class Interactive:
    """Interactive Mode: the flight, its loops, the page's requests."""

    def __init__(self):
        """Start with no flight."""
        self._lock = threading.Lock()
        self.flight = None
        self.ended = []    # recent endings, newest last: {"platform", "reason", "time"}
        self._published = 0.0
        self._gains = None   # each platform's LQR gain (info)

    def info(self):
        """Return the controller as the page shows it.

        The parameters and each platform's gain; thrust is the lab's
        calibrated thrust of each thruster where there is one
        (calibration.py).
        """
        # Imported here: calibration.py imports this module.
        from .calibration import controller_params

        # The gains don't depend on the thrust: computed once.
        if self._gains is None:
            self._gains = dict((name, platform_gains(params, DEFAULTS["data_rate"]))
                               for name, params in DEFAULTS["platforms"].items())

        plats = {}
        for name, params in DEFAULTS["platforms"].items():
            mine = controller_params(name)["platforms"][name]
            plats[name] = dict(params, K=self._gains[name], thrust=mine["thrust"],
                               thrust_decay=mine["thrust_decay"],
                               decay_calibrated=mine["thrust_decay"] != params["thrust_decay"])
        return {"sample_rate": DEFAULTS["sample_rate"], "period": 1.0 / DEFAULTS["sample_rate"],
                "max_iters": DEFAULTS["max_iters"], "tol": DEFAULTS["tol"], "platforms": plats,
                "table": TABLE, "margin": MARGIN, "min_separation": MIN_SEPARATION}

    def status(self):
        """Return the flight's state and recent endings."""
        with self._lock:
            flight = self.flight
            out = {"active": flight is not None, "ended": list(self.ended),
                   "period": 1.0 / DEFAULTS["sample_rate"]}
            if flight is None:
                return out

            # Each platform: goal, measured pose and error (degrees),
            # commands and step timing.
            plats = {}
            for name, session in flight.sessions.items():
                measured = session.measured
                goal = session.goal
                plats[name] = {
                    "goal": [goal[0], goal[1], math.degrees(goal[2])],
                    "measured": measured and [measured[0], measured[1],
                                              math.degrees(measured[2])],
                    "error": measured and [measured[0] - goal[0], measured[1] - goal[1],
                                           math.degrees(wrap(measured[2] - goal[2]))],
                    "u": session.u, "body": session.body,
                    "duty": [100 * d for d in session.duty],
                    "lost": session.lost_since is not None, "started": session.started,
                    "step_ms": round(session.step_ms, 1), "late": session.late,
                    "steps": session.steps}
            out.update({"client": flight.client, "by": flight.requester,
                        "started": flight.started, "platforms": plats})
            return out

    def _publish(self, force=True):
        """Send the status to the pages.

        Unless forced, at most every PUBLISH_PERIOD.
        """
        now = time.monotonic()
        if not force and now - self._published < PUBLISH_PERIOD:
            return
        self._published = now
        hub.publish("interactive", self.status())

    # Requests from the page

    def start(self, goals, client, requester):
        """Fly platforms.

        Those already flying get the new goal; the others join the
        flight, all or none of them.

        Args:
            goals: {NAME: {"x", "y", "att"}, or None for where it is
                now}.
            client: The page's id.
            requester: Who asked.

        Raises:
            ApiError: if PhaseSpace isn't streaming or doesn't see a
                platform, an experiment or calibration is running,
                another page flies, goals are too close, or a platform
                can't be driven now.
        """
        # The request.
        if not isinstance(goals, dict) or not goals:
            raise ApiError("Say which platforms to fly: "
                           "{\"RED\": {\"x\": ..., \"y\": ..., \"att\": ...}, ...}.")
        if not client:
            raise ApiError("Interactive Mode needs the page's id (client).")
        goals = {parse_name(n): goal for n, goal in goals.items()}

        # Nothing else may be driving the platforms.
        if not streamer.status()["running"]:
            raise ApiError("Start PhaseSpace streaming first: Interactive Mode flies the "
                           "platforms from their PhaseSpace poses.")
        if experiment.status()["phase"] in EXPERIMENT_BUSY:
            raise ApiError("An experiment is under way; Interactive Mode waits until it has "
                           "stopped.")
        from .calibration import calibrator   # (calibration.py imports this module)
        if calibrator.active():
            raise ApiError("A calibration is running (Calibration tab); Interactive Mode waits "
                           "until it has finished.")

        # Each goal; none: where the platform is now.
        now = time.time()
        parsed = {}
        for name, goal in goals.items():
            pose = fresh_pose(name, now)
            if pose is None:
                raise ApiError("PhaseSpace doesn't see the %s right now (on the table, LEDs on?)."
                               % shown(name))
            parsed[name] = parse_goal(goal) if goal else [pose[0], pose[1], pose[2]]

        # Only the page flying already may add to its flight.
        with self._lock:
            current = self.flight
            if current is not None and current.client != client:
                raise ApiError("Interactive Mode is flying the %s for %s; stop it first."
                               % (shown_list(sorted(current.sessions)), current.requester))
            flying = (dict((n, session.goal) for n, session in current.sessions.items())
                      if current else {})
        check_separation(dict(flying, **parsed), now)

        # The platforms already flying get their new goal; the others
        # get a controller process (with the lab's calibrated thrusts,
        # where there are some; they start side by side) and are
        # claimed, all or none.
        new = [n for n in sorted(parsed) if n not in flying]
        flight = current or Flight(client, requester)
        from .calibration import controller_params
        processes = dict((n, ControllerProcess(n, controller_params(n))) for n in new)
        claimed = []
        try:
            for name in new:
                processes[name].wait_ready()
            for name in new:
                session = Session(name, flight, parsed[name], processes[name])
                # Raises if the platform can't be driven now.
                hardware.get(name).claim(session, requester)
                claimed.append(session)
        except Exception as e:
            # ApiError: not ready; anything else: the processes failed.
            for session in claimed:
                hardware.get(session.name).release(
                    session, "Interactive Mode did not start (%s); all off" % e)
            for proc in processes.values():
                proc.close()
            raise

        # Join the flight, unless another page started one meanwhile.
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
                for session in claimed:
                    flight.sessions[session.name] = session
        if lost:
            for session in claimed:
                hardware.get(session.name).release(
                    session,
                    "Interactive Mode did not start (another page started first); all off")
                session.process.close()
            raise ApiError("Another page started Interactive Mode at the same moment; try again.")

        # A loop for each new platform.
        for session in claimed:
            goal = session.goal
            hw_log("%s: Interactive Mode started, goal x %.3f m, y %.3f m, %.1f deg (by %s)"
                   % (shown(session.name), goal[0], goal[1], math.degrees(goal[2]), requester))
            threading.Thread(target=self._run, args=(session,), daemon=True).start()
        self._publish()
        return self.status()

    def set_goal(self, name, goal, client, final=True):
        """Move one platform's goal.

        Args:
            name: The platform.
            goal: {"x", "y", "att"}.
            client: The page's id (it must fly the platform).
            final: False while the goal is being dragged (not logged).

        Raises:
            ApiError: if the page doesn't fly it, or the goal is too
                close to another.
        """
        name = parse_name(name)
        goal = parse_goal(goal)
        with self._lock:
            flight = self._own(client)
            session = flight.sessions.get(name)
            if session is None:
                raise ApiError("Interactive Mode isn't flying the %s." % shown(name))
            others = dict((n, o.goal) for n, o in flight.sessions.items() if n != name)
            flight.beat = time.time()
        check_separation(dict(others, **{name: goal}), time.time(), moved=name)
        with self._lock:
            session.goal = goal
        if final:
            hw_log("%s: Interactive Mode goal x %.3f m, y %.3f m, %.1f deg"
                   % (shown(name), goal[0], goal[1], math.degrees(goal[2])))
        self._publish()
        return self.status()

    def heartbeat(self, client):
        """Keep the flight alive; return whether the page flies it."""
        with self._lock:
            flight = self.flight
            if flight is not None and flight.client == client:
                flight.beat = time.time()
                return {"active": True}
        return {"active": False}

    def stop(self, requester, name=None, reason=None):
        """Stop one platform, or the whole flight.

        Args:
            requester: Who asked (for the reason).
            name: The platform; None for all.
            reason: Why; default "stopped by <requester>".
        """
        name = parse_name(name) if name else None
        with self._lock:
            flight = self.flight
            if flight is None:
                stopping = []
            elif name:
                session = flight.sessions.pop(name, None)
                stopping = [session] if session else []
                if not flight.sessions:
                    self.flight = None
            else:
                stopping = list(flight.sessions.values())
                flight.sessions = {}
                self.flight = None
        for session in stopping:
            self._finish(session, reason or "stopped by %s" % requester)
        return self.status()

    def _own(self, client):
        """Return the flight, if the page client flies it.

        Call with the lock held.

        Raises:
            ApiError: if nothing is flying, or another page flies it.
        """
        flight = self.flight
        if flight is None:
            raise ApiError("Interactive Mode isn't flying anything.")
        if flight.client != client:
            raise ApiError("Interactive Mode is flying the %s for %s."
                           % (shown_list(sorted(flight.sessions)), flight.requester))
        return flight

    # The loops, one per platform

    def _current(self, session):
        """Return whether a session is still flying (lock held)."""
        return (self.flight is session.flight
                and session.flight.sessions.get(session.name) is session)

    def _run(self, session):
        """Run a platform's loop every baseRate until it must stop."""
        driver = hardware.get(session.name)
        Ts = session.controller.period
        next_step = time.monotonic()
        reason = None
        while reason is None:
            # Stopped (stop() has switched it off).
            with self._lock:
                if not self._current(session):
                    return

            # One step; never leave the thrusters on after a bug.
            t0 = time.monotonic()
            try:
                reason = self._step(session, driver)
            except Exception as e:
                reason = "internal error: %s: %s" % (type(e).__name__, e)

            # Step time, smoothed.
            took = (time.monotonic() - t0) * 1000
            if session.steps:
                session.step_ms = STEP_SMOOTHING * session.step_ms + 0.1 * took
            else:
                session.step_ms = took
            session.steps += 1
            self._publish(force=False)

            # Wait for the next step; if late, count it and carry on.
            next_step += Ts
            delay = next_step - time.monotonic()
            if delay > 0:
                time.sleep(delay)
            else:
                session.late += 1
                next_step = time.monotonic()

        # The page is gone: the whole flight stops (each loop notices;
        # the first stops them all).
        if reason == "heartbeat":
            self.stop("the console", reason="the page flying it stopped answering "
                                            "(closed, asleep, or off the network)")
            return

        # Only this platform stops.
        with self._lock:
            if self._current(session):
                del session.flight.sessions[session.name]
                if not session.flight.sessions:
                    self.flight = None
        self._finish(session, reason)

    def _step(self, session, driver):
        """Run one control step.

        Returns:
            Why this platform must stop ("heartbeat": the whole
            flight), or None to carry on.
        """
        # The page must still be there.
        now = time.time()
        if now - session.flight.beat > HEARTBEAT_TIMEOUT:
            return "heartbeat"

        # Not seen: thrusters off, until lost for too long.
        pose = fresh_pose(session.name, now)
        if pose is None:
            session.lost_since = session.lost_since or now
            if now - session.lost_since > LOST_TIMEOUT:
                return "PhaseSpace lost the %s for more than %g s" % (shown(session.name),
                                                                       LOST_TIMEOUT)
            session.duty = [0.0] * NUM_THRUSTERS
            return None if driver.drive(session, session.duty) else self._taken(driver)
        session.lost_since = None

        # On the table.
        if not (0 <= pose[0] <= TABLE[0] and 0 <= pose[1] <= TABLE[1]):
            return "PhaseSpace sees the %s off the table (x %.2f m, y %.2f m)" % (
                shown(session.name), pose[0], pose[1])

        # Control, and the duty cycles to the Teensy.
        with self._lock:
            goal = list(session.goal)
        u, duty, body = session.process.step(pose, goal)
        if not driver.drive(session, [100 * d for d in duty]):
            return self._taken(driver)
        session.measured, session.u, session.body, session.duty = pose, u, body, duty
        return None

    @staticmethod
    def _taken(driver):
        """Return why a platform was taken back (its last action)."""
        last = driver.status()["last_action"]
        return "the %s was taken back (%s)" % (shown(driver.name),
                                               last["text"] if last else "switched off")

    def _finish(self, session, reason):
        """End a session once: log why, switch off, end its process."""
        with self._lock:
            if session.done:
                return
            session.done = True
            self.ended = (self.ended + [{"platform": session.name, "reason": reason,
                                         "time": time.time()}])[-ENDED_KEPT:]
        hardware.get(session.name).release(session,
                                           "Interactive Mode stopped (%s); all off" % reason)
        session.process.close()
        hw_log("%s: Interactive Mode ended after %.0f s: %s"
               % (shown(session.name), time.time() - session.started, reason))
        self._publish()

    def shutdown(self):
        """Stop the flight (the console is shutting down)."""
        self.stop("the console", reason="the console is shutting down")


interactive = Interactive()
