/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Template_v3_08_2022a_Jetson_DataOnly2.h
 *
 * Code generated for Simulink model 'Template_v3_08_2022a_Jetson_DataOnly2'.
 *
 * Model version                  : 1.5
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Sun Jul 17 14:26:38 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM 64-bit (LP64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Template_v3_08_2022a_Jetson_DataOnly2_h_
#define RTW_HEADER_Template_v3_08_2022a_Jetson_DataOnly2_h_
#ifndef Template_v3_08_2022a_Jetson_DataOnly2_COMMON_INCLUDES_
#define Template_v3_08_2022a_Jetson_DataOnly2_COMMON_INCLUDES_
#include <stdlib.h>
#include "rtwtypes.h"
#include "rt_logging.h"
#include "DAHostLib_Network.h"
#endif              /* Template_v3_08_2022a_Jetson_DataOnly2_COMMON_INCLUDES_ */

#include "Template_v3_08_2022a_Jetson_DataOnly2_types.h"
#include <stddef.h>
#include <float.h>
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&(rtm)->Timing.taskTime0)
#endif

#define Template_v3_08_2022a_Jetson_DataOnly2_M (Template_v3_08_2022a_Jetson__M)

/* Block signals (default storage) */
typedef struct {
  real_T TmpSignalConversionAtToWork[74];
  real_T UDPReceive1_o1[16];           /* '<Root>/UDP Receive1' */
  real_T In1;                          /* '<S32>/In1' */
  real_T In1_g;                        /* '<S31>/In1' */
  real_T In1_g2;                       /* '<S30>/In1' */
  real_T In1_g20;                      /* '<S29>/In1' */
  real_T In1_g20a;                     /* '<S28>/In1' */
  real_T In1_g20as;                    /* '<S27>/In1' */
  real_T In1_g20asd;                   /* '<S26>/In1' */
  real_T In1_g20asdt;                  /* '<S25>/In1' */
  real_T In1_g20asdtg;                 /* '<S24>/In1' */
  real_T In1_g20asdtgi;                /* '<S23>/In1' */
  real_T In1_g20asdtgi0;               /* '<S22>/In1' */
  real_T In1_g20asdtgi0c;              /* '<S21>/In1' */
  real_T In1_g20asdtgi0cd;             /* '<S20>/In1' */
  real_T In1_g20asdtgi0cdy;            /* '<S19>/In1' */
  real_T In1_g20asdtgi0cdyi;           /* '<S18>/In1' */
  real_T Sum6;                         /* '<S12>/Sum6' */
  real_T Sum6_h;                       /* '<S4>/Sum6' */
  real_T Sum6_o;                       /* '<S5>/Sum6' */
  real_T Sum6_d;                       /* '<S14>/Sum6' */
} B_Template_v3_08_2022a_Jetson_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay1_DSTATE;                /* '<S12>/Delay1' */
  real_T Delay1_DSTATE_f;              /* '<S4>/Delay1' */
  real_T Delay1_DSTATE_ff;             /* '<S13>/Delay1' */
  real_T Delay1_DSTATE_a;              /* '<S5>/Delay1' */
  real_T Delay1_DSTATE_j;              /* '<S14>/Delay1' */
  real_T Delay1_DSTATE_o;              /* '<S6>/Delay1' */
  real_T Delay1_DSTATE_e;              /* '<S7>/Delay1' */
  real_T Delay1_DSTATE_n;              /* '<S3>/Delay1' */
  real_T Delay1_DSTATE_i;              /* '<S15>/Delay1' */
  real_T Delay1_DSTATE_ap;             /* '<S10>/Delay1' */
  real_T Delay1_DSTATE_l;              /* '<S16>/Delay1' */
  real_T Delay1_DSTATE_iy;             /* '<S11>/Delay1' */
  real_T Delay1_DSTATE_nn;             /* '<S17>/Delay1' */
  real_T Delay1_DSTATE_k;              /* '<S8>/Delay1' */
  real_T Delay1_DSTATE_ig;             /* '<S9>/Delay1' */
  real_T UDPReceive1_NetworkLib[137];  /* '<Root>/UDP Receive1' */
  real_T Unwrap1_Prev;                 /* '<Root>/Unwrap1' */
  real_T Unwrap1_Cumsum;               /* '<Root>/Unwrap1' */
  real_T Unwrap_Prev;                  /* '<Root>/Unwrap' */
  real_T Unwrap_Cumsum;                /* '<Root>/Unwrap' */
  real_T Unwrap2_Prev;                 /* '<Root>/Unwrap2' */
  real_T Unwrap2_Cumsum;               /* '<Root>/Unwrap2' */
  real_T BLACK_Fx_Sat;                 /* '<Root>/BLACK_Fx_Sat' */
  real_T BLUE_Fx_Sat;                  /* '<Root>/BLACK_Fx_Sat2' */
  real_T BLACK_Fy_Sat;                 /* '<Root>/BLACK_Fy_Sat' */
  real_T BLUE_Fy_Sat;                  /* '<Root>/BLACK_Fy_Sat2' */
  real_T BLACK_Tz_Sat;                 /* '<Root>/BLACK_Tz_Sat' */
  real_T BLUE_Tz_Sat;                  /* '<Root>/BLACK_Tz_Sat2' */
  real_T RED_Fx_Sat;                   /* '<Root>/RED_Fx_Sat' */
  real_T RED_Fy_Sat;                   /* '<Root>/RED_Fy_Sat' */
  real_T RED_AHRS_Q;                   /* '<Root>/RED_Px14' */
  real_T BLACK_IMU_Ax_b;               /* '<Root>/RED_Px15' */
  real_T BLACK_IMU_Ay_b;               /* '<Root>/RED_Px16' */
  real_T BLACK_IMU_Az_b;               /* '<Root>/RED_Px21' */
  real_T RED_IMU_Q;                    /* '<Root>/RED_Px28' */
  real_T RED_AHRS_P;                   /* '<Root>/RED_Px29' */
  real_T RED_IMU_P;                    /* '<Root>/RED_Px30' */
  real_T RED_IMU_R;                    /* '<Root>/RED_Px31' */
  real_T BLACK_IMU_Q;                  /* '<Root>/RED_Px32' */
  real_T BLACK_IMU_P;                  /* '<Root>/RED_Px33' */
  real_T BLACK_IMU_R;                  /* '<Root>/RED_Px34' */
  real_T RED_IMU_Psi;                  /* '<Root>/RED_Px35' */
  real_T BLACK_IMU_Psi;                /* '<Root>/RED_Px36' */
  real_T RED_AHRS_R;                   /* '<Root>/RED_Px40' */
  real_T RED_IMU_Ax_I;                 /* '<Root>/RED_Px42' */
  real_T RED_IMU_Ay_I;                 /* '<Root>/RED_Px43' */
  real_T BLACK_IMU_Ax_I;               /* '<Root>/RED_Px48' */
  real_T BLACK_IMU_Ay_I;               /* '<Root>/RED_Px49' */
  real_T BLACK_AHRS_Q;                 /* '<Root>/RED_Px51' */
  real_T BLACK_AHRS_P;                 /* '<Root>/RED_Px52' */
  real_T BLACK_AHRS_R;                 /* '<Root>/RED_Px53' */
  real_T RED_IMU_Ax_b;                 /* '<Root>/RED_Px54' */
  real_T RED_IMU_Ay_b;                 /* '<Root>/RED_Px55' */
  real_T RED_IMU_Az_b;                 /* '<Root>/RED_Px56' */
  real_T RED_Tz_Shoulder;              /* '<Root>/RED_Tz1' */
  real_T RED_RzD_Shoulder;             /* '<Root>/RED_Tz12' */
  real_T RED_RzD_Elbow;                /* '<Root>/RED_Tz13' */
  real_T RED_RzD_Wrist;                /* '<Root>/RED_Tz14' */
  real_T RED_Tz_Elbow;                 /* '<Root>/RED_Tz2' */
  real_T RED_Tz_Wrist;                 /* '<Root>/RED_Tz3' */
  real_T RED_Rz_Shoulder;              /* '<Root>/RED_Tz4' */
  real_T RED_Rz_Elbow;                 /* '<Root>/RED_Tz5' */
  real_T RED_Rz_Wrist;                 /* '<Root>/RED_Tz6' */
  real_T RED_Tz_Sat;                   /* '<Root>/RED_Tz_Sat' */
  struct {
    void *LoggedData;
  } ToWorkspace_PWORK;                 /* '<S1>/To Workspace' */

  boolean_T icLoad;                    /* '<S12>/Delay1' */
  boolean_T icLoad_h;                  /* '<S4>/Delay1' */
  boolean_T icLoad_c;                  /* '<S13>/Delay1' */
  boolean_T icLoad_i;                  /* '<S5>/Delay1' */
  boolean_T Unwrap1_FirstStep;         /* '<Root>/Unwrap1' */
  boolean_T icLoad_g;                  /* '<S14>/Delay1' */
  boolean_T icLoad_ic;                 /* '<S6>/Delay1' */
  boolean_T icLoad_f;                  /* '<S7>/Delay1' */
  boolean_T icLoad_j;                  /* '<S3>/Delay1' */
  boolean_T icLoad_ft;                 /* '<S15>/Delay1' */
  boolean_T icLoad_fw;                 /* '<S10>/Delay1' */
  boolean_T icLoad_g4;                 /* '<S16>/Delay1' */
  boolean_T Unwrap_FirstStep;          /* '<Root>/Unwrap' */
  boolean_T icLoad_m;                  /* '<S11>/Delay1' */
  boolean_T icLoad_cp;                 /* '<S17>/Delay1' */
  boolean_T icLoad_ml;                 /* '<S8>/Delay1' */
  boolean_T Unwrap2_FirstStep;         /* '<Root>/Unwrap2' */
  boolean_T icLoad_d;                  /* '<S9>/Delay1' */
} DW_Template_v3_08_2022a_Jetso_T;

/* Parameters (default storage) */
struct P_Template_v3_08_2022a_Jetson_T_ {
  real_T baseRate;                     /* Variable: baseRate
                                        * Referenced by:
                                        *   '<S3>/divide by delta T'
                                        *   '<S4>/divide by delta T'
                                        *   '<S5>/divide by delta T'
                                        *   '<S6>/divide by delta T'
                                        *   '<S7>/divide by delta T'
                                        *   '<S8>/divide by delta T'
                                        *   '<S9>/divide by delta T'
                                        *   '<S10>/divide by delta T'
                                        *   '<S11>/divide by delta T'
                                        *   '<S12>/divide by delta T'
                                        *   '<S13>/divide by delta T'
                                        *   '<S14>/divide by delta T'
                                        *   '<S15>/divide by delta T'
                                        *   '<S16>/divide by delta T'
                                        *   '<S17>/divide by delta T'
                                        */
  real_T Out1_Y0;                      /* Computed Parameter: Out1_Y0
                                        * Referenced by: '<S18>/Out1'
                                        */
  real_T Out1_Y0_c;                    /* Computed Parameter: Out1_Y0_c
                                        * Referenced by: '<S19>/Out1'
                                        */
  real_T Out1_Y0_f;                    /* Computed Parameter: Out1_Y0_f
                                        * Referenced by: '<S20>/Out1'
                                        */
  real_T Out1_Y0_fz;                   /* Computed Parameter: Out1_Y0_fz
                                        * Referenced by: '<S21>/Out1'
                                        */
  real_T Out1_Y0_cw;                   /* Computed Parameter: Out1_Y0_cw
                                        * Referenced by: '<S22>/Out1'
                                        */
  real_T Out1_Y0_o;                    /* Computed Parameter: Out1_Y0_o
                                        * Referenced by: '<S23>/Out1'
                                        */
  real_T Out1_Y0_g;                    /* Computed Parameter: Out1_Y0_g
                                        * Referenced by: '<S24>/Out1'
                                        */
  real_T Out1_Y0_cn;                   /* Computed Parameter: Out1_Y0_cn
                                        * Referenced by: '<S25>/Out1'
                                        */
  real_T Out1_Y0_fu;                   /* Computed Parameter: Out1_Y0_fu
                                        * Referenced by: '<S26>/Out1'
                                        */
  real_T Out1_Y0_gz;                   /* Computed Parameter: Out1_Y0_gz
                                        * Referenced by: '<S27>/Out1'
                                        */
  real_T Out1_Y0_e;                    /* Computed Parameter: Out1_Y0_e
                                        * Referenced by: '<S28>/Out1'
                                        */
  real_T Out1_Y0_p;                    /* Computed Parameter: Out1_Y0_p
                                        * Referenced by: '<S29>/Out1'
                                        */
  real_T Out1_Y0_n;                    /* Computed Parameter: Out1_Y0_n
                                        * Referenced by: '<S30>/Out1'
                                        */
  real_T Out1_Y0_fh;                   /* Computed Parameter: Out1_Y0_fh
                                        * Referenced by: '<S31>/Out1'
                                        */
  real_T Out1_Y0_a;                    /* Computed Parameter: Out1_Y0_a
                                        * Referenced by: '<S32>/Out1'
                                        */
  real_T BLACK_Fx_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fx'
                                        */
  real_T BLACK_Fx1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fx1'
                                        */
  real_T BLACK_Fy_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fy'
                                        */
  real_T BLACK_Fy1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fy1'
                                        */
  real_T BLACK_Tz_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Tz'
                                        */
  real_T BLACK_Tz1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Tz1'
                                        */
  real_T RED_Fx_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Fx'
                                        */
  real_T RED_Fy_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Fy'
                                        */
  real_T RED_Px22_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px22'
                                        */
  real_T RED_Px23_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px23'
                                        */
  real_T RED_Px24_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px24'
                                        */
  real_T RED_Px25_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px25'
                                        */
  real_T RED_Px26_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px26'
                                        */
  real_T RED_Px27_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px27'
                                        */
  real_T RED_Px37_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px37'
                                        */
  real_T RED_Px38_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px38'
                                        */
  real_T RED_Px39_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px39'
                                        */
  real_T RED_Px41_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px41'
                                        */
  real_T RED_Px44_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px44'
                                        */
  real_T RED_Px45_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px45'
                                        */
  real_T RED_Px46_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px46'
                                        */
  real_T RED_Px47_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px47'
                                        */
  real_T RED_Px50_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px50'
                                        */
  real_T RED_Tz_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz'
                                        */
  real_T RED_Tz10_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz10'
                                        */
  real_T RED_Tz11_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz11'
                                        */
  real_T RED_Tz7_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz7'
                                        */
  real_T RED_Tz8_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz8'
                                        */
  real_T RED_Tz9_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz9'
                                        */
  real_T RED_Tz_RW_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_RW'
                                        */
  real_T RED_Tz_RWSat_InitialValue;    /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_RW Sat'
                                        */
  real_T RED_dRz_RWSat_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/RED_dRz_RW Sat'
                                        */
  real_T Universal_Time16_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time16'
                                        */
  real_T Universal_Time17_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time17'
                                        */
  real_T Universal_Time18_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time18'
                                        */
  real_T Constant_Value;               /* Expression: 3
                                        * Referenced by: '<S2>/Constant'
                                        */
  real_T BLACK_Fx_Sat_InitialValue;    /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fx_Sat'
                                        */
  real_T BLACK_Fx_Sat1_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fx_Sat1'
                                        */
  real_T BLACK_Fx_Sat2_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fx_Sat2'
                                        */
  real_T BLACK_Fy_Sat_InitialValue;    /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fy_Sat'
                                        */
  real_T BLACK_Fy_Sat1_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fy_Sat1'
                                        */
  real_T BLACK_Fy_Sat2_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Fy_Sat2'
                                        */
  real_T BLACK_Px_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Px'
                                        */
  real_T BLACK_Px1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Px1'
                                        */
  real_T BLACK_Py_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Py'
                                        */
  real_T BLACK_Py1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Py1'
                                        */
  real_T BLACK_Rz_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Rz'
                                        */
  real_T BLACK_Rz1_InitialValue;       /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Rz1'
                                        */
  real_T BLACK_Tz_Sat_InitialValue;    /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Tz_Sat'
                                        */
  real_T BLACK_Tz_Sat1_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Tz_Sat1'
                                        */
  real_T BLACK_Tz_Sat2_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/BLACK_Tz_Sat2'
                                        */
  real_T RED_Fx_Sat_InitialValue;      /* Expression: 0
                                        * Referenced by: '<Root>/RED_Fx_Sat'
                                        */
  real_T RED_Fy_Sat_InitialValue;      /* Expression: 0
                                        * Referenced by: '<Root>/RED_Fy_Sat'
                                        */
  real_T RED_Px_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px'
                                        */
  real_T RED_Px14_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px14'
                                        */
  real_T RED_Px15_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px15'
                                        */
  real_T RED_Px16_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px16'
                                        */
  real_T RED_Px21_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px21'
                                        */
  real_T RED_Px28_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px28'
                                        */
  real_T RED_Px29_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px29'
                                        */
  real_T RED_Px30_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px30'
                                        */
  real_T RED_Px31_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px31'
                                        */
  real_T RED_Px32_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px32'
                                        */
  real_T RED_Px33_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px33'
                                        */
  real_T RED_Px34_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px34'
                                        */
  real_T RED_Px35_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px35'
                                        */
  real_T RED_Px36_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px36'
                                        */
  real_T RED_Px40_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px40'
                                        */
  real_T RED_Px42_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px42'
                                        */
  real_T RED_Px43_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px43'
                                        */
  real_T RED_Px48_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px48'
                                        */
  real_T RED_Px49_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px49'
                                        */
  real_T RED_Px51_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px51'
                                        */
  real_T RED_Px52_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px52'
                                        */
  real_T RED_Px53_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px53'
                                        */
  real_T RED_Px54_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px54'
                                        */
  real_T RED_Px55_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px55'
                                        */
  real_T RED_Px56_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Px56'
                                        */
  real_T RED_Py_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Py'
                                        */
  real_T RED_Rz_InitialValue;          /* Expression: 0
                                        * Referenced by: '<Root>/RED_Rz'
                                        */
  real_T RED_Tz1_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz1'
                                        */
  real_T RED_Tz12_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz12'
                                        */
  real_T RED_Tz13_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz13'
                                        */
  real_T RED_Tz14_InitialValue;        /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz14'
                                        */
  real_T RED_Tz2_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz2'
                                        */
  real_T RED_Tz3_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz3'
                                        */
  real_T RED_Tz4_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz4'
                                        */
  real_T RED_Tz5_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz5'
                                        */
  real_T RED_Tz6_InitialValue;         /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz6'
                                        */
  real_T RED_Tz_RWSat1_InitialValue;   /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_RW Sat1'
                                        */
  real_T RED_Tz_RW1_InitialValue;      /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_RW1'
                                        */
  real_T RED_Tz_Sat_InitialValue;      /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_Sat'
                                        */
  real_T RED_Tz_Sat1_InitialValue;     /* Expression: 0
                                        * Referenced by: '<Root>/RED_Tz_Sat1'
                                        */
  real_T Universal_Time_InitialValue;  /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time'
                                        */
  real_T Universal_Time1_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time1'
                                        */
  real_T Universal_Time10_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time10'
                                        */
  real_T Universal_Time11_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time11'
                                        */
  real_T Universal_Time12_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time12'
                                        */
  real_T Universal_Time13_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time13'
                                        */
  real_T Universal_Time14_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time14'
                                        */
  real_T Universal_Time15_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time15'
                                        */
  real_T Universal_Time19_InitialValue;/* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time19'
                                        */
  real_T Universal_Time2_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time2'
                                        */
  real_T Universal_Time3_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time3'
                                        */
  real_T Universal_Time4_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time4'
                                        */
  real_T Universal_Time5_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time5'
                                        */
  real_T Universal_Time6_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time6'
                                        */
  real_T Universal_Time7_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time7'
                                        */
  real_T Universal_Time8_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time8'
                                        */
  real_T Universal_Time9_InitialValue; /* Expression: 0
                                        * Referenced by: '<Root>/Universal_Time9'
                                        */
  int32_T UDPReceive1_Port;            /* Computed Parameter: UDPReceive1_Port
                                        * Referenced by: '<Root>/UDP Receive1'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Template_v3_08_2022a__T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (default storage) */
extern P_Template_v3_08_2022a_Jetson_T Template_v3_08_2022a_Jetson_D_P;

/* Block signals (default storage) */
extern B_Template_v3_08_2022a_Jetson_T Template_v3_08_2022a_Jetson_D_B;

/* Block states (default storage) */
extern DW_Template_v3_08_2022a_Jetso_T Template_v3_08_2022a_Jetson__DW;

/* Model entry point functions */
extern void Template_v3_08_2022a_Jetson_DataOnly2_initialize(void);
extern void Template_v3_08_2022a_Jetson_DataOnly2_step(void);
extern void Template_v3_08_2022a_Jetson_DataOnly2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Template_v3_08_2022a_T *const Template_v3_08_2022a_Jetson__M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Reshape' : Reshape block reduction
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Template_v3_08_2022a_Jetson_DataOnly2'
 * '<S1>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/Data Logger Subsystem'
 * '<S2>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/Platform Identification'
 * '<S3>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x1'
 * '<S4>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x10'
 * '<S5>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x11'
 * '<S6>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x12'
 * '<S7>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x16'
 * '<S8>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x17'
 * '<S9>'   : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x18'
 * '<S10>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x2'
 * '<S11>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x3'
 * '<S12>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x4'
 * '<S13>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x5'
 * '<S14>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x6'
 * '<S15>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x7'
 * '<S16>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x8'
 * '<S17>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x9'
 * '<S18>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x1/Hold this value'
 * '<S19>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x10/Hold this value'
 * '<S20>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x11/Hold this value'
 * '<S21>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x12/Hold this value'
 * '<S22>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x16/Hold this value'
 * '<S23>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x17/Hold this value'
 * '<S24>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x18/Hold this value'
 * '<S25>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x2/Hold this value'
 * '<S26>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x3/Hold this value'
 * '<S27>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x4/Hold this value'
 * '<S28>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x5/Hold this value'
 * '<S29>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x6/Hold this value'
 * '<S30>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x7/Hold this value'
 * '<S31>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x8/Hold this value'
 * '<S32>'  : 'Template_v3_08_2022a_Jetson_DataOnly2/x_dot -> x9/Hold this value'
 */
#endif                 /* RTW_HEADER_Template_v3_08_2022a_Jetson_DataOnly2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
