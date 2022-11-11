//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Parker_Velocity_Control_data.cpp
//
// Code generated for Simulink model 'Parker_Velocity_Control'.
//
// Model version                  : 1.645
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Wed Jul 13 11:50:24 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#include "Parker_Velocity_Control.h"
#include "Parker_Velocity_Control_private.h"

// Block parameters (default storage)
P_Parker_Velocity_Control_T Parker_Velocity_Control_P = {
  // Variable: F_Black_X_nominal
  //  Referenced by: '<S34>/MATLAB Function11'

  0.2196,

  // Variable: F_Black_Y_nominal
  //  Referenced by: '<S34>/MATLAB Function11'

  0.2825,

  // Variable: F_Blue_X_nominal
  //  Referenced by: '<S46>/MATLAB Function7'

  0.2196,

  // Variable: F_Blue_Y_nominal
  //  Referenced by: '<S46>/MATLAB Function7'

  0.2825,

  // Variable: F_red_X_nominal
  //  Referenced by:
  //    '<S58>/MATLAB Function4'
  //    '<S58>/MATLAB Function6'

  0.2196,

  // Variable: F_red_Y_nominal
  //  Referenced by:
  //    '<S58>/MATLAB Function4'
  //    '<S58>/MATLAB Function6'

  0.2825,

  // Variable: F_thrusters_BLACK
  //  Referenced by:
  //    '<S34>/MATLAB Function'
  //    '<S34>/MATLAB Function1'

  { 0.2196, 0.2196, 0.2825, 0.2825, 0.2196, 0.2196, 0.2825, 0.2825 },

  // Variable: F_thrusters_BLUE
  //  Referenced by:
  //    '<S46>/MATLAB Function'
  //    '<S46>/MATLAB Function1'

  { 0.2196, 0.2196, 0.2825, 0.2825, 0.2196, 0.2196, 0.2825, 0.2825 },

  // Variable: F_thrusters_RED
  //  Referenced by:
  //    '<S58>/MATLAB Function'
  //    '<S58>/MATLAB Function1'
  //    '<S58>/MATLAB Function3'

  { 0.2196, 0.2196, 0.2825, 0.2825, 0.2196, 0.2196, 0.2825, 0.2825 },

  // Variable: Gamma1
  //  Referenced by: '<S490>/Dynamics Model'

  0.0025,

  // Variable: Gamma2
  //  Referenced by: '<S490>/Dynamics Model'

  100.0,

  // Variable: Gamma3
  //  Referenced by: '<S490>/Dynamics Model'

  10.0,

  // Variable: Gamma4
  //  Referenced by: '<S490>/Dynamics Model'

  0.001,

  // Variable: Gamma5
  //  Referenced by: '<S490>/Dynamics Model'

  100.0,

  // Variable: Gamma6
  //  Referenced by: '<S490>/Dynamics Model'

  0.001,

  // Variable: Kd_tb
  //  Referenced by:
  //    '<S152>/kd_tb'
  //    '<S351>/kd_tb'
  //    '<S404>/kd_tb'
  //    '<S190>/kd_tb'
  //    '<S207>/kd_tb'
  //    '<S224>/kd_tb'
  //    '<S241>/kd_tb'

  0.4,

  // Variable: Kd_tblue
  //  Referenced by:
  //    '<S165>/kd_tb'
  //    '<S364>/kd_tb'
  //    '<S417>/kd_tb'
  //    '<S262>/kd_tb'
  //    '<S280>/kd_tb'
  //    '<S298>/kd_tb'
  //    '<S316>/kd_tb'

  0.4,

  // Variable: Kd_tr
  //  Referenced by:
  //    '<S377>/kd_tr'
  //    '<S430>/kd_tr'

  0.4,

  // Variable: Kd_xb
  //  Referenced by:
  //    '<S153>/kd_xb'
  //    '<S352>/kd_xb'
  //    '<S405>/kd_xb'
  //    '<S191>/kd_xb'
  //    '<S208>/kd_xb'
  //    '<S225>/kd_xb'
  //    '<S242>/kd_xb'

  8.0,

  // Variable: Kd_xblue
  //  Referenced by:
  //    '<S166>/kd_xb'
  //    '<S365>/kd_xb'
  //    '<S418>/kd_xb'
  //    '<S263>/kd_xb'
  //    '<S281>/kd_xb'
  //    '<S299>/kd_xb'
  //    '<S317>/kd_xb'

  8.0,

  // Variable: Kd_xr
  //  Referenced by:
  //    '<S378>/kd_xr'
  //    '<S431>/kd_xr'

  8.0,

  // Variable: Kd_yb
  //  Referenced by:
  //    '<S154>/kd_yb'
  //    '<S353>/kd_yb'
  //    '<S406>/kd_yb'
  //    '<S192>/kd_yb'
  //    '<S209>/kd_yb'
  //    '<S226>/kd_yb'
  //    '<S243>/kd_yb'

  8.0,

  // Variable: Kd_yblue
  //  Referenced by:
  //    '<S167>/kd_yb'
  //    '<S366>/kd_yb'
  //    '<S419>/kd_yb'
  //    '<S264>/kd_yb'
  //    '<S282>/kd_yb'
  //    '<S300>/kd_yb'
  //    '<S318>/kd_yb'

  8.0,

  // Variable: Kd_yr
  //  Referenced by:
  //    '<S379>/kd_yr'
  //    '<S432>/kd_yr'

  8.0,

  // Variable: Kp
  //  Referenced by:
  //    '<S393>/MATLAB Function'
  //    '<S446>/MATLAB Function'

  { 0.08, 0.0, 0.0, 0.0, 0.08, 0.0, 0.0, 0.0, 0.002 },

  // Variable: Kp_tb
  //  Referenced by:
  //    '<S152>/kp_tb'
  //    '<S351>/kp_tb'
  //    '<S404>/kp_tb'
  //    '<S190>/kp_tb'
  //    '<S207>/kp_tb'
  //    '<S224>/kp_tb'
  //    '<S241>/kp_tb'

  0.1,

  // Variable: Kp_tblue
  //  Referenced by:
  //    '<S165>/kp_tb'
  //    '<S364>/kp_tb'
  //    '<S417>/kp_tb'
  //    '<S262>/kp_tb'
  //    '<S280>/kp_tb'
  //    '<S298>/kp_tb'
  //    '<S316>/kp_tb'

  0.1,

  // Variable: Kp_tr
  //  Referenced by:
  //    '<S377>/kp_tr'
  //    '<S430>/kp_tr'

  0.1,

  // Variable: Kp_xb
  //  Referenced by:
  //    '<S153>/kp_xb'
  //    '<S352>/kp_xb'
  //    '<S405>/kp_xb'
  //    '<S191>/kp_xb'
  //    '<S208>/kp_xb'
  //    '<S225>/kp_xb'
  //    '<S242>/kp_xb'

  2.0,

  // Variable: Kp_xblue
  //  Referenced by:
  //    '<S166>/kp_xb'
  //    '<S365>/kp_xb'
  //    '<S418>/kp_xb'
  //    '<S263>/kp_xb'
  //    '<S281>/kp_xb'
  //    '<S299>/kp_xb'
  //    '<S317>/kp_xb'

  2.0,

  // Variable: Kp_xr
  //  Referenced by:
  //    '<S378>/kp_xr'
  //    '<S431>/kp_xr'

  2.0,

  // Variable: Kp_yb
  //  Referenced by:
  //    '<S154>/kp_yb'
  //    '<S353>/kp_yb'
  //    '<S406>/kp_yb'
  //    '<S192>/kp_yb'
  //    '<S209>/kp_yb'
  //    '<S226>/kp_yb'
  //    '<S243>/kp_yb'

  2.0,

  // Variable: Kp_yblue
  //  Referenced by:
  //    '<S167>/kp_yb'
  //    '<S366>/kp_yb'
  //    '<S419>/kp_yb'
  //    '<S264>/kp_yb'
  //    '<S282>/kp_yb'
  //    '<S300>/kp_yb'
  //    '<S318>/kp_yb'

  2.0,

  // Variable: Kp_yr
  //  Referenced by:
  //    '<S379>/kp_yr'
  //    '<S432>/kp_yr'

  2.0,

  // Variable: Kv
  //  Referenced by:
  //    '<S393>/MATLAB Function'
  //    '<S446>/MATLAB Function'

  { 0.05, 0.0, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0, 0.005 },

  // Variable: Phase0_End
  //  Referenced by: '<Root>/Constant4'

  10.0,

  // Variable: Phase1_End
  //  Referenced by: '<Root>/Constant'

  15.0,

  // Variable: Phase2_End
  //  Referenced by:
  //    '<Root>/Constant1'
  //    '<S269>/Constant2'
  //    '<S287>/Constant2'
  //    '<S305>/Constant2'
  //    '<S323>/Constant2'
  //    '<S194>/Constant1'
  //    '<S211>/Constant1'
  //    '<S228>/Constant1'
  //    '<S245>/Constant1'

  35.0,

  // Variable: Phase3_End
  //  Referenced by: '<Root>/Constant2'

  550.0,

  // Variable: Phase3_SubPhase1_End
  //  Referenced by:
  //    '<S180>/Constant4'
  //    '<S181>/Constant4'
  //    '<S182>/Constant4'

  155.0,

  // Variable: Phase3_SubPhase2_End
  //  Referenced by:
  //    '<S180>/Constant1'
  //    '<S181>/Constant1'
  //    '<S182>/Constant1'

  165.0,

  // Variable: Phase3_SubPhase3_End
  //  Referenced by:
  //    '<S180>/Constant2'
  //    '<S181>/Constant2'
  //    '<S182>/Constant2'

  195.0,

  // Variable: Phase3_SubPhase4_End
  //  Referenced by:
  //    '<S180>/Constant3'
  //    '<S181>/Constant3'
  //    '<S182>/Constant3'

  555.0,

  // Variable: Phase4_End
  //  Referenced by: '<Root>/Constant3'

  670.0,

  // Variable: Phase5_End
  //  Referenced by: '<Root>/Constant6'

  690.0,

  // Variable: Tz_lim_elarm
  //  Referenced by:
  //    '<S382>/Saturation1'
  //    '<S435>/Saturation1'

  0.1,

  // Variable: Tz_lim_sharm
  //  Referenced by:
  //    '<S382>/Saturation'
  //    '<S435>/Saturation'

  0.1,

  // Variable: Tz_lim_wrarm
  //  Referenced by:
  //    '<S382>/Saturation3'
  //    '<S435>/Saturation3'

  0.1,

  // Variable: baseRate
  //  Referenced by:
  //    '<S128>/Read Joint Positions using  Dynamixel Encoders'
  //    '<S98>/divide by delta T'
  //    '<S99>/divide by delta T'
  //    '<S100>/divide by delta T'
  //    '<S101>/divide by delta T'
  //    '<S102>/divide by delta T'
  //    '<S103>/divide by delta T'
  //    '<S104>/divide by delta T'
  //    '<S105>/divide by delta T'
  //    '<S106>/divide by delta T'
  //    '<S107>/divide by delta T'
  //    '<S108>/divide by delta T'
  //    '<S109>/divide by delta T'
  //    '<S110>/divide by delta T'
  //    '<S111>/divide by delta T'
  //    '<S112>/divide by delta T'
  //    '<S462>/divide by delta T'
  //    '<S463>/divide by delta T'
  //    '<S464>/divide by delta T'
  //    '<S465>/divide by delta T'
  //    '<S466>/divide by delta T'
  //    '<S474>/divide by delta T'
  //    '<S475>/divide by delta T'
  //    '<S476>/divide by delta T'
  //    '<S477>/divide by delta T'
  //    '<S478>/divide by delta T'
  //    '<S156>/divide by delta T'
  //    '<S158>/divide by delta T'
  //    '<S160>/divide by delta T'
  //    '<S169>/divide by delta T'
  //    '<S171>/divide by delta T'
  //    '<S173>/divide by delta T'
  //    '<S355>/divide by delta T'
  //    '<S357>/divide by delta T'
  //    '<S359>/divide by delta T'
  //    '<S368>/divide by delta T'
  //    '<S370>/divide by delta T'
  //    '<S372>/divide by delta T'
  //    '<S383>/divide by delta T'
  //    '<S385>/divide by delta T'
  //    '<S387>/divide by delta T'
  //    '<S408>/divide by delta T'
  //    '<S410>/divide by delta T'
  //    '<S412>/divide by delta T'
  //    '<S421>/divide by delta T'
  //    '<S423>/divide by delta T'
  //    '<S425>/divide by delta T'
  //    '<S436>/divide by delta T'
  //    '<S438>/divide by delta T'
  //    '<S440>/divide by delta T'
  //    '<S197>/divide by delta T'
  //    '<S199>/divide by delta T'
  //    '<S201>/divide by delta T'
  //    '<S214>/divide by delta T'
  //    '<S216>/divide by delta T'
  //    '<S218>/divide by delta T'
  //    '<S231>/divide by delta T'
  //    '<S233>/divide by delta T'
  //    '<S235>/divide by delta T'
  //    '<S248>/divide by delta T'
  //    '<S250>/divide by delta T'
  //    '<S252>/divide by delta T'
  //    '<S270>/divide by delta T'
  //    '<S272>/divide by delta T'
  //    '<S274>/divide by delta T'
  //    '<S288>/divide by delta T'
  //    '<S290>/divide by delta T'
  //    '<S292>/divide by delta T'
  //    '<S306>/divide by delta T'
  //    '<S308>/divide by delta T'
  //    '<S310>/divide by delta T'
  //    '<S324>/divide by delta T'
  //    '<S326>/divide by delta T'
  //    '<S328>/divide by delta T'
  //    '<S494>/divide by delta T'
  //    '<S495>/divide by delta T'
  //    '<S500>/divide by delta T'
  //    '<S501>/divide by delta T'
  //    '<S502>/divide by delta T'
  //    '<S503>/divide by delta T'
  //    '<S504>/divide by delta T'

  0.05,

  // Variable: drop_states_BLACK
  //  Referenced by: '<S461>/Velocity to Position'

  { 1.755775, 1.209675, 0.0 },

  // Variable: drop_states_BLUE
  //  Referenced by: '<S473>/Velocity to Position'

  { 1.255775, 1.209675, 0.0 },

  // Variable: drop_states_RED
  //  Referenced by: '<S499>/Velocity to Position'

  { 2.255775, 1.209675, 3.1415926535897931 },

  // Variable: home_states_BLACK
  //  Referenced by:
  //    '<S347>/Desired Attitude (BLACK)'
  //    '<S347>/Desired Px (BLACK)'
  //    '<S347>/Desired Py (BLACK)'
  //    '<S400>/Constant'
  //    '<S400>/Constant2'
  //    '<S400>/Constant3'

  { 1.755775, 1.209675, 0.0 },

  // Variable: home_states_BLUE
  //  Referenced by:
  //    '<S348>/Desired Attitude (BLUE)'
  //    '<S348>/Desired Px (BLUE)'
  //    '<S348>/Desired Py (BLUE)'
  //    '<S401>/Desired Attitude (BLUE)'
  //    '<S401>/Desired Px (BLUE)'
  //    '<S401>/Desired Py (BLUE)'

  { 0.85577500000000006, 1.209675, 0.0 },

  // Variable: home_states_RED
  //  Referenced by:
  //    '<S349>/Desired Px (BLACK)'
  //    '<S349>/Desired Px (BLACK)1'
  //    '<S349>/Desired Px (BLACK)2'
  //    '<S402>/Desired Px (BLACK)'
  //    '<S402>/Desired Px (BLACK)1'
  //    '<S402>/Desired Px (BLACK)2'

  { 2.655775, 1.209675, 3.1415926535897931 },

  // Variable: init_states_BLACK
  //  Referenced by:
  //    '<S148>/Desired Attitude (BLACK)'
  //    '<S148>/Desired Px (BLACK)'
  //    '<S148>/Desired Py (BLACK)'

  { 1.755775, 1.209675, 0.0 },

  // Variable: init_states_BLUE
  //  Referenced by:
  //    '<S149>/Desired Attitude (BLUE)'
  //    '<S149>/Desired Px (BLUE)'
  //    '<S149>/Desired Py (BLUE)'

  { 0.85577500000000006, 1.209675, 0.0 },

  // Variable: model_param
  //  Referenced by:
  //    '<S58>/MATLAB Function'
  //    '<S58>/MATLAB Function4'
  //    '<S58>/MATLAB Function6'
  //    '<S461>/MATLAB Function'
  //    '<S473>/MATLAB Function'
  //    '<S392>/End-Effector Position'
  //    '<S445>/End-Effector Position'
  //    '<S490>/Coriolis Function'
  //    '<S490>/Inertia Function '
  //    '<S396>/Inertia Function '
  //    '<S396>/MATLAB Function'
  //    '<S449>/Inertia Function '
  //    '<S449>/MATLAB Function'
  //    '<S499>/MATLAB Function'

  { 13.072, 0.2405, 12.039, 0.225692, 11.267, 0.204245, 1.2788882640695931, 0.25,
    0.196822, 0.107678, 0.198152, 0.106348, 0.062097, 0.025153, 0.345, 0.335,
    0.111, 0.003704, 0.003506, 0.000106 },

  // Variable: noise_variance_BLACK
  //  Referenced by: '<S461>/Random Number'

  0.0,

  // Variable: noise_variance_BLUE
  //  Referenced by: '<S473>/Random Number'

  0.0,

  // Variable: noise_variance_RED
  //  Referenced by: '<S499>/Random Number'

  0.0,

  // Variable: platformSelection
  //  Referenced by:
  //    '<S150>/Constant4'
  //    '<S349>/Constant3'
  //    '<S402>/Constant3'
  //    '<S460>/Constant'
  //    '<S58>/Constant'
  //    '<S58>/Constant1'
  //    '<S334>/Constant4'
  //    '<S335>/Constant4'
  //    '<S336>/Constant1'
  //    '<S337>/Constant3'

  5.0,

  // Variable: simMode
  //  Referenced by:
  //    '<S7>/Constant'
  //    '<S22>/Constant'
  //    '<S28>/Constant'
  //    '<S32>/Constant'
  //    '<S79>/Constant'
  //    '<S96>/Constant'
  //    '<S129>/Constant'
  //    '<S139>/Constant'
  //    '<S143>/Constant'
  //    '<S147>/Constant'
  //    '<S151>/Constant'
  //    '<S183>/Constant'
  //    '<S350>/Constant'
  //    '<S403>/Constant'
  //    '<S456>/Constant'

  0.0,

  // Variable: thruster_count_threshold
  //  Referenced by:
  //    '<S34>/MATLAB Function10'
  //    '<S46>/MATLAB Function6'
  //    '<S58>/MATLAB Function5'

  0.0,

  // Variable: thruster_dist2CG_BLACK
  //  Referenced by:
  //    '<S34>/MATLAB Function'
  //    '<S34>/MATLAB Function1'
  //    '<S34>/MATLAB Function11'

  { 83.42, -52.58, 55.94, -60.05, 54.08, -53.92, 77.06, -55.94 },

  // Variable: thruster_dist2CG_BLUE
  //  Referenced by:
  //    '<S46>/MATLAB Function'
  //    '<S46>/MATLAB Function1'
  //    '<S46>/MATLAB Function7'

  { 73.54, -64.46, 81.39, -60.11, 64.46, -65.94, 54.61, -74.39 },

  // Variable: thruster_dist2CG_RED
  //  Referenced by:
  //    '<S58>/MATLAB Function'
  //    '<S58>/MATLAB Function1'
  //    '<S58>/MATLAB Function3'
  //    '<S58>/MATLAB Function4'
  //    '<S58>/MATLAB Function6'

  { 49.92, -78.08, 70.46, -63.54, 81.08, -50.42, 57.44, -75.96 },

  // Variable: xLength
  //  Referenced by:
  //    '<S184>/Desired X-Position (BLACK)'
  //    '<S185>/Desired X-Position (BLACK)'
  //    '<S186>/Desired X-Position (BLACK)'
  //    '<S187>/Desired X-Position (BLACK)'
  //    '<S257>/MATLAB Function2'
  //    '<S258>/MATLAB Function2'
  //    '<S259>/MATLAB Function2'
  //    '<S260>/MATLAB Function2'

  3.51155,

  // Variable: yLength
  //  Referenced by:
  //    '<S184>/Desired Y-Position (BLACK)'
  //    '<S185>/Desired Y-Position (BLACK)'
  //    '<S186>/Desired Y-Position (BLACK)'
  //    '<S187>/Desired Y-Position (BLACK)'
  //    '<S257>/MATLAB Function3'
  //    '<S258>/MATLAB Function3'
  //    '<S259>/MATLAB Function3'
  //    '<S260>/MATLAB Function3'

  2.41935,

  // Expression: 100
  //  Referenced by: '<Root>/Initialization Block for Speed Control'

  100.0,

  // Expression: 1920
  //  Referenced by: '<Root>/Initialization Block for Speed Control'

  1920.0,

  // Expression: 1023
  //  Referenced by: '<Root>/Initialization Block for Speed Control'

  1023.0,

  // Expression: 2000
  //  Referenced by: '<Root>/Initialization Block for Speed Control'

  2000.0,

  // Computed Parameter: BLACKPWM_Y0
  //  Referenced by: '<S29>/BLACK PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S34>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S34>/Remove Negatives'

  0.0,

  // Computed Parameter: BLUEPWM_Y0
  //  Referenced by: '<S30>/BLUE PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S46>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S46>/Remove Negatives'

  0.0,

  // Computed Parameter: REDPWM_Y0
  //  Referenced by: '<S31>/RED PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S58>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S58>/Remove Negatives'

  0.0,

  // Expression: sampleTime
  //  Referenced by: '<S92>/Digital Read'

  0.1,

  // Expression: 0.1
  //  Referenced by: '<S80>/Saturate Torque'

  0.1,

  // Expression: -0.1
  //  Referenced by: '<S80>/Saturate Torque'

  -0.1,

  // Expression: 0.0
  //  Referenced by: '<S80>/Delay'

  0.0,

  // Expression: 3375/64
  //  Referenced by: '<S84>/Gearbox  Ratio'

  52.734375,

  // Expression: 7000
  //  Referenced by: '<S81>/Saturate Motor'

  7000.0,

  // Expression: -7000
  //  Referenced by: '<S81>/Saturate Motor'

  -7000.0,

  // Expression: (7000*64/3375)
  //  Referenced by: '<S80>/Saturate RPM'

  132.74074074074073,

  // Expression: -(7000*64/3375)
  //  Referenced by: '<S80>/Saturate RPM'

  -132.74074074074073,

  // Computed Parameter: Out1_Y0
  //  Referenced by: '<S113>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_p
  //  Referenced by: '<S114>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a
  //  Referenced by: '<S115>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pf
  //  Referenced by: '<S116>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k
  //  Referenced by: '<S117>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f
  //  Referenced by: '<S118>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_m
  //  Referenced by: '<S119>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_po
  //  Referenced by: '<S120>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_n
  //  Referenced by: '<S121>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pl
  //  Referenced by: '<S122>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_aq
  //  Referenced by: '<S123>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_au
  //  Referenced by: '<S124>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_b
  //  Referenced by: '<S125>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bw
  //  Referenced by: '<S126>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c
  //  Referenced by: '<S127>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_i
  //  Referenced by: '<S468>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bu
  //  Referenced by: '<S469>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j
  //  Referenced by: '<S470>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_mq
  //  Referenced by: '<S471>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_e
  //  Referenced by: '<S472>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_eo
  //  Referenced by: '<S480>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_d
  //  Referenced by: '<S481>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ed
  //  Referenced by: '<S482>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_p0
  //  Referenced by: '<S483>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ia
  //  Referenced by: '<S484>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h
  //  Referenced by: '<S496>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hv
  //  Referenced by: '<S497>/Out1'

  0.0,

  // Expression: 0.0
  //  Referenced by: '<S490>/Delay'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainval
  //  Referenced by: '<S490>/Acceleration  to Velocity'

  0.05,

  // Expression: [drop_states_RED,drop_states_ARM,0,0,0,0,0,0]
  //  Referenced by: '<S490>/Acceleration  to Velocity'

  { 2.255775, 1.209675, 3.1415926535897931, 1.5707963267948966,
    1.5707963267948966, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },

  // Expression: 0
  //  Referenced by: '<S490>/Random Number2'

  0.0,

  // Computed Parameter: RandomNumber2_StdDev
  //  Referenced by: '<S490>/Random Number2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S490>/Random Number2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S490>/Random Number1'

  0.0,

  // Computed Parameter: RandomNumber1_StdDev
  //  Referenced by: '<S490>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S490>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S490>/Random Number3'

  0.0,

  // Computed Parameter: RandomNumber3_StdDev
  //  Referenced by: '<S490>/Random Number3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S490>/Random Number3'

  0.0,

  // Computed Parameter: Out1_Y0_l
  //  Referenced by: '<S506>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f2
  //  Referenced by: '<S507>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fu
  //  Referenced by: '<S508>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lh
  //  Referenced by: '<S509>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a3
  //  Referenced by: '<S510>/Out1'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_k
  //  Referenced by: '<S499>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S499>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval
  //  Referenced by: '<S499>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S499>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S499>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number7'

  0.0,

  // Computed Parameter: RandomNumber7_StdDev
  //  Referenced by: '<S498>/Random Number7'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number6'

  0.0,

  // Computed Parameter: RandomNumber6_StdDev
  //  Referenced by: '<S498>/Random Number6'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number8'

  0.0,

  // Computed Parameter: RandomNumber8_StdDev
  //  Referenced by: '<S498>/Random Number8'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S498>/Random Number8'

  0.0,

  // Expression: 0
  //  Referenced by: '<S485>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S486>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S487>/Constant'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_i
  //  Referenced by: '<S461>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S461>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_c
  //  Referenced by: '<S461>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S461>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S461>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number1'

  0.0,

  // Computed Parameter: RandomNumber1_StdDev_o
  //  Referenced by: '<S457>/Random Number1'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number'

  0.0,

  // Computed Parameter: RandomNumber_StdDev
  //  Referenced by: '<S457>/Random Number'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number2'

  0.0,

  // Computed Parameter: RandomNumber2_StdDev_p
  //  Referenced by: '<S457>/Random Number2'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S457>/Random Number2'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_b
  //  Referenced by: '<S473>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S473>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_f
  //  Referenced by: '<S473>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S473>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S473>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number4'

  0.0,

  // Computed Parameter: RandomNumber4_StdDev
  //  Referenced by: '<S458>/Random Number4'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number3'

  0.0,

  // Computed Parameter: RandomNumber3_StdDev_j
  //  Referenced by: '<S458>/Random Number3'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number5'

  0.0,

  // Computed Parameter: RandomNumber5_StdDev
  //  Referenced by: '<S458>/Random Number5'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S458>/Random Number5'

  0.0,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<S459>/Discrete-Time Integrator'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator1_gainval
  //  Referenced by: '<S459>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator2_gainval
  //  Referenced by: '<S459>/Discrete-Time Integrator2'

  0.05,

  // Expression: 0
  //  Referenced by: '<S130>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S130>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S130>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S130>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S130>/Delay4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S131>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S131>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S131>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S131>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S131>/Delay4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S132>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S132>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S132>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S132>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S132>/Delay4'

  0.0,

  // Expression: 0.1
  //  Referenced by: '<S17>/WhoAmI'

  0.1,

  // Computed Parameter: Out1_Y0_aw
  //  Referenced by: '<S157>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_av
  //  Referenced by: '<S159>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h4
  //  Referenced by: '<S161>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S148>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_mf
  //  Referenced by: '<S170>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a4
  //  Referenced by: '<S172>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_kw
  //  Referenced by: '<S174>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S149>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S179>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S178>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S150>/Constant2'

  0.0,

  // Expression: 1
  //  Referenced by: '<S150>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S344>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S343>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S336>/Constant4'

  0.0,

  // Expression: 1
  //  Referenced by: '<S336>/Puck State'

  1.0,

  // Expression: 1
  //  Referenced by: '<S346>/Constant1'

  1.0,

  // Expression: pi/180
  //  Referenced by: '<S346>/Gain'

  0.017453292519943295,

  // Expression: 0
  //  Referenced by: '<S346>/Constant3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S346>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S345>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S337>/Constant1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S337>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_o
  //  Referenced by: '<S356>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ii
  //  Referenced by: '<S358>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hq
  //  Referenced by: '<S360>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S347>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_lc
  //  Referenced by: '<S384>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_of
  //  Referenced by: '<S386>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_l2
  //  Referenced by: '<S388>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S381>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S349>/Constant2'

  0.0,

  // Expression: 1
  //  Referenced by: '<S349>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_by
  //  Referenced by: '<S409>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c2
  //  Referenced by: '<S411>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f2b
  //  Referenced by: '<S413>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S400>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_jf
  //  Referenced by: '<S437>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fn
  //  Referenced by: '<S439>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a2
  //  Referenced by: '<S441>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S434>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S402>/Constant1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S402>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S453>/Constant3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S453>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S453>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S453>/Puck State'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Puck State'

  0.0,

  // Expression: -1
  //  Referenced by: '<S455>/Gain'

  -1.0,

  // Expression: 0.005
  //  Referenced by: '<S455>/Saturation'

  0.005,

  // Expression: -0.005
  //  Referenced by: '<S455>/Saturation'

  -0.005,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px13'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px14'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px15'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px16'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px17'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px18'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px27'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px28'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px33'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px36'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px39'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz10'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz11'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz9'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fx'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fx1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fx_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fx_Sat1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fx_Sat2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fy'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fy1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fy_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fy_Sat1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Fy_Sat2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Px'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Px1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Py'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Py1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Rz'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Rz1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Tz'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Tz1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Tz_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Tz_Sat1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/BLACK_Tz_Sat2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Fx'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Fx_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Fy'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Fy_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px10'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px11'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px12'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px19'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px20'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px21'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px22'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px23'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px24'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px25'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px26'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px29'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px3'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px30'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px31'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px32'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px34'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px35'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px37'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px38'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px4'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px5'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px6'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px7'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px8'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px9'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Py'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Rz'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz12'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz13'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz14'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz3'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz4'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz5'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz6'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz7'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz8'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_RW'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_RW Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_RW Sat1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_RW1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz_Sat1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_dRz_RW Sat'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time1'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time10'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time11'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time12'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time13'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time14'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time15'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time16'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time17'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time18'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time19'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time3'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time4'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time5'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time6'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time7'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time8'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Universal_Time9'

  0.0,

  // Computed Parameter: UDPReceive_Port
  //  Referenced by: '<S97>/UDP Receive'

  25000,

  // Computed Parameter: UDPSend_Port
  //  Referenced by: '<S18>/UDP Send'

  26000,

  // Computed Parameter: Delay1_InitialCondition_m
  //  Referenced by: '<S85>/Delay1'

  0,

  // Computed Parameter: Delay2_InitialCondition_k
  //  Referenced by: '<S85>/Delay2'

  0,

  // Computed Parameter: Delay5_InitialCondition_e
  //  Referenced by: '<S85>/Delay5'

  0,

  // Computed Parameter: Delay3_InitialCondition_i
  //  Referenced by: '<S85>/Delay3'

  0,

  // Computed Parameter: Delay4_InitialCondition_d
  //  Referenced by: '<S85>/Delay4'

  0,

  // Start of '<S16>/Change BLUE Behavior'
  {
    // Expression: 0
    //  Referenced by: '<S454>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S454>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S454>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S454>/Puck State'

    0.0
  }
  ,

  // End of '<S16>/Change BLUE Behavior'

  // Start of '<S402>/RED+ARM'
  {
    // Expression: 0.9
    //  Referenced by: '<S435>/Constant2'

    0.9,

    // Expression: 0
    //  Referenced by: '<S435>/Constant'

    0.0
  }
  ,

  // End of '<S402>/RED+ARM'

  // Start of '<S15>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S422>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S424>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S426>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S401>/Puck State'

    1.0
  }
  ,

  // End of '<S15>/Change BLUE Behavior'

  // Start of '<S349>/RED+ARM'
  {
    // Expression: 0.9
    //  Referenced by: '<S382>/Constant2'

    0.9,

    // Expression: 0
    //  Referenced by: '<S382>/Constant'

    0.0
  }
  ,

  // End of '<S349>/RED+ARM'

  // Start of '<S14>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S369>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S371>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S373>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S348>/Puck State'

    1.0
  }
  ,

  // End of '<S14>/Change BLUE Behavior'

  // Start of '<S182>/Sub-Phase #2 '
  {
    // Expression: 0
    //  Referenced by: '<S342>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S341>/Constant7'

    0.0,

    // Expression: 0
    //  Referenced by: '<S335>/Constant1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S335>/Puck State'

    1.0
  }
  ,

  // End of '<S182>/Sub-Phase #2 '

  // Start of '<S182>/Sub-Phase #1'
  {
    // Expression: 0
    //  Referenced by: '<S340>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S339>/Constant7'

    0.0,

    // Expression: 0
    //  Referenced by: '<S334>/Constant1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S334>/Puck State'

    1.0
  }
  ,

  // End of '<S182>/Sub-Phase #1'

  // Start of '<S181>/Sub-Phase #4'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S325>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S327>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S329>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S322>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S260>/Puck State'

    1.0
  }
  ,

  // End of '<S181>/Sub-Phase #4'

  // Start of '<S181>/Sub-Phase #3 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S307>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S309>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S311>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S304>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S259>/Puck State'

    1.0
  }
  ,

  // End of '<S181>/Sub-Phase #3 '

  // Start of '<S181>/Sub-Phase #2 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S289>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S291>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S293>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S286>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S258>/Puck State'

    1.0
  }
  ,

  // End of '<S181>/Sub-Phase #2 '

  // Start of '<S181>/Sub-Phase #1'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S271>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S273>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S275>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S257>/Puck State'

    1.0
  }
  ,

  // End of '<S181>/Sub-Phase #1'

  // Start of '<S180>/Sub-Phase #4'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S249>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S251>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S253>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S240>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S187>/Puck State'

    1.0
  }
  ,

  // End of '<S180>/Sub-Phase #4'

  // Start of '<S180>/Sub-Phase #3 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S232>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S234>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S236>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S223>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S186>/Puck State'

    1.0
  }
  ,

  // End of '<S180>/Sub-Phase #3 '

  // Start of '<S180>/Sub-Phase #2 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S215>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S217>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S219>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S206>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S185>/Puck State'

    1.0
  }
  ,

  // End of '<S180>/Sub-Phase #2 '

  // Start of '<S180>/Sub-Phase #1'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S198>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S200>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S202>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S189>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S184>/Puck State'

    1.0
  }
  ,

  // End of '<S180>/Sub-Phase #1'

  // Start of '<Root>/Phase #1:  Start Floating '
  {
    // Expression: 0
    //  Referenced by: '<S144>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S144>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S144>/Constant5'

    0.0,

    // Expression: 1
    //  Referenced by: '<S144>/Puck State'

    1.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S146>/Constant6'

    0.0,

    // Expression: 1
    //  Referenced by: '<S146>/Puck State'

    1.0,

    // Start of '<S10>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S145>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S145>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S145>/Constant5'

      0.0,

      // Expression: 1
      //  Referenced by: '<S145>/Puck State'

      1.0
    }
    // End of '<S10>/Change BLUE Behavior'
  }
  ,

  // End of '<Root>/Phase #1:  Start Floating '

  // Start of '<Root>/Phase #0:  Wait for Synchronization'
  {
    // Expression: 0
    //  Referenced by: '<S140>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S140>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S140>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S140>/Puck State'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Constant6'

    0.0,

    // Expression: 0
    //  Referenced by: '<S142>/Puck State'

    0.0,

    // Start of '<S10>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S141>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S141>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S141>/Constant5'

      0.0,

      // Expression: 0
      //  Referenced by: '<S141>/Puck State'

      0.0
    }
    // End of '<S10>/Change BLUE Behavior'
  }
  // End of '<Root>/Phase #0:  Wait for Synchronization'
};

//
// File trailer for generated code.
//
// [EOF]
//
