function encodeMkv(inputArgs, filters, stamps, fps, frameSize, outFile)
    %ENCODEMKV Encode frames as an H.264 .mkv with a stamp burned into each frame.
    %   encodeMkv(inputArgs, filters, stamps, fps, frameSize, outFile) runs ffmpeg for
    %   spot.camera's converters.
    %
    %   Inputs
    %     inputArgs  ffmpeg's input arguments, a string array ("-i", file, and the format of
    %                a raw file before it)
    %     filters    ffmpeg filters before the stamp ("crop=...", "scale=..."), "" for none
    %     stamps     Nx1 string, frame k's text (spot.camera's writeStamps)
    %     fps        frame rate of the video [frames/s]
    %     frameSize  [width height] after the filters [pixels]
    %     outFile    the .mkv to write (replaced if it exists)
    %
    %   Outputs
    %     (none)
    %
    %   Behavior
    %     1. Frame k is shown at (k - 1)/fps, whatever times the input had, so each frame
    %        gets its own stamp.
    %     2. The stamps go in a temporary .ass file, burned in by ffmpeg's ass filter.
    %     3. H.264 (CRF 18), 4:2:0 colour (padded to even sizes), in Matroska.
    %
    %   See also spot.camera.thermalToMkv.

    %% Constants
    CRF = 18;   % H.264 quality (lower is better; 18 looks lossless)

    %% Stamps
    stampFile = string(tempname) + ".ass";
    cleanup = onCleanup(@() deleteIfThere(stampFile));
    writeStamps(stampFile, stamps, fps, frameSize);

    %% Filters: even frame times, the caller's, the stamp, even sizes for 4:2:0
    % Times in microseconds first: in the input's own time base (an .avi's 1/30 s, say)
    % frame k could land on frame k - 1's stamp.
    rate = sprintf("%.6f", fps);
    chain = "settb=AVTB,setpts=N/(" + rate + "*TB)";
    if strlength(filters) > 0
        chain = chain + "," + filters;
    end
    chain = chain + ",ass=filename='" + filterPath(stampFile) + "'" + ...
        ",pad=ceil(iw/2)*2:ceil(ih/2)*2";

    %% Encode
    args = ["-hide_banner", "-loglevel", "error", "-nostdin", "-y", inputArgs, ...
            "-vf", chain, "-an", "-c:v", "libx264", "-preset", "medium", ...
            "-crf", string(CRF), "-pix_fmt", "yuv420p", outFile];
    fprintf('Writing %s (%d frames)...\n', outFile, numel(stamps));
    [status, out] = system(ffmpegCommand("ffmpeg", args));
    if status ~= 0
        error('spot:camera:ffmpeg', 'ffmpeg could not write %s:\n%s', outFile, out);
    end
end

function p = filterPath(file)
    %FILTERPATH A file name as an ffmpeg filter option (forward slashes, ':' escaped).
    p = replace(replace(string(file), "\", "/"), ":", "\:");
end

function deleteIfThere(file)
    %DELETEIFTHERE Delete a temporary file if it exists.
    if isfile(file)
        delete(file);
    end
end
