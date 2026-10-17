# The Vision Tab

The lab console's **Vision** tab streams the cameras plugged into the **Jetson Orin** (`192.168.1.115`, `Constants.IP_ORIN`): the ZED 2i stereo camera, the Intel RealSense L515 LiDAR camera, the iniVation DVXplorer event camera, and any USB camera. The Orin runs **SPOT Vision** ([spot_vision.py](file:spot_vision.py)), a small web service on port 8090 (`PORT_VISION`); the lab console only tells the page where it is ([vision.py](file:vision.py)), and the video goes from the Orin straight to your browser, so it never loads the TX2. The page's code is [vision.js](file:vision.js).

## Using it

1. **Computer Vision** (the card at the top left): the Orin's address and whether it answers, with the cameras it found. If it can't be reached, the card says why; SPOT Vision is installed and restarted with `deploy_orin.sh` ([Lab Administration](page:lab-admin)).
2. Pick a camera with the **tabs** above the picture, and a view where it has several (the ZED's left and right lenses, the L515's colour, depth and infrared, ...).
3. **Play** starts the stream. The **width** menu trades detail for frame rate (**Full** is the camera's own width). **Snapshot** opens one full-size frame in a new tab; **Fullscreen** fills the screen.
4. **Camera Settings** (the card on the left): the settings the camera's own library offers, with their ranges and defaults (exposure, gain, white balance, depth range, ...). A change applies at once if the camera is open and is kept on the Orin (`~/.spot_vision_settings.json`) for every time it opens. **Reset** goes back to the camera's defaults.

## Cameras and experiments

A camera is opened only while someone watches it (or takes a snapshot), and closed a few seconds after the last viewer leaves. The rest of the time it is free for an experiment's own vision code. A camera that another program holds shows as **busy**.

Each camera runs in a process of its own, with the Python its library needs: the ZED SDK's `pyzed` in the system Python; `pyrealsense2` 2.54 for the L515 (the last librealsense release that supports it; newer ones don't see it), in its own environment; `dv-processing` for the DVXplorer; OpenCV for any other USB camera. `deploy_orin.sh` says which of these it finds.

> [!TIP]
> Plug the cameras into a **USB 3** port. Over USB 2 they offer lower resolutions and frame rates; the tab warns when a camera is on USB 2.

## Settings

The Orin's address is a lab console setting (`vision`: `host` `192.168.1.115`, `port` 8090), kept in `~/.spot_phasespace_console.json` on the TX2. SPOT Vision's own API (`/api/cameras`, `/stream/<camera>/<view>.mjpg`, `/snapshot/...`) is described at the top of [spot_vision.py](file:spot_vision.py); `http://192.168.1.115:8090/` lists the cameras in a plain page for checking by hand.
