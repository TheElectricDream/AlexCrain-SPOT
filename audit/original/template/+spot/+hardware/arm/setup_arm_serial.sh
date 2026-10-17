#!/usr/bin/env bash
# Sets up the arm's U2D2 (FTDI FT232H, SERVO_PORT in Constants.m) on RED for spot.hardware.ArmPWM:
#   - udev rule /etc/udev/rules.d/00-spot-arm.rules: every FTDI serial adapter gets a short latency
#     timer, SERVO_LATENCY_TIMER_MS (the kernel's default is 16 ms: the adapter then holds the
#     servos' replies up to 16 ms, which is what made the old encoder reads miss), the U2D2's port
#     is read/write for everyone, and ModemManager leaves it alone
#   - the platform user is added to the dialout group
# The block also asks the adapter for that timer itself; this makes it permanent whatever the
# kernel allows a non-root program, and its Status output shows the timer it got. Safe to re-run.
# Asks for the board's sudo password (and its login password if your SSH key isn't installed there).
# The platforms' addresses, login names, the port and the timer come from Constants.m
# (constants.sh).
#   ./setup_arm_serial.sh          # RED
#   ./setup_arm_serial.sh black    # another board, if the arm moves
set -euo pipefail
source "$(dirname "$0")/../../+console/deploy/constants.sh"

# Each platform's address and login name (IP_*, USER_*); the U2D2's port and latency timer [ms].
declare -A ADDRESS LOGIN
for board in red black blue; do
    ADDRESS[$board]=$(spot_constant "IP_${board^^}")
    LOGIN[$board]=$(spot_constant "USER_${board^^}")
done
DEV=$(spot_constant SERVO_PORT)
LATENCY=$(spot_constant SERVO_LATENCY_TIMER_MS)
boards=("$@")
[ ${#boards[@]} -eq 0 ] && boards=(red)

# Runs as root on the board; $1 is the login user, $2 the U2D2's port, $3 the latency timer [ms]
remote() {
    set -euo pipefail
    user="$1"
    dev="$2"
    latency="$3"
    rule=/etc/udev/rules.d/00-spot-arm.rules
    cat > "$rule" <<RULE
# SPOT: FTDI serial adapters (the arm's U2D2) pass data on after $latency ms, not 16
ACTION=="add", SUBSYSTEM=="usb-serial", DRIVER=="ftdi_sio", ATTR{latency_timer}="$latency"
# SPOT: the U2D2 (FTDI FT232H, 0403:6014) is read/write for everyone, and ModemManager leaves it alone
KERNEL=="ttyUSB*", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6014", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
RULE
    echo "Wrote $rule"
    usermod -aG dialout "$user"
    echo "Added $user to dialout (takes effect at next login)"
    udevadm control --reload-rules

    # Apply to an adapter that is already plugged in, and check (skipped if something has it open)
    timer=/sys/bus/usb-serial/devices/${dev##*/}/latency_timer
    if [ ! -e "$dev" ]; then
        echo "No $dev now; the rule applies when the U2D2 is plugged in"
    elif command -v fuser >/dev/null && fuser -s "$dev"; then
        echo "$dev is in use; not testing (the rule applies at the next re-plug)"
    else
        udevadm trigger --action=add --subsystem-match=usb-serial
        udevadm trigger --action=change --subsystem-match=tty
        udevadm settle
        [ -e "$timer" ] && [ "$(cat "$timer")" != "$latency" ] && echo "$latency" > "$timer"
        lat=$(cat "$timer" 2>/dev/null || echo '?')
        mode=$(stat -c %a "$dev")
        if [ "$lat" = "$latency" ]; then
            echo "OK: $dev latency timer $lat ms, mode $mode"
        else
            echo "PROBLEM: $dev latency timer is $lat ms (mode $mode); is it the U2D2 (FTDI)?"
            exit 1
        fi
    fi
    ls -l /dev/ttyUSB* 2>/dev/null || true
}

failed=()
for board in "${boards[@]}"; do
    address="${ADDRESS[$board]:-}"
    [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
    user="${LOGIN[$board]}"
    echo "== ${board^^} ($user@$address)"
    if ! ssh -t -o ConnectTimeout=10 "$user@$address" \
            "sudo -p '[sudo] password for $user on ${board^^}: ' bash -c $(printf %q "$(declare -f remote); remote $user $DEV $LATENCY")"; then
        failed+=("$board")
    fi
done
echo
[ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
