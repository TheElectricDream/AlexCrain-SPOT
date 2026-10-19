# SPOT (template): status as of 2026-10-15 (evening)

**Keeping this file short:**
- Edit the sections at the top so they describe the current state. Don't add notes that a later entry overrides.
- Change-log entries are **at most ~10 lines**: what was asked, what changed (files), how it was checked, and what's left. Put details in the code's help text or the tutorial.
- When an item is done, remove it from *Open items* and give it one line in the change log.
- The full history is in `../archive/Legacy/`: `STATUS_2026-10-06_to_10-09.md` and `STATUS_2026-10-10.md` (design notes, test details, every request). This file is gitignored.

## System

```
PhaseSpace server 192.168.1.109 (TCP 8000)
   │ OWL C++ API (libowl-src-410)
   ▼
TX2 "lab console" 192.168.1.116 ── SPOT Hardware Interfaces, :8080 (systemd user service)
   StreamData ── UDP 31535 → 31534 ──► platforms .110 RED  .111 BLACK  .112 BLUE, .104, .115 Orin, + added IPs
   SSH (restricted key → ~/.spot/agent.sh) ──► pucks/thrusters (Teensy), deploy/start/stop/fetch experiments
   ▲ HTTP + events
Your computer: `SPOT` in MATLAB ── SPOT Local Simulations, 127.0.0.1:<port> (local server + MATLAB worker)
   Simulation / Experiment / Results tabs; builds on the TX2 (slbuild, board = TX2); relays the lab console
```

UI names: RED = **Chaser** (silver), BLACK = **Target** (gold), BLUE = **Obstacle** (blue). Code, data, settings and the API still use RED/BLACK/BLUE/ARM.

## Where things are

| Path | What |
|---|---|
| `SPOT.m` | Launcher: `SPOT`, `SPOT open`, `SPOT stop` |
| `+spot/+<pkg>` | All MATLAB logic (`help spot`): `params`, `massprops`, `control`, `sim`, `experiment`, `data`, `plotting`, `workspace`, `worker`, `console`, `hardware`, `arm` |
| `+spot/+arm` | The simulated arm and its controllers: `Plant` (the diagram's RED + ARM Plant block: base + arm, servos in PWM control (PWM in, applied PWM out; torque g(V − Ke w)), stick-slip friction, 1 ms steps inside each `baseRate` step), `PositionControl`/`VelocityControl` (the servos' own laws giving torques: Default Position/Velocity Control (ARM)), `withinLimits`, `defaults`/`parameters` (`armModel`), `loadCalibration`, `compareSteps`, `servoFromDiagram`, `mountGeometry` (Phi/b0/shoulder offset for `s.mass.arm.mount`), `checkCommands` (ignored goals, limit and current-limit torques → simulation warnings) |
| `+spot/+hardware` | What runs in the diagram/on the platforms: System objects, `ArmTorque` (the arm: torques in; angles, speeds, torques, bus status out; `csrc/arm/src/arm_torque.cpp`, no SDK), `udpSend`/`udpReceive`/`phasespaceLatest`, `experimentTime`, `debugMessages`, `liveStream`; C in `csrc/` (`arm`, `udp`, `data_logger`, `debugging`, `identify_platform`); `teensy/` (firmware, `setup_teensy_serial.sh`); `arm/` (`setup_arm_serial.sh`: RED's U2D2 latency timer). The old arm blocks, the Dynamixel SDK and the camera scripts are in `../archive/` |
| `+spot/+console/webapp` | Server (`app.py` + `spot_console/*.py`, **Python 3.6**, stdlib only) and page (`static/*.js`, three.js vendored, CAD in `static/models/`). Calibration tab: `calibration.py` (file store, the table, thruster and arm runs), `calfit.py` (fits), `static/calibration.js`, `calviz.js` |
| `+spot/+console/phasespace` | `StreamData.cc`, `Makefile`, `build_linux.sh`, `libowl-src-410/` |
| `+spot/+console/deploy` | `deploy_tx2.sh`, `setup_hardware_link.sh`, `spot_agent.sh`, the service file. Run them as `./+spot/...` (bash reads a leading `+` as an option) |
| `Template_v5_0_0_2026a_Jetson_PSClock.slx` | **The only diagram.** Edit it directly. New projects get it renamed to the project's name |
| `SPOT_User_Parameters.m`, `spot_settings.json`, `spot_design.json` | The user's extra variables (no longer `PWMFreq`/`phaseSpaceServer`: those are settings now); settings (saved on every change; `pwmFrequency`, `labConsoleAddress` added 2026-10-11); the Spacecraft Designer's design (created at the first change; the lab console keeps its own in `~/.spot_design.json`) |
| `documentation/` | User guide `SPOT_Tutorial.tex/.pdf` and its `figures/` (regenerate the appendices with `python3 documentation/update_tutorial_reference.py`, then `latexmk -pdf SPOT_Tutorial.tex` in `documentation/`); `demo_execution_order.m` (the tutorial's demo); `wiki/` = the GitHub wiki (181 pages: one per `spot.*` function, console module, hardware program, diagram subsystem and console tab, with `images/`; `python3 documentation/wiki/build_wiki.py --strict` regenerates `_Sidebar.md`, the code listings and the used-by/uses lists). Gitignored; new projects get only the PDF |
| `resources/` | Only what runs: `tracker_positions_phasespace.txt`, `arm_calibration/calibrate_arm.py` (lab friction calibration, Dynamixel SDK; `--simulate` to rehearse) → `resources/arm_calibration.json`, the four pictures the console and help refer to (`COMConfig`, `ThrusterConfig`, `ArmConfig`, `SRL_logo`), and `spot_calibration.json` (the project's copy of the lab's calibration, from the Calibration tab; `spot.params.loadCalibration`) |
| `../New_SPOT_Project.m` | Creates a project from `template/` in `../projects/<name>/` (MATLAB function, since 2026-10-12; the Python version and its `.bat` are in `../archive/`) |
| `../archive/` | Everything a user doesn't need (gitignored; see its README): `Legacy/` (retired code, `.slx` backups, old STATUS logs), `Pictures/`, `Resources_unused/`, `Manuals/`, `Saved Data/` (the test runs), `camera/`, `old_arm_blocks/` (+ the Dynamixel SDK), `New_SPOT_Project.py/.bat`, `Template_Files_Updated/` (an old full copy), the June patch notes |
| `../designs/` | The lab's SolidWorks CAD (207 MB, gitignored). **Use it for any CAD work.** `cad_tools/`: `build_cad_models.py` (this folder → `static/models/spot_cad.bin.gz` and `spot_table.bin.gz`; `python3 designs/cad_tools/build_cad_models.py`), `step_mesh.py` |
| `../changes/` | `patch-notes.tex`: the release notes (5.0.0 Pre-Release.2) |

**TX2:** `ssh -i ~/.ssh/id_ed25519_spot spot-tx2@192.168.1.116`. It runs Ubuntu 18.04, Python 3.6 and g++ 7.5, and `sudo` needs a password. Install: `~/PhaseSpace_Server_SendAll/` (written by `deploy_tx2.sh`; don't edit there). Service: `systemctl --user status|restart spot-phasespace-console`, logs with `journalctl --user -u spot-phasespace-console -f`. Settings: `~/.spot_phasespace_console.json`. Experiment builds: `~/ExperimentSoftware/MATLAB_ws/R2026a/…`, data: `~/ExperimentData/<run>/`.

**Platforms:** deployments go in `~/ExperimentSoftware/spot/<model>/` (with `run.log`). The program runs as the platform user, not as root.

## Ports and packets

| What | Where |
|---|---|
| PhaseSpace packet | 115 × float32 LE (460 B), 0-based: `[0]` time; `[1..9]` x, y (mm), att (rad) for RED/BLACK/BLUE; `[10..18]` rates; `[19+3k..21+3k]` LED k xyz (NaN = unseen); `[112]` experiment clock (LED 31's x: NaN = not armed, < 0 = countdown) |
| StreamData → platforms / viewer | UDP 31535 → 31534 / 31540 |
| Experiment clocks (platform → TX2) | UDP 48720–48722 |
| Debug messages (platform → TX2) | UDP 55001–55003 (`[t double, text]`); 55000 = the old diagram's bare text |
| Live streams (platform → TX2) | UDP 55011–55013 |
| Diagram, other | Check Connection broadcasts on 33001–33003 (nothing in the console listens); to the Orin 192.168.1.115:46875; from the Orin 36845 |
| Teensy | `0xAA` + 8 duty cycles (0–100) + pucks, float32 LE, 37 B, on `/dev/ttyACM0`. **2 s watchdog** |

## Lab console API (`http://192.168.1.116:8080`, JSON POSTs)

- `GET/POST /api/stream`, `/api/stream/start?frequency=N`, `/stop`; `/api/destinations/add|remove?ip=X|self`; `/api/tracker_positions`
- `GET /api/state`, `GET /api/events` (server-sent events)
- `/api/hardware`, `/api/hardware/pucks|thruster|off|check`
- `/api/experiment/deploy|start|stop|fetch|download|build|live` (one experiment at a time; Start needs PhaseSpace streaming, waits for every platform's clock, then `exp start 3`)
- `/api/interactive`, `/api/interactive/start|goal|heartbeat|stop` (several platforms at once; goals at least 0.45 m apart)
- `GET/POST /api/design` (`{design}`, checked by `design.py`; both consoles; a `design` event follows each change)
- `GET /api/calibration` (both; the lab's `~/.spot_calibration.json`, raw runs in `~/SPOTCalibration/`); lab: `POST /api/calibration/table/start|thrusters/start|arm/start|stop|clear`, `GET /api/calibration/arm/check` (`calibration_run` events; thrusters and a real arm run need `acknowledged`; thrusters take `assumed` mass/inertia/CG, arm `joints`, `tests`, `simulate`); local: `/api/calibration/take|sync`
- The local console forwards a fixed list of these through `POST /api/lab` (including `design`) and refuses them all when the TX2 can't be reached.

## Things to know

- **Editing `.slx` files from MATLAB:** put the project on the path first. Saving without it dropped the `MaskType` of the `spot.hardware` System-object blocks once. Keep a backup in `../archive/Legacy/slx_before_<change>/` and diff the XML afterwards. Ask the user to `bdclose` the diagram (without saving) if their MATLAB has it loaded.
- **Execution order:** the top-level subsystems are atomic, with priorities (tutorial section 6). A new top-level subsystem needs a priority that falls between its writers and its readers. The "read before write" check is turned off only on stores that have a reason in their description.
- **`spot.control.verifyConsoleController`:** run it after any change to the diagram's LQR or mixer, to `control.py`, or to the defaults. Interactive mode uses `control.py`'s `DEFAULTS`, not `spot_settings.json`.
- **The TX2's server runs Python 3.6:** no f-strings, and `subprocess.run` doesn't accept `input=` together with `stdin=`. Check with pyflakes and a 3.6 parse.
- **Simulink and the worker:** socket callbacks don't run during a simulation, so Stop goes through `.spot_cache/stop_request` and a timer. Timers wait while Simulink compiles (~25 s).
- **Hardware facts:** RED's 4th LED reports as ID 0 and BLACK's as 13 (`StreamData.cc` ~l.300, `rigid_led_ids`). PhaseSpace accepts only marker IDs 0–23. The boards have no clock battery, so their clock reads 1999 until NTP sets it.
- **UI text style:** every label that isn't a sentence uses title case: buttons, tabs, headings, toggles, pills, badges, column headers, menu items, dialog titles, name tags ("Live Plot", "Fly to Goals", "Chaser · Sim"). Short words (a, the, and, of, to, in, on, for…) stay lowercase unless they come first. Sentences (hints, tooltips, toasts, log lines, status lines) stay in sentence case. The tutorial's headings and its `\ui{}` references follow the page.
- **The arm:** the servos stay in PWM (voltage) control (mode 16; current control until 2026-10-15: the current readings are useless at the arm's torques); `ARM_Controls` are joint PWM [885 = 100 %], at rest ≈ 0.0068 N m per unit; the back-EMF (g Ke ≈ 0.9 N m s/rad) damps the joints. `ARM_Control_Law_Enabler`: 1 Default Position, 2 Custom (torque example: the path as torques), 3 Default Velocity (the path as rad/s), else off; `ARM_Control_Mode` logs which. The controllers run at 20 Hz with retuned gains (~1 N m/rad; the servo's 400/0/200 is unstable at 50 ms). Joint angles are the servos' own (0 straight out, the shoulder out from its face; ±90° = 1024–3072 pulses): position goals beyond are ignored, and the block and plant send no torque pushing past them. `ArmTorque` (in Robotic Arm Code, priority 21) reads the encoders in Outputs and sends the torques in Update, its Torque input wired from Arm Torques (After Control) (priority 61: a Data Store Read of `ARM_Controls`), so a torque goes out in the step it was computed, as in simulation: keep its input non-direct-feedthrough and that read after 4 - Control. In simulation the arm's measured states come from the plant every step (encoder/velocity units). The controller blocks take `armMotor` (set by `spot.params.apply`), not `armModel`: blocks sharing `armModel` make it a tunable struct, and its `friction.static` is a C++ keyword. `Phi`/`b0` are the CAD's shoulder (motor flush and centred on the coupler) for the usual mount (+y, left); `s.mass.arm.mount` moves it (`spot.arm.mountGeometry`).
- **An open console page owns the settings:** it sends the whole settings on every change, so a page opened before `spot_settings.json` was edited by hand (or by a script) writes the old values back. That undid the 2026-10-10 `Phi`/`b0` fix on 2026-10-11 14:10 (the arm's first link then sat beside its shoulder motor). Reload open pages after such an edit.
- **The lab console's address** is the setting `labConsoleAddress` (Experiment tab, Lab): the local server's lab link (`/api/lab/address`; at start from the settings file), `buildBoard`, and the diagram's `labConsoleIP` parameter (8 MATLAB Function blocks: PhaseSpace receive, the 3 clock senders, debug messages, live streams; `spot.hardware.ipText` turns it into text). A new Live Stream block copied from an old one without the parameter still sends to 192.168.1.116. `deploy_tx2.sh`/`setup_hardware_link.sh` still take the TX2 from their `TX2` variable.
- **Designer and settings (local console):** the Chaser's Arm in the Designer and the Simulation tab's Arm platform are one choice (`design.py` type `arm`, `simulation.setArm`, the page's `spot-settings` event); the settings win at startup.
- **The mixer takes one thrust per thruster:** the diagram's MATLAB Function5 (×3), its copy `spot.control.thrusterDutyCycles(u, theta, dist, F)` and `control.py` read `F_thrusters_<P>` (8×1; the calibrated thrust or `thrusterForce` for all eight; `F_<p>_X/Y_nominal` are no longer read by the diagram). Change all three together and run `verifyConsoleController`.
- **MATLAB syntax in long `struct(...)` calls:** a comment-only line inside a `...` continuation is an error (it broke `spot.params.defaults` once); keep comments at line ends.
- **Testing in this environment:** MATLAB R2026a runs in the `matlab-ubuntu` distrobox. Use `-batch` on a scratch copy of the project, never the user's open session. Page tests run in headless Firefox or Chromium.

## Status

**Validated in the lab (2026-10-09, confirmed by the user):** all current functions work as expected, from the lab console (PhaseSpace, Hardware, experiments, Interactive mode) to the local console (simulation, build on the TX2, deploy, agreed start, collect, results). RED's corrupted `~/MATLAB` is fixed. `setup_teensy_serial.sh` has been run on the platforms.

**Never tried:** Windows (the `py -3`/`start /b` launch in `spot.console`, `installPython`).

**Not yet tried in the lab:** the Calibration tab's table slope, thruster and arm runs (2026-10-13; need `deploy_tx2.sh`, and for the arm `setup_hardware_link.sh` plus RED's one-time setup; rehearsed against a simulated table: an injected slope recovered exactly, injected thrusts to 0.4 %; the arm with the script's simulated servos); the per-thruster mixer on the platforms (rebuild; code generation checked); a build with the `labConsoleIP` parameter (clocks, messages, live streams and PhaseSpace through it); the lab console with the 2026-10-10 console changes (it needs `deploy_tx2.sh`: Designer Arm choice, `design.py`, the CAD models); the new arm block on RED (`setup_arm_serial.sh` first). Simulation side checked in batch MATLAB and headless Chromium (stand-in MATLAB worker); the arm block's C++ against a stand-in Dynamixel bus.

## Open items

**Bugs / security**
1. `../archive/camera/run_ssh.sh` (out of the repo since 2026-10-11) has a plain-text SSH password (spot-vision). Change that password.
2. The TX2 password is in the git history of `Template_Files/GUI_v5_0_Main.mlapp`. Change the password.
3. The lab console has no login: anyone on the lab network can drive the pucks and thrusters.

**To do**
4. Turn off Wi-Fi power save on the platforms (**not run yet**): `nmcli connection modify <wifi> 802-11-wireless.powersave 2`. On a Wi-Fi laptop: `sudo iw dev wlp2s0 set power_save off`.
5. Rename the TX2 (`sudo hostnamectl set-hostname spot-tx2`); `ubuntu.local` resolves to .115 at the moment.
6. The repo is the top folder (`SPOT-AlexanderCrain-V5PR2`, not a git repository yet): `New_SPOT_Project.m`, `template/`, `projects/` (only `.gitkeep`), `changes/`. Its `.gitignore` leaves out `archive/`, `designs/`, `projects/*`, `template/documentation/`, `template/STATUS.md`, caches, Simulink builds and `Saved Data/`. To publish the wiki, push `template/documentation/wiki/` to `<repo>.wiki.git`.
7. Minor: two diagram annotations still mention a "PD" controller, two others `Resources/` (now `resources/`); the table could use the CAD folder's granite photo (`99 - Textures/granite.png`, scaled down); `data.list` once took ~2 min in batch MATLAB (watch for it); the Data Store Memory blocks of `ARM_Measured_Torque` and `ARM_Servo_Status` carry `ARM_Controls`'s description text (their checks are off too).
8. **Spacecraft Designer, first version:** to refine with the user. Not done yet:
   - attachments in the cube view (Use CAD off);
   - the design in MATLAB (2D figures, mass properties);
   - the Experiment tab draws the project's design, not the lab console's;
9. Not ported from the old GUI: Empty MATLAB_ws/ExperimentSoftware, the vision computer, deploying to one platform at a time, video export, custom drawings.
10. Two System-object mask XMLs are stale (`+spot/+arm/PositionControl_mask.xml` since 10-12, `Plant_mask.xml` since the FloatInput change): MATLAB warns at load and ignores them (blocks keep their saved icons). Fix in the user's MATLAB: open each block's Mask Editor, Save Mask (no programmatic way found).

**The arm** (opened by the user 2026-10-10)
- Torque control throughout (2026-10-11): plant at 1 ms inside each step (servos in current control, stick-slip friction), the servos' position/velocity laws as 20 Hz controllers, `ArmTorque` on the hardware; drawn from CAD (`ARM_MOUNT`, links, `ARM_EE` = male cone), mountable at any Designer place.
- **Lab test of `ArmTorque`** (RED, after `setup_arm_serial.sh`): logged `ARM_Servos_Read` should be 3 every step, `ARM_Servo_Read_ms` ~3, `ARM_USB_Latency_ms` 1; `ARM_Servo_Reads_Missed` says whether the slow-encoder diagnosis (FTDI latency + SDK timeout) was right. Then a small torque (Custom, enabler 2) and position control (enabler 1), arm clear of the table.
- **The user's Sub-Phase 1** writes `ARM_Control_Law_Enabler` 3: that was "off", it is now velocity control, with the path's angles as speeds (the elbow sits at its +90° limit, the rest holds still; one simulation warning). Set 0 if off was meant.
- **Next:** run the arm calibration from the Calibration tab (Rehearse first), then Update from Lab in the project (friction placeholders until then); `spot.arm.compareSteps` on its position steps checks the plant and the controller replica together. To check there: the servos' control period (1 ms), the rotor inertia (3.5e-3 kg m²); then retune the 20 Hz gains on the real friction.

## Change log

### 2026-10-15 20:00 EDT: the audit game (`../audit/`)
- **Asked:** a game, in the console's style, for going through every chunk of code and documentation written here: accept each as it is or rewrite it in the user's words, flags to come back to, a log, checklists and achievements; rewrites land in the final files without touching `template/`.
- **Done:** `audit/app.py` (stdlib server, port 8090) + `audit_game/` (`chunker.py`: [comment] -> [code] chunks per language, files rejoin byte for byte; a rewrite's code checked against the original with comments stripped; `catalog.py`: a snapshot in `audit/original/`, chapters documentation / MATLAB / Python / JavaScript and page / shell / Arduino and C/C++ / configuration, sections per package or folder, guides in the toc's order; `progress.py`: `audit/progress.json`, XP and levels, per-section and per-chapter achievements, `audit/work/` with the rewritten files whole, Apply with a backup). Page: `static/index.html`, `audit.css`, `audit.js`, using the console's CodeMirror and marked bundles from `/vendor`. 278 files, ~4100 chunks.
- **Checked:** every file reassembles from its chunks; the API end to end (accept, rewrite + check, flag, resolve, skip, reset, export, diff); the code check per language. The page was not opened in a browser here (no browser): first thing to try.
- **Not covered:** `static/vendor`, `libowl-src-410`, `spot_calibration.json`, images, binaries. `.gitignore`: `audit/original|work|backup/`.

### 2026-10-15 17:30 EDT: arm in PWM control; resources/ gone; marker file from the calibration file; New_SPOT_Project to the standard
- **PWM control (the user's call: ARM_Controls in servo PWM units):** `spot.hardware.ArmTorque` → `ArmPWM` (`arm_torque.cpp/.h` → `arm_pwm.*`: mode 16, PWM Limit, Goal PWM 100, read from Present PWM 124); `PositionControl`/`VelocityControl` output their PWM (no `Motor`/`BackEMF`; `armMotor` gone); `Plant` takes PWM, motor `torquePerVolt` g and `backEMF` Ke (implicit with viscous), outputs applied PWM; `checkCommands` (PWM Limit), `servoFromDiagram`, `compareSteps`, `withinLimits`; `armModel.motor` = {voltage, torquePerVolt, backEMF}. Gains unchanged (a single-joint replica: stable; the elbow slower, back-EMF damping).
- **Diagram** (backup `archive/Legacy/slx_before_arm_pwm/`, edited as XML, not yet opened in Simulink): Dynamixel Arm (PWM), Arm PWM (After Control), `ARM_Measured_PWM` (store, plant, logger: `ARM_<Joint>_PWM`), Calculate PWM (+ Stateflow paths), controller blocks without Motor/BackEMF, notes.
- **Calibration:** `calibrate_arm.py` rewritten to the standard, PWM only (breakaway ramp at first 2-pulse motion; sweep at PWM levels about breakaway with a kick; inertia by simulating the step with the fitted friction; steps); file version 2 `"control": "pwm"`, per joint `back_emf`, `torque_per_volt`; `--simulate` recovers the model (inertia within ~2 %, wrist rough and flagged). Version-1 (current) calibrations: refused by `spot.arm.loadCalibration`, skipped with a warning by `parameters`/`apply`, not offered or taken by Update from Lab, marked on the lab page; removed from the template's `spot_calibration.json`. `back_emf` overridable (Calibration pane).
- **Earlier today:** `resources/` removed (pictures in `webapp/docs/images`, logo `webapp/static/srl_logo_full.png`); LED positions in `spot_calibration.json` `markers`, edited in the Platforms pane's Thruster and LED Layout window (`layout.js`); `apply` puts `trackerPositions` in the derived values and Start Cameras sends it (no local file; `writeTrackerPositions` gone). `New_SPOT_Project.m` brought to CODE_TEMPLATE (behaviour unchanged). The user deleted the top-level `documentation/`: only `webapp/docs` is maintained.
- **Not checked here (no MATLAB):** the diagram opening and simulating, codegen of `ArmPWM`, the MATLAB changes running.

### 2026-10-13 21:00 EDT: Calibration tab (phase C): arm
- **Asked:** run the arm calibration from the tab, with checks that the Chaser has what it needs; the user tests it in the lab on 10-14.
- **Agent** (`spot_agent.sh`): `arm-cal-check` (Python 3, Dynamixel SDK, U2D2 at `/dev/ttyUSB0`, access, nothing holding it, agent version), `arm-cal-receive|start|status|stop|data` (run detached with `setsid`; stop: TERM, KILL after 10 s). `deploy_tx2.sh` copies `calibrate_arm.py`; the tab sends it before each run.
- **`calibrate_arm.py`:** `--yes`, `PROGRESS {json}` lines, exit 1 when stopped, SIGTERM reset to default at start.
- **Tab** (`ArmRun`): checks card, joints/tests chips, confirmation dialog, **Rehearse (Simulated Servos)** (never saved), progress with an animated arm, results (friction table, friction vs speed, position steps). Holds RED resting (`hardware.reserve`); Stop/Esc/Hardware off stop the script. Raw data `~/SPOTCalibration/<stamp>_arm_RED/`; result in the file's `arm` section.
- **MATLAB:** `spot.arm.parameters`/`loadCalibration` use the newer of that section and `resources/arm_calibration.json` (Scenario **Calibrated Arm**).
- **Checked:** rig: rehearsal end to end, Stop, missing-item reports, real path with a stand-in SDK/device (RED reserved and released; take-back stops the script in ~3 s); batch MATLAB precedence; headless Chromium, no script errors. Tutorial 4.5, wiki.

### 2026-10-13 17:00 EDT: Calibration tab (phase B): thrusters; map fits the window
- **Asked:** the local table-slope map was too big (scroll needed): it is now sized by the window (`fitMap`), the sync card moved to the left column; then phase B.
- **Thruster run** (`calibration.py` `ThrusterRun`): needs the user's confirmation of mass/inertia/CG (dialog, editable; defaults = `control.DEFAULTS`, CG worked back from the moment arms); per thruster and repeat: float 0.5 s, fire 100 % for the burn (2 s), fly back, pucks off, rest. Fit `calfit.fit_burn` (table pull removed); burns with jumps, residual > 3 mm or implausible thrust left out (nominal kept, marked). Results: arrows on the platform, bars ± spread vs nominal, turned angle, torque ratio, burn paths.
- **Mixer per thruster** (diagram backup `archive/Legacy/slx_before_thruster_cal/`, script `archive/Legacy/tools/use_thruster_thrusts.m`): MATLAB Function5 reads `F_thrusters_<P>`; copy, Python port and verifier updated; `apply` uses the project's calibrated thrust (`s.calibration.thrusters`, Scenario **Calibrated Thrust**, Platforms note); Interactive Mode and calibration flights use the lab's.
- **Checked:** rig: 8 thrusts within 0.8 % (one 2 s burn) and 0.4 % (two), angles < 0.5°, torque ratios 0.99–1.02; acknowledgement enforced; verifier all OK with unequal thrusts (diagram duty cycles = copy exactly, Python 1e-15); full run 0 warnings; Jetson code generation OK; headless Chromium both consoles, no script errors.

### 2026-10-13 15:30 EDT: Calibration tab (phase A): table slope, calibration file and sync
- **Lab console:** Calibration tab (Table Slope · Thrusters · Arm). Table slope run (`calibration.py`): rest 2 s, float, fly a grid with the diagram's controller (ramped goal; `fly_to`), drift with thrusters off; pucks off at the edge margin / near platforms / > 0.15 m/s; fit (`calfit.py`: quadratic height surface + drag per platform, position-based least squares; drag < 0 → fixed 0). Not saved if frames jump/go back in time (> 0.5 %) or residual > 20 mm; residual > 5× noise kept with a warning. Owns the platform like Interactive Mode (`Run.label`); excludes experiments/Interactive both ways. Jet map, live run view, 3D overlay (Table Slope toggle).
- **Sync:** lab `~/.spot_calibration.json` → project `resources/spot_calibration.json` (local Calibration tab: Update from Lab; prompt when the lab's is newer: Update / Not Now / Never Ask Again, kept in the file's `sync`).
- **Simulation:** `spot.params.loadCalibration`, `spot.sim.tableSlope`, `s.calibration.tableSlope` (Scenario switch). Diagram (backup `archive/Legacy/slx_before_table_slope/`, script `archive/Legacy/tools/add_table_slope.m`): each plant's MATLAB Function gets previous position/velocity (Unit Delays) and `Float_State`, adds the pull only while floating; `spot.arm.Plant` `FloatInput`; `Float_State` RBW/WAR checks off (reason in description). `lqrCommand` markers fixed to `4 - Control` (verifier check 1 had been failing).
- **Checked:** fits on synthetic data; full runs on a rig (real lab console, stand-in ssh + simulated StreamData with a known slope): 6-drift run recovers the surface to 0.1 %, edge/platform skips, Stop, Hardware all-off, exclusions; local console prompt flows; batch MATLAB: 0 warnings, resting platforms still, drift only while floating (BLACK, BLUE, RED, RED+ARM), verifier OK; headless Chromium, no script errors. Tutorial 4.5, wiki pages.

### 2026-10-13 11:30 EDT: folders reorganised; release notes
- **Moved by the user:** `Legacy Simulator/` → `archive/` (gitignored), the CAD → `designs/` (gitignored; an identical old copy stays in `archive/00 - FULL LAB ASSEMBLY/`), `New_SPOT_Project.py/.bat` → `archive/`, `template/documentation-claude/` → `documentation/`, `template/Resources/` → `resources/`; new `changes/`.
- **Updated to match:** `New_SPOT_Project.m` (leaves out `documentation`, copies its PDF); `'resources'` in `writeTrackerPositions`, `spot.arm.parameters`, `server.py`, comments, `calibrate_arm.py`; `cad_tools/` moved to `designs/` (`build_cad_models.py` reads its own parent, writes `../template`); comments in `Contents.m`, `ArmTorque`, `arm_torque.cpp`, `cad.js`, `verifyConsoleController`; `archive/README.md`; tutorial; wiki (`Legacy-Simulator.md` → `Archive.md`).
- **Checked:** `build_cad_models.py` from `designs/` gives byte-identical models; `build_wiki.py --strict` passes; tutorial builds (29 pages); batch MATLAB: a new project has `resources/` and the PDF, no `documentation/`, and writes its tracker positions.
- `changes/patch-notes.tex`: release notes for 5.0.0 Pre-Release.2 (everything since 2026-10-06); acknowledgments left for the user.

### 2026-10-12 15:30 EDT: Python offered when missing; PWM locked; arm gains in the console
- `SPOT` without Python: `spot.console.offerPython` (dialog: Install Python / Open python.org / Cancel; printed without a desktop) → `spot.console.installPython` (Windows: `winget --scope user`, else python.org 3.12.10 installer `/quiet` per-user; Linux: apt/dnf/yum/zypper/pacman via `pkexec`, root directly, else prints the `sudo` command). `findPython` also looks in the Windows install folders (no MATLAB restart). Tested on Linux with PATH emptied; **the Windows install path is untested**.
- Thruster PWM locked like the data rate (`ratelock.js` now has kinds `rate`/`pwm`; `#pwmLockDialog`).
- Arm controllers' gains in the Control pane (Arm on): `control.ARM.position` (P/I/D per joint, profile velocity/acceleration, PWM limit) and `.velocity` (P/I, profile acceleration, PWM limit) → `armControl`; the user's diagram (backup `Legacy/slx_before_arm_gains/`) now reads `armControl.*` on both blocks. Defaults = the blocks' values (position PWM limit 80). Checked: 0 warnings, other P gains change the response, out-of-range values refused.

### 2026-10-12 09:40 EDT: `template/`, `projects/` and a MATLAB project creator
- `Template_Files_Updated` renamed `template`; new empty `projects/` beside it. `New_SPOT_Project.m` (top level) replaces the `.py`/`.bat`: copies `template/` without `.spot_cache`, `slprj`, `Saved Data`, `documentation-claude`, `STATUS.md`, the settings/design files, Simulink build files and `__pycache__`; adds `SPOT_Tutorial.pdf`; renames the diagram to `<name>.slx`; checks the name (isvarname, ≤ 63, not SPOT's or an existing function). Tested in batch MATLAB: bad names refused, existing folder refused, the new project simulated with 0 warnings.
- `.gitignore` moved to the top folder (the repo is now `New_SPOT_Project.m` + `template/` + empty `projects/`); a new project has no `.gitignore` of its own.
- `build_wiki.py`: markers count only on a line of their own (the documentation page had been re-expanding its own listing on every run); wiki, tutorial, `cad_tools/build_cad_models.py` and the Legacy README updated for the new names.

### 2026-10-12 09:05 EDT: GitHub wiki; Legacy Simulator; the repo trimmed to what a user needs
- `documentation-claude/wiki/`: 181 GitHub-wiki pages (flat `.md`, `[[Title|Page]]` links, `$$` maths, Mermaid): Home, Getting Started, Architecture, Project Folder, Settings File, Workspace Variables, Logged Signals (the 108-signal bus order, from the `.slx`), Data Stores (49); the seven console tabs with 32 screenshots (`images/gui-*.png`, Playwright); 16 diagram pages with 38 subsystem pictures; every `+spot` function line by line (121 pages); every console `.py`/`.js` module and hardware program function by function; the maths pages; the tools. `build_wiki.py --strict` passes (0 missing function pages; all links and images resolve).
- `../Legacy Simulator/` holds everything not needed to run simulations or experiments (old code, `.slx` backups, Saved Data, Resources/Manuals, the camera code, the old Dynamixel blocks and SDK, the CAD with `cad_tools/`, whose `build_cad_models.py` now takes `--out` and finds the template from its own location). `New_SPOT_Project.py/.bat` moved into the template; `.gitignore` added (caches, builds, `Saved Data/`, `STATUS.md`).
- Checked: a new project from the trimmed template (WikiTest) simulates, saves and lists its run; the tutorial rebuilt after the moves.
- 2026-10-12: `documentation/` renamed `documentation-claude/` and gitignored (the tutorial PDF no longer ships with the repo; `New_SPOT_Project.py` copies it when present). Still to do for publishing: `git init` in `Template_Files_Updated`, push `documentation-claude/wiki/` to `<repo>.wiki.git` (item 6 below).

### 2026-10-11 10:40 EDT: arm in torque control; new arm block; slow encoder reads explained
- **Asked:** split the arm's controllers from its model (all torque; the servos' position/velocity laws in the controllers, torque passed through as an example); a new hardware block (torque in; encoders and torques out); find why the encoders read at ~2 Hz. The user chose to stay at 20 Hz (the servos' own gains need ~200 Hz in torque mode) and Return Delay Time as an option.
- **Changed:** `spot.arm.Plant` (torques in, measured torques out), `PositionControl`, `VelocityControl`, `withinLimits`, `checkCommands`, `compareSteps`, `servoFromDiagram`, `defaults`; `spot.hardware.ArmTorque` + `csrc/dynamixel/src/arm_torque.cpp`; `arm/setup_arm_serial.sh`; `spot.params.apply` (`armMotor`). Diagram (backup `Legacy/slx_before_arm_torque/`): Default Position/Velocity Control (ARM) blocks (velocity = enabler 3), Custom (ARM) = torque pass-through, plant wiring, arm measured states every step, ArmTorque in Manipulator Encoder Data fed from Robotic Arm Code, `ARM_Measured_Torque`/`ARM_Servo_Status` stores and 8 logged signals; logged `ARM_Shoulder/Elbow_Desired` names were swapped (fixed). Tutorial 6 rewritten; PDF rebuilt (28 pages).
- **Encoders:** old code vs a stand-in bus (pty, 1 Mbps, return delay, FTDI timer): reads take 11–18 ms of an 18.6 ms timeout; with 0/6/10/30 ms extra delay 20/16/12/6 Hz. New block: 1 ms timer requested, poll() with deadline, per-servo replies, status: every read at 20 Hz, ~3 ms, up to 250 Hz possible; ASan/UBSan clean.
- **Checked:** replicas at 1 ms = old plant modes 1/3 (0.04°/0.16°); 20 Hz gains designed on the coupled arm, robust over poses, base, inertia ±50 %, friction ×0.3–2 (within ~3°); simulations of the user's diagram; generated code: read before control, torque written in Update the same step; `compareSteps` on a simulated calibration run (0.15° rms).

### 2026-10-11 15:20 EDT: arm bracket; PWM, data rate and lab console address in the console
- **Arm bracket offset:** not the CAD: `spot_settings.json` had the old `Phi`/`b0` again (written back at 14:10 by a page opened before the 2026-10-10 fix); set to 1.2337865/0.2502067. Checked in the Designer: the first link sits on the shoulder's horn again.
- **Scenario rates row:** Sample Rate, Data Rate (locked; padlock + warning dialog, shared with the Experiment tab's Data Rate, which is the same setting; `ratelock.js`), Thruster PWM (`pwmFrequency` → `PWMFreq`; a warning when ≠ the Teensy's 5 Hz). The Step box is gone.
- **Lab console address** (Experiment tab, Lab): `labConsoleAddress` → `labConsoleIP` in 8 diagram blocks (backup `Legacy/slx_before_lab_address/`), `buildBoard`, the local server's lab link (follows it live). `SPOT_User_Parameters.m` no longer sets `PWMFreq`/`phaseSpaceServer`.
- **Checked:** batch MATLAB (apply, validation, `ipText`), full-diagram code generation (no literal address left, `labConsoleIP` in the UDP calls), simulation of the user's diagram (6 warnings, the Live Data Streams ones); the real local console with a batch MATLAB worker and a stand-in lab console at 127.0.0.2 in headless Chromium: 26 checks (lock, dialog, linked fields, validation, settings saved, address switch, link) and no script errors. Tutorial 3, 4 updated (29 pages).

### 2026-10-11 15:45 EDT: Live Data Streams at priority 73
- Its 6 "read before write"/"write after read" warnings are gone: it ran at 22, before the plant (25) wrote the measured states; now 73, after the outgoing subsystems (backup `Legacy/slx_before_stream_priority/`). Checked: the user's diagram simulates with 0 warnings. Found: Open items 9 (the arm's torque a step late).

### 2026-10-11 15:55 EDT: the arm's torque in the step it is computed
- The user's diagram read `ARM_Controls` inside Robotic Arm Code (priority 21), a step before 4 - Control writes it: the torque went out 50 ms later than in simulation. New top-level Arm Torques (After Control) (priority 61) reads it and feeds the arm block's Torque input (backup `Legacy/slx_before_arm_handover/`). Checked: 0 simulation warnings; generated code reads the encoders before 4 - Control and sends this step's `ARM_Controls` in Update.

Earlier days, in short (details in `Legacy/STATUS_2026-10-06_to_10-09.md` and `Legacy/STATUS_2026-10-10.md`):
- **10-06/07:** StreamData on the TX2 with the 115-float packet, one socket per destination, timestamp velocities, newest-packet receivers; lab console webapp with the Hardware tab; Teensy serial udev script.
- **10-07:** local console (`SPOT` + MATLAB worker) with Simulation, Experiment (build on the TX2, deploy from it) and Results tabs; `+spot` packages; old GUI, Custom_Library and PhaseSpace_Server retired; many UI refinements.
- **10-08:** server folded into `+spot/+console`; tutorial; `New_SPOT_Project`; error details; PhaseSpace experiment clock and UDP library (`_PSClock` diagram); Messages panel; Live plot; Real time; 0 simulation warnings (atomic order).
- **10-09:** CAD models (Use CAD); Chaser/Target/Obstacle names; Interactive mode (several platforms, controller processes); BLACK/BLUE mixer fix; thrusters every `baseRate`; `_PSClock` made the only diagram.
- **10-10:** STATUS shortened; random session token; tutorial in `documentation/`. CAD: Obstacle frame and blue panels, internal structure, Spacecraft Designer (docks, solar panels, 6 places, panels follow the cone, lab console sync), ZED2 model with real intrinsics and lens views, table barrier/stand/ceiling light, thrusters, LED mounts, flush screws, LIDAR-Lite, solar panels as the lab's photo. Console: PhaseSpace data rate is a setting (`dataRate`), table axes raised, Replay Saved Results button fixed. Arm: profiled; plant at 1 ms inside each step with Dynamixel servo models and stick-slip friction, `Default Position Control (ARM)`, lab calibration script; ignored-goal warnings; Arm in the Designer with its mount.

### 2026-10-10 21:30 EDT: shoulder flush and centred; scratch copy refreshed; day wrapped up
- `s.mass.arm.Phi`/`b0` set to the CAD's shoulder, motor flush and centred on the arm coupler: 1.2337865 rad (70.69°) / 0.2502067 m (were 1.2788883 / 0.25), in `spot.params.defaults`, the project's `spot_settings.json` and `build_cad_models.py` (`ARM_MOUNT` no longer shifts the motor; rebuilt). Saved runs keep their own values. Checked: the six mounts are now symmetric (shoulder at ±0.0827, 0.2361 on +y), Designer close-ups.
- Fixed straight away: the first edit put a comment-only line inside `defaults.m`'s `struct(...)` continuation (a syntax error).
- The test copy of the project was refreshed from the user's diagram of 20:01 (its Sub-Phase 1 now moves the shoulder to 0°: runs, 0 ignored goals); it shows the Live Data Streams priority issue (Open items 9).
- Today's full change log moved to `Legacy/STATUS_2026-10-10.md`.
