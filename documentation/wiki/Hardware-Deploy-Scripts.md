# Deploy Scripts and Agent

**Folder:** `+spot/+console/deploy/`

<!-- source: +spot/+console/deploy/deploy_tx2.sh -->
<!-- source: +spot/+console/deploy/spot-phasespace-console.service -->
<!-- source: +spot/+console/deploy/setup_hardware_link.sh -->
<!-- source: +spot/+console/deploy/spot_agent.sh -->

Run them with a leading `./` (bash takes a path starting with `+` for an option): `./+spot/+console/deploy/deploy_tx2.sh`.

## `deploy_tx2.sh`: install the lab console on the TX2

Copies the console (`webapp/`, `phasespace/`, `deploy/`, and the arm calibration script `resources/arm_calibration/calibrate_arm.py`) to the TX2, compiles StreamData there, and installs and restarts the systemd user service so the console runs at boot on `http://192.168.1.116:8080`. Safe to re-run after any change.

- **Lines 11–16.** The TX2 login (`spot-tx2@192.168.1.116`, override with `TX2=`), the SSH key (`~/.ssh/id_ed25519_spot`, override with `SSH_KEY=`), the destination folder in the TX2 user's home (`PhaseSpace_Server_SendAll`), the service name.
- **Lines 18–23.** `rsync` the three folders (with `--delete`, so each copy mirrors the source; `__pycache__` excluded), and `calibrate_arm.py` into `~/PhaseSpace_Server_SendAll/arm_calibration/` (the Calibration tab sends it from there to the Chaser, see `arm-cal-receive` below).
- **Lines 25–47.** On the TX2: remove files from the pre-2026-10-08 layout, `./phasespace/build_linux.sh`, copy the service file into `~/.config/systemd/user/`, reload, enable and restart it, show its status; enable *linger* so the service starts without anyone logging in (or print the `sudo loginctl enable-linger` to run once).

## `spot-phasespace-console.service`

A systemd *user* service: runs `python3 webapp/app.py --host 0.0.0.0 --port 8080 --no-browser` in the console's folder, unbuffered output (so `journalctl --user -u spot-phasespace-console -f` shows the live log), stopped with SIGINT (so the app stops StreamData cleanly), restarted on failure.

## `setup_hardware_link.sh`: let the TX2 drive the platforms

The lab console reaches the platforms over SSH with a key that can only run one script. Run once from a computer whose key the TX2 and the platforms accept:

1. **Lines 27–31.** On the TX2: create its own key `~/.ssh/id_ed25519_spot_console` if missing (the name must match `HW_KEY` in `app.py`) and read its public half.
2. **Lines 33–61.** On each platform (`spot-red@192.168.1.110`, …): install `spot_agent.sh` as `~/.spot/agent.sh`, make `~/ExperimentSoftware/spot`, and add the TX2's key to `~/.ssh/authorized_keys` with `restrict,command="$HOME/.spot/agent.sh"` (no shell, no forwarding, only that script), replacing an earlier copy of the key (identified by its `spot-console-hw` comment) and backing the file up.
3. **Lines 62–66.** From the TX2, run `status` on the platform to prove the link works.

Re-run it after `spot_agent.sh` changes: the platforms keep the copy installed last (for example, the `arm-cal-…` commands below need the current one).

## `spot_agent.sh`: what the platforms accept

Runs on a platform whenever the TX2 connects; the command comes in `$SSH_ORIGINAL_COMMAND`. Names are checked strictly (`[A-Za-z0-9][A-Za-z0-9_.-]{0,99}`, no `..`), and the arm calibration's arguments by their own patterns, so nothing else can be run.

| Command | What it does |
|---|---|
| `status` | whether the Teensy is at `/dev/ttyACM0`, and which of this user's programs have it open (`user <pid> <name>`) |
| `stream` | set the port to 115200 raw and copy stdin to it until the TX2 hangs up: the Hardware tab's command packets ([[hardware.py|Console-hardware]]) |
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

`running_pid` (lines 46–51) reads `run.pid` and checks the process is alive; `users_of` (52–57) lists `<pid> <name>` of each program with a port open (only this user's are visible); `arm_pid` (58) is `running_pid` for the arm calibration's folder. `ARM_DEV` (line 33) is `/dev/ttyUSB0`; `SPOT_ARM_DEV` overrides it only for tests, since SSH passes no environment to the forced command.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/deploy/deploy_tx2.sh</b> (48 lines; click to show)</summary>

```bash
   1  #!/usr/bin/env bash
   2  # Copies the SPOT Console (webapp/, phasespace/, deploy/ of +spot/+console, and the arm calibration
   3  # script, resources/arm_calibration/calibrate_arm.py, which the Calibration tab runs on the Chaser) to the Jetson TX2,
   4  # compiles StreamData there, and (re)starts the webapp
   5  # service so it runs at boot on http://<tx2>:8080. Safe to re-run after any change.
   6  #   ./deploy_tx2.sh
   7  #   TX2=user@host SSH_KEY=~/.ssh/other_key ./deploy_tx2.sh
   8  set -euo pipefail
   9  cd "$(dirname "$0")/.."   # +spot/+console
  10  
  11  TX2="${TX2:-spot-tx2@192.168.1.116}"
  12  SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
  13  DEST="PhaseSpace_Server_SendAll"   # in the TX2 user's home folder
  14  SERVICE="spot-phasespace-console"
  15  ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
  16  [ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)
  17  
  18  echo "== Copying files to $TX2:~/$DEST"
  19  # The console's three folders; --delete keeps each copy in sync. StreamData builds into output/.
  20  rsync -a --delete --relative --exclude '__pycache__' -e "ssh ${ssh_opts[*]}" \
  21      webapp phasespace deploy \
  22      "$TX2:$DEST/"
  23  rsync -a -e "ssh ${ssh_opts[*]}" ../../resources/arm_calibration/calibrate_arm.py "$TX2:$DEST/arm_calibration/"
  24  
  25  echo "== Compiling and starting the service on $TX2"
  26  ssh "${ssh_opts[@]}" "$TX2" bash -s -- "$DEST" "$SERVICE" <<'REMOTE'
  27  set -euo pipefail
  28  dest="$1"; service="$2"
  29  cd "$HOME/$dest"
  30  # Files from before the console's folders were split (2026-10-08)
  31  rm -rf StreamData.cc Makefile build_linux.sh libowl-src-410 Python
  32  ./phasespace/build_linux.sh
  33  mkdir -p "$HOME/.config/systemd/user"
  34  cp "deploy/$service.service" "$HOME/.config/systemd/user/"
  35  systemctl --user daemon-reload
  36  systemctl --user enable "$service" >/dev/null 2>&1
  37  systemctl --user restart "$service"
  38  sleep 2
  39  systemctl --user --no-pager status "$service" | head -5
  40  if [ "$(loginctl show-user "$(id -un)" -p Linger --value 2>/dev/null)" != "yes" ]; then
  41      loginctl enable-linger 2>/dev/null || {
  42          echo
  43          echo "NOTE: to start at boot without anyone logging in, run this once on the TX2:"
  44          echo "      sudo loginctl enable-linger $(id -un)"
  45      }
  46  fi
  47  REMOTE
  48  echo "== Done: http://${TX2#*@}:8080"
```

</details>

<details>
<summary><b>+spot/+console/deploy/spot-phasespace-console.service</b> (21 lines; click to show)</summary>

```
   1  # systemd user service: runs the SPOT PhaseSpace Console webapp at boot on port 8080,
   2  # reachable from any computer on the network. Installed by deploy_tx2.sh into
   3  # ~/.config/systemd/user/ ("loginctl enable-linger" makes it start without anyone logging in).
   4  #   systemctl --user status spot-phasespace-console
   5  #   journalctl --user -u spot-phasespace-console -f      (live server log)
   6  
   7  [Unit]
   8  Description=SPOT PhaseSpace Console (webapp on port 8080)
   9  
  10  [Service]
  11  WorkingDirectory=%h/PhaseSpace_Server_SendAll
  12  ExecStart=/usr/bin/python3 %h/PhaseSpace_Server_SendAll/webapp/app.py --host 0.0.0.0 --port 8080 --no-browser
  13  Environment=PYTHONUNBUFFERED=1
  14  # Ctrl+C equivalent, so the app stops StreamData cleanly before exiting
  15  KillSignal=SIGINT
  16  TimeoutStopSec=15
  17  Restart=on-failure
  18  RestartSec=3
  19  
  20  [Install]
  21  WantedBy=default.target
```

</details>

<details>
<summary><b>+spot/+console/deploy/setup_hardware_link.sh</b> (69 lines; click to show)</summary>

```bash
   1  #!/usr/bin/env bash
   2  # Lets the SPOT lab console on the TX2 drive RED/BLACK/BLUE: the Teensy (Hardware tab) and the
   3  # experiments (copy, start, stop, collect data). Run from a computer whose SSH key the TX2 and the
   4  # platforms already accept. Safe to re-run (also to upgrade from the Teensy-only version).
   5  #   1. creates the TX2's own key, ~/.ssh/id_ed25519_spot_console, if it doesn't have one
   6  #   2. on each platform: installs spot_agent.sh (next to this script) as ~/.spot/agent.sh, and adds the TX2's key
   7  #      to ~/.ssh/authorized_keys restricted to that script (no shell, no forwarding)
   8  #   3. checks from the TX2 that each platform answers
   9  #
  10  #   ./setup_hardware_link.sh                 # RED, BLACK and BLUE
  11  #   ./setup_hardware_link.sh blue            # only some
  12  #   TX2=user@host SSH_KEY=~/.ssh/other_key ./setup_hardware_link.sh
  13  set -euo pipefail
  14  cd "$(dirname "$0")"
  15  
  16  TX2="${TX2:-spot-tx2@192.168.1.116}"
  17  SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
  18  CONSOLE_KEY=".ssh/id_ed25519_spot_console"   # on the TX2; must match HW_KEY in webapp/app.py
  19  MARKER="spot-console-hw"                      # comment that identifies the key in authorized_keys
  20  declare -A ADDRESS=([red]=192.168.1.110 [black]=192.168.1.111 [blue]=192.168.1.112)
  21  boards=("$@")
  22  [ ${#boards[@]} -eq 0 ] && boards=(red black blue)
  23  
  24  ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
  25  [ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)
  26  
  27  echo "== TX2 ($TX2): console key"
  28  pubkey=$(ssh "${ssh_opts[@]}" "$TX2" "
  29      [ -f ~/$CONSOLE_KEY ] || ssh-keygen -q -t ed25519 -N '' -C '$MARKER' -f ~/$CONSOLE_KEY
  30      cut -d' ' -f1,2 ~/$CONSOLE_KEY.pub")
  31  echo "$pubkey $MARKER"
  32  
  33  failed=()
  34  for board in "${boards[@]}"; do
  35      address="${ADDRESS[$board]:-}"
  36      [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
  37      user="spot-$board"
  38      echo "== ${board^^} ($user@$address)"
  39      # The link script goes over stdin; the key line is passed as an argument
  40      if ! ssh "${ssh_opts[@]}" "$user@$address" bash -s -- "$pubkey" "$MARKER" <<REMOTE
  41  set -euo pipefail
  42  pubkey="\$1"; marker="\$2"
  43  mkdir -p ~/.spot ~/.ssh
  44  chmod 700 ~/.ssh
  45  cat > ~/.spot/agent.sh <<'LINK'
  46  $(cat spot_agent.sh)
  47  LINK
  48  chmod 755 ~/.spot/agent.sh
  49  mkdir -p ~/ExperimentSoftware/spot
  50  touch ~/.ssh/authorized_keys
  51  cp ~/.ssh/authorized_keys ~/.ssh/authorized_keys.bak-spot-console
  52  # Replace any earlier copy of the console key, keep every other line as it was
  53  { grep -v " \$marker\\\$" ~/.ssh/authorized_keys || true
  54    echo "restrict,command=\"\$HOME/.spot/agent.sh\" \$pubkey \$marker"; } > ~/.ssh/authorized_keys.new
  55  chmod 600 ~/.ssh/authorized_keys.new
  56  mv ~/.ssh/authorized_keys.new ~/.ssh/authorized_keys
  57  echo "Installed ~/.spot/agent.sh and the console key (backup: ~/.ssh/authorized_keys.bak-spot-console)"
  58  REMOTE
  59      then
  60          failed+=("$board"); continue
  61      fi
  62      echo "-- check from the TX2:"
  63      if ! ssh "${ssh_opts[@]}" "$TX2" "ssh -i ~/$CONSOLE_KEY -o IdentitiesOnly=yes -o BatchMode=yes \
  64              -o ConnectTimeout=5 -o StrictHostKeyChecking=accept-new $user@$address status"; then
  65          failed+=("$board")
  66      fi
  67  done
  68  echo
  69  [ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
```

</details>

<details>
<summary><b>+spot/+console/deploy/spot_agent.sh</b> (199 lines; click to show)</summary>

```bash
   1  #!/bin/bash
   2  # Runs on RED/BLACK/BLUE. The SPOT lab console on the TX2 logs in with a key that can only run
   3  # this script (a forced command in ~/.ssh/authorized_keys, installed by setup_hardware_link.sh).
   4  # It replaces spot_teensy_link.sh (status and stream are unchanged) and adds the experiments:
   5  #
   6  #   status                  whether the Teensy is plugged in, and which other programs have it open
   7  #   stream                  copies the console's command packets to the Teensy until it hangs up
   8  #   exp-receive NAME        unpacks a tar of experiment files (stdin) into ~/ExperimentSoftware/spot/NAME,
   9  #                           keeping earlier data files; reports libraries the program can't find here
  10  #   exp-start NAME FILE     starts NAME/FILE in the background in that folder (output: run.log)
  11  #   exp-stop NAME           stops it: SIGTERM, then SIGKILL after 5 s
  12  #   exp-status NAME         whether it is running, how many data files there are, the end of run.log
  13  #   exp-data NAME           the data files (ExpLog*) as a tar on stdout
  14  #   exp-clean NAME          deletes the data files (after they were copied off)
  15  #
  16  # and the arm calibration (the lab console's Calibration tab; the Chaser only, which has the arm):
  17  #
  18  #   arm-cal-check           what calibrate_arm.py needs here: python3 and its version, the Dynamixel SDK,
  19  #                           the U2D2 at /dev/ttyUSB0 and access to it, its latency timer, the dialout group,
  20  #                           programs holding the port, and whether a calibration is running
  21  #   arm-cal-receive         unpacks a tar holding calibrate_arm.py (stdin) into ~/.spot/arm_calibration
  22  #   arm-cal-start JOINTS TESTS [simulate]
  23  #                           starts it in the background (--yes): JOINTS the servo IDs as digits ("123"),
  24  #                           TESTS a comma list of breakaway, sweep, inertia, steps; simulate: its simulated
  25  #                           servos (a rehearsal). Output in run.log, its exit code as a last line "EXIT n"
  26  #   arm-cal-status          whether it is running, its run folder, its exit code, the end of run.log
  27  #   arm-cal-stop            stops it (SIGTERM: it switches the servos' torque off and restores them)
  28  #   arm-cal-data            the newest run's results (*.json, *.csv) as a tar on stdout
  29  #
  30  # NAME and FILE are checked strictly (letters, digits, _ . -), so nothing else can be run.
  31  DEV=/dev/ttyACM0
  32  BASE="$HOME/ExperimentSoftware/spot"
  33  ARM_DEV="${SPOT_ARM_DEV:-/dev/ttyUSB0}"   # (SSH passes no environment to the forced command: always ttyUSB0 there)
  34  ARM="$HOME/.spot/arm_calibration"
  35  
  36  read -r -a ARGS <<< "$SSH_ORIGINAL_COMMAND"
  37  CMD="${ARGS[0]}"
  38  NAME="${ARGS[1]}"
  39  FILE="${ARGS[2]}"
  40  
  41  valid() { [[ "$1" =~ ^[A-Za-z0-9][A-Za-z0-9_.-]{0,99}$ ]] && [[ "$1" != *..* ]]; }
  42  need_name() {
  43      valid "$NAME" || { echo "Not a valid experiment name: $NAME" >&2; exit 2; }
  44      DIR="$BASE/$NAME"
  45  }
  46  running_pid() {
  47      local pid
  48      [ -f "$DIR/run.pid" ] || return 1
  49      pid=$(cat "$DIR/run.pid")
  50      [[ "$pid" =~ ^[0-9]+$ ]] && kill -0 "$pid" 2>/dev/null && echo "$pid"
  51  }
  52  users_of() {   # PID NAME of each program with the port $1 open (only this user's are visible)
  53      find /proc/[0-9]*/fd -lname "$1" 2>/dev/null | cut -d/ -f3 | sort -u | while read -r pid; do
  54          name=$(tr '\0' '\n' < "/proc/$pid/cmdline" 2>/dev/null | head -n 1)
  55          echo "$pid ${name##*/}"
  56      done
  57  }
  58  arm_pid() { DIR="$ARM"; running_pid; }
  59  
  60  case "$CMD" in
  61      status)
  62          if [ -e "$DEV" ]; then echo "teensy yes"; else echo "teensy no"; fi
  63          # Programs with the port open (only this user's are visible, which includes experiments)
  64          find /proc/[0-9]*/fd -lname "$DEV" 2>/dev/null | cut -d/ -f3 | sort -u | while read -r pid; do
  65              name=$(tr '\0' '\n' < "/proc/$pid/cmdline" 2>/dev/null | head -n 1)
  66              name=${name##*/}
  67              # stty is this script's own "stream" setting up the port
  68              [ "$name" = stty ] || echo "user $pid $name"
  69          done
  70          ;;
  71      stream)
  72          [ -e "$DEV" ] || { echo "No Teensy at $DEV" >&2; exit 3; }
  73          # Raw: the packets are binary, so no newline translation or echo
  74          stty -F "$DEV" 115200 raw -echo || exit 4
  75          # The name lets "status" tell the console's own link apart from other programs
  76          exec -a spot-teensy-link cat > "$DEV"
  77          ;;
  78      exp-receive)
  79          need_name
  80          if running_pid > /dev/null; then echo "$NAME is running here; stop it first" >&2; exit 4; fi
  81          rm -rf "$DIR.new" && mkdir -p "$DIR.new" || exit 5
  82          if ! tar -x -C "$DIR.new" --no-same-owner; then rm -rf "$DIR.new"; echo "Could not unpack the files" >&2; exit 5; fi
  83          if [ -d "$DIR" ]; then
  84              mv "$DIR"/ExpLog* "$DIR.new"/ 2>/dev/null   # keep data not copied off yet
  85              rm -rf "$DIR"
  86          fi
  87          mv "$DIR.new" "$DIR"
  88          chmod +x "$DIR"/*.elf 2>/dev/null
  89          for f in "$DIR"/*.elf; do
  90              [ -e "$f" ] && ldd "$f" 2>/dev/null | grep "not found" | sed -e 's/^[[:space:]]*/missing /' -e 's/ =>.*//'
  91          done
  92          echo "received $(find "$DIR" -type f | wc -l) files"
  93          ;;
  94      exp-start)
  95          need_name
  96          valid "$FILE" || { echo "Not a valid file name: $FILE" >&2; exit 2; }
  97          [ -x "$DIR/$FILE" ] || { echo "There is no $FILE in $DIR (deploy first)" >&2; exit 3; }
  98          if pid=$(running_pid); then echo "already running $pid"; exit 0; fi
  99          cd "$DIR" || exit 5
 100          # setsid: keeps running after this SSH session ends
 101          setsid nohup "./$FILE" > run.log 2>&1 < /dev/null &
 102          echo $! > run.pid
 103          echo "started $!"
 104          ;;
 105      exp-stop)
 106          need_name
 107          if ! pid=$(running_pid); then echo "not running"; exit 0; fi
 108          kill -TERM "$pid"
 109          for _ in $(seq 50); do kill -0 "$pid" 2>/dev/null || break; sleep 0.1; done
 110          if kill -0 "$pid" 2>/dev/null; then kill -KILL "$pid"; echo "killed $pid"; else echo "stopped $pid"; fi
 111          ;;
 112      exp-status)
 113          need_name
 114          if pid=$(running_pid); then echo "running $pid"; else echo "running no"; fi
 115          echo "data $(ls "$DIR"/ExpLog* 2>/dev/null | wc -l)"
 116          [ -f "$DIR/run.log" ] && tail -n 5 "$DIR/run.log" | sed 's/^/log /'
 117          ;;
 118      exp-data)
 119          need_name
 120          cd "$DIR" 2>/dev/null && ls ExpLog* > /dev/null 2>&1 || { echo "No data files on this platform" >&2; exit 3; }
 121          tar -c ExpLog*
 122          ;;
 123      exp-clean)
 124          need_name
 125          rm -f "$DIR"/ExpLog*
 126          echo "cleaned"
 127          ;;
 128      arm-cal-check)
 129          if command -v python3 > /dev/null; then
 130              echo "python $(python3 -c 'import sys; print("%d.%d.%d" % sys.version_info[:3])')"
 131              if python3 -c "import dynamixel_sdk" 2>/dev/null; then echo "sdk yes"; else echo "sdk no"; fi
 132          else
 133              echo "python no"; echo "sdk no"
 134          fi
 135          if [ -e "$ARM_DEV" ]; then echo "port yes"; else echo "port no"; fi
 136          if [ -r "$ARM_DEV" ] && [ -w "$ARM_DEV" ]; then echo "access yes"; else echo "access no"; fi
 137          lat="/sys/bus/usb-serial/devices/${ARM_DEV##*/}/latency_timer"
 138          if [ -r "$lat" ]; then echo "latency $(cat "$lat")"; else echo "latency unknown"; fi
 139          if id -nG | tr ' ' '\n' | grep -qx dialout; then echo "dialout yes"; else echo "dialout no"; fi
 140          users_of "$ARM_DEV" | sed 's/^/user /'
 141          if [ -f "$ARM/calibrate_arm.py" ]; then echo "script yes"; else echo "script no"; fi
 142          if pid=$(arm_pid); then echo "running $pid"; else echo "running no"; fi
 143          ;;
 144      arm-cal-receive)
 145          if arm_pid > /dev/null; then echo "A calibration is running; stop it first" >&2; exit 4; fi
 146          mkdir -p "$ARM" && rm -rf "$ARM/new" && mkdir "$ARM/new" || exit 5
 147          if ! tar -x -C "$ARM/new" --no-same-owner calibrate_arm.py; then rm -rf "$ARM/new"; echo "Could not unpack calibrate_arm.py" >&2; exit 5; fi
 148          mv "$ARM/new/calibrate_arm.py" "$ARM/calibrate_arm.py" && rm -rf "$ARM/new"
 149          echo "received calibrate_arm.py"
 150          ;;
 151      arm-cal-start)
 152          JOINTS="$NAME"; TESTS="$FILE"; MODE="${ARGS[3]}"
 153          [[ "$JOINTS" =~ ^[1-3]{1,3}$ ]] || { echo "JOINTS must be servo IDs 1-3, e.g. 123" >&2; exit 2; }
 154          [[ "$TESTS" =~ ^(breakaway|sweep|inertia|steps)(,(breakaway|sweep|inertia|steps)){0,3}$ ]] || {
 155              echo "TESTS must be a comma list of breakaway, sweep, inertia, steps" >&2; exit 2; }
 156          [ -z "$MODE" ] || [ "$MODE" = simulate ] || { echo "The third argument can only be simulate" >&2; exit 2; }
 157          [ -f "$ARM/calibrate_arm.py" ] || { echo "No calibrate_arm.py here (send it first)" >&2; exit 3; }
 158          if pid=$(arm_pid); then echo "already running $pid"; exit 0; fi
 159          if [ -z "$MODE" ]; then
 160              [ -e "$ARM_DEV" ] || { echo "No U2D2 at $ARM_DEV (plugged in, powered?)" >&2; exit 3; }
 161              held=$(users_of "$ARM_DEV")
 162              [ -z "$held" ] || { echo "The arm's port is in use by: $held" >&2; exit 4; }
 163          fi
 164          OPTS=(--joints $(echo "$JOINTS" | fold -w1) --tests ${TESTS//,/ })
 165          [ "$MODE" = simulate ] && OPTS+=(--simulate)
 166          cd "$ARM" || exit 5
 167          # setsid: keeps running after this SSH session ends. The shell around it ignores SIGTERM, so a stop
 168          # (SIGTERM to the group) reaches the script, which switches the servos off; the shell then logs its exit
 169          setsid nohup bash -c 'trap "" TERM; python3 -u calibrate_arm.py --yes --out runs "$@"; echo "EXIT $?"' \
 170              arm-cal "${OPTS[@]}" > run.log 2>&1 < /dev/null &
 171          echo $! > run.pid
 172          echo "started $!"
 173          ;;
 174      arm-cal-status)
 175          if pid=$(arm_pid); then echo "running $pid"; else echo "running no"; fi
 176          last=$(ls -1d "$ARM"/runs/*/ 2>/dev/null | tail -n 1)
 177          [ -n "$last" ] && echo "run $(basename "$last")"
 178          if [ -f "$ARM/run.log" ]; then
 179              code=$(grep -E '^EXIT [0-9]+$' "$ARM/run.log" | tail -n 1 | cut -d' ' -f2)
 180              [ -n "$code" ] && echo "exit $code"
 181              tail -n 40 "$ARM/run.log" | sed 's/^/log /'
 182          fi
 183          ;;
 184      arm-cal-stop)
 185          if ! pid=$(arm_pid); then echo "not running"; exit 0; fi
 186          kill -TERM -- "-$pid" 2>/dev/null || kill -TERM "$pid"
 187          for _ in $(seq 100); do kill -0 "$pid" 2>/dev/null || break; sleep 0.1; done
 188          if kill -0 "$pid" 2>/dev/null; then kill -KILL -- "-$pid" 2>/dev/null; echo "killed $pid"; else echo "stopped $pid"; fi
 189          ;;
 190      arm-cal-data)
 191          last=$(ls -1d "$ARM"/runs/*/ 2>/dev/null | tail -n 1)
 192          [ -n "$last" ] || { echo "No calibration run here" >&2; exit 3; }
 193          cd "$last" && tar -c --transform "s,^,$(basename "$last")/," -- *.json *.csv 2>/dev/null
 194          ;;
 195      *)
 196          echo "This key only accepts: status, stream, exp-receive, exp-start, exp-stop, exp-status, exp-data, exp-clean, arm-cal-check, arm-cal-receive, arm-cal-start, arm-cal-status, arm-cal-stop, arm-cal-data" >&2
 197          exit 2
 198          ;;
 199  esac
```

</details>
<!-- /listing -->
