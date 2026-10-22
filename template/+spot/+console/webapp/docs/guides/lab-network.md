# Network, Addresses and Ports

Everything in the lab is on one network, `192.168.1.x` (the lab's Wi-Fi, or a cable in the lab). Your computer only needs it for experiments, the live view and Lab Setup; simulations work anywhere.

## Addresses

| Device | Address | Login (for SSH) |
|---|---|---|
| Lab console (Jetson TX2) | `192.168.1.116` | `spot-tx2` |
| PhaseSpace server | `192.168.1.109` | |
| Chaser (RED) | `192.168.1.110` | `spot-red` |
| Target (BLACK) | `192.168.1.111` | `spot-black` |
| Obstacle (BLUE) | `192.168.1.112` | `spot-blue` |
| Jetson Orin (SPOT Vision) | `192.168.1.115` | `spot-vision` |
| Ground station | `192.168.1.104` | |

The lab console's address is a project setting (`labConsoleAddress`, the Experiment tab's **Lab Console (TX2) Address**): it is compiled into the programs, which send it their clocks, messages and streams, so build again after changing it. The platforms' addresses are lab console settings (its Hardware tab, **Address**), and so is the Orin's (`vision`).

## Ports and streams

| From → to | Port | What |
|---|---|---|
| browser → lab console | TCP 8080 | its page and API; `/api/events` is the event stream |
| browser → Orin | TCP 8090 | SPOT Vision: the camera list, settings and MJPEG video |
| browser → local console | TCP, a free port on 127.0.0.1 | the local console (with its session token) |
| MATLAB worker → local console | TCP, another free port on 127.0.0.1 | the worker's JSON commands |
| PhaseSpace server → StreamData | OWL API | the cameras' frames (TCP by default, or UDP; [The PhaseSpace Tab](page:tab-phasespace)) |
| StreamData → platforms | UDP 31535 → 31534 | every frame: poses, velocities, LEDs and the experiment clock |
| StreamData → lab console | UDP, the viewer port (31540 by default) | a copy of every frame, for the 3D view |
| platforms → lab console | UDP 48720 / 48721 / 48722 | each program's experiment clock (Chaser, Target, Obstacle) |
| | UDP 55001 / 55002 / 55003 | debug messages |
| | UDP 55011 / 55012 / 55013 | Live Stream signals |
| Chaser ↔ Orin | UDP 46875 / 36845 | the Chaser's states out, a vision estimate in |
| lab console → Orin | UDP 31545 | while cameras record: each PhaseSpace packet's experiment clock, for timing the frames ([Camera Recordings](page:camera-recordings)) |
| lab console → Orin | TCP 8090 | SPOT Vision: starting and stopping a recording with the experiment |
| Orin → your computer | TCP 8090 | Collect: the recordings, straight from the Orin |
| lab console → platforms | SSH 22 | `spot_agent.sh`: deploy, start, stop, status, data, the Teensy's stream |
| your computer → TX2 | SSH 22 | MATLAB's remote build; the deployment scripts |

## SSH keys

- `~/.ssh/id_ed25519_spot` on your computer: the lab's key, which the TX2, the Orin and the platforms accept. The deployment scripts and Lab Setup use it.
- `~/.ssh/id_ed25519_spot_console` on the TX2: the lab console's own key. Each platform accepts it only for one command, its agent (`spot_agent.sh`), so the lab console can drive the hardware but can't open a shell. `setup_hardware_link.sh` sets this up ([Lab Administration](page:lab-admin)).

Passwords and the router's login are kept by the lab's administrators, not in this documentation.
