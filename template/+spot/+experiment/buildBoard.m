function board = buildBoard(root, s)
    %BUILDBOARD Where experiments are built: the lab console (Jetson TX2).
    %   board = spot.experiment.buildBoard(root, s) returns the TX2's address and folders. MATLAB
    %   copies the generated code to the TX2 and compiles it in workspaceDir; the lab console
    %   then copies the program to the platforms (spot.experiment.build).
    %
    %   Inputs
    %     root   project folder
    %     s      settings (default: the project's spot_settings.json, else the defaults)
    %
    %   Outputs
    %     board  address (the settings' labConsoleAddress), user, buildDir, workspaceDir
    %
    %   See also spot.experiment.build, spot.experiment.connect.

    release = ['R' version('-release')];
    board = struct();

    %% Defaults
    if nargin < 2
        file = fullfile(root, 'spot_settings.json');
        if isfile(file)
            s = spot.params.load(file);
        else
            s = spot.params.defaults();
        end
    end

    %% Board
    board.address = strtrim(char(s.labConsoleAddress));
    board.user = 'spot-tx2';
    board.buildDir = ['/home/' board.user '/ExperimentSoftware/'];

    % One folder per project on the TX2, named after the project's path as the old GUI did.
    compileDir = regexprep(root, '[:]', '');
    compileDir = regexprep(compileDir, '[ ()]', '_');
    compileDir = strrep(compileDir, '\', '/');
    board.workspaceDir = [board.buildDir 'MATLAB_ws/' release '/' ...
                          regexprep(compileDir, '^/+', '') '/'];
end
