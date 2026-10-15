# spot.workspace: the Workspace panel

`+spot/+workspace/` is the MATLAB side of the Simulation tab's **Workspace** pane and its **MATLAB** command window: list the base workspace, preview a value in one line, inspect a variable (values, fields or a resampled timeseries), and run a command with its output captured.

| Function | One line |
|---|---|
| [[list|spot-workspace-list]] | `whos` of the base workspace with a one-line preview per variable |
| [[preview|spot-workspace-preview]] | a value as the Workspace browser's Value column shows it |
| [[inspect|spot-workspace-inspect]] | a variable (or field) in detail: a matrix, a struct's fields, or a series to draw |
| [[evaluate|spot-workspace-evaluate]] | run a command in the base workspace, return what it printed or the error |

![The Workspace pane](images/gui-simulation-workspace-pane.png)
