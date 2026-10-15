function sys = stateSpace(mass, inertia, Ts)
    %STATESPACE Discrete planar rigid-body model of one platform.
    %   sys = spot.control.stateSpace(mass, inertia, Ts) returns the zero-order-hold
    %   discretisation of the planar double integrator. Needs the Control System Toolbox.
    %
    %   Inputs
    %     mass     [kg]
    %     inertia  [kg m^2] about z
    %     Ts       sample time [s]
    %
    %   Outputs
    %     sys      struct with A, B, C, D; states [x y theta xdot ydot thetadot], inputs
    %              [Fx Fy Tz]
    %
    %   See also spot.control.lqrGains.

    % Continuous double integrator: positions integrate velocities, forces accelerate.
    A = [zeros(3) eye(3); zeros(3, 6)];
    B = [zeros(3); diag([1/mass, 1/mass, 1/inertia])];
    C = eye(6);
    D = zeros(6, 3);

    % Zero-order-hold discretisation.
    d = c2d(ss(A, B, C, D), Ts, 'zoh');
    sys = struct('A', d.A, 'B', d.B, 'C', d.C, 'D', d.D);
end
