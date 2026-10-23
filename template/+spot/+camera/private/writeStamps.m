function writeStamps(file, stamps, fps, frameSize)
    %WRITESTAMPS An ASS subtitle file that stamps each video frame with its own text.
    %   writeStamps(file, stamps, fps, frameSize) writes the text that ffmpeg's ass filter
    %   burns into the top-left corner of each frame (spot.camera's converters).
    %
    %   Inputs
    %     file       the .ass file to write
    %     stamps     Nx1 string, frame k's text (lines separated by "\N")
    %     fps        the video's frame rate [frames/s]; frame k is shown at (k - 1)/fps [s]
    %     frameSize  [width height] of the video the stamps go on [pixels]
    %
    %   Outputs
    %     (none)
    %
    %   Behavior
    %     1. White fixed-width text (Liberation Mono; Consolas on Windows) on a
    %        half-transparent black box, sized to the frame.
    %     2. Frame k's text from half a frame before it to half a frame after it, so that
    %        the centisecond times of ASS never put it on the wrong frame (below 100 fps).
    %
    %   See also spot.camera.thermalToMkv, spot.camera.zedToMkv.

    %% Constants
    LINES_PER_FRAME = 30;      % the font is this fraction of the frame's height
    MIN_FONT = 12;             % [pixels]
    MARGIN = 6;                % [pixels] from the frame's edges
    LAST_END = 360000;         % [s] the last frame's text stays to the end

    %% Header and style
    % A fixed-width font that ships with the system, so the numbers line up.
    font = "Liberation Mono";
    if ispc
        font = "Consolas";
    end
    fontSize = max(MIN_FONT, round(frameSize(2) / LINES_PER_FRAME));
    header = [ ...
        "[Script Info]"
        "ScriptType: v4.00+"
        sprintf("PlayResX: %d", frameSize(1))
        sprintf("PlayResY: %d", frameSize(2))
        "WrapStyle: 2"
        ""
        "[V4+ Styles]"
        "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, " + ...
            "BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, " + ...
            "Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, " + ...
            "Encoding"
        sprintf("Style: Stamp,%s,%d,&H00FFFFFF,&H00FFFFFF,&H60000000," + ...
            "&H60000000,0,0,0,0,100,100,0,0,3,2,0,7,%d,%d,%d,1", ...
            font, fontSize, MARGIN, MARGIN, MARGIN)
        ""
        "[Events]"
        "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text"];

    %% One event per frame
    k = (1:numel(stamps))';
    startTime = max(0, (k - 1.5) / fps);
    endTime = (k - 0.5) / fps;
    endTime(end) = LAST_END;
    events = "Dialogue: 0," + assTime(startTime) + "," + assTime(endTime) + ...
        ",Stamp,,0,0,0,," + stamps(:);

    %% Write
    writelines([header; events], file, 'Encoding', 'UTF-8');
end

function txt = assTime(seconds)
    %ASSTIME Times in ASS's h:mm:ss.cc form.
    cs = round(seconds * 100);
    txt = compose("%d:%02d:%02d.%02d", floor(cs / 360000), mod(floor(cs / 6000), 60), ...
        mod(floor(cs / 100), 60), mod(cs, 100));
end
