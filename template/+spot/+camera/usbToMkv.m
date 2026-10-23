function outFile = usbToMkv(folder, outFile)
    %USBTOMKV A USB camera's recording as an .mkv video, each frame stamped with its times.
    %   outFile = spot.camera.usbToMkv(folder) converts a USB camera's recording (video.avi,
    %   or the test pattern's pattern.avi) into a video next to it, each frame stamped in
    %   its top-left corner with the experiment time, the PhaseSpace time and its frame
    %   number.
    %   outFile = spot.camera.usbToMkv(folder, outFile) writes it there instead.
    %
    %   Inputs
    %     folder   the camera's folder in a run (Saved Data/<run>/cameras/<camera>)
    %     outFile  the .mkv to write, default <folder>/<camera>.mkv
    %
    %   Outputs
    %     outFile  the .mkv written
    %
    %   Behavior
    %     1. Frame k of the .avi is row k of frames.csv.
    %     2. At the rate the frames were recorded at (frames.csv), not the .avi's nominal
    %        rate, so the video plays in real time.
    %
    %   Example
    %     spot.camera.usbToMkv(fullfile('Saved Data', run, 'cameras', 'uvc-video0'));
    %
    %   See also spot.camera.recordingToMkv, spot.camera.thermalToMkv.

    %% Defaults
    if nargin < 2
        outFile = fullfile(folder, cameraName(folder) + ".mkv");
    end

    %% Frame log and file
    [frames, fps] = frameLog(folder);
    aviFile = fullfile(folder, 'video.avi');
    if ~isfile(aviFile)
        aviFile = fullfile(folder, 'pattern.avi');
    end
    if ~isfile(aviFile)
        error('spot:camera:noFile', 'Neither video.avi nor pattern.avi is in %s.', folder);
    end

    %% Encode
    encodeMkv(["-i", aviFile], "", timeStamps(frames), fps, videoSize(aviFile), outFile);
    outFile = string(outFile);
end
