% PARAMS  Settings: defaults, loading/saving, timing, putting them into the workspace
%
%   apply             - Put the settings into the base workspace, as the Simulink diagrams expect them.
%   defaults          - Default SPOT settings.
%   fromLegacyState   - Convert the old SPOT GUI's saved state (Save GUI State) to settings.
%   ipv4              - An IPv4 address as four numbers: [192 168 1 116] from '192.168.1.116'.
%   load              - Read settings from a JSON file, filling anything missing with the defaults.
%   loadCalibration   - The project's calibration of the lab (spot_calibration.json).
%   merge             - Copy the fields of CHANGES over BASE, recursing into structs.
%   platformSelection - The diagrams' platformSelection code for a set of platforms.
%   reset             - Back to the template's default settings, keeping the diagram (a clean slate).
%   save              - Write settings to a JSON file (readable, and diff-friendly in version control).
%   timing            - Phase durations, end times and total time from the settings.
