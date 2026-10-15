#!/usr/bin/env bash
# Copies the SPOT Console (webapp/, phasespace/, deploy/ of +spot/+console, and the arm calibration
# script, +spot/+arm/calibration/calibrate_arm.py, which the Calibration tab runs on the Chaser) to the Jetson TX2,
# compiles StreamData there, and (re)starts the webapp
# service so it runs at boot on http://<tx2>:8080. Safe to re-run after any change.
#   ./deploy_tx2.sh
#   TX2=user@host SSH_KEY=~/.ssh/other_key ./deploy_tx2.sh
set -euo pipefail
cd "$(dirname "$0")/.."   # +spot/+console

TX2="${TX2:-spot-tx2@192.168.1.116}"
SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
DEST="PhaseSpace_Server_SendAll"   # in the TX2 user's home folder
SERVICE="spot-phasespace-console"
ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
[ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)

echo "== Copying files to $TX2:~/$DEST"
# The console's three folders; --delete keeps each copy in sync. StreamData builds into output/.
rsync -a --delete --relative --exclude '__pycache__' -e "ssh ${ssh_opts[*]}" \
    webapp phasespace deploy \
    "$TX2:$DEST/"
rsync -a -e "ssh ${ssh_opts[*]}" ../+arm/calibration/calibrate_arm.py "$TX2:$DEST/arm_calibration/"

echo "== Compiling and starting the service on $TX2"
ssh "${ssh_opts[@]}" "$TX2" bash -s -- "$DEST" "$SERVICE" <<'REMOTE'
set -euo pipefail
dest="$1"; service="$2"
cd "$HOME/$dest"
# Files from before the console's folders were split (2026-10-08)
rm -rf StreamData.cc Makefile build_linux.sh libowl-src-410 Python
./phasespace/build_linux.sh
mkdir -p "$HOME/.config/systemd/user"
cp "deploy/$service.service" "$HOME/.config/systemd/user/"
systemctl --user daemon-reload
systemctl --user enable "$service" >/dev/null 2>&1
systemctl --user restart "$service"
sleep 2
systemctl --user --no-pager status "$service" | head -5
if [ "$(loginctl show-user "$(id -un)" -p Linger --value 2>/dev/null)" != "yes" ]; then
    loginctl enable-linger 2>/dev/null || {
        echo
        echo "NOTE: to start at boot without anyone logging in, run this once on the TX2:"
        echo "      sudo loginctl enable-linger $(id -un)"
    }
fi
REMOTE
echo "== Done: http://${TX2#*@}:8080"
