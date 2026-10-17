function F = nominalForces(force)
    %NOMINALFORCES Thrust of each of the 8 thrusters of a platform.
    %   F = spot.control.nominalForces(force) returns F_thrusters_<platform> for a platform whose
    %   thrusters all give the same thrust.
    %
    %   Inputs
    %     force  thrust of each thruster [N]
    %
    %   Outputs
    %     F      8x1 thrusts [N]. Thrusters 1, 2, 5, 6 act along x and 3, 4, 7, 8 along y
    %            (ThrusterConfig.png in the Mass Properties guide), listed separately so unequal
    %            x and y thrusters can be described.
    %
    %   See also spot.control.thrusterDutyCycles.

    %% Constants
    % Which thrusters push along body x (the others push along y).
    ALONG_X = Constants.THRUSTER_DIRECTION_X(:) ~= 0;

    %% Thrusts
    Fx = force;
    Fy = force;
    F = Fx * ALONG_X + Fy * ~ALONG_X;
end
