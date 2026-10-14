function out = build(s, root)
%BUILD Build the selected diagram for the platforms, on the lab console (TX2).
%   out = spot.experiment.build(s, projectFolder) puts the settings into the workspace, then, for the
%   build only, comments out the diagram's "Simulate Plant Dynamics" block, sets the solver to
%   Fixed-step ode4, the hardware board to NVIDIA Jetson and its build action to "Build" (never
%   "Build and run": the program must not run on the TX2), and runs slbuild with the TX2 as the
%   board (connect first with spot.experiment.connect). Afterwards every change to the diagram is
%   undone and the diagram is NOT saved (the old GUI saved it with the plant commented out).
%   The lab console then deploys the program to the platforms.
%
%   What slbuild prints goes to <projectFolder>/.spot_cache/build.log as it happens (the SPOT
%   Console shows it live) as well as to the Command Window.
%
%   out: model, program (path on the TX2), seconds, log (what the build printed).
p = spot.experiment.plan(s, root);
if ~p.hasJetsonSupport
    error('spot:experiment:build', 'The MATLAB Coder Support Package for NVIDIA Jetson is not installed.');
end
if isempty(p.platforms)
    error('spot:experiment:build', 'Select at least one platform (Scenario) first.');
end
model = p.model;
b = p.board;
spot.params.apply(s, root);

% Build on the TX2, in this project's folder there
group = 'Hardware_Connectivity_Installer_NVIDIAJetson';
setpref(group, 'DefaultIpAddress', b.address);
setpref(group, 'DefaultUserName', b.user);
setpref(group, 'DefaultBuildDirPref', b.buildDir);
setpref(group, 'DefaulttargetwsdirPref', b.workspaceDir);

% Changes for the build, all undone afterwards
wasDirty = strcmp(get_param(model, 'Dirty'), 'on');
cs = getActiveConfigSet(model);
plant = [model '/Simulate Plant Dynamics'];
old = struct('commented', get_param(plant, 'Commented'), 'solverType', get_param(model, 'SolverType'), ...
             'solver', get_param(model, 'SolverName'), 'board', get_param(cs, 'HardwareBoard'), ...
             'action', buildAction(cs));
restore = onCleanup(@() undo(model, cs, plant, old, wasDirty));
set_param(plant, 'Commented', 'on');
set_param(model, 'SolverType', 'Fixed-step', 'SolverName', 'ode4');
set_param(cs, 'HardwareBoard', 'NVIDIA Jetson');
% Build only: with "Build and run" MATLAB would also start the program on the TX2 (as root)
codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', 'Build');
if ~strcmp(codertarget.data.getParameterValue(cs, 'Runtime.BuildAction'), 'Build')
    error('spot:experiment:build', ['Could not set the build action to "Build". Set it in the diagram: ' ...
        'Hardware Implementation > Target hardware resources > Build options.']);
end
assignin('base', 'simMode', 0);

% The build's output, as it happens, for the console
cache = fullfile(root, '.spot_cache');
if ~isfolder(cache)
    mkdir(cache);
end
logFile = fullfile(cache, 'build.log');
if isfile(logFile)
    delete(logFile);
end
diary(logFile);
stopDiary = onCleanup(@() diary('off'));
fprintf('### SPOT: building %s on %s@%s (build action: Build)\n', model, b.user, b.address);

tic;
slbuild(model);
seconds = toc;
diary('off');
log = '';
if isfile(logFile)
    log = strtrim(regexprep(fileread(logFile), '<a [^>]*>|</a>', ''));
end
out = struct('model', model, 'program', p.program, 'seconds', seconds, 'log', log, 'platforms', {p.platforms});
end

function a = buildAction(cs)
% The diagram's build action ('' if its board isn't a Jetson target, so there is none)
try
    a = codertarget.data.getParameterValue(cs, 'Runtime.BuildAction');
catch
    a = '';
end
end

function undo(model, cs, plant, old, wasDirty)
assignin('base', 'simMode', 1);
set_param(plant, 'Commented', old.commented);
set_param(model, 'SolverType', old.solverType, 'SolverName', old.solver);
set_param(cs, 'HardwareBoard', old.board);
if ~isempty(old.action) && ~isempty(buildAction(cs))
    codertarget.data.setParameterValue(cs, 'Runtime.BuildAction', old.action);
end
if ~wasDirty
    set_param(model, 'Dirty', 'off');   % back as it was on disk
end
end
