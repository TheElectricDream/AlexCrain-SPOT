# The Vision Tab

The lab console's **Vision** tab streams the cameras plugged into the **Jetson Orin** (`192.168.1.115`): the ZED 2i stereo camera, the Intel RealSense L515 LiDAR camera, the iniVation DVXplorer event camera, the ICI 9320P thermal camera, and any USB camera. The Orin runs **SPOT Vision** ([spot_vision.py](file:spot_vision.py)), a small web service on port 8090; the lab console only tells the page where it is ([vision.py](file:vision.py)), and the video goes from the Orin straight to your browser, so it never loads the TX2. The page's code is [vision.js](file:vision.js).

## Using it

1. **Computer Vision** (the card at the top left): the Orin's address and whether it answers, with the cameras it found. If it can't be reached, the card says why; SPOT Vision is installed and restarted with `deploy_orin.sh` ([Lab Administration](page:lab-admin)).
2. Pick a camera with the **tabs** above the picture, and a view where it has several (the ZED's left and right lenses, the L515's colour, depth and infrared, ...).
3. **Play** starts the stream. The **width** menu trades detail for frame rate (**Full** is the camera's own width). **Snapshot** opens one full-size frame in a new tab; **Fullscreen** fills the screen.
4. **Camera Settings** (the card on the left): the settings the camera's own library offers, with their ranges and defaults (exposure, gain, white balance, depth range, ...). A change applies at once if the camera is open and is kept on the Orin (`~/.spot_vision_settings.json`) for every time it opens. **Reset** goes back to the camera's defaults.

## Cameras and experiments

A camera is opened only while someone watches it (or takes a snapshot) or an experiment records it, and closed a few seconds after the last viewer leaves and the recording ends. The rest of the time it is free for an experiment's own vision code. A camera that another program holds shows as **busy**.

An experiment can **record** the cameras: the Experiment tab's Recording card picks them. SPOT Vision records each one on the Orin's own disk (`~/spot_vision_recordings/<run>/`), in its library's format, with every frame timed on the experiment's clock; Collect copies the recording into the run ([Camera Recordings](page:camera-recordings)). A recording follows the camera's settings here. The **Recording** group sets what only recording uses: the ZED's **SVO Compression**, and the thermal and USB cameras' **Recording Rate**. While a camera records, the settings that would reopen it are refused, and the RealSense keeps the streams it records (colour and depth), so its infrared view stays empty until the recording ends.

Each camera runs in a process of its own, with the Python its library needs: the ZED SDK's `pyzed` in the system Python; `pyrealsense2` 2.54 for the L515 (the last librealsense release that supports it; newer ones don't see it), in its own environment; `dv-processing` for the DVXplorer; for the thermal camera, its SDK (`ici9320`, copied with SPOT Vision) with `python-libusb1` and `pyusb`; OpenCV for any other USB camera. `deploy_orin.sh` says which of these it finds.

> [!TIP]
> Plug the cameras into a **USB 3** port. Over USB 2 they offer lower resolutions and frame rates; the tab warns when a camera is on USB 2. The thermal camera and ordinary USB cameras are USB 2 devices, so the tab doesn't warn about them.

## The thermal camera

The **ICI 9320P** (Infrared Cameras Inc.: 320 × 240 pixels, 7–14 µm, 60 Hz) streams through its own SDK, [ICI9320P](https://github.com/TheElectricDream/ICI9320P), a driver written for SPOT that needs no vendor software. The camera has no firmware of its own: the first time it opens after being plugged in, the SDK loads the firmware into it, and it comes back on the bus as another device (a second or two). It has no serial number, so SPOT Vision knows it by its USB port (`thermal-1-2.3`, say): plugged into another port, it starts again from the default settings.

Its one view, **Thermal**, shows the sensor's **raw counts**, not temperatures: the SDK has no radiometric calibration yet, so a count can't be read as °C, and the same scene gives different counts as the camera warms up. The settings, all under **Display**:

- **Palette**: Iron, White Hot, Black Hot, Inferno or Rainbow.
- **Automatic Range** (on by default): the palette runs from the picture's coldest to its hottest counts (0.5 % to 99.5 % of its pixels), smoothed so it doesn't flicker. Turned off, it runs from **Range From** to **Range To** instead, which hold it fixed, so a warm object stays the same colour as others come and go.
- **Histogram Equalisation**: spreads the colours by how many pixels have each count. It shows more detail, but the colours are no longer proportional to the counts.
- **Readout** (on by default): written along the bottom of the picture are the range in counts (the automatic one too, so you can copy it into Range From and To), the camera's housing temperature, and **SHUTTER** while it recalibrates.

Every 30 s while it warms up, and every 2 minutes or so once it is stable, the camera closes its shutter for a moment to recalibrate (a flat-field correction). The frames it sends with the shutter closed are skipped, so the picture holds still for a moment. Its SDK times every frame on the sensor's own clock, to a fraction of a millisecond, which an experiment's recording uses ([Camera Recordings](page:camera-recordings)). The camera bar also shows the **housing temperature**: the camera's own, not the scene's.

On the Orin the camera needs SPOT Vision's udev rule, which lets the `plugdev` group open it without root (installing it needs `sudo`; `deploy_orin.sh` says how). Without the rule, the tab says it has no permission. The SDK itself is in `+spot/+console/vision/vendor/ici9320`, copied unchanged from its repository; `vendor/README.md` says how to update it.

## Settings

The Orin's address is a lab console setting (`vision`: `host` `192.168.1.115`, `port` 8090), kept in `~/.spot_phasespace_console.json` on the TX2. SPOT Vision's own API (`/api/cameras`, `/stream/<camera>/<view>.mjpg`, `/snapshot/...`, and the recordings' `/api/record...`) is described at the top of [spot_vision.py](file:spot_vision.py), and each camera's worker in [vision_worker.py](file:vision_worker.py); `http://192.168.1.115:8090/` lists the cameras in a plain page for checking by hand.
