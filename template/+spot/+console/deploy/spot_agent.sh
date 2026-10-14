#!/bin/bash
# Runs on RED/BLACK/BLUE. The SPOT lab console on the TX2 logs in with a key that can only run
# this script (a forced command in ~/.ssh/authorized_keys, installed by setup_hardware_link.sh).
# It replaces spot_teensy_link.sh (status and stream are unchanged) and adds the experiments:
#
#   status                  whether the Teensy is plugged in, and which other programs have it open
#   stream                  copies the console's command packets to the Teensy until it hangs up
#   exp-receive NAME        unpacks a tar of experiment files (stdin) into ~/ExperimentSoftware/spot/NAME,
#                           keeping earlier data files; reports libraries the program can't find here
#   exp-start NAME FILE     starts NAME/FILE in the background in that folder (output: run.log)
#   exp-stop NAME           stops it: SIGTERM, then SIGKILL after 5 s
#   exp-status NAME         whether it is running, how many data files there are, the end of run.log
#   exp-data NAME           the data files (ExpLog*) as a tar on stdout
#   exp-clean NAME          deletes the data files (after they were copied off)
#
# and the arm calibration (the lab console's Calibration tab; the Chaser only, which has the arm):
#
#   arm-cal-check           what calibrate_arm.py needs here: python3 and its version, the Dynamixel SDK,
#                           the U2D2 at /dev/ttyUSB0 and access to it, its latency timer, the dialout group,
#                           programs holding the port, and whether a calibration is running
#   arm-cal-receive         unpacks a tar holding calibrate_arm.py (stdin) into ~/.spot/arm_calibration
#   arm-cal-start JOINTS TESTS [simulate]
#                           starts it in the background (--yes): JOINTS the servo IDs as digits ("123"),
#                           TESTS a comma list of breakaway, sweep, inertia, steps; simulate: its simulated
#                           servos (a rehearsal). Output in run.log, its exit code as a last line "EXIT n"
#   arm-cal-status          whether it is running, its run folder, its exit code, the end of run.log
#   arm-cal-stop            stops it (SIGTERM: it switches the servos' torque off and restores them)
#   arm-cal-data            the newest run's results (*.json, *.csv) as a tar on stdout
#
# NAME and FILE are checked strictly (letters, digits, _ . -), so nothing else can be run.
DEV=/dev/ttyACM0
BASE="$HOME/ExperimentSoftware/spot"
ARM_DEV="${SPOT_ARM_DEV:-/dev/ttyUSB0}"   # (SSH passes no environment to the forced command: always ttyUSB0 there)
ARM="$HOME/.spot/arm_calibration"

read -r -a ARGS <<< "$SSH_ORIGINAL_COMMAND"
CMD="${ARGS[0]}"
NAME="${ARGS[1]}"
FILE="${ARGS[2]}"

valid() { [[ "$1" =~ ^[A-Za-z0-9][A-Za-z0-9_.-]{0,99}$ ]] && [[ "$1" != *..* ]]; }
need_name() {
    valid "$NAME" || { echo "Not a valid experiment name: $NAME" >&2; exit 2; }
    DIR="$BASE/$NAME"
}
running_pid() {
    local pid
    [ -f "$DIR/run.pid" ] || return 1
    pid=$(cat "$DIR/run.pid")
    [[ "$pid" =~ ^[0-9]+$ ]] && kill -0 "$pid" 2>/dev/null && echo "$pid"
}
users_of() {   # PID NAME of each program with the port $1 open (only this user's are visible)
    find /proc/[0-9]*/fd -lname "$1" 2>/dev/null | cut -d/ -f3 | sort -u | while read -r pid; do
        name=$(tr '\0' '\n' < "/proc/$pid/cmdline" 2>/dev/null | head -n 1)
        echo "$pid ${name##*/}"
    done
}
arm_pid() { DIR="$ARM"; running_pid; }

case "$CMD" in
    status)
        if [ -e "$DEV" ]; then echo "teensy yes"; else echo "teensy no"; fi
        # Programs with the port open (only this user's are visible, which includes experiments)
        find /proc/[0-9]*/fd -lname "$DEV" 2>/dev/null | cut -d/ -f3 | sort -u | while read -r pid; do
            name=$(tr '\0' '\n' < "/proc/$pid/cmdline" 2>/dev/null | head -n 1)
            name=${name##*/}
            # stty is this script's own "stream" setting up the port
            [ "$name" = stty ] || echo "user $pid $name"
        done
        ;;
    stream)
        [ -e "$DEV" ] || { echo "No Teensy at $DEV" >&2; exit 3; }
        # Raw: the packets are binary, so no newline translation or echo
        stty -F "$DEV" 115200 raw -echo || exit 4
        # The name lets "status" tell the console's own link apart from other programs
        exec -a spot-teensy-link cat > "$DEV"
        ;;
    exp-receive)
        need_name
        if running_pid > /dev/null; then echo "$NAME is running here; stop it first" >&2; exit 4; fi
        rm -rf "$DIR.new" && mkdir -p "$DIR.new" || exit 5
        if ! tar -x -C "$DIR.new" --no-same-owner; then rm -rf "$DIR.new"; echo "Could not unpack the files" >&2; exit 5; fi
        if [ -d "$DIR" ]; then
            mv "$DIR"/ExpLog* "$DIR.new"/ 2>/dev/null   # keep data not copied off yet
            rm -rf "$DIR"
        fi
        mv "$DIR.new" "$DIR"
        chmod +x "$DIR"/*.elf 2>/dev/null
        for f in "$DIR"/*.elf; do
            [ -e "$f" ] && ldd "$f" 2>/dev/null | grep "not found" | sed -e 's/^[[:space:]]*/missing /' -e 's/ =>.*//'
        done
        echo "received $(find "$DIR" -type f | wc -l) files"
        ;;
    exp-start)
        need_name
        valid "$FILE" || { echo "Not a valid file name: $FILE" >&2; exit 2; }
        [ -x "$DIR/$FILE" ] || { echo "There is no $FILE in $DIR (deploy first)" >&2; exit 3; }
        if pid=$(running_pid); then echo "already running $pid"; exit 0; fi
        cd "$DIR" || exit 5
        # setsid: keeps running after this SSH session ends
        setsid nohup "./$FILE" > run.log 2>&1 < /dev/null &
        echo $! > run.pid
        echo "started $!"
        ;;
    exp-stop)
        need_name
        if ! pid=$(running_pid); then echo "not running"; exit 0; fi
        kill -TERM "$pid"
        for _ in $(seq 50); do kill -0 "$pid" 2>/dev/null || break; sleep 0.1; done
        if kill -0 "$pid" 2>/dev/null; then kill -KILL "$pid"; echo "killed $pid"; else echo "stopped $pid"; fi
        ;;
    exp-status)
        need_name
        if pid=$(running_pid); then echo "running $pid"; else echo "running no"; fi
        echo "data $(ls "$DIR"/ExpLog* 2>/dev/null | wc -l)"
        [ -f "$DIR/run.log" ] && tail -n 5 "$DIR/run.log" | sed 's/^/log /'
        ;;
    exp-data)
        need_name
        cd "$DIR" 2>/dev/null && ls ExpLog* > /dev/null 2>&1 || { echo "No data files on this platform" >&2; exit 3; }
        tar -c ExpLog*
        ;;
    exp-clean)
        need_name
        rm -f "$DIR"/ExpLog*
        echo "cleaned"
        ;;
    arm-cal-check)
        if command -v python3 > /dev/null; then
            echo "python $(python3 -c 'import sys; print("%d.%d.%d" % sys.version_info[:3])')"
            if python3 -c "import dynamixel_sdk" 2>/dev/null; then echo "sdk yes"; else echo "sdk no"; fi
        else
            echo "python no"; echo "sdk no"
        fi
        if [ -e "$ARM_DEV" ]; then echo "port yes"; else echo "port no"; fi
        if [ -r "$ARM_DEV" ] && [ -w "$ARM_DEV" ]; then echo "access yes"; else echo "access no"; fi
        lat="/sys/bus/usb-serial/devices/${ARM_DEV##*/}/latency_timer"
        if [ -r "$lat" ]; then echo "latency $(cat "$lat")"; else echo "latency unknown"; fi
        if id -nG | tr ' ' '\n' | grep -qx dialout; then echo "dialout yes"; else echo "dialout no"; fi
        users_of "$ARM_DEV" | sed 's/^/user /'
        if [ -f "$ARM/calibrate_arm.py" ]; then echo "script yes"; else echo "script no"; fi
        if pid=$(arm_pid); then echo "running $pid"; else echo "running no"; fi
        ;;
    arm-cal-receive)
        if arm_pid > /dev/null; then echo "A calibration is running; stop it first" >&2; exit 4; fi
        mkdir -p "$ARM" && rm -rf "$ARM/new" && mkdir "$ARM/new" || exit 5
        if ! tar -x -C "$ARM/new" --no-same-owner calibrate_arm.py; then rm -rf "$ARM/new"; echo "Could not unpack calibrate_arm.py" >&2; exit 5; fi
        mv "$ARM/new/calibrate_arm.py" "$ARM/calibrate_arm.py" && rm -rf "$ARM/new"
        echo "received calibrate_arm.py"
        ;;
    arm-cal-start)
        JOINTS="$NAME"; TESTS="$FILE"; MODE="${ARGS[3]}"
        [[ "$JOINTS" =~ ^[1-3]{1,3}$ ]] || { echo "JOINTS must be servo IDs 1-3, e.g. 123" >&2; exit 2; }
        [[ "$TESTS" =~ ^(breakaway|sweep|inertia|steps)(,(breakaway|sweep|inertia|steps)){0,3}$ ]] || {
            echo "TESTS must be a comma list of breakaway, sweep, inertia, steps" >&2; exit 2; }
        [ -z "$MODE" ] || [ "$MODE" = simulate ] || { echo "The third argument can only be simulate" >&2; exit 2; }
        [ -f "$ARM/calibrate_arm.py" ] || { echo "No calibrate_arm.py here (send it first)" >&2; exit 3; }
        if pid=$(arm_pid); then echo "already running $pid"; exit 0; fi
        if [ -z "$MODE" ]; then
            [ -e "$ARM_DEV" ] || { echo "No U2D2 at $ARM_DEV (plugged in, powered?)" >&2; exit 3; }
            held=$(users_of "$ARM_DEV")
            [ -z "$held" ] || { echo "The arm's port is in use by: $held" >&2; exit 4; }
        fi
        OPTS=(--joints $(echo "$JOINTS" | fold -w1) --tests ${TESTS//,/ })
        [ "$MODE" = simulate ] && OPTS+=(--simulate)
        cd "$ARM" || exit 5
        # setsid: keeps running after this SSH session ends. The shell around it ignores SIGTERM, so a stop
        # (SIGTERM to the group) reaches the script, which switches the servos off; the shell then logs its exit
        setsid nohup bash -c 'trap "" TERM; python3 -u calibrate_arm.py --yes --out runs "$@"; echo "EXIT $?"' \
            arm-cal "${OPTS[@]}" > run.log 2>&1 < /dev/null &
        echo $! > run.pid
        echo "started $!"
        ;;
    arm-cal-status)
        if pid=$(arm_pid); then echo "running $pid"; else echo "running no"; fi
        last=$(ls -1d "$ARM"/runs/*/ 2>/dev/null | tail -n 1)
        [ -n "$last" ] && echo "run $(basename "$last")"
        if [ -f "$ARM/run.log" ]; then
            code=$(grep -E '^EXIT [0-9]+$' "$ARM/run.log" | tail -n 1 | cut -d' ' -f2)
            [ -n "$code" ] && echo "exit $code"
            tail -n 40 "$ARM/run.log" | sed 's/^/log /'
        fi
        ;;
    arm-cal-stop)
        if ! pid=$(arm_pid); then echo "not running"; exit 0; fi
        kill -TERM -- "-$pid" 2>/dev/null || kill -TERM "$pid"
        for _ in $(seq 100); do kill -0 "$pid" 2>/dev/null || break; sleep 0.1; done
        if kill -0 "$pid" 2>/dev/null; then kill -KILL -- "-$pid" 2>/dev/null; echo "killed $pid"; else echo "stopped $pid"; fi
        ;;
    arm-cal-data)
        last=$(ls -1d "$ARM"/runs/*/ 2>/dev/null | tail -n 1)
        [ -n "$last" ] || { echo "No calibration run here" >&2; exit 3; }
        cd "$last" && tar -c --transform "s,^,$(basename "$last")/," -- *.json *.csv 2>/dev/null
        ;;
    *)
        echo "This key only accepts: status, stream, exp-receive, exp-start, exp-stop, exp-status, exp-data, exp-clean, arm-cal-check, arm-cal-receive, arm-cal-start, arm-cal-status, arm-cal-stop, arm-cal-data" >&2
        exit 2
        ;;
esac
