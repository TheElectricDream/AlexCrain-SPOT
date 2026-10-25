function [setpoint] = defaultPlanner(state, goal, maxSpeed, maxTurnRate, horizon)
    %DEFAULTPLANNER Move toward the goal at a capped speed, then settle on it.
    %   setpoint = spot.guidance.defaultPlanner(state, goal, maxSpeed, maxTurnRate, horizon)
    %
    %   Carrot chasing on the straight line to the goal: the setpoint is placed a
    %   short distance ahead of the spacecraft (maxSpeed * horizon), with a desired
    %   velocity of maxSpeed toward the goal. Once the goal is within that distance,
    %   the setpoint is the goal itself, at rest. Attitude is handled the same way.
    %   This keeps the controller's error small, so it cruises instead of saturating.
    %
    %   Inputs
    %     state         Spacecraft state bus
    %                     state.pose  position and attitude [m; m; rad]
    %                     state.rate  linear and angular velocity [m/s; m/s; rad/s]
    %     goal          Goal pose [m; m; rad]
    %     maxSpeed      Cruise speed [m/s]
    %     maxTurnRate   Cruise turn rate [rad/s]
    %     horizon       How far ahead the setpoint is placed, as time at cruise [s]
    %
    %   Outputs
    %     setpoint      Target state bus
    %                     setpoint.pose  [m; m; rad]
    %                     setpoint.rate  [m/s; m/s; rad/s]
    %
    %   Method: carrot chasing (Sujit, Saripalli & Sousa, "An Evaluation of UAV
    %   Path Following Algorithms", ECC 2013), on the line of sight to the goal.
    %
    %   See also spot.control.defaultController.
    
    %#codegen

    arguments (Input)
            state           (1,1) struct
            goal            (3,1) double
            maxSpeed        (1,1) double {mustBePositive}
            maxTurnRate     (1,1) double {mustBePositive}
            horizon         (1,1) double {mustBePositive}
    end

    % Position: how far the goal is, and how far ahead the carrot sits
    toGoal   = goal(1:2) - state.pose(1:2);       % [m; m]
    distance = norm(toGoal);                      % [m]
    reach    = maxSpeed * horizon;                % [m]
    if distance > reach
        % Far away: carrot a fixed distance ahead, cruising toward the goal
        direction    = toGoal / distance;
        poseXY       = state.pose(1:2) + reach * direction;
        rateXY       = maxSpeed * direction;
    else
        % Close: the goal itself, at rest
        poseXY       = goal(1:2);
        rateXY       = [0; 0];
    end

    % Attitude: the same idea, turning the short way around
    toGoalAngle = mod(goal(3) - state.pose(3) + pi, 2*pi) - pi;   % [rad]
    turnReach   = maxTurnRate * horizon;                          % [rad]
    if abs(toGoalAngle) > turnReach
        poseTheta = state.pose(3) + sign(toGoalAngle) * turnReach;
        rateTheta = sign(toGoalAngle) * maxTurnRate;
    else
        poseTheta = goal(3);
        rateTheta = 0;
    end

    % Assemble the setpoint
    setpoint.pose = [poseXY; poseTheta];          % [m; m; rad]
    setpoint.rate = [rateXY; rateTheta];          % [m/s; m/s; rad/s]
end