function block = showBlock(diagram, root, block)
    %SHOWBLOCK Open a diagram at a block and highlight it (e.g. the block an error is in).
    %   spot.sim.showBlock(diagram, root, blockPath) loads the diagram, opens the subsystem
    %   holding the block and highlights the block in red.
    %
    %   Inputs
    %     diagram  diagram file name
    %     root     project folder
    %     block    full block path, as in spot.sim.errorDetails(err).blocks; must be in diagram
    %
    %   Outputs
    %     block    the block path
    %
    %   See also spot.sim.errorDetails.

    model = spot.sim.loadDiagram(diagram, root);

    % The block must be in this diagram, and still exist.
    block = char(block);
    if ~startsWith(block, [model '/'])
        error('spot:sim:showBlock', 'The block %s is not in %s.', block, model);
    end
    try
        parent = get_param(block, 'Parent');
    catch
        error('spot:sim:showBlock', 'The block %s no longer exists in %s.', block, model);
    end

    % Show it.
    open_system(parent);
    hilite_system(block, 'error');
end
