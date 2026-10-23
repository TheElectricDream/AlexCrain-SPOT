function outFile = zedToMkv(folder, eye, outFile)
    %ZEDTOMKV A ZED camera's recording as an .mkv video, each frame stamped with its times.
    %   outFile = spot.camera.zedToMkv(folder) converts a ZED 2i recording (zed.svo2) into a
    %   video of its left image next to it, each frame stamped in its top-left corner with
    %   the experiment time, the PhaseSpace time and its frame number. No ZED SDK needed.
    %   outFile = spot.camera.zedToMkv(folder, eye) chooses the image: "left", "right" or
    %   "both" (side by side).
    %   outFile = spot.camera.zedToMkv(folder, eye, outFile) writes it there instead.
    %
    %   Inputs
    %     folder   the camera's folder in a run (Saved Data/<run>/cameras/zed-...)
    %     eye      "left" (default), "right" or "both"
    %     outFile  the .mkv to write, default <folder>/<camera>.mkv
    %
    %   Outputs
    %     outFile  the .mkv written
    %
    %   Behavior
    %     1. An SVO2 file is an MCAP file whose side_by_side channel holds each frame as
    %        H.264 or H.265 (the ZED tab's SVO Compression), between an 8-byte header and
    %        the SDK's own bytes: the frames are copied out in order into a temporary stream
    %        for ffmpeg. Lossless SVOs (not H.264/H.265) and the older .svo format need the
    %        ZED SDK's own export instead.
    %     2. Frame k of the file is row k of frames.csv; at the recording's rate.
    %
    %   Example
    %     spot.camera.zedToMkv(fullfile('Saved Data', run, 'cameras', 'zed-OV0001'));
    %
    %   See also spot.camera.recordingToMkv, spot.camera.thermalToMkv.

    %% Defaults
    if nargin < 2
        eye = "left";
    end
    if nargin < 3
        outFile = fullfile(folder, cameraName(folder) + ".mkv");
    end
    eye = validatestring(eye, ["left", "right", "both"]);

    %% Constants
    MCAP_MAGIC = uint8([137 77 67 65 80 48 13 10]);   % "\x89MCAP0\r\n"
    OP_FOOTER = 2;             % MCAP record opcodes
    OP_CHANNEL = 4;
    OP_MESSAGE = 5;
    OP_CHUNK = 6;
    OP_DATA_END = 15;
    VIDEO_TOPIC = "/side_by_side";   % the channel's topic ends so
    HEADER_BYTES = 8;                % [bytes] before each frame in its message
    HEVC_TYPES = [32 33 34 35 39];   % H.265 NAL types that start a stream (VPS ... SEI)

    %% Frame log and file
    [frames, fps] = frameLog(folder);
    svoFile = fullfile(folder, 'zed.svo2');
    if ~isfile(svoFile)
        error('spot:camera:noFile', ...
            ['%s is missing (a zed.svo from ZED SDK before 4.1 needs the SDK''s own ' ...
             'export).'], svoFile);
    end
    fid = fopen(svoFile, 'r', 'l');
    closeSvo = onCleanup(@() fclose(fid));
    if ~isequal(fread(fid, numel(MCAP_MAGIC), '*uint8')', MCAP_MAGIC)
        error('spot:camera:notMcap', '%s is not an SVO2 (MCAP) file.', svoFile);
    end

    %% The frames, out of the MCAP records into a temporary stream
    streamFile = string(tempname) + ".video";
    removeStream = onCleanup(@() delete(streamFile));
    out = fopen(streamFile, 'w');
    state = struct('videoChannel', -1, 'nFrames', 0, 'codec', "", 'out', out);
    while true
        op = fread(fid, 1, '*uint8');
        if isempty(op) || op == OP_FOOTER || op == OP_DATA_END
            break
        end
        len = double(fread(fid, 1, '*uint64'));
        if op == OP_CHUNK || op == OP_CHANNEL || op == OP_MESSAGE
            body = fread(fid, len, '*uint8');
            if op == OP_CHUNK
                state = readRecords(chunkRecords(body), state);
            else
                state = readRecords([op; typecast(uint64(len), 'uint8')'; body], state);
            end
        else
            fseek(fid, len, 'cof');
        end
    end
    fclose(out);
    if state.nFrames == 0
        error('spot:camera:noFrames', 'No H.264/H.265 frames in %s (a lossless SVO?).', ...
            svoFile);
    end

    %% Stamps: one per frame of the file
    n = min(state.nFrames, height(frames));
    stamps = repmat("", state.nFrames, 1);
    stamps(1:n) = timeStamps(frames(1:n, :));
    if state.nFrames ~= height(frames)
        warning('spot:camera:frameCount', ...
            '%s has %d frames but frames.csv %d rows: stamped in order.', ...
            svoFile, state.nFrames, height(frames));
    end

    %% Encode: the chosen image of the side-by-side pair
    inputArgs = ["-f", state.codec, "-framerate", sprintf("%.6f", fps), "-i", streamFile];
    frameSize = videoSize(["-f", state.codec, streamFile]);
    switch eye
        case "left"
            crop = "crop=iw/2:ih:0:0";
            frameSize(1) = frameSize(1) / 2;
        case "right"
            crop = "crop=iw/2:ih:iw/2:0";
            frameSize(1) = frameSize(1) / 2;
        otherwise
            crop = "";
    end
    encodeMkv(inputArgs, crop, stamps, fps, frameSize, outFile);
    outFile = string(outFile);

    function state = readRecords(records, state)
        %READRECORDS Note the video channel and write its frames, from a run of MCAP records.
        p = 1;
        while p + 8 <= numel(records)
            recOp = records(p);
            recLen = double(typecast(records(p + 1:p + 8), 'uint64'));
            rec = records(p + 9:p + 8 + recLen);
            p = p + 9 + recLen;
            if recOp == OP_CHANNEL
                % Channel: id (2), schema id (2), topic (4-byte length, text), ...
                topicLen = double(typecast(rec(5:8), 'uint32'));
                if endsWith(char(rec(9:8 + topicLen))', VIDEO_TOPIC)
                    state.videoChannel = typecast(rec(1:2), 'uint16');
                end
            elseif recOp == OP_MESSAGE && state.videoChannel >= 0 ...
                    && typecast(rec(1:2), 'uint16') == state.videoChannel
                % Message: channel (2), sequence (4), log time (8), publish time (8), data.
                % The data: its size after these 4 bytes (4), the frame's size (4), the
                % H.264/H.265 frame, then the ZED SDK's own bytes (left out: ffmpeg would
                % take them for broken video).
                data = rec(23:end);
                frame = [];
                if numel(data) > HEADER_BYTES
                    frameLen = double(typecast(data(5:8), 'uint32'));
                    if double(typecast(data(1:4), 'uint32')) == numel(data) - 4 ...
                            && HEADER_BYTES + frameLen <= numel(data)
                        frame = data(HEADER_BYTES + 1:HEADER_BYTES + frameLen);
                    end
                end
                if isempty(frame) || ~isequal(startCode(frame), 1)
                    error('spot:camera:notH26x', ...
                        ['%s''s frames are not H.264/H.265 (a lossless SVO?): export it ' ...
                         'with the ZED SDK instead.'], svoFile);
                end
                if state.nFrames == 0
                    state.codec = codecOf(frame);
                end
                fwrite(state.out, frame);
                state.nFrames = state.nFrames + 1;
            end
        end
    end

    function codec = codecOf(frame)
        %CODECOF "h264" or "hevc", from the type of the first unit after the start code.
        first = frame(find(frame(1:4) == 1, 1) + 1);
        if ismember(bitand(bitshift(first, -1), 63), HEVC_TYPES)
            codec = "hevc";
        else
            codec = "h264";
        end
    end
end

function records = chunkRecords(chunk)
    %CHUNKRECORDS The records inside an MCAP chunk (uncompressed, or LZ4).
    %   Chunk: start time (8), end time (8), size (8), CRC (4), compression (4-byte length,
    %   text), records (8-byte length, bytes).
    compressionLen = double(typecast(chunk(29:32), 'uint32'));
    compression = char(chunk(33:32 + compressionLen))';
    at = 33 + compressionLen;
    recordsLen = double(typecast(chunk(at:at + 7), 'uint64'));
    records = chunk(at + 8:at + 7 + recordsLen);
    if strcmp(compression, 'lz4')
        records = lz4Frame(records);
    elseif ~isempty(compression)
        error('spot:camera:compression', ...
            'This SVO''s chunks are %s-compressed, which MATLAB can''t read.', compression);
    end
end

function start = startCode(data)
    %STARTCODE Where the first H.264/H.265 start code (0 0 1 or 0 0 0 1) begins, or [].
    SEARCH = 4;                % [bytes] a frame starts with one
    start = [];
    for k = 1:min(SEARCH, numel(data) - 3)
        if data(k) == 0 && data(k + 1) == 0
            if data(k + 2) == 1
                start = k;
                return
            elseif data(k + 2) == 0 && data(k + 3) == 1
                start = k;
                return
            end
        end
    end
end
