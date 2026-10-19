# Receive Data from Orin

**Priority 22.** The vision computer (an NVIDIA Orin running the ZED2 camera's software) can send the Chaser its own estimate of the Target's pose; this subsystem receives it. It is an input for navigation code you may write; nothing in the template uses it beyond logging.

![Receive Data from Orin](images/sub_Receive_Data_from_Orin.png)

**Writes:** `VIS_Measured_States` (3), `VIS_Confidence` (1). **Reads:** `isSim`.

- **Data Store Read1** (`isSim`) → **If** `u1 == 0`: experiments only.
- **Subsystem** (the action):
  - **UDP Receive**: a MATLAB Function block: `[data, newPackets, age] = spot.hardware.udpReceive(36845, '0.0.0.0', 4, 'single')` (`spot.hardware.udpReceive`): the newest packet of four `single`s on UDP port 36845 (from any address); the packet count and age are terminated.
  - **Demux** `n=4` → four **Cast To Double** blocks: the first three (a pose, `x y θ`) are **Mux**ed and written to `VIS_Measured_States`; the fourth is written to `VIS_Confidence`.

Both stores are logged as `VIS_Measured_States` and `VIS_Confidence`. The other direction, the platforms' states to the Orin, is [Send Data to Orin](page:diagram-outgoing).
