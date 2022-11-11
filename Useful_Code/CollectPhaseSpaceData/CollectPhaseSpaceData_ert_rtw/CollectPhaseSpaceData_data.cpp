//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CollectPhaseSpaceData_data.cpp
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
#include "CollectPhaseSpaceData.h"
#include "CollectPhaseSpaceData_private.h"

// Block parameters (default storage)
P_CollectPhaseSpaceData_T CollectPhaseSpaceData_P = {
  // Mask Parameter: UDPSend4_remotePort
  //  Referenced by: '<Root>/UDP Send4'

  25001,

  // Expression: 10
  //  Referenced by: '<Root>/Stream PhaseSpace to Platform'

  10.0,

  // Computed Parameter: Out1_Y0
  //  Referenced by: '<S6>/Out1'

  0.0,

  // Computed Parameter: Out1_Y0_h
  //  Referenced by: '<S3>/Out1'

  0.0,

  // Computed Parameter: ActualTime_Y0
  //  Referenced by: '<S4>/Actual Time'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/Constant'

  0.0,

  // Computed Parameter: UDPSend_Port
  //  Referenced by: '<Root>/UDP Send'

  25000,

  // Computed Parameter: UDPSend1_Port
  //  Referenced by: '<Root>/UDP Send1'

  25000,

  // Computed Parameter: UDPSend2_Port
  //  Referenced by: '<Root>/UDP Send2'

  25000,

  // Computed Parameter: Delay_InitialCondition
  //  Referenced by: '<S5>/Delay'

  1
};

//
// File trailer for generated code.
//
// [EOF]
//
