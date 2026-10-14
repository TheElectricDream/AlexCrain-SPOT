function b = buildBoard(root, s)
%BUILDBOARD Where experiments are built: the lab console (Jetson TX2).
%   b = spot.experiment.buildBoard(projectFolder, s) returns address, user, buildDir and
%   workspaceDir: MATLAB copies the generated code to the TX2 and compiles it in workspaceDir; the
%   lab console then copies the program to the platforms (see spot.experiment.build). The address
%   is the settings' labConsoleAddress (s, or the project's spot_settings.json without it).
release = ['R' version('-release')];
b = struct();
if nargin < 2
    file = fullfile(root, 'spot_settings.json');
    if isfile(file)
        s = spot.params.load(file);
    else
        s = spot.params.defaults();
    end
end
b.address = strtrim(char(s.labConsoleAddress));
b.user = 'spot-tx2';
b.buildDir = ['/home/' b.user '/ExperimentSoftware/'];
% One folder per project on the TX2, named after the project's path as the old GUI did
compileDir = regexprep(root, '[:]', '');
compileDir = regexprep(compileDir, '[ ()]', '_');
compileDir = strrep(compileDir, '\', '/');
b.workspaceDir = [b.buildDir 'MATLAB_ws/' release '/' regexprep(compileDir, '^/+', '') '/'];
end
