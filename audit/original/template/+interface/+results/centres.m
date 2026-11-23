function cg = centres(settings)
    %CENTRES Each platform's centre of gravity, the point its logged pose is.
    %   cg = interface.results.centres(settings) for a dataset's settings (ds.settings), so its
    %   platforms are drawn around their CG as they were weighed for that run.
    %
    %   Inputs
    %     settings  the dataset's settings, or [] when unknown
    %
    %   Outputs
    %     cg        cg.RED, cg.BLACK, cg.BLUE: [x y] of each CG from its body centre [m]
    %               (spot.massprops.compute), from the settings' scale readings; the default
    %               settings' when they are unknown or can't be read
    %
    %   See also spot.massprops.compute, interface.results.trajectory.

    %% The settings' mass properties, else the defaults'
    props = [];
    if isstruct(settings) && isfield(settings, 'mass')
        try
            props = spot.massprops.compute(settings.mass);
        catch
        end
    end
    if isempty(props)
        props = spot.massprops.compute(spot.params.defaults().mass);
    end

    %% Each platform's CG
    cg = struct('RED', props.RED.cg, 'BLACK', props.BLACK.cg, 'BLUE', props.BLUE.cg);
end
