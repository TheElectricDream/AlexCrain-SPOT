//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CollectPhaseSpaceDataJetson.cpp
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
#include "CollectPhaseSpaceDataJetson.h"
#include <cstring>
#include "rtwtypes.h"

// Block signals (default storage)
B_CollectPhaseSpaceDataJetson_T CollectPhaseSpaceDataJetson_B;

// Block states (default storage)
DW_CollectPhaseSpaceDataJetso_T CollectPhaseSpaceDataJetson_DW;

// Real-time model
RT_MODEL_CollectPhaseSpaceDat_T CollectPhaseSpaceDataJetson_M_ =
  RT_MODEL_CollectPhaseSpaceDat_T();
RT_MODEL_CollectPhaseSpaceDat_T *const CollectPhaseSpaceDataJetson_M =
  &CollectPhaseSpaceDataJetson_M_;

// Model step function
void CollectPhaseSpaceDataJetson_step(void)
{
  real_T y10;
  real_T y11;
  real_T y12;
  real_T y13;
  real_T y14;
  real_T y15;
  char_T *sErr;

  // MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
  if (CollectPhaseSpaceDataJetson_DW.obj.PS_SampleRate !=
      CollectPhaseSpaceDataJetson_P.StreamPhaseSpacetoPlatform_PS_S) {
    CollectPhaseSpaceDataJetson_DW.obj.PS_SampleRate =
      CollectPhaseSpaceDataJetson_P.StreamPhaseSpacetoPlatform_PS_S;
  }

  //         %% Define output properties
  CollectPhaseSpaceDataJetson_B.b_y1 = 0.0;
  CollectPhaseSpaceDataJetson_B.y2 = 0.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[3] = 0.0;
  CollectPhaseSpaceDataJetson_B.y4 = 0.0;
  CollectPhaseSpaceDataJetson_B.y5 = 0.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[6] = 0.0;
  CollectPhaseSpaceDataJetson_B.y7 = 0.0;
  CollectPhaseSpaceDataJetson_B.y8 = 0.0;
  CollectPhaseSpaceDataJetson_B.y9 = 0.0;
  y10 = 0.0;
  y11 = 0.0;
  y12 = 0.0;
  y13 = 0.0;
  y14 = 0.0;
  y15 = 0.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[15] = 0.0;
  stream_phasespace(&CollectPhaseSpaceDataJetson_B.b_y1,
                    &CollectPhaseSpaceDataJetson_B.y2,
                    &CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[3],
                    &CollectPhaseSpaceDataJetson_B.y4,
                    &CollectPhaseSpaceDataJetson_B.y5,
                    &CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[6],
                    &CollectPhaseSpaceDataJetson_B.y7,
                    &CollectPhaseSpaceDataJetson_B.y8,
                    &CollectPhaseSpaceDataJetson_B.y9, &y10, &y11, &y12, &y13,
                    &y14, &y15,
                    &CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[15]);
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[0] =
    CollectPhaseSpaceDataJetson_B.y7 /
    CollectPhaseSpaceDataJetson_DW.obj.PS_SampleRate;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[1] =
    CollectPhaseSpaceDataJetson_B.b_y1 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[2] =
    CollectPhaseSpaceDataJetson_B.y2 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[4] =
    CollectPhaseSpaceDataJetson_B.y4 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[5] =
    CollectPhaseSpaceDataJetson_B.y5 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[7] =
    CollectPhaseSpaceDataJetson_B.y8 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[8] =
    CollectPhaseSpaceDataJetson_B.y9 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[9] = y10 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[10] = y11 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[11] = y12 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[12] = y13 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[13] = y14 / 1000.0;
  CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[14] = y15 / 1000.0;

  // End of MATLABSystem: '<Root>/Stream PhaseSpace to Platform'

  // RelationalOperator: '<S1>/Compare' incorporates:
  //   Constant: '<S1>/Constant'
  //   MATLABSystem: '<Root>/Stream PhaseSpace to Platform'

  for (int32_T i = 0; i < 16; i++) {
    CollectPhaseSpaceDataJetson_B.Compare[i] =
      (CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[i] !=
       CollectPhaseSpaceDataJetson_P.Constant_Value);
  }

  // End of RelationalOperator: '<S1>/Compare'

  // Outputs for Enabled SubSystem: '<Root>/Enabled Subsystem1' incorporates:
  //   EnablePort: '<S2>/Enable'

  // RelationalOperator: '<S1>/Compare'
  if (CollectPhaseSpaceDataJetson_B.Compare[0] ||
      CollectPhaseSpaceDataJetson_B.Compare[1] ||
      CollectPhaseSpaceDataJetson_B.Compare[2] ||
      CollectPhaseSpaceDataJetson_B.Compare[3] ||
      CollectPhaseSpaceDataJetson_B.Compare[4] ||
      CollectPhaseSpaceDataJetson_B.Compare[5] ||
      CollectPhaseSpaceDataJetson_B.Compare[6] ||
      CollectPhaseSpaceDataJetson_B.Compare[7] ||
      CollectPhaseSpaceDataJetson_B.Compare[8] ||
      CollectPhaseSpaceDataJetson_B.Compare[9] ||
      CollectPhaseSpaceDataJetson_B.Compare[10] ||
      CollectPhaseSpaceDataJetson_B.Compare[11] ||
      CollectPhaseSpaceDataJetson_B.Compare[12] ||
      CollectPhaseSpaceDataJetson_B.Compare[13] ||
      CollectPhaseSpaceDataJetson_B.Compare[14] ||
      CollectPhaseSpaceDataJetson_B.Compare[15]) {
    // SignalConversion generated from: '<S2>/In1' incorporates:
    //   MATLABSystem: '<Root>/Stream PhaseSpace to Platform'

    std::memcpy(&CollectPhaseSpaceDataJetson_B.In1[0],
                &CollectPhaseSpaceDataJetson_B.StreamPhaseSpacetoPlatform[0],
                sizeof(real_T) << 4U);
  }

  // End of Outputs for SubSystem: '<Root>/Enabled Subsystem1'

  // Update for S-Function (sdspToNetwork): '<Root>/UDP Send1' incorporates:
  //   SignalConversion generated from: '<S2>/In1'

  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
  LibUpdate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U],
                    &CollectPhaseSpaceDataJetson_B.In1[0U], 16);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send1'

  // Update for S-Function (sdspToNetwork): '<Root>/UDP Send2' incorporates:
  //   SignalConversion generated from: '<S2>/In1'

  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
  LibUpdate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U],
                    &CollectPhaseSpaceDataJetson_B.In1[0U], 16);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send2'

  // Update for S-Function (sdspToNetwork): '<Root>/UDP Send3' incorporates:
  //   SignalConversion generated from: '<S2>/In1'

  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
  LibUpdate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U],
                    &CollectPhaseSpaceDataJetson_B.In1[0U], 16);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send3'

  // Update for S-Function (sdspToNetwork): '<Root>/UDP Send4' incorporates:
  //   SignalConversion generated from: '<S2>/In1'

  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
  LibUpdate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U],
                    &CollectPhaseSpaceDataJetson_B.In1[0U], 16);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send4'
}

// Model initialize function
void CollectPhaseSpaceDataJetson_initialize(void)
{
  {
    char_T *sErr;

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send1'
    sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U],
                        1, "0.0.0.0", -1, "192.168.0.101",
                        CollectPhaseSpaceDataJetson_P.UDPSend1_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send1'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send2'
    sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U],
                        1, "0.0.0.0", -1, "192.168.0.107",
                        CollectPhaseSpaceDataJetson_P.UDPSend2_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send2'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send3'
    sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U],
                        1, "0.0.0.0", -1, "192.168.0.100",
                        CollectPhaseSpaceDataJetson_P.UDPSend3_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send3'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send4'
    sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U],
                        1, "0.0.0.0", -1, "192.168.0.104",
                        CollectPhaseSpaceDataJetson_P.UDPSend4_remotePort, 8192,
                        8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send4'

    // SystemInitialize for Enabled SubSystem: '<Root>/Enabled Subsystem1'
    for (int32_T i = 0; i < 16; i++) {
      // SystemInitialize for SignalConversion generated from: '<S2>/In1' incorporates:
      //   Outport: '<S2>/Out1'

      CollectPhaseSpaceDataJetson_B.In1[i] =
        CollectPhaseSpaceDataJetson_P.Out1_Y0;
    }

    // End of SystemInitialize for SubSystem: '<Root>/Enabled Subsystem1'

    // Start for MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
    CollectPhaseSpaceDataJetson_DW.obj.matlabCodegenIsDeleted = true;

    //  Constructor
    CollectPhaseSpaceDataJetson_DW.obj.isInitialized = 0;

    //  Support name-value pair arguments when constructing the object.
    CollectPhaseSpaceDataJetson_DW.obj.matlabCodegenIsDeleted = false;
    CollectPhaseSpaceDataJetson_DW.obj.PS_SampleRate =
      CollectPhaseSpaceDataJetson_P.StreamPhaseSpacetoPlatform_PS_S;
    CollectPhaseSpaceDataJetson_DW.obj.isSetupComplete = false;
    CollectPhaseSpaceDataJetson_DW.obj.isInitialized = 1;

    //         %% Define output properties
    //  Call C-function implementing device initialization
    initialize_phasespace(CollectPhaseSpaceDataJetson_DW.obj.PS_SampleRate);
    CollectPhaseSpaceDataJetson_DW.obj.isSetupComplete = true;
  }
}

// Model terminate function
void CollectPhaseSpaceDataJetson_terminate(void)
{
  char_T *sErr;

  // Terminate for MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
  if (!CollectPhaseSpaceDataJetson_DW.obj.matlabCodegenIsDeleted) {
    CollectPhaseSpaceDataJetson_DW.obj.matlabCodegenIsDeleted = true;
    if ((CollectPhaseSpaceDataJetson_DW.obj.isInitialized == 1) &&
        CollectPhaseSpaceDataJetson_DW.obj.isSetupComplete) {
      //  Call C-function implementing device termination
      terminate_phasespace();
    }
  }

  // End of Terminate for MATLABSystem: '<Root>/Stream PhaseSpace to Platform'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send1'
  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend1_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send1'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send2'
  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend2_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send2'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send3'
  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend3_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send3'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send4'
  sErr = GetErrorBuffer(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceDataJetson_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceDataJetson_DW.UDPSend4_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send4'
}

//
// File trailer for generated code.
//
// [EOF]
//
