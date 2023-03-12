% The following script is the initializer for SPOT 3.0; in this script,
% users define all initials parameters and/or constants required for
% simulation and experiment.
clear;
clc;
fclose('all')

%% User-defined constants:

%% Set the base sampling rate: 

% This variable will change the frequency at which the template runs. If
% the frequency of the template changes, the frequency of the server must
% also be changed, i.e. open the StreamData.sln under the PhaseSpace Server
% folder, and change line 204 from owl.frequency(10) to 
% owl.frequency(serverRate).:

baseRate                       = 0.05;      % 20 Hz


tsim                           = 50;

% Converting from degrees to radians and vis versa:

d2r                            = pi/180;
r2d                            = 180/pi;

% Initialize the table size for use in the GUI (don't delete):

xLength                        = 3.51155;   % [m]
yLength                        = 2.41935;   % [m]

% Initialize the PID gains for the RED platform:

Kp_xr                          = 4;
Kd_xr                          = 20;

Kp_yr                          = 4;
Kd_yr                          = 20;

Kp_tr                          = 1;
Kd_tr                          = 4;

% Initialize the PID gains for the BLACK platform:

Kp_xb                          = 2;
Kd_xb                          = 8;

Kp_yb                          = 2;
Kd_yb                          = 8;

Kp_tb                          = 0.1;
Kd_tb                          = 1.0;

% Initialize the PID gains for the BLUE platform:

Kp_xblue                       = 2;
Kd_xblue                       = 8;

Kp_yblue                       = 2;
Kd_yblue                       = 8;

Kp_tblue                       = 0.1;
Kd_tblue                       = 1;


%% Set the base sampling rate: 

% This variable will change the frequency at which the template runs. If
% the frequency of the template changes, the frequency of the server must
% also be changed, i.e. open the StreamData.sln under the PhaseSpace Server
% folder, and change line 204 from owl.frequency(10) to 
% owl.frequency(serverRate).:

baseRate                       = 0.05;      % 20 Hz


tsim                           = 50;

% Define the mass properties for the RED, BLACK, and BLUE platforms:

model_param(1)                 = 13.0720;     % RED Mass
model_param(2)                 = 0.2405;      % RED Inertia;
model_param(3)                 = 12.0390;     % BLACK Mass
model_param(4)                 = 0.225692;    % BLACK Inertia
model_param(5)                 = 11.2670;     % BLUE Mass
model_param(6)                 = 0.204245;    % BLUE Inertia
model_param(7)                 = 73.2749*d2r; % ARM Phi
model_param(8)                 = 0.25;        % ARM b0
model_param(9)                 = 0.196822;    % ARM a1
model_param(10)                = 0.107678;    % ARM b1
model_param(11)                = 0.198152;    % ARM a2
model_param(12)                = 0.106348;    % ARM b2
model_param(13)                = 0.062097;    % ARM a3
model_param(14)                = 0.025153;    % ARM b3
model_param(15)                = 0.345;       % ARM m1
model_param(16)                = 0.335;       % ARM m2
model_param(17)                = 0.111;       % ARM m3
model_param(18)                = 0.003704;    % ARM I1
model_param(19)                = 0.003506;    % ARM I2
model_param(20)                = 0.000106;    % ARM I3
 
% Initialize the thruster positions for the RED, BLACK, and BLUE platforms,
% as well as the expected maximum forces. The expected forces will only 
% affect the simulations.

% Currently RED has the most accurate estimate
F_RED_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_RED_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_RED          = [F_RED_X_nominal; F_RED_X_nominal;...
                            F_RED_Y_nominal; F_RED_Y_nominal; F_RED_X_nominal; ...
                            F_RED_X_nominal; F_RED_Y_nominal; F_RED_Y_nominal];
                                  

F_BLACK_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_BLACK_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_BLACK          = [F_BLACK_X_nominal; F_BLACK_X_nominal;...
                            F_BLACK_Y_nominal; F_BLACK_Y_nominal; F_BLACK_X_nominal; ...
                            F_BLACK_X_nominal; F_BLACK_Y_nominal; F_BLACK_Y_nominal];
thruster_count_threshold = 0.0;                                     % how much duty cycle is needed to count this thruster as on (to the point where it disrupts thrust to other thrusters)



F_Blue_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_Blue_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_BLUE          = [F_Blue_X_nominal; F_Blue_X_nominal;...
                            F_Blue_Y_nominal; F_Blue_Y_nominal; F_Blue_X_nominal; ...
                            F_Blue_X_nominal; F_Blue_Y_nominal; F_Blue_Y_nominal];

thruster_dist2CG_RED          = [49.92;-78.08;70.46;-63.54;81.08;-50.42;57.44;-75.96];
thruster_dist2CG_BLACK        = [83.42;-52.58;55.94;-60.05;54.08;-53.92;77.06;-55.94];
thruster_dist2CG_BLUE         = [73.54;-64.46;81.39;-60.11;64.46;-65.94;54.61;-74.39];


drop_states_RED           = [ xLength, yLength, pi];       % [m; m; rad]
drop_states_BLACK         = [ xLength, yLength, pi];            % [m; m; rad]
drop_states_BLUE          = [ xLength/2, yLength/2, pi];            % [m; m; rad]

init_states_RED           = [ 0; 0; pi/2];       % [m; m; rad]
init_states_BLACK         = [ 0; 0; pi/2];            % [m; m; rad]
init_states_BLUE          = [ xLength/2; yLength/2; pi];            % [m; m; rad]

home_states_RED           = [ xLength/2+0.9; yLength/2; pi];       % [m; m; rad]
home_states_BLACK         = [ xLength/2; yLength/2; 0];            % [m; m; rad]
home_states_BLUE          = [ xLength/2; yLength/2; 0];            % [m; m; rad]
 
results = sim('PlatformPIDTuner.slx');

figure()
plot(results.tout,results.simout(:,1));
hold on
plot(results.tout,results.simout(:,2));

figure()
plot(results.tout,results.simout(:,3));

figure()
plot(results.tout,results.simout1(:,1));
hold on
plot(results.tout,results.simout1(:,2));

figure()
plot(results.tout,results.simout1(:,3));

