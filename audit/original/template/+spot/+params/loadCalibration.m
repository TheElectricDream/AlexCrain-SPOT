function c = loadCalibration(root)
    %LOADCALIBRATION The project's calibration of the lab (spot_calibration.json).
    %   c = spot.params.loadCalibration(root) reads the calibration the lab console measured (its
    %   Calibration tab), as the local console copied it into the project, with the values the
    %   project overrides. spot.params.apply uses it; a file that can't be read is reported and
    %   ignored, so simulations still run on a level table.
    %   c = spot.params.loadCalibration() returns no calibration (and no overrides), with the
    %   LEDs where spot.massprops.markerLayout puts them.
    %
    %   The file sits in the project folder, next to spot_settings.json. (A project made before
    %   it moved there kept it in a resources folder, where it is still read; the console moves
    %   it across.)
    %
    %   Inputs
    %     root  project folder (optional)
    %
    %   Outputs
    %     c     struct with:
    %             file       the file ('' if the project has none)
    %             updated    when its newest part was measured (ISO 8601, UTC; '' if none)
    %             table      table slope calibration, or [] if none:
    %                          slope    [a1 a2 a3 a4 a5 xc yc], the table top's height surface
    %                                   (spot.sim.tableSlope)
    %                          damping  RED, BLACK, BLUE: air-bearing drag [1/s] (0 if not
    %                                   measured)
    %                          model, updated, tilt_centre_mrad, tilt_max_mrad, downhill_deg
    %             thrusters  RED, BLACK, BLUE: each platform's thruster calibration, or []:
    %                          thrust   8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>)
    %                          decay    thrust lost per extra valve open at once
    %                                   (thrust_decay_<PLATFORM>); [] if not measured with the
    %                                   linear model
    %                          updated, angle (8x1 [deg], how far each thrust is turned),
    %                          torqueRatio (8x1)
    %             arm        the arm calibration (calibrate_arm.py's), or []: joints' friction
    %                        and inertia, created, updated (spot.arm.parameters applies it)
    %             markers    RED, BLACK, BLUE: the platform's PhaseSpace LEDs, as
    %                        spot.massprops.markerLayout returns them (ids, leds 4x2 [m] from the
    %                        body centre): where the project has them (the Simulation tab's
    %                        Thruster and LED Layout), else the default layout
    %             overrides  the values the project overrides (the Simulation tab's Calibration
    %                        pane), NaN where not overridden; SI units:
    %                          table.height     1x5 [a1 a2 (m/m) a3 a4 a5 (1/m)]
    %                          table.damping    RED, BLACK, BLUE [1/s]
    %                          thrusters.RED    thrust (8x1 [N]), decay (per extra valve)
    %                          arm              static, coulomb, stribeck, viscous, armature,
    %                                           back_emf [V s/rad] (1x3, per joint)
    %             error      why the file couldn't be used ('' if it could)
    %
    %   See also spot.params.apply, spot.sim.tableSlope, spot.arm.loadCalibration,
    %   spot.massprops.markerLayout.

    %% Constants
    TABLE_CENTRE = [1.75 1.2];   % [m] default centre of the height surface
    MIN_THRUST = 0.01;           % [N] plausible thrust range
    MAX_THRUST = 2;
    MAX_DECAY = 0.5;             % plausible thrust decay per extra valve
    MAX_LED = 0.3;               % [m] an LED this far from the body centre at most
    M_PER_MM = 1e-3;

    %% The file
    c = struct('file', '', 'updated', '', 'table', [], ...
               'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
               'markers', spot.massprops.markerLayout(), 'overrides', noOverrides(), ...
               'error', '');
    if nargin < 1 || isempty(root)
        return
    end
    file = fullfile(root, 'spot_calibration.json');
    if ~isfile(file)
        file = fullfile(root, 'resources', 'spot_calibration.json');
    end
    if ~isfile(file)
        return
    end
    c.file = file;

    try
        saved = jsondecode(fileread(file));
        if ~isfield(saved, 'format') || ~strcmp(saved.format, 'spot-calibration')
            error('spot:params:loadCalibration', 'not a SPOT calibration file');
        end
        if isfield(saved, 'updated') && ischar(saved.updated)
            c.updated = saved.updated;
        end

        %% Table slope
        if isfield(saved, 'table') && isstruct(saved.table)
            tableCal = saved.table;

            % Height surface coefficients and its centre.
            height = double(tableCal.height(:).');
            centre = TABLE_CENTRE;
            if isfield(tableCal, 'centre')
                centre = double(tableCal.centre(:).');
            end
            if numel(height) ~= 5 || numel(centre) ~= 2 || ~all(isfinite([height centre]))
                error('spot:params:loadCalibration', ...
                    'table.height must be 5 numbers and table.centre 2');
            end

            % Each platform's air-bearing drag; 0 where not measured.
            damping = struct('RED', 0, 'BLACK', 0, 'BLUE', 0);
            if isfield(tableCal, 'damping') && isstruct(tableCal.damping)
                for name = {'RED', 'BLACK', 'BLUE'}
                    if isfield(tableCal.damping, name{1}) && isfinite(tableCal.damping.(name{1}))
                        damping.(name{1}) = double(tableCal.damping.(name{1}));
                    end
                end
            end

            c.table = struct('slope', [height centre], 'damping', damping, ...
                'model', field(tableCal, 'model', ''), ...
                'updated', field(tableCal, 'updated', ''), ...
                'tilt_centre_mrad', field(tableCal, 'tilt_centre_mrad', NaN), ...
                'tilt_max_mrad', field(tableCal, 'tilt_max_mrad', NaN), ...
                'downhill_deg', field(tableCal, 'downhill_deg', NaN));
        end

        %% Thrusters
        if isfield(saved, 'thrusters') && isstruct(saved.thrusters)
            for name = {'RED', 'BLACK', 'BLUE'}
                if ~isfield(saved.thrusters, name{1})
                    continue
                end
                cal = saved.thrusters.(name{1});

                % Eight plausible thrusts.
                F = double(cal.thrust(:));
                if numel(F) ~= 8 || ~all(isfinite(F)) || any(F < MIN_THRUST | F > MAX_THRUST)
                    error('spot:params:loadCalibration', ...
                        'thrusters.%s.thrust must be 8 thrusts of 0.01 to 2 N', name{1});
                end

                % Thrust decay, if measured with the linear model.
                decay = [];
                if isfield(cal, 'decay') && isstruct(cal.decay) ...
                        && isfield(cal.decay, 'slope') ...
                        && strcmp(field(cal.decay, 'model', 'linear'), 'linear')
                    decay = double(cal.decay.slope);
                    if ~isscalar(decay) || ~isfinite(decay) || decay < 0 || decay > MAX_DECAY
                        error('spot:params:loadCalibration', ...
                            'thrusters.%s.decay.slope must be 0 to 0.5', name{1});
                    end
                end

                c.thrusters.(name{1}) = struct('thrust', F, 'decay', decay, ...
                    'updated', field(cal, 'updated', ''), ...
                    'angle', numbers(field(cal, 'angle', [])), ...
                    'torqueRatio', numbers(field(cal, 'torque_ratio', [])));
            end
        end

        %% Arm
        if isfield(saved, 'arm') && isstruct(saved.arm) && isfield(saved.arm, 'format') ...
                && strcmp(saved.arm.format, 'spot-arm-calibration')
            c.arm = struct('created', field(saved.arm, 'created', ''), ...
                'updated', field(saved.arm, 'updated', ''), ...
                'joints', {field(saved.arm, 'joints', [])});
        end

        %% LEDs
        % Each platform's four LEDs, {id, x, y} [mm] from the body centre, in any order.
        if isfield(saved, 'markers') && isstruct(saved.markers)
            for name = {'RED', 'BLACK', 'BLUE'}
                if ~isfield(saved.markers, name{1})
                    continue
                end
                given = saved.markers.(name{1});
                platform = c.markers.(name{1});
                if ~isstruct(given) || ~all(isfield(given, {'id', 'x', 'y'})) ...
                        || numel(given) ~= numel(platform.ids)
                    error('spot:params:loadCalibration', ...
                        'markers.%s must be its 4 LEDs, each {id, x, y}', name{1});
                end
                for k = 1:numel(platform.ids)
                    led = given([given.id] == platform.ids(k));
                    xy = double([led.x led.y]) * M_PER_MM;
                    if numel(xy) ~= 2 || ~all(isfinite(xy)) || any(abs(xy) > MAX_LED)
                        error('spot:params:loadCalibration', ...
                            'markers.%s must give LED %d one position within 300 mm', ...
                            name{1}, platform.ids(k));
                    end
                    platform.leds(k, :) = xy;
                end
                c.markers.(name{1}) = platform;
            end
        end

        %% Overrides
        % NaN (JSON null) where a value isn't overridden; the console checked the ranges.
        if isfield(saved, 'overrides') && isstruct(saved.overrides)
            ov = saved.overrides;
            if isfield(ov, 'table') && isstruct(ov.table)
                c.overrides.table.height = vector(field(ov.table, 'height', []), 5).';
                if isfield(ov.table, 'damping') && isstruct(ov.table.damping)
                    for name = {'RED', 'BLACK', 'BLUE'}
                        c.overrides.table.damping.(name{1}) = ...
                            scalar(field(ov.table.damping, name{1}, NaN));
                    end
                end
            end
            if isfield(ov, 'thrusters') && isstruct(ov.thrusters)
                for name = {'RED', 'BLACK', 'BLUE'}
                    if isfield(ov.thrusters, name{1}) && isstruct(ov.thrusters.(name{1}))
                        o = ov.thrusters.(name{1});
                        c.overrides.thrusters.(name{1}) = struct( ...
                            'thrust', vector(field(o, 'thrust', []), 8), ...
                            'decay', scalar(field(o, 'decay', NaN)));
                    end
                end
            end
            if isfield(ov, 'arm') && isstruct(ov.arm)
                for f = fieldnames(c.overrides.arm).'
                    c.overrides.arm.(f{1}) = vector(field(ov.arm, f{1}, []), 3).';
                end
            end
        end

    catch err
        % Unusable: report why, and use none of it.
        c = struct('file', file, 'updated', '', 'table', [], ...
            'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
            'markers', spot.massprops.markerLayout(), 'overrides', noOverrides(), ...
            'error', sprintf('%s could not be used (%s); the simulation ignores it.', ...
                             file, err.message));
    end
end

function ov = noOverrides()
    % No value overridden: NaN everywhere.
    damping = struct('RED', NaN, 'BLACK', NaN, 'BLUE', NaN);
    ov.table = struct('height', nan(1, 5), 'damping', damping);
    platform = struct('thrust', nan(8, 1), 'decay', NaN);
    ov.thrusters = struct('RED', platform, 'BLACK', platform, 'BLUE', platform);
    joints = nan(1, 3);
    ov.arm = struct('static', joints, 'coulomb', joints, 'stribeck', joints, 'viscous', joints, ...
                    'armature', joints, 'back_emf', joints);
end

function v = vector(x, n)
    % nx1 numbers from a JSON list (null elements, or a missing or wrong-sized list: NaN).
    v = nan(n, 1);
    if iscell(x)
        x = cellfun(@(e) ifEmpty(e), x);
    end
    if isnumeric(x) && numel(x) == n
        v = double(x(:));
    end
end

function v = scalar(x)
    % A finite number, else NaN.
    v = NaN;
    if isnumeric(x) && isscalar(x) && isfinite(x)
        v = double(x);
    end
end

function v = numbers(x)
    % 8x1 numbers from a JSON list (null, a thruster without a result, or a missing list: NaN).
    v = vector(x, 8);
end

function e = ifEmpty(e)
    % NaN for an empty value.
    if isempty(e)
        e = NaN;
    end
end

function v = field(s, name, default)
    % s.(name) if present, else default.
    if isfield(s, name)
        v = s.(name);
    else
        v = default;
    end
end
