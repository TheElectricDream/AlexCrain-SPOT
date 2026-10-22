function target = New_SPOT_Project(name, location, diagram)
    %NEW_SPOT_PROJECT Create a new SPOT project from the template.
    %   New_SPOT_Project asks for the project's name and creates projects/<name>.
    %   New_SPOT_Project('MyProject') creates projects/MyProject.
    %   New_SPOT_Project('MyProject', folder) creates folder/MyProject instead.
    %   New_SPOT_Project('MyProject', [], 'Template_LinearFlow.slx') starts it from that diagram.
    %   target = New_SPOT_Project(...) also returns the new project's folder ('' if cancelled).
    %
    %   A project is a copy of the template folder (next to this file) with one Simulink diagram,
    %   renamed to <name>.slx, so the name is also the diagram's name: letters, digits and _,
    %   starting with a letter. When the template holds more than one diagram, it asks which
    %   one to start from; the others stay behind. The template's caches, builds, saved data,
    %   settings and documentation stay behind too, so the console starts the new project with
    %   the default settings. Afterwards: cd into the project and run SPOT.
    %
    %   Inputs
    %     name      the project's name (optional: asked for in a dialog, or in the Command
    %               Window without a desktop)
    %     location  the folder to create it in (optional: projects/, next to this file)
    %     diagram   the template's diagram to start from, e.g. 'Template_LinearFlow.slx'
    %               (optional: the template's only diagram, or asked for in a list, or in the
    %               Command Window without a desktop, when it has several)
    %
    %   Outputs
    %     target    the new project's folder; '' if the name or the diagram was not given
    %
    %   Behavior
    %     1. Checks the name (a valid diagram name, not one of SPOT's or MATLAB's own).
    %     2. Picks the diagram: the template's only one, or the one given or chosen.
    %     3. Copies the template's top level, except what is left out, the diagrams and
    %        Simulink's build files, then removes Python caches and the lab console's local
    %        builds.
    %     4. Copies the user guide if the template has one, the diagram as <name>.slx, and
    %        makes an empty Saved Data folder.
    %     On an error the half-made project is removed.
    %
    %   See also SPOT.

    %% Constants
    % Not copied from the template: its own state, builds and the developers' documentation.
    LEAVE_OUT = {'.', '..', '.spot_cache', 'slprj', 'Saved Data', 'documentation', ...
                 'STATUS.md', 'spot_settings.json', 'spot_design.json'};
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

    % A folder not taken yet.
    target = fullfile(char(location), name);
    if exist(target, 'file')
        error('SPOT:newProject', '%s already exists. Choose another name or location.', target);
    end

    % The template's diagram to start from: its only one, else the one given or chosen
    % (cancelled: nothing to do).
    found = dir(fullfile(template, '*.slx'));
    diagrams = {found.name};
    if isempty(diagrams)
        error('SPOT:newProject', 'The template (%s) has no diagram (.slx).', template);
    end
    if nargin < 3 || isempty(diagram)
        if isscalar(diagrams)
            diagram = diagrams{1};
        else
            diagram = askDiagram(diagrams);
            if isempty(diagram)
                target = '';
                return
            end
        end
    end
    diagram = char(diagram);
    if ~endsWith(diagram, '.slx')
        diagram = [diagram '.slx'];
    end
    if ~ismember(diagram, diagrams)
        error('SPOT:newProject', 'The template has no diagram %s; it has %s.', ...
            diagram, strjoin(diagrams, ', '));
    end

    %% Copy
    mkdir(target);
    try
        % Everything at the top of the template, except what is left out, the diagrams (the
        % chosen one is copied below) and Simulink's build files.
        items = dir(template);
        items = items(~ismember({items.name}, [LEAVE_OUT, diagrams]) ...
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

        % The diagram, named after the project, and an empty folder for the results.
        copyfile(fullfile(template, diagram), fullfile(target, [name '.slx']));
        mkdir(fullfile(target, 'Saved Data'));
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

function diagram = askDiagram(diagrams)
    %ASKDIAGRAM The template diagram to start the project from (a list, or the Command Window).
    %   diagram = askDiagram(diagrams) asks which of diagrams (file names) to use; '' if
    %   cancelled.

    %% Constants
    PROMPT = {'The template has more than one diagram.', ...
              'Which one should the project start from?'};
    LIST_SIZE = [360 120];   % [width height] of the dialog's list [pixels]

    %% Ask
    diagram = '';

    % A list to pick from, with a desktop.
    if usejava('desktop')
        choice = listdlg('ListString', diagrams, 'SelectionMode', 'single', ...
                         'PromptString', PROMPT, 'Name', 'New SPOT Project', ...
                         'ListSize', LIST_SIZE);
        if ~isempty(choice)
            diagram = diagrams{choice};
        end
        return
    end

    % Without one, a numbered list in the Command Window, until a number is given.
    fprintf('%s\n', PROMPT{:});
    for k = 1:numel(diagrams)
        fprintf('  %d  %s\n', k, diagrams{k});
    end
    while true
        answer = strtrim(input(sprintf('Number (1-%d, Enter to cancel): ', numel(diagrams)), 's'));
        if isempty(answer)
            return
        end
        choice = str2double(answer);
        if choice >= 1 && choice <= numel(diagrams) && choice == round(choice)
            diagram = diagrams{choice};
            return
        end
        fprintf('Type a number from 1 to %d.\n', numel(diagrams));
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
    elseif any(strcmpi(name, {'spot', 'SPOT_User_Parameters'}))
        problem = sprintf('''%s'' is one of SPOT''s own names; choose another.', name);
    elseif ~isempty(which(name)) && ~strcmp(which(name), 'variable')
        problem = sprintf('''%s'' is already a MATLAB function (%s); choose another name.', ...
            name, which(name));
    end
end
