function t = timing(s)
%TIMING Phase durations, end times and total time from the settings.
%   t = spot.params.timing(s) returns a struct with Phase0_Duration ... Phase5_Duration,
%   Phase3_SubPhase1_Duration ... _4, the matching *_End times and tsim, named as the
%   Simulink diagrams expect them in the workspace.
%
%   Segments switched off in s.timing.on (phase 2, the four phase 3 sub-phases, phase 4) count as
%   zero seconds, so they are skipped: the diagrams pick the phase with half-open windows
%   (PhaseK-1_End <= t < PhaseK_End), and a window of zero length is never entered. The durations
%   typed in the settings are kept. Phases 0, 1 and 5 always run (they start and stop the
%   platforms). Phase 3's clock (time since Phase2_End) starts with the first sub-phase that runs.
%
%   t.phases lists the segments that run: names and end times, for the playback's markers.
%   t.segments lists all nine segments for the console's timeline: name, duration (as typed),
%   run (on or skipped), start and end [s] as they will run.
tm = s.timing;
sub = tm.subPhases(:).';
if numel(sub) ~= 4
    error('spot:params:timing', 'Phase 3 needs exactly four sub-phase durations.');
end
if any([tm.phase0, tm.phase1, tm.phase2, tm.phase4, tm.phase5] < 0) || any(sub < 0)
    error('spot:params:timing', 'Phase durations cannot be negative.');
end
on = switches(tm);
sub = sub .* on.subPhases;
d = [tm.phase0, tm.phase1, tm.phase2 * on.phase2, sum(sub), tm.phase4 * on.phase4, tm.phase5];
ends = cumsum(d);
t = struct();
for k = 0:5
    t.(sprintf('Phase%d_Duration', k)) = d(k + 1);
    t.(sprintf('Phase%d_End', k)) = ends(k + 1);
end
subEnds = ends(3) + cumsum(sub);
for k = 1:4
    t.(sprintf('Phase3_SubPhase%d_Duration', k)) = sub(k);
    t.(sprintf('Phase3_SubPhase%d_End', k)) = subEnds(k);
end
t.tsim = ends(end);

names = {'Phase 0', 'Phase 1', 'Phase 2', 'Phase 3.1', 'Phase 3.2', 'Phase 3.3', 'Phase 3.4', 'Phase 4', 'Phase 5'};
segEnds = [ends(1:3), subEnds, ends(5:6)];
segDur = [d(1:3), sub, d(5:6)];
keep = segDur > 0;
t.phases = struct('names', {names(keep)}, 'ends', segEnds(keep));
typed = [tm.phase0, tm.phase1, tm.phase2, tm.subPhases(:).', tm.phase4, tm.phase5];
runs = [true, true, on.phase2, on.subPhases, on.phase4, true];
t.segments = struct('name', names, 'duration', num2cell(typed), 'run', num2cell(runs), ...
                    'start', num2cell(segEnds - segDur), 'end', num2cell(segEnds));
end

function on = switches(tm)
% The on/off switches, all on when the settings don't have them (older settings files)
on = struct('phase2', true, 'subPhases', true(1, 4), 'phase4', true);
if isfield(tm, 'on') && isstruct(tm.on)
    if isfield(tm.on, 'phase2'), on.phase2 = logical(tm.on.phase2); end
    if isfield(tm.on, 'phase4'), on.phase4 = logical(tm.on.phase4); end
    if isfield(tm.on, 'subPhases') && numel(tm.on.subPhases) == 4
        on.subPhases = logical(tm.on.subPhases(:).');
    end
end
end
