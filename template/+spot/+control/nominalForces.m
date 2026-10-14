function F = nominalForces(force)
%NOMINALFORCES Thrust of each of the 8 thrusters of a platform.
%   F = spot.control.nominalForces(force) returns the 8x1 vector F_thrusters_<platform> for a
%   platform whose thrusters all give FORCE newtons. Thrusters 1, 2, 5, 6 act along x and 3, 4,
%   7, 8 along y (resources/ThrusterConfig.png); they are listed separately so that a platform
%   with unequal x and y thrusters can be described.
Fx = force; Fy = force;
F = [Fx; Fx; Fy; Fy; Fx; Fx; Fy; Fy];
end
