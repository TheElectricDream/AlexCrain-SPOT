function info = connect(root, password)
%CONNECT Connect MATLAB to the build board (the TX2), as jetson() does for a platform.
%   info = spot.experiment.connect(projectFolder) reuses the credentials MATLAB saved the last time
%   it connected to the TX2; spot.experiment.connect(projectFolder, password) connects with a
%   password (MATLAB then remembers it). Needs the MATLAB Coder Support Package for NVIDIA Jetson.
b = spot.experiment.buildBoard(root);
if nargin > 1 && ~isempty(password)
    hw = jetson(b.address, b.user, password);
else
    hw = jetson();   % the last board MATLAB connected to, with its saved password
    if ~strcmp(hw.DeviceAddress, b.address)
        error('spot:experiment:connect', ['MATLAB was last connected to %s, not the TX2 (%s). ' ...
            'Connect once with the TX2''s password.'], hw.DeviceAddress, b.address);
    end
end
setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultIpAddress', b.address);
setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultUserName', b.user);
info = struct('address', hw.DeviceAddress, 'user', b.user, 'board', b);
end
