% SPOT  The SPOT software: everything technical, for the diagrams and for your own scripts.
%   The diagrams' blocks call these functions, and each can be called on its own. The SPOT
%   Console's own code (its web page, server and MATLAB worker) is in the interface package.
%   "help spot.<package>" lists a package.
%
%   spot.arm          The simulated arm: its servos, friction and 1 ms plant; calibration from lab data
%   spot.buses        The diagrams' bus types and the flat list of signals they log
%   spot.camera       Camera recordings: each camera's files as an .mkv video, stamped with its times
%   spot.control      Controllers and thruster mixers: LQR design, both diagrams' control laws and mixers, the arm's servo control
%   spot.data         Logged data: loading saved runs and simulations, their signals, figures
%   spot.dynamics     The simulated platforms: rigid-body motion on the table, thruster forces, measurement
%   spot.experiment   Experiments: checking, connecting to the TX2, building, converting the data
%   spot.guidance     Path planners: the setpoint each controller tracks, from the goal
%   spot.hardware     Everything that talks to the real hardware (platform programs, PhaseSpace, Dynamixel arm, Teensy)
%   spot.massprops    Mass properties: centre of gravity, inertia, LED positions for PhaseSpace
%   spot.params       Settings: defaults, loading/saving, timing, putting them into the workspace
%   spot.plotting     Drawing: the arm, spacecraft outlines, signal labels
%   spot.sim          Simulations: diagrams, solver, running
%
%   Outside the packages: SPOT.m (starts the console), SPOT_User_Parameters.m (your own
%   variables). Retired code (the old GUI, Custom_Library, earlier diagrams) is outside the
%   project, in ../archive/ (not published).
