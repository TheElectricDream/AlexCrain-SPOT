#!/usr/bin/env python3
"""SPOT Console: compile StreamData, start/stop PhaseSpace streaming, and watch the spacecraft
and LEDs live in 3D (PhaseSpace tab); switch the pucks and thrusters on RED/BLACK/BLUE and run
thruster checks (Hardware tab).

Runs on Linux: on the lab console (the Jetson TX2, installed by deploy_tx2.sh as a service on
http://<tx2>:8080) or, for testing, on any Linux computer or WSL. With --local it is the local
console that SPOT starts from MATLAB (Simulation, Experiment and Results tabs).

    python3 webapp/app.py                  # opens http://localhost:8080 in your browser
    python3 webapp/app.py --port 9000      # use a different web port
    python3 webapp/app.py --host 0.0.0.0   # reachable from other computers (see the warning it prints)
    python3 webapp/app.py --no-browser     # don't open a browser tab

Needs only the Python standard library (3.6 or newer), plus g++ and make to compile.

The code is in the spot_console package next to this file, one module per area (see
spot_console/__init__.py); this file only starts it.

Viewer: StreamData is started as "StreamData <frequency> <monitor_port>", which makes it send a
copy of every packet to 127.0.0.1:<monitor_port>; this app listens there and forwards the data
to the browser.

Hardware: this app logs into each platform over SSH with its own key (~/.ssh/id_ed25519_spot_console,
set up by setup_hardware_link.sh), which can only run ~/.spot/teensy_link.sh there, and writes the
Teensy's command packets through it.
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from spot_console.server import main  # noqa: E402

if __name__ == "__main__":
    main()
