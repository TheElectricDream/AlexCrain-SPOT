# PhaseSpace Camera Code

**Priority 20.** The diagram's clock, and on a platform its measurements. In a simulation it only sets the clock; the measurements then come from [Simulate Plant Dynamics](page:diagram-plant).

![PhaseSpace Camera Code](images/sub_PhaseSpace_Camera_Code.png)

**Writes:** `Univ_Time`; on a platform also `RED_Measured_States`, `BLACK_Measured_States`, `BLUE_Measured_States`. **Reads:** `isSim`, `WhoAmI`.

## The top level

- **Data Store Read2** (`isSim`) → **If**: `u1 == 0` runs **Use Hardware to Obtain States**; `u1 == 1` runs **Initialize Universal Time (Simulation)**.
- **Initialize Universal Time (Simulation)**: a **Clock** block (Simulink's own time) → **Data Store Write** `Univ_Time`. That is all a simulation needs.

## Use Hardware to Obtain States

![Use Hardware to Obtain States](images/sub_PhaseSpace_Camera_Code_Use_Hardware_to_Obtain_States.png)

1. **Receive and Process Phasespace Data** gives two outputs: *PhaseSpace Data* (19 values) and *PhaseSpace Clock* (1 value).
2. **Demux** splits the 19 values into `[4, 3, 3, 9]`: the experiment clock plus RED's `x y θ`, BLACK's `x y θ`, BLUE's `x y θ`, and the nine velocities (**Demux1** then splits those into 3 × 3).
3. **Store RED Data**, **Store BLACK Data**, **Store BLUE Data** each take their platform's positions, velocities and the PhaseSpace clock and write the platform's measured states.

### Receive and Process Phasespace Data

![Receive and Process Phasespace Data](images/sub_PhaseSpace_Camera_Code_Use_Hardware_to_Obtain_States_Receive_and_Process_Phasespace_Data.png)

- **Receive PhaseSpace Data** (a MATLAB Function block, one line): `[packet, newPackets, age] = spot.hardware.phasespaceLatest(31534, labConsoleIP)` (`spot.hardware.phasespaceLatest`): the newest 115-float packet (`Constants.PACKET_LENGTH`) from [StreamData](file:StreamData.cc) on UDP port 31534 (`PORT_PHASESPACE_CLIENT`), how many arrived since the last step (terminated) and how old the newest is (−1 = none yet).
- **To double** → **Reshape** → **Demux2** `[1, 18, 96]`: slot 1 is the PhaseSpace server's clock (→ *PhaseSpace Clock*), the next 18 are the three rigid bodies' positions and velocities in **millimetres**, the last 96 are the 32 LEDs' coordinates (**LED Terminator**: not used by the diagram; the console's 3D view shows them).
- **Experiment Clock** takes the packet and its age and gives the *experiment* time:

![Experiment Clock](images/sub_PhaseSpace_Camera_Code_Use_Hardware_to_Obtain_States_Receive_and_Process_Phasespace_Data_Experiment_Clock.png)
  - **Experiment Time**: `[t, state] = spot.hardware.experimentTime(packet, age)` ([spot.hardware.experimentTime](ref:spot.hardware.experimentTime)). The lab console puts an agreed start time into the packet (slot 113) when every platform's program is running; `t` is 0 until then, and afterwards the shared clock. `state` (0 waiting, 1 counting down, 2 running, 3 running on this computer's own clock because the stream was lost) is terminated here; the time itself is what matters.
  - **Send Sync. Time**: the If-blocks pick this platform's branch (`WhoAmI == n & isSim == 0`) and its **UDP Send (GUI)** sends the time to the lab console (`spot.hardware.udpSend(labConsoleIP, 48720/48721/48722, t)`; `PORT_EXP_CLOCK_RED/BLACK/BLUE`), which shows each platform's clock on the Experiment tab and uses the three to agree the start ([experiment.py](file:experiment.py)).
- **Mux** puts the experiment time in front of the 18 position/velocity values → *PhaseSpace Data*.

### Store BLACK Data (RED and BLUE are the same)

Inputs: *BLACK Data* `[x y θ]` (mm, mm, rad), *BLACK Velocities* `[ẋ ẏ θ̇]` (mm/s, mm/s, rad/s), *PS Clock*.

- **Convert … from [mm] to [m]** (Gain × 1/1000) on x, y, ẋ, ẏ; the angle and its rate are already in radians.
- **Mux3** = `[x y θ]` in metres → **Demux**: x and y go straight to **Mux1**; θ goes through **Wrap** first.
- **Wrap**: `atan2(sin θ, cos θ)`: the angle brought into (−π, π]. The camera's angle can come unwrapped; the controller's error calculation expects a wrapped one.
- **Calculate True Time Step**: the time between *new* camera samples. **Delay2**/**Subtract1** take the PS clock's change since the last step; **Delay1**/**Subtract2**/**Relational Operator** check whether x changed (`≠ 0`); **Enabled Subsystem3** passes the clock difference only on steps with a new sample (holding the last one otherwise); the MATLAB Function `CheckforInvalidTime` replaces a non-positive dt with `dataRate`, so the first step and a stalled stream never divide by zero. The result, `dt`, is the real interval between measurements, whatever the diagram's own step is.
- **Subsystem2** (positions → velocity estimate) and **Subsystem1** (that estimate → acceleration): each one is the same **EMA derivative**: **Delay1**/**Subtract1**/**Relational Operator** run the enabled subsystem only when the input changed, and inside, `realtime_derivative_ema(y, dt, τ = 0.1, ema_prev)`:

$$\alpha = 1 - e^{-dt/\tau}, \qquad \text{ema}_k = \alpha\, y_k + (1-\alpha)\,\text{ema}_{k-1}, \qquad \dot y_k = \frac{\text{ema}_k - \text{ema}_{k-1}}{dt}$$

  A first-order low-pass with a 0.1 s time constant (`LOWPASS_TIME_CONSTANT`), differenced: smooth enough to use, with about 0.1 s of lag. The velocities the diagram *stores* are the ones StreamData computed (**Mux4**, converted to m/s); Subsystem2's own velocity estimate is only differentiated again to get the accelerations.
- **Mux2** = `[Mux1 (x y θ); Mux4 (ẋ ẏ θ̇); Subsystem1 (ẍ ÿ θ̈)]` → **Data Store Write** `BLACK_Measured_States`.

**Store RED Data** differs in one block: its first input has four values (the experiment time in front of `x y θ`), so a **Demux1** `n=4` peels the time off and a **Data Store Write** puts it in `Univ_Time`. On a platform, the experiment clock *is* the universal time: every platform's `Univ_Time` is the same clock, so [1 - Experiment Logic](page:diagram-experiment-logic) changes phase on all of them at the same instant.
