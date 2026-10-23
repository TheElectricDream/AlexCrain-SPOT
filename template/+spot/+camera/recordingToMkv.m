function outFiles = recordingToMkv(run)
    %RECORDINGTOMKV A run's camera recordings as .mkv videos, each frame stamped with its times.
    %   outFiles = spot.camera.recordingToMkv(run) converts each camera in a run's cameras
    %   folder with the function for its kind (session.json), each video next to its
    %   camera's files.
    %
    %   Inputs
    %     run       the run's folder in Saved Data (the one holding cameras/)
    %
    %   Outputs
    %     outFiles  the .mkv files written, a string array
    %
    %   Behavior
    %     1. zed: spot.camera.zedToMkv (left image); realsense: spot.camera.realsenseToMkv;
    %        dvs: spot.camera.dvxplorerToMkv (30 frames/s); thermal:
    %        spot.camera.thermalToMkv; uvc and demo: spot.camera.usbToMkv.
    %     2. A camera that fails is reported and skipped; the others are still converted.
    %
    %   Example
    %     spot.camera.recordingToMkv(fullfile('Saved Data', 'MyDiagram_20261020_101500'));
    %
    %   See also spot.camera.thermalToMkv, spot.camera.zedToMkv, spot.camera.usbToMkv,
    %   spot.camera.realsenseToMkv, spot.camera.dvxplorerToMkv.

    %% The cameras and their kinds
    camerasFolder = fullfile(run, 'cameras');
    sessionFile = fullfile(camerasFolder, 'session.json');
    if ~isfile(sessionFile)
        error('spot:camera:noRecording', 'No camera recording in %s (no %s).', ...
            run, sessionFile);
    end
    session = jsondecode(fileread(sessionFile));
    ids = fieldnames(session.cameras);

    %% Convert each
    outFiles = strings(0, 1);
    for j = 1:numel(ids)
        % jsondecode makes ids valid field names: the folder is the camera's own id.
        folder = fullfile(camerasFolder, cameraFolder(camerasFolder, ids{j}));
        kind = session.cameras.(ids{j}).kind;
        try
            switch kind
                case "zed"
                    written = spot.camera.zedToMkv(folder);
                case "realsense"
                    written = spot.camera.realsenseToMkv(folder);
                case "dvs"
                    written = spot.camera.dvxplorerToMkv(folder);
                case "thermal"
                    written = spot.camera.thermalToMkv(folder);
                case {"uvc", "demo"}
                    written = spot.camera.usbToMkv(folder);
                otherwise
                    warning('spot:camera:unknownKind', '%s: unknown camera kind "%s".', ...
                        folder, kind);
                    continue
            end
            outFiles = [outFiles; written(:)]; %#ok<AGROW>
        catch err
            warning('spot:camera:failed', '%s was not converted: %s', folder, err.message);
        end
    end
end

function name = cameraFolder(camerasFolder, fieldName)
    %CAMERAFOLDER The folder whose name jsondecode turned into this field name.
    entries = dir(camerasFolder);
    entries = entries([entries.isdir] & ~startsWith({entries.name}, '.'));
    names = {entries.name};
    match = strcmp(matlab.lang.makeValidName(names), fieldName);
    if any(match)
        name = names{find(match, 1)};
    else
        name = fieldName;
    end
end
