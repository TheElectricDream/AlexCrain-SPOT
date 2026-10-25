function r = simulation(dataClass, s, d, maxRate)
    %SIMULATION Trajectory of a simulation for the SPOT Console's 3D view.
    %   r = interface.results.simulation(dataClass, s, d) resamples the motion to at most 30 Hz.
    %   r = interface.results.simulation(..., maxRate) uses another maximum rate [Hz].
    %
    %   Inputs
    %     dataClass  the logged data (simOut.dataPacket)
    %     s          settings
    %     d          the values from spot.params.apply
    %     maxRate    maximum rate [Hz] (default 30)
    %
    %   Outputs
    %     r          struct with:
    %                  t              time [s]
    %                  samples        number of logged time stamps
    %                  crafts.<NAME>  x, y [m] and att [rad, unwrapped] of each selected
    %                                 platform
    %                  arm            4 x 2 x N joint positions [m] (shoulder, elbow, wrist,
    %                                 end effector)
    %                  phases         end time of each phase and sub-phase [s]
    %                  signals        name and size of every logged signal
    %
    %   See also spot.sim.run, interface.results.trajectory.

    %% Defaults
    if nargin < 4
        maxRate = 30;
    end

    %% Time grid, at most maxRate
    time = unique(timeOf(dataClass));
    nGrid = max(2, min(numel(time), ceil((time(end) - time(1)) * maxRate) + 1));
    tGrid = linspace(time(1), time(end), nGrid).';

    r = struct();
    r.t = tGrid.';
    r.samples = numel(time);

    %% Selected platforms on the grid
    r.crafts = struct();
    names = {'RED', 'BLACK', 'BLUE'};
    for k = 1:3
        key = names{k};
        if ~s.platforms.(key) || ~isfield(dataClass, [key '_Px_m'])
            continue
        end
        x = resample1(dataClass.([key '_Px_m']), tGrid);
        y = resample1(dataClass.([key '_Py_m']), tGrid);
        att = resample1(dataClass.([key '_Rz_rad']), tGrid, true);
        r.crafts.(key) = struct('x', x.', 'y', y.', 'att', att.');
    end

    %% Arm on the grid
    if s.platforms.ARM && isfield(r.crafts, 'RED') && isfield(dataClass, 'ARM_Rz_Shoulder_rad')
        q = [resample1(dataClass.ARM_Rz_Shoulder_rad, tGrid), ...
             resample1(dataClass.ARM_Rz_Elbow_rad, tGrid), ...
             resample1(dataClass.ARM_Rz_Wrist_rad, tGrid)];
        base = [r.crafts.RED.x.', r.crafts.RED.y.', r.crafts.RED.att.'];
        r.arm = spot.plotting.armPosition(base, q, spot.arm.mountGeometry(s.mass.arm));
    end

    %% Phases and signals
    r.phases = d.timing.phases;   % the segments that ran (skipped ones left out)
    r.signals = signalList(dataClass);
end

function time = timeOf(dataClass)
    % The log's time stamps: Time_s, else the first timeseries'.
    if isfield(dataClass, 'Time_s') && isa(dataClass.Time_s, 'timeseries')
        time = dataClass.Time_s.Time;
    else
        f = fieldnames(dataClass);
        for k = 1:numel(f)
            if isa(dataClass.(f{k}), 'timeseries')
                time = dataClass.(f{k}).Time;
                return
            end
        end
        error('spot:sim:results', 'The logged data has no timeseries.');
    end
end

function y = resample1(ts, t, isAngle)
    % A signal (first column) at times t, on its own time stamps (repeated ones dropped);
    % angles unwrapped first.
    [time, keep] = unique(ts.Time(:));
    data = firstColumn(ts.Data);
    data = data(keep);
    if nargin > 2 && isAngle
        data = unwrap(data);
    end
    y = interp1(time, data, t, 'linear', 'extrap');
end

function list = signalList(dataClass)
    % Name and size of every timeseries, one level of structs deep (as parent.child).
    list = struct('name', {}, 'size', {});
    for f = fieldnames(dataClass).'
        x = dataClass.(f{1});
        if isa(x, 'timeseries')
            list(end + 1) = struct('name', f{1}, 'size', size(x.Data)); %#ok<AGROW>
        elseif isstruct(x)
            for g = fieldnames(x).'
                if isa(x.(g{1}), 'timeseries')
                    list(end + 1) = struct('name', [f{1} '.' g{1}], ...
                                           'size', size(x.(g{1}).Data)); %#ok<AGROW>
                end
            end
        end
    end
end

function c = firstColumn(data)
    % First signal of logged data stored as N x m, or as 1 x 1 x N (Simulink's 3-D format).
    data = double(data);
    if ndims(data) == 3
        data = reshape(data, [], size(data, 3)).';
    end
    c = data(:, 1);
end
