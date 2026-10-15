# spot.hardware: the blocks and the hardware

`+spot/+hardware/` is everything in MATLAB that touches the real hardware. Most of it is the code **behind the diagram's blocks**: System objects (MATLAB classes that become blocks) and `%#codegen` functions called from MATLAB Function blocks. In a desktop simulation they do nothing or record what they would have sent; in the program built for a platform they call the C/C++ in `csrc/`.

| | Block in the diagram | Page |
|---|---|---|
| `IdentifyPlatform` | *WhoAmI*: which platform the program runs on, from the hostname | [[page|spot-hardware-IdentifyPlatform]] |
| `phasespaceLatest` | *Receive PhaseSpace Data*: the newest PhaseSpace packet, every step | [[page|spot-hardware-phasespaceLatest]] |
| `experimentTime` | *Experiment Clock*: the shared experiment time from the PhaseSpace stream | [[page|spot-hardware-experimentTime]] |
| `ArmTorque` | *Dynamixel Arm (Torque)*: joint torques in, encoders and torques out | [[page|spot-hardware-ArmTorque]] |
| `udpSend`, `udpReceive` | the UDP links to and from the Orin (ZED camera computer) and the lab console | [[udpSend|spot-hardware-udpSend]], [[udpReceive|spot-hardware-udpReceive]] |
| `packCommand` | the 37-byte thruster/puck command for the Teensy | [[page|spot-hardware-packCommand]] |
| `debugMessages` | *Debugger*: each platform's message to the console's Messages panel | [[page|spot-hardware-debugMessages]] |
| `liveStream`, `streamName` | *Live Stream*: any signal to the console's Live Plot | [[liveStream|spot-hardware-liveStream]], [[streamName|spot-hardware-streamName]] |
| `CustomDataLogger` | *Data Logger*: every step's values to the platform's `ExpLog` file | [[page|spot-hardware-CustomDataLogger]] |
| `Debugging` | prints a value on the platform's console | [[page|spot-hardware-Debugging]] |
| `Udp`, `ipText` | the glue to `csrc/udp/spot_udp.c`; an IP as text | [[Udp|spot-hardware-Udp]], [[ipText|spot-hardware-ipText]] |

The C/C++ behind them, and the firmware:

| Folder | Page |
|---|---|
| `csrc/udp/spot_udp.c` | [[spot_udp.c|Hardware-spot-udp]] |
| `csrc/arm/arm_torque.cpp` | [[arm_torque.cpp|Hardware-arm-torque]] |
| `csrc/data_logger`, `csrc/debugging`, `csrc/identify_platform` | [[Data Logger, Debugging, WhoAmI (C++)|Hardware-small-blocks]] |
| `teensy/teensy_pwm_control.ino`, `teensy/setup_teensy_serial.sh` | [[Teensy Firmware|Hardware-Teensy-Firmware]], [[Serial Setup Scripts|Hardware-Serial-Setup]] |
| `arm/setup_arm_serial.sh` | [[Serial Setup Scripts|Hardware-Serial-Setup]] |

## How a System object becomes a block

`ArmTorque`, `CustomDataLogger`, `Debugging` and `IdentifyPlatform` derive from `matlab.System` (so a *MATLAB System* block can host them) and `coder.ExternalDependency` (so, when the diagram is built, their `updateBuildInfo` adds the C/C++ files to the compilation). Their `stepImpl`/`outputImpl` call the C functions through `coder.ceval` only when `coder.target` is non-empty, i.e. in generated code; in a desktop simulation they do nothing.

The `%#codegen` functions (`udpSend`, `phasespaceLatest`, `experimentTime`, `liveStream`, `debugMessages`) are called from MATLAB Function blocks; `coder.target('Rtw')` is true only in the generated code, so the same source does the simulation's part (recording for the console) in MATLAB and the hardware's part on the platform.

## The lab network

| Who | Address | Ports |
|---|---|---|
| Lab console (Jetson TX2) | 192.168.1.116 (`labConsoleAddress`) | 8080 web; receives 55001–55003 debug messages, 55011–55013 live streams, 48720 clocks; StreamData sends PhaseSpace on 31534 |
| Chaser / Target / Obstacle (Jetsons) | 192.168.1.110 / .111 / .112 | receive PhaseSpace 31534; Teensy over USB serial |
| Orin (ZED2 camera, on the Chaser) | – | UDP to/from the Chaser (*Send Data to Orin*, *Receive Data from Orin*) |
