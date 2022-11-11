//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CollectPhaseSpaceData.h
//
// Code generated for Simulink model 'CollectPhaseSpaceData'.
//
// Model version                  : 1.49
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Fri Nov 11 17:01:11 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_CollectPhaseSpaceData_h_
#define RTW_HEADER_CollectPhaseSpaceData_h_
#include <cstring>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "owl.hpp"
#include "phasespace_headers.h"
#include "DAHostLib_Network.h"
#include "CollectPhaseSpaceData_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
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

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

// Block signals (default storage)
typedef struct {
  real_T TmpSignalConversionAtUDPSendInp[16];
  real_T StreamPhaseSpacetoPlatform[16];
  boolean_T rtb_Compare_m[16];
  real_T In1;                          // '<S4>/In1'
  real_T In1_b[16];                    // '<S3>/In1'
  real_T In1_bn;                       // '<S6>/In1'
  real_T b_y1;
  real_T y2;
  real_T y4;
  real_T y5;
  real_T y7;
  real_T y8;
  boolean_T DataTypeConversion;        // '<S5>/Data Type Conversion'
} B_CollectPhaseSpaceData_T;

// Block states (default storage) for system '<Root>'
typedef struct {
  PhaseSpace_ALL_CollectPhaseSp_T obj;// '<Root>/Stream PhaseSpace to Platform'
  real_T UDPSend_NetworkLib[137];      // '<Root>/UDP Send'
  real_T UDPSend1_NetworkLib[137];     // '<Root>/UDP Send1'
  real_T UDPSend2_NetworkLib[137];     // '<Root>/UDP Send2'
  real_T UDPSend4_NetworkLib[137];     // '<Root>/UDP Send4'
  boolean_T Delay_DSTATE;              // '<S5>/Delay'
} DW_CollectPhaseSpaceData_T;

// Parameters (default storage)
struct P_CollectPhaseSpaceData_T_ {
  int32_T UDPSend4_remotePort;         // Mask Parameter: UDPSend4_remotePort
                                          //  Referenced by: '<Root>/UDP Send4'

  real_T StreamPhaseSpacetoPlatform_PS_S;// Expression: 10
                                            //  Referenced by: '<Root>/Stream PhaseSpace to Platform'

  real_T Out1_Y0;                      // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S6>/Out1'

  real_T Out1_Y0_h;                    // Computed Parameter: Out1_Y0_h
                                          //  Referenced by: '<S3>/Out1'

  real_T ActualTime_Y0;                // Computed Parameter: ActualTime_Y0
                                          //  Referenced by: '<S4>/Actual Time'

  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<S1>/Constant'

  int32_T UDPSend_Port;                // Computed Parameter: UDPSend_Port
                                          //  Referenced by: '<Root>/UDP Send'

  int32_T UDPSend1_Port;               // Computed Parameter: UDPSend1_Port
                                          //  Referenced by: '<Root>/UDP Send1'

  int32_T UDPSend2_Port;               // Computed Parameter: UDPSend2_Port
                                          //  Referenced by: '<Root>/UDP Send2'

  boolean_T Delay_InitialCondition;// Computed Parameter: Delay_InitialCondition
                                      //  Referenced by: '<S5>/Delay'

};

// Real-time Model Data Structure
struct tag_RTM_CollectPhaseSpaceData_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_CollectPhaseSpaceData_T CollectPhaseSpaceData_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern B_CollectPhaseSpaceData_T CollectPhaseSpaceData_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern DW_CollectPhaseSpaceData_T CollectPhaseSpaceData_DW;

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void CollectPhaseSpaceData_initialize(void);
  extern void CollectPhaseSpaceData_step(void);
  extern void CollectPhaseSpaceData_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_CollectPhaseSpaceDat_T *const CollectPhaseSpaceData_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S7>/Check Signal Attributes' : Unused code path elimination
//  Block '<S8>/Check Signal Attributes' : Unused code path elimination
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
//  '<Root>' : 'CollectPhaseSpaceData'
//  '<S1>'   : 'CollectPhaseSpaceData/Compare To Zero1'
//  '<S2>'   : 'CollectPhaseSpaceData/Correct Time'
//  '<S3>'   : 'CollectPhaseSpaceData/Enabled Subsystem1'
//  '<S4>'   : 'CollectPhaseSpaceData/Enabled Subsystem2'
//  '<S5>'   : 'CollectPhaseSpaceData/Correct Time/Edge Detector'
//  '<S6>'   : 'CollectPhaseSpaceData/Correct Time/Enabled Subsystem'
//  '<S7>'   : 'CollectPhaseSpaceData/Correct Time/Edge Detector/Check Signal Attributes'
//  '<S8>'   : 'CollectPhaseSpaceData/Correct Time/Edge Detector/Check Signal Attributes1'

#endif                                 // RTW_HEADER_CollectPhaseSpaceData_h_

//
// File trailer for generated code.
//
// [EOF]
//
