function target = New_SPOT_Project(name, location)
    %NEW_SPOT_PROJECT Create a new SPOT project from the template.
    %   New_SPOT_Project asks for the project's name and creates projects/<name>.
    %   New_SPOT_Project('MyProject') creates projects/MyProject.
    %   New_SPOT_Project('MyProject', folder) creates folder/MyProject instead.
    %   target = New_SPOT_Project(...) also returns the new project's folder ('' if cancelled).
    %
    %   A project is a copy of the template folder (next to this file) with its Simulink diagram
    %   renamed to <name>.slx, so the name is also the diagram's name: letters, digits and _,
    %   starting with a letter. The template's caches, builds, saved data and documentation
    %   stay behind; its Constants.m comes along (with the lab calibration record that goes
    %   with it, Saved Data/Calibration), so the project starts from the template's values and
    %   keeps every change of its own there. Afterwards: cd into the project and
    %   run SPOT.
    %
    %   Inputs
    %     name      the project's name (optional: asked for in a dialog, or in the Command
    %               Window without a desktop)
    %     location  the folder to create it in (optional: projects/, next to this file)
    %
    %   Outputs
    %     target    the new project's folder; '' if the name was not given
    %
    %   Behavior
    %     1. Checks the name (a valid diagram name, not one of SPOT's or MATLAB's own).
    %     2. Copies the template's top level, except what is left out and Simulink's build
    %        files, then removes Python caches and the lab console's local builds.
    %     3. Copies the user guide if the template has one, the diagram as <name>.slx, and
    %        makes an empty Saved Data folder.
    %     On an error the half-made project is removed.
    %
    %   See also SPOT.

    %% Constants
    % Not copied from the template: its own state, builds and the developers' documentation.
    LEAVE_OUT = {'.', '..', '.spot_cache', 'slprj', 'Saved Data', 'documentation', ...
                 'STATUS.md'};
    BUILD_SUFFIXES = {'.slxc', '.autosave', '_ert_rtw'};   % Simulink's build and cache files

    %% Name and location
    root = fileparts(mfilename('fullpath'));
    template = fullfile(root, 'template');

    % The name, asked for if not given (cancelled: nothing to do).
    if nargin < 1 || isempty(name)
        name = askName();
        if isempty(name)
            target = '';
            return
        end
    end
    name = strtrim(char(name));
    problem = nameProblem(name);
    if ~isempty(problem)
        error('SPOT:newProject', '%s', problem);
    end
    if nargin < 2 || isempty(location)
        location = fullfile(root, 'projects');
    end

    % The template's one diagram, and a folder not taken yet.
    diagram = dir(fullfile(template, '*.slx'));
    if numel(diagram) ~= 1
        error('SPOT:newProject', ...
            'The template (%s) must hold exactly one diagram (.slx); it has %d.', ...
            template, numel(diagram));
    end
    target = fullfile(char(location), name);
    if exist(target, 'file')
        error('SPOT:newProject', '%s already exists. Choose another name or location.', target);
    end

    %% Copy
    mkdir(target);
    try
        % Everything at the top of the template, except what is left out and Simulink's build
        % files.
        items = dir(template);
        items = items(~ismember({items.name}, [LEAVE_OUT, {diagram.name}]) ...
                      & ~endsWith({items.name}, BUILD_SUFFIXES));
        for k = 1:numel(items)
            copyfile(fullfile(template, items(k).name), fullfile(target, items(k).name));
        end

        % Python caches and the lab console's local builds inside +spot.
        inside = dir(fullfile(target, '**'));
        caches = inside([inside.isdir] & strcmp({inside.name}, '__pycache__'));
        for k = 1:numel(caches)
            rmdir(fullfile(caches(k).folder, caches(k).name), 's');
        end
        if isfolder(fullfile(target, '+spot', '+console', 'output'))
            rmdir(fullfile(target, '+spot', '+console', 'output'), 's');
        end

        % The user guide, if the template has it.
        guide = fullfile(template, 'documentation', 'SPOT_Tutorial.pdf');
        if isfile(guide)
            copyfile(guide, target);
        end

        % The diagram, named after the project, and an empty folder for the results, with the
        % template's lab calibration record (it goes with the LAB_* numbers in Constants.m).
        copyfile(fullfile(template, diagram.name), fullfile(target, [name '.slx']));
        mkdir(fullfile(target, 'Saved Data'));
        record = fullfile(template, 'Saved Data', 'Calibration');
        if isfolder(record)
            copyfile(record, fullfile(target, 'Saved Data', 'Calibration'));
        end
    catch err
        % Don't leave a half-made project behind.
        rmdir(target, 's');
        rethrow(err);
    end

    %% Report
    fprintf('Created %s\n', target);
    if usejava('desktop')
        fprintf('To start it: <a href="matlab:cd(''%s''); SPOT">cd there and run SPOT</a>\n', ...
            strrep(target, '''', ''''''));
    else
        fprintf('To start it: cd(''%s''); SPOT\n', strrep(target, '''', ''''''));
    end
    if nargout == 0
        clear target
    end
end

function name = askName()
    %ASKNAME The project's name from a dialog (or the Command Window without a desktop).
    %   name = askName() asks until the name can be used; '' if cancelled or left empty.

    %% Constants
    PROMPT = ['Project name (letters, digits and _, starting with a letter; it also names ' ...
              'the diagram):'];
    DIALOG_SIZE = [1 70];   % [lines characters] of the dialog's text box

    %% Ask
    name = '';
    while true

        % The answer, from the dialog or the Command Window.
        if usejava('desktop')
            answer = inputdlg(PROMPT, 'New SPOT Project', DIALOG_SIZE, {name});
            if isempty(answer)
                name = '';
                return
            end
            name = strtrim(answer{1});
        else
            name = strtrim(input([PROMPT ' '], 's'));
        end
        if isempty(name)
            return
        end

        % A usable name ends the questions; otherwise say why, and ask again.
        problem = nameProblem(name);
        if isempty(problem)
            return
        end
        if usejava('desktop')
            uiwait(errordlg(problem, 'New SPOT Project', 'modal'));
        else
            fprintf('%s\n', problem);
        end
    end
end

function problem = nameProblem(name)
    %NAMEPROBLEM Why a name can't be used for a project ('' if it can).
    %   problem = nameProblem(name) checks the name as a folder and Simulink diagram name.

    %% Constants
    MAX_NAME_LENGTH = 63;   % [characters] longest MATLAB name

    %% Checks
    problem = '';
    if ~isvarname(name) || numel(name) > MAX_NAME_LENGTH
        problem = sprintf( ...
            ['''%s'' can''t be a diagram name: use letters, digits and _ only, starting ' ...
             'with a letter, at most 63 characters, and not a MATLAB keyword.'], ...
            name);
    elseif any(strcmpi(name, {'spot', 'Constants'}))
        problem = sprintf('''%s'' is one of SPOT''s own names; choose another.', name);
    elseif ~isempty(which(name)) && ~strcmp(which(name), 'variable')
        problem = sprintf('''%s'' is already a MATLAB function (%s); choose another name.', ...
            name, which(name));
    end
end
