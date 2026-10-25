function measured = measureState(truePose, previousMeasured, time, dataRate, baseRate)
    %MEASURESTATE The state as PhaseSpace reports it, from the true pose.
    %   measured = spot.dynamics.measureState(truePose, previousMeasured, time,
    %       dataRate, baseRate)
    %
    %   The simulated equivalent of the lab's motion capture, done the way StreamData
    %   (+interface/+console/phasespace/StreamData.cc) does it on the real system:
    %     - a new frame arrives only on the first step of each frame period (every
    %       dataRate seconds); on the other steps the last frame is held
    %     - the velocity is the change in pose since the previous frame, divided by the
    %       time between the frames, with no smoothing; on the first frame there is no
    %       previous one, so it is zero
    %     - the attitude is reported unwrapped (StreamData accumulates it), so turning
    %       past +-pi doesn't jump
    %   The memory is outside this function: feed measured back as previousMeasured
    %   through a Unit Delay.
    %
    %   Inputs
    %     truePose          Pose from the dynamics, attitude unwrapped [m; m; rad]
    %     previousMeasured  Measured state on the previous step (Bus: PlatformState)
    %     time              Simulation time [s], from the clock (not the bus)
    %     dataRate          Time between frames [s] (the workspace's dataRate is this
    %                       period, despite its name)
    %     baseRate          Diagram's time step [s]
    %
    %   Outputs
    %     measured          Measured state (Bus: PlatformState)
    %                         measured.pose  [m; m; rad], attitude unwrapped
    %                         measured.rate  [m/s; m/s; rad/s]
    %
    %   The time between frames is taken as one frame period (or one step, if frames come
    %   faster than the diagram runs). That is exact when dataRate is a multiple of
    %   baseRate, as with the defaults (0.1 s and 0.05 s).
    %
    %   See also spot.dynamics.platformDynamics, spot.control.holdDuty.

    %#codegen

    arguments (Input)
        truePose            (3,1) double
        previousMeasured    (1,1) struct
        time                (1,1) double
        dataRate            (1,1) double {mustBePositive}
        baseRate            (1,1) double {mustBePositive}
    end

    % A new frame arrives on this step if the previous step was in an earlier frame
    % period (the small offset avoids rounding down at an exact boundary)
    period         = floor(time / dataRate + 1e-9);
    previousPeriod = floor((time - baseRate) / dataRate + 1e-9);

    if period > previousPeriod
        measured.pose = truePose;                       % [m; m; rad]

        if period == 0
            % First frame: nothing earlier to compare with
            measured.rate = zeros(3, 1);
        else
            % Change since the previous frame (the held pose) over the time between them
            dt = max(dataRate, baseRate);               % [s]
            measured.rate = (truePose - previousMeasured.pose) / dt;   % [m/s; m/s; rad/s]
        end
    else
        % Between frames: keep the last one
        measured = previousMeasured;
    end
end
