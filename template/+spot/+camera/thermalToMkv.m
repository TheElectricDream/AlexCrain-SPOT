function outFile = thermalToMkv(folder, outFile)
    %THERMALTOMKV A thermal camera's recording as an .mkv video, each frame stamped with its times.
    %   outFile = spot.camera.thermalToMkv(folder) converts an ICI 9320P recording
    %   (thermal_raw.bin) into a video next to it, each frame stamped in its top-left corner
    %   with the experiment time, the PhaseSpace time, its frame numbers, the housing
    %   temperature and, when not ok, the shutter's state.
    %   outFile = spot.camera.thermalToMkv(folder, outFile) writes it there instead.
    %
    %   Inputs
    %     folder   the camera's folder in a run (Saved Data/<run>/cameras/thermal-...)
    %     outFile  the .mkv to write, default <folder>/<camera>.mkv
    %
    %   Outputs
    %     outFile  the .mkv written
    %
    %   Behavior
    %     1. Raw counts to grey (white hot) on one scale for the whole recording: its 1st
    %        to 99.5th percentile over up to 200 frames spread through it.
    %     2. Each frame twice its size (pixels kept sharp), so the stamp is readable.
    %     3. Frames at the recording's rate (frames.csv); a frame not kept or lost
    %        (a gap in sensor_frame) shows as the stamp's sensor number jumping.
    %
    %   Example
    %     spot.camera.thermalToMkv(fullfile('Saved Data', run, 'cameras', 'thermal-1-2.4'));
    %
    %   See also spot.camera.recordingToMkv, spot.camera.zedToMkv.

    %% Defaults
    if nargin < 2
        outFile = fullfile(folder, cameraName(folder) + ".mkv");
    end

    %% Constants
    WIDTH = 320;               % [pixels] the sensor's columns
    HEIGHT = 240;              % [pixels] its rows
    BYTES_PER_PIXEL = 2;       % little-endian uint16 counts
    UPSCALE = 2;               % video size per sensor pixel
    LOW_PERCENT = 1;           % [%] of the counts shown black
    HIGH_PERCENT = 99.5;       % [%] of the counts below white
    SAMPLE_FRAMES = 200;       % frames sampled for the scale
    CHUNK_FRAMES = 300;        % frames converted at a time

    %% Frame log and file
    [frames, fps] = frameLog(folder);
    rawFile = fullfile(folder, 'thermal_raw.bin');
    if ~isfile(rawFile)
        error('spot:camera:noFile', '%s is missing.', rawFile);
    end
    frameBytes = WIDTH * HEIGHT * BYTES_PER_PIXEL;
    info = dir(rawFile);
    nFrames = min(floor(info.bytes / frameBytes), height(frames));
    frames = frames(1:nFrames, :);
    fid = fopen(rawFile, 'r', 'l');
    closeRaw = onCleanup(@() fclose(fid));

    %% Grey scale: the counts' percentiles over frames spread through the recording
    picks = unique(round(linspace(1, nFrames, min(SAMPLE_FRAMES, nFrames))));
    sample = zeros(WIDTH * HEIGHT, numel(picks), 'uint16');
    for j = 1:numel(picks)
        fseek(fid, (picks(j) - 1) * frameBytes, 'bof');
        sample(:, j) = fread(fid, WIDTH * HEIGHT, '*uint16');
    end
    sorted = sort(double(sample(:)));
    low = sorted(max(1, round(LOW_PERCENT / 100 * numel(sorted))));
    high = max(sorted(round(HIGH_PERCENT / 100 * numel(sorted))), low + 1);

    %% 8-bit frames into a temporary raw file (rows in the file's order)
    greyFile = string(tempname) + ".gray";
    removeGrey = onCleanup(@() delete(greyFile));
    out = fopen(greyFile, 'w');
    fseek(fid, 0, 'bof');
    for first = 1:CHUNK_FRAMES:nFrames
        n = min(CHUNK_FRAMES, nFrames - first + 1);
        counts = fread(fid, WIDTH * HEIGHT * n, '*uint16');
        fwrite(out, uint8(255 * (double(counts) - low) / (high - low)));
    end
    fclose(out);

    %% Stamps: times, the sensor's frame number, housing temperature, shutter
    stamps = timeStamps(frames) + compose(" (sensor %d)", frames.sensor_frame);
    housing = compose("\\Nhousing %.1f " + char(176) + "C", frames.housing_c);
    housing(isnan(frames.housing_c)) = "";
    shutter = "\Nshutter " + frames.shutter;
    shutter(frames.shutter == "ok" | ismissing(frames.shutter)) = "";
    stamps = stamps + housing + shutter;

    %% Encode
    inputArgs = ["-f", "rawvideo", "-pix_fmt", "gray", ...
                 "-video_size", sprintf("%dx%d", WIDTH, HEIGHT), ...
                 "-framerate", sprintf("%.6f", fps), "-i", greyFile];
    upscale = sprintf("scale=iw*%d:ih*%d:flags=neighbor", UPSCALE, UPSCALE);
    encodeMkv(inputArgs, upscale, stamps, fps, UPSCALE * [WIDTH HEIGHT], outFile);
    outFile = string(outFile);
end
