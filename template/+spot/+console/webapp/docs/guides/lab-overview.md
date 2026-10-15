# The Lab and Its Hardware

## The table

A granite table, 3.5 m × 2.4 m, levelled as well as granite allows. What is left of its slope still pulls a floating platform: the lab console's table slope calibration measures it, and simulations include it ([The Calibration Tab](page:tab-calibration)). Barriers around the edges keep a platform from leaving the table; a cover protects the surface when the lab is closed.

The **table frame** has its origin at the corner where the 3D view draws its axes, +x along the 3.5 m side, +y along the 2.4 m side, +z up ([Units, Frames and Names](page:conventions)). Positions, velocities and commanded forces in the code are in this frame.

## The platforms

Each platform is a 0.3 m cube in three levels, about 13 kg with a full air tank (the default mass properties: Chaser 13.75 kg, Target 13.22 kg, Obstacle 12.77 kg; [Mass Properties](page:math-mass-properties)).

| Part | What it is |
|---|---|
| **Air tank** | a carbon-fibre tank of compressed air (filled to about 4500 psi from the lab's compressor), on the lowest level |
| **Regulators** | the first stage brings the air down to 80 psi for the thrusters; a second brings it to 60 psi for the air bearings. Two gauges on the side show both |
| **Air bearings (pucks)** | three, under the platform, fed through one solenoid valve. With them on, the platform floats on a thin film of air, with almost no friction |
| **Thrusters** | eight nozzles, each with its own solenoid valve, pushing along the body's ±x and ±y (the numbering is `resources/ThrusterConfig.png`). Nominal thrust about 0.28 N each; the thruster calibration measures each one |
| **Jetson computer** | runs the program built from your diagram, reads the PhaseSpace stream, and talks to the Teensy and (on the Chaser) the arm |
| **Teensy** | a micro-controller on USB that opens the valves: it receives the duty cycles every step and turns them into 5 Hz PWM, and shuts every valve if no packet arrives for 2 s ([teensy_pwm_control.ino](file:teensy_pwm_control.ino)) |
| **PhaseSpace LEDs** | four on the top of each platform, powered by an LED driver, so the cameras can track it |
| **E-stop receiver** | cuts the power to the valves when the E-stop is pressed ([Safety and Emergencies](page:lab-safety)) |

The thrusters share one regulated supply, so each gives less thrust when several are open at once: about 11 % less per extra valve on the Obstacle ([The Thruster Mixer](page:math-mixer)).

**The Chaser** also carries the **robotic arm**: three Dynamixel MX servos (shoulder, elbow, wrist; IDs 1, 2, 3) on a U2D2 USB adapter, driven by torque ([The Arm](page:math-arm)), and a **ZED2** stereo camera facing +x. **The Target** carries the docking cup, which takes the Chaser's cone. Which docking parts and solar panels each platform carries, and where, is set on the [Designer](page:tab-designer).

## PhaseSpace

A PhaseSpace X2E motion-capture system: ten cameras around the table track the LEDs, each blinking its own pattern. The PhaseSpace server (`192.168.1.109`) does the tracking; **StreamData**, on the lab console, reads it, turns each platform's LEDs into a pose (using the marker file, each LED's offset from the platform's centre of gravity), and sends every frame to the platforms and the console, with the shared experiment clock ([The PhaseSpace Tab](page:tab-phasespace), [StreamData.cc](file:StreamData.cc)). It runs at the Data Rate, up to 960 Hz.

## The computers

| Computer | Address | Runs |
|---|---|---|
| Lab console, Jetson TX2 | `192.168.1.116` | the lab console (port 8080), StreamData, the experiments' deployment, the calibrations |
| PhaseSpace server | `192.168.1.109` | the motion capture |
| Chaser, Target, Obstacle | `192.168.1.110`, `.111`, `.112` | each platform's Jetson |
| Jetson Orin | `192.168.1.115` | SPOT Vision, the cameras (port 8090; [The Vision Tab](page:tab-vision)) |
| Ground station | `192.168.1.104` | the lab's desktop computer |

[Network, Addresses and Ports](page:lab-network) lists every port and stream.

## The cameras on the Orin

The Orin hosts the vision sensors the Vision tab streams: the ZED 2i stereo camera, the Intel RealSense L515 LiDAR camera, the iniVation DVXplorer event camera, and any USB camera.
