#!/usr/bin/env bash
# Lets the SPOT lab console on the TX2 drive RED/BLACK/BLUE: the Teensy (Hardware tab) and the
# experiments (copy, start, stop, collect data). Run from a computer whose SSH key the TX2 and the
# platforms already accept. Safe to re-run (also to upgrade from the Teensy-only version).
#   1. creates the TX2's own key, ~/.ssh/id_ed25519_spot_console, if it doesn't have one
#   2. on each platform: installs spot_agent.sh (next to this script) as ~/.spot/agent.sh, and adds the TX2's key
#      to ~/.ssh/authorized_keys restricted to that script (no shell, no forwarding)
#   3. checks from the TX2 that each platform answers
#
#   ./setup_hardware_link.sh                 # RED, BLACK and BLUE
#   ./setup_hardware_link.sh blue            # only some
#   TX2=user@host SSH_KEY=~/.ssh/other_key ./setup_hardware_link.sh
set -euo pipefail
cd "$(dirname "$0")"

TX2="${TX2:-spot-tx2@192.168.1.116}"
SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
CONSOLE_KEY=".ssh/id_ed25519_spot_console"   # on the TX2; must match HW_KEY in webapp/app.py
MARKER="spot-console-hw"                      # comment that identifies the key in authorized_keys
declare -A ADDRESS=([red]=192.168.1.110 [black]=192.168.1.111 [blue]=192.168.1.112)
boards=("$@")
[ ${#boards[@]} -eq 0 ] && boards=(red black blue)

ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
[ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)

echo "== TX2 ($TX2): console key"
pubkey=$(ssh "${ssh_opts[@]}" "$TX2" "
    [ -f ~/$CONSOLE_KEY ] || ssh-keygen -q -t ed25519 -N '' -C '$MARKER' -f ~/$CONSOLE_KEY
    cut -d' ' -f1,2 ~/$CONSOLE_KEY.pub")
echo "$pubkey $MARKER"

failed=()
for board in "${boards[@]}"; do
    address="${ADDRESS[$board]:-}"
    [ -z "$address" ] && { echo "Unknown board '$board' (use red, black, blue)"; exit 2; }
    user="spot-$board"
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
    echo "-- check from the TX2:"
    if ! ssh "${ssh_opts[@]}" "$TX2" "ssh -i ~/$CONSOLE_KEY -o IdentitiesOnly=yes -o BatchMode=yes \
            -o ConnectTimeout=5 -o StrictHostKeyChecking=accept-new $user@$address status"; then
        failed+=("$board")
    fi
done
echo
[ ${#failed[@]} -eq 0 ] && echo "All done." || { echo "Failed on: ${failed[*]}"; exit 1; }
