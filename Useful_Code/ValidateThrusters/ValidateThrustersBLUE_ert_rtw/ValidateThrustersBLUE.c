/*
 * ValidateThrustersBLUE.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "ValidateThrustersBLUE".
 *
 * Model version              : 1.18
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C source code generated on : Fri Nov 11 15:42:15 2022
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ValidateThrustersBLUE.h"
#include "ValidateThrustersBLUE_private.h"
#include "ValidateThrustersBLUE_dt.h"

/* Block states (default storage) */
DW_ValidateThrustersBLUE_T ValidateThrustersBLUE_DW;

/* Real-time model */
RT_MODEL_ValidateThrustersBLU_T ValidateThrustersBLUE_M_;
RT_MODEL_ValidateThrustersBLU_T *const ValidateThrustersBLUE_M =
  &ValidateThrustersBLUE_M_;

/* Forward declaration for local functions */
static void ValidateThru_SystemCore_release(const
  raspi_internal_PWMBlock_Valid_T *obj);
static void ValidateThrus_SystemCore_delete(const
  raspi_internal_PWMBlock_Valid_T *obj);
static void matlabCodegenHandle_matlabCodeg(raspi_internal_PWMBlock_Valid_T *obj);
static void ValidateThru_SystemCore_release(const
  raspi_internal_PWMBlock_Valid_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    EXT_PWMBlock_terminate(obj->PinNumber);
  }
}

static void ValidateThrus_SystemCore_delete(const
  raspi_internal_PWMBlock_Valid_T *obj)
{
  ValidateThru_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(raspi_internal_PWMBlock_Valid_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    ValidateThrus_SystemCore_delete(obj);
  }
}

/* Model step function */
void ValidateThrustersBLUE_step(void)
{
  real_T y;

  /* MATLABSystem: '<Root>/RED T1 - BLACK T8' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (ValidateThrustersBLUE_P.Constant_Value < 1.0) {
    y = ValidateThrustersBLUE_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLUE_DW.obj.PinNumber, y);

  /* End of MATLABSystem: '<Root>/RED T1 - BLACK T8' */

  /* External mode */
  rtExtModeUploadCheckTrigger(1);

  {                                    /* Sample time: [0.1s, 0.0s] */
    rtExtModeUpload(0, (real_T)ValidateThrustersBLUE_M->Timing.taskTime0);
  }

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.1s, 0.0s] */
    if ((rtmGetTFinal(ValidateThrustersBLUE_M)!=-1) &&
        !((rtmGetTFinal(ValidateThrustersBLUE_M)-
           ValidateThrustersBLUE_M->Timing.taskTime0) >
          ValidateThrustersBLUE_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(ValidateThrustersBLUE_M, "Simulation finished");
    }

    if (rtmGetStopRequested(ValidateThrustersBLUE_M)) {
      rtmSetErrorStatus(ValidateThrustersBLUE_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++ValidateThrustersBLUE_M->Timing.clockTick0)) {
    ++ValidateThrustersBLUE_M->Timing.clockTickH0;
  }

  ValidateThrustersBLUE_M->Timing.taskTime0 =
    ValidateThrustersBLUE_M->Timing.clockTick0 *
    ValidateThrustersBLUE_M->Timing.stepSize0 +
    ValidateThrustersBLUE_M->Timing.clockTickH0 *
    ValidateThrustersBLUE_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void ValidateThrustersBLUE_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)ValidateThrustersBLUE_M, 0,
                sizeof(RT_MODEL_ValidateThrustersBLU_T));
  rtmSetTFinal(ValidateThrustersBLUE_M, 5.0);
  ValidateThrustersBLUE_M->Timing.stepSize0 = 0.1;

  /* External mode info */
  ValidateThrustersBLUE_M->Sizes.checksums[0] = (559388860U);
  ValidateThrustersBLUE_M->Sizes.checksums[1] = (803544491U);
  ValidateThrustersBLUE_M->Sizes.checksums[2] = (4214768462U);
  ValidateThrustersBLUE_M->Sizes.checksums[3] = (1730279659U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[2];
    ValidateThrustersBLUE_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(ValidateThrustersBLUE_M->extModeInfo,
      &ValidateThrustersBLUE_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(ValidateThrustersBLUE_M->extModeInfo,
                        ValidateThrustersBLUE_M->Sizes.checksums);
    rteiSetTPtr(ValidateThrustersBLUE_M->extModeInfo, rtmGetTPtr
                (ValidateThrustersBLUE_M));
  }

  /* states (dwork) */
  (void) memset((void *)&ValidateThrustersBLUE_DW, 0,
                sizeof(DW_ValidateThrustersBLUE_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    ValidateThrustersBLUE_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 15;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  {
    int32_T ret;
    static const char_T tmp[45] =
      "Unable to configure pin %u for PWM output.\\n";
    char_T tmp_0[45];

    /* Start for MATLABSystem: '<Root>/RED T1 - BLACK T8' */
    ValidateThrustersBLUE_DW.obj.matlabCodegenIsDeleted = false;
    ValidateThrustersBLUE_DW.objisempty = true;
    ValidateThrustersBLUE_DW.obj.isInitialized = 1;
    ValidateThrustersBLUE_DW.obj.PinNumber = 17U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLUE_DW.obj.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLUE_DW.obj.PinNumber);
    }

    ValidateThrustersBLUE_DW.obj.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<Root>/RED T1 - BLACK T8' */
  }
}

/* Model terminate function */
void ValidateThrustersBLUE_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/RED T1 - BLACK T8' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLUE_DW.obj);
}
