//
//  Template_v3_08_2020a.cpp
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


#include "Template_v3_08_2020a.h"
#include "Template_v3_08_2020a_private.h"

// Block signals (default storage)
B_Template_v3_08_2020a_T Template_v3_08_2020a_B;

// Block states (default storage)
DW_Template_v3_08_2020a_T Template_v3_08_2020a_DW;

// Real-time model
RT_MODEL_Template_v3_08_2020a_T Template_v3_08_2020a_M_ =
  RT_MODEL_Template_v3_08_2020a_T();
RT_MODEL_Template_v3_08_2020a_T *const Template_v3_08_2020a_M =
  &Template_v3_08_2020a_M_;

// Forward declaration for local functions
static void Templa_AHRSFilterBase_resetImpl(fusion_simulink_ahrsfilter_Te_T *obj);
static void IMUFusionCommon_computeAngularV(const real_T gfast[3], const real_T
  offset[3], real_T av[3]);
static void Template_v3_08_202_NED_ecompass(const real_T a[3], const real_T m[3],
  real_T R[9], B_AHRS2_Template_v3_08_2020a_T *localB);
static void Templ_quaternioncg_quaternioncg(const real_T varargin_1[9], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Template_v3_08_2020a_T *localB);
static void Tem_quaternioncg_quaternioncg_f(const real_T varargin_1[3], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Template_v3_08_2020a_T *localB);
static void Template__quaternionBase_mtimes(real_T x_a, real_T x_b, real_T x_c,
  real_T x_d, real_T y_a, real_T y_b, real_T y_c, real_T y_d, real_T *o_a,
  real_T *o_b, real_T *o_c, real_T *o_d);
static void IMUFusionCommon_predictOrientat(const
  fusion_simulink_ahrsfilter_Te_T *obj, const real_T gfast[3], const real_T
  offset[3], real_T qorient_a, real_T qorient_b, real_T qorient_c, real_T
  qorient_d, real_T *b_qorient_a, real_T *b_qorient_b, real_T *b_qorient_c,
  real_T *b_qorient_d, B_AHRS2_Template_v3_08_2020a_T *localB);
static void Template__quaternionBase_rotmat(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T r[9], B_AHRS2_Template_v3_08_2020a_T *localB);
static void Temp_IMUFusionCommon_buildHPart(const real_T v[3], real_T h[9],
  B_AHRS2_Template_v3_08_2020a_T *localB);
static real_T Template_v3_08_2020a_norm(const real_T x[3],
  B_AHRS2_Template_v3_08_2020a_T *localB);
static void Template_v3_quaternionBase_conj(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d);
static void Template__quaternionBase_uminus(real_T obj_a, real_T obj_b, real_T
  obj_c, real_T obj_d, real_T *b_obj_a, real_T *b_obj_b, real_T *b_obj_c, real_T
  *b_obj_d);
static void Templa_quaternionBase_normalize(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d);
static void Templa_quaternioncg_parenAssign(real_T rhs_a, real_T rhs_b, real_T
  rhs_c, real_T rhs_d, real_T *o_a, real_T *o_b, real_T *o_c, real_T *o_d);
static void Template_v3_ahrsfilter_stepImpl(fusion_simulink_ahrsfilter_Te_T *obj,
  const real_T accelIn[3], const real_T gyroIn[3], const real_T magIn[3], real_T
  orientOut[4], real_T av[3], B_AHRS2_Template_v3_08_2020a_T *localB);

// Forward declaration for local functions
static real_T Template_v3_08_20_rt_atan2d_snf(real_T u0, real_T u1);
static real_T Temp_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u);
static real_T Temp_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u);
static real_T Template_v3_08_2020a_xnrm2(int32_T n, const real_T x[36], int32_T
  ix0);
static real_T Template_v3_08_2020a_xnrm2_l(int32_T n, const real_T x[6], int32_T
  ix0);
static void Template_v3_08_2020a_xaxpy_ot(int32_T n, real_T a, const real_T x[6],
  int32_T ix0, real_T y[36], int32_T iy0);
static void Template_v3_08_2020a_xaxpy_o(int32_T n, real_T a, const real_T x[36],
  int32_T ix0, real_T y[6], int32_T iy0);
static real_T Template_v3_08_2020a_xdotc(int32_T n, const real_T x[36], int32_T
  ix0, const real_T y[36], int32_T iy0);
static void Template_v3_08_2020a_xaxpy(int32_T n, real_T a, int32_T ix0, real_T
  y[36], int32_T iy0);
static void Template_v3_08_2020a_xscal(real_T a, real_T x[36], int32_T ix0);
static void Template_v3_08_2020a_xswap(real_T x[36], int32_T ix0, int32_T iy0);
static void Template_v3_08_2020a_xrotg(real_T *a, real_T *b, real_T *c, real_T
  *s);
static void Template_v3_08_2020a_xrot(real_T x[36], int32_T ix0, int32_T iy0,
  real_T c, real_T s);
static void Template_v3_08_2020a_svd(const real_T A[36], real_T U[36], real_T s
  [6], real_T V[36]);
static void Template_v3_08_2_rt_invd3x3_snf(const real_T u[9], real_T y[9]);
static void Template_v3__SystemCore_release(const
  codertarget_linux_blocks_Digi_T *obj);
static void Template_v3_0_SystemCore_delete(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCodeg(codertarget_linux_blocks_Digi_T *obj);
static void Template_v_SystemCore_release_g(const
  codertarget_linux_blocks_Digi_T *obj);
static void Template_v3_SystemCore_delete_g(const
  codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCod_g(codertarget_linux_blocks_Digi_T *obj);
static void matlabCodegenHandle_matlabCo_g2(MoveArm_Torque_Template_v3_08_T *obj);
static void SystemCore_release_g2ofinwajqo(const raspi_internal_lsm9ds1Block_T_T
  *obj);
static void SystemCore_delete_g2ofinwajqob(const raspi_internal_lsm9ds1Block_T_T
  *obj);
static void matlabCodegenHandl_g2ofinwajqob(raspi_internal_lsm9ds1Block_T_T *obj);
static void SystemCore_releas_g2ofinwajqobn(j_codertarget_raspi_internal__T *obj);
static void SystemCore_delet_g2ofinwajqobno(j_codertarget_raspi_internal__T *obj);
static void matlabCodegenHan_g2ofinwajqobno(j_codertarget_raspi_internal__T *obj);
static void SystemCore_release_g2ofinwajqob(i_codertarget_raspi_internal__T *obj);
static void SystemCore_delete_g2ofinwajqobn(i_codertarget_raspi_internal__T *obj);
static void matlabCodegenHand_g2ofinwajqobn(i_codertarget_raspi_internal__T *obj);
static void matlabCodegenHa_g2ofinwajqobno3(ReadArm_Position_Rates_Templa_T *obj);
static void SystemCore_relea_g2ofinwajqobno(const
  InitializeForTorque_Template__T *obj);
static void T_SystemCore_dele_h(const InitializeForTorque_Template__T *obj);
static void Template_v3_0_matlabCodegenHa_h(InitializeForTorque_Template__T *obj);
static void Template_v3_0_matlabCodegenHa_d(IdentifyPlatform_Template_v3__T *obj);
static void Template__SystemCore_release_g2(const
  raspi_internal_PWMBlock_Templ_T *obj);
static void Template__SystemCore_delete_g2o(const
  raspi_internal_PWMBlock_Templ_T *obj);
static void matlabCodegenHandle_matlabC_g2o(raspi_internal_PWMBlock_Templ_T *obj);
static raspi_internal_lsm9ds1Block_T_T *Templ_lsm9ds1Block_lsm9ds1Block
  (raspi_internal_lsm9ds1Block_T_T *obj);
static void Template_v3_08_SystemCore_setup(raspi_internal_lsm9ds1Block_T_T *obj);
static void rate_monotonic_scheduler(void);
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  // Column-major Lookup 1-D
  // Search method: 'binary'
  // Use previous index: 'off'
  // Interpolation method: 'Linear point-slope'
  // Extrapolation method: 'Linear'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Extrapolation method: 'Linear'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    // Binary Search
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  // Column-major Interpolation 1-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Overflow mode: 'portable wrapping'

  return (table[iLeft + 1U] - table[iLeft]) * frac + table[iLeft];
}

//
// Set which subrates need to run this base step (base rate always runs).
// This function must be called prior to calling the model step function
// in order to "remember" which rates need to run this base step.  The
// buffering of events allows for overlapping preemption.
//
void Template_v3_08_2020a_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  // Task runs when its counter is zero, computed via rtmStepTask macro
  eventFlags[2] = ((boolean_T)rtmStepTask(Template_v3_08_2020a_M, 2));
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
  if (Template_v3_08_2020a_M->Timing.TaskCounters.TID[1] == 0) {
    Template_v3_08_2020a_M->Timing.RateInteraction.TID1_2 =
      (Template_v3_08_2020a_M->Timing.TaskCounters.TID[2] == 0);
  }

  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (Template_v3_08_2020a_M->Timing.TaskCounters.TID[2])++;
  if ((Template_v3_08_2020a_M->Timing.TaskCounters.TID[2]) > 1) {// Sample time: [0.1s, 0.0s] 
    Template_v3_08_2020a_M->Timing.TaskCounters.TID[2] = 0;
  }
}

//
// Output and update for atomic system:
//    '<S34>/MATLAB Function10'
//    '<S46>/MATLAB Function6'
//    '<S58>/MATLAB Function5'
//
void Template_v_MATLABFunction10(const real_T rtu_PWMs[8],
  B_MATLABFunction10_Template_v_T *localB)
{
  real_T sum_of_duty_cycles;
  boolean_T x[8];
  int32_T idx;
  boolean_T tmp;
  int32_T i;
  boolean_T exitg1;
  memcpy(&localB->PWMs[0], &rtu_PWMs[0], sizeof(real_T) << 3U);

  // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function10': '<S38>:1' 
  // '<S38>:1:4' if max(PWMs) > 1
  tmp = rtIsNaN(rtu_PWMs[0]);
  if (!tmp) {
    idx = 1;
  } else {
    idx = 0;
    i = 2;
    exitg1 = false;
    while ((!exitg1) && (i < 9)) {
      if (!rtIsNaN(rtu_PWMs[i - 1])) {
        idx = i;
        exitg1 = true;
      } else {
        i++;
      }
    }
  }

  if (idx == 0) {
    sum_of_duty_cycles = rtu_PWMs[0];
  } else {
    sum_of_duty_cycles = rtu_PWMs[idx - 1];
    while (idx + 1 < 9) {
      if (sum_of_duty_cycles < rtu_PWMs[idx]) {
        sum_of_duty_cycles = rtu_PWMs[idx];
      }

      idx++;
    }
  }

  if (sum_of_duty_cycles > 1.0) {
    // '<S38>:1:5' PWMs = PWMs/max(PWMs);
    if (!tmp) {
      idx = 1;
    } else {
      idx = 0;
      i = 2;
      exitg1 = false;
      while ((!exitg1) && (i < 9)) {
        if (!rtIsNaN(rtu_PWMs[i - 1])) {
          idx = i;
          exitg1 = true;
        } else {
          i++;
        }
      }
    }

    if (idx == 0) {
      sum_of_duty_cycles = rtu_PWMs[0];
    } else {
      sum_of_duty_cycles = rtu_PWMs[idx - 1];
      while (idx + 1 < 9) {
        if (sum_of_duty_cycles < rtu_PWMs[idx]) {
          sum_of_duty_cycles = rtu_PWMs[idx];
        }

        idx++;
      }
    }

    for (i = 0; i < 8; i++) {
      localB->PWMs[i] = rtu_PWMs[i] / sum_of_duty_cycles;
    }
  }

  // '<S38>:1:8' count = sum(PWMs > thruster_count_threshold);
  for (i = 0; i < 8; i++) {
    x[i] = (localB->PWMs[i] > Template_v3_08_2020a_P.thruster_count_threshold);
  }

  idx = x[0];
  for (i = 0; i < 7; i++) {
    idx += x[i + 1];
  }

  // '<S38>:1:10' positives_only = max(PWMs,0);
  for (i = 0; i < 8; i++) {
    if (localB->PWMs[i] > 0.0) {
      localB->z1[i] = localB->PWMs[i];
    } else {
      localB->z1[i] = 0.0;
    }
  }

  // '<S38>:1:11' sum_of_duty_cycles = sum(positives_only);
  sum_of_duty_cycles = localB->z1[0];
  for (i = 0; i < 7; i++) {
    sum_of_duty_cycles += localB->z1[i + 1];
  }

  // '<S38>:1:13' average_duty_cycle_per_thruster = sum_of_duty_cycles/count;
  sum_of_duty_cycles /= static_cast<real_T>(idx);

  // '<S38>:1:16' if (average_duty_cycle_per_thruster < 0.3) || (count == 0)
  if ((sum_of_duty_cycles < 0.3) || (idx == 0)) {
    // '<S38>:1:17' thrust_decay_factor = 1;
    localB->thrust_decay_factor = 1.0;
  } else {
    // '<S38>:1:18' else
    // '<S38>:1:19' thrust_decay_factor = max(0.6 - 2*average_duty_cycle_per_thruster + 1, 0.5); 
    sum_of_duty_cycles = (0.6 - 2.0 * sum_of_duty_cycles) + 1.0;
    if (sum_of_duty_cycles > 0.5) {
      localB->thrust_decay_factor = sum_of_duty_cycles;
    } else {
      localB->thrust_decay_factor = 0.5;
    }
  }

  localB->count = idx;
}

//
// Output and update for atomic system:
//    '<S34>/MATLAB Function2'
//    '<S46>/MATLAB Function2'
//    '<S58>/MATLAB Function2'
//
void Template_v3_MATLABFunction2(const real_T rtu_ThrustPer[8],
  B_MATLABFunction2_Template_v3_T *localB)
{
  real_T ex;
  int32_T idx;
  int32_T k;
  boolean_T tmp;
  boolean_T exitg1;

  // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function2': '<S40>:1' 
  // '<S40>:1:3' ValveTime       = 0.007;
  // '<S40>:1:4' TControl        = 0.1;
  // '<S40>:1:5' ThrustPer_Final = zeros(8,1);
  // '<S40>:1:7' if max(ThrustPer) > 1
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
    // '<S40>:1:8' ThrustPer_Sat = ThrustPer/max(ThrustPer);
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
    // '<S40>:1:9' else
    // '<S40>:1:10' ThrustPer_Sat = ThrustPer;
    memcpy(&localB->ThrustPer_Sat[0], &rtu_ThrustPer[0], sizeof(real_T) << 3U);
  }

  // '<S40>:1:13' for i = 1:8
  for (idx = 0; idx < 8; idx++) {
    // '<S40>:1:14' if ThrustPer_Sat(i) > ValveTime/TControl
    if (localB->ThrustPer_Sat[idx] > 0.069999999999999993) {
      // '<S40>:1:15' ThrustPer_Final(i) = ThrustPer_Sat(i);
      localB->ThrustPer_Final[idx] = localB->ThrustPer_Sat[idx];
    } else {
      // '<S40>:1:16' else
      // '<S40>:1:17' ThrustPer_Final(i) = 0;
      localB->ThrustPer_Final[idx] = 0.0;
    }
  }
}

//
// Output and update for atomic system:
//    '<S41>/Create Rotation Matrix'
//    '<S53>/Create Rotation Matrix'
//    '<S71>/Create Rotation Matrix'
//    '<S109>/Create Rotation Matrix'
//    '<S110>/Create Rotation Matrix'
//    '<S111>/Create Rotation Matrix'
//
void Templa_CreateRotationMatrix(real_T rtu_Rz, B_CreateRotationMatrix_Templa_T *
  localB)
{
  real_T r;
  real_T t;
  real_T C_bI_idx_0;
  real_T C_bI_idx_2;
  real_T C_bI_idx_1;

  // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/Rotate Forces to Inertial/Create Rotation Matrix': '<S44>:1' 
  // '<S44>:1:3' C_bI = [  cos(Rz) sin(Rz)
  // '<S44>:1:4'          -sin(Rz) cos(Rz) ];
  r = sin(rtu_Rz);
  t = cos(rtu_Rz);
  C_bI_idx_0 = t;
  C_bI_idx_2 = r;
  C_bI_idx_1 = -r;

  // '<S44>:1:6' C_Ib = inv(C_bI);
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
//    '<S35>/Create Rotation Matrix'
//    '<S47>/Create Rotation Matrix'
//    '<S59>/Create Rotation Matrix'
//
void Temp_CreateRotationMatrix_h(real_T rtu_Rz, B_CreateRotationMatrix_Temp_i_T *
  localB)
{
  real_T C_bI_tmp;
  real_T C_bI_tmp_0;

  // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Rotate Forces to Body/Create Rotation Matrix': '<S45>:1' 
  // '<S45>:1:3' C_bI = [  cos(Rz) sin(Rz)
  // '<S45>:1:4'          -sin(Rz) cos(Rz) ];
  C_bI_tmp = sin(rtu_Rz);
  C_bI_tmp_0 = cos(rtu_Rz);
  localB->C_bI[0] = C_bI_tmp_0;
  localB->C_bI[2] = C_bI_tmp;
  localB->C_bI[1] = -C_bI_tmp;
  localB->C_bI[3] = C_bI_tmp_0;
}

//
// Output and update for atomic system:
//    '<S46>/MATLAB Function'
//    '<S46>/MATLAB Function1'
//
void Template_v3__MATLABFunction(B_MATLABFunction_Template_v3__T *localB)
{
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T H_tmp;
  static const int8_T b[8] = { -1, -1, 0, 0, 1, 1, 0, 0 };

  static const int8_T c[8] = { 0, 0, 1, 1, 0, 0, -1, -1 };

  // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S48>:1' 
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
  // '<S48>:1:21' Vec3 = thruster_dist2CG_BLUE./1000;
  // '<S48>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
  // '<S48>:1:25' Mat2 = diag((F_thrusters_BLUE./2));
  for (i = 0; i < 8; i++) {
    localB->v[i] = Template_v3_08_2020a_P.F_thrusters_BLUE[i] / 2.0;
  }

  memset(&localB->Mat2[0], 0, sizeof(real_T) << 6U);

  // '<S48>:1:27' H    = Mat1*Mat2;
  for (i = 0; i < 8; i++) {
    localB->Mat2[i + (i << 3)] = localB->v[i];
    localB->b[3 * i] = b[i];
    localB->b[3 * i + 1] = c[i];
    localB->b[3 * i + 2] = Template_v3_08_2020a_P.thruster_dist2CG_BLUE[i] /
      1000.0;
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

static void Templa_AHRSFilterBase_resetImpl(fusion_simulink_ahrsfilter_Te_T *obj)
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

static void Template_v3_08_202_NED_ecompass(const real_T a[3], const real_T m[3],
  real_T R[9], B_AHRS2_Template_v3_08_2020a_T *localB)
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

static void Templ_quaternioncg_quaternioncg(const real_T varargin_1[9], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Template_v3_08_2020a_T *localB)
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

static void Tem_quaternioncg_quaternioncg_f(const real_T varargin_1[3], real_T
  *obj_a, real_T *obj_b, real_T *obj_c, real_T *obj_d,
  B_AHRS2_Template_v3_08_2020a_T *localB)
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

static void Template__quaternionBase_mtimes(real_T x_a, real_T x_b, real_T x_c,
  real_T x_d, real_T y_a, real_T y_b, real_T y_c, real_T y_d, real_T *o_a,
  real_T *o_b, real_T *o_c, real_T *o_d)
{
  *o_a = ((x_a * y_a - x_b * y_b) - x_c * y_c) - x_d * y_d;
  *o_b = ((x_a * y_b + x_b * y_a) + x_c * y_d) - x_d * y_c;
  *o_c = ((x_a * y_c - x_b * y_d) + x_c * y_a) + x_d * y_b;
  *o_d = ((x_a * y_d + x_b * y_c) - x_c * y_b) + x_d * y_a;
}

static void IMUFusionCommon_predictOrientat(const
  fusion_simulink_ahrsfilter_Te_T *obj, const real_T gfast[3], const real_T
  offset[3], real_T qorient_a, real_T qorient_b, real_T qorient_c, real_T
  qorient_d, real_T *b_qorient_a, real_T *b_qorient_b, real_T *b_qorient_c,
  real_T *b_qorient_d, B_AHRS2_Template_v3_08_2020a_T *localB)
{
  localB->c[0] = (gfast[0] - offset[0]) * obj->pSensorPeriod;
  localB->c[1] = (gfast[1] - offset[1]) * obj->pSensorPeriod;
  localB->c[2] = (gfast[2] - offset[2]) * obj->pSensorPeriod;
  Tem_quaternioncg_quaternioncg_f(localB->c, &localB->assign_temp_a_k,
    &localB->assign_temp_b_c, &localB->assign_temp_c_b, &localB->assign_temp_d_p,
    localB);
  Template__quaternionBase_mtimes(qorient_a, qorient_b, qorient_c, qorient_d,
    localB->assign_temp_a_k, localB->assign_temp_b_c, localB->assign_temp_c_b,
    localB->assign_temp_d_p, b_qorient_a, b_qorient_b, b_qorient_c, b_qorient_d);
  if (*b_qorient_a < 0.0) {
    *b_qorient_a = -*b_qorient_a;
    *b_qorient_b = -*b_qorient_b;
    *b_qorient_c = -*b_qorient_c;
    *b_qorient_d = -*b_qorient_d;
  }
}

static void Template__quaternionBase_rotmat(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T r[9], B_AHRS2_Template_v3_08_2020a_T *localB)
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

static void Temp_IMUFusionCommon_buildHPart(const real_T v[3], real_T h[9],
  B_AHRS2_Template_v3_08_2020a_T *localB)
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

static real_T Template_v3_08_2020a_norm(const real_T x[3],
  B_AHRS2_Template_v3_08_2020a_T *localB)
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

static void Template_v3_quaternionBase_conj(real_T q_a, real_T q_b, real_T q_c,
  real_T q_d, real_T *b_q_a, real_T *b_q_b, real_T *b_q_c, real_T *b_q_d)
{
  *b_q_a = q_a;
  *b_q_b = -q_b;
  *b_q_c = -q_c;
  *b_q_d = -q_d;
}

static void Template__quaternionBase_uminus(real_T obj_a, real_T obj_b, real_T
  obj_c, real_T obj_d, real_T *b_obj_a, real_T *b_obj_b, real_T *b_obj_c, real_T
  *b_obj_d)
{
  *b_obj_a = -obj_a;
  *b_obj_b = -obj_b;
  *b_obj_c = -obj_c;
  *b_obj_d = -obj_d;
}

static void Templa_quaternionBase_normalize(real_T q_a, real_T q_b, real_T q_c,
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

static void Templa_quaternioncg_parenAssign(real_T rhs_a, real_T rhs_b, real_T
  rhs_c, real_T rhs_d, real_T *o_a, real_T *o_b, real_T *o_c, real_T *o_d)
{
  *o_a = rhs_a;
  *o_b = rhs_b;
  *o_c = rhs_c;
  *o_d = rhs_d;
}

static void Template_v3_ahrsfilter_stepImpl(fusion_simulink_ahrsfilter_Te_T *obj,
  const real_T accelIn[3], const real_T gyroIn[3], const real_T magIn[3], real_T
  orientOut[4], real_T av[3], B_AHRS2_Template_v3_08_2020a_T *localB)
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
    Template_v3_08_202_NED_ecompass(accelIn, magIn, localB->Rprior, localB);
    Templ_quaternioncg_quaternioncg(localB->Rprior, &obj->pOrientPost.a,
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
  Template__quaternionBase_rotmat(obj->pOrientPrior.a, obj->pOrientPrior.b,
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
  Temp_IMUFusionCommon_buildHPart(&localB->Rprior[6], localB->h1, localB);
  for (c_0 = 0; c_0 < 3; c_0++) {
    localB->offDiag[c_0] = localB->Rprior[c_0 + 6] * obj->pMagVec[2] +
      (localB->Rprior[c_0 + 3] * obj->pMagVec[1] + localB->Rprior[c_0] *
       obj->pMagVec[0]);
  }

  Temp_IMUFusionCommon_buildHPart(localB->offDiag, localB->Rprior, localB);
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

  localB->temp = Template_v3_08_2020a_norm(localB->offDiag, localB);
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

  Tem_quaternioncg_quaternioncg_f(localB->gravityAccelGyroDiff, &localB->temp,
    &localB->smax, &localB->s, &localB->assign_temp_d_c, localB);
  Template_v3_quaternionBase_conj(localB->temp, localB->smax, localB->s,
    localB->assign_temp_d_c, &localB->assign_temp_a, &localB->assign_temp_b,
    &localB->assign_temp_c, &localB->assign_temp_d);
  Template__quaternionBase_mtimes(obj->pOrientPrior.a, obj->pOrientPrior.b,
    obj->pOrientPrior.c, obj->pOrientPrior.d, localB->assign_temp_a,
    localB->assign_temp_b, localB->assign_temp_c, localB->assign_temp_d,
    &obj->pOrientPost.a, &obj->pOrientPost.b, &obj->pOrientPost.c,
    &obj->pOrientPost.d);
  if (obj->pOrientPost.a < 0.0) {
    Template__quaternionBase_uminus(obj->pOrientPost.a, obj->pOrientPost.b,
      obj->pOrientPost.c, obj->pOrientPost.d, &obj->pOrientPost.a,
      &obj->pOrientPost.b, &obj->pOrientPost.c, &obj->pOrientPost.d);
  }

  Templa_quaternionBase_normalize(obj->pOrientPost.a, obj->pOrientPost.b,
    obj->pOrientPost.c, obj->pOrientPost.d, &obj->pOrientPost.a,
    &obj->pOrientPost.b, &obj->pOrientPost.c, &obj->pOrientPost.d);
  Template__quaternionBase_rotmat(obj->pOrientPost.a, obj->pOrientPost.b,
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
  Templa_quaternioncg_parenAssign(obj->pOrientPost.a, obj->pOrientPost.b,
    obj->pOrientPost.c, obj->pOrientPost.d, &orientOut[0], &orientOut[1],
    &orientOut[2], &orientOut[3]);
}

//
// System initialize for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void Template_v3_08_2_AHRS2_Init(const real_T rtu_0[3],
  DW_AHRS2_Template_v3_08_2020a_T *localDW, P_AHRS2_Template_v3_08_2020a_T
  *localP)
{
  // Start for MATLABSystem: '<S109>/AHRS2'
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

  // InitializeConditions for MATLABSystem: '<S109>/AHRS2'
  Templa_AHRSFilterBase_resetImpl(&localDW->obj);
}

//
// Output and update for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void Template_v3_08_2020a_AHRS2(const real_T rtu_0[3], const real_T rtu_1[3],
  const real_T rtu_2[3], B_AHRS2_Template_v3_08_2020a_T *localB,
  DW_AHRS2_Template_v3_08_2020a_T *localDW, P_AHRS2_Template_v3_08_2020a_T
  *localP)
{
  int32_T tmp;
  static const int8_T tmp_0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  // MATLABSystem: '<S109>/AHRS2'
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

  Template_v3_ahrsfilter_stepImpl(&localDW->obj, rtu_0, rtu_1, rtu_2,
    localB->b_varargout_1, localB->b_varargout_2, localB);
  localB->AHRS2_o2[0] = localB->b_varargout_2[0];
  localB->AHRS2_o2[1] = localB->b_varargout_2[1];
  localB->AHRS2_o2[2] = localB->b_varargout_2[2];

  // End of MATLABSystem: '<S109>/AHRS2'
}

//
// Output and update for atomic system:
//    '<S109>/ChangeOrientation'
//    '<S110>/ChangeOrientation'
//    '<S111>/ChangeOrientation'
//
void Template__ChangeOrientation(real_T rtu_u, real_T rtu_u_e, real_T rtu_u_c,
  B_ChangeOrientation_Template__T *localB)
{
  int32_T i;
  static const int8_T a[9] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };

  // MATLAB Function 'IMU Data  and Algorithms/Change BLACK Behavior/ChangeOrientation': '<S113>:1' 
  // '<S113>:1:3' y = rotz(-90)*u';
  for (i = 0; i < 3; i++) {
    localB->y[i] = 0.0;

    // SignalConversion generated from: '<S113>/ SFunction '
    localB->y[i] += static_cast<real_T>(a[i]) * rtu_u;
    localB->y[i] += static_cast<real_T>(a[i + 3]) * rtu_u_e;
    localB->y[i] += static_cast<real_T>(a[i + 6]) * rtu_u_c;
  }
}

//
// System initialize for action system:
//    '<S119>/Calculate Running Mean'
//    '<S120>/Calculate Running Mean'
//    '<S121>/Calculate Running Mean'
//    '<S132>/Calculate Running Mean'
//    '<S133>/Calculate Running Mean'
//    '<S150>/Calculate Running Mean'
//    '<S151>/Calculate Running Mean'
//    '<S152>/Calculate Running Mean'
//    '<S163>/Calculate Running Mean'
//    '<S164>/Calculate Running Mean'
//    ...
//
void T_CalculateRunningMean_Init(B_CalculateRunningMean_Templa_T *localB,
  DW_CalculateRunningMean_Templ_T *localDW, P_CalculateRunningMean_Templa_T
  *localP)
{
  int32_T i;

  // InitializeConditions for Delay: '<S122>/Delay'
  localDW->Delay_DSTATE = localP->Delay_InitialCondition;

  // InitializeConditions for Delay: '<S122>/Delay1'
  localDW->Delay1_DSTATE[0] = localP->Delay1_InitialCondition;
  localDW->Delay1_DSTATE[1] = localP->Delay1_InitialCondition;

  // InitializeConditions for Delay: '<S122>/Delay2'
  localDW->Delay2_DSTATE[0] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[1] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[2] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[3] = localP->Delay2_InitialCondition;

  // InitializeConditions for Delay: '<S122>/Delay3'
  for (i = 0; i < 5; i++) {
    localDW->Delay3_DSTATE[i] = localP->Delay3_InitialCondition;
  }

  // End of InitializeConditions for Delay: '<S122>/Delay3'

  // InitializeConditions for Delay: '<S122>/Delay4'
  localDW->Delay4_DSTATE[0] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[1] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[2] = localP->Delay4_InitialCondition;

  // SystemInitialize for Outport: '<S122>/Out1'
  localB->Mean = localP->Out1_Y0;
}

//
// Output and update for action system:
//    '<S119>/Calculate Running Mean'
//    '<S120>/Calculate Running Mean'
//    '<S121>/Calculate Running Mean'
//    '<S132>/Calculate Running Mean'
//    '<S133>/Calculate Running Mean'
//    '<S150>/Calculate Running Mean'
//    '<S151>/Calculate Running Mean'
//    '<S152>/Calculate Running Mean'
//    '<S163>/Calculate Running Mean'
//    '<S164>/Calculate Running Mean'
//    ...
//
void Templa_CalculateRunningMean(real_T rtu_In1, B_CalculateRunningMean_Templa_T
  *localB, DW_CalculateRunningMean_Templ_T *localDW)
{
  int32_T n;
  int32_T str;
  int_T idxWidth;
  real_T rtb_TmpSignalConversionAtMeanIn[6];

  // SignalConversion generated from: '<S122>/Mean' incorporates:
  //   Delay: '<S122>/Delay'
  //   Delay: '<S122>/Delay1'
  //   Delay: '<S122>/Delay2'
  //   Delay: '<S122>/Delay3'
  //   Delay: '<S122>/Delay4'

  rtb_TmpSignalConversionAtMeanIn[0] = rtu_In1;
  rtb_TmpSignalConversionAtMeanIn[1] = localDW->Delay_DSTATE;
  rtb_TmpSignalConversionAtMeanIn[2] = localDW->Delay1_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[3] = localDW->Delay4_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[4] = localDW->Delay2_DSTATE[0U];
  rtb_TmpSignalConversionAtMeanIn[5] = localDW->Delay3_DSTATE[0U];

  // S-Function (sdspstatfcns): '<S122>/Mean'
  for (idxWidth = 0; idxWidth < 1; idxWidth++) {
    localDW->Mean_AccVal = rtb_TmpSignalConversionAtMeanIn[idxWidth];
    str = 1;
    for (n = 4; n >= 0; n--) {
      localDW->Mean_AccVal += rtb_TmpSignalConversionAtMeanIn[idxWidth + str];
      str++;
    }

    localB->Mean = localDW->Mean_AccVal / 6.0;
  }

  // End of S-Function (sdspstatfcns): '<S122>/Mean'

  // Update for Delay: '<S122>/Delay'
  localDW->Delay_DSTATE = rtu_In1;

  // Update for Delay: '<S122>/Delay1'
  localDW->Delay1_DSTATE[0] = localDW->Delay1_DSTATE[1];
  localDW->Delay1_DSTATE[1] = rtu_In1;

  // Update for Delay: '<S122>/Delay2'
  localDW->Delay2_DSTATE[0] = localDW->Delay2_DSTATE[1];
  localDW->Delay2_DSTATE[1] = localDW->Delay2_DSTATE[2];
  localDW->Delay2_DSTATE[2] = localDW->Delay2_DSTATE[3];
  localDW->Delay2_DSTATE[3] = rtu_In1;

  // Update for Delay: '<S122>/Delay3'
  localDW->Delay3_DSTATE[0] = localDW->Delay3_DSTATE[1];
  localDW->Delay3_DSTATE[1] = localDW->Delay3_DSTATE[2];
  localDW->Delay3_DSTATE[2] = localDW->Delay3_DSTATE[3];
  localDW->Delay3_DSTATE[3] = localDW->Delay3_DSTATE[4];
  localDW->Delay3_DSTATE[4] = rtu_In1;

  // Update for Delay: '<S122>/Delay4'
  localDW->Delay4_DSTATE[0] = localDW->Delay4_DSTATE[1];
  localDW->Delay4_DSTATE[1] = localDW->Delay4_DSTATE[2];
  localDW->Delay4_DSTATE[2] = rtu_In1;
}

//
// System initialize for action system:
//    '<S134>/Calculate Running Mean'
//    '<S165>/Calculate Running Mean'
//    '<S196>/Calculate Running Mean'
//
void CalculateRunningMean_o_Init(B_CalculateRunningMean_Temp_f_T *localB,
  DW_CalculateRunningMean_Tem_o_T *localDW, P_CalculateRunningMean_Temp_k_T
  *localP)
{
  int32_T i;

  // InitializeConditions for Delay: '<S141>/Delay'
  localDW->Delay_DSTATE = localP->Delay_InitialCondition;

  // InitializeConditions for Delay: '<S141>/Delay1'
  localDW->Delay1_DSTATE[0] = localP->Delay1_InitialCondition;
  localDW->Delay1_DSTATE[1] = localP->Delay1_InitialCondition;

  // InitializeConditions for Delay: '<S141>/Delay2'
  localDW->Delay2_DSTATE[0] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[1] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[2] = localP->Delay2_InitialCondition;
  localDW->Delay2_DSTATE[3] = localP->Delay2_InitialCondition;

  // InitializeConditions for Delay: '<S141>/Delay3'
  for (i = 0; i < 5; i++) {
    localDW->Delay3_DSTATE[i] = localP->Delay3_InitialCondition;
  }

  // End of InitializeConditions for Delay: '<S141>/Delay3'

  // InitializeConditions for Delay: '<S141>/Delay4'
  localDW->Delay4_DSTATE[0] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[1] = localP->Delay4_InitialCondition;
  localDW->Delay4_DSTATE[2] = localP->Delay4_InitialCondition;

  // SystemInitialize for Outport: '<S141>/Out1'
  localB->Subtract = localP->Out1_Y0;
}

//
// Output and update for action system:
//    '<S134>/Calculate Running Mean'
//    '<S165>/Calculate Running Mean'
//    '<S196>/Calculate Running Mean'
//
void Temp_CalculateRunningMean_e(real_T rtu_In1, B_CalculateRunningMean_Temp_f_T
  *localB, DW_CalculateRunningMean_Tem_o_T *localDW,
  P_CalculateRunningMean_Temp_k_T *localP)
{
  int32_T n;
  int32_T str;
  int_T idxWidth;
  real_T rtb_Mean;

  // Delay: '<S141>/Delay4'
  rtb_Mean = localDW->Delay4_DSTATE[0];

  // SignalConversion generated from: '<S141>/Mean' incorporates:
  //   Delay: '<S141>/Delay'
  //   Delay: '<S141>/Delay1'
  //   Delay: '<S141>/Delay2'
  //   Delay: '<S141>/Delay3'
  //   Delay: '<S141>/Delay4'

  localB->TmpSignalConversionAtMean_b[0] = rtu_In1;
  localB->TmpSignalConversionAtMean_b[1] = localDW->Delay_DSTATE;
  localB->TmpSignalConversionAtMean_b[2] = localDW->Delay1_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[3] = localDW->Delay4_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[4] = localDW->Delay2_DSTATE[0U];
  localB->TmpSignalConversionAtMean_b[5] = localDW->Delay3_DSTATE[0U];

  // S-Function (sdspstatfcns): '<S141>/Mean'
  for (idxWidth = 0; idxWidth < 1; idxWidth++) {
    localDW->Mean_AccVal = localB->TmpSignalConversionAtMean_b[idxWidth];
    str = 1;
    for (n = 4; n >= 0; n--) {
      localDW->Mean_AccVal += localB->TmpSignalConversionAtMean_b[idxWidth + str];
      str++;
    }

    rtb_Mean = localDW->Mean_AccVal / 6.0;
  }

  // End of S-Function (sdspstatfcns): '<S141>/Mean'

  // Sum: '<S141>/Subtract' incorporates:
  //   Constant: '<S141>/Constant'

  localB->Subtract = rtb_Mean - localP->Constant_Value;

  // Update for Delay: '<S141>/Delay'
  localDW->Delay_DSTATE = rtu_In1;

  // Update for Delay: '<S141>/Delay1'
  localDW->Delay1_DSTATE[0] = localDW->Delay1_DSTATE[1];
  localDW->Delay1_DSTATE[1] = rtu_In1;

  // Update for Delay: '<S141>/Delay2'
  localDW->Delay2_DSTATE[0] = localDW->Delay2_DSTATE[1];
  localDW->Delay2_DSTATE[1] = localDW->Delay2_DSTATE[2];
  localDW->Delay2_DSTATE[2] = localDW->Delay2_DSTATE[3];
  localDW->Delay2_DSTATE[3] = rtu_In1;

  // Update for Delay: '<S141>/Delay3'
  localDW->Delay3_DSTATE[0] = localDW->Delay3_DSTATE[1];
  localDW->Delay3_DSTATE[1] = localDW->Delay3_DSTATE[2];
  localDW->Delay3_DSTATE[2] = localDW->Delay3_DSTATE[3];
  localDW->Delay3_DSTATE[3] = localDW->Delay3_DSTATE[4];
  localDW->Delay3_DSTATE[4] = rtu_In1;

  // Update for Delay: '<S141>/Delay4'
  localDW->Delay4_DSTATE[0] = localDW->Delay4_DSTATE[1];
  localDW->Delay4_DSTATE[1] = localDW->Delay4_DSTATE[2];
  localDW->Delay4_DSTATE[2] = rtu_In1;
}

//
// Output and update for atomic system:
//    '<S208>/Correct Dropouts'
//    '<S209>/Correct Dropouts'
//    '<S210>/Correct Dropouts'
//
void Template_v3_CorrectDropouts(real_T rtu_InputRaw, real_T rtu_InputRaw_f,
  B_CorrectDropouts_Template_v3_T *localB)
{
  // SignalConversion generated from: '<S211>/ SFunction '
  // MATLAB Function 'Manipulator Encoder Data/Change Behavior/Correct for dropouts/Correct Dropouts': '<S211>:1' 
  // '<S211>:1:3' if (InputRaw(1) < -3)
  if (rtu_InputRaw < -3.0) {
    // '<S211>:1:5' FixedInput = InputRaw(2);
    localB->FixedInput = rtu_InputRaw_f;
  } else {
    // '<S211>:1:15' else
    // '<S211>:1:17' FixedInput = InputRaw(1);
    localB->FixedInput = rtu_InputRaw;
  }

  // End of SignalConversion generated from: '<S211>/ SFunction '
}

//
// Output and update for action system:
//    '<S10>/Change BLUE Behavior'
//    '<S11>/Change BLUE Behavior'
//    '<S16>/Change BLUE Behavior'
//
void Template_ChangeBLUEBehavior(P_ChangeBLUEBehavior_Template_T *localP, real_T
  *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  // DataStoreWrite: '<S219>/BLUE_Fx' incorporates:
  //   Constant: '<S219>/Constant3'

  *rtd_BLUE_Fx = localP->Constant3_Value;

  // DataStoreWrite: '<S219>/BLUE_Fy' incorporates:
  //   Constant: '<S219>/Constant4'

  *rtd_BLUE_Fy = localP->Constant4_Value;

  // DataStoreWrite: '<S219>/BLUE_Tz' incorporates:
  //   Constant: '<S219>/Constant5'

  *rtd_BLUE_Tz = localP->Constant5_Value;

  // DataStoreWrite: '<S219>/Data Store Write4' incorporates:
  //   Constant: '<S219>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;
}

//
// Output and update for action system:
//    '<Root>/Phase #0:  Wait for Synchronization'
//    '<Root>/Phase #1:  Start Floating '
//
void Phase0WaitforSynchronizatio(P_Phase0WaitforSynchronizatio_T *localP, real_T
  *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Tz, real_T *rtd_BLUE_Fx,
  real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T
  *rtd_RED_Fx, real_T *rtd_RED_Fy, real_T *rtd_RED_Tz, real_T *rtd_RED_Tz_Elbow,
  real_T *rtd_RED_Tz_RW, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist,
  real_T *rtd_WhoAmI)
{
  // If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
  //   Constant: '<S221>/Constant'

  if ((*rtd_WhoAmI == 2.0) || (Template_v3_08_2020a_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change BLACK Behavior' incorporates:
    //   ActionPort: '<S218>/Action Port'

    // DataStoreWrite: '<S218>/BLACK_Fx' incorporates:
    //   Constant: '<S218>/Constant3'

    *rtd_BLACK_Fx = localP->Constant3_Value;

    // DataStoreWrite: '<S218>/BLACK_Fy' incorporates:
    //   Constant: '<S218>/Constant4'

    *rtd_BLACK_Fy = localP->Constant4_Value;

    // DataStoreWrite: '<S218>/BLACK_Tz' incorporates:
    //   Constant: '<S218>/Constant5'

    *rtd_BLACK_Tz = localP->Constant5_Value;

    // DataStoreWrite: '<S218>/Data Store Write4' incorporates:
    //   Constant: '<S218>/Puck State'

    *rtd_Float_State = localP->PuckState_Value;

    // End of Outputs for SubSystem: '<S10>/Change BLACK Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the BLACK sim//exp' 

  // If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
  //   Constant: '<S221>/Constant'

  if ((*rtd_WhoAmI == 3.0) || (Template_v3_08_2020a_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change BLUE Behavior' incorporates:
    //   ActionPort: '<S219>/Action Port'

    Template_ChangeBLUEBehavior(&localP->ChangeBLUEBehavior, rtd_BLUE_Fx,
      rtd_BLUE_Fy, rtd_BLUE_Tz, rtd_Float_State);

    // End of Outputs for SubSystem: '<S10>/Change BLUE Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the BLUE sim//exp' 

  // If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
  //   Constant: '<S221>/Constant'

  if ((*rtd_WhoAmI == 1.0) || (Template_v3_08_2020a_P.simMode == 1.0)) {
    // Outputs for IfAction SubSystem: '<S10>/Change RED Behavior' incorporates:
    //   ActionPort: '<S220>/Action Port'

    // DataStoreWrite: '<S220>/RED_Fx' incorporates:
    //   Constant: '<S220>/Constant'

    *rtd_RED_Fx = localP->Constant_Value;

    // DataStoreWrite: '<S220>/RED_Fy' incorporates:
    //   Constant: '<S220>/Constant1'

    *rtd_RED_Fy = localP->Constant1_Value;

    // DataStoreWrite: '<S220>/RED_Tz' incorporates:
    //   Constant: '<S220>/Constant2'

    *rtd_RED_Tz = localP->Constant2_Value;

    // DataStoreWrite: '<S220>/RED_Tz_RW' incorporates:
    //   Constant: '<S220>/Constant3'

    *rtd_RED_Tz_RW = localP->Constant3_Value_h;

    // DataStoreWrite: '<S220>/Data Store Write' incorporates:
    //   Constant: '<S220>/Constant4'

    *rtd_RED_Tz_Shoulder = localP->Constant4_Value_g;

    // DataStoreWrite: '<S220>/Data Store Write1' incorporates:
    //   Constant: '<S220>/Constant5'

    *rtd_RED_Tz_Elbow = localP->Constant5_Value_e;

    // DataStoreWrite: '<S220>/Data Store Write2' incorporates:
    //   Constant: '<S220>/Constant6'

    *rtd_RED_Tz_Wrist = localP->Constant6_Value;

    // DataStoreWrite: '<S220>/Data Store Write4' incorporates:
    //   Constant: '<S220>/Puck State'

    *rtd_Float_State = localP->PuckState_Value_b;

    // End of Outputs for SubSystem: '<S10>/Change RED Behavior'
  }

  // End of If: '<S10>/This IF block determines whether or not to run the RED sim//exp ' 
}

//
// Output and update for atomic system:
//    '<S233>/MATLAB Function2'
//    '<S246>/MATLAB Function2'
//    '<S259>/MATLAB Function2'
//    '<S559>/MATLAB Function2'
//    '<S572>/MATLAB Function2'
//    '<S585>/MATLAB Function2'
//    '<S612>/MATLAB Function2'
//    '<S625>/MATLAB Function2'
//    '<S638>/MATLAB Function2'
//
void Template__MATLABFunction2_o(real_T rtu_z, B_MATLABFunction2_Template__n_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function2': '<S240>:1' 
  // '<S240>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S240>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
}

//
// Output and update for atomic system:
//    '<S233>/MATLAB Function3'
//    '<S246>/MATLAB Function3'
//    '<S259>/MATLAB Function3'
//    '<S293>/MATLAB Function3'
//    '<S310>/MATLAB Function3'
//    '<S327>/MATLAB Function3'
//    '<S344>/MATLAB Function3'
//    '<S365>/MATLAB Function3'
//    '<S383>/MATLAB Function3'
//    '<S401>/MATLAB Function3'
//    ...
//
void Template_v3_MATLABFunction3(real_T rtu_z, B_MATLABFunction3_Template_v3_T
  *localB)
{
  real_T Ox_tmp;
  real_T Ox_tmp_0;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function3': '<S241>:1' 
  // '<S241>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
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
//    '<S233>/MATLAB Function4'
//    '<S246>/MATLAB Function4'
//    '<S259>/MATLAB Function4'
//    '<S293>/MATLAB Function4'
//    '<S310>/MATLAB Function4'
//    '<S327>/MATLAB Function4'
//    '<S344>/MATLAB Function4'
//    '<S365>/MATLAB Function4'
//    '<S383>/MATLAB Function4'
//    '<S401>/MATLAB Function4'
//    ...
//
void Template_v3_MATLABFunction4(const real_T rtu_Oy[2], const real_T rtu_e_in[2],
  B_MATLABFunction4_Template_v3_T *localB)
{
  real_T y;
  real_T scale;
  real_T absxk;
  real_T t;

  // MATLAB Function 'Phase #2:  Move to  Initial Position/Change BLACK Behavior/Hough Control/MATLAB Function4': '<S242>:1' 
  // '<S242>:1:3' a = sign(Oy'*e_in);
  // '<S242>:1:4' e_out = -a*norm(e_in);
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

//
// System initialize for action system:
//    '<S228>/RED+ARM'
//    '<S434>/RED+ARM'
//    '<S435>/RED+ARM'
//
void Template_v3_08__REDARM_Init(B_REDARM_Template_v3_08_2020a_T *localB,
  DW_REDARM_Template_v3_08_2020_T *localDW, P_REDARM_Template_v3_08_2020a_T
  *localP)
{
  // InitializeConditions for Delay: '<S274>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S276>/Delay1'
  localDW->icLoad_c = 1U;

  // InitializeConditions for Delay: '<S278>/Delay1'
  localDW->icLoad_j = 1U;

  // SystemInitialize for IfAction SubSystem: '<S274>/Hold this value'
  // SystemInitialize for Outport: '<S275>/Out1'
  localB->In1_l = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S274>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S276>/Hold this value'
  // SystemInitialize for Outport: '<S277>/Out1'
  localB->In1_n = localP->Out1_Y0_k;

  // End of SystemInitialize for SubSystem: '<S276>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S278>/Hold this value'
  // SystemInitialize for Outport: '<S279>/Out1'
  localB->In1 = localP->Out1_Y0_h;

  // End of SystemInitialize for SubSystem: '<S278>/Hold this value'
}

//
// Output and update for action system:
//    '<S228>/RED+ARM'
//    '<S434>/RED+ARM'
//    '<S435>/RED+ARM'
//
void Template_v3_08_2020a_REDARM(B_REDARM_Template_v3_08_2020a_T *localB,
  DW_REDARM_Template_v3_08_2020_T *localDW, P_REDARM_Template_v3_08_2020a_T
  *localP, real_T *rtd_RED_Rz_Elbow, real_T *rtd_RED_Rz_Shoulder, real_T
  *rtd_RED_Rz_Wrist, real_T *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_Shoulder,
  real_T *rtd_RED_Tz_Wrist)
{
  real_T rtb_Subtract3_km;
  real_T rtb_Sum6_ok;
  real_T rtb_Sum6_do;
  real_T rtb_Sum6_kf;

  // Sum: '<S261>/Subtract3' incorporates:
  //   Constant: '<S261>/Constant9'

  rtb_Subtract3_km = localP->Constant9_Value - *rtd_RED_Rz_Elbow;

  // Delay: '<S274>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract3_km;
  }

  // Sum: '<S274>/Sum6' incorporates:
  //   Delay: '<S274>/Delay1'

  rtb_Sum6_ok = rtb_Subtract3_km - localDW->Delay1_DSTATE;

  // If: '<S274>/if we went through a "step"' incorporates:
  //   Inport: '<S275>/In1'

  if (rtb_Sum6_ok != 0.0) {
    // Outputs for IfAction SubSystem: '<S274>/Hold this value' incorporates:
    //   ActionPort: '<S275>/Action Port'

    localB->In1_l = rtb_Sum6_ok;

    // End of Outputs for SubSystem: '<S274>/Hold this value'
  }

  // End of If: '<S274>/if we went through a "step"'

  // Sum: '<S271>/Sum3' incorporates:
  //   Gain: '<S271>/kd_elarm'
  //   Gain: '<S271>/kp_elarm'
  //   Gain: '<S274>/divide by delta T'

  rtb_Sum6_kf = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_l *
    Template_v3_08_2020a_P.Kd_elarm + Template_v3_08_2020a_P.Kp_elarm *
    rtb_Subtract3_km;

  // Saturate: '<S261>/Saturation1'
  if (rtb_Sum6_kf > Template_v3_08_2020a_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = Template_v3_08_2020a_P.Tz_lim_elarm;
  } else if (rtb_Sum6_kf < -Template_v3_08_2020a_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = -Template_v3_08_2020a_P.Tz_lim_elarm;
  } else {
    *rtd_RED_Tz_Elbow = rtb_Sum6_kf;
  }

  // End of Saturate: '<S261>/Saturation1'

  // Sum: '<S261>/Subtract2' incorporates:
  //   Constant: '<S261>/Constant8'

  rtb_Sum6_ok = localP->Constant8_Value - *rtd_RED_Rz_Shoulder;

  // Delay: '<S276>/Delay1'
  if (localDW->icLoad_c != 0) {
    localDW->Delay1_DSTATE_l = rtb_Sum6_ok;
  }

  // Sum: '<S276>/Sum6' incorporates:
  //   Delay: '<S276>/Delay1'

  rtb_Sum6_do = rtb_Sum6_ok - localDW->Delay1_DSTATE_l;

  // If: '<S276>/if we went through a "step"' incorporates:
  //   Inport: '<S277>/In1'

  if (rtb_Sum6_do != 0.0) {
    // Outputs for IfAction SubSystem: '<S276>/Hold this value' incorporates:
    //   ActionPort: '<S277>/Action Port'

    localB->In1_n = rtb_Sum6_do;

    // End of Outputs for SubSystem: '<S276>/Hold this value'
  }

  // End of If: '<S276>/if we went through a "step"'

  // Sum: '<S272>/Sum3' incorporates:
  //   Gain: '<S272>/kd_sharm'
  //   Gain: '<S272>/kp_sharm'
  //   Gain: '<S276>/divide by delta T'

  rtb_Sum6_kf = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_n *
    Template_v3_08_2020a_P.Kd_sharm + Template_v3_08_2020a_P.Kp_sharm *
    rtb_Sum6_ok;

  // Saturate: '<S261>/Saturation'
  if (rtb_Sum6_kf > Template_v3_08_2020a_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = Template_v3_08_2020a_P.Tz_lim_sharm;
  } else if (rtb_Sum6_kf < -Template_v3_08_2020a_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = -Template_v3_08_2020a_P.Tz_lim_sharm;
  } else {
    *rtd_RED_Tz_Shoulder = rtb_Sum6_kf;
  }

  // End of Saturate: '<S261>/Saturation'

  // Sum: '<S261>/Subtract4' incorporates:
  //   Constant: '<S261>/Constant7'

  rtb_Sum6_do = localP->Constant7_Value - *rtd_RED_Rz_Wrist;

  // Delay: '<S278>/Delay1'
  if (localDW->icLoad_j != 0) {
    localDW->Delay1_DSTATE_h = rtb_Sum6_do;
  }

  // Sum: '<S278>/Sum6' incorporates:
  //   Delay: '<S278>/Delay1'

  rtb_Sum6_kf = rtb_Sum6_do - localDW->Delay1_DSTATE_h;

  // If: '<S278>/if we went through a "step"' incorporates:
  //   Inport: '<S279>/In1'

  if (rtb_Sum6_kf != 0.0) {
    // Outputs for IfAction SubSystem: '<S278>/Hold this value' incorporates:
    //   ActionPort: '<S279>/Action Port'

    localB->In1 = rtb_Sum6_kf;

    // End of Outputs for SubSystem: '<S278>/Hold this value'
  }

  // End of If: '<S278>/if we went through a "step"'

  // Sum: '<S273>/Sum3' incorporates:
  //   Gain: '<S273>/kd_wrarm'
  //   Gain: '<S273>/kp_wrarm'
  //   Gain: '<S278>/divide by delta T'

  rtb_Sum6_kf = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1 *
    Template_v3_08_2020a_P.Kd_wrarm + Template_v3_08_2020a_P.Kp_wrarm *
    rtb_Sum6_do;

  // Saturate: '<S261>/Saturation3'
  if (rtb_Sum6_kf > Template_v3_08_2020a_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = Template_v3_08_2020a_P.Tz_lim_wrarm;
  } else if (rtb_Sum6_kf < -Template_v3_08_2020a_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = -Template_v3_08_2020a_P.Tz_lim_wrarm;
  } else {
    *rtd_RED_Tz_Wrist = rtb_Sum6_kf;
  }

  // End of Saturate: '<S261>/Saturation3'

  // Update for Delay: '<S274>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract3_km;

  // Update for Delay: '<S276>/Delay1'
  localDW->icLoad_c = 0U;
  localDW->Delay1_DSTATE_l = rtb_Sum6_ok;

  // Update for Delay: '<S278>/Delay1'
  localDW->icLoad_j = 0U;
  localDW->Delay1_DSTATE_h = rtb_Sum6_do;
}

//
// Output and update for atomic system:
//    '<S295>/MATLAB Function'
//    '<S312>/MATLAB Function'
//    '<S329>/MATLAB Function'
//    '<S346>/MATLAB Function'
//    '<S468>/MATLAB Function'
//    '<S482>/MATLAB Function'
//
void Template_v_MATLABFunction_o(real_T rtu_u, B_MATLABFunction_Template_v_e_T
  *localB)
{
  // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/BLACK_RzD1/Wrap1/MATLAB Function': '<S296>:1' 
  // '<S296>:1:3' wrappedAngle = atan2(sin(u),cos(u));
  localB->wrappedAngle = rt_atan2d_snf(sin(rtu_u), cos(rtu_u));
}

//
// Output and update for atomic system:
//    '<S293>/MATLAB Function2'
//    '<S310>/MATLAB Function2'
//    '<S327>/MATLAB Function2'
//    '<S344>/MATLAB Function2'
//    '<S365>/MATLAB Function2'
//    '<S383>/MATLAB Function2'
//    '<S401>/MATLAB Function2'
//    '<S419>/MATLAB Function2'
//    '<S442>/MATLAB Function2'
//    '<S475>/MATLAB Function2'
//    '<S527>/MATLAB Function2'
//
void Template__MATLABFunction2_l(real_T rtu_z, B_MATLABFunction2_Template__c_T
  *localB)
{
  real_T Ox_tmp_tmp;
  real_T Ox_tmp_tmp_0;

  // MATLAB Function 'Phase #3: Experiment/Change BLACK Behavior/Sub-Phase #1/Hough Control/MATLAB Function2': '<S303>:1' 
  // '<S303>:1:6' Ox = [cos(z) sin(z); -sin(z) cos(z)]*[1;0];
  Ox_tmp_tmp = sin(rtu_z);
  Ox_tmp_tmp_0 = cos(rtu_z);
  localB->Ox[0] = 0.0;
  localB->Ox[0] += Ox_tmp_tmp_0;
  localB->Ox[0] += Ox_tmp_tmp * 0.0;
  localB->Ox[1] = 0.0;
  localB->Ox[1] += -Ox_tmp_tmp;
  localB->Ox[1] += Ox_tmp_tmp_0 * 0.0;

  // '<S303>:1:7' Oy = [cos(z) sin(z); -sin(z) cos(z)]*[0;1];
  localB->Oy[0] = 0.0;
  localB->Oy[0] += cos(rtu_z) * 0.0;
  localB->Oy[0] += Ox_tmp_tmp;
  localB->Oy[1] = 0.0;
  localB->Oy[1] += -sin(rtu_z) * 0.0;
  localB->Oy[1] += Ox_tmp_tmp_0;
}

//
// System initialize for action system:
//    '<S280>/Sub-Phase #1'
//    '<S280>/Sub-Phase #2 '
//    '<S280>/Sub-Phase #3 '
//    '<S280>/Sub-Phase #4'
//
void Template_v3__SubPhase1_Init(B_SubPhase1_Template_v3_08_20_T *localB,
  DW_SubPhase1_Template_v3_08_2_T *localDW, P_SubPhase1_Template_v3_08_20_T
  *localP)
{
  // InitializeConditions for Delay: '<S299>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S301>/Delay1'
  localDW->icLoad_p = 1U;

  // InitializeConditions for Delay: '<S297>/Delay1'
  localDW->icLoad_pd = 1U;

  // SystemInitialize for IfAction SubSystem: '<S299>/Hold this value'
  // SystemInitialize for Outport: '<S300>/Out1'
  localB->In1_l = localP->Out1_Y0_n;

  // End of SystemInitialize for SubSystem: '<S299>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S301>/Hold this value'
  // SystemInitialize for Outport: '<S302>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S301>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S297>/Hold this value'
  // SystemInitialize for Outport: '<S298>/Out1'
  localB->In1_j = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S297>/Hold this value'
}

//
// Output and update for action system:
//    '<S280>/Sub-Phase #1'
//    '<S280>/Sub-Phase #2 '
//    '<S280>/Sub-Phase #3 '
//    '<S280>/Sub-Phase #4'
//
void Template_v3_08_20_SubPhase1(B_SubPhase1_Template_v3_08_20_T *localB,
  DW_SubPhase1_Template_v3_08_2_T *localDW, P_SubPhase1_Template_v3_08_20_T
  *localP, real_T *rtd_BLACK_Fx, real_T *rtd_BLACK_Fy, real_T *rtd_BLACK_Px,
  real_T *rtd_BLACK_Py, real_T *rtd_BLACK_Rz, real_T *rtd_BLACK_Tz, real_T
  *rtd_Float_State, real_T *rtd_Univ_Time)
{
  real_T rtb_Subtract_cv;
  real_T rtb_Sum6_dm;
  real_T rtb_Sum6_hb;

  // Sum: '<S284>/Subtract' incorporates:
  //   Constant: '<S284>/Desired X-Position (BLACK)'

  rtb_Subtract_cv = Template_v3_08_2020a_P.xLength / 2.0 - *rtd_BLACK_Px;

  // Delay: '<S299>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_cv;
  }

  // Sum: '<S299>/Sum6' incorporates:
  //   Delay: '<S299>/Delay1'

  rtb_Sum6_dm = rtb_Subtract_cv - localDW->Delay1_DSTATE;

  // If: '<S299>/if we went through a "step"' incorporates:
  //   Inport: '<S300>/In1'

  if (rtb_Sum6_dm != 0.0) {
    // Outputs for IfAction SubSystem: '<S299>/Hold this value' incorporates:
    //   ActionPort: '<S300>/Action Port'

    localB->In1_l = rtb_Sum6_dm;

    // End of Outputs for SubSystem: '<S299>/Hold this value'
  }

  // End of If: '<S299>/if we went through a "step"'

  // Sum: '<S291>/Sum3' incorporates:
  //   Gain: '<S291>/kd_xb'
  //   Gain: '<S291>/kp_xb'
  //   Gain: '<S299>/divide by delta T'

  *rtd_BLACK_Fx = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_l *
    Template_v3_08_2020a_P.Kd_xb + Template_v3_08_2020a_P.Kp_xb *
    rtb_Subtract_cv;

  // Sum: '<S284>/Subtract1' incorporates:
  //   Constant: '<S284>/Desired Y-Position (BLACK)'

  rtb_Sum6_dm = Template_v3_08_2020a_P.yLength / 2.0 - *rtd_BLACK_Py;

  // Delay: '<S301>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_j = rtb_Sum6_dm;
  }

  // Sum: '<S301>/Sum6' incorporates:
  //   Delay: '<S301>/Delay1'

  rtb_Sum6_hb = rtb_Sum6_dm - localDW->Delay1_DSTATE_j;

  // If: '<S301>/if we went through a "step"' incorporates:
  //   Inport: '<S302>/In1'

  if (rtb_Sum6_hb != 0.0) {
    // Outputs for IfAction SubSystem: '<S301>/Hold this value' incorporates:
    //   ActionPort: '<S302>/Action Port'

    localB->In1 = rtb_Sum6_hb;

    // End of Outputs for SubSystem: '<S301>/Hold this value'
  }

  // End of If: '<S301>/if we went through a "step"'

  // Sum: '<S292>/Sum3' incorporates:
  //   Gain: '<S292>/kd_yb'
  //   Gain: '<S292>/kp_yb'
  //   Gain: '<S301>/divide by delta T'

  *rtd_BLACK_Fy = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1 *
    Template_v3_08_2020a_P.Kd_yb + Template_v3_08_2020a_P.Kp_yb * rtb_Sum6_dm;

  // MATLAB Function: '<S295>/MATLAB Function' incorporates:
  //   Constant: '<S289>/Desired Rate (BLACK)'
  //   Constant: '<S294>/Constant1'
  //   Product: '<S289>/Product'
  //   Sum: '<S294>/Subtract'

  Template_v_MATLABFunction_o(localP->DesiredRateBLACK_Value * (*rtd_Univ_Time -
    Template_v3_08_2020a_P.Phase2_End), &localB->sf_MATLABFunction);

  // MATLAB Function: '<S293>/MATLAB Function2'
  Template__MATLABFunction2_l(localB->sf_MATLABFunction.wrappedAngle,
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S293>/MATLAB Function3'
  Template_v3_MATLABFunction3(*rtd_BLACK_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S293>/Subtract2'
  localB->rtb_Subtract2_d_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_d_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S293>/MATLAB Function4'
  Template_v3_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_d_m, &localB->sf_MATLABFunction4);

  // Delay: '<S297>/Delay1'
  if (localDW->icLoad_pd != 0) {
    localDW->Delay1_DSTATE_m = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S297>/Sum6' incorporates:
  //   Delay: '<S297>/Delay1'

  rtb_Sum6_hb = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_m;

  // If: '<S297>/if we went through a "step"' incorporates:
  //   Inport: '<S298>/In1'

  if (rtb_Sum6_hb != 0.0) {
    // Outputs for IfAction SubSystem: '<S297>/Hold this value' incorporates:
    //   ActionPort: '<S298>/Action Port'

    localB->In1_j = rtb_Sum6_hb;

    // End of Outputs for SubSystem: '<S297>/Hold this value'
  }

  // End of If: '<S297>/if we went through a "step"'

  // Sum: '<S290>/Sum3' incorporates:
  //   Gain: '<S290>/kd_tb'
  //   Gain: '<S290>/kp_tb'
  //   Gain: '<S297>/divide by delta T'

  *rtd_BLACK_Tz = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_j *
    Template_v3_08_2020a_P.Kd_tb + Template_v3_08_2020a_P.Kp_tb *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S284>/Data Store Write3' incorporates:
  //   Constant: '<S284>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S299>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_cv;

  // Update for Delay: '<S301>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_j = rtb_Sum6_dm;

  // Update for Delay: '<S297>/Delay1'
  localDW->icLoad_pd = 0U;
  localDW->Delay1_DSTATE_m = localB->sf_MATLABFunction4.e_out;
}

//
// System initialize for action system:
//    '<S281>/Sub-Phase #1'
//    '<S281>/Sub-Phase #2 '
//    '<S281>/Sub-Phase #3 '
//    '<S281>/Sub-Phase #4'
//
void Template_v_SubPhase1_a_Init(B_SubPhase1_Template_v3_08__g_T *localB,
  DW_SubPhase1_Template_v3_08_l_T *localDW, P_SubPhase1_Template_v3_08__k_T
  *localP)
{
  // InitializeConditions for Delay: '<S372>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S374>/Delay1'
  localDW->icLoad_p = 1U;

  // InitializeConditions for Delay: '<S370>/Delay1'
  localDW->icLoad_m = 1U;

  // SystemInitialize for IfAction SubSystem: '<S372>/Hold this value'
  // SystemInitialize for Outport: '<S373>/Out1'
  localB->In1_j = localP->Out1_Y0_g;

  // End of SystemInitialize for SubSystem: '<S372>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S374>/Hold this value'
  // SystemInitialize for Outport: '<S375>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S374>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S370>/Hold this value'
  // SystemInitialize for Outport: '<S371>/Out1'
  localB->In1_m = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S370>/Hold this value'
}

//
// Output and update for action system:
//    '<S281>/Sub-Phase #1'
//    '<S281>/Sub-Phase #2 '
//    '<S281>/Sub-Phase #3 '
//    '<S281>/Sub-Phase #4'
//
void Template_v3_08__SubPhase1_a(B_SubPhase1_Template_v3_08__g_T *localB,
  DW_SubPhase1_Template_v3_08_l_T *localDW, P_SubPhase1_Template_v3_08__k_T
  *localP, real_T *rtd_BLACK_Px, real_T *rtd_BLACK_Py, real_T *rtd_BLUE_Fx,
  real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T *rtd_BLUE_Py, real_T
  *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T *rtd_Float_State, real_T
  *rtd_Univ_Time)
{
  real_T rtb_Subtract_m;
  real_T rtb_Sum6_b;
  real_T rtb_Subtract3_f;

  // Sum: '<S369>/Subtract3' incorporates:
  //   Constant: '<S369>/Constant2'

  rtb_Subtract3_f = *rtd_Univ_Time - Template_v3_08_2020a_P.Phase2_End;

  // Sum: '<S357>/Subtract' incorporates:
  //   MATLAB Function: '<S357>/MATLAB Function2'

  // MATLAB Function 'Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function2': '<S366>:1' 
  // '<S366>:1:3' x = -0.85*cos((0.03490659)*t) + xLength/2;
  rtb_Subtract_m = (cos(0.03490659 * rtb_Subtract3_f) * -0.85 +
                    Template_v3_08_2020a_P.xLength / 2.0) - *rtd_BLUE_Px;

  // Delay: '<S372>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_m;
  }

  // Sum: '<S372>/Sum6' incorporates:
  //   Delay: '<S372>/Delay1'

  rtb_Sum6_b = rtb_Subtract_m - localDW->Delay1_DSTATE;

  // If: '<S372>/if we went through a "step"' incorporates:
  //   Inport: '<S373>/In1'

  if (rtb_Sum6_b != 0.0) {
    // Outputs for IfAction SubSystem: '<S372>/Hold this value' incorporates:
    //   ActionPort: '<S373>/Action Port'

    localB->In1_j = rtb_Sum6_b;

    // End of Outputs for SubSystem: '<S372>/Hold this value'
  }

  // End of If: '<S372>/if we went through a "step"'

  // Sum: '<S363>/Sum3' incorporates:
  //   Gain: '<S363>/kd_xb'
  //   Gain: '<S363>/kp_xb'
  //   Gain: '<S372>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_j *
    Template_v3_08_2020a_P.Kd_xblue + Template_v3_08_2020a_P.Kp_xblue *
    rtb_Subtract_m;

  // Sum: '<S357>/Subtract1' incorporates:
  //   MATLAB Function: '<S357>/MATLAB Function3'

  // MATLAB Function 'Phase #3: Experiment/Change BLUE Behavior/Sub-Phase #1/MATLAB Function3': '<S367>:1' 
  // '<S367>:1:3' y = -0.85*sin((0.03490659)*t) + yLength/2;
  rtb_Subtract3_f = (sin(0.03490659 * rtb_Subtract3_f) * -0.85 +
                     Template_v3_08_2020a_P.yLength / 2.0) - *rtd_BLUE_Py;

  // Delay: '<S374>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_j = rtb_Subtract3_f;
  }

  // Sum: '<S374>/Sum6' incorporates:
  //   Delay: '<S374>/Delay1'

  rtb_Sum6_b = rtb_Subtract3_f - localDW->Delay1_DSTATE_j;

  // If: '<S374>/if we went through a "step"' incorporates:
  //   Inport: '<S375>/In1'

  if (rtb_Sum6_b != 0.0) {
    // Outputs for IfAction SubSystem: '<S374>/Hold this value' incorporates:
    //   ActionPort: '<S375>/Action Port'

    localB->In1 = rtb_Sum6_b;

    // End of Outputs for SubSystem: '<S374>/Hold this value'
  }

  // End of If: '<S374>/if we went through a "step"'

  // Sum: '<S364>/Sum3' incorporates:
  //   Gain: '<S364>/kd_yb'
  //   Gain: '<S364>/kp_yb'
  //   Gain: '<S374>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1 *
    Template_v3_08_2020a_P.Kd_yblue + Template_v3_08_2020a_P.Kp_yblue *
    rtb_Subtract3_f;

  // MATLAB Function: '<S365>/MATLAB Function2' incorporates:
  //   Constant: '<S368>/Constant'
  //   Sum: '<S368>/Subtract'
  //   Sum: '<S368>/Subtract1'
  //   Sum: '<S368>/Sum'
  //   Trigonometry: '<S368>/Atan2'

  Template__MATLABFunction2_l(rt_atan2d_snf(*rtd_BLACK_Py - *rtd_BLUE_Py,
    *rtd_BLACK_Px - *rtd_BLUE_Px) + localP->Constant_Value,
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S365>/MATLAB Function3'
  Template_v3_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S365>/Subtract2'
  localB->rtb_Subtract2_e2_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_e2_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S365>/MATLAB Function4'
  Template_v3_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_e2_m, &localB->sf_MATLABFunction4);

  // Delay: '<S370>/Delay1'
  if (localDW->icLoad_m != 0) {
    localDW->Delay1_DSTATE_o = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S370>/Sum6' incorporates:
  //   Delay: '<S370>/Delay1'

  rtb_Sum6_b = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_o;

  // If: '<S370>/if we went through a "step"' incorporates:
  //   Inport: '<S371>/In1'

  if (rtb_Sum6_b != 0.0) {
    // Outputs for IfAction SubSystem: '<S370>/Hold this value' incorporates:
    //   ActionPort: '<S371>/Action Port'

    localB->In1_m = rtb_Sum6_b;

    // End of Outputs for SubSystem: '<S370>/Hold this value'
  }

  // End of If: '<S370>/if we went through a "step"'

  // Sum: '<S362>/Sum3' incorporates:
  //   Gain: '<S362>/kd_tb'
  //   Gain: '<S362>/kp_tb'
  //   Gain: '<S370>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_m *
    Template_v3_08_2020a_P.Kd_tblue + Template_v3_08_2020a_P.Kp_tblue *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S357>/Data Store Write1' incorporates:
  //   Constant: '<S357>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S372>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_m;

  // Update for Delay: '<S374>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_j = rtb_Subtract3_f;

  // Update for Delay: '<S370>/Delay1'
  localDW->icLoad_m = 0U;
  localDW->Delay1_DSTATE_o = localB->sf_MATLABFunction4.e_out;
}

//
// Output and update for atomic system:
//    '<S434>/MATLAB Function'
//    '<S437>/MATLAB Function'
//
void Template_v_MATLABFunction_e(real_T rtu_t, B_MATLABFunction_Template_v_j_T
  *localB)
{
  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/MATLAB Function': '<S443>:1' 
  // '<S443>:1:3' x = 0.85*cos((0.03490659)*t) + xLength/2;
  localB->x = cos(0.03490659 * rtu_t) * 0.85 + Template_v3_08_2020a_P.xLength /
    2.0;
}

//
// Output and update for atomic system:
//    '<S434>/MATLAB Function1'
//    '<S435>/MATLAB Function1'
//    '<S436>/MATLAB Function1'
//    '<S437>/MATLAB Function1'
//
void Template_v3_MATLABFunction1(real_T rtu_t, B_MATLABFunction1_Template_v3_T
  *localB)
{
  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #1/MATLAB Function1': '<S444>:1' 
  // '<S444>:1:3' y = 0.85*sin((0.03490659)*t) + yLength/2;
  localB->y = sin(0.03490659 * rtu_t) * 0.85 + Template_v3_08_2020a_P.yLength /
    2.0;
}

//
// Output and update for atomic system:
//    '<S435>/MATLAB Function'
//    '<S436>/MATLAB Function'
//
void Template_v_MATLABFunction_g(real_T rtu_t, B_MATLABFunction_Template_v_h_T
  *localB)
{
  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #2 /MATLAB Function': '<S476>:1' 
  // '<S476>:1:3' x = 0.9*cos((0.03490659)*t) + xLength/2;
  localB->x = cos(0.03490659 * rtu_t) * 0.9 + Template_v3_08_2020a_P.xLength /
    2.0;
}

//
// Output and update for action system:
//    '<S437>/RED+ARM'
//    '<S554>/RED+ARM'
//    '<S607>/RED+ARM'
//
void Template_v3_08_202_REDARM_a(B_REDARM_Template_v3_08_202_m_T *localB,
  P_REDARM_Template_v3_08_202_a_T *localP, real_T *rtd_RED_RzD_Elbow, real_T
  *rtd_RED_RzD_Shoulder, real_T *rtd_RED_RzD_Wrist, real_T *rtd_RED_Rz_Elbow,
  real_T *rtd_RED_Rz_Shoulder, real_T *rtd_RED_Rz_Wrist, real_T
  *rtd_RED_Tz_Elbow, real_T *rtd_RED_Tz_Shoulder, real_T *rtd_RED_Tz_Wrist)
{
  real_T t114;
  real_T rtb_y_a_tmp;
  real_T rtb_y_a_tmp_0;
  int32_T i;
  int32_T i_0;
  real_T l1_tmp;
  real_T l2_tmp;
  real_T l3_tmp;
  int32_T tmp;

  // MATLAB Function: '<S543>/End-Effector Position' incorporates:
  //   MATLAB Function: '<S547>/MATLAB Function'

  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/End-Effector Position': '<S545>:1' 
  // '<S545>:1:4' phi= model_param(7);
  // '<S545>:1:5' b0= model_param(8);
  // '<S545>:1:6' a1= model_param(9);
  // '<S545>:1:7' b1= model_param(10);
  // '<S545>:1:8' a2= model_param(11);
  // '<S545>:1:9' b2= model_param(12);
  // '<S545>:1:10' a3= model_param(13);
  // '<S545>:1:11' b3= model_param(14);
  // '<S545>:1:13' l1     = a1+b1;
  l1_tmp = Template_v3_08_2020a_P.model_param[8] +
    Template_v3_08_2020a_P.model_param[9];

  // '<S545>:1:14' l2     = a2+b2;
  l2_tmp = Template_v3_08_2020a_P.model_param[10] +
    Template_v3_08_2020a_P.model_param[11];

  // '<S545>:1:15' l3     = a3+b3;
  l3_tmp = Template_v3_08_2020a_P.model_param[12] +
    Template_v3_08_2020a_P.model_param[13];

  // '<S545>:1:18' x0 = q(1);
  // '<S545>:1:19' y0 = q(2);
  // '<S545>:1:20' q0 = q(3);
  // '<S545>:1:21' q1 = q(4);
  // '<S545>:1:22' q2 = q(5);
  // '<S545>:1:23' q3 = q(6);
  // '<S545>:1:26' y = [ (b0)*cos(phi) + (l1)*cos(pi/2+q1) + (l2)*cos(pi/2+q1+q2) + (l3)*cos(pi/2+q1+q2+q3),... 
  // '<S545>:1:27'       (b0)*sin(phi) + (l1)*sin(pi/2+q1) + (l2)*sin(pi/2+q1+q2) + (l3)*sin(pi/2+q1+q2+q3)]; 
  rtb_y_a_tmp = sin(Template_v3_08_2020a_P.model_param[6]);
  rtb_y_a_tmp_0 = cos(Template_v3_08_2020a_P.model_param[6]);

  // MATLAB Function: '<S547>/Inertia Function '
  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /Inertia Function ': '<S549>:1' 
  // '<S549>:1:8' q0 = x(3);
  // '<S549>:1:9' q1 = x(4);
  // '<S549>:1:10' q2 = x(5);
  // '<S549>:1:11' q3 = x(6);
  // '<S549>:1:13' m0= model_param(1);
  // '<S549>:1:14' I0= model_param(2);
  // '<S549>:1:15' phi= model_param(7);
  // '<S549>:1:16' b0= model_param(8);
  // '<S549>:1:17' a1= model_param(9);
  // '<S549>:1:18' b1= model_param(10);
  // '<S549>:1:19' a2= model_param(11);
  // '<S549>:1:20' b2= model_param(12);
  // '<S549>:1:21' a3= model_param(13);
  // '<S549>:1:22' b3= model_param(14);
  // '<S549>:1:23' m1= model_param(15);
  // '<S549>:1:24' m2= model_param(16);
  // '<S549>:1:25' m3= model_param(17);
  // '<S549>:1:26' I1= model_param(18);
  // '<S549>:1:27' I2= model_param(19);
  // '<S549>:1:28' I3= model_param(20);
  // '<S549>:1:30' t2 = cos(q2);
  localB->t2 = cos(*rtd_RED_Rz_Elbow);

  // '<S549>:1:31' t3 = cos(q3);
  localB->t3 = cos(*rtd_RED_Rz_Wrist);

  // '<S549>:1:32' t4 = a1+b1;
  // '<S549>:1:33' t5 = a2+b2;
  // '<S549>:1:34' t6 = a1.*m1;
  localB->t6 = Template_v3_08_2020a_P.model_param[8] *
    Template_v3_08_2020a_P.model_param[14];

  // '<S549>:1:35' t7 = a2.*m2;
  localB->t7 = Template_v3_08_2020a_P.model_param[10] *
    Template_v3_08_2020a_P.model_param[15];

  // '<S549>:1:36' t8 = a2.*m3;
  localB->t8 = Template_v3_08_2020a_P.model_param[10] *
    Template_v3_08_2020a_P.model_param[16];

  // '<S549>:1:37' t9 = a3.*m3;
  localB->t9 = Template_v3_08_2020a_P.model_param[12] *
    Template_v3_08_2020a_P.model_param[16];

  // '<S549>:1:38' t10 = b0.*m1;
  // '<S549>:1:39' t11 = b0.*m2;
  localB->t11 = Template_v3_08_2020a_P.model_param[7] *
    Template_v3_08_2020a_P.model_param[15];

  // '<S549>:1:40' t12 = b0.*m3;
  localB->t12 = Template_v3_08_2020a_P.model_param[7] *
    Template_v3_08_2020a_P.model_param[16];

  // '<S549>:1:41' t13 = b2.*m3;
  localB->t13 = Template_v3_08_2020a_P.model_param[11] *
    Template_v3_08_2020a_P.model_param[16];

  // '<S549>:1:42' t14 = phi+q0;
  // '<S549>:1:43' t15 = q0+q1;
  // '<S549>:1:44' t16 = q2+q3;
  localB->t16 = *rtd_RED_Rz_Elbow + *rtd_RED_Rz_Wrist;

  // '<S549>:1:45' t17 = a1.^2;
  localB->t17 = Template_v3_08_2020a_P.model_param[8] *
    Template_v3_08_2020a_P.model_param[8];

  // '<S549>:1:46' t18 = a2.^2;
  // '<S549>:1:47' t19 = a3.^2;
  // '<S549>:1:48' t20 = b0.^2;
  // '<S549>:1:49' t21 = b1.^2;
  localB->t21 = Template_v3_08_2020a_P.model_param[9] *
    Template_v3_08_2020a_P.model_param[9];

  // '<S549>:1:50' t22 = b2.^2;
  // '<S549>:1:51' t48 = -phi;
  // '<S549>:1:52' t49 = -q1;
  // '<S549>:1:53' t50 = m0+m1+m2+m3;
  // '<S549>:1:54' t76 = a1.*b1.*m2.*2.0;
  // '<S549>:1:55' t77 = a1.*b1.*m3.*2.0;
  // '<S549>:1:56' t23 = a1.*t7;
  localB->t23 = Template_v3_08_2020a_P.model_param[8] * localB->t7;

  // '<S549>:1:57' t24 = a1.*t8;
  localB->t24 = Template_v3_08_2020a_P.model_param[8] * localB->t8;

  // '<S549>:1:58' t25 = a1.*t9;
  localB->t25 = Template_v3_08_2020a_P.model_param[8] * localB->t9;

  // '<S549>:1:59' t26 = a3.*t8;
  localB->t26 = Template_v3_08_2020a_P.model_param[12] * localB->t8;

  // '<S549>:1:60' t27 = b0.*t6;
  // '<S549>:1:61' t28 = a1.*t11;
  // '<S549>:1:62' t29 = a1.*t12;
  // '<S549>:1:63' t30 = b0.*t7;
  localB->t30_c = Template_v3_08_2020a_P.model_param[7] * localB->t7;

  // '<S549>:1:64' t31 = b0.*t8;
  localB->t31 = Template_v3_08_2020a_P.model_param[7] * localB->t8;

  // '<S549>:1:65' t32 = b1.*t7;
  localB->t32 = Template_v3_08_2020a_P.model_param[9] * localB->t7;

  // '<S549>:1:66' t33 = a1.*t13;
  localB->t33 = Template_v3_08_2020a_P.model_param[8] * localB->t13;

  // '<S549>:1:67' t34 = b1.*t8;
  localB->t34 = Template_v3_08_2020a_P.model_param[9] * localB->t8;

  // '<S549>:1:68' t35 = b0.*t9;
  localB->t35 = Template_v3_08_2020a_P.model_param[7] * localB->t9;

  // '<S549>:1:69' t36 = b1.*t9;
  localB->t36 = Template_v3_08_2020a_P.model_param[9] * localB->t9;

  // '<S549>:1:70' t37 = b2.*t9;
  localB->t37 = Template_v3_08_2020a_P.model_param[11] * localB->t9;

  // '<S549>:1:71' t38 = b1.*t11;
  // '<S549>:1:72' t39 = b1.*t12;
  // '<S549>:1:73' t40 = b2.*t12;
  localB->t40 = Template_v3_08_2020a_P.model_param[11] * localB->t12;

  // '<S549>:1:74' t41 = b1.*t13;
  localB->t41 = Template_v3_08_2020a_P.model_param[9] * localB->t13;

  // '<S549>:1:75' t42 = cos(t14);
  // '<S549>:1:76' t43 = cos(t15);
  // '<S549>:1:77' t44 = cos(t16);
  localB->t44 = cos(localB->t16);

  // '<S549>:1:78' t45 = q2+t15;
  // '<S549>:1:79' t46 = sin(t14);
  // '<S549>:1:80' t47 = sin(t15);
  // '<S549>:1:81' t52 = t15+t16;
  // '<S549>:1:82' t54 = -t6;
  // '<S549>:1:83' t55 = -t7;
  // '<S549>:1:84' t56 = -t8;
  // '<S549>:1:85' t57 = -t9;
  // '<S549>:1:86' t58 = -t10;
  // '<S549>:1:87' t59 = -t11;
  // '<S549>:1:88' t60 = -t12;
  // '<S549>:1:89' t61 = -t13;
  // '<S549>:1:90' t62 = a1.*t6;
  // '<S549>:1:91' t63 = m2.*t17;
  // '<S549>:1:92' t64 = m3.*t17;
  // '<S549>:1:93' t65 = a2.*t7;
  // '<S549>:1:94' t66 = a2.*t8;
  // '<S549>:1:95' t67 = a3.*t9;
  // '<S549>:1:96' t69 = m2.*t21;
  // '<S549>:1:97' t70 = m3.*t21;
  // '<S549>:1:98' t71 = b2.*t13;
  // '<S549>:1:99' t81 = b2.*t8.*2.0;
  // '<S549>:1:100' t85 = m2.*t4;
  // '<S549>:1:101' t86 = m3.*t4;
  // '<S549>:1:102' t87 = m3.*t5;
  // '<S549>:1:103' t94 = phi+t49;
  // '<S549>:1:104' t95 = q1+q2+t48;
  // '<S549>:1:105' t101 = q1+t16+t48;
  // '<S549>:1:106' t106 = t10+t11+t12;
  // '<S549>:1:107' t51 = cos(t45);
  // '<S549>:1:108' t53 = sin(t45);
  // '<S549>:1:109' t68 = t67;
  // '<S549>:1:110' t72 = t23.*2.0;
  // '<S549>:1:111' t73 = t24.*2.0;
  // '<S549>:1:112' t74 = t25.*2.0;
  // '<S549>:1:113' t75 = t26.*2.0;
  // '<S549>:1:114' t78 = t32.*2.0;
  // '<S549>:1:115' t79 = t33.*2.0;
  // '<S549>:1:116' t80 = t34.*2.0;
  // '<S549>:1:117' t82 = t36.*2.0;
  // '<S549>:1:118' t83 = t37.*2.0;
  // '<S549>:1:119' t84 = t41.*2.0;
  // '<S549>:1:120' t88 = cos(t52);
  // '<S549>:1:121' t89 = -t30;
  // '<S549>:1:122' t90 = -t31;
  // '<S549>:1:123' t91 = -t35;
  // '<S549>:1:124' t92 = -t40;
  // '<S549>:1:125' t93 = sin(t52);
  // '<S549>:1:126' t96 = sin(t94);
  // '<S549>:1:127' t98 = -t85;
  // '<S549>:1:128' t99 = -t86;
  // '<S549>:1:129' t100 = -t87;
  // '<S549>:1:130' t102 = sin(t95);
  localB->t45 = sin((*rtd_RED_Rz_Shoulder + *rtd_RED_Rz_Elbow) +
                    -Template_v3_08_2020a_P.model_param[6]);

  // '<S549>:1:131' t103 = t25+t36;
  // '<S549>:1:132' t104 = t26+t37;
  // '<S549>:1:133' t105 = sin(t101);
  localB->t16 = sin((*rtd_RED_Rz_Shoulder + localB->t16) +
                    -Template_v3_08_2020a_P.model_param[6]);

  // '<S549>:1:134' t115 = t42.*t106;
  // '<S549>:1:135' t118 = t55+t56+t61;
  // '<S549>:1:136' t119 = t58+t59+t60;
  // '<S549>:1:137' t125 = t27+t28+t29+t38+t39;
  // '<S549>:1:138' t126 = -t46.*t106;
  // '<S549>:1:139' t129 = -t43.*(t6+t85+t86);
  // '<S549>:1:140' t130 = -t47.*(t6+t85+t86);
  // '<S549>:1:141' t131 = t23+t24+t32+t33+t34+t41;
  // '<S549>:1:142' t97 = I3+t68;
  localB->t4 = Template_v3_08_2020a_P.model_param[12] * localB->t9 +
    Template_v3_08_2020a_P.model_param[19];

  // '<S549>:1:143' t107 = t74+t82;
  // '<S549>:1:144' t108 = t75+t83;
  // '<S549>:1:145' t109 = t57.*t88;
  // '<S549>:1:146' t110 = t57.*t93;
  // '<S549>:1:147' t111 = t3.*t104;
  // '<S549>:1:148' t112 = t55+t100;
  // '<S549>:1:149' t113 = t44.*t103;
  localB->t9 = (localB->t25 + localB->t36) * localB->t44;

  // '<S549>:1:150' t117 = t91.*t105;
  localB->t117 = -localB->t35 * localB->t16;

  // '<S549>:1:151' t120 = -t53.*(t7+t87);
  // '<S549>:1:152' t121 = t89+t90+t92;
  // '<S549>:1:153' t122 = -t51.*(t7+t87);
  // '<S549>:1:154' t124 = t54+t98+t99;
  // '<S549>:1:155' t127 = -t51.*(t7+t8+t13);
  // '<S549>:1:156' t128 = -t53.*(t7+t8+t13);
  // '<S549>:1:157' t133 = t2.*t131;
  // '<S549>:1:158' t134 = -t102.*(t30+t31+t40);
  localB->t134 = ((localB->t30_c + localB->t31) + localB->t40) * -localB->t45;

  // '<S549>:1:159' t135 = t96.*t125;
  localB->t52 = ((((Template_v3_08_2020a_P.model_param[7] * localB->t6 +
                    Template_v3_08_2020a_P.model_param[8] * localB->t11) +
                   Template_v3_08_2020a_P.model_param[8] * localB->t12) +
                  Template_v3_08_2020a_P.model_param[9] * localB->t11) +
                 Template_v3_08_2020a_P.model_param[9] * localB->t12) * sin
    (Template_v3_08_2020a_P.model_param[6] + -*rtd_RED_Rz_Shoulder);

  // '<S549>:1:160' t136 = t72+t73+t78+t79+t80+t84;
  // '<S549>:1:161' t114 = t3.*t108;
  t114 = (localB->t26 * 2.0 + localB->t37 * 2.0) * localB->t3;

  // '<S549>:1:162' t116 = t44.*t107;
  localB->t25 = (localB->t25 * 2.0 + localB->t36 * 2.0) * localB->t44;

  // '<S549>:1:163' t123 = t97+t111;
  localB->t3 = (localB->t26 + localB->t37) * localB->t3 + localB->t4;

  // '<S549>:1:164' t132 = I2+t65+t66+t71+t81+t97;
  localB->t26 = ((((Template_v3_08_2020a_P.model_param[10] * localB->t7 +
                    Template_v3_08_2020a_P.model_param[18]) +
                   Template_v3_08_2020a_P.model_param[10] * localB->t8) +
                  Template_v3_08_2020a_P.model_param[11] * localB->t13) +
                 Template_v3_08_2020a_P.model_param[11] * localB->t8 * 2.0) +
    localB->t4;

  // '<S549>:1:165' t138 = t2.*t136;
  localB->t37 = (((((localB->t23 * 2.0 + localB->t24 * 2.0) + localB->t32 * 2.0)
                   + localB->t33 * 2.0) + localB->t34 * 2.0) + localB->t41 * 2.0)
    * localB->t2;

  // '<S549>:1:166' t139 = t109+t127;
  // '<S549>:1:167' t140 = t110+t128;
  // '<S549>:1:168' t142 = t110+t120+t130;
  // '<S549>:1:169' t143 = t109+t122+t129;
  // '<S549>:1:170' t137 = t113+t123;
  // '<S549>:1:171' t144 = I1+t62+t63+t64+t69+t70+t76+t77+t132;
  localB->t7 = Template_v3_08_2020a_P.model_param[8] *
    Template_v3_08_2020a_P.model_param[9];
  localB->t6 = (((((((Template_v3_08_2020a_P.model_param[8] * localB->t6 +
                      Template_v3_08_2020a_P.model_param[17]) +
                     Template_v3_08_2020a_P.model_param[15] * localB->t17) +
                    Template_v3_08_2020a_P.model_param[16] * localB->t17) +
                   Template_v3_08_2020a_P.model_param[15] * localB->t21) +
                  Template_v3_08_2020a_P.model_param[16] * localB->t21) +
                 localB->t7 * Template_v3_08_2020a_P.model_param[15] * 2.0) +
                localB->t7 * Template_v3_08_2020a_P.model_param[16] * 2.0) +
    localB->t26;

  // '<S549>:1:172' t145 = t115+t142;
  // '<S549>:1:173' t146 = t126+t143;
  // '<S549>:1:174' t147 = t113+t114+t132+t133;
  localB->t2 = (((((localB->t23 + localB->t24) + localB->t32) + localB->t33) +
                 localB->t34) + localB->t41) * localB->t2 + ((localB->t9 + t114)
    + localB->t26);

  // '<S549>:1:175' t141 = t117+t137;
  // '<S549>:1:176' t148 = t117+t134+t147;
  // '<S549>:1:177' t149 = t114+t116+t117+t134+t135+t138+t144;
  // '<S549>:1:178' InertiaS = reshape([t50,0.0,t146,t143,t139,t109,0.0,t50,t145,t142,t140,t110,t146,t145,I0+t114+t116+t135.*2.0+t138+t144+b0.*t10+b0.*t11+b0.*t12-t35.*t105.*2.0-t102.*(t30.*2.0+t31.*2.0+t40.*2.0),t149,t148,t141,t143,t142,t149,t114+t116+t138+t144,t147,t137,t139,t140,t148,t147,t114+t132,t123,t109,t110,t141,t137,t123,t97],[6,6]); 
  localB->t21 = (((((((((Template_v3_08_2020a_P.model_param[1] + t114) +
                        localB->t25) + localB->t52 * 2.0) + localB->t37) +
                     localB->t6) + Template_v3_08_2020a_P.model_param[7] *
                    Template_v3_08_2020a_P.model_param[14] *
                    Template_v3_08_2020a_P.model_param[7]) +
                   Template_v3_08_2020a_P.model_param[7] * localB->t11) +
                  Template_v3_08_2020a_P.model_param[7] * localB->t12) -
                 localB->t35 * localB->t16 * 2.0) - ((localB->t30_c * 2.0 +
    localB->t31 * 2.0) + localB->t40 * 2.0) * localB->t45;

  // MATLAB Function: '<S547>/MATLAB Function' incorporates:
  //   MATLAB Function: '<S543>/End-Effector Position'
  //   MATLAB Function: '<S547>/Inertia Function '

  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Transpose Generalized Jacobian /MATLAB Function': '<S550>:1' 
  // '<S550>:1:5' q0     = q(3);
  // '<S550>:1:6' q1     = q(4);
  // '<S550>:1:7' q2     = q(5);
  // '<S550>:1:8' q3     = q(6);
  // '<S550>:1:10' m0= model_param(1);
  // '<S550>:1:11' I0= model_param(2);
  // '<S550>:1:12' phi= model_param(7);
  // '<S550>:1:13' b0= model_param(8);
  // '<S550>:1:14' a1= model_param(9);
  // '<S550>:1:15' b1= model_param(10);
  // '<S550>:1:16' a2= model_param(11);
  // '<S550>:1:17' b2= model_param(12);
  // '<S550>:1:18' a3= model_param(13);
  // '<S550>:1:19' b3= model_param(14);
  // '<S550>:1:20' m1= model_param(15);
  // '<S550>:1:21' m2= model_param(16);
  // '<S550>:1:22' m3= model_param(17);
  // '<S550>:1:23' I1= model_param(18);
  // '<S550>:1:24' I2= model_param(19);
  // '<S550>:1:25' I3= model_param(20);
  // '<S550>:1:26' l1     = a1+b1;
  // '<S550>:1:27' l2     = a2+b2;
  // '<S550>:1:28' l3     = a3+b3;
  // '<S550>:1:30' w = m0+m1+m2+m3;
  localB->t31 = Template_v3_08_2020a_P.model_param[0] +
    Template_v3_08_2020a_P.model_param[14];
  localB->t11 = localB->t31 + Template_v3_08_2020a_P.model_param[15];
  localB->t35 = localB->t11 + Template_v3_08_2020a_P.model_param[16];

  // '<S550>:1:36' K0     = m0*b0/w;
  localB->t26 = Template_v3_08_2020a_P.model_param[0] *
    Template_v3_08_2020a_P.model_param[7] / localB->t35;

  // '<S550>:1:37' K1     = (m0*l1 + m1*b1)/w;
  localB->t30_c = (Template_v3_08_2020a_P.model_param[0] * l1_tmp +
                   Template_v3_08_2020a_P.model_param[14] *
                   Template_v3_08_2020a_P.model_param[9]) / localB->t35;

  // '<S550>:1:38' K2     = ((m0+m1)*l2 + m2*b2)/w;
  localB->t31 = (localB->t31 * l2_tmp + Template_v3_08_2020a_P.model_param[15] *
                 Template_v3_08_2020a_P.model_param[11]) / localB->t35;

  // '<S550>:1:39' K3     = ((m0+m1+m2)*l3 + m3*b3)/w;
  localB->t35 = (localB->t11 * l3_tmp + Template_v3_08_2020a_P.model_param[16] *
                 Template_v3_08_2020a_P.model_param[13]) / localB->t35;

  // '<S550>:1:42' s0     = sin(phi);
  // '<S550>:1:43' s1     = sin(pi/2+q1);
  localB->t11 = sin(*rtd_RED_Rz_Shoulder + 1.5707963267948966);

  // '<S550>:1:44' s2     = sin(pi/2+q1+q2);
  localB->t23 = (*rtd_RED_Rz_Shoulder + 1.5707963267948966) + *rtd_RED_Rz_Elbow;
  localB->t12 = sin(localB->t23);

  // '<S550>:1:45' s3     = sin(pi/2+q1+q2+q3);
  localB->t24 = localB->t23 + *rtd_RED_Rz_Wrist;
  localB->t16 = sin(localB->t24);

  // '<S550>:1:46' c0     = cos(phi);
  // '<S550>:1:47' c1     = cos(pi/2+q1);
  localB->t17 = cos(*rtd_RED_Rz_Shoulder + 1.5707963267948966);

  // '<S550>:1:48' c2     = cos(pi/2+q1+q2);
  localB->t23 = cos(localB->t23);

  // '<S550>:1:49' c3     = cos(pi/2+q1+q2+q3);
  localB->t24 = cos(localB->t24);

  // '<S550>:1:52' Jm     = [ -K1*s1-K2*s2-K3*s3 -K2*s2-K3*s3 -K3*s3
  // '<S550>:1:53'             K1*c1+K2*c2+K3*c3  K2*c2+K3*c3  K3*c3  ];
  // '<S550>:1:56' Js     = [ -K0*b0*s0-K1*l1*s1-K2*l2*s2-K3*l3*s3
  // '<S550>:1:57'             K0*b0*c0+K1*l1*c1+K2*l2*c2+K3*l3*c3 ];
  // '<S550>:1:60' H      = InertiaS;
  // '<S550>:1:62' Is     = H(3,3);
  // '<S550>:1:63' Im     = [H(3,4), H(3,5), H(3,6)];
  // '<S550>:1:66' J      = Jm - Js*(Is\Im);
  // MATLAB Function 'Phase #3: Experiment/Change RED Behavior/Sub-Phase #4/RED+ARM/Transpose Jacobian Controller/Arm Controller/MATLAB Function': '<S548>:1' 
  // '<S548>:1:5' q1_d   = qdot(4);
  // '<S548>:1:6' q2_d   = qdot(5);
  // '<S548>:1:7' q3_d   = qdot(6);
  // '<S548>:1:10' TAU = Kp*Jt*dx' - Kv*[ q1_d
  // '<S548>:1:11'                        q2_d
  // '<S548>:1:12'                        q3_d];
  // '<S548>:1:14' TAU_out = [ 0
  // '<S548>:1:15'             0
  // '<S548>:1:16'             0
  // '<S548>:1:17'             TAU ];
  localB->t32 = localB->t30_c * l1_tmp;
  localB->t33 = localB->t31 * l2_tmp;
  localB->t34 = localB->t35 * l3_tmp;
  localB->t50_m[0] = ((-localB->t26 * Template_v3_08_2020a_P.model_param[7] *
                       rtb_y_a_tmp - localB->t32 * localB->t11) - localB->t33 *
                      localB->t12) - localB->t34 * localB->t16;
  localB->t50_m[1] = ((localB->t26 * Template_v3_08_2020a_P.model_param[7] *
                       rtb_y_a_tmp_0 + localB->t32 * localB->t17) + localB->t33 *
                      localB->t23) + localB->t34 * localB->t24;
  localB->t6 = ((((((t114 + localB->t25) + localB->t117) + localB->t134) +
                  localB->t52) + localB->t37) + localB->t6) / localB->t21;
  localB->t2 = ((localB->t117 + localB->t134) + localB->t2) / localB->t21;
  localB->t3 = ((localB->t9 + localB->t3) + localB->t117) / localB->t21;
  localB->t9 = localB->t35 * localB->t16;

  // Math: '<S547>/Math Function' incorporates:
  //   MATLAB Function: '<S547>/MATLAB Function'

  localB->t30[0] = (-localB->t30_c * localB->t11 - localB->t31 * localB->t12) -
    localB->t9;
  localB->t30[1] = -localB->t31 * localB->t12 - localB->t9;
  localB->t30[2] = -localB->t35 * localB->t16;

  // MATLAB Function: '<S547>/MATLAB Function'
  localB->t9 = localB->t31 * localB->t23;
  localB->t35 *= localB->t24;

  // Math: '<S547>/Math Function' incorporates:
  //   MATLAB Function: '<S547>/MATLAB Function'

  localB->t30[3] = (localB->t30_c * localB->t17 + localB->t9) + localB->t35;
  localB->t30[4] = localB->t9 + localB->t35;
  localB->t30[5] = localB->t35;
  for (i = 0; i < 2; i++) {
    localB->t50[3 * i] = localB->t50_m[i] * localB->t6;
    localB->t50[3 * i + 1] = localB->t50_m[i] * localB->t2;
    localB->t50[3 * i + 2] = localB->t50_m[i] * localB->t3;
  }

  // MATLAB Function: '<S547>/MATLAB Function'
  for (i = 0; i < 6; i++) {
    localB->TAU_out[i] = localB->t30[i] - localB->t50[i];
  }

  // MATLAB Function: '<S544>/MATLAB Function'
  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp = i_0 + 3 * i;
      localB->t30[tmp] = 0.0;
      localB->t30[tmp] += localB->TAU_out[3 * i] * Template_v3_08_2020a_P.Kp[i_0];
      localB->t30[tmp] += localB->TAU_out[3 * i + 1] *
        Template_v3_08_2020a_P.Kp[i_0 + 3];
      localB->t30[tmp] += localB->TAU_out[3 * i + 2] *
        Template_v3_08_2020a_P.Kp[i_0 + 6];
    }
  }

  // Sum: '<S546>/Subtract' incorporates:
  //   Constant: '<S531>/Constant'
  //   Constant: '<S531>/Constant2'
  //   MATLAB Function: '<S543>/End-Effector Position'

  rtb_y_a_tmp_0 = localP->Constant2_Value - (((localB->t17 * l1_tmp +
    Template_v3_08_2020a_P.model_param[7] * rtb_y_a_tmp_0) + localB->t23 *
    l2_tmp) + localB->t24 * l3_tmp);
  l1_tmp = localP->Constant_Value - (((localB->t11 * l1_tmp +
    Template_v3_08_2020a_P.model_param[7] * rtb_y_a_tmp) + localB->t12 * l2_tmp)
    + localB->t16 * l3_tmp);

  // MATLAB Function: '<S544>/MATLAB Function'
  for (i = 0; i < 3; i++) {
    localB->TAU_out[i + 3] = (localB->t30[i + 3] * l1_tmp + localB->t30[i] *
      rtb_y_a_tmp_0) - (Template_v3_08_2020a_P.Kv[i + 6] * *rtd_RED_RzD_Wrist +
                        (Template_v3_08_2020a_P.Kv[i + 3] * *rtd_RED_RzD_Elbow +
                         Template_v3_08_2020a_P.Kv[i] * *rtd_RED_RzD_Shoulder));
  }

  // Saturate: '<S531>/Saturation'
  if (localB->TAU_out[3] > Template_v3_08_2020a_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = Template_v3_08_2020a_P.Tz_lim_sharm;
  } else if (localB->TAU_out[3] < -Template_v3_08_2020a_P.Tz_lim_sharm) {
    *rtd_RED_Tz_Shoulder = -Template_v3_08_2020a_P.Tz_lim_sharm;
  } else {
    *rtd_RED_Tz_Shoulder = localB->TAU_out[3];
  }

  // End of Saturate: '<S531>/Saturation'

  // Saturate: '<S531>/Saturation1'
  if (localB->TAU_out[4] > Template_v3_08_2020a_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = Template_v3_08_2020a_P.Tz_lim_elarm;
  } else if (localB->TAU_out[4] < -Template_v3_08_2020a_P.Tz_lim_elarm) {
    *rtd_RED_Tz_Elbow = -Template_v3_08_2020a_P.Tz_lim_elarm;
  } else {
    *rtd_RED_Tz_Elbow = localB->TAU_out[4];
  }

  // End of Saturate: '<S531>/Saturation1'

  // Saturate: '<S531>/Saturation3'
  if (localB->TAU_out[5] > Template_v3_08_2020a_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = Template_v3_08_2020a_P.Tz_lim_wrarm;
  } else if (localB->TAU_out[5] < -Template_v3_08_2020a_P.Tz_lim_wrarm) {
    *rtd_RED_Tz_Wrist = -Template_v3_08_2020a_P.Tz_lim_wrarm;
  } else {
    *rtd_RED_Tz_Wrist = localB->TAU_out[5];
  }

  // End of Saturate: '<S531>/Saturation3'
}

//
// System initialize for action system:
//    '<S14>/Change BLUE Behavior'
//    '<S15>/Change BLUE Behavior'
//
void T_ChangeBLUEBehavior_c_Init(B_ChangeBLUEBehavior_Templa_k_T *localB,
  DW_ChangeBLUEBehavior_Templ_i_T *localDW, P_ChangeBLUEBehavior_Templa_c_T
  *localP)
{
  // InitializeConditions for Delay: '<S575>/Delay1'
  localDW->icLoad = 1U;

  // InitializeConditions for Delay: '<S577>/Delay1'
  localDW->icLoad_m = 1U;

  // InitializeConditions for Delay: '<S573>/Delay1'
  localDW->icLoad_p = 1U;

  // SystemInitialize for IfAction SubSystem: '<S575>/Hold this value'
  // SystemInitialize for Outport: '<S576>/Out1'
  localB->In1_l = localP->Out1_Y0_n;

  // End of SystemInitialize for SubSystem: '<S575>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S577>/Hold this value'
  // SystemInitialize for Outport: '<S578>/Out1'
  localB->In1 = localP->Out1_Y0_p;

  // End of SystemInitialize for SubSystem: '<S577>/Hold this value'

  // SystemInitialize for IfAction SubSystem: '<S573>/Hold this value'
  // SystemInitialize for Outport: '<S574>/Out1'
  localB->In1_p = localP->Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S573>/Hold this value'
}

//
// Output and update for action system:
//    '<S14>/Change BLUE Behavior'
//    '<S15>/Change BLUE Behavior'
//
void Templa_ChangeBLUEBehavior_g(B_ChangeBLUEBehavior_Templa_k_T *localB,
  DW_ChangeBLUEBehavior_Templ_i_T *localDW, P_ChangeBLUEBehavior_Templa_c_T
  *localP, real_T *rtd_BLUE_Fx, real_T *rtd_BLUE_Fy, real_T *rtd_BLUE_Px, real_T
  *rtd_BLUE_Py, real_T *rtd_BLUE_Rz, real_T *rtd_BLUE_Tz, real_T
  *rtd_Float_State)
{
  real_T rtb_Subtract_ho;
  real_T rtb_Sum6_dk;
  real_T rtb_Sum6_fj;

  // Sum: '<S553>/Subtract' incorporates:
  //   Constant: '<S553>/Desired Px (BLUE)'

  rtb_Subtract_ho = Template_v3_08_2020a_P.home_states_BLUE[0] - *rtd_BLUE_Px;

  // Delay: '<S575>/Delay1'
  if (localDW->icLoad != 0) {
    localDW->Delay1_DSTATE = rtb_Subtract_ho;
  }

  // Sum: '<S575>/Sum6' incorporates:
  //   Delay: '<S575>/Delay1'

  rtb_Sum6_dk = rtb_Subtract_ho - localDW->Delay1_DSTATE;

  // If: '<S575>/if we went through a "step"' incorporates:
  //   Inport: '<S576>/In1'

  if (rtb_Sum6_dk != 0.0) {
    // Outputs for IfAction SubSystem: '<S575>/Hold this value' incorporates:
    //   ActionPort: '<S576>/Action Port'

    localB->In1_l = rtb_Sum6_dk;

    // End of Outputs for SubSystem: '<S575>/Hold this value'
  }

  // End of If: '<S575>/if we went through a "step"'

  // Sum: '<S570>/Sum3' incorporates:
  //   Gain: '<S570>/kd_xb'
  //   Gain: '<S570>/kp_xb'
  //   Gain: '<S575>/divide by delta T'

  *rtd_BLUE_Fx = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_l *
    Template_v3_08_2020a_P.Kd_xblue + Template_v3_08_2020a_P.Kp_xblue *
    rtb_Subtract_ho;

  // Sum: '<S553>/Subtract1' incorporates:
  //   Constant: '<S553>/Desired Py (BLUE)'

  rtb_Sum6_dk = Template_v3_08_2020a_P.home_states_BLUE[1] - *rtd_BLUE_Py;

  // Delay: '<S577>/Delay1'
  if (localDW->icLoad_m != 0) {
    localDW->Delay1_DSTATE_j = rtb_Sum6_dk;
  }

  // Sum: '<S577>/Sum6' incorporates:
  //   Delay: '<S577>/Delay1'

  rtb_Sum6_fj = rtb_Sum6_dk - localDW->Delay1_DSTATE_j;

  // If: '<S577>/if we went through a "step"' incorporates:
  //   Inport: '<S578>/In1'

  if (rtb_Sum6_fj != 0.0) {
    // Outputs for IfAction SubSystem: '<S577>/Hold this value' incorporates:
    //   ActionPort: '<S578>/Action Port'

    localB->In1 = rtb_Sum6_fj;

    // End of Outputs for SubSystem: '<S577>/Hold this value'
  }

  // End of If: '<S577>/if we went through a "step"'

  // Sum: '<S571>/Sum3' incorporates:
  //   Gain: '<S571>/kd_yb'
  //   Gain: '<S571>/kp_yb'
  //   Gain: '<S577>/divide by delta T'

  *rtd_BLUE_Fy = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1 *
    Template_v3_08_2020a_P.Kd_yblue + Template_v3_08_2020a_P.Kp_yblue *
    rtb_Sum6_dk;

  // MATLAB Function: '<S572>/MATLAB Function2' incorporates:
  //   Constant: '<S553>/Desired Attitude (BLUE)'

  Template__MATLABFunction2_o(Template_v3_08_2020a_P.home_states_BLUE[2],
    &localB->sf_MATLABFunction2);

  // MATLAB Function: '<S572>/MATLAB Function3'
  Template_v3_MATLABFunction3(*rtd_BLUE_Rz, &localB->sf_MATLABFunction3);

  // Sum: '<S572>/Subtract2'
  localB->rtb_Subtract2_b1_m[0] = localB->sf_MATLABFunction2.Ox[0] -
    localB->sf_MATLABFunction3.Ox[0];
  localB->rtb_Subtract2_b1_m[1] = localB->sf_MATLABFunction2.Ox[1] -
    localB->sf_MATLABFunction3.Ox[1];

  // MATLAB Function: '<S572>/MATLAB Function4'
  Template_v3_MATLABFunction4(localB->sf_MATLABFunction2.Oy,
    localB->rtb_Subtract2_b1_m, &localB->sf_MATLABFunction4);

  // Delay: '<S573>/Delay1'
  if (localDW->icLoad_p != 0) {
    localDW->Delay1_DSTATE_n = localB->sf_MATLABFunction4.e_out;
  }

  // Sum: '<S573>/Sum6' incorporates:
  //   Delay: '<S573>/Delay1'

  rtb_Sum6_fj = localB->sf_MATLABFunction4.e_out - localDW->Delay1_DSTATE_n;

  // If: '<S573>/if we went through a "step"' incorporates:
  //   Inport: '<S574>/In1'

  if (rtb_Sum6_fj != 0.0) {
    // Outputs for IfAction SubSystem: '<S573>/Hold this value' incorporates:
    //   ActionPort: '<S574>/Action Port'

    localB->In1_p = rtb_Sum6_fj;

    // End of Outputs for SubSystem: '<S573>/Hold this value'
  }

  // End of If: '<S573>/if we went through a "step"'

  // Sum: '<S569>/Sum3' incorporates:
  //   Gain: '<S569>/kd_tb'
  //   Gain: '<S569>/kp_tb'
  //   Gain: '<S573>/divide by delta T'

  *rtd_BLUE_Tz = 1.0 / Template_v3_08_2020a_P.baseRate * localB->In1_p *
    Template_v3_08_2020a_P.Kd_tblue + Template_v3_08_2020a_P.Kp_tblue *
    localB->sf_MATLABFunction4.e_out;

  // DataStoreWrite: '<S553>/Data Store Write3' incorporates:
  //   Constant: '<S553>/Puck State'

  *rtd_Float_State = localP->PuckState_Value;

  // Update for Delay: '<S575>/Delay1'
  localDW->icLoad = 0U;
  localDW->Delay1_DSTATE = rtb_Subtract_ho;

  // Update for Delay: '<S577>/Delay1'
  localDW->icLoad_m = 0U;
  localDW->Delay1_DSTATE_j = rtb_Sum6_dk;

  // Update for Delay: '<S573>/Delay1'
  localDW->icLoad_p = 0U;
  localDW->Delay1_DSTATE_n = localB->sf_MATLABFunction4.e_out;
}

static real_T Template_v3_08_20_rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      Template_v3_08_2020a_B.u0_m = 1;
    } else {
      Template_v3_08_2020a_B.u0_m = -1;
    }

    if (u1 > 0.0) {
      Template_v3_08_2020a_B.u1 = 1;
    } else {
      Template_v3_08_2020a_B.u1 = -1;
    }

    y = atan2(static_cast<real_T>(Template_v3_08_2020a_B.u0_m),
              static_cast<real_T>(Template_v3_08_2020a_B.u1));
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

static real_T Temp_rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
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

  Template_v3_08_2020a_B.lo = *u % 127773U * 16807U;
  Template_v3_08_2020a_B.hi = *u / 127773U * 2836U;
  if (Template_v3_08_2020a_B.lo < Template_v3_08_2020a_B.hi) {
    *u = 2147483647U - (Template_v3_08_2020a_B.hi - Template_v3_08_2020a_B.lo);
  } else {
    *u = Template_v3_08_2020a_B.lo - Template_v3_08_2020a_B.hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

static real_T Temp_rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T y;

  // Normal (Gaussian) random number generator
  do {
    Template_v3_08_2020a_B.sr = 2.0 * Temp_rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    Template_v3_08_2020a_B.si = 2.0 * Temp_rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    Template_v3_08_2020a_B.si = Template_v3_08_2020a_B.sr *
      Template_v3_08_2020a_B.sr + Template_v3_08_2020a_B.si *
      Template_v3_08_2020a_B.si;
  } while (Template_v3_08_2020a_B.si > 1.0);

  y = sqrt(-2.0 * log(Template_v3_08_2020a_B.si) / Template_v3_08_2020a_B.si) *
    Template_v3_08_2020a_B.sr;
  return y;
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static real_T Template_v3_08_2020a_xnrm2(int32_T n, const real_T x[36], int32_T
  ix0)
{
  real_T y;
  y = 0.0;
  Template_v3_08_2020a_B.scale_d = 3.3121686421112381E-170;
  Template_v3_08_2020a_B.kend_n = (ix0 + n) - 1;
  Template_v3_08_2020a_B.k_b = ix0;
  while (Template_v3_08_2020a_B.k_b <= Template_v3_08_2020a_B.kend_n) {
    Template_v3_08_2020a_B.absxk_d = fabs(x[Template_v3_08_2020a_B.k_b - 1]);
    if (Template_v3_08_2020a_B.absxk_d > Template_v3_08_2020a_B.scale_d) {
      Template_v3_08_2020a_B.t_l = Template_v3_08_2020a_B.scale_d /
        Template_v3_08_2020a_B.absxk_d;
      y = y * Template_v3_08_2020a_B.t_l * Template_v3_08_2020a_B.t_l + 1.0;
      Template_v3_08_2020a_B.scale_d = Template_v3_08_2020a_B.absxk_d;
    } else {
      Template_v3_08_2020a_B.t_l = Template_v3_08_2020a_B.absxk_d /
        Template_v3_08_2020a_B.scale_d;
      y += Template_v3_08_2020a_B.t_l * Template_v3_08_2020a_B.t_l;
    }

    Template_v3_08_2020a_B.k_b++;
  }

  return Template_v3_08_2020a_B.scale_d * sqrt(y);
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static real_T Template_v3_08_2020a_xnrm2_l(int32_T n, const real_T x[6], int32_T
  ix0)
{
  real_T y;
  y = 0.0;
  Template_v3_08_2020a_B.scale_l = 3.3121686421112381E-170;
  Template_v3_08_2020a_B.kend = (ix0 + n) - 1;
  Template_v3_08_2020a_B.k = ix0;
  while (Template_v3_08_2020a_B.k <= Template_v3_08_2020a_B.kend) {
    Template_v3_08_2020a_B.absxk = fabs(x[Template_v3_08_2020a_B.k - 1]);
    if (Template_v3_08_2020a_B.absxk > Template_v3_08_2020a_B.scale_l) {
      Template_v3_08_2020a_B.t = Template_v3_08_2020a_B.scale_l /
        Template_v3_08_2020a_B.absxk;
      y = y * Template_v3_08_2020a_B.t * Template_v3_08_2020a_B.t + 1.0;
      Template_v3_08_2020a_B.scale_l = Template_v3_08_2020a_B.absxk;
    } else {
      Template_v3_08_2020a_B.t = Template_v3_08_2020a_B.absxk /
        Template_v3_08_2020a_B.scale_l;
      y += Template_v3_08_2020a_B.t * Template_v3_08_2020a_B.t;
    }

    Template_v3_08_2020a_B.k++;
  }

  return Template_v3_08_2020a_B.scale_l * sqrt(y);
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xaxpy_ot(int32_T n, real_T a, const real_T x[6],
  int32_T ix0, real_T y[36], int32_T iy0)
{
  if (!(a == 0.0)) {
    Template_v3_08_2020a_B.ix_i = ix0 - 1;
    Template_v3_08_2020a_B.iy_oy = iy0 - 1;
    Template_v3_08_2020a_B.k_nv = 0;
    while (Template_v3_08_2020a_B.k_nv <= n - 1) {
      y[Template_v3_08_2020a_B.iy_oy] += a * x[Template_v3_08_2020a_B.ix_i];
      Template_v3_08_2020a_B.ix_i++;
      Template_v3_08_2020a_B.iy_oy++;
      Template_v3_08_2020a_B.k_nv++;
    }
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xaxpy_o(int32_T n, real_T a, const real_T x[36],
  int32_T ix0, real_T y[6], int32_T iy0)
{
  if (!(a == 0.0)) {
    Template_v3_08_2020a_B.ix_j = ix0 - 1;
    Template_v3_08_2020a_B.iy_o = iy0 - 1;
    Template_v3_08_2020a_B.k_n = 0;
    while (Template_v3_08_2020a_B.k_n <= n - 1) {
      y[Template_v3_08_2020a_B.iy_o] += a * x[Template_v3_08_2020a_B.ix_j];
      Template_v3_08_2020a_B.ix_j++;
      Template_v3_08_2020a_B.iy_o++;
      Template_v3_08_2020a_B.k_n++;
    }
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static real_T Template_v3_08_2020a_xdotc(int32_T n, const real_T x[36], int32_T
  ix0, const real_T y[36], int32_T iy0)
{
  real_T d;
  d = 0.0;
  Template_v3_08_2020a_B.ix_f = ix0;
  Template_v3_08_2020a_B.iy_a = iy0;
  Template_v3_08_2020a_B.k_ju = 0;
  while (Template_v3_08_2020a_B.k_ju <= n - 1) {
    d += x[Template_v3_08_2020a_B.ix_f - 1] * y[Template_v3_08_2020a_B.iy_a - 1];
    Template_v3_08_2020a_B.ix_f++;
    Template_v3_08_2020a_B.iy_a++;
    Template_v3_08_2020a_B.k_ju++;
  }

  return d;
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xaxpy(int32_T n, real_T a, int32_T ix0, real_T
  y[36], int32_T iy0)
{
  if (!(a == 0.0)) {
    Template_v3_08_2020a_B.ix_e = ix0 - 1;
    Template_v3_08_2020a_B.iy_bj = iy0 - 1;
    Template_v3_08_2020a_B.k_j = 0;
    while (Template_v3_08_2020a_B.k_j <= n - 1) {
      y[Template_v3_08_2020a_B.iy_bj] += a * y[Template_v3_08_2020a_B.ix_e];
      Template_v3_08_2020a_B.ix_e++;
      Template_v3_08_2020a_B.iy_bj++;
      Template_v3_08_2020a_B.k_j++;
    }
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xscal(real_T a, real_T x[36], int32_T ix0)
{
  Template_v3_08_2020a_B.k_c = ix0;
  while (Template_v3_08_2020a_B.k_c <= ix0 + 5) {
    x[Template_v3_08_2020a_B.k_c - 1] *= a;
    Template_v3_08_2020a_B.k_c++;
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xswap(real_T x[36], int32_T ix0, int32_T iy0)
{
  Template_v3_08_2020a_B.ix_h = ix0 - 1;
  Template_v3_08_2020a_B.iy_b = iy0 - 1;
  for (Template_v3_08_2020a_B.k_d = 0; Template_v3_08_2020a_B.k_d < 6;
       Template_v3_08_2020a_B.k_d++) {
    Template_v3_08_2020a_B.temp_o = x[Template_v3_08_2020a_B.ix_h];
    x[Template_v3_08_2020a_B.ix_h] = x[Template_v3_08_2020a_B.iy_b];
    x[Template_v3_08_2020a_B.iy_b] = Template_v3_08_2020a_B.temp_o;
    Template_v3_08_2020a_B.ix_h++;
    Template_v3_08_2020a_B.iy_b++;
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xrotg(real_T *a, real_T *b, real_T *c, real_T
  *s)
{
  Template_v3_08_2020a_B.roe = *b;
  Template_v3_08_2020a_B.absa = fabs(*a);
  Template_v3_08_2020a_B.absb = fabs(*b);
  if (Template_v3_08_2020a_B.absa > Template_v3_08_2020a_B.absb) {
    Template_v3_08_2020a_B.roe = *a;
  }

  Template_v3_08_2020a_B.scale = Template_v3_08_2020a_B.absa +
    Template_v3_08_2020a_B.absb;
  if (Template_v3_08_2020a_B.scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    *b = 0.0;
  } else {
    Template_v3_08_2020a_B.ads = Template_v3_08_2020a_B.absa /
      Template_v3_08_2020a_B.scale;
    Template_v3_08_2020a_B.bds = Template_v3_08_2020a_B.absb /
      Template_v3_08_2020a_B.scale;
    Template_v3_08_2020a_B.scale *= sqrt(Template_v3_08_2020a_B.ads *
      Template_v3_08_2020a_B.ads + Template_v3_08_2020a_B.bds *
      Template_v3_08_2020a_B.bds);
    if (Template_v3_08_2020a_B.roe < 0.0) {
      Template_v3_08_2020a_B.scale = -Template_v3_08_2020a_B.scale;
    }

    *c = *a / Template_v3_08_2020a_B.scale;
    *s = *b / Template_v3_08_2020a_B.scale;
    if (Template_v3_08_2020a_B.absa > Template_v3_08_2020a_B.absb) {
      *b = *s;
    } else if (*c != 0.0) {
      *b = 1.0 / *c;
    } else {
      *b = 1.0;
    }
  }

  *a = Template_v3_08_2020a_B.scale;
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_xrot(real_T x[36], int32_T ix0, int32_T iy0,
  real_T c, real_T s)
{
  Template_v3_08_2020a_B.ix = ix0 - 1;
  Template_v3_08_2020a_B.iy = iy0 - 1;
  for (Template_v3_08_2020a_B.k_l = 0; Template_v3_08_2020a_B.k_l < 6;
       Template_v3_08_2020a_B.k_l++) {
    Template_v3_08_2020a_B.temp = c * x[Template_v3_08_2020a_B.ix] + s *
      x[Template_v3_08_2020a_B.iy];
    x[Template_v3_08_2020a_B.iy] = c * x[Template_v3_08_2020a_B.iy] - s *
      x[Template_v3_08_2020a_B.ix];
    x[Template_v3_08_2020a_B.ix] = Template_v3_08_2020a_B.temp;
    Template_v3_08_2020a_B.iy++;
    Template_v3_08_2020a_B.ix++;
  }
}

// Function for MATLAB Function: '<S695>/Dynamics Model'
static void Template_v3_08_2020a_svd(const real_T A[36], real_T U[36], real_T s
  [6], real_T V[36])
{
  int32_T exitg1;
  boolean_T exitg2;
  memcpy(&Template_v3_08_2020a_B.b_A[0], &A[0], 36U * sizeof(real_T));
  for (Template_v3_08_2020a_B.i_b = 0; Template_v3_08_2020a_B.i_b < 6;
       Template_v3_08_2020a_B.i_b++) {
    Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] = 0.0;
    Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] = 0.0;
    Template_v3_08_2020a_B.work[Template_v3_08_2020a_B.i_b] = 0.0;
  }

  memset(&U[0], 0, 36U * sizeof(real_T));
  memset(&Template_v3_08_2020a_B.Vf[0], 0, 36U * sizeof(real_T));
  for (Template_v3_08_2020a_B.i_b = 0; Template_v3_08_2020a_B.i_b < 5;
       Template_v3_08_2020a_B.i_b++) {
    Template_v3_08_2020a_B.qq = 6 * Template_v3_08_2020a_B.i_b +
      Template_v3_08_2020a_B.i_b;
    Template_v3_08_2020a_B.apply_transform = false;
    Template_v3_08_2020a_B.nrm = Template_v3_08_2020a_xnrm2(6 -
      Template_v3_08_2020a_B.i_b, Template_v3_08_2020a_B.b_A,
      Template_v3_08_2020a_B.qq + 1);
    if (Template_v3_08_2020a_B.nrm > 0.0) {
      Template_v3_08_2020a_B.apply_transform = true;
      if (Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.qq] < 0.0) {
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] =
          -Template_v3_08_2020a_B.nrm;
      } else {
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] =
          Template_v3_08_2020a_B.nrm;
      }

      if (fabs(Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b]) >=
          1.0020841800044864E-292) {
        Template_v3_08_2020a_B.nrm = 1.0 /
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b];
        Template_v3_08_2020a_B.qp1q = (Template_v3_08_2020a_B.qq -
          Template_v3_08_2020a_B.i_b) + 6;
        Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qq;
        while (Template_v3_08_2020a_B.qjj + 1 <= Template_v3_08_2020a_B.qp1q) {
          Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.qjj] *=
            Template_v3_08_2020a_B.nrm;
          Template_v3_08_2020a_B.qjj++;
        }
      } else {
        Template_v3_08_2020a_B.qp1q = (Template_v3_08_2020a_B.qq -
          Template_v3_08_2020a_B.i_b) + 6;
        Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qq;
        while (Template_v3_08_2020a_B.qjj + 1 <= Template_v3_08_2020a_B.qp1q) {
          Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.qjj] /=
            Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b];
          Template_v3_08_2020a_B.qjj++;
        }
      }

      Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.qq]++;
      Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] =
        -Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b];
    } else {
      Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] = 0.0;
    }

    Template_v3_08_2020a_B.qp1q = Template_v3_08_2020a_B.i_b + 1;
    while (Template_v3_08_2020a_B.qp1q + 1 < 7) {
      Template_v3_08_2020a_B.qjj = 6 * Template_v3_08_2020a_B.qp1q +
        Template_v3_08_2020a_B.i_b;
      if (Template_v3_08_2020a_B.apply_transform) {
        Template_v3_08_2020a_xaxpy(6 - Template_v3_08_2020a_B.i_b,
          -(Template_v3_08_2020a_xdotc(6 - Template_v3_08_2020a_B.i_b,
          Template_v3_08_2020a_B.b_A, Template_v3_08_2020a_B.qq + 1,
          Template_v3_08_2020a_B.b_A, Template_v3_08_2020a_B.qjj + 1) /
            Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.i_b + 6 *
            Template_v3_08_2020a_B.i_b]), Template_v3_08_2020a_B.qq + 1,
          Template_v3_08_2020a_B.b_A, Template_v3_08_2020a_B.qjj + 1);
      }

      Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q] =
        Template_v3_08_2020a_B.b_A[Template_v3_08_2020a_B.qjj];
      Template_v3_08_2020a_B.qp1q++;
    }

    Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b;
    while (Template_v3_08_2020a_B.qq + 1 < 7) {
      U[Template_v3_08_2020a_B.qq + 6 * Template_v3_08_2020a_B.i_b] =
        Template_v3_08_2020a_B.b_A[6 * Template_v3_08_2020a_B.i_b +
        Template_v3_08_2020a_B.qq];
      Template_v3_08_2020a_B.qq++;
    }

    if (Template_v3_08_2020a_B.i_b + 1 <= 4) {
      Template_v3_08_2020a_B.nrm = Template_v3_08_2020a_xnrm2_l(5 -
        Template_v3_08_2020a_B.i_b, Template_v3_08_2020a_B.e,
        Template_v3_08_2020a_B.i_b + 2);
      if (Template_v3_08_2020a_B.nrm == 0.0) {
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] = 0.0;
      } else {
        if (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b + 1] < 0.0) {
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] =
            -Template_v3_08_2020a_B.nrm;
        } else {
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] =
            Template_v3_08_2020a_B.nrm;
        }

        Template_v3_08_2020a_B.nrm =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b];
        if (fabs(Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b]) >=
            1.0020841800044864E-292) {
          Template_v3_08_2020a_B.nrm = 1.0 /
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b];
          Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
          while (Template_v3_08_2020a_B.qq + 1 <= 6) {
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] *=
              Template_v3_08_2020a_B.nrm;
            Template_v3_08_2020a_B.qq++;
          }
        } else {
          Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
          while (Template_v3_08_2020a_B.qq + 1 <= 6) {
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] /=
              Template_v3_08_2020a_B.nrm;
            Template_v3_08_2020a_B.qq++;
          }
        }

        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b + 1]++;
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] =
          -Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b];
        Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
        while (Template_v3_08_2020a_B.qq + 1 < 7) {
          Template_v3_08_2020a_B.work[Template_v3_08_2020a_B.qq] = 0.0;
          Template_v3_08_2020a_B.qq++;
        }

        Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
        while (Template_v3_08_2020a_B.qq + 1 < 7) {
          Template_v3_08_2020a_xaxpy_o(5 - Template_v3_08_2020a_B.i_b,
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq],
            Template_v3_08_2020a_B.b_A, (Template_v3_08_2020a_B.i_b + 6 *
            Template_v3_08_2020a_B.qq) + 2, Template_v3_08_2020a_B.work,
            Template_v3_08_2020a_B.i_b + 2);
          Template_v3_08_2020a_B.qq++;
        }

        Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
        while (Template_v3_08_2020a_B.qq + 1 < 7) {
          Template_v3_08_2020a_xaxpy_ot(5 - Template_v3_08_2020a_B.i_b,
            -Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] /
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b + 1],
            Template_v3_08_2020a_B.work, Template_v3_08_2020a_B.i_b + 2,
            Template_v3_08_2020a_B.b_A, (Template_v3_08_2020a_B.i_b + 6 *
            Template_v3_08_2020a_B.qq) + 2);
          Template_v3_08_2020a_B.qq++;
        }
      }

      Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.i_b + 1;
      while (Template_v3_08_2020a_B.qq + 1 < 7) {
        Template_v3_08_2020a_B.Vf[Template_v3_08_2020a_B.qq + 6 *
          Template_v3_08_2020a_B.i_b] =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq];
        Template_v3_08_2020a_B.qq++;
      }
    }
  }

  Template_v3_08_2020a_B.i_b = 4;
  Template_v3_08_2020a_B.b_s[5] = Template_v3_08_2020a_B.b_A[35];
  Template_v3_08_2020a_B.e[4] = Template_v3_08_2020a_B.b_A[34];
  Template_v3_08_2020a_B.e[5] = 0.0;
  for (Template_v3_08_2020a_B.qq = 0; Template_v3_08_2020a_B.qq < 6;
       Template_v3_08_2020a_B.qq++) {
    U[Template_v3_08_2020a_B.qq + 30] = 0.0;
  }

  U[35] = 1.0;
  for (Template_v3_08_2020a_B.qp1q = 4; Template_v3_08_2020a_B.qp1q >= 0;
       Template_v3_08_2020a_B.qp1q--) {
    Template_v3_08_2020a_B.qq = 6 * Template_v3_08_2020a_B.qp1q +
      Template_v3_08_2020a_B.qp1q;
    if (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] != 0.0) {
      Template_v3_08_2020a_B.qp1jj = Template_v3_08_2020a_B.qp1q + 1;
      while (Template_v3_08_2020a_B.qp1jj + 1 < 7) {
        Template_v3_08_2020a_B.qjj = (6 * Template_v3_08_2020a_B.qp1jj +
          Template_v3_08_2020a_B.qp1q) + 1;
        Template_v3_08_2020a_xaxpy(6 - Template_v3_08_2020a_B.qp1q,
          -(Template_v3_08_2020a_xdotc(6 - Template_v3_08_2020a_B.qp1q, U,
          Template_v3_08_2020a_B.qq + 1, U, Template_v3_08_2020a_B.qjj) /
            U[Template_v3_08_2020a_B.qq]), Template_v3_08_2020a_B.qq + 1, U,
          Template_v3_08_2020a_B.qjj);
        Template_v3_08_2020a_B.qp1jj++;
      }

      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qp1q;
      while (Template_v3_08_2020a_B.qjj + 1 < 7) {
        Template_v3_08_2020a_B.qp1jj = 6 * Template_v3_08_2020a_B.qp1q +
          Template_v3_08_2020a_B.qjj;
        U[Template_v3_08_2020a_B.qp1jj] = -U[Template_v3_08_2020a_B.qp1jj];
        Template_v3_08_2020a_B.qjj++;
      }

      U[Template_v3_08_2020a_B.qq]++;
      Template_v3_08_2020a_B.qq = 0;
      while (Template_v3_08_2020a_B.qq <= Template_v3_08_2020a_B.qp1q - 1) {
        U[Template_v3_08_2020a_B.qq + 6 * Template_v3_08_2020a_B.qp1q] = 0.0;
        Template_v3_08_2020a_B.qq++;
      }
    } else {
      for (Template_v3_08_2020a_B.qjj = 0; Template_v3_08_2020a_B.qjj < 6;
           Template_v3_08_2020a_B.qjj++) {
        U[Template_v3_08_2020a_B.qjj + 6 * Template_v3_08_2020a_B.qp1q] = 0.0;
      }

      U[Template_v3_08_2020a_B.qq] = 1.0;
    }
  }

  for (Template_v3_08_2020a_B.qq = 5; Template_v3_08_2020a_B.qq >= 0;
       Template_v3_08_2020a_B.qq--) {
    if ((Template_v3_08_2020a_B.qq + 1 <= 4) &&
        (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] != 0.0)) {
      Template_v3_08_2020a_B.qp1q = (6 * Template_v3_08_2020a_B.qq +
        Template_v3_08_2020a_B.qq) + 2;
      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qq + 1;
      while (Template_v3_08_2020a_B.qjj + 1 < 7) {
        Template_v3_08_2020a_B.qp1jj = (6 * Template_v3_08_2020a_B.qjj +
          Template_v3_08_2020a_B.qq) + 2;
        Template_v3_08_2020a_xaxpy(5 - Template_v3_08_2020a_B.qq,
          -(Template_v3_08_2020a_xdotc(5 - Template_v3_08_2020a_B.qq,
          Template_v3_08_2020a_B.Vf, Template_v3_08_2020a_B.qp1q,
          Template_v3_08_2020a_B.Vf, Template_v3_08_2020a_B.qp1jj) /
            Template_v3_08_2020a_B.Vf[Template_v3_08_2020a_B.qp1q - 1]),
          Template_v3_08_2020a_B.qp1q, Template_v3_08_2020a_B.Vf,
          Template_v3_08_2020a_B.qp1jj);
        Template_v3_08_2020a_B.qjj++;
      }
    }

    for (Template_v3_08_2020a_B.qp1q = 0; Template_v3_08_2020a_B.qp1q < 6;
         Template_v3_08_2020a_B.qp1q++) {
      Template_v3_08_2020a_B.Vf[Template_v3_08_2020a_B.qp1q + 6 *
        Template_v3_08_2020a_B.qq] = 0.0;
    }

    Template_v3_08_2020a_B.Vf[Template_v3_08_2020a_B.qq + 6 *
      Template_v3_08_2020a_B.qq] = 1.0;
  }

  for (Template_v3_08_2020a_B.qq = 0; Template_v3_08_2020a_B.qq < 6;
       Template_v3_08_2020a_B.qq++) {
    Template_v3_08_2020a_B.ztest =
      Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq];
    if (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq] != 0.0) {
      Template_v3_08_2020a_B.rt = fabs
        (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq]);
      Template_v3_08_2020a_B.nrm =
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq] /
        Template_v3_08_2020a_B.rt;
      Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq] =
        Template_v3_08_2020a_B.rt;
      if (Template_v3_08_2020a_B.qq + 1 < 6) {
        Template_v3_08_2020a_B.ztest =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] /
          Template_v3_08_2020a_B.nrm;
      }

      Template_v3_08_2020a_xscal(Template_v3_08_2020a_B.nrm, U, 6 *
        Template_v3_08_2020a_B.qq + 1);
    }

    if ((Template_v3_08_2020a_B.qq + 1 < 6) && (Template_v3_08_2020a_B.ztest !=
         0.0)) {
      Template_v3_08_2020a_B.rt = fabs(Template_v3_08_2020a_B.ztest);
      Template_v3_08_2020a_B.nrm = Template_v3_08_2020a_B.rt /
        Template_v3_08_2020a_B.ztest;
      Template_v3_08_2020a_B.ztest = Template_v3_08_2020a_B.rt;
      Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq + 1] *=
        Template_v3_08_2020a_B.nrm;
      Template_v3_08_2020a_xscal(Template_v3_08_2020a_B.nrm,
        Template_v3_08_2020a_B.Vf, 6 * (Template_v3_08_2020a_B.qq + 1) + 1);
    }

    Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qq] =
      Template_v3_08_2020a_B.ztest;
  }

  Template_v3_08_2020a_B.qq = 0;
  Template_v3_08_2020a_B.nrm = 0.0;
  for (Template_v3_08_2020a_B.qp1q = 0; Template_v3_08_2020a_B.qp1q < 6;
       Template_v3_08_2020a_B.qp1q++) {
    Template_v3_08_2020a_B.ztest = fabs
      (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q]);
    Template_v3_08_2020a_B.rt = fabs
      (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q]);
    if ((Template_v3_08_2020a_B.ztest > Template_v3_08_2020a_B.rt) || rtIsNaN
        (Template_v3_08_2020a_B.rt)) {
      Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.ztest;
    }

    if ((!(Template_v3_08_2020a_B.nrm > Template_v3_08_2020a_B.rt)) && (!rtIsNaN
         (Template_v3_08_2020a_B.rt))) {
      Template_v3_08_2020a_B.nrm = Template_v3_08_2020a_B.rt;
    }
  }

  while ((Template_v3_08_2020a_B.i_b + 2 > 0) && (Template_v3_08_2020a_B.qq < 75))
  {
    Template_v3_08_2020a_B.qp1jj = Template_v3_08_2020a_B.i_b + 1;
    do {
      exitg1 = 0;
      Template_v3_08_2020a_B.qp1q = Template_v3_08_2020a_B.qp1jj;
      if (Template_v3_08_2020a_B.qp1jj == 0) {
        exitg1 = 1;
      } else {
        Template_v3_08_2020a_B.rt = fabs
          (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1jj - 1]);
        if (Template_v3_08_2020a_B.rt <= (fabs
             (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1jj - 1]) +
             fabs(Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1jj])) *
            2.2204460492503131E-16) {
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else if ((Template_v3_08_2020a_B.rt <= 1.0020841800044864E-292) ||
                   ((Template_v3_08_2020a_B.qq > 20) &&
                    (Template_v3_08_2020a_B.rt <= 2.2204460492503131E-16 *
                     Template_v3_08_2020a_B.nrm))) {
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else {
          Template_v3_08_2020a_B.qp1jj--;
        }
      }
    } while (exitg1 == 0);

    if (Template_v3_08_2020a_B.i_b + 1 == Template_v3_08_2020a_B.qp1jj) {
      Template_v3_08_2020a_B.qp1jj = 4;
    } else {
      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.i_b + 2;
      Template_v3_08_2020a_B.k_ii = Template_v3_08_2020a_B.i_b + 2;
      exitg2 = false;
      while ((!exitg2) && (Template_v3_08_2020a_B.k_ii >=
                           Template_v3_08_2020a_B.qp1jj)) {
        Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.k_ii;
        if (Template_v3_08_2020a_B.k_ii == Template_v3_08_2020a_B.qp1jj) {
          exitg2 = true;
        } else {
          Template_v3_08_2020a_B.rt = 0.0;
          if (Template_v3_08_2020a_B.k_ii < Template_v3_08_2020a_B.i_b + 2) {
            Template_v3_08_2020a_B.rt = fabs
              (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.k_ii - 1]);
          }

          if (Template_v3_08_2020a_B.k_ii > Template_v3_08_2020a_B.qp1jj + 1) {
            Template_v3_08_2020a_B.rt += fabs
              (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.k_ii - 2]);
          }

          Template_v3_08_2020a_B.ztest = fabs
            (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.k_ii - 1]);
          if ((Template_v3_08_2020a_B.ztest <= 2.2204460492503131E-16 *
               Template_v3_08_2020a_B.rt) || (Template_v3_08_2020a_B.ztest <=
               1.0020841800044864E-292)) {
            Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.k_ii - 1] = 0.0;
            exitg2 = true;
          } else {
            Template_v3_08_2020a_B.k_ii--;
          }
        }
      }

      if (Template_v3_08_2020a_B.qjj == Template_v3_08_2020a_B.qp1jj) {
        Template_v3_08_2020a_B.qp1jj = 3;
      } else if (Template_v3_08_2020a_B.i_b + 2 == Template_v3_08_2020a_B.qjj) {
        Template_v3_08_2020a_B.qp1jj = 1;
      } else {
        Template_v3_08_2020a_B.qp1jj = 2;
        Template_v3_08_2020a_B.qp1q = Template_v3_08_2020a_B.qjj;
      }
    }

    switch (Template_v3_08_2020a_B.qp1jj) {
     case 1:
      Template_v3_08_2020a_B.rt =
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b];
      Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] = 0.0;
      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.i_b;
      while (Template_v3_08_2020a_B.qjj + 1 >= Template_v3_08_2020a_B.qp1q + 1)
      {
        Template_v3_08_2020a_xrotg
          (&Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj],
           &Template_v3_08_2020a_B.rt, &Template_v3_08_2020a_B.ztest,
           &Template_v3_08_2020a_B.sqds);
        if (Template_v3_08_2020a_B.qjj + 1 > Template_v3_08_2020a_B.qp1q + 1) {
          Template_v3_08_2020a_B.emm1 =
            Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1];
          Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.emm1 *
            -Template_v3_08_2020a_B.sqds;
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1] =
            Template_v3_08_2020a_B.emm1 * Template_v3_08_2020a_B.ztest;
        }

        Template_v3_08_2020a_xrot(Template_v3_08_2020a_B.Vf, 6 *
          Template_v3_08_2020a_B.qjj + 1, 6 * (Template_v3_08_2020a_B.i_b + 1) +
          1, Template_v3_08_2020a_B.ztest, Template_v3_08_2020a_B.sqds);
        Template_v3_08_2020a_B.qjj--;
      }
      break;

     case 2:
      Template_v3_08_2020a_B.rt =
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q - 1];
      Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q - 1] = 0.0;
      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qp1q;
      while (Template_v3_08_2020a_B.qjj + 1 <= Template_v3_08_2020a_B.i_b + 2) {
        Template_v3_08_2020a_xrotg
          (&Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj],
           &Template_v3_08_2020a_B.rt, &Template_v3_08_2020a_B.ztest,
           &Template_v3_08_2020a_B.sqds);
        Template_v3_08_2020a_B.rt = -Template_v3_08_2020a_B.sqds *
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj];
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj] *=
          Template_v3_08_2020a_B.ztest;
        Template_v3_08_2020a_xrot(U, 6 * Template_v3_08_2020a_B.qjj + 1, 6 *
          (Template_v3_08_2020a_B.qp1q - 1) + 1, Template_v3_08_2020a_B.ztest,
          Template_v3_08_2020a_B.sqds);
        Template_v3_08_2020a_B.qjj++;
      }
      break;

     case 3:
      Template_v3_08_2020a_B.sqds =
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b + 1];
      Template_v3_08_2020a_B.ztest = fabs(Template_v3_08_2020a_B.sqds);
      Template_v3_08_2020a_B.rt = fabs
        (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b]);
      if ((Template_v3_08_2020a_B.ztest > Template_v3_08_2020a_B.rt) || rtIsNaN
          (Template_v3_08_2020a_B.rt)) {
        Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.ztest;
      }

      Template_v3_08_2020a_B.ztest = fabs
        (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b]);
      if ((Template_v3_08_2020a_B.rt > Template_v3_08_2020a_B.ztest) || rtIsNaN
          (Template_v3_08_2020a_B.ztest)) {
        Template_v3_08_2020a_B.ztest = Template_v3_08_2020a_B.rt;
      }

      Template_v3_08_2020a_B.rt = fabs
        (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q]);
      if ((Template_v3_08_2020a_B.ztest > Template_v3_08_2020a_B.rt) || rtIsNaN
          (Template_v3_08_2020a_B.rt)) {
        Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.ztest;
      }

      Template_v3_08_2020a_B.ztest = fabs
        (Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q]);
      if ((Template_v3_08_2020a_B.rt > Template_v3_08_2020a_B.ztest) || rtIsNaN
          (Template_v3_08_2020a_B.ztest)) {
        Template_v3_08_2020a_B.ztest = Template_v3_08_2020a_B.rt;
      }

      Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.sqds /
        Template_v3_08_2020a_B.ztest;
      Template_v3_08_2020a_B.smm1 =
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b] /
        Template_v3_08_2020a_B.ztest;
      Template_v3_08_2020a_B.emm1 =
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] /
        Template_v3_08_2020a_B.ztest;
      Template_v3_08_2020a_B.sqds =
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] /
        Template_v3_08_2020a_B.ztest;
      Template_v3_08_2020a_B.smm1 = ((Template_v3_08_2020a_B.smm1 +
        Template_v3_08_2020a_B.rt) * (Template_v3_08_2020a_B.smm1 -
        Template_v3_08_2020a_B.rt) + Template_v3_08_2020a_B.emm1 *
        Template_v3_08_2020a_B.emm1) / 2.0;
      Template_v3_08_2020a_B.emm1 *= Template_v3_08_2020a_B.rt;
      Template_v3_08_2020a_B.emm1 *= Template_v3_08_2020a_B.emm1;
      if ((Template_v3_08_2020a_B.smm1 != 0.0) || (Template_v3_08_2020a_B.emm1
           != 0.0)) {
        Template_v3_08_2020a_B.shift = sqrt(Template_v3_08_2020a_B.smm1 *
          Template_v3_08_2020a_B.smm1 + Template_v3_08_2020a_B.emm1);
        if (Template_v3_08_2020a_B.smm1 < 0.0) {
          Template_v3_08_2020a_B.shift = -Template_v3_08_2020a_B.shift;
        }

        Template_v3_08_2020a_B.shift = Template_v3_08_2020a_B.emm1 /
          (Template_v3_08_2020a_B.smm1 + Template_v3_08_2020a_B.shift);
      } else {
        Template_v3_08_2020a_B.shift = 0.0;
      }

      Template_v3_08_2020a_B.rt = (Template_v3_08_2020a_B.sqds +
        Template_v3_08_2020a_B.rt) * (Template_v3_08_2020a_B.sqds -
        Template_v3_08_2020a_B.rt) + Template_v3_08_2020a_B.shift;
      Template_v3_08_2020a_B.ztest =
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qp1q] /
        Template_v3_08_2020a_B.ztest * Template_v3_08_2020a_B.sqds;
      Template_v3_08_2020a_B.qjj = Template_v3_08_2020a_B.qp1q + 1;
      while (Template_v3_08_2020a_B.qjj <= Template_v3_08_2020a_B.i_b + 1) {
        Template_v3_08_2020a_xrotg(&Template_v3_08_2020a_B.rt,
          &Template_v3_08_2020a_B.ztest, &Template_v3_08_2020a_B.sqds,
          &Template_v3_08_2020a_B.smm1);
        if (Template_v3_08_2020a_B.qjj > Template_v3_08_2020a_B.qp1q + 1) {
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 2] =
            Template_v3_08_2020a_B.rt;
        }

        Template_v3_08_2020a_B.emm1 =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1];
        Template_v3_08_2020a_B.ztest =
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj - 1];
        Template_v3_08_2020a_B.rt = Template_v3_08_2020a_B.ztest *
          Template_v3_08_2020a_B.sqds + Template_v3_08_2020a_B.emm1 *
          Template_v3_08_2020a_B.smm1;
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1] =
          Template_v3_08_2020a_B.emm1 * Template_v3_08_2020a_B.sqds -
          Template_v3_08_2020a_B.ztest * Template_v3_08_2020a_B.smm1;
        Template_v3_08_2020a_B.ztest = Template_v3_08_2020a_B.smm1 *
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj];
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj] *=
          Template_v3_08_2020a_B.sqds;
        Template_v3_08_2020a_xrot(Template_v3_08_2020a_B.Vf, 6 *
          (Template_v3_08_2020a_B.qjj - 1) + 1, 6 * Template_v3_08_2020a_B.qjj +
          1, Template_v3_08_2020a_B.sqds, Template_v3_08_2020a_B.smm1);
        Template_v3_08_2020a_xrotg(&Template_v3_08_2020a_B.rt,
          &Template_v3_08_2020a_B.ztest, &Template_v3_08_2020a_B.sqds,
          &Template_v3_08_2020a_B.smm1);
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj - 1] =
          Template_v3_08_2020a_B.rt;
        Template_v3_08_2020a_B.rt =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1] *
          Template_v3_08_2020a_B.sqds + Template_v3_08_2020a_B.smm1 *
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj];
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj] =
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj - 1] *
          -Template_v3_08_2020a_B.smm1 + Template_v3_08_2020a_B.sqds *
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qjj];
        Template_v3_08_2020a_B.ztest = Template_v3_08_2020a_B.smm1 *
          Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj];
        Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.qjj] *=
          Template_v3_08_2020a_B.sqds;
        Template_v3_08_2020a_xrot(U, 6 * (Template_v3_08_2020a_B.qjj - 1) + 1, 6
          * Template_v3_08_2020a_B.qjj + 1, Template_v3_08_2020a_B.sqds,
          Template_v3_08_2020a_B.smm1);
        Template_v3_08_2020a_B.qjj++;
      }

      Template_v3_08_2020a_B.e[Template_v3_08_2020a_B.i_b] =
        Template_v3_08_2020a_B.rt;
      Template_v3_08_2020a_B.qq++;
      break;

     default:
      if (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] < 0.0) {
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] =
          -Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q];
        Template_v3_08_2020a_xscal(-1.0, Template_v3_08_2020a_B.Vf, 6 *
          Template_v3_08_2020a_B.qp1q + 1);
      }

      Template_v3_08_2020a_B.qq = Template_v3_08_2020a_B.qp1q + 1;
      while ((Template_v3_08_2020a_B.qp1q + 1 < 6) &&
             (Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] <
              Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq])) {
        Template_v3_08_2020a_B.rt =
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q];
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qp1q] =
          Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq];
        Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.qq] =
          Template_v3_08_2020a_B.rt;
        Template_v3_08_2020a_xswap(Template_v3_08_2020a_B.Vf, 6 *
          Template_v3_08_2020a_B.qp1q + 1, 6 * (Template_v3_08_2020a_B.qp1q + 1)
          + 1);
        Template_v3_08_2020a_xswap(U, 6 * Template_v3_08_2020a_B.qp1q + 1, 6 *
          (Template_v3_08_2020a_B.qp1q + 1) + 1);
        Template_v3_08_2020a_B.qp1q = Template_v3_08_2020a_B.qq;
        Template_v3_08_2020a_B.qq++;
      }

      Template_v3_08_2020a_B.qq = 0;
      Template_v3_08_2020a_B.i_b--;
      break;
    }
  }

  for (Template_v3_08_2020a_B.i_b = 0; Template_v3_08_2020a_B.i_b < 6;
       Template_v3_08_2020a_B.i_b++) {
    s[Template_v3_08_2020a_B.i_b] =
      Template_v3_08_2020a_B.b_s[Template_v3_08_2020a_B.i_b];
    for (Template_v3_08_2020a_B.qq = 0; Template_v3_08_2020a_B.qq < 6;
         Template_v3_08_2020a_B.qq++) {
      V[Template_v3_08_2020a_B.qq + 6 * Template_v3_08_2020a_B.i_b] =
        Template_v3_08_2020a_B.Vf[6 * Template_v3_08_2020a_B.i_b +
        Template_v3_08_2020a_B.qq];
    }
  }
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

static void Template_v3_08_2_rt_invd3x3_snf(const real_T u[9], real_T y[9])
{
  memcpy(&Template_v3_08_2020a_B.x[0], &u[0], 9U * sizeof(real_T));
  Template_v3_08_2020a_B.p1 = 1;
  Template_v3_08_2020a_B.p2 = 3;
  Template_v3_08_2020a_B.p3 = 6;
  Template_v3_08_2020a_B.absx11 = fabs(u[0]);
  Template_v3_08_2020a_B.absx21 = fabs(u[1]);
  Template_v3_08_2020a_B.absx31 = fabs(u[2]);
  if ((Template_v3_08_2020a_B.absx21 > Template_v3_08_2020a_B.absx11) &&
      (Template_v3_08_2020a_B.absx21 > Template_v3_08_2020a_B.absx31)) {
    Template_v3_08_2020a_B.p1 = 4;
    Template_v3_08_2020a_B.p2 = 0;
    Template_v3_08_2020a_B.x[0] = u[1];
    Template_v3_08_2020a_B.x[1] = u[0];
    Template_v3_08_2020a_B.x[3] = u[4];
    Template_v3_08_2020a_B.x[4] = u[3];
    Template_v3_08_2020a_B.x[6] = u[7];
    Template_v3_08_2020a_B.x[7] = u[6];
  } else {
    if (Template_v3_08_2020a_B.absx31 > Template_v3_08_2020a_B.absx11) {
      Template_v3_08_2020a_B.p1 = 7;
      Template_v3_08_2020a_B.p3 = 0;
      Template_v3_08_2020a_B.x[2] = Template_v3_08_2020a_B.x[0];
      Template_v3_08_2020a_B.x[0] = u[2];
      Template_v3_08_2020a_B.x[5] = Template_v3_08_2020a_B.x[3];
      Template_v3_08_2020a_B.x[3] = u[5];
      Template_v3_08_2020a_B.x[8] = Template_v3_08_2020a_B.x[6];
      Template_v3_08_2020a_B.x[6] = u[8];
    }
  }

  Template_v3_08_2020a_B.absx31 = Template_v3_08_2020a_B.x[1] /
    Template_v3_08_2020a_B.x[0];
  Template_v3_08_2020a_B.x[1] = Template_v3_08_2020a_B.absx31;
  Template_v3_08_2020a_B.absx11 = Template_v3_08_2020a_B.x[2] /
    Template_v3_08_2020a_B.x[0];
  Template_v3_08_2020a_B.x[2] = Template_v3_08_2020a_B.absx11;
  Template_v3_08_2020a_B.x[4] -= Template_v3_08_2020a_B.absx31 *
    Template_v3_08_2020a_B.x[3];
  Template_v3_08_2020a_B.x[5] -= Template_v3_08_2020a_B.absx11 *
    Template_v3_08_2020a_B.x[3];
  Template_v3_08_2020a_B.x[7] -= Template_v3_08_2020a_B.absx31 *
    Template_v3_08_2020a_B.x[6];
  Template_v3_08_2020a_B.x[8] -= Template_v3_08_2020a_B.absx11 *
    Template_v3_08_2020a_B.x[6];
  if (fabs(Template_v3_08_2020a_B.x[5]) > fabs(Template_v3_08_2020a_B.x[4])) {
    Template_v3_08_2020a_B.itmp = Template_v3_08_2020a_B.p2;
    Template_v3_08_2020a_B.p2 = Template_v3_08_2020a_B.p3;
    Template_v3_08_2020a_B.p3 = Template_v3_08_2020a_B.itmp;
    Template_v3_08_2020a_B.x[1] = Template_v3_08_2020a_B.absx11;
    Template_v3_08_2020a_B.x[2] = Template_v3_08_2020a_B.absx31;
    Template_v3_08_2020a_B.absx11 = Template_v3_08_2020a_B.x[4];
    Template_v3_08_2020a_B.x[4] = Template_v3_08_2020a_B.x[5];
    Template_v3_08_2020a_B.x[5] = Template_v3_08_2020a_B.absx11;
    Template_v3_08_2020a_B.absx11 = Template_v3_08_2020a_B.x[7];
    Template_v3_08_2020a_B.x[7] = Template_v3_08_2020a_B.x[8];
    Template_v3_08_2020a_B.x[8] = Template_v3_08_2020a_B.absx11;
  }

  Template_v3_08_2020a_B.absx31 = Template_v3_08_2020a_B.x[5] /
    Template_v3_08_2020a_B.x[4];
  Template_v3_08_2020a_B.x[8] -= Template_v3_08_2020a_B.absx31 *
    Template_v3_08_2020a_B.x[7];
  Template_v3_08_2020a_B.absx11 = (Template_v3_08_2020a_B.absx31 *
    Template_v3_08_2020a_B.x[1] - Template_v3_08_2020a_B.x[2]) /
    Template_v3_08_2020a_B.x[8];
  Template_v3_08_2020a_B.absx21 = -(Template_v3_08_2020a_B.x[7] *
    Template_v3_08_2020a_B.absx11 + Template_v3_08_2020a_B.x[1]) /
    Template_v3_08_2020a_B.x[4];
  y[Template_v3_08_2020a_B.p1 - 1] = ((1.0 - Template_v3_08_2020a_B.x[3] *
    Template_v3_08_2020a_B.absx21) - Template_v3_08_2020a_B.x[6] *
    Template_v3_08_2020a_B.absx11) / Template_v3_08_2020a_B.x[0];
  y[Template_v3_08_2020a_B.p1] = Template_v3_08_2020a_B.absx21;
  y[Template_v3_08_2020a_B.p1 + 1] = Template_v3_08_2020a_B.absx11;
  Template_v3_08_2020a_B.absx11 = -Template_v3_08_2020a_B.absx31 /
    Template_v3_08_2020a_B.x[8];
  Template_v3_08_2020a_B.absx21 = (1.0 - Template_v3_08_2020a_B.x[7] *
    Template_v3_08_2020a_B.absx11) / Template_v3_08_2020a_B.x[4];
  y[Template_v3_08_2020a_B.p2] = -(Template_v3_08_2020a_B.x[3] *
    Template_v3_08_2020a_B.absx21 + Template_v3_08_2020a_B.x[6] *
    Template_v3_08_2020a_B.absx11) / Template_v3_08_2020a_B.x[0];
  y[Template_v3_08_2020a_B.p2 + 1] = Template_v3_08_2020a_B.absx21;
  y[Template_v3_08_2020a_B.p2 + 2] = Template_v3_08_2020a_B.absx11;
  Template_v3_08_2020a_B.absx11 = 1.0 / Template_v3_08_2020a_B.x[8];
  Template_v3_08_2020a_B.absx21 = -Template_v3_08_2020a_B.x[7] *
    Template_v3_08_2020a_B.absx11 / Template_v3_08_2020a_B.x[4];
  y[Template_v3_08_2020a_B.p3] = -(Template_v3_08_2020a_B.x[3] *
    Template_v3_08_2020a_B.absx21 + Template_v3_08_2020a_B.x[6] *
    Template_v3_08_2020a_B.absx11) / Template_v3_08_2020a_B.x[0];
  y[Template_v3_08_2020a_B.p3 + 1] = Template_v3_08_2020a_B.absx21;
  y[Template_v3_08_2020a_B.p3 + 2] = Template_v3_08_2020a_B.absx11;
}

static void Template_v3__SystemCore_release(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(10U);
  }
}

static void Template_v3_0_SystemCore_delete(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Template_v3__SystemCore_release(obj);
}

static void matlabCodegenHandle_matlabCodeg(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Template_v3_0_SystemCore_delete(obj);
  }
}

static void Template_v_SystemCore_release_g(const
  codertarget_linux_blocks_Digi_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_gpioTerminate(26U);
  }
}

static void Template_v3_SystemCore_delete_g(const
  codertarget_linux_blocks_Digi_T *obj)
{
  Template_v_SystemCore_release_g(obj);
}

static void matlabCodegenHandle_matlabCod_g(codertarget_linux_blocks_Digi_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Template_v3_SystemCore_delete_g(obj);
  }
}

static void matlabCodegenHandle_matlabCo_g2(MoveArm_Torque_Template_v3_08_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void SystemCore_release_g2ofinwajqo(const raspi_internal_lsm9ds1Block_T_T
  *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    MW_I2C_Close(obj->i2cobj_A_G.MW_I2C_HANDLE);
    MW_I2C_Close(obj->i2cobj_MAG.MW_I2C_HANDLE);
  }
}

static void SystemCore_delete_g2ofinwajqob(const raspi_internal_lsm9ds1Block_T_T
  *obj)
{
  SystemCore_release_g2ofinwajqo(obj);
}

static void matlabCodegenHandl_g2ofinwajqob(raspi_internal_lsm9ds1Block_T_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_g2ofinwajqob(obj);
  }
}

static void SystemCore_releas_g2ofinwajqobn(j_codertarget_raspi_internal__T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void SystemCore_delet_g2ofinwajqobno(j_codertarget_raspi_internal__T *obj)
{
  SystemCore_releas_g2ofinwajqobn(obj);
}

static void matlabCodegenHan_g2ofinwajqobno(j_codertarget_raspi_internal__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delet_g2ofinwajqobno(obj);
  }
}

static void SystemCore_release_g2ofinwajqob(i_codertarget_raspi_internal__T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void SystemCore_delete_g2ofinwajqobn(i_codertarget_raspi_internal__T *obj)
{
  SystemCore_release_g2ofinwajqob(obj);
}

static void matlabCodegenHand_g2ofinwajqobn(i_codertarget_raspi_internal__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    SystemCore_delete_g2ofinwajqobn(obj);
  }
}

static void matlabCodegenHa_g2ofinwajqobno3(ReadArm_Position_Rates_Templa_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void SystemCore_relea_g2ofinwajqobno(const
  InitializeForTorque_Template__T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    terminate_dynamixel();
  }
}

static void T_SystemCore_dele_h(const InitializeForTorque_Template__T *obj)
{
  SystemCore_relea_g2ofinwajqobno(obj);
}

static void Template_v3_0_matlabCodegenHa_h(InitializeForTorque_Template__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    T_SystemCore_dele_h(obj);
  }
}

static void Template_v3_0_matlabCodegenHa_d(IdentifyPlatform_Template_v3__T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void Template__SystemCore_release_g2(const
  raspi_internal_PWMBlock_Templ_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    EXT_PWMBlock_terminate(obj->PinNumber);
  }
}

static void Template__SystemCore_delete_g2o(const
  raspi_internal_PWMBlock_Templ_T *obj)
{
  Template__SystemCore_release_g2(obj);
}

static void matlabCodegenHandle_matlabC_g2o(raspi_internal_PWMBlock_Templ_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    Template__SystemCore_delete_g2o(obj);
  }
}

static raspi_internal_lsm9ds1Block_T_T *Templ_lsm9ds1Block_lsm9ds1Block
  (raspi_internal_lsm9ds1Block_T_T *obj)
{
  raspi_internal_lsm9ds1Block_T_T *b_obj;
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

static void Template_v3_08_SystemCore_setup(raspi_internal_lsm9ds1Block_T_T *obj)
{
  MW_I2C_Mode_Type ModeType;
  uint32_T i2cname;
  uint8_T b_SwappedDataBytes[2];
  uint8_T CastedData;
  uint8_T SwappedDataBytes[2];
  obj->isSetupComplete = false;
  obj->isInitialized = 1;
  ModeType = MW_I2C_MASTER;
  i2cname = 1;
  obj->i2cobj_A_G.MW_I2C_HANDLE = MW_I2C_Open(i2cname, ModeType);
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
  ModeType = MW_I2C_MASTER;
  i2cname = 1;
  obj->i2cobj_MAG.MW_I2C_HANDLE = MW_I2C_Open(i2cname, ModeType);
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
void Template_v3_08_2020a_step0(void)  // Sample time: [0.0s, 0.0s]
{
  {                                    // Sample time: [0.0s, 0.0s]
    rate_monotonic_scheduler();
  }

  {
    char_T *sErr;
    static const int8_T b[8] = { -1, -1, 0, 0, 1, 1, 0, 0 };

    static const int8_T c[8] = { 0, 0, 1, 1, 0, 0, -1, -1 };

    real_T *rtb_Delay_0;

    // If: '<Root>/Separate Phases' incorporates:
    //   Constant: '<Root>/Constant'
    //   Constant: '<Root>/Constant1'
    //   Constant: '<Root>/Constant2'
    //   Constant: '<Root>/Constant3'
    //   Constant: '<Root>/Constant4'
    //   Constant: '<Root>/Constant6'
    //   Constant: '<S661>/Constant'
    //   DataStoreRead: '<S16>/Data Store Read'
    //   DataStoreRead: '<S20>/Universal_Time'
    //   If: '<S16>/This IF block determines whether or not to run the BLACK sim//exp'

    if (Template_v3_08_2020a_DW.Univ_Time < Template_v3_08_2020a_P.Phase0_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #0:  Wait for Synchronization' incorporates:
      //   ActionPort: '<S10>/Action Port'

      Phase0WaitforSynchronizatio
        (&Template_v3_08_2020a_P.Phase0WaitforSynchronization,
         &Template_v3_08_2020a_DW.BLACK_Fx, &Template_v3_08_2020a_DW.BLACK_Fy,
         &Template_v3_08_2020a_DW.BLACK_Tz, &Template_v3_08_2020a_DW.BLUE_Fx,
         &Template_v3_08_2020a_DW.BLUE_Fy, &Template_v3_08_2020a_DW.BLUE_Tz,
         &Template_v3_08_2020a_DW.Float_State, &Template_v3_08_2020a_DW.RED_Fx,
         &Template_v3_08_2020a_DW.RED_Fy, &Template_v3_08_2020a_DW.RED_Tz,
         &Template_v3_08_2020a_DW.RED_Tz_Elbow,
         &Template_v3_08_2020a_B.Subtract_o,
         &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
         &Template_v3_08_2020a_DW.RED_Tz_Wrist, &Template_v3_08_2020a_DW.WhoAmI);

      // End of Outputs for SubSystem: '<Root>/Phase #0:  Wait for Synchronization' 
    } else if (Template_v3_08_2020a_DW.Univ_Time <
               Template_v3_08_2020a_P.Phase1_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #1:  Start Floating ' incorporates:
      //   ActionPort: '<S11>/Action Port'

      Phase0WaitforSynchronizatio(&Template_v3_08_2020a_P.Phase1StartFloating,
        &Template_v3_08_2020a_DW.BLACK_Fx, &Template_v3_08_2020a_DW.BLACK_Fy,
        &Template_v3_08_2020a_DW.BLACK_Tz, &Template_v3_08_2020a_DW.BLUE_Fx,
        &Template_v3_08_2020a_DW.BLUE_Fy, &Template_v3_08_2020a_DW.BLUE_Tz,
        &Template_v3_08_2020a_DW.Float_State, &Template_v3_08_2020a_DW.RED_Fx,
        &Template_v3_08_2020a_DW.RED_Fy, &Template_v3_08_2020a_DW.RED_Tz,
        &Template_v3_08_2020a_DW.RED_Tz_Elbow,
        &Template_v3_08_2020a_B.Subtract_o,
        &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
        &Template_v3_08_2020a_DW.RED_Tz_Wrist, &Template_v3_08_2020a_DW.WhoAmI);

      // End of Outputs for SubSystem: '<Root>/Phase #1:  Start Floating '
    } else if (Template_v3_08_2020a_DW.Univ_Time <
               Template_v3_08_2020a_P.Phase2_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' incorporates:
      //   ActionPort: '<S12>/Action Port'

      // If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S229>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S226>/Action Port'

        // Sum: '<S226>/Subtract' incorporates:
        //   Constant: '<S226>/Desired Px (BLACK)'
        //   DataStoreRead: '<S226>/BLACK_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.init_states_BLACK[0] -
          Template_v3_08_2020a_DW.BLACK_Px;

        // Delay: '<S236>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_ns != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_km =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S236>/Sum6' incorporates:
        //   Delay: '<S236>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_km;

        // If: '<S236>/if we went through a "step"' incorporates:
        //   Inport: '<S237>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S236>/Hold this value' incorporates:
          //   ActionPort: '<S237>/Action Port'

          Template_v3_08_2020a_B.In1_ej = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S236>/Hold this value'
        }

        // End of If: '<S236>/if we went through a "step"'

        // Sum: '<S231>/Sum3' incorporates:
        //   DataStoreWrite: '<S226>/BLACK_Fx'
        //   Gain: '<S231>/kd_xb'
        //   Gain: '<S231>/kp_xb'
        //   Gain: '<S236>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_ej * Template_v3_08_2020a_P.Kd_xb +
          Template_v3_08_2020a_P.Kp_xb * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S226>/Subtract1' incorporates:
        //   Constant: '<S226>/Desired Py (BLACK)'
        //   DataStoreRead: '<S226>/BLACK_Py '

        Template_v3_08_2020a_B.Sum6_l =
          Template_v3_08_2020a_P.init_states_BLACK[1] -
          Template_v3_08_2020a_DW.BLACK_Py;

        // Delay: '<S238>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_a != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_o1 =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S238>/Sum6' incorporates:
        //   Delay: '<S238>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_o1;

        // If: '<S238>/if we went through a "step"' incorporates:
        //   Inport: '<S239>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S238>/Hold this value' incorporates:
          //   ActionPort: '<S239>/Action Port'

          Template_v3_08_2020a_B.In1_g = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S238>/Hold this value'
        }

        // End of If: '<S238>/if we went through a "step"'

        // Sum: '<S232>/Sum3' incorporates:
        //   DataStoreWrite: '<S226>/BLACK_Fy'
        //   Gain: '<S232>/kd_yb'
        //   Gain: '<S232>/kp_yb'
        //   Gain: '<S238>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_g * Template_v3_08_2020a_P.Kd_yb +
          Template_v3_08_2020a_P.Kp_yb * Template_v3_08_2020a_B.Sum6_l;

        // MATLAB Function: '<S233>/MATLAB Function2' incorporates:
        //   Constant: '<S226>/Desired Attitude (BLACK)'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.init_states_BLACK[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_o);

        // MATLAB Function: '<S233>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S226>/BLACK_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.BLACK_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_k);

        // Sum: '<S233>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_o.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_k.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_o.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_k.Ox[1];

        // MATLAB Function: '<S233>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_o.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_j);

        // Delay: '<S234>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_h != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_lg =
            Template_v3_08_2020a_B.sf_MATLABFunction4_j.e_out;
        }

        // Sum: '<S234>/Sum6' incorporates:
        //   Delay: '<S234>/Delay1'

        Template_v3_08_2020a_B.Sum6_d =
          Template_v3_08_2020a_B.sf_MATLABFunction4_j.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_lg;

        // If: '<S234>/if we went through a "step"' incorporates:
        //   Inport: '<S235>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S234>/Hold this value' incorporates:
          //   ActionPort: '<S235>/Action Port'

          Template_v3_08_2020a_B.In1_ds = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S234>/Hold this value'
        }

        // End of If: '<S234>/if we went through a "step"'

        // Sum: '<S230>/Sum3' incorporates:
        //   DataStoreWrite: '<S226>/BLACK_Tz'
        //   Gain: '<S230>/kd_tb'
        //   Gain: '<S230>/kp_tb'
        //   Gain: '<S234>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Tz = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_ds * Template_v3_08_2020a_P.Kd_tb +
          Template_v3_08_2020a_P.Kp_tb *
          Template_v3_08_2020a_B.sf_MATLABFunction4_j.e_out;

        // DataStoreWrite: '<S226>/Data Store Write3' incorporates:
        //   Constant: '<S226>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value;

        // Update for Delay: '<S236>/Delay1'
        Template_v3_08_2020a_DW.icLoad_ns = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_km =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S238>/Delay1'
        Template_v3_08_2020a_DW.icLoad_a = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_o1 = Template_v3_08_2020a_B.Sum6_l;

        // Update for Delay: '<S234>/Delay1'
        Template_v3_08_2020a_DW.icLoad_h = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_lg =
          Template_v3_08_2020a_B.sf_MATLABFunction4_j.e_out;

        // End of Outputs for SubSystem: '<S12>/Change BLACK Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S229>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S227>/Action Port'

        // Sum: '<S227>/Subtract' incorporates:
        //   Constant: '<S227>/Desired Px (BLUE)'
        //   DataStoreRead: '<S227>/BLUE_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.init_states_BLUE[0] -
          Template_v3_08_2020a_DW.BLUE_Px;

        // Delay: '<S249>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_dy != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_fb =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S249>/Sum6' incorporates:
        //   Delay: '<S249>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_fb;

        // If: '<S249>/if we went through a "step"' incorporates:
        //   Inport: '<S250>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S249>/Hold this value' incorporates:
          //   ActionPort: '<S250>/Action Port'

          Template_v3_08_2020a_B.In1_l = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S249>/Hold this value'
        }

        // End of If: '<S249>/if we went through a "step"'

        // Sum: '<S244>/Sum3' incorporates:
        //   DataStoreWrite: '<S227>/BLUE_Fx'
        //   Gain: '<S244>/kd_xb'
        //   Gain: '<S244>/kp_xb'
        //   Gain: '<S249>/divide by delta T'

        Template_v3_08_2020a_DW.BLUE_Fx = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_l * Template_v3_08_2020a_P.Kd_xblue +
          Template_v3_08_2020a_P.Kp_xblue * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S227>/Subtract1' incorporates:
        //   Constant: '<S227>/Desired Py (BLUE)'
        //   DataStoreRead: '<S227>/BLUE_Py '

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_P.init_states_BLUE
          [1] - Template_v3_08_2020a_DW.BLUE_Py;

        // Delay: '<S251>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_n5 != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_la =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S251>/Sum6' incorporates:
        //   Delay: '<S251>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_la;

        // If: '<S251>/if we went through a "step"' incorporates:
        //   Inport: '<S252>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S251>/Hold this value' incorporates:
          //   ActionPort: '<S252>/Action Port'

          Template_v3_08_2020a_B.In1_f1 = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S251>/Hold this value'
        }

        // End of If: '<S251>/if we went through a "step"'

        // Sum: '<S245>/Sum3' incorporates:
        //   DataStoreWrite: '<S227>/BLUE_Fy'
        //   Gain: '<S245>/kd_yb'
        //   Gain: '<S245>/kp_yb'
        //   Gain: '<S251>/divide by delta T'

        Template_v3_08_2020a_DW.BLUE_Fy = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_f1 * Template_v3_08_2020a_P.Kd_yblue +
          Template_v3_08_2020a_P.Kp_yblue * Template_v3_08_2020a_B.Sum6_l;

        // MATLAB Function: '<S246>/MATLAB Function2' incorporates:
        //   Constant: '<S227>/Desired Attitude (BLUE)'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.init_states_BLUE[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_m);

        // MATLAB Function: '<S246>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S227>/BLUE_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.BLUE_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_c);

        // Sum: '<S246>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_m.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_c.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_m.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_c.Ox[1];

        // MATLAB Function: '<S246>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_m.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_c);

        // Delay: '<S247>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_bp != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_n =
            Template_v3_08_2020a_B.sf_MATLABFunction4_c.e_out;
        }

        // Sum: '<S247>/Sum6' incorporates:
        //   Delay: '<S247>/Delay1'

        Template_v3_08_2020a_B.Sum6_d =
          Template_v3_08_2020a_B.sf_MATLABFunction4_c.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_n;

        // If: '<S247>/if we went through a "step"' incorporates:
        //   Inport: '<S248>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S247>/Hold this value' incorporates:
          //   ActionPort: '<S248>/Action Port'

          Template_v3_08_2020a_B.In1_fj = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S247>/Hold this value'
        }

        // End of If: '<S247>/if we went through a "step"'

        // Sum: '<S243>/Sum3' incorporates:
        //   DataStoreWrite: '<S227>/BLUE_Tz'
        //   Gain: '<S243>/kd_tb'
        //   Gain: '<S243>/kp_tb'
        //   Gain: '<S247>/divide by delta T'

        Template_v3_08_2020a_DW.BLUE_Tz = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_fj * Template_v3_08_2020a_P.Kd_tblue +
          Template_v3_08_2020a_P.Kp_tblue *
          Template_v3_08_2020a_B.sf_MATLABFunction4_c.e_out;

        // DataStoreWrite: '<S227>/Data Store Write3' incorporates:
        //   Constant: '<S227>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_o;

        // Update for Delay: '<S249>/Delay1'
        Template_v3_08_2020a_DW.icLoad_dy = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_fb =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S251>/Delay1'
        Template_v3_08_2020a_DW.icLoad_n5 = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_la = Template_v3_08_2020a_B.Sum6_l;

        // Update for Delay: '<S247>/Delay1'
        Template_v3_08_2020a_DW.icLoad_bp = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_n =
          Template_v3_08_2020a_B.sf_MATLABFunction4_c.e_out;

        // End of Outputs for SubSystem: '<S12>/Change BLUE Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S229>/Constant'
      //   DataStoreRead: '<S12>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S12>/Change RED Behavior' incorporates:
        //   ActionPort: '<S228>/Action Port'

        // MATLAB Function: '<S259>/MATLAB Function2' incorporates:
        //   Constant: '<S228>/Constant'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.init_states_RED[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_ol);

        // MATLAB Function: '<S259>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S228>/RED_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_kt);

        // Sum: '<S259>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_ol.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_kt.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_ol.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_kt.Ox[1];

        // MATLAB Function: '<S259>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_ol.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_b);

        // Delay: '<S262>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_jx != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_bs =
            Template_v3_08_2020a_B.sf_MATLABFunction4_b.e_out;
        }

        // Sum: '<S262>/Sum6' incorporates:
        //   Delay: '<S262>/Delay1'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_B.sf_MATLABFunction4_b.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_bs;

        // If: '<S262>/if we went through a "step"' incorporates:
        //   Inport: '<S263>/In1'

        if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
          // Outputs for IfAction SubSystem: '<S262>/Hold this value' incorporates:
          //   ActionPort: '<S263>/Action Port'

          Template_v3_08_2020a_B.In1_i5 = Template_v3_08_2020a_B.Subtract_o;

          // End of Outputs for SubSystem: '<S262>/Hold this value'
        }

        // End of If: '<S262>/if we went through a "step"'

        // Sum: '<S256>/Sum3' incorporates:
        //   DataStoreWrite: '<S228>/RED_Tz'
        //   Gain: '<S256>/kd_tr'
        //   Gain: '<S256>/kp_tr'
        //   Gain: '<S262>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Tz = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_i5 * Template_v3_08_2020a_P.Kd_tr +
          Template_v3_08_2020a_P.Kp_tr *
          Template_v3_08_2020a_B.sf_MATLABFunction4_b.e_out;

        // Sum: '<S228>/Subtract' incorporates:
        //   Constant: '<S228>/Constant1'
        //   DataStoreRead: '<S228>/RED_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.init_states_RED[0] -
          Template_v3_08_2020a_DW.RED_Px;

        // Delay: '<S264>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_l0 != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_cb =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S264>/Sum6' incorporates:
        //   Delay: '<S264>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_cb;

        // If: '<S264>/if we went through a "step"' incorporates:
        //   Inport: '<S265>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S264>/Hold this value' incorporates:
          //   ActionPort: '<S265>/Action Port'

          Template_v3_08_2020a_B.In1_jv = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S264>/Hold this value'
        }

        // End of If: '<S264>/if we went through a "step"'

        // Sum: '<S257>/Sum3' incorporates:
        //   DataStoreWrite: '<S228>/RED_Fx'
        //   Gain: '<S257>/kd_xr'
        //   Gain: '<S257>/kp_xr'
        //   Gain: '<S264>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_jv * Template_v3_08_2020a_P.Kd_xr +
          Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S228>/Subtract1' incorporates:
        //   Constant: '<S228>/Constant3'
        //   DataStoreRead: '<S228>/RED_Py '

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_P.init_states_RED[1]
          - Template_v3_08_2020a_DW.RED_Py;

        // Delay: '<S266>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_iu != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_ig =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S266>/Sum6' incorporates:
        //   Delay: '<S266>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_ig;

        // If: '<S266>/if we went through a "step"' incorporates:
        //   Inport: '<S267>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S266>/Hold this value' incorporates:
          //   ActionPort: '<S267>/Action Port'

          Template_v3_08_2020a_B.In1_jh = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S266>/Hold this value'
        }

        // End of If: '<S266>/if we went through a "step"'

        // Sum: '<S258>/Sum3' incorporates:
        //   DataStoreWrite: '<S228>/RED_Fy'
        //   Gain: '<S258>/kd_yr'
        //   Gain: '<S258>/kp_yr'
        //   Gain: '<S266>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_jh * Template_v3_08_2020a_P.Kd_yr +
          Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

        // DataStoreWrite: '<S228>/Data Store Write3' incorporates:
        //   Constant: '<S228>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_l;

        // If: '<S228>/If' incorporates:
        //   Constant: '<S228>/Constant4'

        if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
            (Template_v3_08_2020a_P.platformSelection == 5.0) ||
            (Template_v3_08_2020a_P.platformSelection == 10.0) ||
            (Template_v3_08_2020a_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S228>/RED+ARM' incorporates:
          //   ActionPort: '<S261>/Action Port'

          Template_v3_08_2020a_REDARM(&Template_v3_08_2020a_B.REDARM_j,
            &Template_v3_08_2020a_DW.REDARM_j, &Template_v3_08_2020a_P.REDARM_j,
            &Template_v3_08_2020a_DW.RED_Rz_Elbow,
            &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Rz_Wrist,
            &Template_v3_08_2020a_DW.RED_Tz_Elbow,
            &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S228>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S228>/RED' incorporates:
          //   ActionPort: '<S260>/Action Port'

          // DataStoreWrite: '<S260>/RED_Fx1' incorporates:
          //   Constant: '<S260>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Shoulder =
            Template_v3_08_2020a_P.Constant7_Value;

          // DataStoreWrite: '<S260>/RED_Fy1' incorporates:
          //   Constant: '<S260>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Elbow =
            Template_v3_08_2020a_P.Constant7_Value;

          // DataStoreWrite: '<S260>/RED_Tz1' incorporates:
          //   Constant: '<S260>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Wrist =
            Template_v3_08_2020a_P.Constant7_Value;

          // End of Outputs for SubSystem: '<S228>/RED'
        }

        // End of If: '<S228>/If'

        // Update for Delay: '<S262>/Delay1'
        Template_v3_08_2020a_DW.icLoad_jx = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_bs =
          Template_v3_08_2020a_B.sf_MATLABFunction4_b.e_out;

        // Update for Delay: '<S264>/Delay1'
        Template_v3_08_2020a_DW.icLoad_l0 = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_cb =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S266>/Delay1'
        Template_v3_08_2020a_DW.icLoad_iu = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_ig = Template_v3_08_2020a_B.Sum6_l;

        // End of Outputs for SubSystem: '<S12>/Change RED Behavior'
      }

      // End of If: '<S12>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    } else if (Template_v3_08_2020a_DW.Univ_Time <
               Template_v3_08_2020a_P.Phase3_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #3: Experiment' incorporates:
      //   ActionPort: '<S13>/Action Port'

      // If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S283>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S280>/Action Port'

        // If: '<S280>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S280>/Constant1'
        //   Constant: '<S280>/Constant2'
        //   Constant: '<S280>/Constant3'
        //   Constant: '<S280>/Constant4'
        //   DataStoreRead: '<S288>/Universal_Time'

        if (Template_v3_08_2020a_DW.Univ_Time <
            Template_v3_08_2020a_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S280>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S284>/Action Port'

          Template_v3_08_20_SubPhase1(&Template_v3_08_2020a_B.SubPhase1,
            &Template_v3_08_2020a_DW.SubPhase1,
            &Template_v3_08_2020a_P.SubPhase1, &Template_v3_08_2020a_DW.BLACK_Fx,
            &Template_v3_08_2020a_DW.BLACK_Fy, &Template_v3_08_2020a_DW.BLACK_Px,
            &Template_v3_08_2020a_DW.BLACK_Py, &Template_v3_08_2020a_DW.BLACK_Rz,
            &Template_v3_08_2020a_DW.BLACK_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S280>/Sub-Phase #1'
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S280>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S285>/Action Port'

          Template_v3_08_20_SubPhase1(&Template_v3_08_2020a_B.SubPhase2,
            &Template_v3_08_2020a_DW.SubPhase2,
            &Template_v3_08_2020a_P.SubPhase2, &Template_v3_08_2020a_DW.BLACK_Fx,
            &Template_v3_08_2020a_DW.BLACK_Fy, &Template_v3_08_2020a_DW.BLACK_Px,
            &Template_v3_08_2020a_DW.BLACK_Py, &Template_v3_08_2020a_DW.BLACK_Rz,
            &Template_v3_08_2020a_DW.BLACK_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S280>/Sub-Phase #2 '
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S280>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S286>/Action Port'

          Template_v3_08_20_SubPhase1(&Template_v3_08_2020a_B.SubPhase3,
            &Template_v3_08_2020a_DW.SubPhase3,
            &Template_v3_08_2020a_P.SubPhase3, &Template_v3_08_2020a_DW.BLACK_Fx,
            &Template_v3_08_2020a_DW.BLACK_Fy, &Template_v3_08_2020a_DW.BLACK_Px,
            &Template_v3_08_2020a_DW.BLACK_Py, &Template_v3_08_2020a_DW.BLACK_Rz,
            &Template_v3_08_2020a_DW.BLACK_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S280>/Sub-Phase #3 '
        } else {
          if (Template_v3_08_2020a_DW.Univ_Time <
              Template_v3_08_2020a_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S280>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S287>/Action Port'

            Template_v3_08_20_SubPhase1(&Template_v3_08_2020a_B.SubPhase4,
              &Template_v3_08_2020a_DW.SubPhase4,
              &Template_v3_08_2020a_P.SubPhase4,
              &Template_v3_08_2020a_DW.BLACK_Fx,
              &Template_v3_08_2020a_DW.BLACK_Fy,
              &Template_v3_08_2020a_DW.BLACK_Px,
              &Template_v3_08_2020a_DW.BLACK_Py,
              &Template_v3_08_2020a_DW.BLACK_Rz,
              &Template_v3_08_2020a_DW.BLACK_Tz,
              &Template_v3_08_2020a_DW.Float_State,
              &Template_v3_08_2020a_DW.Univ_Time);

            // End of Outputs for SubSystem: '<S280>/Sub-Phase #4'
          }
        }

        // End of If: '<S280>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change BLACK Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S283>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S281>/Action Port'

        // If: '<S281>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S281>/Constant1'
        //   Constant: '<S281>/Constant2'
        //   Constant: '<S281>/Constant3'
        //   Constant: '<S281>/Constant4'
        //   DataStoreRead: '<S361>/Universal_Time'

        if (Template_v3_08_2020a_DW.Univ_Time <
            Template_v3_08_2020a_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S281>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S357>/Action Port'

          Template_v3_08__SubPhase1_a(&Template_v3_08_2020a_B.SubPhase1_a,
            &Template_v3_08_2020a_DW.SubPhase1_a,
            &Template_v3_08_2020a_P.SubPhase1_a,
            &Template_v3_08_2020a_DW.BLACK_Px, &Template_v3_08_2020a_DW.BLACK_Py,
            &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
            &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
            &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S281>/Sub-Phase #1'
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S281>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S358>/Action Port'

          Template_v3_08__SubPhase1_a(&Template_v3_08_2020a_B.SubPhase2_m,
            &Template_v3_08_2020a_DW.SubPhase2_m,
            &Template_v3_08_2020a_P.SubPhase2_m,
            &Template_v3_08_2020a_DW.BLACK_Px, &Template_v3_08_2020a_DW.BLACK_Py,
            &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
            &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
            &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S281>/Sub-Phase #2 '
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S281>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S359>/Action Port'

          Template_v3_08__SubPhase1_a(&Template_v3_08_2020a_B.SubPhase3_p,
            &Template_v3_08_2020a_DW.SubPhase3_p,
            &Template_v3_08_2020a_P.SubPhase3_p,
            &Template_v3_08_2020a_DW.BLACK_Px, &Template_v3_08_2020a_DW.BLACK_Py,
            &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
            &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
            &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
            &Template_v3_08_2020a_DW.Float_State,
            &Template_v3_08_2020a_DW.Univ_Time);

          // End of Outputs for SubSystem: '<S281>/Sub-Phase #3 '
        } else {
          if (Template_v3_08_2020a_DW.Univ_Time <
              Template_v3_08_2020a_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S281>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S360>/Action Port'

            Template_v3_08__SubPhase1_a(&Template_v3_08_2020a_B.SubPhase4_f,
              &Template_v3_08_2020a_DW.SubPhase4_f,
              &Template_v3_08_2020a_P.SubPhase4_f,
              &Template_v3_08_2020a_DW.BLACK_Px,
              &Template_v3_08_2020a_DW.BLACK_Py,
              &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
              &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
              &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
              &Template_v3_08_2020a_DW.Float_State,
              &Template_v3_08_2020a_DW.Univ_Time);

            // End of Outputs for SubSystem: '<S281>/Sub-Phase #4'
          }
        }

        // End of If: '<S281>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change BLUE Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S283>/Constant'
      //   DataStoreRead: '<S13>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S13>/Change RED Behavior' incorporates:
        //   ActionPort: '<S282>/Action Port'

        // If: '<S282>/Experiment Sub-Phases' incorporates:
        //   Constant: '<S282>/Constant1'
        //   Constant: '<S282>/Constant2'
        //   Constant: '<S282>/Constant3'
        //   Constant: '<S282>/Constant4'
        //   DataStoreRead: '<S438>/Universal_Time'

        if (Template_v3_08_2020a_DW.Univ_Time <
            Template_v3_08_2020a_P.Phase3_SubPhase1_End) {
          // Outputs for IfAction SubSystem: '<S282>/Sub-Phase #1' incorporates:
          //   ActionPort: '<S434>/Action Port'

          // MATLAB Function: '<S468>/MATLAB Function' incorporates:
          //   Constant: '<S447>/Constant'
          //   Constant: '<S447>/Desired Rate (BLACK)'
          //   Constant: '<S467>/Constant1'
          //   DataStoreRead: '<S467>/Universal_Time'
          //   Product: '<S447>/Product'
          //   Sum: '<S447>/Sum'
          //   Sum: '<S467>/Subtract'

          Template_v_MATLABFunction_o
            (Template_v3_08_2020a_P.DesiredRateBLACK_Value *
             (Template_v3_08_2020a_DW.Univ_Time -
              Template_v3_08_2020a_P.Phase2_End) +
             Template_v3_08_2020a_P.Constant_Value_p,
             &Template_v3_08_2020a_B.sf_MATLABFunction_k);

          // MATLAB Function: '<S442>/MATLAB Function2'
          Template__MATLABFunction2_l
            (Template_v3_08_2020a_B.sf_MATLABFunction_k.wrappedAngle,
             &Template_v3_08_2020a_B.sf_MATLABFunction2_c);

          // MATLAB Function: '<S442>/MATLAB Function3' incorporates:
          //   DataStoreRead: '<S434>/RED_Rz'

          Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
            &Template_v3_08_2020a_B.sf_MATLABFunction3_f);

          // Sum: '<S442>/Subtract2'
          Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
            Template_v3_08_2020a_B.sf_MATLABFunction2_c.Ox[0] -
            Template_v3_08_2020a_B.sf_MATLABFunction3_f.Ox[0];
          Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
            Template_v3_08_2020a_B.sf_MATLABFunction2_c.Ox[1] -
            Template_v3_08_2020a_B.sf_MATLABFunction3_f.Ox[1];

          // MATLAB Function: '<S442>/MATLAB Function4'
          Template_v3_MATLABFunction4
            (Template_v3_08_2020a_B.sf_MATLABFunction2_c.Oy,
             Template_v3_08_2020a_B.rtb_Subtract2_f_f,
             &Template_v3_08_2020a_B.sf_MATLABFunction4_k);

          // Delay: '<S449>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_nw != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_g3 =
              Template_v3_08_2020a_B.sf_MATLABFunction4_k.e_out;
          }

          // Sum: '<S449>/Sum6' incorporates:
          //   Delay: '<S449>/Delay1'

          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_B.sf_MATLABFunction4_k.e_out -
            Template_v3_08_2020a_DW.Delay1_DSTATE_g3;

          // If: '<S449>/if we went through a "step"' incorporates:
          //   Inport: '<S450>/In1'

          if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
            // Outputs for IfAction SubSystem: '<S449>/Hold this value' incorporates:
            //   ActionPort: '<S450>/Action Port'

            Template_v3_08_2020a_B.In1_k = Template_v3_08_2020a_B.Subtract_o;

            // End of Outputs for SubSystem: '<S449>/Hold this value'
          }

          // End of If: '<S449>/if we went through a "step"'

          // Sum: '<S439>/Sum3' incorporates:
          //   DataStoreWrite: '<S434>/RED_Tz'
          //   Gain: '<S439>/kd_tr'
          //   Gain: '<S439>/kp_tr'
          //   Gain: '<S449>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Tz = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_k * Template_v3_08_2020a_P.Kd_tr +
            Template_v3_08_2020a_P.Kp_tr *
            Template_v3_08_2020a_B.sf_MATLABFunction4_k.e_out;

          // Sum: '<S448>/Subtract3' incorporates:
          //   Constant: '<S448>/Constant2'
          //   DataStoreRead: '<S470>/Universal_Time'

          Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.Univ_Time -
            Template_v3_08_2020a_P.Phase2_End;

          // MATLAB Function: '<S434>/MATLAB Function'
          Template_v_MATLABFunction_e(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction_en);

          // Sum: '<S434>/Subtract' incorporates:
          //   DataStoreRead: '<S434>/RED_Px3'

          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_B.sf_MATLABFunction_en.x -
            Template_v3_08_2020a_DW.RED_Px;

          // Delay: '<S451>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_gb != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_dv =
              Template_v3_08_2020a_B.Subtract_o;
          }

          // Sum: '<S451>/Sum6' incorporates:
          //   Delay: '<S451>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Subtract_o -
            Template_v3_08_2020a_DW.Delay1_DSTATE_dv;

          // If: '<S451>/if we went through a "step"' incorporates:
          //   Inport: '<S452>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S451>/Hold this value' incorporates:
            //   ActionPort: '<S452>/Action Port'

            Template_v3_08_2020a_B.In1_pe = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S451>/Hold this value'
          }

          // End of If: '<S451>/if we went through a "step"'

          // Sum: '<S440>/Sum3' incorporates:
          //   DataStoreWrite: '<S434>/RED_Fx'
          //   Gain: '<S440>/kd_xr'
          //   Gain: '<S440>/kp_xr'
          //   Gain: '<S451>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_pe * Template_v3_08_2020a_P.Kd_xr +
            Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

          // MATLAB Function: '<S434>/MATLAB Function1'
          Template_v3_MATLABFunction1(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction1_k);

          // Sum: '<S434>/Subtract1' incorporates:
          //   DataStoreRead: '<S434>/RED_Py '

          Template_v3_08_2020a_B.Sum6_l =
            Template_v3_08_2020a_B.sf_MATLABFunction1_k.y -
            Template_v3_08_2020a_DW.RED_Py;

          // Delay: '<S453>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_nx != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_o =
              Template_v3_08_2020a_B.Sum6_l;
          }

          // Sum: '<S453>/Sum6' incorporates:
          //   Delay: '<S453>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
            Template_v3_08_2020a_DW.Delay1_DSTATE_o;

          // If: '<S453>/if we went through a "step"' incorporates:
          //   Inport: '<S454>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S453>/Hold this value' incorporates:
            //   ActionPort: '<S454>/Action Port'

            Template_v3_08_2020a_B.In1_ay = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S453>/Hold this value'
          }

          // End of If: '<S453>/if we went through a "step"'

          // Sum: '<S441>/Sum3' incorporates:
          //   DataStoreWrite: '<S434>/RED_Fy'
          //   Gain: '<S441>/kd_yr'
          //   Gain: '<S441>/kp_yr'
          //   Gain: '<S453>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_ay * Template_v3_08_2020a_P.Kd_yr +
            Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

          // DataStoreWrite: '<S434>/Data Store Write3' incorporates:
          //   Constant: '<S434>/Puck State'

          Template_v3_08_2020a_DW.Float_State =
            Template_v3_08_2020a_P.PuckState_Value_c;

          // If: '<S434>/If' incorporates:
          //   Constant: '<S434>/Constant4'

          if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
              (Template_v3_08_2020a_P.platformSelection == 5.0) ||
              (Template_v3_08_2020a_P.platformSelection == 10.0) ||
              (Template_v3_08_2020a_P.platformSelection == 11.0)) {
            // Outputs for IfAction SubSystem: '<S434>/RED+ARM' incorporates:
            //   ActionPort: '<S446>/Action Port'

            Template_v3_08_2020a_REDARM(&Template_v3_08_2020a_B.REDARM_c,
              &Template_v3_08_2020a_DW.REDARM_c,
              &Template_v3_08_2020a_P.REDARM_c,
              &Template_v3_08_2020a_DW.RED_Rz_Elbow,
              &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
              &Template_v3_08_2020a_DW.RED_Rz_Wrist,
              &Template_v3_08_2020a_DW.RED_Tz_Elbow,
              &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
              &Template_v3_08_2020a_DW.RED_Tz_Wrist);

            // End of Outputs for SubSystem: '<S434>/RED+ARM'
          } else {
            // Outputs for IfAction SubSystem: '<S434>/RED' incorporates:
            //   ActionPort: '<S445>/Action Port'

            // DataStoreWrite: '<S445>/RED_Fx1' incorporates:
            //   Constant: '<S445>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Shoulder =
              Template_v3_08_2020a_P.Constant7_Value_l;

            // DataStoreWrite: '<S445>/RED_Fy1' incorporates:
            //   Constant: '<S445>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Elbow =
              Template_v3_08_2020a_P.Constant7_Value_l;

            // DataStoreWrite: '<S445>/RED_Tz1' incorporates:
            //   Constant: '<S445>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Wrist =
              Template_v3_08_2020a_P.Constant7_Value_l;

            // End of Outputs for SubSystem: '<S434>/RED'
          }

          // End of If: '<S434>/If'

          // Update for Delay: '<S449>/Delay1'
          Template_v3_08_2020a_DW.icLoad_nw = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_g3 =
            Template_v3_08_2020a_B.sf_MATLABFunction4_k.e_out;

          // Update for Delay: '<S451>/Delay1'
          Template_v3_08_2020a_DW.icLoad_gb = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_dv =
            Template_v3_08_2020a_B.Subtract_o;

          // Update for Delay: '<S453>/Delay1'
          Template_v3_08_2020a_DW.icLoad_nx = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_o =
            Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S282>/Sub-Phase #1'
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase2_End) {
          // Outputs for IfAction SubSystem: '<S282>/Sub-Phase #2 ' incorporates:
          //   ActionPort: '<S435>/Action Port'

          // MATLAB Function: '<S482>/MATLAB Function' incorporates:
          //   Constant: '<S471>/Constant'
          //   Constant: '<S471>/Desired Rate (BLACK)'
          //   Constant: '<S481>/Constant1'
          //   Constant: '<S481>/Constant2'
          //   Product: '<S471>/Product'
          //   Sum: '<S471>/Sum'
          //   Sum: '<S481>/Subtract'

          Template_v_MATLABFunction_o
            (Template_v3_08_2020a_P.DesiredRateBLACK_Value_i *
             (Template_v3_08_2020a_P.Phase3_SubPhase1_End -
              Template_v3_08_2020a_P.Phase2_End) +
             Template_v3_08_2020a_P.Constant_Value_jl,
             &Template_v3_08_2020a_B.sf_MATLABFunction_p);

          // MATLAB Function: '<S475>/MATLAB Function2'
          Template__MATLABFunction2_l
            (Template_v3_08_2020a_B.sf_MATLABFunction_p.wrappedAngle,
             &Template_v3_08_2020a_B.sf_MATLABFunction2_d);

          // MATLAB Function: '<S475>/MATLAB Function3' incorporates:
          //   DataStoreRead: '<S435>/RED_Rz'

          Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
            &Template_v3_08_2020a_B.sf_MATLABFunction3_p);

          // Sum: '<S475>/Subtract2'
          Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
            Template_v3_08_2020a_B.sf_MATLABFunction2_d.Ox[0] -
            Template_v3_08_2020a_B.sf_MATLABFunction3_p.Ox[0];
          Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
            Template_v3_08_2020a_B.sf_MATLABFunction2_d.Ox[1] -
            Template_v3_08_2020a_B.sf_MATLABFunction3_p.Ox[1];

          // MATLAB Function: '<S475>/MATLAB Function4'
          Template_v3_MATLABFunction4
            (Template_v3_08_2020a_B.sf_MATLABFunction2_d.Oy,
             Template_v3_08_2020a_B.rtb_Subtract2_f_f,
             &Template_v3_08_2020a_B.sf_MATLABFunction4_ju);

          // Delay: '<S484>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_jr != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_io =
              Template_v3_08_2020a_B.sf_MATLABFunction4_ju.e_out;
          }

          // Sum: '<S484>/Sum6' incorporates:
          //   Delay: '<S484>/Delay1'

          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_B.sf_MATLABFunction4_ju.e_out -
            Template_v3_08_2020a_DW.Delay1_DSTATE_io;

          // If: '<S484>/if we went through a "step"' incorporates:
          //   Inport: '<S485>/In1'

          if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
            // Outputs for IfAction SubSystem: '<S484>/Hold this value' incorporates:
            //   ActionPort: '<S485>/Action Port'

            Template_v3_08_2020a_B.In1_fl = Template_v3_08_2020a_B.Subtract_o;

            // End of Outputs for SubSystem: '<S484>/Hold this value'
          }

          // End of If: '<S484>/if we went through a "step"'

          // Sum: '<S472>/Sum3' incorporates:
          //   DataStoreWrite: '<S435>/RED_Tz'
          //   Gain: '<S472>/kd_tr'
          //   Gain: '<S472>/kp_tr'
          //   Gain: '<S484>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Tz = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_fl * Template_v3_08_2020a_P.Kd_tr +
            Template_v3_08_2020a_P.Kp_tr *
            Template_v3_08_2020a_B.sf_MATLABFunction4_ju.e_out;

          // Sum: '<S480>/Subtract3' incorporates:
          //   Constant: '<S480>/Constant2'
          //   DataStoreRead: '<S502>/Universal_Time'

          Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.Univ_Time -
            Template_v3_08_2020a_P.Phase2_End;

          // MATLAB Function: '<S435>/MATLAB Function'
          Template_v_MATLABFunction_g(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction_g);

          // Sum: '<S435>/Subtract' incorporates:
          //   DataStoreRead: '<S435>/RED_Px1'

          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_B.sf_MATLABFunction_g.x -
            Template_v3_08_2020a_DW.RED_Px;

          // Delay: '<S486>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_cw != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_l5 =
              Template_v3_08_2020a_B.Subtract_o;
          }

          // Sum: '<S486>/Sum6' incorporates:
          //   Delay: '<S486>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Subtract_o -
            Template_v3_08_2020a_DW.Delay1_DSTATE_l5;

          // If: '<S486>/if we went through a "step"' incorporates:
          //   Inport: '<S487>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S486>/Hold this value' incorporates:
            //   ActionPort: '<S487>/Action Port'

            Template_v3_08_2020a_B.In1_mg = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S486>/Hold this value'
          }

          // End of If: '<S486>/if we went through a "step"'

          // Sum: '<S473>/Sum3' incorporates:
          //   DataStoreWrite: '<S435>/RED_Fx'
          //   Gain: '<S473>/kd_xr'
          //   Gain: '<S473>/kp_xr'
          //   Gain: '<S486>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_mg * Template_v3_08_2020a_P.Kd_xr +
            Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

          // MATLAB Function: '<S435>/MATLAB Function1'
          Template_v3_MATLABFunction1(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction1_n);

          // Sum: '<S435>/Subtract2' incorporates:
          //   DataStoreRead: '<S435>/RED_Py '

          Template_v3_08_2020a_B.Sum6_l =
            Template_v3_08_2020a_B.sf_MATLABFunction1_n.y -
            Template_v3_08_2020a_DW.RED_Py;

          // Delay: '<S488>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_eu != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_ji =
              Template_v3_08_2020a_B.Sum6_l;
          }

          // Sum: '<S488>/Sum6' incorporates:
          //   Delay: '<S488>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
            Template_v3_08_2020a_DW.Delay1_DSTATE_ji;

          // If: '<S488>/if we went through a "step"' incorporates:
          //   Inport: '<S489>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S488>/Hold this value' incorporates:
            //   ActionPort: '<S489>/Action Port'

            Template_v3_08_2020a_B.In1_oz = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S488>/Hold this value'
          }

          // End of If: '<S488>/if we went through a "step"'

          // Sum: '<S474>/Sum3' incorporates:
          //   DataStoreWrite: '<S435>/RED_Fy'
          //   Gain: '<S474>/kd_yr'
          //   Gain: '<S474>/kp_yr'
          //   Gain: '<S488>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_oz * Template_v3_08_2020a_P.Kd_yr +
            Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

          // DataStoreWrite: '<S435>/Data Store Write3' incorporates:
          //   Constant: '<S435>/Puck State'

          Template_v3_08_2020a_DW.Float_State =
            Template_v3_08_2020a_P.PuckState_Value_d;

          // If: '<S435>/If' incorporates:
          //   Constant: '<S435>/Constant4'

          if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
              (Template_v3_08_2020a_P.platformSelection == 5.0) ||
              (Template_v3_08_2020a_P.platformSelection == 10.0) ||
              (Template_v3_08_2020a_P.platformSelection == 11.0)) {
            // Outputs for IfAction SubSystem: '<S435>/RED+ARM' incorporates:
            //   ActionPort: '<S479>/Action Port'

            Template_v3_08_2020a_REDARM(&Template_v3_08_2020a_B.REDARM_b,
              &Template_v3_08_2020a_DW.REDARM_b,
              &Template_v3_08_2020a_P.REDARM_b,
              &Template_v3_08_2020a_DW.RED_Rz_Elbow,
              &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
              &Template_v3_08_2020a_DW.RED_Rz_Wrist,
              &Template_v3_08_2020a_DW.RED_Tz_Elbow,
              &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
              &Template_v3_08_2020a_DW.RED_Tz_Wrist);

            // End of Outputs for SubSystem: '<S435>/RED+ARM'
          } else {
            // Outputs for IfAction SubSystem: '<S435>/RED' incorporates:
            //   ActionPort: '<S478>/Action Port'

            // DataStoreWrite: '<S478>/RED_Fx1' incorporates:
            //   Constant: '<S478>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Shoulder =
              Template_v3_08_2020a_P.Constant7_Value_c;

            // DataStoreWrite: '<S478>/RED_Fy1' incorporates:
            //   Constant: '<S478>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Elbow =
              Template_v3_08_2020a_P.Constant7_Value_c;

            // DataStoreWrite: '<S478>/RED_Tz1' incorporates:
            //   Constant: '<S478>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Wrist =
              Template_v3_08_2020a_P.Constant7_Value_c;

            // End of Outputs for SubSystem: '<S435>/RED'
          }

          // End of If: '<S435>/If'

          // Update for Delay: '<S484>/Delay1'
          Template_v3_08_2020a_DW.icLoad_jr = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_io =
            Template_v3_08_2020a_B.sf_MATLABFunction4_ju.e_out;

          // Update for Delay: '<S486>/Delay1'
          Template_v3_08_2020a_DW.icLoad_cw = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_l5 =
            Template_v3_08_2020a_B.Subtract_o;

          // Update for Delay: '<S488>/Delay1'
          Template_v3_08_2020a_DW.icLoad_eu = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_ji =
            Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S282>/Sub-Phase #2 '
        } else if (Template_v3_08_2020a_DW.Univ_Time <
                   Template_v3_08_2020a_P.Phase3_SubPhase3_End) {
          // Outputs for IfAction SubSystem: '<S282>/Sub-Phase #3 ' incorporates:
          //   ActionPort: '<S436>/Action Port'

          // Sum: '<S509>/Subtract3' incorporates:
          //   Constant: '<S509>/Constant2'
          //   DataStoreRead: '<S523>/Universal_Time'

          Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.Univ_Time -
            Template_v3_08_2020a_P.Phase2_End;

          // MATLAB Function: '<S436>/MATLAB Function'
          Template_v_MATLABFunction_g(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction_or);

          // Sum: '<S436>/Subtract' incorporates:
          //   DataStoreRead: '<S436>/RED_Px1'

          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_B.sf_MATLABFunction_or.x -
            Template_v3_08_2020a_DW.RED_Px;

          // Delay: '<S510>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_ca != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_k =
              Template_v3_08_2020a_B.Subtract_o;
          }

          // Sum: '<S510>/Sum6' incorporates:
          //   Delay: '<S510>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Subtract_o -
            Template_v3_08_2020a_DW.Delay1_DSTATE_k;

          // If: '<S510>/if we went through a "step"' incorporates:
          //   Inport: '<S511>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S510>/Hold this value' incorporates:
            //   ActionPort: '<S511>/Action Port'

            Template_v3_08_2020a_B.In1_n = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S510>/Hold this value'
          }

          // End of If: '<S510>/if we went through a "step"'

          // Sum: '<S503>/Sum3' incorporates:
          //   DataStoreWrite: '<S436>/RED_Fx'
          //   Gain: '<S503>/kd_xr'
          //   Gain: '<S503>/kp_xr'
          //   Gain: '<S510>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_n * Template_v3_08_2020a_P.Kd_xr +
            Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

          // MATLAB Function: '<S436>/MATLAB Function1'
          Template_v3_MATLABFunction1(Template_v3_08_2020a_B.Sum6_l,
            &Template_v3_08_2020a_B.sf_MATLABFunction1_nv);

          // Sum: '<S436>/Subtract2' incorporates:
          //   DataStoreRead: '<S436>/RED_Py '

          Template_v3_08_2020a_B.Sum6_l =
            Template_v3_08_2020a_B.sf_MATLABFunction1_nv.y -
            Template_v3_08_2020a_DW.RED_Py;

          // Delay: '<S512>/Delay1'
          if (Template_v3_08_2020a_DW.icLoad_n != 0) {
            Template_v3_08_2020a_DW.Delay1_DSTATE_b =
              Template_v3_08_2020a_B.Sum6_l;
          }

          // Sum: '<S512>/Sum6' incorporates:
          //   Delay: '<S512>/Delay1'

          Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
            Template_v3_08_2020a_DW.Delay1_DSTATE_b;

          // If: '<S512>/if we went through a "step"' incorporates:
          //   Inport: '<S513>/In1'

          if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
            // Outputs for IfAction SubSystem: '<S512>/Hold this value' incorporates:
            //   ActionPort: '<S513>/Action Port'

            Template_v3_08_2020a_B.In1_oy = Template_v3_08_2020a_B.Sum6_d;

            // End of Outputs for SubSystem: '<S512>/Hold this value'
          }

          // End of If: '<S512>/if we went through a "step"'

          // Sum: '<S504>/Sum3' incorporates:
          //   DataStoreWrite: '<S436>/RED_Fy'
          //   Gain: '<S504>/kd_yr'
          //   Gain: '<S504>/kp_yr'
          //   Gain: '<S512>/divide by delta T'

          Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
            * Template_v3_08_2020a_B.In1_oy * Template_v3_08_2020a_P.Kd_yr +
            Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

          // DataStoreWrite: '<S436>/Data Store Write3' incorporates:
          //   Constant: '<S436>/Puck State'

          Template_v3_08_2020a_DW.Float_State =
            Template_v3_08_2020a_P.PuckState_Value_lm;

          // If: '<S436>/If' incorporates:
          //   Constant: '<S436>/Constant1'

          if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
              (Template_v3_08_2020a_P.platformSelection == 5.0) ||
              (Template_v3_08_2020a_P.platformSelection == 10.0) ||
              (Template_v3_08_2020a_P.platformSelection == 11.0)) {
            // Outputs for IfAction SubSystem: '<S436>/RED+ARM' incorporates:
            //   ActionPort: '<S508>/Action Port'

            // Sum: '<S508>/Subtract4' incorporates:
            //   Constant: '<S508>/Constant'
            //   DataStoreRead: '<S508>/Data Store Read'

            Template_v3_08_2020a_B.uDLookupTable2 =
              Template_v3_08_2020a_DW.Univ_Time -
              Template_v3_08_2020a_P.Phase3_SubPhase2_End;

            // Sum: '<S508>/Subtract7' incorporates:
            //   DataStoreRead: '<S508>/RED_Px5'
            //   Lookup_n-D: '<S508>/1-D Lookup Table1'

            Template_v3_08_2020a_B.Sum6_d = look1_binlxpw
              (Template_v3_08_2020a_B.uDLookupTable2,
               Template_v3_08_2020a_P.uDLookupTable1_bp01Data,
               Template_v3_08_2020a_P.uDLookupTable1_tableData, 100U) -
              Template_v3_08_2020a_DW.RED_Rz_Elbow;

            // Delay: '<S517>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_im != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_a =
                Template_v3_08_2020a_B.Sum6_d;
            }

            // Sum: '<S517>/Sum6' incorporates:
            //   Delay: '<S517>/Delay1'

            Template_v3_08_2020a_B.Sum6_au = Template_v3_08_2020a_B.Sum6_d -
              Template_v3_08_2020a_DW.Delay1_DSTATE_a;

            // If: '<S517>/if we went through a "step"' incorporates:
            //   Inport: '<S518>/In1'

            if (Template_v3_08_2020a_B.Sum6_au != 0.0) {
              // Outputs for IfAction SubSystem: '<S517>/Hold this value' incorporates:
              //   ActionPort: '<S518>/Action Port'

              Template_v3_08_2020a_B.In1_f = Template_v3_08_2020a_B.Sum6_au;

              // End of Outputs for SubSystem: '<S517>/Hold this value'
            }

            // End of If: '<S517>/if we went through a "step"'

            // Sum: '<S514>/Sum3' incorporates:
            //   Gain: '<S514>/kd_elarm'
            //   Gain: '<S514>/kp_elarm'
            //   Gain: '<S517>/divide by delta T'

            Template_v3_08_2020a_B.u0 = 1.0 / Template_v3_08_2020a_P.baseRate *
              Template_v3_08_2020a_B.In1_f * Template_v3_08_2020a_P.Kd_elarm +
              Template_v3_08_2020a_P.Kp_elarm * Template_v3_08_2020a_B.Sum6_d;

            // Saturate: '<S508>/Saturation1'
            if (Template_v3_08_2020a_B.u0 > Template_v3_08_2020a_P.Tz_lim_elarm)
            {
              Template_v3_08_2020a_DW.RED_Tz_Elbow =
                Template_v3_08_2020a_P.Tz_lim_elarm;
            } else if (Template_v3_08_2020a_B.u0 <
                       -Template_v3_08_2020a_P.Tz_lim_elarm) {
              Template_v3_08_2020a_DW.RED_Tz_Elbow =
                -Template_v3_08_2020a_P.Tz_lim_elarm;
            } else {
              Template_v3_08_2020a_DW.RED_Tz_Elbow = Template_v3_08_2020a_B.u0;
            }

            // End of Saturate: '<S508>/Saturation1'

            // Sum: '<S508>/Subtract3' incorporates:
            //   DataStoreRead: '<S508>/RED_Px2'
            //   Lookup_n-D: '<S508>/1-D Lookup Table'

            Template_v3_08_2020a_B.Sum6_au = look1_binlxpw
              (Template_v3_08_2020a_B.uDLookupTable2,
               Template_v3_08_2020a_P.uDLookupTable_bp01Data,
               Template_v3_08_2020a_P.uDLookupTable_tableData, 100U) -
              Template_v3_08_2020a_DW.RED_Rz_Shoulder;

            // Delay: '<S519>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_eb != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_g =
                Template_v3_08_2020a_B.Sum6_au;
            }

            // Sum: '<S519>/Sum6' incorporates:
            //   Delay: '<S519>/Delay1'

            Template_v3_08_2020a_B.Sum6_i = Template_v3_08_2020a_B.Sum6_au -
              Template_v3_08_2020a_DW.Delay1_DSTATE_g;

            // If: '<S519>/if we went through a "step"' incorporates:
            //   Inport: '<S520>/In1'

            if (Template_v3_08_2020a_B.Sum6_i != 0.0) {
              // Outputs for IfAction SubSystem: '<S519>/Hold this value' incorporates:
              //   ActionPort: '<S520>/Action Port'

              Template_v3_08_2020a_B.In1_p4 = Template_v3_08_2020a_B.Sum6_i;

              // End of Outputs for SubSystem: '<S519>/Hold this value'
            }

            // End of If: '<S519>/if we went through a "step"'

            // Sum: '<S515>/Sum3' incorporates:
            //   Gain: '<S515>/kd_sharm'
            //   Gain: '<S515>/kp_sharm'
            //   Gain: '<S519>/divide by delta T'

            Template_v3_08_2020a_B.u0 = 1.0 / Template_v3_08_2020a_P.baseRate *
              Template_v3_08_2020a_B.In1_p4 * Template_v3_08_2020a_P.Kd_sharm +
              Template_v3_08_2020a_P.Kp_sharm * Template_v3_08_2020a_B.Sum6_au;

            // Saturate: '<S508>/Saturation'
            if (Template_v3_08_2020a_B.u0 > Template_v3_08_2020a_P.Tz_lim_sharm)
            {
              Template_v3_08_2020a_DW.RED_Tz_Shoulder =
                Template_v3_08_2020a_P.Tz_lim_sharm;
            } else if (Template_v3_08_2020a_B.u0 <
                       -Template_v3_08_2020a_P.Tz_lim_sharm) {
              Template_v3_08_2020a_DW.RED_Tz_Shoulder =
                -Template_v3_08_2020a_P.Tz_lim_sharm;
            } else {
              Template_v3_08_2020a_DW.RED_Tz_Shoulder =
                Template_v3_08_2020a_B.u0;
            }

            // End of Saturate: '<S508>/Saturation'

            // Lookup_n-D: '<S508>/1-D Lookup Table2'
            Template_v3_08_2020a_B.uDLookupTable2 = look1_binlxpw
              (Template_v3_08_2020a_B.uDLookupTable2,
               Template_v3_08_2020a_P.uDLookupTable2_bp01Data,
               Template_v3_08_2020a_P.uDLookupTable2_tableData, 100U);

            // Sum: '<S508>/Subtract8' incorporates:
            //   DataStoreRead: '<S508>/RED_Rz1'

            Template_v3_08_2020a_B.uDLookupTable2 -=
              Template_v3_08_2020a_DW.RED_Rz_Wrist;

            // Delay: '<S521>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_jf != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_aw =
                Template_v3_08_2020a_B.uDLookupTable2;
            }

            // Sum: '<S521>/Sum6' incorporates:
            //   Delay: '<S521>/Delay1'

            Template_v3_08_2020a_B.Sum6_i =
              Template_v3_08_2020a_B.uDLookupTable2 -
              Template_v3_08_2020a_DW.Delay1_DSTATE_aw;

            // If: '<S521>/if we went through a "step"' incorporates:
            //   Inport: '<S522>/In1'

            if (Template_v3_08_2020a_B.Sum6_i != 0.0) {
              // Outputs for IfAction SubSystem: '<S521>/Hold this value' incorporates:
              //   ActionPort: '<S522>/Action Port'

              Template_v3_08_2020a_B.In1_m5 = Template_v3_08_2020a_B.Sum6_i;

              // End of Outputs for SubSystem: '<S521>/Hold this value'
            }

            // End of If: '<S521>/if we went through a "step"'

            // Sum: '<S516>/Sum3' incorporates:
            //   Gain: '<S516>/kd_wrarm'
            //   Gain: '<S516>/kp_wrarm'
            //   Gain: '<S521>/divide by delta T'

            Template_v3_08_2020a_B.u0 = 1.0 / Template_v3_08_2020a_P.baseRate *
              Template_v3_08_2020a_B.In1_m5 * Template_v3_08_2020a_P.Kd_wrarm +
              Template_v3_08_2020a_P.Kp_wrarm *
              Template_v3_08_2020a_B.uDLookupTable2;

            // Saturate: '<S508>/Saturation3'
            if (Template_v3_08_2020a_B.u0 > Template_v3_08_2020a_P.Tz_lim_wrarm)
            {
              Template_v3_08_2020a_DW.RED_Tz_Wrist =
                Template_v3_08_2020a_P.Tz_lim_wrarm;
            } else if (Template_v3_08_2020a_B.u0 <
                       -Template_v3_08_2020a_P.Tz_lim_wrarm) {
              Template_v3_08_2020a_DW.RED_Tz_Wrist =
                -Template_v3_08_2020a_P.Tz_lim_wrarm;
            } else {
              Template_v3_08_2020a_DW.RED_Tz_Wrist = Template_v3_08_2020a_B.u0;
            }

            // End of Saturate: '<S508>/Saturation3'

            // Update for Delay: '<S517>/Delay1'
            Template_v3_08_2020a_DW.icLoad_im = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_a =
              Template_v3_08_2020a_B.Sum6_d;

            // Update for Delay: '<S519>/Delay1'
            Template_v3_08_2020a_DW.icLoad_eb = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_g =
              Template_v3_08_2020a_B.Sum6_au;

            // Update for Delay: '<S521>/Delay1'
            Template_v3_08_2020a_DW.icLoad_jf = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_aw =
              Template_v3_08_2020a_B.uDLookupTable2;

            // End of Outputs for SubSystem: '<S436>/RED+ARM'
          } else {
            // Outputs for IfAction SubSystem: '<S436>/RED' incorporates:
            //   ActionPort: '<S507>/Action Port'

            // DataStoreWrite: '<S507>/RED_Fx1' incorporates:
            //   Constant: '<S507>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Shoulder =
              Template_v3_08_2020a_P.Constant7_Value_k;

            // DataStoreWrite: '<S507>/RED_Fy1' incorporates:
            //   Constant: '<S507>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Elbow =
              Template_v3_08_2020a_P.Constant7_Value_k;

            // DataStoreWrite: '<S507>/RED_Tz1' incorporates:
            //   Constant: '<S507>/Constant7'

            Template_v3_08_2020a_DW.RED_Tz_Wrist =
              Template_v3_08_2020a_P.Constant7_Value_k;

            // End of Outputs for SubSystem: '<S436>/RED'
          }

          // End of If: '<S436>/If'

          // Update for Delay: '<S510>/Delay1'
          Template_v3_08_2020a_DW.icLoad_ca = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_k =
            Template_v3_08_2020a_B.Subtract_o;

          // Update for Delay: '<S512>/Delay1'
          Template_v3_08_2020a_DW.icLoad_n = 0U;
          Template_v3_08_2020a_DW.Delay1_DSTATE_b =
            Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S282>/Sub-Phase #3 '
        } else {
          if (Template_v3_08_2020a_DW.Univ_Time <
              Template_v3_08_2020a_P.Phase3_SubPhase4_End) {
            // Outputs for IfAction SubSystem: '<S282>/Sub-Phase #4' incorporates:
            //   ActionPort: '<S437>/Action Port'

            // MATLAB Function: '<S527>/MATLAB Function2' incorporates:
            //   Constant: '<S532>/Constant'
            //   DataStoreRead: '<S532>/Data Store Read'
            //   DataStoreRead: '<S532>/Data Store Read1'
            //   DataStoreRead: '<S532>/Data Store Read2'
            //   DataStoreRead: '<S532>/Data Store Read3'
            //   Sum: '<S532>/Subtract'
            //   Sum: '<S532>/Subtract1'
            //   Sum: '<S532>/Sum'
            //   Trigonometry: '<S532>/Atan2'

            Template__MATLABFunction2_l(Template_v3_08_20_rt_atan2d_snf
              (Template_v3_08_2020a_DW.BLACK_Py - Template_v3_08_2020a_DW.RED_Py,
               Template_v3_08_2020a_DW.BLACK_Px - Template_v3_08_2020a_DW.RED_Px)
              + Template_v3_08_2020a_P.Constant_Value_d,
              &Template_v3_08_2020a_B.sf_MATLABFunction2_p);

            // MATLAB Function: '<S527>/MATLAB Function3' incorporates:
            //   DataStoreRead: '<S437>/RED_Rz'

            Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
              &Template_v3_08_2020a_B.sf_MATLABFunction3_fd);

            // Sum: '<S527>/Subtract2'
            Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
              Template_v3_08_2020a_B.sf_MATLABFunction2_p.Ox[0] -
              Template_v3_08_2020a_B.sf_MATLABFunction3_fd.Ox[0];
            Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
              Template_v3_08_2020a_B.sf_MATLABFunction2_p.Ox[1] -
              Template_v3_08_2020a_B.sf_MATLABFunction3_fd.Ox[1];

            // MATLAB Function: '<S527>/MATLAB Function4'
            Template_v3_MATLABFunction4
              (Template_v3_08_2020a_B.sf_MATLABFunction2_p.Oy,
               Template_v3_08_2020a_B.rtb_Subtract2_f_f,
               &Template_v3_08_2020a_B.sf_MATLABFunction4_i);

            // Delay: '<S534>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_c != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_cn =
                Template_v3_08_2020a_B.sf_MATLABFunction4_i.e_out;
            }

            // Sum: '<S534>/Sum6' incorporates:
            //   Delay: '<S534>/Delay1'

            Template_v3_08_2020a_B.Subtract_o =
              Template_v3_08_2020a_B.sf_MATLABFunction4_i.e_out -
              Template_v3_08_2020a_DW.Delay1_DSTATE_cn;

            // If: '<S534>/if we went through a "step"' incorporates:
            //   Inport: '<S535>/In1'

            if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
              // Outputs for IfAction SubSystem: '<S534>/Hold this value' incorporates:
              //   ActionPort: '<S535>/Action Port'

              Template_v3_08_2020a_B.In1_op = Template_v3_08_2020a_B.Subtract_o;

              // End of Outputs for SubSystem: '<S534>/Hold this value'
            }

            // End of If: '<S534>/if we went through a "step"'

            // Sum: '<S524>/Sum3' incorporates:
            //   DataStoreWrite: '<S437>/RED_Tz'
            //   Gain: '<S524>/kd_tr'
            //   Gain: '<S524>/kp_tr'
            //   Gain: '<S534>/divide by delta T'

            Template_v3_08_2020a_DW.RED_Tz = 1.0 /
              Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_op *
              Template_v3_08_2020a_P.Kd_tr + Template_v3_08_2020a_P.Kp_tr *
              Template_v3_08_2020a_B.sf_MATLABFunction4_i.e_out;

            // Sum: '<S533>/Subtract3' incorporates:
            //   Constant: '<S533>/Constant2'
            //   DataStoreRead: '<S551>/Universal_Time'

            Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.Univ_Time -
              Template_v3_08_2020a_P.Phase2_End;

            // MATLAB Function: '<S437>/MATLAB Function'
            Template_v_MATLABFunction_e(Template_v3_08_2020a_B.Sum6_l,
              &Template_v3_08_2020a_B.sf_MATLABFunction_m);

            // Sum: '<S437>/Subtract' incorporates:
            //   DataStoreRead: '<S437>/RED_Px1'

            Template_v3_08_2020a_B.Subtract_o =
              Template_v3_08_2020a_B.sf_MATLABFunction_m.x -
              Template_v3_08_2020a_DW.RED_Px;

            // Delay: '<S536>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_i != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_l =
                Template_v3_08_2020a_B.Subtract_o;
            }

            // Sum: '<S536>/Sum6' incorporates:
            //   Delay: '<S536>/Delay1'

            Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Subtract_o -
              Template_v3_08_2020a_DW.Delay1_DSTATE_l;

            // If: '<S536>/if we went through a "step"' incorporates:
            //   Inport: '<S537>/In1'

            if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
              // Outputs for IfAction SubSystem: '<S536>/Hold this value' incorporates:
              //   ActionPort: '<S537>/Action Port'

              Template_v3_08_2020a_B.In1_d = Template_v3_08_2020a_B.Sum6_d;

              // End of Outputs for SubSystem: '<S536>/Hold this value'
            }

            // End of If: '<S536>/if we went through a "step"'

            // Sum: '<S525>/Sum3' incorporates:
            //   DataStoreWrite: '<S437>/RED_Fx'
            //   Gain: '<S525>/kd_xr'
            //   Gain: '<S525>/kp_xr'
            //   Gain: '<S536>/divide by delta T'

            Template_v3_08_2020a_DW.RED_Fx = 1.0 /
              Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_d *
              Template_v3_08_2020a_P.Kd_xr + Template_v3_08_2020a_P.Kp_xr *
              Template_v3_08_2020a_B.Subtract_o;

            // MATLAB Function: '<S437>/MATLAB Function1'
            Template_v3_MATLABFunction1(Template_v3_08_2020a_B.Sum6_l,
              &Template_v3_08_2020a_B.sf_MATLABFunction1_e);

            // Sum: '<S437>/Subtract1' incorporates:
            //   DataStoreRead: '<S437>/RED_Py '

            Template_v3_08_2020a_B.Sum6_l =
              Template_v3_08_2020a_B.sf_MATLABFunction1_e.y -
              Template_v3_08_2020a_DW.RED_Py;

            // Delay: '<S538>/Delay1'
            if (Template_v3_08_2020a_DW.icLoad_cg != 0) {
              Template_v3_08_2020a_DW.Delay1_DSTATE_lw =
                Template_v3_08_2020a_B.Sum6_l;
            }

            // Sum: '<S538>/Sum6' incorporates:
            //   Delay: '<S538>/Delay1'

            Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
              Template_v3_08_2020a_DW.Delay1_DSTATE_lw;

            // If: '<S538>/if we went through a "step"' incorporates:
            //   Inport: '<S539>/In1'

            if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
              // Outputs for IfAction SubSystem: '<S538>/Hold this value' incorporates:
              //   ActionPort: '<S539>/Action Port'

              Template_v3_08_2020a_B.In1_m = Template_v3_08_2020a_B.Sum6_d;

              // End of Outputs for SubSystem: '<S538>/Hold this value'
            }

            // End of If: '<S538>/if we went through a "step"'

            // Sum: '<S526>/Sum3' incorporates:
            //   DataStoreWrite: '<S437>/RED_Fy'
            //   Gain: '<S526>/kd_yr'
            //   Gain: '<S526>/kp_yr'
            //   Gain: '<S538>/divide by delta T'

            Template_v3_08_2020a_DW.RED_Fy = 1.0 /
              Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_m *
              Template_v3_08_2020a_P.Kd_yr + Template_v3_08_2020a_P.Kp_yr *
              Template_v3_08_2020a_B.Sum6_l;

            // DataStoreWrite: '<S437>/Data Store Write3' incorporates:
            //   Constant: '<S437>/Puck State'

            Template_v3_08_2020a_DW.Float_State =
              Template_v3_08_2020a_P.PuckState_Value_f;

            // If: '<S437>/If' incorporates:
            //   Constant: '<S437>/Constant3'

            if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
                (Template_v3_08_2020a_P.platformSelection == 5.0) ||
                (Template_v3_08_2020a_P.platformSelection == 10.0) ||
                (Template_v3_08_2020a_P.platformSelection == 11.0)) {
              // Outputs for IfAction SubSystem: '<S437>/RED+ARM' incorporates:
              //   ActionPort: '<S531>/Action Port'

              Template_v3_08_202_REDARM_a(&Template_v3_08_2020a_B.REDARM_a,
                &Template_v3_08_2020a_P.REDARM_a,
                &Template_v3_08_2020a_DW.RED_RzD_Elbow,
                &Template_v3_08_2020a_DW.RED_RzD_Shoulder,
                &Template_v3_08_2020a_DW.RED_RzD_Wrist,
                &Template_v3_08_2020a_DW.RED_Rz_Elbow,
                &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
                &Template_v3_08_2020a_DW.RED_Rz_Wrist,
                &Template_v3_08_2020a_DW.RED_Tz_Elbow,
                &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
                &Template_v3_08_2020a_DW.RED_Tz_Wrist);

              // End of Outputs for SubSystem: '<S437>/RED+ARM'
            } else {
              // Outputs for IfAction SubSystem: '<S437>/RED' incorporates:
              //   ActionPort: '<S530>/Action Port'

              // DataStoreWrite: '<S530>/RED_Fx1' incorporates:
              //   Constant: '<S530>/Constant7'

              Template_v3_08_2020a_DW.RED_Tz_Shoulder =
                Template_v3_08_2020a_P.Constant7_Value_f;

              // DataStoreWrite: '<S530>/RED_Fy1' incorporates:
              //   Constant: '<S530>/Constant7'

              Template_v3_08_2020a_DW.RED_Tz_Elbow =
                Template_v3_08_2020a_P.Constant7_Value_f;

              // DataStoreWrite: '<S530>/RED_Tz1' incorporates:
              //   Constant: '<S530>/Constant7'

              Template_v3_08_2020a_DW.RED_Tz_Wrist =
                Template_v3_08_2020a_P.Constant7_Value_f;

              // End of Outputs for SubSystem: '<S437>/RED'
            }

            // End of If: '<S437>/If'

            // Update for Delay: '<S534>/Delay1'
            Template_v3_08_2020a_DW.icLoad_c = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_cn =
              Template_v3_08_2020a_B.sf_MATLABFunction4_i.e_out;

            // Update for Delay: '<S536>/Delay1'
            Template_v3_08_2020a_DW.icLoad_i = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_l =
              Template_v3_08_2020a_B.Subtract_o;

            // Update for Delay: '<S538>/Delay1'
            Template_v3_08_2020a_DW.icLoad_cg = 0U;
            Template_v3_08_2020a_DW.Delay1_DSTATE_lw =
              Template_v3_08_2020a_B.Sum6_l;

            // End of Outputs for SubSystem: '<S282>/Sub-Phase #4'
          }
        }

        // End of If: '<S282>/Experiment Sub-Phases'
        // End of Outputs for SubSystem: '<S13>/Change RED Behavior'
      }

      // End of If: '<S13>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #3: Experiment'
    } else if (Template_v3_08_2020a_DW.Univ_Time <
               Template_v3_08_2020a_P.Phase4_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #4:  Return Home' incorporates:
      //   ActionPort: '<S14>/Action Port'

      // If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S555>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S552>/Action Port'

        // Sum: '<S552>/Subtract' incorporates:
        //   Constant: '<S552>/Desired Px (BLACK)'
        //   DataStoreRead: '<S552>/BLACK_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.home_states_BLACK[0] -
          Template_v3_08_2020a_DW.BLACK_Px;

        // Delay: '<S562>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_e != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_db =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S562>/Sum6' incorporates:
        //   Delay: '<S562>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_db;

        // If: '<S562>/if we went through a "step"' incorporates:
        //   Inport: '<S563>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S562>/Hold this value' incorporates:
          //   ActionPort: '<S563>/Action Port'

          Template_v3_08_2020a_B.In1_b = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S562>/Hold this value'
        }

        // End of If: '<S562>/if we went through a "step"'

        // Sum: '<S557>/Sum3' incorporates:
        //   DataStoreWrite: '<S552>/BLACK_Fx'
        //   Gain: '<S557>/kd_xb'
        //   Gain: '<S557>/kp_xb'
        //   Gain: '<S562>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_b * Template_v3_08_2020a_P.Kd_xb +
          Template_v3_08_2020a_P.Kp_xb * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S552>/Subtract1' incorporates:
        //   Constant: '<S552>/Desired Py (BLACK)'
        //   DataStoreRead: '<S552>/BLACK_Py '

        Template_v3_08_2020a_B.Sum6_l =
          Template_v3_08_2020a_P.home_states_BLACK[1] -
          Template_v3_08_2020a_DW.BLACK_Py;

        // Delay: '<S564>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_fr != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_dz =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S564>/Sum6' incorporates:
        //   Delay: '<S564>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_dz;

        // If: '<S564>/if we went through a "step"' incorporates:
        //   Inport: '<S565>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S564>/Hold this value' incorporates:
          //   ActionPort: '<S565>/Action Port'

          Template_v3_08_2020a_B.In1_o5 = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S564>/Hold this value'
        }

        // End of If: '<S564>/if we went through a "step"'

        // Sum: '<S558>/Sum3' incorporates:
        //   DataStoreWrite: '<S552>/BLACK_Fy'
        //   Gain: '<S558>/kd_yb'
        //   Gain: '<S558>/kp_yb'
        //   Gain: '<S564>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_o5 * Template_v3_08_2020a_P.Kd_yb +
          Template_v3_08_2020a_P.Kp_yb * Template_v3_08_2020a_B.Sum6_l;

        // MATLAB Function: '<S559>/MATLAB Function2' incorporates:
        //   Constant: '<S552>/Desired Attitude (BLACK)'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.home_states_BLACK[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_pt);

        // MATLAB Function: '<S559>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S552>/BLACK_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.BLACK_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_b);

        // Sum: '<S559>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_pt.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_b.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_pt.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_b.Ox[1];

        // MATLAB Function: '<S559>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_pt.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_l);

        // Delay: '<S560>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_o != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_h =
            Template_v3_08_2020a_B.sf_MATLABFunction4_l.e_out;
        }

        // Sum: '<S560>/Sum6' incorporates:
        //   Delay: '<S560>/Delay1'

        Template_v3_08_2020a_B.Sum6_d =
          Template_v3_08_2020a_B.sf_MATLABFunction4_l.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_h;

        // If: '<S560>/if we went through a "step"' incorporates:
        //   Inport: '<S561>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S560>/Hold this value' incorporates:
          //   ActionPort: '<S561>/Action Port'

          Template_v3_08_2020a_B.In1_ei = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S560>/Hold this value'
        }

        // End of If: '<S560>/if we went through a "step"'

        // Sum: '<S556>/Sum3' incorporates:
        //   DataStoreWrite: '<S552>/BLACK_Tz'
        //   Gain: '<S556>/kd_tb'
        //   Gain: '<S556>/kp_tb'
        //   Gain: '<S560>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Tz = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_ei * Template_v3_08_2020a_P.Kd_tb +
          Template_v3_08_2020a_P.Kp_tb *
          Template_v3_08_2020a_B.sf_MATLABFunction4_l.e_out;

        // DataStoreWrite: '<S552>/Data Store Write3' incorporates:
        //   Constant: '<S552>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_fj;

        // Update for Delay: '<S562>/Delay1'
        Template_v3_08_2020a_DW.icLoad_e = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_db =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S564>/Delay1'
        Template_v3_08_2020a_DW.icLoad_fr = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_dz = Template_v3_08_2020a_B.Sum6_l;

        // Update for Delay: '<S560>/Delay1'
        Template_v3_08_2020a_DW.icLoad_o = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_h =
          Template_v3_08_2020a_B.sf_MATLABFunction4_l.e_out;

        // End of Outputs for SubSystem: '<S14>/Change BLACK Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S555>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S553>/Action Port'

        Templa_ChangeBLUEBehavior_g
          (&Template_v3_08_2020a_B.ChangeBLUEBehavior_gj,
           &Template_v3_08_2020a_DW.ChangeBLUEBehavior_gj,
           &Template_v3_08_2020a_P.ChangeBLUEBehavior_gj,
           &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
           &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
           &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
           &Template_v3_08_2020a_DW.Float_State);

        // End of Outputs for SubSystem: '<S14>/Change BLUE Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S555>/Constant'
      //   DataStoreRead: '<S14>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S14>/Change RED Behavior' incorporates:
        //   ActionPort: '<S554>/Action Port'

        // MATLAB Function: '<S585>/MATLAB Function2' incorporates:
        //   Constant: '<S554>/Desired Px (BLACK)2'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.home_states_RED[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_i);

        // MATLAB Function: '<S585>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S554>/RED_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_c1);

        // Sum: '<S585>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_i.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_c1.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_i.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_c1.Ox[1];

        // MATLAB Function: '<S585>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_i.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_n);

        // Delay: '<S588>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_k != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_i =
            Template_v3_08_2020a_B.sf_MATLABFunction4_n.e_out;
        }

        // Sum: '<S588>/Sum6' incorporates:
        //   Delay: '<S588>/Delay1'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_B.sf_MATLABFunction4_n.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_i;

        // If: '<S588>/if we went through a "step"' incorporates:
        //   Inport: '<S589>/In1'

        if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
          // Outputs for IfAction SubSystem: '<S588>/Hold this value' incorporates:
          //   ActionPort: '<S589>/Action Port'

          Template_v3_08_2020a_B.In1_o = Template_v3_08_2020a_B.Subtract_o;

          // End of Outputs for SubSystem: '<S588>/Hold this value'
        }

        // End of If: '<S588>/if we went through a "step"'

        // Sum: '<S582>/Sum3' incorporates:
        //   DataStoreWrite: '<S554>/RED_Tz'
        //   Gain: '<S582>/kd_tr'
        //   Gain: '<S582>/kp_tr'
        //   Gain: '<S588>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Tz = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_o * Template_v3_08_2020a_P.Kd_tr +
          Template_v3_08_2020a_P.Kp_tr *
          Template_v3_08_2020a_B.sf_MATLABFunction4_n.e_out;

        // Sum: '<S554>/Subtract' incorporates:
        //   Constant: '<S554>/Desired Px (BLACK)'
        //   DataStoreRead: '<S554>/RED_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.home_states_RED[0] -
          Template_v3_08_2020a_DW.RED_Px;

        // Delay: '<S590>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_du != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_i3 =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S590>/Sum6' incorporates:
        //   Delay: '<S590>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_i3;

        // If: '<S590>/if we went through a "step"' incorporates:
        //   Inport: '<S591>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S590>/Hold this value' incorporates:
          //   ActionPort: '<S591>/Action Port'

          Template_v3_08_2020a_B.In1_c = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S590>/Hold this value'
        }

        // End of If: '<S590>/if we went through a "step"'

        // Sum: '<S583>/Sum3' incorporates:
        //   DataStoreWrite: '<S554>/RED_Fx'
        //   Gain: '<S583>/kd_xr'
        //   Gain: '<S583>/kp_xr'
        //   Gain: '<S590>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_c * Template_v3_08_2020a_P.Kd_xr +
          Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S554>/Subtract1' incorporates:
        //   Constant: '<S554>/Desired Px (BLACK)1'
        //   DataStoreRead: '<S554>/RED_Py '

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_P.home_states_RED[1]
          - Template_v3_08_2020a_DW.RED_Py;

        // Delay: '<S592>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_g != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_cl =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S592>/Sum6' incorporates:
        //   Delay: '<S592>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_cl;

        // If: '<S592>/if we went through a "step"' incorporates:
        //   Inport: '<S593>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S592>/Hold this value' incorporates:
          //   ActionPort: '<S593>/Action Port'

          Template_v3_08_2020a_B.In1_a = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S592>/Hold this value'
        }

        // End of If: '<S592>/if we went through a "step"'

        // Sum: '<S584>/Sum3' incorporates:
        //   DataStoreWrite: '<S554>/RED_Fy'
        //   Gain: '<S584>/kd_yr'
        //   Gain: '<S584>/kp_yr'
        //   Gain: '<S592>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_a * Template_v3_08_2020a_P.Kd_yr +
          Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

        // DataStoreWrite: '<S554>/Data Store Write3' incorporates:
        //   Constant: '<S554>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_od;

        // If: '<S554>/If' incorporates:
        //   Constant: '<S554>/Constant3'

        if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
            (Template_v3_08_2020a_P.platformSelection == 5.0) ||
            (Template_v3_08_2020a_P.platformSelection == 10.0) ||
            (Template_v3_08_2020a_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S554>/RED+ARM' incorporates:
          //   ActionPort: '<S587>/Action Port'

          Template_v3_08_202_REDARM_a(&Template_v3_08_2020a_B.REDARM_p,
            &Template_v3_08_2020a_P.REDARM_p,
            &Template_v3_08_2020a_DW.RED_RzD_Elbow,
            &Template_v3_08_2020a_DW.RED_RzD_Shoulder,
            &Template_v3_08_2020a_DW.RED_RzD_Wrist,
            &Template_v3_08_2020a_DW.RED_Rz_Elbow,
            &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Rz_Wrist,
            &Template_v3_08_2020a_DW.RED_Tz_Elbow,
            &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S554>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S554>/RED' incorporates:
          //   ActionPort: '<S586>/Action Port'

          // DataStoreWrite: '<S586>/RED_Fx1' incorporates:
          //   Constant: '<S586>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Shoulder =
            Template_v3_08_2020a_P.Constant7_Value_m;

          // DataStoreWrite: '<S586>/RED_Fy1' incorporates:
          //   Constant: '<S586>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Elbow =
            Template_v3_08_2020a_P.Constant7_Value_m;

          // DataStoreWrite: '<S586>/RED_Tz1' incorporates:
          //   Constant: '<S586>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Wrist =
            Template_v3_08_2020a_P.Constant7_Value_m;

          // End of Outputs for SubSystem: '<S554>/RED'
        }

        // End of If: '<S554>/If'

        // Update for Delay: '<S588>/Delay1'
        Template_v3_08_2020a_DW.icLoad_k = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_i =
          Template_v3_08_2020a_B.sf_MATLABFunction4_n.e_out;

        // Update for Delay: '<S590>/Delay1'
        Template_v3_08_2020a_DW.icLoad_du = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_i3 =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S592>/Delay1'
        Template_v3_08_2020a_DW.icLoad_g = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_cl = Template_v3_08_2020a_B.Sum6_l;

        // End of Outputs for SubSystem: '<S14>/Change RED Behavior'
      }

      // End of If: '<S14>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #4:  Return Home'
    } else if (Template_v3_08_2020a_DW.Univ_Time <
               Template_v3_08_2020a_P.Phase5_End) {
      // Outputs for IfAction SubSystem: '<Root>/Phase #5:  Hold Home' incorporates:
      //   ActionPort: '<S15>/Action Port'

      // If: '<S15>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
      //   Constant: '<S608>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S605>/Action Port'

        // Sum: '<S605>/Subtract' incorporates:
        //   Constant: '<S605>/Constant'
        //   DataStoreRead: '<S605>/BLACK_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.home_states_BLACK[0] -
          Template_v3_08_2020a_DW.BLACK_Px;

        // Delay: '<S615>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_d != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_j =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S615>/Sum6' incorporates:
        //   Delay: '<S615>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_j;

        // If: '<S615>/if we went through a "step"' incorporates:
        //   Inport: '<S616>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S615>/Hold this value' incorporates:
          //   ActionPort: '<S616>/Action Port'

          Template_v3_08_2020a_B.In1_it = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S615>/Hold this value'
        }

        // End of If: '<S615>/if we went through a "step"'

        // Sum: '<S610>/Sum3' incorporates:
        //   DataStoreWrite: '<S605>/BLACK_Fx'
        //   Gain: '<S610>/kd_xb'
        //   Gain: '<S610>/kp_xb'
        //   Gain: '<S615>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fx = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_it * Template_v3_08_2020a_P.Kd_xb +
          Template_v3_08_2020a_P.Kp_xb * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S605>/Subtract1' incorporates:
        //   Constant: '<S605>/Constant2'
        //   DataStoreRead: '<S605>/BLACK_Py '

        Template_v3_08_2020a_B.Sum6_l =
          Template_v3_08_2020a_P.home_states_BLACK[1] -
          Template_v3_08_2020a_DW.BLACK_Py;

        // Delay: '<S617>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_f != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_f =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S617>/Sum6' incorporates:
        //   Delay: '<S617>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_f;

        // If: '<S617>/if we went through a "step"' incorporates:
        //   Inport: '<S618>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S617>/Hold this value' incorporates:
          //   ActionPort: '<S618>/Action Port'

          Template_v3_08_2020a_B.In1_p = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S617>/Hold this value'
        }

        // End of If: '<S617>/if we went through a "step"'

        // Sum: '<S611>/Sum3' incorporates:
        //   DataStoreWrite: '<S605>/BLACK_Fy'
        //   Gain: '<S611>/kd_yb'
        //   Gain: '<S611>/kp_yb'
        //   Gain: '<S617>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Fy = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_p * Template_v3_08_2020a_P.Kd_yb +
          Template_v3_08_2020a_P.Kp_yb * Template_v3_08_2020a_B.Sum6_l;

        // MATLAB Function: '<S612>/MATLAB Function2' incorporates:
        //   Constant: '<S605>/Constant3'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.home_states_BLACK[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_g);

        // MATLAB Function: '<S612>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S605>/BLACK_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.BLACK_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_o);

        // Sum: '<S612>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_g.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_o.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_g.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_o.Ox[1];

        // MATLAB Function: '<S612>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_g.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_d);

        // Delay: '<S613>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_b != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_ci =
            Template_v3_08_2020a_B.sf_MATLABFunction4_d.e_out;
        }

        // Sum: '<S613>/Sum6' incorporates:
        //   Delay: '<S613>/Delay1'

        Template_v3_08_2020a_B.Sum6_d =
          Template_v3_08_2020a_B.sf_MATLABFunction4_d.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE_ci;

        // If: '<S613>/if we went through a "step"' incorporates:
        //   Inport: '<S614>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S613>/Hold this value' incorporates:
          //   ActionPort: '<S614>/Action Port'

          Template_v3_08_2020a_B.In1_j = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S613>/Hold this value'
        }

        // End of If: '<S613>/if we went through a "step"'

        // Sum: '<S609>/Sum3' incorporates:
        //   DataStoreWrite: '<S605>/BLACK_Tz'
        //   Gain: '<S609>/kd_tb'
        //   Gain: '<S609>/kp_tb'
        //   Gain: '<S613>/divide by delta T'

        Template_v3_08_2020a_DW.BLACK_Tz = 1.0 / Template_v3_08_2020a_P.baseRate
          * Template_v3_08_2020a_B.In1_j * Template_v3_08_2020a_P.Kd_tb +
          Template_v3_08_2020a_P.Kp_tb *
          Template_v3_08_2020a_B.sf_MATLABFunction4_d.e_out;

        // DataStoreWrite: '<S605>/Data Store Write3' incorporates:
        //   Constant: '<S605>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_g;

        // Update for Delay: '<S615>/Delay1'
        Template_v3_08_2020a_DW.icLoad_d = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_j =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S617>/Delay1'
        Template_v3_08_2020a_DW.icLoad_f = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_f = Template_v3_08_2020a_B.Sum6_l;

        // Update for Delay: '<S613>/Delay1'
        Template_v3_08_2020a_DW.icLoad_b = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_ci =
          Template_v3_08_2020a_B.sf_MATLABFunction4_d.e_out;

        // End of Outputs for SubSystem: '<S15>/Change BLACK Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the BLACK sim//exp' 

      // If: '<S15>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S608>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S606>/Action Port'

        Templa_ChangeBLUEBehavior_g(&Template_v3_08_2020a_B.ChangeBLUEBehavior_b,
          &Template_v3_08_2020a_DW.ChangeBLUEBehavior_b,
          &Template_v3_08_2020a_P.ChangeBLUEBehavior_b,
          &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
          &Template_v3_08_2020a_DW.BLUE_Px, &Template_v3_08_2020a_DW.BLUE_Py,
          &Template_v3_08_2020a_DW.BLUE_Rz, &Template_v3_08_2020a_DW.BLUE_Tz,
          &Template_v3_08_2020a_DW.Float_State);

        // End of Outputs for SubSystem: '<S15>/Change BLUE Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S15>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S608>/Constant'
      //   DataStoreRead: '<S15>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S15>/Change RED Behavior' incorporates:
        //   ActionPort: '<S607>/Action Port'

        // MATLAB Function: '<S638>/MATLAB Function2' incorporates:
        //   Constant: '<S607>/Desired Px (BLACK)2'

        Template__MATLABFunction2_o(Template_v3_08_2020a_P.home_states_RED[2],
          &Template_v3_08_2020a_B.sf_MATLABFunction2_cg);

        // MATLAB Function: '<S638>/MATLAB Function3' incorporates:
        //   DataStoreRead: '<S607>/RED_Rz'

        Template_v3_MATLABFunction3(Template_v3_08_2020a_DW.RED_Rz,
          &Template_v3_08_2020a_B.sf_MATLABFunction3_fp);

        // Sum: '<S638>/Subtract2'
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[0] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_cg.Ox[0] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_fp.Ox[0];
        Template_v3_08_2020a_B.rtb_Subtract2_f_f[1] =
          Template_v3_08_2020a_B.sf_MATLABFunction2_cg.Ox[1] -
          Template_v3_08_2020a_B.sf_MATLABFunction3_fp.Ox[1];

        // MATLAB Function: '<S638>/MATLAB Function4'
        Template_v3_MATLABFunction4
          (Template_v3_08_2020a_B.sf_MATLABFunction2_cg.Oy,
           Template_v3_08_2020a_B.rtb_Subtract2_f_f,
           &Template_v3_08_2020a_B.sf_MATLABFunction4_f);

        // Delay: '<S641>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE =
            Template_v3_08_2020a_B.sf_MATLABFunction4_f.e_out;
        }

        // Sum: '<S641>/Sum6' incorporates:
        //   Delay: '<S641>/Delay1'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_B.sf_MATLABFunction4_f.e_out -
          Template_v3_08_2020a_DW.Delay1_DSTATE;

        // If: '<S641>/if we went through a "step"' incorporates:
        //   Inport: '<S642>/In1'

        if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
          // Outputs for IfAction SubSystem: '<S641>/Hold this value' incorporates:
          //   ActionPort: '<S642>/Action Port'

          Template_v3_08_2020a_B.In1_i = Template_v3_08_2020a_B.Subtract_o;

          // End of Outputs for SubSystem: '<S641>/Hold this value'
        }

        // End of If: '<S641>/if we went through a "step"'

        // Sum: '<S635>/Sum3' incorporates:
        //   DataStoreWrite: '<S607>/RED_Tz'
        //   Gain: '<S635>/kd_tr'
        //   Gain: '<S635>/kp_tr'
        //   Gain: '<S641>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Tz = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_i * Template_v3_08_2020a_P.Kd_tr +
          Template_v3_08_2020a_P.Kp_tr *
          Template_v3_08_2020a_B.sf_MATLABFunction4_f.e_out;

        // Sum: '<S607>/Subtract' incorporates:
        //   Constant: '<S607>/Desired Px (BLACK)'
        //   DataStoreRead: '<S607>/RED_Px'

        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_P.home_states_RED[0] -
          Template_v3_08_2020a_DW.RED_Px;

        // Delay: '<S643>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_l != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_d =
            Template_v3_08_2020a_B.Subtract_o;
        }

        // Sum: '<S643>/Sum6' incorporates:
        //   Delay: '<S643>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Subtract_o -
          Template_v3_08_2020a_DW.Delay1_DSTATE_d;

        // If: '<S643>/if we went through a "step"' incorporates:
        //   Inport: '<S644>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S643>/Hold this value' incorporates:
          //   ActionPort: '<S644>/Action Port'

          Template_v3_08_2020a_B.In1_e = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S643>/Hold this value'
        }

        // End of If: '<S643>/if we went through a "step"'

        // Sum: '<S636>/Sum3' incorporates:
        //   DataStoreWrite: '<S607>/RED_Fx'
        //   Gain: '<S636>/kd_xr'
        //   Gain: '<S636>/kp_xr'
        //   Gain: '<S643>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fx = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_e * Template_v3_08_2020a_P.Kd_xr +
          Template_v3_08_2020a_P.Kp_xr * Template_v3_08_2020a_B.Subtract_o;

        // Sum: '<S607>/Subtract1' incorporates:
        //   Constant: '<S607>/Desired Px (BLACK)1'
        //   DataStoreRead: '<S607>/RED_Py '

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_P.home_states_RED[1]
          - Template_v3_08_2020a_DW.RED_Py;

        // Delay: '<S645>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_j != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_c =
            Template_v3_08_2020a_B.Sum6_l;
        }

        // Sum: '<S645>/Sum6' incorporates:
        //   Delay: '<S645>/Delay1'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l -
          Template_v3_08_2020a_DW.Delay1_DSTATE_c;

        // If: '<S645>/if we went through a "step"' incorporates:
        //   Inport: '<S646>/In1'

        if (Template_v3_08_2020a_B.Sum6_d != 0.0) {
          // Outputs for IfAction SubSystem: '<S645>/Hold this value' incorporates:
          //   ActionPort: '<S646>/Action Port'

          Template_v3_08_2020a_B.In1 = Template_v3_08_2020a_B.Sum6_d;

          // End of Outputs for SubSystem: '<S645>/Hold this value'
        }

        // End of If: '<S645>/if we went through a "step"'

        // Sum: '<S637>/Sum3' incorporates:
        //   DataStoreWrite: '<S607>/RED_Fy'
        //   Gain: '<S637>/kd_yr'
        //   Gain: '<S637>/kp_yr'
        //   Gain: '<S645>/divide by delta T'

        Template_v3_08_2020a_DW.RED_Fy = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1 * Template_v3_08_2020a_P.Kd_yr +
          Template_v3_08_2020a_P.Kp_yr * Template_v3_08_2020a_B.Sum6_l;

        // DataStoreWrite: '<S607>/Data Store Write3' incorporates:
        //   Constant: '<S607>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_e;

        // If: '<S607>/If' incorporates:
        //   Constant: '<S607>/Constant3'

        if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
            (Template_v3_08_2020a_P.platformSelection == 5.0) ||
            (Template_v3_08_2020a_P.platformSelection == 10.0) ||
            (Template_v3_08_2020a_P.platformSelection == 11.0)) {
          // Outputs for IfAction SubSystem: '<S607>/RED+ARM' incorporates:
          //   ActionPort: '<S640>/Action Port'

          Template_v3_08_202_REDARM_a(&Template_v3_08_2020a_B.REDARM_bo,
            &Template_v3_08_2020a_P.REDARM_bo,
            &Template_v3_08_2020a_DW.RED_RzD_Elbow,
            &Template_v3_08_2020a_DW.RED_RzD_Shoulder,
            &Template_v3_08_2020a_DW.RED_RzD_Wrist,
            &Template_v3_08_2020a_DW.RED_Rz_Elbow,
            &Template_v3_08_2020a_DW.RED_Rz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Rz_Wrist,
            &Template_v3_08_2020a_DW.RED_Tz_Elbow,
            &Template_v3_08_2020a_DW.RED_Tz_Shoulder,
            &Template_v3_08_2020a_DW.RED_Tz_Wrist);

          // End of Outputs for SubSystem: '<S607>/RED+ARM'
        } else {
          // Outputs for IfAction SubSystem: '<S607>/RED' incorporates:
          //   ActionPort: '<S639>/Action Port'

          // DataStoreWrite: '<S639>/RED_Fx1' incorporates:
          //   Constant: '<S639>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Shoulder =
            Template_v3_08_2020a_P.Constant7_Value_a;

          // DataStoreWrite: '<S639>/RED_Fy1' incorporates:
          //   Constant: '<S639>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Elbow =
            Template_v3_08_2020a_P.Constant7_Value_a;

          // DataStoreWrite: '<S639>/RED_Tz1' incorporates:
          //   Constant: '<S639>/Constant7'

          Template_v3_08_2020a_DW.RED_Tz_Wrist =
            Template_v3_08_2020a_P.Constant7_Value_a;

          // End of Outputs for SubSystem: '<S607>/RED'
        }

        // End of If: '<S607>/If'

        // Update for Delay: '<S641>/Delay1'
        Template_v3_08_2020a_DW.icLoad = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE =
          Template_v3_08_2020a_B.sf_MATLABFunction4_f.e_out;

        // Update for Delay: '<S643>/Delay1'
        Template_v3_08_2020a_DW.icLoad_l = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_d =
          Template_v3_08_2020a_B.Subtract_o;

        // Update for Delay: '<S645>/Delay1'
        Template_v3_08_2020a_DW.icLoad_j = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_c = Template_v3_08_2020a_B.Sum6_l;

        // End of Outputs for SubSystem: '<S15>/Change RED Behavior'
      }

      // End of If: '<S15>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #5:  Hold Home'
    } else {
      // Outputs for IfAction SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' incorporates:
      //   ActionPort: '<S16>/Action Port'

      if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change BLACK Behavior' incorporates:
        //   ActionPort: '<S658>/Action Port'

        // If: '<S16>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
        //   Constant: '<S658>/Constant3'
        //   Constant: '<S658>/Constant4'
        //   Constant: '<S658>/Constant5'
        //   Constant: '<S658>/Puck State'
        //   DataStoreWrite: '<S658>/BLACK_Fx'
        //   DataStoreWrite: '<S658>/BLACK_Fy'
        //   DataStoreWrite: '<S658>/BLACK_Tz'
        //   DataStoreWrite: '<S658>/Data Store Write3'

        Template_v3_08_2020a_DW.BLACK_Fx =
          Template_v3_08_2020a_P.Constant3_Value;
        Template_v3_08_2020a_DW.BLACK_Fy =
          Template_v3_08_2020a_P.Constant4_Value_e;
        Template_v3_08_2020a_DW.BLACK_Tz =
          Template_v3_08_2020a_P.Constant5_Value;
        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_gd;

        // End of Outputs for SubSystem: '<S16>/Change BLACK Behavior'
      }

      // If: '<S16>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
      //   Constant: '<S661>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change BLUE Behavior' incorporates:
        //   ActionPort: '<S659>/Action Port'

        Template_ChangeBLUEBehavior(&Template_v3_08_2020a_P.ChangeBLUEBehavior_k,
          &Template_v3_08_2020a_DW.BLUE_Fx, &Template_v3_08_2020a_DW.BLUE_Fy,
          &Template_v3_08_2020a_DW.BLUE_Tz, &Template_v3_08_2020a_DW.Float_State);

        // End of Outputs for SubSystem: '<S16>/Change BLUE Behavior'
      }

      // End of If: '<S16>/This IF block determines whether or not to run the BLUE sim//exp' 

      // If: '<S16>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
      //   Constant: '<S661>/Constant'
      //   DataStoreRead: '<S16>/Data Store Read'

      if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
          (Template_v3_08_2020a_P.simMode == 1.0)) {
        // Outputs for IfAction SubSystem: '<S16>/Change RED Behavior' incorporates:
        //   ActionPort: '<S660>/Action Port'

        // DataStoreWrite: '<S660>/RED_Fx' incorporates:
        //   Constant: '<S660>/Constant'

        Template_v3_08_2020a_DW.RED_Fx =
          Template_v3_08_2020a_P.Constant_Value_jq;

        // DataStoreWrite: '<S660>/RED_Fy' incorporates:
        //   Constant: '<S660>/Constant1'

        Template_v3_08_2020a_DW.RED_Fy =
          Template_v3_08_2020a_P.Constant1_Value_l;

        // DataStoreWrite: '<S660>/RED_Tz' incorporates:
        //   Constant: '<S660>/Constant2'

        Template_v3_08_2020a_DW.RED_Tz =
          Template_v3_08_2020a_P.Constant2_Value_m;

        // DataStoreWrite: '<S660>/Data Store Write' incorporates:
        //   Constant: '<S660>/Constant4'

        Template_v3_08_2020a_DW.RED_Tz_Shoulder =
          Template_v3_08_2020a_P.Constant4_Value_o;

        // DataStoreWrite: '<S660>/Data Store Write1' incorporates:
        //   Constant: '<S660>/Constant5'

        Template_v3_08_2020a_DW.RED_Tz_Elbow =
          Template_v3_08_2020a_P.Constant5_Value_f;

        // DataStoreWrite: '<S660>/Data Store Write2' incorporates:
        //   Constant: '<S660>/Constant6'

        Template_v3_08_2020a_DW.RED_Tz_Wrist =
          Template_v3_08_2020a_P.Constant6_Value;

        // DataStoreWrite: '<S660>/Data Store Write3' incorporates:
        //   Constant: '<S660>/Puck State'

        Template_v3_08_2020a_DW.Float_State =
          Template_v3_08_2020a_P.PuckState_Value_h;

        // End of Outputs for SubSystem: '<S16>/Change RED Behavior'
      }

      // End of If: '<S16>/This IF block determines whether or not to run the RED sim//exp ' 
      // End of Outputs for SubSystem: '<Root>/Phase #6:  Stop Floating and Spin Down RW' 
    }

    // End of If: '<Root>/Separate Phases'

    // If: '<Root>/If running a simulation,  grab the simulated states.' incorporates:
    //   Constant: '<S7>/Constant'
    //   DiscreteIntegrator: '<S666>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S666>/Velocity to Position'
    //   DiscreteIntegrator: '<S678>/Acceleration  to Velocity'
    //   DiscreteIntegrator: '<S678>/Velocity to Position'

    if (Template_v3_08_2020a_P.simMode == 1.0) {
      // Outputs for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' incorporates:
      //   ActionPort: '<S19>/Action Port'

      // DiscreteIntegrator: '<S666>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S662>/BLACK_Fx_Sat'
      //   MATLAB Function: '<S666>/MATLAB Function'
      //   SignalConversion generated from: '<S672>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLACK Dynamics/BLACK Dynamics Model/MATLAB Function': '<S672>:1' 
      // '<S672>:1:5' x_ddot     = zeros(3,1);
      // '<S672>:1:9' m_BLACK    = model_param(3);
      // '<S672>:1:10' I_BLACK    = model_param(4);
      // '<S672>:1:13' Fx        = control_inputs(1);
      // '<S672>:1:14' Fy        = control_inputs(2);
      // '<S672>:1:15' Tz        = control_inputs(3);
      // '<S672>:1:18' x_ddot(1) = Fx/m_BLACK;
      // '<S672>:1:19' x_ddot(2) = Fy/m_BLACK;
      // '<S672>:1:20' x_ddot(3) = Tz/I_BLACK;
      Template_v3_08_2020a_B.u0 = Template_v3_08_2020a_DW.BLACK_Fx_Sat /
        Template_v3_08_2020a_P.model_param[2] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_i +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[0];

      // DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_B.VelocitytoPosition_g =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_c *
        Template_v3_08_2020a_B.u0 +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[0];

      // Sum: '<S666>/Sum' incorporates:
      //   RandomNumber: '<S666>/Random Number'

      Template_v3_08_2020a_B.Sum_l[0] =
        Template_v3_08_2020a_B.VelocitytoPosition_g +
        Template_v3_08_2020a_DW.NextOutput;
      Template_v3_08_2020a_B.x_ddot_b[0] = Template_v3_08_2020a_B.u0;
      Template_v3_08_2020a_B.VelocitytoPosition[0] =
        Template_v3_08_2020a_B.VelocitytoPosition_g;

      // DiscreteIntegrator: '<S666>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S662>/BLACK_Fy_Sat'
      //   DiscreteIntegrator: '<S666>/Velocity to Position'
      //   MATLAB Function: '<S666>/MATLAB Function'
      //   SignalConversion generated from: '<S672>/ SFunction '

      Template_v3_08_2020a_B.u0 = Template_v3_08_2020a_DW.BLACK_Fy_Sat /
        Template_v3_08_2020a_P.model_param[2] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_i +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[1];

      // DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_B.VelocitytoPosition_g =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_c *
        Template_v3_08_2020a_B.u0 +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[1];

      // Sum: '<S666>/Sum' incorporates:
      //   RandomNumber: '<S666>/Random Number'

      Template_v3_08_2020a_B.Sum_l[1] =
        Template_v3_08_2020a_B.VelocitytoPosition_g +
        Template_v3_08_2020a_DW.NextOutput;
      Template_v3_08_2020a_B.x_ddot_b[1] = Template_v3_08_2020a_B.u0;
      Template_v3_08_2020a_B.VelocitytoPosition[1] =
        Template_v3_08_2020a_B.VelocitytoPosition_g;

      // DiscreteIntegrator: '<S666>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S662>/BLACK_Tz_Sat'
      //   DiscreteIntegrator: '<S666>/Velocity to Position'
      //   MATLAB Function: '<S666>/MATLAB Function'
      //   SignalConversion generated from: '<S672>/ SFunction '

      Template_v3_08_2020a_B.u0 = Template_v3_08_2020a_DW.BLACK_Tz_Sat /
        Template_v3_08_2020a_P.model_param[3] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_i +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[2];

      // DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_B.VelocitytoPosition_g =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_c *
        Template_v3_08_2020a_B.u0 +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[2];

      // Sum: '<S666>/Sum' incorporates:
      //   RandomNumber: '<S666>/Random Number'

      Template_v3_08_2020a_B.Sum_l[2] =
        Template_v3_08_2020a_B.VelocitytoPosition_g +
        Template_v3_08_2020a_DW.NextOutput;

      // Delay: '<S671>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_bb != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_p = Template_v3_08_2020a_B.Sum_l[0];
      }

      // Sum: '<S671>/Sum6' incorporates:
      //   Delay: '<S671>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Sum_l[0] -
        Template_v3_08_2020a_DW.Delay1_DSTATE_p;

      // If: '<S671>/if we went through a "step"' incorporates:
      //   Inport: '<S677>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S671>/Hold this value' incorporates:
        //   ActionPort: '<S677>/Action Port'

        Template_v3_08_2020a_B.In1_es = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S671>/Hold this value'
      }

      // End of If: '<S671>/if we went through a "step"'

      // Gain: '<S671>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vx'

      Template_v3_08_2020a_DW.BLACK_Vx = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_es;

      // Delay: '<S669>/Delay1' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vx'

      if (Template_v3_08_2020a_DW.icLoad_p != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_ja =
          Template_v3_08_2020a_DW.BLACK_Vx;
      }

      // Sum: '<S669>/Sum6' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vx'
      //   Delay: '<S669>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_DW.BLACK_Vx -
        Template_v3_08_2020a_DW.Delay1_DSTATE_ja;

      // If: '<S669>/if we went through a "step"' incorporates:
      //   Inport: '<S675>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S669>/Hold this value' incorporates:
        //   ActionPort: '<S675>/Action Port'

        Template_v3_08_2020a_B.In1_kz = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S669>/Hold this value'
      }

      // End of If: '<S669>/if we went through a "step"'

      // Sum: '<S662>/Sum1' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_IMU_Ax_I'
      //   Gain: '<S669>/divide by delta T'
      //   RandomNumber: '<S662>/Random Number1'

      Template_v3_08_2020a_DW.BLACK_IMU_Ax_I = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_kz +
        Template_v3_08_2020a_DW.NextOutput_p;

      // Delay: '<S667>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_lv != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_d4 = Template_v3_08_2020a_B.Sum_l
          [1];
      }

      // Sum: '<S667>/Sum6' incorporates:
      //   Delay: '<S667>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Sum_l[1] -
        Template_v3_08_2020a_DW.Delay1_DSTATE_d4;

      // If: '<S667>/if we went through a "step"' incorporates:
      //   Inport: '<S673>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S667>/Hold this value' incorporates:
        //   ActionPort: '<S673>/Action Port'

        Template_v3_08_2020a_B.In1_aw = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S667>/Hold this value'
      }

      // End of If: '<S667>/if we went through a "step"'

      // Gain: '<S667>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vy'

      Template_v3_08_2020a_DW.BLACK_Vy = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_aw;

      // Delay: '<S670>/Delay1' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vy'

      if (Template_v3_08_2020a_DW.icLoad_hu != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_j3 =
          Template_v3_08_2020a_DW.BLACK_Vy;
      }

      // Sum: '<S670>/Sum6' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vy'
      //   Delay: '<S670>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_DW.BLACK_Vy -
        Template_v3_08_2020a_DW.Delay1_DSTATE_j3;

      // If: '<S670>/if we went through a "step"' incorporates:
      //   Inport: '<S676>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S670>/Hold this value' incorporates:
        //   ActionPort: '<S676>/Action Port'

        Template_v3_08_2020a_B.In1_id = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S670>/Hold this value'
      }

      // End of If: '<S670>/if we went through a "step"'

      // Sum: '<S662>/Sum2' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_IMU_Ay_I'
      //   Gain: '<S670>/divide by delta T'
      //   RandomNumber: '<S662>/Random Number'

      Template_v3_08_2020a_DW.BLACK_IMU_Ay_I = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_id +
        Template_v3_08_2020a_DW.NextOutput_o;

      // Delay: '<S668>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_m != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_kmw =
          Template_v3_08_2020a_B.Sum_l[2];
      }

      // Sum: '<S668>/Sum6' incorporates:
      //   Delay: '<S668>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Sum_l[2] -
        Template_v3_08_2020a_DW.Delay1_DSTATE_kmw;

      // If: '<S668>/if we went through a "step"' incorporates:
      //   Inport: '<S674>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S668>/Hold this value' incorporates:
        //   ActionPort: '<S674>/Action Port'

        Template_v3_08_2020a_B.In1_b2 = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S668>/Hold this value'
      }

      // End of If: '<S668>/if we went through a "step"'

      // Gain: '<S668>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_RzD'

      Template_v3_08_2020a_DW.BLACK_RzD = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_b2;

      // Sum: '<S662>/Sum3' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_IMU_R'
      //   DataStoreWrite: '<S662>/BLACK_RzD'
      //   RandomNumber: '<S662>/Random Number2'

      Template_v3_08_2020a_DW.BLACK_IMU_R = Template_v3_08_2020a_DW.BLACK_RzD +
        Template_v3_08_2020a_DW.NextOutput_j;

      // DataStoreWrite: '<S662>/BLACK_Px'
      Template_v3_08_2020a_DW.BLACK_Px = Template_v3_08_2020a_B.Sum_l[0];

      // DataStoreWrite: '<S662>/BLACK_Py'
      Template_v3_08_2020a_DW.BLACK_Py = Template_v3_08_2020a_B.Sum_l[1];

      // DataStoreWrite: '<S662>/BLACK_Rz'
      Template_v3_08_2020a_DW.BLACK_Rz = Template_v3_08_2020a_B.Sum_l[2];

      // DiscreteIntegrator: '<S678>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S663>/BLUE_Fx_Sat '
      //   MATLAB Function: '<S678>/MATLAB Function'
      //   SignalConversion generated from: '<S684>/ SFunction '

      // MATLAB Function 'Simulate Plant Dynamics/BLUE Dynamics/BLUE  Dynamics Model/MATLAB Function': '<S684>:1' 
      // '<S684>:1:5' x_ddot     = zeros(3,1);
      // '<S684>:1:9' m_BLUE    = model_param(5);
      // '<S684>:1:10' I_BLUE    = model_param(6);
      // '<S684>:1:13' Fx        = control_inputs(1);
      // '<S684>:1:14' Fy        = control_inputs(2);
      // '<S684>:1:15' Tz        = control_inputs(3);
      // '<S684>:1:18' x_ddot(1) = Fx/m_BLUE;
      // '<S684>:1:19' x_ddot(2) = Fy/m_BLUE;
      // '<S684>:1:20' x_ddot(3) = Tz/I_BLUE;
      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_DW.BLUE_Fx_Sat /
        Template_v3_08_2020a_P.model_param[4] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_b +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[0];

      // DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_f *
        Template_v3_08_2020a_B.rtb_x_ddot_i_g +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[0];

      // Sum: '<S678>/Sum' incorporates:
      //   RandomNumber: '<S678>/Random Number'

      Template_v3_08_2020a_B.rtb_Sum_lh_idx_0 =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m +
        Template_v3_08_2020a_DW.NextOutput_g;
      Template_v3_08_2020a_B.rtb_x_ddot_i_idx_0 =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_nz =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;

      // DiscreteIntegrator: '<S678>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S663>/BLUE_Fy_Sat'
      //   DiscreteIntegrator: '<S678>/Velocity to Position'
      //   MATLAB Function: '<S678>/MATLAB Function'
      //   SignalConversion generated from: '<S684>/ SFunction '

      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_DW.BLUE_Fy_Sat /
        Template_v3_08_2020a_P.model_param[4] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_b +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[1];

      // DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_f *
        Template_v3_08_2020a_B.rtb_x_ddot_i_g +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[1];

      // Sum: '<S678>/Sum' incorporates:
      //   RandomNumber: '<S678>/Random Number'

      Template_v3_08_2020a_B.rtb_Sum_lh_idx_1 =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m +
        Template_v3_08_2020a_DW.NextOutput_g;
      Template_v3_08_2020a_B.rtb_x_ddot_i_idx_1 =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_p =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;

      // DiscreteIntegrator: '<S678>/Acceleration  to Velocity' incorporates:
      //   DataStoreRead: '<S663>/BLUE_Tz_Sat'
      //   DiscreteIntegrator: '<S678>/Velocity to Position'
      //   MATLAB Function: '<S678>/MATLAB Function'
      //   SignalConversion generated from: '<S684>/ SFunction '

      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_DW.BLUE_Tz_Sat /
        Template_v3_08_2020a_P.model_param[5] *
        Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_b +
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[2];

      // DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.VelocitytoPosition_gainval_f *
        Template_v3_08_2020a_B.rtb_x_ddot_i_g +
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[2];

      // Sum: '<S678>/Sum' incorporates:
      //   RandomNumber: '<S678>/Random Number'

      Template_v3_08_2020a_B.rtb_Sum_lh_idx_2 =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m +
        Template_v3_08_2020a_DW.NextOutput_g;

      // Delay: '<S683>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_dc != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_e =
          Template_v3_08_2020a_B.rtb_Sum_lh_idx_0;
      }

      // Sum: '<S683>/Sum6' incorporates:
      //   Delay: '<S683>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_0 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_e;

      // If: '<S683>/if we went through a "step"' incorporates:
      //   Inport: '<S689>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S683>/Hold this value' incorporates:
        //   ActionPort: '<S689>/Action Port'

        Template_v3_08_2020a_B.In1_adc = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S683>/Hold this value'
      }

      // End of If: '<S683>/if we went through a "step"'

      // Gain: '<S683>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vx'

      Template_v3_08_2020a_DW.BLUE_Vx = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_adc;

      // Delay: '<S681>/Delay1' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vx'

      if (Template_v3_08_2020a_DW.icLoad_ln != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_fi =
          Template_v3_08_2020a_DW.BLUE_Vx;
      }

      // End of Delay: '<S681>/Delay1'

      // Delay: '<S679>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_oc != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_bo =
          Template_v3_08_2020a_B.rtb_Sum_lh_idx_1;
      }

      // Sum: '<S679>/Sum6' incorporates:
      //   Delay: '<S679>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_1 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_bo;

      // If: '<S679>/if we went through a "step"' incorporates:
      //   Inport: '<S685>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S679>/Hold this value' incorporates:
        //   ActionPort: '<S685>/Action Port'

        Template_v3_08_2020a_B.In1_pr = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S679>/Hold this value'
      }

      // End of If: '<S679>/if we went through a "step"'

      // Gain: '<S679>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vy'

      Template_v3_08_2020a_DW.BLUE_Vy = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_pr;

      // Delay: '<S682>/Delay1' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vy'

      if (Template_v3_08_2020a_DW.icLoad_l1 != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_fq =
          Template_v3_08_2020a_DW.BLUE_Vy;
      }

      // End of Delay: '<S682>/Delay1'

      // Delay: '<S680>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_lz != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_od =
          Template_v3_08_2020a_B.rtb_Sum_lh_idx_2;
      }

      // Sum: '<S680>/Sum6' incorporates:
      //   Delay: '<S680>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_2 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_od;

      // If: '<S680>/if we went through a "step"' incorporates:
      //   Inport: '<S686>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S680>/Hold this value' incorporates:
        //   ActionPort: '<S686>/Action Port'

        Template_v3_08_2020a_B.In1_fv = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S680>/Hold this value'
      }

      // End of If: '<S680>/if we went through a "step"'

      // Gain: '<S680>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_RzD'

      Template_v3_08_2020a_DW.BLUE_RzD = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_fv;

      // Sum: '<S663>/Sum6' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_IMU_R'
      //   DataStoreWrite: '<S663>/BLUE_RzD'
      //   RandomNumber: '<S663>/Random Number5'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_DW.BLUE_RzD +
        Template_v3_08_2020a_DW.NextOutput_k;

      // DataStoreWrite: '<S663>/BLUE_Px '
      Template_v3_08_2020a_DW.BLUE_Px = Template_v3_08_2020a_B.rtb_Sum_lh_idx_0;

      // DataStoreWrite: '<S663>/BLUE_Py'
      Template_v3_08_2020a_DW.BLUE_Py = Template_v3_08_2020a_B.rtb_Sum_lh_idx_1;

      // DataStoreWrite: '<S663>/BLUE_Rz'
      Template_v3_08_2020a_DW.BLUE_Rz = Template_v3_08_2020a_B.rtb_Sum_lh_idx_2;

      // RelationalOperator: '<S690>/Compare' incorporates:
      //   Constant: '<S690>/Constant'
      //   DataStoreRead: '<S664>/Data Store Read1'

      Template_v3_08_2020a_B.rtb_Compare_j = (Template_v3_08_2020a_DW.BLUE_Rz ==
        Template_v3_08_2020a_P.Constant_Value_j);

      // DiscreteIntegrator: '<S664>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S664>/Data Store Read1'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator_IC_LOADI != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator_DSTATE =
          Template_v3_08_2020a_DW.BLUE_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_j &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator_PrevRese <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_j) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator_PrevRese == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator_DSTATE =
          Template_v3_08_2020a_DW.BLUE_Rz;
      }

      // RelationalOperator: '<S691>/Compare' incorporates:
      //   Constant: '<S691>/Constant'
      //   DataStoreRead: '<S664>/Data Store Read2'

      Template_v3_08_2020a_B.rtb_Compare_n_m = (Template_v3_08_2020a_DW.BLACK_Rz
        == Template_v3_08_2020a_P.Constant_Value_o);

      // DiscreteIntegrator: '<S664>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S664>/Data Store Read2'
      //   DataStoreRead: '<S664>/Data Store Read4'
      //   DataStoreWrite: '<S664>/RED_Px1'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LOAD != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTATE =
          Template_v3_08_2020a_DW.BLACK_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_n_m &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevRes <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_n_m) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevRes == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTATE =
          Template_v3_08_2020a_DW.BLACK_Rz;
      }

      Template_v3_08_2020a_DW.BLACK_IMU_Psi =
        Template_v3_08_2020a_P.DiscreteTimeIntegrator1_gain_aq *
        Template_v3_08_2020a_DW.BLACK_IMU_R +
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTATE;

      // End of DiscreteIntegrator: '<S664>/Discrete-Time Integrator1'

      // RelationalOperator: '<S692>/Compare' incorporates:
      //   Constant: '<S692>/Constant'
      //   DataStoreRead: '<S664>/Data Store Read5'

      Template_v3_08_2020a_B.rtb_Compare_f_m = (Template_v3_08_2020a_DW.RED_Rz ==
        Template_v3_08_2020a_P.Constant_Value_ih);

      // DiscreteIntegrator: '<S664>/Discrete-Time Integrator2' incorporates:
      //   DataStoreRead: '<S664>/Data Store Read5'
      //   DataStoreRead: '<S664>/Data Store Read6'
      //   DataStoreWrite: '<S664>/RED_Px2'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_IC_LOAD != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_DSTATE =
          Template_v3_08_2020a_DW.RED_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_f_m &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_PrevRes <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_f_m) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_PrevRes == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_DSTATE =
          Template_v3_08_2020a_DW.RED_Rz;
      }

      Template_v3_08_2020a_DW.RED_IMU_Psi =
        Template_v3_08_2020a_P.DiscreteTimeIntegrator2_gainval *
        Template_v3_08_2020a_DW.RED_IMU_R +
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_DSTATE;

      // End of DiscreteIntegrator: '<S664>/Discrete-Time Integrator2'

      // If: '<S665>/If' incorporates:
      //   Constant: '<S665>/Constant'
      //   DiscreteIntegrator: '<S704>/Acceleration  to Velocity'
      //   DiscreteIntegrator: '<S704>/Velocity to Position'

      if ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
          (Template_v3_08_2020a_P.platformSelection == 5.0) ||
          (Template_v3_08_2020a_P.platformSelection == 10.0) ||
          (Template_v3_08_2020a_P.platformSelection == 11.0)) {
        // Outputs for IfAction SubSystem: '<S665>/RED + ARM' incorporates:
        //   ActionPort: '<S693>/Action Port'

        // Delay: '<S695>/Delay'
        rtb_Delay_0 = &Template_v3_08_2020a_DW.Delay_DSTATE[0];

        // MATLAB Function: '<S695>/Inertia Function ' incorporates:
        //   MATLAB Function: '<S695>/Coriolis Function'

        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Inertia Function ': '<S698>:1' 
        // '<S698>:1:8' q0 = x(3);
        // '<S698>:1:9' q1 = x(4);
        // '<S698>:1:10' q2 = x(5);
        // '<S698>:1:11' q3 = x(6);
        // '<S698>:1:13' m0= model_param(1);
        // '<S698>:1:14' I0= model_param(2);
        // '<S698>:1:15' phi= model_param(7);
        // '<S698>:1:16' b0= model_param(8);
        // '<S698>:1:17' a1= model_param(9);
        // '<S698>:1:18' b1= model_param(10);
        // '<S698>:1:19' a2= model_param(11);
        // '<S698>:1:20' b2= model_param(12);
        // '<S698>:1:21' a3= model_param(13);
        // '<S698>:1:22' b3= model_param(14);
        // '<S698>:1:23' m1= model_param(15);
        // '<S698>:1:24' m2= model_param(16);
        // '<S698>:1:25' m3= model_param(17);
        // '<S698>:1:26' I1= model_param(18);
        // '<S698>:1:27' I2= model_param(19);
        // '<S698>:1:28' I3= model_param(20);
        // '<S698>:1:30' t2 = cos(q2);
        Template_v3_08_2020a_B.t2 = cos(rtb_Delay_0[4]);

        // '<S698>:1:31' t3 = cos(q3);
        Template_v3_08_2020a_B.Sum6_l = cos(rtb_Delay_0[5]);

        // '<S698>:1:32' t4 = a1+b1;
        Template_v3_08_2020a_B.t4_tmp = Template_v3_08_2020a_P.model_param[8] +
          Template_v3_08_2020a_P.model_param[9];

        // '<S698>:1:33' t5 = a2+b2;
        // '<S698>:1:34' t6 = a1.*m1;
        Template_v3_08_2020a_B.t6 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_P.model_param[14];

        // '<S698>:1:35' t7 = a2.*m2;
        Template_v3_08_2020a_B.t7_tmp = Template_v3_08_2020a_P.model_param[10] *
          Template_v3_08_2020a_P.model_param[15];

        // '<S698>:1:36' t8 = a2.*m3;
        Template_v3_08_2020a_B.t155 = Template_v3_08_2020a_P.model_param[10] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S698>:1:37' t9 = a3.*m3;
        Template_v3_08_2020a_B.t9_tmp = Template_v3_08_2020a_P.model_param[12] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S698>:1:38' t10 = b0.*m1;
        Template_v3_08_2020a_B.t7 = Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_P.model_param[14];

        // '<S698>:1:39' t11 = b0.*m2;
        Template_v3_08_2020a_B.t9 = Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_P.model_param[15];

        // '<S698>:1:40' t12 = b0.*m3;
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l =
          Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S698>:1:41' t13 = b2.*m3;
        Template_v3_08_2020a_B.t156 = Template_v3_08_2020a_P.model_param[11] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S698>:1:42' t14 = phi+q0;
        Template_v3_08_2020a_B.t14_tmp = Template_v3_08_2020a_P.model_param[6] +
          rtb_Delay_0[2];

        // '<S698>:1:43' t15 = q0+q1;
        Template_v3_08_2020a_B.t15_tmp = rtb_Delay_0[2] + rtb_Delay_0[3];

        // '<S698>:1:44' t16 = q2+q3;
        Template_v3_08_2020a_B.t4 = rtb_Delay_0[4] + rtb_Delay_0[5];

        // '<S698>:1:45' t17 = a1.^2;
        Template_v3_08_2020a_B.t17 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_P.model_param[8];

        // '<S698>:1:46' t18 = a2.^2;
        // '<S698>:1:47' t19 = a3.^2;
        // '<S698>:1:48' t20 = b0.^2;
        // '<S698>:1:49' t21 = b1.^2;
        Template_v3_08_2020a_B.t21 = Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_P.model_param[9];

        // '<S698>:1:50' t22 = b2.^2;
        // '<S698>:1:51' t48 = -phi;
        // '<S698>:1:52' t49 = -q1;
        // '<S698>:1:53' t50 = m0+m1+m2+m3;
        Template_v3_08_2020a_B.Sum6_d = ((Template_v3_08_2020a_P.model_param[0]
          + Template_v3_08_2020a_P.model_param[14]) +
          Template_v3_08_2020a_P.model_param[15]) +
          Template_v3_08_2020a_P.model_param[16];

        // '<S698>:1:54' t76 = a1.*b1.*m2.*2.0;
        // '<S698>:1:55' t77 = a1.*b1.*m3.*2.0;
        // '<S698>:1:56' t23 = a1.*t7;
        Template_v3_08_2020a_B.t23 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.t7_tmp;

        // '<S698>:1:57' t24 = a1.*t8;
        Template_v3_08_2020a_B.t24 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.t155;

        // '<S698>:1:58' t25 = a1.*t9;
        Template_v3_08_2020a_B.t25 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.t9_tmp;

        // '<S698>:1:59' t26 = a3.*t8;
        Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 =
          Template_v3_08_2020a_P.model_param[12] * Template_v3_08_2020a_B.t155;

        // '<S698>:1:60' t27 = b0.*t6;
        // '<S698>:1:61' t28 = a1.*t11;
        // '<S698>:1:62' t29 = a1.*t12;
        // '<S698>:1:63' t30 = b0.*t7;
        Template_v3_08_2020a_B.Sum6_au = Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_B.t7_tmp;

        // '<S698>:1:64' t31 = b0.*t8;
        Template_v3_08_2020a_B.uDLookupTable2 =
          Template_v3_08_2020a_P.model_param[7] * Template_v3_08_2020a_B.t155;

        // '<S698>:1:65' t32 = b1.*t7;
        Template_v3_08_2020a_B.Sum6_i = Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.t7_tmp;

        // '<S698>:1:66' t33 = a1.*t13;
        Template_v3_08_2020a_B.t33 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.t156;

        // '<S698>:1:67' t34 = b1.*t8;
        Template_v3_08_2020a_B.t34 = Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.t155;

        // '<S698>:1:68' t35 = b0.*t9;
        Template_v3_08_2020a_B.t35 = Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_B.t9_tmp;

        // '<S698>:1:69' t36 = b1.*t9;
        Template_v3_08_2020a_B.rtb_x_ddot_idx_1 =
          Template_v3_08_2020a_P.model_param[9] * Template_v3_08_2020a_B.t9_tmp;

        // '<S698>:1:70' t37 = b2.*t9;
        Template_v3_08_2020a_B.t37 = Template_v3_08_2020a_P.model_param[11] *
          Template_v3_08_2020a_B.t9_tmp;

        // '<S698>:1:71' t38 = b1.*t11;
        // '<S698>:1:72' t39 = b1.*t12;
        // '<S698>:1:73' t40 = b2.*t12;
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw =
          Template_v3_08_2020a_P.model_param[11] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S698>:1:74' t41 = b1.*t13;
        Template_v3_08_2020a_B.t41 = Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.t156;

        // '<S698>:1:75' t42 = cos(t14);
        // '<S698>:1:76' t43 = cos(t15);
        // '<S698>:1:77' t44 = cos(t16);
        Template_v3_08_2020a_B.t44 = cos(Template_v3_08_2020a_B.t4);

        // '<S698>:1:78' t45 = q2+t15;
        Template_v3_08_2020a_B.t45 = rtb_Delay_0[4] +
          Template_v3_08_2020a_B.t15_tmp;

        // '<S698>:1:79' t46 = sin(t14);
        // '<S698>:1:80' t47 = sin(t15);
        // '<S698>:1:81' t52 = t15+t16;
        Template_v3_08_2020a_B.t52 = Template_v3_08_2020a_B.t15_tmp +
          Template_v3_08_2020a_B.t4;

        // '<S698>:1:82' t54 = -t6;
        // '<S698>:1:83' t55 = -t7;
        // '<S698>:1:84' t56 = -t8;
        // '<S698>:1:85' t57 = -t9;
        // '<S698>:1:86' t58 = -t10;
        // '<S698>:1:87' t59 = -t11;
        // '<S698>:1:88' t60 = -t12;
        // '<S698>:1:89' t61 = -t13;
        // '<S698>:1:90' t62 = a1.*t6;
        // '<S698>:1:91' t63 = m2.*t17;
        // '<S698>:1:92' t64 = m3.*t17;
        // '<S698>:1:93' t65 = a2.*t7;
        // '<S698>:1:94' t66 = a2.*t8;
        // '<S698>:1:95' t67 = a3.*t9;
        // '<S698>:1:96' t69 = m2.*t21;
        // '<S698>:1:97' t70 = m3.*t21;
        // '<S698>:1:98' t71 = b2.*t13;
        // '<S698>:1:99' t81 = b2.*t8.*2.0;
        // '<S698>:1:100' t85 = m2.*t4;
        // '<S698>:1:101' t86 = m3.*t4;
        // '<S698>:1:102' t87 = m3.*t5;
        Template_v3_08_2020a_B.rtb_Sum_ht_idx_2 =
          Template_v3_08_2020a_P.model_param[10] +
          Template_v3_08_2020a_P.model_param[11];

        // '<S698>:1:103' t94 = phi+t49;
        // '<S698>:1:104' t95 = q1+q2+t48;
        // '<S698>:1:105' t101 = q1+t16+t48;
        // '<S698>:1:106' t106 = t10+t11+t12;
        Template_v3_08_2020a_B.t106 = (Template_v3_08_2020a_B.t7 +
          Template_v3_08_2020a_B.t9) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S698>:1:107' t51 = cos(t45);
        Template_v3_08_2020a_B.t51 = cos(Template_v3_08_2020a_B.t45);

        // '<S698>:1:108' t53 = sin(t45);
        Template_v3_08_2020a_B.t53 = sin(Template_v3_08_2020a_B.t45);

        // '<S698>:1:109' t68 = t67;
        // '<S698>:1:110' t72 = t23.*2.0;
        // '<S698>:1:111' t73 = t24.*2.0;
        // '<S698>:1:112' t74 = t25.*2.0;
        // '<S698>:1:113' t75 = t26.*2.0;
        // '<S698>:1:114' t78 = t32.*2.0;
        // '<S698>:1:115' t79 = t33.*2.0;
        // '<S698>:1:116' t80 = t34.*2.0;
        // '<S698>:1:117' t82 = t36.*2.0;
        // '<S698>:1:118' t83 = t37.*2.0;
        // '<S698>:1:119' t84 = t41.*2.0;
        // '<S698>:1:120' t88 = cos(t52);
        // '<S698>:1:121' t89 = -t30;
        // '<S698>:1:122' t90 = -t31;
        // '<S698>:1:123' t91 = -t35;
        // '<S698>:1:124' t92 = -t40;
        // '<S698>:1:125' t93 = sin(t52);
        // '<S698>:1:126' t96 = sin(t94);
        // '<S698>:1:127' t98 = -t85;
        // '<S698>:1:128' t99 = -t86;
        // '<S698>:1:129' t100 = -t87;
        // '<S698>:1:130' t102 = sin(t95);
        Template_v3_08_2020a_B.t174 = (rtb_Delay_0[3] + rtb_Delay_0[4]) +
          -Template_v3_08_2020a_P.model_param[6];
        Template_v3_08_2020a_B.t102 = sin(Template_v3_08_2020a_B.t174);

        // '<S698>:1:131' t103 = t25+t36;
        // '<S698>:1:132' t104 = t26+t37;
        // '<S698>:1:133' t105 = sin(t101);
        Template_v3_08_2020a_B.t105 = sin((rtb_Delay_0[3] +
          Template_v3_08_2020a_B.t4) + -Template_v3_08_2020a_P.model_param[6]);

        // '<S698>:1:134' t115 = t42.*t106;
        // '<S698>:1:135' t118 = t55+t56+t61;
        // '<S698>:1:136' t119 = t58+t59+t60;
        // '<S698>:1:137' t125 = t27+t28+t29+t38+t39;
        // '<S698>:1:138' t126 = -t46.*t106;
        // '<S698>:1:139' t129 = -t43.*(t6+t85+t86);
        // '<S698>:1:140' t130 = -t47.*(t6+t85+t86);
        // '<S698>:1:141' t131 = t23+t24+t32+t33+t34+t41;
        // '<S698>:1:142' t97 = I3+t68;
        Template_v3_08_2020a_B.t97 = Template_v3_08_2020a_P.model_param[12] *
          Template_v3_08_2020a_B.t9_tmp + Template_v3_08_2020a_P.model_param[19];

        // '<S698>:1:143' t107 = t74+t82;
        // '<S698>:1:144' t108 = t75+t83;
        // '<S698>:1:145' t109 = t57.*t88;
        Template_v3_08_2020a_B.t109 = -Template_v3_08_2020a_B.t9_tmp * cos
          (Template_v3_08_2020a_B.t52);

        // '<S698>:1:146' t110 = t57.*t93;
        Template_v3_08_2020a_B.t110 = -Template_v3_08_2020a_B.t9_tmp * sin
          (Template_v3_08_2020a_B.t52);

        // '<S698>:1:147' t111 = t3.*t104;
        // '<S698>:1:148' t112 = t55+t100;
        // '<S698>:1:149' t113 = t44.*t103;
        Template_v3_08_2020a_B.t113 = (Template_v3_08_2020a_B.t25 +
          Template_v3_08_2020a_B.rtb_x_ddot_idx_1) * Template_v3_08_2020a_B.t44;

        // '<S698>:1:150' t117 = t91.*t105;
        Template_v3_08_2020a_B.t117 = -Template_v3_08_2020a_B.t35 *
          Template_v3_08_2020a_B.t105;

        // '<S698>:1:151' t120 = -t53.*(t7+t87);
        // '<S698>:1:152' t121 = t89+t90+t92;
        // '<S698>:1:153' t122 = -t51.*(t7+t87);
        // '<S698>:1:154' t124 = t54+t98+t99;
        // '<S698>:1:155' t127 = -t51.*(t7+t8+t13);
        // '<S698>:1:156' t128 = -t53.*(t7+t8+t13);
        // '<S698>:1:157' t133 = t2.*t131;
        // '<S698>:1:158' t134 = -t102.*(t30+t31+t40);
        Template_v3_08_2020a_B.t45 = ((Template_v3_08_2020a_B.Sum6_au +
          Template_v3_08_2020a_B.uDLookupTable2) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw) *
          -Template_v3_08_2020a_B.t102;

        // '<S698>:1:159' t135 = t96.*t125;
        Template_v3_08_2020a_B.t127 = Template_v3_08_2020a_P.model_param[6] +
          -rtb_Delay_0[3];
        Template_v3_08_2020a_B.t135 = ((((Template_v3_08_2020a_P.model_param[7] *
          Template_v3_08_2020a_B.t6 + Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.t9) + Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l) +
          Template_v3_08_2020a_P.model_param[9] * Template_v3_08_2020a_B.t9) +
          Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l) * sin
          (Template_v3_08_2020a_B.t127);

        // '<S698>:1:160' t136 = t72+t73+t78+t79+t80+t84;
        // '<S698>:1:161' t114 = t3.*t108;
        Template_v3_08_2020a_B.t114 = (Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 *
          2.0 + Template_v3_08_2020a_B.t37 * 2.0) *
          Template_v3_08_2020a_B.Sum6_l;

        // '<S698>:1:162' t116 = t44.*t107;
        Template_v3_08_2020a_B.rtb_x_ddot_idx_1 = (Template_v3_08_2020a_B.t25 *
          2.0 + Template_v3_08_2020a_B.rtb_x_ddot_idx_1 * 2.0) *
          Template_v3_08_2020a_B.t44;

        // '<S698>:1:163' t123 = t97+t111;
        Template_v3_08_2020a_B.Sum6_l = (Template_v3_08_2020a_B.rtb_Sum_ht_idx_1
          + Template_v3_08_2020a_B.t37) * Template_v3_08_2020a_B.Sum6_l +
          Template_v3_08_2020a_B.t97;

        // '<S698>:1:164' t132 = I2+t65+t66+t71+t81+t97;
        Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 =
          ((((Template_v3_08_2020a_P.model_param[10] *
              Template_v3_08_2020a_B.t7_tmp +
              Template_v3_08_2020a_P.model_param[18]) +
             Template_v3_08_2020a_P.model_param[10] *
             Template_v3_08_2020a_B.t155) + Template_v3_08_2020a_P.model_param
            [11] * Template_v3_08_2020a_B.t156) +
           Template_v3_08_2020a_P.model_param[11] * Template_v3_08_2020a_B.t155 *
           2.0) + Template_v3_08_2020a_B.t97;

        // '<S698>:1:165' t138 = t2.*t136;
        Template_v3_08_2020a_B.t37 = (((((Template_v3_08_2020a_B.t23 * 2.0 +
          Template_v3_08_2020a_B.t24 * 2.0) + Template_v3_08_2020a_B.Sum6_i *
          2.0) + Template_v3_08_2020a_B.t33 * 2.0) + Template_v3_08_2020a_B.t34 *
          2.0) + Template_v3_08_2020a_B.t41 * 2.0) * Template_v3_08_2020a_B.t2;

        // '<S698>:1:166' t139 = t109+t127;
        Template_v3_08_2020a_B.t167 = (Template_v3_08_2020a_B.t7_tmp +
          Template_v3_08_2020a_B.t155) + Template_v3_08_2020a_B.t156;
        Template_v3_08_2020a_B.t25 = Template_v3_08_2020a_B.t167 *
          -Template_v3_08_2020a_B.t51 + Template_v3_08_2020a_B.t109;

        // '<S698>:1:167' t140 = t110+t128;
        Template_v3_08_2020a_B.t8 = Template_v3_08_2020a_B.t167 *
          -Template_v3_08_2020a_B.t53 + Template_v3_08_2020a_B.t110;

        // '<S698>:1:168' t142 = t110+t120+t130;
        Template_v3_08_2020a_B.t52 = Template_v3_08_2020a_B.t7_tmp +
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_2 *
          Template_v3_08_2020a_P.model_param[16];
        Template_v3_08_2020a_B.t14 = (Template_v3_08_2020a_B.t6 +
          Template_v3_08_2020a_P.model_param[15] * Template_v3_08_2020a_B.t4_tmp)
          + Template_v3_08_2020a_P.model_param[16] *
          Template_v3_08_2020a_B.t4_tmp;
        Template_v3_08_2020a_B.t53 = (Template_v3_08_2020a_B.t52 *
          -Template_v3_08_2020a_B.t53 + Template_v3_08_2020a_B.t110) +
          Template_v3_08_2020a_B.t14 * -sin(Template_v3_08_2020a_B.t15_tmp);

        // '<S698>:1:169' t143 = t109+t122+t129;
        Template_v3_08_2020a_B.t44 = (Template_v3_08_2020a_B.t52 *
          -Template_v3_08_2020a_B.t51 + Template_v3_08_2020a_B.t109) +
          Template_v3_08_2020a_B.t14 * -cos(Template_v3_08_2020a_B.t15_tmp);

        // '<S698>:1:170' t137 = t113+t123;
        Template_v3_08_2020a_B.t85 = Template_v3_08_2020a_B.t113 +
          Template_v3_08_2020a_B.Sum6_l;

        // '<S698>:1:171' t144 = I1+t62+t63+t64+t69+t70+t76+t77+t132;
        Template_v3_08_2020a_B.t51 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_P.model_param[9];
        Template_v3_08_2020a_B.t21 = (((((((Template_v3_08_2020a_P.model_param[8]
          * Template_v3_08_2020a_B.t6 + Template_v3_08_2020a_P.model_param[17])
          + Template_v3_08_2020a_P.model_param[15] * Template_v3_08_2020a_B.t17)
          + Template_v3_08_2020a_P.model_param[16] * Template_v3_08_2020a_B.t17)
          + Template_v3_08_2020a_P.model_param[15] * Template_v3_08_2020a_B.t21)
          + Template_v3_08_2020a_P.model_param[16] * Template_v3_08_2020a_B.t21)
          + Template_v3_08_2020a_B.t51 * Template_v3_08_2020a_P.model_param[15] *
          2.0) + Template_v3_08_2020a_B.t51 *
          Template_v3_08_2020a_P.model_param[16] * 2.0) +
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;

        // '<S698>:1:172' t145 = t115+t142;
        Template_v3_08_2020a_B.t51 = cos(Template_v3_08_2020a_B.t14_tmp) *
          Template_v3_08_2020a_B.t106 + Template_v3_08_2020a_B.t53;

        // '<S698>:1:173' t146 = t126+t143;
        Template_v3_08_2020a_B.t14 = -sin(Template_v3_08_2020a_B.t14_tmp) *
          Template_v3_08_2020a_B.t106 + Template_v3_08_2020a_B.t44;

        // '<S698>:1:174' t147 = t113+t114+t132+t133;
        Template_v3_08_2020a_B.t24 = (((((Template_v3_08_2020a_B.t23 +
          Template_v3_08_2020a_B.t24) + Template_v3_08_2020a_B.Sum6_i) +
          Template_v3_08_2020a_B.t33) + Template_v3_08_2020a_B.t34) +
          Template_v3_08_2020a_B.t41) * Template_v3_08_2020a_B.t2 +
          ((Template_v3_08_2020a_B.t113 + Template_v3_08_2020a_B.t114) +
           Template_v3_08_2020a_B.rtb_Sum_ht_idx_1);

        // '<S698>:1:175' t141 = t117+t137;
        Template_v3_08_2020a_B.t106 = Template_v3_08_2020a_B.t117 +
          Template_v3_08_2020a_B.t85;

        // '<S698>:1:176' t148 = t117+t134+t147;
        Template_v3_08_2020a_B.t113 = (Template_v3_08_2020a_B.t117 +
          Template_v3_08_2020a_B.t45) + Template_v3_08_2020a_B.t24;

        // '<S698>:1:177' t149 = t114+t116+t117+t134+t135+t138+t144;
        Template_v3_08_2020a_B.t2 = Template_v3_08_2020a_B.t114 +
          Template_v3_08_2020a_B.rtb_x_ddot_idx_1;
        Template_v3_08_2020a_B.Sum6_i = ((((Template_v3_08_2020a_B.t2 +
          Template_v3_08_2020a_B.t117) + Template_v3_08_2020a_B.t45) +
          Template_v3_08_2020a_B.t135) + Template_v3_08_2020a_B.t37) +
          Template_v3_08_2020a_B.t21;

        // '<S698>:1:178' InertiaS = reshape([t50,0.0,t146,t143,t139,t109,0.0,t50,t145,t142,t140,t110,t146,t145,I0+t114+t116+t135.*2.0+t138+t144+b0.*t10+b0.*t11+b0.*t12-t35.*t105.*2.0-t102.*(t30.*2.0+t31.*2.0+t40.*2.0),t149,t148,t141,t143,t142,t149,t114+t116+t138+t144,t147,t137,t139,t140,t148,t147,t114+t132,t123,t109,t110,t141,t137,t123,t97],[6,6]); 
        Template_v3_08_2020a_B.InertiaS[0] = Template_v3_08_2020a_B.Sum6_d;
        Template_v3_08_2020a_B.InertiaS[1] = 0.0;
        Template_v3_08_2020a_B.InertiaS[2] = Template_v3_08_2020a_B.t14;
        Template_v3_08_2020a_B.InertiaS[3] = Template_v3_08_2020a_B.t44;
        Template_v3_08_2020a_B.InertiaS[4] = Template_v3_08_2020a_B.t25;
        Template_v3_08_2020a_B.InertiaS[5] = Template_v3_08_2020a_B.t109;
        Template_v3_08_2020a_B.InertiaS[6] = 0.0;
        Template_v3_08_2020a_B.InertiaS[7] = Template_v3_08_2020a_B.Sum6_d;
        Template_v3_08_2020a_B.InertiaS[8] = Template_v3_08_2020a_B.t51;
        Template_v3_08_2020a_B.InertiaS[9] = Template_v3_08_2020a_B.t53;
        Template_v3_08_2020a_B.InertiaS[10] = Template_v3_08_2020a_B.t8;
        Template_v3_08_2020a_B.InertiaS[11] = Template_v3_08_2020a_B.t110;
        Template_v3_08_2020a_B.InertiaS[12] = Template_v3_08_2020a_B.t14;
        Template_v3_08_2020a_B.InertiaS[13] = Template_v3_08_2020a_B.t51;
        Template_v3_08_2020a_B.InertiaS[14] =
          (((((((((Template_v3_08_2020a_P.model_param[1] +
                   Template_v3_08_2020a_B.t114) +
                  Template_v3_08_2020a_B.rtb_x_ddot_idx_1) +
                 Template_v3_08_2020a_B.t135 * 2.0) + Template_v3_08_2020a_B.t37)
               + Template_v3_08_2020a_B.t21) +
              Template_v3_08_2020a_P.model_param[7] * Template_v3_08_2020a_B.t7)
             + Template_v3_08_2020a_P.model_param[7] * Template_v3_08_2020a_B.t9)
            + Template_v3_08_2020a_P.model_param[7] *
            Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l) -
           Template_v3_08_2020a_B.t35 * Template_v3_08_2020a_B.t105 * 2.0) -
          ((Template_v3_08_2020a_B.Sum6_au * 2.0 +
            Template_v3_08_2020a_B.uDLookupTable2 * 2.0) +
           Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw * 2.0) *
          Template_v3_08_2020a_B.t102;
        Template_v3_08_2020a_B.InertiaS[15] = Template_v3_08_2020a_B.Sum6_i;
        Template_v3_08_2020a_B.InertiaS[16] = Template_v3_08_2020a_B.t113;
        Template_v3_08_2020a_B.InertiaS[17] = Template_v3_08_2020a_B.t106;
        Template_v3_08_2020a_B.InertiaS[18] = Template_v3_08_2020a_B.t44;
        Template_v3_08_2020a_B.InertiaS[19] = Template_v3_08_2020a_B.t53;
        Template_v3_08_2020a_B.InertiaS[20] = Template_v3_08_2020a_B.Sum6_i;
        Template_v3_08_2020a_B.InertiaS[21] = (Template_v3_08_2020a_B.t2 +
          Template_v3_08_2020a_B.t37) + Template_v3_08_2020a_B.t21;
        Template_v3_08_2020a_B.InertiaS[22] = Template_v3_08_2020a_B.t24;
        Template_v3_08_2020a_B.InertiaS[23] = Template_v3_08_2020a_B.t85;
        Template_v3_08_2020a_B.InertiaS[24] = Template_v3_08_2020a_B.t25;
        Template_v3_08_2020a_B.InertiaS[25] = Template_v3_08_2020a_B.t8;
        Template_v3_08_2020a_B.InertiaS[26] = Template_v3_08_2020a_B.t113;
        Template_v3_08_2020a_B.InertiaS[27] = Template_v3_08_2020a_B.t24;
        Template_v3_08_2020a_B.InertiaS[28] = Template_v3_08_2020a_B.t114 +
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;
        Template_v3_08_2020a_B.InertiaS[29] = Template_v3_08_2020a_B.Sum6_l;
        Template_v3_08_2020a_B.InertiaS[30] = Template_v3_08_2020a_B.t109;
        Template_v3_08_2020a_B.InertiaS[31] = Template_v3_08_2020a_B.t110;
        Template_v3_08_2020a_B.InertiaS[32] = Template_v3_08_2020a_B.t106;
        Template_v3_08_2020a_B.InertiaS[33] = Template_v3_08_2020a_B.t85;
        Template_v3_08_2020a_B.InertiaS[34] = Template_v3_08_2020a_B.Sum6_l;
        Template_v3_08_2020a_B.InertiaS[35] = Template_v3_08_2020a_B.t97;

        // MATLAB Function: '<S695>/Coriolis Function'
        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Coriolis Function': '<S696>:1' 
        // '<S696>:1:7' q0 = x(3);
        // '<S696>:1:8' q1 = x(4);
        // '<S696>:1:9' q2 = x(5);
        // '<S696>:1:10' q3 = x(6);
        // '<S696>:1:12' q0_dot = x(9);
        // '<S696>:1:13' q1_dot = x(10);
        // '<S696>:1:14' q2_dot = x(11);
        // '<S696>:1:15' q3_dot = x(12);
        // '<S696>:1:17' m0= model_param(1);
        // '<S696>:1:18' I0= model_param(2);
        // '<S696>:1:19' phi= model_param(7);
        // '<S696>:1:20' b0= model_param(8);
        // '<S696>:1:21' a1= model_param(9);
        // '<S696>:1:22' b1= model_param(10);
        // '<S696>:1:23' a2= model_param(11);
        // '<S696>:1:24' b2= model_param(12);
        // '<S696>:1:25' a3= model_param(13);
        // '<S696>:1:26' b3= model_param(14);
        // '<S696>:1:27' m1= model_param(15);
        // '<S696>:1:28' m2= model_param(16);
        // '<S696>:1:29' m3= model_param(17);
        // '<S696>:1:30' I1= model_param(18);
        // '<S696>:1:31' I2= model_param(19);
        // '<S696>:1:32' I3= model_param(20);
        // '<S696>:1:34' t2 = sin(q2);
        Template_v3_08_2020a_B.t2 = sin(rtb_Delay_0[4]);

        // '<S696>:1:35' t3 = sin(q3);
        Template_v3_08_2020a_B.Sum6_l = sin(rtb_Delay_0[5]);

        // '<S696>:1:36' t4 = a1+b1;
        // '<S696>:1:37' t5 = a2+b2;
        // '<S696>:1:38' t6 = a1.*m1;
        // '<S696>:1:39' t7 = a2.*m2;
        // '<S696>:1:40' t8 = b0.*m1;
        // '<S696>:1:41' t9 = b0.*m2;
        // '<S696>:1:42' t10 = b0.*m3;
        // '<S696>:1:43' t11 = a1.*q0_dot;
        Template_v3_08_2020a_B.t21 = Template_v3_08_2020a_P.model_param[8] *
          rtb_Delay_0[8];

        // '<S696>:1:44' t12 = a1.*q1_dot;
        Template_v3_08_2020a_B.t85 = Template_v3_08_2020a_P.model_param[8] *
          rtb_Delay_0[9];

        // '<S696>:1:45' t13 = a2.*q0_dot;
        Template_v3_08_2020a_B.t35 = Template_v3_08_2020a_P.model_param[10] *
          rtb_Delay_0[8];

        // '<S696>:1:46' t14 = a2.*q1_dot;
        Template_v3_08_2020a_B.t14 = Template_v3_08_2020a_P.model_param[10] *
          rtb_Delay_0[9];

        // '<S696>:1:47' t15 = a2.*q2_dot;
        Template_v3_08_2020a_B.t37 = Template_v3_08_2020a_P.model_param[10] *
          rtb_Delay_0[10];

        // '<S696>:1:48' t16 = b1.*q0_dot;
        Template_v3_08_2020a_B.t16 = Template_v3_08_2020a_P.model_param[9] *
          rtb_Delay_0[8];

        // '<S696>:1:49' t17 = b1.*q1_dot;
        Template_v3_08_2020a_B.t17 = Template_v3_08_2020a_P.model_param[9] *
          rtb_Delay_0[9];

        // '<S696>:1:50' t18 = b2.*q0_dot;
        Template_v3_08_2020a_B.t18 = Template_v3_08_2020a_P.model_param[11] *
          rtb_Delay_0[8];

        // '<S696>:1:51' t19 = b2.*q1_dot;
        Template_v3_08_2020a_B.t19 = Template_v3_08_2020a_P.model_param[11] *
          rtb_Delay_0[9];

        // '<S696>:1:52' t20 = b2.*q2_dot;
        Template_v3_08_2020a_B.t20 = Template_v3_08_2020a_P.model_param[11] *
          rtb_Delay_0[10];

        // '<S696>:1:53' t21 = phi+q0;
        // '<S696>:1:54' t22 = q2+q3;
        // '<S696>:1:55' t23 = a3.*m3.*q0_dot;
        // '<S696>:1:56' t24 = a3.*m3.*q1_dot;
        // '<S696>:1:57' t25 = a3.*m3.*q2_dot;
        Template_v3_08_2020a_B.t25 = Template_v3_08_2020a_B.t9_tmp *
          rtb_Delay_0[10];

        // '<S696>:1:58' t26 = a3.*m3.*q3_dot;
        Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 = Template_v3_08_2020a_B.t9_tmp *
          rtb_Delay_0[11];

        // '<S696>:1:59' t28 = -phi;
        // '<S696>:1:60' t29 = -q1;
        // '<S696>:1:61' t71 = q0_dot+q1_dot+q2_dot+q3_dot;
        Template_v3_08_2020a_B.t41 = (rtb_Delay_0[8] + rtb_Delay_0[9]) +
          rtb_Delay_0[10];
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw =
          Template_v3_08_2020a_B.t41 + rtb_Delay_0[11];

        // '<S696>:1:62' t72 = pi./2.0;
        // '<S696>:1:63' t27 = sin(t22);
        Template_v3_08_2020a_B.t27 = sin(Template_v3_08_2020a_B.t4);

        // '<S696>:1:64' t30 = b0.*q0_dot.*t6;
        Template_v3_08_2020a_B.t157 = Template_v3_08_2020a_P.model_param[7] *
          rtb_Delay_0[8];
        Template_v3_08_2020a_B.Sum6_au = Template_v3_08_2020a_B.t157 *
          Template_v3_08_2020a_B.t6;

        // '<S696>:1:65' t31 = b0.*q1_dot.*t6;
        Template_v3_08_2020a_B.t53 = Template_v3_08_2020a_P.model_param[7] *
          rtb_Delay_0[9];
        Template_v3_08_2020a_B.uDLookupTable2 = Template_v3_08_2020a_B.t53 *
          Template_v3_08_2020a_B.t6;

        // '<S696>:1:66' t32 = t9.*t11;
        Template_v3_08_2020a_B.Sum6_i = Template_v3_08_2020a_B.t9 *
          Template_v3_08_2020a_B.t21;

        // '<S696>:1:67' t33 = t9.*t12;
        Template_v3_08_2020a_B.t33 = Template_v3_08_2020a_B.t9 *
          Template_v3_08_2020a_B.t85;

        // '<S696>:1:68' t34 = t10.*t11;
        Template_v3_08_2020a_B.t34 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t21;

        // '<S696>:1:69' t35 = b0.*q0_dot.*t7;
        // '<S696>:1:70' t36 = t10.*t12;
        Template_v3_08_2020a_B.rtb_x_ddot_idx_1 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t85;

        // '<S696>:1:71' t37 = b0.*q1_dot.*t7;
        // '<S696>:1:72' t38 = t10.*t13;
        // '<S696>:1:73' t39 = t7.*t16;
        // '<S696>:1:74' t40 = b2.*m3.*t11;
        // '<S696>:1:75' t41 = b0.*q2_dot.*t7;
        // '<S696>:1:76' t42 = t10.*t14;
        Template_v3_08_2020a_B.t102 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t14;

        // '<S696>:1:77' t43 = t7.*t17;
        // '<S696>:1:78' t44 = b1.*m3.*t13;
        // '<S696>:1:79' t45 = a3.*q0_dot.*t10;
        Template_v3_08_2020a_B.t45 = Template_v3_08_2020a_P.model_param[12] *
          rtb_Delay_0[8] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S696>:1:80' t46 = b2.*m3.*t12;
        // '<S696>:1:81' t47 = t10.*t15;
        Template_v3_08_2020a_B.t105 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t37;

        // '<S696>:1:82' t48 = b1.*q2_dot.*t7;
        // '<S696>:1:83' t49 = b1.*m3.*t14;
        // '<S696>:1:84' t50 = a3.*q1_dot.*t10;
        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_P.model_param[12] *
          rtb_Delay_0[9] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S696>:1:85' t51 = a3.*m3.*t16;
        // '<S696>:1:86' t52 = a1.*m3.*t20;
        Template_v3_08_2020a_B.t197 = Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S696>:1:87' t53 = b1.*m3.*t15;
        Template_v3_08_2020a_B.t52 = Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_P.model_param[16];

        // '<S696>:1:88' t54 = a3.*q2_dot.*t10;
        Template_v3_08_2020a_B.t97 = Template_v3_08_2020a_P.model_param[12] *
          rtb_Delay_0[10] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S696>:1:89' t55 = a3.*m3.*t17;
        // '<S696>:1:90' t56 = a3.*q3_dot.*t10;
        Template_v3_08_2020a_B.t109 = Template_v3_08_2020a_P.model_param[12] *
          rtb_Delay_0[11] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // '<S696>:1:91' t57 = b1.*t25;
        // '<S696>:1:92' t58 = b1.*t26;
        // '<S696>:1:93' t59 = b2.*t26;
        // '<S696>:1:94' t60 = t9.*t16;
        Template_v3_08_2020a_B.t110 = Template_v3_08_2020a_B.t9 *
          Template_v3_08_2020a_B.t16;

        // '<S696>:1:95' t61 = t9.*t17;
        Template_v3_08_2020a_B.t135 = Template_v3_08_2020a_B.t9 *
          Template_v3_08_2020a_B.t17;

        // '<S696>:1:96' t62 = t10.*t16;
        Template_v3_08_2020a_B.t114 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t16;

        // '<S696>:1:97' t63 = t10.*t17;
        Template_v3_08_2020a_B.t117 =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t17;

        // '<S696>:1:98' t64 = t10.*t18;
        // '<S696>:1:99' t65 = t10.*t19;
        // '<S696>:1:100' t66 = b2.*m3.*t16;
        // '<S696>:1:101' t67 = t10.*t20;
        // '<S696>:1:102' t68 = b2.*m3.*t17;
        // '<S696>:1:103' t69 = b1.*m3.*t20;
        // '<S696>:1:104' t70 = q0+q1+t22;
        Template_v3_08_2020a_B.t44 = Template_v3_08_2020a_B.t15_tmp +
          Template_v3_08_2020a_B.t4;

        // '<S696>:1:105' t73 = m2.*t4;
        // '<S696>:1:106' t74 = m3.*t4;
        Template_v3_08_2020a_B.t23 = Template_v3_08_2020a_P.model_param[16] *
          Template_v3_08_2020a_B.t4_tmp;

        // '<S696>:1:107' t75 = m3.*t5;
        Template_v3_08_2020a_B.t75 = Template_v3_08_2020a_P.model_param[16] *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_2;

        // '<S696>:1:108' t76 = t7.*t11;
        // '<S696>:1:109' t77 = t7.*t12;
        // '<S696>:1:110' t78 = a2.*m3.*t11;
        // '<S696>:1:111' t79 = a1.*q2_dot.*t7;
        // '<S696>:1:112' t80 = a2.*m3.*t12;
        // '<S696>:1:113' t81 = a3.*m3.*t11;
        // '<S696>:1:114' t82 = a1.*m3.*t15;
        // '<S696>:1:115' t83 = a3.*m3.*t12;
        // '<S696>:1:116' t84 = a1.*t25;
        // '<S696>:1:117' t85 = a1.*t26;
        // '<S696>:1:118' t86 = a2.*t26;
        // '<S696>:1:119' t87 = -t23;
        // '<S696>:1:120' t88 = -t24;
        // '<S696>:1:121' t89 = -t25;
        // '<S696>:1:122' t90 = -t26;
        // '<S696>:1:123' t91 = phi+t29;
        // '<S696>:1:124' t128 = q1+q2+t28;
        // '<S696>:1:125' t140 = a3.*m3.*t71;
        Template_v3_08_2020a_B.t8 = Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw;

        // '<S696>:1:126' t142 = q1+t22+t28;
        // '<S696>:1:127' t143 = q0+q1+t72;
        // '<S696>:1:128' t152 = t8+t9+t10;
        // '<S696>:1:129' t161 = t11+t12+t16+t17;
        // '<S696>:1:130' t180 = t13+t14+t15+t18+t19+t20;
        // '<S696>:1:131' t92 = -t31;
        // '<S696>:1:132' t93 = -t33;
        // '<S696>:1:133' t94 = -t35;
        // '<S696>:1:134' t95 = -t36;
        // '<S696>:1:135' t96 = -t37;
        // '<S696>:1:136' t97 = -t38;
        // '<S696>:1:137' t98 = -t39;
        // '<S696>:1:138' t99 = -t40;
        // '<S696>:1:139' t100 = -t41;
        // '<S696>:1:140' t101 = -t42;
        // '<S696>:1:141' t102 = -t43;
        // '<S696>:1:142' t103 = -t44;
        // '<S696>:1:143' t104 = -t45;
        // '<S696>:1:144' t105 = -t46;
        // '<S696>:1:145' t106 = -t47;
        // '<S696>:1:146' t107 = -t48;
        // '<S696>:1:147' t108 = -t49;
        // '<S696>:1:148' t109 = -t50;
        // '<S696>:1:149' t110 = -t51;
        // '<S696>:1:150' t111 = -t52;
        // '<S696>:1:151' t112 = -t53;
        // '<S696>:1:152' t113 = -t54;
        // '<S696>:1:153' t114 = -t55;
        // '<S696>:1:154' t115 = -t56;
        // '<S696>:1:155' t116 = -t57;
        // '<S696>:1:156' t117 = -t58;
        // '<S696>:1:157' t118 = -t59;
        // '<S696>:1:158' t119 = -t61;
        // '<S696>:1:159' t120 = -t63;
        // '<S696>:1:160' t121 = -t64;
        // '<S696>:1:161' t122 = -t65;
        // '<S696>:1:162' t123 = -t66;
        // '<S696>:1:163' t124 = -t67;
        // '<S696>:1:164' t125 = -t68;
        // '<S696>:1:165' t126 = -t69;
        // '<S696>:1:166' t127 = cos(t91);
        Template_v3_08_2020a_B.t127 = cos(Template_v3_08_2020a_B.t127);

        // '<S696>:1:167' t129 = -t76;
        // '<S696>:1:168' t130 = -t77;
        // '<S696>:1:169' t131 = -t78;
        // '<S696>:1:170' t132 = -t79;
        // '<S696>:1:171' t133 = -t80;
        // '<S696>:1:172' t134 = -t81;
        // '<S696>:1:173' t135 = -t82;
        // '<S696>:1:174' t136 = -t83;
        // '<S696>:1:175' t137 = -t84;
        // '<S696>:1:176' t138 = -t85;
        // '<S696>:1:177' t139 = -t86;
        // '<S696>:1:178' t141 = cos(t128);
        Template_v3_08_2020a_B.t106 = cos(Template_v3_08_2020a_B.t174);

        // '<S696>:1:179' t144 = cos(t143);
        // '<S696>:1:180' t145 = q2+t143;
        Template_v3_08_2020a_B.t51 = rtb_Delay_0[4] +
          (Template_v3_08_2020a_B.t15_tmp + 1.5707963267948966);

        // '<S696>:1:181' t146 = sin(t143);
        // '<S696>:1:182' t147 = t7+t75;
        Template_v3_08_2020a_B.t24 = Template_v3_08_2020a_B.t7_tmp +
          Template_v3_08_2020a_B.t75;

        // '<S696>:1:183' t148 = cos(t142);
        Template_v3_08_2020a_B.t113 = cos((rtb_Delay_0[3] +
          Template_v3_08_2020a_B.t4) + -Template_v3_08_2020a_P.model_param[6]);

        // '<S696>:1:184' t150 = t70+t72;
        // '<S696>:1:185' t158 = a3.*t71.*t74;
        // '<S696>:1:186' t159 = a3.*t71.*t75;
        // '<S696>:1:187' t160 = q0_dot.*t152;
        // '<S696>:1:188' t167 = t6+t73+t74;
        Template_v3_08_2020a_B.t167 = (Template_v3_08_2020a_P.model_param[15] *
          Template_v3_08_2020a_B.t4_tmp + Template_v3_08_2020a_B.t6) +
          Template_v3_08_2020a_B.t23;

        // '<S696>:1:189' t173 = a3.*m3.*t161;
        // '<S696>:1:190' t174 = t35+t38+t64;
        Template_v3_08_2020a_B.t174 = (Template_v3_08_2020a_B.t157 *
          Template_v3_08_2020a_B.t7_tmp +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t35) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t18;

        // '<S696>:1:191' t179 = -t3.*(t59+t86);
        Template_v3_08_2020a_B.t6 = (Template_v3_08_2020a_P.model_param[11] *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 +
          Template_v3_08_2020a_P.model_param[10] *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1) *
          -Template_v3_08_2020a_B.Sum6_l;

        // '<S696>:1:192' t181 = t51+t55+t81+t83;
        Template_v3_08_2020a_B.t181 = ((Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.t16 + Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.t17) + Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.t21) + Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.t85;

        // '<S696>:1:193' t183 = a3.*m3.*t180;
        // '<S696>:1:194' t185 = t87+t88+t89+t90;
        // '<S696>:1:195' t190 = -t27.*(t57+t58+t84+t85);
        Template_v3_08_2020a_B.t4_tmp = ((Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.t25 + Template_v3_08_2020a_P.model_param[9] *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1) +
          Template_v3_08_2020a_P.model_param[8] * Template_v3_08_2020a_B.t25) +
          Template_v3_08_2020a_P.model_param[8] *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;
        Template_v3_08_2020a_B.t4 = Template_v3_08_2020a_B.t4_tmp *
          -Template_v3_08_2020a_B.t27;

        // '<S696>:1:196' t197 = -t2.*(t48+t52+t53+t69+t79+t82);
        Template_v3_08_2020a_B.t197_tmp =
          ((((Template_v3_08_2020a_P.model_param[9] * rtb_Delay_0[10] *
              Template_v3_08_2020a_B.t7_tmp + Template_v3_08_2020a_B.t197 *
              Template_v3_08_2020a_B.t20) + Template_v3_08_2020a_B.t52 *
             Template_v3_08_2020a_B.t37) + Template_v3_08_2020a_B.t52 *
            Template_v3_08_2020a_B.t20) + Template_v3_08_2020a_P.model_param[8] *
           rtb_Delay_0[10] * Template_v3_08_2020a_B.t7_tmp) +
          Template_v3_08_2020a_B.t197 * Template_v3_08_2020a_B.t37;
        Template_v3_08_2020a_B.t197 = Template_v3_08_2020a_B.t197_tmp *
          -Template_v3_08_2020a_B.t2;

        // '<S696>:1:197' t203 = t39+t40+t43+t44+t46+t49+t66+t68+t76+t77+t78+t80; 
        Template_v3_08_2020a_B.t52 = ((((((((((Template_v3_08_2020a_B.t156 *
          Template_v3_08_2020a_B.t21 + Template_v3_08_2020a_B.t7_tmp *
          Template_v3_08_2020a_B.t16) + Template_v3_08_2020a_B.t7_tmp *
          Template_v3_08_2020a_B.t17) + Template_v3_08_2020a_B.t52 *
          Template_v3_08_2020a_B.t35) + Template_v3_08_2020a_B.t156 *
          Template_v3_08_2020a_B.t85) + Template_v3_08_2020a_B.t52 *
          Template_v3_08_2020a_B.t14) + Template_v3_08_2020a_B.t156 *
          Template_v3_08_2020a_B.t16) + Template_v3_08_2020a_B.t156 *
          Template_v3_08_2020a_B.t17) + Template_v3_08_2020a_B.t7_tmp *
          Template_v3_08_2020a_B.t21) + Template_v3_08_2020a_B.t7_tmp *
          Template_v3_08_2020a_B.t85) + Template_v3_08_2020a_B.t155 *
          Template_v3_08_2020a_B.t21) + Template_v3_08_2020a_B.t155 *
          Template_v3_08_2020a_B.t85;

        // '<S696>:1:198' t149 = cos(t145);
        // '<S696>:1:199' t151 = sin(t145);
        // '<S696>:1:200' t153 = cos(t150);
        // '<S696>:1:201' t154 = sin(t150);
        // '<S696>:1:202' t155 = q0_dot.*t147;
        Template_v3_08_2020a_B.t155 = rtb_Delay_0[8] *
          Template_v3_08_2020a_B.t24;

        // '<S696>:1:203' t156 = q1_dot.*t147;
        Template_v3_08_2020a_B.t156 = rtb_Delay_0[9] *
          Template_v3_08_2020a_B.t24;

        // '<S696>:1:204' t157 = q2_dot.*t147;
        Template_v3_08_2020a_B.t157 = rtb_Delay_0[10] *
          Template_v3_08_2020a_B.t24;

        // '<S696>:1:205' t165 = -t158;
        // '<S696>:1:206' t166 = -t159;
        // '<S696>:1:207' t168 = -t160;
        Template_v3_08_2020a_B.t7 = -(((Template_v3_08_2020a_B.t7 +
          Template_v3_08_2020a_B.t9) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l) * rtb_Delay_0
          [8]);

        // '<S696>:1:208' t169 = t45.*t148;
        Template_v3_08_2020a_B.t24 = Template_v3_08_2020a_B.t45 *
          Template_v3_08_2020a_B.t113;

        // '<S696>:1:209' t170 = t118+t139;
        // '<S696>:1:210' t171 = q0_dot.*t167;
        Template_v3_08_2020a_B.t9 = rtb_Delay_0[8] * Template_v3_08_2020a_B.t167;

        // '<S696>:1:211' t172 = q1_dot.*t167;
        Template_v3_08_2020a_B.t167 *= rtb_Delay_0[9];

        // '<S696>:1:212' t182 = t27.*t173;
        Template_v3_08_2020a_B.t21 = (((Template_v3_08_2020a_B.t21 +
          Template_v3_08_2020a_B.t85) + Template_v3_08_2020a_B.t16) +
          Template_v3_08_2020a_B.t17) * Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.t27;

        // '<S696>:1:213' t184 = t141.*t174;
        // '<S696>:1:214' t186 = t3.*t183;
        Template_v3_08_2020a_B.t14 = (((((Template_v3_08_2020a_B.t35 +
          Template_v3_08_2020a_B.t14) + Template_v3_08_2020a_B.t37) +
          Template_v3_08_2020a_B.t18) + Template_v3_08_2020a_B.t19) +
          Template_v3_08_2020a_B.t20) * Template_v3_08_2020a_B.t9_tmp *
          Template_v3_08_2020a_B.Sum6_l;

        // '<S696>:1:215' t187 = t27.*t181;
        Template_v3_08_2020a_B.t37 = Template_v3_08_2020a_B.t27 *
          Template_v3_08_2020a_B.t181;

        // '<S696>:1:216' t188 = t116+t117+t137+t138;
        // '<S696>:1:217' t189 = t104+t109+t113+t115;
        // '<S696>:1:218' t194 = -t148.*(t45+t50+t54+t56);
        Template_v3_08_2020a_B.t35 = (((Template_v3_08_2020a_B.t45 +
          Template_v3_08_2020a_B.Sum6_d) + Template_v3_08_2020a_B.t97) +
          Template_v3_08_2020a_B.t109) * -Template_v3_08_2020a_B.t113;

        // '<S696>:1:219' t196 = t107+t111+t112+t126+t132+t135;
        // '<S696>:1:220' t204 = -t27.*(t57+t58+t84+t85+t181);
        Template_v3_08_2020a_B.t85 = (Template_v3_08_2020a_B.t4_tmp +
          Template_v3_08_2020a_B.t181) * -Template_v3_08_2020a_B.t27;

        // '<S696>:1:221' t205 = t94+t96+t97+t100+t101+t106+t121+t122+t124;
        // '<S696>:1:222' t206 = t2.*t203;
        Template_v3_08_2020a_B.t17 = Template_v3_08_2020a_B.t2 *
          Template_v3_08_2020a_B.t52;

        // '<S696>:1:223' t207 = -t141.*(t37+t41+t42+t47+t65+t67+t174);
        Template_v3_08_2020a_B.t4_tmp = ((((Template_v3_08_2020a_B.t53 *
          Template_v3_08_2020a_B.t7_tmp + Template_v3_08_2020a_P.model_param[7] *
          rtb_Delay_0[10] * Template_v3_08_2020a_B.t7_tmp) +
          Template_v3_08_2020a_B.t102) + Template_v3_08_2020a_B.t105) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t19) +
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.t20;
        Template_v3_08_2020a_B.t45 = (Template_v3_08_2020a_B.t4_tmp +
          Template_v3_08_2020a_B.t174) * -Template_v3_08_2020a_B.t106;

        // '<S696>:1:224' t209 = -t2.*(t48+t52+t53+t69+t79+t82+t203);
        Template_v3_08_2020a_B.t2 = (Template_v3_08_2020a_B.t197_tmp +
          Template_v3_08_2020a_B.t52) * -Template_v3_08_2020a_B.t2;

        // '<S696>:1:225' t162 = -t155;
        // '<S696>:1:226' t163 = -t156;
        // '<S696>:1:227' t164 = -t157;
        // '<S696>:1:228' t175 = t3.*t166;
        Template_v3_08_2020a_B.t52 = Template_v3_08_2020a_P.model_param[12] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw;
        Template_v3_08_2020a_B.t53 = -(Template_v3_08_2020a_B.t52 *
          Template_v3_08_2020a_B.t75) * Template_v3_08_2020a_B.Sum6_l;

        // '<S696>:1:229' t176 = -t171;
        // '<S696>:1:230' t177 = -t172;
        // '<S696>:1:231' t178 = t27.*t165;
        Template_v3_08_2020a_B.t52 = -(Template_v3_08_2020a_B.t52 *
          Template_v3_08_2020a_B.t23) * Template_v3_08_2020a_B.t27;

        // '<S696>:1:232' t191 = -t153.*(t23+t24+t25+t26);
        Template_v3_08_2020a_B.t25 = ((Template_v3_08_2020a_B.t9_tmp *
          rtb_Delay_0[8] + Template_v3_08_2020a_B.t9_tmp * rtb_Delay_0[9]) +
          Template_v3_08_2020a_B.t25) + Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;
        Template_v3_08_2020a_B.t27 = Template_v3_08_2020a_B.t25 * -cos
          (Template_v3_08_2020a_B.t44 + 1.5707963267948966);

        // '<S696>:1:233' t192 = -t154.*(t23+t24+t25+t26);
        Template_v3_08_2020a_B.t23 = Template_v3_08_2020a_B.t25 * -sin
          (Template_v3_08_2020a_B.t44 + 1.5707963267948966);

        // '<S696>:1:234' t198 = -t144.*(t171+t172);
        Template_v3_08_2020a_B.t9_tmp = Template_v3_08_2020a_B.t9 +
          Template_v3_08_2020a_B.t167;

        // '<S696>:1:235' t199 = -t146.*(t171+t172);
        // '<S696>:1:236' t200 = -t149.*(t155+t156+t157);
        Template_v3_08_2020a_B.t167 = (Template_v3_08_2020a_B.t155 +
          Template_v3_08_2020a_B.t156) + Template_v3_08_2020a_B.t157;
        Template_v3_08_2020a_B.t25 = Template_v3_08_2020a_B.t167 * -cos
          (Template_v3_08_2020a_B.t51);

        // '<S696>:1:237' t201 = -t151.*(t155+t156+t157);
        Template_v3_08_2020a_B.t51 = Template_v3_08_2020a_B.t167 * -sin
          (Template_v3_08_2020a_B.t51);

        // MATLAB Function: '<S695>/Dynamics Model'
        // '<S696>:1:238' t202 = t110+t114+t134+t136+t188;
        // '<S696>:1:239' t208 = t98+t99+t102+t103+t105+t108+t123+t125+t129+t130+t131+t133+t196; 
        // '<S696>:1:240' t193 = t176+t177;
        // '<S696>:1:241' t195 = t162+t163+t164;
        // '<S696>:1:242' CoriolisS = reshape([0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,t191+t198+t200+t168.*cos(t21),t192+t199+t201+t168.*sin(t21),t179+t190+t197-t127.*(t31+t33+t36+t61+t63)-t148.*(t50+t54+t56)-t141.*(t37+t41+t42+t47+t65+t67),t169+t179+t184+t190+t197+t127.*(t30+t32+t34+t60+t62),t169+t179+t184+t187+t206,t169+t182+t186,t191+t198+t200,t192+t199+t201,t179+t190+t194+t197+t207-t127.*(t30+t31+t32+t33+t34+t36+t60+t61+t62+t63),t179+t190+t197,t179+t187+t206,t182+t186,t191+t200,t192+t201,t179+t194+t204+t207+t209,t179+t204+t209,t3.*t5.*t90,a3.*t3.*t75.*(q0_dot+q1_dot+q2_dot),t140.*sin(t70),-t140.*cos(t70),t175+t178-a3.*t10.*t71.*t148,t175+t178,t175,0.0],[6,6]); 
        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED + ARM/RED + ARM Dynamics/Dynamics Model': '<S697>:1' 
        // '<S697>:1:3' fqdot1 = Gamma1*(tanh(Gamma2*x(10)) - tanh(Gamma3*x(10))) + Gamma4*tanh(Gamma5*x(10)) + Gamma6*x(10); 
        // '<S697>:1:4' fqdot2 = Gamma1*(tanh(Gamma2*x(11)) - tanh(Gamma3*x(11))) + Gamma4*tanh(Gamma5*x(11)) + Gamma6*x(11); 
        // '<S697>:1:5' fqdot3 = Gamma1*(tanh(Gamma2*x(12)) - tanh(Gamma3*x(12))) + Gamma4*tanh(Gamma5*x(12)) + Gamma6*x(12); 
        // '<S697>:1:7' q_ddot = pinv(InertiaS)*(u - CoriolisS*x(7:12) - [0;0;0;fqdot1;fqdot2;fqdot3]); 
        Template_v3_08_2020a_B.p = true;
        for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 36;
             Template_v3_08_2020a_B.i++) {
          Template_v3_08_2020a_B.X[Template_v3_08_2020a_B.i] = 0.0;
          if (Template_v3_08_2020a_B.p && (rtIsInf
               (Template_v3_08_2020a_B.InertiaS[Template_v3_08_2020a_B.i]) ||
               rtIsNaN(Template_v3_08_2020a_B.InertiaS[Template_v3_08_2020a_B.i])))
          {
            Template_v3_08_2020a_B.p = false;
          }
        }

        if (!Template_v3_08_2020a_B.p) {
          for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 36;
               Template_v3_08_2020a_B.i++) {
            Template_v3_08_2020a_B.X[Template_v3_08_2020a_B.i] = (rtNaN);
          }
        } else {
          Template_v3_08_2020a_svd(Template_v3_08_2020a_B.InertiaS,
            Template_v3_08_2020a_B.U, Template_v3_08_2020a_B.s,
            Template_v3_08_2020a_B.V);
          Template_v3_08_2020a_B.t155 = fabs(Template_v3_08_2020a_B.s[0]);
          if ((!rtIsInf(Template_v3_08_2020a_B.t155)) && (!rtIsNaN
               (Template_v3_08_2020a_B.t155))) {
            if (Template_v3_08_2020a_B.t155 <= 2.2250738585072014E-308) {
              Template_v3_08_2020a_B.t155 = 4.94065645841247E-324;
            } else {
              frexp(Template_v3_08_2020a_B.t155,
                    &Template_v3_08_2020a_B.uElOffset1);
              Template_v3_08_2020a_B.t155 = ldexp(1.0,
                Template_v3_08_2020a_B.uElOffset1 - 53);
            }
          } else {
            Template_v3_08_2020a_B.t155 = (rtNaN);
          }

          Template_v3_08_2020a_B.t155 *= 6.0;
          Template_v3_08_2020a_B.i = -1;
          Template_v3_08_2020a_B.uElOffset1 = 0;
          while ((Template_v3_08_2020a_B.uElOffset1 < 6) &&
                 (Template_v3_08_2020a_B.s[Template_v3_08_2020a_B.uElOffset1] >
                  Template_v3_08_2020a_B.t155)) {
            Template_v3_08_2020a_B.i++;
            Template_v3_08_2020a_B.uElOffset1++;
          }

          if (Template_v3_08_2020a_B.i + 1 > 0) {
            Template_v3_08_2020a_B.vcol = 0;
            Template_v3_08_2020a_B.uElOffset1 = 0;
            while (Template_v3_08_2020a_B.uElOffset1 <= Template_v3_08_2020a_B.i)
            {
              Template_v3_08_2020a_B.t155 = 1.0 /
                Template_v3_08_2020a_B.s[Template_v3_08_2020a_B.uElOffset1];
              Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.vcol;
              while (Template_v3_08_2020a_B.ar + 1 <=
                     Template_v3_08_2020a_B.vcol + 6) {
                Template_v3_08_2020a_B.V[Template_v3_08_2020a_B.ar] *=
                  Template_v3_08_2020a_B.t155;
                Template_v3_08_2020a_B.ar++;
              }

              Template_v3_08_2020a_B.vcol += 6;
              Template_v3_08_2020a_B.uElOffset1++;
            }

            for (Template_v3_08_2020a_B.uElOffset1 = 0;
                 Template_v3_08_2020a_B.uElOffset1 <= 31;
                 Template_v3_08_2020a_B.uElOffset1 += 6) {
              Template_v3_08_2020a_B.vcol = Template_v3_08_2020a_B.uElOffset1;
              while (Template_v3_08_2020a_B.vcol + 1 <=
                     Template_v3_08_2020a_B.uElOffset1 + 6) {
                Template_v3_08_2020a_B.X[Template_v3_08_2020a_B.vcol] = 0.0;
                Template_v3_08_2020a_B.vcol++;
              }
            }

            Template_v3_08_2020a_B.vcol = 0;
            for (Template_v3_08_2020a_B.uElOffset1 = 0;
                 Template_v3_08_2020a_B.uElOffset1 <= 31;
                 Template_v3_08_2020a_B.uElOffset1 += 6) {
              Template_v3_08_2020a_B.ar = -1;
              Template_v3_08_2020a_B.vcol++;
              Template_v3_08_2020a_B.ntIdx0 = 6 * Template_v3_08_2020a_B.i +
                Template_v3_08_2020a_B.vcol;
              Template_v3_08_2020a_B.ib = Template_v3_08_2020a_B.vcol;
              while (Template_v3_08_2020a_B.ib <= Template_v3_08_2020a_B.ntIdx0)
              {
                Template_v3_08_2020a_B.ia = Template_v3_08_2020a_B.ar;
                Template_v3_08_2020a_B.b_ic = Template_v3_08_2020a_B.uElOffset1;
                while (Template_v3_08_2020a_B.b_ic + 1 <=
                       Template_v3_08_2020a_B.uElOffset1 + 6) {
                  Template_v3_08_2020a_B.ia++;
                  Template_v3_08_2020a_B.X[Template_v3_08_2020a_B.b_ic] +=
                    Template_v3_08_2020a_B.U[Template_v3_08_2020a_B.ib - 1] *
                    Template_v3_08_2020a_B.V[Template_v3_08_2020a_B.ia];
                  Template_v3_08_2020a_B.b_ic++;
                }

                Template_v3_08_2020a_B.ar += 6;
                Template_v3_08_2020a_B.ib += 6;
              }
            }
          }
        }

        // MATLAB Function: '<S695>/Coriolis Function'
        // '<S697>:1:9' xdot   = [ x(7:12)
        // '<S697>:1:10'            q_ddot   ];
        memset(&Template_v3_08_2020a_B.InertiaS[0], 0, 12U * sizeof(real_T));
        Template_v3_08_2020a_B.t7_tmp = (Template_v3_08_2020a_B.t27 +
          Template_v3_08_2020a_B.t9_tmp * -cos(Template_v3_08_2020a_B.t15_tmp +
          1.5707963267948966)) + Template_v3_08_2020a_B.t25;
        Template_v3_08_2020a_B.InertiaS[12] = Template_v3_08_2020a_B.t7_tmp +
          Template_v3_08_2020a_B.t7 * cos(Template_v3_08_2020a_B.t14_tmp);
        Template_v3_08_2020a_B.t15_tmp = (Template_v3_08_2020a_B.t23 +
          Template_v3_08_2020a_B.t9_tmp * -sin(Template_v3_08_2020a_B.t15_tmp +
          1.5707963267948966)) + Template_v3_08_2020a_B.t51;
        Template_v3_08_2020a_B.InertiaS[13] = Template_v3_08_2020a_B.t15_tmp +
          Template_v3_08_2020a_B.t7 * sin(Template_v3_08_2020a_B.t14_tmp);
        Template_v3_08_2020a_B.t14_tmp = Template_v3_08_2020a_B.t6 +
          Template_v3_08_2020a_B.t4;
        Template_v3_08_2020a_B.t7 = Template_v3_08_2020a_B.t14_tmp +
          Template_v3_08_2020a_B.t197;
        Template_v3_08_2020a_B.InertiaS[14] = ((Template_v3_08_2020a_B.t7 -
          ((((Template_v3_08_2020a_B.uDLookupTable2 + Template_v3_08_2020a_B.t33)
             + Template_v3_08_2020a_B.rtb_x_ddot_idx_1) +
            Template_v3_08_2020a_B.t135) + Template_v3_08_2020a_B.t117) *
          Template_v3_08_2020a_B.t127) - ((Template_v3_08_2020a_B.Sum6_d +
          Template_v3_08_2020a_B.t97) + Template_v3_08_2020a_B.t109) *
          Template_v3_08_2020a_B.t113) - Template_v3_08_2020a_B.t4_tmp *
          Template_v3_08_2020a_B.t106;
        Template_v3_08_2020a_B.Sum6_d = (Template_v3_08_2020a_B.t24 +
          Template_v3_08_2020a_B.t6) + Template_v3_08_2020a_B.t106 *
          Template_v3_08_2020a_B.t174;
        Template_v3_08_2020a_B.InertiaS[15] = ((((Template_v3_08_2020a_B.Sum6_au
          + Template_v3_08_2020a_B.Sum6_i) + Template_v3_08_2020a_B.t34) +
          Template_v3_08_2020a_B.t110) + Template_v3_08_2020a_B.t114) *
          Template_v3_08_2020a_B.t127 + ((Template_v3_08_2020a_B.Sum6_d +
          Template_v3_08_2020a_B.t4) + Template_v3_08_2020a_B.t197);
        Template_v3_08_2020a_B.InertiaS[16] = (Template_v3_08_2020a_B.Sum6_d +
          Template_v3_08_2020a_B.t37) + Template_v3_08_2020a_B.t17;
        Template_v3_08_2020a_B.InertiaS[17] = (Template_v3_08_2020a_B.t24 +
          Template_v3_08_2020a_B.t21) + Template_v3_08_2020a_B.t14;
        Template_v3_08_2020a_B.InertiaS[18] = Template_v3_08_2020a_B.t7_tmp;
        Template_v3_08_2020a_B.InertiaS[19] = Template_v3_08_2020a_B.t15_tmp;
        Template_v3_08_2020a_B.InertiaS[20] = (((Template_v3_08_2020a_B.t14_tmp
          + Template_v3_08_2020a_B.t35) + Template_v3_08_2020a_B.t197) +
          Template_v3_08_2020a_B.t45) - (((((((((Template_v3_08_2020a_B.Sum6_au
          + Template_v3_08_2020a_B.uDLookupTable2) +
          Template_v3_08_2020a_B.Sum6_i) + Template_v3_08_2020a_B.t33) +
          Template_v3_08_2020a_B.t34) + Template_v3_08_2020a_B.rtb_x_ddot_idx_1)
          + Template_v3_08_2020a_B.t110) + Template_v3_08_2020a_B.t135) +
          Template_v3_08_2020a_B.t114) + Template_v3_08_2020a_B.t117) *
          Template_v3_08_2020a_B.t127;
        Template_v3_08_2020a_B.InertiaS[21] = Template_v3_08_2020a_B.t7;
        Template_v3_08_2020a_B.InertiaS[22] = (Template_v3_08_2020a_B.t6 +
          Template_v3_08_2020a_B.t37) + Template_v3_08_2020a_B.t17;
        Template_v3_08_2020a_B.InertiaS[23] = Template_v3_08_2020a_B.t21 +
          Template_v3_08_2020a_B.t14;
        Template_v3_08_2020a_B.InertiaS[24] = Template_v3_08_2020a_B.t27 +
          Template_v3_08_2020a_B.t25;
        Template_v3_08_2020a_B.InertiaS[25] = Template_v3_08_2020a_B.t23 +
          Template_v3_08_2020a_B.t51;
        Template_v3_08_2020a_B.InertiaS[26] = (((Template_v3_08_2020a_B.t6 +
          Template_v3_08_2020a_B.t35) + Template_v3_08_2020a_B.t85) +
          Template_v3_08_2020a_B.t45) + Template_v3_08_2020a_B.t2;
        Template_v3_08_2020a_B.InertiaS[27] = (Template_v3_08_2020a_B.t6 +
          Template_v3_08_2020a_B.t85) + Template_v3_08_2020a_B.t2;
        Template_v3_08_2020a_B.InertiaS[28] = Template_v3_08_2020a_B.Sum6_l *
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_2 *
          -Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;
        Template_v3_08_2020a_B.InertiaS[29] =
          Template_v3_08_2020a_P.model_param[12] * Template_v3_08_2020a_B.Sum6_l
          * Template_v3_08_2020a_B.t75 * Template_v3_08_2020a_B.t41;
        Template_v3_08_2020a_B.InertiaS[30] = Template_v3_08_2020a_B.t8 * sin
          (Template_v3_08_2020a_B.t44);
        Template_v3_08_2020a_B.InertiaS[31] = -Template_v3_08_2020a_B.t8 * cos
          (Template_v3_08_2020a_B.t44);
        Template_v3_08_2020a_B.t7_tmp = Template_v3_08_2020a_B.t53 +
          Template_v3_08_2020a_B.t52;
        Template_v3_08_2020a_B.InertiaS[32] = Template_v3_08_2020a_B.t7_tmp -
          Template_v3_08_2020a_P.model_param[12] *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l *
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw *
          Template_v3_08_2020a_B.t113;
        Template_v3_08_2020a_B.InertiaS[33] = Template_v3_08_2020a_B.t7_tmp;
        Template_v3_08_2020a_B.InertiaS[34] = Template_v3_08_2020a_B.t53;
        Template_v3_08_2020a_B.InertiaS[35] = 0.0;

        // SignalConversion generated from: '<S697>/ SFunction ' incorporates:
        //   DataStoreRead: '<S695>/Data Store Read1'
        //   DataStoreRead: '<S695>/Data Store Read2'
        //   DataStoreRead: '<S695>/Data Store Read3'
        //   DataStoreRead: '<S695>/Data Store Read4'
        //   DataStoreRead: '<S695>/RED_Fx_Sat1'
        //   DataStoreRead: '<S695>/RED_Fy_Sat1'
        //   DataStoreRead: '<S695>/RED_Tz_Sat1'
        //   MATLAB Function: '<S695>/Dynamics Model'
        //   Sum: '<S695>/Sum1'

        Template_v3_08_2020a_B.X_c[0] = Template_v3_08_2020a_DW.RED_Fx_Sat;
        Template_v3_08_2020a_B.X_c[1] = Template_v3_08_2020a_DW.RED_Fy_Sat;
        Template_v3_08_2020a_B.X_c[2] = Template_v3_08_2020a_DW.RED_Tz_Sat +
          Template_v3_08_2020a_DW.RED_Tz_RW_Sat;
        Template_v3_08_2020a_B.X_c[3] = Template_v3_08_2020a_DW.RED_Tz_Shoulder;
        Template_v3_08_2020a_B.X_c[4] = Template_v3_08_2020a_DW.RED_Tz_Elbow;
        Template_v3_08_2020a_B.X_c[5] = Template_v3_08_2020a_DW.RED_Tz_Wrist;

        // MATLAB Function: '<S695>/Dynamics Model'
        Template_v3_08_2020a_B.dv1[0] = 0.0;
        Template_v3_08_2020a_B.dv1[1] = 0.0;
        Template_v3_08_2020a_B.dv1[2] = 0.0;
        Template_v3_08_2020a_B.dv1[3] = ((tanh(Template_v3_08_2020a_P.Gamma2 *
          rtb_Delay_0[9]) - tanh(Template_v3_08_2020a_P.Gamma3 * rtb_Delay_0[9]))
          * Template_v3_08_2020a_P.Gamma1 + tanh(Template_v3_08_2020a_P.Gamma5 *
          rtb_Delay_0[9]) * Template_v3_08_2020a_P.Gamma4) +
          Template_v3_08_2020a_P.Gamma6 * rtb_Delay_0[9];
        Template_v3_08_2020a_B.dv1[4] = ((tanh(Template_v3_08_2020a_P.Gamma2 *
          rtb_Delay_0[10]) - tanh(Template_v3_08_2020a_P.Gamma3 * rtb_Delay_0[10]))
          * Template_v3_08_2020a_P.Gamma1 + tanh(Template_v3_08_2020a_P.Gamma5 *
          rtb_Delay_0[10]) * Template_v3_08_2020a_P.Gamma4) +
          Template_v3_08_2020a_P.Gamma6 * rtb_Delay_0[10];
        Template_v3_08_2020a_B.dv1[5] = ((tanh(Template_v3_08_2020a_P.Gamma2 *
          rtb_Delay_0[11]) - tanh(Template_v3_08_2020a_P.Gamma3 * rtb_Delay_0[11]))
          * Template_v3_08_2020a_P.Gamma1 + tanh(Template_v3_08_2020a_P.Gamma5 *
          rtb_Delay_0[11]) * Template_v3_08_2020a_P.Gamma4) +
          Template_v3_08_2020a_P.Gamma6 * rtb_Delay_0[11];
        for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 6;
             Template_v3_08_2020a_B.i++) {
          Template_v3_08_2020a_B.dv[Template_v3_08_2020a_B.i] = 0.0;
          for (Template_v3_08_2020a_B.uElOffset1 = 0;
               Template_v3_08_2020a_B.uElOffset1 < 6;
               Template_v3_08_2020a_B.uElOffset1++) {
            Template_v3_08_2020a_B.dv[Template_v3_08_2020a_B.i] +=
              Template_v3_08_2020a_B.InertiaS[6 *
              Template_v3_08_2020a_B.uElOffset1 + Template_v3_08_2020a_B.i] *
              rtb_Delay_0[Template_v3_08_2020a_B.uElOffset1 + 6];
          }

          Template_v3_08_2020a_B.s[Template_v3_08_2020a_B.i] =
            (Template_v3_08_2020a_B.X_c[Template_v3_08_2020a_B.i] -
             Template_v3_08_2020a_B.dv[Template_v3_08_2020a_B.i]) -
            Template_v3_08_2020a_B.dv1[Template_v3_08_2020a_B.i];
        }

        for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 6;
             Template_v3_08_2020a_B.i++) {
          Template_v3_08_2020a_B.X_c[Template_v3_08_2020a_B.i] = 0.0;
          for (Template_v3_08_2020a_B.uElOffset1 = 0;
               Template_v3_08_2020a_B.uElOffset1 < 6;
               Template_v3_08_2020a_B.uElOffset1++) {
            Template_v3_08_2020a_B.X_c[Template_v3_08_2020a_B.i] +=
              Template_v3_08_2020a_B.X[6 * Template_v3_08_2020a_B.uElOffset1 +
              Template_v3_08_2020a_B.i] *
              Template_v3_08_2020a_B.s[Template_v3_08_2020a_B.uElOffset1];
          }

          // DiscreteIntegrator: '<S695>/Acceleration  to Velocity'
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[Template_v3_08_2020a_B.i]
            = rtb_Delay_0[Template_v3_08_2020a_B.i + 6] *
            Template_v3_08_2020a_P.AccelerationtoVelocity_gainval +
            Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_o[Template_v3_08_2020a_B.i];
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[Template_v3_08_2020a_B.i
            + 6] =
            Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_o[Template_v3_08_2020a_B.i
            + 6] + Template_v3_08_2020a_P.AccelerationtoVelocity_gainval *
            Template_v3_08_2020a_B.X_c[Template_v3_08_2020a_B.i];
        }

        // DataStoreWrite: '<S695>/Data Store Write'
        Template_v3_08_2020a_DW.RED_Rz_Shoulder =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[3];

        // DataStoreWrite: '<S695>/Data Store Write1'
        Template_v3_08_2020a_DW.RED_Rz_Elbow =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[4];

        // DataStoreWrite: '<S695>/Data Store Write2'
        Template_v3_08_2020a_DW.RED_Rz_Wrist =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[5];

        // DataStoreWrite: '<S695>/Data Store Write6'
        Template_v3_08_2020a_DW.RED_RzD_Shoulder =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[9];

        // DataStoreWrite: '<S695>/Data Store Write7'
        Template_v3_08_2020a_DW.RED_RzD_Elbow =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[10];

        // DataStoreWrite: '<S695>/Data Store Write8'
        Template_v3_08_2020a_DW.RED_RzD_Wrist =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[11];

        // Delay: '<S700>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_nt != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_po =
            Template_v3_08_2020a_B.AccelerationtoVelocity_o[6];
        }

        // Sum: '<S700>/Sum6' incorporates:
        //   Delay: '<S700>/Delay1'

        Template_v3_08_2020a_B.Sum6_l =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[6] -
          Template_v3_08_2020a_DW.Delay1_DSTATE_po;

        // If: '<S700>/if we went through a "step"' incorporates:
        //   Inport: '<S702>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S700>/Hold this value' incorporates:
          //   ActionPort: '<S702>/Action Port'

          Template_v3_08_2020a_B.In1_h = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S700>/Hold this value'
        }

        // End of If: '<S700>/if we went through a "step"'

        // Sum: '<S695>/Sum2' incorporates:
        //   DataStoreWrite: '<S695>/RED_IMU_Ay1'
        //   Gain: '<S700>/divide by delta T'
        //   RandomNumber: '<S695>/Random Number2'

        Template_v3_08_2020a_DW.RED_IMU_Ax_I = 1.0 /
          Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_h +
          Template_v3_08_2020a_DW.NextOutput_c2;

        // Delay: '<S699>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_el != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_kx =
            Template_v3_08_2020a_B.AccelerationtoVelocity_o[7];
        }

        // Sum: '<S699>/Sum6' incorporates:
        //   Delay: '<S699>/Delay1'

        Template_v3_08_2020a_B.Sum6_l =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[7] -
          Template_v3_08_2020a_DW.Delay1_DSTATE_kx;

        // If: '<S699>/if we went through a "step"' incorporates:
        //   Inport: '<S701>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S699>/Hold this value' incorporates:
          //   ActionPort: '<S701>/Action Port'

          Template_v3_08_2020a_B.In1_lg = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S699>/Hold this value'
        }

        // End of If: '<S699>/if we went through a "step"'

        // Sum: '<S695>/Sum3' incorporates:
        //   DataStoreWrite: '<S695>/RED_IMU_Ay_I1'
        //   Gain: '<S699>/divide by delta T'
        //   RandomNumber: '<S695>/Random Number1'

        Template_v3_08_2020a_DW.RED_IMU_Ay_I = 1.0 /
          Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_lg +
          Template_v3_08_2020a_DW.NextOutput_lz;

        // Sum: '<S695>/Sum4' incorporates:
        //   DataStoreWrite: '<S695>/RED_IMU_R1'
        //   RandomNumber: '<S695>/Random Number3'

        Template_v3_08_2020a_DW.RED_IMU_R =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[8] +
          Template_v3_08_2020a_DW.NextOutput_dv;

        // DataStoreWrite: '<S695>/RED_Px1'
        Template_v3_08_2020a_DW.RED_Px =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[0];

        // DataStoreWrite: '<S695>/RED_Py1'
        Template_v3_08_2020a_DW.RED_Py =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[1];

        // DataStoreWrite: '<S695>/RED_Rz1'
        Template_v3_08_2020a_DW.RED_Rz =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[2];

        // DataStoreWrite: '<S695>/RED_RzD1'
        Template_v3_08_2020a_DW.RED_RzD =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[8];

        // DataStoreWrite: '<S695>/RED_Vx1'
        Template_v3_08_2020a_DW.RED_Vx =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[6];

        // DataStoreWrite: '<S695>/RED_Vy1'
        Template_v3_08_2020a_DW.RED_Vy =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[7];

        // Update for Delay: '<S695>/Delay'
        memcpy(&Template_v3_08_2020a_DW.Delay_DSTATE[0],
               &Template_v3_08_2020a_B.AccelerationtoVelocity_o[0], 12U * sizeof
               (real_T));

        // Update for DiscreteIntegrator: '<S695>/Acceleration  to Velocity' incorporates:
        //   Delay: '<S695>/Delay'

        memcpy(&Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_o[0],
               &Template_v3_08_2020a_B.AccelerationtoVelocity_o[0], 12U * sizeof
               (real_T));

        // Update for Delay: '<S700>/Delay1'
        Template_v3_08_2020a_DW.icLoad_nt = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_po =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[6];

        // Update for RandomNumber: '<S695>/Random Number2'
        Template_v3_08_2020a_DW.NextOutput_c2 = Temp_rt_nrand_Upu32_Yd_f_pw_snf(
          &Template_v3_08_2020a_DW.RandSeed_o) *
          Template_v3_08_2020a_P.RandomNumber2_StdDev +
          Template_v3_08_2020a_P.RandomNumber2_Mean;

        // Update for Delay: '<S699>/Delay1'
        Template_v3_08_2020a_DW.icLoad_el = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_kx =
          Template_v3_08_2020a_B.AccelerationtoVelocity_o[7];

        // Update for RandomNumber: '<S695>/Random Number1'
        Template_v3_08_2020a_DW.NextOutput_lz = Temp_rt_nrand_Upu32_Yd_f_pw_snf(
          &Template_v3_08_2020a_DW.RandSeed_p) *
          Template_v3_08_2020a_P.RandomNumber1_StdDev +
          Template_v3_08_2020a_P.RandomNumber1_Mean;

        // Update for RandomNumber: '<S695>/Random Number3'
        Template_v3_08_2020a_DW.NextOutput_dv = Temp_rt_nrand_Upu32_Yd_f_pw_snf(
          &Template_v3_08_2020a_DW.RandSeed_h) *
          Template_v3_08_2020a_P.RandomNumber3_StdDev +
          Template_v3_08_2020a_P.RandomNumber3_Mean;

        // End of Outputs for SubSystem: '<S665>/RED + ARM'
      } else {
        // Outputs for IfAction SubSystem: '<S665>/RED Only' incorporates:
        //   ActionPort: '<S694>/Action Port'

        // DiscreteIntegrator: '<S704>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S703>/RED_Fx_Sat'
        //   MATLAB Function: '<S704>/MATLAB Function'
        //   SignalConversion generated from: '<S710>/ SFunction '

        // MATLAB Function 'Simulate Plant Dynamics/RED and ARM Dynamics/RED Only/RED Dynamics/RED Dynamics Model/MATLAB Function': '<S710>:1' 
        // '<S710>:1:5' x_ddot     = zeros(3,1);
        // '<S710>:1:9' m_RED    = model_param(1);
        // '<S710>:1:10' I_RED    = model_param(2);
        // '<S710>:1:13' Fx        = control_inputs(1);
        // '<S710>:1:14' Fy        = control_inputs(2);
        // '<S710>:1:15' Tz        = control_inputs(3);
        // '<S710>:1:18' x_ddot(1) = Fx/m_RED;
        // '<S710>:1:19' x_ddot(2) = Fy/m_RED;
        // '<S710>:1:20' x_ddot(3) = Tz/I_RED;
        Template_v3_08_2020a_B.uDLookupTable2 =
          Template_v3_08_2020a_DW.RED_Fx_Sat /
          Template_v3_08_2020a_P.model_param[0] *
          Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_k +
          Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[0];

        // DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_B.Sum6_au =
          Template_v3_08_2020a_P.VelocitytoPosition_gainval *
          Template_v3_08_2020a_B.uDLookupTable2 +
          Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[0];

        // Sum: '<S704>/Sum' incorporates:
        //   RandomNumber: '<S704>/Random Number'

        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_au +
          Template_v3_08_2020a_DW.NextOutput_lo;
        Template_v3_08_2020a_B.Sum6_i = Template_v3_08_2020a_B.uDLookupTable2;
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l =
          Template_v3_08_2020a_B.Sum6_au;

        // DiscreteIntegrator: '<S704>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S703>/RED_Fy_Sat'
        //   DiscreteIntegrator: '<S704>/Velocity to Position'
        //   MATLAB Function: '<S704>/MATLAB Function'
        //   SignalConversion generated from: '<S710>/ SFunction '

        Template_v3_08_2020a_B.uDLookupTable2 =
          Template_v3_08_2020a_DW.RED_Fy_Sat /
          Template_v3_08_2020a_P.model_param[0] *
          Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_k +
          Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[1];

        // DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_B.Sum6_au =
          Template_v3_08_2020a_P.VelocitytoPosition_gainval *
          Template_v3_08_2020a_B.uDLookupTable2 +
          Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[1];

        // Sum: '<S704>/Sum' incorporates:
        //   RandomNumber: '<S704>/Random Number'

        Template_v3_08_2020a_B.rtb_Sum_ht_idx_1 = Template_v3_08_2020a_B.Sum6_au
          + Template_v3_08_2020a_DW.NextOutput_lo;
        Template_v3_08_2020a_B.rtb_x_ddot_idx_1 =
          Template_v3_08_2020a_B.uDLookupTable2;
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw =
          Template_v3_08_2020a_B.Sum6_au;

        // DiscreteIntegrator: '<S704>/Acceleration  to Velocity' incorporates:
        //   DataStoreRead: '<S703>/Data Store Read'
        //   DataStoreRead: '<S703>/RED_Tz_Sat'
        //   DiscreteIntegrator: '<S704>/Velocity to Position'
        //   MATLAB Function: '<S704>/MATLAB Function'
        //   Sum: '<S703>/Sum'

        Template_v3_08_2020a_B.uDLookupTable2 =
          (Template_v3_08_2020a_DW.RED_Tz_Sat +
           Template_v3_08_2020a_DW.RED_Tz_RW_Sat) /
          Template_v3_08_2020a_P.model_param[1] *
          Template_v3_08_2020a_P.AccelerationtoVelocity_gainva_k +
          Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[2];

        // DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_B.Sum6_au =
          Template_v3_08_2020a_P.VelocitytoPosition_gainval *
          Template_v3_08_2020a_B.uDLookupTable2 +
          Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[2];

        // Sum: '<S704>/Sum' incorporates:
        //   RandomNumber: '<S704>/Random Number'

        Template_v3_08_2020a_B.rtb_Sum_ht_idx_2 = Template_v3_08_2020a_B.Sum6_au
          + Template_v3_08_2020a_DW.NextOutput_lo;

        // Delay: '<S706>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_ec != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_gy =
            Template_v3_08_2020a_B.Sum6_d;
        }

        // Sum: '<S706>/Sum6' incorporates:
        //   Delay: '<S706>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Sum6_d -
          Template_v3_08_2020a_DW.Delay1_DSTATE_gy;

        // If: '<S706>/if we went through a "step"' incorporates:
        //   Inport: '<S712>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S706>/Hold this value' incorporates:
          //   ActionPort: '<S712>/Action Port'

          Template_v3_08_2020a_B.In1_ad = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S706>/Hold this value'
        }

        // End of If: '<S706>/if we went through a "step"'

        // Gain: '<S706>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vx'

        Template_v3_08_2020a_DW.RED_Vx = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_ad;

        // Delay: '<S708>/Delay1' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vx'

        if (Template_v3_08_2020a_DW.icLoad_by != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_m =
            Template_v3_08_2020a_DW.RED_Vx;
        }

        // Sum: '<S708>/Sum6' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vx'
        //   Delay: '<S708>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.RED_Vx -
          Template_v3_08_2020a_DW.Delay1_DSTATE_m;

        // If: '<S708>/if we went through a "step"' incorporates:
        //   Inport: '<S714>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S708>/Hold this value' incorporates:
          //   ActionPort: '<S714>/Action Port'

          Template_v3_08_2020a_B.In1_pk = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S708>/Hold this value'
        }

        // End of If: '<S708>/if we went through a "step"'

        // Sum: '<S703>/Sum7' incorporates:
        //   DataStoreWrite: '<S703>/RED_IMU_Ay'
        //   Gain: '<S708>/divide by delta T'
        //   RandomNumber: '<S703>/Random Number7'

        Template_v3_08_2020a_DW.RED_IMU_Ax_I = 1.0 /
          Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_pk +
          Template_v3_08_2020a_DW.NextOutput_de;

        // Delay: '<S705>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_oe != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_et =
            Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;
        }

        // Sum: '<S705>/Sum6' incorporates:
        //   Delay: '<S705>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.rtb_Sum_ht_idx_1
          - Template_v3_08_2020a_DW.Delay1_DSTATE_et;

        // If: '<S705>/if we went through a "step"' incorporates:
        //   Inport: '<S711>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S705>/Hold this value' incorporates:
          //   ActionPort: '<S711>/Action Port'

          Template_v3_08_2020a_B.In1_kj = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S705>/Hold this value'
        }

        // End of If: '<S705>/if we went through a "step"'

        // Gain: '<S705>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vy'

        Template_v3_08_2020a_DW.RED_Vy = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_kj;

        // Delay: '<S709>/Delay1' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vy'

        if (Template_v3_08_2020a_DW.icLoad_mq != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_ln =
            Template_v3_08_2020a_DW.RED_Vy;
        }

        // Sum: '<S709>/Sum6' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vy'
        //   Delay: '<S709>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_DW.RED_Vy -
          Template_v3_08_2020a_DW.Delay1_DSTATE_ln;

        // If: '<S709>/if we went through a "step"' incorporates:
        //   Inport: '<S715>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S709>/Hold this value' incorporates:
          //   ActionPort: '<S715>/Action Port'

          Template_v3_08_2020a_B.In1_lk = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S709>/Hold this value'
        }

        // End of If: '<S709>/if we went through a "step"'

        // Sum: '<S703>/Sum8' incorporates:
        //   DataStoreWrite: '<S703>/RED_IMU_Ay_I'
        //   Gain: '<S709>/divide by delta T'
        //   RandomNumber: '<S703>/Random Number6'

        Template_v3_08_2020a_DW.RED_IMU_Ay_I = 1.0 /
          Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_lk +
          Template_v3_08_2020a_DW.NextOutput_c;

        // Delay: '<S707>/Delay1'
        if (Template_v3_08_2020a_DW.icLoad_ov != 0) {
          Template_v3_08_2020a_DW.Delay1_DSTATE_hs =
            Template_v3_08_2020a_B.rtb_Sum_ht_idx_2;
        }

        // Sum: '<S707>/Sum6' incorporates:
        //   Delay: '<S707>/Delay1'

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.rtb_Sum_ht_idx_2
          - Template_v3_08_2020a_DW.Delay1_DSTATE_hs;

        // If: '<S707>/if we went through a "step"' incorporates:
        //   Inport: '<S713>/In1'

        if (Template_v3_08_2020a_B.Sum6_l != 0.0) {
          // Outputs for IfAction SubSystem: '<S707>/Hold this value' incorporates:
          //   ActionPort: '<S713>/Action Port'

          Template_v3_08_2020a_B.In1_k1 = Template_v3_08_2020a_B.Sum6_l;

          // End of Outputs for SubSystem: '<S707>/Hold this value'
        }

        // End of If: '<S707>/if we went through a "step"'

        // Gain: '<S707>/divide by delta T' incorporates:
        //   DataStoreWrite: '<S703>/RED_RzD'

        Template_v3_08_2020a_DW.RED_RzD = 1.0 / Template_v3_08_2020a_P.baseRate *
          Template_v3_08_2020a_B.In1_k1;

        // Sum: '<S703>/Sum9' incorporates:
        //   DataStoreWrite: '<S703>/RED_IMU_R'
        //   DataStoreWrite: '<S703>/RED_RzD'
        //   RandomNumber: '<S703>/Random Number8'

        Template_v3_08_2020a_DW.RED_IMU_R = Template_v3_08_2020a_DW.RED_RzD +
          Template_v3_08_2020a_DW.NextOutput_h;

        // DataStoreWrite: '<S703>/RED_Px'
        Template_v3_08_2020a_DW.RED_Px = Template_v3_08_2020a_B.Sum6_d;

        // DataStoreWrite: '<S703>/RED_Py'
        Template_v3_08_2020a_DW.RED_Py = Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;

        // DataStoreWrite: '<S703>/RED_Rz'
        Template_v3_08_2020a_DW.RED_Rz = Template_v3_08_2020a_B.rtb_Sum_ht_idx_2;

        // Update for DiscreteIntegrator: '<S704>/Acceleration  to Velocity'
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[0] =
          Template_v3_08_2020a_B.Sum6_i;

        // Update for DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[0] =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_l;

        // Update for DiscreteIntegrator: '<S704>/Acceleration  to Velocity'
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[1] =
          Template_v3_08_2020a_B.rtb_x_ddot_idx_1;

        // Update for DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[1] =
          Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_jw;

        // Update for DiscreteIntegrator: '<S704>/Acceleration  to Velocity'
        Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[2] =
          Template_v3_08_2020a_B.uDLookupTable2;

        // Update for DiscreteIntegrator: '<S704>/Velocity to Position'
        Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[2] =
          Template_v3_08_2020a_B.Sum6_au;

        // Update for RandomNumber: '<S704>/Random Number'
        Template_v3_08_2020a_DW.NextOutput_lo = Temp_rt_nrand_Upu32_Yd_f_pw_snf(
          &Template_v3_08_2020a_DW.RandSeed_mt) * sqrt
          (Template_v3_08_2020a_P.noise_variance_RED) +
          Template_v3_08_2020a_P.RandomNumber_Mean;

        // Update for Delay: '<S706>/Delay1'
        Template_v3_08_2020a_DW.icLoad_ec = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_gy = Template_v3_08_2020a_B.Sum6_d;

        // Update for Delay: '<S708>/Delay1' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vx'

        Template_v3_08_2020a_DW.icLoad_by = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_m = Template_v3_08_2020a_DW.RED_Vx;

        // Update for RandomNumber: '<S703>/Random Number7'
        Template_v3_08_2020a_DW.NextOutput_de = Temp_rt_nrand_Upu32_Yd_f_pw_snf(
          &Template_v3_08_2020a_DW.RandSeed_ka) *
          Template_v3_08_2020a_P.RandomNumber7_StdDev +
          Template_v3_08_2020a_P.RandomNumber7_Mean;

        // Update for Delay: '<S705>/Delay1'
        Template_v3_08_2020a_DW.icLoad_oe = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_et =
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_1;

        // Update for Delay: '<S709>/Delay1' incorporates:
        //   DataStoreWrite: '<S703>/RED_Vy'

        Template_v3_08_2020a_DW.icLoad_mq = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_ln =
          Template_v3_08_2020a_DW.RED_Vy;

        // Update for RandomNumber: '<S703>/Random Number6'
        Template_v3_08_2020a_DW.NextOutput_c = Temp_rt_nrand_Upu32_Yd_f_pw_snf
          (&Template_v3_08_2020a_DW.RandSeed_g) *
          Template_v3_08_2020a_P.RandomNumber6_StdDev +
          Template_v3_08_2020a_P.RandomNumber6_Mean;

        // Update for Delay: '<S707>/Delay1'
        Template_v3_08_2020a_DW.icLoad_ov = 0U;
        Template_v3_08_2020a_DW.Delay1_DSTATE_hs =
          Template_v3_08_2020a_B.rtb_Sum_ht_idx_2;

        // Update for RandomNumber: '<S703>/Random Number8'
        Template_v3_08_2020a_DW.NextOutput_h = Temp_rt_nrand_Upu32_Yd_f_pw_snf
          (&Template_v3_08_2020a_DW.RandSeed_f) *
          Template_v3_08_2020a_P.RandomNumber8_StdDev +
          Template_v3_08_2020a_P.RandomNumber8_Mean;

        // End of Outputs for SubSystem: '<S665>/RED Only'
      }

      // End of If: '<S665>/If'

      // Update for DiscreteIntegrator: '<S666>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[0] =
        Template_v3_08_2020a_B.x_ddot_b[0];

      // Update for DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[0] =
        Template_v3_08_2020a_B.VelocitytoPosition[0];

      // Update for DiscreteIntegrator: '<S666>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[1] =
        Template_v3_08_2020a_B.x_ddot_b[1];

      // Update for DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[1] =
        Template_v3_08_2020a_B.VelocitytoPosition[1];

      // Update for DiscreteIntegrator: '<S666>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[2] =
        Template_v3_08_2020a_B.u0;

      // Update for DiscreteIntegrator: '<S666>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[2] =
        Template_v3_08_2020a_B.VelocitytoPosition_g;

      // Update for RandomNumber: '<S666>/Random Number'
      Template_v3_08_2020a_DW.NextOutput = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed) * sqrt
        (Template_v3_08_2020a_P.noise_variance_BLACK) +
        Template_v3_08_2020a_P.RandomNumber_Mean_n;

      // Update for Delay: '<S671>/Delay1'
      Template_v3_08_2020a_DW.icLoad_bb = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_p = Template_v3_08_2020a_B.Sum_l[0];

      // Update for Delay: '<S669>/Delay1' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vx'

      Template_v3_08_2020a_DW.icLoad_p = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_ja =
        Template_v3_08_2020a_DW.BLACK_Vx;

      // Update for RandomNumber: '<S662>/Random Number1'
      Template_v3_08_2020a_DW.NextOutput_p = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed_m) *
        Template_v3_08_2020a_P.RandomNumber1_StdDev_o +
        Template_v3_08_2020a_P.RandomNumber1_Mean_i;

      // Update for Delay: '<S667>/Delay1'
      Template_v3_08_2020a_DW.icLoad_lv = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_d4 = Template_v3_08_2020a_B.Sum_l[1];

      // Update for Delay: '<S670>/Delay1' incorporates:
      //   DataStoreWrite: '<S662>/BLACK_Vy'

      Template_v3_08_2020a_DW.icLoad_hu = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_j3 =
        Template_v3_08_2020a_DW.BLACK_Vy;

      // Update for RandomNumber: '<S662>/Random Number'
      Template_v3_08_2020a_DW.NextOutput_o = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed_d) *
        Template_v3_08_2020a_P.RandomNumber_StdDev +
        Template_v3_08_2020a_P.RandomNumber_Mean_e;

      // Update for Delay: '<S668>/Delay1'
      Template_v3_08_2020a_DW.icLoad_m = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_kmw = Template_v3_08_2020a_B.Sum_l[2];

      // Update for RandomNumber: '<S662>/Random Number2'
      Template_v3_08_2020a_DW.NextOutput_j = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed_k) *
        Template_v3_08_2020a_P.RandomNumber2_StdDev_p +
        Template_v3_08_2020a_P.RandomNumber2_Mean_c;

      // Update for DiscreteIntegrator: '<S678>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[0] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_idx_0;

      // Update for DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[0] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFu_nz;

      // Update for DiscreteIntegrator: '<S678>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[1] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_idx_1;

      // Update for DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[1] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_p;

      // Update for DiscreteIntegrator: '<S678>/Acceleration  to Velocity'
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[2] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;

      // Update for DiscreteIntegrator: '<S678>/Velocity to Position'
      Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[2] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;

      // Update for RandomNumber: '<S678>/Random Number'
      Template_v3_08_2020a_DW.NextOutput_g = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed_d3) * sqrt
        (Template_v3_08_2020a_P.noise_variance_BLUE) +
        Template_v3_08_2020a_P.RandomNumber_Mean_b;

      // Update for Delay: '<S683>/Delay1'
      Template_v3_08_2020a_DW.icLoad_dc = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_e =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_0;

      // Update for Delay: '<S681>/Delay1' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vx'

      Template_v3_08_2020a_DW.icLoad_ln = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_fi = Template_v3_08_2020a_DW.BLUE_Vx;

      // Update for RandomNumber: '<S663>/Random Number4'
      Temp_rt_nrand_Upu32_Yd_f_pw_snf(&Template_v3_08_2020a_DW.RandSeed_dc);

      // Update for Delay: '<S679>/Delay1'
      Template_v3_08_2020a_DW.icLoad_oc = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_bo =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_1;

      // Update for Delay: '<S682>/Delay1' incorporates:
      //   DataStoreWrite: '<S663>/BLUE_Vy'

      Template_v3_08_2020a_DW.icLoad_l1 = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_fq = Template_v3_08_2020a_DW.BLUE_Vy;

      // Update for RandomNumber: '<S663>/Random Number3'
      Temp_rt_nrand_Upu32_Yd_f_pw_snf(&Template_v3_08_2020a_DW.RandSeed_j);

      // Update for Delay: '<S680>/Delay1'
      Template_v3_08_2020a_DW.icLoad_lz = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_od =
        Template_v3_08_2020a_B.rtb_Sum_lh_idx_2;

      // Update for RandomNumber: '<S663>/Random Number5'
      Template_v3_08_2020a_DW.NextOutput_k = Temp_rt_nrand_Upu32_Yd_f_pw_snf
        (&Template_v3_08_2020a_DW.RandSeed_c) *
        Template_v3_08_2020a_P.RandomNumber5_StdDev +
        Template_v3_08_2020a_P.RandomNumber5_Mean;

      // Update for DiscreteIntegrator: '<S664>/Discrete-Time Integrator' incorporates:
      //   DataStoreRead: '<S664>/Data Store Read3'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator_IC_LOADI = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator_DSTATE +=
        Template_v3_08_2020a_P.DiscreteTimeIntegrator_gainval *
        Template_v3_08_2020a_B.Subtract_o;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator_PrevRese =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_j);

      // Update for DiscreteIntegrator: '<S664>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S664>/RED_Px1'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LOAD = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTATE =
        Template_v3_08_2020a_DW.BLACK_IMU_Psi;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevRes =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_n_m);

      // Update for DiscreteIntegrator: '<S664>/Discrete-Time Integrator2' incorporates:
      //   DataStoreWrite: '<S664>/RED_Px2'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_IC_LOAD = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_DSTATE =
        Template_v3_08_2020a_DW.RED_IMU_Psi;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_PrevRes =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_f_m);

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
    //   DataStoreRead: '<S1>/RED_RzD_Elbow'
    //   DataStoreRead: '<S1>/RED_RzD_Shoulder'
    //   DataStoreRead: '<S1>/RED_RzD_Wrist'
    //   DataStoreRead: '<S1>/RED_Rz_Elbow'
    //   DataStoreRead: '<S1>/RED_Rz_Shoulder'
    //   DataStoreRead: '<S1>/RED_Rz_Wrist'
    //   DataStoreRead: '<S1>/RED_Tz_Elbow'
    //   DataStoreRead: '<S1>/RED_Tz_Sat'
    //   DataStoreRead: '<S1>/RED_Tz_Shoulder'
    //   DataStoreRead: '<S1>/RED_Tz_Wrist'
    //   DataStoreRead: '<S1>/RED_Vx '
    //   DataStoreRead: '<S1>/RED_Vy'
    //   DataStoreRead: '<S1>/RED_Vz'
    //   DataStoreRead: '<S1>/Universal_Time'

    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[0] =
      Template_v3_08_2020a_DW.Univ_Time;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[1] =
      Template_v3_08_2020a_DW.RED_Fx_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[2] =
      Template_v3_08_2020a_DW.RED_Fy_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[3] =
      Template_v3_08_2020a_DW.RED_Tz_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[4] =
      Template_v3_08_2020a_DW.RED_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[5] =
      Template_v3_08_2020a_DW.RED_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[6] =
      Template_v3_08_2020a_DW.RED_Rz;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[7] =
      Template_v3_08_2020a_DW.RED_Vx;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[8] =
      Template_v3_08_2020a_DW.RED_Vy;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[9] =
      Template_v3_08_2020a_DW.RED_RzD;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[10] =
      Template_v3_08_2020a_DW.RED_AHRS_Q;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[11] =
      Template_v3_08_2020a_DW.RED_AHRS_P;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[12] =
      Template_v3_08_2020a_DW.RED_AHRS_R;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[13] =
      Template_v3_08_2020a_DW.RED_IMU_Ax_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[14] =
      Template_v3_08_2020a_DW.RED_IMU_Ay_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[15] =
      Template_v3_08_2020a_DW.RED_IMU_Az_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[16] =
      Template_v3_08_2020a_DW.RED_IMU_Ax_I;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[17] =
      Template_v3_08_2020a_DW.RED_IMU_Ay_I;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[18] =
      Template_v3_08_2020a_DW.RED_Ax;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[19] =
      Template_v3_08_2020a_DW.RED_Ay;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[20] =
      Template_v3_08_2020a_DW.RED_RzDD;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[21] =
      Template_v3_08_2020a_DW.BLACK_Fx_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[22] =
      Template_v3_08_2020a_DW.BLACK_Fy_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[23] =
      Template_v3_08_2020a_DW.BLACK_Tz_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[24] =
      Template_v3_08_2020a_DW.BLACK_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[25] =
      Template_v3_08_2020a_DW.BLACK_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[26] =
      Template_v3_08_2020a_DW.BLACK_Rz;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[27] =
      Template_v3_08_2020a_DW.BLACK_Vx;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[28] =
      Template_v3_08_2020a_DW.BLACK_Vy;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[29] =
      Template_v3_08_2020a_DW.BLACK_RzD;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[30] =
      Template_v3_08_2020a_DW.BLACK_AHRS_Q;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[31] =
      Template_v3_08_2020a_DW.BLACK_AHRS_P;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[32] =
      Template_v3_08_2020a_DW.BLACK_AHRS_R;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[33] =
      Template_v3_08_2020a_DW.BLACK_IMU_Ax_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[34] =
      Template_v3_08_2020a_DW.BLACK_IMU_Ay_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[35] =
      Template_v3_08_2020a_DW.BLACK_IMU_Az_b;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[36] =
      Template_v3_08_2020a_DW.BLACK_IMU_Ax_I;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[37] =
      Template_v3_08_2020a_DW.BLACK_IMU_Ay_I;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[38] =
      Template_v3_08_2020a_DW.BLACK_Ax;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[39] =
      Template_v3_08_2020a_DW.BLACK_Ay;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[40] =
      Template_v3_08_2020a_DW.BLACK_RzDD;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[41] =
      Template_v3_08_2020a_DW.BLUE_Fx_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[42] =
      Template_v3_08_2020a_DW.BLUE_Fy_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[43] =
      Template_v3_08_2020a_DW.BLUE_Tz_Sat;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[44] =
      Template_v3_08_2020a_DW.BLUE_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[45] =
      Template_v3_08_2020a_DW.BLUE_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[46] =
      Template_v3_08_2020a_DW.BLUE_Rz;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[47] =
      Template_v3_08_2020a_DW.BLUE_Vx;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[48] =
      Template_v3_08_2020a_DW.BLUE_Vy;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[49] =
      Template_v3_08_2020a_DW.BLUE_RzD;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[50] =
      Template_v3_08_2020a_DW.RED_IMU_Q;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[51] =
      Template_v3_08_2020a_DW.RED_IMU_P;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[52] =
      Template_v3_08_2020a_DW.RED_IMU_R;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[53] =
      Template_v3_08_2020a_DW.BLACK_IMU_Q;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[54] =
      Template_v3_08_2020a_DW.BLACK_IMU_P;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[55] =
      Template_v3_08_2020a_DW.BLACK_IMU_R;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[56] =
      Template_v3_08_2020a_DW.RED_IMU_Psi;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[57] =
      Template_v3_08_2020a_DW.BLACK_IMU_Psi;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[58] =
      Template_v3_08_2020a_DW.ARM_Elbow_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[59] =
      Template_v3_08_2020a_DW.ARM_Elbow_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[60] =
      Template_v3_08_2020a_DW.ARM_Wrist_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[61] =
      Template_v3_08_2020a_DW.ARM_Wrist_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[62] =
      Template_v3_08_2020a_DW.ARM_EndEff_Px;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[63] =
      Template_v3_08_2020a_DW.ARM_EndEff_Py;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[64] =
      Template_v3_08_2020a_DW.RED_Rz_Shoulder;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[65] =
      Template_v3_08_2020a_DW.RED_Rz_Elbow;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[66] =
      Template_v3_08_2020a_DW.RED_Rz_Wrist;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[67] =
      Template_v3_08_2020a_DW.RED_RzD_Shoulder;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[68] =
      Template_v3_08_2020a_DW.RED_RzD_Elbow;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[69] =
      Template_v3_08_2020a_DW.RED_RzD_Wrist;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[70] =
      Template_v3_08_2020a_DW.RED_Tz_Shoulder;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[71] =
      Template_v3_08_2020a_DW.RED_Tz_Elbow;
    Template_v3_08_2020a_B.TmpSignalConversionAtToWork[72] =
      Template_v3_08_2020a_DW.RED_Tz_Wrist;

    // ToWorkspace: '<S1>/To Workspace'
    rt_UpdateLogVar((LogVar *)(LogVar*)
                    (Template_v3_08_2020a_DW.ToWorkspace_PWORK.LoggedData),
                    &Template_v3_08_2020a_B.TmpSignalConversionAtToWork[0], 0);

    // If: '<S2>/This IF block determines whether or not to run the exp code' incorporates:
    //   Constant: '<S22>/Constant'

    if (Template_v3_08_2020a_P.simMode == 0.0) {
      // Outputs for IfAction SubSystem: '<S2>/Change Behavior' incorporates:
      //   ActionPort: '<S21>/Action Port'

      // MATLABSystem: '<S23>/Digital Write' incorporates:
      //   DataStoreRead: '<S21>/Data Store Read'

      Template_v3_08_2020a_B.t7_tmp = rt_roundd_snf
        (Template_v3_08_2020a_DW.Magnet_State);
      if (Template_v3_08_2020a_B.t7_tmp < 256.0) {
        if (Template_v3_08_2020a_B.t7_tmp >= 0.0) {
          Template_v3_08_2020a_B.status = static_cast<uint8_T>
            (Template_v3_08_2020a_B.t7_tmp);
        } else {
          Template_v3_08_2020a_B.status = 0U;
        }
      } else {
        Template_v3_08_2020a_B.status = MAX_uint8_T;
      }

      MW_gpioWrite(10U, Template_v3_08_2020a_B.status);

      // End of MATLABSystem: '<S23>/Digital Write'

      // MATLABSystem: '<S24>/Digital Write' incorporates:
      //   DataStoreRead: '<S21>/Data Store Read1'

      Template_v3_08_2020a_B.t7_tmp = rt_roundd_snf
        (Template_v3_08_2020a_DW.Float_State);
      if (Template_v3_08_2020a_B.t7_tmp < 256.0) {
        if (Template_v3_08_2020a_B.t7_tmp >= 0.0) {
          Template_v3_08_2020a_B.status = static_cast<uint8_T>
            (Template_v3_08_2020a_B.t7_tmp);
        } else {
          Template_v3_08_2020a_B.status = 0U;
        }
      } else {
        Template_v3_08_2020a_B.status = MAX_uint8_T;
      }

      MW_gpioWrite(26U, Template_v3_08_2020a_B.status);

      // End of MATLABSystem: '<S24>/Digital Write'
      // End of Outputs for SubSystem: '<S2>/Change Behavior'
    }

    // End of If: '<S2>/This IF block determines whether or not to run the exp code' 

    // If: '<S3>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
    //   Constant: '<S28>/Constant'
    //   DataStoreRead: '<S3>/Data Store Read'

    if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) &&
        (Template_v3_08_2020a_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S3>/Change RED Behavior' incorporates:
      //   ActionPort: '<S27>/Action Port'

      // MATLABSystem: '<S27>/Send Torque Commands to Manipulator' incorporates:
      //   DataStoreRead: '<S27>/Elbow Torque Command'
      //   DataStoreRead: '<S27>/Shoulder Torque Command'
      //   DataStoreRead: '<S27>/Wrist Torque Command'

      command_dynamixel_current(Template_v3_08_2020a_DW.RED_Tz_Shoulder,
        Template_v3_08_2020a_DW.RED_Tz_Elbow,
        Template_v3_08_2020a_DW.RED_Tz_Wrist);

      // End of Outputs for SubSystem: '<S3>/Change RED Behavior'
    }

    // End of If: '<S3>/This IF block determines whether or not to run the RED sim//exp ' 

    // If: '<S4>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
    //   Constant: '<S32>/Constant'
    //   DataStoreRead: '<S29>/BLACK_Fx'
    //   DataStoreRead: '<S29>/BLACK_Fy'
    //   DataStoreRead: '<S4>/Data Store Read'
    //   Product: '<S35>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S35>/Rotate F_I to F_b'

    if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) ||
        (Template_v3_08_2020a_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S4>/Change BLACK Behavior' incorporates:
      //   ActionPort: '<S29>/Action Port'

      // MATLAB Function: '<S41>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S34>/BLACK_Rz'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.BLACK_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix);

      // MATLAB Function: '<S34>/MATLAB Function1'
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function1': '<S37>:1' 
      // '<S37>:1:3' Vec1 = [ -1
      // '<S37>:1:4'          -1
      // '<S37>:1:5'           0
      // '<S37>:1:6'           0
      // '<S37>:1:7'           1
      // '<S37>:1:8'           1
      // '<S37>:1:9'           0
      // '<S37>:1:10'           0 ];
      // '<S37>:1:12' Vec2 = [  0
      // '<S37>:1:13'           0
      // '<S37>:1:14'           1
      // '<S37>:1:15'           1
      // '<S37>:1:16'           0
      // '<S37>:1:17'           0
      // '<S37>:1:18'          -1
      // '<S37>:1:19'          -1 ];
      // '<S37>:1:21' Vec3 = thruster_dist2CG_BLACK./1000;
      // '<S37>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S37>:1:25' Mat2 = diag((F_thrusters_BLACK));
      memset(&Template_v3_08_2020a_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // '<S37>:1:27' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S36>:1' 
      // '<S36>:1:3' Vec1 = [ -1
      // '<S36>:1:4'          -1
      // '<S36>:1:5'           0
      // '<S36>:1:6'           0
      // '<S36>:1:7'           1
      // '<S36>:1:8'           1
      // '<S36>:1:9'           0
      // '<S36>:1:10'           0 ];
      // '<S36>:1:12' Vec2 = [  0
      // '<S36>:1:13'           0
      // '<S36>:1:14'           1
      // '<S36>:1:15'           1
      // '<S36>:1:16'           0
      // '<S36>:1:17'           0
      // '<S36>:1:18'          -1
      // '<S36>:1:19'          -1 ];
      // '<S36>:1:21' Vec3 = thruster_dist2CG_BLACK./1000;
      // '<S36>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S36>:1:25' Mat2 = diag((F_thrusters_BLACK./2));
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_P.F_thrusters_BLACK[Template_v3_08_2020a_B.uElOffset1];

        // MATLAB Function: '<S34>/MATLAB Function'
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1]
          =
          Template_v3_08_2020a_P.F_thrusters_BLACK[Template_v3_08_2020a_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S34>/MATLAB Function'
      memset(&Template_v3_08_2020a_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S36>:1:27' H    = Mat1*Mat2;
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2_m[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1] =
          b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 1] =
          c[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 2] =
          Template_v3_08_2020a_P.thruster_dist2CG_BLACK[Template_v3_08_2020a_B.uElOffset1]
          / 1000.0;
      }

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2_m[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // PermuteDimensions: '<S42>/transpose'
      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.Product2_j[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S42>/transpose'

      // Product: '<S42>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.H_hz[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Product2_j[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S42>/Product1' incorporates:
      //   Product: '<S42>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.rtb_H_hz_c,
        Template_v3_08_2020a_B.b_k);

      // Product: '<S42>/Product2'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 +
            (Template_v3_08_2020a_B.i << 3);
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] =
            0.0;
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.Product2_j[Template_v3_08_2020a_B.uElOffset1];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 1] *
            Template_v3_08_2020a_B.Product2_j[Template_v3_08_2020a_B.uElOffset1
            + 8];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 2] *
            Template_v3_08_2020a_B.Product2_j[Template_v3_08_2020a_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S42>/Product2'

      // MATLAB Function: '<S35>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Rz'

      Temp_CreateRotationMatrix_h(Template_v3_08_2020a_DW.BLACK_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_h);
      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_h.C_bI[0] *
        Template_v3_08_2020a_DW.BLACK_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_h.C_bI[2] *
        Template_v3_08_2020a_DW.BLACK_Fy;

      // Product: '<S35>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Fx'
      //   DataStoreRead: '<S29>/BLACK_Fy'
      //   SignalConversion generated from: '<S35>/Rotate F_I to F_b'

      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_h.C_bI[1] *
        Template_v3_08_2020a_DW.BLACK_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_h.C_bI[3] *
        Template_v3_08_2020a_DW.BLACK_Fy;

      // SignalConversion generated from: '<S34>/Product' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Tz'

      Template_v3_08_2020a_B.x_ddot_b[0] = Template_v3_08_2020a_B.Subtract_o;
      Template_v3_08_2020a_B.x_ddot_b[1] = Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.x_ddot_b[2] = Template_v3_08_2020a_DW.BLACK_Tz;

      // Product: '<S34>/Product' incorporates:
      //   DataStoreRead: '<S29>/BLACK_Tz'
      //   SignalConversion generated from: '<S34>/Product'

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 16] *
          Template_v3_08_2020a_DW.BLACK_Tz +
          (Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 8] *
           Template_v3_08_2020a_B.rtb_x_ddot_i_g +
           Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i] *
           Template_v3_08_2020a_B.Subtract_o);
      }

      // End of Product: '<S34>/Product'

      // MATLAB Function: '<S34>/MATLAB Function10'
      Template_v_MATLABFunction10(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction10);

      // MATLAB Function: '<S34>/MATLAB Function11'
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLACK Behavior/Calculate Thruster  ON//OFF/MATLAB Function11': '<S39>:1' 
      // '<S39>:1:4' adjusted_thruster_dist2CG_BLACK = [thruster_dist2CG_BLACK(1); 
      // '<S39>:1:5'                                  thruster_dist2CG_BLACK(2); 
      // '<S39>:1:6'                                  thruster_dist2CG_BLACK(3); 
      // '<S39>:1:7'                                  thruster_dist2CG_BLACK(4); 
      // '<S39>:1:8'                                  thruster_dist2CG_BLACK(5); 
      // '<S39>:1:9'                                  thruster_dist2CG_BLACK(6); 
      // '<S39>:1:10'                                  thruster_dist2CG_BLACK(7); 
      // '<S39>:1:11'                                  thruster_dist2CG_BLACK(8)]; 
      // '<S39>:1:15' Vec1 = [ -1
      // '<S39>:1:16'          -1
      // '<S39>:1:17'           0
      // '<S39>:1:18'           0
      // '<S39>:1:19'           1
      // '<S39>:1:20'           1
      // '<S39>:1:21'           0
      // '<S39>:1:22'           0 ];
      // '<S39>:1:24' Vec2 = [  0
      // '<S39>:1:25'           0
      // '<S39>:1:26'           1
      // '<S39>:1:27'           1
      // '<S39>:1:28'           0
      // '<S39>:1:29'           0
      // '<S39>:1:30'          -1
      // '<S39>:1:31'          -1 ];
      // '<S39>:1:33' Vec3 = adjusted_thruster_dist2CG_BLACK./1000;
      // '<S39>:1:35' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S39>:1:37' F_thrusters_BLACK = [F_Black_X_nominal*thrust_decay_factor; F_Black_X_nominal*thrust_decay_factor; F_Black_Y_nominal*thrust_decay_factor; F_Black_Y_nominal*thrust_decay_factor; F_Black_X_nominal*thrust_decay_factor; F_Black_X_nominal*thrust_decay_factor; F_Black_Y_nominal*thrust_decay_factor; F_Black_Y_nominal*thrust_decay_factor]; 
      // '<S39>:1:39' Mat2 = diag((F_thrusters_BLACK./2));
      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_P.F_Black_X_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction10.thrust_decay_factor / 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[0] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[1] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.F_Black_Y_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction10.thrust_decay_factor / 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[2] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[3] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[4] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[5] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[6] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[7] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      memset(&Template_v3_08_2020a_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // '<S39>:1:41' H    = Mat1*Mat2;
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2_m[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1] =
          b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 1] =
          c[Template_v3_08_2020a_B.uElOffset1];
      }

      Template_v3_08_2020a_B.b[2] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[0] / 1000.0;
      Template_v3_08_2020a_B.b[5] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[1] / 1000.0;
      Template_v3_08_2020a_B.b[8] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[2] / 1000.0;
      Template_v3_08_2020a_B.b[11] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[3] / 1000.0;
      Template_v3_08_2020a_B.b[14] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[4] / 1000.0;
      Template_v3_08_2020a_B.b[17] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[5] / 1000.0;
      Template_v3_08_2020a_B.b[20] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[6] / 1000.0;
      Template_v3_08_2020a_B.b[23] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLACK[7] / 1000.0;
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2_m[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // End of MATLAB Function: '<S34>/MATLAB Function11'

      // PermuteDimensions: '<S43>/transpose'
      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S43>/transpose'

      // Product: '<S43>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.H_hz[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.transpose_f[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S43>/Product1' incorporates:
      //   Product: '<S43>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.rtb_H_hz_c,
        Template_v3_08_2020a_B.b_k);
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        // Product: '<S34>/Product5'
        Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          // Product: '<S43>/Product2' incorporates:
          //   Product: '<S34>/Product5'

          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.i +
            (Template_v3_08_2020a_B.uElOffset1 << 3);
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1] *
            Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i];
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 1]
            * Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 8];
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 2]
            * Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 16];
          Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] *
            Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.uElOffset1];
        }

        // Saturate: '<S34>/Remove Negatives' incorporates:
        //   Product: '<S34>/Product5'
        //   Product: '<S43>/Product2'

        if (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] >
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat;
        } else if
            (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] <
             Template_v3_08_2020a_P.RemoveNegatives_LowerSat) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_LowerSat;
        } else {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i];
        }

        // End of Saturate: '<S34>/Remove Negatives'
      }

      // MATLAB Function: '<S34>/MATLAB Function2'
      Template_v3_MATLABFunction2(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction2);

      // MATLAB Function: '<S34>/MATLAB Function1'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.i] =
          b[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.i + 1] =
          c[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.i + 2] =
          Template_v3_08_2020a_P.thruster_dist2CG_BLACK[Template_v3_08_2020a_B.i]
          / 1000.0;
      }

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] * Template_v3_08_2020a_B.Mat2
              [(Template_v3_08_2020a_B.i << 3) + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S34>/Product1'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.H_hz[3 * Template_v3_08_2020a_B.uElOffset1 +
            Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[Template_v3_08_2020a_B.uElOffset1];
        }
      }

      // End of Product: '<S34>/Product1'

      // DataStoreWrite: '<S34>/BLACK_Fx_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLACK_Fx_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix.C_Ib[0] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix.C_Ib[2] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S34>/BLACK_Fy_Sat' incorporates:
      //   Product: '<S41>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLACK_Fy_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix.C_Ib[1] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix.C_Ib[3] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S34>/BLACK_Tz_Sat'
      Template_v3_08_2020a_DW.BLACK_Tz_Sat = Template_v3_08_2020a_B.x_ddot_b[2];

      // End of Outputs for SubSystem: '<S4>/Change BLACK Behavior'
    }

    // End of If: '<S4>/This IF block determines whether or not to run the BLACK sim//exp' 

    // If: '<S4>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
    //   Constant: '<S32>/Constant'
    //   DataStoreRead: '<S30>/BLUE_Fx'
    //   DataStoreRead: '<S30>/BLUE_Fy'
    //   DataStoreRead: '<S4>/Data Store Read'
    //   Product: '<S47>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S47>/Rotate F_I to F_b'

    if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) ||
        (Template_v3_08_2020a_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S4>/Change BLUE Behavior' incorporates:
      //   ActionPort: '<S30>/Action Port'

      // MATLAB Function: '<S53>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S46>/BLUE_Rz'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.BLUE_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_i);

      // MATLAB Function: '<S46>/MATLAB Function1'
      Template_v3__MATLABFunction(&Template_v3_08_2020a_B.sf_MATLABFunction1_b);

      // MATLAB Function: '<S46>/MATLAB Function'
      Template_v3__MATLABFunction(&Template_v3_08_2020a_B.sf_MATLABFunction_i);

      // PermuteDimensions: '<S54>/transpose'
      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.sf_MATLABFunction_i.H[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S54>/transpose'

      // Product: '<S54>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.vcol = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.b_k[Template_v3_08_2020a_B.vcol] = 0.0;
          for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
               8; Template_v3_08_2020a_B.ntIdx0++) {
            Template_v3_08_2020a_B.b_k[Template_v3_08_2020a_B.vcol] +=
              Template_v3_08_2020a_B.sf_MATLABFunction_i.H[3 *
              Template_v3_08_2020a_B.ntIdx0 + Template_v3_08_2020a_B.uElOffset1]
              * Template_v3_08_2020a_B.Product2_a[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.ntIdx0];
          }
        }
      }

      // Product: '<S54>/Product1' incorporates:
      //   Product: '<S54>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.b_k,
        Template_v3_08_2020a_B.rtb_H_hz_c);

      // Product: '<S54>/Product2'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 +
            (Template_v3_08_2020a_B.i << 3);
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] =
            0.0;
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.rtb_H_hz_c[3 * Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.uElOffset1];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.rtb_H_hz_c[3 * Template_v3_08_2020a_B.i + 1] *
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.uElOffset1
            + 8];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.rtb_H_hz_c[3 * Template_v3_08_2020a_B.i + 2] *
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S54>/Product2'

      // MATLAB Function: '<S47>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Rz'

      Temp_CreateRotationMatrix_h(Template_v3_08_2020a_DW.BLUE_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_f);
      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_f.C_bI[0] *
        Template_v3_08_2020a_DW.BLUE_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_f.C_bI[2] *
        Template_v3_08_2020a_DW.BLUE_Fy;

      // Product: '<S47>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Fx'
      //   DataStoreRead: '<S30>/BLUE_Fy'
      //   SignalConversion generated from: '<S47>/Rotate F_I to F_b'

      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_f.C_bI[1] *
        Template_v3_08_2020a_DW.BLUE_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_f.C_bI[3] *
        Template_v3_08_2020a_DW.BLUE_Fy;

      // SignalConversion generated from: '<S46>/Product' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Tz'

      Template_v3_08_2020a_B.x_ddot_b[0] = Template_v3_08_2020a_B.Subtract_o;
      Template_v3_08_2020a_B.x_ddot_b[1] = Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.x_ddot_b[2] = Template_v3_08_2020a_DW.BLUE_Tz;

      // Product: '<S46>/Product' incorporates:
      //   DataStoreRead: '<S30>/BLUE_Tz'
      //   SignalConversion generated from: '<S46>/Product'

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 16] *
          Template_v3_08_2020a_DW.BLUE_Tz +
          (Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 8] *
           Template_v3_08_2020a_B.rtb_x_ddot_i_g +
           Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i] *
           Template_v3_08_2020a_B.Subtract_o);
      }

      // End of Product: '<S46>/Product'

      // MATLAB Function: '<S46>/MATLAB Function6'
      Template_v_MATLABFunction10(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction6);

      // MATLAB Function: '<S46>/MATLAB Function7'
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change BLUE Behavior/Calculate Thruster  ON//OFF/MATLAB Function7': '<S52>:1' 
      // '<S52>:1:4' adjusted_thruster_dist2CG_BLUE = [thruster_dist2CG_BLUE(1); 
      // '<S52>:1:5'                                  thruster_dist2CG_BLUE(2);
      // '<S52>:1:6'                                  thruster_dist2CG_BLUE(3);
      // '<S52>:1:7'                                  thruster_dist2CG_BLUE(4);
      // '<S52>:1:8'                                  thruster_dist2CG_BLUE(5);
      // '<S52>:1:9'                                  thruster_dist2CG_BLUE(6);
      // '<S52>:1:10'                                  thruster_dist2CG_BLUE(7); 
      // '<S52>:1:11'                                  thruster_dist2CG_BLUE(8)]; 
      // '<S52>:1:15' Vec1 = [ -1
      // '<S52>:1:16'          -1
      // '<S52>:1:17'           0
      // '<S52>:1:18'           0
      // '<S52>:1:19'           1
      // '<S52>:1:20'           1
      // '<S52>:1:21'           0
      // '<S52>:1:22'           0 ];
      // '<S52>:1:24' Vec2 = [  0
      // '<S52>:1:25'           0
      // '<S52>:1:26'           1
      // '<S52>:1:27'           1
      // '<S52>:1:28'           0
      // '<S52>:1:29'           0
      // '<S52>:1:30'          -1
      // '<S52>:1:31'          -1 ];
      // '<S52>:1:33' Vec3 = adjusted_thruster_dist2CG_BLUE./1000;
      // '<S52>:1:35' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S52>:1:37' F_thrusters_BLUE = [F_Blue_X_nominal*thrust_decay_factor; F_Blue_X_nominal*thrust_decay_factor; F_Blue_Y_nominal*thrust_decay_factor; F_Blue_Y_nominal*thrust_decay_factor; F_Blue_X_nominal*thrust_decay_factor; F_Blue_X_nominal*thrust_decay_factor; F_Blue_Y_nominal*thrust_decay_factor; F_Blue_Y_nominal*thrust_decay_factor]; 
      // '<S52>:1:39' Mat2 = diag((F_thrusters_BLUE./2));
      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_P.F_Blue_X_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction6.thrust_decay_factor / 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[0] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[1] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.F_Blue_Y_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction6.thrust_decay_factor / 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[2] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[3] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[4] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[5] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[6] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[7] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      memset(&Template_v3_08_2020a_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // '<S52>:1:41' H    = Mat1*Mat2;
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1] =
          b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 1] =
          c[Template_v3_08_2020a_B.uElOffset1];
      }

      Template_v3_08_2020a_B.b[2] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[0] / 1000.0;
      Template_v3_08_2020a_B.b[5] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[1] / 1000.0;
      Template_v3_08_2020a_B.b[8] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[2] / 1000.0;
      Template_v3_08_2020a_B.b[11] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[3] / 1000.0;
      Template_v3_08_2020a_B.b[14] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[4] / 1000.0;
      Template_v3_08_2020a_B.b[17] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[5] / 1000.0;
      Template_v3_08_2020a_B.b[20] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[6] / 1000.0;
      Template_v3_08_2020a_B.b[23] =
        Template_v3_08_2020a_P.thruster_dist2CG_BLUE[7] / 1000.0;
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] * Template_v3_08_2020a_B.Mat2
              [(Template_v3_08_2020a_B.i << 3) + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // End of MATLAB Function: '<S46>/MATLAB Function7'

      // PermuteDimensions: '<S55>/transpose'
      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S55>/transpose'

      // Product: '<S55>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.vcol
              + Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.transpose_f[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S55>/Product1' incorporates:
      //   Product: '<S55>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.rtb_H_hz_c,
        Template_v3_08_2020a_B.b_k);
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        // Product: '<S46>/Product4'
        Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          // Product: '<S55>/Product2' incorporates:
          //   Product: '<S46>/Product4'

          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.i +
            (Template_v3_08_2020a_B.uElOffset1 << 3);
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1] *
            Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i];
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 1]
            * Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 8];
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 2]
            * Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.i + 16];
          Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] *
            Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.uElOffset1];
        }

        // Saturate: '<S46>/Remove Negatives' incorporates:
        //   Product: '<S46>/Product4'
        //   Product: '<S55>/Product2'

        if (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] >
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat_p) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat_p;
        } else if
            (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] <
             Template_v3_08_2020a_P.RemoveNegatives_LowerSat_c) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_LowerSat_c;
        } else {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i];
        }

        // End of Saturate: '<S46>/Remove Negatives'
      }

      // MATLAB Function: '<S46>/MATLAB Function2'
      Template_v3_MATLABFunction2(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction2_n);

      // Product: '<S46>/Product1'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.sf_MATLABFunction1_b.H[3 *
            Template_v3_08_2020a_B.uElOffset1 + Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[Template_v3_08_2020a_B.uElOffset1];
        }
      }

      // End of Product: '<S46>/Product1'

      // DataStoreWrite: '<S46>/BLUE_Fx_Sat' incorporates:
      //   Product: '<S53>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLUE_Fx_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_i.C_Ib[0] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_i.C_Ib[2] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S46>/BLUE_Fy_Sat' incorporates:
      //   Product: '<S53>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLUE_Fy_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_i.C_Ib[1] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_i.C_Ib[3] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S46>/BLUE_Tz_Sat'
      Template_v3_08_2020a_DW.BLUE_Tz_Sat = Template_v3_08_2020a_B.x_ddot_b[2];

      // End of Outputs for SubSystem: '<S4>/Change BLUE Behavior'
    }

    // End of If: '<S4>/This IF block determines whether or not to run the BLUE sim//exp' 

    // If: '<S4>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
    //   Constant: '<S32>/Constant'
    //   DataStoreRead: '<S31>/RED_Fx'
    //   DataStoreRead: '<S31>/RED_Fy'
    //   DataStoreRead: '<S4>/Data Store Read'
    //   If: '<S58>/If'
    //   If: '<S58>/If1'
    //   Inport: '<S60>/In1'
    //   Inport: '<S61>/In1'
    //   MATLAB Function: '<S58>/MATLAB Function'
    //   MATLAB Function: '<S58>/MATLAB Function4'
    //   Product: '<S59>/Rotate F_I to F_b'
    //   SignalConversion generated from: '<S59>/Rotate F_I to F_b'

    if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) ||
        (Template_v3_08_2020a_P.simMode == 1.0)) {
      // Outputs for IfAction SubSystem: '<S4>/Change RED Behavior' incorporates:
      //   ActionPort: '<S31>/Action Port'

      // MATLAB Function: '<S58>/MATLAB Function3'
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function3': '<S67>:1' 
      // '<S67>:1:3' Vec1 = [ -1
      // '<S67>:1:4'          -1
      // '<S67>:1:5'           0
      // '<S67>:1:6'           0
      // '<S67>:1:7'           1
      // '<S67>:1:8'           1
      // '<S67>:1:9'           0
      // '<S67>:1:10'           0 ];
      // '<S67>:1:12' Vec2 = [  0
      // '<S67>:1:13'           0
      // '<S67>:1:14'           1
      // '<S67>:1:15'           1
      // '<S67>:1:16'           0
      // '<S67>:1:17'           0
      // '<S67>:1:18'          -1
      // '<S67>:1:19'          -1 ];
      // '<S67>:1:21' Vec3 = thruster_dist2CG_RED./1000;
      // '<S67>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S67>:1:25' Mat2 = diag((F_thrusters_RED./2));
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_P.F_thrusters_RED[Template_v3_08_2020a_B.i] / 2.0;
      }

      memset(&Template_v3_08_2020a_B.Mat2[0], 0, sizeof(real_T) << 6U);

      // MATLAB Function: '<S58>/MATLAB Function' incorporates:
      //   DataStoreRead: '<S58>/Data Store Read'
      //   DataStoreRead: '<S58>/Data Store Read7'
      //   DataStoreRead: '<S58>/Data Store Read8'
      //   MATLAB Function: '<S58>/MATLAB Function4'
      //   MATLAB Function: '<S58>/MATLAB Function6'

      // '<S67>:1:27' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function': '<S64>:1' 
      // '<S64>:1:3' m0  = model_param(1);
      // '<S64>:1:4' phi = model_param(7);
      // '<S64>:1:5' b0  = model_param(8);
      // '<S64>:1:6' a1  = model_param(9);
      // '<S64>:1:7' b1  = model_param(10);
      // '<S64>:1:8' a2  = model_param(11);
      // '<S64>:1:9' b2  = model_param(12);
      // '<S64>:1:10' a3  = model_param(13);
      // '<S64>:1:11' m1  = model_param(15);
      // '<S64>:1:12' m2  = model_param(16);
      // '<S64>:1:13' m3  = model_param(17);
      // '<S64>:1:15' link1_com_x = b0*cos(phi) + a1*cos(pi/2 + theta_1);
      Template_v3_08_2020a_B.Sum6_d = cos
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Template_v3_08_2020a_B.t7 = Template_v3_08_2020a_P.model_param[7] * cos
        (Template_v3_08_2020a_P.model_param[6]);
      Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.t7 +
        Template_v3_08_2020a_P.model_param[8] * Template_v3_08_2020a_B.Sum6_d;

      // '<S64>:1:16' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      Template_v3_08_2020a_B.Sum6_au = sin
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Template_v3_08_2020a_B.VelocitytoPosition_g =
        Template_v3_08_2020a_P.model_param[7] * sin
        (Template_v3_08_2020a_P.model_param[6]);
      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.VelocitytoPosition_g +
        Template_v3_08_2020a_P.model_param[8] * Template_v3_08_2020a_B.Sum6_au;

      // '<S64>:1:18' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.t53 = (Template_v3_08_2020a_DW.RED_Rz_Shoulder +
        1.5707963267948966) + Template_v3_08_2020a_DW.RED_Rz_Elbow;
      Template_v3_08_2020a_B.uDLookupTable2 = cos(Template_v3_08_2020a_B.t53);
      Template_v3_08_2020a_B.Sum6_d = (Template_v3_08_2020a_P.model_param[9] *
        Template_v3_08_2020a_B.Sum6_d + Template_v3_08_2020a_B.Sum6_l) +
        Template_v3_08_2020a_P.model_param[10] *
        Template_v3_08_2020a_B.uDLookupTable2;

      // '<S64>:1:19' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.Sum6_i = sin(Template_v3_08_2020a_B.t53);
      Template_v3_08_2020a_B.Sum6_au = (Template_v3_08_2020a_P.model_param[9] *
        Template_v3_08_2020a_B.Sum6_au + Template_v3_08_2020a_B.Subtract_o) +
        Template_v3_08_2020a_P.model_param[10] * Template_v3_08_2020a_B.Sum6_i;

      // '<S64>:1:21' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S64>:1:22' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S64>:1:24' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.t53 += Template_v3_08_2020a_DW.RED_Rz_Wrist;
      Template_v3_08_2020a_B.u0 = ((Template_v3_08_2020a_P.model_param[0] +
        Template_v3_08_2020a_P.model_param[14]) +
        Template_v3_08_2020a_P.model_param[15]) +
        Template_v3_08_2020a_P.model_param[16];
      Template_v3_08_2020a_B.uDLookupTable2 = ((cos(Template_v3_08_2020a_B.t53) *
        Template_v3_08_2020a_P.model_param[12] +
        (Template_v3_08_2020a_P.model_param[11] *
         Template_v3_08_2020a_B.uDLookupTable2 + Template_v3_08_2020a_B.Sum6_d))
        * Template_v3_08_2020a_P.model_param[16] +
        (Template_v3_08_2020a_P.model_param[14] * Template_v3_08_2020a_B.Sum6_l
         + Template_v3_08_2020a_P.model_param[15] *
         Template_v3_08_2020a_B.Sum6_d)) / Template_v3_08_2020a_B.u0 * 1000.0;

      // '<S64>:1:25' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.Sum6_i = ((sin(Template_v3_08_2020a_B.t53) *
        Template_v3_08_2020a_P.model_param[12] +
        (Template_v3_08_2020a_P.model_param[11] * Template_v3_08_2020a_B.Sum6_i
         + Template_v3_08_2020a_B.Sum6_au)) *
        Template_v3_08_2020a_P.model_param[16] +
        (Template_v3_08_2020a_P.model_param[14] *
         Template_v3_08_2020a_B.Subtract_o + Template_v3_08_2020a_P.model_param
         [15] * Template_v3_08_2020a_B.Sum6_au)) / Template_v3_08_2020a_B.u0 *
        1000.0;

      // '<S64>:1:27' adjusted_thruster_dist2CG_RED = [thruster_dist2CG_RED(1)-delta_com_due_to_arm_y; 
      // '<S64>:1:28'                                  thruster_dist2CG_RED(2)-delta_com_due_to_arm_y; 
      // '<S64>:1:29'                                  thruster_dist2CG_RED(3)-delta_com_due_to_arm_x; 
      // '<S64>:1:30'                                  thruster_dist2CG_RED(4)-delta_com_due_to_arm_x; 
      // '<S64>:1:31'                                  thruster_dist2CG_RED(5)+delta_com_due_to_arm_y; 
      // '<S64>:1:32'                                  thruster_dist2CG_RED(6)+delta_com_due_to_arm_y; 
      // '<S64>:1:33'                                  thruster_dist2CG_RED(7)+delta_com_due_to_arm_x; 
      // '<S64>:1:34'                                  thruster_dist2CG_RED(8)+delta_com_due_to_arm_x]; 
      // '<S64>:1:38' Vec1 = [ -1
      // '<S64>:1:39'          -1
      // '<S64>:1:40'           0
      // '<S64>:1:41'           0
      // '<S64>:1:42'           1
      // '<S64>:1:43'           1
      // '<S64>:1:44'           0
      // '<S64>:1:45'           0 ];
      // '<S64>:1:47' Vec2 = [  0
      // '<S64>:1:48'           0
      // '<S64>:1:49'           1
      // '<S64>:1:50'           1
      // '<S64>:1:51'           0
      // '<S64>:1:52'           0
      // '<S64>:1:53'          -1
      // '<S64>:1:54'          -1 ];
      // '<S64>:1:56' Vec3 = adjusted_thruster_dist2CG_RED./1000;
      // '<S64>:1:58' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S64>:1:60' Mat2 = diag((F_thrusters_RED./2));
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        // MATLAB Function: '<S58>/MATLAB Function3'
        Template_v3_08_2020a_B.Mat2[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1]
          =
          Template_v3_08_2020a_P.F_thrusters_RED[Template_v3_08_2020a_B.uElOffset1]
          / 2.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function'
      memset(&Template_v3_08_2020a_B.Mat2_m[0], 0, sizeof(real_T) << 6U);

      // If: '<S58>/If' incorporates:
      //   Constant: '<S58>/Constant'

      // '<S64>:1:62' H    = Mat1*Mat2;
      Template_v3_08_2020a_B.rtb_Compare_j =
        ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
         (Template_v3_08_2020a_P.platformSelection == 5.0) ||
         (Template_v3_08_2020a_P.platformSelection == 10.0) ||
         (Template_v3_08_2020a_P.platformSelection == 11.0));

      // MATLAB Function: '<S58>/MATLAB Function'
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2_m[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1] =
          b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 1] =
          c[Template_v3_08_2020a_B.uElOffset1];
      }

      Template_v3_08_2020a_B.b[2] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[0] -
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[5] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[1] -
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[8] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[2] -
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[11] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[3] -
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[14] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[4] +
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[17] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[5] +
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[20] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[6] +
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[23] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[7] +
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2_m[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }

        // MATLAB Function: '<S58>/MATLAB Function3'
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i] =
          b[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i + 1] =
          c[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i + 2] =
          Template_v3_08_2020a_P.thruster_dist2CG_RED[Template_v3_08_2020a_B.i] /
          1000.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function3'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] =
            0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.vcol
              + Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }
      }

      if (Template_v3_08_2020a_B.rtb_Compare_j) {
        // Outputs for IfAction SubSystem: '<S58>/If Action Subsystem' incorporates:
        //   ActionPort: '<S60>/Action Port'

        // Outputs for IfAction SubSystem: '<S58>/If Action Subsystem1' incorporates:
        //   ActionPort: '<S61>/Action Port'

        memcpy(&Template_v3_08_2020a_B.transpose_f[0],
               &Template_v3_08_2020a_B.H_hz[0], 24U * sizeof(real_T));

        // End of Outputs for SubSystem: '<S58>/If Action Subsystem1'
        // End of Outputs for SubSystem: '<S58>/If Action Subsystem'
      }

      // PermuteDimensions: '<S72>/transpose' incorporates:
      //   If: '<S58>/If'
      //   Inport: '<S60>/In1'
      //   Inport: '<S61>/In1'
      //   MATLAB Function: '<S58>/MATLAB Function'

      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S72>/transpose'

      // Product: '<S72>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.transpose_f[3 * Template_v3_08_2020a_B.vcol
              + Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Product2_b[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S72>/Product1' incorporates:
      //   Product: '<S72>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.rtb_H_hz_c,
        Template_v3_08_2020a_B.b_k);

      // Product: '<S72>/Product2'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 +
            (Template_v3_08_2020a_B.i << 3);
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.uElOffset1];
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 1] *
            Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.uElOffset1
            + 8];
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 2] *
            Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.uElOffset1
            + 16];
        }
      }

      // End of Product: '<S72>/Product2'

      // MATLAB Function: '<S59>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S31>/RED_Rz'

      Temp_CreateRotationMatrix_h(Template_v3_08_2020a_DW.RED_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_n);
      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_n.C_bI[0] *
        Template_v3_08_2020a_DW.RED_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_n.C_bI[2] *
        Template_v3_08_2020a_DW.RED_Fy;

      // Product: '<S59>/Rotate F_I to F_b' incorporates:
      //   DataStoreRead: '<S31>/RED_Fx'
      //   DataStoreRead: '<S31>/RED_Fy'
      //   SignalConversion generated from: '<S59>/Rotate F_I to F_b'

      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_n.C_bI[1] *
        Template_v3_08_2020a_DW.RED_Fx +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_n.C_bI[3] *
        Template_v3_08_2020a_DW.RED_Fy;

      // SignalConversion generated from: '<S58>/Product' incorporates:
      //   DataStoreRead: '<S31>/RED_Tz'

      Template_v3_08_2020a_B.x_ddot_b[0] = Template_v3_08_2020a_B.Subtract_o;
      Template_v3_08_2020a_B.x_ddot_b[1] = Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.x_ddot_b[2] = Template_v3_08_2020a_DW.RED_Tz;

      // Product: '<S58>/Product' incorporates:
      //   DataStoreRead: '<S31>/RED_Tz'
      //   SignalConversion generated from: '<S58>/Product'

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i + 16] *
          Template_v3_08_2020a_DW.RED_Tz +
          (Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i + 8] *
           Template_v3_08_2020a_B.rtb_x_ddot_i_g +
           Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i] *
           Template_v3_08_2020a_B.Subtract_o);
      }

      // End of Product: '<S58>/Product'

      // MATLAB Function: '<S58>/MATLAB Function5'
      Template_v_MATLABFunction10(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction5);

      // MATLAB Function: '<S71>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S58>/RED_Rz'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.RED_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_e);

      // MATLAB Function: '<S58>/MATLAB Function1'
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function1': '<S65>:1' 
      // '<S65>:1:3' Vec1 = [ -1
      // '<S65>:1:4'          -1
      // '<S65>:1:5'           0
      // '<S65>:1:6'           0
      // '<S65>:1:7'           1
      // '<S65>:1:8'           1
      // '<S65>:1:9'           0
      // '<S65>:1:10'           0 ];
      // '<S65>:1:12' Vec2 = [  0
      // '<S65>:1:13'           0
      // '<S65>:1:14'           1
      // '<S65>:1:15'           1
      // '<S65>:1:16'           0
      // '<S65>:1:17'           0
      // '<S65>:1:18'          -1
      // '<S65>:1:19'          -1 ];
      // '<S65>:1:21' Vec3 = thruster_dist2CG_RED./1000;
      // '<S65>:1:23' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S65>:1:25' Mat2 = diag((F_thrusters_RED));
      memset(&Template_v3_08_2020a_B.Mat2[0], 0, sizeof(real_T) << 6U);
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_P.F_thrusters_RED[Template_v3_08_2020a_B.uElOffset1];
      }

      // MATLAB Function: '<S58>/MATLAB Function4' incorporates:
      //   DataStoreRead: '<S58>/Data Store Read1'
      //   DataStoreRead: '<S58>/Data Store Read2'
      //   DataStoreRead: '<S58>/Data Store Read3'
      //   MATLAB Function: '<S58>/MATLAB Function6'

      // '<S65>:1:27' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function4': '<S68>:1' 
      // '<S68>:1:4' m0  = model_param(1);
      // '<S68>:1:5' phi = model_param(7);
      // '<S68>:1:6' b0  = model_param(8);
      // '<S68>:1:7' a1  = model_param(9);
      // '<S68>:1:8' b1  = model_param(10);
      // '<S68>:1:9' a2  = model_param(11);
      // '<S68>:1:10' b2  = model_param(12);
      // '<S68>:1:11' a3  = model_param(13);
      // '<S68>:1:12' m1  = model_param(15);
      // '<S68>:1:13' m2  = model_param(16);
      // '<S68>:1:14' m3  = model_param(17);
      // '<S68>:1:16' link1_com_x = b0*cos(phi) + a1*cos(pi/2 + theta_1);
      Template_v3_08_2020a_B.Sum6_d = cos
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.t7 +
        Template_v3_08_2020a_P.model_param[8] * Template_v3_08_2020a_B.Sum6_d;

      // '<S68>:1:17' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      Template_v3_08_2020a_B.Sum6_au = sin
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966);
      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.VelocitytoPosition_g +
        Template_v3_08_2020a_P.model_param[8] * Template_v3_08_2020a_B.Sum6_au;

      // '<S68>:1:19' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.t53 = (Template_v3_08_2020a_DW.RED_Rz_Shoulder +
        1.5707963267948966) + Template_v3_08_2020a_DW.RED_Rz_Elbow;
      Template_v3_08_2020a_B.uDLookupTable2 = cos(Template_v3_08_2020a_B.t53);
      Template_v3_08_2020a_B.Sum6_d = (Template_v3_08_2020a_P.model_param[9] *
        Template_v3_08_2020a_B.Sum6_d + Template_v3_08_2020a_B.Sum6_l) +
        Template_v3_08_2020a_P.model_param[10] *
        Template_v3_08_2020a_B.uDLookupTable2;

      // '<S68>:1:20' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.Sum6_i = sin(Template_v3_08_2020a_B.t53);
      Template_v3_08_2020a_B.Sum6_au = (Template_v3_08_2020a_P.model_param[9] *
        Template_v3_08_2020a_B.Sum6_au + Template_v3_08_2020a_B.Subtract_o) +
        Template_v3_08_2020a_P.model_param[10] * Template_v3_08_2020a_B.Sum6_i;

      // '<S68>:1:22' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S68>:1:23' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S68>:1:25' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.t53 += Template_v3_08_2020a_DW.RED_Rz_Wrist;
      Template_v3_08_2020a_B.VelocitytoPosition_g = cos
        (Template_v3_08_2020a_B.t53) * Template_v3_08_2020a_P.model_param[12];
      Template_v3_08_2020a_B.uDLookupTable2 =
        ((Template_v3_08_2020a_B.VelocitytoPosition_g +
          (Template_v3_08_2020a_P.model_param[11] *
           Template_v3_08_2020a_B.uDLookupTable2 + Template_v3_08_2020a_B.Sum6_d))
         * Template_v3_08_2020a_P.model_param[16] +
         (Template_v3_08_2020a_P.model_param[14] * Template_v3_08_2020a_B.Sum6_l
          + Template_v3_08_2020a_P.model_param[15] *
          Template_v3_08_2020a_B.Sum6_d)) / Template_v3_08_2020a_B.u0 * 1000.0;

      // '<S68>:1:26' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.t2 = sin(Template_v3_08_2020a_B.t53) *
        Template_v3_08_2020a_P.model_param[12];
      Template_v3_08_2020a_B.Sum6_i = ((Template_v3_08_2020a_B.t2 +
        (Template_v3_08_2020a_P.model_param[11] * Template_v3_08_2020a_B.Sum6_i
         + Template_v3_08_2020a_B.Sum6_au)) *
        Template_v3_08_2020a_P.model_param[16] +
        (Template_v3_08_2020a_P.model_param[14] *
         Template_v3_08_2020a_B.Subtract_o + Template_v3_08_2020a_P.model_param
         [15] * Template_v3_08_2020a_B.Sum6_au)) / Template_v3_08_2020a_B.u0 *
        1000.0;

      // '<S68>:1:28' adjusted_thruster_dist2CG_RED = [thruster_dist2CG_RED(1)-delta_com_due_to_arm_y; 
      // '<S68>:1:29'                                  thruster_dist2CG_RED(2)-delta_com_due_to_arm_y; 
      // '<S68>:1:30'                                  thruster_dist2CG_RED(3)-delta_com_due_to_arm_x; 
      // '<S68>:1:31'                                  thruster_dist2CG_RED(4)-delta_com_due_to_arm_x; 
      // '<S68>:1:32'                                  thruster_dist2CG_RED(5)+delta_com_due_to_arm_y; 
      // '<S68>:1:33'                                  thruster_dist2CG_RED(6)+delta_com_due_to_arm_y; 
      // '<S68>:1:34'                                  thruster_dist2CG_RED(7)+delta_com_due_to_arm_x; 
      // '<S68>:1:35'                                  thruster_dist2CG_RED(8)+delta_com_due_to_arm_x]; 
      // '<S68>:1:39' Vec1 = [ -1
      // '<S68>:1:40'          -1
      // '<S68>:1:41'           0
      // '<S68>:1:42'           0
      // '<S68>:1:43'           1
      // '<S68>:1:44'           1
      // '<S68>:1:45'           0
      // '<S68>:1:46'           0 ];
      // '<S68>:1:48' Vec2 = [  0
      // '<S68>:1:49'           0
      // '<S68>:1:50'           1
      // '<S68>:1:51'           1
      // '<S68>:1:52'           0
      // '<S68>:1:53'           0
      // '<S68>:1:54'          -1
      // '<S68>:1:55'          -1 ];
      // '<S68>:1:57' Vec3 = adjusted_thruster_dist2CG_RED./1000;
      // '<S68>:1:59' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S68>:1:61' F_thrusters_RED = [F_red_X_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor]; 
      Template_v3_08_2020a_B.rtb_x_ddot_i_g =
        Template_v3_08_2020a_P.F_red_X_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction5.thrust_decay_factor;
      Template_v3_08_2020a_B.rtb_Product_h_b[0] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[1] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m =
        Template_v3_08_2020a_P.F_red_Y_nominal *
        Template_v3_08_2020a_B.sf_MATLABFunction5.thrust_decay_factor;
      Template_v3_08_2020a_B.rtb_Product_h_b[2] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[3] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[4] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[5] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[6] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[7] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;

      // '<S68>:1:63' Mat2 = diag((F_thrusters_RED));
      memset(&Template_v3_08_2020a_B.Mat2_m[0], 0, sizeof(real_T) << 6U);
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2_m[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
      }

      // MATLAB Function: '<S58>/MATLAB Function6' incorporates:
      //   DataStoreRead: '<S58>/Data Store Read1'
      //   DataStoreRead: '<S58>/Data Store Read2'

      // '<S68>:1:65' H    = Mat1*Mat2;
      // MATLAB Function 'From Force//Torque  to PWM Signal/Change RED Behavior/Calculate Thruster  ON//OFF/MATLAB Function6': '<S70>:1' 
      // '<S70>:1:3' m0  = model_param(1);
      // '<S70>:1:4' phi = model_param(7);
      // '<S70>:1:5' b0  = model_param(8);
      // '<S70>:1:6' a1  = model_param(9);
      // '<S70>:1:7' b1  = model_param(10);
      // '<S70>:1:8' a2  = model_param(11);
      // '<S70>:1:9' b2  = model_param(12);
      // '<S70>:1:10' a3  = model_param(13);
      // '<S70>:1:11' m1  = model_param(15);
      // '<S70>:1:12' m2  = model_param(16);
      // '<S70>:1:13' m3  = model_param(17);
      // '<S70>:1:15' link1_com_x = b0*cos(phi) + a1*cos(pi/2 + theta_1);
      Template_v3_08_2020a_B.Sum6_l = cos
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Template_v3_08_2020a_P.model_param[8] +
        Template_v3_08_2020a_P.model_param[7] * cos
        (Template_v3_08_2020a_P.model_param[6]);

      // '<S70>:1:16' link1_com_y = b0*sin(phi) + a1*sin(pi/2 + theta_1);
      Template_v3_08_2020a_B.Subtract_o = sin
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Template_v3_08_2020a_P.model_param[8] +
        Template_v3_08_2020a_P.model_param[7] * sin
        (Template_v3_08_2020a_P.model_param[6]);

      // '<S70>:1:18' link2_com_x = link1_com_x + b1*cos(pi/2 + theta_1) + a2*cos(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.Sum6_d = (cos
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Template_v3_08_2020a_P.model_param[9] + Template_v3_08_2020a_B.Sum6_l) +
        cos((Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) +
            Template_v3_08_2020a_DW.RED_Rz_Elbow) *
        Template_v3_08_2020a_P.model_param[10];

      // '<S70>:1:19' link2_com_y = link1_com_y + b1*sin(pi/2 + theta_1) + a2*sin(pi/2 + theta_1 + theta_2); 
      Template_v3_08_2020a_B.Sum6_au = (sin
        (Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) *
        Template_v3_08_2020a_P.model_param[9] +
        Template_v3_08_2020a_B.Subtract_o) + sin
        ((Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Template_v3_08_2020a_DW.RED_Rz_Elbow) *
        Template_v3_08_2020a_P.model_param[10];

      // '<S70>:1:21' link3_com_x = link2_com_x + b2*cos(pi/2 + theta_1 + theta_2) + a3*cos(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S70>:1:22' link3_com_y = link2_com_y + b2*sin(pi/2 + theta_1 + theta_2) + a3*sin(pi/2 + theta_1 + theta_2 + theta_3); 
      // '<S70>:1:24' delta_com_due_to_arm_x = (m1*link1_com_x + m2*link2_com_x + m3*link3_com_x)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.Sum6_l = (((cos
        ((Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Template_v3_08_2020a_DW.RED_Rz_Elbow) *
        Template_v3_08_2020a_P.model_param[11] + Template_v3_08_2020a_B.Sum6_d)
        + Template_v3_08_2020a_B.VelocitytoPosition_g) *
        Template_v3_08_2020a_P.model_param[16] +
        (Template_v3_08_2020a_P.model_param[14] * Template_v3_08_2020a_B.Sum6_l
         + Template_v3_08_2020a_P.model_param[15] *
         Template_v3_08_2020a_B.Sum6_d)) / Template_v3_08_2020a_B.u0 * 1000.0;

      // '<S70>:1:25' delta_com_due_to_arm_y = (m1*link1_com_y + m2*link2_com_y + m3*link3_com_y)/(m0 + m1 + m2 + m3)*1000; 
      Template_v3_08_2020a_B.Subtract_o = (((sin
        ((Template_v3_08_2020a_DW.RED_Rz_Shoulder + 1.5707963267948966) +
         Template_v3_08_2020a_DW.RED_Rz_Elbow) *
        Template_v3_08_2020a_P.model_param[11] + Template_v3_08_2020a_B.Sum6_au)
        + Template_v3_08_2020a_B.t2) * Template_v3_08_2020a_P.model_param[16] +
        (Template_v3_08_2020a_P.model_param[14] *
         Template_v3_08_2020a_B.Subtract_o + Template_v3_08_2020a_P.model_param
         [15] * Template_v3_08_2020a_B.Sum6_au)) / Template_v3_08_2020a_B.u0 *
        1000.0;

      // '<S70>:1:27' adjusted_thruster_dist2CG_RED = [thruster_dist2CG_RED(1)-delta_com_due_to_arm_y; 
      // '<S70>:1:28'                                  thruster_dist2CG_RED(2)-delta_com_due_to_arm_y; 
      // '<S70>:1:29'                                  thruster_dist2CG_RED(3)-delta_com_due_to_arm_x; 
      // '<S70>:1:30'                                  thruster_dist2CG_RED(4)-delta_com_due_to_arm_x; 
      // '<S70>:1:31'                                  thruster_dist2CG_RED(5)+delta_com_due_to_arm_y; 
      // '<S70>:1:32'                                  thruster_dist2CG_RED(6)+delta_com_due_to_arm_y; 
      // '<S70>:1:33'                                  thruster_dist2CG_RED(7)+delta_com_due_to_arm_x; 
      // '<S70>:1:34'                                  thruster_dist2CG_RED(8)+delta_com_due_to_arm_x]; 
      // '<S70>:1:38' Vec1 = [ -1
      // '<S70>:1:39'          -1
      // '<S70>:1:40'           0
      // '<S70>:1:41'           0
      // '<S70>:1:42'           1
      // '<S70>:1:43'           1
      // '<S70>:1:44'           0
      // '<S70>:1:45'           0 ];
      // '<S70>:1:47' Vec2 = [  0
      // '<S70>:1:48'           0
      // '<S70>:1:49'           1
      // '<S70>:1:50'           1
      // '<S70>:1:51'           0
      // '<S70>:1:52'           0
      // '<S70>:1:53'          -1
      // '<S70>:1:54'          -1 ];
      // '<S70>:1:56' Vec3 = adjusted_thruster_dist2CG_RED./1000;
      // '<S70>:1:58' Mat1 = [Vec1, Vec2, Vec3]';
      // '<S70>:1:60' F_thrusters_RED = [F_red_X_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_X_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor; F_red_Y_nominal*thrust_decay_factor]; 
      // '<S70>:1:62' Mat2 = diag((F_thrusters_RED./2));
      Template_v3_08_2020a_B.rtb_x_ddot_i_g /= 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[0] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[1] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m /= 2.0;
      Template_v3_08_2020a_B.rtb_Product_h_b[2] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[3] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[4] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[5] =
        Template_v3_08_2020a_B.rtb_x_ddot_i_g;
      Template_v3_08_2020a_B.rtb_Product_h_b[6] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      Template_v3_08_2020a_B.rtb_Product_h_b[7] =
        Template_v3_08_2020a_B.rtb_TmpSignalConversionAtSFun_m;
      memset(&Template_v3_08_2020a_B.Mat2_c[0], 0, sizeof(real_T) << 6U);

      // '<S70>:1:64' H    = Mat1*Mat2;
      for (Template_v3_08_2020a_B.uElOffset1 = 0;
           Template_v3_08_2020a_B.uElOffset1 < 8;
           Template_v3_08_2020a_B.uElOffset1++) {
        Template_v3_08_2020a_B.Mat2_c[Template_v3_08_2020a_B.uElOffset1 +
          (Template_v3_08_2020a_B.uElOffset1 << 3)] =
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1] =
          b[Template_v3_08_2020a_B.uElOffset1];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 + 1] =
          c[Template_v3_08_2020a_B.uElOffset1];
      }

      Template_v3_08_2020a_B.b[2] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[0] -
         Template_v3_08_2020a_B.Subtract_o) / 1000.0;
      Template_v3_08_2020a_B.b[5] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[1] -
         Template_v3_08_2020a_B.Subtract_o) / 1000.0;
      Template_v3_08_2020a_B.b[8] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[2] -
         Template_v3_08_2020a_B.Sum6_l) / 1000.0;
      Template_v3_08_2020a_B.b[11] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[3] -
         Template_v3_08_2020a_B.Sum6_l) / 1000.0;
      Template_v3_08_2020a_B.b[14] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[4] +
         Template_v3_08_2020a_B.Subtract_o) / 1000.0;
      Template_v3_08_2020a_B.b[17] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[5] +
         Template_v3_08_2020a_B.Subtract_o) / 1000.0;
      Template_v3_08_2020a_B.b[20] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[6] +
         Template_v3_08_2020a_B.Sum6_l) / 1000.0;
      Template_v3_08_2020a_B.b[23] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[7] +
         Template_v3_08_2020a_B.Sum6_l) / 1000.0;
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2_c[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // PermuteDimensions: '<S73>/transpose'
      Template_v3_08_2020a_B.i = 0;
      Template_v3_08_2020a_B.uElOffset1 = 0;
      for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 3;
           Template_v3_08_2020a_B.vcol++) {
        Template_v3_08_2020a_B.ar = Template_v3_08_2020a_B.uElOffset1;
        for (Template_v3_08_2020a_B.ntIdx0 = 0; Template_v3_08_2020a_B.ntIdx0 <
             8; Template_v3_08_2020a_B.ntIdx0++) {
          Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.Product2_b[Template_v3_08_2020a_B.ar];
          Template_v3_08_2020a_B.i++;
          Template_v3_08_2020a_B.ar += 3;
        }

        Template_v3_08_2020a_B.uElOffset1++;
      }

      // End of PermuteDimensions: '<S73>/transpose'

      // Product: '<S73>/Product'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.rtb_H_hz_c[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.Product2_b[3 * Template_v3_08_2020a_B.vcol
              + Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Product2_a[(Template_v3_08_2020a_B.i << 3)
              + Template_v3_08_2020a_B.vcol];
          }
        }
      }

      // Product: '<S73>/Product1' incorporates:
      //   Product: '<S73>/Product'

      Template_v3_08_2_rt_invd3x3_snf(Template_v3_08_2020a_B.rtb_H_hz_c,
        Template_v3_08_2020a_B.b_k);
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        // Product: '<S58>/Product2'
        Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          // Product: '<S73>/Product2' incorporates:
          //   Product: '<S58>/Product2'

          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.i +
            (Template_v3_08_2020a_B.uElOffset1 << 3);
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] =
            0.0;
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1] *
            Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 1]
            * Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i + 8];
          Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] +=
            Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.uElOffset1 + 2]
            * Template_v3_08_2020a_B.Product2_a[Template_v3_08_2020a_B.i + 16];
          Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.transpose_f[Template_v3_08_2020a_B.ntIdx0] *
            Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.uElOffset1];
        }

        // Saturate: '<S58>/Remove Negatives' incorporates:
        //   Product: '<S58>/Product2'
        //   Product: '<S73>/Product2'

        if (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] >
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat_o) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_UpperSat_o;
        } else if
            (Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i] <
             Template_v3_08_2020a_P.RemoveNegatives_LowerSat_d) {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_P.RemoveNegatives_LowerSat_d;
        } else {
          Template_v3_08_2020a_B.rtb_Product_h_b[Template_v3_08_2020a_B.i] =
            Template_v3_08_2020a_B.rtb_transpose_f_p[Template_v3_08_2020a_B.i];
        }

        // End of Saturate: '<S58>/Remove Negatives'
      }

      // MATLAB Function: '<S58>/MATLAB Function2'
      Template_v3_MATLABFunction2(Template_v3_08_2020a_B.rtb_Product_h_b,
        &Template_v3_08_2020a_B.sf_MATLABFunction2_l);

      // If: '<S58>/If1' incorporates:
      //   Constant: '<S58>/Constant1'

      Template_v3_08_2020a_B.rtb_Compare_j =
        ((Template_v3_08_2020a_P.platformSelection == 4.0) ||
         (Template_v3_08_2020a_P.platformSelection == 5.0) ||
         (Template_v3_08_2020a_P.platformSelection == 10.0) ||
         (Template_v3_08_2020a_P.platformSelection == 11.0));

      // MATLAB Function: '<S58>/MATLAB Function4'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.i] =
          b[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.i + 1] =
          c[Template_v3_08_2020a_B.i];
      }

      Template_v3_08_2020a_B.b[2] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[0] -
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[5] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[1] -
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[8] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[2] -
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[11] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[3] -
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[14] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[4] +
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[17] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[5] +
         Template_v3_08_2020a_B.Sum6_i) / 1000.0;
      Template_v3_08_2020a_B.b[20] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[6] +
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      Template_v3_08_2020a_B.b[23] =
        (Template_v3_08_2020a_P.thruster_dist2CG_RED[7] +
         Template_v3_08_2020a_B.uDLookupTable2) / 1000.0;
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.H_hz[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.vcol +
              Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2_m[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }

        // MATLAB Function: '<S58>/MATLAB Function1'
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i] =
          b[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i + 1] =
          c[Template_v3_08_2020a_B.i];
        Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.i + 2] =
          Template_v3_08_2020a_P.thruster_dist2CG_RED[Template_v3_08_2020a_B.i] /
          1000.0;
      }

      // MATLAB Function: '<S58>/MATLAB Function1'
      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 8;
           Template_v3_08_2020a_B.i++) {
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 3;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.ntIdx0 = Template_v3_08_2020a_B.uElOffset1 + 3 *
            Template_v3_08_2020a_B.i;
          Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] = 0.0;
          for (Template_v3_08_2020a_B.vcol = 0; Template_v3_08_2020a_B.vcol < 8;
               Template_v3_08_2020a_B.vcol++) {
            Template_v3_08_2020a_B.b[Template_v3_08_2020a_B.ntIdx0] +=
              Template_v3_08_2020a_B.Product2_a[3 * Template_v3_08_2020a_B.vcol
              + Template_v3_08_2020a_B.uElOffset1] *
              Template_v3_08_2020a_B.Mat2[(Template_v3_08_2020a_B.i << 3) +
              Template_v3_08_2020a_B.vcol];
          }
        }
      }

      if (Template_v3_08_2020a_B.rtb_Compare_j) {
        memcpy(&Template_v3_08_2020a_B.b[0], &Template_v3_08_2020a_B.H_hz[0],
               24U * sizeof(real_T));
      }

      // Product: '<S58>/Product1' incorporates:
      //   If: '<S58>/If1'
      //   MATLAB Function: '<S58>/MATLAB Function4'

      for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
           Template_v3_08_2020a_B.i++) {
        Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] = 0.0;
        for (Template_v3_08_2020a_B.uElOffset1 = 0;
             Template_v3_08_2020a_B.uElOffset1 < 8;
             Template_v3_08_2020a_B.uElOffset1++) {
          Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] +=
            Template_v3_08_2020a_B.b[3 * Template_v3_08_2020a_B.uElOffset1 +
            Template_v3_08_2020a_B.i] *
            Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[Template_v3_08_2020a_B.uElOffset1];
        }
      }

      // End of Product: '<S58>/Product1'

      // DataStoreWrite: '<S58>/RED_Fx_Sat' incorporates:
      //   Product: '<S71>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.RED_Fx_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_e.C_Ib[0] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_e.C_Ib[2] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S58>/RED_Fy_Sat' incorporates:
      //   Product: '<S71>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.RED_Fy_Sat =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_e.C_Ib[1] *
        Template_v3_08_2020a_B.x_ddot_b[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_e.C_Ib[3] *
        Template_v3_08_2020a_B.x_ddot_b[1];

      // DataStoreWrite: '<S58>/RED_Tz_Sat'
      Template_v3_08_2020a_DW.RED_Tz_Sat = Template_v3_08_2020a_B.x_ddot_b[2];

      // End of Outputs for SubSystem: '<S4>/Change RED Behavior'
    }

    // End of If: '<S4>/This IF block determines whether or not to run the RED sim//exp ' 

    // RateTransition: '<S4>/Rate Transition' incorporates:
    //   RateTransition: '<S4>/Rate Transition1'
    //   RateTransition: '<S4>/Rate Transition3'
    //   RateTransition: '<S4>/Rate Transition4'
    //   RateTransition: '<S4>/Rate Transition5'
    //   RateTransition: '<S4>/Rate Transition6'
    //   RateTransition: '<S4>/Rate Transition7'
    //   RateTransition: '<S4>/Rate Transition8'
    //   Sum: '<S4>/Sum'
    //   Sum: '<S4>/Sum1'
    //   Sum: '<S4>/Sum2'
    //   Sum: '<S4>/Sum3'
    //   Sum: '<S4>/Sum4'
    //   Sum: '<S4>/Sum5'
    //   Sum: '<S4>/Sum6'
    //   Sum: '<S4>/Sum7'

    if (Template_v3_08_2020a_M->Timing.RateInteraction.TID1_2) {
      Template_v3_08_2020a_DW.RateTransition_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[0] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[7]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[7];
      Template_v3_08_2020a_DW.RateTransition1_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[1] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[2]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[2];

      // RateTransition: '<S4>/Rate Transition2' incorporates:
      //   Sum: '<S4>/Sum'
      //   Sum: '<S4>/Sum1'

      Template_v3_08_2020a_DW.RateTransition2_Buffer =
        Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[2];
      Template_v3_08_2020a_DW.RateTransition3_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[3] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[4]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[4];
      Template_v3_08_2020a_DW.RateTransition4_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[4] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[3]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[3];
      Template_v3_08_2020a_DW.RateTransition5_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[5] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[6]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[6];
      Template_v3_08_2020a_DW.RateTransition6_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[6] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[5]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[5];
      Template_v3_08_2020a_DW.RateTransition7_Buffer =
        (Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[7] +
         Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[0]) +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[0];
      Template_v3_08_2020a_DW.RateTransition8_Buffer =
        Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[1] +
        Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[1];
    }

    // End of RateTransition: '<S4>/Rate Transition'

    // If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' incorporates:
    //   Constant: '<S77>/Constant'

    Template_v3_08_2020a_B.rtAction = -1;
    if (Template_v3_08_2020a_P.simMode == 0.0) {
      Template_v3_08_2020a_B.rtAction = 0;

      // Outputs for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S78>/Action Port'

      // S-Function (sdspFromNetwork): '<S78>/UDP Receive'
      sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
      Template_v3_08_2020a_B.i = 16;
      LibOutputs_Network(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U],
                         &Template_v3_08_2020a_B.UDPReceive_o1[0U],
                         &Template_v3_08_2020a_B.i);
      if (*sErr != 0) {
        rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
        rtmSetStopRequested(Template_v3_08_2020a_M, 1);
      }

      // End of S-Function (sdspFromNetwork): '<S78>/UDP Receive'

      // DataStoreWrite: '<S78>/Data Store Write3'
      Template_v3_08_2020a_DW.ARM_Elbow_Px =
        Template_v3_08_2020a_B.UDPReceive_o1[7];

      // DataStoreWrite: '<S78>/Data Store Write4'
      Template_v3_08_2020a_DW.ARM_Elbow_Py =
        Template_v3_08_2020a_B.UDPReceive_o1[8];

      // DataStoreWrite: '<S78>/Data Store Write5'
      Template_v3_08_2020a_DW.ARM_Wrist_Px =
        Template_v3_08_2020a_B.UDPReceive_o1[9];

      // DataStoreWrite: '<S78>/Data Store Write6'
      Template_v3_08_2020a_DW.ARM_Wrist_Py =
        Template_v3_08_2020a_B.UDPReceive_o1[10];

      // DataStoreWrite: '<S78>/Data Store Write7'
      Template_v3_08_2020a_DW.ARM_EndEff_Px =
        Template_v3_08_2020a_B.UDPReceive_o1[11];

      // DataStoreWrite: '<S78>/Data Store Write8'
      Template_v3_08_2020a_DW.ARM_EndEff_Py =
        Template_v3_08_2020a_B.UDPReceive_o1[12];

      // DataStoreWrite: '<S78>/RED_Px1'
      Template_v3_08_2020a_DW.RED_Px = Template_v3_08_2020a_B.UDPReceive_o1[1];

      // DataStoreWrite: '<S78>/RED_Px10'
      Template_v3_08_2020a_DW.Univ_Time = Template_v3_08_2020a_B.UDPReceive_o1[0];

      // Delay: '<S88>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_ct != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_hi =
          Template_v3_08_2020a_B.UDPReceive_o1[4];
      }

      // Sum: '<S88>/Sum6' incorporates:
      //   Delay: '<S88>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1[4]
        - Template_v3_08_2020a_DW.Delay1_DSTATE_hi;

      // If: '<S88>/if we went through a "step"' incorporates:
      //   Inport: '<S103>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S88>/Hold this value' incorporates:
        //   ActionPort: '<S103>/Action Port'

        Template_v3_08_2020a_B.In1_gw = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S88>/Hold this value'
      }

      // End of If: '<S88>/if we went through a "step"'

      // Gain: '<S88>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_gw;

      // Delay: '<S80>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_h0 != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_df =
          Template_v3_08_2020a_B.dividebydeltaT;
      }

      // Sum: '<S80>/Sum6' incorporates:
      //   Delay: '<S80>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.dividebydeltaT
        - Template_v3_08_2020a_DW.Delay1_DSTATE_df;

      // If: '<S80>/if we went through a "step"' incorporates:
      //   Inport: '<S95>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S80>/Hold this value' incorporates:
        //   ActionPort: '<S95>/Action Port'

        Template_v3_08_2020a_B.In1_cw = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S80>/Hold this value'
      }

      // End of If: '<S80>/if we went through a "step"'

      // Gain: '<S80>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px11'

      Template_v3_08_2020a_DW.BLACK_Ax = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_cw;

      // Delay: '<S89>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_kt != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_ft =
          Template_v3_08_2020a_B.UDPReceive_o1[5];
      }

      // Sum: '<S89>/Sum6' incorporates:
      //   Delay: '<S89>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1[5]
        - Template_v3_08_2020a_DW.Delay1_DSTATE_ft;

      // If: '<S89>/if we went through a "step"' incorporates:
      //   Inport: '<S104>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S89>/Hold this value' incorporates:
        //   ActionPort: '<S104>/Action Port'

        Template_v3_08_2020a_B.In1_k5 = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S89>/Hold this value'
      }

      // End of If: '<S89>/if we went through a "step"'

      // Gain: '<S89>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT_k = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_k5;

      // Delay: '<S81>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_ktm != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_hz =
          Template_v3_08_2020a_B.dividebydeltaT_k;
      }

      // Sum: '<S81>/Sum6' incorporates:
      //   Delay: '<S81>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.dividebydeltaT_k -
        Template_v3_08_2020a_DW.Delay1_DSTATE_hz;

      // If: '<S81>/if we went through a "step"' incorporates:
      //   Inport: '<S96>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S81>/Hold this value' incorporates:
        //   ActionPort: '<S96>/Action Port'

        Template_v3_08_2020a_B.In1_gp = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S81>/Hold this value'
      }

      // End of If: '<S81>/if we went through a "step"'

      // Gain: '<S81>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px12'

      Template_v3_08_2020a_DW.BLACK_Ay = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_gp;

      // S-Function (sdspunwrap2): '<S78>/Unwrap1'
      if (Template_v3_08_2020a_DW.Unwrap1_FirstStep) {
        Template_v3_08_2020a_B.i = 0;
        while (Template_v3_08_2020a_B.i < 1) {
          Template_v3_08_2020a_DW.Unwrap1_Prev =
            Template_v3_08_2020a_B.UDPReceive_o1[6];
          Template_v3_08_2020a_B.i++;
        }

        Template_v3_08_2020a_DW.Unwrap1_FirstStep = false;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_DW.Unwrap1_Cumsum;
        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.UDPReceive_o1[6]
          - Template_v3_08_2020a_DW.Unwrap1_Prev;
        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l - floor
          ((Template_v3_08_2020a_B.Sum6_l + 3.1415926535897931) /
           6.2831853071795862) * 6.2831853071795862;
        if ((Template_v3_08_2020a_B.Sum6_d == -3.1415926535897931) &&
            (Template_v3_08_2020a_B.Sum6_l > 0.0)) {
          Template_v3_08_2020a_B.Sum6_d = 3.1415926535897931;
        }

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Sum6_d -
          Template_v3_08_2020a_B.Sum6_l;
        if (fabs(Template_v3_08_2020a_B.Sum6_l) > 3.1415926535897931) {
          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_DW.Unwrap1_Cumsum +
            Template_v3_08_2020a_B.Sum6_l;
        }

        Template_v3_08_2020a_DW.Unwrap1_Prev =
          Template_v3_08_2020a_B.UDPReceive_o1[6];
        Template_v3_08_2020a_B.Unwrap1 = Template_v3_08_2020a_B.UDPReceive_o1[6]
          + Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_DW.Unwrap1_Cumsum =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S78>/Unwrap1'

      // Delay: '<S90>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_gn != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_mt =
          Template_v3_08_2020a_B.Unwrap1;
      }

      // Sum: '<S90>/Sum6' incorporates:
      //   Delay: '<S90>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Unwrap1 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_mt;

      // If: '<S90>/if we went through a "step"' incorporates:
      //   Inport: '<S105>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S90>/Hold this value' incorporates:
        //   ActionPort: '<S105>/Action Port'

        Template_v3_08_2020a_B.In1_ex = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S90>/Hold this value'
      }

      // End of If: '<S90>/if we went through a "step"'

      // Gain: '<S90>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT_d = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_ex;

      // Delay: '<S82>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_pz != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_lf =
          Template_v3_08_2020a_B.dividebydeltaT_d;
      }

      // Sum: '<S82>/Sum6' incorporates:
      //   Delay: '<S82>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.dividebydeltaT_d -
        Template_v3_08_2020a_DW.Delay1_DSTATE_lf;

      // If: '<S82>/if we went through a "step"' incorporates:
      //   Inport: '<S97>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S82>/Hold this value' incorporates:
        //   ActionPort: '<S97>/Action Port'

        Template_v3_08_2020a_B.In1_ng = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S82>/Hold this value'
      }

      // End of If: '<S82>/if we went through a "step"'

      // Gain: '<S82>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px13'

      Template_v3_08_2020a_DW.BLACK_RzDD = 1.0 / Template_v3_08_2020a_P.baseRate
        * Template_v3_08_2020a_B.In1_ng;

      // Delay: '<S83>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_fy != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_cs =
          Template_v3_08_2020a_B.UDPReceive_o1[13];
      }

      // Sum: '<S83>/Sum6' incorporates:
      //   Delay: '<S83>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1
        [13] - Template_v3_08_2020a_DW.Delay1_DSTATE_cs;

      // If: '<S83>/if we went through a "step"' incorporates:
      //   Inport: '<S98>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S83>/Hold this value' incorporates:
        //   ActionPort: '<S98>/Action Port'

        Template_v3_08_2020a_B.In1_i0 = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S83>/Hold this value'
      }

      // End of If: '<S83>/if we went through a "step"'

      // Gain: '<S83>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px17'

      Template_v3_08_2020a_DW.BLUE_Vx = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_i0;

      // DataStoreWrite: '<S78>/RED_Px18'
      Template_v3_08_2020a_DW.BLUE_Px = Template_v3_08_2020a_B.UDPReceive_o1[13];

      // DataStoreWrite: '<S78>/RED_Px19'
      Template_v3_08_2020a_DW.BLUE_Py = Template_v3_08_2020a_B.UDPReceive_o1[14];

      // Delay: '<S79>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_ea != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_kv =
          Template_v3_08_2020a_B.UDPReceive_o1[1];
      }

      // Sum: '<S79>/Sum6' incorporates:
      //   Delay: '<S79>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1[1]
        - Template_v3_08_2020a_DW.Delay1_DSTATE_kv;

      // If: '<S79>/if we went through a "step"' incorporates:
      //   Inport: '<S94>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S79>/Hold this value' incorporates:
        //   ActionPort: '<S94>/Action Port'

        Template_v3_08_2020a_B.In1_ky = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S79>/Hold this value'
      }

      // End of If: '<S79>/if we went through a "step"'

      // Gain: '<S79>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT_j = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_ky;

      // DataStoreWrite: '<S78>/RED_Px2'
      Template_v3_08_2020a_DW.RED_Vx = Template_v3_08_2020a_B.dividebydeltaT_j;

      // DataStoreWrite: '<S78>/RED_Px20'
      Template_v3_08_2020a_DW.BLUE_Rz = Template_v3_08_2020a_B.UDPReceive_o1[15];

      // DataStoreWrite: '<S78>/RED_Px3'
      Template_v3_08_2020a_DW.BLACK_Vx = Template_v3_08_2020a_B.dividebydeltaT;

      // DataStoreWrite: '<S78>/RED_Px4'
      Template_v3_08_2020a_DW.BLACK_Px = Template_v3_08_2020a_B.UDPReceive_o1[4];

      // DataStoreWrite: '<S78>/RED_Px5'
      Template_v3_08_2020a_DW.BLACK_Py = Template_v3_08_2020a_B.UDPReceive_o1[5];

      // DataStoreWrite: '<S78>/RED_Px6'
      Template_v3_08_2020a_DW.BLACK_Rz = Template_v3_08_2020a_B.UDPReceive_o1[6];

      // Delay: '<S91>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_df != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_lc =
          Template_v3_08_2020a_B.dividebydeltaT_j;
      }

      // Sum: '<S91>/Sum6' incorporates:
      //   Delay: '<S91>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.dividebydeltaT_j -
        Template_v3_08_2020a_DW.Delay1_DSTATE_lc;

      // If: '<S91>/if we went through a "step"' incorporates:
      //   Inport: '<S106>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S91>/Hold this value' incorporates:
        //   ActionPort: '<S106>/Action Port'

        Template_v3_08_2020a_B.In1_fo = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S91>/Hold this value'
      }

      // End of If: '<S91>/if we went through a "step"'

      // Gain: '<S91>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px7'

      Template_v3_08_2020a_DW.RED_Ax = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_fo;

      // Delay: '<S86>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_pj != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_cd =
          Template_v3_08_2020a_B.UDPReceive_o1[2];
      }

      // Sum: '<S86>/Sum6' incorporates:
      //   Delay: '<S86>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1[2]
        - Template_v3_08_2020a_DW.Delay1_DSTATE_cd;

      // If: '<S86>/if we went through a "step"' incorporates:
      //   Inport: '<S101>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S86>/Hold this value' incorporates:
        //   ActionPort: '<S101>/Action Port'

        Template_v3_08_2020a_B.In1_f0 = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S86>/Hold this value'
      }

      // End of If: '<S86>/if we went through a "step"'

      // Gain: '<S86>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT_d2 = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_f0;

      // Delay: '<S92>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_ap != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_i0 =
          Template_v3_08_2020a_B.dividebydeltaT_d2;
      }

      // Sum: '<S92>/Sum6' incorporates:
      //   Delay: '<S92>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.dividebydeltaT_d2 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_i0;

      // If: '<S92>/if we went through a "step"' incorporates:
      //   Inport: '<S107>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S92>/Hold this value' incorporates:
        //   ActionPort: '<S107>/Action Port'

        Template_v3_08_2020a_B.In1_db = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S92>/Hold this value'
      }

      // End of If: '<S92>/if we went through a "step"'

      // Gain: '<S92>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px8'

      Template_v3_08_2020a_DW.RED_Ay = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_db;

      // S-Function (sdspunwrap2): '<S78>/Unwrap'
      if (Template_v3_08_2020a_DW.Unwrap_FirstStep) {
        Template_v3_08_2020a_B.i = 0;
        while (Template_v3_08_2020a_B.i < 1) {
          Template_v3_08_2020a_DW.Unwrap_Prev =
            Template_v3_08_2020a_B.UDPReceive_o1[3];
          Template_v3_08_2020a_B.i++;
        }

        Template_v3_08_2020a_DW.Unwrap_FirstStep = false;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_DW.Unwrap_Cumsum;
        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.UDPReceive_o1[3]
          - Template_v3_08_2020a_DW.Unwrap_Prev;
        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l - floor
          ((Template_v3_08_2020a_B.Sum6_l + 3.1415926535897931) /
           6.2831853071795862) * 6.2831853071795862;
        if ((Template_v3_08_2020a_B.Sum6_d == -3.1415926535897931) &&
            (Template_v3_08_2020a_B.Sum6_l > 0.0)) {
          Template_v3_08_2020a_B.Sum6_d = 3.1415926535897931;
        }

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Sum6_d -
          Template_v3_08_2020a_B.Sum6_l;
        if (fabs(Template_v3_08_2020a_B.Sum6_l) > 3.1415926535897931) {
          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_DW.Unwrap_Cumsum +
            Template_v3_08_2020a_B.Sum6_l;
        }

        Template_v3_08_2020a_DW.Unwrap_Prev =
          Template_v3_08_2020a_B.UDPReceive_o1[3];
        Template_v3_08_2020a_B.Unwrap = Template_v3_08_2020a_B.UDPReceive_o1[3]
          + Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_DW.Unwrap_Cumsum =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S78>/Unwrap'

      // Delay: '<S87>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_hr != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_jx = Template_v3_08_2020a_B.Unwrap;
      }

      // Sum: '<S87>/Sum6' incorporates:
      //   Delay: '<S87>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Unwrap -
        Template_v3_08_2020a_DW.Delay1_DSTATE_jx;

      // If: '<S87>/if we went through a "step"' incorporates:
      //   Inport: '<S102>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S87>/Hold this value' incorporates:
        //   ActionPort: '<S102>/Action Port'

        Template_v3_08_2020a_B.In1_lb = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S87>/Hold this value'
      }

      // End of If: '<S87>/if we went through a "step"'

      // Gain: '<S87>/divide by delta T'
      Template_v3_08_2020a_B.dividebydeltaT_l = 1.0 /
        Template_v3_08_2020a_P.baseRate * Template_v3_08_2020a_B.In1_lb;

      // Delay: '<S93>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_al != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_ml =
          Template_v3_08_2020a_B.dividebydeltaT_l;
      }

      // Sum: '<S93>/Sum6' incorporates:
      //   Delay: '<S93>/Delay1'

      Template_v3_08_2020a_B.Subtract_o =
        Template_v3_08_2020a_B.dividebydeltaT_l -
        Template_v3_08_2020a_DW.Delay1_DSTATE_ml;

      // If: '<S93>/if we went through a "step"' incorporates:
      //   Inport: '<S108>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S93>/Hold this value' incorporates:
        //   ActionPort: '<S108>/Action Port'

        Template_v3_08_2020a_B.In1_dn = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S93>/Hold this value'
      }

      // End of If: '<S93>/if we went through a "step"'

      // Gain: '<S93>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Px9'

      Template_v3_08_2020a_DW.RED_RzDD = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_dn;

      // DataStoreWrite: '<S78>/RED_Py1'
      Template_v3_08_2020a_DW.RED_Py = Template_v3_08_2020a_B.UDPReceive_o1[2];

      // DataStoreWrite: '<S78>/RED_Py2'
      Template_v3_08_2020a_DW.RED_Vy = Template_v3_08_2020a_B.dividebydeltaT_d2;

      // DataStoreWrite: '<S78>/RED_Py3'
      Template_v3_08_2020a_DW.BLACK_Vy = Template_v3_08_2020a_B.dividebydeltaT_k;

      // Delay: '<S84>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_dt != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_ic =
          Template_v3_08_2020a_B.UDPReceive_o1[14];
      }

      // Sum: '<S84>/Sum6' incorporates:
      //   Delay: '<S84>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.UDPReceive_o1
        [14] - Template_v3_08_2020a_DW.Delay1_DSTATE_ic;

      // If: '<S84>/if we went through a "step"' incorporates:
      //   Inport: '<S99>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S84>/Hold this value' incorporates:
        //   ActionPort: '<S99>/Action Port'

        Template_v3_08_2020a_B.In1_as = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S84>/Hold this value'
      }

      // End of If: '<S84>/if we went through a "step"'

      // Gain: '<S84>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Py4'

      Template_v3_08_2020a_DW.BLUE_Vy = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_as;

      // DataStoreWrite: '<S78>/RED_Rz1'
      Template_v3_08_2020a_DW.RED_Rz = Template_v3_08_2020a_B.UDPReceive_o1[3];

      // DataStoreWrite: '<S78>/RED_Rz2'
      Template_v3_08_2020a_DW.RED_RzD = Template_v3_08_2020a_B.dividebydeltaT_l;

      // DataStoreWrite: '<S78>/RED_Rz3'
      Template_v3_08_2020a_DW.BLACK_RzD =
        Template_v3_08_2020a_B.dividebydeltaT_d;

      // S-Function (sdspunwrap2): '<S78>/Unwrap2'
      if (Template_v3_08_2020a_DW.Unwrap2_FirstStep) {
        Template_v3_08_2020a_B.i = 0;
        while (Template_v3_08_2020a_B.i < 1) {
          Template_v3_08_2020a_DW.Unwrap2_Prev =
            Template_v3_08_2020a_B.UDPReceive_o1[15];
          Template_v3_08_2020a_B.i++;
        }

        Template_v3_08_2020a_DW.Unwrap2_FirstStep = false;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o =
          Template_v3_08_2020a_DW.Unwrap2_Cumsum;
        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.UDPReceive_o1[15]
          - Template_v3_08_2020a_DW.Unwrap2_Prev;
        Template_v3_08_2020a_B.Sum6_d = Template_v3_08_2020a_B.Sum6_l - floor
          ((Template_v3_08_2020a_B.Sum6_l + 3.1415926535897931) /
           6.2831853071795862) * 6.2831853071795862;
        if ((Template_v3_08_2020a_B.Sum6_d == -3.1415926535897931) &&
            (Template_v3_08_2020a_B.Sum6_l > 0.0)) {
          Template_v3_08_2020a_B.Sum6_d = 3.1415926535897931;
        }

        Template_v3_08_2020a_B.Sum6_l = Template_v3_08_2020a_B.Sum6_d -
          Template_v3_08_2020a_B.Sum6_l;
        if (fabs(Template_v3_08_2020a_B.Sum6_l) > 3.1415926535897931) {
          Template_v3_08_2020a_B.Subtract_o =
            Template_v3_08_2020a_DW.Unwrap2_Cumsum +
            Template_v3_08_2020a_B.Sum6_l;
        }

        Template_v3_08_2020a_DW.Unwrap2_Prev =
          Template_v3_08_2020a_B.UDPReceive_o1[15];
        Template_v3_08_2020a_B.Unwrap2 = Template_v3_08_2020a_B.UDPReceive_o1[15]
          + Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_DW.Unwrap2_Cumsum =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspunwrap2): '<S78>/Unwrap2'

      // Delay: '<S85>/Delay1'
      if (Template_v3_08_2020a_DW.icLoad_l1b != 0) {
        Template_v3_08_2020a_DW.Delay1_DSTATE_a2 =
          Template_v3_08_2020a_B.Unwrap2;
      }

      // Sum: '<S85>/Sum6' incorporates:
      //   Delay: '<S85>/Delay1'

      Template_v3_08_2020a_B.Subtract_o = Template_v3_08_2020a_B.Unwrap2 -
        Template_v3_08_2020a_DW.Delay1_DSTATE_a2;

      // If: '<S85>/if we went through a "step"' incorporates:
      //   Inport: '<S100>/In1'

      if (Template_v3_08_2020a_B.Subtract_o != 0.0) {
        // Outputs for IfAction SubSystem: '<S85>/Hold this value' incorporates:
        //   ActionPort: '<S100>/Action Port'

        Template_v3_08_2020a_B.In1_lu = Template_v3_08_2020a_B.Subtract_o;

        // End of Outputs for SubSystem: '<S85>/Hold this value'
      }

      // End of If: '<S85>/if we went through a "step"'

      // Gain: '<S85>/divide by delta T' incorporates:
      //   DataStoreWrite: '<S78>/RED_Rz4'

      Template_v3_08_2020a_DW.BLUE_RzD = 1.0 / Template_v3_08_2020a_P.baseRate *
        Template_v3_08_2020a_B.In1_lu;

      // End of Outputs for SubSystem: '<S5>/Use Hardware to Obtain States'
    } else {
      if (Template_v3_08_2020a_P.simMode == 1.0) {
        Template_v3_08_2020a_B.rtAction = 1;

        // Outputs for IfAction SubSystem: '<S5>/Initialize Universal Time (Simulation)' incorporates:
        //   ActionPort: '<S76>/Action Port'

        // Clock: '<S76>/Set Universal Time (If this is a simulation)' incorporates:
        //   DataStoreWrite: '<S76>/Universal_Time'

        Template_v3_08_2020a_DW.Univ_Time = Template_v3_08_2020a_M->Timing.t[0];

        // End of Outputs for SubSystem: '<S5>/Initialize Universal Time (Simulation)' 
      }
    }

    Template_v3_08_2020a_DW.Ifperforminganexperimentgrabthe =
      Template_v3_08_2020a_B.rtAction;

    // End of If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
    Template_v3_08_2020a_B.status = 24U;
    memcpy((void *)&Template_v3_08_2020a_B.SwappedDataBytes, (void *)
           &Template_v3_08_2020a_B.status, (uint32_T)((size_t)1 * sizeof(uint8_T)));
    Template_v3_08_2020a_B.status = MW_I2C_MasterWrite
      (Template_v3_08_2020a_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
       &Template_v3_08_2020a_B.SwappedDataBytes, 1U, true, false);
    if (0 == Template_v3_08_2020a_B.status) {
      MW_I2C_MasterRead(Template_v3_08_2020a_DW.obj.i2cobj_A_G.MW_I2C_HANDLE,
                        106U, Template_v3_08_2020a_B.output_raw, 6U, false, true);
      memcpy((void *)&Template_v3_08_2020a_B.b_RegisterValue[0], (void *)
             &Template_v3_08_2020a_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Template_v3_08_2020a_B.b_RegisterValue[0] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[1] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Template_v3_08_2020a_B.b_k[0],
           &Template_v3_08_2020a_DW.obj.CalGyroA[0], 9U * sizeof(real_T));
    for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
         Template_v3_08_2020a_B.i++) {
      Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] =
        ((Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 1] *
          static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[1]) +
          Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i] *
          static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[0])) +
         Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 2] *
         static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[2])) +
        Template_v3_08_2020a_DW.obj.CalGyroB[Template_v3_08_2020a_B.i];
    }

    Template_v3_08_2020a_B.status = 40U;
    memcpy((void *)&Template_v3_08_2020a_B.SwappedDataBytes, (void *)
           &Template_v3_08_2020a_B.status, (uint32_T)((size_t)1 * sizeof(uint8_T)));
    Template_v3_08_2020a_B.status = MW_I2C_MasterWrite
      (Template_v3_08_2020a_DW.obj.i2cobj_A_G.MW_I2C_HANDLE, 106U,
       &Template_v3_08_2020a_B.SwappedDataBytes, 1U, true, false);
    if (0 == Template_v3_08_2020a_B.status) {
      MW_I2C_MasterRead(Template_v3_08_2020a_DW.obj.i2cobj_A_G.MW_I2C_HANDLE,
                        106U, Template_v3_08_2020a_B.output_raw, 6U, false, true);
      memcpy((void *)&Template_v3_08_2020a_B.b_RegisterValue[0], (void *)
             &Template_v3_08_2020a_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Template_v3_08_2020a_B.b_RegisterValue[0] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[1] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Template_v3_08_2020a_B.b_k[0],
           &Template_v3_08_2020a_DW.obj.CalAccelA[0], 9U * sizeof(real_T));
    for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
         Template_v3_08_2020a_B.i++) {
      Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.i] =
        ((Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 1] *
          static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[1]) +
          Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i] *
          static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[0])) +
         Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 2] *
         static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[2])) +
        Template_v3_08_2020a_DW.obj.CalAccelB[Template_v3_08_2020a_B.i];
    }

    Template_v3_08_2020a_B.status = 40U;
    memcpy((void *)&Template_v3_08_2020a_B.SwappedDataBytes, (void *)
           &Template_v3_08_2020a_B.status, (uint32_T)((size_t)1 * sizeof(uint8_T)));
    Template_v3_08_2020a_B.status = MW_I2C_MasterWrite
      (Template_v3_08_2020a_DW.obj.i2cobj_MAG.MW_I2C_HANDLE, 28U,
       &Template_v3_08_2020a_B.SwappedDataBytes, 1U, true, false);
    if (0 == Template_v3_08_2020a_B.status) {
      MW_I2C_MasterRead(Template_v3_08_2020a_DW.obj.i2cobj_MAG.MW_I2C_HANDLE,
                        28U, Template_v3_08_2020a_B.output_raw, 6U, false, true);
      memcpy((void *)&Template_v3_08_2020a_B.b_RegisterValue[0], (void *)
             &Template_v3_08_2020a_B.output_raw[0], (uint32_T)((size_t)3 *
              sizeof(int16_T)));
    } else {
      Template_v3_08_2020a_B.b_RegisterValue[0] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[1] = 0;
      Template_v3_08_2020a_B.b_RegisterValue[2] = 0;
    }

    memcpy(&Template_v3_08_2020a_B.b_k[0], &Template_v3_08_2020a_DW.obj.CalMagA
           [0], 9U * sizeof(real_T));
    for (Template_v3_08_2020a_B.i = 0; Template_v3_08_2020a_B.i < 3;
         Template_v3_08_2020a_B.i++) {
      Template_v3_08_2020a_B.Subtract_o = ((Template_v3_08_2020a_B.b_k[3 *
        Template_v3_08_2020a_B.i + 1] * static_cast<real_T>
        (Template_v3_08_2020a_B.b_RegisterValue[1]) +
        Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i] *
        static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[0])) +
        Template_v3_08_2020a_B.b_k[3 * Template_v3_08_2020a_B.i + 2] *
        static_cast<real_T>(Template_v3_08_2020a_B.b_RegisterValue[2])) +
        Template_v3_08_2020a_DW.obj.CalMagB[Template_v3_08_2020a_B.i];
      Template_v3_08_2020a_B.LSM9DS1IMUSensor_o3[Template_v3_08_2020a_B.i] =
        Template_v3_08_2020a_B.Subtract_o * 4.0 / 32768.0;
      Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] =
        Template_v3_08_2020a_B.x_ddot_b[Template_v3_08_2020a_B.i] * 245.0 /
        32768.0;
      Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.i] =
        Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.i] *
        2.0 / 32768.0;
    }

    // End of MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'

    // If: '<S6>/This IF block determines whether or not to run the BLACK sim//exp' incorporates:
    //   Constant: '<S112>/Constant'
    //   DataStoreRead: '<S6>/Data Store Read'

    if ((Template_v3_08_2020a_DW.WhoAmI == 2.0) &&
        (Template_v3_08_2020a_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change BLACK Behavior' incorporates:
      //   ActionPort: '<S109>/Action Port'

      // Gain: '<S109>/Gain'
      Template_v3_08_2020a_B.Gain_o[0] = Template_v3_08_2020a_P.Gain_Gain *
        Template_v3_08_2020a_B.x_ddot_b[0];
      Template_v3_08_2020a_B.Gain_o[1] = Template_v3_08_2020a_P.Gain_Gain *
        Template_v3_08_2020a_B.x_ddot_b[1];
      Template_v3_08_2020a_B.Gain_o[2] = Template_v3_08_2020a_P.Gain_Gain *
        Template_v3_08_2020a_B.x_ddot_b[2];

      // If: '<S119>/If' incorporates:
      //   DataStoreRead: '<S124>/Universal_Time'
      //   Inport: '<S123>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S119>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S122>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_o[0],
          &Template_v3_08_2020a_B.CalculateRunningMean,
          &Template_v3_08_2020a_DW.CalculateRunningMean);

        // End of Outputs for SubSystem: '<S119>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S119>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S123>/Action Port'

        Template_v3_08_2020a_B.In1_oj = Template_v3_08_2020a_B.Gain_o[0];

        // End of Outputs for SubSystem: '<S119>/Pass Current Gyro'
      }

      // End of If: '<S119>/If'

      // Sum: '<S119>/Subtract' incorporates:
      //   DataStoreWrite: '<S109>/RED_Px1'

      Template_v3_08_2020a_DW.BLACK_IMU_Q = Template_v3_08_2020a_B.In1_oj -
        Template_v3_08_2020a_B.CalculateRunningMean.Mean;

      // Gain: '<S109>/Gain1'
      Template_v3_08_2020a_B.Sum_l[0] = Template_v3_08_2020a_P.Gain1_Gain *
        Template_v3_08_2020a_B.VelocitytoPosition[0];
      Template_v3_08_2020a_B.Sum_l[1] = Template_v3_08_2020a_P.Gain1_Gain *
        Template_v3_08_2020a_B.VelocitytoPosition[1];
      Template_v3_08_2020a_B.Sum_l[2] = Template_v3_08_2020a_P.Gain1_Gain *
        Template_v3_08_2020a_B.VelocitytoPosition[2];

      // S-Function (sdspbiquad): '<S115>/Digital Filter'
      Template_v3_08_2020a_B.vcol = 0;
      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[0]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[1];
        Template_v3_08_2020a_B.DigitalFilter_a[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[0] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[1];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[1] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[0];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[0] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[2]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[3];
        Template_v3_08_2020a_B.DigitalFilter_a[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[2] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[3];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[3] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[2];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[2] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[4]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[5];
        Template_v3_08_2020a_B.DigitalFilter_a[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[4] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[5];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[5] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[4];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_h[4] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspbiquad): '<S115>/Digital Filter'

      // If: '<S132>/If' incorporates:
      //   DataStoreRead: '<S137>/Universal_Time'
      //   Inport: '<S136>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S132>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S135>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter_a[0],
          &Template_v3_08_2020a_B.CalculateRunningMean_e,
          &Template_v3_08_2020a_DW.CalculateRunningMean_e);

        // End of Outputs for SubSystem: '<S132>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S132>/Pass Current Accel' incorporates:
        //   ActionPort: '<S136>/Action Port'

        Template_v3_08_2020a_B.In1_dm = Template_v3_08_2020a_B.DigitalFilter_a[0];

        // End of Outputs for SubSystem: '<S132>/Pass Current Accel'
      }

      // End of If: '<S132>/If'

      // If: '<S133>/If' incorporates:
      //   DataStoreRead: '<S140>/Universal_Time'
      //   Inport: '<S139>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S133>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S138>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter_a[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_c,
          &Template_v3_08_2020a_DW.CalculateRunningMean_c);

        // End of Outputs for SubSystem: '<S133>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S133>/Pass Current Accel' incorporates:
        //   ActionPort: '<S139>/Action Port'

        Template_v3_08_2020a_B.In1_j5 = Template_v3_08_2020a_B.DigitalFilter_a[1];

        // End of Outputs for SubSystem: '<S133>/Pass Current Accel'
      }

      // End of If: '<S133>/If'

      // If: '<S134>/If' incorporates:
      //   DataStoreRead: '<S143>/Universal_Time'
      //   Inport: '<S142>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S134>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S141>/Action Port'

        Temp_CalculateRunningMean_e(Template_v3_08_2020a_B.DigitalFilter_a[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_ep,
          &Template_v3_08_2020a_DW.CalculateRunningMean_ep,
          &Template_v3_08_2020a_P.CalculateRunningMean_ep);

        // End of Outputs for SubSystem: '<S134>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S134>/Pass Current Accel' incorporates:
        //   ActionPort: '<S142>/Action Port'

        Template_v3_08_2020a_B.In1_opy = Template_v3_08_2020a_B.DigitalFilter_a
          [2];

        // End of Outputs for SubSystem: '<S134>/Pass Current Accel'
      }

      // End of If: '<S134>/If'

      // MATLAB Function: '<S109>/ChangeOrientation' incorporates:
      //   Sum: '<S132>/Subtract'
      //   Sum: '<S133>/Subtract'
      //   Sum: '<S134>/Subtract'

      Template__ChangeOrientation(Template_v3_08_2020a_B.In1_dm -
        Template_v3_08_2020a_B.CalculateRunningMean_e.Mean,
        Template_v3_08_2020a_B.In1_j5 -
        Template_v3_08_2020a_B.CalculateRunningMean_c.Mean,
        Template_v3_08_2020a_B.In1_opy -
        Template_v3_08_2020a_B.CalculateRunningMean_ep.Subtract,
        &Template_v3_08_2020a_B.sf_ChangeOrientation);

      // DataStoreWrite: '<S109>/RED_Px10'
      Template_v3_08_2020a_DW.BLACK_IMU_Ay_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S109>/RED_Px11'
      Template_v3_08_2020a_DW.BLACK_IMU_Az_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[2];

      // If: '<S120>/If' incorporates:
      //   DataStoreRead: '<S127>/Universal_Time'
      //   Inport: '<S126>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S120>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S125>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_o[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_i,
          &Template_v3_08_2020a_DW.CalculateRunningMean_i);

        // End of Outputs for SubSystem: '<S120>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S120>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S126>/Action Port'

        Template_v3_08_2020a_B.In1_ai = Template_v3_08_2020a_B.Gain_o[1];

        // End of Outputs for SubSystem: '<S120>/Pass Current Gyro'
      }

      // End of If: '<S120>/If'

      // Sum: '<S120>/Subtract' incorporates:
      //   DataStoreWrite: '<S109>/RED_Px2'

      Template_v3_08_2020a_DW.BLACK_IMU_P = Template_v3_08_2020a_B.In1_ai -
        Template_v3_08_2020a_B.CalculateRunningMean_i.Mean;

      // If: '<S121>/If' incorporates:
      //   DataStoreRead: '<S131>/Universal_Time'
      //   Inport: '<S130>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S121>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S128>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_o[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_g,
          &Template_v3_08_2020a_DW.CalculateRunningMean_g);

        // End of Outputs for SubSystem: '<S121>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S121>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S130>/Action Port'

        Template_v3_08_2020a_B.In1_i4 = Template_v3_08_2020a_B.Gain_o[2];

        // End of Outputs for SubSystem: '<S121>/Pass Current Gyro'
      }

      // End of If: '<S121>/If'

      // Sum: '<S121>/Subtract' incorporates:
      //   DataStoreWrite: '<S109>/RED_Px3'

      Template_v3_08_2020a_DW.BLACK_IMU_R = Template_v3_08_2020a_B.In1_i4 -
        Template_v3_08_2020a_B.CalculateRunningMean_g.Mean;

      // MATLAB Function: '<S109>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S109>/Data Store Read'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.BLACK_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_iq);

      // DataStoreWrite: '<S109>/RED_Px4' incorporates:
      //   Product: '<S109>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLACK_IMU_Ax_I =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_iq.C_Ib[0] *
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_iq.C_Ib[2] *
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S109>/RED_Px5' incorporates:
      //   Product: '<S109>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.BLACK_IMU_Ay_I =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_iq.C_Ib[1] *
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_iq.C_Ib[3] *
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[1];

      // DataStoreWrite: '<S109>/RED_Px6'
      Template_v3_08_2020a_DW.BLACK_IMU_Ax_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation.y[0];
      Template_v3_08_2020a_AHRS2(Template_v3_08_2020a_B.DigitalFilter_a,
        Template_v3_08_2020a_B.Gain_o,
        Template_v3_08_2020a_B.LSM9DS1IMUSensor_o3,
        &Template_v3_08_2020a_B.AHRS2, &Template_v3_08_2020a_DW.AHRS2,
        &Template_v3_08_2020a_P.AHRS2);

      // DataStoreWrite: '<S109>/RED_Px7'
      Template_v3_08_2020a_DW.BLACK_AHRS_Q =
        Template_v3_08_2020a_B.AHRS2.AHRS2_o2[0];

      // DataStoreWrite: '<S109>/RED_Px8'
      Template_v3_08_2020a_DW.BLACK_AHRS_P =
        Template_v3_08_2020a_B.AHRS2.AHRS2_o2[1];

      // DataStoreWrite: '<S109>/RED_Px9'
      Template_v3_08_2020a_DW.BLACK_AHRS_R =
        Template_v3_08_2020a_B.AHRS2.AHRS2_o2[2];

      // RelationalOperator: '<S129>/Compare' incorporates:
      //   Constant: '<S129>/Constant'

      Template_v3_08_2020a_B.rtb_Compare_j = (Template_v3_08_2020a_B.In1_i4 ==
        Template_v3_08_2020a_P.Constant_Value);

      // DiscreteIntegrator: '<S121>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S121>/Data Store Read1'
      //   DataStoreRead: '<S121>/Data Store Read3'
      //   DataStoreWrite: '<S121>/RED_Px4'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_c != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_b =
          Template_v3_08_2020a_DW.BLACK_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_j &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_Prev_hi <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_j) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_Prev_hi == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_b =
          Template_v3_08_2020a_DW.BLACK_Rz;
      }

      Template_v3_08_2020a_DW.BLACK_IMU_Psi =
        Template_v3_08_2020a_P.DiscreteTimeIntegrator1_gainval *
        Template_v3_08_2020a_DW.BLACK_IMU_R +
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_b;

      // End of DiscreteIntegrator: '<S121>/Discrete-Time Integrator1'

      // Update for DiscreteIntegrator: '<S121>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S121>/RED_Px4'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_c = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_b =
        Template_v3_08_2020a_DW.BLACK_IMU_Psi;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_Prev_hi =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_j);

      // End of Outputs for SubSystem: '<S6>/Change BLACK Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the BLACK sim//exp' 

    // If: '<S6>/This IF block determines whether or not to run the BLUE sim//exp' incorporates:
    //   Constant: '<S112>/Constant'
    //   DataStoreRead: '<S6>/Data Store Read'

    if ((Template_v3_08_2020a_DW.WhoAmI == 3.0) &&
        (Template_v3_08_2020a_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change BLUE Behavior' incorporates:
      //   ActionPort: '<S110>/Action Port'

      // Gain: '<S110>/Gain'
      Template_v3_08_2020a_B.Gain_c[0] = Template_v3_08_2020a_P.Gain_Gain_k *
        Template_v3_08_2020a_B.x_ddot_b[0];
      Template_v3_08_2020a_B.Gain_c[1] = Template_v3_08_2020a_P.Gain_Gain_k *
        Template_v3_08_2020a_B.x_ddot_b[1];
      Template_v3_08_2020a_B.Gain_c[2] = Template_v3_08_2020a_P.Gain_Gain_k *
        Template_v3_08_2020a_B.x_ddot_b[2];

      // If: '<S150>/If' incorporates:
      //   DataStoreRead: '<S155>/Universal_Time'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S150>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S153>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_c[0],
          &Template_v3_08_2020a_B.CalculateRunningMean_d,
          &Template_v3_08_2020a_DW.CalculateRunningMean_d);

        // End of Outputs for SubSystem: '<S150>/Calculate Running Mean'
      }

      // End of If: '<S150>/If'

      // Gain: '<S110>/Gain1'
      Template_v3_08_2020a_B.Sum_l[0] = Template_v3_08_2020a_P.Gain1_Gain_h *
        Template_v3_08_2020a_B.VelocitytoPosition[0];
      Template_v3_08_2020a_B.Sum_l[1] = Template_v3_08_2020a_P.Gain1_Gain_h *
        Template_v3_08_2020a_B.VelocitytoPosition[1];
      Template_v3_08_2020a_B.Sum_l[2] = Template_v3_08_2020a_P.Gain1_Gain_h *
        Template_v3_08_2020a_B.VelocitytoPosition[2];

      // S-Function (sdspbiquad): '<S146>/Digital Filter'
      Template_v3_08_2020a_B.vcol = 0;
      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[0]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[1];
        Template_v3_08_2020a_B.DigitalFilter_i[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[0] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[1];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[1] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[0];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[0] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[2]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[3];
        Template_v3_08_2020a_B.DigitalFilter_i[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[2] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[3];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[3] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[2];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[2] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.Sum_l[Template_v3_08_2020a_B.vcol] -
          -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[4]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[5];
        Template_v3_08_2020a_B.DigitalFilter_i[Template_v3_08_2020a_B.vcol] =
          (2.0 * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[4] +
           Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[5];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[5] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[4];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES_p[4] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspbiquad): '<S146>/Digital Filter'

      // If: '<S163>/If' incorporates:
      //   DataStoreRead: '<S168>/Universal_Time'
      //   Inport: '<S167>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S163>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S166>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter_i[0],
          &Template_v3_08_2020a_B.CalculateRunningMean_n,
          &Template_v3_08_2020a_DW.CalculateRunningMean_n);

        // End of Outputs for SubSystem: '<S163>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S163>/Pass Current Accel' incorporates:
        //   ActionPort: '<S167>/Action Port'

        Template_v3_08_2020a_B.In1_pf = Template_v3_08_2020a_B.DigitalFilter_i[0];

        // End of Outputs for SubSystem: '<S163>/Pass Current Accel'
      }

      // End of If: '<S163>/If'

      // If: '<S164>/If' incorporates:
      //   DataStoreRead: '<S171>/Universal_Time'
      //   Inport: '<S170>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S164>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S169>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter_i[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_h,
          &Template_v3_08_2020a_DW.CalculateRunningMean_h);

        // End of Outputs for SubSystem: '<S164>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S164>/Pass Current Accel' incorporates:
        //   ActionPort: '<S170>/Action Port'

        Template_v3_08_2020a_B.In1_np = Template_v3_08_2020a_B.DigitalFilter_i[1];

        // End of Outputs for SubSystem: '<S164>/Pass Current Accel'
      }

      // End of If: '<S164>/If'

      // If: '<S165>/If' incorporates:
      //   DataStoreRead: '<S174>/Universal_Time'
      //   Inport: '<S173>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S165>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S172>/Action Port'

        Temp_CalculateRunningMean_e(Template_v3_08_2020a_B.DigitalFilter_i[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_o,
          &Template_v3_08_2020a_DW.CalculateRunningMean_o,
          &Template_v3_08_2020a_P.CalculateRunningMean_o);

        // End of Outputs for SubSystem: '<S165>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S165>/Pass Current Accel' incorporates:
        //   ActionPort: '<S173>/Action Port'

        Template_v3_08_2020a_B.In1_m0 = Template_v3_08_2020a_B.DigitalFilter_i[2];

        // End of Outputs for SubSystem: '<S165>/Pass Current Accel'
      }

      // End of If: '<S165>/If'

      // MATLAB Function: '<S110>/ChangeOrientation' incorporates:
      //   Sum: '<S163>/Subtract'
      //   Sum: '<S164>/Subtract'
      //   Sum: '<S165>/Subtract'

      Template__ChangeOrientation(Template_v3_08_2020a_B.In1_pf -
        Template_v3_08_2020a_B.CalculateRunningMean_n.Mean,
        Template_v3_08_2020a_B.In1_np -
        Template_v3_08_2020a_B.CalculateRunningMean_h.Mean,
        Template_v3_08_2020a_B.In1_m0 -
        Template_v3_08_2020a_B.CalculateRunningMean_o.Subtract,
        &Template_v3_08_2020a_B.sf_ChangeOrientation_l);

      // If: '<S151>/If' incorporates:
      //   DataStoreRead: '<S158>/Universal_Time'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S151>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S156>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_c[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_b,
          &Template_v3_08_2020a_DW.CalculateRunningMean_b);

        // End of Outputs for SubSystem: '<S151>/Calculate Running Mean'
      }

      // End of If: '<S151>/If'

      // If: '<S152>/If' incorporates:
      //   DataStoreRead: '<S162>/Universal_Time'
      //   Inport: '<S161>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S152>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S159>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain_c[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_l,
          &Template_v3_08_2020a_DW.CalculateRunningMean_l);

        // End of Outputs for SubSystem: '<S152>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S152>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S161>/Action Port'

        Template_v3_08_2020a_B.In1_n3 = Template_v3_08_2020a_B.Gain_c[2];

        // End of Outputs for SubSystem: '<S152>/Pass Current Gyro'
      }

      // End of If: '<S152>/If'

      // MATLAB Function: '<S110>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S110>/Data Store Read'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.BLUE_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_p);
      Template_v3_08_2020a_AHRS2(Template_v3_08_2020a_B.DigitalFilter_i,
        Template_v3_08_2020a_B.Gain_c,
        Template_v3_08_2020a_B.LSM9DS1IMUSensor_o3,
        &Template_v3_08_2020a_B.AHRS2_p, &Template_v3_08_2020a_DW.AHRS2_p,
        &Template_v3_08_2020a_P.AHRS2_p);

      // RelationalOperator: '<S160>/Compare' incorporates:
      //   Constant: '<S160>/Constant'

      Template_v3_08_2020a_B.rtb_Compare_j = (Template_v3_08_2020a_B.In1_n3 ==
        Template_v3_08_2020a_P.Constant_Value_f);

      // DiscreteIntegrator: '<S152>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S152>/Data Store Read1'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_L_gl != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_j =
          Template_v3_08_2020a_DW.BLUE_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_j &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_f <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_j) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_f == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_j =
          Template_v3_08_2020a_DW.BLUE_Rz;
      }

      // Update for DiscreteIntegrator: '<S152>/Discrete-Time Integrator1' incorporates:
      //   Sum: '<S152>/Subtract'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_L_gl = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_j +=
        (Template_v3_08_2020a_B.In1_n3 -
         Template_v3_08_2020a_B.CalculateRunningMean_l.Mean) *
        Template_v3_08_2020a_P.DiscreteTimeIntegrator1_gainv_a;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_f =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_j);

      // End of Outputs for SubSystem: '<S6>/Change BLUE Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the BLUE sim//exp' 

    // If: '<S6>/This IF block determines whether or not to run the RED sim//exp ' incorporates:
    //   Constant: '<S112>/Constant'
    //   DataStoreRead: '<S6>/Data Store Read'
    //   Gain: '<S111>/Gain1'

    if ((Template_v3_08_2020a_DW.WhoAmI == 1.0) &&
        (Template_v3_08_2020a_P.simMode == 0.0)) {
      // Outputs for IfAction SubSystem: '<S6>/Change RED Behavior' incorporates:
      //   ActionPort: '<S111>/Action Port'

      // Gain: '<S111>/Gain'
      Template_v3_08_2020a_B.Gain[0] = Template_v3_08_2020a_P.Gain_Gain_h *
        Template_v3_08_2020a_B.x_ddot_b[0];
      Template_v3_08_2020a_B.Gain[1] = Template_v3_08_2020a_P.Gain_Gain_h *
        Template_v3_08_2020a_B.x_ddot_b[1];
      Template_v3_08_2020a_B.Gain[2] = Template_v3_08_2020a_P.Gain_Gain_h *
        Template_v3_08_2020a_B.x_ddot_b[2];

      // If: '<S181>/If' incorporates:
      //   DataStoreRead: '<S186>/Universal_Time'
      //   Inport: '<S185>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S181>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S184>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain[0],
          &Template_v3_08_2020a_B.CalculateRunningMean_hx,
          &Template_v3_08_2020a_DW.CalculateRunningMean_hx);

        // End of Outputs for SubSystem: '<S181>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S181>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S185>/Action Port'

        Template_v3_08_2020a_B.In1_dy = Template_v3_08_2020a_B.Gain[0];

        // End of Outputs for SubSystem: '<S181>/Pass Current Gyro'
      }

      // End of If: '<S181>/If'

      // Sum: '<S181>/Subtract' incorporates:
      //   DataStoreWrite: '<S111>/RED_Px1'

      Template_v3_08_2020a_DW.RED_IMU_Q = Template_v3_08_2020a_B.In1_dy -
        Template_v3_08_2020a_B.CalculateRunningMean_hx.Mean;
      Template_v3_08_2020a_B.VelocitytoPosition[0] *=
        Template_v3_08_2020a_P.Gain1_Gain_g;
      Template_v3_08_2020a_B.VelocitytoPosition[1] *=
        Template_v3_08_2020a_P.Gain1_Gain_g;
      Template_v3_08_2020a_B.VelocitytoPosition[2] *=
        Template_v3_08_2020a_P.Gain1_Gain_g;

      // S-Function (sdspbiquad): '<S177>/Digital Filter' incorporates:
      //   Gain: '<S111>/Gain1'

      Template_v3_08_2020a_B.vcol = 0;
      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.vcol]
          - -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[0]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[1];
        Template_v3_08_2020a_B.DigitalFilter[Template_v3_08_2020a_B.vcol] = (2.0
          * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[0] +
          Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[1];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[1] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[0];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[0] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.vcol]
          - -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[2]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[3];
        Template_v3_08_2020a_B.DigitalFilter[Template_v3_08_2020a_B.vcol] = (2.0
          * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[2] +
          Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[3];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[3] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[2];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[2] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_B.Subtract_o = (0.29289321881345243 *
          Template_v3_08_2020a_B.VelocitytoPosition[Template_v3_08_2020a_B.vcol]
          - -1.3007071811330761E-16 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[4]) -
          0.17157287525380996 *
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[5];
        Template_v3_08_2020a_B.DigitalFilter[Template_v3_08_2020a_B.vcol] = (2.0
          * Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[4] +
          Template_v3_08_2020a_B.Subtract_o) +
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[5];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[5] =
          Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[4];
        Template_v3_08_2020a_DW.DigitalFilter_FILT_STATES[4] =
          Template_v3_08_2020a_B.Subtract_o;
        Template_v3_08_2020a_B.vcol++;
        Template_v3_08_2020a_B.i++;
      }

      // End of S-Function (sdspbiquad): '<S177>/Digital Filter'

      // If: '<S194>/If' incorporates:
      //   DataStoreRead: '<S199>/Universal_Time'
      //   Inport: '<S198>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S194>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S197>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter[0],
          &Template_v3_08_2020a_B.CalculateRunningMean_f,
          &Template_v3_08_2020a_DW.CalculateRunningMean_f);

        // End of Outputs for SubSystem: '<S194>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S194>/Pass Current Accel' incorporates:
        //   ActionPort: '<S198>/Action Port'

        Template_v3_08_2020a_B.In1_eo = Template_v3_08_2020a_B.DigitalFilter[0];

        // End of Outputs for SubSystem: '<S194>/Pass Current Accel'
      }

      // End of If: '<S194>/If'

      // If: '<S195>/If' incorporates:
      //   DataStoreRead: '<S202>/Universal_Time'
      //   Inport: '<S201>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S195>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S200>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.DigitalFilter[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_o2,
          &Template_v3_08_2020a_DW.CalculateRunningMean_o2);

        // End of Outputs for SubSystem: '<S195>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S195>/Pass Current Accel' incorporates:
        //   ActionPort: '<S201>/Action Port'

        Template_v3_08_2020a_B.In1_pm = Template_v3_08_2020a_B.DigitalFilter[1];

        // End of Outputs for SubSystem: '<S195>/Pass Current Accel'
      }

      // End of If: '<S195>/If'

      // If: '<S196>/If' incorporates:
      //   DataStoreRead: '<S205>/Universal_Time'
      //   Inport: '<S204>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S196>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S203>/Action Port'

        Temp_CalculateRunningMean_e(Template_v3_08_2020a_B.DigitalFilter[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_ot,
          &Template_v3_08_2020a_DW.CalculateRunningMean_ot,
          &Template_v3_08_2020a_P.CalculateRunningMean_ot);

        // End of Outputs for SubSystem: '<S196>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S196>/Pass Current Accel' incorporates:
        //   ActionPort: '<S204>/Action Port'

        Template_v3_08_2020a_B.In1_jd = Template_v3_08_2020a_B.DigitalFilter[2];

        // End of Outputs for SubSystem: '<S196>/Pass Current Accel'
      }

      // End of If: '<S196>/If'

      // MATLAB Function: '<S111>/ChangeOrientation' incorporates:
      //   Sum: '<S194>/Subtract'
      //   Sum: '<S195>/Subtract'
      //   Sum: '<S196>/Subtract'

      Template__ChangeOrientation(Template_v3_08_2020a_B.In1_eo -
        Template_v3_08_2020a_B.CalculateRunningMean_f.Mean,
        Template_v3_08_2020a_B.In1_pm -
        Template_v3_08_2020a_B.CalculateRunningMean_o2.Mean,
        Template_v3_08_2020a_B.In1_jd -
        Template_v3_08_2020a_B.CalculateRunningMean_ot.Subtract,
        &Template_v3_08_2020a_B.sf_ChangeOrientation_e);

      // DataStoreWrite: '<S111>/RED_Px10'
      Template_v3_08_2020a_DW.RED_IMU_Ax_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[0];

      // DataStoreWrite: '<S111>/RED_Px11'
      Template_v3_08_2020a_DW.RED_IMU_Ay_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[1];

      // DataStoreWrite: '<S111>/RED_Px12'
      Template_v3_08_2020a_DW.RED_IMU_Az_b =
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[2];

      // If: '<S182>/If' incorporates:
      //   DataStoreRead: '<S189>/Universal_Time'
      //   Inport: '<S188>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S182>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S187>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain[1],
          &Template_v3_08_2020a_B.CalculateRunningMean_hq,
          &Template_v3_08_2020a_DW.CalculateRunningMean_hq);

        // End of Outputs for SubSystem: '<S182>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S182>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S188>/Action Port'

        Template_v3_08_2020a_B.In1_a4 = Template_v3_08_2020a_B.Gain[1];

        // End of Outputs for SubSystem: '<S182>/Pass Current Gyro'
      }

      // End of If: '<S182>/If'

      // Sum: '<S182>/Subtract' incorporates:
      //   DataStoreWrite: '<S111>/RED_Px2'

      Template_v3_08_2020a_DW.RED_IMU_P = Template_v3_08_2020a_B.In1_a4 -
        Template_v3_08_2020a_B.CalculateRunningMean_hq.Mean;

      // If: '<S183>/If' incorporates:
      //   DataStoreRead: '<S193>/Universal_Time'
      //   Inport: '<S192>/In1'

      if (Template_v3_08_2020a_DW.Univ_Time < 5.0) {
        // Outputs for IfAction SubSystem: '<S183>/Calculate Running Mean' incorporates:
        //   ActionPort: '<S190>/Action Port'

        Templa_CalculateRunningMean(Template_v3_08_2020a_B.Gain[2],
          &Template_v3_08_2020a_B.CalculateRunningMean_hn,
          &Template_v3_08_2020a_DW.CalculateRunningMean_hn);

        // End of Outputs for SubSystem: '<S183>/Calculate Running Mean'
      } else {
        // Outputs for IfAction SubSystem: '<S183>/Pass Current Gyro' incorporates:
        //   ActionPort: '<S192>/Action Port'

        Template_v3_08_2020a_B.In1_ed = Template_v3_08_2020a_B.Gain[2];

        // End of Outputs for SubSystem: '<S183>/Pass Current Gyro'
      }

      // End of If: '<S183>/If'

      // Sum: '<S183>/Subtract' incorporates:
      //   DataStoreWrite: '<S111>/RED_Px3'

      Template_v3_08_2020a_DW.RED_IMU_R = Template_v3_08_2020a_B.In1_ed -
        Template_v3_08_2020a_B.CalculateRunningMean_hn.Mean;

      // MATLAB Function: '<S111>/Create Rotation Matrix' incorporates:
      //   DataStoreRead: '<S111>/Data Store Read'

      Templa_CreateRotationMatrix(Template_v3_08_2020a_DW.RED_Rz,
        &Template_v3_08_2020a_B.sf_CreateRotationMatrix_g);

      // DataStoreWrite: '<S111>/RED_Px4' incorporates:
      //   Product: '<S111>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.RED_IMU_Ax_I =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_g.C_Ib[0] *
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_g.C_Ib[2] *
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[1];

      // DataStoreWrite: '<S111>/RED_Px5' incorporates:
      //   Product: '<S111>/Rotate F_b to F_I'

      Template_v3_08_2020a_DW.RED_IMU_Ay_I =
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_g.C_Ib[1] *
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[0] +
        Template_v3_08_2020a_B.sf_CreateRotationMatrix_g.C_Ib[3] *
        Template_v3_08_2020a_B.sf_ChangeOrientation_e.y[1];
      Template_v3_08_2020a_AHRS2(Template_v3_08_2020a_B.DigitalFilter,
        Template_v3_08_2020a_B.Gain, Template_v3_08_2020a_B.LSM9DS1IMUSensor_o3,
        &Template_v3_08_2020a_B.AHRS2_pn, &Template_v3_08_2020a_DW.AHRS2_pn,
        &Template_v3_08_2020a_P.AHRS2_pn);

      // DataStoreWrite: '<S111>/RED_Px7'
      Template_v3_08_2020a_DW.RED_AHRS_P =
        Template_v3_08_2020a_B.AHRS2_pn.AHRS2_o2[1];

      // DataStoreWrite: '<S111>/RED_Px8'
      Template_v3_08_2020a_DW.RED_AHRS_Q =
        Template_v3_08_2020a_B.AHRS2_pn.AHRS2_o2[0];

      // DataStoreWrite: '<S111>/RED_Px9'
      Template_v3_08_2020a_DW.RED_AHRS_R =
        Template_v3_08_2020a_B.AHRS2_pn.AHRS2_o2[2];

      // RelationalOperator: '<S191>/Compare' incorporates:
      //   Constant: '<S191>/Constant'

      Template_v3_08_2020a_B.rtb_Compare_j = (Template_v3_08_2020a_B.In1_ed ==
        Template_v3_08_2020a_P.Constant_Value_i);

      // DiscreteIntegrator: '<S183>/Discrete-Time Integrator1' incorporates:
      //   DataStoreRead: '<S183>/Data Store Read1'
      //   DataStoreRead: '<S183>/Data Store Read3'
      //   DataStoreWrite: '<S183>/RED_Px4'

      if (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_g != 0) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_e =
          Template_v3_08_2020a_DW.RED_Rz;
      }

      if ((Template_v3_08_2020a_B.rtb_Compare_j &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_h <= 0)) || ((
            !Template_v3_08_2020a_B.rtb_Compare_j) &&
           (Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_h == 1))) {
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_e =
          Template_v3_08_2020a_DW.RED_Rz;
      }

      Template_v3_08_2020a_DW.RED_IMU_Psi =
        Template_v3_08_2020a_P.DiscreteTimeIntegrator1_gainv_k *
        Template_v3_08_2020a_DW.RED_IMU_R +
        Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_e;

      // End of DiscreteIntegrator: '<S183>/Discrete-Time Integrator1'

      // Update for DiscreteIntegrator: '<S183>/Discrete-Time Integrator1' incorporates:
      //   DataStoreWrite: '<S183>/RED_Px4'

      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_g = 0U;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_DSTAT_e =
        Template_v3_08_2020a_DW.RED_IMU_Psi;
      Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_h =
        static_cast<int8_T>(Template_v3_08_2020a_B.rtb_Compare_j);

      // End of Outputs for SubSystem: '<S6>/Change RED Behavior'
    }

    // End of If: '<S6>/This IF block determines whether or not to run the RED sim//exp ' 

    // If: '<S8>/This IF block determines whether or not to run the exp code' incorporates:
    //   Constant: '<S207>/Constant'

    if (Template_v3_08_2020a_P.simMode == 0.0) {
      // Outputs for IfAction SubSystem: '<S8>/Change Behavior' incorporates:
      //   ActionPort: '<S206>/Action Port'

      // MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders' incorporates:
      //   DataStoreWrite: '<S206>/Data Store Write3'
      //   DataStoreWrite: '<S206>/Data Store Write4'
      //   DataStoreWrite: '<S206>/Data Store Write5'

      if (Template_v3_08_2020a_DW.obj_c.SampleTime !=
          Template_v3_08_2020a_P.baseRate) {
        if (((!rtIsInf(Template_v3_08_2020a_P.baseRate)) && (!rtIsNaN
              (Template_v3_08_2020a_P.baseRate))) || rtIsInf
            (Template_v3_08_2020a_P.baseRate)) {
          Template_v3_08_2020a_B.sampleTime = Template_v3_08_2020a_P.baseRate;
        }

        Template_v3_08_2020a_DW.obj_c.SampleTime =
          Template_v3_08_2020a_B.sampleTime;
      }

      Template_v3_08_2020a_B.sampleTime = 0.0;
      Template_v3_08_2020a_B.Subtract_o = 0.0;
      Template_v3_08_2020a_B.Sum6_l = 0.0;
      Template_v3_08_2020a_DW.RED_RzD_Shoulder = 0.0;
      Template_v3_08_2020a_DW.RED_RzD_Elbow = 0.0;
      Template_v3_08_2020a_DW.RED_RzD_Wrist = 0.0;
      read_dynamixel_position(&Template_v3_08_2020a_B.sampleTime,
        &Template_v3_08_2020a_B.Subtract_o, &Template_v3_08_2020a_B.Sum6_l,
        &Template_v3_08_2020a_DW.RED_RzD_Shoulder,
        &Template_v3_08_2020a_DW.RED_RzD_Elbow,
        &Template_v3_08_2020a_DW.RED_RzD_Wrist);

      // MATLAB Function: '<S208>/Correct Dropouts' incorporates:
      //   Delay: '<S208>/Delay1'
      //   Delay: '<S208>/Delay2'
      //   Delay: '<S208>/Delay3'
      //   Delay: '<S208>/Delay4'
      //   Delay: '<S208>/Delay5'
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_CorrectDropouts(Template_v3_08_2020a_B.sampleTime,
        Template_v3_08_2020a_DW.Delay1_DSTATE_jp,
        &Template_v3_08_2020a_B.sf_CorrectDropouts);

      // DataStoreWrite: '<S206>/Data Store Write'
      Template_v3_08_2020a_DW.RED_Rz_Shoulder =
        Template_v3_08_2020a_B.sf_CorrectDropouts.FixedInput;

      // MATLAB Function: '<S209>/Correct Dropouts' incorporates:
      //   Delay: '<S209>/Delay1'
      //   Delay: '<S209>/Delay2'
      //   Delay: '<S209>/Delay3'
      //   Delay: '<S209>/Delay4'
      //   Delay: '<S209>/Delay5'
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_CorrectDropouts(Template_v3_08_2020a_B.Subtract_o,
        Template_v3_08_2020a_DW.Delay1_DSTATE_n5,
        &Template_v3_08_2020a_B.sf_CorrectDropouts_b);

      // DataStoreWrite: '<S206>/Data Store Write1'
      Template_v3_08_2020a_DW.RED_Rz_Elbow =
        Template_v3_08_2020a_B.sf_CorrectDropouts_b.FixedInput;

      // MATLAB Function: '<S210>/Correct Dropouts' incorporates:
      //   Delay: '<S210>/Delay1'
      //   Delay: '<S210>/Delay2'
      //   Delay: '<S210>/Delay3'
      //   Delay: '<S210>/Delay4'
      //   Delay: '<S210>/Delay5'
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_CorrectDropouts(Template_v3_08_2020a_B.Sum6_l,
        Template_v3_08_2020a_DW.Delay1_DSTATE_ny,
        &Template_v3_08_2020a_B.sf_CorrectDropouts_h);

      // DataStoreWrite: '<S206>/Data Store Write2'
      Template_v3_08_2020a_DW.RED_Rz_Wrist =
        Template_v3_08_2020a_B.sf_CorrectDropouts_h.FixedInput;

      // Update for Delay: '<S208>/Delay1' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_DW.Delay1_DSTATE_jp =
        Template_v3_08_2020a_B.sampleTime;

      // Update for Delay: '<S208>/Delay2' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_DW.Delay2_DSTATE[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay2_DSTATE[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay2_DSTATE[1] =
        Template_v3_08_2020a_B.sampleTime;

      // End of Update for Delay: '<S208>/Delay2'

      // Update for Delay: '<S208>/Delay5' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 2) {
        Template_v3_08_2020a_DW.Delay5_DSTATE[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay5_DSTATE[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay5_DSTATE[2] =
        Template_v3_08_2020a_B.sampleTime;

      // End of Update for Delay: '<S208>/Delay5'

      // Update for Delay: '<S208>/Delay3' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 3) {
        Template_v3_08_2020a_DW.Delay3_DSTATE[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay3_DSTATE[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay3_DSTATE[3] =
        Template_v3_08_2020a_B.sampleTime;

      // End of Update for Delay: '<S208>/Delay3'

      // Update for Delay: '<S208>/Delay4' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 4) {
        Template_v3_08_2020a_DW.Delay4_DSTATE[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay4_DSTATE[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay4_DSTATE[4] =
        Template_v3_08_2020a_B.sampleTime;

      // End of Update for Delay: '<S208>/Delay4'

      // Update for Delay: '<S209>/Delay1' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_DW.Delay1_DSTATE_n5 =
        Template_v3_08_2020a_B.Subtract_o;

      // Update for Delay: '<S209>/Delay2' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_DW.Delay2_DSTATE_l[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay2_DSTATE_l[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay2_DSTATE_l[1] =
        Template_v3_08_2020a_B.Subtract_o;

      // End of Update for Delay: '<S209>/Delay2'

      // Update for Delay: '<S209>/Delay5' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 2) {
        Template_v3_08_2020a_DW.Delay5_DSTATE_b[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay5_DSTATE_b[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay5_DSTATE_b[2] =
        Template_v3_08_2020a_B.Subtract_o;

      // End of Update for Delay: '<S209>/Delay5'

      // Update for Delay: '<S209>/Delay3' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 3) {
        Template_v3_08_2020a_DW.Delay3_DSTATE_o[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay3_DSTATE_o[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay3_DSTATE_o[3] =
        Template_v3_08_2020a_B.Subtract_o;

      // End of Update for Delay: '<S209>/Delay3'

      // Update for Delay: '<S209>/Delay4' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 4) {
        Template_v3_08_2020a_DW.Delay4_DSTATE_c[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay4_DSTATE_c[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay4_DSTATE_c[4] =
        Template_v3_08_2020a_B.Subtract_o;

      // End of Update for Delay: '<S209>/Delay4'

      // Update for Delay: '<S210>/Delay1' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_DW.Delay1_DSTATE_ny = Template_v3_08_2020a_B.Sum6_l;

      // Update for Delay: '<S210>/Delay2' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 1) {
        Template_v3_08_2020a_DW.Delay2_DSTATE_m[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay2_DSTATE_m[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay2_DSTATE_m[1] = Template_v3_08_2020a_B.Sum6_l;

      // End of Update for Delay: '<S210>/Delay2'

      // Update for Delay: '<S210>/Delay5' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 2) {
        Template_v3_08_2020a_DW.Delay5_DSTATE_n[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay5_DSTATE_n[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay5_DSTATE_n[2] = Template_v3_08_2020a_B.Sum6_l;

      // End of Update for Delay: '<S210>/Delay5'

      // Update for Delay: '<S210>/Delay3' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 3) {
        Template_v3_08_2020a_DW.Delay3_DSTATE_h[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay3_DSTATE_h[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay3_DSTATE_h[3] = Template_v3_08_2020a_B.Sum6_l;

      // End of Update for Delay: '<S210>/Delay3'

      // Update for Delay: '<S210>/Delay4' incorporates:
      //   MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders'

      Template_v3_08_2020a_B.i = 0;
      while (Template_v3_08_2020a_B.i < 4) {
        Template_v3_08_2020a_DW.Delay4_DSTATE_k[Template_v3_08_2020a_B.i] =
          Template_v3_08_2020a_DW.Delay4_DSTATE_k[Template_v3_08_2020a_B.i + 1];
        Template_v3_08_2020a_B.i++;
      }

      Template_v3_08_2020a_DW.Delay4_DSTATE_k[4] = Template_v3_08_2020a_B.Sum6_l;

      // End of Update for Delay: '<S210>/Delay4'
      // End of Outputs for SubSystem: '<S8>/Change Behavior'
    }

    // End of If: '<S8>/This IF block determines whether or not to run the exp code' 

    // Clock: '<S18>/Set Universal Time (If this is a simulation)'
    Template_v3_08_2020a_B.SetUniversalTimeIfthisisasimula =
      Template_v3_08_2020a_M->Timing.t[0];
  }

  // Matfile logging
  rt_UpdateTXYLogVars(Template_v3_08_2020a_M->rtwLogInfo,
                      (Template_v3_08_2020a_M->Timing.t));

  {
    char_T *sErr;

    // Update for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    if (Template_v3_08_2020a_DW.Ifperforminganexperimentgrabthe == 0) {
      // Update for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' incorporates:
      //   ActionPort: '<S78>/Action Port'

      // Update for Delay: '<S88>/Delay1'
      Template_v3_08_2020a_DW.icLoad_ct = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_hi =
        Template_v3_08_2020a_B.UDPReceive_o1[4];

      // Update for Delay: '<S80>/Delay1'
      Template_v3_08_2020a_DW.icLoad_h0 = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_df =
        Template_v3_08_2020a_B.dividebydeltaT;

      // Update for Delay: '<S89>/Delay1'
      Template_v3_08_2020a_DW.icLoad_kt = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_ft =
        Template_v3_08_2020a_B.UDPReceive_o1[5];

      // Update for Delay: '<S81>/Delay1'
      Template_v3_08_2020a_DW.icLoad_ktm = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_hz =
        Template_v3_08_2020a_B.dividebydeltaT_k;

      // Update for Delay: '<S90>/Delay1'
      Template_v3_08_2020a_DW.icLoad_gn = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_mt = Template_v3_08_2020a_B.Unwrap1;

      // Update for Delay: '<S82>/Delay1'
      Template_v3_08_2020a_DW.icLoad_pz = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_lf =
        Template_v3_08_2020a_B.dividebydeltaT_d;

      // Update for Delay: '<S83>/Delay1'
      Template_v3_08_2020a_DW.icLoad_fy = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_cs =
        Template_v3_08_2020a_B.UDPReceive_o1[13];

      // Update for Delay: '<S79>/Delay1'
      Template_v3_08_2020a_DW.icLoad_ea = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_kv =
        Template_v3_08_2020a_B.UDPReceive_o1[1];

      // Update for Delay: '<S91>/Delay1'
      Template_v3_08_2020a_DW.icLoad_df = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_lc =
        Template_v3_08_2020a_B.dividebydeltaT_j;

      // Update for Delay: '<S86>/Delay1'
      Template_v3_08_2020a_DW.icLoad_pj = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_cd =
        Template_v3_08_2020a_B.UDPReceive_o1[2];

      // Update for Delay: '<S92>/Delay1'
      Template_v3_08_2020a_DW.icLoad_ap = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_i0 =
        Template_v3_08_2020a_B.dividebydeltaT_d2;

      // Update for Delay: '<S87>/Delay1'
      Template_v3_08_2020a_DW.icLoad_hr = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_jx = Template_v3_08_2020a_B.Unwrap;

      // Update for Delay: '<S93>/Delay1'
      Template_v3_08_2020a_DW.icLoad_al = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_ml =
        Template_v3_08_2020a_B.dividebydeltaT_l;

      // Update for Delay: '<S84>/Delay1'
      Template_v3_08_2020a_DW.icLoad_dt = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_ic =
        Template_v3_08_2020a_B.UDPReceive_o1[14];

      // Update for Delay: '<S85>/Delay1'
      Template_v3_08_2020a_DW.icLoad_l1b = 0U;
      Template_v3_08_2020a_DW.Delay1_DSTATE_a2 = Template_v3_08_2020a_B.Unwrap2;

      // End of Update for SubSystem: '<S5>/Use Hardware to Obtain States'
    }

    // End of Update for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 

    // Update for S-Function (sdspToNetwork): '<S18>/UDP Send'
    sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
    LibUpdate_Network(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U],
                      &Template_v3_08_2020a_B.SetUniversalTimeIfthisisasimula, 1);
    if (*sErr != 0) {
      rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
      rtmSetStopRequested(Template_v3_08_2020a_M, 1);
    }

    // End of Update for S-Function (sdspToNetwork): '<S18>/UDP Send'
  }

  // signal main to stop simulation
  {                                    // Sample time: [0.0s, 0.0s]
    if ((rtmGetTFinal(Template_v3_08_2020a_M)!=-1) &&
        !((rtmGetTFinal(Template_v3_08_2020a_M)-Template_v3_08_2020a_M->
           Timing.t[0]) > Template_v3_08_2020a_M->Timing.t[0] * (DBL_EPSILON)))
    {
      rtmSetErrorStatus(Template_v3_08_2020a_M, "Simulation finished");
    }
  }

  // Update absolute time
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  Template_v3_08_2020a_M->Timing.t[0] =
    ((time_T)(++Template_v3_08_2020a_M->Timing.clockTick0)) *
    Template_v3_08_2020a_M->Timing.stepSize0;

  // Update absolute time
  // The "clockTick1" counts the number of times the code of this task has
  //  been executed. The resolution of this integer timer is 0.05, which is the step size
  //  of the task. Size of "clockTick1" ensures timer will not overflow during the
  //  application lifespan selected.

  Template_v3_08_2020a_M->Timing.clockTick1++;
}

// Model step function for TID2
void Template_v3_08_2020a_step2(void)  // Sample time: [0.1s, 0.0s]
{
  real_T sampleTime;

  // MATLABSystem: '<S17>/WhoAmI' incorporates:
  //   DataStoreWrite: '<S17>/Data Store Write'

  if (Template_v3_08_2020a_DW.obj_b.SampleTime !=
      Template_v3_08_2020a_P.WhoAmI_SampleTime) {
    if (((!rtIsInf(Template_v3_08_2020a_P.WhoAmI_SampleTime)) && (!rtIsNaN
          (Template_v3_08_2020a_P.WhoAmI_SampleTime))) || rtIsInf
        (Template_v3_08_2020a_P.WhoAmI_SampleTime)) {
      sampleTime = Template_v3_08_2020a_P.WhoAmI_SampleTime;
    }

    Template_v3_08_2020a_DW.obj_b.SampleTime = sampleTime;
  }

  Template_v3_08_2020a_DW.WhoAmI = whoAmI();

  // End of MATLABSystem: '<S17>/WhoAmI'

  // Outputs for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks'
  // MATLABSystem: '<S33>/RED T1 - BLACK T8' incorporates:
  //   RateTransition: '<S4>/Rate Transition'

  if (Template_v3_08_2020a_DW.RateTransition_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_m.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T1 - BLACK T8'

  // MATLABSystem: '<S33>/RED T2 - BLACK T3' incorporates:
  //   RateTransition: '<S4>/Rate Transition1'

  if (Template_v3_08_2020a_DW.RateTransition1_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition1_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_ma.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T2 - BLACK T3'

  // MATLABSystem: '<S33>/RED T3' incorporates:
  //   RateTransition: '<S4>/Rate Transition2'

  if (Template_v3_08_2020a_DW.RateTransition2_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition2_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_h.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T3'

  // MATLABSystem: '<S33>/RED T4 - BLACK T5' incorporates:
  //   RateTransition: '<S4>/Rate Transition3'

  if (Template_v3_08_2020a_DW.RateTransition3_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition3_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_k.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T4 - BLACK T5'

  // MATLABSystem: '<S33>/RED T5 - BLACK T4' incorporates:
  //   RateTransition: '<S4>/Rate Transition4'

  if (Template_v3_08_2020a_DW.RateTransition4_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition4_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_hw.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T5 - BLACK T4'

  // MATLABSystem: '<S33>/RED T6 - BLACK T7' incorporates:
  //   RateTransition: '<S4>/Rate Transition5'

  if (Template_v3_08_2020a_DW.RateTransition5_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition5_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_hf.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T6 - BLACK T7'

  // MATLABSystem: '<S33>/RED T7 - BLACK T6' incorporates:
  //   RateTransition: '<S4>/Rate Transition6'

  if (Template_v3_08_2020a_DW.RateTransition6_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition6_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_ce.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T7 - BLACK T6'

  // MATLABSystem: '<S33>/RED T8 - BLACK T1' incorporates:
  //   RateTransition: '<S4>/Rate Transition7'

  if (Template_v3_08_2020a_DW.RateTransition7_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition7_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_f.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/RED T8 - BLACK T1'

  // MATLABSystem: '<S33>/BLACK T2' incorporates:
  //   RateTransition: '<S4>/Rate Transition8'

  if (Template_v3_08_2020a_DW.RateTransition8_Buffer < 1.0) {
    sampleTime = Template_v3_08_2020a_DW.RateTransition8_Buffer;
  } else {
    sampleTime = 1.0;
  }

  if (!(sampleTime > 0.0)) {
    sampleTime = 0.0;
  }

  EXT_PWMBlock_setDutyCycle(Template_v3_08_2020a_DW.obj_ci.PinNumber, sampleTime);

  // End of MATLABSystem: '<S33>/BLACK T2'
  // End of Outputs for SubSystem: '<S4>/Send Commands to PWM Blocks'
}

// Model step wrapper function for compatibility with a static main program
void Template_v3_08_2020a_step(int_T tid)
{
  switch (tid) {
   case 0 :
    Template_v3_08_2020a_step0();
    break;

   case 2 :
    Template_v3_08_2020a_step2();
    break;

   default :
    break;
  }
}

// Model initialize function
void Template_v3_08_2020a_initialize(void)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&Template_v3_08_2020a_M->solverInfo,
                          &Template_v3_08_2020a_M->Timing.simTimeStep);
    rtsiSetTPtr(&Template_v3_08_2020a_M->solverInfo, &rtmGetTPtr
                (Template_v3_08_2020a_M));
    rtsiSetStepSizePtr(&Template_v3_08_2020a_M->solverInfo,
                       &Template_v3_08_2020a_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Template_v3_08_2020a_M->solverInfo,
                          (&rtmGetErrorStatus(Template_v3_08_2020a_M)));
    rtsiSetRTModelPtr(&Template_v3_08_2020a_M->solverInfo,
                      Template_v3_08_2020a_M);
  }

  rtsiSetSimTimeStep(&Template_v3_08_2020a_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Template_v3_08_2020a_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Template_v3_08_2020a_M, &Template_v3_08_2020a_M->Timing.tArray[0]);
  rtmSetTFinal(Template_v3_08_2020a_M, 690.0);
  Template_v3_08_2020a_M->Timing.stepSize0 = 0.05;

  // Setup for data logging
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    Template_v3_08_2020a_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  // Setup for data logging
  {
    rtliSetLogXSignalInfo(Template_v3_08_2020a_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Template_v3_08_2020a_M->rtwLogInfo, (NULL));
    rtliSetLogT(Template_v3_08_2020a_M->rtwLogInfo, "tout");
    rtliSetLogX(Template_v3_08_2020a_M->rtwLogInfo, "");
    rtliSetLogXFinal(Template_v3_08_2020a_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Template_v3_08_2020a_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Template_v3_08_2020a_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Template_v3_08_2020a_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Template_v3_08_2020a_M->rtwLogInfo, 1);
    rtliSetLogY(Template_v3_08_2020a_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Template_v3_08_2020a_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Template_v3_08_2020a_M->rtwLogInfo, (NULL));
  }

  // Matfile logging
  rt_StartDataLoggingWithStartTime(Template_v3_08_2020a_M->rtwLogInfo, 0.0,
    rtmGetTFinal(Template_v3_08_2020a_M),
    Template_v3_08_2020a_M->Timing.stepSize0, (&rtmGetErrorStatus
    (Template_v3_08_2020a_M)));

  {
    char_T *sErr;
    uint32_T tseed;
    int32_T t;
    int32_T i;
    static const char_T tmp[45] =
      "Unable to configure pin %u for PWM output.\\n";

    // SetupRuntimeResources for ToWorkspace: '<S1>/To Workspace'
    {
      int_T dimensions[1] = { 73 };

      Template_v3_08_2020a_DW.ToWorkspace_PWORK.LoggedData = rt_CreateLogVar(
        Template_v3_08_2020a_M->rtwLogInfo,
        0.0,
        rtmGetTFinal(Template_v3_08_2020a_M),
        Template_v3_08_2020a_M->Timing.stepSize0,
        (&rtmGetErrorStatus(Template_v3_08_2020a_M)),
        "dataPacket",
        SS_DOUBLE,
        0,
        0,
        0,
        73,
        1,
        dimensions,
        NO_LOGVALDIMS,
        (NULL),
        (NULL),
        0,
        1,
        0.05,
        1);
      if (Template_v3_08_2020a_DW.ToWorkspace_PWORK.LoggedData == (NULL))
        return;
    }

    // Start for If: '<S5>/If performing an experiment, grab the PhaseSpace data. Otherwise, use a clock to set time in SIM.' 
    Template_v3_08_2020a_DW.Ifperforminganexperimentgrabthe = -1;

    // Start for S-Function (sdspToNetwork): '<S18>/UDP Send'
    sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
    CreateUDPInterface(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U], 1,
                        "0.0.0.0", -1, "192.168.0.105",
                        Template_v3_08_2020a_P.UDPSend_Port, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
        rtmSetStopRequested(Template_v3_08_2020a_M, 1);
      }
    }

    // End of Start for S-Function (sdspToNetwork): '<S18>/UDP Send'

    // Start for DataStoreMemory: '<Root>/BLACK_Fx'
    Template_v3_08_2020a_DW.BLACK_Fx =
      Template_v3_08_2020a_P.BLACK_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx1'
    Template_v3_08_2020a_DW.BLUE_Fx =
      Template_v3_08_2020a_P.BLACK_Fx1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat'
    Template_v3_08_2020a_DW.BLACK_Fx_Sat =
      Template_v3_08_2020a_P.BLACK_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat1'
    Template_v3_08_2020a_DW.ARM_Elbow_Py =
      Template_v3_08_2020a_P.BLACK_Fx_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fx_Sat2'
    Template_v3_08_2020a_DW.BLUE_Fx_Sat =
      Template_v3_08_2020a_P.BLACK_Fx_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy'
    Template_v3_08_2020a_DW.BLACK_Fy =
      Template_v3_08_2020a_P.BLACK_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy1'
    Template_v3_08_2020a_DW.BLUE_Fy =
      Template_v3_08_2020a_P.BLACK_Fy1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat'
    Template_v3_08_2020a_DW.BLACK_Fy_Sat =
      Template_v3_08_2020a_P.BLACK_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat1'
    Template_v3_08_2020a_DW.ARM_Wrist_Px =
      Template_v3_08_2020a_P.BLACK_Fy_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Fy_Sat2'
    Template_v3_08_2020a_DW.BLUE_Fy_Sat =
      Template_v3_08_2020a_P.BLACK_Fy_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px'
    Template_v3_08_2020a_DW.BLACK_Px =
      Template_v3_08_2020a_P.BLACK_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Px1'
    Template_v3_08_2020a_DW.BLUE_Px =
      Template_v3_08_2020a_P.BLACK_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py'
    Template_v3_08_2020a_DW.BLACK_Py =
      Template_v3_08_2020a_P.BLACK_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Py1'
    Template_v3_08_2020a_DW.BLUE_Py =
      Template_v3_08_2020a_P.BLACK_Py1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz'
    Template_v3_08_2020a_DW.BLACK_Rz =
      Template_v3_08_2020a_P.BLACK_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Rz1'
    Template_v3_08_2020a_DW.BLUE_Rz =
      Template_v3_08_2020a_P.BLACK_Rz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz'
    Template_v3_08_2020a_DW.BLACK_Tz =
      Template_v3_08_2020a_P.BLACK_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz1'
    Template_v3_08_2020a_DW.BLUE_Tz =
      Template_v3_08_2020a_P.BLACK_Tz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat'
    Template_v3_08_2020a_DW.BLACK_Tz_Sat =
      Template_v3_08_2020a_P.BLACK_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat1'
    Template_v3_08_2020a_DW.ARM_Wrist_Py =
      Template_v3_08_2020a_P.BLACK_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/BLACK_Tz_Sat2'
    Template_v3_08_2020a_DW.BLUE_Tz_Sat =
      Template_v3_08_2020a_P.BLACK_Tz_Sat2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx'
    Template_v3_08_2020a_DW.RED_Fx = Template_v3_08_2020a_P.RED_Fx_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fx_Sat'
    Template_v3_08_2020a_DW.RED_Fx_Sat =
      Template_v3_08_2020a_P.RED_Fx_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy'
    Template_v3_08_2020a_DW.RED_Fy = Template_v3_08_2020a_P.RED_Fy_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Fy_Sat'
    Template_v3_08_2020a_DW.RED_Fy_Sat =
      Template_v3_08_2020a_P.RED_Fy_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px'
    Template_v3_08_2020a_DW.RED_Px = Template_v3_08_2020a_P.RED_Px_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px1'
    Template_v3_08_2020a_DW.RED_AHRS_Q =
      Template_v3_08_2020a_P.RED_Px1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px10'
    Template_v3_08_2020a_DW.BLACK_IMU_Ax_b =
      Template_v3_08_2020a_P.RED_Px10_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px11'
    Template_v3_08_2020a_DW.BLACK_IMU_Ay_b =
      Template_v3_08_2020a_P.RED_Px11_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px12'
    Template_v3_08_2020a_DW.BLACK_IMU_Az_b =
      Template_v3_08_2020a_P.RED_Px12_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px19'
    Template_v3_08_2020a_DW.RED_IMU_Q =
      Template_v3_08_2020a_P.RED_Px19_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px2'
    Template_v3_08_2020a_DW.RED_AHRS_P =
      Template_v3_08_2020a_P.RED_Px2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px20'
    Template_v3_08_2020a_DW.RED_IMU_P =
      Template_v3_08_2020a_P.RED_Px20_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px21'
    Template_v3_08_2020a_DW.RED_IMU_R =
      Template_v3_08_2020a_P.RED_Px21_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px22'
    Template_v3_08_2020a_DW.BLACK_IMU_Q =
      Template_v3_08_2020a_P.RED_Px22_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px23'
    Template_v3_08_2020a_DW.BLACK_IMU_P =
      Template_v3_08_2020a_P.RED_Px23_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px24'
    Template_v3_08_2020a_DW.BLACK_IMU_R =
      Template_v3_08_2020a_P.RED_Px24_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px25'
    Template_v3_08_2020a_DW.RED_IMU_Psi =
      Template_v3_08_2020a_P.RED_Px25_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px26'
    Template_v3_08_2020a_DW.BLACK_IMU_Psi =
      Template_v3_08_2020a_P.RED_Px26_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px3'
    Template_v3_08_2020a_DW.RED_AHRS_R =
      Template_v3_08_2020a_P.RED_Px3_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px31'
    Template_v3_08_2020a_DW.RED_IMU_Ax_I =
      Template_v3_08_2020a_P.RED_Px31_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px32'
    Template_v3_08_2020a_DW.RED_IMU_Ay_I =
      Template_v3_08_2020a_P.RED_Px32_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px37'
    Template_v3_08_2020a_DW.BLACK_IMU_Ax_I =
      Template_v3_08_2020a_P.RED_Px37_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px38'
    Template_v3_08_2020a_DW.BLACK_IMU_Ay_I =
      Template_v3_08_2020a_P.RED_Px38_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px4'
    Template_v3_08_2020a_DW.BLACK_AHRS_Q =
      Template_v3_08_2020a_P.RED_Px4_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px5'
    Template_v3_08_2020a_DW.BLACK_AHRS_P =
      Template_v3_08_2020a_P.RED_Px5_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px6'
    Template_v3_08_2020a_DW.BLACK_AHRS_R =
      Template_v3_08_2020a_P.RED_Px6_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px7'
    Template_v3_08_2020a_DW.RED_IMU_Ax_b =
      Template_v3_08_2020a_P.RED_Px7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px8'
    Template_v3_08_2020a_DW.RED_IMU_Ay_b =
      Template_v3_08_2020a_P.RED_Px8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Px9'
    Template_v3_08_2020a_DW.RED_IMU_Az_b =
      Template_v3_08_2020a_P.RED_Px9_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Py'
    Template_v3_08_2020a_DW.RED_Py = Template_v3_08_2020a_P.RED_Py_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Rz'
    Template_v3_08_2020a_DW.RED_Rz = Template_v3_08_2020a_P.RED_Rz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz'
    Template_v3_08_2020a_DW.RED_Tz = Template_v3_08_2020a_P.RED_Tz_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz1'
    Template_v3_08_2020a_DW.RED_Tz_Shoulder =
      Template_v3_08_2020a_P.RED_Tz1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz12'
    Template_v3_08_2020a_DW.RED_RzD_Shoulder =
      Template_v3_08_2020a_P.RED_Tz12_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz13'
    Template_v3_08_2020a_DW.RED_RzD_Elbow =
      Template_v3_08_2020a_P.RED_Tz13_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz14'
    Template_v3_08_2020a_DW.RED_RzD_Wrist =
      Template_v3_08_2020a_P.RED_Tz14_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz2'
    Template_v3_08_2020a_DW.RED_Tz_Elbow =
      Template_v3_08_2020a_P.RED_Tz2_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz3'
    Template_v3_08_2020a_DW.RED_Tz_Wrist =
      Template_v3_08_2020a_P.RED_Tz3_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz4'
    Template_v3_08_2020a_DW.RED_Rz_Shoulder =
      Template_v3_08_2020a_P.RED_Tz4_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz5'
    Template_v3_08_2020a_DW.RED_Rz_Elbow =
      Template_v3_08_2020a_P.RED_Tz5_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz6'
    Template_v3_08_2020a_DW.RED_Rz_Wrist =
      Template_v3_08_2020a_P.RED_Tz6_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz7'
    Template_v3_08_2020a_DW.Float_State =
      Template_v3_08_2020a_P.RED_Tz7_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz8'
    Template_v3_08_2020a_DW.Magnet_State =
      Template_v3_08_2020a_P.RED_Tz8_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat'
    Template_v3_08_2020a_DW.RED_Tz_RW_Sat =
      Template_v3_08_2020a_P.RED_Tz_RWSat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW Sat1'
    Template_v3_08_2020a_DW.ARM_EndEff_Py =
      Template_v3_08_2020a_P.RED_Tz_RWSat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_RW1'
    Template_v3_08_2020a_DW.ARM_EndEff_Px =
      Template_v3_08_2020a_P.RED_Tz_RW1_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat'
    Template_v3_08_2020a_DW.RED_Tz_Sat =
      Template_v3_08_2020a_P.RED_Tz_Sat_InitialValue;

    // Start for DataStoreMemory: '<Root>/RED_Tz_Sat1'
    Template_v3_08_2020a_DW.ARM_Elbow_Px =
      Template_v3_08_2020a_P.RED_Tz_Sat1_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time'
    Template_v3_08_2020a_DW.Univ_Time =
      Template_v3_08_2020a_P.Universal_Time_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time1'
    Template_v3_08_2020a_DW.RED_Vx =
      Template_v3_08_2020a_P.Universal_Time1_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time10'
    Template_v3_08_2020a_DW.RED_Ax =
      Template_v3_08_2020a_P.Universal_Time10_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time11'
    Template_v3_08_2020a_DW.RED_Ay =
      Template_v3_08_2020a_P.Universal_Time11_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time12'
    Template_v3_08_2020a_DW.BLACK_Ax =
      Template_v3_08_2020a_P.Universal_Time12_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time13'
    Template_v3_08_2020a_DW.BLACK_Ay =
      Template_v3_08_2020a_P.Universal_Time13_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time14'
    Template_v3_08_2020a_DW.BLACK_RzDD =
      Template_v3_08_2020a_P.Universal_Time14_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time15'
    Template_v3_08_2020a_DW.RED_RzDD =
      Template_v3_08_2020a_P.Universal_Time15_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time19'
    Template_v3_08_2020a_DW.WhoAmI =
      Template_v3_08_2020a_P.Universal_Time19_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time2'
    Template_v3_08_2020a_DW.RED_Vy =
      Template_v3_08_2020a_P.Universal_Time2_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time3'
    Template_v3_08_2020a_DW.RED_RzD =
      Template_v3_08_2020a_P.Universal_Time3_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time4'
    Template_v3_08_2020a_DW.BLACK_Vx =
      Template_v3_08_2020a_P.Universal_Time4_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time5'
    Template_v3_08_2020a_DW.BLACK_Vy =
      Template_v3_08_2020a_P.Universal_Time5_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time6'
    Template_v3_08_2020a_DW.BLACK_RzD =
      Template_v3_08_2020a_P.Universal_Time6_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time7'
    Template_v3_08_2020a_DW.BLUE_Vx =
      Template_v3_08_2020a_P.Universal_Time7_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time8'
    Template_v3_08_2020a_DW.BLUE_Vy =
      Template_v3_08_2020a_P.Universal_Time8_InitialValue;

    // Start for DataStoreMemory: '<Root>/Universal_Time9'
    Template_v3_08_2020a_DW.BLUE_RzD =
      Template_v3_08_2020a_P.Universal_Time9_InitialValue;

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 
    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S236>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ns = 1U;

    // InitializeConditions for Delay: '<S238>/Delay1'
    Template_v3_08_2020a_DW.icLoad_a = 1U;

    // InitializeConditions for Delay: '<S234>/Delay1'
    Template_v3_08_2020a_DW.icLoad_h = 1U;

    // SystemInitialize for IfAction SubSystem: '<S236>/Hold this value'
    // SystemInitialize for Outport: '<S237>/Out1'
    Template_v3_08_2020a_B.In1_ej = Template_v3_08_2020a_P.Out1_Y0_av;

    // End of SystemInitialize for SubSystem: '<S236>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S238>/Hold this value'
    // SystemInitialize for Outport: '<S239>/Out1'
    Template_v3_08_2020a_B.In1_g = Template_v3_08_2020a_P.Out1_Y0_h4;

    // End of SystemInitialize for SubSystem: '<S238>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S234>/Hold this value'
    // SystemInitialize for Outport: '<S235>/Out1'
    Template_v3_08_2020a_B.In1_ds = Template_v3_08_2020a_P.Out1_Y0_aw;

    // End of SystemInitialize for SubSystem: '<S234>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S12>/Change BLUE Behavior'
    // InitializeConditions for Delay: '<S249>/Delay1'
    Template_v3_08_2020a_DW.icLoad_dy = 1U;

    // InitializeConditions for Delay: '<S251>/Delay1'
    Template_v3_08_2020a_DW.icLoad_n5 = 1U;

    // InitializeConditions for Delay: '<S247>/Delay1'
    Template_v3_08_2020a_DW.icLoad_bp = 1U;

    // SystemInitialize for IfAction SubSystem: '<S249>/Hold this value'
    // SystemInitialize for Outport: '<S250>/Out1'
    Template_v3_08_2020a_B.In1_l = Template_v3_08_2020a_P.Out1_Y0_a4;

    // End of SystemInitialize for SubSystem: '<S249>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S251>/Hold this value'
    // SystemInitialize for Outport: '<S252>/Out1'
    Template_v3_08_2020a_B.In1_f1 = Template_v3_08_2020a_P.Out1_Y0_kw;

    // End of SystemInitialize for SubSystem: '<S251>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S247>/Hold this value'
    // SystemInitialize for Outport: '<S248>/Out1'
    Template_v3_08_2020a_B.In1_fj = Template_v3_08_2020a_P.Out1_Y0_mf;

    // End of SystemInitialize for SubSystem: '<S247>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S12>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S12>/Change RED Behavior'
    // InitializeConditions for Delay: '<S262>/Delay1'
    Template_v3_08_2020a_DW.icLoad_jx = 1U;

    // InitializeConditions for Delay: '<S264>/Delay1'
    Template_v3_08_2020a_DW.icLoad_l0 = 1U;

    // InitializeConditions for Delay: '<S266>/Delay1'
    Template_v3_08_2020a_DW.icLoad_iu = 1U;

    // SystemInitialize for IfAction SubSystem: '<S262>/Hold this value'
    // SystemInitialize for Outport: '<S263>/Out1'
    Template_v3_08_2020a_B.In1_i5 = Template_v3_08_2020a_P.Out1_Y0_pj;

    // End of SystemInitialize for SubSystem: '<S262>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S264>/Hold this value'
    // SystemInitialize for Outport: '<S265>/Out1'
    Template_v3_08_2020a_B.In1_jv = Template_v3_08_2020a_P.Out1_Y0_lk;

    // End of SystemInitialize for SubSystem: '<S264>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S266>/Hold this value'
    // SystemInitialize for Outport: '<S267>/Out1'
    Template_v3_08_2020a_B.In1_jh = Template_v3_08_2020a_P.Out1_Y0_fa;

    // End of SystemInitialize for SubSystem: '<S266>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S228>/RED+ARM'
    Template_v3_08__REDARM_Init(&Template_v3_08_2020a_B.REDARM_j,
      &Template_v3_08_2020a_DW.REDARM_j, &Template_v3_08_2020a_P.REDARM_j);

    // End of SystemInitialize for SubSystem: '<S228>/RED+ARM'
    // End of SystemInitialize for SubSystem: '<S12>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #2:  Move to  Initial Position' 

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #3: Experiment'
    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLACK Behavior'
    // SystemInitialize for IfAction SubSystem: '<S280>/Sub-Phase #1'
    Template_v3__SubPhase1_Init(&Template_v3_08_2020a_B.SubPhase1,
      &Template_v3_08_2020a_DW.SubPhase1, &Template_v3_08_2020a_P.SubPhase1);

    // End of SystemInitialize for SubSystem: '<S280>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S280>/Sub-Phase #2 '
    Template_v3__SubPhase1_Init(&Template_v3_08_2020a_B.SubPhase2,
      &Template_v3_08_2020a_DW.SubPhase2, &Template_v3_08_2020a_P.SubPhase2);

    // End of SystemInitialize for SubSystem: '<S280>/Sub-Phase #2 '

    // SystemInitialize for IfAction SubSystem: '<S280>/Sub-Phase #3 '
    Template_v3__SubPhase1_Init(&Template_v3_08_2020a_B.SubPhase3,
      &Template_v3_08_2020a_DW.SubPhase3, &Template_v3_08_2020a_P.SubPhase3);

    // End of SystemInitialize for SubSystem: '<S280>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S280>/Sub-Phase #4'
    Template_v3__SubPhase1_Init(&Template_v3_08_2020a_B.SubPhase4,
      &Template_v3_08_2020a_DW.SubPhase4, &Template_v3_08_2020a_P.SubPhase4);

    // End of SystemInitialize for SubSystem: '<S280>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change BLUE Behavior'
    // SystemInitialize for IfAction SubSystem: '<S281>/Sub-Phase #1'
    Template_v_SubPhase1_a_Init(&Template_v3_08_2020a_B.SubPhase1_a,
      &Template_v3_08_2020a_DW.SubPhase1_a, &Template_v3_08_2020a_P.SubPhase1_a);

    // End of SystemInitialize for SubSystem: '<S281>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S281>/Sub-Phase #2 '
    Template_v_SubPhase1_a_Init(&Template_v3_08_2020a_B.SubPhase2_m,
      &Template_v3_08_2020a_DW.SubPhase2_m, &Template_v3_08_2020a_P.SubPhase2_m);

    // End of SystemInitialize for SubSystem: '<S281>/Sub-Phase #2 '

    // SystemInitialize for IfAction SubSystem: '<S281>/Sub-Phase #3 '
    Template_v_SubPhase1_a_Init(&Template_v3_08_2020a_B.SubPhase3_p,
      &Template_v3_08_2020a_DW.SubPhase3_p, &Template_v3_08_2020a_P.SubPhase3_p);

    // End of SystemInitialize for SubSystem: '<S281>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S281>/Sub-Phase #4'
    Template_v_SubPhase1_a_Init(&Template_v3_08_2020a_B.SubPhase4_f,
      &Template_v3_08_2020a_DW.SubPhase4_f, &Template_v3_08_2020a_P.SubPhase4_f);

    // End of SystemInitialize for SubSystem: '<S281>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S13>/Change RED Behavior'
    // SystemInitialize for IfAction SubSystem: '<S282>/Sub-Phase #1'
    // InitializeConditions for Delay: '<S449>/Delay1'
    Template_v3_08_2020a_DW.icLoad_nw = 1U;

    // InitializeConditions for Delay: '<S451>/Delay1'
    Template_v3_08_2020a_DW.icLoad_gb = 1U;

    // InitializeConditions for Delay: '<S453>/Delay1'
    Template_v3_08_2020a_DW.icLoad_nx = 1U;

    // SystemInitialize for IfAction SubSystem: '<S434>/RED+ARM'
    Template_v3_08__REDARM_Init(&Template_v3_08_2020a_B.REDARM_c,
      &Template_v3_08_2020a_DW.REDARM_c, &Template_v3_08_2020a_P.REDARM_c);

    // End of SystemInitialize for SubSystem: '<S434>/RED+ARM'

    // SystemInitialize for IfAction SubSystem: '<S449>/Hold this value'
    // SystemInitialize for Outport: '<S450>/Out1'
    Template_v3_08_2020a_B.In1_k = Template_v3_08_2020a_P.Out1_Y0_mm;

    // End of SystemInitialize for SubSystem: '<S449>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S451>/Hold this value'
    // SystemInitialize for Outport: '<S452>/Out1'
    Template_v3_08_2020a_B.In1_pe = Template_v3_08_2020a_P.Out1_Y0_fjl;

    // End of SystemInitialize for SubSystem: '<S451>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S453>/Hold this value'
    // SystemInitialize for Outport: '<S454>/Out1'
    Template_v3_08_2020a_B.In1_ay = Template_v3_08_2020a_P.Out1_Y0_hd;

    // End of SystemInitialize for SubSystem: '<S453>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S282>/Sub-Phase #1'

    // SystemInitialize for IfAction SubSystem: '<S282>/Sub-Phase #2 '
    // InitializeConditions for Delay: '<S484>/Delay1'
    Template_v3_08_2020a_DW.icLoad_jr = 1U;

    // InitializeConditions for Delay: '<S486>/Delay1'
    Template_v3_08_2020a_DW.icLoad_cw = 1U;

    // InitializeConditions for Delay: '<S488>/Delay1'
    Template_v3_08_2020a_DW.icLoad_eu = 1U;

    // SystemInitialize for IfAction SubSystem: '<S484>/Hold this value'
    // SystemInitialize for Outport: '<S485>/Out1'
    Template_v3_08_2020a_B.In1_fl = Template_v3_08_2020a_P.Out1_Y0_b5;

    // End of SystemInitialize for SubSystem: '<S484>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S435>/RED+ARM'
    Template_v3_08__REDARM_Init(&Template_v3_08_2020a_B.REDARM_b,
      &Template_v3_08_2020a_DW.REDARM_b, &Template_v3_08_2020a_P.REDARM_b);

    // End of SystemInitialize for SubSystem: '<S435>/RED+ARM'

    // SystemInitialize for IfAction SubSystem: '<S486>/Hold this value'
    // SystemInitialize for Outport: '<S487>/Out1'
    Template_v3_08_2020a_B.In1_mg = Template_v3_08_2020a_P.Out1_Y0_lha;

    // End of SystemInitialize for SubSystem: '<S486>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S488>/Hold this value'
    // SystemInitialize for Outport: '<S489>/Out1'
    Template_v3_08_2020a_B.In1_oz = Template_v3_08_2020a_P.Out1_Y0_hh;

    // End of SystemInitialize for SubSystem: '<S488>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S282>/Sub-Phase #2 '

    // SystemInitialize for IfAction SubSystem: '<S282>/Sub-Phase #3 '
    // InitializeConditions for Delay: '<S510>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ca = 1U;

    // InitializeConditions for Delay: '<S512>/Delay1'
    Template_v3_08_2020a_DW.icLoad_n = 1U;

    // SystemInitialize for IfAction SubSystem: '<S436>/RED+ARM'
    // InitializeConditions for Delay: '<S517>/Delay1'
    Template_v3_08_2020a_DW.icLoad_im = 1U;

    // InitializeConditions for Delay: '<S519>/Delay1'
    Template_v3_08_2020a_DW.icLoad_eb = 1U;

    // InitializeConditions for Delay: '<S521>/Delay1'
    Template_v3_08_2020a_DW.icLoad_jf = 1U;

    // SystemInitialize for IfAction SubSystem: '<S517>/Hold this value'
    // SystemInitialize for Outport: '<S518>/Out1'
    Template_v3_08_2020a_B.In1_f = Template_v3_08_2020a_P.Out1_Y0_gm;

    // End of SystemInitialize for SubSystem: '<S517>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S519>/Hold this value'
    // SystemInitialize for Outport: '<S520>/Out1'
    Template_v3_08_2020a_B.In1_p4 = Template_v3_08_2020a_P.Out1_Y0_io;

    // End of SystemInitialize for SubSystem: '<S519>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S521>/Hold this value'
    // SystemInitialize for Outport: '<S522>/Out1'
    Template_v3_08_2020a_B.In1_m5 = Template_v3_08_2020a_P.Out1_Y0_dk;

    // End of SystemInitialize for SubSystem: '<S521>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S436>/RED+ARM'

    // SystemInitialize for IfAction SubSystem: '<S510>/Hold this value'
    // SystemInitialize for Outport: '<S511>/Out1'
    Template_v3_08_2020a_B.In1_n = Template_v3_08_2020a_P.Out1_Y0_mv;

    // End of SystemInitialize for SubSystem: '<S510>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S512>/Hold this value'
    // SystemInitialize for Outport: '<S513>/Out1'
    Template_v3_08_2020a_B.In1_oy = Template_v3_08_2020a_P.Out1_Y0_lz;

    // End of SystemInitialize for SubSystem: '<S512>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S282>/Sub-Phase #3 '

    // SystemInitialize for IfAction SubSystem: '<S282>/Sub-Phase #4'
    // InitializeConditions for Delay: '<S534>/Delay1'
    Template_v3_08_2020a_DW.icLoad_c = 1U;

    // InitializeConditions for Delay: '<S536>/Delay1'
    Template_v3_08_2020a_DW.icLoad_i = 1U;

    // InitializeConditions for Delay: '<S538>/Delay1'
    Template_v3_08_2020a_DW.icLoad_cg = 1U;

    // SystemInitialize for IfAction SubSystem: '<S534>/Hold this value'
    // SystemInitialize for Outport: '<S535>/Out1'
    Template_v3_08_2020a_B.In1_op = Template_v3_08_2020a_P.Out1_Y0_ej;

    // End of SystemInitialize for SubSystem: '<S534>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S536>/Hold this value'
    // SystemInitialize for Outport: '<S537>/Out1'
    Template_v3_08_2020a_B.In1_d = Template_v3_08_2020a_P.Out1_Y0_ma;

    // End of SystemInitialize for SubSystem: '<S536>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S538>/Hold this value'
    // SystemInitialize for Outport: '<S539>/Out1'
    Template_v3_08_2020a_B.In1_m = Template_v3_08_2020a_P.Out1_Y0_nps;

    // End of SystemInitialize for SubSystem: '<S538>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S282>/Sub-Phase #4'
    // End of SystemInitialize for SubSystem: '<S13>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #3: Experiment'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #4:  Return Home'
    // SystemInitialize for IfAction SubSystem: '<S14>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S562>/Delay1'
    Template_v3_08_2020a_DW.icLoad_e = 1U;

    // InitializeConditions for Delay: '<S564>/Delay1'
    Template_v3_08_2020a_DW.icLoad_fr = 1U;

    // InitializeConditions for Delay: '<S560>/Delay1'
    Template_v3_08_2020a_DW.icLoad_o = 1U;

    // SystemInitialize for IfAction SubSystem: '<S562>/Hold this value'
    // SystemInitialize for Outport: '<S563>/Out1'
    Template_v3_08_2020a_B.In1_b = Template_v3_08_2020a_P.Out1_Y0_ii;

    // End of SystemInitialize for SubSystem: '<S562>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S564>/Hold this value'
    // SystemInitialize for Outport: '<S565>/Out1'
    Template_v3_08_2020a_B.In1_o5 = Template_v3_08_2020a_P.Out1_Y0_hq;

    // End of SystemInitialize for SubSystem: '<S564>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S560>/Hold this value'
    // SystemInitialize for Outport: '<S561>/Out1'
    Template_v3_08_2020a_B.In1_ei = Template_v3_08_2020a_P.Out1_Y0_o;

    // End of SystemInitialize for SubSystem: '<S560>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S14>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S14>/Change BLUE Behavior'
    T_ChangeBLUEBehavior_c_Init(&Template_v3_08_2020a_B.ChangeBLUEBehavior_gj,
      &Template_v3_08_2020a_DW.ChangeBLUEBehavior_gj,
      &Template_v3_08_2020a_P.ChangeBLUEBehavior_gj);

    // End of SystemInitialize for SubSystem: '<S14>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S14>/Change RED Behavior'
    // InitializeConditions for Delay: '<S588>/Delay1'
    Template_v3_08_2020a_DW.icLoad_k = 1U;

    // InitializeConditions for Delay: '<S590>/Delay1'
    Template_v3_08_2020a_DW.icLoad_du = 1U;

    // InitializeConditions for Delay: '<S592>/Delay1'
    Template_v3_08_2020a_DW.icLoad_g = 1U;

    // SystemInitialize for IfAction SubSystem: '<S588>/Hold this value'
    // SystemInitialize for Outport: '<S589>/Out1'
    Template_v3_08_2020a_B.In1_o = Template_v3_08_2020a_P.Out1_Y0_lc;

    // End of SystemInitialize for SubSystem: '<S588>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S590>/Hold this value'
    // SystemInitialize for Outport: '<S591>/Out1'
    Template_v3_08_2020a_B.In1_c = Template_v3_08_2020a_P.Out1_Y0_of;

    // End of SystemInitialize for SubSystem: '<S590>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S592>/Hold this value'
    // SystemInitialize for Outport: '<S593>/Out1'
    Template_v3_08_2020a_B.In1_a = Template_v3_08_2020a_P.Out1_Y0_l2;

    // End of SystemInitialize for SubSystem: '<S592>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S14>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #4:  Return Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Phase #5:  Hold Home'
    // SystemInitialize for IfAction SubSystem: '<S15>/Change BLACK Behavior'
    // InitializeConditions for Delay: '<S615>/Delay1'
    Template_v3_08_2020a_DW.icLoad_d = 1U;

    // InitializeConditions for Delay: '<S617>/Delay1'
    Template_v3_08_2020a_DW.icLoad_f = 1U;

    // InitializeConditions for Delay: '<S613>/Delay1'
    Template_v3_08_2020a_DW.icLoad_b = 1U;

    // SystemInitialize for IfAction SubSystem: '<S615>/Hold this value'
    // SystemInitialize for Outport: '<S616>/Out1'
    Template_v3_08_2020a_B.In1_it = Template_v3_08_2020a_P.Out1_Y0_c2;

    // End of SystemInitialize for SubSystem: '<S615>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S617>/Hold this value'
    // SystemInitialize for Outport: '<S618>/Out1'
    Template_v3_08_2020a_B.In1_p = Template_v3_08_2020a_P.Out1_Y0_f2b;

    // End of SystemInitialize for SubSystem: '<S617>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S613>/Hold this value'
    // SystemInitialize for Outport: '<S614>/Out1'
    Template_v3_08_2020a_B.In1_j = Template_v3_08_2020a_P.Out1_Y0_by4;

    // End of SystemInitialize for SubSystem: '<S613>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S15>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S15>/Change BLUE Behavior'
    T_ChangeBLUEBehavior_c_Init(&Template_v3_08_2020a_B.ChangeBLUEBehavior_b,
      &Template_v3_08_2020a_DW.ChangeBLUEBehavior_b,
      &Template_v3_08_2020a_P.ChangeBLUEBehavior_b);

    // End of SystemInitialize for SubSystem: '<S15>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S15>/Change RED Behavior'
    // InitializeConditions for Delay: '<S641>/Delay1'
    Template_v3_08_2020a_DW.icLoad = 1U;

    // InitializeConditions for Delay: '<S643>/Delay1'
    Template_v3_08_2020a_DW.icLoad_l = 1U;

    // InitializeConditions for Delay: '<S645>/Delay1'
    Template_v3_08_2020a_DW.icLoad_j = 1U;

    // SystemInitialize for IfAction SubSystem: '<S641>/Hold this value'
    // SystemInitialize for Outport: '<S642>/Out1'
    Template_v3_08_2020a_B.In1_i = Template_v3_08_2020a_P.Out1_Y0_jf;

    // End of SystemInitialize for SubSystem: '<S641>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S643>/Hold this value'
    // SystemInitialize for Outport: '<S644>/Out1'
    Template_v3_08_2020a_B.In1_e = Template_v3_08_2020a_P.Out1_Y0_fn;

    // End of SystemInitialize for SubSystem: '<S643>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S645>/Hold this value'
    // SystemInitialize for Outport: '<S646>/Out1'
    Template_v3_08_2020a_B.In1 = Template_v3_08_2020a_P.Out1_Y0_a2;

    // End of SystemInitialize for SubSystem: '<S645>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S15>/Change RED Behavior'
    // End of SystemInitialize for SubSystem: '<Root>/Phase #5:  Hold Home'

    // SystemInitialize for IfAction SubSystem: '<Root>/Simulate Plant Dynamics' 
    // InitializeConditions for DiscreteIntegrator: '<S666>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[0] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S666>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[0] =
      Template_v3_08_2020a_P.drop_states_BLACK[0];

    // InitializeConditions for DiscreteIntegrator: '<S666>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[1] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S666>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[1] =
      Template_v3_08_2020a_P.drop_states_BLACK[1];

    // InitializeConditions for DiscreteIntegrator: '<S666>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE[2] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_i;

    // InitializeConditions for DiscreteIntegrator: '<S666>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE[2] =
      Template_v3_08_2020a_P.drop_states_BLACK[2];

    // InitializeConditions for RandomNumber: '<S666>/Random Number'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber_Seed_h);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed = tseed;
    Template_v3_08_2020a_DW.NextOutput = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed) * sqrt
      (Template_v3_08_2020a_P.noise_variance_BLACK) +
      Template_v3_08_2020a_P.RandomNumber_Mean_n;

    // End of InitializeConditions for RandomNumber: '<S666>/Random Number'

    // InitializeConditions for Delay: '<S671>/Delay1'
    Template_v3_08_2020a_DW.icLoad_bb = 1U;

    // InitializeConditions for Delay: '<S669>/Delay1'
    Template_v3_08_2020a_DW.icLoad_p = 1U;

    // InitializeConditions for RandomNumber: '<S662>/Random Number1'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber1_Seed_m);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_m = tseed;
    Template_v3_08_2020a_DW.NextOutput_p = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_m) *
      Template_v3_08_2020a_P.RandomNumber1_StdDev_o +
      Template_v3_08_2020a_P.RandomNumber1_Mean_i;

    // End of InitializeConditions for RandomNumber: '<S662>/Random Number1'

    // InitializeConditions for Delay: '<S667>/Delay1'
    Template_v3_08_2020a_DW.icLoad_lv = 1U;

    // InitializeConditions for Delay: '<S670>/Delay1'
    Template_v3_08_2020a_DW.icLoad_hu = 1U;

    // InitializeConditions for RandomNumber: '<S662>/Random Number'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber_Seed_a);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_d = tseed;
    Template_v3_08_2020a_DW.NextOutput_o = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_d) *
      Template_v3_08_2020a_P.RandomNumber_StdDev +
      Template_v3_08_2020a_P.RandomNumber_Mean_e;

    // End of InitializeConditions for RandomNumber: '<S662>/Random Number'

    // InitializeConditions for Delay: '<S668>/Delay1'
    Template_v3_08_2020a_DW.icLoad_m = 1U;

    // InitializeConditions for RandomNumber: '<S662>/Random Number2'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber2_Seed_i);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_k = tseed;
    Template_v3_08_2020a_DW.NextOutput_j = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_k) *
      Template_v3_08_2020a_P.RandomNumber2_StdDev_p +
      Template_v3_08_2020a_P.RandomNumber2_Mean_c;

    // End of InitializeConditions for RandomNumber: '<S662>/Random Number2'

    // InitializeConditions for DiscreteIntegrator: '<S678>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[0] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S678>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[0] =
      Template_v3_08_2020a_P.drop_states_BLUE[0];

    // InitializeConditions for DiscreteIntegrator: '<S678>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[1] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S678>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[1] =
      Template_v3_08_2020a_P.drop_states_BLUE[1];

    // InitializeConditions for DiscreteIntegrator: '<S678>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_b[2] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_p;

    // InitializeConditions for DiscreteIntegrator: '<S678>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_a[2] =
      Template_v3_08_2020a_P.drop_states_BLUE[2];

    // InitializeConditions for RandomNumber: '<S678>/Random Number'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber_Seed_k);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_d3 = tseed;
    Template_v3_08_2020a_DW.NextOutput_g = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_d3) * sqrt
      (Template_v3_08_2020a_P.noise_variance_BLUE) +
      Template_v3_08_2020a_P.RandomNumber_Mean_b;

    // End of InitializeConditions for RandomNumber: '<S678>/Random Number'

    // InitializeConditions for Delay: '<S683>/Delay1'
    Template_v3_08_2020a_DW.icLoad_dc = 1U;

    // InitializeConditions for Delay: '<S681>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ln = 1U;

    // InitializeConditions for RandomNumber: '<S663>/Random Number4'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber4_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_dc = tseed;
    Temp_rt_nrand_Upu32_Yd_f_pw_snf(&Template_v3_08_2020a_DW.RandSeed_dc);

    // End of InitializeConditions for RandomNumber: '<S663>/Random Number4'

    // InitializeConditions for Delay: '<S679>/Delay1'
    Template_v3_08_2020a_DW.icLoad_oc = 1U;

    // InitializeConditions for Delay: '<S682>/Delay1'
    Template_v3_08_2020a_DW.icLoad_l1 = 1U;

    // InitializeConditions for RandomNumber: '<S663>/Random Number3'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber3_Seed_e);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_j = tseed;
    Temp_rt_nrand_Upu32_Yd_f_pw_snf(&Template_v3_08_2020a_DW.RandSeed_j);

    // End of InitializeConditions for RandomNumber: '<S663>/Random Number3'

    // InitializeConditions for Delay: '<S680>/Delay1'
    Template_v3_08_2020a_DW.icLoad_lz = 1U;

    // InitializeConditions for RandomNumber: '<S663>/Random Number5'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber5_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_c = tseed;
    Template_v3_08_2020a_DW.NextOutput_k = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_c) *
      Template_v3_08_2020a_P.RandomNumber5_StdDev +
      Template_v3_08_2020a_P.RandomNumber5_Mean;

    // End of InitializeConditions for RandomNumber: '<S663>/Random Number5'

    // InitializeConditions for DiscreteIntegrator: '<S664>/Discrete-Time Integrator' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator_PrevRese = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator_IC_LOADI = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S664>/Discrete-Time Integrator1' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevRes = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LOAD = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S664>/Discrete-Time Integrator2' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_PrevRes = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator2_IC_LOAD = 1U;

    // SystemInitialize for IfAction SubSystem: '<S667>/Hold this value'
    // SystemInitialize for Outport: '<S673>/Out1'
    Template_v3_08_2020a_B.In1_aw = Template_v3_08_2020a_P.Out1_Y0_i;

    // End of SystemInitialize for SubSystem: '<S667>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S668>/Hold this value'
    // SystemInitialize for Outport: '<S674>/Out1'
    Template_v3_08_2020a_B.In1_b2 = Template_v3_08_2020a_P.Out1_Y0_bu;

    // End of SystemInitialize for SubSystem: '<S668>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S669>/Hold this value'
    // SystemInitialize for Outport: '<S675>/Out1'
    Template_v3_08_2020a_B.In1_kz = Template_v3_08_2020a_P.Out1_Y0_jq;

    // End of SystemInitialize for SubSystem: '<S669>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S670>/Hold this value'
    // SystemInitialize for Outport: '<S676>/Out1'
    Template_v3_08_2020a_B.In1_id = Template_v3_08_2020a_P.Out1_Y0_mq;

    // End of SystemInitialize for SubSystem: '<S670>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S671>/Hold this value'
    // SystemInitialize for Outport: '<S677>/Out1'
    Template_v3_08_2020a_B.In1_es = Template_v3_08_2020a_P.Out1_Y0_eo;

    // End of SystemInitialize for SubSystem: '<S671>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S679>/Hold this value'
    // SystemInitialize for Outport: '<S685>/Out1'
    Template_v3_08_2020a_B.In1_pr = Template_v3_08_2020a_P.Out1_Y0_eom;

    // End of SystemInitialize for SubSystem: '<S679>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S680>/Hold this value'
    // SystemInitialize for Outport: '<S686>/Out1'
    Template_v3_08_2020a_B.In1_fv = Template_v3_08_2020a_P.Out1_Y0_d;

    // End of SystemInitialize for SubSystem: '<S680>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S683>/Hold this value'
    // SystemInitialize for Outport: '<S689>/Out1'
    Template_v3_08_2020a_B.In1_adc = Template_v3_08_2020a_P.Out1_Y0_ia;

    // End of SystemInitialize for SubSystem: '<S683>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S665>/RED + ARM'
    for (i = 0; i < 12; i++) {
      // InitializeConditions for Delay: '<S695>/Delay'
      Template_v3_08_2020a_DW.Delay_DSTATE[i] =
        Template_v3_08_2020a_P.Delay_InitialCondition;

      // InitializeConditions for DiscreteIntegrator: '<S695>/Acceleration  to Velocity' 
      Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_o[i] =
        Template_v3_08_2020a_P.AccelerationtoVelocity_IC[i];
    }

    // InitializeConditions for Delay: '<S700>/Delay1'
    Template_v3_08_2020a_DW.icLoad_nt = 1U;

    // InitializeConditions for RandomNumber: '<S695>/Random Number2'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber2_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_o = tseed;
    Template_v3_08_2020a_DW.NextOutput_c2 = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_o) *
      Template_v3_08_2020a_P.RandomNumber2_StdDev +
      Template_v3_08_2020a_P.RandomNumber2_Mean;

    // End of InitializeConditions for RandomNumber: '<S695>/Random Number2'

    // InitializeConditions for Delay: '<S699>/Delay1'
    Template_v3_08_2020a_DW.icLoad_el = 1U;

    // InitializeConditions for RandomNumber: '<S695>/Random Number1'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber1_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_p = tseed;
    Template_v3_08_2020a_DW.NextOutput_lz = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_p) *
      Template_v3_08_2020a_P.RandomNumber1_StdDev +
      Template_v3_08_2020a_P.RandomNumber1_Mean;

    // End of InitializeConditions for RandomNumber: '<S695>/Random Number1'

    // InitializeConditions for RandomNumber: '<S695>/Random Number3'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber3_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_h = tseed;
    Template_v3_08_2020a_DW.NextOutput_dv = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_h) *
      Template_v3_08_2020a_P.RandomNumber3_StdDev +
      Template_v3_08_2020a_P.RandomNumber3_Mean;

    // End of InitializeConditions for RandomNumber: '<S695>/Random Number3'

    // SystemInitialize for IfAction SubSystem: '<S699>/Hold this value'
    // SystemInitialize for Outport: '<S701>/Out1'
    Template_v3_08_2020a_B.In1_lg = Template_v3_08_2020a_P.Out1_Y0_h;

    // End of SystemInitialize for SubSystem: '<S699>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S700>/Hold this value'
    // SystemInitialize for Outport: '<S702>/Out1'
    Template_v3_08_2020a_B.In1_h = Template_v3_08_2020a_P.Out1_Y0_hv;

    // End of SystemInitialize for SubSystem: '<S700>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S665>/RED + ARM'

    // SystemInitialize for IfAction SubSystem: '<S665>/RED Only'
    // InitializeConditions for DiscreteIntegrator: '<S704>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[0] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S704>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[0] =
      Template_v3_08_2020a_P.drop_states_RED[0];

    // InitializeConditions for DiscreteIntegrator: '<S704>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[1] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S704>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[1] =
      Template_v3_08_2020a_P.drop_states_RED[1];

    // InitializeConditions for DiscreteIntegrator: '<S704>/Acceleration  to Velocity' 
    Template_v3_08_2020a_DW.AccelerationtoVelocity_DSTATE_m[2] =
      Template_v3_08_2020a_P.AccelerationtoVelocity_IC_c;

    // InitializeConditions for DiscreteIntegrator: '<S704>/Velocity to Position' 
    Template_v3_08_2020a_DW.VelocitytoPosition_DSTATE_f[2] =
      Template_v3_08_2020a_P.drop_states_RED[2];

    // InitializeConditions for RandomNumber: '<S704>/Random Number'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_mt = tseed;
    Template_v3_08_2020a_DW.NextOutput_lo = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_mt) * sqrt
      (Template_v3_08_2020a_P.noise_variance_RED) +
      Template_v3_08_2020a_P.RandomNumber_Mean;

    // End of InitializeConditions for RandomNumber: '<S704>/Random Number'

    // InitializeConditions for Delay: '<S706>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ec = 1U;

    // InitializeConditions for Delay: '<S708>/Delay1'
    Template_v3_08_2020a_DW.icLoad_by = 1U;

    // InitializeConditions for RandomNumber: '<S703>/Random Number7'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber7_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_ka = tseed;
    Template_v3_08_2020a_DW.NextOutput_de = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_ka) *
      Template_v3_08_2020a_P.RandomNumber7_StdDev +
      Template_v3_08_2020a_P.RandomNumber7_Mean;

    // End of InitializeConditions for RandomNumber: '<S703>/Random Number7'

    // InitializeConditions for Delay: '<S705>/Delay1'
    Template_v3_08_2020a_DW.icLoad_oe = 1U;

    // InitializeConditions for Delay: '<S709>/Delay1'
    Template_v3_08_2020a_DW.icLoad_mq = 1U;

    // InitializeConditions for RandomNumber: '<S703>/Random Number6'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber6_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_g = tseed;
    Template_v3_08_2020a_DW.NextOutput_c = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_g) *
      Template_v3_08_2020a_P.RandomNumber6_StdDev +
      Template_v3_08_2020a_P.RandomNumber6_Mean;

    // End of InitializeConditions for RandomNumber: '<S703>/Random Number6'

    // InitializeConditions for Delay: '<S707>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ov = 1U;

    // InitializeConditions for RandomNumber: '<S703>/Random Number8'
    Template_v3_08_2020a_B.d = floor(Template_v3_08_2020a_P.RandomNumber8_Seed);
    if (rtIsNaN(Template_v3_08_2020a_B.d) || rtIsInf(Template_v3_08_2020a_B.d))
    {
      Template_v3_08_2020a_B.d = 0.0;
    } else {
      Template_v3_08_2020a_B.d = fmod(Template_v3_08_2020a_B.d, 4.294967296E+9);
    }

    tseed = Template_v3_08_2020a_B.d < 0.0 ? static_cast<uint32_T>
      (-static_cast<int32_T>(static_cast<uint32_T>(-Template_v3_08_2020a_B.d))) :
      static_cast<uint32_T>(Template_v3_08_2020a_B.d);
    i = static_cast<int32_T>(tseed >> 16U);
    t = static_cast<int32_T>(tseed & 32768U);
    tseed = ((((tseed - (static_cast<uint32_T>(i) << 16U)) + t) << 16U) + t) + i;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    Template_v3_08_2020a_DW.RandSeed_f = tseed;
    Template_v3_08_2020a_DW.NextOutput_h = Temp_rt_nrand_Upu32_Yd_f_pw_snf
      (&Template_v3_08_2020a_DW.RandSeed_f) *
      Template_v3_08_2020a_P.RandomNumber8_StdDev +
      Template_v3_08_2020a_P.RandomNumber8_Mean;

    // End of InitializeConditions for RandomNumber: '<S703>/Random Number8'

    // SystemInitialize for IfAction SubSystem: '<S705>/Hold this value'
    // SystemInitialize for Outport: '<S711>/Out1'
    Template_v3_08_2020a_B.In1_kj = Template_v3_08_2020a_P.Out1_Y0_lt;

    // End of SystemInitialize for SubSystem: '<S705>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S706>/Hold this value'
    // SystemInitialize for Outport: '<S712>/Out1'
    Template_v3_08_2020a_B.In1_ad = Template_v3_08_2020a_P.Out1_Y0_f2;

    // End of SystemInitialize for SubSystem: '<S706>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S707>/Hold this value'
    // SystemInitialize for Outport: '<S713>/Out1'
    Template_v3_08_2020a_B.In1_k1 = Template_v3_08_2020a_P.Out1_Y0_fu;

    // End of SystemInitialize for SubSystem: '<S707>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S708>/Hold this value'
    // SystemInitialize for Outport: '<S714>/Out1'
    Template_v3_08_2020a_B.In1_pk = Template_v3_08_2020a_P.Out1_Y0_lh;

    // End of SystemInitialize for SubSystem: '<S708>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S709>/Hold this value'
    // SystemInitialize for Outport: '<S715>/Out1'
    Template_v3_08_2020a_B.In1_lk = Template_v3_08_2020a_P.Out1_Y0_a3;

    // End of SystemInitialize for SubSystem: '<S709>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S665>/RED Only'
    // End of SystemInitialize for SubSystem: '<Root>/Simulate Plant Dynamics'

    // SystemInitialize for IfAction SubSystem: '<S2>/Change Behavior'
    // Start for MATLABSystem: '<S23>/Digital Write'
    Template_v3_08_2020a_DW.obj_i.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_i.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_i.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_i.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_i.isInitialized = 1;
    MW_gpioInit(10U, true);
    Template_v3_08_2020a_DW.obj_i.isSetupComplete = true;

    // Start for MATLABSystem: '<S24>/Digital Write'
    Template_v3_08_2020a_DW.obj_a.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_a.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_a.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_a.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_a.isInitialized = 1;
    MW_gpioInit(26U, true);
    Template_v3_08_2020a_DW.obj_a.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S2>/Change Behavior'

    // SystemInitialize for IfAction SubSystem: '<S3>/Change RED Behavior'
    // Start for MATLABSystem: '<S27>/Send Torque Commands to Manipulator'
    Template_v3_08_2020a_DW.obj_j.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_j.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_j.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_j.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S3>/Change RED Behavior'

    // SystemInitialize for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks' 
    // Start for MATLABSystem: '<S33>/RED T1 - BLACK T8'
    Template_v3_08_2020a_DW.obj_m.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_m.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_m.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_m.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_m.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_m.PinNumber = 27U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_m.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_m.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_m.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T1 - BLACK T8'

    // Start for MATLABSystem: '<S33>/RED T2 - BLACK T3'
    Template_v3_08_2020a_DW.obj_ma.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_ma.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_ma.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_ma.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_ma.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_ma.PinNumber = 19U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_ma.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_ma.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_ma.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T2 - BLACK T3'

    // Start for MATLABSystem: '<S33>/RED T3'
    Template_v3_08_2020a_DW.obj_h.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_h.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_h.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_h.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_h.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_h.PinNumber = 4U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_h.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_h.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_h.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T3'

    // Start for MATLABSystem: '<S33>/RED T4 - BLACK T5'
    Template_v3_08_2020a_DW.obj_k.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_k.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_k.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_k.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_k.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_k.PinNumber = 22U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_k.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_k.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_k.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T4 - BLACK T5'

    // Start for MATLABSystem: '<S33>/RED T5 - BLACK T4'
    Template_v3_08_2020a_DW.obj_hw.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_hw.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_hw.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_hw.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_hw.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_hw.PinNumber = 5U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_hw.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_hw.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_hw.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T5 - BLACK T4'

    // Start for MATLABSystem: '<S33>/RED T6 - BLACK T7'
    Template_v3_08_2020a_DW.obj_hf.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_hf.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_hf.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_hf.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_hf.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_hf.PinNumber = 6U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_hf.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_hf.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_hf.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T6 - BLACK T7'

    // Start for MATLABSystem: '<S33>/RED T7 - BLACK T6'
    Template_v3_08_2020a_DW.obj_ce.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_ce.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_ce.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_ce.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_ce.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_ce.PinNumber = 13U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_ce.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_ce.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_ce.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T7 - BLACK T6'

    // Start for MATLABSystem: '<S33>/RED T8 - BLACK T1'
    Template_v3_08_2020a_DW.obj_f.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_f.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_f.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_f.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_f.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_f.PinNumber = 17U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_f.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_f.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_f.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/RED T8 - BLACK T1'

    // Start for MATLABSystem: '<S33>/BLACK T2'
    Template_v3_08_2020a_DW.obj_ci.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_ci.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_ci.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_ci.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_ci.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_ci.PinNumber = 9U;
    i = EXT_PWMBlock_init(Template_v3_08_2020a_DW.obj_ci.PinNumber, 10U, 0.0);
    if (i != 0) {
      for (i = 0; i < 45; i++) {
        Template_v3_08_2020a_B.cv[i] = tmp[i];
      }

      printf(Template_v3_08_2020a_B.cv, Template_v3_08_2020a_DW.obj_ci.PinNumber);
    }

    Template_v3_08_2020a_DW.obj_ci.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S33>/BLACK T2'
    // End of SystemInitialize for SubSystem: '<S4>/Send Commands to PWM Blocks' 

    // SystemInitialize for IfAction SubSystem: '<S4>/Change BLACK Behavior'
    // SystemInitialize for Outport: '<S29>/BLACK PWM'
    for (i = 0; i < 8; i++) {
      Template_v3_08_2020a_B.sf_MATLABFunction2.ThrustPer_Final[i] =
        Template_v3_08_2020a_P.BLACKPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S29>/BLACK PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S4>/Change BLUE Behavior'
    // SystemInitialize for Outport: '<S30>/BLUE PWM'
    for (i = 0; i < 8; i++) {
      Template_v3_08_2020a_B.sf_MATLABFunction2_n.ThrustPer_Final[i] =
        Template_v3_08_2020a_P.BLUEPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S30>/BLUE PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S4>/Change RED Behavior'
    // SystemInitialize for Outport: '<S31>/RED PWM'
    for (i = 0; i < 8; i++) {
      Template_v3_08_2020a_B.sf_MATLABFunction2_l.ThrustPer_Final[i] =
        Template_v3_08_2020a_P.REDPWM_Y0;
    }

    // End of SystemInitialize for Outport: '<S31>/RED PWM'
    // End of SystemInitialize for SubSystem: '<S4>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S5>/Use Hardware to Obtain States' 
    // Start for S-Function (sdspFromNetwork): '<S78>/UDP Receive'
    sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
    CreateUDPInterface(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
    if (*sErr == 0) {
      LibCreate_Network(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U], 0,
                        "0.0.0.0", Template_v3_08_2020a_P.UDPReceive_Port,
                        "0.0.0.0", -1, 8192, 8, 0);
    }

    if (*sErr == 0) {
      LibStart(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
    }

    if (*sErr != 0) {
      DestroyUDPInterface(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
        rtmSetStopRequested(Template_v3_08_2020a_M, 1);
      }
    }

    // End of Start for S-Function (sdspFromNetwork): '<S78>/UDP Receive'

    // InitializeConditions for Delay: '<S88>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ct = 1U;

    // InitializeConditions for Delay: '<S80>/Delay1'
    Template_v3_08_2020a_DW.icLoad_h0 = 1U;

    // InitializeConditions for Delay: '<S89>/Delay1'
    Template_v3_08_2020a_DW.icLoad_kt = 1U;

    // InitializeConditions for Delay: '<S81>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ktm = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S78>/Unwrap1'
    Template_v3_08_2020a_DW.Unwrap1_FirstStep = true;

    // InitializeConditions for Delay: '<S90>/Delay1'
    Template_v3_08_2020a_DW.icLoad_gn = 1U;

    // InitializeConditions for Delay: '<S82>/Delay1'
    Template_v3_08_2020a_DW.icLoad_pz = 1U;

    // InitializeConditions for Delay: '<S83>/Delay1'
    Template_v3_08_2020a_DW.icLoad_fy = 1U;

    // InitializeConditions for Delay: '<S79>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ea = 1U;

    // InitializeConditions for Delay: '<S91>/Delay1'
    Template_v3_08_2020a_DW.icLoad_df = 1U;

    // InitializeConditions for Delay: '<S86>/Delay1'
    Template_v3_08_2020a_DW.icLoad_pj = 1U;

    // InitializeConditions for Delay: '<S92>/Delay1'
    Template_v3_08_2020a_DW.icLoad_ap = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S78>/Unwrap'
    Template_v3_08_2020a_DW.Unwrap_FirstStep = true;

    // InitializeConditions for Delay: '<S87>/Delay1'
    Template_v3_08_2020a_DW.icLoad_hr = 1U;

    // InitializeConditions for Delay: '<S93>/Delay1'
    Template_v3_08_2020a_DW.icLoad_al = 1U;

    // InitializeConditions for Delay: '<S84>/Delay1'
    Template_v3_08_2020a_DW.icLoad_dt = 1U;

    // InitializeConditions for S-Function (sdspunwrap2): '<S78>/Unwrap2'
    Template_v3_08_2020a_DW.Unwrap2_FirstStep = true;

    // InitializeConditions for Delay: '<S85>/Delay1'
    Template_v3_08_2020a_DW.icLoad_l1b = 1U;

    // SystemInitialize for IfAction SubSystem: '<S79>/Hold this value'
    // SystemInitialize for Outport: '<S94>/Out1'
    Template_v3_08_2020a_B.In1_ky = Template_v3_08_2020a_P.Out1_Y0;

    // End of SystemInitialize for SubSystem: '<S79>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S80>/Hold this value'
    // SystemInitialize for Outport: '<S95>/Out1'
    Template_v3_08_2020a_B.In1_cw = Template_v3_08_2020a_P.Out1_Y0_p;

    // End of SystemInitialize for SubSystem: '<S80>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S81>/Hold this value'
    // SystemInitialize for Outport: '<S96>/Out1'
    Template_v3_08_2020a_B.In1_gp = Template_v3_08_2020a_P.Out1_Y0_a;

    // End of SystemInitialize for SubSystem: '<S81>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S82>/Hold this value'
    // SystemInitialize for Outport: '<S97>/Out1'
    Template_v3_08_2020a_B.In1_ng = Template_v3_08_2020a_P.Out1_Y0_pf;

    // End of SystemInitialize for SubSystem: '<S82>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S83>/Hold this value'
    // SystemInitialize for Outport: '<S98>/Out1'
    Template_v3_08_2020a_B.In1_i0 = Template_v3_08_2020a_P.Out1_Y0_k;

    // End of SystemInitialize for SubSystem: '<S83>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S84>/Hold this value'
    // SystemInitialize for Outport: '<S99>/Out1'
    Template_v3_08_2020a_B.In1_as = Template_v3_08_2020a_P.Out1_Y0_f;

    // End of SystemInitialize for SubSystem: '<S84>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S85>/Hold this value'
    // SystemInitialize for Outport: '<S100>/Out1'
    Template_v3_08_2020a_B.In1_lu = Template_v3_08_2020a_P.Out1_Y0_m;

    // End of SystemInitialize for SubSystem: '<S85>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S86>/Hold this value'
    // SystemInitialize for Outport: '<S101>/Out1'
    Template_v3_08_2020a_B.In1_f0 = Template_v3_08_2020a_P.Out1_Y0_po;

    // End of SystemInitialize for SubSystem: '<S86>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S87>/Hold this value'
    // SystemInitialize for Outport: '<S102>/Out1'
    Template_v3_08_2020a_B.In1_lb = Template_v3_08_2020a_P.Out1_Y0_n;

    // End of SystemInitialize for SubSystem: '<S87>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S88>/Hold this value'
    // SystemInitialize for Outport: '<S103>/Out1'
    Template_v3_08_2020a_B.In1_gw = Template_v3_08_2020a_P.Out1_Y0_pl;

    // End of SystemInitialize for SubSystem: '<S88>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S89>/Hold this value'
    // SystemInitialize for Outport: '<S104>/Out1'
    Template_v3_08_2020a_B.In1_k5 = Template_v3_08_2020a_P.Out1_Y0_aq;

    // End of SystemInitialize for SubSystem: '<S89>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S90>/Hold this value'
    // SystemInitialize for Outport: '<S105>/Out1'
    Template_v3_08_2020a_B.In1_ex = Template_v3_08_2020a_P.Out1_Y0_au;

    // End of SystemInitialize for SubSystem: '<S90>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S91>/Hold this value'
    // SystemInitialize for Outport: '<S106>/Out1'
    Template_v3_08_2020a_B.In1_fo = Template_v3_08_2020a_P.Out1_Y0_b;

    // End of SystemInitialize for SubSystem: '<S91>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S92>/Hold this value'
    // SystemInitialize for Outport: '<S107>/Out1'
    Template_v3_08_2020a_B.In1_db = Template_v3_08_2020a_P.Out1_Y0_bw;

    // End of SystemInitialize for SubSystem: '<S92>/Hold this value'

    // SystemInitialize for IfAction SubSystem: '<S93>/Hold this value'
    // SystemInitialize for Outport: '<S108>/Out1'
    Template_v3_08_2020a_B.In1_dn = Template_v3_08_2020a_P.Out1_Y0_c;

    // End of SystemInitialize for SubSystem: '<S93>/Hold this value'
    // End of SystemInitialize for SubSystem: '<S5>/Use Hardware to Obtain States' 

    // SystemInitialize for IfAction SubSystem: '<S6>/Change BLACK Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S121>/Discrete-Time Integrator1' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_Prev_hi = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_c = 1U;

    // SystemInitialize for IfAction SubSystem: '<S121>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_g,
      &Template_v3_08_2020a_DW.CalculateRunningMean_g,
      &Template_v3_08_2020a_P.CalculateRunningMean_g);

    // End of SystemInitialize for SubSystem: '<S121>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S121>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S130>/Out1'
    Template_v3_08_2020a_B.In1_i4 = Template_v3_08_2020a_P.Out1_Y0_bo;

    // End of SystemInitialize for SubSystem: '<S121>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S119>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean,
      &Template_v3_08_2020a_DW.CalculateRunningMean,
      &Template_v3_08_2020a_P.CalculateRunningMean);

    // End of SystemInitialize for SubSystem: '<S119>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S119>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S123>/Out1'
    Template_v3_08_2020a_B.In1_oj = Template_v3_08_2020a_P.Out1_Y0_e;

    // End of SystemInitialize for SubSystem: '<S119>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S120>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_i,
      &Template_v3_08_2020a_DW.CalculateRunningMean_i,
      &Template_v3_08_2020a_P.CalculateRunningMean_i);

    // End of SystemInitialize for SubSystem: '<S120>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S120>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S126>/Out1'
    Template_v3_08_2020a_B.In1_ai = Template_v3_08_2020a_P.Out1_Y0_by;

    // End of SystemInitialize for SubSystem: '<S120>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S132>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_e,
      &Template_v3_08_2020a_DW.CalculateRunningMean_e,
      &Template_v3_08_2020a_P.CalculateRunningMean_e);

    // End of SystemInitialize for SubSystem: '<S132>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S132>/Pass Current Accel'
    // SystemInitialize for Outport: '<S136>/Out1'
    Template_v3_08_2020a_B.In1_dm = Template_v3_08_2020a_P.Out1_Y0_b1;

    // End of SystemInitialize for SubSystem: '<S132>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S133>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_c,
      &Template_v3_08_2020a_DW.CalculateRunningMean_c,
      &Template_v3_08_2020a_P.CalculateRunningMean_c);

    // End of SystemInitialize for SubSystem: '<S133>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S133>/Pass Current Accel'
    // SystemInitialize for Outport: '<S139>/Out1'
    Template_v3_08_2020a_B.In1_j5 = Template_v3_08_2020a_P.Out1_Y0_ms;

    // End of SystemInitialize for SubSystem: '<S133>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S134>/Calculate Running Mean'
    CalculateRunningMean_o_Init(&Template_v3_08_2020a_B.CalculateRunningMean_ep,
      &Template_v3_08_2020a_DW.CalculateRunningMean_ep,
      &Template_v3_08_2020a_P.CalculateRunningMean_ep);

    // End of SystemInitialize for SubSystem: '<S134>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S134>/Pass Current Accel'
    // SystemInitialize for Outport: '<S142>/Out1'
    Template_v3_08_2020a_B.In1_opy = Template_v3_08_2020a_P.Out1_Y0_g;

    // End of SystemInitialize for SubSystem: '<S134>/Pass Current Accel'
    Template_v3_08_2_AHRS2_Init(Template_v3_08_2020a_B.DigitalFilter_a,
      &Template_v3_08_2020a_DW.AHRS2, &Template_v3_08_2020a_P.AHRS2);

    // End of SystemInitialize for SubSystem: '<S6>/Change BLACK Behavior'

    // SystemInitialize for IfAction SubSystem: '<S6>/Change BLUE Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S152>/Discrete-Time Integrator1' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_f = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_L_gl = 1U;

    // SystemInitialize for IfAction SubSystem: '<S152>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_l,
      &Template_v3_08_2020a_DW.CalculateRunningMean_l,
      &Template_v3_08_2020a_P.CalculateRunningMean_l);

    // End of SystemInitialize for SubSystem: '<S152>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S152>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S161>/Out1'
    Template_v3_08_2020a_B.In1_n3 = Template_v3_08_2020a_P.Out1_Y0_bf;

    // End of SystemInitialize for SubSystem: '<S152>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S150>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_d,
      &Template_v3_08_2020a_DW.CalculateRunningMean_d,
      &Template_v3_08_2020a_P.CalculateRunningMean_d);

    // End of SystemInitialize for SubSystem: '<S150>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S151>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_b,
      &Template_v3_08_2020a_DW.CalculateRunningMean_b,
      &Template_v3_08_2020a_P.CalculateRunningMean_b);

    // End of SystemInitialize for SubSystem: '<S151>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S163>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_n,
      &Template_v3_08_2020a_DW.CalculateRunningMean_n,
      &Template_v3_08_2020a_P.CalculateRunningMean_n);

    // End of SystemInitialize for SubSystem: '<S163>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S163>/Pass Current Accel'
    // SystemInitialize for Outport: '<S167>/Out1'
    Template_v3_08_2020a_B.In1_pf = Template_v3_08_2020a_P.Out1_Y0_jp;

    // End of SystemInitialize for SubSystem: '<S163>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S164>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_h,
      &Template_v3_08_2020a_DW.CalculateRunningMean_h,
      &Template_v3_08_2020a_P.CalculateRunningMean_h);

    // End of SystemInitialize for SubSystem: '<S164>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S164>/Pass Current Accel'
    // SystemInitialize for Outport: '<S170>/Out1'
    Template_v3_08_2020a_B.In1_np = Template_v3_08_2020a_P.Out1_Y0_nw;

    // End of SystemInitialize for SubSystem: '<S164>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S165>/Calculate Running Mean'
    CalculateRunningMean_o_Init(&Template_v3_08_2020a_B.CalculateRunningMean_o,
      &Template_v3_08_2020a_DW.CalculateRunningMean_o,
      &Template_v3_08_2020a_P.CalculateRunningMean_o);

    // End of SystemInitialize for SubSystem: '<S165>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S165>/Pass Current Accel'
    // SystemInitialize for Outport: '<S173>/Out1'
    Template_v3_08_2020a_B.In1_m0 = Template_v3_08_2020a_P.Out1_Y0_au3;

    // End of SystemInitialize for SubSystem: '<S165>/Pass Current Accel'
    Template_v3_08_2_AHRS2_Init(Template_v3_08_2020a_B.DigitalFilter_i,
      &Template_v3_08_2020a_DW.AHRS2_p, &Template_v3_08_2020a_P.AHRS2_p);

    // End of SystemInitialize for SubSystem: '<S6>/Change BLUE Behavior'

    // SystemInitialize for IfAction SubSystem: '<S6>/Change RED Behavior'
    // InitializeConditions for DiscreteIntegrator: '<S183>/Discrete-Time Integrator1' 
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_PrevR_h = 2;
    Template_v3_08_2020a_DW.DiscreteTimeIntegrator1_IC_LO_g = 1U;

    // SystemInitialize for IfAction SubSystem: '<S183>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_hn,
      &Template_v3_08_2020a_DW.CalculateRunningMean_hn,
      &Template_v3_08_2020a_P.CalculateRunningMean_hn);

    // End of SystemInitialize for SubSystem: '<S183>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S183>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S192>/Out1'
    Template_v3_08_2020a_B.In1_ed = Template_v3_08_2020a_P.Out1_Y0_fj;

    // End of SystemInitialize for SubSystem: '<S183>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S181>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_hx,
      &Template_v3_08_2020a_DW.CalculateRunningMean_hx,
      &Template_v3_08_2020a_P.CalculateRunningMean_hx);

    // End of SystemInitialize for SubSystem: '<S181>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S181>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S185>/Out1'
    Template_v3_08_2020a_B.In1_dy = Template_v3_08_2020a_P.Out1_Y0_b3;

    // End of SystemInitialize for SubSystem: '<S181>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S182>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_hq,
      &Template_v3_08_2020a_DW.CalculateRunningMean_hq,
      &Template_v3_08_2020a_P.CalculateRunningMean_hq);

    // End of SystemInitialize for SubSystem: '<S182>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S182>/Pass Current Gyro'
    // SystemInitialize for Outport: '<S188>/Out1'
    Template_v3_08_2020a_B.In1_a4 = Template_v3_08_2020a_P.Out1_Y0_l;

    // End of SystemInitialize for SubSystem: '<S182>/Pass Current Gyro'

    // SystemInitialize for IfAction SubSystem: '<S194>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_f,
      &Template_v3_08_2020a_DW.CalculateRunningMean_f,
      &Template_v3_08_2020a_P.CalculateRunningMean_f);

    // End of SystemInitialize for SubSystem: '<S194>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S194>/Pass Current Accel'
    // SystemInitialize for Outport: '<S198>/Out1'
    Template_v3_08_2020a_B.In1_eo = Template_v3_08_2020a_P.Out1_Y0_np;

    // End of SystemInitialize for SubSystem: '<S194>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S195>/Calculate Running Mean'
    T_CalculateRunningMean_Init(&Template_v3_08_2020a_B.CalculateRunningMean_o2,
      &Template_v3_08_2020a_DW.CalculateRunningMean_o2,
      &Template_v3_08_2020a_P.CalculateRunningMean_o2);

    // End of SystemInitialize for SubSystem: '<S195>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S195>/Pass Current Accel'
    // SystemInitialize for Outport: '<S201>/Out1'
    Template_v3_08_2020a_B.In1_pm = Template_v3_08_2020a_P.Out1_Y0_ki;

    // End of SystemInitialize for SubSystem: '<S195>/Pass Current Accel'

    // SystemInitialize for IfAction SubSystem: '<S196>/Calculate Running Mean'
    CalculateRunningMean_o_Init(&Template_v3_08_2020a_B.CalculateRunningMean_ot,
      &Template_v3_08_2020a_DW.CalculateRunningMean_ot,
      &Template_v3_08_2020a_P.CalculateRunningMean_ot);

    // End of SystemInitialize for SubSystem: '<S196>/Calculate Running Mean'

    // SystemInitialize for IfAction SubSystem: '<S196>/Pass Current Accel'
    // SystemInitialize for Outport: '<S204>/Out1'
    Template_v3_08_2020a_B.In1_jd = Template_v3_08_2020a_P.Out1_Y0_k2;

    // End of SystemInitialize for SubSystem: '<S196>/Pass Current Accel'
    Template_v3_08_2_AHRS2_Init(Template_v3_08_2020a_B.DigitalFilter,
      &Template_v3_08_2020a_DW.AHRS2_pn, &Template_v3_08_2020a_P.AHRS2_pn);

    // End of SystemInitialize for SubSystem: '<S6>/Change RED Behavior'

    // SystemInitialize for IfAction SubSystem: '<S8>/Change Behavior'
    // InitializeConditions for Delay: '<S208>/Delay1'
    Template_v3_08_2020a_DW.Delay1_DSTATE_jp =
      Template_v3_08_2020a_P.Delay1_InitialCondition;

    // InitializeConditions for Delay: '<S208>/Delay2'
    Template_v3_08_2020a_DW.Delay2_DSTATE[0] =
      Template_v3_08_2020a_P.Delay2_InitialCondition;
    Template_v3_08_2020a_DW.Delay2_DSTATE[1] =
      Template_v3_08_2020a_P.Delay2_InitialCondition;

    // InitializeConditions for Delay: '<S208>/Delay5'
    Template_v3_08_2020a_DW.Delay5_DSTATE[0] =
      Template_v3_08_2020a_P.Delay5_InitialCondition;
    Template_v3_08_2020a_DW.Delay5_DSTATE[1] =
      Template_v3_08_2020a_P.Delay5_InitialCondition;
    Template_v3_08_2020a_DW.Delay5_DSTATE[2] =
      Template_v3_08_2020a_P.Delay5_InitialCondition;

    // InitializeConditions for Delay: '<S208>/Delay3'
    Template_v3_08_2020a_DW.Delay3_DSTATE[0] =
      Template_v3_08_2020a_P.Delay3_InitialCondition;
    Template_v3_08_2020a_DW.Delay3_DSTATE[1] =
      Template_v3_08_2020a_P.Delay3_InitialCondition;
    Template_v3_08_2020a_DW.Delay3_DSTATE[2] =
      Template_v3_08_2020a_P.Delay3_InitialCondition;
    Template_v3_08_2020a_DW.Delay3_DSTATE[3] =
      Template_v3_08_2020a_P.Delay3_InitialCondition;

    // InitializeConditions for Delay: '<S209>/Delay1'
    Template_v3_08_2020a_DW.Delay1_DSTATE_n5 =
      Template_v3_08_2020a_P.Delay1_InitialCondition_j;

    // InitializeConditions for Delay: '<S209>/Delay2'
    Template_v3_08_2020a_DW.Delay2_DSTATE_l[0] =
      Template_v3_08_2020a_P.Delay2_InitialCondition_j;
    Template_v3_08_2020a_DW.Delay2_DSTATE_l[1] =
      Template_v3_08_2020a_P.Delay2_InitialCondition_j;

    // InitializeConditions for Delay: '<S209>/Delay5'
    Template_v3_08_2020a_DW.Delay5_DSTATE_b[0] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_g;
    Template_v3_08_2020a_DW.Delay5_DSTATE_b[1] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_g;
    Template_v3_08_2020a_DW.Delay5_DSTATE_b[2] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_g;

    // InitializeConditions for Delay: '<S209>/Delay3'
    Template_v3_08_2020a_DW.Delay3_DSTATE_o[0] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_k;
    Template_v3_08_2020a_DW.Delay3_DSTATE_o[1] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_k;
    Template_v3_08_2020a_DW.Delay3_DSTATE_o[2] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_k;
    Template_v3_08_2020a_DW.Delay3_DSTATE_o[3] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_k;

    // InitializeConditions for Delay: '<S210>/Delay1'
    Template_v3_08_2020a_DW.Delay1_DSTATE_ny =
      Template_v3_08_2020a_P.Delay1_InitialCondition_n;

    // InitializeConditions for Delay: '<S210>/Delay2'
    Template_v3_08_2020a_DW.Delay2_DSTATE_m[0] =
      Template_v3_08_2020a_P.Delay2_InitialCondition_p;
    Template_v3_08_2020a_DW.Delay2_DSTATE_m[1] =
      Template_v3_08_2020a_P.Delay2_InitialCondition_p;

    // InitializeConditions for Delay: '<S210>/Delay5'
    Template_v3_08_2020a_DW.Delay5_DSTATE_n[0] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_o;
    Template_v3_08_2020a_DW.Delay5_DSTATE_n[1] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_o;
    Template_v3_08_2020a_DW.Delay5_DSTATE_n[2] =
      Template_v3_08_2020a_P.Delay5_InitialCondition_o;

    // InitializeConditions for Delay: '<S210>/Delay3'
    Template_v3_08_2020a_DW.Delay3_DSTATE_h[0] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_e;
    Template_v3_08_2020a_DW.Delay3_DSTATE_h[1] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_e;
    Template_v3_08_2020a_DW.Delay3_DSTATE_h[2] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_e;
    Template_v3_08_2020a_DW.Delay3_DSTATE_h[3] =
      Template_v3_08_2020a_P.Delay3_InitialCondition_e;
    for (i = 0; i < 5; i++) {
      // InitializeConditions for Delay: '<S208>/Delay4'
      Template_v3_08_2020a_DW.Delay4_DSTATE[i] =
        Template_v3_08_2020a_P.Delay4_InitialCondition;

      // InitializeConditions for Delay: '<S209>/Delay4'
      Template_v3_08_2020a_DW.Delay4_DSTATE_c[i] =
        Template_v3_08_2020a_P.Delay4_InitialCondition_m;

      // InitializeConditions for Delay: '<S210>/Delay4'
      Template_v3_08_2020a_DW.Delay4_DSTATE_k[i] =
        Template_v3_08_2020a_P.Delay4_InitialCondition_h;
    }

    // Start for MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders' 
    Template_v3_08_2020a_DW.obj_c.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_c.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_c.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Template_v3_08_2020a_P.baseRate)) && (!rtIsNaN
          (Template_v3_08_2020a_P.baseRate))) || rtIsInf
        (Template_v3_08_2020a_P.baseRate)) {
      Template_v3_08_2020a_B.sampleTime_g = Template_v3_08_2020a_P.baseRate;
    }

    Template_v3_08_2020a_DW.obj_c.SampleTime =
      Template_v3_08_2020a_B.sampleTime_g;
    Template_v3_08_2020a_DW.obj_c.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_c.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_c.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders' 
    // End of SystemInitialize for SubSystem: '<S8>/Change Behavior'

    // SystemInitialize for IfAction SubSystem: '<S9>/Change RED Behavior'
    // Start for MATLABSystem: '<S216>/Initialization Block for Torque Control'
    Template_v3_08_2020a_DW.obj_g.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_g.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_g.matlabCodegenIsDeleted = false;
    Template_v3_08_2020a_DW.obj_g.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_g.isInitialized = 1;
    initialize_dynamixel_current_control(850.0);
    Template_v3_08_2020a_DW.obj_g.isSetupComplete = true;

    // End of SystemInitialize for SubSystem: '<S9>/Change RED Behavior'

    // Start for MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
    Template_v3_08_2020a_DW.obj.i2cobj_A_G.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj.i2cobj_MAG.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj.matlabCodegenIsDeleted = true;
    Templ_lsm9ds1Block_lsm9ds1Block(&Template_v3_08_2020a_DW.obj);
    Template_v3_08_SystemCore_setup(&Template_v3_08_2020a_DW.obj);

    // Start for MATLABSystem: '<S17>/WhoAmI'
    Template_v3_08_2020a_DW.obj_b.matlabCodegenIsDeleted = true;
    Template_v3_08_2020a_DW.obj_b.isInitialized = 0;
    Template_v3_08_2020a_DW.obj_b.matlabCodegenIsDeleted = false;
    if (((!rtIsInf(Template_v3_08_2020a_P.WhoAmI_SampleTime)) && (!rtIsNaN
          (Template_v3_08_2020a_P.WhoAmI_SampleTime))) || rtIsInf
        (Template_v3_08_2020a_P.WhoAmI_SampleTime)) {
      Template_v3_08_2020a_B.sampleTime_d =
        Template_v3_08_2020a_P.WhoAmI_SampleTime;
    }

    Template_v3_08_2020a_DW.obj_b.SampleTime =
      Template_v3_08_2020a_B.sampleTime_d;
    Template_v3_08_2020a_DW.obj_b.isSetupComplete = false;
    Template_v3_08_2020a_DW.obj_b.isInitialized = 1;
    Template_v3_08_2020a_DW.obj_b.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S17>/WhoAmI'
  }
}

// Model terminate function
void Template_v3_08_2020a_terminate(void)
{
  char_T *sErr;

  // Terminate for IfAction SubSystem: '<S2>/Change Behavior'
  // Terminate for MATLABSystem: '<S23>/Digital Write'
  matlabCodegenHandle_matlabCodeg(&Template_v3_08_2020a_DW.obj_i);

  // Terminate for MATLABSystem: '<S24>/Digital Write'
  matlabCodegenHandle_matlabCod_g(&Template_v3_08_2020a_DW.obj_a);

  // End of Terminate for SubSystem: '<S2>/Change Behavior'

  // Terminate for IfAction SubSystem: '<S3>/Change RED Behavior'
  // Terminate for MATLABSystem: '<S27>/Send Torque Commands to Manipulator'
  matlabCodegenHandle_matlabCo_g2(&Template_v3_08_2020a_DW.obj_j);

  // End of Terminate for SubSystem: '<S3>/Change RED Behavior'

  // Terminate for IfAction SubSystem: '<S5>/Use Hardware to Obtain States'
  // Terminate for S-Function (sdspFromNetwork): '<S78>/UDP Receive'
  sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
  LibTerminate(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
    rtmSetStopRequested(Template_v3_08_2020a_M, 1);
  }

  LibDestroy(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U], 0);
  DestroyUDPInterface(&Template_v3_08_2020a_DW.UDPReceive_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspFromNetwork): '<S78>/UDP Receive'
  // End of Terminate for SubSystem: '<S5>/Use Hardware to Obtain States'

  // Terminate for MATLABSystem: '<S6>/LSM9DS1 IMU Sensor'
  matlabCodegenHandl_g2ofinwajqob(&Template_v3_08_2020a_DW.obj);
  matlabCodegenHan_g2ofinwajqobno(&Template_v3_08_2020a_DW.obj.i2cobj_MAG);
  matlabCodegenHand_g2ofinwajqobn(&Template_v3_08_2020a_DW.obj.i2cobj_A_G);

  // Terminate for IfAction SubSystem: '<S8>/Change Behavior'
  // Terminate for MATLABSystem: '<S206>/Read Joint Positions using  Dynamixel Encoders' 
  matlabCodegenHa_g2ofinwajqobno3(&Template_v3_08_2020a_DW.obj_c);

  // End of Terminate for SubSystem: '<S8>/Change Behavior'

  // Terminate for IfAction SubSystem: '<S9>/Change RED Behavior'
  // Terminate for MATLABSystem: '<S216>/Initialization Block for Torque Control' 
  Template_v3_0_matlabCodegenHa_h(&Template_v3_08_2020a_DW.obj_g);

  // End of Terminate for SubSystem: '<S9>/Change RED Behavior'

  // Terminate for S-Function (sdspToNetwork): '<S18>/UDP Send'
  sErr = GetErrorBuffer(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
  LibTerminate(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(Template_v3_08_2020a_M, sErr);
    rtmSetStopRequested(Template_v3_08_2020a_M, 1);
  }

  LibDestroy(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U], 1);
  DestroyUDPInterface(&Template_v3_08_2020a_DW.UDPSend_NetworkLib[0U]);

  // End of Terminate for S-Function (sdspToNetwork): '<S18>/UDP Send'

  // Terminate for MATLABSystem: '<S17>/WhoAmI'
  Template_v3_0_matlabCodegenHa_d(&Template_v3_08_2020a_DW.obj_b);

  // Terminate for Atomic SubSystem: '<S4>/Send Commands to PWM Blocks'
  // Terminate for MATLABSystem: '<S33>/RED T1 - BLACK T8'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_m);

  // Terminate for MATLABSystem: '<S33>/RED T2 - BLACK T3'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_ma);

  // Terminate for MATLABSystem: '<S33>/RED T3'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_h);

  // Terminate for MATLABSystem: '<S33>/RED T4 - BLACK T5'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_k);

  // Terminate for MATLABSystem: '<S33>/RED T5 - BLACK T4'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_hw);

  // Terminate for MATLABSystem: '<S33>/RED T6 - BLACK T7'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_hf);

  // Terminate for MATLABSystem: '<S33>/RED T7 - BLACK T6'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_ce);

  // Terminate for MATLABSystem: '<S33>/RED T8 - BLACK T1'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_f);

  // Terminate for MATLABSystem: '<S33>/BLACK T2'
  matlabCodegenHandle_matlabC_g2o(&Template_v3_08_2020a_DW.obj_ci);

  // End of Terminate for SubSystem: '<S4>/Send Commands to PWM Blocks'
}
