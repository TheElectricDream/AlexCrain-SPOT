function v = toConstants(s)
    %TOCONSTANTS The constants a settings struct stands for (the inverse of fromConstants).
    %   v = spot.params.toConstants(s) returns a struct NAME -> value for every constant the
    %   settings come from, so spot.params.writeConstants(v) puts a changed setting back into
    %   Constants.m. The bifilar pendulum's string length and spacing are one constant each
    %   (the lab's rig): the first platform's value that differs from the constant wins.
    %
    %   Inputs
    %     s  settings struct (spot.params.fromConstants, with the page's changes)
    %
    %   Outputs
    %     v  struct, constant name -> value
    %
    %   See also spot.params.fromConstants, spot.params.writeConstants.

    v = struct();

    %% Diagram, platforms and rates
    v.DIAGRAM = char(s.diagram);
    v.USE_RED = logical(s.platforms.RED);
    v.USE_BLACK = logical(s.platforms.BLACK);
    v.USE_BLUE = logical(s.platforms.BLUE);
    v.USE_ARM = logical(s.platforms.ARM);
    v.REAL_TIME = logical(s.realTime);
    v.SAMPLE_RATE = s.sampleRate;
    v.DATA_RATE = s.dataRate;
    v.PWM_FREQUENCY = s.pwmFrequency;
    v.IP_LAB_CONSOLE = char(s.labConsoleAddress);

    %% Timeline
    t = s.timing;
    v.PHASE0_DURATION = t.phase0;
    v.PHASE1_DURATION = t.phase1;
    v.PHASE2_DURATION = t.phase2;
    v.SUB_PHASE_DURATIONS = row(t.subPhases);
    v.PHASE4_DURATION = t.phase4;
    v.PHASE5_DURATION = t.phase5;
    v.PHASE2_ON = logical(t.on.phase2);
    v.SUB_PHASES_ON = logical(row(t.on.subPhases));
    v.PHASE4_ON = logical(t.on.phase4);

    %% Solver
    v.SOLVER_TYPE = char(s.solver.type);
    v.SOLVER_FIXED_STEP = char(s.solver.fixedStepName);
    v.SOLVER_VARIABLE_STEP = char(s.solver.variableStepName);
    v.SOLVER_MAX_STEP = char(s.solver.maxStep);
    v.SOLVER_MIN_STEP = char(s.solver.minStep);
    v.SOLVER_INITIAL_STEP = char(s.solver.initialStep);
    v.SOLVER_REL_TOL = char(s.solver.relTol);
    v.SOLVER_ABS_TOL = char(s.solver.absTol);

    %% Poses, masses, thrusters and weights, per platform
    for name = {'RED', 'BLACK', 'BLUE'}
        p = name{1};
        v.(['INITIAL_POSE_' p]) = row(s.initial.(p).initial);
        v.(['HOME_POSE_' p]) = row(s.initial.(p).home);
        v.(['SCALES_' p]) = row(s.mass.(p).scales);
        v.(['BIFILAR_PERIOD_' p]) = s.mass.(p).bifilar.T;
        v.(['MASS_' p]) = s.mass.(p).mass;
        v.(['INERTIA_' p]) = s.mass.(p).inertia;
        v.(['THRUST_' p]) = s.thrusterForce.(p);
        v.(['THRUST_DECAY_' p]) = s.thrusterDecay.(p);
        v.(['LQR_Q_' p]) = row(s.control.(p).lqr.Q);
        v.(['LQR_R_' p]) = row(s.control.(p).lqr.R);
    end
    v.INITIAL_POSE_ARM = row(s.initial.ARM.initial);
    v.MASS_OVERRIDE = logical(s.mass.override);
    v.BIFILAR_LENGTH = shared(Constants.BIFILAR_LENGTH, s.mass.RED.bifilar.L, ...
                              s.mass.BLACK.bifilar.L, s.mass.BLUE.bifilar.L);
    v.BIFILAR_SPACING = shared(Constants.BIFILAR_SPACING, s.mass.RED.bifilar.D, ...
                               s.mass.BLACK.bifilar.D, s.mass.BLUE.bifilar.D);

    %% The arm
    a = s.mass.arm;
    v.ARM_PHI = a.Phi;
    v.ARM_B0 = a.b0;
    v.ARM_A1 = a.a1;
    v.ARM_B1 = a.b1;
    v.ARM_A2 = a.a2;
    v.ARM_B2 = a.b2;
    v.ARM_A3 = a.a3;
    v.ARM_B3 = a.b3;
    v.ARM_MASS_PROX = a.mPROX;
    v.ARM_MASS_DIST = a.mDIST;
    v.ARM_MASS_EE = a.mEE;
    v.ARM_INERTIA_PROX = a.IPROX;
    v.ARM_INERTIA_DIST = a.IDIST;
    v.ARM_INERTIA_EE = a.IEE;
    v.ARM_MOUNT_FACE = char(a.mount.face);
    v.ARM_MOUNT_SIDE = char(a.mount.side);
    c = s.control.ARM;
    v.ARM_POSITION_P = row(c.position.P);
    v.ARM_POSITION_I = row(c.position.I);
    v.ARM_POSITION_D = row(c.position.D);
    v.ARM_POSITION_PROFILE_VELOCITY = c.position.profileVelocity;
    v.ARM_POSITION_PROFILE_ACCELERATION = c.position.profileAcceleration;
    v.ARM_POSITION_PWM_LIMIT = c.position.pwmLimit;
    v.ARM_VELOCITY_P = row(c.velocity.P);
    v.ARM_VELOCITY_I = row(c.velocity.I);
    v.ARM_VELOCITY_PROFILE_ACCELERATION = c.velocity.profileAcceleration;
    v.ARM_VELOCITY_PWM_LIMIT = c.velocity.pwmLimit;

    %% The calibration switches
    v.CAL_USE_TABLE = logical(s.calibration.tableSlope);
    v.CAL_USE_THRUSTERS = logical(s.calibration.thrusters);
    v.CAL_USE_ARM = logical(s.calibration.arm);
end

function x = row(x)
    % A vector as a row (the page's JSON arrays come back as columns).
    if isnumeric(x) || islogical(x)
        x = reshape(x, 1, []);
    end
end

function x = shared(current, varargin)
    % One constant behind several fields: the first value that differs from it, else itself.
    x = current;
    for k = 1:numel(varargin)
        if ~isequal(varargin{k}, current)
            x = varargin{k};
            return
        end
    end
end
