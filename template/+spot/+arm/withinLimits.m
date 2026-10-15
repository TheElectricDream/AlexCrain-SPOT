function tau = withinLimits(tau, pulses, minPosition, maxPosition)
    %WITHINLIMITS No joint torque that pushes a joint further beyond its position limits.
    %   tau = spot.arm.withinLimits(tau, pulses, minPosition, maxPosition) zeroes each torque
    %   that pushes its joint further past a position limit. In current control the servos have
    %   no position limits of their own, so spot.hardware.ArmTorque and spot.arm.Plant apply
    %   this before sending torques.
    %
    %   Inputs
    %     tau          3x1 joint torques [N m]
    %     pulses       3x1 joint positions [pulses], 2048 straight out
    %     minPosition  Min Position Limit [pulses]
    %     maxPosition  Max Position Limit [pulses]
    %
    %   Outputs
    %     tau          3x1 joint torques [N m], zero where they pushed past a limit
    %
    %   See also spot.hardware.ArmTorque, spot.arm.Plant, spot.arm.checkCommands.

    %#codegen

    % Zero a positive torque at or past Max Position, or a negative one at or past Min Position.
    for j = 1:3
        if (pulses(j) >= maxPosition && tau(j) > 0) || (pulses(j) <= minPosition && tau(j) < 0)
            tau(j) = 0;
        end
    end
end
