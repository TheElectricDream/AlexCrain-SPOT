function stamps = timeStamps(frames)
    %TIMESTAMPS Each frame's times as stamp text: experiment clock, PhaseSpace, frame.
    %   stamps = timeStamps(frames) for spot.camera's converters, which add each camera's
    %   own lines.
    %
    %   Inputs
    %     frames  the camera's frames.csv as a table (experiment_time, phasespace_time and
    %             frame columns; experiment_time NaN where no clock had reached the Orin)
    %
    %   Outputs
    %     stamps  Nx1 string, one per row: "t  +12.345 s\NPS   85.873 s\Nframe 123"
    %
    %   See also spot.camera.thermalToMkv.

    %% Experiment clock, PhaseSpace packet time, frame index
    stamps = compose("t  %+9.3f s", frames.experiment_time);
    stamps(isnan(frames.experiment_time)) = "t          - s";
    ps = compose("PS  %9.3f s", frames.phasespace_time);
    ps(isnan(frames.phasespace_time)) = "PS         - s";
    stamps = stamps + "\N" + ps + "\N" + compose("frame %d", frames.frame);
end
