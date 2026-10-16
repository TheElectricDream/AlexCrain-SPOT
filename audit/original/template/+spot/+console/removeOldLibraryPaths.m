function removed = removeOldLibraryPaths()
    %REMOVEOLDLIBRARYPATHS Take old Custom_Library folders off the MATLAB path.
    %   removed = spot.console.removeOldLibraryPaths() removes every Custom_Library folder (and
    %   its subfolders) from the path, and closes MainLibrary if loaded. SPOT no longer uses that
    %   library (its code is now spot.hardware), but old copies would still define classes with
    %   the blocks' old names.
    %
    %   Outputs
    %     removed  cell array of the folders removed
    %
    %   See also spot.console.start.

    % Path entries named Custom_Library, or inside one.
    folders = strsplit(path, pathsep);
    removed = folders(~cellfun(@isempty, ...
        regexp(folders, '[\\/]Custom_Library([\\/]|$)', 'once')));
    if ~isempty(removed)
        rmpath(removed{:});
    end

    % Close the old library if loaded.
    try
        if bdIsLoaded('MainLibrary')
            close_system('MainLibrary', 0);
        end
    catch
        % Simulink not available: nothing is loaded
    end
end
