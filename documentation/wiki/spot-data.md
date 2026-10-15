# spot.data: results

`+spot/+data/` serves the Results tab and the 3D playback: what is in `Saved Data/`, how a run is loaded (a simulation from the workspace, an experiment's per-platform `.mat` files merged into one, or any `.mat`), the signals thinned for plotting, the trajectory for the 3D view, and publication-quality figures.

A loaded **dataset** is a struct: `name`, `kind` (`'simulation'`, `'experiment'` or `'file'`), `source`, `signals` (a flat struct of timeseries, one per logged signal, e.g. `signals.RED_Px_m`) and `settings` (the settings saved with the run, or `[]`). Datasets live in the MATLAB worker; the page only receives summaries and the series it draws.

```matlab
runs = spot.data.listSaved(pwd);                           % what is in Saved Data
ds = spot.data.load(struct('source', 'simulation'), pwd, s);
ds = spot.data.load(struct('file', 'Saved Data/x/simulation.mat', 'settings', 'Saved Data/x/spot_settings.json'), pwd);
r = spot.data.series(ds{1}, struct('signal', 'RED_Px_m'));   % r.t, r.y
fig = spot.data.toFigure({struct('traces', {{struct('id', 1, 'signal', 'RED_Px_m', 'col', 1)}})}, @(id) ds{1});
spot.data.exportFigure(fig, 'figure.pdf');
```

| Function | One line |
|---|---|
| [[listSaved|spot-data-listSaved]] | the runs in `Saved Data/`, newest first, with what each can load |
| [[load|spot-data-load]] | read a dataset: the last simulation, an experiment (merged), or a file |
| [[flatten|spot-data-flatten]] | any logged structure → one flat struct of timeseries |
| [[samples|spot-data-samples]] | a timeseries' time and data as plain columns |
| [[summary|spot-data-summary]] | what the page shows about a dataset (names, units, span), no data |
| [[bodies|spot-data-bodies]] | which platforms (and the arm) a dataset can show on the table |
| [[series|spot-data-series]] | one signal, smoothed, ranged and thinned for a plot; or against another signal |
| [[trajectory|spot-data-trajectory]] | x, y, θ per platform and the arm's joints at ≤30 Hz for the 3D view |
| [[saveSimulation|spot-data-saveSimulation]] | keep `dataClass` in `Saved Data/` with its settings |
| [[toFigure|spot-data-toFigure]] | the Results page's plots as a MATLAB figure in the lab's style |
| [[exportFigure|spot-data-exportFigure]] | print a figure at 700 dpi as PDF/EPS/SVG/PNG |

See also [[Logged Signals|Logged-Signals]] for what the diagram logs.
