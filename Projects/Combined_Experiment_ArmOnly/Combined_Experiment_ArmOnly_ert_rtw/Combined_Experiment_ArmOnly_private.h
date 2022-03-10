//
//  Combined_Experiment_ArmOnly_private.h
//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  Code generation for model "Combined_Experiment_ArmOnly".
//
//  Model version              : 1.194
//  Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
//  C++ source code generated on : Thu Mar 10 13:54:46 2022
//
//  Target selection: ert.tlc
//  Embedded hardware selection: ARM Compatible->ARM Cortex
//  Code generation objective: Execution efficiency
//  Validation result: Not run


#ifndef RTW_HEADER_Combined_Experiment_ArmOnly_private_h_
#define RTW_HEADER_Combined_Experiment_ArmOnly_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "Combined_Experiment_ArmOnly.h"

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

// Used by FromWorkspace Block: '<S389>/From Workspace'
#ifndef rtInterpolate
# define rtInterpolate(v1,v2,f1,f2)    (((v1)==(v2))?((double)(v1)): (((f1)*((double)(v1)))+((f2)*((double)(v2)))))
#endif

#ifndef rtRound
# define rtRound(v)                    ( ((v) >= 0) ? floor((v) + 0.5) : ceil((v) - 0.5) )
#endif

extern real_T rt_atan2d_snf(real_T u0, real_T u1);
extern real_T rt_roundd_snf(real_T u);
extern void Combined_Ex_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_Combined_Ex_T *localB);
extern void Combin_CreateRotationMatrix(real_T rtu_Rz,
  B_CreateRotationMatrix_Combin_T *localB);
extern void Comb_CreateRotationMatrix_h(real_T rtu_Rz,
  B_CreateRotationMatrix_Comb_i_T *localB);
extern void Combined_Exp_MATLABFunction(B_MATLABFunction_Combined_Exp_T *localB);
extern void Combined_Experim_AHRS2_Init(const real_T rtu_0[3],
  DW_AHRS2_Combined_Experiment__T *localDW, P_AHRS2_Combined_Experiment_A_T
  *localP);
extern void Combined_Experiment_A_AHRS2(const real_T rtu_0[3], const real_T
  rtu_1[3], const real_T rtu_2[3], B_AHRS2_Combined_Experiment_A_T *localB,
  DW_AHRS2_Combined_Experiment__T *localDW, P_AHRS2_Combined_Experiment_A_T
  *localP);
extern void Combined__ChangeOrientation(real_T rtu_u, real_T rtu_u_g, real_T
  rtu_u_d, B_ChangeOrientation_Combined__T *localB);
extern void C_CalculateRunningMean_Init(B_CalculateRunningMean_Combin_T *localB,
  DW_CalculateRunningMean_Combi_T *localDW, P_CalculateRunningMean_Combin_T
  *localP);
extern void Combin_CalculateRunningMean(real_T rtu_In1,
  B_CalculateRunningMean_Combin_T *localB, DW_CalculateRunningMean_Combi_T
  *localDW);
extern void CalculateRunningMean_b_Init(B_CalculateRunningMean_Comb_e_T *localB,
  DW_CalculateRunningMean_Com_e_T *localDW, P_CalculateRunningMean_Comb_p_T
  *localP);
extern void Comb_CalculateRunningMean_a(real_T rtu_In1,
  B_CalculateRunningMean_Comb_e_T *localB, DW_CalculateRunningMean_Com_e_T
  *localDW, P_CalculateRunningMean_Comb_p_T *localP);
extern void Combined_E_MATLABFunction_f(real_T rtu_TIME, real_T rtu_SERVER,
  B_MATLABFunction_Combined_E_l_T *localB);
extern void Combined_ChangeBLUEBehavior(P_ChangeBLUEBehavior_Combined_T *localP,
  real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Phase0WaitforSynchroni_Init(DW_Phase0WaitforSynchronizati_T *localDW);
extern void Phase0WaitforSynchronizatio(DW_Phase0WaitforSynchronizati_T *localDW,
  P_Phase0WaitforSynchronizatio_T *localP, real_T *rtd_BLACK_Fx, real_T
  *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy,
  real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T *rtd_RED_Fx, real_T
  *rtd_RED_Fy, real_T *rtd_RED_Tz, real_T *rtd_RED_Tz_Elbow, real_T
  *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist);
extern void Combined__MATLABFunction2_o(real_T rtu_z,
  B_MATLABFunction2_Combined__n_T *localB);
extern void Combined_Ex_MATLABFunction3(real_T rtu_z,
  B_MATLABFunction3_Combined_Ex_T *localB);
extern void Combined_Ex_MATLABFunction4(const real_T rtu_Oy[2], const real_T
  rtu_e_in[2], B_MATLABFunction4_Combined_Ex_T *localB);
extern void SendPositionCommandsto_Init(DW_SendPositionCommandstoMani_T *localDW);
extern void SendPositionCommandstoManip(real_T rtu_0, real_T rtu_1, real_T rtu_2);
extern void Combined_Experime_SubPhase2(P_SubPhase2_Combined_Experime_T *localP,
  real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State);
extern void Combined__MATLABFunction2_b(real_T rtu_z,
  B_MATLABFunction2_Combined__k_T *localB);
extern void Combined_Exp_SubPhase1_Init(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW, P_SubPhase1_Combined_Experime_T
  *localP);
extern void Combined_E_SubPhase1_Update(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW);
extern void Combined_Experime_SubPhase1(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW, P_SubPhase1_Combined_Experime_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Combined_Experi_SubPhase2_m(P_SubPhase2_Combined_Experi_h_T *localP,
  real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void Combined_Experi_SubPhase1_j(P_SubPhase1_Combined_Experi_m_T *localP,
  real_T *rtd_Float_State, real_T *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T
  *rtd_RED_Tz, real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T
  *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist);
extern void C_ChangeBLUEBehavior_b_Init(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW, P_ChangeBLUEBehavior_Combin_c_T
  *localP);
extern void C_ChangeBLUEBehavior_Update(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW);
extern void Combin_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW, P_ChangeBLUEBehavior_Combin_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State);
extern void SendPositionCommandsto_Term(DW_SendPositionCommandstoMani_T *localDW);
extern void Combined_Experiment_ArmOnly_step0(void);
extern void Combined_Experiment_ArmOnly_step2(void);

#endif                     // RTW_HEADER_Combined_Experiment_ArmOnly_private_h_
