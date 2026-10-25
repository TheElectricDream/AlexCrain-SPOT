# Camera Recordings

An experiment can record the cameras plugged into the Jetson Orin: switch them on in the Experiment tab's **Recording** card ([The Experiment Tab](page:tab-experiment)). Every frame is timed on the **experiment clock**, the one the platforms run on and log as `Time_s` ([Logged Signals](page:logged-signals)), so a frame and a logged sample with the same time happened at the same moment. This page covers what is recorded, how the time is worked out, and how to read the files.

## Where they are

**Collect** copies the recording into the run, next to the platforms' data:

```
Saved Data/<run>/
  RED/  BLACK/  BLUE/             the platforms' ExpLog files and .mat (as before)
  cameras/
    README.txt                    what every file and column is
    session.json                  the cameras, their settings, the clock fits, any errors
    clock.csv                     the experiment clock as it reached the Orin
    <camera>/                     one folder per camera, named by its id (zed-…, thermal-1-2.3, …)
      frames.csv                  a row per frame
      …                           the camera's own files (below)
```

The copy goes straight from the Orin to your computer, after the experiment. Each file's size is checked, and then the Orin's copy is deleted, as Collect does with the platforms' files. If the copy fails, the recording stays on the Orin, and **Collect** tries again.

## What each camera records

Each camera records in its own library's format, so its tools can read the files:

| Camera | Files | What they hold |
|---|---|---|
| ZED 2i | `zed.svo2` (`zed.svo` before ZED SDK 4.1) | both images, every frame, compressed by the Orin's hardware encoder (the ZED tab's **SVO Compression**: H.264, H.265 or lossless). The ZED SDK replays it as if it were the camera, depth included. |
| RealSense L515 | `realsense_1.bag` | the colour and depth streams at their chosen modes, in librealsense's ROS bag format (LZ4-compressed). Large: raw colour and depth. |
| DVXplorer | `events.aedat4` | every event, in iniVation's AEDAT4 format (dv-processing). If dv-processing can't write AEDAT4, `events.bin` holds the raw event rows instead, and `events_bin.json` gives their layout. |
| ICI 9320P thermal | `thermal_raw.bin` | raw 14-bit counts: 240 × 320 little-endian `uint16` per frame, at the **Recording Rate** (30 fps by default; every frame is 59.6 fps). These are counts, not temperatures. |
| Any other USB camera | `video.avi` | MJPEG, at the camera's resolution and the **Recording Rate** |

A recording keeps the camera's own data, whatever is being watched on the Vision tab, and at the camera's settings (the Vision tab's Camera Settings, as kept on the Orin). While a camera records, its settings that would reopen it (the ZED's resolution, the RealSense's modes, a USB camera's size) are refused until the recording ends.

## frames.csv

| Column | |
|---|---|
| `frame` | the frame's index in the camera's file (a batch of events for the DVXplorer) |
| `orin_time` | when it was taken, on the Orin's monotonic clock [s]: from the camera's own time stamp where that is on the Orin's clock (the ZED, the RealSense's global time, the DVXplorer); for the thermal camera, the middle of its readout, on the sensor's own clock (below); else when it arrived |
| `arrival_time` | when it reached SPOT Vision [s, same clock] |
| … | the camera's own: `camera_time_ns` (ZED), `camera_time_ms`, `file`, `color_frame`, `depth_frame` (RealSense), `first_us`, `last_us`, `events` (DVXplorer), `sensor_frame` (the sensor's frame number: a gap is a frame not kept at the Recording Rate, or lost), `shutter` (`ok`, `flat`: shutter closed, `settling`) and `housing_c` (thermal) |
| `experiment_time` | the experiment clock [s]: 0 when the platforms left phase 0, negative before. Empty if the experiment never started (no clock reached the Orin) |
| `phasespace_time` | the PhaseSpace stream's packet time [s] |

Recording starts before the platforms' programs do, so the first frames have negative times: the start, the countdown, and phase 0. It stops when the experiment stops.

## How the time is worked out

The platforms take their experiment clock from the PhaseSpace stream ([PhaseSpace Camera Code](page:diagram-phasespace)). While cameras record, the lab console relays that clock from every packet to SPOT Vision (UDP 31545, a few dozen bytes a packet: [Network, Addresses and Ports](page:lab-network)). The Orin keeps each value with the time it arrived (`clock.csv`). When the recording stops, it fits a straight line, `clock = offset + slope × orin_time`, through those values:

- the slope by least squares (it is 1 to within the two computers' clock drift);
- the offset by the least delayed packets (the 90th percentile of `clock − slope × orin_time`), because a packet's clock is already out of date by its delay when it arrives.

Each frame's `orin_time` then gives its `experiment_time`. The line also covers the frames from before the countdown. `session.json` keeps the fits (`clock.experiment`, `clock.phasespace`: offset, slope, samples, and the spread of the packets' delays in ms), and `clock.csv` keeps the raw values, so you can fit them again your own way.

**Accuracy.** In tests, every frame's time was within 1 ms of the true clock, with up to 4 ms of random delay added to the packets. In the lab, add the camera's own latency (exposure and transfer: the camera's documentation) and, where a camera has no time stamp of its own, the time between capture and arrival.

**The thermal camera** sends no time stamps, so its SDK times the frames itself (`ici9320.timing`, SDK 0.2). It stamps each frame the moment its last bytes arrive. The sensor ends a frame every 16.78 ms on its own crystal, so a line fitted under those arrivals gives each frame's true end of readout, however late Python was to read it. When the recording stops, the fit is redone over the whole recording (`ici9320.retime`), which also numbers every frame (`sensor_frame`), lost ones included. `orin_time` is the middle of the frame's readout: the 16.4 ms readout ends at the frame's end, rows top to bottom. In tests on a simulated camera, recorded frames were within 0.25 ms of their true times, and every frame was numbered right, including after dropped frames, against arrivals up to 16 ms late.

## Reading them

**MATLAB**

```matlab
run = fullfile('Saved Data', 'MyDiagram_20261020_101500');
f = readtable(fullfile(run, 'cameras', 'thermal-1-2.3', 'frames.csv'));

% Thermal: frame k, raw counts
fid = fopen(fullfile(run, 'cameras', 'thermal-1-2.3', 'thermal_raw.bin'));
fseek(fid, (k - 1) * 240 * 320 * 2, 'bof');
counts = fread(fid, [320 240], '*uint16', 0, 'l')';
fclose(fid);

% A USB camera's video: frame k is row k of frames.csv
v = VideoReader(fullfile(run, 'cameras', 'uvc-video0', 'video.avi'));
img = read(v, k);

% The Chaser's x at a frame's time (each logged signal is a timeseries on the same clock)
data = load(fullfile(run, 'RED', 'RED_1.mat'));     % dataClass_rt (spot.experiment.decodeData)
x = data.dataClass_rt.RED_Px_m;
[~, i] = min(abs(x.Time - f.experiment_time(k)));
x.Data(i)
```

**Python**

```python
import numpy as np, pandas as pd
f = pd.read_csv(f"{run}/cameras/thermal-1-2.3/frames.csv")
counts = np.fromfile(f"{run}/cameras/thermal-1-2.3/thermal_raw.bin", "<u2").reshape(-1, 240, 320)
```

The ZED's SVO opens with the ZED SDK (`InitParameters.set_from_svo_file`; frame `k` with `set_svo_position(k)`); the RealSense bag with librealsense (`config.enable_device_from_file`); AEDAT4 with dv-processing (`dv.io.MonoCameraRecording`).

## Videos to watch

`spot.camera` turns each camera's files into an `.mkv` video next to them, with each frame's times stamped in its top-left corner: the experiment time `t`, the PhaseSpace time `PS` and the frame number, plus each camera's own details (the thermal camera's sensor frame, housing temperature and shutter; the RealSense's colour frame; the events in each event-camera frame).

```matlab
spot.camera.recordingToMkv(run)        % every camera of the run, by its kind
spot.camera.thermalToMkv(fullfile(run, 'cameras', 'thermal-1-2.3'))
spot.camera.zedToMkv(fullfile(run, 'cameras', 'zed-OV0001'), 'left')   % 'left', 'right', 'both'
spot.camera.usbToMkv(fullfile(run, 'cameras', 'uvc-video0'))           % video.avi or pattern.avi
spot.camera.realsenseToMkv(fullfile(run, 'cameras', 'realsense-1'))    % colour; ROS Toolbox
spot.camera.dvxplorerToMkv(fullfile(run, 'cameras', 'dvs-1'), 30)      % events in 1/30 s frames
```

Each video plays at the rate its camera recorded at. The thermal video is shown in grey (white hot) on one scale for the whole recording, at twice the sensor's size. The ZED's SVO and the event camera's AEDAT4 are read without their SDKs, though lossless SVOs, the older `zed.svo` and ZSTD-compressed AEDAT4 files still need them. All of these functions need [ffmpeg](https://ffmpeg.org) (with libass and libx264, as in most builds) on the PATH.

## Size and space

The Recording card estimates the size from each camera's rate × the experiment's length (its phases, plus 30 s for the start and the stop), against the Orin's free space. A camera's rate is a rough estimate from its settings until it has recorded once; then it is the rate it actually recorded at, until its settings change. The Start confirmation repeats the estimate. A recording that would not fit asks again, and SPOT Vision stops a recording itself when 1 GB is left (`session.json` says so).

Rough sizes: the thermal camera 4.6 MB/s at 30 fps; a ZED at HD720/30 fps in H.264, a few MB/s; a USB camera at 1280 × 720, a few MB/s; the RealSense L515 at full modes, about 100 MB/s, so choose smaller modes for long runs.

## Things to know

- A recorded camera is held by SPOT Vision from Start to Stop, so **your own code on the Orin can't open it** during the experiment. The Vision tab still streams it.
- Recording needs the lab console and SPOT Vision of the same version as your console. Otherwise the card says which one to update.
- The recording lives on the Orin (`~/spot_vision_recordings/<run>`) until Collect copies it. SPOT Vision's `/api/record` lists what is there ([The Vision Tab](page:tab-vision)).
