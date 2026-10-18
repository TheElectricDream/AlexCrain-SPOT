# Making a New Project

You don't work in the repository's `template/` folder directly: each person or experiment gets a **project**, a copy of the template with its own settings, results and diagram. `New_SPOT_Project.m`, next to `template/`, makes one.

```text
SPOT/
├── New_SPOT_Project.m     makes projects
├── template/              what is copied
└── projects/              where new projects go (not published)
    └── MyProject/         SPOT.m, Constants.m, +spot/, MyProject.slx, Saved Data/, ...
```

## How to call it

In MATLAB, in the folder that holds `New_SPOT_Project.m`:

```matlab
New_SPOT_Project                          % asks for the name (a dialog, or the Command Window)
New_SPOT_Project('MyProject')             % creates projects/MyProject with MyProject.slx
New_SPOT_Project('MyProject', 'D:\SPOT')  % creates D:\SPOT\MyProject instead
folder = New_SPOT_Project('MyProject');   % also returns the new folder ('' if cancelled)
```

Then `cd` into the project and run `SPOT` ([Quick Start: Your First Simulation](page:quick-start-simulation)).

## The name

The name is also the **Simulink model's name**, so it must be a valid MATLAB name: letters, digits and `_`, starting with a letter, at most 63 characters, not a MATLAB keyword (`end`, `for`, ...), not `spot` or `Constants`, and not the name of an existing MATLAB function (a model called `plot` would hide `plot`). A name with a problem is refused with the reason, and the dialog asks again.

## What is copied

Everything at the top of the template, except:

- its state: `.spot_cache`, and `Saved Data/` (a new, empty one is made). `Constants.m` is copied, so a new project starts from the template's values;
- Simulink's build files (`slprj`, `*.slxc`, `*.autosave`, `*_ert_rtw`), Python's `__pycache__` folders and the lab console's StreamData builds;
- the developers' `documentation/` sources and `STATUS.md`.

The diagram is copied as `<name>.slx`; Simulink takes a model's name from its file name, so nothing inside it changes. This documentation is part of `+spot`, so every project has it, and notes you add in a project stay with that project.

## Moving your work to a newer template

A project keeps the `+spot` it was made with. To use a newer version of SPOT, make a new project from the updated template and copy your own work across: your diagram's custom subsystems, the values you changed or added in `Constants.m` (every setting, the Designer's choices and the calibration you took are constants there too), and `Saved Data/`.
