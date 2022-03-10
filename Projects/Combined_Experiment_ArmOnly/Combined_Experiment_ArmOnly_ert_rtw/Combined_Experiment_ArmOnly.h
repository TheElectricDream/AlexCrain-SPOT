//
//  Combined_Experiment_ArmOnly.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Combined_Experiment_ArmOnly".
//
//  Model version              : 1.194
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Thu Mar 10 13:54:46 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_Combined_Experiment_ArmOnly_h_
#define RTW_HEADER_Combined_Experiment_ArmOnly_h_
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "dynamixel_functions.h"
#include "dynamixel_sdk.h"
#include "MW_gpio.h"
#include "MW_pigs.h"
#include "owl.hpp"
#include "phasespace_headers.h"
#include "DAHostLib_Network.h"
#include "MW_I2C.h"
#include "Combined_Experiment_ArmOnly_types.h"

// Shared type includes
#include "multiword_types.h"
#include "rt_nonfinite.h"
#include "rt_zcfcn.h"
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

// Block signals for system '<S27>/MATLAB Function2'
typedef struct {
  real_T ThrustPer_Final[8];           // '<S27>/MATLAB Function2'
  real_T ThrustPer_Sat[8];
} B_MATLABFunction2_Combined_Ex_T;

// Block signals for system '<S32>/Create Rotation Matrix'
typedef struct {
  real_T C_Ib[4];                      // '<S32>/Create Rotation Matrix'
} B_CreateRotationMatrix_Combin_T;

// Block signals for system '<S28>/Create Rotation Matrix'
typedef struct {
  real_T C_bI[4];                      // '<S28>/Create Rotation Matrix'
} B_CreateRotationMatrix_Comb_i_T;

// Block signals for system '<S36>/MATLAB Function'
typedef struct {
  real_T Mat2[64];
  real_T H[24];                        // '<S36>/MATLAB Function'
  real_T b[24];
  real_T v[8];
} B_MATLABFunction_Combined_Exp_T;

// Block signals for system '<S169>/AHRS2'
typedef struct {
  real_T Qw[144];
  real_T Ppost[144];
  real_T H[72];
  real_T b_X[72];
  real_T H_m[72];
  real_T c_A[36];
  real_T xe_post[12];
  real_T Rprior[9];
  real_T h1[9];
  real_T x[9];
  real_T h[9];
  real_T ze[6];
  real_T b_varargout_1[4];
  real_T psquared[4];
  real_T AHRS2_o2[3];                  // '<S169>/AHRS2'
  real_T b_varargout_2[3];
  real_T gravityAccelGyroDiff[3];
  real_T offDiag[3];
  real_T Reast[3];
  real_T c[3];
  boolean_T b[9];
  real_T accelMeasNoiseVar;
  real_T magMeasNoiseVar;
  real_T smax;
  real_T s;
  real_T temp;
  real_T assign_temp_a;
  real_T assign_temp_b;
  real_T assign_temp_c;
  real_T assign_temp_d;
  real_T assign_temp_d_c;
  real_T gyroOffsetErr_idx_0;
  real_T linAccelErr_idx_0;
  real_T gyroOffsetErr_idx_1;
  real_T linAccelErr_idx_1;
  real_T gyroOffsetErr_idx_2;
  real_T linAccelErr_idx_2;
  real_T assign_temp_a_k;
  real_T assign_temp_b_c;
  real_T assign_temp_c_b;
  real_T assign_temp_d_p;
  real_T theta;
  real_T tr;
  real_T d;
  real_T ac2;
  real_T ad2;
  real_T bc2;
  real_T bd2;
  real_T cd2;
  real_T aasq;
  real_T scale;
  real_T absxk;
  int8_T b_ipiv[6];
  int32_T i;
  int32_T i1;
} B_AHRS2_Combined_Experiment_A_T;

// Block states (default storage) for system '<S169>/AHRS2'
typedef struct {
  fusion_simulink_ahrsfilter_Co_T obj; // '<S169>/AHRS2'
  c_fusion_internal_frames_NED__T gobj_1;// '<S169>/AHRS2'
  c_fusion_internal_frames_NED__T gobj_2;// '<S169>/AHRS2'
  boolean_T objisempty;                // '<S169>/AHRS2'
} DW_AHRS2_Combined_Experiment__T;

// Block signals for system '<S169>/ChangeOrientation'
typedef struct {
  real_T y[3];                         // '<S169>/ChangeOrientation'
} B_ChangeOrientation_Combined__T;

// Block signals for system '<S179>/Calculate Running Mean'
typedef struct {
  real_T Mean;                         // '<S182>/Mean'
} B_CalculateRunningMean_Combin_T;

// Block states (default storage) for system '<S179>/Calculate Running Mean'
typedef struct {
  real_T Delay_DSTATE;                 // '<S182>/Delay'
  real_T Delay1_DSTATE[2];             // '<S182>/Delay1'
  real_T Delay2_DSTATE[4];             // '<S182>/Delay2'
  real_T Delay3_DSTATE[5];             // '<S182>/Delay3'
  real_T Delay4_DSTATE[3];             // '<S182>/Delay4'
  real_T Mean_AccVal;                  // '<S182>/Mean'
} DW_CalculateRunningMean_Combi_T;

// Block signals for system '<S194>/Calculate Running Mean'
typedef struct {
  real_T TmpSignalConversionAtMean_b[6];
  real_T Subtract;                     // '<S201>/Subtract'
} B_CalculateRunningMean_Comb_e_T;

// Block states (default storage) for system '<S194>/Calculate Running Mean'
typedef struct {
  real_T Delay_DSTATE;                 // '<S201>/Delay'
  real_T Delay1_DSTATE[2];             // '<S201>/Delay1'
  real_T Delay2_DSTATE[4];             // '<S201>/Delay2'
  real_T Delay3_DSTATE[5];             // '<S201>/Delay3'
  real_T Delay4_DSTATE[3];             // '<S201>/Delay4'
  real_T Mean_AccVal;                  // '<S201>/Mean'
} DW_CalculateRunningMean_Com_e_T;

// Block signals for system '<S506>/MATLAB Function'
typedef struct {
  real_T y;                            // '<S506>/MATLAB Function'
} B_MATLABFunction_Combined_E_l_T;

// Block states (default storage) for system '<Root>/Phase #0:  Wait for Synchronization' 
typedef struct {
  int8_T ThisIFblockdetermineswhetherorn;
    // '<S8>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhethero_m;
     // '<S8>/This IF block determines whether or not to run the BLUE sim//exp'
  int8_T ThisIFblockdetermineswhethero_n;
     // '<S8>/This IF block determines whether or not to run the RED sim//exp '
} DW_Phase0WaitforSynchronizati_T;

// Block signals for system '<S281>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S281>/MATLAB Function2'
  real_T Ox[2];                        // '<S281>/MATLAB Function2'
} B_MATLABFunction2_Combined__n_T;

// Block signals for system '<S281>/MATLAB Function3'
typedef struct {
  real_T Ox[2];                        // '<S281>/MATLAB Function3'
} B_MATLABFunction3_Combined_Ex_T;

// Block signals for system '<S281>/MATLAB Function4'
typedef struct {
  real_T e_out;                        // '<S281>/MATLAB Function4'
} B_MATLABFunction4_Combined_Ex_T;

// Block states (default storage) for system '<S276>/Send Position Commands to Manipulator' 
typedef struct {
  MoveArm_Position_Combined_Exp_T obj;
                              // '<S276>/Send Position Commands to Manipulator'
  boolean_T objisempty;       // '<S276>/Send Position Commands to Manipulator'
} DW_SendPositionCommandstoMani_T;

// Block signals for system '<S343>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S343>/MATLAB Function2'
  real_T Ox[2];                        // '<S343>/MATLAB Function2'
} B_MATLABFunction2_Combined__k_T;

// Block signals for system '<S318>/Sub-Phase #1'
typedef struct {
  real_T Subtract;                     // '<S356>/Subtract'
  real_T Delay1;                       // '<S367>/Delay1'
  real_T Subtract1;                    // '<S356>/Subtract1'
  real_T Delay1_i;                     // '<S369>/Delay1'
  real_T Delay1_j;                     // '<S365>/Delay1'
  real_T In1;                          // '<S370>/In1'
  real_T In1_p;                        // '<S368>/In1'
  real_T In1_j;                        // '<S366>/In1'
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4;// '<S364>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3;// '<S364>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2;// '<S364>/MATLAB Function2' 
} B_SubPhase1_Combined_Experime_T;

// Block states (default storage) for system '<S318>/Sub-Phase #1'
typedef struct {
  real_T Delay1_DSTATE;                // '<S367>/Delay1'
  real_T Delay1_DSTATE_g;              // '<S369>/Delay1'
  real_T Delay1_DSTATE_f;              // '<S365>/Delay1'
  int8_T ifwewentthroughastep_ActiveSubs;// '<S367>/if we went through a "step"' 
  int8_T ifwewentthroughastep_ActiveSu_j;// '<S369>/if we went through a "step"' 
  int8_T ifwewentthroughastep_ActiveSu_h;// '<S365>/if we went through a "step"' 
  uint8_T icLoad;                      // '<S367>/Delay1'
  uint8_T icLoad_p;                    // '<S369>/Delay1'
  uint8_T icLoad_d;                    // '<S365>/Delay1'
} DW_SubPhase1_Combined_Experim_T;

// Block signals for system '<S12>/Change BLUE Behavior'
typedef struct {
  real_T Subtract;                     // '<S411>/Subtract'
  real_T Delay1;                       // '<S433>/Delay1'
  real_T Subtract1;                    // '<S411>/Subtract1'
  real_T Delay1_m;                     // '<S435>/Delay1'
  real_T Delay1_p;                     // '<S431>/Delay1'
  real_T In1;                          // '<S436>/In1'
  real_T In1_k;                        // '<S434>/In1'
  real_T In1_a;                        // '<S432>/In1'
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4;// '<S430>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3;// '<S430>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2;// '<S430>/MATLAB Function2' 
} B_ChangeBLUEBehavior_Combin_k_T;

// Block states (default storage) for system '<S12>/Change BLUE Behavior'
typedef struct {
  real_T Delay1_DSTATE;                // '<S433>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S435>/Delay1'
  real_T Delay1_DSTATE_k;              // '<S431>/Delay1'
  int8_T ifwewentthroughastep_ActiveSubs;// '<S433>/if we went through a "step"' 
  int8_T ifwewentthroughastep_ActiveSu_j;// '<S435>/if we went through a "step"' 
  int8_T ifwewentthroughastep_ActiveSu_a;// '<S431>/if we went through a "step"' 
  uint8_T icLoad;                      // '<S433>/Delay1'
  uint8_T icLoad_h;                    // '<S435>/Delay1'
  uint8_T icLoad_l;                    // '<S431>/Delay1'
} DW_ChangeBLUEBehavior_Combi_i_T;

// Block signals (default storage)
typedef struct {
  real_T Mat2[64];
  real_T Mat2_m[64];
  real_T TmpSignalConversionAtToWork[64];
  real_T transpose[24];                // '<S51>/transpose'
  real_T transpose_p[24];              // '<S42>/transpose'
  real_T transpose_d[24];              // '<S33>/transpose'
  real_T H_bu[24];                     // '<S27>/MATLAB Function'
  real_T b[24];
  real_T StreamPhaseSpacetoPlatform[13];
  real_T b_c[9];
  real_T rtb_H_bu_k[9];
  real_T x[9];
  real_T rtb_RemoveNegatives_i_c[8];
  char_T cv[45];
  real_T x_ddot_m[3];                  // '<S501>/MATLAB Function'
  real_T x_ddot_a[3];                  // '<S500>/MATLAB Function'
  real_T VelocitytoPosition[3];
  real_T LSM9DS1IMUSensor_o3[3];       // '<S6>/LSM9DS1 IMU Sensor'
  real_T Gain[3];                      // '<S171>/Gain'
  real_T DigitalFilter[3];             // '<S237>/Digital Filter'
  real_T Gain_o[3];                    // '<S170>/Gain'
  real_T DigitalFilter_n[3];           // '<S206>/Digital Filter'
  real_T Gain_k[3];                    // '<S169>/Gain'
  real_T DigitalFilter_k[3];           // '<S175>/Digital Filter'
  real_T rtb_Subtract2_f_b[2];
  boolean_T Compare_p[13];
  MW_I2C_Mode_Type ModeType;
  real_T SetUniversalTimeIfthisisasimula;
                        // '<S15>/Set Universal Time (If this is a simulation)'
  real_T Subtract;                     // '<S455>/Subtract'
  real_T Subtract1;                    // '<S455>/Subtract1'
  real_T In1;                          // '<S492>/In1'
  real_T In1_j;                        // '<S490>/In1'
  real_T In1_d;                        // '<S488>/In1'
  real_T Subtract_o;                   // '<S453>/Subtract'
  real_T Subtract1_c;                  // '<S453>/Subtract1'
  real_T In1_c;                        // '<S466>/In1'
  real_T In1_i;                        // '<S464>/In1'
  real_T In1_g;                        // '<S462>/In1'
  real_T Subtract_l;                   // '<S412>/Subtract'
  real_T Subtract1_g;                  // '<S412>/Subtract1'
  real_T In1_f;                        // '<S449>/In1'
  real_T In1_b;                        // '<S447>/In1'
  real_T In1_h;                        // '<S445>/In1'
  real_T Subtract_d;                   // '<S410>/Subtract'
  real_T Subtract1_d;                  // '<S410>/Subtract1'
  real_T In1_d0;                       // '<S423>/In1'
  real_T In1_fx;                       // '<S421>/In1'
  real_T In1_fk;                       // '<S419>/In1'
  real_T Subtract_b;                   // '<S390>/Subtract'
  real_T Subtract1_m;                  // '<S390>/Subtract1'
  real_T In1_g4;                       // '<S405>/In1'
  real_T In1_go;                       // '<S403>/In1'
  real_T In1_fa;                       // '<S401>/In1'
  real_T Subtract_a;                   // '<S324>/Subtract'
  real_T Subtract1_h;                  // '<S324>/Subtract1'
  real_T In1_n;                        // '<S352>/In1'
  real_T In1_l;                        // '<S350>/In1'
  real_T In1_ck;                       // '<S348>/In1'
  real_T Subtract_oi;                  // '<S321>/Subtract'
  real_T Subtract1_d2;                 // '<S321>/Subtract1'
  real_T In1_ly;                       // '<S335>/In1'
  real_T In1_cw;                       // '<S333>/In1'
  real_T In1_a;                        // '<S331>/In1'
  real_T Subtract_f;                   // '<S276>/Subtract'
  real_T Subtract1_dr;                 // '<S276>/Subtract1'
  real_T In1_if;                       // '<S313>/In1'
  real_T In1_cr;                       // '<S311>/In1'
  real_T In1_k;                        // '<S309>/In1'
  real_T Subtract_j;                   // '<S275>/Subtract'
  real_T Subtract1_o;                  // '<S275>/Subtract1'
  real_T In1_jw;                       // '<S300>/In1'
  real_T In1_lc;                       // '<S298>/In1'
  real_T In1_o;                        // '<S296>/In1'
  real_T Subtract_ou;                  // '<S274>/Subtract'
  real_T Subtract1_m0;                 // '<S274>/Subtract1'
  real_T In1_gc;                       // '<S287>/In1'
  real_T In1_e;                        // '<S285>/In1'
  real_T In1_fau;                      // '<S283>/In1'
  real_T In1_n0;                       // '<S547>/In1'
  real_T In1_hg;                       // '<S546>/In1'
  real_T In1_b0;                       // '<S545>/In1'
  real_T In1_m;                        // '<S544>/In1'
  real_T In1_fu;                       // '<S543>/In1'
  real_T In1_gi;                       // '<S542>/In1'
  real_T In1_ju;                       // '<S541>/In1'
  real_T In1_dl;                       // '<S540>/In1'
  real_T In1_fw;                       // '<S537>/In1'
  real_T In1_k0;                       // '<S536>/In1'
  real_T In1_ed;                       // '<S535>/In1'
  real_T In1_kd;                       // '<S534>/In1'
  real_T In1_p;                        // '<S533>/In1'
  real_T In[3];                        // '<S532>/In'
  real_T In_k[3];                      // '<S530>/In'
  real_T In_l[3];                      // '<S528>/In'
  real_T In1_gv;                       // '<S264>/In1'
  real_T In1_e5;                       // '<S261>/In1'
  real_T In1_gy;                       // '<S258>/In1'
  real_T In1_f1;                       // '<S252>/In1'
  real_T In1_cx;                       // '<S248>/In1'
  real_T In1_lm;                       // '<S245>/In1'
  real_T In1_ai;                       // '<S233>/In1'
  real_T In1_b1;                       // '<S230>/In1'
  real_T In1_le;                       // '<S227>/In1'
  real_T In1_hy;                       // '<S221>/In1'
  real_T In1_cd;                       // '<S202>/In1'
  real_T In1_gs;                       // '<S199>/In1'
  real_T In1_ic;                       // '<S196>/In1'
  real_T In1_jp;                       // '<S190>/In1'
  real_T In1_ny;                       // '<S186>/In1'
  real_T In1_hd;                       // '<S183>/In1'
  real_T dividebydeltaT;               // '<S115>/divide by delta T'
  real_T dividebydeltaT_l;             // '<S116>/divide by delta T'
  real_T Unwrap1;                      // '<S80>/Unwrap1'
  real_T dividebydeltaT_h;             // '<S117>/divide by delta T'
  real_T dividebydeltaT_a;             // '<S109>/divide by delta T'
  real_T dividebydeltaT_l4;            // '<S113>/divide by delta T'
  real_T Unwrap;                       // '<S80>/Unwrap'
  real_T dividebydeltaT_f;             // '<S114>/divide by delta T'
  real_T TmpSignalConversionAtSendBLACKS[13];
  real_T In1_pb;                       // '<S136>/In1'
  real_T In1_mj;                       // '<S135>/In1'
  real_T In1_lj;                       // '<S134>/In1'
  real_T In1_hs;                       // '<S133>/In1'
  real_T In1_bi;                       // '<S132>/In1'
  real_T In1_n5;                       // '<S131>/In1'
  real_T In1_mc;                       // '<S130>/In1'
  real_T In1_i2;                       // '<S129>/In1'
  real_T In1_kg;                       // '<S128>/In1'
  real_T In1_ox;                       // '<S127>/In1'
  real_T In1_av;                       // '<S126>/In1'
  real_T In1_gd;                       // '<S125>/In1'
  real_T In1_nh;                       // '<S108>/In1'
  real_T In1_n1[13];                   // '<S107>/In1'
  real_T In1_it;                       // '<S122>/In1'
  real_T UDPReceive_o1[13];            // '<S79>/UDP Receive'
  real_T dividebydeltaT_m;             // '<S87>/divide by delta T'
  real_T dividebydeltaT_me;            // '<S88>/divide by delta T'
  real_T Unwrap1_l;                    // '<S79>/Unwrap1'
  real_T dividebydeltaT_g;             // '<S89>/divide by delta T'
  real_T dividebydeltaT_hw;            // '<S81>/divide by delta T'
  real_T dividebydeltaT_d;             // '<S85>/divide by delta T'
  real_T Unwrap_l;                     // '<S79>/Unwrap'
  real_T dividebydeltaT_n;             // '<S86>/divide by delta T'
  real_T In1_oi;                       // '<S104>/In1'
  real_T In1_dr;                       // '<S103>/In1'
  real_T In1_df;                       // '<S102>/In1'
  real_T In1_fr;                       // '<S101>/In1'
  real_T In1_jk;                       // '<S100>/In1'
  real_T In1_dn;                       // '<S99>/In1'
  real_T In1_cxn;                      // '<S98>/In1'
  real_T In1_jp3;                      // '<S97>/In1'
  real_T In1_gw;                       // '<S96>/In1'
  real_T In1_jj;                       // '<S95>/In1'
  real_T In1_fg;                       // '<S94>/In1'
  real_T In1_kw;                       // '<S93>/In1'
  real_T dividebydeltaT_p;             // '<S147>/divide by delta T'
  real_T dividebydeltaT_nx;            // '<S148>/divide by delta T'
  real_T Unwrap1_c;                    // '<S77>/Unwrap1'
  real_T dividebydeltaT_dx;            // '<S149>/divide by delta T'
  real_T dividebydeltaT_ph;            // '<S141>/divide by delta T'
  real_T dividebydeltaT_pf;            // '<S145>/divide by delta T'
  real_T Unwrap_n;                     // '<S77>/Unwrap'
  real_T dividebydeltaT_hu;            // '<S146>/divide by delta T'
  real_T In1_kt;                       // '<S168>/In1'
  real_T In1_on;                       // '<S167>/In1'
  real_T In1_aw;                       // '<S166>/In1'
  real_T In1_awb;                      // '<S165>/In1'
  real_T In1_ljk;                      // '<S164>/In1'
  real_T In1_d4;                       // '<S163>/In1'
  real_T In1_cp;                       // '<S162>/In1'
  real_T In1_mm;                       // '<S161>/In1'
  real_T In1_lyk;                      // '<S160>/In1'
  real_T In1_dy;                       // '<S159>/In1'
  real_T In1_ds;                       // '<S158>/In1'
  real_T In1_kd2;                      // '<S157>/In1'
  real_T In1_pl;                       // '<S140>/In1'
  real_T In1_b4[13];                   // '<S139>/In1'
  real_T In1_j1;                       // '<S154>/In1'
  real_T y5;
  real_T y10;
  real_T Sum6_dk;                      // '<S402>/Sum6'
  real_T Sum6_l1;                      // '<S463>/Sum6'
  real_T RED_Tz_Wrist;
  real_T rtb_TmpSignalConversionAtSFun_p;
  real_T rtb_TmpSignalConversionAtSFun_c;
  real_T rtb_TmpSignalConversionAtSFun_f;
  real_T rtb_x_ddot_idx_0;
  real_T rtb_x_ddot_idx_1;
  real_T VelocitytoPosition_d_idx_0;
  real_T VelocitytoPosition_d_idx_1;
  real_T VelocitytoPosition_d_idx_2;
  real_T FromWorkspace2;               // '<S389>/From Workspace2'
  real_T FromWorkspace1;               // '<S389>/From Workspace1'
  real_T FromWorkspace;                // '<S389>/From Workspace'
  real_T absx11;
  real_T absx21;
  real_T absx31;
  real_T sampleTime;
  real_T d;
  real_T sr;
  real_T si;
  int16_T b_RegisterValue[3];
  uint8_T output_raw[6];
  int32_T ntIdx0;
  int32_T uElOffset0;
  int32_T ntIdx1;
  int32_T uElOffset1;
  int32_T yElIdx;
  int32_T p1;
  int32_T p2;
  int32_T p3;
  int32_T itmp;
  int32_T u0;
  int32_T u1;
  uint32_T lo;
  uint32_T hi;
  ZCEventType zcEvent;
  int8_T rtAction;
  int8_T rtPrevAction;
  uint8_T SwappedDataBytes;
  uint8_T status;
  boolean_T DataTypeConversion;        // '<S121>/Data Type Conversion'
  boolean_T DataTypeConversion_h;      // '<S153>/Data Type Conversion'
  boolean_T rtb_Compare_g_g;
  boolean_T rtb_Compare_m_g;
  boolean_T rtb_Compare_me;
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_df;// '<S486>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_bc;// '<S486>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_mx;// '<S486>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Combin_k_T ChangeBLUEBehavior_b;// '<S13>/Change BLUE Behavior' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_de;// '<S460>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_o;// '<S460>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_gh;// '<S460>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_na;// '<S443>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_c1;// '<S443>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_ia;// '<S443>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Combin_k_T ChangeBLUEBehavior_gj;// '<S12>/Change BLUE Behavior' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_l;// '<S417>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_b;// '<S417>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_p;// '<S417>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_n;// '<S395>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_lg;// '<S395>/MATLAB Function3' 
  B_MATLABFunction2_Combined__k_T sf_MATLABFunction2_g;// '<S395>/MATLAB Function2' 
  B_SubPhase1_Combined_Experime_T SubPhase4_f;// '<S318>/Sub-Phase #4'
  B_SubPhase1_Combined_Experime_T SubPhase1_a;// '<S318>/Sub-Phase #1'
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_dk;// '<S343>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_ly;// '<S343>/MATLAB Function3' 
  B_MATLABFunction2_Combined__k_T sf_MATLABFunction2_b;// '<S343>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_d;// '<S329>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_l;// '<S329>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_i;// '<S329>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_b;// '<S307>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_k;// '<S307>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_ol;// '<S307>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4_c;// '<S294>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3_c;// '<S294>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_m;// '<S294>/MATLAB Function2' 
  B_MATLABFunction4_Combined_Ex_T sf_MATLABFunction4;// '<S281>/MATLAB Function4' 
  B_MATLABFunction3_Combined_Ex_T sf_MATLABFunction3;// '<S281>/MATLAB Function3' 
  B_MATLABFunction2_Combined__n_T sf_MATLABFunction2_o;// '<S281>/MATLAB Function2' 
  B_MATLABFunction_Combined_E_l_T sf_MATLABFunction_d;// '<S508>/MATLAB Function' 
  B_MATLABFunction_Combined_E_l_T sf_MATLABFunction_g;// '<S507>/MATLAB Function' 
  B_MATLABFunction_Combined_E_l_T sf_MATLABFunction_fm;// '<S506>/MATLAB Function' 
  B_CalculateRunningMean_Comb_e_T CalculateRunningMean_cf;// '<S256>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_jr;// '<S255>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_j1;// '<S254>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_et;// '<S243>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_om;// '<S242>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_oo;// '<S241>/Calculate Running Mean' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix_k;// '<S171>/Create Rotation Matrix' 
  B_ChangeOrientation_Combined__T sf_ChangeOrientation_b;// '<S171>/ChangeOrientation' 
  B_AHRS2_Combined_Experiment_A_T AHRS2_pn;// '<S169>/AHRS2'
  B_CalculateRunningMean_Comb_e_T CalculateRunningMean_o;// '<S225>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_b;// '<S224>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_j;// '<S223>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_l;// '<S212>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_f;// '<S211>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_c;// '<S210>/Calculate Running Mean' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix_h1;// '<S170>/Create Rotation Matrix' 
  B_ChangeOrientation_Combined__T sf_ChangeOrientation_p;// '<S170>/ChangeOrientation' 
  B_AHRS2_Combined_Experiment_A_T AHRS2_p;// '<S169>/AHRS2'
  B_CalculateRunningMean_Comb_e_T CalculateRunningMean_ab;// '<S194>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_a;// '<S193>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_d;// '<S192>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_h;// '<S181>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean_e;// '<S180>/Calculate Running Mean' 
  B_CalculateRunningMean_Combin_T CalculateRunningMean;// '<S179>/Calculate Running Mean' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix_fq;// '<S169>/Create Rotation Matrix' 
  B_ChangeOrientation_Combined__T sf_ChangeOrientation;// '<S169>/ChangeOrientation' 
  B_AHRS2_Combined_Experiment_A_T AHRS2;// '<S169>/AHRS2'
  B_CreateRotationMatrix_Comb_i_T sf_CreateRotationMatrix_n;// '<S46>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix_l;// '<S50>/Create Rotation Matrix' 
  B_MATLABFunction2_Combined_Ex_T sf_MATLABFunction2_l;// '<S45>/MATLAB Function2' 
  B_CreateRotationMatrix_Comb_i_T sf_CreateRotationMatrix_f;// '<S37>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix_i;// '<S41>/Create Rotation Matrix' 
  B_MATLABFunction2_Combined_Ex_T sf_MATLABFunction2_n;// '<S36>/MATLAB Function2' 
  B_MATLABFunction_Combined_Exp_T sf_MATLABFunction1_b;// '<S36>/MATLAB Function1' 
  B_MATLABFunction_Combined_Exp_T sf_MATLABFunction_i;// '<S36>/MATLAB Function' 
  B_CreateRotationMatrix_Comb_i_T sf_CreateRotationMatrix_h;// '<S28>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Combin_T sf_CreateRotationMatrix;// '<S32>/Create Rotation Matrix' 
  B_MATLABFunction2_Combined_Ex_T sf_MATLABFunction2;// '<S27>/MATLAB Function2' 
} B_Combined_Experiment_ArmOnly_T;

// Block states (default storage) for system '<Root>'
typedef struct {
  raspi_internal_lsm9ds1Block_C_T obj; // '<S6>/LSM9DS1 IMU Sensor'
  InitializeForPosition_Combine_T obj_i;
                          // '<Root>/Initialization Block for Position Control'
  PhaseSpace_ALL_Combined_Exper_T obj_f;// '<S77>/Stream PhaseSpace to Platform' 
  codertarget_linux_blocks_Di_b_T obj_fq;// '<S70>/Digital Read'
  raspi_internal_PWMBlock_Combi_T obj_d;// '<S68>/PWM1'
  raspi_internal_PWMBlock_Combi_T obj_m;// '<S26>/RED T1 - BLACK T8'
  raspi_internal_PWMBlock_Combi_T obj_ma;// '<S26>/RED T2 - BLACK T3'
  raspi_internal_PWMBlock_Combi_T obj_h;// '<S26>/RED T3'
  raspi_internal_PWMBlock_Combi_T obj_k;// '<S26>/RED T4 - BLACK T5'
  raspi_internal_PWMBlock_Combi_T obj_hw;// '<S26>/RED T5 - BLACK T4'
  raspi_internal_PWMBlock_Combi_T obj_hf;// '<S26>/RED T6 - BLACK T7'
  raspi_internal_PWMBlock_Combi_T obj_c;// '<S26>/RED T7 - BLACK T6'
  raspi_internal_PWMBlock_Combi_T obj_fo;// '<S26>/RED T8 - BLACK T1'
  raspi_internal_PWMBlock_Combi_T obj_ci;// '<S26>/BLACK T2'
  codertarget_linux_blocks_Digi_T obj_cp;// '<S72>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_b;// '<S71>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_a;// '<S21>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_iv;// '<S20>/Digital Write'
  real_T Delay1_DSTATE;                // '<S489>/Delay1'
  real_T Delay1_DSTATE_m;              // '<S491>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S487>/Delay1'
  real_T Delay1_DSTATE_o;              // '<S463>/Delay1'
  real_T Delay1_DSTATE_p;              // '<S465>/Delay1'
  real_T Delay1_DSTATE_b;              // '<S461>/Delay1'
  real_T Delay1_DSTATE_i;              // '<S444>/Delay1'
  real_T Delay1_DSTATE_h;              // '<S446>/Delay1'
  real_T Delay1_DSTATE_l;              // '<S448>/Delay1'
  real_T Delay1_DSTATE_lt;             // '<S420>/Delay1'
  real_T Delay1_DSTATE_im;             // '<S422>/Delay1'
  real_T Delay1_DSTATE_f;              // '<S418>/Delay1'
  real_T Delay1_DSTATE_c;              // '<S400>/Delay1'
  real_T Delay1_DSTATE_mu;             // '<S402>/Delay1'
  real_T Delay1_DSTATE_il;             // '<S404>/Delay1'
  real_T Delay1_DSTATE_hp;             // '<S349>/Delay1'
  real_T Delay1_DSTATE_ha;             // '<S351>/Delay1'
  real_T Delay1_DSTATE_li;             // '<S347>/Delay1'
  real_T Delay1_DSTATE_g;              // '<S332>/Delay1'
  real_T Delay1_DSTATE_pe;             // '<S334>/Delay1'
  real_T Delay1_DSTATE_lu;             // '<S330>/Delay1'
  real_T Delay1_DSTATE_ct;             // '<S308>/Delay1'
  real_T Delay1_DSTATE_bm;             // '<S310>/Delay1'
  real_T Delay1_DSTATE_pr;             // '<S312>/Delay1'
  real_T Delay1_DSTATE_fb;             // '<S297>/Delay1'
  real_T Delay1_DSTATE_n1;             // '<S299>/Delay1'
  real_T Delay1_DSTATE_cq;             // '<S295>/Delay1'
  real_T Delay1_DSTATE_k;              // '<S284>/Delay1'
  real_T Delay1_DSTATE_d;              // '<S286>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S282>/Delay1'
  real_T AccelerationtoVelocity_DSTATE[3];// '<S500>/Acceleration  to Velocity'
  real_T VelocitytoPosition_DSTATE[3]; // '<S500>/Velocity to Position'
  real_T Delay1_DSTATE_ff;             // '<S518>/Delay1'
  real_T Delay1_DSTATE_p5;             // '<S512>/Delay1'
  real_T Delay1_DSTATE_e;              // '<S519>/Delay1'
  real_T Delay1_DSTATE_cb;             // '<S513>/Delay1'
  real_T Delay1_DSTATE_ew;             // '<S520>/Delay1'
  real_T AccelerationtoVelocity_DSTATE_k[3];// '<S501>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_p[3];// '<S501>/Velocity to Position'
  real_T Delay1_DSTATE_kn;             // '<S521>/Delay1'
  real_T Delay1_DSTATE_gc;             // '<S514>/Delay1'
  real_T Delay1_DSTATE_gu;             // '<S522>/Delay1'
  real_T Delay1_DSTATE_a;              // '<S515>/Delay1'
  real_T Delay1_DSTATE_dd;             // '<S523>/Delay1'
  real_T DiscreteTimeIntegrator_DSTATE;// '<S16>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator1_DSTATE;// '<S16>/Discrete-Time Integrator1'
  real_T DiscreteTimeIntegrator2_DSTATE;// '<S16>/Discrete-Time Integrator2'
  real_T AccelerationtoVelocity_DSTATE_j[3];// '<S505>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_h[3];// '<S505>/Velocity to Position'
  real_T Delay1_DSTATE_ol;             // '<S509>/Delay1'
  real_T Delay1_DSTATE_pm;             // '<S510>/Delay1'
  real_T Delay1_DSTATE_a5;             // '<S516>/Delay1'
  real_T Delay1_DSTATE_c3;             // '<S511>/Delay1'
  real_T Delay1_DSTATE_mj;             // '<S517>/Delay1'
  real_T DigitalFilter_FILT_STATES[6]; // '<S237>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_h;// '<S243>/Discrete-Time Integrator1'
  real_T DigitalFilter_FILT_STATES_n[6];// '<S206>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_k;// '<S212>/Discrete-Time Integrator1'
  real_T DigitalFilter_FILT_STATES_e[6];// '<S175>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_f;// '<S181>/Discrete-Time Integrator1'
  real_T Delay1_DSTATE_p3;             // '<S115>/Delay1'
  real_T Delay1_DSTATE_bk;             // '<S110>/Delay1'
  real_T Delay1_DSTATE_ko;             // '<S116>/Delay1'
  real_T Delay1_DSTATE_kx;             // '<S111>/Delay1'
  real_T Delay1_DSTATE_at;             // '<S117>/Delay1'
  real_T Delay1_DSTATE_hg;             // '<S112>/Delay1'
  real_T Delay1_DSTATE_knv;            // '<S109>/Delay1'
  real_T Delay1_DSTATE_oz;             // '<S118>/Delay1'
  real_T Delay1_DSTATE_cf;             // '<S113>/Delay1'
  real_T Delay1_DSTATE_me;             // '<S119>/Delay1'
  real_T Delay1_DSTATE_gy;             // '<S114>/Delay1'
  real_T Delay1_DSTATE_hf;             // '<S120>/Delay1'
  real_T Delay1_DSTATE_nw;             // '<S87>/Delay1'
  real_T Delay1_DSTATE_gx;             // '<S82>/Delay1'
  real_T Delay1_DSTATE_ct1;            // '<S88>/Delay1'
  real_T Delay1_DSTATE_i0;             // '<S83>/Delay1'
  real_T Delay1_DSTATE_n0;             // '<S89>/Delay1'
  real_T Delay1_DSTATE_da;             // '<S84>/Delay1'
  real_T Delay1_DSTATE_m1;             // '<S81>/Delay1'
  real_T Delay1_DSTATE_bo;             // '<S90>/Delay1'
  real_T Delay1_DSTATE_ed;             // '<S85>/Delay1'
  real_T Delay1_DSTATE_k5;             // '<S91>/Delay1'
  real_T Delay1_DSTATE_p4;             // '<S86>/Delay1'
  real_T Delay1_DSTATE_fr;             // '<S92>/Delay1'
  real_T Delay1_DSTATE_ln;             // '<S147>/Delay1'
  real_T Delay1_DSTATE_pv;             // '<S142>/Delay1'
  real_T Delay1_DSTATE_mk;             // '<S148>/Delay1'
  real_T Delay1_DSTATE_le;             // '<S143>/Delay1'
  real_T Delay1_DSTATE_de;             // '<S149>/Delay1'
  real_T Delay1_DSTATE_lu2;            // '<S144>/Delay1'
  real_T Delay1_DSTATE_b3;             // '<S141>/Delay1'
  real_T Delay1_DSTATE_ng;             // '<S150>/Delay1'
  real_T Delay1_DSTATE_m3;             // '<S145>/Delay1'
  real_T Delay1_DSTATE_fs;             // '<S151>/Delay1'
  real_T Delay1_DSTATE_c4;             // '<S146>/Delay1'
  real_T Delay1_DSTATE_cz;             // '<S152>/Delay1'
  real_T Delay_DSTATE;                 // '<S58>/Delay'
  real_T RateTransition_Buffer;        // '<S3>/Rate Transition'
  real_T RateTransition1_Buffer;       // '<S3>/Rate Transition1'
  real_T RateTransition2_Buffer;       // '<S3>/Rate Transition2'
  real_T RateTransition3_Buffer;       // '<S3>/Rate Transition3'
  real_T RateTransition4_Buffer;       // '<S3>/Rate Transition4'
  real_T RateTransition5_Buffer;       // '<S3>/Rate Transition5'
  real_T RateTransition6_Buffer;       // '<S3>/Rate Transition6'
  real_T RateTransition7_Buffer;       // '<S3>/Rate Transition7'
  real_T RateTransition8_Buffer;       // '<S3>/Rate Transition8'
  real_T UDPSend_NetworkLib[137];      // '<S15>/UDP Send'
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
  real_T RED_Vy;                       // '<Root>/Universal_Time2'
  real_T RED_RzD;                      // '<Root>/Universal_Time3'
  real_T BLACK_Vx;                     // '<Root>/Universal_Time4'
  real_T BLACK_Vy;                     // '<Root>/Universal_Time5'
  real_T BLACK_RzD;                    // '<Root>/Universal_Time6'
  real_T BLUE_Vx;                      // '<Root>/Universal_Time7'
  real_T BLUE_Vy;                      // '<Root>/Universal_Time8'
  real_T BLUE_RzD;                     // '<Root>/Universal_Time9'
  real_T NextOutput;                   // '<S500>/Random Number'
  real_T NextOutput_g;                 // '<S16>/Random Number1'
  real_T NextOutput_h;                 // '<S16>/Random Number'
  real_T NextOutput_e;                 // '<S16>/Random Number2'
  real_T NextOutput_m;                 // '<S501>/Random Number'
  real_T NextOutput_f;                 // '<S16>/Random Number5'
  real_T NextOutput_l;                 // '<S505>/Random Number'
  real_T NextOutput_n;                 // '<S16>/Random Number7'
  real_T NextOutput_hl;                // '<S16>/Random Number6'
  real_T NextOutput_fw;                // '<S16>/Random Number8'
  real_T Unwrap1_Prev;                 // '<S80>/Unwrap1'
  real_T Unwrap1_Cumsum;               // '<S80>/Unwrap1'
  real_T Unwrap_Prev;                  // '<S80>/Unwrap'
  real_T Unwrap_Cumsum;                // '<S80>/Unwrap'
  real_T SendBLACKStatestoBLACKPlatform_[137];
                                // '<S80>/Send BLACK States to  BLACK Platform'
  real_T UDPReceive_NetworkLib[137];   // '<S79>/UDP Receive'
  real_T Unwrap1_Prev_p;               // '<S79>/Unwrap1'
  real_T Unwrap1_Cumsum_n;             // '<S79>/Unwrap1'
  real_T Unwrap_Prev_f;                // '<S79>/Unwrap'
  real_T Unwrap_Cumsum_k;              // '<S79>/Unwrap'
  real_T Unwrap1_Prev_l;               // '<S77>/Unwrap1'
  real_T Unwrap1_Cumsum_a;             // '<S77>/Unwrap1'
  real_T Unwrap_Prev_p;                // '<S77>/Unwrap'
  real_T Unwrap_Cumsum_o;              // '<S77>/Unwrap'
  real_T Product1_DWORK4[9];           // '<S51>/Product1'
  real_T Product1_DWORK4_g[9];         // '<S42>/Product1'
  real_T Product1_DWORK4_gx[9];        // '<S33>/Product1'
  struct {
    void *LoggedData;
  } ToWorkspace_PWORK;                 // '<S1>/To Workspace'

  struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } FromWorkspace_PWORK;               // '<S389>/From Workspace'

  struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } FromWorkspace1_PWORK;              // '<S389>/From Workspace1'

  struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } FromWorkspace2_PWORK;              // '<S389>/From Workspace2'

  uint32_T RandSeed;                   // '<S500>/Random Number'
  uint32_T RandSeed_p;                 // '<S16>/Random Number1'
  uint32_T RandSeed_i;                 // '<S16>/Random Number'
  uint32_T RandSeed_b;                 // '<S16>/Random Number2'
  uint32_T RandSeed_m;                 // '<S501>/Random Number'
  uint32_T RandSeed_bw;                // '<S16>/Random Number4'
  uint32_T RandSeed_n;                 // '<S16>/Random Number3'
  uint32_T RandSeed_g;                 // '<S16>/Random Number5'
  uint32_T RandSeed_pz;                // '<S505>/Random Number'
  uint32_T RandSeed_a;                 // '<S16>/Random Number7'
  uint32_T RandSeed_b3;                // '<S16>/Random Number6'
  uint32_T RandSeed_iv;                // '<S16>/Random Number8'
  struct {
    int_T PrevIndex;
  } FromWorkspace_IWORK;               // '<S389>/From Workspace'

  struct {
    int_T PrevIndex;
  } FromWorkspace1_IWORK;              // '<S389>/From Workspace1'

  struct {
    int_T PrevIndex;
  } FromWorkspace2_IWORK;              // '<S389>/From Workspace2'

  boolean_T Delay_DSTATE_p;            // '<S121>/Delay'
  boolean_T Delay_DSTATE_o;            // '<S153>/Delay'
  boolean_T Delay1_DSTATE_kt;          // '<S63>/Delay1'
  boolean_T Delay2_DSTATE[2];          // '<S63>/Delay2'
  boolean_T Delay5_DSTATE[3];          // '<S63>/Delay5'
  boolean_T Delay3_DSTATE[4];          // '<S63>/Delay3'
  boolean_T Delay4_DSTATE[5];          // '<S63>/Delay4'
  int8_T SeparatePhases_ActiveSubsystem;// '<Root>/Separate Phases'
  int8_T Ifperforminganexperimentgrabthe;
  // '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
  int8_T ThisIFblockdetermineswhethero_b;
  // '<S13>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhethero_n;
    // '<S13>/This IF block determines whether or not to run the BLUE sim//exp'
  int8_T ThisIFblockdetermineswhethero_j;
    // '<S13>/This IF block determines whether or not to run the RED sim//exp '
  int8_T ThisIFblockdetermineswhethero_d;
  // '<S12>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhether_jc;
    // '<S12>/This IF block determines whether or not to run the BLUE sim//exp'
  int8_T ThisIFblockdetermineswhether_a4;
    // '<S12>/This IF block determines whether or not to run the RED sim//exp '
  int8_T ThisIFblockdetermineswhethero_p;
  // '<S11>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhether_pu;
    // '<S11>/This IF block determines whether or not to run the BLUE sim//exp'
  int8_T ThisIFblockdetermineswhether_au;
    // '<S11>/This IF block determines whether or not to run the RED sim//exp '
  int8_T ExperimentSubPhases_ActiveSubsy;// '<S319>/Experiment Sub-Phases'
  int8_T ExperimentSubPhases_ActiveSub_e;// '<S318>/Experiment Sub-Phases'
  int8_T ExperimentSubPhases_ActiveSub_o;// '<S317>/Experiment Sub-Phases'
  int8_T ThisIFblockdetermineswhether_dy;
  // '<S10>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhether_jm;
    // '<S10>/This IF block determines whether or not to run the BLUE sim//exp'
  int8_T ThisIFblockdetermineswhether_aj;
    // '<S10>/This IF block determines whether or not to run the RED sim//exp '
  int8_T DiscreteTimeIntegrator_PrevRese;// '<S16>/Discrete-Time Integrator'
  int8_T DiscreteTimeIntegrator1_PrevRes;// '<S16>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator2_PrevRes;// '<S16>/Discrete-Time Integrator2'
  int8_T DiscreteTimeIntegrator1_PrevR_f;// '<S243>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator1_PrevR_h;// '<S212>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator1_PrevR_o;// '<S181>/Discrete-Time Integrator1'
  int8_T Checkwhetherbothplatformsarebei;
  // '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
  int8_T ThisIFblockdetermineswhether_d1;
  // '<S76>/This IF block determines whether or not to run the BLACK sim//exp'
  int8_T ThisIFblockdetermineswhethero_o;
    // '<S76>/This IF block determines whether or not to run the RED sim//exp '
  uint8_T icLoad;                      // '<S489>/Delay1'
  uint8_T icLoad_j;                    // '<S491>/Delay1'
  uint8_T icLoad_a;                    // '<S487>/Delay1'
  uint8_T icLoad_e;                    // '<S463>/Delay1'
  uint8_T icLoad_m;                    // '<S465>/Delay1'
  uint8_T icLoad_d;                    // '<S461>/Delay1'
  uint8_T icLoad_dh;                   // '<S444>/Delay1'
  uint8_T icLoad_a2;                   // '<S446>/Delay1'
  uint8_T icLoad_f;                    // '<S448>/Delay1'
  uint8_T icLoad_dq;                   // '<S420>/Delay1'
  uint8_T icLoad_h;                    // '<S422>/Delay1'
  uint8_T icLoad_o;                    // '<S418>/Delay1'
  uint8_T icLoad_jl;                   // '<S400>/Delay1'
  uint8_T icLoad_n;                    // '<S402>/Delay1'
  uint8_T icLoad_g;                    // '<S404>/Delay1'
  uint8_T icLoad_fr;                   // '<S349>/Delay1'
  uint8_T icLoad_me;                   // '<S351>/Delay1'
  uint8_T icLoad_ey;                   // '<S347>/Delay1'
  uint8_T icLoad_ot;                   // '<S332>/Delay1'
  uint8_T icLoad_k;                    // '<S334>/Delay1'
  uint8_T icLoad_l;                    // '<S330>/Delay1'
  uint8_T icLoad_ji;                   // '<S308>/Delay1'
  uint8_T icLoad_a1;                   // '<S310>/Delay1'
  uint8_T icLoad_o0;                   // '<S312>/Delay1'
  uint8_T icLoad_dy;                   // '<S297>/Delay1'
  uint8_T icLoad_hf;                   // '<S299>/Delay1'
  uint8_T icLoad_kl;                   // '<S295>/Delay1'
  uint8_T icLoad_ns;                   // '<S284>/Delay1'
  uint8_T icLoad_k1;                   // '<S286>/Delay1'
  uint8_T icLoad_ep;                   // '<S282>/Delay1'
  uint8_T icLoad_lm;                   // '<S518>/Delay1'
  uint8_T icLoad_em;                   // '<S512>/Delay1'
  uint8_T icLoad_nw;                   // '<S519>/Delay1'
  uint8_T icLoad_og;                   // '<S513>/Delay1'
  uint8_T icLoad_ka;                   // '<S520>/Delay1'
  uint8_T icLoad_lf;                   // '<S521>/Delay1'
  uint8_T icLoad_aq;                   // '<S514>/Delay1'
  uint8_T icLoad_nc;                   // '<S522>/Delay1'
  uint8_T icLoad_kx;                   // '<S515>/Delay1'
  uint8_T icLoad_fa;                   // '<S523>/Delay1'
  uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S16>/Discrete-Time Integrator'
  uint8_T DiscreteTimeIntegrator1_IC_LOAD;// '<S16>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator2_IC_LOAD;// '<S16>/Discrete-Time Integrator2'
  uint8_T icLoad_ky;                   // '<S509>/Delay1'
  uint8_T icLoad_eg;                   // '<S510>/Delay1'
  uint8_T icLoad_h5;                   // '<S516>/Delay1'
  uint8_T icLoad_p;                    // '<S511>/Delay1'
  uint8_T icLoad_c;                    // '<S517>/Delay1'
  uint8_T DiscreteTimeIntegrator1_IC_LO_g;// '<S243>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator1_IC_LO_f;// '<S212>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator1_IC_LO_i;// '<S181>/Discrete-Time Integrator1'
  uint8_T icLoad_kc;                   // '<S115>/Delay1'
  uint8_T icLoad_p1;                   // '<S110>/Delay1'
  uint8_T icLoad_kv;                   // '<S116>/Delay1'
  uint8_T icLoad_cg;                   // '<S111>/Delay1'
  uint8_T icLoad_f0;                   // '<S117>/Delay1'
  uint8_T icLoad_a5;                   // '<S112>/Delay1'
  uint8_T icLoad_l4;                   // '<S109>/Delay1'
  uint8_T icLoad_go;                   // '<S118>/Delay1'
  uint8_T icLoad_ay;                   // '<S113>/Delay1'
  uint8_T icLoad_aqe;                  // '<S119>/Delay1'
  uint8_T icLoad_ma;                   // '<S114>/Delay1'
  uint8_T icLoad_hj;                   // '<S120>/Delay1'
  uint8_T icLoad_ky1;                  // '<S87>/Delay1'
  uint8_T icLoad_of;                   // '<S82>/Delay1'
  uint8_T icLoad_oi;                   // '<S88>/Delay1'
  uint8_T icLoad_hb;                   // '<S83>/Delay1'
  uint8_T icLoad_ap;                   // '<S89>/Delay1'
  uint8_T icLoad_hc;                   // '<S84>/Delay1'
  uint8_T icLoad_nn;                   // '<S81>/Delay1'
  uint8_T icLoad_gc;                   // '<S90>/Delay1'
  uint8_T icLoad_d0;                   // '<S85>/Delay1'
  uint8_T icLoad_a1a;                  // '<S91>/Delay1'
  uint8_T icLoad_ms;                   // '<S86>/Delay1'
  uint8_T icLoad_gi;                   // '<S92>/Delay1'
  uint8_T icLoad_dr;                   // '<S147>/Delay1'
  uint8_T icLoad_mq;                   // '<S142>/Delay1'
  uint8_T icLoad_lr;                   // '<S148>/Delay1'
  uint8_T icLoad_b;                    // '<S143>/Delay1'
  uint8_T icLoad_aa;                   // '<S149>/Delay1'
  uint8_T icLoad_cw;                   // '<S144>/Delay1'
  uint8_T icLoad_nsj;                  // '<S141>/Delay1'
  uint8_T icLoad_lrw;                  // '<S150>/Delay1'
  uint8_T icLoad_a2d;                  // '<S145>/Delay1'
  uint8_T icLoad_mp;                   // '<S151>/Delay1'
  uint8_T icLoad_gb;                   // '<S146>/Delay1'
  uint8_T icLoad_i;                    // '<S152>/Delay1'
  boolean_T Unwrap1_FirstStep;         // '<S80>/Unwrap1'
  boolean_T Unwrap_FirstStep;          // '<S80>/Unwrap'
  boolean_T Unwrap1_FirstStep_o;       // '<S79>/Unwrap1'
  boolean_T Unwrap_FirstStep_p;        // '<S79>/Unwrap'
  boolean_T Unwrap1_FirstStep_d;       // '<S77>/Unwrap1'
  boolean_T Unwrap_FirstStep_d;        // '<S77>/Unwrap'
  boolean_T EnabledSubsystem1_MODE;    // '<S80>/Enabled Subsystem1'
  boolean_T EnabledSubsystem_MODE;     // '<S106>/Enabled Subsystem'
  boolean_T EnabledSubsystem1_MODE_d;  // '<S77>/Enabled Subsystem1'
  boolean_T EnabledSubsystem_MODE_j;   // '<S77>/Enabled Subsystem'
  boolean_T EnabledSubsystem_MODE_m;   // '<S138>/Enabled Subsystem'
  DW_ChangeBLUEBehavior_Combi_i_T ChangeBLUEBehavior_b;// '<S13>/Change BLUE Behavior' 
  DW_ChangeBLUEBehavior_Combi_i_T ChangeBLUEBehavior_gj;// '<S12>/Change BLUE Behavior' 
  DW_SendPositionCommandstoMani_T SendPositionCommandstoManipul_p;
                              // '<S276>/Send Position Commands to Manipulator'
  DW_SubPhase1_Combined_Experim_T SubPhase4_f;// '<S318>/Sub-Phase #4'
  DW_SubPhase1_Combined_Experim_T SubPhase1_a;// '<S318>/Sub-Phase #1'
  DW_SendPositionCommandstoMani_T SendPositionCommandstoManipulat;
                              // '<S276>/Send Position Commands to Manipulator'
  DW_Phase0WaitforSynchronizati_T Phase1StartFloating;// '<Root>/Phase #1:  Start Floating ' 
  DW_Phase0WaitforSynchronizati_T Phase0WaitforSynchronization;
                                // '<Root>/Phase #0:  Wait for Synchronization'
  DW_CalculateRunningMean_Com_e_T CalculateRunningMean_cf;// '<S256>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_jr;// '<S255>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_j1;// '<S254>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_et;// '<S243>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_om;// '<S242>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_oo;// '<S241>/Calculate Running Mean' 
  DW_AHRS2_Combined_Experiment__T AHRS2_pn;// '<S169>/AHRS2'
  DW_CalculateRunningMean_Com_e_T CalculateRunningMean_o;// '<S225>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_b;// '<S224>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_j;// '<S223>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_l;// '<S212>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_f;// '<S211>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_c;// '<S210>/Calculate Running Mean' 
  DW_AHRS2_Combined_Experiment__T AHRS2_p;// '<S169>/AHRS2'
  DW_CalculateRunningMean_Com_e_T CalculateRunningMean_ab;// '<S194>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_a;// '<S193>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_d;// '<S192>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_h;// '<S181>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean_e;// '<S180>/Calculate Running Mean' 
  DW_CalculateRunningMean_Combi_T CalculateRunningMean;// '<S179>/Calculate Running Mean' 
  DW_AHRS2_Combined_Experiment__T AHRS2;// '<S169>/AHRS2'
} DW_Combined_Experiment_ArmOnl_T;

// Zero-crossing (trigger) state
typedef struct {
  ZCSigState SampleandHold_Trig_ZCE;   // '<S508>/Sample and Hold'
  ZCSigState SampleandHold1_Trig_ZCE;  // '<S507>/Sample and Hold1'
  ZCSigState SampleandHold1_Trig_ZCE_c;// '<S506>/Sample and Hold1'
} PrevZCX_Combined_Experiment_A_T;

// Parameters for system: '<S169>/AHRS2'
struct P_AHRS2_Combined_Experiment_A_T_ {
  real_T AHRS2_AccelerometerNoise;     // Expression: 0.0001924722
                                          //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_GyroscopeNoise;         // Expression: 9.1385e-5
                                          //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_MagnetometerNoise;      // Expression: 0.1
                                          //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_GyroscopeDriftNoise;    // Expression: 3.0462e-13
                                          //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_LinearAccelerationNoise;// Expression: 0.0096236100000000012
                                          //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_MagneticDisturbanceNoise;// Expression: 0.5
                                           //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_LinearAccelerationDecayFa;// Expression: 0.5
                                            //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_MagneticDisturbanceDecayF;// Expression: 0.5
                                            //  Referenced by: '<S169>/AHRS2'

  real_T AHRS2_ExpectedMagneticFieldStre;// Expression: 33
                                            //  Referenced by: '<S169>/AHRS2'

};

// Parameters for system: '<S179>/Calculate Running Mean'
struct P_CalculateRunningMean_Combin_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S182>/Out1'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S182>/Delay'

  real_T Delay1_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S182>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S182>/Delay2'

  real_T Delay3_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S182>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S182>/Delay4'

};

// Parameters for system: '<S194>/Calculate Running Mean'
struct P_CalculateRunningMean_Comb_p_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S201>/Out1'

  real_T Constant_Value;               // Expression: 9.81
                                          //  Referenced by: '<S201>/Constant'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S201>/Delay'

  real_T Delay1_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S201>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S201>/Delay2'

  real_T Delay3_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S201>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S201>/Delay4'

};

// Parameters for system: '<S8>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Combined_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S267>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S267>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S267>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S267>/Puck State'

};

// Parameters for system: '<Root>/Phase #0:  Wait for Synchronization'
struct P_Phase0WaitforSynchronizatio_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S266>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S266>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S266>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S266>/Puck State'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S268>/Constant'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S268>/Constant1'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S268>/Constant2'

  real_T Constant3_Value_h;            // Expression: 0
                                          //  Referenced by: '<S268>/Constant3'

  real_T Constant4_Value_g;            // Expression: 0
                                          //  Referenced by: '<S268>/Constant4'

  real_T Constant5_Value_e;            // Expression: 0
                                          //  Referenced by: '<S268>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S268>/Constant6'

  real_T PuckState_Value_b;            // Expression: 0
                                          //  Referenced by: '<S268>/Puck State'

  P_ChangeBLUEBehavior_Combined_T ChangeBLUEBehavior;// '<S8>/Change BLUE Behavior' 
};

// Parameters for system: '<S317>/Sub-Phase #2 '
struct P_SubPhase2_Combined_Experime_T_ {
  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S322>/Constant'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S322>/Puck State'

};

// Parameters for system: '<S318>/Sub-Phase #1'
struct P_SubPhase1_Combined_Experime_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S366>/Out1'

  real_T Out1_Y0_m;                    // Computed Parameter: Out1_Y0_m
                                          //  Referenced by: '<S368>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S370>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S356>/Puck State'

};

// Parameters for system: '<S318>/Sub-Phase #2 '
struct P_SubPhase2_Combined_Experi_h_T_ {
  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S357>/Constant'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S357>/Puck State'

};

// Parameters for system: '<S319>/Sub-Phase #1'
struct P_SubPhase1_Combined_Experi_m_T_ {
  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S387>/Constant'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S387>/Constant1'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S387>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S387>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S387>/Constant6'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S387>/Puck State'

};

// Parameters for system: '<S12>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Combin_c_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S432>/Out1'

  real_T Out1_Y0_k;                    // Computed Parameter: Out1_Y0_k
                                          //  Referenced by: '<S434>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S436>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S411>/Puck State'

};

// Parameters (default storage)
struct P_Combined_Experiment_ArmOnly_T_ {
  real_T F_thrusters_BLACK[8];         // Variable: F_thrusters_BLACK
                                          //  Referenced by:
                                          //    '<S27>/MATLAB Function'
                                          //    '<S27>/MATLAB Function1'

  real_T F_thrusters_BLUE[8];          // Variable: F_thrusters_BLUE
                                          //  Referenced by:
                                          //    '<S36>/MATLAB Function'
                                          //    '<S36>/MATLAB Function1'

  real_T F_thrusters_RED[8];           // Variable: F_thrusters_RED
                                          //  Referenced by:
                                          //    '<S45>/MATLAB Function'
                                          //    '<S45>/MATLAB Function1'

  real_T Kd_tb;                        // Variable: Kd_tb
                                          //  Referenced by:
                                          //    '<S278>/kd_tb'
                                          //    '<S414>/kd_tb'
                                          //    '<S457>/kd_tb'
                                          //    '<S326>/kd_tb'
                                          //    '<S340>/kd_tb'

  real_T Kd_tblue;                     // Variable: Kd_tblue
                                          //  Referenced by:
                                          //    '<S291>/kd_tb'
                                          //    '<S427>/kd_tb'
                                          //    '<S470>/kd_tb'
                                          //    '<S361>/kd_tb'
                                          //    '<S374>/kd_tb'

  real_T Kd_tr;                        // Variable: Kd_tr
                                          //  Referenced by:
                                          //    '<S304>/kd_tr'
                                          //    '<S440>/kd_tr'
                                          //    '<S483>/kd_tr'
                                          //    '<S392>/kd_tr'

  real_T Kd_xb;                        // Variable: Kd_xb
                                          //  Referenced by:
                                          //    '<S279>/kd_xb'
                                          //    '<S415>/kd_xb'
                                          //    '<S458>/kd_xb'
                                          //    '<S327>/kd_xb'
                                          //    '<S341>/kd_xb'

  real_T Kd_xblue;                     // Variable: Kd_xblue
                                          //  Referenced by:
                                          //    '<S292>/kd_xb'
                                          //    '<S428>/kd_xb'
                                          //    '<S471>/kd_xb'
                                          //    '<S362>/kd_xb'
                                          //    '<S375>/kd_xb'

  real_T Kd_xr;                        // Variable: Kd_xr
                                          //  Referenced by:
                                          //    '<S305>/kd_xr'
                                          //    '<S441>/kd_xr'
                                          //    '<S484>/kd_xr'
                                          //    '<S393>/kd_xr'

  real_T Kd_yb;                        // Variable: Kd_yb
                                          //  Referenced by:
                                          //    '<S280>/kd_yb'
                                          //    '<S416>/kd_yb'
                                          //    '<S459>/kd_yb'
                                          //    '<S328>/kd_yb'
                                          //    '<S342>/kd_yb'

  real_T Kd_yblue;                     // Variable: Kd_yblue
                                          //  Referenced by:
                                          //    '<S293>/kd_yb'
                                          //    '<S429>/kd_yb'
                                          //    '<S472>/kd_yb'
                                          //    '<S363>/kd_yb'
                                          //    '<S376>/kd_yb'

  real_T Kd_yr;                        // Variable: Kd_yr
                                          //  Referenced by:
                                          //    '<S306>/kd_yr'
                                          //    '<S442>/kd_yr'
                                          //    '<S485>/kd_yr'
                                          //    '<S394>/kd_yr'

  real_T Kp_tb;                        // Variable: Kp_tb
                                          //  Referenced by:
                                          //    '<S278>/kp_tb'
                                          //    '<S414>/kp_tb'
                                          //    '<S457>/kp_tb'
                                          //    '<S326>/kp_tb'
                                          //    '<S340>/kp_tb'

  real_T Kp_tblue;                     // Variable: Kp_tblue
                                          //  Referenced by:
                                          //    '<S291>/kp_tb'
                                          //    '<S427>/kp_tb'
                                          //    '<S470>/kp_tb'
                                          //    '<S361>/kp_tb'
                                          //    '<S374>/kp_tb'

  real_T Kp_tr;                        // Variable: Kp_tr
                                          //  Referenced by:
                                          //    '<S304>/kp_tr'
                                          //    '<S440>/kp_tr'
                                          //    '<S483>/kp_tr'
                                          //    '<S392>/kp_tr'

  real_T Kp_xb;                        // Variable: Kp_xb
                                          //  Referenced by:
                                          //    '<S279>/kp_xb'
                                          //    '<S415>/kp_xb'
                                          //    '<S458>/kp_xb'
                                          //    '<S327>/kp_xb'
                                          //    '<S341>/kp_xb'

  real_T Kp_xblue;                     // Variable: Kp_xblue
                                          //  Referenced by:
                                          //    '<S292>/kp_xb'
                                          //    '<S428>/kp_xb'
                                          //    '<S471>/kp_xb'
                                          //    '<S362>/kp_xb'
                                          //    '<S375>/kp_xb'

  real_T Kp_xr;                        // Variable: Kp_xr
                                          //  Referenced by:
                                          //    '<S305>/kp_xr'
                                          //    '<S441>/kp_xr'
                                          //    '<S484>/kp_xr'
                                          //    '<S393>/kp_xr'

  real_T Kp_yb;                        // Variable: Kp_yb
                                          //  Referenced by:
                                          //    '<S280>/kp_yb'
                                          //    '<S416>/kp_yb'
                                          //    '<S459>/kp_yb'
                                          //    '<S328>/kp_yb'
                                          //    '<S342>/kp_yb'

  real_T Kp_yblue;                     // Variable: Kp_yblue
                                          //  Referenced by:
                                          //    '<S293>/kp_yb'
                                          //    '<S429>/kp_yb'
                                          //    '<S472>/kp_yb'
                                          //    '<S363>/kp_yb'
                                          //    '<S376>/kp_yb'

  real_T Kp_yr;                        // Variable: Kp_yr
                                          //  Referenced by:
                                          //    '<S306>/kp_yr'
                                          //    '<S442>/kp_yr'
                                          //    '<S485>/kp_yr'
                                          //    '<S394>/kp_yr'

  real_T Phase0_End;                   // Variable: Phase0_End
                                          //  Referenced by: '<Root>/Constant4'

  real_T Phase1_End;                   // Variable: Phase1_End
                                          //  Referenced by: '<Root>/Constant'

  real_T Phase2_End;                   // Variable: Phase2_End
                                          //  Referenced by: '<Root>/Constant1'

  real_T Phase3_End;                   // Variable: Phase3_End
                                          //  Referenced by: '<Root>/Constant2'

  real_T Phase3_SubPhase1_End;         // Variable: Phase3_SubPhase1_End
                                          //  Referenced by:
                                          //    '<S317>/Constant4'
                                          //    '<S318>/Constant4'
                                          //    '<S319>/Constant4'

  real_T Phase3_SubPhase2_End;         // Variable: Phase3_SubPhase2_End
                                          //  Referenced by:
                                          //    '<S317>/Constant1'
                                          //    '<S318>/Constant1'
                                          //    '<S319>/Constant1'

  real_T Phase3_SubPhase3_End;         // Variable: Phase3_SubPhase3_End
                                          //  Referenced by:
                                          //    '<S317>/Constant2'
                                          //    '<S318>/Constant2'
                                          //    '<S319>/Constant2'
                                          //    '<S399>/Constant2'
                                          //    '<S344>/Constant1'

  real_T Phase3_SubPhase4_End;         // Variable: Phase3_SubPhase4_End
                                          //  Referenced by:
                                          //    '<S317>/Constant3'
                                          //    '<S318>/Constant3'
                                          //    '<S319>/Constant3'

  real_T Phase4_End;                   // Variable: Phase4_End
                                          //  Referenced by: '<Root>/Constant3'

  real_T Phase5_End;                   // Variable: Phase5_End
                                          //  Referenced by: '<Root>/Constant6'

  real_T WhoAmI;                       // Variable: WhoAmI
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

  real_T drop_states_BLACK[3];         // Variable: drop_states_BLACK
                                          //  Referenced by: '<S500>/Velocity to Position'

  real_T drop_states_BLUE[3];          // Variable: drop_states_BLUE
                                          //  Referenced by: '<S501>/Velocity to Position'

  real_T drop_states_RED[3];           // Variable: drop_states_RED
                                          //  Referenced by: '<S505>/Velocity to Position'

  real_T home_states_BLACK[3];         // Variable: home_states_BLACK
                                          //  Referenced by:
                                          //    '<S410>/Desired Attitude (BLACK)'
                                          //    '<S410>/Desired Px (BLACK)'
                                          //    '<S410>/Desired Py (BLACK)'
                                          //    '<S453>/Constant'
                                          //    '<S453>/Constant2'
                                          //    '<S453>/Constant3'

  real_T home_states_BLUE[3];          // Variable: home_states_BLUE
                                          //  Referenced by:
                                          //    '<S411>/Desired Attitude (BLUE)'
                                          //    '<S411>/Desired Px (BLUE)'
                                          //    '<S411>/Desired Py (BLUE)'
                                          //    '<S454>/Desired Attitude (BLUE)'
                                          //    '<S454>/Desired Px (BLUE)'
                                          //    '<S454>/Desired Py (BLUE)'

  real_T home_states_RED[3];           // Variable: home_states_RED
                                          //  Referenced by:
                                          //    '<S412>/Constant'
                                          //    '<S412>/Constant1'
                                          //    '<S412>/Constant3'
                                          //    '<S455>/Constant'
                                          //    '<S455>/Constant2'
                                          //    '<S455>/Constant3'

  real_T init_states_BLACK[3];         // Variable: init_states_BLACK
                                          //  Referenced by:
                                          //    '<S274>/Desired Attitude (BLACK)'
                                          //    '<S274>/Desired Px (BLACK)'
                                          //    '<S274>/Desired Py (BLACK)'
                                          //    '<S321>/Desired Attitude (BLACK)'
                                          //    '<S321>/Desired X-Position (BLACK)'
                                          //    '<S321>/Desired Y-Position (BLACK)'

  real_T init_states_BLUE[3];          // Variable: init_states_BLUE
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

  real_T init_states_RED[3];           // Variable: init_states_RED
                                          //  Referenced by:
                                          //    '<S276>/Constant'
                                          //    '<S276>/Constant1'
                                          //    '<S276>/Constant3'

  real_T model_param[6];               // Variable: model_param
                                          //  Referenced by:
                                          //    '<S500>/MATLAB Function'
                                          //    '<S501>/MATLAB Function'
                                          //    '<S505>/MATLAB Function'

  real_T noise_variance_BLACK;         // Variable: noise_variance_BLACK
                                          //  Referenced by: '<S500>/Random Number'

  real_T noise_variance_BLUE;          // Variable: noise_variance_BLUE
                                          //  Referenced by: '<S501>/Random Number'

  real_T noise_variance_RED;           // Variable: noise_variance_RED
                                          //  Referenced by: '<S505>/Random Number'

  real_T platformSelection;            // Variable: platformSelection
                                          //  Referenced by:
                                          //    '<S75>/Which PLATFORM is being used?'
                                          //    '<S77>/Stream PhaseSpace to Platform'

  real_T serverRate;                   // Variable: serverRate
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

  real_T simMode;                      // Variable: simMode
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

  real_T thruster_dist2CG_BLACK[8];    // Variable: thruster_dist2CG_BLACK
                                          //  Referenced by:
                                          //    '<S27>/MATLAB Function'
                                          //    '<S27>/MATLAB Function1'

  real_T thruster_dist2CG_BLUE[8];     // Variable: thruster_dist2CG_BLUE
                                          //  Referenced by:
                                          //    '<S36>/MATLAB Function'
                                          //    '<S36>/MATLAB Function1'

  real_T thruster_dist2CG_RED[8];      // Variable: thruster_dist2CG_RED
                                          //  Referenced by:
                                          //    '<S45>/MATLAB Function'
                                          //    '<S45>/MATLAB Function1'

  real_T xLength;                      // Variable: xLength
                                          //  Referenced by:
                                          //    '<S324>/Desired X-Position (BLACK)'
                                          //    '<S390>/MATLAB Function'

  real_T yLength;                      // Variable: yLength
                                          //  Referenced by:
                                          //    '<S324>/Desired Y-Position (BLACK)'
                                          //    '<S390>/MATLAB Function1'

  real_T InitializationBlockforPositionC;// Expression: 400
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T InitializationBlockforPositio_b;// Expression: 0
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T InitializationBlockforPositio_g;// Expression: 200
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T InitializationBlockforPositio_i;// Expression: 4095
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T InitializationBlockforPositio_k;// Expression: 0
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T InitializationBlockforPositi_ba;// Expression: 0
                                            //  Referenced by: '<Root>/Initialization Block for Position Control'

  real_T BLACKPWM_Y0;                  // Computed Parameter: BLACKPWM_Y0
                                          //  Referenced by: '<S22>/BLACK PWM'

  real_T RemoveNegatives_UpperSat;     // Expression: 1000
                                          //  Referenced by: '<S27>/Remove Negatives'

  real_T RemoveNegatives_LowerSat;     // Expression: 0
                                          //  Referenced by: '<S27>/Remove Negatives'

  real_T BLUEPWM_Y0;                   // Computed Parameter: BLUEPWM_Y0
                                          //  Referenced by: '<S23>/BLUE PWM'

  real_T RemoveNegatives_UpperSat_p;   // Expression: 1000
                                          //  Referenced by: '<S36>/Remove Negatives'

  real_T RemoveNegatives_LowerSat_c;   // Expression: 0
                                          //  Referenced by: '<S36>/Remove Negatives'

  real_T REDPWM_Y0;                    // Computed Parameter: REDPWM_Y0
                                          //  Referenced by: '<S24>/RED PWM'

  real_T RemoveNegatives_UpperSat_d;   // Expression: 1000
                                          //  Referenced by: '<S45>/Remove Negatives'

  real_T RemoveNegatives_LowerSat_a;   // Expression: 0
                                          //  Referenced by: '<S45>/Remove Negatives'

  real_T DigitalRead_SampleTime;       // Expression: sampleTime
                                          //  Referenced by: '<S70>/Digital Read'

  real_T SaturateTorque_UpperSat;      // Expression: 0.1
                                          //  Referenced by: '<S58>/Saturate Torque'

  real_T SaturateTorque_LowerSat;      // Expression: -0.1
                                          //  Referenced by: '<S58>/Saturate Torque'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S58>/Delay'

  real_T GearboxRatio_Value;           // Expression: 3375/64
                                          //  Referenced by: '<S62>/Gearbox  Ratio'

  real_T SaturateMotor_UpperSat;       // Expression: 7000
                                          //  Referenced by: '<S59>/Saturate Motor'

  real_T SaturateMotor_LowerSat;       // Expression: -7000
                                          //  Referenced by: '<S59>/Saturate Motor'

  real_T SaturateRPM_UpperSat;         // Expression: (7000*64/3375)
                                          //  Referenced by: '<S58>/Saturate RPM'

  real_T SaturateRPM_LowerSat;         // Expression: -(7000*64/3375)
                                          //  Referenced by: '<S58>/Saturate RPM'

  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S154>/Out1'

  real_T Out1_Y0_m;                    // Computed Parameter: Out1_Y0_m
                                          //  Referenced by: '<S139>/Out1'

  real_T ActualTime_Y0;                // Computed Parameter: ActualTime_Y0
                                          //  Referenced by: '<S140>/Actual Time'

  real_T Out1_Y0_l;                    // Computed Parameter: Out1_Y0_l
                                          //  Referenced by: '<S157>/Out1'

  real_T Out1_Y0_f;                    // Computed Parameter: Out1_Y0_f
                                          //  Referenced by: '<S158>/Out1'

  real_T Out1_Y0_e;                    // Computed Parameter: Out1_Y0_e
                                          //  Referenced by: '<S159>/Out1'

  real_T Out1_Y0_b;                    // Computed Parameter: Out1_Y0_b
                                          //  Referenced by: '<S160>/Out1'

  real_T Out1_Y0_k;                    // Computed Parameter: Out1_Y0_k
                                          //  Referenced by: '<S161>/Out1'

  real_T Out1_Y0_h;                    // Computed Parameter: Out1_Y0_h
                                          //  Referenced by: '<S162>/Out1'

  real_T Out1_Y0_d;                    // Computed Parameter: Out1_Y0_d
                                          //  Referenced by: '<S163>/Out1'

  real_T Out1_Y0_o;                    // Computed Parameter: Out1_Y0_o
                                          //  Referenced by: '<S164>/Out1'

  real_T Out1_Y0_ec;                   // Computed Parameter: Out1_Y0_ec
                                          //  Referenced by: '<S165>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S166>/Out1'

  real_T Out1_Y0_oi;                   // Computed Parameter: Out1_Y0_oi
                                          //  Referenced by: '<S167>/Out1'

  real_T Out1_Y0_fr;                   // Computed Parameter: Out1_Y0_fr
                                          //  Referenced by: '<S168>/Out1'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S137>/Constant'

  real_T Out1_Y0_e2;                   // Computed Parameter: Out1_Y0_e2
                                          //  Referenced by: '<S93>/Out1'

  real_T Out1_Y0_a;                    // Computed Parameter: Out1_Y0_a
                                          //  Referenced by: '<S94>/Out1'

  real_T Out1_Y0_bu;                   // Computed Parameter: Out1_Y0_bu
                                          //  Referenced by: '<S95>/Out1'

  real_T Out1_Y0_i;                    // Computed Parameter: Out1_Y0_i
                                          //  Referenced by: '<S96>/Out1'

  real_T Out1_Y0_bw;                   // Computed Parameter: Out1_Y0_bw
                                          //  Referenced by: '<S97>/Out1'

  real_T Out1_Y0_j;                    // Computed Parameter: Out1_Y0_j
                                          //  Referenced by: '<S98>/Out1'

  real_T Out1_Y0_j3;                   // Computed Parameter: Out1_Y0_j3
                                          //  Referenced by: '<S99>/Out1'

  real_T Out1_Y0_lj;                   // Computed Parameter: Out1_Y0_lj
                                          //  Referenced by: '<S100>/Out1'

  real_T Out1_Y0_g;                    // Computed Parameter: Out1_Y0_g
                                          //  Referenced by: '<S101>/Out1'

  real_T Out1_Y0_id;                   // Computed Parameter: Out1_Y0_id
                                          //  Referenced by: '<S102>/Out1'

  real_T Out1_Y0_j1;                   // Computed Parameter: Out1_Y0_j1
                                          //  Referenced by: '<S103>/Out1'

  real_T Out1_Y0_fb;                   // Computed Parameter: Out1_Y0_fb
                                          //  Referenced by: '<S104>/Out1'

  real_T Out1_Y0_os;                   // Computed Parameter: Out1_Y0_os
                                          //  Referenced by: '<S122>/Out1'

  real_T Out1_Y0_nx;                   // Computed Parameter: Out1_Y0_nx
                                          //  Referenced by: '<S107>/Out1'

  real_T ActualTime_Y0_l;              // Computed Parameter: ActualTime_Y0_l
                                          //  Referenced by: '<S108>/Actual Time'

  real_T Out1_Y0_lf;                   // Computed Parameter: Out1_Y0_lf
                                          //  Referenced by: '<S125>/Out1'

  real_T Out1_Y0_gx;                   // Computed Parameter: Out1_Y0_gx
                                          //  Referenced by: '<S126>/Out1'

  real_T Out1_Y0_ih;                   // Computed Parameter: Out1_Y0_ih
                                          //  Referenced by: '<S127>/Out1'

  real_T Out1_Y0_o0;                   // Computed Parameter: Out1_Y0_o0
                                          //  Referenced by: '<S128>/Out1'

  real_T Out1_Y0_ac;                   // Computed Parameter: Out1_Y0_ac
                                          //  Referenced by: '<S129>/Out1'

  real_T Out1_Y0_bk;                   // Computed Parameter: Out1_Y0_bk
                                          //  Referenced by: '<S130>/Out1'

  real_T Out1_Y0_l0;                   // Computed Parameter: Out1_Y0_l0
                                          //  Referenced by: '<S131>/Out1'

  real_T Out1_Y0_ok;                   // Computed Parameter: Out1_Y0_ok
                                          //  Referenced by: '<S132>/Out1'

  real_T Out1_Y0_ou;                   // Computed Parameter: Out1_Y0_ou
                                          //  Referenced by: '<S133>/Out1'

  real_T Out1_Y0_k0;                   // Computed Parameter: Out1_Y0_k0
                                          //  Referenced by: '<S134>/Out1'

  real_T Out1_Y0_ks;                   // Computed Parameter: Out1_Y0_ks
                                          //  Referenced by: '<S135>/Out1'

  real_T Out1_Y0_ju;                   // Computed Parameter: Out1_Y0_ju
                                          //  Referenced by: '<S136>/Out1'

  real_T Constant_Value_p;             // Expression: 0
                                          //  Referenced by: '<S105>/Constant'

  real_T Out1_Y0_l0t;                  // Computed Parameter: Out1_Y0_l0t
                                          //  Referenced by: '<S183>/Out1'

  real_T Out1_Y0_et;                   // Computed Parameter: Out1_Y0_et
                                          //  Referenced by: '<S186>/Out1'

  real_T Out1_Y0_nxn;                  // Computed Parameter: Out1_Y0_nxn
                                          //  Referenced by: '<S190>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S196>/Out1'

  real_T Out1_Y0_es;                   // Computed Parameter: Out1_Y0_es
                                          //  Referenced by: '<S199>/Out1'

  real_T Out1_Y0_c;                    // Computed Parameter: Out1_Y0_c
                                          //  Referenced by: '<S202>/Out1'

  real_T Constant_Value_h;             // Expression: 0
                                          //  Referenced by: '<S189>/Constant'

  real_T Gain_Gain;                    // Expression: pi/180
                                          //  Referenced by: '<S169>/Gain'

  real_T Gain1_Gain;                   // Expression: 9.81
                                          //  Referenced by: '<S169>/Gain1'

  real_T DiscreteTimeIntegrator1_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainval
                             //  Referenced by: '<S181>/Discrete-Time Integrator1'

  real_T Out1_Y0_i0;                   // Computed Parameter: Out1_Y0_i0
                                          //  Referenced by: '<S214>/Out1'

  real_T Out1_Y0_k3;                   // Computed Parameter: Out1_Y0_k3
                                          //  Referenced by: '<S217>/Out1'

  real_T Out1_Y0_nl;                   // Computed Parameter: Out1_Y0_nl
                                          //  Referenced by: '<S221>/Out1'

  real_T Out1_Y0_pj;                   // Computed Parameter: Out1_Y0_pj
                                          //  Referenced by: '<S227>/Out1'

  real_T Out1_Y0_aci;                  // Computed Parameter: Out1_Y0_aci
                                          //  Referenced by: '<S230>/Out1'

  real_T Out1_Y0_c1;                   // Computed Parameter: Out1_Y0_c1
                                          //  Referenced by: '<S233>/Out1'

  real_T Constant_Value_d;             // Expression: 0
                                          //  Referenced by: '<S220>/Constant'

  real_T Gain_Gain_e;                  // Expression: pi/180
                                          //  Referenced by: '<S170>/Gain'

  real_T Gain1_Gain_e;                 // Expression: 9.81
                                          //  Referenced by: '<S170>/Gain1'

  real_T DiscreteTimeIntegrator1_gainv_d;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_d
                             //  Referenced by: '<S212>/Discrete-Time Integrator1'

  real_T Out1_Y0_e5;                   // Computed Parameter: Out1_Y0_e5
                                          //  Referenced by: '<S245>/Out1'

  real_T Out1_Y0_ov;                   // Computed Parameter: Out1_Y0_ov
                                          //  Referenced by: '<S248>/Out1'

  real_T Out1_Y0_be;                   // Computed Parameter: Out1_Y0_be
                                          //  Referenced by: '<S252>/Out1'

  real_T Out1_Y0_is;                   // Computed Parameter: Out1_Y0_is
                                          //  Referenced by: '<S258>/Out1'

  real_T Out1_Y0_c4;                   // Computed Parameter: Out1_Y0_c4
                                          //  Referenced by: '<S261>/Out1'

  real_T Out1_Y0_kq;                   // Computed Parameter: Out1_Y0_kq
                                          //  Referenced by: '<S264>/Out1'

  real_T Constant_Value_k;             // Expression: 0
                                          //  Referenced by: '<S251>/Constant'

  real_T Gain_Gain_a;                  // Expression: pi/180
                                          //  Referenced by: '<S171>/Gain'

  real_T Gain1_Gain_c;                 // Expression: 9.81
                                          //  Referenced by: '<S171>/Gain1'

  real_T DiscreteTimeIntegrator1_gainv_k;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_k
                             //  Referenced by: '<S243>/Discrete-Time Integrator1'

  real_T _Y0[3];                       // Expression: initCond
                                          //  Referenced by: '<S528>/ '

  real_T _Y0_g[3];                     // Expression: initCond
                                          //  Referenced by: '<S530>/ '

  real_T _Y0_e[3];                     // Expression: initCond
                                          //  Referenced by: '<S532>/ '

  real_T Out1_Y0_cg;                   // Computed Parameter: Out1_Y0_cg
                                          //  Referenced by: '<S533>/Out1'

  real_T Out1_Y0_lc;                   // Computed Parameter: Out1_Y0_lc
                                          //  Referenced by: '<S534>/Out1'

  real_T Out1_Y0_j5;                   // Computed Parameter: Out1_Y0_j5
                                          //  Referenced by: '<S535>/Out1'

  real_T Out1_Y0_fg;                   // Computed Parameter: Out1_Y0_fg
                                          //  Referenced by: '<S536>/Out1'

  real_T Out1_Y0_pt;                   // Computed Parameter: Out1_Y0_pt
                                          //  Referenced by: '<S537>/Out1'

  real_T Out1_Y0_hk;                   // Computed Parameter: Out1_Y0_hk
                                          //  Referenced by: '<S538>/Out1'

  real_T Out1_Y0_cv;                   // Computed Parameter: Out1_Y0_cv
                                          //  Referenced by: '<S539>/Out1'

  real_T Out1_Y0_cu;                   // Computed Parameter: Out1_Y0_cu
                                          //  Referenced by: '<S540>/Out1'

  real_T Out1_Y0_or;                   // Computed Parameter: Out1_Y0_or
                                          //  Referenced by: '<S541>/Out1'

  real_T Out1_Y0_ls;                   // Computed Parameter: Out1_Y0_ls
                                          //  Referenced by: '<S542>/Out1'

  real_T Out1_Y0_c4g;                  // Computed Parameter: Out1_Y0_c4g
                                          //  Referenced by: '<S543>/Out1'

  real_T Out1_Y0_gp;                   // Computed Parameter: Out1_Y0_gp
                                          //  Referenced by: '<S544>/Out1'

  real_T Out1_Y0_m2;                   // Computed Parameter: Out1_Y0_m2
                                          //  Referenced by: '<S545>/Out1'

  real_T Out1_Y0_gr;                   // Computed Parameter: Out1_Y0_gr
                                          //  Referenced by: '<S546>/Out1'

  real_T Out1_Y0_gq;                   // Computed Parameter: Out1_Y0_gq
                                          //  Referenced by: '<S547>/Out1'

  real_T Constant_Value_l;             // Expression: 0
                                          //  Referenced by: '<S502>/Constant'

  real_T Constant_Value_b;             // Expression: 0
                                          //  Referenced by: '<S503>/Constant'

  real_T Constant_Value_hs;            // Expression: 0
                                          //  Referenced by: '<S504>/Constant'

  real_T AccelerationtoVelocity_gainval;
                           // Computed Parameter: AccelerationtoVelocity_gainval
                              //  Referenced by: '<S500>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC;    // Expression: 0
                                          //  Referenced by: '<S500>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval;
                               // Computed Parameter: VelocitytoPosition_gainval
                                  //  Referenced by: '<S500>/Velocity to Position'

  real_T RandomNumber_Mean;            // Expression: 0
                                          //  Referenced by: '<S500>/Random Number'

  real_T RandomNumber_Seed;            // Expression: 0
                                          //  Referenced by: '<S500>/Random Number'

  real_T RandomNumber1_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number1'

  real_T RandomNumber1_StdDev;       // Computed Parameter: RandomNumber1_StdDev
                                        //  Referenced by: '<S16>/Random Number1'

  real_T RandomNumber1_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number1'

  real_T RandomNumber_Mean_g;          // Expression: 0
                                          //  Referenced by: '<S16>/Random Number'

  real_T RandomNumber_StdDev;         // Computed Parameter: RandomNumber_StdDev
                                         //  Referenced by: '<S16>/Random Number'

  real_T RandomNumber_Seed_d;          // Expression: 0
                                          //  Referenced by: '<S16>/Random Number'

  real_T RandomNumber2_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number2'

  real_T RandomNumber2_StdDev;       // Computed Parameter: RandomNumber2_StdDev
                                        //  Referenced by: '<S16>/Random Number2'

  real_T RandomNumber2_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number2'

  real_T AccelerationtoVelocity_gainva_e;
                          // Computed Parameter: AccelerationtoVelocity_gainva_e
                             //  Referenced by: '<S501>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_f;  // Expression: 0
                                          //  Referenced by: '<S501>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_n;
                             // Computed Parameter: VelocitytoPosition_gainval_n
                                //  Referenced by: '<S501>/Velocity to Position'

  real_T RandomNumber_Mean_o;          // Expression: 0
                                          //  Referenced by: '<S501>/Random Number'

  real_T RandomNumber_Seed_a;          // Expression: 0
                                          //  Referenced by: '<S501>/Random Number'

  real_T RandomNumber4_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number4'

  real_T RandomNumber4_StdDev;       // Computed Parameter: RandomNumber4_StdDev
                                        //  Referenced by: '<S16>/Random Number4'

  real_T RandomNumber4_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number4'

  real_T RandomNumber3_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number3'

  real_T RandomNumber3_StdDev;       // Computed Parameter: RandomNumber3_StdDev
                                        //  Referenced by: '<S16>/Random Number3'

  real_T RandomNumber3_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number3'

  real_T RandomNumber5_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number5'

  real_T RandomNumber5_StdDev;       // Computed Parameter: RandomNumber5_StdDev
                                        //  Referenced by: '<S16>/Random Number5'

  real_T RandomNumber5_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number5'

  real_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<S16>/Discrete-Time Integrator'

  real_T DiscreteTimeIntegrator1_gainv_c;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_c
                             //  Referenced by: '<S16>/Discrete-Time Integrator1'

  real_T DiscreteTimeIntegrator2_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator2_gainval
                             //  Referenced by: '<S16>/Discrete-Time Integrator2'

  real_T AccelerationtoVelocity_gainva_n;
                          // Computed Parameter: AccelerationtoVelocity_gainva_n
                             //  Referenced by: '<S505>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_k;  // Expression: 0
                                          //  Referenced by: '<S505>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_p;
                             // Computed Parameter: VelocitytoPosition_gainval_p
                                //  Referenced by: '<S505>/Velocity to Position'

  real_T RandomNumber_Mean_n;          // Expression: 0
                                          //  Referenced by: '<S505>/Random Number'

  real_T RandomNumber_Seed_n;          // Expression: 0
                                          //  Referenced by: '<S505>/Random Number'

  real_T RandomNumber7_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number7'

  real_T RandomNumber7_StdDev;       // Computed Parameter: RandomNumber7_StdDev
                                        //  Referenced by: '<S16>/Random Number7'

  real_T RandomNumber7_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number7'

  real_T RandomNumber6_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number6'

  real_T RandomNumber6_StdDev;       // Computed Parameter: RandomNumber6_StdDev
                                        //  Referenced by: '<S16>/Random Number6'

  real_T RandomNumber6_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number6'

  real_T RandomNumber8_Mean;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number8'

  real_T RandomNumber8_StdDev;       // Computed Parameter: RandomNumber8_StdDev
                                        //  Referenced by: '<S16>/Random Number8'

  real_T RandomNumber8_Seed;           // Expression: 0
                                          //  Referenced by: '<S16>/Random Number8'

  real_T Out1_Y0_mp;                   // Computed Parameter: Out1_Y0_mp
                                          //  Referenced by: '<S283>/Out1'

  real_T Out1_Y0_av;                   // Computed Parameter: Out1_Y0_av
                                          //  Referenced by: '<S285>/Out1'

  real_T Out1_Y0_g1;                   // Computed Parameter: Out1_Y0_g1
                                          //  Referenced by: '<S287>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S274>/Puck State'

  real_T Out1_Y0_ovt;                  // Computed Parameter: Out1_Y0_ovt
                                          //  Referenced by: '<S296>/Out1'

  real_T Out1_Y0_a4;                   // Computed Parameter: Out1_Y0_a4
                                          //  Referenced by: '<S298>/Out1'

  real_T Out1_Y0_nv;                   // Computed Parameter: Out1_Y0_nv
                                          //  Referenced by: '<S300>/Out1'

  real_T PuckState_Value_o;            // Expression: 1
                                          //  Referenced by: '<S275>/Puck State'

  real_T Out1_Y0_frl;                  // Computed Parameter: Out1_Y0_frl
                                          //  Referenced by: '<S309>/Out1'

  real_T Out1_Y0_bka;                  // Computed Parameter: Out1_Y0_bka
                                          //  Referenced by: '<S311>/Out1'

  real_T Out1_Y0_j2;                   // Computed Parameter: Out1_Y0_j2
                                          //  Referenced by: '<S313>/Out1'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S276>/Constant2'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S276>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S276>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S276>/Constant6'

  real_T PuckState_Value_l;            // Expression: 1
                                          //  Referenced by: '<S276>/Puck State'

  real_T Constant7_Value;              // Expression: 1.5708
                                          //  Referenced by: '<S276>/Constant7'

  real_T Constant8_Value;              // Expression: 1.5708
                                          //  Referenced by: '<S276>/Constant8'

  real_T Constant9_Value;              // Expression: 0
                                          //  Referenced by: '<S276>/Constant9'

  real_T Out1_Y0_pn;                   // Computed Parameter: Out1_Y0_pn
                                          //  Referenced by: '<S331>/Out1'

  real_T Out1_Y0_bd;                   // Computed Parameter: Out1_Y0_bd
                                          //  Referenced by: '<S333>/Out1'

  real_T Out1_Y0_ai;                   // Computed Parameter: Out1_Y0_ai
                                          //  Referenced by: '<S335>/Out1'

  real_T PuckState_Value_n;            // Expression: 1
                                          //  Referenced by: '<S321>/Puck State'

  real_T Out1_Y0_pz;                   // Computed Parameter: Out1_Y0_pz
                                          //  Referenced by: '<S348>/Out1'

  real_T Out1_Y0_dz;                   // Computed Parameter: Out1_Y0_dz
                                          //  Referenced by: '<S350>/Out1'

  real_T Out1_Y0_jz;                   // Computed Parameter: Out1_Y0_jz
                                          //  Referenced by: '<S352>/Out1'

  real_T DesiredRateBLACK_Value;       // Expression: 0.034906585
                                          //  Referenced by: '<S339>/Desired Rate (BLACK)'

  real_T PuckState_Value_e;            // Expression: 1
                                          //  Referenced by: '<S324>/Puck State'

  real_T Constant_Value_f;             // Expression: 0
                                          //  Referenced by: '<S389>/Constant'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S389>/Constant1'

  real_T Constant4_Value_p;            // Expression: 0
                                          //  Referenced by: '<S389>/Constant4'

  real_T Constant5_Value_k;            // Expression: 0
                                          //  Referenced by: '<S389>/Constant5'

  real_T Constant6_Value_d;            // Expression: 0
                                          //  Referenced by: '<S389>/Constant6'

  real_T PuckState_Value_lm;           // Expression: 1
                                          //  Referenced by: '<S389>/Puck State'

  real_T Out1_Y0_pz4;                  // Computed Parameter: Out1_Y0_pz4
                                          //  Referenced by: '<S401>/Out1'

  real_T Out1_Y0_of;                   // Computed Parameter: Out1_Y0_of
                                          //  Referenced by: '<S403>/Out1'

  real_T Out1_Y0_ko;                   // Computed Parameter: Out1_Y0_ko
                                          //  Referenced by: '<S405>/Out1'

  real_T Constant1_Value_f;            // Expression: 0
                                          //  Referenced by: '<S390>/Constant1'

  real_T Constant4_Value_k;            // Expression: 0
                                          //  Referenced by: '<S390>/Constant4'

  real_T Constant5_Value_l;            // Expression: 0
                                          //  Referenced by: '<S390>/Constant5'

  real_T Constant6_Value_k;            // Expression: 0
                                          //  Referenced by: '<S390>/Constant6'

  real_T Constant_Value_bf;            // Expression: 0
                                          //  Referenced by: '<S398>/Constant'

  real_T PuckState_Value_f;            // Expression: 1
                                          //  Referenced by: '<S390>/Puck State'

  real_T Out1_Y0_jr;                   // Computed Parameter: Out1_Y0_jr
                                          //  Referenced by: '<S419>/Out1'

  real_T Out1_Y0_mr;                   // Computed Parameter: Out1_Y0_mr
                                          //  Referenced by: '<S421>/Out1'

  real_T Out1_Y0_i0r;                  // Computed Parameter: Out1_Y0_i0r
                                          //  Referenced by: '<S423>/Out1'

  real_T PuckState_Value_fj;           // Expression: 1
                                          //  Referenced by: '<S410>/Puck State'

  real_T Out1_Y0_nf;                   // Computed Parameter: Out1_Y0_nf
                                          //  Referenced by: '<S445>/Out1'

  real_T Out1_Y0_gb;                   // Computed Parameter: Out1_Y0_gb
                                          //  Referenced by: '<S447>/Out1'

  real_T Out1_Y0_da;                   // Computed Parameter: Out1_Y0_da
                                          //  Referenced by: '<S449>/Out1'

  real_T Constant2_Value_g;            // Expression: 0
                                          //  Referenced by: '<S412>/Constant2'

  real_T Constant4_Value_h;            // Expression: 0
                                          //  Referenced by: '<S412>/Constant4'

  real_T Constant5_Value_h;            // Expression: 0
                                          //  Referenced by: '<S412>/Constant5'

  real_T Constant6_Value_p;            // Expression: 0
                                          //  Referenced by: '<S412>/Constant6'

  real_T PuckState_Value_od;           // Expression: 1
                                          //  Referenced by: '<S412>/Puck State'

  real_T Out1_Y0_gx1;                  // Computed Parameter: Out1_Y0_gx1
                                          //  Referenced by: '<S462>/Out1'

  real_T Out1_Y0_fj;                   // Computed Parameter: Out1_Y0_fj
                                          //  Referenced by: '<S464>/Out1'

  real_T Out1_Y0_bm;                   // Computed Parameter: Out1_Y0_bm
                                          //  Referenced by: '<S466>/Out1'

  real_T PuckState_Value_g;            // Expression: 1
                                          //  Referenced by: '<S453>/Puck State'

  real_T Out1_Y0_i3;                   // Computed Parameter: Out1_Y0_i3
                                          //  Referenced by: '<S488>/Out1'

  real_T Out1_Y0_bs;                   // Computed Parameter: Out1_Y0_bs
                                          //  Referenced by: '<S490>/Out1'

  real_T Out1_Y0_c5;                   // Computed Parameter: Out1_Y0_c5
                                          //  Referenced by: '<S492>/Out1'

  real_T Constant1_Value_k;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant1'

  real_T Constant4_Value_p0;           // Expression: 0
                                          //  Referenced by: '<S455>/Constant4'

  real_T Constant5_Value_o;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant5'

  real_T Constant6_Value_a;            // Expression: 0
                                          //  Referenced by: '<S455>/Constant6'

  real_T PuckState_Value_es;           // Expression: 1
                                          //  Referenced by: '<S455>/Puck State'

  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S496>/Constant3'

  real_T Constant4_Value_e;            // Expression: 0
                                          //  Referenced by: '<S496>/Constant4'

  real_T Constant5_Value_n;            // Expression: 0
                                          //  Referenced by: '<S496>/Constant5'

  real_T PuckState_Value_gd;           // Expression: 0
                                          //  Referenced by: '<S496>/Puck State'

  real_T Constant_Value_j;             // Expression: 0
                                          //  Referenced by: '<S498>/Constant'

  real_T Constant1_Value_l;            // Expression: 0
                                          //  Referenced by: '<S498>/Constant1'

  real_T Constant2_Value_m;            // Expression: 0
                                          //  Referenced by: '<S498>/Constant2'

  real_T Constant4_Value_o;            // Expression: 0
                                          //  Referenced by: '<S498>/Constant4'

  real_T Constant5_Value_f;            // Expression: 0
                                          //  Referenced by: '<S498>/Constant5'

  real_T Constant6_Value_j;            // Expression: 0
                                          //  Referenced by: '<S498>/Constant6'

  real_T PuckState_Value_h;            // Expression: 0
                                          //  Referenced by: '<S498>/Puck State'

  real_T Gain_Gain_a3;                 // Expression: -1
                                          //  Referenced by: '<S498>/Gain'

  real_T Saturation_UpperSat;          // Expression: 0.005
                                          //  Referenced by: '<S498>/Saturation'

  real_T Saturation_LowerSat;          // Expression: -0.005
                                          //  Referenced by: '<S498>/Saturation'

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

  real_T RED_Tz4_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz4'

  real_T RED_Tz5_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz5'

  real_T RED_Tz6_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz6'

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

  real_T RED_Px27_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px27'

  real_T RED_Px28_InitialValue;        // Expression: 0
                                          //  Referenced by: '<Root>/RED_Px28'

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

  real_T RED_Tz2_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz2'

  real_T RED_Tz3_InitialValue;         // Expression: 0
                                          //  Referenced by: '<Root>/RED_Tz3'

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
                                          //  Referenced by: '<S79>/UDP Receive'

  int32_T SendBLACKStatestoBLACKPlatform_;
                          // Computed Parameter: SendBLACKStatestoBLACKPlatform_
                             //  Referenced by: '<S80>/Send BLACK States to  BLACK Platform'

  int32_T UDPSend_Port;                // Computed Parameter: UDPSend_Port
                                          //  Referenced by: '<S15>/UDP Send'

  boolean_T Delay1_InitialCondition;
                                  // Computed Parameter: Delay1_InitialCondition
                                     //  Referenced by: '<S63>/Delay1'

  boolean_T Delay2_InitialCondition;
                                  // Computed Parameter: Delay2_InitialCondition
                                     //  Referenced by: '<S63>/Delay2'

  boolean_T Delay5_InitialCondition;
                                  // Computed Parameter: Delay5_InitialCondition
                                     //  Referenced by: '<S63>/Delay5'

  boolean_T Delay3_InitialCondition;
                                  // Computed Parameter: Delay3_InitialCondition
                                     //  Referenced by: '<S63>/Delay3'

  boolean_T Delay4_InitialCondition;
                                  // Computed Parameter: Delay4_InitialCondition
                                     //  Referenced by: '<S63>/Delay4'

  boolean_T Delay_InitialCondition_k;
                                 // Computed Parameter: Delay_InitialCondition_k
                                    //  Referenced by: '<S153>/Delay'

  boolean_T Delay_InitialCondition_e;
                                 // Computed Parameter: Delay_InitialCondition_e
                                    //  Referenced by: '<S121>/Delay'

  P_ChangeBLUEBehavior_Combined_T ChangeBLUEBehavior_k;// '<S14>/Change BLUE Behavior' 
  P_ChangeBLUEBehavior_Combin_c_T ChangeBLUEBehavior_b;// '<S13>/Change BLUE Behavior' 
  P_ChangeBLUEBehavior_Combin_c_T ChangeBLUEBehavior_gj;// '<S12>/Change BLUE Behavior' 
  P_SubPhase1_Combined_Experi_m_T SubPhase2_g;// '<S319>/Sub-Phase #2 '
  P_SubPhase1_Combined_Experi_m_T SubPhase1_j;// '<S319>/Sub-Phase #1'
  P_SubPhase1_Combined_Experime_T SubPhase4_f;// '<S318>/Sub-Phase #4'
  P_SubPhase2_Combined_Experi_h_T SubPhase3_p;// '<S318>/Sub-Phase #3 '
  P_SubPhase2_Combined_Experi_h_T SubPhase2_m;// '<S318>/Sub-Phase #2 '
  P_SubPhase1_Combined_Experime_T SubPhase1_a;// '<S318>/Sub-Phase #1'
  P_SubPhase2_Combined_Experime_T SubPhase3;// '<S317>/Sub-Phase #3 '
  P_SubPhase2_Combined_Experime_T SubPhase2;// '<S317>/Sub-Phase #2 '
  P_Phase0WaitforSynchronizatio_T Phase1StartFloating;// '<Root>/Phase #1:  Start Floating ' 
  P_Phase0WaitforSynchronizatio_T Phase0WaitforSynchronization;
                                // '<Root>/Phase #0:  Wait for Synchronization'
  P_CalculateRunningMean_Comb_p_T CalculateRunningMean_cf;// '<S256>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_jr;// '<S255>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_j1;// '<S254>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_et;// '<S243>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_om;// '<S242>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_oo;// '<S241>/Calculate Running Mean' 
  P_AHRS2_Combined_Experiment_A_T AHRS2_pn;// '<S169>/AHRS2'
  P_CalculateRunningMean_Comb_p_T CalculateRunningMean_o;// '<S225>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_b;// '<S224>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_j;// '<S223>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_l;// '<S212>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_f;// '<S211>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_c;// '<S210>/Calculate Running Mean' 
  P_AHRS2_Combined_Experiment_A_T AHRS2_p;// '<S169>/AHRS2'
  P_CalculateRunningMean_Comb_p_T CalculateRunningMean_ab;// '<S194>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_a;// '<S193>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_d;// '<S192>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_h;// '<S181>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean_e;// '<S180>/Calculate Running Mean' 
  P_CalculateRunningMean_Combin_T CalculateRunningMean;// '<S179>/Calculate Running Mean' 
  P_AHRS2_Combined_Experiment_A_T AHRS2;// '<S169>/AHRS2'
};

// Real-time Model Data Structure
struct tag_RTM_Combined_Experiment_A_T {
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

  extern P_Combined_Experiment_ArmOnly_T Combined_Experiment_ArmOnly_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern B_Combined_Experiment_ArmOnly_T Combined_Experiment_ArmOnly_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern DW_Combined_Experiment_ArmOnl_T Combined_Experiment_ArmOnly_DW;

// Zero-crossing (trigger) state
extern PrevZCX_Combined_Experiment_A_T Combined_Experiment_Arm_PrevZCX;

// External function called from main
#ifdef __cplusplus

extern "C" {

#endif

  extern void Combined_Experiment_ArmOnly_SetEventsForThisBaseStep(boolean_T
    *eventFlags);

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void Combined_Experiment_ArmOnly_SetEventsForThisBaseStep(boolean_T
    *eventFlags);
  extern void Combined_Experiment_ArmOnly_initialize(void);
  extern void Combined_Experiment_ArmOnly_step(int_T tid);
  extern void Combined_Experiment_ArmOnly_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_Combined_Experiment__T *const Combined_Experiment_ArmOnly_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S123>/Check Signal Attributes' : Unused code path elimination
//  Block '<S124>/Check Signal Attributes' : Unused code path elimination
//  Block '<S155>/Check Signal Attributes' : Unused code path elimination
//  Block '<S156>/Check Signal Attributes' : Unused code path elimination
//  Block '<S178>/Check Signal Attributes' : Unused code path elimination
//  Block '<S209>/Check Signal Attributes' : Unused code path elimination
//  Block '<S240>/Check Signal Attributes' : Unused code path elimination
//  Block '<S79>/Reshape' : Reshape block reduction
//  Block '<S169>/Reshape' : Reshape block reduction
//  Block '<S169>/Reshape1' : Reshape block reduction
//  Block '<S169>/Reshape2' : Reshape block reduction
//  Block '<S169>/Reshape3' : Reshape block reduction
//  Block '<S170>/Reshape' : Reshape block reduction
//  Block '<S170>/Reshape1' : Reshape block reduction
//  Block '<S170>/Reshape2' : Reshape block reduction
//  Block '<S170>/Reshape3' : Reshape block reduction
//  Block '<S171>/Reshape' : Reshape block reduction
//  Block '<S171>/Reshape1' : Reshape block reduction
//  Block '<S171>/Reshape2' : Reshape block reduction
//  Block '<S171>/Reshape3' : Reshape block reduction


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
//  '<Root>' : 'Combined_Experiment_ArmOnly'
//  '<S1>'   : 'Combined_Experiment_ArmOnly/Data Logger Subsystem'
//  '<S2>'   : 'Combined_Experiment_ArmOnly/Float & Magnet Controls'
//  '<S3>'   : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal'
//  '<S4>'   : 'Combined_Experiment_ArmOnly/From Torque, Command RW'
//  '<S5>'   : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP'
//  '<S6>'   : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms'
//  '<S7>'   : 'Combined_Experiment_ArmOnly/Is this a  simulation?'
//  '<S8>'   : 'Combined_Experiment_ArmOnly/Phase #0:  Wait for Synchronization'
//  '<S9>'   : 'Combined_Experiment_ArmOnly/Phase #1:  Start Floating '
//  '<S10>'  : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position'
//  '<S11>'  : 'Combined_Experiment_ArmOnly/Phase #3: Experiment'
//  '<S12>'  : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home'
//  '<S13>'  : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home'
//  '<S14>'  : 'Combined_Experiment_ArmOnly/Phase #6:  Stop Floating and Spin Down RW'
//  '<S15>'  : 'Combined_Experiment_ArmOnly/Real-Time Visualization'
//  '<S16>'  : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics'
//  '<S17>'  : 'Combined_Experiment_ArmOnly/Subsystem'
//  '<S18>'  : 'Combined_Experiment_ArmOnly/Float & Magnet Controls/Change Behavior'
//  '<S19>'  : 'Combined_Experiment_ArmOnly/Float & Magnet Controls/Is this a  simulation?'
//  '<S20>'  : 'Combined_Experiment_ArmOnly/Float & Magnet Controls/Change Behavior/GPIO for Magnet'
//  '<S21>'  : 'Combined_Experiment_ArmOnly/Float & Magnet Controls/Change Behavior/GPIO for Pucks'
//  '<S22>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior'
//  '<S23>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior'
//  '<S24>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior'
//  '<S25>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Is this a  simulation?'
//  '<S26>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Send Commands to PWM Blocks'
//  '<S27>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF'
//  '<S28>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Rotate Forces to Body'
//  '<S29>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S30>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S31>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S32>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S33>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S34>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S35>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S36>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF'
//  '<S37>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Rotate Forces to Body'
//  '<S38>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S39>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S40>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S41>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S42>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S43>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S44>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change BLUE Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S45>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF'
//  '<S46>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Rotate Forces to Body'
//  '<S47>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S48>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S49>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S50>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S51>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S52>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S53>'  : 'Combined_Experiment_ArmOnly/From Force//Torque to PWM Signal/Change RED Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S54>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change BLACK Behavior'
//  '<S55>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change BLUE Behavior'
//  '<S56>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior'
//  '<S57>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Is this a  simulation?'
//  '<S58>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Calculate Saturated RPM Signal'
//  '<S59>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM'
//  '<S60>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Calculate Saturated RPM Signal/Calculate RW RPM Increment'
//  '<S61>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Check if RW is Ready'
//  '<S62>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Convert Wheel Rate to  Motor Rate'
//  '<S63>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Delay Bank'
//  '<S64>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Get RW Direction'
//  '<S65>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Obtain RW Status'
//  '<S66>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/RPM to PWM Value'
//  '<S67>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Send Direction to Motor Controller'
//  '<S68>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Send PWM to Motor Controller'
//  '<S69>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Turn on Motor'
//  '<S70>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Obtain RW Status/GPIO Read1'
//  '<S71>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Send Direction to Motor Controller/GPIO Write'
//  '<S72>'  : 'Combined_Experiment_ArmOnly/From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Turn on Motor/GPIO Write1'
//  '<S73>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Initialize Universal Time (Simulation)'
//  '<S74>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Is this a  simulation?1'
//  '<S75>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States'
//  '<S76>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM'
//  '<S77>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM'
//  '<S78>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Is this a  simulation?'
//  '<S79>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States'
//  '<S80>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States'
//  '<S81>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x1'
//  '<S82>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x10'
//  '<S83>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x11'
//  '<S84>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x12'
//  '<S85>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x2'
//  '<S86>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x3'
//  '<S87>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x4'
//  '<S88>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x5'
//  '<S89>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x6'
//  '<S90>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x7'
//  '<S91>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x8'
//  '<S92>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x9'
//  '<S93>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x1/Hold this value'
//  '<S94>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x10/Hold this value'
//  '<S95>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x11/Hold this value'
//  '<S96>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x12/Hold this value'
//  '<S97>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x2/Hold this value'
//  '<S98>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x3/Hold this value'
//  '<S99>'  : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x4/Hold this value'
//  '<S100>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x5/Hold this value'
//  '<S101>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x6/Hold this value'
//  '<S102>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x7/Hold this value'
//  '<S103>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x8/Hold this value'
//  '<S104>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain BLACK States/x_dot -> x9/Hold this value'
//  '<S105>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Compare To Zero'
//  '<S106>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Correct Time'
//  '<S107>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Enabled Subsystem'
//  '<S108>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Enabled Subsystem1'
//  '<S109>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x1'
//  '<S110>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x10'
//  '<S111>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x11'
//  '<S112>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x12'
//  '<S113>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x2'
//  '<S114>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x3'
//  '<S115>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x4'
//  '<S116>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x5'
//  '<S117>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x6'
//  '<S118>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x7'
//  '<S119>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x8'
//  '<S120>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x9'
//  '<S121>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Correct Time/Edge Detector'
//  '<S122>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Correct Time/Enabled Subsystem'
//  '<S123>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Correct Time/Edge Detector/Check Signal Attributes'
//  '<S124>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/Correct Time/Edge Detector/Check Signal Attributes1'
//  '<S125>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x1/Hold this value'
//  '<S126>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x10/Hold this value'
//  '<S127>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x11/Hold this value'
//  '<S128>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x12/Hold this value'
//  '<S129>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x2/Hold this value'
//  '<S130>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x3/Hold this value'
//  '<S131>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x4/Hold this value'
//  '<S132>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x5/Hold this value'
//  '<S133>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x6/Hold this value'
//  '<S134>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x7/Hold this value'
//  '<S135>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x8/Hold this value'
//  '<S136>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED+BLACK, or RED+BLACK+ARM/Obtain RED States/x_dot -> x9/Hold this value'
//  '<S137>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Compare To Zero'
//  '<S138>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Correct Time'
//  '<S139>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Enabled Subsystem'
//  '<S140>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Enabled Subsystem1'
//  '<S141>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x1'
//  '<S142>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x10'
//  '<S143>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x11'
//  '<S144>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x12'
//  '<S145>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x2'
//  '<S146>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x3'
//  '<S147>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x4'
//  '<S148>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x5'
//  '<S149>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x6'
//  '<S150>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x7'
//  '<S151>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x8'
//  '<S152>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x9'
//  '<S153>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Correct Time/Edge Detector'
//  '<S154>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Correct Time/Enabled Subsystem'
//  '<S155>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Correct Time/Edge Detector/Check Signal Attributes'
//  '<S156>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/Correct Time/Edge Detector/Check Signal Attributes1'
//  '<S157>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x1/Hold this value'
//  '<S158>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x10/Hold this value'
//  '<S159>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x11/Hold this value'
//  '<S160>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x12/Hold this value'
//  '<S161>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x2/Hold this value'
//  '<S162>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x3/Hold this value'
//  '<S163>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x4/Hold this value'
//  '<S164>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x5/Hold this value'
//  '<S165>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x6/Hold this value'
//  '<S166>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x7/Hold this value'
//  '<S167>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x8/Hold this value'
//  '<S168>' : 'Combined_Experiment_ArmOnly/Grab PhaseSpace Data via UDP/Use Hardware to Obtain States/Using RED, BLACK, BLUE, or RED + ARM/x_dot -> x9/Hold this value'
//  '<S169>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior'
//  '<S170>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior'
//  '<S171>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior'
//  '<S172>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Is this a  simulation?'
//  '<S173>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/ChangeOrientation'
//  '<S174>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Create Rotation Matrix'
//  '<S175>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Digital Filter Design'
//  '<S176>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias'
//  '<S177>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)'
//  '<S178>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Digital Filter Design/Check Signal Attributes'
//  '<S179>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX'
//  '<S180>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY'
//  '<S181>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S182>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S183>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S184>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S185>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S186>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S187>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S188>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S189>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S190>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S191>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S192>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S193>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S194>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S195>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S196>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S197>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S198>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S199>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S200>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S201>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S202>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S203>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S204>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/ChangeOrientation'
//  '<S205>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Create Rotation Matrix'
//  '<S206>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Digital Filter Design'
//  '<S207>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias'
//  '<S208>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)'
//  '<S209>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Digital Filter Design/Check Signal Attributes'
//  '<S210>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX'
//  '<S211>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY'
//  '<S212>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S213>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S214>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S215>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S216>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S217>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S218>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S219>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S220>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S221>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S222>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S223>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S224>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S225>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S226>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S227>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S228>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S229>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S230>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S231>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S232>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S233>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S234>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S235>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/ChangeOrientation'
//  '<S236>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Create Rotation Matrix'
//  '<S237>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Digital Filter Design1'
//  '<S238>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias'
//  '<S239>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)'
//  '<S240>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Digital Filter Design1/Check Signal Attributes'
//  '<S241>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX'
//  '<S242>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY'
//  '<S243>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S244>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S245>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S246>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S247>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S248>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S249>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S250>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S251>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S252>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S253>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S254>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S255>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S256>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S257>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S258>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S259>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S260>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S261>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S262>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S263>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S264>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S265>' : 'Combined_Experiment_ArmOnly/Gyroscope & Acceleration Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S266>' : 'Combined_Experiment_ArmOnly/Phase #0:  Wait for Synchronization/Change BLACK Behavior'
//  '<S267>' : 'Combined_Experiment_ArmOnly/Phase #0:  Wait for Synchronization/Change BLUE Behavior'
//  '<S268>' : 'Combined_Experiment_ArmOnly/Phase #0:  Wait for Synchronization/Change RED Behavior'
//  '<S269>' : 'Combined_Experiment_ArmOnly/Phase #0:  Wait for Synchronization/Is this a  simulation?'
//  '<S270>' : 'Combined_Experiment_ArmOnly/Phase #1:  Start Floating /Change BLACK Behavior'
//  '<S271>' : 'Combined_Experiment_ArmOnly/Phase #1:  Start Floating /Change BLUE Behavior'
//  '<S272>' : 'Combined_Experiment_ArmOnly/Phase #1:  Start Floating /Change RED Behavior'
//  '<S273>' : 'Combined_Experiment_ArmOnly/Phase #1:  Start Floating /Is this a  simulation?'
//  '<S274>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior'
//  '<S275>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior'
//  '<S276>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior'
//  '<S277>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Is this a  simulation?'
//  '<S278>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S279>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S280>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S281>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control'
//  '<S282>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S283>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S284>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S285>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S286>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S287>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S288>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S289>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S290>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S291>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S292>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S293>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S294>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control'
//  '<S295>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S296>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S297>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S298>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S299>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S300>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S301>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S302>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S303>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S304>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S305>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S306>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S307>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control'
//  '<S308>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S309>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S310>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S311>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S312>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S313>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S314>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S315>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S316>' : 'Combined_Experiment_ArmOnly/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S317>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior'
//  '<S318>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior'
//  '<S319>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior'
//  '<S320>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Is this a  simulation?'
//  '<S321>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1'
//  '<S322>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 '
//  '<S323>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 '
//  '<S324>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4'
//  '<S325>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Subsystem'
//  '<S326>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S327>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S328>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S329>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control'
//  '<S330>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S331>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S332>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S333>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S334>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S335>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S336>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S337>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S338>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S339>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1'
//  '<S340>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S341>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S342>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S343>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control'
//  '<S344>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Subsystem'
//  '<S345>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1'
//  '<S346>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S347>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S348>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S349>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S350>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S351>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S352>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S353>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S354>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S355>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S356>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1'
//  '<S357>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 '
//  '<S358>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 '
//  '<S359>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4'
//  '<S360>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Subsystem'
//  '<S361>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S362>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S363>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S364>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control'
//  '<S365>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S366>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S367>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S368>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S369>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S370>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S371>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S372>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S373>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S374>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S375>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S376>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S377>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control'
//  '<S378>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S379>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S380>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S381>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S382>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S383>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S384>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S385>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S386>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S387>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1'
//  '<S388>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 '
//  '<S389>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 '
//  '<S390>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4'
//  '<S391>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Subsystem'
//  '<S392>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S393>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S394>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S395>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control'
//  '<S396>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function'
//  '<S397>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function1'
//  '<S398>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED_Rz_REF'
//  '<S399>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Subsystem1'
//  '<S400>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S401>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S402>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S403>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S404>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S405>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S406>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S407>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S408>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S409>' : 'Combined_Experiment_ArmOnly/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Subsystem1/Subsystem'
//  '<S410>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior'
//  '<S411>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior'
//  '<S412>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior'
//  '<S413>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Is this a  simulation?'
//  '<S414>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S415>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S416>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S417>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Hough Control'
//  '<S418>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S419>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S420>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S421>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S422>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S423>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S424>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S425>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S426>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S427>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S428>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S429>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S430>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Hough Control'
//  '<S431>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S432>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S433>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S434>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S435>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S436>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S437>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S438>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S439>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S440>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S441>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S442>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S443>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Hough Control'
//  '<S444>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S445>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S446>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S447>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S448>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S449>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S450>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S451>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S452>' : 'Combined_Experiment_ArmOnly/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S453>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior'
//  '<S454>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior'
//  '<S455>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior'
//  '<S456>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Is this a  simulation?'
//  '<S457>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S458>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S459>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S460>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control'
//  '<S461>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S462>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S463>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S464>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S465>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S466>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S467>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S468>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S469>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S470>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S471>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S472>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S473>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control'
//  '<S474>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S475>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S476>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S477>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S478>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S479>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S480>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S481>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S482>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S483>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S484>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S485>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S486>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Hough Control'
//  '<S487>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S488>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S489>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S490>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S491>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S492>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S493>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S494>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S495>' : 'Combined_Experiment_ArmOnly/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S496>' : 'Combined_Experiment_ArmOnly/Phase #6:  Stop Floating and Spin Down RW/Change BLACK Behavior'
//  '<S497>' : 'Combined_Experiment_ArmOnly/Phase #6:  Stop Floating and Spin Down RW/Change BLUE Behavior'
//  '<S498>' : 'Combined_Experiment_ArmOnly/Phase #6:  Stop Floating and Spin Down RW/Change RED Behavior'
//  '<S499>' : 'Combined_Experiment_ArmOnly/Phase #6:  Stop Floating and Spin Down RW/Is this a  simulation?'
//  '<S500>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/BLACK Dynamics Model'
//  '<S501>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/BLUE  Dynamics Model'
//  '<S502>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Compare To Zero'
//  '<S503>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Compare To Zero1'
//  '<S504>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Compare To Zero2'
//  '<S505>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/RED Dynamics Model'
//  '<S506>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLACK)'
//  '<S507>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLUE)'
//  '<S508>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (RED)'
//  '<S509>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x1'
//  '<S510>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x10'
//  '<S511>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x11'
//  '<S512>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x13'
//  '<S513>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x14'
//  '<S514>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x16'
//  '<S515>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x17'
//  '<S516>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x2'
//  '<S517>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x3'
//  '<S518>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x4'
//  '<S519>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x5'
//  '<S520>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x6'
//  '<S521>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x7'
//  '<S522>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x8'
//  '<S523>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x9'
//  '<S524>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/BLACK Dynamics Model/MATLAB Function'
//  '<S525>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/BLUE  Dynamics Model/MATLAB Function'
//  '<S526>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/RED Dynamics Model/MATLAB Function'
//  '<S527>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLACK)/MATLAB Function'
//  '<S528>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLACK)/Sample and Hold1'
//  '<S529>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLUE)/MATLAB Function'
//  '<S530>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLUE)/Sample and Hold1'
//  '<S531>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (RED)/MATLAB Function'
//  '<S532>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (RED)/Sample and Hold'
//  '<S533>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x1/Hold this value'
//  '<S534>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x10/Hold this value'
//  '<S535>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x11/Hold this value'
//  '<S536>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x13/Hold this value'
//  '<S537>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x14/Hold this value'
//  '<S538>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x16/Hold this value'
//  '<S539>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x17/Hold this value'
//  '<S540>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x2/Hold this value'
//  '<S541>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x3/Hold this value'
//  '<S542>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x4/Hold this value'
//  '<S543>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x5/Hold this value'
//  '<S544>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x6/Hold this value'
//  '<S545>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x7/Hold this value'
//  '<S546>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x8/Hold this value'
//  '<S547>' : 'Combined_Experiment_ArmOnly/Simulate Plant Dynamics/x_dot -> x9/Hold this value'

#endif                             // RTW_HEADER_Combined_Experiment_ArmOnly_h_
