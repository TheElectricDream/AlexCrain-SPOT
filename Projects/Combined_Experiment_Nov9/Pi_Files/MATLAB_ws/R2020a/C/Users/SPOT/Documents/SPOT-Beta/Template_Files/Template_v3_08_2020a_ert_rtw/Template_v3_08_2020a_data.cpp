//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Template_v3_08_2020a_data.cpp
//
// Code generated for Simulink model 'Template_v3_08_2020a'.
//
// Model version                  : 1.622
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun Jul 10 13:38:58 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#include "Template_v3_08_2020a.h"
#include "Template_v3_08_2020a_private.h"

// Block parameters (default storage)
P_Template_v3_08_2020a_T Template_v3_08_2020a_P = {
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
  //  Referenced by: '<S695>/Dynamics Model'

  0.0025,

  // Variable: Gamma2
  //  Referenced by: '<S695>/Dynamics Model'

  100.0,

  // Variable: Gamma3
  //  Referenced by: '<S695>/Dynamics Model'

  10.0,

  // Variable: Gamma4
  //  Referenced by: '<S695>/Dynamics Model'

  0.001,

  // Variable: Gamma5
  //  Referenced by: '<S695>/Dynamics Model'

  100.0,

  // Variable: Gamma6
  //  Referenced by: '<S695>/Dynamics Model'

  0.001,

  // Variable: Kd_elarm
  //  Referenced by:
  //    '<S271>/kd_elarm'
  //    '<S458>/kd_elarm'
  //    '<S493>/kd_elarm'
  //    '<S514>/kd_elarm'

  0.05,

  // Variable: Kd_sharm
  //  Referenced by:
  //    '<S272>/kd_sharm'
  //    '<S459>/kd_sharm'
  //    '<S494>/kd_sharm'
  //    '<S515>/kd_sharm'

  0.05,

  // Variable: Kd_tb
  //  Referenced by:
  //    '<S230>/kd_tb'
  //    '<S556>/kd_tb'
  //    '<S609>/kd_tb'
  //    '<S290>/kd_tb'
  //    '<S307>/kd_tb'
  //    '<S324>/kd_tb'
  //    '<S341>/kd_tb'

  0.4,

  // Variable: Kd_tblue
  //  Referenced by:
  //    '<S243>/kd_tb'
  //    '<S569>/kd_tb'
  //    '<S622>/kd_tb'
  //    '<S362>/kd_tb'
  //    '<S380>/kd_tb'
  //    '<S398>/kd_tb'
  //    '<S416>/kd_tb'

  0.4,

  // Variable: Kd_tr
  //  Referenced by:
  //    '<S256>/kd_tr'
  //    '<S582>/kd_tr'
  //    '<S635>/kd_tr'
  //    '<S439>/kd_tr'
  //    '<S472>/kd_tr'
  //    '<S524>/kd_tr'

  0.4,

  // Variable: Kd_wrarm
  //  Referenced by:
  //    '<S273>/kd_wrarm'
  //    '<S460>/kd_wrarm'
  //    '<S495>/kd_wrarm'
  //    '<S516>/kd_wrarm'

  0.005,

  // Variable: Kd_xb
  //  Referenced by:
  //    '<S231>/kd_xb'
  //    '<S557>/kd_xb'
  //    '<S610>/kd_xb'
  //    '<S291>/kd_xb'
  //    '<S308>/kd_xb'
  //    '<S325>/kd_xb'
  //    '<S342>/kd_xb'

  8.0,

  // Variable: Kd_xblue
  //  Referenced by:
  //    '<S244>/kd_xb'
  //    '<S570>/kd_xb'
  //    '<S623>/kd_xb'
  //    '<S363>/kd_xb'
  //    '<S381>/kd_xb'
  //    '<S399>/kd_xb'
  //    '<S417>/kd_xb'

  8.0,

  // Variable: Kd_xr
  //  Referenced by:
  //    '<S257>/kd_xr'
  //    '<S583>/kd_xr'
  //    '<S636>/kd_xr'
  //    '<S440>/kd_xr'
  //    '<S473>/kd_xr'
  //    '<S503>/kd_xr'
  //    '<S525>/kd_xr'

  8.0,

  // Variable: Kd_yb
  //  Referenced by:
  //    '<S232>/kd_yb'
  //    '<S558>/kd_yb'
  //    '<S611>/kd_yb'
  //    '<S292>/kd_yb'
  //    '<S309>/kd_yb'
  //    '<S326>/kd_yb'
  //    '<S343>/kd_yb'

  8.0,

  // Variable: Kd_yblue
  //  Referenced by:
  //    '<S245>/kd_yb'
  //    '<S571>/kd_yb'
  //    '<S624>/kd_yb'
  //    '<S364>/kd_yb'
  //    '<S382>/kd_yb'
  //    '<S400>/kd_yb'
  //    '<S418>/kd_yb'

  8.0,

  // Variable: Kd_yr
  //  Referenced by:
  //    '<S258>/kd_yr'
  //    '<S584>/kd_yr'
  //    '<S637>/kd_yr'
  //    '<S441>/kd_yr'
  //    '<S474>/kd_yr'
  //    '<S504>/kd_yr'
  //    '<S526>/kd_yr'

  8.0,

  // Variable: Kp
  //  Referenced by:
  //    '<S598>/MATLAB Function'
  //    '<S651>/MATLAB Function'
  //    '<S544>/MATLAB Function'

  { 0.08, 0.0, 0.0, 0.0, 0.08, 0.0, 0.0, 0.0, 0.002 },

  // Variable: Kp_elarm
  //  Referenced by:
  //    '<S271>/kp_elarm'
  //    '<S458>/kp_elarm'
  //    '<S493>/kp_elarm'
  //    '<S514>/kp_elarm'

  0.08,

  // Variable: Kp_sharm
  //  Referenced by:
  //    '<S272>/kp_sharm'
  //    '<S459>/kp_sharm'
  //    '<S494>/kp_sharm'
  //    '<S515>/kp_sharm'

  0.15,

  // Variable: Kp_tb
  //  Referenced by:
  //    '<S230>/kp_tb'
  //    '<S556>/kp_tb'
  //    '<S609>/kp_tb'
  //    '<S290>/kp_tb'
  //    '<S307>/kp_tb'
  //    '<S324>/kp_tb'
  //    '<S341>/kp_tb'

  0.1,

  // Variable: Kp_tblue
  //  Referenced by:
  //    '<S243>/kp_tb'
  //    '<S569>/kp_tb'
  //    '<S622>/kp_tb'
  //    '<S362>/kp_tb'
  //    '<S380>/kp_tb'
  //    '<S398>/kp_tb'
  //    '<S416>/kp_tb'

  0.1,

  // Variable: Kp_tr
  //  Referenced by:
  //    '<S256>/kp_tr'
  //    '<S582>/kp_tr'
  //    '<S635>/kp_tr'
  //    '<S439>/kp_tr'
  //    '<S472>/kp_tr'
  //    '<S524>/kp_tr'

  0.1,

  // Variable: Kp_wrarm
  //  Referenced by:
  //    '<S273>/kp_wrarm'
  //    '<S460>/kp_wrarm'
  //    '<S495>/kp_wrarm'
  //    '<S516>/kp_wrarm'

  0.007,

  // Variable: Kp_xb
  //  Referenced by:
  //    '<S231>/kp_xb'
  //    '<S557>/kp_xb'
  //    '<S610>/kp_xb'
  //    '<S291>/kp_xb'
  //    '<S308>/kp_xb'
  //    '<S325>/kp_xb'
  //    '<S342>/kp_xb'

  2.0,

  // Variable: Kp_xblue
  //  Referenced by:
  //    '<S244>/kp_xb'
  //    '<S570>/kp_xb'
  //    '<S623>/kp_xb'
  //    '<S363>/kp_xb'
  //    '<S381>/kp_xb'
  //    '<S399>/kp_xb'
  //    '<S417>/kp_xb'

  2.0,

  // Variable: Kp_xr
  //  Referenced by:
  //    '<S257>/kp_xr'
  //    '<S583>/kp_xr'
  //    '<S636>/kp_xr'
  //    '<S440>/kp_xr'
  //    '<S473>/kp_xr'
  //    '<S503>/kp_xr'
  //    '<S525>/kp_xr'

  2.0,

  // Variable: Kp_yb
  //  Referenced by:
  //    '<S232>/kp_yb'
  //    '<S558>/kp_yb'
  //    '<S611>/kp_yb'
  //    '<S292>/kp_yb'
  //    '<S309>/kp_yb'
  //    '<S326>/kp_yb'
  //    '<S343>/kp_yb'

  2.0,

  // Variable: Kp_yblue
  //  Referenced by:
  //    '<S245>/kp_yb'
  //    '<S571>/kp_yb'
  //    '<S624>/kp_yb'
  //    '<S364>/kp_yb'
  //    '<S382>/kp_yb'
  //    '<S400>/kp_yb'
  //    '<S418>/kp_yb'

  2.0,

  // Variable: Kp_yr
  //  Referenced by:
  //    '<S258>/kp_yr'
  //    '<S584>/kp_yr'
  //    '<S637>/kp_yr'
  //    '<S441>/kp_yr'
  //    '<S474>/kp_yr'
  //    '<S504>/kp_yr'
  //    '<S526>/kp_yr'

  2.0,

  // Variable: Kv
  //  Referenced by:
  //    '<S598>/MATLAB Function'
  //    '<S651>/MATLAB Function'
  //    '<S544>/MATLAB Function'

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
  //    '<S369>/Constant2'
  //    '<S387>/Constant2'
  //    '<S405>/Constant2'
  //    '<S423>/Constant2'
  //    '<S448>/Constant2'
  //    '<S480>/Constant2'
  //    '<S509>/Constant2'
  //    '<S533>/Constant2'
  //    '<S294>/Constant1'
  //    '<S311>/Constant1'
  //    '<S328>/Constant1'
  //    '<S345>/Constant1'
  //    '<S467>/Constant1'
  //    '<S481>/Constant1'

  35.0,

  // Variable: Phase3_End
  //  Referenced by: '<Root>/Constant2'

  550.0,

  // Variable: Phase3_SubPhase1_End
  //  Referenced by:
  //    '<S280>/Constant4'
  //    '<S281>/Constant4'
  //    '<S282>/Constant4'
  //    '<S481>/Constant2'

  155.0,

  // Variable: Phase3_SubPhase2_End
  //  Referenced by:
  //    '<S280>/Constant1'
  //    '<S281>/Constant1'
  //    '<S282>/Constant1'
  //    '<S508>/Constant'

  165.0,

  // Variable: Phase3_SubPhase3_End
  //  Referenced by:
  //    '<S280>/Constant2'
  //    '<S281>/Constant2'
  //    '<S282>/Constant2'

  195.0,

  // Variable: Phase3_SubPhase4_End
  //  Referenced by:
  //    '<S280>/Constant3'
  //    '<S281>/Constant3'
  //    '<S282>/Constant3'

  555.0,

  // Variable: Phase4_End
  //  Referenced by: '<Root>/Constant3'

  670.0,

  // Variable: Phase5_End
  //  Referenced by: '<Root>/Constant6'

  690.0,

  // Variable: Tz_lim_elarm
  //  Referenced by:
  //    '<S261>/Saturation1'
  //    '<S587>/Saturation1'
  //    '<S640>/Saturation1'
  //    '<S446>/Saturation1'
  //    '<S479>/Saturation1'
  //    '<S508>/Saturation1'
  //    '<S531>/Saturation1'

  0.1,

  // Variable: Tz_lim_sharm
  //  Referenced by:
  //    '<S261>/Saturation'
  //    '<S587>/Saturation'
  //    '<S640>/Saturation'
  //    '<S446>/Saturation'
  //    '<S479>/Saturation'
  //    '<S508>/Saturation'
  //    '<S531>/Saturation'

  0.1,

  // Variable: Tz_lim_wrarm
  //  Referenced by:
  //    '<S261>/Saturation3'
  //    '<S587>/Saturation3'
  //    '<S640>/Saturation3'
  //    '<S446>/Saturation3'
  //    '<S479>/Saturation3'
  //    '<S508>/Saturation3'
  //    '<S531>/Saturation3'

  0.1,

  // Variable: baseRate
  //  Referenced by:
  //    '<S206>/Read Joint Positions using  Dynamixel Encoders'
  //    '<S79>/divide by delta T'
  //    '<S80>/divide by delta T'
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
  //    '<S93>/divide by delta T'
  //    '<S667>/divide by delta T'
  //    '<S668>/divide by delta T'
  //    '<S669>/divide by delta T'
  //    '<S670>/divide by delta T'
  //    '<S671>/divide by delta T'
  //    '<S679>/divide by delta T'
  //    '<S680>/divide by delta T'
  //    '<S681>/divide by delta T'
  //    '<S682>/divide by delta T'
  //    '<S683>/divide by delta T'
  //    '<S234>/divide by delta T'
  //    '<S236>/divide by delta T'
  //    '<S238>/divide by delta T'
  //    '<S247>/divide by delta T'
  //    '<S249>/divide by delta T'
  //    '<S251>/divide by delta T'
  //    '<S262>/divide by delta T'
  //    '<S264>/divide by delta T'
  //    '<S266>/divide by delta T'
  //    '<S560>/divide by delta T'
  //    '<S562>/divide by delta T'
  //    '<S564>/divide by delta T'
  //    '<S573>/divide by delta T'
  //    '<S575>/divide by delta T'
  //    '<S577>/divide by delta T'
  //    '<S588>/divide by delta T'
  //    '<S590>/divide by delta T'
  //    '<S592>/divide by delta T'
  //    '<S613>/divide by delta T'
  //    '<S615>/divide by delta T'
  //    '<S617>/divide by delta T'
  //    '<S626>/divide by delta T'
  //    '<S628>/divide by delta T'
  //    '<S630>/divide by delta T'
  //    '<S641>/divide by delta T'
  //    '<S643>/divide by delta T'
  //    '<S645>/divide by delta T'
  //    '<S274>/divide by delta T'
  //    '<S276>/divide by delta T'
  //    '<S278>/divide by delta T'
  //    '<S297>/divide by delta T'
  //    '<S299>/divide by delta T'
  //    '<S301>/divide by delta T'
  //    '<S314>/divide by delta T'
  //    '<S316>/divide by delta T'
  //    '<S318>/divide by delta T'
  //    '<S331>/divide by delta T'
  //    '<S333>/divide by delta T'
  //    '<S335>/divide by delta T'
  //    '<S348>/divide by delta T'
  //    '<S350>/divide by delta T'
  //    '<S352>/divide by delta T'
  //    '<S370>/divide by delta T'
  //    '<S372>/divide by delta T'
  //    '<S374>/divide by delta T'
  //    '<S388>/divide by delta T'
  //    '<S390>/divide by delta T'
  //    '<S392>/divide by delta T'
  //    '<S406>/divide by delta T'
  //    '<S408>/divide by delta T'
  //    '<S410>/divide by delta T'
  //    '<S424>/divide by delta T'
  //    '<S426>/divide by delta T'
  //    '<S428>/divide by delta T'
  //    '<S449>/divide by delta T'
  //    '<S451>/divide by delta T'
  //    '<S453>/divide by delta T'
  //    '<S484>/divide by delta T'
  //    '<S486>/divide by delta T'
  //    '<S488>/divide by delta T'
  //    '<S510>/divide by delta T'
  //    '<S512>/divide by delta T'
  //    '<S534>/divide by delta T'
  //    '<S536>/divide by delta T'
  //    '<S538>/divide by delta T'
  //    '<S699>/divide by delta T'
  //    '<S700>/divide by delta T'
  //    '<S705>/divide by delta T'
  //    '<S706>/divide by delta T'
  //    '<S707>/divide by delta T'
  //    '<S708>/divide by delta T'
  //    '<S709>/divide by delta T'
  //    '<S461>/divide by delta T'
  //    '<S463>/divide by delta T'
  //    '<S465>/divide by delta T'
  //    '<S496>/divide by delta T'
  //    '<S498>/divide by delta T'
  //    '<S500>/divide by delta T'
  //    '<S517>/divide by delta T'
  //    '<S519>/divide by delta T'
  //    '<S521>/divide by delta T'

  0.05,

  // Variable: drop_states_BLACK
  //  Referenced by: '<S666>/Velocity to Position'

  { 1.755775, 1.209675, 0.0 },

  // Variable: drop_states_BLUE
  //  Referenced by: '<S678>/Velocity to Position'

  { 1.255775, 1.209675, 0.0 },

  // Variable: drop_states_RED
  //  Referenced by: '<S704>/Velocity to Position'

  { 2.255775, 1.209675, 3.1415926535897931 },

  // Variable: home_states_BLACK
  //  Referenced by:
  //    '<S552>/Desired Attitude (BLACK)'
  //    '<S552>/Desired Px (BLACK)'
  //    '<S552>/Desired Py (BLACK)'
  //    '<S605>/Constant'
  //    '<S605>/Constant2'
  //    '<S605>/Constant3'

  { 1.755775, 1.209675, 0.0 },

  // Variable: home_states_BLUE
  //  Referenced by:
  //    '<S553>/Desired Attitude (BLUE)'
  //    '<S553>/Desired Px (BLUE)'
  //    '<S553>/Desired Py (BLUE)'
  //    '<S606>/Desired Attitude (BLUE)'
  //    '<S606>/Desired Px (BLUE)'
  //    '<S606>/Desired Py (BLUE)'

  { 0.85577500000000006, 1.209675, 0.0 },

  // Variable: home_states_RED
  //  Referenced by:
  //    '<S554>/Desired Px (BLACK)'
  //    '<S554>/Desired Px (BLACK)1'
  //    '<S554>/Desired Px (BLACK)2'
  //    '<S607>/Desired Px (BLACK)'
  //    '<S607>/Desired Px (BLACK)1'
  //    '<S607>/Desired Px (BLACK)2'

  { 2.655775, 1.209675, 3.1415926535897931 },

  // Variable: init_states_BLACK
  //  Referenced by:
  //    '<S226>/Desired Attitude (BLACK)'
  //    '<S226>/Desired Px (BLACK)'
  //    '<S226>/Desired Py (BLACK)'

  { 1.755775, 1.209675, 0.0 },

  // Variable: init_states_BLUE
  //  Referenced by:
  //    '<S227>/Desired Attitude (BLUE)'
  //    '<S227>/Desired Px (BLUE)'
  //    '<S227>/Desired Py (BLUE)'

  { 0.85577500000000006, 1.209675, 0.0 },

  // Variable: init_states_RED
  //  Referenced by:
  //    '<S228>/Constant'
  //    '<S228>/Constant1'
  //    '<S228>/Constant3'

  { 2.655775, 1.209675, 3.1415926535897931 },

  // Variable: model_param
  //  Referenced by:
  //    '<S58>/MATLAB Function'
  //    '<S58>/MATLAB Function4'
  //    '<S58>/MATLAB Function6'
  //    '<S666>/MATLAB Function'
  //    '<S678>/MATLAB Function'
  //    '<S597>/End-Effector Position'
  //    '<S650>/End-Effector Position'
  //    '<S695>/Coriolis Function'
  //    '<S695>/Inertia Function '
  //    '<S543>/End-Effector Position'
  //    '<S601>/Inertia Function '
  //    '<S601>/MATLAB Function'
  //    '<S654>/Inertia Function '
  //    '<S654>/MATLAB Function'
  //    '<S704>/MATLAB Function'
  //    '<S547>/Inertia Function '
  //    '<S547>/MATLAB Function'

  { 13.072, 0.2405, 12.039, 0.225692, 11.267, 0.204245, 1.2788882640695931, 0.25,
    0.196822, 0.107678, 0.198152, 0.106348, 0.062097, 0.025153, 0.345, 0.335,
    0.111, 0.003704, 0.003506, 0.000106 },

  // Variable: noise_variance_BLACK
  //  Referenced by: '<S666>/Random Number'

  0.0,

  // Variable: noise_variance_BLUE
  //  Referenced by: '<S678>/Random Number'

  0.0,

  // Variable: noise_variance_RED
  //  Referenced by: '<S704>/Random Number'

  0.0,

  // Variable: platformSelection
  //  Referenced by:
  //    '<S228>/Constant4'
  //    '<S554>/Constant3'
  //    '<S607>/Constant3'
  //    '<S665>/Constant'
  //    '<S58>/Constant'
  //    '<S58>/Constant1'
  //    '<S434>/Constant4'
  //    '<S435>/Constant4'
  //    '<S436>/Constant1'
  //    '<S437>/Constant3'

  8.0,

  // Variable: simMode
  //  Referenced by:
  //    '<S7>/Constant'
  //    '<S22>/Constant'
  //    '<S28>/Constant'
  //    '<S32>/Constant'
  //    '<S77>/Constant'
  //    '<S112>/Constant'
  //    '<S207>/Constant'
  //    '<S217>/Constant'
  //    '<S221>/Constant'
  //    '<S225>/Constant'
  //    '<S229>/Constant'
  //    '<S283>/Constant'
  //    '<S555>/Constant'
  //    '<S608>/Constant'
  //    '<S661>/Constant'

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
  //    '<S284>/Desired X-Position (BLACK)'
  //    '<S285>/Desired X-Position (BLACK)'
  //    '<S286>/Desired X-Position (BLACK)'
  //    '<S287>/Desired X-Position (BLACK)'
  //    '<S357>/MATLAB Function2'
  //    '<S358>/MATLAB Function2'
  //    '<S359>/MATLAB Function2'
  //    '<S360>/MATLAB Function2'
  //    '<S434>/MATLAB Function'
  //    '<S435>/MATLAB Function'
  //    '<S436>/MATLAB Function'
  //    '<S437>/MATLAB Function'

  3.51155,

  // Variable: yLength
  //  Referenced by:
  //    '<S284>/Desired Y-Position (BLACK)'
  //    '<S285>/Desired Y-Position (BLACK)'
  //    '<S286>/Desired Y-Position (BLACK)'
  //    '<S287>/Desired Y-Position (BLACK)'
  //    '<S357>/MATLAB Function3'
  //    '<S358>/MATLAB Function3'
  //    '<S359>/MATLAB Function3'
  //    '<S360>/MATLAB Function3'
  //    '<S434>/MATLAB Function1'
  //    '<S435>/MATLAB Function1'
  //    '<S436>/MATLAB Function1'
  //    '<S437>/MATLAB Function1'

  2.41935,

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

  // Computed Parameter: Out1_Y0
  //  Referenced by: '<S94>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_p
  //  Referenced by: '<S95>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a
  //  Referenced by: '<S96>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pf
  //  Referenced by: '<S97>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k
  //  Referenced by: '<S98>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f
  //  Referenced by: '<S99>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_m
  //  Referenced by: '<S100>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_po
  //  Referenced by: '<S101>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_n
  //  Referenced by: '<S102>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_pl
  //  Referenced by: '<S103>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_aq
  //  Referenced by: '<S104>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_au
  //  Referenced by: '<S105>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_b
  //  Referenced by: '<S106>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bw
  //  Referenced by: '<S107>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c
  //  Referenced by: '<S108>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_e
  //  Referenced by: '<S123>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_by
  //  Referenced by: '<S126>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bo
  //  Referenced by: '<S130>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_b1
  //  Referenced by: '<S136>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ms
  //  Referenced by: '<S139>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_g
  //  Referenced by: '<S142>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S129>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S109>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S109>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainval
  //  Referenced by: '<S121>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: Out1_Y0_bi
  //  Referenced by: '<S154>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_j
  //  Referenced by: '<S157>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bf
  //  Referenced by: '<S161>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_jp
  //  Referenced by: '<S167>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nw
  //  Referenced by: '<S170>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_au3
  //  Referenced by: '<S173>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S160>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S110>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S110>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainv_a
  //  Referenced by: '<S152>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: Out1_Y0_b3
  //  Referenced by: '<S185>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_l
  //  Referenced by: '<S188>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fj
  //  Referenced by: '<S192>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_np
  //  Referenced by: '<S198>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ki
  //  Referenced by: '<S201>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_k2
  //  Referenced by: '<S204>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S191>/Constant'

  0.0,

  // Expression: pi/180
  //  Referenced by: '<S111>/Gain'

  0.017453292519943295,

  // Expression: 9.81
  //  Referenced by: '<S111>/Gain1'

  9.81,

  // Computed Parameter: DiscreteTimeIntegrator1_gainv_k
  //  Referenced by: '<S183>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: Out1_Y0_i
  //  Referenced by: '<S673>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_bu
  //  Referenced by: '<S674>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_jq
  //  Referenced by: '<S675>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_mq
  //  Referenced by: '<S676>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_eo
  //  Referenced by: '<S677>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_eom
  //  Referenced by: '<S685>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_d
  //  Referenced by: '<S686>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ed
  //  Referenced by: '<S687>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_p0
  //  Referenced by: '<S688>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ia
  //  Referenced by: '<S689>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h
  //  Referenced by: '<S701>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hv
  //  Referenced by: '<S702>/Out1'

  0.0,

  // Expression: 0.0
  //  Referenced by: '<S695>/Delay'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainval
  //  Referenced by: '<S695>/Acceleration  to Velocity'

  0.05,

  // Expression: [drop_states_RED,drop_states_ARM,0,0,0,0,0,0]
  //  Referenced by: '<S695>/Acceleration  to Velocity'

  { 2.255775, 1.209675, 3.1415926535897931, 1.5707963267948966,
    1.5707963267948966, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },

  // Expression: 0
  //  Referenced by: '<S695>/Random Number2'

  0.0,

  // Computed Parameter: RandomNumber2_StdDev
  //  Referenced by: '<S695>/Random Number2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S695>/Random Number2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S695>/Random Number1'

  0.0,

  // Computed Parameter: RandomNumber1_StdDev
  //  Referenced by: '<S695>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S695>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S695>/Random Number3'

  0.0,

  // Computed Parameter: RandomNumber3_StdDev
  //  Referenced by: '<S695>/Random Number3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S695>/Random Number3'

  0.0,

  // Computed Parameter: Out1_Y0_lt
  //  Referenced by: '<S711>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f2
  //  Referenced by: '<S712>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fu
  //  Referenced by: '<S713>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lh
  //  Referenced by: '<S714>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a3
  //  Referenced by: '<S715>/Out1'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_k
  //  Referenced by: '<S704>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S704>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval
  //  Referenced by: '<S704>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S704>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S704>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number7'

  0.0,

  // Computed Parameter: RandomNumber7_StdDev
  //  Referenced by: '<S703>/Random Number7'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number6'

  0.0,

  // Computed Parameter: RandomNumber6_StdDev
  //  Referenced by: '<S703>/Random Number6'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number8'

  0.0,

  // Computed Parameter: RandomNumber8_StdDev
  //  Referenced by: '<S703>/Random Number8'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S703>/Random Number8'

  0.0,

  // Expression: 0
  //  Referenced by: '<S690>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S691>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S692>/Constant'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_i
  //  Referenced by: '<S666>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S666>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_c
  //  Referenced by: '<S666>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S666>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S666>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number1'

  0.0,

  // Computed Parameter: RandomNumber1_StdDev_o
  //  Referenced by: '<S662>/Random Number1'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number'

  0.0,

  // Computed Parameter: RandomNumber_StdDev
  //  Referenced by: '<S662>/Random Number'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number2'

  0.0,

  // Computed Parameter: RandomNumber2_StdDev_p
  //  Referenced by: '<S662>/Random Number2'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S662>/Random Number2'

  0.0,

  // Computed Parameter: AccelerationtoVelocity_gainva_b
  //  Referenced by: '<S678>/Acceleration  to Velocity'

  0.05,

  // Expression: 0
  //  Referenced by: '<S678>/Acceleration  to Velocity'

  0.0,

  // Computed Parameter: VelocitytoPosition_gainval_f
  //  Referenced by: '<S678>/Velocity to Position'

  0.05,

  // Expression: 0
  //  Referenced by: '<S678>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S678>/Random Number'

  0.0,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number4'

  0.0,

  // Computed Parameter: RandomNumber4_StdDev
  //  Referenced by: '<S663>/Random Number4'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number3'

  0.0,

  // Computed Parameter: RandomNumber3_StdDev_j
  //  Referenced by: '<S663>/Random Number3'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number5'

  0.0,

  // Computed Parameter: RandomNumber5_StdDev
  //  Referenced by: '<S663>/Random Number5'

  0.0012247448713915891,

  // Expression: 0
  //  Referenced by: '<S663>/Random Number5'

  0.0,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<S664>/Discrete-Time Integrator'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator1_gain_aq
  //  Referenced by: '<S664>/Discrete-Time Integrator1'

  0.05,

  // Computed Parameter: DiscreteTimeIntegrator2_gainval
  //  Referenced by: '<S664>/Discrete-Time Integrator2'

  0.05,

  // Expression: 0
  //  Referenced by: '<S208>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S208>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S208>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S208>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S208>/Delay4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S209>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S209>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S209>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S209>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S209>/Delay4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S210>/Delay1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S210>/Delay2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S210>/Delay5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S210>/Delay3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S210>/Delay4'

  0.0,

  // Expression: 0.1
  //  Referenced by: '<S17>/WhoAmI'

  0.1,

  // Computed Parameter: Out1_Y0_aw
  //  Referenced by: '<S235>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_av
  //  Referenced by: '<S237>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h4
  //  Referenced by: '<S239>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S226>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_mf
  //  Referenced by: '<S248>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a4
  //  Referenced by: '<S250>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_kw
  //  Referenced by: '<S252>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S227>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_pj
  //  Referenced by: '<S263>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lk
  //  Referenced by: '<S265>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fa
  //  Referenced by: '<S267>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S260>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S228>/Constant2'

  0.0,

  // Expression: 1
  //  Referenced by: '<S228>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_mm
  //  Referenced by: '<S450>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fjl
  //  Referenced by: '<S452>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hd
  //  Referenced by: '<S454>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S445>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S434>/Constant1'

  0.0,

  // Expression: 0.03490659
  //  Referenced by: '<S447>/Desired Rate (BLACK)'

  0.03490659,

  // Expression: pi
  //  Referenced by: '<S447>/Constant'

  3.1415926535897931,

  // Expression: 1
  //  Referenced by: '<S434>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_b5
  //  Referenced by: '<S485>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lha
  //  Referenced by: '<S487>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hh
  //  Referenced by: '<S489>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S478>/Constant7'

  0.0,

  // Expression: 0.03490659
  //  Referenced by: '<S471>/Desired Rate (BLACK)'

  0.03490659,

  // Expression: pi
  //  Referenced by: '<S471>/Constant'

  3.1415926535897931,

  // Expression: 0
  //  Referenced by: '<S435>/Constant1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S435>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_mv
  //  Referenced by: '<S511>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_lz
  //  Referenced by: '<S513>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_gm
  //  Referenced by: '<S518>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_io
  //  Referenced by: '<S520>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_dk
  //  Referenced by: '<S522>/Out1'

  0.0,

  // Expression: [1.57000000000000
  // 1.57050205302399
  // 1.57012603662556
  // 1.56661162770156
  // 1.55794488562962
  // 1.54285929753597
  // 1.52042643744410
  // 1.48973080887809
  // 1.45048425411524
  // 1.40334397159352
  // 1.34904540279350
  // 1.28832398919572
  // 1.22195859634965
  // 1.15108368811919
  // 1.07700776270488
  // 1.00104050705195
  // 0.924491608105622
  // 0.848670299127955
  // 0.774774399465782
  // 0.703745216061739
  // 0.636488084287358
  // 0.573908339514172
  // 0.516911317113714
  // 0.466402352457517
  // 0.423202448662068
  // 0.387397731516399
  // 0.358695572215780
  // 0.336800200665272
  // 0.321415846769937
  // 0.312246740434836
  // 0.308997111565029
  // 0.311335869993361
  // 0.318438699519253
  // 0.329118863211428
  // 0.342181246399115
  // 0.356430734411547
  // 0.370672212577955
  // 0.383710566227569
  // 0.394350680689622
  // 0.401518359647600
  // 0.404656733001405
  // 0.403347252632020
  // 0.397171371877986
  // 0.385710544077849
  // 0.368546222570152
  // 0.345259860693440
  // 0.315432914518483
  // 0.278792464474893
  // 0.235602325706072
  // 0.186249942164213
  // 0.131122757801505
  // 0.0706082165701402
  // 0.00509376242231024
  // -0.0650331606897945
  // -0.139385108813982
  // -0.217505825480346
  // -0.29864030
  { 1.57, 1.57050205302399, 1.57012603662556, 1.56661162770156, 1.55794488562962,
    1.54285929753597, 1.5204264374441, 1.48973080887809, 1.45048425411524,
    1.40334397159352, 1.3490454027935, 1.28832398919572, 1.22195859634965,
    1.15108368811919, 1.07700776270488, 1.00104050705195, 0.924491608105622,
    0.848670299127955, 0.774774399465782, 0.703745216061739, 0.636488084287358,
    0.573908339514172, 0.516911317113714, 0.466402352457517, 0.423202448662068,
    0.387397731516399, 0.35869557221578, 0.336800200665272, 0.321415846769937,
    0.312246740434836, 0.308997111565029, 0.311335869993361, 0.318438699519253,
    0.329118863211428, 0.342181246399115, 0.356430734411547, 0.370672212577955,
    0.383710566227569, 0.394350680689622, 0.4015183596476, 0.404656733001405,
    0.40334725263202, 0.397171371877986, 0.385710544077849, 0.368546222570152,
    0.34525986069344, 0.315432914518483, 0.278792464474893, 0.235602325706072,
    0.186249942164213, 0.131122757801505, 0.0706082165701402,
    0.00509376242231024, -0.0650331606897945, -0.139385108813982,
    -0.217505825480346, -0.298640304539486, -0.381952484472539,
    -0.466606302239867, -0.551765694801835, -0.636594599118806,
    -0.720256952151143, -0.801916690711806, -0.880723763116082,
    -0.955775800494991, -1.02615820551012, -1.09095638082305, -1.14925572909537,
    -1.20014165298867, -1.24269955516454, -1.27602769714638, -1.2997806143903,
    -1.31436988730625, -1.32026130853269, -1.31792067070809, -1.30781376647092,
    -1.29040638845964, -1.26616853457129, -1.23607724309064, -1.20209333571039,
    -1.16629053007224, -1.13074254381788, -1.09752309458901, -1.06870590002731,
    -1.0463035949636, -1.03189323418052, -1.02686268208325, -1.03259903268412,
    -1.05048937999544, -1.08191093831646, -1.12679451437392, -1.18211552054693,
    -1.2444884697317, -1.31052787482445, -1.37689087303085, -1.44074960138572,
    -1.49961795103105, -1.55095978791926, -1.5808144508849, -1.54996443310863,
    -1.48 },

  // Expression: [0
  // 0.300000000000000
  // 0.600000000000000
  // 0.900000000000000
  // 1.20000000000000
  // 1.50000000000000
  // 1.80000000000000
  // 2.10000000000000
  // 2.40000000000000
  // 2.70000000000000
  // 3
  // 3.30000000000000
  // 3.60000000000000
  // 3.90000000000000
  // 4.20000000000000
  // 4.50000000000000
  // 4.80000000000000
  // 5.10000000000000
  // 5.40000000000000
  // 5.70000000000000
  // 6
  // 6.30000000000000
  // 6.60000000000000
  // 6.90000000000000
  // 7.20000000000000
  // 7.50000000000000
  // 7.80000000000000
  // 8.10000000000000
  // 8.40000000000000
  // 8.70000000000000
  // 9
  // 9.30000000000000
  // 9.60000000000000
  // 9.90000000000000
  // 10.2000000000000
  // 10.5000000000000
  // 10.8000000000000
  // 11.1000000000000
  // 11.4000000000000
  // 11.7000000000000
  // 12
  // 12.3000000000000
  // 12.6000000000000
  // 12.9000000000000
  // 13.2000000000000
  // 13.5000000000000
  // 13.8000000000000
  // 14.1000000000000
  // 14.4000000000000
  // 14.7000000000000
  // 15
  // 15.3000000000000
  // 15.6000000000000
  // 15.9000000000000
  // 16.2000000000000
  // 16.5000000000000
  // 16.8000000000000
  // 17.1000000000000
  // 17.4000000000000
  // 17.7000000000000
  // 18
  // 18.3000000000000
  // 18.6000000000000
  // 18.9000000000000
  // 19.2000000000000
  // 19.5
  { 0.0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.2,
    4.5, 4.8, 5.1, 5.4, 5.7, 6.0, 6.3, 6.6, 6.9, 7.2, 7.5, 7.8, 8.1, 8.4, 8.7,
    9.0, 9.3, 9.6, 9.9, 10.2, 10.5, 10.8, 11.1, 11.4, 11.7, 12.0, 12.3, 12.6,
    12.9, 13.2, 13.5, 13.8, 14.1, 14.4, 14.7, 15.0, 15.3, 15.6, 15.9, 16.2, 16.5,
    16.8, 17.1, 17.4, 17.7, 18.0, 18.3, 18.6, 18.9, 19.2, 19.5, 19.8, 20.1, 20.4,
    20.7, 21.0, 21.3, 21.6, 21.9, 22.2, 22.5, 22.8, 23.1, 23.4, 23.7, 24.0, 24.3,
    24.6, 24.9, 25.2, 25.5, 25.8, 26.1, 26.4, 26.7, 27.0, 27.3, 27.6, 27.9, 28.2,
    28.5, 28.8, 29.1, 29.4, 29.7, 30.0 },

  // Expression: [1.57000000000000
  // 1.54816700100684
  // 1.49097838714515
  // 1.40738374526561
  // 1.30517963261499
  // 1.19037595061406
  // 1.06817443571262
  // 0.943755752477061
  // 0.821348547944454
  // 0.703746844506339
  // 0.593625926874354
  // 0.493661079760138
  // 0.406301951764504
  // 0.332150464577563
  // 0.270904238844892
  // 0.222254718367453
  // 0.185893346946205
  // 0.161510391400890
  // 0.148507079640713
  // 0.145619174532266
  // 0.151489118608687
  // 0.164759354403110
  // 0.184072324448674
  // 0.208070471278512
  // 0.235433340233479
  // 0.265163793185692
  // 0.296431328827974
  // 0.328406827894775
  // 0.360261171120541
  // 0.391165239239719
  // 0.420289912986756
  // 0.446835197706220
  // 0.470407808441224
  // 0.490913309071205
  // 0.508264171683338
  // 0.522372868364801
  // 0.533151871202771
  // 0.540513652284423
  // 0.544370683696935
  // 0.544708678506983
  // 0.541826697403650
  // 0.536107583537685
  // 0.527934180942696
  // 0.517689333652287
  // 0.505755885700064
  // 0.492516681119634
  // 0.478354564411779
  // 0.463677280032886
  // 0.448984347405091
  // 0.434796424940426
  // 0.421634171050923
  // 0.410018244148617
  // 0.400469302645538
  // 0.393508004953720
  // 0.389655009485195
  // 0.389318793355856
  // 0.3924207
  { 1.57, 1.54816700100684, 1.49097838714515, 1.40738374526561, 1.30517963261499,
    1.19037595061406, 1.06817443571262, 0.943755752477061, 0.821348547944454,
    0.703746844506339, 0.593625926874354, 0.493661079760138, 0.406301951764504,
    0.332150464577563, 0.270904238844892, 0.222254718367453, 0.185893346946205,
    0.16151039140089, 0.148507079640713, 0.145619174532266, 0.151489118608687,
    0.16475935440311, 0.184072324448674, 0.208070471278512, 0.235433340233479,
    0.265163793185692, 0.296431328827974, 0.328406827894775, 0.360261171120541,
    0.391165239239719, 0.420289912986756, 0.44683519770622, 0.470407808441224,
    0.490913309071205, 0.508264171683338, 0.522372868364801, 0.533151871202771,
    0.540513652284423, 0.544370683696935, 0.544708678506983, 0.54182669740365,
    0.536107583537685, 0.527934180942696, 0.517689333652287, 0.505755885700064,
    0.492516681119634, 0.478354564411779, 0.463677280032886, 0.448984347405091,
    0.434796424940426, 0.421634171050923, 0.410018244148617, 0.400469302645538,
    0.39350800495372, 0.389655009485195, 0.389318793355856, 0.392420798366228,
    0.398750326166947, 0.408096675929411, 0.420249146825022, 0.434997038025178,
    0.45212964870128, 0.471436276672662, 0.49257790950895, 0.514735652583953,
    0.5369784449758, 0.558375225762617, 0.577994934022531, 0.594906508833669,
    0.608178889274159, 0.616885200503004, 0.620279657398225, 0.617862923664403,
    0.609153311284521, 0.593669132241562, 0.570928698518508, 0.540450322098342,
    0.501754708369804, 0.454651142688837, 0.399508826722755, 0.336761216407457,
    0.266841767678842, 0.19018393647281, 0.107221178725261, 0.0184637402812976,
    -0.0750305460272099, -0.17196600775858, -0.271046003976081,
    -0.370973893742981, -0.470452548764951, -0.568113490727538,
    -0.662442454298735, -0.75190737129079, -0.834976173515947, -0.91013022188517,
    -0.976013131751286, -1.03137619058426, -1.07497442671845, -1.10593044052833,
    -1.12417944460303, -1.13 },

  // Expression: [0
  // 0.300000000000000
  // 0.600000000000000
  // 0.900000000000000
  // 1.20000000000000
  // 1.50000000000000
  // 1.80000000000000
  // 2.10000000000000
  // 2.40000000000000
  // 2.70000000000000
  // 3
  // 3.30000000000000
  // 3.60000000000000
  // 3.90000000000000
  // 4.20000000000000
  // 4.50000000000000
  // 4.80000000000000
  // 5.10000000000000
  // 5.40000000000000
  // 5.70000000000000
  // 6
  // 6.30000000000000
  // 6.60000000000000
  // 6.90000000000000
  // 7.20000000000000
  // 7.50000000000000
  // 7.80000000000000
  // 8.10000000000000
  // 8.40000000000000
  // 8.70000000000000
  // 9
  // 9.30000000000000
  // 9.60000000000000
  // 9.90000000000000
  // 10.2000000000000
  // 10.5000000000000
  // 10.8000000000000
  // 11.1000000000000
  // 11.4000000000000
  // 11.7000000000000
  // 12
  // 12.3000000000000
  // 12.6000000000000
  // 12.9000000000000
  // 13.2000000000000
  // 13.5000000000000
  // 13.8000000000000
  // 14.1000000000000
  // 14.4000000000000
  // 14.7000000000000
  // 15
  // 15.3000000000000
  // 15.6000000000000
  // 15.9000000000000
  // 16.2000000000000
  // 16.5000000000000
  // 16.8000000000000
  // 17.1000000000000
  // 17.4000000000000
  // 17.7000000000000
  // 18
  // 18.3000000000000
  // 18.6000000000000
  // 18.9000000000000
  // 19.2000000000000
  // 19.5
  { 0.0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.2,
    4.5, 4.8, 5.1, 5.4, 5.7, 6.0, 6.3, 6.6, 6.9, 7.2, 7.5, 7.8, 8.1, 8.4, 8.7,
    9.0, 9.3, 9.6, 9.9, 10.2, 10.5, 10.8, 11.1, 11.4, 11.7, 12.0, 12.3, 12.6,
    12.9, 13.2, 13.5, 13.8, 14.1, 14.4, 14.7, 15.0, 15.3, 15.6, 15.9, 16.2, 16.5,
    16.8, 17.1, 17.4, 17.7, 18.0, 18.3, 18.6, 18.9, 19.2, 19.5, 19.8, 20.1, 20.4,
    20.7, 21.0, 21.3, 21.6, 21.9, 22.2, 22.5, 22.8, 23.1, 23.4, 23.7, 24.0, 24.3,
    24.6, 24.9, 25.2, 25.5, 25.8, 26.1, 26.4, 26.7, 27.0, 27.3, 27.6, 27.9, 28.2,
    28.5, 28.8, 29.1, 29.4, 29.7, 30.0 },

  // Expression: [4.24105195407198e-14
  // 0.00792484586540242
  // 0.0289766808051712
  // 0.0591226460349239
  // 0.0945266677508536
  // 0.132723049258546
  // 0.171865961865416
  // 0.210118613330810
  // 0.245977670299252
  // 0.278442298917847
  // 0.306553255045181
  // 0.329351294539841
  // 0.345978769405532
  // 0.356409999236062
  // 0.361026479142565
  // 0.360212485455902
  // 0.354352294506931
  // 0.343830913350855
  // 0.329212797758318
  // 0.311475554957133
  // 0.291654729751521
  // 0.270785866945703
  // 0.249904511343898
  // 0.230046207750326
  // 0.212190891231227
  // 0.196833909602021
  // 0.184220855187696
  // 0.174595248907400
  // 0.168200611680283
  // 0.165280464425496
  // 0.166078328062186
  // 0.170819192753770
  // 0.179469276510222
  // 0.191804652485855
  // 0.207596998435017
  // 0.226617992112058
  // 0.248639311271326
  // 0.273432633667171
  // 0.300769637053943
  // 0.330372138422153
  // 0.361748634952746
  // 0.394350586665731
  // 0.427629452980086
  // 0.461036693314793
  // 0.494023767088832
  // 0.526042133721183
  // 0.556543253290674
  // 0.585013755003088
  // 0.611069892609436
  // 0.634357776934821
  // 0.654523518804344
  // 0.671213229043108
  // 0.684073018476216
  // 0.692748997928769
  // 0.696887278225870
  // 0.6962393946
  { 4.24105195407198E-14, 0.00792484586540242, 0.0289766808051712,
    0.0591226460349239, 0.0945266677508536, 0.132723049258546, 0.171865961865416,
    0.21011861333081, 0.245977670299252, 0.278442298917847, 0.306553255045181,
    0.329351294539841, 0.345978769405532, 0.356409999236062, 0.361026479142565,
    0.360212485455902, 0.354352294506931, 0.343830913350855, 0.329212797758318,
    0.311475554957133, 0.291654729751521, 0.270785866945703, 0.249904511343898,
    0.230046207750326, 0.212190891231227, 0.196833909602021, 0.184220855187696,
    0.1745952489074, 0.168200611680283, 0.165280464425496, 0.166078328062186,
    0.17081919275377, 0.179469276510222, 0.191804652485855, 0.207596998435017,
    0.226617992112058, 0.248639311271326, 0.273432633667171, 0.300769637053943,
    0.330372138422153, 0.361748634952746, 0.394350586665731, 0.427629452980086,
    0.461036693314793, 0.494023767088832, 0.526042133721183, 0.556543253290674,
    0.585013755003088, 0.611069892609436, 0.634357776934821, 0.654523518804344,
    0.671213229043108, 0.684073018476216, 0.692748997928769, 0.69688727822587,
    0.696239394653257, 0.691014582997561, 0.681546260210274, 0.6681678455728,
    0.651212758366539, 0.631014417872893, 0.607906243373263, 0.582221655279522,
    0.55440135514419, 0.525287277542556, 0.495815140112214, 0.466920660490759,
    0.439539556315785, 0.414607545224887, 0.393060344855659, 0.375828988968287,
    0.363641886942561, 0.356951692355833, 0.356191311825241, 0.361793651967925,
    0.374191619401025, 0.39381812074168, 0.421102648855173, 0.456063090510969,
    0.497918715134657, 0.545797145289127, 0.598826003537267, 0.656132912441967,
    0.716845494566115, 0.780034633378752, 0.844366607241886, 0.908331957834384,
    0.970420511226358, 1.02912209348792, 1.08292634428082, 1.13029561275212,
    1.16963648628818, 1.19934874289962, 1.21783216059706, 1.22367347867057,
    1.21771835926845, 1.20231148671538, 1.17980152164021, 1.14786929261614,
    1.09387618894762, 1.04 },

  // Expression: [0
  // 0.300000000000000
  // 0.600000000000000
  // 0.900000000000000
  // 1.20000000000000
  // 1.50000000000000
  // 1.80000000000000
  // 2.10000000000000
  // 2.40000000000000
  // 2.70000000000000
  // 3
  // 3.30000000000000
  // 3.60000000000000
  // 3.90000000000000
  // 4.20000000000000
  // 4.50000000000000
  // 4.80000000000000
  // 5.10000000000000
  // 5.40000000000000
  // 5.70000000000000
  // 6
  // 6.30000000000000
  // 6.60000000000000
  // 6.90000000000000
  // 7.20000000000000
  // 7.50000000000000
  // 7.80000000000000
  // 8.10000000000000
  // 8.40000000000000
  // 8.70000000000000
  // 9
  // 9.30000000000000
  // 9.60000000000000
  // 9.90000000000000
  // 10.2000000000000
  // 10.5000000000000
  // 10.8000000000000
  // 11.1000000000000
  // 11.4000000000000
  // 11.7000000000000
  // 12
  // 12.3000000000000
  // 12.6000000000000
  // 12.9000000000000
  // 13.2000000000000
  // 13.5000000000000
  // 13.8000000000000
  // 14.1000000000000
  // 14.4000000000000
  // 14.7000000000000
  // 15
  // 15.3000000000000
  // 15.6000000000000
  // 15.9000000000000
  // 16.2000000000000
  // 16.5000000000000
  // 16.8000000000000
  // 17.1000000000000
  // 17.4000000000000
  // 17.7000000000000
  // 18
  // 18.3000000000000
  // 18.6000000000000
  // 18.9000000000000
  // 19.2000000000000
  // 19.5
  { 0.0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.2,
    4.5, 4.8, 5.1, 5.4, 5.7, 6.0, 6.3, 6.6, 6.9, 7.2, 7.5, 7.8, 8.1, 8.4, 8.7,
    9.0, 9.3, 9.6, 9.9, 10.2, 10.5, 10.8, 11.1, 11.4, 11.7, 12.0, 12.3, 12.6,
    12.9, 13.2, 13.5, 13.8, 14.1, 14.4, 14.7, 15.0, 15.3, 15.6, 15.9, 16.2, 16.5,
    16.8, 17.1, 17.4, 17.7, 18.0, 18.3, 18.6, 18.9, 19.2, 19.5, 19.8, 20.1, 20.4,
    20.7, 21.0, 21.3, 21.6, 21.9, 22.2, 22.5, 22.8, 23.1, 23.4, 23.7, 24.0, 24.3,
    24.6, 24.9, 25.2, 25.5, 25.8, 26.1, 26.4, 26.7, 27.0, 27.3, 27.6, 27.9, 28.2,
    28.5, 28.8, 29.1, 29.4, 29.7, 30.0 },

  // Expression: 0
  //  Referenced by: '<S507>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S436>/Constant4'

  0.0,

  // Expression: 1
  //  Referenced by: '<S436>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_ej
  //  Referenced by: '<S535>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ma
  //  Referenced by: '<S537>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_nps
  //  Referenced by: '<S539>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S530>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S437>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S532>/Constant'

  0.0,

  // Expression: 1
  //  Referenced by: '<S437>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_o
  //  Referenced by: '<S561>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_ii
  //  Referenced by: '<S563>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_hq
  //  Referenced by: '<S565>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S552>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_lc
  //  Referenced by: '<S589>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_of
  //  Referenced by: '<S591>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_l2
  //  Referenced by: '<S593>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S586>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S554>/Constant2'

  0.0,

  // Expression: 1
  //  Referenced by: '<S554>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_by4
  //  Referenced by: '<S614>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_c2
  //  Referenced by: '<S616>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_f2b
  //  Referenced by: '<S618>/Out1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S605>/Puck State'

  1.0,

  // Computed Parameter: Out1_Y0_jf
  //  Referenced by: '<S642>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_fn
  //  Referenced by: '<S644>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_a2
  //  Referenced by: '<S646>/Out1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S639>/Constant7'

  0.0,

  // Expression: 0
  //  Referenced by: '<S607>/Constant1'

  0.0,

  // Expression: 1
  //  Referenced by: '<S607>/Puck State'

  1.0,

  // Expression: 0
  //  Referenced by: '<S658>/Constant3'

  0.0,

  // Expression: 0
  //  Referenced by: '<S658>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S658>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S658>/Puck State'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant1'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant2'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant4'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant5'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Constant6'

  0.0,

  // Expression: 0
  //  Referenced by: '<S660>/Puck State'

  0.0,

  // Expression: -1
  //  Referenced by: '<S660>/Gain'

  -1.0,

  // Expression: 0.005
  //  Referenced by: '<S660>/Saturation'

  0.005,

  // Expression: -0.005
  //  Referenced by: '<S660>/Saturation'

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
  //  Referenced by: '<S78>/UDP Receive'

  25000,

  // Computed Parameter: UDPSend_Port
  //  Referenced by: '<S18>/UDP Send'

  26000,

  // Start of '<S16>/Change BLUE Behavior'
  {
    // Expression: 0
    //  Referenced by: '<S659>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S659>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S659>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S659>/Puck State'

    0.0
  }
  ,

  // End of '<S16>/Change BLUE Behavior'

  // Start of '<S607>/RED+ARM'
  {
    // Expression: 0.9
    //  Referenced by: '<S640>/Constant2'

    0.9,

    // Expression: 0
    //  Referenced by: '<S640>/Constant'

    0.0
  }
  ,

  // End of '<S607>/RED+ARM'

  // Start of '<S15>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S627>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S629>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S631>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S606>/Puck State'

    1.0
  }
  ,

  // End of '<S15>/Change BLUE Behavior'

  // Start of '<S554>/RED+ARM'
  {
    // Expression: 0.9
    //  Referenced by: '<S587>/Constant2'

    0.9,

    // Expression: 0
    //  Referenced by: '<S587>/Constant'

    0.0
  }
  ,

  // End of '<S554>/RED+ARM'

  // Start of '<S14>/Change BLUE Behavior'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S574>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S576>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S578>/Out1'

    0.0,

    // Expression: 1
    //  Referenced by: '<S553>/Puck State'

    1.0
  }
  ,

  // End of '<S14>/Change BLUE Behavior'

  // Start of '<S437>/RED+ARM'
  {
    // Expression: 0.9
    //  Referenced by: '<S531>/Constant2'

    0.9,

    // Expression: 0
    //  Referenced by: '<S531>/Constant'

    0.0
  }
  ,

  // End of '<S437>/RED+ARM'

  // Start of '<S435>/RED+ARM'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S497>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_k
    //  Referenced by: '<S499>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_h
    //  Referenced by: '<S501>/Out1'

    0.0,

    // Expression: pi/2
    //  Referenced by: '<S479>/Constant9'

    1.5707963267948966,

    // Expression: pi/2
    //  Referenced by: '<S479>/Constant8'

    1.5707963267948966,

    // Expression: 0
    //  Referenced by: '<S479>/Constant7'

    0.0
  }
  ,

  // End of '<S435>/RED+ARM'

  // Start of '<S434>/RED+ARM'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S462>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_k
    //  Referenced by: '<S464>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_h
    //  Referenced by: '<S466>/Out1'

    0.0,

    // Expression: pi/2
    //  Referenced by: '<S446>/Constant9'

    1.5707963267948966,

    // Expression: pi/2
    //  Referenced by: '<S446>/Constant8'

    1.5707963267948966,

    // Expression: 0
    //  Referenced by: '<S446>/Constant7'

    0.0
  }
  ,

  // End of '<S434>/RED+ARM'

  // Start of '<S281>/Sub-Phase #4'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S425>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S427>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S429>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S422>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S360>/Puck State'

    1.0
  }
  ,

  // End of '<S281>/Sub-Phase #4'

  // Start of '<S281>/Sub-Phase #3 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S407>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S409>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S411>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S404>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S359>/Puck State'

    1.0
  }
  ,

  // End of '<S281>/Sub-Phase #3 '

  // Start of '<S281>/Sub-Phase #2 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S389>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S391>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S393>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S386>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S358>/Puck State'

    1.0
  }
  ,

  // End of '<S281>/Sub-Phase #2 '

  // Start of '<S281>/Sub-Phase #1'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S371>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_g
    //  Referenced by: '<S373>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S375>/Out1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S368>/Constant'

    0.0,

    // Expression: 1
    //  Referenced by: '<S357>/Puck State'

    1.0
  }
  ,

  // End of '<S281>/Sub-Phase #1'

  // Start of '<S280>/Sub-Phase #4'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S349>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S351>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S353>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S340>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S287>/Puck State'

    1.0
  }
  ,

  // End of '<S280>/Sub-Phase #4'

  // Start of '<S280>/Sub-Phase #3 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S332>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S334>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S336>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S323>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S286>/Puck State'

    1.0
  }
  ,

  // End of '<S280>/Sub-Phase #3 '

  // Start of '<S280>/Sub-Phase #2 '
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S315>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S317>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S319>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S306>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S285>/Puck State'

    1.0
  }
  ,

  // End of '<S280>/Sub-Phase #2 '

  // Start of '<S280>/Sub-Phase #1'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S298>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_n
    //  Referenced by: '<S300>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_p
    //  Referenced by: '<S302>/Out1'

    0.0,

    // Expression: 0.03490659
    //  Referenced by: '<S289>/Desired Rate (BLACK)'

    0.03490659,

    // Expression: 1
    //  Referenced by: '<S284>/Puck State'

    1.0
  }
  ,

  // End of '<S280>/Sub-Phase #1'

  // Start of '<S228>/RED+ARM'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S275>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_k
    //  Referenced by: '<S277>/Out1'

    0.0,

    // Computed Parameter: Out1_Y0_h
    //  Referenced by: '<S279>/Out1'

    0.0,

    // Expression: pi/2
    //  Referenced by: '<S261>/Constant9'

    1.5707963267948966,

    // Expression: pi/2
    //  Referenced by: '<S261>/Constant8'

    1.5707963267948966,

    // Expression: 0
    //  Referenced by: '<S261>/Constant7'

    0.0
  }
  ,

  // End of '<S228>/RED+ARM'

  // Start of '<Root>/Phase #1:  Start Floating '
  {
    // Expression: 0
    //  Referenced by: '<S222>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S222>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S222>/Constant5'

    0.0,

    // Expression: 1
    //  Referenced by: '<S222>/Puck State'

    1.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S224>/Constant6'

    0.0,

    // Expression: 1
    //  Referenced by: '<S224>/Puck State'

    1.0,

    // Start of '<S10>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S223>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S223>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S223>/Constant5'

      0.0,

      // Expression: 1
      //  Referenced by: '<S223>/Puck State'

      1.0
    }
    // End of '<S10>/Change BLUE Behavior'
  }
  ,

  // End of '<Root>/Phase #1:  Start Floating '

  // Start of '<Root>/Phase #0:  Wait for Synchronization'
  {
    // Expression: 0
    //  Referenced by: '<S218>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S218>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S218>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S218>/Puck State'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant1'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant2'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant3'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant4'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant5'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Constant6'

    0.0,

    // Expression: 0
    //  Referenced by: '<S220>/Puck State'

    0.0,

    // Start of '<S10>/Change BLUE Behavior'
    {
      // Expression: 0
      //  Referenced by: '<S219>/Constant3'

      0.0,

      // Expression: 0
      //  Referenced by: '<S219>/Constant4'

      0.0,

      // Expression: 0
      //  Referenced by: '<S219>/Constant5'

      0.0,

      // Expression: 0
      //  Referenced by: '<S219>/Puck State'

      0.0
    }
    // End of '<S10>/Change BLUE Behavior'
  }
  ,

  // End of '<Root>/Phase #0:  Wait for Synchronization'

  // Start of '<S196>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S203>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S203>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S203>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S203>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S203>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S203>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S203>/Delay4'

    0.0
  }
  ,

  // End of '<S196>/Calculate Running Mean'

  // Start of '<S195>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S200>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S200>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S200>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S200>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S200>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S200>/Delay4'

    0.0
  }
  ,

  // End of '<S195>/Calculate Running Mean'

  // Start of '<S194>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S197>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S197>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S197>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S197>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S197>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S197>/Delay4'

    0.0
  }
  ,

  // End of '<S194>/Calculate Running Mean'

  // Start of '<S183>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S190>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S190>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S190>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S190>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S190>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S190>/Delay4'

    0.0
  }
  ,

  // End of '<S183>/Calculate Running Mean'

  // Start of '<S182>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S187>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S187>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S187>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S187>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S187>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S187>/Delay4'

    0.0
  }
  ,

  // End of '<S182>/Calculate Running Mean'

  // Start of '<S181>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S184>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S184>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S184>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S184>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S184>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S184>/Delay4'

    0.0
  }
  ,

  // End of '<S181>/Calculate Running Mean'

  // Start of '<S111>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S111>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S111>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S111>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S111>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S111>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S111>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S111>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S111>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S111>/AHRS2'

    33.0
  }
  ,

  // End of '<S111>/AHRS2'

  // Start of '<S165>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S172>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S172>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S172>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S172>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S172>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S172>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S172>/Delay4'

    0.0
  }
  ,

  // End of '<S165>/Calculate Running Mean'

  // Start of '<S164>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S169>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S169>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S169>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S169>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S169>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S169>/Delay4'

    0.0
  }
  ,

  // End of '<S164>/Calculate Running Mean'

  // Start of '<S163>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S166>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S166>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S166>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S166>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S166>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S166>/Delay4'

    0.0
  }
  ,

  // End of '<S163>/Calculate Running Mean'

  // Start of '<S152>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S159>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S159>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S159>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S159>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S159>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S159>/Delay4'

    0.0
  }
  ,

  // End of '<S152>/Calculate Running Mean'

  // Start of '<S151>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S156>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S156>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S156>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S156>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S156>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S156>/Delay4'

    0.0
  }
  ,

  // End of '<S151>/Calculate Running Mean'

  // Start of '<S150>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S153>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S153>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S153>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S153>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S153>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S153>/Delay4'

    0.0
  }
  ,

  // End of '<S150>/Calculate Running Mean'

  // Start of '<S110>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S110>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S110>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S110>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S110>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S110>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S110>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S110>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S110>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S110>/AHRS2'

    33.0
  }
  ,

  // End of '<S110>/AHRS2'

  // Start of '<S134>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S141>/Out1'

    0.0,

    // Expression: 9.81
    //  Referenced by: '<S141>/Constant'

    9.81,

    // Expression: 0.0
    //  Referenced by: '<S141>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S141>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S141>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S141>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S141>/Delay4'

    0.0
  }
  ,

  // End of '<S134>/Calculate Running Mean'

  // Start of '<S133>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S138>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S138>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S138>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S138>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S138>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S138>/Delay4'

    0.0
  }
  ,

  // End of '<S133>/Calculate Running Mean'

  // Start of '<S132>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S135>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S135>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S135>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S135>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S135>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S135>/Delay4'

    0.0
  }
  ,

  // End of '<S132>/Calculate Running Mean'

  // Start of '<S121>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S128>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S128>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S128>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S128>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S128>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S128>/Delay4'

    0.0
  }
  ,

  // End of '<S121>/Calculate Running Mean'

  // Start of '<S120>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S125>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S125>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S125>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S125>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S125>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S125>/Delay4'

    0.0
  }
  ,

  // End of '<S120>/Calculate Running Mean'

  // Start of '<S119>/Calculate Running Mean'
  {
    // Computed Parameter: Out1_Y0
    //  Referenced by: '<S122>/Out1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S122>/Delay'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S122>/Delay1'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S122>/Delay2'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S122>/Delay3'

    0.0,

    // Expression: 0.0
    //  Referenced by: '<S122>/Delay4'

    0.0
  }
  ,

  // End of '<S119>/Calculate Running Mean'

  // Start of '<S109>/AHRS2'
  {
    // Expression: 0.0001924722
    //  Referenced by: '<S109>/AHRS2'

    0.0001924722,

    // Expression: 9.1385e-5
    //  Referenced by: '<S109>/AHRS2'

    9.1385E-5,

    // Expression: 0.1
    //  Referenced by: '<S109>/AHRS2'

    0.1,

    // Expression: 3.0462e-13
    //  Referenced by: '<S109>/AHRS2'

    3.0462E-13,

    // Expression: 0.0096236100000000012
    //  Referenced by: '<S109>/AHRS2'

    0.0096236100000000012,

    // Expression: 0.5
    //  Referenced by: '<S109>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S109>/AHRS2'

    0.5,

    // Expression: 0.5
    //  Referenced by: '<S109>/AHRS2'

    0.5,

    // Expression: 33
    //  Referenced by: '<S109>/AHRS2'

    33.0
  }
  // End of '<S109>/AHRS2'
};

//
// File trailer for generated code.
//
// [EOF]
//
