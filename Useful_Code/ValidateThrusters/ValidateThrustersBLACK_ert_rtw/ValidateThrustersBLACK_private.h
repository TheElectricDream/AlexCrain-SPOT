/*
 * ValidateThrustersBLACK_private.h
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

#ifndef RTW_HEADER_ValidateThrustersBLACK_private_h_
#define RTW_HEADER_ValidateThrustersBLACK_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "ValidateThrustersBLACK.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

extern void ValidateThrus_IfActionSubsystem(real_T rty_PWM[8],
  P_IfActionSubsystem_ValidateT_T *localP);

#endif                        /* RTW_HEADER_ValidateThrustersBLACK_private_h_ */
