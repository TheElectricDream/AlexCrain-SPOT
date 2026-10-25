% HARDWARE  Everything that talks to the real hardware (platform programs, PhaseSpace, Dynamixel arm, Teensy)
%
%   ArmPWM                 - The arm's Dynamixel servos driven by PWM: joint PWM in, encoders and applied PWM out.
%   CustomDataLogger       - Appends each step's logged values to the platform's ExpLog text file.
%   Debugging              - Prints its input on the platform's console, for debugging a running experiment.
%   debugMessages          - Each platform's debug message, for the SPOT Console's Messages panel.
%   experimentTime         - The experiment's time, the same on every platform, from the PhaseSpace stream.
%   IdentifyPlatform       - Which platform the program runs on, from the computer's hostname.
%   ipText                 - An IPv4 address as text for the C code: '192.168.1.116' from [192 168 1 116].
%   liveStream             - Send a signal to the SPOT Console's Live Plot, every step (the Live Stream block).
%   logVector              - The logged signals as one row of numbers, for the platform's data logger.
%   packCommand            - Command packet for a platform's Teensy (firmware: teensy/teensy_pwm_control.ino).
%   phasespaceLatest       - Newest PhaseSpace packet every model step (replaces the UDP Receive block).
%   phaseSpaceStates       - Each platform's measured state from a PhaseSpace packet.
%   sendExperimentTime     - This platform's experiment time, to the lab console, every step.
%   streamName             - The name a Live Stream block shows in the SPOT Console's Live Plot.
%   thrusterCommand        - The frame for this platform's Teensy: its own duty cycles and air bearings.
%   Udp                    - Connects csrc/udp/spot_udp.c to the generated code (used by the UDP functions here).
%   udpReceive             - Newest UDP packet every model step (replaces the UDP Receive block).
%   udpSend                - Send a signal as one UDP packet every model step (replaces the UDP Send block).
%
%   Not MATLAB, in this package folder too:
%     csrc/      C/C++ behind the blocks above (arm/arm_pwm.cpp for ArmPWM, WhoAmI, Data Logger,
%                Debugging, UDP)
%     teensy/    the platforms' Teensy firmware (teensy_pwm_control.ino) and setup_teensy_serial.sh
%     arm/       setup_arm_serial.sh (RED: the U2D2's 1 ms latency timer, permissions)
%   The blocks using these are part of the diagrams themselves (WhoAmI, Data Logger, the
%   Dynamixel Arm (PWM) block, Receive PhaseSpace Data, the Experiment Clock, every UDP link, and
%   the Linear diagram's Hardware variant). The lab console's side of the hardware (Teensy links
%   over SSH, deploying experiments) belongs to the console:
%   +interface/+console/webapp/spot_console/hardware.py and experiment.py, and
%   +interface/+console/deploy/spot_agent.sh on the platforms.
