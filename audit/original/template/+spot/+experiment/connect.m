function info = connect(root, password)
    %CONNECT Connect MATLAB to the build board (the TX2), as jetson() does for a platform.
    %   info = spot.experiment.connect(root) reuses the credentials MATLAB saved the last time it
    %   connected to the TX2.
    %   info = spot.experiment.connect(root, password) connects with a password (MATLAB then
    %   remembers it). Needs the MATLAB Coder Support Package for NVIDIA Jetson.
    %
    %   Inputs
    %     root      project folder
    %     password  the TX2's password (optional)
    %
    %   Outputs
    %     info      address, user, board (spot.experiment.buildBoard)
    %
    %   See also spot.experiment.build, spot.experiment.buildBoard.

    board = spot.experiment.buildBoard(root);

    %% Connect
    if nargin > 1 && ~isempty(password)
        device = jetson(board.address, board.user, password);
    else
        % The last board MATLAB connected to, with its saved password: must be the TX2.
        device = jetson();
        if ~strcmp(device.DeviceAddress, board.address)
            error('spot:experiment:connect', ...
                ['MATLAB was last connected to %s, not the TX2 (%s). Connect once with the ' ...
                 'TX2''s password.'], device.DeviceAddress, board.address);
        end
    end

    %% Remember it for the build
    setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultIpAddress', board.address);
    setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultUserName', board.user);
    info = struct('address', device.DeviceAddress, 'user', board.user, 'board', board);
end
