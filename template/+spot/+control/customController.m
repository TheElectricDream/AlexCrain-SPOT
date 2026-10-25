function controls = customController(state, setpoint, context, params)
    %CUSTOMCONTROLLER User control law, slot 1. Selected when controllerChoice == 2.
    %   controls = spot.control.customController(state, setpoint, context, params)
    %
    %   Inputs
    %     state     this platform:  state.pose [x; y; theta],  state.rate [xdot; ydot; thetadot]
    %     setpoint  from the planner:  setpoint.pose,  setpoint.rate
    %     context   the diagram's whole bus (Bus: SpotBus, see spot.buses.define):
    %                 context.time                  [s]
    %                 context.phase                 current experiment phase, 0 to 6
    %                 context.RED / .BLACK / .BLUE  each with .state.pose, .state.rate,
    %                                               .command, .setpoint, ...
    %                 context.user                  signals you wire into the diagram yourself
    %     params    your own constants (gains, limits...), a struct you define in the workspace
    %
    %   Outputs
    %     controls    [Fx; Fy; Tz] in the table frame [N, N, N m]
    
    %#codegen

    controls = zeros(3, 1);   % replace with your control law

end