% SPOT  The SPOT software, as packages that can be audited one by one.
%   Every technical function of the SPOT Console lives here; the console (web page and server)
%   only calls them. "help spot.<package>" lists a package.
%
%   spot.arm          The simulated arm: its servos, friction and 1 ms plant; its controllers; calibration from lab data
%   spot.console      Starting, opening and stopping the SPOT Console (the web page and its server)
%   spot.control      Controller design: platform models, LQR gains, thruster forces
%   spot.data         Results: loading saved runs and simulations, signals to plot, 3D playback, figures
%   spot.experiment   Experiments: checking, connecting to the TX2, building, converting the data
%   spot.hardware     Everything that talks to the real hardware (platform programs, PhaseSpace, Dynamixel arm, Teensy)
%   spot.massprops    Mass properties: centre of gravity, inertia, LED positions for PhaseSpace
%   spot.params       Settings: defaults, loading/saving, timing, putting them into the workspace
%   spot.plotting     Drawing: the arm, spacecraft outlines, signal labels
%   spot.sim          Simulations: diagrams, solver, running, results for the 3D view
%   spot.worker       The MATLAB worker that runs the console's commands in this session
%   spot.workspace    The console's Workspace panel and Command Window
%
%   Outside the packages: SPOT.m (starts the console) and Constants.m (every parameter of
%   SPOT, in one place; change your project's copy for your own values). Retired code (the old GUI, Custom_Library, earlier diagrams) is outside the
%   project, in ../archive/ (not published).
