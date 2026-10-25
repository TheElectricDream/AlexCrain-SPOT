# The Results Tab

Plots of anything that was logged: the last simulation, saved runs, collected experiments and any `.mat` file, side by side. The code is [results.js and data.js](file:results.js) in the page and `spot.data` in MATLAB (the data stays in MATLAB; the page asks for what it draws).

## Datasets

The loaded datasets, one row each: its kind (**SIM**, **EXP**, **MAT**), name, platforms and duration; **3D ▸** replays it on the Simulation tab; ✕ unloads it. Load more with:

- **Last Simulation**: the run just made (it is also loaded automatically when a run finishes);
- **Saved Data…**: the dialog listing `Saved Data/` by run (date, kind, platforms), with a button per item; experiment runs that haven't been decoded yet show **Convert ExpLog Files**; **Other .mat File…** opens MATLAB's file dialog;
- **Open .mat…**: any `.mat` with a `dataClass`, the old GUI's `RED.Px_m` structs, Simulink `logsout` or a struct of timeseries (`spot.data.flatten` sorts out the shape).

Select a dataset (click its row) to list its **Signals**.

## Signals

The selected dataset's signals ([Logged Signals](page:logged-signals)) grouped by platform (RED, BLACK, BLUE, ARM, Other), filtered by the search box; a multi-column signal (the duty cycles) has a button per column; signals that are all zero are greyed. **Click a signal to add it to the active plot**, click again to remove it.

## Plots

Up to six plots (**+ Plot**), each with one y axis and up to eight lines from any datasets. Click a plot to make it the active one. In its header:

- the **x axis**: time, or any signal of the datasets in the plot, for an x–y plot (e.g. `RED_Px_m` against `RED_Py_m` draws the Chaser's path); **Equal** makes the scales equal;
- a **Mixed Units** warning when the lines don't share a unit;
- **Zoom Out**, the **Axes** limits, and ✕;
- the legend, with a remove button per line.

Drag on a plot to **zoom** (a time range on a time plot, a box on an x–y plot); double-click to zoom out. **Link Time** zooms and hovers all time plots together. Hover for a crosshair with each line's value. **Smoothing** applies a Gaussian filter over the chosen window (the old GUI's filter). Long signals are drawn thinned to about two points per pixel and refetched in detail when you zoom in.

## Export

**Save PDF…** and **Open in MATLAB** draw the same plots in MATLAB in the lab's publication style (`interface.results.toFigure`, `spot.data.exportFigure`): the width, height and font size are the fields next to the buttons; the zoom becomes the axis limits. *Open in MATLAB* leaves the figure open for you to edit.

## Scripting the same thing

`interface.results.listSaved(root)` lists the runs, `spot.data.load(...)` reads one, `interface.results.series` picks signals; [Running Without the Console](page:matlab-without-console) has the recipes.
