function [controls] = defaultController(state, setpoint, K)
    %DEFAULTCONTROLLER A sample LQR control law that brings the platform to its setpoint.
    %   controls = spot.control.defaultController(state, setpoint, K)
    %
    %   Inputs
    %       state       Spacecraft state bus
    %                       state.pose  Position and attitude [m; m; rad]
    %                       state.rate  Linear and angular velocity [m/s; m/s; rad/s]
    %       setpoint    Target state bus obtained from the path planner
    %                       setpoint.pose   Desired position and attitude [m; m; rad]
    %                       setpoint.rate   Desired linear and angular
    %                       velocity [m/s; m/s; rad/s]
    %       K           LQR gain matrix (see spot.control.lqrGains)
    %
    %   Outputs
    %       controls    Commanded force and torques in the inertial frame
    %
    %   See also spot.dynamics.platformDynamics, spot.control.lqrGains.

    %#codegen
    
    arguments (Input)
        state       (1,1) struct
        setpoint    (1,1) struct
        K           (3,6) double
    end

    % Calculate the errors between where the spacecraft currently is
    % located and the target setpoint
    poseError = state.pose - setpoint.pose;     % [m; m; rad]
    rateError = state.rate - setpoint.rate;     % [m/s; m/s; rad/s]

    % The attitude error should be wrapped to [-pi, pi) so that large
    % errors don't cause the spacecraft to spin more then required (350
    % [deg] is the same as -10 [deg])
    poseError(3) = mod(poseError(3) + pi, 2*pi) - pi;   % [rad]

    % Using the LQR gains, calculate the desired control inputs to the
    % spacecraft
    controls = -K * [poseError; rateError];     % [N; N; N.m]

end