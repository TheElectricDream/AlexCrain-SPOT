# Writing Your Own Guidance and Control

The template diagram flies each platform round a circle with an LQR controller. An experiment of your own changes three subsystems, and leaves the rest alone. Read [Overview and Step Order](page:diagram-overview) first: the subsystems talk only through data stores, in a fixed order within each step.

## What goes where

| You want to change... | Edit | It writes |
|---|---|---|
| where each platform should be, and when | [1 - Experiment Logic](page:diagram-experiment-logic), the Phase 3 sub-phases | `X_Desired_States`, and which controller and path planner run |
| how it gets there (a trajectory or path planner) | [3 - Guidance](page:diagram-guidance) | `X_Path`, what the controller tracks |
| the control law | [4 - Control](page:diagram-control), a Custom Control branch | `X_Controls`, `[Fx Fy Tz]` in the table frame |
| your own state estimate | [2 - Navigation](page:diagram-navigation) (empty in the template) | a data store of your own |
| what the console shows | the phases' String Constants (`X_Logger`), and [Live Data Streams](page:diagram-live-streams) | messages and live plots |

Leave the rest as it is: reading PhaseSpace and the hardware, the simulated plant, the thruster mixer, the data logger. They are what makes the same diagram run in simulation and on the platforms.

## Step by step

1. **Define the experiment's steps.** In 1 - Experiment Logic, Phase #3 has four sub-phases. In each, set what every platform should do: its desired states, `X_Control_Law_Enabler` (0 off, 1 the default LQR, 2 or 3 your controllers; for the arm 1 position, 2 your torque controller, 3 velocity) and `X_Path_Planner_Selection` (0 none, 1 or 2 your planners). Set the durations, and switch off the sub-phases you don't need, on the Simulation tab's Timeline.
2. **Write the controller.** In 4 - Control, open a **Custom Control** branch (each platform has one; the Chaser two). Read the data stores you need (`X_Measured_States`, `X_Path`, ...) and output `[Fx Fy Tz]` in the table frame; the thruster code turns it into duty cycles. Your controller's gains go in your project's `Constants.m`, read in the block as `Constants.NAME` ([Constants](page:dev-constants)). A MATLAB Function block is the simplest way to write one; it gets its own page in this documentation's Reference.
3. **Simulate.** Run it on the Simulation tab and check the trajectory, the forces (`X_Fx_N` against `X_Fx_Sat_N`: what you asked for against what the thrusters can give) and the duty cycles on the Results tab.
4. **Run it in the lab** ([Quick Start: An Experiment in the Lab](page:quick-start-experiment)).

## Rules that keep it working

- **Keep code inside the numbered subsystems**, which have the right priorities. A block at the top level, or in a subsystem that runs earlier, reads last step's values.
- **Write each data store once per step**, in one subsystem. A new data store needs its *Data Store Memory* block at the top level ([Data Stores](page:data-stores)).
- **Keep phases 0, 1, 2, 4 and 5 as they are:** synchronising, floating, going to the start and coming home are what the lab procedure needs.
- **Code generation:** MATLAB Function blocks must use functions MATLAB Coder supports, with fixed-size outputs. Simulate first: most problems show there.
- **Time:** use `Univ_Time` (the data store) for time, not Simulink's clock: on the platforms it is the shared experiment clock.
- **Rates:** everything runs at `baseRate`; new measurements arrive at `dataRate` ([Workspace Variables](page:workspace-variables)).
- After changing the diagram, build again before an experiment.

To log a new signal, add it to the [Data Logger](page:diagram-data-logger); to watch it live, connect it to a Live Stream block ([Live Data Streams](page:diagram-live-streams)).
