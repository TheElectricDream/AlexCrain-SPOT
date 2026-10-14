function r = compareSteps(folder, armModel, show)
%COMPARESTEPS The simulated servos against the real ones, on the calibration's position steps.
%   r = spot.arm.compareSteps(folder, armModel) replays each steps_j<N>.csv in folder (a run of
%   resources/arm_calibration/calibrate_arm.py: position-mode goal steps with SPOT's servo
%   settings, base fixed) with the same goals through the servo's own position control law
%   (spot.arm.PositionControl at the servo's period, armModel.servo's gains, back-EMF) driving
%   spot.arm.Plant, and compares the joint's motion. A match checks both the plant (motor,
%   friction, inertia) and the controller replica that the diagram's Default Position Control
%   (ARM) runs, retuned, at 20 Hz. armModel is the model to try (e.g. evalin('base', 'armModel') after a simulation, or
%   spot.arm.loadCalibration(fullfile(folder, 'arm_calibration.json'), armModel)).
%   r(k) has joint, t, goal, measured, simulated [rad] and rms, maxError [rad].
%   r = spot.arm.compareSteps(folder, armModel, true) also plots them.
if nargin < 3
    show = nargout == 0;
end
files = dir(fullfile(folder, 'steps_j*.csv'));
if isempty(files)
    error('spot:arm:compareSteps', 'No steps_j*.csv in %s.', folder);
end
dt = 0.01;   % the goals change every 10 ms (the logs are faster and uneven)
r = struct('joint', {}, 't', {}, 'goal', {}, 'measured', {}, 'simulated', {}, 'rms', {}, 'maxError', {});
for f = files.'
    T = readtable(fullfile(f.folder, f.name));
    j = T.joint(1);
    qCols = startsWith(T.Properties.VariableNames, 'q');
    Q = T{:, qCols};
    t = T.t - T.t(1);
    tt = (0:dt:t(end)).';
    goal = interp1(t, T.goal, tt, 'previous', 'extrap');
    a = armModel;
    sv = a.servo;
    a.outerStep = sv.period;
    a.x0 = [0; 0; 0; Q(1, :).'];
    a.servo.currentLimit = 1941;        % position mode isn't held to the Current Limit (only to the PWM Limit)
    p = spot.arm.Plant('Model', a, 'FixedBase', true);
    c = spot.arm.PositionControl('Motor', a.motor, 'StepTime', sv.period, 'ServoPeriod', sv.period, ...
        'PositionP', sv.positionP, 'PositionI', sv.positionI, 'PositionD', sv.positionD, ...
        'Feedforward1', sv.feedforward1, 'Feedforward2', sv.feedforward2, 'ProfileVelocity', sv.profileVelocity, ...
        'ProfileAcceleration', sv.profileAcceleration, 'MinPosition', sv.minPosition, 'MaxPosition', sv.maxPosition, ...
        'PWMLimit', sv.pwmLimit, 'BackEMF', true);
    scale = (4096 / (2*pi)) / 651.74;   % the script sends true pulses; the controller converts as SPOT's code did
    held = Q(1, :).' * scale;
    sim = zeros(numel(tt), 1);
    per = round(dt / sv.period);
    meas = [Q(1, :).'; 0; 0; 0];
    for k = 1:numel(tt)
        cmd = held;
        cmd(j) = goal(k) * scale;
        for i = 1:per
            [pos, vel] = p([0; 0; 0], c(cmd, meas));
            meas = [pos(4:6); vel(4:6)];
        end
        sim(k) = pos(3 + j);
    end
    meas = interp1(t, Q(:, j), tt);
    err = sim - meas;
    r(end + 1) = struct('joint', j, 't', tt, 'goal', goal, 'measured', meas, 'simulated', sim, ...
                        'rms', sqrt(mean(err.^2, 'omitnan')), 'maxError', max(abs(err)));
    fprintf('joint %d: simulated vs measured %.2f deg rms, %.2f deg at most\n', j, r(end).rms * 180/pi, r(end).maxError * 180/pi);
end
if show
    figure('Name', 'Arm servos: simulated vs measured');
    for k = 1:numel(r)
        subplot(numel(r), 1, k);
        plot(r(k).t, r(k).goal * 180/pi, 'k:', r(k).t, r(k).measured * 180/pi, 'b', r(k).t, r(k).simulated * 180/pi, 'r--');
        ylabel(sprintf('Joint %d [deg]', r(k).joint));
        grid on
        if k == 1, legend('Goal', 'Measured', 'Simulated', 'Location', 'best'); end
    end
    xlabel('Time [s]');
end
end
