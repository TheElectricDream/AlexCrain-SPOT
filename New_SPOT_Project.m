function target = New_SPOT_Project(name, location)
%NEW_SPOT_PROJECT Creates a new SPOT project from the template.
%   New_SPOT_Project                     asks for the project's name
%   New_SPOT_Project('MyProject')        creates projects/MyProject
%   New_SPOT_Project('MyProject', folder) creates folder/MyProject instead
%   target = New_SPOT_Project(...)       also returns the new project's folder ('' if cancelled)
%
%   A project is a copy of the template folder (next to this file) with its Simulink diagram
%   renamed to <name>.slx, so the name is also the diagram's name: letters, digits and _,
%   starting with a letter. The template's caches, builds, saved data, settings and documentation
%   stay behind, so the console starts the new project with the default settings.
%   Afterwards: cd into the project and run SPOT.

root = fileparts(mfilename('fullpath'));
template = fullfile(root, 'template');
% Not copied from the template (its own state, builds and the developers' documentation)
leaveOut = {'.', '..', '.spot_cache', 'slprj', 'Saved Data', 'documentation', 'STATUS.md', ...
            'spot_settings.json', 'spot_design.json'};

if nargin < 1 || isempty(name)
    name = askName();
    if isempty(name)
        target = '';
        return                                      % cancelled
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
diagram = dir(fullfile(template, '*.slx'));
if numel(diagram) ~= 1
    error('SPOT:newProject', 'The template (%s) must hold exactly one diagram (.slx); it has %d.', ...
        template, numel(diagram));
end
target = fullfile(char(location), name);
if exist(target, 'file')
    error('SPOT:newProject', '%s already exists. Choose another name or location.', target);
end

mkdir(target);
try
    % Everything at the top of the template, except what is left out and Simulink's build files
    items = dir(template);
    items = items(~ismember({items.name}, [leaveOut, {diagram.name}]) & ...
                  ~endsWith({items.name}, {'.slxc', '.autosave', '_ert_rtw'}));
    for k = 1:numel(items)
        copyfile(fullfile(template, items(k).name), fullfile(target, items(k).name));
    end
    % Python caches and the lab console's local builds inside +spot
    inside = dir(fullfile(target, '**'));
    caches = inside([inside.isdir] & strcmp({inside.name}, '__pycache__'));
    for k = 1:numel(caches)
        rmdir(fullfile(caches(k).folder, caches(k).name), 's');
    end
    if isfolder(fullfile(target, '+spot', '+console', 'output'))
        rmdir(fullfile(target, '+spot', '+console', 'output'), 's');
    end
    % The user guide, if the template has it
    guide = fullfile(template, 'documentation', 'SPOT_Tutorial.pdf');
    if isfile(guide)
        copyfile(guide, target);
    end
    % The diagram, named after the project, and an empty folder for the results
    copyfile(fullfile(template, diagram.name), fullfile(target, [name '.slx']));
    mkdir(fullfile(target, 'Saved Data'));
catch err
    rmdir(target, 's');                             % don't leave a half-made project behind
    rethrow(err);
end

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
% The project's name from a dialog (or the Command Window without a desktop); '' if cancelled
prompt = 'Project name (letters, digits and _, starting with a letter; it also names the diagram):';
name = '';
while true
    if usejava('desktop')
        answer = inputdlg(prompt, 'New SPOT Project', [1 70], {name});
        if isempty(answer)
            name = '';
            return
        end
        name = strtrim(answer{1});
    else
        name = strtrim(input([prompt ' '], 's'));
    end
    if isempty(name)
        return
    end
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
% Why the name can't be used ('' if it can): it names a folder and a Simulink diagram
problem = '';
if ~isvarname(name) || numel(name) > 63
    problem = sprintf(['''%s'' can''t be a diagram name: use letters, digits and _ only, starting ' ...
        'with a letter, at most 63 characters, and not a MATLAB keyword.'], name);
elseif any(strcmpi(name, {'spot', 'SPOT_User_Parameters'}))
    problem = sprintf('''%s'' is one of SPOT''s own names; choose another.', name);
elseif ~isempty(which(name)) && ~strcmp(which(name), 'variable')
    problem = sprintf('''%s'' is already a MATLAB function (%s); choose another name.', name, which(name));
end
end
