function r = compute(mass, markers)
    %COMPUTE Mass properties of RED, BLACK and BLUE from the settings' mass section.
    %   r = spot.massprops.compute(s.mass)
    %   r = spot.massprops.compute(s.mass, markers) with the LEDs where the project's
    %   calibration has them (spot.params.loadCalibration's markers).
    %
    %   Inputs
    %     mass     the settings' mass section (s.mass)
    %     markers  each platform's LEDs, as spot.massprops.markerLayout returns them (optional:
    %              that layout)
    %
    %   Outputs
    %     r        r.RED, r.BLACK, r.BLUE, each with:
    %                mass, inertia                  values used in the simulation (typed in,
    %                                               if mass.override)
    %                measuredMass, measuredInertia  from the scales and the bifilar test
    %                cg, thrusters, leds            see spot.massprops.centerOfGravity
    %                ledIds                         1x4 ids of the LEDs in leds
    %
    %   See also spot.massprops.centerOfGravity, spot.massprops.bifilarInertia,
    %   spot.massprops.markerLayout.

    %% Defaults
    if nargin < 2
        markers = spot.massprops.markerLayout();
    end

    %% Each platform
    r = struct();
    for name = {'RED', 'BLACK', 'BLUE'}
        platform = mass.(name{1});

        % Measured: scales and bifilar test; the LEDs where they are.
        props = spot.massprops.centerOfGravity(platform.scales, markers.(name{1}).leds);
        props.ledIds = markers.(name{1}).ids;
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
