# spot.sim.SimulationFailed

**File:** `+spot/+sim/SimulationFailed.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/SimulationFailed.m -->

## What it is for

The error [[spot.sim.run|spot-sim-run]] throws when Simulink stops a simulation. It is an `MException` whose **message is already the root cause** (with the block it is in), whose cause is Simulink's own error, and which keeps what Simulink printed before failing. The console shows its message under the Run button; *Details* expands it with [[errorDetails|spot-sim-errorDetails]].

## How to use it

```matlab
try
    spot.sim.run(s, pwd);
catch err                      % err is a spot.sim.SimulationFailed
    err.message                % the root cause
    err.Output                 % Simulink's captured output (warnings) before the failure
    err.Original               % Simulink's error object
end
```

## Line by line

**Line 1.** A class derived from `MException`, so it can be thrown and caught like any error.

**Lines 9–12.** Two read-only properties: `Output` (the captured text) and `Original` (the Simulink error, an `MSLException` or an `MSLDiagnostic`).

**Lines 15–23.** The constructor: ask [[errorDetails|spot-sim-errorDetails]] for the summary of the original error, build the `MException` with identifier `spot:sim:failed` and that summary as the message, store the two properties, and (when the original is a real exception; a diagnostic object isn't) attach it as the cause so `getReport` shows the whole chain.

<!-- xref -->
- **Used by:** `+spot/+sim/errorDetails.m`, `+spot/+sim/run.m`
- **Uses:** [[spot.sim.errorDetails|spot-sim-errorDetails]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/SimulationFailed.m</b> (25 lines; click to show)</summary>

```matlab
   1  classdef SimulationFailed < MException
   2  %SIMULATIONFAILED The error spot.sim.run throws when Simulink stops a simulation with an error.
   3  %   Its message is the root cause (e.g. "Variable 'K_RED' does not exist. (in 4 - Control/...)"),
   4  %   its cause is Simulink's own error, and it keeps what Simulink printed before failing:
   5  %     err.Output     the captured text (warnings), without hyperlinks
   6  %     err.Original   Simulink's error (an MSLException, or the MSLDiagnostic of a run-time error)
   7  %   spot.sim.errorDetails(err) turns it into the parts the SPOT Console shows.
   8  
   9      properties (SetAccess = private)
  10          Output = ''
  11          Original = []
  12      end
  13  
  14      methods
  15          function obj = SimulationFailed(original, output)
  16              d = spot.sim.errorDetails(original);
  17              obj = obj@MException('spot:sim:failed', '%s', d.summary);
  18              obj.Output = output;
  19              obj.Original = original;
  20              if isa(original, 'MException')
  21                  obj = obj.addCause(original);
  22              end
  23          end
  24      end
  25  end
```

</details>
<!-- /listing -->
