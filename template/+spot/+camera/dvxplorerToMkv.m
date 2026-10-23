function outFile = dvxplorerToMkv(folder, fps, outFile)
    %DVXPLORERTOMKV An event camera's recording as an .mkv video, each frame stamped with its times.
    %   outFile = spot.camera.dvxplorerToMkv(folder) renders a DVXplorer recording
    %   (events.aedat4, or events.bin) as a video next to it: each frame shows the events of
    %   its 1/30 s (on white, off black, on grey), stamped in its top-left corner with the
    %   experiment time and PhaseSpace time at its end, its frame number and its events.
    %   outFile = spot.camera.dvxplorerToMkv(folder, fps) renders fps frames a second.
    %   outFile = spot.camera.dvxplorerToMkv(folder, fps, outFile) writes it there instead.
    %
    %   Inputs
    %     folder   the camera's folder in a run (Saved Data/<run>/cameras/dvs-...)
    %     fps      frames per second of event time, default 30
    %     outFile  the .mkv to write, default <folder>/<camera>.mkv
    %
    %   Outputs
    %     outFile  the .mkv written
    %
    %   Behavior
    %     1. events.aedat4 (dv-processing's AEDAT4) is read packet by packet, its LZ4
    %        decompressed with the Java that ships with MATLAB; events.bin by its layout
    %        in events_bin.json. ZSTD-compressed AEDAT4 files can't be read.
    %     2. Each frame's end, in the camera's microseconds, becomes experiment and
    %        PhaseSpace time through a straight line fitted to frames.csv (each event
    %        batch's last_us against its times).
    %
    %   Example
    %     spot.camera.dvxplorerToMkv(fullfile('Saved Data', run, 'cameras', 'dvs-1'));
    %
    %   See also spot.camera.recordingToMkv, spot.camera.thermalToMkv.

    %% Defaults
    if nargin < 2
        fps = 30;
    end
    if nargin < 3
        outFile = fullfile(folder, cameraName(folder) + ".mkv");
    end

    %% Constants
    DEFAULT_SIZE = [640 480];  % [pixels] DVXplorer and DVXplorer Micro, if not in the file
    GREY = 128;                % no event
    US = 1e6;                  % [us/s]
    BIN_ROWS = 1e6;            % events.bin rows read at a time

    %% Frame log: event time [us] to experiment and PhaseSpace time [s]
    frames = frameLog(folder);
    toExperiment = timeLine(frames.last_us, frames.experiment_time);
    toPhaseSpace = timeLine(frames.last_us, frames.phasespace_time);

    %% Event source
    aedatFile = fullfile(folder, 'events.aedat4');
    binFile = fullfile(folder, 'events.bin');
    if isfile(aedatFile)
        source = openAedat(aedatFile);
    elseif isfile(binFile)
        source = openBin(binFile, fullfile(folder, 'events_bin.json'));
    else
        error('spot:camera:noFile', 'Neither events.aedat4 nor events.bin is in %s.', folder);
    end
    closeSource = onCleanup(@() fclose(source.fid));
    if isempty(source.frameSize)
        source.frameSize = DEFAULT_SIZE;
    end
    nCols = source.frameSize(1);
    nRows = source.frameSize(2);

    %% Render: events into frames of 1/fps, as grey raw video in a temporary file
    greyFile = string(tempname) + ".gray";
    removeGrey = onCleanup(@() delete(greyFile));
    out = fopen(greyFile, 'w');
    frameUs = US / fps;
    frameEnd = [];             % [us] end of the frame being filled
    img = [];
    nEvents = 0;
    stamps = strings(0, 1);
    endsUs = zeros(0, 1);
    while true
        [ev, source] = source.next(source);
        if isempty(ev.t)
            break
        end
        if isempty(frameEnd)
            frameEnd = double(ev.t(1)) + frameUs;
            img = repmat(uint8(GREY), nRows, nCols);
        end

        % Close every frame that ends before the last of these events.
        t = double(ev.t);
        k = 1;
        while k <= numel(t)
            last = find(t(k:end) < frameEnd, 1, 'last') + k - 1;
            if isempty(last)
                last = k - 1;
            end
            img = paint(img, ev, k, last);
            nEvents = nEvents + last - k + 1;
            k = last + 1;
            if k <= numel(t)
                [stamps, endsUs] = flush(out, img', stamps, endsUs, frameEnd, nEvents);
                img(:) = GREY;
                nEvents = 0;
                frameEnd = frameEnd + frameUs;
            end
        end
    end
    if ~isempty(frameEnd)
        [stamps, endsUs] = flush(out, img', stamps, endsUs, frameEnd, nEvents);
    end
    fclose(out);
    if isempty(stamps)
        error('spot:camera:noEvents', 'No events in %s.', folder);
    end

    %% Stamps: times at each frame's end
    times = struct('frame', (0:numel(stamps) - 1)', ...
        'experiment_time', toExperiment(endsUs), 'phasespace_time', toPhaseSpace(endsUs));
    stamps = timeStamps(struct2table(times)) + stamps;

    %% Encode
    inputArgs = ["-f", "rawvideo", "-pix_fmt", "gray", ...
                 "-video_size", sprintf("%dx%d", nCols, nRows), ...
                 "-framerate", sprintf("%.6f", fps), "-i", greyFile];
    encodeMkv(inputArgs, "", stamps, fps, [nCols nRows], outFile);
    outFile = string(outFile);

    function img = paint(img, ev, first, last)
        %PAINT Events first..last onto a frame: on white, off black (the later one wins).
        if last < first
            return
        end
        idx = sub2ind([nRows nCols], double(ev.y(first:last)) + 1, ...
            double(ev.x(first:last)) + 1);
        img(idx) = uint8(255 * ev.on(first:last));
    end

    function source = openBin(file, layoutFile)
        %OPENBIN events.bin: rows of the numpy layout in events_bin.json.
        layout = jsondecode(fileread(layoutFile));
        fields = struct('name', {}, 'offset', {}, 'type', {});
        offset = 0;

        % dtype: [name, numpy type code] per field (jsondecode gives a cell of cells).
        for j = 1:numel(layout.dtype)
            name = string(layout.dtype{j}{1});
            code = char(layout.dtype{j}{2});
            nBytes = str2double(regexp(code, '\d+$', 'match', 'once'));
            fields(end + 1) = struct('name', name, 'offset', offset, ...
                'type', numpyType(code)); %#ok<AGROW>
            offset = offset + nBytes;
        end
        source = struct('fid', fopen(file, 'r', 'l'), 'frameSize', [], ...
            'rowBytes', offset, 'fields', fields, 'next', @nextBin);
    end

    function [ev, source] = nextBin(source)
        %NEXTBIN The next rows of events.bin.
        raw = fread(source.fid, [source.rowBytes, BIN_ROWS], '*uint8');
        ev = struct('t', [], 'x', [], 'y', [], 'on', []);
        if isempty(raw)
            return
        end
        names = ["timestamp", "x", "y", "polarity"];
        keys = ["t", "x", "y", "on"];
        for j = 1:numel(names)
            f = source.fields([source.fields.name] == names(j));
            bytes = raw(f.offset + 1:f.offset + sizeOf(f.type), :);
            ev.(keys(j)) = typecast(bytes(:), f.type);
        end
        ev.on = ev.on ~= 0;
    end
end

function line = timeLine(us, seconds)
    %TIMELINE A straight line from camera time [us] to a clock [s], fitted where it is known.
    known = ~isnan(us) & ~isnan(seconds);
    if nnz(known) < 2
        line = @(t) nan(size(t));
        return
    end
    us0 = us(find(known, 1));
    p = polyfit(us(known) - us0, seconds(known), 1);
    line = @(t) polyval(p, t - us0);
end

function [stamps, endsUs] = flush(out, frame, stamps, endsUs, frameEnd, nEvents)
    %FLUSH Write a finished frame (rows in order) and note its end and events.
    fwrite(out, frame);
    stamps(end + 1, 1) = sprintf("\\Nevents %d", nEvents);
    endsUs(end + 1, 1) = frameEnd;
end

function source = openAedat(file)
    %OPENAEDAT events.aedat4: its header (compression, sensor size), ready for its packets.
    %   File: "#!AER-DAT4.0\r\n", the header's size (4), the header, then packets: stream
    %   id (4), size (4), compressed bytes (an event packet's FlatBuffer is marked "EVTS"),
    %   up to the data table. The header is a FlatBuffer table: compression (int32: 0 none,
    %   1 LZ4, 2 LZ4 high, 3 and 4 ZSTD), the data table's position (int64), and an XML
    %   description of the streams (their sensor size).
    MAGIC = '#!AER-DAT4.0';
    COMPRESSION_NONE = 0;
    LZ4_CODES = [1 2];
    fid = fopen(file, 'r', 'l');
    % Bytes, not '*char': that reads text in the file's encoding, which binary data upsets.
    if ~strcmp(char(fread(fid, numel(MAGIC), '*uint8'))', MAGIC) %#ok<FREAD>
        fclose(fid);
        error('spot:camera:notAedat', '%s is not an AEDAT4 file.', file);
    end
    fseek(fid, 2, 'cof');
    headerSize = fread(fid, 1, 'int32');
    header = fread(fid, headerSize, '*uint8');

    % The header table's fields, through its vtable (an absent field keeps its default).
    tableAt = double(typecast(header(1:4), 'uint32'));
    vtableAt = tableAt - double(typecast(header(tableAt + 1:tableAt + 4), 'int32'));
    nFields = (double(typecast(header(vtableAt + 1:vtableAt + 2), 'uint16')) - 4) / 2;
    fieldAt = zeros(1, 2);
    for j = 1:min(2, nFields)
        at = vtableAt + 4 + 2 * (j - 1);
        fieldAt(j) = double(typecast(header(at + 1:at + 2), 'uint16'));
    end
    compression = COMPRESSION_NONE;
    if fieldAt(1) > 0
        compression = double(typecast(header(tableAt + fieldAt(1) + 1: ...
            tableAt + fieldAt(1) + 4), 'int32'));
    end
    dataTableAt = Inf;
    if fieldAt(2) > 0
        dataTableAt = double(typecast(header(tableAt + fieldAt(2) + 1: ...
            tableAt + fieldAt(2) + 8), 'int64'));
    end
    if dataTableAt < 0
        dataTableAt = Inf;
    end
    if compression ~= COMPRESSION_NONE && ~ismember(compression, LZ4_CODES)
        fclose(fid);
        error('spot:camera:compression', ...
            ['%s is ZSTD-compressed, which MATLAB can''t read: convert it with ' ...
             'dv-processing (LZ4 or no compression).'], file);
    end

    % The sensor's size, from the XML.
    xml = char(header)';
    sizeX = regexp(xml, '<attr key="sizeX" type="int">(\d+)<', 'tokens', 'once');
    sizeY = regexp(xml, '<attr key="sizeY" type="int">(\d+)<', 'tokens', 'once');
    frameSize = [];
    if ~isempty(sizeX) && ~isempty(sizeY)
        frameSize = [str2double(sizeX{1}) str2double(sizeY{1})];
    end
    source = struct('fid', fid, 'frameSize', frameSize, 'dataTableAt', dataTableAt, ...
        'lz4', compression ~= COMPRESSION_NONE, 'next', @nextAedat);
end

function [ev, source] = nextAedat(source)
    %NEXTAEDAT The events of the next event packet (other streams' packets are skipped).
    %   A packet is a size-prefixed FlatBuffer EventPacket: its table's first field is a
    %   vector of 16-byte events (timestamp int64 [us], x int16, y int16, polarity bool).
    EVENT_BYTES = 16;
    ev = struct('t', [], 'x', [], 'y', [], 'on', []);
    while ftell(source.fid) < source.dataTableAt
        head = fread(source.fid, 2, 'int32');
        if numel(head) < 2
            return
        end
        data = fread(source.fid, head(2), '*uint8');
        if source.lz4
            data = lz4Frame(data);
        end

        % FlatBuffer: size prefix (4), root table offset (4), "EVTS"; the table's vtable
        % gives its first field, an offset to the events vector (count, then events).
        if numel(data) < 12 || ~strcmp(char(data(9:12))', 'EVTS')
            continue
        end
        buf = data(5:end);
        u32 = @(at) double(typecast(buf(at + 1:at + 4), 'uint32'));
        tableAt = u32(0);
        vtableAt = tableAt - double(typecast(buf(tableAt + 1:tableAt + 4), 'int32'));
        fieldOffset = double(typecast(buf(vtableAt + 5:vtableAt + 6), 'uint16'));
        if fieldOffset == 0
            continue
        end
        vectorAt = tableAt + fieldOffset + u32(tableAt + fieldOffset);
        count = u32(vectorAt);
        if count == 0
            continue
        end
        raw = reshape(buf(vectorAt + 5:vectorAt + 4 + count * EVENT_BYTES), EVENT_BYTES, []);
        ev.t = typecast(reshape(raw(1:8, :), [], 1), 'int64');
        ev.x = typecast(reshape(raw(9:10, :), [], 1), 'int16');
        ev.y = typecast(reshape(raw(11:12, :), [], 1), 'int16');
        ev.on = raw(13, :)' ~= 0;
        return
    end
end

function type = numpyType(code)
    %NUMPYTYPE The MATLAB class of a numpy type code ('<i8', '<i2', '|b1', '|u1', ...).
    kind = regexp(code, '[a-zA-Z]', 'match', 'once');
    nBytes = str2double(regexp(code, '\d+$', 'match', 'once'));
    switch kind
        case 'i'
            type = sprintf('int%d', 8 * nBytes);
        case {'u', 'b'}
            type = sprintf('uint%d', 8 * nBytes);
        otherwise
            type = 'uint8';
    end
end

function n = sizeOf(type)
    %SIZEOF Bytes of an integer class.
    n = numel(typecast(zeros(1, 1, type), 'uint8'));
end
