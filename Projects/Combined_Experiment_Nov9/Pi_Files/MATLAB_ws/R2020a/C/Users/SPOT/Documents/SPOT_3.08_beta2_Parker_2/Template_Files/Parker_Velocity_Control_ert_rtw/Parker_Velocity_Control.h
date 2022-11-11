//
//  Parker_Velocity_Control.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Parker_Velocity_Control".
//
//  Model version              : 1.645
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Wed Jul 13 11:50:24 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_Parker_Velocity_Control_h_
#define RTW_HEADER_Parker_Velocity_Control_h_
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "dynamixel_functions.h"
#include "dynamixel_sdk.h"
#include "MW_gpio.h"
#include "MW_pigs.h"
#include "DAHostLib_Network.h"
#include "resource_headers.h"
#include "Parker_Velocity_Control_types.h"

// Shared type includes
#include "multiword_types.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rtGetInf.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

#ifndef rtmTaskCounter
# define rtmTaskCounter(rtm, idx)      ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

// Block signals for system '<S34>/MATLAB Function10'
typedef struct {
  real_T PWMs[8];
  real_T z1[8];
  real_T thrust_decay_factor;          // '<S34>/MATLAB Function10'
  real_T count;                        // '<S34>/MATLAB Function10'
} B_MATLABFunction10_Parker_Vel_T;

// Block signals for system '<S34>/MATLAB Function2'
typedef struct {
  real_T ThrustPer_Final[8];           // '<S34>/MATLAB Function2'
  real_T ThrustPer_Sat[8];
} B_MATLABFunction2_Parker_Velo_T;

// Block signals for system '<S41>/Create Rotation Matrix'
typedef struct {
  real_T C_Ib[4];                      // '<S41>/Create Rotation Matrix'
} B_CreateRotationMatrix_Parker_T;

// Block signals for system '<S35>/Create Rotation Matrix'
typedef struct {
  real_T C_bI[4];                      // '<S35>/Create Rotation Matrix'
} B_CreateRotationMatrix_Park_i_T;

// Block signals for system '<S46>/MATLAB Function'
typedef struct {
  real_T Mat2[64];
  real_T H[24];                        // '<S46>/MATLAB Function'
  real_T b[24];
  real_T v[8];
} B_MATLABFunction_Parker_Veloc_T;

// Block signals for system '<S130>/Correct Dropouts'
typedef struct {
  real_T FixedInput;                   // '<S130>/Correct Dropouts'
} B_CorrectDropouts_Parker_Velo_T;

// Block signals for system '<S155>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S155>/MATLAB Function2'
  real_T Ox[2];                        // '<S155>/MATLAB Function2'
} B_MATLABFunction2_Parker_Ve_n_T;

// Block signals for system '<S155>/MATLAB Function3'
typedef struct {
  real_T Ox[2];                        // '<S155>/MATLAB Function3'
} B_MATLABFunction3_Parker_Velo_T;

// Block signals for system '<S155>/MATLAB Function4'
typedef struct {
  real_T e_out;                        // '<S155>/MATLAB Function4'
} B_MATLABFunction4_Parker_Velo_T;

// Block signals for system '<S193>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S193>/MATLAB Function2'
  real_T Ox[2];                        // '<S193>/MATLAB Function2'
} B_MATLABFunction2_Parker_Ve_c_T;

// Block signals for system '<S180>/Sub-Phase #1'
typedef struct {
  real_T rtb_Subtract2_d_m[2];
  real_T In1;                          // '<S202>/In1'
  real_T In1_l;                        // '<S200>/In1'
  real_T In1_j;                        // '<S198>/In1'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4;// '<S193>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3;// '<S193>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_c_T sf_MATLABFunction2;// '<S193>/MATLAB Function2' 
} B_SubPhase1_Parker_Velocity_C_T;

// Block states (default storage) for system '<S180>/Sub-Phase #1'
typedef struct {
  real_T Delay1_DSTATE;                // '<S199>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S201>/Delay1'
  real_T Delay1_DSTATE_m;              // '<S197>/Delay1'
  uint8_T icLoad;                      // '<S199>/Delay1'
  uint8_T icLoad_p;                    // '<S201>/Delay1'
  uint8_T icLoad_pd;                   // '<S197>/Delay1'
} DW_SubPhase1_Parker_Velocity__T;

// Block signals for system '<S181>/Sub-Phase #1'
typedef struct {
  real_T rtb_Subtract2_e_m[2];
  real_T In1;                          // '<S275>/In1'
  real_T In1_j;                        // '<S273>/In1'
  real_T In1_m;                        // '<S271>/In1'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4;// '<S265>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3;// '<S265>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_c_T sf_MATLABFunction2;// '<S265>/MATLAB Function2' 
} B_SubPhase1_Parker_Velocity_g_T;

// Block states (default storage) for system '<S181>/Sub-Phase #1'
typedef struct {
  real_T Delay1_DSTATE;                // '<S272>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S274>/Delay1'
  real_T Delay1_DSTATE_o;              // '<S270>/Delay1'
  uint8_T icLoad;                      // '<S272>/Delay1'
  uint8_T icLoad_p;                    // '<S274>/Delay1'
  uint8_T icLoad_m;                    // '<S270>/Delay1'
} DW_SubPhase1_Parker_Velocit_l_T;

// Block signals for system '<S14>/Change BLUE Behavior'
typedef struct {
  real_T rtb_Subtract2_b_m[2];
  real_T In1;                          // '<S373>/In1'
  real_T In1_l;                        // '<S371>/In1'
  real_T In1_p;                        // '<S369>/In1'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4;// '<S367>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3;// '<S367>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2;// '<S367>/MATLAB Function2' 
} B_ChangeBLUEBehavior_Parker_k_T;

// Block states (default storage) for system '<S14>/Change BLUE Behavior'
typedef struct {
  real_T Delay1_DSTATE;                // '<S370>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S372>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S368>/Delay1'
  uint8_T icLoad;                      // '<S370>/Delay1'
  uint8_T icLoad_m;                    // '<S372>/Delay1'
  uint8_T icLoad_p;                    // '<S368>/Delay1'
} DW_ChangeBLUEBehavior_Parke_i_T;

// Block signals for system '<S349>/RED+ARM'
typedef struct {
  real_T TAU_out[6];                   // '<S393>/MATLAB Function'
  real_T t30[6];
  real_T t50[6];
  real_T t50_m[2];
  real_T t2;
  real_T t3;
  real_T t4;
  real_T t6;
  real_T t7;
  real_T t8;
  real_T t9;
  real_T t11;
  real_T t12;
  real_T t13;
  real_T t16;
  real_T t17;
  real_T t21;
  real_T t23;
  real_T t24;
  real_T t25;
  real_T t26;
  real_T t30_c;
  real_T t31;
  real_T t32;
  real_T t33;
  real_T t34;
  real_T t35;
  real_T t36;
  real_T t37;
  real_T t40;
  real_T t41;
  real_T t44;
  real_T t45;
  real_T t52;
  real_T t117;
  real_T t134;
} B_REDARM_Parker_Velocity_Cont_T;

// Block signals (default storage)
typedef struct {
  real_T TmpSignalConversionAtToWork[73];
  real_T Mat2[64];
  real_T Mat2_m[64];
  real_T Mat2_c[64];
  real_T X[36];
  real_T V[36];
  real_T U[36];
  real_T InertiaS[36];                 // '<S490>/Inertia Function '
  real_T b_A[36];
  real_T Vf[36];
  real_T Product2_b[24];               // '<S73>/Product2'
  real_T Product2_a[24];               // '<S55>/Product2'
  real_T transpose_f[24];              // '<S43>/transpose'
  real_T Product2_j[24];               // '<S43>/Product2'
  real_T H_hz[24];                     // '<S34>/MATLAB Function'
  real_T b[24];
  real_T AccelerationtoVelocity_o[12];
  real_T rtb_H_hz_k[9];
  real_T dv[9];
  real_T x[9];
  real_T x_c[9];
  real_T x_b[9];
  real_T rtb_Product_h_p[8];
  real_T rtb_transpose_f_c[8];
  real_T s[6];
  real_T dv1[6];
  real_T dv2[6];
  real_T X_f[6];
  real_T b_s[6];
  real_T e[6];
  real_T work[6];
  char_T cv[45];
  real_T x_ddot_b[3];                  // '<S461>/MATLAB Function'
  real_T rtb_Subtract2_f_g[2];
  real_T SetUniversalTimeIfthisisasimula;
                        // '<S18>/Set Universal Time (If this is a simulation)'
  real_T In1;                          // '<S441>/In1'
  real_T In1_e;                        // '<S439>/In1'
  real_T In1_i;                        // '<S437>/In1'
  real_T In1_p;                        // '<S413>/In1'
  real_T In1_it;                       // '<S411>/In1'
  real_T In1_j;                        // '<S409>/In1'
  real_T In1_a;                        // '<S388>/In1'
  real_T In1_c;                        // '<S386>/In1'
  real_T In1_o;                        // '<S384>/In1'
  real_T In1_o5;                       // '<S360>/In1'
  real_T In1_b;                        // '<S358>/In1'
  real_T In1_ei;                       // '<S356>/In1'
  real_T In1_f;                        // '<S174>/In1'
  real_T In1_l;                        // '<S172>/In1'
  real_T In1_fj;                       // '<S170>/In1'
  real_T In1_g;                        // '<S161>/In1'
  real_T In1_ej;                       // '<S159>/In1'
  real_T In1_d;                        // '<S157>/In1'
  real_T In1_lk;                       // '<S510>/In1'
  real_T In1_pk;                       // '<S509>/In1'
  real_T In1_k;                        // '<S508>/In1'
  real_T In1_ad;                       // '<S507>/In1'
  real_T In1_kj;                       // '<S506>/In1'
  real_T In1_h;                        // '<S497>/In1'
  real_T In1_lg;                       // '<S496>/In1'
  real_T In1_adc;                      // '<S484>/In1'
  real_T In1_fv;                       // '<S481>/In1'
  real_T In1_pr;                       // '<S480>/In1'
  real_T In1_es;                       // '<S472>/In1'
  real_T In1_id;                       // '<S471>/In1'
  real_T In1_kz;                       // '<S470>/In1'
  real_T In1_b2;                       // '<S469>/In1'
  real_T In1_aw;                       // '<S468>/In1'
  real_T UDPReceive_o1[16];            // '<S97>/UDP Receive'
  real_T dividebydeltaT;               // '<S107>/divide by delta T'
  real_T dividebydeltaT_k;             // '<S108>/divide by delta T'
  real_T Unwrap1;                      // '<S97>/Unwrap1'
  real_T dividebydeltaT_d;             // '<S109>/divide by delta T'
  real_T dividebydeltaT_j;             // '<S98>/divide by delta T'
  real_T dividebydeltaT_d2;            // '<S105>/divide by delta T'
  real_T Unwrap;                       // '<S97>/Unwrap'
  real_T dividebydeltaT_l;             // '<S106>/divide by delta T'
  real_T Unwrap2;                      // '<S97>/Unwrap2'
  real_T In1_dn;                       // '<S127>/In1'
  real_T In1_db;                       // '<S126>/In1'
  real_T In1_fo;                       // '<S125>/In1'
  real_T In1_ex;                       // '<S124>/In1'
  real_T In1_k5;                       // '<S123>/In1'
  real_T In1_gw;                       // '<S122>/In1'
  real_T In1_lb;                       // '<S121>/In1'
  real_T In1_f0;                       // '<S120>/In1'
  real_T In1_lu;                       // '<S119>/In1'
  real_T In1_as;                       // '<S118>/In1'
  real_T In1_i0;                       // '<S117>/In1'
  real_T In1_n;                        // '<S116>/In1'
  real_T In1_gp;                       // '<S115>/In1'
  real_T In1_cw;                       // '<S114>/In1'
  real_T In1_ky;                       // '<S113>/In1'
  real_T sampleTime;
  real_T t2;
  real_T t4;
  real_T t6;
  real_T t7;
  real_T t8;
  real_T t9;
  real_T t14;
  real_T t16;
  real_T t17;
  real_T t21;
  real_T t23;
  real_T t24;
  real_T t25;
  real_T t30;
  real_T t31;
  real_T t33;
  real_T t34;
  real_T t35;
  real_T t37;
  real_T t41;
  real_T t44;
  real_T t45;
  real_T t52;
  real_T t85;
  real_T t106;
  real_T t51;
  real_T t53;
  real_T t102;
  real_T t105;
  real_T t97;
  real_T t109;
  real_T t110;
  real_T t113;
  real_T t117;
  real_T t135;
  real_T t114;
  real_T t18;
  real_T t19;
  real_T t20;
  real_T t27;
  real_T t75;
  real_T t127;
  real_T t167;
  real_T t174;
  real_T t181;
  real_T t197;
  real_T t155;
  real_T t156;
  real_T t157;
  real_T Sum6_d;                       // '<S412>/Sum6'
  real_T Sum6_l;                       // '<S410>/Sum6'
  real_T Subtract_o;                   // '<S400>/Subtract'
  real_T rtb_x_ddot_b_g;
  real_T VelocitytoPosition;
  real_T rtb_x_ddot_i_m;
  real_T rtb_TmpSignalConversionAtSFu_nz;
  real_T rtb_TmpSignalConversionAtSFun_p;
  real_T rtb_TmpSignalConversionAtSFun_l;
  real_T rtb_TmpSignalConversionAtSFu_jw;
  real_T rtb_TmpSignalConversionAtSFun_d;
  real_T rtb_Sum_idx_0;
  real_T rtb_Sum_idx_1;
  real_T rtb_Sum_idx_2;
  real_T rtb_x_ddot_i_idx_0;
  real_T rtb_x_ddot_i_idx_1;
  real_T rtb_Sum_l_idx_0;
  real_T rtb_Sum_l_idx_1;
  real_T rtb_Sum_l_idx_2;
  real_T rtb_x_ddot_idx_1;
  real_T rtb_Sum_h_idx_0;
  real_T rtb_Sum_h_idx_1;
  real_T rtb_Sum_h_idx_2;
  real_T VelocitytoPosition_idx_0;
  real_T VelocitytoPosition_idx_1;
  real_T t4_tmp;
  real_T t7_tmp;
  real_T t14_tmp;
  real_T t9_tmp;
  real_T t15_tmp;
  real_T t197_tmp;
  real_T nrm;
  real_T rt;
  real_T ztest;
  real_T smm1;
  real_T emm1;
  real_T sqds;
  real_T shift;
  real_T roe;
  real_T absa;
  real_T absb;
  real_T scale;
  real_T ads;
  real_T bds;
  real_T sampleTime_g;
  real_T sampleTime_l;
  real_T absx11;
  real_T absx21;
  real_T absx31;
  real_T scale_d;
  real_T absxk;
  real_T t;
  real_T scale_dy;
  real_T absxk_l;
  real_T t_o;
  real_T sr;
  real_T si;
  real_T temp;
  real_T temp_b;
  int32_T vcol;
  int32_T ar;
  int32_T ia;
  int32_T ib;
  int32_T b_ic;
  int32_T ntIdx0;
  int32_T uElOffset1;
  int32_T i;
  int32_T qq;
  int32_T qjj;
  int32_T qp1jj;
  int32_T qp1q;
  int32_T k_ii;
  int32_T i_n;
  int32_T p1;
  int32_T p2;
  int32_T p3;
  int32_T itmp;
  int32_T kend;
  int32_T k;
  int32_T kend_b;
  int32_T k_l;
  int32_T ix;
  int32_T iy;
  int32_T k_h;
  int32_T ix_b;
  int32_T iy_d;
  int32_T k_e;
  int32_T ix_bj;
  int32_T iy_j;
  int32_T k_f;
  int32_T ix_a;
  int32_T iy_ju;
  int32_T k_j;
  int32_T ix_o;
  int32_T iy_n;
  int32_T k_i;
  int32_T ix_oy;
  int32_T iy_nv;
  int32_T k_m;
  int32_T k_c;
  uint32_T lo;
  uint32_T hi;
  int8_T rtAction;
  uint8_T u;
  boolean_T p;
  boolean_T rtb_Compare_f_m;
  boolean_T rtb_Compare_n_m;
  boolean_T rtb_Compare_j;
  boolean_T apply_transform;
  B_REDARM_Parker_Velocity_Cont_T REDARM_bo;// '<S402>/RED+ARM'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_f;// '<S433>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_f;// '<S433>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_c;// '<S433>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Parker_k_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_d;// '<S407>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_o;// '<S407>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_g;// '<S407>/MATLAB Function2' 
  B_REDARM_Parker_Velocity_Cont_T REDARM_p;// '<S349>/RED+ARM'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_n;// '<S380>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_c1;// '<S380>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_i;// '<S380>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Parker_k_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_l;// '<S354>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_b;// '<S354>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_p;// '<S354>/MATLAB Function2' 
  B_SubPhase1_Parker_Velocity_g_T SubPhase4_f;// '<S181>/Sub-Phase #4'
  B_SubPhase1_Parker_Velocity_g_T SubPhase3_p;// '<S181>/Sub-Phase #3 '
  B_SubPhase1_Parker_Velocity_g_T SubPhase2_m;// '<S181>/Sub-Phase #2 '
  B_SubPhase1_Parker_Velocity_g_T SubPhase1_a;// '<S181>/Sub-Phase #1'
  B_SubPhase1_Parker_Velocity_C_T SubPhase4;// '<S180>/Sub-Phase #4'
  B_SubPhase1_Parker_Velocity_C_T SubPhase3;// '<S180>/Sub-Phase #3 '
  B_SubPhase1_Parker_Velocity_C_T SubPhase2;// '<S180>/Sub-Phase #2 '
  B_SubPhase1_Parker_Velocity_C_T SubPhase1;// '<S180>/Sub-Phase #1'
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_c;// '<S168>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_c;// '<S168>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_m;// '<S168>/MATLAB Function2' 
  B_MATLABFunction4_Parker_Velo_T sf_MATLABFunction4_j;// '<S155>/MATLAB Function4' 
  B_MATLABFunction3_Parker_Velo_T sf_MATLABFunction3_k;// '<S155>/MATLAB Function3' 
  B_MATLABFunction2_Parker_Ve_n_T sf_MATLABFunction2_o;// '<S155>/MATLAB Function2' 
  B_CorrectDropouts_Parker_Velo_T sf_CorrectDropouts_h;// '<S132>/Correct Dropouts' 
  B_CorrectDropouts_Parker_Velo_T sf_CorrectDropouts_b;// '<S131>/Correct Dropouts' 
  B_CorrectDropouts_Parker_Velo_T sf_CorrectDropouts;// '<S130>/Correct Dropouts' 
  B_CreateRotationMatrix_Park_i_T sf_CreateRotationMatrix_n;// '<S59>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Parker_T sf_CreateRotationMatrix_e;// '<S71>/Create Rotation Matrix' 
  B_MATLABFunction10_Parker_Vel_T sf_MATLABFunction5;// '<S58>/MATLAB Function5' 
  B_MATLABFunction2_Parker_Velo_T sf_MATLABFunction2_l;// '<S58>/MATLAB Function2' 
  B_CreateRotationMatrix_Park_i_T sf_CreateRotationMatrix_f;// '<S47>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Parker_T sf_CreateRotationMatrix_i;// '<S53>/Create Rotation Matrix' 
  B_MATLABFunction10_Parker_Vel_T sf_MATLABFunction6;// '<S46>/MATLAB Function6' 
  B_MATLABFunction2_Parker_Velo_T sf_MATLABFunction2_n;// '<S46>/MATLAB Function2' 
  B_MATLABFunction_Parker_Veloc_T sf_MATLABFunction1_b;// '<S46>/MATLAB Function1' 
  B_MATLABFunction_Parker_Veloc_T sf_MATLABFunction_i;// '<S46>/MATLAB Function' 
  B_CreateRotationMatrix_Park_i_T sf_CreateRotationMatrix_h;// '<S35>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Parker_T sf_CreateRotationMatrix;// '<S41>/Create Rotation Matrix' 
  B_MATLABFunction2_Parker_Velo_T sf_MATLABFunction2;// '<S34>/MATLAB Function2' 
  B_MATLABFunction10_Parker_Vel_T sf_MATLABFunction10;// '<S34>/MATLAB Function10' 
} B_Parker_Velocity_Control_T;

// Block states (default storage) for system '<Root>'
typedef struct {
  InitializeForSpeed_Parker_Vel_T obj;
                             // '<Root>/Initialization Block for Speed Control'
  codertarget_linux_blocks_Di_i_T obj_f;// '<S92>/Digital Read'
  ReadArm_Position_Rates_Parker_T obj_c;
                     // '<S128>/Read Joint Positions using  Dynamixel Encoders'
  IdentifyPlatform_Parker_Veloc_T obj_b;// '<S17>/WhoAmI'
  raspi_internal_PWMBlock_Parke_T obj_d;// '<S90>/PWM1'
  raspi_internal_PWMBlock_Parke_T obj_m;// '<S33>/RED T1 - BLACK T8'
  raspi_internal_PWMBlock_Parke_T obj_ma;// '<S33>/RED T2 - BLACK T3'
  raspi_internal_PWMBlock_Parke_T obj_h;// '<S33>/RED T3'
  raspi_internal_PWMBlock_Parke_T obj_k;// '<S33>/RED T4 - BLACK T5'
  raspi_internal_PWMBlock_Parke_T obj_hw;// '<S33>/RED T5 - BLACK T4'
  raspi_internal_PWMBlock_Parke_T obj_hf;// '<S33>/RED T6 - BLACK T7'
  raspi_internal_PWMBlock_Parke_T obj_ce;// '<S33>/RED T7 - BLACK T6'
  raspi_internal_PWMBlock_Parke_T obj_fo;// '<S33>/RED T8 - BLACK T1'
  raspi_internal_PWMBlock_Parke_T obj_ci;// '<S33>/BLACK T2'
  codertarget_linux_blocks_Digi_T obj_cp;// '<S94>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_bm;// '<S93>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_a;// '<S24>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_i;// '<S23>/Digital Write'
  MoveArm_Speed_Parker_Velocity_T obj_o;
                                // '<S346>/Send Speed Commands to  Manipulator'
  real_T Delay1_DSTATE;                // '<S436>/Delay1'
  real_T Delay1_DSTATE_d;              // '<S438>/Delay1'
  real_T Delay1_DSTATE_c;              // '<S440>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S410>/Delay1'
  real_T Delay1_DSTATE_f;              // '<S412>/Delay1'
  real_T Delay1_DSTATE_ci;             // '<S408>/Delay1'
  real_T Delay1_DSTATE_i;              // '<S383>/Delay1'
  real_T Delay1_DSTATE_i3;             // '<S385>/Delay1'
  real_T Delay1_DSTATE_cl;             // '<S387>/Delay1'
  real_T Delay1_DSTATE_db;             // '<S357>/Delay1'
  real_T Delay1_DSTATE_dz;             // '<S359>/Delay1'
  real_T Delay1_DSTATE_h;              // '<S355>/Delay1'
  real_T Delay1_DSTATE_fb;             // '<S171>/Delay1'
  real_T Delay1_DSTATE_l;              // '<S173>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S169>/Delay1'
  real_T Delay1_DSTATE_k;              // '<S158>/Delay1'
  real_T Delay1_DSTATE_o;              // '<S160>/Delay1'
  real_T Delay1_DSTATE_lg;             // '<S156>/Delay1'
  real_T Delay1_DSTATE_jp;             // '<S130>/Delay1'
  real_T Delay2_DSTATE[2];             // '<S130>/Delay2'
  real_T Delay5_DSTATE[3];             // '<S130>/Delay5'
  real_T Delay3_DSTATE[4];             // '<S130>/Delay3'
  real_T Delay4_DSTATE[5];             // '<S130>/Delay4'
  real_T Delay1_DSTATE_n5;             // '<S131>/Delay1'
  real_T Delay2_DSTATE_l[2];           // '<S131>/Delay2'
  real_T Delay5_DSTATE_b[3];           // '<S131>/Delay5'
  real_T Delay3_DSTATE_o[4];           // '<S131>/Delay3'
  real_T Delay4_DSTATE_c[5];           // '<S131>/Delay4'
  real_T Delay1_DSTATE_ny;             // '<S132>/Delay1'
  real_T Delay2_DSTATE_m[2];           // '<S132>/Delay2'
  real_T Delay5_DSTATE_n[3];           // '<S132>/Delay5'
  real_T Delay3_DSTATE_h[4];           // '<S132>/Delay3'
  real_T Delay4_DSTATE_k[5];           // '<S132>/Delay4'
  real_T AccelerationtoVelocity_DSTATE[3];// '<S461>/Acceleration  to Velocity'
  real_T VelocitytoPosition_DSTATE[3]; // '<S461>/Velocity to Position'
  real_T Delay1_DSTATE_p;              // '<S466>/Delay1'
  real_T Delay1_DSTATE_ja;             // '<S464>/Delay1'
  real_T Delay1_DSTATE_d4;             // '<S462>/Delay1'
  real_T Delay1_DSTATE_j3;             // '<S465>/Delay1'
  real_T Delay1_DSTATE_km;             // '<S463>/Delay1'
  real_T AccelerationtoVelocity_DSTATE_b[3];// '<S473>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_a[3];// '<S473>/Velocity to Position'
  real_T Delay1_DSTATE_e;              // '<S478>/Delay1'
  real_T Delay1_DSTATE_fi;             // '<S476>/Delay1'
  real_T Delay1_DSTATE_b;              // '<S474>/Delay1'
  real_T Delay1_DSTATE_fq;             // '<S477>/Delay1'
  real_T Delay1_DSTATE_od;             // '<S475>/Delay1'
  real_T DiscreteTimeIntegrator_DSTATE;// '<S459>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator1_DSTATE;// '<S459>/Discrete-Time Integrator1'
  real_T DiscreteTimeIntegrator2_DSTATE;// '<S459>/Discrete-Time Integrator2'
  real_T AccelerationtoVelocity_DSTATE_m[3];// '<S499>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_f[3];// '<S499>/Velocity to Position'
  real_T Delay1_DSTATE_g;              // '<S501>/Delay1'
  real_T Delay1_DSTATE_m;              // '<S503>/Delay1'
  real_T Delay1_DSTATE_et;             // '<S500>/Delay1'
  real_T Delay1_DSTATE_ln;             // '<S504>/Delay1'
  real_T Delay1_DSTATE_hs;             // '<S502>/Delay1'
  real_T Delay_DSTATE[12];             // '<S490>/Delay'
  real_T AccelerationtoVelocity_DSTATE_o[12];// '<S490>/Acceleration  to Velocity' 
  real_T Delay1_DSTATE_po;             // '<S495>/Delay1'
  real_T Delay1_DSTATE_kx;             // '<S494>/Delay1'
  real_T Delay1_DSTATE_hi;             // '<S107>/Delay1'
  real_T Delay1_DSTATE_df;             // '<S99>/Delay1'
  real_T Delay1_DSTATE_ft;             // '<S108>/Delay1'
  real_T Delay1_DSTATE_hz;             // '<S100>/Delay1'
  real_T Delay1_DSTATE_mt;             // '<S109>/Delay1'
  real_T Delay1_DSTATE_lf;             // '<S101>/Delay1'
  real_T Delay1_DSTATE_cs;             // '<S102>/Delay1'
  real_T Delay1_DSTATE_kv;             // '<S98>/Delay1'
  real_T Delay1_DSTATE_lc;             // '<S110>/Delay1'
  real_T Delay1_DSTATE_cd;             // '<S105>/Delay1'
  real_T Delay1_DSTATE_i0;             // '<S111>/Delay1'
  real_T Delay1_DSTATE_jx;             // '<S106>/Delay1'
  real_T Delay1_DSTATE_ml;             // '<S112>/Delay1'
  real_T Delay1_DSTATE_ic;             // '<S103>/Delay1'
  real_T Delay1_DSTATE_a;              // '<S104>/Delay1'
  real_T Delay_DSTATE_k;               // '<S80>/Delay'
  real_T RateTransition_Buffer;        // '<S4>/Rate Transition'
  real_T RateTransition1_Buffer;       // '<S4>/Rate Transition1'
  real_T RateTransition2_Buffer;       // '<S4>/Rate Transition2'
  real_T RateTransition3_Buffer;       // '<S4>/Rate Transition3'
  real_T RateTransition4_Buffer;       // '<S4>/Rate Transition4'
  real_T RateTransition5_Buffer;       // '<S4>/Rate Transition5'
  real_T RateTransition6_Buffer;       // '<S4>/Rate Transition6'
  real_T RateTransition7_Buffer;       // '<S4>/Rate Transition7'
  real_T RateTransition8_Buffer;       // '<S4>/Rate Transition8'
  real_T UDPSend_NetworkLib[137];      // '<S18>/UDP Send'
  real_T BLACK_Fx;                     // '<Root>/BLACK_Fx'
  real_T BLUE_Fx;                      // '<Root>/BLACK_Fx1'
  real_T BLACK_Fx_Sat;                 // '<Root>/BLACK_Fx_Sat'
  real_T ARM_Elbow_Py;                 // '<Root>/BLACK_Fx_Sat1'
  real_T BLUE_Fx_Sat;                  // '<Root>/BLACK_Fx_Sat2'
  real_T BLACK_Fy;                     // '<Root>/BLACK_Fy'
  real_T BLUE_Fy;                      // '<Root>/BLACK_Fy1'
  real_T BLACK_Fy_Sat;                 // '<Root>/BLACK_Fy_Sat'
  real_T ARM_Wrist_Px;                 // '<Root>/BLACK_Fy_Sat1'
  real_T BLUE_Fy_Sat;                  // '<Root>/BLACK_Fy_Sat2'
  real_T BLACK_Px;                     // '<Root>/BLACK_Px'
  real_T BLUE_Px;                      // '<Root>/BLACK_Px1'
  real_T BLACK_Py;                     // '<Root>/BLACK_Py'
  real_T BLUE_Py;                      // '<Root>/BLACK_Py1'
  real_T BLACK_Rz;                     // '<Root>/BLACK_Rz'
  real_T BLUE_Rz;                      // '<Root>/BLACK_Rz1'
  real_T BLACK_Tz;                     // '<Root>/BLACK_Tz'
  real_T BLUE_Tz;                      // '<Root>/BLACK_Tz1'
  real_T BLACK_Tz_Sat;                 // '<Root>/BLACK_Tz_Sat'
  real_T ARM_Wrist_Py;                 // '<Root>/BLACK_Tz_Sat1'
  real_T BLUE_Tz_Sat;                  // '<Root>/BLACK_Tz_Sat2'
  real_T RED_Fx;                       // '<Root>/RED_Fx'
  real_T RED_Fx_Sat;                   // '<Root>/RED_Fx_Sat'
  real_T RED_Fy;                       // '<Root>/RED_Fy'
  real_T RED_Fy_Sat;                   // '<Root>/RED_Fy_Sat'
  real_T RED_Px;                       // '<Root>/RED_Px'
  real_T RED_AHRS_Q;                   // '<Root>/RED_Px1'
  real_T BLACK_IMU_Ax_b;               // '<Root>/RED_Px10'
  real_T BLACK_IMU_Ay_b;               // '<Root>/RED_Px11'
  real_T BLACK_IMU_Az_b;               // '<Root>/RED_Px12'
  real_T RED_IMU_Q;                    // '<Root>/RED_Px19'
  real_T RED_AHRS_P;                   // '<Root>/RED_Px2'
  real_T RED_IMU_P;                    // '<Root>/RED_Px20'
  real_T RED_IMU_R;                    // '<Root>/RED_Px21'
  real_T BLACK_IMU_Q;                  // '<Root>/RED_Px22'
  real_T BLACK_IMU_P;                  // '<Root>/RED_Px23'
  real_T BLACK_IMU_R;                  // '<Root>/RED_Px24'
  real_T RED_IMU_Psi;                  // '<Root>/RED_Px25'
  real_T BLACK_IMU_Psi;                // '<Root>/RED_Px26'
  real_T RED_AHRS_R;                   // '<Root>/RED_Px3'
  real_T RED_IMU_Ax_I;                 // '<Root>/RED_Px31'
  real_T RED_IMU_Ay_I;                 // '<Root>/RED_Px32'
  real_T BLACK_IMU_Ax_I;               // '<Root>/RED_Px37'
  real_T BLACK_IMU_Ay_I;               // '<Root>/RED_Px38'
  real_T BLACK_AHRS_Q;                 // '<Root>/RED_Px4'
  real_T BLACK_AHRS_P;                 // '<Root>/RED_Px5'
  real_T BLACK_AHRS_R;                 // '<Root>/RED_Px6'
  real_T RED_IMU_Ax_b;                 // '<Root>/RED_Px7'
  real_T RED_IMU_Ay_b;                 // '<Root>/RED_Px8'
  real_T RED_IMU_Az_b;                 // '<Root>/RED_Px9'
  real_T RED_Py;                       // '<Root>/RED_Py'
  real_T RED_Rz;                       // '<Root>/RED_Rz'
  real_T RED_Tz;                       // '<Root>/RED_Tz'
  real_T RED_Tz_Shoulder;              // '<Root>/RED_Tz1'
  real_T RED_RzD_Shoulder;             // '<Root>/RED_Tz12'
  real_T RED_RzD_Elbow;                // '<Root>/RED_Tz13'
  real_T RED_RzD_Wrist;                // '<Root>/RED_Tz14'
  real_T RED_Tz_Elbow;                 // '<Root>/RED_Tz2'
  real_T RED_Tz_Wrist;                 // '<Root>/RED_Tz3'
  real_T RED_Rz_Shoulder;              // '<Root>/RED_Tz4'
  real_T RED_Rz_Elbow;                 // '<Root>/RED_Tz5'
  real_T RED_Rz_Wrist;                 // '<Root>/RED_Tz6'
  real_T Float_State;                  // '<Root>/RED_Tz7'
  real_T Magnet_State;                 // '<Root>/RED_Tz8'
  real_T RED_Tz_RW;                    // '<Root>/RED_Tz_RW'
  real_T RED_Tz_RW_Sat;                // '<Root>/RED_Tz_RW Sat'
  real_T ARM_EndEff_Py;                // '<Root>/RED_Tz_RW Sat1'
  real_T ARM_EndEff_Px;                // '<Root>/RED_Tz_RW1'
  real_T RED_Tz_Sat;                   // '<Root>/RED_Tz_Sat'
  real_T ARM_Elbow_Px;                 // '<Root>/RED_Tz_Sat1'
  real_T RED_dRz_RW_Sat;               // '<Root>/RED_dRz_RW Sat'
  real_T Univ_Time;                    // '<Root>/Universal_Time'
  real_T RED_Vx;                       // '<Root>/Universal_Time1'
  real_T RED_Ax;                       // '<Root>/Universal_Time10'
  real_T RED_Ay;                       // '<Root>/Universal_Time11'
  real_T BLACK_Ax;                     // '<Root>/Universal_Time12'
  real_T BLACK_Ay;                     // '<Root>/Universal_Time13'
  real_T BLACK_RzDD;                   // '<Root>/Universal_Time14'
  real_T RED_RzDD;                     // '<Root>/Universal_Time15'
  real_T WhoAmI;                       // '<Root>/Universal_Time19'
  real_T RED_Vy;                       // '<Root>/Universal_Time2'
  real_T RED_RzD;                      // '<Root>/Universal_Time3'
  real_T BLACK_Vx;                     // '<Root>/Universal_Time4'
  real_T BLACK_Vy;                     // '<Root>/Universal_Time5'
  real_T BLACK_RzD;                    // '<Root>/Universal_Time6'
  real_T BLUE_Vx;                      // '<Root>/Universal_Time7'
  real_T BLUE_Vy;                      // '<Root>/Universal_Time8'
  real_T BLUE_RzD;                     // '<Root>/Universal_Time9'
  real_T NextOutput;                   // '<S461>/Random Number'
  real_T NextOutput_p;                 // '<S457>/Random Number1'
  real_T NextOutput_o;                 // '<S457>/Random Number'
  real_T NextOutput_j;                 // '<S457>/Random Number2'
  real_T NextOutput_g;                 // '<S473>/Random Number'
  real_T NextOutput_k;                 // '<S458>/Random Number5'
  real_T NextOutput_lo;                // '<S499>/Random Number'
  real_T NextOutput_de;                // '<S498>/Random Number7'
  real_T NextOutput_c;                 // '<S498>/Random Number6'
  real_T NextOutput_h;                 // '<S498>/Random Number8'
  real_T NextOutput_c2;                // '<S490>/Random Number2'
  real_T NextOutput_lz;                // '<S490>/Random Number1'
  real_T NextOutput_dv;                // '<S490>/Random Number3'
  real_T UDPReceive_NetworkLib[137];   // '<S97>/UDP Receive'
  real_T Unwrap1_Prev;                 // '<S97>/Unwrap1'
  real_T Unwrap1_Cumsum;               // '<S97>/Unwrap1'
  real_T Unwrap_Prev;                  // '<S97>/Unwrap'
  real_T Unwrap_Cumsum;                // '<S97>/Unwrap'
  real_T Unwrap2_Prev;                 // '<S97>/Unwrap2'
  real_T Unwrap2_Cumsum;               // '<S97>/Unwrap2'
  real_T Product1_DWORK4[9];           // '<S72>/Product1'
  real_T Product1_DWORK4_j[9];         // '<S73>/Product1'
  real_T Product1_DWORK4_g[9];         // '<S54>/Product1'
  real_T Product1_DWORK4_b[9];         // '<S55>/Product1'
  real_T Product1_DWORK4_gx[9];        // '<S42>/Product1'
  real_T Product1_DWORK4_c[9];         // '<S43>/Product1'
  struct {
    void *LoggedData;
  } ToWorkspace_PWORK;                 // '<S1>/To Workspace'

  uint32_T RandSeed;                   // '<S461>/Random Number'
  uint32_T RandSeed_m;                 // '<S457>/Random Number1'
  uint32_T RandSeed_d;                 // '<S457>/Random Number'
  uint32_T RandSeed_k;                 // '<S457>/Random Number2'
  uint32_T RandSeed_d3;                // '<S473>/Random Number'
  uint32_T RandSeed_dc;                // '<S458>/Random Number4'
  uint32_T RandSeed_j;                 // '<S458>/Random Number3'
  uint32_T RandSeed_c;                 // '<S458>/Random Number5'
  uint32_T RandSeed_mt;                // '<S499>/Random Number'
  uint32_T RandSeed_ka;                // '<S498>/Random Number7'
  uint32_T RandSeed_g;                 // '<S498>/Random Number6'
  uint32_T RandSeed_f;                 // '<S498>/Random Number8'
  uint32_T RandSeed_o;                 // '<S490>/Random Number2'
  uint32_T RandSeed_p;                 // '<S490>/Random Number1'
  uint32_T RandSeed_h;                 // '<S490>/Random Number3'
  boolean_T Delay1_DSTATE_kt;          // '<S85>/Delay1'
  boolean_T Delay2_DSTATE_a[2];        // '<S85>/Delay2'
  boolean_T Delay5_DSTATE_g[3];        // '<S85>/Delay5'
  boolean_T Delay3_DSTATE_n[4];        // '<S85>/Delay3'
  boolean_T Delay4_DSTATE_i[5];        // '<S85>/Delay4'
  int8_T Ifperforminganexperimentgrabthe;
  // '<S6>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
  int8_T DiscreteTimeIntegrator_PrevRese;// '<S459>/Discrete-Time Integrator'
  int8_T DiscreteTimeIntegrator1_PrevRes;// '<S459>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator2_PrevRes;// '<S459>/Discrete-Time Integrator2'
  uint8_T icLoad;                      // '<S436>/Delay1'
  uint8_T icLoad_l;                    // '<S438>/Delay1'
  uint8_T icLoad_j;                    // '<S440>/Delay1'
  uint8_T icLoad_d;                    // '<S410>/Delay1'
  uint8_T icLoad_f;                    // '<S412>/Delay1'
  uint8_T icLoad_b;                    // '<S408>/Delay1'
  uint8_T icLoad_k;                    // '<S383>/Delay1'
  uint8_T icLoad_du;                   // '<S385>/Delay1'
  uint8_T icLoad_g;                    // '<S387>/Delay1'
  uint8_T icLoad_e;                    // '<S357>/Delay1'
  uint8_T icLoad_fr;                   // '<S359>/Delay1'
  uint8_T icLoad_o;                    // '<S355>/Delay1'
  uint8_T icLoad_dy;                   // '<S171>/Delay1'
  uint8_T icLoad_n;                    // '<S173>/Delay1'
  uint8_T icLoad_bp;                   // '<S169>/Delay1'
  uint8_T icLoad_ns;                   // '<S158>/Delay1'
  uint8_T icLoad_a;                    // '<S160>/Delay1'
  uint8_T icLoad_h;                    // '<S156>/Delay1'
  uint8_T icLoad_bb;                   // '<S466>/Delay1'
  uint8_T icLoad_p;                    // '<S464>/Delay1'
  uint8_T icLoad_lv;                   // '<S462>/Delay1'
  uint8_T icLoad_hu;                   // '<S465>/Delay1'
  uint8_T icLoad_m;                    // '<S463>/Delay1'
  uint8_T icLoad_dc;                   // '<S478>/Delay1'
  uint8_T icLoad_ln;                   // '<S476>/Delay1'
  uint8_T icLoad_oc;                   // '<S474>/Delay1'
  uint8_T icLoad_l1;                   // '<S477>/Delay1'
  uint8_T icLoad_lz;                   // '<S475>/Delay1'
  uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S459>/Discrete-Time Integrator'
  uint8_T DiscreteTimeIntegrator1_IC_LOAD;// '<S459>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator2_IC_LOAD;// '<S459>/Discrete-Time Integrator2'
  uint8_T icLoad_ec;                   // '<S501>/Delay1'
  uint8_T icLoad_by;                   // '<S503>/Delay1'
  uint8_T icLoad_oe;                   // '<S500>/Delay1'
  uint8_T icLoad_mq;                   // '<S504>/Delay1'
  uint8_T icLoad_ov;                   // '<S502>/Delay1'
  uint8_T icLoad_nt;                   // '<S495>/Delay1'
  uint8_T icLoad_el;                   // '<S494>/Delay1'
  uint8_T icLoad_c;                    // '<S107>/Delay1'
  uint8_T icLoad_h0;                   // '<S99>/Delay1'
  uint8_T icLoad_kt;                   // '<S108>/Delay1'
  uint8_T icLoad_ktm;                  // '<S100>/Delay1'
  uint8_T icLoad_gn;                   // '<S109>/Delay1'
  uint8_T icLoad_pz;                   // '<S101>/Delay1'
  uint8_T icLoad_fy;                   // '<S102>/Delay1'
  uint8_T icLoad_ea;                   // '<S98>/Delay1'
  uint8_T icLoad_df;                   // '<S110>/Delay1'
  uint8_T icLoad_pj;                   // '<S105>/Delay1'
  uint8_T icLoad_ap;                   // '<S111>/Delay1'
  uint8_T icLoad_hr;                   // '<S106>/Delay1'
  uint8_T icLoad_al;                   // '<S112>/Delay1'
  uint8_T icLoad_dt;                   // '<S103>/Delay1'
  uint8_T icLoad_l1b;                  // '<S104>/Delay1'
  boolean_T Unwrap1_FirstStep;         // '<S97>/Unwrap1'
  boolean_T Unwrap_FirstStep;          // '<S97>/Unwrap'
  boolean_T Unwrap2_FirstStep;         // '<S97>/Unwrap2'
  DW_ChangeBLUEBehavior_Parke_i_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  DW_ChangeBLUEBehavior_Parke_i_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  DW_SubPhase1_Parker_Velocit_l_T SubPhase4_f;// '<S181>/Sub-Phase #4'
  DW_SubPhase1_Parker_Velocit_l_T SubPhase3_p;// '<S181>/Sub-Phase #3 '
  DW_SubPhase1_Parker_Velocit_l_T SubPhase2_m;// '<S181>/Sub-Phase #2 '
  DW_SubPhase1_Parker_Velocit_l_T SubPhase1_a;// '<S181>/Sub-Phase #1'
  DW_SubPhase1_Parker_Velocity__T SubPhase4;// '<S180>/Sub-Phase #4'
  DW_SubPhase1_Parker_Velocity__T SubPhase3;// '<S180>/Sub-Phase #3 '
  DW_SubPhase1_Parker_Velocity__T SubPhase2;// '<S180>/Sub-Phase #2 '
  DW_SubPhase1_Parker_Velocity__T SubPhase1;// '<S180>/Sub-Phase #1'
} DW_Parker_Velocity_Control_T;

// Parameters for system: '<S10>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Parker_V_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S141>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S141>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S141>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S141>/Puck State'

};

// Parameters for system: '<Root>/Phase #0:  Wait for Synchronization'
struct P_Phase0WaitforSynchronizatio_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S140>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S140>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S140>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S140>/Puck State'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S142>/Constant'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S142>/Constant1'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S142>/Constant2'

  real_T Constant3_Value_h;            // Expression: 0
                                          //  Referenced by: '<S142>/Constant3'

  real_T Constant4_Value_g;            // Expression: 0
                                          //  Referenced by: '<S142>/Constant4'

  real_T Constant5_Value_e;            // Expression: 0
                                          //  Referenced by: '<S142>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S142>/Constant6'

  real_T PuckState_Value_b;            // Expression: 0
                                          //  Referenced by: '<S142>/Puck State'

  P_ChangeBLUEBehavior_Parker_V_T ChangeBLUEBehavior;// '<S10>/Change BLUE Behavior' 
};

// Parameters for system: '<S180>/Sub-Phase #1'
struct P_SubPhase1_Parker_Velocity_C_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S198>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S200>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S202>/Out1'

  real_T DesiredRateBLACK_Value;       // Expression: 0.03490659
                                          //  Referenced by: '<S189>/Desired Rate (BLACK)'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S184>/Puck State'

};

// Parameters for system: '<S181>/Sub-Phase #1'
struct P_SubPhase1_Parker_Velocity_k_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S271>/Out1'

  real_T Out1_Y0_g;                    // Computed Parameter: Out1_Y0_g
                                          //  Referenced by: '<S273>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S275>/Out1'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S268>/Constant'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S257>/Puck State'

};

// Parameters for system: '<S182>/Sub-Phase #1'
struct P_SubPhase1_Parker_Velocity_m_T_ {
  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S340>/Constant1'

  real_T Constant7_Value;              // Expression: 0
                                          //  Referenced by: '<S339>/Constant7'

  real_T Constant1_Value_o;            // Expression: 0
                                          //  Referenced by: '<S334>/Constant1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S334>/Puck State'

};

// Parameters for system: '<S14>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Parker_c_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S369>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S371>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S373>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S348>/Puck State'

};

// Parameters for system: '<S349>/RED+ARM'
struct P_REDARM_Parker_Velocity_Cont_T_ {
  real_T Constant2_Value;              // Expression: 0.9
                                          //  Referenced by: '<S382>/Constant2'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S382>/Constant'

};

// Parameters (default storage)
struct P_Parker_Velocity_Control_T_ {
  real_T F_Black_X_nominal;            // Variable: F_Black_X_nominal
                                          //  Referenced by: '<S34>/MATLAB Function11'

  real_T F_Black_Y_nominal;            // Variable: F_Black_Y_nominal
                                          //  Referenced by: '<S34>/MATLAB Function11'

  real_T F_Blue_X_nominal;             // Variable: F_Blue_X_nominal
                                          //  Referenced by: '<S46>/MATLAB Function7'

  real_T F_Blue_Y_nominal;             // Variable: F_Blue_Y_nominal
                                          //  Referenced by: '<S46>/MATLAB Function7'

  real_T F_red_X_nominal;              // Variable: F_red_X_nominal
                                          //  Referenced by:
                                          //    '<S58>/MATLAB Function4'
                                          //    '<S58>/MATLAB Function6'

  real_T F_red_Y_nominal;              // Variable: F_red_Y_nominal
                                          //  Referenced by:
                                          //    '<S58>/MATLAB Function4'
                                          //    '<S58>/MATLAB Function6'

  real_T F_thrusters_BLACK[8];         // Variable: F_thrusters_BLACK
                                          //  Referenced by:
                                          //    '<S34>/MATLAB Function'
                                          //    '<S34>/MATLAB Function1'

  real_T F_thrusters_BLUE[8];          // Variable: F_thrusters_BLUE
                                          //  Referenced by:
                                          //    '<S46>/MATLAB Function'
                                          //    '<S46>/MATLAB Function1'

  real_T F_thrusters_RED[8];           // Variable: F_thrusters_RED
                                          //  Referenced by:
                                          //    '<S58>/MATLAB Function'
                                          //    '<S58>/MATLAB Function1'
                                          //    '<S58>/MATLAB Function3'

  real_T Gamma1;                       // Variable: Gamma1
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Gamma2;                       // Variable: Gamma2
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Gamma3;                       // Variable: Gamma3
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Gamma4;                       // Variable: Gamma4
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Gamma5;                       // Variable: Gamma5
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Gamma6;                       // Variable: Gamma6
                                          //  Referenced by: '<S490>/Dynamics Model'

  real_T Kd_tb;                        // Variable: Kd_tb
                                          //  Referenced by:
                                          //    '<S152>/kd_tb'
                                          //    '<S351>/kd_tb'
                                          //    '<S404>/kd_tb'
                                          //    '<S190>/kd_tb'
                                          //    '<S207>/kd_tb'
                                          //    '<S224>/kd_tb'
                                          //    '<S241>/kd_tb'

  real_T Kd_tblue;                     // Variable: Kd_tblue
                                          //  Referenced by:
                                          //    '<S165>/kd_tb'
                                          //    '<S364>/kd_tb'
                                          //    '<S417>/kd_tb'
                                          //    '<S262>/kd_tb'
                                          //    '<S280>/kd_tb'
                                          //    '<S298>/kd_tb'
                                          //    '<S316>/kd_tb'

  real_T Kd_tr;                        // Variable: Kd_tr
                                          //  Referenced by:
                                          //    '<S377>/kd_tr'
                                          //    '<S430>/kd_tr'

  real_T Kd_xb;                        // Variable: Kd_xb
                                          //  Referenced by:
                                          //    '<S153>/kd_xb'
                                          //    '<S352>/kd_xb'
                                          //    '<S405>/kd_xb'
                                          //    '<S191>/kd_xb'
                                          //    '<S208>/kd_xb'
                                          //    '<S225>/kd_xb'
                                          //    '<S242>/kd_xb'

  real_T Kd_xblue;                     // Variable: Kd_xblue
                                          //  Referenced by:
                                          //    '<S166>/kd_xb'
                                          //    '<S365>/kd_xb'
                                          //    '<S418>/kd_xb'
                                          //    '<S263>/kd_xb'
                                          //    '<S281>/kd_xb'
                                          //    '<S299>/kd_xb'
                                          //    '<S317>/kd_xb'

  real_T Kd_xr;                        // Variable: Kd_xr
                                          //  Referenced by:
                                          //    '<S378>/kd_xr'
                                          //    '<S431>/kd_xr'

  real_T Kd_yb;                        // Variable: Kd_yb
                                          //  Referenced by:
                                          //    '<S154>/kd_yb'
                                          //    '<S353>/kd_yb'
                                          //    '<S406>/kd_yb'
                                          //    '<S192>/kd_yb'
                                          //    '<S209>/kd_yb'
                                          //    '<S226>/kd_yb'
                                          //    '<S243>/kd_yb'

  real_T Kd_yblue;                     // Variable: Kd_yblue
                                          //  Referenced by:
                                          //    '<S167>/kd_yb'
                                          //    '<S366>/kd_yb'
                                          //    '<S419>/kd_yb'
                                          //    '<S264>/kd_yb'
                                          //    '<S282>/kd_yb'
                                          //    '<S300>/kd_yb'
                                          //    '<S318>/kd_yb'

  real_T Kd_yr;                        // Variable: Kd_yr
                                          //  Referenced by:
                                          //    '<S379>/kd_yr'
                                          //    '<S432>/kd_yr'

  real_T Kp[9];                        // Variable: Kp
                                          //  Referenced by:
                                          //    '<S393>/MATLAB Function'
                                          //    '<S446>/MATLAB Function'

  real_T Kp_tb;                        // Variable: Kp_tb
                                          //  Referenced by:
                                          //    '<S152>/kp_tb'
                                          //    '<S351>/kp_tb'
                                          //    '<S404>/kp_tb'
                                          //    '<S190>/kp_tb'
                                          //    '<S207>/kp_tb'
                                          //    '<S224>/kp_tb'
                                          //    '<S241>/kp_tb'

  real_T Kp_tblue;                     // Variable: Kp_tblue
                                          //  Referenced by:
                                          //    '<S165>/kp_tb'
                                          //    '<S364>/kp_tb'
                                          //    '<S417>/kp_tb'
                                          //    '<S262>/kp_tb'
                                          //    '<S280>/kp_tb'
                                          //    '<S298>/kp_tb'
                                          //    '<S316>/kp_tb'

  real_T Kp_tr;                        // Variable: Kp_tr
                                          //  Referenced by:
                                          //    '<S377>/kp_tr'
                                          //    '<S430>/kp_tr'

  real_T Kp_xb;                        // Variable: Kp_xb
                                          //  Referenced by:
                                          //    '<S153>/kp_xb'
                                          //    '<S352>/kp_xb'
                                          //    '<S405>/kp_xb'
                                          //    '<S191>/kp_xb'
                                          //    '<S208>/kp_xb'
                                          //    '<S225>/kp_xb'
                                          //    '<S242>/kp_xb'

  real_T Kp_xblue;                     // Variable: Kp_xblue
                                          //  Referenced by:
                                          //    '<S166>/kp_xb'
                                          //    '<S365>/kp_xb'
                                          //    '<S418>/kp_xb'
                                          //    '<S263>/kp_xb'
                                          //    '<S281>/kp_xb'
                                          //    '<S299>/kp_xb'
                                          //    '<S317>/kp_xb'

  real_T Kp_xr;                        // Variable: Kp_xr
                                          //  Referenced by:
                                          //    '<S378>/kp_xr'
                                          //    '<S431>/kp_xr'

  real_T Kp_yb;                        // Variable: Kp_yb
                                          //  Referenced by:
                                          //    '<S154>/kp_yb'
                                          //    '<S353>/kp_yb'
                                          //    '<S406>/kp_yb'
                                          //    '<S192>/kp_yb'
                                          //    '<S209>/kp_yb'
                                          //    '<S226>/kp_yb'
                                          //    '<S243>/kp_yb'

  real_T Kp_yblue;                     // Variable: Kp_yblue
                                          //  Referenced by:
                                          //    '<S167>/kp_yb'
                                          //    '<S366>/kp_yb'
                                          //    '<S419>/kp_yb'
                                          //    '<S264>/kp_yb'
                                          //    '<S282>/kp_yb'
                                          //    '<S300>/kp_yb'
                                          //    '<S318>/kp_yb'

  real_T Kp_yr;                        // Variable: Kp_yr
                                          //  Referenced by:
                                          //    '<S379>/kp_yr'
                                          //    '<S432>/kp_yr'

  real_T Kv[9];                        // Variable: Kv
                                          //  Referenced by:
                                          //    '<S393>/MATLAB Function'
                                          //    '<S446>/MATLAB Function'

  real_T Phase0_End;                   // Variable: Phase0_End
                                          //  Referenced by: '<Root>/Constant4'

  real_T Phase1_End;                   // Variable: Phase1_End
                                          //  Referenced by: '<Root>/Constant'

  real_T Phase2_End;                   // Variable: Phase2_End
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

  real_T Phase3_End;                   // Variable: Phase3_End
                                          //  Referenced by: '<Root>/Constant2'

  real_T Phase3_SubPhase1_End;         // Variable: Phase3_SubPhase1_End
                                          //  Referenced by:
                                          //    '<S180>/Constant4'
                                          //    '<S181>/Constant4'
                                          //    '<S182>/Constant4'

  real_T Phase3_SubPhase2_End;         // Variable: Phase3_SubPhase2_End
                                          //  Referenced by:
                                          //    '<S180>/Constant1'
                                          //    '<S181>/Constant1'
                                          //    '<S182>/Constant1'

  real_T Phase3_SubPhase3_End;         // Variable: Phase3_SubPhase3_End
                                          //  Referenced by:
                                          //    '<S180>/Constant2'
                                          //    '<S181>/Constant2'
                                          //    '<S182>/Constant2'

  real_T Phase3_SubPhase4_End;         // Variable: Phase3_SubPhase4_End
                                          //  Referenced by:
                                          //    '<S180>/Constant3'
                                          //    '<S181>/Constant3'
                                          //    '<S182>/Constant3'

  real_T Phase4_End;                   // Variable: Phase4_End
                                          //  Referenced by: '<Root>/Constant3'

  real_T Phase5_End;                   // Variable: Phase5_End
                                          //  Referenced by: '<Root>/Constant6'

  real_T Tz_lim_elarm;                 // Variable: Tz_lim_elarm
                                          //  Referenced by:
                                          //    '<S382>/Saturation1'
                                          //    '<S435>/Saturation1'

  real_T Tz_lim_sharm;                 // Variable: Tz_lim_sharm
                                          //  Referenced by:
                                          //    '<S382>/Saturation'
                                          //    '<S435>/Saturation'

  real_T Tz_lim_wrarm;                 // Variable: Tz_lim_wrarm
                                          //  Referenced by:
                                          //    '<S382>/Saturation3'
                                          //    '<S435>/Saturation3'

  real_T baseRate;                     // Variable: baseRate
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

  real_T drop_states_BLACK[3];         // Variable: drop_states_BLACK
                                          //  Referenced by: '<S461>/Velocity to Position'

  real_T drop_states_BLUE[3];          // Variable: drop_states_BLUE
                                          //  Referenced by: '<S473>/Velocity to Position'

  real_T drop_states_RED[3];           // Variable: drop_states_RED
                                          //  Referenced by: '<S499>/Velocity to Position'

  real_T home_states_BLACK[3];         // Variable: home_states_BLACK
                                          //  Referenced by:
                                          //    '<S347>/Desired Attitude (BLACK)'
                                          //    '<S347>/Desired Px (BLACK)'
                                          //    '<S347>/Desired Py (BLACK)'
                                          //    '<S400>/Constant'
                                          //    '<S400>/Constant2'
                                          //    '<S400>/Constant3'

  real_T home_states_BLUE[3];          // Variable: home_states_BLUE
                                          //  Referenced by:
                                          //    '<S348>/Desired Attitude (BLUE)'
                                          //    '<S348>/Desired Px (BLUE)'
                                          //    '<S348>/Desired Py (BLUE)'
                                          //    '<S401>/Desired Attitude (BLUE)'
                                          //    '<S401>/Desired Px (BLUE)'
                                          //    '<S401>/Desired Py (BLUE)'

  real_T home_states_RED[3];           // Variable: home_states_RED
                                          //  Referenced by:
                                          //    '<S349>/Desired Px (BLACK)'
                                          //    '<S349>/Desired Px (BLACK)1'
                                          //    '<S349>/Desired Px (BLACK)2'
                                          //    '<S402>/Desired Px (BLACK)'
                                          //    '<S402>/Desired Px (BLACK)1'
                                          //    '<S402>/Desired Px (BLACK)2'

  real_T init_states_BLACK[3];         // Variable: init_states_BLACK
                                          //  Referenced by:
                                          //    '<S148>/Desired Attitude (BLACK)'
                                          //    '<S148>/Desired Px (BLACK)'
                                          //    '<S148>/Desired Py (BLACK)'

  real_T init_states_BLUE[3];          // Variable: init_states_BLUE
                                          //  Referenced by:
                                          //    '<S149>/Desired Attitude (BLUE)'
                                          //    '<S149>/Desired Px (BLUE)'
                                          //    '<S149>/Desired Py (BLUE)'

  real_T model_param[20];              // Variable: model_param
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

  real_T noise_variance_BLACK;         // Variable: noise_variance_BLACK
                                          //  Referenced by: '<S461>/Random Number'

  real_T noise_variance_BLUE;          // Variable: noise_variance_BLUE
                                          //  Referenced by: '<S473>/Random Number'

  real_T noise_variance_RED;           // Variable: noise_variance_RED
                                          //  Referenced by: '<S499>/Random Number'

  real_T platformSelection;            // Variable: platformSelection
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

  real_T simMode;                      // Variable: simMode
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

  real_T thruster_count_threshold;     // Variable: thruster_count_threshold
                                          //  Referenced by:
                                          //    '<S34>/MATLAB Function10'
                                          //    '<S46>/MATLAB Function6'
                                          //    '<S58>/MATLAB Function5'

  real_T thruster_dist2CG_BLACK[8];    // Variable: thruster_dist2CG_BLACK
                                          //  Referenced by:
                                          //    '<S34>/MATLAB Function'
                                          //    '<S34>/MATLAB Function1'
                                          //    '<S34>/MATLAB Function11'

  real_T thruster_dist2CG_BLUE[8];     // Variable: thruster_dist2CG_BLUE
                                          //  Referenced by:
                                          //    '<S46>/MATLAB Function'
                                          //    '<S46>/MATLAB Function1'
                                          //    '<S46>/MATLAB Function7'

  real_T thruster_dist2CG_RED[8];      // Variable: thruster_dist2CG_RED
                                          //  Referenced by:
                                          //    '<S58>/MATLAB Function'
                                          //    '<S58>/MATLAB Function1'
                                          //    '<S58>/MATLAB Function3'
                                          //    '<S58>/MATLAB Function4'
                                          //    '<S58>/MATLAB Function6'

  real_T xLength;                      // Variable: xLength
                                          //  Referenced by:
                                          //    '<S184>/Desired X-Position (BLACK)'
                                          //    '<S185>/Desired X-Position (BLACK)'
                                          //    '<S186>/Desired X-Position (BLACK)'
                                          //    '<S187>/Desired X-Position (BLACK)'
                                          //    '<S257>/MATLAB Function2'
                                          //    '<S258>/MATLAB Function2'
                                          //    '<S259>/MATLAB Function2'
                                          //    '<S260>/MATLAB Function2'

  real_T yLength;                      // Variable: yLength
                                          //  Referenced by:
                                          //    '<S184>/Desired Y-Position (BLACK)'
                                          //    '<S185>/Desired Y-Position (BLACK)'
                                          //    '<S186>/Desired Y-Position (BLACK)'
                                          //    '<S187>/Desired Y-Position (BLACK)'
                                          //    '<S257>/MATLAB Function3'
                                          //    '<S258>/MATLAB Function3'
                                          //    '<S259>/MATLAB Function3'
                                          //    '<S260>/MATLAB Function3'

  real_T InitializationBlockforSpeedCont;// Expression: 100
                                            //  Referenced by: '<Root>/Initialization Block for Speed Control'

  real_T InitializationBlockforSpeedCo_d;// Expression: 1920
                                            //  Referenced by: '<Root>/Initialization Block for Speed Control'

  real_T InitializationBlockforSpeedCo_k;// Expression: 1023
                                            //  Referenced by: '<Root>/Initialization Block for Speed Control'

  real_T InitializationBlockforSpeedCo_g;// Expression: 2000
                                            //  Referenced by: '<Root>/Initialization Block for Speed Control'

  real_T BLACKPWM_Y0;                  // Computed Parameter: BLACKPWM_Y0
                                          //  Referenced by: '<S29>/BLACK PWM'

  real_T RemoveNegatives_UpperSat;     // Expression: 1000
                                          //  Referenced by: '<S34>/Remove Negatives'

  real_T RemoveNegatives_LowerSat;     // Expression: 0
                                          //  Referenced by: '<S34>/Remove Negatives'

  real_T BLUEPWM_Y0;                   // Computed Parameter: BLUEPWM_Y0
                                          //  Referenced by: '<S30>/BLUE PWM'

  real_T RemoveNegatives_UpperSat_p;   // Expression: 1000
                                          //  Referenced by: '<S46>/Remove Negatives'

  real_T RemoveNegatives_LowerSat_c;   // Expression: 0
                                          //  Referenced by: '<S46>/Remove Negatives'

  real_T REDPWM_Y0;                    // Computed Parameter: REDPWM_Y0
                                          //  Referenced by: '<S31>/RED PWM'

  real_T RemoveNegatives_UpperSat_o;   // Expression: 1000
                                          //  Referenced by: '<S58>/Remove Negatives'

  real_T RemoveNegatives_LowerSat_d;   // Expression: 0
                                          //  Referenced by: '<S58>/Remove Negatives'

  real_T DigitalRead_SampleTime;       // Expression: sampleTime
                                          //  Referenced by: '<S92>/Digital Read'

  real_T SaturateTorque_UpperSat;      // Expression: 0.1
                                          //  Referenced by: '<S80>/Saturate Torque'

  real_T SaturateTorque_LowerSat;      // Expression: -0.1
                                          //  Referenced by: '<S80>/Saturate Torque'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S80>/Delay'

  real_T GearboxRatio_Value;           // Expression: 3375/64
                                          //  Referenced by: '<S84>/Gearbox  Ratio'

  real_T SaturateMotor_UpperSat;       // Expression: 7000
                                          //  Referenced by: '<S81>/Saturate Motor'

  real_T SaturateMotor_LowerSat;       // Expression: -7000
                                          //  Referenced by: '<S81>/Saturate Motor'

  real_T SaturateRPM_UpperSat;         // Expression: (7000*64/3375)
                                          //  Referenced by: '<S80>/Saturate RPM'

  real_T SaturateRPM_LowerSat;         // Expression: -(7000*64/3375)
                                          //  Referenced by: '<S80>/Saturate RPM'

  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S113>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S114>/Out1'

  real_T Out1_Y0_a;                    // Computed Parameter: Out1_Y0_a
                                          //  Referenced by: '<S115>/Out1'

  real_T Out1_Y0_pf;                   // Computed Parameter: Out1_Y0_pf
                                          //  Referenced by: '<S116>/Out1'

  real_T Out1_Y0_k;                    // Computed Parameter: Out1_Y0_k
                                          //  Referenced by: '<S117>/Out1'

  real_T Out1_Y0_f;                    // Computed Parameter: Out1_Y0_f
                                          //  Referenced by: '<S118>/Out1'

  real_T Out1_Y0_m;                    // Computed Parameter: Out1_Y0_m
                                          //  Referenced by: '<S119>/Out1'

  real_T Out1_Y0_po;                   // Computed Parameter: Out1_Y0_po
                                          //  Referenced by: '<S120>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S121>/Out1'

  real_T Out1_Y0_pl;                   // Computed Parameter: Out1_Y0_pl
                                          //  Referenced by: '<S122>/Out1'

  real_T Out1_Y0_aq;                   // Computed Parameter: Out1_Y0_aq
                                          //  Referenced by: '<S123>/Out1'

  real_T Out1_Y0_au;                   // Computed Parameter: Out1_Y0_au
                                          //  Referenced by: '<S124>/Out1'

  real_T Out1_Y0_b;                    // Computed Parameter: Out1_Y0_b
                                          //  Referenced by: '<S125>/Out1'

  real_T Out1_Y0_bw;                   // Computed Parameter: Out1_Y0_bw
                                          //  Referenced by: '<S126>/Out1'

  real_T Out1_Y0_c;                    // Computed Parameter: Out1_Y0_c
                                          //  Referenced by: '<S127>/Out1'

  real_T Out1_Y0_i;                    // Computed Parameter: Out1_Y0_i
                                          //  Referenced by: '<S468>/Out1'

  real_T Out1_Y0_bu;                   // Computed Parameter: Out1_Y0_bu
                                          //  Referenced by: '<S469>/Out1'

  real_T Out1_Y0_j;                    // Computed Parameter: Out1_Y0_j
                                          //  Referenced by: '<S470>/Out1'

  real_T Out1_Y0_mq;                   // Computed Parameter: Out1_Y0_mq
                                          //  Referenced by: '<S471>/Out1'

  real_T Out1_Y0_e;                    // Computed Parameter: Out1_Y0_e
                                          //  Referenced by: '<S472>/Out1'

  real_T Out1_Y0_eo;                   // Computed Parameter: Out1_Y0_eo
                                          //  Referenced by: '<S480>/Out1'

  real_T Out1_Y0_d;                    // Computed Parameter: Out1_Y0_d
                                          //  Referenced by: '<S481>/Out1'

  real_T Out1_Y0_ed;                   // Computed Parameter: Out1_Y0_ed
                                          //  Referenced by: '<S482>/Out1'

  real_T Out1_Y0_p0;                   // Computed Parameter: Out1_Y0_p0
                                          //  Referenced by: '<S483>/Out1'

  real_T Out1_Y0_ia;                   // Computed Parameter: Out1_Y0_ia
                                          //  Referenced by: '<S484>/Out1'

  real_T Out1_Y0_h;                    // Computed Parameter: Out1_Y0_h
                                          //  Referenced by: '<S496>/Out1'

  real_T Out1_Y0_hv;                   // Computed Parameter: Out1_Y0_hv
                                          //  Referenced by: '<S497>/Out1'

  real_T Delay_InitialCondition_k;     // Expression: 0.0
                                          //  Referenced by: '<S490>/Delay'

  real_T AccelerationtoVelocity_gainval;
                           // Computed Parameter: AccelerationtoVelocity_gainval
                              //  Referenced by: '<S490>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC[12];
                    // Expression: [drop_states_RED,drop_states_ARM,0,0,0,0,0,0]
                       //  Referenced by: '<S490>/Acceleration  to Velocity'

  real_T RandomNumber2_Mean;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number2'

  real_T RandomNumber2_StdDev;       // Computed Parameter: RandomNumber2_StdDev
                                        //  Referenced by: '<S490>/Random Number2'

  real_T RandomNumber2_Seed;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number2'

  real_T RandomNumber1_Mean;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number1'

  real_T RandomNumber1_StdDev;       // Computed Parameter: RandomNumber1_StdDev
                                        //  Referenced by: '<S490>/Random Number1'

  real_T RandomNumber1_Seed;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number1'

  real_T RandomNumber3_Mean;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number3'

  real_T RandomNumber3_StdDev;       // Computed Parameter: RandomNumber3_StdDev
                                        //  Referenced by: '<S490>/Random Number3'

  real_T RandomNumber3_Seed;           // Expression: 0
                                          //  Referenced by: '<S490>/Random Number3'

  real_T Out1_Y0_l;                    // Computed Parameter: Out1_Y0_l
                                          //  Referenced by: '<S506>/Out1'

  real_T Out1_Y0_f2;                   // Computed Parameter: Out1_Y0_f2
                                          //  Referenced by: '<S507>/Out1'

  real_T Out1_Y0_fu;                   // Computed Parameter: Out1_Y0_fu
                                          //  Referenced by: '<S508>/Out1'

  real_T Out1_Y0_lh;                   // Computed Parameter: Out1_Y0_lh
                                          //  Referenced by: '<S509>/Out1'

  real_T Out1_Y0_a3;                   // Computed Parameter: Out1_Y0_a3
                                          //  Referenced by: '<S510>/Out1'

  real_T AccelerationtoVelocity_gainva_k;
                          // Computed Parameter: AccelerationtoVelocity_gainva_k
                             //  Referenced by: '<S499>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_c;  // Expression: 0
                                          //  Referenced by: '<S499>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval;
                               // Computed Parameter: VelocitytoPosition_gainval
                                  //  Referenced by: '<S499>/Velocity to Position'

  real_T RandomNumber_Mean;            // Expression: 0
                                          //  Referenced by: '<S499>/Random Number'

  real_T RandomNumber_Seed;            // Expression: 0
                                          //  Referenced by: '<S499>/Random Number'

  real_T RandomNumber7_Mean;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number7'

  real_T RandomNumber7_StdDev;       // Computed Parameter: RandomNumber7_StdDev
                                        //  Referenced by: '<S498>/Random Number7'

  real_T RandomNumber7_Seed;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number7'

  real_T RandomNumber6_Mean;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number6'

  real_T RandomNumber6_StdDev;       // Computed Parameter: RandomNumber6_StdDev
                                        //  Referenced by: '<S498>/Random Number6'

  real_T RandomNumber6_Seed;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number6'

  real_T RandomNumber8_Mean;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number8'

  real_T RandomNumber8_StdDev;       // Computed Parameter: RandomNumber8_StdDev
                                        //  Referenced by: '<S498>/Random Number8'

  real_T RandomNumber8_Seed;           // Expression: 0
                                          //  Referenced by: '<S498>/Random Number8'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S485>/Constant'

  real_T Constant_Value_o;             // Expression: 0
                                          //  Referenced by: '<S486>/Constant'

  real_T Constant_Value_i;             // Expression: 0
                                          //  Referenced by: '<S487>/Constant'

  real_T AccelerationtoVelocity_gainva_i;
                          // Computed Parameter: AccelerationtoVelocity_gainva_i
                             //  Referenced by: '<S461>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_i;  // Expression: 0
                                          //  Referenced by: '<S461>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_c;
                             // Computed Parameter: VelocitytoPosition_gainval_c
                                //  Referenced by: '<S461>/Velocity to Position'

  real_T RandomNumber_Mean_n;          // Expression: 0
                                          //  Referenced by: '<S461>/Random Number'

  real_T RandomNumber_Seed_h;          // Expression: 0
                                          //  Referenced by: '<S461>/Random Number'

  real_T RandomNumber1_Mean_i;         // Expression: 0
                                          //  Referenced by: '<S457>/Random Number1'

  real_T RandomNumber1_StdDev_o;   // Computed Parameter: RandomNumber1_StdDev_o
                                      //  Referenced by: '<S457>/Random Number1'

  real_T RandomNumber1_Seed_m;         // Expression: 0
                                          //  Referenced by: '<S457>/Random Number1'

  real_T RandomNumber_Mean_e;          // Expression: 0
                                          //  Referenced by: '<S457>/Random Number'

  real_T RandomNumber_StdDev;         // Computed Parameter: RandomNumber_StdDev
                                         //  Referenced by: '<S457>/Random Number'

  real_T RandomNumber_Seed_a;          // Expression: 0
                                          //  Referenced by: '<S457>/Random Number'

  real_T RandomNumber2_Mean_c;         // Expression: 0
                                          //  Referenced by: '<S457>/Random Number2'

  real_T RandomNumber2_StdDev_p;   // Computed Parameter: RandomNumber2_StdDev_p
                                      //  Referenced by: '<S457>/Random Number2'

  real_T RandomNumber2_Seed_i;         // Expression: 0
                                          //  Referenced by: '<S457>/Random Number2'

  real_T AccelerationtoVelocity_gainva_b;
                          // Computed Parameter: AccelerationtoVelocity_gainva_b
                             //  Referenced by: '<S473>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_p;  // Expression: 0
                                          //  Referenced by: '<S473>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_f;
                             // Computed Parameter: VelocitytoPosition_gainval_f
                                //  Referenced by: '<S473>/Velocity to Position'

  real_T RandomNumber_Mean_b;          // Expression: 0
                                          //  Referenced by: '<S473>/Random Number'

  real_T RandomNumber_Seed_k;          // Expression: 0
                                          //  Referenced by: '<S473>/Random Number'

  real_T RandomNumber4_Mean;           // Expression: 0
                                          //  Referenced by: '<S458>/Random Number4'

  real_T RandomNumber4_StdDev;       // Computed Parameter: RandomNumber4_StdDev
                                        //  Referenced by: '<S458>/Random Number4'

  real_T RandomNumber4_Seed;           // Expression: 0
                                          //  Referenced by: '<S458>/Random Number4'

  real_T RandomNumber3_Mean_o;         // Expression: 0
                                          //  Referenced by: '<S458>/Random Number3'

  real_T RandomNumber3_StdDev_j;   // Computed Parameter: RandomNumber3_StdDev_j
                                      //  Referenced by: '<S458>/Random Number3'

  real_T RandomNumber3_Seed_e;         // Expression: 0
                                          //  Referenced by: '<S458>/Random Number3'

  real_T RandomNumber5_Mean;           // Expression: 0
                                          //  Referenced by: '<S458>/Random Number5'

  real_T RandomNumber5_StdDev;       // Computed Parameter: RandomNumber5_StdDev
                                        //  Referenced by: '<S458>/Random Number5'

  real_T RandomNumber5_Seed;           // Expression: 0
                                          //  Referenced by: '<S458>/Random Number5'

  real_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<S459>/Discrete-Time Integrator'

  real_T DiscreteTimeIntegrator1_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainval
                             //  Referenced by: '<S459>/Discrete-Time Integrator1'

  real_T DiscreteTimeIntegrator2_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator2_gainval
                             //  Referenced by: '<S459>/Discrete-Time Integrator2'

  real_T Delay1_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S130>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S130>/Delay2'

  real_T Delay5_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S130>/Delay5'

  real_T Delay3_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S130>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S130>/Delay4'

  real_T Delay1_InitialCondition_j;    // Expression: 0
                                          //  Referenced by: '<S131>/Delay1'

  real_T Delay2_InitialCondition_j;    // Expression: 0
                                          //  Referenced by: '<S131>/Delay2'

  real_T Delay5_InitialCondition_g;    // Expression: 0
                                          //  Referenced by: '<S131>/Delay5'

  real_T Delay3_InitialCondition_k;    // Expression: 0
                                          //  Referenced by: '<S131>/Delay3'

  real_T Delay4_InitialCondition_m;    // Expression: 0
                                          //  Referenced by: '<S131>/Delay4'

  real_T Delay1_InitialCondition_n;    // Expression: 0
                                          //  Referenced by: '<S132>/Delay1'

  real_T Delay2_InitialCondition_p;    // Expression: 0
                                          //  Referenced by: '<S132>/Delay2'

  real_T Delay5_InitialCondition_o;    // Expression: 0
                                          //  Referenced by: '<S132>/Delay5'

  real_T Delay3_InitialCondition_e;    // Expression: 0
                                          //  Referenced by: '<S132>/Delay3'

  real_T Delay4_InitialCondition_h;    // Expression: 0
                                          //  Referenced by: '<S132>/Delay4'

  real_T WhoAmI_SampleTime;            // Expression: 0.1
                                          //  Referenced by: '<S17>/WhoAmI'

  real_T Out1_Y0_aw;                   // Computed Parameter: Out1_Y0_aw
                                          //  Referenced by: '<S157>/Out1'

  real_T Out1_Y0_av;                   // Computed Parameter: Out1_Y0_av
                                          //  Referenced by: '<S159>/Out1'

  real_T Out1_Y0_h4;                   // Computed Parameter: Out1_Y0_h4
                                          //  Referenced by: '<S161>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S148>/Puck State'

  real_T Out1_Y0_mf;                   // Computed Parameter: Out1_Y0_mf
                                          //  Referenced by: '<S170>/Out1'

  real_T Out1_Y0_a4;                   // Computed Parameter: Out1_Y0_a4
                                          //  Referenced by: '<S172>/Out1'

  real_T Out1_Y0_kw;                   // Computed Parameter: Out1_Y0_kw
                                          //  Referenced by: '<S174>/Out1'

  real_T PuckState_Value_o;            // Expression: 1
                                          //  Referenced by: '<S149>/Puck State'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S179>/Constant1'

  real_T Constant7_Value;              // Expression: 0
                                          //  Referenced by: '<S178>/Constant7'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S150>/Constant2'

  real_T PuckState_Value_l;            // Expression: 1
                                          //  Referenced by: '<S150>/Puck State'

  real_T Constant1_Value_n;            // Expression: 0
                                          //  Referenced by: '<S344>/Constant1'

  real_T Constant7_Value_k;            // Expression: 0
                                          //  Referenced by: '<S343>/Constant7'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S336>/Constant4'

  real_T PuckState_Value_lm;           // Expression: 1
                                          //  Referenced by: '<S336>/Puck State'

  real_T Constant1_Value_h;            // Expression: 1
                                          //  Referenced by: '<S346>/Constant1'

  real_T Gain_Gain;                    // Expression: pi/180
                                          //  Referenced by: '<S346>/Gain'

  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S346>/Constant3'

  real_T Constant4_Value_l;            // Expression: 0
                                          //  Referenced by: '<S346>/Constant4'

  real_T Constant7_Value_f;            // Expression: 0
                                          //  Referenced by: '<S345>/Constant7'

  real_T Constant1_Value_f;            // Expression: 0
                                          //  Referenced by: '<S337>/Constant1'

  real_T PuckState_Value_f;            // Expression: 1
                                          //  Referenced by: '<S337>/Puck State'

  real_T Out1_Y0_o;                    // Computed Parameter: Out1_Y0_o
                                          //  Referenced by: '<S356>/Out1'

  real_T Out1_Y0_ii;                   // Computed Parameter: Out1_Y0_ii
                                          //  Referenced by: '<S358>/Out1'

  real_T Out1_Y0_hq;                   // Computed Parameter: Out1_Y0_hq
                                          //  Referenced by: '<S360>/Out1'

  real_T PuckState_Value_fj;           // Expression: 1
                                          //  Referenced by: '<S347>/Puck State'

  real_T Out1_Y0_lc;                   // Computed Parameter: Out1_Y0_lc
                                          //  Referenced by: '<S384>/Out1'

  real_T Out1_Y0_of;                   // Computed Parameter: Out1_Y0_of
                                          //  Referenced by: '<S386>/Out1'

  real_T Out1_Y0_l2;                   // Computed Parameter: Out1_Y0_l2
                                          //  Referenced by: '<S388>/Out1'

  real_T Constant7_Value_m;            // Expression: 0
                                          //  Referenced by: '<S381>/Constant7'

  real_T Constant2_Value_g;            // Expression: 0
                                          //  Referenced by: '<S349>/Constant2'

  real_T PuckState_Value_od;           // Expression: 1
                                          //  Referenced by: '<S349>/Puck State'

  real_T Out1_Y0_by;                   // Computed Parameter: Out1_Y0_by
                                          //  Referenced by: '<S409>/Out1'

  real_T Out1_Y0_c2;                   // Computed Parameter: Out1_Y0_c2
                                          //  Referenced by: '<S411>/Out1'

  real_T Out1_Y0_f2b;                  // Computed Parameter: Out1_Y0_f2b
                                          //  Referenced by: '<S413>/Out1'

  real_T PuckState_Value_g;            // Expression: 1
                                          //  Referenced by: '<S400>/Puck State'

  real_T Out1_Y0_jf;                   // Computed Parameter: Out1_Y0_jf
                                          //  Referenced by: '<S437>/Out1'

  real_T Out1_Y0_fn;                   // Computed Parameter: Out1_Y0_fn
                                          //  Referenced by: '<S439>/Out1'

  real_T Out1_Y0_a2;                   // Computed Parameter: Out1_Y0_a2
                                          //  Referenced by: '<S441>/Out1'

  real_T Constant7_Value_a;            // Expression: 0
                                          //  Referenced by: '<S434>/Constant7'

  real_T Constant1_Value_k;            // Expression: 0
                                          //  Referenced by: '<S402>/Constant1'

  real_T PuckState_Value_e;            // Expression: 1
                                          //  Referenced by: '<S402>/Puck State'

  real_T Constant3_Value_d;            // Expression: 0
                                          //  Referenced by: '<S453>/Constant3'

  real_T Constant4_Value_e;            // Expression: 0
                                          //  Referenced by: '<S453>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S453>/Constant5'

  real_T PuckState_Value_gd;           // Expression: 0
                                          //  Referenced by: '<S453>/Puck State'

  real_T Constant_Value_j;             // Expression: 0
                                          //  Referenced by: '<S455>/Constant'

  real_T Constant1_Value_l;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant1'

  real_T Constant2_Value_m;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant2'

  real_T Constant4_Value_o;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant4'

  real_T Constant5_Value_f;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S455>/Constant6'

  real_T PuckState_Value_h;            // Expression: 0
                                          //  Referenced by: '<S455>/Puck State'

  real_T Gain_Gain_a;                  // Expression: -1
                                          //  Referenced by: '<S455>/Gain'

  real_T Saturation_UpperSat;          // Expression: 0.005
                                          //  Referenced by: '<S455>/Saturation'

  real_T Saturation_LowerSat;          // Expression: -0.005
                                          //  Referenced by: '<S455>/Saturation'

  real_T RED_Px13_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px13'

  real_T RED_Px14_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px14'

  real_T RED_Px15_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px15'

  real_T RED_Px16_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px16'

  real_T RED_Px17_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px17'

  real_T RED_Px18_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px18'

  real_T RED_Px27_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px27'

  real_T RED_Px28_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px28'

  real_T RED_Px33_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px33'

  real_T RED_Px36_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px36'

  real_T RED_Px39_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px39'

  real_T RED_Tz10_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz10'

  real_T RED_Tz11_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz11'

  real_T RED_Tz9_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz9'

  real_T BLACK_Fx_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fx'

  real_T BLACK_Fx1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fx1'

  real_T BLACK_Fx_Sat_InitialValue;    // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fx_Sat'

  real_T BLACK_Fx_Sat1_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fx_Sat1'

  real_T BLACK_Fx_Sat2_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fx_Sat2'

  real_T BLACK_Fy_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fy'

  real_T BLACK_Fy1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fy1'

  real_T BLACK_Fy_Sat_InitialValue;    // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fy_Sat'

  real_T BLACK_Fy_Sat1_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fy_Sat1'

  real_T BLACK_Fy_Sat2_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Fy_Sat2'

  real_T BLACK_Px_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Px'

  real_T BLACK_Px1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Px1'

  real_T BLACK_Py_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Py'

  real_T BLACK_Py1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Py1'

  real_T BLACK_Rz_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Rz'

  real_T BLACK_Rz1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Rz1'

  real_T BLACK_Tz_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Tz'

  real_T BLACK_Tz1_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Tz1'

  real_T BLACK_Tz_Sat_InitialValue;    // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Tz_Sat'

  real_T BLACK_Tz_Sat1_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Tz_Sat1'

  real_T BLACK_Tz_Sat2_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/BLACK_Tz_Sat2'

  real_T RED_Fx_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Fx'

  real_T RED_Fx_Sat_InitialValue;      // Expression: 0
                                          //  Referenced by: '<Root>/RED_Fx_Sat'

  real_T RED_Fy_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Fy'

  real_T RED_Fy_Sat_InitialValue;      // Expression: 0
                                          //  Referenced by: '<Root>/RED_Fy_Sat'

  real_T RED_Px_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px'

  real_T RED_Px1_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px1'

  real_T RED_Px10_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px10'

  real_T RED_Px11_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px11'

  real_T RED_Px12_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px12'

  real_T RED_Px19_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px19'

  real_T RED_Px2_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px2'

  real_T RED_Px20_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px20'

  real_T RED_Px21_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px21'

  real_T RED_Px22_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px22'

  real_T RED_Px23_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px23'

  real_T RED_Px24_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px24'

  real_T RED_Px25_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px25'

  real_T RED_Px26_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px26'

  real_T RED_Px29_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px29'

  real_T RED_Px3_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px3'

  real_T RED_Px30_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px30'

  real_T RED_Px31_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px31'

  real_T RED_Px32_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px32'

  real_T RED_Px34_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px34'

  real_T RED_Px35_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px35'

  real_T RED_Px37_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px37'

  real_T RED_Px38_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px38'

  real_T RED_Px4_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px4'

  real_T RED_Px5_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px5'

  real_T RED_Px6_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px6'

  real_T RED_Px7_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px7'

  real_T RED_Px8_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px8'

  real_T RED_Px9_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px9'

  real_T RED_Py_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Py'

  real_T RED_Rz_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Rz'

  real_T RED_Tz_InitialValue;          // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz'

  real_T RED_Tz1_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz1'

  real_T RED_Tz12_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz12'

  real_T RED_Tz13_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz13'

  real_T RED_Tz14_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz14'

  real_T RED_Tz2_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz2'

  real_T RED_Tz3_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz3'

  real_T RED_Tz4_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz4'

  real_T RED_Tz5_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz5'

  real_T RED_Tz6_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz6'

  real_T RED_Tz7_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz7'

  real_T RED_Tz8_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz8'

  real_T RED_Tz_RW_InitialValue;       // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_RW'

  real_T RED_Tz_RWSat_InitialValue;    // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_RW Sat'

  real_T RED_Tz_RWSat1_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_RW Sat1'

  real_T RED_Tz_RW1_InitialValue;      // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_RW1'

  real_T RED_Tz_Sat_InitialValue;      // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_Sat'

  real_T RED_Tz_Sat1_InitialValue;     // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz_Sat1'

  real_T RED_dRz_RWSat_InitialValue;   // Expression: 0
                                          //  Referenced by: '<Root>/RED_dRz_RW Sat'

  real_T Universal_Time_InitialValue;  // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time'

  real_T Universal_Time1_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time1'

  real_T Universal_Time10_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time10'

  real_T Universal_Time11_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time11'

  real_T Universal_Time12_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time12'

  real_T Universal_Time13_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time13'

  real_T Universal_Time14_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time14'

  real_T Universal_Time15_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time15'

  real_T Universal_Time16_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time16'

  real_T Universal_Time17_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time17'

  real_T Universal_Time18_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time18'

  real_T Universal_Time19_InitialValue;// Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time19'

  real_T Universal_Time2_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time2'

  real_T Universal_Time3_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time3'

  real_T Universal_Time4_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time4'

  real_T Universal_Time5_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time5'

  real_T Universal_Time6_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time6'

  real_T Universal_Time7_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time7'

  real_T Universal_Time8_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time8'

  real_T Universal_Time9_InitialValue; // Expression: 0
                                          //  Referenced by: '<Root>/Universal_Time9'

  int32_T UDPReceive_Port;             // Computed Parameter: UDPReceive_Port
                                          //  Referenced by: '<S97>/UDP Receive'

  int32_T UDPSend_Port;                // Computed Parameter: UDPSend_Port
                                          //  Referenced by: '<S18>/UDP Send'

  boolean_T Delay1_InitialCondition_m;
                                // Computed Parameter: Delay1_InitialCondition_m
                                   //  Referenced by: '<S85>/Delay1'

  boolean_T Delay2_InitialCondition_k;
                                // Computed Parameter: Delay2_InitialCondition_k
                                   //  Referenced by: '<S85>/Delay2'

  boolean_T Delay5_InitialCondition_e;
                                // Computed Parameter: Delay5_InitialCondition_e
                                   //  Referenced by: '<S85>/Delay5'

  boolean_T Delay3_InitialCondition_i;
                                // Computed Parameter: Delay3_InitialCondition_i
                                   //  Referenced by: '<S85>/Delay3'

  boolean_T Delay4_InitialCondition_d;
                                // Computed Parameter: Delay4_InitialCondition_d
                                   //  Referenced by: '<S85>/Delay4'

  P_ChangeBLUEBehavior_Parker_V_T ChangeBLUEBehavior_k;// '<S16>/Change BLUE Behavior' 
  P_REDARM_Parker_Velocity_Cont_T REDARM_bo;// '<S402>/RED+ARM'
  P_ChangeBLUEBehavior_Parker_c_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  P_REDARM_Parker_Velocity_Cont_T REDARM_p;// '<S349>/RED+ARM'
  P_ChangeBLUEBehavior_Parker_c_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  P_SubPhase1_Parker_Velocity_m_T SubPhase2_g;// '<S182>/Sub-Phase #2 '
  P_SubPhase1_Parker_Velocity_m_T SubPhase1_j;// '<S182>/Sub-Phase #1'
  P_SubPhase1_Parker_Velocity_k_T SubPhase4_f;// '<S181>/Sub-Phase #4'
  P_SubPhase1_Parker_Velocity_k_T SubPhase3_p;// '<S181>/Sub-Phase #3 '
  P_SubPhase1_Parker_Velocity_k_T SubPhase2_m;// '<S181>/Sub-Phase #2 '
  P_SubPhase1_Parker_Velocity_k_T SubPhase1_a;// '<S181>/Sub-Phase #1'
  P_SubPhase1_Parker_Velocity_C_T SubPhase4;// '<S180>/Sub-Phase #4'
  P_SubPhase1_Parker_Velocity_C_T SubPhase3;// '<S180>/Sub-Phase #3 '
  P_SubPhase1_Parker_Velocity_C_T SubPhase2;// '<S180>/Sub-Phase #2 '
  P_SubPhase1_Parker_Velocity_C_T SubPhase1;// '<S180>/Sub-Phase #1'
  P_Phase0WaitforSynchronizatio_T Phase1StartFloating;// '<Root>/Phase #1:  Start Floating ' 
  P_Phase0WaitforSynchronizatio_T Phase0WaitforSynchronization;
                                // '<Root>/Phase #0:  Wait for Synchronization'
};

// Real-time Model Data Structure
struct tag_RTM_Parker_Velocity_Contr_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    struct {
      uint8_T TID[3];
    } TaskCounters;

    struct {
      boolean_T TID1_2;
    } RateInteraction;

    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[3];
  } Timing;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_Parker_Velocity_Control_T Parker_Velocity_Control_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern B_Parker_Velocity_Control_T Parker_Velocity_Control_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern DW_Parker_Velocity_Control_T Parker_Velocity_Control_DW;

// External function called from main
#ifdef __cplusplus

extern "C" {

#endif

  extern void Parker_Velocity_Control_SetEventsForThisBaseStep(boolean_T
    *eventFlags);

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void Parker_Velocity_Control_SetEventsForThisBaseStep(boolean_T
    *eventFlags);
  extern void Parker_Velocity_Control_initialize(void);
  extern void Parker_Velocity_Control_step(int_T tid);
  extern void Parker_Velocity_Control_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_Parker_Velocity_Cont_T *const Parker_Velocity_Control_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S97>/Reshape' : Reshape block reduction
//  Block '<S395>/Reshape' : Reshape block reduction
//  Block '<S448>/Reshape' : Reshape block reduction


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Parker_Velocity_Control'
//  '<S1>'   : 'Parker_Velocity_Control/Data Logger Subsystem'
//  '<S2>'   : 'Parker_Velocity_Control/Float & Magnet Controls'
//  '<S3>'   : 'Parker_Velocity_Control/From ARM Torques Command Joints'
//  '<S4>'   : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal'
//  '<S5>'   : 'Parker_Velocity_Control/From Torque Command RW'
//  '<S6>'   : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP'
//  '<S7>'   : 'Parker_Velocity_Control/Is this a  simulation?'
//  '<S8>'   : 'Parker_Velocity_Control/Manipulator Encoder Data'
//  '<S9>'   : 'Parker_Velocity_Control/Manipulator  Initialization'
//  '<S10>'  : 'Parker_Velocity_Control/Phase #0:  Wait for Synchronization'
//  '<S11>'  : 'Parker_Velocity_Control/Phase #1:  Start Floating '
//  '<S12>'  : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position'
//  '<S13>'  : 'Parker_Velocity_Control/Phase #3: Experiment'
//  '<S14>'  : 'Parker_Velocity_Control/Phase #4:  Return Home'
//  '<S15>'  : 'Parker_Velocity_Control/Phase #5:  Hold Home'
//  '<S16>'  : 'Parker_Velocity_Control/Phase #6:  Stop Floating and Spin Down RW'
//  '<S17>'  : 'Parker_Velocity_Control/Platform Identification'
//  '<S18>'  : 'Parker_Velocity_Control/Real-Time Visualization'
//  '<S19>'  : 'Parker_Velocity_Control/Simulate Plant Dynamics'
//  '<S20>'  : 'Parker_Velocity_Control/Subsystem'
//  '<S21>'  : 'Parker_Velocity_Control/Float & Magnet Controls/Change Behavior'
//  '<S22>'  : 'Parker_Velocity_Control/Float & Magnet Controls/Is this a  simulation?'
//  '<S23>'  : 'Parker_Velocity_Control/Float & Magnet Controls/Change Behavior/GPIO for Magnet'
//  '<S24>'  : 'Parker_Velocity_Control/Float & Magnet Controls/Change Behavior/GPIO for Pucks'
//  '<S25>'  : 'Parker_Velocity_Control/From ARM Torques Command Joints/Change BLACK Behavior'
//  '<S26>'  : 'Parker_Velocity_Control/From ARM Torques Command Joints/Change BLUE Behavior'
//  '<S27>'  : 'Parker_Velocity_Control/From ARM Torques Command Joints/Change RED Behavior'
//  '<S28>'  : 'Parker_Velocity_Control/From ARM Torques Command Joints/Is this a  simulation?'
//  '<S29>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior'
//  '<S30>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior'
//  '<S31>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior'
//  '<S32>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Is this a  simulation?'
//  '<S33>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Send Commands to PWM Blocks'
//  '<S34>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF'
//  '<S35>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Rotate Forces to Body'
//  '<S36>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S37>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S38>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function10'
//  '<S39>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function11'
//  '<S40>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S41>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S42>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S43>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/pseudo-inverse3'
//  '<S44>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S45>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S46>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF'
//  '<S47>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Rotate Forces to Body'
//  '<S48>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S49>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S50>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S51>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function6'
//  '<S52>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function7'
//  '<S53>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S54>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S55>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/pseudo-inverse2'
//  '<S56>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S57>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change BLUE Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S58>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF'
//  '<S59>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Rotate Forces to Body'
//  '<S60>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem'
//  '<S61>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem1'
//  '<S62>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem2'
//  '<S63>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem3'
//  '<S64>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S65>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S66>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S67>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function3'
//  '<S68>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function4'
//  '<S69>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function5'
//  '<S70>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function6'
//  '<S71>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S72>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S73>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/pseudo-inverse1'
//  '<S74>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S75>'  : 'Parker_Velocity_Control/From Force//Torque  to PWM Signal/Change RED Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S76>'  : 'Parker_Velocity_Control/From Torque Command RW/Change BLACK Behavior'
//  '<S77>'  : 'Parker_Velocity_Control/From Torque Command RW/Change BLUE Behavior'
//  '<S78>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior'
//  '<S79>'  : 'Parker_Velocity_Control/From Torque Command RW/Is this a  simulation?'
//  '<S80>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Calculate Saturated RPM Signal'
//  '<S81>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM'
//  '<S82>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Calculate Saturated RPM Signal/Calculate RW RPM Increment'
//  '<S83>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Check if RW is Ready'
//  '<S84>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Convert Wheel Rate to  Motor Rate'
//  '<S85>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Delay Bank'
//  '<S86>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Get RW Direction'
//  '<S87>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Obtain RW Status'
//  '<S88>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/RPM to PWM Value'
//  '<S89>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Send Direction to Motor Controller'
//  '<S90>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Send PWM to Motor Controller'
//  '<S91>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Turn on Motor'
//  '<S92>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Obtain RW Status/GPIO Read1'
//  '<S93>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Send Direction to Motor Controller/GPIO Write'
//  '<S94>'  : 'Parker_Velocity_Control/From Torque Command RW/Change RED Behavior/Wheel RPM to PWM/Turn on Motor/GPIO Write1'
//  '<S95>'  : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Initialize Universal Time (Simulation)'
//  '<S96>'  : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Is this a  simulation?1'
//  '<S97>'  : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States'
//  '<S98>'  : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x1'
//  '<S99>'  : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x10'
//  '<S100>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x11'
//  '<S101>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x12'
//  '<S102>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x16'
//  '<S103>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x17'
//  '<S104>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x18'
//  '<S105>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x2'
//  '<S106>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x3'
//  '<S107>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x4'
//  '<S108>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x5'
//  '<S109>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x6'
//  '<S110>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x7'
//  '<S111>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x8'
//  '<S112>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x9'
//  '<S113>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x1/Hold this value'
//  '<S114>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x10/Hold this value'
//  '<S115>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x11/Hold this value'
//  '<S116>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x12/Hold this value'
//  '<S117>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x16/Hold this value'
//  '<S118>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x17/Hold this value'
//  '<S119>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x18/Hold this value'
//  '<S120>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x2/Hold this value'
//  '<S121>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x3/Hold this value'
//  '<S122>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x4/Hold this value'
//  '<S123>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x5/Hold this value'
//  '<S124>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x6/Hold this value'
//  '<S125>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x7/Hold this value'
//  '<S126>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x8/Hold this value'
//  '<S127>' : 'Parker_Velocity_Control/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x9/Hold this value'
//  '<S128>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior'
//  '<S129>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Is this a  simulation?'
//  '<S130>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts'
//  '<S131>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts1'
//  '<S132>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts2'
//  '<S133>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts/Correct Dropouts'
//  '<S134>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts1/Correct Dropouts'
//  '<S135>' : 'Parker_Velocity_Control/Manipulator Encoder Data/Change Behavior/Correct for dropouts2/Correct Dropouts'
//  '<S136>' : 'Parker_Velocity_Control/Manipulator  Initialization/Change BLACK Behavior'
//  '<S137>' : 'Parker_Velocity_Control/Manipulator  Initialization/Change BLUE Behavior'
//  '<S138>' : 'Parker_Velocity_Control/Manipulator  Initialization/Change RED Behavior'
//  '<S139>' : 'Parker_Velocity_Control/Manipulator  Initialization/Is this a  simulation?'
//  '<S140>' : 'Parker_Velocity_Control/Phase #0:  Wait for Synchronization/Change BLACK Behavior'
//  '<S141>' : 'Parker_Velocity_Control/Phase #0:  Wait for Synchronization/Change BLUE Behavior'
//  '<S142>' : 'Parker_Velocity_Control/Phase #0:  Wait for Synchronization/Change RED Behavior'
//  '<S143>' : 'Parker_Velocity_Control/Phase #0:  Wait for Synchronization/Is this a  simulation?'
//  '<S144>' : 'Parker_Velocity_Control/Phase #1:  Start Floating /Change BLACK Behavior'
//  '<S145>' : 'Parker_Velocity_Control/Phase #1:  Start Floating /Change BLUE Behavior'
//  '<S146>' : 'Parker_Velocity_Control/Phase #1:  Start Floating /Change RED Behavior'
//  '<S147>' : 'Parker_Velocity_Control/Phase #1:  Start Floating /Is this a  simulation?'
//  '<S148>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior'
//  '<S149>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior'
//  '<S150>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change RED Behavior'
//  '<S151>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Is this a  simulation?'
//  '<S152>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S153>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S154>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S155>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control'
//  '<S156>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S157>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S158>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S159>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S160>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S161>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S162>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S163>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S164>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S165>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S166>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S167>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S168>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control'
//  '<S169>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S170>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S171>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S172>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S173>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S174>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S175>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S176>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S177>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S178>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change RED Behavior/RED'
//  '<S179>' : 'Parker_Velocity_Control/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM'
//  '<S180>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior'
//  '<S181>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior'
//  '<S182>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior'
//  '<S183>' : 'Parker_Velocity_Control/Phase #3: Experiment/Is this a  simulation?'
//  '<S184>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1'
//  '<S185>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 '
//  '<S186>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 '
//  '<S187>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4'
//  '<S188>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Subsystem'
//  '<S189>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1'
//  '<S190>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S191>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S192>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S193>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control'
//  '<S194>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Subsystem'
//  '<S195>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1'
//  '<S196>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S197>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S198>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S199>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S200>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S201>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S202>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S203>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S204>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S205>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S206>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1'
//  '<S207>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)'
//  '<S208>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)'
//  '<S209>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)'
//  '<S210>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control'
//  '<S211>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Subsystem'
//  '<S212>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1'
//  '<S213>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S214>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S215>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S216>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S217>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S218>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S219>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S220>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function2'
//  '<S221>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function3'
//  '<S222>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function4'
//  '<S223>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1'
//  '<S224>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)'
//  '<S225>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)'
//  '<S226>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)'
//  '<S227>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control'
//  '<S228>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Subsystem'
//  '<S229>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Wrap1'
//  '<S230>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S231>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S232>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S233>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S234>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S235>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S236>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S237>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function2'
//  '<S238>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function3'
//  '<S239>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function4'
//  '<S240>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1'
//  '<S241>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S242>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S243>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S244>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control'
//  '<S245>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Subsystem'
//  '<S246>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1'
//  '<S247>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S248>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S249>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S250>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S251>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S252>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S253>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S254>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S255>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S256>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S257>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1'
//  '<S258>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 '
//  '<S259>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 '
//  '<S260>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4'
//  '<S261>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Subsystem'
//  '<S262>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S263>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S264>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S265>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control'
//  '<S266>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function2'
//  '<S267>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function3'
//  '<S268>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/RED_Rz_REF1'
//  '<S269>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Subsystem2'
//  '<S270>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S271>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S272>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S273>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S274>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S275>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S276>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S277>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S278>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S279>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Subsystem2/Subsystem'
//  '<S280>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)'
//  '<S281>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)'
//  '<S282>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)'
//  '<S283>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control'
//  '<S284>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /MATLAB Function2'
//  '<S285>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /MATLAB Function3'
//  '<S286>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /RED_Rz_REF1'
//  '<S287>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Subsystem2'
//  '<S288>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S289>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S290>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S291>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S292>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S293>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S294>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function2'
//  '<S295>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function3'
//  '<S296>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function4'
//  '<S297>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Subsystem2/Subsystem'
//  '<S298>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)'
//  '<S299>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)'
//  '<S300>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)'
//  '<S301>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control'
//  '<S302>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /MATLAB Function2'
//  '<S303>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /MATLAB Function3'
//  '<S304>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /RED_Rz_REF1'
//  '<S305>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Subsystem2'
//  '<S306>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S307>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S308>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S309>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S310>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S311>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S312>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function2'
//  '<S313>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function3'
//  '<S314>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function4'
//  '<S315>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Subsystem2/Subsystem'
//  '<S316>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S317>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S318>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S319>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control'
//  '<S320>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/MATLAB Function2'
//  '<S321>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/MATLAB Function3'
//  '<S322>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/RED_Rz_REF1'
//  '<S323>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Subsystem2'
//  '<S324>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S325>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S326>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S327>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S328>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S329>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S330>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S331>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S332>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S333>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Subsystem2/Subsystem'
//  '<S334>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1'
//  '<S335>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 '
//  '<S336>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 '
//  '<S337>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4'
//  '<S338>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Subsystem'
//  '<S339>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED'
//  '<S340>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM'
//  '<S341>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED'
//  '<S342>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM'
//  '<S343>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED'
//  '<S344>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM'
//  '<S345>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED'
//  '<S346>' : 'Parker_Velocity_Control/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM'
//  '<S347>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior'
//  '<S348>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior'
//  '<S349>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior'
//  '<S350>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Is this a  simulation?'
//  '<S351>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S352>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S353>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S354>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Hough Control'
//  '<S355>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S356>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S357>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S358>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S359>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S360>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S361>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S362>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S363>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S364>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S365>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S366>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S367>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Hough Control'
//  '<S368>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S369>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S370>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S371>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S372>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S373>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S374>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S375>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S376>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S377>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S378>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S379>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S380>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Hough Control'
//  '<S381>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED'
//  '<S382>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM'
//  '<S383>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S384>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S385>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S386>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S387>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S388>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S389>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S390>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S391>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S392>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller'
//  '<S393>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller'
//  '<S394>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/End-Effector Position'
//  '<S395>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Error Signal'
//  '<S396>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian '
//  '<S397>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function'
//  '<S398>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function '
//  '<S399>' : 'Parker_Velocity_Control/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function'
//  '<S400>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior'
//  '<S401>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior'
//  '<S402>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior'
//  '<S403>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Is this a  simulation?'
//  '<S404>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S405>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S406>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S407>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control'
//  '<S408>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S409>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S410>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S411>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S412>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S413>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S414>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S415>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S416>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S417>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S418>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S419>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S420>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control'
//  '<S421>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S422>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S423>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S424>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S425>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S426>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S427>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S428>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S429>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S430>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S431>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S432>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S433>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Hough Control'
//  '<S434>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED'
//  '<S435>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM'
//  '<S436>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S437>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S438>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S439>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S440>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S441>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S442>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S443>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S444>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S445>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller'
//  '<S446>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller'
//  '<S447>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/End-Effector Position'
//  '<S448>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Error Signal'
//  '<S449>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian '
//  '<S450>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function'
//  '<S451>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function '
//  '<S452>' : 'Parker_Velocity_Control/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function'
//  '<S453>' : 'Parker_Velocity_Control/Phase #6:  Stop Floating and Spin Down RW/Change BLACK Behavior'
//  '<S454>' : 'Parker_Velocity_Control/Phase #6:  Stop Floating and Spin Down RW/Change BLUE Behavior'
//  '<S455>' : 'Parker_Velocity_Control/Phase #6:  Stop Floating and Spin Down RW/Change RED Behavior'
//  '<S456>' : 'Parker_Velocity_Control/Phase #6:  Stop Floating and Spin Down RW/Is this a  simulation?'
//  '<S457>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics'
//  '<S458>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics'
//  '<S459>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/IMU Model'
//  '<S460>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics'
//  '<S461>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model'
//  '<S462>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x1'
//  '<S463>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x2'
//  '<S464>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x3'
//  '<S465>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x4'
//  '<S466>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x7'
//  '<S467>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model/MATLAB Function'
//  '<S468>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x1/Hold this value'
//  '<S469>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x2/Hold this value'
//  '<S470>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x3/Hold this value'
//  '<S471>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x4/Hold this value'
//  '<S472>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x7/Hold this value'
//  '<S473>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model'
//  '<S474>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x1'
//  '<S475>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x2'
//  '<S476>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x3'
//  '<S477>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x4'
//  '<S478>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x7'
//  '<S479>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model/MATLAB Function'
//  '<S480>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x1/Hold this value'
//  '<S481>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x2/Hold this value'
//  '<S482>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x3/Hold this value'
//  '<S483>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x4/Hold this value'
//  '<S484>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x7/Hold this value'
//  '<S485>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/IMU Model/Compare To Zero'
//  '<S486>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/IMU Model/Compare To Zero1'
//  '<S487>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/IMU Model/Compare To Zero2'
//  '<S488>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM'
//  '<S489>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only'
//  '<S490>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics'
//  '<S491>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Coriolis Function'
//  '<S492>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Dynamics Model'
//  '<S493>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Inertia Function '
//  '<S494>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x1'
//  '<S495>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x2'
//  '<S496>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x1/Hold this value'
//  '<S497>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x2/Hold this value'
//  '<S498>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics'
//  '<S499>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model'
//  '<S500>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x1'
//  '<S501>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x2'
//  '<S502>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x3'
//  '<S503>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x4'
//  '<S504>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x5'
//  '<S505>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model/MATLAB Function'
//  '<S506>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x1/Hold this value'
//  '<S507>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x2/Hold this value'
//  '<S508>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x3/Hold this value'
//  '<S509>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x4/Hold this value'
//  '<S510>' : 'Parker_Velocity_Control/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x5/Hold this value'

#endif                                 // RTW_HEADER_Parker_Velocity_Control_h_
