#!/usr/bin/env bash
# Copies SPOT Vision (+interface/+console/vision: the Orin's cameras for the lab console's Vision tab) to the Jetson
# Orin and (re)starts its service, so it runs at boot on http://<orin>:8090. Safe to re-run after any change.
#   ./deploy_orin.sh
#   ORIN=user@host SSH_KEY=~/.ssh/other_key REALSENSE_PYTHON=/path/to/python DVS_PYTHON=/path/to/python \
#       THERMAL_PYTHON=/path/to/python ./deploy_orin.sh
# REALSENSE_PYTHON: a Python with pyrealsense2 2.54 (the last librealsense that supports the L515 LiDAR; the
# system's 2.57 doesn't see it). The default is the environment already on the Orin.
# DVS_PYTHON: a Python with iniVation's dv-processing, for the DVXplorer (python3 -m pip install --user dv-processing).
# The default is the system's python3.
# THERMAL_PYTHON: a Python with python-libusb1, pyusb, numpy and OpenCV, for the ICI 9320P thermal camera, whose SDK
# is copied with SPOT Vision (vision/vendor/ici9320; python3 -m pip install --user libusb1 pyusb). The default is the
# system's python3. The camera also needs SPOT Vision's udev rule (vision/99-ici-9320p.rules), which needs sudo: the
# script says when it is missing.
set -euo pipefail
cd "$(dirname "$0")/.."   # +interface/+console

ORIN="${ORIN:-spot-vision@192.168.1.115}"
SSH_KEY="${SSH_KEY:-$HOME/.ssh/id_ed25519_spot}"
REALSENSE_PYTHON="${REALSENSE_PYTHON:-/home/spot-vision/Documents/lidarLegends2026/.lidar_venv/bin/python}"
DVS_PYTHON="${DVS_PYTHON:-/usr/bin/python3}"
THERMAL_PYTHON="${THERMAL_PYTHON:-/usr/bin/python3}"
DEST="spot_vision"   # in the Orin user's home folder
SERVICE="spot-vision"
ssh_opts=(-o BatchMode=yes -o ConnectTimeout=10)
[ -f "$SSH_KEY" ] && ssh_opts+=(-i "$SSH_KEY" -o IdentitiesOnly=yes)

echo "== Copying SPOT Vision to $ORIN:~/$DEST"
rsync -a --delete --exclude '__pycache__' -e "ssh ${ssh_opts[*]}" vision/ "$ORIN:$DEST/"

echo "== Starting the service on $ORIN"
ssh "${ssh_opts[@]}" "$ORIN" bash -s -- "$DEST" "$SERVICE" "$REALSENSE_PYTHON" "$DVS_PYTHON" "$THERMAL_PYTHON" <<'REMOTE'
set -euo pipefail
dest="$1"; service="$2"; rs_python="$3"; dvs_python="$4"; thermal_python="$5"
cd "$HOME/$dest"
python3 -c "import cv2, pyzed.sl" 2>/dev/null || echo "NOTE: the system python3 lacks OpenCV or the ZED SDK's pyzed: the ZED won't stream"
if [ -x "$rs_python" ] && "$rs_python" -c "import pyrealsense2, cv2, numpy" 2>/dev/null; then
    echo "RealSense: $rs_python (pyrealsense2 $("$rs_python" -c 'import pyrealsense2 as rs; print(getattr(rs, "__version__", None) or getattr(rs.pyrealsense2, "__version__", "?"))' 2>/dev/null))"
else
    echo "NOTE: $rs_python can't import pyrealsense2, cv2 and numpy: a RealSense won't stream (set REALSENSE_PYTHON)"
    rs_python=/usr/bin/python3
fi
if [ -x "$dvs_python" ] && "$dvs_python" -c "import dv_processing, cv2, numpy" 2>/dev/null; then
    echo "DVXplorer: $dvs_python (dv-processing $("$dvs_python" -c 'import dv_processing as dv; print(getattr(dv, "__version__", "?"))' 2>/dev/null))"
else
    echo "NOTE: $dvs_python can't import dv_processing: a DVXplorer won't stream (on the Orin: $dvs_python -m pip install --user dv-processing)"
    dvs_python=/usr/bin/python3
fi
if [ -x "$thermal_python" ] && "$thermal_python" -c "import usb1, usb.core, cv2, numpy" 2>/dev/null; then
    echo "ICI 9320P: $thermal_python (python-libusb1 $("$thermal_python" -c 'import usb1; print(getattr(usb1, "__version__", "?"))' 2>/dev/null))"
else
    echo "NOTE: $thermal_python can't import usb1, usb (pyusb), cv2 and numpy: the thermal camera won't stream (on the Orin: $thermal_python -m pip install --user libusb1 pyusb)"
    thermal_python=/usr/bin/python3
fi
rule="99-ici-9320p.rules"
if ! cmp -s "$rule" "/etc/udev/rules.d/$rule"; then
    echo "NOTE: the thermal camera needs SPOT Vision's udev rule (once, and again when it changes). On the Orin:"
    echo "      sudo cp ~/$dest/$rule /etc/udev/rules.d/ && sudo udevadm control --reload-rules && sudo udevadm trigger"
    echo "      then replug the camera"
fi
if ! id -nG | grep -qw plugdev; then
    echo "NOTE: $(id -un) isn't in the plugdev group, which the thermal camera's udev rule lets in: sudo usermod -aG plugdev $(id -un)"
fi
mkdir -p "$HOME/.config/systemd/user"
sed -e "s|@REALSENSE_PYTHON@|$rs_python|" -e "s|@DVS_PYTHON@|$dvs_python|" -e "s|@THERMAL_PYTHON@|$thermal_python|" \
    "$service.service" > "$HOME/.config/systemd/user/$service.service"
systemctl --user daemon-reload
systemctl --user enable "$service" >/dev/null 2>&1
systemctl --user restart "$service"
sleep 2
systemctl --user --no-pager status "$service" | head -5
if [ "$(loginctl show-user "$(id -un)" -p Linger --value 2>/dev/null)" != "yes" ]; then
    loginctl enable-linger 2>/dev/null || {
        echo
        echo "NOTE: to start at boot without anyone logging in, run this once on the Orin:"
        echo "      sudo loginctl enable-linger $(id -un)"
    }
fi
REMOTE
echo "== Done: http://${ORIN#*@}:8090 (the lab console's Vision tab)"
