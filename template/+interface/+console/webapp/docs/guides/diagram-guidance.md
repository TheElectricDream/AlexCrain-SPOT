# 3 - Guidance

**Priority 40.** The **path planners**: from the desired state that [1 - Experiment Logic](page:diagram-experiment-logic) set, produce the state the controller should track *right now*. For each of RED, BLACK, BLUE and ARM there is a selector, a custom planner you can fill in and a pass-through.

![3 - Guidance](images/sub_3_Guidance.png)

**Reads:** `X_Path_Planner_Selection`, `X_Desired_States`. **Writes:** `X_Path` (RED, BLACK, BLUE, ARM).

Per platform (the four columns are identical):
- **Data Store Read** `X_Path_Planner_Selection` → **If**: `u1 == 1` (RED and BLACK) or `u1 == 2` (BLUE and ARM) runs **Custom PPL (X)**, anything else runs **Disable PPL (X)**. (The two different numbers are a leftover; the phases of the template always write 0, so the *Disable* branch runs. Pick a number and write it from the phase that needs the planner.)
- **Custom PPL (X)** and **Disable PPL (X)** are, in the template, the same thing: a **Desired Position** subsystem that reads `X_Desired_States` and outputs it unchanged as the *Path*.
- **Merge** takes whichever branch ran → **Data Store Write** `X_Path`.

So out of the box `X_Path = X_Desired_States`: the controller is given the target directly and flies there as fast as the LQR lets it. A path planner belongs in **Custom PPL**: for example a straight line from the current position (`X_Measured_States`) to `X_Desired_States` at a set speed, or a spline through waypoints, output as `[x y θ]` for a platform, or `[θ1 θ2 θ3]` (angles for the position controller, rad/s for the velocity controller) for the arm.

The planner's output is what [4 - Control](page:diagram-control) tracks; it is logged as `X_Px_Path_m`, `X_Py_Path_m`, `X_Rz_Path_rad` and `ARM_*_Path`, next to the desired states (`X_Px_Desired_m` …), so you can see both the goal and the path to it in the [Results tab](page:tab-results).
