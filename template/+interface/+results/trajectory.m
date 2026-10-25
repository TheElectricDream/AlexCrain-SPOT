function r = trajectory(ds, maxRate)
    %TRAJECTORY A dataset's motion for the console's 3D view.
    %   r = interface.results.trajectory(dataset) resamples the motion to at most 30 Hz.
    %   r = interface.results.trajectory(dataset, maxRate) uses another maximum rate [Hz].
    %
    %   Inputs
    %     ds       dataset (spot.data.load)
    %     maxRate  maximum rate [Hz] (default 30)
    %
    %   Outputs
    %     r        struct with:
    %                t              time [s]
    %                crafts.<NAME>  x, y [m] and att [rad, unwrapped] of each platform
    %                               (interface.results.bodies); NaN where it has no position (before
    %                               its log starts, or all zeros in an experiment: PhaseSpace
    %                               didn't see it yet)
    %                arm            4 x 2 x N joint positions [m], when the arm is in it
    %                phases         phase names and end times, when the settings are known
    %
    %   See also interface.results.bodies, spot.plotting.armPosition.

    %% Defaults
    if nargin < 2
        maxRate = 30;
    end

    sig = ds.signals;
    found = interface.results.bodies(ds);
    r = struct('t', [], 'crafts', struct());
    if isempty(found.crafts)
        error('spot:data:trajectory', ...
            '%s has no platform positions (<NAME>_Px_m, _Py_m, _Rz_rad) to show.', ds.name);
    end

    % In an experiment a position of exactly (0, 0) means PhaseSpace hasn't seen the platform.
    measured = ~strcmp(ds.kind, 'simulation');

    %% Raw positions and the common time span
    raw = struct();
    t0 = inf;
    t1 = -inf;
    nSamples = 0;
    for k = 1:numel(found.crafts)
        craft = found.crafts{k};

        % Position and attitude samples.
        [t, x] = spot.data.samples(sig.([craft '_Px_m']));
        [~, y] = spot.data.samples(sig.([craft '_Py_m']));
        [tAtt, att] = spot.data.samples(sig.([craft '_Rz_rad']));
        x = x(:, 1);
        y = y(:, 1);

        % Drop unseen samples in experiments.
        valid = true(size(t));
        if measured
            valid = ~(x == 0 & y == 0);
        end
        raw.(craft) = struct('t', t(valid), 'x', x(valid), 'y', y(valid), ...
                             'ta', tAtt, 'a', unwrap(att(:, 1)));

        % From the first to the last position of any platform.
        if any(valid)
            t0 = min(t0, raw.(craft).t(1));
            t1 = max(t1, raw.(craft).t(end));
            nSamples = max(nSamples, nnz(valid));
        end
    end
    if ~isfinite(t0)
        error('spot:data:trajectory', '%s has no position other than zero.', ds.name);
    end

    % Keep the experiment's own clock: 0 is when the program started.
    if measured
        t0 = min(t0, 0);
    end

    %% Common time grid, at most maxRate
    nGrid = max(2, min(nSamples, ceil((t1 - t0) * maxRate) + 1));
    tGrid = linspace(t0, t1, nGrid).';
    r.t = tGrid.';

    %% Platforms on the grid
    for k = 1:numel(found.crafts)
        craft = found.crafts{k};
        track = raw.(craft);
        if numel(track.t) < 2
            continue
        end
        r.crafts.(craft) = struct('x', interp1(track.t, track.x, tGrid, 'linear', NaN).', ...
                                  'y', interp1(track.t, track.y, tGrid, 'linear', NaN).', ...
                                  'att', interp1(track.ta, track.a, tGrid, 'linear', NaN).');

        % No attitude where there is no position.
        r.crafts.(craft).att(isnan(r.crafts.(craft).x)) = NaN;
    end

    %% Arm on the grid
    if found.arm && isfield(r.crafts, 'RED')

        % Joint angles.
        joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
        q = zeros(nGrid, 3);
        for j = 1:3
            [tj, v] = spot.data.samples(sig.(joints{j}));
            q(:, j) = interp1(tj, v(:, 1), tGrid, 'linear', NaN);
        end

        % The arm's geometry: the dataset's settings, else the defaults.
        arm = spot.params.defaults().mass.arm;
        if isstruct(ds.settings)
            arm = ds.settings.mass.arm;
        end

        % Joint positions from RED's pose.
        base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
        r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(arm));
    end

    %% Phases, when the settings are known
    if isstruct(ds.settings)
        try
            r.phases = spot.params.timing(ds.settings).phases;
        catch
        end
    end
end
