classdef Constants
    %CONSTANTS Every parameter of SPOT, in one place.
    %   Constants (Constants.m, next to SPOT.m) holds every number that describes the lab, the
    %   platforms, the arm's servos, the network, the limits the consoles check against, and
    %   every choice this project makes: the Simulation tab's settings, the Spacecraft Designer's
    %   design and the lab calibration's values the project took. It is the project's only
    %   configuration file: there is no settings, design or calibration file beside it. Nothing
    %   else in SPOT types these numbers: the MATLAB code reads them here (Constants.NAME), the
    %   Simulink diagram's blocks reference them the same way, the local console's Python reads
    %   them from this MATLAB session (the MATLAB Engine for Python; SPOT shares the session),
    %   and the lab console (the TX2, no MATLAB) reads this file with spot_console/constants.py.
    %   The Teensy firmware and the platforms' C code can't read it: their copies name the
    %   constant they mirror, and Constants.verify compares them with this file.
    %
    %   The console writes this file: a value changed on the Simulation tab, in the Designer or
    %   by Update from Lab rewrites that constant's line (spot.params.writeConstants), keeping
    %   its comment. Edit it by hand too: MATLAB re-reads it when the file changes.
    %
    %   Rules (the Coding Standard, +spot/CODE_TEMPLATE.md):
    %     - UPPER_SNAKE_CASE, with the unit in brackets in the trailing comment.
    %     - Values are numbers, 'strings', true/false, NaN, row vectors [a b c] (matrices with
    %       ;, [] for none) or arithmetic on earlier constants (Constants.NAME) and pi, a
    %       scalar times a vector included: the lab console's reader parses no more than that.
    %       No structs, cells or function calls.
    %     - Per-platform values carry the platform's suffix: _RED, _BLACK, _BLUE (_ARM).
    %     - A value the lab measures (LAB_*) is written by Update from Lab; its placeholder
    %       (THRUST_NOMINAL, the ARM_* bench values) is what simulations use without one.
    %
    %   Sections: the project (the diagram, the platforms); the table and gravity; the network;
    %   the PhaseSpace packet; the platforms' bodies, scales, inertia and LEDs; the thrusters,
    %   the Teensy and the mixer; rates, phases and the solver; the poses; the LQR; the
    %   Dynamixel servos, the arm's bench calibration, geometry and controllers; the Spacecraft
    %   Designer; the calibrations' limits and defaults; Interactive Mode; the experiment's
    %   streams; the lab's calibration as this project took it.
    %
    %   Example
    %     g = Constants.TORQUE_PER_VOLT_MX64;     % [N m/V]
    %     s = Constants.all();                    % every constant, as a struct
    %     Constants.verify                         % the firmware's copies against this file
    %
    %   See also spot.params.fromConstants, spot.params.writeConstants, spot.arm.defaults,
    %   spot.params.apply.

    %% The project: the diagram and the platforms
    % What the Simulation tab simulates and the Experiment tab builds. The console writes these.
    properties (Constant)
        DIAGRAM = 'Template_v5_0_0_2026a_Jetson_PSClock.slx'; % the diagram (.slx); '' = the project's one
        USE_RED = true;                                 % the platforms taking part
        USE_BLACK = false;
        USE_BLUE = true;
        USE_ARM = true;                                 % the Chaser's arm (with USE_RED)
        REAL_TIME = false;                              % simulate at the clock's pace (Live Plot)
    end

    %% The table and gravity
    % The table top, measured; the height-surface fits and the guidance circles are about its
    % centre. Everything that draws or bounds the table derives from these two sides.
    properties (Constant)
        TABLE_X = 3.51155;                              % [m] along +x
        TABLE_Y = 2.41935;                              % [m] along +y
        TABLE_THICKNESS = 0.33;                         % [m] the top, as the 3D view draws it
        TABLE_CENTRE_X = Constants.TABLE_X / 2;    % [m] centre of the height surface
        TABLE_CENTRE_Y = Constants.TABLE_Y / 2;    % [m]
        % tableSlope of a level table: [a1 a2 a3 a4 a5 xc yc] (spot.sim.tableSlope).
        LEVEL_TABLE = [0, 0, 0, 0, 0, Constants.TABLE_CENTRE_X, ...
                       Constants.TABLE_CENTRE_Y];
        G = 9.80665;                                    % [m/s^2] standard gravity
        MRAD_PULL = Constants.G / 1000;            % [m/s^2] per mrad of tilt
    end

    %% The network
    % The lab's addresses (Units, Frames and Names in the documentation), the consoles' ports
    % and every UDP port of the experiment.
    properties (Constant)
        IP_LAB_CONSOLE = '192.168.1.116';               % the Jetson TX2
        IP_PHASESPACE_SERVER = '192.168.1.109';
        IP_RED = '192.168.1.110';                       % the Chaser
        IP_BLACK = '192.168.1.111';                     % the Target
        IP_BLUE = '192.168.1.112';                      % the Obstacle
        IP_SPARE = '192.168.1.104';                     % a fourth platform computer
        IP_ORIN = '192.168.1.115';                      % the Jetson Orin (SPOT Vision)
        BROADCAST_ADDRESS = '255.255.255.255';
        ANY_ADDRESS = '0.0.0.0';
        USER_LAB_CONSOLE = 'spot-tx2';                  % login names
        USER_RED = 'spot-red';
        USER_BLACK = 'spot-black';
        USER_BLUE = 'spot-blue';
        USER_VISION = 'spot-vision';
        % WhoAmI: each platform computer's number (csrc/identify_platform), 0 in a simulation.
        WHO_AM_I_SIMULATION = 0;
        WHO_AM_I_RED = 1;
        WHO_AM_I_BLACK = 2;
        WHO_AM_I_BLUE = 3;
        WHO_AM_I_SPARE = 4;
        WHO_AM_I_VISION = 5;
        PORT_LAB_CONSOLE = 8080;                        % TCP, the lab console's page
        PORT_VISION = 8090;                             % TCP, SPOT Vision on the Orin
        PORT_PHASESPACE_SERVER = 8000;                  % TCP, the OWL API
        PORT_STREAMDATA = 31535;                        % UDP, StreamData sends from here ...
        PORT_PHASESPACE_CLIENT = 31534;                 % UDP, ... to the platforms here
        PORT_PHASESPACE_MONITOR = 31540;                % UDP, the lab console's own copy
        PORT_EXP_CLOCK_RED = 48720;                     % UDP, each platform's experiment clock
        PORT_EXP_CLOCK_BLACK = 48721;
        PORT_EXP_CLOCK_BLUE = 48722;
        PORT_DEBUG_RED = 55001;                         % UDP, debug messages (debugMessages)
        PORT_DEBUG_BLACK = 55002;
        PORT_DEBUG_BLUE = 55003;
        PORT_DEBUG_LEGACY = 55000;                      % UDP, the old diagram's bare text
        PORT_LIVE_RED = 55011;                          % UDP, live streams (liveStream)
        PORT_LIVE_BLACK = 55012;
        PORT_LIVE_BLUE = 55013;
        PORT_HEARTBEAT_RED = 33001;                     % UDP, Check Connection broadcasts
        PORT_HEARTBEAT_BLACK = 33002;
        PORT_HEARTBEAT_BLUE = 33003;
        PORT_ORIN_COMMANDS = 36845;                     % UDP, from the Orin to RED
        PORT_ORIN_STATES = 46875;                       % UDP, RED's states to the Orin
        ORIN_COMMAND_LENGTH = 4;                        % singles in a packet from the Orin
        PHASESPACE_FREQUENCY = 100;                     % [Hz] the lab console starts the cameras at
        MAX_DATA_RATE = 960;                            % [Hz] PhaseSpace's fastest
    end

    %% The PhaseSpace packet
    % StreamData's UDP packet: PACKET_LENGTH float32 little-endian values. 1-based indices (the
    % C and Python code subtract 1): the time, then x, y [mm] and attitude [rad] for RED, BLACK
    % and BLUE, their rates, the LEDs' xyz and, in the last LED's x, the experiment clock.
    properties (Constant)
        PACKET_RIGID_FIELDS = 19;                       % time + 3 x (pose + rates)
        PACKET_LEDS = 32;                               % LED slots of 3 values each
        PACKET_LENGTH = Constants.PACKET_RIGID_FIELDS ...
                        + 3 * Constants.PACKET_LEDS;       % values in a packet (115)
        PACKET_BYTES_PER_VALUE = 4;                     % float32
        PACKET_TIME = 1;                                % index of the stream time
        PACKET_POSE_RED = 2;                            % index of x; y, attitude follow
        PACKET_POSE_BLACK = 5;
        PACKET_POSE_BLUE = 8;
        PACKET_RATE_RED = 11;                           % index of vx; vy, attitude rate follow
        PACKET_RATE_BLACK = 14;
        PACKET_RATE_BLUE = 17;
        PACKET_LED_START = 20;                          % index of LED 0's x
        PACKET_EXP_CLOCK = 113;                         % index of the experiment clock (LED 31's x)
        PACKET_SERVER_MARKERS = 24;                     % markers StreamData asks the server for
        EXP_CLOCK_MAX_BACKWARDS = 0.5;                  % [s] a clock stepping back more: a restart
    end

    %% The platforms: body, scales, inertia and LEDs
    % The 0.3 m body, its scale positions (the Mass Properties guide's COMConfig.png), the
    % thrusters' line of action and the LEDs' default places; each platform's scale readings
    % and bifilar pendulum test (2025-08-29), and the mass and inertia they gave.
    properties (Constant)
        BODY_HALF_SIDE = 0.15;                          % [m] body centre to a face
        THRUSTER_INSET = 0.081;                         % [m] a thruster's line in from the face
        THRUSTER_ARM = Constants.BODY_HALF_SIDE - Constants.THRUSTER_INSET;  % [m]
        NOZZLE_PROTRUSION = 0.1547;                     % [m] a nozzle's tip from the centre
        NOZZLE_HEIGHT = 0.0276;                         % [m] above the body's base
        LED_INSET = 0.013;                              % [m] LEDs in from the edges
        LED_DEFAULT_OFFSET = Constants.BODY_HALF_SIDE - Constants.LED_INSET;  % [m]
        LED_MAX_OFFSET = 0.3;                           % [m] an LED at most this far off centre
        % LED corners in the order (+x,+y), (+x,-y), (-x,-y), (-x,+y); ids as StreamData knows them.
        LED_CORNERS = [1 1; 1 -1; -1 -1; -1 1];
        LED_IDS_RED = [5 3 7 4];
        LED_IDS_BLACK = [13 11 9 15];
        LED_IDS_BLUE = [8 14 12 10];
        % Where each LED is on this project's platforms [m] from the body centre (4x2, one row
        % per LED in the order above); the Thruster and LED Layout window writes these.
        LED_XY_RED = Constants.LED_DEFAULT_OFFSET * Constants.LED_CORNERS;
        LED_XY_BLACK = Constants.LED_DEFAULT_OFFSET * Constants.LED_CORNERS;
        LED_XY_BLUE = Constants.LED_DEFAULT_OFFSET * Constants.LED_CORNERS;
        % Scale positions from the body centre: A left-middle, B right-back, C right-front.
        SCALE_A = [0, Constants.BODY_HALF_SIDE];                                 % [m]
        SCALE_B = [-Constants.BODY_HALF_SIDE, -Constants.BODY_HALF_SIDE];   % [m]
        SCALE_C = [Constants.BODY_HALF_SIDE, -Constants.BODY_HALF_SIDE];    % [m]
        SCALES_RED = [6.17 3.12 4.46];                  % [kg] readings A, B, C
        SCALES_BLACK = [6.14 3.07 4.01];                % [kg]
        SCALES_BLUE = [5.65 3.08 4.04];                 % [kg]
        BIFILAR_PERIOD_RED = 3.00;                      % [s] pendulum period
        BIFILAR_PERIOD_BLACK = 3.11;                    % [s]
        BIFILAR_PERIOD_BLUE = 3.02;                     % [s]
        BIFILAR_LENGTH = 2.56;                          % [m] string length
        BIFILAR_SPACING = 0.28;                         % [m] string spacing
        MASS_RED = 13.75;                               % [kg] (the scales' sum)
        MASS_BLACK = 13.22;                             % [kg]
        MASS_BLUE = 12.77;                              % [kg]
        % The bifilar results (spot.massprops.bifilarInertia with G), kept for the override.
        INERTIA_RED = 0.23535436237213403;              % [kg m^2] about the CG
        INERTIA_BLACK = 0.24318079735046158;            % [kg m^2]
        INERTIA_BLUE = 0.22150412995888016;             % [kg m^2]
        PLATFORM_CLEARANCE = 0.45;                      % [m] centre to centre, platforms apart
        MASS_OVERRIDE = false;                          % use MASS_* and INERTIA_* as typed, not the scales
    end

    %% The thrusters, the Teensy and the mixer
    % Eight thrusters, 1-2 along -x, 3-4 +y, 5-6 +x, 7-8 -y (ThrusterConfig.png). The Teensy
    % (teensy_pwm_control.ino) takes [SYNC_BYTE, 8 duty cycles, pucks] as float32 and switches
    % off after TEENSY_WATCHDOG without a packet. The mixer (Calculate Duty Cycle) iterates the
    % thrust decay and solves a box-constrained least squares.
    properties (Constant)
        NUM_THRUSTERS = 8;
        THRUSTER_DIRECTION_X = [-1 -1 0 0 1 1 0 0];     % each thruster's push along body x
        THRUSTER_DIRECTION_Y = [0 0 1 1 0 0 -1 -1];     % ... and body y
        THRUST_NOMINAL = 0.2825;                        % [N] one thruster, until calibrated
        THRUST_DECAY_DEFAULT = 0.112;                   % per extra valve (Obstacle, 2026-10-14)
        % This project's thrust of one thruster and thrust decay per platform (the Platforms
        % pane), used where there is no thruster calibration or CAL_USE_THRUSTERS is off.
        THRUST_RED = Constants.THRUST_NOMINAL;          % [N]
        THRUST_BLACK = Constants.THRUST_NOMINAL;
        THRUST_BLUE = Constants.THRUST_NOMINAL;
        THRUST_DECAY_RED = Constants.THRUST_DECAY_DEFAULT;
        THRUST_DECAY_BLACK = Constants.THRUST_DECAY_DEFAULT;
        THRUST_DECAY_BLUE = Constants.THRUST_DECAY_DEFAULT;
        VALVE_OPEN_TIME = 0.007;                        % [s] the shortest opening a valve gives
        TEENSY_PWM_FREQUENCY = 5;                       % [Hz] PWM_FREQUENCY in the firmware
        PWM_FREQUENCY = Constants.TEENSY_PWM_FREQUENCY; % [Hz] the diagram's PWMFreq: keep them equal
        TEENSY_PWM_TICK = 10000;                        % [Hz] PWM_TICK_HZ in the firmware
        TEENSY_SYNC_BYTE = 170;                         % 0xAA, SYNC_BYTE in the firmware
        TEENSY_PACKET_FLOATS = 9;                       % 8 duty cycles + pucks, NUM_FLOATS
        TEENSY_PACKET_BYTES = 1 + 4 * Constants.TEENSY_PACKET_FLOATS;   % 37
        TEENSY_WATCHDOG = 2;                            % [s] PACKET_TIMEOUT_MS / 1000
        TEENSY_PUCK_THRESHOLD = 0.5;                    % pucks on at or above this
        TEENSY_BAUD = 115200;
        TEENSY_PORT = '/dev/ttyACM0';                   % on each platform
        TEENSY_SEND_PERIOD = 0.2;                       % [s] the lab console re-sends at
        THRUSTER_CHECK_DUTY = 100;                      % [%] the Hardware tab's check, default
        THRUSTER_CHECK_TIME = 0.5;                      % [s] default
        THRUSTER_CHECK_TIME_RANGE = [0.2 10];           % [s]
        MIXER_MAX_ITERATIONS = 20;                      % mask: Maximum Number of Iterations
        MIXER_TOLERANCE = 1e-6;                         % mask: Convergence Tolerance (duty cycles)
        MIXER_CLS_MAX_ITERATIONS = 100;                 % the inner least squares
        MIXER_CLS_TOLERANCE = 1e-6;
        MIXER_CLS_ALPHA = 0.01;                         % first step
        MIXER_CLS_BETA = 0.5;                           % backtracking factor
        MIXER_CLS_LINE_SEARCH = 20;                     % backtracking steps at most
        MIXER_CLS_STEP_GROWTH = 1.1;                    % step growth after a success ...
        MIXER_CLS_STEP_CAP = 0.1;                       % ... to this at most
        MIXER_CLS_STEP_SHRINK = 0.9;                    % step shrink after a failure
    end

    %% Rates, phases and the solver
    % The diagram's step, the PhaseSpace rate the LQR is designed at, the default phase
    % durations (phase 3 is its four sub-phases) and the diagram's solver.
    properties (Constant)
        SAMPLE_RATE = 20;                               % [Hz] the diagram's step, baseRate = 1/this
        DATA_RATE = 10;                                 % [Hz] PhaseSpace rate for the LQR design
        PHASE0_DURATION = 10;                           % [s] Synchronization
        PHASE1_DURATION = 5;                            % [s] Start Floating
        PHASE2_DURATION = 40;                           % [s] Initial Position
        SUB_PHASE_DURATIONS = [10 10 30 120];           % [s] Experiment sub-phases 1-4
        PHASE4_DURATION = 30;                           % [s] Return Home
        PHASE5_DURATION = 20;                           % [s] Hold Home
        PHASE2_ON = true;                               % the segments that run (0, 1 and 5 always do)
        SUB_PHASES_ON = [true true true true];
        PHASE4_ON = true;
        SYNC_COUNTDOWN = 3;                             % [s] from every program running to the go
        SOLVER_TYPE = 'Fixed-step';
        SOLVER_FIXED_STEP = 'ode4';
        SOLVER_VARIABLE_STEP = 'VariableStepAuto';
        SOLVER_REL_TOL = '1e-3';
        SOLVER_ABS_TOL = 'auto';
        SOLVER_MAX_STEP = 'auto';
        SOLVER_MIN_STEP = 'auto';
        SOLVER_INITIAL_STEP = 'auto';
        LOWPASS_TIME_CONSTANT = 0.1;                    % [s] the measured states' low-pass
        MEASUREMENT_NOISE_VARIANCE = 1e-9;              % the simulated states' noise (diagram)
        GUIDANCE_CIRCLE_RADIUS = 0.85;                  % [m] the example guidance's circle
        GUIDANCE_CIRCLE_RATE = 0.03490659;              % [rad/s] 2 deg/s, as the diagram had it
    end

    %% Initial and home poses
    % [x y theta] [m m deg] in the settings; the arm's [q1 q2 q3] [deg].
    properties (Constant)
        INITIAL_POSE_RED = [2.61 1.21 0];               % [m m deg]
        INITIAL_POSE_BLACK = [1.33 1.95 120];
        INITIAL_POSE_BLUE = [1.33 0.47 240];
        HOME_POSE_RED = [2.61 1.21 0];                  % [m m deg]
        HOME_POSE_BLACK = [1.33 1.95 120];
        HOME_POSE_BLUE = [1.33 0.47 240];
        INITIAL_POSE_ARM = [90 90 0];                   % [deg]
    end

    %% The LQR
    % Weights Q = [x y theta dx/dt dy/dt dtheta/dt], R = [Fx Fy Tz], the same for each platform.
    properties (Constant)
        LQR_Q = [1 1 0.05 10 10 1];
        LQR_R = [6 6 6];
        % This project's weights per platform (the Control pane).
        LQR_Q_RED = Constants.LQR_Q;
        LQR_Q_BLACK = Constants.LQR_Q;
        LQR_Q_BLUE = Constants.LQR_Q;
        LQR_R_RED = Constants.LQR_R;
        LQR_R_BLACK = Constants.LQR_R;
        LQR_R_BLUE = Constants.LQR_R;
    end

    %% The Dynamixel servos
    % Protocol 2.0 units and limits (Dynamixel Units in the documentation), the MX-64(2.0)
    % datasheet at 12 V, and the servos' position mode as SPOT sets it for the calibration's
    % steps (spot.arm.compareSteps).
    properties (Constant)
        SERVO_IDS = [1 2 3];                            % shoulder, elbow, wrist
        SERVO_PORT = '/dev/ttyUSB0';                    % the U2D2 on the Chaser
        SERVO_BAUD = 1000000;
        SERVO_FACTORY_BAUD = 57600;                     % a reset or new servo answers here
        SERVO_LATENCY_TIMER_MS = 1;                     % [ms] the FTDI adapter's (kernel: 16)
        PULSES_PER_REV = 4096;                          % [pulses/rev] encoder
        CENTER_PULSE = 2048;                            % [pulses] joint straight out
        CMD_PULSES_PER_RAD = 651.74;                    % [pulses/rad] goal conversion (old hw code)
        CMD_RADPS_TO_RPM = 9.549296585513702;           % [rev/min / (rad/s)] old code's 60/(2 pi)
        CMD_RPM_TO_VEL_UNITS = 4.366756307;             % [units per rev/min] old code's 1/0.229
        VEL_UNIT_RPM = 0.229;                           % [rev/min] per velocity unit
        ACC_UNIT_RPM2 = 214.577;                        % [rev/min^2] per profile acceleration unit
        CURRENT_UNIT = 3.36e-3;                         % [A] per current unit
        PWM_FULL = 885;                                 % PWM at 100 % of the supply
        GAIN_SCALE_P = 128;                             % PWM = P/128 e + I/65536 sum e + D/16 de
        GAIN_SCALE_I = 65536;
        GAIN_SCALE_D = 16;
        GAIN_SCALE_FF = 4;
        MAX_GAIN = 16383;                               % the gain registers' range
        MAX_PROFILE = 32767;                            % the profile registers' range
        MIN_POSITION = 1024;                            % [pulses] -90 deg
        MAX_POSITION = 3072;                            % [pulses] +90 deg
        VELOCITY_LIMIT = 1023;                          % [velocity units]
        SERVO_PERIOD = 1e-3;                            % [s] the servo's control period
        MODE_VELOCITY = 1;                              % operating modes
        MODE_POSITION = 3;
        MODE_PWM = 16;
        SERVO_MODEL_MX64 = 311;                         % model numbers
        SERVO_MODEL_MX106 = 321;
        SERVO_MODEL_MX28 = 30;
        SERVO_MODEL_XM430 = 1020;
        SERVO_MODEL_XM540 = 1120;
        MX64_STALL_TORQUE = 6.0;                        % [N m] datasheet at 12 V
        MX64_NO_LOAD_RPM = 63;                          % [rev/min] datasheet at 12 V
        MX64_SUPPLY = 12;                               % [V]
        MX64_GEAR_RATIO = 200;
        TORQUE_PER_VOLT_MX64 = Constants.MX64_STALL_TORQUE ...
                               / Constants.MX64_SUPPLY;    % [N m/V] at the joint
        BACK_EMF_MX64_DATASHEET = Constants.MX64_SUPPLY ...
                                  / (Constants.MX64_NO_LOAD_RPM * 2 * pi / 60);   % [V s/rad]
        SERVO_POSITION_P = 400;                         % position mode, SPOT's settings
        SERVO_POSITION_I = 0;
        SERVO_POSITION_D = 200;
        SERVO_FEEDFORWARD_1 = 0;
        SERVO_FEEDFORWARD_2 = 0;
        SERVO_PROFILE_VELOCITY = 20;                    % [velocity units]
        SERVO_PROFILE_ACCELERATION = 0;                 % 0 = none
    end

    %% The arm: bench calibration, geometry and controllers
    % One MX-64(2.0) (ID 1, firmware 45) measured unloaded on the bench with calibrate_arm.py,
    % 2026-10-17, at 11.8 V, for every joint until the arm is calibrated: its model replayed
    % the servo's position steps to 0.15 deg rms. The Stribeck speed is the script's
    % convention (half the slowest steady speed; the joint stick-slips below 0.04 rad/s), and
    % the speed coefficient, 18 % below the datasheet's back-EMF, is taken as all back-EMF.
    % The links' geometry, masses and inertias are the CAD's.
    properties (Constant)
        ARM_BENCH_DATE = '2026-10-17';
        ARM_STATIC = 0.163;                             % [N m] breakaway (24 to 27 PWM)
        ARM_STATIC_POS = 0.167;                         % [N m] turning +
        ARM_STATIC_NEG = 0.160;                         % [N m] turning -
        ARM_COULOMB = 0.123;                            % [N m]
        ARM_STRIBECK = 0.021;                           % [rad/s]
        ARM_VISCOUS = 0;                                % [N m s/rad] besides the back-EMF
        ARM_BACK_EMF = 1.485;                           % [V s/rad] the measured speed coefficient
        ARM_ARMATURE = 0.0115;                          % [kg m^2] rotor and gears at the joint
        ARM_PLANT_INNER_STEP = 1e-3;                    % [s] spot.arm.Plant's internal step
        ARM_CALIBRATION_FORMAT = 2;                     % first calibration version in PWM control
        ARM_CAL_STEP_GOALS = [10 0 -10 20 0 -20 5 0];   % [deg] calibrate_arm.py's position steps
        ARM_CAL_STEP_TIME = 2.5;                        % [s] at each
        ARM_PHI = 1.2337865258868412;                   % [rad] shoulder angle from the centre (CAD)
        ARM_B0 = 0.2502067178238507;                    % [m] shoulder distance from the body centre
        ARM_MOUNT_FACE = '+y';                          % the usual mount
        ARM_MOUNT_SIDE = 'left';
        ARM_MOUNT_LATERAL = 0.082735;                   % [m] a mount's offset from its face centre
        ARM_A1 = 0.196822;                              % [m] joint to CoM, link 1
        ARM_B1 = 0.107678;                              % [m] CoM to next joint, link 1
        ARM_A2 = 0.198152;                              % [m]
        ARM_B2 = 0.106348;                              % [m]
        ARM_A3 = 0.062097;                              % [m]
        ARM_B3 = 0.025153;                              % [m]
        ARM_MASS_PROX = 0.345;                          % [kg]
        ARM_MASS_DIST = 0.335;                          % [kg]
        ARM_MASS_EE = 0.111;                            % [kg]
        ARM_INERTIA_PROX = 0.003704;                    % [kg m^2] about the link's CoM
        ARM_INERTIA_DIST = 0.003506;                    % [kg m^2]
        ARM_INERTIA_EE = 0.000106;                      % [kg m^2]
        % The diagram's Default Position/Velocity Control (ARM), retuned for SAMPLE_RATE.
        ARM_POSITION_P = [25 56 21];                    % [shoulder elbow wrist]
        ARM_POSITION_I = [10 10 10];
        ARM_POSITION_D = [1545 230 236];
        ARM_POSITION_PROFILE_VELOCITY = 20;             % [velocity units]
        ARM_POSITION_PROFILE_ACCELERATION = 0;
        ARM_POSITION_PWM_LIMIT = 80;                    % [PWM]
        ARM_VELOCITY_P = [160 70 30];
        ARM_VELOCITY_I = [220 1500 300];
        ARM_VELOCITY_PROFILE_ACCELERATION = 0;
        ARM_VELOCITY_PWM_LIMIT = Constants.PWM_FULL;
        % ARM_Control_Law_Enabler codes (1 - Experiment Logic).
        ARM_LAW_OFF = 0;
        ARM_LAW_POSITION = 1;
        ARM_LAW_CUSTOM = 2;
        ARM_LAW_VELOCITY = 3;
        ARM_PWM_ROUNDING = 0.5;                         % [PWM] a command this far off a unit rounds
    end

    %% The Spacecraft Designer
    % What is mounted on each platform (the Designer tab writes these): the dock, one of 'none',
    % 'male' (the cone), 'female' (the cup) or 'arm' (the Chaser only; its end effector is the
    % cone), on a side face ('+x', '+y' or '-y'; not the back, where the control panel is) and
    % its 'left' or 'right' place seen from outside; and the solar panels, over the top, along
    % the x or y faces. As in the lab: the Chaser's cone on its arm coupler, the Target's cup at
    % the same place. The Chaser's arm and ARM_MOUNT_FACE/SIDE follow each other in the console.
    properties (Constant)
        DOCK_RED = 'arm';
        DOCK_FACE_RED = '+y';
        DOCK_SIDE_RED = 'left';
        SOLAR_RED = false;
        SOLAR_AXIS_RED = 'x';
        DOCK_BLACK = 'female';
        DOCK_FACE_BLACK = '+y';
        DOCK_SIDE_BLACK = 'left';
        SOLAR_BLACK = false;
        SOLAR_AXIS_BLACK = 'x';
        DOCK_BLUE = 'none';
        DOCK_FACE_BLUE = '+y';
        DOCK_SIDE_BLUE = 'left';
        SOLAR_BLUE = false;
        SOLAR_AXIS_BLUE = 'x';
    end

    %% The calibrations: limits, defaults and fit thresholds
    % What a calibrated value may be (spot.params.loadCalibration, the lab console's file
    % checks), the Calibration tab's run defaults and the fits' thresholds (calibration.py,
    % calfit.py).
    properties (Constant)
        CAL_MIN_THRUST = 0.01;                          % [N] plausible calibrated thrust
        CAL_MAX_THRUST = 2;                             % [N]
        CAL_MAX_DECAY = 0.5;                            % per extra valve
        CAL_MAX_HEIGHT = 1;                             % height coefficients [m/m, 1/m] at most
        CAL_MAX_DAMPING = 10;                           % [1/s] air-bearing drag at most
        CAL_ARM_RANGE_FRICTION = [0 10];                % [N m, N m s/rad] static, coulomb, viscous
        CAL_ARM_RANGE_STRIBECK = [1e-4 10];             % [rad/s]
        CAL_ARM_RANGE_ARMATURE = [1e-6 1];              % [kg m^2]
        CAL_ARM_RANGE_BACK_EMF = [0 20];                % [V s/rad]
        CAL_GOAL_SPEED = 0.10;                          % [m/s] a goal moves to the next point at
        CAL_GOAL_TURN = 15;                             % [deg/s]
        CAL_SETTLED_POSITION = 0.10;                    % [m] at a point within this ...
        CAL_SETTLED_ATTITUDE = 5;                       % [deg] ... and this ...
        CAL_SETTLED_SPEED = 0.01;                       % [m/s] ... slower than this ...
        CAL_SETTLED_RATE = 2;                           % [deg/s] ... and this ...
        CAL_SETTLED_TIME = 1.5;                         % [s] ... for this long
        CAL_FLY_TIMEOUT = 90;                           % [s] to reach a point
        CAL_LOST_TIMEOUT = 1;                           % [s] PhaseSpace may lose a platform for
        CAL_STOP_PAUSE = 5;                             % [s] pucks off: the platform comes to rest
        CAL_DRIFT_LIFT = 5;                             % [s] pucks on before a drift is recorded
        CAL_NOISE_TIME = 2;                             % [s] the cameras' noise is recorded at rest
        CAL_MIN_NOISE_SAMPLES = 5;                      % frames the noise needs
        CAL_GLITCH_STEP = 0.02;                         % [m] a bigger jump between frames: a glitch
        CAL_GLITCH_FRACTION = 0.005;                    % more glitches than this: not saved
        CAL_POOR_FIT_FACTOR = 5;                        % residual over noise: the model doesn't fit
        CAL_POOR_FIT_FLOOR = 1;                         % [mm] at least
        CAL_REJECT_RESIDUAL = 20;                       % [mm] residual above this: not saved
        CAL_NOISE_FALLBACK = 0.2;                       % [mm] the cameras' noise when not measured
        CAL_MIN_DRIFT_SAMPLES = 20;                     % frames a drift needs
        CAL_SURFACE_COVERAGE = 0.4;                     % of the table each way for a quadratic fit
        CAL_SURFACE_MIN_DRIFTS = 3;                     % drifts a quadratic fit needs
        CAL_TABLE_GRID = [3 2];                         % columns x rows of drift points, default
        CAL_TABLE_GRID_MAX = [5 4];
        CAL_DRIFT_TIME = 12;                            % [s] default
        CAL_DRIFT_TIME_RANGE = [4 40];                  % [s]
        CAL_EDGE_MARGIN = 0.35;                         % [m] default
        CAL_EDGE_MARGIN_RANGE = [0.25 0.8];             % [m]
        CAL_GRID_INSET = 0.35;                          % [m] grid points this far inside the margin
        CAL_EDGE_EXTRA = 0.1;                           % [m] the first drift ends this much earlier
        CAL_MAX_DRIFT_SPEED = 0.15;                     % [m/s] default: a faster drift stops
        CAL_MAX_DRIFT_SPEED_RANGE = [0.03 0.3];         % [m/s]
        CAL_PATH_SPACING = 0.003;                       % [m] between the points of a drawn path
        CAL_BURN_REPEATS = 2;                           % default
        CAL_BURN_REPEATS_RANGE = [1 3];
        CAL_BURN_TIME = 2;                              % [s] default
        CAL_BURN_TIME_RANGE = [0.5 3];                  % [s]
        CAL_BURN_LATENCY = 0.15;                        % [s] from the command to the push
        CAL_BURN_REST = 5;                              % [s] at rest before a burn
        CAL_BURN_SETTLE = 0.2;                          % [s] after a burn
        % Other platforms this far from the table's centre, where burns start.
        CAL_BURN_CLEARANCE = Constants.PLATFORM_CLEARANCE + 0.15;   % [m]
        CAL_BURN_RMS_LIMIT = 3;                         % [mm] a burn's fit residual at most
        CAL_MIN_BURN_FRAMES = 10;
        CAL_MIN_FIT_THRUST = 0.02;                      % [N] a fitted thrust below is implausible
        CAL_MAX_FIT_THRUST = 1.5;                       % [N] ... above too
        CAL_DECAY_REPEATS = 1;                          % default, the Together stage
        CAL_DECAY_MIN_PUSH = 0.5;                       % of the single pushes' sum: a firing counts
        CAL_DECAY_MIN_FIRINGS = 3;
        CAL_ASSUMED_MASS_RANGE = [1 50];                % [kg] the thruster run's confirmed mass
        CAL_ASSUMED_INERTIA_RANGE = [0.01 5];           % [kg m^2]
        CAL_ASSUMED_CG_LIMIT = 60;                      % [mm] the CG at most this far off centre
        CAL_THRUST_WARN_FRACTION = 0.03;                % a thrust this far from nominal is marked
        CAL_THRUST_BAD_FRACTION = 0.1;                  % ... and this far, in red
        CAL_TORQUE_RATIO_WARN = 0.1;                    % torque ratio this far from 1 is marked
        CAL_ARM_MINUTES = [1.5 2.5 0.5 0.5];            % [min] a joint's four tests, in order
        CAL_DRIFT_OVERHEAD = 35;                        % [s] per drift point beyond the drift
        CAL_BURN_OVERHEAD = 20;                         % [s] per burn beyond the burn itself
    end

    %% Interactive Mode
    properties (Constant)
        INTERACTIVE_MARGIN = 0.2;                       % [m] goals stay this far inside the edges
        INTERACTIVE_STALE_HOLD = 0.25;                  % [s] no fresh pose: thrusters off
        INTERACTIVE_LOST_TIMEOUT = 1;                   % [s] no pose this long: the platform stops
        INTERACTIVE_HEARTBEAT_PERIOD = 0.5;             % [s] the flying page's heartbeat
        INTERACTIVE_HEARTBEAT_TIMEOUT = 3;              % [s] without one: everything stops
        INTERACTIVE_GOAL_PERIOD = 0.1;                  % [s] a dragged goal is sent this often
    end

    %% The experiment's streams
    properties (Constant)
        DEBUG_REPEAT_PERIOD = 0.5;                      % [s] a platform re-sends its message
        LIVE_PACKET_VERSION = 1;                        % spot.hardware.liveStream's format
    end

    %% The lab's calibration, as this project took it
    % The numbers the lab console measured (its Calibration tab), copied here by Update from Lab
    % (the Simulation tab's Calibration pane): the table's height surface and each platform's
    % air-bearing drag, each platform's eight thrusts and thrust decay, each joint's friction,
    % back-EMF and rotor inertia [shoulder elbow wrist]. [] or NaN: not measured. The raw runs
    % (drifts, burns, sweeps) stay on the lab console. CAL_USE_* say whether simulations use each
    % part; an OVERRIDE_* value (NaN: none) replaces the measured one while the part is used.
    % The sections' times are ISO 8601 UTC ('' = none).
    properties (Constant)
        CAL_USE_TABLE = true;                           % the measured slope and drag (off: level)
        CAL_USE_THRUSTERS = true;                       % the measured thrusts and decay (off: THRUST_*)
        CAL_USE_ARM = true;                             % the measured joints (off: the ARM_* bench values)
        LAB_SYNC_ASK = true;                            % offer the lab's calibration when it is newer
        LAB_SYNC_DECLINED = '';                         % the lab calibration's time declined with Not Now
        LAB_TABLE_UPDATED = '2026-10-14T14:52:08Z';
        LAB_TABLE_MODEL = 'quadratic';                  % 'quadratic' or 'plane'
        LAB_TABLE_HEIGHT = [-3.0271816981657004e-05 -3.601790542353335e-05 ... % [a1 a2 a3 a4 a5] about LAB_TABLE_CENTRE
                            -7.097653036182514e-06 -1.020925661181953e-05 1.541462231499134e-05];
        LAB_TABLE_CENTRE = [1.75 1.2];                  % [m] [xc yc]
        LAB_TABLE_DAMPING_RED = NaN;                    % [1/s] air-bearing drag
        LAB_TABLE_DAMPING_BLACK = NaN;
        LAB_TABLE_DAMPING_BLUE = 0;
        LAB_TABLE_TILT_CENTRE_MRAD = 0.0470496802802052; % [mrad] the slope at the centre (for display)
        LAB_TABLE_TILT_MAX_MRAD = 0.10047995863820404;  % [mrad]
        LAB_TABLE_DOWNHILL_DEG = 49.954097484440815;    % [deg] from +x
        LAB_THRUST_UPDATED_RED = '';
        LAB_THRUST_UPDATED_BLACK = '';
        LAB_THRUST_UPDATED_BLUE = '2026-10-14T16:01:40Z';
        LAB_THRUST_RED = [];                            % [N] thrusters 1-8
        LAB_THRUST_BLACK = [];
        LAB_THRUST_BLUE = [0.4236068394531058 0.44956738236908395 0.4458700991245686 ...
                           0.41298725924960106 0.4549356796566134 0.42997061286621885 ...
                           0.42129554713719425 0.4513611000444936];
        LAB_THRUST_DECAY_RED = NaN;                     % per extra valve open at once
        LAB_THRUST_DECAY_BLACK = NaN;
        LAB_THRUST_DECAY_BLUE = 0.1118794201729616;
        LAB_THRUST_ANGLE_RED = [];                      % [deg] how far each thrust is turned
        LAB_THRUST_ANGLE_BLACK = [];
        LAB_THRUST_ANGLE_BLUE = [0.25620485968328954 -0.03407973608628069 2.1206754720677194 ...
                                 3.692636566886689 1.892311928105987 -2.9313949612410948 ...
                                 3.551080492974271 3.0223470314028607];
        LAB_TORQUE_RATIO_RED = [];                      % each thruster's torque against the expected
        LAB_TORQUE_RATIO_BLACK = [];
        LAB_TORQUE_RATIO_BLUE = [0.8967376585751481 1.0935788236914519 0.9959286756260053 ...
                                 1.0079427060204047 1.1198719016044238 0.9482753710547183 ...
                                 0.8660302061449543 1.3542417114326435];
        LAB_ARM_UPDATED = '';
        LAB_ARM_STATIC = [NaN NaN NaN];                 % [N m] breakaway
        LAB_ARM_COULOMB = [NaN NaN NaN];                % [N m]
        LAB_ARM_STRIBECK = [NaN NaN NaN];               % [rad/s]
        LAB_ARM_VISCOUS = [NaN NaN NaN];                % [N m s/rad]
        LAB_ARM_BACK_EMF = [NaN NaN NaN];               % [V s/rad]
        LAB_ARM_TORQUE_PER_VOLT = [NaN NaN NaN];        % [N m/V]
        LAB_ARM_ARMATURE = [NaN NaN NaN];               % [kg m^2] rotor and gears at the joint
        OVERRIDE_TABLE_HEIGHT = [NaN NaN NaN NaN NaN];
        OVERRIDE_TABLE_DAMPING_RED = NaN;               % [1/s]
        OVERRIDE_TABLE_DAMPING_BLACK = NaN;
        OVERRIDE_TABLE_DAMPING_BLUE = NaN;
        OVERRIDE_THRUST_RED = [NaN NaN NaN NaN NaN NaN NaN NaN];   % [N]
        OVERRIDE_THRUST_BLACK = [NaN NaN NaN NaN NaN NaN NaN NaN];
        OVERRIDE_THRUST_BLUE = [NaN NaN NaN NaN NaN NaN NaN NaN];
        OVERRIDE_THRUST_DECAY_RED = NaN;
        OVERRIDE_THRUST_DECAY_BLACK = NaN;
        OVERRIDE_THRUST_DECAY_BLUE = NaN;
        OVERRIDE_ARM_STATIC = [NaN NaN NaN];            % [N m]
        OVERRIDE_ARM_COULOMB = [NaN NaN NaN];           % [N m]
        OVERRIDE_ARM_STRIBECK = [NaN NaN NaN];          % [rad/s]
        OVERRIDE_ARM_VISCOUS = [NaN NaN NaN];           % [N m s/rad]
        OVERRIDE_ARM_ARMATURE = [NaN NaN NaN];          % [kg m^2]
        OVERRIDE_ARM_BACK_EMF = [NaN NaN NaN];          % [V s/rad]
    end

    methods (Static)
        function s = all()
            %ALL Every constant, as a struct (the Python consoles read this through the engine).
            %   s = Constants.all() returns a struct with one field per constant property, in the
            %   file's order. The local console reads jsonencode(Constants.all()) from the shared
            %   MATLAB session; spot.params.writeConstants compares new values with it.
            %
            %   Outputs
            %     s  struct, NAME -> value
            %
            %   See also spot.params.fromConstants, spot.params.writeConstants.
            s = struct();
            for p = meta.class.fromName('Constants').PropertyList.'
                if p.Constant
                    s.(p.Name) = Constants.(p.Name);
                end
            end
        end

        function file = file()
            %FILE This class's file (the project's Constants.m).
            file = [mfilename('fullpath') '.m'];
        end

        function report = verify(root)
            %VERIFY The firmware's and the scripts' copies of these constants against this file.
            %   report = Constants.verify() checks the files that can't read this class
            %   (the Teensy firmware, the platforms' C code, StreamData, calibrate_arm.py and
            %   the setup scripts) for the values they mirror; each line of the report says
            %   same or DIFFERENT, and the function errors if any differ.
            %   report = Constants.verify(root) checks the project at root (default: this
            %   file's project).
            %
            %   Inputs
            %     root    project folder (optional)
            %
            %   Outputs
            %     report  cell array of lines, one per mirrored value
            %
            %   See also spot.control.verifyConsoleController.

            %% Defaults
            if nargin < 1
                root = fileparts(mfilename('fullpath'));
            end

            %% The mirrors: file, a regular expression with one token, the value expected
            C = Constants;
            hw = fullfile(root, '+spot', '+hardware');
            console = fullfile(root, '+spot', '+console');
            teensy = fullfile(hw, 'teensy', 'teensy_pwm_control.ino');
            armCpp = fullfile(hw, 'csrc', 'arm', 'src', 'arm_pwm.cpp');
            armCal = fullfile(root, '+spot', '+arm', 'calibration', 'calibrate_arm.py');
            stream = fullfile(console, 'phasespace', 'StreamData.cc');
            mirrors = {
                teensy, 'PWM_FREQUENCY\s*=\s*([\d.]+)f', C.TEENSY_PWM_FREQUENCY
                teensy, 'PWM_TICK_HZ\s*=\s*(\d+)', C.TEENSY_PWM_TICK
                teensy, 'SYNC_BYTE\s*=\s*0x([0-9A-Fa-f]+)', dec2hex(C.TEENSY_SYNC_BYTE)
                teensy, 'NUM_FLOATS\s*=\s*(\d+)', C.TEENSY_PACKET_FLOATS
                teensy, 'PACKET_TIMEOUT_MS\s*=\s*(\d+)', C.TEENSY_WATCHDOG * 1000
                teensy, 'NUM_THRUSTERS\s*=\s*(\d+)', C.NUM_THRUSTERS
                teensy, 'Serial\.begin\((\d+)\)', C.TEENSY_BAUD
                armCpp, 'MODE_PWM\s*=\s*(\d+)', C.MODE_PWM
                armCpp, 'pwmLimit\s*=\s*(\d+);', C.PWM_FULL
                armCpp, 'lastPos\[j\]\s*=\s*(\d+)', C.CENTER_PULSE
                armCal, 'PWM_FULL\s*=\s*(\d+)', C.PWM_FULL
                armCal, 'CENTER_PULSE\s*=\s*(\d+)', C.CENTER_PULSE
                armCal, 'PULSES\s*=\s*(\d+)\s*/', C.PULSES_PER_REV
                armCal, 'VEL_UNIT\s*=\s*([\d.]+)\s*\*', C.VEL_UNIT_RPM
                armCal, 'MODE_POSITION\s*=\s*(\d+)', C.MODE_POSITION
                armCal, 'MODE_PWM\s*=\s*(\d+)', C.MODE_PWM
                armCal, '311:\s*\(([\d.]+),\s*\d+\)', C.MX64_STALL_TORQUE
                armCal, '311:\s*\([\d.]+,\s*(\d+)\)', C.MX64_NO_LOAD_RPM
                armCal, 'DATASHEET_VOLTS\s*=\s*([\d.]+)', C.MX64_SUPPLY
                armCal, '"position_p":\s*(\d+)', C.SERVO_POSITION_P
                armCal, '"position_d":\s*(\d+)', C.SERVO_POSITION_D
                armCal, '"profile_velocity":\s*(\d+)', C.SERVO_PROFILE_VELOCITY
                armCal, '"--step-time",\s*type=float,\s*default=([\d.]+)', C.ARM_CAL_STEP_TIME
                armCal, '"--port",\s*default="([^"]+)"', C.SERVO_PORT
                armCal, '"--baud",\s*type=int,\s*default=(\d+)', C.SERVO_BAUD
                stream, 'NUM_RIGID_FIELDS\s*=\s*(\d+)', C.PACKET_RIGID_FIELDS
                stream, 'NUM_LEDS\s*=\s*(\d+)', C.PACKET_LEDS
                stream, 'NUM_SERVER_MARKERS\s*=\s*(\d+)', C.PACKET_SERVER_MARKERS
                stream, 'port_number\s*=\s*(\d+)', C.PORT_STREAMDATA
                stream, 'client_port\s*=\s*(\d+)', C.PORT_PHASESPACE_CLIENT
                stream, 'string address = "([^"]+)"', C.IP_PHASESPACE_SERVER
            };

            %% Compare
            report = {};
            ok = true;
            for k = 1:size(mirrors, 1)
                [file, pattern, expected] = mirrors{k, :};
                [~, name, ext] = fileparts(file);
                label = sprintf('%s%s: %s', name, ext, strtok(pattern, '\('));
                if ~isfile(file)
                    report{end + 1} = ['MISSING: ' file]; %#ok<AGROW>
                    ok = false;
                    continue
                end
                found = regexp(fileread(file), pattern, 'tokens', 'once');
                if isempty(found)
                    report{end + 1} = ['NOT FOUND: ' label]; %#ok<AGROW>
                    ok = false;
                elseif isnumeric(expected)
                    if abs(str2double(found{1}) - expected) <= 1e-9 * max(1, abs(expected))
                        report{end + 1} = sprintf('same: %s = %g', label, expected); %#ok<AGROW>
                    else
                        report{end + 1} = sprintf('DIFFERENT: %s is %s, Constants says %g', ...
                                                  label, found{1}, expected); %#ok<AGROW>
                        ok = false;
                    end
                else
                    if strcmpi(found{1}, expected)
                        report{end + 1} = sprintf('same: %s = %s', label, expected); %#ok<AGROW>
                    else
                        report{end + 1} = sprintf('DIFFERENT: %s is %s, Constants says %s', ...
                                                  label, found{1}, expected); %#ok<AGROW>
                        ok = false;
                    end
                end
            end

            %% The lab console's reader
            % The TX2 has no MATLAB: spot_console/constants.py parses this file. Read it here
            % the same way and compare every value, so a line outside its grammar is caught
            % before deploy_tx2.sh sends the file.
            [pyOk, pyLines] = Constants.verifyReader(root);
            report = [report, pyLines];
            ok = ok && pyOk;

            if nargout == 0
                fprintf('%s\n', report{:});
            end
            if ~ok
                error('spot:Constants:verify', ...
                      'A mirrored constant differs from Constants (see the report).');
            end
        end

        function [ok, lines] = verifyReader(root)
            %VERIFYREADER The Python reader's values of this file against MATLAB's.
            %   [ok, lines] = Constants.verifyReader(root) runs spot_console/constants.py on the
            %   project's Constants.m with the Python the console uses, and compares what it read
            %   with Constants.all(). Without a Python, one line says so and ok is true.
            %
            %   Inputs
            %     root   project folder (default: this file's)
            %
            %   Outputs
            %     ok     whether every value agrees (or Python is missing)
            %     lines  cell array of report lines
            %
            %   See also Constants.verify.

            %% Constants
            TOLERANCE = 1e-9;   % relative, for numbers

            if nargin < 1
                root = fileparts(mfilename('fullpath'));
            end
            lines = {};
            try
                python = spot.console.findPython();
            catch
                lines{end + 1} = 'python reader: not checked (no Python 3 found)';
                ok = true;
                return
            end
            webapp = fullfile(root, '+spot', '+console', 'webapp');
            cmd = sprintf('%s "%s" --json "%s"', python, ...
                          fullfile(webapp, 'spot_console', 'constants.py'), ...
                          fullfile(root, 'Constants.m'));
            [status, out] = system(cmd);
            if status ~= 0
                lines{end + 1} = sprintf('DIFFERENT: python reader failed: %s', strtrim(out));
                ok = false;
                return
            end
            read = jsondecode(out);
            mine = Constants.all();
            ok = true;
            for f = fieldnames(mine).'
                name = f{1};
                if ~isfield(read, name)
                    lines{end + 1} = sprintf('DIFFERENT: python reader has no %s', name); %#ok<AGROW>
                    ok = false;
                    continue
                end
                if ~Constants.sameValue(mine.(name), read.(name), TOLERANCE)
                    lines{end + 1} = sprintf('DIFFERENT: python reader reads %s as %s', ...
                                             name, jsonencode(read.(name))); %#ok<AGROW>
                    ok = false;
                end
            end
            if ok
                lines{end + 1} = sprintf('same: python reader, %d constants', ...
                                         numel(fieldnames(mine)));
            end
        end

        function same = sameValue(a, b, tolerance)
            %SAMEVALUE Whether a value read back (through JSON) equals the constant.
            %   Numbers within tolerance (NaN equal to NaN, [] to []), logicals and text equal;
            %   a row vector against a column (jsondecode's) counts as the same.
            if ischar(a) || isstring(a)
                same = ischar(b) && strcmp(char(a), b);
            elseif islogical(a)
                same = (islogical(b) || isnumeric(b)) && isequal(size(a(:)), size(b(:))) ...
                       && all(a(:) == logical(b(:)));
            elseif isnumeric(a)
                if isempty(a)
                    same = isempty(b);
                    return
                end
                if isempty(b)   % JSON's null for a lone NaN comes back as []
                    same = isscalar(a) && isnan(a);
                    return
                end
                if ~isnumeric(b) || numel(a) ~= numel(b)
                    same = false;
                    return
                end
                if ~isvector(a) && ~isequal(size(a), size(b))
                    same = false;
                    return
                end
                a = double(a(:));
                b = double(b(:));
                both = isnan(a) & isnan(b);
                same = all(both | abs(a - b) <= tolerance * max(1, abs(a)));
            else
                same = isequal(a, b);
            end
        end
    end
end
