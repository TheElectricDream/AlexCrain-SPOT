function names = listDiagrams(root)
    %LISTDIAGRAMS Simulink diagrams (.slx) in the project folder.
    %   names = spot.sim.listDiagrams(root)
    %
    %   Inputs
    %     root   project folder
    %
    %   Outputs
    %     names  cell array of file names, sorted
    %
    %   See also spot.sim.loadDiagram.

    files = dir(fullfile(root, '*.slx'));
    names = sort({files.name});
end
