function [H] = thrusterMatrix(thrusterDist, thrust)
    %THRUSTERMATRIX Map from the 8 thruster duty cycles to body-frame force and torque.
    %   H = diagram.control.thrusterMatrix(thrusterDist, thrust)
    %
    %   Column i of H is the force and torque thruster i produces, averaged over one
    %   PWM period, when its duty cycle is 1. So the body-frame force and torque for
    %   any duty cycles d is H * d.
    %
    %   Inputs
    %     thrusterDist  Moment arm of each thruster about the CG [mm]
    %                   (thruster_dist2CG_<PLATFORM>)
    %     thrust        Thrust of each thruster [N] (F_thrusters_<PLATFORM>)
    %
    %   Outputs
    %     H             3x8 map from duty cycles to [Fx_b; Fy_b; Tz] [N; N; N.m]
    %
    %   See also diagram.control.allocateThrusters, spot.control.nominalForces.

    %#codegen

    arguments (Input)
            thrusterDist    (8,1) double
            thrust          (8,1) double {mustBePositive}
    end

    % Direction each thruster pushes the body: thrusters 1, 2 along -x, 3, 4
    % along +y, 5, 6 along +x and 7, 8 along -y
    direction = [-1 -1  0  0  1  1  0  0;     % x
                  0  0  1  1  0  0 -1 -1];    % y
    
    % Stack the force directions with the moment arms, then scale each column
    % by that thruster's thrust
    H = [direction; thrusterDist.' / 1000] .* thrust.';   % [N; N; N.m]
end