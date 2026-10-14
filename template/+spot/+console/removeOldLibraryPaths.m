function removed = removeOldLibraryPaths()
%REMOVEOLDLIBRARYPATHS Take old Custom_Library folders off the MATLAB path.
%   removed = spot.console.removeOldLibraryPaths() removes every folder named Custom_Library (and
%   its subfolders) from the path, and closes MainLibrary if it is loaded. SPOT no longer uses
%   that library: the diagram's blocks are its own, and the code behind them is the spot.hardware
%   package. Old copies (this project's from before, another project's, or the shared one at the
%   top of the SPOT repository) would still define classes with the blocks' old names, so they are
%   taken off the path. Returns the folders removed.
folders = strsplit(path, pathsep);
removed = folders(~cellfun(@isempty, regexp(folders, '[\\/]Custom_Library([\\/]|$)', 'once')));
if ~isempty(removed)
    rmpath(removed{:});
end
try
    if bdIsLoaded('MainLibrary')
        close_system('MainLibrary', 0);
    end
catch
    % Simulink not available: nothing is loaded
end
end
