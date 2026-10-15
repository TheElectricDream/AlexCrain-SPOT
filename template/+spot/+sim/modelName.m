function name = modelName(diagram)
    %MODELNAME Simulink model name of a diagram file ('Template.slx' -> 'Template').
    %   name = spot.sim.modelName(diagram)
    %
    %   Inputs
    %     diagram  diagram file name
    %
    %   Outputs
    %     name     model name; errors if there is none
    %
    %   See also spot.sim.loadDiagram.

    [~, name] = fileparts(diagram);
    if isempty(name)
        error('spot:sim:modelName', 'Select a Simulink diagram first.');
    end
end
