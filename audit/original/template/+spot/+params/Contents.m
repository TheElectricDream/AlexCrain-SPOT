% PARAMS  Settings: the project's values from Constants.m, timing, putting them into the workspace
%
%   apply             - Put the settings into the base workspace, as the Simulink diagrams expect them.
%   calibrationRecord - The lab calibration's full record, as the project took it.
%   constantsCatalog  - Every constant of Constants.m, in its sections, for the console's window.
%   fromConstants     - The project's settings, as Constants.m has them.
%   fromLegacyState   - Convert the old SPOT GUI's saved state (Save GUI State) to settings.
%   labCalibration    - The lab's calibration as this project took it (Constants.LAB_*).
%   parseConstant     - A value typed in the console, as the type of the constant it replaces.
%   platformSelection - The diagrams' platformSelection code for a set of platforms.
%   setPath           - Set one value of a settings struct by its dotted path (as the page names it).
%   templateValues    - The settings' constants as the repository's template has them (for Reset).
%   timing            - Phase durations, end times and total time from the settings.
%   toConstants       - The constants a settings struct stands for (the inverse of fromConstants).
%   writeConstants    - Put new values into Constants.m, keeping everything else as it is.
