function r = compute(mass)
    %COMPUTE Mass properties of RED, BLACK and BLUE from the settings' mass section.
    %   r = spot.massprops.compute(s.mass)
    %
    %   Inputs
    %     mass  the settings' mass section (s.mass)
    %
    %   Outputs
    %     r     r.RED, r.BLACK, r.BLUE, each with:
    %             mass, inertia                  values used in the simulation (typed in, if
    %                                            mass.override)
    %             measuredMass, measuredInertia  from the scales and the bifilar test
    %             cg, thrusters, leds            see spot.massprops.centerOfGravity
    %
    %   See also spot.massprops.centerOfGravity, spot.massprops.bifilarInertia.

    r = struct();
    for name = {'RED', 'BLACK', 'BLUE'}
        platform = mass.(name{1});

        % Measured: scales and bifilar test.
        props = spot.massprops.centerOfGravity(platform.scales);
        props.measuredMass = props.mass;
        props.measuredInertia = spot.massprops.bifilarInertia(props.mass, platform.bifilar.T, ...
            platform.bifilar.L, platform.bifilar.D);

        % Used: typed-in values if overridden, else the measured ones.
        if mass.override
            props.mass = platform.mass;
            props.inertia = platform.inertia;
        else
            props.inertia = props.measuredInertia;
        end
        r.(name{1}) = props;
    end
end
