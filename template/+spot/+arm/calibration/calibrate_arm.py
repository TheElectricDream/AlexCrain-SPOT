#!/usr/bin/env python3
"""Calibrate the SPOT arm's joint friction and inertia with PWM alone.

The servos run in PWM (voltage) control, as SPOT drives them: each
command is the share of the supply voltage across the motor (885 =
100 %). Their current readings are not used: at the arm's small
torques they are a few units of 3.36 mA, too coarse and noisy. At the
joint, the motor gives

    torque_per_volt (V - back_emf w)

so at rest a voltage is a torque, and turning, the back-EMF brakes
the joint. The datasheet gives torque_per_volt (stall torque / supply)
and back_emf (supply / no-load speed).

Tests, for each joint in turn (the other joints hold their position in
position mode):
  breakaway  the PWM ramped slowly from zero, each way, until the
             joint moves: the breakaway (static) friction, several
             times per direction
  sweep      fixed PWM levels about the breakaway, back and forth
             across a window, each started with a short kick (below
             the breakaway the back-EMF's braking lets a joint that
             is moving keep sliding slowly, which shows the Stribeck
             drop): the steady speed at each. The voltage
             a speed needs is friction plus back-EMF,
               V = Vc + (Vs - Vc) exp(-(w/ws)^2) + k w,
             from which the Coulomb friction, the Stribeck speed and
             the speed coefficient k (back-EMF and viscous friction
             together, which voltage control can't tell apart; the
             datasheet's back-EMF splits them)
  inertia    a PWM step from rest, each way: how fast the speed
             settles (its time constant T) gives the joint's inertia,
             J = T torque_per_volt k (link and rotor); SPOT subtracts
             the link's part to get the rotor's (armature)
  steps      position mode with SPOT's servo settings (P 400, I 0,
             D 200, profile velocity 20): goal steps, logged for
             spot.arm.compareSteps (the simulated servo against the
             real one)

Writes arm_calibration.json (version 2, "control": "pwm") for SPOT's
arm model (copy it to the project's folder, next to
spot_settings.json; spot.arm.parameters reads it, see
spot.arm.loadCalibration), and the raw data as CSV and JSON files.

Before running:
  - the platform rests on the table (not floating) or is held: the
    arm's motion must not move it
  - each tested joint will move within +-WINDOW degrees (default 25)
    of where it is: clear the space, and start with the joints away
    from their limits
  - keep the power switch at hand; Ctrl+C stops at once (all torque
    off)
  - pip install dynamixel-sdk (the script needs nothing else)

Examples:
  python3 calibrate_arm.py                  # all tests, joints 1-3
  python3 calibrate_arm.py --joints 3 --tests breakaway sweep
  python3 calibrate_arm.py --fit runs/20261016_101500  # fit again
  python3 calibrate_arm.py --simulate       # no hardware (a model)
  python3 calibrate_arm.py --yes            # no "Press Enter" (the
                                            # SPOT Console runs it so)

Progress: a line "PROGRESS {json}" before each test ({"joint",
"test", "index", "total"}), for the Calibration tab. Exit code 0 when
arm_calibration.json was written; 1 when stopped or aborted.

Units: the servos' (protocol 2.0, MX and X series): position 4096
pulses/rev, velocity 0.229 rev/min, PWM 885 = 100 % of the supply.
Python 3.6 or newer.
"""

import argparse
import collections
import csv
import datetime
import json
import math
import os
import random
import signal
import sys
import time

# Servo registers: name -> (address, bytes) in protocol 2.0's
# control table.
ADDR = {
    "model_number": (0, 2), "drive_mode": (10, 1), "operating_mode": (11, 1),
    "pwm_limit": (36, 2), "velocity_limit": (44, 4),
    "max_position": (48, 4), "min_position": (52, 4), "torque_enable": (64, 1),
    "velocity_i": (76, 2), "velocity_p": (78, 2), "position_d": (80, 2), "position_i": (82, 2),
    "position_p": (84, 2), "ff2": (88, 2), "ff1": (90, 2),
    "goal_pwm": (100, 2), "goal_velocity": (104, 4),
    "profile_acceleration": (108, 4), "profile_velocity": (112, 4), "goal_position": (116, 4),
    "present_input_voltage": (144, 2), "present_temperature": (146, 1),
}

# The state read each time: realtime tick (2), moving (1), moving
# status (1), PWM (2), current (2, not used), velocity (4), position
# (4).
STATE_START = 120
STATE_LEN = 16

MODE_POSITION = 3
MODE_PWM = 16
PWM_FULL = 885                       # PWM at 100 % of the supply
CENTER_PULSE = 2048                  # [pulses] joint straight out
PULSES = 4096 / (2 * math.pi)        # [pulses/rad]
VEL_UNIT = 0.229 * 2 * math.pi / 60  # [rad/s] per velocity unit
VOLT_UNIT = 0.1                      # [V] per Present Input Voltage unit
MAX_TEMPERATURE = 65                 # [C] a servo this hot stops the run

# A sweep pass is steady when the joint's mean speed over
# STEADY_WINDOW (from its position: a pulse is 0.0015 rad, finer than
# the velocity reading's 0.024 rad/s) changes by at most STEADY_TOL of
# itself over STEADY_LAG; its samples count after STEADY_HOLD of that.
STEADY_WINDOW = 0.5                  # [s]
STEADY_LAG = 0.25                    # [s]
STEADY_TOL = 0.08
STEADY_HOLD = 0.3                    # [s]
MIN_SAMPLES = 10                     # steady samples for a sweep point

# Simulating a PWM step (fit_step): the longest sub-step, and the
# longest dead time searched between the command and its effect.
SIM_STEP = 2.5e-4                    # [s]
MAX_DELAY = 4e-3                     # [s]

# A breakaway is where the joint first moved this far from where its
# ramp started (more than the encoder's last-pulse flicker).
BREAK_PULSES = 2

# Each sweep pass starts with a kick this far above the breakaway PWM,
# for this long, so that the joint is moving when the PWM drops to its
# level.
KICK_PWM = 20
KICK_TIME = 0.15                     # [s]

# The servos' models, and their datasheet values at 12 V: stall
# torque [N m] and no-load speed [rev/min].
MODELS = {311: "MX-64(2.0)", 321: "MX-106(2.0)", 30: "MX-28(2.0)", 1020: "XM430-W350",
          1120: "XM540-W270"}
DATASHEET = {311: (6.0, 63), 321: (8.4, 45), 30: (2.5, 55), 1020: (4.1, 46),
             1120: (10.6, 30)}
DATASHEET_VOLTS = 12.0               # [V]
NAMES = {1: "shoulder", 2: "elbow", 3: "wrist"}

# How SPOT's old position-mode block set the servos up (its P/I/D
# gains and profile), for holding joints and the steps test.
SPOT_POSITION = {"position_p": 400, "position_i": 0, "position_d": 200, "profile_velocity": 20,
                 "profile_acceleration": 0, "drive_mode": 0}
SAVED_REGISTERS = ["operating_mode", "drive_mode", "pwm_limit", "velocity_limit", "velocity_i",
                   "velocity_p", "position_d", "position_i", "position_p",
                   "profile_acceleration", "profile_velocity"]
TESTS = ("breakaway", "sweep", "inertia", "steps")


def signed(value, nbytes):
    """Return a register's unsigned value as a signed number.

    Args:
        value: The value read, unsigned.
        nbytes: The register's size in bytes.
    """
    bits = 8 * nbytes
    return value - (1 << bits) if value >= 1 << (bits - 1) else value


def motor_constants(model):
    """Return a servo model's motor at its joint, from its datasheet.

    Returns:
        {"torque_per_volt" [N m/V], "back_emf" [V s/rad]}, or None for
        a model with no datasheet values here.
    """
    if model not in DATASHEET:
        return None
    stall, rpm = DATASHEET[model]
    return {"torque_per_volt": stall / DATASHEET_VOLTS,
            "back_emf": DATASHEET_VOLTS / (rpm * 2 * math.pi / 60)}


class Abort(Exception):
    """A test can't go on (a joint left its window, a servo too hot)."""


# The real servos

class DxlBus:
    """The servos on a U2D2 (or another serial adapter), protocol 2."""

    def __init__(self, device, baud, ids):
        """Open the adapter and prepare the group read of the state.

        Exits with a message if the Dynamixel SDK is missing or the
        port can't be opened.
        """
        try:
            import dynamixel_sdk as sdk
        except ImportError:
            sys.exit("The Dynamixel SDK is missing: pip install dynamixel-sdk")
        self.sdk = sdk
        self.ids = list(ids)
        self.port = sdk.PortHandler(device)
        if not self.port.openPort():
            sys.exit("Cannot open %s (is the arm's adapter plugged in, and is SPOT not "
                     "using it?)" % device)
        if not self.port.setBaudRate(baud):
            sys.exit("Cannot set %d baud on %s" % (baud, device))
        self.ph = sdk.PacketHandler(2.0)
        self.reader = sdk.GroupSyncRead(self.port, self.ph, STATE_START, STATE_LEN)
        for i in self.ids:
            self.reader.addParam(i)

    def now(self):
        """Return the time [s] on a monotonic clock."""
        return time.monotonic()

    def sleep(self, dt):
        """Wait dt seconds."""
        time.sleep(dt)

    def _check(self, result, error, what):
        """Raise IOError if a transaction failed or the servo erred."""
        if result != self.sdk.COMM_SUCCESS:
            raise IOError("%s: %s" % (what, self.ph.getTxRxResult(result)))
        if error:
            raise IOError("%s: %s" % (what, self.ph.getRxPacketError(error)))

    def ping(self, i):
        """Return servo i's model number."""
        model, result, error = self.ph.ping(self.port, i)
        self._check(result, error, "ping ID %d" % i)
        return model

    def read(self, i, name):
        """Return a register of servo i (goals, present: signed)."""
        addr, n = ADDR[name]
        fn = {1: self.ph.read1ByteTxRx, 2: self.ph.read2ByteTxRx, 4: self.ph.read4ByteTxRx}[n]
        value, result, error = fn(self.port, i, addr)
        self._check(result, error, "read %s of ID %d" % (name, i))
        return signed(value, n) if name.startswith(("goal", "present")) else value

    def write(self, i, name, value):
        """Write a register of servo i (negative: two's complement)."""
        addr, n = ADDR[name]
        value = int(round(value)) & ((1 << (8 * n)) - 1)
        fn = {1: self.ph.write1ByteTxRx, 2: self.ph.write2ByteTxRx,
              4: self.ph.write4ByteTxRx}[n]
        result, error = fn(self.port, i, addr, value)
        self._check(result, error, "write %s of ID %d" % (name, i))

    def state(self):
        """Return {id: (tick [ms], PWM, speed [rad/s], angle [rad])}."""
        result = self.reader.txRxPacket()
        if result != self.sdk.COMM_SUCCESS:
            raise IOError("sync read: %s" % self.ph.getTxRxResult(result))
        out = {}
        for i in self.ids:
            def get(addr, n):
                """Return a signed field of servo i's state."""
                return signed(self.reader.getData(i, addr, n), n)
            out[i] = (self.reader.getData(i, 120, 2), get(124, 2), get(128, 4) * VEL_UNIT,
                      (get(132, 4) - CENTER_PULSE) / PULSES)
        return out

    def close(self):
        """Close the adapter."""
        self.port.closePort()


# A simulated arm

class SimBus:
    """Three simulated joints, to try the script without hardware.

    Each joint has stick-slip friction, a motor with back-EMF driven by
    PWM, and the servo's position law, on a virtual clock (each bus
    transaction takes a millisecond or two). The "true" values are
    printed so the fits can be checked against them.
    """

    def __init__(self, ids, seed=1):
        """Make the joints, each with its own friction and back-EMF."""
        rnd = random.Random(seed)
        self.ids = list(ids)
        self.t = 0.0
        self.h = 2.5e-4                  # [s] simulation step
        self.volts = 12.0                # [V] supply
        motor = motor_constants(311)
        self.g = motor["torque_per_volt"]
        self.j = {}
        for i in self.ids:
            link = {1: 0.095, 2: 0.035, 3: 0.004}.get(i, 0.01)   # [kg m^2]
            s = dict(J=link + 0.0035 * rnd.uniform(0.8, 1.2),
                     Ts=rnd.uniform(0.25, 0.4), vs=rnd.uniform(0.03, 0.08),
                     b=rnd.uniform(0.02, 0.1), ke=motor["back_emf"] * rnd.uniform(0.95, 1.15),
                     q=rnd.uniform(-0.3, 0.3), v=0.0, mode=MODE_POSITION, torque=0,
                     goal_pwm=0, goal_pos=None, pd=None, ei=0.0, ep=0.0, out=0.0,
                     reg=dict(operating_mode=MODE_POSITION, drive_mode=0, pwm_limit=885,
                              velocity_limit=285, velocity_i=1920, velocity_p=100, position_d=0,
                              position_i=0, position_p=850, profile_acceleration=0,
                              profile_velocity=0, max_position=3072, min_position=1024))
            s["Tc"] = s["Ts"] * rnd.uniform(0.6, 0.8)
            self.j[i] = s
        self.truth = {i: dict({k: round(self.j[i][k], 5) for k in ("Ts", "Tc", "vs", "b", "J")},
                              back_emf=round(self.j[i]["ke"], 4),
                              speed_coefficient=round(self.j[i]["ke"] + self.j[i]["b"] / self.g,
                                                      4))
                      for i in self.ids}

    def now(self):
        """Return the virtual time [s]."""
        return self.t

    def sleep(self, dt):
        """Let dt seconds pass."""
        self._advance(dt)

    def _advance(self, dt):
        """Simulate every joint for dt seconds."""
        n = max(1, int(round(dt / self.h)))
        for _ in range(n):
            for i in self.ids:
                self._step(self.j[i], dt / n)
        self.t += dt

    def _step(self, s, h):
        """Simulate one joint for h seconds."""
        r = s["reg"]
        u = 0.0
        if s["torque"]:
            if s["mode"] == MODE_PWM:
                u = s["goal_pwm"]
            else:
                # The servo's position law (profile, PID), giving PWM.
                if s["pd"] is None:
                    s["pd"] = s["q"] * PULSES
                goal = s["goal_pos"] if s["goal_pos"] is not None else s["q"] * PULSES
                vp = r["profile_velocity"] * VEL_UNIT * PULSES
                d = goal - s["pd"]
                if vp <= 0 or abs(d) <= vp * h:
                    s["pd"] = goal
                else:
                    s["pd"] += math.copysign(vp * h, d)
                e = s["pd"] - round(s["q"] * PULSES)
                de = (e - s["ep"]) * 1e-3 / h
                s["ep"] = e
                s["ei"] += e * h / 1e-3
                u = (r["position_p"] / 128 * e + r["position_i"] / 65536 * s["ei"]
                     + r["position_d"] / 16 * de)
            u = min(max(u, -r["pwm_limit"]), r["pwm_limit"])
        s["out"] = u

        # The motor's torque at rest, its back-EMF and the viscous
        # friction implicitly, then stick or slip.
        tau0 = self.g * u / PWM_FULL * self.volts
        damp = self.g * s["ke"] + s["b"]
        mass = s["J"] + h * damp
        vfree = (s["J"] * s["v"] + h * tau0) / mass
        mu = s["Tc"] + (s["Ts"] - s["Tc"]) * math.exp(-(s["v"] / s["vs"]) ** 2)
        p = min(max(-vfree * mass, -h * mu), h * mu)
        s["v"] = vfree + p / mass
        s["q"] += h * s["v"]

    def ping(self, i):
        """Return the model number of an MX-64(2.0)."""
        self._advance(1e-3)
        return 311

    def read(self, i, name):
        """Return one register of joint i."""
        self._advance(1e-3)
        s = self.j[i]
        if name == "model_number":
            return 311
        if name == "torque_enable":
            return s["torque"]
        if name == "present_input_voltage":
            return int(round(self.volts / VOLT_UNIT))
        if name == "present_temperature":
            return 35
        return s["reg"].get(name, 0)

    def write(self, i, name, value):
        """Write one register of joint i."""
        self._advance(1e-3)
        s = self.j[i]
        if name == "torque_enable":
            s["torque"] = int(value)
            s.update(pd=None, ei=0.0, ep=0.0, goal_pos=None, goal_pwm=0)
        elif name == "operating_mode":
            s["mode"] = int(value)
            s["reg"]["operating_mode"] = int(value)
        elif name == "goal_pwm":
            s["goal_pwm"] = signed(int(value) & 0xFFFF, 2)
        elif name == "goal_position":
            s["goal_pos"] = int(value) - CENTER_PULSE
        else:
            s["reg"][name] = int(value)

    def state(self):
        """Return every joint's state, as DxlBus.state does."""
        self._advance(2e-3)
        out = {}
        for i in self.ids:
            s = self.j[i]
            out[i] = (int(self.t * 1000) & 0xFFFF, int(round(s["out"])),
                      round(s["v"] / VEL_UNIT) * VEL_UNIT, round(s["q"] * PULSES) / PULSES)
        return out

    def close(self):
        """Nothing to close."""


# The arm and its joints

class Arm:
    """The servos' settings, modes and moves; the joints' windows."""

    def __init__(self, bus, args):
        """Keep the bus and the options; nothing is changed yet."""
        self.bus, self.args = bus, args
        self.ids = bus.ids
        self.window = math.radians(args.window)
        self.saved = {}
        self.center = {}
        self.t0 = bus.now()

    def setup(self):
        """Ping each servo and save its settings; return {id: model}."""
        info = {}
        for i in self.ids:
            info[i] = self.bus.ping(i)
            self.saved[i] = {n: self.bus.read(i, n) for n in SAVED_REGISTERS}
        return info

    def restore(self):
        """Switch every servo's torque off and put its settings back."""
        for i in self.ids:
            try:
                self.bus.write(i, "torque_enable", 0)
                for n, v in self.saved.get(i, {}).items():
                    self.bus.write(i, n, v)
            except Exception as e:
                # Keep going: the other servos still get their torque
                # off.
                print("  could not restore ID %d: %s" % (i, e))

    def mode(self, i, mode, **registers):
        """Put servo i in an operating mode with these registers."""
        self.bus.write(i, "torque_enable", 0)
        self.bus.write(i, "operating_mode", mode)
        for n, v in registers.items():
            self.bus.write(i, n, v)
        self.bus.write(i, "torque_enable", 1)

    def pwm_mode(self, i):
        """Put servo i in PWM control at Goal PWM 0 (PWM Limit 100%)."""
        self.mode(i, MODE_PWM, pwm_limit=PWM_FULL)
        self.bus.write(i, "goal_pwm", 0)

    def hold(self, i):
        """Hold servo i where it is (position mode, SPOT's settings)."""
        q = self.bus.state()[i][3]
        self.mode(i, MODE_POSITION, **SPOT_POSITION)
        self.bus.write(i, "goal_position", round(q * PULSES + CENTER_PULSE))
        return q

    def goto(self, i, q, timeout=8.0):
        """Move joint i to q [rad] in position mode; let it settle."""
        self.mode(i, MODE_POSITION, **SPOT_POSITION)
        self.bus.write(i, "goal_position", round(q * PULSES + CENTER_PULSE))
        t_end = self.bus.now() + timeout
        while self.bus.now() < t_end:
            s = self.bus.state()[i]
            if abs(s[3] - q) < math.radians(1.5) and abs(s[2]) < 0.03:
                break
        self.bus.sleep(0.3)

    def guard(self, i, s):
        """Stop the run if joint i has left its window.

        Raises:
            Abort: if it is more than 5 deg past the window.
        """
        if abs(s[3] - self.center[i]) > self.window + math.radians(5):
            raise Abort("joint %d left its window (%.1f deg from where it started)"
                        % (i, math.degrees(s[3] - self.center[i])))

    def t(self):
        """Return the time since the run started [s]."""
        return self.bus.now() - self.t0


# Files and small sums

def write_csv(path, header, rows):
    """Write rows under a header as a CSV file."""
    with open(path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(header)
        w.writerows(rows)


def read_csv(path):
    """Return a CSV file's rows as dicts of floats."""
    with open(path) as f:
        r = csv.reader(f)
        header = next(r)
        return [dict(zip(header, map(float, row))) for row in r]


def read_json(path):
    """Return a JSON file's contents."""
    with open(path) as f:
        return json.load(f)


def write_json(path, data):
    """Write data as an indented JSON file."""
    with open(path, "w") as f:
        json.dump(data, f, indent=2)


def lstsq(rows, rhs):
    """Return the least-squares solution for a few unknowns.

    Normal equations, solved by Gaussian elimination with partial
    pivoting.

    Raises:
        ValueError: if the problem is singular.
    """
    n = len(rows[0])
    a = [[sum(r[i] * r[k] for r in rows) for k in range(n)] for i in range(n)]
    y = [sum(r[i] * v for r, v in zip(rows, rhs)) for i in range(n)]
    for c in range(n):
        p = max(range(c, n), key=lambda k: abs(a[k][c]))
        a[c], a[p], y[c], y[p] = a[p], a[c], y[p], y[c]
        if abs(a[c][c]) < 1e-15:
            raise ValueError("singular fit")
        for k in range(c + 1, n):
            f = a[k][c] / a[c][c]
            a[k] = [x - f * z for x, z in zip(a[k], a[c])]
            y[k] -= f * y[c]
    x = [0.0] * n
    for c in reversed(range(n)):
        x[c] = (y[c] - sum(a[c][k] * x[k] for k in range(c + 1, n))) / a[c][c]
    return x


def median(xs):
    """Return the median of a non-empty list."""
    xs = sorted(xs)
    n = len(xs)
    return xs[n // 2] if n % 2 else 0.5 * (xs[n // 2 - 1] + xs[n // 2])


def volts(pwm, supply):
    """Return the voltage across the motor [V] for a PWM."""
    return pwm / PWM_FULL * supply


# The tests

def test_breakaway(arm, i, out):
    """Ramp the PWM from zero until the joint moves, each way in turn.

    Returns:
        The breakaway PWM per direction, {1: [...], -1: [...]}.
    """
    a, bus = arm.args, arm.bus
    rows = []
    found = {1: [], -1: []}
    arm.pwm_mode(i)
    for _ in range(a.repeats):
        for sign in (1, -1):
            bus.write(i, "goal_pwm", 0)
            bus.sleep(0.5)
            start, t_start = bus.state()[i][3], bus.now()
            while True:
                # The PWM rises at RAMP per second, to the cap.
                pwm = sign * min(int(a.ramp * (bus.now() - t_start)), a.max_pwm)
                bus.write(i, "goal_pwm", pwm)
                s = bus.state()[i]
                arm.guard(i, s)
                rows.append([arm.t(), sign, pwm, s[1], s[2], s[3]])
                if abs(s[3] - start) > math.radians(a.move_deg) or abs(s[2]) >= 2 * VEL_UNIT:
                    found[sign].append(abs(pwm))
                    print("    %s: moved at PWM %d" % ("+" if sign > 0 else "-", abs(pwm)))
                    break
                if abs(pwm) >= a.max_pwm:
                    print("    %s: no motion up to the PWM cap (%d)"
                          % ("+" if sign > 0 else "-", a.max_pwm))
                    break
            bus.write(i, "goal_pwm", 0)
            bus.sleep(0.3)

            # Drifted far: back to the middle.
            if abs(bus.state()[i][3] - arm.center[i]) > arm.window / 2:
                arm.goto(i, arm.center[i])
                arm.pwm_mode(i)
    bus.write(i, "goal_pwm", 0)
    write_csv(os.path.join(out, "breakaway_j%d.csv" % i),
              ["t", "direction", "goal_pwm", "pwm", "velocity", "position"], rows)
    return found


def breakaway_pwm(found, sign, a):
    """Return a direction's breakaway PWM (a guess if not measured)."""
    values = found.get(sign) or found.get(-sign) or [a.guess_pwm]
    return median(values)


def test_sweep(arm, i, out, found):
    """Fixed PWM levels about the breakaway, back and forth.

    Each level drives the joint across the window one way, then the
    other, after a kick of KICK_PWM above the breakaway for KICK_TIME
    (so a level below the breakaway finds the joint moving); the speed
    it settles at is recorded.

    Returns:
        The steady points: [{"direction", "pwm", "velocity",
        "samples"}].
    """
    a, bus = arm.args, arm.bus
    rows, points = [], []
    lo, hi = arm.center[i] - arm.window, arm.center[i] + arm.window
    arm.goto(i, lo)
    arm.pwm_mode(i)
    for step in a.pwm_steps:
        for sign in (1, -1):
            start_pwm = int(round(breakaway_pwm(found, sign, a)))
            pwm = sign * min(max(start_pwm + step, 1), a.max_pwm)
            kick = sign * min(start_pwm + KICK_PWM, a.max_pwm)
            bus.write(i, "goal_pwm", kick)
            t_start, steady_since, samples = bus.now(), None, []
            kicking = True
            recent = collections.deque()   # (t, position) over the window
            speeds = collections.deque()   # (t, window speed)
            while True:
                s = bus.state()[i]
                arm.guard(i, s)
                now = bus.now()
                rows.append([arm.t(), kick if kicking else pwm, s[1], s[2], s[3]])
                if ((sign > 0 and s[3] >= hi) or (sign < 0 and s[3] <= lo)
                        or now - t_start > a.pass_time):
                    break
                if kicking and now - t_start >= KICK_TIME:
                    bus.write(i, "goal_pwm", pwm)
                    kicking = False
                    recent.clear()
                    speeds.clear()
                if kicking:
                    continue

                # The mean speed over the last STEADY_WINDOW.
                recent.append((now, s[3]))
                while now - recent[0][0] > STEADY_WINDOW:
                    recent.popleft()
                if now - recent[0][0] < 0.8 * STEADY_WINDOW:
                    continue
                speed = (s[3] - recent[0][1]) / (now - recent[0][0])
                speeds.append((now, speed))
                while now - speeds[0][0] > STEADY_LAG:
                    speeds.popleft()

                # Steady: moving the PWM's way, and the speed no longer
                # changing.
                earlier = speeds[0][1]
                moving = speed * sign > 0.5 * VEL_UNIT
                settled = (now - speeds[0][0] >= 0.8 * STEADY_LAG
                           and abs(speed - earlier) <= STEADY_TOL * abs(speed))
                if moving and settled:
                    steady_since = steady_since or now
                    if now - steady_since > STEADY_HOLD:
                        samples.append(speed)
                else:
                    steady_since = None
            if len(samples) >= MIN_SAMPLES:
                points.append(dict(direction=sign, pwm=pwm, velocity=median(samples),
                                   samples=len(samples)))
                print("    PWM %+d: %+.3f rad/s over %d samples"
                      % (pwm, points[-1]["velocity"], len(samples)))
            else:
                # Just above the breakaway a joint stick-slips instead
                # of sliding (expected at the first steps, and the fit
                # allows for it); at higher PWM the pass may be too
                # short for the window.
                print("    PWM %+d: not steady, left out (the joint stick-slips this slowly; "
                      "at higher PWM raise --window or --pass-time)" % pwm)
            bus.write(i, "goal_pwm", 0)
            bus.sleep(0.3)
            arm.goto(i, hi if sign > 0 else lo)
            arm.pwm_mode(i)
    bus.write(i, "goal_pwm", 0)
    write_csv(os.path.join(out, "sweep_j%d.csv" % i),
              ["t", "goal_pwm", "pwm", "velocity", "position"], rows)
    return points


def test_inertia(arm, i, out, found):
    """A PWM step from rest, each way: how fast the speed settles.

    Returns:
        The pose of all joints at the first step [rad].
    """
    a, bus = arm.args, arm.bus
    pose = None
    for sign in (1, -1):
        arm.goto(i, arm.center[i] - sign * arm.window * 0.8)
        if pose is None:
            pose = [bus.state()[k][3] for k in arm.ids]
        arm.pwm_mode(i)
        start_pwm = breakaway_pwm(found, sign, a)
        pwm = sign * min(int(round(max(a.inertia_pwm * start_pwm, start_pwm + 10))), a.max_pwm)
        rows = []
        start = bus.state()[i][3]
        t0 = bus.now()
        bus.write(i, "goal_pwm", pwm)
        while bus.now() - t0 < a.inertia_time:
            s = bus.state()[i]
            arm.guard(i, s)
            rows.append([bus.now() - t0, pwm, s[1], s[2], s[3] - start])
            if abs(s[3] - start) > math.radians(a.inertia_deg):
                break
        bus.write(i, "goal_pwm", 0)
        write_csv(os.path.join(out, "inertia_j%d_%s.csv" % (i, "pos" if sign > 0 else "neg")),
                  ["t", "goal_pwm", "pwm", "velocity", "moved"], rows)
        bus.sleep(0.5)
    return pose


def test_steps(arm, i, out):
    """Position-mode goal steps with SPOT's servo settings, logged."""
    a, bus = arm.args, arm.bus
    c = arm.center[i]
    arm.goto(i, c)
    arm.mode(i, MODE_POSITION, **SPOT_POSITION)
    rows = []
    t0 = bus.now()
    for g in [c + math.radians(d) for d in a.step_deg]:
        bus.write(i, "goal_position", round(g * PULSES + CENTER_PULSE))
        t_end = bus.now() + a.step_time
        while bus.now() < t_end:
            st = bus.state()
            arm.guard(i, st[i])
            rows.append([bus.now() - t0, i, g] + [st[k][3] for k in arm.ids]
                        + [st[i][2], st[i][1]])
    write_csv(os.path.join(out, "steps_j%d.csv" % i),
              ["t", "joint", "goal"] + ["q%d" % k for k in arm.ids] + ["velocity", "pwm"], rows)
    print("    %d samples over %.1f s" % (len(rows), rows[-1][0] if rows else 0))


# The fits

def fit_breakaway(out, i, supply, motor, r):
    """Fit the breakaway friction from the PWM ramps into r.

    Returns:
        The breakaway voltage [V], or None if not measured.
    """
    path = os.path.join(out, "breakaway_j%d.csv" % i)
    if not os.path.exists(path):
        return None
    rows = read_csv(path)
    found = {1: [], -1: []}
    for d in (1, -1):
        run = [x for x in rows if x["direction"] == d]

        # Each ramp ends where the joint moved (or at the cap, which
        # doesn't count); its breakaway is the PWM where it first
        # moved BREAK_PULSES.
        first = 0
        for k, x in enumerate(run):
            if k == len(run) - 1 or run[k + 1]["t"] - x["t"] > 0.2:
                ramp = run[first:k + 1]
                start = ramp[0]["position"]
                if x["position"] != start or x["velocity"] != 0:
                    moved = [y for y in ramp
                             if abs(y["position"] - start) >= BREAK_PULSES / PULSES]
                    found[d].append(abs((moved[0] if moved else x)["goal_pwm"]))
                first = k + 1
    both = found[1] + found[-1]
    if not both:
        return None
    g = motor["torque_per_volt"]

    def torque(pwm):
        """Return the torque at rest [N m] for a PWM."""
        return g * volts(pwm, supply)
    r["breakaway_pwm_pos"] = median(found[1]) if found[1] else None
    r["breakaway_pwm_neg"] = median(found[-1]) if found[-1] else None
    r["static_pos"] = torque(r["breakaway_pwm_pos"]) if found[1] else None
    r["static_neg"] = torque(r["breakaway_pwm_neg"]) if found[-1] else None
    r["static"] = torque(median(both))
    r["static_spread"] = torque(max(both) - min(both)) if len(both) > 1 else 0.0
    return volts(median(both), supply)


def fit_sweep(out, i, supply, motor, static_volts, r):
    """Fit friction and the speed coefficient from the sweep into r.

    Along the motion, the steady voltage at speed w is
    Vc + (Vs - Vc) exp(-(w/ws)^2) + k w, with Vs the breakaway voltage
    when measured. The Stribeck speed ws is searched on a grid, the
    rest by least squares.

    Returns:
        The speed coefficient k [V s/rad], or None if not fitted.
    """
    path = os.path.join(out, "sweep_points_j%d.json" % i)
    if not os.path.exists(path):
        return None

    # The voltage and speed along the motion; a point turning against
    # its PWM can't be friction (something else drove the joint).
    pts = read_json(path)
    along = [(p["velocity"] * p["direction"], volts(abs(p["pwm"]), supply)) for p in pts]
    dropped = [round(p["velocity"], 3) for (w, _), p in zip(along, pts) if w <= 0]
    if dropped:
        r["sweep_dropped"] = dropped
        r.setdefault("warnings", []).append(
            "friction against speed: %d of %d PWM levels turned the joint against them; left "
            "out" % (len(dropped), len(pts)))
    data = [(w, v) for w, v in along if w > 0]
    if len(data) < 3:
        return None

    # Stribeck speed on a log grid; the rest linear.
    best = None
    for k in range(60):
        ws = 0.003 * 10 ** (k / 60 * 2.5)
        decay = [math.exp(-(w / ws) ** 2) for w, _ in data]
        try:
            if static_volts is not None:
                rows = [[1 - e, w] for (w, _), e in zip(data, decay)]
                rhs = [v - static_volts * e for (_, v), e in zip(data, decay)]
                vc, coef = lstsq(rows, rhs)
                vs = static_volts
            else:
                rows = [[1, e, w] for (w, _), e in zip(data, decay)]
                vc, dv, coef = lstsq(rows, [v for _, v in data])
                vs = vc + dv
        except ValueError:
            continue
        pred = [vc + (vs - vc) * e + coef * w for (w, _), e in zip(data, decay)]
        rms = math.sqrt(sum((p - v) ** 2 for p, (_, v) in zip(pred, data)) / len(data))
        if vc > 0 and coef > 0 and vs >= vc and (best is None or rms < best[0]):
            best = (rms, vc, vs, ws, coef)
    if not best:
        r.setdefault("warnings", []).append(
            "friction against speed: no fit with positive friction and speed coefficient")
        return None
    rms, vc, vs, ws, coef = best
    g = motor["torque_per_volt"]
    r.update(coulomb=g * vc, stribeck=ws, speed_coefficient=coef, sweep_rms=g * rms)
    if static_volts is None:
        r["static"] = g * vs

    # Below the slowest steady speed the data can't tell where the drop
    # from static to Coulomb friction happens (the joint stick-slips
    # there): take half that speed.
    slowest = min(w for w, _ in data)
    if ws < 0.6 * slowest:
        r["stribeck"] = 0.5 * slowest
        r["stribeck_note"] = ("below the slowest steady speed measured (%.3f rad/s), where the "
                              "joint stick-slips; set to half of it" % slowest)
    if rms > max(vc, 0.02):
        r.setdefault("warnings", []).append(
            "friction against speed: the points scatter by %.3f N m about the fit, more than the "
            "sliding friction it finds (%.3f N m): something else loads the joint (a cable?); "
            "don't rely on the Coulomb and Stribeck terms" % (g * rms, g * vc))

    # Back-EMF and viscous friction act alike (in proportion to speed):
    # the datasheet's back-EMF, the rest viscous; a speed coefficient
    # below the datasheet's is all back-EMF.
    datasheet = motor["back_emf"]
    if coef >= datasheet:
        r.update(back_emf=datasheet, viscous=g * (coef - datasheet))
    else:
        r.update(back_emf=coef, viscous=0.0)
        r["back_emf_note"] = ("the speed coefficient (%.3f V s/rad) is below the datasheet's "
                              "back-EMF (%.3f): taken as all back-EMF, no viscous friction"
                              % (coef, datasheet))
    return coef


def step_response(rows):
    """Fit a PWM step's motion: the speed rising to w_end over T.

    The motion from rest is w_end (t - T (1 - exp(-t/T))); T is
    searched on a log grid (1 ms to 3 s), w_end by least squares.

    Returns:
        (T [s], w_end [rad/s], rms [rad]), or None with too little
        motion.
    """
    data = [(x["t"], abs(x["moved"])) for x in rows if x["t"] > 0]
    if len(data) < 6 or max(m for _, m in data) < 4 / PULSES:
        return None
    best = None
    for k in range(90):
        tc = 1e-3 * 10 ** (k / 90 * 3.5)
        x = [t - tc * (1 - math.exp(-t / tc)) for t, _ in data]
        sxx = sum(v * v for v in x)
        if sxx <= 0:
            continue
        w_end = sum(v * m for v, (_, m) in zip(x, data)) / sxx
        rms = math.sqrt(sum((w_end * v - m) ** 2 for v, (_, m) in zip(x, data)) / len(data))
        if best is None or rms < best[2]:
            best = (tc, w_end, rms)
    return best


def simulate_step(rows, inertia, delay, push, damping, friction):
    """Return a PWM step's motion [rad] at each row's time, simulated.

    The joint starts at rest; from DELAY [s] on, PUSH [N m] turns it
    against DAMPING [N m s/rad] times its speed and the dry friction
    FRICTION(w) [N m], with INERTIA [kg m^2]. Sub-steps of at most
    SIM_STEP.
    """
    out = []
    t = q = w = 0.0
    for row in rows:
        while t < row["t"] - 1e-12:
            h = min(SIM_STEP, row["t"] - t)
            if t >= delay:
                # Implicit damping; dry friction stops the joint if it
                # can (the push is above the breakaway, so it won't).
                free = (inertia * w + h * (push - friction(w))) / (inertia + h * damping)
                w = max(free, 0.0)
            q += h * w
            t += h
        out.append(q)
    return out


def fit_step(rows, push, damping, friction):
    """Fit a PWM step's inertia by simulating it (friction known).

    Searches the inertia on a log grid (1e-4 to 1 kg m^2) and the dead
    time before the step acts (0 to MAX_DELAY), then refines the
    inertia about the best.

    Returns:
        (inertia [kg m^2], dead time [s], rms [rad]), or None with too
        little motion.
    """
    data = [x for x in rows if x["t"] > 0]
    if len(data) < 6 or max(abs(x["moved"]) for x in data) < 4 / PULSES:
        return None
    def error(inertia, delay):
        """Return the rms difference [rad] of a simulated step."""
        sim = simulate_step(data, inertia, delay, push, damping, friction)
        return math.sqrt(sum((q - abs(x["moved"])) ** 2 for q, x in zip(sim, data)) / len(data))

    # The grid, then smaller and smaller steps about its best.
    delays = [MAX_DELAY * n / 4 for n in range(5)]
    best = min((error(j, d), j, d) for j in (1e-4 * 10 ** (k / 20) for k in range(81))
               for d in delays)
    for scale in (1.05, 1.015, 1.004):
        improved = True
        while improved:
            improved = False
            for j in (best[1] * scale, best[1] / scale):
                trial = min((error(j, d), j, d) for d in delays)
                if trial[0] < best[0]:
                    best, improved = trial, True
    return best[1], best[2], best[0]


def fit_inertia(out, i, supply, motor, coef, r):
    """Fit the joint's inertia from the PWM steps into r.

    With the friction fitted (breakaway, sweep), each step is
    simulated with the inertia that matches it best (fit_step);
    otherwise the speed's time constant T gives J = T torque_per_volt
    k, k the speed coefficient (the sweep's, else the datasheet's
    back-EMF).
    """
    files = sorted(f for f in os.listdir(out) if f.startswith("inertia_j%d_" % i))
    if not files:
        return
    g = motor["torque_per_volt"]
    if coef is None:
        coef = motor["back_emf"]
        r.setdefault("warnings", []).append(
            "inertia: without the sweep, the speed coefficient is the datasheet's back-EMF "
            "(viscous friction left out)")
    damping = g * coef                       # [N m s/rad]
    known = all(isinstance(r.get(k), float) for k in ("static", "coulomb", "stribeck"))

    def friction(w):
        """Return the dry friction at speed w [N m]."""
        return r["coulomb"] + (r["static"] - r["coulomb"]) * math.exp(-(w / r["stribeck"]) ** 2)
    found = []
    for f in files:
        rows = read_csv(os.path.join(out, f))
        key = "neg" if "_neg" in f else "pos"
        turn = "-" if key == "neg" else "+"
        fit = step_response(rows)
        if fit is None:
            continue
        tc, w_end, _ = fit
        r["time_constant_" + key] = tc
        inertia = damping * tc
        push = g * volts(abs(rows[0]["goal_pwm"]), supply)
        if known and push > r["static"]:
            simulated = fit_step(rows, push, damping, friction)
            if simulated:
                inertia = simulated[0]
                r["dead_time_" + key] = simulated[1]
        r["inertia_" + key] = inertia
        found.append(inertia)

        # The speed it settled at should be what the friction and the
        # back-EMF leave of the step's voltage.
        expected = (g * volts(abs(rows[0]["goal_pwm"]), supply) - r.get("coulomb", 0)) / damping
        if expected > 0 and not 0.6 < w_end / expected < 1.6:
            r.setdefault("warnings", []).append(
                "inertia: the step turning %s settled at %.3f rad/s, the friction fit expects "
                "%.3f: check the joint turns freely" % (turn, w_end, expected))

        # Not much slower than the bus samples it: only roughly known.
        dt = rows[-1]["t"] / max(len(rows) - 1, 1)
        if tc < 5 * dt:
            r.setdefault("warnings", []).append(
                "inertia: the step turning %s settled in %.1f ms, close to the %.1f ms between "
                "reads, so its inertia is only roughly known" % (turn, 1e3 * tc, 1e3 * dt))
    if not found:
        return
    r["inertia"] = median(found)
    if len(found) == 2 and max(found) > 3 * min(found):
        r.setdefault("warnings", []).append(
            "inertia: %.4g kg m^2 turning +, %.4g turning -; a joint's inertia is the same both "
            "ways, so something loads it one way (a cable dragging or catching?): check it by "
            "hand, torque off" % (r["inertia_pos"], r["inertia_neg"]))
    meta = os.path.join(out, "inertia_pose_j%d.json" % i)
    if os.path.exists(meta):
        r["pose"] = read_json(meta)


def fit_joint(out, i, supply, motor):
    """Return joint i's calibration from the run's files."""
    r = {"index": i, "id": i, "name": NAMES.get(i, "joint %d" % i),
         "torque_per_volt": motor["torque_per_volt"],
         "back_emf_datasheet": motor["back_emf"]}
    static_volts = fit_breakaway(out, i, supply, motor, r)
    coef = fit_sweep(out, i, supply, motor, static_volts, r)
    fit_inertia(out, i, supply, motor, coef, r)
    return r


def fit_all(out, joints, supply, motors, extra):
    """Fit every joint, write arm_calibration.json and return it."""
    result = {"format": "spot-arm-calibration", "version": 2, "control": "pwm",
              "created": datetime.datetime.now().isoformat(timespec="seconds"),
              "folder": os.path.abspath(out), "supply_voltage": supply, "joints": []}
    result.update(extra)

    def fmt(x, n="%.3f"):
        """Return a number formatted, or "-"."""
        return n % x if isinstance(x, (int, float)) else "-"
    for i in joints:
        r = fit_joint(out, i, supply, motors[str(i)])
        result["joints"].append(r)
        print("  joint %d (%s): static %s N m (+%s/-%s), coulomb %s N m, stribeck %s rad/s, "
              "viscous %s N m s/rad, back-EMF %s V s/rad, inertia %s kg m^2"
              % (i, r["name"], fmt(r.get("static")), fmt(r.get("static_pos")),
                 fmt(r.get("static_neg")), fmt(r.get("coulomb")), fmt(r.get("stribeck")),
                 fmt(r.get("viscous")), fmt(r.get("back_emf")), fmt(r.get("inertia"), "%.4g")))
    path = os.path.join(out, "arm_calibration.json")
    write_json(path, result)
    print("Wrote %s\nCopy it to the SPOT project's folder (next to spot_settings.json); the next "
          "simulation uses it." % path)
    return result


# The run

def parse_args():
    """Return the command line's options."""
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0],
                                formatter_class=argparse.RawDescriptionHelpFormatter,
                                epilog=__doc__.split("\n", 1)[1])
    p.add_argument("--port", default="/dev/ttyUSB0")
    p.add_argument("--baud", type=int, default=1000000)
    p.add_argument("--joints", type=int, nargs="+", default=[1, 2, 3],
                   help="servo IDs to calibrate (1 shoulder, 2 elbow, 3 wrist)")
    p.add_argument("--all-ids", type=int, nargs="+", default=[1, 2, 3],
                   help="every servo on the bus (the others hold)")
    p.add_argument("--tests", nargs="+", default=list(TESTS), choices=TESTS)
    p.add_argument("--out", default="runs", help="folder for the run's data (a timestamped "
                                                 "folder inside)")
    p.add_argument("--fit", metavar="RUN_FOLDER", help="only fit the data in RUN_FOLDER again")
    p.add_argument("--torque-per-volt", type=float,
                   help="motor torque per volt at the joint [N m/V] (default: the datasheet's)")
    p.add_argument("--back-emf", type=float,
                   help="datasheet back-EMF at the joint [V s/rad], which splits the speed "
                        "coefficient into back-EMF and viscous friction (default: the "
                        "datasheet's)")
    p.add_argument("--window", type=float, default=25.0,
                   help="each joint moves within +-WINDOW deg of its start")
    p.add_argument("--ramp", type=float, default=8.0, help="breakaway PWM ramp [PWM/s]")
    p.add_argument("--repeats", type=int, default=3, help="breakaway ramps per direction")
    p.add_argument("--move-deg", type=float, default=0.5,
                   help="motion that counts as breakaway [deg]")
    p.add_argument("--max-pwm", type=int, default=300,
                   help="PWM cap in the tests [885 = 100 %%]")
    p.add_argument("--guess-pwm", type=float, default=45.0,
                   help="breakaway PWM assumed when the breakaway test didn't run")
    p.add_argument("--pwm-steps", type=int, nargs="+",
                   default=[-15, -10, -6, -3, 0, 3, 6, 10, 16, 25, 40, 60, 90],
                   help="sweep levels: PWM above (or below) each direction's breakaway")
    p.add_argument("--pass-time", type=float, default=25.0, help="longest sweep pass [s]")
    p.add_argument("--inertia-pwm", type=float, default=2.5,
                   help="PWM step, as a multiple of the breakaway PWM")
    p.add_argument("--inertia-deg", type=float, default=15.0,
                   help="stop the PWM step after this motion [deg]")
    p.add_argument("--inertia-time", type=float, default=1.0,
                   help="longest PWM step [s]")
    p.add_argument("--step-deg", type=float, nargs="+", default=[10, 0, -10, 20, 0, -20, 5, 0],
                   help="position steps [deg]")
    p.add_argument("--step-time", type=float, default=2.5, help="time at each position step [s]")
    p.add_argument("--simulate", action="store_true", help="a simulated arm instead of the servos")
    p.add_argument("--yes", action="store_true",
                   help="start without asking (Enter); for the SPOT Console")
    return p.parse_args()


def refit(a):
    """Fit a saved run again (--fit)."""
    info = read_json(os.path.join(a.fit, "run.json"))
    if info.get("control") != "pwm":
        sys.exit("%s was measured with the motor current (an older version of this script): "
                 "its data can't be fitted for PWM control. Run the calibration again." % a.fit)
    args = info.get("args", {})
    motors = info["motors"]
    for m in motors.values():
        if a.torque_per_volt:
            m["torque_per_volt"] = a.torque_per_volt
        if a.back_emf:
            m["back_emf"] = a.back_emf
    fit_all(a.fit, info["joints"], info["supply_voltage"], motors,
            {"servos": info.get("servos"), "simulated": bool(args.get("simulate")),
             "tests": [t for t in TESTS if t in args.get("tests", ())]})


def motors_for(models, a):
    """Return each joint's motor constants, {id: {...}}.

    Exits with a message for a servo model with no datasheet values
    (unless both are given on the command line).
    """
    motors = {}
    for i in a.joints:
        m = motor_constants(models[i]) or {}
        if a.torque_per_volt:
            m["torque_per_volt"] = a.torque_per_volt
        if a.back_emf:
            m["back_emf"] = a.back_emf
        if "torque_per_volt" not in m or "back_emf" not in m:
            sys.exit("No datasheet values for ID %d (%s): give --torque-per-volt and --back-emf."
                     % (i, MODELS.get(models[i], models[i])))
        m["model"] = MODELS.get(models[i], models[i])
        motors[str(i)] = m
    return motors


def run_tests(arm, a, out, order):
    """Run the chosen tests on each joint in turn, saving their data."""
    bus = arm.bus
    progress = {"index": 0, "total": len(a.joints) * len(order)}

    def announce(joint, test):
        """Print the PROGRESS line for a test about to start."""
        progress["index"] += 1
        print("PROGRESS %s" % json.dumps({"joint": joint, "test": test,
                                          "index": progress["index"],
                                          "total": progress["total"]}))
        sys.stdout.flush()
    for i in a.joints:
        print("Joint %d (%s), from %.1f deg:"
              % (i, NAMES.get(i, "joint %d" % i), math.degrees(arm.center[i])))
        found = {1: [], -1: []}
        if "breakaway" in a.tests:
            announce(i, "breakaway")
            print("  breakaway (PWM ramps)")
            found = test_breakaway(arm, i, out)
        if "sweep" in a.tests:
            announce(i, "sweep")
            print("  friction against speed (PWM levels)")
            write_json(os.path.join(out, "sweep_points_j%d.json" % i),
                       test_sweep(arm, i, out, found))
        if "inertia" in a.tests:
            announce(i, "inertia")
            print("  inertia (PWM steps)")
            write_json(os.path.join(out, "inertia_pose_j%d.json" % i),
                       test_inertia(arm, i, out, found))
        if "steps" in a.tests:
            announce(i, "steps")
            print("  position steps (SPOT's servo settings)")
            test_steps(arm, i, out)
        arm.goto(i, arm.center[i])
        arm.hold(i)
        for k in a.all_ids:
            temp = bus.read(k, "present_temperature")
            if temp > MAX_TEMPERATURE:
                raise Abort("ID %d is at %d C: let it cool down" % (k, temp))


def main():
    """Run the calibration (or fit a saved run again)."""
    a = parse_args()
    if a.fit:
        refit(a)
        return

    # SIGTERM stops it (the SPOT Console's agent starts it from a shell
    # that ignores SIGTERM, which it would inherit): until the servos
    # are set up, at once; afterwards through on_signal below, which
    # restores them.
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    out = os.path.join(a.out, datetime.datetime.now().strftime("%Y%m%d_%H%M%S"))
    os.makedirs(out, exist_ok=True)
    bus = SimBus(a.all_ids) if a.simulate else DxlBus(a.port, a.baud, a.all_ids)
    arm = Arm(bus, a)

    def on_signal(*_):
        """Stop the run as Ctrl+C does (the servos are restored)."""
        raise KeyboardInterrupt
    signal.signal(signal.SIGTERM, on_signal)

    ok = False
    try:
        # The servos, their supply and their motors.
        models = arm.setup()
        supply = bus.read(a.all_ids[0], "present_input_voltage") * VOLT_UNIT
        motors = motors_for(models, a)
        servos = {str(i): MODELS.get(m, m) for i, m in models.items()}
        print("Servos: %s; supply %.1f V; data in %s"
              % (", ".join("ID %d %s" % (i, MODELS.get(m, m)) for i, m in models.items()),
                 supply, out))
        if a.simulate:
            print("Simulated arm, true values: %s" % json.dumps(bus.truth))
        else:
            print("The platform must not float, and each joint will move up to %.0f deg from "
                  "where it is. Ctrl+C stops." % a.window)
            if not a.yes:
                input("Press Enter to start... ")
        write_json(os.path.join(out, "run.json"),
                   {"control": "pwm", "joints": a.joints, "motors": motors, "servos": servos,
                    "supply_voltage": supply, "args": vars(a)})

        # Every joint holds where it is; then the tests, and the fit.
        for i in a.all_ids:
            arm.center[i] = arm.hold(i)
        order = [t for t in TESTS if t in a.tests]
        run_tests(arm, a, out, order)
        print("Fitting:")
        fit_all(out, a.joints, supply, motors,
                {"servos": servos, "simulated": bool(a.simulate), "tests": order})
        ok = True
    except KeyboardInterrupt:
        print("\nStopped: torque off.")
    except Abort as e:
        print("\nStopped: %s. Torque off." % e)
    finally:
        arm.restore()
        bus.close()
    if not ok:
        sys.exit(1)


if __name__ == "__main__":
    main()
