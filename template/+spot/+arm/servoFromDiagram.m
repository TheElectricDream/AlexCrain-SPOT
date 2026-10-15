function servo = servoFromDiagram(model, servo)
    %SERVOFROMDIAGRAM The simulated servos' limits, from the diagram's arm blocks.
    %   servo = spot.arm.servoFromDiagram(model, servo) copies into servo the limits set on the
    %   diagram's arm blocks. A missing block, or a value that can't be evaluated, leaves the
    %   servo's own value.
    %
    %   Inputs
    %     model  diagram name
    %     servo  armModel.servo
    %
    %   Outputs
    %     servo  with the blocks' values:
    %              spot.arm.PositionControl   Min/Max Position (checked by checkCommands)
    %              spot.arm.VelocityControl   Velocity Limit (checked by checkCommands)
    %              spot.hardware.ArmTorque    Current Limit, Min/Max Position (what the real
    %                                         servos get and the plant holds torques within)
    %
    %   See also spot.arm.checkCommands, spot.sim.run.

    % Block class, then {block parameter, servo field} pairs. Later blocks override earlier.
    maps = {'spot.arm.PositionControl', {'MinPosition', 'minPosition'; ...
                                         'MaxPosition', 'maxPosition'};
            'spot.arm.VelocityControl', {'VelocityLimit', 'velocityLimit'};
            'spot.hardware.ArmTorque',  {'CurrentLimit', 'currentLimit'; ...
                                         'MinPosition', 'minPosition'; ...
                                         'MaxPosition', 'maxPosition'}};

    for b = 1:size(maps, 1)

        % First block of this class, in any variant.
        blocks = find_system(model, 'LookUnderMasks', 'all', 'FollowLinks', 'on', ...
            'MatchFilter', @Simulink.match.allVariants, ...
            'BlockType', 'MATLABSystem', 'System', maps{b, 1});
        if isempty(blocks)
            continue
        end

        % Copy each parameter that resolves to a finite number.
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
