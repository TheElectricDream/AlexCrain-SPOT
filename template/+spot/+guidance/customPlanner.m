function setpoint = customPlanner(state, goal, context, params)
    %CUSTOMPLANNER User path planner: turns the goal into the setpoint the controller tracks.
    %   setpoint = spot.guidance.customPlanner(state, goal, context, params)
    %
    %   Called from a planner block's callPlanner in place of spot.guidance.defaultPlanner.
    %   The goal is where the phase manager wants the platform to end up; the setpoint is
    %   where the controller should try to put it right now.
    %
    %   Inputs
    %     state     this platform:  state.pose [x; y; theta],  state.rate [xdot; ydot; thetadot]
    %     goal      goal pose from the phase manager (command.goal) [x; y; theta]
    %     context   the diagram's whole bus (Bus: SpotBus, see spot.buses.define):
    %                 context.time                  [s]
    %                 context.phase                 current experiment phase, 0 to 6
    %                 context.RED / .BLACK / .BLUE  each with .state.pose, .state.rate
    %                                               and .command (goal, controllerChoice,
    %                                               floating)
    %                 context.user                  signals you wire into the diagram yourself
    %               The planners run before the controllers and mixers, so .setpoint,
    %               .controls, .duty and .forces are still zero here.
    %     params    your own constants (speeds, distances...), a struct you define in the
    %               workspace
    %
    %   Outputs
    %     setpoint  target state for the controller (Bus: Setpoint):
    %                 setpoint.pose  [x; y; theta]           [m; m; rad]
    %                 setpoint.rate  [xdot; ydot; thetadot]  [m/s; m/s; rad/s]
    %
    %   See also spot.guidance.defaultPlanner, spot.control.customController.

    %#codegen

    % Replace with your planner. As it is: the goal itself, at rest (what the default
    % planner gives once the platform is close to the goal)
    setpoint.pose = goal;               % [m; m; rad]
    setpoint.rate = zeros(3, 1);        % [m/s; m/s; rad/s]

end
