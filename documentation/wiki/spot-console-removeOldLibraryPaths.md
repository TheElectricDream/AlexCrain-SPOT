# spot.console.removeOldLibraryPaths

**File:** `+spot/+console/removeOldLibraryPaths.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/removeOldLibraryPaths.m -->

## What it is for

Earlier SPOT versions had a Simulink library (`Custom_Library`, `MainLibrary.slx`) whose classes had the same names as some of the diagram's blocks. If such a folder is still on the MATLAB path (from an old project, or the shared copy at the top of the old repository) its classes shadow the `spot.hardware` ones. This takes every `Custom_Library` folder off the path and closes `MainLibrary` if it is loaded.

## Line by line

**Lines 9–13.** Split the path, keep the entries whose path contains a `Custom_Library` folder, remove them.

**Lines 14–20.** Close `MainLibrary` without saving; ignore errors (no Simulink, nothing loaded).

<!-- xref -->
- **Used by:** `+spot/+console/start.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/removeOldLibraryPaths.m</b> (21 lines; click to show)</summary>

```matlab
   1  function removed = removeOldLibraryPaths()
   2  %REMOVEOLDLIBRARYPATHS Take old Custom_Library folders off the MATLAB path.
   3  %   removed = spot.console.removeOldLibraryPaths() removes every folder named Custom_Library (and
   4  %   its subfolders) from the path, and closes MainLibrary if it is loaded. SPOT no longer uses
   5  %   that library: the diagram's blocks are its own, and the code behind them is the spot.hardware
   6  %   package. Old copies (this project's from before, another project's, or the shared one at the
   7  %   top of the SPOT repository) would still define classes with the blocks' old names, so they are
   8  %   taken off the path. Returns the folders removed.
   9  folders = strsplit(path, pathsep);
  10  removed = folders(~cellfun(@isempty, regexp(folders, '[\\/]Custom_Library([\\/]|$)', 'once')));
  11  if ~isempty(removed)
  12      rmpath(removed{:});
  13  end
  14  try
  15      if bdIsLoaded('MainLibrary')
  16          close_system('MainLibrary', 0);
  17      end
  18  catch
  19      % Simulink not available: nothing is loaded
  20  end
  21  end
```

</details>
<!-- /listing -->
