function packet = logPacket(bus)
    %LOGPACKET The diagram's bus as the flat list of signals the SPOT Console reads.
    %   packet = diagram.buses.logPacket(bus)
    %
    %   The console reads a simulation's logged data (simOut.dataPacket) by signal name,
    %   e.g. RED_Px_m, the names the v5 template logs. This copies each field of the bus
    %   to its logged name. Save the output with a To Workspace block named dataPacket
    %   (Save format: Timeseries).
    %
    %   Inputs
    %     bus       the diagram's bus (Bus: SpotBus), as it leaves the mixers
    %
    %   Outputs
    %     packet    one field per logged signal (Bus: LogPacket):
    %                 Time_s, Phase, and for each of RED, BLACK and BLUE:
    %                 _Fx_N .. _Tz_Nm            commanded force and torque (controls)
    %                 _Fx_Sat_N .. _Tz_Sat_Nm    force and torque the thrusters apply (forces)
    %                 _Px_m .. _Rz_rad           pose
    %                 _Vx_mpers .. _RzD_radpers  rate
    %                 _Px_Desired_m ..           goal from the phase manager
    %                 _Px_Path_m ..              setpoint from the path planner
    %                 _Duty_Cycle                the 8 thrusters' duty cycles
    %                 _Controller_Choice         0 off, 1 default LQR, 2 custom
    %                 _Float_State               1 floating, 0 resting
    %
    %   The fields are set in the order diagram.buses.define lists them, which is the
    %   order Simulink expects.
    %
    %   See also diagram.buses.define, spot.sim.results.

    %#codegen

    arguments (Input)
        bus     (1,1) struct
    end

    packet.Time_s = bus.time;       % [s]
    packet.Phase  = bus.phase;

    % RED
    packet.RED_Fx_N              = bus.RED.controls(1);        % [N]
    packet.RED_Fy_N              = bus.RED.controls(2);        % [N]
    packet.RED_Tz_Nm             = bus.RED.controls(3);        % [N m]
    packet.RED_Fx_Sat_N          = bus.RED.satForces(1);          % [N]
    packet.RED_Fy_Sat_N          = bus.RED.satForces(2);          % [N]
    packet.RED_Tz_Sat_Nm         = bus.RED.satForces(3);          % [N m]
    packet.RED_Px_m              = bus.RED.state.pose(1);      % [m]
    packet.RED_Py_m              = bus.RED.state.pose(2);      % [m]
    packet.RED_Rz_rad            = bus.RED.state.pose(3);      % [rad]
    packet.RED_Vx_mpers          = bus.RED.state.rate(1);      % [m/s]
    packet.RED_Vy_mpers          = bus.RED.state.rate(2);      % [m/s]
    packet.RED_RzD_radpers       = bus.RED.state.rate(3);      % [rad/s]
    packet.RED_Px_Desired_m      = bus.RED.command.goal(1);    % [m]
    packet.RED_Py_Desired_m      = bus.RED.command.goal(2);    % [m]
    packet.RED_Rz_Desired_rad    = bus.RED.command.goal(3);    % [rad]
    packet.RED_Px_Path_m         = bus.RED.setpoint.pose(1);   % [m]
    packet.RED_Py_Path_m         = bus.RED.setpoint.pose(2);   % [m]
    packet.RED_Rz_Path_rad       = bus.RED.setpoint.pose(3);   % [rad]
    packet.RED_Duty_Cycle        = bus.RED.duty;               % 0 to 1
    packet.RED_Controller_Choice = bus.RED.command.controllerChoice;
    packet.RED_Float_State       = double(bus.RED.command.floating);

    % BLACK
    packet.BLACK_Fx_N              = bus.BLACK.controls(1);
    packet.BLACK_Fy_N              = bus.BLACK.controls(2);
    packet.BLACK_Tz_Nm             = bus.BLACK.controls(3);
    packet.BLACK_Fx_Sat_N          = bus.BLACK.satForces(1);
    packet.BLACK_Fy_Sat_N          = bus.BLACK.satForces(2);
    packet.BLACK_Tz_Sat_Nm         = bus.BLACK.satForces(3);
    packet.BLACK_Px_m              = bus.BLACK.state.pose(1);
    packet.BLACK_Py_m              = bus.BLACK.state.pose(2);
    packet.BLACK_Rz_rad            = bus.BLACK.state.pose(3);
    packet.BLACK_Vx_mpers          = bus.BLACK.state.rate(1);
    packet.BLACK_Vy_mpers          = bus.BLACK.state.rate(2);
    packet.BLACK_RzD_radpers       = bus.BLACK.state.rate(3);
    packet.BLACK_Px_Desired_m      = bus.BLACK.command.goal(1);
    packet.BLACK_Py_Desired_m      = bus.BLACK.command.goal(2);
    packet.BLACK_Rz_Desired_rad    = bus.BLACK.command.goal(3);
    packet.BLACK_Px_Path_m         = bus.BLACK.setpoint.pose(1);
    packet.BLACK_Py_Path_m         = bus.BLACK.setpoint.pose(2);
    packet.BLACK_Rz_Path_rad       = bus.BLACK.setpoint.pose(3);
    packet.BLACK_Duty_Cycle        = bus.BLACK.duty;
    packet.BLACK_Controller_Choice = bus.BLACK.command.controllerChoice;
    packet.BLACK_Float_State       = double(bus.BLACK.command.floating);

    % BLUE
    packet.BLUE_Fx_N              = bus.BLUE.controls(1);
    packet.BLUE_Fy_N              = bus.BLUE.controls(2);
    packet.BLUE_Tz_Nm             = bus.BLUE.controls(3);
    packet.BLUE_Fx_Sat_N          = bus.BLUE.satForces(1);
    packet.BLUE_Fy_Sat_N          = bus.BLUE.satForces(2);
    packet.BLUE_Tz_Sat_Nm         = bus.BLUE.satForces(3);
    packet.BLUE_Px_m              = bus.BLUE.state.pose(1);
    packet.BLUE_Py_m              = bus.BLUE.state.pose(2);
    packet.BLUE_Rz_rad            = bus.BLUE.state.pose(3);
    packet.BLUE_Vx_mpers          = bus.BLUE.state.rate(1);
    packet.BLUE_Vy_mpers          = bus.BLUE.state.rate(2);
    packet.BLUE_RzD_radpers       = bus.BLUE.state.rate(3);
    packet.BLUE_Px_Desired_m      = bus.BLUE.command.goal(1);
    packet.BLUE_Py_Desired_m      = bus.BLUE.command.goal(2);
    packet.BLUE_Rz_Desired_rad    = bus.BLUE.command.goal(3);
    packet.BLUE_Px_Path_m         = bus.BLUE.setpoint.pose(1);
    packet.BLUE_Py_Path_m         = bus.BLUE.setpoint.pose(2);
    packet.BLUE_Rz_Path_rad       = bus.BLUE.setpoint.pose(3);
    packet.BLUE_Duty_Cycle        = bus.BLUE.duty;
    packet.BLUE_Controller_Choice = bus.BLUE.command.controllerChoice;
    packet.BLUE_Float_State       = double(bus.BLUE.command.floating);
end
