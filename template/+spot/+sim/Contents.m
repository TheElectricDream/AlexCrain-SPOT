% SIM  Simulations: diagrams, solver, running, results for the 3D view
%
%   debugLog     - The platforms' debug messages recorded during a simulation (by spot.hardware.debugMessages).
%   errorDetails - An error's root cause, the blocks involved and the full report, as plain text.
%   liveData     - The Live Stream blocks' signals recorded during a simulation (by spot.hardware.liveStream).
%   listDiagrams - Simulink diagrams (.slx) in the project folder.
%   loadDiagram  - Load a diagram from the project folder (without opening its window).
%   modelName    - Simulink model name of a diagram file ('Template.slx' -> 'Template').
%   readSolver   - Copy a diagram's own solver settings into the settings' solver section.
%   results      - Trajectory of a simulation for the SPOT Console's 3D view.
%   run          - Simulate the selected diagram with the given settings.
%   showBlock    - Open a diagram at a block and highlight it (e.g. the block an error is in).
%   tableSlope   - The table's pull on a floating platform: its slope and the air bearings' drag.
%   SimulationFailed - The error spot.sim.run throws when Simulink stops a simulation with an error.
