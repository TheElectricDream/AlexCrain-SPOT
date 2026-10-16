classdef SimulationFailed < MException
    %SIMULATIONFAILED The error spot.sim.run throws when Simulink stops a simulation with an error.
    %   Its message is the root cause (e.g. "Variable 'K_RED' does not exist. (in 4 -
    %   Control/...)"), its cause is Simulink's own error, and it keeps what Simulink printed
    %   before failing. spot.sim.errorDetails(err) turns it into the parts the console shows.
    %
    %   Properties
    %     Output    the captured text (warnings), without hyperlinks
    %     Original  Simulink's error (an MSLException, or the MSLDiagnostic of a run-time error)
    %
    %   See also spot.sim.run, spot.sim.errorDetails.

    properties (SetAccess = private)
        Output = ''         % captured text, without hyperlinks
        Original = []       % Simulink's error
    end

    methods
        function obj = SimulationFailed(original, output)
            % Message: the root cause; cause: Simulink's error, when it is an MException.
            details = spot.sim.errorDetails(original);
            obj = obj@MException('spot:sim:failed', '%s', details.summary);
            obj.Output = output;
            obj.Original = original;
            if isa(original, 'MException')
                obj = obj.addCause(original);
            end
        end
    end
end
