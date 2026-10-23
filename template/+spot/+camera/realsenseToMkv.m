function outFiles = realsenseToMkv(folder)
    %REALSENSETOMKV A RealSense recording as .mkv videos, each frame stamped with its times.
    %   outFiles = spot.camera.realsenseToMkv(folder) converts the colour stream of a
    %   RealSense L515 recording (realsense_<n>.bag) into a video next to each bag
    %   (realsense_<n>.mkv), each frame stamped in its top-left corner with the experiment
    %   time, the PhaseSpace time and its frame number. Needs the ROS Toolbox.
    %
    %   Inputs
    %     folder    the camera's folder in a run (Saved Data/<run>/cameras/realsense-...)
    %
    %   Outputs
    %     outFiles  the .mkv files written, a string array (one per bag: a recording starts
    %               a new bag when its streams restart)
    %
    %   Behavior
    %     1. Each colour image's frame number (its ROS header's seq, as librealsense writes
    %        it) finds its row of frames.csv (color_frame); an image with none gets only
    %        its frame number.
    %     2. At the rate of the bag's rows of frames.csv. The depth stream is left out.
    %
    %   Example
    %     spot.camera.realsenseToMkv(fullfile('Saved Data', run, 'cameras', 'realsense-1'));
    %
    %   See also spot.camera.recordingToMkv, spot.camera.thermalToMkv.

    %% Constants
    COLOR_TOPIC = "Color_0/image/data";   % librealsense's colour images end so
    BATCH = 100;                          % images read at a time

    %% Frame log and bags
    if ~exist('rosbagreader', 'file')
        error('spot:camera:noRos', 'Reading a RealSense .bag needs the ROS Toolbox.');
    end
    [frames, fps] = frameLog(folder);
    bags = dir(fullfile(folder, 'realsense_*.bag'));
    if isempty(bags)
        error('spot:camera:noFile', 'No realsense_<n>.bag in %s.', folder);
    end

    %% Each bag
    outFiles = strings(0, 1);
    for b = 1:numel(bags)
        bagFile = fullfile(folder, bags(b).name);
        bag = rosbagreader(bagFile);
        topics = string(bag.AvailableTopics.Properties.RowNames);
        topic = topics(endsWith(topics, COLOR_TOPIC));
        if isempty(topic)
            warning('spot:camera:noColor', '%s has no colour stream: skipped.', bagFile);
            continue
        end
        images = select(bag, 'Topic', topic(1));

        % This bag's rows of frames.csv, and its rate.
        rows = frames(frames.file == bags(b).name, :);
        bagFps = fps;
        if height(rows) > 1
            bagFps = 1 / median(diff(rows.orin_time));
        end

        % The colour images, as raw RGB into a temporary file; each one's stamp.
        rgbFile = string(tempname) + ".rgb";
        removeRgb = onCleanup(@() delete(rgbFile));
        out = fopen(rgbFile, 'w');
        n = images.NumMessages;
        stamps = strings(n, 1);
        for first = 1:BATCH:n
            msgs = readMessages(images, first:min(first + BATCH - 1, n), ...
                'DataFormat', 'struct');
            for j = 1:numel(msgs)
                k = first + j - 1;
                img = rosReadImage(msgs{j});
                if k == 1
                    frameSize = [size(img, 2) size(img, 1)];
                end
                fwrite(out, permute(img, [3 2 1]));
                row = find(rows.color_frame == double(msgs{j}.Header.Seq), 1);
                if isempty(row)
                    stamps(k) = sprintf("frame %d", msgs{j}.Header.Seq);
                else
                    stamps(k) = timeStamps(rows(row, :)) + ...
                        sprintf(" (colour %d)", msgs{j}.Header.Seq);
                end
            end
        end
        fclose(out);

        % Encode.
        [~, name] = fileparts(bagFile);
        outFile = string(fullfile(folder, name + ".mkv"));
        inputArgs = ["-f", "rawvideo", "-pix_fmt", "rgb24", ...
                     "-video_size", sprintf("%dx%d", frameSize), ...
                     "-framerate", sprintf("%.6f", bagFps), "-i", rgbFile];
        encodeMkv(inputArgs, "", stamps, bagFps, frameSize, outFile);
        outFiles(end + 1, 1) = outFile; %#ok<AGROW>
        clear removeRgb
    end
end
