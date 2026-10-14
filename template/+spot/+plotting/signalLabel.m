function L = signalLabel(name)
%SIGNALLABEL Readable name and unit of a logged signal, for figure labels.
%   L = spot.plotting.signalLabel('BLACK_Vx_mpers') returns platform ('BLACK'), quantity
%   ('X-Velocity'), unit ('m/s', TeX) and label ('X-Velocity [m/s]'). Names that don't follow
%   the diagrams' <PLATFORM>_<quantity>_<unit> pattern get their underscores turned to spaces.
L = struct('platform', '', 'quantity', '', 'unit', '', 'label', '');
rest = name;
tok = regexp(rest, '^(RED|BLACK|BLUE|ARM)_(.*)$', 'tokens', 'once');
if ~isempty(tok)
    L.platform = tok{1};
    rest = tok{2};
end
units = {'_mpers2$', 'm/s^2'; '_radpers2$', 'rad/s^2'; '_mpers$', 'm/s'; '_radpers$', 'rad/s'; ...
         '_Nm$', 'N m'; '_N$', 'N'; '_rad$', 'rad'; '_m$', 'm'; '_s$', 's'};
for k = 1:size(units, 1)
    if ~isempty(regexp(rest, units{k, 1}, 'once'))
        L.unit = units{k, 2};
        rest = regexprep(rest, units{k, 1}, '');
        break
    end
end
words = strsplit(rest, '_');
base = containers.Map( ...
    {'Px', 'Py', 'Rz', 'Vx', 'Vy', 'RzD', 'Ax', 'Ay', 'RzDD', 'Fx', 'Fy', 'Tz', 'Gz', 'Time'}, ...
    {'X-Position', 'Y-Position', 'Attitude', 'X-Velocity', 'Y-Velocity', 'Angular Rate', ...
     'X-Acceleration', 'Y-Acceleration', 'Angular Acceleration', 'X-Force', 'Y-Force', 'Torque', ...
     'Angular Rate', 'Time'});
prefix = containers.Map({'Desired', 'Path', 'Sat', 'IMU'}, {'Desired', 'Path', 'Saturated', 'IMU'});
joints = {'Shoulder', 'Elbow', 'Wrist'};
pre = {}; main = ''; joint = ''; other = {};
for w = words
    if isempty(w{1})
        continue
    elseif isKey(base, w{1}) && isempty(main)
        main = base(w{1});
    elseif isKey(prefix, w{1})
        pre{end + 1} = prefix(w{1}); %#ok<AGROW>
    elseif ismember(w{1}, joints)
        joint = w{1};
    else
        other{end + 1} = w{1}; %#ok<AGROW>
    end
end
if ~isempty(joint)   % ARM_Rz_Shoulder_rad -> Shoulder Angle, ARM_RzD_Elbow_radpers -> Elbow Rate
    main = strrep(strrep(strrep(main, 'Angular Acceleration', 'Acceleration'), 'Angular Rate', 'Rate'), 'Attitude', 'Angle');
    if isempty(main)
        main = strjoin(other, ' ');
        other = {};
    end
    main = strtrim([joint ' ' main]);
end
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
