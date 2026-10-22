function setpoint = directPlanner(goal, previousGoal, baseRate)
    %DIRECTPLANNER No path planning: the goal is the setpoint, moving as the goal moves.
    %   setpoint = diagram.planners.directPlanner(goal, previousGoal, baseRate)
    %
    %   The setpoint pose is the goal itself, and the setpoint rate is how fast the goal
    %   moved since the previous step, so the controller follows a moving goal (a circle,
    %   another platform) instead of lagging behind it. This is the v5 template's
    %   "Disable PPL" with the Default LQR's derivative of the path. The memory is outside
    %   this function: feed goal back as previousGoal through a Unit Delay (initial
    %   condition: the platform's initial pose).
    %
    %   When the goal jumps (at a phase change), the rate is a large one-step spike, as it
    %   is in v5. Unlike v5, the attitude change is wrapped to [-pi, pi], so a goal
    %   attitude crossing +-pi doesn't look like a fast spin.
    %
    %   Inputs
    %     goal          Goal pose from the phase manager this step [m; m; rad]
    %     previousGoal  Goal pose on the previous step [m; m; rad]
    %     baseRate      Diagram's time step [s]
    %
    %   Outputs
    %     setpoint      Target state for the controller (Bus: Setpoint)
    %                     setpoint.pose  [m; m; rad]
    %                     setpoint.rate  [m/s; m/s; rad/s]
    %
    %   See also diagram.planners.defaultPlanner, diagram.planners.customPlanner.

    %#codegen

    arguments (Input)
        goal            (3,1) double
        previousGoal    (3,1) double
        baseRate        (1,1) double {mustBePositive}
    end

    % How far the goal moved this step, the attitude the short way around
    change    = goal - previousGoal;                        % [m; m; rad]
    change(3) = mod(change(3) + pi, 2*pi) - pi;             % [rad]

    setpoint.pose = goal;                                   % [m; m; rad]
    setpoint.rate = change / baseRate;                      % [m/s; m/s; rad/s]
end
