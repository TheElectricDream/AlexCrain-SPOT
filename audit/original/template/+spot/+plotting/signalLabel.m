function L = signalLabel(name)
    %SIGNALLABEL Readable name and unit of a logged signal, for figure labels.
    %   L = spot.plotting.signalLabel('BLACK_Vx_mpers') returns platform 'BLACK', quantity
    %   'X-Velocity', unit 'm/s' and label 'X-Velocity [m/s]'.
    %
    %   Inputs
    %     name  signal name, normally <PLATFORM>_<quantity>_<unit>
    %
    %   Outputs
    %     L     platform, quantity, unit (TeX) and label. Names that don't follow the pattern
    %           get their underscores turned to spaces.
    %
    %   See also interface.results.summary, interface.results.toFigure.

    L = struct('platform', '', 'quantity', '', 'unit', '', 'label', '');
    rest = name;

    %% Platform prefix
    tok = regexp(rest, '^(RED|BLACK|BLUE|ARM)_(.*)$', 'tokens', 'once');
    if ~isempty(tok)
        L.platform = tok{1};
        rest = tok{2};
    end

    %% Unit suffix (longest first, so _mpers2 isn't read as _mpers)
    units = {'_mpers2$', 'm/s^2'
             '_radpers2$', 'rad/s^2'
             '_mpers$', 'm/s'
             '_radpers$', 'rad/s'
             '_Nm$', 'N m'
             '_N$', 'N'
             '_rad$', 'rad'
             '_m$', 'm'
             '_s$', 's'};
    for k = 1:size(units, 1)
        if ~isempty(regexp(rest, units{k, 1}, 'once'))
            L.unit = units{k, 2};
            rest = regexprep(rest, units{k, 1}, '');
            break
        end
    end

    %% Words: quantity, prefixes, joint, others
    words = strsplit(rest, '_');
    quantityOf = containers.Map( ...
        {'Px', 'Py', 'Rz', 'Vx', 'Vy', 'RzD', 'Ax', 'Ay', 'RzDD', 'Fx', 'Fy', 'Tz', 'Gz', ...
         'Time'}, ...
        {'X-Position', 'Y-Position', 'Attitude', 'X-Velocity', 'Y-Velocity', 'Angular Rate', ...
         'X-Acceleration', 'Y-Acceleration', 'Angular Acceleration', 'X-Force', 'Y-Force', ...
         'Torque', 'Angular Rate', 'Time'});
    prefix = containers.Map({'Desired', 'Path', 'Sat', 'IMU'}, ...
                            {'Desired', 'Path', 'Saturated', 'IMU'});
    joints = {'Shoulder', 'Elbow', 'Wrist'};
    pre = {};
    main = '';
    joint = '';
    other = {};
    for word = words
        if isempty(word{1})
            continue
        elseif isKey(quantityOf, word{1}) && isempty(main)
            main = quantityOf(word{1});
        elseif isKey(prefix, word{1})
            pre{end + 1} = prefix(word{1}); %#ok<AGROW>
        elseif ismember(word{1}, joints)
            joint = word{1};
        else
            other{end + 1} = word{1}; %#ok<AGROW>
        end
    end

    %% Arm joints: ARM_Rz_Shoulder_rad -> Shoulder Angle, ARM_RzD_Elbow_radpers -> Elbow Rate
    if ~isempty(joint)
        main = strrep(strrep(strrep(main, 'Angular Acceleration', 'Acceleration'), ...
            'Angular Rate', 'Rate'), 'Attitude', 'Angle');
        if isempty(main)
            main = strjoin(other, ' ');
            other = {};
        end
        main = strtrim([joint ' ' main]);
    end

    %% Quantity and label
    L.quantity = strjoin([pre, {main}, other], ' ');
    L.quantity = strtrim(regexprep(L.quantity, '\s+', ' '));
    if isempty(L.quantity)
        L.quantity = strrep(name, '_', ' ');
    end
    L.label = L.quantity;
    if ~isempty(L.unit)
        L.label = sprintf('%s [%s]', L.quantity, L.unit);
    end
end
