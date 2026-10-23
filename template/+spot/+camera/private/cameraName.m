function name = cameraName(folder)
    %CAMERANAME A camera's id: its folder's name, dots included (thermal-1-2.4).
    %   name = cameraName(folder) for spot.camera's converters, which name their videos so.
    %
    %   Inputs
    %     folder  the camera's folder, with or without a trailing separator
    %
    %   Outputs
    %     name    the folder's last part, a string
    %
    %   See also spot.camera.thermalToMkv.

    %% The last part of the path (fileparts alone would take ".4" for an extension)
    [~, base, ext] = fileparts(regexprep(char(folder), '[\\/]+$', ''));
    name = string(base) + ext;
end
