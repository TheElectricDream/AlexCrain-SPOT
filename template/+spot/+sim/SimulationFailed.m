classdef SimulationFailed < MException
%SIMULATIONFAILED The error spot.sim.run throws when Simulink stops a simulation with an error.
%   Its message is the root cause (e.g. "Variable 'K_RED' does not exist. (in 4 - Control/...)"),
%   its cause is Simulink's own error, and it keeps what Simulink printed before failing:
%     err.Output     the captured text (warnings), without hyperlinks
%     err.Original   Simulink's error (an MSLException, or the MSLDiagnostic of a run-time error)
%   spot.sim.errorDetails(err) turns it into the parts the SPOT Console shows.

    properties (SetAccess = private)
        Output = ''
        Original = []
    end

    methods
        function obj = SimulationFailed(original, output)
            d = spot.sim.errorDetails(original);
            obj = obj@MException('spot:sim:failed', '%s', d.summary);
            obj.Output = output;
            obj.Original = original;
            if isa(original, 'MException')
                obj = obj.addCause(original);
            end
        end
    end
end
