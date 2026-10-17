# Network, Addresses and Ports

Everything in the lab is on one network, `192.168.1.x` (the lab's Wi-Fi, or a cable in the lab). Your computer only needs it for experiments, the live view and Lab Setup; simulations work anywhere.

## Addresses

The addresses, logins and ports are the network section of `Constants` ([Constants](page:dev-constants)); the names in brackets are its properties, which the consoles read from the same file.

| Device | Address | Login (for SSH) |
|---|---|---|
| Lab console (Jetson TX2) | `192.168.1.116` (`IP_LAB_CONSOLE`) | `spot-tx2` (`USER_LAB_CONSOLE`) |
| PhaseSpace server | `192.168.1.109` (`IP_PHASESPACE_SERVER`) | |
| Chaser (RED) | `192.168.1.110` (`IP_RED`) | `spot-red` (`USER_RED`) |
| Target (BLACK) | `192.168.1.111` (`IP_BLACK`) | `spot-black` (`USER_BLACK`) |
| Obstacle (BLUE) | `192.168.1.112` (`IP_BLUE`) | `spot-blue` (`USER_BLUE`) |
| Jetson Orin (SPOT Vision) | `192.168.1.115` (`IP_ORIN`) | `spot-vision` (`USER_VISION`) |
| Ground station | `192.168.1.104` (`IP_SPARE`) | |

The lab console's address is a project setting (`labConsoleAddress`, the Experiment tab's **Lab Console (TX2) Address**): it is compiled into the programs, which send it their clocks, messages and streams, so build again after changing it. The platforms' addresses are lab console settings (its Hardware tab, **Address**), and so is the Orin's (`vision`).

## Ports and streams

| From → to | Port | What |
|---|---|---|
| browser → lab console | TCP 8080 (`PORT_LAB_CONSOLE`) | its page and API; `/api/events` is the event stream |
| browser → Orin | TCP 8090 (`PORT_VISION`) | SPOT Vision: the camera list, settings and MJPEG video |
| browser → local console | TCP, a free port on 127.0.0.1 | the local console (with its session token) |
| MATLAB worker → local console | TCP, another free port on 127.0.0.1 | the worker's JSON commands |
| PhaseSpace server → StreamData | OWL API (TCP 8000, `PORT_PHASESPACE_SERVER`) | the cameras' frames (TCP by default, or UDP; [The PhaseSpace Tab](page:tab-phasespace)) |
| StreamData → platforms | UDP 31535 → 31534 (`PORT_STREAMDATA`, `PORT_PHASESPACE_CLIENT`) | every frame: poses, velocities, LEDs and the experiment clock |
| StreamData → lab console | UDP, the viewer port (31540 by default, `PORT_PHASESPACE_MONITOR`) | a copy of every frame, for the 3D view |
| platforms → lab console | UDP 48720 / 48721 / 48722 (`PORT_EXP_CLOCK_RED/BLACK/BLUE`) | each program's experiment clock (Chaser, Target, Obstacle) |
| | UDP 55001 / 55002 / 55003 (`PORT_DEBUG_*`) | debug messages |
| | UDP 55011 / 55012 / 55013 (`PORT_LIVE_*`) | Live Stream signals |
| Chaser ↔ Orin | UDP 46875 / 36845 (`PORT_ORIN_STATES`, `PORT_ORIN_COMMANDS`) | the Chaser's states out, a vision estimate in |
| lab console → platforms | SSH 22 | `spot_agent.sh`: deploy, start, stop, status, data, the Teensy's stream |
| your computer → TX2 | SSH 22 | MATLAB's remote build; the deployment scripts |

## SSH keys

- `~/.ssh/id_ed25519_spot` on your computer: the lab's key, which the TX2, the Orin and the platforms accept. The deployment scripts and Lab Setup use it.
- `~/.ssh/id_ed25519_spot_console` on the TX2: the lab console's own key. Each platform accepts it only for one command, its agent (`spot_agent.sh`), so the lab console can drive the hardware but can't open a shell. `setup_hardware_link.sh` sets this up ([Lab Administration](page:lab-admin)).

Passwords and the router's login are kept by the lab's administrators, not in this documentation.
