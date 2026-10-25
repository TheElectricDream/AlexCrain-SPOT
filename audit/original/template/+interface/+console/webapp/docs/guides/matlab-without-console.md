# Running Without the Console

Everything the console does is an ordinary MATLAB function in the `spot` packages (and, for what its pages show, `interface`), so you can script it: for batch runs, parameter sweeps, or when you prefer the Command Window. Run these from the project folder.

## One simulation

```matlab
root = pwd;                                     % the project folder
s = spot.params.load('spot_settings.json');     % the console's settings (or spot.params.defaults())
s.diagram = 'MyProject.slx';
s.platforms.RED = true;
s.platforms.BLACK = false;
s.timing.subPhases = [10 10 30 60];             % phase 3's sub-phases [s]
s.timing.on.phase4 = false;                     % skip phase 4
s.initial.RED.initial = [2.0 1.2 90];           % x [m], y [m], theta [deg]
s.control.RED.lqr.Q = [2 2 0.1 10 10 1];        % x y theta dx/dt dy/dt dtheta/dt
s.solver = spot.sim.readSolver(s.diagram, root, s.solver);   % the diagram's own solver
out = spot.sim.run(s, root);                    % simulates; dataClass is in the workspace
fprintf('%g s simulated in %.1f s\n', out.tsim, out.seconds);
```

`spot.sim.run` applies the settings, runs `SPOT_User_Parameters.m`, simulates without changing the diagram file, and leaves `dataClass`, `dataPacket` and `simOut` in the base workspace. The settings are an ordinary struct: `spot.params.defaults` lists every field with its unit ([The Settings File](page:settings-file)). When Simulink stops with an error, `spot.sim.run` throws a `spot.sim.SimulationFailed` whose message is the root cause; `interface.worker.errorDetails(err)` gives the whole chain.

## Looking at the results

```matlab
plot(dataClass.RED_Px_m);                        % a timeseries
hold on
plot(dataClass.RED_Px_Desired_m)
spot.data.saveSimulation(s, root);              % keep it in Saved Data (with s)

% A figure in the lab's publication style, saved as PDF
ds = spot.data.load(struct('source', 'simulation'), root, s);   % a copy of this run
lookup = @(id) ds{1};
panel = struct('traces', struct('id', 'sim', 'signal', {'RED_Px_m', 'RED_Px_Desired_m'}, ...
                                'col', 1));
fig = interface.results.toFigure({panel}, lookup);
spot.data.exportFigure(fig, fullfile(root, 'red_x.pdf'));
```

A panel with `x = struct('signal', 'RED_Px_m', 'col', 1)` and `RED_Py_m` as its trace draws the Chaser's path on the table, with its outline at the start and the end. Experiment runs load the same way: `interface.results.listSaved(root)` lists them, and `spot.data.load` reads one (its help explains the `files` and `file` forms).

## From a terminal (batch)

MATLAB's `-batch` option runs a command without the desktop and exits when it ends. The same line works on Windows and Linux:

```bash
matlab -batch "cd('C:/Users/you/SPOT/MyProject'); s = spot.params.load('spot_settings.json'); spot.sim.run(s, pwd); spot.data.saveSimulation(s, pwd);"
```

For anything longer, put the commands in a script in the project folder (say `my_runs.m`) and run `matlab -batch "my_runs"`.

## A parameter sweep

```matlab
root = pwd;
s = spot.params.load('spot_settings.json');
s.solver = spot.sim.readSolver(s.diagram, root, s.solver);
for q = [0.5 1 2 4]
    s.control.RED.lqr.Q(1:2) = q;               % position weights
    out = spot.sim.run(s, root);
    folder = spot.data.saveSimulation(s, root); % each run in its own Saved Data folder
    fprintf('Q = %g: saved in %s\n', q, folder);
end
```

The saved runs then appear in the Results tab's **Saved Data…** list, ready to plot or replay side by side.

## Experiments from MATLAB

Building works from MATLAB too, on the lab network:

```matlab
p = spot.experiment.plan(s, root);              % what the build would do (no compiling)
spot.experiment.connect(root, 'tx2-password');  % once; afterwards spot.experiment.connect(root)
out = spot.experiment.build(s, root);           % generates the code and compiles it on the TX2
```

Deploying, starting, stopping and collecting the data are done by the lab console: use the local console's Experiment tab for those steps.
