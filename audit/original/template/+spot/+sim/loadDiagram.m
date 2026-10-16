function model = loadDiagram(diagram, root)
    %LOADDIAGRAM Load a diagram from the project folder (without opening its window).
    %   model = spot.sim.loadDiagram('Template.slx', root)
    %
    %   Inputs
    %     diagram  diagram file name
    %     root     project folder
    %
    %   Outputs
    %     model    model name
    %
    %   See also spot.sim.modelName, spot.sim.listDiagrams.

    % Load it unless it already is.
    model = spot.sim.modelName(diagram);
    if ~bdIsLoaded(model)
        file = fullfile(root, diagram);
        if ~isfile(file)
            error('spot:sim:loadDiagram', 'There is no %s in %s.', diagram, root);
        end
        load_system(file);
    end
end
