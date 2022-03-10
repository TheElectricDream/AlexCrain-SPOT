/*
 * ValidateThrustersBLACK.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "ValidateThrustersBLACK".
 *
 * Model version              : 1.13
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C source code generated on : Thu Mar 10 09:57:15 2022
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ValidateThrustersBLACK.h"
#include "ValidateThrustersBLACK_private.h"

/* Block signals (default storage) */
B_ValidateThrustersBLACK_T ValidateThrustersBLACK_B;

/* Block states (default storage) */
DW_ValidateThrustersBLACK_T ValidateThrustersBLACK_DW;

/* Real-time model */
RT_MODEL_ValidateThrustersBLA_T ValidateThrustersBLACK_M_;
RT_MODEL_ValidateThrustersBLA_T *const ValidateThrustersBLACK_M =
  &ValidateThrustersBLACK_M_;

/* Forward declaration for local functions */
static void ValidateThru_SystemCore_release(const
  raspi_internal_PWMBlock_Valid_T *obj);
static void ValidateThrus_SystemCore_delete(const
  raspi_internal_PWMBlock_Valid_T *obj);
static void matlabCodegenHandle_matlabCodeg(raspi_internal_PWMBlock_Valid_T *obj);

/*
 * Output and update for action system:
 *    '<S1>/If Action Subsystem'
 *    '<S1>/If Action Subsystem1'
 *    '<S1>/If Action Subsystem2'
 *    '<S1>/If Action Subsystem3'
 *    '<S1>/If Action  Subsystem4'
 *    '<S1>/If Action Subsystem5'
 *    '<S1>/If Action Subsystem6'
 *    '<S1>/If Action Subsystem7'
 *    '<S2>/If Action Subsystem'
 *    '<S2>/If Action Subsystem1'
 *    ...
 */
void ValidateThrus_IfActionSubsystem(real_T rty_PWM[8],
  P_IfActionSubsystem_ValidateT_T *localP)
{
  /* SignalConversion generated from: '<S6>/PWM' incorporates:
   *  Constant: '<S6>/Constant'
   */
  memcpy(&rty_PWM[0], &localP->Constant_Value[0], sizeof(real_T) << 3U);
}

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
void ValidateThrustersBLACK_step(void)
{
  real_T rtb_Clock;

  /* If: '<Root>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
   *  Constant: '<Root>/Constant8'
   *  Constant: '<S4>/Constant'
   */
  ValidateThrustersBLACK_DW.ThisIFblockdetermineswhetherorn = -1;
  if ((ValidateThrustersBLACK_P.Constant8_Value == 1.0) ||
      (ValidateThrustersBLACK_P.Constant_Value == 1.0)) {
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhetherorn = 0;

    /* Outputs for IfAction SubSystem: '<Root>/Change RED Behavior' incorporates:
     *  ActionPort: '<S3>/Action Port'
     */
    /* Clock: '<S3>/Clock' */
    rtb_Clock = ValidateThrustersBLACK_M->Timing.t[0];

    /* If: '<S3>/If' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Constant: '<S3>/Constant3'
     *  Constant: '<S3>/Constant4'
     *  Constant: '<S3>/Constant5'
     *  Constant: '<S3>/Constant6'
     *  Constant: '<S3>/Constant7'
     */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem = -1;
    if (rtb_Clock < ValidateThrustersBLACK_P.Thruster1_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 0;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
       *  ActionPort: '<S22>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem_m);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster2_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 1;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem1' incorporates:
       *  ActionPort: '<S23>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem1_n);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem1' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster3_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 2;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem2' incorporates:
       *  ActionPort: '<S24>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem2_i);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem2' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster4_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 3;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem3' incorporates:
       *  ActionPort: '<S25>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem3_i);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem3' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster5_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 4;

      /* Outputs for IfAction SubSystem: '<S3>/If Action  Subsystem4' incorporates:
       *  ActionPort: '<S29>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem4_fu);

      /* End of Outputs for SubSystem: '<S3>/If Action  Subsystem4' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster6_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 5;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem5' incorporates:
       *  ActionPort: '<S26>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem5_g);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem5' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster7_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem = 6;

      /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem6' incorporates:
       *  ActionPort: '<S27>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
        &ValidateThrustersBLACK_P.IfActionSubsystem6_hk);

      /* End of Outputs for SubSystem: '<S3>/If Action Subsystem6' */
    } else {
      if (rtb_Clock < ValidateThrustersBLACK_P.Thruster8_OnTime) {
        ValidateThrustersBLACK_DW.If_ActiveSubsystem = 7;

        /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem7' incorporates:
         *  ActionPort: '<S28>/Action Port'
         */
        ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge,
          &ValidateThrustersBLACK_P.IfActionSubsystem7_j);

        /* End of Outputs for SubSystem: '<S3>/If Action Subsystem7' */
      }
    }

    /* End of If: '<S3>/If' */
    /* End of Outputs for SubSystem: '<Root>/Change RED Behavior' */
  }

  /* End of If: '<Root>/This IF block determines whether or not to run the RED sim//exp ' */

  /* If: '<Root>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
   *  Constant: '<Root>/Constant8'
   *  Constant: '<S4>/Constant'
   */
  ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_d = -1;
  if ((ValidateThrustersBLACK_P.Constant8_Value == 2.0) ||
      (ValidateThrustersBLACK_P.Constant_Value == 1.0)) {
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_d = 0;

    /* Outputs for IfAction SubSystem: '<Root>/Change BLACK Behavior' incorporates:
     *  ActionPort: '<S1>/Action Port'
     */
    /* Clock: '<S1>/Clock' */
    rtb_Clock = ValidateThrustersBLACK_M->Timing.t[0];

    /* If: '<S1>/If' incorporates:
     *  Constant: '<S1>/Constant'
     *  Constant: '<S1>/Constant1'
     *  Constant: '<S1>/Constant2'
     *  Constant: '<S1>/Constant3'
     *  Constant: '<S1>/Constant4'
     *  Constant: '<S1>/Constant5'
     *  Constant: '<S1>/Constant6'
     *  Constant: '<S1>/Constant7'
     */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = -1;
    if (rtb_Clock < ValidateThrustersBLACK_P.Thruster1_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 0;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem' incorporates:
       *  ActionPort: '<S6>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster2_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 1;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem1' incorporates:
       *  ActionPort: '<S7>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem1);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem1' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster3_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 2;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem2' incorporates:
       *  ActionPort: '<S8>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem2);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem2' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster4_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 3;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem3' incorporates:
       *  ActionPort: '<S9>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem3);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem3' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster5_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 4;

      /* Outputs for IfAction SubSystem: '<S1>/If Action  Subsystem4' incorporates:
       *  ActionPort: '<S13>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem4);

      /* End of Outputs for SubSystem: '<S1>/If Action  Subsystem4' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster6_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 5;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem5' incorporates:
       *  ActionPort: '<S10>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem5);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem5' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster7_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 6;

      /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem6' incorporates:
       *  ActionPort: '<S11>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
        &ValidateThrustersBLACK_P.IfActionSubsystem6);

      /* End of Outputs for SubSystem: '<S1>/If Action Subsystem6' */
    } else {
      if (rtb_Clock < ValidateThrustersBLACK_P.Thruster8_OnTime) {
        ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = 7;

        /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem7' incorporates:
         *  ActionPort: '<S12>/Action Port'
         */
        ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_c,
          &ValidateThrustersBLACK_P.IfActionSubsystem7);

        /* End of Outputs for SubSystem: '<S1>/If Action Subsystem7' */
      }
    }

    /* End of If: '<S1>/If' */
    /* End of Outputs for SubSystem: '<Root>/Change BLACK Behavior' */
  }

  /* End of If: '<Root>/This IF block determines whether or not to run the BLACK sim//exp' */

  /* If: '<Root>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
   *  Constant: '<Root>/Constant8'
   *  Constant: '<S4>/Constant'
   */
  ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_i = -1;
  if ((ValidateThrustersBLACK_P.Constant8_Value == 3.0) ||
      (ValidateThrustersBLACK_P.Constant_Value == 1.0)) {
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_i = 0;

    /* Outputs for IfAction SubSystem: '<Root>/Change BLUE Behavior' incorporates:
     *  ActionPort: '<S2>/Action Port'
     */
    /* Clock: '<S2>/Clock' */
    rtb_Clock = ValidateThrustersBLACK_M->Timing.t[0];

    /* If: '<S2>/If' incorporates:
     *  Constant: '<S2>/Constant'
     *  Constant: '<S2>/Constant1'
     *  Constant: '<S2>/Constant2'
     *  Constant: '<S2>/Constant3'
     *  Constant: '<S2>/Constant4'
     *  Constant: '<S2>/Constant5'
     *  Constant: '<S2>/Constant6'
     *  Constant: '<S2>/Constant7'
     */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = -1;
    if (rtb_Clock < ValidateThrustersBLACK_P.Thruster1_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 0;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem' incorporates:
       *  ActionPort: '<S14>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem_d);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster2_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 1;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
       *  ActionPort: '<S15>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem1_p);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem1' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster3_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 2;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem2' incorporates:
       *  ActionPort: '<S16>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem2_d);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem2' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster4_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 3;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem3' incorporates:
       *  ActionPort: '<S17>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem3_h);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem3' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster5_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 4;

      /* Outputs for IfAction SubSystem: '<S2>/If Action  Subsystem4' incorporates:
       *  ActionPort: '<S21>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem4_f);

      /* End of Outputs for SubSystem: '<S2>/If Action  Subsystem4' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster6_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 5;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem5' incorporates:
       *  ActionPort: '<S18>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem5_b);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem5' */
    } else if (rtb_Clock < ValidateThrustersBLACK_P.Thruster7_OnTime) {
      ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 6;

      /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem6' incorporates:
       *  ActionPort: '<S19>/Action Port'
       */
      ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
        &ValidateThrustersBLACK_P.IfActionSubsystem6_h);

      /* End of Outputs for SubSystem: '<S2>/If Action Subsystem6' */
    } else {
      if (rtb_Clock < ValidateThrustersBLACK_P.Thruster8_OnTime) {
        ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = 7;

        /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem7' incorporates:
         *  ActionPort: '<S20>/Action Port'
         */
        ValidateThrus_IfActionSubsystem(ValidateThrustersBLACK_B.Merge_a,
          &ValidateThrustersBLACK_P.IfActionSubsystem7_p);

        /* End of Outputs for SubSystem: '<S2>/If Action Subsystem7' */
      }
    }

    /* End of If: '<S2>/If' */
    /* End of Outputs for SubSystem: '<Root>/Change BLUE Behavior' */
  }

  /* End of If: '<Root>/This IF block determines whether or not to run the BLUE sim//exp' */

  /* Sum: '<Root>/Sum' */
  ValidateThrustersBLACK_B.Sum = (ValidateThrustersBLACK_B.Merge[0] +
    ValidateThrustersBLACK_B.Merge_c[7]) + ValidateThrustersBLACK_B.Merge_a[7];

  /* Sum: '<Root>/Sum1' */
  ValidateThrustersBLACK_B.Sum1 = (ValidateThrustersBLACK_B.Merge[1] +
    ValidateThrustersBLACK_B.Merge_c[2]) + ValidateThrustersBLACK_B.Merge_a[2];

  /* Sum: '<Root>/Sum2' */
  ValidateThrustersBLACK_B.Sum2 = (ValidateThrustersBLACK_B.Merge[3] +
    ValidateThrustersBLACK_B.Merge_c[4]) + ValidateThrustersBLACK_B.Merge_a[4];

  /* Sum: '<Root>/Sum3' */
  ValidateThrustersBLACK_B.Sum3 = (ValidateThrustersBLACK_B.Merge[4] +
    ValidateThrustersBLACK_B.Merge_c[3]) + ValidateThrustersBLACK_B.Merge_a[3];

  /* Sum: '<Root>/Sum4' */
  ValidateThrustersBLACK_B.Sum4 = (ValidateThrustersBLACK_B.Merge[5] +
    ValidateThrustersBLACK_B.Merge_c[6]) + ValidateThrustersBLACK_B.Merge_a[6];

  /* Sum: '<Root>/Sum5' */
  ValidateThrustersBLACK_B.Sum5 = (ValidateThrustersBLACK_B.Merge[6] +
    ValidateThrustersBLACK_B.Merge_c[5]) + ValidateThrustersBLACK_B.Merge_a[5];

  /* Sum: '<Root>/Sum6' */
  ValidateThrustersBLACK_B.Sum6 = (ValidateThrustersBLACK_B.Merge[7] +
    ValidateThrustersBLACK_B.Merge_c[0]) + ValidateThrustersBLACK_B.Merge_a[0];

  /* Sum: '<Root>/Sum7' */
  ValidateThrustersBLACK_B.Sum7 = ValidateThrustersBLACK_B.Merge_c[1] +
    ValidateThrustersBLACK_B.Merge_a[1];

  /* Outputs for Atomic SubSystem: '<Root>/Send Commands to PWM Blocks' */
  /* MATLABSystem: '<S5>/RED T1 - BLACK T8' */
  if (ValidateThrustersBLACK_B.Sum < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T1 - BLACK T8' */

  /* MATLABSystem: '<S5>/RED T2 - BLACK T3' */
  if (ValidateThrustersBLACK_B.Sum1 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum1;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_m.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T2 - BLACK T3' */

  /* MATLABSystem: '<S5>/RED T3' */
  if (ValidateThrustersBLACK_B.Merge[2] < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Merge[2];
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_f.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T3' */

  /* MATLABSystem: '<S5>/RED T4 - BLACK T5' */
  if (ValidateThrustersBLACK_B.Sum2 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum2;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_fe.PinNumber,
    rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T4 - BLACK T5' */

  /* MATLABSystem: '<S5>/RED T5 - BLACK T4' */
  if (ValidateThrustersBLACK_B.Sum3 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum3;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_p.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T5 - BLACK T4' */

  /* MATLABSystem: '<S5>/RED T6 - BLACK T7' */
  if (ValidateThrustersBLACK_B.Sum4 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum4;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_a.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T6 - BLACK T7' */

  /* MATLABSystem: '<S5>/RED T7 - BLACK T6' */
  if (ValidateThrustersBLACK_B.Sum5 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum5;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_k.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T7 - BLACK T6' */

  /* MATLABSystem: '<S5>/RED T8 - BLACK T1' */
  if (ValidateThrustersBLACK_B.Sum6 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum6;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_e.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/RED T8 - BLACK T1' */

  /* MATLABSystem: '<S5>/BLACK T2' */
  if (ValidateThrustersBLACK_B.Sum7 < 1.0) {
    rtb_Clock = ValidateThrustersBLACK_B.Sum7;
  } else {
    rtb_Clock = 1.0;
  }

  if (!(rtb_Clock > 0.0)) {
    rtb_Clock = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(ValidateThrustersBLACK_DW.obj_i.PinNumber, rtb_Clock);

  /* End of MATLABSystem: '<S5>/BLACK T2' */
  /* End of Outputs for SubSystem: '<Root>/Send Commands to PWM Blocks' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++ValidateThrustersBLACK_M->Timing.clockTick0)) {
    ++ValidateThrustersBLACK_M->Timing.clockTickH0;
  }

  ValidateThrustersBLACK_M->Timing.t[0] =
    ValidateThrustersBLACK_M->Timing.clockTick0 *
    ValidateThrustersBLACK_M->Timing.stepSize0 +
    ValidateThrustersBLACK_M->Timing.clockTickH0 *
    ValidateThrustersBLACK_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.1s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.1, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    ValidateThrustersBLACK_M->Timing.clockTick1++;
    if (!ValidateThrustersBLACK_M->Timing.clockTick1) {
      ValidateThrustersBLACK_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void ValidateThrustersBLACK_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)ValidateThrustersBLACK_M, 0,
                sizeof(RT_MODEL_ValidateThrustersBLA_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&ValidateThrustersBLACK_M->solverInfo,
                          &ValidateThrustersBLACK_M->Timing.simTimeStep);
    rtsiSetTPtr(&ValidateThrustersBLACK_M->solverInfo, &rtmGetTPtr
                (ValidateThrustersBLACK_M));
    rtsiSetStepSizePtr(&ValidateThrustersBLACK_M->solverInfo,
                       &ValidateThrustersBLACK_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&ValidateThrustersBLACK_M->solverInfo,
                          (&rtmGetErrorStatus(ValidateThrustersBLACK_M)));
    rtsiSetRTModelPtr(&ValidateThrustersBLACK_M->solverInfo,
                      ValidateThrustersBLACK_M);
  }

  rtsiSetSimTimeStep(&ValidateThrustersBLACK_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&ValidateThrustersBLACK_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(ValidateThrustersBLACK_M, &ValidateThrustersBLACK_M->Timing.tArray
             [0]);
  ValidateThrustersBLACK_M->Timing.stepSize0 = 0.1;

  /* block I/O */
  (void) memset(((void *) &ValidateThrustersBLACK_B), 0,
                sizeof(B_ValidateThrustersBLACK_T));

  /* states (dwork) */
  (void) memset((void *)&ValidateThrustersBLACK_DW, 0,
                sizeof(DW_ValidateThrustersBLACK_T));

  {
    int32_T ret;
    static const char_T tmp[45] =
      "Unable to configure pin %u for PWM output.\\n";
    char_T tmp_0[45];

    /* Start for If: '<Root>/This IF block determines whether or not to run the RED sim//exp ' */
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhetherorn = -1;

    /* Start for IfAction SubSystem: '<Root>/Change RED Behavior' */
    /* Start for If: '<S3>/If' */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem = -1;

    /* End of Start for SubSystem: '<Root>/Change RED Behavior' */

    /* Start for If: '<Root>/This IF block determines whether or not to run the BLACK sim//exp' */
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_d = -1;

    /* Start for IfAction SubSystem: '<Root>/Change BLACK Behavior' */
    /* Start for If: '<S1>/If' */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem_m = -1;

    /* End of Start for SubSystem: '<Root>/Change BLACK Behavior' */

    /* Start for If: '<Root>/This IF block determines whether or not to run the BLUE sim//exp' */
    ValidateThrustersBLACK_DW.ThisIFblockdetermineswhethero_i = -1;

    /* Start for IfAction SubSystem: '<Root>/Change BLUE Behavior' */
    /* Start for If: '<S2>/If' */
    ValidateThrustersBLACK_DW.If_ActiveSubsystem_f = -1;

    /* End of Start for SubSystem: '<Root>/Change BLUE Behavior' */

    /* Start for Atomic SubSystem: '<Root>/Send Commands to PWM Blocks' */
    /* Start for MATLABSystem: '<S5>/RED T1 - BLACK T8' */
    ValidateThrustersBLACK_DW.obj.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty = true;
    ValidateThrustersBLACK_DW.obj.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj.PinNumber = 27U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T1 - BLACK T8' */

    /* Start for MATLABSystem: '<S5>/RED T2 - BLACK T3' */
    ValidateThrustersBLACK_DW.obj_m.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_p = true;
    ValidateThrustersBLACK_DW.obj_m.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_m.PinNumber = 19U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_m.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_m.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_m.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T2 - BLACK T3' */

    /* Start for MATLABSystem: '<S5>/RED T3' */
    ValidateThrustersBLACK_DW.obj_f.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_pn = true;
    ValidateThrustersBLACK_DW.obj_f.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_f.PinNumber = 4U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_f.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_f.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_f.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T3' */

    /* Start for MATLABSystem: '<S5>/RED T4 - BLACK T5' */
    ValidateThrustersBLACK_DW.obj_fe.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_j = true;
    ValidateThrustersBLACK_DW.obj_fe.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_fe.PinNumber = 22U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_fe.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_fe.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_fe.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T4 - BLACK T5' */

    /* Start for MATLABSystem: '<S5>/RED T5 - BLACK T4' */
    ValidateThrustersBLACK_DW.obj_p.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_m = true;
    ValidateThrustersBLACK_DW.obj_p.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_p.PinNumber = 5U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_p.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_p.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_p.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T5 - BLACK T4' */

    /* Start for MATLABSystem: '<S5>/RED T6 - BLACK T7' */
    ValidateThrustersBLACK_DW.obj_a.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_e = true;
    ValidateThrustersBLACK_DW.obj_a.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_a.PinNumber = 6U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_a.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_a.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_a.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T6 - BLACK T7' */

    /* Start for MATLABSystem: '<S5>/RED T7 - BLACK T6' */
    ValidateThrustersBLACK_DW.obj_k.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_n = true;
    ValidateThrustersBLACK_DW.obj_k.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_k.PinNumber = 13U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_k.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_k.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_k.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T7 - BLACK T6' */

    /* Start for MATLABSystem: '<S5>/RED T8 - BLACK T1' */
    ValidateThrustersBLACK_DW.obj_e.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_b = true;
    ValidateThrustersBLACK_DW.obj_e.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_e.PinNumber = 17U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_e.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_e.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_e.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/RED T8 - BLACK T1' */

    /* Start for MATLABSystem: '<S5>/BLACK T2' */
    ValidateThrustersBLACK_DW.obj_i.matlabCodegenIsDeleted = false;
    ValidateThrustersBLACK_DW.objisempty_ew = true;
    ValidateThrustersBLACK_DW.obj_i.isInitialized = 1;
    ValidateThrustersBLACK_DW.obj_i.PinNumber = 9U;
    ret = EXT_PWMBlock_init(ValidateThrustersBLACK_DW.obj_i.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, ValidateThrustersBLACK_DW.obj_i.PinNumber);
    }

    ValidateThrustersBLACK_DW.obj_i.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S5>/BLACK T2' */
    /* End of Start for SubSystem: '<Root>/Send Commands to PWM Blocks' */
  }

  {
    int32_T i;

    /* SystemInitialize for IfAction SubSystem: '<Root>/Change BLUE Behavior' */
    /* SystemInitialize for IfAction SubSystem: '<Root>/Change BLACK Behavior' */
    /* SystemInitialize for IfAction SubSystem: '<Root>/Change RED Behavior' */
    for (i = 0; i < 8; i++) {
      /* SystemInitialize for Merge: '<S3>/Merge' */
      ValidateThrustersBLACK_B.Merge[i] =
        ValidateThrustersBLACK_P.Merge_InitialOutput_g;

      /* SystemInitialize for Merge: '<S1>/Merge' */
      ValidateThrustersBLACK_B.Merge_c[i] =
        ValidateThrustersBLACK_P.Merge_InitialOutput;

      /* SystemInitialize for Merge: '<S2>/Merge' */
      ValidateThrustersBLACK_B.Merge_a[i] =
        ValidateThrustersBLACK_P.Merge_InitialOutput_j;
    }

    /* End of SystemInitialize for SubSystem: '<Root>/Change RED Behavior' */
    /* End of SystemInitialize for SubSystem: '<Root>/Change BLACK Behavior' */
    /* End of SystemInitialize for SubSystem: '<Root>/Change BLUE Behavior' */
  }
}

/* Model terminate function */
void ValidateThrustersBLACK_terminate(void)
{
  /* Terminate for Atomic SubSystem: '<Root>/Send Commands to PWM Blocks' */
  /* Terminate for MATLABSystem: '<S5>/RED T1 - BLACK T8' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj);

  /* Terminate for MATLABSystem: '<S5>/RED T2 - BLACK T3' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_m);

  /* Terminate for MATLABSystem: '<S5>/RED T3' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_f);

  /* Terminate for MATLABSystem: '<S5>/RED T4 - BLACK T5' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_fe);

  /* Terminate for MATLABSystem: '<S5>/RED T5 - BLACK T4' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_p);

  /* Terminate for MATLABSystem: '<S5>/RED T6 - BLACK T7' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_a);

  /* Terminate for MATLABSystem: '<S5>/RED T7 - BLACK T6' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_k);

  /* Terminate for MATLABSystem: '<S5>/RED T8 - BLACK T1' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_e);

  /* Terminate for MATLABSystem: '<S5>/BLACK T2' */
  matlabCodegenHandle_matlabCodeg(&ValidateThrustersBLACK_DW.obj_i);

  /* End of Terminate for SubSystem: '<Root>/Send Commands to PWM Blocks' */
}
