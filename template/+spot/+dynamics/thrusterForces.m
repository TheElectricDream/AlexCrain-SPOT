function [forces] = thrusterForces(duty, theta, thrusterDist, thrust, decayPerValve)
    %THRUSTERFORCES Force and torque the 8 thrusters apply, in the inertial frame.
    %   forces = spot.dynamics.thrusterForces(duty, theta, thrusterDist, thrust, decayPerValve)
    %
    %   Each thruster gives its full thrust while open, less the thrust lost to the
    %   other valves open at the same time (spot.control.thrustShare).
    %
    %   Inputs
    %     duty            Duty cycle of each thruster, 0 to 1
    %     theta           Spacecraft attitude [rad]
    %     thrusterDist    Moment arm of each thruster about the CG [mm]
    %     thrust          Thrust of each thruster [N]
    %     decayPerValve   Thrust lost per extra valve open (thrust_decay_<PLATFORM>)
    %
    %   Outputs
    %     forces          Applied force and torque in the inertial frame [N; N; N.m]
    %
    %   See also spot.control.thrusterMatrix, spot.control.allocateThrusters.

    %#codegen

    arguments (Input)
            duty            (8,1) double
            theta           (1,1) double
            thrusterDist    (8,1) double
            thrust          (8,1) double {mustBePositive}
            decayPerValve   (1,1) double {mustBeNonnegative}
    end

    % Force and torque in the body frame, each thruster's thrust reduced by the
    % others open at the same time
    share      = spot.control.thrustShare(duty, decayPerValve);
    bodyForces = spot.control.thrusterMatrix(thrusterDist, thrust .* share) * duty;

    % Rotate the force into the inertial frame; torque is unchanged
    C_Ib = [cos(theta) -sin(theta);
            sin(theta)  cos(theta)];
    forces = [C_Ib * bodyForces(1:2); bodyForces(3)];

end
