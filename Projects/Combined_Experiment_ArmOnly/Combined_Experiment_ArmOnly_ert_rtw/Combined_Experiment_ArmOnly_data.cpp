//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Combined_Experiment_ArmOnly_data.cpp
//
// Code generated for Simulink model 'Combined_Experiment_ArmOnly'.
//
// Model version                  : 1.194
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Thu Mar 10 13:54:46 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#include "Combined_Experiment_ArmOnly.h"
#include "Combined_Experiment_ArmOnly_private.h"

// Block parameters (default storage)
P_Combined_Experiment_ArmOnly_T Combined_Experiment_ArmOnly_P = {
  // Variable: F_thrusters_BLACK
  //  Referenced by:
  //    '<S27>/MATLAB Function'
  //    '<S27>/MATLAB Function1'

  { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25 },

  // Variable: F_thrusters_BLUE
  //  Referenced by:
  //    '<S36>/MATLAB Function'
  //    '<S36>/MATLAB Function1'

  { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25 },

  // Variable: F_thrusters_RED
  //  Referenced by:
  //    '<S45>/MATLAB Function'
  //    '<S45>/MATLAB Function1'

  { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25 },

  // Variable: Kd_tb
  //  Referenced by:
  //    '<S278>/kd_tb'
  //    '<S414>/kd_tb'
  //    '<S457>/kd_tb'
  //    '<S326>/kd_tb'
  //    '<S340>/kd_tb'

  0.4,

  // Variable: Kd_tblue
  //  Referenced by:
  //    '<S291>/kd_tb'
  //    '<S427>/kd_tb'
  //    '<S470>/kd_tb'
  //    '<S361>/kd_tb'
  //    '<S374>/kd_tb'

  0.4,

  // Variable: Kd_tr
  //  Referenced by:
  //    '<S304>/kd_tr'
  //    '<S440>/kd_tr'
  //    '<S483>/kd_tr'
  //    '<S392>/kd_tr'

  0.4,

  // Variable: Kd_xb
  //  Referenced by:
  //    '<S279>/kd_xb'
  //    '<S415>/kd_xb'
  //    '<S458>/kd_xb'
  //    '<S327>/kd_xb'
  //    '<S341>/kd_xb'

  5.0,

  // Variable: Kd_xblue
  //  Referenced by:
  //    '<S292>/kd_xb'
  //    '<S428>/kd_xb'
  //    '<S471>/kd_xb'
  //    '<S362>/kd_xb'
  //    '<S375>/kd_xb'

  5.0,

  // Variable: Kd_xr
  //  Referenced by:
  //    '<S305>/kd_xr'
  //    '<S441>/kd_xr'
  //    '<S484>/kd_xr'
  //    '<S393>/kd_xr'

  5.0,

  // Variable: Kd_yb
  //  Referenced by:
  //    '<S280>/kd_yb'
  //    '<S416>/kd_yb'
  //    '<S459>/kd_yb'
  //    '<S328>/kd_yb'
  //    '<S342>/kd_yb'

  5.0,

  // Variable: Kd_yblue
  //  Referenced by:
  //    '<S293>/kd_yb'
  //    '<S429>/kd_yb'
  //    '<S472>/kd_yb'
  //    '<S363>/kd_yb'
  //    '<S376>/kd_yb'

  5.0,

  // Variable: Kd_yr
  //  Referenced by:
  //    '<S306>/kd_yr'
  //    '<S442>/kd_yr'
  //    '<S485>/kd_yr'
  //    '<S394>/kd_yr'

  5.0,

  // Variable: Kp_tb
  //  Referenced by:
  //    '<S278>/kp_tb'
  //    '<S414>/kp_tb'
  //    '<S457>/kp_tb'
  //    '<S326>/kp_tb'
  //    '<S340>/kp_tb'

  0.1,

  // Variable: Kp_tblue
  //  Referenced by:
  //    '<S291>/kp_tb'
  //    '<S427>/kp_tb'
  //    '<S470>/kp_tb'
  //    '<S361>/kp_tb'
  //    '<S374>/kp_tb'

  0.1,

  // Variable: Kp_tr
  //  Referenced by:
  //    '<S304>/kp_tr'
  //    '<S440>/kp_tr'
  //    '<S483>/kp_tr'
  //    '<S392>/kp_tr'

  0.1,

  // Variable: Kp_xb
  //  Referenced by:
  //    '<S279>/kp_xb'
  //    '<S415>/kp_xb'
  //    '<S458>/kp_xb'
  //    '<S327>/kp_xb'
  //    '<S341>/kp_xb'

  2.0,

  // Variable: Kp_xblue
  //  Referenced by:
  //    '<S292>/kp_xb'
  //    '<S428>/kp_xb'
  //    '<S471>/kp_xb'
  //    '<S362>/kp_xb'
  //    '<S375>/kp_xb'

  2.0,

  // Variable: Kp_xr
  //  Referenced by:
  //    '<S305>/kp_xr'
  //    '<S441>/kp_xr'
  //    '<S484>/kp_xr'
  //    '<S393>/kp_xr'

  2.0,

  // Variable: Kp_yb
  //  Referenced by:
  //    '<S280>/kp_yb'
  //    '<S416>/kp_yb'
  //    '<S459>/kp_yb'
  //    '<S328>/kp_yb'
  //    '<S342>/kp_yb'

  2.0,

  // Variable: Kp_yblue
  //  Referenced by:
  //    '<S293>/kp_yb'
  //    '<S429>/kp_yb'
  //    '<S472>/kp_yb'
  //    '<S363>/kp_yb'
  //    '<S376>/kp_yb'

  2.0,

  // Variable: Kp_yr
  //  Referenced by:
  //    '<S306>/kp_yr'
  //    '<S442>/kp_yr'
  //    '<S485>/kp_yr'
  //    '<S394>/kp_yr'

  2.0,

  // Variable: Phase0_End
  //  Referenced by: '<Root>/Constant4'

  10.0,

  // Variable: Phase1_End
  //  Referenced by: '<Root>/Constant'

  15.0,

  // Variable: Phase2_End
  //  Referenced by: '<Root>/Constant1'

  35.0,

  // Variable: Phase3_End
  //  Referenced by: '<Root>/Constant2'

  251.0,

  // Variable: Phase3_SubPhase1_End
  //  Referenced by:
  //    '<S317>/Constant4'
  //    '<S318>/Constant4'
  //    '<S319>/Constant4'

  40.0,

  // Variable: Phase3_SubPhase2_End
  //  Referenced by:
  //    '<S317>/Constant1'
  //    '<S318>/Constant1'
  //    '<S319>/Constant1'

  41.0,

  // Variable: Phase3_SubPhase3_End
  //  Referenced by:
  //    '<S317>/Constant2'
  //    '<S318>/Constant2'
  //    '<S319>/Constant2'
  //    '<S399>/Constant2'
  //    '<S344>/Constant1'

  71.0,

  // Variable: Phase3_SubPhase4_End
  //  Referenced by:
  //    '<S317>/Constant3'
  //    '<S318>/Constant3'
  //    '<S319>/Constant3'

  251.0,

  // Variable: Phase4_End
  //  Referenced by: '<Root>/Constant3'

  271.0,

  // Variable: Phase5_End
  //  Referenced by: '<Root>/Constant6'

  291.0,

  // Variable: WhoAmI
  //  Referenced by:
  //    '<S3>/Constant'
  //    '<S4>/Constant'
  //    '<S6>/Constant1'
  //    '<S8>/Constant'
  //    '<S9>/Constant'
  //    '<S10>/Constant'
  //    '<S11>/Constant'
  //    '<S12>/Constant'
  //    '<S13>/Constant'
  //    '<S14>/Constant'
  //    '<S76>/Constant'

  1.0,

  // Variable: drop_states_BLACK
  //  Referenced by: '<S500>/Velocity to Position'

  { 1.36947387695313, 1.22175756835938, -0.0161599479615688 },

  // Variable: drop_states_BLUE
  //  Referenced by: '<S501>/Velocity to Position'

  { 2.655775, 1.709675, 0.0 },

  // Variable: drop_states_RED
  //  Referenced by: '<S505>/Velocity to Position'

  { 2.18401342773438, 1.21458435058594, -3.12528014183044 },

  // Variable: home_states_BLACK
  //  Referenced by:
  //    '<S410>/Desired Attitude (BLACK)'
  //    '<S410>/Desired Px (BLACK)'
  //    '<S410>/Desired Py (BLACK)'
  //    '<S453>/Constant'
  //    '<S453>/Constant2'
  //    '<S453>/Constant3'

  { 1.755775, 1.209675, 0.0 },

  // Variable: home_states_BLUE
  //  Referenced by:
  //    '<S411>/Desired Attitude (BLUE)'
  //    '<S411>/Desired Px (BLUE)'
  //    '<S411>/Desired Py (BLUE)'
  //    '<S454>/Desired Attitude (BLUE)'
  //    '<S454>/Desired Px (BLUE)'
  //    '<S454>/Desired Py (BLUE)'

  { 0.85577500000000006, 1.709675, 0.0 },

  // Variable: home_states_RED
  //  Referenced by:
  //    '<S412>/Constant'
  //    '<S412>/Constant1'
  //    '<S412>/Constant3'
  //    '<S455>/Constant'
  //    '<S455>/Constant2'
  //    '<S455>/Constant3'

  { 2.455775, 1.209675, 3.1415926535897931 },

  // Variable: init_states_BLACK
  //  Referenced by:
  //    '<S274>/Desired Attitude (BLACK)'
  //    '<S274>/Desired Px (BLACK)'
  //    '<S274>/Desired Py (BLACK)'
  //    '<S321>/Desired Attitude (BLACK)'
  //    '<S321>/Desired X-Position (BLACK)'
  //    '<S321>/Desired Y-Position (BLACK)'

  { 1.755775, 1.209675, 0.0 },

  // Variable: init_states_BLUE
  //  Referenced by:
  //    '<S275>/Desired Attitude (BLUE)'
  //    '<S275>/Desired Px (BLUE)'
  //    '<S275>/Desired Py (BLUE)'
  //    '<S356>/Desired Attitude (BLUE)'
  //    '<S356>/Desired Px (BLUE)'
  //    '<S356>/Desired Py (BLUE)'
  //    '<S359>/Desired Attitude (BLUE)'
  //    '<S359>/Desired Px (BLUE)'
  //    '<S359>/Desired Py (BLUE)'

  { 2.655775, 1.709675, 0.0 },

  // Variable: init_states_RED
  //  Referenced by:
  //    '<S276>/Constant'
  //    '<S276>/Constant1'
  //    '<S276>/Constant3'

  { 2.455775, 1.209675, 3.1415926535897931 },

  // Variable: model_param
  //  Referenced by:
  //    '<S500>/MATLAB Function'
  //    '<S501>/MATLAB Function'
  //    '<S505>/MATLAB Function'

  { 16.9478, 0.2709, 12.3341, 0.188, 12.7621, 0.193 },

  // Variable: noise_variance_BLACK
  //  Referenced by: '<S500>/Random Number'

  0.0,

  // Variable: noise_variance_BLUE
  //  Referenced by: '<S501>/Random Number'

  0.0,

  // Variable: noise_variance_RED
  //  Referenced by: '<S505>/Random Number'

  0.0,

  // Variable: platformSelection
  //  Referenced by:
  //    '<S75>/Which PLATFORM is being used?'
  //    '<S77>/Stream PhaseSpace to Platform'

  1.0,

  // Variable: serverRate
  //  Referenced by:
  //    '<S506>/Constant1'
  //    '<S507>/Constant1'
  //    '<S508>/Constant'
  //    '<S509>/divide by delta T'
  //    '<S510>/divide by delta T'
  //    '<S511>/divide by delta T'
  //    '<S512>/divide by delta T'
  //    '<S513>/divide by delta T'
  //    '<S514>/divide by delta T'
  //    '<S515>/divide by delta T'
  //    '<S516>/divide by delta T'
  //    '<S517>/divide by delta T'
  //    '<S518>/divide by delta T'
  //    '<S519>/divide by delta T'
  //    '<S520>/divide by delta T'
  //    '<S521>/divide by delta T'
  //    '<S522>/divide by delta T'
  //    '<S523>/divide by delta T'
  //    '<S77>/Stream PhaseSpace to Platform'
  //    '<S141>/divide by delta T'
  //    '<S142>/divide by delta T'
  //    '<S143>/divide by delta T'
  //    '<S144>/divide by delta T'
  //    '<S145>/divide by delta T'
  //    '<S146>/divide by delta T'
  //    '<S147>/divide by delta T'
  //    '<S148>/divide by delta T'
  //    '<S149>/divide by delta T'
  //    '<S150>/divide by delta T'
  //    '<S151>/divide by delta T'
  //    '<S152>/divide by delta T'
  //    '<S282>/divide by delta T'
  //    '<S284>/divide by delta T'
  //    '<S286>/divide by delta T'
  //    '<S295>/divide by delta T'
  //    '<S297>/divide by delta T'
  //    '<S299>/divide by delta T'
  //    '<S308>/divide by delta T'
  //    '<S310>/divide by delta T'
  //    '<S312>/divide by delta T'
  //    '<S418>/divide by delta T'
  //    '<S420>/divide by delta T'
  //    '<S422>/divide by delta T'
  //    '<S431>/divide by delta T'
  //    '<S433>/divide by delta T'
  //    '<S435>/divide by delta T'
  //    '<S444>/divide by delta T'
  //    '<S446>/divide by delta T'
  //    '<S448>/divide by delta T'
  //    '<S461>/divide by delta T'
  //    '<S463>/divide by delta T'
  //    '<S465>/divide by delta T'
  //    '<S474>/divide by delta T'
  //    '<S476>/divide by delta T'
  //    '<S478>/divide by delta T'
  //    '<S487>/divide by delta T'
  //    '<S489>/divide by delta T'
  //    '<S491>/divide by delta T'
  //    '<S81>/divide by delta T'
  //    '<S82>/divide by delta T'
  //    '<S83>/divide by delta T'
  //    '<S84>/divide by delta T'
  //    '<S85>/divide by delta T'
  //    '<S86>/divide by delta T'
  //    '<S87>/divide by delta T'
  //    '<S88>/divide by delta T'
  //    '<S89>/divide by delta T'
  //    '<S90>/divide by delta T'
  //    '<S91>/divide by delta T'
  //    '<S92>/divide by delta T'
  //    '<S109>/divide by delta T'
  //    '<S110>/divide by delta T'
  //    '<S111>/divide by delta T'
  //    '<S112>/divide by delta T'
  //    '<S113>/divide by delta T'
  //    '<S114>/divide by delta T'
  //    '<S115>/divide by delta T'
  //    '<S116>/divide by delta T'
  //    '<S117>/divide by delta T'
  //    '<S118>/divide by delta T'
  //    '<S119>/divide by delta T'
  //    '<S120>/divide by delta T'
  //    '<S330>/divide by delta T'
  //    '<S332>/divide by delta T'
  //    '<S334>/divide by delta T'
  //    '<S347>/divide by delta T'
  //    '<S349>/divide by delta T'
  //    '<S351>/divide by delta T'
  //    '<S365>/divide by delta T'
  //    '<S367>/divide by delta T'
  //    '<S369>/divide by delta T'
  //    '<S378>/divide by delta T'
  //    '<S380>/divide by delta T'
  //    '<S382>/divide by delta T'
  //    '<S400>/divide by delta T'
  //    '<S402>/divide by delta T'
  //    '<S404>/divide by delta T'

  0.1,

  // Variable: simMode
  //  Referenced by:
  //    '<S7>/Constant'
  //    '<S19>/Constant'
  //    '<S25>/Constant'
  //    '<S57>/Constant'
  //    '<S74>/Constant'
  //    '<S172>/Constant'
  //    '<S269>/Constant'
  //    '<S273>/Constant'
  //    '<S277>/Constant'
  //    '<S320>/Constant'
  //    '<S413>/Constant'
  //    '<S456>/Constant'
  //    '<S499>/Constant'
  //    '<S78>/Constant'

  0.0,

  // Variable: thruster_dist2CG_BLACK
  //  Referenced by:
  //    '<S27>/MATLAB Function'
  //    '<S27>/MATLAB Function1'

  { 83.42, -52.58, 55.94, -60.05, 54.08, -53.92, 77.06, -55.94 },

  // Variable: thruster_dist2CG_BLUE
  //  Referenced by:
  //    '<S36>/MATLAB Function'
  //    '<S36>/MATLAB Function1'

  { 83.42, -52.58, 55.94, -60.05, 54.08, -53.92, 77.06, -55.94 },

  // Variable: thruster_dist2CG_RED
  //  Referenced by:
  //    '<S45>/MATLAB Function'
  //    '<S45>/MATLAB Function1'

  { 49.92, -78.08, 70.46, -63.54, 81.08, -50.42, 57.44, -75.96 },

  // Variable: xLength
  //  Referenced by:
  //    '<S324>/Desired X-Position (BLACK)'
  //    '<S390>/MATLAB Function'

  3.51155,

  // Variable: yLength
  //  Referenced by:
  //    '<S324>/Desired Y-Position (BLACK)'
  //    '<S390>/MATLAB Function1'

  2.41935,

  // Expression: 400
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  400.0,

  // Expression: 0
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  0.0,

  // Expression: 200
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  200.0,

  // Expression: 4095
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  4095.0,

  // Expression: 0
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/Initialization Block for Position Control'

  0.0,

  // Computed Parameter: BLACKPWM_Y0
  //  Referenced by: '<S22>/BLACK PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S27>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S27>/Remove Negatives'

  0.0,

  // Computed Parameter: BLUEPWM_Y0
  //  Referenced by: '<S23>/BLUE PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S36>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S36>/Remove Negatives'

  0.0,

  // Computed Parameter: REDPWM_Y0
  //  Referenced by: '<S24>/RED PWM'

  0.0,

  // Expression: 1000
  //  Referenced by: '<S45>/Remove Negatives'

  1000.0,

  // Expression: 0
  //  Referenced by: '<S45>/Remove Negatives'

  0.0,

  // Expression: sampleTime
  //  Referenced by: '<S70>/Digital Read'

  0.1,

  // Expression: 0.1
  //  Referenced by: '<S58>/Saturate Torque'

  0.1,

  // Expression: -0.1
  //  Referenced by: '<S58>/Saturate Torque'

  -0.1,

  // Expression: 0.0
  //  Referenced by: '<S58>/Delay'

  0.0,

  // Expression: 3375/64
  //  Referenced by: '<S62>/Gearbox  Ratio'

  52.734375,

  // Expression: 7000
  //  Referenced by: '<S59>/Saturate Motor'

  7000.0,

  // Expression: -7000
  //  Referenced by: '<S59>/Saturate Motor'

  -7000.0,

  // Expression: (7000*64/3375)
  //  Referenced by: '<S58>/Saturate RPM'

  132.74074074074073,

  // Expression: -(7000*64/3375)
  //  Referenced by: '<S58>/Saturate RPM'

  -132.74074074074073,

  // Computed Parameter: Out1_Y0
  //  Referenced by: '<S154>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_m
  //  Referenced by: '<S139>/Out1'

  0.0,

  // Computed Parameter: ActualTime_Y0
  //  Referenced by: '<S140>/Actual Time'

  0.0,

  // Computed Parameter: Out1_Y0_l
  //  Referenced by: '<S157>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f
  //  Referenced by: '<S158>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_e
  //  Referenced by: '<S159>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_b
  //  Referenced by: '<S160>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k
  //  Referenced by: '<S161>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h
  //  Referenced by: '<S162>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_d
  //  Referenced by: '<S163>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_o
  //  Referenced by: '<S164>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ec
  //  Referenced by: '<S165>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_n
  //  Referenced by: '<S166>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_oi
  //  Referenced by: '<S167>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fr
  //  Referenced by: '<S168>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S137>/Constant'

  0.0,

  // Computed Parameter: Out1_Y0_e2
  //  Referenced by: '<S93>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a
  //  Referenced by: '<S94>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bu
  //  Referenced by: '<S95>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_i
  //  Referenced by: '<S96>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bw
  //  Referenced by: '<S97>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j
  //  Referenced by: '<S98>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j3
  //  Referenced by: '<S99>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lj
  //  Referenced by: '<S100>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_g
  //  Referenced by: '<S101>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_id
  //  Referenced by: '<S102>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j1
  //  Referenced by: '<S103>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fb
  //  Referenced by: '<S104>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_os
  //  Referenced by: '<S122>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nx
  //  Referenced by: '<S107>/Out1'

  0.0,

  // Computed Parameter: ActualTime_Y0_l
  //  Referenced by: '<S108>/Actual Time'

  0.0,

  // Computed Parameter: Out1_Y0_lf
  //  Referenced by: '<S125>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gx
  //  Referenced by: '<S126>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ih
  //  Referenced by: '<S127>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_o0
  //  Referenced by: '<S128>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ac
  //  Referenced by: '<S129>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bk
  //  Referenced by: '<S130>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_l0
  //  Referenced by: '<S131>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ok
  //  Referenced by: '<S132>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ou
  //  Referenced by: '<S133>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k0
  //  Referenced by: '<S134>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ks
  //  Referenced by: '<S135>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ju
  //  Referenced by: '<S136>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S105>/Constant'

  0.0,

  // Computed Parameter: Out1_Y0_l0t
  //  Referenced by: '<S183>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_et
  //  Referenced by: '<S186>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nxn
  //  Referenced by: '<S190>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_p
  //  Referenced by: '<S196>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_es
  //  Referenced by: '<S199>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c
  //  Referenced by: '<S202>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S189>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S169>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S169>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainval
  //  Referenced by: '<S181>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: Out1_Y0_i0
  //  Referenced by: '<S214>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k3
  //  Referenced by: '<S217>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nl
  //  Referenced by: '<S221>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pj
  //  Referenced by: '<S227>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_aci
  //  Referenced by: '<S230>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c1
  //  Referenced by: '<S233>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S220>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S170>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S170>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainv_d
  //  Referenced by: '<S212>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: Out1_Y0_e5
  //  Referenced by: '<S245>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ov
  //  Referenced by: '<S248>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_be
  //  Referenced by: '<S252>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_is
  //  Referenced by: '<S258>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c4
  //  Referenced by: '<S261>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_kq
  //  Referenced by: '<S264>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S251>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S171>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S171>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainv_k
  //  Referenced by: '<S243>/Discrete-Time Integrator1'

  0.05,

  // Expression: initCond
  //  Referenced by: '<S528>/ '

  { 1.36947387695313, 1.22175756835938, -0.0161599479615688 },

  // Expression: initCond
  //  Referenced by: '<S530>/ '

  { 2.655775, 1.709675, 0.0 },

  // Expression: initCond
  //  Referenced by: '<S532>/ '

  { 2.18401342773438, 1.21458435058594, -3.12528014183044 },

  // Computed Parameter: Out1_Y0_cg
  //  Referenced by: '<S533>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lc
  //  Referenced by: '<S534>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j5
  //  Referenced by: '<S535>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fg
  //  Referenced by: '<S536>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pt
  //  Referenced by: '<S537>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hk
  //  Referenced by: '<S538>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_cv
  //  Referenced by: '<S539>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_cu
  //  Referenced by: '<S540>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_or
  //  Referenced by: '<S541>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ls
  //  Referenced by: '<S542>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c4g
  //  Referenced by: '<S543>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gp
  //  Referenced by: '<S544>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_m2
  //  Referenced by: '<S545>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gr
  //  Referenced by: '<S546>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gq
  //  Referenced by: '<S547>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S502>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S503>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S504>/Constant'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainval
  //  Referenced by: '<S500>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S500>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval
  //  Referenced by: '<S500>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S500>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S500>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number1'

  0.0,

  // Computed Parameter: RandomNumber1_StdDev
  //  Referenced by: '<S16>/Random Number1'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number'

  0.0,

  // Computed Parameter: RandomNumber_StdDev
  //  Referenced by: '<S16>/Random Number'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number2'

  0.0,

  // Computed Parameter: RandomNumber2_StdDev
  //  Referenced by: '<S16>/Random Number2'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number2'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_e
  //  Referenced by: '<S501>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S501>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_n
  //  Referenced by: '<S501>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S501>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S501>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number4'

  0.0,

  // Computed Parameter: RandomNumber4_StdDev
  //  Referenced by: '<S16>/Random Number4'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number3'

  0.0,

  // Computed Parameter: RandomNumber3_StdDev
  //  Referenced by: '<S16>/Random Number3'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number5'

  0.0,

  // Computed Parameter: RandomNumber5_StdDev
  //  Referenced by: '<S16>/Random Number5'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number5'

  0.0,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<S16>/Discrete-Time Integrator'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator1_gainv_c
  //  Referenced by: '<S16>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator2_gainval
  //  Referenced by: '<S16>/Discrete-Time Integrator2'

  0.05,

  // Computed Parameter: AccelerationtoVelocity_gainva_n
  //  Referenced by: '<S505>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S505>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_p
  //  Referenced by: '<S505>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S505>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S505>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number7'

  0.0,

  // Computed Parameter: RandomNumber7_StdDev
  //  Referenced by: '<S16>/Random Number7'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number6'

  0.0,

  // Computed Parameter: RandomNumber6_StdDev
  //  Referenced by: '<S16>/Random Number6'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number8'

  0.0,

  // Computed Parameter: RandomNumber8_StdDev
  //  Referenced by: '<S16>/Random Number8'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S16>/Random Number8'

  0.0,

  // Computed Parameter: Out1_Y0_mp
  //  Referenced by: '<S283>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_av
  //  Referenced by: '<S285>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_g1
  //  Referenced by: '<S287>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S274>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_ovt
  //  Referenced by: '<S296>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a4
  //  Referenced by: '<S298>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nv
  //  Referenced by: '<S300>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S275>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_frl
  //  Referenced by: '<S309>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bka
  //  Referenced by: '<S311>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j2
  //  Referenced by: '<S313>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S276>/Constant2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S276>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S276>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S276>/Constant6'

  0.0,

  // Expression: 1
  //  Referenced by: '<S276>/Puck State'

  1.0,

  // Expression: 1.5708
  //  Referenced by: '<S276>/Constant7'

  1.5708,

  // Expression: 1.5708
  //  Referenced by: '<S276>/Constant8'

  1.5708,

  // Expression: 0
  //  Referenced by: '<S276>/Constant9'

  0.0,

  // Computed Parameter: Out1_Y0_pn
  //  Referenced by: '<S331>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bd
  //  Referenced by: '<S333>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ai
  //  Referenced by: '<S335>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S321>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_pz
  //  Referenced by: '<S348>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_dz
  //  Referenced by: '<S350>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_jz
  //  Referenced by: '<S352>/Out1'

  0.0,

  // Expression: 0.034906585
  //  Referenced by: '<S339>/Desired Rate (BLACK)'

  0.034906585,

  // Expression: 1
  //  Referenced by: '<S324>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S389>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S389>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S389>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S389>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S389>/Constant6'

  0.0,

  // Expression: 1
  //  Referenced by: '<S389>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_pz4
  //  Referenced by: '<S401>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_of
  //  Referenced by: '<S403>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ko
  //  Referenced by: '<S405>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S390>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S390>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S390>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S390>/Constant6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S398>/Constant'

  0.0,

  // Expression: 1
  //  Referenced by: '<S390>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_jr
  //  Referenced by: '<S419>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_mr
  //  Referenced by: '<S421>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_i0r
  //  Referenced by: '<S423>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S410>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_nf
  //  Referenced by: '<S445>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gb
  //  Referenced by: '<S447>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_da
  //  Referenced by: '<S449>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S412>/Constant2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S412>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S412>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S412>/Constant6'

  0.0,

  // Expression: 1
  //  Referenced by: '<S412>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_gx1
  //  Referenced by: '<S462>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fj
  //  Referenced by: '<S464>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bm
  //  Referenced by: '<S466>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S453>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_i3
  //  Referenced by: '<S488>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bs
  //  Referenced by: '<S490>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c5
  //  Referenced by: '<S492>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S455>/Constant1'

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

  // Expression: 1
  //  Referenced by: '<S455>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S496>/Constant3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S496>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S496>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S496>/Puck State'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Constant6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S498>/Puck State'

  0.0,

  // Expression: -1
  //  Referenced by: '<S498>/Gain'

  -1.0,

  // Expression: 0.005
  //  Referenced by: '<S498>/Saturation'

  0.005,

  // Expression: -0.005
  //  Referenced by: '<S498>/Saturation'

  -0.005,

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
  //  Referenced by: '<Root>/RED_Tz4'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz5'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz6'

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
  //  Referenced by: '<Root>/RED_Px27'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Px28'

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
  //  Referenced by: '<Root>/RED_Tz2'

  0.0,

  // Expression: 0
  //  Referenced by: '<Root>/RED_Tz3'

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
  //  Referenced by: '<S79>/UDP Receive'

  25000,

  // Computed Parameter: SendBLACKStatestoBLACKPlatform_
  //  Referenced by: '<S80>/Send BLACK States to  BLACK Platform'

  25000,

  // Computed Parameter: UDPSend_Port
  //  Referenced by: '<S15>/UDP Send'

  26000,

  // Computed Parameter: Delay1_InitialCondition
  //  Referenced by: '<S63>/Delay1'

  0,

  // Computed Parameter: Delay2_InitialCondition
  //  Referenced by: '<S63>/Delay2'

  0,

  // Computed Parameter: Delay5_InitialCondition
  //  Referenced by: '<S63>/Delay5'

  0,

  // Computed Parameter: Delay3_InitialCondition
  //  Referenced by: '<S63>/Delay3'

  0,

  // Computed Parameter: Delay4_InitialCondition
  //  Referenced by: '<S63>/Delay4'

  0,

  // Computed Parameter: Delay_InitialCondition_k
  //  Referenced by: '<S153>/Delay'

  1,

  // Computed Parameter: Delay_InitialCondition_e
  //  Referenced by: '<S121>/Delay'

  1,

  // Start of '<S14>/Change BLUE Behavior'
  {
    // Expression: 0
    //  Referenced by: '<S497>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S497>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S497>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S497>/Puck State'

    0.0
  }
  ,

  // End of '<S14>/Change BLUE Behavior'

  // Start of '<S13>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S475>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_k
    //  Referenced by: '<S477>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S479>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S454>/Puck State'

    1.0
  }
  ,

  // End of '<S13>/Change BLUE Behavior'

  // Start of '<S12>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S432>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_k
    //  Referenced by: '<S434>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S436>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S411>/Puck State'

    1.0
  }
  ,

  // End of '<S12>/Change BLUE Behavior'

  // Start of '<S319>/Sub-Phase #2 '
  {
    // Expression: 0
    //  Referenced by: '<S388>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S388>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S388>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S388>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S388>/Constant6'

    0.0,

    // Expression: 1
    //  Referenced by: '<S388>/Puck State'

    1.0
  }
  ,

  // End of '<S319>/Sub-Phase #2 '

  // Start of '<S319>/Sub-Phase #1'
  {
    // Expression: 0
    //  Referenced by: '<S387>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S387>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S387>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S387>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S387>/Constant6'

    0.0,

    // Expression: 0
    //  Referenced by: '<S387>/Puck State'

    0.0
  }
  ,

  // End of '<S319>/Sub-Phase #1'

  // Start of '<S318>/Sub-Phase #4'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S379>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_m
    //  Referenced by: '<S381>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S383>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S359>/Puck State'

    1.0
  }
  ,

  // End of '<S318>/Sub-Phase #4'

  // Start of '<S318>/Sub-Phase #3 '
  {
    // Expression: 0
    //  Referenced by: '<S358>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S358>/Puck State'

    1.0
  }
  ,

  // End of '<S318>/Sub-Phase #3 '

  // Start of '<S318>/Sub-Phase #2 '
  {
    // Expression: 0
    //  Referenced by: '<S357>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S357>/Puck State'

    0.0
  }
  ,

  // End of '<S318>/Sub-Phase #2 '

  // Start of '<S318>/Sub-Phase #1'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S366>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_m
    //  Referenced by: '<S368>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S370>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S356>/Puck State'

    1.0
  }
  ,

  // End of '<S318>/Sub-Phase #1'

  // Start of '<S317>/Sub-Phase #3 '
  {
    // Expression: 0
    //  Referenced by: '<S323>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S323>/Puck State'

    1.0
  }
  ,

  // End of '<S317>/Sub-Phase #3 '

  // Start of '<S317>/Sub-Phase #2 '
  {
    // Expression: 0
    //  Referenced by: '<S322>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S322>/Puck State'

    0.0
  }
  ,

  // End of '<S317>/Sub-Phase #2 '

  // Start of '<Root>/Phase #1:  Start Floating '
  {
    // Expression: 0
    //  Referenced by: '<S270>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S270>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S270>/Constant5'

    0.0,

    // Expression: 1
    //  Referenced by: '<S270>/Puck State'

    1.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S272>/Constant6'

    0.0,

    // Expression: 1
    //  Referenced by: '<S272>/Puck State'

    1.0,

    // Start of '<S8>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S271>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S271>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S271>/Constant5'

      0.0,

      // Expression: 1
      //  Referenced by: '<S271>/Puck State'

      1.0
    }
    // End of '<S8>/Change BLUE Behavior'
  }
  ,

  // End of '<Root>/Phase #1:  Start Floating '

  // Start of '<Root>/Phase #0:  Wait for Synchronization'
  {
    // Expression: 0
    //  Referenced by: '<S266>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S266>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S266>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S266>/Puck State'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Constant6'

    0.0,

    // Expression: 0
    //  Referenced by: '<S268>/Puck State'

    0.0,

    // Start of '<S8>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S267>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S267>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S267>/Constant5'

      0.0,

      // Expression: 0
      //  Referenced by: '<S267>/Puck State'

      0.0
    }
    // End of '<S8>/Change BLUE Behavior'
  }
  ,

  // End of '<Root>/Phase #0:  Wait for Synchronization'

  // Start of '<S256>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S263>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S263>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S263>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S263>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S263>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S263>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S263>/Delay4'

    0.0
  }
  ,

  // End of '<S256>/Calculate Running Mean'

  // Start of '<S255>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S260>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S260>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S260>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S260>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S260>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S260>/Delay4'

    0.0
  }
  ,

  // End of '<S255>/Calculate Running Mean'

  // Start of '<S254>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S257>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S257>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S257>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S257>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S257>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S257>/Delay4'

    0.0
  }
  ,

  // End of '<S254>/Calculate Running Mean'

  // Start of '<S243>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S250>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S250>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S250>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S250>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S250>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S250>/Delay4'

    0.0
  }
  ,

  // End of '<S243>/Calculate Running Mean'

  // Start of '<S242>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S247>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S247>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S247>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S247>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S247>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S247>/Delay4'

    0.0
  }
  ,

  // End of '<S242>/Calculate Running Mean'

  // Start of '<S241>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S244>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S244>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S244>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S244>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S244>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S244>/Delay4'

    0.0
  }
  ,

  // End of '<S241>/Calculate Running Mean'

  // Start of '<S171>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S171>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S171>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S171>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S171>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S171>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S171>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S171>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S171>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S171>/AHRS2'

    33.0
  }
  ,

  // End of '<S171>/AHRS2'

  // Start of '<S225>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S232>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S232>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S232>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S232>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S232>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S232>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S232>/Delay4'

    0.0
  }
  ,

  // End of '<S225>/Calculate Running Mean'

  // Start of '<S224>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S229>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S229>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S229>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S229>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S229>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S229>/Delay4'

    0.0
  }
  ,

  // End of '<S224>/Calculate Running Mean'

  // Start of '<S223>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S226>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S226>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S226>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S226>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S226>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S226>/Delay4'

    0.0
  }
  ,

  // End of '<S223>/Calculate Running Mean'

  // Start of '<S212>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S219>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S219>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S219>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S219>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S219>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S219>/Delay4'

    0.0
  }
  ,

  // End of '<S212>/Calculate Running Mean'

  // Start of '<S211>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S216>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S216>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S216>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S216>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S216>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S216>/Delay4'

    0.0
  }
  ,

  // End of '<S211>/Calculate Running Mean'

  // Start of '<S210>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S213>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S213>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S213>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S213>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S213>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S213>/Delay4'

    0.0
  }
  ,

  // End of '<S210>/Calculate Running Mean'

  // Start of '<S170>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S170>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S170>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S170>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S170>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S170>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S170>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S170>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S170>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S170>/AHRS2'

    33.0
  }
  ,

  // End of '<S170>/AHRS2'

  // Start of '<S194>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S201>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S201>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S201>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S201>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S201>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S201>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S201>/Delay4'

    0.0
  }
  ,

  // End of '<S194>/Calculate Running Mean'

  // Start of '<S193>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S198>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S198>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S198>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S198>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S198>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S198>/Delay4'

    0.0
  }
  ,

  // End of '<S193>/Calculate Running Mean'

  // Start of '<S192>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S195>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S195>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S195>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S195>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S195>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S195>/Delay4'

    0.0
  }
  ,

  // End of '<S192>/Calculate Running Mean'

  // Start of '<S181>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S188>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S188>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S188>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S188>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S188>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S188>/Delay4'

    0.0
  }
  ,

  // End of '<S181>/Calculate Running Mean'

  // Start of '<S180>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S185>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S185>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S185>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S185>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S185>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S185>/Delay4'

    0.0
  }
  ,

  // End of '<S180>/Calculate Running Mean'

  // Start of '<S179>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S182>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S182>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S182>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S182>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S182>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S182>/Delay4'

    0.0
  }
  ,

  // End of '<S179>/Calculate Running Mean'

  // Start of '<S169>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S169>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S169>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S169>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S169>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S169>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S169>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S169>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S169>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S169>/AHRS2'

    33.0
  }
  // End of '<S169>/AHRS2'
};

//
// File trailer for generated code.
//
// [EOF]
//
