function block = showBlock(diagram, root, block)
%SHOWBLOCK Open a diagram at a block and highlight it (e.g. the block an error is in).
%   spot.sim.showBlock(diagramFile, projectFolder, blockPath) loads the diagram from the project
%   folder, opens the subsystem holding the block and highlights the block in red. blockPath is
%   a full path, as in spot.sim.errorDetails(err).blocks; it must be in that diagram.
model = spot.sim.loadDiagram(diagram, root);
block = char(block);
if ~startsWith(block, [model '/'])
    error('spot:sim:showBlock', 'The block %s is not in %s.', block, model);
end
try
    parent = get_param(block, 'Parent');
catch
    error('spot:sim:showBlock', 'The block %s no longer exists in %s.', block, model);
end
open_system(parent);
hilite_system(block, 'error');
end
