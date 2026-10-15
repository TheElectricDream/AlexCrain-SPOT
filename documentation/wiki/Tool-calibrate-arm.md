# calibrate_arm.py

**Files:** `resources/arm_calibration/calibrate_arm.py`, `README.md` · **Used by:** [[spot.arm.loadCalibration|spot-arm-loadCalibration]], [[spot.arm.compareSteps|spot-arm-compareSteps]]

<!-- source: resources/arm_calibration/calibrate_arm.py -->

## What it is for

Measure the real arm's joint friction (and inertia) from its Dynamixel servos, and write `arm_calibration.json` for the simulated arm ([[The Arm|Math-Arm]]). It runs by hand (below) or from the lab console's [[Calibration tab|Calibration-Tab]] (Arm). Four tests per joint, the other joints holding their position:

| Test | Servo mode | Gives |
|---|---|---|
| **breakaway** | current, ramped slowly from zero each way until the joint moves (3× per direction) | the breakaway (static) torque |
| **sweep** | velocity, constant speeds back and forth across a window | friction torque against speed → `coulomb`, `stribeck`, `viscous` by a fit of $T_c + (T_s - T_c)e^{-(v/v_s)^2} + b\,v$ |
| **inertia** | current, a torque step from rest | the joint's inertia (link + rotor); SPOT subtracts the links' part to get the `armature` |
| **steps** | position, with SPOT's old servo settings (P 400, I 0, D 200, profile velocity 20) | goal-step logs for [[spot.arm.compareSteps|spot-arm-compareSteps]] |

Torque = measured motor current × the datasheet torque constant (MX-64(2.0): 6.0/4.1 = 1.46 N m/A; MX-106: 8.4/5.2). MX-28 servos don't measure current and can't be calibrated this way.

## From the Calibration tab

The lab console runs the script on the Chaser (RED) through its agent ([[spot_agent.sh|Hardware-Deploy-Scripts]]: `arm-cal-check`, `arm-cal-receive`, `arm-cal-start`, `arm-cal-status`, `arm-cal-stop`, `arm-cal-data`): it checks what the Chaser has, sends this script, starts it with `--yes`, follows its `PROGRESS` lines as the steps, and collects the run's data and fit into the lab's calibration (the `arm` section of `~/.spot_calibration.json` on the TX2); projects copy it into their `resources/spot_calibration.json` with Update from Lab. During a real run the Chaser rests: the Hardware tab shows it as Calibrating and nothing may float it. With the **Rehearse (Simulated Servos)** switch on it runs the script's simulated servos (`--simulate`) instead: it needs neither the U2D2 nor a free port, moves nothing, and its fit is shown but not saved.

## In the lab, by hand

1. `pip install dynamixel-sdk` on RED's computer (or any computer with the arm's U2D2).
2. No SPOT program running (it holds `/dev/ttyUSB0`).
3. The platform resting on the table (not floating). Each joint moves up to ±25° (`--window`) from where it is: clear the space, start away from the limits, keep the power switch at hand. Ctrl+C stops at once with every torque off.
4. `python3 calibrate_arm.py` (≈15 min for the three joints). Options: `--joints 3`, `--tests breakaway sweep`, `--window 15`, `--port`, `--simulate` (rehearse without hardware against a built-in model whose true values are printed), `--fit runs/<stamp>` (fit saved data again, e.g. with another `--kt`), `--yes` (start without the "Press Enter" prompt, as the Calibration tab does). Before each test it prints a line `PROGRESS {"joint", "test", "index", "total"}` (flushed at once). Exit code 0 when `arm_calibration.json` was written, 1 when stopped or aborted.
5. Copy `runs/<date_time>/arm_calibration.json` to the project's `resources/`; the next simulation uses it, unless the project's `spot_calibration.json` has a newer arm calibration from the Calibration tab ([[spot.arm.parameters|spot-arm-parameters]] takes the newer by `created`). Keep the run folder with the raw CSVs.

Afterwards in MATLAB: `spot.arm.compareSteps('runs/<stamp>', evalin('base', 'armModel'))` compares the simulated servos with the recorded steps.

## Function by function

**Lines 53–75.** The control-table addresses used (position gains, limits, goals, profiles, state), the realtime block read in one go (address 120, 16 bytes: tick, moving, PWM, current, velocity, position), the modes, the units, the known models and their torque constants, the joint names, SPOT's position-mode settings, and which registers are saved and restored.

**Lines 77–83.** `signed` (two's complement) and the `Abort` exception.

**Lines 89–153, `DxlBus`.** The real servos through the Dynamixel SDK: open the port at 1 Mbps, a `GroupSyncRead` of the state block; `ping` (model number), `read`/`write` a register by name (1, 2 or 4 bytes, signed for goals and present values), `state` (all servos: tick, PWM, current [A], velocity [rad/s], position [rad] about 2048), `close`.

**Lines 158–283, `SimBus`.** The same interface on a simulated arm with stick-slip friction and Dynamixel-like controllers (current mode with back-EMF, velocity PI, position PID with profile), on a virtual clock where each transaction takes 1–2 ms, with randomised true parameters printed at the start so the fits can be checked. `_step` (195–235) is the same implicit friction scheme as [[spot.arm.Plant|spot-arm-Plant]] for one joint.

**Lines 288–343, `Arm`.** Helpers: `setup` (ping every servo, save its registers), `restore` (torque off and registers back, whatever happened), `mode` (switch a servo's mode with torque off and set registers), `hold` (position mode at the present position), `goto` (move to an angle and wait until it settles, 8 s at most), `guard` (abort if a joint leaves its window by more than 5°), `t` (time since the start).

**Lines 346–383.** CSV read/write, a small least-squares solver (normal equations, Gaussian elimination) and `median`.

**Lines 388–420, `test_breakaway`.** Current mode; for each repeat and direction: goal 0, settle, then raise the current at `--ramp` A/s until the joint has moved 0.5° or reached 2 velocity units, or the current cap; log every sample and the breakaway event; back to the window's middle if it drifted.

**Lines 423–462, `test_sweep`.** From one edge of the window, velocity mode; for each speed and direction: command it, log, and once the measured speed has been within 15 % of the command for 0.3 s start collecting samples until the other edge (or the pass time); the median current and speed of ≥10 samples is one point; back to the edge.

**Lines 465–490, `test_inertia`.** From 80 % of the window on one side: record the pose of all joints, current mode, a torque of `--inertia-torque` × the breakaway (at least 0.05 N m) as a step, log for up to 0.8 s or 15° of motion; both directions.

**Lines 493–511, `test_steps`.** Position mode with SPOT's settings; the goal steps `--step-deg` from the centre, each held `--step-time`, logging all joints' angles, the velocity, current and PWM as fast as the bus allows.

**Lines 516–602, `fit_joint`.** From the saved files: the breakaway torque per direction (the current at the end of each ramp that moved) and its median and spread; the sweep points that were steady sliding, fitted on a log grid of Stribeck velocities (the rest linear least squares; with a known breakaway the fit has two unknowns, otherwise three), keeping the best positive fit, and a note when the Stribeck velocity is below the slowest steady speed (set to half of it: the data can't say); the inertia from each torque step by fitting $q = q_0 + \omega_0 t + \tfrac{1}{2} a t^2$ to the motion and dividing the net torque (applied minus Coulomb minus viscous) by the acceleration, median over the steps, with the pose it was measured at.

**Lines 605–622, `fit_all`.** The JSON: format, version, date (`created`), folder, torque constant(s), the servos, the supply voltage, whether the servos were `simulated`, the `tests` that ran, and one entry per joint; printed as a summary. (A `--fit` re-fit takes them from the run's `run.json`.)

**Lines 627–750, `main`.** The arguments; `--fit` re-fits a run (with what `run.json` recorded); otherwise SIGTERM back to its default action (the agent starts the script from a shell that ignores SIGTERM, which it would inherit), so that it stops at once until `on_signal` takes over once the bus is open (it raises `KeyboardInterrupt`, so the servos are restored); a time-stamped output folder, the real or simulated bus, model check (MX-28 refused, torque constant required), a confirmation prompt (not with `--simulate` or `--yes`), `run.json`, every servo held at its position, then for each joint the selected tests, each announced by `announce` (a `PROGRESS` line, flushed), with a temperature check of all servos after each joint (aborting above 65 °C); the fit; and finally restore and close, then exit code 1 if it was stopped or aborted.

## The code
<!-- listing -->
<details>
<summary><b>resources/arm_calibration/calibrate_arm.py</b> (754 lines; click to show)</summary>

```python
   1  #!/usr/bin/env python3
   2  """Calibrate the SPOT arm's joint friction (and inertia) from the real Dynamixel servos.
   3  
   4  Writes arm_calibration.json for SPOT's arm model (copy it to the project's resources folder;
   5  spot.arm.parameters reads it, see spot.arm.loadCalibration), and the raw data as CSV files.
   6  
   7  Tests, for each joint in turn (the other joints hold their position in position mode):
   8    breakaway  current mode, the current ramped slowly from zero in each direction until the joint
   9               moves: the static (breakaway) friction torque, several times per direction
  10    sweep      velocity mode, constant speeds back and forth across a window: the friction torque
  11               at each speed (motor current x torque constant), fitted with
  12               coulomb + (static - coulomb) exp(-(v/stribeck)^2) + viscous v
  13    inertia    current mode, a torque step from rest: the joint's acceleration gives its inertia
  14               (link + rotor); SPOT subtracts the link's part to get the rotor's (armature)
  15    steps      position mode with SPOT's servo settings (P 400, I 0, D 200, profile velocity 20):
  16               goal steps, logged for spot.arm.compareSteps (the simulated servo against the real)
  17  
  18  Before running:
  19    - the platform rests on the table (not floating) or is held: the arm's motion must not move it
  20    - each tested joint will move within +-WINDOW degrees (default 25) of where it is: clear the
  21      space, and start with the joints away from their limits
  22    - keep the power switch at hand; Ctrl+C stops at once (all torque off)
  23    - pip install dynamixel-sdk (the script needs nothing else)
  24  
  25  Examples:
  26    python3 calibrate_arm.py                          # all tests, joints 1-3, /dev/ttyUSB0
  27    python3 calibrate_arm.py --joints 3 --tests breakaway sweep
  28    python3 calibrate_arm.py --fit runs/20261011_101500  # fit again from saved data
  29    python3 calibrate_arm.py --simulate               # try it without hardware (a built-in model)
  30    python3 calibrate_arm.py --yes                    # no "Press Enter" (the SPOT Console's Calibration tab
  31                                                      # runs it this way on the Chaser, through its agent)
  32  
  33  Progress: a line "PROGRESS {json}" before each test ({"joint", "test", "index", "total"}), for the
  34  Calibration tab. Exit code 0 when arm_calibration.json was written; 1 when stopped or aborted.
  35  
  36  Units: the servos' (protocol 2.0, MX series): position 4096 pulses/rev, velocity 0.229 rev/min,
  37  current 3.36 mA, PWM 885 = 100 %. Torque = Kt x current, Kt from the datasheet (--kt to change).
  38  """
  39  
  40  import argparse
  41  import csv
  42  import datetime
  43  import json
  44  import math
  45  import os
  46  import random
  47  import signal
  48  import sys
  49  import time
  50  
  51  # ---------------------------------------------------------------------------- servo constants
  52  
  53  ADDR = {
  54      "model_number": (0, 2), "operating_mode": (11, 1), "drive_mode": (10, 1),
  55      "pwm_limit": (36, 2), "current_limit": (38, 2), "velocity_limit": (44, 4),
  56      "max_position": (48, 4), "min_position": (52, 4), "torque_enable": (64, 1),
  57      "velocity_i": (76, 2), "velocity_p": (78, 2), "position_d": (80, 2), "position_i": (82, 2),
  58      "position_p": (84, 2), "ff2": (88, 2), "ff1": (90, 2),
  59      "goal_pwm": (100, 2), "goal_current": (102, 2), "goal_velocity": (104, 4),
  60      "profile_acceleration": (108, 4), "profile_velocity": (112, 4), "goal_position": (116, 4),
  61      "present_input_voltage": (144, 2), "present_temperature": (146, 1),
  62  }
  63  STATE_START, STATE_LEN = 120, 16   # realtime tick (2), moving, moving status, PWM (2), current (2), velocity (4), position (4)
  64  MODE_CURRENT, MODE_VELOCITY, MODE_POSITION = 0, 1, 3
  65  PULSES = 4096 / (2 * math.pi)        # per rad
  66  VEL_UNIT = 0.229 * 2 * math.pi / 60  # rad/s per unit
  67  CUR_UNIT = 3.36e-3                   # A per unit
  68  MODELS = {311: "MX-64(2.0)", 321: "MX-106(2.0)", 30: "MX-28(2.0)", 1020: "XM430-W350", 1120: "XM540-W270"}
  69  KT = {311: 6.0 / 4.1, 321: 8.4 / 5.2}  # N m/A at the joint, datasheet stall torque / stall current at 12 V
  70  NAMES = {1: "shoulder", 2: "elbow", 3: "wrist"}
  71  SPOT_POSITION = {"position_p": 400, "position_i": 0, "position_d": 200, "profile_velocity": 20, "profile_acceleration": 0,
  72                   "drive_mode": 0}   # as SPOT's old position-mode block set the servos up (its P/I/D gains)
  73  SAVED_REGISTERS = ["operating_mode", "drive_mode", "current_limit", "velocity_limit", "velocity_i", "velocity_p",
  74                     "position_d", "position_i", "position_p", "profile_acceleration", "profile_velocity"]
  75  
  76  
  77  def signed(value, nbytes):
  78      bits = 8 * nbytes
  79      return value - (1 << bits) if value >= 1 << (bits - 1) else value
  80  
  81  
  82  class Abort(Exception):
  83      pass
  84  
  85  
  86  # ---------------------------------------------------------------------------- the real servos
  87  
  88  class DxlBus:
  89      """The servos on a U2D2 (or the TX2's serial adapter), protocol 2.0."""
  90  
  91      def __init__(self, device, baud, ids):
  92          try:
  93              import dynamixel_sdk as sdk
  94          except ImportError:
  95              sys.exit("The Dynamixel SDK is missing: pip install dynamixel-sdk")
  96          self.sdk = sdk
  97          self.ids = list(ids)
  98          self.port = sdk.PortHandler(device)
  99          if not self.port.openPort():
 100              sys.exit("Cannot open %s (is the arm's adapter plugged in, and is SPOT not using it?)" % device)
 101          if not self.port.setBaudRate(baud):
 102              sys.exit("Cannot set %d baud on %s" % (baud, device))
 103          self.ph = sdk.PacketHandler(2.0)
 104          self.reader = sdk.GroupSyncRead(self.port, self.ph, STATE_START, STATE_LEN)
 105          for i in self.ids:
 106              self.reader.addParam(i)
 107  
 108      def now(self):
 109          return time.monotonic()
 110  
 111      def sleep(self, dt):
 112          time.sleep(dt)
 113  
 114      def _check(self, result, error, what):
 115          if result != self.sdk.COMM_SUCCESS:
 116              raise IOError("%s: %s" % (what, self.ph.getTxRxResult(result)))
 117          if error:
 118              raise IOError("%s: %s" % (what, self.ph.getRxPacketError(error)))
 119  
 120      def ping(self, i):
 121          model, result, error = self.ph.ping(self.port, i)
 122          self._check(result, error, "ping ID %d" % i)
 123          return model
 124  
 125      def read(self, i, name):
 126          addr, n = ADDR[name]
 127          fn = {1: self.ph.read1ByteTxRx, 2: self.ph.read2ByteTxRx, 4: self.ph.read4ByteTxRx}[n]
 128          value, result, error = fn(self.port, i, addr)
 129          self._check(result, error, "read %s of ID %d" % (name, i))
 130          return signed(value, n) if name.startswith("goal") or name.startswith("present") else value
 131  
 132      def write(self, i, name, value):
 133          addr, n = ADDR[name]
 134          value = int(round(value)) & ((1 << (8 * n)) - 1)
 135          fn = {1: self.ph.write1ByteTxRx, 2: self.ph.write2ByteTxRx, 4: self.ph.write4ByteTxRx}[n]
 136          result, error = fn(self.port, i, addr, value)
 137          self._check(result, error, "write %s of ID %d" % (name, i))
 138  
 139      def state(self):
 140          """{id: (tick ms, pwm, current A, velocity rad/s, position rad)} for all servos."""
 141          result = self.reader.txRxPacket()
 142          if result != self.sdk.COMM_SUCCESS:
 143              raise IOError("sync read: %s" % self.ph.getTxRxResult(result))
 144          out = {}
 145          for i in self.ids:
 146              g = lambda a, n: signed(self.reader.getData(i, a, n), n)
 147              out[i] = (self.reader.getData(i, 120, 2), g(124, 2), g(126, 2) * CUR_UNIT, g(128, 4) * VEL_UNIT,
 148                        (g(132, 4) - 2048) / PULSES)
 149          return out
 150  
 151      def close(self):
 152          self.port.closePort()
 153  
 154  
 155  # ---------------------------------------------------------------------------- a simulated arm
 156  
 157  class SimBus:
 158      """Three joints with stick-slip friction and Dynamixel-like controllers, on a virtual clock
 159      (each bus transaction takes 2 ms), to try the script without hardware. The "true" friction
 160      is printed so the fits can be checked against it."""
 161  
 162      def __init__(self, ids, seed=1):
 163          rnd = random.Random(seed)
 164          self.ids = list(ids)
 165          self.t = 0.0
 166          self.h = 2.5e-4
 167          self.kt, self.R, self.ke, self.V = 6.0 / 4.1, 12 / 4.1, 12 / (63 * 2 * math.pi / 60), 12.0
 168          self.j = {}
 169          for i in self.ids:
 170              self.j[i] = dict(
 171                  J={1: 0.095, 2: 0.035, 3: 0.004}.get(i, 0.01) + 0.0035 * rnd.uniform(0.8, 1.2),
 172                  Ts=rnd.uniform(0.25, 0.4), vs=rnd.uniform(0.03, 0.08), b=rnd.uniform(0.05, 0.12),
 173                  q=rnd.uniform(-0.3, 0.3), v=0.0, mode=MODE_POSITION, torque=0, goal_cur=0, goal_vel=0, goal_pos=None,
 174                  pd=None, ei=0.0, ep=0.0, vi=0.0, out=0.0, cur=0.0,
 175                  reg=dict(operating_mode=3, drive_mode=0, current_limit=1941, velocity_limit=285, velocity_i=1920,
 176                           velocity_p=100, position_d=0, position_i=0, position_p=850, profile_acceleration=0,
 177                           profile_velocity=0, pwm_limit=885, max_position=3072, min_position=1024))
 178              self.j[i]["Tc"] = self.j[i]["Ts"] * rnd.uniform(0.6, 0.8)
 179          self.truth = {i: {k: self.j[i][k] for k in ("Ts", "Tc", "vs", "b", "J")} for i in self.ids}
 180  
 181      def now(self):
 182          return self.t
 183  
 184      def sleep(self, dt):
 185          self._advance(dt)
 186  
 187      def _advance(self, dt):
 188          n = max(1, int(round(dt / self.h)))
 189          for _ in range(n):
 190              for i in self.ids:
 191                  self._step(self.j[i], dt / n)
 192          self.t += dt
 193  
 194      def _step(self, s, h):
 195          r = s["reg"]
 196          tau0, damp = 0.0, 0.0
 197          if s["torque"]:
 198              if s["mode"] == MODE_CURRENT:
 199                  imax = (self.V - self.ke * s["v"]) / self.R
 200                  imin = (-self.V - self.ke * s["v"]) / self.R
 201                  s["cur"] = min(max(s["goal_cur"] * CUR_UNIT, imin), imax)
 202                  tau0 = self.kt * s["cur"]
 203              else:
 204                  if s["mode"] == MODE_VELOCITY:
 205                      e = s["goal_vel"] - round(s["v"] / VEL_UNIT)
 206                      s["vi"] += e * h / 1e-3
 207                      u = r["velocity_p"] / 128 * e + r["velocity_i"] / 65536 * s["vi"]
 208                  else:
 209                      if s["pd"] is None:
 210                          s["pd"] = s["q"] * PULSES
 211                      goal = s["goal_pos"] if s["goal_pos"] is not None else s["q"] * PULSES
 212                      vp = r["profile_velocity"] * VEL_UNIT * PULSES
 213                      d = goal - s["pd"]
 214                      if vp <= 0 or abs(d) <= vp * h:
 215                          s["pd"] = goal
 216                      else:
 217                          s["pd"] += math.copysign(vp * h, d)
 218                      e = s["pd"] - round(s["q"] * PULSES)
 219                      de = (e - s["ep"]) * 1e-3 / h
 220                      s["ep"] = e
 221                      s["ei"] += e * h / 1e-3
 222                      u = r["position_p"] / 128 * e + r["position_i"] / 65536 * s["ei"] + r["position_d"] / 16 * de
 223                  u = min(max(u, -r["pwm_limit"]), r["pwm_limit"])
 224                  s["out"] = u
 225                  tau0 = self.kt / self.R * u / 885 * self.V
 226                  damp = self.kt * self.ke / self.R
 227                  s["cur"] = (u / 885 * self.V - self.ke * s["v"]) / self.R
 228          # implicit viscous + back-EMF, then stick or slip
 229          J, b = s["J"], s["b"] + damp
 230          vfree = (J * s["v"] + h * tau0) / (J + h * b)
 231          mu = s["Tc"] + (s["Ts"] - s["Tc"]) * math.exp(-(s["v"] / s["vs"]) ** 2)
 232          p = min(max(-vfree * (J + h * b), -h * mu), h * mu)
 233          s["v"] = vfree + p / (J + h * b)
 234          s["q"] += h * s["v"]
 235  
 236      def ping(self, i):
 237          self._advance(1e-3)
 238          return 311
 239  
 240      def read(self, i, name):
 241          self._advance(1e-3)
 242          s = self.j[i]
 243          if name == "model_number":
 244              return 311
 245          if name == "torque_enable":
 246              return s["torque"]
 247          if name == "present_input_voltage":
 248              return 120
 249          if name == "present_temperature":
 250              return 35
 251          return s["reg"].get(name, 0)
 252  
 253      def write(self, i, name, value):
 254          self._advance(1e-3)
 255          s = self.j[i]
 256          if name == "torque_enable":
 257              s["torque"] = int(value)
 258              s.update(pd=None, ei=0.0, ep=0.0, vi=0.0, goal_pos=None, goal_cur=0, goal_vel=0)
 259          elif name == "operating_mode":
 260              s["mode"] = int(value)
 261              s["reg"]["operating_mode"] = int(value)
 262          elif name == "goal_current":
 263              s["goal_cur"] = signed(int(value) & 0xFFFF, 2)
 264          elif name == "goal_velocity":
 265              s["goal_vel"] = signed(int(value) & 0xFFFFFFFF, 4)
 266          elif name == "goal_position":
 267              s["goal_pos"] = int(value) - 2048
 268          else:
 269              s["reg"][name] = int(value)
 270  
 271      def state(self):
 272          self._advance(2e-3)
 273          out = {}
 274          for i in self.ids:
 275              s = self.j[i]
 276              cur = round((s["cur"] + random.gauss(0, 0.004)) / CUR_UNIT) * CUR_UNIT
 277              out[i] = (int(self.t * 1000) & 0xFFFF, int(s["out"]), cur, round(s["v"] / VEL_UNIT) * VEL_UNIT,
 278                        round(s["q"] * PULSES) / PULSES)
 279          return out
 280  
 281      def close(self):
 282          pass
 283  
 284  
 285  # ---------------------------------------------------------------------------- helpers
 286  
 287  class Arm:
 288      def __init__(self, bus, args):
 289          self.bus, self.args = bus, args
 290          self.ids = bus.ids
 291          self.window = math.radians(args.window)
 292          self.saved = {}
 293          self.center = {}
 294          self.t0 = bus.now()
 295  
 296      def setup(self):
 297          info = {}
 298          for i in self.ids:
 299              model = self.bus.ping(i)
 300              info[i] = model
 301              self.saved[i] = {n: self.bus.read(i, n) for n in SAVED_REGISTERS}
 302          return info
 303  
 304      def restore(self):
 305          for i in self.ids:
 306              try:
 307                  self.bus.write(i, "torque_enable", 0)
 308                  for n, v in self.saved.get(i, {}).items():
 309                      self.bus.write(i, n, v)
 310              except Exception as e:   # keep going: the other servos still get their torque off
 311                  print("  could not restore ID %d: %s" % (i, e))
 312  
 313      def mode(self, i, mode, **registers):
 314          self.bus.write(i, "torque_enable", 0)
 315          self.bus.write(i, "operating_mode", mode)
 316          for n, v in registers.items():
 317              self.bus.write(i, n, v)
 318          self.bus.write(i, "torque_enable", 1)
 319  
 320      def hold(self, i):
 321          """Position mode at the present position, with SPOT's settings."""
 322          q = self.bus.state()[i][4]
 323          self.mode(i, MODE_POSITION, **SPOT_POSITION)
 324          self.bus.write(i, "goal_position", round(q * PULSES + 2048))
 325          return q
 326  
 327      def goto(self, i, q, timeout=8.0):
 328          self.mode(i, MODE_POSITION, **SPOT_POSITION)
 329          self.bus.write(i, "goal_position", round(q * PULSES + 2048))
 330          t_end = self.bus.now() + timeout
 331          while self.bus.now() < t_end:
 332              s = self.bus.state()[i]
 333              if abs(s[4] - q) < math.radians(1.5) and abs(s[3]) < 0.03:
 334                  break
 335          self.bus.sleep(0.3)
 336  
 337      def guard(self, i, s):
 338          if abs(s[4] - self.center[i]) > self.window + math.radians(5):
 339              raise Abort("joint %d left its window (%.1f deg from where it started)" % (i, math.degrees(s[4] - self.center[i])))
 340  
 341      def t(self):
 342          return self.bus.now() - self.t0
 343  
 344  
 345  def write_csv(path, header, rows):
 346      with open(path, "w", newline="") as f:
 347          w = csv.writer(f)
 348          w.writerow(header)
 349          w.writerows(rows)
 350  
 351  
 352  def read_csv(path):
 353      with open(path) as f:
 354          r = csv.reader(f)
 355          header = next(r)
 356          return [dict(zip(header, map(float, row))) for row in r]
 357  
 358  
 359  def lstsq(rows, rhs):
 360      """Least squares for a few unknowns (normal equations, Gaussian elimination)."""
 361      n = len(rows[0])
 362      A = [[sum(r[a] * r[b] for r in rows) for b in range(n)] for a in range(n)]
 363      y = [sum(r[a] * v for r, v in zip(rows, rhs)) for a in range(n)]
 364      for c in range(n):
 365          p = max(range(c, n), key=lambda k: abs(A[k][c]))
 366          A[c], A[p], y[c], y[p] = A[p], A[c], y[p], y[c]
 367          if abs(A[c][c]) < 1e-15:
 368              raise ValueError("singular fit")
 369          for k in range(c + 1, n):
 370              f = A[k][c] / A[c][c]
 371              A[k] = [a - f * b for a, b in zip(A[k], A[c])]
 372              y[k] -= f * y[c]
 373      x = [0.0] * n
 374      for c in reversed(range(n)):
 375          x[c] = (y[c] - sum(A[c][k] * x[k] for k in range(c + 1, n))) / A[c][c]
 376      return x
 377  
 378  
 379  def median(xs):
 380      xs = sorted(xs)
 381      n = len(xs)
 382      return xs[n // 2] if n % 2 else 0.5 * (xs[n // 2 - 1] + xs[n // 2])
 383  
 384  
 385  # ---------------------------------------------------------------------------- tests
 386  
 387  def test_breakaway(arm, i, out, kt):
 388      """Ramp the current from zero until the joint moves, alternately each way."""
 389      a, bus = arm.args, arm.bus
 390      rows, events = [], []
 391      arm.mode(i, MODE_CURRENT)
 392      rate = a.ramp / CUR_UNIT   # units per second
 393      for rep in range(a.repeats):
 394          for sign in (1, -1):
 395              bus.write(i, "goal_current", 0)
 396              bus.sleep(0.5)
 397              s0 = bus.state()[i]
 398              start, t_start = s0[4], bus.now()
 399              while True:
 400                  units = sign * min(int(rate * (bus.now() - t_start)), a.max_current)
 401                  bus.write(i, "goal_current", units)
 402                  s = bus.state()[i]
 403                  arm.guard(i, s)
 404                  rows.append([arm.t(), sign, units * CUR_UNIT, s[2], s[3], s[4]])
 405                  if abs(s[4] - start) > math.radians(a.move_deg) or abs(s[3]) >= 2 * VEL_UNIT:
 406                      events.append(dict(direction=sign, current=s[2], command=units * CUR_UNIT, torque=kt * abs(s[2])))
 407                      print("    %s: moved at %.0f mA (%.3f N m)" % ("+" if sign > 0 else "-", 1e3 * abs(s[2]), kt * abs(s[2])))
 408                      break
 409                  if abs(units) >= a.max_current:
 410                      print("    %s: no motion up to the current cap (%.0f mA)" % ("+" if sign > 0 else "-", a.max_current * CUR_UNIT * 1e3))
 411                      break
 412              bus.write(i, "goal_current", 0)
 413              bus.sleep(0.3)
 414              if abs(bus.state()[i][4] - arm.center[i]) > arm.window / 2:   # drifted: back to the middle
 415                  arm.goto(i, arm.center[i])
 416                  arm.mode(i, MODE_CURRENT)
 417      bus.write(i, "goal_current", 0)
 418      write_csv(os.path.join(out, "breakaway_j%d.csv" % i), ["t", "direction", "goal_current", "current", "velocity", "position"], rows)
 419      return events
 420  
 421  
 422  def test_sweep(arm, i, out):
 423      """Constant speeds back and forth across the window (velocity mode)."""
 424      a, bus = arm.args, arm.bus
 425      rows, points = [], []
 426      arm.goto(i, arm.center[i] - arm.window)
 427      arm.mode(i, MODE_VELOCITY, velocity_limit=max(a.speeds) + 5, velocity_p=100, velocity_i=1920, profile_acceleration=0)
 428      lo, hi = arm.center[i] - arm.window, arm.center[i] + arm.window
 429      for units in a.speeds:
 430          for sign in (1, -1):
 431              v = sign * units * VEL_UNIT
 432              bus.write(i, "goal_velocity", sign * units)
 433              t_start, steady, samples = bus.now(), None, []
 434              while True:
 435                  s = bus.state()[i]
 436                  arm.guard(i, s)
 437                  rows.append([arm.t(), sign * units, v, s[2], s[3], s[4]])
 438                  if (sign > 0 and s[4] >= hi) or (sign < 0 and s[4] <= lo) or bus.now() - t_start > a.pass_time:
 439                      break
 440                  if abs(s[3] - v) <= max(0.15 * abs(v), 0.5 * VEL_UNIT):   # sliding at the speed (not stuck, not slipping)
 441                      steady = steady or bus.now()
 442                      if bus.now() - steady > 0.3:
 443                          samples.append((s[2], s[3]))
 444                  else:
 445                      steady = None
 446              if len(samples) >= 10:
 447                  points.append(dict(command=v, current=median([c for c, _ in samples]), velocity=median([w for _, w in samples]),
 448                                     samples=len(samples)))
 449                  print("    %+.3f rad/s: %.0f mA over %d samples" % (v, 1e3 * points[-1]["current"], len(samples)))
 450              else:
 451                  print("    %+.3f rad/s: not steady long enough (raise --window or --pass-time)" % v)
 452              bus.write(i, "goal_velocity", 0)
 453              bus.sleep(0.3)
 454              if sign > 0:
 455                  arm.goto(i, hi)
 456              else:
 457                  arm.goto(i, lo)
 458              arm.mode(i, MODE_VELOCITY, velocity_limit=max(a.speeds) + 5, velocity_p=100, velocity_i=1920, profile_acceleration=0)
 459      bus.write(i, "goal_velocity", 0)
 460      write_csv(os.path.join(out, "sweep_j%d.csv" % i), ["t", "goal_units", "goal_velocity", "current", "velocity", "position"], rows)
 461      return points
 462  
 463  
 464  def test_inertia(arm, i, out, kt, static):
 465      """A torque step from rest, each way: the acceleration gives the inertia."""
 466      a, bus = arm.args, arm.bus
 467      results = []
 468      for sign in (1, -1):
 469          arm.goto(i, arm.center[i] - sign * arm.window * 0.8)
 470          pose = [bus.state()[k][4] for k in arm.ids]
 471          arm.mode(i, MODE_CURRENT)
 472          torque = a.inertia_torque * max(static, 0.05)
 473          units = sign * min(int(round(torque / kt / CUR_UNIT)), a.max_current)
 474          rows = []
 475          start = bus.state()[i][4]
 476          t0 = bus.now()
 477          bus.write(i, "goal_current", units)
 478          while bus.now() - t0 < 0.8:
 479              s = bus.state()[i]
 480              arm.guard(i, s)
 481              rows.append([bus.now() - t0, units * CUR_UNIT, s[2], s[3], s[4]])
 482              if abs(s[4] - start) > math.radians(a.inertia_deg):
 483                  break
 484          bus.write(i, "goal_current", 0)
 485          write_csv(os.path.join(out, "inertia_j%d_%s.csv" % (i, "pos" if sign > 0 else "neg")),
 486                    ["t", "goal_current", "current", "velocity", "position"], rows)
 487          results.append(dict(direction=sign, pose=pose, file=os.path.basename(out)))
 488          bus.sleep(0.5)
 489      return results
 490  
 491  
 492  def test_steps(arm, i, out):
 493      """Position-mode goal steps with SPOT's servo settings, logged as fast as the bus allows."""
 494      a, bus = arm.args, arm.bus
 495      c = arm.center[i]
 496      arm.goto(i, c)
 497      arm.mode(i, MODE_POSITION, **SPOT_POSITION)
 498      goals = [c + math.radians(d) for d in a.step_deg]
 499      rows = []
 500      t0 = bus.now()
 501      for g in goals:
 502          bus.write(i, "goal_position", round(g * PULSES + 2048))
 503          t_end = bus.now() + a.step_time
 504          while bus.now() < t_end:
 505              st = bus.state()
 506              arm.guard(i, st[i])
 507              rows.append([bus.now() - t0, i, g] + [st[k][4] for k in arm.ids_all] + [st[i][3], st[i][2], st[i][1]])
 508      write_csv(os.path.join(out, "steps_j%d.csv" % i),
 509                ["t", "joint", "goal"] + ["q%d" % k for k in arm.ids_all] + ["velocity", "current", "pwm"], rows)
 510      print("    %d samples over %.1f s" % (len(rows), rows[-1][0] if rows else 0))
 511  
 512  
 513  # ---------------------------------------------------------------------------- fits
 514  
 515  def fit_joint(out, i, kt):
 516      r = {"index": i, "id": i, "name": NAMES.get(i, "joint %d" % i)}
 517      path = os.path.join(out, "breakaway_j%d.csv" % i)
 518      static = None
 519      if os.path.exists(path):
 520          rows = read_csv(path)
 521          pos, neg = [], []
 522          for d in (1, -1):
 523              run = [x for x in rows if x["direction"] == d]
 524              # each ramp ends where the joint moved (or at the current cap, which doesn't count)
 525              first = 0
 526              for k, x in enumerate(run):
 527                  last = k == len(run) - 1 or run[k + 1]["t"] - x["t"] > 0.2
 528                  if last:
 529                      if abs(x["position"] - run[first]["position"]) > 0 or abs(x["velocity"]) > 0:
 530                          (pos if d > 0 else neg).append(kt * abs(x["current"]))
 531                      first = k + 1
 532          if pos or neg:
 533              r["static_pos"] = median(pos) if pos else None
 534              r["static_neg"] = median(neg) if neg else None
 535              static = median(pos + neg)
 536              r["static"] = static
 537              r["static_spread"] = (max(pos + neg) - min(pos + neg)) if len(pos + neg) > 1 else 0.0
 538      path = os.path.join(out, "sweep_points_j%d.json" % i)
 539      if os.path.exists(path):
 540          pts = [x for x in json.load(open(path)) if abs(x["velocity"]) >= 0.5 * abs(x["command"]) > 0]   # steady sliding only
 541          if len(pts) >= 3:
 542              v = [abs(p["velocity"]) for p in pts]
 543              tau = [kt * abs(p["current"]) for p in pts]
 544              best = None
 545              for k in range(60):   # Stribeck velocity on a log grid, the rest linear
 546                  vs = 0.003 * 10 ** (k / 60 * 2.5)
 547                  if static is not None:
 548                      rows = [[1 - math.exp(-(x / vs) ** 2), x] for x in v]
 549                      rhs = [t - static * math.exp(-(x / vs) ** 2) for x, t in zip(v, tau)]
 550                      try:
 551                          tc, b = lstsq(rows, rhs)
 552                      except ValueError:
 553                          continue
 554                      ts = static
 555                  else:
 556                      rows = [[1, math.exp(-(x / vs) ** 2), x] for x in v]
 557                      try:
 558                          tc, dt, b = lstsq(rows, tau)
 559                      except ValueError:
 560                          continue
 561                      ts = tc + dt
 562                  pred = [tc + (ts - tc) * math.exp(-(x / vs) ** 2) + b * x for x in v]
 563                  rms = math.sqrt(sum((p - t) ** 2 for p, t in zip(pred, tau)) / len(tau))
 564                  if tc > 0 and b >= 0 and ts >= tc and (best is None or rms < best[0]):
 565                      best = (rms, tc, ts, vs, b)
 566              if best:
 567                  r.update(coulomb=best[1], stribeck=best[3], viscous=best[4], sweep_rms=best[0])
 568                  if static is None:
 569                      r["static"] = best[2]
 570                  # Below the slowest steady speed the data can't tell where the drop from static to
 571                  # Coulomb friction happens (the joint stick-slips there): take half that speed
 572                  vmin = min(v)
 573                  if best[3] < 0.6 * vmin:
 574                      r["stribeck"] = 0.5 * vmin
 575                      r["stribeck_note"] = ("below the slowest steady speed measured (%.3f rad/s), where the joint stick-slips; "
 576                                            "set to half of it" % vmin)
 577      files = [f for f in os.listdir(out) if f.startswith("inertia_j%d_" % i)]
 578      Js = []
 579      for f in files:
 580          rows = read_csv(os.path.join(out, f))
 581          moving = [x for x in rows if abs(x["velocity"]) > 0]
 582          if len(moving) < 6 or r.get("coulomb") is None:
 583              continue
 584          t0 = moving[0]["t"]
 585          # position over time after it starts: q = q0 + w0 t + a t^2 / 2
 586          rows_fit = [[1, x["t"] - t0, 0.5 * (x["t"] - t0) ** 2] for x in moving]
 587          try:
 588              _, _, acc = lstsq(rows_fit, [x["position"] for x in moving])
 589          except ValueError:
 590              continue
 591          torque = kt * median([abs(x["current"]) for x in moving])
 592          vmean = median([abs(x["velocity"]) for x in moving])
 593          net = torque - r["coulomb"] - r.get("viscous", 0) * vmean
 594          if abs(acc) > 1e-3 and net > 0:
 595              Js.append(net / abs(acc))
 596      if Js:
 597          r["inertia"] = median(Js)
 598          meta = os.path.join(out, "inertia_pose_j%d.json" % i)
 599          if os.path.exists(meta):
 600              r["pose"] = json.load(open(meta))
 601      return r
 602  
 603  
 604  def fit_all(out, joints, kt, extra):
 605      result = {"format": "spot-arm-calibration", "version": 1,
 606                "created": datetime.datetime.now().isoformat(timespec="seconds"), "folder": os.path.abspath(out),
 607                "kt": kt, "joints": []}
 608      result.update(extra)
 609      for i in joints:
 610          r = fit_joint(out, i, kt[str(i)] if isinstance(kt, dict) else kt)
 611          result["joints"].append(r)
 612          def f(x, n="%.3f"):
 613              return n % x if isinstance(x, (int, float)) else "-"
 614          print("  joint %d (%s): static %s N m (+%s/-%s), coulomb %s N m, stribeck %s rad/s, viscous %s N m s/rad, inertia %s kg m^2"
 615                % (i, r["name"], f(r.get("static")), f(r.get("static_pos")), f(r.get("static_neg")), f(r.get("coulomb")),
 616                   f(r.get("stribeck")), f(r.get("viscous")), f(r.get("inertia"), "%.4g")))
 617      path = os.path.join(out, "arm_calibration.json")
 618      with open(path, "w") as fh:
 619          json.dump(result, fh, indent=2)
 620      print("Wrote %s\nCopy it to the SPOT project's resources folder; the next simulation uses it." % path)
 621      return result
 622  
 623  
 624  # ---------------------------------------------------------------------------- main
 625  
 626  def main():
 627      p = argparse.ArgumentParser(description=__doc__.split("\n")[0], formatter_class=argparse.RawDescriptionHelpFormatter,
 628                                  epilog=__doc__.split("\n", 1)[1])
 629      p.add_argument("--port", default="/dev/ttyUSB0")
 630      p.add_argument("--baud", type=int, default=1000000)
 631      p.add_argument("--joints", type=int, nargs="+", default=[1, 2, 3], help="servo IDs to calibrate (1 shoulder, 2 elbow, 3 wrist)")
 632      p.add_argument("--all-ids", type=int, nargs="+", default=[1, 2, 3], help="every servo on the bus (the others hold)")
 633      p.add_argument("--tests", nargs="+", default=["breakaway", "sweep", "inertia", "steps"],
 634                     choices=["breakaway", "sweep", "inertia", "steps"])
 635      p.add_argument("--out", default="runs", help="folder for the run's data (a timestamped folder inside)")
 636      p.add_argument("--fit", metavar="RUN_FOLDER", help="only fit the data in RUN_FOLDER again")
 637      p.add_argument("--kt", type=float, help="torque constant at the joint [N m/A] (default: from the servo model)")
 638      p.add_argument("--window", type=float, default=25.0, help="each joint moves within +-WINDOW deg of its start")
 639      p.add_argument("--ramp", type=float, default=0.03, help="breakaway current ramp [A/s]")
 640      p.add_argument("--repeats", type=int, default=3, help="breakaway ramps per direction")
 641      p.add_argument("--move-deg", type=float, default=0.5, help="motion that counts as breakaway [deg]")
 642      p.add_argument("--max-current", type=int, default=450, help="current cap in tests [3.36 mA units]")
 643      p.add_argument("--speeds", type=int, nargs="+", default=[1, 2, 3, 5, 8, 12, 18, 25, 35],
 644                     help="sweep speeds [0.229 rev/min units]")
 645      p.add_argument("--pass-time", type=float, default=25.0, help="longest sweep pass [s]")
 646      p.add_argument("--inertia-torque", type=float, default=2.0, help="torque step, as a multiple of the breakaway torque")
 647      p.add_argument("--inertia-deg", type=float, default=15.0, help="stop the torque step after this motion [deg]")
 648      p.add_argument("--step-deg", type=float, nargs="+", default=[10, 0, -10, 20, 0, -20, 5, 0], help="position steps [deg]")
 649      p.add_argument("--step-time", type=float, default=2.5, help="time at each position step [s]")
 650      p.add_argument("--simulate", action="store_true", help="a simulated arm instead of the servos")
 651      p.add_argument("--yes", action="store_true", help="start without asking (Enter); for the SPOT Console")
 652      a = p.parse_args()
 653  
 654      if a.fit:
 655          info = json.load(open(os.path.join(a.fit, "run.json")))
 656          args = info.get("args", {})
 657          fit_all(a.fit, info["joints"], info["kt"], {"servos": info.get("servos"), "supply_voltage": info.get("supply_voltage"),
 658                                                      "simulated": bool(args.get("simulate")),
 659                                                      "tests": [t for t in ("breakaway", "sweep", "inertia", "steps")
 660                                                                if t in args.get("tests", ())]})
 661          return
 662  
 663      # SIGTERM stops it (the SPOT Console's agent starts it from a shell that ignores SIGTERM, which it would
 664      # inherit): until the servos are set up, at once; afterwards through on_signal below, which restores them
 665      signal.signal(signal.SIGTERM, signal.SIG_DFL)
 666      out = os.path.join(a.out, datetime.datetime.now().strftime("%Y%m%d_%H%M%S"))
 667      os.makedirs(out, exist_ok=True)
 668      bus = SimBus(a.all_ids) if a.simulate else DxlBus(a.port, a.baud, a.all_ids)
 669      arm = Arm(bus, a)
 670      arm.ids_all = a.all_ids
 671      stop = {"now": False}
 672  
 673      def on_signal(*_):
 674          stop["now"] = True
 675          raise KeyboardInterrupt
 676      signal.signal(signal.SIGTERM, on_signal)
 677  
 678      try:
 679          models = arm.setup()
 680          voltage = bus.read(a.all_ids[0], "present_input_voltage") * 0.1
 681          kt = {}
 682          for i in a.joints:
 683              if models[i] == 30:
 684                  sys.exit("ID %d is an MX-28: it doesn't measure current, so it can't be calibrated this way." % i)
 685              kt[str(i)] = a.kt or KT.get(models[i])
 686              if kt[str(i)] is None:
 687                  sys.exit("No torque constant known for ID %d (%s): give one with --kt." % (i, MODELS.get(models[i], models[i])))
 688          print("Servos: %s; supply %.1f V; data in %s" % (", ".join("ID %d %s" % (i, MODELS.get(m, m)) for i, m in models.items()),
 689                                                         voltage, out))
 690          if a.simulate:
 691              print("Simulated arm, true values: %s" % json.dumps(bus.truth))
 692          else:
 693              print("The platform must not float, and each joint will move up to %.0f deg from where it is. Ctrl+C stops." % a.window)
 694              if not a.yes:
 695                  input("Press Enter to start... ")
 696          json.dump({"joints": a.joints, "kt": kt, "servos": {str(i): MODELS.get(m, m) for i, m in models.items()},
 697                     "supply_voltage": voltage, "args": vars(a)}, open(os.path.join(out, "run.json"), "w"), indent=2)
 698          for i in a.all_ids:
 699              arm.center[i] = arm.hold(i)
 700          order = [t for t in ("breakaway", "sweep", "inertia", "steps") if t in a.tests]
 701          progress = {"index": 0, "total": len(a.joints) * len(order)}
 702  
 703          def announce(joint, test):
 704              progress["index"] += 1
 705              print("PROGRESS %s" % json.dumps({"joint": joint, "test": test, "index": progress["index"],
 706                                                "total": progress["total"]}))
 707              sys.stdout.flush()
 708          for i in a.joints:
 709              name = NAMES.get(i, "joint %d" % i)
 710              print("Joint %d (%s), from %.1f deg:" % (i, name, math.degrees(arm.center[i])))
 711              if "breakaway" in a.tests:
 712                  announce(i, "breakaway")
 713                  print("  breakaway (current ramps)")
 714                  test_breakaway(arm, i, out, kt[str(i)])
 715              static = fit_joint(out, i, kt[str(i)]).get("static", 0.3)
 716              if "sweep" in a.tests:
 717                  announce(i, "sweep")
 718                  print("  friction against speed (velocity mode)")
 719                  pts = test_sweep(arm, i, out)
 720                  json.dump(pts, open(os.path.join(out, "sweep_points_j%d.json" % i), "w"), indent=2)
 721              if "inertia" in a.tests:
 722                  announce(i, "inertia")
 723                  print("  inertia (torque steps)")
 724                  res = test_inertia(arm, i, out, kt[str(i)], static)
 725                  json.dump(res[0]["pose"], open(os.path.join(out, "inertia_pose_j%d.json" % i), "w"))
 726              if "steps" in a.tests:
 727                  announce(i, "steps")
 728                  print("  position steps (SPOT's servo settings)")
 729                  test_steps(arm, i, out)
 730              arm.goto(i, arm.center[i])
 731              arm.hold(i)
 732              for k in a.all_ids:
 733                  temp = bus.read(k, "present_temperature")
 734                  if temp > 65:
 735                      raise Abort("ID %d is at %d C: let it cool down" % (k, temp))
 736          print("Fitting:")
 737          fit_all(out, a.joints, kt, {"servos": {str(i): MODELS.get(m, m) for i, m in models.items()}, "supply_voltage": voltage,
 738                                      "simulated": bool(a.simulate), "tests": order})
 739          ok = True
 740      except KeyboardInterrupt:
 741          print("\nStopped: torque off.")
 742          ok = False
 743      except Abort as e:
 744          print("\nStopped: %s. Torque off." % e)
 745          ok = False
 746      finally:
 747          arm.restore()
 748          bus.close()
 749      if not ok:
 750          sys.exit(1)
 751  
 752  
 753  if __name__ == "__main__":
 754      main()
```

</details>
<!-- /listing -->
