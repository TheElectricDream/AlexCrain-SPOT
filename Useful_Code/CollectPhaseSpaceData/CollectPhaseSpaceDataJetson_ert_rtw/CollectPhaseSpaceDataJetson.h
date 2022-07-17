//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CollectPhaseSpaceDataJetson.h
//
// Code generated for Simulink model 'CollectPhaseSpaceDataJetson'.
//
// Model version                  : 5.2
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Sun Jul 17 13:19:08 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_CollectPhaseSpaceDataJetson_h_
#define RTW_HEADER_CollectPhaseSpaceDataJetson_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "owl.hpp"
#include "phasespace_headers.h"
#include "DAHostLib_Network.h"
#include "CollectPhaseSpaceDataJetson_types.h"
#include <stddef.h>

// Macros for accessing real-time model data structure
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

// Block signals (default storage)
struct B_CollectPhaseSpaceDataJetson_T {
  real_T In1[16];                      // '<S2>/In1'
  real_T StreamPhaseSpacetoPlatform[16];
                                      // '<Root>/Stream PhaseSpace to Platform'
  boolean_T Compare[16];               // '<S1>/Compare'
  real_T b_y1;
  real_T y2;
  real_T y4;
  real_T y5;
  real_T y7;
  real_T y8;
  real_T y9;
};

// Block states (default storage) for system '<Root>'
struct DW_CollectPhaseSpaceDataJetso_T {
  PhaseSpace_ALL_CollectPhaseSp_T obj;// '<Root>/Stream PhaseSpace to Platform'
  real_T UDPSend1_NetworkLib[137];     // '<Root>/UDP Send1'
  real_T UDPSend2_NetworkLib[137];     // '<Root>/UDP Send2'
  real_T UDPSend3_NetworkLib[137];     // '<Root>/UDP Send3'
  real_T UDPSend4_NetworkLib[137];     // '<Root>/UDP Send4'
};

// Parameters (default storage)
struct P_CollectPhaseSpaceDataJetson_T_ {
  int32_T UDPSend4_remotePort;         // Mask Parameter: UDPSend4_remotePort
                                          //  Referenced by: '<Root>/UDP Send4'

  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S2>/Out1'

  real_T StreamPhaseSpacetoPlatform_PS_S;// Expression: 20
                                            //  Referenced by: '<Root>/Stream PhaseSpace to Platform'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S1>/Constant'

  int32_T UDPSend1_Port;               // Computed Parameter: UDPSend1_Port
                                          //  Referenced by: '<Root>/UDP Send1'

  int32_T UDPSend2_Port;               // Computed Parameter: UDPSend2_Port
                                          //  Referenced by: '<Root>/UDP Send2'

  int32_T UDPSend3_Port;               // Computed Parameter: UDPSend3_Port
                                          //  Referenced by: '<Root>/UDP Send3'

};

// Real-time Model Data Structure
struct tag_RTM_CollectPhaseSpaceData_T {
  const char_T * volatile errorStatus;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    boolean_T stopRequestedFlag;
  } Timing;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_CollectPhaseSpaceDataJetson_T CollectPhaseSpaceDataJetson_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern struct B_CollectPhaseSpaceDataJetson_T CollectPhaseSpaceDataJetson_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern struct DW_CollectPhaseSpaceDataJetso_T CollectPhaseSpaceDataJetson_DW;

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void CollectPhaseSpaceDataJetson_initialize(void);
  extern void CollectPhaseSpaceDataJetson_step(void);
  extern void CollectPhaseSpaceDataJetson_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_CollectPhaseSpaceDat_T *const CollectPhaseSpaceDataJetson_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Data Type Conversion' : Eliminate redundant data type conversion


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
//  '<Root>' : 'CollectPhaseSpaceDataJetson'
//  '<S1>'   : 'CollectPhaseSpaceDataJetson/Compare To Zero1'
//  '<S2>'   : 'CollectPhaseSpaceDataJetson/Enabled Subsystem1'

#endif                             // RTW_HEADER_CollectPhaseSpaceDataJetson_h_

//
// File trailer for generated code.
//
// [EOF]
//
