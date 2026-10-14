function names = listDiagrams(root)
%LISTDIAGRAMS Simulink diagrams (.slx) in the project folder.
%   names = spot.sim.listDiagrams(projectFolder) returns a cell array of file names.
files = dir(fullfile(root, '*.slx'));
names = sort({files.name});
end
