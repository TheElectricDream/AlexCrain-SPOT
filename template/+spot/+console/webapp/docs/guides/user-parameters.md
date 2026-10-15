# Your Own Parameters: `SPOT_User_Parameters.m`

The diagram reads its parameters from MATLAB's base workspace. The console puts SPOT's there from the settings ([Workspace Variables](page:workspace-variables)), every time a setting changes and before each simulation and build, then runs **`SPOT_User_Parameters.m`**, a script in the project folder. Anything it defines is available to the diagram, and it can override any of SPOT's values.

Use it for:

- **your own variables**: gains of a controller you wrote, a trajectory's parameters, a lookup table;
- **changing SPOT's** where the console has no field for it, for example the simulated arm's friction or servo voltage (`armModel`, see `spot.arm.defaults`).

The template's copy shows both: commented examples of changing `armModel`, and two example variables, the gains `Kp` and `Kv` of a transpose-Jacobian arm controller (the template's diagram doesn't use them).

```matlab
%% My guidance: a circle around the table's centre
circleRadius = 0.6;                              % [m]
circleRate = 2 * pi / 120;                       % [rad/s] one turn in 2 minutes

%% The simulated arm: stiffer friction than the default
armModel.friction.static = [0.30 0.30 0.30];     % breakaway torque [N m] (shoulder, elbow, wrist)
```

## Things to know

- It runs **after** the settings, so a value set here wins over the console's. The Calibration pane shows the values the next simulation uses, read back after this script, and marks the ones the script changed.
- Don't define the settings the console owns here: `PWMFreq` (the Simulation tab's Thruster PWM) and `labConsoleIP` (the Experiment tab's Lab Console Address). The script says so at its end.
- An error in the script stops the settings from being applied, and the console shows it; fix the script and change a setting (or run) again.
- The same values are compiled into the platforms' programs when you build, so build again after changing it.
- The console's Workspace pane lists every variable, yours included.
