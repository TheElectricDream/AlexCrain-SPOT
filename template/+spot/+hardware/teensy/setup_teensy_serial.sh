#!/usr/bin/env bash
# Makes the Teensy's USB serial port (TEENSY_PORT in Constants.m) on RED/BLACK/BLUE permanently usable
# without sudo:
#   - udev rule /etc/udev/rules.d/00-spot-teensy.rules: any PJRC Teensy (vendor 16c0) serial port gets
#     mode 0666 every time it appears, and ModemManager is told to leave it alone
#   - the platform user is added to the dialout group
# Safe to re-run. Asks for each board's sudo password (and its login password if your SSH key isn't
# installed there).
#   ./setup_teensy_serial.sh              # RED, BLACK and BLUE
#   ./setup_teensy_serial.sh black blue   # only some
# The platforms' addresses, login names and the port come from Constants.m (constants.sh).
set -euo pipefail
source "$(dirname "$0")/../../+console/deploy/constants.sh"

# Each platform's address and login name (IP_*, USER_*), and the Teensy's port.
declare -A ADDRESS LOGIN
for board in red black blue; do
    ADDRESS[$board]=$(spot_constant "IP_${board^^}")
    LOGIN[$board]=$(spot_constant "USER_${board^^}")
done
DEV=$(spot_constant TEENSY_PORT)
boards=("$@")
[ ${#boards[@]} -eq 0 ] && boards=(red black blue)

# Runs as root on the board; $1 is the login user, $2 the Teensy's port
remote() {
    set -euo pipefail
    user="$1"
    dev="$2"
    rule=/etc/udev/rules.d/00-spot-teensy.rules
    cat > "$rule" <<RULE
# SPOT: Teensy (PJRC, vendor 16c0) USB serial is read/write for everyone, and ModemManager leaves it alone
ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789B]?", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
KERNEL=="ttyACM*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789B]?", MODE:="0666"
RULE
    echo "Wrote $rule"
    usermod -aG dialout "$user"
    echo "Added $user to dialout (takes effect at next login)"
    udevadm control --reload-rules

    # Apply to a Teensy that is already plugged in, and prove the rule works: drop the mode, then
    # let udev restore it. Skipped if something has the port open.
    if [ ! -e "$dev" ]; then
        echo "No $dev now; the rule applies when the Teensy is plugged in"
    elif command -v fuser >/dev/null && fuser -s "$dev"; then
        echo "$dev is in use; not testing (the rule applies at the next re-plug)"
    else
        chmod 600 "$dev"
        udevadm trigger --action=change --subsystem-match=tty
        udevadm settle
        mode=$(stat -c %a "$dev")
        mm=$(udevadm info -q property -n "$dev" | grep -c '^ID_MM_DEVICE_IGNORE=1$' || true)
        if [ "$mode" = 666 ] && [ "$mm" = 1 ]; then
            echo "OK: $dev restored to 0666 by udev, ModemManager ignore set"
        else
            chmod 666 "$dev"
            echo "PROBLEM: udev gave mode $mode, ModemManager ignore=$mm (port set to 0666 by hand)"
            exit 1
        fi
    fi
    ls -l /dev/ttyACM* 2>/dev/null || true
}

failed=()
for board in "${boards[@]}"; do
    address="${ADDRESS[$board]:-}"
    [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
    user="${LOGIN[$board]}"
    echo "== ${board^^} ($user@$address)"
    if ! ssh -t -o ConnectTimeout=10 "$user@$address" \
            "sudo -p '[sudo] password for $user on ${board^^}: ' bash -c $(printf %q "$(declare -f remote); remote $user $DEV")"; then
        failed+=("$board")
    fi
done
echo
[ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
