#!/usr/bin/env bash
# Makes the Teensy's USB serial port (/dev/ttyACM0) on RED/BLACK/BLUE permanently usable without sudo:
#   - udev rule /etc/udev/rules.d/00-spot-teensy.rules: any PJRC Teensy (vendor 16c0) serial port gets
#     mode 0666 every time it appears, and ModemManager is told to leave it alone
#   - the platform user is added to the dialout group
# Safe to re-run. Asks for each board's sudo password (and its login password if your SSH key isn't
# installed there).
#   ./setup_teensy_serial.sh              # RED, BLACK and BLUE
#   ./setup_teensy_serial.sh black blue   # only some
set -euo pipefail

declare -A ADDRESS=([red]=192.168.1.110 [black]=192.168.1.111 [blue]=192.168.1.112)
boards=("$@")
[ ${#boards[@]} -eq 0 ] && boards=(red black blue)

# Runs as root on the board; $1 is the login user
remote() {
    set -euo pipefail
    user="$1"
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
    if [ ! -e /dev/ttyACM0 ]; then
        echo "No /dev/ttyACM0 now; the rule applies when the Teensy is plugged in"
    elif command -v fuser >/dev/null && fuser -s /dev/ttyACM0; then
        echo "/dev/ttyACM0 is in use; not testing (the rule applies at the next re-plug)"
    else
        chmod 600 /dev/ttyACM0
        udevadm trigger --action=change --subsystem-match=tty
        udevadm settle
        mode=$(stat -c %a /dev/ttyACM0)
        mm=$(udevadm info -q property -n /dev/ttyACM0 | grep -c '^ID_MM_DEVICE_IGNORE=1$' || true)
        if [ "$mode" = 666 ] && [ "$mm" = 1 ]; then
            echo "OK: /dev/ttyACM0 restored to 0666 by udev, ModemManager ignore set"
        else
            chmod 666 /dev/ttyACM0
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
    user="spot-$board"
    echo "== ${board^^} ($user@$address)"
    if ! ssh -t -o ConnectTimeout=10 "$user@$address" \
            "sudo -p '[sudo] password for $user on ${board^^}: ' bash -c $(printf %q "$(declare -f remote); remote $user")"; then
        failed+=("$board")
    fi
done
echo
[ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
