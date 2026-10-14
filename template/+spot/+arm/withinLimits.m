function tau = withinLimits(tau, pulses, minPosition, maxPosition)
%WITHINLIMITS No joint torque that pushes a joint further beyond its position limits.
%   tau = spot.arm.withinLimits(tau, pulses, minPosition, maxPosition) zeroes each torque [N m]
%   whose joint is at or beyond maxPosition and pushes it up, or at or beyond minPosition and
%   pushes it down (positions in encoder pulses, 2048 straight out). In current control the
%   servos have no position limits of their own (position control had Min/Max Position), so the
%   hardware block (spot.hardware.ArmTorque) applies this before sending the torques, and the
%   plant (spot.arm.Plant) does the same.
%#codegen
for j = 1:3
    if (pulses(j) >= maxPosition && tau(j) > 0) || (pulses(j) <= minPosition && tau(j) < 0)
        tau(j) = 0;
    end
end
end
