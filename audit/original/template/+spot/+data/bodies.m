function b = bodies(ds)
    %BODIES Which platforms (and the arm) a dataset can show on the table.
    %   b = spot.data.bodies(dataset)
    %
    %   Inputs
    %     ds  dataset (spot.data.load)
    %
    %   Outputs
    %     b   crafts (cell of 'RED', 'BLACK', 'BLUE') and arm (true/false)
    %
    %   A platform needs <NAME>_Px_m, _Py_m and _Rz_rad. For a simulation, the platforms its
    %   settings select; for an experiment or a file, those whose position isn't all zero (every
    %   platform logs every signal, but only the ones in the run move).
    %
    %   See also spot.data.summary, spot.data.trajectory.

    sig = ds.signals;

    % A simulation's selected platforms; [] otherwise.
    selected = [];
    if strcmp(ds.kind, 'simulation') && isstruct(ds.settings) && isfield(ds.settings, 'platforms')
        selected = ds.settings.platforms;
    end

    %% Platforms
    b = struct('crafts', {{}}, 'arm', false);
    for p = {'RED', 'BLACK', 'BLUE'}

        % Needs all three position signals.
        posFields = strcat(p{1}, {'_Px_m', '_Py_m', '_Rz_rad'});
        if ~all(isfield(sig, posFields))
            continue
        end

        % Selected, or (without settings) moved.
        if isempty(selected)
            shown = any(sig.(posFields{1}).Data(:)) || any(sig.(posFields{2}).Data(:));
        else
            shown = selected.(p{1});
        end
        if shown
            b.crafts{end + 1} = p{1};
        end
    end

    %% Arm: on RED, with all three joint angles; selected or moved
    joints = {'ARM_Rz_Shoulder_rad', 'ARM_Rz_Elbow_rad', 'ARM_Rz_Wrist_rad'};
    if ismember('RED', b.crafts) && all(isfield(sig, joints))
        if isempty(selected)
            b.arm = any(cellfun(@(j) any(sig.(j).Data(:)), joints));
        else
            b.arm = selected.ARM;
        end
    end
end
