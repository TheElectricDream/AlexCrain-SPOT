function c = labCalibration()
    %LABCALIBRATION The lab's calibration as this project took it (Constants.LAB_*).
    %   c = spot.params.labCalibration() returns the calibration the lab console measured (its
    %   Calibration tab), as Update from Lab wrote it into the project's Constants.m, with the
    %   values the project overrides (OVERRIDE_*) and where its LEDs are (LED_XY_*).
    %   spot.params.apply uses it; a value outside its plausible range is reported in c.error
    %   and that part ignored, so simulations still run.
    %
    %   Outputs
    %     c  struct with:
    %          file       Constants.m when a part has been taken, else ''
    %          updated    when its newest part was measured (ISO 8601, UTC; '' if none)
    %          table      table slope calibration, or [] if none:
    %                       slope    [a1 a2 a3 a4 a5 xc yc], the table top's height surface
    %                                (spot.sim.tableSlope)
    %                       damping  RED, BLACK, BLUE: air-bearing drag [1/s] (0 if not
    %                                measured)
    %                       model, updated, tilt_centre_mrad, tilt_max_mrad, downhill_deg
    %          thrusters  RED, BLACK, BLUE: each platform's thruster calibration, or []:
    %                       thrust   8x1 thrust of each thruster [N] (F_thrusters_<PLATFORM>)
    %                       decay    thrust lost per extra valve open at once; [] if not
    %                                measured
    %                       updated, angle (8x1 [deg]), torqueRatio (8x1)
    %          arm        the arm calibration, or []: created, updated, joints (a cell array
    %                     of structs with index and the measured fields: static, coulomb,
    %                     stribeck, viscous, back_emf, torque_per_volt, armature), and the
    %                     format, version and control spot.arm.loadCalibration expects
    %          markers    RED, BLACK, BLUE: the platform's PhaseSpace LEDs, as
    %                     spot.massprops.markerLayout returns them (ids, leds 4x2 [m] from the
    %                     body centre), where this project has them (LED_XY_*)
    %          overrides  the values the project overrides, NaN where not overridden; SI units:
    %                       table.height     1x5 [a1 a2 (m/m) a3 a4 a5 (1/m)]
    %                       table.damping    RED, BLACK, BLUE [1/s]
    %                       thrusters.RED    thrust (8x1 [N]), decay (per extra valve)
    %                       arm              static, coulomb, stribeck, viscous, armature,
    %                                        back_emf [V s/rad] (1x3, per joint)
    %          error      why a part couldn't be used ('' if all could)
    %
    %   See also spot.params.apply, spot.sim.tableSlope, spot.arm.loadCalibration,
    %   spot.massprops.markerLayout.

    %% Constants
    MIN_THRUST = Constants.CAL_MIN_THRUST;   % [N] plausible thrust range
    MAX_THRUST = Constants.CAL_MAX_THRUST;   % [N]
    MAX_DECAY = Constants.CAL_MAX_DECAY;     % plausible thrust decay per extra valve
    MAX_LED = Constants.LED_MAX_OFFSET;      % [m] an LED this far from the body centre at most
    PLATFORMS = {'RED', 'BLACK', 'BLUE'};
    ARM_FIELDS = {'static', 'coulomb', 'stribeck', 'viscous', 'back_emf', 'torque_per_volt', ...
                  'armature'};
    ARM_CONSTANTS = {'LAB_ARM_STATIC', 'LAB_ARM_COULOMB', 'LAB_ARM_STRIBECK', ...
                     'LAB_ARM_VISCOUS', 'LAB_ARM_BACK_EMF', 'LAB_ARM_TORQUE_PER_VOLT', ...
                     'LAB_ARM_ARMATURE'};

    c = struct('file', '', 'updated', '', 'table', [], ...
               'thrusters', struct('RED', [], 'BLACK', [], 'BLUE', []), 'arm', [], ...
               'markers', spot.massprops.markerLayout(), 'overrides', noOverrides(), ...
               'error', '');
    problems = {};
    updated = {};

    %% Table slope
    if ~isempty(Constants.LAB_TABLE_UPDATED)
        height = double(Constants.LAB_TABLE_HEIGHT(:).');
        centre = double(Constants.LAB_TABLE_CENTRE(:).');
        if numel(height) ~= 5 || numel(centre) ~= 2 || ~all(isfinite([height centre]))
            problems{end + 1} = 'LAB_TABLE_HEIGHT must be 5 numbers and LAB_TABLE_CENTRE 2';
        else
            damping = struct('RED', 0, 'BLACK', 0, 'BLUE', 0);
            for name = PLATFORMS
                d = Constants.(['LAB_TABLE_DAMPING_' name{1}]);
                if isfinite(d)
                    damping.(name{1}) = double(d);
                end
            end
            c.table = struct('slope', [height centre], 'damping', damping, ...
                'model', char(Constants.LAB_TABLE_MODEL), ...
                'updated', char(Constants.LAB_TABLE_UPDATED), ...
                'tilt_centre_mrad', Constants.LAB_TABLE_TILT_CENTRE_MRAD, ...
                'tilt_max_mrad', Constants.LAB_TABLE_TILT_MAX_MRAD, ...
                'downhill_deg', Constants.LAB_TABLE_DOWNHILL_DEG);
            updated{end + 1} = c.table.updated;
        end
    end

    %% Thrusters
    for name = PLATFORMS
        key = name{1};
        when = char(Constants.(['LAB_THRUST_UPDATED_' key]));
        if isempty(when)
            continue
        end
        F = double(Constants.(['LAB_THRUST_' key])(:));
        if numel(F) ~= 8 || ~all(isfinite(F)) || any(F < MIN_THRUST | F > MAX_THRUST)
            problems{end + 1} = sprintf('LAB_THRUST_%s must be 8 thrusts of %g to %g N', ...
                                        key, MIN_THRUST, MAX_THRUST); %#ok<AGROW>
            continue
        end
        decay = double(Constants.(['LAB_THRUST_DECAY_' key]));
        if ~isfinite(decay)
            decay = [];
        elseif decay < 0 || decay > MAX_DECAY
            problems{end + 1} = sprintf('LAB_THRUST_DECAY_%s must be 0 to %g', ...
                                        key, MAX_DECAY); %#ok<AGROW>
            continue
        end
        c.thrusters.(key) = struct('thrust', F, 'decay', decay, 'updated', when, ...
            'angle', eight(Constants.(['LAB_THRUST_ANGLE_' key])), ...
            'torqueRatio', eight(Constants.(['LAB_TORQUE_RATIO_' key])));
        updated{end + 1} = when; %#ok<AGROW>
    end

    %% Arm
    if ~isempty(Constants.LAB_ARM_UPDATED)
        joints = {};
        for j = 1:3
            joint = struct('index', j);
            for f = 1:numel(ARM_FIELDS)
                v = Constants.(ARM_CONSTANTS{f});
                if numel(v) >= j && isfinite(v(j))
                    joint.(ARM_FIELDS{f}) = double(v(j));
                end
            end
            if numel(fieldnames(joint)) > 1
                joints{end + 1} = joint; %#ok<AGROW>
            end
        end
        if ~isempty(joints)
            c.arm = struct('format', 'spot-arm-calibration', ...
                'version', Constants.ARM_CALIBRATION_FORMAT, 'control', 'pwm', ...
                'created', char(Constants.LAB_ARM_UPDATED), ...
                'updated', char(Constants.LAB_ARM_UPDATED), 'joints', {joints});
            updated{end + 1} = c.arm.updated;
        end
    end

    %% LEDs
    for name = PLATFORMS
        xy = double(Constants.(['LED_XY_' name{1}]));
        if ~isequal(size(xy), [4 2]) || ~all(isfinite(xy(:))) || any(abs(xy(:)) > MAX_LED)
            problems{end + 1} = sprintf( ...
                'LED_XY_%s must be 4 LEDs [x y] within %g m of the body centre', ...
                name{1}, MAX_LED); %#ok<AGROW>
            continue
        end
        c.markers.(name{1}).leds = xy;
    end

    %% Overrides
    c.overrides.table.height = vector(Constants.OVERRIDE_TABLE_HEIGHT, 5).';
    for name = PLATFORMS
        c.overrides.table.damping.(name{1}) = ...
            scalar(Constants.(['OVERRIDE_TABLE_DAMPING_' name{1}]));
        c.overrides.thrusters.(name{1}) = struct( ...
            'thrust', vector(Constants.(['OVERRIDE_THRUST_' name{1}]), 8), ...
            'decay', scalar(Constants.(['OVERRIDE_THRUST_DECAY_' name{1}])));
    end
    for f = fieldnames(c.overrides.arm).'
        c.overrides.arm.(f{1}) = vector(Constants.(['OVERRIDE_ARM_' upper(f{1})]), 3).';
    end

    %% Summary
    if ~isempty(updated)
        c.file = Constants.file();
        newest = sort(string(updated));
        c.updated = char(newest(end));
    end
    if ~isempty(problems)
        c.error = sprintf('Constants.m: %s; the simulation ignores that part.', ...
                          strjoin(problems, '; '));
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
    % nx1 numbers (NaN where the constant is shorter or empty).
    v = nan(n, 1);
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

function v = eight(x)
    % 8x1 numbers (NaN where not given).
    v = vector(x, 8);
end
