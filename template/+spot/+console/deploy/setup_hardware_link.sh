#!/usr/bin/env bash
# Lets the SPOT lab console on the TX2 drive RED/BLACK/BLUE: the Teensy (Hardware tab) and the
# experiments (copy, start, stop, collect data). Run from a computer whose SSH key the TX2 and the
# platforms already accept. Safe to re-run (also to upgrade from the Teensy-only version).
#   1. creates the TX2's own key, ~/.ssh/id_ed25519_spot_console, if it doesn't have one
#   2. on each platform: installs spot_agent.sh (next to this script) as ~/.spot/agent.sh, and adds the TX2's key
#      to ~/.ssh/authorized_keys restricted to that script (no shell, no forwarding)
#   3. on each platform, with sudo (it asks for the platform's password): keeps it on the network, Wi-Fi power
#      saving off (a dozing adapter misses the TX2's ARP requests, and the platform looks switched off) and
#      the computer never suspending
#   4. checks from the TX2 that each platform answers
#
#   ./setup_hardware_link.sh                 # RED, BLACK and BLUE
#   ./setup_hardware_link.sh blue            # only some
#   TX2=user@host SSH_KEY=~/.ssh/other_key ./setup_hardware_link.sh
#   NO_POWER=1 ./setup_hardware_link.sh      # skip step 3 (no sudo)
# The computers' addresses and login names come from Constants.m (constants.sh).
set -euo pipefail
cd "$(dirname "$0")"
source ./constants.sh

TX2="${TX2:-$(spot_constant USER_LAB_CONSOLE)@$(spot_constant IP_LAB_CONSOLE)}"
SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
CONSOLE_KEY=".ssh/id_ed25519_spot_console"   # on the TX2; must match HW_KEY in webapp/app.py
MARKER="spot-console-hw"                      # comment that identifies the key in authorized_keys
# Each platform's address and login name (IP_*, USER_*).
declare -A ADDRESS LOGIN
for board in red black blue; do
    ADDRESS[$board]=$(spot_constant "IP_${board^^}")
    LOGIN[$board]=$(spot_constant "USER_${board^^}")
done
boards=("$@")
[ ${#boards[@]} -eq 0 ] && boards=(red black blue)

ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
[ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)

# Step 3, run on a platform (in its own session, with a terminal, so sudo can ask for the password).
# NetworkManager's wifi.powersave = 2 is "off" (Ubuntu ships this file with 3, "on"); it applies from the next
# reconnect, so iw switches it off now too (restarting NetworkManager would drop the Wi-Fi this runs over).
POWER_SCRIPT='set -e
sudo -v
conf=/etc/NetworkManager/conf.d/default-wifi-powersave-on.conf
if [ -d /etc/NetworkManager/conf.d ]; then
    printf "[connection]\nwifi.powersave = 2\n" | sudo tee "$conf" >/dev/null
    echo "Wi-Fi power saving: off in $conf"
fi
if command -v iw >/dev/null; then
    for dev in $(iw dev | awk "\$1 == \"Interface\" {print \$2}"); do
        sudo iw dev "$dev" set power_save off && echo "$dev: $(iw dev "$dev" get power_save)"
    done
fi
sudo systemctl mask sleep.target suspend.target hibernate.target hybrid-sleep.target >/dev/null 2>&1
echo "Suspend: off (sleep, suspend and hibernate masked)"'

echo "== TX2 ($TX2): console key"
pubkey=$(ssh "${ssh_opts[@]}" "$TX2" "
    [ -f ~/$CONSOLE_KEY ] || ssh-keygen -q -t ed25519 -N '' -C '$MARKER' -f ~/$CONSOLE_KEY
    cut -d' ' -f1,2 ~/$CONSOLE_KEY.pub")
echo "$pubkey $MARKER"

failed=()
warned=()   # linked, but power saving not switched off
for board in "${boards[@]}"; do
    address="${ADDRESS[$board]:-}"
    [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
    user="${LOGIN[$board]}"
    echo "== ${board^^} ($user@$address)"
    # The link script goes over stdin; the key line is passed as an argument
    if ! ssh "${ssh_opts[@]}" "$user@$address" bash -s -- "$pubkey" "$MARKER" <<REMOTE
set -euo pipefail
pubkey="\$1"; marker="\$2"
mkdir -p ~/.spot ~/.ssh
chmod 700 ~/.ssh
cat > ~/.spot/agent.sh <<'LINK'
$(cat spot_agent.sh)
LINK
chmod 755 ~/.spot/agent.sh
mkdir -p ~/ExperimentSoftware/spot
touch ~/.ssh/authorized_keys
cp ~/.ssh/authorized_keys ~/.ssh/authorized_keys.bak-spot-console
# Replace any earlier copy of the console key, keep every other line as it was
{ grep -v " \$marker\\\$" ~/.ssh/authorized_keys || true
  echo "restrict,command=\"\$HOME/.spot/agent.sh\" \$pubkey \$marker"; } > ~/.ssh/authorized_keys.new
chmod 600 ~/.ssh/authorized_keys.new
mv ~/.ssh/authorized_keys.new ~/.ssh/authorized_keys
echo "Installed ~/.spot/agent.sh and the console key (backup: ~/.ssh/authorized_keys.bak-spot-console)"
REMOTE
    then
        failed+=("$board"); continue
    fi
    if [ -z "${NO_POWER:-}" ]; then
        echo "-- power saving off (sudo may ask for $user's password on the platform)"
        ssh -t "${ssh_opts[@]}" "$user@$address" "bash -c $(printf %q "$POWER_SCRIPT")" ||
            { echo "   (not done: run it again for ${board}, or set it by hand)"; warned+=("$board"); }
    fi
    echo "-- check from the TX2:"
    if ! ssh "${ssh_opts[@]}" "$TX2" "ssh -i ~/$CONSOLE_KEY -o IdentitiesOnly=yes -o BatchMode=yes \
            -o ConnectTimeout=5 -o StrictHostKeyChecking=accept-new $user@$address status"; then
        failed+=("$board")
    fi
done
echo
[ ${#warned[@]} -eq 0 ] || echo "Power saving not switched off on: ${warned[*]}"
[ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
