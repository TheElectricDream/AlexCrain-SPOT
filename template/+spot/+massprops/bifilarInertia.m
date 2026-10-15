function I = bifilarInertia(mass, T, L, D)
    %BIFILARINERTIA Moment of inertia from a bifilar (two-string) pendulum test.
    %   I = spot.massprops.bifilarInertia(mass, T, L, D)
    %
    %   Inputs
    %     mass  [kg]
    %     T     period of oscillation [s]
    %     L     string length [m]
    %     D     string spacing [m]
    %
    %   Outputs
    %     I     moment of inertia [kg m^2]
    %
    %   See also spot.massprops.compute.

    %% Constants
    G = 9.81;   % [m/s^2]

    % Bifilar pendulum: I = T^2 m g D^2 / (16 pi^2 L).
    I = T^2 * mass * G * D^2 / (16 * pi^2 * L);
end
