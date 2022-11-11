//
//  Template_v3_08_2020a.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Template_v3_08_2020a".
//
//  Model version              : 1.622
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Sun Jul 10 13:38:58 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_Template_v3_08_2020a_h_
#define RTW_HEADER_Template_v3_08_2020a_h_
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "MW_gpio.h"
#include "dynamixel_functions.h"
#include "dynamixel_sdk.h"
#include "MW_pigs.h"
#include "DAHostLib_Network.h"
#include "MW_I2C.h"
#include "resource_headers.h"
#include "Template_v3_08_2020a_types.h"

// Shared type includes
#include "multiword_types.h"
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
} B_MATLABFunction10_Template_v_T;

// Block signals for system '<S34>/MATLAB Function2'
typedef struct {
  real_T ThrustPer_Final[8];           // '<S34>/MATLAB Function2'
  real_T ThrustPer_Sat[8];
} B_MATLABFunction2_Template_v3_T;

// Block signals for system '<S41>/Create Rotation Matrix'
typedef struct {
  real_T C_Ib[4];                      // '<S41>/Create Rotation Matrix'
} B_CreateRotationMatrix_Templa_T;

// Block signals for system '<S35>/Create Rotation Matrix'
typedef struct {
  real_T C_bI[4];                      // '<S35>/Create Rotation Matrix'
} B_CreateRotationMatrix_Temp_i_T;

// Block signals for system '<S46>/MATLAB Function'
typedef struct {
  real_T Mat2[64];
  real_T H[24];                        // '<S46>/MATLAB Function'
  real_T b[24];
  real_T v[8];
} B_MATLABFunction_Template_v3__T;

// Block signals for system '<S109>/AHRS2'
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
  real_T AHRS2_o2[3];                  // '<S109>/AHRS2'
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
} B_AHRS2_Template_v3_08_2020a_T;

// Block states (default storage) for system '<S109>/AHRS2'
typedef struct {
  fusion_simulink_ahrsfilter_Te_T obj; // '<S109>/AHRS2'
  c_fusion_internal_frames_NED__T gobj_1;// '<S109>/AHRS2'
  c_fusion_internal_frames_NED__T gobj_2;// '<S109>/AHRS2'
  boolean_T objisempty;                // '<S109>/AHRS2'
} DW_AHRS2_Template_v3_08_2020a_T;

// Block signals for system '<S109>/ChangeOrientation'
typedef struct {
  real_T y[3];                         // '<S109>/ChangeOrientation'
} B_ChangeOrientation_Template__T;

// Block signals for system '<S119>/Calculate Running Mean'
typedef struct {
  real_T Mean;                         // '<S122>/Mean'
} B_CalculateRunningMean_Templa_T;

// Block states (default storage) for system '<S119>/Calculate Running Mean'
typedef struct {
  real_T Delay_DSTATE;                 // '<S122>/Delay'
  real_T Delay1_DSTATE[2];             // '<S122>/Delay1'
  real_T Delay2_DSTATE[4];             // '<S122>/Delay2'
  real_T Delay3_DSTATE[5];             // '<S122>/Delay3'
  real_T Delay4_DSTATE[3];             // '<S122>/Delay4'
  real_T Mean_AccVal;                  // '<S122>/Mean'
} DW_CalculateRunningMean_Templ_T;

// Block signals for system '<S134>/Calculate Running Mean'
typedef struct {
  real_T TmpSignalConversionAtMean_b[6];
  real_T Subtract;                     // '<S141>/Subtract'
} B_CalculateRunningMean_Temp_f_T;

// Block states (default storage) for system '<S134>/Calculate Running Mean'
typedef struct {
  real_T Delay_DSTATE;                 // '<S141>/Delay'
  real_T Delay1_DSTATE[2];             // '<S141>/Delay1'
  real_T Delay2_DSTATE[4];             // '<S141>/Delay2'
  real_T Delay3_DSTATE[5];             // '<S141>/Delay3'
  real_T Delay4_DSTATE[3];             // '<S141>/Delay4'
  real_T Mean_AccVal;                  // '<S141>/Mean'
} DW_CalculateRunningMean_Tem_o_T;

// Block signals for system '<S208>/Correct Dropouts'
typedef struct {
  real_T FixedInput;                   // '<S208>/Correct Dropouts'
} B_CorrectDropouts_Template_v3_T;

// Block signals for system '<S233>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S233>/MATLAB Function2'
  real_T Ox[2];                        // '<S233>/MATLAB Function2'
} B_MATLABFunction2_Template__n_T;

// Block signals for system '<S233>/MATLAB Function3'
typedef struct {
  real_T Ox[2];                        // '<S233>/MATLAB Function3'
} B_MATLABFunction3_Template_v3_T;

// Block signals for system '<S233>/MATLAB Function4'
typedef struct {
  real_T e_out;                        // '<S233>/MATLAB Function4'
} B_MATLABFunction4_Template_v3_T;

// Block signals for system '<S228>/RED+ARM'
typedef struct {
  real_T In1;                          // '<S279>/In1'
  real_T In1_n;                        // '<S277>/In1'
  real_T In1_l;                        // '<S275>/In1'
} B_REDARM_Template_v3_08_2020a_T;

// Block states (default storage) for system '<S228>/RED+ARM'
typedef struct {
  real_T Delay1_DSTATE;                // '<S274>/Delay1'
  real_T Delay1_DSTATE_l;              // '<S276>/Delay1'
  real_T Delay1_DSTATE_h;              // '<S278>/Delay1'
  uint8_T icLoad;                      // '<S274>/Delay1'
  uint8_T icLoad_c;                    // '<S276>/Delay1'
  uint8_T icLoad_j;                    // '<S278>/Delay1'
} DW_REDARM_Template_v3_08_2020_T;

// Block signals for system '<S295>/MATLAB Function'
typedef struct {
  real_T wrappedAngle;                 // '<S295>/MATLAB Function'
} B_MATLABFunction_Template_v_e_T;

// Block signals for system '<S293>/MATLAB Function2'
typedef struct {
  real_T Oy[2];                        // '<S293>/MATLAB Function2'
  real_T Ox[2];                        // '<S293>/MATLAB Function2'
} B_MATLABFunction2_Template__c_T;

// Block signals for system '<S280>/Sub-Phase #1'
typedef struct {
  real_T rtb_Subtract2_d_m[2];
  real_T In1;                          // '<S302>/In1'
  real_T In1_l;                        // '<S300>/In1'
  real_T In1_j;                        // '<S298>/In1'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4;// '<S293>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3;// '<S293>/MATLAB Function3' 
  B_MATLABFunction2_Template__c_T sf_MATLABFunction2;// '<S293>/MATLAB Function2' 
  B_MATLABFunction_Template_v_e_T sf_MATLABFunction;// '<S295>/MATLAB Function'
} B_SubPhase1_Template_v3_08_20_T;

// Block states (default storage) for system '<S280>/Sub-Phase #1'
typedef struct {
  real_T Delay1_DSTATE;                // '<S299>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S301>/Delay1'
  real_T Delay1_DSTATE_m;              // '<S297>/Delay1'
  uint8_T icLoad;                      // '<S299>/Delay1'
  uint8_T icLoad_p;                    // '<S301>/Delay1'
  uint8_T icLoad_pd;                   // '<S297>/Delay1'
} DW_SubPhase1_Template_v3_08_2_T;

// Block signals for system '<S281>/Sub-Phase #1'
typedef struct {
  real_T rtb_Subtract2_e2_m[2];
  real_T In1;                          // '<S375>/In1'
  real_T In1_j;                        // '<S373>/In1'
  real_T In1_m;                        // '<S371>/In1'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4;// '<S365>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3;// '<S365>/MATLAB Function3' 
  B_MATLABFunction2_Template__c_T sf_MATLABFunction2;// '<S365>/MATLAB Function2' 
} B_SubPhase1_Template_v3_08__g_T;

// Block states (default storage) for system '<S281>/Sub-Phase #1'
typedef struct {
  real_T Delay1_DSTATE;                // '<S372>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S374>/Delay1'
  real_T Delay1_DSTATE_o;              // '<S370>/Delay1'
  uint8_T icLoad;                      // '<S372>/Delay1'
  uint8_T icLoad_p;                    // '<S374>/Delay1'
  uint8_T icLoad_m;                    // '<S370>/Delay1'
} DW_SubPhase1_Template_v3_08_l_T;

// Block signals for system '<S434>/MATLAB Function'
typedef struct {
  real_T x;                            // '<S434>/MATLAB Function'
} B_MATLABFunction_Template_v_j_T;

// Block signals for system '<S434>/MATLAB Function1'
typedef struct {
  real_T y;                            // '<S434>/MATLAB Function1'
} B_MATLABFunction1_Template_v3_T;

// Block signals for system '<S435>/MATLAB Function'
typedef struct {
  real_T x;                            // '<S435>/MATLAB Function'
} B_MATLABFunction_Template_v_h_T;

// Block signals for system '<S437>/RED+ARM'
typedef struct {
  real_T TAU_out[6];                   // '<S544>/MATLAB Function'
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
} B_REDARM_Template_v3_08_202_m_T;

// Block signals for system '<S14>/Change BLUE Behavior'
typedef struct {
  real_T rtb_Subtract2_b1_m[2];
  real_T In1;                          // '<S578>/In1'
  real_T In1_l;                        // '<S576>/In1'
  real_T In1_p;                        // '<S574>/In1'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4;// '<S572>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3;// '<S572>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2;// '<S572>/MATLAB Function2' 
} B_ChangeBLUEBehavior_Templa_k_T;

// Block states (default storage) for system '<S14>/Change BLUE Behavior'
typedef struct {
  real_T Delay1_DSTATE;                // '<S575>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S577>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S573>/Delay1'
  uint8_T icLoad;                      // '<S575>/Delay1'
  uint8_T icLoad_m;                    // '<S577>/Delay1'
  uint8_T icLoad_p;                    // '<S573>/Delay1'
} DW_ChangeBLUEBehavior_Templ_i_T;

// Block signals (default storage)
typedef struct {
  real_T TmpSignalConversionAtToWork[73];
  real_T Mat2[64];
  real_T Mat2_m[64];
  real_T Mat2_c[64];
  real_T X[36];
  real_T V[36];
  real_T U[36];
  real_T InertiaS[36];                 // '<S695>/Inertia Function '
  real_T b_A[36];
  real_T Vf[36];
  real_T Product2_b[24];               // '<S73>/Product2'
  real_T Product2_a[24];               // '<S55>/Product2'
  real_T transpose_f[24];              // '<S43>/transpose'
  real_T Product2_j[24];               // '<S43>/Product2'
  real_T H_hz[24];                     // '<S34>/MATLAB Function'
  real_T b[24];
  real_T AccelerationtoVelocity_o[12];
  real_T b_k[9];
  real_T rtb_H_hz_c[9];
  real_T x[9];
  real_T rtb_Product_h_b[8];
  real_T rtb_transpose_f_p[8];
  real_T s[6];
  real_T dv[6];
  real_T dv1[6];
  real_T X_c[6];
  real_T b_s[6];
  real_T e[6];
  real_T work[6];
  char_T cv[45];
  real_T Sum_l[3];                     // '<S666>/Sum'
  real_T x_ddot_b[3];                  // '<S666>/MATLAB Function'
  real_T VelocitytoPosition[3];
  real_T LSM9DS1IMUSensor_o3[3];       // '<S6>/LSM9DS1 IMU Sensor'
  real_T Gain[3];                      // '<S111>/Gain'
  real_T DigitalFilter[3];             // '<S177>/Digital Filter'
  real_T Gain_c[3];                    // '<S110>/Gain'
  real_T DigitalFilter_i[3];           // '<S146>/Digital Filter'
  real_T Gain_o[3];                    // '<S109>/Gain'
  real_T DigitalFilter_a[3];           // '<S115>/Digital Filter'
  real_T rtb_Subtract2_f_f[2];
  real_T SetUniversalTimeIfthisisasimula;
                        // '<S18>/Set Universal Time (If this is a simulation)'
  real_T In1;                          // '<S646>/In1'
  real_T In1_e;                        // '<S644>/In1'
  real_T In1_i;                        // '<S642>/In1'
  real_T In1_p;                        // '<S618>/In1'
  real_T In1_it;                       // '<S616>/In1'
  real_T In1_j;                        // '<S614>/In1'
  real_T In1_a;                        // '<S593>/In1'
  real_T In1_c;                        // '<S591>/In1'
  real_T In1_o;                        // '<S589>/In1'
  real_T In1_o5;                       // '<S565>/In1'
  real_T In1_b;                        // '<S563>/In1'
  real_T In1_ei;                       // '<S561>/In1'
  real_T In1_m;                        // '<S539>/In1'
  real_T In1_d;                        // '<S537>/In1'
  real_T In1_op;                       // '<S535>/In1'
  real_T In1_m5;                       // '<S522>/In1'
  real_T In1_p4;                       // '<S520>/In1'
  real_T In1_f;                        // '<S518>/In1'
  real_T In1_oy;                       // '<S513>/In1'
  real_T In1_n;                        // '<S511>/In1'
  real_T In1_oz;                       // '<S489>/In1'
  real_T In1_mg;                       // '<S487>/In1'
  real_T In1_fl;                       // '<S485>/In1'
  real_T In1_ay;                       // '<S454>/In1'
  real_T In1_pe;                       // '<S452>/In1'
  real_T In1_k;                        // '<S450>/In1'
  real_T In1_jh;                       // '<S267>/In1'
  real_T In1_jv;                       // '<S265>/In1'
  real_T In1_i5;                       // '<S263>/In1'
  real_T In1_f1;                       // '<S252>/In1'
  real_T In1_l;                        // '<S250>/In1'
  real_T In1_fj;                       // '<S248>/In1'
  real_T In1_g;                        // '<S239>/In1'
  real_T In1_ej;                       // '<S237>/In1'
  real_T In1_ds;                       // '<S235>/In1'
  real_T In1_lk;                       // '<S715>/In1'
  real_T In1_pk;                       // '<S714>/In1'
  real_T In1_k1;                       // '<S713>/In1'
  real_T In1_ad;                       // '<S712>/In1'
  real_T In1_kj;                       // '<S711>/In1'
  real_T In1_h;                        // '<S702>/In1'
  real_T In1_lg;                       // '<S701>/In1'
  real_T In1_adc;                      // '<S689>/In1'
  real_T In1_fv;                       // '<S686>/In1'
  real_T In1_pr;                       // '<S685>/In1'
  real_T In1_es;                       // '<S677>/In1'
  real_T In1_id;                       // '<S676>/In1'
  real_T In1_kz;                       // '<S675>/In1'
  real_T In1_b2;                       // '<S674>/In1'
  real_T In1_aw;                       // '<S673>/In1'
  real_T In1_jd;                       // '<S204>/In1'
  real_T In1_pm;                       // '<S201>/In1'
  real_T In1_eo;                       // '<S198>/In1'
  real_T In1_ed;                       // '<S192>/In1'
  real_T In1_a4;                       // '<S188>/In1'
  real_T In1_dy;                       // '<S185>/In1'
  real_T In1_m0;                       // '<S173>/In1'
  real_T In1_np;                       // '<S170>/In1'
  real_T In1_pf;                       // '<S167>/In1'
  real_T In1_n3;                       // '<S161>/In1'
  real_T In1_opy;                      // '<S142>/In1'
  real_T In1_j5;                       // '<S139>/In1'
  real_T In1_dm;                       // '<S136>/In1'
  real_T In1_i4;                       // '<S130>/In1'
  real_T In1_ai;                       // '<S126>/In1'
  real_T In1_oj;                       // '<S123>/In1'
  real_T UDPReceive_o1[16];            // '<S78>/UDP Receive'
  real_T dividebydeltaT;               // '<S88>/divide by delta T'
  real_T dividebydeltaT_k;             // '<S89>/divide by delta T'
  real_T Unwrap1;                      // '<S78>/Unwrap1'
  real_T dividebydeltaT_d;             // '<S90>/divide by delta T'
  real_T dividebydeltaT_j;             // '<S79>/divide by delta T'
  real_T dividebydeltaT_d2;            // '<S86>/divide by delta T'
  real_T Unwrap;                       // '<S78>/Unwrap'
  real_T dividebydeltaT_l;             // '<S87>/divide by delta T'
  real_T Unwrap2;                      // '<S78>/Unwrap2'
  real_T In1_dn;                       // '<S108>/In1'
  real_T In1_db;                       // '<S107>/In1'
  real_T In1_fo;                       // '<S106>/In1'
  real_T In1_ex;                       // '<S105>/In1'
  real_T In1_k5;                       // '<S104>/In1'
  real_T In1_gw;                       // '<S103>/In1'
  real_T In1_lb;                       // '<S102>/In1'
  real_T In1_f0;                       // '<S101>/In1'
  real_T In1_lu;                       // '<S100>/In1'
  real_T In1_as;                       // '<S99>/In1'
  real_T In1_i0;                       // '<S98>/In1'
  real_T In1_ng;                       // '<S97>/In1'
  real_T In1_gp;                       // '<S96>/In1'
  real_T In1_cw;                       // '<S95>/In1'
  real_T In1_ky;                       // '<S94>/In1'
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
  real_T sampleTime;
  real_T uDLookupTable2;               // '<S508>/1-D Lookup Table2'
  real_T Sum6_i;                       // '<S519>/Sum6'
  real_T Sum6_au;                      // '<S517>/Sum6'
  real_T Sum6_d;                       // '<S617>/Sum6'
  real_T Sum6_l;                       // '<S615>/Sum6'
  real_T Subtract_o;                   // '<S605>/Subtract'
  real_T VelocitytoPosition_g;
  real_T rtb_x_ddot_i_g;
  real_T rtb_TmpSignalConversionAtSFun_m;
  real_T rtb_TmpSignalConversionAtSFu_nz;
  real_T rtb_TmpSignalConversionAtSFun_p;
  real_T rtb_TmpSignalConversionAtSFun_l;
  real_T rtb_TmpSignalConversionAtSFu_jw;
  real_T rtb_x_ddot_i_idx_0;
  real_T rtb_x_ddot_i_idx_1;
  real_T rtb_Sum_lh_idx_0;
  real_T rtb_Sum_lh_idx_1;
  real_T rtb_Sum_lh_idx_2;
  real_T rtb_x_ddot_idx_1;
  real_T rtb_Sum_ht_idx_1;
  real_T rtb_Sum_ht_idx_2;
  real_T u0;
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
  real_T sampleTime_d;
  real_T sampleTime_g;
  real_T d;
  real_T sr;
  real_T absx11;
  real_T absx21;
  real_T absx31;
  real_T scale_l;
  real_T absxk;
  real_T t;
  real_T scale_d;
  real_T absxk_d;
  real_T t_l;
  real_T temp;
  real_T temp_o;
  real_T si;
  int16_T b_RegisterValue[3];
  uint8_T output_raw[6];
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
  int32_T i_b;
  int32_T p1;
  int32_T p2;
  int32_T p3;
  int32_T itmp;
  int32_T kend;
  int32_T k;
  int32_T kend_n;
  int32_T k_b;
  int32_T ix;
  int32_T iy;
  int32_T k_l;
  int32_T ix_h;
  int32_T iy_b;
  int32_T k_d;
  int32_T ix_e;
  int32_T iy_bj;
  int32_T k_j;
  int32_T ix_f;
  int32_T iy_a;
  int32_T k_ju;
  int32_T ix_j;
  int32_T iy_o;
  int32_T k_n;
  int32_T ix_i;
  int32_T iy_oy;
  int32_T k_nv;
  int32_T u0_m;
  int32_T u1;
  int32_T k_c;
  uint32_T lo;
  uint32_T hi;
  int8_T rtAction;
  uint8_T SwappedDataBytes;
  uint8_T status;
  boolean_T p;
  boolean_T rtb_Compare_f_m;
  boolean_T rtb_Compare_n_m;
  boolean_T rtb_Compare_j;
  boolean_T apply_transform;
  B_REDARM_Template_v3_08_202_m_T REDARM_bo;// '<S607>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_f;// '<S638>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_fp;// '<S638>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_cg;// '<S638>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Templa_k_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_d;// '<S612>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_o;// '<S612>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_g;// '<S612>/MATLAB Function2' 
  B_REDARM_Template_v3_08_202_m_T REDARM_p;// '<S554>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_n;// '<S585>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_c1;// '<S585>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_i;// '<S585>/MATLAB Function2' 
  B_ChangeBLUEBehavior_Templa_k_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_l;// '<S559>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_b;// '<S559>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_pt;// '<S559>/MATLAB Function2' 
  B_MATLABFunction1_Template_v3_T sf_MATLABFunction1_e;// '<S437>/MATLAB Function1' 
  B_MATLABFunction_Template_v_j_T sf_MATLABFunction_m;// '<S437>/MATLAB Function' 
  B_REDARM_Template_v3_08_202_m_T REDARM_a;// '<S437>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_i;// '<S527>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_fd;// '<S527>/MATLAB Function3' 
  B_MATLABFunction2_Template__c_T sf_MATLABFunction2_p;// '<S527>/MATLAB Function2' 
  B_MATLABFunction1_Template_v3_T sf_MATLABFunction1_nv;// '<S436>/MATLAB Function1' 
  B_MATLABFunction_Template_v_h_T sf_MATLABFunction_or;// '<S436>/MATLAB Function' 
  B_MATLABFunction1_Template_v3_T sf_MATLABFunction1_n;// '<S435>/MATLAB Function1' 
  B_MATLABFunction_Template_v_h_T sf_MATLABFunction_g;// '<S435>/MATLAB Function' 
  B_REDARM_Template_v3_08_2020a_T REDARM_b;// '<S435>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_ju;// '<S475>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_p;// '<S475>/MATLAB Function3' 
  B_MATLABFunction2_Template__c_T sf_MATLABFunction2_d;// '<S475>/MATLAB Function2' 
  B_MATLABFunction_Template_v_e_T sf_MATLABFunction_p;// '<S482>/MATLAB Function' 
  B_MATLABFunction_Template_v_e_T sf_MATLABFunction_k;// '<S468>/MATLAB Function' 
  B_MATLABFunction1_Template_v3_T sf_MATLABFunction1_k;// '<S434>/MATLAB Function1' 
  B_MATLABFunction_Template_v_j_T sf_MATLABFunction_en;// '<S434>/MATLAB Function' 
  B_REDARM_Template_v3_08_2020a_T REDARM_c;// '<S434>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_k;// '<S442>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_f;// '<S442>/MATLAB Function3' 
  B_MATLABFunction2_Template__c_T sf_MATLABFunction2_c;// '<S442>/MATLAB Function2' 
  B_SubPhase1_Template_v3_08__g_T SubPhase4_f;// '<S281>/Sub-Phase #4'
  B_SubPhase1_Template_v3_08__g_T SubPhase3_p;// '<S281>/Sub-Phase #3 '
  B_SubPhase1_Template_v3_08__g_T SubPhase2_m;// '<S281>/Sub-Phase #2 '
  B_SubPhase1_Template_v3_08__g_T SubPhase1_a;// '<S281>/Sub-Phase #1'
  B_SubPhase1_Template_v3_08_20_T SubPhase4;// '<S280>/Sub-Phase #4'
  B_SubPhase1_Template_v3_08_20_T SubPhase3;// '<S280>/Sub-Phase #3 '
  B_SubPhase1_Template_v3_08_20_T SubPhase2;// '<S280>/Sub-Phase #2 '
  B_SubPhase1_Template_v3_08_20_T SubPhase1;// '<S280>/Sub-Phase #1'
  B_REDARM_Template_v3_08_2020a_T REDARM_j;// '<S228>/RED+ARM'
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_b;// '<S259>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_kt;// '<S259>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_ol;// '<S259>/MATLAB Function2' 
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_c;// '<S246>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_c;// '<S246>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_m;// '<S246>/MATLAB Function2' 
  B_MATLABFunction4_Template_v3_T sf_MATLABFunction4_j;// '<S233>/MATLAB Function4' 
  B_MATLABFunction3_Template_v3_T sf_MATLABFunction3_k;// '<S233>/MATLAB Function3' 
  B_MATLABFunction2_Template__n_T sf_MATLABFunction2_o;// '<S233>/MATLAB Function2' 
  B_CorrectDropouts_Template_v3_T sf_CorrectDropouts_h;// '<S210>/Correct Dropouts' 
  B_CorrectDropouts_Template_v3_T sf_CorrectDropouts_b;// '<S209>/Correct Dropouts' 
  B_CorrectDropouts_Template_v3_T sf_CorrectDropouts;// '<S208>/Correct Dropouts' 
  B_CalculateRunningMean_Temp_f_T CalculateRunningMean_ot;// '<S196>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_o2;// '<S195>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_f;// '<S194>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_hn;// '<S183>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_hq;// '<S182>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_hx;// '<S181>/Calculate Running Mean' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix_g;// '<S111>/Create Rotation Matrix' 
  B_ChangeOrientation_Template__T sf_ChangeOrientation_e;// '<S111>/ChangeOrientation' 
  B_AHRS2_Template_v3_08_2020a_T AHRS2_pn;// '<S109>/AHRS2'
  B_CalculateRunningMean_Temp_f_T CalculateRunningMean_o;// '<S165>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_h;// '<S164>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_n;// '<S163>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_l;// '<S152>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_b;// '<S151>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_d;// '<S150>/Calculate Running Mean' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix_p;// '<S110>/Create Rotation Matrix' 
  B_ChangeOrientation_Template__T sf_ChangeOrientation_l;// '<S110>/ChangeOrientation' 
  B_AHRS2_Template_v3_08_2020a_T AHRS2_p;// '<S109>/AHRS2'
  B_CalculateRunningMean_Temp_f_T CalculateRunningMean_ep;// '<S134>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_c;// '<S133>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_e;// '<S132>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_g;// '<S121>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean_i;// '<S120>/Calculate Running Mean' 
  B_CalculateRunningMean_Templa_T CalculateRunningMean;// '<S119>/Calculate Running Mean' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix_iq;// '<S109>/Create Rotation Matrix' 
  B_ChangeOrientation_Template__T sf_ChangeOrientation;// '<S109>/ChangeOrientation' 
  B_AHRS2_Template_v3_08_2020a_T AHRS2;// '<S109>/AHRS2'
  B_CreateRotationMatrix_Temp_i_T sf_CreateRotationMatrix_n;// '<S59>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix_e;// '<S71>/Create Rotation Matrix' 
  B_MATLABFunction10_Template_v_T sf_MATLABFunction5;// '<S58>/MATLAB Function5' 
  B_MATLABFunction2_Template_v3_T sf_MATLABFunction2_l;// '<S58>/MATLAB Function2' 
  B_CreateRotationMatrix_Temp_i_T sf_CreateRotationMatrix_f;// '<S47>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix_i;// '<S53>/Create Rotation Matrix' 
  B_MATLABFunction10_Template_v_T sf_MATLABFunction6;// '<S46>/MATLAB Function6' 
  B_MATLABFunction2_Template_v3_T sf_MATLABFunction2_n;// '<S46>/MATLAB Function2' 
  B_MATLABFunction_Template_v3__T sf_MATLABFunction1_b;// '<S46>/MATLAB Function1' 
  B_MATLABFunction_Template_v3__T sf_MATLABFunction_i;// '<S46>/MATLAB Function' 
  B_CreateRotationMatrix_Temp_i_T sf_CreateRotationMatrix_h;// '<S35>/Create Rotation Matrix' 
  B_CreateRotationMatrix_Templa_T sf_CreateRotationMatrix;// '<S41>/Create Rotation Matrix' 
  B_MATLABFunction2_Template_v3_T sf_MATLABFunction2;// '<S34>/MATLAB Function2' 
  B_MATLABFunction10_Template_v_T sf_MATLABFunction10;// '<S34>/MATLAB Function10' 
} B_Template_v3_08_2020a_T;

// Block states (default storage) for system '<Root>'
typedef struct {
  raspi_internal_lsm9ds1Block_T_T obj; // '<S6>/LSM9DS1 IMU Sensor'
  ReadArm_Position_Rates_Templa_T obj_c;
                     // '<S206>/Read Joint Positions using  Dynamixel Encoders'
  IdentifyPlatform_Template_v3__T obj_b;// '<S17>/WhoAmI'
  raspi_internal_PWMBlock_Templ_T obj_m;// '<S33>/RED T1 - BLACK T8'
  raspi_internal_PWMBlock_Templ_T obj_ma;// '<S33>/RED T2 - BLACK T3'
  raspi_internal_PWMBlock_Templ_T obj_h;// '<S33>/RED T3'
  raspi_internal_PWMBlock_Templ_T obj_k;// '<S33>/RED T4 - BLACK T5'
  raspi_internal_PWMBlock_Templ_T obj_hw;// '<S33>/RED T5 - BLACK T4'
  raspi_internal_PWMBlock_Templ_T obj_hf;// '<S33>/RED T6 - BLACK T7'
  raspi_internal_PWMBlock_Templ_T obj_ce;// '<S33>/RED T7 - BLACK T6'
  raspi_internal_PWMBlock_Templ_T obj_f;// '<S33>/RED T8 - BLACK T1'
  raspi_internal_PWMBlock_Templ_T obj_ci;// '<S33>/BLACK T2'
  codertarget_linux_blocks_Digi_T obj_a;// '<S24>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_i;// '<S23>/Digital Write'
  InitializeForTorque_Template__T obj_g;
                            // '<S216>/Initialization Block for Torque Control'
  real_T Delay1_DSTATE;                // '<S641>/Delay1'
  real_T Delay1_DSTATE_d;              // '<S643>/Delay1'
  real_T Delay1_DSTATE_c;              // '<S645>/Delay1'
  real_T Delay1_DSTATE_j;              // '<S615>/Delay1'
  real_T Delay1_DSTATE_f;              // '<S617>/Delay1'
  real_T Delay1_DSTATE_ci;             // '<S613>/Delay1'
  real_T Delay1_DSTATE_i;              // '<S588>/Delay1'
  real_T Delay1_DSTATE_i3;             // '<S590>/Delay1'
  real_T Delay1_DSTATE_cl;             // '<S592>/Delay1'
  real_T Delay1_DSTATE_db;             // '<S562>/Delay1'
  real_T Delay1_DSTATE_dz;             // '<S564>/Delay1'
  real_T Delay1_DSTATE_h;              // '<S560>/Delay1'
  real_T Delay1_DSTATE_cn;             // '<S534>/Delay1'
  real_T Delay1_DSTATE_l;              // '<S536>/Delay1'
  real_T Delay1_DSTATE_lw;             // '<S538>/Delay1'
  real_T Delay1_DSTATE_k;              // '<S510>/Delay1'
  real_T Delay1_DSTATE_b;              // '<S512>/Delay1'
  real_T Delay1_DSTATE_a;              // '<S517>/Delay1'
  real_T Delay1_DSTATE_g;              // '<S519>/Delay1'
  real_T Delay1_DSTATE_aw;             // '<S521>/Delay1'
  real_T Delay1_DSTATE_io;             // '<S484>/Delay1'
  real_T Delay1_DSTATE_l5;             // '<S486>/Delay1'
  real_T Delay1_DSTATE_ji;             // '<S488>/Delay1'
  real_T Delay1_DSTATE_g3;             // '<S449>/Delay1'
  real_T Delay1_DSTATE_dv;             // '<S451>/Delay1'
  real_T Delay1_DSTATE_o;              // '<S453>/Delay1'
  real_T Delay1_DSTATE_bs;             // '<S262>/Delay1'
  real_T Delay1_DSTATE_cb;             // '<S264>/Delay1'
  real_T Delay1_DSTATE_ig;             // '<S266>/Delay1'
  real_T Delay1_DSTATE_fb;             // '<S249>/Delay1'
  real_T Delay1_DSTATE_la;             // '<S251>/Delay1'
  real_T Delay1_DSTATE_n;              // '<S247>/Delay1'
  real_T Delay1_DSTATE_km;             // '<S236>/Delay1'
  real_T Delay1_DSTATE_o1;             // '<S238>/Delay1'
  real_T Delay1_DSTATE_lg;             // '<S234>/Delay1'
  real_T Delay1_DSTATE_jp;             // '<S208>/Delay1'
  real_T Delay2_DSTATE[2];             // '<S208>/Delay2'
  real_T Delay5_DSTATE[3];             // '<S208>/Delay5'
  real_T Delay3_DSTATE[4];             // '<S208>/Delay3'
  real_T Delay4_DSTATE[5];             // '<S208>/Delay4'
  real_T Delay1_DSTATE_n5;             // '<S209>/Delay1'
  real_T Delay2_DSTATE_l[2];           // '<S209>/Delay2'
  real_T Delay5_DSTATE_b[3];           // '<S209>/Delay5'
  real_T Delay3_DSTATE_o[4];           // '<S209>/Delay3'
  real_T Delay4_DSTATE_c[5];           // '<S209>/Delay4'
  real_T Delay1_DSTATE_ny;             // '<S210>/Delay1'
  real_T Delay2_DSTATE_m[2];           // '<S210>/Delay2'
  real_T Delay5_DSTATE_n[3];           // '<S210>/Delay5'
  real_T Delay3_DSTATE_h[4];           // '<S210>/Delay3'
  real_T Delay4_DSTATE_k[5];           // '<S210>/Delay4'
  real_T AccelerationtoVelocity_DSTATE[3];// '<S666>/Acceleration  to Velocity'
  real_T VelocitytoPosition_DSTATE[3]; // '<S666>/Velocity to Position'
  real_T Delay1_DSTATE_p;              // '<S671>/Delay1'
  real_T Delay1_DSTATE_ja;             // '<S669>/Delay1'
  real_T Delay1_DSTATE_d4;             // '<S667>/Delay1'
  real_T Delay1_DSTATE_j3;             // '<S670>/Delay1'
  real_T Delay1_DSTATE_kmw;            // '<S668>/Delay1'
  real_T AccelerationtoVelocity_DSTATE_b[3];// '<S678>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_a[3];// '<S678>/Velocity to Position'
  real_T Delay1_DSTATE_e;              // '<S683>/Delay1'
  real_T Delay1_DSTATE_fi;             // '<S681>/Delay1'
  real_T Delay1_DSTATE_bo;             // '<S679>/Delay1'
  real_T Delay1_DSTATE_fq;             // '<S682>/Delay1'
  real_T Delay1_DSTATE_od;             // '<S680>/Delay1'
  real_T DiscreteTimeIntegrator_DSTATE;// '<S664>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator1_DSTATE;// '<S664>/Discrete-Time Integrator1'
  real_T DiscreteTimeIntegrator2_DSTATE;// '<S664>/Discrete-Time Integrator2'
  real_T AccelerationtoVelocity_DSTATE_m[3];// '<S704>/Acceleration  to Velocity' 
  real_T VelocitytoPosition_DSTATE_f[3];// '<S704>/Velocity to Position'
  real_T Delay1_DSTATE_gy;             // '<S706>/Delay1'
  real_T Delay1_DSTATE_m;              // '<S708>/Delay1'
  real_T Delay1_DSTATE_et;             // '<S705>/Delay1'
  real_T Delay1_DSTATE_ln;             // '<S709>/Delay1'
  real_T Delay1_DSTATE_hs;             // '<S707>/Delay1'
  real_T Delay_DSTATE[12];             // '<S695>/Delay'
  real_T AccelerationtoVelocity_DSTATE_o[12];// '<S695>/Acceleration  to Velocity' 
  real_T Delay1_DSTATE_po;             // '<S700>/Delay1'
  real_T Delay1_DSTATE_kx;             // '<S699>/Delay1'
  real_T DigitalFilter_FILT_STATES[6]; // '<S177>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_e;// '<S183>/Discrete-Time Integrator1'
  real_T DigitalFilter_FILT_STATES_p[6];// '<S146>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_j;// '<S152>/Discrete-Time Integrator1'
  real_T DigitalFilter_FILT_STATES_h[6];// '<S115>/Digital Filter'
  real_T DiscreteTimeIntegrator1_DSTAT_b;// '<S121>/Discrete-Time Integrator1'
  real_T Delay1_DSTATE_hi;             // '<S88>/Delay1'
  real_T Delay1_DSTATE_df;             // '<S80>/Delay1'
  real_T Delay1_DSTATE_ft;             // '<S89>/Delay1'
  real_T Delay1_DSTATE_hz;             // '<S81>/Delay1'
  real_T Delay1_DSTATE_mt;             // '<S90>/Delay1'
  real_T Delay1_DSTATE_lf;             // '<S82>/Delay1'
  real_T Delay1_DSTATE_cs;             // '<S83>/Delay1'
  real_T Delay1_DSTATE_kv;             // '<S79>/Delay1'
  real_T Delay1_DSTATE_lc;             // '<S91>/Delay1'
  real_T Delay1_DSTATE_cd;             // '<S86>/Delay1'
  real_T Delay1_DSTATE_i0;             // '<S92>/Delay1'
  real_T Delay1_DSTATE_jx;             // '<S87>/Delay1'
  real_T Delay1_DSTATE_ml;             // '<S93>/Delay1'
  real_T Delay1_DSTATE_ic;             // '<S84>/Delay1'
  real_T Delay1_DSTATE_a2;             // '<S85>/Delay1'
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
  real_T RED_Tz_RW_Sat;                // '<Root>/RED_Tz_RW Sat'
  real_T ARM_EndEff_Py;                // '<Root>/RED_Tz_RW Sat1'
  real_T ARM_EndEff_Px;                // '<Root>/RED_Tz_RW1'
  real_T RED_Tz_Sat;                   // '<Root>/RED_Tz_Sat'
  real_T ARM_Elbow_Px;                 // '<Root>/RED_Tz_Sat1'
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
  real_T NextOutput;                   // '<S666>/Random Number'
  real_T NextOutput_p;                 // '<S662>/Random Number1'
  real_T NextOutput_o;                 // '<S662>/Random Number'
  real_T NextOutput_j;                 // '<S662>/Random Number2'
  real_T NextOutput_g;                 // '<S678>/Random Number'
  real_T NextOutput_k;                 // '<S663>/Random Number5'
  real_T NextOutput_lo;                // '<S704>/Random Number'
  real_T NextOutput_de;                // '<S703>/Random Number7'
  real_T NextOutput_c;                 // '<S703>/Random Number6'
  real_T NextOutput_h;                 // '<S703>/Random Number8'
  real_T NextOutput_c2;                // '<S695>/Random Number2'
  real_T NextOutput_lz;                // '<S695>/Random Number1'
  real_T NextOutput_dv;                // '<S695>/Random Number3'
  real_T UDPReceive_NetworkLib[137];   // '<S78>/UDP Receive'
  real_T Unwrap1_Prev;                 // '<S78>/Unwrap1'
  real_T Unwrap1_Cumsum;               // '<S78>/Unwrap1'
  real_T Unwrap_Prev;                  // '<S78>/Unwrap'
  real_T Unwrap_Cumsum;                // '<S78>/Unwrap'
  real_T Unwrap2_Prev;                 // '<S78>/Unwrap2'
  real_T Unwrap2_Cumsum;               // '<S78>/Unwrap2'
  real_T Product1_DWORK4[9];           // '<S72>/Product1'
  real_T Product1_DWORK4_j[9];         // '<S73>/Product1'
  real_T Product1_DWORK4_g[9];         // '<S54>/Product1'
  real_T Product1_DWORK4_b[9];         // '<S55>/Product1'
  real_T Product1_DWORK4_gx[9];        // '<S42>/Product1'
  real_T Product1_DWORK4_c[9];         // '<S43>/Product1'
  struct {
    void *LoggedData;
  } ToWorkspace_PWORK;                 // '<S1>/To Workspace'

  uint32_T RandSeed;                   // '<S666>/Random Number'
  uint32_T RandSeed_m;                 // '<S662>/Random Number1'
  uint32_T RandSeed_d;                 // '<S662>/Random Number'
  uint32_T RandSeed_k;                 // '<S662>/Random Number2'
  uint32_T RandSeed_d3;                // '<S678>/Random Number'
  uint32_T RandSeed_dc;                // '<S663>/Random Number4'
  uint32_T RandSeed_j;                 // '<S663>/Random Number3'
  uint32_T RandSeed_c;                 // '<S663>/Random Number5'
  uint32_T RandSeed_mt;                // '<S704>/Random Number'
  uint32_T RandSeed_ka;                // '<S703>/Random Number7'
  uint32_T RandSeed_g;                 // '<S703>/Random Number6'
  uint32_T RandSeed_f;                 // '<S703>/Random Number8'
  uint32_T RandSeed_o;                 // '<S695>/Random Number2'
  uint32_T RandSeed_p;                 // '<S695>/Random Number1'
  uint32_T RandSeed_h;                 // '<S695>/Random Number3'
  MoveArm_Torque_Template_v3_08_T obj_j;
                                 // '<S27>/Send Torque Commands to Manipulator'
  int8_T Ifperforminganexperimentgrabthe;
  // '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
  int8_T DiscreteTimeIntegrator_PrevRese;// '<S664>/Discrete-Time Integrator'
  int8_T DiscreteTimeIntegrator1_PrevRes;// '<S664>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator2_PrevRes;// '<S664>/Discrete-Time Integrator2'
  int8_T DiscreteTimeIntegrator1_PrevR_h;// '<S183>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator1_PrevR_f;// '<S152>/Discrete-Time Integrator1'
  int8_T DiscreteTimeIntegrator1_Prev_hi;// '<S121>/Discrete-Time Integrator1'
  uint8_T icLoad;                      // '<S641>/Delay1'
  uint8_T icLoad_l;                    // '<S643>/Delay1'
  uint8_T icLoad_j;                    // '<S645>/Delay1'
  uint8_T icLoad_d;                    // '<S615>/Delay1'
  uint8_T icLoad_f;                    // '<S617>/Delay1'
  uint8_T icLoad_b;                    // '<S613>/Delay1'
  uint8_T icLoad_k;                    // '<S588>/Delay1'
  uint8_T icLoad_du;                   // '<S590>/Delay1'
  uint8_T icLoad_g;                    // '<S592>/Delay1'
  uint8_T icLoad_e;                    // '<S562>/Delay1'
  uint8_T icLoad_fr;                   // '<S564>/Delay1'
  uint8_T icLoad_o;                    // '<S560>/Delay1'
  uint8_T icLoad_c;                    // '<S534>/Delay1'
  uint8_T icLoad_i;                    // '<S536>/Delay1'
  uint8_T icLoad_cg;                   // '<S538>/Delay1'
  uint8_T icLoad_ca;                   // '<S510>/Delay1'
  uint8_T icLoad_n;                    // '<S512>/Delay1'
  uint8_T icLoad_im;                   // '<S517>/Delay1'
  uint8_T icLoad_eb;                   // '<S519>/Delay1'
  uint8_T icLoad_jf;                   // '<S521>/Delay1'
  uint8_T icLoad_jr;                   // '<S484>/Delay1'
  uint8_T icLoad_cw;                   // '<S486>/Delay1'
  uint8_T icLoad_eu;                   // '<S488>/Delay1'
  uint8_T icLoad_nw;                   // '<S449>/Delay1'
  uint8_T icLoad_gb;                   // '<S451>/Delay1'
  uint8_T icLoad_nx;                   // '<S453>/Delay1'
  uint8_T icLoad_jx;                   // '<S262>/Delay1'
  uint8_T icLoad_l0;                   // '<S264>/Delay1'
  uint8_T icLoad_iu;                   // '<S266>/Delay1'
  uint8_T icLoad_dy;                   // '<S249>/Delay1'
  uint8_T icLoad_n5;                   // '<S251>/Delay1'
  uint8_T icLoad_bp;                   // '<S247>/Delay1'
  uint8_T icLoad_ns;                   // '<S236>/Delay1'
  uint8_T icLoad_a;                    // '<S238>/Delay1'
  uint8_T icLoad_h;                    // '<S234>/Delay1'
  uint8_T icLoad_bb;                   // '<S671>/Delay1'
  uint8_T icLoad_p;                    // '<S669>/Delay1'
  uint8_T icLoad_lv;                   // '<S667>/Delay1'
  uint8_T icLoad_hu;                   // '<S670>/Delay1'
  uint8_T icLoad_m;                    // '<S668>/Delay1'
  uint8_T icLoad_dc;                   // '<S683>/Delay1'
  uint8_T icLoad_ln;                   // '<S681>/Delay1'
  uint8_T icLoad_oc;                   // '<S679>/Delay1'
  uint8_T icLoad_l1;                   // '<S682>/Delay1'
  uint8_T icLoad_lz;                   // '<S680>/Delay1'
  uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S664>/Discrete-Time Integrator'
  uint8_T DiscreteTimeIntegrator1_IC_LOAD;// '<S664>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator2_IC_LOAD;// '<S664>/Discrete-Time Integrator2'
  uint8_T icLoad_ec;                   // '<S706>/Delay1'
  uint8_T icLoad_by;                   // '<S708>/Delay1'
  uint8_T icLoad_oe;                   // '<S705>/Delay1'
  uint8_T icLoad_mq;                   // '<S709>/Delay1'
  uint8_T icLoad_ov;                   // '<S707>/Delay1'
  uint8_T icLoad_nt;                   // '<S700>/Delay1'
  uint8_T icLoad_el;                   // '<S699>/Delay1'
  uint8_T DiscreteTimeIntegrator1_IC_LO_g;// '<S183>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator1_IC_L_gl;// '<S152>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator1_IC_LO_c;// '<S121>/Discrete-Time Integrator1'
  uint8_T icLoad_ct;                   // '<S88>/Delay1'
  uint8_T icLoad_h0;                   // '<S80>/Delay1'
  uint8_T icLoad_kt;                   // '<S89>/Delay1'
  uint8_T icLoad_ktm;                  // '<S81>/Delay1'
  uint8_T icLoad_gn;                   // '<S90>/Delay1'
  uint8_T icLoad_pz;                   // '<S82>/Delay1'
  uint8_T icLoad_fy;                   // '<S83>/Delay1'
  uint8_T icLoad_ea;                   // '<S79>/Delay1'
  uint8_T icLoad_df;                   // '<S91>/Delay1'
  uint8_T icLoad_pj;                   // '<S86>/Delay1'
  uint8_T icLoad_ap;                   // '<S92>/Delay1'
  uint8_T icLoad_hr;                   // '<S87>/Delay1'
  uint8_T icLoad_al;                   // '<S93>/Delay1'
  uint8_T icLoad_dt;                   // '<S84>/Delay1'
  uint8_T icLoad_l1b;                  // '<S85>/Delay1'
  boolean_T Unwrap1_FirstStep;         // '<S78>/Unwrap1'
  boolean_T Unwrap_FirstStep;          // '<S78>/Unwrap'
  boolean_T Unwrap2_FirstStep;         // '<S78>/Unwrap2'
  DW_ChangeBLUEBehavior_Templ_i_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  DW_ChangeBLUEBehavior_Templ_i_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  DW_REDARM_Template_v3_08_2020_T REDARM_b;// '<S435>/RED+ARM'
  DW_REDARM_Template_v3_08_2020_T REDARM_c;// '<S434>/RED+ARM'
  DW_SubPhase1_Template_v3_08_l_T SubPhase4_f;// '<S281>/Sub-Phase #4'
  DW_SubPhase1_Template_v3_08_l_T SubPhase3_p;// '<S281>/Sub-Phase #3 '
  DW_SubPhase1_Template_v3_08_l_T SubPhase2_m;// '<S281>/Sub-Phase #2 '
  DW_SubPhase1_Template_v3_08_l_T SubPhase1_a;// '<S281>/Sub-Phase #1'
  DW_SubPhase1_Template_v3_08_2_T SubPhase4;// '<S280>/Sub-Phase #4'
  DW_SubPhase1_Template_v3_08_2_T SubPhase3;// '<S280>/Sub-Phase #3 '
  DW_SubPhase1_Template_v3_08_2_T SubPhase2;// '<S280>/Sub-Phase #2 '
  DW_SubPhase1_Template_v3_08_2_T SubPhase1;// '<S280>/Sub-Phase #1'
  DW_REDARM_Template_v3_08_2020_T REDARM_j;// '<S228>/RED+ARM'
  DW_CalculateRunningMean_Tem_o_T CalculateRunningMean_ot;// '<S196>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_o2;// '<S195>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_f;// '<S194>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_hn;// '<S183>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_hq;// '<S182>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_hx;// '<S181>/Calculate Running Mean' 
  DW_AHRS2_Template_v3_08_2020a_T AHRS2_pn;// '<S109>/AHRS2'
  DW_CalculateRunningMean_Tem_o_T CalculateRunningMean_o;// '<S165>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_h;// '<S164>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_n;// '<S163>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_l;// '<S152>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_b;// '<S151>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_d;// '<S150>/Calculate Running Mean' 
  DW_AHRS2_Template_v3_08_2020a_T AHRS2_p;// '<S109>/AHRS2'
  DW_CalculateRunningMean_Tem_o_T CalculateRunningMean_ep;// '<S134>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_c;// '<S133>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_e;// '<S132>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_g;// '<S121>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean_i;// '<S120>/Calculate Running Mean' 
  DW_CalculateRunningMean_Templ_T CalculateRunningMean;// '<S119>/Calculate Running Mean' 
  DW_AHRS2_Template_v3_08_2020a_T AHRS2;// '<S109>/AHRS2'
} DW_Template_v3_08_2020a_T;

// Parameters for system: '<S109>/AHRS2'
struct P_AHRS2_Template_v3_08_2020a_T_ {
  real_T AHRS2_AccelerometerNoise;     // Expression: 0.0001924722
                                          //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_GyroscopeNoise;         // Expression: 9.1385e-5
                                          //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_MagnetometerNoise;      // Expression: 0.1
                                          //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_GyroscopeDriftNoise;    // Expression: 3.0462e-13
                                          //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_LinearAccelerationNoise;// Expression: 0.0096236100000000012
                                          //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_MagneticDisturbanceNoise;// Expression: 0.5
                                           //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_LinearAccelerationDecayFa;// Expression: 0.5
                                            //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_MagneticDisturbanceDecayF;// Expression: 0.5
                                            //  Referenced by: '<S109>/AHRS2'

  real_T AHRS2_ExpectedMagneticFieldStre;// Expression: 33
                                            //  Referenced by: '<S109>/AHRS2'

};

// Parameters for system: '<S119>/Calculate Running Mean'
struct P_CalculateRunningMean_Templa_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S122>/Out1'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S122>/Delay'

  real_T Delay1_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S122>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S122>/Delay2'

  real_T Delay3_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S122>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S122>/Delay4'

};

// Parameters for system: '<S134>/Calculate Running Mean'
struct P_CalculateRunningMean_Temp_k_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S141>/Out1'

  real_T Constant_Value;               // Expression: 9.81
                                          //  Referenced by: '<S141>/Constant'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S141>/Delay'

  real_T Delay1_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S141>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S141>/Delay2'

  real_T Delay3_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S141>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0.0
                                          //  Referenced by: '<S141>/Delay4'

};

// Parameters for system: '<S10>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Template_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S219>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S219>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S219>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S219>/Puck State'

};

// Parameters for system: '<Root>/Phase #0:  Wait for Synchronization'
struct P_Phase0WaitforSynchronizatio_T_ {
  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S218>/Constant3'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S218>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S218>/Constant5'

  real_T PuckState_Value;              // Expression: 0
                                          //  Referenced by: '<S218>/Puck State'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S220>/Constant'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S220>/Constant1'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S220>/Constant2'

  real_T Constant3_Value_h;            // Expression: 0
                                          //  Referenced by: '<S220>/Constant3'

  real_T Constant4_Value_g;            // Expression: 0
                                          //  Referenced by: '<S220>/Constant4'

  real_T Constant5_Value_e;            // Expression: 0
                                          //  Referenced by: '<S220>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S220>/Constant6'

  real_T PuckState_Value_b;            // Expression: 0
                                          //  Referenced by: '<S220>/Puck State'

  P_ChangeBLUEBehavior_Template_T ChangeBLUEBehavior;// '<S10>/Change BLUE Behavior' 
};

// Parameters for system: '<S228>/RED+ARM'
struct P_REDARM_Template_v3_08_2020a_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S275>/Out1'

  real_T Out1_Y0_k;                    // Computed Parameter: Out1_Y0_k
                                          //  Referenced by: '<S277>/Out1'

  real_T Out1_Y0_h;                    // Computed Parameter: Out1_Y0_h
                                          //  Referenced by: '<S279>/Out1'

  real_T Constant9_Value;              // Expression: pi/2
                                          //  Referenced by: '<S261>/Constant9'

  real_T Constant8_Value;              // Expression: pi/2
                                          //  Referenced by: '<S261>/Constant8'

  real_T Constant7_Value;              // Expression: 0
                                          //  Referenced by: '<S261>/Constant7'

};

// Parameters for system: '<S280>/Sub-Phase #1'
struct P_SubPhase1_Template_v3_08_20_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S298>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S300>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S302>/Out1'

  real_T DesiredRateBLACK_Value;       // Expression: 0.03490659
                                          //  Referenced by: '<S289>/Desired Rate (BLACK)'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S284>/Puck State'

};

// Parameters for system: '<S281>/Sub-Phase #1'
struct P_SubPhase1_Template_v3_08__k_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S371>/Out1'

  real_T Out1_Y0_g;                    // Computed Parameter: Out1_Y0_g
                                          //  Referenced by: '<S373>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S375>/Out1'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S368>/Constant'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S357>/Puck State'

};

// Parameters for system: '<S437>/RED+ARM'
struct P_REDARM_Template_v3_08_202_a_T_ {
  real_T Constant2_Value;              // Expression: 0.9
                                          //  Referenced by: '<S531>/Constant2'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S531>/Constant'

};

// Parameters for system: '<S14>/Change BLUE Behavior'
struct P_ChangeBLUEBehavior_Templa_c_T_ {
  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S574>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S576>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S578>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S553>/Puck State'

};

// Parameters (default storage)
struct P_Template_v3_08_2020a_T_ {
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
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Gamma2;                       // Variable: Gamma2
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Gamma3;                       // Variable: Gamma3
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Gamma4;                       // Variable: Gamma4
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Gamma5;                       // Variable: Gamma5
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Gamma6;                       // Variable: Gamma6
                                          //  Referenced by: '<S695>/Dynamics Model'

  real_T Kd_elarm;                     // Variable: Kd_elarm
                                          //  Referenced by:
                                          //    '<S271>/kd_elarm'
                                          //    '<S458>/kd_elarm'
                                          //    '<S493>/kd_elarm'
                                          //    '<S514>/kd_elarm'

  real_T Kd_sharm;                     // Variable: Kd_sharm
                                          //  Referenced by:
                                          //    '<S272>/kd_sharm'
                                          //    '<S459>/kd_sharm'
                                          //    '<S494>/kd_sharm'
                                          //    '<S515>/kd_sharm'

  real_T Kd_tb;                        // Variable: Kd_tb
                                          //  Referenced by:
                                          //    '<S230>/kd_tb'
                                          //    '<S556>/kd_tb'
                                          //    '<S609>/kd_tb'
                                          //    '<S290>/kd_tb'
                                          //    '<S307>/kd_tb'
                                          //    '<S324>/kd_tb'
                                          //    '<S341>/kd_tb'

  real_T Kd_tblue;                     // Variable: Kd_tblue
                                          //  Referenced by:
                                          //    '<S243>/kd_tb'
                                          //    '<S569>/kd_tb'
                                          //    '<S622>/kd_tb'
                                          //    '<S362>/kd_tb'
                                          //    '<S380>/kd_tb'
                                          //    '<S398>/kd_tb'
                                          //    '<S416>/kd_tb'

  real_T Kd_tr;                        // Variable: Kd_tr
                                          //  Referenced by:
                                          //    '<S256>/kd_tr'
                                          //    '<S582>/kd_tr'
                                          //    '<S635>/kd_tr'
                                          //    '<S439>/kd_tr'
                                          //    '<S472>/kd_tr'
                                          //    '<S524>/kd_tr'

  real_T Kd_wrarm;                     // Variable: Kd_wrarm
                                          //  Referenced by:
                                          //    '<S273>/kd_wrarm'
                                          //    '<S460>/kd_wrarm'
                                          //    '<S495>/kd_wrarm'
                                          //    '<S516>/kd_wrarm'

  real_T Kd_xb;                        // Variable: Kd_xb
                                          //  Referenced by:
                                          //    '<S231>/kd_xb'
                                          //    '<S557>/kd_xb'
                                          //    '<S610>/kd_xb'
                                          //    '<S291>/kd_xb'
                                          //    '<S308>/kd_xb'
                                          //    '<S325>/kd_xb'
                                          //    '<S342>/kd_xb'

  real_T Kd_xblue;                     // Variable: Kd_xblue
                                          //  Referenced by:
                                          //    '<S244>/kd_xb'
                                          //    '<S570>/kd_xb'
                                          //    '<S623>/kd_xb'
                                          //    '<S363>/kd_xb'
                                          //    '<S381>/kd_xb'
                                          //    '<S399>/kd_xb'
                                          //    '<S417>/kd_xb'

  real_T Kd_xr;                        // Variable: Kd_xr
                                          //  Referenced by:
                                          //    '<S257>/kd_xr'
                                          //    '<S583>/kd_xr'
                                          //    '<S636>/kd_xr'
                                          //    '<S440>/kd_xr'
                                          //    '<S473>/kd_xr'
                                          //    '<S503>/kd_xr'
                                          //    '<S525>/kd_xr'

  real_T Kd_yb;                        // Variable: Kd_yb
                                          //  Referenced by:
                                          //    '<S232>/kd_yb'
                                          //    '<S558>/kd_yb'
                                          //    '<S611>/kd_yb'
                                          //    '<S292>/kd_yb'
                                          //    '<S309>/kd_yb'
                                          //    '<S326>/kd_yb'
                                          //    '<S343>/kd_yb'

  real_T Kd_yblue;                     // Variable: Kd_yblue
                                          //  Referenced by:
                                          //    '<S245>/kd_yb'
                                          //    '<S571>/kd_yb'
                                          //    '<S624>/kd_yb'
                                          //    '<S364>/kd_yb'
                                          //    '<S382>/kd_yb'
                                          //    '<S400>/kd_yb'
                                          //    '<S418>/kd_yb'

  real_T Kd_yr;                        // Variable: Kd_yr
                                          //  Referenced by:
                                          //    '<S258>/kd_yr'
                                          //    '<S584>/kd_yr'
                                          //    '<S637>/kd_yr'
                                          //    '<S441>/kd_yr'
                                          //    '<S474>/kd_yr'
                                          //    '<S504>/kd_yr'
                                          //    '<S526>/kd_yr'

  real_T Kp[9];                        // Variable: Kp
                                          //  Referenced by:
                                          //    '<S598>/MATLAB Function'
                                          //    '<S651>/MATLAB Function'
                                          //    '<S544>/MATLAB Function'

  real_T Kp_elarm;                     // Variable: Kp_elarm
                                          //  Referenced by:
                                          //    '<S271>/kp_elarm'
                                          //    '<S458>/kp_elarm'
                                          //    '<S493>/kp_elarm'
                                          //    '<S514>/kp_elarm'

  real_T Kp_sharm;                     // Variable: Kp_sharm
                                          //  Referenced by:
                                          //    '<S272>/kp_sharm'
                                          //    '<S459>/kp_sharm'
                                          //    '<S494>/kp_sharm'
                                          //    '<S515>/kp_sharm'

  real_T Kp_tb;                        // Variable: Kp_tb
                                          //  Referenced by:
                                          //    '<S230>/kp_tb'
                                          //    '<S556>/kp_tb'
                                          //    '<S609>/kp_tb'
                                          //    '<S290>/kp_tb'
                                          //    '<S307>/kp_tb'
                                          //    '<S324>/kp_tb'
                                          //    '<S341>/kp_tb'

  real_T Kp_tblue;                     // Variable: Kp_tblue
                                          //  Referenced by:
                                          //    '<S243>/kp_tb'
                                          //    '<S569>/kp_tb'
                                          //    '<S622>/kp_tb'
                                          //    '<S362>/kp_tb'
                                          //    '<S380>/kp_tb'
                                          //    '<S398>/kp_tb'
                                          //    '<S416>/kp_tb'

  real_T Kp_tr;                        // Variable: Kp_tr
                                          //  Referenced by:
                                          //    '<S256>/kp_tr'
                                          //    '<S582>/kp_tr'
                                          //    '<S635>/kp_tr'
                                          //    '<S439>/kp_tr'
                                          //    '<S472>/kp_tr'
                                          //    '<S524>/kp_tr'

  real_T Kp_wrarm;                     // Variable: Kp_wrarm
                                          //  Referenced by:
                                          //    '<S273>/kp_wrarm'
                                          //    '<S460>/kp_wrarm'
                                          //    '<S495>/kp_wrarm'
                                          //    '<S516>/kp_wrarm'

  real_T Kp_xb;                        // Variable: Kp_xb
                                          //  Referenced by:
                                          //    '<S231>/kp_xb'
                                          //    '<S557>/kp_xb'
                                          //    '<S610>/kp_xb'
                                          //    '<S291>/kp_xb'
                                          //    '<S308>/kp_xb'
                                          //    '<S325>/kp_xb'
                                          //    '<S342>/kp_xb'

  real_T Kp_xblue;                     // Variable: Kp_xblue
                                          //  Referenced by:
                                          //    '<S244>/kp_xb'
                                          //    '<S570>/kp_xb'
                                          //    '<S623>/kp_xb'
                                          //    '<S363>/kp_xb'
                                          //    '<S381>/kp_xb'
                                          //    '<S399>/kp_xb'
                                          //    '<S417>/kp_xb'

  real_T Kp_xr;                        // Variable: Kp_xr
                                          //  Referenced by:
                                          //    '<S257>/kp_xr'
                                          //    '<S583>/kp_xr'
                                          //    '<S636>/kp_xr'
                                          //    '<S440>/kp_xr'
                                          //    '<S473>/kp_xr'
                                          //    '<S503>/kp_xr'
                                          //    '<S525>/kp_xr'

  real_T Kp_yb;                        // Variable: Kp_yb
                                          //  Referenced by:
                                          //    '<S232>/kp_yb'
                                          //    '<S558>/kp_yb'
                                          //    '<S611>/kp_yb'
                                          //    '<S292>/kp_yb'
                                          //    '<S309>/kp_yb'
                                          //    '<S326>/kp_yb'
                                          //    '<S343>/kp_yb'

  real_T Kp_yblue;                     // Variable: Kp_yblue
                                          //  Referenced by:
                                          //    '<S245>/kp_yb'
                                          //    '<S571>/kp_yb'
                                          //    '<S624>/kp_yb'
                                          //    '<S364>/kp_yb'
                                          //    '<S382>/kp_yb'
                                          //    '<S400>/kp_yb'
                                          //    '<S418>/kp_yb'

  real_T Kp_yr;                        // Variable: Kp_yr
                                          //  Referenced by:
                                          //    '<S258>/kp_yr'
                                          //    '<S584>/kp_yr'
                                          //    '<S637>/kp_yr'
                                          //    '<S441>/kp_yr'
                                          //    '<S474>/kp_yr'
                                          //    '<S504>/kp_yr'
                                          //    '<S526>/kp_yr'

  real_T Kv[9];                        // Variable: Kv
                                          //  Referenced by:
                                          //    '<S598>/MATLAB Function'
                                          //    '<S651>/MATLAB Function'
                                          //    '<S544>/MATLAB Function'

  real_T Phase0_End;                   // Variable: Phase0_End
                                          //  Referenced by: '<Root>/Constant4'

  real_T Phase1_End;                   // Variable: Phase1_End
                                          //  Referenced by: '<Root>/Constant'

  real_T Phase2_End;                   // Variable: Phase2_End
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

  real_T Phase3_End;                   // Variable: Phase3_End
                                          //  Referenced by: '<Root>/Constant2'

  real_T Phase3_SubPhase1_End;         // Variable: Phase3_SubPhase1_End
                                          //  Referenced by:
                                          //    '<S280>/Constant4'
                                          //    '<S281>/Constant4'
                                          //    '<S282>/Constant4'
                                          //    '<S481>/Constant2'

  real_T Phase3_SubPhase2_End;         // Variable: Phase3_SubPhase2_End
                                          //  Referenced by:
                                          //    '<S280>/Constant1'
                                          //    '<S281>/Constant1'
                                          //    '<S282>/Constant1'
                                          //    '<S508>/Constant'

  real_T Phase3_SubPhase3_End;         // Variable: Phase3_SubPhase3_End
                                          //  Referenced by:
                                          //    '<S280>/Constant2'
                                          //    '<S281>/Constant2'
                                          //    '<S282>/Constant2'

  real_T Phase3_SubPhase4_End;         // Variable: Phase3_SubPhase4_End
                                          //  Referenced by:
                                          //    '<S280>/Constant3'
                                          //    '<S281>/Constant3'
                                          //    '<S282>/Constant3'

  real_T Phase4_End;                   // Variable: Phase4_End
                                          //  Referenced by: '<Root>/Constant3'

  real_T Phase5_End;                   // Variable: Phase5_End
                                          //  Referenced by: '<Root>/Constant6'

  real_T Tz_lim_elarm;                 // Variable: Tz_lim_elarm
                                          //  Referenced by:
                                          //    '<S261>/Saturation1'
                                          //    '<S587>/Saturation1'
                                          //    '<S640>/Saturation1'
                                          //    '<S446>/Saturation1'
                                          //    '<S479>/Saturation1'
                                          //    '<S508>/Saturation1'
                                          //    '<S531>/Saturation1'

  real_T Tz_lim_sharm;                 // Variable: Tz_lim_sharm
                                          //  Referenced by:
                                          //    '<S261>/Saturation'
                                          //    '<S587>/Saturation'
                                          //    '<S640>/Saturation'
                                          //    '<S446>/Saturation'
                                          //    '<S479>/Saturation'
                                          //    '<S508>/Saturation'
                                          //    '<S531>/Saturation'

  real_T Tz_lim_wrarm;                 // Variable: Tz_lim_wrarm
                                          //  Referenced by:
                                          //    '<S261>/Saturation3'
                                          //    '<S587>/Saturation3'
                                          //    '<S640>/Saturation3'
                                          //    '<S446>/Saturation3'
                                          //    '<S479>/Saturation3'
                                          //    '<S508>/Saturation3'
                                          //    '<S531>/Saturation3'

  real_T baseRate;                     // Variable: baseRate
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

  real_T drop_states_BLACK[3];         // Variable: drop_states_BLACK
                                          //  Referenced by: '<S666>/Velocity to Position'

  real_T drop_states_BLUE[3];          // Variable: drop_states_BLUE
                                          //  Referenced by: '<S678>/Velocity to Position'

  real_T drop_states_RED[3];           // Variable: drop_states_RED
                                          //  Referenced by: '<S704>/Velocity to Position'

  real_T home_states_BLACK[3];         // Variable: home_states_BLACK
                                          //  Referenced by:
                                          //    '<S552>/Desired Attitude (BLACK)'
                                          //    '<S552>/Desired Px (BLACK)'
                                          //    '<S552>/Desired Py (BLACK)'
                                          //    '<S605>/Constant'
                                          //    '<S605>/Constant2'
                                          //    '<S605>/Constant3'

  real_T home_states_BLUE[3];          // Variable: home_states_BLUE
                                          //  Referenced by:
                                          //    '<S553>/Desired Attitude (BLUE)'
                                          //    '<S553>/Desired Px (BLUE)'
                                          //    '<S553>/Desired Py (BLUE)'
                                          //    '<S606>/Desired Attitude (BLUE)'
                                          //    '<S606>/Desired Px (BLUE)'
                                          //    '<S606>/Desired Py (BLUE)'

  real_T home_states_RED[3];           // Variable: home_states_RED
                                          //  Referenced by:
                                          //    '<S554>/Desired Px (BLACK)'
                                          //    '<S554>/Desired Px (BLACK)1'
                                          //    '<S554>/Desired Px (BLACK)2'
                                          //    '<S607>/Desired Px (BLACK)'
                                          //    '<S607>/Desired Px (BLACK)1'
                                          //    '<S607>/Desired Px (BLACK)2'

  real_T init_states_BLACK[3];         // Variable: init_states_BLACK
                                          //  Referenced by:
                                          //    '<S226>/Desired Attitude (BLACK)'
                                          //    '<S226>/Desired Px (BLACK)'
                                          //    '<S226>/Desired Py (BLACK)'

  real_T init_states_BLUE[3];          // Variable: init_states_BLUE
                                          //  Referenced by:
                                          //    '<S227>/Desired Attitude (BLUE)'
                                          //    '<S227>/Desired Px (BLUE)'
                                          //    '<S227>/Desired Py (BLUE)'

  real_T init_states_RED[3];           // Variable: init_states_RED
                                          //  Referenced by:
                                          //    '<S228>/Constant'
                                          //    '<S228>/Constant1'
                                          //    '<S228>/Constant3'

  real_T model_param[20];              // Variable: model_param
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

  real_T noise_variance_BLACK;         // Variable: noise_variance_BLACK
                                          //  Referenced by: '<S666>/Random Number'

  real_T noise_variance_BLUE;          // Variable: noise_variance_BLUE
                                          //  Referenced by: '<S678>/Random Number'

  real_T noise_variance_RED;           // Variable: noise_variance_RED
                                          //  Referenced by: '<S704>/Random Number'

  real_T platformSelection;            // Variable: platformSelection
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

  real_T simMode;                      // Variable: simMode
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

  real_T yLength;                      // Variable: yLength
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

  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S94>/Out1'

  real_T Out1_Y0_p;                    // Computed Parameter: Out1_Y0_p
                                          //  Referenced by: '<S95>/Out1'

  real_T Out1_Y0_a;                    // Computed Parameter: Out1_Y0_a
                                          //  Referenced by: '<S96>/Out1'

  real_T Out1_Y0_pf;                   // Computed Parameter: Out1_Y0_pf
                                          //  Referenced by: '<S97>/Out1'

  real_T Out1_Y0_k;                    // Computed Parameter: Out1_Y0_k
                                          //  Referenced by: '<S98>/Out1'

  real_T Out1_Y0_f;                    // Computed Parameter: Out1_Y0_f
                                          //  Referenced by: '<S99>/Out1'

  real_T Out1_Y0_m;                    // Computed Parameter: Out1_Y0_m
                                          //  Referenced by: '<S100>/Out1'

  real_T Out1_Y0_po;                   // Computed Parameter: Out1_Y0_po
                                          //  Referenced by: '<S101>/Out1'

  real_T Out1_Y0_n;                    // Computed Parameter: Out1_Y0_n
                                          //  Referenced by: '<S102>/Out1'

  real_T Out1_Y0_pl;                   // Computed Parameter: Out1_Y0_pl
                                          //  Referenced by: '<S103>/Out1'

  real_T Out1_Y0_aq;                   // Computed Parameter: Out1_Y0_aq
                                          //  Referenced by: '<S104>/Out1'

  real_T Out1_Y0_au;                   // Computed Parameter: Out1_Y0_au
                                          //  Referenced by: '<S105>/Out1'

  real_T Out1_Y0_b;                    // Computed Parameter: Out1_Y0_b
                                          //  Referenced by: '<S106>/Out1'

  real_T Out1_Y0_bw;                   // Computed Parameter: Out1_Y0_bw
                                          //  Referenced by: '<S107>/Out1'

  real_T Out1_Y0_c;                    // Computed Parameter: Out1_Y0_c
                                          //  Referenced by: '<S108>/Out1'

  real_T Out1_Y0_e;                    // Computed Parameter: Out1_Y0_e
                                          //  Referenced by: '<S123>/Out1'

  real_T Out1_Y0_by;                   // Computed Parameter: Out1_Y0_by
                                          //  Referenced by: '<S126>/Out1'

  real_T Out1_Y0_bo;                   // Computed Parameter: Out1_Y0_bo
                                          //  Referenced by: '<S130>/Out1'

  real_T Out1_Y0_b1;                   // Computed Parameter: Out1_Y0_b1
                                          //  Referenced by: '<S136>/Out1'

  real_T Out1_Y0_ms;                   // Computed Parameter: Out1_Y0_ms
                                          //  Referenced by: '<S139>/Out1'

  real_T Out1_Y0_g;                    // Computed Parameter: Out1_Y0_g
                                          //  Referenced by: '<S142>/Out1'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S129>/Constant'

  real_T Gain_Gain;                    // Expression: pi/180
                                          //  Referenced by: '<S109>/Gain'

  real_T Gain1_Gain;                   // Expression: 9.81
                                          //  Referenced by: '<S109>/Gain1'

  real_T DiscreteTimeIntegrator1_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainval
                             //  Referenced by: '<S121>/Discrete-Time Integrator1'

  real_T Out1_Y0_bi;                   // Computed Parameter: Out1_Y0_bi
                                          //  Referenced by: '<S154>/Out1'

  real_T Out1_Y0_j;                    // Computed Parameter: Out1_Y0_j
                                          //  Referenced by: '<S157>/Out1'

  real_T Out1_Y0_bf;                   // Computed Parameter: Out1_Y0_bf
                                          //  Referenced by: '<S161>/Out1'

  real_T Out1_Y0_jp;                   // Computed Parameter: Out1_Y0_jp
                                          //  Referenced by: '<S167>/Out1'

  real_T Out1_Y0_nw;                   // Computed Parameter: Out1_Y0_nw
                                          //  Referenced by: '<S170>/Out1'

  real_T Out1_Y0_au3;                  // Computed Parameter: Out1_Y0_au3
                                          //  Referenced by: '<S173>/Out1'

  real_T Constant_Value_f;             // Expression: 0
                                          //  Referenced by: '<S160>/Constant'

  real_T Gain_Gain_k;                  // Expression: pi/180
                                          //  Referenced by: '<S110>/Gain'

  real_T Gain1_Gain_h;                 // Expression: 9.81
                                          //  Referenced by: '<S110>/Gain1'

  real_T DiscreteTimeIntegrator1_gainv_a;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_a
                             //  Referenced by: '<S152>/Discrete-Time Integrator1'

  real_T Out1_Y0_b3;                   // Computed Parameter: Out1_Y0_b3
                                          //  Referenced by: '<S185>/Out1'

  real_T Out1_Y0_l;                    // Computed Parameter: Out1_Y0_l
                                          //  Referenced by: '<S188>/Out1'

  real_T Out1_Y0_fj;                   // Computed Parameter: Out1_Y0_fj
                                          //  Referenced by: '<S192>/Out1'

  real_T Out1_Y0_np;                   // Computed Parameter: Out1_Y0_np
                                          //  Referenced by: '<S198>/Out1'

  real_T Out1_Y0_ki;                   // Computed Parameter: Out1_Y0_ki
                                          //  Referenced by: '<S201>/Out1'

  real_T Out1_Y0_k2;                   // Computed Parameter: Out1_Y0_k2
                                          //  Referenced by: '<S204>/Out1'

  real_T Constant_Value_i;             // Expression: 0
                                          //  Referenced by: '<S191>/Constant'

  real_T Gain_Gain_h;                  // Expression: pi/180
                                          //  Referenced by: '<S111>/Gain'

  real_T Gain1_Gain_g;                 // Expression: 9.81
                                          //  Referenced by: '<S111>/Gain1'

  real_T DiscreteTimeIntegrator1_gainv_k;
                          // Computed Parameter: DiscreteTimeIntegrator1_gainv_k
                             //  Referenced by: '<S183>/Discrete-Time Integrator1'

  real_T Out1_Y0_i;                    // Computed Parameter: Out1_Y0_i
                                          //  Referenced by: '<S673>/Out1'

  real_T Out1_Y0_bu;                   // Computed Parameter: Out1_Y0_bu
                                          //  Referenced by: '<S674>/Out1'

  real_T Out1_Y0_jq;                   // Computed Parameter: Out1_Y0_jq
                                          //  Referenced by: '<S675>/Out1'

  real_T Out1_Y0_mq;                   // Computed Parameter: Out1_Y0_mq
                                          //  Referenced by: '<S676>/Out1'

  real_T Out1_Y0_eo;                   // Computed Parameter: Out1_Y0_eo
                                          //  Referenced by: '<S677>/Out1'

  real_T Out1_Y0_eom;                  // Computed Parameter: Out1_Y0_eom
                                          //  Referenced by: '<S685>/Out1'

  real_T Out1_Y0_d;                    // Computed Parameter: Out1_Y0_d
                                          //  Referenced by: '<S686>/Out1'

  real_T Out1_Y0_ed;                   // Computed Parameter: Out1_Y0_ed
                                          //  Referenced by: '<S687>/Out1'

  real_T Out1_Y0_p0;                   // Computed Parameter: Out1_Y0_p0
                                          //  Referenced by: '<S688>/Out1'

  real_T Out1_Y0_ia;                   // Computed Parameter: Out1_Y0_ia
                                          //  Referenced by: '<S689>/Out1'

  real_T Out1_Y0_h;                    // Computed Parameter: Out1_Y0_h
                                          //  Referenced by: '<S701>/Out1'

  real_T Out1_Y0_hv;                   // Computed Parameter: Out1_Y0_hv
                                          //  Referenced by: '<S702>/Out1'

  real_T Delay_InitialCondition;       // Expression: 0.0
                                          //  Referenced by: '<S695>/Delay'

  real_T AccelerationtoVelocity_gainval;
                           // Computed Parameter: AccelerationtoVelocity_gainval
                              //  Referenced by: '<S695>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC[12];
                    // Expression: [drop_states_RED,drop_states_ARM,0,0,0,0,0,0]
                       //  Referenced by: '<S695>/Acceleration  to Velocity'

  real_T RandomNumber2_Mean;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number2'

  real_T RandomNumber2_StdDev;       // Computed Parameter: RandomNumber2_StdDev
                                        //  Referenced by: '<S695>/Random Number2'

  real_T RandomNumber2_Seed;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number2'

  real_T RandomNumber1_Mean;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number1'

  real_T RandomNumber1_StdDev;       // Computed Parameter: RandomNumber1_StdDev
                                        //  Referenced by: '<S695>/Random Number1'

  real_T RandomNumber1_Seed;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number1'

  real_T RandomNumber3_Mean;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number3'

  real_T RandomNumber3_StdDev;       // Computed Parameter: RandomNumber3_StdDev
                                        //  Referenced by: '<S695>/Random Number3'

  real_T RandomNumber3_Seed;           // Expression: 0
                                          //  Referenced by: '<S695>/Random Number3'

  real_T Out1_Y0_lt;                   // Computed Parameter: Out1_Y0_lt
                                          //  Referenced by: '<S711>/Out1'

  real_T Out1_Y0_f2;                   // Computed Parameter: Out1_Y0_f2
                                          //  Referenced by: '<S712>/Out1'

  real_T Out1_Y0_fu;                   // Computed Parameter: Out1_Y0_fu
                                          //  Referenced by: '<S713>/Out1'

  real_T Out1_Y0_lh;                   // Computed Parameter: Out1_Y0_lh
                                          //  Referenced by: '<S714>/Out1'

  real_T Out1_Y0_a3;                   // Computed Parameter: Out1_Y0_a3
                                          //  Referenced by: '<S715>/Out1'

  real_T AccelerationtoVelocity_gainva_k;
                          // Computed Parameter: AccelerationtoVelocity_gainva_k
                             //  Referenced by: '<S704>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_c;  // Expression: 0
                                          //  Referenced by: '<S704>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval;
                               // Computed Parameter: VelocitytoPosition_gainval
                                  //  Referenced by: '<S704>/Velocity to Position'

  real_T RandomNumber_Mean;            // Expression: 0
                                          //  Referenced by: '<S704>/Random Number'

  real_T RandomNumber_Seed;            // Expression: 0
                                          //  Referenced by: '<S704>/Random Number'

  real_T RandomNumber7_Mean;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number7'

  real_T RandomNumber7_StdDev;       // Computed Parameter: RandomNumber7_StdDev
                                        //  Referenced by: '<S703>/Random Number7'

  real_T RandomNumber7_Seed;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number7'

  real_T RandomNumber6_Mean;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number6'

  real_T RandomNumber6_StdDev;       // Computed Parameter: RandomNumber6_StdDev
                                        //  Referenced by: '<S703>/Random Number6'

  real_T RandomNumber6_Seed;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number6'

  real_T RandomNumber8_Mean;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number8'

  real_T RandomNumber8_StdDev;       // Computed Parameter: RandomNumber8_StdDev
                                        //  Referenced by: '<S703>/Random Number8'

  real_T RandomNumber8_Seed;           // Expression: 0
                                          //  Referenced by: '<S703>/Random Number8'

  real_T Constant_Value_j;             // Expression: 0
                                          //  Referenced by: '<S690>/Constant'

  real_T Constant_Value_o;             // Expression: 0
                                          //  Referenced by: '<S691>/Constant'

  real_T Constant_Value_ih;            // Expression: 0
                                          //  Referenced by: '<S692>/Constant'

  real_T AccelerationtoVelocity_gainva_i;
                          // Computed Parameter: AccelerationtoVelocity_gainva_i
                             //  Referenced by: '<S666>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_i;  // Expression: 0
                                          //  Referenced by: '<S666>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_c;
                             // Computed Parameter: VelocitytoPosition_gainval_c
                                //  Referenced by: '<S666>/Velocity to Position'

  real_T RandomNumber_Mean_n;          // Expression: 0
                                          //  Referenced by: '<S666>/Random Number'

  real_T RandomNumber_Seed_h;          // Expression: 0
                                          //  Referenced by: '<S666>/Random Number'

  real_T RandomNumber1_Mean_i;         // Expression: 0
                                          //  Referenced by: '<S662>/Random Number1'

  real_T RandomNumber1_StdDev_o;   // Computed Parameter: RandomNumber1_StdDev_o
                                      //  Referenced by: '<S662>/Random Number1'

  real_T RandomNumber1_Seed_m;         // Expression: 0
                                          //  Referenced by: '<S662>/Random Number1'

  real_T RandomNumber_Mean_e;          // Expression: 0
                                          //  Referenced by: '<S662>/Random Number'

  real_T RandomNumber_StdDev;         // Computed Parameter: RandomNumber_StdDev
                                         //  Referenced by: '<S662>/Random Number'

  real_T RandomNumber_Seed_a;          // Expression: 0
                                          //  Referenced by: '<S662>/Random Number'

  real_T RandomNumber2_Mean_c;         // Expression: 0
                                          //  Referenced by: '<S662>/Random Number2'

  real_T RandomNumber2_StdDev_p;   // Computed Parameter: RandomNumber2_StdDev_p
                                      //  Referenced by: '<S662>/Random Number2'

  real_T RandomNumber2_Seed_i;         // Expression: 0
                                          //  Referenced by: '<S662>/Random Number2'

  real_T AccelerationtoVelocity_gainva_b;
                          // Computed Parameter: AccelerationtoVelocity_gainva_b
                             //  Referenced by: '<S678>/Acceleration  to Velocity'

  real_T AccelerationtoVelocity_IC_p;  // Expression: 0
                                          //  Referenced by: '<S678>/Acceleration  to Velocity'

  real_T VelocitytoPosition_gainval_f;
                             // Computed Parameter: VelocitytoPosition_gainval_f
                                //  Referenced by: '<S678>/Velocity to Position'

  real_T RandomNumber_Mean_b;          // Expression: 0
                                          //  Referenced by: '<S678>/Random Number'

  real_T RandomNumber_Seed_k;          // Expression: 0
                                          //  Referenced by: '<S678>/Random Number'

  real_T RandomNumber4_Mean;           // Expression: 0
                                          //  Referenced by: '<S663>/Random Number4'

  real_T RandomNumber4_StdDev;       // Computed Parameter: RandomNumber4_StdDev
                                        //  Referenced by: '<S663>/Random Number4'

  real_T RandomNumber4_Seed;           // Expression: 0
                                          //  Referenced by: '<S663>/Random Number4'

  real_T RandomNumber3_Mean_o;         // Expression: 0
                                          //  Referenced by: '<S663>/Random Number3'

  real_T RandomNumber3_StdDev_j;   // Computed Parameter: RandomNumber3_StdDev_j
                                      //  Referenced by: '<S663>/Random Number3'

  real_T RandomNumber3_Seed_e;         // Expression: 0
                                          //  Referenced by: '<S663>/Random Number3'

  real_T RandomNumber5_Mean;           // Expression: 0
                                          //  Referenced by: '<S663>/Random Number5'

  real_T RandomNumber5_StdDev;       // Computed Parameter: RandomNumber5_StdDev
                                        //  Referenced by: '<S663>/Random Number5'

  real_T RandomNumber5_Seed;           // Expression: 0
                                          //  Referenced by: '<S663>/Random Number5'

  real_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<S664>/Discrete-Time Integrator'

  real_T DiscreteTimeIntegrator1_gain_aq;
                          // Computed Parameter: DiscreteTimeIntegrator1_gain_aq
                             //  Referenced by: '<S664>/Discrete-Time Integrator1'

  real_T DiscreteTimeIntegrator2_gainval;
                          // Computed Parameter: DiscreteTimeIntegrator2_gainval
                             //  Referenced by: '<S664>/Discrete-Time Integrator2'

  real_T Delay1_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S208>/Delay1'

  real_T Delay2_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S208>/Delay2'

  real_T Delay5_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S208>/Delay5'

  real_T Delay3_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S208>/Delay3'

  real_T Delay4_InitialCondition;      // Expression: 0
                                          //  Referenced by: '<S208>/Delay4'

  real_T Delay1_InitialCondition_j;    // Expression: 0
                                          //  Referenced by: '<S209>/Delay1'

  real_T Delay2_InitialCondition_j;    // Expression: 0
                                          //  Referenced by: '<S209>/Delay2'

  real_T Delay5_InitialCondition_g;    // Expression: 0
                                          //  Referenced by: '<S209>/Delay5'

  real_T Delay3_InitialCondition_k;    // Expression: 0
                                          //  Referenced by: '<S209>/Delay3'

  real_T Delay4_InitialCondition_m;    // Expression: 0
                                          //  Referenced by: '<S209>/Delay4'

  real_T Delay1_InitialCondition_n;    // Expression: 0
                                          //  Referenced by: '<S210>/Delay1'

  real_T Delay2_InitialCondition_p;    // Expression: 0
                                          //  Referenced by: '<S210>/Delay2'

  real_T Delay5_InitialCondition_o;    // Expression: 0
                                          //  Referenced by: '<S210>/Delay5'

  real_T Delay3_InitialCondition_e;    // Expression: 0
                                          //  Referenced by: '<S210>/Delay3'

  real_T Delay4_InitialCondition_h;    // Expression: 0
                                          //  Referenced by: '<S210>/Delay4'

  real_T WhoAmI_SampleTime;            // Expression: 0.1
                                          //  Referenced by: '<S17>/WhoAmI'

  real_T Out1_Y0_aw;                   // Computed Parameter: Out1_Y0_aw
                                          //  Referenced by: '<S235>/Out1'

  real_T Out1_Y0_av;                   // Computed Parameter: Out1_Y0_av
                                          //  Referenced by: '<S237>/Out1'

  real_T Out1_Y0_h4;                   // Computed Parameter: Out1_Y0_h4
                                          //  Referenced by: '<S239>/Out1'

  real_T PuckState_Value;              // Expression: 1
                                          //  Referenced by: '<S226>/Puck State'

  real_T Out1_Y0_mf;                   // Computed Parameter: Out1_Y0_mf
                                          //  Referenced by: '<S248>/Out1'

  real_T Out1_Y0_a4;                   // Computed Parameter: Out1_Y0_a4
                                          //  Referenced by: '<S250>/Out1'

  real_T Out1_Y0_kw;                   // Computed Parameter: Out1_Y0_kw
                                          //  Referenced by: '<S252>/Out1'

  real_T PuckState_Value_o;            // Expression: 1
                                          //  Referenced by: '<S227>/Puck State'

  real_T Out1_Y0_pj;                   // Computed Parameter: Out1_Y0_pj
                                          //  Referenced by: '<S263>/Out1'

  real_T Out1_Y0_lk;                   // Computed Parameter: Out1_Y0_lk
                                          //  Referenced by: '<S265>/Out1'

  real_T Out1_Y0_fa;                   // Computed Parameter: Out1_Y0_fa
                                          //  Referenced by: '<S267>/Out1'

  real_T Constant7_Value;              // Expression: 0
                                          //  Referenced by: '<S260>/Constant7'

  real_T Constant2_Value;              // Expression: 0
                                          //  Referenced by: '<S228>/Constant2'

  real_T PuckState_Value_l;            // Expression: 1
                                          //  Referenced by: '<S228>/Puck State'

  real_T Out1_Y0_mm;                   // Computed Parameter: Out1_Y0_mm
                                          //  Referenced by: '<S450>/Out1'

  real_T Out1_Y0_fjl;                  // Computed Parameter: Out1_Y0_fjl
                                          //  Referenced by: '<S452>/Out1'

  real_T Out1_Y0_hd;                   // Computed Parameter: Out1_Y0_hd
                                          //  Referenced by: '<S454>/Out1'

  real_T Constant7_Value_l;            // Expression: 0
                                          //  Referenced by: '<S445>/Constant7'

  real_T Constant1_Value;              // Expression: 0
                                          //  Referenced by: '<S434>/Constant1'

  real_T DesiredRateBLACK_Value;       // Expression: 0.03490659
                                          //  Referenced by: '<S447>/Desired Rate (BLACK)'

  real_T Constant_Value_p;             // Expression: pi
                                          //  Referenced by: '<S447>/Constant'

  real_T PuckState_Value_c;            // Expression: 1
                                          //  Referenced by: '<S434>/Puck State'

  real_T Out1_Y0_b5;                   // Computed Parameter: Out1_Y0_b5
                                          //  Referenced by: '<S485>/Out1'

  real_T Out1_Y0_lha;                  // Computed Parameter: Out1_Y0_lha
                                          //  Referenced by: '<S487>/Out1'

  real_T Out1_Y0_hh;                   // Computed Parameter: Out1_Y0_hh
                                          //  Referenced by: '<S489>/Out1'

  real_T Constant7_Value_c;            // Expression: 0
                                          //  Referenced by: '<S478>/Constant7'

  real_T DesiredRateBLACK_Value_i;     // Expression: 0.03490659
                                          //  Referenced by: '<S471>/Desired Rate (BLACK)'

  real_T Constant_Value_jl;            // Expression: pi
                                          //  Referenced by: '<S471>/Constant'

  real_T Constant1_Value_e;            // Expression: 0
                                          //  Referenced by: '<S435>/Constant1'

  real_T PuckState_Value_d;            // Expression: 1
                                          //  Referenced by: '<S435>/Puck State'

  real_T Out1_Y0_mv;                   // Computed Parameter: Out1_Y0_mv
                                          //  Referenced by: '<S511>/Out1'

  real_T Out1_Y0_lz;                   // Computed Parameter: Out1_Y0_lz
                                          //  Referenced by: '<S513>/Out1'

  real_T Out1_Y0_gm;                   // Computed Parameter: Out1_Y0_gm
                                          //  Referenced by: '<S518>/Out1'

  real_T Out1_Y0_io;                   // Computed Parameter: Out1_Y0_io
                                          //  Referenced by: '<S520>/Out1'

  real_T Out1_Y0_dk;                   // Computed Parameter: Out1_Y0_dk
                                          //  Referenced by: '<S522>/Out1'

  real_T uDLookupTable1_tableData[101];// Expression: [1.57000000000000
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
                                          // -0.298640304539486
                                          // -0.381952484472539
                                          // -0.466606302239867
                                          // -0.551765694801835
                                          // -0.636594599118806
                                          // -0.720256952151143
                                          // -0.801916690711806
                                          // -0.880723763116082
                                          // -0.955775800494991
                                          // -1.02615820551012
                                          // -1.09095638082305
                                          // -1.14925572909537
                                          // -1.20014165298867
                                          // -1.24269955516454
                                          // -1.27602769714638
                                          // -1.29978061439030
                                          // -1.31436988730625
                                          // -1.32026130853269
                                          // -1.31792067070809
                                          // -1.30781376647092
                                          // -1.29040638845964
                                          // -1.26616853457129
                                          // -1.23607724309064
                                          // -1.20209333571039
                                          // -1.16629053007224
                                          // -1.13074254381788
                                          // -1.09752309458901
                                          // -1.06870590002731
                                          // -1.04630359496360
                                          // -1.03189323418052
                                          // -1.02686268208325
                                          // -1.03259903268412
                                          // -1.05048937999544
                                          // -1.08191093831646
                                          // -1.12679451437392
                                          // -1.18211552054693
                                          // -1.24448846973170
                                          // -1.31052787482445
                                          // -1.37689087303085
                                          // -1.44074960138572
                                          // -1.49961795103105
                                          // -1.55095978791926
                                          // -1.58081445088490
                                          // -1.54996443310863
                                          // -1.48000000000000]
                                          //  Referenced by: '<S508>/1-D Lookup Table1'

  real_T uDLookupTable1_bp01Data[101]; // Expression: [0
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
                                          // 19.5000000000000
                                          // 19.8000000000000
                                          // 20.1000000000000
                                          // 20.4000000000000
                                          // 20.7000000000000
                                          // 21
                                          // 21.3000000000000
                                          // 21.6000000000000
                                          // 21.9000000000000
                                          // 22.2000000000000
                                          // 22.5000000000000
                                          // 22.8000000000000
                                          // 23.1000000000000
                                          // 23.4000000000000
                                          // 23.7000000000000
                                          // 24
                                          // 24.3000000000000
                                          // 24.6000000000000
                                          // 24.9000000000000
                                          // 25.2000000000000
                                          // 25.5000000000000
                                          // 25.8000000000000
                                          // 26.1000000000000
                                          // 26.4000000000000
                                          // 26.7000000000000
                                          // 27
                                          // 27.3000000000000
                                          // 27.6000000000000
                                          // 27.9000000000000
                                          // 28.2000000000000
                                          // 28.5000000000000
                                          // 28.8000000000000
                                          // 29.1000000000000
                                          // 29.4000000000000
                                          // 29.7000000000000
                                          // 30]
                                          //  Referenced by: '<S508>/1-D Lookup Table1'

  real_T uDLookupTable_tableData[101]; // Expression: [1.57000000000000
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
                                          // 0.392420798366228
                                          // 0.398750326166947
                                          // 0.408096675929411
                                          // 0.420249146825022
                                          // 0.434997038025178
                                          // 0.452129648701280
                                          // 0.471436276672662
                                          // 0.492577909508950
                                          // 0.514735652583953
                                          // 0.536978444975800
                                          // 0.558375225762617
                                          // 0.577994934022531
                                          // 0.594906508833669
                                          // 0.608178889274159
                                          // 0.616885200503004
                                          // 0.620279657398225
                                          // 0.617862923664403
                                          // 0.609153311284521
                                          // 0.593669132241562
                                          // 0.570928698518508
                                          // 0.540450322098342
                                          // 0.501754708369804
                                          // 0.454651142688837
                                          // 0.399508826722755
                                          // 0.336761216407457
                                          // 0.266841767678842
                                          // 0.190183936472810
                                          // 0.107221178725261
                                          // 0.0184637402812976
                                          // -0.0750305460272099
                                          // -0.171966007758580
                                          // -0.271046003976081
                                          // -0.370973893742981
                                          // -0.470452548764951
                                          // -0.568113490727538
                                          // -0.662442454298735
                                          // -0.751907371290790
                                          // -0.834976173515947
                                          // -0.910130221885170
                                          // -0.976013131751286
                                          // -1.03137619058426
                                          // -1.07497442671845
                                          // -1.10593044052833
                                          // -1.12417944460303
                                          // -1.13000000000000]
                                          //  Referenced by: '<S508>/1-D Lookup Table'

  real_T uDLookupTable_bp01Data[101];  // Expression: [0
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
                                          // 19.5000000000000
                                          // 19.8000000000000
                                          // 20.1000000000000
                                          // 20.4000000000000
                                          // 20.7000000000000
                                          // 21
                                          // 21.3000000000000
                                          // 21.6000000000000
                                          // 21.9000000000000
                                          // 22.2000000000000
                                          // 22.5000000000000
                                          // 22.8000000000000
                                          // 23.1000000000000
                                          // 23.4000000000000
                                          // 23.7000000000000
                                          // 24
                                          // 24.3000000000000
                                          // 24.6000000000000
                                          // 24.9000000000000
                                          // 25.2000000000000
                                          // 25.5000000000000
                                          // 25.8000000000000
                                          // 26.1000000000000
                                          // 26.4000000000000
                                          // 26.7000000000000
                                          // 27
                                          // 27.3000000000000
                                          // 27.6000000000000
                                          // 27.9000000000000
                                          // 28.2000000000000
                                          // 28.5000000000000
                                          // 28.8000000000000
                                          // 29.1000000000000
                                          // 29.4000000000000
                                          // 29.7000000000000
                                          // 30]
                                          //  Referenced by: '<S508>/1-D Lookup Table'

  real_T uDLookupTable2_tableData[101];// Expression: [4.24105195407198e-14
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
                                          // 0.696239394653257
                                          // 0.691014582997561
                                          // 0.681546260210274
                                          // 0.668167845572800
                                          // 0.651212758366539
                                          // 0.631014417872893
                                          // 0.607906243373263
                                          // 0.582221655279522
                                          // 0.554401355144190
                                          // 0.525287277542556
                                          // 0.495815140112214
                                          // 0.466920660490759
                                          // 0.439539556315785
                                          // 0.414607545224887
                                          // 0.393060344855659
                                          // 0.375828988968287
                                          // 0.363641886942561
                                          // 0.356951692355833
                                          // 0.356191311825241
                                          // 0.361793651967925
                                          // 0.374191619401025
                                          // 0.393818120741680
                                          // 0.421102648855173
                                          // 0.456063090510969
                                          // 0.497918715134657
                                          // 0.545797145289127
                                          // 0.598826003537267
                                          // 0.656132912441967
                                          // 0.716845494566115
                                          // 0.780034633378752
                                          // 0.844366607241886
                                          // 0.908331957834384
                                          // 0.970420511226358
                                          // 1.02912209348792
                                          // 1.08292634428082
                                          // 1.13029561275212
                                          // 1.16963648628818
                                          // 1.19934874289962
                                          // 1.21783216059706
                                          // 1.22367347867057
                                          // 1.21771835926845
                                          // 1.20231148671538
                                          // 1.17980152164021
                                          // 1.14786929261614
                                          // 1.09387618894762
                                          // 1.04000000000000]
                                          //  Referenced by: '<S508>/1-D Lookup Table2'

  real_T uDLookupTable2_bp01Data[101]; // Expression: [0
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
                                          // 19.5000000000000
                                          // 19.8000000000000
                                          // 20.1000000000000
                                          // 20.4000000000000
                                          // 20.7000000000000
                                          // 21
                                          // 21.3000000000000
                                          // 21.6000000000000
                                          // 21.9000000000000
                                          // 22.2000000000000
                                          // 22.5000000000000
                                          // 22.8000000000000
                                          // 23.1000000000000
                                          // 23.4000000000000
                                          // 23.7000000000000
                                          // 24
                                          // 24.3000000000000
                                          // 24.6000000000000
                                          // 24.9000000000000
                                          // 25.2000000000000
                                          // 25.5000000000000
                                          // 25.8000000000000
                                          // 26.1000000000000
                                          // 26.4000000000000
                                          // 26.7000000000000
                                          // 27
                                          // 27.3000000000000
                                          // 27.6000000000000
                                          // 27.9000000000000
                                          // 28.2000000000000
                                          // 28.5000000000000
                                          // 28.8000000000000
                                          // 29.1000000000000
                                          // 29.4000000000000
                                          // 29.7000000000000
                                          // 30]
                                          //  Referenced by: '<S508>/1-D Lookup Table2'

  real_T Constant7_Value_k;            // Expression: 0
                                          //  Referenced by: '<S507>/Constant7'

  real_T Constant4_Value;              // Expression: 0
                                          //  Referenced by: '<S436>/Constant4'

  real_T PuckState_Value_lm;           // Expression: 1
                                          //  Referenced by: '<S436>/Puck State'

  real_T Out1_Y0_ej;                   // Computed Parameter: Out1_Y0_ej
                                          //  Referenced by: '<S535>/Out1'

  real_T Out1_Y0_ma;                   // Computed Parameter: Out1_Y0_ma
                                          //  Referenced by: '<S537>/Out1'

  real_T Out1_Y0_nps;                  // Computed Parameter: Out1_Y0_nps
                                          //  Referenced by: '<S539>/Out1'

  real_T Constant7_Value_f;            // Expression: 0
                                          //  Referenced by: '<S530>/Constant7'

  real_T Constant1_Value_f;            // Expression: 0
                                          //  Referenced by: '<S437>/Constant1'

  real_T Constant_Value_d;             // Expression: 0
                                          //  Referenced by: '<S532>/Constant'

  real_T PuckState_Value_f;            // Expression: 1
                                          //  Referenced by: '<S437>/Puck State'

  real_T Out1_Y0_o;                    // Computed Parameter: Out1_Y0_o
                                          //  Referenced by: '<S561>/Out1'

  real_T Out1_Y0_ii;                   // Computed Parameter: Out1_Y0_ii
                                          //  Referenced by: '<S563>/Out1'

  real_T Out1_Y0_hq;                   // Computed Parameter: Out1_Y0_hq
                                          //  Referenced by: '<S565>/Out1'

  real_T PuckState_Value_fj;           // Expression: 1
                                          //  Referenced by: '<S552>/Puck State'

  real_T Out1_Y0_lc;                   // Computed Parameter: Out1_Y0_lc
                                          //  Referenced by: '<S589>/Out1'

  real_T Out1_Y0_of;                   // Computed Parameter: Out1_Y0_of
                                          //  Referenced by: '<S591>/Out1'

  real_T Out1_Y0_l2;                   // Computed Parameter: Out1_Y0_l2
                                          //  Referenced by: '<S593>/Out1'

  real_T Constant7_Value_m;            // Expression: 0
                                          //  Referenced by: '<S586>/Constant7'

  real_T Constant2_Value_g;            // Expression: 0
                                          //  Referenced by: '<S554>/Constant2'

  real_T PuckState_Value_od;           // Expression: 1
                                          //  Referenced by: '<S554>/Puck State'

  real_T Out1_Y0_by4;                  // Computed Parameter: Out1_Y0_by4
                                          //  Referenced by: '<S614>/Out1'

  real_T Out1_Y0_c2;                   // Computed Parameter: Out1_Y0_c2
                                          //  Referenced by: '<S616>/Out1'

  real_T Out1_Y0_f2b;                  // Computed Parameter: Out1_Y0_f2b
                                          //  Referenced by: '<S618>/Out1'

  real_T PuckState_Value_g;            // Expression: 1
                                          //  Referenced by: '<S605>/Puck State'

  real_T Out1_Y0_jf;                   // Computed Parameter: Out1_Y0_jf
                                          //  Referenced by: '<S642>/Out1'

  real_T Out1_Y0_fn;                   // Computed Parameter: Out1_Y0_fn
                                          //  Referenced by: '<S644>/Out1'

  real_T Out1_Y0_a2;                   // Computed Parameter: Out1_Y0_a2
                                          //  Referenced by: '<S646>/Out1'

  real_T Constant7_Value_a;            // Expression: 0
                                          //  Referenced by: '<S639>/Constant7'

  real_T Constant1_Value_k;            // Expression: 0
                                          //  Referenced by: '<S607>/Constant1'

  real_T PuckState_Value_e;            // Expression: 1
                                          //  Referenced by: '<S607>/Puck State'

  real_T Constant3_Value;              // Expression: 0
                                          //  Referenced by: '<S658>/Constant3'

  real_T Constant4_Value_e;            // Expression: 0
                                          //  Referenced by: '<S658>/Constant4'

  real_T Constant5_Value;              // Expression: 0
                                          //  Referenced by: '<S658>/Constant5'

  real_T PuckState_Value_gd;           // Expression: 0
                                          //  Referenced by: '<S658>/Puck State'

  real_T Constant_Value_jq;            // Expression: 0
                                          //  Referenced by: '<S660>/Constant'

  real_T Constant1_Value_l;            // Expression: 0
                                          //  Referenced by: '<S660>/Constant1'

  real_T Constant2_Value_m;            // Expression: 0
                                          //  Referenced by: '<S660>/Constant2'

  real_T Constant4_Value_o;            // Expression: 0
                                          //  Referenced by: '<S660>/Constant4'

  real_T Constant5_Value_f;            // Expression: 0
                                          //  Referenced by: '<S660>/Constant5'

  real_T Constant6_Value;              // Expression: 0
                                          //  Referenced by: '<S660>/Constant6'

  real_T PuckState_Value_h;            // Expression: 0
                                          //  Referenced by: '<S660>/Puck State'

  real_T Gain_Gain_a;                  // Expression: -1
                                          //  Referenced by: '<S660>/Gain'

  real_T Saturation_UpperSat;          // Expression: 0.005
                                          //  Referenced by: '<S660>/Saturation'

  real_T Saturation_LowerSat;          // Expression: -0.005
                                          //  Referenced by: '<S660>/Saturation'

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
                                          //  Referenced by: '<S78>/UDP Receive'

  int32_T UDPSend_Port;                // Computed Parameter: UDPSend_Port
                                          //  Referenced by: '<S18>/UDP Send'

  P_ChangeBLUEBehavior_Template_T ChangeBLUEBehavior_k;// '<S16>/Change BLUE Behavior' 
  P_REDARM_Template_v3_08_202_a_T REDARM_bo;// '<S607>/RED+ARM'
  P_ChangeBLUEBehavior_Templa_c_T ChangeBLUEBehavior_b;// '<S15>/Change BLUE Behavior' 
  P_REDARM_Template_v3_08_202_a_T REDARM_p;// '<S554>/RED+ARM'
  P_ChangeBLUEBehavior_Templa_c_T ChangeBLUEBehavior_gj;// '<S14>/Change BLUE Behavior' 
  P_REDARM_Template_v3_08_202_a_T REDARM_a;// '<S437>/RED+ARM'
  P_REDARM_Template_v3_08_2020a_T REDARM_b;// '<S435>/RED+ARM'
  P_REDARM_Template_v3_08_2020a_T REDARM_c;// '<S434>/RED+ARM'
  P_SubPhase1_Template_v3_08__k_T SubPhase4_f;// '<S281>/Sub-Phase #4'
  P_SubPhase1_Template_v3_08__k_T SubPhase3_p;// '<S281>/Sub-Phase #3 '
  P_SubPhase1_Template_v3_08__k_T SubPhase2_m;// '<S281>/Sub-Phase #2 '
  P_SubPhase1_Template_v3_08__k_T SubPhase1_a;// '<S281>/Sub-Phase #1'
  P_SubPhase1_Template_v3_08_20_T SubPhase4;// '<S280>/Sub-Phase #4'
  P_SubPhase1_Template_v3_08_20_T SubPhase3;// '<S280>/Sub-Phase #3 '
  P_SubPhase1_Template_v3_08_20_T SubPhase2;// '<S280>/Sub-Phase #2 '
  P_SubPhase1_Template_v3_08_20_T SubPhase1;// '<S280>/Sub-Phase #1'
  P_REDARM_Template_v3_08_2020a_T REDARM_j;// '<S228>/RED+ARM'
  P_Phase0WaitforSynchronizatio_T Phase1StartFloating;// '<Root>/Phase #1:  Start Floating ' 
  P_Phase0WaitforSynchronizatio_T Phase0WaitforSynchronization;
                                // '<Root>/Phase #0:  Wait for Synchronization'
  P_CalculateRunningMean_Temp_k_T CalculateRunningMean_ot;// '<S196>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_o2;// '<S195>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_f;// '<S194>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_hn;// '<S183>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_hq;// '<S182>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_hx;// '<S181>/Calculate Running Mean' 
  P_AHRS2_Template_v3_08_2020a_T AHRS2_pn;// '<S109>/AHRS2'
  P_CalculateRunningMean_Temp_k_T CalculateRunningMean_o;// '<S165>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_h;// '<S164>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_n;// '<S163>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_l;// '<S152>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_b;// '<S151>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_d;// '<S150>/Calculate Running Mean' 
  P_AHRS2_Template_v3_08_2020a_T AHRS2_p;// '<S109>/AHRS2'
  P_CalculateRunningMean_Temp_k_T CalculateRunningMean_ep;// '<S134>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_c;// '<S133>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_e;// '<S132>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_g;// '<S121>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean_i;// '<S120>/Calculate Running Mean' 
  P_CalculateRunningMean_Templa_T CalculateRunningMean;// '<S119>/Calculate Running Mean' 
  P_AHRS2_Template_v3_08_2020a_T AHRS2;// '<S109>/AHRS2'
};

// Real-time Model Data Structure
struct tag_RTM_Template_v3_08_2020a_T {
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

  extern P_Template_v3_08_2020a_T Template_v3_08_2020a_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern B_Template_v3_08_2020a_T Template_v3_08_2020a_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern DW_Template_v3_08_2020a_T Template_v3_08_2020a_DW;

// External function called from main
#ifdef __cplusplus

extern "C" {

#endif

  extern void Template_v3_08_2020a_SetEventsForThisBaseStep(boolean_T
    *eventFlags);

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void Template_v3_08_2020a_SetEventsForThisBaseStep(boolean_T
    *eventFlags);
  extern void Template_v3_08_2020a_initialize(void);
  extern void Template_v3_08_2020a_step(int_T tid);
  extern void Template_v3_08_2020a_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_Template_v3_08_2020a_T *const Template_v3_08_2020a_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S118>/Check Signal Attributes' : Unused code path elimination
//  Block '<S149>/Check Signal Attributes' : Unused code path elimination
//  Block '<S180>/Check Signal Attributes' : Unused code path elimination
//  Block '<S436>/Saturation' : Unused code path elimination
//  Block '<S436>/Saturation1' : Unused code path elimination
//  Block '<S436>/Saturation3' : Unused code path elimination
//  Block '<S78>/Reshape' : Reshape block reduction
//  Block '<S109>/Reshape' : Reshape block reduction
//  Block '<S109>/Reshape1' : Reshape block reduction
//  Block '<S109>/Reshape2' : Reshape block reduction
//  Block '<S109>/Reshape3' : Reshape block reduction
//  Block '<S110>/Reshape' : Reshape block reduction
//  Block '<S110>/Reshape1' : Reshape block reduction
//  Block '<S110>/Reshape2' : Reshape block reduction
//  Block '<S110>/Reshape3' : Reshape block reduction
//  Block '<S111>/Reshape' : Reshape block reduction
//  Block '<S111>/Reshape1' : Reshape block reduction
//  Block '<S111>/Reshape2' : Reshape block reduction
//  Block '<S111>/Reshape3' : Reshape block reduction
//  Block '<S546>/Reshape' : Reshape block reduction
//  Block '<S600>/Reshape' : Reshape block reduction
//  Block '<S653>/Reshape' : Reshape block reduction


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
//  '<Root>' : 'Template_v3_08_2020a'
//  '<S1>'   : 'Template_v3_08_2020a/Data Logger Subsystem'
//  '<S2>'   : 'Template_v3_08_2020a/Float & Magnet Controls'
//  '<S3>'   : 'Template_v3_08_2020a/From ARM Torques Command Joints'
//  '<S4>'   : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal'
//  '<S5>'   : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP'
//  '<S6>'   : 'Template_v3_08_2020a/IMU Data  and Algorithms'
//  '<S7>'   : 'Template_v3_08_2020a/Is this a  simulation?'
//  '<S8>'   : 'Template_v3_08_2020a/Manipulator Encoder Data'
//  '<S9>'   : 'Template_v3_08_2020a/Manipulator  Initialization'
//  '<S10>'  : 'Template_v3_08_2020a/Phase #0:  Wait for Synchronization'
//  '<S11>'  : 'Template_v3_08_2020a/Phase #1:  Start Floating '
//  '<S12>'  : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position'
//  '<S13>'  : 'Template_v3_08_2020a/Phase #3: Experiment'
//  '<S14>'  : 'Template_v3_08_2020a/Phase #4:  Return Home'
//  '<S15>'  : 'Template_v3_08_2020a/Phase #5:  Hold Home'
//  '<S16>'  : 'Template_v3_08_2020a/Phase #6:  Stop Floating and Spin Down RW'
//  '<S17>'  : 'Template_v3_08_2020a/Platform Identification'
//  '<S18>'  : 'Template_v3_08_2020a/Real-Time Visualization'
//  '<S19>'  : 'Template_v3_08_2020a/Simulate Plant Dynamics'
//  '<S20>'  : 'Template_v3_08_2020a/Subsystem'
//  '<S21>'  : 'Template_v3_08_2020a/Float & Magnet Controls/Change Behavior'
//  '<S22>'  : 'Template_v3_08_2020a/Float & Magnet Controls/Is this a  simulation?'
//  '<S23>'  : 'Template_v3_08_2020a/Float & Magnet Controls/Change Behavior/GPIO for Magnet'
//  '<S24>'  : 'Template_v3_08_2020a/Float & Magnet Controls/Change Behavior/GPIO for Pucks'
//  '<S25>'  : 'Template_v3_08_2020a/From ARM Torques Command Joints/Change BLACK Behavior'
//  '<S26>'  : 'Template_v3_08_2020a/From ARM Torques Command Joints/Change BLUE Behavior'
//  '<S27>'  : 'Template_v3_08_2020a/From ARM Torques Command Joints/Change RED Behavior'
//  '<S28>'  : 'Template_v3_08_2020a/From ARM Torques Command Joints/Is this a  simulation?'
//  '<S29>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior'
//  '<S30>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior'
//  '<S31>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior'
//  '<S32>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Is this a  simulation?'
//  '<S33>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Send Commands to PWM Blocks'
//  '<S34>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF'
//  '<S35>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Rotate Forces to Body'
//  '<S36>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S37>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S38>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function10'
//  '<S39>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function11'
//  '<S40>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S41>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S42>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S43>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/pseudo-inverse3'
//  '<S44>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S45>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S46>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF'
//  '<S47>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Rotate Forces to Body'
//  '<S48>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S49>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S50>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S51>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function6'
//  '<S52>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function7'
//  '<S53>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S54>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S55>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/pseudo-inverse2'
//  '<S56>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S57>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change BLUE Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S58>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF'
//  '<S59>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Rotate Forces to Body'
//  '<S60>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem'
//  '<S61>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem1'
//  '<S62>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem2'
//  '<S63>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/If Action Subsystem3'
//  '<S64>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function'
//  '<S65>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function1'
//  '<S66>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function2'
//  '<S67>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function3'
//  '<S68>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function4'
//  '<S69>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function5'
//  '<S70>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function6'
//  '<S71>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial'
//  '<S72>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/pseudo-inverse'
//  '<S73>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/pseudo-inverse1'
//  '<S74>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix'
//  '<S75>'  : 'Template_v3_08_2020a/From Force//Torque  to PWM Signal/Change RED Behavior/Rotate Forces to Body/Create Rotation Matrix'
//  '<S76>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Initialize Universal Time (Simulation)'
//  '<S77>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Is this a  simulation?1'
//  '<S78>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States'
//  '<S79>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x1'
//  '<S80>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x10'
//  '<S81>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x11'
//  '<S82>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x12'
//  '<S83>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x16'
//  '<S84>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x17'
//  '<S85>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x18'
//  '<S86>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x2'
//  '<S87>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x3'
//  '<S88>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x4'
//  '<S89>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x5'
//  '<S90>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x6'
//  '<S91>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x7'
//  '<S92>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x8'
//  '<S93>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x9'
//  '<S94>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x1/Hold this value'
//  '<S95>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x10/Hold this value'
//  '<S96>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x11/Hold this value'
//  '<S97>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x12/Hold this value'
//  '<S98>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x16/Hold this value'
//  '<S99>'  : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x17/Hold this value'
//  '<S100>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x18/Hold this value'
//  '<S101>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x2/Hold this value'
//  '<S102>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x3/Hold this value'
//  '<S103>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x4/Hold this value'
//  '<S104>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x5/Hold this value'
//  '<S105>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x6/Hold this value'
//  '<S106>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x7/Hold this value'
//  '<S107>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x8/Hold this value'
//  '<S108>' : 'Template_v3_08_2020a/Grab PhaseSpace  Data via UDP/Use Hardware to Obtain States/x_dot -> x9/Hold this value'
//  '<S109>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior'
//  '<S110>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior'
//  '<S111>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior'
//  '<S112>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Is this a  simulation?'
//  '<S113>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/ChangeOrientation'
//  '<S114>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Create Rotation Matrix'
//  '<S115>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Digital Filter Design'
//  '<S116>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias'
//  '<S117>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)'
//  '<S118>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Digital Filter Design/Check Signal Attributes'
//  '<S119>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX'
//  '<S120>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY'
//  '<S121>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S122>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S123>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S124>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S125>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S126>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S127>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S128>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S129>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S130>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S131>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S132>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S133>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S134>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S135>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S136>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S137>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S138>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S139>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S140>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S141>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S142>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S143>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLACK Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S144>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/ChangeOrientation'
//  '<S145>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Create Rotation Matrix'
//  '<S146>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Digital Filter Design'
//  '<S147>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias'
//  '<S148>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)'
//  '<S149>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Digital Filter Design/Check Signal Attributes'
//  '<S150>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX'
//  '<S151>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY'
//  '<S152>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S153>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S154>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S155>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S156>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S157>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S158>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S159>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S160>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S161>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S162>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S163>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S164>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S165>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S166>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S167>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S168>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S169>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S170>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S171>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S172>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S173>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S174>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change BLUE Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S175>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/ChangeOrientation'
//  '<S176>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Create Rotation Matrix'
//  '<S177>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Digital Filter Design1'
//  '<S178>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias'
//  '<S179>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)'
//  '<S180>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Digital Filter Design1/Check Signal Attributes'
//  '<S181>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX'
//  '<S182>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY'
//  '<S183>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ'
//  '<S184>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Calculate Running Mean'
//  '<S185>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Pass Current Gyro'
//  '<S186>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroX/Subsystem'
//  '<S187>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Calculate Running Mean'
//  '<S188>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Pass Current Gyro'
//  '<S189>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroY/Subsystem'
//  '<S190>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Calculate Running Mean'
//  '<S191>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Compare To Zero1'
//  '<S192>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Pass Current Gyro'
//  '<S193>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias/Correct GyroZ/Subsystem'
//  '<S194>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX'
//  '<S195>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY'
//  '<S196>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ'
//  '<S197>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Calculate Running Mean'
//  '<S198>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Pass Current Accel'
//  '<S199>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelX/Subsystem'
//  '<S200>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Calculate Running Mean'
//  '<S201>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Pass Current Accel'
//  '<S202>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelY/Subsystem'
//  '<S203>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Calculate Running Mean'
//  '<S204>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Pass Current Accel'
//  '<S205>' : 'Template_v3_08_2020a/IMU Data  and Algorithms/Change RED Behavior/Fix Initial Bias (Accel)/Correct AccelZ/Subsystem'
//  '<S206>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior'
//  '<S207>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Is this a  simulation?'
//  '<S208>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts'
//  '<S209>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts1'
//  '<S210>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts2'
//  '<S211>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts/Correct Dropouts'
//  '<S212>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts1/Correct Dropouts'
//  '<S213>' : 'Template_v3_08_2020a/Manipulator Encoder Data/Change Behavior/Correct for dropouts2/Correct Dropouts'
//  '<S214>' : 'Template_v3_08_2020a/Manipulator  Initialization/Change BLACK Behavior'
//  '<S215>' : 'Template_v3_08_2020a/Manipulator  Initialization/Change BLUE Behavior'
//  '<S216>' : 'Template_v3_08_2020a/Manipulator  Initialization/Change RED Behavior'
//  '<S217>' : 'Template_v3_08_2020a/Manipulator  Initialization/Is this a  simulation?'
//  '<S218>' : 'Template_v3_08_2020a/Phase #0:  Wait for Synchronization/Change BLACK Behavior'
//  '<S219>' : 'Template_v3_08_2020a/Phase #0:  Wait for Synchronization/Change BLUE Behavior'
//  '<S220>' : 'Template_v3_08_2020a/Phase #0:  Wait for Synchronization/Change RED Behavior'
//  '<S221>' : 'Template_v3_08_2020a/Phase #0:  Wait for Synchronization/Is this a  simulation?'
//  '<S222>' : 'Template_v3_08_2020a/Phase #1:  Start Floating /Change BLACK Behavior'
//  '<S223>' : 'Template_v3_08_2020a/Phase #1:  Start Floating /Change BLUE Behavior'
//  '<S224>' : 'Template_v3_08_2020a/Phase #1:  Start Floating /Change RED Behavior'
//  '<S225>' : 'Template_v3_08_2020a/Phase #1:  Start Floating /Is this a  simulation?'
//  '<S226>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior'
//  '<S227>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior'
//  '<S228>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior'
//  '<S229>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Is this a  simulation?'
//  '<S230>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S231>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S232>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S233>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control'
//  '<S234>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S235>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S236>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S237>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S238>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S239>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S240>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S241>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S242>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S243>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S244>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S245>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S246>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control'
//  '<S247>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S248>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S249>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S250>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S251>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S252>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S253>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S254>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S255>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S256>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S257>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S258>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S259>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control'
//  '<S260>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED'
//  '<S261>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM'
//  '<S262>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S263>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S264>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S265>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S266>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S267>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S268>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S269>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S270>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S271>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Elbow)'
//  '<S272>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Shoulder)'
//  '<S273>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Wrist)'
//  '<S274>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2'
//  '<S275>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2/Hold this value'
//  '<S276>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2'
//  '<S277>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2/Hold this value'
//  '<S278>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2'
//  '<S279>' : 'Template_v3_08_2020a/Phase #2:  Move to  Initial Position/Change RED Behavior/RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2/Hold this value'
//  '<S280>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior'
//  '<S281>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior'
//  '<S282>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior'
//  '<S283>' : 'Template_v3_08_2020a/Phase #3: Experiment/Is this a  simulation?'
//  '<S284>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1'
//  '<S285>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 '
//  '<S286>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 '
//  '<S287>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4'
//  '<S288>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Subsystem'
//  '<S289>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1'
//  '<S290>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S291>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S292>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S293>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control'
//  '<S294>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Subsystem'
//  '<S295>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1'
//  '<S296>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S297>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S298>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S299>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S300>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S301>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S302>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S303>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S304>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S305>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S306>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1'
//  '<S307>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)'
//  '<S308>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)'
//  '<S309>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)'
//  '<S310>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control'
//  '<S311>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Subsystem'
//  '<S312>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1'
//  '<S313>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S314>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S315>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S316>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S317>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S318>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S319>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S320>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function2'
//  '<S321>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function3'
//  '<S322>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #2 /Hough Control/MATLAB Function4'
//  '<S323>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1'
//  '<S324>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)'
//  '<S325>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)'
//  '<S326>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)'
//  '<S327>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control'
//  '<S328>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Subsystem'
//  '<S329>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Wrap1'
//  '<S330>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S331>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S332>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S333>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S334>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S335>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S336>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S337>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function2'
//  '<S338>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function3'
//  '<S339>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #3 /Hough Control/MATLAB Function4'
//  '<S340>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1'
//  '<S341>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S342>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S343>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S344>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control'
//  '<S345>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Subsystem'
//  '<S346>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1'
//  '<S347>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S348>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S349>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S350>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S351>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S352>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S353>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S354>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S355>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S356>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S357>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1'
//  '<S358>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 '
//  '<S359>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 '
//  '<S360>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4'
//  '<S361>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Subsystem'
//  '<S362>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S363>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S364>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S365>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control'
//  '<S366>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function2'
//  '<S367>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function3'
//  '<S368>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/RED_Rz_REF1'
//  '<S369>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Subsystem2'
//  '<S370>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S371>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S372>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S373>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S374>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S375>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S376>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S377>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S378>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S379>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/Subsystem2/Subsystem'
//  '<S380>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)'
//  '<S381>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)'
//  '<S382>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)'
//  '<S383>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control'
//  '<S384>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /MATLAB Function2'
//  '<S385>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /MATLAB Function3'
//  '<S386>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /RED_Rz_REF1'
//  '<S387>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Subsystem2'
//  '<S388>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S389>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S390>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S391>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S392>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S393>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S394>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function2'
//  '<S395>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function3'
//  '<S396>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Hough Control/MATLAB Function4'
//  '<S397>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #2 /Subsystem2/Subsystem'
//  '<S398>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)'
//  '<S399>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)'
//  '<S400>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)'
//  '<S401>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control'
//  '<S402>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /MATLAB Function2'
//  '<S403>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /MATLAB Function3'
//  '<S404>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /RED_Rz_REF1'
//  '<S405>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Subsystem2'
//  '<S406>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S407>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S408>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S409>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S410>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S411>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S412>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function2'
//  '<S413>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function3'
//  '<S414>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Hough Control/MATLAB Function4'
//  '<S415>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #3 /Subsystem2/Subsystem'
//  '<S416>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S417>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S418>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S419>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control'
//  '<S420>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/MATLAB Function2'
//  '<S421>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/MATLAB Function3'
//  '<S422>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/RED_Rz_REF1'
//  '<S423>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Subsystem2'
//  '<S424>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S425>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S426>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S427>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S428>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S429>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S430>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S431>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S432>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S433>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #4/Subsystem2/Subsystem'
//  '<S434>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1'
//  '<S435>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 '
//  '<S436>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 '
//  '<S437>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4'
//  '<S438>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Subsystem'
//  '<S439>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)'
//  '<S440>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)'
//  '<S441>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)'
//  '<S442>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Hough Control'
//  '<S443>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/MATLAB Function'
//  '<S444>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/MATLAB Function1'
//  '<S445>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED'
//  '<S446>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM'
//  '<S447>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED_RzD'
//  '<S448>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Subsystem1'
//  '<S449>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S450>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S451>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S452>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S453>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S454>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S455>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Hough Control/MATLAB Function2'
//  '<S456>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Hough Control/MATLAB Function3'
//  '<S457>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Hough Control/MATLAB Function4'
//  '<S458>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Elbow)'
//  '<S459>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Shoulder)'
//  '<S460>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Wrist)'
//  '<S461>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2'
//  '<S462>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2/Hold this value'
//  '<S463>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2'
//  '<S464>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2/Hold this value'
//  '<S465>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2'
//  '<S466>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2/Hold this value'
//  '<S467>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED_RzD/Subsystem'
//  '<S468>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED_RzD/Wrap1'
//  '<S469>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/RED_RzD/Wrap1/MATLAB Function'
//  '<S470>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/Subsystem1/Subsystem'
//  '<S471>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /BLACK_RzD1'
//  '<S472>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)'
//  '<S473>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)'
//  '<S474>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)'
//  '<S475>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Hough Control'
//  '<S476>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /MATLAB Function'
//  '<S477>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /MATLAB Function1'
//  '<S478>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED'
//  '<S479>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM'
//  '<S480>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Subsystem1'
//  '<S481>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /BLACK_RzD1/Subsystem'
//  '<S482>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1'
//  '<S483>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /BLACK_RzD1/Wrap1/MATLAB Function'
//  '<S484>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S485>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S486>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S487>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S488>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S489>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S490>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Hough Control/MATLAB Function2'
//  '<S491>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Hough Control/MATLAB Function3'
//  '<S492>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Hough Control/MATLAB Function4'
//  '<S493>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Elbow)'
//  '<S494>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Shoulder)'
//  '<S495>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Wrist)'
//  '<S496>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2'
//  '<S497>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2/Hold this value'
//  '<S498>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2'
//  '<S499>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2/Hold this value'
//  '<S500>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2'
//  '<S501>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2/Hold this value'
//  '<S502>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /Subsystem1/Subsystem'
//  '<S503>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)'
//  '<S504>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)'
//  '<S505>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /MATLAB Function'
//  '<S506>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /MATLAB Function1'
//  '<S507>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED'
//  '<S508>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM'
//  '<S509>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Subsystem1'
//  '<S510>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S511>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S512>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S513>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S514>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Elbow)'
//  '<S515>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Shoulder)'
//  '<S516>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Wrist)'
//  '<S517>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2'
//  '<S518>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Elbow)/x_dot -> x2/Hold this value'
//  '<S519>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2'
//  '<S520>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Shoulder)/x_dot -> x2/Hold this value'
//  '<S521>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2'
//  '<S522>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /RED+ARM/Custom Discrete PD (Wrist)/x_dot -> x2/Hold this value'
//  '<S523>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #3 /Subsystem1/Subsystem'
//  '<S524>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)'
//  '<S525>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)'
//  '<S526>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)'
//  '<S527>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control'
//  '<S528>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function'
//  '<S529>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function1'
//  '<S530>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED'
//  '<S531>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM'
//  '<S532>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED_Rz_REF'
//  '<S533>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Subsystem1'
//  '<S534>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S535>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S536>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S537>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S538>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S539>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S540>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function2'
//  '<S541>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function3'
//  '<S542>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Hough Control/MATLAB Function4'
//  '<S543>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller'
//  '<S544>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Arm Controller'
//  '<S545>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/End-Effector Position'
//  '<S546>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Error Signal'
//  '<S547>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian '
//  '<S548>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function'
//  '<S549>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function '
//  '<S550>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function'
//  '<S551>' : 'Template_v3_08_2020a/Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/Subsystem1/Subsystem'
//  '<S552>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior'
//  '<S553>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior'
//  '<S554>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior'
//  '<S555>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Is this a  simulation?'
//  '<S556>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S557>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S558>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S559>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Hough Control'
//  '<S560>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S561>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S562>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S563>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S564>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S565>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S566>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S567>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S568>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S569>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S570>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S571>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S572>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Hough Control'
//  '<S573>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S574>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S575>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S576>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S577>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S578>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S579>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S580>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S581>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S582>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S583>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S584>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S585>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Hough Control'
//  '<S586>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED'
//  '<S587>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM'
//  '<S588>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S589>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S590>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S591>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S592>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S593>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S594>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S595>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S596>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S597>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller'
//  '<S598>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller'
//  '<S599>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/End-Effector Position'
//  '<S600>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Error Signal'
//  '<S601>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian '
//  '<S602>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function'
//  '<S603>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function '
//  '<S604>' : 'Template_v3_08_2020a/Phase #4:  Return Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function'
//  '<S605>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior'
//  '<S606>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior'
//  '<S607>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior'
//  '<S608>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Is this a  simulation?'
//  '<S609>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)'
//  '<S610>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)'
//  '<S611>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)'
//  '<S612>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control'
//  '<S613>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S614>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S615>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S616>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S617>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S618>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S619>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function2'
//  '<S620>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function3'
//  '<S621>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLACK Behavior/Hough Control/MATLAB Function4'
//  '<S622>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)'
//  '<S623>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)'
//  '<S624>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)'
//  '<S625>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control'
//  '<S626>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1'
//  '<S627>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Attitude)/x_dot -> x1/Hold this value'
//  '<S628>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1'
//  '<S629>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (X-Position)/x_dot -> x1/Hold this value'
//  '<S630>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1'
//  '<S631>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Custom Discrete PD (Y-Position)/x_dot -> x1/Hold this value'
//  '<S632>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function2'
//  '<S633>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function3'
//  '<S634>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change BLUE Behavior/Hough Control/MATLAB Function4'
//  '<S635>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)'
//  '<S636>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)'
//  '<S637>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)'
//  '<S638>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Hough Control'
//  '<S639>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED'
//  '<S640>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM'
//  '<S641>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2'
//  '<S642>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Attitude)/x_dot -> x2/Hold this value'
//  '<S643>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2'
//  '<S644>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (X-Position)/x_dot -> x2/Hold this value'
//  '<S645>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2'
//  '<S646>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Custom Discrete PD (Y-Position)/x_dot -> x2/Hold this value'
//  '<S647>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function2'
//  '<S648>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function3'
//  '<S649>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/Hough Control/MATLAB Function4'
//  '<S650>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller'
//  '<S651>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller'
//  '<S652>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/End-Effector Position'
//  '<S653>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Error Signal'
//  '<S654>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian '
//  '<S655>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function'
//  '<S656>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function '
//  '<S657>' : 'Template_v3_08_2020a/Phase #5:  Hold Home/Change RED Behavior/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function'
//  '<S658>' : 'Template_v3_08_2020a/Phase #6:  Stop Floating and Spin Down RW/Change BLACK Behavior'
//  '<S659>' : 'Template_v3_08_2020a/Phase #6:  Stop Floating and Spin Down RW/Change BLUE Behavior'
//  '<S660>' : 'Template_v3_08_2020a/Phase #6:  Stop Floating and Spin Down RW/Change RED Behavior'
//  '<S661>' : 'Template_v3_08_2020a/Phase #6:  Stop Floating and Spin Down RW/Is this a  simulation?'
//  '<S662>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics'
//  '<S663>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics'
//  '<S664>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/IMU Model'
//  '<S665>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics'
//  '<S666>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model'
//  '<S667>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x1'
//  '<S668>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x2'
//  '<S669>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x3'
//  '<S670>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x4'
//  '<S671>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x7'
//  '<S672>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model/MATLAB Function'
//  '<S673>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x1/Hold this value'
//  '<S674>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x2/Hold this value'
//  '<S675>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x3/Hold this value'
//  '<S676>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x4/Hold this value'
//  '<S677>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLACK Dynamics/x_dot -> x7/Hold this value'
//  '<S678>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model'
//  '<S679>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x1'
//  '<S680>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x2'
//  '<S681>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x3'
//  '<S682>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x4'
//  '<S683>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x7'
//  '<S684>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model/MATLAB Function'
//  '<S685>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x1/Hold this value'
//  '<S686>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x2/Hold this value'
//  '<S687>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x3/Hold this value'
//  '<S688>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x4/Hold this value'
//  '<S689>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/BLUE Dynamics/x_dot -> x7/Hold this value'
//  '<S690>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/IMU Model/Compare To Zero'
//  '<S691>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/IMU Model/Compare To Zero1'
//  '<S692>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/IMU Model/Compare To Zero2'
//  '<S693>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM'
//  '<S694>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only'
//  '<S695>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics'
//  '<S696>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Coriolis Function'
//  '<S697>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Dynamics Model'
//  '<S698>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Inertia Function '
//  '<S699>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x1'
//  '<S700>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x2'
//  '<S701>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x1/Hold this value'
//  '<S702>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/x_dot -> x2/Hold this value'
//  '<S703>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics'
//  '<S704>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model'
//  '<S705>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x1'
//  '<S706>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x2'
//  '<S707>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x3'
//  '<S708>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x4'
//  '<S709>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x5'
//  '<S710>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model/MATLAB Function'
//  '<S711>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x1/Hold this value'
//  '<S712>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x2/Hold this value'
//  '<S713>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x3/Hold this value'
//  '<S714>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x4/Hold this value'
//  '<S715>' : 'Template_v3_08_2020a/Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/x_dot -> x5/Hold this value'

#endif                                 // RTW_HEADER_Template_v3_08_2020a_h_
