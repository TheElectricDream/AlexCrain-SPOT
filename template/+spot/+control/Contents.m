% CONTROL  Controller design: platform models, LQR gains, thruster forces, the diagram's controller
%
%   lqrGains        - Discrete LQR gain matrix for a platform.
%   nominalForces   - Thrust of each of the 8 thrusters of a platform.
%   stateSpace      - Discrete planar rigid-body model of one platform.
%
%   The template diagram's default controller, taken out of its blocks (verbatim copies of their
%   code) so that it can be read, run and checked on its own:
%   lqrCommand         - The diagram's default LQR control law: forces and torque for one platform, one step.
%   thrusterDutyCycles - The diagram's thruster mixer: the 8 thrusters' duty cycles for a force command.
%   verifyConsoleController - Check the lab console's Interactive-mode controller against the diagram.
%
%   The lab console runs the same two functions in Python for Interactive Mode (PhaseSpace tab):
%   +spot/+console/webapp/spot_console/control.py, checked against these by verifyConsoleController.
