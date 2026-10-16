# Lab Console: the PhaseSpace Tab

The lab console's page (`http://192.168.1.116:8080`, **SPOT Hardware Interfaces**) opens on this tab: the motion-capture stream, what it sees on the table, and Interactive Mode. The code is [app.js](file:app.js) (the cards) and [interactive.js](file:interactive.js) in the page, [phasespace.py](file:phasespace.py) and [interactive.py](file:interactive.py) in the server.

## Compile

[StreamData](file:StreamData.cc) is a C++ program; this card builds it on the lab console: whether a compiler was found (**Re-Check** after installing one), the **Output Folder** (**Browse…** opens a folder dialog), the last build's result, and **Compile** (its output goes to the **Build** log at the bottom). Needed once, and after the source changes.

## Stream

- **Frequency** (Hz, 1–960): the PhaseSpace rate; the Experiment tab's **Start Cameras** uses the project's Data Rate instead.
- **Start Streaming** / **Stop Streaming**: runs StreamData, which connects to the PhaseSpace server, builds the three rigid bodies from the LED calibration (`tracker_positions_phasespace.txt` in its folder; the Experiment tab sends the project's), and sends a pose packet every frame to the platforms (`.110`, `.111`, `.112`) and to this console's **Viewer UDP Port**. The pill shows **Streaming** with the time and the packet rate, **Stopped**, or **Exited** with the reason; who started or stopped it is noted. Frequency and transport are locked while it streams.
- **Destinations**: extra computers to send the packets to (**Add**, or **+ This Computer** for the one you are browsing from); remove with ×.
- The hint about **UDP under WSL**: on a Windows laptop running the console under WSL 2 with NAT, the packets can't reach the laptop's own address.

## Telemetry

Each platform's x, y, θ and rates from the newest packet, and the visible LEDs as chips coloured by the platform they belong to. The 3D view shows the same live: the platforms where PhaseSpace sees them and their LEDs with drop lines to the table (**Viewing Options**: LED IDs, Labels, Axes, LED Height Lines, Use CAD, ZED2 Viewpoint, Table Slope, and Simulation to lay the last simulation over the live view). A **Waiting for Packets** overlay appears when nothing arrived for 2 s, with the reason.

## Interactive Mode

Flying the platforms with no MATLAB, by dragging goals on the table: its own page, [Interactive Mode](page:interactive-mode).

## Messages and the logs

The **Messages** panel shows the running experiment's messages (what each platform's program says). The **Build** and **Stream** logs at the bottom are StreamData's compiler and runtime output (▾ collapses them, **Clear** empties).

## Lab console settings

The folder, frequency, viewer port and transport are the lab console's own settings (`~/.spot_phasespace_console.json` on the TX2), not the project's. Installing or updating the lab console: [the deploy scripts](page:lab-admin).
