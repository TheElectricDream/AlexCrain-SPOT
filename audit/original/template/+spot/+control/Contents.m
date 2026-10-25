% CONTROL  Controllers and thruster mixers: LQR design, both diagrams' control laws and mixers, the arm's servo control
%
%   Design
%   lqrGains                - Discrete LQR gain matrix for a platform.
%   nominalForces           - Thrust of each of the 8 thrusters of a platform.
%   stateSpace              - Discrete planar rigid-body model of one platform.
%
%   Template_v5_0_0_2026a_Linear's controllers and mixer
%   defaultController       - A sample LQR control law that brings the platform to its setpoint.
%   customController        - User control law, slot 1. Selected when controllerChoice == 2.
%   allocateThrusters       - Duty cycles for the 8 thrusters that best produce a force command.
%   thrusterMatrix          - Map from the 8 thruster duty cycles to body-frame force and torque.
%   thrustShare             - Fraction of its own thrust each thruster gives, with others open at once.
%   dropShortPulses         - Turn off thrusters whose pulse is shorter than a valve can open.
%   holdDuty                - Hold the duty cycles for a whole PWM period, as the thrusters do.
%
%   Template_v5_0_0_2026a_Original's default controller, taken out of its blocks (verbatim
%   copies of their code) so that it can be read, run and checked on its own
%   lqrCommand              - The diagram's default LQR control law: forces and torque for one platform, one step.
%   thrusterDutyCycles      - The diagram's thruster mixer: 8 thrusters' duty cycles for a force command.
%   verifyConsoleController - Check the lab console's interactive controller against the diagram.
%   updateDiagramMixer      - Put thrusterDutyCycles's mixer code into a diagram's mixer blocks.
%
%   The arm's Dynamixel servo laws, as controllers giving joint PWM
%   ArmPositionControl      - The Dynamixel servos' position control, as a controller giving joint PWM.
%   ArmVelocityControl      - The Dynamixel servos' velocity control, as a controller giving joint PWM.
%
%   The lab console runs lqrCommand and thrusterDutyCycles in Python for Interactive Mode
%   (PhaseSpace tab): +interface/+console/webapp/spot_console/control.py, checked against these
%   by verifyConsoleController.
