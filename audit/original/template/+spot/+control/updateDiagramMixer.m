function updateDiagramMixer(diagram)
    %UPDATEDIAGRAMMIXER Put thrusterDutyCycles's mixer code into a diagram's mixer blocks.
    %   spot.control.updateDiagramMixer() updates the project's diagram (Constants.DIAGRAM, else
    %   the first .slx in the project folder).
    %   spot.control.updateDiagramMixer('My_Diagram.slx') updates another one.
    %
    %   For RED, BLACK and BLUE, ".../Change <PLATFORM> Behavior/Calculate Duty Cycle/MATLAB
    %   Function5" gets thrusterDutyCycles.m's verbatim MATLAB Function5 code (with that
    %   platform's names), and reads the thrust decay from the parameter thrust_decay_<PLATFORM>
    %   (put in the workspace by spot.params.apply). A backup <name>.slx.bak-<yyyymmdd_HHMMSS>
    %   is kept first.
    %
    %   Run it once per diagram, and again whenever the mixer in thrusterDutyCycles.m changes.
    %   Then check with spot.control.verifyConsoleController and rebuild the experiment: the
    %   platforms run the diagram's mixer.
    %
    %   See also spot.control.thrusterDutyCycles, spot.control.verifyConsoleController.

    %% Diagram
    % Project folder, above +spot.
    root = fileparts(fileparts(fileparts(mfilename('fullpath'))));

    % Default: the project's diagram, else the first in the project.
    if nargin < 1 || isempty(diagram)
        diagram = char(Constants.DIAGRAM);
        if isempty(diagram) || ~isfile(fullfile(root, diagram))
            names = spot.sim.listDiagrams(root);
            if isempty(names)
                error('spot:control:updateDiagramMixer', ...
                    'There is no diagram (.slx) in %s.', root);
            end
            diagram = names{1};
        end
    end

    % Relative to the project, or a full path.
    diagramFile = fullfile(root, diagram);
    if ~isfile(diagramFile)
        diagramFile = diagram;
    end
    [~, model] = fileparts(diagramFile);

    %% Mixer code
    % The verbatim MATLAB Function5 section of thrusterDutyCycles.m (RED's names).
    mixerFile = fullfile(fileparts(mfilename('fullpath')), 'thrusterDutyCycles.m');
    sections = regexp(fileread(mixerFile), ...
        '% ---- verbatim: ([^\n]*)\n(.*?)\n% ---- end verbatim', 'tokens');
    sections = vertcat(sections{:});
    hit = endsWith(sections(:, 1), 'Calculate Duty Cycle/MATLAB Function5');
    if ~any(hit)
        error('spot:control:updateDiagramMixer', ...
            'thrusterDutyCycles.m has no verbatim MATLAB Function5 section.');
    end
    code = sections{hit, 2};

    %% Backup and load
    if bdIsLoaded(model) && strcmp(get_param(model, 'Dirty'), 'on')
        error('spot:control:updateDiagramMixer', ...
            '%s has unsaved changes: save or close it first.', model);
    end
    backup = sprintf('%s.bak-%s', diagramFile, datestr(now, 'yyyymmdd_HHMMSS')); %#ok<TNOW1,DATST>
    copyfile(diagramFile, backup);
    fprintf('Kept a copy of the diagram: %s\n', backup);
    load_system(diagramFile);

    %% Update each platform's block
    for p = {'RED', 'BLACK', 'BLUE'}
        platform = p{1};

        % The block's chart.
        block = [model '/Thruster & Float' newline 'Control Code/Change ' platform ...
                 ' Behavior/Calculate Duty Cycle/MATLAB Function5'];
        chart = find(sfroot, '-isa', 'Stateflow.EMChart', 'Path', block);
        if isempty(chart)
            error('spot:control:updateDiagramMixer', 'Block not found: %s', ...
                strrep(block, newline, ' '));
        end

        % The code with this platform's names (as verifyConsoleController compares them).
        chart.Script = strrep(strrep(code, '_RED', ['_' platform]), ...
            '_red_', ['_' lower(platform) '_']);

        % The thrust decay argument is a parameter read from the workspace, not an input port.
        name = ['thrust_decay_' platform];
        data = chart.find('-isa', 'Stateflow.Data', 'Name', name);
        if isempty(data)
            data = Stateflow.Data(chart);
            data.Name = name;
        end
        data.Scope = 'Parameter';
        fprintf('%s: mixer updated (reads %s)\n', platform, name);
    end

    %% Save
    save_system(model);
    fprintf(['Saved %s. Check it with spot.control.verifyConsoleController, and build the ' ...
             'experiment again: the platforms run this mixer.\n'], diagramFile);
end
