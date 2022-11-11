//
//  Template_v3_08_2020a_private.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Template_v3_08_2020a".
//
//  Model version              : 1.622
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Sun Jul 10 13:38:58 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_Template_v3_08_2020a_private_h_
#define RTW_HEADER_Template_v3_08_2020a_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "Template_v3_08_2020a.h"

// Private macros used by the generated code to access rtModel
#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTFinal
# define rtmSetTFinal(rtm, val)        ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

extern real_T rt_atan2d_snf(real_T u0, real_T u1);
extern real_T rt_roundd_snf(real_T u);
extern real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T maxIndex);
extern void Template_v_MATLABFunction10(const real_T rtu_PWMs[8],
  B_MATLABFunction10_Template_v_T *localB);
extern void Template_v3_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_Template_v3_T *localB);
extern void Templa_CreateRotationMatrix(real_T rtu_Rz,
  B_CreateRotationMatrix_Templa_T *localB);
extern void Temp_CreateRotationMatrix_h(real_T rtu_Rz,
  B_CreateRotationMatrix_Temp_i_T *localB);
extern void Template_v3__MATLABFunction(B_MATLABFunction_Template_v3__T *localB);
extern void Template_v3_08_2_AHRS2_Init(const real_T rtu_0[3],
  DW_AHRS2_Template_v3_08_2020a_T *localDW, P_AHRS2_Template_v3_08_2020a_T
  *localP);
extern void Template_v3_08_2020a_AHRS2(const real_T rtu_0[3], const real_T
  rtu_1[3], const real_T rtu_2[3], B_AHRS2_Template_v3_08_2020a_T *localB,
  DW_AHRS2_Template_v3_08_2020a_T *localDW, P_AHRS2_Template_v3_08_2020a_T
  *localP);
extern void Template__ChangeOrientation(real_T rtu_u, real_T rtu_u_e, real_T
  rtu_u_c, B_ChangeOrientation_Template__T *localB);
extern void T_CalculateRunningMean_Init(B_CalculateRunningMean_Templa_T *localB,
  DW_CalculateRunningMean_Templ_T *localDW, P_CalculateRunningMean_Templa_T
  *localP);
extern void Templa_CalculateRunningMean(real_T rtu_In1,
  B_CalculateRunningMean_Templa_T *localB, DW_CalculateRunningMean_Templ_T
  *localDW);
extern void CalculateRunningMean_o_Init(B_CalculateRunningMean_Temp_f_T *localB,
  DW_CalculateRunningMean_Tem_o_T *localDW, P_CalculateRunningMean_Temp_k_T
  *localP);
extern void Temp_CalculateRunningMean_e(real_T rtu_In1,
  B_CalculateRunningMean_Temp_f_T *localB, DW_CalculateRunningMean_Tem_o_T
  *localDW, P_CalculateRunningMean_Temp_k_T *localP);
extern void Template_v3_CorrectDropouts(real_T rtu_InputRaw, real_T
  rtu_InputRaw_f, B_CorrectDropouts_Template_v3_T *localB);
extern void Template_ChangeBLUEBehavior(P_ChangeBLUEBehavior_Template_T *localP,
  real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Phase0WaitforSynchronizatio(P_Phase0WaitforSynchronizatio_T *localP,
  real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State, real_T *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T *rtd_RED_Tz,
  real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder,
  real_T *rtd_RED_Tz_Wrist, real_T *rtd_WhoAmI);
extern void Template__MATLABFunction2_o(real_T rtu_z,
  B_MATLABFunction2_Template__n_T *localB);
extern void Template_v3_MATLABFunction3(real_T rtu_z,
  B_MATLABFunction3_Template_v3_T *localB);
extern void Template_v3_MATLABFunction4(const real_T rtu_Oy[2], const real_T
  rtu_e_in[2], B_MATLABFunction4_Template_v3_T *localB);
extern void Template_v3_08__REDARM_Init(B_REDARM_Template_v3_08_2020a_T *localB,
  DW_REDARM_Template_v3_08_2020_T *localDW, P_REDARM_Template_v3_08_2020a_T
  *localP);
extern void Template_v3_08_2020a_REDARM(B_REDARM_Template_v3_08_2020a_T *localB,
  DW_REDARM_Template_v3_08_2020_T *localDW, P_REDARM_Template_v3_08_2020a_T
  *localP, real_T *rtd_RED_Rz_Elbow, real_T *rtd_RED_Rz_Shoulder, real_T
  *rtd_RED_Rz_Wrist, real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_Shoulder,
  real_T *rtd_RED_Tz_Wrist);
extern void Template_v_MATLABFunction_o(real_T rtu_u,
  B_MATLABFunction_Template_v_e_T *localB);
extern void Template__MATLABFunction2_l(real_T rtu_z,
  B_MATLABFunction2_Template__c_T *localB);
extern void Template_v3__SubPhase1_Init(B_SubPhase1_Template_v3_08_20_T *localB,
  DW_SubPhase1_Template_v3_08_2_T *localDW, P_SubPhase1_Template_v3_08_20_T
  *localP);
extern void Template_v3_08_20_SubPhase1(B_SubPhase1_Template_v3_08_20_T *localB,
  DW_SubPhase1_Template_v3_08_2_T *localDW, P_SubPhase1_Template_v3_08_20_T
  *localP, real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Px,
  real_T *rtd_BLACK_Py, real_T *rtd_BLACK_Rz, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State, real_T *rtd_Univ_Time);
extern void Template_v_SubPhase1_a_Init(B_SubPhase1_Template_v3_08__g_T *localB,
  DW_SubPhase1_Template_v3_08_l_T *localDW, P_SubPhase1_Template_v3_08__k_T
  *localP);
extern void Template_v3_08__SubPhase1_a(B_SubPhase1_Template_v3_08__g_T *localB,
  DW_SubPhase1_Template_v3_08_l_T *localDW, P_SubPhase1_Template_v3_08__k_T
  *localP, real_T *rtd_BLACK_Px, real_T *rtd_BLACK_Py, real_T *rtd_BLUE_Fx,
  real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T *rtd_BLUE_Py, real_T
  *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T
  *rtd_Univ_Time);
extern void Template_v_MATLABFunction_e(real_T rtu_t,
  B_MATLABFunction_Template_v_j_T *localB);
extern void Template_v3_MATLABFunction1(real_T rtu_t,
  B_MATLABFunction1_Template_v3_T *localB);
extern void Template_v_MATLABFunction_g(real_T rtu_t,
  B_MATLABFunction_Template_v_h_T *localB);
extern void Template_v3_08_202_REDARM_a(B_REDARM_Template_v3_08_202_m_T *localB,
  P_REDARM_Template_v3_08_202_a_T *localP, real_T *rtd_RED_RzD_Elbow, real_T
  *rtd_RED_RzD_Shoulder, real_T *rtd_RED_RzD_Wrist, real_T *rtd_RED_Rz_Elbow,
  real_T *rtd_RED_Rz_Shoulder, real_T *rtd_RED_Rz_Wrist, real_T
  *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist);
extern void T_ChangeBLUEBehavior_c_Init(B_ChangeBLUEBehavior_Templa_k_T *localB,
  DW_ChangeBLUEBehavior_Templ_i_T *localDW, P_ChangeBLUEBehavior_Templa_c_T
  *localP);
extern void Templa_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_Templa_k_T *localB,
  DW_ChangeBLUEBehavior_Templ_i_T *localDW, P_ChangeBLUEBehavior_Templa_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Template_v3_08_2020a_step0(void);
extern void Template_v3_08_2020a_step2(void);

#endif                            // RTW_HEADER_Template_v3_08_2020a_private_h_
