function t = timing(s)
    %TIMING Phase durations, end times and total time from the settings.
    %   t = spot.params.timing(s)
    %
    %   Segments switched off in s.timing.on (phase 2, the four phase 3 sub-phases, phase 4)
    %   count as zero seconds, so they are skipped: the diagrams pick the phase with half-open
    %   windows (PhaseK-1_End <= t < PhaseK_End), and a zero-length window is never entered.
    %   Phases 0, 1 and 5 always run (they start and stop the platforms). Phase 3's clock (time
    %   since Phase2_End) starts with the first sub-phase that runs.
    %
    %   Inputs
    %     s  settings
    %
    %   Outputs
    %     t  struct, named as the diagrams expect in the workspace:
    %          Phase0_Duration ... Phase5_Duration, Phase3_SubPhase1_Duration ... _4, the
    %          matching *_End times, and tsim
    %          phases    the segments that run: names and end times (playback markers)
    %          segments  all nine segments for the console's timeline: name, duration (as
    %                    typed), run (on or skipped), start and end [s] as they will run
    %
    %   See also spot.params.apply.

    %% Check the durations
    cfg = s.timing;
    sub = cfg.subPhases(:).';
    if numel(sub) ~= 4
        error('spot:params:timing', 'Phase 3 needs exactly four sub-phase durations.');
    end
    if any([cfg.phase0, cfg.phase1, cfg.phase2, cfg.phase4, cfg.phase5] < 0) || any(sub < 0)
        error('spot:params:timing', 'Phase durations cannot be negative.');
    end

    %% Durations as they run (switched-off segments are 0)
    on = switches(cfg);
    sub = sub .* on.subPhases;
    durations = [cfg.phase0, cfg.phase1, cfg.phase2 * on.phase2, sum(sub), ...
                 cfg.phase4 * on.phase4, cfg.phase5];
    ends = cumsum(durations);

    %% Workspace variables
    t = struct();
    for k = 0:5
        t.(sprintf('Phase%d_Duration', k)) = durations(k + 1);
        t.(sprintf('Phase%d_End', k)) = ends(k + 1);
    end
    subEnds = ends(3) + cumsum(sub);
    for k = 1:4
        t.(sprintf('Phase3_SubPhase%d_Duration', k)) = sub(k);
        t.(sprintf('Phase3_SubPhase%d_End', k)) = subEnds(k);
    end
    t.tsim = ends(end);

    %% Segments, for the console
    names = {'Phase 0', 'Phase 1', 'Phase 2', 'Phase 3.1', 'Phase 3.2', 'Phase 3.3', ...
             'Phase 3.4', 'Phase 4', 'Phase 5'};
    segEnds = [ends(1:3), subEnds, ends(5:6)];
    segDur = [durations(1:3), sub, durations(5:6)];

    % The ones that run, for playback markers.
    keep = segDur > 0;
    t.phases = struct('names', {names(keep)}, 'ends', segEnds(keep));

    % All nine, with the typed durations and whether each runs.
    typed = [cfg.phase0, cfg.phase1, cfg.phase2, cfg.subPhases(:).', cfg.phase4, cfg.phase5];
    runs = [true, true, on.phase2, on.subPhases, on.phase4, true];
    t.segments = struct('name', names, 'duration', num2cell(typed), 'run', num2cell(runs), ...
                        'start', num2cell(segEnds - segDur), 'end', num2cell(segEnds));
end

function on = switches(cfg)
    % The on/off switches; all on when the settings don't have them (older settings files).
    on = struct('phase2', true, 'subPhases', true(1, 4), 'phase4', true);
    if isfield(cfg, 'on') && isstruct(cfg.on)
        if isfield(cfg.on, 'phase2')
            on.phase2 = logical(cfg.on.phase2);
        end
        if isfield(cfg.on, 'phase4')
            on.phase4 = logical(cfg.on.phase4);
        end
        if isfield(cfg.on, 'subPhases') && numel(cfg.on.subPhases) == 4
            on.subPhases = logical(cfg.on.subPhases(:).');
        end
    end
end
