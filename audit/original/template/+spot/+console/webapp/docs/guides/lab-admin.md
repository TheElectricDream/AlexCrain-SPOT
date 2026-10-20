# Lab Administration: Installing the Lab's Software

The lab console, SPOT Vision and the platforms' links are installed by the scripts in `+spot/+console/deploy/`. The easiest way to run them is the local console's **Lab Setup** button ([Lab Setup](page:lab-setup)), on the lab network. From a terminal, run them with a leading `./` (bash takes a path starting with `+` for an option): `./+spot/+console/deploy/deploy_tx2.sh`. All of them use the SSH key `~/.ssh/id_ed25519_spot`, which the TX2, the Orin and the platforms accept.

## `deploy_tx2.sh`: install the lab console on the TX2

Copies the console (`webapp/`, `phasespace/`, `deploy/`, and the arm calibration script `+spot/+arm/calibration/calibrate_arm.py`) to the TX2, with a read-only copy of the project's code and diagrams in `~/PhaseSpace_Server_SendAll/docs_source/` for the Documentation tab, compiles StreamData there, and installs and restarts the systemd user service so the console runs at boot on `http://192.168.1.116:8080`. Safe to re-run after any change.

- The TX2 login (`spot-tx2@192.168.1.116`, override with `TX2=`), the SSH key (`~/.ssh/id_ed25519_spot`, override with `SSH_KEY=`), the destination folder in the TX2 user's home (`PhaseSpace_Server_SendAll`), the service name.
- `rsync` the three folders (with `--delete`, so each copy mirrors the source; `__pycache__` excluded), and `calibrate_arm.py` into `~/PhaseSpace_Server_SendAll/arm_calibration/` (the Calibration tab sends it from there to the Chaser, see `arm-cal-receive` below).
- On the TX2: remove files from the pre-2026-10-08 layout, `./phasespace/build_linux.sh`, copy the service file into `~/.config/systemd/user/`, reload, enable and restart it, show its status; enable *linger* so the service starts without anyone logging in (or print the `sudo loginctl enable-linger` to run once).

## `deploy_orin.sh`: install SPOT Vision on the Orin

Copies `+spot/+console/vision/` to `~/spot_vision` on the Orin (`spot-vision@192.168.1.115`, override with `ORIN=`) and installs and restarts its systemd user service, `spot-vision`, which serves the cameras on port 8090 for the lab console's [Vision tab](page:tab-vision). On the Orin it checks the camera libraries and says which cameras won't stream: the ZED needs OpenCV and the ZED SDK's `pyzed` in the system Python; the RealSense L515 needs a Python with `pyrealsense2` 2.54, the last release that supports it (`REALSENSE_PYTHON=`); the DVXplorer needs iniVation's `dv-processing` (`DVS_PYTHON=`); the ICI 9320P thermal camera needs `python-libusb1` and `pyusb` (`python3 -m pip install --user libusb1 pyusb`; `THERMAL_PYTHON=`), its SDK being copied with SPOT Vision. The thermal camera also needs SPOT Vision's udev rule, and the Orin's user in the `plugdev` group. Both need `sudo`, which the script doesn't have, so it prints the commands when either is missing; install the rule again whenever it changes:

```bash
sudo cp ~/spot_vision/99-ici-9320p.rules /etc/udev/rules.d/ && sudo udevadm control --reload-rules && sudo udevadm trigger
sudo usermod -aG plugdev spot-vision     # then log in again, or restart the Orin
```

`journalctl --user -u spot-vision -f` on the Orin shows its log. To check the thermal camera by hand, run `cd ~/spot_vision/vendor && python3 -m ici9320 info` on the Orin: it prints the firmware version and housing temperature.

## `spot-phasespace-console.service`

A systemd *user* service: runs `python3 webapp/app.py --host 0.0.0.0 --port 8080 --no-browser` in the console's folder, unbuffered output (so `journalctl --user -u spot-phasespace-console -f` shows the live log), stopped with SIGINT (so the app stops StreamData cleanly), restarted on failure.

## `setup_hardware_link.sh`: let the TX2 drive the platforms

The lab console reaches the platforms over SSH with a key that can only run one script. Run once from a computer whose key the TX2 and the platforms accept:

1. On the TX2: create its own key `~/.ssh/id_ed25519_spot_console` if missing (the name must match `HW_KEY` in `app.py`) and read its public half.
2. On each platform (`spot-red@192.168.1.110`, …): install `spot_agent.sh` as `~/.spot/agent.sh`, make `~/ExperimentSoftware/spot`, and add the TX2's key to `~/.ssh/authorized_keys` with `restrict,command="$HOME/.spot/agent.sh"` (no shell, no forwarding, only that script), replacing an earlier copy of the key (identified by its `spot-console-hw` comment) and backing the file up.
3. On each platform, with `sudo` (it asks for the platform's password): keep it on the network: Wi-Fi power saving off (a dozing adapter misses the TX2's ARP requests, and the platform looks switched off) and the computer never suspending. `NO_POWER=1` skips this step; Lab Setup always does, since it can't answer a password prompt.
4. From the TX2, run `status` on the platform to prove the link works.

`./setup_hardware_link.sh blue` links only the Obstacle (any of `red`, `black`, `blue`).

Re-run it after `spot_agent.sh` changes: the platforms keep the copy installed last (for example, the `arm-cal-…` commands below need the current one).

## `spot_agent.sh`: what the platforms accept

Runs on a platform whenever the TX2 connects; the command comes in `$SSH_ORIGINAL_COMMAND`. Names are checked strictly (`[A-Za-z0-9][A-Za-z0-9_.-]{0,99}`, no `..`), and the arm calibration's arguments by their own patterns, so nothing else can be run.

| Command | What it does |
|---|---|
| `status` | whether the Teensy is at `/dev/ttyACM0`, and which of this user's programs have it open (`user <pid> <name>`) |
| `stream` | set the port to 115200 raw and copy stdin to it until the TX2 hangs up: the Hardware tab's command packets ([hardware.py](file:hardware.py)) |
| `exp-receive NAME` | unpack a tar from stdin into `~/ExperimentSoftware/spot/NAME` (refused while it is running), keeping any `ExpLog*` not collected yet, make the `.elf` executable, report shared libraries it can't find (`ldd`) |
| `exp-start NAME FILE` | start `NAME/FILE` in that folder with `setsid nohup` (it survives the SSH session), output to `run.log`, pid in `run.pid` |
| `exp-stop NAME` | SIGTERM, then SIGKILL after 5 s |
| `exp-status NAME` | running or not, how many `ExpLog*` files, the last 5 lines of `run.log` |
| `exp-data NAME` | the `ExpLog*` files as a tar on stdout |
| `exp-clean NAME` | delete them (after they were copied off) |
| `arm-cal-check` | what `calibrate_arm.py` needs on the Chaser: `python` (its version), `sdk` (whether `dynamixel_sdk` imports), `port` (the U2D2 at `/dev/ttyUSB0`), `access` (readable and writable), `latency` (the adapter's latency timer [ms]), `dialout` (this user in the group), `user <pid> <name>` for each program holding the port, `script` (sent yet), `running` |
| `arm-cal-receive` | unpack a tar holding `calibrate_arm.py` from stdin into `~/.spot/arm_calibration` (refused while a calibration runs) |
| `arm-cal-start JOINTS TESTS [simulate]` | JOINTS the servo IDs as digits (`^[1-3]{1,3}$`, e.g. `123`), TESTS a comma list of `breakaway`, `sweep`, `inertia`, `steps`, the third argument only `simulate`; refused without the script, and (unless `simulate`) without the U2D2 or while a program holds its port; starts `python3 -u calibrate_arm.py --yes --out runs --joints … --tests … [--simulate]` in `~/.spot/arm_calibration` with `setsid nohup`, inside a bash that ignores SIGTERM and logs `EXIT n` (the script's exit code) when it ends; output to `run.log`, pid in `run.pid` |
| `arm-cal-status` | `running`, `run` (the newest run folder), `exit` (from the `EXIT` line) and the last 40 lines of `run.log` (`log …`) |
| `arm-cal-stop` | SIGTERM to the process group (the script switches the servos' torque off and restores them), SIGKILL after 10 s |
| `arm-cal-data` | the newest run's `*.json` and `*.csv` as a tar on stdout (in a folder named after the run) |

`running_pid` reads `run.pid` and checks the process is alive; `users_of` lists `<pid> <name>` of each program with a port open (only this user's are visible); `arm_pid` is `running_pid` for the arm calibration's folder. `ARM_DEV` is `/dev/ttyUSB0`; `SPOT_ARM_DEV` overrides it only for tests, since SSH passes no environment to the forced command.

