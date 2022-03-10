//
//  Combined_Experiment_ArmOnly.cpp
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


#include "Combined_Experiment_ArmOnly.h"
#include "Combined_Experiment_ArmOnly_private.h"

// Block signals (default storage)
B_Combined_Experiment_ArmOnly_T Combined_Experiment_ArmOnly_B;

// Block states (default storage)
DW_Combined_Experiment_ArmOnl_T Combined_Experiment_ArmOnly_DW;

// Previous zero-crossings (trigger) states
PrevZCX_Combined_Experiment_A_T Combined_Experiment_Arm_PrevZCX;

// Real-time model
RT_MODEL_Combined_Experiment__T Combined_Experiment_ArmOnly_M_ =
  RT_MODEL_Combined_Experiment__T();
RT_MODEL_Combined_Experiment__T *const Combined_Experiment_ArmOnly_M =
  &Combined_Experiment_ArmOnly_M_;

// Forward declaration for local functions
static void Combin_AHRSFilterBase_resetImpl(fusion_simulink_ahrsfilter_Co_T *obj);
static void IMUFusionCommon_computeAngularV(const real_T gfast[3], const real_T
  offset[3], real_T av[3]);
static void Combined_Experimen_NED_ecompass(const real_T a[3], const real_T m[3],
  real_T R[9], B_AHRS2_Combined_Experiment_A_T *localB);
static void Combi_quaternioncg_quaternioncg(const real_T varargin_1[9], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Combined_Experiment_A_T *localB);
static void Com_quaternioncg_quaternioncg_f(const real_T varargin_1[3], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Combined_Experiment_A_T *localB);
static void Combined__quaternionBase_mtimes(real_T x_a, real_T x_b, real_T x_c,
  real_T x_d, real_T y_a, real_T y_b, real_T y_c, real_T y_d, real_T *o_a,
  real_T *o_b, real_T *o_c, real_T *o_d);
static void IMUFusionCommon_predictOrientat(const
  fusion_simulink_ahrsfilter_Co_T *obj, const real_T gfast[3], const real_T
  offset[3], real_T qorient_a, real_T qorient_b, real_T qorient_c, real_T
  qorient_d, real_T *b_qorient_a, real_T *b_qorient_b, real_T *b_qorient_c,
  real_T *b_qorient_d, B_AHRS2_Combined_Experiment_A_T *localB);
static void Combined__quaternionBase_rotmat(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T r[9], B_AHRS2_Combined_Experiment_A_T *localB);
static void Comb_IMUFusionCommon_buildHPart(const real_T v[3], real_T h[9],
  B_AHRS2_Combined_Experiment_A_T *localB);
static real_T Combined_Experiment_ArmOnl_norm(const real_T x[3],
  B_AHRS2_Combined_Experiment_A_T *localB);
static void Combined_Ex_quaternionBase_conj(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d);
static void Combined__quaternionBase_uminus(real_T obj_a, real_T obj_b, real_T
  obj_c, real_T obj_d, real_T *b_obj_a, real_T *b_obj_b, real_T *b_obj_c, real_T
  *b_obj_d);
static void Combin_quaternionBase_normalize(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d);
static void Combin_quaternioncg_parenAssign(real_T rhs_a, real_T rhs_b, real_T
  rhs_c, real_T rhs_d, real_T *o_a, real_T *o_b, real_T *o_c, real_T *o_d);
static void Combined_Ex_ahrsfilter_stepImpl(fusion_simulink_ahrsfilter_Co_T *obj,
  const real_T accelIn[3], const real_T gyroIn[3], const real_T magIn[3], real_T
  orientOut[4], real_T av[3], B_AHRS2_Combined_Experiment_A_T *localB);

// Forward declaration for local functions
static void matlabCodegenHandle_matlabCodeg(MoveArm_Position_Combined_Exp_T *obj);

// Forward declaration for local functions
static real_T Combined_Experime_rt_atan2d_snf(real_T u0, real_T u1);
static real_T Comb_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u);
static real_T Comb_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u);
static void Combined_Experim_rt_invd3x3_snf(const real_T u[9], real_T y[9]);
static void Combined_E_SystemCore_release_b(const
  codertarget_linux_blocks_Digi_T *obj);
static void Combined_E_SystemCore_delete_ba(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCo_ba(codertarget_linux_blocks_Digi_T *obj);
static void Combined__SystemCore_release_ba(const
  codertarget_linux_blocks_Digi_T *obj);
static void Combined__SystemCore_delete_ba5(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabC_ba5(codertarget_linux_blocks_Digi_T *obj);
static void C_SystemCore_rele_a(const PhaseSpace_ALL_Combined_Exper_T *obj);
static void C_SystemCore_dele_p(const PhaseSpace_ALL_Combined_Exper_T *obj);
static void Combined_Expe_matlabCodegenHa_p(PhaseSpace_ALL_Combined_Exper_T *obj);
static void C_SystemCore_rele_p(const raspi_internal_lsm9ds1Block_C_T *obj);
static void C_SystemCore_dele_l(const raspi_internal_lsm9ds1Block_C_T *obj);
static void Combined_Expe_matlabCodegenHa_l(raspi_internal_lsm9ds1Block_C_T *obj);
static void C_SystemCore_rele_k(j_codertarget_raspi_internal__T *obj);
static void C_SystemCore_dele_e(j_codertarget_raspi_internal__T *obj);
static void Combined_Expe_matlabCodegenHa_e(j_codertarget_raspi_internal__T *obj);
static void C_SystemCore_rele_l(i_codertarget_raspi_internal__T *obj);
static void C_SystemCore_dele_k(i_codertarget_raspi_internal__T *obj);
static void Combined_Expe_matlabCodegenHa_k(i_codertarget_raspi_internal__T *obj);
static void Combined_Exp_SystemCore_release(const
  InitializeForPosition_Combine_T *obj);
static void Combined_Ex_SystemCore_delete_b(const
  InitializeForPosition_Combine_T *obj);
static void matlabCodegenHandle_matlabCod_b(InitializeForPosition_Combine_T *obj);
static void Combined_SystemCore_release_ba5(const
  raspi_internal_PWMBlock_Combi_T *obj);
static void Combined_SystemCore_delete_ba5m(const
  raspi_internal_PWMBlock_Combi_T *obj);
static void matlabCodegenHandle_matlab_ba5m(raspi_internal_PWMBlock_Combi_T *obj);
static void SystemCore_release_ba5m25ew4zuq(const
  codertarget_linux_blocks_Di_b_T *obj);
static void SystemCore_delete_ba5m25ew4zuqz(const
  codertarget_linux_blocks_Di_b_T *obj);
static void matlabCodegenHand_ba5m25ew4zuqz(codertarget_linux_blocks_Di_b_T *obj);
static void SystemCore_releas_ba5m25ew4zuqz(const
  codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_delet_ba5m25ew4zuqzr(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHan_ba5m25ew4zuqzr(codertarget_linux_blocks_Digi_T *obj);
static void SystemCore_rele_ba5m25ew4zuqzro(const
  codertarget_linux_blocks_Digi_T *obj);
static void C_SystemCore_dele_a(const codertarget_linux_blocks_Digi_T *obj);
static void Combined_Expe_matlabCodegenHa_a(codertarget_linux_blocks_Digi_T *obj);
static raspi_internal_lsm9ds1Block_C_T *Combi_lsm9ds1Block_lsm9ds1Block
  (raspi_internal_lsm9ds1Block_C_T *obj);
static void Combined_Exper_SystemCore_setup(raspi_internal_lsm9ds1Block_C_T *obj);
static void rate_monotonic_scheduler(void);

//
// Set which subrates need to run this base step (base rate always runs).
// This function must be called prior to calling the model step function
// in order to "remember" which rates need to run this base step.  The
// buffering of events allows for overlapping preemption.
//
void Combined_Experiment_ArmOnly_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  // Task runs when its counter is zero, computed via rtmStepTask macro
  eventFlags[2] = ((boolean_T)rtmStepTask(Combined_Experiment_ArmOnly_M, 2));
}

//
//   This function updates active task flag for each subrate
// and rate transition flags for tasks that exchange data.
// The function assumes rate-monotonic multitasking scheduler.
// The function must be called at model base rate so that
// the generated code self-manages all its subrates and rate
// transition flags.
//
static void rate_monotonic_scheduler(void)
{
  // To ensure a deterministic data transfer between two rates,
  //  data is transferred at the priority of a fast task and the frequency
  //  of the slow task.  The following flags indicate when the data transfer
  //  happens.  That is, a rate interaction flag is set true when both rates
  //  will run, and false otherwise.


  // tid 1 shares data with slower tid rate: 2
  if (Combined_Experiment_ArmOnly_M->Timing.TaskCounters.TID[1] == 0) {
    Combined_Experiment_ArmOnly_M->Timing.RateInteraction.TID1_2 =
      (Combined_Experiment_ArmOnly_M->Timing.TaskCounters.TID[2] == 0);
  }

  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (Combined_Experiment_ArmOnly_M->Timing.TaskCounters.TID[2])++;
  if ((Combined_Experiment_ArmOnly_M->Timing.TaskCounters.TID[2]) > 1) {// Sample time: [0.1s, 0.0s] 
    Combined_Experiment_ArmOnly_M->Timing.TaskCounters.TID[2] = 0;
  }
}

//
// Output and update for atomic system:
//    '<S27>/MATLAB Function2'
//    '<S36>/MATLAB Function2'
//    '<S45>/MATLAB Function2'
//
void Combined_Ex_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_Combined_Ex_T *localB)
{
  real_T ex;
  int32_T idx;
  int32_T k;
  boolean_T tmp;
  boolean_T exitg1;

  // MATLAB Function 'From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function2': '<S31>:1' 
  // '<S31>:1:3' ValveTime       = 0.007;
  // '<S31>:1:4' TControl        = 0.1;
  // '<S31>:1:5' ThrustPer_Final = zeros(8,1);
  // '<S31>:1:7' if max(ThrustPer) > 1
  tmp = rtIsNaN(rtu_ThrustPer[0]);
  if (!tmp) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 9)) {
      if (!rtIsNaN(rtu_ThrustPer[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    ex = rtu_ThrustPer[0];
  } else {
    ex = rtu_ThrustPer[idx - 1];
    while (idx + 1 < 9) {
      if (ex < rtu_ThrustPer[idx]) {
        ex = rtu_ThrustPer[idx];
      }

      idx++;
    }
  }

  if (ex > 1.0) {
    // '<S31>:1:8' ThrustPer_Sat = ThrustPer/max(ThrustPer);
    if (!tmp) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 9)) {
        if (!rtIsNaN(rtu_ThrustPer[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (idx == 0) {
      ex = rtu_ThrustPer[0];
    } else {
      ex = rtu_ThrustPer[idx - 1];
      while (idx + 1 < 9) {
        if (ex < rtu_ThrustPer[idx]) {
          ex = rtu_ThrustPer[idx];
        }

        idx++;
      }
    }

    for (idx = 0; idx < 8; idx++) {
      localB->ThrustPer_Sat[idx] = rtu_ThrustPer[idx] / ex;
    }
  } else {
    // '<S31>:1:9' else
    // '<S31>:1:10' ThrustPer_Sat = ThrustPer;
    memcpy(&localB->ThrustPer_Sat[0], &rtu_ThrustPer[0], sizeof(real_T) << 3U);
  }

  // '<S31>:1:13' for i = 1:8
  for (idx = 0; idx < 8; idx++) {
    // '<S31>:1:14' if ThrustPer_Sat(i) > ValveTime/TControl
    if (localB->ThrustPer_Sat[idx] > 0.069999999999999993) {
      // '<S31>:1:15' ThrustPer_Final(i) = ThrustPer_Sat(i);
      localB->ThrustPer_Final[idx] = localB->ThrustPer_Sat[idx];
    } else {
      // '<S31>:1:16' else
      // '<S31>:1:17' ThrustPer_Final(i) = 0;
      localB->ThrustPer_Final[idx] = 0.0;
    }
  }
}

//
// Output and update for atomic system:
//    '<S32>/Create Rotation Matrix'
//    '<S41>/Create Rotation Matrix'
//    '<S50>/Create Rotation Matrix'
//    '<S169>/Create Rotation Matrix'
//    '<S170>/Create Rotation Matrix'
//    '<S171>/Create Rotation Matrix'
//
void Combin_CreateRotationMatrix(real_T rtu_Rz, B_CreateRotationMatrix_Combin_T *
  localB)
{
  real_T r;
  real_T t;
  real_T C_bI_idx_0;
  real_T C_bI_idx_2;
  real_T C_bI_idx_1;

  // MATLAB Function 'From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix': '<S34>:1' 
  // '<S34>:1:3' C_bI = [  cos(Rz) sin(Rz)
  // '<S34>:1:4'          -sin(Rz) cos(Rz) ];
  r = sin(rtu_Rz);
  t = cos(rtu_Rz);
  C_bI_idx_0 = t;
  C_bI_idx_2 = r;
  C_bI_idx_1 = -r;

  // '<S34>:1:6' C_Ib = inv(C_bI);
  if (fabs(-r) > fabs(t)) {
    r = t / -r;
    t = 1.0 / (r * t - C_bI_idx_2);
    localB->C_Ib[0] = C_bI_idx_0 / C_bI_idx_1 * t;
    localB->C_Ib[1] = -t;
    localB->C_Ib[2] = -C_bI_idx_2 / C_bI_idx_1 * t;
    localB->C_Ib[3] = r * t;
  } else {
    r = -r / t;
    t = 1.0 / (t - r * C_bI_idx_2);
    localB->C_Ib[0] = C_bI_idx_0 / C_bI_idx_0 * t;
    localB->C_Ib[1] = -r * t;
    localB->C_Ib[2] = -C_bI_idx_2 / C_bI_idx_0 * t;
    localB->C_Ib[3] = t;
  }
}

//
// Output and update for atomic system:
//    '<S28>/Create Rotation Matrix'
//    '<S37>/Create Rotation Matrix'
//    '<S46>/Create Rotation Matrix'
//
void Comb_CreateRotationMatrix_h(real_T rtu_Rz, B_CreateRotationMatrix_Comb_i_T *
  localB)
{
  real_T C_bI_tmp;
  real_T C_bI_tmp_0;

  // MATLAB Function 'From Force//Torque to PWM Signal/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix': '<S35>:1' 
  // '<S35>:1:3' C_bI = [  cos(Rz) sin(Rz)
  // '<S35>:1:4'          -sin(Rz) cos(Rz) ];
  C_bI_tmp = sin(rtu_Rz);
  C_bI_tmp_0 = cos(rtu_Rz);
  localB->C_bI[0] = C_bI_tmp_0;
  localB->C_bI[2] = C_bI_tmp;
  localB->C_bI[1] = -C_bI_tmp;
  localB->C_bI[3] = C_bI_tmp_0;
}

//
// Output and update for atomic system:
//    '<S36>/MATLAB Function'
//    '<S36>/MATLAB Function1'
//
void Combined_Exp_MATLABFunction(B_MATLABFunction_Combined_Exp_T *localB)
{
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T H_tmp;
  static const int8_T b[8] = { -1, -1, 0, 0, 1, 1, 0, 0 };

  static const int8_T c[8] = { 0, 0, 1, 1, 0, 0, -1, -1 };

  // MATLAB Function 'From Force//Torque to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S38>:1' 
  // '<S38>:1:3' Vec1 = [ -1
  // '<S38>:1:4'          -1
  // '<S38>:1:5'           0
  // '<S38>:1:6'           0
  // '<S38>:1:7'           1
  // '<S38>:1:8'           1
  // '<S38>:1:9'           0
  // '<S38>:1:10'           0 ];
  // '<S38>:1:12' Vec2 = [  0
  // '<S38>:1:13'           0
  // '<S38>:1:14'           1
  // '<S38>:1:15'           1
  // '<S38>:1:16'           0
  // '<S38>:1:17'           0
  // '<S38>:1:18'          -1
  // '<S38>:1:19'          -1 ];
  // '<S38>:1:21' Vec3 = thruster_dist2CG_BLUE./1000;
  // '<S38>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
  // '<S38>:1:25' Mat2 = diag((F_thrusters_BLUE./2));
  for (i = 0; i < 8; i++) {
    localB->v[i] = Combined_Experiment_ArmOnly_P.F_thrusters_BLUE[i] / 2.0;
  }

  memset(&localB->Mat2[0], 0, sizeof(real_T) << 6U);

  // '<S38>:1:27' H    = Mat1*Mat2;
  for (i = 0; i < 8; i++) {
    localB->Mat2[i + (i << 3)] = localB->v[i];
    localB->b[3 * i] = b[i];
    localB->b[3 * i + 1] = c[i];
    localB->b[3 * i + 2] = Combined_Experiment_ArmOnly_P.thruster_dist2CG_BLUE[i]
      / 1000.0;
  }

  for (i = 0; i < 8; i++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      H_tmp = i_0 + 3 * i;
      localB->H[H_tmp] = 0.0;
      for (i_1 = 0; i_1 < 8; i_1++) {
        localB->H[H_tmp] += localB->b[3 * i_1 + i_0] * localB->Mat2[(i << 3) +
          i_1];
      }
    }
  }
}

static void Combin_AHRSFilterBase_resetImpl(fusion_simulink_ahrsfilter_Co_T *obj)
{
  real_T accelMeasNoiseVar;
  real_T magMeasNoiseVar;
  static const real_T tmp[144] = { 6.0923483957341713E-6, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6.0923483957341713E-6, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6.0923483957341713E-6, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.6154354946677142E-5, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.6154354946677142E-5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    7.6154354946677142E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0096236100000000012, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0096236100000000012, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0096236100000000012, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.6 };

  int32_T i;
  int32_T tmp_0;
  int32_T tmp_1;
  static const int8_T tmp_2[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  obj->pOrientPost.a = 1.0;
  obj->pOrientPost.b = 0.0;
  obj->pOrientPost.c = 0.0;
  obj->pOrientPost.d = 0.0;
  obj->pGyroOffset[0] = 0.0;
  obj->pMagVec[0] = 0.0;
  obj->pGyroOffset[1] = 0.0;
  obj->pMagVec[1] = 0.0;
  obj->pGyroOffset[2] = 0.0;
  obj->pMagVec[2] = 0.0;
  obj->pMagVec[0] = obj->ExpectedMagneticFieldStrength;
  magMeasNoiseVar = obj->pKalmanPeriod * obj->pKalmanPeriod *
    (obj->GyroscopeDriftNoise + obj->GyroscopeNoise);
  accelMeasNoiseVar = magMeasNoiseVar + (obj->AccelerometerNoise +
    obj->LinearAccelerationNoise);
  magMeasNoiseVar += obj->MagnetometerNoise + obj->MagneticDisturbanceNoise;
  memset(&obj->pQv[0], 0, 36U * sizeof(real_T));
  for (i = 0; i < 3; i++) {
    tmp_0 = tmp_2[3 * i];
    obj->pQv[6 * i] = accelMeasNoiseVar * static_cast<real_T>(tmp_0);
    tmp_1 = 6 * (i + 3);
    obj->pQv[tmp_1 + 3] = magMeasNoiseVar * static_cast<real_T>(tmp_0);
    tmp_0 = tmp_2[3 * i + 1];
    obj->pQv[6 * i + 1] = accelMeasNoiseVar * static_cast<real_T>(tmp_0);
    obj->pQv[tmp_1 + 4] = magMeasNoiseVar * static_cast<real_T>(tmp_0);
    tmp_0 = tmp_2[3 * i + 2];
    obj->pQv[6 * i + 2] = accelMeasNoiseVar * static_cast<real_T>(tmp_0);
    obj->pQv[tmp_1 + 5] = magMeasNoiseVar * static_cast<real_T>(tmp_0);
  }

  memcpy(&obj->pQw[0], &tmp[0], 144U * sizeof(real_T));
  obj->pLinAccelPost[0] = 0.0;
  obj->pLinAccelPost[1] = 0.0;
  obj->pLinAccelPost[2] = 0.0;
  obj->pFirstTime = true;
}

static void IMUFusionCommon_computeAngularV(const real_T gfast[3], const real_T
  offset[3], real_T av[3])
{
  av[0] = gfast[0] - offset[0];
  av[1] = gfast[1] - offset[1];
  av[2] = gfast[2] - offset[2];
}

static void Combined_Experimen_NED_ecompass(const real_T a[3], const real_T m[3],
  real_T R[9], B_AHRS2_Combined_Experiment_A_T *localB)
{
  boolean_T nanPageIdx;
  boolean_T y[3];
  int32_T iy;
  int32_T xpageoffset;
  boolean_T exitg1;
  localB->Reast[0] = a[1] * m[2] - a[2] * m[1];
  localB->Reast[1] = a[2] * m[0] - a[0] * m[2];
  localB->Reast[2] = a[0] * m[1] - a[1] * m[0];
  R[6] = a[0];
  R[3] = localB->Reast[0];
  R[7] = a[1];
  R[4] = localB->Reast[1];
  R[8] = a[2];
  R[5] = localB->Reast[2];
  R[0] = localB->Reast[1] * a[2] - localB->Reast[2] * a[1];
  R[1] = localB->Reast[2] * a[0] - localB->Reast[0] * a[2];
  R[2] = localB->Reast[0] * a[1] - localB->Reast[1] * a[0];
  for (iy = 0; iy < 9; iy++) {
    localB->x[iy] = R[iy] * R[iy];
  }

  for (iy = 0; iy < 3; iy++) {
    xpageoffset = iy * 3;
    localB->Reast[iy] = localB->x[xpageoffset + 2] + (localB->x[xpageoffset + 1]
      + localB->x[xpageoffset]);
  }

  localB->Reast[0] = sqrt(localB->Reast[0]);
  localB->Reast[1] = sqrt(localB->Reast[1]);
  localB->Reast[2] = sqrt(localB->Reast[2]);
  memcpy(&localB->x[0], &R[0], 9U * sizeof(real_T));
  for (iy = 0; iy < 3; iy++) {
    R[3 * iy] = localB->x[3 * iy] / localB->Reast[iy];
    xpageoffset = 3 * iy + 1;
    R[xpageoffset] = localB->x[xpageoffset] / localB->Reast[iy];
    xpageoffset = 3 * iy + 2;
    R[xpageoffset] = localB->x[xpageoffset] / localB->Reast[iy];
  }

  for (iy = 0; iy < 9; iy++) {
    localB->b[iy] = rtIsNaN(R[iy]);
  }

  y[0] = false;
  y[1] = false;
  y[2] = false;
  xpageoffset = 1;
  exitg1 = false;
  while ((!exitg1) && (xpageoffset <= 3)) {
    if (!localB->b[xpageoffset - 1]) {
      xpageoffset++;
    } else {
      y[0] = true;
      exitg1 = true;
    }
  }

  xpageoffset = 4;
  exitg1 = false;
  while ((!exitg1) && (xpageoffset <= 6)) {
    if (!localB->b[xpageoffset - 1]) {
      xpageoffset++;
    } else {
      y[1] = true;
      exitg1 = true;
    }
  }

  xpageoffset = 7;
  exitg1 = false;
  while ((!exitg1) && (xpageoffset <= 9)) {
    if (!localB->b[xpageoffset - 1]) {
      xpageoffset++;
    } else {
      y[2] = true;
      exitg1 = true;
    }
  }

  nanPageIdx = false;
  iy = 0;
  exitg1 = false;
  while ((!exitg1) && (iy < 3)) {
    if (!y[iy]) {
      iy++;
    } else {
      nanPageIdx = true;
      exitg1 = true;
    }
  }

  if (nanPageIdx) {
    memset(&R[0], 0, 9U * sizeof(real_T));
    R[0] = 1.0;
    R[4] = 1.0;
    R[8] = 1.0;
  }
}

static void Combi_quaternioncg_quaternioncg(const real_T varargin_1[9], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Combined_Experiment_A_T *localB)
{
  int32_T b_k;
  int32_T b_idx;
  int32_T b;
  boolean_T exitg1;
  localB->tr = (varargin_1[0] + varargin_1[4]) + varargin_1[8];
  localB->psquared[0] = (localB->tr * 2.0 + 1.0) - localB->tr;
  localB->psquared[1] = (2.0 * varargin_1[0] + 1.0) - localB->tr;
  localB->psquared[2] = (2.0 * varargin_1[4] + 1.0) - localB->tr;
  localB->psquared[3] = (2.0 * varargin_1[8] + 1.0) - localB->tr;
  if (!rtIsNaN(localB->psquared[0])) {
    b_idx = 1;
  } else {
    b_idx = 0;
    b_k = 2;
    exitg1 = false;
    while ((!exitg1) && (b_k < 5)) {
      if (!rtIsNaN(localB->psquared[b_k - 1])) {
        b_idx = b_k;
        exitg1 = true;
      } else {
        b_k++;
      }
    }
  }

  if (b_idx == 0) {
    localB->tr = localB->psquared[0];
    b_idx = 1;
  } else {
    localB->tr = localB->psquared[b_idx - 1];
    b = b_idx;
    for (b_k = b + 1; b_k < 5; b_k++) {
      localB->d = localB->psquared[b_k - 1];
      if (localB->tr < localB->d) {
        localB->tr = localB->d;
        b_idx = b_k;
      }
    }
  }

  switch (b_idx) {
   case 1:
    localB->tr = sqrt(localB->tr);
    *obj_a = 0.5 * localB->tr;
    localB->tr = 0.5 / localB->tr;
    *obj_b = (varargin_1[7] - varargin_1[5]) * localB->tr;
    *obj_c = (varargin_1[2] - varargin_1[6]) * localB->tr;
    *obj_d = (varargin_1[3] - varargin_1[1]) * localB->tr;
    break;

   case 2:
    localB->tr = sqrt(localB->tr);
    *obj_b = 0.5 * localB->tr;
    localB->tr = 0.5 / localB->tr;
    *obj_a = (varargin_1[7] - varargin_1[5]) * localB->tr;
    *obj_c = (varargin_1[3] + varargin_1[1]) * localB->tr;
    *obj_d = (varargin_1[2] + varargin_1[6]) * localB->tr;
    break;

   case 3:
    localB->tr = sqrt(localB->tr);
    *obj_c = 0.5 * localB->tr;
    localB->tr = 0.5 / localB->tr;
    *obj_a = (varargin_1[2] - varargin_1[6]) * localB->tr;
    *obj_b = (varargin_1[3] + varargin_1[1]) * localB->tr;
    *obj_d = (varargin_1[7] + varargin_1[5]) * localB->tr;
    break;

   default:
    localB->tr = sqrt(localB->tr);
    *obj_d = 0.5 * localB->tr;
    localB->tr = 0.5 / localB->tr;
    *obj_a = (varargin_1[3] - varargin_1[1]) * localB->tr;
    *obj_b = (varargin_1[2] + varargin_1[6]) * localB->tr;
    *obj_c = (varargin_1[7] + varargin_1[5]) * localB->tr;
    break;
  }

  if (*obj_a < 0.0) {
    *obj_a = -*obj_a;
    *obj_b = -*obj_b;
    *obj_c = -*obj_c;
    *obj_d = -*obj_d;
  }
}

static void Com_quaternioncg_quaternioncg_f(const real_T varargin_1[3], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Combined_Experiment_A_T *localB)
{
  real_T st;
  *obj_a = 1.0;
  *obj_b = 0.0;
  *obj_c = 0.0;
  *obj_d = 0.0;
  localB->theta = sqrt((varargin_1[0] * varargin_1[0] + varargin_1[1] *
                        varargin_1[1]) + varargin_1[2] * varargin_1[2]);
  st = sin(localB->theta / 2.0);
  if (localB->theta != 0.0) {
    *obj_a = cos(localB->theta / 2.0);
    *obj_b = varargin_1[0] / localB->theta * st;
    *obj_c = varargin_1[1] / localB->theta * st;
    *obj_d = varargin_1[2] / localB->theta * st;
  }
}

static void Combined__quaternionBase_mtimes(real_T x_a, real_T x_b, real_T x_c,
  real_T x_d, real_T y_a, real_T y_b, real_T y_c, real_T y_d, real_T *o_a,
  real_T *o_b, real_T *o_c, real_T *o_d)
{
  *o_a = ((x_a * y_a - x_b * y_b) - x_c * y_c) - x_d * y_d;
  *o_b = ((x_a * y_b + x_b * y_a) + x_c * y_d) - x_d * y_c;
  *o_c = ((x_a * y_c - x_b * y_d) + x_c * y_a) + x_d * y_b;
  *o_d = ((x_a * y_d + x_b * y_c) - x_c * y_b) + x_d * y_a;
}

static void IMUFusionCommon_predictOrientat(const
  fusion_simulink_ahrsfilter_Co_T *obj, const real_T gfast[3], const real_T
  offset[3], real_T qorient_a, real_T qorient_b, real_T qorient_c, real_T
  qorient_d, real_T *b_qorient_a, real_T *b_qorient_b, real_T *b_qorient_c,
  real_T *b_qorient_d, B_AHRS2_Combined_Experiment_A_T *localB)
{
  localB->c[0] = (gfast[0] - offset[0]) * obj->pSensorPeriod;
  localB->c[1] = (gfast[1] - offset[1]) * obj->pSensorPeriod;
  localB->c[2] = (gfast[2] - offset[2]) * obj->pSensorPeriod;
  Com_quaternioncg_quaternioncg_f(localB->c, &localB->assign_temp_a_k,
    &localB->assign_temp_b_c, &localB->assign_temp_c_b, &localB->assign_temp_d_p,
    localB);
  Combined__quaternionBase_mtimes(qorient_a, qorient_b, qorient_c, qorient_d,
    localB->assign_temp_a_k, localB->assign_temp_b_c, localB->assign_temp_c_b,
    localB->assign_temp_d_p, b_qorient_a, b_qorient_b, b_qorient_c, b_qorient_d);
  if (*b_qorient_a < 0.0) {
    *b_qorient_a = -*b_qorient_a;
    *b_qorient_b = -*b_qorient_b;
    *b_qorient_c = -*b_qorient_c;
    *b_qorient_d = -*b_qorient_d;
  }
}

static void Combined__quaternionBase_rotmat(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T r[9], B_AHRS2_Combined_Experiment_A_T *localB)
{
  real_T n;
  n = sqrt(((q_a * q_a + q_b * q_b) + q_c * q_c) + q_d * q_d);
  q_a /= n;
  q_b /= n;
  q_c /= n;
  q_d /= n;
  n = q_a * q_b * 2.0;
  localB->ac2 = q_a * q_c * 2.0;
  localB->ad2 = q_a * q_d * 2.0;
  localB->bc2 = q_b * q_c * 2.0;
  localB->bd2 = q_b * q_d * 2.0;
  localB->cd2 = q_c * q_d * 2.0;
  localB->aasq = q_a * q_a * 2.0 - 1.0;
  r[0] = q_b * q_b * 2.0 + localB->aasq;
  r[3] = localB->bc2 + localB->ad2;
  r[6] = localB->bd2 - localB->ac2;
  r[1] = localB->bc2 - localB->ad2;
  r[4] = q_c * q_c * 2.0 + localB->aasq;
  r[7] = localB->cd2 + n;
  r[2] = localB->bd2 + localB->ac2;
  r[5] = localB->cd2 - n;
  r[8] = q_d * q_d * 2.0 + localB->aasq;
}

static void Comb_IMUFusionCommon_buildHPart(const real_T v[3], real_T h[9],
  B_AHRS2_Combined_Experiment_A_T *localB)
{
  int32_T i;
  int32_T h_tmp;
  memset(&h[0], 0, 9U * sizeof(real_T));
  h[3] = v[2];
  h[6] = -v[1];
  h[7] = v[0];
  for (i = 0; i < 3; i++) {
    localB->h[3 * i] = h[3 * i];
    h_tmp = 3 * i + 1;
    localB->h[h_tmp] = h[h_tmp] - h[i + 3];
    h_tmp = 3 * i + 2;
    localB->h[h_tmp] = h[h_tmp] - h[i + 6];
  }

  memcpy(&h[0], &localB->h[0], 9U * sizeof(real_T));
}

static real_T Combined_Experiment_ArmOnl_norm(const real_T x[3],
  B_AHRS2_Combined_Experiment_A_T *localB)
{
  real_T y;
  real_T t;
  localB->scale = 3.3121686421112381E-170;
  localB->absxk = fabs(x[0]);
  if (localB->absxk > 3.3121686421112381E-170) {
    y = 1.0;
    localB->scale = localB->absxk;
  } else {
    t = localB->absxk / 3.3121686421112381E-170;
    y = t * t;
  }

  localB->absxk = fabs(x[1]);
  if (localB->absxk > localB->scale) {
    t = localB->scale / localB->absxk;
    y = y * t * t + 1.0;
    localB->scale = localB->absxk;
  } else {
    t = localB->absxk / localB->scale;
    y += t * t;
  }

  localB->absxk = fabs(x[2]);
  if (localB->absxk > localB->scale) {
    t = localB->scale / localB->absxk;
    y = y * t * t + 1.0;
    localB->scale = localB->absxk;
  } else {
    t = localB->absxk / localB->scale;
    y += t * t;
  }

  return localB->scale * sqrt(y);
}

static void Combined_Ex_quaternionBase_conj(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d)
{
  *b_q_a = q_a;
  *b_q_b = -q_b;
  *b_q_c = -q_c;
  *b_q_d = -q_d;
}

static void Combined__quaternionBase_uminus(real_T obj_a, real_T obj_b, real_T
  obj_c, real_T obj_d, real_T *b_obj_a, real_T *b_obj_b, real_T *b_obj_c, real_T
  *b_obj_d)
{
  *b_obj_a = -obj_a;
  *b_obj_b = -obj_b;
  *b_obj_c = -obj_c;
  *b_obj_d = -obj_d;
}

static void Combin_quaternionBase_normalize(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d)
{
  real_T n;
  n = sqrt(((q_a * q_a + q_b * q_b) + q_c * q_c) + q_d * q_d);
  *b_q_a = q_a / n;
  *b_q_b = q_b / n;
  *b_q_c = q_c / n;
  *b_q_d = q_d / n;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(static_cast<real_T>(u0_0), static_cast<real_T>(u1_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static void Combin_quaternioncg_parenAssign(real_T rhs_a, real_T rhs_b, real_T
  rhs_c, real_T rhs_d, real_T *o_a, real_T *o_b, real_T *o_c, real_T *o_d)
{
  *o_a = rhs_a;
  *o_b = rhs_b;
  *o_c = rhs_c;
  *o_d = rhs_d;
}

static void Combined_Ex_ahrsfilter_stepImpl(fusion_simulink_ahrsfilter_Co_T *obj,
  const real_T accelIn[3], const real_T gyroIn[3], const real_T magIn[3], real_T
  orientOut[4], real_T av[3], B_AHRS2_Combined_Experiment_A_T *localB)
{
  boolean_T isJamming;
  int32_T jp;
  int32_T jj;
  int32_T jp1j;
  int32_T c;
  int32_T ix;
  int32_T iy;
  int32_T jA;
  int32_T jy;
  int32_T c_0;
  int32_T H_tmp;
  int32_T H_tmp_0;
  static const int8_T tmp[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  static const int8_T tmp_0[9] = { -1, 0, 0, 0, -1, 0, 0, 0, -1 };

  IMUFusionCommon_computeAngularV(gyroIn, obj->pGyroOffset, av);
  if (obj->pFirstTime) {
    obj->pFirstTime = false;
    Combined_Experimen_NED_ecompass(accelIn, magIn, localB->Rprior, localB);
    Combi_quaternioncg_quaternioncg(localB->Rprior, &obj->pOrientPost.a,
      &obj->pOrientPost.b, &obj->pOrientPost.c, &obj->pOrientPost.d, localB);
  }

  for (c_0 = 0; c_0 < 3; c_0++) {
    localB->gravityAccelGyroDiff[c_0] = obj->pGyroOffset[c_0];
  }

  IMUFusionCommon_predictOrientat(obj, gyroIn, localB->gravityAccelGyroDiff,
    obj->pOrientPost.a, obj->pOrientPost.b, obj->pOrientPost.c,
    obj->pOrientPost.d, &localB->temp, &localB->smax, &localB->s,
    &localB->gyroOffsetErr_idx_0, localB);
  obj->pOrientPrior.d = localB->gyroOffsetErr_idx_0;
  obj->pOrientPrior.c = localB->s;
  obj->pOrientPrior.b = localB->smax;
  obj->pOrientPrior.a = localB->temp;
  Combined__quaternionBase_rotmat(obj->pOrientPrior.a, obj->pOrientPrior.b,
    obj->pOrientPrior.c, obj->pOrientPrior.d, localB->Rprior, localB);
  obj->pLinAccelPrior[0] = obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[0];
  localB->gravityAccelGyroDiff[0] = (accelIn[0] + obj->pLinAccelPrior[0]) -
    localB->Rprior[6];
  obj->pLinAccelPrior[1] = obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[1];
  localB->gravityAccelGyroDiff[1] = (accelIn[1] + obj->pLinAccelPrior[1]) -
    localB->Rprior[7];
  obj->pLinAccelPrior[2] = obj->LinearAccelerationDecayFactor *
    obj->pLinAccelPost[2];
  localB->gravityAccelGyroDiff[2] = (accelIn[2] + obj->pLinAccelPrior[2]) -
    localB->Rprior[8];
  Comb_IMUFusionCommon_buildHPart(&localB->Rprior[6], localB->h1, localB);
  for (c_0 = 0; c_0 < 3; c_0++) {
    localB->offDiag[c_0] = localB->Rprior[c_0 + 6] * obj->pMagVec[2] +
      (localB->Rprior[c_0 + 3] * obj->pMagVec[1] + localB->Rprior[c_0] *
       obj->pMagVec[0]);
  }

  Comb_IMUFusionCommon_buildHPart(localB->offDiag, localB->Rprior, localB);
  for (c_0 = 0; c_0 < 3; c_0++) {
    localB->temp = localB->Rprior[3 * c_0];
    localB->smax = localB->h1[3 * c_0];
    localB->H[6 * c_0] = localB->smax;
    jp = 6 * (c_0 + 3);
    localB->H[jp] = -localB->smax * obj->pKalmanPeriod;
    H_tmp = 6 * (c_0 + 6);
    localB->H[H_tmp] = tmp[3 * c_0];
    H_tmp_0 = 6 * (c_0 + 9);
    localB->H[H_tmp_0] = 0.0;
    localB->H[6 * c_0 + 3] = localB->temp;
    localB->H[jp + 3] = -localB->temp * obj->pKalmanPeriod;
    localB->H[H_tmp + 3] = 0.0;
    localB->H[H_tmp_0 + 3] = tmp_0[3 * c_0];
    jj = 3 * c_0 + 1;
    localB->H[6 * c_0 + 1] = localB->h1[jj];
    localB->H[jp + 1] = -localB->h1[jj] * obj->pKalmanPeriod;
    localB->H[H_tmp + 1] = tmp[jj];
    localB->H[H_tmp_0 + 1] = 0.0;
    localB->H[6 * c_0 + 4] = localB->Rprior[jj];
    localB->H[jp + 4] = -localB->Rprior[jj] * obj->pKalmanPeriod;
    localB->H[H_tmp + 4] = 0.0;
    localB->H[H_tmp_0 + 4] = tmp_0[jj];
    jj = 3 * c_0 + 2;
    localB->H[6 * c_0 + 2] = localB->h1[jj];
    localB->H[jp + 2] = -localB->h1[jj] * obj->pKalmanPeriod;
    localB->H[H_tmp + 2] = tmp[jj];
    localB->H[H_tmp_0 + 2] = 0.0;
    localB->H[6 * c_0 + 5] = localB->Rprior[jj];
    localB->H[jp + 5] = -localB->Rprior[jj] * obj->pKalmanPeriod;
    localB->H[H_tmp + 5] = 0.0;
    localB->H[H_tmp_0 + 5] = tmp_0[jj];
  }

  memcpy(&localB->Qw[0], &obj->pQw[0], 144U * sizeof(real_T));
  for (c_0 = 0; c_0 < 12; c_0++) {
    for (H_tmp = 0; H_tmp < 6; H_tmp++) {
      jp = H_tmp + 6 * c_0;
      localB->H_m[jp] = 0.0;
      for (jj = 0; jj < 12; jj++) {
        localB->H_m[jp] += localB->H[6 * jj + H_tmp] * obj->pQw[12 * c_0 + jj];
      }
    }
  }

  for (c_0 = 0; c_0 < 6; c_0++) {
    for (H_tmp = 0; H_tmp < 6; H_tmp++) {
      localB->temp = 0.0;
      for (jj = 0; jj < 12; jj++) {
        localB->temp += localB->H_m[6 * jj + H_tmp] * localB->H[6 * jj + c_0];
      }

      localB->c_A[c_0 + 6 * H_tmp] = obj->pQv[6 * c_0 + H_tmp] + localB->temp;
    }

    localB->b_ipiv[c_0] = static_cast<int8_T>(c_0 + 1);
  }

  for (jp = 0; jp < 5; jp++) {
    H_tmp = jp * 7 + 2;
    jj = jp * 7;
    c = 6 - jp;
    iy = 1;
    ix = H_tmp - 2;
    localB->smax = fabs(localB->c_A[jj]);
    for (jA = 2; jA <= c; jA++) {
      ix++;
      localB->s = fabs(localB->c_A[ix]);
      if (localB->s > localB->smax) {
        iy = jA;
        localB->smax = localB->s;
      }
    }

    if (localB->c_A[(H_tmp + iy) - 3] != 0.0) {
      if (iy - 1 != 0) {
        localB->b_ipiv[jp] = static_cast<int8_T>(jp + iy);
        ix = jp;
        iy = (jp + iy) - 1;
        for (jA = 0; jA < 6; jA++) {
          localB->temp = localB->c_A[ix];
          localB->c_A[ix] = localB->c_A[iy];
          localB->c_A[iy] = localB->temp;
          ix += 6;
          iy += 6;
        }
      }

      iy = H_tmp - jp;
      for (ix = H_tmp; ix <= iy + 4; ix++) {
        localB->c_A[ix - 1] /= localB->c_A[jj];
      }
    }

    c = 4 - jp;
    jA = jj;
    jy = jj + 6;
    for (jj = 0; jj <= c; jj++) {
      localB->temp = localB->c_A[jy];
      if (localB->c_A[jy] != 0.0) {
        ix = H_tmp - 1;
        c_0 = jA - jp;
        for (jp1j = jA + 8; jp1j <= c_0 + 12; jp1j++) {
          localB->c_A[jp1j - 1] += localB->c_A[ix] * -localB->temp;
          ix++;
        }
      }

      jy += 6;
      jA += 6;
    }
  }

  for (c_0 = 0; c_0 < 12; c_0++) {
    for (H_tmp = 0; H_tmp < 6; H_tmp++) {
      H_tmp_0 = c_0 + 12 * H_tmp;
      localB->b_X[H_tmp_0] = 0.0;
      for (jj = 0; jj < 12; jj++) {
        localB->b_X[H_tmp_0] += obj->pQw[12 * jj + c_0] * localB->H[6 * jj +
          H_tmp];
      }
    }
  }

  for (jp = 0; jp < 6; jp++) {
    jj = 12 * jp - 1;
    jp1j = 6 * jp - 1;
    iy = jp - 1;
    for (c = 0; c <= iy; c++) {
      jy = 12 * c - 1;
      c_0 = (c + jp1j) + 1;
      if (localB->c_A[c_0] != 0.0) {
        for (H_tmp = 0; H_tmp < 12; H_tmp++) {
          ix = H_tmp + 1;
          H_tmp_0 = ix + jj;
          localB->b_X[H_tmp_0] -= localB->c_A[c_0] * localB->b_X[ix + jy];
        }
      }
    }

    localB->temp = 1.0 / localB->c_A[(jp + jp1j) + 1];
    for (c = 0; c < 12; c++) {
      H_tmp_0 = (c + jj) + 1;
      localB->b_X[H_tmp_0] *= localB->temp;
    }
  }

  for (c = 5; c >= 0; c--) {
    jj = 12 * c - 1;
    jp1j = 6 * c - 1;
    for (jA = c + 2; jA < 7; jA++) {
      jy = (jA - 1) * 12 - 1;
      c_0 = jA + jp1j;
      if (localB->c_A[c_0] != 0.0) {
        for (H_tmp = 0; H_tmp < 12; H_tmp++) {
          ix = H_tmp + 1;
          H_tmp_0 = ix + jj;
          localB->b_X[H_tmp_0] -= localB->c_A[c_0] * localB->b_X[ix + jy];
        }
      }
    }
  }

  for (c = 4; c >= 0; c--) {
    if (c + 1 != localB->b_ipiv[c]) {
      jp = localB->b_ipiv[c] - 1;
      for (H_tmp = 0; H_tmp < 12; H_tmp++) {
        c_0 = 12 * c + H_tmp;
        localB->temp = localB->b_X[c_0];
        H_tmp_0 = 12 * jp + H_tmp;
        localB->b_X[c_0] = localB->b_X[H_tmp_0];
        localB->b_X[H_tmp_0] = localB->temp;
      }
    }
  }

  localB->ze[0] = localB->gravityAccelGyroDiff[0];
  localB->ze[3] = magIn[0] - localB->offDiag[0];
  localB->ze[1] = localB->gravityAccelGyroDiff[1];
  localB->ze[4] = magIn[1] - localB->offDiag[1];
  localB->ze[2] = localB->gravityAccelGyroDiff[2];
  localB->ze[5] = magIn[2] - localB->offDiag[2];
  for (c_0 = 0; c_0 < 3; c_0++) {
    localB->offDiag[c_0] = 0.0;
    for (H_tmp = 0; H_tmp < 6; H_tmp++) {
      localB->offDiag[c_0] += localB->b_X[(12 * H_tmp + c_0) + 9] * localB->
        ze[H_tmp];
    }
  }

  localB->temp = Combined_Experiment_ArmOnl_norm(localB->offDiag, localB);
  isJamming = (localB->temp * localB->temp > obj->ExpectedMagneticFieldStrength *
               obj->ExpectedMagneticFieldStrength * 4.0);
  if (isJamming) {
    for (c_0 = 0; c_0 < 9; c_0++) {
      localB->h1[c_0] = localB->b_X[c_0 + 24] * localB->gravityAccelGyroDiff[2]
        + (localB->b_X[c_0 + 12] * localB->gravityAccelGyroDiff[1] + localB->
           b_X[c_0] * localB->gravityAccelGyroDiff[0]);
    }

    localB->gravityAccelGyroDiff[0] = localB->h1[0];
    localB->gyroOffsetErr_idx_0 = localB->h1[3];
    localB->linAccelErr_idx_0 = localB->h1[6];
    localB->gravityAccelGyroDiff[1] = localB->h1[1];
    localB->gyroOffsetErr_idx_1 = localB->h1[4];
    localB->linAccelErr_idx_1 = localB->h1[7];
    localB->gravityAccelGyroDiff[2] = localB->h1[2];
    localB->gyroOffsetErr_idx_2 = localB->h1[5];
    localB->linAccelErr_idx_2 = localB->h1[8];
  } else {
    for (c_0 = 0; c_0 < 12; c_0++) {
      localB->xe_post[c_0] = 0.0;
      for (H_tmp = 0; H_tmp < 6; H_tmp++) {
        localB->xe_post[c_0] += localB->b_X[12 * H_tmp + c_0] * localB->ze[H_tmp];
      }
    }

    localB->gravityAccelGyroDiff[0] = localB->xe_post[0];
    localB->gyroOffsetErr_idx_0 = localB->xe_post[3];
    localB->linAccelErr_idx_0 = localB->xe_post[6];
    localB->gravityAccelGyroDiff[1] = localB->xe_post[1];
    localB->gyroOffsetErr_idx_1 = localB->xe_post[4];
    localB->linAccelErr_idx_1 = localB->xe_post[7];
    localB->gravityAccelGyroDiff[2] = localB->xe_post[2];
    localB->gyroOffsetErr_idx_2 = localB->xe_post[5];
    localB->linAccelErr_idx_2 = localB->xe_post[8];
  }

  Com_quaternioncg_quaternioncg_f(localB->gravityAccelGyroDiff, &localB->temp,
    &localB->smax, &localB->s, &localB->assign_temp_d_c, localB);
  Combined_Ex_quaternionBase_conj(localB->temp, localB->smax, localB->s,
    localB->assign_temp_d_c, &localB->assign_temp_a, &localB->assign_temp_b,
    &localB->assign_temp_c, &localB->assign_temp_d);
  Combined__quaternionBase_mtimes(obj->pOrientPrior.a, obj->pOrientPrior.b,
    obj->pOrientPrior.c, obj->pOrientPrior.d, localB->assign_temp_a,
    localB->assign_temp_b, localB->assign_temp_c, localB->assign_temp_d,
    &obj->pOrientPost.a, &obj->pOrientPost.b, &obj->pOrientPost.c,
    &obj->pOrientPost.d);
  if (obj->pOrientPost.a < 0.0) {
    Combined__quaternionBase_uminus(obj->pOrientPost.a, obj->pOrientPost.b,
      obj->pOrientPost.c, obj->pOrientPost.d, &obj->pOrientPost.a,
      &obj->pOrientPost.b, &obj->pOrientPost.c, &obj->pOrientPost.d);
  }

  Combin_quaternionBase_normalize(obj->pOrientPost.a, obj->pOrientPost.b,
    obj->pOrientPost.c, obj->pOrientPost.d, &obj->pOrientPost.a,
    &obj->pOrientPost.b, &obj->pOrientPost.c, &obj->pOrientPost.d);
  Combined__quaternionBase_rotmat(obj->pOrientPost.a, obj->pOrientPost.b,
    obj->pOrientPost.c, obj->pOrientPost.d, localB->h1, localB);
  obj->pGyroOffset[0] -= localB->gyroOffsetErr_idx_0;
  obj->pLinAccelPost[0] = obj->pLinAccelPrior[0] - localB->linAccelErr_idx_0;
  obj->pGyroOffset[1] -= localB->gyroOffsetErr_idx_1;
  obj->pLinAccelPost[1] = obj->pLinAccelPrior[1] - localB->linAccelErr_idx_1;
  obj->pGyroOffset[2] -= localB->gyroOffsetErr_idx_2;
  obj->pLinAccelPost[2] = obj->pLinAccelPrior[2] - localB->linAccelErr_idx_2;
  if (!isJamming) {
    for (H_tmp = 0; H_tmp < 3; H_tmp++) {
      jp = H_tmp * 3 - 1;
      localB->gravityAccelGyroDiff[H_tmp] = (localB->h1[jp + 1] *
        localB->offDiag[0] + localB->h1[jp + 2] * localB->offDiag[1]) +
        localB->h1[jp + 3] * localB->offDiag[2];
    }

    localB->gravityAccelGyroDiff[0] = obj->pMagVec[0] -
      localB->gravityAccelGyroDiff[0];
    localB->temp = rt_atan2d_snf(obj->pMagVec[2] - localB->gravityAccelGyroDiff
      [2], localB->gravityAccelGyroDiff[0]);
    if (localB->temp < -1.5707963267948966) {
      localB->temp = -1.5707963267948966;
    }

    if (localB->temp > 1.5707963267948966) {
      localB->temp = 1.5707963267948966;
    }

    obj->pMagVec[0] = 0.0;
    obj->pMagVec[1] = 0.0;
    obj->pMagVec[2] = 0.0;
    obj->pMagVec[0] = cos(localB->temp);
    obj->pMagVec[2] = sin(localB->temp);
    obj->pMagVec[0] *= obj->ExpectedMagneticFieldStrength;
    obj->pMagVec[1] *= obj->ExpectedMagneticFieldStrength;
    obj->pMagVec[2] *= obj->ExpectedMagneticFieldStrength;
  }

  for (c_0 = 0; c_0 < 12; c_0++) {
    for (H_tmp = 0; H_tmp < 6; H_tmp++) {
      jp = H_tmp + 6 * c_0;
      localB->H_m[jp] = 0.0;
      for (jj = 0; jj < 12; jj++) {
        localB->H_m[jp] += localB->H[6 * jj + H_tmp] * localB->Qw[12 * c_0 + jj];
      }
    }
  }

  for (c_0 = 0; c_0 < 12; c_0++) {
    for (H_tmp = 0; H_tmp < 12; H_tmp++) {
      localB->temp = 0.0;
      for (jj = 0; jj < 6; jj++) {
        localB->temp += localB->b_X[12 * jj + c_0] * localB->H_m[6 * H_tmp + jj];
      }

      jp = 12 * H_tmp + c_0;
      localB->Ppost[jp] = localB->Qw[jp] - localB->temp;
    }
  }

  memset(&localB->Qw[0], 0, 144U * sizeof(real_T));
  localB->temp = obj->pKalmanPeriod * obj->pKalmanPeriod;
  localB->smax = obj->GyroscopeDriftNoise + obj->GyroscopeNoise;
  localB->Qw[0] = (localB->Ppost[39] + localB->smax) * localB->temp +
    localB->Ppost[0];
  localB->Qw[39] = localB->Ppost[39] + obj->GyroscopeDriftNoise;
  localB->offDiag[0] = -obj->pKalmanPeriod * localB->Qw[39];
  localB->Qw[13] = (localB->Ppost[52] + localB->smax) * localB->temp +
    localB->Ppost[13];
  localB->Qw[52] = localB->Ppost[52] + obj->GyroscopeDriftNoise;
  localB->offDiag[1] = -obj->pKalmanPeriod * localB->Qw[52];
  localB->Qw[26] = (localB->Ppost[65] + localB->smax) * localB->temp +
    localB->Ppost[26];
  localB->Qw[65] = localB->Ppost[65] + obj->GyroscopeDriftNoise;
  localB->offDiag[2] = -obj->pKalmanPeriod * localB->Qw[65];
  localB->Qw[3] = localB->offDiag[0];
  localB->Qw[16] = localB->offDiag[1];
  localB->Qw[29] = localB->offDiag[2];
  localB->Qw[36] = localB->offDiag[0];
  localB->Qw[49] = localB->offDiag[1];
  localB->Qw[62] = localB->offDiag[2];
  localB->temp = obj->LinearAccelerationDecayFactor *
    obj->LinearAccelerationDecayFactor;
  localB->Qw[78] = localB->temp * localB->Ppost[78] +
    obj->LinearAccelerationNoise;
  localB->Qw[91] = localB->temp * localB->Ppost[91] +
    obj->LinearAccelerationNoise;
  localB->Qw[104] = localB->temp * localB->Ppost[104] +
    obj->LinearAccelerationNoise;
  localB->temp = obj->MagneticDisturbanceDecayFactor *
    obj->MagneticDisturbanceDecayFactor;
  localB->Qw[117] = localB->temp * localB->Ppost[117] +
    obj->MagneticDisturbanceNoise;
  localB->Qw[130] = localB->temp * localB->Ppost[130] +
    obj->MagneticDisturbanceNoise;
  localB->Qw[143] = localB->temp * localB->Ppost[143] +
    obj->MagneticDisturbanceNoise;
  memcpy(&obj->pQw[0], &localB->Qw[0], 144U * sizeof(real_T));
  Combin_quaternioncg_parenAssign(obj->pOrientPost.a, obj->pOrientPost.b,
    obj->pOrientPost.c, obj->pOrientPost.d, &orientOut[0], &orientOut[1],
    &orientOut[2], &orientOut[3]);
}

//
// System initialize for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void Combined_Experim_AHRS2_Init(const real_T rtu_0[3],
  DW_AHRS2_Combined_Experiment__T *localDW, P_AHRS2_Combined_Experiment_A_T
  *localP)
{
  // Start for MATLABSystem: '<S169>/AHRS2'
  localDW->obj.isInitialized = 0;
  localDW->objisempty = true;
  localDW->obj.AccelerometerNoise = localP->AHRS2_AccelerometerNoise;
  localDW->obj.GyroscopeNoise = localP->AHRS2_GyroscopeNoise;
  localDW->obj.MagnetometerNoise = localP->AHRS2_MagnetometerNoise;
  localDW->obj.GyroscopeDriftNoise = localP->AHRS2_GyroscopeDriftNoise;
  localDW->obj.LinearAccelerationNoise = localP->AHRS2_LinearAccelerationNoise;
  localDW->obj.MagneticDisturbanceNoise = localP->AHRS2_MagneticDisturbanceNoise;
  localDW->obj.LinearAccelerationDecayFactor =
    localP->AHRS2_LinearAccelerationDecayFa;
  localDW->obj.MagneticDisturbanceDecayFactor =
    localP->AHRS2_MagneticDisturbanceDecayF;
  localDW->obj.ExpectedMagneticFieldStrength =
    localP->AHRS2_ExpectedMagneticFieldStre;
  localDW->obj.isInitialized = 1;
  localDW->obj.pInputPrototype[0] = rtu_0[0];
  localDW->obj.pInputPrototype[1] = rtu_0[1];
  localDW->obj.pInputPrototype[2] = rtu_0[2];
  localDW->obj.pSensorPeriod = 0.05;
  localDW->obj.pKalmanPeriod = localDW->obj.pSensorPeriod;
  localDW->obj.pRefSys = &localDW->gobj_2;
  localDW->obj.TunablePropsChanged = false;

  // InitializeConditions for MATLABSystem: '<S169>/AHRS2'
  Combin_AHRSFilterBase_resetImpl(&localDW->obj);
}

//
// Output and update for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void Combined_Experiment_A_AHRS2(const real_T rtu_0[3], const real_T rtu_1[3],
  const real_T rtu_2[3], B_AHRS2_Combined_Experiment_A_T *localB,
  DW_AHRS2_Combined_Experiment__T *localDW, P_AHRS2_Combined_Experiment_A_T
  *localP)
{
  int32_T tmp;
  static const int8_T tmp_0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  // MATLABSystem: '<S169>/AHRS2'
  if (localDW->obj.AccelerometerNoise != localP->AHRS2_AccelerometerNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.AccelerometerNoise = localP->AHRS2_AccelerometerNoise;
  }

  if (localDW->obj.GyroscopeNoise != localP->AHRS2_GyroscopeNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.GyroscopeNoise = localP->AHRS2_GyroscopeNoise;
  }

  if (localDW->obj.MagnetometerNoise != localP->AHRS2_MagnetometerNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.MagnetometerNoise = localP->AHRS2_MagnetometerNoise;
  }

  if (localDW->obj.GyroscopeDriftNoise != localP->AHRS2_GyroscopeDriftNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.GyroscopeDriftNoise = localP->AHRS2_GyroscopeDriftNoise;
  }

  if (localDW->obj.LinearAccelerationNoise !=
      localP->AHRS2_LinearAccelerationNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.LinearAccelerationNoise = localP->AHRS2_LinearAccelerationNoise;
  }

  if (localDW->obj.MagneticDisturbanceNoise !=
      localP->AHRS2_MagneticDisturbanceNoise) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.MagneticDisturbanceNoise =
      localP->AHRS2_MagneticDisturbanceNoise;
  }

  if (localDW->obj.LinearAccelerationDecayFactor !=
      localP->AHRS2_LinearAccelerationDecayFa) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.LinearAccelerationDecayFactor =
      localP->AHRS2_LinearAccelerationDecayFa;
  }

  if (localDW->obj.MagneticDisturbanceDecayFactor !=
      localP->AHRS2_MagneticDisturbanceDecayF) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.MagneticDisturbanceDecayFactor =
      localP->AHRS2_MagneticDisturbanceDecayF;
  }

  if (localDW->obj.ExpectedMagneticFieldStrength !=
      localP->AHRS2_ExpectedMagneticFieldStre) {
    if (localDW->obj.isInitialized == 1) {
      localDW->obj.TunablePropsChanged = true;
    }

    localDW->obj.ExpectedMagneticFieldStrength =
      localP->AHRS2_ExpectedMagneticFieldStre;
  }

  if (localDW->obj.TunablePropsChanged) {
    localDW->obj.TunablePropsChanged = false;
    localB->magMeasNoiseVar = localDW->obj.pKalmanPeriod *
      localDW->obj.pKalmanPeriod * (localDW->obj.GyroscopeDriftNoise +
      localDW->obj.GyroscopeNoise);
    localB->accelMeasNoiseVar = localB->magMeasNoiseVar +
      (localDW->obj.AccelerometerNoise + localDW->obj.LinearAccelerationNoise);
    localB->magMeasNoiseVar += localDW->obj.MagnetometerNoise +
      localDW->obj.MagneticDisturbanceNoise;
    memset(&localDW->obj.pQv[0], 0, 36U * sizeof(real_T));
    for (localB->i = 0; localB->i < 3; localB->i++) {
      localB->i1 = tmp_0[3 * localB->i];
      localDW->obj.pQv[6 * localB->i] = localB->accelMeasNoiseVar * static_cast<
        real_T>(localB->i1);
      tmp = 6 * (localB->i + 3);
      localDW->obj.pQv[tmp + 3] = localB->magMeasNoiseVar * static_cast<real_T>
        (localB->i1);
      localB->i1 = tmp_0[3 * localB->i + 1];
      localDW->obj.pQv[6 * localB->i + 1] = localB->accelMeasNoiseVar *
        static_cast<real_T>(localB->i1);
      localDW->obj.pQv[tmp + 4] = localB->magMeasNoiseVar * static_cast<real_T>
        (localB->i1);
      localB->i1 = tmp_0[3 * localB->i + 2];
      localDW->obj.pQv[6 * localB->i + 2] = localB->accelMeasNoiseVar *
        static_cast<real_T>(localB->i1);
      localDW->obj.pQv[tmp + 5] = localB->magMeasNoiseVar * static_cast<real_T>
        (localB->i1);
    }
  }

  Combined_Ex_ahrsfilter_stepImpl(&localDW->obj, rtu_0, rtu_1, rtu_2,
    localB->b_varargout_1, localB->b_varargout_2, localB);
  localB->AHRS2_o2[0] = localB->b_varargout_2[0];
  localB->AHRS2_o2[1] = localB->b_varargout_2[1];
  localB->AHRS2_o2[2] = localB->b_varargout_2[2];

  // End of MATLABSystem: '<S169>/AHRS2'
}

//
// Output and update for atomic system:
//    '<S169>/ChangeOrientation'
//    '<S170>/ChangeOrientation'
//    '<S171>/ChangeOrientation'
//
void Combined__ChangeOrientation(real_T rtu_u, real_T rtu_u_g, real_T rtu_u_d,
  B_ChangeOrientation_Combined__T *localB)
{
  int32_T i;
  static const int8_T a[9] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };

  // MATLAB Function 'Gyroscope & Acceleration Algorithms/Change BLACK Behavior/ChangeOrientation': '<S173>:1' 
  // '<S173>:1:3' y = rotz(-90)*u';
  for (i = 0; i < 3; i++) {
    localB->y[i] = 0.0;

    // SignalConversion generated from: '<S173>/ SFunction '
    localB->y[i] += static_cast<real_T>(a[i]) * rtu_u;
    localB->y[i] += static_cast<real_T>(a[i + 3]) * rtu_u_g;
    localB->y[i] += static_cast<real_T>(a[i + 6]) * rtu_u_d;
  }
}

//
// System initialize for action system:
//    '<S179>/Calculate Running Mean'
//    '<S180>/Calculate Running Mean'
//    '<S181>/Calculate Running Mean'
//    '<S192>/Calculate Running Mean'
//    '<S193>/Calculate Running Mean'
//    '<S210>/Calculate Running Mean'
//    '<S211>/Calculate Running Mean'
//    '<S212>/Calculate Running Mean'
//    '<S223>/Calculate Running Mean'
//    '<S224>/Calculate Running Mean'
//    ...
//
void C_CalculateRunningMean_Init(B_CalculateRunningMean_Combin_T *localB,
  DW_CalculateRunningMean_Combi_T *localDW, P_CalculateRunningMean_Combin_T
  *localP)
{
  int32_T i;

  // InitializeConditions for Delay: '<S182>/Delay'
  localDW->Delay_DSTATE = localP->Delay_InitialCondition;

  // InitializeConditions for Delay: '<S182>/Delay1'
  localDW->Delay1_DSTATE[0] = localP->Delay1_InitialCondition;
  localDW->Delay1_DSTATE[1] = localP->Delay1_InitialCondition;

  // InitializeConditions for Delay: '<S182>/Delay2'
  localDW->Delay2_DSTATE[0] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[1] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[2] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[3] = localP->Delay2_InitialCondition;

  // InitializeConditions for Delay: '<S182>/Delay3'
  for (i = 0; i < 5; i++) {
    localDW->Delay3_DSTATE[i] = localP->Delay3_InitialCondition;
  }

  // End of InitializeConditions for Delay: '<S182>/Delay3'

  // InitializeConditions for Delay: '<S182>/Delay4'
  localDW->Delay4_DSTATE[0] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[1] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[2] = localP->Delay4_InitialCondition;

  // SystemInitialize for Outport: '<S182>/Out1'
  localB->Mean = localP->Out1_Y0;
}

//
// Output and update for action system:
//    '<S179>/Calculate Running Mean'
//    '<S180>/Calculate Running Mean'
//    '<S181>/Calculate Running Mean'
//    '<S192>/Calculate Running Mean'
//    '<S193>/Calculate Running Mean'
//    '<S210>/Calculate Running Mean'
//    '<S211>/Calculate Running Mean'
//    '<S212>/Calculate Running Mean'
//    '<S223>/Calculate Running Mean'
//    '<S224>/Calculate Running Mean'
//    ...
//
void Combin_CalculateRunningMean(real_T rtu_In1, B_CalculateRunningMean_Combin_T
  *localB, DW_CalculateRunningMean_Combi_T *localDW)
{
  int32_T n;
  int32_T str;
  int_T idxWidth;
  real_T rtb_TmpSignalConversionAtMeanIn[6];

  // SignalConversion generated from: '<S182>/Mean' incorporates:
  //   Delay: '<S182>/Delay'
  //   Delay: '<S182>/Delay1'
  //   Delay: '<S182>/Delay2'
  //   Delay: '<S182>/Delay3'
  //   Delay: '<S182>/Delay4'

  rtb_TmpSignalConversionAtMeanIn[0] = rtu_In1;
  rtb_TmpSignalConversionAtMeanIn[1] = localDW->Delay_DSTATE;
  rtb_TmpSignalConversionAtMeanIn[2] = localDW->Delay1_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[3] = localDW->Delay4_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[4] = localDW->Delay2_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[5] = localDW->Delay3_DSTATE[0U];

  // S-Function (sdspstatfcns): '<S182>/Mean'
  for (idxWidth = 0; idxWidth < 1; idxWidth++) {
    localDW->Mean_AccVal = rtb_TmpSignalConversionAtMeanIn[idxWidth];
    str = 1;
    for (n = 4; n >= 0; n--) {
      localDW->Mean_AccVal += rtb_TmpSignalConversionAtMeanIn[idxWidth + str];
      str++;
    }

    localB->Mean = localDW->Mean_AccVal / 6.0;
  }

  // End of S-Function (sdspstatfcns): '<S182>/Mean'

  // Update for Delay: '<S182>/Delay'
  localDW->Delay_DSTATE = rtu_In1;

  // Update for Delay: '<S182>/Delay1'
  localDW->Delay1_DSTATE[0] = localDW->Delay1_DSTATE[1];
  localDW->Delay1_DSTATE[1] = rtu_In1;

  // Update for Delay: '<S182>/Delay2'
  localDW->Delay2_DSTATE[0] = localDW->Delay2_DSTATE[1];
  localDW->Delay2_DSTATE[1] = localDW->Delay2_DSTATE[2];
  localDW->Delay2_DSTATE[2] = localDW->Delay2_DSTATE[3];
  localDW->Delay2_DSTATE[3] = rtu_In1;

  // Update for Delay: '<S182>/Delay3'
  localDW->Delay3_DSTATE[0] = localDW->Delay3_DSTATE[1];
  localDW->Delay3_DSTATE[1] = localDW->Delay3_DSTATE[2];
  localDW->Delay3_DSTATE[2] = localDW->Delay3_DSTATE[3];
  localDW->Delay3_DSTATE[3] = localDW->Delay3_DSTATE[4];
  localDW->Delay3_DSTATE[4] = rtu_In1;

  // Update for Delay: '<S182>/Delay4'
  localDW->Delay4_DSTATE[0] = localDW->Delay4_DSTATE[1];
  localDW->Delay4_DSTATE[1] = localDW->Delay4_DSTATE[2];
  localDW->Delay4_DSTATE[2] = rtu_In1;
}

//
// System initialize for action system:
//    '<S194>/Calculate Running Mean'
//    '<S225>/Calculate Running Mean'
//    '<S256>/Calculate Running Mean'
//
void CalculateRunningMean_b_Init(B_CalculateRunningMean_Comb_e_T *localB,
  DW_CalculateRunningMean_Com_e_T *localDW, P_CalculateRunningMean_Comb_p_T
  *localP)
{
  int32_T i;

  // InitializeConditions for Delay: '<S201>/Delay'
  localDW->Delay_DSTATE = localP->Delay_InitialCondition;

  // InitializeConditions for Delay: '<S201>/Delay1'
  localDW->Delay1_DSTATE[0] = localP->Delay1_InitialCondition;
  localDW->Delay1_DSTATE[1] = localP->Delay1_InitialCondition;

  // InitializeConditions for Delay: '<S201>/Delay2'
  localDW->Delay2_DSTATE[0] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[1] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[2] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[3] = localP->Delay2_InitialCondition;

  // InitializeConditions for Delay: '<S201>/Delay3'
  for (i = 0; i < 5; i++) {
    localDW->Delay3_DSTATE[i] = localP->Delay3_InitialCondition;
  }

  // End of InitializeConditions for Delay: '<S201>/Delay3'

  // InitializeConditions for Delay: '<S201>/Delay4'
  localDW->Delay4_DSTATE[0] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[1] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[2] = localP->Delay4_InitialCondition;

  // SystemInitialize for Outport: '<S201>/Out1'
  localB->Subtract = localP->Out1_Y0;
}

//
// Output and update for action system:
//    '<S194>/Calculate Running Mean'
//    '<S225>/Calculate Running Mean'
//    '<S256>/Calculate Running Mean'
//
void Comb_CalculateRunningMean_a(real_T rtu_In1, B_CalculateRunningMean_Comb_e_T
  *localB, DW_CalculateRunningMean_Com_e_T *localDW,
  P_CalculateRunningMean_Comb_p_T *localP)
{
  int32_T n;
  int32_T str;
  int_T idxWidth;
  real_T rtb_Mean;

  // Delay: '<S201>/Delay4'
  rtb_Mean = localDW->Delay4_DSTATE[0];

  // SignalConversion generated from: '<S201>/Mean' incorporates:
  //   Delay: '<S201>/Delay'
  //   Delay: '<S201>/Delay1'
  //   Delay: '<S201>/Delay2'
  //   Delay: '<S201>/Delay3'
  //   Delay: '<S201>/Delay4'

  localB->TmpSignalConversionAtMean_b[0] = rtu_In1;
  localB->TmpSignalConversionAtMean_b[1] = localDW->Delay_DSTATE;
  localB->TmpSignalConversionAtMean_b[2] = localDW->Delay1_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[3] = localDW->Delay4_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[4] = localDW->Delay2_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[5] = localDW->Delay3_DSTATE[0U];

  // S-Function (sdspstatfcns): '<S201>/Mean'
  for (idxWidth = 0; idxWidth < 1; idxWidth++) {
    localDW->Mean_AccVal = localB->TmpSignalConversionAtMean_b[idxWidth];
    str = 1;
    for (n = 4; n >= 0; n--) {
      localDW->Mean_AccVal += localB->TmpSignalConversionAtMean_b[idxWidth + str];
      str++;
    }

    rtb_Mean = localDW->Mean_AccVal / 6.0;
  }

  // End of S-Function (sdspstatfcns): '<S201>/Mean'

  // Sum: '<S201>/Subtract' incorporates:
  //   Constant: '<S201>/Constant'

  localB->Subtract = rtb_Mean - localP->Constant_Value;

  // Update for Delay: '<S201>/Delay'
  localDW->Delay_DSTATE = rtu_In1;

  // Update for Delay: '<S201>/Delay1'
  localDW->Delay1_DSTATE[0] = localDW->Delay1_DSTATE[1];
  localDW->Delay1_DSTATE[1] = rtu_In1;

  // Update for Delay: '<S201>/Delay2'
  localDW->Delay2_DSTATE[0] = localDW->Delay2_DSTATE[1];
  localDW->Delay2_DSTATE[1] = localDW->Delay2_DSTATE[2];
  localDW->Delay2_DSTATE[2] = localDW->Delay2_DSTATE[3];
  localDW->Delay2_DSTATE[3] = rtu_In1;

  // Update for Delay: '<S201>/Delay3'
  localDW->Delay3_DSTATE[0] = localDW->Delay3_DSTATE[1];
  localDW->Delay3_DSTATE[1] = localDW->Delay3_DSTATE[2];
  localDW->Delay3_DSTATE[2] = localDW->Delay3_DSTATE[3];
  localDW->Delay3_DSTATE[3] = localDW->Delay3_DSTATE[4];
  localDW->Delay3_DSTATE[4] = rtu_In1;

  // Update for Delay: '<S201>/Delay4'
  localDW->Delay4_DSTATE[0] = localDW->Delay4_DSTATE[1];
  localDW->Delay4_DSTATE[1] = localDW->Delay4_DSTATE[2];
  localDW->Delay4_DSTATE[2] = rtu_In1;
}

//
// Output and update for atomic system:
//    '<S506>/MATLAB Function'
//    '<S507>/MATLAB Function'
//    '<S508>/MATLAB Function'
//
void Combined_E_MATLABFunction_f(real_T rtu_TIME, real_T rtu_SERVER,
  B_MATLABFunction_Combined_E_l_T *localB)
{
  real_T r;
  boolean_T rEQ0;
  real_T q;

  // MATLAB Function 'Simulate Plant Dynamics/Sample and Hold  Data to Match  PhaseSpace (BLACK)/MATLAB Function': '<S527>:1' 
  // '<S527>:1:3' if mod(TIME,SERVER) == 0
  r = rtu_TIME;
  if (rtu_SERVER == 0.0) {
    if (rtu_TIME == 0.0) {
      r = rtu_SERVER;
    }
  } else if (rtIsNaN(rtu_TIME)) {
    r = (rtNaN);
  } else if (rtIsNaN(rtu_SERVER)) {
    r = (rtNaN);
  } else if (rtIsInf(rtu_TIME)) {
    r = (rtNaN);
  } else if (rtu_TIME == 0.0) {
    r = 0.0 / rtu_SERVER;
  } else if (rtIsInf(rtu_SERVER)) {
    if ((rtu_SERVER < 0.0) != (rtu_TIME < 0.0)) {
      r = rtu_SERVER;
    }
  } else {
    r = fmod(rtu_TIME, rtu_SERVER);
    rEQ0 = (r == 0.0);
    if ((!rEQ0) && (rtu_SERVER > floor(rtu_SERVER))) {
      q = fabs(rtu_TIME / rtu_SERVER);
      rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }

    if (rEQ0) {
      r = rtu_SERVER * 0.0;
    } else {
      if ((rtu_TIME < 0.0) != (rtu_SERVER < 0.0)) {
        r += rtu_SERVER;
      }
    }
  }

  if (r == 0.0) {
    // '<S527>:1:4' y = 1;
    localB->y = 1.0;
  } else {
    // '<S527>:1:5' else
    // '<S527>:1:6' y = 0;
    localB->y = 0.0;
  }
}

//
// Output and update for action system:
//    '<S8>/Change BLUE Behavior'
//    '<S9>/Change BLUE Behavior'
//    '<S14>/Change BLUE Behavior'
//
void Combined_ChangeBLUEBehavior(P_ChangeBLUEBehavior_Combined_T *localP, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  // DataStoreWrite: '<S267>/BLUE_Fx' incorporates:
  //   Constant: '<S267>/Constant3'

  *rtd_BLUE_Fx = localP->Constant3_Value;

  // DataStoreWrite: '<S267>/BLUE_Fy' incorporates:
  //   Constant: '<S267>/Constant4'

  *rtd_BLUE_Fy = localP->Constant4_Value;

  // DataStoreWrite: '<S267>/BLUE_Tz' incorporates:
  //   Constant: '<S267>/Constant5'

  *rtd_BLUE_Tz = localP->Constant5_Value;

  // DataStoreWrite: '<S267>/Data Store Write4' incorporates:
  //   Constant: '<S267>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// System initialize for action system:
//    '<Root>/Phase #0:  Wait for Synchronization'
//    '<Root>/Phase #1:  Start Floating '
//
void Phase0WaitforSynchroni_Init(DW_Phase0WaitforSynchronizati_T *localDW)
{
  // Start for If: '<S8>/This IF block determines whether or not to run the BLACK sim//exp' 
  localDW->ThisIFblockdetermineswhetherorn = -1;

  // Start for If: '<S8>/This IF block determines whether or not to run the BLUE sim//exp' 
  localDW->ThisIFblockdetermineswhethero_m = -1;

  // Start for If: '<S8>/This IF block determines whether or not to run the RED sim//exp ' 
  localDW->ThisIFblockdetermineswhethero_n = -1;
}

//
// Output and update for action system:
//    '<Root>/Phase #0:  Wait for Synchronization'
//    '<Root>/Phase #1:  Start Floating '
//
void Phase0WaitforSynchronizatio(DW_Phase0WaitforSynchronizati_T *localDW,
  P_Phase0WaitforSynchronizatio_T *localP, real_T *rtd_BLACK_Fx, real_T
  *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy,
  real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T *rtd_RED_Fx, real_T
  *rtd_RED_Fy, real_T *rtd_RED_Tz, real_T *rtd_RED_Tz_Elbow, real_T
  *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist)
{
  // If: '<S8>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
  //   Constant: '<S269>/Constant'
  //   Constant: '<S8>/Constant'

  localDW->ThisIFblockdetermineswhetherorn = -1;
  if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
      (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
    localDW->ThisIFblockdetermineswhetherorn = 0;

    // Outputs for IfAction SubSystem: '<S8>/Change BLACK Behavior' incorporates:
    //   ActionPort: '<S266>/Action Port'

    // DataStoreWrite: '<S266>/BLACK_Fx' incorporates:
    //   Constant: '<S266>/Constant3'

    *rtd_BLACK_Fx = localP->Constant3_Value;

    // DataStoreWrite: '<S266>/BLACK_Fy' incorporates:
    //   Constant: '<S266>/Constant4'

    *rtd_BLACK_Fy = localP->Constant4_Value;

    // DataStoreWrite: '<S266>/BLACK_Tz' incorporates:
    //   Constant: '<S266>/Constant5'

    *rtd_BLACK_Tz = localP->Constant5_Value;

    // DataStoreWrite: '<S266>/Data Store Write4' incorporates:
    //   Constant: '<S266>/Puck State'

    *rtd_Float_State = localP->PuckState_Value;

    // End of Outputs for SubSystem: '<S8>/Change BLACK Behavior'
  }

  // End of If: '<S8>/This IF block determines whether or not to run the BLACK sim//exp' 

  // If: '<S8>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
  //   Constant: '<S269>/Constant'
  //   Constant: '<S8>/Constant'

  localDW->ThisIFblockdetermineswhethero_m = -1;
  if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
      (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
    localDW->ThisIFblockdetermineswhethero_m = 0;

    // Outputs for IfAction SubSystem: '<S8>/Change BLUE Behavior' incorporates:
    //   ActionPort: '<S267>/Action Port'

    Combined_ChangeBLUEBehavior(&localP->ChangeBLUEBehavior, rtd_BLUE_Fx,
      rtd_BLUE_Fy, rtd_BLUE_Tz, rtd_Float_State);

    // End of Outputs for SubSystem: '<S8>/Change BLUE Behavior'
  }

  // End of If: '<S8>/This IF block determines whether or not to run the BLUE sim//exp' 

  // If: '<S8>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
  //   Constant: '<S269>/Constant'
  //   Constant: '<S8>/Constant'

  localDW->ThisIFblockdetermineswhethero_n = -1;
  if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
      (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
    localDW->ThisIFblockdetermineswhethero_n = 0;

    // Outputs for IfAction SubSystem: '<S8>/Change RED Behavior' incorporates:
    //   ActionPort: '<S268>/Action Port'

    // DataStoreWrite: '<S268>/RED_Fx' incorporates:
    //   Constant: '<S268>/Constant'

    *rtd_RED_Fx = localP->Constant_Value;

    // DataStoreWrite: '<S268>/RED_Fy' incorporates:
    //   Constant: '<S268>/Constant1'

    *rtd_RED_Fy = localP->Constant1_Value;

    // DataStoreWrite: '<S268>/RED_Tz' incorporates:
    //   Constant: '<S268>/Constant2'

    *rtd_RED_Tz = localP->Constant2_Value;

    // DataStoreWrite: '<S268>/RED_Tz_RW' incorporates:
    //   Constant: '<S268>/Constant3'

    *rtd_RED_Tz_RW = localP->Constant3_Value_h;

    // DataStoreWrite: '<S268>/Data Store Write' incorporates:
    //   Constant: '<S268>/Constant4'

    *rtd_RED_Tz_Shoulder = localP->Constant4_Value_g;

    // DataStoreWrite: '<S268>/Data Store Write1' incorporates:
    //   Constant: '<S268>/Constant5'

    *rtd_RED_Tz_Elbow = localP->Constant5_Value_e;

    // DataStoreWrite: '<S268>/Data Store Write2' incorporates:
    //   Constant: '<S268>/Constant6'

    *rtd_RED_Tz_Wrist = localP->Constant6_Value;

    // DataStoreWrite: '<S268>/Data Store Write4' incorporates:
    //   Constant: '<S268>/Puck State'

    *rtd_Float_State = localP->PuckState_Value_b;

    // End of Outputs for SubSystem: '<S8>/Change RED Behavior'
  }

  // End of If: '<S8>/This IF block determines whether or not to run the RED sim//exp ' 
}

//
// Output and update for atomic system:
//    '<S281>/MATLAB Function2'
//    '<S294>/MATLAB Function2'
//    '<S307>/MATLAB Function2'
//    '<S329>/MATLAB Function2'
//    '<S364>/MATLAB Function2'
//    '<S377>/MATLAB Function2'
//    '<S417>/MATLAB Function2'
//    '<S430>/MATLAB Function2'
//    '<S443>/MATLAB Function2'
//    '<S460>/MATLAB Function2'
//    ...
//
void Combined__MATLABFunction2_o(real_T rtu_z, B_MATLABFunction2_Combined__n_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2': '<S288>:1' 
  // '<S288>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S288>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
}

//
// Output and update for atomic system:
//    '<S281>/MATLAB Function3'
//    '<S294>/MATLAB Function3'
//    '<S307>/MATLAB Function3'
//    '<S329>/MATLAB Function3'
//    '<S343>/MATLAB Function3'
//    '<S364>/MATLAB Function3'
//    '<S377>/MATLAB Function3'
//    '<S395>/MATLAB Function3'
//    '<S417>/MATLAB Function3'
//    '<S430>/MATLAB Function3'
//    ...
//
void Combined_Ex_MATLABFunction3(real_T rtu_z, B_MATLABFunction3_Combined_Ex_T
  *localB)
{
  real_T Ox_tmp;
  real_T Ox_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3': '<S289>:1' 
  // '<S289>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp = sin(rtu_z);
  Ox_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_0;
  localB->Ox[0] += Ox_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp;
  localB->Ox[1] += Ox_tmp_0 * 0.0;
}

//
// Output and update for atomic system:
//    '<S281>/MATLAB Function4'
//    '<S294>/MATLAB Function4'
//    '<S307>/MATLAB Function4'
//    '<S329>/MATLAB Function4'
//    '<S343>/MATLAB Function4'
//    '<S364>/MATLAB Function4'
//    '<S377>/MATLAB Function4'
//    '<S395>/MATLAB Function4'
//    '<S417>/MATLAB Function4'
//    '<S430>/MATLAB Function4'
//    ...
//
void Combined_Ex_MATLABFunction4(const real_T rtu_Oy[2], const real_T rtu_e_in[2],
  B_MATLABFunction4_Combined_Ex_T *localB)
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4': '<S290>:1' 
  // '<S290>:1:3' a = sign(Oy'*e_in);
  // '<S290>:1:4' e_out = -a*norm(e_in);
  scale = 3.3121686421112381E-170;
  absxk = fabs(rtu_e_in[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }

  absxk = fabs(rtu_e_in[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = rtu_Oy[0] * rtu_e_in[0] + rtu_Oy[1] * rtu_e_in[1];
  y = scale * sqrt(y);
  if (absxk < 0.0) {
    absxk = -1.0;
  } else if (absxk > 0.0) {
    absxk = 1.0;
  } else if (absxk == 0.0) {
    absxk = 0.0;
  } else {
    absxk = (rtNaN);
  }

  localB->e_out = -absxk * y;
}

static void matlabCodegenHandle_matlabCodeg(MoveArm_Position_Combined_Exp_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

//
// System initialize for atomic system:
//    synthesized block
//    synthesized block
//
void SendPositionCommandsto_Init(DW_SendPositionCommandstoMani_T *localDW)
{
  // Start for MATLABSystem: '<S276>/Send Position Commands to Manipulator'
  localDW->obj.matlabCodegenIsDeleted = false;
  localDW->objisempty = true;
  localDW->obj.isSetupComplete = true;
}

//
// Output and update for atomic system:
//    synthesized block
//    synthesized block
//
void SendPositionCommandstoManip(real_T rtu_0, real_T rtu_1, real_T rtu_2)
{
  // MATLABSystem: '<S276>/Send Position Commands to Manipulator'
  command_dynamixel_position(rtu_0, rtu_1, rtu_2);
}

//
// Termination for atomic system:
//    synthesized block
//    synthesized block
//
void SendPositionCommandsto_Term(DW_SendPositionCommandstoMani_T *localDW)
{
  // Terminate for MATLABSystem: '<S276>/Send Position Commands to Manipulator'
  matlabCodegenHandle_matlabCodeg(&localDW->obj);
}

//
// Output and update for action system:
//    '<S317>/Sub-Phase #2 '
//    '<S317>/Sub-Phase #3 '
//
void Combined_Experime_SubPhase2(P_SubPhase2_Combined_Experime_T *localP, real_T
  *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State)
{
  // DataStoreWrite: '<S322>/BLACK_Fx' incorporates:
  //   Constant: '<S322>/Constant'

  *rtd_BLACK_Fx = localP->Constant_Value;

  // DataStoreWrite: '<S322>/BLACK_Fy' incorporates:
  //   Constant: '<S322>/Constant'

  *rtd_BLACK_Fy = localP->Constant_Value;

  // DataStoreWrite: '<S322>/BLACK_Tz' incorporates:
  //   Constant: '<S322>/Constant'

  *rtd_BLACK_Tz = localP->Constant_Value;

  // DataStoreWrite: '<S322>/Data Store Write3' incorporates:
  //   Constant: '<S322>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// Output and update for atomic system:
//    '<S343>/MATLAB Function2'
//    '<S395>/MATLAB Function2'
//
void Combined__MATLABFunction2_b(real_T rtu_z, B_MATLABFunction2_Combined__k_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/Hough Control/MATLAB Function2': '<S353>:1' 
  // '<S353>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S353>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
}

//
// System initialize for action system:
//    '<S318>/Sub-Phase #1'
//    '<S318>/Sub-Phase #4'
//
void Combined_Exp_SubPhase1_Init(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW, P_SubPhase1_Combined_Experime_T
  *localP)
{
  // Start for If: '<S367>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSubs = -1;

  // Start for If: '<S369>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSu_j = -1;

  // Start for If: '<S365>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSu_h = -1;

  // InitializeConditions for Delay: '<S367>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S369>/Delay1'
  localDW->icLoad_p = 1U;

  // InitializeConditions for Delay: '<S365>/Delay1'
  localDW->icLoad_d = 1U;

  // SystemInitialize for IfAction SubSystem: '<S367>/Hold this value'
  // SystemInitialize for Outport: '<S368>/Out1'
  localB->In1_p = localP->Out1_Y0_m;

  // End of SystemInitialize for SubSystem: '<S367>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S369>/Hold this value'
  // SystemInitialize for Outport: '<S370>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S369>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S365>/Hold this value'
  // SystemInitialize for Outport: '<S366>/Out1'
  localB->In1_j = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S365>/Hold this value'
}

//
// Outputs for action system:
//    '<S318>/Sub-Phase #1'
//    '<S318>/Sub-Phase #4'
//
void Combined_Experime_SubPhase1(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW, P_SubPhase1_Combined_Experime_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  real_T rtb_Sum6_el;
  real_T rtb_Subtract2_o[2];

  // Sum: '<S356>/Subtract' incorporates:
  //   Constant: '<S356>/Desired Px (BLUE)'

  localB->Subtract = Combined_Experiment_ArmOnly_P.init_states_BLUE[0] -
    *rtd_BLUE_Px;

  // Delay: '<S367>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = localB->Subtract;
  }

  localB->Delay1 = localDW->Delay1_DSTATE;

  // End of Delay: '<S367>/Delay1'

  // Sum: '<S367>/Sum6'
  rtb_Sum6_el = localB->Subtract - localB->Delay1;

  // If: '<S367>/if we went through a "step"' incorporates:
  //   Inport: '<S368>/In1'

  localDW->ifwewentthroughastep_ActiveSubs = -1;
  if (rtb_Sum6_el != 0.0) {
    localDW->ifwewentthroughastep_ActiveSubs = 0;

    // Outputs for IfAction SubSystem: '<S367>/Hold this value' incorporates:
    //   ActionPort: '<S368>/Action Port'

    localB->In1_p = rtb_Sum6_el;

    // End of Outputs for SubSystem: '<S367>/Hold this value'
  }

  // End of If: '<S367>/if we went through a "step"'

  // Sum: '<S362>/Sum3' incorporates:
  //   Gain: '<S362>/kd_xb'
  //   Gain: '<S362>/kp_xb'
  //   Gain: '<S367>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1_p *
    Combined_Experiment_ArmOnly_P.Kd_xblue +
    Combined_Experiment_ArmOnly_P.Kp_xblue * localB->Subtract;

  // Sum: '<S356>/Subtract1' incorporates:
  //   Constant: '<S356>/Desired Py (BLUE)'

  localB->Subtract1 = Combined_Experiment_ArmOnly_P.init_states_BLUE[1] -
    *rtd_BLUE_Py;

  // Delay: '<S369>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_g = localB->Subtract1;
  }

  localB->Delay1_i = localDW->Delay1_DSTATE_g;

  // End of Delay: '<S369>/Delay1'

  // Sum: '<S369>/Sum6'
  rtb_Sum6_el = localB->Subtract1 - localB->Delay1_i;

  // If: '<S369>/if we went through a "step"' incorporates:
  //   Inport: '<S370>/In1'

  localDW->ifwewentthroughastep_ActiveSu_j = -1;
  if (rtb_Sum6_el != 0.0) {
    localDW->ifwewentthroughastep_ActiveSu_j = 0;

    // Outputs for IfAction SubSystem: '<S369>/Hold this value' incorporates:
    //   ActionPort: '<S370>/Action Port'

    localB->In1 = rtb_Sum6_el;

    // End of Outputs for SubSystem: '<S369>/Hold this value'
  }

  // End of If: '<S369>/if we went through a "step"'

  // Sum: '<S363>/Sum3' incorporates:
  //   Gain: '<S363>/kd_yb'
  //   Gain: '<S363>/kp_yb'
  //   Gain: '<S369>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1 *
    Combined_Experiment_ArmOnly_P.Kd_yblue +
    Combined_Experiment_ArmOnly_P.Kp_yblue * localB->Subtract1;

  // MATLAB Function: '<S364>/MATLAB Function2' incorporates:
  //   Constant: '<S356>/Desired Attitude (BLUE)'

  Combined__MATLABFunction2_o(Combined_Experiment_ArmOnly_P.init_states_BLUE[2],
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S364>/MATLAB Function3'
  Combined_Ex_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S364>/Subtract2'
  rtb_Subtract2_o[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  rtb_Subtract2_o[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S364>/MATLAB Function4'
  Combined_Ex_MATLABFunction4(localB->sf_MATLABFunction2.Oy, rtb_Subtract2_o,
    &localB->sf_MATLABFunction4);

  // Delay: '<S365>/Delay1'
  if (localDW->icLoad_d != 0) {
    localDW->Delay1_DSTATE_f = localB->sf_MATLABFunction4.e_out;
  }

  localB->Delay1_j = localDW->Delay1_DSTATE_f;

  // End of Delay: '<S365>/Delay1'

  // Sum: '<S365>/Sum6'
  rtb_Sum6_el = localB->sf_MATLABFunction4.e_out - localB->Delay1_j;

  // If: '<S365>/if we went through a "step"' incorporates:
  //   Inport: '<S366>/In1'

  localDW->ifwewentthroughastep_ActiveSu_h = -1;
  if (rtb_Sum6_el != 0.0) {
    localDW->ifwewentthroughastep_ActiveSu_h = 0;

    // Outputs for IfAction SubSystem: '<S365>/Hold this value' incorporates:
    //   ActionPort: '<S366>/Action Port'

    localB->In1_j = rtb_Sum6_el;

    // End of Outputs for SubSystem: '<S365>/Hold this value'
  }

  // End of If: '<S365>/if we went through a "step"'

  // Sum: '<S361>/Sum3' incorporates:
  //   Gain: '<S361>/kd_tb'
  //   Gain: '<S361>/kp_tb'
  //   Gain: '<S365>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1_j *
    Combined_Experiment_ArmOnly_P.Kd_tblue +
    Combined_Experiment_ArmOnly_P.Kp_tblue * localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S356>/Data Store Write1' incorporates:
  //   Constant: '<S356>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// Update for action system:
//    '<S318>/Sub-Phase #1'
//    '<S318>/Sub-Phase #4'
//
void Combined_E_SubPhase1_Update(B_SubPhase1_Combined_Experime_T *localB,
  DW_SubPhase1_Combined_Experim_T *localDW)
{
  // Update for Delay: '<S367>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = localB->Subtract;

  // Update for Delay: '<S369>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_g = localB->Subtract1;

  // Update for Delay: '<S365>/Delay1'
  localDW->icLoad_d = 0U;
  localDW->Delay1_DSTATE_f = localB->sf_MATLABFunction4.e_out;
}

//
// Output and update for action system:
//    '<S318>/Sub-Phase #2 '
//    '<S318>/Sub-Phase #3 '
//
void Combined_Experi_SubPhase2_m(P_SubPhase2_Combined_Experi_h_T *localP, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  // DataStoreWrite: '<S357>/BLUE_Fx' incorporates:
  //   Constant: '<S357>/Constant'

  *rtd_BLUE_Fx = localP->Constant_Value;

  // DataStoreWrite: '<S357>/BLUE_Fy' incorporates:
  //   Constant: '<S357>/Constant'

  *rtd_BLUE_Fy = localP->Constant_Value;

  // DataStoreWrite: '<S357>/BLUE_Tz' incorporates:
  //   Constant: '<S357>/Constant'

  *rtd_BLUE_Tz = localP->Constant_Value;

  // DataStoreWrite: '<S357>/Data Store Write1' incorporates:
  //   Constant: '<S357>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// Output and update for action system:
//    '<S319>/Sub-Phase #1'
//    '<S319>/Sub-Phase #2 '
//
void Combined_Experi_SubPhase1_j(P_SubPhase1_Combined_Experi_m_T *localP, real_T
  *rtd_Float_State, real_T *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T *rtd_RED_Tz,
  real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder,
  real_T *rtd_RED_Tz_Wrist)
{
  // DataStoreWrite: '<S387>/RED_Fx' incorporates:
  //   Constant: '<S387>/Constant'

  *rtd_RED_Fx = localP->Constant_Value;

  // DataStoreWrite: '<S387>/RED_Fy' incorporates:
  //   Constant: '<S387>/Constant'

  *rtd_RED_Fy = localP->Constant_Value;

  // DataStoreWrite: '<S387>/RED_Tz' incorporates:
  //   Constant: '<S387>/Constant'

  *rtd_RED_Tz = localP->Constant_Value;

  // DataStoreWrite: '<S387>/RED_Tz_RW' incorporates:
  //   Constant: '<S387>/Constant1'

  *rtd_RED_Tz_RW = localP->Constant1_Value;

  // DataStoreWrite: '<S387>/Data Store Write' incorporates:
  //   Constant: '<S387>/Constant4'

  *rtd_RED_Tz_Shoulder = localP->Constant4_Value;

  // DataStoreWrite: '<S387>/Data Store Write1' incorporates:
  //   Constant: '<S387>/Constant5'

  *rtd_RED_Tz_Elbow = localP->Constant5_Value;

  // DataStoreWrite: '<S387>/Data Store Write2' incorporates:
  //   Constant: '<S387>/Constant6'

  *rtd_RED_Tz_Wrist = localP->Constant6_Value;

  // DataStoreWrite: '<S387>/Data Store Write3' incorporates:
  //   Constant: '<S387>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// System initialize for action system:
//    '<S12>/Change BLUE Behavior'
//    '<S13>/Change BLUE Behavior'
//
void C_ChangeBLUEBehavior_b_Init(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW, P_ChangeBLUEBehavior_Combin_c_T
  *localP)
{
  // Start for If: '<S433>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSubs = -1;

  // Start for If: '<S435>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSu_j = -1;

  // Start for If: '<S431>/if we went through a "step"'
  localDW->ifwewentthroughastep_ActiveSu_a = -1;

  // InitializeConditions for Delay: '<S433>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S435>/Delay1'
  localDW->icLoad_h = 1U;

  // InitializeConditions for Delay: '<S431>/Delay1'
  localDW->icLoad_l = 1U;

  // SystemInitialize for IfAction SubSystem: '<S433>/Hold this value'
  // SystemInitialize for Outport: '<S434>/Out1'
  localB->In1_k = localP->Out1_Y0_k;

  // End of SystemInitialize for SubSystem: '<S433>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S435>/Hold this value'
  // SystemInitialize for Outport: '<S436>/Out1'
  localB->In1 = localP->Out1_Y0_n;

  // End of SystemInitialize for SubSystem: '<S435>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S431>/Hold this value'
  // SystemInitialize for Outport: '<S432>/Out1'
  localB->In1_a = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S431>/Hold this value'
}

//
// Outputs for action system:
//    '<S12>/Change BLUE Behavior'
//    '<S13>/Change BLUE Behavior'
//
void Combin_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW, P_ChangeBLUEBehavior_Combin_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  real_T rtb_Sum6_cb;
  real_T rtb_Subtract2_ic[2];

  // Sum: '<S411>/Subtract' incorporates:
  //   Constant: '<S411>/Desired Px (BLUE)'

  localB->Subtract = Combined_Experiment_ArmOnly_P.home_states_BLUE[0] -
    *rtd_BLUE_Px;

  // Delay: '<S433>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = localB->Subtract;
  }

  localB->Delay1 = localDW->Delay1_DSTATE;

  // End of Delay: '<S433>/Delay1'

  // Sum: '<S433>/Sum6'
  rtb_Sum6_cb = localB->Subtract - localB->Delay1;

  // If: '<S433>/if we went through a "step"' incorporates:
  //   Inport: '<S434>/In1'

  localDW->ifwewentthroughastep_ActiveSubs = -1;
  if (rtb_Sum6_cb != 0.0) {
    localDW->ifwewentthroughastep_ActiveSubs = 0;

    // Outputs for IfAction SubSystem: '<S433>/Hold this value' incorporates:
    //   ActionPort: '<S434>/Action Port'

    localB->In1_k = rtb_Sum6_cb;

    // End of Outputs for SubSystem: '<S433>/Hold this value'
  }

  // End of If: '<S433>/if we went through a "step"'

  // Sum: '<S428>/Sum3' incorporates:
  //   Gain: '<S428>/kd_xb'
  //   Gain: '<S428>/kp_xb'
  //   Gain: '<S433>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1_k *
    Combined_Experiment_ArmOnly_P.Kd_xblue +
    Combined_Experiment_ArmOnly_P.Kp_xblue * localB->Subtract;

  // Sum: '<S411>/Subtract1' incorporates:
  //   Constant: '<S411>/Desired Py (BLUE)'

  localB->Subtract1 = Combined_Experiment_ArmOnly_P.home_states_BLUE[1] -
    *rtd_BLUE_Py;

  // Delay: '<S435>/Delay1'
  if (localDW->icLoad_h != 0) {
    localDW->Delay1_DSTATE_n = localB->Subtract1;
  }

  localB->Delay1_m = localDW->Delay1_DSTATE_n;

  // End of Delay: '<S435>/Delay1'

  // Sum: '<S435>/Sum6'
  rtb_Sum6_cb = localB->Subtract1 - localB->Delay1_m;

  // If: '<S435>/if we went through a "step"' incorporates:
  //   Inport: '<S436>/In1'

  localDW->ifwewentthroughastep_ActiveSu_j = -1;
  if (rtb_Sum6_cb != 0.0) {
    localDW->ifwewentthroughastep_ActiveSu_j = 0;

    // Outputs for IfAction SubSystem: '<S435>/Hold this value' incorporates:
    //   ActionPort: '<S436>/Action Port'

    localB->In1 = rtb_Sum6_cb;

    // End of Outputs for SubSystem: '<S435>/Hold this value'
  }

  // End of If: '<S435>/if we went through a "step"'

  // Sum: '<S429>/Sum3' incorporates:
  //   Gain: '<S429>/kd_yb'
  //   Gain: '<S429>/kp_yb'
  //   Gain: '<S435>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1 *
    Combined_Experiment_ArmOnly_P.Kd_yblue +
    Combined_Experiment_ArmOnly_P.Kp_yblue * localB->Subtract1;

  // MATLAB Function: '<S430>/MATLAB Function2' incorporates:
  //   Constant: '<S411>/Desired Attitude (BLUE)'

  Combined__MATLABFunction2_o(Combined_Experiment_ArmOnly_P.home_states_BLUE[2],
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S430>/MATLAB Function3'
  Combined_Ex_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S430>/Subtract2'
  rtb_Subtract2_ic[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  rtb_Subtract2_ic[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S430>/MATLAB Function4'
  Combined_Ex_MATLABFunction4(localB->sf_MATLABFunction2.Oy, rtb_Subtract2_ic,
    &localB->sf_MATLABFunction4);

  // Delay: '<S431>/Delay1'
  if (localDW->icLoad_l != 0) {
    localDW->Delay1_DSTATE_k = localB->sf_MATLABFunction4.e_out;
  }

  localB->Delay1_p = localDW->Delay1_DSTATE_k;

  // End of Delay: '<S431>/Delay1'

  // Sum: '<S431>/Sum6'
  rtb_Sum6_cb = localB->sf_MATLABFunction4.e_out - localB->Delay1_p;

  // If: '<S431>/if we went through a "step"' incorporates:
  //   Inport: '<S432>/In1'

  localDW->ifwewentthroughastep_ActiveSu_a = -1;
  if (rtb_Sum6_cb != 0.0) {
    localDW->ifwewentthroughastep_ActiveSu_a = 0;

    // Outputs for IfAction SubSystem: '<S431>/Hold this value' incorporates:
    //   ActionPort: '<S432>/Action Port'

    localB->In1_a = rtb_Sum6_cb;

    // End of Outputs for SubSystem: '<S431>/Hold this value'
  }

  // End of If: '<S431>/if we went through a "step"'

  // Sum: '<S427>/Sum3' incorporates:
  //   Gain: '<S427>/kd_tb'
  //   Gain: '<S427>/kp_tb'
  //   Gain: '<S431>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Combined_Experiment_ArmOnly_P.serverRate * localB->In1_a *
    Combined_Experiment_ArmOnly_P.Kd_tblue +
    Combined_Experiment_ArmOnly_P.Kp_tblue * localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S411>/Data Store Write3' incorporates:
  //   Constant: '<S411>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// Update for action system:
//    '<S12>/Change BLUE Behavior'
//    '<S13>/Change BLUE Behavior'
//
void C_ChangeBLUEBehavior_Update(B_ChangeBLUEBehavior_Combin_k_T *localB,
  DW_ChangeBLUEBehavior_Combi_i_T *localDW)
{
  // Update for Delay: '<S433>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = localB->Subtract;

  // Update for Delay: '<S435>/Delay1'
  localDW->icLoad_h = 0U;
  localDW->Delay1_DSTATE_n = localB->Subtract1;

  // Update for Delay: '<S431>/Delay1'
  localDW->icLoad_l = 0U;
  localDW->Delay1_DSTATE_k = localB->sf_MATLABFunction4.e_out;
}

static real_T Combined_Experime_rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      Combined_Experiment_ArmOnly_B.u0 = 1;
    } else {
      Combined_Experiment_ArmOnly_B.u0 = -1;
    }

    if (u1 > 0.0) {
      Combined_Experiment_ArmOnly_B.u1 = 1;
    } else {
      Combined_Experiment_ArmOnly_B.u1 = -1;
    }

    y = atan2(static_cast<real_T>(Combined_Experiment_ArmOnly_B.u0),
              static_cast<real_T>(Combined_Experiment_ArmOnly_B.u1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static real_T Comb_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  // Uniform random number generator (random number between 0 and 1)

  // #define IA      16807                      magic multiplier = 7^5
  // #define IM      2147483647                 modulus = 2^31-1
  // #define IQ      127773                     IM div IA
  // #define IR      2836                       IM modulo IA
  // #define S       4.656612875245797e-10      reciprocal of 2^31-1
  // test = IA * (seed % IQ) - IR * (seed/IQ)
  // seed = test < 0 ? (test + IM) : test
  // return (seed*S)

  Combined_Experiment_ArmOnly_B.lo = *u % 127773U * 16807U;
  Combined_Experiment_ArmOnly_B.hi = *u / 127773U * 2836U;
  if (Combined_Experiment_ArmOnly_B.lo < Combined_Experiment_ArmOnly_B.hi) {
    *u = 2147483647U - (Combined_Experiment_ArmOnly_B.hi -
                        Combined_Experiment_ArmOnly_B.lo);
  } else {
    *u = Combined_Experiment_ArmOnly_B.lo - Combined_Experiment_ArmOnly_B.hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

static real_T Comb_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T y;

  // Normal (Gaussian) random number generator
  do {
    Combined_Experiment_ArmOnly_B.sr = 2.0 * Comb_rt_urand_Upu32_Yd_f_pw_snf(u)
      - 1.0;
    Combined_Experiment_ArmOnly_B.si = 2.0 * Comb_rt_urand_Upu32_Yd_f_pw_snf(u)
      - 1.0;
    Combined_Experiment_ArmOnly_B.si = Combined_Experiment_ArmOnly_B.sr *
      Combined_Experiment_ArmOnly_B.sr + Combined_Experiment_ArmOnly_B.si *
      Combined_Experiment_ArmOnly_B.si;
  } while (Combined_Experiment_ArmOnly_B.si > 1.0);

  y = sqrt(-2.0 * log(Combined_Experiment_ArmOnly_B.si) /
           Combined_Experiment_ArmOnly_B.si) * Combined_Experiment_ArmOnly_B.sr;
  return y;
}

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

static void Combined_Experim_rt_invd3x3_snf(const real_T u[9], real_T y[9])
{
  memcpy(&Combined_Experiment_ArmOnly_B.x[0], &u[0], 9U * sizeof(real_T));
  Combined_Experiment_ArmOnly_B.p1 = 1;
  Combined_Experiment_ArmOnly_B.p2 = 3;
  Combined_Experiment_ArmOnly_B.p3 = 6;
  Combined_Experiment_ArmOnly_B.absx11 = fabs(u[0]);
  Combined_Experiment_ArmOnly_B.absx21 = fabs(u[1]);
  Combined_Experiment_ArmOnly_B.absx31 = fabs(u[2]);
  if ((Combined_Experiment_ArmOnly_B.absx21 >
       Combined_Experiment_ArmOnly_B.absx11) &&
      (Combined_Experiment_ArmOnly_B.absx21 >
       Combined_Experiment_ArmOnly_B.absx31)) {
    Combined_Experiment_ArmOnly_B.p1 = 4;
    Combined_Experiment_ArmOnly_B.p2 = 0;
    Combined_Experiment_ArmOnly_B.x[0] = u[1];
    Combined_Experiment_ArmOnly_B.x[1] = u[0];
    Combined_Experiment_ArmOnly_B.x[3] = u[4];
    Combined_Experiment_ArmOnly_B.x[4] = u[3];
    Combined_Experiment_ArmOnly_B.x[6] = u[7];
    Combined_Experiment_ArmOnly_B.x[7] = u[6];
  } else {
    if (Combined_Experiment_ArmOnly_B.absx31 >
        Combined_Experiment_ArmOnly_B.absx11) {
      Combined_Experiment_ArmOnly_B.p1 = 7;
      Combined_Experiment_ArmOnly_B.p3 = 0;
      Combined_Experiment_ArmOnly_B.x[2] = Combined_Experiment_ArmOnly_B.x[0];
      Combined_Experiment_ArmOnly_B.x[0] = u[2];
      Combined_Experiment_ArmOnly_B.x[5] = Combined_Experiment_ArmOnly_B.x[3];
      Combined_Experiment_ArmOnly_B.x[3] = u[5];
      Combined_Experiment_ArmOnly_B.x[8] = Combined_Experiment_ArmOnly_B.x[6];
      Combined_Experiment_ArmOnly_B.x[6] = u[8];
    }
  }

  Combined_Experiment_ArmOnly_B.absx31 = Combined_Experiment_ArmOnly_B.x[1] /
    Combined_Experiment_ArmOnly_B.x[0];
  Combined_Experiment_ArmOnly_B.x[1] = Combined_Experiment_ArmOnly_B.absx31;
  Combined_Experiment_ArmOnly_B.absx11 = Combined_Experiment_ArmOnly_B.x[2] /
    Combined_Experiment_ArmOnly_B.x[0];
  Combined_Experiment_ArmOnly_B.x[2] = Combined_Experiment_ArmOnly_B.absx11;
  Combined_Experiment_ArmOnly_B.x[4] -= Combined_Experiment_ArmOnly_B.absx31 *
    Combined_Experiment_ArmOnly_B.x[3];
  Combined_Experiment_ArmOnly_B.x[5] -= Combined_Experiment_ArmOnly_B.absx11 *
    Combined_Experiment_ArmOnly_B.x[3];
  Combined_Experiment_ArmOnly_B.x[7] -= Combined_Experiment_ArmOnly_B.absx31 *
    Combined_Experiment_ArmOnly_B.x[6];
  Combined_Experiment_ArmOnly_B.x[8] -= Combined_Experiment_ArmOnly_B.absx11 *
    Combined_Experiment_ArmOnly_B.x[6];
  if (fabs(Combined_Experiment_ArmOnly_B.x[5]) > fabs
      (Combined_Experiment_ArmOnly_B.x[4])) {
    Combined_Experiment_ArmOnly_B.itmp = Combined_Experiment_ArmOnly_B.p2;
    Combined_Experiment_ArmOnly_B.p2 = Combined_Experiment_ArmOnly_B.p3;
    Combined_Experiment_ArmOnly_B.p3 = Combined_Experiment_ArmOnly_B.itmp;
    Combined_Experiment_ArmOnly_B.x[1] = Combined_Experiment_ArmOnly_B.absx11;
    Combined_Experiment_ArmOnly_B.x[2] = Combined_Experiment_ArmOnly_B.absx31;
    Combined_Experiment_ArmOnly_B.absx11 = Combined_Experiment_ArmOnly_B.x[4];
    Combined_Experiment_ArmOnly_B.x[4] = Combined_Experiment_ArmOnly_B.x[5];
    Combined_Experiment_ArmOnly_B.x[5] = Combined_Experiment_ArmOnly_B.absx11;
    Combined_Experiment_ArmOnly_B.absx11 = Combined_Experiment_ArmOnly_B.x[7];
    Combined_Experiment_ArmOnly_B.x[7] = Combined_Experiment_ArmOnly_B.x[8];
    Combined_Experiment_ArmOnly_B.x[8] = Combined_Experiment_ArmOnly_B.absx11;
  }

  Combined_Experiment_ArmOnly_B.absx31 = Combined_Experiment_ArmOnly_B.x[5] /
    Combined_Experiment_ArmOnly_B.x[4];
  Combined_Experiment_ArmOnly_B.x[8] -= Combined_Experiment_ArmOnly_B.absx31 *
    Combined_Experiment_ArmOnly_B.x[7];
  Combined_Experiment_ArmOnly_B.absx11 = (Combined_Experiment_ArmOnly_B.absx31 *
    Combined_Experiment_ArmOnly_B.x[1] - Combined_Experiment_ArmOnly_B.x[2]) /
    Combined_Experiment_ArmOnly_B.x[8];
  Combined_Experiment_ArmOnly_B.absx21 = -(Combined_Experiment_ArmOnly_B.x[7] *
    Combined_Experiment_ArmOnly_B.absx11 + Combined_Experiment_ArmOnly_B.x[1]) /
    Combined_Experiment_ArmOnly_B.x[4];
  y[Combined_Experiment_ArmOnly_B.p1 - 1] = ((1.0 -
    Combined_Experiment_ArmOnly_B.x[3] * Combined_Experiment_ArmOnly_B.absx21) -
    Combined_Experiment_ArmOnly_B.x[6] * Combined_Experiment_ArmOnly_B.absx11) /
    Combined_Experiment_ArmOnly_B.x[0];
  y[Combined_Experiment_ArmOnly_B.p1] = Combined_Experiment_ArmOnly_B.absx21;
  y[Combined_Experiment_ArmOnly_B.p1 + 1] = Combined_Experiment_ArmOnly_B.absx11;
  Combined_Experiment_ArmOnly_B.absx11 = -Combined_Experiment_ArmOnly_B.absx31 /
    Combined_Experiment_ArmOnly_B.x[8];
  Combined_Experiment_ArmOnly_B.absx21 = (1.0 - Combined_Experiment_ArmOnly_B.x
    [7] * Combined_Experiment_ArmOnly_B.absx11) /
    Combined_Experiment_ArmOnly_B.x[4];
  y[Combined_Experiment_ArmOnly_B.p2] = -(Combined_Experiment_ArmOnly_B.x[3] *
    Combined_Experiment_ArmOnly_B.absx21 + Combined_Experiment_ArmOnly_B.x[6] *
    Combined_Experiment_ArmOnly_B.absx11) / Combined_Experiment_ArmOnly_B.x[0];
  y[Combined_Experiment_ArmOnly_B.p2 + 1] = Combined_Experiment_ArmOnly_B.absx21;
  y[Combined_Experiment_ArmOnly_B.p2 + 2] = Combined_Experiment_ArmOnly_B.absx11;
  Combined_Experiment_ArmOnly_B.absx11 = 1.0 / Combined_Experiment_ArmOnly_B.x[8];
  Combined_Experiment_ArmOnly_B.absx21 = -Combined_Experiment_ArmOnly_B.x[7] *
    Combined_Experiment_ArmOnly_B.absx11 / Combined_Experiment_ArmOnly_B.x[4];
  y[Combined_Experiment_ArmOnly_B.p3] = -(Combined_Experiment_ArmOnly_B.x[3] *
    Combined_Experiment_ArmOnly_B.absx21 + Combined_Experiment_ArmOnly_B.x[6] *
    Combined_Experiment_ArmOnly_B.absx11) / Combined_Experiment_ArmOnly_B.x[0];
  y[Combined_Experiment_ArmOnly_B.p3 + 1] = Combined_Experiment_ArmOnly_B.absx21;
  y[Combined_Experiment_ArmOnly_B.p3 + 2] = Combined_Experiment_ArmOnly_B.absx11;
}

static void Combined_E_SystemCore_release_b(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(10U);
  }
}

static void Combined_E_SystemCore_delete_ba(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Combined_E_SystemCore_release_b(obj);
}

static void matlabCodegenHandle_matlabCo_ba(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Combined_E_SystemCore_delete_ba(obj);
  }
}

static void Combined__SystemCore_release_ba(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(26U);
  }
}

static void Combined__SystemCore_delete_ba5(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Combined__SystemCore_release_ba(obj);
}

static void matlabCodegenHandle_matlabC_ba5(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Combined__SystemCore_delete_ba5(obj);
  }
}

static void C_SystemCore_rele_a(const PhaseSpace_ALL_Combined_Exper_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    terminate_phasespace();
  }
}

static void C_SystemCore_dele_p(const PhaseSpace_ALL_Combined_Exper_T *obj)
{
  C_SystemCore_rele_a(obj);
}

static void Combined_Expe_matlabCodegenHa_p(PhaseSpace_ALL_Combined_Exper_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_dele_p(obj);
  }
}

static void C_SystemCore_rele_p(const raspi_internal_lsm9ds1Block_C_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_I2C_Close(obj->i2cobj_A_G.MW_I2C_HANDLE);
    MW_I2C_Close(obj->i2cobj_MAG.MW_I2C_HANDLE);
  }
}

static void C_SystemCore_dele_l(const raspi_internal_lsm9ds1Block_C_T *obj)
{
  C_SystemCore_rele_p(obj);
}

static void Combined_Expe_matlabCodegenHa_l(raspi_internal_lsm9ds1Block_C_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_dele_l(obj);
  }
}

static void C_SystemCore_rele_k(j_codertarget_raspi_internal__T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void C_SystemCore_dele_e(j_codertarget_raspi_internal__T *obj)
{
  C_SystemCore_rele_k(obj);
}

static void Combined_Expe_matlabCodegenHa_e(j_codertarget_raspi_internal__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_dele_e(obj);
  }
}

static void C_SystemCore_rele_l(i_codertarget_raspi_internal__T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void C_SystemCore_dele_k(i_codertarget_raspi_internal__T *obj)
{
  C_SystemCore_rele_l(obj);
}

static void Combined_Expe_matlabCodegenHa_k(i_codertarget_raspi_internal__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_dele_k(obj);
  }
}

static void Combined_Exp_SystemCore_release(const
  InitializeForPosition_Combine_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    terminate_dynamixel();
  }
}

static void Combined_Ex_SystemCore_delete_b(const
  InitializeForPosition_Combine_T *obj)
{
  Combined_Exp_SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCod_b(InitializeForPosition_Combine_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Combined_Ex_SystemCore_delete_b(obj);
  }
}

static void Combined_SystemCore_release_ba5(const
  raspi_internal_PWMBlock_Combi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    EXT_PWMBlock_terminate(obj->PinNumber);
  }
}

static void Combined_SystemCore_delete_ba5m(const
  raspi_internal_PWMBlock_Combi_T *obj)
{
  Combined_SystemCore_release_ba5(obj);
}

static void matlabCodegenHandle_matlab_ba5m(raspi_internal_PWMBlock_Combi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Combined_SystemCore_delete_ba5m(obj);
  }
}

static void SystemCore_release_ba5m25ew4zuq(const
  codertarget_linux_blocks_Di_b_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(24U);
  }
}

static void SystemCore_delete_ba5m25ew4zuqz(const
  codertarget_linux_blocks_Di_b_T *obj)
{
  SystemCore_release_ba5m25ew4zuq(obj);
}

static void matlabCodegenHand_ba5m25ew4zuqz(codertarget_linux_blocks_Di_b_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_ba5m25ew4zuqz(obj);
  }
}

static void SystemCore_releas_ba5m25ew4zuqz(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(21U);
  }
}

static void SystemCore_delet_ba5m25ew4zuqzr(const
  codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_releas_ba5m25ew4zuqz(obj);
}

static void matlabCodegenHan_ba5m25ew4zuqzr(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delet_ba5m25ew4zuqzr(obj);
  }
}

static void SystemCore_rele_ba5m25ew4zuqzro(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(25U);
  }
}

static void C_SystemCore_dele_a(const codertarget_linux_blocks_Digi_T *obj)
{
  SystemCore_rele_ba5m25ew4zuqzro(obj);
}

static void Combined_Expe_matlabCodegenHa_a(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    C_SystemCore_dele_a(obj);
  }
}

static raspi_internal_lsm9ds1Block_C_T *Combi_lsm9ds1Block_lsm9ds1Block
  (raspi_internal_lsm9ds1Block_C_T *obj)
{
  raspi_internal_lsm9ds1Block_C_T *b_obj;
  int32_T i;
  static const int8_T tmp[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  for (i = 0; i < 9; i++) {
    obj->CalGyroA[i] = tmp[i];
  }

  obj->CalGyroB[0] = 0.0;
  obj->CalGyroB[1] = 0.0;
  obj->CalGyroB[2] = 0.0;
  for (i = 0; i < 9; i++) {
    obj->CalAccelA[i] = tmp[i];
  }

  obj->CalAccelB[0] = 0.0;
  obj->CalAccelB[1] = 0.0;
  obj->CalAccelB[2] = 0.0;
  for (i = 0; i < 9; i++) {
    obj->CalMagA[i] = tmp[i];
  }

  obj->CalMagB[0] = 0.0;
  obj->CalMagB[1] = 0.0;
  obj->CalMagB[2] = 0.0;
  obj->isInitialized = 0;
  b_obj = obj;
  obj->i2cobj_A_G.isInitialized = 0;
  obj->i2cobj_A_G.matlabCodegenIsDeleted = false;
  obj->i2cobj_MAG.isInitialized = 0;
  obj->i2cobj_MAG.matlabCodegenIsDeleted = false;
  obj->matlabCodegenIsDeleted = false;
  return b_obj;
}

static void Combined_Exper_SystemCore_setup(raspi_internal_lsm9ds1Block_C_T *obj)
{
  uint32_T i2cname;
  uint8_T b_SwappedDataBytes[2];
  uint8_T CastedData;
  uint8_T SwappedDataBytes[2];
  obj->isSetupComplete = false;
  obj->isInitialized = 1;
  Combined_Experiment_ArmOnly_B.ModeType = MW_I2C_MASTER;
  i2cname = 1;
  obj->i2cobj_A_G.MW_I2C_HANDLE = MW_I2C_Open(i2cname,
    Combined_Experiment_ArmOnly_B.ModeType);
  obj->i2cobj_A_G.BusSpeed = 100000U;
  MW_I2C_SetBusSpeed(obj->i2cobj_A_G.MW_I2C_HANDLE, obj->i2cobj_A_G.BusSpeed);
  CastedData = 96U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 16U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 0U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 17U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 18U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 56U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 30U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 0U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 19U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 56U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 31U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 103U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 32U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 0U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 33U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 4U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 34U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_A_G.MW_I2C_HANDLE, 106U, SwappedDataBytes, 2U,
                     false, false);
  Combined_Experiment_ArmOnly_B.ModeType = MW_I2C_MASTER;
  i2cname = 1;
  obj->i2cobj_MAG.MW_I2C_HANDLE = MW_I2C_Open(i2cname,
    Combined_Experiment_ArmOnly_B.ModeType);
  obj->i2cobj_MAG.BusSpeed = 100000U;
  MW_I2C_SetBusSpeed(obj->i2cobj_MAG.MW_I2C_HANDLE, obj->i2cobj_MAG.BusSpeed);
  CastedData = 24U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 32U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_MAG.MW_I2C_HANDLE, 28U, SwappedDataBytes, 2U,
                     false, false);
  CastedData = 0U;
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 33U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_MAG.MW_I2C_HANDLE, 28U, SwappedDataBytes, 2U,
                     false, false);
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 34U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_MAG.MW_I2C_HANDLE, 28U, SwappedDataBytes, 2U,
                     false, false);
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 35U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_MAG.MW_I2C_HANDLE, 28U, SwappedDataBytes, 2U,
                     false, false);
  memcpy((void *)&b_SwappedDataBytes[1], (void *)&CastedData, (uint32_T)((size_t)
          1 * sizeof(uint8_T)));
  b_SwappedDataBytes[0] = 36U;
  memcpy((void *)&SwappedDataBytes[0], (void *)&b_SwappedDataBytes[0], (uint32_T)
         ((size_t)2 * sizeof(uint8_T)));
  MW_I2C_MasterWrite(obj->i2cobj_MAG.MW_I2C_HANDLE, 28U, SwappedDataBytes, 2U,
                     false, false);
  obj->isSetupComplete = true;
}

// Model step function for TID0
void Combined_Experiment_ArmOnly_step0(void) // Sample time: [0.0s, 0.0s]
{
  {                                    // Sample time: [0.0s, 0.0s]
    rate_monotonic_scheduler();
  }

  {
    char_T *sErr;
    static const int8_T b[8] = { -1, -1, 0, 0, 1, 1, 0, 0 };

    static const int8_T c[8] = { 0, 0, 1, 1, 0, 0, -1, -1 };

    // If: '<Root>/Separate Phases' incorporates:
    //   Constant: '<Root>/Constant'
    //   Constant: '<Root>/Constant1'
    //   Constant: '<Root>/Constant2'
    //   Constant: '<Root>/Constant3'
    //   Constant: '<Root>/Constant4'
    //   Constant: '<Root>/Constant6'
    //   DataStoreRead: '<S17>/Universal_Time'

    if (Combined_Experiment_ArmOnly_DW.Univ_Time <
        Combined_Experiment_ArmOnly_P.Phase0_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 0;

      // Outputs for IfAction SubSystem: '<Root>/Phase #0:  Wait for Synchronization' incorporates:
      //   ActionPort: '<S8>/Action Port'

      Phase0WaitforSynchronizatio
        (&Combined_Experiment_ArmOnly_DW.Phase0WaitforSynchronization,
         &Combined_Experiment_ArmOnly_P.Phase0WaitforSynchronization,
         &Combined_Experiment_ArmOnly_DW.BLACK_Fx,
         &Combined_Experiment_ArmOnly_DW.BLACK_Fy,
         &Combined_Experiment_ArmOnly_DW.BLACK_Tz,
         &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
         &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
         &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
         &Combined_Experiment_ArmOnly_DW.Float_State,
         &Combined_Experiment_ArmOnly_DW.RED_Fx,
         &Combined_Experiment_ArmOnly_DW.RED_Fy,
         &Combined_Experiment_ArmOnly_DW.RED_Tz,
         &Combined_Experiment_ArmOnly_B.Sum6_l1,
         &Combined_Experiment_ArmOnly_DW.RED_Tz_RW,
         &Combined_Experiment_ArmOnly_B.Sum6_dk,
         &Combined_Experiment_ArmOnly_B.RED_Tz_Wrist);

      // End of Outputs for SubSystem: '<Root>/Phase #0:  Wait for Synchronization' 
    } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
               Combined_Experiment_ArmOnly_P.Phase1_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 1;

      // Outputs for IfAction SubSystem: '<Root>/Phase #1:  Start Floating ' incorporates:
      //   ActionPort: '<S9>/Action Port'

      Phase0WaitforSynchronizatio
        (&Combined_Experiment_ArmOnly_DW.Phase1StartFloating,
         &Combined_Experiment_ArmOnly_P.Phase1StartFloating,
         &Combined_Experiment_ArmOnly_DW.BLACK_Fx,
         &Combined_Experiment_ArmOnly_DW.BLACK_Fy,
         &Combined_Experiment_ArmOnly_DW.BLACK_Tz,
         &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
         &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
         &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
         &Combined_Experiment_ArmOnly_DW.Float_State,
         &Combined_Experiment_ArmOnly_DW.RED_Fx,
         &Combined_Experiment_ArmOnly_DW.RED_Fy,
         &Combined_Experiment_ArmOnly_DW.RED_Tz,
         &Combined_Experiment_ArmOnly_B.Sum6_l1,
         &Combined_Experiment_ArmOnly_DW.RED_Tz_RW,
         &Combined_Experiment_ArmOnly_B.Sum6_dk,
         &Combined_Experiment_ArmOnly_B.RED_Tz_Wrist);

      // End of Outputs for SubSystem: '<Root>/Phase #1:  Start Floating '
    } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
               Combined_Experiment_ArmOnly_P.Phase2_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 2;

      // Outputs for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' incorporates:
      //   ActionPort: '<S10>/Action Port'

      // If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S10>/Constant'
      //   Constant: '<S277>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_dy = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_dy = 0;

        // Outputs for IfAction SubSystem: '<S10>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S274>/Action Port'

        // Sum: '<S274>/Subtract' incorporates:
        //   Constant: '<S274>/Desired Px (BLACK)'
        //   DataStoreRead: '<S274>/BLACK_Px'

        Combined_Experiment_ArmOnly_B.Subtract_ou =
          Combined_Experiment_ArmOnly_P.init_states_BLACK[0] -
          Combined_Experiment_ArmOnly_DW.BLACK_Px;

        // Delay: '<S284>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_ns != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k =
            Combined_Experiment_ArmOnly_B.Subtract_ou;
        }

        // Sum: '<S284>/Sum6' incorporates:
        //   Delay: '<S284>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_ou -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k;

        // If: '<S284>/if we went through a "step"' incorporates:
        //   Inport: '<S285>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S284>/Hold this value' incorporates:
          //   ActionPort: '<S285>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_e =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S284>/Hold this value'
        }

        // End of If: '<S284>/if we went through a "step"'

        // Sum: '<S279>/Sum3' incorporates:
        //   DataStoreWrite: '<S274>/BLACK_Fx'
        //   Gain: '<S279>/kd_xb'
        //   Gain: '<S279>/kp_xb'
        //   Gain: '<S284>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_e *
          Combined_Experiment_ArmOnly_P.Kd_xb +
          Combined_Experiment_ArmOnly_P.Kp_xb *
          Combined_Experiment_ArmOnly_B.Subtract_ou;

        // Sum: '<S274>/Subtract1' incorporates:
        //   Constant: '<S274>/Desired Py (BLACK)'
        //   DataStoreRead: '<S274>/BLACK_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_m0 =
          Combined_Experiment_ArmOnly_P.init_states_BLACK[1] -
          Combined_Experiment_ArmOnly_DW.BLACK_Py;

        // Delay: '<S286>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_k1 != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_d =
            Combined_Experiment_ArmOnly_B.Subtract1_m0;
        }

        // Sum: '<S286>/Sum6' incorporates:
        //   Delay: '<S286>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_m0 -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_d;

        // If: '<S286>/if we went through a "step"' incorporates:
        //   Inport: '<S287>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S286>/Hold this value' incorporates:
          //   ActionPort: '<S287>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_gc =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S286>/Hold this value'
        }

        // End of If: '<S286>/if we went through a "step"'

        // Sum: '<S280>/Sum3' incorporates:
        //   DataStoreWrite: '<S274>/BLACK_Fy'
        //   Gain: '<S280>/kd_yb'
        //   Gain: '<S280>/kp_yb'
        //   Gain: '<S286>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_gc *
          Combined_Experiment_ArmOnly_P.Kd_yb +
          Combined_Experiment_ArmOnly_P.Kp_yb *
          Combined_Experiment_ArmOnly_B.Subtract1_m0;

        // MATLAB Function: '<S281>/MATLAB Function2' incorporates:
        //   Constant: '<S274>/Desired Attitude (BLACK)'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.init_states_BLACK[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_o);

        // MATLAB Function: '<S281>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S274>/BLACK_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3);

        // Sum: '<S281>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_o.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_o.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3.Ox[1];

        // MATLAB Function: '<S281>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_o.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4);

        // Delay: '<S282>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_ep != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_j =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4.e_out;
        }

        // Sum: '<S282>/Sum6' incorporates:
        //   Delay: '<S282>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_j;

        // If: '<S282>/if we went through a "step"' incorporates:
        //   Inport: '<S283>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S282>/Hold this value' incorporates:
          //   ActionPort: '<S283>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_fau =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S282>/Hold this value'
        }

        // End of If: '<S282>/if we went through a "step"'

        // Sum: '<S278>/Sum3' incorporates:
        //   DataStoreWrite: '<S274>/BLACK_Tz'
        //   Gain: '<S278>/kd_tb'
        //   Gain: '<S278>/kp_tb'
        //   Gain: '<S282>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_fau *
          Combined_Experiment_ArmOnly_P.Kd_tb +
          Combined_Experiment_ArmOnly_P.Kp_tb *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4.e_out;

        // DataStoreWrite: '<S274>/Data Store Write3' incorporates:
        //   Constant: '<S274>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value;

        // End of Outputs for SubSystem: '<S10>/Change BLACK Behavior'
      }

      // End of If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S10>/Constant'
      //   Constant: '<S277>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jm = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jm = 0;

        // Outputs for IfAction SubSystem: '<S10>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S275>/Action Port'

        // Sum: '<S275>/Subtract' incorporates:
        //   Constant: '<S275>/Desired Px (BLUE)'
        //   DataStoreRead: '<S275>/BLUE_Px'

        Combined_Experiment_ArmOnly_B.Subtract_j =
          Combined_Experiment_ArmOnly_P.init_states_BLUE[0] -
          Combined_Experiment_ArmOnly_DW.BLUE_Px;

        // Delay: '<S297>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_dy != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fb =
            Combined_Experiment_ArmOnly_B.Subtract_j;
        }

        // Sum: '<S297>/Sum6' incorporates:
        //   Delay: '<S297>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_j -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fb;

        // If: '<S297>/if we went through a "step"' incorporates:
        //   Inport: '<S298>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S297>/Hold this value' incorporates:
          //   ActionPort: '<S298>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_lc =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S297>/Hold this value'
        }

        // End of If: '<S297>/if we went through a "step"'

        // Sum: '<S292>/Sum3' incorporates:
        //   DataStoreWrite: '<S275>/BLUE_Fx'
        //   Gain: '<S292>/kd_xb'
        //   Gain: '<S292>/kp_xb'
        //   Gain: '<S297>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLUE_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_lc *
          Combined_Experiment_ArmOnly_P.Kd_xblue +
          Combined_Experiment_ArmOnly_P.Kp_xblue *
          Combined_Experiment_ArmOnly_B.Subtract_j;

        // Sum: '<S275>/Subtract1' incorporates:
        //   Constant: '<S275>/Desired Py (BLUE)'
        //   DataStoreRead: '<S275>/BLUE_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_o =
          Combined_Experiment_ArmOnly_P.init_states_BLUE[1] -
          Combined_Experiment_ArmOnly_DW.BLUE_Py;

        // Delay: '<S299>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_hf != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n1 =
            Combined_Experiment_ArmOnly_B.Subtract1_o;
        }

        // Sum: '<S299>/Sum6' incorporates:
        //   Delay: '<S299>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_o -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n1;

        // If: '<S299>/if we went through a "step"' incorporates:
        //   Inport: '<S300>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S299>/Hold this value' incorporates:
          //   ActionPort: '<S300>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_jw =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S299>/Hold this value'
        }

        // End of If: '<S299>/if we went through a "step"'

        // Sum: '<S293>/Sum3' incorporates:
        //   DataStoreWrite: '<S275>/BLUE_Fy'
        //   Gain: '<S293>/kd_yb'
        //   Gain: '<S293>/kp_yb'
        //   Gain: '<S299>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLUE_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_jw *
          Combined_Experiment_ArmOnly_P.Kd_yblue +
          Combined_Experiment_ArmOnly_P.Kp_yblue *
          Combined_Experiment_ArmOnly_B.Subtract1_o;

        // MATLAB Function: '<S294>/MATLAB Function2' incorporates:
        //   Constant: '<S275>/Desired Attitude (BLUE)'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.init_states_BLUE[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_m);

        // MATLAB Function: '<S294>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S275>/BLUE_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLUE_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c);

        // Sum: '<S294>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_m.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_m.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c.Ox[1];

        // MATLAB Function: '<S294>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_m.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_c);

        // Delay: '<S295>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_kl != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cq =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_c.e_out;
        }

        // Sum: '<S295>/Sum6' incorporates:
        //   Delay: '<S295>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_c.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cq;

        // If: '<S295>/if we went through a "step"' incorporates:
        //   Inport: '<S296>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S295>/Hold this value' incorporates:
          //   ActionPort: '<S296>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_o =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S295>/Hold this value'
        }

        // End of If: '<S295>/if we went through a "step"'

        // Sum: '<S291>/Sum3' incorporates:
        //   DataStoreWrite: '<S275>/BLUE_Tz'
        //   Gain: '<S291>/kd_tb'
        //   Gain: '<S291>/kp_tb'
        //   Gain: '<S295>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLUE_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_o *
          Combined_Experiment_ArmOnly_P.Kd_tblue +
          Combined_Experiment_ArmOnly_P.Kp_tblue *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_c.e_out;

        // DataStoreWrite: '<S275>/Data Store Write3' incorporates:
        //   Constant: '<S275>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_o;

        // End of Outputs for SubSystem: '<S10>/Change BLUE Behavior'
      }

      // End of If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S10>/Constant'
      //   Constant: '<S276>/Constant7'
      //   Constant: '<S276>/Constant8'
      //   Constant: '<S276>/Constant9'
      //   Constant: '<S277>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_aj = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_aj = 0;

        // Outputs for IfAction SubSystem: '<S10>/Change RED Behavior' incorporates:
        //   ActionPort: '<S276>/Action Port'

        // MATLAB Function: '<S307>/MATLAB Function2' incorporates:
        //   Constant: '<S276>/Constant'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.init_states_RED[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ol);

        // MATLAB Function: '<S307>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S276>/RED_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.RED_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_k);

        // Sum: '<S307>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ol.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_k.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ol.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_k.Ox[1];

        // MATLAB Function: '<S307>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ol.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_b);

        // Delay: '<S308>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_ji != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_b.e_out;
        }

        // Sum: '<S308>/Sum6' incorporates:
        //   Delay: '<S308>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_b.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct;

        // If: '<S308>/if we went through a "step"' incorporates:
        //   Inport: '<S309>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S308>/Hold this value' incorporates:
          //   ActionPort: '<S309>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_k =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S308>/Hold this value'
        }

        // End of If: '<S308>/if we went through a "step"'

        // Sum: '<S304>/Sum3' incorporates:
        //   DataStoreWrite: '<S276>/RED_Tz'
        //   Gain: '<S304>/kd_tr'
        //   Gain: '<S304>/kp_tr'
        //   Gain: '<S308>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_k *
          Combined_Experiment_ArmOnly_P.Kd_tr +
          Combined_Experiment_ArmOnly_P.Kp_tr *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_b.e_out;

        // Sum: '<S276>/Subtract' incorporates:
        //   Constant: '<S276>/Constant1'
        //   DataStoreRead: '<S276>/RED_Px'

        Combined_Experiment_ArmOnly_B.Subtract_f =
          Combined_Experiment_ArmOnly_P.init_states_RED[0] -
          Combined_Experiment_ArmOnly_DW.RED_Px;

        // Delay: '<S310>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_a1 != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bm =
            Combined_Experiment_ArmOnly_B.Subtract_f;
        }

        // Sum: '<S310>/Sum6' incorporates:
        //   Delay: '<S310>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_f -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bm;

        // If: '<S310>/if we went through a "step"' incorporates:
        //   Inport: '<S311>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S310>/Hold this value' incorporates:
          //   ActionPort: '<S311>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_cr =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S310>/Hold this value'
        }

        // End of If: '<S310>/if we went through a "step"'

        // Sum: '<S305>/Sum3' incorporates:
        //   DataStoreWrite: '<S276>/RED_Fx'
        //   Gain: '<S305>/kd_xr'
        //   Gain: '<S305>/kp_xr'
        //   Gain: '<S310>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_cr *
          Combined_Experiment_ArmOnly_P.Kd_xr +
          Combined_Experiment_ArmOnly_P.Kp_xr *
          Combined_Experiment_ArmOnly_B.Subtract_f;

        // DataStoreWrite: '<S276>/RED_Tz_RW1' incorporates:
        //   Constant: '<S276>/Constant2'

        Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
          Combined_Experiment_ArmOnly_P.Constant2_Value;

        // Sum: '<S276>/Subtract1' incorporates:
        //   Constant: '<S276>/Constant3'
        //   DataStoreRead: '<S276>/RED_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_dr =
          Combined_Experiment_ArmOnly_P.init_states_RED[1] -
          Combined_Experiment_ArmOnly_DW.RED_Py;

        // Delay: '<S312>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_o0 != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pr =
            Combined_Experiment_ArmOnly_B.Subtract1_dr;
        }

        // Sum: '<S312>/Sum6' incorporates:
        //   Delay: '<S312>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_dr -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pr;

        // If: '<S312>/if we went through a "step"' incorporates:
        //   Inport: '<S313>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S312>/Hold this value' incorporates:
          //   ActionPort: '<S313>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_if =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S312>/Hold this value'
        }

        // End of If: '<S312>/if we went through a "step"'

        // Sum: '<S306>/Sum3' incorporates:
        //   DataStoreWrite: '<S276>/RED_Fy'
        //   Gain: '<S306>/kd_yr'
        //   Gain: '<S306>/kp_yr'
        //   Gain: '<S312>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_if *
          Combined_Experiment_ArmOnly_P.Kd_yr +
          Combined_Experiment_ArmOnly_P.Kp_yr *
          Combined_Experiment_ArmOnly_B.Subtract1_dr;

        // DataStoreWrite: '<S276>/Data Store Write3' incorporates:
        //   Constant: '<S276>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_l;
        SendPositionCommandstoManip
          (Combined_Experiment_ArmOnly_P.Constant7_Value,
           Combined_Experiment_ArmOnly_P.Constant8_Value,
           Combined_Experiment_ArmOnly_P.Constant9_Value);

        // End of Outputs for SubSystem: '<S10>/Change RED Behavior'
      }

      // End of If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
               Combined_Experiment_ArmOnly_P.Phase3_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 3;

      // Outputs for IfAction SubSystem: '<Root>/Phase #3: Experiment' incorporates:
      //   ActionPort: '<S11>/Action Port'

      // If: '<S11>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S11>/Constant'
      //   Constant: '<S320>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_p = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_p = 0;

        // Outputs for IfAction SubSystem: '<S11>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S317>/Action Port'

        // If: '<S317>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S317>/Constant1'
        //   Constant: '<S317>/Constant2'
        //   Constant: '<S317>/Constant3'
        //   Constant: '<S317>/Constant4'
        //   DataStoreRead: '<S325>/Universal_Time'

        Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = -1;
        if (Combined_Experiment_ArmOnly_DW.Univ_Time <
            Combined_Experiment_ArmOnly_P.Phase3_SubPhase1_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = 0;

          // Outputs for IfAction SubSystem: '<S317>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S321>/Action Port'

          // Sum: '<S321>/Subtract' incorporates:
          //   Constant: '<S321>/Desired X-Position (BLACK)'
          //   DataStoreRead: '<S321>/BLACK_Px'

          Combined_Experiment_ArmOnly_B.Subtract_oi =
            Combined_Experiment_ArmOnly_P.init_states_BLACK[0] -
            Combined_Experiment_ArmOnly_DW.BLACK_Px;

          // Delay: '<S332>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ot != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_g =
              Combined_Experiment_ArmOnly_B.Subtract_oi;
          }

          // Sum: '<S332>/Sum6' incorporates:
          //   Delay: '<S332>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Subtract_oi -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_g;

          // If: '<S332>/if we went through a "step"' incorporates:
          //   Inport: '<S333>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S332>/Hold this value' incorporates:
            //   ActionPort: '<S333>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_cw =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S332>/Hold this value'
          }

          // End of If: '<S332>/if we went through a "step"'

          // Sum: '<S327>/Sum3' incorporates:
          //   DataStoreWrite: '<S321>/BLACK_Fx'
          //   Gain: '<S327>/kd_xb'
          //   Gain: '<S327>/kp_xb'
          //   Gain: '<S332>/divide by delta T'

          Combined_Experiment_ArmOnly_DW.BLACK_Fx = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_cw *
            Combined_Experiment_ArmOnly_P.Kd_xb +
            Combined_Experiment_ArmOnly_P.Kp_xb *
            Combined_Experiment_ArmOnly_B.Subtract_oi;

          // Sum: '<S321>/Subtract1' incorporates:
          //   Constant: '<S321>/Desired Y-Position (BLACK)'
          //   DataStoreRead: '<S321>/BLACK_Py '

          Combined_Experiment_ArmOnly_B.Subtract1_d2 =
            Combined_Experiment_ArmOnly_P.init_states_BLACK[1] -
            Combined_Experiment_ArmOnly_DW.BLACK_Py;

          // Delay: '<S334>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_k != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pe =
              Combined_Experiment_ArmOnly_B.Subtract1_d2;
          }

          // Sum: '<S334>/Sum6' incorporates:
          //   Delay: '<S334>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Subtract1_d2 -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pe;

          // If: '<S334>/if we went through a "step"' incorporates:
          //   Inport: '<S335>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S334>/Hold this value' incorporates:
            //   ActionPort: '<S335>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_ly =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S334>/Hold this value'
          }

          // End of If: '<S334>/if we went through a "step"'

          // Sum: '<S328>/Sum3' incorporates:
          //   DataStoreWrite: '<S321>/BLACK_Fy'
          //   Gain: '<S328>/kd_yb'
          //   Gain: '<S328>/kp_yb'
          //   Gain: '<S334>/divide by delta T'

          Combined_Experiment_ArmOnly_DW.BLACK_Fy = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_ly *
            Combined_Experiment_ArmOnly_P.Kd_yb +
            Combined_Experiment_ArmOnly_P.Kp_yb *
            Combined_Experiment_ArmOnly_B.Subtract1_d2;

          // MATLAB Function: '<S329>/MATLAB Function2' incorporates:
          //   Constant: '<S321>/Desired Attitude (BLACK)'

          Combined__MATLABFunction2_o
            (Combined_Experiment_ArmOnly_P.init_states_BLACK[2],
             &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_i);

          // MATLAB Function: '<S329>/MATLAB Function3' incorporates:
          //   DataStoreRead: '<S321>/BLACK_Rz'

          Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
            &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_l);

          // Sum: '<S329>/Subtract2'
          Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_i.Ox[0] -
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_l.Ox[0];
          Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_i.Ox[1] -
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_l.Ox[1];

          // MATLAB Function: '<S329>/MATLAB Function4'
          Combined_Ex_MATLABFunction4
            (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_i.Oy,
             Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
             &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_d);

          // Delay: '<S330>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_l != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_d.e_out;
          }

          // Sum: '<S330>/Sum6' incorporates:
          //   Delay: '<S330>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_d.e_out -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu;

          // If: '<S330>/if we went through a "step"' incorporates:
          //   Inport: '<S331>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S330>/Hold this value' incorporates:
            //   ActionPort: '<S331>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_a =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S330>/Hold this value'
          }

          // End of If: '<S330>/if we went through a "step"'

          // Sum: '<S326>/Sum3' incorporates:
          //   DataStoreWrite: '<S321>/BLACK_Tz'
          //   Gain: '<S326>/kd_tb'
          //   Gain: '<S326>/kp_tb'
          //   Gain: '<S330>/divide by delta T'

          Combined_Experiment_ArmOnly_DW.BLACK_Tz = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_a *
            Combined_Experiment_ArmOnly_P.Kd_tb +
            Combined_Experiment_ArmOnly_P.Kp_tb *
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_d.e_out;

          // DataStoreWrite: '<S321>/Data Store Write3' incorporates:
          //   Constant: '<S321>/Puck State'

          Combined_Experiment_ArmOnly_DW.Float_State =
            Combined_Experiment_ArmOnly_P.PuckState_Value_n;

          // End of Outputs for SubSystem: '<S317>/Sub-Phase #1'
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase2_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = 1;

          // Outputs for IfAction SubSystem: '<S317>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S322>/Action Port'

          Combined_Experime_SubPhase2(&Combined_Experiment_ArmOnly_P.SubPhase2,
            &Combined_Experiment_ArmOnly_DW.BLACK_Fx,
            &Combined_Experiment_ArmOnly_DW.BLACK_Fy,
            &Combined_Experiment_ArmOnly_DW.BLACK_Tz,
            &Combined_Experiment_ArmOnly_DW.Float_State);

          // End of Outputs for SubSystem: '<S317>/Sub-Phase #2 '
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase3_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = 2;

          // Outputs for IfAction SubSystem: '<S317>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S323>/Action Port'

          Combined_Experime_SubPhase2(&Combined_Experiment_ArmOnly_P.SubPhase3,
            &Combined_Experiment_ArmOnly_DW.BLACK_Fx,
            &Combined_Experiment_ArmOnly_DW.BLACK_Fy,
            &Combined_Experiment_ArmOnly_DW.BLACK_Tz,
            &Combined_Experiment_ArmOnly_DW.Float_State);

          // End of Outputs for SubSystem: '<S317>/Sub-Phase #3 '
        } else {
          if (Combined_Experiment_ArmOnly_DW.Univ_Time <
              Combined_Experiment_ArmOnly_P.Phase3_SubPhase4_End) {
            Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = 3;

            // Outputs for IfAction SubSystem: '<S317>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S324>/Action Port'

            // Sum: '<S324>/Subtract' incorporates:
            //   Constant: '<S324>/Desired X-Position (BLACK)'
            //   DataStoreRead: '<S324>/BLACK_Px'

            Combined_Experiment_ArmOnly_B.Subtract_a =
              Combined_Experiment_ArmOnly_P.xLength / 2.0 -
              Combined_Experiment_ArmOnly_DW.BLACK_Px;

            // Delay: '<S349>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_fr != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hp =
                Combined_Experiment_ArmOnly_B.Subtract_a;
            }

            // Sum: '<S349>/Sum6' incorporates:
            //   Delay: '<S349>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_B.Subtract_a -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hp;

            // If: '<S349>/if we went through a "step"' incorporates:
            //   Inport: '<S350>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
              // Outputs for IfAction SubSystem: '<S349>/Hold this value' incorporates:
              //   ActionPort: '<S350>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_l =
                Combined_Experiment_ArmOnly_B.Sum6_l1;

              // End of Outputs for SubSystem: '<S349>/Hold this value'
            }

            // End of If: '<S349>/if we went through a "step"'

            // Sum: '<S341>/Sum3' incorporates:
            //   DataStoreWrite: '<S324>/RED_Fx'
            //   Gain: '<S341>/kd_xb'
            //   Gain: '<S341>/kp_xb'
            //   Gain: '<S349>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.BLACK_Fx = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_l *
              Combined_Experiment_ArmOnly_P.Kd_xb +
              Combined_Experiment_ArmOnly_P.Kp_xb *
              Combined_Experiment_ArmOnly_B.Subtract_a;

            // Sum: '<S324>/Subtract1' incorporates:
            //   Constant: '<S324>/Desired Y-Position (BLACK)'
            //   DataStoreRead: '<S324>/BLACK_Py '

            Combined_Experiment_ArmOnly_B.Subtract1_h =
              Combined_Experiment_ArmOnly_P.yLength / 2.0 -
              Combined_Experiment_ArmOnly_DW.BLACK_Py;

            // Delay: '<S351>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_me != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ha =
                Combined_Experiment_ArmOnly_B.Subtract1_h;
            }

            // Sum: '<S351>/Sum6' incorporates:
            //   Delay: '<S351>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_B.Subtract1_h -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ha;

            // If: '<S351>/if we went through a "step"' incorporates:
            //   Inport: '<S352>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
              // Outputs for IfAction SubSystem: '<S351>/Hold this value' incorporates:
              //   ActionPort: '<S352>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_n =
                Combined_Experiment_ArmOnly_B.Sum6_l1;

              // End of Outputs for SubSystem: '<S351>/Hold this value'
            }

            // End of If: '<S351>/if we went through a "step"'

            // Sum: '<S342>/Sum3' incorporates:
            //   DataStoreWrite: '<S324>/RED_Fy'
            //   Gain: '<S342>/kd_yb'
            //   Gain: '<S342>/kp_yb'
            //   Gain: '<S351>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.BLACK_Fy = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_n *
              Combined_Experiment_ArmOnly_P.Kd_yb +
              Combined_Experiment_ArmOnly_P.Kp_yb *
              Combined_Experiment_ArmOnly_B.Subtract1_h;

            // Product: '<S339>/Product' incorporates:
            //   Constant: '<S339>/Desired Rate (BLACK)'
            //   Constant: '<S344>/Constant1'
            //   DataStoreRead: '<S344>/Universal_Time'
            //   Sum: '<S344>/Subtract'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              (Combined_Experiment_ArmOnly_DW.Univ_Time -
               Combined_Experiment_ArmOnly_P.Phase3_SubPhase3_End) *
              Combined_Experiment_ArmOnly_P.DesiredRateBLACK_Value;

            // MATLAB Function: '<S343>/MATLAB Function2' incorporates:
            //   MATLAB Function: '<S345>/MATLAB Function'

            // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #4/BLACK_RzD1/Wrap1/MATLAB Function': '<S346>:1' 
            // '<S346>:1:3' wrappedAngle = atan2(sin(u),cos(u));
            Combined__MATLABFunction2_b(Combined_Experime_rt_atan2d_snf(sin
              (Combined_Experiment_ArmOnly_B.Sum6_l1), cos
              (Combined_Experiment_ArmOnly_B.Sum6_l1)),
              &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_b);

            // MATLAB Function: '<S343>/MATLAB Function3' incorporates:
            //   DataStoreRead: '<S324>/BLACK_Rz'

            Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
              &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_ly);

            // Sum: '<S343>/Subtract2'
            Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_b.Ox[0] -
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_ly.Ox[0];
            Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_b.Ox[1] -
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_ly.Ox[1];

            // MATLAB Function: '<S343>/MATLAB Function4'
            Combined_Ex_MATLABFunction4
              (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_b.Oy,
               Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
               &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_dk);

            // Delay: '<S347>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_ey != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_li =
                Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_dk.e_out;
            }

            // Sum: '<S347>/Sum6' incorporates:
            //   Delay: '<S347>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_dk.e_out -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_li;

            // If: '<S347>/if we went through a "step"' incorporates:
            //   Inport: '<S348>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
              // Outputs for IfAction SubSystem: '<S347>/Hold this value' incorporates:
              //   ActionPort: '<S348>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_ck =
                Combined_Experiment_ArmOnly_B.Sum6_l1;

              // End of Outputs for SubSystem: '<S347>/Hold this value'
            }

            // End of If: '<S347>/if we went through a "step"'

            // Sum: '<S340>/Sum3' incorporates:
            //   DataStoreWrite: '<S324>/BLACK_Tz'
            //   Gain: '<S340>/kd_tb'
            //   Gain: '<S340>/kp_tb'
            //   Gain: '<S347>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.BLACK_Tz = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_ck *
              Combined_Experiment_ArmOnly_P.Kd_tb +
              Combined_Experiment_ArmOnly_P.Kp_tb *
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_dk.e_out;

            // DataStoreWrite: '<S324>/Data Store Write3' incorporates:
            //   Constant: '<S324>/Puck State'

            Combined_Experiment_ArmOnly_DW.Float_State =
              Combined_Experiment_ArmOnly_P.PuckState_Value_e;

            // End of Outputs for SubSystem: '<S317>/Sub-Phase #4'
          }
        }

        // End of If: '<S317>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S11>/Change BLACK Behavior'
      }

      // End of If: '<S11>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S11>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S11>/Constant'
      //   Constant: '<S320>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_pu = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_pu = 0;

        // Outputs for IfAction SubSystem: '<S11>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S318>/Action Port'

        // If: '<S318>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S318>/Constant1'
        //   Constant: '<S318>/Constant2'
        //   Constant: '<S318>/Constant3'
        //   Constant: '<S318>/Constant4'
        //   DataStoreRead: '<S360>/Universal_Time'

        Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = -1;
        if (Combined_Experiment_ArmOnly_DW.Univ_Time <
            Combined_Experiment_ArmOnly_P.Phase3_SubPhase1_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = 0;

          // Outputs for IfAction SubSystem: '<S318>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S356>/Action Port'

          Combined_Experime_SubPhase1(&Combined_Experiment_ArmOnly_B.SubPhase1_a,
            &Combined_Experiment_ArmOnly_DW.SubPhase1_a,
            &Combined_Experiment_ArmOnly_P.SubPhase1_a,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
            &Combined_Experiment_ArmOnly_DW.BLUE_Px,
            &Combined_Experiment_ArmOnly_DW.BLUE_Py,
            &Combined_Experiment_ArmOnly_DW.BLUE_Rz,
            &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
            &Combined_Experiment_ArmOnly_DW.Float_State);

          // End of Outputs for SubSystem: '<S318>/Sub-Phase #1'
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase2_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = 1;

          // Outputs for IfAction SubSystem: '<S318>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S357>/Action Port'

          Combined_Experi_SubPhase2_m(&Combined_Experiment_ArmOnly_P.SubPhase2_m,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
            &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
            &Combined_Experiment_ArmOnly_DW.Float_State);

          // End of Outputs for SubSystem: '<S318>/Sub-Phase #2 '
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase3_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = 2;

          // Outputs for IfAction SubSystem: '<S318>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S358>/Action Port'

          Combined_Experi_SubPhase2_m(&Combined_Experiment_ArmOnly_P.SubPhase3_p,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
            &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
            &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
            &Combined_Experiment_ArmOnly_DW.Float_State);

          // End of Outputs for SubSystem: '<S318>/Sub-Phase #3 '
        } else {
          if (Combined_Experiment_ArmOnly_DW.Univ_Time <
              Combined_Experiment_ArmOnly_P.Phase3_SubPhase4_End) {
            Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = 3;

            // Outputs for IfAction SubSystem: '<S318>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S359>/Action Port'

            Combined_Experime_SubPhase1
              (&Combined_Experiment_ArmOnly_B.SubPhase4_f,
               &Combined_Experiment_ArmOnly_DW.SubPhase4_f,
               &Combined_Experiment_ArmOnly_P.SubPhase4_f,
               &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
               &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
               &Combined_Experiment_ArmOnly_DW.BLUE_Px,
               &Combined_Experiment_ArmOnly_DW.BLUE_Py,
               &Combined_Experiment_ArmOnly_DW.BLUE_Rz,
               &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
               &Combined_Experiment_ArmOnly_DW.Float_State);

            // End of Outputs for SubSystem: '<S318>/Sub-Phase #4'
          }
        }

        // End of If: '<S318>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S11>/Change BLUE Behavior'
      }

      // End of If: '<S11>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S11>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S11>/Constant'
      //   Constant: '<S320>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_au = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_au = 0;

        // Outputs for IfAction SubSystem: '<S11>/Change RED Behavior' incorporates:
        //   ActionPort: '<S319>/Action Port'

        // If: '<S319>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S319>/Constant1'
        //   Constant: '<S319>/Constant2'
        //   Constant: '<S319>/Constant3'
        //   Constant: '<S319>/Constant4'
        //   DataStoreRead: '<S391>/Universal_Time'

        Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = -1;
        if (Combined_Experiment_ArmOnly_DW.Univ_Time <
            Combined_Experiment_ArmOnly_P.Phase3_SubPhase1_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = 0;

          // Outputs for IfAction SubSystem: '<S319>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S387>/Action Port'

          Combined_Experi_SubPhase1_j(&Combined_Experiment_ArmOnly_P.SubPhase1_j,
            &Combined_Experiment_ArmOnly_DW.Float_State,
            &Combined_Experiment_ArmOnly_DW.RED_Fx,
            &Combined_Experiment_ArmOnly_DW.RED_Fy,
            &Combined_Experiment_ArmOnly_DW.RED_Tz,
            &Combined_Experiment_ArmOnly_B.Sum6_l1,
            &Combined_Experiment_ArmOnly_DW.RED_Tz_RW,
            &Combined_Experiment_ArmOnly_B.Sum6_dk,
            &Combined_Experiment_ArmOnly_B.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S319>/Sub-Phase #1'
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase2_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = 1;

          // Outputs for IfAction SubSystem: '<S319>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S388>/Action Port'

          Combined_Experi_SubPhase1_j(&Combined_Experiment_ArmOnly_P.SubPhase2_g,
            &Combined_Experiment_ArmOnly_DW.Float_State,
            &Combined_Experiment_ArmOnly_DW.RED_Fx,
            &Combined_Experiment_ArmOnly_DW.RED_Fy,
            &Combined_Experiment_ArmOnly_DW.RED_Tz,
            &Combined_Experiment_ArmOnly_B.Sum6_l1,
            &Combined_Experiment_ArmOnly_DW.RED_Tz_RW,
            &Combined_Experiment_ArmOnly_B.Sum6_dk,
            &Combined_Experiment_ArmOnly_B.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S319>/Sub-Phase #2 '
        } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
                   Combined_Experiment_ArmOnly_P.Phase3_SubPhase3_End) {
          Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = 2;

          // Outputs for IfAction SubSystem: '<S319>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S389>/Action Port'

          // DataStoreWrite: '<S389>/RED_Fx' incorporates:
          //   Constant: '<S389>/Constant'

          Combined_Experiment_ArmOnly_DW.RED_Fx =
            Combined_Experiment_ArmOnly_P.Constant_Value_f;

          // DataStoreWrite: '<S389>/RED_Fy' incorporates:
          //   Constant: '<S389>/Constant'

          Combined_Experiment_ArmOnly_DW.RED_Fy =
            Combined_Experiment_ArmOnly_P.Constant_Value_f;

          // DataStoreWrite: '<S389>/RED_Tz' incorporates:
          //   Constant: '<S389>/Constant'

          Combined_Experiment_ArmOnly_DW.RED_Tz =
            Combined_Experiment_ArmOnly_P.Constant_Value_f;

          // DataStoreWrite: '<S389>/RED_Tz_RW' incorporates:
          //   Constant: '<S389>/Constant1'

          Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
            Combined_Experiment_ArmOnly_P.Constant1_Value;

          // DataStoreWrite: '<S389>/Data Store Write3' incorporates:
          //   Constant: '<S389>/Puck State'

          Combined_Experiment_ArmOnly_DW.Float_State =
            Combined_Experiment_ArmOnly_P.PuckState_Value_lm;

          // FromWorkspace: '<S389>/From Workspace'
          {
            real_T *pDataValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace_PWORK.DataPtr;
            real_T *pTimeValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace_PWORK.TimePtr;
            int_T currTimeIndex =
              Combined_Experiment_ArmOnly_DW.FromWorkspace_IWORK.PrevIndex;
            real_T t = Combined_Experiment_ArmOnly_M->Timing.t[0];

            // Get index
            if (t <= pTimeValues[0]) {
              currTimeIndex = 0;
            } else if (t >= pTimeValues[9]) {
              currTimeIndex = 8;
            } else {
              if (t < pTimeValues[currTimeIndex]) {
                while (t < pTimeValues[currTimeIndex]) {
                  currTimeIndex--;
                }
              } else {
                while (t >= pTimeValues[currTimeIndex + 1]) {
                  currTimeIndex++;
                }
              }
            }

            Combined_Experiment_ArmOnly_DW.FromWorkspace_IWORK.PrevIndex =
              currTimeIndex;

            // Post output
            {
              real_T t1 = pTimeValues[currTimeIndex];
              real_T t2 = pTimeValues[currTimeIndex + 1];
              if (t1 == t2) {
                if (t < t1) {
                  Combined_Experiment_ArmOnly_B.FromWorkspace =
                    pDataValues[currTimeIndex];
                } else {
                  Combined_Experiment_ArmOnly_B.FromWorkspace =
                    pDataValues[currTimeIndex + 1];
                }
              } else {
                real_T f1 = (t2 - t) / (t2 - t1);
                real_T f2 = 1.0 - f1;
                real_T d1;
                real_T d2;
                int_T TimeIndex= currTimeIndex;
                d1 = pDataValues[TimeIndex];
                d2 = pDataValues[TimeIndex + 1];
                Combined_Experiment_ArmOnly_B.FromWorkspace = (real_T)
                  rtInterpolate(d1, d2, f1, f2);
                pDataValues += 10;
              }
            }
          }

          // FromWorkspace: '<S389>/From Workspace1'
          {
            real_T *pDataValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace1_PWORK.DataPtr;
            real_T *pTimeValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace1_PWORK.TimePtr;
            int_T currTimeIndex =
              Combined_Experiment_ArmOnly_DW.FromWorkspace1_IWORK.PrevIndex;
            real_T t = Combined_Experiment_ArmOnly_M->Timing.t[0];

            // Get index
            if (t <= pTimeValues[0]) {
              currTimeIndex = 0;
            } else if (t >= pTimeValues[9]) {
              currTimeIndex = 8;
            } else {
              if (t < pTimeValues[currTimeIndex]) {
                while (t < pTimeValues[currTimeIndex]) {
                  currTimeIndex--;
                }
              } else {
                while (t >= pTimeValues[currTimeIndex + 1]) {
                  currTimeIndex++;
                }
              }
            }

            Combined_Experiment_ArmOnly_DW.FromWorkspace1_IWORK.PrevIndex =
              currTimeIndex;

            // Post output
            {
              real_T t1 = pTimeValues[currTimeIndex];
              real_T t2 = pTimeValues[currTimeIndex + 1];
              if (t1 == t2) {
                if (t < t1) {
                  Combined_Experiment_ArmOnly_B.FromWorkspace1 =
                    pDataValues[currTimeIndex];
                } else {
                  Combined_Experiment_ArmOnly_B.FromWorkspace1 =
                    pDataValues[currTimeIndex + 1];
                }
              } else {
                real_T f1 = (t2 - t) / (t2 - t1);
                real_T f2 = 1.0 - f1;
                real_T d1;
                real_T d2;
                int_T TimeIndex= currTimeIndex;
                d1 = pDataValues[TimeIndex];
                d2 = pDataValues[TimeIndex + 1];
                Combined_Experiment_ArmOnly_B.FromWorkspace1 = (real_T)
                  rtInterpolate(d1, d2, f1, f2);
                pDataValues += 10;
              }
            }
          }

          // FromWorkspace: '<S389>/From Workspace2'
          {
            real_T *pDataValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace2_PWORK.DataPtr;
            real_T *pTimeValues = (real_T *)
              Combined_Experiment_ArmOnly_DW.FromWorkspace2_PWORK.TimePtr;
            int_T currTimeIndex =
              Combined_Experiment_ArmOnly_DW.FromWorkspace2_IWORK.PrevIndex;
            real_T t = Combined_Experiment_ArmOnly_M->Timing.t[0];

            // Get index
            if (t <= pTimeValues[0]) {
              currTimeIndex = 0;
            } else if (t >= pTimeValues[9]) {
              currTimeIndex = 8;
            } else {
              if (t < pTimeValues[currTimeIndex]) {
                while (t < pTimeValues[currTimeIndex]) {
                  currTimeIndex--;
                }
              } else {
                while (t >= pTimeValues[currTimeIndex + 1]) {
                  currTimeIndex++;
                }
              }
            }

            Combined_Experiment_ArmOnly_DW.FromWorkspace2_IWORK.PrevIndex =
              currTimeIndex;

            // Post output
            {
              real_T t1 = pTimeValues[currTimeIndex];
              real_T t2 = pTimeValues[currTimeIndex + 1];
              if (t1 == t2) {
                if (t < t1) {
                  Combined_Experiment_ArmOnly_B.FromWorkspace2 =
                    pDataValues[currTimeIndex];
                } else {
                  Combined_Experiment_ArmOnly_B.FromWorkspace2 =
                    pDataValues[currTimeIndex + 1];
                }
              } else {
                real_T f1 = (t2 - t) / (t2 - t1);
                real_T f2 = 1.0 - f1;
                real_T d1;
                real_T d2;
                int_T TimeIndex= currTimeIndex;
                d1 = pDataValues[TimeIndex];
                d2 = pDataValues[TimeIndex + 1];
                Combined_Experiment_ArmOnly_B.FromWorkspace2 = (real_T)
                  rtInterpolate(d1, d2, f1, f2);
                pDataValues += 10;
              }
            }
          }

          SendPositionCommandstoManip
            (Combined_Experiment_ArmOnly_B.FromWorkspace,
             Combined_Experiment_ArmOnly_B.FromWorkspace1,
             Combined_Experiment_ArmOnly_B.FromWorkspace2);

          // End of Outputs for SubSystem: '<S319>/Sub-Phase #3 '
        } else {
          if (Combined_Experiment_ArmOnly_DW.Univ_Time <
              Combined_Experiment_ArmOnly_P.Phase3_SubPhase4_End) {
            Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = 3;

            // Outputs for IfAction SubSystem: '<S319>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S390>/Action Port'

            // DataStoreWrite: '<S390>/RED_Tz_RW' incorporates:
            //   Constant: '<S390>/Constant1'

            Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
              Combined_Experiment_ArmOnly_P.Constant1_Value_f;

            // MATLAB Function: '<S395>/MATLAB Function2' incorporates:
            //   Constant: '<S398>/Constant'
            //   DataStoreRead: '<S398>/Data Store Read'
            //   DataStoreRead: '<S398>/Data Store Read1'
            //   DataStoreRead: '<S398>/Data Store Read2'
            //   DataStoreRead: '<S398>/Data Store Read3'
            //   Sum: '<S398>/Subtract'
            //   Sum: '<S398>/Subtract1'
            //   Sum: '<S398>/Sum'
            //   Trigonometry: '<S398>/Atan2'

            Combined__MATLABFunction2_b(Combined_Experime_rt_atan2d_snf
              (Combined_Experiment_ArmOnly_DW.BLACK_Py -
               Combined_Experiment_ArmOnly_DW.RED_Py,
               Combined_Experiment_ArmOnly_DW.BLACK_Px -
               Combined_Experiment_ArmOnly_DW.RED_Px) +
              Combined_Experiment_ArmOnly_P.Constant_Value_bf,
              &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_g);

            // MATLAB Function: '<S395>/MATLAB Function3' incorporates:
            //   DataStoreRead: '<S390>/RED_Rz'

            Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.RED_Rz,
              &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_lg);

            // Sum: '<S395>/Subtract2'
            Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_g.Ox[0] -
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_lg.Ox[0];
            Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_g.Ox[1] -
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_lg.Ox[1];

            // MATLAB Function: '<S395>/MATLAB Function4'
            Combined_Ex_MATLABFunction4
              (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_g.Oy,
               Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
               &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_n);

            // Delay: '<S400>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_jl != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c =
                Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_n.e_out;
            }

            // Sum: '<S400>/Sum6' incorporates:
            //   Delay: '<S400>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_n.e_out -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c;

            // If: '<S400>/if we went through a "step"' incorporates:
            //   Inport: '<S401>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
              // Outputs for IfAction SubSystem: '<S400>/Hold this value' incorporates:
              //   ActionPort: '<S401>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_fa =
                Combined_Experiment_ArmOnly_B.Sum6_l1;

              // End of Outputs for SubSystem: '<S400>/Hold this value'
            }

            // End of If: '<S400>/if we went through a "step"'

            // Sum: '<S392>/Sum3' incorporates:
            //   DataStoreWrite: '<S390>/RED_Tz'
            //   Gain: '<S392>/kd_tr'
            //   Gain: '<S392>/kp_tr'
            //   Gain: '<S400>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.RED_Tz = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_fa *
              Combined_Experiment_ArmOnly_P.Kd_tr +
              Combined_Experiment_ArmOnly_P.Kp_tr *
              Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_n.e_out;

            // Sum: '<S399>/Subtract3' incorporates:
            //   Constant: '<S399>/Constant2'
            //   DataStoreRead: '<S409>/Universal_Time'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_DW.Univ_Time -
              Combined_Experiment_ArmOnly_P.Phase3_SubPhase3_End;

            // Sum: '<S390>/Subtract' incorporates:
            //   DataStoreRead: '<S390>/RED_Px'
            //   MATLAB Function: '<S390>/MATLAB Function'

            // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function': '<S396>:1' 
            // '<S396>:1:3' x = 0.7*cos((0.034906585)*t) + xLength/2;
            Combined_Experiment_ArmOnly_B.Subtract_b = (cos(0.034906585 *
              Combined_Experiment_ArmOnly_B.Sum6_l1) * 0.7 +
              Combined_Experiment_ArmOnly_P.xLength / 2.0) -
              Combined_Experiment_ArmOnly_DW.RED_Px;

            // Delay: '<S402>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_n != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mu =
                Combined_Experiment_ArmOnly_B.Subtract_b;
            }

            // Sum: '<S402>/Sum6' incorporates:
            //   Delay: '<S402>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_dk =
              Combined_Experiment_ArmOnly_B.Subtract_b -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mu;

            // If: '<S402>/if we went through a "step"' incorporates:
            //   Inport: '<S403>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
              // Outputs for IfAction SubSystem: '<S402>/Hold this value' incorporates:
              //   ActionPort: '<S403>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_go =
                Combined_Experiment_ArmOnly_B.Sum6_dk;

              // End of Outputs for SubSystem: '<S402>/Hold this value'
            }

            // End of If: '<S402>/if we went through a "step"'

            // Sum: '<S393>/Sum3' incorporates:
            //   DataStoreWrite: '<S390>/RED_Fx'
            //   Gain: '<S393>/kd_xr'
            //   Gain: '<S393>/kp_xr'
            //   Gain: '<S402>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.RED_Fx = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_go *
              Combined_Experiment_ArmOnly_P.Kd_xr +
              Combined_Experiment_ArmOnly_P.Kp_xr *
              Combined_Experiment_ArmOnly_B.Subtract_b;

            // Sum: '<S390>/Subtract1' incorporates:
            //   DataStoreRead: '<S390>/RED_Py '
            //   MATLAB Function: '<S390>/MATLAB Function1'

            // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/MATLAB Function1': '<S397>:1' 
            // '<S397>:1:3' y = 0.7*sin((0.034906585)*t) + yLength/2;
            Combined_Experiment_ArmOnly_B.Subtract1_m = (sin(0.034906585 *
              Combined_Experiment_ArmOnly_B.Sum6_l1) * 0.7 +
              Combined_Experiment_ArmOnly_P.yLength / 2.0) -
              Combined_Experiment_ArmOnly_DW.RED_Py;

            // Delay: '<S404>/Delay1'
            if (Combined_Experiment_ArmOnly_DW.icLoad_g != 0) {
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_il =
                Combined_Experiment_ArmOnly_B.Subtract1_m;
            }

            // Sum: '<S404>/Sum6' incorporates:
            //   Delay: '<S404>/Delay1'

            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_B.Subtract1_m -
              Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_il;

            // If: '<S404>/if we went through a "step"' incorporates:
            //   Inport: '<S405>/In1'

            if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
              // Outputs for IfAction SubSystem: '<S404>/Hold this value' incorporates:
              //   ActionPort: '<S405>/Action Port'

              Combined_Experiment_ArmOnly_B.In1_g4 =
                Combined_Experiment_ArmOnly_B.Sum6_l1;

              // End of Outputs for SubSystem: '<S404>/Hold this value'
            }

            // End of If: '<S404>/if we went through a "step"'

            // Sum: '<S394>/Sum3' incorporates:
            //   DataStoreWrite: '<S390>/RED_Fy'
            //   Gain: '<S394>/kd_yr'
            //   Gain: '<S394>/kp_yr'
            //   Gain: '<S404>/divide by delta T'

            Combined_Experiment_ArmOnly_DW.RED_Fy = 1.0 /
              Combined_Experiment_ArmOnly_P.serverRate *
              Combined_Experiment_ArmOnly_B.In1_g4 *
              Combined_Experiment_ArmOnly_P.Kd_yr +
              Combined_Experiment_ArmOnly_P.Kp_yr *
              Combined_Experiment_ArmOnly_B.Subtract1_m;

            // DataStoreWrite: '<S390>/Data Store Write3' incorporates:
            //   Constant: '<S390>/Puck State'

            Combined_Experiment_ArmOnly_DW.Float_State =
              Combined_Experiment_ArmOnly_P.PuckState_Value_f;

            // End of Outputs for SubSystem: '<S319>/Sub-Phase #4'
          }
        }

        // End of If: '<S319>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S11>/Change RED Behavior'
      }

      // End of If: '<S11>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #3: Experiment'
    } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
               Combined_Experiment_ArmOnly_P.Phase4_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 4;

      // Outputs for IfAction SubSystem: '<Root>/Phase #4:  Return Home' incorporates:
      //   ActionPort: '<S12>/Action Port'

      // If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S12>/Constant'
      //   Constant: '<S413>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_d = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_d = 0;

        // Outputs for IfAction SubSystem: '<S12>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S410>/Action Port'

        // Sum: '<S410>/Subtract' incorporates:
        //   Constant: '<S410>/Desired Px (BLACK)'
        //   DataStoreRead: '<S410>/BLACK_Px'

        Combined_Experiment_ArmOnly_B.Subtract_d =
          Combined_Experiment_ArmOnly_P.home_states_BLACK[0] -
          Combined_Experiment_ArmOnly_DW.BLACK_Px;

        // Delay: '<S420>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_dq != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lt =
            Combined_Experiment_ArmOnly_B.Subtract_d;
        }

        // Sum: '<S420>/Sum6' incorporates:
        //   Delay: '<S420>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_d -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lt;

        // If: '<S420>/if we went through a "step"' incorporates:
        //   Inport: '<S421>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S420>/Hold this value' incorporates:
          //   ActionPort: '<S421>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_fx =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S420>/Hold this value'
        }

        // End of If: '<S420>/if we went through a "step"'

        // Sum: '<S415>/Sum3' incorporates:
        //   DataStoreWrite: '<S410>/BLACK_Fx'
        //   Gain: '<S415>/kd_xb'
        //   Gain: '<S415>/kp_xb'
        //   Gain: '<S420>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_fx *
          Combined_Experiment_ArmOnly_P.Kd_xb +
          Combined_Experiment_ArmOnly_P.Kp_xb *
          Combined_Experiment_ArmOnly_B.Subtract_d;

        // Sum: '<S410>/Subtract1' incorporates:
        //   Constant: '<S410>/Desired Py (BLACK)'
        //   DataStoreRead: '<S410>/BLACK_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_d =
          Combined_Experiment_ArmOnly_P.home_states_BLACK[1] -
          Combined_Experiment_ArmOnly_DW.BLACK_Py;

        // Delay: '<S422>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_h != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_im =
            Combined_Experiment_ArmOnly_B.Subtract1_d;
        }

        // Sum: '<S422>/Sum6' incorporates:
        //   Delay: '<S422>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_d -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_im;

        // If: '<S422>/if we went through a "step"' incorporates:
        //   Inport: '<S423>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S422>/Hold this value' incorporates:
          //   ActionPort: '<S423>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_d0 =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S422>/Hold this value'
        }

        // End of If: '<S422>/if we went through a "step"'

        // Sum: '<S416>/Sum3' incorporates:
        //   DataStoreWrite: '<S410>/BLACK_Fy'
        //   Gain: '<S416>/kd_yb'
        //   Gain: '<S416>/kp_yb'
        //   Gain: '<S422>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_d0 *
          Combined_Experiment_ArmOnly_P.Kd_yb +
          Combined_Experiment_ArmOnly_P.Kp_yb *
          Combined_Experiment_ArmOnly_B.Subtract1_d;

        // MATLAB Function: '<S417>/MATLAB Function2' incorporates:
        //   Constant: '<S410>/Desired Attitude (BLACK)'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.home_states_BLACK[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_p);

        // MATLAB Function: '<S417>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S410>/BLACK_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_b);

        // Sum: '<S417>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_p.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_b.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_p.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_b.Ox[1];

        // MATLAB Function: '<S417>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_p.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_l);

        // Delay: '<S418>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_o != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_f =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_l.e_out;
        }

        // Sum: '<S418>/Sum6' incorporates:
        //   Delay: '<S418>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_l.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_f;

        // If: '<S418>/if we went through a "step"' incorporates:
        //   Inport: '<S419>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S418>/Hold this value' incorporates:
          //   ActionPort: '<S419>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_fk =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S418>/Hold this value'
        }

        // End of If: '<S418>/if we went through a "step"'

        // Sum: '<S414>/Sum3' incorporates:
        //   DataStoreWrite: '<S410>/BLACK_Tz'
        //   Gain: '<S414>/kd_tb'
        //   Gain: '<S414>/kp_tb'
        //   Gain: '<S418>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_fk *
          Combined_Experiment_ArmOnly_P.Kd_tb +
          Combined_Experiment_ArmOnly_P.Kp_tb *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_l.e_out;

        // DataStoreWrite: '<S410>/Data Store Write3' incorporates:
        //   Constant: '<S410>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_fj;

        // End of Outputs for SubSystem: '<S12>/Change BLACK Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S12>/Constant'
      //   Constant: '<S413>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jc = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jc = 0;

        // Outputs for IfAction SubSystem: '<S12>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S411>/Action Port'

        Combin_ChangeBLUEBehavior_g
          (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_gj,
           &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_gj,
           &Combined_Experiment_ArmOnly_P.ChangeBLUEBehavior_gj,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
           &Combined_Experiment_ArmOnly_DW.BLUE_Px,
           &Combined_Experiment_ArmOnly_DW.BLUE_Py,
           &Combined_Experiment_ArmOnly_DW.BLUE_Rz,
           &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
           &Combined_Experiment_ArmOnly_DW.Float_State);

        // End of Outputs for SubSystem: '<S12>/Change BLUE Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S12>/Constant'
      //   Constant: '<S413>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_a4 = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_a4 = 0;

        // Outputs for IfAction SubSystem: '<S12>/Change RED Behavior' incorporates:
        //   ActionPort: '<S412>/Action Port'

        // MATLAB Function: '<S443>/MATLAB Function2' incorporates:
        //   Constant: '<S412>/Constant'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.home_states_RED[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ia);

        // MATLAB Function: '<S443>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S412>/RED_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.RED_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c1);

        // Sum: '<S443>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ia.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c1.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ia.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_c1.Ox[1];

        // MATLAB Function: '<S443>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_ia.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_na);

        // Delay: '<S444>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_dh != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_na.e_out;
        }

        // Sum: '<S444>/Sum6' incorporates:
        //   Delay: '<S444>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_na.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i;

        // If: '<S444>/if we went through a "step"' incorporates:
        //   Inport: '<S445>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S444>/Hold this value' incorporates:
          //   ActionPort: '<S445>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_h =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S444>/Hold this value'
        }

        // End of If: '<S444>/if we went through a "step"'

        // Sum: '<S440>/Sum3' incorporates:
        //   DataStoreWrite: '<S412>/RED_Tz'
        //   Gain: '<S440>/kd_tr'
        //   Gain: '<S440>/kp_tr'
        //   Gain: '<S444>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_h *
          Combined_Experiment_ArmOnly_P.Kd_tr +
          Combined_Experiment_ArmOnly_P.Kp_tr *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_na.e_out;

        // Sum: '<S412>/Subtract' incorporates:
        //   Constant: '<S412>/Constant1'
        //   DataStoreRead: '<S412>/RED_Px'

        Combined_Experiment_ArmOnly_B.Subtract_l =
          Combined_Experiment_ArmOnly_P.home_states_RED[0] -
          Combined_Experiment_ArmOnly_DW.RED_Px;

        // Delay: '<S446>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_a2 != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_h =
            Combined_Experiment_ArmOnly_B.Subtract_l;
        }

        // Sum: '<S446>/Sum6' incorporates:
        //   Delay: '<S446>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_l -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_h;

        // If: '<S446>/if we went through a "step"' incorporates:
        //   Inport: '<S447>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S446>/Hold this value' incorporates:
          //   ActionPort: '<S447>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_b =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S446>/Hold this value'
        }

        // End of If: '<S446>/if we went through a "step"'

        // Sum: '<S441>/Sum3' incorporates:
        //   DataStoreWrite: '<S412>/RED_Fx'
        //   Gain: '<S441>/kd_xr'
        //   Gain: '<S441>/kp_xr'
        //   Gain: '<S446>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_b *
          Combined_Experiment_ArmOnly_P.Kd_xr +
          Combined_Experiment_ArmOnly_P.Kp_xr *
          Combined_Experiment_ArmOnly_B.Subtract_l;

        // DataStoreWrite: '<S412>/RED_Tz_RW1' incorporates:
        //   Constant: '<S412>/Constant2'

        Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
          Combined_Experiment_ArmOnly_P.Constant2_Value_g;

        // Sum: '<S412>/Subtract1' incorporates:
        //   Constant: '<S412>/Constant3'
        //   DataStoreRead: '<S412>/RED_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_g =
          Combined_Experiment_ArmOnly_P.home_states_RED[1] -
          Combined_Experiment_ArmOnly_DW.RED_Py;

        // Delay: '<S448>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_f != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_l =
            Combined_Experiment_ArmOnly_B.Subtract1_g;
        }

        // Sum: '<S448>/Sum6' incorporates:
        //   Delay: '<S448>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_g -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_l;

        // If: '<S448>/if we went through a "step"' incorporates:
        //   Inport: '<S449>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S448>/Hold this value' incorporates:
          //   ActionPort: '<S449>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_f =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S448>/Hold this value'
        }

        // End of If: '<S448>/if we went through a "step"'

        // Sum: '<S442>/Sum3' incorporates:
        //   DataStoreWrite: '<S412>/RED_Fy'
        //   Gain: '<S442>/kd_yr'
        //   Gain: '<S442>/kp_yr'
        //   Gain: '<S448>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_f *
          Combined_Experiment_ArmOnly_P.Kd_yr +
          Combined_Experiment_ArmOnly_P.Kp_yr *
          Combined_Experiment_ArmOnly_B.Subtract1_g;

        // DataStoreWrite: '<S412>/Data Store Write3' incorporates:
        //   Constant: '<S412>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_od;

        // End of Outputs for SubSystem: '<S12>/Change RED Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #4:  Return Home'
    } else if (Combined_Experiment_ArmOnly_DW.Univ_Time <
               Combined_Experiment_ArmOnly_P.Phase5_End) {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 5;

      // Outputs for IfAction SubSystem: '<Root>/Phase #5:  Hold Home' incorporates:
      //   ActionPort: '<S13>/Action Port'

      // If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S13>/Constant'
      //   Constant: '<S456>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_b = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_b = 0;

        // Outputs for IfAction SubSystem: '<S13>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S453>/Action Port'

        // Sum: '<S453>/Subtract' incorporates:
        //   Constant: '<S453>/Constant'
        //   DataStoreRead: '<S453>/BLACK_Px'

        Combined_Experiment_ArmOnly_B.Subtract_o =
          Combined_Experiment_ArmOnly_P.home_states_BLACK[0] -
          Combined_Experiment_ArmOnly_DW.BLACK_Px;

        // Delay: '<S463>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_e != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_o =
            Combined_Experiment_ArmOnly_B.Subtract_o;
        }

        // Sum: '<S463>/Sum6' incorporates:
        //   Delay: '<S463>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract_o -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_o;

        // If: '<S463>/if we went through a "step"' incorporates:
        //   Inport: '<S464>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S463>/Hold this value' incorporates:
          //   ActionPort: '<S464>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_i =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S463>/Hold this value'
        }

        // End of If: '<S463>/if we went through a "step"'

        // Sum: '<S458>/Sum3' incorporates:
        //   DataStoreWrite: '<S453>/BLACK_Fx'
        //   Gain: '<S458>/kd_xb'
        //   Gain: '<S458>/kp_xb'
        //   Gain: '<S463>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_i *
          Combined_Experiment_ArmOnly_P.Kd_xb +
          Combined_Experiment_ArmOnly_P.Kp_xb *
          Combined_Experiment_ArmOnly_B.Subtract_o;

        // Sum: '<S453>/Subtract1' incorporates:
        //   Constant: '<S453>/Constant2'
        //   DataStoreRead: '<S453>/BLACK_Py '

        Combined_Experiment_ArmOnly_B.Subtract1_c =
          Combined_Experiment_ArmOnly_P.home_states_BLACK[1] -
          Combined_Experiment_ArmOnly_DW.BLACK_Py;

        // Delay: '<S465>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_m != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p =
            Combined_Experiment_ArmOnly_B.Subtract1_c;
        }

        // Sum: '<S465>/Sum6' incorporates:
        //   Delay: '<S465>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1_c -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p;

        // If: '<S465>/if we went through a "step"' incorporates:
        //   Inport: '<S466>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S465>/Hold this value' incorporates:
          //   ActionPort: '<S466>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_c =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S465>/Hold this value'
        }

        // End of If: '<S465>/if we went through a "step"'

        // Sum: '<S459>/Sum3' incorporates:
        //   DataStoreWrite: '<S453>/BLACK_Fy'
        //   Gain: '<S459>/kd_yb'
        //   Gain: '<S459>/kp_yb'
        //   Gain: '<S465>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_c *
          Combined_Experiment_ArmOnly_P.Kd_yb +
          Combined_Experiment_ArmOnly_P.Kp_yb *
          Combined_Experiment_ArmOnly_B.Subtract1_c;

        // MATLAB Function: '<S460>/MATLAB Function2' incorporates:
        //   Constant: '<S453>/Constant3'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.home_states_BLACK[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_gh);

        // MATLAB Function: '<S460>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S453>/BLACK_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_o);

        // Sum: '<S460>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_gh.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_o.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_gh.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_o.Ox[1];

        // MATLAB Function: '<S460>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_gh.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_de);

        // Delay: '<S461>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_d != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_de.e_out;
        }

        // Sum: '<S461>/Sum6' incorporates:
        //   Delay: '<S461>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_de.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b;

        // If: '<S461>/if we went through a "step"' incorporates:
        //   Inport: '<S462>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S461>/Hold this value' incorporates:
          //   ActionPort: '<S462>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_g =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S461>/Hold this value'
        }

        // End of If: '<S461>/if we went through a "step"'

        // Sum: '<S457>/Sum3' incorporates:
        //   DataStoreWrite: '<S453>/BLACK_Tz'
        //   Gain: '<S457>/kd_tb'
        //   Gain: '<S457>/kp_tb'
        //   Gain: '<S461>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.BLACK_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_g *
          Combined_Experiment_ArmOnly_P.Kd_tb +
          Combined_Experiment_ArmOnly_P.Kp_tb *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_de.e_out;

        // DataStoreWrite: '<S453>/Data Store Write3' incorporates:
        //   Constant: '<S453>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_g;

        // End of Outputs for SubSystem: '<S13>/Change BLACK Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S13>/Constant'
      //   Constant: '<S456>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_n = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_n = 0;

        // Outputs for IfAction SubSystem: '<S13>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S454>/Action Port'

        Combin_ChangeBLUEBehavior_g
          (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_b,
           &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_b,
           &Combined_Experiment_ArmOnly_P.ChangeBLUEBehavior_b,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
           &Combined_Experiment_ArmOnly_DW.BLUE_Px,
           &Combined_Experiment_ArmOnly_DW.BLUE_Py,
           &Combined_Experiment_ArmOnly_DW.BLUE_Rz,
           &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
           &Combined_Experiment_ArmOnly_DW.Float_State);

        // End of Outputs for SubSystem: '<S13>/Change BLUE Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S13>/Constant'
      //   Constant: '<S456>/Constant'

      Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_j = -1;
      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_j = 0;

        // Outputs for IfAction SubSystem: '<S13>/Change RED Behavior' incorporates:
        //   ActionPort: '<S455>/Action Port'

        // Sum: '<S455>/Subtract' incorporates:
        //   Constant: '<S455>/Constant'
        //   DataStoreRead: '<S455>/RED_Px'

        Combined_Experiment_ArmOnly_B.Subtract =
          Combined_Experiment_ArmOnly_P.home_states_RED[0] -
          Combined_Experiment_ArmOnly_DW.RED_Px;

        // Delay: '<S489>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE =
            Combined_Experiment_ArmOnly_B.Subtract;
        }

        // Sum: '<S489>/Sum6' incorporates:
        //   Delay: '<S489>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE;

        // If: '<S489>/if we went through a "step"' incorporates:
        //   Inport: '<S490>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S489>/Hold this value' incorporates:
          //   ActionPort: '<S490>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_j =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S489>/Hold this value'
        }

        // End of If: '<S489>/if we went through a "step"'

        // Sum: '<S484>/Sum3' incorporates:
        //   DataStoreWrite: '<S455>/RED_Fx'
        //   Gain: '<S484>/kd_xr'
        //   Gain: '<S484>/kp_xr'
        //   Gain: '<S489>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_j *
          Combined_Experiment_ArmOnly_P.Kd_xr +
          Combined_Experiment_ArmOnly_P.Kp_xr *
          Combined_Experiment_ArmOnly_B.Subtract;

        // DataStoreWrite: '<S455>/RED_Tz_RW' incorporates:
        //   Constant: '<S455>/Constant1'

        Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
          Combined_Experiment_ArmOnly_P.Constant1_Value_k;

        // Sum: '<S455>/Subtract1' incorporates:
        //   Constant: '<S455>/Constant2'
        //   DataStoreRead: '<S455>/RED_Py '

        Combined_Experiment_ArmOnly_B.Subtract1 =
          Combined_Experiment_ArmOnly_P.home_states_RED[1] -
          Combined_Experiment_ArmOnly_DW.RED_Py;

        // Delay: '<S491>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_j != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m =
            Combined_Experiment_ArmOnly_B.Subtract1;
        }

        // Sum: '<S491>/Sum6' incorporates:
        //   Delay: '<S491>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Subtract1 -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m;

        // If: '<S491>/if we went through a "step"' incorporates:
        //   Inport: '<S492>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S491>/Hold this value' incorporates:
          //   ActionPort: '<S492>/Action Port'

          Combined_Experiment_ArmOnly_B.In1 =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S491>/Hold this value'
        }

        // End of If: '<S491>/if we went through a "step"'

        // Sum: '<S485>/Sum3' incorporates:
        //   DataStoreWrite: '<S455>/RED_Fy'
        //   Gain: '<S485>/kd_yr'
        //   Gain: '<S485>/kp_yr'
        //   Gain: '<S491>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Fy = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1 *
          Combined_Experiment_ArmOnly_P.Kd_yr +
          Combined_Experiment_ArmOnly_P.Kp_yr *
          Combined_Experiment_ArmOnly_B.Subtract1;

        // MATLAB Function: '<S486>/MATLAB Function2' incorporates:
        //   Constant: '<S455>/Constant3'

        Combined__MATLABFunction2_o
          (Combined_Experiment_ArmOnly_P.home_states_RED[2],
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_mx);

        // MATLAB Function: '<S486>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S455>/RED_Rz'

        Combined_Ex_MATLABFunction3(Combined_Experiment_ArmOnly_DW.RED_Rz,
          &Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_bc);

        // Sum: '<S486>/Subtract2'
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[0] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_mx.Ox[0] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_bc.Ox[0];
        Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b[1] =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_mx.Ox[1] -
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction3_bc.Ox[1];

        // MATLAB Function: '<S486>/MATLAB Function4'
        Combined_Ex_MATLABFunction4
          (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_mx.Oy,
           Combined_Experiment_ArmOnly_B.rtb_Subtract2_f_b,
           &Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_df);

        // Delay: '<S487>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_a != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_df.e_out;
        }

        // Sum: '<S487>/Sum6' incorporates:
        //   Delay: '<S487>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_df.e_out -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n;

        // If: '<S487>/if we went through a "step"' incorporates:
        //   Inport: '<S488>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S487>/Hold this value' incorporates:
          //   ActionPort: '<S488>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_d =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S487>/Hold this value'
        }

        // End of If: '<S487>/if we went through a "step"'

        // Sum: '<S483>/Sum3' incorporates:
        //   DataStoreWrite: '<S455>/RED_Tz'
        //   Gain: '<S483>/kd_tr'
        //   Gain: '<S483>/kp_tr'
        //   Gain: '<S487>/divide by delta T'

        Combined_Experiment_ArmOnly_DW.RED_Tz = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_d *
          Combined_Experiment_ArmOnly_P.Kd_tr +
          Combined_Experiment_ArmOnly_P.Kp_tr *
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_df.e_out;

        // DataStoreWrite: '<S455>/Data Store Write3' incorporates:
        //   Constant: '<S455>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_es;

        // End of Outputs for SubSystem: '<S13>/Change RED Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #5:  Hold Home'
    } else {
      Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = 6;

      // Outputs for IfAction SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' incorporates:
      //   ActionPort: '<S14>/Action Port'

      // If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S14>/Constant'
      //   Constant: '<S499>/Constant'

      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S496>/Action Port'

        // DataStoreWrite: '<S496>/BLACK_Fx' incorporates:
        //   Constant: '<S496>/Constant3'

        Combined_Experiment_ArmOnly_DW.BLACK_Fx =
          Combined_Experiment_ArmOnly_P.Constant3_Value;

        // DataStoreWrite: '<S496>/BLACK_Fy' incorporates:
        //   Constant: '<S496>/Constant4'

        Combined_Experiment_ArmOnly_DW.BLACK_Fy =
          Combined_Experiment_ArmOnly_P.Constant4_Value_e;

        // DataStoreWrite: '<S496>/BLACK_Tz' incorporates:
        //   Constant: '<S496>/Constant5'

        Combined_Experiment_ArmOnly_DW.BLACK_Tz =
          Combined_Experiment_ArmOnly_P.Constant5_Value_n;

        // DataStoreWrite: '<S496>/Data Store Write3' incorporates:
        //   Constant: '<S496>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_gd;

        // End of Outputs for SubSystem: '<S14>/Change BLACK Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S14>/Constant'
      //   Constant: '<S499>/Constant'

      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S497>/Action Port'

        Combined_ChangeBLUEBehavior
          (&Combined_Experiment_ArmOnly_P.ChangeBLUEBehavior_k,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fx,
           &Combined_Experiment_ArmOnly_DW.BLUE_Fy,
           &Combined_Experiment_ArmOnly_DW.BLUE_Tz,
           &Combined_Experiment_ArmOnly_DW.Float_State);

        // End of Outputs for SubSystem: '<S14>/Change BLUE Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S14>/Constant'
      //   Constant: '<S499>/Constant'

      if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
          (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change RED Behavior' incorporates:
        //   ActionPort: '<S498>/Action Port'

        // DataStoreWrite: '<S498>/RED_Fx' incorporates:
        //   Constant: '<S498>/Constant'

        Combined_Experiment_ArmOnly_DW.RED_Fx =
          Combined_Experiment_ArmOnly_P.Constant_Value_j;

        // DataStoreWrite: '<S498>/RED_Fy' incorporates:
        //   Constant: '<S498>/Constant1'

        Combined_Experiment_ArmOnly_DW.RED_Fy =
          Combined_Experiment_ArmOnly_P.Constant1_Value_l;

        // DataStoreWrite: '<S498>/RED_Tz' incorporates:
        //   Constant: '<S498>/Constant2'

        Combined_Experiment_ArmOnly_DW.RED_Tz =
          Combined_Experiment_ArmOnly_P.Constant2_Value_m;

        // DataStoreWrite: '<S498>/Data Store Write3' incorporates:
        //   Constant: '<S498>/Puck State'

        Combined_Experiment_ArmOnly_DW.Float_State =
          Combined_Experiment_ArmOnly_P.PuckState_Value_h;

        // Gain: '<S498>/Gain' incorporates:
        //   DataStoreRead: '<S498>/RED_dRz_Speed'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_P.Gain_Gain_a3 *
          Combined_Experiment_ArmOnly_DW.RED_dRz_RW_Sat;

        // Saturate: '<S498>/Saturation'
        if (Combined_Experiment_ArmOnly_B.Sum6_l1 >
            Combined_Experiment_ArmOnly_P.Saturation_UpperSat) {
          Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
            Combined_Experiment_ArmOnly_P.Saturation_UpperSat;
        } else if (Combined_Experiment_ArmOnly_B.Sum6_l1 <
                   Combined_Experiment_ArmOnly_P.Saturation_LowerSat) {
          Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
            Combined_Experiment_ArmOnly_P.Saturation_LowerSat;
        } else {
          Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
            Combined_Experiment_ArmOnly_B.Sum6_l1;
        }

        // End of Saturate: '<S498>/Saturation'
        // End of Outputs for SubSystem: '<S14>/Change RED Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' 
    }

    // End of If: '<Root>/Separate Phases'

    // If: '<Root>/If running a simulation,  grab the simulated states.' incorporates:
    //   Constant: '<S7>/Constant'
    //   DiscreteIntegrator: '<S500>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S501>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S505>/Acceleration  to Velocity'
    //   Inport: '<S528>/In'
    //   Inport: '<S530>/In'
    //   Inport: '<S532>/In'
    //   RandomNumber: '<S500>/Random Number'
    //   RandomNumber: '<S501>/Random Number'
    //   RandomNumber: '<S505>/Random Number'
    //   Sum: '<S500>/Sum'
    //   Sum: '<S501>/Sum'
    //   Sum: '<S505>/Sum'

    if (Combined_Experiment_ArmOnly_P.simMode == 1.0) {
      // Outputs for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' incorporates:
      //   ActionPort: '<S16>/Action Port'

      // DiscreteIntegrator: '<S500>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLACK_Fx_Sat'
      //   MATLAB Function: '<S500>/MATLAB Function'
      //   SignalConversion generated from: '<S524>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLACK Dynamics Model/MATLAB Function': '<S524>:1' 
      // '<S524>:1:5' x_ddot     = zeros(3,1);
      // '<S524>:1:9' m_BLACK    = model_param(3);
      // '<S524>:1:10' I_BLACK    = model_param(4);
      // '<S524>:1:13' Fx        = control_inputs(1);
      // '<S524>:1:14' Fy        = control_inputs(2);
      // '<S524>:1:15' Tz        = control_inputs(3);
      // '<S524>:1:18' x_ddot(1) = Fx/m_BLACK;
      // '<S524>:1:19' x_ddot(2) = Fy/m_BLACK;
      // '<S524>:1:20' x_ddot(3) = Tz/I_BLACK;
      Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
        Combined_Experiment_ArmOnly_DW.BLACK_Fx_Sat /
        Combined_Experiment_ArmOnly_P.model_param[2] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainval +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[0];

      // DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval *
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[0];
      Combined_Experiment_ArmOnly_B.x_ddot_a[0] =
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;

      // DiscreteIntegrator: '<S500>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLACK_Fy_Sat'
      //   MATLAB Function: '<S500>/MATLAB Function'
      //   SignalConversion generated from: '<S524>/ SFunction '

      Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
        Combined_Experiment_ArmOnly_DW.BLACK_Fy_Sat /
        Combined_Experiment_ArmOnly_P.model_param[2] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainval +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[1];

      // DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval *
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[1];
      Combined_Experiment_ArmOnly_B.x_ddot_a[1] =
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;

      // DiscreteIntegrator: '<S500>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLACK_Tz_Sat'
      //   MATLAB Function: '<S500>/MATLAB Function'
      //   SignalConversion generated from: '<S524>/ SFunction '

      Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
        Combined_Experiment_ArmOnly_DW.BLACK_Tz_Sat /
        Combined_Experiment_ArmOnly_P.model_param[3] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainval +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[2];

      // DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[2] =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval *
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[2];

      // MATLAB Function: '<S506>/MATLAB Function' incorporates:
      //   Constant: '<S506>/Constant1'
      //   DataStoreRead: '<S506>/Data Store Read2'

      Combined_E_MATLABFunction_f(Combined_Experiment_ArmOnly_DW.Univ_Time,
        Combined_Experiment_ArmOnly_P.serverRate,
        &Combined_Experiment_ArmOnly_B.sf_MATLABFunction_fm);

      // Outputs for Triggered SubSystem: '<S506>/Sample and Hold1' incorporates:
      //   TriggerPort: '<S528>/Trigger'

      Combined_Experiment_ArmOnly_B.zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
        &Combined_Experiment_Arm_PrevZCX.SampleandHold1_Trig_ZCE_c,
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction_fm.y));
      if (Combined_Experiment_ArmOnly_B.zcEvent != NO_ZCEVENT) {
        Combined_Experiment_ArmOnly_B.In_l[0] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] +
          Combined_Experiment_ArmOnly_DW.NextOutput;
        Combined_Experiment_ArmOnly_B.In_l[1] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] +
          Combined_Experiment_ArmOnly_DW.NextOutput;
        Combined_Experiment_ArmOnly_B.In_l[2] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition[2] +
          Combined_Experiment_ArmOnly_DW.NextOutput;
      }

      // End of Outputs for SubSystem: '<S506>/Sample and Hold1'

      // Delay: '<S518>/Delay1' incorporates:
      //   Inport: '<S528>/In'
      //   RandomNumber: '<S500>/Random Number'
      //   Sum: '<S500>/Sum'

      if (Combined_Experiment_ArmOnly_DW.icLoad_lm != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ff =
          Combined_Experiment_ArmOnly_B.In_l[0];
      }

      // Sum: '<S518>/Sum6' incorporates:
      //   Delay: '<S518>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_l[0] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ff;

      // If: '<S518>/if we went through a "step"' incorporates:
      //   Inport: '<S542>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S518>/Hold this value' incorporates:
        //   ActionPort: '<S542>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_gi =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S518>/Hold this value'
      }

      // End of If: '<S518>/if we went through a "step"'

      // Gain: '<S518>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vx'

      Combined_Experiment_ArmOnly_DW.BLACK_Vx = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_gi;

      // Delay: '<S512>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vx'

      if (Combined_Experiment_ArmOnly_DW.icLoad_em != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p5 =
          Combined_Experiment_ArmOnly_DW.BLACK_Vx;
      }

      // Sum: '<S512>/Sum6' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vx'
      //   Delay: '<S512>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_DW.BLACK_Vx -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p5;

      // If: '<S512>/if we went through a "step"' incorporates:
      //   Inport: '<S536>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S512>/Hold this value' incorporates:
        //   ActionPort: '<S536>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_k0 =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S512>/Hold this value'
      }

      // End of If: '<S512>/if we went through a "step"'

      // Sum: '<S16>/Sum1' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_IMU_Ax_I'
      //   Gain: '<S512>/divide by delta T'
      //   RandomNumber: '<S16>/Random Number1'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_I = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_k0 +
        Combined_Experiment_ArmOnly_DW.NextOutput_g;

      // Delay: '<S519>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_nw != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_e =
          Combined_Experiment_ArmOnly_B.In_l[1];
      }

      // Sum: '<S519>/Sum6' incorporates:
      //   Delay: '<S519>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_l[1] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_e;

      // If: '<S519>/if we went through a "step"' incorporates:
      //   Inport: '<S543>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S519>/Hold this value' incorporates:
        //   ActionPort: '<S543>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_fu =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S519>/Hold this value'
      }

      // End of If: '<S519>/if we went through a "step"'

      // Gain: '<S519>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vy'

      Combined_Experiment_ArmOnly_DW.BLACK_Vy = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_fu;

      // Delay: '<S513>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vy'

      if (Combined_Experiment_ArmOnly_DW.icLoad_og != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cb =
          Combined_Experiment_ArmOnly_DW.BLACK_Vy;
      }

      // Sum: '<S513>/Sum6' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vy'
      //   Delay: '<S513>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_DW.BLACK_Vy -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cb;

      // If: '<S513>/if we went through a "step"' incorporates:
      //   Inport: '<S537>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S513>/Hold this value' incorporates:
        //   ActionPort: '<S537>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_fw =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S513>/Hold this value'
      }

      // End of If: '<S513>/if we went through a "step"'

      // Sum: '<S16>/Sum2' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_IMU_Ay_I'
      //   Gain: '<S513>/divide by delta T'
      //   RandomNumber: '<S16>/Random Number'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_I = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_fw +
        Combined_Experiment_ArmOnly_DW.NextOutput_h;

      // Delay: '<S520>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_ka != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ew =
          Combined_Experiment_ArmOnly_B.In_l[2];
      }

      // Sum: '<S520>/Sum6' incorporates:
      //   Delay: '<S520>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_l[2] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ew;

      // If: '<S520>/if we went through a "step"' incorporates:
      //   Inport: '<S544>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S520>/Hold this value' incorporates:
        //   ActionPort: '<S544>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_m =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S520>/Hold this value'
      }

      // End of If: '<S520>/if we went through a "step"'

      // Gain: '<S520>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_RzD'

      Combined_Experiment_ArmOnly_DW.BLACK_RzD = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_m;

      // Sum: '<S16>/Sum3' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_IMU_R'
      //   DataStoreWrite: '<S16>/BLACK_RzD'
      //   RandomNumber: '<S16>/Random Number2'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_R =
        Combined_Experiment_ArmOnly_DW.BLACK_RzD +
        Combined_Experiment_ArmOnly_DW.NextOutput_e;

      // DataStoreWrite: '<S16>/BLACK_Px'
      Combined_Experiment_ArmOnly_DW.BLACK_Px =
        Combined_Experiment_ArmOnly_B.In_l[0];

      // DataStoreWrite: '<S16>/BLACK_Py'
      Combined_Experiment_ArmOnly_DW.BLACK_Py =
        Combined_Experiment_ArmOnly_B.In_l[1];

      // DataStoreWrite: '<S16>/BLACK_Rz'
      Combined_Experiment_ArmOnly_DW.BLACK_Rz =
        Combined_Experiment_ArmOnly_B.In_l[2];

      // DiscreteIntegrator: '<S501>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLUE_Fx_Sat '
      //   MATLAB Function: '<S501>/MATLAB Function'
      //   SignalConversion generated from: '<S525>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLUE  Dynamics Model/MATLAB Function': '<S525>:1' 
      // '<S525>:1:5' x_ddot     = zeros(3,1);
      // '<S525>:1:9' m_BLUE    = model_param(5);
      // '<S525>:1:10' I_BLUE    = model_param(6);
      // '<S525>:1:13' Fx        = control_inputs(1);
      // '<S525>:1:14' Fy        = control_inputs(2);
      // '<S525>:1:15' Tz        = control_inputs(3);
      // '<S525>:1:18' x_ddot(1) = Fx/m_BLUE;
      // '<S525>:1:19' x_ddot(2) = Fy/m_BLUE;
      // '<S525>:1:20' x_ddot(3) = Tz/I_BLUE;
      Combined_Experiment_ArmOnly_B.y5 =
        Combined_Experiment_ArmOnly_DW.BLUE_Fx_Sat /
        Combined_Experiment_ArmOnly_P.model_param[4] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_e +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[0];

      // DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0 =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_n *
        Combined_Experiment_ArmOnly_B.y5 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[0];
      Combined_Experiment_ArmOnly_B.x_ddot_m[0] =
        Combined_Experiment_ArmOnly_B.y5;

      // DiscreteIntegrator: '<S501>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLUE_Fy_Sat'
      //   MATLAB Function: '<S501>/MATLAB Function'
      //   SignalConversion generated from: '<S525>/ SFunction '

      Combined_Experiment_ArmOnly_B.y5 =
        Combined_Experiment_ArmOnly_DW.BLUE_Fy_Sat /
        Combined_Experiment_ArmOnly_P.model_param[4] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_e +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[1];

      // DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1 =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_n *
        Combined_Experiment_ArmOnly_B.y5 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[1];
      Combined_Experiment_ArmOnly_B.x_ddot_m[1] =
        Combined_Experiment_ArmOnly_B.y5;

      // DiscreteIntegrator: '<S501>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/BLUE_Tz_Sat'
      //   MATLAB Function: '<S501>/MATLAB Function'
      //   SignalConversion generated from: '<S525>/ SFunction '

      Combined_Experiment_ArmOnly_B.y5 =
        Combined_Experiment_ArmOnly_DW.BLUE_Tz_Sat /
        Combined_Experiment_ArmOnly_P.model_param[5] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_e +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[2];

      // DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2 =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_n *
        Combined_Experiment_ArmOnly_B.y5 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[2];

      // MATLAB Function: '<S507>/MATLAB Function' incorporates:
      //   Constant: '<S507>/Constant1'
      //   DataStoreRead: '<S507>/Data Store Read2'

      Combined_E_MATLABFunction_f(Combined_Experiment_ArmOnly_DW.Univ_Time,
        Combined_Experiment_ArmOnly_P.serverRate,
        &Combined_Experiment_ArmOnly_B.sf_MATLABFunction_g);

      // Outputs for Triggered SubSystem: '<S507>/Sample and Hold1' incorporates:
      //   TriggerPort: '<S530>/Trigger'

      Combined_Experiment_ArmOnly_B.zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
        &Combined_Experiment_Arm_PrevZCX.SampleandHold1_Trig_ZCE,
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction_g.y));
      if (Combined_Experiment_ArmOnly_B.zcEvent != NO_ZCEVENT) {
        Combined_Experiment_ArmOnly_B.In_k[0] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0 +
          Combined_Experiment_ArmOnly_DW.NextOutput_m;
        Combined_Experiment_ArmOnly_B.In_k[1] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1 +
          Combined_Experiment_ArmOnly_DW.NextOutput_m;
        Combined_Experiment_ArmOnly_B.In_k[2] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2 +
          Combined_Experiment_ArmOnly_DW.NextOutput_m;
      }

      // End of Outputs for SubSystem: '<S507>/Sample and Hold1'

      // Delay: '<S521>/Delay1' incorporates:
      //   Inport: '<S530>/In'
      //   RandomNumber: '<S501>/Random Number'
      //   Sum: '<S501>/Sum'

      if (Combined_Experiment_ArmOnly_DW.icLoad_lf != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kn =
          Combined_Experiment_ArmOnly_B.In_k[0];
      }

      // Sum: '<S521>/Sum6' incorporates:
      //   Delay: '<S521>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_k[0] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kn;

      // If: '<S521>/if we went through a "step"' incorporates:
      //   Inport: '<S545>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S521>/Hold this value' incorporates:
        //   ActionPort: '<S545>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_b0 =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S521>/Hold this value'
      }

      // End of If: '<S521>/if we went through a "step"'

      // Gain: '<S521>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vx'

      Combined_Experiment_ArmOnly_DW.BLUE_Vx = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_b0;

      // Delay: '<S514>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vx'

      if (Combined_Experiment_ArmOnly_DW.icLoad_aq != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gc =
          Combined_Experiment_ArmOnly_DW.BLUE_Vx;
      }

      // End of Delay: '<S514>/Delay1'

      // Delay: '<S522>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_nc != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gu =
          Combined_Experiment_ArmOnly_B.In_k[1];
      }

      // Sum: '<S522>/Sum6' incorporates:
      //   Delay: '<S522>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_k[1] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gu;

      // If: '<S522>/if we went through a "step"' incorporates:
      //   Inport: '<S546>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S522>/Hold this value' incorporates:
        //   ActionPort: '<S546>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_hg =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S522>/Hold this value'
      }

      // End of If: '<S522>/if we went through a "step"'

      // Gain: '<S522>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vy'

      Combined_Experiment_ArmOnly_DW.BLUE_Vy = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_hg;

      // Delay: '<S515>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vy'

      if (Combined_Experiment_ArmOnly_DW.icLoad_kx != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_a =
          Combined_Experiment_ArmOnly_DW.BLUE_Vy;
      }

      // End of Delay: '<S515>/Delay1'

      // Delay: '<S523>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_fa != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_dd =
          Combined_Experiment_ArmOnly_B.In_k[2];
      }

      // Sum: '<S523>/Sum6' incorporates:
      //   Delay: '<S523>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.In_k[2] -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_dd;

      // If: '<S523>/if we went through a "step"' incorporates:
      //   Inport: '<S547>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
        // Outputs for IfAction SubSystem: '<S523>/Hold this value' incorporates:
        //   ActionPort: '<S547>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_n0 =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of Outputs for SubSystem: '<S523>/Hold this value'
      }

      // End of If: '<S523>/if we went through a "step"'

      // Gain: '<S523>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_RzD'

      Combined_Experiment_ArmOnly_DW.BLUE_RzD = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_n0;

      // Sum: '<S16>/Sum6' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_IMU_R'
      //   DataStoreWrite: '<S16>/BLUE_RzD'
      //   RandomNumber: '<S16>/Random Number5'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_DW.BLUE_RzD +
        Combined_Experiment_ArmOnly_DW.NextOutput_f;

      // DataStoreWrite: '<S16>/BLUE_Px '
      Combined_Experiment_ArmOnly_DW.BLUE_Px =
        Combined_Experiment_ArmOnly_B.In_k[0];

      // DataStoreWrite: '<S16>/BLUE_Py'
      Combined_Experiment_ArmOnly_DW.BLUE_Py =
        Combined_Experiment_ArmOnly_B.In_k[1];

      // DataStoreWrite: '<S16>/BLUE_Rz'
      Combined_Experiment_ArmOnly_DW.BLUE_Rz =
        Combined_Experiment_ArmOnly_B.In_k[2];

      // RelationalOperator: '<S502>/Compare' incorporates:
      //   Constant: '<S502>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read1'

      Combined_Experiment_ArmOnly_B.rtb_Compare_me =
        (Combined_Experiment_ArmOnly_DW.BLUE_Rz ==
         Combined_Experiment_ArmOnly_P.Constant_Value_l);

      // DiscreteIntegrator: '<S16>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S16>/Data Store Read1'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_IC_LOADI != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_DSTATE =
          Combined_Experiment_ArmOnly_DW.BLUE_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_me &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_PrevRese <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_me) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_PrevRese ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_DSTATE =
          Combined_Experiment_ArmOnly_DW.BLUE_Rz;
      }

      // RelationalOperator: '<S503>/Compare' incorporates:
      //   Constant: '<S503>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read2'

      Combined_Experiment_ArmOnly_B.rtb_Compare_m_g =
        (Combined_Experiment_ArmOnly_DW.BLACK_Rz ==
         Combined_Experiment_ArmOnly_P.Constant_Value_b);

      // DiscreteIntegrator: '<S16>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S16>/Data Store Read2'
      //   DataStoreRead: '<S16>/Data Store Read4'
      //   DataStoreWrite: '<S16>/RED_Px1'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LOAD != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTATE =
          Combined_Experiment_ArmOnly_DW.BLACK_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_m_g &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevRes <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_m_g) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevRes ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTATE =
          Combined_Experiment_ArmOnly_DW.BLACK_Rz;
      }

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi =
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator1_gainv_c *
        Combined_Experiment_ArmOnly_DW.BLACK_IMU_R +
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTATE;

      // End of DiscreteIntegrator: '<S16>/Discrete-Time Integrator1'

      // RelationalOperator: '<S504>/Compare' incorporates:
      //   Constant: '<S504>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read5'

      Combined_Experiment_ArmOnly_B.rtb_Compare_g_g =
        (Combined_Experiment_ArmOnly_DW.RED_Rz ==
         Combined_Experiment_ArmOnly_P.Constant_Value_hs);

      // DiscreteIntegrator: '<S16>/Discrete-Time Integrator2' incorporates:
      //   DataStoreRead: '<S16>/Data Store Read5'
      //   DataStoreRead: '<S16>/Data Store Read6'
      //   DataStoreWrite: '<S16>/RED_Px2'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_IC_LOAD != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_DSTATE =
          Combined_Experiment_ArmOnly_DW.RED_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_g_g &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_PrevRes <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_g_g) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_PrevRes ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_DSTATE =
          Combined_Experiment_ArmOnly_DW.RED_Rz;
      }

      Combined_Experiment_ArmOnly_DW.RED_IMU_Psi =
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator2_gainval *
        Combined_Experiment_ArmOnly_DW.RED_IMU_R +
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_DSTATE;

      // End of DiscreteIntegrator: '<S16>/Discrete-Time Integrator2'

      // DiscreteIntegrator: '<S505>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/RED_Fx_Sat'
      //   MATLAB Function: '<S505>/MATLAB Function'
      //   SignalConversion generated from: '<S526>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/RED Dynamics Model/MATLAB Function': '<S526>:1' 
      // '<S526>:1:5' x_ddot     = zeros(3,1);
      // '<S526>:1:9' m_RED    = model_param(1);
      // '<S526>:1:10' I_RED    = model_param(2);
      // '<S526>:1:13' Fx        = control_inputs(1);
      // '<S526>:1:14' Fy        = control_inputs(2);
      // '<S526>:1:15' Tz        = control_inputs(3);
      // '<S526>:1:18' x_ddot(1) = Fx/m_RED;
      // '<S526>:1:19' x_ddot(2) = Fy/m_RED;
      // '<S526>:1:20' x_ddot(3) = Tz/I_RED;
      Combined_Experiment_ArmOnly_B.y10 =
        Combined_Experiment_ArmOnly_DW.RED_Fx_Sat /
        Combined_Experiment_ArmOnly_P.model_param[0] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_n +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[0];

      // DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_p *
        Combined_Experiment_ArmOnly_B.y10 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[0];
      Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_0 =
        Combined_Experiment_ArmOnly_B.y10;

      // DiscreteIntegrator: '<S505>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/RED_Fy_Sat'
      //   MATLAB Function: '<S505>/MATLAB Function'
      //   SignalConversion generated from: '<S526>/ SFunction '

      Combined_Experiment_ArmOnly_B.y10 =
        Combined_Experiment_ArmOnly_DW.RED_Fy_Sat /
        Combined_Experiment_ArmOnly_P.model_param[0] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_n +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[1];

      // DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_p *
        Combined_Experiment_ArmOnly_B.y10 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[1];
      Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_1 =
        Combined_Experiment_ArmOnly_B.y10;

      // DiscreteIntegrator: '<S505>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S16>/Data Store Read'
      //   DataStoreRead: '<S16>/RED_Tz_Sat'
      //   MATLAB Function: '<S505>/MATLAB Function'
      //   Sum: '<S16>/Sum'

      Combined_Experiment_ArmOnly_B.y10 =
        (Combined_Experiment_ArmOnly_DW.RED_Tz_Sat +
         Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat) /
        Combined_Experiment_ArmOnly_P.model_param[1] *
        Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_gainva_n +
        Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[2];

      // DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_f =
        Combined_Experiment_ArmOnly_P.VelocitytoPosition_gainval_p *
        Combined_Experiment_ArmOnly_B.y10 +
        Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[2];

      // MATLAB Function: '<S508>/MATLAB Function' incorporates:
      //   Constant: '<S508>/Constant'
      //   DataStoreRead: '<S508>/Data Store Read1'

      Combined_E_MATLABFunction_f(Combined_Experiment_ArmOnly_DW.Univ_Time,
        Combined_Experiment_ArmOnly_P.serverRate,
        &Combined_Experiment_ArmOnly_B.sf_MATLABFunction_d);

      // Outputs for Triggered SubSystem: '<S508>/Sample and Hold' incorporates:
      //   TriggerPort: '<S532>/Trigger'

      Combined_Experiment_ArmOnly_B.zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
        &Combined_Experiment_Arm_PrevZCX.SampleandHold_Trig_ZCE,
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction_d.y));
      if (Combined_Experiment_ArmOnly_B.zcEvent != NO_ZCEVENT) {
        Combined_Experiment_ArmOnly_B.In[0] =
          Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p +
          Combined_Experiment_ArmOnly_DW.NextOutput_l;
        Combined_Experiment_ArmOnly_B.In[1] =
          Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c +
          Combined_Experiment_ArmOnly_DW.NextOutput_l;
        Combined_Experiment_ArmOnly_B.In[2] =
          Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_f +
          Combined_Experiment_ArmOnly_DW.NextOutput_l;
      }

      // End of Outputs for SubSystem: '<S508>/Sample and Hold'

      // Delay: '<S509>/Delay1' incorporates:
      //   Inport: '<S532>/In'
      //   RandomNumber: '<S505>/Random Number'
      //   Sum: '<S505>/Sum'

      if (Combined_Experiment_ArmOnly_DW.icLoad_ky != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ol =
          Combined_Experiment_ArmOnly_B.In[0];
      }

      // Sum: '<S509>/Sum6' incorporates:
      //   Delay: '<S509>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_dk = Combined_Experiment_ArmOnly_B.In[0]
        - Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ol;

      // If: '<S509>/if we went through a "step"' incorporates:
      //   Inport: '<S533>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
        // Outputs for IfAction SubSystem: '<S509>/Hold this value' incorporates:
        //   ActionPort: '<S533>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_p =
          Combined_Experiment_ArmOnly_B.Sum6_dk;

        // End of Outputs for SubSystem: '<S509>/Hold this value'
      }

      // End of If: '<S509>/if we went through a "step"'

      // Gain: '<S509>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vx'

      Combined_Experiment_ArmOnly_DW.RED_Vx = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_p;

      // Delay: '<S510>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vx'

      if (Combined_Experiment_ArmOnly_DW.icLoad_eg != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pm =
          Combined_Experiment_ArmOnly_DW.RED_Vx;
      }

      // Sum: '<S510>/Sum6' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vx'
      //   Delay: '<S510>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_dk =
        Combined_Experiment_ArmOnly_DW.RED_Vx -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pm;

      // If: '<S510>/if we went through a "step"' incorporates:
      //   Inport: '<S534>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
        // Outputs for IfAction SubSystem: '<S510>/Hold this value' incorporates:
        //   ActionPort: '<S534>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_kd =
          Combined_Experiment_ArmOnly_B.Sum6_dk;

        // End of Outputs for SubSystem: '<S510>/Hold this value'
      }

      // End of If: '<S510>/if we went through a "step"'

      // Sum: '<S16>/Sum7' incorporates:
      //   DataStoreWrite: '<S16>/RED_IMU_Ay'
      //   Gain: '<S510>/divide by delta T'
      //   RandomNumber: '<S16>/Random Number7'

      Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_I = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_kd +
        Combined_Experiment_ArmOnly_DW.NextOutput_n;

      // Delay: '<S516>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_h5 != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_a5 =
          Combined_Experiment_ArmOnly_B.In[1];
      }

      // Sum: '<S516>/Sum6' incorporates:
      //   Delay: '<S516>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_dk = Combined_Experiment_ArmOnly_B.In[1]
        - Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_a5;

      // If: '<S516>/if we went through a "step"' incorporates:
      //   Inport: '<S540>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
        // Outputs for IfAction SubSystem: '<S516>/Hold this value' incorporates:
        //   ActionPort: '<S540>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_dl =
          Combined_Experiment_ArmOnly_B.Sum6_dk;

        // End of Outputs for SubSystem: '<S516>/Hold this value'
      }

      // End of If: '<S516>/if we went through a "step"'

      // Gain: '<S516>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vy'

      Combined_Experiment_ArmOnly_DW.RED_Vy = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_dl;

      // Delay: '<S511>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vy'

      if (Combined_Experiment_ArmOnly_DW.icLoad_p != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c3 =
          Combined_Experiment_ArmOnly_DW.RED_Vy;
      }

      // Sum: '<S511>/Sum6' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vy'
      //   Delay: '<S511>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_dk =
        Combined_Experiment_ArmOnly_DW.RED_Vy -
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c3;

      // If: '<S511>/if we went through a "step"' incorporates:
      //   Inport: '<S535>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
        // Outputs for IfAction SubSystem: '<S511>/Hold this value' incorporates:
        //   ActionPort: '<S535>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_ed =
          Combined_Experiment_ArmOnly_B.Sum6_dk;

        // End of Outputs for SubSystem: '<S511>/Hold this value'
      }

      // End of If: '<S511>/if we went through a "step"'

      // Sum: '<S16>/Sum8' incorporates:
      //   DataStoreWrite: '<S16>/RED_IMU_Ay_I'
      //   Gain: '<S511>/divide by delta T'
      //   RandomNumber: '<S16>/Random Number6'

      Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_I = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_ed +
        Combined_Experiment_ArmOnly_DW.NextOutput_hl;

      // Delay: '<S517>/Delay1'
      if (Combined_Experiment_ArmOnly_DW.icLoad_c != 0) {
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mj =
          Combined_Experiment_ArmOnly_B.In[2];
      }

      // Sum: '<S517>/Sum6' incorporates:
      //   Delay: '<S517>/Delay1'

      Combined_Experiment_ArmOnly_B.Sum6_dk = Combined_Experiment_ArmOnly_B.In[2]
        - Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mj;

      // If: '<S517>/if we went through a "step"' incorporates:
      //   Inport: '<S541>/In1'

      if (Combined_Experiment_ArmOnly_B.Sum6_dk != 0.0) {
        // Outputs for IfAction SubSystem: '<S517>/Hold this value' incorporates:
        //   ActionPort: '<S541>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_ju =
          Combined_Experiment_ArmOnly_B.Sum6_dk;

        // End of Outputs for SubSystem: '<S517>/Hold this value'
      }

      // End of If: '<S517>/if we went through a "step"'

      // Gain: '<S517>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S16>/RED_RzD'

      Combined_Experiment_ArmOnly_DW.RED_RzD = 1.0 /
        Combined_Experiment_ArmOnly_P.serverRate *
        Combined_Experiment_ArmOnly_B.In1_ju;

      // Sum: '<S16>/Sum9' incorporates:
      //   DataStoreWrite: '<S16>/RED_IMU_R'
      //   DataStoreWrite: '<S16>/RED_RzD'
      //   RandomNumber: '<S16>/Random Number8'

      Combined_Experiment_ArmOnly_DW.RED_IMU_R =
        Combined_Experiment_ArmOnly_DW.RED_RzD +
        Combined_Experiment_ArmOnly_DW.NextOutput_fw;

      // DataStoreWrite: '<S16>/RED_Px'
      Combined_Experiment_ArmOnly_DW.RED_Px = Combined_Experiment_ArmOnly_B.In[0];

      // DataStoreWrite: '<S16>/RED_Py'
      Combined_Experiment_ArmOnly_DW.RED_Py = Combined_Experiment_ArmOnly_B.In[1];

      // DataStoreWrite: '<S16>/RED_Rz'
      Combined_Experiment_ArmOnly_DW.RED_Rz = Combined_Experiment_ArmOnly_B.In[2];

      // Update for DiscreteIntegrator: '<S500>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[0] =
        Combined_Experiment_ArmOnly_B.x_ddot_a[0];

      // Update for DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[0] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[0];

      // Update for DiscreteIntegrator: '<S500>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[1] =
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // Update for DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[1] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[1];

      // Update for DiscreteIntegrator: '<S500>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[2] =
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;

      // Update for DiscreteIntegrator: '<S500>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[2] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[2];

      // Update for RandomNumber: '<S500>/Random Number'
      Combined_Experiment_ArmOnly_DW.NextOutput =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed)
        * sqrt(Combined_Experiment_ArmOnly_P.noise_variance_BLACK) +
        Combined_Experiment_ArmOnly_P.RandomNumber_Mean;

      // Update for Delay: '<S518>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_lm = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ff =
        Combined_Experiment_ArmOnly_B.In_l[0];

      // Update for Delay: '<S512>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vx'

      Combined_Experiment_ArmOnly_DW.icLoad_em = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p5 =
        Combined_Experiment_ArmOnly_DW.BLACK_Vx;

      // Update for RandomNumber: '<S16>/Random Number1'
      Combined_Experiment_ArmOnly_DW.NextOutput_g =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_p) *
        Combined_Experiment_ArmOnly_P.RandomNumber1_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber1_Mean;

      // Update for Delay: '<S519>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_nw = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_e =
        Combined_Experiment_ArmOnly_B.In_l[1];

      // Update for Delay: '<S513>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLACK_Vy'

      Combined_Experiment_ArmOnly_DW.icLoad_og = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cb =
        Combined_Experiment_ArmOnly_DW.BLACK_Vy;

      // Update for RandomNumber: '<S16>/Random Number'
      Combined_Experiment_ArmOnly_DW.NextOutput_h =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_i) *
        Combined_Experiment_ArmOnly_P.RandomNumber_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber_Mean_g;

      // Update for Delay: '<S520>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_ka = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ew =
        Combined_Experiment_ArmOnly_B.In_l[2];

      // Update for RandomNumber: '<S16>/Random Number2'
      Combined_Experiment_ArmOnly_DW.NextOutput_e =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_b) *
        Combined_Experiment_ArmOnly_P.RandomNumber2_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber2_Mean;

      // Update for RandomNumber: '<S501>/Random Number'
      Combined_Experiment_ArmOnly_DW.NextOutput_m =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_m) * sqrt
        (Combined_Experiment_ArmOnly_P.noise_variance_BLUE) +
        Combined_Experiment_ArmOnly_P.RandomNumber_Mean_o;

      // Update for Delay: '<S521>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_lf = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kn =
        Combined_Experiment_ArmOnly_B.In_k[0];

      // Update for Delay: '<S514>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vx'

      Combined_Experiment_ArmOnly_DW.icLoad_aq = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gc =
        Combined_Experiment_ArmOnly_DW.BLUE_Vx;

      // Update for RandomNumber: '<S16>/Random Number4'
      Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_bw);

      // Update for Delay: '<S522>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_nc = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gu =
        Combined_Experiment_ArmOnly_B.In_k[1];

      // Update for Delay: '<S515>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/BLUE_Vy'

      Combined_Experiment_ArmOnly_DW.icLoad_kx = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_a =
        Combined_Experiment_ArmOnly_DW.BLUE_Vy;

      // Update for RandomNumber: '<S16>/Random Number3'
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_n);

      // Update for Delay: '<S523>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_fa = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_dd =
        Combined_Experiment_ArmOnly_B.In_k[2];

      // Update for RandomNumber: '<S16>/Random Number5'
      Combined_Experiment_ArmOnly_DW.NextOutput_f =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_g) *
        Combined_Experiment_ArmOnly_P.RandomNumber5_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber5_Mean;

      // Update for DiscreteIntegrator: '<S16>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S16>/Data Store Read3'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_IC_LOADI = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_DSTATE +=
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator_gainval *
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_PrevRese =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_me);

      // Update for DiscreteIntegrator: '<S16>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S16>/RED_Px1'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LOAD = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTATE =
        Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevRes =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_m_g);

      // Update for DiscreteIntegrator: '<S16>/Discrete-Time Integrator2' incorporates:
      //   DataStoreWrite: '<S16>/RED_Px2'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_IC_LOAD = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_DSTATE =
        Combined_Experiment_ArmOnly_DW.RED_IMU_Psi;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_PrevRes =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_g_g);

      // Update for DiscreteIntegrator: '<S501>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[0] =
        Combined_Experiment_ArmOnly_B.x_ddot_m[0];

      // Update for DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[0] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0;

      // Update for DiscreteIntegrator: '<S505>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[0] =
        Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_0;

      // Update for DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[0] =
        Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p;

      // Update for DiscreteIntegrator: '<S501>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[1] =
        Combined_Experiment_ArmOnly_B.x_ddot_m[1];

      // Update for DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[1] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1;

      // Update for DiscreteIntegrator: '<S505>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[1] =
        Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_1;

      // Update for DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[1] =
        Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c;

      // Update for DiscreteIntegrator: '<S501>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[2] =
        Combined_Experiment_ArmOnly_B.y5;

      // Update for DiscreteIntegrator: '<S501>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[2] =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2;

      // Update for DiscreteIntegrator: '<S505>/Acceleration  to Velocity'
      Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[2] =
        Combined_Experiment_ArmOnly_B.y10;

      // Update for DiscreteIntegrator: '<S505>/Velocity to Position'
      Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[2] =
        Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_f;

      // Update for RandomNumber: '<S505>/Random Number'
      Combined_Experiment_ArmOnly_DW.NextOutput_l =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_pz) * sqrt
        (Combined_Experiment_ArmOnly_P.noise_variance_RED) +
        Combined_Experiment_ArmOnly_P.RandomNumber_Mean_n;

      // Update for Delay: '<S509>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_ky = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ol =
        Combined_Experiment_ArmOnly_B.In[0];

      // Update for Delay: '<S510>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vx'

      Combined_Experiment_ArmOnly_DW.icLoad_eg = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pm =
        Combined_Experiment_ArmOnly_DW.RED_Vx;

      // Update for RandomNumber: '<S16>/Random Number7'
      Combined_Experiment_ArmOnly_DW.NextOutput_n =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_a) *
        Combined_Experiment_ArmOnly_P.RandomNumber7_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber7_Mean;

      // Update for Delay: '<S516>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_h5 = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_a5 =
        Combined_Experiment_ArmOnly_B.In[1];

      // Update for Delay: '<S511>/Delay1' incorporates:
      //   DataStoreWrite: '<S16>/RED_Vy'

      Combined_Experiment_ArmOnly_DW.icLoad_p = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c3 =
        Combined_Experiment_ArmOnly_DW.RED_Vy;

      // Update for RandomNumber: '<S16>/Random Number6'
      Combined_Experiment_ArmOnly_DW.NextOutput_hl =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_b3) *
        Combined_Experiment_ArmOnly_P.RandomNumber6_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber6_Mean;

      // Update for Delay: '<S517>/Delay1'
      Combined_Experiment_ArmOnly_DW.icLoad_c = 0U;
      Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mj =
        Combined_Experiment_ArmOnly_B.In[2];

      // Update for RandomNumber: '<S16>/Random Number8'
      Combined_Experiment_ArmOnly_DW.NextOutput_fw =
        Comb_rt_nrand_Upu32_Yd_f_pw_snf
        (&Combined_Experiment_ArmOnly_DW.RandSeed_iv) *
        Combined_Experiment_ArmOnly_P.RandomNumber8_StdDev +
        Combined_Experiment_ArmOnly_P.RandomNumber8_Mean;

      // End of Outputs for SubSystem: '<Root>/Simulate Plant Dynamics'
    }

    // End of If: '<Root>/If running a simulation,  grab the simulated states.'

    // SignalConversion generated from: '<S1>/To Workspace' incorporates:
    //   DataStoreRead: '<S1>/ARM_Elbow_Px'
    //   DataStoreRead: '<S1>/ARM_Elbow_Py'
    //   DataStoreRead: '<S1>/ARM_EndEff_Px'
    //   DataStoreRead: '<S1>/ARM_EndEff_Py'
    //   DataStoreRead: '<S1>/ARM_Wrist_Px'
    //   DataStoreRead: '<S1>/ARM_Wrist_Py'
    //   DataStoreRead: '<S1>/BLACK_AHRS_P'
    //   DataStoreRead: '<S1>/BLACK_AHRS_Q'
    //   DataStoreRead: '<S1>/BLACK_AHRS_R'
    //   DataStoreRead: '<S1>/BLACK_Ax'
    //   DataStoreRead: '<S1>/BLACK_Ay'
    //   DataStoreRead: '<S1>/BLACK_Fx_Sat'
    //   DataStoreRead: '<S1>/BLACK_Fy_Sat'
    //   DataStoreRead: '<S1>/BLACK_GyroX_Raw'
    //   DataStoreRead: '<S1>/BLACK_GyroY_Raw'
    //   DataStoreRead: '<S1>/BLACK_GyroZ_Raw'
    //   DataStoreRead: '<S1>/BLACK_Gyro_Attitude'
    //   DataStoreRead: '<S1>/BLACK_IMU_Ax_I'
    //   DataStoreRead: '<S1>/BLACK_IMU_Ax_b'
    //   DataStoreRead: '<S1>/BLACK_IMU_Ay_I'
    //   DataStoreRead: '<S1>/BLACK_IMU_Ay_b'
    //   DataStoreRead: '<S1>/BLACK_IMU_Az_b'
    //   DataStoreRead: '<S1>/BLACK_Px'
    //   DataStoreRead: '<S1>/BLACK_Py'
    //   DataStoreRead: '<S1>/BLACK_Rz'
    //   DataStoreRead: '<S1>/BLACK_RzD'
    //   DataStoreRead: '<S1>/BLACK_RzDD'
    //   DataStoreRead: '<S1>/BLACK_Tz_Sat'
    //   DataStoreRead: '<S1>/BLACK_Vx'
    //   DataStoreRead: '<S1>/BLACK_Vy'
    //   DataStoreRead: '<S1>/BLUE_Fx_Sat'
    //   DataStoreRead: '<S1>/BLUE_Fy_Sat'
    //   DataStoreRead: '<S1>/BLUE_Px'
    //   DataStoreRead: '<S1>/BLUE_Py'
    //   DataStoreRead: '<S1>/BLUE_Rz'
    //   DataStoreRead: '<S1>/BLUE_RzD'
    //   DataStoreRead: '<S1>/BLUE_Tz_Sat'
    //   DataStoreRead: '<S1>/BLUE_Vx'
    //   DataStoreRead: '<S1>/BLUE_Vy'
    //   DataStoreRead: '<S1>/RED_AHRS_P'
    //   DataStoreRead: '<S1>/RED_AHRS_Q'
    //   DataStoreRead: '<S1>/RED_AHRS_R'
    //   DataStoreRead: '<S1>/RED_Ax'
    //   DataStoreRead: '<S1>/RED_Ay'
    //   DataStoreRead: '<S1>/RED_Fx_Sat'
    //   DataStoreRead: '<S1>/RED_Fy_Sat'
    //   DataStoreRead: '<S1>/RED_GyroX_Raw'
    //   DataStoreRead: '<S1>/RED_GyroY_Raw'
    //   DataStoreRead: '<S1>/RED_GyroZ_Raw'
    //   DataStoreRead: '<S1>/RED_Gyro_Attitude'
    //   DataStoreRead: '<S1>/RED_IMU_Ax_I'
    //   DataStoreRead: '<S1>/RED_IMU_Ax_b'
    //   DataStoreRead: '<S1>/RED_IMU_Ay_I'
    //   DataStoreRead: '<S1>/RED_IMU_Ay_b'
    //   DataStoreRead: '<S1>/RED_IMU_Az_b'
    //   DataStoreRead: '<S1>/RED_Px'
    //   DataStoreRead: '<S1>/RED_Py '
    //   DataStoreRead: '<S1>/RED_Rz'
    //   DataStoreRead: '<S1>/RED_RzDD'
    //   DataStoreRead: '<S1>/RED_Tz_Sat'
    //   DataStoreRead: '<S1>/RED_Vx '
    //   DataStoreRead: '<S1>/RED_Vy'
    //   DataStoreRead: '<S1>/RED_Vz'
    //   DataStoreRead: '<S1>/Universal_Time'

    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[0] =
      Combined_Experiment_ArmOnly_DW.Univ_Time;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[1] =
      Combined_Experiment_ArmOnly_DW.RED_Fx_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[2] =
      Combined_Experiment_ArmOnly_DW.RED_Fy_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[3] =
      Combined_Experiment_ArmOnly_DW.RED_Tz_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[4] =
      Combined_Experiment_ArmOnly_DW.RED_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[5] =
      Combined_Experiment_ArmOnly_DW.RED_Py;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[6] =
      Combined_Experiment_ArmOnly_DW.RED_Rz;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[7] =
      Combined_Experiment_ArmOnly_DW.RED_Vx;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[8] =
      Combined_Experiment_ArmOnly_DW.RED_Vy;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[9] =
      Combined_Experiment_ArmOnly_DW.RED_RzD;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[10] =
      Combined_Experiment_ArmOnly_DW.RED_AHRS_Q;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[11] =
      Combined_Experiment_ArmOnly_DW.RED_AHRS_P;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[12] =
      Combined_Experiment_ArmOnly_DW.RED_AHRS_R;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[13] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[14] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[15] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Az_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[16] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_I;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[17] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_I;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[18] =
      Combined_Experiment_ArmOnly_DW.RED_Ax;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[19] =
      Combined_Experiment_ArmOnly_DW.RED_Ay;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[20] =
      Combined_Experiment_ArmOnly_DW.RED_RzDD;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[21] =
      Combined_Experiment_ArmOnly_DW.BLACK_Fx_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[22] =
      Combined_Experiment_ArmOnly_DW.BLACK_Fy_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[23] =
      Combined_Experiment_ArmOnly_DW.BLACK_Tz_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[24] =
      Combined_Experiment_ArmOnly_DW.BLACK_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[25] =
      Combined_Experiment_ArmOnly_DW.BLACK_Py;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[26] =
      Combined_Experiment_ArmOnly_DW.BLACK_Rz;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[27] =
      Combined_Experiment_ArmOnly_DW.BLACK_Vx;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[28] =
      Combined_Experiment_ArmOnly_DW.BLACK_Vy;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[29] =
      Combined_Experiment_ArmOnly_DW.BLACK_RzD;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[30] =
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_Q;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[31] =
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_P;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[32] =
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_R;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[33] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[34] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[35] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Az_b;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[36] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_I;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[37] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_I;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[38] =
      Combined_Experiment_ArmOnly_DW.BLACK_Ax;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[39] =
      Combined_Experiment_ArmOnly_DW.BLACK_Ay;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[40] =
      Combined_Experiment_ArmOnly_DW.BLACK_RzDD;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[41] =
      Combined_Experiment_ArmOnly_DW.BLUE_Fx_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[42] =
      Combined_Experiment_ArmOnly_DW.BLUE_Fy_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[43] =
      Combined_Experiment_ArmOnly_DW.BLUE_Tz_Sat;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[44] =
      Combined_Experiment_ArmOnly_DW.BLUE_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[45] =
      Combined_Experiment_ArmOnly_DW.BLUE_Py;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[46] =
      Combined_Experiment_ArmOnly_DW.BLUE_Rz;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[47] =
      Combined_Experiment_ArmOnly_DW.BLUE_Vx;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[48] =
      Combined_Experiment_ArmOnly_DW.BLUE_Vy;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[49] =
      Combined_Experiment_ArmOnly_DW.BLUE_RzD;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[50] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Q;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[51] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_P;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[52] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_R;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[53] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Q;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[54] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_P;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[55] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_R;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[56] =
      Combined_Experiment_ArmOnly_DW.RED_IMU_Psi;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[57] =
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[58] =
      Combined_Experiment_ArmOnly_DW.ARM_Elbow_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[59] =
      Combined_Experiment_ArmOnly_DW.ARM_Elbow_Py;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[60] =
      Combined_Experiment_ArmOnly_DW.ARM_Wrist_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[61] =
      Combined_Experiment_ArmOnly_DW.ARM_Wrist_Py;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[62] =
      Combined_Experiment_ArmOnly_DW.ARM_EndEff_Px;
    Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[63] =
      Combined_Experiment_ArmOnly_DW.ARM_EndEff_Py;

    // ToWorkspace: '<S1>/To Workspace'
    rt_UpdateLogVar((LogVar *)(LogVar*)
                    (Combined_Experiment_ArmOnly_DW.ToWorkspace_PWORK.LoggedData),
                    &Combined_Experiment_ArmOnly_B.TmpSignalConversionAtToWork[0],
                    0);

    // If: '<S2>/This IF block determines whether or not to run the exp code' incorporates:
    //   Constant: '<S19>/Constant'

    if (Combined_Experiment_ArmOnly_P.simMode == 0.0) {
      // Outputs for IfAction SubSystem: '<S2>/Change Behavior' incorporates:
      //   ActionPort: '<S18>/Action Port'

      // MATLABSystem: '<S20>/Digital Write' incorporates:
      //   DataStoreRead: '<S18>/Data Store Read'

      Combined_Experiment_ArmOnly_B.Sum6_l1 = rt_roundd_snf
        (Combined_Experiment_ArmOnly_DW.Magnet_State);
      if (Combined_Experiment_ArmOnly_B.Sum6_l1 < 256.0) {
        if (Combined_Experiment_ArmOnly_B.Sum6_l1 >= 0.0) {
          Combined_Experiment_ArmOnly_B.status = static_cast<uint8_T>
            (Combined_Experiment_ArmOnly_B.Sum6_l1);
        } else {
          Combined_Experiment_ArmOnly_B.status = 0U;
        }
      } else {
        Combined_Experiment_ArmOnly_B.status = MAX_uint8_T;
      }

      MW_gpioWrite(10U, Combined_Experiment_ArmOnly_B.status);

      // End of MATLABSystem: '<S20>/Digital Write'

      // MATLABSystem: '<S21>/Digital Write' incorporates:
      //   DataStoreRead: '<S18>/Data Store Read1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 = rt_roundd_snf
        (Combined_Experiment_ArmOnly_DW.Float_State);
      if (Combined_Experiment_ArmOnly_B.Sum6_l1 < 256.0) {
        if (Combined_Experiment_ArmOnly_B.Sum6_l1 >= 0.0) {
          Combined_Experiment_ArmOnly_B.status = static_cast<uint8_T>
            (Combined_Experiment_ArmOnly_B.Sum6_l1);
        } else {
          Combined_Experiment_ArmOnly_B.status = 0U;
        }
      } else {
        Combined_Experiment_ArmOnly_B.status = MAX_uint8_T;
      }

      MW_gpioWrite(26U, Combined_Experiment_ArmOnly_B.status);

      // End of MATLABSystem: '<S21>/Digital Write'
      // End of Outputs for SubSystem: '<S2>/Change Behavior'
    }

    // End of If: '<S2>/This IF block determines whether or not to run the exp code' 

    // If: '<S3>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
    //   Constant: '<S25>/Constant'
    //   Constant: '<S3>/Constant'
    //   DataStoreRead: '<S22>/BLACK_Fx'
    //   DataStoreRead: '<S22>/BLACK_Fy'
    //   Product: '<S28>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S28>/Rotate F_I to F_b'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
        (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S3>/Change BLACK Behavior' incorporates:
      //   ActionPort: '<S22>/Action Port'

      // MATLAB Function: '<S32>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S27>/BLACK_Rz'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix);

      // MATLAB Function: '<S27>/MATLAB Function1'
      // MATLAB Function 'From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function1': '<S30>:1' 
      // '<S30>:1:3' Vec1 = [ -1
      // '<S30>:1:4'          -1
      // '<S30>:1:5'           0
      // '<S30>:1:6'           0
      // '<S30>:1:7'           1
      // '<S30>:1:8'           1
      // '<S30>:1:9'           0
      // '<S30>:1:10'           0 ];
      // '<S30>:1:12' Vec2 = [  0
      // '<S30>:1:13'           0
      // '<S30>:1:14'           1
      // '<S30>:1:15'           1
      // '<S30>:1:16'           0
      // '<S30>:1:17'           0
      // '<S30>:1:18'          -1
      // '<S30>:1:19'          -1 ];
      // '<S30>:1:21' Vec3 = thruster_dist2CG_BLACK./1000;
      // '<S30>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S30>:1:25' Mat2 = diag((F_thrusters_BLACK));
      memset(&Combined_Experiment_ArmOnly_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // '<S30>:1:27' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S29>:1' 
      // '<S29>:1:3' Vec1 = [ -1
      // '<S29>:1:4'          -1
      // '<S29>:1:5'           0
      // '<S29>:1:6'           0
      // '<S29>:1:7'           1
      // '<S29>:1:8'           1
      // '<S29>:1:9'           0
      // '<S29>:1:10'           0 ];
      // '<S29>:1:12' Vec2 = [  0
      // '<S29>:1:13'           0
      // '<S29>:1:14'           1
      // '<S29>:1:15'           1
      // '<S29>:1:16'           0
      // '<S29>:1:17'           0
      // '<S29>:1:18'          -1
      // '<S29>:1:19'          -1 ];
      // '<S29>:1:21' Vec3 = thruster_dist2CG_BLACK./1000;
      // '<S29>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S29>:1:25' Mat2 = diag((F_thrusters_BLACK./2));
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.Mat2[Combined_Experiment_ArmOnly_B.uElOffset1
          + (Combined_Experiment_ArmOnly_B.uElOffset1 << 3)] =
          Combined_Experiment_ArmOnly_P.F_thrusters_BLACK[Combined_Experiment_ArmOnly_B.uElOffset1];

        // MATLAB Function: '<S27>/MATLAB Function'
        Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
          =
          Combined_Experiment_ArmOnly_P.F_thrusters_BLACK[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S27>/MATLAB Function'
      memset(&Combined_Experiment_ArmOnly_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S29>:1:27' H    = Mat1*Mat2;
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.Mat2_m[Combined_Experiment_ArmOnly_B.uElOffset1
          + (Combined_Experiment_ArmOnly_B.uElOffset1 << 3)] =
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1] =
          b[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 1] =
          c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 2] =
          Combined_Experiment_ArmOnly_P.thruster_dist2CG_BLACK[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 1000.0;
      }

      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.b[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.Mat2_m
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // PermuteDimensions: '<S33>/transpose'
      Combined_Experiment_ArmOnly_B.yElIdx = 0;
      Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
      for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
           Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
           Combined_Experiment_ArmOnly_B.ntIdx1++) {
        Combined_Experiment_ArmOnly_B.uElOffset0 =
          Combined_Experiment_ArmOnly_B.uElOffset1;
        for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx0++) {
          Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.yElIdx]
            =
            Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.uElOffset0];
          Combined_Experiment_ArmOnly_B.yElIdx++;
          Combined_Experiment_ArmOnly_B.uElOffset0 += 3;
        }

        Combined_Experiment_ArmOnly_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S33>/transpose'

      // MATLAB Function: '<S28>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S22>/BLACK_Rz'

      Comb_CreateRotationMatrix_h(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h);

      // Product: '<S33>/Product'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.rtb_H_bu_k[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.rtb_H_bu_k[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.H_bu[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.transpose_d
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // Product: '<S33>/Product1' incorporates:
      //   Product: '<S33>/Product'

      Combined_Experim_rt_invd3x3_snf(Combined_Experiment_ArmOnly_B.rtb_H_bu_k,
        Combined_Experiment_ArmOnly_B.b_c);
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h.C_bI[0] *
        Combined_Experiment_ArmOnly_DW.BLACK_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h.C_bI[2] *
        Combined_Experiment_ArmOnly_DW.BLACK_Fy;

      // Product: '<S28>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S22>/BLACK_Fx'
      //   DataStoreRead: '<S22>/BLACK_Fy'
      //   SignalConversion generated from: '<S28>/Rotate F_I to F_b'

      Combined_Experiment_ArmOnly_B.Sum6_dk =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h.C_bI[1] *
        Combined_Experiment_ArmOnly_DW.BLACK_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h.C_bI[3] *
        Combined_Experiment_ArmOnly_DW.BLACK_Fy;

      // Product: '<S33>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.ntIdx0 =
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            (Combined_Experiment_ArmOnly_B.uElOffset1 << 3);
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] =
            0.0;
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.ntIdx1];
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 1] *
            Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.ntIdx1
            + 8];
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 2] *
            Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.ntIdx1
            + 16];
        }
      }

      // End of Product: '<S33>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1
          + 16] * Combined_Experiment_ArmOnly_DW.BLACK_Tz +
          (Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1
           + 8] * Combined_Experiment_ArmOnly_B.Sum6_dk +
           Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1]
           * Combined_Experiment_ArmOnly_B.Sum6_l1);

        // Saturate: '<S27>/Remove Negatives' incorporates:
        //   DataStoreRead: '<S22>/BLACK_Tz'
        //   Product: '<S27>/Product'
        //   SignalConversion generated from: '<S27>/Product'

        if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist >
            Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat;
        } else if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist <
                   Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat;
        } else {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;
        }

        // End of Saturate: '<S27>/Remove Negatives'
      }

      // MATLAB Function: '<S27>/MATLAB Function2' incorporates:
      //   DataStoreRead: '<S22>/BLACK_Tz'
      //   Product: '<S27>/Product'
      //   SignalConversion generated from: '<S27>/Product'

      Combined_Ex_MATLABFunction2
        (Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c,
         &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2);

      // MATLAB Function: '<S27>/MATLAB Function1'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1] =
          b[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 1] =
          c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 2] =
          Combined_Experiment_ArmOnly_P.thruster_dist2CG_BLACK[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 1000.0;
      }

      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.b[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.Mat2
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // Product: '<S27>/Product1'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
          = 0.0;
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
            += Combined_Experiment_ArmOnly_B.transpose_d[3 *
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[Combined_Experiment_ArmOnly_B.ntIdx1];
        }
      }

      // End of Product: '<S27>/Product1'

      // DataStoreWrite: '<S27>/BLACK_Fx_Sat' incorporates:
      //   Product: '<S32>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLACK_Fx_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix.C_Ib[0] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix.C_Ib[2] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S27>/BLACK_Fy_Sat' incorporates:
      //   Product: '<S32>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLACK_Fy_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix.C_Ib[1] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix.C_Ib[3] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S27>/BLACK_Tz_Sat'
      Combined_Experiment_ArmOnly_DW.BLACK_Tz_Sat =
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // End of Outputs for SubSystem: '<S3>/Change BLACK Behavior'
    }

    // End of If: '<S3>/This IF block determines whether or not to run the BLACK sim//exp' 

    // If: '<S3>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
    //   Constant: '<S25>/Constant'
    //   Constant: '<S3>/Constant'
    //   DataStoreRead: '<S23>/BLUE_Fx'
    //   DataStoreRead: '<S23>/BLUE_Fy'
    //   Product: '<S37>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S37>/Rotate F_I to F_b'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) ||
        (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S3>/Change BLUE Behavior' incorporates:
      //   ActionPort: '<S23>/Action Port'

      // MATLAB Function: '<S41>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S36>/BLUE_Rz'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.BLUE_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_i);

      // MATLAB Function: '<S36>/MATLAB Function1'
      Combined_Exp_MATLABFunction
        (&Combined_Experiment_ArmOnly_B.sf_MATLABFunction1_b);

      // MATLAB Function: '<S36>/MATLAB Function'
      Combined_Exp_MATLABFunction
        (&Combined_Experiment_ArmOnly_B.sf_MATLABFunction_i);

      // PermuteDimensions: '<S42>/transpose'
      Combined_Experiment_ArmOnly_B.yElIdx = 0;
      Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
      for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
           Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
           Combined_Experiment_ArmOnly_B.ntIdx1++) {
        Combined_Experiment_ArmOnly_B.uElOffset0 =
          Combined_Experiment_ArmOnly_B.uElOffset1;
        for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx0++) {
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.yElIdx]
            =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction_i.H[Combined_Experiment_ArmOnly_B.uElOffset0];
          Combined_Experiment_ArmOnly_B.yElIdx++;
          Combined_Experiment_ArmOnly_B.uElOffset0 += 3;
        }

        Combined_Experiment_ArmOnly_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S42>/transpose'

      // MATLAB Function: '<S37>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S23>/BLUE_Rz'

      Comb_CreateRotationMatrix_h(Combined_Experiment_ArmOnly_DW.BLUE_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_f);

      // Product: '<S42>/Product'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.ntIdx0 =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.b_c[Combined_Experiment_ArmOnly_B.ntIdx0]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.yElIdx = 0;
               Combined_Experiment_ArmOnly_B.yElIdx < 8;
               Combined_Experiment_ArmOnly_B.yElIdx++) {
            Combined_Experiment_ArmOnly_B.b_c[Combined_Experiment_ArmOnly_B.ntIdx0]
              += Combined_Experiment_ArmOnly_B.sf_MATLABFunction_i.H[3 *
              Combined_Experiment_ArmOnly_B.yElIdx +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.transpose_p
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.yElIdx];
          }
        }
      }

      // Product: '<S42>/Product1' incorporates:
      //   Product: '<S42>/Product'

      Combined_Experim_rt_invd3x3_snf(Combined_Experiment_ArmOnly_B.b_c,
        Combined_Experiment_ArmOnly_B.rtb_H_bu_k);
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_f.C_bI[0] *
        Combined_Experiment_ArmOnly_DW.BLUE_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_f.C_bI[2] *
        Combined_Experiment_ArmOnly_DW.BLUE_Fy;

      // Product: '<S37>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S23>/BLUE_Fx'
      //   DataStoreRead: '<S23>/BLUE_Fy'
      //   SignalConversion generated from: '<S37>/Rotate F_I to F_b'

      Combined_Experiment_ArmOnly_B.Sum6_dk =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_f.C_bI[1] *
        Combined_Experiment_ArmOnly_DW.BLUE_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_f.C_bI[3] *
        Combined_Experiment_ArmOnly_DW.BLUE_Fy;

      // Product: '<S42>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.ntIdx0 =
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            (Combined_Experiment_ArmOnly_B.uElOffset1 << 3);
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] =
            0.0;
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.rtb_H_bu_k[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx1];
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.rtb_H_bu_k[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 1] *
            Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx1
            + 8];
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.ntIdx0] +=
            Combined_Experiment_ArmOnly_B.rtb_H_bu_k[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 2] *
            Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx1
            + 16];
        }
      }

      // End of Product: '<S42>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
          Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1
          + 16] * Combined_Experiment_ArmOnly_DW.BLUE_Tz +
          (Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1
           + 8] * Combined_Experiment_ArmOnly_B.Sum6_dk +
           Combined_Experiment_ArmOnly_B.b[Combined_Experiment_ArmOnly_B.uElOffset1]
           * Combined_Experiment_ArmOnly_B.Sum6_l1);

        // Saturate: '<S36>/Remove Negatives' incorporates:
        //   DataStoreRead: '<S23>/BLUE_Tz'
        //   Product: '<S36>/Product'
        //   SignalConversion generated from: '<S36>/Product'

        if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist >
            Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat_p) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat_p;
        } else if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist <
                   Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat_c) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat_c;
        } else {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;
        }

        // End of Saturate: '<S36>/Remove Negatives'
      }

      // MATLAB Function: '<S36>/MATLAB Function2' incorporates:
      //   DataStoreRead: '<S23>/BLUE_Tz'
      //   Product: '<S36>/Product'
      //   SignalConversion generated from: '<S36>/Product'

      Combined_Ex_MATLABFunction2
        (Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c,
         &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n);

      // Product: '<S36>/Product1'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
          = 0.0;
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
            += Combined_Experiment_ArmOnly_B.sf_MATLABFunction1_b.H[3 *
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[Combined_Experiment_ArmOnly_B.ntIdx1];
        }
      }

      // End of Product: '<S36>/Product1'

      // DataStoreWrite: '<S36>/BLUE_Fx_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLUE_Fx_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_i.C_Ib[0] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_i.C_Ib[2] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S36>/BLUE_Fy_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLUE_Fy_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_i.C_Ib[1] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_i.C_Ib[3] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S36>/BLUE_Tz_Sat'
      Combined_Experiment_ArmOnly_DW.BLUE_Tz_Sat =
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // End of Outputs for SubSystem: '<S3>/Change BLUE Behavior'
    }

    // End of If: '<S3>/This IF block determines whether or not to run the BLUE sim//exp' 

    // If: '<S3>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
    //   Constant: '<S25>/Constant'
    //   Constant: '<S3>/Constant'
    //   DataStoreRead: '<S24>/RED_Fx'
    //   DataStoreRead: '<S24>/RED_Fy'
    //   Product: '<S46>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S46>/Rotate F_I to F_b'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
        (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S3>/Change RED Behavior' incorporates:
      //   ActionPort: '<S24>/Action Port'

      // MATLAB Function: '<S50>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S45>/RED_Rz'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.RED_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_l);

      // MATLAB Function: '<S45>/MATLAB Function1'
      // MATLAB Function 'From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function1': '<S48>:1' 
      // '<S48>:1:3' Vec1 = [ -1
      // '<S48>:1:4'          -1
      // '<S48>:1:5'           0
      // '<S48>:1:6'           0
      // '<S48>:1:7'           1
      // '<S48>:1:8'           1
      // '<S48>:1:9'           0
      // '<S48>:1:10'           0 ];
      // '<S48>:1:12' Vec2 = [  0
      // '<S48>:1:13'           0
      // '<S48>:1:14'           1
      // '<S48>:1:15'           1
      // '<S48>:1:16'           0
      // '<S48>:1:17'           0
      // '<S48>:1:18'          -1
      // '<S48>:1:19'          -1 ];
      // '<S48>:1:21' Vec3 = thruster_dist2CG_RED./1000;
      // '<S48>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S48>:1:25' Mat2 = diag((F_thrusters_RED));
      memset(&Combined_Experiment_ArmOnly_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // '<S48>:1:27' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S47>:1' 
      // '<S47>:1:3' Vec1 = [ -1
      // '<S47>:1:4'          -1
      // '<S47>:1:5'           0
      // '<S47>:1:6'           0
      // '<S47>:1:7'           1
      // '<S47>:1:8'           1
      // '<S47>:1:9'           0
      // '<S47>:1:10'           0 ];
      // '<S47>:1:12' Vec2 = [  0
      // '<S47>:1:13'           0
      // '<S47>:1:14'           1
      // '<S47>:1:15'           1
      // '<S47>:1:16'           0
      // '<S47>:1:17'           0
      // '<S47>:1:18'          -1
      // '<S47>:1:19'          -1 ];
      // '<S47>:1:21' Vec3 = thruster_dist2CG_RED./1000;
      // '<S47>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S47>:1:25' Mat2 = diag((F_thrusters_RED./2));
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.Mat2[Combined_Experiment_ArmOnly_B.uElOffset1
          + (Combined_Experiment_ArmOnly_B.uElOffset1 << 3)] =
          Combined_Experiment_ArmOnly_P.F_thrusters_RED[Combined_Experiment_ArmOnly_B.uElOffset1];

        // MATLAB Function: '<S45>/MATLAB Function'
        Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
          =
          Combined_Experiment_ArmOnly_P.F_thrusters_RED[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S45>/MATLAB Function'
      memset(&Combined_Experiment_ArmOnly_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S47>:1:27' H    = Mat1*Mat2;
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.Mat2_m[Combined_Experiment_ArmOnly_B.uElOffset1
          + (Combined_Experiment_ArmOnly_B.uElOffset1 << 3)] =
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1] =
          b[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 1] =
          c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 2] =
          Combined_Experiment_ArmOnly_P.thruster_dist2CG_RED[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 1000.0;
      }

      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.b[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.Mat2_m
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // PermuteDimensions: '<S51>/transpose'
      Combined_Experiment_ArmOnly_B.yElIdx = 0;
      Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
      for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
           Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
           Combined_Experiment_ArmOnly_B.ntIdx1++) {
        Combined_Experiment_ArmOnly_B.uElOffset0 =
          Combined_Experiment_ArmOnly_B.uElOffset1;
        for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx0++) {
          Combined_Experiment_ArmOnly_B.transpose[Combined_Experiment_ArmOnly_B.yElIdx]
            =
            Combined_Experiment_ArmOnly_B.H_bu[Combined_Experiment_ArmOnly_B.uElOffset0];
          Combined_Experiment_ArmOnly_B.yElIdx++;
          Combined_Experiment_ArmOnly_B.uElOffset0 += 3;
        }

        Combined_Experiment_ArmOnly_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S51>/transpose'

      // MATLAB Function: '<S46>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S24>/RED_Rz'

      Comb_CreateRotationMatrix_h(Combined_Experiment_ArmOnly_DW.RED_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_n);

      // Product: '<S51>/Product'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.rtb_H_bu_k[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.rtb_H_bu_k[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.H_bu[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.transpose
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // Product: '<S51>/Product1' incorporates:
      //   Product: '<S51>/Product'

      Combined_Experim_rt_invd3x3_snf(Combined_Experiment_ArmOnly_B.rtb_H_bu_k,
        Combined_Experiment_ArmOnly_B.b_c);
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_n.C_bI[0] *
        Combined_Experiment_ArmOnly_DW.RED_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_n.C_bI[2] *
        Combined_Experiment_ArmOnly_DW.RED_Fy;

      // Product: '<S46>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S24>/RED_Fx'
      //   DataStoreRead: '<S24>/RED_Fy'
      //   SignalConversion generated from: '<S46>/Rotate F_I to F_b'

      Combined_Experiment_ArmOnly_B.Sum6_dk =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_n.C_bI[1] *
        Combined_Experiment_ArmOnly_DW.RED_Fx +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_n.C_bI[3] *
        Combined_Experiment_ArmOnly_DW.RED_Fy;

      // Product: '<S51>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.ntIdx0 =
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            (Combined_Experiment_ArmOnly_B.uElOffset1 << 3);
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx0]
            = 0.0;
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx0]
            += Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.transpose[Combined_Experiment_ArmOnly_B.ntIdx1];
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx0]
            += Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 1] *
            Combined_Experiment_ArmOnly_B.transpose[Combined_Experiment_ArmOnly_B.ntIdx1
            + 8];
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.ntIdx0]
            += Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 2] *
            Combined_Experiment_ArmOnly_B.transpose[Combined_Experiment_ArmOnly_B.ntIdx1
            + 16];
        }
      }

      // End of Product: '<S51>/Product2'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
          Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.uElOffset1
          + 16] * Combined_Experiment_ArmOnly_DW.RED_Tz +
          (Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.uElOffset1
           + 8] * Combined_Experiment_ArmOnly_B.Sum6_dk +
           Combined_Experiment_ArmOnly_B.transpose_p[Combined_Experiment_ArmOnly_B.uElOffset1]
           * Combined_Experiment_ArmOnly_B.Sum6_l1);

        // Saturate: '<S45>/Remove Negatives' incorporates:
        //   DataStoreRead: '<S24>/RED_Tz'
        //   Product: '<S45>/Product'
        //   SignalConversion generated from: '<S45>/Product'

        if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist >
            Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat_d) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_UpperSat_d;
        } else if (Combined_Experiment_ArmOnly_B.RED_Tz_Wrist <
                   Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat_a) {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_P.RemoveNegatives_LowerSat_a;
        } else {
          Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c[Combined_Experiment_ArmOnly_B.uElOffset1]
            = Combined_Experiment_ArmOnly_B.RED_Tz_Wrist;
        }

        // End of Saturate: '<S45>/Remove Negatives'
      }

      // MATLAB Function: '<S45>/MATLAB Function2' incorporates:
      //   DataStoreRead: '<S24>/RED_Tz'
      //   Product: '<S45>/Product'
      //   SignalConversion generated from: '<S45>/Product'

      Combined_Ex_MATLABFunction2
        (Combined_Experiment_ArmOnly_B.rtb_RemoveNegatives_i_c,
         &Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l);

      // MATLAB Function: '<S45>/MATLAB Function1'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1] =
          b[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 1] =
          c[Combined_Experiment_ArmOnly_B.uElOffset1];
        Combined_Experiment_ArmOnly_B.b[3 *
          Combined_Experiment_ArmOnly_B.uElOffset1 + 2] =
          Combined_Experiment_ArmOnly_P.thruster_dist2CG_RED[Combined_Experiment_ArmOnly_B.uElOffset1]
          / 1000.0;
      }

      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 8;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 3;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.yElIdx =
            Combined_Experiment_ArmOnly_B.ntIdx1 + 3 *
            Combined_Experiment_ArmOnly_B.uElOffset1;
          Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.yElIdx]
            = 0.0;
          for (Combined_Experiment_ArmOnly_B.ntIdx0 = 0;
               Combined_Experiment_ArmOnly_B.ntIdx0 < 8;
               Combined_Experiment_ArmOnly_B.ntIdx0++) {
            Combined_Experiment_ArmOnly_B.transpose_d[Combined_Experiment_ArmOnly_B.yElIdx]
              += Combined_Experiment_ArmOnly_B.b[3 *
              Combined_Experiment_ArmOnly_B.ntIdx0 +
              Combined_Experiment_ArmOnly_B.ntIdx1] *
              Combined_Experiment_ArmOnly_B.Mat2
              [(Combined_Experiment_ArmOnly_B.uElOffset1 << 3) +
              Combined_Experiment_ArmOnly_B.ntIdx0];
          }
        }
      }

      // Product: '<S45>/Product1'
      for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
           Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
           Combined_Experiment_ArmOnly_B.uElOffset1++) {
        Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
          = 0.0;
        for (Combined_Experiment_ArmOnly_B.ntIdx1 = 0;
             Combined_Experiment_ArmOnly_B.ntIdx1 < 8;
             Combined_Experiment_ArmOnly_B.ntIdx1++) {
          Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
            += Combined_Experiment_ArmOnly_B.transpose_d[3 *
            Combined_Experiment_ArmOnly_B.ntIdx1 +
            Combined_Experiment_ArmOnly_B.uElOffset1] *
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[Combined_Experiment_ArmOnly_B.ntIdx1];
        }
      }

      // End of Product: '<S45>/Product1'

      // DataStoreWrite: '<S45>/RED_Fx_Sat' incorporates:
      //   Product: '<S50>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.RED_Fx_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_l.C_Ib[0] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_l.C_Ib[2] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S45>/RED_Fy_Sat' incorporates:
      //   Product: '<S50>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.RED_Fy_Sat =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_l.C_Ib[1] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_l.C_Ib[3] *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];

      // DataStoreWrite: '<S45>/RED_Tz_Sat'
      Combined_Experiment_ArmOnly_DW.RED_Tz_Sat =
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // End of Outputs for SubSystem: '<S3>/Change RED Behavior'
    }

    // End of If: '<S3>/This IF block determines whether or not to run the RED sim//exp ' 

    // RateTransition: '<S3>/Rate Transition' incorporates:
    //   RateTransition: '<S3>/Rate Transition1'
    //   RateTransition: '<S3>/Rate Transition3'
    //   RateTransition: '<S3>/Rate Transition4'
    //   RateTransition: '<S3>/Rate Transition5'
    //   RateTransition: '<S3>/Rate Transition6'
    //   RateTransition: '<S3>/Rate Transition7'
    //   RateTransition: '<S3>/Rate Transition8'
    //   Sum: '<S3>/Sum'
    //   Sum: '<S3>/Sum1'
    //   Sum: '<S3>/Sum2'
    //   Sum: '<S3>/Sum3'
    //   Sum: '<S3>/Sum4'
    //   Sum: '<S3>/Sum5'
    //   Sum: '<S3>/Sum6'
    //   Sum: '<S3>/Sum7'

    if (Combined_Experiment_ArmOnly_M->Timing.RateInteraction.TID1_2) {
      Combined_Experiment_ArmOnly_DW.RateTransition_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[0] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[7]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[7];
      Combined_Experiment_ArmOnly_DW.RateTransition1_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[1] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[2]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[2];

      // RateTransition: '<S3>/Rate Transition2' incorporates:
      //   Sum: '<S3>/Sum'
      //   Sum: '<S3>/Sum1'

      Combined_Experiment_ArmOnly_DW.RateTransition2_Buffer =
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[2];
      Combined_Experiment_ArmOnly_DW.RateTransition3_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[3] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[4]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[4];
      Combined_Experiment_ArmOnly_DW.RateTransition4_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[4] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[3]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[3];
      Combined_Experiment_ArmOnly_DW.RateTransition5_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[5] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[6]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[6];
      Combined_Experiment_ArmOnly_DW.RateTransition6_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[6] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[5]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[5];
      Combined_Experiment_ArmOnly_DW.RateTransition7_Buffer =
        (Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[7] +
         Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[0]) +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[0];
      Combined_Experiment_ArmOnly_DW.RateTransition8_Buffer =
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[1] +
        Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[1];
    }

    // End of RateTransition: '<S3>/Rate Transition'

    // If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' incorporates:
    //   Constant: '<S74>/Constant'

    Combined_Experiment_ArmOnly_B.rtPrevAction =
      Combined_Experiment_ArmOnly_DW.Ifperforminganexperimentgrabthe;
    Combined_Experiment_ArmOnly_B.rtAction = -1;
    if (Combined_Experiment_ArmOnly_P.simMode == 0.0) {
      Combined_Experiment_ArmOnly_B.rtAction = 0;
    } else {
      if (Combined_Experiment_ArmOnly_P.simMode == 1.0) {
        Combined_Experiment_ArmOnly_B.rtAction = 1;
      }
    }

    Combined_Experiment_ArmOnly_DW.Ifperforminganexperimentgrabthe =
      Combined_Experiment_ArmOnly_B.rtAction;
    if ((Combined_Experiment_ArmOnly_B.rtPrevAction !=
         Combined_Experiment_ArmOnly_B.rtAction) &&
        (Combined_Experiment_ArmOnly_B.rtPrevAction == 0)) {
      // Disable for If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
      switch (Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei) {
       case 0:
        // Disable for Enabled SubSystem: '<S77>/Enabled Subsystem'
        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j) {
          Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j = false;
        }

        // End of Disable for SubSystem: '<S77>/Enabled Subsystem'

        // Disable for Enabled SubSystem: '<S77>/Enabled Subsystem1'
        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d) {
          Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d = false;
        }

        // End of Disable for SubSystem: '<S77>/Enabled Subsystem1'

        // Disable for Enabled SubSystem: '<S138>/Enabled Subsystem'
        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m) {
          Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m = false;
        }

        // End of Disable for SubSystem: '<S138>/Enabled Subsystem'
        break;

       case 1:
        // Disable for If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 
        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_d1 = -1;

        // Disable for If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
        if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o == 0)
        {
          // Disable for Enabled SubSystem: '<S80>/Enabled Subsystem1'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE = false;
          }

          // End of Disable for SubSystem: '<S80>/Enabled Subsystem1'

          // Disable for Enabled SubSystem: '<S106>/Enabled Subsystem'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE = false;
          }

          // End of Disable for SubSystem: '<S106>/Enabled Subsystem'
        }

        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o = -1;
        break;
      }

      Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei = -1;

      // End of Disable for If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
    }

    switch (Combined_Experiment_ArmOnly_B.rtAction) {
     case 0:
      // Outputs for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S75>/Action Port'

      // If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' incorporates:
      //   Clock: '<S77>/Clock'
      //   Constant: '<S75>/Which PLATFORM is being used?'
      //   Delay: '<S153>/Delay'
      //   Inport: '<S139>/In1'
      //   Inport: '<S140>/In1'
      //   Inport: '<S154>/In1'
      //   Logic: '<S153>/Logical Operator'
      //   Logic: '<S153>/Logical Operator1'
      //   SignalConversion generated from: '<S140>/Enable'
      //   SignalConversion generated from: '<S139>/Enable'

      Combined_Experiment_ArmOnly_B.rtPrevAction =
        Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei;
      Combined_Experiment_ArmOnly_B.rtAction = -1;
      if ((Combined_Experiment_ArmOnly_P.platformSelection == 1.0) ||
          (Combined_Experiment_ArmOnly_P.platformSelection == 2.0) ||
          (Combined_Experiment_ArmOnly_P.platformSelection == 5.0)) {
        Combined_Experiment_ArmOnly_B.rtAction = 0;
      } else {
        if ((Combined_Experiment_ArmOnly_P.platformSelection == 3.0) ||
            (Combined_Experiment_ArmOnly_P.platformSelection == 4.0)) {
          Combined_Experiment_ArmOnly_B.rtAction = 1;
        }
      }

      Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei =
        Combined_Experiment_ArmOnly_B.rtAction;
      if (Combined_Experiment_ArmOnly_B.rtPrevAction !=
          Combined_Experiment_ArmOnly_B.rtAction) {
        switch (Combined_Experiment_ArmOnly_B.rtPrevAction) {
         case 0:
          // Disable for Enabled SubSystem: '<S77>/Enabled Subsystem'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j = false;
          }

          // End of Disable for SubSystem: '<S77>/Enabled Subsystem'

          // Disable for Enabled SubSystem: '<S77>/Enabled Subsystem1'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d = false;
          }

          // End of Disable for SubSystem: '<S77>/Enabled Subsystem1'

          // Disable for Enabled SubSystem: '<S138>/Enabled Subsystem'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m = false;
          }

          // End of Disable for SubSystem: '<S138>/Enabled Subsystem'
          break;

         case 1:
          // Disable for If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 
          Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_d1 = -1;

          // Disable for If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
          if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o ==
              0) {
            // Disable for Enabled SubSystem: '<S80>/Enabled Subsystem1'
            if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE = false;
            }

            // End of Disable for SubSystem: '<S80>/Enabled Subsystem1'

            // Disable for Enabled SubSystem: '<S106>/Enabled Subsystem'
            if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE = false;
            }

            // End of Disable for SubSystem: '<S106>/Enabled Subsystem'
          }

          Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o = -1;
          break;
        }
      }

      switch (Combined_Experiment_ArmOnly_B.rtAction) {
       case 0:
        // Outputs for IfAction SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' incorporates:
        //   ActionPort: '<S77>/Action Port'

        // MATLABSystem: '<S77>/Stream PhaseSpace to Platform'
        Combined_Experiment_ArmOnly_B.Sum6_l1 = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate;
        if (Combined_Experiment_ArmOnly_DW.obj_f.platformSelection !=
            Combined_Experiment_ArmOnly_P.platformSelection) {
          Combined_Experiment_ArmOnly_DW.obj_f.platformSelection =
            Combined_Experiment_ArmOnly_P.platformSelection;
        }

        if (Combined_Experiment_ArmOnly_DW.obj_f.PS_SampleRate !=
            Combined_Experiment_ArmOnly_B.Sum6_l1) {
          Combined_Experiment_ArmOnly_DW.obj_f.PS_SampleRate =
            Combined_Experiment_ArmOnly_B.Sum6_l1;
        }

        Combined_Experiment_ArmOnly_B.Sum6_l1 = 0.0;
        Combined_Experiment_ArmOnly_B.Sum6_dk = 0.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[3] = 0.0;
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 0.0;
        Combined_Experiment_ArmOnly_B.y5 = 0.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[6] = 0.0;
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0 = 0.0;
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1 = 0.0;
        Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2 = 0.0;
        Combined_Experiment_ArmOnly_B.y10 = 0.0;
        Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p = 0.0;
        Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_0 = 0.0;
        Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c = 0.0;
        stream_phasespace(&Combined_Experiment_ArmOnly_B.Sum6_l1,
                          &Combined_Experiment_ArmOnly_B.Sum6_dk,
                          &Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform
                          [3], &Combined_Experiment_ArmOnly_B.RED_Tz_Wrist,
                          &Combined_Experiment_ArmOnly_B.y5,
                          &Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform
                          [6],
                          &Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0,
                          &Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1,
                          &Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2,
                          &Combined_Experiment_ArmOnly_B.y10,
                          &Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p,
                          &Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_0,
                          &Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c,
                          Combined_Experiment_ArmOnly_DW.obj_f.platformSelection);
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[0] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_0 /
          Combined_Experiment_ArmOnly_DW.obj_f.PS_SampleRate;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[1] =
          Combined_Experiment_ArmOnly_B.Sum6_l1 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[2] =
          Combined_Experiment_ArmOnly_B.Sum6_dk / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[4] =
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[5] =
          Combined_Experiment_ArmOnly_B.y5 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[7] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_1 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[8] =
          Combined_Experiment_ArmOnly_B.VelocitytoPosition_d_idx_2 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[9] =
          Combined_Experiment_ArmOnly_B.y10 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[10] =
          Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_p / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[11] =
          Combined_Experiment_ArmOnly_B.rtb_x_ddot_idx_0 / 1000.0;
        Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[12] =
          Combined_Experiment_ArmOnly_B.rtb_TmpSignalConversionAtSFun_c / 1000.0;

        // End of MATLABSystem: '<S77>/Stream PhaseSpace to Platform'

        // RelationalOperator: '<S137>/Compare' incorporates:
        //   Constant: '<S137>/Constant'

        for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
             Combined_Experiment_ArmOnly_B.uElOffset1 < 13;
             Combined_Experiment_ArmOnly_B.uElOffset1++) {
          Combined_Experiment_ArmOnly_B.Compare_p[Combined_Experiment_ArmOnly_B.uElOffset1]
            =
            (Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[Combined_Experiment_ArmOnly_B.uElOffset1]
             != Combined_Experiment_ArmOnly_P.Constant_Value);
        }

        // End of RelationalOperator: '<S137>/Compare'

        // Outputs for Enabled SubSystem: '<S77>/Enabled Subsystem1' incorporates:
        //   EnablePort: '<S140>/Enable'

        // Outputs for Enabled SubSystem: '<S77>/Enabled Subsystem' incorporates:
        //   EnablePort: '<S139>/Enable'

        Combined_Experiment_ArmOnly_B.rtb_Compare_me =
          (Combined_Experiment_ArmOnly_B.Compare_p[0] ||
           Combined_Experiment_ArmOnly_B.Compare_p[1] ||
           Combined_Experiment_ArmOnly_B.Compare_p[2] ||
           Combined_Experiment_ArmOnly_B.Compare_p[3] ||
           Combined_Experiment_ArmOnly_B.Compare_p[4] ||
           Combined_Experiment_ArmOnly_B.Compare_p[5] ||
           Combined_Experiment_ArmOnly_B.Compare_p[6] ||
           Combined_Experiment_ArmOnly_B.Compare_p[7] ||
           Combined_Experiment_ArmOnly_B.Compare_p[8] ||
           Combined_Experiment_ArmOnly_B.Compare_p[9] ||
           Combined_Experiment_ArmOnly_B.Compare_p[10] ||
           Combined_Experiment_ArmOnly_B.Compare_p[11]);

        // End of Outputs for SubSystem: '<S77>/Enabled Subsystem1'
        if (Combined_Experiment_ArmOnly_B.rtb_Compare_me ||
            Combined_Experiment_ArmOnly_B.Compare_p[12]) {
          if (!Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j = true;
          }
        } else {
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j = false;
          }
        }

        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_j) {
          memcpy(&Combined_Experiment_ArmOnly_B.In1_b4[0],
                 &Combined_Experiment_ArmOnly_B.StreamPhaseSpacetoPlatform[0],
                 13U * sizeof(real_T));
        }

        // End of Outputs for SubSystem: '<S77>/Enabled Subsystem'

        // DataStoreWrite: '<S77>/Data Store Write3' incorporates:
        //   Inport: '<S139>/In1'
        //   SignalConversion generated from: '<S139>/Enable'

        Combined_Experiment_ArmOnly_DW.ARM_Elbow_Px =
          Combined_Experiment_ArmOnly_B.In1_b4[7];

        // DataStoreWrite: '<S77>/Data Store Write4'
        Combined_Experiment_ArmOnly_DW.ARM_Elbow_Py =
          Combined_Experiment_ArmOnly_B.In1_b4[8];

        // DataStoreWrite: '<S77>/Data Store Write5'
        Combined_Experiment_ArmOnly_DW.ARM_Wrist_Px =
          Combined_Experiment_ArmOnly_B.In1_b4[9];

        // DataStoreWrite: '<S77>/Data Store Write6'
        Combined_Experiment_ArmOnly_DW.ARM_Wrist_Py =
          Combined_Experiment_ArmOnly_B.In1_b4[10];

        // DataStoreWrite: '<S77>/Data Store Write7'
        Combined_Experiment_ArmOnly_DW.ARM_EndEff_Px =
          Combined_Experiment_ArmOnly_B.In1_b4[11];

        // DataStoreWrite: '<S77>/Data Store Write8'
        Combined_Experiment_ArmOnly_DW.ARM_EndEff_Py =
          Combined_Experiment_ArmOnly_B.In1_b4[12];

        // DataStoreWrite: '<S77>/RED_Px1'
        Combined_Experiment_ArmOnly_DW.RED_Px =
          Combined_Experiment_ArmOnly_B.In1_b4[1];

        // Outputs for Enabled SubSystem: '<S77>/Enabled Subsystem1' incorporates:
        //   EnablePort: '<S140>/Enable'

        if (Combined_Experiment_ArmOnly_B.rtb_Compare_me ||
            Combined_Experiment_ArmOnly_B.Compare_p[12]) {
          if (!Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d = true;
          }
        } else {
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d = false;
          }
        }

        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE_d) {
          Combined_Experiment_ArmOnly_B.In1_pl =
            Combined_Experiment_ArmOnly_M->Timing.t[0];
        }

        // End of Outputs for SubSystem: '<S77>/Enabled Subsystem1'

        // DataTypeConversion: '<S153>/Data Type Conversion' incorporates:
        //   Clock: '<S77>/Clock'
        //   Inport: '<S140>/In1'
        //   SignalConversion generated from: '<S140>/Enable'

        Combined_Experiment_ArmOnly_B.DataTypeConversion_h =
          (Combined_Experiment_ArmOnly_B.In1_pl != 0.0);

        // Outputs for Enabled SubSystem: '<S138>/Enabled Subsystem' incorporates:
        //   EnablePort: '<S154>/Enable'

        if (Combined_Experiment_ArmOnly_B.DataTypeConversion_h &&
            (!Combined_Experiment_ArmOnly_DW.Delay_DSTATE_o)) {
          if (!Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m = true;
          }
        } else {
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m = false;
          }
        }

        if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE_m) {
          Combined_Experiment_ArmOnly_B.In1_j1 =
            Combined_Experiment_ArmOnly_B.In1_pl;
        }

        // End of Outputs for SubSystem: '<S138>/Enabled Subsystem'

        // Sum: '<S138>/Subtract' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px10'
        //   Delay: '<S153>/Delay'
        //   Inport: '<S154>/In1'
        //   Logic: '<S153>/Logical Operator'
        //   Logic: '<S153>/Logical Operator1'

        Combined_Experiment_ArmOnly_DW.Univ_Time =
          Combined_Experiment_ArmOnly_B.In1_pl -
          Combined_Experiment_ArmOnly_B.In1_j1;

        // Delay: '<S147>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_dr != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ln =
            Combined_Experiment_ArmOnly_B.In1_b4[4];
        }

        // Sum: '<S147>/Sum6' incorporates:
        //   Delay: '<S147>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.In1_b4[4] -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ln;

        // If: '<S147>/if we went through a "step"' incorporates:
        //   Inport: '<S163>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S147>/Hold this value' incorporates:
          //   ActionPort: '<S163>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_d4 =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S147>/Hold this value'
        }

        // End of If: '<S147>/if we went through a "step"'

        // Gain: '<S147>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_p = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_d4;

        // Delay: '<S142>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_mq != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pv =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_p;
        }

        // Sum: '<S142>/Sum6' incorporates:
        //   Delay: '<S142>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_p -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pv;

        // If: '<S142>/if we went through a "step"' incorporates:
        //   Inport: '<S158>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S142>/Hold this value' incorporates:
          //   ActionPort: '<S158>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_ds =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S142>/Hold this value'
        }

        // End of If: '<S142>/if we went through a "step"'

        // Gain: '<S142>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px11'

        Combined_Experiment_ArmOnly_DW.BLACK_Ax = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_ds;

        // Delay: '<S148>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_lr != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mk =
            Combined_Experiment_ArmOnly_B.In1_b4[5];
        }

        // Sum: '<S148>/Sum6' incorporates:
        //   Delay: '<S148>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.In1_b4[5] -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mk;

        // If: '<S148>/if we went through a "step"' incorporates:
        //   Inport: '<S164>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S148>/Hold this value' incorporates:
          //   ActionPort: '<S164>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_ljk =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S148>/Hold this value'
        }

        // End of If: '<S148>/if we went through a "step"'

        // Gain: '<S148>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_nx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_ljk;

        // Delay: '<S143>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_b != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_le =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_nx;
        }

        // Sum: '<S143>/Sum6' incorporates:
        //   Delay: '<S143>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_nx -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_le;

        // If: '<S143>/if we went through a "step"' incorporates:
        //   Inport: '<S159>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S143>/Hold this value' incorporates:
          //   ActionPort: '<S159>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_dy =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S143>/Hold this value'
        }

        // End of If: '<S143>/if we went through a "step"'

        // Gain: '<S143>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px12'

        Combined_Experiment_ArmOnly_DW.BLACK_Ay = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_dy;

        // S-Function (sdspunwrap2): '<S77>/Unwrap1'
        if (Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_d) {
          Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_l =
            Combined_Experiment_ArmOnly_B.In1_b4[6];
          Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_d = false;
        }

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_a;
        Combined_Experiment_ArmOnly_B.Sum6_dk =
          Combined_Experiment_ArmOnly_B.In1_b4[6] -
          Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_l;
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
          Combined_Experiment_ArmOnly_B.Sum6_dk - floor
          ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
           6.2831853071795862) * 6.2831853071795862;
        if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931) &&
            (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
        }

        Combined_Experiment_ArmOnly_B.Sum6_dk =
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
          Combined_Experiment_ArmOnly_B.Sum6_dk;
        if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931) {
          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_a +
            Combined_Experiment_ArmOnly_B.Sum6_dk;
        }

        Combined_Experiment_ArmOnly_B.Unwrap1_c =
          Combined_Experiment_ArmOnly_B.In1_b4[6] +
          Combined_Experiment_ArmOnly_B.Sum6_l1;
        Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_l =
          Combined_Experiment_ArmOnly_B.In1_b4[6];
        Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_a =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of S-Function (sdspunwrap2): '<S77>/Unwrap1'

        // Delay: '<S149>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_aa != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_de =
            Combined_Experiment_ArmOnly_B.Unwrap1_c;
        }

        // Sum: '<S149>/Sum6' incorporates:
        //   Delay: '<S149>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Unwrap1_c -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_de;

        // If: '<S149>/if we went through a "step"' incorporates:
        //   Inport: '<S165>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S149>/Hold this value' incorporates:
          //   ActionPort: '<S165>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_awb =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S149>/Hold this value'
        }

        // End of If: '<S149>/if we went through a "step"'

        // Gain: '<S149>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_dx = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_awb;

        // Delay: '<S144>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_cw != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu2 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_dx;
        }

        // Sum: '<S144>/Sum6' incorporates:
        //   Delay: '<S144>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_dx -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu2;

        // If: '<S144>/if we went through a "step"' incorporates:
        //   Inport: '<S160>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S144>/Hold this value' incorporates:
          //   ActionPort: '<S160>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_lyk =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S144>/Hold this value'
        }

        // End of If: '<S144>/if we went through a "step"'

        // Gain: '<S144>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px13'

        Combined_Experiment_ArmOnly_DW.BLACK_RzDD = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_lyk;

        // Delay: '<S141>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_nsj != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b3 =
            Combined_Experiment_ArmOnly_B.In1_b4[1];
        }

        // Sum: '<S141>/Sum6' incorporates:
        //   Delay: '<S141>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.In1_b4[1] -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b3;

        // If: '<S141>/if we went through a "step"' incorporates:
        //   Inport: '<S157>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S141>/Hold this value' incorporates:
          //   ActionPort: '<S157>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_kd2 =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S141>/Hold this value'
        }

        // End of If: '<S141>/if we went through a "step"'

        // Gain: '<S141>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_ph = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_kd2;

        // DataStoreWrite: '<S77>/RED_Px2'
        Combined_Experiment_ArmOnly_DW.RED_Vx =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_ph;

        // DataStoreWrite: '<S77>/RED_Px3'
        Combined_Experiment_ArmOnly_DW.BLACK_Vx =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_p;

        // DataStoreWrite: '<S77>/RED_Px4'
        Combined_Experiment_ArmOnly_DW.BLACK_Px =
          Combined_Experiment_ArmOnly_B.In1_b4[4];

        // DataStoreWrite: '<S77>/RED_Px5'
        Combined_Experiment_ArmOnly_DW.BLACK_Py =
          Combined_Experiment_ArmOnly_B.In1_b4[5];

        // DataStoreWrite: '<S77>/RED_Px6'
        Combined_Experiment_ArmOnly_DW.BLACK_Rz =
          Combined_Experiment_ArmOnly_B.In1_b4[6];

        // Delay: '<S150>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_lrw != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ng =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_ph;
        }

        // Sum: '<S150>/Sum6' incorporates:
        //   Delay: '<S150>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_ph -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ng;

        // If: '<S150>/if we went through a "step"' incorporates:
        //   Inport: '<S166>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S150>/Hold this value' incorporates:
          //   ActionPort: '<S166>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_aw =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S150>/Hold this value'
        }

        // End of If: '<S150>/if we went through a "step"'

        // Gain: '<S150>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px7'

        Combined_Experiment_ArmOnly_DW.RED_Ax = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_aw;

        // Delay: '<S145>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_a2d != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m3 =
            Combined_Experiment_ArmOnly_B.In1_b4[2];
        }

        // Sum: '<S145>/Sum6' incorporates:
        //   Delay: '<S145>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.In1_b4[2] -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m3;

        // If: '<S145>/if we went through a "step"' incorporates:
        //   Inport: '<S161>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S145>/Hold this value' incorporates:
          //   ActionPort: '<S161>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_mm =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S145>/Hold this value'
        }

        // End of If: '<S145>/if we went through a "step"'

        // Gain: '<S145>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_pf = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_mm;

        // Delay: '<S151>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_mp != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fs =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_pf;
        }

        // Sum: '<S151>/Sum6' incorporates:
        //   Delay: '<S151>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_pf -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fs;

        // If: '<S151>/if we went through a "step"' incorporates:
        //   Inport: '<S167>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S151>/Hold this value' incorporates:
          //   ActionPort: '<S167>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_on =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S151>/Hold this value'
        }

        // End of If: '<S151>/if we went through a "step"'

        // Gain: '<S151>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px8'

        Combined_Experiment_ArmOnly_DW.RED_Ay = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_on;

        // S-Function (sdspunwrap2): '<S77>/Unwrap'
        if (Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_d) {
          Combined_Experiment_ArmOnly_DW.Unwrap_Prev_p =
            Combined_Experiment_ArmOnly_B.In1_b4[3];
          Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_d = false;
        }

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_o;
        Combined_Experiment_ArmOnly_B.Sum6_dk =
          Combined_Experiment_ArmOnly_B.In1_b4[3] -
          Combined_Experiment_ArmOnly_DW.Unwrap_Prev_p;
        Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
          Combined_Experiment_ArmOnly_B.Sum6_dk - floor
          ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
           6.2831853071795862) * 6.2831853071795862;
        if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931) &&
            (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
        }

        Combined_Experiment_ArmOnly_B.Sum6_dk =
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
          Combined_Experiment_ArmOnly_B.Sum6_dk;
        if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931) {
          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_o +
            Combined_Experiment_ArmOnly_B.Sum6_dk;
        }

        Combined_Experiment_ArmOnly_B.Unwrap_n =
          Combined_Experiment_ArmOnly_B.In1_b4[3] +
          Combined_Experiment_ArmOnly_B.Sum6_l1;
        Combined_Experiment_ArmOnly_DW.Unwrap_Prev_p =
          Combined_Experiment_ArmOnly_B.In1_b4[3];
        Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_o =
          Combined_Experiment_ArmOnly_B.Sum6_l1;

        // End of S-Function (sdspunwrap2): '<S77>/Unwrap'

        // Delay: '<S146>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_gb != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c4 =
            Combined_Experiment_ArmOnly_B.Unwrap_n;
        }

        // Sum: '<S146>/Sum6' incorporates:
        //   Delay: '<S146>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.Unwrap_n -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c4;

        // If: '<S146>/if we went through a "step"' incorporates:
        //   Inport: '<S162>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S146>/Hold this value' incorporates:
          //   ActionPort: '<S162>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_cp =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S146>/Hold this value'
        }

        // End of If: '<S146>/if we went through a "step"'

        // Gain: '<S146>/divide by delta T'
        Combined_Experiment_ArmOnly_B.dividebydeltaT_hu = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_cp;

        // Delay: '<S152>/Delay1'
        if (Combined_Experiment_ArmOnly_DW.icLoad_i != 0) {
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cz =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_hu;
        }

        // Sum: '<S152>/Sum6' incorporates:
        //   Delay: '<S152>/Delay1'

        Combined_Experiment_ArmOnly_B.Sum6_l1 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_hu -
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cz;

        // If: '<S152>/if we went through a "step"' incorporates:
        //   Inport: '<S168>/In1'

        if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
          // Outputs for IfAction SubSystem: '<S152>/Hold this value' incorporates:
          //   ActionPort: '<S168>/Action Port'

          Combined_Experiment_ArmOnly_B.In1_kt =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of Outputs for SubSystem: '<S152>/Hold this value'
        }

        // End of If: '<S152>/if we went through a "step"'

        // Gain: '<S152>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S77>/RED_Px9'

        Combined_Experiment_ArmOnly_DW.RED_RzDD = 1.0 /
          Combined_Experiment_ArmOnly_P.serverRate *
          Combined_Experiment_ArmOnly_B.In1_kt;

        // DataStoreWrite: '<S77>/RED_Py1'
        Combined_Experiment_ArmOnly_DW.RED_Py =
          Combined_Experiment_ArmOnly_B.In1_b4[2];

        // DataStoreWrite: '<S77>/RED_Py2'
        Combined_Experiment_ArmOnly_DW.RED_Vy =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_pf;

        // DataStoreWrite: '<S77>/RED_Py3'
        Combined_Experiment_ArmOnly_DW.BLACK_Vy =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_nx;

        // DataStoreWrite: '<S77>/RED_Rz1'
        Combined_Experiment_ArmOnly_DW.RED_Rz =
          Combined_Experiment_ArmOnly_B.In1_b4[3];

        // DataStoreWrite: '<S77>/RED_Rz2'
        Combined_Experiment_ArmOnly_DW.RED_RzD =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_hu;

        // DataStoreWrite: '<S77>/RED_Rz3'
        Combined_Experiment_ArmOnly_DW.BLACK_RzD =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_dx;

        // End of Outputs for SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 
        break;

       case 1:
        // Outputs for IfAction SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' incorporates:
        //   ActionPort: '<S76>/Action Port'

        // If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
        //   Constant: '<S76>/Constant'
        //   Constant: '<S78>/Constant'

        Combined_Experiment_ArmOnly_B.rtAction = -1;
        if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) ||
            (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
          Combined_Experiment_ArmOnly_B.rtAction = 0;

          // Outputs for IfAction SubSystem: '<S76>/Obtain BLACK States' incorporates:
          //   ActionPort: '<S79>/Action Port'

          // S-Function (sdspFromNetwork): '<S79>/UDP Receive'
          sErr = GetErrorBuffer
            (&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);
          Combined_Experiment_ArmOnly_B.uElOffset1 = 13;
          LibOutputs_Network
            (&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U],
             &Combined_Experiment_ArmOnly_B.UDPReceive_o1[0U],
             &Combined_Experiment_ArmOnly_B.uElOffset1);
          if (*sErr != 0) {
            rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
            rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
          }

          // End of S-Function (sdspFromNetwork): '<S79>/UDP Receive'

          // DataStoreWrite: '<S79>/Data Store Write3'
          Combined_Experiment_ArmOnly_DW.ARM_Elbow_Px =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[7];

          // DataStoreWrite: '<S79>/Data Store Write4'
          Combined_Experiment_ArmOnly_DW.ARM_Elbow_Py =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[8];

          // DataStoreWrite: '<S79>/Data Store Write5'
          Combined_Experiment_ArmOnly_DW.ARM_Wrist_Px =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[9];

          // DataStoreWrite: '<S79>/Data Store Write6'
          Combined_Experiment_ArmOnly_DW.ARM_Wrist_Py =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[10];

          // DataStoreWrite: '<S79>/Data Store Write7'
          Combined_Experiment_ArmOnly_DW.ARM_EndEff_Px =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[11];

          // DataStoreWrite: '<S79>/Data Store Write8'
          Combined_Experiment_ArmOnly_DW.ARM_EndEff_Py =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[12];

          // DataStoreWrite: '<S79>/RED_Px1'
          Combined_Experiment_ArmOnly_DW.RED_Px =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[1];

          // DataStoreWrite: '<S79>/RED_Px10'
          Combined_Experiment_ArmOnly_DW.Univ_Time =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[0];

          // Delay: '<S87>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ky1 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_nw =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[4];
          }

          // Sum: '<S87>/Sum6' incorporates:
          //   Delay: '<S87>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[4] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_nw;

          // If: '<S87>/if we went through a "step"' incorporates:
          //   Inport: '<S99>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S87>/Hold this value' incorporates:
            //   ActionPort: '<S99>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_dn =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S87>/Hold this value'
          }

          // End of If: '<S87>/if we went through a "step"'

          // Gain: '<S87>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_m = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_dn;

          // Delay: '<S82>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_of != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gx =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_m;
          }

          // Sum: '<S82>/Sum6' incorporates:
          //   Delay: '<S82>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_m -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gx;

          // If: '<S82>/if we went through a "step"' incorporates:
          //   Inport: '<S94>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S82>/Hold this value' incorporates:
            //   ActionPort: '<S94>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_fg =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S82>/Hold this value'
          }

          // End of If: '<S82>/if we went through a "step"'

          // Gain: '<S82>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px11'

          Combined_Experiment_ArmOnly_DW.BLACK_Ax = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_fg;

          // Delay: '<S88>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_oi != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct1 =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[5];
          }

          // Sum: '<S88>/Sum6' incorporates:
          //   Delay: '<S88>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[5] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct1;

          // If: '<S88>/if we went through a "step"' incorporates:
          //   Inport: '<S100>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S88>/Hold this value' incorporates:
            //   ActionPort: '<S100>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_jk =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S88>/Hold this value'
          }

          // End of If: '<S88>/if we went through a "step"'

          // Gain: '<S88>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_me = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_jk;

          // Delay: '<S83>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_hb != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i0 =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_me;
          }

          // Sum: '<S83>/Sum6' incorporates:
          //   Delay: '<S83>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_me -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i0;

          // If: '<S83>/if we went through a "step"' incorporates:
          //   Inport: '<S95>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S83>/Hold this value' incorporates:
            //   ActionPort: '<S95>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_jj =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S83>/Hold this value'
          }

          // End of If: '<S83>/if we went through a "step"'

          // Gain: '<S83>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px12'

          Combined_Experiment_ArmOnly_DW.BLACK_Ay = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_jj;

          // S-Function (sdspunwrap2): '<S79>/Unwrap1'
          if (Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_o) {
            Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_p =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[6];
            Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_o = false;
          }

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_n;
          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[6] -
            Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_p;
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
            Combined_Experiment_ArmOnly_B.Sum6_dk - floor
            ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
             6.2831853071795862) * 6.2831853071795862;
          if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931)
              && (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
          }

          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
            Combined_Experiment_ArmOnly_B.Sum6_dk;
          if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931)
          {
            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_n +
              Combined_Experiment_ArmOnly_B.Sum6_dk;
          }

          Combined_Experiment_ArmOnly_DW.Unwrap1_Prev_p =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[6];
          Combined_Experiment_ArmOnly_B.Unwrap1_l =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[6] +
            Combined_Experiment_ArmOnly_B.Sum6_l1;
          Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum_n =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of S-Function (sdspunwrap2): '<S79>/Unwrap1'

          // Delay: '<S89>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ap != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n0 =
              Combined_Experiment_ArmOnly_B.Unwrap1_l;
          }

          // Sum: '<S89>/Sum6' incorporates:
          //   Delay: '<S89>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Unwrap1_l -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n0;

          // If: '<S89>/if we went through a "step"' incorporates:
          //   Inport: '<S101>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S89>/Hold this value' incorporates:
            //   ActionPort: '<S101>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_fr =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S89>/Hold this value'
          }

          // End of If: '<S89>/if we went through a "step"'

          // Gain: '<S89>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_g = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_fr;

          // Delay: '<S84>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_hc != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_da =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_g;
          }

          // Sum: '<S84>/Sum6' incorporates:
          //   Delay: '<S84>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_g -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_da;

          // If: '<S84>/if we went through a "step"' incorporates:
          //   Inport: '<S96>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S84>/Hold this value' incorporates:
            //   ActionPort: '<S96>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_gw =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S84>/Hold this value'
          }

          // End of If: '<S84>/if we went through a "step"'

          // Gain: '<S84>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px13'

          Combined_Experiment_ArmOnly_DW.BLACK_RzDD = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_gw;

          // Delay: '<S81>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_nn != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m1 =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[1];
          }

          // Sum: '<S81>/Sum6' incorporates:
          //   Delay: '<S81>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[1] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m1;

          // If: '<S81>/if we went through a "step"' incorporates:
          //   Inport: '<S93>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S81>/Hold this value' incorporates:
            //   ActionPort: '<S93>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_kw =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S81>/Hold this value'
          }

          // End of If: '<S81>/if we went through a "step"'

          // Gain: '<S81>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_hw = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_kw;

          // DataStoreWrite: '<S79>/RED_Px2'
          Combined_Experiment_ArmOnly_DW.RED_Vx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_hw;

          // DataStoreWrite: '<S79>/RED_Px3'
          Combined_Experiment_ArmOnly_DW.BLACK_Vx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_m;

          // DataStoreWrite: '<S79>/RED_Px4'
          Combined_Experiment_ArmOnly_DW.BLACK_Px =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[4];

          // DataStoreWrite: '<S79>/RED_Px5'
          Combined_Experiment_ArmOnly_DW.BLACK_Py =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[5];

          // DataStoreWrite: '<S79>/RED_Px6'
          Combined_Experiment_ArmOnly_DW.BLACK_Rz =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[6];

          // Delay: '<S90>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_gc != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bo =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_hw;
          }

          // Sum: '<S90>/Sum6' incorporates:
          //   Delay: '<S90>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_hw -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bo;

          // If: '<S90>/if we went through a "step"' incorporates:
          //   Inport: '<S102>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S90>/Hold this value' incorporates:
            //   ActionPort: '<S102>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_df =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S90>/Hold this value'
          }

          // End of If: '<S90>/if we went through a "step"'

          // Gain: '<S90>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px7'

          Combined_Experiment_ArmOnly_DW.RED_Ax = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_df;

          // Delay: '<S85>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_d0 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ed =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[2];
          }

          // Sum: '<S85>/Sum6' incorporates:
          //   Delay: '<S85>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[2] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ed;

          // If: '<S85>/if we went through a "step"' incorporates:
          //   Inport: '<S97>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S85>/Hold this value' incorporates:
            //   ActionPort: '<S97>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_jp3 =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S85>/Hold this value'
          }

          // End of If: '<S85>/if we went through a "step"'

          // Gain: '<S85>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_d = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_jp3;

          // Delay: '<S91>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_a1a != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k5 =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_d;
          }

          // Sum: '<S91>/Sum6' incorporates:
          //   Delay: '<S91>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_d -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k5;

          // If: '<S91>/if we went through a "step"' incorporates:
          //   Inport: '<S103>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S91>/Hold this value' incorporates:
            //   ActionPort: '<S103>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_dr =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S91>/Hold this value'
          }

          // End of If: '<S91>/if we went through a "step"'

          // Gain: '<S91>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px8'

          Combined_Experiment_ArmOnly_DW.RED_Ay = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_dr;

          // S-Function (sdspunwrap2): '<S79>/Unwrap'
          if (Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_p) {
            Combined_Experiment_ArmOnly_DW.Unwrap_Prev_f =
              Combined_Experiment_ArmOnly_B.UDPReceive_o1[3];
            Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_p = false;
          }

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_k;
          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[3] -
            Combined_Experiment_ArmOnly_DW.Unwrap_Prev_f;
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
            Combined_Experiment_ArmOnly_B.Sum6_dk - floor
            ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
             6.2831853071795862) * 6.2831853071795862;
          if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931)
              && (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
          }

          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
            Combined_Experiment_ArmOnly_B.Sum6_dk;
          if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931)
          {
            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_k +
              Combined_Experiment_ArmOnly_B.Sum6_dk;
          }

          Combined_Experiment_ArmOnly_DW.Unwrap_Prev_f =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[3];
          Combined_Experiment_ArmOnly_B.Unwrap_l =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[3] +
            Combined_Experiment_ArmOnly_B.Sum6_l1;
          Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum_k =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of S-Function (sdspunwrap2): '<S79>/Unwrap'

          // Delay: '<S86>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ms != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p4 =
              Combined_Experiment_ArmOnly_B.Unwrap_l;
          }

          // Sum: '<S86>/Sum6' incorporates:
          //   Delay: '<S86>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Unwrap_l -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p4;

          // If: '<S86>/if we went through a "step"' incorporates:
          //   Inport: '<S98>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S86>/Hold this value' incorporates:
            //   ActionPort: '<S98>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_cxn =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S86>/Hold this value'
          }

          // End of If: '<S86>/if we went through a "step"'

          // Gain: '<S86>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_n = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_cxn;

          // Delay: '<S92>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_gi != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fr =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_n;
          }

          // Sum: '<S92>/Sum6' incorporates:
          //   Delay: '<S92>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_n -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fr;

          // If: '<S92>/if we went through a "step"' incorporates:
          //   Inport: '<S104>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S92>/Hold this value' incorporates:
            //   ActionPort: '<S104>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_oi =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S92>/Hold this value'
          }

          // End of If: '<S92>/if we went through a "step"'

          // Gain: '<S92>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S79>/RED_Px9'

          Combined_Experiment_ArmOnly_DW.RED_RzDD = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_oi;

          // DataStoreWrite: '<S79>/RED_Py1'
          Combined_Experiment_ArmOnly_DW.RED_Py =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[2];

          // DataStoreWrite: '<S79>/RED_Py2'
          Combined_Experiment_ArmOnly_DW.RED_Vy =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_d;

          // DataStoreWrite: '<S79>/RED_Py3'
          Combined_Experiment_ArmOnly_DW.BLACK_Vy =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_me;

          // DataStoreWrite: '<S79>/RED_Rz1'
          Combined_Experiment_ArmOnly_DW.RED_Rz =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[3];

          // DataStoreWrite: '<S79>/RED_Rz2'
          Combined_Experiment_ArmOnly_DW.RED_RzD =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_n;

          // DataStoreWrite: '<S79>/RED_Rz3'
          Combined_Experiment_ArmOnly_DW.BLACK_RzD =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_g;

          // End of Outputs for SubSystem: '<S76>/Obtain BLACK States'
        }

        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_d1 =
          Combined_Experiment_ArmOnly_B.rtAction;

        // End of If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 

        // If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
        //   Clock: '<S80>/Clock'
        //   Constant: '<S76>/Constant'
        //   Constant: '<S78>/Constant'
        //   Delay: '<S121>/Delay'
        //   Inport: '<S107>/In1'
        //   Inport: '<S108>/In1'
        //   Inport: '<S122>/In1'
        //   Logic: '<S121>/Logical Operator'
        //   Logic: '<S121>/Logical Operator1'

        Combined_Experiment_ArmOnly_B.rtPrevAction =
          Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o;
        Combined_Experiment_ArmOnly_B.rtAction = -1;
        if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
            (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
          Combined_Experiment_ArmOnly_B.rtAction = 0;
        }

        Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o =
          Combined_Experiment_ArmOnly_B.rtAction;
        if ((Combined_Experiment_ArmOnly_B.rtPrevAction !=
             Combined_Experiment_ArmOnly_B.rtAction) &&
            (Combined_Experiment_ArmOnly_B.rtPrevAction == 0)) {
          // Disable for Enabled SubSystem: '<S80>/Enabled Subsystem1'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE = false;
          }

          // End of Disable for SubSystem: '<S80>/Enabled Subsystem1'

          // Disable for Enabled SubSystem: '<S106>/Enabled Subsystem'
          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
            Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE = false;
          }

          // End of Disable for SubSystem: '<S106>/Enabled Subsystem'
        }

        if (Combined_Experiment_ArmOnly_B.rtAction == 0) {
          // Outputs for IfAction SubSystem: '<S76>/Obtain RED States' incorporates:
          //   ActionPort: '<S80>/Action Port'

          // RelationalOperator: '<S105>/Compare' incorporates:
          //   Constant: '<S105>/Constant'

          for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
               Combined_Experiment_ArmOnly_B.uElOffset1 < 13;
               Combined_Experiment_ArmOnly_B.uElOffset1++) {
            Combined_Experiment_ArmOnly_B.Compare_p[Combined_Experiment_ArmOnly_B.uElOffset1]
              = (0.0 != Combined_Experiment_ArmOnly_P.Constant_Value_p);
          }

          // End of RelationalOperator: '<S105>/Compare'

          // Outputs for Enabled SubSystem: '<S80>/Enabled Subsystem1' incorporates:
          //   EnablePort: '<S108>/Enable'

          // Outputs for Enabled SubSystem: '<S80>/Enabled Subsystem' incorporates:
          //   EnablePort: '<S107>/Enable'

          Combined_Experiment_ArmOnly_B.rtb_Compare_me =
            (Combined_Experiment_ArmOnly_B.Compare_p[0] ||
             Combined_Experiment_ArmOnly_B.Compare_p[1] ||
             Combined_Experiment_ArmOnly_B.Compare_p[2] ||
             Combined_Experiment_ArmOnly_B.Compare_p[3] ||
             Combined_Experiment_ArmOnly_B.Compare_p[4] ||
             Combined_Experiment_ArmOnly_B.Compare_p[5] ||
             Combined_Experiment_ArmOnly_B.Compare_p[6] ||
             Combined_Experiment_ArmOnly_B.Compare_p[7] ||
             Combined_Experiment_ArmOnly_B.Compare_p[8] ||
             Combined_Experiment_ArmOnly_B.Compare_p[9] ||
             Combined_Experiment_ArmOnly_B.Compare_p[10] ||
             Combined_Experiment_ArmOnly_B.Compare_p[11]);

          // End of Outputs for SubSystem: '<S80>/Enabled Subsystem1'
          if (Combined_Experiment_ArmOnly_B.rtb_Compare_me ||
              Combined_Experiment_ArmOnly_B.Compare_p[12]) {
            memset(&Combined_Experiment_ArmOnly_B.In1_n1[0], 0, 13U * sizeof
                   (real_T));
          }

          // End of Outputs for SubSystem: '<S80>/Enabled Subsystem'

          // DataStoreWrite: '<S80>/Data Store Write3' incorporates:
          //   Inport: '<S107>/In1'

          Combined_Experiment_ArmOnly_DW.ARM_Elbow_Px =
            Combined_Experiment_ArmOnly_B.In1_n1[7];

          // DataStoreWrite: '<S80>/Data Store Write4'
          Combined_Experiment_ArmOnly_DW.ARM_Elbow_Py =
            Combined_Experiment_ArmOnly_B.In1_n1[8];

          // DataStoreWrite: '<S80>/Data Store Write5'
          Combined_Experiment_ArmOnly_DW.ARM_Wrist_Px =
            Combined_Experiment_ArmOnly_B.In1_n1[9];

          // DataStoreWrite: '<S80>/Data Store Write6'
          Combined_Experiment_ArmOnly_DW.ARM_Wrist_Py =
            Combined_Experiment_ArmOnly_B.In1_n1[10];

          // DataStoreWrite: '<S80>/Data Store Write7'
          Combined_Experiment_ArmOnly_DW.ARM_EndEff_Px =
            Combined_Experiment_ArmOnly_B.In1_n1[11];

          // DataStoreWrite: '<S80>/Data Store Write8'
          Combined_Experiment_ArmOnly_DW.ARM_EndEff_Py =
            Combined_Experiment_ArmOnly_B.In1_n1[12];

          // DataStoreWrite: '<S80>/RED_Px1'
          Combined_Experiment_ArmOnly_DW.RED_Px =
            Combined_Experiment_ArmOnly_B.In1_n1[1];

          // Outputs for Enabled SubSystem: '<S80>/Enabled Subsystem1' incorporates:
          //   EnablePort: '<S108>/Enable'

          if (Combined_Experiment_ArmOnly_B.rtb_Compare_me ||
              Combined_Experiment_ArmOnly_B.Compare_p[12]) {
            if (!Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE = true;
            }
          } else {
            if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE = false;
            }
          }

          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem1_MODE) {
            Combined_Experiment_ArmOnly_B.In1_nh =
              Combined_Experiment_ArmOnly_M->Timing.t[0];
          }

          // End of Outputs for SubSystem: '<S80>/Enabled Subsystem1'

          // DataTypeConversion: '<S121>/Data Type Conversion' incorporates:
          //   Clock: '<S80>/Clock'
          //   Inport: '<S108>/In1'

          Combined_Experiment_ArmOnly_B.DataTypeConversion =
            (Combined_Experiment_ArmOnly_B.In1_nh != 0.0);

          // Outputs for Enabled SubSystem: '<S106>/Enabled Subsystem' incorporates:
          //   EnablePort: '<S122>/Enable'

          if (Combined_Experiment_ArmOnly_B.DataTypeConversion &&
              (!Combined_Experiment_ArmOnly_DW.Delay_DSTATE_p)) {
            if (!Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE = true;
            }
          } else {
            if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
              Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE = false;
            }
          }

          if (Combined_Experiment_ArmOnly_DW.EnabledSubsystem_MODE) {
            Combined_Experiment_ArmOnly_B.In1_it =
              Combined_Experiment_ArmOnly_B.In1_nh;
          }

          // End of Outputs for SubSystem: '<S106>/Enabled Subsystem'

          // Sum: '<S106>/Subtract' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px10'
          //   Delay: '<S121>/Delay'
          //   Inport: '<S122>/In1'
          //   Logic: '<S121>/Logical Operator'
          //   Logic: '<S121>/Logical Operator1'

          Combined_Experiment_ArmOnly_DW.Univ_Time =
            Combined_Experiment_ArmOnly_B.In1_nh -
            Combined_Experiment_ArmOnly_B.In1_it;

          // Delay: '<S115>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_kc != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p3 =
              Combined_Experiment_ArmOnly_B.In1_n1[4];
          }

          // Sum: '<S115>/Sum6' incorporates:
          //   Delay: '<S115>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.In1_n1[4] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p3;

          // If: '<S115>/if we went through a "step"' incorporates:
          //   Inport: '<S131>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S115>/Hold this value' incorporates:
            //   ActionPort: '<S131>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_n5 =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S115>/Hold this value'
          }

          // End of If: '<S115>/if we went through a "step"'

          // Gain: '<S115>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_n5;

          // Delay: '<S110>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_p1 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bk =
              Combined_Experiment_ArmOnly_B.dividebydeltaT;
          }

          // Sum: '<S110>/Sum6' incorporates:
          //   Delay: '<S110>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bk;

          // If: '<S110>/if we went through a "step"' incorporates:
          //   Inport: '<S126>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S110>/Hold this value' incorporates:
            //   ActionPort: '<S126>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_av =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S110>/Hold this value'
          }

          // End of If: '<S110>/if we went through a "step"'

          // Gain: '<S110>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px11'

          Combined_Experiment_ArmOnly_DW.BLACK_Ax = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_av;

          // Delay: '<S116>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_kv != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ko =
              Combined_Experiment_ArmOnly_B.In1_n1[5];
          }

          // Sum: '<S116>/Sum6' incorporates:
          //   Delay: '<S116>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.In1_n1[5] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ko;

          // If: '<S116>/if we went through a "step"' incorporates:
          //   Inport: '<S132>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S116>/Hold this value' incorporates:
            //   ActionPort: '<S132>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_bi =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S116>/Hold this value'
          }

          // End of If: '<S116>/if we went through a "step"'

          // Gain: '<S116>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_l = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_bi;

          // Delay: '<S111>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_cg != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kx =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_l;
          }

          // Sum: '<S111>/Sum6' incorporates:
          //   Delay: '<S111>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kx;

          // If: '<S111>/if we went through a "step"' incorporates:
          //   Inport: '<S127>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S111>/Hold this value' incorporates:
            //   ActionPort: '<S127>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_ox =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S111>/Hold this value'
          }

          // End of If: '<S111>/if we went through a "step"'

          // Gain: '<S111>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px12'

          Combined_Experiment_ArmOnly_DW.BLACK_Ay = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_ox;

          // S-Function (sdspunwrap2): '<S80>/Unwrap1'
          if (Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep) {
            Combined_Experiment_ArmOnly_DW.Unwrap1_Prev =
              Combined_Experiment_ArmOnly_B.In1_n1[6];
            Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep = false;
          }

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum;
          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.In1_n1[6] -
            Combined_Experiment_ArmOnly_DW.Unwrap1_Prev;
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
            Combined_Experiment_ArmOnly_B.Sum6_dk - floor
            ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
             6.2831853071795862) * 6.2831853071795862;
          if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931)
              && (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
          }

          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
            Combined_Experiment_ArmOnly_B.Sum6_dk;
          if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931)
          {
            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum +
              Combined_Experiment_ArmOnly_B.Sum6_dk;
          }

          Combined_Experiment_ArmOnly_B.Unwrap1 =
            Combined_Experiment_ArmOnly_B.In1_n1[6] +
            Combined_Experiment_ArmOnly_B.Sum6_l1;
          Combined_Experiment_ArmOnly_DW.Unwrap1_Prev =
            Combined_Experiment_ArmOnly_B.In1_n1[6];
          Combined_Experiment_ArmOnly_DW.Unwrap1_Cumsum =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of S-Function (sdspunwrap2): '<S80>/Unwrap1'

          // Delay: '<S117>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_f0 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_at =
              Combined_Experiment_ArmOnly_B.Unwrap1;
          }

          // Sum: '<S117>/Sum6' incorporates:
          //   Delay: '<S117>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Unwrap1 -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_at;

          // If: '<S117>/if we went through a "step"' incorporates:
          //   Inport: '<S133>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S117>/Hold this value' incorporates:
            //   ActionPort: '<S133>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_hs =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S117>/Hold this value'
          }

          // End of If: '<S117>/if we went through a "step"'

          // Gain: '<S117>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_h = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_hs;

          // Delay: '<S112>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_a5 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hg =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_h;
          }

          // Sum: '<S112>/Sum6' incorporates:
          //   Delay: '<S112>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_h -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hg;

          // If: '<S112>/if we went through a "step"' incorporates:
          //   Inport: '<S128>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S112>/Hold this value' incorporates:
            //   ActionPort: '<S128>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_kg =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S112>/Hold this value'
          }

          // End of If: '<S112>/if we went through a "step"'

          // Gain: '<S112>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px13'

          Combined_Experiment_ArmOnly_DW.BLACK_RzDD = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_kg;

          // Delay: '<S109>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_l4 != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_knv =
              Combined_Experiment_ArmOnly_B.In1_n1[1];
          }

          // Sum: '<S109>/Sum6' incorporates:
          //   Delay: '<S109>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.In1_n1[1] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_knv;

          // If: '<S109>/if we went through a "step"' incorporates:
          //   Inport: '<S125>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S109>/Hold this value' incorporates:
            //   ActionPort: '<S125>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_gd =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S109>/Hold this value'
          }

          // End of If: '<S109>/if we went through a "step"'

          // Gain: '<S109>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_a = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_gd;

          // DataStoreWrite: '<S80>/RED_Px2'
          Combined_Experiment_ArmOnly_DW.RED_Vx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_a;

          // DataStoreWrite: '<S80>/RED_Px3'
          Combined_Experiment_ArmOnly_DW.BLACK_Vx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT;

          // DataStoreWrite: '<S80>/RED_Px4'
          Combined_Experiment_ArmOnly_DW.BLACK_Px =
            Combined_Experiment_ArmOnly_B.In1_n1[4];

          // DataStoreWrite: '<S80>/RED_Px5'
          Combined_Experiment_ArmOnly_DW.BLACK_Py =
            Combined_Experiment_ArmOnly_B.In1_n1[5];

          // DataStoreWrite: '<S80>/RED_Px6'
          Combined_Experiment_ArmOnly_DW.BLACK_Rz =
            Combined_Experiment_ArmOnly_B.In1_n1[6];

          // Delay: '<S118>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_go != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_oz =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_a;
          }

          // Sum: '<S118>/Sum6' incorporates:
          //   Delay: '<S118>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_a -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_oz;

          // If: '<S118>/if we went through a "step"' incorporates:
          //   Inport: '<S134>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S118>/Hold this value' incorporates:
            //   ActionPort: '<S134>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_lj =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S118>/Hold this value'
          }

          // End of If: '<S118>/if we went through a "step"'

          // Gain: '<S118>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px7'

          Combined_Experiment_ArmOnly_DW.RED_Ax = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_lj;

          // Delay: '<S113>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ay != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cf =
              Combined_Experiment_ArmOnly_B.In1_n1[2];
          }

          // Sum: '<S113>/Sum6' incorporates:
          //   Delay: '<S113>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.In1_n1[2] -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cf;

          // If: '<S113>/if we went through a "step"' incorporates:
          //   Inport: '<S129>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S113>/Hold this value' incorporates:
            //   ActionPort: '<S129>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_i2 =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S113>/Hold this value'
          }

          // End of If: '<S113>/if we went through a "step"'

          // Gain: '<S113>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_l4 = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_i2;

          // Delay: '<S119>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_aqe != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_me =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_l4;
          }

          // Sum: '<S119>/Sum6' incorporates:
          //   Delay: '<S119>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l4 -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_me;

          // If: '<S119>/if we went through a "step"' incorporates:
          //   Inport: '<S135>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S119>/Hold this value' incorporates:
            //   ActionPort: '<S135>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_mj =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S119>/Hold this value'
          }

          // End of If: '<S119>/if we went through a "step"'

          // Gain: '<S119>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px8'

          Combined_Experiment_ArmOnly_DW.RED_Ay = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_mj;

          // S-Function (sdspunwrap2): '<S80>/Unwrap'
          if (Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep) {
            Combined_Experiment_ArmOnly_DW.Unwrap_Prev =
              Combined_Experiment_ArmOnly_B.In1_n1[3];
            Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep = false;
          }

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum;
          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.In1_n1[3] -
            Combined_Experiment_ArmOnly_DW.Unwrap_Prev;
          Combined_Experiment_ArmOnly_B.RED_Tz_Wrist =
            Combined_Experiment_ArmOnly_B.Sum6_dk - floor
            ((Combined_Experiment_ArmOnly_B.Sum6_dk + 3.1415926535897931) /
             6.2831853071795862) * 6.2831853071795862;
          if ((Combined_Experiment_ArmOnly_B.RED_Tz_Wrist == -3.1415926535897931)
              && (Combined_Experiment_ArmOnly_B.Sum6_dk > 0.0)) {
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist = 3.1415926535897931;
          }

          Combined_Experiment_ArmOnly_B.Sum6_dk =
            Combined_Experiment_ArmOnly_B.RED_Tz_Wrist -
            Combined_Experiment_ArmOnly_B.Sum6_dk;
          if (fabs(Combined_Experiment_ArmOnly_B.Sum6_dk) > 3.1415926535897931)
          {
            Combined_Experiment_ArmOnly_B.Sum6_l1 =
              Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum +
              Combined_Experiment_ArmOnly_B.Sum6_dk;
          }

          Combined_Experiment_ArmOnly_B.Unwrap =
            Combined_Experiment_ArmOnly_B.In1_n1[3] +
            Combined_Experiment_ArmOnly_B.Sum6_l1;
          Combined_Experiment_ArmOnly_DW.Unwrap_Prev =
            Combined_Experiment_ArmOnly_B.In1_n1[3];
          Combined_Experiment_ArmOnly_DW.Unwrap_Cumsum =
            Combined_Experiment_ArmOnly_B.Sum6_l1;

          // End of S-Function (sdspunwrap2): '<S80>/Unwrap'

          // Delay: '<S114>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_ma != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gy =
              Combined_Experiment_ArmOnly_B.Unwrap;
          }

          // Sum: '<S114>/Sum6' incorporates:
          //   Delay: '<S114>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.Unwrap -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gy;

          // If: '<S114>/if we went through a "step"' incorporates:
          //   Inport: '<S130>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S114>/Hold this value' incorporates:
            //   ActionPort: '<S130>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_mc =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S114>/Hold this value'
          }

          // End of If: '<S114>/if we went through a "step"'

          // Gain: '<S114>/divide by delta T'
          Combined_Experiment_ArmOnly_B.dividebydeltaT_f = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_mc;

          // Delay: '<S120>/Delay1'
          if (Combined_Experiment_ArmOnly_DW.icLoad_hj != 0) {
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hf =
              Combined_Experiment_ArmOnly_B.dividebydeltaT_f;
          }

          // Sum: '<S120>/Sum6' incorporates:
          //   Delay: '<S120>/Delay1'

          Combined_Experiment_ArmOnly_B.Sum6_l1 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_f -
            Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hf;

          // If: '<S120>/if we went through a "step"' incorporates:
          //   Inport: '<S136>/In1'

          if (Combined_Experiment_ArmOnly_B.Sum6_l1 != 0.0) {
            // Outputs for IfAction SubSystem: '<S120>/Hold this value' incorporates:
            //   ActionPort: '<S136>/Action Port'

            Combined_Experiment_ArmOnly_B.In1_pb =
              Combined_Experiment_ArmOnly_B.Sum6_l1;

            // End of Outputs for SubSystem: '<S120>/Hold this value'
          }

          // End of If: '<S120>/if we went through a "step"'

          // Gain: '<S120>/divide by delta T' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px9'

          Combined_Experiment_ArmOnly_DW.RED_RzDD = 1.0 /
            Combined_Experiment_ArmOnly_P.serverRate *
            Combined_Experiment_ArmOnly_B.In1_pb;

          // DataStoreWrite: '<S80>/RED_Py1'
          Combined_Experiment_ArmOnly_DW.RED_Py =
            Combined_Experiment_ArmOnly_B.In1_n1[2];

          // DataStoreWrite: '<S80>/RED_Py2'
          Combined_Experiment_ArmOnly_DW.RED_Vy =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l4;

          // DataStoreWrite: '<S80>/RED_Py3'
          Combined_Experiment_ArmOnly_DW.BLACK_Vy =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l;

          // DataStoreWrite: '<S80>/RED_Rz1'
          Combined_Experiment_ArmOnly_DW.RED_Rz =
            Combined_Experiment_ArmOnly_B.In1_n1[3];

          // DataStoreWrite: '<S80>/RED_Rz2'
          Combined_Experiment_ArmOnly_DW.RED_RzD =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_f;

          // DataStoreWrite: '<S80>/RED_Rz3'
          Combined_Experiment_ArmOnly_DW.BLACK_RzD =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_h;

          // SignalConversion generated from: '<S80>/Send BLACK States to  BLACK Platform' incorporates:
          //   DataStoreWrite: '<S80>/RED_Px10'

          Combined_Experiment_ArmOnly_B.TmpSignalConversionAtSendBLACKS[0] =
            Combined_Experiment_ArmOnly_DW.Univ_Time;
          memcpy(&Combined_Experiment_ArmOnly_B.TmpSignalConversionAtSendBLACKS
                 [1], &Combined_Experiment_ArmOnly_B.In1_n1[1], 12U * sizeof
                 (real_T));

          // End of Outputs for SubSystem: '<S76>/Obtain RED States'
        }

        // End of If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
        // End of Outputs for SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
        break;
      }

      // End of If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
      // End of Outputs for SubSystem: '<S5>/Use Hardware to Obtain States'
      break;

     case 1:
      // Outputs for IfAction SubSystem: '<S5>/Initialize Universal Time (Simulation)' incorporates:
      //   ActionPort: '<S73>/Action Port'

      // Clock: '<S73>/Set Universal Time (If this is a simulation)' incorporates:
      //   DataStoreWrite: '<S73>/Universal_Time'

      Combined_Experiment_ArmOnly_DW.Univ_Time =
        Combined_Experiment_ArmOnly_M->Timing.t[0];

      // End of Outputs for SubSystem: '<S5>/Initialize Universal Time (Simulation)' 
      break;
    }

    // End of If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
    Combined_Experiment_ArmOnly_B.status = 24U;
    memcpy((void *)&Combined_Experiment_ArmOnly_B.SwappedDataBytes, (void *)
           &Combined_Experiment_ArmOnly_B.status, (uint32_T)((size_t)1 * sizeof
            (uint8_T)));
    Combined_Experiment_ArmOnly_B.status = MW_I2C_MasterWrite
      (Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
       &Combined_Experiment_ArmOnly_B.SwappedDataBytes, 1U, true, false);
    if (0 == Combined_Experiment_ArmOnly_B.status) {
      MW_I2C_MasterRead
        (Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
         Combined_Experiment_ArmOnly_B.output_raw, 6U, false, true);
      memcpy((void *)&Combined_Experiment_ArmOnly_B.b_RegisterValue[0], (void *)
             &Combined_Experiment_ArmOnly_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Combined_Experiment_ArmOnly_B.b_RegisterValue[0] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[1] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Combined_Experiment_ArmOnly_B.b_c[0],
           &Combined_Experiment_ArmOnly_DW.obj.CalGyroA[0], 9U * sizeof(real_T));
    for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
         Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
         Combined_Experiment_ArmOnly_B.uElOffset1++) {
      Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
        = ((Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 1] * static_cast<real_T>
            (Combined_Experiment_ArmOnly_B.b_RegisterValue[1]) +
            Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1] * static_cast<real_T>
            (Combined_Experiment_ArmOnly_B.b_RegisterValue[0])) +
           Combined_Experiment_ArmOnly_B.b_c[3 *
           Combined_Experiment_ArmOnly_B.uElOffset1 + 2] * static_cast<real_T>
           (Combined_Experiment_ArmOnly_B.b_RegisterValue[2])) +
        Combined_Experiment_ArmOnly_DW.obj.CalGyroB[Combined_Experiment_ArmOnly_B.uElOffset1];
    }

    Combined_Experiment_ArmOnly_B.status = 40U;
    memcpy((void *)&Combined_Experiment_ArmOnly_B.SwappedDataBytes, (void *)
           &Combined_Experiment_ArmOnly_B.status, (uint32_T)((size_t)1 * sizeof
            (uint8_T)));
    Combined_Experiment_ArmOnly_B.status = MW_I2C_MasterWrite
      (Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
       &Combined_Experiment_ArmOnly_B.SwappedDataBytes, 1U, true, false);
    if (0 == Combined_Experiment_ArmOnly_B.status) {
      MW_I2C_MasterRead
        (Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
         Combined_Experiment_ArmOnly_B.output_raw, 6U, false, true);
      memcpy((void *)&Combined_Experiment_ArmOnly_B.b_RegisterValue[0], (void *)
             &Combined_Experiment_ArmOnly_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Combined_Experiment_ArmOnly_B.b_RegisterValue[0] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[1] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Combined_Experiment_ArmOnly_B.b_c[0],
           &Combined_Experiment_ArmOnly_DW.obj.CalAccelA[0], 9U * sizeof(real_T));
    for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
         Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
         Combined_Experiment_ArmOnly_B.uElOffset1++) {
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[Combined_Experiment_ArmOnly_B.uElOffset1]
        = ((Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1 + 1] * static_cast<real_T>
            (Combined_Experiment_ArmOnly_B.b_RegisterValue[1]) +
            Combined_Experiment_ArmOnly_B.b_c[3 *
            Combined_Experiment_ArmOnly_B.uElOffset1] * static_cast<real_T>
            (Combined_Experiment_ArmOnly_B.b_RegisterValue[0])) +
           Combined_Experiment_ArmOnly_B.b_c[3 *
           Combined_Experiment_ArmOnly_B.uElOffset1 + 2] * static_cast<real_T>
           (Combined_Experiment_ArmOnly_B.b_RegisterValue[2])) +
        Combined_Experiment_ArmOnly_DW.obj.CalAccelB[Combined_Experiment_ArmOnly_B.uElOffset1];
    }

    Combined_Experiment_ArmOnly_B.status = 40U;
    memcpy((void *)&Combined_Experiment_ArmOnly_B.SwappedDataBytes, (void *)
           &Combined_Experiment_ArmOnly_B.status, (uint32_T)((size_t)1 * sizeof
            (uint8_T)));
    Combined_Experiment_ArmOnly_B.status = MW_I2C_MasterWrite
      (Combined_Experiment_ArmOnly_DW.obj.i2cobj_MAG.MW_I2C_HANDLE, 28U,
       &Combined_Experiment_ArmOnly_B.SwappedDataBytes, 1U, true, false);
    if (0 == Combined_Experiment_ArmOnly_B.status) {
      MW_I2C_MasterRead
        (Combined_Experiment_ArmOnly_DW.obj.i2cobj_MAG.MW_I2C_HANDLE, 28U,
         Combined_Experiment_ArmOnly_B.output_raw, 6U, false, true);
      memcpy((void *)&Combined_Experiment_ArmOnly_B.b_RegisterValue[0], (void *)
             &Combined_Experiment_ArmOnly_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Combined_Experiment_ArmOnly_B.b_RegisterValue[0] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[1] = 0;
      Combined_Experiment_ArmOnly_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Combined_Experiment_ArmOnly_B.b_c[0],
           &Combined_Experiment_ArmOnly_DW.obj.CalMagA[0], 9U * sizeof(real_T));
    for (Combined_Experiment_ArmOnly_B.uElOffset1 = 0;
         Combined_Experiment_ArmOnly_B.uElOffset1 < 3;
         Combined_Experiment_ArmOnly_B.uElOffset1++) {
      Combined_Experiment_ArmOnly_B.y5 = ((Combined_Experiment_ArmOnly_B.b_c[3 *
        Combined_Experiment_ArmOnly_B.uElOffset1 + 1] * static_cast<real_T>
        (Combined_Experiment_ArmOnly_B.b_RegisterValue[1]) +
        Combined_Experiment_ArmOnly_B.b_c[3 *
        Combined_Experiment_ArmOnly_B.uElOffset1] * static_cast<real_T>
        (Combined_Experiment_ArmOnly_B.b_RegisterValue[0])) +
        Combined_Experiment_ArmOnly_B.b_c[3 *
        Combined_Experiment_ArmOnly_B.uElOffset1 + 2] * static_cast<real_T>
        (Combined_Experiment_ArmOnly_B.b_RegisterValue[2])) +
        Combined_Experiment_ArmOnly_DW.obj.CalMagB[Combined_Experiment_ArmOnly_B.uElOffset1];
      Combined_Experiment_ArmOnly_B.LSM9DS1IMUSensor_o3[Combined_Experiment_ArmOnly_B.uElOffset1]
        = Combined_Experiment_ArmOnly_B.y5 * 4.0 / 32768.0;
      Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
        =
        Combined_Experiment_ArmOnly_B.x_ddot_a[Combined_Experiment_ArmOnly_B.uElOffset1]
        * 245.0 / 32768.0;
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[Combined_Experiment_ArmOnly_B.uElOffset1]
        =
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[Combined_Experiment_ArmOnly_B.uElOffset1]
        * 2.0 / 32768.0;
    }

    // End of MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'

    // If: '<S6>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
    //   Constant: '<S172>/Constant'
    //   Constant: '<S6>/Constant1'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 2.0) &&
        (Combined_Experiment_ArmOnly_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change BLACK Behavior' incorporates:
      //   ActionPort: '<S169>/Action Port'

      // Gain: '<S169>/Gain'
      Combined_Experiment_ArmOnly_B.Gain_k[0] =
        Combined_Experiment_ArmOnly_P.Gain_Gain *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0];
      Combined_Experiment_ArmOnly_B.Gain_k[1] =
        Combined_Experiment_ArmOnly_P.Gain_Gain *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];
      Combined_Experiment_ArmOnly_B.Gain_k[2] =
        Combined_Experiment_ArmOnly_P.Gain_Gain *
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // If: '<S179>/If' incorporates:
      //   DataStoreRead: '<S184>/Universal_Time'
      //   Inport: '<S183>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S179>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S182>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_k[0],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean);

        // End of Outputs for SubSystem: '<S179>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S179>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S183>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_hd =
          Combined_Experiment_ArmOnly_B.Gain_k[0];

        // End of Outputs for SubSystem: '<S179>/Pass Current Gyro'
      }

      // End of If: '<S179>/If'

      // Sum: '<S179>/Subtract' incorporates:
      //   DataStoreWrite: '<S169>/RED_Px1'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Q =
        Combined_Experiment_ArmOnly_B.In1_hd -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean.Mean;

      // S-Function (sdspbiquad): '<S175>/Digital Filter' incorporates:
      //   Gain: '<S169>/Gain1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[0]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[1];
      Combined_Experiment_ArmOnly_B.DigitalFilter_k[0] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[0] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[1];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[1] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[0];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[0] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[2]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[3];
      Combined_Experiment_ArmOnly_B.DigitalFilter_k[1] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[2] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[3];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[3] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[2];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[2] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[2] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[4]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[5];
      Combined_Experiment_ArmOnly_B.DigitalFilter_k[2] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[4] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[5];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[5] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[4];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_e[4] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;

      // If: '<S192>/If' incorporates:
      //   DataStoreRead: '<S197>/Universal_Time'
      //   Inport: '<S196>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S192>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S195>/Action Port'

        Combin_CalculateRunningMean
          (Combined_Experiment_ArmOnly_B.DigitalFilter_k[0],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_d,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_d);

        // End of Outputs for SubSystem: '<S192>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S192>/Pass Current Accel' incorporates:
        //   ActionPort: '<S196>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_ic =
          Combined_Experiment_ArmOnly_B.DigitalFilter_k[0];

        // End of Outputs for SubSystem: '<S192>/Pass Current Accel'
      }

      // End of If: '<S192>/If'

      // If: '<S193>/If' incorporates:
      //   DataStoreRead: '<S200>/Universal_Time'
      //   Inport: '<S199>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S193>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S198>/Action Port'

        Combin_CalculateRunningMean
          (Combined_Experiment_ArmOnly_B.DigitalFilter_k[1],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_a,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_a);

        // End of Outputs for SubSystem: '<S193>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S193>/Pass Current Accel' incorporates:
        //   ActionPort: '<S199>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_gs =
          Combined_Experiment_ArmOnly_B.DigitalFilter_k[1];

        // End of Outputs for SubSystem: '<S193>/Pass Current Accel'
      }

      // End of If: '<S193>/If'

      // If: '<S194>/If' incorporates:
      //   DataStoreRead: '<S203>/Universal_Time'
      //   Inport: '<S202>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S194>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S201>/Action Port'

        Comb_CalculateRunningMean_a
          (Combined_Experiment_ArmOnly_B.DigitalFilter_k[2],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_ab,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_ab,
           &Combined_Experiment_ArmOnly_P.CalculateRunningMean_ab);

        // End of Outputs for SubSystem: '<S194>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S194>/Pass Current Accel' incorporates:
        //   ActionPort: '<S202>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_cd =
          Combined_Experiment_ArmOnly_B.DigitalFilter_k[2];

        // End of Outputs for SubSystem: '<S194>/Pass Current Accel'
      }

      // End of If: '<S194>/If'

      // MATLAB Function: '<S169>/ChangeOrientation' incorporates:
      //   Sum: '<S192>/Subtract'
      //   Sum: '<S193>/Subtract'
      //   Sum: '<S194>/Subtract'

      Combined__ChangeOrientation(Combined_Experiment_ArmOnly_B.In1_ic -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_d.Mean,
        Combined_Experiment_ArmOnly_B.In1_gs -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_a.Mean,
        Combined_Experiment_ArmOnly_B.In1_cd -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_ab.Subtract,
        &Combined_Experiment_ArmOnly_B.sf_ChangeOrientation);

      // DataStoreWrite: '<S169>/RED_Px10'
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S169>/RED_Px11'
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Az_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[2];

      // If: '<S180>/If' incorporates:
      //   DataStoreRead: '<S187>/Universal_Time'
      //   Inport: '<S186>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S180>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S185>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_k[1],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_e,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_e);

        // End of Outputs for SubSystem: '<S180>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S180>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S186>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_ny =
          Combined_Experiment_ArmOnly_B.Gain_k[1];

        // End of Outputs for SubSystem: '<S180>/Pass Current Gyro'
      }

      // End of If: '<S180>/If'

      // Sum: '<S180>/Subtract' incorporates:
      //   DataStoreWrite: '<S169>/RED_Px2'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_P =
        Combined_Experiment_ArmOnly_B.In1_ny -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_e.Mean;

      // If: '<S181>/If' incorporates:
      //   DataStoreRead: '<S191>/Universal_Time'
      //   Inport: '<S190>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S181>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S188>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_k[2],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_h,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_h);

        // End of Outputs for SubSystem: '<S181>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S181>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S190>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_jp =
          Combined_Experiment_ArmOnly_B.Gain_k[2];

        // End of Outputs for SubSystem: '<S181>/Pass Current Gyro'
      }

      // End of If: '<S181>/If'

      // Sum: '<S181>/Subtract' incorporates:
      //   DataStoreWrite: '<S169>/RED_Px3'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_R =
        Combined_Experiment_ArmOnly_B.In1_jp -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_h.Mean;

      // MATLAB Function: '<S169>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S169>/Data Store Read'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.BLACK_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_fq);

      // DataStoreWrite: '<S169>/RED_Px4' incorporates:
      //   Product: '<S169>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_I =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_fq.C_Ib[0] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_fq.C_Ib[2] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S169>/RED_Px5' incorporates:
      //   Product: '<S169>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_I =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_fq.C_Ib[1] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_fq.C_Ib[3] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S169>/RED_Px6'
      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation.y[0];
      Combined_Experiment_A_AHRS2(Combined_Experiment_ArmOnly_B.DigitalFilter_k,
        Combined_Experiment_ArmOnly_B.Gain_k,
        Combined_Experiment_ArmOnly_B.LSM9DS1IMUSensor_o3,
        &Combined_Experiment_ArmOnly_B.AHRS2,
        &Combined_Experiment_ArmOnly_DW.AHRS2,
        &Combined_Experiment_ArmOnly_P.AHRS2);

      // DataStoreWrite: '<S169>/RED_Px7'
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_Q =
        Combined_Experiment_ArmOnly_B.AHRS2.AHRS2_o2[0];

      // DataStoreWrite: '<S169>/RED_Px8'
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_P =
        Combined_Experiment_ArmOnly_B.AHRS2.AHRS2_o2[1];

      // DataStoreWrite: '<S169>/RED_Px9'
      Combined_Experiment_ArmOnly_DW.BLACK_AHRS_R =
        Combined_Experiment_ArmOnly_B.AHRS2.AHRS2_o2[2];

      // RelationalOperator: '<S189>/Compare' incorporates:
      //   Constant: '<S189>/Constant'

      Combined_Experiment_ArmOnly_B.rtb_Compare_me =
        (Combined_Experiment_ArmOnly_B.In1_jp ==
         Combined_Experiment_ArmOnly_P.Constant_Value_h);

      // DiscreteIntegrator: '<S181>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S181>/Data Store Read1'
      //   DataStoreRead: '<S181>/Data Store Read3'
      //   DataStoreWrite: '<S181>/RED_Px4'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_i != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_f =
          Combined_Experiment_ArmOnly_DW.BLACK_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_me &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_o <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_me) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_o ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_f =
          Combined_Experiment_ArmOnly_DW.BLACK_Rz;
      }

      Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi =
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator1_gainval *
        Combined_Experiment_ArmOnly_DW.BLACK_IMU_R +
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_f;

      // End of DiscreteIntegrator: '<S181>/Discrete-Time Integrator1'

      // Update for DiscreteIntegrator: '<S181>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S181>/RED_Px4'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_i = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_f =
        Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_o =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_me);

      // End of Outputs for SubSystem: '<S6>/Change BLACK Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the BLACK sim//exp' 

    // If: '<S6>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
    //   Constant: '<S172>/Constant'
    //   Constant: '<S6>/Constant1'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 3.0) &&
        (Combined_Experiment_ArmOnly_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change BLUE Behavior' incorporates:
      //   ActionPort: '<S170>/Action Port'

      // Gain: '<S170>/Gain'
      Combined_Experiment_ArmOnly_B.Gain_o[0] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_e *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0];
      Combined_Experiment_ArmOnly_B.Gain_o[1] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_e *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];
      Combined_Experiment_ArmOnly_B.Gain_o[2] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_e *
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // If: '<S210>/If' incorporates:
      //   DataStoreRead: '<S215>/Universal_Time'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S210>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S213>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_o[0],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_c,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_c);

        // End of Outputs for SubSystem: '<S210>/Calculate Running Mean'
      }

      // End of If: '<S210>/If'

      // S-Function (sdspbiquad): '<S206>/Digital Filter' incorporates:
      //   Gain: '<S170>/Gain1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain_e *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[0]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[1];
      Combined_Experiment_ArmOnly_B.DigitalFilter_n[0] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[0] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[1];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[1] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[0];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[0] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain_e *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[2]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[3];
      Combined_Experiment_ArmOnly_B.DigitalFilter_n[1] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[2] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[3];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[3] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[2];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[2] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain_e *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[2] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[4]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[5];
      Combined_Experiment_ArmOnly_B.DigitalFilter_n[2] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[4] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[5];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[5] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[4];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES_n[4] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;

      // If: '<S223>/If' incorporates:
      //   DataStoreRead: '<S228>/Universal_Time'
      //   Inport: '<S227>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S223>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S226>/Action Port'

        Combin_CalculateRunningMean
          (Combined_Experiment_ArmOnly_B.DigitalFilter_n[0],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_j,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_j);

        // End of Outputs for SubSystem: '<S223>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S223>/Pass Current Accel' incorporates:
        //   ActionPort: '<S227>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_le =
          Combined_Experiment_ArmOnly_B.DigitalFilter_n[0];

        // End of Outputs for SubSystem: '<S223>/Pass Current Accel'
      }

      // End of If: '<S223>/If'

      // If: '<S224>/If' incorporates:
      //   DataStoreRead: '<S231>/Universal_Time'
      //   Inport: '<S230>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S224>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S229>/Action Port'

        Combin_CalculateRunningMean
          (Combined_Experiment_ArmOnly_B.DigitalFilter_n[1],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_b,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_b);

        // End of Outputs for SubSystem: '<S224>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S224>/Pass Current Accel' incorporates:
        //   ActionPort: '<S230>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_b1 =
          Combined_Experiment_ArmOnly_B.DigitalFilter_n[1];

        // End of Outputs for SubSystem: '<S224>/Pass Current Accel'
      }

      // End of If: '<S224>/If'

      // If: '<S225>/If' incorporates:
      //   DataStoreRead: '<S234>/Universal_Time'
      //   Inport: '<S233>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S225>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S232>/Action Port'

        Comb_CalculateRunningMean_a
          (Combined_Experiment_ArmOnly_B.DigitalFilter_n[2],
           &Combined_Experiment_ArmOnly_B.CalculateRunningMean_o,
           &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_o,
           &Combined_Experiment_ArmOnly_P.CalculateRunningMean_o);

        // End of Outputs for SubSystem: '<S225>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S225>/Pass Current Accel' incorporates:
        //   ActionPort: '<S233>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_ai =
          Combined_Experiment_ArmOnly_B.DigitalFilter_n[2];

        // End of Outputs for SubSystem: '<S225>/Pass Current Accel'
      }

      // End of If: '<S225>/If'

      // MATLAB Function: '<S170>/ChangeOrientation' incorporates:
      //   Sum: '<S223>/Subtract'
      //   Sum: '<S224>/Subtract'
      //   Sum: '<S225>/Subtract'

      Combined__ChangeOrientation(Combined_Experiment_ArmOnly_B.In1_le -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_j.Mean,
        Combined_Experiment_ArmOnly_B.In1_b1 -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_b.Mean,
        Combined_Experiment_ArmOnly_B.In1_ai -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_o.Subtract,
        &Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_p);

      // If: '<S211>/If' incorporates:
      //   DataStoreRead: '<S218>/Universal_Time'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S211>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S216>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_o[1],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_f,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_f);

        // End of Outputs for SubSystem: '<S211>/Calculate Running Mean'
      }

      // End of If: '<S211>/If'

      // If: '<S212>/If' incorporates:
      //   DataStoreRead: '<S222>/Universal_Time'
      //   Inport: '<S221>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S212>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S219>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain_o[2],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_l,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_l);

        // End of Outputs for SubSystem: '<S212>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S212>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S221>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_hy =
          Combined_Experiment_ArmOnly_B.Gain_o[2];

        // End of Outputs for SubSystem: '<S212>/Pass Current Gyro'
      }

      // End of If: '<S212>/If'

      // MATLAB Function: '<S170>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S170>/Data Store Read'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.BLUE_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_h1);
      Combined_Experiment_A_AHRS2(Combined_Experiment_ArmOnly_B.DigitalFilter_n,
        Combined_Experiment_ArmOnly_B.Gain_o,
        Combined_Experiment_ArmOnly_B.LSM9DS1IMUSensor_o3,
        &Combined_Experiment_ArmOnly_B.AHRS2_p,
        &Combined_Experiment_ArmOnly_DW.AHRS2_p,
        &Combined_Experiment_ArmOnly_P.AHRS2_p);

      // RelationalOperator: '<S220>/Compare' incorporates:
      //   Constant: '<S220>/Constant'

      Combined_Experiment_ArmOnly_B.rtb_Compare_me =
        (Combined_Experiment_ArmOnly_B.In1_hy ==
         Combined_Experiment_ArmOnly_P.Constant_Value_d);

      // DiscreteIntegrator: '<S212>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S212>/Data Store Read1'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_f != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_k =
          Combined_Experiment_ArmOnly_DW.BLUE_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_me &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_h <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_me) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_h ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_k =
          Combined_Experiment_ArmOnly_DW.BLUE_Rz;
      }

      // Update for DiscreteIntegrator: '<S212>/Discrete-Time Integrator1' incorporates:
      //   Sum: '<S212>/Subtract'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_f = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_k +=
        (Combined_Experiment_ArmOnly_B.In1_hy -
         Combined_Experiment_ArmOnly_B.CalculateRunningMean_l.Mean) *
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator1_gainv_d;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_h =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_me);

      // End of Outputs for SubSystem: '<S6>/Change BLUE Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the BLUE sim//exp' 

    // If: '<S6>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
    //   Constant: '<S172>/Constant'
    //   Constant: '<S6>/Constant1'
    //   Gain: '<S171>/Gain1'

    if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) &&
        (Combined_Experiment_ArmOnly_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change RED Behavior' incorporates:
      //   ActionPort: '<S171>/Action Port'

      // Gain: '<S171>/Gain'
      Combined_Experiment_ArmOnly_B.Gain[0] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_a *
        Combined_Experiment_ArmOnly_B.x_ddot_a[0];
      Combined_Experiment_ArmOnly_B.Gain[1] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_a *
        Combined_Experiment_ArmOnly_B.x_ddot_a[1];
      Combined_Experiment_ArmOnly_B.Gain[2] =
        Combined_Experiment_ArmOnly_P.Gain_Gain_a *
        Combined_Experiment_ArmOnly_B.x_ddot_a[2];

      // If: '<S241>/If' incorporates:
      //   DataStoreRead: '<S246>/Universal_Time'
      //   Inport: '<S245>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S241>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S244>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain[0],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_oo,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_oo);

        // End of Outputs for SubSystem: '<S241>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S241>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S245>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_lm =
          Combined_Experiment_ArmOnly_B.Gain[0];

        // End of Outputs for SubSystem: '<S241>/Pass Current Gyro'
      }

      // End of If: '<S241>/If'

      // Sum: '<S241>/Subtract' incorporates:
      //   DataStoreWrite: '<S171>/RED_Px1'

      Combined_Experiment_ArmOnly_DW.RED_IMU_Q =
        Combined_Experiment_ArmOnly_B.In1_lm -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_oo.Mean;
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] *=
        Combined_Experiment_ArmOnly_P.Gain1_Gain_c;
      Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] *=
        Combined_Experiment_ArmOnly_P.Gain1_Gain_c;

      // S-Function (sdspbiquad): '<S237>/Digital Filter' incorporates:
      //   Gain: '<S171>/Gain1'

      Combined_Experiment_ArmOnly_B.Sum6_l1 = (0.29289321881345243 *
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[0] -
        -1.3007071811330761E-16 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[0]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[1];
      Combined_Experiment_ArmOnly_B.DigitalFilter[0] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[0] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[1];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[1] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[0];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[0] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 = (0.29289321881345243 *
        Combined_Experiment_ArmOnly_B.VelocitytoPosition[1] -
        -1.3007071811330761E-16 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[2]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[3];
      Combined_Experiment_ArmOnly_B.DigitalFilter[1] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[2] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[3];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[3] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[2];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[2] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;
      Combined_Experiment_ArmOnly_B.Sum6_l1 =
        (Combined_Experiment_ArmOnly_P.Gain1_Gain_c *
         Combined_Experiment_ArmOnly_B.VelocitytoPosition[2] *
         0.29289321881345243 - -1.3007071811330761E-16 *
         Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[4]) -
        0.17157287525380996 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[5];
      Combined_Experiment_ArmOnly_B.DigitalFilter[2] = (2.0 *
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[4] +
        Combined_Experiment_ArmOnly_B.Sum6_l1) +
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[5];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[5] =
        Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[4];
      Combined_Experiment_ArmOnly_DW.DigitalFilter_FILT_STATES[4] =
        Combined_Experiment_ArmOnly_B.Sum6_l1;

      // If: '<S254>/If' incorporates:
      //   DataStoreRead: '<S259>/Universal_Time'
      //   Inport: '<S258>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S254>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S257>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.DigitalFilter
          [0], &Combined_Experiment_ArmOnly_B.CalculateRunningMean_j1,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_j1);

        // End of Outputs for SubSystem: '<S254>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S254>/Pass Current Accel' incorporates:
        //   ActionPort: '<S258>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_gy =
          Combined_Experiment_ArmOnly_B.DigitalFilter[0];

        // End of Outputs for SubSystem: '<S254>/Pass Current Accel'
      }

      // End of If: '<S254>/If'

      // If: '<S255>/If' incorporates:
      //   DataStoreRead: '<S262>/Universal_Time'
      //   Inport: '<S261>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S255>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S260>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.DigitalFilter
          [1], &Combined_Experiment_ArmOnly_B.CalculateRunningMean_jr,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_jr);

        // End of Outputs for SubSystem: '<S255>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S255>/Pass Current Accel' incorporates:
        //   ActionPort: '<S261>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_e5 =
          Combined_Experiment_ArmOnly_B.DigitalFilter[1];

        // End of Outputs for SubSystem: '<S255>/Pass Current Accel'
      }

      // End of If: '<S255>/If'

      // If: '<S256>/If' incorporates:
      //   DataStoreRead: '<S265>/Universal_Time'
      //   Inport: '<S264>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S256>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S263>/Action Port'

        Comb_CalculateRunningMean_a(Combined_Experiment_ArmOnly_B.DigitalFilter
          [2], &Combined_Experiment_ArmOnly_B.CalculateRunningMean_cf,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_cf,
          &Combined_Experiment_ArmOnly_P.CalculateRunningMean_cf);

        // End of Outputs for SubSystem: '<S256>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S256>/Pass Current Accel' incorporates:
        //   ActionPort: '<S264>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_gv =
          Combined_Experiment_ArmOnly_B.DigitalFilter[2];

        // End of Outputs for SubSystem: '<S256>/Pass Current Accel'
      }

      // End of If: '<S256>/If'

      // MATLAB Function: '<S171>/ChangeOrientation' incorporates:
      //   Sum: '<S254>/Subtract'
      //   Sum: '<S255>/Subtract'
      //   Sum: '<S256>/Subtract'

      Combined__ChangeOrientation(Combined_Experiment_ArmOnly_B.In1_gy -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_j1.Mean,
        Combined_Experiment_ArmOnly_B.In1_e5 -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_jr.Mean,
        Combined_Experiment_ArmOnly_B.In1_gv -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_cf.Subtract,
        &Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b);

      // DataStoreWrite: '<S171>/RED_Px10'
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[0];

      // DataStoreWrite: '<S171>/RED_Px11'
      Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[1];

      // DataStoreWrite: '<S171>/RED_Px12'
      Combined_Experiment_ArmOnly_DW.RED_IMU_Az_b =
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[2];

      // If: '<S242>/If' incorporates:
      //   DataStoreRead: '<S249>/Universal_Time'
      //   Inport: '<S248>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S242>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S247>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain[1],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_om,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_om);

        // End of Outputs for SubSystem: '<S242>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S242>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S248>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_cx =
          Combined_Experiment_ArmOnly_B.Gain[1];

        // End of Outputs for SubSystem: '<S242>/Pass Current Gyro'
      }

      // End of If: '<S242>/If'

      // Sum: '<S242>/Subtract' incorporates:
      //   DataStoreWrite: '<S171>/RED_Px2'

      Combined_Experiment_ArmOnly_DW.RED_IMU_P =
        Combined_Experiment_ArmOnly_B.In1_cx -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_om.Mean;

      // If: '<S243>/If' incorporates:
      //   DataStoreRead: '<S253>/Universal_Time'
      //   Inport: '<S252>/In1'

      if (Combined_Experiment_ArmOnly_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S243>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S250>/Action Port'

        Combin_CalculateRunningMean(Combined_Experiment_ArmOnly_B.Gain[2],
          &Combined_Experiment_ArmOnly_B.CalculateRunningMean_et,
          &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_et);

        // End of Outputs for SubSystem: '<S243>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S243>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S252>/Action Port'

        Combined_Experiment_ArmOnly_B.In1_f1 =
          Combined_Experiment_ArmOnly_B.Gain[2];

        // End of Outputs for SubSystem: '<S243>/Pass Current Gyro'
      }

      // End of If: '<S243>/If'

      // Sum: '<S243>/Subtract' incorporates:
      //   DataStoreWrite: '<S171>/RED_Px3'

      Combined_Experiment_ArmOnly_DW.RED_IMU_R =
        Combined_Experiment_ArmOnly_B.In1_f1 -
        Combined_Experiment_ArmOnly_B.CalculateRunningMean_et.Mean;

      // MATLAB Function: '<S171>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S171>/Data Store Read'

      Combin_CreateRotationMatrix(Combined_Experiment_ArmOnly_DW.RED_Rz,
        &Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_k);

      // DataStoreWrite: '<S171>/RED_Px4' incorporates:
      //   Product: '<S171>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_I =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_k.C_Ib[0] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_k.C_Ib[2] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[1];

      // DataStoreWrite: '<S171>/RED_Px5' incorporates:
      //   Product: '<S171>/Rotate F_b to F_I'

      Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_I =
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_k.C_Ib[1] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[0] +
        Combined_Experiment_ArmOnly_B.sf_CreateRotationMatrix_k.C_Ib[3] *
        Combined_Experiment_ArmOnly_B.sf_ChangeOrientation_b.y[1];
      Combined_Experiment_A_AHRS2(Combined_Experiment_ArmOnly_B.DigitalFilter,
        Combined_Experiment_ArmOnly_B.Gain,
        Combined_Experiment_ArmOnly_B.LSM9DS1IMUSensor_o3,
        &Combined_Experiment_ArmOnly_B.AHRS2_pn,
        &Combined_Experiment_ArmOnly_DW.AHRS2_pn,
        &Combined_Experiment_ArmOnly_P.AHRS2_pn);

      // DataStoreWrite: '<S171>/RED_Px7'
      Combined_Experiment_ArmOnly_DW.RED_AHRS_P =
        Combined_Experiment_ArmOnly_B.AHRS2_pn.AHRS2_o2[1];

      // DataStoreWrite: '<S171>/RED_Px8'
      Combined_Experiment_ArmOnly_DW.RED_AHRS_Q =
        Combined_Experiment_ArmOnly_B.AHRS2_pn.AHRS2_o2[0];

      // DataStoreWrite: '<S171>/RED_Px9'
      Combined_Experiment_ArmOnly_DW.RED_AHRS_R =
        Combined_Experiment_ArmOnly_B.AHRS2_pn.AHRS2_o2[2];

      // RelationalOperator: '<S251>/Compare' incorporates:
      //   Constant: '<S251>/Constant'

      Combined_Experiment_ArmOnly_B.rtb_Compare_me =
        (Combined_Experiment_ArmOnly_B.In1_f1 ==
         Combined_Experiment_ArmOnly_P.Constant_Value_k);

      // DiscreteIntegrator: '<S243>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S243>/Data Store Read1'
      //   DataStoreRead: '<S243>/Data Store Read3'
      //   DataStoreWrite: '<S243>/RED_Px4'

      if (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_g != 0) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_h =
          Combined_Experiment_ArmOnly_DW.RED_Rz;
      }

      if ((Combined_Experiment_ArmOnly_B.rtb_Compare_me &&
           (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_f <= 0))
          || ((!Combined_Experiment_ArmOnly_B.rtb_Compare_me) &&
              (Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_f ==
               1))) {
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_h =
          Combined_Experiment_ArmOnly_DW.RED_Rz;
      }

      Combined_Experiment_ArmOnly_DW.RED_IMU_Psi =
        Combined_Experiment_ArmOnly_P.DiscreteTimeIntegrator1_gainv_k *
        Combined_Experiment_ArmOnly_DW.RED_IMU_R +
        Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_h;

      // End of DiscreteIntegrator: '<S243>/Discrete-Time Integrator1'

      // Update for DiscreteIntegrator: '<S243>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S243>/RED_Px4'

      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_g = 0U;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_DSTAT_h =
        Combined_Experiment_ArmOnly_DW.RED_IMU_Psi;
      Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_f =
        static_cast<int8_T>(Combined_Experiment_ArmOnly_B.rtb_Compare_me);

      // End of Outputs for SubSystem: '<S6>/Change RED Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the RED sim//exp ' 

    // Clock: '<S15>/Set Universal Time (If this is a simulation)'
    Combined_Experiment_ArmOnly_B.SetUniversalTimeIfthisisasimula =
      Combined_Experiment_ArmOnly_M->Timing.t[0];

    // MATLABSystem: '<Root>/Initialization Block for Position Control'
    if (Combined_Experiment_ArmOnly_DW.obj_i.P_GAIN !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositionC) {
      Combined_Experiment_ArmOnly_DW.obj_i.P_GAIN =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositionC;
    }

    if (Combined_Experiment_ArmOnly_DW.obj_i.I_GAIN !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_b) {
      Combined_Experiment_ArmOnly_DW.obj_i.I_GAIN =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_b;
    }

    if (Combined_Experiment_ArmOnly_DW.obj_i.D_GAIN !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_g) {
      Combined_Experiment_ArmOnly_DW.obj_i.D_GAIN =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_g;
    }

    if (Combined_Experiment_ArmOnly_DW.obj_i.MAX_POSITION !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_i) {
      Combined_Experiment_ArmOnly_DW.obj_i.MAX_POSITION =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_i;
    }

    if (Combined_Experiment_ArmOnly_DW.obj_i.MIN_POSITION !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_k) {
      Combined_Experiment_ArmOnly_DW.obj_i.MIN_POSITION =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_k;
    }

    if (Combined_Experiment_ArmOnly_DW.obj_i.MOVE_TIME !=
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositi_ba) {
      Combined_Experiment_ArmOnly_DW.obj_i.MOVE_TIME =
        Combined_Experiment_ArmOnly_P.InitializationBlockforPositi_ba;
    }

    // End of MATLABSystem: '<Root>/Initialization Block for Position Control'
  }

  // Matfile logging
  rt_UpdateTXYLogVars(Combined_Experiment_ArmOnly_M->rtwLogInfo,
                      (Combined_Experiment_ArmOnly_M->Timing.t));

  {
    char_T *sErr;

    // Update for If: '<Root>/Separate Phases'
    switch (Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem) {
     case 0:
     case 1:
     case 6:
      break;

     case 2:
      // Update for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' incorporates:
      //   ActionPort: '<S10>/Action Port'

      // Update for If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_dy == 0) {
        // Update for IfAction SubSystem: '<S10>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S274>/Action Port'

        // Update for Delay: '<S284>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_ns = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k =
          Combined_Experiment_ArmOnly_B.Subtract_ou;

        // Update for Delay: '<S286>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_k1 = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_d =
          Combined_Experiment_ArmOnly_B.Subtract1_m0;

        // Update for Delay: '<S282>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_ep = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_j =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4.e_out;

        // End of Update for SubSystem: '<S10>/Change BLACK Behavior'
      }

      // End of Update for If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 

      // Update for If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jm == 0) {
        // Update for IfAction SubSystem: '<S10>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S275>/Action Port'

        // Update for Delay: '<S297>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_dy = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fb =
          Combined_Experiment_ArmOnly_B.Subtract_j;

        // Update for Delay: '<S299>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_hf = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n1 =
          Combined_Experiment_ArmOnly_B.Subtract1_o;

        // Update for Delay: '<S295>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_kl = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cq =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_c.e_out;

        // End of Update for SubSystem: '<S10>/Change BLUE Behavior'
      }

      // End of Update for If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 

      // Update for If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_aj == 0) {
        // Update for IfAction SubSystem: '<S10>/Change RED Behavior' incorporates:
        //   ActionPort: '<S276>/Action Port'

        // Update for Delay: '<S308>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_ji = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_b.e_out;

        // Update for Delay: '<S310>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_a1 = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bm =
          Combined_Experiment_ArmOnly_B.Subtract_f;

        // Update for Delay: '<S312>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_o0 = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pr =
          Combined_Experiment_ArmOnly_B.Subtract1_dr;

        // End of Update for SubSystem: '<S10>/Change RED Behavior'
      }

      // End of Update for If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Update for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
      break;

     case 3:
      // Update for IfAction SubSystem: '<Root>/Phase #3: Experiment' incorporates:
      //   ActionPort: '<S11>/Action Port'

      // Update for If: '<S11>/This IF block determines whether or not to run the BLACK sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_p == 0) {
        // Update for IfAction SubSystem: '<S11>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S317>/Action Port'

        // Update for If: '<S317>/Experiment Sub-Phases'
        switch (Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o)
        {
         case 0:
          // Update for IfAction SubSystem: '<S317>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S321>/Action Port'

          // Update for Delay: '<S332>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ot = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_g =
            Combined_Experiment_ArmOnly_B.Subtract_oi;

          // Update for Delay: '<S334>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_k = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pe =
            Combined_Experiment_ArmOnly_B.Subtract1_d2;

          // Update for Delay: '<S330>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_l = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_d.e_out;

          // End of Update for SubSystem: '<S317>/Sub-Phase #1'
          break;

         case 1:
         case 2:
          break;

         case 3:
          // Update for IfAction SubSystem: '<S317>/Sub-Phase #4' incorporates:
          //   ActionPort: '<S324>/Action Port'

          // Update for Delay: '<S349>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_fr = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hp =
            Combined_Experiment_ArmOnly_B.Subtract_a;

          // Update for Delay: '<S351>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_me = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ha =
            Combined_Experiment_ArmOnly_B.Subtract1_h;

          // Update for Delay: '<S347>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ey = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_li =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_dk.e_out;

          // End of Update for SubSystem: '<S317>/Sub-Phase #4'
          break;
        }

        // End of Update for If: '<S317>/Experiment Sub-Phases'
        // End of Update for SubSystem: '<S11>/Change BLACK Behavior'
      }

      // End of Update for If: '<S11>/This IF block determines whether or not to run the BLACK sim//exp' 

      // Update for If: '<S11>/This IF block determines whether or not to run the BLUE sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_pu == 0) {
        // Update for IfAction SubSystem: '<S11>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S318>/Action Port'

        // Update for If: '<S318>/Experiment Sub-Phases'
        switch (Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e)
        {
         case 0:
          // Update for IfAction SubSystem: '<S318>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S356>/Action Port'

          Combined_E_SubPhase1_Update(&Combined_Experiment_ArmOnly_B.SubPhase1_a,
            &Combined_Experiment_ArmOnly_DW.SubPhase1_a);

          // End of Update for SubSystem: '<S318>/Sub-Phase #1'
          break;

         case 1:
         case 2:
          break;

         case 3:
          // Update for IfAction SubSystem: '<S318>/Sub-Phase #4' incorporates:
          //   ActionPort: '<S359>/Action Port'

          Combined_E_SubPhase1_Update(&Combined_Experiment_ArmOnly_B.SubPhase4_f,
            &Combined_Experiment_ArmOnly_DW.SubPhase4_f);

          // End of Update for SubSystem: '<S318>/Sub-Phase #4'
          break;
        }

        // End of Update for If: '<S318>/Experiment Sub-Phases'
        // End of Update for SubSystem: '<S11>/Change BLUE Behavior'
      }

      // End of Update for If: '<S11>/This IF block determines whether or not to run the BLUE sim//exp' 

      // Update for If: '<S11>/This IF block determines whether or not to run the RED sim//exp ' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_au == 0) {
        // Update for IfAction SubSystem: '<S11>/Change RED Behavior' incorporates:
        //   ActionPort: '<S319>/Action Port'

        // Update for If: '<S319>/Experiment Sub-Phases'
        if (Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy == 3)
        {
          // Update for IfAction SubSystem: '<S319>/Sub-Phase #4' incorporates:
          //   ActionPort: '<S390>/Action Port'

          // Update for Delay: '<S400>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_jl = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c =
            Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_n.e_out;

          // Update for Delay: '<S402>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_n = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mu =
            Combined_Experiment_ArmOnly_B.Subtract_b;

          // Update for Delay: '<S404>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_g = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_il =
            Combined_Experiment_ArmOnly_B.Subtract1_m;

          // End of Update for SubSystem: '<S319>/Sub-Phase #4'
        }

        // End of Update for If: '<S319>/Experiment Sub-Phases'
        // End of Update for SubSystem: '<S11>/Change RED Behavior'
      }

      // End of Update for If: '<S11>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Update for SubSystem: '<Root>/Phase #3: Experiment'
      break;

     case 4:
      // Update for IfAction SubSystem: '<Root>/Phase #4:  Return Home' incorporates:
      //   ActionPort: '<S12>/Action Port'

      // Update for If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_d == 0) {
        // Update for IfAction SubSystem: '<S12>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S410>/Action Port'

        // Update for Delay: '<S420>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_dq = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lt =
          Combined_Experiment_ArmOnly_B.Subtract_d;

        // Update for Delay: '<S422>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_h = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_im =
          Combined_Experiment_ArmOnly_B.Subtract1_d;

        // Update for Delay: '<S418>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_o = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_f =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_l.e_out;

        // End of Update for SubSystem: '<S12>/Change BLACK Behavior'
      }

      // End of Update for If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 

      // Update for If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jc == 0) {
        // Update for IfAction SubSystem: '<S12>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S411>/Action Port'

        C_ChangeBLUEBehavior_Update
          (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_gj,
           &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_gj);

        // End of Update for SubSystem: '<S12>/Change BLUE Behavior'
      }

      // End of Update for If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 

      // Update for If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_a4 == 0) {
        // Update for IfAction SubSystem: '<S12>/Change RED Behavior' incorporates:
        //   ActionPort: '<S412>/Action Port'

        // Update for Delay: '<S444>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_dh = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_na.e_out;

        // Update for Delay: '<S446>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_a2 = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_h =
          Combined_Experiment_ArmOnly_B.Subtract_l;

        // Update for Delay: '<S448>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_f = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_l =
          Combined_Experiment_ArmOnly_B.Subtract1_g;

        // End of Update for SubSystem: '<S12>/Change RED Behavior'
      }

      // End of Update for If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Update for SubSystem: '<Root>/Phase #4:  Return Home'
      break;

     case 5:
      // Update for IfAction SubSystem: '<Root>/Phase #5:  Hold Home' incorporates:
      //   ActionPort: '<S13>/Action Port'

      // Update for If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_b == 0) {
        // Update for IfAction SubSystem: '<S13>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S453>/Action Port'

        // Update for Delay: '<S463>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_e = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_o =
          Combined_Experiment_ArmOnly_B.Subtract_o;

        // Update for Delay: '<S465>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_m = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p =
          Combined_Experiment_ArmOnly_B.Subtract1_c;

        // Update for Delay: '<S461>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_d = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_de.e_out;

        // End of Update for SubSystem: '<S13>/Change BLACK Behavior'
      }

      // End of Update for If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 

      // Update for If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_n == 0) {
        // Update for IfAction SubSystem: '<S13>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S454>/Action Port'

        C_ChangeBLUEBehavior_Update
          (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_b,
           &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_b);

        // End of Update for SubSystem: '<S13>/Change BLUE Behavior'
      }

      // End of Update for If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 

      // Update for If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
      if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_j == 0) {
        // Update for IfAction SubSystem: '<S13>/Change RED Behavior' incorporates:
        //   ActionPort: '<S455>/Action Port'

        // Update for Delay: '<S489>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE =
          Combined_Experiment_ArmOnly_B.Subtract;

        // Update for Delay: '<S491>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_j = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m =
          Combined_Experiment_ArmOnly_B.Subtract1;

        // Update for Delay: '<S487>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_a = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n =
          Combined_Experiment_ArmOnly_B.sf_MATLABFunction4_df.e_out;

        // End of Update for SubSystem: '<S13>/Change RED Behavior'
      }

      // End of Update for If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Update for SubSystem: '<Root>/Phase #5:  Hold Home'
      break;
    }

    // End of Update for If: '<Root>/Separate Phases'

    // Update for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    if (Combined_Experiment_ArmOnly_DW.Ifperforminganexperimentgrabthe == 0) {
      // Update for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S75>/Action Port'

      // Update for If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
      switch (Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei) {
       case 0:
        // Update for IfAction SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' incorporates:
        //   ActionPort: '<S77>/Action Port'

        // Update for Delay: '<S153>/Delay'
        Combined_Experiment_ArmOnly_DW.Delay_DSTATE_o =
          Combined_Experiment_ArmOnly_B.DataTypeConversion_h;

        // Update for Delay: '<S147>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_dr = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ln =
          Combined_Experiment_ArmOnly_B.In1_b4[4];

        // Update for Delay: '<S142>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_mq = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_pv =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_p;

        // Update for Delay: '<S148>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_lr = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_mk =
          Combined_Experiment_ArmOnly_B.In1_b4[5];

        // Update for Delay: '<S143>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_b = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_le =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_nx;

        // Update for Delay: '<S149>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_aa = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_de =
          Combined_Experiment_ArmOnly_B.Unwrap1_c;

        // Update for Delay: '<S144>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_cw = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_lu2 =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_dx;

        // Update for Delay: '<S141>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_nsj = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_b3 =
          Combined_Experiment_ArmOnly_B.In1_b4[1];

        // Update for Delay: '<S150>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_lrw = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ng =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_ph;

        // Update for Delay: '<S145>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_a2d = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m3 =
          Combined_Experiment_ArmOnly_B.In1_b4[2];

        // Update for Delay: '<S151>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_mp = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fs =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_pf;

        // Update for Delay: '<S146>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_gb = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_c4 =
          Combined_Experiment_ArmOnly_B.Unwrap_n;

        // Update for Delay: '<S152>/Delay1'
        Combined_Experiment_ArmOnly_DW.icLoad_i = 0U;
        Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cz =
          Combined_Experiment_ArmOnly_B.dividebydeltaT_hu;

        // End of Update for SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 
        break;

       case 1:
        // Update for IfAction SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' incorporates:
        //   ActionPort: '<S76>/Action Port'

        // Update for If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 
        if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_d1 == 0)
        {
          // Update for IfAction SubSystem: '<S76>/Obtain BLACK States' incorporates:
          //   ActionPort: '<S79>/Action Port'

          // Update for Delay: '<S87>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ky1 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_nw =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[4];

          // Update for Delay: '<S82>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_of = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_m;

          // Update for Delay: '<S88>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_oi = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ct1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[5];

          // Update for Delay: '<S83>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_hb = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_i0 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_me;

          // Update for Delay: '<S89>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ap = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_n0 =
            Combined_Experiment_ArmOnly_B.Unwrap1_l;

          // Update for Delay: '<S84>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_hc = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_da =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_g;

          // Update for Delay: '<S81>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_nn = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_m1 =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[1];

          // Update for Delay: '<S90>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_gc = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bo =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_hw;

          // Update for Delay: '<S85>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_d0 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ed =
            Combined_Experiment_ArmOnly_B.UDPReceive_o1[2];

          // Update for Delay: '<S91>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_a1a = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_k5 =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_d;

          // Update for Delay: '<S86>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ms = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p4 =
            Combined_Experiment_ArmOnly_B.Unwrap_l;

          // Update for Delay: '<S92>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_gi = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_fr =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_n;

          // End of Update for SubSystem: '<S76>/Obtain BLACK States'
        }

        // End of Update for If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 

        // Update for If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
        if (Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o == 0)
        {
          // Update for IfAction SubSystem: '<S76>/Obtain RED States' incorporates:
          //   ActionPort: '<S80>/Action Port'

          // Update for Delay: '<S121>/Delay'
          Combined_Experiment_ArmOnly_DW.Delay_DSTATE_p =
            Combined_Experiment_ArmOnly_B.DataTypeConversion;

          // Update for Delay: '<S115>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_kc = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_p3 =
            Combined_Experiment_ArmOnly_B.In1_n1[4];

          // Update for Delay: '<S110>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_p1 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_bk =
            Combined_Experiment_ArmOnly_B.dividebydeltaT;

          // Update for Delay: '<S116>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_kv = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_ko =
            Combined_Experiment_ArmOnly_B.In1_n1[5];

          // Update for Delay: '<S111>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_cg = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kx =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l;

          // Update for Delay: '<S117>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_f0 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_at =
            Combined_Experiment_ArmOnly_B.Unwrap1;

          // Update for Delay: '<S112>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_a5 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hg =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_h;

          // Update for Delay: '<S109>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_l4 = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_knv =
            Combined_Experiment_ArmOnly_B.In1_n1[1];

          // Update for Delay: '<S118>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_go = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_oz =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_a;

          // Update for Delay: '<S113>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ay = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_cf =
            Combined_Experiment_ArmOnly_B.In1_n1[2];

          // Update for Delay: '<S119>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_aqe = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_me =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_l4;

          // Update for Delay: '<S114>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_ma = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_gy =
            Combined_Experiment_ArmOnly_B.Unwrap;

          // Update for Delay: '<S120>/Delay1'
          Combined_Experiment_ArmOnly_DW.icLoad_hj = 0U;
          Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_hf =
            Combined_Experiment_ArmOnly_B.dividebydeltaT_f;

          // Update for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 
          sErr = GetErrorBuffer
            (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);
          LibUpdate_Network
            (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U],
             &Combined_Experiment_ArmOnly_B.TmpSignalConversionAtSendBLACKS[0U],
             13);
          if (*sErr != 0) {
            rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
            rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
          }

          // End of Update for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 
          // End of Update for SubSystem: '<S76>/Obtain RED States'
        }

        // End of Update for If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
        // End of Update for SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
        break;
      }

      // End of Update for If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
      // End of Update for SubSystem: '<S5>/Use Hardware to Obtain States'
    }

    // End of Update for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // Update for S-Function (sdspToNetwork): '<S15>/UDP Send'
    sErr = GetErrorBuffer(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
    LibUpdate_Network(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U],
                      &Combined_Experiment_ArmOnly_B.SetUniversalTimeIfthisisasimula,
                      1);
    if (*sErr != 0) {
      rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
      rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<S15>/UDP Send'
  }

  // signal main to stop simulation
  {                                    // Sample time: [0.0s, 0.0s]
    if ((rtmGetTFinal(Combined_Experiment_ArmOnly_M)!=-1) &&
        !((rtmGetTFinal(Combined_Experiment_ArmOnly_M)-
           Combined_Experiment_ArmOnly_M->Timing.t[0]) >
          Combined_Experiment_ArmOnly_M->Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, "Simulation finished");
    }
  }

  // Update absolute time
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  Combined_Experiment_ArmOnly_M->Timing.t[0] =
    ((time_T)(++Combined_Experiment_ArmOnly_M->Timing.clockTick0)) *
    Combined_Experiment_ArmOnly_M->Timing.stepSize0;

  // Update absolute time
  // The "clockTick1" counts the number of times the code of this task has
  //  been executed. The resolution of this integer timer is 0.05, which is the step size
  //  of the task. Size of "clockTick1" ensures timer will not overflow during the
  //  application lifespan selected.

  Combined_Experiment_ArmOnly_M->Timing.clockTick1++;
}

// Model step function for TID2
void Combined_Experiment_ArmOnly_step2(void) // Sample time: [0.1s, 0.0s]
{
  boolean_T rtb_DigitalRead_0;
  real_T sampleTime;
  int32_T rtb_EnableSig;
  int32_T rtb_WheelDirection;
  real_T rtb_SaturateMotor;

  // Outputs for Atomic SubSystem: '<S3>/Send Commands to PWM Blocks'
  // MATLABSystem: '<S26>/RED T1 - BLACK T8' incorporates:
  //   RateTransition: '<S3>/Rate Transition'

  if (Combined_Experiment_ArmOnly_DW.RateTransition_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_m.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T1 - BLACK T8'

  // MATLABSystem: '<S26>/RED T2 - BLACK T3' incorporates:
  //   RateTransition: '<S3>/Rate Transition1'

  if (Combined_Experiment_ArmOnly_DW.RateTransition1_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition1_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_ma.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T2 - BLACK T3'

  // MATLABSystem: '<S26>/RED T3' incorporates:
  //   RateTransition: '<S3>/Rate Transition2'

  if (Combined_Experiment_ArmOnly_DW.RateTransition2_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition2_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_h.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T3'

  // MATLABSystem: '<S26>/RED T4 - BLACK T5' incorporates:
  //   RateTransition: '<S3>/Rate Transition3'

  if (Combined_Experiment_ArmOnly_DW.RateTransition3_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition3_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_k.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T4 - BLACK T5'

  // MATLABSystem: '<S26>/RED T5 - BLACK T4' incorporates:
  //   RateTransition: '<S3>/Rate Transition4'

  if (Combined_Experiment_ArmOnly_DW.RateTransition4_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition4_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_hw.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T5 - BLACK T4'

  // MATLABSystem: '<S26>/RED T6 - BLACK T7' incorporates:
  //   RateTransition: '<S3>/Rate Transition5'

  if (Combined_Experiment_ArmOnly_DW.RateTransition5_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition5_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_hf.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T6 - BLACK T7'

  // MATLABSystem: '<S26>/RED T7 - BLACK T6' incorporates:
  //   RateTransition: '<S3>/Rate Transition6'

  if (Combined_Experiment_ArmOnly_DW.RateTransition6_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition6_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_c.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T7 - BLACK T6'

  // MATLABSystem: '<S26>/RED T8 - BLACK T1' incorporates:
  //   RateTransition: '<S3>/Rate Transition7'

  if (Combined_Experiment_ArmOnly_DW.RateTransition7_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition7_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_fo.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/RED T8 - BLACK T1'

  // MATLABSystem: '<S26>/BLACK T2' incorporates:
  //   RateTransition: '<S3>/Rate Transition8'

  if (Combined_Experiment_ArmOnly_DW.RateTransition8_Buffer < 1.0) {
    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RateTransition8_Buffer;
  } else {
    rtb_SaturateMotor = 1.0;
  }

  if (!(rtb_SaturateMotor > 0.0)) {
    rtb_SaturateMotor = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_ci.PinNumber,
    rtb_SaturateMotor);

  // End of MATLABSystem: '<S26>/BLACK T2'
  // End of Outputs for SubSystem: '<S3>/Send Commands to PWM Blocks'

  // If: '<S4>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
  //   Constant: '<S4>/Constant'
  //   Constant: '<S57>/Constant'

  if ((Combined_Experiment_ArmOnly_P.WhoAmI == 1.0) ||
      (Combined_Experiment_ArmOnly_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S4>/Change RED Behavior' incorporates:
    //   ActionPort: '<S56>/Action Port'

    // Saturate: '<S58>/Saturate Torque'
    if (Combined_Experiment_ArmOnly_DW.RED_Tz_RW >
        Combined_Experiment_ArmOnly_P.SaturateTorque_UpperSat) {
      Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat =
        Combined_Experiment_ArmOnly_P.SaturateTorque_UpperSat;
    } else if (Combined_Experiment_ArmOnly_DW.RED_Tz_RW <
               Combined_Experiment_ArmOnly_P.SaturateTorque_LowerSat) {
      Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat =
        Combined_Experiment_ArmOnly_P.SaturateTorque_LowerSat;
    } else {
      Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat =
        Combined_Experiment_ArmOnly_DW.RED_Tz_RW;
    }

    // End of Saturate: '<S58>/Saturate Torque'

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    // MATLABSystem: '<S70>/Digital Read'
    if (Combined_Experiment_ArmOnly_DW.obj_fq.SampleTime !=
        Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime) {
      if (((!rtIsInf(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime)) && (
            !rtIsNaN(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime))) ||
          rtIsInf(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime)) {
        sampleTime = Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime;
      }

      Combined_Experiment_ArmOnly_DW.obj_fq.SampleTime = sampleTime;
    }

    rtb_DigitalRead_0 = MW_gpioRead(24U);

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Sum: '<S58>/Sum' incorporates:
    //   DataStoreWrite: '<S56>/RED_Tz_RW_Sat'
    //   Delay: '<S58>/Delay'
    //   MATLAB Function: '<S58>/Calculate RW RPM Increment'

    // MATLAB Function 'From Torque, Command RW/Change RED Behavior/Calculate Saturated RPM Signal/Calculate RW RPM Increment': '<S60>:1' 
    // '<S60>:1:3' dRPM_RW =  (Tz_Sat/0.007244720)*(60/2/pi)*0.1;
    sampleTime = Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat / 0.00724472 *
      9.5492965855137211 * 0.1 + Combined_Experiment_ArmOnly_DW.Delay_DSTATE;

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    // MATLAB Function: '<S59>/Check if RW is Ready' incorporates:
    //   DataStoreWrite: '<S59>/RED_RW_Speed'
    //   Delay: '<S63>/Delay1'
    //   Delay: '<S63>/Delay2'
    //   Delay: '<S63>/Delay3'
    //   Delay: '<S63>/Delay4'
    //   Delay: '<S63>/Delay5'
    //   MATLABSystem: '<S70>/Digital Read'
    //   MinMax: '<S63>/Mostly zeros?'

    // MATLAB Function 'From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Check if RW is Ready': '<S61>:1' 
    // '<S61>:1:3' if Status == 0
    if ((!rtb_DigitalRead_0) ||
        (!Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kt) ||
        (!Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[0U]) ||
        (!Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[0U]) ||
        (!Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[0U]) ||
        (!Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[0U])) {
      // '<S61>:1:4' EnableSig    = 1;
      rtb_EnableSig = 1;

      // '<S61>:1:5' WheelRPM_Out = WheelRPM_In;
      Combined_Experiment_ArmOnly_DW.RED_dRz_RW_Sat = sampleTime;
    } else {
      // '<S61>:1:6' else
      // '<S61>:1:7' EnableSig    = 0;
      rtb_EnableSig = 0;

      // '<S61>:1:8' WheelRPM_Out = 0;
      Combined_Experiment_ArmOnly_DW.RED_dRz_RW_Sat = 0.0;
    }

    // End of MATLAB Function: '<S59>/Check if RW is Ready'
    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Product: '<S62>/Divide' incorporates:
    //   Constant: '<S62>/Gearbox  Ratio'
    //   DataStoreWrite: '<S59>/RED_RW_Speed'

    rtb_SaturateMotor = Combined_Experiment_ArmOnly_DW.RED_dRz_RW_Sat *
      Combined_Experiment_ArmOnly_P.GearboxRatio_Value;

    // Saturate: '<S59>/Saturate Motor'
    if (rtb_SaturateMotor > Combined_Experiment_ArmOnly_P.SaturateMotor_UpperSat)
    {
      rtb_SaturateMotor = Combined_Experiment_ArmOnly_P.SaturateMotor_UpperSat;
    } else {
      if (rtb_SaturateMotor <
          Combined_Experiment_ArmOnly_P.SaturateMotor_LowerSat) {
        rtb_SaturateMotor = Combined_Experiment_ArmOnly_P.SaturateMotor_LowerSat;
      }
    }

    // End of Saturate: '<S59>/Saturate Motor'

    // MATLAB Function: '<S59>/Get RW Direction'
    // MATLAB Function 'From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/Get RW Direction': '<S64>:1' 
    // '<S64>:1:3' if MotorRPM_Sat > 0
    if (rtb_SaturateMotor > 0.0) {
      // '<S64>:1:4' WheelDirection = 0;
      rtb_WheelDirection = 0;
    } else {
      // '<S64>:1:5' else
      // '<S64>:1:6' WheelDirection = 1;
      rtb_WheelDirection = 1;
    }

    // End of MATLAB Function: '<S59>/Get RW Direction'

    // Outputs for Atomic SubSystem: '<S59>/Send Direction to Motor Controller'
    // MATLABSystem: '<S71>/Digital Write'
    MW_gpioWrite(21U, static_cast<uint8_T>(rtb_WheelDirection));

    // End of Outputs for SubSystem: '<S59>/Send Direction to Motor Controller'

    // MATLAB Function: '<S59>/RPM to PWM Value' incorporates:
    //   Abs: '<S59>/Abs'

    // MATLAB Function 'From Torque, Command RW/Change RED Behavior/Wheel RPM to PWM/RPM to PWM Value': '<S66>:1' 
    // '<S66>:1:3' PWMRange = 0.9-0.1;
    // '<S66>:1:5' MotorPWM = ((MotorRPM_Sat*PWMRange)/7000) + 0.1;
    rtb_SaturateMotor = fabs(rtb_SaturateMotor) * 0.8 / 7000.0 + 0.1;

    // Outputs for Atomic SubSystem: '<S59>/Send PWM to Motor Controller'
    // MATLABSystem: '<S68>/PWM1'
    if (!(rtb_SaturateMotor < 1.0)) {
      rtb_SaturateMotor = 1.0;
    }

    EXT_PWMBlock_setDutyCycle(Combined_Experiment_ArmOnly_DW.obj_d.PinNumber,
      rtb_SaturateMotor);

    // End of MATLABSystem: '<S68>/PWM1'
    // End of Outputs for SubSystem: '<S59>/Send PWM to Motor Controller'

    // Outputs for Atomic SubSystem: '<S59>/Turn on Motor'
    // MATLABSystem: '<S72>/Digital Write'
    MW_gpioWrite(25U, static_cast<uint8_T>(rtb_EnableSig));

    // End of Outputs for SubSystem: '<S59>/Turn on Motor'

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    // Update for Delay: '<S63>/Delay1' incorporates:
    //   MATLABSystem: '<S70>/Digital Read'

    Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kt = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Update for Delay: '<S63>/Delay2' incorporates:
    //   MATLABSystem: '<S70>/Digital Read'

    Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[0] =
      Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[1];

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[1] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Update for Delay: '<S63>/Delay5' incorporates:
    //   MATLABSystem: '<S70>/Digital Read'

    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[0] =
      Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[1];
    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[1] =
      Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[2];

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[2] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Update for Delay: '<S63>/Delay3' incorporates:
    //   MATLABSystem: '<S70>/Digital Read'

    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[0] =
      Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[1];
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[1] =
      Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[2];
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[2] =
      Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[3];

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[3] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Update for Delay: '<S63>/Delay4' incorporates:
    //   MATLABSystem: '<S70>/Digital Read'

    Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[0] =
      Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[1];
    Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[1] =
      Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[2];
    Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[2] =
      Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[3];
    Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[3] =
      Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[4];

    // Outputs for Atomic SubSystem: '<S59>/Obtain RW Status'
    Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[4] = rtb_DigitalRead_0;

    // End of Outputs for SubSystem: '<S59>/Obtain RW Status'

    // Saturate: '<S58>/Saturate RPM'
    if (sampleTime > Combined_Experiment_ArmOnly_P.SaturateRPM_UpperSat) {
      // Update for Delay: '<S58>/Delay'
      Combined_Experiment_ArmOnly_DW.Delay_DSTATE =
        Combined_Experiment_ArmOnly_P.SaturateRPM_UpperSat;
    } else if (sampleTime < Combined_Experiment_ArmOnly_P.SaturateRPM_LowerSat)
    {
      // Update for Delay: '<S58>/Delay'
      Combined_Experiment_ArmOnly_DW.Delay_DSTATE =
        Combined_Experiment_ArmOnly_P.SaturateRPM_LowerSat;
    } else {
      // Update for Delay: '<S58>/Delay'
      Combined_Experiment_ArmOnly_DW.Delay_DSTATE = sampleTime;
    }

    // End of Saturate: '<S58>/Saturate RPM'
    // End of Outputs for SubSystem: '<S4>/Change RED Behavior'
  }

  // End of If: '<S4>/This IF block determines whether or not to run the RED sim//exp ' 
}

// Model step wrapper function for compatibility with a static main program
void Combined_Experiment_ArmOnly_step(int_T tid)
{
  switch (tid) {
   case 0 :
    Combined_Experiment_ArmOnly_step0();
    break;

   case 2 :
    Combined_Experiment_ArmOnly_step2();
    break;

   default :
    break;
  }
}

// Model initialize function
void Combined_Experiment_ArmOnly_initialize(void)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&Combined_Experiment_ArmOnly_M->solverInfo,
                          &Combined_Experiment_ArmOnly_M->Timing.simTimeStep);
    rtsiSetTPtr(&Combined_Experiment_ArmOnly_M->solverInfo, &rtmGetTPtr
                (Combined_Experiment_ArmOnly_M));
    rtsiSetStepSizePtr(&Combined_Experiment_ArmOnly_M->solverInfo,
                       &Combined_Experiment_ArmOnly_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Combined_Experiment_ArmOnly_M->solverInfo,
                          (&rtmGetErrorStatus(Combined_Experiment_ArmOnly_M)));
    rtsiSetRTModelPtr(&Combined_Experiment_ArmOnly_M->solverInfo,
                      Combined_Experiment_ArmOnly_M);
  }

  rtsiSetSimTimeStep(&Combined_Experiment_ArmOnly_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Combined_Experiment_ArmOnly_M->solverInfo,
                    "FixedStepDiscrete");
  rtmSetTPtr(Combined_Experiment_ArmOnly_M,
             &Combined_Experiment_ArmOnly_M->Timing.tArray[0]);
  rtmSetTFinal(Combined_Experiment_ArmOnly_M, 291.0);
  Combined_Experiment_ArmOnly_M->Timing.stepSize0 = 0.05;

  // Setup for data logging
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    Combined_Experiment_ArmOnly_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  // Setup for data logging
  {
    rtliSetLogXSignalInfo(Combined_Experiment_ArmOnly_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Combined_Experiment_ArmOnly_M->rtwLogInfo, (NULL));
    rtliSetLogT(Combined_Experiment_ArmOnly_M->rtwLogInfo, "tout");
    rtliSetLogX(Combined_Experiment_ArmOnly_M->rtwLogInfo, "");
    rtliSetLogXFinal(Combined_Experiment_ArmOnly_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Combined_Experiment_ArmOnly_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Combined_Experiment_ArmOnly_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Combined_Experiment_ArmOnly_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Combined_Experiment_ArmOnly_M->rtwLogInfo, 1);
    rtliSetLogY(Combined_Experiment_ArmOnly_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Combined_Experiment_ArmOnly_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Combined_Experiment_ArmOnly_M->rtwLogInfo, (NULL));
  }

  // Matfile logging
  rt_StartDataLoggingWithStartTime(Combined_Experiment_ArmOnly_M->rtwLogInfo,
    0.0, rtmGetTFinal(Combined_Experiment_ArmOnly_M),
    Combined_Experiment_ArmOnly_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Combined_Experiment_ArmOnly_M)));

  {
    char_T *sErr;
    int32_T ret;
    uint32_T tseed;
    int32_T t;
    static const char_T tmp[45] =
      "Unable to configure pin %u for PWM output.\\n";

    // SetupRuntimeResources for ToWorkspace: '<S1>/To Workspace'
    {
      int_T dimensions[1] = { 64 };

      Combined_Experiment_ArmOnly_DW.ToWorkspace_PWORK.LoggedData =
        rt_CreateLogVar(
                        Combined_Experiment_ArmOnly_M->rtwLogInfo,
                        0.0,
                        rtmGetTFinal(Combined_Experiment_ArmOnly_M),
                        Combined_Experiment_ArmOnly_M->Timing.stepSize0,
                        (&rtmGetErrorStatus(Combined_Experiment_ArmOnly_M)),
                        "dataPacket",
                        SS_DOUBLE,
                        0,
                        0,
                        0,
                        64,
                        1,
                        dimensions,
                        NO_LOGVALDIMS,
                        (NULL),
                        (NULL),
                        0,
                        1,
                        0.05,
                        1);
      if (Combined_Experiment_ArmOnly_DW.ToWorkspace_PWORK.LoggedData == (NULL))
        return;
    }

    // Start for If: '<Root>/Separate Phases'
    Combined_Experiment_ArmOnly_DW.SeparatePhases_ActiveSubsystem = -1;

    // Start for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    Combined_Experiment_ArmOnly_DW.Ifperforminganexperimentgrabthe = -1;

    // Start for S-Function (sdspToNetwork): '<S15>/UDP Send'
    sErr = GetErrorBuffer(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
    CreateUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U],
                        1, "0.0.0.0", -1, "192.168.0.105",
                        Combined_Experiment_ArmOnly_P.UDPSend_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
        rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<S15>/UDP Send'

    // Start for DataStoreMemory: '<Root>/BLACK_Fx'
    Combined_Experiment_ArmOnly_DW.BLACK_Fx =
      Combined_Experiment_ArmOnly_P.BLACK_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx1'
    Combined_Experiment_ArmOnly_DW.BLUE_Fx =
      Combined_Experiment_ArmOnly_P.BLACK_Fx1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat'
    Combined_Experiment_ArmOnly_DW.BLACK_Fx_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat1'
    Combined_Experiment_ArmOnly_DW.ARM_Elbow_Py =
      Combined_Experiment_ArmOnly_P.BLACK_Fx_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat2'
    Combined_Experiment_ArmOnly_DW.BLUE_Fx_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Fx_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy'
    Combined_Experiment_ArmOnly_DW.BLACK_Fy =
      Combined_Experiment_ArmOnly_P.BLACK_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy1'
    Combined_Experiment_ArmOnly_DW.BLUE_Fy =
      Combined_Experiment_ArmOnly_P.BLACK_Fy1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat'
    Combined_Experiment_ArmOnly_DW.BLACK_Fy_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat1'
    Combined_Experiment_ArmOnly_DW.ARM_Wrist_Px =
      Combined_Experiment_ArmOnly_P.BLACK_Fy_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat2'
    Combined_Experiment_ArmOnly_DW.BLUE_Fy_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Fy_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px'
    Combined_Experiment_ArmOnly_DW.BLACK_Px =
      Combined_Experiment_ArmOnly_P.BLACK_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px1'
    Combined_Experiment_ArmOnly_DW.BLUE_Px =
      Combined_Experiment_ArmOnly_P.BLACK_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py'
    Combined_Experiment_ArmOnly_DW.BLACK_Py =
      Combined_Experiment_ArmOnly_P.BLACK_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py1'
    Combined_Experiment_ArmOnly_DW.BLUE_Py =
      Combined_Experiment_ArmOnly_P.BLACK_Py1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz'
    Combined_Experiment_ArmOnly_DW.BLACK_Rz =
      Combined_Experiment_ArmOnly_P.BLACK_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz1'
    Combined_Experiment_ArmOnly_DW.BLUE_Rz =
      Combined_Experiment_ArmOnly_P.BLACK_Rz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz'
    Combined_Experiment_ArmOnly_DW.BLACK_Tz =
      Combined_Experiment_ArmOnly_P.BLACK_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz1'
    Combined_Experiment_ArmOnly_DW.BLUE_Tz =
      Combined_Experiment_ArmOnly_P.BLACK_Tz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat'
    Combined_Experiment_ArmOnly_DW.BLACK_Tz_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat1'
    Combined_Experiment_ArmOnly_DW.ARM_Wrist_Py =
      Combined_Experiment_ArmOnly_P.BLACK_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat2'
    Combined_Experiment_ArmOnly_DW.BLUE_Tz_Sat =
      Combined_Experiment_ArmOnly_P.BLACK_Tz_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx'
    Combined_Experiment_ArmOnly_DW.RED_Fx =
      Combined_Experiment_ArmOnly_P.RED_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx_Sat'
    Combined_Experiment_ArmOnly_DW.RED_Fx_Sat =
      Combined_Experiment_ArmOnly_P.RED_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy'
    Combined_Experiment_ArmOnly_DW.RED_Fy =
      Combined_Experiment_ArmOnly_P.RED_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy_Sat'
    Combined_Experiment_ArmOnly_DW.RED_Fy_Sat =
      Combined_Experiment_ArmOnly_P.RED_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px'
    Combined_Experiment_ArmOnly_DW.RED_Px =
      Combined_Experiment_ArmOnly_P.RED_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px1'
    Combined_Experiment_ArmOnly_DW.RED_AHRS_Q =
      Combined_Experiment_ArmOnly_P.RED_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px10'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_b =
      Combined_Experiment_ArmOnly_P.RED_Px10_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px11'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_b =
      Combined_Experiment_ArmOnly_P.RED_Px11_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px12'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Az_b =
      Combined_Experiment_ArmOnly_P.RED_Px12_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px19'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Q =
      Combined_Experiment_ArmOnly_P.RED_Px19_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px2'
    Combined_Experiment_ArmOnly_DW.RED_AHRS_P =
      Combined_Experiment_ArmOnly_P.RED_Px2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px20'
    Combined_Experiment_ArmOnly_DW.RED_IMU_P =
      Combined_Experiment_ArmOnly_P.RED_Px20_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px21'
    Combined_Experiment_ArmOnly_DW.RED_IMU_R =
      Combined_Experiment_ArmOnly_P.RED_Px21_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px22'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Q =
      Combined_Experiment_ArmOnly_P.RED_Px22_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px23'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_P =
      Combined_Experiment_ArmOnly_P.RED_Px23_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px24'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_R =
      Combined_Experiment_ArmOnly_P.RED_Px24_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px25'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Psi =
      Combined_Experiment_ArmOnly_P.RED_Px25_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px26'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Psi =
      Combined_Experiment_ArmOnly_P.RED_Px26_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px3'
    Combined_Experiment_ArmOnly_DW.RED_AHRS_R =
      Combined_Experiment_ArmOnly_P.RED_Px3_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px31'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_I =
      Combined_Experiment_ArmOnly_P.RED_Px31_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px32'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_I =
      Combined_Experiment_ArmOnly_P.RED_Px32_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px37'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ax_I =
      Combined_Experiment_ArmOnly_P.RED_Px37_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px38'
    Combined_Experiment_ArmOnly_DW.BLACK_IMU_Ay_I =
      Combined_Experiment_ArmOnly_P.RED_Px38_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px4'
    Combined_Experiment_ArmOnly_DW.BLACK_AHRS_Q =
      Combined_Experiment_ArmOnly_P.RED_Px4_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px5'
    Combined_Experiment_ArmOnly_DW.BLACK_AHRS_P =
      Combined_Experiment_ArmOnly_P.RED_Px5_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px6'
    Combined_Experiment_ArmOnly_DW.BLACK_AHRS_R =
      Combined_Experiment_ArmOnly_P.RED_Px6_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px7'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Ax_b =
      Combined_Experiment_ArmOnly_P.RED_Px7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px8'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Ay_b =
      Combined_Experiment_ArmOnly_P.RED_Px8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px9'
    Combined_Experiment_ArmOnly_DW.RED_IMU_Az_b =
      Combined_Experiment_ArmOnly_P.RED_Px9_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Py'
    Combined_Experiment_ArmOnly_DW.RED_Py =
      Combined_Experiment_ArmOnly_P.RED_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Rz'
    Combined_Experiment_ArmOnly_DW.RED_Rz =
      Combined_Experiment_ArmOnly_P.RED_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz'
    Combined_Experiment_ArmOnly_DW.RED_Tz =
      Combined_Experiment_ArmOnly_P.RED_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz7'
    Combined_Experiment_ArmOnly_DW.Float_State =
      Combined_Experiment_ArmOnly_P.RED_Tz7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz8'
    Combined_Experiment_ArmOnly_DW.Magnet_State =
      Combined_Experiment_ArmOnly_P.RED_Tz8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW'
    Combined_Experiment_ArmOnly_DW.RED_Tz_RW =
      Combined_Experiment_ArmOnly_P.RED_Tz_RW_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat'
    Combined_Experiment_ArmOnly_DW.RED_Tz_RW_Sat =
      Combined_Experiment_ArmOnly_P.RED_Tz_RWSat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat1'
    Combined_Experiment_ArmOnly_DW.ARM_EndEff_Py =
      Combined_Experiment_ArmOnly_P.RED_Tz_RWSat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW1'
    Combined_Experiment_ArmOnly_DW.ARM_EndEff_Px =
      Combined_Experiment_ArmOnly_P.RED_Tz_RW1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat'
    Combined_Experiment_ArmOnly_DW.RED_Tz_Sat =
      Combined_Experiment_ArmOnly_P.RED_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat1'
    Combined_Experiment_ArmOnly_DW.ARM_Elbow_Px =
      Combined_Experiment_ArmOnly_P.RED_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_dRz_RW Sat'
    Combined_Experiment_ArmOnly_DW.RED_dRz_RW_Sat =
      Combined_Experiment_ArmOnly_P.RED_dRz_RWSat_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time'
    Combined_Experiment_ArmOnly_DW.Univ_Time =
      Combined_Experiment_ArmOnly_P.Universal_Time_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time1'
    Combined_Experiment_ArmOnly_DW.RED_Vx =
      Combined_Experiment_ArmOnly_P.Universal_Time1_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time10'
    Combined_Experiment_ArmOnly_DW.RED_Ax =
      Combined_Experiment_ArmOnly_P.Universal_Time10_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time11'
    Combined_Experiment_ArmOnly_DW.RED_Ay =
      Combined_Experiment_ArmOnly_P.Universal_Time11_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time12'
    Combined_Experiment_ArmOnly_DW.BLACK_Ax =
      Combined_Experiment_ArmOnly_P.Universal_Time12_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time13'
    Combined_Experiment_ArmOnly_DW.BLACK_Ay =
      Combined_Experiment_ArmOnly_P.Universal_Time13_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time14'
    Combined_Experiment_ArmOnly_DW.BLACK_RzDD =
      Combined_Experiment_ArmOnly_P.Universal_Time14_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time15'
    Combined_Experiment_ArmOnly_DW.RED_RzDD =
      Combined_Experiment_ArmOnly_P.Universal_Time15_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time2'
    Combined_Experiment_ArmOnly_DW.RED_Vy =
      Combined_Experiment_ArmOnly_P.Universal_Time2_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time3'
    Combined_Experiment_ArmOnly_DW.RED_RzD =
      Combined_Experiment_ArmOnly_P.Universal_Time3_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time4'
    Combined_Experiment_ArmOnly_DW.BLACK_Vx =
      Combined_Experiment_ArmOnly_P.Universal_Time4_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time5'
    Combined_Experiment_ArmOnly_DW.BLACK_Vy =
      Combined_Experiment_ArmOnly_P.Universal_Time5_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time6'
    Combined_Experiment_ArmOnly_DW.BLACK_RzD =
      Combined_Experiment_ArmOnly_P.Universal_Time6_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time7'
    Combined_Experiment_ArmOnly_DW.BLUE_Vx =
      Combined_Experiment_ArmOnly_P.Universal_Time7_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time8'
    Combined_Experiment_ArmOnly_DW.BLUE_Vy =
      Combined_Experiment_ArmOnly_P.Universal_Time8_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time9'
    Combined_Experiment_ArmOnly_DW.BLUE_RzD =
      Combined_Experiment_ArmOnly_P.Universal_Time9_InitialValue;
    Combined_Experiment_Arm_PrevZCX.SampleandHold1_Trig_ZCE_c =
      UNINITIALIZED_ZCSIG;
    Combined_Experiment_Arm_PrevZCX.SampleandHold1_Trig_ZCE =
      UNINITIALIZED_ZCSIG;
    Combined_Experiment_Arm_PrevZCX.SampleandHold_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #0:  Wait for Synchronization' 
    Phase0WaitforSynchroni_Init
      (&Combined_Experiment_ArmOnly_DW.Phase0WaitforSynchronization);

    // End of SystemInitialize for SubSystem: '<Root>/Phase #0:  Wait for Synchronization' 

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #1:  Start Floating ' 
    Phase0WaitforSynchroni_Init
      (&Combined_Experiment_ArmOnly_DW.Phase1StartFloating);

    // End of SystemInitialize for SubSystem: '<Root>/Phase #1:  Start Floating ' 

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    // Start for If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_dy = -1;

    // Start for If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jm = -1;

    // Start for If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_aj = -1;

    // SystemInitialize for IfAction SubSystem: '<S10>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S284>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ns = 1U;

    // InitializeConditions for Delay: '<S286>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_k1 = 1U;

    // InitializeConditions for Delay: '<S282>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ep = 1U;

    // SystemInitialize for IfAction SubSystem: '<S284>/Hold this value'
    // SystemInitialize for Outport: '<S285>/Out1'
    Combined_Experiment_ArmOnly_B.In1_e =
      Combined_Experiment_ArmOnly_P.Out1_Y0_av;

    // End of SystemInitialize for SubSystem: '<S284>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S286>/Hold this value'
    // SystemInitialize for Outport: '<S287>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gc =
      Combined_Experiment_ArmOnly_P.Out1_Y0_g1;

    // End of SystemInitialize for SubSystem: '<S286>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S282>/Hold this value'
    // SystemInitialize for Outport: '<S283>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fau =
      Combined_Experiment_ArmOnly_P.Out1_Y0_mp;

    // End of SystemInitialize for SubSystem: '<S282>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S10>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S10>/Change BLUE Behavior'
    // InitializeConditions for Delay: '<S297>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_dy = 1U;

    // InitializeConditions for Delay: '<S299>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_hf = 1U;

    // InitializeConditions for Delay: '<S295>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_kl = 1U;

    // SystemInitialize for IfAction SubSystem: '<S297>/Hold this value'
    // SystemInitialize for Outport: '<S298>/Out1'
    Combined_Experiment_ArmOnly_B.In1_lc =
      Combined_Experiment_ArmOnly_P.Out1_Y0_a4;

    // End of SystemInitialize for SubSystem: '<S297>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S299>/Hold this value'
    // SystemInitialize for Outport: '<S300>/Out1'
    Combined_Experiment_ArmOnly_B.In1_jw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_nv;

    // End of SystemInitialize for SubSystem: '<S299>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S295>/Hold this value'
    // SystemInitialize for Outport: '<S296>/Out1'
    Combined_Experiment_ArmOnly_B.In1_o =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ovt;

    // End of SystemInitialize for SubSystem: '<S295>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S10>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S10>/Change RED Behavior'
    // InitializeConditions for Delay: '<S308>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ji = 1U;

    // InitializeConditions for Delay: '<S310>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a1 = 1U;

    // InitializeConditions for Delay: '<S312>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_o0 = 1U;

    // SystemInitialize for IfAction SubSystem: '<S308>/Hold this value'
    // SystemInitialize for Outport: '<S309>/Out1'
    Combined_Experiment_ArmOnly_B.In1_k =
      Combined_Experiment_ArmOnly_P.Out1_Y0_frl;

    // End of SystemInitialize for SubSystem: '<S308>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S310>/Hold this value'
    // SystemInitialize for Outport: '<S311>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cr =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bka;

    // End of SystemInitialize for SubSystem: '<S310>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S312>/Hold this value'
    // SystemInitialize for Outport: '<S313>/Out1'
    Combined_Experiment_ArmOnly_B.In1_if =
      Combined_Experiment_ArmOnly_P.Out1_Y0_j2;

    // End of SystemInitialize for SubSystem: '<S312>/Hold this value'

    // Constant: '<S276>/Constant7' incorporates:
    //   Constant: '<S276>/Constant8'
    //   Constant: '<S276>/Constant9'

    SendPositionCommandsto_Init
      (&Combined_Experiment_ArmOnly_DW.SendPositionCommandstoManipulat);

    // End of SystemInitialize for SubSystem: '<S10>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #3: Experiment'
    // Start for If: '<S11>/This IF block determines whether or not to run the BLACK sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_p = -1;

    // Start for If: '<S11>/This IF block determines whether or not to run the BLUE sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_pu = -1;

    // Start for If: '<S11>/This IF block determines whether or not to run the RED sim//exp ' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_au = -1;

    // SystemInitialize for IfAction SubSystem: '<S11>/Change BLACK Behavior'
    // Start for If: '<S317>/Experiment Sub-Phases'
    Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_o = -1;

    // SystemInitialize for IfAction SubSystem: '<S317>/Sub-Phase #1'
    // InitializeConditions for Delay: '<S332>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ot = 1U;

    // InitializeConditions for Delay: '<S334>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_k = 1U;

    // InitializeConditions for Delay: '<S330>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_l = 1U;

    // SystemInitialize for IfAction SubSystem: '<S332>/Hold this value'
    // SystemInitialize for Outport: '<S333>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bd;

    // End of SystemInitialize for SubSystem: '<S332>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S334>/Hold this value'
    // SystemInitialize for Outport: '<S335>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ly =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ai;

    // End of SystemInitialize for SubSystem: '<S334>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S330>/Hold this value'
    // SystemInitialize for Outport: '<S331>/Out1'
    Combined_Experiment_ArmOnly_B.In1_a =
      Combined_Experiment_ArmOnly_P.Out1_Y0_pn;

    // End of SystemInitialize for SubSystem: '<S330>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S317>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S317>/Sub-Phase #4'
    // InitializeConditions for Delay: '<S349>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_fr = 1U;

    // InitializeConditions for Delay: '<S351>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_me = 1U;

    // InitializeConditions for Delay: '<S347>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ey = 1U;

    // SystemInitialize for IfAction SubSystem: '<S349>/Hold this value'
    // SystemInitialize for Outport: '<S350>/Out1'
    Combined_Experiment_ArmOnly_B.In1_l =
      Combined_Experiment_ArmOnly_P.Out1_Y0_dz;

    // End of SystemInitialize for SubSystem: '<S349>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S351>/Hold this value'
    // SystemInitialize for Outport: '<S352>/Out1'
    Combined_Experiment_ArmOnly_B.In1_n =
      Combined_Experiment_ArmOnly_P.Out1_Y0_jz;

    // End of SystemInitialize for SubSystem: '<S351>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S347>/Hold this value'
    // SystemInitialize for Outport: '<S348>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ck =
      Combined_Experiment_ArmOnly_P.Out1_Y0_pz;

    // End of SystemInitialize for SubSystem: '<S347>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S317>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S11>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S11>/Change BLUE Behavior'
    // Start for If: '<S318>/Experiment Sub-Phases'
    Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSub_e = -1;

    // SystemInitialize for IfAction SubSystem: '<S318>/Sub-Phase #1'
    Combined_Exp_SubPhase1_Init(&Combined_Experiment_ArmOnly_B.SubPhase1_a,
      &Combined_Experiment_ArmOnly_DW.SubPhase1_a,
      &Combined_Experiment_ArmOnly_P.SubPhase1_a);

    // End of SystemInitialize for SubSystem: '<S318>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S318>/Sub-Phase #4'
    Combined_Exp_SubPhase1_Init(&Combined_Experiment_ArmOnly_B.SubPhase4_f,
      &Combined_Experiment_ArmOnly_DW.SubPhase4_f,
      &Combined_Experiment_ArmOnly_P.SubPhase4_f);

    // End of SystemInitialize for SubSystem: '<S318>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S11>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S11>/Change RED Behavior'
    // Start for If: '<S319>/Experiment Sub-Phases'
    Combined_Experiment_ArmOnly_DW.ExperimentSubPhases_ActiveSubsy = -1;

    // SystemInitialize for IfAction SubSystem: '<S319>/Sub-Phase #3 '
    // Start for FromWorkspace: '<S389>/From Workspace'
    {
      static real_T pTimeValues0[] = { 41.0, 42.206991377503115,
        44.918392023417425, 48.831125752843334, 53.5208156350042,
        58.4791843649958, 63.168874247156666, 67.081607976582575,
        69.793008622496885, 71.0 } ;

      static real_T pDataValues0[] = { 1.5707963267948966, 1.3002983889406583,
        0.80615377281438272, 1.5707963267948966, 1.1750912433041067,
        -0.15206197308470215, 0.11259519095605781, 0.20110758420896063,
        -0.82711536531092233, -1.1344 } ;

      Combined_Experiment_ArmOnly_DW.FromWorkspace_PWORK.TimePtr = static_cast<
        void *>(pTimeValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace_PWORK.DataPtr = static_cast<
        void *>(pDataValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace_IWORK.PrevIndex = 0;
    }

    // Start for FromWorkspace: '<S389>/From Workspace1'
    {
      static real_T pTimeValues0[] = { 41.0, 42.206991377503115,
        44.918392023417425, 48.831125752843334, 53.5208156350042,
        58.4791843649958, 63.168874247156666, 67.081607976582575,
        69.793008622496885, 71.0 } ;

      static real_T pDataValues0[] = { 1.5707963267948966, 1.4781668052876038,
        1.5707963267948966, 1.5638709046340737, -0.0630212155184412,
        0.12858933283107898, 0.93209955596794036, -0.4668866764643379,
        -1.4020208855466245, -1.4835 } ;

      Combined_Experiment_ArmOnly_DW.FromWorkspace1_PWORK.TimePtr = static_cast<
        void *>(pTimeValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace1_PWORK.DataPtr = static_cast<
        void *>(pDataValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace1_IWORK.PrevIndex = 0;
    }

    // Start for FromWorkspace: '<S389>/From Workspace2'
    {
      static real_T pTimeValues0[] = { 41.0, 42.206991377503115,
        44.918392023417425, 48.831125752843334, 53.5208156350042,
        58.4791843649958, 63.168874247156666, 67.081607976582575,
        69.793008622496885, 71.0 } ;

      static real_T pDataValues0[] = { 0.0, 0.32106677845700848,
        0.063289639751335822, 0.25869954181959726, 1.5707963267948966,
        -1.5707963267948966, -0.67476648557754959, 0.095555010376457611,
        0.58930730169040446, 1.0471 } ;

      Combined_Experiment_ArmOnly_DW.FromWorkspace2_PWORK.TimePtr = static_cast<
        void *>(pTimeValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace2_PWORK.DataPtr = static_cast<
        void *>(pDataValues0);
      Combined_Experiment_ArmOnly_DW.FromWorkspace2_IWORK.PrevIndex = 0;
    }

    SendPositionCommandsto_Init
      (&Combined_Experiment_ArmOnly_DW.SendPositionCommandstoManipul_p);

    // End of SystemInitialize for SubSystem: '<S319>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S319>/Sub-Phase #4'
    // InitializeConditions for Delay: '<S400>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_jl = 1U;

    // InitializeConditions for Delay: '<S402>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_n = 1U;

    // InitializeConditions for Delay: '<S404>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_g = 1U;

    // SystemInitialize for IfAction SubSystem: '<S400>/Hold this value'
    // SystemInitialize for Outport: '<S401>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fa =
      Combined_Experiment_ArmOnly_P.Out1_Y0_pz4;

    // End of SystemInitialize for SubSystem: '<S400>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S402>/Hold this value'
    // SystemInitialize for Outport: '<S403>/Out1'
    Combined_Experiment_ArmOnly_B.In1_go =
      Combined_Experiment_ArmOnly_P.Out1_Y0_of;

    // End of SystemInitialize for SubSystem: '<S402>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S404>/Hold this value'
    // SystemInitialize for Outport: '<S405>/Out1'
    Combined_Experiment_ArmOnly_B.In1_g4 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ko;

    // End of SystemInitialize for SubSystem: '<S404>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S319>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S11>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #3: Experiment'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #4:  Return Home'
    // Start for If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_d = -1;

    // Start for If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_jc = -1;

    // Start for If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_a4 = -1;

    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S420>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_dq = 1U;

    // InitializeConditions for Delay: '<S422>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_h = 1U;

    // InitializeConditions for Delay: '<S418>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_o = 1U;

    // SystemInitialize for IfAction SubSystem: '<S420>/Hold this value'
    // SystemInitialize for Outport: '<S421>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fx =
      Combined_Experiment_ArmOnly_P.Out1_Y0_mr;

    // End of SystemInitialize for SubSystem: '<S420>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S422>/Hold this value'
    // SystemInitialize for Outport: '<S423>/Out1'
    Combined_Experiment_ArmOnly_B.In1_d0 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_i0r;

    // End of SystemInitialize for SubSystem: '<S422>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S418>/Hold this value'
    // SystemInitialize for Outport: '<S419>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fk =
      Combined_Experiment_ArmOnly_P.Out1_Y0_jr;

    // End of SystemInitialize for SubSystem: '<S418>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLUE Behavior'
    C_ChangeBLUEBehavior_b_Init
      (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_gj,
       &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_gj,
       &Combined_Experiment_ArmOnly_P.ChangeBLUEBehavior_gj);

    // End of SystemInitialize for SubSystem: '<S12>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S12>/Change RED Behavior'
    // InitializeConditions for Delay: '<S444>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_dh = 1U;

    // InitializeConditions for Delay: '<S446>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a2 = 1U;

    // InitializeConditions for Delay: '<S448>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_f = 1U;

    // SystemInitialize for IfAction SubSystem: '<S444>/Hold this value'
    // SystemInitialize for Outport: '<S445>/Out1'
    Combined_Experiment_ArmOnly_B.In1_h =
      Combined_Experiment_ArmOnly_P.Out1_Y0_nf;

    // End of SystemInitialize for SubSystem: '<S444>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S446>/Hold this value'
    // SystemInitialize for Outport: '<S447>/Out1'
    Combined_Experiment_ArmOnly_B.In1_b =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gb;

    // End of SystemInitialize for SubSystem: '<S446>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S448>/Hold this value'
    // SystemInitialize for Outport: '<S449>/Out1'
    Combined_Experiment_ArmOnly_B.In1_f =
      Combined_Experiment_ArmOnly_P.Out1_Y0_da;

    // End of SystemInitialize for SubSystem: '<S448>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #4:  Return Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #5:  Hold Home'
    // Start for If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_b = -1;

    // Start for If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_n = -1;

    // Start for If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_j = -1;

    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S463>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_e = 1U;

    // InitializeConditions for Delay: '<S465>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_m = 1U;

    // InitializeConditions for Delay: '<S461>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_d = 1U;

    // SystemInitialize for IfAction SubSystem: '<S463>/Hold this value'
    // SystemInitialize for Outport: '<S464>/Out1'
    Combined_Experiment_ArmOnly_B.In1_i =
      Combined_Experiment_ArmOnly_P.Out1_Y0_fj;

    // End of SystemInitialize for SubSystem: '<S463>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S465>/Hold this value'
    // SystemInitialize for Outport: '<S466>/Out1'
    Combined_Experiment_ArmOnly_B.In1_c =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bm;

    // End of SystemInitialize for SubSystem: '<S465>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S461>/Hold this value'
    // SystemInitialize for Outport: '<S462>/Out1'
    Combined_Experiment_ArmOnly_B.In1_g =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gx1;

    // End of SystemInitialize for SubSystem: '<S461>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S13>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLUE Behavior'
    C_ChangeBLUEBehavior_b_Init
      (&Combined_Experiment_ArmOnly_B.ChangeBLUEBehavior_b,
       &Combined_Experiment_ArmOnly_DW.ChangeBLUEBehavior_b,
       &Combined_Experiment_ArmOnly_P.ChangeBLUEBehavior_b);

    // End of SystemInitialize for SubSystem: '<S13>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change RED Behavior'
    // InitializeConditions for Delay: '<S489>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad = 1U;

    // InitializeConditions for Delay: '<S491>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_j = 1U;

    // InitializeConditions for Delay: '<S487>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a = 1U;

    // SystemInitialize for IfAction SubSystem: '<S489>/Hold this value'
    // SystemInitialize for Outport: '<S490>/Out1'
    Combined_Experiment_ArmOnly_B.In1_j =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bs;

    // End of SystemInitialize for SubSystem: '<S489>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S491>/Hold this value'
    // SystemInitialize for Outport: '<S492>/Out1'
    Combined_Experiment_ArmOnly_B.In1 = Combined_Experiment_ArmOnly_P.Out1_Y0_c5;

    // End of SystemInitialize for SubSystem: '<S491>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S487>/Hold this value'
    // SystemInitialize for Outport: '<S488>/Out1'
    Combined_Experiment_ArmOnly_B.In1_d =
      Combined_Experiment_ArmOnly_P.Out1_Y0_i3;

    // End of SystemInitialize for SubSystem: '<S487>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S13>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #5:  Hold Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' 
    // InitializeConditions for DiscreteIntegrator: '<S500>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[0] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC;

    // InitializeConditions for DiscreteIntegrator: '<S500>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[0] =
      Combined_Experiment_ArmOnly_P.drop_states_BLACK[0];

    // InitializeConditions for DiscreteIntegrator: '<S500>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[1] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC;

    // InitializeConditions for DiscreteIntegrator: '<S500>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[1] =
      Combined_Experiment_ArmOnly_P.drop_states_BLACK[1];

    // InitializeConditions for DiscreteIntegrator: '<S500>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE[2] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC;

    // InitializeConditions for DiscreteIntegrator: '<S500>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE[2] =
      Combined_Experiment_ArmOnly_P.drop_states_BLACK[2];

    // InitializeConditions for RandomNumber: '<S500>/Random Number'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput = Comb_rt_nrand_Upu32_Yd_f_pw_snf(
      &Combined_Experiment_ArmOnly_DW.RandSeed) * sqrt
      (Combined_Experiment_ArmOnly_P.noise_variance_BLACK) +
      Combined_Experiment_ArmOnly_P.RandomNumber_Mean;

    // End of InitializeConditions for RandomNumber: '<S500>/Random Number'

    // InitializeConditions for Delay: '<S518>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_lm = 1U;

    // InitializeConditions for Delay: '<S512>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_em = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number1'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber1_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_p = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_g =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_p)
      * Combined_Experiment_ArmOnly_P.RandomNumber1_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber1_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number1'

    // InitializeConditions for Delay: '<S519>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_nw = 1U;

    // InitializeConditions for Delay: '<S513>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_og = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber_Seed_d);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_i = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_h =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_i)
      * Combined_Experiment_ArmOnly_P.RandomNumber_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber_Mean_g;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number'

    // InitializeConditions for Delay: '<S520>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ka = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number2'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber2_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_b = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_e =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_b)
      * Combined_Experiment_ArmOnly_P.RandomNumber2_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber2_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number2'

    // InitializeConditions for DiscreteIntegrator: '<S501>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[0] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_f;

    // InitializeConditions for DiscreteIntegrator: '<S501>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[0] =
      Combined_Experiment_ArmOnly_P.drop_states_BLUE[0];

    // InitializeConditions for DiscreteIntegrator: '<S501>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[1] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_f;

    // InitializeConditions for DiscreteIntegrator: '<S501>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[1] =
      Combined_Experiment_ArmOnly_P.drop_states_BLUE[1];

    // InitializeConditions for DiscreteIntegrator: '<S501>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_k[2] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_f;

    // InitializeConditions for DiscreteIntegrator: '<S501>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_p[2] =
      Combined_Experiment_ArmOnly_P.drop_states_BLUE[2];

    // InitializeConditions for RandomNumber: '<S501>/Random Number'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber_Seed_a);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_m = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_m =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_m)
      * sqrt(Combined_Experiment_ArmOnly_P.noise_variance_BLUE) +
      Combined_Experiment_ArmOnly_P.RandomNumber_Mean_o;

    // End of InitializeConditions for RandomNumber: '<S501>/Random Number'

    // InitializeConditions for Delay: '<S521>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_lf = 1U;

    // InitializeConditions for Delay: '<S514>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_aq = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number4'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber4_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_bw = tseed;
    Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_bw);

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number4'

    // InitializeConditions for Delay: '<S522>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_nc = 1U;

    // InitializeConditions for Delay: '<S515>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_kx = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number3'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber3_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_n = tseed;
    Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_n);

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number3'

    // InitializeConditions for Delay: '<S523>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_fa = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number5'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber5_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_g = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_f =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_g)
      * Combined_Experiment_ArmOnly_P.RandomNumber5_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber5_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number5'

    // InitializeConditions for DiscreteIntegrator: '<S16>/Discrete-Time Integrator' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_PrevRese = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator_IC_LOADI = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S16>/Discrete-Time Integrator1' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevRes = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LOAD = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S16>/Discrete-Time Integrator2' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_PrevRes = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator2_IC_LOAD = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S505>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[0] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_k;

    // InitializeConditions for DiscreteIntegrator: '<S505>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[0] =
      Combined_Experiment_ArmOnly_P.drop_states_RED[0];

    // InitializeConditions for DiscreteIntegrator: '<S505>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[1] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_k;

    // InitializeConditions for DiscreteIntegrator: '<S505>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[1] =
      Combined_Experiment_ArmOnly_P.drop_states_RED[1];

    // InitializeConditions for DiscreteIntegrator: '<S505>/Acceleration  to Velocity' 
    Combined_Experiment_ArmOnly_DW.AccelerationtoVelocity_DSTATE_j[2] =
      Combined_Experiment_ArmOnly_P.AccelerationtoVelocity_IC_k;

    // InitializeConditions for DiscreteIntegrator: '<S505>/Velocity to Position' 
    Combined_Experiment_ArmOnly_DW.VelocitytoPosition_DSTATE_h[2] =
      Combined_Experiment_ArmOnly_P.drop_states_RED[2];

    // InitializeConditions for RandomNumber: '<S505>/Random Number'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber_Seed_n);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_pz = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_l =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf
      (&Combined_Experiment_ArmOnly_DW.RandSeed_pz) * sqrt
      (Combined_Experiment_ArmOnly_P.noise_variance_RED) +
      Combined_Experiment_ArmOnly_P.RandomNumber_Mean_n;

    // End of InitializeConditions for RandomNumber: '<S505>/Random Number'

    // InitializeConditions for Delay: '<S509>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ky = 1U;

    // InitializeConditions for Delay: '<S510>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_eg = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number7'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber7_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_a = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_n =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf(&Combined_Experiment_ArmOnly_DW.RandSeed_a)
      * Combined_Experiment_ArmOnly_P.RandomNumber7_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber7_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number7'

    // InitializeConditions for Delay: '<S516>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_h5 = 1U;

    // InitializeConditions for Delay: '<S511>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_p = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number6'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber6_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_b3 = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_hl =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf
      (&Combined_Experiment_ArmOnly_DW.RandSeed_b3) *
      Combined_Experiment_ArmOnly_P.RandomNumber6_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber6_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number6'

    // InitializeConditions for Delay: '<S517>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_c = 1U;

    // InitializeConditions for RandomNumber: '<S16>/Random Number8'
    Combined_Experiment_ArmOnly_B.d = floor
      (Combined_Experiment_ArmOnly_P.RandomNumber8_Seed);
    if (rtIsNaN(Combined_Experiment_ArmOnly_B.d) || rtIsInf
        (Combined_Experiment_ArmOnly_B.d)) {
      Combined_Experiment_ArmOnly_B.d = 0.0;
    } else {
      Combined_Experiment_ArmOnly_B.d = fmod(Combined_Experiment_ArmOnly_B.d,
        4.294967296E+9);
    }

    tseed = Combined_Experiment_ArmOnly_B.d < 0.0 ? static_cast<uint32_T>(-
      static_cast<int32_T>(static_cast<uint32_T>
      (-Combined_Experiment_ArmOnly_B.d))) : static_cast<uint32_T>
      (Combined_Experiment_ArmOnly_B.d);
    ret = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(ret) << 16U)) + t) << 16U) + t) +
      ret;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Combined_Experiment_ArmOnly_DW.RandSeed_iv = tseed;
    Combined_Experiment_ArmOnly_DW.NextOutput_fw =
      Comb_rt_nrand_Upu32_Yd_f_pw_snf
      (&Combined_Experiment_ArmOnly_DW.RandSeed_iv) *
      Combined_Experiment_ArmOnly_P.RandomNumber8_StdDev +
      Combined_Experiment_ArmOnly_P.RandomNumber8_Mean;

    // End of InitializeConditions for RandomNumber: '<S16>/Random Number8'

    // SystemInitialize for IfAction SubSystem: '<S512>/Hold this value'
    // SystemInitialize for Outport: '<S536>/Out1'
    Combined_Experiment_ArmOnly_B.In1_k0 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_fg;

    // End of SystemInitialize for SubSystem: '<S512>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S513>/Hold this value'
    // SystemInitialize for Outport: '<S537>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_pt;

    // End of SystemInitialize for SubSystem: '<S513>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S518>/Hold this value'
    // SystemInitialize for Outport: '<S542>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gi =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ls;

    // End of SystemInitialize for SubSystem: '<S518>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S519>/Hold this value'
    // SystemInitialize for Outport: '<S543>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fu =
      Combined_Experiment_ArmOnly_P.Out1_Y0_c4g;

    // End of SystemInitialize for SubSystem: '<S519>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S520>/Hold this value'
    // SystemInitialize for Outport: '<S544>/Out1'
    Combined_Experiment_ArmOnly_B.In1_m =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gp;

    // End of SystemInitialize for SubSystem: '<S520>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S521>/Hold this value'
    // SystemInitialize for Outport: '<S545>/Out1'
    Combined_Experiment_ArmOnly_B.In1_b0 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_m2;

    // End of SystemInitialize for SubSystem: '<S521>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S522>/Hold this value'
    // SystemInitialize for Outport: '<S546>/Out1'
    Combined_Experiment_ArmOnly_B.In1_hg =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gr;

    // End of SystemInitialize for SubSystem: '<S522>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S523>/Hold this value'
    // SystemInitialize for Outport: '<S547>/Out1'
    Combined_Experiment_ArmOnly_B.In1_n0 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gq;

    // End of SystemInitialize for SubSystem: '<S523>/Hold this value'

    // SystemInitialize for Triggered SubSystem: '<S506>/Sample and Hold1'
    // SystemInitialize for Outport: '<S528>/ '
    Combined_Experiment_ArmOnly_B.In_l[0] = Combined_Experiment_ArmOnly_P._Y0[0];

    // End of SystemInitialize for SubSystem: '<S506>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S507>/Sample and Hold1'
    // SystemInitialize for Outport: '<S530>/ '
    Combined_Experiment_ArmOnly_B.In_k[0] = Combined_Experiment_ArmOnly_P._Y0_g
      [0];

    // End of SystemInitialize for SubSystem: '<S507>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S508>/Sample and Hold'
    // SystemInitialize for Outport: '<S532>/ '
    Combined_Experiment_ArmOnly_B.In[0] = Combined_Experiment_ArmOnly_P._Y0_e[0];

    // End of SystemInitialize for SubSystem: '<S508>/Sample and Hold'

    // SystemInitialize for Triggered SubSystem: '<S506>/Sample and Hold1'
    // SystemInitialize for Outport: '<S528>/ '
    Combined_Experiment_ArmOnly_B.In_l[1] = Combined_Experiment_ArmOnly_P._Y0[1];

    // End of SystemInitialize for SubSystem: '<S506>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S507>/Sample and Hold1'
    // SystemInitialize for Outport: '<S530>/ '
    Combined_Experiment_ArmOnly_B.In_k[1] = Combined_Experiment_ArmOnly_P._Y0_g
      [1];

    // End of SystemInitialize for SubSystem: '<S507>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S508>/Sample and Hold'
    // SystemInitialize for Outport: '<S532>/ '
    Combined_Experiment_ArmOnly_B.In[1] = Combined_Experiment_ArmOnly_P._Y0_e[1];

    // End of SystemInitialize for SubSystem: '<S508>/Sample and Hold'

    // SystemInitialize for Triggered SubSystem: '<S506>/Sample and Hold1'
    // SystemInitialize for Outport: '<S528>/ '
    Combined_Experiment_ArmOnly_B.In_l[2] = Combined_Experiment_ArmOnly_P._Y0[2];

    // End of SystemInitialize for SubSystem: '<S506>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S507>/Sample and Hold1'
    // SystemInitialize for Outport: '<S530>/ '
    Combined_Experiment_ArmOnly_B.In_k[2] = Combined_Experiment_ArmOnly_P._Y0_g
      [2];

    // End of SystemInitialize for SubSystem: '<S507>/Sample and Hold1'

    // SystemInitialize for Triggered SubSystem: '<S508>/Sample and Hold'
    // SystemInitialize for Outport: '<S532>/ '
    Combined_Experiment_ArmOnly_B.In[2] = Combined_Experiment_ArmOnly_P._Y0_e[2];

    // End of SystemInitialize for SubSystem: '<S508>/Sample and Hold'

    // SystemInitialize for IfAction SubSystem: '<S509>/Hold this value'
    // SystemInitialize for Outport: '<S533>/Out1'
    Combined_Experiment_ArmOnly_B.In1_p =
      Combined_Experiment_ArmOnly_P.Out1_Y0_cg;

    // End of SystemInitialize for SubSystem: '<S509>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S510>/Hold this value'
    // SystemInitialize for Outport: '<S534>/Out1'
    Combined_Experiment_ArmOnly_B.In1_kd =
      Combined_Experiment_ArmOnly_P.Out1_Y0_lc;

    // End of SystemInitialize for SubSystem: '<S510>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S511>/Hold this value'
    // SystemInitialize for Outport: '<S535>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ed =
      Combined_Experiment_ArmOnly_P.Out1_Y0_j5;

    // End of SystemInitialize for SubSystem: '<S511>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S516>/Hold this value'
    // SystemInitialize for Outport: '<S540>/Out1'
    Combined_Experiment_ArmOnly_B.In1_dl =
      Combined_Experiment_ArmOnly_P.Out1_Y0_cu;

    // End of SystemInitialize for SubSystem: '<S516>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S517>/Hold this value'
    // SystemInitialize for Outport: '<S541>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ju =
      Combined_Experiment_ArmOnly_P.Out1_Y0_or;

    // End of SystemInitialize for SubSystem: '<S517>/Hold this value'
    // End of SystemInitialize for SubSystem: '<Root>/Simulate Plant Dynamics'

    // SystemInitialize for IfAction SubSystem: '<S2>/Change Behavior'
    // Start for MATLABSystem: '<S20>/Digital Write'
    Combined_Experiment_ArmOnly_DW.obj_iv.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_iv.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_iv.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_iv.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_iv.isInitialized = 1;
    MW_gpioInit(10U, true);
    Combined_Experiment_ArmOnly_DW.obj_iv.isSetupComplete = true;

    // Start for MATLABSystem: '<S21>/Digital Write'
    Combined_Experiment_ArmOnly_DW.obj_a.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_a.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_a.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_a.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_a.isInitialized = 1;
    MW_gpioInit(26U, true);
    Combined_Experiment_ArmOnly_DW.obj_a.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S2>/Change Behavior'

    // SystemInitialize for Atomic SubSystem: '<S3>/Send Commands to PWM Blocks' 
    // Start for MATLABSystem: '<S26>/RED T1 - BLACK T8'
    Combined_Experiment_ArmOnly_DW.obj_m.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_m.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_m.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_m.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_m.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_m.PinNumber = 27U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_m.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_m.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_m.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T1 - BLACK T8'

    // Start for MATLABSystem: '<S26>/RED T2 - BLACK T3'
    Combined_Experiment_ArmOnly_DW.obj_ma.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_ma.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_ma.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_ma.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_ma.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_ma.PinNumber = 19U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_ma.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_ma.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_ma.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T2 - BLACK T3'

    // Start for MATLABSystem: '<S26>/RED T3'
    Combined_Experiment_ArmOnly_DW.obj_h.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_h.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_h.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_h.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_h.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_h.PinNumber = 4U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_h.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_h.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_h.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T3'

    // Start for MATLABSystem: '<S26>/RED T4 - BLACK T5'
    Combined_Experiment_ArmOnly_DW.obj_k.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_k.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_k.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_k.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_k.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_k.PinNumber = 22U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_k.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_k.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_k.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T4 - BLACK T5'

    // Start for MATLABSystem: '<S26>/RED T5 - BLACK T4'
    Combined_Experiment_ArmOnly_DW.obj_hw.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_hw.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_hw.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_hw.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_hw.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_hw.PinNumber = 5U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_hw.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_hw.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_hw.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T5 - BLACK T4'

    // Start for MATLABSystem: '<S26>/RED T6 - BLACK T7'
    Combined_Experiment_ArmOnly_DW.obj_hf.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_hf.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_hf.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_hf.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_hf.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_hf.PinNumber = 6U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_hf.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_hf.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_hf.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T6 - BLACK T7'

    // Start for MATLABSystem: '<S26>/RED T7 - BLACK T6'
    Combined_Experiment_ArmOnly_DW.obj_c.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_c.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_c.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_c.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_c.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_c.PinNumber = 13U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_c.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_c.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_c.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T7 - BLACK T6'

    // Start for MATLABSystem: '<S26>/RED T8 - BLACK T1'
    Combined_Experiment_ArmOnly_DW.obj_fo.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_fo.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_fo.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_fo.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_fo.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_fo.PinNumber = 17U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_fo.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_fo.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_fo.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/RED T8 - BLACK T1'

    // Start for MATLABSystem: '<S26>/BLACK T2'
    Combined_Experiment_ArmOnly_DW.obj_ci.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_ci.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_ci.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_ci.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_ci.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_ci.PinNumber = 9U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_ci.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_ci.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_ci.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S26>/BLACK T2'
    // End of SystemInitialize for SubSystem: '<S3>/Send Commands to PWM Blocks' 

    // SystemInitialize for IfAction SubSystem: '<S3>/Change BLACK Behavior'
    // SystemInitialize for Outport: '<S22>/BLACK PWM'
    for (ret = 0; ret < 8; ret++) {
      Combined_Experiment_ArmOnly_B.sf_MATLABFunction2.ThrustPer_Final[ret] =
        Combined_Experiment_ArmOnly_P.BLACKPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S22>/BLACK PWM'
    // End of SystemInitialize for SubSystem: '<S3>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S3>/Change BLUE Behavior'
    // SystemInitialize for Outport: '<S23>/BLUE PWM'
    for (ret = 0; ret < 8; ret++) {
      Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_n.ThrustPer_Final[ret] =
        Combined_Experiment_ArmOnly_P.BLUEPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S23>/BLUE PWM'
    // End of SystemInitialize for SubSystem: '<S3>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S3>/Change RED Behavior'
    // SystemInitialize for Outport: '<S24>/RED PWM'
    for (ret = 0; ret < 8; ret++) {
      Combined_Experiment_ArmOnly_B.sf_MATLABFunction2_l.ThrustPer_Final[ret] =
        Combined_Experiment_ArmOnly_P.REDPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S24>/RED PWM'
    // End of SystemInitialize for SubSystem: '<S3>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S4>/Change RED Behavior'
    // InitializeConditions for Delay: '<S63>/Delay1'
    Combined_Experiment_ArmOnly_DW.Delay1_DSTATE_kt =
      Combined_Experiment_ArmOnly_P.Delay1_InitialCondition;

    // InitializeConditions for Delay: '<S63>/Delay2'
    Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[0] =
      Combined_Experiment_ArmOnly_P.Delay2_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay2_DSTATE[1] =
      Combined_Experiment_ArmOnly_P.Delay2_InitialCondition;

    // InitializeConditions for Delay: '<S63>/Delay5'
    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[0] =
      Combined_Experiment_ArmOnly_P.Delay5_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[1] =
      Combined_Experiment_ArmOnly_P.Delay5_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay5_DSTATE[2] =
      Combined_Experiment_ArmOnly_P.Delay5_InitialCondition;

    // InitializeConditions for Delay: '<S63>/Delay3'
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[0] =
      Combined_Experiment_ArmOnly_P.Delay3_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[1] =
      Combined_Experiment_ArmOnly_P.Delay3_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[2] =
      Combined_Experiment_ArmOnly_P.Delay3_InitialCondition;
    Combined_Experiment_ArmOnly_DW.Delay3_DSTATE[3] =
      Combined_Experiment_ArmOnly_P.Delay3_InitialCondition;

    // InitializeConditions for Delay: '<S63>/Delay4'
    for (ret = 0; ret < 5; ret++) {
      Combined_Experiment_ArmOnly_DW.Delay4_DSTATE[ret] =
        Combined_Experiment_ArmOnly_P.Delay4_InitialCondition;
    }

    // End of InitializeConditions for Delay: '<S63>/Delay4'

    // InitializeConditions for Delay: '<S58>/Delay'
    Combined_Experiment_ArmOnly_DW.Delay_DSTATE =
      Combined_Experiment_ArmOnly_P.Delay_InitialCondition;

    // SystemInitialize for Atomic SubSystem: '<S59>/Send Direction to Motor Controller' 
    // Start for MATLABSystem: '<S71>/Digital Write'
    Combined_Experiment_ArmOnly_DW.obj_b.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_b.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_b.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_b.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_b.isInitialized = 1;
    MW_gpioInit(21U, true);
    Combined_Experiment_ArmOnly_DW.obj_b.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S59>/Send Direction to Motor Controller' 

    // SystemInitialize for Atomic SubSystem: '<S59>/Send PWM to Motor Controller' 
    // Start for MATLABSystem: '<S68>/PWM1'
    Combined_Experiment_ArmOnly_DW.obj_d.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_d.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_d.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_d.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_d.isInitialized = 1;
    Combined_Experiment_ArmOnly_DW.obj_d.PinNumber = 18U;
    ret = EXT_PWMBlock_init(Combined_Experiment_ArmOnly_DW.obj_d.PinNumber, 10U,
      0.0);
    if (ret != 0) {
      for (ret = 0; ret < 45; ret++) {
        Combined_Experiment_ArmOnly_B.cv[ret] = tmp[ret];
      }

      printf(Combined_Experiment_ArmOnly_B.cv,
             Combined_Experiment_ArmOnly_DW.obj_d.PinNumber);
    }

    Combined_Experiment_ArmOnly_DW.obj_d.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S68>/PWM1'
    // End of SystemInitialize for SubSystem: '<S59>/Send PWM to Motor Controller' 

    // SystemInitialize for Atomic SubSystem: '<S59>/Turn on Motor'
    // Start for MATLABSystem: '<S72>/Digital Write'
    Combined_Experiment_ArmOnly_DW.obj_cp.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_cp.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_cp.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_cp.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_cp.isInitialized = 1;
    MW_gpioInit(25U, true);
    Combined_Experiment_ArmOnly_DW.obj_cp.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S59>/Turn on Motor'

    // SystemInitialize for Atomic SubSystem: '<S59>/Obtain RW Status'
    // Start for MATLABSystem: '<S70>/Digital Read'
    Combined_Experiment_ArmOnly_DW.obj_fq.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_fq.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_fq.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime)) &&
         (!rtIsNaN(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime))) ||
        rtIsInf(Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime)) {
      Combined_Experiment_ArmOnly_B.sampleTime =
        Combined_Experiment_ArmOnly_P.DigitalRead_SampleTime;
    }

    Combined_Experiment_ArmOnly_DW.obj_fq.SampleTime =
      Combined_Experiment_ArmOnly_B.sampleTime;
    Combined_Experiment_ArmOnly_DW.obj_fq.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_fq.isInitialized = 1;
    MW_gpioInit(24U, false);
    Combined_Experiment_ArmOnly_DW.obj_fq.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S70>/Digital Read'
    // End of SystemInitialize for SubSystem: '<S59>/Obtain RW Status'
    // End of SystemInitialize for SubSystem: '<S4>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' 
    // Start for If: '<S75>/Check whether both platforms are being used, and if so then use RED to send data to BLACK ' 
    Combined_Experiment_ArmOnly_DW.Checkwhetherbothplatformsarebei = -1;

    // SystemInitialize for IfAction SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
    // Start for If: '<S76>/This IF block determines whether or not to run the BLACK sim//exp' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhether_d1 = -1;

    // Start for If: '<S76>/This IF block determines whether or not to run the RED sim//exp ' 
    Combined_Experiment_ArmOnly_DW.ThisIFblockdetermineswhethero_o = -1;

    // SystemInitialize for IfAction SubSystem: '<S76>/Obtain BLACK States'
    // Start for S-Function (sdspFromNetwork): '<S79>/UDP Receive'
    sErr = GetErrorBuffer(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib
                          [0U]);
    CreateUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U],
                        0, "0.0.0.0",
                        Combined_Experiment_ArmOnly_P.UDPReceive_Port, "0.0.0.0",
                        -1, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib
                          [0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
        rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
      }
    }

    // End of Start for S-Function (sdspFromNetwork): '<S79>/UDP Receive'

    // InitializeConditions for Delay: '<S87>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ky1 = 1U;

    // InitializeConditions for Delay: '<S82>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_of = 1U;

    // InitializeConditions for Delay: '<S88>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_oi = 1U;

    // InitializeConditions for Delay: '<S83>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_hb = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S79>/Unwrap1'
    Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_o = true;

    // InitializeConditions for Delay: '<S89>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ap = 1U;

    // InitializeConditions for Delay: '<S84>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_hc = 1U;

    // InitializeConditions for Delay: '<S81>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_nn = 1U;

    // InitializeConditions for Delay: '<S90>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_gc = 1U;

    // InitializeConditions for Delay: '<S85>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_d0 = 1U;

    // InitializeConditions for Delay: '<S91>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a1a = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S79>/Unwrap'
    Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_p = true;

    // InitializeConditions for Delay: '<S86>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ms = 1U;

    // InitializeConditions for Delay: '<S92>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_gi = 1U;

    // SystemInitialize for IfAction SubSystem: '<S81>/Hold this value'
    // SystemInitialize for Outport: '<S93>/Out1'
    Combined_Experiment_ArmOnly_B.In1_kw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_e2;

    // End of SystemInitialize for SubSystem: '<S81>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S82>/Hold this value'
    // SystemInitialize for Outport: '<S94>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fg =
      Combined_Experiment_ArmOnly_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S82>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S83>/Hold this value'
    // SystemInitialize for Outport: '<S95>/Out1'
    Combined_Experiment_ArmOnly_B.In1_jj =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bu;

    // End of SystemInitialize for SubSystem: '<S83>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S84>/Hold this value'
    // SystemInitialize for Outport: '<S96>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_i;

    // End of SystemInitialize for SubSystem: '<S84>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S85>/Hold this value'
    // SystemInitialize for Outport: '<S97>/Out1'
    Combined_Experiment_ArmOnly_B.In1_jp3 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bw;

    // End of SystemInitialize for SubSystem: '<S85>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S86>/Hold this value'
    // SystemInitialize for Outport: '<S98>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cxn =
      Combined_Experiment_ArmOnly_P.Out1_Y0_j;

    // End of SystemInitialize for SubSystem: '<S86>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S87>/Hold this value'
    // SystemInitialize for Outport: '<S99>/Out1'
    Combined_Experiment_ArmOnly_B.In1_dn =
      Combined_Experiment_ArmOnly_P.Out1_Y0_j3;

    // End of SystemInitialize for SubSystem: '<S87>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S88>/Hold this value'
    // SystemInitialize for Outport: '<S100>/Out1'
    Combined_Experiment_ArmOnly_B.In1_jk =
      Combined_Experiment_ArmOnly_P.Out1_Y0_lj;

    // End of SystemInitialize for SubSystem: '<S88>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S89>/Hold this value'
    // SystemInitialize for Outport: '<S101>/Out1'
    Combined_Experiment_ArmOnly_B.In1_fr =
      Combined_Experiment_ArmOnly_P.Out1_Y0_g;

    // End of SystemInitialize for SubSystem: '<S89>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S90>/Hold this value'
    // SystemInitialize for Outport: '<S102>/Out1'
    Combined_Experiment_ArmOnly_B.In1_df =
      Combined_Experiment_ArmOnly_P.Out1_Y0_id;

    // End of SystemInitialize for SubSystem: '<S90>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S91>/Hold this value'
    // SystemInitialize for Outport: '<S103>/Out1'
    Combined_Experiment_ArmOnly_B.In1_dr =
      Combined_Experiment_ArmOnly_P.Out1_Y0_j1;

    // End of SystemInitialize for SubSystem: '<S91>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S92>/Hold this value'
    // SystemInitialize for Outport: '<S104>/Out1'
    Combined_Experiment_ArmOnly_B.In1_oi =
      Combined_Experiment_ArmOnly_P.Out1_Y0_fb;

    // End of SystemInitialize for SubSystem: '<S92>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S76>/Obtain BLACK States'

    // SystemInitialize for IfAction SubSystem: '<S76>/Obtain RED States'
    // Start for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 
    sErr = GetErrorBuffer
      (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);
    CreateUDPInterface
      (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);
    if (*sErr == 0) {
      LibCreate_Network
        (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U], 1,
         "255.255.255.255", -1, "192.168.0.101",
         Combined_Experiment_ArmOnly_P.SendBLACKStatestoBLACKPlatform_, 8192, 8,
         0);
    }

    if (*sErr == 0) {
      LibStart(&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_
               [0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface
        (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
        rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 

    // InitializeConditions for Delay: '<S121>/Delay'
    Combined_Experiment_ArmOnly_DW.Delay_DSTATE_p =
      Combined_Experiment_ArmOnly_P.Delay_InitialCondition_e;

    // InitializeConditions for Delay: '<S115>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_kc = 1U;

    // InitializeConditions for Delay: '<S110>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_p1 = 1U;

    // InitializeConditions for Delay: '<S116>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_kv = 1U;

    // InitializeConditions for Delay: '<S111>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_cg = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S80>/Unwrap1'
    Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep = true;

    // InitializeConditions for Delay: '<S117>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_f0 = 1U;

    // InitializeConditions for Delay: '<S112>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a5 = 1U;

    // InitializeConditions for Delay: '<S109>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_l4 = 1U;

    // InitializeConditions for Delay: '<S118>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_go = 1U;

    // InitializeConditions for Delay: '<S113>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ay = 1U;

    // InitializeConditions for Delay: '<S119>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_aqe = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S80>/Unwrap'
    Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep = true;

    // InitializeConditions for Delay: '<S114>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_ma = 1U;

    // InitializeConditions for Delay: '<S120>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_hj = 1U;

    // SystemInitialize for Enabled SubSystem: '<S106>/Enabled Subsystem'
    // SystemInitialize for Outport: '<S122>/Out1'
    Combined_Experiment_ArmOnly_B.In1_it =
      Combined_Experiment_ArmOnly_P.Out1_Y0_os;

    // End of SystemInitialize for SubSystem: '<S106>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S80>/Enabled Subsystem1'
    // SystemInitialize for Outport: '<S108>/Actual Time'
    Combined_Experiment_ArmOnly_B.In1_nh =
      Combined_Experiment_ArmOnly_P.ActualTime_Y0_l;

    // End of SystemInitialize for SubSystem: '<S80>/Enabled Subsystem1'

    // SystemInitialize for IfAction SubSystem: '<S109>/Hold this value'
    // SystemInitialize for Outport: '<S125>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gd =
      Combined_Experiment_ArmOnly_P.Out1_Y0_lf;

    // End of SystemInitialize for SubSystem: '<S109>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S110>/Hold this value'
    // SystemInitialize for Outport: '<S126>/Out1'
    Combined_Experiment_ArmOnly_B.In1_av =
      Combined_Experiment_ArmOnly_P.Out1_Y0_gx;

    // End of SystemInitialize for SubSystem: '<S110>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S111>/Hold this value'
    // SystemInitialize for Outport: '<S127>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ox =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ih;

    // End of SystemInitialize for SubSystem: '<S111>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S112>/Hold this value'
    // SystemInitialize for Outport: '<S128>/Out1'
    Combined_Experiment_ArmOnly_B.In1_kg =
      Combined_Experiment_ArmOnly_P.Out1_Y0_o0;

    // End of SystemInitialize for SubSystem: '<S112>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S113>/Hold this value'
    // SystemInitialize for Outport: '<S129>/Out1'
    Combined_Experiment_ArmOnly_B.In1_i2 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ac;

    // End of SystemInitialize for SubSystem: '<S113>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S114>/Hold this value'
    // SystemInitialize for Outport: '<S130>/Out1'
    Combined_Experiment_ArmOnly_B.In1_mc =
      Combined_Experiment_ArmOnly_P.Out1_Y0_bk;

    // End of SystemInitialize for SubSystem: '<S114>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S115>/Hold this value'
    // SystemInitialize for Outport: '<S131>/Out1'
    Combined_Experiment_ArmOnly_B.In1_n5 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_l0;

    // End of SystemInitialize for SubSystem: '<S115>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S116>/Hold this value'
    // SystemInitialize for Outport: '<S132>/Out1'
    Combined_Experiment_ArmOnly_B.In1_bi =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ok;

    // End of SystemInitialize for SubSystem: '<S116>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S117>/Hold this value'
    // SystemInitialize for Outport: '<S133>/Out1'
    Combined_Experiment_ArmOnly_B.In1_hs =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ou;

    // End of SystemInitialize for SubSystem: '<S117>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S118>/Hold this value'
    // SystemInitialize for Outport: '<S134>/Out1'
    Combined_Experiment_ArmOnly_B.In1_lj =
      Combined_Experiment_ArmOnly_P.Out1_Y0_k0;

    // End of SystemInitialize for SubSystem: '<S118>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S119>/Hold this value'
    // SystemInitialize for Outport: '<S135>/Out1'
    Combined_Experiment_ArmOnly_B.In1_mj =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ks;

    // End of SystemInitialize for SubSystem: '<S119>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S120>/Hold this value'
    // SystemInitialize for Outport: '<S136>/Out1'
    Combined_Experiment_ArmOnly_B.In1_pb =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ju;

    // End of SystemInitialize for SubSystem: '<S120>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S76>/Obtain RED States'
    // End of SystemInitialize for SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 

    // SystemInitialize for IfAction SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 
    // InitializeConditions for Delay: '<S153>/Delay'
    Combined_Experiment_ArmOnly_DW.Delay_DSTATE_o =
      Combined_Experiment_ArmOnly_P.Delay_InitialCondition_k;

    // InitializeConditions for Delay: '<S147>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_dr = 1U;

    // InitializeConditions for Delay: '<S142>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_mq = 1U;

    // InitializeConditions for Delay: '<S148>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_lr = 1U;

    // InitializeConditions for Delay: '<S143>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_b = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S77>/Unwrap1'
    Combined_Experiment_ArmOnly_DW.Unwrap1_FirstStep_d = true;

    // InitializeConditions for Delay: '<S149>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_aa = 1U;

    // InitializeConditions for Delay: '<S144>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_cw = 1U;

    // InitializeConditions for Delay: '<S141>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_nsj = 1U;

    // InitializeConditions for Delay: '<S150>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_lrw = 1U;

    // InitializeConditions for Delay: '<S145>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_a2d = 1U;

    // InitializeConditions for Delay: '<S151>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_mp = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S77>/Unwrap'
    Combined_Experiment_ArmOnly_DW.Unwrap_FirstStep_d = true;

    // InitializeConditions for Delay: '<S146>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_gb = 1U;

    // InitializeConditions for Delay: '<S152>/Delay1'
    Combined_Experiment_ArmOnly_DW.icLoad_i = 1U;

    // SystemInitialize for Enabled SubSystem: '<S138>/Enabled Subsystem'
    // SystemInitialize for Outport: '<S154>/Out1'
    Combined_Experiment_ArmOnly_B.In1_j1 = Combined_Experiment_ArmOnly_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S138>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S77>/Enabled Subsystem'
    // SystemInitialize for IfAction SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
    // SystemInitialize for IfAction SubSystem: '<S76>/Obtain RED States'
    // SystemInitialize for Enabled SubSystem: '<S80>/Enabled Subsystem'
    for (ret = 0; ret < 13; ret++) {
      // SystemInitialize for Outport: '<S107>/Out1'
      Combined_Experiment_ArmOnly_B.In1_n1[ret] =
        Combined_Experiment_ArmOnly_P.Out1_Y0_nx;

      // SystemInitialize for Outport: '<S139>/Out1'
      Combined_Experiment_ArmOnly_B.In1_b4[ret] =
        Combined_Experiment_ArmOnly_P.Out1_Y0_m;
    }

    // End of SystemInitialize for SubSystem: '<S80>/Enabled Subsystem'
    // End of SystemInitialize for SubSystem: '<S76>/Obtain RED States'
    // End of SystemInitialize for SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
    // End of SystemInitialize for SubSystem: '<S77>/Enabled Subsystem'

    // SystemInitialize for Enabled SubSystem: '<S77>/Enabled Subsystem1'
    // SystemInitialize for Outport: '<S140>/Actual Time'
    Combined_Experiment_ArmOnly_B.In1_pl =
      Combined_Experiment_ArmOnly_P.ActualTime_Y0;

    // End of SystemInitialize for SubSystem: '<S77>/Enabled Subsystem1'

    // SystemInitialize for IfAction SubSystem: '<S141>/Hold this value'
    // SystemInitialize for Outport: '<S157>/Out1'
    Combined_Experiment_ArmOnly_B.In1_kd2 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_l;

    // End of SystemInitialize for SubSystem: '<S141>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S142>/Hold this value'
    // SystemInitialize for Outport: '<S158>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ds =
      Combined_Experiment_ArmOnly_P.Out1_Y0_f;

    // End of SystemInitialize for SubSystem: '<S142>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S143>/Hold this value'
    // SystemInitialize for Outport: '<S159>/Out1'
    Combined_Experiment_ArmOnly_B.In1_dy =
      Combined_Experiment_ArmOnly_P.Out1_Y0_e;

    // End of SystemInitialize for SubSystem: '<S143>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S144>/Hold this value'
    // SystemInitialize for Outport: '<S160>/Out1'
    Combined_Experiment_ArmOnly_B.In1_lyk =
      Combined_Experiment_ArmOnly_P.Out1_Y0_b;

    // End of SystemInitialize for SubSystem: '<S144>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S145>/Hold this value'
    // SystemInitialize for Outport: '<S161>/Out1'
    Combined_Experiment_ArmOnly_B.In1_mm =
      Combined_Experiment_ArmOnly_P.Out1_Y0_k;

    // End of SystemInitialize for SubSystem: '<S145>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S146>/Hold this value'
    // SystemInitialize for Outport: '<S162>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cp =
      Combined_Experiment_ArmOnly_P.Out1_Y0_h;

    // End of SystemInitialize for SubSystem: '<S146>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S147>/Hold this value'
    // SystemInitialize for Outport: '<S163>/Out1'
    Combined_Experiment_ArmOnly_B.In1_d4 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_d;

    // End of SystemInitialize for SubSystem: '<S147>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S148>/Hold this value'
    // SystemInitialize for Outport: '<S164>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ljk =
      Combined_Experiment_ArmOnly_P.Out1_Y0_o;

    // End of SystemInitialize for SubSystem: '<S148>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S149>/Hold this value'
    // SystemInitialize for Outport: '<S165>/Out1'
    Combined_Experiment_ArmOnly_B.In1_awb =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ec;

    // End of SystemInitialize for SubSystem: '<S149>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S150>/Hold this value'
    // SystemInitialize for Outport: '<S166>/Out1'
    Combined_Experiment_ArmOnly_B.In1_aw =
      Combined_Experiment_ArmOnly_P.Out1_Y0_n;

    // End of SystemInitialize for SubSystem: '<S150>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S151>/Hold this value'
    // SystemInitialize for Outport: '<S167>/Out1'
    Combined_Experiment_ArmOnly_B.In1_on =
      Combined_Experiment_ArmOnly_P.Out1_Y0_oi;

    // End of SystemInitialize for SubSystem: '<S151>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S152>/Hold this value'
    // SystemInitialize for Outport: '<S168>/Out1'
    Combined_Experiment_ArmOnly_B.In1_kt =
      Combined_Experiment_ArmOnly_P.Out1_Y0_fr;

    // End of SystemInitialize for SubSystem: '<S152>/Hold this value'

    // Start for MATLABSystem: '<S77>/Stream PhaseSpace to Platform'
    Combined_Experiment_ArmOnly_DW.obj_f.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_f.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_f.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_f.platformSelection =
      Combined_Experiment_ArmOnly_P.platformSelection;
    Combined_Experiment_ArmOnly_DW.obj_f.PS_SampleRate = 1.0 /
      Combined_Experiment_ArmOnly_P.serverRate;
    Combined_Experiment_ArmOnly_DW.obj_f.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_f.isInitialized = 1;
    initialize_phasespace(Combined_Experiment_ArmOnly_DW.obj_f.platformSelection,
                          Combined_Experiment_ArmOnly_DW.obj_f.PS_SampleRate);
    Combined_Experiment_ArmOnly_DW.obj_f.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 
    // End of SystemInitialize for SubSystem: '<S5>/Use Hardware to Obtain States' 

    // SystemInitialize for IfAction SubSystem: '<S6>/Change BLACK Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S181>/Discrete-Time Integrator1' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_o = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_i = 1U;

    // SystemInitialize for IfAction SubSystem: '<S181>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_h,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_h,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_h);

    // End of SystemInitialize for SubSystem: '<S181>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S181>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S190>/Out1'
    Combined_Experiment_ArmOnly_B.In1_jp =
      Combined_Experiment_ArmOnly_P.Out1_Y0_nxn;

    // End of SystemInitialize for SubSystem: '<S181>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S179>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean);

    // End of SystemInitialize for SubSystem: '<S179>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S179>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S183>/Out1'
    Combined_Experiment_ArmOnly_B.In1_hd =
      Combined_Experiment_ArmOnly_P.Out1_Y0_l0t;

    // End of SystemInitialize for SubSystem: '<S179>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S180>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_e,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_e,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_e);

    // End of SystemInitialize for SubSystem: '<S180>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S180>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S186>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ny =
      Combined_Experiment_ArmOnly_P.Out1_Y0_et;

    // End of SystemInitialize for SubSystem: '<S180>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S192>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_d,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_d,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_d);

    // End of SystemInitialize for SubSystem: '<S192>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S192>/Pass Current Accel'
    // SystemInitialize for Outport: '<S196>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ic =
      Combined_Experiment_ArmOnly_P.Out1_Y0_p;

    // End of SystemInitialize for SubSystem: '<S192>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S193>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_a,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_a,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_a);

    // End of SystemInitialize for SubSystem: '<S193>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S193>/Pass Current Accel'
    // SystemInitialize for Outport: '<S199>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gs =
      Combined_Experiment_ArmOnly_P.Out1_Y0_es;

    // End of SystemInitialize for SubSystem: '<S193>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S194>/Calculate Running Mean'
    CalculateRunningMean_b_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_ab,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_ab,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_ab);

    // End of SystemInitialize for SubSystem: '<S194>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S194>/Pass Current Accel'
    // SystemInitialize for Outport: '<S202>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cd =
      Combined_Experiment_ArmOnly_P.Out1_Y0_c;

    // End of SystemInitialize for SubSystem: '<S194>/Pass Current Accel'
    Combined_Experim_AHRS2_Init(Combined_Experiment_ArmOnly_B.DigitalFilter_k,
      &Combined_Experiment_ArmOnly_DW.AHRS2,
      &Combined_Experiment_ArmOnly_P.AHRS2);

    // End of SystemInitialize for SubSystem: '<S6>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S6>/Change BLUE Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S212>/Discrete-Time Integrator1' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_h = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_f = 1U;

    // SystemInitialize for IfAction SubSystem: '<S212>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_l,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_l,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_l);

    // End of SystemInitialize for SubSystem: '<S212>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S212>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S221>/Out1'
    Combined_Experiment_ArmOnly_B.In1_hy =
      Combined_Experiment_ArmOnly_P.Out1_Y0_nl;

    // End of SystemInitialize for SubSystem: '<S212>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S210>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_c,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_c,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_c);

    // End of SystemInitialize for SubSystem: '<S210>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S211>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_f,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_f,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_f);

    // End of SystemInitialize for SubSystem: '<S211>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S223>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_j,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_j,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_j);

    // End of SystemInitialize for SubSystem: '<S223>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S223>/Pass Current Accel'
    // SystemInitialize for Outport: '<S227>/Out1'
    Combined_Experiment_ArmOnly_B.In1_le =
      Combined_Experiment_ArmOnly_P.Out1_Y0_pj;

    // End of SystemInitialize for SubSystem: '<S223>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S224>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_b,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_b,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_b);

    // End of SystemInitialize for SubSystem: '<S224>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S224>/Pass Current Accel'
    // SystemInitialize for Outport: '<S230>/Out1'
    Combined_Experiment_ArmOnly_B.In1_b1 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_aci;

    // End of SystemInitialize for SubSystem: '<S224>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S225>/Calculate Running Mean'
    CalculateRunningMean_b_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_o,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_o,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_o);

    // End of SystemInitialize for SubSystem: '<S225>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S225>/Pass Current Accel'
    // SystemInitialize for Outport: '<S233>/Out1'
    Combined_Experiment_ArmOnly_B.In1_ai =
      Combined_Experiment_ArmOnly_P.Out1_Y0_c1;

    // End of SystemInitialize for SubSystem: '<S225>/Pass Current Accel'
    Combined_Experim_AHRS2_Init(Combined_Experiment_ArmOnly_B.DigitalFilter_n,
      &Combined_Experiment_ArmOnly_DW.AHRS2_p,
      &Combined_Experiment_ArmOnly_P.AHRS2_p);

    // End of SystemInitialize for SubSystem: '<S6>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S6>/Change RED Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S243>/Discrete-Time Integrator1' 
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_PrevR_f = 2;
    Combined_Experiment_ArmOnly_DW.DiscreteTimeIntegrator1_IC_LO_g = 1U;

    // SystemInitialize for IfAction SubSystem: '<S243>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_et,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_et,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_et);

    // End of SystemInitialize for SubSystem: '<S243>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S243>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S252>/Out1'
    Combined_Experiment_ArmOnly_B.In1_f1 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_be;

    // End of SystemInitialize for SubSystem: '<S243>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S241>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_oo,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_oo,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_oo);

    // End of SystemInitialize for SubSystem: '<S241>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S241>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S245>/Out1'
    Combined_Experiment_ArmOnly_B.In1_lm =
      Combined_Experiment_ArmOnly_P.Out1_Y0_e5;

    // End of SystemInitialize for SubSystem: '<S241>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S242>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_om,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_om,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_om);

    // End of SystemInitialize for SubSystem: '<S242>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S242>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S248>/Out1'
    Combined_Experiment_ArmOnly_B.In1_cx =
      Combined_Experiment_ArmOnly_P.Out1_Y0_ov;

    // End of SystemInitialize for SubSystem: '<S242>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S254>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_j1,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_j1,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_j1);

    // End of SystemInitialize for SubSystem: '<S254>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S254>/Pass Current Accel'
    // SystemInitialize for Outport: '<S258>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gy =
      Combined_Experiment_ArmOnly_P.Out1_Y0_is;

    // End of SystemInitialize for SubSystem: '<S254>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S255>/Calculate Running Mean'
    C_CalculateRunningMean_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_jr,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_jr,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_jr);

    // End of SystemInitialize for SubSystem: '<S255>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S255>/Pass Current Accel'
    // SystemInitialize for Outport: '<S261>/Out1'
    Combined_Experiment_ArmOnly_B.In1_e5 =
      Combined_Experiment_ArmOnly_P.Out1_Y0_c4;

    // End of SystemInitialize for SubSystem: '<S255>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S256>/Calculate Running Mean'
    CalculateRunningMean_b_Init
      (&Combined_Experiment_ArmOnly_B.CalculateRunningMean_cf,
       &Combined_Experiment_ArmOnly_DW.CalculateRunningMean_cf,
       &Combined_Experiment_ArmOnly_P.CalculateRunningMean_cf);

    // End of SystemInitialize for SubSystem: '<S256>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S256>/Pass Current Accel'
    // SystemInitialize for Outport: '<S264>/Out1'
    Combined_Experiment_ArmOnly_B.In1_gv =
      Combined_Experiment_ArmOnly_P.Out1_Y0_kq;

    // End of SystemInitialize for SubSystem: '<S256>/Pass Current Accel'
    Combined_Experim_AHRS2_Init(Combined_Experiment_ArmOnly_B.DigitalFilter,
      &Combined_Experiment_ArmOnly_DW.AHRS2_pn,
      &Combined_Experiment_ArmOnly_P.AHRS2_pn);

    // End of SystemInitialize for SubSystem: '<S6>/Change RED Behavior'

    // Start for MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
    Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj.i2cobj_MAG.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj.matlabCodegenIsDeleted = true;
    Combi_lsm9ds1Block_lsm9ds1Block(&Combined_Experiment_ArmOnly_DW.obj);
    Combined_Exper_SystemCore_setup(&Combined_Experiment_ArmOnly_DW.obj);

    // Start for MATLABSystem: '<Root>/Initialization Block for Position Control' 
    Combined_Experiment_ArmOnly_DW.obj_i.matlabCodegenIsDeleted = true;
    Combined_Experiment_ArmOnly_DW.obj_i.isInitialized = 0;
    Combined_Experiment_ArmOnly_DW.obj_i.matlabCodegenIsDeleted = false;
    Combined_Experiment_ArmOnly_DW.obj_i.P_GAIN =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositionC;
    Combined_Experiment_ArmOnly_DW.obj_i.I_GAIN =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_b;
    Combined_Experiment_ArmOnly_DW.obj_i.D_GAIN =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_g;
    Combined_Experiment_ArmOnly_DW.obj_i.MAX_POSITION =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_i;
    Combined_Experiment_ArmOnly_DW.obj_i.MIN_POSITION =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositio_k;
    Combined_Experiment_ArmOnly_DW.obj_i.MOVE_TIME =
      Combined_Experiment_ArmOnly_P.InitializationBlockforPositi_ba;
    Combined_Experiment_ArmOnly_DW.obj_i.isSetupComplete = false;
    Combined_Experiment_ArmOnly_DW.obj_i.isInitialized = 1;
    initialize_dynamixel_position_control
      (Combined_Experiment_ArmOnly_DW.obj_i.P_GAIN,
       Combined_Experiment_ArmOnly_DW.obj_i.I_GAIN,
       Combined_Experiment_ArmOnly_DW.obj_i.D_GAIN,
       Combined_Experiment_ArmOnly_DW.obj_i.MAX_POSITION,
       Combined_Experiment_ArmOnly_DW.obj_i.MIN_POSITION,
       Combined_Experiment_ArmOnly_DW.obj_i.MOVE_TIME);
    Combined_Experiment_ArmOnly_DW.obj_i.isSetupComplete = true;
  }
}

// Model terminate function
void Combined_Experiment_ArmOnly_terminate(void)
{
  char_T *sErr;

  // Terminate for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
  // Terminate for IfAction SubSystem: '<S10>/Change RED Behavior'
  SendPositionCommandsto_Term
    (&Combined_Experiment_ArmOnly_DW.SendPositionCommandstoManipulat);

  // End of Terminate for SubSystem: '<S10>/Change RED Behavior'
  // End of Terminate for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
  SendPositionCommandsto_Term
    (&Combined_Experiment_ArmOnly_DW.SendPositionCommandstoManipul_p);

  // End of Terminate for SubSystem: '<S319>/Sub-Phase #3 '
  // End of Terminate for SubSystem: '<S11>/Change RED Behavior'
  // End of Terminate for SubSystem: '<Root>/Phase #3: Experiment'

  // Terminate for IfAction SubSystem: '<S2>/Change Behavior'
  // Terminate for MATLABSystem: '<S20>/Digital Write'
  matlabCodegenHandle_matlabCo_ba(&Combined_Experiment_ArmOnly_DW.obj_iv);

  // Terminate for MATLABSystem: '<S21>/Digital Write'
  matlabCodegenHandle_matlabC_ba5(&Combined_Experiment_ArmOnly_DW.obj_a);

  // End of Terminate for SubSystem: '<S2>/Change Behavior'

  // Terminate for IfAction SubSystem: '<S5>/Use Hardware to Obtain States'
  // Terminate for IfAction SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 
  // Terminate for MATLABSystem: '<S77>/Stream PhaseSpace to Platform'
  Combined_Expe_matlabCodegenHa_p(&Combined_Experiment_ArmOnly_DW.obj_f);

  // End of Terminate for SubSystem: '<S75>/Using RED, BLACK, BLUE, or RED + ARM' 

  // Terminate for IfAction SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM' 
  // Terminate for IfAction SubSystem: '<S76>/Obtain BLACK States'
  // Terminate for S-Function (sdspFromNetwork): '<S79>/UDP Receive'
  sErr = GetErrorBuffer(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);
  LibTerminate(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
    rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
  }

  LibDestroy(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U], 0);
  DestroyUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPReceive_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspFromNetwork): '<S79>/UDP Receive'
  // End of Terminate for SubSystem: '<S76>/Obtain BLACK States'

  // Terminate for IfAction SubSystem: '<S76>/Obtain RED States'
  // Terminate for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 
  sErr = GetErrorBuffer
    (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);
  LibTerminate(&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_
               [0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
    rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
  }

  LibDestroy(&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U],
             1);
  DestroyUDPInterface
    (&Combined_Experiment_ArmOnly_DW.SendBLACKStatestoBLACKPlatform_[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<S80>/Send BLACK States to  BLACK Platform' 
  // End of Terminate for SubSystem: '<S76>/Obtain RED States'
  // End of Terminate for SubSystem: '<S75>/Using RED+BLACK, or RED+BLACK+ARM'
  // End of Terminate for SubSystem: '<S5>/Use Hardware to Obtain States'

  // Terminate for MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
  Combined_Expe_matlabCodegenHa_l(&Combined_Experiment_ArmOnly_DW.obj);
  Combined_Expe_matlabCodegenHa_e(&Combined_Experiment_ArmOnly_DW.obj.i2cobj_MAG);
  Combined_Expe_matlabCodegenHa_k(&Combined_Experiment_ArmOnly_DW.obj.i2cobj_A_G);

  // Terminate for S-Function (sdspToNetwork): '<S15>/UDP Send'
  sErr = GetErrorBuffer(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
  LibTerminate(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Combined_Experiment_ArmOnly_M, sErr);
    rtmSetStopRequested(Combined_Experiment_ArmOnly_M, 1);
  }

  LibDestroy(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U], 1);
  DestroyUDPInterface(&Combined_Experiment_ArmOnly_DW.UDPSend_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<S15>/UDP Send'

  // Terminate for MATLABSystem: '<Root>/Initialization Block for Position Control' 
  matlabCodegenHandle_matlabCod_b(&Combined_Experiment_ArmOnly_DW.obj_i);

  // Terminate for Atomic SubSystem: '<S3>/Send Commands to PWM Blocks'
  // Terminate for MATLABSystem: '<S26>/RED T1 - BLACK T8'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_m);

  // Terminate for MATLABSystem: '<S26>/RED T2 - BLACK T3'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_ma);

  // Terminate for MATLABSystem: '<S26>/RED T3'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_h);

  // Terminate for MATLABSystem: '<S26>/RED T4 - BLACK T5'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_k);

  // Terminate for MATLABSystem: '<S26>/RED T5 - BLACK T4'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_hw);

  // Terminate for MATLABSystem: '<S26>/RED T6 - BLACK T7'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_hf);

  // Terminate for MATLABSystem: '<S26>/RED T7 - BLACK T6'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_c);

  // Terminate for MATLABSystem: '<S26>/RED T8 - BLACK T1'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_fo);

  // Terminate for MATLABSystem: '<S26>/BLACK T2'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_ci);

  // End of Terminate for SubSystem: '<S3>/Send Commands to PWM Blocks'

  // Terminate for IfAction SubSystem: '<S4>/Change RED Behavior'
  // Terminate for Atomic SubSystem: '<S59>/Obtain RW Status'
  // Terminate for MATLABSystem: '<S70>/Digital Read'
  matlabCodegenHand_ba5m25ew4zuqz(&Combined_Experiment_ArmOnly_DW.obj_fq);

  // End of Terminate for SubSystem: '<S59>/Obtain RW Status'

  // Terminate for Atomic SubSystem: '<S59>/Send Direction to Motor Controller'
  // Terminate for MATLABSystem: '<S71>/Digital Write'
  matlabCodegenHan_ba5m25ew4zuqzr(&Combined_Experiment_ArmOnly_DW.obj_b);

  // End of Terminate for SubSystem: '<S59>/Send Direction to Motor Controller'

  // Terminate for Atomic SubSystem: '<S59>/Send PWM to Motor Controller'
  // Terminate for MATLABSystem: '<S68>/PWM1'
  matlabCodegenHandle_matlab_ba5m(&Combined_Experiment_ArmOnly_DW.obj_d);

  // End of Terminate for SubSystem: '<S59>/Send PWM to Motor Controller'

  // Terminate for Atomic SubSystem: '<S59>/Turn on Motor'
  // Terminate for MATLABSystem: '<S72>/Digital Write'
  Combined_Expe_matlabCodegenHa_a(&Combined_Experiment_ArmOnly_DW.obj_cp);

  // End of Terminate for SubSystem: '<S59>/Turn on Motor'
  // End of Terminate for SubSystem: '<S4>/Change RED Behavior'
}
