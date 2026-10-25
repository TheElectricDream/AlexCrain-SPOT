function define()
    %DEFINE Create the bus types (struct layouts) used by the diagram.
    %   spot.buses.define()
    %
    %   A bus type is a named description of a struct: which fields it has and how many
    %   numbers each field holds. It contains no data. Simulink needs one whenever a bus
    %   enters a MATLAB Function block, so the block knows to hand the bus to the code as
    %   a struct (state.pose) instead of a flat vector of numbers.
    %
    %   The diagram passes one bus, SpotBus, from block to block. It starts as all zeros
    %   at the plants, and each stage fills in its own fields with a Bus Assignment block
    %   and reads what it needs with a Bus Selector:
    %
    %     Plants          time,   RED/BLACK/BLUE.state
    %     Phase Manager   phase,  RED/BLACK/BLUE.command
    %     Paths                   RED/BLACK/BLUE.setpoint
    %     Controllers             RED/BLACK/BLUE.controls
    %     Mixers                  RED/BLACK/BLUE.duty and .forces
    %
    %   A Bus Assignment block can only fill a field the bus already has, which is why
    %   every field is listed here from the start.
    %
    %   This creates seven bus types in the base workspace:
    %     PlatformState   .pose  .rate
    %     Setpoint        .pose  .rate
    %     PhaseCommand    .controllerChoice  .goal  .floating
    %     Platform        .state  .command  .setpoint  .controls  .duty  .forces
    %     UserData        .placeholder
    %     SpotBus         .time  .phase  .RED  .BLACK  .BLUE  .user
    %     LogPacket       .Time_s  .Phase  .RED_Px_m  ...  (the logged signals, one field each;
    %                     filled by spot.buses.logPacket)
    %
    %   Run it before simulating (the diagram's InitFcn does). In a MATLAB Function block,
    %   select a type in the Symbols pane as 'Bus: SpotBus' (etc.).
    %
    %   See also Simulink.Bus, Simulink.BusElement.

    % PlatformState: the state of one spacecraft
    % Field 'pose': position and attitude [m; m; rad]
    pose = Simulink.BusElement;
    pose.Name       = 'pose';
    pose.Dimensions = 3;

    % Field 'rate': linear and angular velocity [m/s; m/s; rad/s]
    rate = Simulink.BusElement;
    rate.Name       = 'rate';
    rate.Dimensions = 3;

    % Put the two fields together into a type called PlatformState
    PlatformState = Simulink.Bus;
    PlatformState.Elements = [pose rate];

    % Setpoint: the target state from the path planner
    % Same two fields as PlatformState, under its own name so the diagram says
    % which is which
    Setpoint = Simulink.Bus;
    Setpoint.Elements = [pose rate];

    % PhaseCommand: what the phase manager tells one spacecraft to do
    % Field 'controllerChoice': 0 thrusters off, 1 default LQR, 2 custom
    choiceController = Simulink.BusElement;
    choiceController.Name       = 'controllerChoice';
    choiceController.Dimensions = 1;

    % PhaseCommand: what the phase manager tells one spacecraft to do
    % Field 'plannerChoice': 0 thrusters off, 1 default, 2 custom
    choicePlanner = Simulink.BusElement;
    choicePlanner.Name       = 'plannerChoice';
    choicePlanner.Dimensions = 1;

    % Field 'goal': goal pose for the path planner [m; m; rad]
    goal = Simulink.BusElement;
    goal.Name       = 'goal';
    goal.Dimensions = 3;

    % Field 'floating': true to float on the air bearings
    floating = Simulink.BusElement;
    floating.Name     = 'floating';
    floating.DataType = 'boolean';

    PhaseCommand = Simulink.Bus;
    PhaseCommand.Elements = [choiceController goal floating choicePlanner];

    % Platform: everything about one spacecraft, filled in stage by stage
    % Fields 'state', 'command', 'setpoint': each one is a whole bus. Setting the
    % DataType to 'Bus: PlatformState' is what nests the struct, giving bus.RED.state.pose
    state = Simulink.BusElement;
    state.Name     = 'state';
    state.DataType = 'Bus: PlatformState';

    command = Simulink.BusElement;
    command.Name     = 'command';
    command.DataType = 'Bus: PhaseCommand';

    setpoint = Simulink.BusElement;
    setpoint.Name     = 'setpoint';
    setpoint.DataType = 'Bus: Setpoint';

    % Field 'controls': commanded force and torque from the controller [N; N; N m]
    controls = Simulink.BusElement;
    controls.Name       = 'controls';
    controls.Dimensions = 3;

    % Field 'duty': duty cycle of each of the 8 thrusters from the mixer, 0 to 1
    duty = Simulink.BusElement;
    duty.Name       = 'duty';
    duty.Dimensions = 8;

    % Field 'forces': force and torque the thrusters apply [N; N; N m]
    forces = Simulink.BusElement;
    forces.Name       = 'forces';
    forces.Dimensions = 3;

    % Field 'satForces': sat. force and torque the thrusters apply [N; N; N m]
    satForces = Simulink.BusElement;
    satForces.Name       = 'satForces';
    satForces.Dimensions = 3;

    Platform = Simulink.Bus;
    Platform.Elements = [state command setpoint controls duty forces satForces];

    % UserData: signals users wire in for their own algorithms
    % A bus cannot be empty, so it starts with one unused field. To add a signal:
    % copy this block, change the name and size, add it to the list below, and fill
    % it in the diagram with a Bus Assignment block (element user.<name>).
    placeholder = Simulink.BusElement;
    placeholder.Name       = 'placeholder';
    placeholder.Dimensions = 1;

    UserData = Simulink.Bus;
    UserData.Elements = placeholder;

    % SpotBus: the one bus that flows through the diagram
    % Field 'time': simulation or experiment time [s]
    time = Simulink.BusElement;
    time.Name       = 'time';
    time.Dimensions = 1;

    % Field 'phase': current experiment phase, 0 to 6
    phase = Simulink.BusElement;
    phase.Name       = 'phase';
    phase.Dimensions = 1;

    % Fields 'RED', 'BLACK', 'BLUE': each one is a whole Platform
    red = Simulink.BusElement;
    red.Name     = 'RED';
    red.DataType = 'Bus: Platform';

    black = Simulink.BusElement;
    black.Name     = 'BLACK';
    black.DataType = 'Bus: Platform';

    blue = Simulink.BusElement;
    blue.Name     = 'BLUE';
    blue.DataType = 'Bus: Platform';

    % Field 'user': the UserData struct, giving bus.user.<name>
    user = Simulink.BusElement;
    user.Name     = 'user';
    user.DataType = 'Bus: UserData';

    SpotBus = Simulink.Bus;
    SpotBus.Elements = [time phase red black blue user];

    % LogPacket: what is saved after a simulation (dataPacket), one flat field per signal
    % The SPOT Console reads the logged data by these names (interface.results.simulation), the same
    % names the v5 template logs, so both diagrams work with it. Each name is
    % <PLATFORM>_<quantity>_<unit>; the console turns it into a plot label.
    elements = [signal('Time_s', 1), ...      % [s]
                signal('Phase', 1)];          % 0 to 6
    for platform = {'RED', 'BLACK', 'BLUE'}
        p = platform{1};
        elements = [elements, ...
            signal([p '_Fx_N'], 1),           signal([p '_Fy_N'], 1), ...           % controls
            signal([p '_Tz_Nm'], 1), ...
            signal([p '_Fx_Sat_N'], 1),       signal([p '_Fy_Sat_N'], 1), ...       % forces
            signal([p '_Tz_Sat_Nm'], 1), ...
            signal([p '_Px_m'], 1),           signal([p '_Py_m'], 1), ...           % state.pose
            signal([p '_Rz_rad'], 1), ...
            signal([p '_Vx_mpers'], 1),       signal([p '_Vy_mpers'], 1), ...       % state.rate
            signal([p '_RzD_radpers'], 1), ...
            signal([p '_Px_Desired_m'], 1),   signal([p '_Py_Desired_m'], 1), ...   % command.goal
            signal([p '_Rz_Desired_rad'], 1), ...
            signal([p '_Px_Path_m'], 1),      signal([p '_Py_Path_m'], 1), ...      % setpoint.pose
            signal([p '_Rz_Path_rad'], 1), ...
            signal([p '_Duty_Cycle'], 8), ...                                       % duty
            signal([p '_Controller_Choice'], 1), ...                                % command
            signal([p '_Float_State'], 1)]; %#ok<AGROW>
    end

    LogPacket = Simulink.Bus;
    LogPacket.Elements = elements;

    % Make the types visible to Simulink
    % Simulink looks bus types up by name in the base workspace
    assignin('base', 'PlatformState', PlatformState);
    assignin('base', 'Setpoint',      Setpoint);
    assignin('base', 'PhaseCommand',  PhaseCommand);
    assignin('base', 'Platform',      Platform);
    assignin('base', 'UserData',      UserData);
    assignin('base', 'SpotBus',       SpotBus);
    assignin('base', 'LogPacket',     LogPacket);
end

function element = signal(name, dimensions)
    % One logged signal: a bus field of doubles
    element = Simulink.BusElement;
    element.Name       = name;
    element.Dimensions = dimensions;
end
