//
//  CleanShutdown.h
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


#ifndef RTW_HEADER_CleanShutdown_h_
#define RTW_HEADER_CleanShutdown_h_
#include <math.h>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "MW_pigs.h"
#include "MW_gpio.h"
#include "CleanShutdown_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Block states (default storage) for system '<Root>'
typedef struct {
  raspi_internal_PWMBlock_Clean_T obj; // '<Root>/BLACK T2'
  raspi_internal_PWMBlock_Clean_T obj_i;// '<Root>/PWM1'
  raspi_internal_PWMBlock_Clean_T obj_e;// '<Root>/RED T1 - BLACK T8'
  raspi_internal_PWMBlock_Clean_T obj_i5;// '<Root>/RED T2 - BLACK T3'
  raspi_internal_PWMBlock_Clean_T obj_b;// '<Root>/RED T3'
  raspi_internal_PWMBlock_Clean_T obj_d;// '<Root>/RED T4 - BLACK T5'
  raspi_internal_PWMBlock_Clean_T obj_f;// '<Root>/RED T5 - BLACK T4'
  raspi_internal_PWMBlock_Clean_T obj_o;// '<Root>/RED T6 - BLACK T7'
  raspi_internal_PWMBlock_Clean_T obj_l;// '<Root>/RED T7 - BLACK T6'
  raspi_internal_PWMBlock_Clean_T obj_k;// '<Root>/RED T8 - BLACK T1'
  codertarget_linux_blocks_Digi_T obj_j;// '<S4>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_jh;// '<S3>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_fn;// '<S2>/Digital Write'
  codertarget_linux_blocks_Digi_T obj_j2;// '<S1>/Digital Write'
} DW_CleanShutdown_T;

// Parameters (default storage)
struct P_CleanShutdown_T_ {
  real_T Constant_Value;               // Expression: 0
                                          //  Referenced by: '<Root>/Constant'

};

// Real-time Model Data Structure
struct tag_RTM_CleanShutdown_T {
  const char_T *errorStatus;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_CleanShutdown_T CleanShutdown_P;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern DW_CleanShutdown_T CleanShutdown_DW;

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void CleanShutdown_initialize(void);
  extern void CleanShutdown_step(void);
  extern void CleanShutdown_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_CleanShutdown_T *const CleanShutdown_M;

#ifdef __cplusplus

}
#endif

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
//  '<Root>' : 'CleanShutdown'
//  '<S1>'   : 'CleanShutdown/GPIO Write'
//  '<S2>'   : 'CleanShutdown/GPIO Write1'
//  '<S3>'   : 'CleanShutdown/GPIO for Magnet'
//  '<S4>'   : 'CleanShutdown/GPIO for Pucks'

#endif                                 // RTW_HEADER_CleanShutdown_h_
