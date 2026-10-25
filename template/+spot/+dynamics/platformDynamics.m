function [acceleration] = platformDynamics(controls, position, velocity,...
    floating, mass, inertia, tableSlope, tableDamping)
    %PLATFORMDYNAMICS Acceleration of a platform on the sloped, near-frictionless granite table.
    %   acceleration = spot.dynamics.platformDynamics(controls, position,
    %       velocity, floating, mass, inertia, tableSlope, tableDamping)
    %
    %   Inputs
    %     controls      Applied force and torque in the inertial frame [N, N, N.m]
    %     position      Spacecraft position and attitude [m; m; rad]
    %     velocity      Spacecraft linear and angular velocity [m/s; m/s; rad/s]
    %     floating      Logical indicating float status
    %     mass          Mass of the spacecraft [kg]
    %     inertia       Inertia of the spacecraft about z-axis [kg.m^2]
    %     tableSlope    Table calibration coefficients
    %     tableDamping  Drag estimate for the air bearings [1/s]
    %
    %   Outputs
    %     acceleration  Spacecraft linear and angular acceleration [m/s^2, m/s^2, rad/s^2]
    %
    %   See also spot.dynamics.tableSlope.

    %#codegen

    arguments (Input)
        controls        (3,1) double    
        position        (3,1) double
        velocity        (3,1) double
        floating        (1,1) logical
        mass            (1,1) double {mustBePositive}
        inertia         (1,1) double {mustBePositive}
        tableSlope      (1,7) double
        tableDamping    (1,1) double {mustBeNonnegative}
    end
    
    % Calculate the rigid-body accelerations from the force and torque
    % applied to the spacecraft
    acceleration = [controls(1:2) / mass;       % [m/s^2]
                    controls(3) / inertia];     % [rad/s^2]
    
    % While the platform is floating, the table slope causes drift - the
    % accuracy of this drift is a function of the latest calibration
    if floating 
        acceleration(1:2) = acceleration(1:2) +...
            spot.dynamics.tableSlope(position, velocity, ...
            tableSlope, tableDamping);
    end

end