function servo = servoFromDiagram(model, servo)
%SERVOFROMDIAGRAM The simulated servos' limits, from the diagram's arm blocks.
%   servo = spot.arm.servoFromDiagram(model, servo) copies into servo (armModel.servo) the limits
%   the diagram's blocks set: the Velocity Limit of the velocity controller
%   (spot.arm.VelocityControl) and the Min/Max Position of the position controller
%   (spot.arm.PositionControl), which spot.arm.checkCommands warns about, then the Current Limit
%   and Min/Max Position of "Dynamixel Arm (Torque)" (spot.hardware.ArmTorque: what the real
%   servos get, and what the plant holds the torques within). A block that isn't there, or a
%   value that can't be evaluated, leaves servo's own.
maps = {'spot.arm.PositionControl', {'MinPosition', 'minPosition'; 'MaxPosition', 'maxPosition'};
        'spot.arm.VelocityControl', {'VelocityLimit', 'velocityLimit'};
        'spot.hardware.ArmTorque', {'CurrentLimit', 'currentLimit'; 'MinPosition', 'minPosition'; 'MaxPosition', 'maxPosition'}};
for b = 1:size(maps, 1)
    blocks = find_system(model, 'LookUnderMasks', 'all', 'FollowLinks', 'on', 'MatchFilter', @Simulink.match.allVariants, ...
        'BlockType', 'MATLABSystem', 'System', maps{b, 1});
    if isempty(blocks)
        continue
    end
    map = maps{b, 2};
    for k = 1:size(map, 1)
        try
            value = slResolve(get_param(blocks{1}, map{k, 1}), blocks{1});
            if isnumeric(value) && isscalar(value) && isfinite(value)
                servo.(map{k, 2}) = double(value);
            end
        catch
        end
    end
end
end
