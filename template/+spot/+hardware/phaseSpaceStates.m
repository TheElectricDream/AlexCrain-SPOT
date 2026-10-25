function [red, black, blue] = phaseSpaceStates(packet)
    %PHASESPACESTATES Each platform's measured state from a PhaseSpace packet.
    %   [red, black, blue] = spot.hardware.phaseSpaceStates(packet)
    %
    %   Reads the packet StreamData sends (+interface/+console/phasespace/StreamData.cc),
    %   as spot.hardware.phasespaceLatest receives it. The positions and velocities
    %   arrive in millimetres and are converted to metres. The attitude arrives
    %   accumulated (unwrapped) and is kept that way, as in the simulation
    %   (spot.dynamics.measureState). The velocities are StreamData's: the change
    %   since the previous PhaseSpace frame, unsmoothed.
    %
    %   Until the first packet arrives, everything is 0. A platform PhaseSpace has
    %   lost keeps its last values (StreamData only updates a platform it tracks).
    %
    %   Inputs
    %     packet  PhaseSpace packet, 115 values (spot.hardware.phasespaceLatest)
    %               slots 2-4    RED   x, y [mm], attitude [rad]
    %               slots 5-7    BLACK x, y [mm], attitude [rad]
    %               slots 8-10   BLUE  x, y [mm], attitude [rad]
    %               slots 11-13  RED   vx, vy [mm/s], angular rate [rad/s]
    %               slots 14-16  BLACK vx, vy [mm/s], angular rate [rad/s]
    %               slots 17-19  BLUE  vx, vy [mm/s], angular rate [rad/s]
    %
    %   Outputs
    %     red, black, blue  Each platform's measured state (Bus: PlatformState)
    %                         .pose  [m; m; rad]
    %                         .rate  [m/s; m/s; rad/s]
    %
    %   See also spot.hardware.phasespaceLatest, spot.hardware.experimentTime.

    %#codegen

    arguments (Input)
        packet  (115,1) {mustBeNumeric}
    end

    values = double(packet);

    red   = platformState(values, 2, 11);
    black = platformState(values, 5, 14);
    blue  = platformState(values, 8, 17);
end

function state = platformState(values, poseSlot, rateSlot)
    % One platform's state from its slots in the packet, millimetres to metres

    MM_TO_M = 1 / 1000;

    state.pose = [values(poseSlot)     * MM_TO_M;       % [m]
                  values(poseSlot + 1) * MM_TO_M;       % [m]
                  values(poseSlot + 2)];                % [rad]
    state.rate = [values(rateSlot)     * MM_TO_M;       % [m/s]
                  values(rateSlot + 1) * MM_TO_M;       % [m/s]
                  values(rateSlot + 2)];                % [rad/s]
end
