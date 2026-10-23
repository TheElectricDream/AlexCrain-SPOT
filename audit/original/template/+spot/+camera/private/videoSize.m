function frameSize = videoSize(inputArgs)
    %VIDEOSIZE The frame size of a video file, from ffprobe.
    %   frameSize = videoSize(inputArgs) for spot.camera's converters.
    %
    %   Inputs
    %     inputArgs  ffprobe's input arguments, a string array (the file, and "-f", format
    %                before it for a raw stream)
    %
    %   Outputs
    %     frameSize  [width height] of its first video stream [pixels]
    %
    %   See also spot.camera.zedToMkv, spot.camera.usbToMkv.

    %% Ask ffprobe
    args = ["-v", "error", "-select_streams", "v:0", "-show_entries", ...
            "stream=width,height", "-of", "csv=p=0", inputArgs];
    [status, out] = system(ffmpegCommand("ffprobe", args));
    frameSize = sscanf(out, '%d,%d')';
    if status ~= 0 || numel(frameSize) ~= 2
        error('spot:camera:ffprobe', 'ffprobe could not read the video''s size:\n%s', out);
    end
end
