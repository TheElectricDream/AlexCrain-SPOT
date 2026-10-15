function r = compareSteps(folder, armModel, show)
    %COMPARESTEPS The simulated servos against the real ones, on the calibration's position steps.
    %   r = spot.arm.compareSteps(folder, armModel) replays each steps_j<N>.csv in folder
    %   through the servo's position control law (spot.arm.PositionControl at the servo's
    %   period) driving spot.arm.Plant with the base fixed, and compares the joint's motion with
    %   the measured one. The real servo's position mode sets PWM as the replica does, so a
    %   match checks the plant (motor, back-EMF, friction, inertia) and the controller replica
    %   the diagram's Default Position Control (ARM) runs.
    %   r = spot.arm.compareSteps(folder, armModel, true) also plots them.
    %
    %   Inputs
    %     folder    a run of +spot/+arm/calibration/calibrate_arm.py (position-mode goal
    %               steps with SPOT's servo settings, base fixed)
    %     armModel  the model to try, e.g. evalin('base', 'armModel') after a simulation, or
    %               spot.arm.loadCalibration(fullfile(folder, 'arm_calibration.json'), armModel)
    %     show      plot the results (default: true when there is no output)
    %
    %   Outputs
    %     r         struct array, one per file: joint, t [s], goal, measured, simulated [rad],
    %               rms, maxError [rad]
    %
    %   See also spot.arm.loadCalibration, spot.arm.PositionControl, spot.arm.Plant.

    %% Defaults
    if nargin < 3
        show = nargout == 0;
    end

    %% Constants
    GOAL_STEP = 0.01;                   % [s] goals change every 10 ms (logs are faster, uneven)
    PULSES_PER_RAD = 4096 / (2*pi);     % [pulses/rad] true encoder scale, used by the script
    CMD_PULSES_PER_RAD = 651.74;        % [pulses/rad] SPOT's goal conversion (PositionControl)

    %% Step logs
    files = dir(fullfile(folder, 'steps_j*.csv'));
    if isempty(files)
        error('spot:arm:compareSteps', 'No steps_j*.csv in %s.', folder);
    end

    %% Replay each log
    r = struct('joint', {}, 't', {}, 'goal', {}, 'measured', {}, 'simulated', {}, ...
               'rms', {}, 'maxError', {});
    for f = files.'

        % Joint stepped, measured angles of all joints, and the goal held at each goal step.
        stepLog = readtable(fullfile(f.folder, f.name));
        j = stepLog.joint(1);
        qCols = startsWith(stepLog.Properties.VariableNames, 'q');
        qLogged = stepLog{:, qCols};
        t = stepLog.t - stepLog.t(1);
        tGoal = (0:GOAL_STEP:t(end)).';
        goal = interp1(t, stepLog.goal, tGoal, 'previous', 'extrap');

        % Plant with the base fixed, stepped at the servo's period from the first measured pose.
        a = armModel;
        sv = a.servo;
        a.outerStep = sv.period;
        a.x0 = [0; 0; 0; qLogged(1, :).'];
        p = spot.arm.Plant('Model', a, 'FixedBase', true);

        % The servo's own position control, with the servo's gains.
        c = spot.arm.PositionControl( ...
            'StepTime', sv.period, 'ServoPeriod', sv.period, ...
            'PositionP', sv.positionP, 'PositionI', sv.positionI, 'PositionD', sv.positionD, ...
            'Feedforward1', sv.feedforward1, 'Feedforward2', sv.feedforward2, ...
            'ProfileVelocity', sv.profileVelocity, ...
            'ProfileAcceleration', sv.profileAcceleration, ...
            'MinPosition', sv.minPosition, 'MaxPosition', sv.maxPosition, ...
            'PWMLimit', sv.pwmLimit);

        % The script sends true pulses; scale goals so the controller's conversion gives the same.
        scale = PULSES_PER_RAD / CMD_PULSES_PER_RAD;
        held = qLogged(1, :).' * scale;

        % Simulate: the stepped joint follows the goal, the others hold their first angle.
        simulated = zeros(numel(tGoal), 1);
        stepsPerGoal = round(GOAL_STEP / sv.period);
        state = [qLogged(1, :).'; 0; 0; 0];
        for k = 1:numel(tGoal)
            cmd = held;
            cmd(j) = goal(k) * scale;
            for i = 1:stepsPerGoal
                [pos, vel] = p([0; 0; 0], c(cmd, state));
                state = [pos(4:6); vel(4:6)];
            end
            simulated(k) = pos(3 + j);
        end

        % Compare with the measured angle at the same times.
        measured = interp1(t, qLogged(:, j), tGoal);
        err = simulated - measured;
        r(end + 1) = struct('joint', j, 't', tGoal, 'goal', goal, 'measured', measured, ...
                            'simulated', simulated, ...
                            'rms', sqrt(mean(err.^2, 'omitnan')), 'maxError', max(abs(err)));
        fprintf('joint %d: simulated vs measured %.2f deg rms, %.2f deg at most\n', ...
            j, r(end).rms * 180/pi, r(end).maxError * 180/pi);
    end

    %% Plot
    if show
        figure('Name', 'Arm servos: simulated vs measured');
        for k = 1:numel(r)
            subplot(numel(r), 1, k);
            plot(r(k).t, r(k).goal * 180/pi, 'k:', ...
                 r(k).t, r(k).measured * 180/pi, 'b', ...
                 r(k).t, r(k).simulated * 180/pi, 'r--');
            ylabel(sprintf('Joint %d [deg]', r(k).joint));
            grid on
            if k == 1
                legend('Goal', 'Measured', 'Simulated', 'Location', 'best');
            end
        end
        xlabel('Time [s]');
    end
end
