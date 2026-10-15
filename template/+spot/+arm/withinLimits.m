function cmd = withinLimits(cmd, pulses, minPosition, maxPosition)
    %WITHINLIMITS No joint command that pushes a joint further beyond its position limits.
    %   cmd = spot.arm.withinLimits(cmd, pulses, minPosition, maxPosition) zeroes each command
    %   that pushes its joint further past a position limit. In PWM control the servos have no
    %   position limits of their own, so spot.hardware.ArmPWM and spot.arm.Plant apply this
    %   before sending commands.
    %
    %   Inputs
    %     cmd          3x1 joint commands [PWM] (positive turns a joint towards Max Position)
    %     pulses       3x1 joint positions [pulses], 2048 straight out
    %     minPosition  Min Position Limit [pulses]
    %     maxPosition  Max Position Limit [pulses]
    %
    %   Outputs
    %     cmd          3x1 joint commands, zero where they pushed past a limit
    %
    %   See also spot.hardware.ArmPWM, spot.arm.Plant, spot.arm.checkCommands.

    %#codegen

    % Zero a positive command at or past Max Position, or a negative one at or past Min Position.
    for j = 1:3
        if (pulses(j) >= maxPosition && cmd(j) > 0) || (pulses(j) <= minPosition && cmd(j) < 0)
            cmd(j) = 0;
        end
    end
end
