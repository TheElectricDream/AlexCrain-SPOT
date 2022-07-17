/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Template_v3_08_2022a_Jetson_DataOnly2.c
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

#include "Template_v3_08_2022a_Jetson_DataOnly2.h"
#include <math.h>
#include "rtwtypes.h"
#include "Template_v3_08_2022a_Jetson_DataOnly2_private.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_Template_v3_08_2022a_Jetson_T Template_v3_08_2022a_Jetson_D_B;

/* Block states (default storage) */
DW_Template_v3_08_2022a_Jetso_T Template_v3_08_2022a_Jetson__DW;

/* Real-time model */
static RT_MODEL_Template_v3_08_2022a_T Template_v3_08_2022a_Jetson__M_;
RT_MODEL_Template_v3_08_2022a_T *const Template_v3_08_2022a_Jetson__M =
  &Template_v3_08_2022a_Jetson__M_;

/* Model step function */
void Template_v3_08_2022a_Jetson_DataOnly2_step(void)
{
  real_T rtb_Sum6_a2;
  real_T rtb_Sum6_bs;
  real_T rtb_Sum6_ci;
  real_T rtb_Sum6_fb;
  real_T rtb_Sum6_k;
  real_T rtb_Sum6_mx;
  int32_T samplesRead;
  char_T *sErr;

  /* S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */
  sErr = GetErrorBuffer(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib
                        [0U]);
  samplesRead = 16;
  LibOutputs_Network(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U],
                     &Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[0U],
                     &samplesRead);
  if (*sErr != 0) {
    rtmSetErrorStatus(Template_v3_08_2022a_Jetson__M, sErr);
    rtmSetStopRequested(Template_v3_08_2022a_Jetson__M, 1);
  }

  /* End of S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  DataStoreWrite: '<Root>/Data Store Write3'
   *  DataStoreWrite: '<Root>/Data Store Write4'
   *  DataStoreWrite: '<Root>/Data Store Write5'
   *  DataStoreWrite: '<Root>/Data Store Write6'
   *  DataStoreWrite: '<Root>/Data Store Write7'
   *  DataStoreWrite: '<Root>/Data Store Write8'
   *  DataStoreWrite: '<Root>/RED_Px1'
   *  DataStoreWrite: '<Root>/RED_Px10'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[58] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[7];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[59] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[8];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[60] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[9];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[61] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[10];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[62] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[11];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[63] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[12];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[4] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[1];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[0] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[0];

  /* Delay: '<S12>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[4];
  }

  /* Sum: '<S12>/Sum6' incorporates:
   *  Delay: '<S12>/Delay1'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6 =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[4] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE;

  /* If: '<S12>/if we went through a "step"' */
  if (Template_v3_08_2022a_Jetson_D_B.Sum6 != 0.0) {
    /* Outputs for IfAction SubSystem: '<S12>/Hold this value' incorporates:
     *  ActionPort: '<S27>/Action Port'
     */
    /* SignalConversion generated from: '<S27>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20as =
      Template_v3_08_2022a_Jetson_D_B.Sum6;

    /* End of Outputs for SubSystem: '<S12>/Hold this value' */
  }

  /* End of If: '<S12>/if we went through a "step"' */

  /* Gain: '<S12>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px3'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6 = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20as;

  /* Delay: '<S4>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px3'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_h) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_f =
      Template_v3_08_2022a_Jetson_D_B.Sum6;
  }

  /* Sum: '<S4>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px3'
   *  Delay: '<S4>/Delay1'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_h = Template_v3_08_2022a_Jetson_D_B.Sum6
    - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_f;

  /* If: '<S4>/if we went through a "step"' */
  if (Template_v3_08_2022a_Jetson_D_B.Sum6_h != 0.0) {
    /* Outputs for IfAction SubSystem: '<S4>/Hold this value' incorporates:
     *  ActionPort: '<S19>/Action Port'
     */
    /* SignalConversion generated from: '<S19>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdy =
      Template_v3_08_2022a_Jetson_D_B.Sum6_h;

    /* End of Outputs for SubSystem: '<S4>/Hold this value' */
  }

  /* End of If: '<S4>/if we went through a "step"' */

  /* Delay: '<S13>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_c) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ff =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[5];
  }

  /* Sum: '<S13>/Sum6' incorporates:
   *  Delay: '<S13>/Delay1'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_h =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[5] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ff;

  /* If: '<S13>/if we went through a "step"' */
  if (Template_v3_08_2022a_Jetson_D_B.Sum6_h != 0.0) {
    /* Outputs for IfAction SubSystem: '<S13>/Hold this value' incorporates:
     *  ActionPort: '<S28>/Action Port'
     */
    /* SignalConversion generated from: '<S28>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20a =
      Template_v3_08_2022a_Jetson_D_B.Sum6_h;

    /* End of Outputs for SubSystem: '<S13>/Hold this value' */
  }

  /* End of If: '<S13>/if we went through a "step"' */

  /* Gain: '<S13>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py3'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_h = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20a;

  /* Delay: '<S5>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py3'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_i) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_a =
      Template_v3_08_2022a_Jetson_D_B.Sum6_h;
  }

  /* Sum: '<S5>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py3'
   *  Delay: '<S5>/Delay1'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_o =
    Template_v3_08_2022a_Jetson_D_B.Sum6_h -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_a;

  /* If: '<S5>/if we went through a "step"' */
  if (Template_v3_08_2022a_Jetson_D_B.Sum6_o != 0.0) {
    /* Outputs for IfAction SubSystem: '<S5>/Hold this value' incorporates:
     *  ActionPort: '<S20>/Action Port'
     */
    /* SignalConversion generated from: '<S20>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cd =
      Template_v3_08_2022a_Jetson_D_B.Sum6_o;

    /* End of Outputs for SubSystem: '<S5>/Hold this value' */
  }

  /* End of If: '<S5>/if we went through a "step"' */

  /* S-Function (sdspunwrap2): '<Root>/Unwrap1' */
  if (Template_v3_08_2022a_Jetson__DW.Unwrap1_FirstStep) {
    Template_v3_08_2022a_Jetson__DW.Unwrap1_Prev =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[6];
    Template_v3_08_2022a_Jetson__DW.Unwrap1_FirstStep = false;
  }

  rtb_Sum6_bs = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[6] -
    Template_v3_08_2022a_Jetson__DW.Unwrap1_Prev;
  rtb_Sum6_k = rtb_Sum6_bs - floor((rtb_Sum6_bs + 3.1415926535897931) /
    6.2831853071795862) * 6.2831853071795862;
  if ((rtb_Sum6_k == -3.1415926535897931) && (rtb_Sum6_bs > 0.0)) {
    rtb_Sum6_k = 3.1415926535897931;
  }

  rtb_Sum6_bs = rtb_Sum6_k - rtb_Sum6_bs;
  if (fabs(rtb_Sum6_bs) > 3.1415926535897931) {
    Template_v3_08_2022a_Jetson__DW.Unwrap1_Cumsum += rtb_Sum6_bs;
  }

  Template_v3_08_2022a_Jetson__DW.Unwrap1_Prev =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[6];
  Template_v3_08_2022a_Jetson_D_B.Sum6_o =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[6] +
    Template_v3_08_2022a_Jetson__DW.Unwrap1_Cumsum;

  /* End of S-Function (sdspunwrap2): '<Root>/Unwrap1' */

  /* Delay: '<S14>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_g) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_j =
      Template_v3_08_2022a_Jetson_D_B.Sum6_o;
  }

  /* Sum: '<S14>/Sum6' incorporates:
   *  Delay: '<S14>/Delay1'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_d =
    Template_v3_08_2022a_Jetson_D_B.Sum6_o -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_j;

  /* If: '<S14>/if we went through a "step"' */
  if (Template_v3_08_2022a_Jetson_D_B.Sum6_d != 0.0) {
    /* Outputs for IfAction SubSystem: '<S14>/Hold this value' incorporates:
     *  ActionPort: '<S29>/Action Port'
     */
    /* SignalConversion generated from: '<S29>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20 =
      Template_v3_08_2022a_Jetson_D_B.Sum6_d;

    /* End of Outputs for SubSystem: '<S14>/Hold this value' */
  }

  /* End of If: '<S14>/if we went through a "step"' */

  /* Gain: '<S14>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz3'
   */
  Template_v3_08_2022a_Jetson_D_B.Sum6_d = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20;

  /* Delay: '<S6>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz3'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_ic) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_o =
      Template_v3_08_2022a_Jetson_D_B.Sum6_d;
  }

  /* Sum: '<S6>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz3'
   *  Delay: '<S6>/Delay1'
   */
  rtb_Sum6_fb = Template_v3_08_2022a_Jetson_D_B.Sum6_d -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_o;

  /* If: '<S6>/if we went through a "step"' */
  if (rtb_Sum6_fb != 0.0) {
    /* Outputs for IfAction SubSystem: '<S6>/Hold this value' incorporates:
     *  ActionPort: '<S21>/Action Port'
     */
    /* SignalConversion generated from: '<S21>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0c = rtb_Sum6_fb;

    /* End of Outputs for SubSystem: '<S6>/Hold this value' */
  }

  /* End of If: '<S6>/if we went through a "step"' */

  /* Delay: '<S7>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_f) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_e =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[13];
  }

  /* Sum: '<S7>/Sum6' incorporates:
   *  Delay: '<S7>/Delay1'
   */
  rtb_Sum6_fb = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[13] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_e;

  /* If: '<S7>/if we went through a "step"' */
  if (rtb_Sum6_fb != 0.0) {
    /* Outputs for IfAction SubSystem: '<S7>/Hold this value' incorporates:
     *  ActionPort: '<S22>/Action Port'
     */
    /* SignalConversion generated from: '<S22>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0 = rtb_Sum6_fb;

    /* End of Outputs for SubSystem: '<S7>/Hold this value' */
  }

  /* End of If: '<S7>/if we went through a "step"' */

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px18'
   *  DataStoreWrite: '<Root>/RED_Px19'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[44] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[13];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[45] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[14];

  /* Delay: '<S3>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_j) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_n =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[1];
  }

  /* Sum: '<S3>/Sum6' incorporates:
   *  Delay: '<S3>/Delay1'
   */
  rtb_Sum6_fb = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[1] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_n;

  /* If: '<S3>/if we went through a "step"' */
  if (rtb_Sum6_fb != 0.0) {
    /* Outputs for IfAction SubSystem: '<S3>/Hold this value' incorporates:
     *  ActionPort: '<S18>/Action Port'
     */
    /* SignalConversion generated from: '<S18>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdyi = rtb_Sum6_fb;

    /* End of Outputs for SubSystem: '<S3>/Hold this value' */
  }

  /* End of If: '<S3>/if we went through a "step"' */

  /* Gain: '<S3>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px2'
   */
  rtb_Sum6_fb = 1.0 / Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdyi;

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px20'
   *  DataStoreWrite: '<Root>/RED_Px4'
   *  DataStoreWrite: '<Root>/RED_Px5'
   *  DataStoreWrite: '<Root>/RED_Px6'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[46] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[15];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[24] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[4];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[25] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[5];
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[26] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[6];

  /* Delay: '<S15>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px2'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_ft) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_i = rtb_Sum6_fb;
  }

  /* Sum: '<S15>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px2'
   *  Delay: '<S15>/Delay1'
   */
  rtb_Sum6_mx = rtb_Sum6_fb - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_i;

  /* If: '<S15>/if we went through a "step"' */
  if (rtb_Sum6_mx != 0.0) {
    /* Outputs for IfAction SubSystem: '<S15>/Hold this value' incorporates:
     *  ActionPort: '<S30>/Action Port'
     */
    /* SignalConversion generated from: '<S30>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g2 = rtb_Sum6_mx;

    /* End of Outputs for SubSystem: '<S15>/Hold this value' */
  }

  /* End of If: '<S15>/if we went through a "step"' */

  /* Delay: '<S10>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_fw) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ap =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[2];
  }

  /* Sum: '<S10>/Sum6' incorporates:
   *  Delay: '<S10>/Delay1'
   */
  rtb_Sum6_mx = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[2] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ap;

  /* If: '<S10>/if we went through a "step"' */
  if (rtb_Sum6_mx != 0.0) {
    /* Outputs for IfAction SubSystem: '<S10>/Hold this value' incorporates:
     *  ActionPort: '<S25>/Action Port'
     */
    /* SignalConversion generated from: '<S25>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdt = rtb_Sum6_mx;

    /* End of Outputs for SubSystem: '<S10>/Hold this value' */
  }

  /* End of If: '<S10>/if we went through a "step"' */

  /* Gain: '<S10>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py2'
   */
  rtb_Sum6_mx = 1.0 / Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdt;

  /* Delay: '<S16>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py2'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_g4) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_l = rtb_Sum6_mx;
  }

  /* Sum: '<S16>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py2'
   *  Delay: '<S16>/Delay1'
   */
  rtb_Sum6_a2 = rtb_Sum6_mx - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_l;

  /* If: '<S16>/if we went through a "step"' */
  if (rtb_Sum6_a2 != 0.0) {
    /* Outputs for IfAction SubSystem: '<S16>/Hold this value' incorporates:
     *  ActionPort: '<S31>/Action Port'
     */
    /* SignalConversion generated from: '<S31>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g = rtb_Sum6_a2;

    /* End of Outputs for SubSystem: '<S16>/Hold this value' */
  }

  /* End of If: '<S16>/if we went through a "step"' */

  /* S-Function (sdspunwrap2): '<Root>/Unwrap' */
  if (Template_v3_08_2022a_Jetson__DW.Unwrap_FirstStep) {
    Template_v3_08_2022a_Jetson__DW.Unwrap_Prev =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[3];
    Template_v3_08_2022a_Jetson__DW.Unwrap_FirstStep = false;
  }

  rtb_Sum6_bs = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[3] -
    Template_v3_08_2022a_Jetson__DW.Unwrap_Prev;
  rtb_Sum6_k = rtb_Sum6_bs - floor((rtb_Sum6_bs + 3.1415926535897931) /
    6.2831853071795862) * 6.2831853071795862;
  if ((rtb_Sum6_k == -3.1415926535897931) && (rtb_Sum6_bs > 0.0)) {
    rtb_Sum6_k = 3.1415926535897931;
  }

  rtb_Sum6_bs = rtb_Sum6_k - rtb_Sum6_bs;
  if (fabs(rtb_Sum6_bs) > 3.1415926535897931) {
    Template_v3_08_2022a_Jetson__DW.Unwrap_Cumsum += rtb_Sum6_bs;
  }

  Template_v3_08_2022a_Jetson__DW.Unwrap_Prev =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[3];
  rtb_Sum6_a2 = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[3] +
    Template_v3_08_2022a_Jetson__DW.Unwrap_Cumsum;

  /* End of S-Function (sdspunwrap2): '<Root>/Unwrap' */

  /* Delay: '<S11>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_m) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_iy = rtb_Sum6_a2;
  }

  /* Sum: '<S11>/Sum6' incorporates:
   *  Delay: '<S11>/Delay1'
   */
  rtb_Sum6_ci = rtb_Sum6_a2 - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_iy;

  /* If: '<S11>/if we went through a "step"' */
  if (rtb_Sum6_ci != 0.0) {
    /* Outputs for IfAction SubSystem: '<S11>/Hold this value' incorporates:
     *  ActionPort: '<S26>/Action Port'
     */
    /* SignalConversion generated from: '<S26>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asd = rtb_Sum6_ci;

    /* End of Outputs for SubSystem: '<S11>/Hold this value' */
  }

  /* End of If: '<S11>/if we went through a "step"' */

  /* Gain: '<S11>/divide by delta T' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz2'
   */
  rtb_Sum6_ci = 1.0 / Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asd;

  /* Delay: '<S17>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz2'
   */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_cp) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_nn = rtb_Sum6_ci;
  }

  /* Sum: '<S17>/Sum6' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz2'
   *  Delay: '<S17>/Delay1'
   */
  rtb_Sum6_bs = rtb_Sum6_ci - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_nn;

  /* If: '<S17>/if we went through a "step"' */
  if (rtb_Sum6_bs != 0.0) {
    /* Outputs for IfAction SubSystem: '<S17>/Hold this value' incorporates:
     *  ActionPort: '<S32>/Action Port'
     */
    /* SignalConversion generated from: '<S32>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1 = rtb_Sum6_bs;

    /* End of Outputs for SubSystem: '<S17>/Hold this value' */
  }

  /* End of If: '<S17>/if we went through a "step"' */

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py1'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[5] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[2];

  /* Delay: '<S8>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_ml) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_k =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[14];
  }

  /* Sum: '<S8>/Sum6' incorporates:
   *  Delay: '<S8>/Delay1'
   */
  rtb_Sum6_bs = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[14] -
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_k;

  /* If: '<S8>/if we went through a "step"' */
  if (rtb_Sum6_bs != 0.0) {
    /* Outputs for IfAction SubSystem: '<S8>/Hold this value' incorporates:
     *  ActionPort: '<S23>/Action Port'
     */
    /* SignalConversion generated from: '<S23>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi = rtb_Sum6_bs;

    /* End of Outputs for SubSystem: '<S8>/Hold this value' */
  }

  /* End of If: '<S8>/if we went through a "step"' */

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz1'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[6] =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[3];

  /* S-Function (sdspunwrap2): '<Root>/Unwrap2' */
  if (Template_v3_08_2022a_Jetson__DW.Unwrap2_FirstStep) {
    Template_v3_08_2022a_Jetson__DW.Unwrap2_Prev =
      Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[15];
    Template_v3_08_2022a_Jetson__DW.Unwrap2_FirstStep = false;
  }

  rtb_Sum6_bs = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[15] -
    Template_v3_08_2022a_Jetson__DW.Unwrap2_Prev;
  rtb_Sum6_k = rtb_Sum6_bs - floor((rtb_Sum6_bs + 3.1415926535897931) /
    6.2831853071795862) * 6.2831853071795862;
  if ((rtb_Sum6_k == -3.1415926535897931) && (rtb_Sum6_bs > 0.0)) {
    rtb_Sum6_k = 3.1415926535897931;
  }

  rtb_Sum6_bs = rtb_Sum6_k - rtb_Sum6_bs;
  if (fabs(rtb_Sum6_bs) > 3.1415926535897931) {
    Template_v3_08_2022a_Jetson__DW.Unwrap2_Cumsum += rtb_Sum6_bs;
  }

  Template_v3_08_2022a_Jetson__DW.Unwrap2_Prev =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[15];
  rtb_Sum6_bs = Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[15] +
    Template_v3_08_2022a_Jetson__DW.Unwrap2_Cumsum;

  /* End of S-Function (sdspunwrap2): '<Root>/Unwrap2' */

  /* Delay: '<S9>/Delay1' */
  if (Template_v3_08_2022a_Jetson__DW.icLoad_d) {
    Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ig = rtb_Sum6_bs;
  }

  /* Sum: '<S9>/Sum6' incorporates:
   *  Delay: '<S9>/Delay1'
   */
  rtb_Sum6_k = rtb_Sum6_bs - Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ig;

  /* If: '<S9>/if we went through a "step"' */
  if (rtb_Sum6_k != 0.0) {
    /* Outputs for IfAction SubSystem: '<S9>/Hold this value' incorporates:
     *  ActionPort: '<S24>/Action Port'
     */
    /* SignalConversion generated from: '<S24>/In1' */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtg = rtb_Sum6_k;

    /* End of Outputs for SubSystem: '<S9>/Hold this value' */
  }

  /* End of If: '<S9>/if we went through a "step"' */

  /* SignalConversion generated from: '<S1>/To Workspace' incorporates:
   *  Constant: '<S2>/Constant'
   *  DataStoreRead: '<S1>/BLACK_AHRS_P'
   *  DataStoreRead: '<S1>/BLACK_AHRS_Q'
   *  DataStoreRead: '<S1>/BLACK_AHRS_R'
   *  DataStoreRead: '<S1>/BLACK_Fx_Sat'
   *  DataStoreRead: '<S1>/BLACK_Fy_Sat'
   *  DataStoreRead: '<S1>/BLACK_GyroX_Raw'
   *  DataStoreRead: '<S1>/BLACK_GyroY_Raw'
   *  DataStoreRead: '<S1>/BLACK_GyroZ_Raw'
   *  DataStoreRead: '<S1>/BLACK_Gyro_Attitude'
   *  DataStoreRead: '<S1>/BLACK_IMU_Ax_I'
   *  DataStoreRead: '<S1>/BLACK_IMU_Ax_b'
   *  DataStoreRead: '<S1>/BLACK_IMU_Ay_I'
   *  DataStoreRead: '<S1>/BLACK_IMU_Ay_b'
   *  DataStoreRead: '<S1>/BLACK_IMU_Az_b'
   *  DataStoreRead: '<S1>/BLACK_RzD'
   *  DataStoreRead: '<S1>/BLACK_Tz_Sat'
   *  DataStoreRead: '<S1>/BLACK_Vx'
   *  DataStoreRead: '<S1>/BLACK_Vy'
   *  DataStoreRead: '<S1>/BLUE_Fx_Sat'
   *  DataStoreRead: '<S1>/BLUE_Fy_Sat'
   *  DataStoreRead: '<S1>/BLUE_Tz_Sat'
   *  DataStoreRead: '<S1>/RED_AHRS_P'
   *  DataStoreRead: '<S1>/RED_AHRS_Q'
   *  DataStoreRead: '<S1>/RED_AHRS_R'
   *  DataStoreRead: '<S1>/RED_Fx_Sat'
   *  DataStoreRead: '<S1>/RED_Fy_Sat'
   *  DataStoreRead: '<S1>/RED_GyroX_Raw'
   *  DataStoreRead: '<S1>/RED_GyroY_Raw'
   *  DataStoreRead: '<S1>/RED_GyroZ_Raw'
   *  DataStoreRead: '<S1>/RED_Gyro_Attitude'
   *  DataStoreRead: '<S1>/RED_IMU_Ax_I'
   *  DataStoreRead: '<S1>/RED_IMU_Ax_b'
   *  DataStoreRead: '<S1>/RED_IMU_Ay_I'
   *  DataStoreRead: '<S1>/RED_IMU_Ay_b'
   *  DataStoreRead: '<S1>/RED_IMU_Az_b'
   *  DataStoreRead: '<S1>/RED_RzD_Elbow'
   *  DataStoreRead: '<S1>/RED_RzD_Shoulder'
   *  DataStoreRead: '<S1>/RED_RzD_Wrist'
   *  DataStoreRead: '<S1>/RED_Rz_Elbow'
   *  DataStoreRead: '<S1>/RED_Rz_Shoulder'
   *  DataStoreRead: '<S1>/RED_Rz_Wrist'
   *  DataStoreRead: '<S1>/RED_Tz_Elbow'
   *  DataStoreRead: '<S1>/RED_Tz_Sat'
   *  DataStoreRead: '<S1>/RED_Tz_Shoulder'
   *  DataStoreRead: '<S1>/RED_Tz_Wrist'
   *  DataStoreRead: '<S1>/RED_Vx '
   *  DataStoreRead: '<S1>/RED_Vy'
   *  DataStoreRead: '<S1>/RED_Vz'
   *  Gain: '<S15>/divide by delta T'
   *  Gain: '<S16>/divide by delta T'
   *  Gain: '<S17>/divide by delta T'
   *  Gain: '<S4>/divide by delta T'
   *  Gain: '<S5>/divide by delta T'
   *  Gain: '<S6>/divide by delta T'
   *  Gain: '<S7>/divide by delta T'
   *  Gain: '<S8>/divide by delta T'
   *  Gain: '<S9>/divide by delta T'
   */
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[1] =
    Template_v3_08_2022a_Jetson__DW.RED_Fx_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[2] =
    Template_v3_08_2022a_Jetson__DW.RED_Fy_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[3] =
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[7] = rtb_Sum6_fb;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[8] = rtb_Sum6_mx;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[9] = rtb_Sum6_ci;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[10] =
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_Q;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[11] =
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_P;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[12] =
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_R;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[13] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ax_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[14] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ay_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[15] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Az_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[16] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ax_I;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[17] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ay_I;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[18] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g2;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[19] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[20] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[21] =
    Template_v3_08_2022a_Jetson__DW.BLACK_Fx_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[22] =
    Template_v3_08_2022a_Jetson__DW.BLACK_Fy_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[23] =
    Template_v3_08_2022a_Jetson__DW.BLACK_Tz_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[27] =
    Template_v3_08_2022a_Jetson_D_B.Sum6;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[28] =
    Template_v3_08_2022a_Jetson_D_B.Sum6_h;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[29] =
    Template_v3_08_2022a_Jetson_D_B.Sum6_d;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[30] =
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_Q;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[31] =
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_P;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[32] =
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_R;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[33] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ax_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[34] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ay_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[35] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Az_b;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[36] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ax_I;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[37] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ay_I;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[38] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdy;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[39] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cd;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[40] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0c;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[41] =
    Template_v3_08_2022a_Jetson__DW.BLUE_Fx_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[42] =
    Template_v3_08_2022a_Jetson__DW.BLUE_Fy_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[43] =
    Template_v3_08_2022a_Jetson__DW.BLUE_Tz_Sat;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[47] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[48] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[49] = 1.0 /
    Template_v3_08_2022a_Jetson_D_P.baseRate *
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtg;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[50] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Q;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[51] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_P;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[52] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_R;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[53] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Q;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[54] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_P;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[55] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_R;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[56] =
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Psi;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[57] =
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Psi;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[64] =
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Shoulder;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[65] =
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Elbow;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[66] =
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Wrist;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[67] =
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Shoulder;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[68] =
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Elbow;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[69] =
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Wrist;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[70] =
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Shoulder;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[71] =
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Elbow;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[72] =
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Wrist;
  Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[73] =
    Template_v3_08_2022a_Jetson_D_P.Constant_Value;

  /* ToWorkspace: '<S1>/To Workspace' */
  rt_UpdateLogVar((LogVar *)(LogVar*)
                  (Template_v3_08_2022a_Jetson__DW.ToWorkspace_PWORK.LoggedData),
                  &Template_v3_08_2022a_Jetson_D_B.TmpSignalConversionAtToWork[0],
                  0);

  /* Update for Delay: '<S12>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[4];

  /* Update for Delay: '<S4>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px3'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_h = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_f =
    Template_v3_08_2022a_Jetson_D_B.Sum6;

  /* Update for Delay: '<S13>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_c = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ff =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[5];

  /* Update for Delay: '<S5>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py3'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_i = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_a =
    Template_v3_08_2022a_Jetson_D_B.Sum6_h;

  /* Update for Delay: '<S14>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_g = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_j =
    Template_v3_08_2022a_Jetson_D_B.Sum6_o;

  /* Update for Delay: '<S6>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz3'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_ic = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_o =
    Template_v3_08_2022a_Jetson_D_B.Sum6_d;

  /* Update for Delay: '<S7>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_f = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_e =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[13];

  /* Update for Delay: '<S3>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_j = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_n =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[1];

  /* Update for Delay: '<S15>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Px2'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_ft = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_i = rtb_Sum6_fb;

  /* Update for Delay: '<S10>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_fw = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ap =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[2];

  /* Update for Delay: '<S16>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Py2'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_g4 = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_l = rtb_Sum6_mx;

  /* Update for Delay: '<S11>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_m = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_iy = rtb_Sum6_a2;

  /* Update for Delay: '<S17>/Delay1' incorporates:
   *  DataStoreWrite: '<Root>/RED_Rz2'
   */
  Template_v3_08_2022a_Jetson__DW.icLoad_cp = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_nn = rtb_Sum6_ci;

  /* Update for Delay: '<S8>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_ml = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_k =
    Template_v3_08_2022a_Jetson_D_B.UDPReceive1_o1[14];

  /* Update for Delay: '<S9>/Delay1' */
  Template_v3_08_2022a_Jetson__DW.icLoad_d = false;
  Template_v3_08_2022a_Jetson__DW.Delay1_DSTATE_ig = rtb_Sum6_bs;

  /* Matfile logging */
  rt_UpdateTXYLogVars(Template_v3_08_2022a_Jetson__M->rtwLogInfo,
                      (&Template_v3_08_2022a_Jetson__M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.05s, 0.0s] */
    if ((rtmGetTFinal(Template_v3_08_2022a_Jetson__M)!=-1) &&
        !((rtmGetTFinal(Template_v3_08_2022a_Jetson__M)-
           Template_v3_08_2022a_Jetson__M->Timing.taskTime0) >
          Template_v3_08_2022a_Jetson__M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(Template_v3_08_2022a_Jetson__M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  Template_v3_08_2022a_Jetson__M->Timing.taskTime0 =
    ((time_T)(++Template_v3_08_2022a_Jetson__M->Timing.clockTick0)) *
    Template_v3_08_2022a_Jetson__M->Timing.stepSize0;
}

/* Model initialize function */
void Template_v3_08_2022a_Jetson_DataOnly2_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
  rtmSetTFinal(Template_v3_08_2022a_Jetson__M, 10.0);
  Template_v3_08_2022a_Jetson__M->Timing.stepSize0 = 0.05;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    Template_v3_08_2022a_Jetson__M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(Template_v3_08_2022a_Jetson__M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Template_v3_08_2022a_Jetson__M->rtwLogInfo, (NULL));
    rtliSetLogT(Template_v3_08_2022a_Jetson__M->rtwLogInfo, "tout");
    rtliSetLogX(Template_v3_08_2022a_Jetson__M->rtwLogInfo, "");
    rtliSetLogXFinal(Template_v3_08_2022a_Jetson__M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Template_v3_08_2022a_Jetson__M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Template_v3_08_2022a_Jetson__M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Template_v3_08_2022a_Jetson__M->rtwLogInfo, 0);
    rtliSetLogDecimation(Template_v3_08_2022a_Jetson__M->rtwLogInfo, 1);
    rtliSetLogY(Template_v3_08_2022a_Jetson__M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Template_v3_08_2022a_Jetson__M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Template_v3_08_2022a_Jetson__M->rtwLogInfo, (NULL));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(Template_v3_08_2022a_Jetson__M->rtwLogInfo,
    0.0, rtmGetTFinal(Template_v3_08_2022a_Jetson__M),
    Template_v3_08_2022a_Jetson__M->Timing.stepSize0, (&rtmGetErrorStatus
    (Template_v3_08_2022a_Jetson__M)));

  {
    char_T *sErr;

    /* SetupRuntimeResources for ToWorkspace: '<S1>/To Workspace' */
    {
      int_T dimensions[1] = { 74 };

      Template_v3_08_2022a_Jetson__DW.ToWorkspace_PWORK.LoggedData =
        rt_CreateLogVar(
                        Template_v3_08_2022a_Jetson__M->rtwLogInfo,
                        0.0,
                        rtmGetTFinal(Template_v3_08_2022a_Jetson__M),
                        Template_v3_08_2022a_Jetson__M->Timing.stepSize0,
                        (&rtmGetErrorStatus(Template_v3_08_2022a_Jetson__M)),
                        "dataPacket",
                        SS_DOUBLE,
                        0,
                        0,
                        0,
                        74,
                        1,
                        dimensions,
                        NO_LOGVALDIMS,
                        (NULL),
                        (NULL),
                        0,
                        1,
                        0.05,
                        1);
      if (Template_v3_08_2022a_Jetson__DW.ToWorkspace_PWORK.LoggedData == (NULL))
        return;
    }

    /* Start for S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */
    sErr = GetErrorBuffer
      (&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U]);
    CreateUDPInterface(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib
                       [0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib
                        [0U], 0, "0.0.0.0",
                        Template_v3_08_2022a_Jetson_D_P.UDPReceive1_Port,
                        "0.0.0.0", -1, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface
        (&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Template_v3_08_2022a_Jetson__M, sErr);
        rtmSetStopRequested(Template_v3_08_2022a_Jetson__M, 1);
      }
    }

    /* End of Start for S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */

    /* Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat' */
    Template_v3_08_2022a_Jetson__DW.BLACK_Fx_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Fx_Sat_InitialValue;

    /* Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat2' */
    Template_v3_08_2022a_Jetson__DW.BLUE_Fx_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Fx_Sat2_InitialValue;

    /* Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat' */
    Template_v3_08_2022a_Jetson__DW.BLACK_Fy_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Fy_Sat_InitialValue;

    /* Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat2' */
    Template_v3_08_2022a_Jetson__DW.BLUE_Fy_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Fy_Sat2_InitialValue;

    /* Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat' */
    Template_v3_08_2022a_Jetson__DW.BLACK_Tz_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Tz_Sat_InitialValue;

    /* Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat2' */
    Template_v3_08_2022a_Jetson__DW.BLUE_Tz_Sat =
      Template_v3_08_2022a_Jetson_D_P.BLACK_Tz_Sat2_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Fx_Sat' */
    Template_v3_08_2022a_Jetson__DW.RED_Fx_Sat =
      Template_v3_08_2022a_Jetson_D_P.RED_Fx_Sat_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Fy_Sat' */
    Template_v3_08_2022a_Jetson__DW.RED_Fy_Sat =
      Template_v3_08_2022a_Jetson_D_P.RED_Fy_Sat_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px14' */
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_Q =
      Template_v3_08_2022a_Jetson_D_P.RED_Px14_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px15' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ax_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px15_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px16' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ay_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px16_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px21' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Az_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px21_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px28' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Q =
      Template_v3_08_2022a_Jetson_D_P.RED_Px28_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px29' */
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_P =
      Template_v3_08_2022a_Jetson_D_P.RED_Px29_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px30' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_P =
      Template_v3_08_2022a_Jetson_D_P.RED_Px30_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px31' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_R =
      Template_v3_08_2022a_Jetson_D_P.RED_Px31_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px32' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Q =
      Template_v3_08_2022a_Jetson_D_P.RED_Px32_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px33' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_P =
      Template_v3_08_2022a_Jetson_D_P.RED_Px33_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px34' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_R =
      Template_v3_08_2022a_Jetson_D_P.RED_Px34_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px35' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Psi =
      Template_v3_08_2022a_Jetson_D_P.RED_Px35_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px36' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Psi =
      Template_v3_08_2022a_Jetson_D_P.RED_Px36_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px40' */
    Template_v3_08_2022a_Jetson__DW.RED_AHRS_R =
      Template_v3_08_2022a_Jetson_D_P.RED_Px40_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px42' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ax_I =
      Template_v3_08_2022a_Jetson_D_P.RED_Px42_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px43' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ay_I =
      Template_v3_08_2022a_Jetson_D_P.RED_Px43_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px48' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ax_I =
      Template_v3_08_2022a_Jetson_D_P.RED_Px48_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px49' */
    Template_v3_08_2022a_Jetson__DW.BLACK_IMU_Ay_I =
      Template_v3_08_2022a_Jetson_D_P.RED_Px49_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px51' */
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_Q =
      Template_v3_08_2022a_Jetson_D_P.RED_Px51_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px52' */
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_P =
      Template_v3_08_2022a_Jetson_D_P.RED_Px52_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px53' */
    Template_v3_08_2022a_Jetson__DW.BLACK_AHRS_R =
      Template_v3_08_2022a_Jetson_D_P.RED_Px53_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px54' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ax_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px54_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px55' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Ay_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px55_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Px56' */
    Template_v3_08_2022a_Jetson__DW.RED_IMU_Az_b =
      Template_v3_08_2022a_Jetson_D_P.RED_Px56_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz1' */
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Shoulder =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz1_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz12' */
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Shoulder =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz12_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz13' */
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Elbow =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz13_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz14' */
    Template_v3_08_2022a_Jetson__DW.RED_RzD_Wrist =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz14_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz2' */
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Elbow =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz2_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz3' */
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Wrist =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz3_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz4' */
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Shoulder =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz4_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz5' */
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Elbow =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz5_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz6' */
    Template_v3_08_2022a_Jetson__DW.RED_Rz_Wrist =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz6_InitialValue;

    /* Start for DataStoreMemory: '<Root>/RED_Tz_Sat' */
    Template_v3_08_2022a_Jetson__DW.RED_Tz_Sat =
      Template_v3_08_2022a_Jetson_D_P.RED_Tz_Sat_InitialValue;

    /* InitializeConditions for Delay: '<S12>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad = true;

    /* InitializeConditions for Delay: '<S4>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_h = true;

    /* InitializeConditions for Delay: '<S13>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_c = true;

    /* InitializeConditions for Delay: '<S5>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_i = true;

    /* InitializeConditions for S-Function (sdspunwrap2): '<Root>/Unwrap1' */
    Template_v3_08_2022a_Jetson__DW.Unwrap1_FirstStep = true;

    /* InitializeConditions for Delay: '<S14>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_g = true;

    /* InitializeConditions for Delay: '<S6>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_ic = true;

    /* InitializeConditions for Delay: '<S7>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_f = true;

    /* InitializeConditions for Delay: '<S3>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_j = true;

    /* InitializeConditions for Delay: '<S15>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_ft = true;

    /* InitializeConditions for Delay: '<S10>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_fw = true;

    /* InitializeConditions for Delay: '<S16>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_g4 = true;

    /* InitializeConditions for S-Function (sdspunwrap2): '<Root>/Unwrap' */
    Template_v3_08_2022a_Jetson__DW.Unwrap_FirstStep = true;

    /* InitializeConditions for Delay: '<S11>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_m = true;

    /* InitializeConditions for Delay: '<S17>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_cp = true;

    /* InitializeConditions for Delay: '<S8>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_ml = true;

    /* InitializeConditions for S-Function (sdspunwrap2): '<Root>/Unwrap2' */
    Template_v3_08_2022a_Jetson__DW.Unwrap2_FirstStep = true;

    /* InitializeConditions for Delay: '<S9>/Delay1' */
    Template_v3_08_2022a_Jetson__DW.icLoad_d = true;

    /* SystemInitialize for IfAction SubSystem: '<S12>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S27>/In1' incorporates:
     *  Outport: '<S27>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20as =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_gz;

    /* End of SystemInitialize for SubSystem: '<S12>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S4>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S19>/In1' incorporates:
     *  Outport: '<S19>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdy =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_c;

    /* End of SystemInitialize for SubSystem: '<S4>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S13>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S28>/In1' incorporates:
     *  Outport: '<S28>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20a =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_e;

    /* End of SystemInitialize for SubSystem: '<S13>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S5>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S20>/In1' incorporates:
     *  Outport: '<S20>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cd =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_f;

    /* End of SystemInitialize for SubSystem: '<S5>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S14>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S29>/In1' incorporates:
     *  Outport: '<S29>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20 =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_p;

    /* End of SystemInitialize for SubSystem: '<S14>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S6>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S21>/In1' incorporates:
     *  Outport: '<S21>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0c =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_fz;

    /* End of SystemInitialize for SubSystem: '<S6>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S7>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S22>/In1' incorporates:
     *  Outport: '<S22>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0 =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_cw;

    /* End of SystemInitialize for SubSystem: '<S7>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S3>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S18>/In1' incorporates:
     *  Outport: '<S18>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi0cdyi =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0;

    /* End of SystemInitialize for SubSystem: '<S3>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S15>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S30>/In1' incorporates:
     *  Outport: '<S30>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g2 =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_n;

    /* End of SystemInitialize for SubSystem: '<S15>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S10>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S25>/In1' incorporates:
     *  Outport: '<S25>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdt =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_cn;

    /* End of SystemInitialize for SubSystem: '<S10>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S16>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S31>/In1' incorporates:
     *  Outport: '<S31>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_fh;

    /* End of SystemInitialize for SubSystem: '<S16>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S11>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S26>/In1' incorporates:
     *  Outport: '<S26>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asd =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_fu;

    /* End of SystemInitialize for SubSystem: '<S11>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S17>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S32>/In1' incorporates:
     *  Outport: '<S32>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1 =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_a;

    /* End of SystemInitialize for SubSystem: '<S17>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S8>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S23>/In1' incorporates:
     *  Outport: '<S23>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtgi =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_o;

    /* End of SystemInitialize for SubSystem: '<S8>/Hold this value' */

    /* SystemInitialize for IfAction SubSystem: '<S9>/Hold this value' */
    /* SystemInitialize for SignalConversion generated from: '<S24>/In1' incorporates:
     *  Outport: '<S24>/Out1'
     */
    Template_v3_08_2022a_Jetson_D_B.In1_g20asdtg =
      Template_v3_08_2022a_Jetson_D_P.Out1_Y0_g;

    /* End of SystemInitialize for SubSystem: '<S9>/Hold this value' */
  }
}

/* Model terminate function */
void Template_v3_08_2022a_Jetson_DataOnly2_terminate(void)
{
  char_T *sErr;

  /* Terminate for S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */
  sErr = GetErrorBuffer(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib
                        [0U]);
  LibTerminate(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Template_v3_08_2022a_Jetson__M, sErr);
    rtmSetStopRequested(Template_v3_08_2022a_Jetson__M, 1);
  }

  LibDestroy(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U], 0);
  DestroyUDPInterface(&Template_v3_08_2022a_Jetson__DW.UDPReceive1_NetworkLib[0U]);

  /* End of Terminate for S-Function (sdspFromNetwork): '<Root>/UDP Receive1' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
