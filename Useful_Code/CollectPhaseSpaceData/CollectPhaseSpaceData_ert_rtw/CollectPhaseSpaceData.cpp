//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: CollectPhaseSpaceData.cpp
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

// Block signals (default storage)
B_CollectPhaseSpaceData_T CollectPhaseSpaceData_B;

// Block states (default storage)
DW_CollectPhaseSpaceData_T CollectPhaseSpaceData_DW;

// Real-time model
RT_MODEL_CollectPhaseSpaceDat_T CollectPhaseSpaceData_M_ =
  RT_MODEL_CollectPhaseSpaceDat_T();
RT_MODEL_CollectPhaseSpaceDat_T *const CollectPhaseSpaceData_M =
  &CollectPhaseSpaceData_M_;

// Forward declaration for local functions
static void CollectPhase_SystemCore_release(const
  PhaseSpace_ALL_CollectPhaseSp_T *obj);
static void CollectPhaseS_SystemCore_delete(const
  PhaseSpace_ALL_CollectPhaseSp_T *obj);
static void matlabCodegenHandle_matlabCodeg(PhaseSpace_ALL_CollectPhaseSp_T *obj);
static void CollectPhase_SystemCore_release(const
  PhaseSpace_ALL_CollectPhaseSp_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    terminate_phasespace();
  }
}

static void CollectPhaseS_SystemCore_delete(const
  PhaseSpace_ALL_CollectPhaseSp_T *obj)
{
  CollectPhase_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(PhaseSpace_ALL_CollectPhaseSp_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    CollectPhaseS_SystemCore_delete(obj);
  }
}

// Model step function
void CollectPhaseSpaceData_step(void)
{
  {
    real_T y9;
    real_T y10;
    real_T y11;
    real_T y12;
    real_T y13;
    real_T y14;
    real_T y15;
    int32_T i;

    // MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
    if (CollectPhaseSpaceData_DW.obj.PS_SampleRate !=
        CollectPhaseSpaceData_P.StreamPhaseSpacetoPlatform_PS_S) {
      CollectPhaseSpaceData_DW.obj.PS_SampleRate =
        CollectPhaseSpaceData_P.StreamPhaseSpacetoPlatform_PS_S;
    }

    CollectPhaseSpaceData_B.b_y1 = 0.0;
    CollectPhaseSpaceData_B.y2 = 0.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[3] = 0.0;
    CollectPhaseSpaceData_B.y4 = 0.0;
    CollectPhaseSpaceData_B.y5 = 0.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[6] = 0.0;
    CollectPhaseSpaceData_B.y7 = 0.0;
    CollectPhaseSpaceData_B.y8 = 0.0;
    y9 = 0.0;
    y10 = 0.0;
    y11 = 0.0;
    y12 = 0.0;
    y13 = 0.0;
    y14 = 0.0;
    y15 = 0.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[15] = 0.0;
    stream_phasespace(&CollectPhaseSpaceData_B.b_y1, &CollectPhaseSpaceData_B.y2,
                      &CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[3],
                      &CollectPhaseSpaceData_B.y4, &CollectPhaseSpaceData_B.y5,
                      &CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[6],
                      &CollectPhaseSpaceData_B.y7, &CollectPhaseSpaceData_B.y8,
                      &y9, &y10, &y11, &y12, &y13, &y14, &y15,
                      &CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[15]);
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[0] =
      CollectPhaseSpaceData_B.y7 / CollectPhaseSpaceData_DW.obj.PS_SampleRate;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[1] =
      CollectPhaseSpaceData_B.b_y1 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[2] =
      CollectPhaseSpaceData_B.y2 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[4] =
      CollectPhaseSpaceData_B.y4 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[5] =
      CollectPhaseSpaceData_B.y5 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[7] =
      CollectPhaseSpaceData_B.y8 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[8] = y9 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[9] = y10 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[10] = y11 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[11] = y12 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[12] = y13 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[13] = y14 / 1000.0;
    CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[14] = y15 / 1000.0;

    // End of MATLABSystem: '<Root>/Stream PhaseSpace to Platform'

    // RelationalOperator: '<S1>/Compare' incorporates:
    //   Constant: '<S1>/Constant'

    for (i = 0; i < 16; i++) {
      CollectPhaseSpaceData_B.rtb_Compare_m[i] =
        (CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[i] !=
         CollectPhaseSpaceData_P.Constant_Value);
    }

    // End of RelationalOperator: '<S1>/Compare'

    // Outputs for Enabled SubSystem: '<Root>/Enabled Subsystem1' incorporates:
    //   EnablePort: '<S3>/Enable'

    // Outputs for Enabled SubSystem: '<Root>/Enabled Subsystem2' incorporates:
    //   EnablePort: '<S4>/Enable'

    if (CollectPhaseSpaceData_B.rtb_Compare_m[0] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[1] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[2] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[3] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[4] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[5] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[6] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[7] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[8] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[9] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[10] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[11] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[12] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[13] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[14] ||
        CollectPhaseSpaceData_B.rtb_Compare_m[15]) {
      // Inport: '<S4>/In1' incorporates:
      //   Clock: '<Root>/Clock'

      CollectPhaseSpaceData_B.In1 = CollectPhaseSpaceData_M->Timing.t[0];

      // Inport: '<S3>/In1'
      std::memcpy(&CollectPhaseSpaceData_B.In1_b[0],
                  &CollectPhaseSpaceData_B.StreamPhaseSpacetoPlatform[0], sizeof
                  (real_T) << 4U);
    }

    // End of Outputs for SubSystem: '<Root>/Enabled Subsystem2'
    // End of Outputs for SubSystem: '<Root>/Enabled Subsystem1'

    // DataTypeConversion: '<S5>/Data Type Conversion'
    CollectPhaseSpaceData_B.DataTypeConversion = (CollectPhaseSpaceData_B.In1 !=
      0.0);

    // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
    //   EnablePort: '<S6>/Enable'

    // Logic: '<S5>/Logical Operator1' incorporates:
    //   Delay: '<S5>/Delay'
    //   Inport: '<S6>/In1'
    //   Logic: '<S5>/Logical Operator'

    if (CollectPhaseSpaceData_B.DataTypeConversion &&
        (!CollectPhaseSpaceData_DW.Delay_DSTATE)) {
      CollectPhaseSpaceData_B.In1_bn = CollectPhaseSpaceData_B.In1;
    }

    // End of Logic: '<S5>/Logical Operator1'
    // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

    // SignalConversion generated from: '<Root>/UDP Send' incorporates:
    //   Sum: '<S2>/Subtract'

    CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp[0] =
      CollectPhaseSpaceData_B.In1 - CollectPhaseSpaceData_B.In1_bn;
    std::memcpy(&CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp[1],
                &CollectPhaseSpaceData_B.In1_b[1], 15U * sizeof(real_T));
  }

  {
    char_T *sErr;

    // Update for Delay: '<S5>/Delay'
    CollectPhaseSpaceData_DW.Delay_DSTATE =
      CollectPhaseSpaceData_B.DataTypeConversion;

    // Update for S-Function (sdspToNetwork): '<Root>/UDP Send'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
    LibUpdate_Network(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U],
                      &CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp
                      [0U], 16);
    if (*sErr != 0) {
      rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
      rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send'

    // Update for S-Function (sdspToNetwork): '<Root>/UDP Send1'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
    LibUpdate_Network(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U],
                      &CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp
                      [0U], 16);
    if (*sErr != 0) {
      rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
      rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send1'

    // Update for S-Function (sdspToNetwork): '<Root>/UDP Send2'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
    LibUpdate_Network(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U],
                      &CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp
                      [0U], 16);
    if (*sErr != 0) {
      rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
      rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send2'

    // Update for S-Function (sdspToNetwork): '<Root>/UDP Send4'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
    LibUpdate_Network(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U],
                      &CollectPhaseSpaceData_B.TmpSignalConversionAtUDPSendInp
                      [0U], 16);
    if (*sErr != 0) {
      rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
      rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<Root>/UDP Send4'
  }

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  CollectPhaseSpaceData_M->Timing.t[0] =
    ((time_T)(++CollectPhaseSpaceData_M->Timing.clockTick0)) *
    CollectPhaseSpaceData_M->Timing.stepSize0;

  {
    // Update absolute timer for sample time: [0.1s, 0.0s]
    // The "clockTick1" counts the number of times the code of this task has
    //  been executed. The resolution of this integer timer is 0.1, which is the step size
    //  of the task. Size of "clockTick1" ensures timer will not overflow during the
    //  application lifespan selected.

    CollectPhaseSpaceData_M->Timing.clockTick1++;
  }
}

// Model initialize function
void CollectPhaseSpaceData_initialize(void)
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&CollectPhaseSpaceData_M->solverInfo,
                          &CollectPhaseSpaceData_M->Timing.simTimeStep);
    rtsiSetTPtr(&CollectPhaseSpaceData_M->solverInfo, &rtmGetTPtr
                (CollectPhaseSpaceData_M));
    rtsiSetStepSizePtr(&CollectPhaseSpaceData_M->solverInfo,
                       &CollectPhaseSpaceData_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&CollectPhaseSpaceData_M->solverInfo,
                          (&rtmGetErrorStatus(CollectPhaseSpaceData_M)));
    rtsiSetRTModelPtr(&CollectPhaseSpaceData_M->solverInfo,
                      CollectPhaseSpaceData_M);
  }

  rtsiSetSimTimeStep(&CollectPhaseSpaceData_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&CollectPhaseSpaceData_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(CollectPhaseSpaceData_M, &CollectPhaseSpaceData_M->Timing.tArray[0]);
  CollectPhaseSpaceData_M->Timing.stepSize0 = 0.1;

  {
    char_T *sErr;
    int32_T i;

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.100",
                        CollectPhaseSpaceData_P.UDPSend_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send1'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.101",
                        CollectPhaseSpaceData_P.UDPSend1_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send1'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send2'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.102",
                        CollectPhaseSpaceData_P.UDPSend2_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send2'

    // Start for S-Function (sdspToNetwork): '<Root>/UDP Send4'
    sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
    CreateUDPInterface(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.104",
                        CollectPhaseSpaceData_P.UDPSend4_remotePort, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
        rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<Root>/UDP Send4'

    // InitializeConditions for Delay: '<S5>/Delay'
    CollectPhaseSpaceData_DW.Delay_DSTATE =
      CollectPhaseSpaceData_P.Delay_InitialCondition;

    // SystemInitialize for Enabled SubSystem: '<Root>/Enabled Subsystem2'
    // SystemInitialize for Outport: '<S4>/Actual Time'
    CollectPhaseSpaceData_B.In1 = CollectPhaseSpaceData_P.ActualTime_Y0;

    // End of SystemInitialize for SubSystem: '<Root>/Enabled Subsystem2'

    // SystemInitialize for Enabled SubSystem: '<Root>/Enabled Subsystem1'
    // SystemInitialize for Outport: '<S3>/Out1'
    for (i = 0; i < 16; i++) {
      CollectPhaseSpaceData_B.In1_b[i] = CollectPhaseSpaceData_P.Out1_Y0_h;
    }

    // End of SystemInitialize for Outport: '<S3>/Out1'
    // End of SystemInitialize for SubSystem: '<Root>/Enabled Subsystem1'

    // SystemInitialize for Enabled SubSystem: '<S2>/Enabled Subsystem'
    // SystemInitialize for Outport: '<S6>/Out1'
    CollectPhaseSpaceData_B.In1_bn = CollectPhaseSpaceData_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S2>/Enabled Subsystem'

    // Start for MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
    CollectPhaseSpaceData_DW.obj.matlabCodegenIsDeleted = true;
    CollectPhaseSpaceData_DW.obj.isInitialized = 0;
    CollectPhaseSpaceData_DW.obj.matlabCodegenIsDeleted = false;
    CollectPhaseSpaceData_DW.obj.PS_SampleRate =
      CollectPhaseSpaceData_P.StreamPhaseSpacetoPlatform_PS_S;
    CollectPhaseSpaceData_DW.obj.isSetupComplete = false;
    CollectPhaseSpaceData_DW.obj.isInitialized = 1;
    initialize_phasespace(CollectPhaseSpaceData_DW.obj.PS_SampleRate);
    CollectPhaseSpaceData_DW.obj.isSetupComplete = true;
  }
}

// Model terminate function
void CollectPhaseSpaceData_terminate(void)
{
  char_T *sErr;

  // Terminate for MATLABSystem: '<Root>/Stream PhaseSpace to Platform'
  matlabCodegenHandle_matlabCodeg(&CollectPhaseSpaceData_DW.obj);

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send'
  sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send1'
  sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend1_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send1'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send2'
  sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend2_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send2'

  // Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send4'
  sErr = GetErrorBuffer(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
  LibTerminate(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(CollectPhaseSpaceData_M, sErr);
    rtmSetStopRequested(CollectPhaseSpaceData_M, 1);
  }

  LibDestroy(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U], 1);
  DestroyUDPInterface(&CollectPhaseSpaceData_DW.UDPSend4_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<Root>/UDP Send4'
}

//
// File trailer for generated code.
//
// [EOF]
//
