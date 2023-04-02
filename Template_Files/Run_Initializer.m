% The following script is the initializer for SPOT 3.0; in this script,
% users define all initials parameters and/or constants required for
% simulation and experiment.

clear;
clc;
close all force;
addpath(genpath('../../Custom_Library'))

warning('off','all')

%% User-defined constants:

% Converting from degrees to radians and vis versa:

d2r                            = pi/180;
r2d                            = 180/pi;

% Initialize the table size for use in the GUI (don't delete):

xLength                        = 3.51155;   % [m]
yLength                        = 2.41935;   % [m]

% Initialize the PID gains for the RED platform:

Kp_xr                          = 2;
Kd_xr                          = 8;

Kp_yr                          = 2;
Kd_yr                          = 8;

Kp_tr                          = 0.1;
Kd_tr                          = 1.0;

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

% Initialize the PID gains for the ARM:

Kp_sharm                       = 200;
Kd_sharm                       = 50;

Kp_elarm                       = 200;
Kd_elarm                       = 20;

Kp_wrarm                       = 200;
Kd_wrarm                       = 10;

% Set torque limits on joints

Tz_lim_sharm                   = .1; % Shoulder Joint [Nm]

Tz_lim_elarm                   = .1; % Elbow Joint [Nm]

Tz_lim_wrarm                   = .1; % Wrist Joint [Nm]

% Transpose Jacobian controller gains:

Kp = [0.08 0 0
      0    0.08 0
      0    0    0.002];
Kv = [0.05 0 0
      0    0.05 0
      0    0    0.005];     

% Set the noise variance level for the RED and BLACK platforms:

noise_variance_RED             = 0;
noise_variance_BLACK           = 0;
noise_variance_BLUE            = 0;

%% Set the base sampling rate: 

% This variable will change the frequency at which the template runs. If
% the frequency of the template changes, the frequency of the server must
% also be changed, i.e. open the StreamData.sln under the PhaseSpace Server
% folder, and change line 204 from owl.frequency(10) to 
% owl.frequency(serverRate).:

baseRate                       = 0.05;      % 20 Hz

%% Set the duration of each major phase in the experiment, in seconds:

Phase0_Duration                = 10;        % [s]
Phase1_Duration                = 5;         % [s]
Phase2_Duration                = 20;        % [s]
Phase3_Duration                = 120;        % [s]
Phase4_Duration                = 120;        % [s]
Phase5_Duration                = 20;         % [s]

% Set the duration of the sub-phases. Sub-phases occur during the
% experiment phase (Phase3_Duration) and must be manually inserted into the
% diagram. The total duration of the sub-phases must equal the length of
% the Phase3_Duration.

Phase3_SubPhase1_Duration      = 120;        % [s]
Phase3_SubPhase2_Duration      = 00;          % [s]
Phase3_SubPhase3_Duration      = 0;        % [s]
Phase3_SubPhase4_Duration      = 0;        % [s]

% Determine the total experiment time from the durations:

tsim                           = Phase0_Duration + Phase1_Duration + ...
                                 Phase2_Duration + Phase3_Duration + ...
                                 Phase4_Duration + Phase5_Duration;        

% Determine the start time of each phase based on the duration:

Phase0_End                     = Phase0_Duration;
Phase1_End                     = Phase0_Duration + Phase1_Duration;           
Phase2_End                     = Phase0_Duration + Phase1_Duration + ...
                                 Phase2_Duration;         
Phase3_End                     = Phase0_Duration + Phase1_Duration + ...
                                 Phase2_Duration + Phase3_Duration;      
Phase4_End                     = Phase0_Duration + Phase1_Duration + ...
                                 Phase2_Duration + Phase3_Duration + ...
                                 Phase4_Duration; 
Phase5_End                     = Phase0_Duration + Phase1_Duration + ...
                                 Phase2_Duration + Phase3_Duration + ...
                                 Phase4_Duration + Phase5_Duration;                              
                             
% Determine the start time of each sub-phase based on the duration:  

Phase3_SubPhase1_End           = Phase2_End + Phase3_SubPhase1_Duration;
Phase3_SubPhase2_End           = Phase2_End + Phase3_SubPhase1_Duration + ...
                                 Phase3_SubPhase2_Duration;
Phase3_SubPhase3_End           = Phase2_End + Phase3_SubPhase1_Duration + ...
                                 Phase3_SubPhase2_Duration +...
                                 Phase3_SubPhase3_Duration;
Phase3_SubPhase4_End           = Phase2_End + Phase3_SubPhase1_Duration + ...
                                 Phase3_SubPhase2_Duration +...
                                 Phase3_SubPhase3_Duration +...
                                 Phase3_SubPhase4_Duration;     
                             
%% Load in any required data:

% Define the model properties for the joint friction:
% Based on https://ieeexplore.ieee.org/document/1511048

Gamma1_sh = 0.005;
Gamma2_sh = 15;
Gamma3_sh = 40;
Gamma4_sh = 0.015;
Gamma5_sh = 800;
Gamma6_sh = 0.005;

Gamma1_el = 0.12;
Gamma2_el = 5;
Gamma3_el = 10;
Gamma4_el = 0.039;
Gamma5_el = 800;
Gamma6_el = 0.000001;

Gamma1_wr = 0.025;
Gamma2_wr = 5;
Gamma3_wr = 40;
Gamma4_wr = 0.029;
Gamma5_wr = 800;
Gamma6_wr = 0.02;

% Define the mass properties for the RED, BLACK, and BLUE platforms:

model_param(1)                 = 11.211000; % RED Mass
model_param(2)                 = 0.202150; % RED Inertia
model_param(3)                 = 12.039000; % BLACK Mass
model_param(4)                 = 0.225692; % BLACK Inertia
model_param(5)                 = 10.726000; % BLUE Mass
model_param(6)                 = 0.194438; % BLUE Inertia
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
F_red_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_red_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_RED          = [F_red_X_nominal; F_red_X_nominal;...
                            F_red_Y_nominal; F_red_Y_nominal; F_red_X_nominal; ...
                            F_red_X_nominal; F_red_Y_nominal; F_red_Y_nominal];
                                  

F_Black_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_Black_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_BLACK          = [F_Black_X_nominal; F_Black_X_nominal;...
                            F_Black_Y_nominal; F_Black_Y_nominal; F_Black_X_nominal; ...
                            F_Black_X_nominal; F_Black_Y_nominal; F_Black_Y_nominal];
thruster_count_threshold = 0.0;                                     % how much duty cycle is needed to count this thruster as on (to the point where it disrupts thrust to other thrusters)



F_Blue_X_nominal          = 0.2196;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_Blue_Y_nominal          = 0.2825;                                  % [N] nominal thrust with only 2 or less thrusters firing
F_thrusters_BLUE          = [F_Blue_X_nominal; F_Blue_X_nominal;...
                            F_Blue_Y_nominal; F_Blue_Y_nominal; F_Blue_X_nominal; ...
                            F_Blue_X_nominal; F_Blue_Y_nominal; F_Blue_Y_nominal];

thruster_dist2CG_RED          = [69.38;-56.62;50.01;-81.49;61.12;-68.38;78.99;-52.01];
thruster_dist2CG_BLACK        = [72.80;-65.20;61.75;-79.75;65.20;-65.20;74.25;-54.75];
thruster_dist2CG_BLUE         = [83.10;-54.90;71.50;-70.00;54.90;-75.50;64.50;-64.50];

%%  Set the drop, initial, and home positions for each platform:

drop_states_ARM           = [ pi/2, pi/2, 0];                      % [rad; rad; rad]
drop_states_RED           = [ xLength/2+0.5, yLength/2, pi];       % [m; m; rad]
drop_states_BLACK         = [ xLength/2, yLength/2, 0];            % [m; m; rad]
drop_states_BLUE          = [ xLength/2-0.5; yLength/2; 0];        % [m; m; rad]

init_states_RED           = [ xLength/2+0.9; yLength/2; pi];       % [m; m; rad]
init_states_BLACK         = [ xLength/2; yLength/2; 0];            % [m; m; rad]
init_states_BLUE          = [ xLength/2-0.9; yLength/2; 0];        % [m; m; rad]

home_states_RED           = [ xLength/2+0.9; yLength/2; pi];       % [m; m; rad]
home_states_BLACK         = [ xLength/2; yLength/2; 0];            % [m; m; rad]
home_states_BLUE          = [ xLength/2-0.9; yLength/2; 0];        % [m; m; rad]

                                              
%% Start the graphical user interface:

run('GUI_v3_08')
