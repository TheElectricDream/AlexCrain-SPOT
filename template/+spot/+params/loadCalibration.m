function c = loadCalibration(root)
    %LOADCALIBRATION The project's calibration of the lab (resources/spot_calibration.json).
    %   c = spot.params.loadCalibration(root) reads the calibration the lab console measured (its
    %   Calibration tab), as the local console copied it into the project. spot.params.apply
    %   uses it; a file that can't be read is reported and ignored, so simulations still run on
    %   a level table.
    %
    %   Inputs
    %     root  project folder
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
    %             error      why the file couldn't be used ('' if it could)
    %
    %   See also spot.params.apply, spot.sim.tableSlope, spot.arm.loadCalibration.

    %% Constants
    TABLE_CENTRE = [1.75 1.2];   % [m] default centre of the height surface
    MIN_THRUST = 0.01;           % [N] plausible thrust range
    MAX_THRUST = 2;
    MAX_DECAY = 0.5;             % plausible thrust decay per extra valve

    %% The file
    c = struct('file', '', 'updated', '', 'table', [], ...
               'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], 'error', '');
    file = fullfile(root, 'resources', 'spot_calibration.json');
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

    catch err
        % Unusable: report why, and use none of it.
        c = struct('file', file, 'updated', '', 'table', [], ...
            'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
            'error', sprintf('%s could not be used (%s); the simulation ignores it.', ...
                             file, err.message));
    end
end

function v = numbers(x)
    % 8x1 numbers from a JSON list (null, a thruster without a result, or a missing list: NaN).
    v = nan(8, 1);
    if iscell(x)
        x = cellfun(@(e) ifEmpty(e), x);
    end
    if isnumeric(x) && numel(x) == 8
        v = double(x(:));
    end
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
