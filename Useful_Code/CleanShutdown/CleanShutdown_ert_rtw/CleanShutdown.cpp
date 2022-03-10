//
//  CleanShutdown.cpp
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "CleanShutdown".
//
//  Model version              : 1.164
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Thu Mar 10 11:30:43 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#include "CleanShutdown.h"
#include "CleanShutdown_private.h"

// Block states (default storage)
DW_CleanShutdown_T CleanShutdown_DW;

// Real-time model
RT_MODEL_CleanShutdown_T CleanShutdown_M_ = RT_MODEL_CleanShutdown_T();
RT_MODEL_CleanShutdown_T *const CleanShutdown_M = &CleanShutdown_M_;

// Forward declaration for local functions
static void CleanShutdow_SystemCore_release(const
  raspi_internal_PWMBlock_Clean_T *obj);
static void CleanShutdown_SystemCore_delete(const
  raspi_internal_PWMBlock_Clean_T *obj);
static void matlabCodegenHandle_matlabCodeg(raspi_internal_PWMBlock_Clean_T *obj);
static void C_SystemCore_release_gx21kozvdu(const
  codertarget_linux_blocks_Digi_T *obj);
static void Cl_SystemCore_delete_gx21kozvdu(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle__gx21kozvdu(codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_release_gx21kozvdug(const codertarget_linux_blocks_Digi_T
  *obj);
static void C_SystemCore_delete_gx21kozvdug(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_gx21kozvdug(codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_release_gx21kozvdugn(const
  codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_delete_gx21kozvdugn(const codertarget_linux_blocks_Digi_T
  *obj);
static void matlabCodegenHandl_gx21kozvdugn(codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_releas_gx21kozvdugn1(const
  codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_delete_gx21kozvdugn1(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHand_gx21kozvdugn1(codertarget_linux_blocks_Digi_T *obj);
real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

static void CleanShutdow_SystemCore_release(const
  raspi_internal_PWMBlock_Clean_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    EXT_PWMBlock_terminate(obj->PinNumber);
  }
}

static void CleanShutdown_SystemCore_delete(const
  raspi_internal_PWMBlock_Clean_T *obj)
{
  CleanShutdow_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(raspi_internal_PWMBlock_Clean_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    CleanShutdown_SystemCore_delete(obj);
  }
}

static void C_SystemCore_release_gx21kozvdu(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(21U);
  }
}

static void Cl_SystemCore_delete_gx21kozvdu(const
  codertarget_linux_blocks_Digi_T *obj)
{
  C_SystemCore_release_gx21kozvdu(obj);
}

static void matlabCodegenHandle__gx21kozvdu(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Cl_SystemCore_delete_gx21kozvdu(obj);
  }
}

static void SystemCore_release_gx21kozvdug(const codertarget_linux_blocks_Digi_T
  *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(25U);
  }
}

static void C_SystemCore_delete_gx21kozvdug(const
  codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_release_gx21kozvdug(obj);
}

static void matlabCodegenHandle_gx21kozvdug(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_delete_gx21kozvdug(obj);
  }
}

static void SystemCore_release_gx21kozvdugn(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(10U);
  }
}

static void SystemCore_delete_gx21kozvdugn(const codertarget_linux_blocks_Digi_T
  *obj)
{
  SystemCore_release_gx21kozvdugn(obj);
}

static void matlabCodegenHandl_gx21kozvdugn(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_gx21kozvdugn(obj);
  }
}

static void SystemCore_releas_gx21kozvdugn1(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(26U);
  }
}

static void SystemCore_delete_gx21kozvdugn1(const
  codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_releas_gx21kozvdugn1(obj);
}

static void matlabCodegenHand_gx21kozvdugn1(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_gx21kozvdugn1(obj);
  }
}

// Model step function
void CleanShutdown_step(void)
{
  real_T y;
  uint8_T tmp;

  // MATLABSystem: '<Root>/BLACK T2' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj.PinNumber, y);

  // End of MATLABSystem: '<Root>/BLACK T2'

  // MATLABSystem: '<S1>/Digital Write' incorporates:
  //   Constant: '<Root>/Constant'
  //   MATLABSystem: '<S2>/Digital Write'
  //   MATLABSystem: '<S3>/Digital Write'
  //   MATLABSystem: '<S4>/Digital Write'

  y = rt_roundd_snf(CleanShutdown_P.Constant_Value);
  if (y < 256.0) {
    if (y >= 0.0) {
      tmp = static_cast<uint8_T>(y);
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  MW_gpioWrite(21U, tmp);

  // End of MATLABSystem: '<S1>/Digital Write'

  // MATLABSystem: '<S2>/Digital Write'
  if (y < 256.0) {
    if (y >= 0.0) {
      tmp = static_cast<uint8_T>(y);
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  MW_gpioWrite(25U, tmp);

  // MATLABSystem: '<S3>/Digital Write'
  if (y < 256.0) {
    if (y >= 0.0) {
      tmp = static_cast<uint8_T>(y);
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  MW_gpioWrite(10U, tmp);

  // MATLABSystem: '<S4>/Digital Write'
  if (y < 256.0) {
    if (y >= 0.0) {
      tmp = static_cast<uint8_T>(y);
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  MW_gpioWrite(26U, tmp);

  // MATLABSystem: '<Root>/PWM1' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_i.PinNumber, y);

  // End of MATLABSystem: '<Root>/PWM1'

  // MATLABSystem: '<Root>/RED T1 - BLACK T8' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_e.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T1 - BLACK T8'

  // MATLABSystem: '<Root>/RED T2 - BLACK T3' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_i5.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T2 - BLACK T3'

  // MATLABSystem: '<Root>/RED T3' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_b.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T3'

  // MATLABSystem: '<Root>/RED T4 - BLACK T5' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_d.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T4 - BLACK T5'

  // MATLABSystem: '<Root>/RED T5 - BLACK T4' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_f.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T5 - BLACK T4'

  // MATLABSystem: '<Root>/RED T6 - BLACK T7' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_o.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T6 - BLACK T7'

  // MATLABSystem: '<Root>/RED T7 - BLACK T6' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_l.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T7 - BLACK T6'

  // MATLABSystem: '<Root>/RED T8 - BLACK T1' incorporates:
  //   Constant: '<Root>/Constant'

  if (CleanShutdown_P.Constant_Value < 1.0) {
    y = CleanShutdown_P.Constant_Value;
  } else {
    y = 1.0;
  }

  if (!(y > 0.0)) {
    y = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(CleanShutdown_DW.obj_k.PinNumber, y);

  // End of MATLABSystem: '<Root>/RED T8 - BLACK T1'
}

// Model initialize function
void CleanShutdown_initialize(void)
{
  {
    int32_T ret;
    static const char_T tmp[45] =
      "Unable to configure pin %u for PWM output.\\n";
    char_T tmp_0[45];

    // Start for MATLABSystem: '<Root>/BLACK T2'
    CleanShutdown_DW.obj.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj.isInitialized = 1;
    CleanShutdown_DW.obj.PinNumber = 9U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj.PinNumber);
    }

    CleanShutdown_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/BLACK T2'

    // Start for MATLABSystem: '<S1>/Digital Write'
    CleanShutdown_DW.obj_j2.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_j2.isInitialized = 1;
    MW_gpioInit(21U, true);
    CleanShutdown_DW.obj_j2.isSetupComplete = true;

    // Start for MATLABSystem: '<S2>/Digital Write'
    CleanShutdown_DW.obj_fn.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_fn.isInitialized = 1;
    MW_gpioInit(25U, true);
    CleanShutdown_DW.obj_fn.isSetupComplete = true;

    // Start for MATLABSystem: '<S3>/Digital Write'
    CleanShutdown_DW.obj_jh.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_jh.isInitialized = 1;
    MW_gpioInit(10U, true);
    CleanShutdown_DW.obj_jh.isSetupComplete = true;

    // Start for MATLABSystem: '<S4>/Digital Write'
    CleanShutdown_DW.obj_j.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_j.isInitialized = 1;
    MW_gpioInit(26U, true);
    CleanShutdown_DW.obj_j.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/PWM1'
    CleanShutdown_DW.obj_i.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_i.isInitialized = 1;
    CleanShutdown_DW.obj_i.PinNumber = 18U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_i.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_i.PinNumber);
    }

    CleanShutdown_DW.obj_i.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/PWM1'

    // Start for MATLABSystem: '<Root>/RED T1 - BLACK T8'
    CleanShutdown_DW.obj_e.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_e.isInitialized = 1;
    CleanShutdown_DW.obj_e.PinNumber = 27U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_e.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_e.PinNumber);
    }

    CleanShutdown_DW.obj_e.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T1 - BLACK T8'

    // Start for MATLABSystem: '<Root>/RED T2 - BLACK T3'
    CleanShutdown_DW.obj_i5.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_i5.isInitialized = 1;
    CleanShutdown_DW.obj_i5.PinNumber = 19U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_i5.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_i5.PinNumber);
    }

    CleanShutdown_DW.obj_i5.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T2 - BLACK T3'

    // Start for MATLABSystem: '<Root>/RED T3'
    CleanShutdown_DW.obj_b.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_b.isInitialized = 1;
    CleanShutdown_DW.obj_b.PinNumber = 4U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_b.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_b.PinNumber);
    }

    CleanShutdown_DW.obj_b.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T3'

    // Start for MATLABSystem: '<Root>/RED T4 - BLACK T5'
    CleanShutdown_DW.obj_d.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_d.isInitialized = 1;
    CleanShutdown_DW.obj_d.PinNumber = 22U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_d.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_d.PinNumber);
    }

    CleanShutdown_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T4 - BLACK T5'

    // Start for MATLABSystem: '<Root>/RED T5 - BLACK T4'
    CleanShutdown_DW.obj_f.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_f.isInitialized = 1;
    CleanShutdown_DW.obj_f.PinNumber = 5U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_f.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_f.PinNumber);
    }

    CleanShutdown_DW.obj_f.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T5 - BLACK T4'

    // Start for MATLABSystem: '<Root>/RED T6 - BLACK T7'
    CleanShutdown_DW.obj_o.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_o.isInitialized = 1;
    CleanShutdown_DW.obj_o.PinNumber = 6U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_o.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_o.PinNumber);
    }

    CleanShutdown_DW.obj_o.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T6 - BLACK T7'

    // Start for MATLABSystem: '<Root>/RED T7 - BLACK T6'
    CleanShutdown_DW.obj_l.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_l.isInitialized = 1;
    CleanShutdown_DW.obj_l.PinNumber = 13U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_l.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_l.PinNumber);
    }

    CleanShutdown_DW.obj_l.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T7 - BLACK T6'

    // Start for MATLABSystem: '<Root>/RED T8 - BLACK T1'
    CleanShutdown_DW.obj_k.matlabCodegenIsDeleted = false;
    CleanShutdown_DW.obj_k.isInitialized = 1;
    CleanShutdown_DW.obj_k.PinNumber = 17U;
    ret = EXT_PWMBlock_init(CleanShutdown_DW.obj_k.PinNumber, 10U, 0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        tmp_0[ret] = tmp[ret];
      }

      printf(tmp_0, CleanShutdown_DW.obj_k.PinNumber);
    }

    CleanShutdown_DW.obj_k.isSetupComplete = true;

    // End of Start for MATLABSystem: '<Root>/RED T8 - BLACK T1'
  }
}

// Model terminate function
void CleanShutdown_terminate(void)
{
  // Terminate for MATLABSystem: '<Root>/BLACK T2'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj);

  // Terminate for MATLABSystem: '<S1>/Digital Write'
  matlabCodegenHandle__gx21kozvdu(&CleanShutdown_DW.obj_j2);

  // Terminate for MATLABSystem: '<S2>/Digital Write'
  matlabCodegenHandle_gx21kozvdug(&CleanShutdown_DW.obj_fn);

  // Terminate for MATLABSystem: '<S3>/Digital Write'
  matlabCodegenHandl_gx21kozvdugn(&CleanShutdown_DW.obj_jh);

  // Terminate for MATLABSystem: '<S4>/Digital Write'
  matlabCodegenHand_gx21kozvdugn1(&CleanShutdown_DW.obj_j);

  // Terminate for MATLABSystem: '<Root>/PWM1'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_i);

  // Terminate for MATLABSystem: '<Root>/RED T1 - BLACK T8'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_e);

  // Terminate for MATLABSystem: '<Root>/RED T2 - BLACK T3'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_i5);

  // Terminate for MATLABSystem: '<Root>/RED T3'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_b);

  // Terminate for MATLABSystem: '<Root>/RED T4 - BLACK T5'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_d);

  // Terminate for MATLABSystem: '<Root>/RED T5 - BLACK T4'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_f);

  // Terminate for MATLABSystem: '<Root>/RED T6 - BLACK T7'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_o);

  // Terminate for MATLABSystem: '<Root>/RED T7 - BLACK T6'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_l);

  // Terminate for MATLABSystem: '<Root>/RED T8 - BLACK T1'
  matlabCodegenHandle_matlabCodeg(&CleanShutdown_DW.obj_k);
}
