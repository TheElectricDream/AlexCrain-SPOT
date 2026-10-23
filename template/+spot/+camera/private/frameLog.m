function [frames, fps] = frameLog(folder)
    %FRAMELOG A camera's frames.csv, and the rate it recorded at.
    %   [frames, fps] = frameLog(folder) for spot.camera's converters.
    %
    %   Inputs
    %     folder  the camera's folder in a run (Saved Data/<run>/cameras/<camera>)
    %
    %   Outputs
    %     frames  frames.csv as a table (text columns as strings, empty numbers NaN)
    %     fps     the median rate of its rows, from orin_time [frames/s]
    %
    %   See also spot.camera.thermalToMkv.

    %% Read
    file = fullfile(folder, 'frames.csv');
    if ~isfile(file)
        error('spot:camera:noFrameLog', '%s is missing: is %s a camera''s folder?', ...
            file, folder);
    end
    opts = detectImportOptions(file, 'TextType', 'string');
    frames = readtable(file, opts);
    if height(frames) < 2
        error('spot:camera:tooShort', '%s has fewer than two frames.', file);
    end

    %% Rate
    fps = 1 / median(diff(frames.orin_time));
end
