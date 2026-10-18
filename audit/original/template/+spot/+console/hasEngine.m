function ok = hasEngine(python)
    %HASENGINE Whether a Python can import the MATLAB Engine for Python.
    %   ok = spot.console.hasEngine(python)
    %
    %   Inputs
    %     python  the Python command (spot.console.findPython)
    %
    %   Outputs
    %     ok      true when "import matlab.engine" succeeds
    %
    %   See also spot.console.ensureEngine, spot.console.findPython.

    [status, ~] = system([python ' -c "import matlab.engine"']);
    ok = status == 0;
end
