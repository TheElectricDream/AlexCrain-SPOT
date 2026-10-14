function msgs = checkCommands(dataClass, armModel)
%CHECKCOMMANDS Arm goals the controllers ignore and torques the servos can't give, from a run's log.
%   msgs = spot.arm.checkCommands(dataClass, armModel) looks at the logged ARM_Control_Mode (the
%   arm controller in use: 0 off, 1 position, 2 custom/torque, 3 velocity), the path
%   (ARM_Shoulder/Elbow/Wrist_Path) and the torques sent (ARM_Shoulder/Elbow/Wrist_Command), and
%   returns one message per joint and kind of problem:
%     - a goal angle beyond the servo's position limits (position control), or a goal speed beyond
%       its Velocity Limit (velocity control): the controller ignores it, as the servo would, so
%       the joint keeps its last goal
%     - a torque pushing a joint further beyond its position limits (from the logged angles,
%       ARM_Rz_*): the hardware block and the plant send none (spot.arm.withinLimits)
%     - a torque beyond what the Current Limit allows: the servo gets the limit
%   spot.sim.run adds them to the simulation's warnings.
msgs = {};
need = {'ARM_Control_Mode', 'ARM_Shoulder_Command', 'ARM_Elbow_Command', 'ARM_Wrist_Command', ...
        'ARM_Shoulder_Path', 'ARM_Elbow_Path', 'ARM_Wrist_Path'};
if ~all(isfield(dataClass, need))
    return
end
t = dataClass.ARM_Control_Mode.Time;
md = round(dataClass.ARM_Control_Mode.Data(:));
cmd = [dataClass.ARM_Shoulder_Command.Data(:), dataClass.ARM_Elbow_Command.Data(:), dataClass.ARM_Wrist_Command.Data(:)];
path = [dataClass.ARM_Shoulder_Path.Data(:), dataClass.ARM_Elbow_Path.Data(:), dataClass.ARM_Wrist_Path.Data(:)];
n = min([numel(md), size(cmd, 1), size(path, 1)]);
q = nan(n, 3);
if all(isfield(dataClass, {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'}))
    q = [dataClass.ARM_Rz_Shoulder_rad.Data(:), dataClass.ARM_Rz_Elbow_rad.Data(:), dataClass.ARM_Rz_Wrist_rad.Data(:)];
    n = min(n, size(q, 1));
    q = q(1:n, :);
end
md = md(1:n); cmd = cmd(1:n, :); path = path(1:n, :); t = t(1:n);
s = armModel.servo;
mo = armModel.motor;
Kt = mo.stallTorque(:).' ./ mo.stallCurrent(:).';
tauMax = s.currentLimit * 3.36e-3 * Kt;
names = {'shoulder', 'elbow', 'wrist'};
bad = ~ismember(md, 0:3);
if any(bad)
    k = find(bad, 1);
    msgs{end + 1} = sprintf(['ARM_Control_Mode was %g at %.1f s (%d steps): SPOT''s arm controllers write 0 (off), ' ...
        '1 (position), 2 (custom, torque) or 3 (velocity).'], md(k), t(k), nnz(bad));
end
lo = (s.minPosition - 2048) / 651.74 * 180/pi;
hi = (s.maxPosition - 2048) / 651.74 * 180/pi;
vmax = s.velocityLimit * 0.229 * 2*pi/60;
for j = 1:3
    pulses = round(651.74 * path(:, j) + 2048);
    out = md == 1 & (pulses < s.minPosition | pulses > s.maxPosition);
    if any(out)
        k = find(out, 1);
        msgs{end + 1} = sprintf(['The arm''s %s goal of %.1f deg (%d pulses) at %.1f s (%d steps) is beyond its servo''s ' ...
            'position limits, %.0f to %.0f deg (Min/Max Position %d-%d on the Default Position Control (ARM) block). ' ...
            'The controller ignores it, as the servo would, so the joint holds its last goal. Joint angles are 0 ' ...
            'straight out (the shoulder: out from its face) and +-90 deg to either side.'], ...
            names{j}, path(k, j) * 180/pi, pulses(k), t(k), nnz(out), lo, hi, s.minPosition, s.maxPosition);
    end
    units = round(path(:, j) * 9.549296585513702 * 4.366756307);
    out = md == 3 & abs(units) > s.velocityLimit;
    if any(out)
        k = find(out, 1);
        msgs{end + 1} = sprintf(['The arm''s %s goal speed of %.3f rad/s at %.1f s (%d steps) is beyond its Velocity ' ...
            'Limit (%d x 0.229 rev/min = %.3f rad/s, Default Velocity Control (ARM)): the controller ignores it, so ' ...
            'the joint keeps its last goal.'], names{j}, path(k, j), t(k), nnz(out), s.velocityLimit, vmax);
    end
    pulses = round(q(:, j) * 4096 / (2*pi)) + 2048;
    cut = (pulses >= s.maxPosition & cmd(:, j) > 0) | (pulses <= s.minPosition & cmd(:, j) < 0);
    if any(cut)
        k = find(cut, 1);
        msgs{end + 1} = sprintf(['The arm''s %s was at its position limit (%.0f deg) with a torque of %.2f N m pushing it ' ...
            'further at %.1f s (%d steps): it got none (Min/Max Position on the Dynamixel Arm (Torque) block).'], ...
            names{j}, q(k, j) * 180/pi, cmd(k, j), t(k), nnz(cut));
    end
    out = md ~= 0 & ~cut & abs(cmd(:, j)) > tauMax(j) * (1 + 1e-9) + 3.36e-3 * Kt(j) / 2;
    if any(out)
        k = find(out, 1);
        msgs{end + 1} = sprintf(['The arm''s %s torque of %.2f N m at %.1f s (%d steps) is beyond what its servo''s ' ...
            'Current Limit allows (%d x 3.36 mA = %.2f N m, on the Dynamixel Arm (Torque) block): the servo gets ' ...
            'the limit.'], names{j}, cmd(k, j), t(k), nnz(out), s.currentLimit, tauMax(j));
    end
end
end
