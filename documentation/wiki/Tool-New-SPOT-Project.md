# New_SPOT_Project

**File:** `New_SPOT_Project.m`, next to the `template/` and `projects/` folders

<!-- source: ../New_SPOT_Project.m -->

## What it is for

Make a new project from the template: a copy of `template/` with the Simulink diagram renamed after the project, without the template's caches, builds, saved data, settings or documentation. A MATLAB function, so it works the same on Windows, Linux and macOS with nothing else installed.

```
SPOT-…/
├── New_SPOT_Project.m      ← this
├── template/               ← what is copied
└── projects/               ← where new projects go
    └── MyProject/          (SPOT.m, SPOT_User_Parameters.m, +spot/, resources/, MyProject.slx, Saved Data/, …)
```

## How to call it

```matlab
New_SPOT_Project                          % asks for the name (a dialog, or the Command Window)
New_SPOT_Project('MyProject')             % creates projects/MyProject with MyProject.slx
New_SPOT_Project('MyProject', 'D:\SPOT')  % creates D:\SPOT\MyProject instead
folder = New_SPOT_Project('MyProject');   % also returns the new folder ('' if the dialog was cancelled)
```

Then `cd` into the project and run `SPOT` (in the desktop, the printed link does both).

The name is also the **Simulink model's name**, so it must be a valid MATLAB name: letters, digits and `_`, starting with a letter, at most 63 characters, not a keyword (`end`, `for`, …), not `spot` or `SPOT_User_Parameters`, and not the name of an existing MATLAB function (a model called `plot` would hide `plot`).

## Line by line

**Lines 1–12.** The signature and the help text (`help New_SPOT_Project`). `name` and `location` are optional; `target` is the new folder.

**Lines 14–15.** `root` is the folder this file is in (`mfilename('fullpath')` is this file without `.m`); the template is `root/template`. The function therefore works from anywhere, whatever the current folder is, as long as it sits next to `template/`.

**Lines 16–18.** `leaveOut`: the entries at the top of the template that a project must not start with: `.` and `..` (which `dir` always lists), the console's `.spot_cache` (session token, last results), Simulink's `slprj` build cache, the template's own `Saved Data`, the developers' `documentation` and `STATUS.md`, and the template's `spot_settings.json` / `spot_design.json`, so the new project starts from the default settings (the console creates them on first start).

**Lines 20–26.** No name given: ask for one (`askName`, below). An empty answer means the user cancelled: return `''` without creating anything.

**Lines 27–31.** Tidy the name (a string becomes a char array; spaces around it are dropped) and check it with `nameProblem`; a problem is an error with the reason.

**Lines 32–34.** No location given: `root/projects`.

**Lines 35–39.** The template's diagram: exactly one `.slx` is expected at its top (the one that will be renamed).

**Lines 40–43.** The project's folder, `location/name`; refuse if anything of that name exists (`exist(…, 'file')` is true for files and folders).

**Line 45.** Create the folder.

**Lines 46–53.** Copy every entry at the top of the template except those in `leaveOut`, the diagram (copied under its new name below) and Simulink's per-computer build files (`*.slxc`, `*.autosave`, `*_ert_rtw`). `copyfile` copies a folder with everything in it, so `+spot/` and `resources/` come whole.

**Lines 54–62.** Remove what came along inside `+spot`: Python's `__pycache__` folders (`dir(…, '**')` lists every file and folder below the project; keep the folders named `__pycache__` and delete each) and the lab console's local StreamData builds in `+spot/+console/output`.

**Lines 63–67.** The user guide, `SPOT_Tutorial.pdf`, goes to the top of the project, if the template has it (the documentation folder is not part of the published repository).

**Lines 68–70.** The diagram, copied as `<name>.slx`. Simulink takes a model's name from its file name, so nothing inside the file needs changing; the console finds it as the project's only diagram ([[spot.sim.listDiagrams|spot-sim-listDiagrams]]). Then an empty `Saved Data/` for the results.

**Lines 71–74.** If any step failed, delete the half-made folder and pass the error on.

**Lines 76–82.** Say where the project is and how to start it. In the MATLAB desktop the second line is a link that runs `cd('<folder>'); SPOT`; elsewhere (e.g. `matlab -batch`) the same command as text. Quotes in the path are doubled so the command stays valid.

**Lines 83–85.** Called without an output (`New_SPOT_Project('X')` at the prompt), don't also print `target = …`.

**Lines 88–116, `askName`.** With the desktop: a dialog (`inputdlg`) pre-filled with the last attempt; Cancel returns `''`. Without it: a prompt in the Command Window (`input(…, 's')`). An empty answer returns `''`. A name with a problem shows the reason (an error dialog, or a printed line) and asks again.

**Lines 118–129, `nameProblem`.** Returns `''` or the reason a name can't be used:
- `isvarname` is true for letters, digits and `_` starting with a letter, not a keyword; Simulink's limit on a model name is 63 characters;
- `spot` (the package) and `SPOT_User_Parameters` are SPOT's own;
- `which(name)` finds an existing function or file of that name on the path (a model with that name would shadow it, or be shadowed); `'variable'` only means a variable in this function, which doesn't matter.

## The old version

Until 2026-10-12 this was `New_SPOT_Project.py` with a `.bat` launcher (a Tk window, Python 3.6+), inside the template. Both files are in `archive/`.

## The code
<!-- listing -->
<details>
<summary><b>../New_SPOT_Project.m</b> (129 lines; click to show)</summary>

```matlab
   1  function target = New_SPOT_Project(name, location)
   2  %NEW_SPOT_PROJECT Creates a new SPOT project from the template.
   3  %   New_SPOT_Project                     asks for the project's name
   4  %   New_SPOT_Project('MyProject')        creates projects/MyProject
   5  %   New_SPOT_Project('MyProject', folder) creates folder/MyProject instead
   6  %   target = New_SPOT_Project(...)       also returns the new project's folder ('' if cancelled)
   7  %
   8  %   A project is a copy of the template folder (next to this file) with its Simulink diagram
   9  %   renamed to <name>.slx, so the name is also the diagram's name: letters, digits and _,
  10  %   starting with a letter. The template's caches, builds, saved data, settings and documentation
  11  %   stay behind, so the console starts the new project with the default settings.
  12  %   Afterwards: cd into the project and run SPOT.
  13  
  14  root = fileparts(mfilename('fullpath'));
  15  template = fullfile(root, 'template');
  16  % Not copied from the template (its own state, builds and the developers' documentation)
  17  leaveOut = {'.', '..', '.spot_cache', 'slprj', 'Saved Data', 'documentation', 'STATUS.md', ...
  18              'spot_settings.json', 'spot_design.json'};
  19  
  20  if nargin < 1 || isempty(name)
  21      name = askName();
  22      if isempty(name)
  23          target = '';
  24          return                                      % cancelled
  25      end
  26  end
  27  name = strtrim(char(name));
  28  problem = nameProblem(name);
  29  if ~isempty(problem)
  30      error('SPOT:newProject', '%s', problem);
  31  end
  32  if nargin < 2 || isempty(location)
  33      location = fullfile(root, 'projects');
  34  end
  35  diagram = dir(fullfile(template, '*.slx'));
  36  if numel(diagram) ~= 1
  37      error('SPOT:newProject', 'The template (%s) must hold exactly one diagram (.slx); it has %d.', ...
  38          template, numel(diagram));
  39  end
  40  target = fullfile(char(location), name);
  41  if exist(target, 'file')
  42      error('SPOT:newProject', '%s already exists. Choose another name or location.', target);
  43  end
  44  
  45  mkdir(target);
  46  try
  47      % Everything at the top of the template, except what is left out and Simulink's build files
  48      items = dir(template);
  49      items = items(~ismember({items.name}, [leaveOut, {diagram.name}]) & ...
  50                    ~endsWith({items.name}, {'.slxc', '.autosave', '_ert_rtw'}));
  51      for k = 1:numel(items)
  52          copyfile(fullfile(template, items(k).name), fullfile(target, items(k).name));
  53      end
  54      % Python caches and the lab console's local builds inside +spot
  55      inside = dir(fullfile(target, '**'));
  56      caches = inside([inside.isdir] & strcmp({inside.name}, '__pycache__'));
  57      for k = 1:numel(caches)
  58          rmdir(fullfile(caches(k).folder, caches(k).name), 's');
  59      end
  60      if isfolder(fullfile(target, '+spot', '+console', 'output'))
  61          rmdir(fullfile(target, '+spot', '+console', 'output'), 's');
  62      end
  63      % The user guide, if the template has it
  64      guide = fullfile(template, 'documentation', 'SPOT_Tutorial.pdf');
  65      if isfile(guide)
  66          copyfile(guide, target);
  67      end
  68      % The diagram, named after the project, and an empty folder for the results
  69      copyfile(fullfile(template, diagram.name), fullfile(target, [name '.slx']));
  70      mkdir(fullfile(target, 'Saved Data'));
  71  catch err
  72      rmdir(target, 's');                             % don't leave a half-made project behind
  73      rethrow(err);
  74  end
  75  
  76  fprintf('Created %s\n', target);
  77  if usejava('desktop')
  78      fprintf('To start it: <a href="matlab:cd(''%s''); SPOT">cd there and run SPOT</a>\n', ...
  79          strrep(target, '''', ''''''));
  80  else
  81      fprintf('To start it: cd(''%s''); SPOT\n', strrep(target, '''', ''''''));
  82  end
  83  if nargout == 0
  84      clear target
  85  end
  86  end
  87  
  88  function name = askName()
  89  % The project's name from a dialog (or the Command Window without a desktop); '' if cancelled
  90  prompt = 'Project name (letters, digits and _, starting with a letter; it also names the diagram):';
  91  name = '';
  92  while true
  93      if usejava('desktop')
  94          answer = inputdlg(prompt, 'New SPOT Project', [1 70], {name});
  95          if isempty(answer)
  96              name = '';
  97              return
  98          end
  99          name = strtrim(answer{1});
 100      else
 101          name = strtrim(input([prompt ' '], 's'));
 102      end
 103      if isempty(name)
 104          return
 105      end
 106      problem = nameProblem(name);
 107      if isempty(problem)
 108          return
 109      end
 110      if usejava('desktop')
 111          uiwait(errordlg(problem, 'New SPOT Project', 'modal'));
 112      else
 113          fprintf('%s\n', problem);
 114      end
 115  end
 116  end
 117  
 118  function problem = nameProblem(name)
 119  % Why the name can't be used ('' if it can): it names a folder and a Simulink diagram
 120  problem = '';
 121  if ~isvarname(name) || numel(name) > 63
 122      problem = sprintf(['''%s'' can''t be a diagram name: use letters, digits and _ only, starting ' ...
 123          'with a letter, at most 63 characters, and not a MATLAB keyword.'], name);
 124  elseif any(strcmpi(name, {'spot', 'SPOT_User_Parameters'}))
 125      problem = sprintf('''%s'' is one of SPOT''s own names; choose another.', name);
 126  elseif ~isempty(which(name)) && ~strcmp(which(name), 'variable')
 127      problem = sprintf('''%s'' is already a MATLAB function (%s); choose another name.', name, which(name));
 128  end
 129  end
```

</details>
<!-- /listing -->
