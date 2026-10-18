function [values, file] = templateValues(root)
    %TEMPLATEVALUES The settings' constants as the repository's template has them (for Reset).
    %   [values, file] = spot.params.templateValues(root) reads template/Constants.m, two
    %   folders above a project made by New_SPOT_Project in projects/, and returns the values
    %   of the constants that are settings (spot.params.toConstants' names), so the console's
    %   Reset can write them back into the project's Constants.m. The template's class is read
    %   under another name in a temporary folder (two classes can't share a name).
    %
    %   Inputs
    %     root    project folder
    %
    %   Outputs
    %     values  struct, constant name -> the template's value; [] when there is no template
    %             next to the project (or root is the template itself)
    %     file    the template's Constants.m ('' if none)
    %
    %   See also spot.params.writeConstants, spot.worker.Worker.

    %% Constants
    TEMP_CLASS = 'SPOTTemplateConstants';   % the template's class, renamed to read it

    %% The template
    values = [];
    file = fullfile(root, '..', '..', 'template', 'Constants.m');
    if ~isfile(file) || strcmp(fullfile(root, 'Constants.m'), file) ...
            || isequal(dir(file), dir(fullfile(root, 'Constants.m')))
        file = '';
        return
    end
    file = char(java.io.File(file).getCanonicalPath());

    %% Read it under another name
    folder = tempname;
    mkdir(folder);
    cleanup = onCleanup(@() cleanUp(folder, TEMP_CLASS));
    text = regexprep(fileread(file), '\<Constants\>', TEMP_CLASS);
    fid = fopen(fullfile(folder, [TEMP_CLASS '.m']), 'w');
    fwrite(fid, text, 'char');
    fclose(fid);
    addpath(folder);
    template = feval([TEMP_CLASS '.all']);

    %% The settings among them
    wanted = fieldnames(spot.params.toConstants(spot.params.fromConstants()));
    values = struct();
    for k = 1:numel(wanted)
        if isfield(template, wanted{k})
            values.(wanted{k}) = template.(wanted{k});
        end
    end
end

function cleanUp(folder, className)
    % Forget the temporary class and its folder.
    rmpath(folder);
    clear(className);
    try
        rmdir(folder, 's');
    catch
    end
end
