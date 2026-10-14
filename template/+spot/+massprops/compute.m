function r = compute(mass)
%COMPUTE Mass properties of RED, BLACK and BLUE from the settings' mass section.
%   r = spot.massprops.compute(s.mass) returns r.RED, r.BLACK, r.BLUE, each with
%     mass, inertia     the values used in the simulation (typed in, if s.mass.override)
%     measuredMass, measuredInertia   from the scales and the bifilar test
%     cg, thrusters, leds             see spot.massprops.centerOfGravity
r = struct();
for name = {'RED', 'BLACK', 'BLUE'}
    p = mass.(name{1});
    c = spot.massprops.centerOfGravity(p.scales);
    c.measuredMass = c.mass;
    c.measuredInertia = spot.massprops.bifilarInertia(c.mass, p.bifilar.T, p.bifilar.L, p.bifilar.D);
    if mass.override
        c.mass = p.mass;
        c.inertia = p.inertia;
    else
        c.inertia = c.measuredInertia;
    end
    r.(name{1}) = c;
end
end
