function I = bifilarInertia(mass, T, L, D)
%BIFILARINERTIA Moment of inertia from a bifilar (two-string) pendulum test.
%   I = spot.massprops.bifilarInertia(mass, T, L, D) with the mass [kg], period of oscillation
%   T [s], string length L [m] and string spacing D [m]. Returns I in kg*m^2.
g = 9.81;
I = T^2 * mass * g * D^2 / (16 * pi^2 * L);
end
