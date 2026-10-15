# Serial Setup Scripts

Two scripts that make a platform's USB serial ports usable by the experiment program without root, by installing udev rules over SSH. Both are safe to re-run; both ask for the board's sudo password.

<!-- source: +spot/+hardware/teensy/setup_teensy_serial.sh -->
<!-- source: +spot/+hardware/arm/setup_arm_serial.sh -->

## `setup_teensy_serial.sh`: the Teensy on every platform

```
./+spot/+hardware/teensy/setup_teensy_serial.sh              # RED, BLACK and BLUE
./+spot/+hardware/teensy/setup_teensy_serial.sh black blue   # some
```

- **Lines 12–14.** The boards' addresses; all three by default.
- **Lines 17–52, `remote` (runs as root on the board).** Write `/etc/udev/rules.d/00-spot-teensy.rules`: any PJRC Teensy (USB vendor `16c0`, products `04[789B]?`) serial port gets mode 0666 whenever it appears, and ModemManager is told to ignore it (otherwise it probes the port and swallows packets). Add the platform user to `dialout`. Reload the rules. If a Teensy is plugged in and nobody has it open, prove the rule works: drop the mode to 600, trigger udev, check it came back as 666 with the ModemManager flag set (otherwise set 666 by hand and report a problem).
- **Lines 54–66.** For each board: `ssh -t` as `spot-<board>`, run `remote` under `sudo` (the function is shipped inline with `declare -f`), collect failures.

## `setup_arm_serial.sh`: the arm's U2D2 on the Chaser

```
./+spot/+hardware/arm/setup_arm_serial.sh          # RED
./+spot/+hardware/arm/setup_arm_serial.sh black    # if the arm moves to another platform
```

Same structure. The rule file `00-spot-arm.rules` does two things: every FTDI serial adapter (`ftdi_sio`) gets a **1 ms latency timer** on insertion (the kernel's default is 16 ms, which is what made the old encoder reads miss; see [[arm_torque.cpp|Hardware-arm-torque]]), and the U2D2 (FTDI FT232H, `0403:6014`) is mode 0666 and ignored by ModemManager. The check reads `/sys/bus/usb-serial/devices/ttyUSB0/latency_timer` back and insists on 1. The hardware block also asks for 1 ms itself at run time and reports the timer it got in its Status output; the rule makes it permanent whatever the kernel allows a non-root program.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/teensy/setup_teensy_serial.sh</b> (66 lines; click to show)</summary>

```bash
   1  #!/usr/bin/env bash
   2  # Makes the Teensy's USB serial port (/dev/ttyACM0) on RED/BLACK/BLUE permanently usable without sudo:
   3  #   - udev rule /etc/udev/rules.d/00-spot-teensy.rules: any PJRC Teensy (vendor 16c0) serial port gets
   4  #     mode 0666 every time it appears, and ModemManager is told to leave it alone
   5  #   - the platform user is added to the dialout group
   6  # Safe to re-run. Asks for each board's sudo password (and its login password if your SSH key isn't
   7  # installed there).
   8  #   ./setup_teensy_serial.sh              # RED, BLACK and BLUE
   9  #   ./setup_teensy_serial.sh black blue   # only some
  10  set -euo pipefail
  11  
  12  declare -A ADDRESS=([red]=192.168.1.110 [black]=192.168.1.111 [blue]=192.168.1.112)
  13  boards=("$@")
  14  [ ${#boards[@]} -eq 0 ] && boards=(red black blue)
  15  
  16  # Runs as root on the board; $1 is the login user
  17  remote() {
  18      set -euo pipefail
  19      user="$1"
  20      rule=/etc/udev/rules.d/00-spot-teensy.rules
  21      cat > "$rule" <<RULE
  22  # SPOT: Teensy (PJRC, vendor 16c0) USB serial is read/write for everyone, and ModemManager leaves it alone
  23  ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789B]?", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
  24  KERNEL=="ttyACM*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789B]?", MODE:="0666"
  25  RULE
  26      echo "Wrote $rule"
  27      usermod -aG dialout "$user"
  28      echo "Added $user to dialout (takes effect at next login)"
  29      udevadm control --reload-rules
  30  
  31      # Apply to a Teensy that is already plugged in, and prove the rule works: drop the mode, then
  32      # let udev restore it. Skipped if something has the port open.
  33      if [ ! -e /dev/ttyACM0 ]; then
  34          echo "No /dev/ttyACM0 now; the rule applies when the Teensy is plugged in"
  35      elif command -v fuser >/dev/null && fuser -s /dev/ttyACM0; then
  36          echo "/dev/ttyACM0 is in use; not testing (the rule applies at the next re-plug)"
  37      else
  38          chmod 600 /dev/ttyACM0
  39          udevadm trigger --action=change --subsystem-match=tty
  40          udevadm settle
  41          mode=$(stat -c %a /dev/ttyACM0)
  42          mm=$(udevadm info -q property -n /dev/ttyACM0 | grep -c '^ID_MM_DEVICE_IGNORE=1$' || true)
  43          if [ "$mode" = 666 ] && [ "$mm" = 1 ]; then
  44              echo "OK: /dev/ttyACM0 restored to 0666 by udev, ModemManager ignore set"
  45          else
  46              chmod 666 /dev/ttyACM0
  47              echo "PROBLEM: udev gave mode $mode, ModemManager ignore=$mm (port set to 0666 by hand)"
  48              exit 1
  49          fi
  50      fi
  51      ls -l /dev/ttyACM* 2>/dev/null || true
  52  }
  53  
  54  failed=()
  55  for board in "${boards[@]}"; do
  56      address="${ADDRESS[$board]:-}"
  57      [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
  58      user="spot-$board"
  59      echo "== ${board^^} ($user@$address)"
  60      if ! ssh -t -o ConnectTimeout=10 "$user@$address" \
  61              "sudo -p '[sudo] password for $user on ${board^^}: ' bash -c $(printf %q "$(declare -f remote); remote $user")"; then
  62          failed+=("$board")
  63      fi
  64  done
  65  echo
  66  [ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
```

</details>

<details>
<summary><b>+spot/+hardware/arm/setup_arm_serial.sh</b> (70 lines; click to show)</summary>

```bash
   1  #!/usr/bin/env bash
   2  # Sets up the arm's U2D2 (FTDI FT232H, /dev/ttyUSB0) on RED for spot.hardware.ArmTorque:
   3  #   - udev rule /etc/udev/rules.d/00-spot-arm.rules: every FTDI serial adapter gets a 1 ms latency
   4  #     timer (the kernel's default is 16 ms: the adapter then holds the servos' replies up to 16 ms,
   5  #     which is what made the old encoder reads miss), the U2D2's port is read/write for everyone,
   6  #     and ModemManager leaves it alone
   7  #   - the platform user is added to the dialout group
   8  # The block also asks the adapter for 1 ms itself; this makes it permanent whatever the kernel
   9  # allows a non-root program, and its Status output shows the timer it got. Safe to re-run. Asks
  10  # for the board's sudo password (and its login password if your SSH key isn't installed there).
  11  #   ./setup_arm_serial.sh          # RED
  12  #   ./setup_arm_serial.sh black    # another board, if the arm moves
  13  set -euo pipefail
  14  
  15  declare -A ADDRESS=([red]=192.168.1.110 [black]=192.168.1.111 [blue]=192.168.1.112)
  16  boards=("$@")
  17  [ ${#boards[@]} -eq 0 ] && boards=(red)
  18  
  19  # Runs as root on the board; $1 is the login user
  20  remote() {
  21      set -euo pipefail
  22      user="$1"
  23      rule=/etc/udev/rules.d/00-spot-arm.rules
  24      cat > "$rule" <<RULE
  25  # SPOT: FTDI serial adapters (the arm's U2D2) pass data on after 1 ms, not 16
  26  ACTION=="add", SUBSYSTEM=="usb-serial", DRIVER=="ftdi_sio", ATTR{latency_timer}="1"
  27  # SPOT: the U2D2 (FTDI FT232H, 0403:6014) is read/write for everyone, and ModemManager leaves it alone
  28  KERNEL=="ttyUSB*", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6014", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
  29  RULE
  30      echo "Wrote $rule"
  31      usermod -aG dialout "$user"
  32      echo "Added $user to dialout (takes effect at next login)"
  33      udevadm control --reload-rules
  34  
  35      # Apply to an adapter that is already plugged in, and check (skipped if something has it open)
  36      timer=/sys/bus/usb-serial/devices/ttyUSB0/latency_timer
  37      if [ ! -e /dev/ttyUSB0 ]; then
  38          echo "No /dev/ttyUSB0 now; the rule applies when the U2D2 is plugged in"
  39      elif command -v fuser >/dev/null && fuser -s /dev/ttyUSB0; then
  40          echo "/dev/ttyUSB0 is in use; not testing (the rule applies at the next re-plug)"
  41      else
  42          udevadm trigger --action=add --subsystem-match=usb-serial
  43          udevadm trigger --action=change --subsystem-match=tty
  44          udevadm settle
  45          [ -e "$timer" ] && [ "$(cat "$timer")" != 1 ] && echo 1 > "$timer"
  46          lat=$(cat "$timer" 2>/dev/null || echo '?')
  47          mode=$(stat -c %a /dev/ttyUSB0)
  48          if [ "$lat" = 1 ]; then
  49              echo "OK: /dev/ttyUSB0 latency timer 1 ms, mode $mode"
  50          else
  51              echo "PROBLEM: /dev/ttyUSB0 latency timer is $lat ms (mode $mode); is it the U2D2 (FTDI)?"
  52              exit 1
  53          fi
  54      fi
  55      ls -l /dev/ttyUSB* 2>/dev/null || true
  56  }
  57  
  58  failed=()
  59  for board in "${boards[@]}"; do
  60      address="${ADDRESS[$board]:-}"
  61      [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
  62      user="spot-$board"
  63      echo "== ${board^^} ($user@$address)"
  64      if ! ssh -t -o ConnectTimeout=10 "$user@$address" \
  65              "sudo -p '[sudo] password for $user on ${board^^}: ' bash -c $(printf %q "$(declare -f remote); remote $user")"; then
  66          failed+=("$board")
  67      fi
  68  done
  69  echo
  70  [ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
```

</details>
<!-- /listing -->
