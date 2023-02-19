clear;
clc;

Torque_Constant = 1.46; % Nm/A 
Rotor_Inertia = 0.007;
Gear_Ratio = 1; %200:1
Fv = 0.002;
Fc = 0.001;

A = [0 1; 0 -(Fv + Fc/Gear_Ratio^2)/(Rotor_Inertia*Gear_Ratio^2)];
B = [0; Torque_Constant/(Rotor_Inertia*Gear_Ratio^2)];
C = [1 0];
D = 0;

sys = ss(A,B,C,D);
G = tf(sys);

K = place(sys.A, sys.B,[-0.5, -0.6]);
L = feedback(sys,K);

step(L)

% s2 = tf('s')
% G2 = 1/(s2+1)
% step(G2)

% s = tf('s');
% 
% Kp = 500;
% Ki = 100;
% 
% C = Kp + Ki/s;
% 
% sys_cl = G*C;
% 
% step(sys_cl);