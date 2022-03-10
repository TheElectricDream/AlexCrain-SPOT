//
//  EX2_CircularTrajectory_private.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "EX2_CircularTrajectory".
//
//  Model version              : 1.187
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Thu Mar 10 12:22:53 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_EX2_CircularTrajectory_private_h_
#define RTW_HEADER_EX2_CircularTrajectory_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "EX2_CircularTrajectory.h"

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
extern void EX2_Circula_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_EX2_Circula_T *localB);
extern void EX2_Ci_CreateRotationMatrix(real_T rtu_Rz,
  B_CreateRotationMatrix_EX2_Ci_T *localB);
extern void EX2__CreateRotationMatrix_h(real_T rtu_Rz,
  B_CreateRotationMatrix_EX2__i_T *localB);
extern void EX2_Circular_MATLABFunction(B_MATLABFunction_EX2_Circular_T *localB);
extern void EX2_CircularTraj_AHRS2_Init(const real_T rtu_0[3],
  DW_AHRS2_EX2_CircularTrajecto_T *localDW, P_AHRS2_EX2_CircularTrajector_T
  *localP);
extern void EX2_CircularTrajector_AHRS2(const real_T rtu_0[3], const real_T
  rtu_1[3], const real_T rtu_2[3], B_AHRS2_EX2_CircularTrajector_T *localB,
  DW_AHRS2_EX2_CircularTrajecto_T *localDW, P_AHRS2_EX2_CircularTrajector_T
  *localP);
extern void EX2_Circu_ChangeOrientation(real_T rtu_u, real_T rtu_u_g, real_T
  rtu_u_d, B_ChangeOrientation_EX2_Circu_T *localB);
extern void E_CalculateRunningMean_Init(B_CalculateRunningMean_EX2_Ci_T *localB,
  DW_CalculateRunningMean_EX2_C_T *localDW, P_CalculateRunningMean_EX2_Ci_T
  *localP);
extern void EX2_Ci_CalculateRunningMean(real_T rtu_In1,
  B_CalculateRunningMean_EX2_Ci_T *localB, DW_CalculateRunningMean_EX2_C_T
  *localDW);
extern void CalculateRunningMean_c_Init(B_CalculateRunningMean_EX2__e_T *localB,
  DW_CalculateRunningMean_EX2_e_T *localDW, P_CalculateRunningMean_EX2__p_T
  *localP);
extern void EX2__CalculateRunningMean_a(real_T rtu_In1,
  B_CalculateRunningMean_EX2__e_T *localB, DW_CalculateRunningMean_EX2_e_T
  *localDW, P_CalculateRunningMean_EX2__p_T *localP);
extern void EX2_Circul_MATLABFunction_f(real_T rtu_TIME, real_T rtu_SERVER,
  B_MATLABFunction_EX2_Circul_l_T *localB);
extern void EX2_Circ_ChangeBLUEBehavior(P_ChangeBLUEBehavior_EX2_Circ_T *localP,
  real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Phase0WaitforSynchronizatio(P_Phase0WaitforSynchronizatio_T *localP,
  real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State, real_T *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T *rtd_RED_Tz,
  real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder,
  real_T *rtd_RED_Tz_Wrist);
extern void EX2_Circu_MATLABFunction2_o(real_T rtu_z,
  B_MATLABFunction2_EX2_Circu_n_T *localB);
extern void EX2_Circula_MATLABFunction3(real_T rtu_z,
  B_MATLABFunction3_EX2_Circula_T *localB);
extern void EX2_Circula_MATLABFunction4(const real_T rtu_Oy[2], const real_T
  rtu_e_in[2], B_MATLABFunction4_EX2_Circula_T *localB);
extern void EX2_CircularTraje_SubPhase2(P_SubPhase2_EX2_CircularTraje_T *localP,
  real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State);
extern void EX2_Circu_MATLABFunction2_b(real_T rtu_z,
  B_MATLABFunction2_EX2_Circu_k_T *localB);
extern void EX2_Circular_SubPhase1_Init(B_SubPhase1_EX2_CircularTraje_T *localB,
  DW_SubPhase1_EX2_CircularTraj_T *localDW, P_SubPhase1_EX2_CircularTraje_T
  *localP);
extern void EX2_CircularTraje_SubPhase1(B_SubPhase1_EX2_CircularTraje_T *localB,
  DW_SubPhase1_EX2_CircularTraj_T *localDW, P_SubPhase1_EX2_CircularTraje_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void EX2_CircularTra_SubPhase2_m(P_SubPhase2_EX2_CircularTra_h_T *localP,
  real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void EX2_CircularTra_SubPhase2_g(P_SubPhase2_EX2_CircularTra_g_T *localP,
  real_T *rtd_Float_State, real_T *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T
  *rtd_RED_Tz, real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T
  *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist);
extern void E_ChangeBLUEBehavior_h_Init(B_ChangeBLUEBehavior_EX2_Ci_k_T *localB,
  DW_ChangeBLUEBehavior_EX2_C_i_T *localDW, P_ChangeBLUEBehavior_EX2_Ci_c_T
  *localP);
extern void EX2_Ci_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_EX2_Ci_k_T *localB,
  DW_ChangeBLUEBehavior_EX2_C_i_T *localDW, P_ChangeBLUEBehavior_EX2_Ci_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void EX2_CircularTrajectory_step0(void);
extern void EX2_CircularTrajectory_step2(void);

#endif                          // RTW_HEADER_EX2_CircularTrajectory_private_h_
