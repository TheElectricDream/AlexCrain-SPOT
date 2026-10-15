#!/usr/bin/env python3
"""Calibrate the SPOT arm's joint friction (and inertia) from the real Dynamixel servos.

Writes arm_calibration.json for SPOT's arm model (copy it to the project's folder, next to
spot_settings.json; spot.arm.parameters reads it, see spot.arm.loadCalibration), and the raw
data as CSV files.

Tests, for each joint in turn (the other joints hold their position in position mode):
  breakaway  current mode, the current ramped slowly from zero in each direction until the joint
             moves: the static (breakaway) friction torque, several times per direction
  sweep      velocity mode, constant speeds back and forth across a window: the friction torque
             at each speed (motor current x torque constant), fitted with
             coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous v
  inertia    current mode, a torque step from rest: the joint's acceleration gives its inertia
             (link + rotor); SPOT subtracts the link's part to get the rotor's (armature)
  steps      position mode with SPOT's servo settings (P 400, I 0, D 200, profile velocity 20):
             goal steps, logged for spot.arm.compareSteps (the simulated servo against the real)

Before running:
  - the platform rests on the table (not floating) or is held: the arm's motion must not move it
  - each tested joint will move within +-WINDOW degrees (default 25) of where it is: clear the
    space, and start with the joints away from their limits
  - keep the power switch at hand; Ctrl+C stops at once (all torque off)
  - pip install dynamixel-sdk (the script needs nothing else)

Examples:
  python3 calibrate_arm.py                          # all tests, joints 1-3, /dev/ttyUSB0
  python3 calibrate_arm.py --joints 3 --tests breakaway sweep
  python3 calibrate_arm.py --fit runs/20261011_101500  # fit again from saved data
  python3 calibrate_arm.py --simulate               # try it without hardware (a built-in model)
  python3 calibrate_arm.py --yes                    # no "Press Enter" (the SPOT Console's Calibration tab
                                                    # runs it this way on the Chaser, through its agent)

Progress: a line "PROGRESS {json}" before each test ({"joint", "test", "index", "total"}), for the
Calibration tab. Exit code 0 when arm_calibration.json was written; 1 when stopped or aborted.

Units: the servos' (protocol 2.0, MX series): position 4096 pulses/rev, velocity 0.229 rev/min,
current 3.36 mA, PWM 885 = 100 %. Torque = Kt x current, Kt from the datasheet (--kt to change).
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

# ---------------------------------------------------------------------------- servo constants

ADDR = {
    "model_number": (0, 2), "operating_mode": (11, 1), "drive_mode": (10, 1),
    "pwm_limit": (36, 2), "current_limit": (38, 2), "velocity_limit": (44, 4),
    "max_position": (48, 4), "min_position": (52, 4), "torque_enable": (64, 1),
    "velocity_i": (76, 2), "velocity_p": (78, 2), "position_d": (80, 2), "position_i": (82, 2),
    "position_p": (84, 2), "ff2": (88, 2), "ff1": (90, 2),
    "goal_pwm": (100, 2), "goal_current": (102, 2), "goal_velocity": (104, 4),
    "profile_acceleration": (108, 4), "profile_velocity": (112, 4), "goal_position": (116, 4),
    "present_input_voltage": (144, 2), "present_temperature": (146, 1),
}
STATE_START, STATE_LEN = 120, 16   # realtime tick (2), moving, moving status, PWM (2), current (2), velocity (4), position (4)
MODE_CURRENT, MODE_VELOCITY, MODE_POSITION = 0, 1, 3
PULSES = 4096 / (2 * math.pi)        # per rad
VEL_UNIT = 0.229 * 2 * math.pi / 60  # rad/s per unit
CUR_UNIT = 3.36e-3                   # A per unit
# The sweep's steadiness test: the speed averaged over this long [s], from the position (a pulse is 0.0015 rad),
# not the servo's present velocity, which comes in whole units of 0.024 rad/s and, at the slowest speeds of a
# few units, flickers by one unit either way (the servo's own speed loop, and a little stick-slip)
STEADY_WINDOW = 0.5
MODELS = {311: "MX-64(2.0)", 321: "MX-106(2.0)", 30: "MX-28(2.0)", 1020: "XM430-W350", 1120: "XM540-W270"}
KT = {311: 6.0 / 4.1, 321: 8.4 / 5.2}  # N m/A at the joint, datasheet stall torque / stall current at 12 V
NAMES = {1: "shoulder", 2: "elbow", 3: "wrist"}
SPOT_POSITION = {"position_p": 400, "position_i": 0, "position_d": 200, "profile_velocity": 20, "profile_acceleration": 0,
                 "drive_mode": 0}   # as SPOT's old position-mode block set the servos up (its P/I/D gains)
SAVED_REGISTERS = ["operating_mode", "drive_mode", "current_limit", "velocity_limit", "velocity_i", "velocity_p",
                   "position_d", "position_i", "position_p", "profile_acceleration", "profile_velocity"]


def signed(value, nbytes):
    bits = 8 * nbytes
    return value - (1 << bits) if value >= 1 << (bits - 1) else value


class Abort(Exception):
    pass


# ---------------------------------------------------------------------------- the real servos

class DxlBus:
    """The servos on a U2D2 (or the TX2's serial adapter), protocol 2.0."""

    def __init__(self, device, baud, ids):
        try:
            import dynamixel_sdk as sdk
        except ImportError:
            sys.exit("The Dynamixel SDK is missing: pip install dynamixel-sdk")
        self.sdk = sdk
        self.ids = list(ids)
        self.port = sdk.PortHandler(device)
        if not self.port.openPort():
            sys.exit("Cannot open %s (is the arm's adapter plugged in, and is SPOT not using it?)" % device)
        if not self.port.setBaudRate(baud):
            sys.exit("Cannot set %d baud on %s" % (baud, device))
        self.ph = sdk.PacketHandler(2.0)
        self.reader = sdk.GroupSyncRead(self.port, self.ph, STATE_START, STATE_LEN)
        for i in self.ids:
            self.reader.addParam(i)

    def now(self):
        return time.monotonic()

    def sleep(self, dt):
        time.sleep(dt)

    def _check(self, result, error, what):
        if result != self.sdk.COMM_SUCCESS:
            raise IOError("%s: %s" % (what, self.ph.getTxRxResult(result)))
        if error:
            raise IOError("%s: %s" % (what, self.ph.getRxPacketError(error)))

    def ping(self, i):
        model, result, error = self.ph.ping(self.port, i)
        self._check(result, error, "ping ID %d" % i)
        return model

    def read(self, i, name):
        addr, n = ADDR[name]
        fn = {1: self.ph.read1ByteTxRx, 2: self.ph.read2ByteTxRx, 4: self.ph.read4ByteTxRx}[n]
        value, result, error = fn(self.port, i, addr)
        self._check(result, error, "read %s of ID %d" % (name, i))
        return signed(value, n) if name.startswith("goal") or name.startswith("present") else value

    def write(self, i, name, value):
        addr, n = ADDR[name]
        value = int(round(value)) & ((1 << (8 * n)) - 1)
        fn = {1: self.ph.write1ByteTxRx, 2: self.ph.write2ByteTxRx, 4: self.ph.write4ByteTxRx}[n]
        result, error = fn(self.port, i, addr, value)
        self._check(result, error, "write %s of ID %d" % (name, i))

    def state(self):
        """{id: (tick ms, pwm, current A, velocity rad/s, position rad)} for all servos."""
        result = self.reader.txRxPacket()
        if result != self.sdk.COMM_SUCCESS:
            raise IOError("sync read: %s" % self.ph.getTxRxResult(result))
        out = {}
        for i in self.ids:
            g = lambda a, n: signed(self.reader.getData(i, a, n), n)
            out[i] = (self.reader.getData(i, 120, 2), g(124, 2), g(126, 2) * CUR_UNIT, g(128, 4) * VEL_UNIT,
                      (g(132, 4) - 2048) / PULSES)
        return out

    def close(self):
        self.port.closePort()


# ---------------------------------------------------------------------------- a simulated arm

class SimBus:
    """Three joints with stick-slip friction and Dynamixel-like controllers, on a virtual clock
    (each bus transaction takes 2 ms), to try the script without hardware. The "true" friction
    is printed so the fits can be checked against it."""

    def __init__(self, ids, seed=1):
        rnd = random.Random(seed)
        self.ids = list(ids)
        self.t = 0.0
        self.h = 2.5e-4
        self.kt, self.R, self.ke, self.V = 6.0 / 4.1, 12 / 4.1, 12 / (63 * 2 * math.pi / 60), 12.0
        self.j = {}
        for i in self.ids:
            self.j[i] = dict(
                J={1: 0.095, 2: 0.035, 3: 0.004}.get(i, 0.01) + 0.0035 * rnd.uniform(0.8, 1.2),
                Ts=rnd.uniform(0.25, 0.4), vs=rnd.uniform(0.03, 0.08), b=rnd.uniform(0.05, 0.12),
                q=rnd.uniform(-0.3, 0.3), v=0.0, mode=MODE_POSITION, torque=0, goal_cur=0, goal_vel=0, goal_pos=None,
                pd=None, ei=0.0, ep=0.0, vi=0.0, out=0.0, cur=0.0,
                reg=dict(operating_mode=3, drive_mode=0, current_limit=1941, velocity_limit=285, velocity_i=1920,
                         velocity_p=100, position_d=0, position_i=0, position_p=850, profile_acceleration=0,
                         profile_velocity=0, pwm_limit=885, max_position=3072, min_position=1024))
            self.j[i]["Tc"] = self.j[i]["Ts"] * rnd.uniform(0.6, 0.8)
        self.truth = {i: {k: self.j[i][k] for k in ("Ts", "Tc", "vs", "b", "J")} for i in self.ids}

    def now(self):
        return self.t

    def sleep(self, dt):
        self._advance(dt)

    def _advance(self, dt):
        n = max(1, int(round(dt / self.h)))
        for _ in range(n):
            for i in self.ids:
                self._step(self.j[i], dt / n)
        self.t += dt

    def _step(self, s, h):
        r = s["reg"]
        tau0, damp = 0.0, 0.0
        if s["torque"]:
            if s["mode"] == MODE_CURRENT:
                imax = (self.V - self.ke * s["v"]) / self.R
                imin = (-self.V - self.ke * s["v"]) / self.R
                s["cur"] = min(max(s["goal_cur"] * CUR_UNIT, imin), imax)
                tau0 = self.kt * s["cur"]
            else:
                if s["mode"] == MODE_VELOCITY:
                    e = s["goal_vel"] - round(s["v"] / VEL_UNIT)
                    s["vi"] += e * h / 1e-3
                    u = r["velocity_p"] / 128 * e + r["velocity_i"] / 65536 * s["vi"]
                else:
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
                    u = r["position_p"] / 128 * e + r["position_i"] / 65536 * s["ei"] + r["position_d"] / 16 * de
                u = min(max(u, -r["pwm_limit"]), r["pwm_limit"])
                s["out"] = u
                tau0 = self.kt / self.R * u / 885 * self.V
                damp = self.kt * self.ke / self.R
                s["cur"] = (u / 885 * self.V - self.ke * s["v"]) / self.R
        # implicit viscous + back-EMF, then stick or slip
        J, b = s["J"], s["b"] + damp
        vfree = (J * s["v"] + h * tau0) / (J + h * b)
        mu = s["Tc"] + (s["Ts"] - s["Tc"]) * math.exp(-(s["v"] / s["vs"]) ** 2)
        p = min(max(-vfree * (J + h * b), -h * mu), h * mu)
        s["v"] = vfree + p / (J + h * b)
        s["q"] += h * s["v"]

    def ping(self, i):
        self._advance(1e-3)
        return 311

    def read(self, i, name):
        self._advance(1e-3)
        s = self.j[i]
        if name == "model_number":
            return 311
        if name == "torque_enable":
            return s["torque"]
        if name == "present_input_voltage":
            return 120
        if name == "present_temperature":
            return 35
        return s["reg"].get(name, 0)

    def write(self, i, name, value):
        self._advance(1e-3)
        s = self.j[i]
        if name == "torque_enable":
            s["torque"] = int(value)
            s.update(pd=None, ei=0.0, ep=0.0, vi=0.0, goal_pos=None, goal_cur=0, goal_vel=0)
        elif name == "operating_mode":
            s["mode"] = int(value)
            s["reg"]["operating_mode"] = int(value)
        elif name == "goal_current":
            s["goal_cur"] = signed(int(value) & 0xFFFF, 2)
        elif name == "goal_velocity":
            s["goal_vel"] = signed(int(value) & 0xFFFFFFFF, 4)
        elif name == "goal_position":
            s["goal_pos"] = int(value) - 2048
        else:
            s["reg"][name] = int(value)

    def state(self):
        self._advance(2e-3)
        out = {}
        for i in self.ids:
            s = self.j[i]
            cur = round((s["cur"] + random.gauss(0, 0.004)) / CUR_UNIT) * CUR_UNIT
            out[i] = (int(self.t * 1000) & 0xFFFF, int(s["out"]), cur, round(s["v"] / VEL_UNIT) * VEL_UNIT,
                      round(s["q"] * PULSES) / PULSES)
        return out

    def close(self):
        pass


# ---------------------------------------------------------------------------- helpers

class Arm:
    def __init__(self, bus, args):
        self.bus, self.args = bus, args
        self.ids = bus.ids
        self.window = math.radians(args.window)
        self.saved = {}
        self.center = {}
        self.t0 = bus.now()

    def setup(self):
        info = {}
        for i in self.ids:
            model = self.bus.ping(i)
            info[i] = model
            self.saved[i] = {n: self.bus.read(i, n) for n in SAVED_REGISTERS}
        return info

    def restore(self):
        for i in self.ids:
            try:
                self.bus.write(i, "torque_enable", 0)
                for n, v in self.saved.get(i, {}).items():
                    self.bus.write(i, n, v)
            except Exception as e:   # keep going: the other servos still get their torque off
                print("  could not restore ID %d: %s" % (i, e))

    def mode(self, i, mode, **registers):
        self.bus.write(i, "torque_enable", 0)
        self.bus.write(i, "operating_mode", mode)
        for n, v in registers.items():
            self.bus.write(i, n, v)
        self.bus.write(i, "torque_enable", 1)

    def hold(self, i):
        """Position mode at the present position, with SPOT's settings."""
        q = self.bus.state()[i][4]
        self.mode(i, MODE_POSITION, **SPOT_POSITION)
        self.bus.write(i, "goal_position", round(q * PULSES + 2048))
        return q

    def goto(self, i, q, timeout=8.0):
        self.mode(i, MODE_POSITION, **SPOT_POSITION)
        self.bus.write(i, "goal_position", round(q * PULSES + 2048))
        t_end = self.bus.now() + timeout
        while self.bus.now() < t_end:
            s = self.bus.state()[i]
            if abs(s[4] - q) < math.radians(1.5) and abs(s[3]) < 0.03:
                break
        self.bus.sleep(0.3)

    def guard(self, i, s):
        if abs(s[4] - self.center[i]) > self.window + math.radians(5):
            raise Abort("joint %d left its window (%.1f deg from where it started)" % (i, math.degrees(s[4] - self.center[i])))

    def t(self):
        return self.bus.now() - self.t0


def write_csv(path, header, rows):
    with open(path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(header)
        w.writerows(rows)


def read_csv(path):
    with open(path) as f:
        r = csv.reader(f)
        header = next(r)
        return [dict(zip(header, map(float, row))) for row in r]


def lstsq(rows, rhs):
    """Least squares for a few unknowns (normal equations, Gaussian elimination)."""
    n = len(rows[0])
    A = [[sum(r[a] * r[b] for r in rows) for b in range(n)] for a in range(n)]
    y = [sum(r[a] * v for r, v in zip(rows, rhs)) for a in range(n)]
    for c in range(n):
        p = max(range(c, n), key=lambda k: abs(A[k][c]))
        A[c], A[p], y[c], y[p] = A[p], A[c], y[p], y[c]
        if abs(A[c][c]) < 1e-15:
            raise ValueError("singular fit")
        for k in range(c + 1, n):
            f = A[k][c] / A[c][c]
            A[k] = [a - f * b for a, b in zip(A[k], A[c])]
            y[k] -= f * y[c]
    x = [0.0] * n
    for c in reversed(range(n)):
        x[c] = (y[c] - sum(A[c][k] * x[k] for k in range(c + 1, n))) / A[c][c]
    return x


def median(xs):
    xs = sorted(xs)
    n = len(xs)
    return xs[n // 2] if n % 2 else 0.5 * (xs[n // 2 - 1] + xs[n // 2])


# ---------------------------------------------------------------------------- tests

def test_breakaway(arm, i, out, kt):
    """Ramp the current from zero until the joint moves, alternately each way."""
    a, bus = arm.args, arm.bus
    rows, events = [], []
    arm.mode(i, MODE_CURRENT)
    rate = a.ramp / CUR_UNIT   # units per second
    for rep in range(a.repeats):
        for sign in (1, -1):
            bus.write(i, "goal_current", 0)
            bus.sleep(0.5)
            s0 = bus.state()[i]
            start, t_start = s0[4], bus.now()
            while True:
                units = sign * min(int(rate * (bus.now() - t_start)), a.max_current)
                bus.write(i, "goal_current", units)
                s = bus.state()[i]
                arm.guard(i, s)
                rows.append([arm.t(), sign, units * CUR_UNIT, s[2], s[3], s[4]])
                if abs(s[4] - start) > math.radians(a.move_deg) or abs(s[3]) >= 2 * VEL_UNIT:
                    events.append(dict(direction=sign, current=s[2], command=units * CUR_UNIT, torque=kt * abs(s[2])))
                    print("    %s: moved at %.0f mA (%.3f N m)" % ("+" if sign > 0 else "-", 1e3 * abs(s[2]), kt * abs(s[2])))
                    break
                if abs(units) >= a.max_current:
                    print("    %s: no motion up to the current cap (%.0f mA)" % ("+" if sign > 0 else "-", a.max_current * CUR_UNIT * 1e3))
                    break
            bus.write(i, "goal_current", 0)
            bus.sleep(0.3)
            if abs(bus.state()[i][4] - arm.center[i]) > arm.window / 2:   # drifted: back to the middle
                arm.goto(i, arm.center[i])
                arm.mode(i, MODE_CURRENT)
    bus.write(i, "goal_current", 0)
    write_csv(os.path.join(out, "breakaway_j%d.csv" % i), ["t", "direction", "goal_current", "current", "velocity", "position"], rows)
    return events


def test_sweep(arm, i, out):
    """Constant speeds back and forth across the window (velocity mode)."""
    a, bus = arm.args, arm.bus
    rows, points = [], []
    arm.goto(i, arm.center[i] - arm.window)
    arm.mode(i, MODE_VELOCITY, velocity_limit=max(a.speeds) + 5, velocity_p=100, velocity_i=1920, profile_acceleration=0)
    lo, hi = arm.center[i] - arm.window, arm.center[i] + arm.window
    for units in a.speeds:
        for sign in (1, -1):
            v = sign * units * VEL_UNIT
            bus.write(i, "goal_velocity", sign * units)
            t_start, steady, samples = bus.now(), None, []
            recent = collections.deque()   # (t, position) over the last STEADY_WINDOW s
            while True:
                s = bus.state()[i]
                arm.guard(i, s)
                now = bus.now()
                rows.append([arm.t(), sign * units, v, s[2], s[3], s[4]])
                if (sign > 0 and s[4] >= hi) or (sign < 0 and s[4] <= lo) or now - t_start > a.pass_time:
                    break
                recent.append((now, s[4]))
                while now - recent[0][0] > STEADY_WINDOW:
                    recent.popleft()
                if now - recent[0][0] < 0.8 * STEADY_WINDOW:
                    continue
                speed = (s[4] - recent[0][1]) / (now - recent[0][0])   # the mean speed over the window
                if abs(speed - v) <= max(0.15 * abs(v), 0.25 * VEL_UNIT):   # sliding at the speed (not stuck, not slipping)
                    steady = steady or now
                    if now - steady > 0.3:
                        samples.append((s[2], speed))
                else:
                    steady = None
            if len(samples) >= 10:
                points.append(dict(command=v, current=median([c for c, _ in samples]), velocity=median([w for _, w in samples]),
                                   samples=len(samples)))
                print("    %+.3f rad/s: %.0f mA over %d samples" % (v, 1e3 * points[-1]["current"], len(samples)))
            else:
                # (slower than the Stribeck speed a joint stick-slips instead of sliding: expected at the slowest
                # speeds, and the fit allows for it; at faster ones the pass may be too short for the window)
                print("    %+.3f rad/s: not steady, left out (the joint stick-slips this slowly; at faster speeds raise "
                      "--window or --pass-time)" % v)
            bus.write(i, "goal_velocity", 0)
            bus.sleep(0.3)
            if sign > 0:
                arm.goto(i, hi)
            else:
                arm.goto(i, lo)
            arm.mode(i, MODE_VELOCITY, velocity_limit=max(a.speeds) + 5, velocity_p=100, velocity_i=1920, profile_acceleration=0)
    bus.write(i, "goal_velocity", 0)
    write_csv(os.path.join(out, "sweep_j%d.csv" % i), ["t", "goal_units", "goal_velocity", "current", "velocity", "position"], rows)
    return points


def test_inertia(arm, i, out, kt, static):
    """A torque step from rest, each way: the acceleration gives the inertia."""
    a, bus = arm.args, arm.bus
    results = []
    for sign in (1, -1):
        arm.goto(i, arm.center[i] - sign * arm.window * 0.8)
        pose = [bus.state()[k][4] for k in arm.ids]
        arm.mode(i, MODE_CURRENT)
        torque = a.inertia_torque * max(static, 0.05)
        units = sign * min(int(round(torque / kt / CUR_UNIT)), a.max_current)
        rows = []
        start = bus.state()[i][4]
        t0 = bus.now()
        bus.write(i, "goal_current", units)
        while bus.now() - t0 < 0.8:
            s = bus.state()[i]
            arm.guard(i, s)
            rows.append([bus.now() - t0, units * CUR_UNIT, s[2], s[3], s[4]])
            if abs(s[4] - start) > math.radians(a.inertia_deg):
                break
        bus.write(i, "goal_current", 0)
        write_csv(os.path.join(out, "inertia_j%d_%s.csv" % (i, "pos" if sign > 0 else "neg")),
                  ["t", "goal_current", "current", "velocity", "position"], rows)
        results.append(dict(direction=sign, pose=pose, file=os.path.basename(out)))
        bus.sleep(0.5)
    return results


def test_steps(arm, i, out):
    """Position-mode goal steps with SPOT's servo settings, logged as fast as the bus allows."""
    a, bus = arm.args, arm.bus
    c = arm.center[i]
    arm.goto(i, c)
    arm.mode(i, MODE_POSITION, **SPOT_POSITION)
    goals = [c + math.radians(d) for d in a.step_deg]
    rows = []
    t0 = bus.now()
    for g in goals:
        bus.write(i, "goal_position", round(g * PULSES + 2048))
        t_end = bus.now() + a.step_time
        while bus.now() < t_end:
            st = bus.state()
            arm.guard(i, st[i])
            rows.append([bus.now() - t0, i, g] + [st[k][4] for k in arm.ids_all] + [st[i][3], st[i][2], st[i][1]])
    write_csv(os.path.join(out, "steps_j%d.csv" % i),
              ["t", "joint", "goal"] + ["q%d" % k for k in arm.ids_all] + ["velocity", "current", "pwm"], rows)
    print("    %d samples over %.1f s" % (len(rows), rows[-1][0] if rows else 0))


# ---------------------------------------------------------------------------- fits

def fit_joint(out, i, kt):
    r = {"index": i, "id": i, "name": NAMES.get(i, "joint %d" % i)}
    path = os.path.join(out, "breakaway_j%d.csv" % i)
    static = None
    if os.path.exists(path):
        rows = read_csv(path)
        pos, neg = [], []
        for d in (1, -1):
            run = [x for x in rows if x["direction"] == d]
            # each ramp ends where the joint moved (or at the current cap, which doesn't count)
            first = 0
            for k, x in enumerate(run):
                last = k == len(run) - 1 or run[k + 1]["t"] - x["t"] > 0.2
                if last:
                    if abs(x["position"] - run[first]["position"]) > 0 or abs(x["velocity"]) > 0:
                        (pos if d > 0 else neg).append(kt * abs(x["current"]))
                    first = k + 1
        if pos or neg:
            r["static_pos"] = median(pos) if pos else None
            r["static_neg"] = median(neg) if neg else None
            static = median(pos + neg)
            r["static"] = static
            r["static_spread"] = (max(pos + neg) - min(pos + neg)) if len(pos + neg) > 1 else 0.0
    path = os.path.join(out, "sweep_points_j%d.json" % i)
    if os.path.exists(path):
        pts = [x for x in json.load(open(path)) if abs(x["velocity"]) >= 0.5 * abs(x["command"]) > 0]   # steady sliding only
        # Friction opposes the motion, so at a steady speed the motor pushes along it: the torque along the motion
        # (the current's sign against the speed's, not its size alone). A point where the current opposes the
        # motion can't be friction (the servo's current reading at speed, or something else driving the joint):
        # left out, and counted
        along = [(abs(x["velocity"]), kt * x["current"] * (1 if x["velocity"] > 0 else -1)) for x in pts]
        dropped = [round(math.copysign(v, x["velocity"]), 3) for (v, t), x in zip(along, pts) if t <= 0]
        if len(dropped) * 3 >= len(pts):
            r.setdefault("warnings", []).append("friction against speed: " + "%d of %d speeds had the current opposing the "
                                                "motion" % (len(dropped), len(pts)))
        if dropped:
            r["sweep_dropped"] = dropped
            r["sweep_note"] = ("%d of %d speeds left out: the current there opposed the motion (%s rad/s), which friction can't "
                               "do" % (len(dropped), len(pts), ", ".join("%+.3f" % d for d in dropped)))
        pts = [x for (v, t), x in zip(along, pts) if t > 0]
        if len(pts) >= 3:
            v = [abs(p["velocity"]) for p in pts]
            tau = [kt * abs(p["current"]) for p in pts]
            best = None
            for k in range(60):   # Stribeck velocity on a log grid, the rest linear
                vs = 0.003 * 10 ** (k / 60 * 2.5)
                if static is not None:
                    rows = [[1 - math.exp(-(x / vs) ** 2), x] for x in v]
                    rhs = [t - static * math.exp(-(x / vs) ** 2) for x, t in zip(v, tau)]
                    try:
                        tc, b = lstsq(rows, rhs)
                    except ValueError:
                        continue
                    ts = static
                else:
                    rows = [[1, math.exp(-(x / vs) ** 2), x] for x in v]
                    try:
                        tc, dt, b = lstsq(rows, tau)
                    except ValueError:
                        continue
                    ts = tc + dt
                pred = [tc + (ts - tc) * math.exp(-(x / vs) ** 2) + b * x for x in v]
                rms = math.sqrt(sum((p - t) ** 2 for p, t in zip(pred, tau)) / len(tau))
                if tc > 0 and b >= 0 and ts >= tc and (best is None or rms < best[0]):
                    best = (rms, tc, ts, vs, b)
            if best:
                r.update(coulomb=best[1], stribeck=best[3], viscous=best[4], sweep_rms=best[0])
                if best[0] > max(best[1], 0.01):
                    r.setdefault("warnings", []).append(
                        "friction against speed: the points scatter by %.3f N m about the fit, more than the sliding "
                        "friction it finds (%.3f N m): too little friction for the servo's current reading to show, or "
                        "something else loading the joint; don't rely on the Coulomb, Stribeck and viscous terms"
                        % (best[0], best[1]))
                if static is None:
                    r["static"] = best[2]
                # Below the slowest steady speed the data can't tell where the drop from static to
                # Coulomb friction happens (the joint stick-slips there): take half that speed
                vmin = min(v)
                if best[3] < 0.6 * vmin:
                    r["stribeck"] = 0.5 * vmin
                    r["stribeck_note"] = ("below the slowest steady speed measured (%.3f rad/s), where the joint stick-slips; "
                                          "set to half of it" % vmin)
    files = [f for f in os.listdir(out) if f.startswith("inertia_j%d_" % i)]
    Js = []
    for f in files:
        rows = read_csv(os.path.join(out, f))
        moving = [x for x in rows if abs(x["velocity"]) > 0]
        if len(moving) < 6 or r.get("coulomb") is None:
            continue
        t0 = moving[0]["t"]
        # position over time after it starts: q = q0 + w0 t + a t^2 / 2
        rows_fit = [[1, x["t"] - t0, 0.5 * (x["t"] - t0) ** 2] for x in moving]
        try:
            _, _, acc = lstsq(rows_fit, [x["position"] for x in moving])
        except ValueError:
            continue
        torque = kt * median([abs(x["current"]) for x in moving])
        vmean = median([abs(x["velocity"]) for x in moving])
        net = torque - r["coulomb"] - r.get("viscous", 0) * vmean
        if abs(acc) > 1e-3 and net > 0:
            Js.append(net / abs(acc))
            r["inertia_neg" if "_neg" in f else "inertia_pos"] = net / abs(acc)
    if Js:
        r["inertia"] = median(Js)
        if len(Js) == 1 and len(files) == 2:
            r.setdefault("warnings", []).append(
                "inertia: from the step turning %s only (the other's push didn't exceed the fitted friction): check "
                "it by hand, torque off" % ("-" if "inertia_neg" in r else "+"))
        if len(Js) == 2 and max(Js) > 3 * min(Js):
            r.setdefault("warnings", []).append(
                "inertia: %.4g kg m^2 turning +, %.4g turning -; a joint's inertia is the same both ways, so something "
                "loads it one way (a cable dragging or catching?): check it by hand, torque off"
                % (r["inertia_pos"], r["inertia_neg"]))
        meta = os.path.join(out, "inertia_pose_j%d.json" % i)
        if os.path.exists(meta):
            r["pose"] = json.load(open(meta))
    return r


def fit_all(out, joints, kt, extra):
    result = {"format": "spot-arm-calibration", "version": 1,
              "created": datetime.datetime.now().isoformat(timespec="seconds"), "folder": os.path.abspath(out),
              "kt": kt, "joints": []}
    result.update(extra)
    for i in joints:
        r = fit_joint(out, i, kt[str(i)] if isinstance(kt, dict) else kt)
        result["joints"].append(r)
        def f(x, n="%.3f"):
            return n % x if isinstance(x, (int, float)) else "-"
        print("  joint %d (%s): static %s N m (+%s/-%s), coulomb %s N m, stribeck %s rad/s, viscous %s N m s/rad, inertia %s kg m^2"
              % (i, r["name"], f(r.get("static")), f(r.get("static_pos")), f(r.get("static_neg")), f(r.get("coulomb")),
                 f(r.get("stribeck")), f(r.get("viscous")), f(r.get("inertia"), "%.4g")))
    path = os.path.join(out, "arm_calibration.json")
    with open(path, "w") as fh:
        json.dump(result, fh, indent=2)
    print("Wrote %s\nCopy it to the SPOT project's folder (next to spot_settings.json); the next "
          "simulation uses it." % path)
    return result


# ---------------------------------------------------------------------------- main

def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0], formatter_class=argparse.RawDescriptionHelpFormatter,
                                epilog=__doc__.split("\n", 1)[1])
    p.add_argument("--port", default="/dev/ttyUSB0")
    p.add_argument("--baud", type=int, default=1000000)
    p.add_argument("--joints", type=int, nargs="+", default=[1, 2, 3], help="servo IDs to calibrate (1 shoulder, 2 elbow, 3 wrist)")
    p.add_argument("--all-ids", type=int, nargs="+", default=[1, 2, 3], help="every servo on the bus (the others hold)")
    p.add_argument("--tests", nargs="+", default=["breakaway", "sweep", "inertia", "steps"],
                   choices=["breakaway", "sweep", "inertia", "steps"])
    p.add_argument("--out", default="runs", help="folder for the run's data (a timestamped folder inside)")
    p.add_argument("--fit", metavar="RUN_FOLDER", help="only fit the data in RUN_FOLDER again")
    p.add_argument("--kt", type=float, help="torque constant at the joint [N m/A] (default: from the servo model)")
    p.add_argument("--window", type=float, default=25.0, help="each joint moves within +-WINDOW deg of its start")
    p.add_argument("--ramp", type=float, default=0.03, help="breakaway current ramp [A/s]")
    p.add_argument("--repeats", type=int, default=3, help="breakaway ramps per direction")
    p.add_argument("--move-deg", type=float, default=0.5, help="motion that counts as breakaway [deg]")
    p.add_argument("--max-current", type=int, default=450, help="current cap in tests [3.36 mA units]")
    p.add_argument("--speeds", type=int, nargs="+", default=[1, 2, 3, 5, 8, 12, 18, 25, 35],
                   help="sweep speeds [0.229 rev/min units]")
    p.add_argument("--pass-time", type=float, default=25.0, help="longest sweep pass [s]")
    p.add_argument("--inertia-torque", type=float, default=2.0, help="torque step, as a multiple of the breakaway torque")
    p.add_argument("--inertia-deg", type=float, default=15.0, help="stop the torque step after this motion [deg]")
    p.add_argument("--step-deg", type=float, nargs="+", default=[10, 0, -10, 20, 0, -20, 5, 0], help="position steps [deg]")
    p.add_argument("--step-time", type=float, default=2.5, help="time at each position step [s]")
    p.add_argument("--simulate", action="store_true", help="a simulated arm instead of the servos")
    p.add_argument("--yes", action="store_true", help="start without asking (Enter); for the SPOT Console")
    a = p.parse_args()

    if a.fit:
        info = json.load(open(os.path.join(a.fit, "run.json")))
        args = info.get("args", {})
        fit_all(a.fit, info["joints"], info["kt"], {"servos": info.get("servos"), "supply_voltage": info.get("supply_voltage"),
                                                    "simulated": bool(args.get("simulate")),
                                                    "tests": [t for t in ("breakaway", "sweep", "inertia", "steps")
                                                              if t in args.get("tests", ())]})
        return

    # SIGTERM stops it (the SPOT Console's agent starts it from a shell that ignores SIGTERM, which it would
    # inherit): until the servos are set up, at once; afterwards through on_signal below, which restores them
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    out = os.path.join(a.out, datetime.datetime.now().strftime("%Y%m%d_%H%M%S"))
    os.makedirs(out, exist_ok=True)
    bus = SimBus(a.all_ids) if a.simulate else DxlBus(a.port, a.baud, a.all_ids)
    arm = Arm(bus, a)
    arm.ids_all = a.all_ids
    stop = {"now": False}

    def on_signal(*_):
        stop["now"] = True
        raise KeyboardInterrupt
    signal.signal(signal.SIGTERM, on_signal)

    try:
        models = arm.setup()
        voltage = bus.read(a.all_ids[0], "present_input_voltage") * 0.1
        kt = {}
        for i in a.joints:
            if models[i] == 30:
                sys.exit("ID %d is an MX-28: it doesn't measure current, so it can't be calibrated this way." % i)
            kt[str(i)] = a.kt or KT.get(models[i])
            if kt[str(i)] is None:
                sys.exit("No torque constant known for ID %d (%s): give one with --kt." % (i, MODELS.get(models[i], models[i])))
        print("Servos: %s; supply %.1f V; data in %s" % (", ".join("ID %d %s" % (i, MODELS.get(m, m)) for i, m in models.items()),
                                                       voltage, out))
        if a.simulate:
            print("Simulated arm, true values: %s" % json.dumps(bus.truth))
        else:
            print("The platform must not float, and each joint will move up to %.0f deg from where it is. Ctrl+C stops." % a.window)
            if not a.yes:
                input("Press Enter to start... ")
        json.dump({"joints": a.joints, "kt": kt, "servos": {str(i): MODELS.get(m, m) for i, m in models.items()},
                   "supply_voltage": voltage, "args": vars(a)}, open(os.path.join(out, "run.json"), "w"), indent=2)
        for i in a.all_ids:
            arm.center[i] = arm.hold(i)
        order = [t for t in ("breakaway", "sweep", "inertia", "steps") if t in a.tests]
        progress = {"index": 0, "total": len(a.joints) * len(order)}

        def announce(joint, test):
            progress["index"] += 1
            print("PROGRESS %s" % json.dumps({"joint": joint, "test": test, "index": progress["index"],
                                              "total": progress["total"]}))
            sys.stdout.flush()
        for i in a.joints:
            name = NAMES.get(i, "joint %d" % i)
            print("Joint %d (%s), from %.1f deg:" % (i, name, math.degrees(arm.center[i])))
            if "breakaway" in a.tests:
                announce(i, "breakaway")
                print("  breakaway (current ramps)")
                test_breakaway(arm, i, out, kt[str(i)])
            static = fit_joint(out, i, kt[str(i)]).get("static", 0.3)
            if "sweep" in a.tests:
                announce(i, "sweep")
                print("  friction against speed (velocity mode)")
                pts = test_sweep(arm, i, out)
                json.dump(pts, open(os.path.join(out, "sweep_points_j%d.json" % i), "w"), indent=2)
            if "inertia" in a.tests:
                announce(i, "inertia")
                print("  inertia (torque steps)")
                res = test_inertia(arm, i, out, kt[str(i)], static)
                json.dump(res[0]["pose"], open(os.path.join(out, "inertia_pose_j%d.json" % i), "w"))
            if "steps" in a.tests:
                announce(i, "steps")
                print("  position steps (SPOT's servo settings)")
                test_steps(arm, i, out)
            arm.goto(i, arm.center[i])
            arm.hold(i)
            for k in a.all_ids:
                temp = bus.read(k, "present_temperature")
                if temp > 65:
                    raise Abort("ID %d is at %d C: let it cool down" % (k, temp))
        print("Fitting:")
        fit_all(out, a.joints, kt, {"servos": {str(i): MODELS.get(m, m) for i, m in models.items()}, "supply_voltage": voltage,
                                    "simulated": bool(a.simulate), "tests": order})
        ok = True
    except KeyboardInterrupt:
        print("\nStopped: torque off.")
        ok = False
    except Abort as e:
        print("\nStopped: %s. Torque off." % e)
        ok = False
    finally:
        arm.restore()
        bus.close()
    if not ok:
        sys.exit(1)


if __name__ == "__main__":
    main()
