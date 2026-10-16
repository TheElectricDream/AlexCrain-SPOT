## initSimulation.schedulePush

Sends the settings to MATLAB `delay` ms (300 by default) after the last change, so typing a number sends it once, not on every keystroke. A new change restarts the wait.

## initSimulation.$('#simRealTime').onchange

The **Real Time** switch: saves `realTime` in the settings (simulations then run at the clock's pace).

## initSimulation.refreshWorkspaceSoon

Reads MATLAB's workspace again 0.8 s after the last call, for the Workspace pane (after a command or a run changes it).
