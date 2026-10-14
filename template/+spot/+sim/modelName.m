function name = modelName(diagram)
%MODELNAME Simulink model name of a diagram file ('Template.slx' -> 'Template').
[~, name] = fileparts(diagram);
if isempty(name)
    error('spot:sim:modelName', 'Select a Simulink diagram first.');
end
end
