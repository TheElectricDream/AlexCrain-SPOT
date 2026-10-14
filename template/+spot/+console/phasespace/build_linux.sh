#!/usr/bin/env bash
# Builds ../output/linux-<arch>/StreamData and libowlsock.so (output/ in the console's folder) (from the PhaseSpace source in
# libowl-src-410). Needs g++ and make (Ubuntu/WSL: sudo apt install g++ make; Fedora: sudo dnf install gcc-c++ make).
set -euo pipefail
cd "$(dirname "$0")"
make -j"$(nproc)" "$@"
echo "Built ../output/linux-$(uname -m)/StreamData  (run: ../output/linux-$(uname -m)/StreamData <frequency>)"
